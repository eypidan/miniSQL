#pragma once
#include <string>
#include <memory>
#include "MetaData.h"
#include <map>

namespace CM {

	bool hasTable(std::string & tableName);
	std::shared_ptr<Table> findTable(std::string & tableName);
	void createTable(Table &table);
	void dropTable(Table &table);

	bool hasIndex(std::string & indexName);
	Index* findIndexByName(std::string & indexName);
	Index* findIndexByTable(std::string & tableName, std::string & propertyName);
	void createIndex(Index &index);
	void dropIndex(Index &index);

	void updateIndexCache();
	void ensureIndexFile();

	// indexName -> position in file
	static std::map<std::string, int> indexPositionCache;
	// indexName -> Index*
	static std::map<std::string, Index*> indexCache;
}