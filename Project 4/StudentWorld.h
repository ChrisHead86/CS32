#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <algorithm>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir) : GameWorld(assetDir), m_ticks(0), m_earth(NULL), m_tm(NULL) { }
    virtual ~StudentWorld() {cleanUp();}
    virtual int init();
    virtual int move();
    bool removeEarth(int x,int y);
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    virtual void cleanUp();
    TunnelMan* getTM() {return m_tm;}
    std::vector<Base*> getActs() {return m_act;}
    void addActs();
    Earth* getEarth(int a, int b);
    int getTick() {return m_ticks;}
    void addTick() {++m_ticks;}
    void resetTick() {m_ticks = 0;}
    int getTicksb4() {return m_ticksb4Prot;}
    void minusTicksb4() {m_ticksb4Prot--;}
    void resetTicksB4();
    int getNumProt() {return m_numProt;}
    void minusNumProt() {m_numProt--;}
    void gatherBarrel() { m_barrels--; }
    
    
    // helper functions
    void createPath(GraphObject::Direction map[][VIEW_HEIGHT], int x, int y);
    bool closeToTM(int x, int y, double r);
    bool annoyProt(int x, int y, int a);
    bool isMoveValid(int x, int y, GraphObject::Direction dir);
    bool isCoordValid(int x, int y);
    void createActor(Base* actor) {m_act.push_back(actor);}
    int howFarFromTM(int x, int y);
    std::string formatWords(std::string &s, int lvl,  int health, int lives,  int gold, int water, int oil, int sonar, int scr);
    bool removeHlp(int a, int b);
    double dist(double x, double y, double x_2, double y_2);
    void buryDead();
    bool compareBldHlp(int a, int b);
    void getPos(int &x, int &y);
    void getWaterPos(int &x, int &y);
    void getBoulderPos(int &x, int &y);
    void setWords();
    bool boulder(int x, int y);
    bool earth(int x, int y);
    bool closeToBld(int x, int y);
    void lightUp(int x, int y);
    Protester* closestProt(int x, int y);
    GraphObject::Direction exitDir(int x, int y) {return m_exitpath[x][y];}
    GraphObject::Direction tmDir(int x, int y) {return m_tmpath[x][y];}
    
private:
    struct Dir {
        Dir(int x, int y): m_x(x), m_y(y) {}
        int m_x;
        int m_y;
        int getX(){
            return m_x;
            
        }
        int getY(){
            return m_y;
            
        }
     };
    std::vector<Base*> m_act;
    std::vector<std::vector<Earth*>> m_earth;
    TunnelMan *m_tm;
    int m_ticks;
    int m_ticksb4Prot;
    int m_numProt;
    int m_barrels;

    //arrays of possible paths for protestors
    GraphObject::Direction m_exitpath[VIEW_WIDTH][VIEW_HEIGHT];
    GraphObject::Direction m_tmpath[VIEW_WIDTH][VIEW_HEIGHT];
};

#endif // STUDENTWORLD_H_
