
//eventually move all file related code into here
// #include "server.h"
#include "files.h"
#include <fts.h>
#include <unistd.h>

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

// void read_files(){
//     char *file = "./files/index.html";
//     int fd = open(file, O_RDONLY, S_IRUSR | S_IRUSR);
//     struct stat fd_info;

//     if(fstat(fd, &fd_info) == -1){
//         perror("Couldn't get file size info.\n");
//     }
//     printf("File %s size is %ld\n", file, fd_info.st_size);

//     char* file_in_memory = mmap(NULL, fd_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

//     munmap(file_in_memory, fd_info.st_size);
//     close(fd);

// }

// MappedFiles** read_files_into_shm(){
//     char files[] = "./files";
//     char *fp = realpath(files, NULL);
//     char* const* path[] = {fp, NULL};
    

//     FTS* file_dir
// }

int main(){
    char files[] = "./files\0";
    char* fp = realpath(files, NULL);
    char* const path[] = {fp, NULL};

    printf("Printing where the directory is\n");

    printf("Directory: %s \n", fp);

    FTS* file_dir;

    file_dir = fts_open(path, FTS_PHYSICAL, NULL);

    FTSENT* iter_dir;

    while((iter_dir = fts_read(file_dir))!= NULL){
        printf("Children in %s depth: %d:\n", iter_dir->fts_name, iter_dir->fts_level);
        FTSENT* node = fts_children(file_dir, 0);
        while(node != NULL){
            printf("\tFile name is %s accessible at %s/%s\n", node->fts_name, node->fts_path, node->fts_accpath);
            node = node->fts_link;
        }
        free(node);
    }

    fts_close(file_dir);
    free(iter_dir);
    free(fp);

}