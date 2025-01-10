#include "Resolver.hpp"

Object Resolver::visit(const Var& stmt) {
    declare(stmt.name);
    if (stmt.initializer != nullptr) {
        resolve(stmt.initializer);
    }
    define(stmt.name);
    return Object::make_nil_obj();
}

void Resolver::define(const Token& name) {
    if (scopes.empty()) return;
    scopes.top()[name.lexeme] = true;
}


void Resolver::declare(const Token& name) {
    if (scopes.empty()) return;
    auto& scope = scopes.top();
    scope[name.lexeme] = false;
}


Object Resolver::visit(const Block& stmt)
{
    beginScope();
    resolve(stmt.statements);
    endScope();
	return Object::make_nil_obj();
}

Object Resolver::visit(const Variable& expr)
{
    if (!scopes.empty() &&
        scopes.top()[expr.name.lexeme] == false) {
        BS::error(expr.name.line,
            L"Can't read local variable in its own initializer.");
    }

    resolveLocal(std::make_shared<Variable>(expr), expr.name);
    return Object::make_nil_obj();
}

void Resolver::resolveLocal(const shared_ptr<Expr>& expr, const Token& name) {
    //for (int i = scopes.size() - 1; i >= 0; i--) {
    //    if (scopes[i].find(name.lexeme) != scopes[i].end()) {
    //        interpreter.resolve(expr, scopes.size() - 1 - i);
    //        return;
    //    }
    //}
}


void Resolver::resolve(vector<shared_ptr<Stmt>> statements)
{
    for (shared_ptr<Stmt> statement : statements) {
        resolve(statement);
    }
}

void Resolver::resolve(shared_ptr<Stmt> stmt)
{
    stmt->accept(*this);

}

void Resolver::resolve(shared_ptr<Expr> expr) {
    expr->accept(*this);
}

void Resolver::beginScope() {
    //scopes.push(std::unordered_map<std::string, bool>());
}

void Resolver::endScope() {
    scopes.pop();
}