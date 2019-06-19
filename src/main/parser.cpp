#include "ParseError.h"
#include "Parser.h"
#include <sstream>

namespace Interpreter {
	Parser::Parser(std::istream & is): lexer(is)
	{
		tokens = lexer.tokenize();
		itr = tokens.begin();
	}

	Statements Parser::parse()
	{
		Statements stats;
		while (itr != tokens.end()) {
			Token & token = *itr;
			if (token.getType() == TokenType::KEYWORD) {
				Keyword keyword = token.getValue().kvalue;
				if (keyword == Keyword::CREATE) {
					stats.push_back(parseCreate());
				}
				else if (keyword == Keyword::DROP) {
					stats.push_back(parseDrop());
				}
				else if (keyword == Keyword::SELECT) {
					stats.push_back(parseSelect());
				}
				else if (keyword == Keyword::INSERT) {
					stats.push_back(parseInsert());
				}
				else if (keyword == Keyword::DELETE) {
					stats.push_back(parseDelete());
				}
				else if (keyword == Keyword::QUIT) {
					stats.push_back(parseQuit());
				}
				else if (keyword == Keyword::EXECFILE) {
					stats.push_back(parseExecFile());
				}
				else raiseException("Invalid Statement.");
			}
			else raiseException("Invalid Statement.");

		}
		return stats;
	}

	PtrStat Parser::parseCreate()
	{
		itr++;
		Token token = *itr;
		if (token.getType() == TokenType::KEYWORD) {
			Keyword keyword = token.getValue().kvalue;
			if (keyword == Keyword::TABLE)
				return parseCreateTable();
			else if (keyword == Keyword::INDEX)
				return parseCreateIndex();
		}
		else raiseException("Invalid Create Statement.");
	}

	PtrStat Parser::parseDrop() 
	{
		itr++;	
		Token token = *itr;
		if (token.getType() == TokenType::KEYWORD) {
			Keyword keyword = token.getValue().kvalue;
			if (keyword == Keyword::TABLE)
				return parseDropTable();
			else if (keyword == Keyword::INDEX)
				return parseDropIndex();
		}
		else raiseException("Invalid Drop Statement.");
	}
	
	/* e.g.

		CREATE TABLE person ( 
			height FLOAT UNIQUE,
			pid INT,
			name CHAR(32),
			identity CHAR(128) UNIQUE,
			age INT UNIQUE,
			PRIMARY KEY(pid)
		);
	*/
	PtrStat Parser::parseCreateTable()
	{
		auto p = std::make_shared<AST::CreateTableStatement>();

		itr++;	// skip TABLE
		p->setTableName(getIdentifier());
		expect(Symbol::LBRACKET);
		bool first = true;
		do {
			if (!first) itr++;	// if not 1st loop skip COMMA
			if (meet(Keyword::PRIMARY)) {
				itr++;
				expect(Keyword::KEY);
				expect(Symbol::LBRACKET);
				std::string pKey = getIdentifier();
				expect(Symbol::RBRACKET);
				p->setPrimaryKey(pKey);
			}
			else {
				std::string* name = new std::string(getIdentifier());
				Type *type = new Type(getAttributeType());
				Property *prpt = new Property(*type, false, *name);
				if (meet(Keyword::UNIQUE)) {
					prpt->unique = true;
					itr++;
				}
				p->addProperty(*prpt);
			}
			first = false;
		} while (meet(Symbol::COMMA));
		expect(Symbol::RBRACKET);
		expect(Symbol::SEMICOLON);

		return p;
	}

	/* e.g.
		CREATE INDEX idx_height ON person(height);
	*/
	PtrStat Parser::parseCreateIndex()
	{
		auto p = std::make_shared<AST::CreateIndexStatement>();
		
		itr++;		
		p->setIndexName(getIdentifier());
		expect(Keyword::ON);
		p->setTableName(getIdentifier());
		expect(Symbol::LBRACKET);
		p->setPropertyName(getIdentifier());
		expect(Symbol::RBRACKET);
		expect(Symbol::SEMICOLON);
		return p;
	}

