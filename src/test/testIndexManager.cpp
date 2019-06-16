#include "IndexManager.h"
#include "string"

using namespace std;

int main()
{
	Index index(string("indexname"), string("tablename"), string("a"));
	IndexManager::createNewIndex(index);

}