CC = clang

# Compiler flags
CFLAGS = -Iinclude -Wall -Wextra -std=c11 -Wno-deprecated-declarations

# Homebrew prefix (works on Intel + Apple Silicon)
BREW_PREFIX := $(shell brew --prefix)

# GLFW include & link flags
RAYLIB_CFLAGS = -I$(BREW_PREFIX)/include
RAYLIB_LIBS = -L$(BREW_PREFIX)/lib \
              -lraylib \
              -framework OpenGL \
              -framework Cocoa \
              -framework IOKit \
              -framework CoreVideo

SRCS = src/main.c src/vector.c src/particle.c src/camera.c src/input.c src/utils.c
OBJS = build/main.o build/vector.o build/particle.o build/camera.o build/input.o build/utils.o

program: $(OBJS)
	$(CC) $(OBJS) -o build/program $(RAYLIB_LIBS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) -Wno-unused-parameter -c $< -o $@

clean:
	rm -f build/*.o build/program