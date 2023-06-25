#ifndef wall_H_INCLUDED
#define wall_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "util.h"
#include "wall.h"

using std::string;

//Wall class:
//describes a wall in Cartesian space, placed at positions x1 and x2
//each wall can have one of the types contained in Selector wall_types, usually {"X", "Y"}
class Wall
{
  public:
    //constructors
    Wall();
    Wall(std::vector<double> L, Selector wall_axes);
    Wall(const Wall &other);

    //getters
    double Lmin() const;
    double Lmax() const;
    bool isAxis(string str) const;
    bool isAxis(int i) const;
    int axis() const;

    //modifiers
    void setLmin(double val);
    void setLmax(double val);
    void setAxis(string str);
    void setAxis(int i);

  private:
    double myLmin;
    double myLmax;
    Selector myWallAxes;
};



#endif
