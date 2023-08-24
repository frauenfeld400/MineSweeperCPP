# MineSweeperCPP

NOTICE: This only works on Windows. If you compile, and it does not work, disable Quick Edit mode in the console.
You should be able to click and play as any normal game.

Minesweeper written in C++.

As of 8/23/2023, this project is finished. I wanted to add sounds, but that might come in a V2. It's good to wrap things up
once they are finished, and not spend too much time on a proof of concept.

This is Minesweeper. You win by revealing all the tiles on the board that are not mines. You can right click to put down flags,
denoted by X's on the game board. Flags are used to keep track of potential mines. If you click on a mine, you lose, and have to
restart. Any numbers revealed represent the number of mines around that position.

Although fully functional, it is difficult to play because the text is small. I'd like to make a V2 sometime, with sounds,
difficulty, and a larger game board that is more fun to use. But that is for another day.

To compile, I used g++, with the following: 

g++ minesweeper.cpp -o minesweeper.exe -static

It is good practice not to trust random .exe's found, and compile things yourself. 
Alas, I have included an .exe for those who live dangerously. And the lazy.

--------------------------------------------------------------------------------------------------------------

Dev log and original description below:

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

As of 8/21/2023, the player can now win, but it needs fine tuning. The player can also place down flags. I have removed
many of the debug messages, and I don't think any should pop up anymore.
