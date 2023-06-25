#ifndef util_H_INCLUDED
#define util_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // std::find
#include <iterator>  // std::distance

#include <Eigen/Dense>

using std::string;
using Eigen::VectorXd;

//Error handling
void check_sizes_equal(int valA, int valB, string msg);
void check_sizes_equal(double valA, double valB, string msg);
void check_space_exists(int i, int max, string msg);

//Comparing doubles
bool dbEqual(const double a, const double b);
bool dbLess(const double a, const double b);
bool dbGreater(const double a, const double b);
bool dbLessEqual(const double a, const double b);
bool dbGreaterEqual(const double a, const double b);

//Comparing Eigen::VectorXd
bool vxdEqual(const VectorXd a, const VectorXd b);

//Selector class: 
//contains a number of options represented by indices <options> and names <aliases>
//only one option is set to TRUE, others are set to FALSE
//we use Selector when we've say N options available, and only one of them can be correct
//
//for instance, suppose we've [symmetry] = "right", "left", "centred"; 
//this is not a boolean var so we wanna find out which option [symmetry] is equal to, 
//we need to use string comparisons instead that are more cumbersome, prone to typos, etc
//Selector class offers an easier to determine which option has been specified 
//(in the future might extend to doubles, etc)
class Selector
{
  public:
    //constructor
    Selector();
    Selector(int size, int i);
    Selector(std::vector<string> aliases, string str);

    //getters
    bool isOption(int i) const;
    bool isOption(string str) const;
    int option() const;

    //modifiers
    void setOption(int i);
    void setOption(string str);

  private:
    std::vector<bool> myOptions;
    std::vector<string> myAliases; 
    int mySize;
};



template <class T>
int search_vector(std::vector<T> vec, T val)
{
  //iterate over vector searching for item = str
  auto iter = std::find(vec.begin(), vec.end(), val);

  if (iter == vec.end()) {
     //if we've reached the end -- item not found
     printf("search_vector: val not found");
     exit(EXIT_FAILURE);
  }
  else {
     //item found, calculate its distance from the beginning
     int i = std::distance(vec.begin(), iter);
     return i;
  }
}


#endif
