#pragma once
#include "MetaData.h"
#include "Exception.h


namespace Interpreter {
	namespace AST {
		class Statement
		{
		public:
			Statement();
			virtual ~Statement();
			virtual void callAPI() const;
		};

		class CreateTableStatement: public Statement
		{
		public:
			void callAPI() const override;

		private:
			std::string tableName;
			std::string primaryKey;
			std::vector<Property> properties;
		};

		class DropStatement: public Statement
		{
		public:
			void callAPI() const override;

		private:
			std::string tableName;
		};

		class CreateIndexStatement : public Statement
		{
		public:
			void callAPI() const override;
		private:
			std::string indexName;
			std::string tableName;
			std::string propertyName;
		};

		class DropIndexStatement : public Statement
		{
		public:
			void callAPI() const override;
		private:
			std::string indexName;
		};

		class SelectStatement : public Statement
		{
		public:
			void callAPI() const override;
		private:
			std::vector<std::string> properties;
			std::string tableName;
			std::vector<Predicate> predicates;
		};

		class InsertStatement : public Statement
		{
		public:
			void callAPI() const override;
		private:
			std::string tableName;
			std::vector<Value> values;
		};

		class DeleteStatement : public Statement
		{
		public:
			void callAPI() const override;
		private:
			std::string tableName;
			std::vector<Predicate> predicates;
		};

		class QuitStatement : public Statement
		{
		public:
			void callAPI() const override;
		};


		class ExecfileStatement : public Statement
		{
		public:
			void callAPI() const override;
		private:
			std::string filePath;
		};



		
	}
}