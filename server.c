#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT_NUM 8080
#define BUFF_SIZE 20000

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char const *argv[])
{

    struct addrinfo hints, *res;
    struct sockaddr_storage incoming_addr; // this is to make it ipv4 or ipv6 agnostic...

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; //only ipv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, "8080", &hints, &res) < 0)
    {
        perror("Error with instantiating addrinfo struct");
    }

    // Create Socket, bind to it
    // If Protocol is 0 it's automatically selected
    int server_file_d = socket(res->ai_family, res->ai_socktype, res->ai_protocol); //change to PF_INET for the sake of "correctedness"
    if (server_file_d < 0)
    {
        //error has occured
        perror("Cannot create the socket");
        return 0;
    }

    //setsockopt to be able to reuse to an unclosed socket...usually happens if you close and re-start the server
    int yes = 1;

    if (setsockopt(server_file_d, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
    {
        perror("Could not reuse the unclosed socket.");
        return 0;
    }

    /*
        binding to socket requires a socketaddr struct,
        struct is a generic, the actual definition is determined byt the type of network
        IP networking:
        https://pubs.opengroup.org/onlinepubs/009695399/basedefs/netinet/in.h.html

        struct sockaddr_in
        {
            __uint8_t sin_len; //note this is specific to linux...not portable
            sa_family_t sin_family;
            in_port_t sin_port;
            struct in_addr sin_addr;
            char sin_zero[8]; //technically removed...see link
        };

        NOTE: you do not have to pack it by hand,  use the helper func "getaddrinfo" and use the result ptr.
    */

    int bind_result = bind(server_file_d, res->ai_addr, res->ai_addrlen);

    if (bind_result < 0)
    {
        //failed to bind correctly
        perror("Failed to bind socket to address/port");
        return 0;
    }

    //Now listen to the bound port
    if (listen(server_file_d, 1) < 0)
    {
        perror("failed to listen");
        exit(EXIT_FAILURE);
    }
    //listen doesn't mean accept

    socklen_t addrlen = sizeof(incoming_addr);

    while (1)
    {
        printf("\n~~~~~~~~~Waiting to accept a new conn~~~~~~~~\n\n\n");
        int accepted_socket = accept(server_file_d, (struct sockaddr *)&incoming_addr, &addrlen);
        if (accepted_socket < 0)
        {
            perror("Failed to accept connection");
            exit(EXIT_FAILURE);
        }
        char message[BUFF_SIZE] = {0};
        char from[INET6_ADDRSTRLEN] = {0};

        long value = read(accepted_socket, message, BUFF_SIZE); //don't use pread socket is not "seekable"

        inet_ntop(incoming_addr.ss_family, get_in_addr((struct sockaddr *)&incoming_addr), from, sizeof(from));

        if (value < 0)
        {
            close(accepted_socket);
            perror("Error reading incoming msg");
            exit(EXIT_FAILURE);
        }
        printf("Incoming message from %s : \n %s \n", from, message);

        char *hello_msg = "Hello Stranger!";
        int write_result = write(accepted_socket, hello_msg, strlen(hello_msg));
        if (write_result < 0)
        {
            close(accepted_socket);
            perror("Error writing outgoing msg");
            exit(EXIT_FAILURE);
        }
        printf("~~~~~~~~~~~Sent outgoing message!~~~~~~~~~~~~~\n\n\n");
        close(accepted_socket);
    }

    return 0;
}
