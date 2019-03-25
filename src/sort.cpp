/***********************************************************************************/
/*                                     LIBRARIES                                   */
/***********************************************************************************/
#include <ctime>
#include <cstring>
#include <cstdio>
#include "Timer.c"
#include "Files.c"
#include "Structures.h"
#include <sys/stat.h>
#include <cstdlib>
#include <cstdint>
#include <fcntl.h>
#include <sys/mman.h>
#include <algorithm>
#include <unistd.h>

#define DEBUG 1
#define ROWSIZE 100
#define BUFFER_SIZE 100

// Global Variables
unsigned long total_rows;


/***********************************************************************************/
/*                           Sorting Comparator                                    */
/***********************************************************************************/
inline int rowCmp(const void *row1,
                  const void *row2) {

    row *newRow1 = ((row *) row1);
    row *newRow2 = ((row *) row2);

    return memcmp(newRow1->key, newRow2->key, 10);
}

bool comparison(row lhs, row rhs) { return memcmp(lhs.key, rhs.key, 10) < 1; }

int main(int argc, char *argv[]) {

    if (DEBUG)
        printf("Sorting is starting...\n");

    if (argc != 3) {
        printf("[in-file] [outfile]\n");
        exit(1);
    }

    if (DEBUG)
        printf("Retrieving total rows...\n");

    // Retrieve total number of rows.
    total_rows = calTotalNoOfRows(argv[1]);

    row *rows = (row *) (malloc(sizeof(struct row) * total_rows));

    clock_t t0 = startTimer();
    readNFile(rows, total_rows, argv[1]);

    if (DEBUG)
        printExecutionTime(t0, "Reading the file");

    clock_t t1 = startTimer();

     qsort(rows, total_rows, sizeof(row), rowCmp);
//   std::sort(rows, rows + (total_rows), comparison);

    if (DEBUG)
        printExecutionTime(t1, "Sorting the file");

    clock_t t2 = startTimer();
    writeOutput(rows, total_rows, argv[2]);
    if (DEBUG)
        printExecutionTime(t2, "Writing the file");

    return 0;
}




