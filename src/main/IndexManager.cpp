#include "IndexManager.h"

struct CharWrapper 
{
	char content[255];
	CharWrapper(Value* value) {
		memcpy(content, value->getAsType<char*>(), value->getType().getSize() * sizeof(char));
	}
	CharWrapper& operator =(CharWrapper& obj) {
		memcpy(content, obj.content, sizeof(content));
		return *this;
	}
};

IndexManager::IndexManager(Index& index) : _index(index)
{
	Table table = CM::findTable(index.tableName);
	int i;
	for (i = 0; i < table.properties.size; i++) {
		if (table.properties[i].name == index.propertyName) {
			property = &table.properties[i];
		}
	}
	if (i == table.properties.size) {
		throw SQLException("Index " + index.indexName + ".property: " + index.propertyName + "not found in table: " + index.tableName + "!");
	}
	// TODO: Need to test !!!!
	BufferManager bufferManager;
	treeFile = bufferManager.GetFile(index.indexName + ".index");
	if (treeFile == NULL) {
		throw SQLException("Index file " + index.indexName + ".index not found!");
	}
	BlockNode* header = (*treeFile)[0];
	int* rootIndex = (int*)header->Data;
	switch (property->type.getBaseType()) {
	case BaseType::INT:
		createTree<int, BLOCKSIZE>(*rootIndex);
		break;
	case BaseType::FLOAT:
		createTree<float, BLOCKSIZE>(*rootIndex);
		break;
	case BaseType::CHAR:
		createTree<CharWrapper, BLOCKSIZE>(*rootIndex);
		break;
	}
}

IndexManager::~IndexManager() 
{

}

template<typename T, int size>
void IndexManager::createTree(int rootIndex)
{
	tree = new BPlusTree<T, size>(
		(INode<T, size>*)((*treeFile)[rootIndex]->Data), 
		[](int id) {
			BlockNode* node = (*treeFile)[id];
			return (INode<T, size>*) node->Data;
		}, [](auto node) {
			BlockNode* block = (*treeFile)[id];
			block->Data = (char*) node;
			block->dirty = true;
		}, [](auto node) {
			// TODO: mark delete
		}, []() {
			BlockNode* newBlock = new BlockNode;
			INode<T, size>* treeNode = new INode<T, size>;
			newBlock->Data = (char*)treeNode;
			treeNode->id = treeFile->allocNewNode(newBlock) / BLOCKSIZE;
			newBlock->dirty = true;
			return treeNode;
		}
		);
}

void IndexManager::insertEntry(Value* newValue, int indexInRecord)
{
	BaseType type = property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT:
		auto treeImpl = (BPlusTree<float, BLOCKSIZE>*)tree;
		treeImpl->insert(*(newValue->getAsType<float>()), indexInRecord);
		break;
	case BaseType::INT:
		auto treeImpl2 = (BPlusTree<int, BLOCKSIZE>*)tree;
		treeImpl2->insert(*(newValue->getAsType<int>()), indexInRecord);
		break;
	case BaseType::CHAR:
		auto treeImpl3 = (BPlusTree<CharWrapper, BLOCKSIZE>*)tree;
		treeImpl3->insert(CharWrapper(newValue), indexInRecord);
		break;
	}
}

bool IndexManager::deleteEntry(Value* value)
{
	BaseType type = property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT:
		auto treeImpl = (BPlusTree<float, BLOCKSIZE>*)tree;
		return treeImpl->del(*(value->getAsType<float>()));
	case BaseType::INT:
		auto treeImpl2 = (BPlusTree<int, BLOCKSIZE>*)tree;
		return treeImpl2->del(*(value->getAsType<int>()));
	case BaseType::CHAR:
		auto treeImpl3 = (BPlusTree<CharWrapper, BLOCKSIZE>*)tree;
		return treeImpl3->del(CharWrapper(value));
	}
}

