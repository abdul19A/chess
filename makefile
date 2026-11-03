RAYLIB_DIR = $(HOME)/libs/raylib/src


LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
DEBUG = -g
CFLAGS = -O2
CC = g++
LIBS =-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
SRCS = main.cpp
HDRS = game.h states.h textures.h attributes.h interactions.h position.h

debug: build/debug
release: build/release
debug_no_o: build/debug_no_optimization

build/debug: main.cpp $(HDRS)
	@echo 'compiling debug'
	$(CC) $(CFLAGS) $(DEBUG) main.cpp -o build/debug $(LIBS)

build/debug_no_optimization: main.cpp $(HDRS)
	@echo 'compiling debug (no optimizations)'
	$(CC) $(DEBUG) main.cpp -o build/debug $(LIBS)

build/release: main.cpp $(HDRS)
	@echo 'compiling release'
	$(CC) $(CFLAGS) main.cpp -o build/release $(LIBS)

build/test: testing.cpp $(HDRS)
	@echo 'compiling testing'
	$(CC) $(CFLAGS) testing.cpp -o build/test $(LIBS)

run_debug: build/debug main.cpp $(HDRS)
	./build/debug

run_release: build/release main.cpp $(HDRS)
	./build/release

run_test: build/test testing.cpp $(HDRS)
	./build/test

clean: build
	rm build/*