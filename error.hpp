#pragma once

#include <cstdlib>
#include <print>

#define ERROR(msg) {\
	std::println("Error in file \"{}\", function \"{}\", line {}: {}", __FILE__, __func__, __LINE__, msg);\
	std::exit(1);\
}
