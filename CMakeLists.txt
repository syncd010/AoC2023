cmake_minimum_required(VERSION 3.10)

# For CLang
# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fexperimental-library")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

project(AdventOfCode2023 VERSION 1.0)

add_executable(aoc)

target_compile_features(aoc PUBLIC cxx_std_20)

target_sources(aoc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/aoc.cpp
)

set(AOC_DAYS_LIST "1;2;3;4;5;6;7;8")

foreach(DAY ${AOC_DAYS_LIST})
    target_sources(aoc PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/day${DAY}.cpp
    )
endforeach()


