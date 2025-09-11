#include <data_handler.hpp>
Entry::Entry(const std::optional<std::string> &title,
             const std::optional<std::string> &username,
             const std::optional<std::string> &password,
             const std::optional<std::string> &site,
             const std::optional<std::string> &notes,
             const std::optional<std::vector<std::string>> &tags,
             const std::optional<std::chrono::sys_seconds> &exp_time)
    : title(std::move(title)),
      username(std::move(username)),
      password(std::move(password)),
      site(std::move(site)),
      notes(std::move(notes)),
      tags(std::move(tags)),
      exp_time(std::move(exp_time)) {}
const std::optional<std::string> &Entry::getTitle() const { return title; }
const std::optional<std::string> &Entry::getUsername() const {
  return username;
}
const std::optional<std::string> &Entry::getPassword() const {
  return password;
}
const std::optional<std::string> &Entry::getSite() const { return site; }
const std::optional<std::string> &Entry::getNotes() const { return notes; }
const std::optional<std::vector<std::string>> &Entry::getTags() const {
  return tags;
}
const std::optional<std::chrono::sys_seconds> &Entry::getExpTime() const {
  return exp_time;
}
const std::string Entry::to_string() const {
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

json Entry::to_json() const {
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