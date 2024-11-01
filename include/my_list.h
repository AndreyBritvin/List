#ifndef MY_LIST_H_
#define MY_LIST_H_

#include <stdlib.h>

typedef int list_val_t;

struct my_list
{
    list_val_t *data;
    size_t     *next;
    size_t     *prev;
    size_t      size;
    size_t      head;
    size_t      tail;
};

#endif // MY_LIST_H_
