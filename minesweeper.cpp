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

int gameBoard[18][9]; // board pos is 14, 4 to 31, 12.
char displayBoard[18][9]; // what is displayed to user
int mines;

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

      randNum = (rand() % 3) + 1; // gen num either 1, 2, or 3. 1 is no mine, 2 is mine, 3 is no mine

      if (randNum == 1) {
        gameBoard[i][j] = 0;
      } else if (randNum == 2) {
        ++mines; // add to mines count
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
  for (int i = 0; i < 18; i++) {
    printf("\n");
    for (int j = 0; j < 9; j++) {
      cout << gameBoard[i][j] << " ";
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
  int prevX, prevY = 0; // prev mouse coords
  while(1) {
    // loop: read input first, then handle, then read, etc
    
    ReadConsoleInput(hIn, &InputRecord, 1, &Events); // to capture mouse pos in window

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 != 0) {
      int lMouseX = InputRecord.Event.MouseEvent.dwMousePosition.X;
      int lMouseY = InputRecord.Event.MouseEvent.dwMousePosition.Y;
      gotoxy(0, 2);
      cout << "lmb pressed at " << lMouseX << ", " << lMouseY << ".\n";
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
        hover(mouseX, mouseY);
      } else {
        // if mouse has exited, or is not in game board, dehover last known position
        dehover(prevX, prevY);
      }
    }
    
  }
}


