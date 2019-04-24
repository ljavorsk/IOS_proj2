# -*- MakeFile -*-

CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LDFLAGS = -lrt -pthread
TARGET = proj2
RM = rm -f

.PHONY : all

all : $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(TARGET).o: $(TARGET).c $(TARGET).h
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o $(TARGET)
