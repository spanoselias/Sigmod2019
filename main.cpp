#include <ctime>
#include "string.h"
#include "stdio.h"
#include "timer.c"
#include "structures.h"
#include <sys/stat.h>
#include <cstdlib>

// https://stackoverflow.com/questions/17598572/read-write-to-binary-files-in-c

#define DEBUG 1

// Global Variables
long int TOTALROWS;

long int calTotalNoOfRows(const char *fileName) {
    struct stat st;

    /*get the size using stat()*/
    stat(fileName, &st);

    return (st.st_size) / 100;
}


void printExecutionTime(clock_t &t);

clock_t startTimer();

int rowCmp(const void *row1,
           const void *row2) {

    row *newRow1 = (struct row *) row1;
    row *newRow2 = (struct row *) row2;

    int result = memcmp(newRow1->key, newRow2->key, 10);

    return result;
}

void printExecutionTime(clock_t &t) {
    t = clock() - t;
    double time_taken = (((double) t) / CLOCKS_PER_SEC) * 1000; // in seconds

    printf("Sorting took: [%f] seconds to execute \n", time_taken);
}

void readFile(row rows[]) {
    FILE *file;
    row buf;

    file = fopen("input", "rb");
    if (file == nullptr) {
        fprintf(stderr, "\nError opening file\n");
    }

    int idx = 0;
    while (fread(&buf, sizeof(struct row), 1, file)) {
        memcpy(&rows[idx], &buf, sizeof(struct row));
        ++idx;
    }

    fclose(file);
}

void writeOutput(const row *rows, const long int totalRows) {
    FILE *write_ptr;
    write_ptr = fopen("output", "wb");  // w for write, b for binary
    for (int i = 0; i < totalRows; i++) {
        fwrite(&rows[i], sizeof(struct row), 1, write_ptr);
    }
}

int main() {

    clock_t t = startTimer();

    // Retrieve total number of rows.
    TOTALROWS = calTotalNoOfRows("input");

//   row *rows = static_cast<row *>(malloc(TOTALROWS * sizeof(row)));
     row rows [TOTALROWS] ;
     struct row * newRow;
     newRow = (row *) malloc(sizeof(struct row) * TOTALROWS);

    readFile(newRow);

    size_t structs_len = sizeof(rows) / sizeof(struct row);

    // Sorting Algorithm. Need to be implemented a parallel sorting algorithm
    qsort(rows, structs_len, sizeof(struct row), rowCmp);

//    clock_t t2 = startTimer();
     writeOutput(rows, TOTALROWS);
//    printExecutionTime(t2);

    printExecutionTime(t);

    return 0;
}




