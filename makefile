CC = gcc
CFLAGS = -Wall -Wextra 
CPPFLAGS = -I./include

all: bin/labyrinth

bin/labyrinth: src/main.c obj/etape1.o obj/etape2.o | bin
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -lm -o $@

obj/etape1.o: src/etape1.c include/etape1.h  | obj
	$(CC) $(CFLAGS) $(CPPFLAGS) -c src/etape1.c -o obj/etape1.o

obj/etape2.o: src/etape2.c include/etape2.h  | obj
	$(CC) $(CFLAGS) $(CPPFLAGS) -c src/etape2.c -o obj/etape2.o

obj:	
	mkdir -p obj

bin:
	mkdir -p bin

clean:
	rm -rf obj bin