// C++
#include <vector>
#include <iostream>

// Local
#include "game.hpp"
#include "logger.hpp"
#include "player.hpp"


using namespace std;


const string kPropertyNames[] =
{
  "D.C.",
  "Wien",
  "Rome",
  "Seoul",
  "Paris",
  
  "Tokyo",
  "Zürich",
  "Dublin",
  "Berlin",
  "London",
  
  "Ottawa",
  "Prague",
  "Mumbai",
  "Warsaw",
  "Beijing",
  
  "Canberra",
  "Stockholm",
  "København",
  "Jerusalem",
  "Wellington",
};


// Return a number between 1000 and 5000, divisible by 500
int GetRandomCost()
{
  return 1000 + (rand() % 9) * 500;
}


int main(int argc, const char * argv[])
{
  unsigned int rand_seed = 0;
  
  if (argc > 1)
  {
    rand_seed = atoi(argv[1]);
  }
  
  Logger::Info("Random seed: " + to_string(rand_seed));
  
  srand(rand_seed);
  
  
  vector<Player*> players;
  
  for (int i = 1; i <= 4; ++ i)
  {
    players.push_back(new Player(2000, "Player " + to_string(i)));
  }
  
  vector<Property*> properties;
  
  properties.push_back(new Property(0, "start"));
  
  for (auto city_name : kPropertyNames)
  {
    auto cost = GetRandomCost();
    
    properties.push_back(new Property(cost, city_name));
  }
  
  Game game(players, properties);
  
  auto result = game.Run();
  
  auto winner = result.winner;
  
  Logger::Info("After ", result.rounds, " rounds, the monopoly is ", winner->name);
  
  for (auto player :players)
  {
    delete player;
  }
  
  for (auto property : properties)
  {
    delete property;
  }
  
  return 0;
}
