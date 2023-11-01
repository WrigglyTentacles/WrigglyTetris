#pragma once
#include <vector>
using std::vector, std::pair;

// Tetromino shape definitions
class Shape {
public:
  pair<int, int> pivot = {0, 0};
  Shape(char predefinedShape) {
    switch (predefinedShape) {
    case 'L':

      this->blocks_used = {pivot, {0, -1}, {0, -2}, {0, -3}, {1, 0}, {2, 0}};
      break;
    }
  };
  void rotateShape() {
    vector<pair<int, int>> nXY;
    for (auto [xx, yy] : blocks_used) {
      yy = yy * (-1);
      std::swap(xx, yy);
      nXY.push_back({xx, yy});
    }
    this->blocks_used = nXY;
  }
  vector<pair<int, int>> getBlockLocation(void) { return blocks_used; }
  int getTopBlock(void) {
    int smallestY = pivot.second;
    for (auto i : this->blocks_used) {
      smallestY = (i.second < smallestY) ? i.second : smallestY;
    }
    return smallestY;
  }
  int getBottomBlock(void) {
    int largestY = pivot.second;
    for (auto i : this->blocks_used) {
      largestY = (i.second > largestY) ? i.second : largestY;
    }
    return largestY;
  }
  int getRightBlock(void) {
    int largestX = pivot.first;
    for (auto i : this->blocks_used) {
      largestX = (i.first > largestX) ? i.first : largestX;
    }
    return largestX;
  }
  int getLeftBlock(void) {
    int smallestX = pivot.first;
    for (auto i : this->blocks_used) {
      smallestX = (i.first < smallestX) ? i.first : smallestX;
    }
    return smallestX;
  }

private:
  vector<pair<int, int>> blocks_used;
};
