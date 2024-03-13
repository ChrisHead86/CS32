#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
#include <string>
#include <vector>


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


void TunnelMan::doSomething(){
  
  if (!alive())
    return;

  //the following gets the key that the user inputs and uses a switch statement to do the action corresponding to that key
  int key;
  if (getSW()->getKey(key))
  {
      switch (key)
      {
          case KEY_PRESS_LEFT:
              if (getDirection() == left && getX()-1 >= 0)
                  moveTo(getX()-1, getY());
              else
                  setDirection(left);
              break;
          case KEY_PRESS_RIGHT:
              if (getDirection() == right && getX()+1 <= 60)
                  moveTo(getX()+1, getY());
              else
                  setDirection(right);
              break;
          case KEY_PRESS_UP:
              if (getDirection() == up && getY()+1 <= 60)
                  moveTo(getX(), getY()+1);
              else
                  setDirection(up);
              break;
          case KEY_PRESS_DOWN:
              if (getDirection() == down && getY()-1 >= 0)
                  moveTo(getX(), getY()-1);
              else
                  setDirection(down);
              break;
          case 'Z':
          case 'z':
              if (m_sonar > 0)
              {
                  getSW()->lightUp(getX(), getY());
                  m_sonar--;
              }
              break;
          case KEY_PRESS_TAB:
              if (m_gold > 0)
              {
                  GoldNugget *p = new GoldNugget(getSW(), getX(), getY(), true, false, true, false);
                  getSW()->createActor(p);
                  m_gold--;
              }
              break;
          case KEY_PRESS_SPACE:
              if (m_squirt > 0)
              {
                  getSW()->playSound(SOUND_PLAYER_SQUIRT);
                  squirter(getX(), getY(), getDirection());
                  m_squirt--;
              }
              break;
          case KEY_PRESS_ESCAPE:
              setDead();
              break;
      }
      
      //if there is earth, we play the digging sound
      if (getSW()->removeHlp(getX(), getY()))
          getSW()->playSound(SOUND_DIG);
  }
    
  return;
    
}
  
 


double Base::giveDist(double x_1, double y_1, double x_2, double y_2){
    
    double ret = sqrt(pow(x_2 - x_1, 2) + pow(y_2 - y_1, 2));
    return ret;
    
}

GraphObject::Direction Base::randDir(int a, int b){
    
 
    
    //generates a random number between 0 and 3 and uses switch statement to give random direction
    int x = (rand() % 4);
    switch (x)
    {
        case 0:
            if (getSW()->isCoordValid(a, b+1))
                return up;
            break;
        case 1:
            if (getSW()->isCoordValid(a, b-1))
                return down;
            break;
        case 2:
            if (getSW()->isCoordValid(a-1, b))
                return left;
            break;
        case 3:
            if (getSW()->isCoordValid(a+1, b))
                return right;
            break;
        }
    return up;
}

//shoots the squirt in the correct direction
void Base::squirter(int x, int y, Direction d){
   
    //switch statement for each possible direction
    switch (d)
    {
        case left:
        {
            //check if close to boulder or if the space already has earth
            if (x-4 >= 0 && !getSW()->closeToBld(x-4,y) && !getSW()->earth(x-4, y))
            {
                Squirt *p = new Squirt(getSW(), getX()-4, getY(), left);
                getSW()->createActor(p);
            }
            break;
        }
        case right:
        {
            //check if close to boulder or if the space already has earth
            if (x+4 <= 60 && !getSW()->closeToBld(x+4,y) && !getSW()->earth(x+4, y))
            {
                Squirt *p = new Squirt(getSW(), getX()+4, getY(), right);
                getSW()->createActor(p);
            }
            break;
        }
        case down:
        {
            //check if close to boulder or if the space already has earth
            if (y-4 >= 0 && !getSW()->closeToBld(x, y-4) && !getSW()->earth(x, y-4) )
            {
                Squirt *p = new Squirt(getSW(), getX(), getY()-4, down);
                getSW()->createActor(p);
            }
            break;
        }
        case up:
        {
            //check if close to boulder or if the space already has earth
            if (y+4 <= 57 && !getSW()->closeToBld(x, y+4) &&!getSW()->earth(x, y+4))
            {
                Squirt *p = new Squirt(getSW(), getX(), getY()+4, up);
                getSW()->createActor(p);
            }
            break;
        }
        case none:
            break;
    }
}


