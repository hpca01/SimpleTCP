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

void *get_in_addr(struct sockaddr *sa);
void check(int value, char *err_str);
void handle_new_conn(int accepted_socket, void *additional_args);
void parse_http(char *input);
Route *parse_route(char *input);

typedef enum
{
    GET,
    POST,
    PUT
} ReqType;

typedef struct
{
    ReqType type;
    char *route;
} Route;