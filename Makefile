.PHONY: clean all

# CFLAGS = -std=c++17 -g	# for debug
CFLAGS = -Wall -Wextra -std=c++17 -O2
targets = main.exe
sources = $(wildcard *.cpp)
objects = $(patsubst %.cpp,%.o,$(sources))
# sources = main.cpp poker.cpp logic.cpp
# objects = main.o poker.o logic.o


all: $(targets)
	@echo all done

%.exe: $(objects)
	g++ $(CFLAGS) $^ -o $@

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	del $(objects) *.exe