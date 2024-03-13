

#ifndef WordTree_h
#define WordTree_h

#include <stdio.h>
#include <iostream>
#include <string>
         
typedef std::string WordType;
         
struct WordNode {
   WordType m_data;
   WordNode *m_left;
   WordNode *m_right;
   int m_num;
   // You may add additional data members and a constructor
   // in WordNode
};
         
class WordTree {
   private:
      WordNode *root;
    //helper functions- almost every function is named after the function it is helping
    void swap(WordTree& notThis);
    void cpyHlp(WordNode* start, WordNode*& finish);
    void newVal(WordNode*& node, WordType val, int num);
    void addHlp(WordType val, WordNode* node);
    int distHlp(WordNode* node) const;
    int totalHlp(WordNode* node) const;
    void outHlp(std::ostream& p_out, WordNode* node) const;
    void dstrHlp(WordNode* node);
    
  public:
    // default constructor
    WordTree() : root(nullptr) { };
     
    // copy constructor
    WordTree(const WordTree& rhs);
     
    // assignment operator
    const WordTree& operator=(const WordTree& rhs);
 
    // Inserts v into the WordTree
    void add(WordType v);

    // Returns the number of distinct words / nodes

    int distinctWords() const;

    // Returns the total number of words inserted, including
    // duplicate values
    int totalWords() const;

    // Prints the WordTree
    friend std::ostream& operator<<(std::ostream &out, const
                                    WordTree& rhs);

    // Destroys all the dynamically allocated memory in the
    // tree
    ~WordTree();
};



#endif /* WordTree_h */




