CXX = g++
CXXFLAGS ?= -O2
OUT_O_DIR ?= obj
IFLAGS = -I./include
TESTS = ./tests
SRCDIR = ./src
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

override CXXFLAGS += $(IFLAGS)

SRC := $(SRCDIR)/LFU_Cache.cpp
OBJ := $(addprefix $(OUT_O_DIR)/,$(SRC:.cpp=.o))
OUT := HWC.x 

.PHONY: all
all: $(OUT_O_DIR)/$(OUT)

$(OUT_O_DIR)/$(OUT): $(OBJ)
	@$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ) : $(OUT_O_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm -rf $(COBJ) $(OUT_O_DIR)/*.x