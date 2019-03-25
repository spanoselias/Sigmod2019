
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <memory.h>
#include "Structures.h"
#include "Files.c"

unsigned long total_rows;

// Macro for swapping two values.
#define SWAP(x, y) do {\
    __typeof__(x) tmp = x;\
    x = y;\
    y = tmp;\
} while(0)

/**
 * Partition the array.  Takes the index of the pivot point as the pivot
 * argument.  Puts all of the values lower than this point into the first part
 * of the array and returns the new location of the pivot point.
 */
int partition(row *array, int left, int right, int pivot) {
    row pivotValue = array[pivot];
    SWAP(array[pivot], array[right]);
    int storeIndex = left;
    for (int i = left; i < right; i++) {
        if (memcmp(array[i].key, pivotValue.key, 10) <= 0) {
            SWAP(array[i], array[storeIndex]);
            storeIndex++;
        }
    }
    SWAP(array[storeIndex], array[right]);
    return storeIndex;
}

/**
 * Serial Quicksort implementation.
 */
void quicksort(row *array, int left, int right) {
    if (right > left) {
        int pivotIndex = left + (right - left) / 2;
        pivotIndex = partition(array, left, right, pivotIndex);
        quicksort(array, left, pivotIndex - 1);
        quicksort(array, pivotIndex + 1, right);
    }
}

/**
 * Structure containing the arguments to the parallel_quicksort function.  Used
 * when starting it in a new thread, because pthread_create() can only pass one
 * (pointer) argument.
 */
struct qsort_starter {
    row *array;
    int left;
    int right;
    int depth;
};

void parallel_quicksort(row *array, int left, int right, int depth);

/**
 * Thread trampoline that extracts the arguments from a qsort_starter structure
 * and calls parallel_quicksort.
 */
void *quicksort_thread(void *init) {
    struct qsort_starter *start = init;
    parallel_quicksort(start->array, start->left, start->right, start->depth);
    return NULL;
}

/**
 * Parallel version of the quicksort function.  Takes an extra parameter:
 * depth.  This indicates the number of recursive calls that should be run in
 * parallel.  The total number of threads will be 2^depth.  If this is 0, this
 * function is equivalent to the serial quicksort.
 */
void parallel_quicksort(row *array, int left, int right, int depth) {
    if (right > left) {
        int pivotIndex = left + (right - left) / 2;
        pivotIndex = partition(array, left, right, pivotIndex);
        // Either do the parallel or serial quicksort, depending on the depth
        // specified.
        if (depth-- > 0) {
            // Create the thread for the first recursive call
            struct qsort_starter arg = {array, left, pivotIndex - 1, depth};
            pthread_t thread;
            int ret = pthread_create(&thread, NULL, quicksort_thread, &arg);
            assert((ret == 0) && "Thread creation failed");
            // Perform the second recursive call in this thread
            parallel_quicksort(array, pivotIndex + 1, right, depth);
            // Wait for the first call to finish.
            pthread_join(thread, NULL);
        } else {
            quicksort(array, left, pivotIndex - 1);
            quicksort(array, pivotIndex + 1, right);
        }
    }
}

int debugMain(int argc, char **argv) {

    printf("Sorting is starting...\n");

    if (argc != 3) {
        printf("[in-file] [outfile]\n");
        exit(1);
    }

    // Retrieve total number of rows.
    total_rows = calTotalNoOfRows(argv[1]);

    row *rows = (row *) (malloc(sizeof(struct row) * total_rows));
    readNFile(rows, total_rows, argv[1]);

    parallel_quicksort(rows, 0, total_rows - 1, 11);

    writeOutput(rows, total_rows, argv[2]);
}

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("[in-file] [outfile]\n");
        exit(1);
    }

    // Retrieve total number of rows.
    total_rows = calTotalNoOfRows(argv[1]);

    row *rows = (row *) (malloc(sizeof(struct row) * total_rows));
    readNFile(rows, total_rows, argv[1]);

    parallel_quicksort(rows, 0, total_rows - 1, 7);

    writeOutput(rows, total_rows, argv[2]);

    return 0;
}