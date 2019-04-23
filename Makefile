all: sim

CXX = g++

CXXFLAGS = -g -std=c++11

clean:
	rm sim

sim:
	$(CXX) $(CXXFLAGS) -o sim main.cpp player.cpp game.cpp property.cpp logger.cpp
