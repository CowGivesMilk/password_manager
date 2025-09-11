#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <chrono>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

using json = nlohmann::json;
class Entry {
 private:
  std::optional<std::string> title, username, password, site, notes;
  std::optional<std::vector<std::string>> tags;
  std::optional<std::chrono::sys_seconds> exp_time;
  const std::optional<std::string>& getTitle() const;
  const std::optional<std::string>& getUsername() const;
  const std::optional<std::string>& getPassword() const;
  const std::optional<std::string>& getSite() const;
  const std::optional<std::string>& getNotes() const;
  const std::optional<std::vector<std::string>>& getTags() const;
  const std::optional<std::chrono::sys_seconds>& getExpTime() const;

 public:
  Entry(const std::optional<std::string>& title,
        const std::optional<std::string>& username,
        const std::optional<std::string>& password,
        const std::optional<std::string>& site,
        const std::optional<std::string>& notes,
        const std::optional<std::vector<std::string>>& tags,
        const std::optional<std::chrono::sys_seconds>& exp_time);
  const std::string to_string() const;
  json to_json() const;
};

#endif