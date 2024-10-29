#pragma once
#include <string>;

enum class TokenType {
	// Single-wchar_tacter tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, // 10

	// One or two wchar_tacter tokens.
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

inline std::wstring TokenTypeToString(TokenType type) {
    switch (type) {
    case TokenType::LEFT_PAREN: return L"LEFT_PAREN";
    case TokenType::RIGHT_PAREN: return L"RIGHT_PAREN";
    case TokenType::LEFT_BRACE: return L"LEFT_BRACE";
    case TokenType::RIGHT_BRACE: return L"RIGHT_BRACE";
    case TokenType::COMMA: return L"COMMA";
    case TokenType::DOT: return L"DOT";
    case TokenType::MINUS: return L"MINUS";
    case TokenType::PLUS: return L"PLUS";
    case TokenType::SEMICOLON: return L"SEMICOLON";
    case TokenType::SLASH: return L"SLASH";
    case TokenType::STAR: return L"STAR";
    case TokenType::BANG: return L"BANG";
    case TokenType::BANG_EQUAL: return L"BANG_EQUAL";
    case TokenType::EQUAL: return L"EQUAL";
    case TokenType::EQUAL_EQUAL: return L"EQUAL_EQUAL";
    case TokenType::GREATER: return L"GREATER";
    case TokenType::GREATER_EQUAL: return L"GREATER_EQUAL";
    case TokenType::LESS: return L"LESS";
    case TokenType::LESS_EQUAL: return L"LESS_EQUAL";
    case TokenType::IDENTIFIER: return L"IDENTIFIER";
    case TokenType::STRING: return L"STRING";
    case TokenType::NUMBER: return L"NUMBER";
    case TokenType::AND: return L"AND";
    case TokenType::CLASS: return L"CLASS";
    case TokenType::ELSE: return L"ELSE";
    case TokenType::FALSE: return L"FALSE";
    case TokenType::FOR: return L"FOR";
    case TokenType::FUN: return L"FUN";
    case TokenType::IF: return L"IF";
    case TokenType::NIL: return L"NIL";
    case TokenType::OR: return L"OR";
    case TokenType::PRINT: return L"PRINT";
    case TokenType::RETURN: return L"RETURN";
    case TokenType::EF: return L"EOF";
    default: return L"UNKNOWN";
    }
}

