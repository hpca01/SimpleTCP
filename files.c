#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

//eventuall move all file related code into here

void read_file();

int main()
{
    read_file();
    return 0;
}

// deprecated
void read_file()
{
    //route requested resource = file name inside of files dir
    struct stat sbuf;
    char files[] = "./files/pdf1.pdf";
    printf("File path %s\n", files);

    if (stat(files, &sbuf) < 0)
    {
        perror("Error locating file");
        return;
    }

    printf("File length of : %s\n\n----------------------------\n%d bytes\n", files, (int)sbuf.st_size);
    fflush(stdout);

    unsigned char *buffer = calloc(sbuf.st_size, sizeof(unsigned char)); //TODO free
    FILE *fp = fopen(files, "r");

    if (fp < 0)
        perror("Cannot open file");

    fread(buffer, sizeof(unsigned char), sbuf.st_size, fp);
    fflush(stdout);
    free(buffer);
}
