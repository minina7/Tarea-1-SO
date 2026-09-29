CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDLIBS   = -lpthread

SRC = $(wildcard src/*.cpp)

planificador: $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC) $(LDLIBS)

clean:
	rm -f planificador

.PHONY: clean