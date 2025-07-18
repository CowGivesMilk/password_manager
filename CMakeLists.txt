cmake_minimum_required(VERSION 4.0)  
project(password_manager)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)
set(SOURCES
    main.cpp
    src/password_generator.cpp
    src/input_validator.cpp
)

set(HEADERS
    include/password_generator.hpp
    include/input_validator.hpp
)

# Create executable FIRST before setting properties
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Now set target-specific properties AFTER creating the target
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

