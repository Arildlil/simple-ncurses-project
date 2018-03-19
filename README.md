# simple-ncurses-project

This is a new side project of mine, where I will try to create a simple game and game engine for the Linux terminal. The main purpose of the project is to learn more, both about the usage of external libraries in C, unit testing, test driven development and how a simple game engine might work. As this is my first attempt at creating an engine, efficiency is not a primary concern.

# Build and run

To build the project, make sure that the 'ncurses' library in installed. In one also wants to run the tests, make sure the 'cmocka' library is installed as well. 

## Running the tests

```c
make test
``` 

## Running the game

```c
make app
make run
``` 

Press CTRL-C to stop