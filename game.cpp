// C++
#include <vector>
#include <string>

// Local
#include "game.hpp"
#include "player.hpp"
#include "logger.hpp"
#include "property.hpp"


using namespace std;

const int kOriginPassGrant = 500;


Game::Game(vector<Player*>& players, vector<Property*>& properties)
: players_(players), properties_(properties), kBoardSize(properties.size())
{
  Reset();
}



void Game::Reset()
{
  for (auto player : players_)
  {
    player->Reset();
  }
  
  for (auto property : properties_)
  {
    property->Reset();
  }
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
  
  auto prop = properties_[location];
  
  string owner = (prop->owner == nullptr) ? "(No owner)" : "(" + prop->owner->name + "'s)";
  
  return prop->name + " Lv" + to_string(prop->upgrade_level) + " - " + owner;
}



void Game::Move(Player * player)
{
  
  Logger::Debug(player->name, " at ", GetLocationName(player->location_));
  
  auto steps = rollDice(6);
  
  player->location_ += steps;
  
  // Pass by the origin -> Receive money
  if (player->location_ >= kBoardSize)
  {
    player->GrantMoney(kOriginPassGrant);
    
    player->location_ = (player->location_ % kBoardSize);
    
    Logger::Debug("  received origin pass grant");
  }
  
  Logger::Debug("  moves ", steps, " steps to ", GetLocationName(player->location_));
}



void Game::Engage(Player* player, Property* property)
{
  if (property->name == "start") return;
  
  // The property is available for sale
  if (property->owner == nullptr)
  {
    Logger::Debug(property->name, " is available at ", property->kCost);
    
    if (player->WillingToBuy(property) && player->Charge(property->kCost))
    {
      player->GrantProperty(property);
      
      Logger::Debug("  bought it");
      
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
    
    Logger::Debug("  needs to pay ", pass_cost);
    
    if (player->Charge(pass_cost))
    {
      assert(!property->owner->Bankrupted());
      
      property->owner->GrantMoney(pass_cost);
      
      Logger::Debug("  paid to ", property->owner->name);
    }
  }
}


Result Game::Run()
{
  Result result;
  
  // Run the game until there is only one player left - Monopoly
  while (true)
  {
    result.rounds ++;
    
    Logger::Debug("\nRound ", result.rounds);
    
    // Each player rolls the dice and takes random steps
    for (int id = 0; id < players_.size(); id ++)
    {
      auto player = players_[id];
      
      if (player->bankrupted_) continue;
      
      Move(player);
      
      Property* property = properties_[player->location_];
      
      Engage(player, property);
      
      if (player->bankrupted_)
      {
        Logger::Debug("  bankrupted");
       
        result.losers.push_back(player);
      }
    }
    
    result.winner = GetWinner();
    
    if (result.winner != nullptr) break;
  }
  
  return result;
}


