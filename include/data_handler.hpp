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
  std::optional<std::chrono::system_clock::time_point> exp_time;

 public:
  const std::optional<std::string>& get_title() const;
  const std::optional<std::string>& get_username() const;
  const std::optional<std::string>& get_password() const;
  const std::optional<std::string>& get_site() const;
  const std::optional<std::string>& get_notes() const;
  const std::optional<std::vector<std::string>>& get_tags() const;
  const std::optional<std::chrono::system_clock::time_point>& get_exp_time()
      const;

  Entry();
  Entry(const json& j);
  Entry(const std::optional<std::string>& title,
        const std::optional<std::string>& username,
        const std::optional<std::string>& password,
        const std::optional<std::string>& site,
        const std::optional<std::string>& notes,
        const std::optional<std::vector<std::string>>& tags,
        const std::optional<std::chrono::system_clock::time_point>& exp_time);

  const std::string to_string() const;
  json to_json() const;
};
#endif