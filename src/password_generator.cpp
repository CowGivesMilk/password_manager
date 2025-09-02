#include "password_generator.hpp"

#include <array>
#include <functional>
#include <print>
#include <random>

PasswordGenerator::PasswordGenerator()
    : numbers(true),
      small(true),
      capital(true),
      special_characters(true),
      rng(std::random_device{}()) {}

PasswordGenerator::PasswordGenerator(bool numbers, bool small, bool capital,
                                     bool special_characters)
    : numbers(numbers),
      small(small),
      capital(capital),
      special_characters(special_characters),
      rng(std::random_device{}()) {
  if (not(numbers || small || capital || special_characters)) {
    std::println(
        "All fields can't be abscent so switching small alphabets on.");
    this->small = true;
  }
}

std::string PasswordGenerator::generatePassword(size_t length) {
  if (length > 999) {
    std::println(
        "You don't need that long password.\nReturning 256 character long "
        "password instead!");
    length = 256;
  }
  std::string password;
  password.reserve(length);

  auto getRandomSymbol = [this]() {
    int lower = 0, upper = this->symbols.size() - 1;
    std::uniform_int_distribution<size_t> dist(lower, upper);
    return symbols[dist(rng)];
  };
  auto getRandomNumber = [this]() {
    std::uniform_int_distribution<int> dist(int('0'), int('9'));
    return dist(rng);
  };
  auto getRandomCapital = [this]() {
    std::uniform_int_distribution<int> dist(int('A'), int('Z'));
    return dist(rng);
  };
  auto getRandomSmall = [this]() {
    std::uniform_int_distribution<int> dist(int('a'), int('z'));
    return dist(rng);
  };

  std::vector<std::function<char()>> char_generators;
  if (numbers) char_generators.push_back(getRandomNumber);
  if (special_characters) char_generators.push_back(getRandomSymbol);
  if (capital) char_generators.push_back(getRandomCapital);
  if (small) char_generators.push_back(getRandomSmall);

  std::uniform_int_distribution<int> dist(0, char_generators.size() - 1);
  for (size_t i = 0; i < length; i++) {
    password.push_back(char_generators[dist(rng)]());
  }
  return password;
}