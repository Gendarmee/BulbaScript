#include "Parser.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <codecvt>
#include "Expr.hpp"

vector<shared_ptr<Stmt>> Parser::parse() {
    vector<shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        auto stmt = declaration();
        if (stmt) {
            statements.push_back(stmt);
        }
    }
    return statements;
}

shared_ptr<Stmt> Parser::declaration() {    
    try {
        if (match({ VAR })) return varDeclaration();
        return statement();
    }
    catch (const exception&) {
        synchronize();
        return nullptr;
    }
}

shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, L"Expect variable name.");
    shared_ptr<Expr> initializer = nullptr;
    if (match({ EQUAL })) {
        initializer = expression();
    }
    consume(SEMICOLON, L"Expect ';' after variable declaration.");
    return make_shared<Var>(name, initializer);
}

shared_ptr<Stmt> Parser::statement() {
    if (match({ FOR })) return forStatement();
    if (match({ FUN })) return function(L"function");
    if (match({ RETURN })) return returnStatement();
    if (match({ IF })) return ifStatement();
    if (match({ WHILE })) return whileStatement();
    if (match({ PRINT })) return printStatement();
    if (match({ LEFT_BRACE })) return make_shared<Block>(block());
    return expressionStatement();
}

shared_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    shared_ptr<Expr> value = nullptr;
    if (!check(SEMICOLON)) {
        value = expression();
    }
    consume(SEMICOLON, L"Expect ';' after return value.");
    return make_shared<Return>(keyword, value);
}

shared_ptr<Stmt> Parser::function(wstring kind) {
    Token name = consume(IDENTIFIER, L"Expect " + kind + L" name.");
    consume(LEFT_PAREN, L"Expect '(' after " + kind + L" name.");
    vector<Token> parameters;
    if (!check({ RIGHT_PAREN })) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), L"Can't have more than 255 parameters.");
            }
            parameters.push_back(consume(IDENTIFIER, L"Expect parameter name."));
        } while (match({ COMMA }));
    }
    consume(RIGHT_PAREN, L"Expect ')' after parameters.");
    consume(LEFT_BRACE, L"Expect '{' before " + kind + L" body.");
    vector<shared_ptr<Stmt>> body = block();
    return make_shared<Function>(name, parameters, body);
}

shared_ptr<Stmt> Parser::forStatement() {
    consume(LEFT_PAREN, L"Expect '(' after 'for'.");
    shared_ptr<Stmt> initializer;
    if (match({ SEMICOLON })) {
        initializer = nullptr;
    }
    else if (match({ VAR })) {
        initializer = varDeclaration();
    }
    else {
        initializer = expressionStatement();
    }
    shared_ptr<Expr> condition = nullptr;
    if (!check(SEMICOLON)) {
        condition = expression();
    }
    consume(SEMICOLON, L"Expect ';' after loop condition.");

    shared_ptr<Expr> increment = nullptr;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, L"Expect ')' after for clauses.");
    shared_ptr<Stmt> body = statement();
    
    if (increment != nullptr) {
        body = make_shared<Block>(
            vector<shared_ptr<Stmt>>({
                body,
                make_shared<Expression>(increment)}));
    }

    if (condition == nullptr) condition = make_shared<Literal>(Object::make_str_obj(L""));
    body = make_shared<While>(condition, body);

    if (initializer != nullptr) {
        body = make_shared<Block>(vector<shared_ptr<Stmt>>({ initializer, body }));
    }
    return body;
}

shared_ptr<Stmt> Parser::whileStatement() {
    consume(LEFT_PAREN, L"Expect '(' after 'while'.");
    shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, L"Expect ')' after condition.");
    shared_ptr<Stmt> body = statement();
    return make_shared<While>(condition, body);
}

shared_ptr<Stmt> Parser::ifStatement() {
    consume(LEFT_PAREN, L"Expect '(' after 'if'.");
    shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, L"Expect ')' after if condition.");

    shared_ptr<Stmt> thenBranch = statement();
    shared_ptr<Stmt> elseBranch = nullptr;
    if (match({ ELSE })) {
        elseBranch = statement();
    }
    return make_shared<If>(condition, thenBranch, elseBranch);
}

vector<shared_ptr<Stmt>> Parser::block() {
    vector<shared_ptr<Stmt>> statements;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        auto stmt = declaration();
        if (stmt) statements.push_back(stmt);
    }
    consume(RIGHT_BRACE, L"Expect '}' after block.");
    return statements;
}

shared_ptr<Stmt> Parser::expressionStatement() {
    auto expr = expression();
    consume(SEMICOLON, L"Expect ';' after expression.");
    return make_shared<Expression>(expr);
}

