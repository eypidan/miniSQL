#pragma once
<<<<<<< HEAD
#include <string>
#include "MetaData.h"
=======
#include "MetaData.h"
#include <string>
>>>>>>> 3186590c5828d7c6d012c6c04bfa96a011a0adca

namespace RM {
	bool hasTable(std::string tableName);
	void createTable(Table& table);
	void dropTable(Table& table);

	void insertRecord(Table& table, std::vector<Value> record);
	int deleteAllRecords(Table& table);
	int deleteRecords(
		Table& table,
		std::vector<Predicate> & predicates
	);
	View selectRecords(
		std::vector<std::string> &properties,
		Table& table,
		std::vector<Predicate> & predicates
	);
}
