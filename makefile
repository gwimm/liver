
CC = clang
CFLAGS = -std=c11 -pipe -I inc -include src/inc/lib.h
CC_CMD = $(CC) $(CFLAGS) -c $< -o $@

p = .
SRC_P = $(p)/src
OBJ_P = obj

SRC = $(wildcard $(SRC_P)/*.c)
OBJ = $(addprefix $(OBJ_P)/, $(SRC:$(p)/%=%.o))

all: $(OBJ)

$(OBJ_P)/%.o: $(p)/%.c
	$(CC_CMD)
