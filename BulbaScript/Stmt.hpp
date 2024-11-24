#pragma once

#include <memory>
#include <vector>
#include <variant>
#include "Object.hpp"
#include "Token.hpp"
#include "Expr.hpp"
class Block;
class Expression;
class Print;
class Var;
namespace NSStmt {
class Visitor {
  public:
      virtual Object visit(const Block& BlockExpr) = 0;
      virtual Object visit(const Expression& ExpressionExpr) = 0;
      virtual Object visit(const Print& PrintExpr) = 0;
      virtual Object visit(const Var& VarExpr) = 0;
};
} // namespace Stmt

class Stmt {
public:
   virtual ~Stmt() = default;
   virtual Object accept(NSStmt::Visitor& visitor) = 0; 
};

class Block : public Stmt {
public:
  Block(std::vector<std::shared_ptr<Stmt>> statements) : statements(std::move(statements)) {};
    std::vector<std::shared_ptr<Stmt>> statements;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Expression : public Stmt {
public:
  Expression(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {};
    std::shared_ptr<Expr> expression;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Print : public Stmt {
public:
  Print(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {};
    std::shared_ptr<Expr> expression;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Var : public Stmt {
public:
  Var(Token name, std::shared_ptr<Expr> initializer) : name(std::move(name)), initializer(std::move(initializer)) {};
    Token name;
    std::shared_ptr<Expr> initializer;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

