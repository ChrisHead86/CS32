#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include "GameController.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


int StudentWorld::init(){
    
    //we first initialize all of the variales and set them to their proper values
    m_ticks = 0;
    int level = getLevel();
    m_ticksb4Prot = max(25, (int)(200-level));
    m_numProt = fmin(15, (int)(2 + level * 1.5));
    m_tm = new TunnelMan(this);
    vector<vector<Earth*>> earth;
    for (int x = 0; x <= 63; ++x)
    {
        vector<Earth*> temp;
        for (int y = 0; y <= 59; ++y)
        {
                
            if ( x < 30 || x > 33 || y < 4 )
                temp.push_back(new Earth(this, x, y));
            else
                temp.push_back(nullptr);
        }
        earth.push_back(temp);
    }
    m_earth = earth;
    
    //we now create the actors and add them to the field
    vector<Base*> acts;
    m_act = acts;
    
    //create boulders
    int B = min((int)level / 2 + 2, 9);
    int i = 0;
    int j;
    int k;
    while (i < B)
    {
        getBoulderPos(j, k);
        createActor(new Boulder(this, j, k));
        i++;
    }
    
    //create gold
    int G = max((int)(5 - level / 2), 2);
    int a = 0;
    int b;
    int c;
    while (a < G)
    {
        getBoulderPos(b, c);
        createActor(new Boulder(this, b, c));
        a++;
    }
    
    
    //create oil barrels
    int L = min((int)(2 + level), 2);
    int x = 0;
    int y;
    int z;
    while (x < L)
    {
        getBoulderPos(y, z);
        createActor(new Boulder(this, y, z));
        x++;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}


bool StudentWorld::removeEarth(int x, int y)
{
    bool is_dug = false;
    for (int i = 0; i < 4; ++i)
    {
        for (int k = 0; k < 4; ++k)
        {
            
            if ((x+i>=0 && x+i<64) && (y+k>=0 && y+k<60))
            {
               
                if (m_earth[x+i][y+k] != NULL && m_earth[x+i][y+k]->alive())
                {
                    m_earth[x+i][y+k]->setDead();
                    is_dug = true;
                }
            }
        }
    }
    return is_dug;
}

Earth* StudentWorld::getEarth(int a, int b){
    {
            if ((a < 0 || a > 63) || (b < 0 && b > 59))
                return nullptr;
            else
                return m_earth[a][b];
        }
}


//helper function for move-adds actors to the field
void StudentWorld::addActs()
{
    int level = getLevel();
    //the following creates the odds that a hardcore protestor or goodie will be added
    int g_prob = level * 25 + 300;
    bool g_add = ((rand() % g_prob) < 1);
    int hardProt_prob = min(90, (int)level * 10 + 30);
    bool hardProt_add = ((rand() % hardProt_prob) < 1);
    minusTicksb4();
    
    
    if (g_add)
    {
        //adds a sonar kit and/or waterpool with respective odds
        if (rand() % 5 < 1)
        {
            createActor(new SonarKit(this, 0, 60));
        }
        
        if (rand() % 5 < 4)
        {
            int a;
            int b;
            getWaterPos(a, b);
            createActor(new WaterPool(this, a, b));
        }
        
    }
    
    //adds protestor on first tick- either hardcopre or regular
    if (m_ticks == 1)
    {
        if (hardProt_add)
        {
            HardProt* h = new HardProt(this);
            createActor(h);
        }
        else
        {
            RegProt* r = new RegProt(this);
            createActor(r);
        }
       
        minusNumProt();
        resetTicksB4();
    }
    
    // add new protesters after every tick until the number of protesters is reached
    else if (getTick() <= 0 && getNumProt() > 0)
    {
        if (hardProt_add)
        {
            HardProt* h2 = new HardProt(this);
            createActor(h2);
        }
        else
        {
            RegProt* r2 = new RegProt(this);
            createActor(r2);
        }
        
        minusNumProt();
        resetTicksB4();
    }


}


void StudentWorld::resetTicksB4(){
    
    int level = getLevel();
    m_ticksb4Prot = max(25, 200-level);
    
}

int StudentWorld::move()
{
    //increment thew ticks, create actors and ask them to do something
    m_ticks++;
    setWords();
    addActs();

    m_tm->doSomething();
    
    //makes protester path to exit
    createPath(m_exitpath, 60, 60);
   
    //creates map for hardcore protester
    createPath(m_tmpath, m_tm->getX(), m_tm->getY());
 
    //call dosomething for all actors
    vector<Base*>::iterator acts = m_act.begin();
    while(acts != m_act.end())
    {
        if((*acts)->alive())
            (*acts)->doSomething();
        acts++;
    }
    
    acts = m_act.begin();
    
    //get rid of all the dead actors
    buryDead();
    
    //check if TM is dead
    if (!m_tm->alive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    
    
    // if player has all barrels, end level
    if (m_barrels == 0 && m_ticks > 1000)
        return GWSTATUS_FINISHED_LEVEL;
    
    return GWSTATUS_CONTINUE_GAME;
}

//helper function for destructor
void StudentWorld::cleanUp()
{
    //we delete the TM and all of the earth and the actors
    delete m_tm;
    
    //earth
    for (int i = 0; i < m_earth.size(); i++)
    {
        vector<Earth*>::iterator it = m_earth[i].begin();
        while (it != m_earth[i].end())
        {
            Earth *p = *it;
            delete p;
            it = m_earth[i].erase(it);
        }
    }
    
    //actors
    vector<Base*>::iterator actors;
    actors = m_act.begin();
    while (actors != m_act.end())
    {
        Base *p = *actors;
        delete p;
        actors = m_act.erase(actors);
    }
}


//rest of helper functions

void StudentWorld::buryDead()
{
 
    //get rid of the actors first
    vector<Base*>::iterator actors = m_act.begin();
    while(actors != m_act.end())
    {
        //iterates through actors vector and deletes actors
        if(!(*actors)->alive())
        {
            Base *p = *actors;
            delete p;
            actors = m_act.erase(actors);
        }
        else
            actors++;
    }
}

// helper function to remove earth mid game
bool StudentWorld::removeHlp(int a, int b)
{

    bool dig = false;
    
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((a + i >= 0 && a + i < 64) && (b + j >= 0 && b + j < 60))
            {
                // check for access errors
                if (m_earth[a+i][b+j] != nullptr && m_earth[a+i][b+j]->alive())
                {
                    m_earth[a+i][b+j]->setDead();
                    dig = true;
                }
            }
        }
    }
    return dig;
}

