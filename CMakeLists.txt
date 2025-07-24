cmake_minimum_required(VERSION 3.16)
project(password_manager LANGUAGES CXX)

# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Strict warning function
function(apply_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

# ================== Source & Header Files ==================
set(SOURCES
    src/password_generator.cpp
    src/file_handler.cpp
    src/helper.cpp
    src/enc_dec.cpp
)

set(HEADERS
    include/password_generator.hpp
    include/file_handler.hpp
    include/helper.hpp
    include/enc_dec.hpp
)

# ================== Core Static Library ==================
add_library(core_lib STATIC ${SOURCES} ${HEADERS})
target_include_directories(core_lib PUBLIC include)

# Link Crypto++ manually
find_library(CRYPTOPP_LIB NAMES cryptopp REQUIRED)
target_link_libraries(core_lib PUBLIC ${CRYPTOPP_LIB})

apply_warnings(core_lib)

# ================== Main Executable ==================
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/data/password.csv" CONTENT "")
add_executable(password_manager main.cpp)
target_link_libraries(password_manager PRIVATE core_lib)
apply_warnings(password_manager)

# ================== Unit Tests ==================
# Make sure Catch2 v3 is installed: sudo pacman -S catch2
find_package(Catch2 3 REQUIRED)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/data")
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/data/test_password.csv" CONTENT "")

set(TEST_SOURCES
    tests/password_generator.cpp
    tests/file_handler.cpp
    tests/helper.cpp
    tests/enc_dec.cpp
)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE core_lib Catch2::Catch2WithMain)
apply_warnings(tests)

include(CTest)
include(Catch)
catch_discover_tests(tests)

# ================== Optional: Install Rules ==================
# install(TARGETS password_manager RUNTIME DESTINATION bin)
# install(TARGETS core_lib ARCHIVE DESTINATION lib)
# install(DIRECTORY include/ DESTINATION include)