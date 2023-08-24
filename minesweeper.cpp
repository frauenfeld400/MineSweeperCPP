/*
  A Console-Based Minesweeper Clone
  Brandon Frauenfeld
  Finished: 8/23/2023
*/

#include <iostream>
#include <stdio.h>
#include <cstdlib>

#include <windows.h>

using namespace std;

int gameBoard[18][9]; // board pos is 14, 4 to 31, 12.
char displayBoard[18][9]; // what is displayed to user
int revealedBoard[18][9]; // what has been revealed

int mines = 0; // how many mines exist
int inputLocked = 0; // locks input for user, can only press RESET if 1
int firstClick = 1; // so that if user clicks mine, game not over
int revealedTiles = 0; // num of times that have been revealed so far
int winningTiles = 0; // num of tiles that must be revealed to win
int flags = 0; // num of flags user has

void gotoxy(int x, int y);
void setcolor(WORD color);
void boom();
char toChar(int given);
void updateTable();
void initBoard();
void initRevealBoard();
void printInitialBoard();


// player wins game.
// occurs only when revealedTiles = winningTiles.
void win() {
  // print winning messages.
  setcolor(15);
  gotoxy(14, 3);
  cout << "      ";
  gotoxy(21, 2);
  setcolor(240);
  cout << "!";
  gotoxy(22, 2);
  cout << ":)";
  gotoxy(24, 2);
  cout << "!";
  gotoxy(19, 3);
  cout << "YOU WIN!";

  gotoxy(17, 15);
  cout << "->";
  gotoxy(26, 15);
  cout << "<-";
  gotoxy(22, 2);
  // lock user input.
  inputLocked = 1;
  // reveal all mine positions to user.
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 9; j++) {
      if (gameBoard[i][j] == 1) {
        gotoxy(i+14, j+4);
        cout << "*";
      }
    }
  }
  setcolor(15);
  return;
}

// resets all tables and game state.
void resetTables() {
  // give control back to user.
  inputLocked = 0;
  // clear any residual winning / losing messages.
  setcolor(15);
  gotoxy(14, 3);
  cout << "                 ";
  gotoxy(14, 2);
  cout << "                 ";
  gotoxy(17, 15);
  cout << "  ";
  gotoxy(26, 15);
  cout << "  ";
  gotoxy(22, 2);
  // re-print original face and unknown flags.
  cout << ":)";
  gotoxy(14, 3);
  cout << "Flags: ??";
  // re-initialize displayBoard. 
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 9; j++) {
      displayBoard[i][j] = '?';
    }
  }
  // set firstClick, so that the user does not click on a mine 
  // for their first click.
  firstClick = 1;
  // init all other boards.
  initBoard();
  initRevealBoard();
  printInitialBoard();
  // set flags to mines with a 3 mine buffer, so as to not be sure.
  // better to use a random number, though.
  flags = mines + 3;
}

// user clicked on a mine. lock input, show sad face, reveal mines to user.
void boom() {
  // game over! 
  // place arrows near reset button, so user knows what to do.
  gotoxy(17, 15);
  cout << "->";
  gotoxy(26, 15);
  cout << "<-";
  gotoxy(22, 2);
  cout << "X(";
  // reveal mines to user.
  setcolor(240);
  inputLocked = 1;
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 9; j++) {
      if (gameBoard[i][j] == 1) {
        gotoxy(i+14, j+4);
        cout << "*";
      }
    }
  }
  setcolor(15);
  return;
}

