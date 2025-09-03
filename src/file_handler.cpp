#include "file_handler.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <limits>
#include <print>
#include <string>

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

FileHandler::FileHandler(const std::string file_path) : file_path(file_path) {}

std::string FileHandler::get_file_path() const { return this->file_path; }

std::string FileHandler::read() {
  // Open the stream to 'lock' the file.
  const std::filesystem::path path{this->get_file_path()};
  std::ifstream f(path, std::ios::in | std::ios::binary);

  // Obtain the size of the file.
  const auto sz = std::filesystem::file_size(path);

  // Create a buffer.
  std::string result(sz, '\0');

  // Read the whole file into the buffer.
  f.read(result.data(), sz);

  return result;
}

bool FileHandler::write(const std::string &str) const {
  std::ofstream file(this->get_file_path());
  if (!file) {
    throw std::runtime_error("Cannot open file in output mode");
  }
  file << str;
  return file.good();
}
