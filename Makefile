CC = gcc
CFLAGS = -g -Wall
TARGET = proj1
OBJ = proj1.c 

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o exec $(OBJ) $(LIBS)
clean:
	rm -f *.o $(TARGET)
	rm -f exec
	rm -f output.dat
