//

#include "LinkedList.h"
using namespace std;
#include <iostream>

//default constructor
LinkedList::LinkedList(){
    head = nullptr;
}

LinkedList::LinkedList(const LinkedList &rhs){
    
    if (rhs.head == nullptr)
    {
        head = nullptr;
        return;
    }
    
    head = new Node;
    head->value = rhs.head->value;
    Node *p = rhs.head;
    Node *c = head;
  
    while(p->next != nullptr)
    {
        c->next = new Node;
        c->next->value = p->next->value;
        c = c->next;
        p = p->next;
    }
    
    c->next = nullptr;
    
    
}

LinkedList::~LinkedList(){
    Node *p;
    p = head;
    while (p != nullptr)
    {
        Node *n = p->next;
        delete p;
        p = n;
    }
}

const LinkedList& LinkedList::operator=(const LinkedList& rhs){
   
    if (&rhs == this)
        return *this;
    
    Node *a;
    a = head;
    while (a != nullptr)
    {
        Node *n = a->next;
        delete a;
        a = n;
    }
    
    head = new Node;
    head->value = rhs.head->value;
    Node *p = rhs.head;
    Node *c = head;
    
    while(p->next != nullptr)
    {
        c->next = new Node;
        c->next->value = p->next->value;
        c = c->next;
        p = p->next;
    }
    
    c->next = nullptr;
    return *this;
    
}





void LinkedList::insertToFront(const ItemType &val){
    Node *p = new Node;
    p->value = val;
    p->next = head;
    head = p;
}


void LinkedList::printList() const{
    Node *&p = head;
    while (p != nullptr)
    {
        cout << p->value << " ";
        p = p->next;
    }
    cout << endl;
}


bool LinkedList::get(int i, ItemType& item) const{

    if (i > size()-1 || i < 0 || size() == 0)
        return false;
    
    Node *temp = head;
    int index = 0;
    while (index != i)
    {
        temp = temp->next;
        index++;
    }
    
    item = temp->value;
    return true;
    
}

void LinkedList::reverseList(){
   
    Node *p = head;
    Node *n = head;
    n = n->next;
    head->next = nullptr;
    Node *t;
    
    while (n != nullptr)
    {
        t = n;
        n = n->next;
        t->next = p;
        p = t;
    }
    head = p;
}

void LinkedList::printReverse() const{
    
    LinkedList temp(*this);
    temp.reverseList();
    temp.printList();
    
 }

void LinkedList::append(const LinkedList &other){
    if (head == nullptr)
        *this = other;
    else
    {
    Node *t = head;
    while (t->next != nullptr)
    {
        t = t->next;
    }
    
    Node *t2 = other.head;
    
    while (t2 != nullptr)
    {
        t->next = new Node;
        t->next->value = t2->value;
        t = t->next;
        t2 = t2->next;
        
    }
    
    t->next = nullptr;
    
}
}

void LinkedList::swap(LinkedList &other){
    Node *p = other.head;
    other.head = head;
    head = p;
    p = nullptr;
}


int LinkedList::size() const{
   
    int counter = 0;
    Node *p = head;
    while (p != nullptr)
    {
        counter++;
        p = p->next;
    }
    
    return counter;
    
};
