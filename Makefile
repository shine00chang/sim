CXX 		:= g++
CXXFLAGS 	:= `sdl2-config --libs --cflags` -I /opt/homebrew/Cellar/sdl2/2.28.1/include/SDL2 -lm -Wall -pedantic -g --std=c++20 -Wno-sign-compare -Wno-comment
OBJECTS 	:= 

all: clean run

run: prog
	./prog

prog: main.cpp $(foreach OBJ, $(OBJECTS), bin/$(OBJ).o)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(foreach OBJ, $(OBJECTS), bin/$(OBJ).o): bin/%.o: src/%.cpp bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin:
	mkdir -p bin

clean:
	rm -rf bin prog *.dSYM
