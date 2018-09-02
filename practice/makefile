CC = g++
CFLAGS = -g -I.
DEPS = functions.h
OBJ = main.o functions.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
TFX: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean
clean:
	rm TFX *.o
