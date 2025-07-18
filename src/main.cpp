#include <iostream>

#include "input_validator.hpp"
#include "password_generator.hpp"

int main(int argc, char** argv) {
  PasswordGenerator obj;
  size_t length;

  if (argc == 1) {
    std::string input;
    std::cout << "Length: ";
    std::getline(std::cin, input);
    if (!parse_size_t(input, length)) {
      std::cerr << "Please input a valid positive integer" << std::endl;
      return 1;
    }
  } else {
    if (!parse_size_t(argv[1], length)) {
      std::cerr << "Please input a valid positive integer as arg[1]"
                << std::endl;
      return 1;
    }
  }

  std::cout << obj.generatePassword(length) << std::endl;

  return 0;
}