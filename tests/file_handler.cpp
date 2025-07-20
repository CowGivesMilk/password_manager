#include "file_handler.hpp"

#include <catch2/catch_all.hpp>
#include <format>
#include <fstream>
#include <iostream>

Entry entry1("username1", "password1", "https://site1.com");
Entry entry2("username2", "password2", "https://site2.com");
FileHandler handler1("data/test_password.csv");
void truncate(const FileHandler& file_handler) {
  std::ifstream file(file_handler.get_file_path(), std::ios::trunc);
}
std::string get_last_line(const FileHandler& file_handler) {
  std::ifstream file(file_handler.get_file_path());
  std::string line;
  if (!file.is_open()) {
    std::cerr << "Failed to open File" << std::endl;
    throw std::runtime_error(
        std::format("Failed to open File {}", file_handler.get_file_path()));
  }
  while (getline(file, line));
  file.close();
  return line;
}
TEST_CASE("Entry Class Test") {
  std::string uname = "Username", pass = "Password",
              url = "https://password_manager.com";
  Entry e1(uname, pass, url);
  REQUIRE(std::format("{} {} {}", url, uname, pass) == e1.to_string());
}
TEST_CASE("Adding Entry Check") {
  truncate(handler1);
  REQUIRE(handler1.add_entry(entry1));
  CHECK(entry1.to_string() == get_last_line(handler1));

  handler1.add_entry(entry1);
  handler1.add_entry(entry1);
  handler1.add_entry(entry1);
  handler1.add_entry(entry2);
  CHECK(entry2.to_string() == get_last_line(handler1));
}