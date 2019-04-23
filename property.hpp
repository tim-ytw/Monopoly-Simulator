#ifndef property_hpp
#define property_hpp

// C++
#include <string>


class Player;


using namespace std;


class Property
{

public:
  
  static const int kMaxPropertyLevel;

  const int kUpgradeCost;
  
  const int kCost;
  
  const string name;
  
  
  Player* owner;

  int upgrade_level;
  
  
  void Reset();
  
  int PassCost() const;
  
  int SalePrice() const;
  
  Property(int cost, string name);
};


#endif /* property_hpp */