string StudentWorld::formatWords(std::string &s, int lvl,  int health, int lives,  int gold, int water, int oil, int sonar, int score)
{

    // formats words with correct spaces for level
    std::ostringstream o_lvl;
    o_lvl.fill(' ');
    o_lvl << setw(2) << lvl;
    s += "  Lvl: ";
    s += o_lvl.str();
    s += "  ";

    //creates words for lives
    s += "Lives: ";
    s += lives + '0';
    s += "  ";
    
    //creates words for health
    ostringstream o_health;
    o_health.fill(' ');
    o_health << setw(3) << health;
    s += "Hlth: ";
    s += o_health.str();
    s += "%  ";
    
    //creates words for water
    ostringstream o_water;
    o_water.fill(' ');
    o_water << setw(2) << water;
    s += "Wtr: ";
    s += o_water.str();
    s += "  ";
    
    // creates words for gold
    ostringstream o_gold;
    o_gold.fill(' ');
    o_gold << setw(2) << gold;
    s += "Gld: ";
    s += o_gold.str();
    s += "  ";
    
    // creates words for oil field
    ostringstream o_oil;
    o_oil.fill(' ');
    o_oil << setw(2) << oil;
    s += "Oil Left: ";
    s += o_oil.str();
    s += "  ";
    
    // creates words for sonar
    ostringstream o_sonar;
    o_sonar.fill(' ');
    o_sonar<<setw(2)<<sonar;
    s += "Sonar: ";
    s += o_sonar.str();
    s += "  ";
    
    //creates the acore words
    ostringstream o_score;
    o_score.fill('0');
    o_score << setw(6) << score;
    s += "Scr: ";
    s += o_score.str();
    return s;
}
void StudentWorld::setWords()
{
    //gets values for the display and call our helper function
    int health = m_tm->getHealth();
    int water = m_tm->getSquirt();
    int gold = m_tm->getGold();
    int oil = m_barrels;
    int lvl = getLevel();
    int lives = getLives();
    int sonar = m_tm->getSonar();
    int score = getScore();
    string s = "";
    
    formatWords(s, lvl, health, lives, gold, water, oil, sonar, score);
    
    setGameStatText(s);
    
}

//gets a valid position
void StudentWorld::getPos(int &x, int &y)
{
    bool v = false;
    do {
        x = rand() % 60;
        y = rand() % 56;
        v = true;
        
        //make sure there is no actor in the position
        vector<Base*>::iterator it = m_act.begin();
        while (it != m_act.end())
        {
            if (dist(x, y, (*it)->getX(), (*it)->getY()) <= 6.0)
                v = false;
            it++;
        }
        //adjust for different sizes for gold and oil
        if (((26 <= x && x <= 33) && (1 <= y && y <= 59)))
            v = false;
        } while(!v);
    
}
//finds a valid position for a boulder to be placed
void StudentWorld::getBoulderPos(int &x, int &y)
{
    //follows the exact same structure as the last function, but adjusts for boulders
    bool v = false;
    do {
        
        x = rand() % 60;
        y = rand() % (56 - 20 + 1) + 20;
        v = true;

        vector<Base*>::iterator it = m_act.begin();
        while (it != m_act.end())
        {
            if (dist(x, y, (*it)->getX(), (*it)->getY()) <= 6.0)
                v = false;
            it++;
        }
        if (((26 <= x && x <= 33) && (1 <= y && y <= 59)))
            v = false;
    } while(!v);
    
    //call the function to remove earth mid game
    removeHlp(x, y);
    
}

