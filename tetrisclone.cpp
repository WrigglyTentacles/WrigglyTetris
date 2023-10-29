#include <curses.h>
#include <ncurses.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

// Class to represent the game board
class Board {
  public:
    Board(int width, int height) {
        this->width = width;
        this->height = height;
        blocks = vector<vector<bool>>(height, vector<bool>(width, false));
    }

    void SetBlock(int x, int y, bool filled) { blocks[y][x] = filled; }

    bool IsBlockFilled(int x, int y) { return blocks[y][x]; }

    void ClearLine(int line) {
        for (int x = 0; x < width; x++) {
            blocks[line][x] = false;
        }
    }

    bool IsLineCompleted(int line) {
        for (int x = 0; x < width; x++) {
            if (!blocks[line][x]) {
                return false;
            }
        }
        return true;
    }
    int width;
    int height;

    vector<vector<bool>> blocks;
};

class Tetromino {
  public:
    Tetromino(const vector<vector<bool>> &shape) { this->shape = shape; }

    void MoveDown() { x++; }

    void MoveLeft() { y--; }

    void MoveRight() { y++; }

    void Rotate() {
        // TODO: Implement Tetromino rotation
    }

    bool IsCollidingWith(Board &board) {
        unsigned int tx = this->x, ty = this->y;

        for (unsigned int y = 0; y < shape.size(); y++) {
            for (unsigned int x = 0; x < shape[0].size(); x++) {
                if (shape[y][x] && board.IsBlockFilled(x + tx, y + ty)) {
                    return true;
                }
            }
        }
        return false;
    }
    vector<vector<bool>> shape;
    int x;
    int y;
};

// Main game function
void GameLoop() {
    // Initialize ncurses
    initscr();
    int windowX = 33, windowY = 100;

    // Create a new window for the game board
    WINDOW *window = newwin(windowX + 1, windowY + 1, 0, 0);

    // SideWindows
    WINDOW *highscore = newwin(10, 29, 0, 101);
    WINDOW *nextblock = newwin(24, 29, 10, 101);

    // Create a new game board
    Board board(windowX, windowY);

    // Create a new Tetromino
    Tetromino tetromino(vector<vector<bool>>{{true, true}, {true, false}});
    tetromino.y = 0 + 25;
    tetromino.x = 0 + 2;

    // Start the game loop
    while (true) {
        // TODO: Split the Screen boxes into a seperate file
        // ###################################-Screen-###############################
        box(window, 0, 0);
        touchwin(window);
        wrefresh(window);
        box(nextblock, 0, 0);
        mvwprintw(nextblock, 0, 9, "Block List");
        string currTetrominoLocation = "Current X: " + to_string(tetromino.x) +
                                       " Y: " + to_string(tetromino.y) + "";
        mvwprintw(nextblock, 1, 5, currTetrominoLocation.c_str());
        mvwprintw(
            nextblock, 2, 5,
            ("Tet: " + to_string(board.IsBlockFilled(tetromino.x, tetromino.y)))
                .c_str());
        mvwprintw(nextblock, 3, 5,
                  ("Colliding: " + to_string(tetromino.IsCollidingWith(board)))
                      .c_str());
        touchwin(nextblock);
        wrefresh(nextblock);
        box(highscore, 0, 0);
        mvwprintw(highscore, 0, 9, "High Score");
        touchwin(highscore);
        wrefresh(highscore);
        // ###################################-Screen-###############################
        //  Render the game board to the window
        for (int y = 1; y < board.height; y++) {
            for (int x = 1; x < board.width; x++) {
				if(tetromino.x==x&&tetromino.y==y){
					board.SetBlock(tetromino.x,tetromino.y, true);
				}else {
					board.SetBlock(tetromino.x,tetromino.y, false);
                }

                if (board.IsBlockFilled( x, y)) {// If the tetromino is currently
                    mvwprintw(window, x, y, "#");
                } else {
                    mvwprintw(window, x, y, " ");
                }
            }
        }
        wrefresh(window);

        // Handle user input
        int ch = getch();

        // Update the game state based on user input
        switch (ch) {
        case 'h':
            if (tetromino.y > 1) {
                tetromino.MoveLeft();
            }
            break;
        case 'l':
            if (tetromino.y < windowY-2) {
                tetromino.MoveRight();
            }
            break;
        case 'j':
            if (tetromino.x < windowX) {
                tetromino.MoveDown();
            }
            break;
        case 'k':
            tetromino.Rotate();
            break;
        case 'q':
            return;
            break;
        }

        // Check if the Tetromino is colliding with the game board
        if (tetromino.IsCollidingWith(board)) {
            // If the Tetromino is colliding, it has landed.
            // Add the Tetromino blocks to the game board.
            for (unsigned int y = 0; y < tetromino.shape.size(); y++) {
                for (unsigned int x = 0; x < tetromino.shape[0].size(); x++) {
                    if (tetromino.shape[y][x]) {
                        board.SetBlock(x + tetromino.x, y + tetromino.y, true);
                    }
                }
            }

            // Check for completed lines
            for (int y = 0; y < board.height; y++) {
                if (board.IsLineCompleted(y)) {
                    board.ClearLine(y);
                }
            }

            // Create a new Tetromino
            tetromino =
                Tetromino(vector<vector<bool>>{{true, true}, {true, false}});
        }

        // Refresh the window
        refresh();

        // Wait for a short period of time before the next frame
        usleep(10000);
    }
}

int main() {
    GameLoop();
    endwin();
    return 0;
}
