CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default to dev build (dynamic linking)
TARGET = $(BUILD_DIR)/notify_dev
LDFLAGS = -lyaml-cpp -lcurl

# Production build (static linking)
TARGET_PROD = $(BUILD_DIR)/notify_prod
LDFLAGS_PROD = /usr/local/lib/libyaml-cpp.a -ldl -pthread -lcurl
# the libyaml-cpp.a might be different for your installation

all: dev prod

dev: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

prod: $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET_PROD) $^ $(LDFLAGS_PROD)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/config.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run_dev: dev
	./$(TARGET)

run_prod: prod
	./$(TARGET_PROD)

.PHONY: all dev prod clean run_dev run_prod
