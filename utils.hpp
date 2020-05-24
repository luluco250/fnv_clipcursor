#ifndef FNV_CLIPCURSOR_UTILS_HPP
#define FNV_CLIPCURSOR_UTILS_HPP

#include <string>

#define EXPORT __declspec(dllexport)

std::string get_last_error_message(size_t max_size = 256);

void get_last_error_message(std::string& output);

void fatal_error(const char* message, bool include_last_error = true);

#endif // Include guard.