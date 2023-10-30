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
	int start_width;
    int width;
	int start_height;
    int height;

    vector<vector<bool>> blocks;
};

// Tetromino shape definitions
class Shape {
  public:
    pair<int, int> pivot = {0, 0};
    Shape(char predefinedShape) {
        switch (predefinedShape) {
        case 'L':

            this->blocks_used = {{0, 1}, {0, 2}, {0, 3}, {1, 0}, {2, 0}};
            break;
        }
    };
    vector<pair<int, int>> getBlockLocation(void) { return blocks_used; }
    int getTopBlock(void) { 
		int largestY = pivot.second;
		for(auto i:this->blocks_used){
			largestY=(i.second>largestY)?i.second:largestY;
		}
		return largestY; 
	}
    int getBottomBlock(void) { 
		int smallestY = pivot.second;
		for(auto i:this->blocks_used){
			smallestY=(i.second<smallestY)?i.second:smallestY;
		}
		return smallestY; 
	}
    int getRightBlock(void) { 
		int largestX = pivot.first;
		for(auto i:this->blocks_used){
			largestX=(i.first>largestX)?i.first:largestX;
		}
		return largestX; 
	}
    int getLeftBlock(void) { 
		int smallestX = pivot.first;
		for(auto i:this->blocks_used){
			smallestX=(i.first<smallestX)?i.first:smallestX;
		}
		return smallestX; 
	}

  private:
    vector<pair<int, int>> blocks_used;
};

class Tetromino {
  public:
    Tetromino(char c) { this->shape = Shape(c); }

    void MoveDown() { x++; }

    void MoveLeft() { y--; }

    void MoveRight() { y++; }

    void Rotate() {
        // TODO: Implement Tetromino rotation
		// should rotate current shape 90 deg clockwise
    }


	bool IsOnBoard(Board& board){
		if(shape.getTopBlock()<board.start_height && shape.getBottomBlock()<board.height){
			return true;
		}
		return false;
	}

    /*
     * A shape should be defined as the blocks relative to it's pivot point
     * therefore.
     * pair<int,int>xy pivot_point;
     * should be private and should only be allowed to be set using an interface
     * vector<pair<int,int>> blocks_aroundPivotPoint;//
     *
     *
     */
    //vector<vector<bool>> shape;
	Shape shape = Shape('L');
	int rotation;
private:
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
	Tetromino tetromino('L');

    // Start the game loop
    while (true) {
        // TODO: Split the Screen boxes into a seperate file
        // ###################################-Screen-###############################
        box(window, 0, 0);
        touchwin(window);
        wrefresh(window);
        box(nextblock, 0, 0);
        mvwprintw(nextblock, 0, 9, "Block List");
		auto [tetrominox,tetrominoy] = tetromino.shape.pivot;
        string currTetrominoLocation = "Current X: " + to_string(tetrominox) +
                                       " Y: " + to_string(tetrominoy) + "";
        mvwprintw(nextblock, 1, 5, currTetrominoLocation.c_str());
        mvwprintw(
            nextblock, 2, 5,
            ("Tet: " + to_string(board.IsBlockFilled(tetrominox, tetrominoy)))
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
                if (tetrominox == x && tetrominoy == y) {
                    board.SetBlock(tetrominox, tetrominoy, true);
                } else {
                    board.SetBlock(tetrominox, tetrominoy, false);
                }

                if (board.IsBlockFilled(x,
                                        y)) { // If the tetromino is currently
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
            if (tetrominoy > 1) {
                tetromino.MoveLeft();
            }
            break;
        case 'l':
            if (tetrominoy < windowY - 2) {
                tetromino.MoveRight();
            }
            break;
        case 'j':
            if (tetrominox < windowX) {
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
                        board.SetBlock(x + tetrominox, y + tetrominoy, true);
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
                Tetromino('L');
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