// gives the distance between the 2 points- exact same as delaration in actor.cpp
double StudentWorld::dist(double x, double y, double x_2, double y_2){
    
    double ret = sqrt(pow(x_2 - x, 2) + pow(y_2 - y, 2));
    return ret;
    
}
//follows exact same structure as previous functions but adjusted for waterpools
void StudentWorld::getWaterPos(int &x, int &y)
{
    bool v=false;
    do {
        x = rand() % 60;
        y = rand() % 56;
        v = true;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (getEarth(x + i, y + j) != nullptr && getEarth(x + i, y + j)->alive())
                    v = false;
            }
        }
        
        //if its within the radius of other objects, can not be a valid position
        vector<Base*>::iterator it = m_act.begin();
        while (it != m_act.end())
        {
            if (dist(x, y, (*it)->getX(), (*it)->getY()) <= 6.0)
                v = false;
            it++;
        }
    } while(!v);

}

//returns if a falling boulder falls onto a boulder
bool StudentWorld::compareBldHlp(int x, int y){
    
    
    for (vector<Base*>::iterator it = m_act.begin(); it != m_act.end(); it++)
     {
        
         //dynamic_cast in order to safely assign- will return 0 if fails
         Boulder *boulder = dynamic_cast<Boulder*>(*it);
         
         //checks to see if assined properly
         if (boulder)
         {
             //compare the positions of the boulders
             for (int i = 0; i < 4; i++)
             {
                 int x1 = x + i;
                 int y1 = y;
                 for (int k = 0; k < 4; ++k)
                 {
                     int x2 = boulder->getX() + k;
                     int y2 = boulder->getY() + 3;

                     //returns if the positions match up such that the boulder falls on the other
                     if (x1 == x2 && y1 == y2)
                         return true;
                 }
             }
         }
         
         it++;
         
     }
    //returns false if boulders to not hit
    return false;
    
}
//checks if there is a boulder within 4 spots up and to the right
bool StudentWorld::boulder(int x, int y)
{
    
    for (vector<Base*>::iterator it = m_act.begin(); it != m_act.end(); it++)
     {
         //same strategy as before
         Boulder *boulder = dynamic_cast<Boulder*>(*it);
         
         if(boulder)
         {
             int x_ = boulder->getX();
             int y_ = boulder->getY();
             for (int i = 1; i <= 4; i++)
             {
                //looks for matching positions, returns true if found
                 for (int k = 1; k <+ 4; k++)
                 {
                     if (x == x_ + i && y == y_ + k)
                         return true;
                 }
             }
         }
         
         it++;
         
     }
    
    return false;
    
}

//function follows same structure as previous function, but adjusts for earth
bool StudentWorld::earth(int x, int y)
{
    for (int i = 1; i < 5; i++)
    {
        for (int k = 1; k < 5; k++)
        {
            if (getEarth(x + i, y + k) != nullptr)
                if(getEarth(x+i, y+k)->alive())
                    return true;
        }
    }
    
    return false;
    
}

// returns if boulder is within radius
bool StudentWorld::closeToBld(int x, int y)
{
    
    for (vector<Base*>::iterator it = m_act.begin(); it != m_act.end(); it++)
     {
         //same strategy as before
         Boulder *boulder = dynamic_cast<Boulder*>(*it);
         
         if(boulder)
         {
            if(dist(x, y, boulder->getX(), boulder->getY()) < 3.0)
                return true;
         }
         
         it++;
     }
    
    return false;
    
}

//returns a protester if they are within radius, nullptr if no protesters
Protester* StudentWorld::closestProt(int x, int y){
   
    for (vector<Base*>::iterator it = m_act.begin(); it != m_act.end(); it++)
     {
         //same strategy as before- not adding extra comments to avoud redundancy
         Protester *protester = dynamic_cast<Protester*>(*it);
         
         if(protester)
         {
            if(dist(x, y, protester->getX(), protester->getY()) < 3.0)
                return protester;
         }
         
         it++;
         
     }
    
    return nullptr;
    
}