	/* e.g.
		DROP TABLE person;
	*/
	PtrStat Parser::parseDropTable()
	{
		auto p = std::make_shared<AST::DropStatement>();

		itr++;
		p->setTableName(getIdentifier());
		expect(Symbol::SEMICOLON);
		return p;
	}

	/* e.g.
		DROP INDEX idx_height;
	*/
	PtrStat Parser::parseDropIndex()
	{
		auto p = std::make_shared<AST::DropIndexStatement>();
		
		itr++;		
		p->setIndexName(getIdentifier());
		expect(Symbol::SEMICOLON);
		return p;
	}

	/* e.g.
		SELECT * FROM person;
		SELECT * FROM person WHERE age <= 20;
		SELECT * FROM person WHERE age <= 20 AND height < 170;
	*/
	PtrStat Parser::parseSelect()
	{

		auto p = std::make_shared<AST::SelectStatement>();

		itr++;
		if (meet(Symbol::ASTERISK)) {
			itr++;
		}
		else {
			bool first = true;
			do {
				if (!first) itr++;
				p->addRequiredProperty(getIdentifier());
				first = false;
			} while (meet(Symbol::COMMA));
		}
		expect(Keyword::FROM);
		p->setTableName(getIdentifier());
		if (meet(Keyword::WHERE)) {
			itr++;
			bool first = true;
			do {
				if (!first) itr++;
				p->addPredicate(getPredicate());
				first = false;
			} while (meet(Keyword::AND));
		}
		expect(Symbol::SEMICOLON);

		return p;
	}

	/* e.g.
		INSERT INTO person VALUES (171.1, 1, "Person1", "000001", 81);
	*/
	PtrStat Parser::parseInsert()
	{
		auto p = std::make_shared<AST::InsertStatement>();

		itr++;
		expect(Keyword::INTO);
		p->setTableName(getIdentifier());
		expect(Keyword::VALUES);
		expect(Symbol::LBRACKET);
		bool first = true;
		do {
			if (!first) itr++;
			p->addValue(getValue());
			first = false;
		} while (meet(Symbol::COMMA));
		expect(Symbol::RBRACKET);
		expect(Symbol::SEMICOLON);

		return p;
	}

	/* e.g.
		DELETE FROM person WHERE age <= 20 AND height < 170 AND identity = "000016";
	*/
	PtrStat Parser::parseDelete()
	{
		auto p = std::make_shared<AST::DeleteStatement>();

		itr++;
		expect(Keyword::FROM);
		p->setTableName(getIdentifier());
		if (meet(Keyword::WHERE)) {
			itr++;
			bool first = true;
			do {
				if (!first) itr++;
				p->addPredicate(getPredicate());
				first = false;
			} while (meet(Keyword::AND));
		}
		expect(Symbol::SEMICOLON);

		return p;
	}

	/* e.g.
		QUIT;
	*/
	PtrStat Parser::parseQuit()
	{
		auto p = std::make_shared<AST::QuitStatement>();

		itr++;
		expect(Symbol::SEMICOLON);

		return p;
	}

	/* e.g.
		EXECFILE test.txt;
	*/
	PtrStat Parser::parseExecFile()
	{
		auto p = std::make_shared<AST::ExecfileStatement>();

		itr++;
		p->setFilePath(getString());
		expect(Symbol::SEMICOLON);

		return p;
	}

	void Parser::raiseException(const std::string & msg) {
		if (itr == tokens.end()) {
			throw ParseError(msg, -1, -1);
		}
		else {
			throw ParseError(msg, itr->getlineNum(), itr->getcolNum());
		}
	}

	void Parser::expect(const Keyword & k)
	{
		if (meet(k)) {
			itr++;
		}
		else {
			std::stringstream s;
			s << "Expecting \'"
				<< mapKeywordToString(k)
				<< "\'";
			raiseException(s.str());
		}
	}
	void Parser::expect(const Symbol & s)
	{
		if (meet(s)) {
			itr++;
		}
		else {
			std::stringstream ss;
			ss << "Expecting \'"
				<< mapSymbolToString(s)
				<< "\'.";
			raiseException(ss.str());
		}
	}

