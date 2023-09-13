cmake_minimum_required(VERSION 3.26)
project(Source_Code C)

set(CMAKE_C_STANDARD 23)

add_executable(Source_Code operations.c
        serial_impl.c
        operations.h
        random_array.c
        random_array.h
        parallel_with_one_mutex.c
        Parallel_with_rwlock.c
        main.c
        serial_impl.h
        parallel_with_one_mutex.h)
