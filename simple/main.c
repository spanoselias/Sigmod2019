#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

unsigned long long getFileSize(char *fileName) {
    struct stat st;

    stat(fileName, &st);

    return st.st_size;
}

int cmp(const void *pVoid, const void *pVoid1) {
    return memcmp(pVoid, pVoid1, 10);
}

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("in out");
        exit(-1);
    }

    long long fileSize = getFileSize(argv[1]);

    char *data = malloc(fileSize);

    FILE *input = fopen(argv[1], "r");

    time_t start = time(NULL);
    fread(data, fileSize, 1, input);
    printf("Read: %ld\n", time(NULL) - start);

    start = time(NULL);
    qsort(data, fileSize / 100, 100, cmp);
    printf("Sort: %ld\n", time(NULL) - start);

    FILE *output = fopen(argv[2], "w+");

    start = time(NULL);
    fwrite(data, fileSize, 1, output);
    printf("Write: %ld\n", time(NULL) - start);

    return 0;
}