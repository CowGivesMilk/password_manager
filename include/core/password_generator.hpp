#ifndef PASSWORD_GENERATOR_H
#define PASSWORD_GENERATOR_H

#include <random>
#include <string>

class PasswordGenerator {
 private:
  bool numbers;
  bool small;
  bool capital;
  bool special_characters;
  static constexpr std::string_view symbols =
      R"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)";
  mutable std::mt19937 rng;

  size_t min_length() const noexcept;

 public:
  PasswordGenerator();
  PasswordGenerator(bool numbers, bool small, bool capital,
                    bool special_characters);
  [[nodiscard]] std::string generatePassword(size_t length) const;
};

#endif
