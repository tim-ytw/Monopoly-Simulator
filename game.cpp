// C++
#include <vector>
#include <string>

// Local
#include "game.hpp"
#include "player.hpp"
#include "logger.hpp"
#include "property.hpp"


using namespace std;


Game::Game(vector<Player*>& players, vector<Property*>& properties)
: players_(players), properties_(properties), kBoardSize(properties.size())
{
  // Everyone starts at the origin
  locations_ = vector<int>(players.size(), 0);
}



Player* Game::GetWinner()
{
  Player * winner = nullptr;
  
  for (auto player : players_)
  {
    if (player->Bankrupted()) continue;
    
    // There are at least 2 players left unbankrupted
    if (winner != nullptr) return nullptr;
    
    winner = player;
  }
  
  return winner;
}



inline int rollDice(int max)
{
  return (rand() % max) + 1;
}


string Game::GetLocationName(int location)
{
  if (location == 0) return "Starting Point";
  
  auto prop = properties_[location - 1];
  
  string owner = (prop->owner == nullptr) ? "(No owner)" : "(" + prop->owner->name + "'s)";
  
  return prop->name + " Lv" + to_string(prop->upgrade_level) + " - " + owner;
}


const int kOriginPassGrant = 500;


Result Game::Run()
{
  Result result;

  auto log = Logger::Debug;
  
  // Run the game until there is only one player left - Monopoly
  while (true)
  {
    result.rounds ++;
    
    log("\nRound " + to_string(result.rounds));
    
    // Each player rolls the dice and takes random steps
    for (int id = 0; id < players_.size(); id ++)
    {
      auto player = players_[id];
      
      if (player->Bankrupted()) continue;

      auto loc_idx = locations_[id];
      
      log(player->name + " at " + GetLocationName(loc_idx));
      
      auto steps = rollDice(6);
      
      locations_[id] += steps;
      
      // Pass by the origin -> Receive money
      if (locations_[id] > properties_.size())
      {
        player->GrantMoney(kOriginPassGrant);
        
        locations_[id] = (locations_[id] % kBoardSize);
      
        log("  received origin pass grant");
      }
      
      
      loc_idx = locations_[id];
      
      log("  moves " + to_string(steps) + " steps to " + GetLocationName(loc_idx));
      
      auto property_index = locations_[id] - 1;
      
      Property* property = properties_[property_index];
      
      // The property is available for sale
      if (property->owner == nullptr)
      {
        log(property->name + " is available at " + to_string(property->kCost));
        
        if (player->WillingToBuy(property) && player->Charge(property->kCost))
        {
          player->GrantProperty(property);
          
          log("  bought it");
          
          assert(property->owner == player);
        }
      }
      
      // The player decides how to handle its own property.
      else if (property->owner == player)
      {
        player->Handle(property);
      }
      
      // The player must be charged for passing by other's property.
      else
      {
        int pass_cost = property->PassCost();
        
        log("  will be charged " + to_string(pass_cost));
        
        if (player->Charge(pass_cost))
        {
          assert(!property->owner->Bankrupted());
          
          property->owner->GrantMoney(pass_cost);
          
          log("  paid to " + property->owner->name);
        }
        else if (player->Bankrupted())
        {
          log ("  bankrupted");
          
          result.losers.push_back(player);
        }
      }
    }
    
    result.winner = GetWinner();
    
    if (result.winner != nullptr) break;
  }
  
  log("\nThe monopoly is " + result.winner->name);
  
  return result;
}
