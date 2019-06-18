#include "AST.h"
#include "..\include\Api.h"
#include "..\include\Exception.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace Interpreter {

	void AST::CreateTableStatement::callAPI()
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

	void AST::CreateTableStatement::setTableName(std::string & s)
	{
		tableName = s;
	}

	void AST::CreateTableStatement::setPrimaryKey(std::string & s)
	{
		primaryKey = s;
	}

	void AST::CreateTableStatement::addProperty(Property & p)
	{
		properties.push_back(p);
	}

	void AST::DropStatement::setTableName(std::string & s)
	{
		tableName = s;
	}

	void AST::DropStatement::callAPI()
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

	void AST::CreateIndexStatement::callAPI()
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

	void AST::CreateIndexStatement::setIndexName(std::string & s) {
		indexName = s;
	}

	void AST::CreateIndexStatement::setTableName(std::string & s)
	{
		tableName = s;
	}

	void AST::CreateIndexStatement::setPropertyName(std::string & s)
	{
		propertyName = s;
	}
	
	
	void AST::DropIndexStatement::setIndexName(std::string & s)
	{
		indexName = s;
	}

	void AST::DropIndexStatement::callAPI()
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
	
	void AST::SelectStatement::addRequiredProperty(std::string & s)
	{
		properties.push_back(s);
	}

	void AST::SelectStatement::addPredicate(Predicate & p)
	{
		predicates.push_back(p);
	}

	void AST::SelectStatement::setTableName(std::string & s)
	{
		tableName = s;
	}

	void AST::SelectStatement::callAPI()
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
			for (int i = 0; i < attrCnt; i++) {
				maxWidth[i] = std::max(maxWidth[i], (int)t.properties[i].name.size());
			}

			for (int i = 0; i < tupleCnt; i++) {
				for (int j = 0; j < attrCnt; j++) {
					Value & val = v[i][j];
					Type & type = val.getType();

					switch (type.getBaseType)
					{
					case BaseType::INT:
						maxWidth[j] = std::max(maxWidth[j], getLength(*(val.getAsType<int>())));
					case BaseType::FLOAT:
						maxWidth[j] = std::max(maxWidth[j], getLength(*(val.getAsType<float>())));
					case BaseType::CHAR:
						maxWidth[j] = std::max(maxWidth[j], getLength(*(val.getAsType<char>())));
					default:
						break;
					}

					
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
					Value & val = v[i][j];
					Type & t = v[i][j].getType();
					switch (t.getBaseType())
					{
					case BaseType::INT:
						std::cout << "|"
							<< std::setw(maxWidth[j] + 2)
							<< *(val.getAsType<int>());
					case BaseType::FLOAT:
						std::cout << "|"
							<< std::setw(maxWidth[j] + 2)
							<< *(val.getAsType<float>());
					case BaseType::CHAR:
						std::cout << "|"
							<< std::setw(maxWidth[j] + 2)
							<< *(val.getAsType<char>());
					default:
						break;
					}
					
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

	void AST::InsertStatement::setTableName(std::string & s)
	{
		tableName = s;
	}

	void AST::InsertStatement::addValue(Value & v)
	{
		values.push_back(v);
	}

	void AST::InsertStatement::callAPI()
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

	void AST::DeleteStatement::setTableName(std::string & s)
	{
		tableName = s;
	}

	void AST::DeleteStatement::addPredicate(Predicate & p)
	{
		predicates.push_back(p);
	}

	void AST::DeleteStatement::callAPI()
	{
		API::SQLResult<int> res = API::deleteFrom(tableName, predicates);
		if (res.isSuccess) {
			std::cout << *(res.result)
				<< "rows deleted successful in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			throw SQLException("Delete Value fails: " + res.errorMessage);
		}
	}

	void AST::QuitStatement::callAPI()
	{
		throw std::logic_error("Quit API not exists.");
	}

	void AST::ExecfileStatement::setFilePath(std::string & s)
	{
		filePath = s;
	}

	void AST::ExecfileStatement::callAPI()
	{
		throw std::logic_error("Execfile API not exists.");
	}


	


}

