#include <stdio.h>
#include <stdlib.h>

int main() {

    char *c = calloc(100, 1);

    printf("%lu\n", 0xFF00000000000000);

    c[7] = 0xFF;
//    *((char *) (&x) + 7) = 0xFF;

    printf("%llu\n", *((unsigned long long *) c));

    return 0;
}