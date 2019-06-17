#include "IndexManager.h"
#include "string"

using namespace std;

int main()
{
	string indexName = "indexname";
	string tableName = "tableName";
	string keyName = "a";
	Index index(indexName, tableName, keyName);
	BufferManager bufferManager;
	if (bufferManager.JudgeFileExistence(indexName + ".index")) {
		IndexManager::dropIndex(index);
	}
	IndexManager::createNewIndex(index);

	IndexManager manager(index);
	Type type(BaseType::INT);
	for (int i = 0; i < 520; i++) {
		int j = i;
		Value value(type, &j);
		manager.insertEntry(&value, j + 1000);
	}
	manager.printTree();
	// test disk
	IndexManager manager2(index);
	manager2.printTree();
}