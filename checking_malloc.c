#include <stdlib.h>
#include "my_malloc.h"

int main() {
    void *c = my_Malloc(20); // Allocate 20 bytes
    freeall_malloc();        // Free all allocated memory
    return 0;
}
