#ifndef HELPER_H
#define HELPER_H
#include <filesystem>
#include <optional>
#include <string>

namespace helper {
std::optional<size_t> parse_size_t(const std::string& str);
}  // namespace helper

#endif  // HELPER_H
