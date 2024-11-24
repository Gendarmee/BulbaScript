#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"

using std::wstring;
using std::vector;

class Scanner {
private:
    wstring source;  
    vector<Token> tokens;              
    int start = 0;                    
    int current = 0;                
    int line = 1;                      

    std::unordered_map<wstring, TokenType> keywords = {
        {L"var",    TokenType::VAR},
        {L"and",    TokenType::AND},
        {L"class",  TokenType::CLASS},
        {L"else",   TokenType::ELSE},
        {L"false",  TokenType::FALSE},
        {L"for",    TokenType::FOR},
        {L"fun",    TokenType::FUN},
        {L"if",     TokenType::IF},
        {L"nil",    TokenType::NIL},
        {L"or",     TokenType::OR},
        {L"print",  TokenType::PRINT},
        {L"return", TokenType::RETURN},
    };

    bool isAtEnd();                 
    void str();                       
    void number();                  
    bool isAlpha(wchar_t c);         
    bool isDigit(wchar_t c);       
    bool isAlphaNumeric(wchar_t c);   
    void identifier();                 
    wchar_t peekNext();                
    bool match(wchar_t expected);  
    wchar_t peek();
    wchar_t& advance();
    void addToken(TokenType type);   
    void addToken(TokenType type, Object literal);  
public:
    Scanner(wstring source);         

    vector<Token> scanTokens();     
    void scanToken();                
};