#pragma once

#include <memory>
#include <variant>
#include "Object.hpp"
#include "Token.hpp"

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor {
  public:
      virtual Object visit(const Binary& BinaryExpr) = 0;
      virtual Object visit(const Grouping& GroupingExpr) = 0;
      virtual Object visit(const Literal& LiteralExpr) = 0;
      virtual Object visit(const Unary& UnaryExpr) = 0;
};

class Expr {
public:
   virtual ~Expr() = default;
   virtual Object accept(Visitor& visitor) = 0;
};

class Binary : public Expr {
public:
  Binary(std::shared_ptr<Expr> left, Token opr, std::shared_ptr<Expr> right) : left(std::move(left)), opr(std::move(opr)), right(std::move(right)) {};
    std::shared_ptr<Expr> left;
    Token opr;
    std::shared_ptr<Expr> right;
    Object accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Grouping : public Expr {
public:
  Grouping(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {};
    std::shared_ptr<Expr> expression;
    Object accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Literal : public Expr {
public:
  Literal(Object value) : value(std::move(value)) {};
    Object value;
    Object accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Unary : public Expr {
public:
  Unary(Token opr, std::shared_ptr<Expr> right) : opr(std::move(opr)), right(std::move(right)) {};
    Token opr;
    std::shared_ptr<Expr> right;
    Object accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

