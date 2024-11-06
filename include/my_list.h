#ifndef MY_LIST_H_
#define MY_LIST_H_

#include <stdlib.h>
#include "utils.h"
#include <stdio.h>

// extern FILE* LOG_FILE;

const int  FREE_POS = -1;
const int EMPTY_POS =  0;

typedef int list_val_t;
typedef int labels_t;

struct my_list
{
    list_val_t *     data;
    labels_t   *     next;
    labels_t   *     prev;
    labels_t         free;
    size_t           size;
    size_t       capacity;
};

err_code_t list_ctor(my_list *list, size_t size);
err_code_t list_dtor(my_list *list);

err_code_t print_list(my_list *list);

err_code_t list_insert(my_list *list, size_t pos, list_val_t value);
err_code_t list_remove(my_list *list, size_t pos);

labels_t get_head(my_list *list);
labels_t get_tail(my_list *list);

err_code_t list_verificator(my_list list);

err_code_t list_dump(my_list list);
size_t generate_graph(my_list *list);
err_code_t make_graph(char *filename, my_list *list);

#endif // MY_LIST_H_
