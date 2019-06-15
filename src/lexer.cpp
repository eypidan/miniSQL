#include "..\include\lexer.h"

namespace Interpreter {
	Lexer::Lexer(std::istream & is)
	{
		srcText = is;
		lineNum = colNum = 0;
		getNextLine();
	}

	std::vector<Token> Lexer::tokenize()
	{
		std::vector<Token> tokens;
		tokens.push_back(getNextToken());
		return tokens;
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


		// Create a new token with line and column index
		return Token();
	}



}

