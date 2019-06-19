#include "AST.h"
#include "Api.h"
#include "Exception.h"
#include <iostream>
#include <iomanip>
#include <algorithm>



namespace Interpreter {

	/*void AST::CreateTableStatement::callAPI() 
	{
		Table table(tableName, primaryKey, properties);
		
		API::SQLResult<void> res = API::createTable(table);
		if (res.isSuccess) {
			std::cout << "Table \'" << tableName
				<< "\' created successfully in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			std::cout << "Create Table fails: " + res.errorMessage << std::endl;
		}
	}*/

	void AST::CreateTableStatement::printStatement()
	{
		std::cout << "CREATE TABLE" << std::endl;
		std::cout << "\t " << tableName << std::endl;
		std::cout << "\t " << primaryKey << std::endl;
		for (auto property : properties) {
			std::cout << "\t "
				<< property.name
				<< "\t ";
			if (property.isUnique)
				std::cout << "Unique" << std::endl;
			else std::cout << std::endl;
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

	void AST::CreateTableStatement::addProperty(const Property & p)
	{
		properties.push_back(p);
	}

	void AST::DropStatement::setTableName(std::string & s)
	{
		tableName = s;
	}

	/*void AST::DropStatement::callAPI()
	{
		API::SQLResult<void> res = API::dropTable(tableName);
		if (res.isSuccess) {
			std::cout << "Table \'" << tableName
				<< "\' dropped successfully in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			std::cout << "Drop Table fails: " + res.errorMessage << std::endl;
		}
	}*/

	void AST::DropStatement::printStatement()
	{
		std::cout << "DROP TABLE" << std::endl;
		std::cout << "\t " << tableName << std::endl;
	}

	/*void AST::CreateIndexStatement::callAPI()
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
			std::cout << "Create Index fails: " + res.errorMessage << std::endl;
		}
	}*/

	void AST::CreateIndexStatement::printStatement()
	{
		std::cout << "CREATE INDEX" << std::endl;
		std::cout << "\t " << indexName << std::endl;
		std::cout << "\t " << tableName << std::endl;
		std::cout << "\t " << propertyName << std::endl;
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

	/*void AST::DropIndexStatement::callAPI()
	{
		API::SQLResult<void> res = API::dropIndex(indexName);

		if (res.isSuccess) {
			std::cout << "Index \'" << indexName
				<< "\' dropped successfully in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			std::cout << "Drop Index fails: " + res.errorMessage << std::endl;
		}
	}*/

	void AST::DropIndexStatement::printStatement()
	{
		std::cout << "DROP INDEX" << std::endl;
		std::cout << "\t " << indexName << std::endl;
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

	//void AST::SelectStatement::callAPI()
	//{
	//	API::SQLResult<std::pair<View, Table>> res = API::select(properties, tableName, predicates);
	//	if (res.isSuccess) {
	//		View v = res.result->first;
	//		int tupleCnt = v.size();
	//		if (tupleCnt == 0) {
	//			std::cout << "No record selected." << std::endl;
	//			return;
	//		}

	//		Table t = res.result->second;
	//		int attrCnt = t.properties.size();

	//		// calculate the maxWidth of each column
	//		std::vector<int> maxWidth(attrCnt, 0);
	//		for (int i = 0; i < attrCnt; i++) {
	//			maxWidth[i] = std::max(maxWidth[i], (int)t.properties[i].name.size());
	//		}

	//		for (int i = 0; i < tupleCnt; i++) {
	//			for (int j = 0; j < attrCnt; j++) {
	//				Value *val = &v[i][j];
	//				Type *type = &(val->getType());

	//				switch (type->getBaseType())
	//				{
	//				case BaseType::INT:
	//					maxWidth[j] = std::max(maxWidth[j], getLength(*(val->getAsType<int>())));
	//				case BaseType::FLOAT:
	//					maxWidth[j] = std::max(maxWidth[j], getLength(*(val->getAsType<float>())));
	//				case BaseType::CHAR:
	//					maxWidth[j] = std::max(maxWidth[j], getLength(val->getAsType<char>()));
	//				default:
	//					break;
	//				}
	//			}
	//		}

	//		// Draw output table
	//		for (int i = 0; i < attrCnt; i++) {
	//			std::cout << "+";
	//			DrawSymbol(maxWidth[i] + 2, '-');
	//		}
	//		std::cout << "+";
	//		std::cout << std::endl;

	//		for (int i = 0; i < attrCnt; i++) {
	//			std::cout << "|"
	//				<< std::setw(maxWidth[i] + 2)
	//				<< t.properties[i].name;
	//		}
	//		std::cout << "|" << std::endl;

	//		for (int i = 0; i < attrCnt; i++) {
	//			std::cout << "+";
	//			DrawSymbol(maxWidth[i] + 2, '-');
	//		}
	//		std::cout << "+";
	//		std::cout << std::endl;

	//		for (int i = 0; i < tupleCnt; i++) {
	//			for (int j = 0; j < attrCnt; j++) {
	//				Value *val = &v[i][j];
	//				Type *t = &(val->getType());
	//				switch (t->getBaseType())
	//				{
	//				case BaseType::INT:
	//					std::cout << "|"
	//						<< std::setw(maxWidth[j] + 2)
	//						<< *(val->getAsType<int>());
	//				case BaseType::FLOAT:
	//					std::cout << "|"
	//						<< std::setw(maxWidth[j] + 2)
	//						<< *(val->getAsType<float>());
	//				case BaseType::CHAR:
	//					std::cout << "|"
	//						<< std::setw(maxWidth[j] + 2)
	//						<< val->getAsType<char>();
	//				default:
	//					break;
	//				}

	//			}
	//		}

	//		std::cout << "|" << std::endl;

	//		for (int i = 0; i < attrCnt; i++) {
	//			std::cout << "+";
	//			DrawSymbol(maxWidth[i] + 2, '-');
	//		}
	//		std::cout << "+";
	//		std::cout << std::endl;

	//		std::cout << tupleCnt << " rows in set (" << res.durationMS << " ms)";
	//	}
	//	else {
	//		std::cout << "Select fails: " + res.errorMessage << std::endl;
	//	}
	//}

	void AST::SelectStatement::printStatement()
	{
		std::cout << "SELECT" << std::endl;
		for (auto property : properties) {
			std::cout << "\t "
				<< property
				<< std::endl;
		}
		std::cout << "\t " << tableName << std::endl;
		for (auto predicate : predicates) {
			std::cout << "\t "
				<< std::left
				<< std::setw(31)
				<< predicate.propertyName
				<< std::setw(31)
				<< predicate.op;
			switch (predicate.val.getType().getBaseType())
			{
			case BaseType::INT:
				std::cout << std::left
					<< std::setw(31)
					<< *(predicate.val.getAsType<int>())
					<< std::endl;
				break;
			case BaseType::FLOAT:
				std::cout << std::left
					<< std::setw(31)
					<< *(predicate.val.getAsType<float>())
					<< std::endl;
				break;
			case BaseType::CHAR:
				std::cout << std::left
					<< std::setw(31)
					<< predicate.val.getAsType<char>()
					<< std::endl;
				break;
			default:
				break;
			}
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

	/*void AST::InsertStatement::callAPI()
	{
		API::SQLResult<void> res = API::insert(tableName, values);
		if (res.isSuccess) {
			std::cout << "Insertion on Table \'" << tableName
				<< "\' is successful in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			std::cout << "Insert Value fails: " + res.errorMessage << std::endl;
		}
	}*/

	void AST::InsertStatement::printStatement()
	{
		std::cout << "INSERT" << std::endl;

		for (auto value : values) {
			switch (value.getType().getBaseType())
			{
			case BaseType::INT:
				std::cout << "\t "
					<< *(value.getAsType<int>())
					<< std::endl;
				break;
			case BaseType::FLOAT:
				std::cout << "\t "
					<< *(value.getAsType<float>())
					<< std::endl;
				break;
			case BaseType::CHAR:
				std::cout << "\t "
					<< value.getAsType<char>()
					<< std::endl;
				break;
			default:
				break;
			}
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

	/*void AST::DeleteStatement::callAPI()
	{
		API::SQLResult<int> res = API::deleteFrom(tableName, predicates);
		if (res.isSuccess) {
			std::cout << *(res.result)
				<< "rows deleted successful in " << res.durationMS
				<< " ms" << std::endl;
		}
		else {
			std::cout << "Delete Value fails: " + res.errorMessage << std::endl;
		}
	}*/

	void AST::DeleteStatement::printStatement()
	{
		std::cout << "DELETE" << std::endl;
		std::cout << "\t " << tableName << std::endl;
		for (auto predicate : predicates) {
			std::cout << "\t "
				<< std::left
				<< std::setw(31)
				<< predicate.propertyName
				<< std::setw(31)
				<< predicate.op;
			switch (predicate.val.getType().getBaseType())
			{
			case BaseType::INT:
				std::cout << std::left
					<< std::setw(31)
					<< *(predicate.val.getAsType<int>())
					<< std::endl;
				break;
			case BaseType::FLOAT:
				std::cout << std::left
					<< std::setw(31)
					<< *(predicate.val.getAsType<float>())
					<< std::endl;
				break;
			case BaseType::CHAR:
				std::cout << std::left
					<< std::setw(31)
					<< predicate.val.getAsType<char>()
					<< std::endl;
				break;
			default:
				break;
			}
		}
	}

	/*void AST::QuitStatement::callAPI()
	{
		throw std::logic_error("Quit API does not exists.");
	}*/

	void AST::QuitStatement::printStatement()
	{
		std::cout << "QUIT" << std::endl;
	}

	void AST::ExecfileStatement::setFilePath(std::string & s)
	{
		filePath = s;
	}

	void AST::ExecfileStatement::printStatement()
	{
		std::cout << "EXECFILE" << std::endl;
		std::cout << ' \t' << filePath << std::endl;
	}

	/*void AST::ExecfileStatement::callAPI()
	{
		throw std::logic_error("Execfile API does not exists.");
	}*/


	


}

