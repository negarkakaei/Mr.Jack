cmake_minimum_required(VERSION 3.20)
project(game C)

set(CMAKE_C_STANDARD 99)

add_executable(game main.c basic.c basic.h map.c map.h list.c list.h action.c action.h)
