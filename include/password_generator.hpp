#ifndef PASSWORD_GENERATOR_H
#define PASSWORD_GENERATOR_H

#include <array>
#include <experimental/random>
#include <iostream>
#include <string>

class PasswordGenerator {
 private:
  bool special_characters;
  bool small;
  bool capital;
  bool numbers;
  const std::array<char, 32> symbols = {
      '!', '"',  '#', '$', '%', '&', '\'', '(', ')', '*', '+',
      ',', '-',  '.', '/', ':', ';', '<',  '=', '>', '?', '@',
      '[', '\\', ']', '^', '_', '`', '{',  '|', '}', '~'};

  char getRandomNumber();
  char getRandomSmall();
  char getRandomCapital();
  char getRandomSymbol();

 public:
  PasswordGenerator();
  std::string generatePassword(size_t length);
};

#endif