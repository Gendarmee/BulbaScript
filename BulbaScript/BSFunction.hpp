#pragma once

#include "BSCallable.hpp"
#include "Environment.hpp"
#include "Stmt.hpp"

class Interpritator;

class BSFunction : public BSCallable {
public:
    BSFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure)
        : declaration(std::move(declaration)), closure(std::move(closure)) {}

    Object call(Interpritator& interpreter, std::vector<Object> arguments) override;
    int arity() const override;
    std::wstring toString() const override;

private:
    std::shared_ptr<Environment> closure;
    std::shared_ptr<Function> declaration;
};
