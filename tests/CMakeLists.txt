cmake_minimum_required(VERSION 3.10)
project(SICPI_Tests)

# Specify the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

# Compiler flags
add_definitions(-DUNITY_INCLUDE_DOUBLE)
# add_compile_options(-pedantic -pedantic-errors -Wall -Wextra -Wold-style-definition -Wno-psabi)

# Include directories for the Unity test framework and the main project headers
#include_directories(${PROJECT_SOURCE_DIR}/unity/src ${PROJECT_SOURCE_DIR}/src)

# Source files and test files
set(TEST_SOURCE_FILES
    ../unity/src/unity.c
    ../tests/test.c
    ../tests/test-explicit-control-evaluator.c
    ../tests/test-lexer.c
    ../tests/test-list.c
    ../tests/test-obarray.c
    ../tests/test-object.c
    ../tests/test-parser.c
    ../tests/test-primitive-procedures.c
    ../tests/test-source.c
    ../tests/test-string-builder.c
    ../tests/test-table-token-source.c
    ../tests/test-token-table.c
)

# Add the test executable
add_executable(unity_tests ${TEST_SOURCE_FILES})

# Link against the math library
target_link_libraries(unity_tests PRIVATE sicplib m)

