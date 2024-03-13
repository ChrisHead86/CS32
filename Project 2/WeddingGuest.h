
#ifndef WeddingGuest_h
#define WeddingGuest_h

#include <string>

typedef std::string GuestType; // This can change to other types such
                               // as double and int, not just string


 
class WeddingGuest
{
  public:
    WeddingGuest();// Create an empty WeddingGuest list
    ~WeddingGuest();
    WeddingGuest(const WeddingGuest& that);
    WeddingGuest& operator=(const WeddingGuest& that);
    
 
    bool noGuests() const;  // Return true if the WeddingGuest list
                               // is empty, otherwise false.
 
    int guestCount() const;  // Return the number of matches
                                 // on the WeddingGuest list.
 
    bool inviteGuest(const std::string& firstName, const std::string&
               lastName, const GuestType& value);
      // If the full name (both the first and last name) is not equal
      // to any full name currently in the list then add it and return
      // true. Elements should be added according to their last name.
      // Elements with the same last name should be added according to
      // their first names. Otherwise, make no change to the list and
      // return false (indicating that the name is already in the
      // list).
 
    bool alterGuest(const std::string& firstName, const std::string&
lastName, const GuestType & value);
      // If the full name is equal to a full name currently in the
      // list, then make that full name no longer map to the value it
      // currently maps to, but instead map to the value of the third
// parameter; return true in this case. Otherwise, make no
// change to the list and return false.

bool inviteOrAlter(const std::string& firstName, const
std::string& lastName, const GuestType& value);
// If full name is equal to a name currently in the list, then
// make that full name no longer map to the value it currently
// maps to, but instead map to the value of the third parameter;
// return true in this case. If the full name is not equal to
// any full name currently in the list then add it and return
// true. In fact, this function always returns true.

bool crossGuestOff(const std::string& firstName, const
std::string& lastName);
// If the full name is equal to a full name currently in the
// list, remove the full name and value from the list and return
// true.  Otherwise, make no change to the list and return
// false.

bool invitedToTheWedding(const std::string& firstName, const
std::string& lastName) const;
// Return true if the full name is equal to a full name
// currently in the list, otherwise false.

bool matchInvitedGuest(const std::string& firstName, const
std::string& lastName, GuestType& value) const;
// If the full name is equal to a full name currently in the
// list, set value to the value in the list that that full name
// maps to, and return true.  Otherwise, make no change to the
// value parameter of this function and return false.

bool verifyGuestOnTheList(int i, std::string& firstName,
std::string& lastName, GuestType & value) const;
// If 0 <= i < size(), copy into firstName, lastName and value
// parameters the corresponding information of the element at
// position i in the list and return true.  Otherwise, leave the
// parameters unchanged and return false. (See below for details
// about this function.)
     
void swapWeddingGuests(WeddingGuest& other);
// Exchange the contents of this list with the other one.
    

    void dump() const;
    
private:
    
    struct Node{
        std::string first;
        std::string last;
        Node *next;
        Node *prev;
        GuestType value;
    };
    
    Node *head;
    Node *tail;
    int m_num;
};


bool joinGuests(const WeddingGuest & odOne,
                const WeddingGuest & odTwo,
                WeddingGuest & odJoined);

void attestGuests (const std::string& fsearch,
                   const std::string& lsearch,
                   const WeddingGuest& odOne,
                   WeddingGuest& odResult);




//bool comeBefore(const std::string& str1, const std::string& str2); //I added this function to see if a name should come before another name in the list, as capital letters will confuse comparison- ex. Minnie Mouse would come before mickey mouse if I use normal <,>,=, but mickey should come before Minnie for alphabetical order. Returns true if the first string comes before/is equal to the second string alphabetically. Returns false otherwise.

#endif /* WeddingGuest_h */
