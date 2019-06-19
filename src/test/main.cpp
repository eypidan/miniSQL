#include "lexer.h"
#include "Parser.h"
#include "REPL.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <crtdbg.h>

#define IFILENAME "lexer_test.sql"

using namespace Interpreter;

void testLexer() {
	std::ifstream fin(IFILENAME);
	if (!fin) {
		std::cout << "FILE ERROR" << std::endl;
	}
	else {
		std::istream & is = fin;
		Lexer lexer(is);

		std::vector<Token> tokens = lexer.tokenize();

		std::for_each(tokens.begin(), tokens.end(), printToken);
	}
}

void testParser(){
	
	std::ifstream fin(IFILENAME);
	if (!fin) {
		std::cout << "FILE ERROR" << std::endl;
	}
	else {
		std::istream & is = fin;
		Parser parser(is);

		Statements statements = parser.parse();

		for (PtrStat statement : statements) {
			statement->printStatement();
		}
	}
}

void testREPL() {
	REPL repl;
	repl.run();
}

int main() {
	//_CrtSetBreakAlloc(30788);
	//testLexer();
	//testParser();
	testREPL();
	_CrtDumpMemoryLeaks();
	return 0;
}