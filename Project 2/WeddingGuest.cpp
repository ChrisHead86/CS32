
#include "WeddingGuest.h"

WeddingGuest::WeddingGuest(){
    head = nullptr;
    tail = nullptr;
    m_num = 0;
}

WeddingGuest::~WeddingGuest(){
    Node *p = head;
    while (head != nullptr)
    {
        p = head;
        head = head->next;
        delete p;
    }
}

WeddingGuest::WeddingGuest(const WeddingGuest& that){
    

    
    
    //case of that being empty
    if (that.m_num == 0)
    {
        head = nullptr;
        tail = nullptr;
        m_num = 0;
    }
    else
    {
        //we create the head node and then copy the rest of the nodes
        head = nullptr;
        m_num = that.m_num;
        
        head = new Node;
        head->prev = nullptr;
        
        Node *p = head;
        Node *c = that.head;
        
        //copy rest of nodes except for tail
        while(c->next != nullptr)
        {
            Node* kd = new Node;
            p->value = c->value;
            p->first = c->first;
            p->last = c->last;
            p->next = kd;
            kd->prev = p;
            p = kd;
            c = c->next;
        }
        
        //copy tail
        tail = p;
        p->next = nullptr;
        p->value = that.tail->value;
        p->first = that.tail->first;
        p->last = that.tail->last;
        
        
    }
    
}

WeddingGuest& WeddingGuest::operator=(const WeddingGuest& that){

    //if statement makes sure this behaves correctly in the face of aliasing
    if (this != &that)
    {
        WeddingGuest temp(that);
        swapWeddingGuests(temp);
    }
    return *this;
}

bool WeddingGuest::noGuests() const{
    if (head == nullptr)
        return true;
    else
        return false;
}

int WeddingGuest::guestCount() const{
    return m_num;
}

bool WeddingGuest::inviteGuest(const std::string& firstName, const std::string&
                 lastName, const GuestType& value){
    
    //if name is already on list-return false
   if (invitedToTheWedding(firstName, lastName))
        return false;
    
    
    
    //if list is empty, add to front
    if(m_num == 0)
    {
        head = new Node;
        head->first = firstName;
        head->last = lastName;
        head->value = value;
        head->next = nullptr;
        head->prev = nullptr;
        tail = head;
        m_num++;
        return true;
    }
    
    //creates a pointer that recurses through the list to find where to put the name
    Node *p = head;
    while (p != nullptr)
    {
        //case of same last name
        if (p->last == lastName)
        {
            
            if (p->next != nullptr)
            {
            //create a new pointer to see how many people have this same last name- pointer ends pointing at the last person with the  last name
            Node *c = p;
            
            
            while (c->next != nullptr && c->next->last == lastName)
            {
                c = c->next;
            }
            
            //while loop goes until we reach the last person with this last name, or we find someone whose first name should come after our new guest's
            while (p != c && p->first <= firstName)
            {
                p = p->next;
            }
            }
            
            //case where we want to put our new guest at the end of this list of people with this last name
            if (p->first <= firstName)
            {
                //case if we want to put name at the end of the list
                if(p->next == nullptr)
                {
                    p->next = new Node;
                    p->next->prev = p;
                    p->next->next = nullptr;
                    Node *dude = p->next;
                    dude->value = value;
                    dude->first = firstName;
                    dude->last = lastName;
                    tail = dude;
                    break;
                }
                Node *t = p->next;
                p->next = new Node;
                p->next->prev = p;
                p->next->next = t;
                t->prev = p->next;
                Node *dude = p->next;
                dude->value = value;
                dude->first = firstName;
                dude->last = lastName;
                break;
            }
            else if (firstName <= p->first)
            {
                //if we did not get to the last person with this last name
                
                //if we want to put the name at the front of the list
                if (p->prev == nullptr)
                {
                    p->prev = new Node;
                    p->prev->next = p;
                    p->prev->prev = nullptr;
                    Node *dude = p->prev;
                    dude->value = value;
                    dude->first = firstName;
                    dude->last = lastName;
                    head = dude;
                    break;
                }
                Node *t = p->prev;
                p->prev = new Node;
                p->prev->next = p;
                p->prev->prev = t;
                t->next = p->prev;
                Node *dude = p->prev;
                dude->value = value;
                dude->first = firstName;
                dude->last = lastName;
                break;
            }
            
            

            
            
        }
        else if (lastName <= p->last)
        {
            //if last names are different, and our name comes somewhere before the end of the list
            
            //if our name goes at the front of the list
            if (p->prev == nullptr)
            {
                p->prev = new Node;
                p->prev->next = p;
                p->prev->prev = nullptr;
                Node *dude = p->prev;
                dude->value = value;
                dude->first = firstName;
                dude->last = lastName;
                head = dude;
                break;
            }
            
            Node *t = p->prev;
            p->prev = new Node;
            p->prev->next = p;
            p->prev->prev = t;
            t->next = p->prev;
            Node *dude = p->prev;
            dude->value = value;
            dude->first = firstName;
            dude->last = lastName;
            if (dude->prev == nullptr)
                head = dude;
            break;
        }
        else if (p->next == nullptr)
        {
            //if last names are different, and we put our name at the end of the list
            Node *t = p->next;
            p->next = new Node;
            p->next->prev = p;
            p->next->next = t;
            Node *dude = p->next;
            dude->value = value;
            dude->first = firstName;
            dude->last = lastName;
            tail = dude;
            break;
        }
            p = p->next;
    }
    
    m_num++;
    return true;

  
}


