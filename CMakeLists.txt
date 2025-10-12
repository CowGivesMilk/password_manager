cmake_minimum_required(VERSION 3.16)
project(passman LANGUAGES CXX)

# -------------------- C++ Standard --------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Enable Qt's automatic MOC, UIC, and RCC for all targets
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# -------------------- Dependencies --------------------
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(nlohmann_json REQUIRED)
find_package(Catch2 3 REQUIRED)
find_library(CRYPTOPP_LIB NAMES cryptopp REQUIRED)
# -------------------- Strict Warnings --------------------
function(apply_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

# -------------------- Core Library --------------------
set(CORE_SOURCES
    src/core/password_generator.cpp
    src/core/file_handler.cpp
    src/core/helper.cpp
    src/core/enc_dec.cpp
    src/core/data_handler.cpp
)

set(CORE_HEADERS
    include/core/password_generator.hpp
    include/core/file_handler.hpp
    include/core/helper.hpp
    include/core/enc_dec.hpp
    include/core/data_handler.hpp
)

add_library(core_lib STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_include_directories(core_lib PUBLIC include)


target_link_libraries(core_lib
    PUBLIC
        nlohmann_json::nlohmann_json
    PRIVATE
        ${CRYPTOPP_LIB}
)

# apply_warnings(core_lib)

# -------------------- GUI Library --------------------
set(GUI_SOURCES
    src/gui/test_window.cpp
    src/gui/test_window.ui
)

set(GUI_HEADERS
    include/gui/test_window.hpp
)
add_library(gui_lib STATIC ${GUI_SOURCES} ${GUI_HEADERS})
target_include_directories(gui_lib PUBLIC include)
target_link_libraries(gui_lib
    PUBLIC
        Qt6::Core
        Qt6::Widgets
        core_lib
)

# apply_warnings(gui_lib)

# -------------------- Main Executable (GUI) --------------------
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        gui_lib
)

# apply_warnings(${PROJECT_NAME})

# -------------------- Unit Tests --------------------
set(TEST_SOURCES
    tests/password_generator.cpp
    tests/file_handler.cpp
    tests/helper.cpp
    tests/enc_dec.cpp
    tests/data_handler.cpp
)

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests
    PRIVATE
        core_lib
        Catch2::Catch2WithMain
)

# apply_warnings(tests)

include(CTest)
include(Catch)
catch_discover_tests(tests)
