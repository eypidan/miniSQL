#include "AST.h"
#include "..\include\Api.h"
#include "..\include\Exception.h"
#include <iostream>
#include <iomanip>

namespace Interpreter {

	


	void AST::CreateTableStatement::callAPI() const
	{
		Table table(tableName, primaryKey, properties);
		
		API::SQLResult<void> res = API::createTable(table);
		if (res.isSuccess) {
			std::cout << "Table \'" << tableName
				<< "\' created successfully in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			throw SQLException("Create Table fails: " + res.errorMessage);
		}
		
	}

	void AST::DropStatement::callAPI() const
	{
		API::SQLResult<void> res = API::dropTable(tableName);
		if (res.isSuccess) {
			std::cout << "Table \'" << tableName
				<< "\' dropped successfully in " << res.durationMS
				<< " ms" << std::endl;
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
			std::cout << "Index \'" << indexName
				<< "\' created on property \'" << propertyName
				<< "\' successfully in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			throw SQLException("Create Index fails: " + res.errorMessage);
		}
	}
		
	
	void AST::DropIndexStatement::callAPI() const
	{
		API::SQLResult<void> res = API::dropIndex(indexName);

		if (res.isSuccess) {
			std::cout << "Index \'" << indexName 
				<< "\' dropped successfully in " << res.durationMS 
				<< " ms" << std::endl;
		}
		else {
			throw SQLException("Drop Index fails: " + res.errorMessage);
		}
	}

	void DrawSymbol(int n, char c) {
		for (int i = 0; i < n; i++) {
			std::cout << c << std::endl;
		}
	}
	
	void AST::SelectStatement::callAPI() const
	{
		API::SQLResult<std::pair<View, Table>> res = API::select(properties, tableName, predicates);
		if (res.isSuccess) {

			View v = res.result->first;
			int tupleCnt = v.size();
			if (tupleCnt == 0) {
				std::cout << "No record selected." << std::endl;
				return;
			}
			
			Table t = res.result->second;
			int attrCnt = t.properties.size();

			// calculate the maxWidth of each column
			std::vector<int> maxWidth(attrCnt, 0);
			for (i = 0; i < attrCnt; i++) {
				maxWidth[i] = max(maxWidth[i], t.properties[i].name.size())
			}
			for (i = 0; i < tupleCnt; i++) {
				for (j = 0; j < attrCnt; j++) {
					maxWidth[j] = max(maxWidth[j], v[i][j].getAsType().toString().size());
				}
			}
			
			// Draw output table
			for (int i = 0; i < attrCnt; i++) {
				std::cout << "+";
				DrawSymbol(maxWidth[i] + 2, '-');
			}
			std::cout << "+";
			std::cout << std::endl;

			for (int i = 0; i < attrCnt; i++) {
				std::cout << "|"
					<< std::setw(maxWidth[i] + 2)
					<< t.properties[i].name;
			}
			std::cout << "|" << std::endl;

			for (int i = 0; i < attrCnt; i++) {
				std::cout << "+";
				DrawSymbol(maxWidth[i] + 2, '-');
			}
			std::cout << "+";
			std::cout << std::endl;

			for (int i = 0; i < tupleCnt; i++) {
				for (int j = 0; j < attrCnt; j++) {
					std::cout << "|"
						<< std::setw(maxWidth[j] + 2)
						<< v[i][j].getAsType;
				}
			}
			std::cout << "|" << std::endl;

			for (int i = 0; i < attrCnt; i++) {
				std::cout << "+";
				DrawSymbol(maxWidth[i] + 2, '-');
			}
			std::cout << "+";
			std::cout << std::endl;

			std::cout << tupleCnt << " rows in set (" << res.durationMS << " ms)";
		}
		else {
			throw SQLException("Select fails: " + res.errorMessage);
		}
	}

	void AST::InsertStatement::callAPI() const
	{
		API::SQLResult<void> res = API::insert(tableName, values);
		if (res.isSuccess) {
			std::cout << "Insertion on Table \'" << tableName
				<< "\' is successful in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			throw SQLException("Insert Value fails: " + res.errorMessage);
		}
	}

	void AST::DeleteStatement::callAPI() const
	{
		API::SQLResult<void> res = API::deleteFrom(tableName, predicates);
		if (res.isSuccess) {
			std::cout << "Deletion on Table \'" << tableName
				<< "\' is successful in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			throw SQLException("Delete Value fails: " + res.errorMessage);
		}
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

