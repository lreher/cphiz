CC = clang

# Compiler flags
CFLAGS = -Iinclude -Wall -Wextra -std=c11 -Wno-deprecated-declarations

# Homebrew prefix
BREW_PREFIX := $(shell brew --prefix)

# Raylib flags
RAYLIB_CFLAGS = -I$(BREW_PREFIX)/include
RAYLIB_LIBS = -L$(BREW_PREFIX)/lib \
              -lraylib \
              -framework OpenGL \
              -framework Cocoa \
              -framework IOKit \
              -framework CoreVideo

# Output folders
OBJDIR = build/objects
BINDIR = build/programs

# Library code (no main)
LIB_SRCS = lib/vector.c lib/particle.c lib/camera.c lib/utils.c
LIB_OBJS = $(LIB_SRCS:lib/%.c=$(OBJDIR)/lib_%.o)

# Demos (each has a main)
DEMOS = explosion fireworks

# Default target
all: $(DEMOS)

# Build each demo
$(DEMOS): %: src/%.c $(LIB_OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) $< $(LIB_OBJS) \
		-o $(BINDIR)/$@ $(RAYLIB_LIBS)

# Compile library object files
$(OBJDIR)/lib_%.o: lib/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) -Wno-unused-parameter \
		-c $< -o $@

clean:
	rm -rf build