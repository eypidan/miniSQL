#pragma once
#include <string>
#include <stdlib.h>

namespace Interpreter {

enum class Symbol {
	// one-character symbols
	ASTERISK,
	SEMICOLON,
	COMMA,
	LBRACKET,
	RBRACKET,
	LT,
	GT,
	EQ,
	// two-character symbols
	LEQ,
	GEQ,
	NEQ
};

enum class Keyword {
	AND,
	CHAR,
	CREATE,
	DELETE,
	DROP,
	EXECFILE,
	FLOAT,
	FROM,
	INDEX,
	INSERT,
	INT,
	INTO,
	KEY,
	ON,
	PRIMARY,
	QUIT,
	SELECT,
	TABLE,
	UNIQUE,
	VALUES,
	WHERE
};

enum class TokenType
{
	SYMBOL, 
	KEYWORD, 	
	IDENTIFIER, 
	INTEGER, 
	FLOAT, 
	STRING
};

struct TokenValue {
	Symbol svalue;
	Keyword kvalue;
	std::string strvalue;	// IDENTIFIER or STRING
	int intvalue;
	float fvalue;
};


class Token
{
public:
	Token(const Symbol & sv, const int ln, const int cn);
	Token(const Keyword & kv, const int ln, const int cn);
	Token(TokenType t, const std::string & strv, const int ln, const int cn);
	Token(const int intv, const int ln, const int cn);
	Token(float fv, const int ln, const int cn);

	friend void printToken(Token & token);
	TokenType getType();
	TokenValue getValue();
	int getlineNum();
	int getcolNum();

private:
	TokenType type;
	TokenValue value;
	int lineNum, colNum;
};


const char *mapKeywordToString(const Keyword &);
const char *mapSymbolToString(const Symbol &);
const char *mapTokenTypeToString(const TokenType &);

}