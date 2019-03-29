#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <assert.h>

#define PAGE_SIZE 4096

char *data;

int comparator(const void *p, const void *q) {
    int diff = ((unsigned long long) p >> 35) - ((unsigned long long) q >> 35);

    if (diff != 0) return diff;


}

long long getFileSize(char *fileName) {
    struct stat st;

    stat(fileName, &st);

    return st.st_size;
}

int main() {
    time_t start = time(NULL);

    char *inputFileName = "../../../input";
    long long inputFileSize = getFileSize(inputFileName);
    data = malloc(inputFileSize);

    int inputFile = open(inputFileName, O_RDONLY);

    for (long long i = 0; i < inputFileSize; i += PAGE_SIZE)
        read(inputFile, (data + i), PAGE_SIZE);

    read(inputFile, (data + inputFileSize - inputFileSize % PAGE_SIZE),
         inputFileSize % PAGE_SIZE);

    int arraySize = inputFileSize / 100;
    unsigned long long *array = malloc(arraySize * sizeof(long long));

    time_t stop = time(NULL);
    printf("Read from file: %ld\n", stop - start);

    start = time(0);
    for (long long i = 0; i < arraySize; ++i)
        array[i] = (*(unsigned long long *) &data[i * 100] & 0xFFFFFFF800000000)
                   | (unsigned long long) i * 100;

    stop = time(0);
    printf("Make array for sorting: %ld\n", stop - start);

    for (int j = 0; j < 8; ++j)
        printf("%d ", data[j]);
    for (int j = 100; j < 108; ++j)
        printf("%d ", data[j]);

    printf("\n");
    for (int j = 0; j < 16; ++j)
        printf("%d ", *((char *) array + j));

    start = time(0);

    qsort(array, arraySize, sizeof(unsigned long long), comparator);

    stop = time(0);
    printf("\nSort the elements: %ld\n", stop - start);

    start = time(0);
    FILE *outputFile = fopen("../../../output", "wb+");

    for (int i = 0; i < arraySize; ++i) {
        fwrite(data + (array[i] & 0x00000007FFFFFFFF), 100, 1, outputFile);
    }

    stop = time(0);
    printf("\nWrite to file: %ld\n", stop - start);

    fclose(outputFile);
    free(data);
    free(array);

    return 0;
}