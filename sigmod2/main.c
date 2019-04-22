#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define THREADS 4
#define TUPLE 100
#define BUFFER 500000000

char *data[THREADS];
unsigned long long fileSize;
pthread_t threads[THREADS];
unsigned long long position[THREADS];
unsigned long long arraySize;
char *buffer[2];
int bufferWritten = 0;
int fileWritten = 0;


unsigned long long getFileSize(char *fileName) {
    struct stat st;

    stat(fileName, &st);

    return st.st_size;
}

int cmp(const void *pVoid, const void *pVoid1) {
    return memcmp(pVoid, pVoid1, 10);
}

void *sortThread(void *id) {
    qsort(data[(int) id], arraySize / TUPLE, TUPLE, cmp);

    return NULL;
}

void merge(char *buf) {
    int bufferPosition = 0;

    for (unsigned long long i = 0; i < BUFFER / TUPLE; ++i) {
        int index = -1;
        for (int j = 0; j < THREADS; ++j) {
            if (position[j] != arraySize) {
                index = j;
                break;
            }
        }

        for (unsigned long long j = index + 1; j < THREADS; ++j) {
            if (position[j] != arraySize
                && cmp(data[j] + position[j], data[index] + position[index]) < 0)
                index = j;
        }

        memcpy(buf + bufferPosition, data[index] + position[index], 100);

        position[index] += 100;
        bufferPosition += 100;
    }
}

void *writeThread(void *pVoid) {

    FILE *output = fopen("output", "w+");

    for (int i = 0; i < fileSize / BUFFER; ++i) {

        while (fileWritten == bufferWritten)
            printf("Writer %d waitting...\n", i);

        time_t start = time(NULL);
        fwrite(buffer[i % 2], BUFFER, 1, output);
        time_t stop = time(NULL);
//        printf("Write: %ld\n", stop - start);
        ++fileWritten;
    }

    return NULL;
}

int main() {
    FILE *input = fopen("input", "r");
    fileSize = getFileSize("input");
    arraySize = fileSize / THREADS;

    time_t start = time(NULL);
    for (int i = 0; i < THREADS; ++i) {
        data[i] = malloc(arraySize);
        fread(data[i], arraySize, 1, input);
        pthread_create(&threads[i], NULL, sortThread, (void *) i);
    }
    time_t stop = time(NULL);
    printf("Read: %ld\n", stop - start);

    start = time(NULL);
    for (int i = 0; i < THREADS; ++i)
        pthread_join(threads[i], NULL);

    stop = time(NULL);
    printf("Sort: %ld\n", stop - start);

    buffer[0] = malloc(BUFFER);
    buffer[1] = malloc(BUFFER);

    pthread_t writer;
    pthread_create(&writer, NULL, writeThread, NULL);

    merge(buffer[0]);
    ++bufferWritten;

    for (int i = 1; i < fileSize / BUFFER; ++i) {

        while (fileWritten + 1 != bufferWritten);

        start = time(NULL);
        merge(buffer[i % 2]);
        stop = time(NULL);
//        printf("Merge: %ld\n", stop - start);
        ++bufferWritten;
    }

    pthread_join(writer, NULL);

    return 0;
}