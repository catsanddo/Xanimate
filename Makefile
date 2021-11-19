CC=gcc

SRC=src/xanimate.c src/utils.c
OBJ=${SRC:.c=.o}

DIR=bin
BIN=bin/xanimate

all: $(DIR) $(BIN)

run: all
	./$(BIN)

$(DIR):
	mkdir -p bin

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(BIN)
	rm -f $(OBJ)

.PHONY: all clean run
