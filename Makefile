

CC = gcc
LD = ld

WARNINGS = -pedantic -Wall -Wextra
DEBUG = -g

TESTS = tests.c
OBJS = app.c

INCS = -I/usr/include/cmocka
LIB_PATH = -L/usr/lib/
LIBS = -lcmocka

FLAGS = $(WARNINGS) $(DEBUG)
LDLIBS = $(LIB_PATH) $(LIBS)

all: app

# Compile all .c files into .o files.
# .c.o: 
#	@echo "Compiling" $< "..."
#	$(CC) $(CFLAGS) -c $<

tests: $(TESTS)
	@echo "Building target" $@ "..."
	$(CC) $(FLAGS) $(TESTS) $(LDLIBS) -o $@ 

app: $(OBJS)
	@echo "Building target" $@ "..." 
	$(CC) $(FLAGS) $(OBJS) $(LDLIBS) -o $@

rtests: tests
	./tests

run: app
	./app

# Clean up!
clean:
	rm -f *.o app tests



# $? 	list of dependencies changed more recently than current target.
# $@	name of current target.
# $<	name of current dependency.
# $*	name of current dependency without extension.