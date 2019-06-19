#include "lexer.h"
#include <stdexcept>
#include <ctype.h>
#include <ParseError.h>

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

	void Lexer::skipSpaces(){
		while (pos != currLine.end() && (*pos == ' ' || *pos == '\t')) {
			skipChars(1);
		}
	}

	bool Lexer::getNextLine()
	{
		if (std::getline(srcText, currLine)) {
			lineNum++;
			colNum = 1;
			pos = currLine.begin();
			return true;
		}
		else return false;
	}

	void Lexer::skipChars(int n) {
		colNum += n;
		pos += n;
	}

	Token Lexer::getNextToken()
	{
		// read past and ignore any whitespace characters
		skipSpaces();
		if (pos == currLine.end()) {
			getNextLine();
			skipSpaces();
		}

		int currLineNum = lineNum;
		int currColNum = colNum;

		// Create a symbol token with line and column index
		if (*pos == '*') {
			skipChars(1);
			return Token(Symbol::ASTERISK, currLineNum, currColNum);
		}
		else if (*pos == ';') {
			skipChars(1);
			return Token(Symbol::SEMICOLON, currLineNum, currColNum);
		}
		else if (*pos == ',') {
			skipChars(1);
			return Token(Symbol::COMMA, currLineNum, currColNum);
		}
		else if (*pos == '(') {
			skipChars(1);
			return Token(Symbol::LBRACKET, currLineNum, currColNum);
		}
		else if (*pos == ')') {
			skipChars(1);
			return Token(Symbol::RBRACKET, currLineNum, currColNum);
		}
		else if (*pos == '<') {
			skipChars(1);
			if (*pos == '=') {
				skipChars(1);
				return Token(Symbol::LEQ, currLineNum, currColNum);
			}	
			else if (*pos == '>') {
				skipChars(1);
				return Token(Symbol::NEQ, currLineNum, currColNum);
			}
			else return Token(Symbol::LT, currLineNum, currColNum);
		}
		else if (*pos == '>') {
			skipChars(1);
			if (*pos == '=') {
				skipChars(1);
				return Token(Symbol::GEQ, currLineNum, currColNum);
			}
			else return Token(Symbol::GT, currLineNum, currColNum);
		}
		else if (*pos == '=') {
			skipChars(1);
			return Token(Symbol::EQ, currLineNum, currColNum);
		}
		// Create a string token with line and column index
		else if (*pos == '\'' || *pos == '"') {
			size_t foundEnd = currLine.substr(currColNum).find_first_of('\'"');
			
			if (foundEnd != std::string::npos) {
				std::string s = currLine.substr(currColNum, foundEnd);
				skipChars(foundEnd + 2);
				return Token(TokenType::STRING, s, currLineNum, currColNum);
			}
			else throw ParseError("Multi-line strings not supported.", currLineNum, currColNum);
		}
		// Create a number token with line and column index
		else if (isdigit(*pos)) {
			size_t foundEnd = currLine.substr(currColNum).find_first_not_of("0123456789.");
			if (currLine.substr(currColNum - 1, foundEnd + 1).find('.') != std::string::npos) {
				float f = atof(currLine.substr(currColNum - 1, foundEnd + 1).c_str());
				skipChars(foundEnd + 1);
				return Token(f, currLineNum, currColNum);
			}
			else {
				int i = atoi(currLine.substr(currColNum - 1, foundEnd + 1).c_str());
				skipChars(foundEnd + 1);
				return Token(i, currLineNum, currColNum);
			}	
		}
		// Create a keyword or identifier token with line and column index
		else if (isalpha(*pos)) {
			size_t foundEnd = currLine.substr(currColNum).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_");
			std::string s = currLine.substr(currColNum - 1, foundEnd + 1);
			skipChars(foundEnd + 1);
			if (s == "and") {
				return Token(Keyword::AND, currLineNum, currColNum);
			}
			else if (s == "char") {
				return Token(Keyword::CHAR, currLineNum, currColNum);
			}
			else if (s == "create") {
				return Token(Keyword::CREATE, currLineNum, currColNum);
			}
			else if (s == "delete") {
				return Token(Keyword::DELETE, currLineNum, currColNum);
			}
			else if (s == "drop") {
				return Token(Keyword::DROP, currLineNum, currColNum);
			}
			else if (s == "execfile") {
				return Token(Keyword::EXECFILE, currLineNum, currColNum);
			}
			else if (s == "float") {
				return Token(Keyword::FLOAT, currLineNum, currColNum);
			}
			else if (s == "from") {
				return Token(Keyword::FROM, currLineNum, currColNum);
			}
			else if (s == "index") {
				return Token(Keyword::INDEX, currLineNum, currColNum);
			}
			else if (s == "insert") {
				return Token(Keyword::INSERT, currLineNum, currColNum);
			}
			else if (s == "int") {
				return Token(Keyword::INT, currLineNum, currColNum);
			}
			else if (s == "into") {
				return Token(Keyword::INTO, currLineNum, currColNum);
			}
			else if (s == "key") {
				return Token(Keyword::KEY, currLineNum, currColNum);
			}
			else if (s == "on") {
				return Token(Keyword::ON, currLineNum, currColNum);
			}
			else if (s == "primary") {
				return Token(Keyword::PRIMARY, currLineNum, currColNum);
			}
			else if (s == "quit") {
				return Token(Keyword::QUIT, currLineNum, currColNum);
			}
			else if (s == "select") {
				return Token(Keyword::SELECT, currLineNum, currColNum);
			}
			else if (s == "table") {
				return Token(Keyword::TABLE, currLineNum, currColNum);
			}
			else if (s == "unique") {
				return Token(Keyword::UNIQUE, currLineNum, currColNum);
			}
			else if (s == "values") {
				return Token(Keyword::VALUES, currLineNum, currColNum);
			}
			else if (s == "where") {
				return Token(Keyword::WHERE, currLineNum, currColNum);
			}
			else {
				return Token(TokenType::IDENTIFIER, s, currLineNum, currColNum);
			}
		}
		else {
			throw ParseError("Unknown Token.", currLineNum, currColNum);
		}
	}
}

