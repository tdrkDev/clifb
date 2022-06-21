cmake_minimum_required(VERSION 3.10)
project(clifb VERSION 0.0.1)

# Main library
set(CURSES_NEED_NCURSES true)
set(CURSES_NEED_WIDE true)
find_package(Curses REQUIRED)
include_directories(
    inc
    ${CURSES_INCLUDE_DIR}
)
add_library(clifb 
    src/clifb.c
    src/symbol_fb.c
    src/mono_doubled_fb.c
    inc/clifb.h
)
add_compile_definitions(NCURSES_WIDECHAR=1)
target_link_libraries(clifb
    ${CURSES_LIBRARY}
    ${ADDITIONAL_LIBRARIES}
)

# Test
add_executable(clifb_test src/tests/main.c)
add_dependencies(clifb_test clifb)
target_link_libraries(clifb_test clifb)
