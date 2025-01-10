#include "Environment.hpp"

//Environment::Environment() : enclosing(nullptr) {}

Environment::Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(std::move(enclosing)) {}

void Environment::define(const std::wstring& name, const Object& value) {
    values[name] = value;
}

Object Environment::get(const Token& name) const {
    if (values.contains(name.lexeme)) {
        return values.at(name.lexeme);
    }
    if (enclosing) {
        return enclosing->get(name);
    }
    throw RuntimeError(name, L"Undefined variable '" + name.lexeme + L"'.");
}


void Environment::assign(const Token& name, const Object& value) {
    if (values.contains(name.lexeme)) {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing) {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, L"Undefined variable '" + name.lexeme + L"'.");
}
