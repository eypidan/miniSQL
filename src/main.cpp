#include "lexer.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

#define IFILENAME "lexer_test.sql"

using namespace Interpreter;

int main() {
	std::ifstream fin(IFILENAME);
	if (!fin) {
		std::cout << "FILE ERROR" << std::endl;
	}
	else {
		std::istream & is = fin;
		Lexer lexer(is);

		std::vector<Token> tokens = lexer.tokenize();

		/*for (auto itr = tokens.begin(); itr != tokens.end(); itr++)
			printToken(*itr);*/
		std::for_each(tokens.begin(), tokens.end(), printToken);
	}


	return 0;
}