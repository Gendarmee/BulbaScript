#include "Token.hpp"

wstring Token::toString()
{
		return L"TokenType: " + TokenTypeToString(type) +
	        L", lexeme: \"" + lexeme +
	        L"\", literal: " + literal.toString() +
	        L", line: " + std::to_wstring(line) + L" }";
 }
