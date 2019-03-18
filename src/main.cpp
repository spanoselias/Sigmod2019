/***********************************************************************************/
/*                                     LIBRARIES                                   */
/***********************************************************************************/
#include <ctime>
#include "string.h"
#include "stdio.h"
#include "../Utils/Timer.c"
#include "../Utils/Files.c"
#include "../DataStructures/Structures.h"
#include <sys/stat.h>
#include <cstdlib>
#include <cstdint>
#include <fcntl.h>
#include <sys/mman.h>
#include <algorithm>
#include <unistd.h>


// https://stackoverflow.com/questions/17598572/read-write-to-binary-files-in-c
// set stack size unlimited ( ulimit -s hard )
// http://icps.u-strasbg.fr/~bastoul/local_copies/lee.html
// https://lemire.me/blog/2012/06/26/which-is-fastest-read-fread-ifstream-or-mmap/
// https://github.com/gary-funck/parallel-merge-sort

#define DEBUG 1
#define ROWSIZE 100
#define BUFFER_SIZE 100
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Global Variables
long int total_rows;



/***********************************************************************************/
/*                           Sorting Comparator                                    */
/***********************************************************************************/
inline int rowCmp(const void *row1,
                  const void *row2) {

    return memcmp(((struct row *) row1)->key, ((struct row *) row2)->key, 10);
}

bool rowCmp2(row row1,
             row row2) {

    return memcmp(row1.key, row2.key, 10) < 1;

}

int main() {

    // Retrieve total number of rows.
    total_rows = calTotalNoOfRows("input");

    row rows[total_rows];

    clock_t t0 = startTimer();
    bulkfRead(rows, total_rows);

//  readFile(rows);
    printExecutionTime(t0, "Reading the file");

    size_t structs_len = sizeof(rows) / sizeof(struct row);

    clock_t t1 = startTimer();

    // Sorting Algorithm. Need to be implemented a parallel sorting algorithm
    qsort(rows, structs_len, sizeof(struct row), rowCmp);
//  std::sort(rows, rows + total_rows, rows[0]);
    printExecutionTime(t1, "Sorting the file");

//    clock_t t2 = startTimer();
//    writeOutput(rows, total_rows);
//    printExecutionTime(t2, "Writing the file");

    clock_t t3 = startTimer();
    bulkWriteOutput(rows, total_rows);
    printExecutionTime(t3, "Writing the file");

    return 0;
}




