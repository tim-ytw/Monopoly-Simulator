// C++
#include <string>
#include <sstream>
#include <iostream>


// Local
#include "logger.hpp"

using namespace std;


void Logger::Info(string msg)
{
  cout << msg << endl;
};



void Logger::Debug(string msg)
{
  cout << msg << endl;
}

