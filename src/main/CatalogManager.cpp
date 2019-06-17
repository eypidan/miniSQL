#include "CatalogManager.h"
#include "MetaData.h"
#include "Exception.h"
#include "IndexManager.h"
#include "BufferManager.h"

#define TABLE_STRUCT_FILE(x) (x + "_table.struct")
#define INDEX_STRUCT_FILE(x) (x + "_index.struct")
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
		return make_shared<Table>(*name, *primary, *properties);
	}
	
	void dropTable(Table &table) {
		if (hasTable(table.tableName)) {
			bufferManager.DeleteFile(TABLE_STRUCT_FILE(table.tableName));
		}
		else {
			throw SQLException("Table doesn't exist!");
		}
	}

	bool hasIndex(std::string & indexName) {
		return bufferManager.JudgeFileExistence(INDEX_STRUCT_FILE(indexName));
	}

	std::shared_ptr<Table> findIndex(std::string & indexName) {
		/*if (hasIndex(indexName)) {

		}
		else {*/
			throw SQLException("Index doesn't exist!");
		//}
	}

	void createIndex(Index &index) {
	/*	if (hasIndex(index.indexName)) {
			throw SQLException("");
		}
		else {
			IndexManager *im = new IndexManager(index);
			im->createNewIndex(index);
		}*/
	}

	void dropIndex(Index &index) {
		/*if (hasIndex(index.indexName)) {
			IndexManager *im = new IndexManager(index);
			im->dropIndex(index);
		}
		else {
			throw SQLException("");
		}*/
	}
}
