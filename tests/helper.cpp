#include "helper.hpp"

#include <catch2/catch_all.hpp>
#include <iostream>

TEST_CASE("parse_size_t(str, out)") {
  size_t output = 777;
  REQUIRE((helper::parse_size_t("12345", output)));
  REQUIRE(output == 12345);
  REQUIRE((helper::parse_size_t("-12345", output)));
  REQUIRE(not(helper::parse_size_t("aeiou", output)));
}