#===============================================================================================================
# File:        Makefile
# Project:     VUT, FIT, IMS, Queueing system in logistics
# Date:        11. 12. 2021
# Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
#===============================================================================================================

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -g -MMD
BUILD_DIR = ../build/
EXE = ../model
SRC = $(wildcard *.cpp)
OBJ = ${SRC:.cpp=.o}
DEPS = ${OBJ:.o=.d}
DIR_OBJ = $(addprefix $(BUILD_DIR), $(OBJ))
DIR_DEPS = $(addprefix $(BUILD_DIR), $(DEPS))
LIBS = -lsimlib -lm

.PHONY: all clean

all: $(EXE)

-include $(DIR_DEPS)

$(EXE): $(DIR_OBJ)
	$(CC) $(DIR_OBJ) $(CFLAGS) $(LIBS) -o $(EXE)

$(BUILD_DIR)%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $< $(CFLAGS) -c -o $@
