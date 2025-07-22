#include "helper_functions.hpp"

#include <catch2/catch_all.hpp>
#include <iostream>

TEST_CASE("parse_size_t(str, out)") {
  size_t output = 777;
  REQUIRE((HelperFunctions::parse_size_t("12345", output)));
  REQUIRE(output == 12345);
  REQUIRE((HelperFunctions::parse_size_t("-12345", output)));
  REQUIRE(not(HelperFunctions::parse_size_t("aeiou", output)));
}