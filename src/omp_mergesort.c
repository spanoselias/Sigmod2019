
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "Timer.c"
#include "Structures.h"
#include "Files.c"

int total_rows;

// Arrays size <= SMALL switches to insertion sort
#define SMALL    32

extern double get_time(void);

void merge(row a[], int size, row temp[]);

void insertion_sort(row a[], int size);

void mergesort_serial(row a[], int size, row temp[]);

void mergesort_parallel_omp(row a[], int size, row temp[], int threads);

void run_omp(row a[], int size, row temp[], int threads);

int main(int argc, char **argv) {
    printf("Merge sort is running...");


    // Check nested parallelism availability
    omp_set_nested(1);
    if (omp_get_nested() != 1) {
        puts("Warning: Nested parallelism desired but unavailable");
    }

 // Retrieve total number of rows.
    total_rows = calTotalNoOfRows(argv[1]);

    int size = total_rows;    // Array size
    int threads = 30;    // Requested number of threads
    row *rows = (row *) (malloc(sizeof(struct row) * total_rows));
    row *temp = (row *) (malloc(sizeof(struct row) * total_rows));

    clock_t t0 = startTimer();
    readNFile(rows, total_rows, argv[1]);
    printExecutionTime(t0, "Reading the file");

    clock_t t1 = startTimer();
    run_omp(rows, size, temp, threads);
    printExecutionTime(t1, "Sorting the file");

    clock_t t2 = startTimer();
    bulkWriteOutput(rows, total_rows, argv[2]);
    printExecutionTime(t2, "Writing the file");

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
        if (memcmp(&a[i1].key, &a[i2].key, 10) < 0) {
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
    memcpy(a, temp, size * sizeof(row));
}

void insertion_sort(row arr[], int size)
{
    int i, j;
    row key;
    for (i = 1; i < size; i++) {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && memcmp(&arr[j].key, key.key, 10) > 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}