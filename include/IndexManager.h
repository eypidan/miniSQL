#ifndef _INDEX_MANAGER_H_
#define _INDEX_MANAGER_H_

#include "MetaData.h"
#include "BPlusTree.h"
#include "BufferManager.h"
#include "CatalogManager.h"
#include <list>
#include <string>
#include <memory>

class IndexIterator;

class IndexManager {
public:
	IndexManager(Index& index);
	~IndexManager();
	void insertEntry(Value* newValue, int indexInRecord);
	bool deleteEntry(Value* value);
	std::shared_ptr<IndexIterator> find(Value* value);
	std::shared_ptr<IndexIterator> findMinOrMax(bool min);

	// FIX: The following two methods should be implemented by CatalogManager
	//static std::shared_ptr<Index> findIndex(std::string& tableName, std::string& propertyName);
	//static std::shared_ptr<Index> findIndexByName(std::string& indexName);
	static void createNewIndex(Index& index);
	static void dropIndex(Index& index);
private:
	Index& _index;
	Property* property;
	FileNode* treeFile;
	void* tree;

	template<typename T, int size>
	void createTree(int rootIndex);
	
	template<typename T>
	std::shared_ptr<IndexIterator> generateIterator(LeafPosition<T, BLOCKSIZE>* position);

	friend class IndexIterator;
};

class IndexIterator {
public:
	Value* currentValue;
	int indexInRecord;
	std::shared_ptr<IndexIterator> next();
private:
	//~IndexIterator();
	void* leafPosition;
	IndexManager* indexManager;

	template <typename T>
	std::shared_ptr<IndexIterator> generateNext();

	friend class IndexManager;
};

#endif // _INDEX_MANAGER_H_