#include <iostream>
#include <string>
#include <vector>

#include "util.h"
#include "wall.h"

using std::string;


//constructors
Wall::Wall(void) 
{ }

Wall::Wall(std::vector<double> L, Selector wall_axes)
{
  check_sizes_equal(L.size(), 2, "Wall constructor -- L size must equal 2: ");
  myLmin = L[0];
  myLmax = L[1];
  myWallAxes = wall_axes;
}

Wall::Wall(const Wall &other)
{
  myLmin = other.myLmin;
  myLmax = other.myLmax; 
  myWallAxes = other.myWallAxes;
}


//getters
double Wall::Lmin() const
{ return myLmin; }

double Wall::Lmax() const
{ return myLmax; }

bool Wall::isAxis(string str) const
{ return myWallAxes.isOption(str); }

bool Wall::isAxis(int i) const
{ return myWallAxes.isOption(i); }

int Wall::axis() const
{ return myWallAxes.option(); }


//modifiers
void Wall::setLmin(double val)
{ myLmin = val; }

void Wall::setLmax(double val)
{ myLmax = val; }

void Wall::setAxis(string str)
{ myWallAxes.setOption(str); }

void Wall::setAxis(int i)
{ myWallAxes.setOption(i); }









