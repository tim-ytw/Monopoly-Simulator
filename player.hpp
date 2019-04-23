#ifndef player_hpp
#define player_hpp

// C++
#include <queue>
#include <vector>
#include <string>

// Local
#include "property.hpp"


using namespace std;

struct PropertyComparator
{
  // More expensive properties are placed behind cheaper ones.
  bool operator()(Property * prop1, Property * prop2)
  {
    return (prop1->kCost > prop2->kCost);
  }
};


// The queue keeps track of the cheapest property
// It will be sold when a player cannot afford a charge
typedef priority_queue<Property*, vector<Property*>, PropertyComparator> Properties;


class Player
{
  const int kInitMoney;
  
  int money_ = 0;
  
  bool bankrupted_ = false;
  
  Properties properties_;
  
public:
  
  const string name;
  
  Player(int init_money, string name = "Unnamed");

  void reset();
  
  bool Bankrupted() { return bankrupted_; }
  
  // Pay up or go bankrupt
  bool Charge(int amount);
  
  void GrantMoney(int amount);
  
  // Must be its own property
  void Handle(Property* property);
  
  // Successfully purchased it
  void GrantProperty(Property* property);
  
  // Must be a property that has no owner
  bool WillingToBuy(const Property* property);
};



#endif /* player_hpp */
