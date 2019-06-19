#include "IndexManager.h"
#include <string>

int strcmp(const char *str1, const char *str2)
{
	while (*str1 == *str2) {
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
		memcpy(content, value->getAsType<char>(), value->getType().getSize() * sizeof(char));
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
	bool operator !=(const CharWrapper& another) { return strcmp(content, another.content) != 0; }
	friend ostream & operator<<(ostream & os, const CharWrapper & c) {
		std::string s(c.content);
		os << s;
		return os;
	}
};

IndexManager::IndexManager(Index& index) : _index(index)
{
	auto table = CM::findTable(index.tableName);
	vector<Property> properties;
	/*// for test
	std::string a = "a";
	std::string b = "b";
	std::string c = "c";
	std::string d = "d";
	Type t1(BaseType::INT, 0);
	Type t2(BaseType::FLOAT, 0);
	Type t3(BaseType::CHAR, 7);
	Type t4(BaseType::CHAR, 254);
	Property p1(t1, a);
	Property p2(t2, b);
	Property p3(t3, c);
	Property p4(t4, d);
	properties.push_back(p1);
	properties.push_back(p2);
	properties.push_back(p3);
	properties.push_back(p4);
	Table table(index.tableName, a, properties);*/
	// Find property
	int i;
	for (i = 0; i < table->properties.size(); i++) {
		if (table->properties[i].name == index.propertyName) {
			property = new Property(table->properties[i]);
			break;
		}
	}
	if (i == table->properties.size()) {
		throw SQLException("Index " + index.indexName + ".property: " + index.propertyName + "not found in table: " + index.tableName + "!");
	}
	treeFile = bufferManager.GetFile(index.indexName + ".index");
	if (treeFile == NULL) {
		throw SQLException("Index file " + index.indexName + ".index not found!");
	}
	BlockNode* header = treeFile->getblock(0);
	int* rootIndex = (int*)header->Data;
	// template tree instantiation with different value type.
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
	delete property;
}

template<typename T, int size>
void IndexManager::createTree(int rootIndex)
{
	auto block = treeFile->getblock(rootIndex);
	auto root = (INode<T, size>*)block->Data;
	if (root->id != rootIndex) {
		root->id = rootIndex;
		block->dirty = true;
	}
	tree = new BPlusTree<T, size>(
		root,
		[=](int id) { // get child
		BlockNode* node = treeFile->getblock(id);
		return (INode<T, size>*) node->Data;
	}, [=](auto node) { // modify
		BlockNode* block = treeFile->getblock(node->id);
		block->dirty = true;
	}, [=](auto node) { // delete
		// nothing to do
	}, [=]() { // create
		BlockNode* newBlock = new BlockNode;
		newBlock->Data = new char[BLOCKSIZE];
		auto treeNode = (INode<T, size>*) newBlock->Data;
		treeNode->id = treeFile->allocNewNode(newBlock);
		newBlock->dirty = true;
		return treeNode;
	}, [=](auto newRootId) { // root change
		BlockNode* block = treeFile->getblock(0);
		memcpy(block->Data, &newRootId, sizeof(int));
		block->dirty = true;
	});
}

void IndexManager::insertEntry(Value* newValue, int indexInRecord)
{
	BaseType type = property->type.getBaseType();
	if (newValue->getType().getBaseType() != type) {
		throw SQLException("Type of the entry inserted doesn't match B+ tree type!");
	}
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
}

bool IndexManager::deleteEntry(Value* value)
{
	BaseType type = property->type.getBaseType();
	if (value->getType().getBaseType() != type) {
		throw SQLException("Type of the entry deleted doesn't match B+ tree type!");
	}
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
	return re;
}

std::shared_ptr<IndexIterator> IndexManager::find(Value* value)
{
	BaseType type = property->type.getBaseType();
	if (value->getType().getBaseType() != type) {
		throw SQLException("Type of the entry to find doesn't match B+ tree type!");
	}
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
	throw SQLException("Unknown type");
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
	throw SQLException("Unknown type");
}

void IndexManager::createNewIndex(Index& index)
{
	std::string fileName = index.indexName + ".index";
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
	memset(header->Data, 0, BLOCKSIZE);
	fileNode->allocNewNode(header);

	BlockNode* root = new BlockNode;
	root->Data = new char[BLOCKSIZE];
	memset(root->Data, 0, BLOCKSIZE);

	int id = fileNode->allocNewNode(root);
	memcpy(header->Data, &id, sizeof(id));

	header->dirty = root->dirty = true;
}

void IndexManager::dropIndex(Index& index)
{
	std::string fileName = index.indexName + ".index";
	bufferManager.DeleteFile(fileName);
}

bool IndexManager::hasIndex(std::string indexName)
{
	std::string fileName = indexName + ".index";
	return bufferManager.JudgeFileExistence(fileName);
}


void IndexManager::printTree() const
{
	BaseType type = property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT: {
		auto treeImpl = (BPlusTree<float, BLOCKSIZE>*)tree;
		treeImpl->printTree();
		break;
	}
	case BaseType::INT: {
		auto treeImpl = (BPlusTree<int, BLOCKSIZE>*)tree;
		treeImpl->printTree();
		break;
	}
	case BaseType::CHAR: {
		auto treeImpl = (BPlusTree<CharWrapper, BLOCKSIZE>*)tree;
		treeImpl->printTree();
		break;
	}
	}
}

template<typename T>
std::shared_ptr<IndexIterator> IndexManager::generateIterator(LeafPosition<T, BLOCKSIZE>* position)
{
	auto re = std::make_shared<IndexIterator>();
	re->leafPosition = (void*)position;
	re->currentValue = new Value(property->type, (void*)&position->node->value[position->position]);
	re->indexInRecord = position->node->pointers[position->position];
	re->indexManager = this;
	return re;
}

IndexIterator::~IndexIterator()
{

	BaseType type = indexManager->property->type.getBaseType();
	switch (type) {
	case BaseType::FLOAT:
		delete (LeafPosition<float, BLOCKSIZE>*)leafPosition;
		break;
	case BaseType::INT:
		delete (LeafPosition<int, BLOCKSIZE>*)leafPosition;
		break;
	case BaseType::CHAR:
		delete (LeafPosition<CharWrapper, BLOCKSIZE>*)leafPosition;
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
	throw SQLException("Unknown type");
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