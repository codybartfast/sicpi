
cmake_minimum_required (VERSION 3.8)

project (sicp-scheme)

# set(C_FLAGS "-lm -O3 -std=c17 -pedantic -pedantic-errors -Wall -Wextra -Wdouble-promotion -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition")
set(C_FLAGS "-lm -O3 -std=c17 -pedantic -pedantic-errors -Wall -Wextra -Wdouble-promotion -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition")

set (SRC_CORE
	)

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_executable (sicp ${SRC_CORE} src/sicp.c)

target_link_libraries(sicp m)
