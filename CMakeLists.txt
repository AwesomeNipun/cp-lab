cmake_minimum_required(VERSION 3.26)
project(Source_Code C)

set(CMAKE_C_STANDARD 23)

add_executable(Source_Code operations.c
        serial_impl.c
        operations.h
        random_array.c
        random_array.h)
