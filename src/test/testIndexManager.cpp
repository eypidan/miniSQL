#include "IndexManager.h"
#include "string"

using namespace std;

void generateString(char* data, int i)
{
	int j = i, k = 0;;
	while (j > 0) {
		data[k++] = j % 26 + 'a';
		j /= 26;
	}
	data[k] = '\0';
}
int main()
{
	string indexName = "indexname";
	string tableName = "tableName";
	string keyName = "c";
	Index index(indexName, tableName, keyName);
	BufferManager bufferManager;
	if (bufferManager.JudgeFileExistence(indexName + ".index")) {
		IndexManager::dropIndex(index);
	}
	IndexManager::createNewIndex(index);
	IndexManager manager(index);
	Type type(BaseType::CHAR, 7);
	// test add
	for (int i = 0; i < 10000; i++) {
		char data[7];
		generateString(data, i);
		Value value(type, data);
		manager.insertEntry(&value, i + 1000);
	}
	manager.printTree();
	// test delete
	cout << "begin delete: ";
	for (int i = 0; i < 520; i += 2) {
		char data[7];
		generateString(data, i);
		Value value(type, data);
		manager.deleteEntry(&value);
	}
	// test disk
	cout << "begin fetch from disk: ";
	IndexManager manager2(index);
	manager2.printTree();
	// test find
	cout << "begin find" << endl;
	char data[7];
	generateString(data, 100);
	Value value(type, data);
	auto iter = manager2.find(&value);
	while (iter) {
		string re(iter->currentValue->getAsType<char>());
		cout << re << ": " << iter->indexInRecord << endl;
		iter = iter->next();
	}
	// test iterator
	cout << "begin iteration" << endl;
	int count = 0;
	iter = manager2.findMinOrMax(true);
	while (iter) {
		string re(iter->currentValue->getAsType<char>());
		cout << re << ": " << iter->indexInRecord << endl;
		iter = iter->next();
		count++;
	}
	cout << count;

}