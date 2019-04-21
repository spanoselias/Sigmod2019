#include <sys/stat.h>
#include <string.h>

#define PAGE_SIZE 4096
#define TUPLE_SIZE 100

long long getFileSize(char *fileName) {
    struct stat st;

    stat(fileName, &st);

    return st.st_size;
}

int cmp(const void *a, const void *b) {
    return memcmp(a, b, 10);
}


