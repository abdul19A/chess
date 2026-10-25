RAYLIB_DIR = $(HOME)/libs/raylib/src


LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
DEBUG = -g
CFLAGS = -O2
CC = g++
LIBS =-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
SRCS = main.cpp
HDRS = game.h states.h textures.h attributes.h interactions.h

debug: build/debug
release: build/release

build/debug: main.cpp $(HDRS)
	@echo 'compiling debug'
	$(CC) $(CFLAGS) $(DEBUG) main.cpp -o build/debug $(LIBS)

build/release: main.cpp $(HDRS)
	@echo 'compiling release'
	$(CC) $(CFLAGS) main.cpp -o build/release $(LIBS)

run_debug: build/debug
	./build/debug

run_release: build/release
	./build/release

clean: build
	rm build/*