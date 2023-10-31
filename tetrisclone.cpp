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
    Board(int height, int width) {
        this->height = height;
        this->width = width;
        blocks = vector<vector<bool>>(height, vector<bool>(width, false));
    }

    void SetBlock(int x, int y, bool filled) { this->blocks[y][x] = filled; }

    bool IsBlockFilled(int x, int y) { return blocks[y][x]; }

    void DrawBoard(WINDOW *window) {
		touchwin(window);
        for (int y = this->start_height; y < this->height; y++) {
            for (int x = this->start_width; x < this->width-1; x++) {
                if (this->IsBlockFilled(x, y)) {
                    mvwprintw(window, x, y, "#");
                } else {
                    mvwprintw(window, x, y, "0");
                }
            }
        }
		refresh();
		wrefresh(window);
    }

    void ClearLine(int line) {
        for (int x = 1; x < width; x++) {
            blocks[line][x] = false;
        }
    }

    bool IsLineCompleted(int line) {
        for (int x = 1; x < this->width - 1; x++) {
            if (!this->blocks[line][x]) {
                return false;
            }
        }
        this->highScore++;
        return true;
    }
    int start_height = 1;
    int height;
    int start_width = 1;
    int width;
    int highScore;

    vector<vector<bool>> blocks;
};

// Tetromino shape definitions
class Shape {
  public:
    pair<int, int> pivot = {0, 0};
    Shape(char predefinedShape) {
        switch (predefinedShape) {
        case 'L':

            this->blocks_used = {{0, 1},  {0, 2},  {-1, 0},
                                 {-2, 0}, {-3, 0}, pivot};
            break;
        }
    };
    vector<pair<int, int>> getBlockLocation(void) { return blocks_used; }
    int getTopBlock(void) {
        int largestX = pivot.first;
        for (auto i : this->blocks_used) {
            largestX = (i.first > largestX) ? i.first : largestX;
        }
        return largestX;
    }
    int getBottomBlock(void) {
        int smallestX = pivot.first;
        for (auto i : this->blocks_used) {
            smallestX = (i.first < smallestX) ? i.first : smallestX;
        }
        return smallestX;
    }
    int getRightBlock(void) {
        int largestY = pivot.second;
        for (auto i : this->blocks_used) {
            largestY = (i.second > largestY) ? i.second : largestY;
        }
        return largestY;
    }
    int getLeftBlock(void) {
        int smallestY = pivot.second;
        for (auto i : this->blocks_used) {
            smallestY = (i.second < smallestY) ? i.second : smallestY;
        }
        return smallestY;
    }

  private:
    vector<pair<int, int>> blocks_used;
};

class Tetromino {
  public:
    Tetromino(char c, int spawnx, int spawny) {
        this->x = spawnx;
        this->y = spawny;
        this->shape = Shape(c);
    }

    void MoveDown() { x++; }

    void MoveLeft() { y--; }

    void MoveRight() { y++; }

    void Rotate() {
        // TODO: Implement Tetromino rotation
        // should rotate current shape 90 deg clockwise
    }
    int GetRightBoundary() { return this->y + this->shape.getRightBlock(); }

    bool IsCollidingWithBoard(Board &board) {
        auto blockVector = this->shape.getBlockLocation();
        for (auto [blockx, blocky] : blockVector) {
            if (this->x + blockx == board.height - 1 ||
                board.IsBlockFilled(this->y + blocky, this->x + blockx)) {
                return true;
            }
        }
        return false;
    }
    bool IsOnBoard(Board &board) {
        if (shape.getTopBlock() < board.start_width &&
            shape.getBottomBlock() < board.width) {
            return true;
        }
        return false;
    }
    void Show(const char *c, WINDOW &window) {
        for (auto xyVec : shape.getBlockLocation()) {
            mvwprintw(&window, this->x + xyVec.first, this->y + xyVec.second,
                      c);
        }
    }
    void SetOnBoard(Board &board, pair<int, int> currentPivot) {
        for (auto xyVec : shape.getBlockLocation()) {
            board.SetBlock(currentPivot.second + xyVec.second,
                           currentPivot.first + xyVec.first, true);
        }
    }
    pair<int, int> GetXY() { return pair<int, int>{this->x, this->y}; }

    /*
     * A shape should be defined as the blocks relative to it's pivot point
     * therefore.
     * pair<int,int>xy pivot_point;
     * should be private and should only be allowed to be set using an interface
     * vector<pair<int,int>> blocks_aroundPivotPoint;//
     *
     *
     */
    // vector<vector<bool>> shape;
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
	cbreak();
    noecho();
    curs_set(0);
	nodelay(stdscr, TRUE);

