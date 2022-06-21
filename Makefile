#STRUKTURA PROJEKTU
SRC_DIR := ./src
HEAD_DIR := ./headers
BUILD_DIR := ./build
TESTS_DIR := ./tests

#KTÓRY 
MODE := app #Albo app albo test

#PROJECT FILES
SRC := $(wildcard $(SRC_DIR)/*.c)

ifeq ($(MODE), test)
	SRC := $(SRC) $(wildcard $(TESTS_DIR)/*.c)
	EXEC := $(TESTS_DIR)/tests
else
	SRC := $(SRC) $(wildcard $(BUILD_DIR)/*.c)
	EXEC := tracker
endif

#COMPILATION
OBJ := $(SRC:%.c=%.o)
DEPS := $(OBJ:%.o=%.d) 

CC ?= gcc
C_FLAGS := -Wall -Wextra -Werror
DEP_FLAGS := -MMD -MP

LIB := pthread
INC := $(foreach h, $(HEAD_DIR), -I$h)
L_INC := $(foreach l, $(LIB), -l$l)


ifeq ($(CC), clang)
	C_FLAGS += -Weverything -Wno-vla -Wno-float-equal -Wno-disabled-macro-expansion
endif

GFLAG := no

ifeq ($(GFLAG), yes)
	GGDB := -ggdb
else
	GGDB :=
endif
C_FLAGS += $(GGDB)

all: $(EXEC)

$(EXEC): $(OBJ)
	@echo "[CREATING EXEC]"
	$(CC) $(C_FLAGS) $(INC) $(OBJ) -o $@ $(L_INC)

%.o:%.c %.d
	@echo "[COMPILATION]"
	$(CC) $(C_FLAGS) $(INC) -c $< -o $@

#CLEAN
clean:
	@echo "[CLEANING EXEC]"
	rm -rf $(EXEC)
	@echo "[CLEANING OBJECTS]"
	rm -rf $(OBJ)
	@echo "[CLEANING DEPENDECIES]"
	rm -rf $(DEPS)

#DEPS 
$(DEPS):

include $(wildcard $(DEPS))

