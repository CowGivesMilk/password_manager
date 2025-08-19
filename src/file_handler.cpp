#include "file_handler.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <limits>
#include <print>
#include <string>

Entry::Entry(const std::optional<std::string>& title,
             const std::optional<std::string>& username,
             const std::optional<std::string>& password,
             const std::optional<std::string>& site,
             const std::optional<std::string>& notes,
             const std::optional<std::vector<std::string>>& tags,
             const std::optional<std::chrono::sys_seconds>& exp_time)
    : title(title),
      username(username),
      password(password),
      site(site),
      notes(notes),
      tags(tags),
      exp_time(exp_time) {}

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

FileHandler::FileHandler(const std::string file_path) : file_path(file_path) {}

bool FileHandler::add_entry(const Entry& entry) {
  std::ofstream out_file(this->get_file_path(), std::ios::app);
  if (!out_file) {
    std::println("Error Cannot open file");
    return false;
  }
  if (std::filesystem::file_size(this->get_file_path()) ==
      0) {  // File is empty. Adding first entry so no newline character
    out_file << entry.to_string();
  } else {
    out_file << std::endl << entry.to_string();
  }

  return out_file.good();
}
std::string FileHandler::get_file_path() const { return this->file_path; }
std::string FileHandler::read_file(const std::filesystem::path path) {
  // Open the stream to 'lock' the file.
  std::ifstream f(path, std::ios::in | std::ios::binary);

  // Obtain the size of the file.
  const auto sz = std::filesystem::file_size(path);

  // Create a buffer.
  std::string result(sz, '\0');

  // Read the whole file into the buffer.
  f.read(result.data(), sz);

  return result;
}
std::string FileHandler::read_file(const std::string path) {
  return FileHandler::read_file(std::filesystem::path(path));
}
std::string FileHandler::read_file() {
  return FileHandler::read_file(this->get_file_path());
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