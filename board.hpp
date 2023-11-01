#pragma once
#include <vector>
#include <ncurses.h>

using std::vector;

// Class to represent the game board
class Board {
public:
  Board(int width, int height) {
    this->width = width;
    this->height = height;
    blocks = vector<vector<bool>>(height, vector<bool>(width, false));
  }

  void SetBlock(int x, int y, bool filled) { this->blocks[y][x] = filled; }

  bool IsBlockFilled(int x, int y) { return blocks[y][x]; }

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

  void ClearLine(int line) {
    for (int x = 1; x <= width; x++) {
      // blocks[line][x] = (!blocks[line][x])?true:blocks[line][x];
      blocks[line][x] = false;
    }
  }

  bool IsLineCompleted(int line) {
    for (int x = 1; x < this->width; x++) {
      if (!this->blocks[line][x]) {
        return false;
      }
    }
    return true;
  }
  int start_height = 1;
  int height;
  int start_width = 1;
  int width;
  int highScore;

  vector<vector<bool>> blocks;
};
