cmake_minimum_required(VERSION 3.10)
project(SICP VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

# Compiler flags
add_compile_options(-pedantic -pedantic-errors -Wall -Wextra -Wdouble-promotion -Wold-style-definition -Wno-psabi)

# Source files
set(SOURCE_FILES
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
    src/sicp.c
    src/sicp-error.c
    src/sicp-std.c
    src/source.c
    src/string-builder.c
    src/token.c
    src/token-table.c
    src/table-token-source.c
)

# Add executable
add_executable(sicp ${SOURCE_FILES})

# Link against the math library
target_link_libraries(sicp m)

# Enable testing
# enable_testing()

# Assume there's a CMakeLists.txt in 'tests' directory that defines the 'test' target
# add_subdirectory(tests)


