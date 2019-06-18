#include "RecordManager.h"
#include "IndexManager.h"
#include "CatalogManager.h"
#include <string>
#include <vector>

using namespace std;

string tablename("table");
std::string a = "a";
std::string b = "b";
std::string c = "c";
std::string d = "d";
Type t1(BaseType::INT, 0);
Type t2(BaseType::FLOAT, 0);
Type t3(BaseType::CHAR, 5);
Type t4(BaseType::CHAR, 7);
Property p1(t1, true, a);
Property p2(t2, false, b);
Property p3(t3, false, c);
Property p4(t4, false, d);
vector<string> propertyNames;

vector<Value> genRecords(int* a, float* b, char* c, char* d) {
	vector<Value>* re = new vector<Value>;
	re->push_back(*(new Value(t1, a)));
	re->push_back(*(new Value(t2, b)));
	re->push_back(*(new Value(t3, c)));
	re->push_back(*(new Value(t4, d)));
	string ad(re->at(3).getAsType<char>());
	return *re;
}


char c1[5] = "wwf";
char* d1 = "ffffff";
string indexName = "indexname";
int main() {
	
	vector<Property> properties;
	properties.push_back(p1);
	properties.push_back(p2);
	properties.push_back(p3);
	properties.push_back(p4);
	propertyNames.push_back(a);
	propertyNames.push_back(b);
	//propertyNames.push_back(c);
	propertyNames.push_back(d);
	Table table(tablename, a, properties);
	if (RM::hasTable(table.tableName)) {
		RM::dropTable(table);
	}
	RM::createTable(table);
	Index* index = new Index(indexName, tablename, a);
	if (IndexManager::hasIndex(indexName)) {
		IndexManager::dropIndex(*index);
	}
	IndexManager::createNewIndex(*index);
	if (!CM::hasTable(tablename)) {
		CM::createTable(table);
	}
	if (!CM::hasIndex(indexName)) {
		CM::createIndex(*index);
	}
	cout << "begin";
	for (int i = 0; i < 500; i++) {
		cout << i;
		float b1 = 2.44;
		RM::insertRecord(table, genRecords(&i, &b1, c1, d1));
/*
		IndexManager indexManager(*index);
		indexManager.printTree();*/
	}
	IndexManager indexManager(*index);
	indexManager.printTree();
	//CM::dropIndex(*index);
	//IndexManager::dropIndex(*index);
	vector<Predicate> ve;
	int jjj = 400;
	int iii = 400;
	Predicate predicate(a, OpType::NE, *(new Value(t1, &jjj)));
	Predicate predicate2(c, OpType::EQ, *(new Value(t3, c1)));
	ve.push_back(predicate);
	ve.push_back(predicate2);
	cout << "ss:: " << (*(new Value(t1, &jjj)) == *(new Value(t1, &iii)));
	auto view = RM::selectRecords(propertyNames, table, ve);
	for (int i = 0; i < view.size(); i++) {
		auto re = view[i];
		for (int j = 0; j < re->size(); j++) {
			auto c = re->at(j);
			switch (c.getType().getBaseType()) {
			case BaseType::INT:
				cout << *c.getAsType<int>();
				break;
			case BaseType::FLOAT:
				cout << *c.getAsType<float>();
				break;
			case BaseType::CHAR:
				string a(c.getAsType<char>());
				cout << a;
				break;
			}
			cout << " ";
		}
		cout << endl;
	}
	cout << "end";
}