void Boulder::doSomething()
{
    
    //make sure boulder is "alive"
    upTick();
    if (!alive())
        return;
    
    
    //we check for each of the states to find which state the boulder is in
    if (getStable())
    {
        //check for earth underneath boulder, if none make it wait
        int count = 0;
        int i = 0;
        while (i <= 3)
        {
            if (!getSW()->getEarth(getX()+i,getY()-1)->alive() && getSW()->getEarth(getX()+i,getY()-1) != nullptr)
            {
                ++count;
            }
            i++;
        }
        if (count == 4)
        {
            makeWait();
            resetTick();
        }
    }
    
    
    if (getWaiting())
    {
        //once we know its waiting, we check to see if 30 ticks have happened- if so, transition to falling
        if (getTick() >= 30)
        {
            getSW()->playSound(SOUND_FALLING_ROCK);
            makeFall();
        }
    }
    
    if (getFalling())
    {
        //we first move it down one space
        moveTo(getX(), getY()-1);
        
        //make it dead if it hits bottom of screen
        if (getY() == 0)
        {
            setDead();
            return;
        }
        
        //set dead if it hits ground
        int j = 0;
        while (j < 4)
        {
            if (getSW()->getEarth(getX() + j, getY() - 1)->alive() && getSW()->getEarth(getX() + j,getY() - 1) != nullptr)
            {
                setDead();
                return;
            }
            j++;
        }
        
        
        if (getSW()->compareBldHlp(getX(),getY()))
        {
            setDead();
            return;
        }
        
        //responds properly if the distance between the boulder and tunnelman is <= 3
        if (giveDist(getX(), getY(), getSW()->getTM()->getX(), getSW()->getTM()->getY()) <= 3.0)
        {
            getSW()->getTM()->doAnnoy(10);
            getSW()->getTM()->setDead();
        }
        
        //fall on protestor if one exists
        getSW()->annoyProt(10, getX(), getY());
    }
    return;
}


Protester::Protester(StudentWorld *sw, int imageID): Base(sw, imageID, 56, 60, left, 1.0, 0), m_leaving(false), m_numMove(rand()%(53)+8), m_health(0), m_restTick(0),m_numSinceShout(0), m_numSincePerpendicular(0) {
    
    setVisible(true);
    int level = getSW()->getLevel();
    m_numBtwn = fmax(0, (int)(3 - level / 4));
    
}

//returns whether the protester is facing the TM or not
bool Protester::isFaceTM(){
    
    //we get the protesters direction
    GraphObject::Direction d = getDirection();
    
    //we check if the direction faces the protester by checking what the tunnel man's location is- for example, if the protester is facing right, the TM's x value must be larger- if any of these statements are true, the protester is facing the TM
    bool ret1 = (d == up && getSW()->getTM()->getY() >= getY());
    bool ret2 = (d == down && getSW()->getTM()->getY() <= getY());
    bool ret3 = (d == left && getSW()->getTM()->getX() <= getX());
    bool ret4 = (d == right && getSW()->getTM()->getX() >= getX());
    
    if (ret1 || ret2 || ret3 || ret4)
        return true;
    else return false;
    
}

//moves the protester one spot in the given direction
void Protester::move(GraphObject::Direction d){
    
    //depending on the direction of the attempted move, checks to see if the space we try to move to is valid- if so, the protester moves to that space
    
    if (d == up){
            if (getSW()->isCoordValid(getX(), getY()+1))
                moveTo(getX(), getY()+1);
    }
    
    else if (d == down){
        if (getSW()->isCoordValid(getX(), getY()-1))
            moveTo(getX(), getY()-1);
    }
    
    else if (d == left)
    {
        if (getSW()->isCoordValid(getX()-1, getY()))
            moveTo(getX()-1, getY());
    }
    
    else if (d == right){
        if (getSW()->isCoordValid(getX()+1, getY()))
            moveTo(getX()+1, getY());
    }

}

