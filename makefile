CC = gcc
CFLAGS = -Wall -Wextra 
CPPFLAGS = -I./include
LDFLAGS = -lncurses

all: bin/labyrinth

bin/labyrinth: obj/main.o obj/etape1.o obj/etape2.o | bin
	$(CC) $(CFLAGS) $(CPPFLAGS)  $^ -o $@ $(LDFLAGS)

obj/main.o: src/main.c include/etape1.h include/etape2.h  | obj
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c src/main.c -o obj/main.o

obj/etape1.o: src/etape1.c include/etape1.h  | obj
	$(CC) $(CFLAGS) $(CPPFLAGS) -c src/etape1.c -o obj/etape1.o

obj/etape2.o: src/etape2.c include/etape2.h  | obj
	$(CC) $(CFLAGS) $(CPPFLAGS) -c src/etape2.c -o obj/etape2.o

bin/exemple: src/ncurses.c | bin
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) 

obj:	
	mkdir -p obj

bin:
	mkdir -p bin

clean:
	rm -rf obj bin

run: bin/labyrinth
	./bin/labyrinth

exemple: bin/exemple
	./bin/exemple