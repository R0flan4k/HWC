CXX = g++
CXXFLAGS ?= -O2 -std=c++2a
OUT_O_DIR ?= obj
IFLAGS = -I./include
TESTS = ./tests
SRCDIR = ./src
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

override CXXFLAGS += $(IFLAGS)

SRC := perfect_caching.cpp
OBJ := $(addprefix $(OUT_O_DIR)/,$(SRC:.cpp=.o))
OUT := PC.x 

.PHONY: all
all: $(OUT_O_DIR)/$(OUT)

$(OUT_O_DIR)/$(OUT): $(OBJ)
	@$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ) : $(OUT_O_DIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm -rf $(OBJ) $(OUT_O_DIR)/*.x

TESTFILES=$(wildcard $(TESTS)/*.dat)

.PHONY: testrun
testrun: $(TESTFILES)

.PHONY: $(TESTFILES)
$(TESTFILES): $(OUT_O_DIR)/$(OUT)
	@$(ROOT_DIR)/runtest.sh $@ $(OUT_O_DIR)/$(OUT)