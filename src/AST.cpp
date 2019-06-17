#include "AST.h"
#include "../include/Api.h"
#include <iostream>

namespace Interpreter {

	void AST::CreateTableStatement::callAPI() const
	{
		Table table(tableName, primaryKey, properties);
		API::createTable(table);
		std::cout << "Table \'" << tableName << "\' created." << std::endl;
	}

	void AST::DropStatement::callAPI() const
	{
		API::dropTable(tableName);
		std::cout << "Table \'" << tableName << "\' created." << std::endl;	
	}

	void AST::CreateIndexStatement::callAPI() const
	{
		Index index(indexName, tableName, propertyName);
		API::createIndex(index);
		std::cout << "Index \'" << indexName << "\' created on property \'" << propertyName << "." << std::endl;
		// TODO
	}
	
	void AST::DropIndexStatement::callAPI() const
	{
		API::dropIndex(indexName);
		std::cout << "Index \'" << indexName << "\' dropped." << std::endl;
	}

	void AST::SelectStatement::callAPI() const
	{
		API::select(properties, tableName, predicates);
		
	}


}


