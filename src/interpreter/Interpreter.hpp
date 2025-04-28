#pragma once

#include <cstdint>
#include <unordered_map>

#include "Environment.hpp"
#include "../parser/Stmt.hpp"

struct Return {
  std::any value;
};

class Interpreter : public ExprVisitor, public Statement::StmtVisitor {
  public:
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
    Interpreter();
    void interpret(std::vector<std::shared_ptr<Statement::Stmt>> &statements);
    void execute(std::shared_ptr<Statement::Stmt> statement);
    void executeBlock(const std::vector<std::shared_ptr<Statement::Stmt>> &statements, std::shared_ptr<Env> new_env);
    void resolve(std::shared_ptr<Expr> expr, int depth);

    std::any visitExpressionStmt(std::shared_ptr<Statement::Expression> stmt) override;
    std::any visitPrintStmt(std::shared_ptr<Statement::Print> stmt) override;   
    std::any visitOutStmt(std::shared_ptr<Statement::Out> stmt) override;   

    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
    std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
    std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override;
    std::any visitCallExpr(std::shared_ptr<Call> expr) override;
    std::any visitGetExpr(std::shared_ptr<Get> expr) override;
    std::any visitSetExpr(std::shared_ptr<Set> expr) override;
    std::any visitArrayExpr(std::shared_ptr<Array> expr) override;
    std::any visitCallistExpr(std::shared_ptr<Callist> expr) override;

    std::any visitVarStmt(std::shared_ptr<Statement::Var> stmt) override;
    std::any visitBlockStmt(std::shared_ptr<Statement::Block> stmt) override;
    std::any visitIfStmt(std::shared_ptr<Statement::If> stmt) override;
    std::any visitWhileStmt(std::shared_ptr<Statement::While> stmt) override;
    std::any visitFunctionStmt(std::shared_ptr<Statement::Function> stmt) override;
    std::any visitReturnStmt(std::shared_ptr<Statement::Return> stmt) override;
    std::any visitClassStmt(std::shared_ptr<Statement::Class> stmt) override;
    std::any visitIncludeStmt(std::shared_ptr<Statement::Include> stmt) override;

    std::shared_ptr<Env> global = std::make_shared<Env>();

  private:
    void checkNumberOperand(const Token& oper, const std::any& operand);
    void checkNumberOperands(const Token& oper, const std::any& left, const std::any& right);
    int64_t doubleToInt(const Token& oper, const std::any& value);
    bool isTruthy(const std::any& object);
    bool isEqual(const std::any& a, const std::any& b);
    std::string stringify(const std::any& object);
    std::any evaluate(std::shared_ptr<Expr> expr);

    std::unordered_map<std::shared_ptr<Expr>, int> locals;
    std::shared_ptr<Env> curr_env = global;
    std::any lookUpVariable(Token& name, std::shared_ptr<Expr> expr);
};

