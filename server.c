#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
    // Create Socket, bind to it
    // If Protocol is 0 it's automatically selected
    int server_file_d = socket(AF_INET, SOCK_STREAM, 0);
    if (server_file_d < 0)
    {
        //error has occured
        perror("Cannot create the socket");
        return 0;
    }

    /*
        binding to socket requires a socketaddr struct,
        struct is a generic, the actual definition is determined byt the type of network
        IP networking:
        struct sockaddr_in
        {
            __uint8_t sin_len; //note this is specific to linux...not portable
            sa_family_t sin_family;
            in_port_t sin_port;
            struct in_addr sin_addr;
            char sin_zero[8];
        };
    */

    struct sockaddr_in address;
    const int PORT_NUM = 8080; //where client goes

    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET; //0.0.0.0
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT_NUM);

    return 0;
}
