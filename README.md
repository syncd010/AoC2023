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
These warmups are more challenging than previous years. The parsing is contrived, particularly because there are multiple layers of vectors involved. Initially i used regular expressions to parse the input, but the API doesn't seem to be as user-friendly as others and it increased the runtime, so i reverted to simple string parsing to get the runtime bellow 10ms.

## [Day 3](https://adventofcode.com/2023/day/3)
Clearly a Sunday, as this was a hard puzzle for day 3. 
I went for a straightforward solution, representing the input as a 2d array and manually looping over it. For each cell that has a relevant character, check its surrounding neighbors and, if any has a digit, get the enclosing number by going backwards and forwards from the cell.
With this setup part two is relatively straightforward just requiring some simple hard coded adaptions.

## [Day 4](https://adventofcode.com/2023/day/4)
Relatively easy day. Part one is the intersection of the 2 sets of numbers on each card. Could/should have been done with sets but i computed it manually. Part two is also straightforward, just propagate the number of winners on each card to the following n cards and sum.

## [Day 5](https://adventofcode.com/2023/day/5)
First part is relatively easy, apart from parsing the input, which as usual took some time to get right (that's punishment for insisting on C++ for this). It's basically passing each seed through the maps sequentially, and converting it accordingly.
Part two is much more challenging, and i got to compliment the authors for, given the same input and general structure, with a simple change of the rules devising a completely different problem (and much harder). The general structure is to identify the overlaps on two ranges of numbers and convert each segment individually, keeping the non overlapping in a queue for posterior conversion. Needs attention to the way the looping is done (first through the maps, then through seeds, which is different from what was in part one).

## [Day 6](https://adventofcode.com/2023/day/6)
An easy day, with a bit of high school math. The distance traveled is given by d(t) = t(T - t), where T is the race time, so we want all solutions for d(t) > D, where D is the given distance. This is a quadratic equation and solving it gives the interval (min_t, max_t) where the traveled distance is greater than D. To get the integer values on that interval calculate ceil(max_t) - floor(min_t) - 1.

## [Day 7](https://adventofcode.com/2023/day/7)
Today wasn't challenging, but it took some work. 
For part one the relevant bit is to sort the hands according to the rules, which requires establishing an order on the hands, done by calculating:
- A score for the type of hands, which is done by replacing each card in the hand with the number of times that card appears in the hand and summing it (so that a five of a kind becomes 25, a four of a kind becomes 16, etc - this creates a correct order on the types);
- For hands of the same type, substitute `AKQJT` by chars in lexicographic order (`edcba`), so that a string comparison between 2 hands of the same type orders them correctly;
For part two jokers need to be dealt with when determining the hand type. These are substituted by other cards already in the hand, creating new hands without them which are scored, and the maximum type score is used.

## [Day 8](https://adventofcode.com/2023/day/8)
Not a hard day, but it had a twist in part two.
The parsed representation of the input is: 
1) a string of the path, each character considered individually;
2) a map where the keys are nodes (as strings) and the values are pairs of the left/right nodes to follow when traversing the graph.
Given this, part one is a relatively simple tree/graph traversal, starting on the "AAA" node and following the given path until the end node is reached. I included a simple direct loop check, so that the test input from part one didn't hang up here.
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
I did some micro-optimizations to improve the runtime, the main ones being: 1) when tilting a board, instead of moving a rock one place at a time, keep a list of the last empty position and directly move the rock there (improves by about 30%), 2) pass the board by reference and change it, don't return it (about 10%). Further improvements would need to change the approach: instead of dealing directly with the board, use as the main structure a list of the rocks and cubes positions and manipulate them on tilts. Given that there are a lot less rocks/cubes than cells on the board this would predictably lead to a vastly improved runtime.

## [Day 15](https://adventofcode.com/2023/day/15)
A pretty straightforward day. In fact it is so straightforward that i was at a loss when the result of part one wasn't correct... The issue was an extra `\n` on the input that the problem description even warned about,
Part two's description is very convoluted but describes a simple procedure. I used vectors for the boxes and lists, which are not ideal for removing elements but given the limited size of the input, are adequate.

