#pragma once

#include "Visitor.hpp"
#include "Token.hpp"
#include <vector>

namespace Statement {

  struct Expression final: Stmt, public std::enable_shared_from_this<Expression>{
    const std::shared_ptr<Expr> expression;

    Expression(std::shared_ptr<Expr> expression);
    std::any accept(StmtVisitor& visitor) override;
    ~Expression() =  default;
  };

  struct Print final: Stmt, public std::enable_shared_from_this<Print>{
    const std::shared_ptr<Expr> expression;

    Print(std::shared_ptr<Expr> expression);
    std::any accept(StmtVisitor& visitor) override;
    ~Print() = default;
  };

  struct Out final: Stmt, public std::enable_shared_from_this<Out>{
    const std::shared_ptr<Expr> expression;

    Out(std::shared_ptr<Expr> expression);
    std::any accept(StmtVisitor& visitor) override;
    ~Out() = default;
  };

  struct Var final: Stmt, public std::enable_shared_from_this<Var> {
    Token name;
    std::shared_ptr<Expr> init;

    Var(Token name, std::shared_ptr<Expr> init);
    std::any accept(StmtVisitor &visitor) override;
    ~Var() = default;
  };

  struct Block final: Stmt, public std::enable_shared_from_this<Block> {
    std::vector<std::shared_ptr<Stmt>> statements;

    Block(std::vector<std::shared_ptr<Stmt>> statements);
    std::any accept(StmtVisitor &visitor) override;
    ~Block() = default;
  };

  struct If final: Stmt, public std::enable_shared_from_this<If> {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;

    If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch,
        std::shared_ptr<Stmt> elseBranch);
    std::any accept(StmtVisitor &visitor) override;
    ~If() = default;
  };

  struct While final: Stmt, public std::enable_shared_from_this<While> {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;

    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);
    std::any accept(StmtVisitor& visitor) override;
    ~While() = default;
  };

  struct Function final: Stmt, public std::enable_shared_from_this<Function> {
    Token name;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;
    Function(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body);
    std::any accept(StmtVisitor &visitor) override;
    ~Function() = default;
  };

  struct Return final: Stmt, public std::enable_shared_from_this<Return> {
    Token keyword;
    std::shared_ptr<Expr> value;
    Return(Token keyword, std::shared_ptr<Expr> value);
    std::any accept(StmtVisitor &visitor) override;
  };

  struct Class final: Stmt, public std::enable_shared_from_this<Class> {
    Token name;
    std::vector<std::shared_ptr<Function>> methods;

    Class(Token name, std::vector<std::shared_ptr<Function>> methods);
    std::any accept(StmtVisitor& visitor) override;
    ~Class() = default;
  };

  struct Include final: Stmt, public std::enable_shared_from_this<Include> {
    Token keyword;
    std::string path;

    Include(Token keyword, std::string path);
    std::any accept(StmtVisitor& visitor) override;
    ~Include() = default;
  };

}

