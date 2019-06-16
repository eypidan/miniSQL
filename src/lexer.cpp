#include "..\include\lexer.h"
#include <stdexcept>

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

		// Create a new token with line and column index
		if (*pos)
		return Token();
	}



}