// search for mines.
// int x, y: coordinates given from mouse pos.
void search(int x, int y) {
  /*
    searching algorithm:
    begin at x, y given.
    check for mines all around the x, y.
    if no mines, reveal tile, store all coordinates around x, y.
    go through coordinate list, recursively calling search.

    if even just one mine, start counting all mines around x, y.
    change displayBoard coordinate to num mines.
    exit
  */

  int xArray[8]; // for each pos to visit if no mines (excluding center)
  int yArray[8];
  // check if mouse is out of bounds
  if (x > 31 || y > 12 || x < 14 || y < 4) {
    //gotoxy(0, 18);
    //cout << "out of bounds";
    return;
  }
  // init the coordinate arrays
  // use negative vals, so that if searched, immediately return
  for (int i = 0; i < 8; i++) {
    xArray[i] = -1;
    yArray[i] = -1;
  }
  // apply proper offsets to x and y to use in array
  int xCoord = x-14;
  int yCoord = y-4;
  // keep track of how many mines found if any
  int minesFound = 0;

  // check if current spot is empty / revealed
  if (revealedBoard[xCoord][yCoord] == 1) {
    // do nothing
    return;
  }

  // check coordinate and around each coordinate
  if (gameBoard[xCoord][yCoord] ==  1) { // origin
    // mine here!
    //gotoxy(0, 16);
    //cout << "set * to displayBoard[" << xCoord << "][" << yCoord << "].";
    displayBoard[xCoord][yCoord] = '*';
    updateTable();
    boom(); // user lost
    return;
  }
  // search 8 remaining positions.
  gotoxy(0, 16); // for debug output, redundant now
  if (xCoord != 0) { // left side of origin
    if (gameBoard[xCoord-1][yCoord] == 1) {
      //cout << "mine found at " << xCoord-1 << ", " << yCoord;
      minesFound++;
    } else {
      xArray[0] = xCoord-1;
      yArray[0] = yCoord;
    }
  }

  if (xCoord != 17) { // right side of origin
    if (gameBoard[xCoord+1][yCoord] == 1) {
      //cout << "mine found at " << xCoord+1 << ", " << yCoord;
      minesFound++;
    } else {
      xArray[1] = xCoord+1;
      yArray[1] = yCoord;
    }
  }

  if (yCoord != 0) { // top of origin
    if (gameBoard[xCoord][yCoord-1] == 1) {
      //cout << "mine found at " << xCoord << ", " << yCoord-1;
      minesFound++;
    } else {
      xArray[2] = xCoord;
      yArray[2] = yCoord-1;
    }
  }

  if (yCoord != 8) { // bottom of origin
    if (gameBoard[xCoord][yCoord+1] == 1) {
      //cout << "mine found at " << xCoord << ", " << yCoord+1;
      minesFound++;
    } else {
      xArray[3] = xCoord;
      yArray[3] = yCoord+1;
    }
  }

  if (xCoord != 0 && yCoord != 0) { // top left of origin
    if (gameBoard[xCoord-1][yCoord-1] == 1) {
      //cout << "mine found at " << xCoord-1 << ", " << yCoord-1;
      minesFound++;
    } else {
      xArray[4] = xCoord-1;
      yArray[4] = yCoord-1;
    }
  }

  if (xCoord != 17 && yCoord != 0) { // top right of origin
    if (gameBoard[xCoord+1][yCoord-1] == 1) {
      //cout << "mine found at " << xCoord+1 << ", " << yCoord-1;
      minesFound++;
    } else {
      xArray[5] = xCoord+1;
      yArray[5] = yCoord-1;
    }
  }

  if (xCoord != 0 && yCoord != 8) { // bottom left of origin
    if (gameBoard[xCoord-1][yCoord+1] == 1){
      //cout << "mine found at " << xCoord-1 << ", " << yCoord+1;
      minesFound++;
    } else {
      xArray[6] = xCoord-1;
      yArray[6] = yCoord+1;
    }
  }

  if (xCoord != 17 && yCoord != 8) { // bottom right of origin
    if (gameBoard[xCoord+1][yCoord+1] == 1) {
      //cout << "mine found at " << xCoord+1 << ", " << yCoord+1;
      minesFound++;
    } else {
      xArray[7] = xCoord+1;
      yArray[7] = yCoord+1;
    }
  }

  // searching array now populated, or mines have been found.
  revealedBoard[xCoord][yCoord] = 1; // either way, we must mark board pos as revealed.
  // this is so that the algorithm does not re-check an already revealed spot.
  if (minesFound != 0) {
    //gotoxy(0, 20);
    //cout << "found " << minesFound << " mines.";
    displayBoard[xCoord][yCoord] = toChar(minesFound);
    ++revealedTiles;
    return;
  } else { // mines not found. begin recursive search 
    displayBoard[xCoord][yCoord] = ' ';
    ++revealedTiles;
    //gotoxy(0, 21);
    //cout << "no mines found";
    for (int i = 0; i < 4; i++) { // only search up, down, left, right
      gotoxy(0, 15+i);
      //cout << "check: " << xArray[i] << ", " << yArray[i] << ".";
      if (revealedBoard[xArray[i]][yArray[i]] != 1) {
        // only search if not revealed.
        //cout << "searching " << xArray[i] << ", " << yArray[i] << ".";
        search(xArray[i]+14, yArray[i]+4);
      }
    }
  }

}
// this is a Dumb Method but it will Stay!
char toChar(int given) {
  switch(given){
    case(1):
      return '1';
    case(2):
      return '2';
    case(3):
      return '3';
    case(4):
      return '4';
    case(5):
      return '5';
    case(6):
      return '6';
    case(7):
      return '7';
    case(8):
      return '8';
    default:
      return 'n';
  }
}

