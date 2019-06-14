#include <Api.h>
#include <time.h>
#include <Constant.h>
#include <Exception.h>
#include <CatalogManager.h>
#include <RecordManager.h>
#include <IndexManager.h>
#include <MetaData.h>

namespace API {

	SQLResult<void> createTable(Table &table) {
		clock_t start = clock();
		try {
			CM::createTable(table);
			RM::createTable(table);
			Index index(
				getPrimaryIndexName(table.tableName, table.primaryKey),
				table.tableName, table.primaryKey
			);
			createIndex(index);
			return SQLResult<void>(std::make_shared<void>(), clock() - start, true, "");
		}
		catch (SQLException e) {
			return SQLResult<void>(std::make_shared<void>(), clock() - start, false, e.msg);
		}
	}

	SQLResult<void> dropTable(Table &table) {
		clock_t start = clock();
		try {
			CM::dropTable(table);
			RM::dropTable(table);
			return SQLResult<void>(std::make_shared<void>(), clock() - start, true, "");
		}
		catch (SQLException e) {
			return SQLResult<void>(std::make_shared<void>(), clock() - start, false, e.msg);
		}
	}

	SQLResult<void> createIndex(Index &index) {
		clock_t start = clock();
		try {
			CM::createIndex(index);
			IM::createIndex(index);
			return SQLResult<void>(std::make_shared<void>(), clock() - start, true, "");
		}
		catch (SQLException e) {
			return SQLResult<void>(std::make_shared<void>(), clock() - start, false, e.msg);
		}
	}

	SQLResult<void> dropIndex(Index &index) {
		clock_t start = clock();
		try {
			CM::dropIndex(index);
			IM::dropIndex(index);
			return SQLResult<void>(std::make_shared<void>(), clock() - start, true, "");
		}
		catch (SQLException e) {
			return SQLResult<void>(std::make_shared<void>(), clock() - start, false, e.msg);
		}
	}

	SQLResult<View> select(
		std::vector<std::string> &properties,
		std::string &tableName,
		std::vector<Predicate> &predicates
	) {
		clock_t start = clock();
		try {
			Table &table = CM::findTable(tableName);
			View &result = RM::selectRecords(properties, table, predicates);
			return SQLResult<View>(
				std::make_shared<View>(result), clock() - start, true, "");
		}
		catch (SQLException e) {
			return SQLResult<std::vector<std::vector<Value>>>(
				std::make_shared<View>(), clock() - start, false, e.msg);
		}
	}

	SQLResult<int> deleteFrom(
		std::string &tableName,
		std::vector<Predicate> &predicates
	) {
		clock_t start = clock();
		try {
			int result = 0;
			Table &table = CM::findTable(tableName);
			if (predicates.empty()) {
				result = RM::deleteAllRecords(table);
			}
			else {
				result = RM::deleteRecords(table, predicates);
			}
			return SQLResult<int>(
				std::make_shared<int>(result), clock() - start, true, "");
		}
		catch (SQLException e) {
			return SQLResult<int>(
				std::make_shared<int>(), clock() - start, false, e.msg);
		}
	}

}
