/***********************************************************************************/
/*                                     LIBRARIES                                   */
/***********************************************************************************/
#include <ctime>
#include "string.h"
#include "stdio.h"
#include "Utils/Timer.c"
#include "DataStructures/structures.h"
#include <sys/stat.h>
#include <cstdlib>
#include <cstdint>
#include <fcntl.h>
#include <sys/mman.h>
#include <algorithm>



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
/*                                 FUNCTIONS                                       */
/***********************************************************************************/
long int calTotalNoOfRows(const char *fileName) {
    struct stat st;

    /*get the size using stat()*/
    stat(fileName, &st);

    return (st.st_size) / ROWSIZE;
}


void printExecutionTime(clock_t &t);


/***********************************************************************************/
/*                           Sorting Comparator                                    */
/***********************************************************************************/
int rowCmp(const void *row1,
           const void *row2) {

    row *newRow1 = (struct row *) row1;
    row *newRow2 = (struct row *) row2;

    int result = memcmp(newRow1->key, newRow2->key, 10);

    return result;
}

/***********************************************************************************/
/*                          Print Execution Time                                   */
/***********************************************************************************/
void printExecutionTime(clock_t &t, const char *msg) {
    t = clock() - t;
    double time_taken = (((double) t) / CLOCKS_PER_SEC); // in seconds

    printf("%s took: [%f] seconds to execute. \n", msg, time_taken);
}

void readFile(row *rows) {
    FILE *file;
    row buf;

    file = fopen("input", "rb");
    if (file == NULL) {
        fprintf(stderr, "\nError opening file\n");
    }

    long idx = 0;
    while (fread(&buf, sizeof(struct row), 1, file)) {
        memcpy(&rows[idx], &buf, sizeof(struct row));
        ++idx;
    }

    fclose(file);
}

void bulkfRead(row *rows, const unsigned int totalRows) {
    const unsigned char *memblock;
    int fd;
    struct stat sb;

    fd = open("input", O_RDONLY);
    fstat(fd, &sb);
    printf("Size: %lu\n", (uint64_t)sb.st_size);

    memblock = (unsigned const char*)(mmap(NULL, sb.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0));
    if (memblock == (unsigned const char*)MAP_FAILED) handle_error("mmap");

    int idx = 0;
    for(int i = 0; i< totalRows; i += ROWSIZE){
        memcpy(&rows[idx], &memblock + i, sizeof(struct row));
        idx +=1;
    }

}

void writeOutput(const row *rows, const long int totalRows) {
    FILE *write_ptr;
    write_ptr = fopen("output", "wb");  // w for write, b for binary
    for (int i = 0; i < totalRows; i++) {
        fwrite(&rows[i], sizeof(struct row), 1, write_ptr);
    }
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
//    std::sort(rows, rows + total_rows, rows[0]);
    printExecutionTime(t1, "Sorting the file");

    clock_t t2 = startTimer();
    writeOutput(rows, total_rows);
    printExecutionTime(t2, "Writing the file");

    return 0;
}




