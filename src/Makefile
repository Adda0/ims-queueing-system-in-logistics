CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -MMD
BUILD_DIR = ../build/
EXE = ../sim
SRC = $(wildcard *.cpp)
OBJ = ${SRC:.cpp=.o}
DEPS = ${OBJ:.o=.d}
DIR_OBJ = $(addprefix $(BUILD_DIR), $(OBJ))
DIR_DEPS = $(addprefix $(BUILD_DIR), $(DEPS))
LIBS = /usr/local/lib/libsimlib.so -lm

.PHONY: all clean

all: $(EXE)

-include $(DIR_DEPS)

$(EXE): $(DIR_OBJ)
	$(CC) $(DIR_OBJ) $(CFLAGS) $(LIBS) -o $(EXE)

$(BUILD_DIR)%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $< $(CFLAGS) -c -o $@