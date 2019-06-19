#pragma once
#include <stdexcept>
#include <string>

namespace Interpreter {

	class ParseError :public std::runtime_error {
	public:
		explicit ParseError(const std::string & msg, const int ln, const int cn);
	};

}