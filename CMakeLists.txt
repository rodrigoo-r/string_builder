cmake_minimum_required(VERSION 3.30)
project(string_builder C)

set(CMAKE_C_STANDARD 11)

add_library(string_builder STATIC string_builder.c)
