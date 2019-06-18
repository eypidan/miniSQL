#include "ParseError.h"
#include <sstream>

namespace Interpreter {
	static std::string errMsg(const std::string & msg, const int ln, const int cn) {
		std::stringstream s;
		if (ln == -1 && cn == -1) {
			s << "At the end of input:" << std::endl;
		}
		else {
			s << "Line " << ln << ", Column " << cn << ":" << std::endl;
		}
		s << "ParseError: " << msg;
		return s.str();
	}

	ParseError::ParseError(const std::string & msg, const int ln, const int cn) 
		: std::runtime_error(errMsg(msg, ln, cn)) {
	};
}

