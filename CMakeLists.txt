cmake_minimum_required(VERSION 3.25)
project(Progetto_API C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address ")

add_executable(Progetto_API main.c)
