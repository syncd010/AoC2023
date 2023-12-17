#include <vector>
#include <variant>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <gtest/gtest.h>

#include "aoc.h"

namespace aoc_tests {
using namespace std;

vector<tuple<string, Result, Result>> testFiles{
    {"./data/input1Test", 209, 281},
    {"./data/input2Test", 8, 2286},
    {"./data/input3Test", 4361, 467835},
    {"./data/input4Test", 13, 30},
    {"./data/input5Test", 35, 46},
    {"./data/input6Test", 288, 71503},
    {"./data/input7Test", 6440, 5905},
    {"./data/input8Test", monostate(), 6},
    {"./data/input9Test", 114, 2},
    {"./data/input10Test", 80, 10},
    {"./data/input11Test", 374, 82000210},
    {"./data/input12Test", 21, 525152},
    {"./data/input13Test", 405, 400},
    {"./data/input14Test", 136, 64},
    {"./data/input15Test", 1320, 145},
    {"./data/input16Test", 46, 51},
    {"./data/input17Test", 102, 94},
};

vector<tuple<string, Result, Result>> dataFiles{
    {"./data/input1", 53921, 54676},
    {"./data/input2", 2795, 75561},
    {"./data/input3", 532445, 79842967},
    {"./data/input4", 26443, 6284877},
    {"./data/input5", 323142486, 79874951},
    {"./data/input6", 2612736, 29891250},
    {"./data/input7", 248217452, 245576185},
    {"./data/input8", 17263, 14631604759649},
    {"./data/input9", 1743490457, 1053},
    {"./data/input10", 6725, 383},
    {"./data/input11", 9627977, 644248339497},
    {"./data/input12", 6488, 815364548481},
    {"./data/input13", 35210, 31974},
    {"./data/input14", 108918, 100310},
    {"./data/input15", 507291, 296921},
    {"./data/input16", 8034, 8225},
    {"./data/input17", 722, 894},
};

string readFile(const string &filePath) {
    ifstream fs(filePath);
    if (!fs.is_open()) {
        cout << "Error: cannot open input file: " + filePath + "\n";
        return string{};
    }
    // Read whole file
    stringstream buffer;
    buffer << fs.rdbuf();
    fs.close();
    return buffer.str();
}

#define AOC_TEST_TEST_FILES(macro_day)                          \
    TEST(AoCTest, TestFilesDay##macro_day) {                    \
        int idx = stoi(#macro_day) - 1;                         \
        string input = readFile(get<0>(testFiles[idx]));        \
        ASSERT_TRUE(!input.empty());                            \
        Result part1 = aoc##macro_day::solvePartOne(input);     \
        Result part2 = aoc##macro_day::solvePartTwo(input);     \
        EXPECT_EQ(part1, get<1>(testFiles[idx]));               \
        EXPECT_EQ(part2, get<2>(testFiles[idx]));               \
    }

#define AOC_TEST_DATA_FILES(macro_day)                          \
    TEST(AoCTest, DataFilesDay##macro_day) {                    \
        int idx = stoi(#macro_day) - 1;                         \
        string input = readFile(get<0>(dataFiles[idx]));        \
        ASSERT_TRUE(!input.empty());                            \
        Result part1 = aoc##macro_day::solvePartOne(input);     \
        Result part2 = aoc##macro_day::solvePartTwo(input);     \
        EXPECT_EQ(part1, get<1>(dataFiles[idx]));               \
        EXPECT_EQ(part2, get<2>(dataFiles[idx]));               \
    }


REPEAT_FOR_DAY(AOC_TEST_TEST_FILES)
REPEAT_FOR_DAY(AOC_TEST_DATA_FILES)

}
