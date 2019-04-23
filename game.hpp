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
  
  
  vector<int> locations_;
  
  vector<Player*>& players_;
 
  vector<Property*>& properties_;
  
  
  Player* GetWinner();
  
  string GetLocationName(int location);
  
public:
  
  Game(vector<Player*>& players, vector<Property*>& properties);
  
  // Run the game until there is only one player left - Monopoly
  Result Run();
  
};


#endif /* game_hpp */
