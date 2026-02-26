CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude

all: hash_table

hash_table:
	$(CC) $(CFLAGS) src/main.c src/hashtable.c -o hash_table.exe

run: hash_table
	./hash_table.exe

clean:
	rm -f hash_table.exe
