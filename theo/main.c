#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include "myFunctions.h"

unsigned long long inputFileSize;
unsigned char *data;
long long bytesRead;
unsigned long long arrayForSortingSize;
unsigned long long *arrayForSorting;

int cmp(const void *a, const void *b) {
    return memcmp(a, b, 10);
}

int main() {
    time_t start = time(NULL);

    char *inputFileName = "../../../input";
    inputFileSize = getFileSize(inputFileName);
    data = malloc(inputFileSize);

    int inputFile = open(inputFileName, O_RDONLY);

    for (bytesRead = 0; bytesRead < inputFileSize - PAGE_SIZE; bytesRead += PAGE_SIZE)
        read(inputFile, (data + bytesRead), PAGE_SIZE);

    read(inputFile, (data + inputFileSize - inputFileSize % PAGE_SIZE),
         inputFileSize % PAGE_SIZE);

    bytesRead += inputFileSize % PAGE_SIZE;

    time_t stop = time(NULL);
    printf("Read from file: %ld\n", stop - start);

    arrayForSortingSize = inputFileSize / TUPLE_SIZE * 2;
    arrayForSorting = malloc(arrayForSortingSize * sizeof(long long));

    start = time(0);
    for (unsigned long long i = 0; i < arrayForSortingSize; i += 2) {
        arrayForSorting[i] = ptrToLong(&data[i * 50]);
        arrayForSorting[i + 1] = ptrToShortAndOffset(&data[i * 50 + 8], i * 50);
    }
    stop = time(0);
    printf("\nMake the array for sorting: %ld\n", stop - start);

//    for (int j = 100; j < 116; ++j)
//        printf("%x ", data[j]);
//    printf("\n");
//    for (int j = 16; j < 32; ++j)
//        printf("%x ", *((unsigned char *) arrayForSorting + j));
//    printf("%llu %llu\n", arrayForSorting[0], arrayForSorting[2]);

    start = time(0);
    qsort(data, inputFileSize / 100, 100, cmp);
//    qsort(arrayForSorting, inputFileSize / 100, 2 * sizeof(long long), comparator);
    stop = time(0);
    printf("\nSort the elements: %ld\n", stop - start);

    start = time(0);
    FILE *outputFile = fopen("../../../output", "wb+");

//    for (unsigned long long i = 1; i < arrayForSortingSize; i += 2) {
//        fwrite((unsigned char *) (data + getOffsetFromLong(arrayForSorting[i])), 100, 1, outputFile);
//    }
    for (unsigned long long i = 0; i < inputFileSize - PAGE_SIZE; i += PAGE_SIZE) {
        fwrite((unsigned char *) (data + i), PAGE_SIZE, 1, outputFile);
    }
    fwrite((data + inputFileSize - inputFileSize % PAGE_SIZE),
         inputFileSize % PAGE_SIZE, 1, outputFile);


    stop = time(0);
    printf("\nWrite to file: %ld\n", stop - start);

    fclose(outputFile);
    free(data);
    free(arrayForSorting);

    return 0;
}