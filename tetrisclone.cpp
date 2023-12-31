#include "board.hpp"
#include "shape.hpp"
#include "tetromino.hpp"
#include <curses.h>
#include <ncurses.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

int ColorBlock(char c) {
    switch (c) {
    case 'L':
        return 2;
    case 'J':
        return 3;
    case 'S':
        return 4;
    case 'Z':
        return 5;
    case 'T':
        return 6;
    case 'O':
        return 7; // TODO Fix. These should be different
    case 'I':
        return 7; // TODO Fix. These should be different
    }
    return 1;
}

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
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_BLUE, COLOR_BLACK);

    // int windowX = 33, windowX = 100;
    int windowY = 33, windowX = 5;

    // Create a new window for the game board
    WINDOW *window = newwin(windowY + 1, windowX + 1, 0, 0);

    // SideWindows
    WINDOW *highscore = newwin(10, 29, 0, windowX + 2);
    WINDOW *nextblock = newwin(24, 29, 10, windowX + 2);

    // Create a new game board
    // Board board(windowY-1, windowX-1);
    Board board(windowX - 1, windowY);

    board.spawnX = windowX - 1;
    board.spawnY = windowY;
    pair<int, int> nextBlockCoord{3, 3};

    // Create a new Tetromino
    // Tetromino tetromino('L', 4, windowX / 2);
    Tetromino next_tetromino(3, 3);
    Tetromino tetromino(windowX / 2, 3);

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
        mvwprintw(nextblock, 3, 1,
                  ("Next Down?: " +
                   to_string(tetrominox + 1 == board.height - 1 ||
                             board.IsBlockFilled(tetrominox, tetrominoy + 1)))
                      .c_str());
        mvwprintw(nextblock, 4, 5, currRightBound.c_str());
        mvwprintw(highscore, 0, 9, "High Score");
        mvwprintw(highscore, 1, 18,
                  ("Score: " + to_string(board.highScore)).c_str());
        // ###################################-Screen-###############################
        //  Render the game board to the window
        wattron(window, COLOR_PAIR(1));
        board.DrawBoard(window);
        wattroff(window, COLOR_PAIR(1));
        wrefresh(window);
        // Render the Tetromino
        wattron(window, COLOR_PAIR(ColorBlock(tetromino.shape.getType())));
        tetromino.Show("*", *window);
        wattroff(window, COLOR_PAIR(ColorBlock(tetromino.shape.getType())));

        wattron(highscore,
                COLOR_PAIR(ColorBlock(next_tetromino.shape.getType())));
        next_tetromino.Show("*", *highscore);
        wattroff(highscore,
                 COLOR_PAIR(ColorBlock(next_tetromino.shape.getType())));

        refresh();
        wrefresh(window);
        wrefresh(nextblock);
        wrefresh(highscore);
        // Handle user input
        // int ch = 'k';
        int ch = wgetch(window);

        // Update the game state based on user input
        switch (ch) {
        case 'h':
        case 'a': // move left one
            if (tetromino.GetLeftBoundary() > 1 &&
                !board.IsBlockFilled(tetromino.GetLeftBoundary() - 1,
                                     tetrominoy)) {
                tetromino.MoveLeft();
            }
            break;
        case 'l':
        case 'd': // Move right one
            if (tetromino.GetRightBoundary() < windowX - 1 &&
                !board.IsBlockFilled(tetromino.GetRightBoundary(),
                                     tetrominoy)) {
                tetromino.MoveRight();
            }
            break;
        case 'j':
        case 's': // MoveDown one
            if (tetrominox < windowY - 1) {
                tetromino.MoveDown();
            }
            break;
        case 'k':
        case 'w': // Rotate
            tetromino.RotateIfNoCollision(board);
            break;
        case 'i': // QuickPlace
            tetromino.QuickDrop(board);
            break;
        case 'q': // Exit Game
            delwin(window);
            delwin(highscore);
            delwin(nextblock);
            endwin();
            return;
        case ' ': // Swap with nextBlock
            tetromino.UnShow(*window);
            next_tetromino.UnShow(*highscore);
            if (next_tetromino.ExchangeWillBeOutOfBounds(tetromino.GetXY(),
                                                         board)) {
                continue; // Skip the command
            }

            Tetromino temp = tetromino;
            tetromino = next_tetromino;
            next_tetromino = temp;
            tetromino.SetXY(temp.GetXY());
            next_tetromino.SetXY(nextBlockCoord);
            continue;
        }

        // Check if the Tetromino is colliding with the game board
        if (tetromino.IsCollidingWithBoard(board)) {

            // Add the Tetromino blocks to the game board.
            tetromino.SetOnBoard(board, tetromino.GetXY());

            // Check for completed lines
            for (int line = 1; line < board.height; line++) {
                if (board.IsLineCompleted(line)) {
                    board.ClearLine(line);
                    // board.DropBlocks(line, window);//WrigglyMode
                    board.ClearBlocks(line, window); // TetrisMode
                    board.highScore++;
                }
            }

            // Swap Tetromino with next up
            next_tetromino.UnShow(*highscore);
            refresh();
            wrefresh(highscore);
            tetromino = next_tetromino;
            next_tetromino = Tetromino(windowX / 2, 4);
        }
        if (board.IsGameOver(tetromino.GetXY())) {
            delwin(window);
            delwin(highscore);
            delwin(nextblock);
            endwin();
            return;
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
