#include "..\include\Parser.h"

namespace Interpreter {
	Parser::Parser(std::istream & is): lexer(is)
	{
		tokens = lexer.tokenize();
		itr = tokens.begin();
	}

	Statements Parser::parse()
	{
		return Statements();
	}
}