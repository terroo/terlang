#pragma once

#include <utility>
#include <vector>
#include "Visitor.hpp"
#include "Token.hpp"

struct Binary final : Expr, public std::enable_shared_from_this<Binary> {
  std::shared_ptr<Expr> left;
  Token oper;
  std::shared_ptr<Expr> right;

  Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right);
  std::any accept(ExprVisitor &visitor) override;
  ~Binary() = default;
};

struct Grouping final: Expr, public std::enable_shared_from_this<Grouping> {
  std::shared_ptr<Expr> expression;

  Grouping(std::shared_ptr<Expr> expression);
  std::any accept(ExprVisitor &visitor) override;
  ~Grouping() = default;
};

struct Literal final: Expr, public std::enable_shared_from_this<Literal> {
  std::any value;

  Literal(std::any value);
  std::any accept(ExprVisitor &visitor) override;
  ~Literal() = default;
};

struct Unary final: Expr, public std::enable_shared_from_this<Unary> {
  Token oper;
  std::shared_ptr<Expr> right;
  bool isPostOperator;

  Unary(Token oper, std::shared_ptr<Expr> right, bool isPostOperator);
  std::any accept(ExprVisitor &visitor) override;
  ~Unary() = default;
};

struct Variable final: Expr, public std::enable_shared_from_this<Variable> {
  Token name;
  Variable(Token name);
  std::any accept(ExprVisitor &visitor) override;
  ~Variable() = default;
};

struct Assign final: Expr, public std::enable_shared_from_this<Assign> {
  Token name;
  std::shared_ptr<Expr> value;

  Assign(Token name, std::shared_ptr<Expr> value);
  std::any accept(ExprVisitor &visitor) override;
  ~Assign() = default;
};

struct Logical final: Expr, public std::enable_shared_from_this<Logical> {
  std::shared_ptr<Expr> left;
  Token oper;
  std::shared_ptr<Expr> right;
  
  Logical(std::shared_ptr<Expr> left,
      Token oper, 
     std::shared_ptr<Expr> right );
  
  std::any accept(ExprVisitor &visitor) override;
  ~Logical() = default;
};

struct Call final: Expr, public std::enable_shared_from_this<Call> {
  std::shared_ptr<Expr> callee;
  Token paren;
  std::vector<std::shared_ptr<Expr>> arguments;

  Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments);
  std::any accept(ExprVisitor &visitor) override;
  ~Call() = default;
};

struct Get final: Expr, public std::enable_shared_from_this<Get> {
  std::shared_ptr<Expr> object;
  Token name;

  Get(std::shared_ptr<Expr> object, Token name);
  std::any accept(ExprVisitor &visitor) override;
  ~Get() = default;
};


struct Set final: Expr, public std::enable_shared_from_this<Set> {
  std::shared_ptr<Expr> object;
  Token name;
  std::shared_ptr<Expr> value;

  Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value);
  std::any accept(ExprVisitor &visitor) override;
  ~Set() = default;
};

struct Array final: Expr, public std::enable_shared_from_this<Array> {
  std::vector<std::shared_ptr<Expr>> values;

  Array(std::vector<std::shared_ptr<Expr>> values);
  std::any accept(ExprVisitor &visitor) override;
  ~Array() = default;
};

struct Callist final: Expr, public std::enable_shared_from_this<Callist> {
  std::shared_ptr<Expr> name;
  std::shared_ptr<Expr> index;
  std::shared_ptr<Expr> value;
  Token paren;

  Callist(std::shared_ptr<Expr> name, std::shared_ptr<Expr> index, std::shared_ptr<Expr> value, Token paren);
  std::any accept(ExprVisitor &visitor) override;
  ~Callist() = default;
};
