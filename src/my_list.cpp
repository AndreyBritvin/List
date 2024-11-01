#include "my_list.h"
#include <stdio.h>

err_code_t list_ctor(my_list *list, size_t size)
{
    list->size = size;
    list->data = (list_val_t *) calloc(size, sizeof(list_val_t));
    list->next = (size_t     *) calloc(size, sizeof(size_t    ));
    list->prev = (size_t     *) calloc(size, sizeof(size_t    ));
    // TODO: handle errors for NULL PTRS
    return OK;
}

err_code_t list_dtor(my_list *list)
{
    for (size_t i = 0; i < list->size; i++)
    {
        list->data[i] = 0;
        list->next[i] = list->prev[i] = 0;
    }

    free(list->data); list->data = NULL;
    free(list->next); list->next = NULL;
    free(list->prev); list->prev = NULL;

    return OK;
}

err_code_t list_dump(my_list list)
{
    printf("\nList index =     ");
    for (size_t i = 0; i < list.size; i++)
    {
        printf("%02lu ", i);
    }

    printf("\nPrinting [data]: ");
    for (size_t i = 0; i < list.size; i++)
    {
        printf("%02d ", list.data[i]);
    }

    printf("\nPrinting [next]: ");
    for (size_t i = 0; i < list.size; i++)
    {
        printf("%02lu ", list.next[i]);
    }

    printf("\nPrinting [prev]: ");
    for (size_t i = 0; i < list.size; i++)
    {
        printf("%02lu ", list.prev[i]);
    }

    printf("\n");

    return OK;
}

err_code_t print_list(my_list list)
{
    for (size_t i = 0; i < list.size; i++)
    {
        printf("%d ", list.data[list.next[i]]);
    }

    printf("\n");

    return OK;
}
