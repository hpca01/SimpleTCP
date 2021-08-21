#ifndef FILES_H
#define FILES_H

typedef struct {
    char *file_name;
    unsigned char *mapped_file;
    unsigned int used;
} MappedFiles;

// MappedFiles** read_files_into_shm();
// MappedFiles* get_file(Route route);
// void unmap_files(MappedFiles** files);

#endif