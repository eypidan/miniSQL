#pragma once
#include <string>
#include <MetaData.h>

namespace CM {

	bool hasTable(std::string & tableName);
	Table& findTable(std::string & tableName);
	void createTable(Table &table);
	void dropTable(Table &table);

	bool hasIndex(std::string & indexName);
	Index& findIndex(std::string & indexName);
	void createIndex(Index &index);
	void dropIndex(Index &index);

}