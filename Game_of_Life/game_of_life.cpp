/*
 Name: Jay Roy
 Email: jroy4@crimson.ua.edu
 Course Section: CS 481
 Homework #: 0
 Instructions to compile the program: g++ -Wall -O3 -o game_of_life game_of_life.cpp
 Instructions to run the program: ./game_of_life <board_size> <max_generations>
                                  Example: ./game_of_life 1000 1000
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

/*
 * GameOfLife class encapsulates the board and simulation logic
 */
class GameOfLife {
private:
    int size;                           // Board size (N x N)
    int actual_size;                    // Actual size including ghost cells ((N+2) x (N+2))
    vector<vector<int>> current_board;  // Current generation board
    vector<vector<int>> next_board;     // Next generation board

public:
    /*
     * Constructor: Allocates boards with ghost cells and initializes
     */
    GameOfLife(int board_size) : size(board_size), actual_size(board_size + 2) {
        // Allocate boards with ghost cells (initialized to 0)
        current_board.resize(actual_size, vector<int>(actual_size, 0));
        next_board.resize(actual_size, vector<int>(actual_size, 0));
    }
    /*
     * Initializes the board with random alive/dead cells
     * Ghost cells (borders) remain dead (0)
     * Inner cells are randomly set to alive (1) or dead (0)
     */
    void initialize() {
        // Seed random number generator
        srand(static_cast<unsigned int>(time(nullptr)));

        // Initialize inner cells (excluding ghost cells)
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                // Approximately 20% chance of being alive initially
                current_board[i][j] = (rand() % 100 < 20) ? 1 : 0;
            }
        }
        // Ghost cells are already 0 from initialization
    }

    /*
     * Counts the number of alive neighbors for a cell at (row, col)
     * Uses the 8-neighbor rule (Moore neighborhood)
     */
    inline int countNeighbors(int row, int col) const {
        return current_board[row - 1][col - 1] +  // Top-left
               current_board[row - 1][col] +      // Top
               current_board[row - 1][col + 1] +  // Top-right
               current_board[row][col - 1] +      // Left
               current_board[row][col + 1] +      // Right
               current_board[row + 1][col - 1] +  // Bottom-left
               current_board[row + 1][col] +      // Bottom
               current_board[row + 1][col + 1];   // Bottom-right
    }

    /*
     * Simulates one generation of the Game of Life
     * Applies the rules:
     *   - Alive cell with 0-1 neighbors dies (loneliness)
     *   - Alive cell with 4+ neighbors dies (overpopulation)
     *   - Alive cell with 2-3 neighbors survives
     *   - Dead cell with exactly 3 neighbors becomes alive (birth)
     *
     * Returns the number of cells that changed state
     */
    int simulateGeneration() {
        int changes = 0;

        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                int neighbors = countNeighbors(i, j);
                int current_state = current_board[i][j];
                int next_state;

                if (current_state == 1) {
                    // Cell is alive
                    if (neighbors <= 1) {
                        // Dies by loneliness
                        next_state = 0;
                    } else if (neighbors >= 4) {
                        // Dies by overpopulation
                        next_state = 0;
                    } else {
                        // Survives (2 or 3 neighbors)
                        next_state = 1;
                    }
                } else {
                    // Cell is dead
                    if (neighbors == 3) {
                        // Birth
                        next_state = 1;
                    } else {
                        // Stays dead
                        next_state = 0;
                    }
                }

                next_board[i][j] = next_state;

                if (current_state != next_state) {
                    changes++;
                }
            }
        }

        return changes;
    }

    /*
     * Swaps current and next boards (efficient pointer swap via std::swap)
     */
    void swapBoards() {
        swap(current_board, next_board);
    }

    /*
     * Counts total alive cells in the board
     */
    int countAliveCells() const {
        int count = 0;
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                count += current_board[i][j];
            }
        }
        return count;
    }

    /*
     * Prints the board (for debugging with small boards)
     * Only prints the inner cells, not ghost cells
     */
    void printBoard() const {
        cout << "\nBoard state:\n";
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                cout << (current_board[i][j] ? '*' : '.') << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }

    int getSize() const { return size; }
};

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <board_size> <max_generations>\n";
        cerr << "Example: " << argv[0] << " 1000 1000\n";
        return EXIT_FAILURE;
    }

    // Parse command line arguments
    int board_size = atoi(argv[1]);
    int max_generations = atoi(argv[2]);

    // Validate arguments
    if (board_size <= 0) {
        cerr << "Error: Board size must be a positive integer\n";
        return EXIT_FAILURE;
    }
    if (max_generations <= 0) {
        cerr << "Error: Maximum generations must be a positive integer\n";
        return EXIT_FAILURE;
    }

    cout << "Game of Life Simulation\n";
    cout << "=======================\n";
    cout << "Board size: " << board_size << " x " << board_size << '\n';
    cout << "Maximum generations: " << max_generations << '\n';

    // Create and initialize the game
    GameOfLife game(board_size);
    game.initialize();

    // Print initial state for small boards (debugging)
    /* Uncomment the following lines for small board debugging:
    if (board_size <= 10) {
        cout << "Initial board (Generation 0):";
        game.printBoard();
    }
    */

    int initial_alive = game.countAliveCells();
    cout << "Initial alive cells: " << initial_alive << '\n';

    // Start timing using high-resolution clock
    auto start_time = chrono::high_resolution_clock::now();

    // Main simulation loop
    int generation;
    bool stable = false;

    for (generation = 1; generation <= max_generations; generation++) {
        // Simulate one generation
        int changes = game.simulateGeneration();

        // Check for stable state (no changes)
        if (changes == 0) {
            stable = true;
            break;
        }

        // Swap boards
        game.swapBoards();

        // Print progress for small boards (debugging)
        /* Uncomment the following lines for small board debugging:
        if (board_size <= 10) {
            cout << "Generation " << generation << ":";
            game.printBoard();
        }
        */
    }

    // Stop timing
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;

    // Print results
    cout << "\n--- Results ---\n";
    if (stable) {
        cout << "Simulation reached stable state at generation " << generation << '\n';
    } else {
        cout << "Simulation completed " << max_generations << " generations\n";
    }

    int final_alive = game.countAliveCells();
    cout << "Final alive cells: " << final_alive << '\n';
    cout << "Time taken: " << fixed << elapsed.count() << " seconds\n";

    return EXIT_SUCCESS;
}
