// C++
#include <queue>
#include <vector>
#include <string>

// Local
#include "logger.hpp"
#include "player.hpp"
#include "property.hpp"


using namespace std;


Player::Player(int init_money, string n)
: kInitMoney(init_money), name(n)
{
  Reset();
}


void Player::Reset()
{
  location_ = 0;
  
  bankrupted_ = false;
  
  while (!properties_.empty()) SellCheapestProperty();
  
  money_ = kInitMoney;
}



void Player::GrantMoney(int amount)
{
  assert(amount > 0);
  
  money_ += amount;
}



bool Player::WillingToBuy(const Property* property)
{
  return (money_ > property->kCost);
}



void Player::GrantProperty(Property* prop)
{
  prop->owner = this;
  
  properties_.push(prop);
}


// Update this property if possible
void Player::Handle(Property* property)
{
  // Must be handling its own property
  assert(property->owner == this);
  
  // Cannot be leveled up
  if (property->upgrade_level == Property::kMaxPropertyLevel) return;
  
  // Cannot afford an upgrade
  if (property->kUpgradeCost > money_) return;
  
  money_ -= property->kUpgradeCost;
  
  property->upgrade_level ++;
}



// Pay up or go bankrupt
bool Player::Charge(int amount)
{
  assert(amount >= 0);
  
  // Can afford the charge with money in hand
  if (money_ > amount)
  {
    money_ -= amount;
   
    Logger::Debug("  was charged ", amount, " and now has ", money_, " dollars left");
    
    return true;
  }
  
  // Insufficient fund and no assets to liquidize
  if (properties_.size() == 0)
  {
    bankrupted_ = true;
    
    return false;
  }
  
  Logger::Debug("  has only ", money_, " dollars");

  // Must sell some properties to get cash.
  SellCheapestProperty();
  
  // Try again.
  return Charge(amount);
}



void Player::SellCheapestProperty()
{
  auto cheapest_prop = properties_.top();
  properties_.pop();
  
  money_ += cheapest_prop->SalePrice();
  
  Logger::Debug("  sold ", cheapest_prop->name, " at ", cheapest_prop->SalePrice());
  
  Logger::Debug("  now has ", money_, " dollars");
  
  // Sell it (to the bank)
  cheapest_prop->Reset();
}