bool WeddingGuest::alterGuest(const std::string& firstName, const std::string&
                lastName, const GuestType & value){
   
    //return false if name is not on list
    if (!invitedToTheWedding(firstName,lastName))
        return false;
    
    //recurse through list and change value when we get to name
    Node *p = head;
    
    while (p != nullptr)
    {
        if (p->first == firstName && p->last == lastName)
        {
            p->value = value;
            return true;
        }
        p = p->next;
    }
    return false;
    
}


bool WeddingGuest::inviteOrAlter(const std::string& firstName, const
                   std::string& lastName, const GuestType& value){

    //we try to alter the name, if that fials, we invite, return true in either case
    if(alterGuest(firstName,lastName,value))
    {
        return true;
    }
    
    inviteGuest(firstName,lastName,value);
    return true;
    
}


bool WeddingGuest::crossGuestOff(const std::string& firstName, const
                   std::string& lastName){
   
    //if name is not on list, return false
    if (!invitedToTheWedding(firstName,lastName))
        return false;
    //return false if list is empty
    if (m_num == 0)
        return false;
    
    //recurse through list and stop when we get to the node with the name we want
    Node *p = head;
    while (p != nullptr)
    {
        if (p->first == firstName && p->last == lastName)
        {
            //case that there is only one item in the list and it is the name we want to cross off
            if (p == head && p == tail)
            {
                delete p;
                head = nullptr;
                tail = nullptr;
                m_num--;
                return true;
            }
            else if (p == head)
            {
                //we cross off the head
                head = p->next;
                head->prev = nullptr;
            }
            else if (p == tail)
            {
                //we cross off the tail
                tail = p->prev;
                tail->next = nullptr;
            }
            else
            {
                //every other case- we cross off a name in the middle of the list
                p->next->prev = p->prev;
                p->prev->next = p->next;
            }
            m_num--;
            delete p;
            return true;
        }
        
        p = p->next;

    }
    
    return false;
    
    
    
}





bool WeddingGuest::invitedToTheWedding(const std::string& firstName, const
                                       std::string& lastName) const{
    //recurse through list until/if we find name
    Node *p = head;
    while (p != nullptr)
    {
        if (p->first == firstName && p->last == lastName)
            return true;
        p = p->next;
    }
    return false;
}



bool WeddingGuest::matchInvitedGuest(const std::string& firstName, const
                       std::string& lastName, GuestType& value) const{
    
    //return false if name is not on list
    if (!invitedToTheWedding(firstName,lastName))
        return false;
    
    //recurse through list and assign values accordingly when we reach the desired name
    Node *p = head;
    
    while (p != nullptr)
    {
        if (p->first == firstName && p->last == lastName)
        {
            value = p->value;
            return true;
        }
        p = p->next;
    }
    
    return false;
    
}




