

CC = gcc
LD = ld

WARNINGS = -pedantic -Wall -Wextra
DEBUG = -g

TESTS = src/tests/tests.c
APP = src/app.c
OBJS = src/core/player/*.c src/core/graphics/*.c \
	src/core/gameobject/*.c src/core/world/*.c src/definitions/*.c src/utils.c src/core/*.c

INCS = -I/usr/include/cmocka
LIB_PATH = -L/usr/lib/
LIBS = -lncurses
LIBS_TEST = -lcmocka

FLAGS = $(WARNINGS) $(DEBUG)
LDLIBS = $(LIBPATH) $(LIBS)
LDLIBS_TEST = $(LDLIBS) $(LIBS_TEST)

.PHONY: all clean install debug run lrun test vtest

all: app

# Compile all .c files into .o files.
# .c.o: 
#	@echo "Compiling" $< "..."
#	$(CC) $(CFLAGS) -c $<

test: $(TESTS) $(OBJS) 
	@echo "Building target" $@ "..."
	$(CC) $(FLAGS) $(TESTS) $(OBJS) $(LDLIBS) -o $@ 

debug: $(APP) $(OBJS)
	@echo "Building target" $@ "in DEBUG mode..." 
	$(CC) $(FLAGS) -DDEBUG $(APP) $(OBJS) $(LDLIBS_TEST) -o app

app: $(APP) $(OBJS)
	@echo "Building target" $@ "..." 
	$(CC) $(FLAGS) $(APP) $(OBJS) $(LDLIBS) -o $@

rtest: test
	@echo "Running test suite with all tests..."
	./test

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
