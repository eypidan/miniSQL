#include <token.h>

namespace Interpreter {
	Token::Token(const Symbol & sv, const int ln, const int cn)
	{
		type =	TokenType::SYMBOL;
		value.svalue = sv;
		lineNum = ln;
		colNum = cn;
	}
	Token::Token(const Keyword & kv, const int ln, const int cn)
	{
		type = TokenType::KEYWORD;
		value.svalue = kv;
		lineNum = ln;
		colNum = cn;
	}
	Token::Token(TokenType t, const std::string & strv, const int ln, const int cn)
	{
		type = t;
		value.svalue = strv;
		lineNum = ln;
		colNum = cn;
	}
	Token::Token(const int intv, const int ln, const int cn)
	{
		type = TokenType::INTEGER;
		value.svalue = intv;
		lineNum = ln;
		colNum = cn;
	}
	Token::Token(float fv, const int ln, const int cn)
	{
		type = TokenType::FLOAT;
		value.svalue = fv;
		lineNum = ln;
		colNum = cn;
	}
}