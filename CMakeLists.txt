
cmake_minimum_required (VERSION 3.8)

project (sicp)

set(C_FLAGS "-lm -O3 -std=c2x -pedantic -pedantic-errors -Wall -Wextra -Wdouble-promotion -Wold-style-definition")

set (SRC_CORE
	src/string-builder.c
	)

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_executable (sicp ${SRC_CORE} src/sicp.c)

target_link_libraries(sicp m)
