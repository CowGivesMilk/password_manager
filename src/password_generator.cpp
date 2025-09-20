#include "password_generator.hpp"

#include <algorithm>
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

size_t PasswordGenerator::min_length() const noexcept {
  return static_cast<size_t>(numbers + special_characters + small + capital);
}

std::string PasswordGenerator::generatePassword(size_t length) const noexcept {
  if (length > 999) {
    std::println(
        "You don't need that long password.\nReturning 256 character long "
        "password instead!");
    length = 256;
  }
  if (length < this->min_length()) {
    throw std::invalid_argument(
        "length < numbers + special_characters + capital + small");
  }
  std::string password;
  password.reserve(length);

  auto getRandomSymbol = [this]() {
    int lower = 0, upper = PasswordGenerator::symbols.size() - 1;
    std::uniform_int_distribution<size_t> dist(lower, upper);
    return static_cast<char>(symbols[dist(rng)]);
  };
  auto getRandomNumber = [this]() {
    std::uniform_int_distribution<int> dist(int('0'), int('9'));
    return static_cast<char>(dist(rng));
  };
  auto getRandomCapital = [this]() {
    std::uniform_int_distribution<int> dist(int('A'), int('Z'));
    return static_cast<char>(dist(rng));
  };
  auto getRandomSmall = [this]() {
    std::uniform_int_distribution<int> dist(int('a'), int('z'));
    return static_cast<char>(dist(rng));
  };

  std::vector<std::function<char()>> char_generators;
  if (numbers) {
    char_generators.push_back(getRandomNumber);
    password.push_back(getRandomNumber());
  }
  if (special_characters) {
    char_generators.push_back(getRandomSymbol);
    password.push_back(getRandomSymbol());
  }
  if (capital) {
    char_generators.push_back(getRandomCapital);
    password.push_back(getRandomCapital());
  }
  if (small) {
    char_generators.push_back(getRandomSmall);
    password.push_back(getRandomSmall());
  }

  std::uniform_int_distribution<int> dist(0, char_generators.size() - 1);

  for (size_t i = this->min_length(); i < length; i++) {
    password.push_back(char_generators[dist(rng)]());
  }
  std::shuffle(password.begin(), password.end(), rng);
  return password;
}