## Sudoku solving genetic algorithm with GUI build in SFML.
<p align="center">
  <img src="sudoku_gif.gif" alt="animated" />
</p>


## The algorithm works as follows:
1. We start by selecting an initial population of solutions. These solutions are created by randomly selecting an empty cell on the Sudoku board, searching for possible numbers to insert into that cell, and then randomly choosing one of them. If a cell has no possible numbers to insert, it remains empty.
2. Next, for each individual in the population, which consists of a Sudoku board solved in the manner mentioned above, we calculate an evaluation function. This function works by subtracting the number of empty cells for which there were no possible number insertions while maintaining the game rules from 100.
3. We then perform selection using a ranking method. Individuals with the highest evaluation function values are selected for reproduction, while the rest are removed from the population.
4. Reproduction involves selecting a random cell from one of the parents and, if possible, copying the value to the same cell in a new solution (child).
5. Mutation consists of ignoring the value of a cell from one of the parents and selecting a random possible value.
6. After a certain number of generations, if a complete solution has not been found, a reset occurs, and the entire evolution process starts from the beginning. This is necessary because solving NxN Sudoku puzzles is known to be NP-complete, so in the case of more challenging boards, the algorithm may get stuck at a locally optimal solution.


