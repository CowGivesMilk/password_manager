cmake_minimum_required(VERSION 4.0)  
project(password_manager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Catch2 3 REQUIRED) # FOR TESTING
set(SOURCES
    src/password_generator.cpp
    src/input_validator.cpp
)

set(HEADERS
    include/password_generator.hpp
    include/input_validator.hpp
)
set(TESTS
    tests/password_generator.cpp
)

# MAKING STATIC LIBRARY NAMED AS core_lib 
add_library(core_lib STATIC ${SOURCES} ${HEADERS})
target_include_directories(core_lib PUBLIC include)

# MAKING MAIN EXECUTABLE 
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE core_lib)


###########  TESTING   #################
# These tests can use the Catch2-provided main
add_executable(tests ${TESTS})
target_link_libraries(tests PRIVATE core_lib Catch2::Catch2WithMain)


# These tests need their own main
#add_executable(custom-main-tests test.cpp test-main.cpp)
#target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)

########################################################

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE 
        /W4     # High warning level
        /WX     # Treat warnings as errors
        /permissive-  # Standards conformance
    )
else()
    # GCC/Clang flags
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -Wall
        -Wextra
        -pedantic
        -Werror  # Treat warnings as errors
    )
endif()