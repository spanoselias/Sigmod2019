//
// Created by elias on 3/18/19.
//


#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <memory.h>
#include <stdlib.h>
#include "../DataStructures/structures.h"


#ifndef UTILS_STRUCTURES_H
#define UTILS_STRUCTURES_H

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define ROWSIZE 100

long int calTotalNoOfRows(const char *fileName) {
    struct stat st;

    /*get the size using stat()*/
    stat(fileName, &st);

    return (st.st_size) / ROWSIZE;
}


void bulkfRead(row *rows, const unsigned int totalRows) {
    const unsigned char *memblock;
    int fd;
    struct stat sb;

    fd = open("input", O_RDONLY);
    fstat(fd, &sb);
    printf("Size: %lu\n", (uint64_t) sb.st_size);

    memblock = (unsigned const char *) (mmap(NULL, sb.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0));
    if (memblock == (unsigned const char *) MAP_FAILED) handle_error("mmap");

    int idx = 0;
    for (int i = 0; i < totalRows; i += ROWSIZE) {
        memcpy(&rows[idx], &memblock + i, sizeof(struct row));
        idx += 1;
    }

}

void writeOutput(const row *rows, const long int totalRows) {
    FILE *write_ptr;
    write_ptr = fopen("output", "wb");  // w for write, b for binary
    for (int i = 0; i < totalRows; i++) {
        fwrite(&rows[i], sizeof(struct row), 1, write_ptr);
    }
}

#endif //SORTINGALGORITHM_STRUCTURES_H
