#pragma once
#include "MetaData.h"
#include "Exception.h"
#include <sstream>


namespace Interpreter {
	namespace AST {
		class Statement
		{
		public:
			Statement() = default;
			virtual ~Statement() = default;
			//virtual void callAPI();
			virtual void printStatement() = 0;
		};

		class CreateTableStatement: public Statement
		{
		public:
			//void callAPI() override;
			void printStatement() override;
			void setTableName(std::string &);
			void setPrimaryKey(std::string &);
			void addProperty(const Property &);

		private:
			std::string tableName;
			std::string primaryKey;
			std::vector<Property> properties;
		};

		class DropStatement: public Statement
		{
		public:
			void setTableName(std::string &);
			//void callAPI() override;
			void printStatement() override;

		private:
			std::string tableName;
		};

		class CreateIndexStatement : public Statement
		{
		public:
			//void callAPI() override;
			void printStatement() override;
			void setIndexName(std::string &);
			void setTableName(std::string &);
			void setPropertyName(std::string &);
		private:
			std::string indexName;
			std::string tableName;
			std::string propertyName;
		};

		class DropIndexStatement : public Statement
		{
		public:
			void setIndexName(std::string &);
			//void callAPI() override;
			void printStatement() override;
		private:
			std::string indexName;
		};

		class SelectStatement : public Statement
		{
		public:
			void addRequiredProperty(std::string &);
			void addPredicate(Predicate &);
			void setTableName(std::string &);
			//void callAPI() override;
			void printStatement() override;
			template<typename T>
			inline int getLength(const T& t) {
				std::ostringstream os;
				os << t;
				return os.str().size();
			}

		private:
			std::vector<std::string> properties;
			std::string tableName;
			std::vector<Predicate> predicates;
		};

		class InsertStatement : public Statement
		{
		public:
			void setTableName(std::string &);
			void addValue(Value &);
			//void callAPI() override;
			void printStatement() override;
		private:
			std::string tableName;
			std::vector<Value> values;
		};

		class DeleteStatement : public Statement
		{
		public:
			void setTableName(std::string &);
			void addPredicate(Predicate &);
			//void callAPI() override;
			void printStatement() override;
		private:
			std::string tableName;
			std::vector<Predicate> predicates;
		};

		class QuitStatement : public Statement
		{
		public:
			//void callAPI() override;
			void printStatement() override;
		};


		class ExecfileStatement : public Statement
		{
		public:
			void setFilePath(std::string &);
			void printStatement() override;
			//void callAPI() override;
		private:
			std::string filePath;
		};
	}
}