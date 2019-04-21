#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "myFunctions.h"

#define NUMBER_OF_THREADS 2
#define BUFFER_SIZE PAGE_SIZE * TUPLE_SIZE

unsigned long long inputFileSize;
unsigned long long bytesRead;
unsigned char *data;
unsigned long long arrayForSortingSize;
struct keyAndOffset *arrayForSorting;
pthread_t sortThreads[NUMBER_OF_THREADS];
unsigned long long GROUP_SIZE;
unsigned char buffer[BUFFER_SIZE];
unsigned long long position[NUMBER_OF_THREADS];

int getThreadId(pthread_t self) {
    for (int i = 0; i < NUMBER_OF_THREADS; ++i)
        if (self == sortThreads[i])
            return i;

    return -1;
}

void *sort() {
    int id = getThreadId(pthread_self());

    qsort(data + (id * GROUP_SIZE), GROUP_SIZE / TUPLE_SIZE, sizeof(struct keyAndOffset), comparator);

    printf("Sorted: %d / %d\n", id + 1, NUMBER_OF_THREADS);

    return NULL;
}

void *makeArray() {
    GROUP_SIZE = inputFileSize / NUMBER_OF_THREADS;
    arrayForSortingSize = inputFileSize / TUPLE_SIZE;
    arrayForSorting = malloc(arrayForSortingSize * sizeof(struct keyAndOffset));

    for (unsigned long long i = 0; i < inputFileSize; i += GROUP_SIZE) {
        while (bytesRead < i + GROUP_SIZE);

        for (unsigned long long j = i / 100; j < (i + GROUP_SIZE) / 100; ++j) {
            arrayForSorting[j] = ptrToKeyAndOffset(&data[j * 100], j * 100);
        }
        pthread_create(&sortThreads[i / GROUP_SIZE], NULL, sort, NULL);
    }

    return NULL;
}

void merge() {
    int bufferPosition = 0;

    for (int i = 0; i < BUFFER_SIZE / TUPLE_SIZE; ++i) {

        int maxIndex = -1;
        for (int j = 0; j < NUMBER_OF_THREADS; ++j)
            if (position[j] != arrayForSortingSize) {
                maxIndex = j;
                break;
            }


        if (maxIndex == -1) {
            printf("out of elements\n");
            return;
        }


        for (int j = maxIndex + 1; j < NUMBER_OF_THREADS; j++)
            if (position[j] != arrayForSortingSize &&
                comparator(&arrayForSorting[j * arrayForSortingSize / NUMBER_OF_THREADS + position[j]],
                           &arrayForSorting[maxIndex * arrayForSortingSize / NUMBER_OF_THREADS + position[maxIndex]])
                > 0)
                maxIndex = j;

//        printf("%d %d\n", i, maxIndex);

        buffer[bufferPosition] = data[maxIndex * arrayForSortingSize / NUMBER_OF_THREADS + position[maxIndex]];
        memcpy((void *) buffer[bufferPosition], (unsigned char *) (data + getOffsetFromLong(
                arrayForSorting[maxIndex * arrayForSortingSize / NUMBER_OF_THREADS + position[maxIndex]].long2)), 100);

        bufferPosition += 100;
        ++position[maxIndex];

    }
//    FILE *outputFile = fopen("../../../output", "wb+");
//    fwrite(buffer, 100, 4096, outputFile);

}

int main() {

    time_t start = time(NULL);

    char *inputFileName = "../../../input";
    inputFileSize = getFileSize(inputFileName);
    pthread_t threadForArray;
    pthread_create(&threadForArray, NULL, makeArray, NULL);

    data = malloc(inputFileSize);

    int inputFile = open(inputFileName, O_RDONLY);
    for (bytesRead = 0; bytesRead < inputFileSize - PAGE_SIZE; bytesRead += PAGE_SIZE)
        read(inputFile, (data + bytesRead), PAGE_SIZE);
    read(inputFile, (data + inputFileSize - inputFileSize % PAGE_SIZE),
         inputFileSize % PAGE_SIZE);

    bytesRead += inputFileSize % PAGE_SIZE;

    time_t stop = time(NULL);
    printf("Read from file: %ld\n", stop - start);

    pthread_join(threadForArray, NULL);

    start = time(0);
    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        pthread_join(sortThreads[i], NULL);
    }
//    qsort(arrayForSorting, inputFileSize / 100, sizeof(struct keyAndOffset), comparator);
    stop = time(0);
    printf("\nSort the elements: %ld\n", stop - start);


    start = time(0);

    merge();

    stop = time(0);
    printf("\nMerge N elements: %ld\n", stop - start);



//    start = time(0);
//    FILE *outputFile = fopen("../../../output", "wb+");

//    for (unsigned long long i = 1; i < inputFileSize / TUPLE_SIZE; ++i) {
//        fwrite((unsigned char *) (data + getOffsetFromLong(arrayForSorting[i].long2)), 100, 1, outputFile);
//    }

//    stop = time(0);
//    printf("\nWrite to file: %ld\n", stop - start);

//    fclose(outputFile);
    free(data);
    free(arrayForSorting);

    return 0;
}