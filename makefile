RAYLIB_DIR = $(HOME)/libs/raylib/src


LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

CFLAGS = -O2 -g
CC = g++
LIBS =-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
SRCS = main.cpp

build: main

main: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main $(LIBS)
test: testing.cpp
	$(CC) $(CFLAGS) testing.cpp -o test $(LIBS)
run: main
	./main
run_test: test
	./test
	rm test
clean: main
	rm main