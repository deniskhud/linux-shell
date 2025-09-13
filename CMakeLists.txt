cmake_minimum_required(VERSION 3.31)
project(shell_c C)

set(CMAKE_C_STANDARD 11)

add_executable(shell_c main.c
        builtins.h
        builtins.c
        shell.h
        shell.c)
target_link_libraries(shell_c PRIVATE readline)