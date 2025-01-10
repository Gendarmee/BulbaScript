#include <memory>
#include "Interpritator.hpp"
#include <stack>

class Interpreter;

class Resolver : public NSStmt::Visitor, public NSExpr::Visitor {
private:
    std::shared_ptr<Interpreter> interpreter;
    std::stack<std::unordered_map<wstring, bool>> scopes;

public:
    explicit Resolver(std::shared_ptr<Interpreter> interpreter)
        : interpreter(std::move(interpreter)) {}
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
    void resolveLocal(const shared_ptr<Expr>& expr, const Token& name); 
    void resolve(vector<shared_ptr<Stmt>> statements);
    void resolve(shared_ptr<Stmt> stmt);
    void resolve(shared_ptr<Expr> expr);
    void define(const Token& name);
    void declare(const Token& name);
    void beginScope();
    void endScope();

};
