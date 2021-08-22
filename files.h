#ifndef FILES_H
#define FILES_H


typedef struct {
    char *file_name;
    unsigned char *mapped_file;
    struct stat *info;
    unsigned int used;
} MappedFiles;

MappedFiles** read_files_into_shm();
// MappedFiles* get_file(Route route);
MappedFiles get_file(char* path);
void unmap_file(MappedFiles* files);
int len(MappedFiles** files);
#endif