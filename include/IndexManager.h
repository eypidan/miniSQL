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
	BlockNode* findBlock(Value* value);
	std::list<BlockNode*> findRangeBlock(Value* minValue, Value* maxValue);

	static std::shared_ptr<Index> findIndex(std::string& tableName, std::string& propertyName);
	static std::shared_ptr<Index> findIndexByName(std::string& indexName);
	static void createNewIndex(Index& index);
	static void dropIndex(Index& index);
private:
	Index& _index;
	FileNode treeFile;
}

#endif // _INDEX_MANAGER_H_