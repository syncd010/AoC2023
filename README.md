# Advent of Code 2023 in C++

My solutions for the [Advent of Code 2023](https://adventofcode.com/2023) in C++. Target version is C++20, so that solutions can compile with current versions of gcc and clang (as of late 2023) and also with their respective implementations of the Standard Library. CMake is the build system.

As usual, there's very little or no error handling so the solutions blow up if the input files aren't exactly as expected and specified in each day's puzzle description.

Code is in `src/`, input files are in `data/`, CMake outputs to `build/`.

## Build
On the root dir, make sure that `build/` exists and is empty, and:

> `cmake -S . -B build/`
>
> `cmake --build build/`


Output is `build/aoc`.

## Usage
To run, from the root dir:
> `./build/aoc -d $day [-t] [$file]`

This will run the 2 parts of the specified `$day`, using `$file` as input. If no file is specified `./data/input{$day}` is used as input. If no file is specified and `-t` is used, `./data/input{$day}Test` is used instead. 

## Post event impressions

---

## [Day 1](https://adventofcode.com/2023/day/1)
Simple warm up. Part two is a hack, directly substituting the names of the digits in the string. Need to be careful because the names can share characters (oneigth should be 18) but it works because at most one character is shared between them.

## [Day 2](https://adventofcode.com/2023/day/2)
These warmups are more challenging than previous years. The parsing is contrived, particularly because there are multiple layers of vectors involved.
Anyway, still getting the hang of using C++ ranges, and today also fighting with regular expressions. The regex API doesn't seem as user-friendly as others.

## [Day 3](https://adventofcode.com/2023/day/3)
Clearly a Sunday, as this was a hard puzzle for day 3. 
I went for a straightforward solution, representing the input as a 2d array and manually looping over it. For each cell that has a relevant character, check its surrounding neighbors and, if any has a digit, get the enclosing number by going backwards and forwards from the cell.
With this setup part two is relatively straightforward just requiring some simple hard coded adaptions.

## [Day 4](https://adventofcode.com/2023/day/4)
Relatively easy day. Part one core is the intersection of the 2 sets of numbers on each card. Could/should have been done with sets but i computed it manually. Part two is also straightforward, just propagate the number of winners on each card to the following n cards and sum.

## [Day 5](https://adventofcode.com/2023/day/5)
First part is relatively easy, apart from parsing the input, which as usual took some time to get right (that's punishment for insisting on C++ for this). It's basically passing each seed through the maps sequentially, and converting it accordingly.
Part two is much more challenging, and i got to compliment the authors for, given the same input and general structure, with a simple change of the rules devising a completely different problem (and much harder). The general structure is to identify the overlaps on two ranges of numbers and convert each segment individually, keeping the non overlapping in a queue for posterior conversion. Needs attention to the way the looping is done (first through the maps, then through seeds, which is different from what was in part one).

## [Day 6](https://adventofcode.com/2023/day/6)
An easy day, with a bit of high school math. The distance traveled is given by the function d(t) = t(T - t), where T is the race time, so we want all solutions for d(t) > D, where D is the given distance. This is a quadratic equation and solving it gives the interval (min_t, max_t) where the traveled distance is greater than D. To get the integer values on that interval calculate ceil(max_t) - floor(min_t) - 1.

## [Day 7](https://adventofcode.com/2023/day/7)
Today wasn't challenging, but it took some work. 
For part one the relevant issue is to sort the hands according to the rules, so we need to calculate:
- A score for the type of hands, which is done by replacing each card in the hand with the number of times that card appears in the hand and summing it (so that a five of a kind becomes 25, a four of a kind becomes 17, etc - this creates a correct order on the types);
- For hands of the same type, substitute `AKQJT` by chars in lexicographic order (`edcba`), so that a string comparison between 2 hands of the same type orders them correctly;
For part two we need to deal with jokers when determining the hand type. Jokers are substituted by other cards already in the hand, creating new hands without the jokers which are scored, and the maximum type score is used.

## [Day 8](https://adventofcode.com/2023/day/8)
Not a hard day, but it had a twist in part two.
The parsed representation of the input is: 
1) a string of the path, each character considered individually;
2) a map where the keys are nodes (as strings) and the values are pairs of the left/right nodes to follow when traversing the graph.
Given this, part one is a relatively simple tree/graph traversal, starting on the "AAA" node and following the given path until the end node is reached. I included a simple direct loop check, so that the test input from part two didn't hang up here.
For part two there should be multiple start nodes, all those that end with 'A'. My first solution was a direct one, trying to advance from the start nodes step by step following the path, until all of them were end nodes. This (predictably) worked on the test data, but not on the real input data, which become apparent after about 30s of runtime. Taking a second look at the test data, and given previous AoC experiences, it was clear that there should be loops in the paths followed by the different start nodes, and that the solution would be the least common multiple of the length of the different loops. Making a general version of this would take some work (identifying all loops (making sure that they were real loops, considering the path), and account for start path segments not part of the loop), but i first tried a simple solution of taking the initial length of each path until the end node and get the LCM of those lengths - lots of assumptions on this, but thankfully the input was precisely built for this, and i got the right result.
The end result is very large, so i still had to convert to int64_t to avoid overflows.

## [Day 9](https://adventofcode.com/2023/day/9)
One of the most straightforward days, guess it's the calm before the storm.
Not much to explain, need to calculate the differences until all 0 (constant) and sum up the last difference to get the next value - high school calculus. For part two the most direct way is to reverse the input numbers and do the same computation.

## [Day 10](https://adventofcode.com/2023/day/10)
Todo later, too much to say

## [Day 11](https://adventofcode.com/2023/day/11)
Relatively straightforward and fun day, though care must be taken to correctly expand the space.
The input is scanned to get a list of the empty rows and columns that will be expanded and a list of the positions that are filled. Each position is then expanded by a factor proportional to how many empty rows/columns are before it in the space (using different factors for part one and two). Finally the Manhattan distance is calculated for each unique pair of positions.

## [Day 12](https://adventofcode.com/2023/day/12)
Todo later, too much to say

## [Day 13](https://adventofcode.com/2023/day/13)
Much simpler than yesterday. There's no major insight on the solution, just a straightforward search for reflected rows. Searching on columns is done by first transposing the input and then searching on the transposed rows.
Part two introduces some noise on the solution, but the structure is the same. The difference is that, when a line is not perfectly reflected, it checks to see if they differ by only one position, and if so, allow the reflection test to continue. Need to be careful with the conditions because only one differing line can be found.


## [Day 14](https://adventofcode.com/2023/day/14)

## [Day 15](https://adventofcode.com/2023/day/15)

## [Day 16](https://adventofcode.com/2023/day/16)

## [Day 17](https://adventofcode.com/2023/day/17)

## [Day 18](https://adventofcode.com/2023/day/18)

## [Day 19](https://adventofcode.com/2023/day/19)

## [Day 20](https://adventofcode.com/2023/day/20)

## [Day 21](https://adventofcode.com/2023/day/21)

## [Day 22](https://adventofcode.com/2023/day/22)

## [Day 23](https://adventofcode.com/2023/day/23)

## [Day 24](https://adventofcode.com/2023/day/24)

## [Day 25](https://adventofcode.com/2023/day/25)
