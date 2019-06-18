#pragma once
#include "MetaData.h"
#include "Exception.h"


namespace Interpreter {
	namespace AST {
		class Statement
		{
		public:
			Statement();
			virtual ~Statement();
			virtual void callAPI();
			//virtual void printStatement();
		};

		class CreateTableStatement: public Statement
		{
		public:
			void callAPI() override;
			void setTableName(std::string &);
			void setPrimaryKey(std::string &);
			void addProperty(Property &);

		private:
			std::string tableName;
			std::string primaryKey;
			std::vector<Property> properties;
		};

		class DropStatement: public Statement
		{
		public:
			void setTableName(std::string &);
			void callAPI() override;

		private:
			std::string tableName;
		};

		class CreateIndexStatement : public Statement
		{
		public:
			void callAPI() override;
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
			void callAPI() override;
		private:
			std::string indexName;
		};

		class SelectStatement : public Statement
		{
		public:
			void addRequiredProperty(std::string &);
			void addPredicate(Predicate &);
			void setTableName(std::string &);
			void callAPI() override;
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
			void callAPI() override;
		private:
			std::string tableName;
			std::vector<Value> values;
		};

		class DeleteStatement : public Statement
		{
		public:
			void setTableName(std::string &);
			void addPredicate(Predicate &);
			void callAPI() override;
		private:
			std::string tableName;
			std::vector<Predicate> predicates;
		};

		class QuitStatement : public Statement
		{
		public:
			void callAPI() override;
		};


		class ExecfileStatement : public Statement
		{
		public:
			void setFilePath(std::string &);
			void callAPI() override;
		private:
			std::string filePath;
		};



		
	}
}