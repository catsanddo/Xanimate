CC=gcc

SRC=*.c
OBJ=${SRC:.c=.o}

BIN=Xanimate

all: $(BIN)

run: all
	./$(BIN)

Xanimate: $(OBJ) config.h
	$(CC) -o $@ $(OBJ)

%.o: %.c
	$(CC) -c $<

config.h: 
	cp config.def.h $@

clean:
	rm -f $(BIN)
	rm -f *.o

.PHONY: all clean run
