OUT := violet

CC = gcc

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS := -Wall -Iinclude -g
LDFLAGS :=
LDLIBS := -lm -lcsfml-audio -lcsfml-graphics -lcsfml-window -lcsfml-system

.PHONY: all clean

run: all
	./violet

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

clean:
	rm -rf $(OUT) $(OBJ_DIR)
