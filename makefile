CC=gcc
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=
EXEC=zepplin
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

zepplin: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

zepplin.o: zepplin.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
			
