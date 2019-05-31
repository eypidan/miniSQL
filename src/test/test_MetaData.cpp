#include <iostream>
#include <string>
#include <vector>
#include <MetaData.h>

using namespace std;

int main(void)
{
	string name("test");
	Property property(Type(BaseType::CHAR, 23), name);
	Value value(Type(BaseType::CHAR, 23), name.c_str());
	std::cout << property.type.getSize() << value.getAsType<char>();
}