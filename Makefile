
CC = gcc
CFLAGS = -g -Wall -O2

fela: main.o
	$(CC) -o fela main.o
	
%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

clean:
	rm -f main.o main

	  
	
