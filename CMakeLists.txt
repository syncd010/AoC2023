cmake_minimum_required(VERSION 3.14)

# For CLang
# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fexperimental-library")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

set(AOC_DAYS_LIST "1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18")

project(AdventOfCode2023 VERSION 1.0)

add_executable(aoc)

target_compile_features(aoc PUBLIC cxx_std_23)

target_sources(aoc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/aoc.cpp
)

foreach(DAY ${AOC_DAYS_LIST})
    target_sources(aoc PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/day${DAY}.cpp
    )
endforeach()

# Google tests
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(aoc_tests)

target_compile_features(aoc_tests PUBLIC cxx_std_20)

target_sources(aoc_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/aoc_tests.cpp
)

foreach(DAY ${AOC_DAYS_LIST})
    target_sources(aoc_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/day${DAY}.cpp
    )
endforeach()

target_link_libraries(
  aoc_tests
  GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(aoc_tests)
