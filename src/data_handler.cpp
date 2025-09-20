#include "data_handler.hpp"
Entry::Entry()
    : title(std::nullopt),
      username(std::nullopt),
      password(std::nullopt),
      site(std::nullopt),
      notes(std::nullopt),
      tags(std::nullopt),
      exp_time(std::nullopt) {}
Entry::Entry(
    const std::optional<std::string> &title,
    const std::optional<std::string> &username,
    const std::optional<std::string> &password,
    const std::optional<std::string> &site,
    const std::optional<std::string> &notes,
    const std::optional<std::vector<std::string>> &tags,
    const std::optional<std::chrono::system_clock::time_point> &exp_time)
    : title(std::move(title)),
      username(std::move(username)),
      password(std::move(password)),
      site(std::move(site)),
      notes(std::move(notes)),
      tags(std::move(tags)),
      exp_time(std::move(exp_time)) {}
const std::optional<std::string> &Entry::get_title() const noexcept {
  return title;
}
const std::optional<std::string> &Entry::get_username() const noexcept {
  return username;
}
const std::optional<std::string> &Entry::get_password() const noexcept {
  return password;
}
const std::optional<std::string> &Entry::get_site() const noexcept {
  return site;
}
const std::optional<std::string> &Entry::get_notes() const noexcept {
  return notes;
}
const std::optional<std::vector<std::string>> &Entry::get_tags()
    const noexcept {
  return tags;
}
const std::optional<std::chrono::system_clock::time_point> &
Entry::get_exp_time() const noexcept {
  return exp_time;
}
const std::string Entry::to_string() const noexcept {
  std::string date_time{}, tags_string{};
  if (exp_time.has_value()) {
    date_time = std::format("{0:%Y-%m-%d %H:%M:%S}", exp_time.value());
  }
  if (tags.has_value()) {
    std::for_each(tags.value().begin(), tags.value().end(),
                  [&tags_string](std::string tag) { tags_string.append(tag); });
  }
  return std::format(
      "Title: {}\nUsername: {}\nPassword: {}\nSite: {}\nNotes: {}\nDate Time: "
      "{}",
      title.value_or(""), username.value_or(""), password.value_or(""),
      site.value_or(""), notes.value_or(""), date_time, tags_string);
}

json Entry::to_json() const noexcept {
  json j;
  if (this->title) j["Title"] = this->title.value();
  if (this->username) j["Username"] = this->username.value();
  if (this->password) j["Password"] = this->password.value();
  if (this->site) j["Site"] = this->site.value();
  if (this->notes) j["Notes"] = this->notes.value();
  if (this->tags) j["Tags"] = this->tags.value();
  if (this->exp_time)
    j["Expire Time"] = this->exp_time.value().time_since_epoch().count();
  return j;
}

Entry::Entry(const json &j)
    : title(j.contains("Title")
                ? std::make_optional(j.at("Title").get<std::string>())
                : std::nullopt),
      username(j.contains("Username")
                   ? std::make_optional(j.at("Username").get<std::string>())
                   : std::nullopt),
      password(j.contains("Password")
                   ? std::make_optional(j.at("Password").get<std::string>())
                   : std::nullopt),
      site(j.contains("Site")
               ? std::make_optional(j.at("Site").get<std::string>())
               : std::nullopt),
      notes(j.contains("Notes")
                ? std::make_optional(j.at("Notes").get<std::string>())
                : std::nullopt),
      tags(
          j.contains("Tags")
              ? std::make_optional(j.at("Tags").get<std::vector<std::string>>())
              : std::nullopt),
      exp_time(j.contains("Expire Time")
                   ? std::make_optional(std::chrono::system_clock::time_point(
                         std::chrono::system_clock::duration(
                             j.at("Expire Time").get<long long>())))
                   : std::nullopt) {}