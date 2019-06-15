#include <lexer.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

#define IFILENAME "lexer_test.sql"

using namespace Interpreter;



int main() {
	std::ifstream fin(IFILENAME);
	std::istream is;
	fin >> is;
	Lexer lexer(is);
	
	vector<Token> tokens = lexer.tokenize();
	std::for_each(tokens.begin(), tokens.end(), printToken);

	return 0;
}