#ifndef game_hpp
#define game_hpp

// C++
#include <vector>

// Local
#include "property.hpp"


class Player;


using namespace std;


struct Result
{
  int rounds;
  
  Player* winner;
 
  vector<Player*> losers;
};



class Game
{
  const int kBoardSize;
  
  vector<Player*>& players_;
 
  vector<Property*>& properties_;

  // Reset properties and players
  void Reset();
  
  Player* GetWinner();
  
  string GetLocationName(int location);
  
  // Each round contains 2 parts
  
  // 1) Move each player forward by random steps.
  void Move(Player * player);
  
  // 2) The player engages the space they stand upon
  void Engage(Player * player, Property * property);
  
public:
  
  Game(vector<Player *>& players, vector<Property *>& properties);
  
  // Run the game until there is only one player left - Monopoly
  Result Run();
};


#endif /* game_hpp */