//returns the direcition of a path to the TM
GraphObject::Direction Protester::pathDir(){
    
    bool tf = true;
    
    //this if statement runs if the TM is in a straight path to the right or left of the protester
    
    if (getY() == getSW()->getTM()->getY())
    {
        //how far apart are they
        int distance = abs(getX() - getSW()->getTM()->getX());
        
        //check is the TM is to the left of the protester
        if (getX() > getSW()->getTM()->getY())
        {
            
            for (int i = 0; i < distance; i++)
            {
                if(!getSW()->isCoordValid(getX()-i, getY()))
                {
                    tf = false;
                    break;
                }
            }
            if (tf)
                return GraphObject::left;
            else
                tf = true;
        }
        //check if TM is to the right of the protester
        else
        {
            for (int i = 0; i < distance; i++)
            {
                if (!getSW()->isCoordValid(getX()+i, getY()))
                {
                    tf = false;
                    break;
                }
            }
            if (tf)
                return GraphObject::right;
            else
                tf = true;
        }
    }
    
    //this if statement runs if the TM is in a straight path above or below the protester
    else if ((getX() == getSW()->getTM()->getX()))
    {
        int distance = abs(getY() - getSW()->getTM()->getY());
        // check if TM is below protester
        if (getY() > getSW()->getTM()->getY())
        {
            for (int i = 0; i < distance; i++)
            {
                if (!getSW()->isCoordValid(getX(), getY()-i))
                {
                    tf = false;
                    break;
                }
                
            }
            if (tf)
                return GraphObject::down;
            else
                tf = true;
        }
        // check if TM is above protester
        else
        {
            for (int i = 0; i < distance; i++)
            {
                if (!getSW()->isCoordValid(getX(), getY()+i))
                {
                    tf = false;
                    break;
                }
            }
            if (tf)
                return GraphObject::up;
        }
    }
    
    return GraphObject::none;
}

void Protester::doSomething(){
    
    if (!alive())
        return;
    
    int level = getSW()->getLevel();
    
    //returns if protester needs to wait to move
    if (m_numBtwn >= 1)
    {
        m_numBtwn--;
        return;
    }
    //if not-
    else
    {
        //we reset the number of turns to wait between moves and increment the number of moves since shouting and moving perpendicular
        if (!getLeaving())
            m_numBtwn = fmax(0, (int)(3-level/4));
        
        m_numSinceShout++;
        m_numSincePerpendicular++;
    }

    //if protester is leaving, he moves towards the exit or leaves the game if they are already there
    if (getLeaving())
    {
        if (getX() != 60 && getY() != 60)
        {
            //find the direction of the exit and move that way
            GraphObject::Direction d = getSW()->exitDir(getX(), getY());
            move(d);
            return;
        }
        else
        {
            setDead();
            return;
        }
    }

    //if they are close enough to the TM, they shout, and the TM is annoyed
    if (getSW()->closeToTM(4.0, getX(), getY()) && isFaceTM() && m_numSinceShout > 15)
    {
        m_numSinceShout = 0;
        getSW()->playSound(SOUND_PROTESTER_YELL);
        getSW()->getTM()->doAnnoy(2);
        return;
    }
    
    //if protester is hardcore, we proceed
    if (hard())
    {
        int M = 16 + getSW()->getLevel() * 2;
        
        //if they are close enough, hardcore protester senses the cell phone signal of the TM and moves towards him
        if (getSW()->howFarFromTM(getX(), getY()) <= M && getSW()->howFarFromTM(getX(), getY()) != -1)
        {
            setDirection(getSW()->tmDir(getX(), getY()));
            move(getDirection());
            return;
        }
    }
    
    //if protester has a stright path to the TM, they move in that direction
    if (!getSW()->closeToTM(4.0, getX(), getY()) && pathDir() != none)
    {
        setDirection(pathDir());
        move(getDirection());
        m_numMove = 0;
        return;
    }
    
    //case in which the protester does not have a straight path to the TM- in this case we pick a random direction in which to move
    m_numMove--;

    
    //case in which the protester is at an intersection and can move perpendicularly
    if (getPerpendicular() != GraphObject::none && m_numSincePerpendicular > 200)
    {
        m_numSincePerpendicular = 0;
        setDirection(getPerpendicular());
        m_numMove = rand() % 53 + 8;
        move(getDirection());
        return;
    }
    else if (m_numMove <= 0)
    {
        setDirection(randDir(getX(), getY()));
        m_numMove = rand() % 53 + 8;
        move(getDirection());
    }
    
    //case if path is blocked
    GraphObject::Direction t = getDirection();

    if (t == up)
    {
        if (!getSW()->isCoordValid(getX(), getY()-1))
            m_numSincePerpendicular = 0;
    }
    else if (t == down)
    {
        if (!getSW()->isCoordValid(getX(), getY()+1))
            m_numSincePerpendicular = 0;
    }
    else if (t == left)
    {
        if (!getSW()->isCoordValid(getX()-1, getY()))
            m_numSincePerpendicular = 0;
    }
    else if (t == right)
    {
        if (!getSW()->isCoordValid(getX()+1, getY()))
            m_numSincePerpendicular = 0;
    }
    
    
}


