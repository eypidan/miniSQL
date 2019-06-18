#include <iostream>
#include "include\Api.h"
#include "BufferManager.h"
#include "RecordManager.h"
#include "CatalogManager.h"
#include "MetaData.h"
#include "Exception.h"

#define INDEX_FILE "index.struct"
#define TABLE_STRUCT_FILE(x) (x + "_table.struct")
#define RECORD_STRUCT_FILE(x) (x+"_record.struct")
#define STR_LEN 64

namespace RM {
	using namespace std;
	bool hasTable(std::string & tableName) {
		return bufferManager.JudgeFileExistence(TABLE_STRUCT_FILE(tableName));   //judge the struct if exists
	}
	void createTable(Table &table) {
		if (!hasTable(table.tableName)) {
			CM::createTable(table);
		}
		else
		{
			throw SQLException("table "+table.tableName+"exists");                         //throw the file exist exception
		}
	}
	void dropTable(Table& table){
		if (hasTable(table.tableName)) {
			bufferManager.DeleteFile(TABLE_STRUCT_FILE(table.tableName));
		}
		else {
			throw SQLException("Table doesn't exist!");          
		}
	}

	void insertRecord(Table& table, std::vector<Value> record) {
		if (!hasTable(table.tableName)){
			throw SQLException("table " + table.tableName + "doesn't exist");
		}
		bufferManager.CreateFile(RECORD_STRUCT_FILE(table.tableName));
		FileNode* file = bufferManager.GetFile(RECORD_STRUCT_FILE(table.tableName));
		BlockNode* block = new BlockNode;
		block->Data = new char[BLOCKSIZE];

		int p = 0;
		int size_r = record.size();
		memcpy(block->Data + p, &size_r, sizeof(int));
		p += sizeof(int);
		for (int i = 0; i < size_r; i++) {
			memcpy(block->Data + p, &record[i], sizeof(Value));
			p += sizeof(Value);
			if (p + sizeof(Value) > BLOCKSIZE) {
				int offset = file->allocNewNode(block);
				block = file->getblock(offset);
				p = 0;
			}
		}

	}
	int deleteAllRecords(Table& table){
		if (!hasTable(table.tableName)) {
			throw SQLException("table " +table.tableName+" doesn't exist");
		}
		bufferManager.CreateFile(RECORD_STRUCT_FILE(table.tableName));
		FileNode* file = bufferManager.GetFile(RECORD_STRUCT_FILE(table.tableName));
		int position = file->getBlockNum();
		BlockNode* block = file->getblock(position);
		bool deleted = true;
		memcpy(block->Data, &deleted, sizeof(bool));
		block->dirty = true;
	}
	int deleteRecords(
		Table& table,
		std::vector<Predicate> & predicates
	){
		if (!hasTable(table.tableName)) {
			throw SQLException("table "+ table.tableName+"doesn't exist");
		}
		bufferManager.CreateFile(RECORD_STRUCT_FILE(table.tableName));
		FileNode* file = bufferManager.GetFile(RECORD_STRUCT_FILE(table.tableName));
		int blockNum = file->getBlockNum();
		int p = 0;
		BlockNode* block = file->getblock(0);
		int size;
		memcpy(&size, block->Data, sizeof(int));
		p += sizeof(int);
		vector<Value>* records = new vector<Value>();
		for (int i = 0; i < size; i++) {
			Value* record = new Value;
			memcpy(record, block->Data + p, sizeof(Value));
			records->push_back(*record);
			p += sizeof(record);
		}
		for (int j = 0; j < predicates.size(); j++) {
			switch (predicates[j].op) {
			case EQ:// =
				predicates[j].propertyName
			case NE: // <>
			case LT: // <
			case LEQ: // <=
			case GT: // >
			case GEQ: // >=
			}
		}
	}
	View selectRecords(
		std::vector<std::string> &properties,
		Table& table,
		std::vector<Predicate> & predicates
	) {
		View view;
	}
}
