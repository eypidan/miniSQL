#pragma once
#include "lexer.h"
#include "ParseError.h"
#include "token.h"

namespace Interpreter {
	class Parser {
	private:
		Lexer lexer;
		std::vector<Token> tokens;
		std::vector<Token>::iterator itr;

	public:

	};
}