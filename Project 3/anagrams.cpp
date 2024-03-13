#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
using namespace std;
const int MAXRESULTS   = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in


int lexiconBuilder(ifstream &dictfile, string dict[]);
int theJumbler(string word, const string dict[], int size,
string results[]);
void divulgeSolutions(const string results[], int size);


//helper functions
int lexCount(ifstream &dictfile, string dict[], int &words);
void perms(string word, int begin, int end, string dict[], int size, string perms[], int& track);
void permsHelping(string word, int begin, string finish[], int &track);
void perms(string word, int begin, int end, const string dict[], int size, string finish[], int& track);






int main()
{
    string results[0];
    string dict[MAXDICTWORDS];
    ifstream dictfile;         // file containing the list of words
    int nwords;                // number of words read from dictionary
    string word;
    dictfile.open("/Users/chrishead86/Downloads/words.txt");
    if (!dictfile) {
        cout << "File not found!" << endl;
        return (1);
    }
    cout << "worked" << endl;
   nwords = lexiconBuilder(dictfile, dict);
    cout << dict[2345] << endl;
    cout << nwords << endl;
    cout << "Please enter a string for an anagram: ";
    cin >> word;
    int numMatches = theJumbler(word, dict, nwords, results);
    if (!numMatches) {
        cout << "No matches found" << endl;
}
    else {
        divulgeSolutions(results, numMatches);
}
return 0;
}











int lexiconBuilder(ifstream &dictfile, string dict[]){
    
    //create a helper function so that I can keep track of the number of words
    int words = 0;
    return lexCount(dictfile, dict, words);
}



int theJumbler(string word, const string dict[], int size,
           string results[]){
    
    //call helper function described later in code
    int x = 0;
    perms(word, 0, word.size(), dict, size, results, x);
    return x;
    
}




void divulgeSolutions(const string results[], int size){
    if (size <= 0)
        return;
    else
    {
        cout << results[0] << endl;
        divulgeSolutions(results + 1, size - 1);
    }
}



//helper functions


//helper function for lexiconBuilder
int lexCount(ifstream &dictfile, string dict[], int &words){
    
    if (words == MAXDICTWORDS)
        return MAXDICTWORDS;
    
    if (!getline(dictfile, dict[words]))
        return words;
    
    
    return lexCount(dictfile, dict, ++words);
    
    
}


//ended up not using this
/*//helper function that finds the sum of a words ASCII values
int sumAs(string word){
    //if only one letter left, return that letter
    if (word.length() == 1)
        return word[0];
    
    //recursively add every letter to eachother
    return (word[0] + sumAs(word.substr(1)));
}*/


//swap helper function to swap 2 letters for permutations
void swap(char &char1, char &char2){
    char temp = char2;
    char2 = char1;
    char1 = temp;
}

//helper functions for permutation function
void permsHelp(string word, int begin, int end, int perm, const string dict[], int size, string finish[], int& track){
    
    //base case
    if (begin == end)
        return;
    
    //initiates permutation- recurses back to perms funtion until a fully permutated word is passed through
    swap(word[perm], word[begin]);
    perms(word, perm + 1, end, dict, size, finish, track);
    swap(word[perm], word[begin]);
    permsHelp(word, begin + 1, end, perm, dict, size, finish, track);
    
}

void permsHelper(string word, int begin, const string dict[], string finish[], int size, int &track){
    
    //base case
    if (begin == size)
        return;

    //if permuted word is equal to the word we are comparing to, move to helper function that attempts to place in final array
    else if (word == dict[begin])
        permsHelping(word, 0, finish, track);
    
    permsHelper(word, begin + 1, dict, finish, size, track);
    
}

void permsHelping(string word, int begin, string finish[], int &track){
    
    //base case
    if (begin == MAXRESULTS)
        return;
    
    //case duplicate
    else if (finish[begin] == word)
        return;
    
    //case that the word matches and has not been stored yet, store in spot with an empty string
    else if (finish[begin] == "")
    {
        finish[begin] = word;
        track++;
        return;
    }
    
    permsHelping(word, begin + 1, finish, track);
    
}


void perms(string word, int begin, int end, const string dict[], int size, string finish[], int& track){
    
    //base case- word has a permutation- moves on to be checked if permutation matches word we are comparing to
    if (begin == end)
        permsHelper(word, 0 , dict, finish, size, track);
    
    //if word is still permutating, recurse back to helper function
    permsHelp(word, begin, end, begin, dict, size, finish, track);
    
    
}
