#include "IndexManager.h"

IndexManager::IndexManager(Index& index) {

}

IndexManager::~IndexManager() {

}

void IndexManager::insertEntry(Value* newValue) {

}

void IndexManager::deleteEntry(Value* value) {

}

BlockNode* IndexManager::findBlock(Value* value) {

}

std::list<BlockNode*> IndexManager::findRangeBlock(Value* minValue, Value* maxValue) {

}

std::shared_ptr<Index> IndexManager::findIndex(
	std::string& tableName, std::string& propertyName) {

}

std::shared_ptr<Index> IndexManager::findIndexByName(
	std::string& indexName) {

}

void IndexManager::createNewIndex(Index& index) {

}

void IndexManager::dropIndex(Index& index) {

}