    // Start up colors
    if (has_colors() == false) {
        endwin();
        printf("Your Terminal doesn't support colors scrub\n");
        exit(0);
    }
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

    // int windowX = 33, windowX = 100;
    int windowY = 33, windowX = 20;

    // Create a new window for the game board
    WINDOW *window = newwin(windowY + 1, windowX + 1, 0, 0);

    // SideWindows
    WINDOW *highscore = newwin(10, 29, 0, windowX + 2);
    WINDOW *nextblock = newwin(24, 29, 10, windowX + 2);

    // Create a new game board
    // Board board(windowY-1, windowX-1);
    Board board(windowY - 1, windowX - 1);

    // Create a new Tetromino
    // Tetromino tetromino('L', 4, windowX / 2);
    Tetromino tetromino('L', 4, windowX / 2);

    box(window, 0, 0);
    box(nextblock, 0, 0);
    box(highscore, 0, 0);
    refresh();
    wrefresh(window);

    // Start the game loop
    while (true) {
		refresh();
        // TODO: Split the Screen boxes into a seperate file
        // ###################################-Screen-###############################
        mvwprintw(nextblock, 0, 9, "Block List");
        auto [tetrominox, tetrominoy] = tetromino.GetXY();
        string currRightBound =
            "RightBoundary: " + to_string(tetromino.GetRightBoundary());
        string currTetrominoLocation = "Current X: " + to_string(tetrominox) +
                                       " Y: " + to_string(tetrominoy) + "";
        string currBoardDims = "Height: " + to_string(board.height) +
                               " Width: " + to_string(board.width) + "";
        mvwprintw(nextblock, 1, 5, currTetrominoLocation.c_str());
        mvwprintw(nextblock, 2, 5, currBoardDims.c_str());
        mvwprintw(
            nextblock, 3, 1,
            ("Next Down?: " + to_string(tetrominox+1==board.height-1||board.IsBlockFilled(tetrominox, tetrominoy+1)))
                .c_str());
        mvwprintw(nextblock, 4, 5, currRightBound.c_str());
        mvwprintw(highscore, 0, 9, "High Score");
        mvwprintw(highscore, 1, 18,
                  ("Score: " + to_string(board.highScore)).c_str());
        // ###################################-Screen-###############################
        //  Render the game board to the window
        wattron(window, COLOR_PAIR(2));
        board.DrawBoard(window);
        wattroff(window, COLOR_PAIR(2));
        wrefresh(window);
        // Render the Tetromino
        wattron(window, COLOR_PAIR(1));
        tetromino.Show("*", *window);
        wattroff(window, COLOR_PAIR(1));
        // wrefresh(window);

        //touchwin(window);
        //touchwin(nextblock);
        //touchwin(highscore);
        refresh();
        wrefresh(window);
        wrefresh(nextblock);
        wrefresh(highscore);
        // Handle user input
        // int ch = 'j';
        int ch = wgetch(window);

        // Update the game state based on user input
        switch (ch) {
        case 'h':
            if (tetrominoy > 1 &&
                !board.IsBlockFilled(tetrominox, tetrominoy - 1)) {
                tetromino.MoveLeft();
            }
            break;
        case 'l':
            if (tetromino.GetRightBoundary() < windowX - 1 &&
                !board.IsBlockFilled(tetrominox,
                                     tetromino.GetRightBoundary() + 1)) {
                tetromino.MoveRight();
            }
            break;
        case 'j':
            if (tetrominox < windowY - 1) {
                tetromino.MoveDown();
            }
            break;
        case 'k':
            tetromino.Rotate();
            break;
        case 'q':
			delwin(window);
			delwin(highscore);
			delwin(nextblock);
			endwin();
            return;
            break;
        }

        // Check if the Tetromino is colliding with the game board
        if (tetromino.IsCollidingWithBoard(board)) {
            // If the Tetromino is colliding, it has landed.
            // Add the Tetromino blocks to the game board.

            tetromino.SetOnBoard(board, tetromino.GetXY());

            /*
// Check for completed lines
for (int line = 1; line < board.height; line++) {
if (board.IsLineCompleted(line)) {
            board.highScore++;
board.ClearLine(line);
}
}
       */

            // Create a new Tetromino
            tetromino = Tetromino('L', 4, windowX / 2);
        }

        // Wait for a short period of time before the next frame
        usleep(1000);
    }

}

int main() {
    GameLoop();
	endwin();
    return 0;
}
