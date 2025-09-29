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
  [[nodiscard]] const std::optional<std::string>& get_title() const noexcept;
  [[nodiscard]] const std::optional<std::string>& get_username() const noexcept;
  [[nodiscard]] const std::optional<std::string>& get_password() const noexcept;
  [[nodiscard]] const std::optional<std::string>& get_site() const noexcept;
  [[nodiscard]] const std::optional<std::string>& get_notes() const noexcept;
  [[nodiscard]] const std::optional<std::vector<std::string>>& get_tags()
      const noexcept;
  [[nodiscard]] const std::optional<std::chrono::system_clock::time_point>&
  get_exp_time() const noexcept;

  Entry();
  explicit Entry(const json& j);
  Entry(const std::optional<std::string>& title,
        const std::optional<std::string>& username,
        const std::optional<std::string>& password,
        const std::optional<std::string>& site,
        const std::optional<std::string>& notes,
        const std::optional<std::vector<std::string>>& tags,
        const std::optional<std::chrono::system_clock::time_point>& exp_time);

  [[nodiscard]] const std::string to_string() const noexcept;
  [[nodiscard]] json to_json() const noexcept;
};
std::vector<Entry> to_entry_list(const std::string& file_blob);
json combine_json(const std::vector<json>& json_array);
json combine_json(const std::vector<Entry>& entry_array);
std::vector<Entry> to_entry_list(const json& json_array);
#endif