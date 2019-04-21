#define PAGE_SIZE 4096
#define TUPLE_SIZE 100

struct keyAndOffset {
    unsigned long long long1, long2;
};

union charLong {
    unsigned long long aLong;
    unsigned char array[8];
};

unsigned long long ptrToLong(void *ptr) {

    union charLong input = *(union charLong *) ptr;
    union charLong result;

    result.array[0] = input.array[7];
    result.array[1] = input.array[6];
    result.array[2] = input.array[5];
    result.array[3] = input.array[4];
    result.array[4] = input.array[3];
    result.array[5] = input.array[2];
    result.array[6] = input.array[1];
    result.array[7] = input.array[0];

    return result.aLong;
}

unsigned long long ptrToShortAndOffset(void *ptr1, unsigned long long offset) {

    union charLong input = *(union charLong *) ptr1;
    union charLong result;

    result.aLong = 0;

    result.array[0] = input.array[1];
    result.array[1] = input.array[0];

    return result.aLong | offset << 16;
}

unsigned long long getOffsetFromLong(unsigned long long aLong) {
    return aLong >> 16;
}

struct keyAndOffset ptrToKeyAndOffset(void *ptr, unsigned long long offset) {
    struct keyAndOffset result;
    result.long1 = ptrToLong(ptr);
    result.long2 = ptrToShortAndOffset(ptr + 8, offset);

    return result;
}

int comparator(const void *a, const void *b) {
    if (*(unsigned long long *) a > *(unsigned long long *) b)
        return 1;
    if (*(unsigned long long *) a < *(unsigned long long *) b)
        return -1;

    if (*(unsigned short *) (a + 8) >= *(unsigned short *) (b + 8))
        return 1;
    if (*(unsigned short *) (a + 8) < *(unsigned short *) (b + 8))
        return -1;

    return 0;
}

long long getFileSize(char *fileName) {
    struct stat st;

    stat(fileName, &st);

    return st.st_size;
}
