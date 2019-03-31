
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// gcc -O3 ompExample.c -m64  -fopenmp -o exp
int main (int argc, char *argv[])
{
    int nthreads, tid;

#pragma omp parallel num_threads(2)

#pragma omp task
    printf("Thread = %d\n", tid);

///* Fork a team of threads giving them their own copies of variables */
//#pragma omp parallel private(nthreads, tid)
//    {
//
//        /* Obtain thread number */
//        tid = omp_get_thread_num();
//        printf("Hello World from thread = %d\n", tid);
//
//        /* Only master thread does this */
//        if (tid == 0)
//        {
//            nthreads = omp_get_num_threads();
//            printf("Number of threads = %d\n", nthreads);
//        }
//
//    }  /* All threads join master thread and disband */

}
