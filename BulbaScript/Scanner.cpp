#include "Scanner.hpp"
#include "BS.hpp"

Scanner::Scanner(wstring source) : source(source) {}


bool Scanner::isAtEnd()
{
    return current >= source.length();
}


vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    Token token(EF, L"", Object::make_str_obj(L""), line);
    tokens.push_back(token);

    return tokens;
}


void Scanner::scanToken()
{
    wchar_t c = advance();
    switch (c) {
    case L'(': addToken(LEFT_PAREN); break;
    case L')': addToken(RIGHT_PAREN); break;
    case L'{': addToken(LEFT_BRACE); break;
    case L'}': addToken(RIGHT_BRACE); break;
    case L',': addToken(COMMA); break;
    case L'.': addToken(DOT); break;
    case L'-': addToken(MINUS); break;
    case L'+': addToken(PLUS); break;
    case L';': addToken(SEMICOLON); break;
    case L'*': addToken(STAR); break;
    case L'!':
        addToken(match(L'=') ? BANG_EQUAL : BANG);
        break;
    case L'=':
        addToken(match(L'=') ? EQUAL_EQUAL : EQUAL);
        break;
    case L'<':
        addToken(match(L'=') ? LESS_EQUAL : LESS);
        break;
    case L'>':
        addToken(match(L'=') ? GREATER_EQUAL : GREATER);
        break;
    case L'/':
        if (match(L'/')) {
            // Комментарий идет до конца строки
            while (peek() != L'\n' && !isAtEnd())
                advance();
        } else if (match(L'*')) {
            while (!isAtEnd()) {
                if (peek() == L'\n') {
                    line++;
                }
                if (peek() == L'*' && match(L'/')) {
                    break;
                }
                advance();
            }
            if (isAtEnd()) {
                BS::error(line, L"Unterminated block comment.");
            }
        }
        else {
            addToken(SLASH);
        }
        break;
    case L' ':
    case L'\r':
    case L'\t':
        // Игнорируем пробелы
        break;

    case L'\n':
        line++;
        break;
    case L'"': str(); break;
    default:
        if (isDigit(c)) {
            number();
        }
        else if (isAlpha(c)) {
            identifier();
        }
        else {
            BS::error(line, L"Unexpected character.");
        }
        break;
    }
}


void Scanner::identifier() {
    while (isAlphaNumeric(peek())) {
        advance();
    }
    wstring id = source.substr(start, current - start);
    TokenType token;
    auto iter = keywords.find(id);
    if (iter == keywords.end()) {
        token = IDENTIFIER;
    }
    else {
        token = iter->second;
    }
    addToken(token);
}


bool Scanner::isAlphaNumeric(wchar_t c) {
    return isAlpha(c) || isDigit(c);
}


bool Scanner::isAlpha(wchar_t c) {
    return iswalpha(c) || c == '_';
        //(c >= 'a' && c <= 'z') ||
        //(c >= 'A' && c <= 'Z') ||
        //(c == '_') ||
        //(c >= '\u0410' && c <= '\u044F') ||  // (А-я)
        //(c == '\u0401' || c == '\u0451') ||  // Ё, ё
        //(c == '\u0406' || c == '\u0456') ||  // І, і
        //(c == '\u040E' || c == '\u045E');    // Ў, ў
}


bool Scanner::isDigit(wchar_t c) {
    return c >= L'0' && c <= L'9';
}


void Scanner::number() {
    while (isDigit(peek()))
        advance();
    // Ищем дробную часть
    if (peek() == L'.' && isDigit(peekNext())) {
        // Потребляем "."
        advance();

        while (isDigit(peek())) {
            advance();
        }
    }
    double value = (double)_wtof(source.substr(start, current - start).c_str());
    addToken(NUMBER, Object::make_num_obj(value));
}

wchar_t Scanner::peekNext() {
    if (current + 1 >= source.length()) return L'\0';
    return source.at(current + 1);
}


void Scanner::str() {
    while (peek() != L'"' && !isAtEnd()) {
        if (peek() == L'\n') line++;
        advance();
    }

    if (isAtEnd()) {
        BS::error(line, L"Unterminated string.");
        return;
    }

    // Закрытие "
    advance();

    // Убираем окружающие кавычки, корректируем длину
    wstring value = source.substr(start + 1, current - start - 2);
    addToken(STRING, Object::make_str_obj(value)); 
}


wchar_t Scanner::peek() {
    if (isAtEnd()) return L'\0';
    return source.at(current);
}


bool Scanner::match(wchar_t expected) {
    if (isAtEnd()) return false;
    if (source.at(current) != expected) return false;

    current++;
    return true;
}


wchar_t& Scanner::advance() {
    return source.at(current++);
}


void Scanner::addToken(TokenType type) {
    addToken(type, Object::make_str_obj(L""));
}


void Scanner::addToken(TokenType type, Object literal) {
    wstring text = source.substr(start, current - start);
    Token token(type, text, literal, line);
    tokens.push_back(token);
}