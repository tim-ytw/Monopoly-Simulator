#ifndef logger_hpp
#define logger_hpp

// C++
#include <string>


using namespace std;


class Logger
{
  
public:
  
  static void Info(string msg);
  
  static void Debug(string msg);
  
};


#endif /* logger_hpp */
