#ifndef particle_H_INCLUDED
#define particle_H_INCLUDED

#include <iostream>
#include <vector>
#include <tuple>

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#include "util.h"
#include "wall.h"

using Eigen::VectorXd;


class Particle 
{
  public:
    //constructor && copy constructor
    Particle();
    Particle(VectorXd position, VectorXd velocity, double mass, double radius, std::tuple<int,int,int> colour); 
    Particle(const Particle &other);                 
 
    //get methods
    VectorXd position();  
    VectorXd velocity();
    double radius();
    double mass();
    int count();
    
    //draw particle
    void draw(sf::RenderWindow &window);

    //comparison
    bool operator== (const Particle &other) const;

    //collision prediction and processing
    void move(double dt);
    double timeToHit(const Particle &other) const;
    double timeToHitWall(const Wall &wall) const;
    void bounceOff(Particle &other);
    void bounceOffWall(const Wall &wall);

    //properties
    double kineticEnergy() const;

  private:
    VectorXd myPosition;
    VectorXd myVelocity;
    double myRadius;
    double myMass;
    std::tuple<int,int,int> myColour;

    int myCount;
    double myINFINITY;    
};


#endif
