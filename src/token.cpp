#include "token.h"
#include <iostream>
#include <map>

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
		value.kvalue = kv;
		lineNum = ln;
		colNum = cn;
	}
	Token::Token(TokenType t, const std::string & strv, const int ln, const int cn)
	{
		type = t;
		value.strvalue = strv;
		lineNum = ln;
		colNum = cn;
	}
	Token::Token(const int intv, const int ln, const int cn)
	{
		type = TokenType::INTEGER;
		value.intvalue = intv;
		lineNum = ln;
		colNum = cn;
	}
	Token::Token(float fv, const int ln, const int cn)
	{
		type = TokenType::FLOAT;
		value.fvalue = fv;
		lineNum = ln;
		colNum = cn;
	}

	static const std::map<TokenType, std::string> strTokenType{
		{TokenType::SYMBOL, "Symbol"},
		{TokenType::KEYWORD, "Keyword"},
		{TokenType::IDENTIFIER, "Identifier"},
		{TokenType::INTEGER, "Integer"},
		{TokenType::FLOAT, "Float"},
		{TokenType::STRING, "String"}
	};

	static const std::map<Symbol, std::string> strSymbolType{
		{Symbol::ASTERISK, "*"},
		{Symbol::SEMICOLON, ";"},
		{Symbol::COMMA, ","},
		{Symbol::LBRACKET, "("},
		{Symbol::RBRACKET, ")"},
		{Symbol::PLUS, "+"},
		{Symbol::LT, "<"},
		{Symbol::GT, ">"},
		{Symbol::EQ, "="},
		{Symbol::LEQ, "<="},
		{Symbol::GEQ, ">="},
		{Symbol::NEQ, "!="}
	};

	static const std::map<Keyword, std::string> strKeywordType{
		{Keyword::AND, "and"},
		{Keyword::CHAR, "char"},
		{Keyword::CREATE, "create"},
		{Keyword::DELETE, "delete"},
		{Keyword::DROP, "drop"},
		{Keyword::EXECFILE, "execfile"},
		{Keyword::FLOAT, "float"},
		{Keyword::FROM, "from"},
		{Keyword::INDEX, "index"},
		{Keyword::INSERT, "insert"},
		{Keyword::INT, "int"},
		{Keyword::INTO, "into"},
		{Keyword::KEY, "key"},
		{Keyword::ON, "on"},
		{Keyword::PRIMARY, "primary"},
		{Keyword::QUIT, "quit"},
		{Keyword::SELECT, "select"},
		{Keyword::TABLE, "table"},
		{Keyword::UNIQUE, "unique"},
		{Keyword::VALUES, "values"},
		{Keyword::WHERE, "where"}
	};

	void printToken(Token & token)
	{
		std::cout << token.lineNum << " \t"
			<< token.colNum << " \t"
			<< strTokenType.find(token.type)->second << " \t";

		switch (token.type)
		{
			case TokenType::SYMBOL:
				std::cout << strSymbolType.find(token.value.svalue)->second;
				break;
			case TokenType::KEYWORD:
				std::cout << strKeywordType.find(token.value.kvalue)->second;
				break;
			case TokenType::INTEGER:
				std::cout << token.value.intvalue;
				break;
			case TokenType::FLOAT:
				std::cout << token.value.fvalue;
				break;
			case TokenType::IDENTIFIER:
				std::cout << token.value.strvalue;
				break;
			case TokenType::STRING:
				std::cout << token.value.strvalue;
				break;
		default:
			break;
		}

		std::cout << '\n';
	}

	TokenType Token::getType() {
		return type;
	}

	TokenValue Token::getValue() {
		return value;
	}

	int Token::getlineNum() {
		return lineNum;
	};
	int Token::getcolNum() {
		return colNum;
	};

}