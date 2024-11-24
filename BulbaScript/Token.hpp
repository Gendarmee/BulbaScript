#pragma once

#include <string>
#include "TokenType.hpp"        
#include <variant>
#include <any>
#include "Object.hpp"

using std::wstring;

class Token {            
public:
    TokenType type;
    wstring lexeme;
    Object literal;
    int line;
    Token(TokenType type, wstring lexeme, Object literal, int line)
        : type(type), lexeme(lexeme), literal(literal),line(line) {};

    wstring toString();
};
