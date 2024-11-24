#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Object.hpp"
#include "Token.hpp"
#include "RuntimeError.hpp"

class Environment {
public:
    Environment() : enclosing(nullptr) {};
    explicit Environment(std::shared_ptr<Environment> enclosing);

    void define(const std::wstring& name, const Object& value);
    Object get(const Token& name) const;
    void assign(const Token& name, const Object& value);

private:
    std::unordered_map<std::wstring, Object> values;
    std::shared_ptr<Environment> enclosing;
};
