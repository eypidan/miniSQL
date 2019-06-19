#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include "Exception.h"

enum BaseType
{
	INT,
	FLOAT,
	CHAR
};

class Type
{
private:
	BaseType baseType;
	size_t size;

public:
	Type() = default;

	Type(BaseType baseType, size_t charSize = 0)
	{
		setType(baseType, charSize);
	}

	Type(const Type &that) : baseType(that.baseType), size(that.size) {}

	inline void setType(BaseType baseType, size_t charSize = 0)
	{
		this->baseType = baseType;
		switch (baseType)
		{
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

	inline size_t getSize() const
	{
		return size;
	}
	inline BaseType getBaseType() const
	{
		return baseType;
	}

	bool operator==(const Type &rhs) const
	{
		return baseType == rhs.baseType && size == rhs.size;
	}

	bool operator!=(const Type &rhs) const
	{
		return !(rhs == *this);
	}
};

struct Property
{
	Type type;
	char name[64];
	bool unique;
	Property() {}
	Property(Type type, bool unique, std::string &name) : type(type), unique(unique)
	{
		strcpy(this->name, name.c_str());
	}
};

class Value
{
private:
	Type type;
	void *val;

public:
	Value() = default;
	Value(Type type, void *val) : type(type), val(val) {}
	Value(Type type, const void *val) : type(type)
	{
		setConst(val);
	}
	Value(const Value &that) : type(that.type) {
		int size = that.type.getSize();
		if (size == 0) {
			size = 256;
		}
		val = new char[size];
		if (that.type.getBaseType() == BaseType::CHAR) {
			strcpy((char*)val, (char*)that.val);
		}
		else {
			memcpy(val, that.val, type.getSize());
		}
	}
	template <typename T>
	inline T *getAsType() const
	{
		return reinterpret_cast<T *>(val);
	}

	inline void setConst(const void *val)
	{
		this->val = (void *)val;
	}

	inline void set(void *val)
	{
		this->val = val;
	}

	inline void setType(Type &type)
	{
		this->type = type;
	}

	inline Type getType() const
	{
		return type;
	}

	bool operator!=(const Value &rhs) const { return !(*this == rhs); }
	bool operator>(const Value &rhs) const { return rhs < *this; }
	bool operator<=(const Value &rhs) const { return !(*this > rhs); }
	bool operator>=(const Value &rhs) const { return !(*this < rhs); }
	bool operator<(const Value &rhs) const
	{
		if (type.getBaseType() == BaseType::CHAR && rhs.getType().getBaseType() == BaseType::CHAR) {
			return std::strcmp((char *)val, (char *)rhs.val) < 0;
		}
		if (type.getBaseType() == BaseType::CHAR || rhs.getType().getBaseType() == BaseType::CHAR)
		{
			throw SQLException("cannot compare values with different types");
		}
		float thisValue = type.getBaseType() == BaseType::FLOAT ? *(float*)val : (float)(*(int*)val);
		float thatValue = rhs.getType().getBaseType() == BaseType::FLOAT ? *(float*)rhs.val : (float)(*(int*)rhs.val);
		return thisValue < thatValue;
	}

	bool operator==(const Value &rhs) const
	{
		if (type.getBaseType() == BaseType::CHAR && rhs.getType().getBaseType() == BaseType::CHAR) {
			return std::strcmp((char *)val, (char *)rhs.val) == 0;
		}
		if (type.getBaseType() == BaseType::CHAR || rhs.getType().getBaseType() == BaseType::CHAR)
		{
			throw SQLException("cannot compare values with different types");
		}
		float thisValue = type.getBaseType() == BaseType::FLOAT ? *(float*)val : (float)(*(int*)val);
		float thatValue = rhs.getType().getBaseType() == BaseType::FLOAT ? *(float*)rhs.val : (float)(*(int*)rhs.val);
		return thisValue == thatValue;
	}
};

enum OpType
{
	EQ,  // =
	NE,  // <>
	LT,  // <
	LEQ, // <=
	GT,  // >
	GEQ  // >=
};

struct Predicate
{
	std::string &propertyName;
	OpType op;
	Value &val;
	Predicate(std::string &propertyName, OpType op, Value &val) : propertyName(propertyName), op(op), val(val) {}
};

struct Table
{
	std::string &tableName;
	std::string &primaryKey;
	std::vector<Property> &properties;
	Table(std::string &tableName, std::string &primaryKey, std::vector<Property> &properties)
		: tableName(tableName), primaryKey(primaryKey), properties(properties) {}
};

struct Index
{
	std::string indexName;
	std::string tableName;
	std::string propertyName;
	Index(std::string indexName, std::string tableName, std::string propertyName)
		: indexName(indexName), tableName(tableName), propertyName(propertyName) {}
};

typedef std::vector<std::shared_ptr<std::vector<Value>>> View;
