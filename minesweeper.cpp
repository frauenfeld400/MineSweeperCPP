/*
  A Console-Based Minesweeper Clone
  Brandon Frauenfeld
*/
/*
  TODO: 
  1. need a function to update the board
  1a. this function must draw from some list of coordinates that function as flags set by user
  2. need win condition
  2a. this win condition is likely a mixture of revealed tiles and flagged mines
  3. need revealing algorithm
  3a. likely best to: 
  from origin given (xy) by user click: 
    see if current square has no mines in vicinity
    reveal if so. if mine, boom
    if mines in vicinity, deploy proper number
    for each square without mines in vicinity, recursively insert origin into func
    repeat until no more empty squares
    do need a way to make sure we dont go back into already revealed squares
*/
#include <iostream>
#include <stdio.h>
#include <cstdlib>

#include <windows.h>

using namespace std;

void gotoxy(int x, int y);
void setcolor(WORD color);
void boom();
char toChar(int given);
void updateTable();

int gameBoard[18][9]; // board pos is 14, 4 to 31, 12.
char displayBoard[18][9]; // what is displayed to user
int revealedBoard[18][9]; // what has been revealed


int mines;

void boom() {
  // game over! TODO

  gotoxy(0, 17);
  cout << "BOOM!";
  return;
}
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
  if (x > 31 || y > 12 || x < 14 || y < 4) {
    gotoxy(0, 18);
    cout << "out of bounds";
    return;
  }
  // init the coordinate arrays
  for (int i = 0; i < 8; i++) {
    xArray[i] = -1;
    yArray[i] = -1;
  }
  int xCoord = x-14;
  int yCoord = y-4;

  int minesFound = 0;

  // check coordinate and around each coordinate

  if (gameBoard[xCoord][yCoord] ==  1) { // origin
    // mine here!
    gotoxy(0, 16);
    cout << "set * to displayBoard[" << xCoord << "][" << yCoord << "].";
    displayBoard[xCoord][yCoord] = '*';
    updateTable();
    boom();
    return;
  }
  // search 8 remaining positions.
  gotoxy(0, 16); 
  if (xCoord != 0) { // left side of origin
    if (gameBoard[xCoord-1][yCoord] == 1) {
      cout << "mine found at " << xCoord-1 << ", " << yCoord;
      minesFound++;
    } else {
      xArray[0] = xCoord-1;
      yArray[0] = yCoord;
    }
  }

  if (xCoord != 17) { // right side of origin
    if (gameBoard[xCoord+1][yCoord] == 1) {
      cout << "mine found at " << xCoord+1 << ", " << yCoord;
      minesFound++;
    } else {
      xArray[1] = xCoord+1;
      yArray[1] = yCoord;
    }
  }

  if (yCoord != 0) { // top of origin
    if (gameBoard[xCoord][yCoord-1] == 1) {
      cout << "mine found at " << xCoord << ", " << yCoord-1;
      minesFound++;
    } else {
      xArray[2] = xCoord;
      yArray[2] = yCoord-1;
    }
  }

  if (yCoord != 8) { // bottom of origin
    if (gameBoard[xCoord][yCoord+1] == 1) {
      cout << "mine found at " << xCoord << ", " << yCoord+1;
      minesFound++;
    } else {
      xArray[3] = xCoord;
      yArray[3] = yCoord+1;
    }
  }

  if (xCoord != 0 && yCoord != 0) { // top left of origin
    if (gameBoard[xCoord-1][yCoord-1] == 1) {
      cout << "mine found at " << xCoord-1 << ", " << yCoord-1;
      minesFound++;
    } else {
      xArray[4] = xCoord-1;
      yArray[4] = yCoord-1;
    }
  }

  if (xCoord != 17 && yCoord != 0) { // top right of origin
    if (gameBoard[xCoord+1][yCoord-1] == 1) {
      cout << "mine found at " << xCoord+1 << ", " << yCoord-1;
      minesFound++;
    } else {
      xArray[5] = xCoord+1;
      yArray[5] = yCoord-1;
    }
  }

  if (xCoord != 0 && yCoord != 8) { // bottom left of origin
    if (gameBoard[xCoord-1][yCoord+1] == 1){
      cout << "mine found at " << xCoord-1 << ", " << yCoord+1;
      minesFound++;
    } else {
      xArray[6] = xCoord-1;
      yArray[6] = yCoord+1;
    }
  }

  if (xCoord != 17 && yCoord != 8) { // bottom right of origin
    if (gameBoard[xCoord+1][yCoord+1] == 1) {
      cout << "mine found at " << xCoord+1 << ", " << yCoord+1;
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
    gotoxy(0, 20);
    cout << "found " << minesFound << " mines.";
    displayBoard[xCoord][yCoord] = toChar(minesFound);
    return;
  } else { // mines not found. begin recursive search 
    displayBoard[xCoord][yCoord] = ' ';
    gotoxy(0, 21);
    cout << "no mines found";
    for (int i = 0; i < 8; i++) {
      gotoxy(0, 15+i);
      cout << "check: " << xArray[i] << ", " << yArray[i] << ".";
      if (revealedBoard[xArray[i]][yArray[i]] != 1) {
        // only search if not revealed.
        //cout << "searching " << xArray[i] << ", " << yArray[i] << ".";
        search(xArray[i]+14, yArray[i]+4);
      }
    }
  }

}
// this is a Dumb Method
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
void initRevealBoard() {
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 9; j++) {
      revealedBoard[i][j] = 0;
    }
  }
}
void hover(int x, int y) {
  // changes highlight of whatever char is displayed.
  setcolor(240);
  gotoxy(x, y);
  cout << displayBoard[x-14][y-4];
  setcolor(15);
}

