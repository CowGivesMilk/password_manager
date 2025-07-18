#include "password_generator.hpp"

#include <array>
#include <experimental/random>
#include <iostream>

PasswordGenerator::PasswordGenerator() {
  special_characters = true;
  small = true;
  capital = true;
  numbers = true;
}

char PasswordGenerator::getRandomSymbol() {
  int lower = 0, upper = this->symbols.size() - 1;
  return symbols[std::experimental::randint(lower, upper)];
}
char PasswordGenerator::getRandomNumber() {
  return std::experimental::randint(int('0'), int('9'));
}
char PasswordGenerator::getRandomCapital() {
  char res = std::experimental::randint(int('A'), int('Z'));
  return res;
}
char PasswordGenerator::getRandomSmall() {
  char res = std::experimental::randint(int('a'), int('z'));
  return res;
}

std::string PasswordGenerator::generatePassword(size_t length) {
  std::string password;
  password.reserve(length);
  while (length != password.length()) {
    int option = std::experimental::randint(0, 3);
    switch (option) {
      case 0:
        password.push_back(getRandomSymbol());
        break;
      case 1:
        password.push_back(getRandomSmall());
        break;
      case 2:
        password.push_back(getRandomCapital());
        break;
      case 3:
        password.push_back(getRandomNumber());
        break;
    }
  }
  return password;
}