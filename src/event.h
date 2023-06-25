#ifndef event_H_INCLUDED
#define event_H_INCLUDED

#include <iostream>
#include <vector>

#include "util.h"
#include "wall.h"
#include "particle.h"


class Event
{
  public:
    //constructor && copy constructor
    Event();
    Event(double t, Particle* pA, Particle* pB); 
    Event(double t, Particle* particle, const Wall &wall); 
    Event(double t); 
    Event(const Event &other);  

    //comparison
    bool operator< (const Event &other) const;

    //validation
    bool isValid();

    //get methods
    int particleNum();
    double time();
    Wall wall();
    Particle* A();
    Particle* B();
    int countA();
    int countB();

  private:
    double myTime;
    int myCountA, myCountB;
    Particle* myA; 
    Particle* myB;
    Wall myWall;
    int myParticleNum;  
};    



#endif




