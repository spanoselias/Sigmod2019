//
// Created by elias on 3/17/19.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#if _POSIX_TIMERS

#include <time.h>

#else
#include <sys/time.h>
#endif

// Arrays size <= SMALL switches to insertion sort
#define SMALL    32

void merge(int a[], int size, int temp[]);

void insertion_sort(int a[], int size);

void mergesort_serial(int a[], int size, int temp[]);

//extern double get_time(void);

//int main(int argc, char *argv[]);

//int
//main(int argc, char *argv[]) {
//    puts("-Serial Recursive Mergesort-\t");
//    // Check arguments
//    if (argc != 2)        /* argc must be 2 for proper execution! */
//    {
//        printf("Usage: %s array-size\n", argv[0]);
//        return 1;
//    }
//    // Get arguments
//    int size = atoi(argv[1]);    // Array size
//    printf("Array size = %d\n", size);
//    // Array allocation
//    int *a = malloc(sizeof(int) * size);
//    int *temp = malloc(sizeof(int) * size);
//    if (a == NULL || temp == NULL) {
//        printf("Error: Could not allocate array of size %d\n", size);
//        return 1;
//    }
//    // Random array initialization
//    int i;
//    srand(314159);
//    for (i = 0; i < size; i++) {
//        a[i] = rand() % size;
//    }
//    // Sort
//    double start = get_time();
//    mergesort_serial(a, size, temp);
//    double end = get_time();
//    printf("Start = %.2f\nEnd = %.2f\nElapsed = %.2f\n",
//           start, end, end - start);
//    // Result check
//    for (i = 1; i < size; i++) {
//        if (!(a[i - 1] <= a[i])) {
//            printf("Implementation error: a[%d]=%d > a[%d]=%d\n", i - 1,
//                   a[i - 1], i, a[i]);
//            return 1;
//        }
//    }
//    puts("-Success-");
//    return 0;
//}

void
mergesort_serial(int a[], int size, int temp[]) {
    // Switch to insertion sort for small arrays
    if (size <= SMALL) {
        insertion_sort(a, size);
        return;
    }
    mergesort_serial(a, size / 2, temp);
    mergesort_serial(a + size / 2, size - size / 2, temp);
    // Merge the two sorted subarrays into a temp array
    merge(a, size, temp);
}

void
merge(int a[], int size, int temp[]) {
    int i1 = 0;
    int i2 = size / 2;
    int tempi = 0;
    while (i1 < size / 2 && i2 < size) {
        if (a[i1] < a[i2]) {
            temp[tempi] = a[i1];
            i1++;
        } else {
            temp[tempi] = a[i2];
            i2++;
        }
        tempi++;
    }
    while (i1 < size / 2) {
        temp[tempi] = a[i1];
        i1++;
        tempi++;
    }
    while (i2 < size) {
        temp[tempi] = a[i2];
        i2++;
        tempi++;
    }
    // Copy sorted temp array into main array, a
    memcpy(a, temp, size * sizeof(int));
}

void
insertion_sort(int a[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        int j, v = a[i];
        for (j = i - 1; j >= 0; j--) {
            if (a[j] <= v)
                break;
            a[j + 1] = a[j];
        }
        a[j + 1] = v;
    }
}

int main() {

    int a1[] = {5, 2, 1, 7, 3};

    int temp[5];

    mergesort_serial(a1, sizeof(a1) / sizeof(a1[0]), temp);

    printf("\n[");
    for (int i = 0; i < sizeof(a1) / sizeof(a1[0]); i++) {
        if (i == sizeof(a1) / sizeof(a1[0]) - 1) {
            printf("%d", a1[i]);
        } else {
            printf("%d,", a1[i]);

        }

    }

    printf("]\n");

}