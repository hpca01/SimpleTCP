CC =gcc
CFLAGS = -Wall -Wextra -g
RM = rm

all: server.c client.c
	$(CC) $(CFLAGS) -o server server.c -lpthread
	$(CC) $(CFLAGS) -o client client.c -lpthread
	./server
	make ./client

clean:server client
	$(RM) server
	$(RM) client
