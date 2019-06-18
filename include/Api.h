#pragma once
#include <time.h>
#include "MetaData.h"

namespace API {
	template <typename T>
	struct SQLResult {
		std::shared_ptr<T> result;
		clock_t durationMS;
		bool isSuccess = true;
		const std::string &errorMessage;

		SQLResult(std::shared_ptr<T> result, clock_t durationMS, bool isSuccess, const std::string &errorMessage)
			: result(result), durationMS(durationMS), isSuccess(isSuccess), errorMessage(errorMessage) {}
	};

	SQLResult<void> createTable(Table &table);

	SQLResult<void> dropTable(Table &table);

	SQLResult<void> createIndex(Index &index);

	SQLResult<void> dropIndex(Index &index);

	SQLResult<std::pair<View, Table>> select(
		std::vector<std::string> &properties,
		std::string &tableName,
		std::vector<Predicate> &predicates
	);

	SQLResult<void> insert(
		std::string &tableName,
		std::vector<Value> &values
	);

	SQLResult<int> deleteFrom(
		std::string &tableName,
		std::vector<Predicate> &predicates
	);

}