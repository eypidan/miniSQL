#include "CatalogManager.h"
#include <iostream>

using namespace std;

int main() {
	//std::string &tableName, std::string &primaryKey, std::vector<Property> &properties
	vector<Property> properties;
	std::string a = "a";
	std::string b = "b";
	std::string c = "c";
	std::string d = "d";
	Type t1(BaseType::INT, 0);
	Type t2(BaseType::FLOAT, 0);
	Type t3(BaseType::CHAR, 7);
	Type t4(BaseType::CHAR, 254);
	Property p1(t1, true, a);
	Property p2(t2, true, b);
	Property p3(t3, true, c);
	Property p4(t4, false, d);
	properties.push_back(p1);
	properties.push_back(p2);
	properties.push_back(p3);
	properties.push_back(p4);
	string tableName("tablename");
	Table table(tableName, a, properties);
	if (CM::hasTable(tableName)) {
		CM::dropTable(table);
	}
	CM::createTable(table);

	auto t = CM::findTable(tableName);
	cout << t->primaryKey << " " << t->tableName << " " << t->properties.size() << " " << t->properties[3].name << endl;

	string indexName1("index1");
	Index index1(indexName1, tableName, a);
	if (CM::hasIndex(indexName1)) {
		CM::dropIndex(index1);
	}
	CM::createIndex(index1);
	cout << CM::findIndexByName(indexName1)->tableName << endl;
	cout << CM::findIndexByTable(tableName, a)->propertyName << endl;

	string indexName2("index2");
	Index index2(indexName2, tableName, b);
	if (CM::hasIndex(indexName2)) {
		CM::dropIndex(index2);
	}
	CM::createIndex(index2);
	cout << CM::findIndexByName(indexName2)->tableName << endl;
	cout << CM::findIndexByTable(tableName, b)->propertyName << endl;

	string indexName3("index757575");
	Index index3(indexName3, tableName, c);
	if (CM::hasIndex(indexName3)) {
		CM::dropIndex(index3);
	}
	CM::createIndex(index3);
	cout << CM::findIndexByName(indexName3)->indexName << endl;
	cout << CM::findIndexByTable(tableName, c)->propertyName << endl;

}