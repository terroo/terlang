#include "Resolver.hpp"
#include "../utils/Debug.hpp"
#include "../parser/Expr.hpp"

Resolver::Resolver(Interpreter& interpreter) :
  interpreter(interpreter) {}

void Resolver::resolve(std::shared_ptr<Statement::Stmt> statement){
  statement->accept(*this);
}

void Resolver::resolve(std::shared_ptr<Expr> expression){
  expression->accept(*this);
}

void Resolver::beginScope(){
  identifiers.push_back(std::map<Token, int>{});
  scopes.push_back(std::map<std::string, bool>{});
}

void Resolver::endScope(){
  identifiers.pop_back();
  scopes.pop_back();
}

void Resolver::declare(Token& name){
  if(scopes.empty() && identifiers.empty()) return;
  std::map<Token, int> currentBlock = identifiers.back();
  std::map<std::string, bool> currentScope = scopes.back();
  if(currentScope.find(name.lexeme) != currentScope.end()){
    Debug::error(name, "Multiples variables with same name not allowed.");
  }
  currentBlock[name] = 0;
  currentScope[name.lexeme] = false;
}

void Resolver::define(Token& name){
  if(scopes.empty() && identifiers.empty()) return;
  scopes.back()[name.lexeme] = true;
}

void Resolver::resolveLocal(std::shared_ptr<Expr> expr, Token& name){
  int scopeSize = static_cast<int>(scopes.size()) - 1;
  for(int i = scopeSize; i >= 0; i--){
    if(scopes[static_cast<size_t>(i)].find(name.lexeme) != scopes[static_cast<size_t>(i)].end()) {
      identifiers[static_cast<size_t>(i)].erase(name);
      interpreter.resolve(expr, scopeSize - i);
      return;
    }
  }
}

void Resolver::resolveFunction(std::vector<Token> params, std::vector<std::shared_ptr<Statement::Stmt>> body, FType type){
  FType enclosingFunction = currentFunction;
  currentFunction = type;
  beginScope();
  for(Token& param : params){
    declare(param);
    define(param);
  }
  resolve(body);
  endScope();
  currentFunction = enclosingFunction;
}


void Resolver::resolve(std::vector<std::shared_ptr<Statement::Stmt>> &statements){
  for(std::shared_ptr<Statement::Stmt> &statement : statements){
    resolve(statement);
  }
}

std::any Resolver::visitBinaryExpr(std::shared_ptr<Binary> expr){
  resolve(expr->left);
  resolve(expr->right);
  return {};
}

std::any Resolver::visitGroupingExpr(std::shared_ptr<Grouping> expr){
  resolve(expr->expression);
  return {};
}

std::any Resolver::visitLiteralExpr(std::shared_ptr<Literal> expr){
  if(expr.get() != 0){}
  return {};
}

std::any Resolver::visitUnaryExpr(std::shared_ptr<Unary> expr){
  resolve(expr->right);
  return {};
}

std::any Resolver::visitExpressionStmt(std::shared_ptr<Statement::Expression> stmt){
  resolve(stmt->expression);
  return {};
}

std::any Resolver::visitPrintStmt(std::shared_ptr<Statement::Print> stmt){
  resolve(stmt->expression);
  return {};
}

std::any Resolver::visitOutStmt(std::shared_ptr<Statement::Out> stmt){
  resolve(stmt->expression);
  return {};
}

std::any Resolver::visitVariableExpr(std::shared_ptr<Variable> expr){
  if(!scopes.empty()){
    auto &currentScope = scopes.back();
    auto elem = currentScope.find(expr->name.lexeme);
    if(elem != currentScope.end() && elem->second == false){
      Debug::error(expr->name, "Can't read local variable in this own initializer.");
    }
  }
  resolveLocal(expr, expr->name);
  return {};
}

std::any Resolver::visitAssignExpr(std::shared_ptr<Assign> expr){
  resolve(expr->value);
  resolveLocal(expr, expr->name);
  return {};
}

std::any Resolver::visitLogicalExpr(std::shared_ptr<Logical> expr){
  resolve(expr->left);
  resolve(expr->right);
  return {};
}

std::any Resolver::visitCallExpr(std::shared_ptr<Call> expr){
  resolve(expr->callee);
  for(std::shared_ptr<Expr>& argument : expr->arguments){
    resolve(argument);
  }
  return {};
}

std::any Resolver::visitVarStmt(std::shared_ptr<Statement::Var> stmt){
  declare(stmt->name);
  if(stmt->init != nullptr) resolve(stmt->init);
  define(stmt->name);
  return {};
}

std::any Resolver::visitBlockStmt(std::shared_ptr<Statement::Block> stmt){
  beginScope();
  resolve(stmt->statements);
  endScope();
  return {};
}

std::any Resolver::visitIfStmt(std::shared_ptr<Statement::If> stmt){
  resolve(stmt->condition);
  resolve(stmt->thenBranch);
  if(stmt->elseBranch != nullptr) resolve(stmt->elseBranch);
  return {};
}

std::any Resolver::visitWhileStmt(std::shared_ptr<Statement::While> stmt){
  resolve(stmt->condition);
  resolve(stmt->body);
  return {};
}

std::any Resolver::visitFunctionStmt(std::shared_ptr<Statement::Function> stmt){
  declare(stmt->name);
  define(stmt->name);
  resolveFunction(stmt->params, stmt->body, FType::FUNCTION);
  return {};
}

std::any Resolver::visitReturnStmt(std::shared_ptr<Statement::Return> stmt){
  if(currentFunction == FType::NONE){
    Debug::error(stmt->keyword, "Can't return from top level code.");
  }
  if(stmt->value != nullptr) resolve(stmt->value);
  return {};
}

std::any Resolver::visitClassStmt(std::shared_ptr<Statement::Class> stmt){
  declare(stmt->name);
  define(stmt->name);
  
  for(const auto& method : stmt->methods){
    std::vector<Token> params = method->params;
    std::vector<std::shared_ptr<Statement::Stmt>> body = method->body;

    FType declaration = FType::METHOD;
    resolveFunction(params, body, declaration);
  }

  return {};
}

std::any Resolver::visitGetExpr(std::shared_ptr<Get> expr){
  resolve(expr->object);
  return {};
}

std::any Resolver::visitSetExpr(std::shared_ptr<Set> expr){
  resolve(expr->value);
  resolve(expr->object);
  return {};
}

std::any Resolver::visitIncludeStmt(std::shared_ptr<Statement::Include> stmt){
  if(stmt->path != ""){
    return {};
  }
  return {};
}

std::any Resolver::visitArrayExpr(std::shared_ptr<Array> expr) {
  for (std::shared_ptr<Expr> value : expr->values) {
    resolve(value);
  }
  return {};
}

std::any Resolver::visitCallistExpr(std::shared_ptr<Callist> expr) {
  resolve(expr->name);
  resolve(expr->index);
  if (expr->value != nullptr) resolve(expr->value);
  return {};
}
