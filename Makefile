.PHONY: clean all run

CXX = g++
# CFLAGS = -std=c++17 -g   # for debug
CFLAGS = -Wall -Wextra -std=c++17 -MMD -MP -Iinclude
targets = main.exe
BUILD_DIR = build

VPATH = src

sources = $(wildcard *.cpp) $(wildcard src/*.cpp)
objects = $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(basename $(notdir $(sources)))))
deps = $(objects:.o=.d)

all: $(targets)
	@echo "Build complete."

run: $(targets)
	./$(targets) lhy 9 200 0

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

%.exe: $(objects)
	$(CXX) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c $< -o $@

-include $(deps)

clean:
	rmdir /s /q $(BUILD_DIR)
	del /f $(targets)
