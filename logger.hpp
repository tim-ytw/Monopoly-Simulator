#ifndef logger_hpp
#define logger_hpp

// C++
#include <string>
#include <iostream>

using namespace std;

namespace Logger
{
  
  template<typename T>
  void Info(T msg)
  {
    cout << msg << endl;
  }
  
  
  template<typename T, typename... Args>
  void Info(T msg, Args... args)
  {
    cout << msg;
    
    Info(args...);
  }
  
  template<typename T>
  void Debug(T msg)
  {
    cout << msg << endl;
  }
  
  
  template<typename T, typename... Args>
  void Debug(T msg, Args... args)
  {
    cout << msg;
    
    Debug(args...);
  }
  
};


#endif /* logger_hpp */
