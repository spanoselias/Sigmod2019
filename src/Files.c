
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include "Structures.h"


#ifndef UTILS_STRUCTURES_H
#define UTILS_STRUCTURES_H

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define ROWSIZE 100

inline unsigned int calTotalNoOfRows(const char *fileName) {
    struct stat st;

    /*get the size using stat()*/
    stat(fileName, &st);

    return (unsigned int) (st.st_size) / ROWSIZE;
}

void readNFile(row *rows, long totalRows, char *filename) {
    int fd, bytes;
    unsigned char buf[100];

    if ((fd = open(filename, O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }

    bzero(buf, sizeof(buf));
    long idx = 0;
    while (bytes = read(fd, &buf, 100) > 0) {

        rows[idx].key = (unsigned char *) (malloc(sizeof(unsigned char) * 10));
        rows[idx].data = (unsigned char *) (malloc(sizeof(unsigned char) * 90));

        memcpy(rows[idx].key, buf, 10);
        memcpy(rows[idx].data, buf + 10, 90);

        bzero(buf, sizeof(buf));

        ++idx;
    }

    close(fd);
}

void writeNFile(row *rows, long totalRows, char *filename) {
    int fd, bytes;

    if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
        perror("open");
        exit(1);
    }

    bytes = write(fd, rows, totalRows * 100); /* Data out */
    printf("%d bytes were written\n", bytes);
    close(fd);
}

void readFile(row *rows, long totalRows, char *filename) {
    FILE *file;
    row buf;

    file = fopen(filename, "r");
    if (file == nullptr) {
        fprintf(stderr, "\nError opening file\n");
    }

    long idx = 0;
    while (fread(&buf, 100, 1, file)) {
        rows[idx].key = (unsigned char *) (malloc(sizeof(unsigned char) * 10));
        rows[idx].data = (unsigned char *) (malloc(sizeof(unsigned char) * 90));

        memcpy(&rows[idx].key, &buf, 10);
        memcpy(&rows[idx].data, &buf + 10, 90);
        ++idx;
    }

    fclose(file);
}

void bulkfRead(row *rows, const long totalRows, char *input) {
    const unsigned char *memblock;
    int fd;
    struct stat sb;

    fd = open(input, O_RDONLY);
    fstat(fd, &sb);

    memblock = (unsigned const char *) (mmap(NULL, sb.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0));
    if (memblock == (unsigned const char *) MAP_FAILED) handle_error("mmap");

    int idx = 0;
    for (int i = 0; i < totalRows; i++) {
        rows[idx].key = (unsigned char *) (malloc(sizeof(unsigned char) * 10));
        rows[idx].data = (unsigned char *) (malloc(sizeof(unsigned char) * 90));
        memcpy(&rows[idx], &memblock + i, 100);
    }
}

void writeOutput(const row *rows, const long int totalRows, char *filename) {
    FILE *write_ptr;
    write_ptr = fopen(filename, "w");  // w for write, b for binary
    char record[100];
    for (int i = 0; i < totalRows; i++) {

        bzero(record, sizeof(record));
        sprintf(record, "%s%s", rows[i].key, rows[i].data);
        fwrite(record, 100, 1, write_ptr);
    }
}

void bulkWriteOutput(const row *rows, const long int totalRows, char *ouput) {

    FILE *write_ptr;
    write_ptr = fopen(ouput, "wb");  // w for write, b for binary
    fwrite(rows, 100, totalRows, write_ptr);

    fclose(write_ptr);
}

//void fastBulkWriteOutput(const row *rows, const long int totalRows, char * filename) {
//    int fd, bytes;
//
//    if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
//        perror("open");
//        exit(1);
//    }
//
//    bytes = write(fd, rows, sizeof(row) * totalRows); /* Data out */
//    printf("%d bytes were written\n", bytes);
//    close(fd);
//}

#endif //SORTINGALGORITHM_STRUCTURES_H