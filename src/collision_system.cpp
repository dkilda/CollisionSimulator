#include <iostream>
#include <queue>  // std::priority_queue
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#include "util.h"
#include "sfml_util.h"
#include "wall.h"
#include "particle.h"
#include "event.h"
#include "collision_system.h"

using Eigen::VectorXd;



CollisionSystem::CollisionSystem(std::vector<Particle> particles, std::vector<Wall> walls, double dt)  
{ //init simulation
  myTime = 0.0;
  myDeltaT = dt;   //0.1; //1.0; 
  myParticles = particles;
  myWalls = walls;
}



void CollisionSystem::predict(Particle &p, double tmax)
{
  int N  = myParticles.size(); 
  int NW = myWalls.size(); 

  //Collisions b/n particle p and other particles pX
  for (int i=0; i<N; i++) {
   
      //get dt after which p will hit particle-i
      double dt = p.timeToHit(myParticles[i]);     

      //if col-time doesnt go beyond tmax, put this col on PQ
      if (myTime + dt <= tmax)           
         myPQ.emplace(Event(myTime + dt, &p, &myParticles[i])); 
  }

  //Collisions b/n particle p and walls
  for (int i=0; i<NW; i++) {

      //loop over all walls, get dt after which p will hit the wall
      Wall wall = myWalls[i];
      double dt = p.timeToHitWall(wall);

      //if col-time doesnt go beyond tmax, put this col on PQ
      if (dbLessEqual(myTime + dt, tmax)) 
         myPQ.emplace(Event(myTime + dt, &p, wall));      
  }   
}


void CollisionSystem::draw(double tmax, sf::RenderWindow &window)
{ //Draw all particles in animation [window]
  //clear old view
  window.clear(sf::Color(30, 30, 120));

  //draw && display particles at new positions
  int N = myParticles.size();
  for (int i=0; i<N; i++) {
      myParticles[i].draw(window);
  }
  window.display();

  //place next drawing event on PQ
  if (myTime < tmax) 
     myPQ.emplace(Event(myTime + myDeltaT));
}



void CollisionSystem::simulate(double tmax, sf::RenderWindow &window)
{ //Run collision simulation
  Particle *pA, *pB;
  int N = myParticles.size();

  //Initialize PQ
  for (int i=0; i<N; i++) {
      predict(myParticles[i], tmax);
  }
  myPQ.emplace(Event(0.0)); 

  //The main event-driven simulation loop (until PQ is empty)
  while (not myPQ.empty() && window.isOpen()) {

      //Check for user events like mouse && button clicks
      check_user_events(window);

      //Remove from PQ next event, check if it's valid
      Event event = myPQ.top();
      myPQ.pop();
      if (not event.isValid()) 
         continue; 

      //Get particles that are involved in col-event
      //Take 0 or 1 or 2 particles depending on the num of particles in the event 
      if (event.particleNum() >= 1)      
         pA = event.A(); 
      if (event.particleNum() == 2)       
         pB = event.B();

      //Update system clock and particle positions in straight-line trajectory 
      //from [current simulation time] to [next event time]
      for (int i=0; i<N; i++) {
          myParticles[i].move(event.time() - myTime);
      }
      myTime = event.time(); 

      //Process collision event
      //Execute col --> update velocities of pA and pB involved in col (others stay the same)
      //Different functions are called for particle-particle and particle-wall
      //If no particles are involved, execute "drawing event"
      if      (event.particleNum() == 2) { pA->bounceOff(*pB); }
      else if (event.particleNum() == 1) { pA->bounceOffWall(event.wall()); }
      else                               { draw(tmax, window); }

      //Predict future collisions
      //Since pA,pB had their velocities updated by col 
      //and this pA-pB col has been executed && discarded from PQ
      //
      //--> get NEW next-col times for [pA, pB] with [all other particles]
      //--> update PQ with these NEW cols
      if (event.particleNum() >= 1)
         predict(*pA, tmax);
      if (event.particleNum() == 2)
         predict(*pB, tmax);
  }
}






















