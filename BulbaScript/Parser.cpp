#include "Parser.hpp"
#include "BS.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <locale>
#include <codecvt>
/*
equality       → comparison ( ( "!=" | "==" ) comparison )* ;

comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;

unary          → ( "!" | "-" ) unary
               | primary ;

primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;

               */

using std::make_shared;

shared_ptr<Expr> Parser::expression()
{
    return equality();
}

shared_ptr<Expr> Parser::equality()
{
    shared_ptr<Expr> expr = comparison();

    while (match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL })) {
        Token opr = previous();
        shared_ptr<Expr> right = comparison();
        expr = make_shared<Binary>(expr, opr, right);

    }

    return expr;
}

shared_ptr<Expr> Parser::primary() {
    if (match({ TokenType::FALSE }))
        return make_shared<Literal>(Object::make_bool_obj(false));

    if (match({ TokenType::TRUE }))
        return make_shared<Literal>(Object::make_bool_obj(true));

    if (match({ TokenType::NIL }))
        return make_shared<Literal>(Object::make_nil_obj());

    if (match({ TokenType::NUMBER, TokenType::STRING })) {
        return make_shared<Literal>(previous().literal);
    }

    if (match({ TokenType::LEFT_PAREN })) {
        shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, L"Expect ')' after expression.");
        return make_shared<Grouping>(std::move(expr));
    }

    throw error(peek(), L"Expect expression.");
}


shared_ptr<Expr> Parser::comparison()
{
    shared_ptr<Expr> expr = term();

    while (match({ TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL })) {
        Token opr = previous();
        shared_ptr<Expr> right = term();
        expr = make_shared<Binary>(std::move(expr), opr, std::move(right));
    }

    return expr;
}

shared_ptr<Expr> Parser::term()
{
    shared_ptr<Expr> expr = factor();

    while (match({ TokenType::MINUS, TokenType::PLUS })) {
        Token opr = previous();
        shared_ptr<Expr> right = factor();
        expr = make_shared<Binary>(std::move(expr), opr, std::move(right));
    }

    return expr;
}

shared_ptr<Expr> Parser::factor() {
    shared_ptr<Expr> expr = unary();

    while (match({ TokenType::SLASH, TokenType::STAR })) {
        Token opr = previous();
        shared_ptr<Expr> right = unary();
        expr = make_shared<Binary>(std::move(expr), opr, std::move(right));
    }

    return expr;
}

shared_ptr<Expr> Parser::unary() {
    if (match({ TokenType::BANG, TokenType::MINUS })) {
        Token opr = previous();
        shared_ptr<Expr> right = unary();
        return make_shared<Unary>(opr, std::move(right));
    }
    return primary();
}


bool Parser::match(const std::initializer_list<TokenType>& types)
{
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type)
{
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance()
{
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::consume(TokenType type, wstring message) {
    if (check(type)) return advance();

    throw error(peek(), message);
}

std::runtime_error Parser::error(Token token, std::wstring message) {
    std::wstring errorMessage;

    if (token.type == TokenType::EF) {
        errorMessage = std::to_wstring(token.line) + L" at end" + message;
    }
    else {
        errorMessage = std::to_wstring(token.line) + L" at '" + token.lexeme + L"'" + message;
    }
    return std::runtime_error("");
}

bool Parser::isAtEnd() // проверяет, закончились ли у нас токены для анализа
{
    return peek().type == TokenType::EF;
}

Token Parser::peek() // возвращает текущий токен, который нам еще предстоит использовать
{
    return tokens.at(current);
}

Token Parser::previous() // возвращает самые последние израсходованный токен
{
    return tokens.at(current - 1);
}
