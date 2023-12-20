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
As expected, a lot of work on Sundays. This type of problems is sometimes suited for complex math, but given that in C++ a solution using `Complex` would be very similar to using a 2D vector to model positions and directions i choose the later as it is more readable. Some relevant notes on the solution:
- A map of the exits of each type of cell is created and used to navigate the maze;
- To find the biggest loop, start from the initial cell, follow the 4 possible directions and check if it returns to the initial cell;
- To navigate the maze, follow a specific direction and use the mentioned map of exits for each cell type to move from cell to cell. Need to handle dead ends and cells that don't communicate with each other (their entry/exit directions aren't consisted, using the exits map to check that);
- For part two, start by creating a new maze with just the main loop and deduce the type of the initial cell by checking its neighbors and making sure it is consistent with their entry/exit directions. The type of all cells must be know for the following step;
- To count inside cells, loop on each line keeping an inside/outside indicator. When a "wall" is found, invert th indicator and count the cells when inside is active. Only invert the indicator on '|' cell types and one of the corner pairs ['F', '7'] or ['L', 'J'] otherwise a double inversion of the indicator could occur.

## [Day 11](https://adventofcode.com/2023/day/11)
Relatively straightforward and fun day, though care must be taken to correctly expand the space.
The input is scanned to get a list of the empty rows and columns that will be expanded and a list of the positions that are filled. Each position is then expanded by a factor proportional to how many empty rows/columns are before it in the space (using different factors for part one and two). Finally the Manhattan distance is calculated for each unique pair of positions.

## [Day 12](https://adventofcode.com/2023/day/12)
Yet another challenging day, so soon after Sunday. An error correcting code that can be understood as a constraint satisfaction problem, the records being the state and the groups the constraints. Problem is, i couldn't devise any rules for constraint propagation, so what's left is trying possible solutions, checking if they are valid and backtracking if not. A dynamic programming problem, with the potential for exponential runtime (and/or memory usage).
Two possible approaches are available: 1) starting from the records, try the different possibilities for each '?' and check if the group constraints are satisfied or 2) starting from the group constraints, try to place them sequentially where possible and check if they are consistent with the record. The second approach seemed more efficient in time and space so that was what i followed, hoping that it could be used directly for part two of the day.
The solution is therefore a recursion on the records/groups, placing one group on each iteration on available space of the record. Recurse until the end of the record is reached or the final group is placed and increase the combinations if successful. On the last group a consistency check needs to be made - whether there are any remaining '#' signs on the record indicating that there should be more groups placed.
For part two, as expected, the input grows. At first i tried to brute force the solution on the expanded input but it was fruitlessly. I then tried to approach the problem from different angles, but couldn't find any other workable solution, until the obvious and simplest one presented itself: memoization of previous steps. It should have been obvious from the start, but it took me an embarrassingly long time to recognize it... A cache with the key being the current position on the record and the current group (hashed via a Cantor Pairing function) and the value being the number of combinations found from that point on is used to prevent repeating calculations, resulting in a quick runtime. Hopefully i don't forget this in future problems.

## [Day 13](https://adventofcode.com/2023/day/13)
Much simpler than yesterday. There's no major insight on the solution, just a straightforward search for reflected rows. Searching on columns is done by first transposing the input and then searching on the transposed rows.
Part two introduces some noise on the solution, but the structure is the same. The difference is that, when a line is not perfectly reflected, it checks to see if they differ by only one position, and if so, allow the reflection test to continue. Need to be careful with the conditions because only one differing line can be found.

## [Day 14](https://adventofcode.com/2023/day/14)
A familiar day, i feel that i've done this one in previous years. That didn't stop me from falling into the error of overwriting board positions when tilting. Relevant notes:
- When tilting, each rock is moved in the tilt direction until an obstacle is found. Needs some attention to not overwrite positions, which is done by adjusting the loop according to the tilt direction: if tilting South, loop from the end of the board to the start, otherwise loop from the beginning of the board. Likewise for tilting East/West. This guarantees that rocks are moved in the correct order, not overwriting each other.
- For part two, a cache of the boards and indexes when they were generated is kept so that cycles can be detected. The board is evolved until a cycle is detected, which is "short-circuit" by calculating the remaining steps after the last loop on the cycle and only execute those steps. Straightforward modular arithmetic.

