# -*- MakeFile -*-

CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -pedantic
LDFLAGS = -lrt -pthread
TARGET = proj2
RM = rm -f

.PHONY : all

all : $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(TARGET).o: $(TARGET).c $(TARGET).h
	$(CC) $(CFLAGS) -c $<

pack:
	zip $(TARGET).zip $(TARGET).c $(TARGET).h Makefile

clean:
	$(RM) *.o $(TARGET) *.out
