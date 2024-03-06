

#include "Previous.h"
#include <iostream>
using namespace std;

Previous::Previous(int nRows, int nCols){
    m_row = nRows;
    m_col = nCols;
    
    
    //these for loops set the initial values of every location in the arena to 0 visits
    for (int j = 0; j < m_row; j++)
    {
        for (int k = 0; k < m_col; k++)
        {
            num_visits[j][k] = 0;
        }
    }
}


bool Previous::dropACrumb(int r, int c){
    
    if (r > m_row || r < 1 || c > m_col || c < 1)
        return false;
    
    num_visits[r-1][c-1]++;
    return true;
}

void Previous::showPreviousMoves() const{
    
    char character;
    
    clearScreen();
    
    for (int j = 0; j < m_row; j++)
    {
        for (int k = 0; k < m_col; k++)
        {
            if (num_visits[j][k] >= 26)
                cout << "Z";
            else if (num_visits[j][k] == 0)
                cout << ".";
            else
            {
                character = num_visits[j][k] + 64; //corresponds to the character for each number accoring to ASCII
                cout << character;
            }
        }
        cout << endl;
       
    }
    cout << endl;
    
    
}

