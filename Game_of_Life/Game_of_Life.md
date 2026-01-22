# Game of Life - Cellular Automaton Simulation

---

## Problem Specification

The objective of this homework is to design and implement a simulation of Conway's Game of Life, a well-known cellular automaton devised by mathematician John Horton Conway. The Game of Life operates on a two-dimensional grid of cells, where each cell can be in one of two states: alive or dead. Each cell has eight neighbors (horizontal, vertical, and diagonal).

The simulation evolves through discrete generations according to the following rules:

1. A living cell with one or no neighbors dies from loneliness.
2. A living cell with four or more neighbors dies from overpopulation.
3. A living cell with two or three neighbors survives to the next generation.
4. A dead cell with exactly three living neighbors becomes alive (birth).

The program requirements include: accepting board size and maximum generations as command-line arguments, terminating when either the board reaches a stable state (no changes between generations) or the maximum generation count is reached, and measuring execution time for performance analysis.

---

## Program Design

### Data Structures

The program uses two dynamically allocated 2D arrays of size (N+2) x (N+2), where N is the board size. The extra cells form a "ghost cell" boundary that remains permanently dead, eliminating the need for boundary checking during neighbor counting. This design simplifies the code and improves performance by avoiding conditional statements in the inner loop.

### Double Buffering

Two arrays are used to implement double buffering. The current generation is read from one array while the next generation is written to the other. At the end of each generation, the array pointers are swapped, avoiding the need to copy data between arrays.

### Initialization

The board is initialized using a random number generator with approximately 20% of cells starting in the alive state. The ghost cells are initialized to zero using `calloc()`, which zero-initializes the allocated memory.

### Neighbor Counting

For each cell, the `count_neighbors()` function sums the values of all eight neighboring cells. Since cells store either 0 (dead) or 1 (alive), the sum directly gives the number of alive neighbors.

### Generation Simulation

The `simulate_generation()` function iterates through all non-ghost cells, applies the Game of Life rules based on the neighbor count, and writes the result to the next-generation array. It returns the number of cells that changed state, enabling early termination detection.

### Termination Conditions

The simulation terminates when either:
1. No cells change state between generations (stable state), or
2. The maximum generation count is reached.

---

## Testing Plan

### Correctness Testing

The program was first tested with small board sizes (5x5, 10x10) with visual output enabled to verify that the Game of Life rules were being applied correctly. The visual output showed the board state at each generation, allowing manual verification of neighbor counting and state transitions.

### Boundary Testing

Tests verified that ghost cells remained dead and did not interfere with the simulation.

### Termination Testing

Tests were run to verify both termination conditions: stable state detection (when no changes occur) and maximum generation limit.

### Performance Testing

After correctness was verified, performance tests were conducted using the specified test cases with print statements disabled. Each test case was run three times, and the average execution time was recorded.

---

## Test Cases

The following table shows the performance test cases as specified in the assignment:

| Test Case # | Problem Size | Max. Generations | Avg. Time (sec) |
|-------------|--------------|------------------|------------------|
| 1           | 1000x1000    | 1000             | 1.78             |
| 2           | 1000x1000    | 5000             | 7.21             |
| 3           | 5000x5000    | 1000             | 32.5             |
| 4           | 5000x5000    | 5000             | 160              |
| 5           | 10000x10000  | 1000             | 140              |
| 6           | 10000x10000  | 5000             | 670              |

---

## Machine Specifications

The performance tests were executed on the following system:

| Specification      | Details                                                      |
|--------------------|--------------------------------------------------------------|
| **Machine Name**   | Apple M2 Pro                                                 |
| **Operating System** | MacOS Tahoe (26.1)                                         |
| **Processor**      | Apple M2 Pro – 10 cores                                      |
| **Clock Speed**    | Performance cores: 3.504 GHz<br>Efficiency cores: 2.424 GHz  |
| **Memory**         | 16 GB                                                        |
| **Compiler**       | G++ 17.0.0                                                   |
| **Compiler Flags** | `gcc -Wall -O3 -o game_of_life game_of_life.c`               |

---

## Analysis and Conclusions

### 1. Did you achieve the goals or objectives of this assignment?

Yes, all objectives were achieved. The program successfully simulates Conway's Game of Life, correctly implements all rules, accepts command-line arguments for board size and maximum generations, detects stable states for early termination, and measures execution time for performance analysis.

### 2. How did you achieve these goals (be specific)?

The goals were achieved through careful program design:

- The ghost cell boundary technique simplified neighbor counting by eliminating boundary checks
- Double buffering with pointer swapping enabled efficient generation transitions without data copying
- Dynamic memory allocation using `calloc()` provided automatic zero-initialization for ghost cells
- Change counter comparison during simulation, not a full board comparison after the fact. This is more efficient than doing a separate full board comparison, which would require an additional O(N²) pass through both boards
- The `-O3` compiler optimization flag was used to enable aggressive compiler optimizations for better performance

### 3. What can you infer from the performance results you have obtained?

The performance results demonstrate the following patterns:

**Linear scaling with generations:** Increasing the number of generations from 1000 to 5000 (5x increase) results in approximately 4-5x increase in execution time. This is expected since each generation requires O(N²) operations.

**Quadratic scaling with board size:** Increasing the board size from 1000x1000 to 5000x5000 (25x more cells) results in approximately 25x increase in execution time. This confirms the O(N²) complexity per generation.

**Overall complexity:** The total time complexity is O(G × N²), where G is the number of generations and N is the board dimension. The results match expectations based on this analysis.

### 4. Looking back on the project, what would you do differently?

I would consider implementing additional optimizations from the start, such as using a 1D array with manual index calculation for potentially better cache performance.

### 5. What changes would you make in the design phase of your project?

In the design phase, I think a code structure that facilitates future parallelization could reduce the time taken. The current design with separate arrays and clear function boundaries should translate well to parallel implementations.

### 6. What additions would you suggest to improve the working of your program?

Potential improvements include:
- Parallelization for shared-memory systems
- Adding support for loading initial board configurations from files
- Adding visualization output capabilities
- Detecting oscillating patterns (see [playgameoflife.com](https://playgameoflife.com))

---

## References

1. Conway, J. (1970). The Game of Life. *Scientific American*, 223(4), 4-10.
2. C Programming Language Reference - Dynamic Memory Allocation (malloc, calloc, free).
3. Used Claude Code to optimize the implementation. It suggested using the built-in swap instead of a manual function, and using a change counter comparison instead of a full board comparison, which helped decrease the time taken massively.

