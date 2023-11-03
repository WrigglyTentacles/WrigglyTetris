#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <vector>

using std::vector, std::pair;

// Class to represent the game board
class Board {
  public:
    int start_height = 1;
    int height;
    int start_width = 1;
    int width;
    int highScore = 0;
    int spawnX = 0;
    int spawnY = 0;

    vector<vector<bool>> blocks;

    Board(int width, int height) {
        this->width = width;
        this->height = height;
        blocks = vector<vector<bool>>(height, vector<bool>(width, false));
    }

    void SetBlock(int x, int y, bool filled) { this->blocks[y][x] = filled; }

    bool IsGameOver(pair<int,int> xy) { 
		if(IsBlockFilled(xy.first, xy.second)){
			return true;
		}
		return false;
	}

    bool IsBlockFilled(int x, int y) { return blocks[y][x]; }
    bool ExpensiveIsBlockFilled(int x, int y) {
        if (x < this->start_width || y < this->start_height ||
            x > this->width || y > this->height)
            return true;
        return blocks[y][x];
    }

    void DrawBoard(WINDOW *window) {
        touchwin(window);
        for (int y = this->start_height; y < this->height; y++) {
            for (int x = this->start_width; x <= this->width; x++) {
                if (this->IsBlockFilled(x, y)) {
                    mvwprintw(window, y, x, "#");
                } else {
                    mvwprintw(window, y, x, " ");
                }
            }
        }
        refresh();
        wrefresh(window);
    }
    /*
     * @Returns: Void
     * @Takes: Line
     * @ExtraNotes: Moves all floating blocks above this line down until they
     * reache the board.
     */
    void DropBlocks(int line, WINDOW *window) {
        for (int col = line; col > 1; col--) { // Starting at the cleared line
            for (int block = 0; block <= width;
                 block++) { // Look at each block in the line
                if (col + 1 < height && blocks[col][block] &&
                    !blocks[col + 1]
                           [block]) { // if we can check the next column and
                                      // the current block is filled but the
                                      // block one column down isn't
                    int detachedCol = col;
                    while (detachedCol + 1 < height &&
                           blocks[detachedCol][block] &&
                           !blocks[detachedCol + 1][block]) {
                        blocks[detachedCol + 1][block] = true;
                        blocks[detachedCol][block] = false;
                        detachedCol++;
                        usleep(10000);
                        DrawBoard(window);
                        refresh();
                        wrefresh(window);
                    }
                }
            }
        }
    }

    void ClearBlocks(int line, WINDOW *window) {
        for (int col = line; col > 1; col--) { // Starting at the cleared line
            for (int block = 0; block <= width;
                 block++) { // Look at each block in the line
                if (col + 1 < height && blocks[col][block] &&
                    !blocks[col + 1]
                           [block]) { // if we can check the next column and
                                      // the current block is filled but the
                                      // block one column down isn't
                    blocks[col + 1][block] = true;
                    blocks[col][block] = false;
                    usleep(100000);
                    DrawBoard(window);
                    refresh();
                    wrefresh(window);
                }
            }
        }
    }

    void ClearLine(int line) {
        for (int x = 1; x <= width; x++) {
            // blocks[line][x] = (!blocks[line][x])?true:blocks[line][x];
            blocks[line][x] = false;
        }
    }

    bool IsLineCompleted(int line) {
        for (int x = 1; x <= this->width; x++) {
            if (!this->blocks[line][x]) {
                return false;
            }
        }
        return true;
    }
};
