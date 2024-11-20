#pragma once

#include <vector>
#include <stdexcept>
#include "Scanner.hpp"
#include "Expr.hpp"
#include "Debug.hpp"
#include "Stmt.hpp"
#include "IncludeRun.hpp"

class Parser {
  private:
    struct ParseError : public std::runtime_error {
      using std::runtime_error::runtime_error;
    };

    //const std::vector<Token> tokens;
    std::vector<Token> tokens;
    int current = 0;
    std::vector<std::shared_ptr<Statement::Stmt>> statements;
    std::vector<std::string> includedFiles;

    ParseError error(const Token&, const std::string&);

    void synchronize();

    bool isAtEnd();
    bool check(const TokenType&);
    template<class...T>
    bool match(T...types);

    template<class...T>
    void matchVoid(T...types);

    Token previous();
    Token peek();
    Token advance();
    Token consume(const TokenType&, const std::string&);

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> logicalOr();
    std::shared_ptr<Expr> logicalAnd();
    std::shared_ptr<Expr> call();
    std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);
    std::shared_ptr<Expr> arrayList();
    std::shared_ptr<Expr> callist();
    std::shared_ptr<Expr> finishCallist(std::shared_ptr<Expr> name);

    std::shared_ptr<Statement::Stmt> statement();
    std::shared_ptr<Statement::Stmt> printStatement();
    std::shared_ptr<Statement::Stmt> outStatement();
    std::shared_ptr<Statement::Stmt> expressionStatement();
    std::shared_ptr<Statement::Stmt> declaration();
    std::shared_ptr<Statement::Stmt> varDeclaration();
    std::vector<std::shared_ptr<Statement::Stmt>> block();
    std::shared_ptr<Statement::Stmt> IfStatement();
    std::shared_ptr<Statement::Stmt> whileStatement();
    std::shared_ptr<Statement::Stmt> forStatement();
    std::shared_ptr<Statement::Stmt> returnStatement();
    std::shared_ptr<Statement::Stmt> classDeclaration();
    std::shared_ptr<Statement::Stmt> includeStatement();

    std::shared_ptr<Statement::Function> function(std::string kind);

  public:
    Parser(const std::vector<Token>&);
    std::vector<std::shared_ptr<Statement::Stmt>> parse();
};
