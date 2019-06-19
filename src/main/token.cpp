#include "token.h"
#include <iostream>
#include <map>
#include <iomanip>

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

	static const char *symbols[] = { 
		"*",  ";",  ",", "(",  ")", "<",
		">", "=", "<=", ">=", "<>" 
	};

	static const char *keywords[] = {
		"and",     "char",  "create", "delete", "drop",   "execfile", "float",
		"from",    "index", "insert", "int",    "into",   "key",      "on",
		"primary", "quit",  "select", "table",  "unique", "values",   "where" 
	};

	static const char *tokentypes[] = {
		"symbol", "keyword", "identifier", "integer", "float", "string"
	};
	
	const char * mapKeywordToString(const Keyword & k)
	{
		return keywords[static_cast<int>(k)];
	}

	const char * mapSymbolToString(const Symbol & s)
	{
		return symbols[static_cast<int>(s)];
	}

	const char * mapTokenTypeToString(const TokenType & t)
	{
		return tokentypes[static_cast<int>(t)];
	}

	void printToken(Token & token)
	{
		std::cout << std::left << std::setw(15)
			<< token.lineNum
			<< std::left << std::setw(15)
			<< token.colNum
			<< std::left << std::setw(15)
			<< mapTokenTypeToString(token.type);

		switch (token.type)
		{
		case TokenType::SYMBOL:
			std::cout << std::left << std::setw(15) << mapSymbolToString(token.value.svalue);
			break;
		case TokenType::KEYWORD:
			std::cout << std::left << std::setw(15) << mapKeywordToString(token.value.kvalue);
			break;
		case TokenType::INTEGER:
			std::cout << std::left << std::setw(15) << token.value.intvalue;
			break;
		case TokenType::FLOAT:
			std::cout << std::left << std::setw(15) << token.value.fvalue;
			break;
		case TokenType::IDENTIFIER:
			std::cout << std::left << std::setw(15) << token.value.strvalue;
			break;
		case TokenType::STRING:
			std::cout << std::left << std::setw(15) << token.value.strvalue;
			break;
		default:
			break;
		}

		std::cout << std::endl;
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