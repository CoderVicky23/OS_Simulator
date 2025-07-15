# ============================ Makefile ============================

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Folders
SRC_DIR = src
INCLUDE_DIR = include
BIN = os_simulator

# Source files
SRCS = $(SRC_DIR)/memory_manager.cpp \
       $(SRC_DIR)/process_queues.cpp \
       $(SRC_DIR)/scheduler.cpp \
       main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target
all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o *.o $(BIN)

# ===================================================================
# Usage:
#   make         → builds the simulator executable
#   make clean   → removes object files and executable
