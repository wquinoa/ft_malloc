cmake_minimum_required(VERSION 3.17)
project(untitled2 C)

set(CMAKE_C_STANDARD 11)

add_library(untitled2 SHARED libmalloc.c main.c libmalloc.h)