cmake_minimum_required(VERSION 3.10)
project(SICPI VERSION 1.0)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options(-pedantic -pedantic-errors -Wall -Wextra -Wdouble-promotion -Wold-style-definition -Wno-psabi)

set(SICPILIB_FILES
    src/args.c
    src/character-classes.c
    src/dialect.c
    src/explicit-control-evaluator.c
    src/hardware.c
    src/lexer.c
    src/list.c
    src/metacircular-procedures.c
    src/obarray.c
    src/object.c
    src/parser.c
    src/primitive-procedures.c
    src/run.c
    src/sicp-error.c
    src/sicp-std.c
    src/source.c
    src/string-builder.c
    src/token.c
    src/token-table.c
    src/table-token-source.c
)

add_library(sicplib STATIC ${SICPILIB_FILES})

add_executable(sicp src/sicp.c)

# Link against the math library
target_link_libraries(sicp sicplib m)

# Enable testing
#enable_testing()

# Assume there's a CMakeLists.txt in 'tests' directory that defines the 'test' target
add_subdirectory(tests)
