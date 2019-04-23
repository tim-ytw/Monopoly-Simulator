// C++
#include <string>

// Local
#include "property.hpp"

const int Property::kMaxPropertyLevel = 5;


Property::Property(int init_cost, string n)
: name(n), kCost(init_cost), kUpgradeCost(init_cost / 5)
{
  Reset();
}


void Property::Reset()
{
  owner = nullptr;
  
  upgrade_level = 0;
}


int Property::PassCost() const
{
  return (upgrade_level + 1) * (kCost / 10);
}


int Property::SalePrice() const
{
  return kCost + (upgrade_level * kUpgradeCost / 2);
}

