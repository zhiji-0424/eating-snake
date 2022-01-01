sources = food.c score.c snake.c locals.c main.c
objects = $(sources:.c=.o)
linklib = -lncurses -lm
exefile = /home/tyri/bin/snake
compile = gcc

all: main
main: $(objects)
	gcc  -o $@ $^ $(linklib)

food.o: locals.h food.h food.c
	$(compile) -c food.c

score.o: locals.h score.h score.c
	$(compile) -c score.c

snake.o: locals.h snake.h snake.c
	$(compile) -c snake.c

locals.o: locals.h locals.c
	$(compile) -c locals.c


.PHONY: clean
clean:
	-rm -f $(objects)
	-rm -f main
