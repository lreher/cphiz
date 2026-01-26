CC = clang

# Compiler flags
CFLAGS = -Iinclude -Wall -Wextra -std=c11 -Wno-deprecated-declarations

# Homebrew prefix (works on Intel + Apple Silicon)
BREW_PREFIX := $(shell brew --prefix)

# GLFW include & link flags
GLFW_CFLAGS = -I$(BREW_PREFIX)/include
GLFW_LIBS = -L$(BREW_PREFIX)/lib \
            -lglfw \
            -framework OpenGL \
            -framework Cocoa \
            -framework IOKit \
            -framework CoreVideo

SRCS = src/main.c src/vector.c src/particle.c src/camera.c
OBJS = build/main.o build/vector.o build/particle.o build/camera.o

program: $(OBJS)
	$(CC) $(OBJS) -o build/program $(GLFW_LIBS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) $(GLFW_CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o build/program