// init revealedBoard. revealedBoard keeps track of what tiles have
// been revealed, so they are not revealed infinitely in the recursive 
// algorithm used in search.
void initRevealBoard() {
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 9; j++) {
      revealedBoard[i][j] = 0;
    }
  }
}
// highlights the position on the minefield.
// int x, y: coordinates given by mouse pos.
void hover(int x, int y) {
  // changes highlight of whatever char is displayed.
  setcolor(240); // 240 is used as it is white background with black text
  gotoxy(x, y);
  cout << displayBoard[x-14][y-4];
  setcolor(15);
}

// de-highlights pos on minefield.
// int x, y: coordinates given by mouse pos.
void dehover(int x, int y) {
  // reverts change.
  setcolor(15);
  gotoxy(x, y);
  cout << displayBoard[x-14][y-4];
}

// sets color of text.
// WORD color: desired color passed in by method call.
void setcolor(WORD color){
  // set color of what is to be printed.
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    return;
}

// updates the game table based on a user's click on correct pos.
void updateTable() {
  // update flags regardless if any are used.
  gotoxy(21, 3);
  if (flags < 10) { 
    cout << "0" << flags;
  } else {
    cout << flags;
  }
  // update what user sees.
  for (int i = 0; i < 9; i++) {
    gotoxy(14, i+4);
    for (int j = 0; j < 18; j++) {
      cout << displayBoard[j][i];
    }
  }
}

// sets the position of the console's cursor to the specified x,y
// is not the mouse cursor, used for printing in specific locations
// int x, y: specified by method call, goes to x, y position in console window
void gotoxy(int x, int y) {
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// init the gameboard, which keeps track of mines.
void initBoard() {
  // init board. 0 is no mine, 1 is a mine.
  mines = 0;
  // reset winningTiles and revealedTiles.
  // winningTiles is every tile that is not a mine.
  winningTiles = 0; 
  revealedTiles = 0;
  srand((unsigned) time(NULL));
  int randNum;
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 9; j++) {

      randNum = (rand() % 10) + 1; // gen num 1-10. 1 is mine. anything else is not.

      if (randNum == 1) {
        ++mines;
        gameBoard[i][j] = 1;
      } else {
        winningTiles++;
        gameBoard[i][j] = 0;
      } 
    }
  }
  // init displayboard too, just to ?s.
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 9; j++) {
      displayBoard[i][j] = '?';
    }
  }
}


// prints a board that remains to be clicked.
// after this board is printed, init the board.
// ensure there is no mine where the player clicks.
void printInitialBoard() {
  // board begins at x=14, y=4. goto this pos, 
  // increment i instead of newline
  for (int i = 0; i < 9; i++) {
    gotoxy(14, i+4);
    cout << "??????????????????";
  }
}

