#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include <cryptopp/aes.h>

#include <chrono>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>
using json = nlohmann::json;
class Entry {
 private:
  std::optional<std::string> title, username, password, site, notes;
  std::optional<std::vector<std::string>> tags;
  std::optional<std::chrono::sys_time<std::chrono::seconds>> exp_time;

 public:
  Entry(const std::optional<std::string>& title,
        const std::optional<std::string>& username,
        const std::optional<std::string>& password,
        const std::optional<std::string>& site,
        const std::optional<std::vector<std::string>>& tags,
        const std::optional<std::chrono::sys_time<std::chrono::seconds>>&
            exp_time);
  const std::string to_string() const;
  json to_json() const;
};
class FileHandler {
 private:
  std::string file_path;

 public:
  FileHandler(const std::string file_path);
  bool add_entry(const Entry& entry);
  std::string get_file_path() const;
  static std::string read_file(
      const std::filesystem::path path);  // Reads full file
  static std::string read_file(const std::string path);
  std::string read_file();
};

#endif  // FILE_HANDLER_H