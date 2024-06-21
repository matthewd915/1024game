/*
Author: Matthew Deng
Course: CSCI-135
Instructor: Tong Yi
Assignment: Project 3 Task A


*/

#include "Board.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

Board::Board() : Board(3, 3) {}

Board::Board(int m) : Board(m, m) {}

Board::Board(int m, int n)
{
    max = 0;
    target = 1024;
    if(m >= 1 && n >= 1)
    {
        numRows = m;
        numCols = n;
    }
    else
    {
        numRows = 3;
        numCols = 3;
    }
    panel = new int*[numRows];
    for(int i = 0; i < numRows; i++)
    {
        panel[i] = new int[numCols];
    }
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            panel[i][j] = 0;
        }
    }
}

Board::~Board()
{
    for(int i = 0; i < numRows; i++)
    {
        delete[] panel[i];
    }
    delete[] panel;
    panel = nullptr;
}

void Board::print() const
{
    cout << "+";
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            cout << "----+";
        }
        cout << "\n|";
        for(int j = 0; j < numCols; j++)
        {
            if(panel[i][j]!=0)
            {
                cout << setw(4);
                cout << panel[i][j];
            }
            else
            {
                cout << setw(5);
            }
            cout << "|";
        }
        cout << "\n+";
    }
    for(int i = 0; i < numCols; i++)
    {
        cout << "----+";
    }
    cout << endl;
}

int Board::getNumRows() const
{
    return numRows;
}

int Board::getNumCols() const
{
    return numCols;
}

int Board::getTarget() const
{
    return target;
}

int Board::getMax() const
{
    return max;
}

bool Board::noAdjacentSameValue() const
{
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols-1; j++)
        {
            if(panel[i][j]==panel[i][j+1])
            {
                return false;
            }
        }
    }
    for(int i = 0; i < numRows-1; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            if(panel[i][j]==panel[i+1][j])
            {
                return false;
            }
        }
    }
    return true;
}

struct Location
{
    int row;
    int col;
};

void Board::selectRandomCell(int& row, int& col)
{
    vector<Location> zeros;
    Location cell;
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            if(panel[i][j]==0)
            {
                cell.row = i;
                cell.col = j;
                zeros.push_back(cell);
            }
        }
    }

    if(zeros.size() > 0)
    {
        int index = rand() % zeros.size();
        row = zeros[index].row;
        col = zeros[index].col;
        panel[row][col] = 1;
        print();
    }

    if(zeros.size() == 0 || zeros.size() == 1)
    {
        if(noAdjacentSameValue())
        {
            if(max < target)
            {
                cout << "Game over. Try again." << endl;
            }
            else
            {
                cout << "Congratulations!" << endl;
                exit(0);
            }
        }
        else return;
    }
}

void merge(vector<int>& result, int& max)
{
    int i = 0;
    int size = result.size();
    while(i < size)
    {
        if(i+1 < size && result[i]==result[i+1])
        {
            result[i]*=2;
            result[i+1] = 0;
            if(result[i] > max)
            {
                max = result[i];
            }
            i+=2;
        }
        else
        {
            i++;
        }
    }
}

void Board::pressUp()
{
    for(int col = 0; col < numCols; col++)
    {
        vector<int> nonZeros;
        for(int row = 0; row < numRows; row++)
        {
            if(panel[row][col]!=0)
            {
                nonZeros.push_back(panel[row][col]);
            }
        }
        merge(nonZeros, max);
        vector<int> add;
        for(int i = 0; i < nonZeros.size(); i++)
        {
            if(nonZeros[i]!=0)
            {
                add.push_back(nonZeros[i]);
            }
        }
        for(int row = 0; row < add.size(); row++)
        {
            panel[row][col] = add[row];
        }
        for(int row = add.size(); row < numRows; row++)
        {
            panel[row][col] = 0;
        }
    }
    int r, c;
    selectRandomCell(r, c);
}

