#pragma once
#include "Token.hpp"
#include <string>
#include <stdexcept>
#include <locale>
#include <string_view>

class RuntimeError : public std::runtime_error {
public:
    Token token;
    std::wstring message;

    explicit RuntimeError(const Token& token, const std::wstring& wmessage)
        : std::runtime_error(toNarrowString(wmessage)), token(token), message(wmessage) {}

    explicit RuntimeError(const Token& token, const std::string& message)
        : std::runtime_error(message), token(token), message(toWideString(message)) {}

    std::wstring getMessage() const {
        return message;
    }

private:
    static std::string toNarrowString(const std::wstring& wstr) {
        std::string result;
        for (wchar_t wc : wstr) {
            if (wc < 0x80) {
                result += static_cast<char>(wc);
            }
            else {
                throw std::runtime_error("Non-ASCII character in wstring. Use proper UTF-8 handling.");
            }
        }
        return result;
    }

    static std::wstring toWideString(const std::string& str) {
        std::wstring result;
        for (char c : str) {
            result += static_cast<wchar_t>(static_cast<unsigned char>(c));
        }
        return result;
    }
};
