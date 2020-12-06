#include "server.h"

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

//help w/ troubleshooting and simplifications.
void check(int value, char *err_str)
{
    if (value < 0)
    {
        perror(err_str);
        exit(EXIT_FAILURE);
    }
}

char *humanize(Route *route, int *pid)
{
    char *req_type;
    switch (route->type)
    {
    case GET:
        req_type = "GET";
        break;
    case POST:
        req_type = "POST";
        break;
    case PUT:
        req_type = "PUT";
        break;
    default:
        req_type = "OTHER";
        break;
    }

    int len_str = snprintf(NULL, 0, "<html> \
                                    <head>  \
                                        <title> Hello Process %d ! </title> \
                                    </head> \
                                    <body> \
                                        <h1>%s Request</h1> \
                                        <p> Host: %s <p> User-Agent: %s\
                                    </body> \
                                    </html>",
                           *pid, req_type, route->host, route->user_string);

    char *buf = malloc(len_str + 1); //TODO Free
    snprintf(buf, len_str + 1, "<html> \
                                    <head>  \
                                        <title> Hello Process %d ! </title> \
                                    </head> \
                                    <body> \
                                        <h1>%s Request</h1> \
                                        <p> Host: %s <p> User-Agent: %s\
                                    </body> \
                                    </html>",
             *pid, req_type, route->host, route->user_string);

    return buf;
}

void handle_new_conn(int accepted_socket, void *additional_args)
{
    char *from = (char *)additional_args;
    printf(">> Incoming message from %s : \n", from);

    pid_t child_pid = getpid(); // underlying size is an int, so #10 chars max.

    char message[BUFF_SIZE] = {0};

    long value = read(accepted_socket, message, BUFF_SIZE); //don't use pread socket is not "seekable"

    if (value < 0)
    {
        close(accepted_socket);
        perror("Error reading incoming msg");
        exit(EXIT_FAILURE);
    }

    Route *route = parse_http(message);

    // char *output = humanize(&route, &child_pid);

    char *hello_msg = "Hello Stranger!";
    char *outgoing = (char *)malloc((20 * sizeof(char)) + sizeof(hello_msg) + 1); //Todo: FREE

    sprintf(outgoing, "%s from PID: %d \n\0", hello_msg, child_pid);

    int write_result = write(accepted_socket, outgoing, strlen(outgoing));
    if (write_result < 0)
    {
        close(accepted_socket);
        perror("Error writing outgoing msg");
        exit(EXIT_FAILURE);
    }
    close(accepted_socket);
    // free(output);
    // free(route);
    free(outgoing);
}

Route *parse_route(char *token, Route *req)
{
    char *request;
    int i = 0;
    printf("Token to parse route from %s\n", token);

    while ((request = strsep(&token, " ")) != NULL)
    {
        if (i == 0)
        {
            // req type
            if (strcmp(request, "GET"))
                req->type = GET;
            else if (strcmp(request, "POST"))
                req->type = POST;
            else if (strcmp(request, "PUT"))
                req->type = PUT;
        }
        else if (i == 1)
        {
            req->route = request;
        }
        i++;
    }
    return req;
}

char *translate_reqtype(ReqType type)
{
    switch (type)
    {
    case GET:
        return "GET";
    case POST:
        return "POST";
    case PUT:
        return "PUT";
    }
    return "OTHER";
}

void pretty_print_route(Route *route)
{
    char *type = translate_reqtype(route->type);
    printf("Route {\n\t %s\n\t%s\n\t%s\n\t}\n", type, route->route, route->host);
    return;
}

Route *parse_http(char *input)
{
    char *cp = strdup(input);
    char *saveptr = NULL;

    char *token;

    //first line is request type, resource uri and http protocol

    while (token = strtok_r(cp, "\n", &cp))
    {
        printf("- %s\n", token);
    }

    
    return NULL;
}

int main(int argc, char const *argv[])
{

    struct addrinfo hints, *res;
    struct sockaddr_storage incoming_addr; // this is to make it ipv4 or ipv6 agnostic
    int server_file_d;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; //only ipv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    check(getaddrinfo(NULL, "8080", &hints, &res), "Error with instantiating addrinfo struct");

    // Create Socket, bind to it
    // If Protocol is 0 it's automatically selected
    check(server_file_d = socket(res->ai_family, res->ai_socktype, res->ai_protocol), "Cannot create the socket"); //change to PF_INET for the sake of "correctedness"

    //setsockopt to be able to reuse to an unclosed socket...usually happens if you close and re-start the server
    int yes = 1;

    check(setsockopt(server_file_d, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)), "Could not reuse the unclosed socket.");

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

    check(bind(server_file_d, res->ai_addr, res->ai_addrlen), "Failed to bind socket to address/port");

    //Now listen to the bound port
    check(listen(server_file_d, 1), "Failed to listen");
    //listen doesn't mean accept

    socklen_t addrlen = sizeof(incoming_addr);

    while (1)
    {
        printf("\n~~~~~~~~~Waiting to accept a new conn~~~~~~~~\n\n\n");
        int accepted_socket;

        check(accepted_socket = accept(server_file_d, (struct sockaddr *)&incoming_addr, &addrlen), "Failed to accept connection");

        char from[INET6_ADDRSTRLEN] = {0};

        inet_ntop(incoming_addr.ss_family, get_in_addr((struct sockaddr *)&incoming_addr), from, sizeof(from));

        if (!fork())
        {
            close(server_file_d);
            handle_new_conn(accepted_socket, (void *)from);
            exit(EXIT_SUCCESS);
        }
        close(accepted_socket);
    }

    return 0;
}
