#pragma once
#include <time.h>
#include <MetaData.h>

namespace API {
	/**
	 * template struct of SQL result returned by API functions.
	 */
	template <typename T>
	struct SQLResult {
		std::shared_ptr<T> result; // result
		clock_t durationMS; // time period in millisecond
		bool isSuccess = true;
		const std::string errorMessage;

		SQLResult(std::shared_ptr<T> result, clock_t durationMS, bool isSuccess, const std::string &errorMessage)
			: result(result), durationMS(durationMS), isSuccess(isSuccess), errorMessage(errorMessage) {}
	};

	
	SQLResult<void> createTable(Table &table);

	SQLResult<void> dropTable(std::string & tableName);

	SQLResult<void> createIndex(Index &index);

	SQLResult<void> dropIndex(std::string & indexName);

	/**
	 * Do something like "select [properties] from [tableName] where [predicates]"
	 * @param properties: projection properties
	 * @param tableName: from clause
	 * @param predicates: where clauses linked by "and"
	 * @return: select results and table specified
	 */
	SQLResult<std::pair<View, Table>> select(
		std::vector<std::string> &properties,
		std::string &tableName,
		std::vector<Predicate> &predicates
	);

	/**
	 * Do something like "insert into [tableName] values ([values])"
	 * @param tableName
	 * @param values: record
	 */
	SQLResult<void> insert(
		std::string &tableName,
		std::vector<Value> &values
	);

	/**
	 * Do something like "delete from [tableName] where [predicates]"
	 * @param tableName: from clause
	 * @param predicates: where clauses linked by "and"
	 * @return: the total number of deleted records
	 */
	SQLResult<int> deleteFrom(
		std::string &tableName,
		std::vector<Predicate> &predicates
	);

}