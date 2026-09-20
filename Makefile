
CC = gcc

CPPFLAGS = -Iinclude
CFLAGS = -Wall -Wextra -Werror -std=c11 -O3 -g

TARGET = inverted-search

SRC = $(wildcard src/*.c)
OBJ = $(notdir $(SRC:.c=.o))
HEADERS = include/inverted-search.h include/ll-ops.h

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJ) -o $@

%.o: src/%.c $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
