cmake_minimum_required(VERSION 3.0)

project(caps2esc)

add_executable(caps2esc caps2esc.c)
target_compile_options(caps2esc PRIVATE -Wall -Wextra)

install(TARGETS caps2esc RUNTIME DESTINATION bin)
