#pragma once
#include <MetaData.h>

namespace IM {
	bool hasIndex(std::string & indexName);

	void createIndex(Index & index);

	void dropIndex(Index & index);

	// TODO: Implement it after block file is defined.
	// BlockPtr findBlockByIndex(Value &value, Index &index);
}