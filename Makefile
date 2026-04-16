.PHONY: clean all

CXX = g++
# CFLAGS = -std=c++17 -g	# for debug
CFLAGS = -Wall -Wextra -std=c++17 -MMD -MP
targets = main.exe
sources = $(wildcard *.cpp)
objects = $(patsubst %.cpp,%.o,$(sources))
deps = $(objects:.o=.d)
# sources = main.cpp poker.cpp logic.cpp
# objects = main.o poker.o logic.o


run: $(targets)
	./$(targets)

%.exe: $(objects)
	$(CXX) $(CFLAGS) $^ -o $@
	
%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

-include $(deps)

clean:
	del $(objects) *.exe *.d