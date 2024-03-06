
#ifndef Previous_hpp
#define Previous_hpp

#include "globals.h"
#include <string>

class Previous
 {
   public:
     Previous(int nRows, int nCols);
     bool dropACrumb(int r, int c);
     void showPreviousMoves() const;
     void function(std::string);
   private:
     int m_row;
     int m_col;
     int num_visits[MAXROWS][MAXCOLS];
 };

#endif 
