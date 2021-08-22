
//eventually move all file related code into here
// #include "server.h"
#include "files.h"
#include <dirent.h>
#include <unistd.h>

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

MappedFiles* read_files_into_shm(){
    char favico[] = "./files/favicon.ico";
    char index[] = "./files/index.html";
    char pdf1[] = "./files/pdf1.pdf";
    char simple[] = "./files/simple.pdf";

    char* favico_path = realpath(favico, NULL);
    char* index_path = realpath(index, NULL);
    char* pdf1_path = realpath(pdf1, NULL);
    char* simple_path = realpath(simple, NULL);

    printf("File paths:\n");
    printf("\t %s \n", favico_path);
    printf("\t %s \n", index_path);
    printf("\t %s \n", pdf1_path);
    printf("\t %s \n", simple_path);

    int favico_fd = open(favico_path, O_RDONLY, S_IRUSR | S_IRUSR);
    int index_fd = open(index_path, O_RDONLY, S_IRUSR | S_IRUSR);
    int pdf1_fd = open(pdf1_path, O_RDONLY, S_IRUSR | S_IRUSR);
    int simple_fd = open(simple_path, O_RDONLY, S_IRUSR | S_IRUSR);

    struct stat favico_info;
    struct stat index_info;
    struct stat pdf1_info;
    struct stat simple_info;

    if((fstat(favico_fd, &favico_info) | fstat(index_fd, &index_info) | fstat(pdf1_fd, &pdf1_info) | fstat(simple_fd, &simple_info)) == -1){
        perror("Error getting file size info\n");
    }

    unsigned char* favico_in_mem = mmap(NULL, favico_info.st_size, PROT_READ, MAP_PRIVATE, favico_fd, 0);
    unsigned char* index_in_mem = mmap(NULL, index_info.st_size, PROT_READ, MAP_PRIVATE, index_fd, 0); 
    unsigned char* pdf1_in_mem = mmap(NULL, pdf1_info.st_size, PROT_READ, MAP_PRIVATE, pdf1_fd, 0);
    unsigned char* simple_in_mem = mmap(NULL, simple_info.st_size, PROT_READ, MAP_PRIVATE, simple_fd, 0);

    printf("\tMapped file name: %s size: %ld address: %x\n", favico, favico_info.st_size, favico_in_mem);
    printf("\tMapped file name: %s size: %ld address: %x\n", index, index_info.st_size, index_in_mem);
    printf("\tMapped file name: %s size: %ld address: %x\n", pdf1, pdf1_info.st_size, pdf1_in_mem);
    printf("\tMapped file name: %s size: %ld address: %x\n", simple, simple_info.st_size, simple_in_mem);


    MappedFiles* files = calloc(4, sizeof(MappedFiles));

    files[0] = (MappedFiles){favico_path, favico_in_mem, &favico_info, 0};
    files[1] = (MappedFiles){index_path, index_in_mem, &index_info, 0};
    files[2] = (MappedFiles){pdf1_path, pdf1_in_mem, &pdf1_info, 0};
    files[3] = (MappedFiles){simple_path, simple_in_mem, &simple_info, 0};

    close(favico_fd);
    close(index_fd);
    close(pdf1_fd);
    close(simple_fd);

    return files;
}

int len(MappedFiles* files){
    //caller should be aware this can cause segfault if array is not populated w/ elements..
    int size = sizeof(files)/sizeof(files[0]);
    return size;
}



void unmap_file(MappedFiles file){
    printf("Umapping file %s size: %ld address: %x\n", file.file_name, file.info->st_size, file.mapped_file);
    if (munmap(file.mapped_file, file.info->st_size) !=0){
        perror("Error unmapping file");
    }
}

int main(){

    MappedFiles* files = read_files_into_shm();

    int length = 4;

    for(int i =0; i<length; i++){
        MappedFiles file = files[i];
        printf("Mapped file name: %s\n", file.file_name);
        printf("Mapped file size: %ld\n", file.info->st_size);
        printf("Mapped file address: %x\n", file.mapped_file);
        unmap_file(file);
    }

    free(files);
    return 0;
}