#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "myFunctions.h"

unsigned long long inputFileSize;
unsigned char *data;
long long bytesRead;


void *foo() {
    const unsigned long long sortBlockSize = 1000000;
    for (unsigned long long i = 0; i < inputFileSize / sortBlockSize; ++i) {
        while (bytesRead < (i + 1) * sortBlockSize);
        qsort((data + i * sortBlockSize), sortBlockSize / TUPLE_SIZE, TUPLE_SIZE, cmp);
    }
//    qsort(data, inputFileSize / TUPLE_SIZE, TUPLE_SIZE, cmp);
    return NULL;
}

int main() {
    time_t start = time(NULL);

    char *inputFileName = "../../../input";
    char *outputFileName = "../../../output";
    inputFileSize = getFileSize(inputFileName);
    data = malloc(inputFileSize);
    FILE *inputFile = fopen(inputFileName, "rb");

    bytesRead = 0;
    pthread_t thread1;
    pthread_create(&thread1, NULL, foo, NULL);

    for (; bytesRead < inputFileSize - PAGE_SIZE; bytesRead += PAGE_SIZE)
        fread_unlocked((data + bytesRead), PAGE_SIZE, 1, inputFile);
    fread_unlocked((data + inputFileSize - inputFileSize % PAGE_SIZE),
         inputFileSize % PAGE_SIZE, 1, inputFile);
    bytesRead += inputFileSize % PAGE_SIZE;

    time_t stop = time(NULL);
    printf("Read from file: %ld\n", stop - start);

    start = time(0);
    pthread_join(thread1, NULL);
    stop = time(0);
    printf("\nSort ended: %ld\n", stop - start);

    return 0;

    start = time(0);
    qsort(data, inputFileSize / TUPLE_SIZE, TUPLE_SIZE, cmp);
    stop = time(0);
    printf("\nSort the elements: %ld\n", stop - start);

    start = time(0);
    FILE *outputFile = fopen(outputFileName, "wb+");

    for (unsigned long long i = 0; i < inputFileSize - PAGE_SIZE; i += PAGE_SIZE)
        fwrite((unsigned char *) (data + i), PAGE_SIZE, 1, outputFile);
    fwrite((data + inputFileSize - inputFileSize % PAGE_SIZE),
         inputFileSize % PAGE_SIZE, 1, outputFile);

    stop = time(0);
    printf("\nWrite to file: %ld\n", stop - start);

    fclose(outputFile);
    free(data);

    return 0;
}