## [Day 16](https://adventofcode.com/2023/day/16)
A similar day to day 10, though a bit easier. Reding part one i was expecting a hard part two, but it turned out to be pretty straightforward.
The solution is based on sending rays through the grid, and when a junction is encountered, change direction and generate new rays to follow. Stop when reaching the borders of the grid or when reaching a cell that has already been visited in the same direction. To implement this, a map of junctions/directions is used where, for each type of junction and direction of travel, the resulting possible directions of travel can be obtained, generating new rays for those directions. A set of cells / directions is kept to control which cells have been visited and in which direction, stopping when one has already been visited.
For part two the runtime is a bit high, so the solution isn't very efficient but it's readable so i kept it.
Post-event: As noticed the solution wasn't very efficient, it took about 800ms with debug info and about 120ms with -O3. To optimize it i had to ditch the vectors, maps and sets and do the following adaptations:
- Use a plain char array to keep track of visited positions/directions (where each cell has different bits set for different directions traveled);
- Keep the ray traveling as far as it can until a junction is reached (grid positions are '.' or '-'/'|' while traveling vertically/horizontally);
- Use a switch to process junctions, instead of using a map with the junctions/new directions.
This improves the runtime to about 70ms with debug info and 8ms with -O3. Maybe it could still be improved by using threads but it's enough for now.

## [Day 17](https://adventofcode.com/2023/day/17)
This was a familiar day, a Uniform Cost Search problem, with some conditions to respect. This was so familiar that i solved it initially by using code from last year: a generic greedy search in Dart. It was pretty quick  and straightforward to implement, but this year is C++, so i coded that generic search in C++. I'm not really happy with the result given that the runtime is about 1800ms without compiler optimizations, and around 400ms with -O2, but that's what i have time for for now.
Post-event: I returned to this problem because i wasn't happy with the runtime. As usual the key to improve this kind of problems is to reduce the number of nodes that are generated and analyzed, so i changed the implementation with the following:
- Instead of generating the next states one at a time for each direction (simulating moving forward step by step and having to keep track of how many steps were taken on a straight direction), generate in one step all possible states from a given position/direction: These are the positions that are between min and max steps away from the current one and which are rotated +90/-90 degrees. No need to generate the intermediate "going forward" positions, as these are a "prelude" to the rotated ones. This transforms the problem making it similar to a regular graph search with the nodes being a pair of position/direction and the edges being the possible moves from that nodes. The moves already contain the constraint about the min and max steps taken in a straight direction which therefore doesn't need to be specifically tracked;
- Only save states in the frontier if they have the least cost to reach a given position/direction. This is can be seen as a bit redundant with the use of a priority queue in the search, but it allows to cut a lot of states to analyze. Likewise (and doubly redundant though efficient), when processing a state always check if it can be reached with a lower cost.
With these changes, runtimes improved to about 35ms (both parts). This was good enough but i was curious about the impact of using a priority queue, so i kept optimizing and used a simple bucket queue instead of the priority queue, which brought down the runtime to 20ms (both parts).

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
Part two was initially a let down... I suspected that i needed to fully decode the circuit to see what it was doing, just like in previous years we had to interpret pseudo assembler code to get the answer. I'm up to look at assembler, but i'm not up to decode digital circuits, that was a long time ago, and never was my strong (or my interest). I therefore looked for clues on Reddit, where there were some graphs of separate loops, so i looked more carefully at the input. So by inspection: the desired result is in RX, which is fed by a NAND gate LL, which is itself fed by 4 gates, which are the end points of 4 different chains. RX will be 0 when all those 4 gates are 1, so we need to calculate the lengths of each of those chains and return its LCM (which is its multiple as they all have prime length).
Post-event: Even though the performance for this day was acceptable, it still was > 10ms. To improve it i switched to store the `Gate` references instead of their ids (and constantly accessing the circuit `Map`), thus improving the runtime to about 5ms. Some care must be taken on the code to always work with the references, and not with value copies, so that the circuit is always updated.