void dehover(int x, int y) {
  // reverts change.
  setcolor(15);
  gotoxy(x, y);
  cout << displayBoard[x-14][y-4];
}

void setcolor(WORD color){
  // set color of what is to be printed.
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
    return;
}

// updates the game table based on a user's click on correct pos.
void updateTable() {
  for (int i = 0; i < 9; i++) {
    gotoxy(14, i+4);
    for (int j = 0; j < 18; j++) {
      cout << displayBoard[j][i];
    }
  }
  

}

// sets the position of the console's cursor to the specified x,y
// is not the mouse cursor
void gotoxy(int x, int y) {
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void initBoard() {
  // init board. 0 is no mine, 1 is a mine.
  mines = 0;
  srand((unsigned) time(NULL));
  int randNum;
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 9; j++) {

      randNum = (rand() % 10) + 1; // gen num 1-4. 1 is mine. anything else is not.

      if (randNum == 1) {
        ++mines;
        gameBoard[i][j] = 1;
      } else {
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

void printBoardDebug() {
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
  cout << "            Welcome to Minesweeper!\n          Made by Brandon Frauenfeld\n";

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
  printBoardDebug();
  int prevX, prevY = 0; // prev mouse coords
  while(1) {
    // loop: read input first, then handle, then read, etc
    
    ReadConsoleInput(hIn, &InputRecord, 1, &Events); // to capture mouse pos in window

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 != 0) {
      int lMouseX = InputRecord.Event.MouseEvent.dwMousePosition.X;
      int lMouseY = InputRecord.Event.MouseEvent.dwMousePosition.Y;
      gotoxy(0, 2);
      cout << "lmb pressed at " << lMouseX << ", " << lMouseY << ".\n";
      search(lMouseX, lMouseY);
      updateTable();
    }

    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 != 0) {
      int rMouseX = InputRecord.Event.MouseEvent.dwMousePosition.X;
      int rMouseY = InputRecord.Event.MouseEvent.dwMousePosition.Y;
      gotoxy(0, 2);
      cout << "rmb pressed at " << rMouseX << ", " << rMouseY << ".\n";
    }
    
    if (MOUSE_EVENT) {
      // gather mouse coordinates
      int mouseX = InputRecord.Event.MouseEvent.dwMousePosition.X;
      int mouseY = InputRecord.Event.MouseEvent.dwMousePosition.Y;
      // check if mouse is in bounds of board
      if ((mouseX >= 14 && mouseX <= 31) && (mouseY >= 4 && mouseY <= 12)) {
        // check if prev coordinates are different. dehover prev coordinates if so
        if (prevX != mouseX || prevY != mouseY) {
          dehover(prevX, prevY);
          prevX = mouseX;
          prevY = mouseY;
        }
        gotoxy(0,3);
        // TODO debug output
        cout << "mouse in pos " << mouseX << ", " << mouseY << ".\n";
        gotoxy(0, 14);
        cout << "real board pos " << mouseX-14 << ", " << mouseY-4 << ".\n";
        hover(mouseX, mouseY);
      } else {
        // if mouse has exited, or is not in game board, dehover last known position
        dehover(prevX, prevY);
      }
    }
    
  }
}


