#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#include "util.h"
#include "wall.h"
#include "particle.h"

using Eigen::VectorXd;


//default constructor
Particle::Particle(void) 
{ }


//initialize particle
Particle::Particle(VectorXd position, VectorXd velocity, double radius, double mass, std::tuple<int,int,int> colour)
{
  myPosition = position;
  myVelocity = velocity;
  myRadius   = radius;
  myMass     = mass;
  myColour   = colour;

  myCount = 0;
  myINFINITY = std::numeric_limits<double>::infinity();
}


//copy another particle
Particle::Particle(const Particle &other)
{
  myPosition = other.myPosition;
  myVelocity = other.myVelocity;
  myRadius   = other.myRadius;
  myMass     = other.myMass;
  myColour   = other.myColour;

  myCount = 0;
  myINFINITY = std::numeric_limits<double>::infinity();
}


//get methods
VectorXd Particle::position(void)  { return myPosition; }
VectorXd Particle::velocity(void)  { return myVelocity; }
double   Particle::radius(void)    { return myRadius; }
double   Particle::mass(void)      { return myMass; }
int      Particle::count(void)     { return myCount; }


//draw method
void Particle::draw(sf::RenderWindow &window) 
{
  const float fradius = (float) myRadius;  
  const float fx      = (float) myPosition(0); 
  const float fy      = (float) myPosition(1); 

  //create a ball
  sf::CircleShape ball(fradius);
 
  //set colour
  sf::Color colour(std::get<0>(myColour), 
                   std::get<1>(myColour), 
                   std::get<2>(myColour));

  //set ball params
  ball.setOutlineThickness(0.25f*fradius);
  ball.setOutlineColor(sf::Color::Black);
  ball.setFillColor(colour); 
  ball.setOrigin(ball.getRadius(), ball.getRadius());

  //draw particle
  ball.setPosition(fx,fy);  
  window.draw(ball);
}



bool Particle::operator== (const Particle &other) const
{ //overloading "==" operator for particle class
  return (vxdEqual(myPosition, other.myPosition) && vxdEqual(myVelocity, other.myVelocity) &&
           dbEqual(myRadius,   other.myRadius)   &&  dbEqual(myMass,     other.myMass));
}


void Particle::move(double dt)
{ //update particle position r -> r + v*dt
  myPosition = myPosition + myVelocity * dt;
}


double Particle::timeToHit(const Particle &other) const
{ //calc time interval dt till the next particle-particle collision
  //if two particles are the same --> no collision (t_col -> inf)
  if (*this == other) {
     return myINFINITY;
  }

  //get position && velocity differences dr=rP-r, dv=vP-v
  VectorXd dr = other.myPosition - myPosition;
  VectorXd dv = other.myVelocity - myVelocity;

  //particle-particle collision occurs when dr(t+dt) * dr(t+dt) = sigma^2, 
  //                                   with dr(t+dt) = dr + dv*dt 
  // 
  //--> solving quadratic eqn gives dt = [-dvdr \pm sqrt(d)] / dvdv
  //    with d = (dvdr)^2 - (drdr - sigma^2)*dvdv
  //
  //--> if dvdr >= 0, because of sqrt(d) <= |dvdr| we've dt<0 no pos sol       --> no col, set dt = INF
  //--> if dvdv = 0,                               means dt->INF no finite sol --> no col, set dt = INF
  //--> if d<0,                                    means dt=CMPLX  no real sol --> no col, set dt = INF
  //
  //--> since only dvdr < 0 is valid, we take "-" sqrt sol to get the earliest col-time
  //    which gives dt = -[dvdr + sqrt(d)] / dvdv

  //calc dv * dr (set dt=INF if invalid)
  double dvdr = dr.transpose() * dv;
  if (dvdr > 0) {
     return myINFINITY;
  }

  //calc dv * dv (set dt=INF if invalid)
  double dvdv = dv.transpose() * dv; 
  if (dvdv == 0) {
     return myINFINITY;
  }

  //calc dr * dr and [sigma = col-radius, sigma = R + RP] 
  double drdr  = dr.transpose() * dr; 
  double sigma = myRadius + other.myRadius;

  //calc d (set dt=INF if invalid)
  double d = dvdr*dvdr - dvdv*(drdr - sigma*sigma);
  if (d < 0) {
     return myINFINITY;
  }

  //return time interval till the next collision: dt = -[dvdr + sqrt(d)] / dvdv
  return -(dvdr + sqrt(d)) / dvdv;  
}



double Particle::timeToHitWall(const Wall &wall) const
{ //calc time interval dt till the next particle-wall collision
      
  //particle-wall collision occurs when r + v*dt = L
  //where L = effective pos of wall with col-radius factored in: [L=L0-radius] or [L=L0+radius] 
  //
  //-->  solution is dt = |(r - Lmax + radius) / v| for v > 0 (towards the upper boundary)
  //-->              dt = |(r - Lmin - radius) / v| for v < 0 (towards the lower boundary)
  //-->  no col if v=0, set dt=INF
  int i = wall.axis();
  check_space_exists(i, myPosition.size(), "timeToHitWall -- wall axis outside dim range ");

  double r = myPosition(i);
  double v = myVelocity(i);
  
  if      (v > 0) { return std::abs((r - wall.Lmax() + myRadius) / v); }
  else if (v < 0) { return std::abs((r - wall.Lmin() - myRadius) / v); }
  else            { return myINFINITY; }
}



void Particle::bounceOff(Particle &other)
{ //particle-particle collision resolution: update particle velocities after col
  //get position && velocity differences dr=rP-r, dv=vP-v
  VectorXd dr = other.myPosition - myPosition;
  VectorXd dv = other.myVelocity - myVelocity;    

  //calc dv * dr and [sigma = col-radius, sigma = R + RP] 
  double dvdr = dr.transpose() * dv;
  double sigma = myRadius + other.myRadius;

  //calc total momentum change P = m*(v1-v) = -mP*(vP1-vP) of each particle
  //can do calc as follows:
  //--> all the momentum change is on axis normal to the surface
  //    so project particle velocities onto this axis: vn = v*n = v*dr/sigma, cause |dr| = sigma
  //    and for normal && tangential components:       v^2 = vn^2 + vt^2 
  //    reduces vector eqns to scalar on a single axis
  //
  //--> now write down momentum && energy conservation on this axis
  //--> solving for v1 gives P = 2*m*mP * (dv*dr) / ((m+mP)*sigma)
  double P = 2 * myMass * other.myMass * dvdr / ((myMass + other.myMass) * sigma);

  //calc x,y projections of P --> cos(&) = dx/sigma, sin(&) = dy/sigma
  VectorXd Pvec = dr * (P / sigma);

  //update velocities after col: vx -> vx + Px/m, etc
  myVelocity       += Pvec / myMass;
  other.myVelocity -= Pvec / other.myMass;

  //update col-count of both particles
  myCount+=1;
  other.myCount+=1;
}


void Particle::bounceOffWall(const Wall &wall)
{ //particle-wall collision resolution: update particle velocity after col (simple reflection)
  int i = wall.axis();
  check_space_exists(i, myVelocity.size(), "bounceOffWall -- wall axis outside dim range ");  

  double v = myVelocity(i);
  myVelocity(i) = -v;
  myCount+=1;
}


double Particle::kineticEnergy() const
{ //calc kinetic energy of the particle
  double vv = myVelocity.transpose() * myVelocity;
  return  0.5 * myMass * vv; 
}

























































