CC=gcc
CFLAGS=-I. -Wall -Wextra -g

SOURCES=main.c \
		buffer/inputbuff.c \
		command/command.c \
	
OBJECTS=$(SOURCES:.c=.o)

db: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS) db