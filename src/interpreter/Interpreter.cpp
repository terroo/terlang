#include <cmath>
#include <iostream>

#include "Interpreter.hpp"
#include "BuiltinFactory.hpp"
#include "../parser/Expr.hpp"
#include "../utils/Debug.hpp"
#include "../parser/Stmt.hpp"
#include "Function.hpp"
#include "Class.hpp"
#include "Instance.hpp"
#include "ArrayType.hpp"  
#include "../utils/RuntimeError.hpp"

Interpreter::Interpreter(){}

/* Do not initialize built-ins in constructor.
   It was causing Static Initialization Order Fiasco (SIOF).
   Late initialization to be done manually before first use of the class. */
void Interpreter::lateInitializator()
{
  for(const auto& [name, type] : builtinNames){
    auto it = builtinFactory.find(type);
    if(it != builtinFactory.end()){
      global->define(name, it->second());
    }
  }
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr){
  return expr->value;
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr){
  int64_t i_right;
  std::any right = evaluate(expr->right);

  switch(expr->oper.type){

    case TokenType::PLUS_PLUS:
      checkNumberOperand(expr->oper, right);
      right = std::any_cast<double>(right) + 1;
      if (auto varExpr = std::dynamic_pointer_cast<Variable>(expr->right)) {
        curr_env->assign(varExpr->name, right);
      }
      if (expr->isPostOperator) {
        return std::any_cast<double>(right) - 1;
      }
      return right;

    case TokenType::MINUS_MINUS:
      checkNumberOperand(expr->oper, right);
      right = std::any_cast<double>(right) - 1;
      if (auto varExpr = std::dynamic_pointer_cast<Variable>(expr->right)) {
        curr_env->assign(varExpr->name, right);
      }
      if (expr->isPostOperator) {
        return std::any_cast<double>(right) + 1;
      }
      return right;

    case TokenType::BANG:
      return !isTruthy(right);
    case TokenType::MINUS:
      checkNumberOperand(expr->oper, right);
      return -std::any_cast<double>(right);
    case TokenType::TILDE:
      i_right = doubleToInt(expr->oper,right);
      return static_cast<double>(~i_right);
    default:
      return {};
  }
}

bool Interpreter::isTruthy(const std::any& object){
  if(object.type() == typeid(nullptr)) return false;
  if(object.type() == typeid(bool)){
    return std::any_cast<bool>(object);
  }
  if(object.type() == typeid(double)){
    return std::any_cast<double>(object) != 0.0;
  }
  if(object.type() == typeid(std::string)){
    return !std::any_cast<std::string>(object).empty();
  }
  // never
  return true;
}

void Interpreter::checkNumberOperand(const Token& oper, const std::any& operand){
  if(operand.type() == typeid(double)) return;
  throw RuntimeError{oper, "Operand must be a number."};
}

void Interpreter::checkNumberOperands(const Token& oper, const std::any& left, const std::any& right){
  if(left.type() == typeid(double) && right.type() == typeid(double)) return;
  throw RuntimeError{oper, "Operand must be a number."};
}

int64_t Interpreter::doubleToInt(const Token& oper, const std::any& value) {
  double integerPart;
  if(value.type() != typeid(double)) {
    throw RuntimeError{oper, "Operand must be a number."};
  }
  if (modf(std::any_cast<double>(value), &integerPart) != 0.0) {
    throw RuntimeError{oper, "Operand must be an integer."};
  }
  if (integerPart < static_cast<double>(std::numeric_limits<int64_t>::min()) ||
      integerPart > static_cast<double>(std::numeric_limits<int64_t>::max())) {
    throw RuntimeError{oper, "Value out of int64_t range"};
  }
  return static_cast<int64_t>(integerPart);
}

bool Interpreter::isEqual(const std::any& a, const std::any& b){
  if(a.type() == typeid(nullptr) && b.type() == typeid(nullptr)){
    return true;
  }

  if(a.type() == typeid(nullptr) || b.type() == typeid(nullptr)){
    return false;
  }

  if(a.type() == typeid(double) && b.type() == typeid(double)){
    return std::any_cast<double>(a) == std::any_cast<double>(b);
  }

  if(a.type() == typeid(std::string) && b.type() == typeid(std::string)){
    return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
  }

  if(a.type() == typeid(bool) && b.type() == typeid(bool)){
    return std::any_cast<bool>(a) == std::any_cast<bool>(b);
  }

  return false;
}

