#include "my_list.h"
#include <stdio.h>

const int FREE_POS = -1;

err_code_t list_ctor(my_list *list, size_t size)
{
    list->size = size;
    list->data = (list_val_t *) calloc(size, sizeof(list_val_t));
    list->next = (labels_t   *) calloc(size, sizeof(labels_t  ));
    list->prev = (labels_t   *) calloc(size, sizeof(labels_t  ));

    for (size_t i = 1; i < size; i++)
    {
        list->prev[i] = list->next[i] = FREE_POS;
    }
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
        printf("%02d ", list.next[i]);
    }

    printf("\nPrinting [prev]: ");
    for (size_t i = 0; i < list.size; i++)
    {
        printf("%02d ", list.prev[i]);
    }

    printf("\n");

    return OK;
}

err_code_t print_list(my_list list)
{
    size_t i = 1;
    while (list.next[i] != 0)
    {
        printf("%d ", list.data[list.next[i]]);

        i++;
    }

    printf("\n");

    return OK;
}

err_code_t list_insert(my_list *list, size_t pos, list_val_t value)
{
    pos += 1;

    if (list->head == 0)
    {
        list->data[1] = value;
        list->next[1] = 0;
        list->prev[1] = 0;
        list->head    = 1;
        list->free    = 2;
    }

    else
    {
        list->free = find_first_free(*list);
        list->data[list->free] = value;

        labels_t buffer        = list->next[pos - 1];
        list->next[pos - 1]    = list->free;
        list->next[list->free] = buffer;

        buffer                 = list->prev[pos - 1]; // TODO - remove -1 via prev array
        printf("\n\nPos = %lu, buffer = %d, free = %d \n", pos, buffer, list->free);
        list_dump(*list);
        // list->prev[pos - 1]    = list->free;
        list->prev[list->free] = pos - 1;
        list->prev[list->next[list->free]] = list->free;
    }

    return OK;
}

size_t find_first_free(my_list list)
{
    size_t first_free_pos = 1;

    for (size_t i = 0; i < list.size; i++)
    {
        if (list.next[i] == FREE_POS)
        {
            first_free_pos = i;
            break;
        }
    }

    return first_free_pos;
}

err_code_t list_remove(my_list list)
{
    return OK;
}
