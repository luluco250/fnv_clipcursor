#include "utils.hpp"

#include "windows_lean.h"

#include <string>
#include <sstream>

#include "common/IDebugLog.h"

std::string get_last_error_message(size_t max_size) {
	std::string output(max_size, '\0');
	
	get_last_error_message(output);
	
	return output;
}

void get_last_error_message(std::string& output) {
	if (output.empty())
		return;

	size_t written_count = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		&output[0],
		output.size(),
		NULL);

	output.resize(written_count);
}

void fatal_error(const char* message, bool include_last_error) {
	std::stringstream ss;
	
	if (message != NULL)
		ss << message << '\n';

	if (include_last_error)
		ss << "Last error:\n" << get_last_error_message();

	std::string output = ss.str();
	_ERROR(output.c_str());
}