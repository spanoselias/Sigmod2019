#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <tgmath.h>
#include "Files.c"
#include "Structures.h"
#include "Timer.c"

#define SWAP(x, y) do {\
    __typeof__(x) tmp = x;\
    (x) = y;\
    (y) = tmp;\
} while(0)

// Constants & Variables (Test Related)
//===========================================================

int total_rows;

// for time measurements
struct timeval startwtime, endwtime;
double seq_time;


// Constants & Variables (Algorithm Related)
//===========================================================

row *a; //array to sort with bitonic sort

const int ASCENDING = 1;
const int DESCENDING = 0;

int N;                   //problem size
int P;                   //number of threads (user option)
int Nthreads ;            //number of threads (maximum to be created)
int current_threads = 0; //current number of threads (actually created)

int p;  //log2(number of threads, user option)
int q;  //log2(problem size)

struct args {

    int lo;
    int cnt;
    int dir;
}; // arguments to pass to recursive bitonic sort function

const int parallel_threshold = 1;


// Function Declaration
//===========================================================

void parse_arguments(int argc, char *argv[]);

void init(int argc, char *argv[]);

void parallelBitonicSorting(void);

int cmpfunc(const void *, const void *);

void rec_bitonic_sort(int, int, int);

void bitonic_merge(int, int, int);

void compare(int, int, int);

int cmpfunc_asc(const void *, const void *);

int cmpfunc_des(const void *, const void *);


// Main
//===========================================================

int main(int argc, char *argv[]) {

    // Retrieve total number of rows.
    total_rows = calTotalNoOfRows(argv[1]);

    P = 1 << 2;
    N = 1 << total_rows;

    Nthreads = P;
    if (P > (N/2)) {
        Nthreads = N/2;
        p = q - 1;
    }


    a = (row *) (malloc(sizeof(struct row) * total_rows));

    N = total_rows;

    clock_t t0 = startTimer();
    readNFile(a, total_rows, argv[1]);
    printExecutionTime(t0, "Reading the file");

    clock_t t1 = startTimer();
    parallelBitonicSorting();
    printExecutionTime(t1, "Sorting the file");

    clock_t t2 = startTimer();
    bulkWriteOutput(a, total_rows, argv[2]);
    printExecutionTime(t2, "Writing the file");

    return (0);
}


// Function Definition 
//===========================================================

// function : parse_arguments()
// description : Parse the user arguments and store the inputs
//               to the respective global variables.
//               (see doc. for parsing)
//---------------------------------------------------------------------


// function : init()
// description : Allocate memory for the arrays (bitonic sort array and
//               qsort) and for pthreads. Also, initialize arrays.
//---------------------------------------------------------------------

void init(int argc, char *argv[]) {


}

// function : parallelBitonicSorting()
// description : Create the pthreads and join them with the current
//               function. Execution of the bitonic computation starts here.
//               Also, measure the time to execute.
//---------------------------------------------------------------------

void parallelBitonicSorting(void) {

    // sort the array
#pragma omp parallel num_threads(Nthreads)
#pragma omp single nowait
    rec_bitonic_sort(0, total_rows, ASCENDING);
}

// function : bitonic_merge()
// description : The bitonic merge algorithm.
//---------------------------------------------------------------------

//void bitonic_merge(int lo, int cnt, int dir) {
//    if (cnt > 1) {
//
//        int k = cnt / 2;
//        int i;
//
//        for (i = lo; i < lo + k; i++) {
//            compare(i, i + k, dir);
//        }
//
//        bitonic_merge(lo, k, dir);
//        bitonic_merge(lo + k, k, dir);
//    }
//}


int greatestPowerOfTwoLessThan(int n) {
    int k = 1;
    while (k > 0 && k < n)
        k = k << 1;
    return k >> 1;
}

void bitonic_merge(int lo, int cnt, int dir) {

    if (cnt > 1) {

//      int k = cnt / 2;
        int k = greatestPowerOfTwoLessThan(cnt);

        for (int i = lo; i < lo + cnt - k; i++) {
            compare(i, i + k, dir);
        }

        bitonic_merge(lo, k, dir);
        bitonic_merge(lo + k, (cnt - k), dir);
    }
}

// function : rec_bitonic_sort()
// description : The recursive bitonic sort algortithm executes from
//               each pthread.
//---------------------------------------------------------------------

void rec_bitonic_sort(int lo, int cnt, int dir) {
    if (cnt > 1) {

        int k = cnt / 2;

        // Sorting Part 1
        //----------------------------

        // create new thread and split the work
#pragma omp task
        {
            if (k > parallel_threshold) {

                rec_bitonic_sort(lo, k, !DESCENDING);
            } else {

                qsort(a + lo, k, sizeof(row), cmpfunc_des);
            }
        }

        // Sorting Part 2
        //----------------------------

        // create new thread and split the work
#pragma omp task
        {
            if (k > parallel_threshold) {

                rec_bitonic_sort(lo + k, cnt - k, !ASCENDING);
            } else {

                qsort(a + lo + k, (cnt - k), sizeof(row), cmpfunc_asc);
            }
        }

        // synchronize tasks
#pragma omp taskwait

        // Merging Part
        //----------------------------

        bitonic_merge(lo, cnt, dir);
    }


}

// function : compare()
// description: Compare two positions and swap (if necessary) to get
//              the desired direction (i.e. ascending).
//---------------------------------------------------------------------

void compare(int i, int j, int dir) {
    if (dir == (memcmp(&a[i].key, &a[j].key, 10) > 0)) {

        SWAP(a[i], a[j]);
    }
}

//void compare(int i, int j, int dir) {
//    if (dir == (memcmp(&a[i].key, &a[j].key, 10) > 0)) {
//        SWAP(a[i], a[j]);
//    }
//}

int cmpfunc_asc(const void *a, const void *b) {
//	return ( *(int*)a - *(int*)b );
    row *newRow1 = ((row *) a);
    row *newRow2 = ((row *) b);

    return memcmp(newRow1->key, newRow2->key, 10);
}

int cmpfunc_des(const void *a, const void *b) {
//	return ( - *(int*)a + *(int*)b );
    row *newRow1 = ((row *) a);
    row *newRow2 = ((row *) b);

    return (-memcmp(newRow1->key, newRow2->key, 10));
}