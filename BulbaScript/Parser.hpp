#ifndef PARSER_HPP
#define PARSER_HPP

#include "BS.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>
#include "Expr.hpp"

using std::make_shared;
using std::shared_ptr;
using std::vector;
using std::exception;
using std::initializer_list;
using std::runtime_error;
using std::wcout;
using std::to_wstring;
using std::string;
using std::unordered_set;

class Parser {
public:
    explicit Parser(const vector<Token>& tokens) : tokens(tokens), current(0) {}

    vector<shared_ptr<Stmt>> parse();
    shared_ptr<Stmt> declaration();
    shared_ptr<Stmt> varDeclaration();

private:
    const vector<Token>& tokens;
    size_t current;

    shared_ptr<Stmt> statement();
    shared_ptr<Stmt> returnStatement();
    shared_ptr<Stmt> function(wstring kind);
    shared_ptr<Stmt> forStatement();
    shared_ptr<Stmt> whileStatement();
    shared_ptr<Stmt> ifStatement();
    vector<shared_ptr<Stmt>> block();
    shared_ptr<Stmt> expressionStatement();
    shared_ptr<Stmt> printStatement();
    shared_ptr<Expr> expression();
    shared_ptr<Expr> assignment();
    shared_ptr<Expr> _or();
    shared_ptr<Expr> _and();
    shared_ptr<Expr> equality();
    shared_ptr<Expr> comparison();
    shared_ptr<Expr> term();
    shared_ptr<Expr> factor();
    shared_ptr<Expr> unary();
    shared_ptr<Expr> call();
    shared_ptr<Expr> finishCall(shared_ptr<Expr> callee);
    shared_ptr<Expr> primary();

    bool match(const initializer_list<TokenType>& types);
    bool check(TokenType type);
    Token advance();
    Token consume(TokenType type, const wstring& message);
    bool isAtEnd();
    Token peek();
    Token previous();

    runtime_error error(const Token& token, const wstring& message);

    void synchronize();

    void debugTokens() const;
};

#endif // PARSER_HPP
