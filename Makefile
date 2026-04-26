.PHONY: clean all run

CXX = g++
# CFLAGS = -std=c++17 -g	# for debug
CFLAGS = -Wall -Wextra -std=c++17 -MMD -MP -Iinclude
targets = main.exe

VPATH = src

sources = $(wildcard *.cpp) $(wildcard src/*.cpp)
objects = $(addsuffix .o,$(basename $(notdir $(sources))))
deps = $(objects:.o=.d)

all: $(targets)
	@echo "Build complete."

run: $(targets)
	./$(targets) lhy 9 200 0

%.exe: $(objects)
	$(CXX) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

-include $(deps)

clean:
	del $(objects) $(deps) $(targets)
