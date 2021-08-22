CC =gcc
CFLAGS = -Wall -Wextra -g -pthread
CFLAGS_NO_THREADS = -Wall -Wextra -g
RM = rm
GDB = gdb

all: server.c client.c
	$(CC) $(CFLAGS) -o server server.c queue.c
	$(CC) $(CFLAGS) -o client client.c
	make ./client

clean:server client
	$(RM) server
	$(RM) client

debug:server
	$(GDB) server

files: files.c
	$(CC) $(CFLAGS_NO_THREADS) -o files_test files.c
	# ./files_test

clean_files: files_test
	$(RM) files_test