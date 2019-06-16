#include "IndexManager.h"
#include <string>

int strcmp(const char *str1, const char *str2)
{
	while (*str1 == *str2){
		if (*str1 == '\0') return 0;
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

struct CharWrapper 
{
	char content[255];
	CharWrapper() = default;
	CharWrapper(Value* value) {
		memcpy(content, value->getAsType<char*>(), value->getType().getSize() * sizeof(char));
	}
	CharWrapper& operator =(CharWrapper& obj) {
		memcpy(content, obj.content, sizeof(content));
		return *this;
	}
	bool operator <(const CharWrapper& another) { return strcmp(content, another.content) < 0; }
	bool operator >(const CharWrapper& another) { return strcmp(content, another.content) > 0; }
	bool operator <=(const CharWrapper& another) { return strcmp(content, another.content) <= 0; }
	bool operator >=(const CharWrapper& another) { return strcmp(content, another.content) >= 0; }
	bool operator ==(const CharWrapper& another) { return strcmp(content, another.content) == 0; }
};

IndexManager::IndexManager(Index& index) : _index(index)
{
	//Table table = CM::findTable(index.tableName);
	// for test
	vector<Property> properties;
	properties.push_back(Property(Type(BaseType::INT, 0), std::string("a")));
	properties.push_back(Property(Type(BaseType::FLOAT, 0), std::string("b")));
	properties.push_back(Property(Type(BaseType::CHAR, 7), std::string("c")));
	properties.push_back(Property(Type(BaseType::CHAR, 254), std::string("d")));

	Table table(std::string("tablename"), std::string("a"), properties);

	int i;
	for (i = 0; i < table.properties.size(); i++) {
		if (table.properties[i].name == index.propertyName) {
			property = &table.properties[i];
		}
	}
	if (i == table.properties.size()) {
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
		[=](int id) {
			BlockNode* node = (*treeFile)[id];
			return (INode<T, size>*) node->Data;
		}, [=](auto node) {
			BlockNode* block = (*treeFile)[node->id];
			block->Data = (char*) node;
			block->dirty = true;
		}, [=](auto node) {
			// TODO: mark delete
		}, [=]() {
			BlockNode* newBlock = new BlockNode;
			newBlock->Data = new char[BLOCKSIZE];
			INode<T, size>* treeNode = new INode<T, size>;
			memcpy(newBlock->Data, treeNode, sizeof(treeNode));
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
	case BaseType::FLOAT: {
		auto treeImpl = (BPlusTree<float, BLOCKSIZE>*)tree;
		treeImpl->insert(*(newValue->getAsType<float>()), indexInRecord);
		break;
	}
	case BaseType::INT: {
		auto treeImpl2 = (BPlusTree<int, BLOCKSIZE>*)tree;
		treeImpl2->insert(*(newValue->getAsType<int>()), indexInRecord);
		break;
	}
	case BaseType::CHAR: {
		auto treeImpl3 = (BPlusTree<CharWrapper, BLOCKSIZE>*)tree;
		treeImpl3->insert(CharWrapper(newValue), indexInRecord);
		break;
	}
	}
	treeFile->synchronize();
}

bool IndexManager::deleteEntry(Value* value)
{
	BaseType type = property->type.getBaseType();
	bool re;
	switch (type) {
	case BaseType::FLOAT: {
		auto treeImpl = (BPlusTree<float, BLOCKSIZE>*)tree;
		re = treeImpl->del(*(value->getAsType<float>()));
		break;
	}
	case BaseType::INT: {
		auto treeImpl2 = (BPlusTree<int, BLOCKSIZE>*)tree;
		re = treeImpl2->del(*(value->getAsType<int>()));
		break;
	}
	case BaseType::CHAR: {
		auto treeImpl3 = (BPlusTree<CharWrapper, BLOCKSIZE>*)tree;
		re = treeImpl3->del(CharWrapper(value));
		break;
	}
	}
	treeFile->synchronize();
	return re;
}

std::shared_ptr<IndexIterator> IndexManager::find(Value* value)
{
	BaseType type = property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT: {
		auto treeImpl = (BPlusTree<float, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl->find(*(value->getAsType<float>())));
	}
	case BaseType::INT: {
		auto treeImpl2 = (BPlusTree<int, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl2->find(*(value->getAsType<int>())));
	}
	case BaseType::CHAR: {
		auto treeImpl3 = (BPlusTree<CharWrapper, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl3->find(CharWrapper(value)));
	}
	}
}

std::shared_ptr<IndexIterator> IndexManager::findMinOrMax(bool min)
{
	BaseType type = property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT: {
		auto treeImpl = (BPlusTree<float, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl->findExtreme(!min));
	}
	case BaseType::INT: {
		auto treeImpl2 = (BPlusTree<int, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl2->findExtreme(!min));
	}
	case BaseType::CHAR: {
		auto treeImpl3 = (BPlusTree<CharWrapper, BLOCKSIZE>*)tree;
		return generateIterator(treeImpl3->findExtreme(!min));
	}
	}
}

void IndexManager::createNewIndex(Index& index)
{
	// TODO: Need to test !!!!
	std::string fileName = index.indexName + ".index";
	BufferManager bufferManager;
	if (bufferManager.JudgeFileExistence(fileName)) {
		throw SQLException("index: " + fileName + " has already existed!");
	}
	bufferManager.CreateFile(fileName);
	FileNode* fileNode = bufferManager.GetFile(fileName);
	if (!fileNode) {
		throw SQLException("Fail to create file: " + fileName);
	}
	BlockNode* header = new BlockNode;
	header->Data = new char[BLOCKSIZE];
	fileNode->allocNewNode(header);

	BlockNode* root = new BlockNode;
	root->Data = new char[BLOCKSIZE];
	int id = fileNode->allocNewNode(root);
	memcpy(header->Data, &id, sizeof(id));

	header->dirty = root->dirty = true;
	fileNode->synchronize();
}

void IndexManager::dropIndex(Index& index)
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
	re->currentValue = new Value(property->type, (void*)&position->node->value[position->position]);
	re->indexInRecord = position->node->pointers[position->position];
	re->indexManager = this;
	return re;
}
/*
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
	*/
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
		return indexManager->generateIterator(new LeafPosition<T, BLOCKSIZE>(
			treeNode, treePosition + 1));
	}
	if (treeNode->pointers[tree->getM()] > 0) {
		treeNode = tree->next(treeNode);
		return indexManager->generateIterator(new LeafPosition<T, BLOCKSIZE>(
			treeNode, 0));
	}
	return (std::shared_ptr<IndexIterator>)NULL;
}