#include "Parser.hpp"
#define assert(E)

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::shared_ptr<Statement::Stmt>> Parser::parse(){
  statements.clear();
  try {
    while(!isAtEnd()){
      statements.push_back(declaration());
    }
  }catch(const std::exception& e) {
    std::cerr << "[Exception parse]: " << e.what() << '\n'; 
  }
  return statements;
}

std::shared_ptr<Expr> Parser::expression(){
  return assignment();
}

std::shared_ptr<Expr> Parser::bitwise(){
  std::shared_ptr<Expr> expr = equality();
  while(match(TokenType::AMPERSAND, TokenType::CARET, TokenType::VBAR)){
    Token oper = previous();
    std::shared_ptr<Expr> right = equality();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::equality(){
  std::shared_ptr<Expr> expr = comparison();
  while(match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)){
    Token oper = previous();
    std::shared_ptr<Expr> right = comparison();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::comparison(){
  std::shared_ptr<Expr> expr = shift();
  while(match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)){
    Token oper = previous();
    std::shared_ptr<Expr> right = shift();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::shift(){
  std::shared_ptr<Expr> expr = term();
  while(match(TokenType::GREATER_GREATER, TokenType::LESS_LESS)){
    Token oper = previous();
    std::shared_ptr<Expr> right = term();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::term(){
  std::shared_ptr<Expr> expr = factor();
  while(match(TokenType::MINUS, TokenType::PLUS)){
    Token oper = previous();
    std::shared_ptr<Expr> right = factor();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::factor(){
  std::shared_ptr<Expr> expr = unary();
  while(match(TokenType::SLASH, TokenType::STAR, TokenType::PERCENT)){
    Token oper = previous();
    std::shared_ptr<Expr> right = unary();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::unary(){
  while(match(TokenType::BANG, TokenType::MINUS,
        TokenType::PLUS_PLUS, TokenType::MINUS_MINUS, TokenType::TILDE)){
    Token oper = previous(); // ++
    std::shared_ptr<Expr> right = unary(); // var name

    if(previous().lexeme == "++" || previous().lexeme == "--") { // to optimize
      Debug::error(previous(), "Invalid operator: postfix increment/decrement followed by prefix.");
    }
    matchVoid(TokenType::SEMICOLON);

    return std::make_shared<Unary>(oper, right, false);
  }
  return call();
}

std::shared_ptr<Expr> Parser::primary(){
  if(match(TokenType::LEFT_BRACE)) return arrayList();
  if(match(TokenType::FALSE)) return std::make_shared<Literal>(false);
  if(match(TokenType::TRUE)) return std::make_shared<Literal>(true);
  if(match(TokenType::NIL)) return std::make_shared<Literal>(nullptr);
  if(match(TokenType::IDENTIFIER)){ 
    std::shared_ptr<Expr> left = std::make_shared<Variable>(previous());
    if (match(TokenType::PLUS_PLUS, TokenType::MINUS_MINUS)) {
      Token oper = previous();
      matchVoid(TokenType::SEMICOLON);
      return std::make_shared<Unary>(oper, left, true);
    }
    return left;
  }

  if(match(TokenType::NUMBER, TokenType::STRING)){
    return std::make_shared<Literal>(previous().literal);
  }

  if(match(TokenType::LEFT_PAREN)){
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
    return std::make_shared<Grouping>(expr);
  }

  throw error(peek(), "Expected expression.");
}

template<class...T>
bool Parser::match(T...types){
  assert((... && std::is_same_v<T, TokenType>)); 
  if((... || check(types))){
    advance();
    return true;
  }
  return false;
}

template<class...T>
void Parser::matchVoid(T...types){
  assert((... && std::is_same_v<T, TokenType>)); 
  if((... || check(types))){
    advance();
  }
}

Token Parser::consume(const TokenType& token, const std::string& message){
  if(check(token)) return advance();
  throw error(peek(), message);
}

bool Parser::check(const TokenType& type){
  if(isAtEnd()) return false;
  return peek().type == type;
}

bool Parser::isAtEnd(){
  return peek().type == TokenType::TER_EOF;
}

Token Parser::advance(){
  if(!isAtEnd()) current++;
  return previous();
}

Token Parser::peek(){
  return tokens.at(static_cast<size_t>(current));
}

Token Parser::previous(){
  return tokens.at(static_cast<size_t>(current - 1));
}

Parser::ParseError Parser::error(const Token& token, const std::string& message){
  Debug::error(token, message);
  return ParseError("");
}

void Parser::synchronize(){
  advance();
  while(!isAtEnd()){
    if(previous().type == TokenType::SEMICOLON) return;
    if(previous().lexeme == "\n") return;
    switch (peek().type) {
      //case TokenType::INCLUDE:
      case TokenType::CLASS:
      case TokenType::SET:
      case TokenType::AUTO:
      case TokenType::FOR:
      case TokenType::IF:
      case TokenType::WHILE:
      case TokenType::OUT:
      case TokenType::OUTPUT:
      case TokenType::RETURN:
      default:
        return;
    }
  }
  advance();
}

std::shared_ptr<Statement::Stmt> Parser::statement(){
  if(match(TokenType::INCLUDE)) return includeStatement();
  if(match(TokenType::OUTPUT)) return printStatement();
  if(match(TokenType::OUT)) return outStatement();
  if(match(TokenType::IF)) return IfStatement();
  if(match(TokenType::RETURN)) return returnStatement();
  if(match(TokenType::WHILE)) return whileStatement();
  if(match(TokenType::FOR)) return forStatement();
  if(match(TokenType::LEFT_BRACE)) return std::make_shared<Statement::Block>(block());
  return expressionStatement();
}

std::shared_ptr<Statement::Stmt> Parser::printStatement(){
  std::shared_ptr<Expr> value = expression();
  //consume(TokenType::SEMICOLON, "Expected ';' after value."); // MOD
  matchVoid(TokenType::SEMICOLON);
  return std::make_shared<Statement::Print>(value);
}

std::shared_ptr<Statement::Stmt> Parser::outStatement(){
  std::shared_ptr<Expr> value = expression();
  //consume(TokenType::SEMICOLON, "Expected ';' after value.");
  matchVoid(TokenType::SEMICOLON);
  return std::make_shared<Statement::Out>(value);
}

std::shared_ptr<Statement::Stmt> Parser::expressionStatement(){
  std::shared_ptr<Expr> expr = expression();
  //consume(TokenType::SEMICOLON, "Expected ';' after value.");
  return std::make_shared<Statement::Expression>(expr);
}

std::shared_ptr<Statement::Stmt> Parser::declaration(){
  try {
    if(match(TokenType::SET)) return function("function");
    if(match(TokenType::CLASS)) return classDeclaration();
    if(match(TokenType::AUTO)) return varDeclaration();
    return statement();
  } catch (const std::exception& e) {
    synchronize();
    return nullptr;
  }
}

std::shared_ptr<Statement::Stmt> Parser::varDeclaration(){
  Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");
  std::shared_ptr<Expr> init = nullptr;
  if(match(TokenType::EQUAL)){
    init = expression();
  }
  //consume(TokenType::SEMICOLON, "Expected ';' after variable declaration."); // MOD
  matchVoid(TokenType::SEMICOLON);
  return std::make_shared<Statement::Var>(name, init);
}

std::shared_ptr<Expr> Parser::assignment(){
  std::shared_ptr<Expr> expr = logicalOr();
  if(match(TokenType::EQUAL)){
    Token equals = previous();
    std::shared_ptr<Expr> value = assignment();
    if(Variable *e = dynamic_cast<Variable*>(expr.get())){
      Token name = e->name;
      return std::make_shared<Assign>(std::move(name), value);
    }else if(Get *get = dynamic_cast<Get*>(expr.get())){
      return std::make_shared<Set>(get->object, get->name, value);
    }else if (Callist *s = dynamic_cast<Callist*>(expr.get())) {
      return std::make_shared<Callist>(s->name, s->index, value, s->paren);
    }
    error(std::move(equals), "Invalid assignment target.");
  }
  return expr;
}

std::vector<std::shared_ptr<Statement::Stmt>> Parser::block(){
  std::vector<std::shared_ptr<Statement::Stmt>> localStatements;
  while(!check(TokenType::RIGHT_BRACE) && !isAtEnd()){
    localStatements.push_back(declaration());
  }
  consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
  return localStatements;
}

std::shared_ptr<Statement::Stmt> Parser::IfStatement(){
  consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
  std::shared_ptr<Expr> condition = expression();
  consume(TokenType::RIGHT_PAREN, "Expected ')' after 'if' condition.");
  std::shared_ptr<Statement::Stmt> thenBranch = statement();
  std::shared_ptr<Statement::Stmt> elseBranch = nullptr;
  if(match(TokenType::ELSE)){
    elseBranch = statement();
  }
  return std::make_shared<Statement::If>(condition, thenBranch, elseBranch);
  return {};
}

std::shared_ptr<Expr> Parser::logicalOr(){
  std::shared_ptr<Expr> expr = logicalAnd();
  while(match(TokenType::OR)){
    Token oper = previous();
    std::shared_ptr<Expr> right = logicalAnd();
    expr = std::make_shared<Logical>(expr, std::move(oper), right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::logicalAnd(){
  std::shared_ptr<Expr> expr = bitwise();
  while(match(TokenType::AND)){
    Token oper = previous();
    std::shared_ptr<Expr> right = bitwise();
    expr = std::make_shared<Logical>(expr, std::move(oper), right);
  }
  return expr;
}

std::shared_ptr<Statement::Stmt> Parser::whileStatement(){
  consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
  std::shared_ptr<Expr> condition = expression();
  consume(TokenType::RIGHT_PAREN, "Expected ')' after 'while' condition.");
  std::shared_ptr<Statement::Stmt> body = statement();
  return std::make_shared<Statement::While>(condition, body);
}

std::shared_ptr<Statement::Stmt> Parser::forStatement(){
  consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");

  std::shared_ptr<Statement::Stmt> init;
  if(match(TokenType::SEMICOLON)){
    init = nullptr;
  }else if(match(TokenType::AUTO)){
    init = varDeclaration();
  }else{
    init = expressionStatement();
  }

  std::shared_ptr<Expr> condition = nullptr;
  if(!check(TokenType::SEMICOLON)){
    condition = expression();
  }
  consume(TokenType::SEMICOLON, "Expected ';' after for condition.");

  std::shared_ptr<Expr> increment = nullptr;
  if(!check(TokenType::RIGHT_PAREN)){
    increment = expression();
  }
  consume(TokenType::RIGHT_PAREN, "Expected ')' after loop condition.");

  std::shared_ptr<Statement::Stmt> body = statement();
  if(increment != nullptr){
    body = std::make_shared<Statement::Block>(
        std::vector<std::shared_ptr<Statement::Stmt>> {
        body, std::make_shared<Statement::Expression>(increment)
        } 
        );
  }

  if(condition == nullptr){
    condition = std::make_shared<Literal>(true);
  }
  body = std::make_shared<Statement::While>(condition, body);

  if(init != nullptr){
    body = std::make_shared<Statement::Block>(
        std::vector<std::shared_ptr<Statement::Stmt>>{
        init, body
        }
        );
  }

  return body;
}

std::shared_ptr<Expr> Parser::call(){
  //std::shared_ptr<Expr> expr = primary();
  std::shared_ptr<Expr> expr = callist();
  while(true){
    if(match(TokenType::LEFT_PAREN)){
      expr = finishCall(expr);
    }else if(match(TokenType::DOT)){
      Token name = consume(TokenType::IDENTIFIER, "Expected property name after '.'");
      expr = std::make_shared<Get>(expr, name);
    }else{
      break;
    }
  }
  return expr;
}

std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee){
  std::vector<std::shared_ptr<Expr>> arguments;
  if(!check(TokenType::RIGHT_PAREN)){
    do {
      if(arguments.size() >= 255){
        error(peek(), "Can't have more than 255 arguments.");
      }
      arguments.push_back(expression());
    } while(match(TokenType::COMMA));
  }
  Token paren = consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");

  matchVoid(TokenType::SEMICOLON);
  return std::make_shared<Call>(callee, paren, arguments);
}

std::shared_ptr<Statement::Stmt> Parser::returnStatement(){
  Token keyword = previous();
  std::shared_ptr<Expr> value = nullptr;
  //if(!check(TokenType::SEMICOLON)){
  value = expression();
  matchVoid(TokenType::SEMICOLON);
  //}
  //consume(TokenType::SEMICOLON, "Expected ';' after return value.");
  return std::make_shared<Statement::Return>(keyword, value);
}

std::shared_ptr<Statement::Function> Parser::function(std::string kind){
  Token funcName = consume(TokenType::IDENTIFIER, "Expected " + kind + " name.");
  consume(TokenType::LEFT_PAREN, "Expected '(' after " + kind + " name.");
  std::vector<Token> parameters;
  if(!check(TokenType::RIGHT_PAREN)){
    do {
      if(parameters.size() >= 255){
        error(peek(), "Can't have more than 255 arguments.");
      }
      parameters.push_back(
          consume(TokenType::IDENTIFIER, "Expected parameter name.")
          );
    } while(match(TokenType::COMMA));
  }
  consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");
  consume(TokenType::LEFT_BRACE, "Expected '{' before " + kind + " body.");
  std::vector<std::shared_ptr<Statement::Stmt>> body = block();
  return std::make_shared<Statement::Function>(
      std::move(funcName), std::move(parameters), std::move(body)
      );
}

std::shared_ptr<Statement::Stmt> Parser::classDeclaration(){
  Token name = consume(TokenType::IDENTIFIER, "Expected class name");
  consume(TokenType::LEFT_BRACE, "Expected '{' before class body.");

  std::vector<std::shared_ptr<Statement::Function>> methods;
  while(!check(TokenType::RIGHT_BRACE) && !isAtEnd()){
    methods.push_back(function("method"));
  }
  consume(TokenType::RIGHT_BRACE, "Expected '}' after class body");
  return std::make_shared<Statement::Class>(name, std::move(methods));
}

std::shared_ptr<Statement::Stmt> Parser::includeStatement() {
  Token keyword = previous();

  consume(TokenType::LEFT_PAREN, "Expect '(' after 'include' keyword.");
  Token path = consume(TokenType::STRING, "Expect 'path' as a string inside 'include' statement.");
  consume(TokenType::RIGHT_PAREN, "Expect ')' after path in 'include' statement.");
  matchVoid(TokenType::SEMICOLON);

  if (std::find(includedFiles.begin(), includedFiles.end(), path.lexeme) != includedFiles.end()) {
    Debug::error(path, "Duplicate header file.");
  }
  includedFiles.push_back(path.lexeme);

  IncludeRun::scanFile(path.lexeme);
  const std::vector<Token>& tempTokens = IncludeRun::getTokens();

  Parser includedParser(tempTokens);
  std::vector<std::shared_ptr<Statement::Stmt>> includedStatements = includedParser.parse();

  for (const auto& stmt : includedStatements) {
    statements.push_back(stmt);
  }

  return std::make_shared<Statement::Include>(keyword, path.lexeme);
}

std::shared_ptr<Expr> Parser::arrayList() {
  std::vector<std::shared_ptr<Expr>> values = {};
  if (match(TokenType::RIGHT_BRACE)) {
    return std::make_shared<Array>(values);
  } else {
    do {
      if (values.size() >= 255) {
        error(peek(), "Can't have more than 255 elements in a array.");
      }
      std::shared_ptr<Expr> value = logicalOr();
      values.push_back(value);
    } while (match(TokenType::COMMA));
  }
  consume(TokenType::RIGHT_BRACE, "Expect '}' at end of array.");
  return std::make_shared<Array>(values);
}

std::shared_ptr<Expr> Parser::finishCallist(std::shared_ptr<Expr> name) {
  std::shared_ptr<Expr> index = logicalOr();
  Token paren = consume(TokenType::RIGHT_BRACKET,
      "Expect ']' after arguments.");
  return std::make_shared<Callist>(name, index, nullptr, paren);
}

std::shared_ptr<Expr> Parser::callist() {
  std::shared_ptr<Expr> expr = primary();
  while (true) {
    if (match(TokenType::LEFT_BRACKET)) {
      expr = finishCallist(expr);
    } else {
      break;
    }
  }
  return expr;
}