bool WeddingGuest::verifyGuestOnTheList(int i, std::string& firstName,
                          std::string& lastName, GuestType & value) const{
    
    //make sure i is in bounds
    if (i < 0 || i >= m_num)
        return false;
    
    //recurse until we get to desired node
    Node *p = head;
    for (int j = 0; j < i; j++)
    {
        p = p->next;
    }
    
    //assign values
    firstName = p->first;
    lastName = p->last;
    value = p->value;
    return true;
    
}


void WeddingGuest::swapWeddingGuests(WeddingGuest& other){
    
    //we swap the heads, then the tails, then the sizes
    
    Node *h = head;
    head = other.head;
    other.head = h;
    
    Node *t = tail;
    tail = other.tail;
    other.tail = t;
    
    int swap = m_num;
    m_num = other.m_num;
    other.m_num = swap;
    
    
    
}
 


bool joinGuests(const WeddingGuest & odOne,
                const WeddingGuest & odTwo,
                WeddingGuest & odJoined){
    bool isVal = true;
    
    //testing if one or both original lists are empty
    if (odOne.guestCount() == 0 && odTwo.guestCount() != 0)
    {
        odJoined = odTwo;
        return true;
    }
    
    
    if (odTwo.guestCount() == 0 && odOne.guestCount() != 0)
    {
        odJoined = odOne;
        return true;
    }
    
    if (odOne.guestCount() == 0 && odTwo.guestCount() != 0)
    {
        odJoined = odOne;
        return true;
    }
    
    
    //create variables to check for name and set joined list equal to the first list
    std::string firstName;
    std::string lastName;
    GuestType value1;
    GuestType value2;
    odJoined = odOne;
    
    // this loop eliminates each name that does not belong on the final list, and adds names from the second list that do
    for (int i = 0; odTwo.verifyGuestOnTheList(i, firstName, lastName, value1); i++)
    {
        if (!odJoined.inviteGuest(firstName, lastName, value1))
        {
            odJoined.matchInvitedGuest(firstName, lastName, value2);
            if (value1 != value2)
            {
                odJoined.crossGuestOff(firstName, lastName);
                isVal = false;
            }
        }
    }
    
    return isVal;
    
}


void attestGuests (const std::string& fsearch,
                   const std::string& lsearch,
                   const WeddingGuest& odOne,
                   WeddingGuest& odResult){
    
    bool f_wild = false;
    bool l_wild = false;
    std::string firstName;
    std::string lastName;
    GuestType value;
    
    //set booleans to align with wildcards
    if (fsearch == "*")
        f_wild = true;
    if (lsearch == "*")
        l_wild = true;
    
    //set result list equal to first list
    odResult = odOne;
    
    //case: if both names are wildcards
    if (f_wild && l_wild)
        return;
    
    //case: if first name is wildcard but last name isn't
    if (f_wild && !l_wild)
    {
        for (int i = 0; i < odResult.guestCount(); i++)
        {
            odResult.verifyGuestOnTheList(i, firstName, lastName, value);
            if (lastName != lsearch)
            {
                odResult.crossGuestOff(firstName, lastName);
                i--;
            }
        }
        return;
    }
    
    //case: if last name is wildcard, but first name isn't
    if (!f_wild && l_wild)
    {
        for (int i = 0; i < odResult.guestCount(); i++)
        {
            odResult.verifyGuestOnTheList(i, firstName, lastName, value);
            if (firstName != fsearch)
            {
                odResult.crossGuestOff(firstName, lastName);
                i--;
            }
        }
        return;
    }
    
    //case: if neither name is wildcard
    if (!f_wild && !l_wild)
    {
        for (int i = 0; i < odResult.guestCount(); i++)
        {
            odResult.verifyGuestOnTheList(i, firstName, lastName, value);
            if (firstName != fsearch || lastName != lsearch)
            {
                odResult.crossGuestOff(firstName, lastName);
                i--;
            }
        }
        return;
    }
    
    return;
    
}

/*
//funciton explained in header file
bool comeBefore(const std::string& str1, const std::string& str2 ){
    std::string s1 = str1;
    std::string s2 = str2;
    
    for (int i = 0; i < s1.length(); i++)
    {
       s1.at(i) = tolower(s1.at(i));
    }
    
    for (int i = 0; i < s2.length(); i++)
    {
        s2.at(i) = tolower(s2.at(i));
    }
    
    if (s1 <= s2)
        return true;
    else
        return false;
    
}*/
