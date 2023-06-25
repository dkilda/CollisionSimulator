#include <iostream>
#include <vector>
#include <cmath>

#include "util.h"
#include "wall.h"
#include "particle.h"
#include "event.h"

using Eigen::VectorXd;

//Event class --> initialize event
Event::Event(void)
{ }

Event::Event(double t, Particle* pA, Particle* pB)
{ //create particle-particle col-event at time = t
  myTime = t;
  myA = pA;
  myB = pB;

  myCountA = myA->count();
  myCountB = myB->count();
  myParticleNum = 2;
}

Event::Event(double t, Particle* particle, const Wall& wall)
{ //create particle-wall col-event at time = t
  myTime = t;
  myA = particle;
  myWall = wall;

  myCountA = myA->count();
  myParticleNum = 1;
}

Event::Event(double t)
{ //create drawing-event at time = t
  myTime = t;
  myCountA = -1;
  myCountB = -1;

  myParticleNum = 0;
}

Event::Event(const Event &other)
{ //Event copy constructor
  myTime = other.myTime;
  myParticleNum = other.myParticleNum;

  //For N=2 particles
  if (myParticleNum == 2) {
     myB = other.myB;
     myCountB = other.myCountB;
  } 
  //For N>=1 particles
  if (myParticleNum >= 1) {
     myA = other.myA;
     myCountA = other.myCountA;
  }
  //For N=1 particles
  if (myParticleNum == 1) {
     myWall = other.myWall;
  } 
}



//get methods
int Event::particleNum()
//num of particles involved in the event
{ return myParticleNum; }

double Event::time()
//event time
{ return myTime; }

Wall Event::wall()
//wall
{ return myWall; }

Particle* Event::A()
//particle-A
{ return myA; }

Particle* Event::B()
//particle-B
{ return myB; }

int Event::countA()
//col-count of A
{ return myCountA; }

int Event::countB()
//col-count of B
{ return myCountB; }




//comparison operator
bool Event::operator< (const Event &other) const
//Compare event times -- event with earlier time gets priority 
//(comparison reversed to get MinPQ order -- STL gives MaxPQ order by default)
{ return (myTime > other.myTime); }

//dbGreater(time(), other.time()); } FIXME
//{ return (myTime < other.myTime); }




//validity checks
bool Event::isValid()
{ //Check if a given col-event is still valid (i.e. not invalidated by other cols)

  //--> event.count[A,B] = col-count of A,B at the time the event was created
  //--> p[A,B].count = current col-count of A,B
  //--> if these counts don't match, 
  //    means other cols involving A or B have happened 
  //    between the creation of this col-event, and the current time
  //--> hence this event has been invalidated and should be ignored  

  //check if any events involving particles-A,B have happened since the creation of event
  //It's important to check for such invalidated events in a many-event simulation, 
  //where some events can invalidate others
  if (myParticleNum >= 1) {   
     if (myA->count() != myCountA)
        return false;
  } 
  if (myParticleNum == 2) {  
     if (myB->count() != myCountB) 
        return false; 
  }
  return true; 
}












