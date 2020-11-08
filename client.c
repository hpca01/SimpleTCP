#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h> //easy conversions to binary form

#define PORT_NUM 8080
#define ADDRESS "127.0.0.1"
#define BUFF_SIZE 1024

int main(int argc, char const *argv[])
{

    struct sockaddr_in server_address;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("Couldn't initiate socket");
        return EXIT_FAILURE;
    }
    //initialize server_address
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_NUM);

    if (inet_pton(AF_INET, ADDRESS, &server_address.sin_addr) <= 0)
    {
        printf("\n Address %s is not supported \n", ADDRESS);
        return EXIT_FAILURE;
    }

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Could not connect");
        return EXIT_FAILURE;
    }
    char *init = "Hi, there!";
    if (send(sock, init, strlen(init), 0) < 0)
    {
        perror("Error sending!");
        return EXIT_FAILURE;
    }
    char buffer[BUFF_SIZE] = {0};

    long value = read(sock, buffer, BUFF_SIZE);
    printf("rtn -> %s\n", buffer);

    return 0;
}
