MAKE = make
MAKEFLAGS = --no-print-directory
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
DIRS = $(ROOT_DIR)/LFU $(ROOT_DIR)/perfect_caching 
OBJ = $(wildcard $(ROOT_DIR)/LFU/obj/*.o) $(wildcard $(ROOT_DIR)/perfect_caching/obj/*o)

.PHONY: all clean
all: 
	@$(MAKE) $(MAKEFLAGS) -C $(ROOT_DIR)/LFU
	@$(MAKE) $(MAKEFLAGS) -C $(ROOT_DIR)/perfect_caching

testrun: all
	@echo LFU:
	@$(MAKE) $(MAKEFLAGS) -C $(ROOT_DIR)/LFU testrun
	@echo 
	@echo Perfect:
	@$(MAKE) $(MAKEFLAGS) -C $(ROOT_DIR)/perfect_caching testrun

clean: $(DIRS)

$(DIRS): $(OBJ)
	@$(MAKE) $(MAKEFLAGS) -C $@ clean
