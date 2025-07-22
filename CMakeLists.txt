cmake_minimum_required(VERSION 3.16)
project(password_manager LANGUAGES CXX)

# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Strict warnings
function(apply_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

# Include directory
include_directories(include)

# ========== Source Files ==========
set(SOURCES
    src/password_generator.cpp
    src/file_handler.cpp
    src/helper.cpp
)

set(HEADERS
    include/password_generator.hpp
    include/file_handler.hpp
    include/helper.hpp
)

# ========== Core Static Library ==========
add_library(core_lib STATIC ${SOURCES} ${HEADERS})
target_include_directories(core_lib PUBLIC include)
apply_warnings(core_lib)

# ========== Main Executable ==========
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/data/password.csv" CONTENT "")
add_executable(password_manager main.cpp)
target_link_libraries(password_manager PRIVATE core_lib)
apply_warnings(password_manager)

# ========== Testing ==========
# Requires: sudo pacman -S catch2 or manually install Catch2 v3
find_package(Catch2 3 REQUIRED)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/data")

# Generate the test CSV file
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/data/test_password.csv" CONTENT "")
set(TEST_SOURCES
    tests/password_generator.cpp
    tests/file_handler.cpp
    tests/helper.cpp
)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE core_lib Catch2::Catch2WithMain)
apply_warnings(tests)

include(CTest)
include(Catch)
catch_discover_tests(tests)