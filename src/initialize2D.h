#ifndef initialize2D_H_INCLUDED
#define initialize2D_H_INCLUDED

#include <iostream>
#include <vector>

#include "util.h"
#include "wall.h"
#include "particle.h"

std::vector<Wall> setup_walls(std::vector<double> Lx, std::vector<double> Ly);
std::vector<Particle> create_random_particles(int N, std::vector<double> Lx, std::vector<double> Ly); 
std::vector<Particle> read_particles_from_file(string filename, double fac); 


#endif