std::string Interpreter::stringify(const std::any& object){
  if(object.type() == typeid(nullptr)) return "nil";

  if(object.type() == typeid(double)){
    std::string text = std::to_string(std::any_cast<double>(object));
    if(text[text.length() - 7] == '.' && text[text.length() - 6] == '0'){
      text = text.substr(0, text.length() - 7);
    }
    return text;
  }

  if (object.type() == typeid(std::string)) {
    std::string result = std::any_cast<std::string>(object);

    // Replace the "\n" and "\r" sequences with real newlines and carriage returns
    size_t pos;
    while ((pos = result.find("\\n")) != std::string::npos) {
      result.replace(pos, 2, "\n");
    }
    while ((pos = result.find("\\r")) != std::string::npos) {
      result.replace(pos, 2, "\r");
    }

    return result;
  }

  if(object.type() == typeid(bool)){
    if(std::any_cast<bool>(object)){
      std::string result{"true"};
      return result;
    }else{
      std::string result{"false"};
      return result;
    }
  }

  if(object.type() == typeid(std::shared_ptr<Function>)){
    std::shared_ptr<Callable> function;
    function = std::any_cast<std::shared_ptr<Function>>(object);
    return function->toString();
  }

  if(object.type() == typeid(std::shared_ptr<Instance>)){
    auto instance = std::any_cast<std::shared_ptr<Instance>>(object);
    return instance->toString();
  }

  if(object.type() == typeid(std::shared_ptr<Callable>)){
    auto callable = std::any_cast<std::shared_ptr<Callable>>(object);
    return callable->toString();
  }

  if(object.type() == typeid(std::shared_ptr<Class>)){
    auto klass = std::any_cast<std::shared_ptr<Class>>(object);
    return klass->toString();
  }

  if(object.type() == typeid(std::shared_ptr<ArrayType>)){
    std::string result = "[";
    std::shared_ptr<ArrayType> list;
    list = std::any_cast<std::shared_ptr<ArrayType>>(object);
    auto values = list->values;
    for (auto i = values.begin(); i != values.end(); ++i) {
      auto next = i + 1;
      result.append(stringify(*i));
      if (next != values.end()) {
        result.append(", ");
      }
    }
    result.append("]");
    return result;
  }

  return "stringify: cannot reconize type";
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr){
  return evaluate(expr->expression);
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr){
  return expr->accept(*this);
}

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr){
  std::any left = evaluate(expr->left);
  std::any right = evaluate(expr->right);
  int64_t i_left, i_right;

  switch (expr->oper.type) {
    case TokenType::GREATER:
      checkNumberOperands(expr->oper, left, right);
      return std::any_cast<double>(left) > std::any_cast<double>(right);
    case TokenType::GREATER_EQUAL:
      checkNumberOperands(expr->oper, left, right);
      return std::any_cast<double>(left) >= std::any_cast<double>(right);
    case TokenType::GREATER_GREATER:
      i_left = doubleToInt(expr->oper,left);
      i_right = doubleToInt(expr->oper,right);
      return static_cast<double>(i_left >> i_right);
    case TokenType::LESS:
      checkNumberOperands(expr->oper, left, right);
      return std::any_cast<double>(left) < std::any_cast<double>(right);
    case TokenType::LESS_EQUAL:
      checkNumberOperands(expr->oper, left, right);
      return std::any_cast<double>(left) <= std::any_cast<double>(right);
    case TokenType::LESS_LESS:
      i_left = doubleToInt(expr->oper,left);
      i_right = doubleToInt(expr->oper,right);
      return static_cast<double>(i_left << i_right);
    case TokenType::MINUS:
      checkNumberOperands(expr->oper, left, right);
      return std::any_cast<double>(left) - std::any_cast<double>(right);
    case TokenType::PLUS:

      if(left.type() == typeid(double) && right.type() == typeid(double)){
        return std::any_cast<double>(left) + std::any_cast<double>(right);
      }

      if(left.type() == typeid(std::string) && right.type() == typeid(std::string)){
        return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
      }

      throw RuntimeError{expr->oper, "Operands not a same type"};

    case TokenType::PERCENT:
      checkNumberOperands(expr->oper, left, right);
      return fmod(std::any_cast<double>(left), std::any_cast<double>(right));
    case TokenType::AMPERSAND:
      i_left = doubleToInt(expr->oper,left);
      i_right = doubleToInt(expr->oper,right);
      return static_cast<double>(i_left & i_right);
    case TokenType::CARET:
      i_left = doubleToInt(expr->oper,left);
      i_right = doubleToInt(expr->oper,right);
      return static_cast<double>(i_left ^ i_right);
    case TokenType::VBAR:
      i_left = doubleToInt(expr->oper,left);
      i_right = doubleToInt(expr->oper,right);
      return static_cast<double>(i_left | i_right);
    case TokenType::STAR:
      checkNumberOperands(expr->oper, left, right);
      return std::any_cast<double>(left) * std::any_cast<double>(right);
    case TokenType::SLASH:
      checkNumberOperands(expr->oper, left, right);
      return std::any_cast<double>(left) / std::any_cast<double>(right);
    case TokenType::BANG_EQUAL:
      return !isEqual(left, right);
    case TokenType::EQUAL_EQUAL:
      return isEqual(left, right);
    default:
      return {};
  }
}

