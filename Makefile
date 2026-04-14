.PHONY: clean all

CXX = g++
# CFLAGS = -std=c++17 -g	# for debug
CFLAGS = -Wall -Wextra -std=c++17
targets = main.exe
sources = $(wildcard *.cpp)
objects = $(patsubst %.cpp,%.o,$(sources))
# sources = main.cpp poker.cpp logic.cpp
# objects = main.o poker.o logic.o


run: $(targets)
	./$(targets)

%.exe: $(objects)
	$(CXX) $(CFLAGS) $^ -o $@
	
%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	del $(objects) *.exe