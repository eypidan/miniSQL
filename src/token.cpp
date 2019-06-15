#include <token.h>
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

	void printToken(Token token)
	{
		const std::map<TokenType, std::string> strTokenType{
			{TokenType::SYMBOL, "Symbol"},
			{TokenType::KEYWORD, "Keyword"},
			{TokenType::IDENTIFIER, "Identifier"},
			{TokenType::INTEGER, "Integer"},
			{TokenType::FLOAT, "Float"},
			{TokenType::STRING, "String"},
		}; 
		
		const std::map<Symbol, std::string> strSymbolType{
			{Symbol::ASTERISK, "*"},
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

		const std::map<Keyword, std::string> strKeywordType{
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
			{Keyword::WHERE, "where"},
		};

		std::cout << token.lineNum << '\t'
			<< token.colNum << '\t'
			<< strTokenType.find(token.type) << '\t';

		switch (token.type)
		{
			case TokenType::SYMBOL:
				std::cout << strSymbolType.find(token.value.svalue);
				break;
			case TokenType::KEYWORD:
				std::cout << strKeywordType.find(token.value.kvalue);
				break;
			case TokenType::INTEGER:
				std::cout << token.value.intvalue;
			case TokenType::FLOAT:
				std::cout << token.value.fvalue;
			case TokenType::IDENTIFIER:
				std::cout << token.value.strvalue;
			case TokenType::STRING:
				std::cout << token.value.strvalue;
		default:
			break;
		}

		std::cout << '\n';
	}
}