#include "Parser.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include "Expr.hpp"

using std::make_shared;

// Entry point for parsing
std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match({ TokenType::VAR })) return varDeclaration();

        return statement();
    }
    catch (std::exception error) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, L"Expect variable name.");

    std::shared_ptr<Expr> initializer = nullptr;
    if (match({ TokenType::EQUAL })) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, L"Expect ';' after variable declaration.");
    return std::make_shared<Var>(name, initializer);
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({ TokenType::PRINT })) return printStatement();
    if (match({ TokenType::LEFT_BRACE })) return std::make_shared<Block>(block());

    return expressionStatement();
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, L"Expect '}' after block.");
    return statements;
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, L"Expect ';' after expression.");
    return std::make_shared<Expression>(expr);
}

std::shared_ptr<Stmt> Parser::printStatement() {
    std::shared_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, L"Expect ';' after value.");
    return std::make_shared<Print>(value);
}

// Grammar: expression → equality
std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = equality();

    if (match({ TokenType::EQUAL })) {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();

        if (auto varExpr = std::dynamic_pointer_cast<Variable>(expr)) {
            Token name = varExpr->name;
            return std::make_shared<Assign>(name, value);
        }
        error(equals, L"Invalid assignment target.");
    }
    return expr;
}

// Grammar: equality → comparison ( ( "!=" | "==" ) comparison )*
std::shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();

    while (match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL })) {
        Token opr = previous();
        auto right = comparison();
        expr = make_shared<Binary>(std::move(expr), opr, std::move(right));
    }

    return expr;
}

// Grammar: comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )*
std::shared_ptr<Expr> Parser::comparison() {
    auto expr = term();

    while (match({ TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL })) {
        Token opr = previous();
        auto right = term();
        expr = make_shared<Binary>(std::move(expr), opr, std::move(right));
    }

    return expr;
}

// Grammar: term → factor ( ( "-" | "+" ) factor )*
std::shared_ptr<Expr> Parser::term() {
    auto expr = factor();

    while (match({ TokenType::MINUS, TokenType::PLUS })) {
        Token opr = previous();
        auto right = factor();
        expr = make_shared<Binary>(std::move(expr), opr, std::move(right));
    }

    return expr;
}

// Grammar: factor → unary ( ( "/" | "*" ) unary )*
std::shared_ptr<Expr> Parser::factor() {
    auto expr = unary();

    while (match({ TokenType::SLASH, TokenType::STAR })) {
        Token opr = previous();
        auto right = unary();
        expr = make_shared<Binary>(std::move(expr), opr, std::move(right));
    }

    return expr;
}

// Grammar: unary → ( "!" | "-" ) unary | fprimary
std::shared_ptr<Expr> Parser::unary() {
    if (match({ TokenType::BANG, TokenType::MINUS })) {
        Token opr = previous();
        auto right = unary();
        return make_shared<Unary>(opr, std::move(right));
    }

    return primary();
}

// Grammar: primary → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
std::shared_ptr<Expr> Parser::primary() {
    if (match({ TokenType::FALSE })) return make_shared<Literal>(Object::make_bool_obj(false));
    if (match({ TokenType::TRUE })) return make_shared<Literal>(Object::make_bool_obj(true));
    if (match({ TokenType::NIL })) return make_shared<Literal>(Object::make_nil_obj());

    if (match({ TokenType::NUMBER, TokenType::STRING })) {
        return make_shared<Literal>(previous().literal);
    }
    if (match({ TokenType::IDENTIFIER })) {
        return std::make_shared<Variable>(previous());
    }
    if (match({ TokenType::LEFT_PAREN })) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, L"Expect ')' after expression.");
        return make_shared<Grouping>(std::move(expr));
    }

    throw error(peek(), L"Expect expression.");
}

// Matches if the next token is of one of the specified types
bool Parser::match(const std::initializer_list<TokenType>& types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::consume(TokenType type, const std::wstring& message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EF;
}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

std::runtime_error Parser::error(const Token& token, const std::wstring& message) {
    std::wstring errorMessage;

    if (token.type == TokenType::EF) {
        errorMessage = std::to_wstring(token.line) + L" at end: " + message;
    }
    else {
        errorMessage = std::to_wstring(token.line) + L" at '" + token.lexeme + L"': " + message;
    }

    return std::runtime_error(std::string(errorMessage.begin(), errorMessage.end()));
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        }

        advance();
    }
}