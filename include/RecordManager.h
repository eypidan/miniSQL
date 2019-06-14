#pragma once
<<<<<<< HEAD

#include "MetaData.h"
=======
#include <MetaData.h>
>>>>>>> dd73bdcee87841e3db7175448dd3a0fba3a16d0c
#include <string>

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
