#include "data_handler.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Entry default constructor initializes all fields to nullopt",
          "[Entry]") {
  Entry entry;

  REQUIRE_FALSE(entry.get_title().has_value());
  REQUIRE_FALSE(entry.get_username().has_value());
  REQUIRE_FALSE(entry.get_password().has_value());
  REQUIRE_FALSE(entry.get_site().has_value());
  REQUIRE_FALSE(entry.get_notes().has_value());
  REQUIRE_FALSE(entry.get_tags().has_value());
  REQUIRE_FALSE(entry.get_exp_time().has_value());
}

TEST_CASE("Entry parameterized constructor sets values correctly", "[Entry]") {
  std::optional<std::string> title = "Test Title";
  std::optional<std::string> username = "test_user";
  std::optional<std::string> password = "test_password";
  std::optional<std::string> site = "https://example.com";
  std::optional<std::string> notes = "Test notes";
  std::optional<std::vector<std::string>> tags =
      std::vector<std::string>{"tag1", "tag2"};
  auto exp_time = std::chrono::system_clock::now();

  Entry entry(title, username, password, site, notes, tags, exp_time);

  REQUIRE(entry.get_title() == title);
  REQUIRE(entry.get_username() == username);
  REQUIRE(entry.get_password() == password);
  REQUIRE(entry.get_site() == site);
  REQUIRE(entry.get_notes() == notes);
  REQUIRE(entry.get_tags() == tags);
  REQUIRE(entry.get_exp_time() == exp_time);
}

TEST_CASE("Entry to_json includes only set fields", "[Entry]") {
  Entry entry("Title", "user", "pass", std::nullopt, std::nullopt, std::nullopt,
              std::nullopt);

  json j = entry.to_json();

  REQUIRE(j["Title"] == "Title");
  REQUIRE(j["Username"] == "user");
  REQUIRE(j["Password"] == "pass");
  REQUIRE_FALSE(j.contains("Site"));
  REQUIRE_FALSE(j.contains("Notes"));
  REQUIRE_FALSE(j.contains("Tags"));
  REQUIRE_FALSE(j.contains("Expire Time"));
}

TEST_CASE("Entry JSON constructor handles all fields correctly", "[Entry]") {
  json j;
  j["Title"] = "JSON Title";
  j["Username"] = "json_user";
  j["Password"] = "json_pass";
  j["Site"] = "https://json.com";
  j["Notes"] = "JSON notes";
  j["Tags"] = {"json1", "json2"};
  auto expected_time = std::chrono::system_clock::now();
  j["Expire Time"] = expected_time.time_since_epoch().count();

  Entry entry(j);

  REQUIRE(entry.get_title().value() == "JSON Title");
  REQUIRE(entry.get_username().value() == "json_user");
  REQUIRE(entry.get_password().value() == "json_pass");
  REQUIRE(entry.get_site().value() == "https://json.com");
  REQUIRE(entry.get_notes().value() == "JSON notes");
  REQUIRE(entry.get_tags().value() ==
          std::vector<std::string>{"json1", "json2"});
  REQUIRE(entry.get_exp_time().value().time_since_epoch().count() ==
          expected_time.time_since_epoch().count());
}

TEST_CASE("Entry to_string formats correctly with empty fields", "[Entry]") {
  Entry entry;
  std::string result = entry.to_string();

  // Should contain all field labels but empty values
  REQUIRE(result.find("Title: ") != std::string::npos);
  REQUIRE(result.find("Username: ") != std::string::npos);
  REQUIRE(result.find("Password: ") != std::string::npos);
  REQUIRE(result.find("Site: ") != std::string::npos);
  REQUIRE(result.find("Notes: ") != std::string::npos);
  REQUIRE(result.find("Date Time: ") != std::string::npos);
}

TEST_CASE("Entry round-trip JSON serialization", "[Entry]") {
  std::optional<std::string> title = "Test Title";
  std::optional<std::string> username = "test_user";
  std::optional<std::string> password = "test_password";
  std::optional<std::string> site = "https://example.com";
  std::optional<std::string> notes = "Test notes";
  std::optional<std::vector<std::string>> tags =
      std::vector<std::string>{"tag1", "tag2"};
  auto exp_time = std::chrono::system_clock::now();

  Entry original(title, username, password, site, notes, tags, exp_time);
  json j = original.to_json();
  Entry reconstructed(j);

  REQUIRE(reconstructed.get_title() == original.get_title());
  REQUIRE(reconstructed.get_username() == original.get_username());
  REQUIRE(reconstructed.get_password() == original.get_password());
  REQUIRE(reconstructed.get_site() == original.get_site());
  REQUIRE(reconstructed.get_notes() == original.get_notes());
  REQUIRE(reconstructed.get_tags() == original.get_tags());

  // Compare time points by converting to time_since_epoch
  if (reconstructed.get_exp_time().has_value() &&
      original.get_exp_time().has_value()) {
    REQUIRE(reconstructed.get_exp_time().value().time_since_epoch().count() ==
            original.get_exp_time().value().time_since_epoch().count());
  } else {
    REQUIRE(reconstructed.get_exp_time().has_value() ==
            original.get_exp_time().has_value());
  }
}