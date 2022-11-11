#==============================================================================
SHELL   = zsh
#------------------------------------------------------------------------------
NO_COLOR=\x1b[0m
OK_COLOR=\x1b[32m
ERROR_COLOR=\x1b[31m
WARN_COLOR=\x1b[33m
OK_STRING=$(OK_COLOR)✓$(NO_COLOR)
ERROR_STRING=$(ERROR_COLOR)⨯$(NO_COLOR)
WARN_STRING=$(WARN_COLOR)problems$(NO_COLOR)
#------------------------------------------------------------------------------
CC      = gcc
LD      = gcc
CFLAGS  = -O2 -Wall -std=gnu99 -Wextra
CFLAGS += -Wno-unused-parameter -Wno-unused-function -Wno-unused-result
LIBS    = `pkg-config --cflags --libs glib-2.0`
INCLDS  = -I $(INC_DIR)
#------------------------------------------------------------------------------
BIN_DIR = saida
BLD_DIR = obj
DOC_DIR = docs
INC_DIR = libs
OUT_DIR = Resultados
SRC_DIR = src
UTI_DIR = libs
#------------------------------------------------------------------------------
TRASH   = $(BLD_DIR)
#------------------------------------------------------------------------------
SRC     = $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRC))
DEPS    = $(patsubst $(BLD_DIR)/%.o,$(BLD_DIR)/%.d,$(OBJS))
#------------------------------------------------------------------------------
PROGRAM = programa-principal
#==============================================================================

vpath %.c $(SRC_DIR)

.DEFAULT_GOAL = build

define show
@./$(UTI_DIR)/scripts/fmt.sh --color $(1) --type $(2) $(3)
endef

$(BLD_DIR)/%.d: %.c
	-$(call show,cyan,reset,"Generating $(shell basename $@) ... ")
	@$(CC) -M $(INCLDS) $(CFLAGS) $(LIBS) $< -o $@
	@echo -e "$(OK_STRING)"

$(BLD_DIR)/%.o: %.c
	-$(call show,yellow,reset,"Building $(shell basename $@) ... ")
	@$(CC) -c $(INCLDS) $(LIBS) $(CFLAGS) $< -o $@
	@echo -e "$(OK_STRING)"

$(PROGRAM): $(DEPS) $(OBJS)
	-$(call show,green,bold,"Compiling $(shell basename $@) ... ")
	@$(CC) $(INCLDS) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
	@echo -e "$(OK_STRING)"

##=================================================================================

.PHONY: clean # Delete build artifacts
clean:
	for i in {0..15};do;clear;if (( i % 2 == 0 ));then cat libs/.art/maid.ascii;else cat libs/.art/maid2.ascii;fi;sleep 0.09;done
	@clear
	@cat libs/.art/maid3.ascii
	@echo -n "Cleaning "
	@-rm -rf $(TRASH)/*.o
	@-rm -rf $(TRASH)/*.d
	@echo -e "$(OK_STRING)"

##=================================================================================

.PHONY: build # Compile the binary program
build compile program: setup $(PROGRAM)

##=================================================================================

setup:

	@mkdir -p $(BLD_DIR)
	@mkdir -p $(OUT_DIR)

##=================================================================================

.PHONY: help # Generate list of targets with descriptions
help:
	@grep '^.PHONY: .* #' Makefile | sed 's/\.PHONY: \(.*\) # \(.*\)/    \1 \t \2/'

##=================================================================================