#!/bin/sh

if [ -z "$1" ]
then
    echo "Please provide a day number"
    exit
fi

DAY="day$1"

if [ -f "./src/day$1.cpp" ]
then
    echo "Day $1 already exists"
else
    cp "./src/day_template.cpp.in" "./src/day$1.cpp"
    sed -i "s/namespace aocDAY/namespace aoc$1/" "./src/day$1.cpp"
    sed -i "s+  // Additional days here+  macro($1)  \\\\\n  // Additional days here+" "./src/aoc.h"
    sed -i -E "s/set\(AOC_DAYS_LIST \"(.+)\"\)/set\(AOC_DAYS_LIST \"\1;$1\"\)/" "./CMakeLists.txt"
    rm -rf ./build/*
    cmake -S . -B build/
    touch "./data/input$1"
    touch "./data/input$1Test"
    echo "Day $1 created"
fi