// a debug method used to see where all the mines are.
void printBoardDebug() {
  // prints a board with 0 as no mine and 1 as mine.
  setcolor(15);
  for (int i = 0; i < 9; i++) {
    gotoxy(14, i+4);
    for (int j = 0; j < 18; j++) {
      cout << gameBoard[j][i];
    }
  }

}

int main() {
  // code to set up the window
  // -------------------------

    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) 
    & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX); // make window unresizeable by user

    RECT rect = {100, 100, 500, 500}; // 500x500 px window
    MoveWindow(consoleWindow, rect.left, rect.top, rect.right-rect.left, 
    rect.bottom-rect.top, TRUE);  // set console window size

  // -------------------------
  // code to remove scroll ability and scrollbar 
  // -------------------------

    // get handle to the console window
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    // retrieve screen buffer info
    CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
    GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);
    // current window size
    short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1; 
    short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
    // current screen buffer size
    short scrBufferWidth = scrBufferInfo.dwSize.X;
    short scrBufferHeight = scrBufferInfo.dwSize.Y;        
    // to remove the scrollbar, make sure the window height matches the screen buffer height
    COORD newSize;
    newSize.X = scrBufferWidth;
    newSize.Y = winHeight;
    // set the new screen buffer dimensions
    int Status = SetConsoleScreenBufferSize(hOut, newSize);
    if (Status == 0) {
          cout << "SetConsoleScreenBufferSize() failed! Reason : " << GetLastError() << endl;
          exit(Status);
      }

  // -------------------------
  setcolor(15);
  cout << "            Welcome to Minesweeper!";
  gotoxy(10, 20);
  cout << "Made by Brandon Frauenfeld";

  // initializing everything to handle mouse inputs
  // -------------------------
  HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE); // to handle mouse input
  INPUT_RECORD InputRecord; // input events in console buffer: mouse events, key events, etc
  DWORD Events;
  COORD coord;
  CONSOLE_CURSOR_INFO cci; 
  cci.dwSize = 1; // redundant
  cci.bVisible = FALSE; // do not allow cursor to be visible

  SetConsoleCursorInfo(hOut, &cci);
  SetConsoleMode(hIn, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
  // -------------------------

  // actual game loop
  printInitialBoard();
  initBoard();
  initRevealBoard();
  //printBoardDebug();
  gotoxy(22, 2);
  cout << ":)";
  gotoxy(20, 15);
  int hoveringReset = 0;
  cout << "RESET";
  gotoxy(20, 17);
  int hoveringDebug = 0;
  cout << "DEBUG";
  gotoxy(14, 3);
  cout << "Flags: ??";
  flags = mines + 3;
  int prevX, prevY = 0; // prev mouse coords
  int insideBoard = 0;
  while(1) {
    // loop: read input first, then handle, then read, etc
    
    ReadConsoleInput(hIn, &InputRecord, 1, &Events); // to capture mouse pos in window

    // check if user won

    if (revealedTiles == winningTiles) {
      win();
    }

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 != 0) {

      int lMouseX = InputRecord.Event.MouseEvent.dwMousePosition.X;
      int lMouseY = InputRecord.Event.MouseEvent.dwMousePosition.Y;

      //gotoxy(0, 2);
      //cout << "lmb pressed at " << lMouseX << ", " << lMouseY << ".\n";

      if ((lMouseX >= 20 && lMouseX < 25) && lMouseY == 15) {
        // user pressed reset button
        resetTables();
        continue;
      }

      if (inputLocked != 1) {

        if ((lMouseX >= 20 && lMouseX < 25) && lMouseY == 17) {
         // user pressed debug button
         gotoxy(0, 18);
         cout << "                               ";
         gotoxy(0, 18);
         cout << "goal: " << winningTiles << ", revealed: " << revealedTiles;
         printBoardDebug();
         continue;
        }

        if ((lMouseX >= 14 && lMouseX <= 31) && (lMouseY >= 4 && lMouseY <= 12)) {

          if (firstClick == 1) { // first click of game
            firstClick = 0;
            gameBoard[lMouseX-14][lMouseY-4] = 0;
            --mines;
          }

          if (displayBoard[lMouseX - 14][lMouseY - 4] == 'X') { // flag, dont do anything
            continue;
          }
          
          search(lMouseX, lMouseY);

        }
        
        if (inputLocked != 1) { // second check needed.
          updateTable();
        }
      }
    }

    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 != 0) {
      int rMouseX = InputRecord.Event.MouseEvent.dwMousePosition.X;
      int rMouseY = InputRecord.Event.MouseEvent.dwMousePosition.Y;
      gotoxy(0, 2);
      //cout << "rmb pressed at " << rMouseX << ", " << rMouseY << ".\n";
      if (firstClick == 1) {
        continue; // user has not clicked yet, do not place flag
      }
      if (inputLocked == 1) {
        continue; // user lost or won
      }
      if ((rMouseX >= 14 && rMouseX <= 31) && (rMouseY >= 4 && rMouseY <= 12)) {
        //cout << "in bounds";
        // if in bounds, put flag denoted by X. but only if it is not an empty tile / numbered tile.
        char given = displayBoard[rMouseX-14][rMouseY-4];
        if (given == 'X') {
          // remove flag
          displayBoard[rMouseX-14][rMouseY-4] = '?';
          ++flags;
          updateTable();
        } else if (given != '?') {
          // do not place flag
          continue;
        } else {
          if (flags == 0) {
            continue;
          }
          // place flag
          --flags;
          displayBoard[rMouseX - 14][rMouseY - 4] = 'X';
          updateTable();
        }
      }
    }
    
    if (MOUSE_EVENT) {
      // gather mouse coordinates
      int mouseX = InputRecord.Event.MouseEvent.dwMousePosition.X;
      int mouseY = InputRecord.Event.MouseEvent.dwMousePosition.Y;
      // check if mouse is in bounds of board
      if ((mouseX >= 14 && mouseX <= 31) && (mouseY >= 4 && mouseY <= 12)) {
        insideBoard = 1;
        // check if prev coordinates are different. dehover prev coordinates if so
        if (prevX != mouseX || prevY != mouseY) {
          if (inputLocked != 1) {
            dehover(prevX, prevY);
          }
          prevX = mouseX;
          prevY = mouseY;
        }
        // TODO debug output
        //gotoxy(0,3);
        //cout << "mouse in pos " << mouseX << ", " << mouseY << ".\n";
        //gotoxy(0, 14);
        //cout << "real board pos " << mouseX-14 << ", " << mouseY-4 << ".\n";
        if (inputLocked != 1) {
          hover(mouseX, mouseY);
        }
      } else {
        // if mouse has exited, or is not in game board, dehover last known position
        if (inputLocked != 1 && insideBoard == 1) {
          insideBoard = 0;
          dehover(prevX, prevY);
        }
      }

      // handle RESET button hovering
      if ((mouseX >= 20 && mouseX < 25) && (mouseY == 15)) {
        hoveringReset = 1; 
        setcolor(240);
        gotoxy(20, 15);
        cout << "RESET";
      } else if (hoveringReset == 1) {
        hoveringReset = 0; // so that it does not print reset forever
        setcolor(15);
        gotoxy(20, 15);
        cout << "RESET";
      }
      // handle DEBUG button hovering
      if ((mouseX >= 20 && mouseX < 25) && mouseY == 17) {
        hoveringDebug = 1;
        setcolor(240);
        gotoxy(20, 17);
        cout << "DEBUG";
      } else if (hoveringDebug == 1) {
        hoveringDebug = 0;
        setcolor(15);
        gotoxy(20, 17);
        cout << "DEBUG";
      }
    }
  }
}


