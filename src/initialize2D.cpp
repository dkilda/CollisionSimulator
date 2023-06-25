#include <iostream>
#include <vector>
#include <random>
#include <fstream>

#include <Eigen/Dense>

#include "util.h"
#include "wall.h"
#include "particle.h"
#include "initialize2D.h"

using Eigen::VectorXd;
using std::string;


std::vector<Wall> setup_walls(std::vector<double> Lx, std::vector<double> Ly)  
{ //Create X,Y walls
  std::vector<Wall> walls;
  std::vector<string> axes = {"X", "Y"};
  walls.emplace_back(Wall(Lx, Selector(axes, "X")));  
  walls.emplace_back(Wall(Ly, Selector(axes, "Y")));
 
  return walls;
}




std::vector<Particle> create_random_particles(int N, std::vector<double> Lx, std::vector<double> Ly)
{ //create a random set of N particles
  //call random device
  std::random_device rd;
  std::mt19937 mt(rd());

  //generate random distributions for pos && vel
  std::uniform_real_distribution<double> rx_dist(Lx[0], Lx[1]);
  std::uniform_real_distribution<double> ry_dist(Ly[0], Ly[1]);
  std::uniform_real_distribution<double> vx_dist(-0.5, 0.5);
  std::uniform_real_distribution<double> vy_dist(-0.5, 0.5);

  //set default vals of radius, mass, colour
  double radius = 8.0; 
  double mass = 10.0;
  auto colour = std::make_tuple(0,0,0);

  //generate N random particles
  std::vector<Particle> particles;
  for (int i=0; i<N; i++) {

      //random pos && vel vectors
      VectorXd position(2);
      position << rx_dist(mt), ry_dist(mt);
      VectorXd velocity(2);
      position << vx_dist(mt), vy_dist(mt);

      //create new particle, add it to the collection
      particles.emplace_back(Particle(position, velocity, radius, mass, colour));
  }

  return particles;
}



std::vector<Particle> read_particles_from_file(string filename, double fac) 
{ //read a set of particles from file

  //open file 
  std::ifstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open()) {
     std::cerr << "cannot open the input file ";
     exit(EXIT_FAILURE);
  }

  //parse first line
  string line;
  int N;
  std::getline(file, line);
  std::istringstream iss(line);
  iss >> N;

  //parse other lines
  std::vector<Particle> particles;
  while (std::getline(file, line)) {

      //read && parse next line     
      std::istringstream iss(line);
      double rx, ry;
      double vx, vy;
      double radius;
      double mass;
      int clrR, clrG, clrB;
      if (!(iss >> rx >> ry >> vx >> vy >> radius >> mass >> clrR >> clrG >> clrB)) { break; } 

      //create pos && vel vectors && colour tuple
      VectorXd position(2);
      position << rx, ry;
      VectorXd velocity(2);
      velocity << vx, vy;
      std::tuple<int,int,int> colour (clrR, clrG, clrB);

      //rescale values
      position = fac * position;
      velocity = fac * velocity; 
      radius   = fac * radius;  

      //create new particle
      particles.emplace_back(Particle(position, velocity, radius, mass, colour));
  }
  //close file
  file.close();

  return particles;
}



















