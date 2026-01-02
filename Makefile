CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g -Iinclude

SRC = src/main.c 		\
	  src/lexer.c		\
	  src/utils.c
BIN = bin
EXEC = acin

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(SRC)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$(EXEC) $(SRC)

clean:
	rm -f $(BIN)/$(EXEC)