#ifndef ACTOR_H_
#define ACTOR_H_
#include <vector>
#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Base : public GraphObject{
    
public:
    Base(StudentWorld *sw, int imageID, int x_start, int y_start, Direction startDirection, float size, unsigned int depth) : GraphObject(imageID, x_start, y_start, startDirection, size, depth), m_sw(sw), m_alive(true) {setVisible(true);}
    
    //helper functions
    double giveDist(double x_1, double y_1, double x_2, double y_2);
    Direction randDir(int a, int b);
    void squirter(int x, int y, Direction d);
    
    //functions to implement
    virtual ~Base(){}
    virtual void doSomething() = 0;
    bool alive() {
        return m_alive;
        
    }
    void setDead() {
        m_alive = false;
        setVisible(false);
        
    }
    StudentWorld *getSW(){
        return m_sw;
        
    }
    virtual void doAnnoy(int x) = 0;
    
    
private:
    StudentWorld *m_sw;
    bool m_alive;
};

class Earth : public Base{
public:
    Earth(StudentWorld *sw, int x, int y) : Base(sw, TID_EARTH, x, y, right, 0.25, 3) {}
    virtual ~Earth() {}
    virtual void doSomething() {}
    virtual void doAnnoy(int x){}
    
    
};

class TunnelMan : public Base{
public:
    TunnelMan (StudentWorld *sw) : Base(sw, TID_PLAYER, 30, 60, right, 1.0, 0), m_sonar(1), m_gold(0),m_health(10), m_squirt(5) {}
    virtual ~TunnelMan(){}
    virtual void doSomething();
    virtual void doAnnoy(int x){
        m_health = m_health - x;
        
    }
    //do not confuse the obtain and get functions- the get functions are to access the materials while the obtain functions are for the player to obtain them
    void obtainSKit() {
        m_sonar++;
        
    }
    void obtainGold() {
        m_gold++;
        
    }
    void obtainSquirt() {
        m_squirt = m_squirt + 5;
        
    }
    int getSonar() {
        return m_sonar;
        
    }
    int getGold() {
        return m_gold;
        
    }
    int getHealth() {
        return m_health * 10;
        
    }
    int getSquirt() {
        return m_squirt;
        
    }
    
    
private:
    int m_sonar;
    int m_gold;
    int m_health;
    int m_squirt;
    
};

class Boulder : public Base{
public:
    Boulder(StudentWorld *sw, int x_start, int y_start): Base(sw, TID_BOULDER, x_start, y_start, down, 1.0, 1), isStable(true), isWaiting(false), isFalling(false) {}
    virtual ~Boulder(){}
    virtual void doSomething();
    virtual void doAnnoy(int x) {}
    void resetTick() {
        m_tick = 0;
    }
    void upTick() {
        m_tick++;
    }
    void makeStable() {
        isStable = true;
        isWaiting = false;
        isFalling = false;
        
    }
    void makeWait() {
        isStable = false;
        isWaiting = true;
        isFalling = false;
        
    }
    void makeFall() {
        isStable = false;
        isWaiting = false;
        isFalling = true;
        
    }
    bool getStable(){
        return isStable;
        
    }
    bool getWaiting() {
        return isWaiting;
        
    }
    bool getFalling() {
        return isFalling;
        
    }
    int getTick() {
        return m_tick;
        
    }
    
private:
    int m_tick;
    bool isStable;
    bool isWaiting;
    bool isFalling;
};

class Protester : public Base{
public:
    Protester(StudentWorld *sw, int imageID);
    
    // helper functions
    virtual void getGold()=0;
    bool isFaceTM();
    void move(GraphObject::Direction dir);
    GraphObject::Direction pathDir();
    virtual ~Protester() {}
    void doSomething();
    void doAnnoy(int x);
    virtual bool hard()=0;
    int getHealth() {
        return m_health * 10;
    }
    void setHealth(int x) {
        m_health = x;
    }
    bool getLeaving(){
        return m_leaving;
    }
    void setLeaving(){
        m_leaving = true; m_numBtwn = 0;
    }
    void stun();
    GraphObject::Direction getPerpendicular();
    

    
private:
    int m_health;
    bool m_leaving;
    int m_restTick;
    int m_numSinceShout;
    int m_numMove;
    int m_numSincePerpendicular;
    int m_numBtwn;
};

