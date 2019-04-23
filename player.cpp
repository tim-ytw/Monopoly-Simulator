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
  reset();
}


void Player::reset()
{
  money_ = kInitMoney;
  
  bankrupted_ = false;
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
  
  static auto log = Logger::Debug;
  
  // Can afford the charge with money in hand
  if (money_ > amount)
  {
    money_ -= amount;
   
    log ("  was charged " + to_string(amount) + " now has " + to_string(money_) + " dollars");
    
    return true;
  }
  
  // Insufficient fund and no assets to liquidize
  if (properties_.size() == 0)
  {
    bankrupted_ = true;
    
    return false;
  }

  // Must sell some properties to get cash.
  
  auto cheapest_prop = properties_.top();
  properties_.pop();
  
  money_ += cheapest_prop->SalePrice();
  
  log ("  sold " + cheapest_prop->name + " at " + to_string(cheapest_prop->SalePrice()));
  
  log ("  now has " + to_string(money_) + " dollars");
  
  // Sell it (to the bank)
  cheapest_prop->Reset();
  
  // Try again.
  return Charge(amount);
}