## [Day 21](https://adventofcode.com/2023/day/21)
Today was a bit frustrating... My main issues with it are 1) the solution is far from general, it's specific for the given input, and 2) the real input has properties that the test input didn't have, which are crucial to the solution. These factors combined with the fact that part two was a head scratcher and a time sink, made it kind of frustrating.
Anyway, part one can be solved with a simple Breadth First Search (not really a search, more of an expansion), to check the nodes reached.
For part two, i tried to find a model that was general enough to include all possible input patterns. I noticed that each position seems to be reached only on even or odd steps. There doesn't seem to be a way for a given position to be reached from one path through an odd number of steps and from other path through an even number of steps. I couldn't prove this for all possible input configurations, but i couldn't imagine one in which that occurred, so i just assumed it to be true. This means that the cells "blink" on and off, and to know which cells are active on a given step we just need to know the first step the cell become active (afterwards it will "blink" on odd/even steps). So, on the BFS there's no need to keep all active cells, it just needs to keep the frontier cells and store the visited ones with the first step number that it was reached.
What i didn't notice (because i didn't look at the real input, should've know better after yesterday) was that the real input has direct paths from the center to the 4 edges, which make propagation to other tiles simpler to calculate. The test input doesn't have this, so i kept trying to come up with a model for how active cells would spread through tiles (given that they could enter new tiles from different positions), not finding any general recurrence pattern there. Having a direct path from the start to the edges means that the other tiles will always be entered on the same position and there's a pattern to the spreading. Anyway, i didn't found this alone, i looked for clues on Reddit, where i also learned that the pattern can be described by a second degree polynomial, estimated from 3 points using Lagrange Polynomial Estimation.
This works because the input seems to be carefully crafted for it to work... The input board is 131x131, the first "outer" tiles are reached after 65 steps (because the start in in the center), and we are asked to calculate the result after 26501365 steps. Now 26501365 - 65 = 26501300 and 26501300 / 131 = 202300, so that number of steps was carefully chosen to be when the edge of a tile is reached.
This was the first day that i felt i couldn't solve on my own. I've looked for clues on other days, but i always saw a path to the solution - i looked for clues either because i suspected that it involved some math trickery that i didn't know about (but could find on Google, so finding it on Reddit isn't much different), like in day 18, or i was too lazy to do some particular task (but could do it with with enough effort), like in day 20. Today i felt at a loss on part two, and the solution isn't very satisfying so it was kind of frustrating. At least the code is small.
Post-event: Optimizing this to run in less than 10ms needed the usual trick of substituting the visited set (map) by a vector and mark the visited nodes there. This improves runtime from 40ms to about 8ms, at the cost of making the code more complex and less legible.

## [Day 22](https://adventofcode.com/2023/day/22)
Simplified 3D tetris is a nice one. Not much to say, for now it just tries to drop down each piece one place at a time and checks for collisions. A bit inefficient, takes a couple of seconds to run, so i need to revisit this, but not today.

## [Day 23](https://adventofcode.com/2023/day/23)
Nice day, though the solution is too much inefficient (about 30s). Need to optimize it further, but not now.

## [Day 24](https://adventofcode.com/2023/day/24)
Pure high school math today.
For part one i calculate the equations and coded them. Each stone movement is described by `x(t) = x0 + vx*t` `and y(t) = y0 + vy*t`. From there, project to the 2D xy plane by changing the variables, getting `y(x) = y0 - (vy/vx)*x0 + (vy/vx)*x`. Equalizing 2 stones to get an equation for x (and using `s = vy/vx`): `x = (y0_b - y0_a - s_b*x0_b + s_a*x0_a) / (s_a - s_b)`. From x, get the time plugging x into the 2 original movement equations, check if both times are positive and if so, get the y.
Part two is a big system of equations: equating the rock movement equation to the movement equations of 3 stones, we obtain 9 equations with 9 unknowns (x, y, x, vx, vy, vz, t1, t2, t3). This system can be solved by hand but it's a lot of work and error prone, better to use a tool. I just plugged the equations into Wolfram Alpha and got the result.

## [Day 25](https://adventofcode.com/2023/day/25)
And that's a wrap, with a graph theory problem. I'm no expert on graph theory, but i know enough to recognize a minimum cut problem, just not enough to have memorized any solutions. Wikipedia to the rescue (probably should have used ChatGPT), which after a little search pointed me to [Karger's algorithm](https://en.wikipedia.org/wiki/Karger%27s_algorithm), straightforward and fairly easy to implement. It's a randomized algorithm and runs can take from tens to hundreds of milliseconds, averaging about 100 ms, not ideal but more than adequate. So, i didn't really thought about a solution to the problem, i categorized the problem, and looked for a solution within that category that would fit. Perfectly valid approach, although...
I later looked at Reddit and noticed some references to removing 3 edges and a lightbulb come on: the description explicitly states that 3 is the maximum flow on the graph, so removing 3 paths from 2 vertices on opposite sides will result in a cut of the graph. Given the problem description this is a more adequate solution, and one that could be reached without knowing any graph theory so i also implemented it (as part two). It involves iteratively getting a path from a source vertex to a sink vertex and disconnecting that path (3 times). If the source and sink vertices have maximum flow of 3, that will disconnect the whole graph and we can get answer by checking the reachable vertices from the source one. There remains the issue of selecting the source and sink vertices, so that they are on "opposite" sides: for the test and real data a bfs can be done from the source and assume that the sink vertex is the farthest one, but that might not be true on all graphs, so the sink is selected randomly and the resulting cut is checked to see if it really separated the original graph in 2 (the reachable vertices from the source aren't the whole graph). This solution runs in just a few ms, much faster than Karger's, and somewhat more satisfying.
