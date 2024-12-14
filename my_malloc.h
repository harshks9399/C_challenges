#ifndef MY_MALLOC_H
#define MY_MALLOC_H



#include<unistd.h>
static unsigned int _alloc = 0;
#define my_Malloc(x) (void * )sbrk(x) ; _alloc += x;
#define freeall_malloc() (void *)sbrk(-_alloc); _alloc = 0;

#endif