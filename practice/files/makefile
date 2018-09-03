CC = gcc
CFLAGS = -I.
DEPS = printDirFunc.h
OBJ = printDir.o printDirFunc.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
printDir: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean
clean:
	rm printDir *.o
