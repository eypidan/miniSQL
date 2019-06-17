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
		//����Ѵ��ڸñ����쳣
		FileNode *file = new FileNode;
		BlockNode *block = new BlockNode;
		block->Data = new char[BLOCKSIZE];
		if (hasTable(name)) {
			throw SQLException("table exists");
		}
		//ȷ������Ϊunique
		Property property = properties[0];
		property.unique = true;
		//��¼ÿ����Ϣ���ַ��������������5����
		std::string str_tmp = "0000 ";
		//���name
		str_tmp += name;
		//���û��attribute����Ϣ��˳��Ϊ���ͣ����֣��Ƿ�ΪΨһ
		for (int i = 0; i < properties.size; i++)
			str_tmp = str_tmp + " " + properties[i].type.getBaseType + " " + properties[i].name + " " + (properties[i].unique == true ? "1" : "0");
		//���index������, ;���������index�Ŀ�ʼ
		str_tmp = str_tmp + " ;" + name;
		//���index����Ϣ��˳��Ϊ���λ�ú�����
		for (int i = 0; i < index.size; i++)
			str_tmp = str_tmp + " " + index[i].indexName + " " + index[i].propertyName;
		//���к��ڽ�β����һ��#��ÿ������#��β
		str_tmp = str_tmp + "\n" + "#";
		//����ÿ����Ϣ�ĳ��ȵļ�¼
		std::string str_len = ((int)str_tmp.length() - 1).to_string();
		str_tmp = str_len + str_tmp.substr(4, str_tmp.length() - 4);
		char *buffer = file->operator[](file->allocNewNode(block))->Data;
		strcat(buffer, str_tmp.c_str());
		//����û����buffer�����������
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