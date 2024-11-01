#ifndef MY_LIST_H_
#define MY_LIST_H_

#include <stdlib.h>
#include "utils.h"

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

err_code_t list_ctor(my_list *list, size_t size);
err_code_t list_dtor(my_list *list);

err_code_t list_dump(my_list list);
err_code_t print_list(my_list list);

err_code_t list_insert(my_list list, list_val_t value);
err_code_t list_remove(my_list list);

#endif // MY_LIST_H_
