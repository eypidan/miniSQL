#pragma once
#include "MetaData.h"
#include <string>
#include "BufferManager.h"
#include "IndexManager.h"
#include <functional>

namespace RM {
	bool hasTable(std::string& tableName);
	void createTable(Table& table);
	void dropTable(Table& table);

	void insertRecord(Table& table, std::vector<Value> record);
	// return delete count
	int deleteAllRecords(Table& table);
	// return delete count
	int deleteRecords(
		Table& table,
		std::vector<Predicate> & predicates
	);
	View selectRecords(
		std::vector<std::string> &properties,
		Table& table,
		std::vector<Predicate> & predicates
	);

	size_t getRecordSize(Table& table);
	void checkExist(Table& table);
	void checkTypes(Table& table, std::vector<Value> record);
	std::shared_ptr<std::vector<Value>> readRecord(Table& table, char* ptr);
	void writeRecord(std::vector<Value> records, char* ptr);
	void saveIndexes(Table& table, std::vector<Value> record, int id);
	void deleteIndexes(Table& table, std::shared_ptr<std::vector<Value>> record);
	int count(Table& table);
	// Search for the table according to predicates. Process each record 
	// found with given function.
	void forEach(
		Table& table, 
		std::vector<Predicate>& predicates, 
		std::function<bool(BlockNode*, size_t, std::shared_ptr<std::vector<Value>>)> consumer // (blocknode, offset, record) => (goOn)
	);
	void forEachLinear(
		Table& table,
		std::vector<Predicate>& predicates,
		std::function<bool(BlockNode*, size_t, std::shared_ptr<std::vector<Value>>)> consumer // (blocknode, offset, record) => (goOn)
	);
	void forEachIndexLeave(
		Table& table,
		std::shared_ptr<IndexIterator> iter,
		FileNode* file,
		std::vector<Predicate>& predicates,
		std::function<bool(BlockNode*, size_t, std::shared_ptr<std::vector<Value>>)> consumer // (blocknode, offset, record) => (goOn)
	);
}
