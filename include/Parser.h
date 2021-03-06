#pragma once
#include "lexer.h"
#include "ParseError.h"
#include "token.h"
#include "..\include\AST.h"
#include <vector>

namespace Interpreter {
	typedef std::shared_ptr<AST::Statement> PtrStat;
	typedef std::vector<PtrStat> Statements;
	
	class Parser {
	private:
		Lexer lexer;
		std::vector<Token> tokens;
		std::vector<Token>::iterator itr;

	public:
		Parser(std::istream & is);
		Statements parse();
		// utilities
		void raiseException(const std::string & msg);
		void expect(const Keyword &);	// parse a specific keyword, raise exception if keyword not found
		void expect(const Symbol &);	// parse a specific symbol, raise exception if symbol not found
		bool meet(const Keyword &);	// check for a specific keyword (do not proceed)
		bool meet(const Symbol &);		// check for a specific sumbol (do not proceed)
		// functions to get a specific type of token and proceed
		std::string getIdentifier();
		Type getAttributeType();
		Predicate getPredicate();
		Value getValue();
		int getInteger();
		float getFloat();
		std::string getString();
		// parse a specific type of statement
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