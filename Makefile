

CC = gcc
LD = ld

WARNINGS = -pedantic -Wall -Wextra
DEBUG = -g

TESTS = tests.c
APP = app.c
OBJS = curses.c utils.c surface.c rect.c image.c

INCS = -I/usr/include/cmocka
LIB_PATH = -L/usr/lib/
LIBS = -lcmocka -lncurses

FLAGS = $(WARNINGS) $(DEBUG)
LDLIBS = $(LIB_PATH) $(LIBS)

.PHONY: all clean install debug run lrun test vtest

all: app

# Compile all .c files into .o files.
# .c.o: 
#	@echo "Compiling" $< "..."
#	$(CC) $(CFLAGS) -c $<

tests: $(TESTS) $(OBJS) 
	@echo "Building target" $@ "..."
	$(CC) $(FLAGS) $(TESTS) $(OBJS) $(LDLIBS) -o $@ 

debug: $(APP) $(OBJS)
	@echo "Building target" $@ "in DEBUG mode..." 
	$(CC) $(FLAGS) -DDEBUG $(APP) $(OBJS) $(LDLIBS) -o app

app: $(APP) $(OBJS)
	@echo "Building target" $@ "..." 
	$(CC) $(FLAGS) $(APP) $(OBJS) $(LDLIBS) -o $@

test: tests
	@echo "Running test suite with all tests..."
	./tests

vtest: tests
	@echo "(Valgrind) Running test suite with all test..."
	valgrind ./tests -v --leak-check=full

run: app
	./app

lrun: app
	make run > output.log; cat output.log

# Clean up!
clean:
	rm -f *.o app tests



# $? 	list of dependencies changed more recently than current target.
# $@	name of current target.
# $<	name of current dependency.
# $*	name of current dependency without extension.