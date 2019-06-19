#include "Api.h"
#include <iostream>

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
vector<Property> properties;


vector<Value>* genRecords(int* a, float* b, char* c, char* d) {
	vector<Value>* re = new vector<Value>;
	re->push_back(*(new Value(t1, a)));
	re->push_back(*(new Value(t2, b)));
	re->push_back(*(new Value(t3, c)));
	re->push_back(*(new Value(t4, d)));
	string ad(re->at(3).getAsType<char>());
	return re;
}

int main() {

	properties.push_back(p1);
	properties.push_back(p2);
	properties.push_back(p3);
	properties.push_back(p4);
	propertyNames.push_back(a);
	propertyNames.push_back(b);
	//propertyNames.push_back(c);
	propertyNames.push_back(d);

	Table table(tablename, a, properties);

	// test create
	auto create = API::createTable(table);
	cout << "create: " << create.isSuccess << " " << create.errorMessage << endl;

	// test insert
	clock_t s = clock();
	for (int i = 0; i < 10000; i++) {
		int a = i;
		float b = i * 100;
		char* c = "wwww";
		char* d = "1111";
		auto record = genRecords(&a, &b, c, d);
		auto insert = API::insert(tablename, *record);
	}
	cout << "insert :" << clock() - s << endl;
	// test create index on b
	string newindex("index2");
	Index index(newindex, tablename, b);
	auto indexRe = API::createIndex(index);
	cout << "index " << indexRe.isSuccess << " " << indexRe.durationMS << indexRe.errorMessage << endl;
	// test delete ( >= 9000)
	int jjd = 9000;
	Value delValue(t1, &jjd);
	Predicate delPre(a, OpType::GEQ, delValue);
	vector<Predicate> delPres;
	//delPres.push_back(delPre);
	auto del = API::deleteFrom(tablename, delPres);
	cout << "delete " << *del.result << ": " << del.isSuccess << " " << del.errorMessage << endl;
	// test select
	int i = 4000;
	Value value(t1, &i);
	Predicate predicate(a, OpType::GT, value);
	vector<Predicate> predicates;
	predicates.push_back(predicate);
	auto select = API::select(propertyNames, tablename, predicates);
	auto selectRe = select.result->first;
	for (int i = 0; i < selectRe.size(); i++) {
		auto re = selectRe[i];
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
	
}