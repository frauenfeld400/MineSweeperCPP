# MineSweeperCPP
Minesweeper written in C++.

NOTICE: This only works on Windows. If you compile, and the .exe does not work, disable Quick Edit mode in your console.

Originally a project to determine how quickly I could make an extremely simple minesweeper clone in C++,
this project evolved into me wanting to create a fully functioning text-based minesweeper clone, and 
trying to do things in a console environment that I have never done before.

Upon completion, it should play entirely like minesweeper does, with mouse input and sounds. 

In working on this project, I have learned how to utilize windows.h to make a functional console-based
game. The user can click around in the console, and has no freedom in resizing or scrolling in the console.
The user can also hover over the board, which highlights what is being displayed, so they know where they are
actually clicking.

I have come up with a recursive algorithm to reveal empty tiles. When the user clicks, 
0. If the user clicked on the mine, game over.
1. Check all 8 squares around the origin (and the origin itself)
2. For each position, check if there is a mine. If not, log the position in two arrays, xArray and yArray.
3. At the end, if even a single mine has been found, set the current tile to how many mines have been detected, return.
4. If no mines found, recursively call the algorithm 8 times with each coordinate pair given by xArray and yArray.

Upon completion, all empty tiles adjacent to the origin will be revealed, and any empty ones along them, too.

As of 8/20/2023, the game is "functional", but lacks a game over screen, reset button, flags, and win condition.
It also first displays all the mines to the user for testing. 

I would still like to add all of that. 

As of 8/21/2023, the player can now lose, there is a reset button, and a debug button. There is no win condiiton yet.
The searching algorithm now more closely mimics the original.

To compile, I used g++, with the following: 

g++ minesweeper.cpp -o minesweeper.exe -static

It is good practice not to trust random .exe's found, and compile things yourself. 

You can also live dangerously, if you want. So, I will include the .exe when it's all done.
