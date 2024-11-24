#pragma once

#include <memory>
#include <vector>
#include <variant>
#include "Object.hpp"
#include "Token.hpp"

class Assign;
class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;
namespace NSExpr {
class Visitor {
  public:
      virtual Object visit(const Assign& AssignExpr) = 0;
      virtual Object visit(const Binary& BinaryExpr) = 0;
      virtual Object visit(const Grouping& GroupingExpr) = 0;
      virtual Object visit(const Literal& LiteralExpr) = 0;
      virtual Object visit(const Unary& UnaryExpr) = 0;
      virtual Object visit(const Variable& VariableExpr) = 0;
};
} // namespace Expr

class Expr {
public:
   virtual ~Expr() = default;
   virtual Object accept(NSExpr::Visitor& visitor) = 0; 
};

class Assign : public Expr {
public:
  Assign(Token name, std::shared_ptr<Expr> value) : name(std::move(name)), value(std::move(value)) {};
    Token name;
    std::shared_ptr<Expr> value;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Binary : public Expr {
public:
  Binary(std::shared_ptr<Expr> left, Token opr, std::shared_ptr<Expr> right) : left(std::move(left)), opr(std::move(opr)), right(std::move(right)) {};
    std::shared_ptr<Expr> left;
    Token opr;
    std::shared_ptr<Expr> right;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Grouping : public Expr {
public:
  Grouping(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {};
    std::shared_ptr<Expr> expression;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Literal : public Expr {
public:
  Literal(Object value) : value(std::move(value)) {};
    Object value;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Unary : public Expr {
public:
  Unary(Token opr, std::shared_ptr<Expr> right) : opr(std::move(opr)), right(std::move(right)) {};
    Token opr;
    std::shared_ptr<Expr> right;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Variable : public Expr {
public:
  Variable(Token name) : name(std::move(name)) {};
    Token name;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

