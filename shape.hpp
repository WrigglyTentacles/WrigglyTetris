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
            this->type = 'L';
            this->blocks_used = {pivot, {0, -1}, {0, -2}, {1, 0}};
            break;
        case 'J':
            this->type = 'J';
            this->blocks_used = {pivot, {-1, 0}, {0, -1}, {0, -2}};
            break;
        case 'T':
            this->type = 'T';
            this->blocks_used = {pivot, {-1, 0}, {1, 0}, {0, 1}};
            break;
        case 'S':
            this->type = 'S';
            this->blocks_used = {pivot, {-1, 0}, {0, -1}, {1, -1}};
            break;
        case 'Z':
            this->type = 'Z';
            this->blocks_used = {pivot, {-1, -1}, {0, -1}, {0, 1}};
            break;
        case 'I':
            this->type = 'I';
            this->blocks_used = {pivot, {0, 1}, {0, 2}, {0, 3}};
            break;
        case 'O':
            this->type = 'O';
            this->blocks_used = {pivot, {0, 1}, {1, 0}, {1, 1}};
            break;
        }
    };
    int shapeCount = 7; // used for other classes to know the total number of
                        // diff shapes when calcing random shapes
    char getType() { return this->type; }
    void rotateShape() {
        vector<pair<int, int>> nXY;
        for (auto [xx, yy] : blocks_used) {
            yy = yy * (-1);
            std::swap(xx, yy);
            nXY.push_back({xx, yy});
        }
        this->blocks_used = nXY;
    }
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
    vector<pair<int, int>> getBlockLocation(void) { return blocks_used; }
    void setBlockLocation(vector<pair<int, int>> nBlocks) {
        this->blocks_used = nBlocks;
    }

  private:
    char type;
    vector<pair<int, int>> blocks_used;
};
