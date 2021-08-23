#ifndef FILES_H
#define FILES_H

#include <sys/stat.h>

typedef struct {
    char *file_name;
    unsigned char *mapped_file;
    unsigned long size;
    unsigned int used;
    struct stat info;
} MappedFiles;

MappedFiles* read_files_into_shm();
// MappedFiles* get_file(Route route);
MappedFiles get_file(char* path);
void unmap_file(MappedFiles files);
int len(MappedFiles* files);
#endif