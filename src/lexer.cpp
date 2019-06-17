#include "..\include\lexer.h"
#include <stdexcept>
#include <ctype.h>

namespace Interpreter {
	Lexer::Lexer(std::istream & is):srcText(is)
	{
		lineNum = colNum = 0;
		getNextLine();
	}

	std::vector<Token> Lexer::tokenize()
	{
		std::vector<Token> tokens;
		while (!reachEnd()) {
			tokens.push_back(getNextToken());
		}
		return tokens;
	}


	bool Lexer::reachEnd()
	{
		if (pos == currLine.end() && !getNextLine()) {
			return true;
		}
		else return false;
	}


	bool Lexer::getNextLine()
	{
		if (std::getline(srcText, currLine)) {
			currLine += '\n';
			lineNum++;
			colNum = 1;
			pos = currLine.begin();
			return true;
		}
		else return false;
	}

	Token Lexer::getNextToken()
	{
		// read past and ignore any whitespace characters
		while (*pos == ' ') {
			pos++;
			colNum++;
		}

		// Create a symbol token with line and column index
		if (*pos == '*') {
			skipChars(1);
			return Token(Symbol::ASTERISK, lineNum, colNum);
		}
		else if (*pos == '(') {
			skipChars(1);
			return Token(Symbol::LBRACKET, lineNum, colNum);
		}
		else if (*pos == ')') {
			skipChars(1);
			return Token(Symbol::RBRACKET, lineNum, colNum);
		}
		else if (*pos == '+') {
			skipChars(1);
			return Token(Symbol::PLUS, lineNum, colNum);
		}
		else if (*pos == '-') {
			skipChars(1);
			return Token(Symbol::MINUS, lineNum, colNum);
		}
		else if (*pos == '<') {
			skipChars(1);
			if (*pos == '=') {
				skipChars(1);
				return Token(Symbol::LEQ, lineNum, colNum);
			}	
			else if (*pos == '>') {
				skipChars(1);
				return Token(Symbol::NEQ, lineNum, colNum);
			}
			else return Token(Symbol::LT, lineNum, colNum);
		}
		else if (*pos == '>') {
			skipChars(1);
			if (*pos == '=') {
				skipChars(1);
				return Token(Symbol::GEQ, lineNum, colNum);
			}
			else return Token(Symbol::GT, lineNum, colNum);
		}
		else if (*pos == '=') {
			skipChars(1);
			return Token(Symbol::EQ, lineNum, colNum);
		}
		// Create a string token with line and column index
		else if (*pos == '\'') {
			size_t foundEnd = currLine.substr(colNum).find_first_of('\'');
			
			if (foundEnd != std::string::npos) {
				std::string s = currLine.substr(colNum, colNum + foundEnd);
				skipChars(foundEnd);
				return Token(TokenType::STRING, s, lineNum, colNum);
			}
			else throw ParseError("Multi-line strings not supported.", lineNum, colNum);
		}
		// Create a number token with line and column index
		else if (isdigit(*pos)) {
			size_t foundEnd = currLine.substr(colNum).find_first_not_of("0123456789.");
			if (currLine.substr(colNum, colNum + foundEnd).find('.') != std::string::npos) {
				float f = atof(currLine.substr.c_str(colNum, colNum + foundEnd));
				skipChars(foundEnd);
				return Token(f, lineNum, colNum);
			}
			else {
				int i = atoi(currLine.substr.c_str(colNum, colNum + foundEnd));
				skipChars(foundEnd);
				return Token(i, lineNum, colNum);
			}	
		}
		// Create a keyword or identifier token with line and column index
		else if (isalpha(*pos)) {
			size_t foundEnd = currLine.substr(colNum).find_first_not_of("abcdefghijklmnopqrstuvwxyz_");
			std::string s = currLine.substr(colNum, colNum + foundEnd);
			skipChars(foundEnd);
			if (s == "and") {
				return Token(Keyword::AND, lineNum, colNum);
			}
			else if (s == "char") {
				return Token(Keyword::CHAR, lineNum, colNum);
			}
			else if (s == "create") {
				return Token(Keyword::CREATE, lineNum, colNum);
			}
			else if (s == "delete") {
				return Token(Keyword::DELETE, lineNum, colNum);
			}
			else if (s == "drop") {
				return Token(Keyword::DROP, lineNum, colNum);
			}
			else if (s == "execfile") {
				return Token(Keyword::EXECFILE, lineNum, colNum);
			}
			else if (s == "float") {
				return Token(Keyword::FLOAT, lineNum, colNum);
			}
			else if (s == "from") {
				return Token(Keyword::FROM, lineNum, colNum);
			}
			else if (s == "index") {
				return Token(Keyword::INDEX, lineNum, colNum);
			}
			else if (s == "insert") {
				return Token(Keyword::INSERT, lineNum, colNum);
			}
			else if (s == "int") {
				return Token(Keyword::INT, lineNum, colNum);
			}
			else if (s == "into") {
				return Token(Keyword::INTO, lineNum, colNum);
			}
			else if (s == "key") {
				return Token(Keyword::INTO, lineNum, colNum);
			}
			else if (s == "on") {
				return Token(Keyword::ON, lineNum, colNum);
			}
			else if (s == "primary") {
				return Token(Keyword::PRIMARY, lineNum, colNum);
			}
			else if (s == "quit") {
				return Token(Keyword::QUIT, lineNum, colNum);
			}
			else if (s == "select") {
				return Token(Keyword::SELECT, lineNum, colNum);
			}
			else if (s == "table") {
				return Token(Keyword::TABLE, lineNum, colNum);
			}
			else if (s == "unique") {
				return Token(Keyword::UNIQUE, lineNum, colNum);
			}
			else if (s == "values") {
				return Token(Keyword::VALUES, lineNum, colNum);
			}
			else if (s == "where") {
				return Token(Keyword::WHERE, lineNum, colNum);
			}
			else {
				return Token(TokenType::IDENTIFIER, s, lineNum, colNum);
			}
		}
		else {
			throw ParseError("Unknown Token.", lineNum, colNum);
		}
	}
}