## [Day 15](https://adventofcode.com/2023/day/15)
A pretty straightforward day. In fact it is so straightforward that i was at a loss when the result of part one wasn't correct... The issue was an extra `\n` on the input that the problem description even warned about,
Part two's description is very convoluted but describes a simple procedure. I used vectors for the boxes and lists, which are not ideal for removing elements but given the limited size of the input, are adequate.

## [Day 16](https://adventofcode.com/2023/day/16)
A similar day to day 10, though a bit easier. Reding part one i was expecting a hard part two, but it turned out to be pretty straightforward.
The solution is based on following rays through the grid, and when a junction is encountered, generate new rays to follow. Stop when reaching the borders of the grid or when reaching a cell that has already been visited in the same direction. To implement this, a map of junctions/directions is defined, where for each type of junction and direction of travel, the resulting possible directions of travel can be obtained. This allows to quickly generate new rays in the correct direction when a junction is encountered. A grid of cells with a vector of directions is kept to control which cells have been visited and in which direction.
For part two the runtime is a bit high (about 600ms), so the solution isn't very efficient but it's readable so i kept it.

## [Day 17](https://adventofcode.com/2023/day/17)
Not yest finished as it is too inefficient.

## [Day 18](https://adventofcode.com/2023/day/18)
It's a learn something new day. Part one was initially done similarly to day 10, though that didn't scale to part two. Spent part of the day trying to devise a way to get the interior area from a list of the vertices, but before implementing something complex i gave up and looked for clues on Reddit to check if that was the right track. Learned about [Shoelace formula](https://en.wikipedia.org/wiki/Shoelace_formula) and [Pick's theorem](https://en.wikipedia.org/wiki/Pick%27s_theorem) and also that, next time, need to look for clues earlier.

## [Day 19](https://adventofcode.com/2023/day/19)
A fun and interesting day. Somewhat challenging dealing with various levels of data, but interesting.
Parsing the input is messy but straightforward. Rules have a specific `struct` to model them, workflows are modelled as a map of workflow name to a list of its rules, parts are a map of its category to its value. Parsing the input returns the workflows a the list of parts.
Part one is relatively straightforward: pass each part through the workflows (starting at "in") and collect the accepted parts.
Part two is more interesting : The idea is to splice a hypercube iteratively with each rule on the workflow. This involves keeping a queue of the hypercubes yet to be completely spliced, and using the workflows to splice them step by step. Each rule in a workflow that is not final  splices the hypercube and sent to the queue for further exploration. The inverse of the hypercube continues to be processed with the next rule of the workflow. When an accepted hypercube is found, its area is calculated and added to the combinations.

## [Day 20](https://adventofcode.com/2023/day/20)
A different and interesting day. Don't remember any previous day with a digital circuit simulation. It was a bit confusing because it mixes Flip Flops, which only switches when there's a negative pulse, with NAND gates, which always switch depending on the input, so care must be taken to propagate the pulses through the circuit.
To represent the circuit a map of gates is used, with each gate storing its type, state and the respective input and output gates. A pulse is represented by the source and dest gate and its value.
Part one simulates 1000 button pushes, collecting the resulting pulses and accumulating the high and low pulses. For each push of the button a queue is kept of the gates that have received input pulses and have to send their output pulses. For each of those gates, pulses are sent to its destinations, updating their states according to its type (Flip Flop or NAND), and if there was a switch, queue them in the unprocessed queue.
Part two was initially a let down... I suspected that i needed to fully decode the circuit to see what it was doing, just like in previous years we had to interpret pseudo assembler code to get the answer. I'm up to look at assembler, but i'm not up to decode digital circuits, that was a long time ago, and never was my strong (or my interest). I therefore looked for clues on Reddit, where there were some graphs of separate loops, so i looked more carefully at the input. So by inspection: the desired result is in RX, which is fed by a NAND gate LL, which is fed by 4 gates, which apparently are the end points of 4 different chains. RX will be 0 when all those 4 gates are 1, so we need to calculate the lengths of each of those chains and return its LCM (which is its multiple as they all have prime length).

## [Day 21](https://adventofcode.com/2023/day/21)

## [Day 22](https://adventofcode.com/2023/day/22)

## [Day 23](https://adventofcode.com/2023/day/23)

## [Day 24](https://adventofcode.com/2023/day/24)

## [Day 25](https://adventofcode.com/2023/day/25)
