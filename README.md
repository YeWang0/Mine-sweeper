# Mine-sweeper
Command-Line Minesweeper. Written in C++.

## Compile
g++ -std=c++11 minesweeper.cpp -o minesweeper

## Run
./minesweeper

## How to play
1. Set the size of game board 
2. Pick a square by inputting the row and column number
3. Then, you get a number. That number is the number of how many mines are surrounding it. If you find the mine, you can open "unopened" squares around it, opening more areas.
4. You lose if you pick a square of mine(first click always blank)
5. You win if all squares but mines are uncovered
