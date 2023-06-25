#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>  
#include <stdlib.h>
#include <cmath> 

#include <Eigen/Dense>
#include "util.h"

using std::string;
using Eigen::VectorXd;


//Check sizes equal (msg.c_str() converts string msg to char*)
void check_sizes_equal(int valA, int valB, string msg)
{
  if (valA != valB) {
     printf("%s: %d, %d", msg.c_str(), valA, valB);
     printf("\n");
     exit(EXIT_FAILURE);
  }
}   

void check_sizes_equal(double valA, double valB, string msg)
{
  if (valA != valB) {
     printf("%s: %E, %E", msg.c_str(), valA, valB);
     printf("\n");
     exit(EXIT_FAILURE);
  }
}   

//Check space exists (msg.c_str() converts string msg to char*)
void check_space_exists(int i, int max, string msg)
{
  if (i >= max) {
     printf("%s: %d, %d", msg.c_str(), i, max);
     printf("\n");
     exit(EXIT_FAILURE);
  }
}   


//Comparing doubles
bool dbEqual(const double a, const double b)
{ return std::abs(a-b) < 1e-08; }

bool dbLess(const double a, const double b)
{ return ((a < b) and not dbEqual(a,b)); }

bool dbGreater(const double a, const double b)
{ return ((a > b) and not dbEqual(a,b)); } 

bool dbLessEqual(const double a, const double b)
{ return (not dbGreater(a,b)); }

bool dbGreaterEqual(const double a, const double b)
{ return (not dbLess(a,b)); }


//Comparing Eigen::VectorXd
bool vxdEqual(const VectorXd a, const VectorXd b)
{ return (a-b).isMuchSmallerThan(1e-08); }


//Selector class methods
Selector::Selector(void) 
{ }

Selector::Selector(int size, int i)
{ //selector with option-i = TRUE
  mySize = size;
  myAliases.assign(mySize, "NULL"); 
  myOptions.assign(mySize,  false); 
  myOptions.at(i) = true;
}

Selector::Selector(std::vector<string> aliases, string str)
{ //selector with option-str = TRUE
  myAliases = aliases;
  mySize = aliases.size();
  myOptions.assign(mySize, false);

  //find the index of option-str, set it to TRUE
  int i = search_vector<string>(aliases, str);
  myOptions.at(i) = true;
}


//modifiers
void Selector::setOption(int i)
{ //set option-i to TRUE
  myOptions.assign(mySize, false);  
  myOptions.at(i) = true;
}

void Selector::setOption(string str)
{ //set option-str to TRUE, we need to search for index corresponding to [str] 
  myOptions.assign(mySize, false);  
  int i = search_vector<string>(myAliases, str);
  myOptions.at(i) = true;
}


//getters
bool Selector::isOption(int i) const
  //check if option-i is TRUE
{ return myOptions.at(i); }

bool Selector::isOption(string str) const
{ //check if option-str is TRUE
  int i = search_vector<string>(myAliases, str);
  return myOptions.at(i); 
}

int Selector::option() const
  //return index of the TRUE option
{ return search_vector<bool>(myOptions, true); }











































 






