#pragma once

#include <memory>
#include <vector>
#include <variant>
#include "Object.hpp"
#include "Token.hpp"

using std::shared_ptr;
using std::vector;
class Assign;
class Binary;
class Call;
class Grouping;
class Logical;
class Literal;
class Unary;
class Variable;

namespace NSExpr {
class Visitor {
  public:
      virtual Object visit(const Assign& AssignExpr) = 0;
      virtual Object visit(const Binary& BinaryExpr) = 0;
      virtual Object visit(const Call& CallExpr) = 0;
      virtual Object visit(const Grouping& GroupingExpr) = 0;
      virtual Object visit(const Logical& LogicalExpr) = 0;
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
    Assign(Token name, shared_ptr<Expr> value) : name(std::move(name)), value(std::move(value)) {};
    Token name;
    shared_ptr<Expr> value;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Binary : public Expr {
public:
  Binary(shared_ptr<Expr> left, Token opr, shared_ptr<Expr> right) : left(std::move(left)), opr(std::move(opr)), right(std::move(right)) {};
    shared_ptr<Expr> left;
    Token opr;
    shared_ptr<Expr> right;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Call : public Expr {
public:
  Call(shared_ptr<Expr> callee, Token paren, vector<shared_ptr<Expr>> arguments) : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {};
    shared_ptr<Expr> callee;
    Token paren;
    vector<shared_ptr<Expr>> arguments;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Grouping : public Expr {
public:
  Grouping(shared_ptr<Expr> expression) : expression(std::move(expression)) {};
    shared_ptr<Expr> expression;
    Object accept(NSExpr::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Logical : public Expr {
public:
  Logical(shared_ptr<Expr> left, Token opr, shared_ptr<Expr> right) : left(std::move(left)), opr(std::move(opr)), right(std::move(right)) {};
    shared_ptr<Expr> left;
    Token opr;
    shared_ptr<Expr> right;
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
  Unary(Token opr, shared_ptr<Expr> right) : opr(std::move(opr)), right(std::move(right)) {};
    Token opr;
    shared_ptr<Expr> right;
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

