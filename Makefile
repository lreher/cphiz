CC = gcc
CFLAGS = -Iinclude -Wall

SRCS = src/main.c src/vector.c src/particle.c
OBJS = build/main.o build/vector.o build/particle.o

program: $(OBJS)
	$(CC) $(OBJS) -o build/program

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o build/program