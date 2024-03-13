#include "WordTree.h"

//helper function to help swap two WordTrees
void WordTree::swap(WordTree& notThis){
    WordNode* temp = notThis.root;
    notThis.root = this->root;
    this->root = temp;
    temp = nullptr;
}

//helper function
void WordTree::newVal(WordNode*& node, WordType val, int num){
    node = new WordNode;
    node->m_data = val;
    node->m_num = num;
    node->m_left = nullptr;
    node->m_right = nullptr;
}


//helkper function for copy constructor
void WordTree::cpyHlp(WordNode* start, WordNode*& finish){
  //check for nullptr
    if (start == nullptr)
        return;
    newVal(finish, start->m_data, start->m_num);
    cpyHlp(start->m_left, finish->m_left);
    cpyHlp(start->m_right, finish->m_right);
}

//helper function for add
void WordTree::addHlp(WordType val, WordNode* node){
    
    //check if value is equal to, greater than or less than node's value
    if (val == node->m_data)
        node->m_num = node->m_num + 1;
    else if (val < node->m_data)
    {
        if (node->m_left == nullptr)
            newVal(node->m_left, val, 1);
        else
            addHlp(val, node->m_left);
            
    }
    else if (val > node->m_data)
    {
        if (node->m_right == nullptr)
            newVal(node->m_right, val, 1);
        else
            addHlp(val, node->m_right);
            
    }
}

//helper function for distinctWords
int WordTree::distHlp(WordNode* node) const{
    if (node != nullptr)
        return (1 + distHlp(node->m_left) + distHlp(node->m_right));
    else
        return 0;
}

//helper function for totalWords
int WordTree::totalHlp(WordNode* node) const{
    if (node != nullptr)
        return (node->m_num + totalHlp(node->m_left) + totalHlp(node->m_right));
    else
        return 0;
}


//helper for operator<<
void WordTree::outHlp(std::ostream& p_out, WordNode* node) const{
    if (node == nullptr)
        return;
    else
    {
        outHlp(p_out, node->m_left);
        p_out << node->m_data << " " << node->m_num << std::endl;
        outHlp(p_out, node->m_right);
    }
}

//helper for destructor
void WordTree::dstrHlp(WordNode* node){
    if (node == nullptr) return;
    dstrHlp(node->m_left);
    dstrHlp(node->m_right);
    delete node;
}


//start of implemented functions

WordTree::WordTree(const WordTree& rhs){
   
    //make sure it does not point to nullptr
    if (rhs.root != nullptr)
        cpyHlp(rhs.root, this->root);
    else
        this->root = nullptr;
    
    
}

const WordTree& WordTree::operator=(const WordTree& rhs){
    //check to make sure they are not the same
    if (this != &rhs)
    {
        //create new WordTree and call our helper function
        WordTree t(rhs);
        swap(t);
    }
    return *this;
}

void WordTree::add(WordType v){
    
    if (this->root == nullptr)
        newVal(this->root, v, 1);
    else
        addHlp(v, this->root);
}

int WordTree::distinctWords() const{
    //call helper function so that we can pass through a root parameter every call to the function
    return distHlp(this->root);
}

int WordTree::totalWords() const{
    //call helper function so that we can pass through a root parameter every call to the function
    return totalHlp(this->root);
}

std::ostream& operator<<(std::ostream &out, const WordTree& rhs){
    //call helper function and return the new out
    rhs.outHlp(out, rhs.root);
    return out;
}

WordTree::~WordTree(){
    dstrHlp(this->root);
}






