#pragma once
#include <string>

struct SQLException {
	const std::string msg;
	SQLException(const std::string msg) : msg(msg) {}
};