#include <lexer.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

#define IFILENAME "lexer_test.sql"

using namespace Interpreter;

int main() {
	std::ifstream fin(IFILENAME);
	std::istream & is = fin;
	Lexer lexer(is);
	
	std::vector<Token> tokens = lexer.tokenize();
	std::for_each(tokens.begin(), tokens.end(), printToken);

	return 0;
}