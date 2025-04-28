#pragma once

#include <vector>
#include <map>
#include "Interpreter.hpp"

class Resolver : public ExprVisitor, public Statement::StmtVisitor {

  private:
    Interpreter& interpreter;
    std::vector<std::map<std::string, bool>> scopes;
    std::vector<std::map<Token, int>> identifiers; 
    enum class FType{
      NONE,
      FUNCTION,
      METHOD
    };

    FType currentFunction = FType::NONE;

    void resolve(std::shared_ptr<Statement::Stmt> statement);
    void resolve(std::shared_ptr<Expr> expression);
    void beginScope();
    void endScope();
    void declare(Token& name);
    void define(Token& name);
    void resolveLocal(std::shared_ptr<Expr> expr, Token& name);
    void resolveFunction(std::vector<Token> params, std::vector<std::shared_ptr<Statement::Stmt>> body, FType type);

  public:
    Resolver(Interpreter &interpreter);
    void resolve(std::vector<std::shared_ptr<Statement::Stmt>> &statements);
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
    std::any visitExpressionStmt(std::shared_ptr<Statement::Expression> stmt) override;
    
    std::any visitPrintStmt(std::shared_ptr<Statement::Print> stmt) override;   
    std::any visitOutStmt(std::shared_ptr<Statement::Out> stmt) override;   

    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
    std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
    std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override;
    std::any visitCallExpr(std::shared_ptr<Call> expr) override;
    std::any visitVarStmt(std::shared_ptr<Statement::Var> stmt) override;
    std::any visitBlockStmt(std::shared_ptr<Statement::Block> stmt) override;
    std::any visitIfStmt(std::shared_ptr<Statement::If> stmt) override;
    std::any visitWhileStmt(std::shared_ptr<Statement::While> stmt) override;
    std::any visitFunctionStmt(std::shared_ptr<Statement::Function> stmt) override;
    std::any visitReturnStmt(std::shared_ptr<Statement::Return> stmt) override;
    std::any visitClassStmt(std::shared_ptr<Statement::Class> stmt) override;
    std::any visitIncludeStmt(std::shared_ptr<Statement::Include> stmt) override;
    std::any visitGetExpr(std::shared_ptr<Get> expr) override;
    std::any visitSetExpr(std::shared_ptr<Set> expr) override;
    std::any visitArrayExpr(std::shared_ptr<Array> expr) override;
    std::any visitCallistExpr(std::shared_ptr<Callist> expr) override;
};