void Protester::stun(){
    //if stunned, the protester has to wait to move again
    m_numBtwn = fmax(50, (int)(100 - (getSW()->getLevel() * 10)));
}

void Protester::doAnnoy(int x){
    
    // check if they are already leaving
    if (getLeaving())
        return;
    
    //decrement health
    m_health -= x;

    //if protester is annoyed to the point of leaving, make them leave and give the player the correct amount of points
    if (m_health <= 0)
    {
        m_numBtwn = 0;
        setLeaving();
        getSW()->playSound(SOUND_PROTESTER_GIVE_UP);
        if (x == 10)
            getSW()->increaseScore(500);
        else
            getSW()->increaseScore(100);
    }
    else
    {
        getSW()->playSound(SOUND_PROTESTER_ANNOYED);
        stun();
    }
}

//returns the direction in which a perpendicular move can be made- if no possible perpendicular move, returns none
GraphObject::Direction Protester::getPerpendicular(){
   
    //direction protester is facing
    GraphObject::Direction d = getDirection();
    int x = getX();
    int y = getY();
    
    //case if perpendicular direction is right/left
    if (d == up || d == down)
    {
        //case if both right and left are valid moves
        if(getSW()->isCoordValid(x-1, y) && getSW()->isCoordValid(x+1, y))
        {
           if ((rand() % 10 + 1) <= 5)
               return right;
           else
               return left;
        }
        //case if only one direction or neither direction is valid
        else if (getSW()->isCoordValid(x-1, y))
            return left;
        else if (getSW()->isCoordValid(x+1, y))
            return right;
        return none;
    }
    //case if perpendicular direction is up/down
    else if (d == right || d == left)
    {
        if(getSW()->isCoordValid(x, y-1) && getSW()->isCoordValid(x, y+1))
        {
            // case if both up/down are valid
           if ((rand() % 10 + 1) <= 5)
               return down;
           else
               return up;
        }
        //case if neither direction is valid
        else if (getSW()->isCoordValid(x, y-1))
            return down;
        else if (getSW()->isCoordValid(x, y+1))
            return up;
        
        return none;

    }
    return none;
}

//gives points for gold
void RegProt::getGold(){
    
    getSW()->increaseScore(25);
    getSW()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    setLeaving();
    
}

//gives points for gold and stuns to stare at it
void HardProt::getGold(){
    
    getSW()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getSW()->increaseScore(50);
    stun();
    
}


