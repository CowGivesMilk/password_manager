#include <iostream>

#include "file_handler.hpp"
#include "helper.hpp"
#include "input_validator.hpp"
#include "password_generator.hpp"

int main(int argc, char** argv) {
  PasswordGenerator obj;
  size_t length;

  std::string file_path = Helper::get_db_path(argv[0]);  // default

  if (argc == 1) {
    std::string input;
    std::cout << "Length: ";
    std::getline(std::cin, input);
    if (!parse_size_t(input, length)) {
      std::cerr << "Please input a valid positive integer" << std::endl;
      return 1;
    }
  } else if (argc >= 2) {
    if (!parse_size_t(argv[1], length)) {
      std::cerr << "Please input a valid positive integer as arg[1]"
                << std::endl;
      return 1;
    }

    if (argc >= 3) {
      file_path = argv[2];  // use user-provided path
    }
  }

  std::string username = "nimes", password = obj.generatePassword(length),
              site = "https://www.archlinux.org";
  Entry entry(username, password, site);

  FileHandler database(file_path);
  if (database.add_entry(entry))
    std::cout << "Successfully added entry\n";
  else
    std::cout << "Failed to add entry\n";

  return 0;
}