void Interpreter::interpret(std::vector<std::shared_ptr<Statement::Stmt>> &statements){
  try {
    for(std::shared_ptr<Statement::Stmt> &statement: statements){
      execute(statement);
    }
  }catch(const RuntimeError& e){
    Debug::runtimeError(e);
  }
}

void Interpreter::execute(std::shared_ptr<Statement::Stmt> statement){
  statement->accept(*this);
}

std::any Interpreter::visitExpressionStmt(std::shared_ptr<Statement::Expression> stmt){
  std::any value = evaluate(stmt->expression);
  return {};
}

std::any Interpreter::visitPrintStmt(std::shared_ptr<Statement::Print> stmt){
  std::any value = evaluate(stmt->expression);
  std::cout << stringify(value) << '\n';
  return {};
}

std::any Interpreter::visitOutStmt(std::shared_ptr<Statement::Out> stmt){
  std::any value = evaluate(stmt->expression);
  std::cout << stringify(value);
  return {};
}

std::any Interpreter::visitVariableExpr(std::shared_ptr<Variable> expr){
  std::any value = curr_env->get(expr->name);
  if(value.type() == typeid(nullptr)){
    throw RuntimeError(expr->name, "Variable not initialized.");
  }
  return curr_env->get(expr->name);
}

std::any Interpreter::visitVarStmt(std::shared_ptr<Statement::Var> stmt){
  std::any value = nullptr;
  if(stmt->init != nullptr){
    value = evaluate(stmt->init);
  }
  curr_env->define(stmt->name.lexeme, std::move(value));
  return {};
}

std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr){
  std::any value = evaluate(expr->value);
  auto elem = locals.find(expr);
  if(elem != locals.end()){
    int distance = elem->second;
    curr_env->assignAt(distance, expr->name, value);
  }else{
    global->assign(expr->name, value);
  }
  return value;
}


void Interpreter::executeBlock(
    const std::vector<std::shared_ptr<Statement::Stmt>> &statements, 
    std::shared_ptr<Env> new_env){

  std::shared_ptr<Env> previous = curr_env;
  try{
    curr_env = new_env;
    for(const std::shared_ptr<Statement::Stmt> &statement : statements){
      execute(statement);
    }
  }catch(...) {
    curr_env = previous;
    throw;
  }
  curr_env = previous;
}


std::any Interpreter::visitBlockStmt(std::shared_ptr<Statement::Block> stmt){
  executeBlock(stmt->statements, std::make_shared<Env>(curr_env));
  return {};
}

std::any Interpreter::visitIfStmt(std::shared_ptr<Statement::If> stmt){
  if(isTruthy(evaluate(stmt->condition))){
    execute(stmt->thenBranch);
  }else if(stmt->elseBranch != nullptr){
    execute(stmt->elseBranch);
  }
  return {};
}

std::any Interpreter::visitLogicalExpr(std::shared_ptr<Logical> expr){
  std::any left = evaluate(expr->left);
  if(expr->oper.type == TokenType::OR){
    if(isTruthy(left)) return left;
  }else{
    if(!isTruthy(left)) return left;
  }
  return evaluate(expr->right);
}


std::any Interpreter::visitWhileStmt(std::shared_ptr<Statement::While> stmt){
  while(isTruthy(evaluate(stmt->condition))){
    execute(stmt->body);
  }
  return {};
}

