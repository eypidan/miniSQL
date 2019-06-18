#pragma once
#include <string>
#include <vector>
#include <memory>

enum BaseType { 
	INT, 
	FLOAT, 
	CHAR 
};

class Type {
private:
	BaseType baseType;
	size_t size;
public:

	Type(BaseType baseType, size_t charSize = 0) {
		setType(baseType, charSize);
	}

	inline void setType(BaseType baseType, size_t charSize = 0) {
		switch (baseType) {
			case BaseType::INT:
				size = sizeof(int);
				break;
			case BaseType::FLOAT:
				size = sizeof(float);
				break;
			case BaseType::CHAR:
				size = charSize;
				break;
		}
	}

	inline size_t getSize() const {
		return size;
	}
	inline BaseType getBaseType() const {
		return baseType;
	}
};

struct Property {
	Type &type;
	std::string &name;
	bool isUnique;
	Property(Type &type, std::string &name, bool isUnique) : type(type), name(name), isUnique(isUnique) {}
};

class Value {
private:
	Type &type;
	void *val;
public:
	Value(Type &type, void *val) : type(type), val(val) {}
	Value(Type &type, const void *val) : type(type) {
		setConst(val);
	}
	Value(Value & that) : type(that.type), val(that.val) {}
	template<typename T>
	inline T* getAsType() const {
		return reinterpret_cast<T*>(val);
	}

	inline void setConst(const void* val) {
		this->val = (void*)val;
	}

	inline void set(void* val) {
		this->val = val;
	}

	inline void setType(Type &type) {
		this->type = type;
	}

	inline Type& getType() const {
		return type;
	}

};

enum OpType { 
	EQ, // =
	NE, // <>
	LT, // <
	LEQ, // <=
	GT, // >
	GEQ // >=
};

struct Predicate {
	std::string &propertyName;
	OpType op;
	Value &val;
	Predicate(std::string &propertyName, OpType op, Value &val) : propertyName(propertyName), op(op), val(val) {}
};

struct Table {
	std::string &tableName;
	std::string &primaryKey;
	std::vector<Property> &properties;
	Table(std::string &tableName, std::string &primaryKey, std::vector<Property> &properties)
		: tableName(tableName), primaryKey(primaryKey), properties(properties) {}
};

struct Index {
	std::string &indexName;
	std::string &tableName;
	std::string &propertyName;
	Index(std::string &indexName, std::string &tableName, std::string &propertyName)
		: indexName(indexName), tableName(tableName), propertyName(propertyName) {}
};

typedef std::vector<std::vector<Value>> View;