
CC = gcc
CFLAGS = -g -Wall -O2
OBJS = build/main.o build/token.o
fela: $(OBJS)
	$(CC) -o fela $(OBJS)
	
build/%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)


clean:
	rm -f build/*

	  
	
