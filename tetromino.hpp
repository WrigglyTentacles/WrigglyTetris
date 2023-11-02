#pragma once
#include "board.hpp"
#include "shape.hpp"
#include <cstdlib>

class Tetromino {
  public:
    Tetromino(char c, int spawnx, int spawny) {
        this->x = spawnx;
        this->y = spawny;
        this->shape = Shape(c);
    }

    Tetromino(int spawnx, int spawny) {
        this->x = spawnx;
        this->y = spawny;
        int rVal = rand();
        rVal %= shape.shapeCount;
        char c = 'L';
        switch (rVal) {
        case 0:
            c = 'L';
            break;
        case 1:
            c = 'J';
            break;
        case 2:
            c = 'S';
            break;
        case 3:
            c = 'Z';
            break;
        case 4:
            c = 'T';
            break;
        case 5:
            c = 'I';
            break;
        case 6:
            c = 'O';
            break;
        }
        this->shape = Shape(c);
    }

    void MoveDown() { y++; }

    void MoveLeft() { x--; }

    void MoveRight() { x++; }

    void Rotate() {
        if (shape.getType() != 'O') {
            shape.rotateShape();
        }
    }
    void QuickDrop(Board &board) {
        while (!this->IsGoingToCollideWithBoard(board)) {
            MoveDown();
        }
    }
    int GetRightBoundary() { return this->x + this->shape.getRightBlock(); }
    int GetLeftBoundary() { return this->x + this->shape.getLeftBlock(); }

	/*
	* @Returns: Boolean
	* @Takes: void
	* @ExtraNotes: Should tell us whether a rotation will cause a collision with either the frame of the board or a currently placed block;
	*/
	bool RotateIfNoCollision(Board& board){
        if (shape.getType() == 'O') {
			return false;
        }
		auto oldBlockPosition = this->shape.getBlockLocation();
		shape.rotateShape();
		for(auto [bx,by] : shape.getBlockLocation()){
			if(board.ExpensiveIsBlockFilled(this->x+bx, this->y+by)){
				shape.setBlockLocation(oldBlockPosition);
				return false;
			}
		}
		return false;
	}

    bool IsGoingToCollideWithBoard(
        Board &board) { // Alternate version that is used for auto dropping. The
                        // original function moves y down a row that would put
                        // our collision out of bounds.
        auto blockVector = this->shape.getBlockLocation();
        for (auto [blockx, blocky] :
             blockVector) { // If The collision would be with the board
            if (this->y + blocky == board.height - 1 &&
                !board.IsBlockFilled(this->x + blockx, board.height - 1)) {
                return true;
            } else if (board.IsBlockFilled(this->x +
                                               blockx, // if the collision would
                                                       // be with another block
                                           this->y + blocky)) {
                return true;
            }
        }
        return false;
    }
    bool IsCollidingWithBoard(Board &board) {
        auto blockVector = this->shape.getBlockLocation();
        for (auto [blockx, blocky] :
             blockVector) { // If The collision would be with the board
            if (this->y + blocky == board.height - 1 &&
                !board.IsBlockFilled(this->x + blockx, board.height - 1)) {
                this->y += 1;
                return true;
            } else if (board.IsBlockFilled(this->x +
                                               blockx, // if the collision would
                                                       // be with another block
                                           this->y + blocky)) {
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
    void UnShow(WINDOW &window) {
        for (auto xyVec : shape.getBlockLocation()) {
            mvwprintw(&window, this->y + xyVec.second, this->x + xyVec.first,
                      " ");
        }
    }
    void Show(const char *c, WINDOW &window) {
        for (auto xyVec : shape.getBlockLocation()) {
            mvwprintw(&window, this->y + xyVec.second, this->x + xyVec.first,
                      c);
        }
    }
    void SetOnBoard(Board &board, pair<int, int> currentPivot) {
        for (auto xyVec : shape.getBlockLocation()) {
            board.SetBlock(currentPivot.first + xyVec.first,
                           currentPivot.second + xyVec.second - 1, true);
        }
    }
    pair<int, int> GetXY() { return pair<int, int>{this->x, this->y}; }
    void SetXY(pair<int, int> xy) {
        this->x = xy.first;
        this->y = xy.second;
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
    // vector<vector<bool>> shape;
    Shape shape = Shape('L');
    int rotation;

  private:
    int x;
    int y;
};
