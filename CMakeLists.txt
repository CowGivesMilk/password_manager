cmake_minimum_required(VERSION 3.16)
project(password_manager LANGUAGES CXX)

# -------------------- C++ Standard --------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------- Strict Warnings --------------------
function(apply_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

# -------------------- Source & Header Files --------------------
set(SOURCES
    src/password_generator.cpp
    src/file_handler.cpp
    src/helper.cpp
    src/enc_dec.cpp
    src/data_handler.cpp
)

set(HEADERS
    include/password_generator.hpp
    include/file_handler.hpp
    include/helper.hpp
    include/enc_dec.hpp
    include/data_handler.hpp
)

# -------------------- Core Static Library --------------------
add_library(core_lib STATIC ${SOURCES} ${HEADERS})
target_include_directories(core_lib PUBLIC include)

# -------------------- Find & Link Dependencies --------------------
# Crypto++
find_library(CRYPTOPP_LIB NAMES cryptopp REQUIRED)
target_link_libraries(core_lib PUBLIC ${CRYPTOPP_LIB})

# nlohmann-json (header-only)
find_package(nlohmann_json REQUIRED)
target_link_libraries(core_lib PUBLIC nlohmann_json::nlohmann_json)

apply_warnings(core_lib)

# -------------------- Main Executable --------------------
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/data/password.csv" CONTENT "")
add_executable(password_manager main.cpp)
target_link_libraries(password_manager PRIVATE core_lib)
apply_warnings(password_manager)

# -------------------- Unit Tests --------------------
find_package(Catch2 3 REQUIRED)

set(TEST_SOURCES
    tests/password_generator.cpp
    tests/file_handler.cpp
    tests/helper.cpp
    tests/enc_dec.cpp
    tests/data_handler.cpp
)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE core_lib Catch2::Catch2WithMain)
apply_warnings(tests)

include(CTest)
include(Catch)
catch_discover_tests(tests)
