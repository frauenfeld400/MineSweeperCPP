/*
  Brandon Frauenfeld
  How quickly can I make a minesweeper clone in C++?
  Started: 6:27 PM, 7/29/2023
  Paused: 6:38 PM, 7/29/2023
  Resumed: 2:35 PM, 7/30/2023
  Paused: 2:52 PM, 7/30/2023
*/
#define _WIN32_WINNT 0x0500

#include <iostream>
#include <stdio.h>
#include <cstdlib>

#include <windows.h>

using namespace std;

int gameBoard[9][9];

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

void printBoardDebug() {
  for (int i = 0; i < 9; i++) {
    printf("\n");
    for (int j = 0; j < 9; j++) {
      cout << gameBoard[i][j] << " ";
    }
  }

}

int main() {
  HWND consoleWindow = GetConsoleWindow();

  SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) 
  & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX); // make window unresizeable by user

  RECT rect = {100, 100, 500, 500};
  MoveWindow(consoleWindow, rect.left, rect.top, rect.right-rect.left, 
  rect.bottom-rect.top, TRUE);  // set console window size
  
  cout << "Welcome to Minesweeper! Made by Brandon Frauenfeld in .";

  // code to remove scroll ability and scrollbar 
  // --------
  
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
    if (Status == 0)
    {
        cout << "SetConsoleScreenBufferSize() failed! Reason : " << GetLastError() << endl;
        exit(Status);
    }

    // print the current screen buffer dimensions
    GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);
    cout << "Screen Buffer Size : " << scrBufferInfo.dwSize.X << " x " << scrBufferInfo.dwSize.Y << endl;

    // --------


  initBoard();
  printBoardDebug();
  system("pause");
}


