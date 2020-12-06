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
    char *host;
    char *user_string;
} Route;

void *get_in_addr(struct sockaddr *sa);
void check(int value, char *err_str);
void handle_new_conn(int accepted_socket, void *additional_args);
Route *parse_http(char *input);
Route *parse_route(char *input, Route *req);
char *humanize(Route *route, pid_t *pid);
void pretty_print_route(Route *route);
char *translate_reqtype(ReqType type);