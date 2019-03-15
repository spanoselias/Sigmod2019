#include <cstdlib>
#include <ctime>
#include "string.h"
#include "stdio.h"


// https://stackoverflow.com/questions/17598572/read-write-to-binary-files-in-c

#define TOTALROWS 10000
#define DEBUG 1

struct row {
    unsigned char key[10];
    unsigned char data[90];
};

int rowCmp(const void *row1,
           const void *row2) {

    row *newRow1 = (struct row *) row1;
    row *newRow2 = (struct row *) row2;

    int result = memcmp(newRow1->key, newRow2->key, 10);

    return result;
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

void writeOuput(const row *rows) {
    FILE *write_ptr;
    write_ptr = fopen("output", "wb");  // w for write, b for binary
    for (int i = 0; i < TOTALROWS; i++) {
        fwrite(&rows[i], sizeof(struct row), 1, write_ptr);
    }
}

int main() {

    clock_t t;
    t = clock();

    row rows[TOTALROWS];
    readFile(rows);

    size_t structs_len = sizeof(rows) / sizeof(struct row);

    // Sorting Algorithm. Need to be implemented a parallel sorting algorithm
    qsort(rows, structs_len, sizeof(struct row), rowCmp);

    writeOuput(rows);

    t = clock() - t;
    double time_taken = (((double) t) / CLOCKS_PER_SEC) * 1000; // in seconds

    printf("Sorting took %f seconds to execute \n", time_taken);
    return 0;
}
