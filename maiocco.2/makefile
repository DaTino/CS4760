CC = gcc
CFLAGS = -g -Wall -std=c11 -D_BSD_SOURCE
TARGET = logParse

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
