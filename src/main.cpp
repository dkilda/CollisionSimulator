#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#include "util.h"
#include "sfml_util.h"
#include "wall.h"
#include "particle.h"
#include "collision_system.h"
#include "initialize2D.h"

using std::string;


void mini_draw(std::vector<Particle> particles, sf::RenderWindow &window)
{
  window.clear(sf::Color(30, 30, 120));
  particles[0].draw(window);
  window.display();
}


void mini_simulator(std::vector<Particle> particles, sf::RenderWindow &window)
{
  while (window.isOpen()) {
      check_user_events(window);
      particles[0].move(0.01);
      mini_draw(particles, window);
  }
}



int main(int argc, char* argv[])
{
  //Check num of user args is correct
  //argv[0] -- name of exec
  //argv[1] -- read file if > 0, else do default initialization
  //argv[2] -- filename (if argv[1] > 0) or N of particles (else)
  //argv[3] -- tmax simulation time
  //argv[4] -- dt time interval
  check_sizes_equal(argc, 5, "main: num of args must be 5");

  //Box size
  std::vector<double> Lx = {0.0, 1300.0};
  std::vector<double> Ly = {0.0, 1000.0};

  //Set of particles and walls
  std::vector<Particle> particles;
  std::vector<Wall> walls = setup_walls(Lx, Ly);

  //Create animation window
  const double window_width  = Lx[1] - Lx[0];  
  const double window_height = Ly[1] - Ly[0]; 
  const int bpp = 32;
  sf::RenderWindow window(sf::VideoMode(window_width, window_height, bpp), "Collision Simulator");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(0);

  //Read particles from file or create N random particles
  bool read_file = (atoi(argv[1]) > 0);

  if (read_file) {
     string filename = argv[2];
     double fac = 1000.0;
     particles = read_particles_from_file(filename, fac);
  }
  else {
     int N = atoi(argv[2]);
     particles = create_random_particles(N, Lx, Ly);
  }

  //Run simulation
  double tmax = atof(argv[3]);
  double dt   = atof(argv[4]);
  CollisionSystem colsys = CollisionSystem(particles, walls, dt);
  colsys.simulate(tmax, window);
}



     //std::cout << "read-file clause: \n";
     //std::cout << read_file << ", " << filename << std::endl;
     //std::cout << std::endl;

  //std::cout << "args: \n";
  //std::cout << *argv << std::endl;
  //std::cout << tmax << std::endl;
  //std::cout << Lx[0] << ", " << Lx[1] << ", " << Lx.size() << std::endl; 
  //std::cout << Ly[0] << ", " << Ly[1] << ", " << Ly.size() << std::endl;
  //std::cout << std::endl;



  //mini_simulator(particles, window);




