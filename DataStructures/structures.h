//
// Created by elias on 3/15/19.
//

#ifndef SORTINGALGORITHM_STRUCTURES_H
#define SORTINGALGORITHM_STRUCTURES_H

#include <cstdlib>

typedef struct row {
    unsigned char key[10] ;
    unsigned char data[90];
}row;

void readFile(row *rows);

#endif //SORTINGALGORITHM_STRUCTURES_H