//annoys all protesters in the radius
bool StudentWorld::annoyProt(int x, int y, int a)
{
    //bool to return at end of function
    bool b = false;
    
    for (vector<Base*>::iterator it = m_act.begin(); it != m_act.end(); it++)
     {
         //same strategy as before
         Protester *protester = dynamic_cast<Protester*>(*it);
         
         if(protester)
         {
            if(dist(x, y, protester->getX(), protester->getY()) < 3.0)
            {
                
                b = true;
                protester->doAnnoy(a);
 
            }
         }
         
         ++it;
         
     }
    
    return b;
    
}

// makes things visible within given radius
void StudentWorld::lightUp(int x, int y){
    
    for (vector<Base*>::iterator it = m_act.begin(); it != m_act.end(); it++)
    {
        
        if (dist(x, y, (*it)->getX(), (*it)->getY()) <= 12.0)
        {
            (*it)->setVisible(true);
        }
        
        it++;
        
    }
    
}


// creates a map of directions that are optimized for the protester to leave the map
void StudentWorld::createPath(GraphObject::Direction arr[][VIEW_HEIGHT], int x, int y){
    
    //creates an array to store  a series of directions in to be added to later
    
    for (int i = 0; i < VIEW_WIDTH; i++)
        for (int j = 0; j < VIEW_HEIGHT; j++)
            arr[i][j] = GraphObject::Direction::none;
    
    //creates a queue for a BFS- uses Dir struct created in the header file
    queue<Dir> d;
    Dir first(x, y);
    
    d.push(first);
    Dir curr(0, 0);
    //Breadth First Search- searches for valid positions to find a path- checks right, left, up, then down
    while (!d.empty()) {
        curr = d.front();
        d.pop();
        
        int x_ = curr.getX();
        int y_ = curr.getY();
    
        if (arr[x_ + 1][y_] == GraphObject::Direction::none && isCoordValid(x_ + 1, y_))
        {
            d.push(Dir(x_ + 1, y_));
            arr[x_ + 1][y_] = GraphObject::Direction::left;
        }

        if (isCoordValid(x-1, y) && arr[x - 1][y] == GraphObject::Direction::none) {
            d.push(Dir(x - 1, y));
            arr[x - 1][y] = GraphObject::Direction::right;
        }

        if (isCoordValid(x, y+1) && arr[x][y + 1] == GraphObject::Direction::none) {
            d.push(Dir(x, y + 1));
            arr[x][y + 1] = GraphObject::Direction::down;
        }

        if (isCoordValid(x, y-1) && arr[x][y - 1] == GraphObject::Direction::none) {
            d.push(Dir(x, y - 1));
            arr[x][y - 1] = GraphObject::Direction::up;
        }
    }
}

//returns if the coordinates that are passed in are valid coordinates
bool StudentWorld::isCoordValid(int x, int y){
    
    //check for bounds
    if (x < 0 || x > 60 || y < 0 || y > 60)
        return false;
    
    //check for earth
    if (earth(x,y))
        return false;

    //check for boulders
    if (boulder(x, y) || closeToBld(x, y))
        return false;
    
    return true;
    
}

//checks if an attempted move is able to be done
bool StudentWorld::isMoveValid(int x, int y, GraphObject::Direction d){
   
    //adjusts x,y for testing
    if (d == GraphObject::up)
    {
        y++;
    }
    else if (d == GraphObject::down)
    {
        y--;
    }
    else if (d == GraphObject::left)
    {
        x--;
    }
    else if (d == GraphObject::right)
    {
        x++;
    }
        
    

    // check for bound then if there are boulders/earth- same as last function
    if (x < 0 || x > 60 || y < 0 || y > 60)
        return false;

    //check for earth
    else if (earth(x,y))
        return false;

    //check for boulders
    if (boulder(x, y) || closeToBld(x, y))
        return false;

    return true;

}

// returns true if the given coordinates are within the given radius
bool StudentWorld::closeToTM(int x, int y, double r)
{
    double ret = dist(x, y, m_tm->getX(), m_tm->getY());
    if (ret <= r)
        return true;
    else return false;
    
}

//use strategy for solving maze talked about in class- using recursion
int StudentWorld::howFarFromTM(int x, int y)
{
    //case if coordinates are at same place as TM
    if (m_tm->getX() == x && m_tm->getY() == y)
        return 0;

    
    //recursively checks each move until it reaches TM
    if (m_tmpath[x][y] == Base::down)
        return (1 + howFarFromTM(x, y - 1));
    else if (m_tmpath[x][y] == Base::up)
        return (1 + howFarFromTM(x, y + 1));
    else if(m_tmpath[x][y] == Base::left)
        return (1 + howFarFromTM(x - 1,y));
    else if (m_tmpath[x][y] == Base::right)
        return (1 + howFarFromTM(x + 1, y));


    //failed to find path
    return -1;
    
}

