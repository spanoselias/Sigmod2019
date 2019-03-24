//
// Created by elias on 3/15/19.
//

#include <bits/types/clock_t.h>
#include <time.h>
#include <stdio.h>

#ifndef UTILS_TIMER_C
#define UTILS_TIMER_C

clock_t startTimer() {
    clock_t t;
    t = clock();
    return t;
}

void printExecutionTime(const clock_t t, const char *msg) {
    clock_t t1 = clock() - t;
    double time_taken = (((double) t1) / CLOCKS_PER_SEC); // in seconds

    printf("%s took: [%f] seconds to execute. \n", msg, time_taken);
}

#endif //SORTINGALGORITHM_STRUCTURES_H


