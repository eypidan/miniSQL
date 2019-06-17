#include "CatalogManager.h"
#include "MetaData.h"
#include "Exception.h"
#include "IndexManager.h""
#include "BufferManager.h"
namespace CM {
	BufferManager *bm = new BufferManager();
	IndexIterator *iter = new IndexIterator();
	Table table;
	BlockNode *block;
	FileNode *file;
	Value *value;
	char data[BLOCKSIZE];
	bool hasTable(std::string & tableName) {
		string fileName = (tableName + ".struct").c_str(); //don't know the table structure
		return bm->JudgeFileExistence(fileName);   //judge the struct if exists
	}
	Table& findTable(std::string & tableName) {
		if (!hasTable(tableName)) {
			throw SQLException("Table exists!");
		}
		else
		{
			table.tableName = tableName;
			string fileName = (tableName + ".struct").c_str();
			file = bm->GetFile(fileName);
			

		}
		return table;
	}
	void createTable(std::string name, vector<Property> &properties, int primary, vector<Index> index) {
		//如果已存在该表，则异常
		FileNode *file = new FileNode;
		BlockNode *block = new BlockNode;
		block->Data = new char[BLOCKSIZE];
		if (hasTable(name)) {
			throw SQLException("table exists");
		}
		//确保主键为unique
		Property property = properties[0];
		property.unique = true;
		//记录每条信息的字符数（包括这里的5个）
		std::string str_tmp = "0000 ";
		//添加name
		str_tmp += name;
		//添加没个attribute的信息，顺序为类型，名字，是否为唯一
		for (int i = 0; i < properties.size; i++)
			str_tmp = str_tmp + " " + properties[i].type.getBaseType + " " + properties[i].name + " " + (properties[i].unique == true ? "1" : "0");
		//添加index的数量, ;用来做标记index的开始
		str_tmp = str_tmp + " ;" + name;
		//添加index的信息，顺序为相对位置和名字
		for (int i = 0; i < index.size; i++)
			str_tmp = str_tmp + " " + index[i].indexName + " " + index[i].propertyName;
		//换行后在结尾接上一个#，每个块以#结尾
		str_tmp = str_tmp + "\n" + "#";
		//更改每条信息的长度的记录
		std::string str_len = ((int)str_tmp.length() - 1).to_string();
		str_tmp = str_len + str_tmp.substr(4, str_tmp.length() - 4);
		char *buffer = file->operator[](file->allocNewNode(block))->Data;
		strcat(buffer, str_tmp.c_str());
		//这里没考虑buffer不够的情况。
		strcat(buffer, str_tmp.c_str());
	}
	void dropTable(Table &table) {
		if (hasTable(table.tableName)) {
			bm->DeleteFile(table.tableName + ".struct");
		}
		else {
			throw SQLException("Table doesn't exist!");
		}
	}

	bool hasIndex(std::string & indexName) {
		return bm->JudgeFileExistence((indexName + ".index").c_str());
	}
	Index& findIndex(std::string & indexName) {
		if (hasIndex(indexName)) {

		}
		else {
			throw SQLException("Index doesn't exist!");
		}
	}
	void createIndex(Index &index) {
		if (hasIndex(index.indexName)) {
			throw SQLException("");
		}
		else {
			IndexManager *im = new IndexManager(index);
			im->createNewIndex(index);
		}
	}
	void dropIndex(Index &index) {
		if (hasIndex(index.indexName)) {
			IndexManager *im = new IndexManager(index);
			im->dropIndex(index);
		}
		else {
			throw SQLException("");
		}
	}
}