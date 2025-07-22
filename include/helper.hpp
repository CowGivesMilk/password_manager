#ifndef HELPER_H
#define HELPER_H
#include <filesystem>
#include <string>

namespace helper {
std::string get_db_path(char* exe);
bool parse_size_t(const std::string& str, size_t& out);
}  // namespace helper

#endif  // HELPER_H