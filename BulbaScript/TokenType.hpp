#pragma once
#include <string>

namespace TokenTypes {
    enum class TokenType {
        // Single-character tokens.
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, // 10

        // One or two character tokens.
        BANG, BANG_EQUAL,
        EQUAL, EQUAL_EQUAL,
        GREATER, GREATER_EQUAL,
        LESS, LESS_EQUAL, // 18

        // Literals.
        IDENTIFIER, STRING, NUMBER, // 21

        // Keywords.
        AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
        PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, // 37

        EF // 38
    };
    using enum TokenType;

    inline std::wstring TokenTypeToString(TokenType type) {
        switch (type) {
        case LEFT_PAREN: return L"LEFT_PAREN";
        case RIGHT_PAREN: return L"RIGHT_PAREN";
        case LEFT_BRACE: return L"LEFT_BRACE";
        case RIGHT_BRACE: return L"RIGHT_BRACE";
        case COMMA: return L"COMMA";
        case DOT: return L"DOT";
        case MINUS: return L"MINUS";
        case PLUS: return L"PLUS";
        case SEMICOLON: return L"SEMICOLON";
        case SLASH: return L"SLASH";
        case STAR: return L"STAR";
        case BANG: return L"BANG";
        case BANG_EQUAL: return L"BANG_EQUAL";
        case EQUAL: return L"EQUAL";
        case EQUAL_EQUAL: return L"EQUAL_EQUAL";
        case GREATER: return L"GREATER";
        case GREATER_EQUAL: return L"GREATER_EQUAL";
        case LESS: return L"LESS";
        case LESS_EQUAL: return L"LESS_EQUAL";
        case IDENTIFIER: return L"IDENTIFIER";
        case STRING: return L"STRING";
        case NUMBER: return L"NUMBER";
        case AND: return L"AND";
        case CLASS: return L"CLASS";
        case ELSE: return L"ELSE";
        case FALSE: return L"FALSE";
        case FOR: return L"FOR";
        case FUN: return L"FUN";
        case IF: return L"IF";
        case NIL: return L"NIL";
        case OR: return L"OR";
        case PRINT: return L"PRINT";
        case RETURN: return L"RETURN";
        case EF: return L"EOF";
        default: return L"UNKNOWN";
        }
    }
}

using namespace TokenTypes;