//
// Created by elias on 3/17/19.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "get_time.c"
#include "../DataStructures/structures.h"

// Arrays size <= SMALL switches to insertion sort
#define SMALL    32

extern double get_time(void);

void merge(row a[], int size, row temp[]);

void insertion_sort(row a[], int size);

void mergesort_serial(row a[], int size, row temp[]);

void mergesort_parallel_omp(row a[], int size, row temp[], int threads);

void run_omp(row a[], int size, row temp[], int threads);

//int main(int argc, char *argv[]);

//int
//main(int argc, char *argv[]) {
//    puts("-OpenMP Recursive Mergesort-\t");
//    // Check arguments
//    if (argc != 3)        /* argc must be 3 for proper execution! */
//    {
//        printf("Usage: %s array-size number-of-threads\n", argv[0]);
//        return 1;
//    }
//    // Get arguments
//    int size = atoi(argv[1]);    // Array size
//    int threads = atoi(argv[2]);    // Requested number of threads
//    // Check nested parallelism availability
//    omp_set_nested(1);
//    if (omp_get_nested() != 1) {
//        puts("Warning: Nested parallelism desired but unavailable");
//    }
//    // Check processors and threads
//    int processors = omp_get_num_procs();    // Available processors
//    printf("Array size = %d\nProcesses = %d\nProcessors = %d\n",
//           size, threads, processors);
//    if (threads > processors) {
//        printf
//                ("Warning: %d threads requested, will run_omp on %d processors available\n",
//                 threads, processors);
//        omp_set_num_threads(threads);
//    }
//    int max_threads = omp_get_max_threads();    // Max available threads
//    if (threads > max_threads)    // Requested threads are more than max available
//    {
//        printf("Error: Cannot use %d threads, only %d threads available\n",
//               threads, max_threads);
//        return 1;
//    }
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
//    run_omp(a, size, temp, threads);
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

void main() {


    return 0;
}

// Driver
void
run_omp(row a[], int size, row temp[], int threads) {
    // Enable nested parallelism, if available
    omp_set_nested(1);
    // Parallel mergesort
    mergesort_parallel_omp(a, size, temp, threads);
}

// OpenMP merge sort with given number of threads
void
mergesort_parallel_omp(row a[], int size, row temp[], int threads) {
    if (threads == 1) {
//        printf("Thread %d begins serial merge sort\n", omp_get_thread_num());
        mergesort_serial(a, size, temp);
    } else if (threads > 1) {
#pragma omp parallel sections
        {
//                      printf("Thread %d begins recursive section\n", omp_get_thread_num());
#pragma omp section
            {            //printf("Thread %d begins recursive call\n", omp_get_thread_num());
                mergesort_parallel_omp(a, size / 2, temp, threads / 2);
            }
#pragma omp section
            {            //printf("Thread %d begins recursive call\n", omp_get_thread_num());
                mergesort_parallel_omp(a + size / 2, size - size / 2,
                                       temp + size / 2, threads - threads / 2);
            }
        }
        // Thread allocation is implementation dependent
        // Some threads can execute multiple sections while others are idle
        // Merge the two sorted sub-arrays through temp
        merge(a, size, temp);
    } else {
        printf("Error: %d threads\n", threads);
        return;
    }
}

void
mergesort_serial(row a[], int size, row temp[]) {
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
merge(row a[], int size, row temp[]) {
    int i1 = 0;
    int i2 = size / 2;
    int tempi = 0;
    while (i1 < size / 2 && i2 < size) {
        if (memcpy(a[i1].key, a[i2].key, 10) < 0) {
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

//void
//insertion_sort(row a[], int size) {
//    row i;
//    for (i = 0; i < size; i++) {
//        int j, v = a[i];
//        for (j = i - 1; j >= 0; j--) {
//            if (a[j] <= v)
//                break;
//            a[j + 1] = a[j];
//        }
//        a[j + 1] = v;
//    }

    void
    insertion_sort(row arr[], int n)
    {
        row key;
        int i,  j;
        for (i = 1; i < n; i++) {
            key = arr[i];
            j = i - 1;

            /* Move elements of arr[0..i-1], that are
              greater than key, to one position ahead
              of their current position */
            while (j >= 0 && memcpy(arr[j].key, key.key, 10) >0) {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
            arr[j + 1] = key;
        }
    }
