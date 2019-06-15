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
		return std::vector<Token>();
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
		return Token();
	}


}

