#include <catch2/catch_all.hpp>
#include <string>

#include "password_generator.hpp"

typedef struct {
  bool nums = false, cap = false, small = false, symbols = false;
} Contains;

Contains string_contains(const std::string& str) {
  Contains res;
  for (char c : str) {
    if (std::isdigit(c))
      res.nums = true;
    else if (std::islower(c))
      res.small = true;
    else if (std::isupper(c))
      res.cap = true;
    else
      res.symbols = true;
  }
  return res;
}

TEST_CASE("Length of password equal to requirement if under 1000") {
  PasswordGenerator obj;
  size_t len = 99;
  REQUIRE(len == obj.generatePassword(len).length());
}

TEST_CASE("Length of password set to 256 if above 1000") {
  PasswordGenerator obj;
  size_t len = 10'000;
  REQUIRE(256 == obj.generatePassword(len).length());
}

TEST_CASE("Constructer Tests") {
  PasswordGenerator all_true, small(false, true, false, false),
      capital(false, false, true, false), symbols(false, false, false, true),
      numbers(true, false, false, false),
      fallback_small(false, false, false, false);

  std::string only_small = small.generatePassword(20),
              only_capital = capital.generatePassword(20),
              only_numbers = numbers.generatePassword(20),
              only_symbols = symbols.generatePassword(20),
              fallback_small_str = fallback_small.generatePassword(20),
              all = all_true.generatePassword(256);
  Contains contains_small = string_contains(only_small),
           contains_capital = string_contains(only_capital),
           contains_numbers = string_contains(only_numbers),
           contains_symbols = string_contains(only_symbols),
           contains_small_fallback = string_contains(fallback_small_str),
           contains_all = string_contains(all);
  // SMALL
  REQUIRE((not contains_small.cap and not contains_small.nums and
           contains_small.small and not contains_small.symbols));

  // CAPITAL
  REQUIRE((contains_capital.cap and not contains_capital.nums and
           not contains_capital.small and not contains_capital.symbols));

  // NUMBERS
  REQUIRE((not contains_numbers.cap and contains_numbers.nums and
           not contains_numbers.small and not contains_numbers.symbols));

  // SYMBOLS
  REQUIRE((not contains_symbols.cap and not contains_symbols.nums and
           not contains_symbols.small and contains_symbols.symbols));

  // FALLBACK SMALL
  REQUIRE(
      (not contains_small_fallback.cap and not contains_small_fallback.nums and
       contains_small_fallback.small and not contains_small_fallback.symbols));

  // Contains ALL (PROBABILISTIC)
  CHECK((contains_all.cap and contains_all.nums and contains_all.small and
         contains_all.symbols));
}