#ifndef collision_system_H_INCLUDED
#define collision_system_H_INCLUDED

#include <iostream>
#include <queue>  // std::priority_queue
#include <vector>

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#include "util.h"
#include "wall.h"
#include "particle.h"
#include "event.h"



class CollisionSystem
{
  public:
    CollisionSystem(std::vector<Particle> particles, std::vector<Wall> walls, double dt);
    void predict(Particle& p, double tmax);
    void simulate(double tmax, sf::RenderWindow &window);
    void draw(double tmax, sf::RenderWindow &window);

  private:
    std::vector<Particle> myParticles; 
    std::vector<Wall> myWalls;
    std::priority_queue<Event> myPQ;
    double myTime;
    double myDeltaT;
};


#endif
