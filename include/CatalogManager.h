#pragma once
#include <string>
#include <memory>
#include <map>
#include "MetaData.h"

namespace CM {

	bool hasTable(std::string & tableName);
	std::shared_ptr<Table> findTable(std::string & tableName);
	void createTable(Table &table);
	void dropTable(Table &table);

	bool hasIndex(std::string indexName);
	Index* findIndexByName(std::string & indexName);
	Index* findIndexByTable(std::string & tableName, std::string & propertyName);
	void createIndex(Index& index);
	void dropIndex(Index& index);

	void updateIndexCache();
	void ensureIndexFile();

	static std::map<std::string, std::shared_ptr<Table>> tableCache;
	// indexName -> position in file
	static std::map<std::string, int> indexPositionCache;
	// indexName -> Index*
	static std::map<std::string, Index*> indexCache;
}