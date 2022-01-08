mainsrc = main.c
mainobj = $(mainsrc:.c=.o)
sources = food.c score.c snake.c locals.c
objects = $(sources:.c=.o)
sore    = eating-snake
sorelib = lib$(sore).a
linklib = -L. -l$(sore) -lncurses -lm
exefile = ./snake
compile = gcc -Wall -Wextra -Wshadow -pedantic \
    -ansi -O3 -fPIC -std=c99 -g

RM = rm -f


all: main
main: $(sorelib) $(mainobj)
	$(compile) -o $@ $(mainobj) $(linklib)
	@-cp $@ $(exefile)

$(sorelib): $(objects)
	@-$(RM) $@
	ar rs $@ $^

# objs =====================
main.o: locals.h main.h main.c
	$(compile) -c main.c

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
	-$(RM) $(mainobj)
	-$(RM) $(objects)
	-$(RM) $(sorelib)
	-$(RM) main $(exefile)
