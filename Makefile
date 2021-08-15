CC =gcc
CFLAGS = -Wall -Wextra -g -pthread
RM = rm

all: server.c client.c
	$(CC) $(CFLAGS) -o server server.c queue.c
	$(CC) $(CFLAGS) -o client client.c
	make ./client

clean:server client
	$(RM) server
	$(RM) client
