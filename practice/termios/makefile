CC = gcc
CFLAGS = -I.
DEPS = kbhitfunc.h
OBJ = kbhit.o kbhitfunc.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
kbhit: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean
clean:
	rm kbhit *.o