void Squirt::doSomething(){
    
    //reached max distance
    if (m_d <= 0)
        setDead();
    
    //if close enough, annoy protesters
    if(getSW()->annoyProt(getX(), getY(), 2))
        setDead();
    
    //continue to move if possible
    if (!getSW()->isMoveValid(getX(), getY(), getDirection()))
        setDead();
    else
    {
        move(getDirection());
        m_d--;
    }
}

//moves one space in the given direction
void Squirt::move(Direction d){
    
    if (d == up)
    {
        if (getSW()->isCoordValid(getX(), getY()+1))
            moveTo(getX(), getY()+1);
    }
    else if (d == down)
    {
        if (getSW()->isCoordValid(getX(), getY()-1))
            moveTo(getX(), getY()-1);
    }
    else if (d == right)
    {
        if (getSW()->isCoordValid(getX()+1, getY()))
            moveTo(getX()+1, getY());
    }
    else if (d == left)
    {
        if (getSW()->isCoordValid(getX()-1, getY()))
            moveTo(getX()-1, getY());
    }
    
}

void Barrel::doSomething(){
    
    if (!alive())
        return;

    else if (!isVisible() && giveDist(getX(), getY(), getSW()->getTM()->getX(), getSW()->getTM()->getY()) <= 4.0)
    {
        setVisible(true);
    }
    if (giveDist(getX(), getY(), getSW()->getTM()->getX(), getSW()->getTM()->getY()) <= 3.0)
    {
        getSW()->playSound(SOUND_FOUND_OIL);
        getSW()->increaseScore(1000);
        getSW()->gatherBarrel();
        setDead();
    }
}

void GoldNugget::doSomething(){
 
    if(!alive())
        return;
    
    m_tick++;
    
    //if nugget is not visible, but is within visible radius, set to visible
    if (!isVisible() && giveDist(getX(), getY(), getSW()->getTM()->getX(), getSW()->getTM()->getY()) <= 4.0)
    {
        setVisible(true);
        return;
    }
    
    //if it can be picked up and TM is within radius, it is picked up by TM, sore is increased
    if (getTmPick() && giveDist(getX(), getY(), getSW()->getTM()->getX(), getSW()->getTM()->getY()) <= 3.00)
    {
        getSW()->playSound(SOUND_GOT_GOODIE);
        getSW()->increaseScore(10);
        getSW()->getTM()->getGold();
        setDead();
    }
    
    // check if it can be picked up by protesters and act accordingly
    if (getPPick() && getSW()->closestProt(getX(), getY()) != nullptr)
    {
        setDead();
        getSW()->closestProt(getX(), getY())->getGold();
    }

    //if it is not permanent and it has no time left, kill it
    if (!getPerm())
    {
        if (m_tick >= 100)
            setDead();
    }
    
}

void SonarKit::doSomething(){
    
    m_tick++;
    if (!alive())
        return;
    
    // if the sonar kit is within radius, TM picks it up
    if (giveDist(getX(), getY(), getSW()->getTM()->getX(), getSW()->getTM()->getY()) <= 3.0)
    {
        setDead();
        getSW()->playSound(SOUND_GOT_GOODIE);
        getSW()->getTM()->obtainSKit();
        getSW()->increaseScore(75);
    }
    
    //if it has reached its max time, kill it
    int level = getSW()->getLevel();
    int max = fmax(100, (int)(300 - 10 * level));
    if (m_tick > max)
        setDead();
}

void WaterPool::doSomething(){
    
    m_tick++;
    if (!alive())
        return;
    
    //if it is within radius of the TM, TM collects it and is awarded points
    if (giveDist(getX(), getY(), getSW()->getTM()->getX(), getSW()->getTM()->getY()) <= 3.00)
    {
        setDead();
        getSW()->playSound(SOUND_GOT_GOODIE);
        getSW()->getTM()->getSquirt();
        getSW()->increaseScore(100);
    }
    
    // if it has no more time left, kill it
    int level = getSW()->getLevel();
    int max = fmax(100, (int)(300 - 10 * level));
    if (m_tick > max)
        setDead();
}






    



