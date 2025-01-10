#pragma once

#include <memory>
#include <vector>
#include <variant>
#include "Object.hpp"
#include "Expr.hpp"
#include "Token.hpp"

using std::shared_ptr;
using std::vector;
class Block;
class Expression;
class Function;
class If;
class Print;
class Var;
class Return;
class While;

namespace NSStmt {
class Visitor {
  public:
      virtual Object visit(const Block& BlockExpr) = 0;
      virtual Object visit(const Expression& ExpressionExpr) = 0;
      virtual Object visit(const Function& FunctionExpr) = 0;
      virtual Object visit(const If& IfExpr) = 0;
      virtual Object visit(const Print& PrintExpr) = 0;
      virtual Object visit(const Var& VarExpr) = 0;
      virtual Object visit(const Return& ReturnExpr) = 0;
      virtual Object visit(const While& WhileExpr) = 0;
};
} // namespace Stmt

class Stmt {
public:
   virtual ~Stmt() = default;
   virtual Object accept(NSStmt::Visitor& visitor) = 0; 
};

class Block : public Stmt {
public:
  Block(vector<shared_ptr<Stmt>> statements) : statements(std::move(statements)) {};
    vector<shared_ptr<Stmt>> statements;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Expression : public Stmt {
public:
  Expression(shared_ptr<Expr> expression) : expression(std::move(expression)) {};
    shared_ptr<Expr> expression;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Function : public Stmt {
public:
  Function(Token name, vector<Token> params, vector<shared_ptr<Stmt>> body) : name(std::move(name)), params(std::move(params)), body(std::move(body)) {};
    Token name;
    vector<Token> params;
    vector<shared_ptr<Stmt>> body;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class If : public Stmt {
public:
  If(shared_ptr<Expr> condition, shared_ptr<Stmt> thenBranch, shared_ptr<Stmt> elseBranch) : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {};
    shared_ptr<Expr> condition;
    shared_ptr<Stmt> thenBranch;
    shared_ptr<Stmt> elseBranch;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Print : public Stmt {
public:
  Print(shared_ptr<Expr> expression) : expression(std::move(expression)) {};
    shared_ptr<Expr> expression;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Var : public Stmt {
public:
  Var(Token name, shared_ptr<Expr> initializer) : name(std::move(name)), initializer(std::move(initializer)) {};
    Token name;
    shared_ptr<Expr> initializer;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Return : public Stmt {
public:
  Return(Token keyword, shared_ptr<Expr> value) : keyword(std::move(keyword)), value(std::move(value)) {};
    Token keyword;
    shared_ptr<Expr> value;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class While : public Stmt {
public:
  While(shared_ptr<Expr> condition, shared_ptr<Stmt> body) : condition(std::move(condition)), body(std::move(body)) {};
    shared_ptr<Expr> condition;
    shared_ptr<Stmt> body;
    Object accept(NSStmt::Visitor& visitor) override {
        return visitor.visit(*this);
    }
};  