/*
	- SPACE, TAB, NEWLINE count as whitespace.
	- How are strings (string literals) delimited? single quotes.
	What symbols and operators does the language support? ( ) + - = <= >= != > < *;
	What are the rules governing the formation of names (identifiers)? Can names contain dashes? underscores?
	What are the rules governing the formation of numbers (numeric literals)?
*/

#pragma once
#include <token.h>
#include <string>

namespace Interpreter {

	class Lexer
	{
	public:
		Lexer(std::istream &is);
		std::vector<Token> tokenize();

	private:
		bool getNextLine();
		Token getNextToken();
		std::istream &srcText;
		std::string currLine;
		std::string::iterator pos;
		int lineNum, colNum;
	};
}