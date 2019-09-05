CC = gcc
CFLAGS = -g -Wall -std=c11
TARGET = dt

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