	bool Parser::meet(const Keyword & k)
	{
		return (itr != tokens.end() && itr->getType() == TokenType::KEYWORD && itr->getValue().kvalue == k);
	}

	bool Parser::meet(const Symbol & s)
	{
		return (itr != tokens.end() && itr->getType() == TokenType::SYMBOL && itr->getValue().svalue == s);;
	}

	std::string Parser::getIdentifier()
	{
		if (itr != tokens.end() && itr->getType() == TokenType::IDENTIFIER) {
			return (itr++)->getValue().strvalue;
		}
		else {
			raiseException("Expecting an identifier.");
		}
	}
	

	Type Parser::getAttributeType()
	{
		if (meet(Keyword::INT)) {
			itr++;
			return Type(BaseType::INT);
		}
		else if (meet(Keyword::FLOAT)) {
			itr++;
			return Type(BaseType::FLOAT);
		}
		else if (meet(Keyword::CHAR)) {
			itr++;
			expect(Symbol::LBRACKET);
			int* size = new int(getInteger());
			if (*size > 255) {
				raiseException("Char size should not exceed 255.");
			}
			else if (*size < 1) {
				raiseException("Char size should not be smaller than 1.");
			}
			expect(Symbol::RBRACKET);
			return Type(BaseType::CHAR, *size);
		}
		else {
			raiseException("Expecting \'int\', \'float\' or \'char\'.");
		}
	}

	Predicate Parser::getPredicate()
	{
		std::string* propertyName = new std::string(getIdentifier());
		OpType *op = new OpType;
		if (meet(Symbol::EQ)) {
			*op = OpType::EQ;
		}
		else if (meet(Symbol::NEQ)) {
			*op = OpType::NE;
		}
		else if (meet(Symbol::LT)) {
			*op = OpType::LT;
		}
		else if (meet(Symbol::LEQ)) {
			*op = OpType::LEQ;
		}
		else if (meet(Symbol::GT)) {
			*op = OpType::GT;
		}
		else if (meet(Symbol::GEQ)) {
			*op = OpType::GEQ;
		}
		else raiseException("Expecting '=', '<>', '<', '<=', '>', '>='");
		itr++;
		Value *val = new Value(getValue());
		return Predicate(*propertyName, *op, *val);
	}

	Value Parser::getValue()
	{
		if (itr != tokens.end()) {
			if (itr->getType() == TokenType::INTEGER) {
				Type* type = new Type(BaseType::INT);
				int* val = new int((itr++)->getValue().intvalue);
				return Value(*type, val);
			}
			else if (itr->getType() == TokenType::FLOAT) {
				Type* type = new Type(BaseType::FLOAT);
				float* val = new float((itr++)->getValue().fvalue);
				return Value(*type, val);
			}
			else if (itr->getType() == TokenType::STRING) {
				Type* type = new Type(BaseType::CHAR);
				std::string* val = new std::string((itr++)->getValue().strvalue);
				if (val->length() >= 1 && val->length() <= 255) {
					return Value(*type, val->c_str());
				}
				else {
					raiseException("Invalid length of string literal.");
				}
			}
		}
		raiseException("Expecting int, float or string.");
	}

	int Parser::getInteger()
	{
		if (itr != tokens.end() && itr->getType() == TokenType::INTEGER) {
			return (itr++)->getValue().intvalue;
		}
		else {
			raiseException("Expecting an integer.");
		}
	}
	float Parser::getFloat()
	{
		if (itr != tokens.end() && itr->getType() == TokenType::FLOAT) {
			return (itr++)->getValue().fvalue;
		}
		else {
			raiseException("Expecting a float number.");
		}
	}
	std::string Parser::getString()
	{
		if (itr != tokens.end() && itr->getType() == TokenType::STRING) {
			return (itr++)->getValue().strvalue;
		}
		else {
			raiseException("Expecting string.");
		}
	}
}