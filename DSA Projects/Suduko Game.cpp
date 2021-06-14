#include<bits/stdc++.h>
using namespace std;

#define N 9

bool findUnassignedLocation(vector<vector<int>>grid, int &row, int &col);
bool isSafe(vector<vector<int>>grid, int row, int col, int num);

/* assign values to all unassigned locations for Sudoku solution
 */
bool SolveSudoku(vector<vector<int>>grid)
{
    int row, col;
    if (!findUnassignedLocation(grid, row, col))
       return true;
    for (int num = 1; num <= 9; num++)
    {
        if (isSafe(grid, row, col, num))
        {
            grid[row][col] = num;
            if (SolveSudoku(grid))
                return true;
            grid[row][col] =0;
        }
    }
    return false;
}

/* Searches the grid to find an entry that is still unassigned. */
bool findUnassignedLocation(vector<vector<int>>grid, int &row, int &col)
{
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid[row][col] == 0)
                return true;
    return false;
}

/* Returns whether any assigned entry n the specified row matches
   the given number. */
bool usedInRow(vector<vector<int>>grid, int row, int num)
{
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/* Returns whether any assigned entry in the specified column matches
   the given number. */
bool usedInCol(vector<vector<int>>grid, int col, int num)
{
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/* Returns whether any assigned entry within the specified 3x3 box matches
   the given number. */
bool usedInBox(vector<vector<int>>grid, int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}

/* Returns whether it will be legal to assign num to the given row,col location.
 */
bool isSafe(vector<vector<int>>grid, int row, int col, int num)
{
    return !usedInRow(grid, row, num) && !usedInCol(grid, col, num) &&
           !usedInBox(grid, row - row % 3 , col - col % 3, num);
}

/* print grid  */
void printGrid(vector<vector<int>>grid)
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
            cout<<grid[row][col]<<"  ";
        cout<<endl;
    }
}

/* Main */
int main()
{
    vector<vector<int>>grid = {{3, 0, 6, 5, 0, 8, 4, 0, 0},
                      {5, 2, 0, 0, 0, 0, 0, 0, 0},
                      {0, 8, 7, 0, 0, 0, 0, 3, 1},
                      {0, 0, 3, 0, 1, 0, 0, 8, 0},
                      {9, 0, 0, 8, 6, 3, 0, 0, 5},
                      {0, 5, 0, 0, 9, 0, 6, 0, 0},
                      {1, 3, 0, 0, 0, 0, 2, 5, 0},
                      {0, 0, 0, 0, 0, 0, 0, 7, 4},
                      {0, 0, 5, 2, 0, 6, 3, 0, 0}};
    if (SolveSudoku(grid) == true)
          printGrid(grid);
    else
        cout<<"No solution exists"<<endl;
    return 0;
}
