#pragma once
#include <string>
#include <memory>
#include "MetaData.h"

namespace CM {

	bool hasTable(std::string & tableName);
	std::shared_ptr<Table> findTable(std::string & tableName);
	void createTable(Table &table);
	void dropTable(Table &table);

	bool hasIndex(std::string & indexName);
	std::shared_ptr<Table> findIndex(std::string & indexName);
	void createIndex(Index &index);
	void dropIndex(Index &index);

}