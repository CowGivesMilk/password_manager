#ifndef PASSWORD_GENERATOR_H
#define PASSWORD_GENERATOR_H

#include <array>
#include <random>
#include <string>

class PasswordGenerator {
 private:
  bool numbers;
  bool small;
  bool capital;
  bool special_characters;
  const std::array<char, 32> symbols = {
      '!', '"',  '#', '$', '%', '&', '\'', '(', ')', '*', '+',
      ',', '-',  '.', '/', ':', ';', '<',  '=', '>', '?', '@',
      '[', '\\', ']', '^', '_', '`', '{',  '|', '}', '~'};
  std::mt19937 rng;

  char getRandomNumber();
  char getRandomSmall();
  char getRandomCapital();
  char getRandomSymbol();

 public:
  PasswordGenerator();
  PasswordGenerator(bool numbers, bool small, bool capital,
                    bool special_characters);
  std::string generatePassword(size_t length);
};

#endif