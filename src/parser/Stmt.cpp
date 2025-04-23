#include "Stmt.hpp"

namespace Statement {
  Expression::Expression(std::shared_ptr<Expr> expression) : expression(expression) {}

  std::any Expression::accept(StmtVisitor& visitor){
    return visitor.visitExpressionStmt(shared_from_this());
  }

  Print::Print(std::shared_ptr<Expr> expression) : expression(expression) {}

  std::any Print::accept(StmtVisitor& visitor){
    return visitor.visitPrintStmt(shared_from_this());
  }

  Out::Out(std::shared_ptr<Expr> expression) : expression(expression) {}

  std::any Out::accept(StmtVisitor& visitor){
    return visitor.visitOutStmt(shared_from_this());
  }

  Var::Var(Token name, std::shared_ptr<Expr> init) : name(name), init(init) {}

  std::any Var::accept(StmtVisitor &visitor){
    return visitor.visitVarStmt(shared_from_this());
  }

  Block::Block(std::vector<std::shared_ptr<Stmt>> statements) : statements(statements) {}

  std::any Block::accept(StmtVisitor &visitor){
    return visitor.visitBlockStmt(shared_from_this());
  }

  If::If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch,
      std::shared_ptr<Stmt> elseBranch) : condition{std::move(condition)},
    thenBranch{std::move(thenBranch)}, elseBranch{std::move(elseBranch)} {}

  std::any If::accept(StmtVisitor &visitor){
    return visitor.visitIfStmt(shared_from_this());
  }

  While::While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body) :
    condition{std::move(condition)}, body{std::move(body)} {}

  std::any While::accept(StmtVisitor &visitor){
    return visitor.visitWhileStmt(shared_from_this());
  }


  Function::Function(Token name, 
      std::vector<Token> params, 
      std::vector<std::shared_ptr<Stmt>> body) :
    name{std::move(name)},
    params{std::move(params)},
    body{std::move(body)} {}

  std::any Function::accept(StmtVisitor &visitor){
    return visitor.visitFunctionStmt(shared_from_this());
  }

  Return:: Return(Token keyword, std::shared_ptr<Expr> value) :
    keyword{std::move(keyword)}, value{std::move(value)} {}

  std::any Return::accept(StmtVisitor &visitor){
    return visitor.visitReturnStmt(shared_from_this());
  }


  Class::Class(Token name, std::vector<std::shared_ptr<Function>> methods) :
    name(name), methods(methods) {}

  std::any Class::accept(StmtVisitor &visitor){
    return visitor.visitClassStmt(shared_from_this());
  }

  Include::Include(Token keyword, std::string path) : 
    keyword{std::move(keyword)}, path{std::move(path)} {}

  std::any Include::accept(StmtVisitor &visitor){
    return visitor.visitIncludeStmt(shared_from_this());
  }
}