std::shared_ptr<IndexIterator> IndexManager::find(Value* value)
{
	BaseType type = property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT:
		auto treeImpl = (BPlusTree<float, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl->find(*(value->getAsType<float>())));
	case BaseType::INT:
		auto treeImpl2 = (BPlusTree<int, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl2->find(*(value->getAsType<int>())));
	case BaseType::CHAR:
		auto treeImpl3 = (BPlusTree<CharWrapper, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl3->find(CharWrapper(value)));
	}
}

std::shared_ptr<IndexIterator> IndexManager::findMinOrMax(bool min)
{
	BaseType type = property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT:
		auto treeImpl = (BPlusTree<float, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl->findExtreme(!min));
	case BaseType::INT:
		auto treeImpl2 = (BPlusTree<int, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl2->findExtreme(!min));
	case BaseType::CHAR:
		auto treeImpl3 = (BPlusTree<CharWrapper, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl3->findExtreme(!min));
	}
}

static void IndexManager::createNewIndex(Index& index)
{
	// TODO: Need to test !!!!
	std::string fileName = index.indexName + ".index";
	BufferManager bufferManager;
	if (bufferManager.JudgeStructExistence(fileName)) {
		throw SQLException("index: " + fileName + " has already existed!");
	}
	BlockNode* header = new BlockNode;
	header->FileName = fileName;
	bufferManager.CreateStruct(header);
	BlockNode* root = new BlockNode;
	FileNode* fileNode = bufferManager.GetFile(fileName);
	int id = fileNode->allocNewNode(root) / BLOCKSIZE;
	memcpy(header->Data, &id, sizeof(id));
	header->dirty = root->dirty = true;
}

static void IndexManager::dropIndex(Index& index)
{
	std::string fileName = index.indexName + ".index";
	BufferManager bufferManager;
	bufferManager.DeleteFile(fileName);
}

template<typename T>
std::shared_ptr<IndexIterator> IndexManager::generateIterator(LeafPosition<T, BLOCKSIZE>* position)
{
	auto re = std::make_shared<IndexIterator>();
	re->leafPosition = (void*) position;
	re->currentValue = Value(property->type, (void*)treeNode->value[position->position]);
	re->indexInRecord = treeNode->pointers[position->position];
	re->indexManager = this;
	return re;
}

IndexIterator::~IndexIterator()
{
	BaseType type = indexManager->property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT:
		//delete (LeafPosition<float, BLOCKSIZE>*)leafPosition;
		break;
	case BaseType::INT:
		//delete (LeafPosition<int, BLOCKSIZE>*)leafPosition;
		break;
	case BaseType::CHAR:
		//delete (LeafPosition<CharWrapper, BLOCKSIZE>*)leafPosition;
		break;
	}
	delete currentValue;
}

std::shared_ptr<IndexIterator> IndexIterator::next()
{
	BaseType type = indexManager->property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT:
		return generateNext<float>();
	case BaseType::INT:
		return generateNext<int>();
	case BaseType::CHAR:
		return generateNext<CharWrapper>();
	}
}

template <typename T>
std::shared_ptr<IndexIterator> IndexIterator::generateNext()
{
	auto tree = (BPlusTree<T, BLOCKSIZE>*)indexManager->tree;
	auto pos = (LeafPosition<T, BLOCKSIZE>*) leafPosition;
	auto treeNode = pos->node;
	int treePosition = pos->position;
	if (treePosition < treeNode->contentSize - 1) {
		return indexManager->generateIterator(new LeafPosition<T, BLOCKSIZE >> (
			treeNode, treePosition + 1);
	}
	if (treeNode->pointers[tree->getM()] > 0) {
		treeNode = tree->next(treeNode);
		return indexManager->generateIterator(new LeafPosition<T, BLOCKSIZE >> (
			treeNode, 0);
	}
	return (std::shared_ptr<IndexIterator>)NULL;
}