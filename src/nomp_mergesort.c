//
// Created by elias on 3/17/19.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "../Sorting/get_time.c"
#include "Timer.c"
#include "Structures.h"

#ifndef SORTINGALGORITHM_MERGESORT_H
#define SORTINGALGORITHM_MERGESORT_H


// Arrays size <= SMALL switches to insertion sort
#define SMALL    30

extern double get_time(void);

void merge(row a[], long int size, row temp[]);

void insertion_sort(row a[], long int size);

void mergesort_serial(row a[], long int size, row temp[]);

void mergesort_parallel_omp(row a[], long int size, row temp[], int threads);

void run_omp(row a[], long int size, row temp[], int threads);

#define DEBUG 1
#define ROWSIZE 100
#define BUFFER_SIZE 100
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)


//int main() {
//
//    // Retrieve total number of rows.
//    total_rows = calTotalNoOfRows("input");
//
//    row rows[total_rows];
//    row temp[total_rows];
//
//    clock_t t0 = startTimer();
//    bulkfRead(rows, total_rows);
//    printExecutionTime(t0, "Reading the file");
//
////  readFile(rows);
////    printExecutionTime(t0, "Reading the file");
//
//    size_t structs_len = sizeof(rows) / sizeof(struct row);
//
//    clock_t t1 = startTimer();
//
//    // Parallel merge sort sorting Algorithm.
//    run_omp(rows, total_rows, temp, 4);
//    printExecutionTime(t1, "Sorting the file");
//
//    clock_t t2 = startTimer();
//    writeOutput(rows, total_rows);
//    printExecutionTime(t2, "Writing the file");
//
//
//    return 0;
//}

// Driver
void
run_omp(row a[], long int size, row temp[], int threads) {
    // Enable nested parallelism, if available
    omp_set_nested(1);
    // Parallel mergesort
    mergesort_parallel_omp(a, size, temp, threads);
}

// OpenMP merge sort with given number of threads
void
mergesort_parallel_omp(row a[], long int size, row temp[], int threads) {
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
mergesort_serial(row a[], long int size, row temp[]) {
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
merge(row a[], long int size, row temp[]) {
    long int i1 = 0;
    long int i2 = size / 2;
    long int tempi = 0;
    while (i1 < size / 2 && i2 < size) {
        if (memcmp(a[i1].key, a[i2].key, 10) < 0) {
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
    memcpy(a, temp, size * sizeof(struct row));
}

void
insertion_sort(row arr[], long int n) {
    row key;
    long int i, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && memcmp(arr[j].key, key.key, 10) > 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

#endif //SORTINGALGORITHM_MERGESORT_H
