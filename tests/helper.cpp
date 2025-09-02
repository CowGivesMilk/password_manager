#include "helper.hpp"

#include <catch2/catch_all.hpp>
#include <limits>
#include <print>
TEST_CASE("parse_size_t(str)") {
  std::string valid1 = "6238", valid2 = std::to_string(SIZE_MAX);
  std::string
      invalid1 = "-77",
      invalid2 =
          "99999999999999999999999999999999999999999999999999999999999999999999"
          "999999999999999999999999999999999999999999999999999999999999999",
      invalid3 = "iqwheli89689oh", invalid4 = "";
  REQUIRE((*helper::parse_size_t(valid1) == 6238uz));
  REQUIRE((*helper::parse_size_t(valid2) == SIZE_MAX));
  REQUIRE((helper::parse_size_t(invalid1) == std::nullopt));
  REQUIRE((helper::parse_size_t(invalid2) == std::nullopt));
  REQUIRE((helper::parse_size_t(invalid3) == std::nullopt));
  REQUIRE((helper::parse_size_t(invalid4) == std::nullopt));
}