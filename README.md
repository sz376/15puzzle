Solves the 15-puzzle problem using the A* search algorithm. Solving a 15-puzzle problem needs to keep swapping the locations of the blank tile and a tile adjacent to it (i.e., above/below it or to the left/right of the blank title), such that in the end all the tiles are moved from their initial locations to their goal locations. For example, the command
```
./major_hw2 2 3 0 4 1 6 7 8 5 9 10 12 13 14 11 15
```
is to solve a 15-puzzle problem, in which the tiles are initially placed as follows, and are to be moved to their goal locations.

GenGemPuzzle.c is used to generate initial states. The program starts from the goal state (i.e., all the tiles are at their goal locations) and moves tiles randomly. It takes an integer as its argument (example shown below), which is the number of moves it makes before it finishes.
```
./GenGemPuzzle 20
```
