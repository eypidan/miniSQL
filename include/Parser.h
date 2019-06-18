#pragma once
#include "lexer.h"
#include "ParseError.h"
#include "token.h"
#include "..\include\AST.h"
#include <vector>

namespace Interpreter {
	typedef std::shared_ptr<AST::Statement> PtrStat;
	typedef std::vector<ptrStat> Statements;
	
	class Parser {
	private:
		Lexer lexer;
		std::vector<Token> tokens;
		std::vector<Token>::iterator itr;

	public:
		Parser(std::istream & is);
		Statements parse();

		PtrStat parseCreate();
		PtrStat parseDrop();
		PtrStat parseCreateTable();
		PtrStat parseCreateIndex();
		PtrStat parseDropTable();
		PtrStat parseDropIndex();
		PtrStat parseSelect();
		PtrStat parseInsert();
		PtrStat parseDelete();
		PtrStat parseQuit();
		PtrStat parseExecFile();
	};
}