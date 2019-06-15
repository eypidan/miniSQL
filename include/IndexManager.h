#ifndef _INDEX_MANAGER_H_
#define _INDEX_MANAGER_H_

#include "MetaData.h"
#include "BPlusTree.h"
#include "BufferManager.h"
#include <list>
#include <string>
#include <memory>

class IndexManager {
public:
	IndexManager(Index& index);
	~IndexManager();
	void insertEntry(Value* newValue);
	void deleteEntry(Value* value);
	std::shared_ptr<IndexIterator> find(Value* value);
	std::shared_ptr<IndexIterator> findMinOrMax(bool min);

	static std::shared_ptr<Index> findIndex(std::string& tableName, std::string& propertyName);
	static std::shared_ptr<Index> findIndexByName(std::string& indexName);
	static void createNewIndex(Index& index);
	static void dropIndex(Index& index);
private:
	static int calculateNodeSize(void* indexPointer);
	Index& _index;
	FileNode treeFile;
	BPlusTree<Value, BLOCKSIZE> tree;
};

class IndexIterator {
public:
	Value* currentValue;
	int indexInRecord;
	IndexIterator* next();
private:
	IndexManager& indexManager;
};

#endif // _INDEX_MANAGER_H_