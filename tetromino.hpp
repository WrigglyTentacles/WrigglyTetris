#pragma once
#include "board.hpp"
#include "shape.hpp"


class Tetromino {
public:
  Tetromino(char c, int spawnx, int spawny) {
    this->x = spawnx;
    this->y = spawny;
    this->shape = Shape(c);
  }

  void MoveDown() { y++; }

  void MoveLeft() { x--; }

  void MoveRight() { x++; }

  void Rotate() {
    // TODO: Implement Tetromino rotation
    shape.rotateShape();
  }
  int GetRightBoundary() { return this->x + this->shape.getRightBlock(); }
  int GetLeftBoundary() { return this->x + this->shape.getLeftBlock(); }

  bool IsCollidingWithBoard(Board &board) {
    auto blockVector = this->shape.getBlockLocation();
    for (auto [blockx, blocky] : blockVector) {
      if (this->y + blocky == board.height - 1 &&
          !board.IsBlockFilled(this->x + blockx, board.height-1)) {
        this->y += 1;
        return true;
      } else if (board.IsBlockFilled(this->x + blockx, this->y + blocky)) {
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
      mvwprintw(&window, this->y + xyVec.second, this->x + xyVec.first, c);
    }
  }
  void SetOnBoard(Board &board, pair<int, int> currentPivot) {
    for (auto xyVec : shape.getBlockLocation()) {
      board.SetBlock(currentPivot.first + xyVec.first,
                     currentPivot.second + xyVec.second - 1, true);
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
