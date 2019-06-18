#include "AST.h"
#include "Api.h"
#include "..\include\Exception.h"
#include <iostream>

namespace Interpreter {

	void AST::CreateTableStatement::callAPI() const
	{
		Table table(tableName, primaryKey, properties);
		
		API::SQLResult<void> res = API::createTable(table);
		if (res.isSuccess) {
			std::cout << "Table \'" << tableName << "\' created." << std::endl;
			std::cout << "Time elapsed: " << res.durationMS << " ms" << std::endl;
		}
		else {
			throw SQLException("Create Table fails: " + res.errorMessage);
		}
		
	}

	void AST::DropStatement::callAPI() const
	{
		API::SQLResult<void> res = API::dropTable(tableName);
		if (res.isSuccess) {
			std::cout << "Table \'" << tableName << "\' created." << std::endl;
			std::cout << "Time elapsed: " << res.durationMS << " ms" << std::endl;
		}
		else {
			throw SQLException("Drop Table fails: " + res.errorMessage);
		}
		
	}

	void AST::CreateIndexStatement::callAPI() const
	{
		Index index(indexName, tableName, propertyName);

		API::SQLResult<void> res = API::createIndex(index);
		if (res.isSuccess) {
			std::cout << "Index \'" << indexName << "\' created on property \'" << propertyName << "." << std::endl;
			std::cout << "Time elapsed: " << res.durationMS << " ms" << std::endl;
		}
		else {
			throw SQLException("Create Index fails: " + res.errorMessage);
		}
	}
		
	
	void AST::DropIndexStatement::callAPI() const
	{
		API::SQLResult<void> res = API::dropIndex(indexName);

		if (res.isSuccess) {
			std::cout << "Index \'" << indexName << "\' dropped." << std::endl;
			std::cout << "Time elapsed: " << res.durationMS << " ms" << std::endl;
		}
		else {
			throw SQLException("Drop Index fails: " + res.errorMessage);
		}
	}

	void AST::SelectStatement::callAPI() const
	{
		API::SQLResult<View> res = API::select(properties, tableName, predicates);
		std::cout << ""
	}

	void AST::InsertStatement::callAPI() const
	{
		API::SQLResult<void> res = API::insert(tableName, values);
	}

	void AST::DeleteStatement::callAPI() const
	{
		API::SQLResult<void> res = API::deleteFrom(tableName, predicates);

	}

	void AST::QuitStatement::callAPI() const
	{
		throw std::logic_error("Quit API not exists.");
	}

	void AST::ExecfileStatement::callAPI() const
	{
		throw std::logic_error("Execfile API not exists.");
	}


	


}

