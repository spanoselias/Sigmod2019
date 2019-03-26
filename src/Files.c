
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

unsigned int calTotalNoOfRows(const char *fileName) {
    struct stat st;

    /*get the size using stat()*/
    stat(fileName, &st);

    return (unsigned long) (st.st_size) / ROWSIZE;
}

//void writeNFile(row *rows, long totalRows, char *filename) {
//    int fd, bytes;
//
//    if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
//        perror("open");
//        exit(1);
//    }
//
//    bytes = write(fd, rows, totalRows * 100); /* Data out */
//    printf("%d bytes were written\n", bytes);
//    close(fd);
//}

//void readFile(row *rows, long totalRows, char *filename) {
//    FILE *file;
//    row buf;
//
//    file = fopen(filename, "r");
//    if (file == NULL) {
//        fprintf(stderr, "\nError opening file\n");
//    }
//
//    long idx = 0;
//    while (fread(&buf, 100, 1, file)) {
//        rows[idx].key = (unsigned char *) (malloc(sizeof(unsigned char) * 10));
//        rows[idx].data = (unsigned char *) (malloc(sizeof(unsigned char) * 90));
//
//        memcpy(&rows[idx].key, &buf, 10);
//        memcpy(&rows[idx].data, &buf + 10, 90);%
//        ++idx;
//    }
//
//    fclose(file);
//}

void readNFile(row *rows, long totalRows, char *filename) {
    FILE *fd, bytes;
    unsigned char buf[100];
    unsigned char bufKey1[8];
    unsigned char bufKey2[2];

    fd = fopen(filename, "r");
    if (fd == NULL) {
        fprintf(stderr, "\nError while opening the file: [%s]\n", filename);
        exit(0);
    }

    unsigned int idx = 0;
    while (fread(&buf, 100, 1, fd)) {

//      rows[idx].key = (unsigned char *) (malloc(sizeof(unsigned char) * 10));
        rows[idx].data = (unsigned char *) (malloc(sizeof(unsigned char) * 90));

        memcpy(&bufKey1, buf, 8);
        memcpy(&bufKey2, buf, 2);
        rows[idx].key1 = (unsigned long long) *bufKey1;
        rows[idx].key2 = (unsigned short) *bufKey2;

        memcpy(rows[idx].data, buf + 10, 90);

        ++idx;
    }

    fclose(fd);
}

void bulkfRead(row *rows, const long totalRows, char *input_old) {
    const unsigned char *memblock;
    int fd;
    struct stat sb;

    fd = open(input_old, O_RDONLY);
    fstat(fd, &sb);

    unsigned char bufKey1[8];
    unsigned char bufKey2[2];

    memblock = (unsigned const char *) (mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0));
    if (memblock == (unsigned const char *) MAP_FAILED) handle_error("mmap");

      int offset = 0;
    for (int idx = 0; idx < totalRows; idx++) {

        rows[idx].data = (unsigned char *) (malloc(sizeof(unsigned char) * 90));

        bzero(bufKey1, sizeof(bufKey1));
        bzero(bufKey2, sizeof(bufKey2));

        memcpy(&bufKey1, (memblock + offset), 8);
        memcpy(&bufKey2, (memblock + 2 + offset ), 8);
        rows[idx].key1 = (unsigned long long) *bufKey1;
        rows[idx].key2 = (unsigned short) *bufKey2;

        memcpy(rows[idx].data, (memblock + offset + 10), 90);
        offset += 100;
    }
}

void writeOutput(const row *rows, const long int totalRows, char *filename) {
    FILE *write_ptr;
    write_ptr = fopen(filename, "w");  // w for write, b for binary
    for (int i = 0; i < totalRows; i++) {

        fwrite(&rows[i].key1, 8, 1, write_ptr);
        fwrite(&rows[i].key2, 2, 1, write_ptr);
        fwrite(rows[i].data, 90, 1, write_ptr);
    }
}

void bulkWriteOutput(const row *rows, const long int totalRows, char *ouput) {

    FILE *write_ptr;
    write_ptr = fopen(ouput, "wb");  // w for write, b for binary
    fwrite(rows, totalRows * 100, 1, write_ptr);

    fclose(write_ptr);
}

int memcmp2(const void *__s1, const void *__s2) {

    row *row1 = (row *) __s1;
    row *row2 = (row *) __s2;

    if (row1->key1 < row2->key1) {
        return -1;
    } else if (row1->key1 > row2->key1) {
        return 1;
    } else if (row1->key1 == row2->key1) {
        if (row1->key2 < row2->key2)
            return -1;

        return row1->key2 > row2->key2 ? 1 : 0;

    }
}

#endif //SORTINGALGORITHM_STRUCTURES_H
