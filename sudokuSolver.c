#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int system (const char* command);

int boardSize;
int subBoard;
// Stores entries for sudoku
int table[25][25];

void solveSudoku(int row, int column);

int AreEqual(char str1[], char str2[]) {  // Checks if two strings are equal
  if (strlen(str1) != strlen(str2)) {
    return 0;
  }
  int i;
  for (i = 0; str1[i] != '\0'; i++) {
    if (str1[i] != str2[i]) {
      return 0;
    }
  }
  return 1;
}

void start() {
    char name[20];
    char size[1];
    int sizes;
    printf("Sudoku Board Sizes:\na. 1\nb. 4\nc. 9\nd. 16\ne. 25\n\nEnter letter: ");
    scanf("%s", size);
    if (AreEqual(size, "a")) {
        sizes = 1;
    }else if (AreEqual(size, "b")) {
        sizes = 4;
    }else if (AreEqual(size, "c")) {
        sizes = 9;
    }else if (AreEqual(size, "d")) {
        sizes = 16;
    }else if (AreEqual(size, "e")) {
        sizes = 25;
    }
    boardSize = sizes;
    int grid[25][25];
    printf("\nEnter File Name: ");
    scanf("%s", name);
    FILE* fp = fopen(name, "r");
    subBoard = (sqrt(boardSize));

    if (fp == NULL) {
        printf("\nError: No Such File or Directory!\n\n");
        system("pause");
        exit(1);
    } else {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                fscanf(fp, "%d", &grid[i][j]);
            }
        }
        memcpy(table, grid, sizeof(table));
        solveSudoku(0, 0); // The solving starts by passing parameters
        exit(1);
    }
    fclose(fp);
    exit(1);
}

// Prints the solved Sudoku
void solutionPrint() {
    char save[20];
    printf("\nSave Results to: ");
    scanf("%s", save);
    FILE* ofp = fopen(save, "w");
    int countAcross = 0;

    if (boardSize == 9) {
        fprintf(ofp, "The Solution of the Sudoku Puzzle is: \n");
        fprintf(ofp, "\n+-----------+-----------+-----------+\n");
        for (int row = 0; row < boardSize; row++) {
            fprintf(ofp, "|");
            for (int column = 0; column < boardSize; column++) {
                    fprintf(ofp, " %d |", table[row][column]);
            }

            countAcross++;
            if (countAcross == 3 || countAcross == 6) {
                fprintf(ofp, "\n|===========#===========#===========|");
            }else if (countAcross != 9){
                fprintf(ofp, "\n|---+---+---|---+---+---|---+---+---|");
            }
            fprintf(ofp, "\n");
        }
        fprintf(ofp, "+-----------+-----------+-----------+\n");
        fclose(ofp);
        system(save);
        exit(1);
    }else {
        fprintf(ofp, "The Solution of the Sudoku Puzzle is: \n");
        fprintf(ofp, "\n+-------------------+-------------------+-------------------+-------------------+\n");
        for (int row = 0; row < boardSize; row++) {
            fprintf(ofp, "|");
            for (int column = 0; column < boardSize; column++) {
                fprintf(ofp, " %02d |", table[row][column]);
            }
            //countDown = 0;
            countAcross++;
            if (countAcross == 4 || countAcross == 8 || countAcross == 12) {
                fprintf(ofp, "\n|===================#===================#===================#===================|");
            }else if (countAcross != 16){
                fprintf(ofp, "\n|----+----+----+----|----+----+----+----|----+----+----+----|----+----+----+----|");
            }
            fprintf(ofp, "\n");
        }
        fprintf(ofp, "+-------------------+-------------------+-------------------+-------------------+\n");
        fclose(ofp);
        system(save);
        exit(1);
    }
}

// Checks if no conflict in rows
bool rowCheck(int row, int n) {
    for(int column = 0; column < boardSize; column++) {
        if(table[row][column] == n) {
            return false; // if number is already found in the row
        }
    }
    return true; // number can be one of the solutions in the row
}

// Checks if no conflict column
bool columnCheck(int column, int num) {
    for(int row = 0; row < boardSize; row++) {
        if(table[row][column] == num) {
            return false; // if number is already found in the column
        }
    }
    return true; // number can be one of the solutions in the column
}

// Checks if no conflict in a 3x3 box
bool boxCheck(int row, int column, int n) {
    row = ((row / subBoard) * subBoard) ;
    column = (column / subBoard) * subBoard;
    for(int r = 0; r < subBoard; r++) {
        for(int c = 0; c < subBoard; c++) {
            if (table[row + r][column + c] == n) {
                return false; // if number is already found in the 3x3 grid
            }
        }
    }
    return true; // number can be one of the solutions in the 3x3 grid
}

// Move to next cell when its filled already
void nextCell(int row, int column) {
    if(column < (boardSize - 1)) {
        solveSudoku(row, (column + 1));
    }else {
        solveSudoku((row + 1), 0);
   }
}

// Solution for the puzzle by Backtracking
void solveSudoku(int row, int column) {
    if (row == boardSize) { // Means that all cells are filled
        solutionPrint();
    }
    if (table[row][column] != 0) { // If a cell is not a zero, it means we have to move to the next cell
        nextCell(row, column);
    }else {
        for (int counter = 1; counter <= boardSize; counter++) { // Checks if numbers 1-9 can be put on a cell
            if ((rowCheck(row, counter) == true) && (columnCheck(column, counter) == true) && (boxCheck(row, column, counter) == true)) { // If looks promising
                table[row][column] = counter; // Make tentative assignment
                nextCell(row, column);
            }
        }
        table[row][column] = 0; // If doesn't lead to a solution, reset to 0. This triggers the backtracking
        // Note this backtracking step, a very important step**
        // We come at this position, this step, this line when we have already checked all possible values at 
        // sudoku[i][j] and we couldn't find the solution
        // Put any value does not solves our board implies that we must have made wrong choice earlier
        // so we make this sudoku[i][j] again a vacant cell and try to correct our previous guesses/choices.
    }
}

int main() {
    start();
    return 0;
}