class RegProt : public Protester
{
public:
    RegProt(StudentWorld* sw) : Protester(sw, TID_PROTESTER) { setHealth(5); }
    virtual ~RegProt() {}
    virtual void getGold();
    virtual bool hard(){
        return false;
    }
private:
};

class HardProt : public Protester
{
public:
    HardProt(StudentWorld* sw) : Protester(sw, TID_HARD_CORE_PROTESTER) { setHealth(20); }
    virtual ~HardProt() {}
    virtual void getGold();
    virtual bool hard() {return true;}
private:
};

class Squirt : public Base
{
public:
    Squirt (StudentWorld *sw, int x_start, int y_start, Direction d):Base(sw, TID_WATER_SPURT, x_start, y_start, d, 1.0, 1), m_d(4) {setVisible(true);}
    //helper
    void move(GraphObject::Direction d);
    virtual ~Squirt() {}
    virtual void doSomething();
    virtual void doAnnoy(int x) {}
private:
    int m_d;
};


class Goodie : public Base{
public:
    Goodie(StudentWorld *sw, int imageID, int x_start, int y_start, Direction d_start, unsigned int depth, bool visible, bool tm_pickup, bool p_pickup, bool permanent): Base(sw, imageID, x_start, y_start, d_start, 1.0, depth), m_tmPickup(tm_pickup), m_pPickup(p_pickup), m_permanent(permanent), m_visible(visible) {setVisible(visible);}
    virtual ~Goodie() {}
    virtual void doSomething()=0;
    virtual void doAnnoy(int x)=0;
    void setTemp() {
        m_permanent = false;
        
    }
    void setPerm() {
        m_permanent = true;
        
    }
    bool getPerm() {
        return m_permanent;
        
    }
    void setVis(bool x) {
        setVisible(x);
        m_visible = x;
        
    }
    bool getVis() {
        return m_visible;
        
    }
    bool getTmPick()   {
        return m_tmPickup;
        
    }
    bool getPPick()    {
        return m_pPickup;
        
    }
private:
    bool m_visible;
    bool m_permanent;
    bool m_pPickup;
    bool m_tmPickup;
    
};

class Barrel : public Goodie{
public:
    Barrel(StudentWorld *sw, int x_start, int y_start) : Goodie(sw, TID_BARREL, x_start, y_start, right, 2, false, true, false, true) {}
    virtual ~Barrel() {}
    virtual void doSomething();
    virtual void doAnnoy(int x) {}
private:
};

class GoldNugget : public Goodie
{
public:
    GoldNugget(StudentWorld *sw, int x_start, int y_start, bool visible, bool tmPickup, bool pPickup, bool permanent) : Goodie(sw, TID_GOLD, x_start, y_start, right, 2, visible, tmPickup, pPickup, permanent), m_tick(0){}
    virtual ~GoldNugget() {}
    virtual void doSomething();
    virtual void doAnnoy(int x) {}
private:
    int m_tick;
};

class SonarKit : public Goodie
{
public:
    SonarKit(StudentWorld *sw, int x_start, int y_start) : Goodie(sw, TID_SONAR, x_start, y_start, right, 2, true, true, false, false), m_tick(0){}
    virtual ~SonarKit() {}
    virtual void doSomething();
    virtual void doAnnoy(int x) {}
private:
    int m_tick;
};

class WaterPool : public Goodie
{
public:
    WaterPool(StudentWorld *sw, int x_start, int y_start) : Goodie(sw, TID_WATER_POOL, x_start, y_start, right, 2, true, true, false, false), m_tick(0){}
    virtual ~WaterPool() {}
    virtual void doSomething();
    virtual void doAnnoy(int x) {}
private:
    int m_tick;
};


#endif // ACTOR_H_
