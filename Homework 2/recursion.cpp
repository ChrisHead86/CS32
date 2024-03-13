

#include <iostream>

int multiplyTwoNumbers(unsigned int m, unsigned int n);
int tabulationStation(int num, int digit);
std::string highFives(std::string n);
std::string jheriCurls(std::string str);
bool aggregationNation(const int a[], int size, int target);
bool onMyWayHome(std::string maze[], int nRows, int nCols,
                 int sr, int sc, int er, int ec);

    int main()
    {

    }





// Returns the product of two non-negative integers, m and n,
// using only repeated addition.
int multiplyTwoNumbers(unsigned int m, unsigned int n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return m;
    
    int sum = m + multiplyTwoNumbers(m, n-1);
    return sum;
}
 
// Returns the number of occurrences of digit in the decimal
// representation of num. digit is an int between 0 and 9
// inclusive.
//
// Pseudocode Example:
//    tabulationStation(18838, 8) => 3
//    tabulationStation(55555, 3) => 0
//    tabulationStation(0, 0)     => 0 or 1 (either if fine)
//
int tabulationStation(int num, int digit)
{

    if (num == 0)
        return 0;
    
    if (num%10 == digit)
        return 1 + tabulationStation(num/10, digit);
    else
        return tabulationStation(num/10, digit);
        
    
}
 
// Returns a string where the same characters next each other in
// string n are separated by "55" (you can assume the input
// string will not have 5’s)
//
// Pseudocode Example:
//    highFives("goodbye") => "go55odbye"
//    highFives("yyuu")    => "y55yu55u"
//    highFives("aaaa")    => "a55a55a55a"
//
std::string highFives(std::string n)
{
    if (n.length() == 1 || n.length() == 0)
        return n;
    
    if (n.substr(0,1) == n.substr(1,1))
        return n.substr(0,1) + "55" + highFives(n.substr(1, n.length() - 1));
    
    else
        return n.substr(0,1) + highFives(n.substr(1, n.length() - 1));
    
}
 
// str contains a single pair of parentheses, return a new
// string made of only the parentheses and whatever those
// parentheses contain
//
//  Pseudocode Example:
//     jheriCurls("abc{ghj}789") => "{ghj}"
//     jheriCurls("{x}7")        => "{x}"
//     jheriCurls("4agh{y}")    => "{y}"
//     jheriCurls("4agh{}")     => "{}"
//
std::string jheriCurls(std::string str)
{
    if (str.length() == 0)
        return str;
    
    if (str.substr(0, 1) == "{")
    {
        if (str.substr(str.length() - 1, 1) == "}")
            return str;
        else
            return jheriCurls(str.substr(0, str.length() - 1));
    }
    else
        return jheriCurls(str.substr(1, str.length() - 1));
}
 
// Return true if the total of any combination of elements in
// the array a equals the value of the target.
//
//  Pseudocode Example:
//     aggregationNation({2, 4, 8}, 3, 10) => true
//     aggregationNation({2, 4, 8}, 3, 6)  => true
//     aggregationNation({2, 4, 8}, 3, 11) => false
//     aggregationNation({2, 4, 8}, 3, 0)  => true
//     aggregationNation({}, 0, 0)         => true
//
bool aggregationNation(const int a[], int size, int target)
{
    if (target == 0)
        return true;
    if (size == 0)
        return false;
    
    int new_targ = target - a[0];
    return (aggregationNation(a + 1, size - 1, new_targ)  ||  aggregationNation(a + 1, size - 1, target));
}

bool onMyWayHome(std::string maze[], int nRows, int nCols,
                 int sr, int sc, int er, int ec){
    
    //base case
    if (sr == er && sc == ec)
        return true;
    
    //keep track of where we have been
    maze[sr][sc] = '*';
    
    //try to move up, down, left, right
    if (maze[sr - 1][sc] == '.')
    {
        if(onMyWayHome(maze, nRows, nCols, sr - 1, sc, er, ec))
            return true;
    }
    if (maze[sr + 1][sc] == '.')
    {
        if(onMyWayHome(maze, nRows, nCols, sr + 1, sc, er, ec))
            return true;
    }
    if (maze[sr][sc - 1] == '.')
    {
        if(onMyWayHome(maze, nRows, nCols, sr, sc - 1, er, ec))
            return true;
    }
    if (maze[sr][sc + 1] == '.')
    {
        if(onMyWayHome(maze, nRows, nCols, sr, sc + 1, er, ec))
            return true;
    }
    
    //if no moves work, we return false
    return false;
    
}
