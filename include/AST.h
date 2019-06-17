#pragma once
#include "MetaData.h"

namespace Interpreter {
	namespace AST {
		class Statement
		{
		public:
			Statement();
			~Statement();
			void callAPI() const;
		};

		class CreateStatement: public Statement
		{
		public:
			CreateStatement();
			~CreateStatement();

		private:
			std::string tableName;
			std::string primaryKey;
			std::vector<Property> properties;
		};

	}
}