void Board::pressDown()
{
    for(int col = 0; col < numCols; col++)
    {
        vector<int> nonZeros;
        for(int row = 0; row < numRows; row++)
        {
            if(panel[row][col]!=0)
            {
                nonZeros.push_back(panel[row][col]);
            }
        }
        // reverse nonZeros
        vector<int> reverseNonZeros;
        for(int i = nonZeros.size()-1; i >= 0; i--)
        {
            reverseNonZeros.push_back(nonZeros[i]);
        }
        merge(reverseNonZeros, max); // 2 4 4 -> 2 8 0 || 1 1 1 -> 2 0 1 || 4 4 2 -> 8 0 2
        // rereverse nonZeros
        vector<int> add;
        int index = 0;
        for(int i = reverseNonZeros.size()-1; i >= 0; i--)
        {
            if(reverseNonZeros[i]!=0)
            {
                add.push_back(reverseNonZeros[i]);
            }
        }
        for(int row = 0; row < numRows-add.size(); row++)
        {
            panel[row][col] = 0;
        }
        for(int row = numRows-add.size(); row < numRows; row++)
        {
            panel[row][col] = add[index];
            index++;
        }
    }
    int r, c;
    selectRandomCell(r, c);
}

void Board::pressLeft()
{
    for(int row = 0; row < numRows; row++)
    {
        vector<int> nonZeros;
        for(int col = 0; col < numCols; col++)
        {
            if(panel[row][col]!=0)
            {
                nonZeros.push_back(panel[row][col]);
            }
        }
        merge(nonZeros, max); // 1 1 2 -> MERGE -> 2 0 2
        vector<int> add;
        for(int i = 0; i < nonZeros.size(); i++)
        {
            if(nonZeros[i]!=0)
            {
                add.push_back(nonZeros[i]);
            }
        }
        for(int col = 0; col < add.size(); col++)
        {
            panel[row][col] = add[col];
        }
        for(int col = add.size(); col < numCols; col++)
        {
            panel[row][col] = 0;
        }
    }
    int r, c;
    selectRandomCell(r, c);
}

void Board::pressRight()
{
    for(int row = 0; row < numRows; row++)
    {
        vector<int> nonZeros;
        for(int col = 0; col < numCols; col++)
        {
            if(panel[row][col]) // if the cell is not 0, append to nonZeros vector
            {
                nonZeros.push_back(panel[row][col]);
            }
        }
        // if row is {1, 1, 1}, pressRight() should make it {0, 1, 2}
        // vector = {1, 1, 1} merge() -> {2, 0, 1}
        // vector = {1, 1, 1} -> reverse -> {1, 1, 1} merge() -> {2, 0, 1} -> reverse -> {1, 0, 2}
        // vector = {2, 4, 4} merge() -> {2, 8, 0}
        // vector = {2, 4, 4} -> r -> {4, 4, 2} merge() -> {8, 0, 2} -> r -> {2, 0, 8}
        // reverse nonZeros
        vector<int> reverseNonZeros;
        for(int i = nonZeros.size()-1; i >= 0; i--)
        {
            reverseNonZeros.push_back(nonZeros[i]);
        }
        merge(reverseNonZeros, max);
        vector<int> add;
        int index = 0;
        for(int i = reverseNonZeros.size()-1; i >= 0; i--)
        {
            if(reverseNonZeros[i]!=0)
            {
                add.push_back(reverseNonZeros[i]);
            }
        }
        for(int col = 0; col < numCols-add.size(); col++)
        {
            panel[row][col] = 0;
        }
        for(int col = numCols-add.size(); col < numCols; col++)
        {
            panel[row][col] = add[index];
            index++;
        }
    }
    int r, c;
    selectRandomCell(r, c);
}

void Board::start()
{
    int round = 1;
    int r1, c1, r2, c2;
    selectRandomCell(r1, c1);
    selectRandomCell(r2, c2);
    char ch;
    while(true)
    {
        if(max >= target)
        {
            cout << "Congratulation!" << endl;
            break;
        }
        ch = getchar();
        if(ch=='S')
        {
            break;
        }
        if(ch=='\033')
        {
            getchar();
            switch(getchar())
            {
                case 'A':
                    cout << "Round " << setw(4) << round << ": ";
                    cout << "Press UP. " << "Goal: " << target << endl;
                    pressUp();
                    round++;
                    break;
                case 'B':
                    cout << "Round " << setw(4) << round << ": ";
                    cout << "Press DOWN. " << "Goal: " << target << endl;
                    pressDown();
                    round++;
                    break;
                case 'C':
                    cout << "Round " << setw(4) << round << ": ";
                    cout << "Press RIGHT. " << "Goal: " << target << endl;
                    pressRight();
                    round++;
                    break;
                case 'D':
                    cout << "Round " << setw(4) << round << ": ";
                    cout << "Press LEFT. " << "Goal: " << target << endl;
                    pressLeft();
                    round++;
                    break;
            }
        }
    }
}