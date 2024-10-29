#pragma once
#include <vector>
#include <initializer_list>
#include "TokenType.hpp"
#include "Token.hpp"
#include "Expr.hpp"
using std::shared_ptr;
class Parser {
private:
    const std::vector<Token> tokens;
    int current = 0;
    shared_ptr<Expr> expression();
    shared_ptr<Expr> equality();
    shared_ptr<Expr> primary();
    shared_ptr<Expr> comparison();
    shared_ptr<Expr> term();
    shared_ptr<Expr> factor();
    shared_ptr<Expr> unary();
    bool match(const std::initializer_list<TokenType> &types);
    bool check(TokenType type);
    Token advance();
    std::runtime_error error(Token token, wstring message);
    Token consume(TokenType type, wstring message);
    bool isAtEnd();
    Token peek();
    Token previous();
public:
    Parser(std::vector<Token> tokens) {
        tokens = tokens;
    }
};