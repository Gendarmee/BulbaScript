#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "Expr.hpp"
#include "RuntimeError.hpp"
#include "BS.hpp"
#include "Stmt.hpp"
#include "Environment.hpp"


class Interpritator : public NSStmt::Visitor, public NSExpr::Visitor {
public:
    void interpret(vector<shared_ptr<Stmt>> statements);
    Interpritator();
    shared_ptr<Environment> globals;
    void executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> newEnvironment);

private:
    shared_ptr<Environment> environment = globals;

    void execute(const std::shared_ptr<Stmt>& stmt);

    Object evaluate(const std::shared_ptr<Expr>& expr);
    wstring stringify(const Object& object);
    Object visit(const Return& stmt) override;
    Object visit(const Function& stmt) override;
    Object visit(const Call& expr) override;
    Object visit(const Assign& expr) override;
    Object visit(const If& stmt) override;
    Object visit(const Logical& expr) override;
    Object visit(const Expression& expr) override;
    Object visit(const Var& stmt) override;
    Object visit(const Block& stmt) override;
    Object visit(const Print& stmt) override;
    Object visit(const While& stmt) override;
    Object visit(const Variable& expr) override;
    Object visit(const Literal& literalExpr) override;
    Object visit(const Grouping& groupingExpr) override;
    Object visit(const Unary& unaryExpr) override;
    Object visit(const Binary& binaryExpr) override;

    void checkNumberOperand(const Token& opr, const Object& operand);
    void checkNumberOperands(const Token& opr, const Object& left, const Object& right);

    bool isEqual(const Object& a, const Object& b);
    bool isTruthy(const Object& obj);
};