shared_ptr<Stmt> Parser::printStatement() {
    auto value = expression();
    consume(SEMICOLON, L"Expect ';' after value.");
    return make_shared<Print>(value);
}

shared_ptr<Expr> Parser::expression() {
    return assignment();
}

shared_ptr<Expr> Parser::assignment() {
    auto expr = _or();
    if (match({ EQUAL })) {
        Token equals = previous();
        auto value = assignment();
        if (auto varExpr = dynamic_pointer_cast<Variable>(expr)) {
            return make_shared<Assign>(varExpr->name, value);
        }
        throw error(equals, L"Invalid assignment target.");
    }
    return expr;
}

shared_ptr<Expr> Parser::_or() {
    shared_ptr<Expr> expr = _and();
    while (match({ OR })) {
        Token opr = previous();
        shared_ptr<Expr> right = _and();
        expr = make_shared<Logical>(expr, opr, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::_and() {
    shared_ptr<Expr> expr = equality();
    while (match({ OR })) {
        Token opr = previous();
        shared_ptr<Expr> right = equality();
        expr = make_shared<Logical>(expr, opr, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    while (match({ BANG_EQUAL, EQUAL_EQUAL })) {
        Token opr = previous();
        auto right = comparison();
        expr = make_shared<Binary>(expr, opr, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::comparison() {
    auto expr = term();
    while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL })) {
        Token opr = previous();
        auto right = term();
        expr = make_shared<Binary>(expr, opr, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::term() {
    auto expr = factor();
    while (match({ MINUS, PLUS })) {
        Token opr = previous();
        auto right = factor();
        expr = make_shared<Binary>(expr, opr, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::factor() {
    auto expr = unary();
    while (match({ SLASH, STAR })) {
        Token opr = previous();
        auto right = unary();
        expr = make_shared<Binary>(expr, opr, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::unary() {
    if (match({ BANG, MINUS })) {
        Token opr = previous();
        auto right = unary();
        return make_shared<Unary>(opr, right);
    }
    return call();
}

shared_ptr<Expr> Parser::call() {
    shared_ptr<Expr> expr = primary();

    while (true) {
        if (match({ LEFT_PAREN })) {
            expr = finishCall(expr);
        }
        else {
            break;
        }
    }

    return expr;
}

shared_ptr<Expr> Parser::finishCall(shared_ptr<Expr> callee) {
    vector<shared_ptr<Expr>> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), L"Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match({ COMMA }));
    }

    Token paren = consume(RIGHT_PAREN,
        L"Expect ')' after arguments.");

    return make_shared<Call>(callee, paren, arguments);
}

shared_ptr<Expr> Parser::primary() {
    if (match({ FALSE })) return make_shared<Literal>(Object::make_bool_obj(false));
    if (match({ TRUE })) return make_shared<Literal>(Object::make_bool_obj(true));
    if (match({ NIL })) return make_shared<Literal>(Object::make_nil_obj());
    if (match({ NUMBER, STRING })) {
        return make_shared<Literal>(previous().literal);
    }
    if (match({ IDENTIFIER })) {
        return make_shared<Variable>(previous());
    }
    if (match({ LEFT_PAREN })) {
        auto expr = expression();
        consume(RIGHT_PAREN, L"Expect ')' after expression.");
        return make_shared<Grouping>(expr);
    }
    throw error(peek(), L"Expect expression.");
}

bool Parser::match(const initializer_list<TokenType>& types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    return !isAtEnd() && peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::consume(TokenType type, const wstring& message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

bool Parser::isAtEnd() {
    return peek().type == EF;
}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

runtime_error Parser::error(const Token& token, const wstring& message) {
    wstring errorMessage;

    if (token.type == EF) {
        errorMessage = to_wstring(token.line) + L" at end: " + message;
    }
    else {
        errorMessage = to_wstring(token.line) + L" at '" + token.lexeme + L"': " + message;
    }
    return runtime_error(string(errorMessage.begin(), errorMessage.end()));
}

void Parser::synchronize() {
    advance();  
    while (!isAtEnd()) {
        if (previous().type == SEMICOLON) return;
        if (unordered_set<TokenType>{
            CLASS, FUN, VAR,
                FOR, IF, WHILE,
                PRINT, RETURN
        }.count(peek().type)) return;
        advance();
    }
}

void Parser::debugTokens() const {
    for (size_t i = current; i < tokens.size(); ++i) {
        wcout << L"Token[" << i << L"]: " << tokens[i].lexeme << L"\n";
    }
}
