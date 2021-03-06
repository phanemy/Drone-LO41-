CC=gcc
CFLAGS=-W -Wall -pedantic
LDFLAGS=-lpthread
EXEC=zeppelin
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

zeppelin: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

zeppelin.o: zeppelin.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
			
