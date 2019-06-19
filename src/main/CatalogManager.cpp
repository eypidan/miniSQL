#include "CatalogManager.h"
#include "MetaData.h"
#include "Exception.h"
#include "IndexManager.h"
#include "BufferManager.h"

#define INDEX_FILE "index.struct"
#define TABLE_STRUCT_FILE(x) (x + "_table.struct")
#define STR_LEN 64

namespace CM {
	using namespace std;
	bool hasTable(std::string & tableName) {
		return bufferManager.JudgeFileExistence(TABLE_STRUCT_FILE(tableName));   //judge the struct if exists
	}

	void createTable(Table &table) {
		if (hasTable(table.tableName)) {
			throw SQLException("table " + table.tableName + " exists");
		}
		bufferManager.CreateFile(TABLE_STRUCT_FILE(table.tableName));
		FileNode* file = bufferManager.GetFile(TABLE_STRUCT_FILE(table.tableName));
		BlockNode* block = new BlockNode;
		block->Data = new char[BLOCKSIZE];

		int p = 0;
		char buff[STR_LEN];
		strcpy(buff, table.tableName.c_str());
		memcpy(block->Data, buff, STR_LEN);
		p += STR_LEN;
		
		strcpy(buff, table.primaryKey.c_str());
		memcpy(block->Data + p, buff, STR_LEN);
		p += STR_LEN;

		int size = table.properties.size();
		memcpy(block->Data + p, &size, sizeof(int));
		p += sizeof(int);

		for (int i = 0; i < size; i++) {
			memcpy(block->Data + p, &table.properties[i], sizeof(Property));
			p += sizeof(Property);
		}

		file->allocNewNode(block);
	}

	shared_ptr<Table> findTable(std::string & tableName) {
		if (tableCache.count(tableName) != 0) {
			return tableCache[tableName];
		}
		if (!hasTable(tableName)) {
			throw SQLException("Table not exists!");
		}
		FileNode* file = bufferManager.GetFile(TABLE_STRUCT_FILE(tableName));
		BlockNode* block = file->getblock(0);
		int p = 0;
		char* nameBuff = new char[STR_LEN];
		memcpy(nameBuff, block->Data, STR_LEN);
		string* name = new string(nameBuff);
		p += STR_LEN;
		char* primaryBuff = new char[STR_LEN];
		memcpy(primaryBuff, block->Data + p, STR_LEN);
		string* primary = new string(primaryBuff);
		p += STR_LEN;
		int size;
		memcpy(&size, block->Data + p, sizeof(int));
		p += sizeof(int);
		vector<Property>* properties = new vector<Property>();
		for (int i = 0; i < size; i++) {
			Property* property = new Property;
			memcpy(property, block->Data + p, sizeof(Property));
			properties->push_back(*property);
			p += sizeof(Property);
		}
		auto re = make_shared<Table>(*name, *primary, *properties);
		tableCache.insert(std::make_pair(tableName, re));
		return re;
	}
	
	void dropTable(Table &table) {
		if (hasTable(table.tableName)) {
			bufferManager.DeleteFile(TABLE_STRUCT_FILE(table.tableName));
			tableCache.erase(table.tableName);
		}
		else {
			throw SQLException("Table doesn't exist!");
		}
	}

	bool hasIndex(std::string indexName) {
		ensureIndexFile();
		
		return indexCache.count(indexName) != 0;
	}

	void createIndex(Index& index) {
		ensureIndexFile();
		if (indexCache.count(index.indexName) != 0) {
			throw SQLException("index " + index.indexName + " has already existed.");
		}
		// alloc new block
		auto indexFile = bufferManager.GetFile(INDEX_FILE);
		BlockNode* block = new BlockNode;
		block->Data = new char[BLOCKSIZE];
		int position = indexFile->allocNewNode(block);
		block->dirty = true;
		// update cache
		Index* newIndex = new Index(index);
		auto a = make_pair(index.indexName, newIndex);
		indexCache.insert(a);
		indexPositionCache.insert(make_pair(index.indexName, position));
		// write delete status flag
		bool deleted = false;
		memcpy(block->Data, &deleted, sizeof(bool));
		int p = sizeof(bool);
		char buff[STR_LEN];
		// write indexName
		strcpy(buff, index.indexName.c_str());
		memcpy(block->Data + p, buff, STR_LEN);
		p += STR_LEN;
		// write tableName
		strcpy(buff, index.tableName.c_str());
		memcpy(block->Data + p, buff, STR_LEN);
		p += STR_LEN;
		// write propertyName
		strcpy(buff, index.propertyName.c_str());
		memcpy(block->Data + p, buff, STR_LEN);
	}

	void dropIndex(Index &index) {
		if (!hasIndex(index.indexName)) {
			throw SQLException("Index " + index.indexName + " not found!");
		}
		int position = indexPositionCache[index.indexName];
		BlockNode* block = bufferManager.GetFile(INDEX_FILE)->getblock(position);
		// mark delete status
		bool deleted = true;
		memcpy(block->Data, &deleted, sizeof(bool));
		block->dirty = true;
		indexCache.erase(index.indexName);
		indexPositionCache.erase(index.indexName);
	}

	Index* findIndexByName(std::string & indexName) {
		if (!hasIndex(indexName)) {
			throw SQLException("Index " + indexName + " not found!");
		}
		return indexCache[indexName];
	}

	Index* findIndexByTable(std::string & tableName, std::string & propertyName) {
		ensureIndexFile();
		auto it = indexCache.begin();
		while (it != indexCache.end()) {
			Index* index = it->second;
			if (index->tableName == tableName && index->propertyName == propertyName) {
				return index;
			}
			it++;
		}
		return nullptr;
	}

	void updateIndexCache() {
		indexCache.clear();
		indexPositionCache.clear();
		auto file = bufferManager.GetFile(INDEX_FILE);
		for (int i = 0; i < file->getBlockNum(); i++) {
			auto data = file->getblock(i)->Data;
			// get delete status flag
			bool deleted;
			memcpy(&deleted, data, sizeof(bool));
			if (deleted) {
				continue;
			}
			int p = sizeof(bool);
			char* nameBuff = new char[STR_LEN];
			// read indexName
			memcpy(nameBuff, data + p, STR_LEN);
			string* indexName = new string(nameBuff);
			p += STR_LEN;
			// read tableName
			char* tableBuff = new char[STR_LEN];
			memcpy(tableBuff, data + p, STR_LEN);
			string* tableName = new string(tableBuff);
			p += STR_LEN;
			// read propertyName
			char* propertyBuff = new char[STR_LEN];
			memcpy(propertyBuff, data + p, STR_LEN);
			string* propertyName = new string(propertyBuff);
			// create Index and write into cache
			Index* index = new Index(*indexName, *tableName, *propertyName);
			auto value = pair<int, Index&>(i, *index);
			auto ele = pair<string, pair<int, Index&>>(index->indexName, value);
			indexCache.insert(make_pair(index->indexName, index));
			indexPositionCache.insert(make_pair(index->indexName, i));
		}
	}

	void ensureIndexFile() {
		if (!bufferManager.JudgeFileExistence(INDEX_FILE)) {
			bufferManager.CreateFile(INDEX_FILE);
		}
		if (indexCache.size() == 0) {
			updateIndexCache();
		}
	}
}
