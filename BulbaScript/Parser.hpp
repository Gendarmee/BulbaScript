#ifndef PARSER_HPP
#define PARSER_HPP

#include "BS.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>
#include "Expr.hpp"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

    std::vector<std::shared_ptr<Stmt>> parse();
    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> varDeclaration();

private:
    const std::vector<Token>& tokens;
    size_t current;

    std::shared_ptr<Stmt> statement();
    std::vector<std::shared_ptr<Stmt>> block();
    std::shared_ptr<Stmt> expressionStatement();
    std::shared_ptr<Stmt> printStatement();
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

    bool match(const std::initializer_list<TokenType>& types);
    bool check(TokenType type);
    Token advance();
    Token consume(TokenType type, const std::wstring& message);
    bool isAtEnd();
    Token peek();
    Token previous();

    std::runtime_error error(const Token& token, const std::wstring& message);

    void synchronize();

    void debugTokens() const;
};

#endif // PARSER_HPP
