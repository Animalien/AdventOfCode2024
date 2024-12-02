cmake_minimum_required(VERSION 3.31.1)

project(AdventOfCode2024)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(AdventOfCode2024
	AdventOfCode2024.cpp
	AdventOfCode2024.h
	Problem1.cpp
	_clang-format
)