std::any Interpreter::visitCallExpr(std::shared_ptr<Call> expr){
  std::any callee = evaluate(expr->callee);
  std::vector<std::any> arguments;

  for(std::shared_ptr<Expr> &argument : expr->arguments){
    arguments.push_back(evaluate(argument));
  }

  // Check if it is a generic Callable
  if(callee.type() == typeid(std::shared_ptr<Callable>)){
    auto function = std::any_cast<std::shared_ptr<Callable>>(callee);
    return function->call(*this, arguments);
  }

  // Checks if it is a user-defined function
  if(callee.type() == typeid(std::shared_ptr<Function>)){
    auto function = std::any_cast<std::shared_ptr<Function>>(callee);
    return function->call(*this, arguments);
  }

  // Checks if it is a class (to instantiate objects)
  if(callee.type() == typeid(std::shared_ptr<Class>)){
    auto klass = std::any_cast<std::shared_ptr<Class>>(callee);
    return std::make_shared<Instance>(klass);
  }

  throw RuntimeError{expr->paren, "Can only call functions and classes."};
}


std::any Interpreter::visitFunctionStmt(std::shared_ptr<Statement::Function> stmt){
  auto function = std::make_shared<Function>(stmt, curr_env);
  curr_env->define(stmt->name.lexeme, function);
  return {};
}

std::any Interpreter::visitReturnStmt(std::shared_ptr<Statement::Return> stmt){
  std::any value = nullptr;
  if(stmt->value != nullptr){
    value = evaluate(stmt->value);
  }
  throw Return{value};
}

std::any Interpreter::lookUpVariable(Token& name, std::shared_ptr<Expr> expr){
  auto elem = locals.find(expr);
  if(elem != locals.end()){
    int distance = elem->second;
    return curr_env->getAt(distance, name.lexeme);
  }else{
    return global->get(name);
  }
}

void Interpreter::resolve(std::shared_ptr<Expr> expr, int depth){
  locals[expr] = depth;
}

std::any Interpreter::visitClassStmt(std::shared_ptr<Statement::Class> stmt){
  std::any value = nullptr;
  curr_env->define(stmt->name.lexeme, value);

  std::unordered_map<std::string, std::shared_ptr<Function>> methods;

  for(const auto &method : stmt->methods){
    auto function = std::make_shared<Function>(method, curr_env);
    methods[method->name.lexeme] = function;
  }

  auto klass = std::make_shared<Class>(stmt->name.lexeme, methods);
  curr_env->assign(stmt->name, klass);
  return {};
}

std::any Interpreter::visitGetExpr(std::shared_ptr<Get> expr){
  std::any object = evaluate(expr->object);
  if(object.type() == typeid(std::shared_ptr<Instance>)){
    auto instance = std::any_cast<std::shared_ptr<Instance>>(object);
    return instance->get(expr->name);
  }

  throw RuntimeError(expr->name, "Only instances have properties.");
}

std::any Interpreter::visitSetExpr(std::shared_ptr<Set> expr){
  std::any object = evaluate(expr->object);
  if(object.type() != typeid(std::shared_ptr<Instance>)){
    throw RuntimeError(expr->name, "Only instances have properties.");
  }
  std::any value = evaluate(expr->value);
  auto instance = std::any_cast<std::shared_ptr<Instance>>(object);
  instance->set(expr->name, value);
  return value;
}


std::any Interpreter::visitIncludeStmt(std::shared_ptr<Statement::Include> stmt){
  if(stmt->keyword.type != TokenType::INCLUDE){
    return {};
  }
  return {};
}

std::any Interpreter::visitArrayExpr(std::shared_ptr<Array> expr){
  auto list = std::make_shared<ArrayType>();
  for (std::shared_ptr<Expr> &value : expr->values) {
    list->append(evaluate(value));
  }
  return list;
}

std::any Interpreter::visitCallistExpr(std::shared_ptr<Callist> expr){
  std::any name = evaluate(expr->name);
  std::any index = evaluate(expr->index);
  if(name.type() == typeid(std::shared_ptr<ArrayType>)){
    if(index.type() == typeid(double)){
      std::shared_ptr<ArrayType> list;
      double castedIndex;
      list = std::any_cast<std::shared_ptr<ArrayType>>(name);
      castedIndex = std::any_cast<double>(index);
      if(expr->value != nullptr){
        std::any value = evaluate(expr->value);
        if(list->setAtIndex(static_cast<int>(castedIndex), value)) {
          return value; 
        }else{
          throw RuntimeError{expr->paren, "Index out of range."};
        }
      }else{
        if(castedIndex >= list->length() || castedIndex < 0){
          return nullptr;
        }
        return list->getEleAt(static_cast<int>(castedIndex));
      }
    }else{
      throw RuntimeError{expr->paren, "Index should be of type int."};
    }
  }else{
    throw RuntimeError{expr->paren, "Only arrays can be callist."};
  }
  return {};
}
