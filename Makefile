CXX := c++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -I src

BUILD_DIR := build
TARGET := $(BUILD_DIR)/GameEngine72

SOURCES := \
	src/Engine/Application.cpp \
	src/Sandbox/SandboxApp.cpp

OBJECTS := $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all run clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
