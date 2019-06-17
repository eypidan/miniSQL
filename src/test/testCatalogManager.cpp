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
	cout << t->primaryKey << " " << t->tableName << " " << t->properties.size() << " " << t->properties[3].name;
}