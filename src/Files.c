
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

void readNFile(row *rows, long totalRows, char *filename) {
    FILE *fd, bytes;
    unsigned char buf[100];

    fd = fopen(filename, "rb");
    if (fd == NULL) {
        fprintf(stderr, "\nError opening file\n");
    }

    long idx = 0;
    while (fread(&buf, 100, 1, fd)) {

//     rows[idx].key = (unsigned char *) (malloc(sizeof(unsigned char) * 10));
//     rows[idx].data = (unsigned char *) (malloc(sizeof(unsigned char) * 90));

        memcpy(rows[idx].key, buf, 10);
        memcpy(rows[idx].data, buf + 10, 90);

        ++idx;
    }

    fclose(fd);
}

void readNBytesFile(row *rows, long totalRows, char *filename) {
    FILE *fd, bytes;
    int bufferSize = 10000;

    unsigned char buf[bufferSize];

    fd = fopen(filename, "r");
    if (fd == NULL) {
        fprintf(stderr, "\nError opening file\n");
    }

    long idx = 0;
    while (fread(&buf, bufferSize, 1, fd)) {

        for (int i = 0; i < bufferSize; i += 100) {

            if (idx >= totalRows) {
                return;
            }

//            rows[idx].data = (unsigned char *) (malloc(sizeof(unsigned char) * 90));

            memcpy(rows[idx].key, buf + i, 10);
            memcpy(rows[idx].data, buf + i + 10, 90);

            ++idx;
        }

    }

    fclose(fd);
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
    int offset = 0;
    for (int i = 0; i < totalRows; i++) {
//        rows[idx].key = (unsigned char *) (malloc(sizeof(unsigned char) * 10));
//         rows[idx].data = (unsigned char *) (malloc(sizeof(unsigned char) * 90));
//        memcpy(&rows[idx], &memblock + i, 100);

        memcpy(rows[idx].key, memblock + i, 10);
        memcpy(rows[idx].data, memblock + i + 10, 90);
        offset += 100;
    }
}




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
//        memcpy(&rows[idx].data, &buf + 10, 90);
//        ++idx;
//    }
//
//    fclose(file);
//}

void writeOutput(const row *rows, const long int totalRows, const char *filename) {
    FILE *write_ptr;
    write_ptr = fopen(filename, "wb");  // w for write, b for binary
    unsigned char *record = (unsigned char* ) (malloc(100 * totalRows));

    int offset = 0;
    for (int i = 0; i < totalRows; i++) {

//      bzero(record, sizeof(record));
        memcpy(record + offset, rows[i].key, 10);
        memcpy((record + 10 + offset), rows[i].data, 90);

//        free(rows[i].data);

        offset += 100;

//     fwrite(rows[i].key, 10, 1, write_ptr);
//     fwrite(rows[i].data, 90, 1, write_ptr);

    }

    fwrite(record, totalRows * 100, 1, write_ptr);

    fclose(write_ptr);
}

void writeBStructOutput(const packedRow *rows, const long int totalRows, char *filename) {
    FILE *write_ptr;
    write_ptr = fopen(filename, "wb");  // w for write, b for binary
    unsigned char record[10000];

    for (int i = 0; i < totalRows; i++) {

//   memcpy(record, rows[i].key, 10);
//   memcpy((record + 10), rows[i].data, 90);
        fwrite(rows, 100, 1, write_ptr);

    }

    fclose(write_ptr);
}


void bulkWriteOutput(row *rows, const long int totalRows, const char *ouput) {
    FILE *outfile; /**< the destination file object */

    outfile = fopen(ouput, "wb");
    if (outfile == NULL) {
        fprintf(stderr, "\nError opening file\n"); /* Error reading the file, exit. */
        exit(1);
    }
//    int i;
//    for (i = 0; i < totalRows; i++) {
    fwrite(rows, totalRows * 100, 1, outfile);
//    }
    fclose(outfile); /* never forget to close or your data won't be written. */
}


void writeToFileSysCall(row *rows, long totalRows, char *filename) {
    int fd, bytes;

    if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
        perror("open");
        exit(1);
    }

    bytes = write(fd, rows, totalRows * 100); /* Data out */
//    printf("%d bytes were written\n", bytes);
    close(fd);
}

#endif //SORTINGALGORITHM_STRUCTURES_H
