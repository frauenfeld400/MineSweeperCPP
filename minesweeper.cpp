/*
  A Console-Based Minesweeper Clone
  Brandon Frauenfeld
  
*/

#include <iostream>
#include <stdio.h>
#include <cstdlib>

#include <windows.h>

using namespace std;

int gameBoard[9][9];


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
  // init board. 0 is no mine, 1 is a mine. limit of 20 mines.
  int mines = 20;
  srand((unsigned) time(NULL));
  int randNum;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {

      randNum = (rand() % 3) + 1; // gen num either 1, 2, or 3. 1 is no mine, 2 is mine, 3 is no mine

      if (randNum == 1) {
        gameBoard[i][j] = 0;
      } else if (randNum == 2) {
        gameBoard[i][j] = 1;
      } else {
        gameBoard[i][j] = 0;
      }
      
    }
  }
}

// prints a board that remains to be clicked.
// after this board is printed, init the board.
// ensure there is no mine where the player clicks.
void printInitialBoard() {
  // board begins at x=14, y=3. goto this pos, 
  // increment i instead of newline
  for (int i = 0; i < 9; i++) {
    gotoxy(14, i+3);
    cout << "? ? ? ? ? ? ? ? ?";
  }
}

void printBoardDebug() {
  for (int i = 0; i < 9; i++) {
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
  while(1) {
    // loop: read input first, then handle, then read, etc
    
    ReadConsoleInput(hIn, &InputRecord, 1, &Events); // to capture mouse pos in window

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 != 0) {
      int lMouseX = InputRecord.Event.MouseEvent.dwMousePosition.X;
      int lMouseY = InputRecord.Event.MouseEvent.dwMousePosition.Y;
      cout << "lmb pressed at " << lMouseX << ", " << lMouseY << ".\n";
    }

    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 != 0) {
      int rMouseX = InputRecord.Event.MouseEvent.dwMousePosition.X;
      int rMouseY = InputRecord.Event.MouseEvent.dwMousePosition.Y;
      cout << "rmb pressed at " << rMouseX << ", " << rMouseY << ".\n";
    }
    
  }
}


