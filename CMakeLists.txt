cmake_minimum_required(VERSION 4.2)
project(course_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(course_project main.cpp
        chord.cpp
        chord.h
        genreRuleset.cpp
        genreRuleset.h
        harmonyEngine.cpp
        harmonyEngine.h)
