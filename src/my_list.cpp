#include "my_list.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "my_log.h"

#define CHECK_LIST; if(err_code_t err_num = list_verificator(*list))     \
    {                                                                   \
        fprintf(stderr, "Error %d occured, check enum :(\n", err_num);  \
                                                                        \
        return err_num;                                                 \
    }                                                                   \

err_code_t list_ctor(my_list *list, size_t capacity)
{
    assert(list != NULL);

    list->capacity = capacity;
    list->data = (list_val_t *) calloc(capacity, sizeof(list_val_t));
    list->next = (labels_t   *) calloc(capacity, sizeof(labels_t  ));
    list->prev = (labels_t   *) calloc(capacity, sizeof(labels_t  ));

    if (!(list->data && list->next && list->prev)) // CHECK if any is null
    {
        return ERROR_LIST_ALLOCATION_MEMORY;
    }

    for (size_t i = 1; i < capacity; i++)
    {
        list->next[i] = -i - 1;
        list->prev[i] = FREE_POS;
    }

    list->next[0] = 0;
    list->prev[0] = 0;
    list->free    = 1;
    list->size    = 0;

    return OK;
}

err_code_t list_dtor(my_list *list)
{
    assert(list != NULL);

    CHECK_LIST;

    for (size_t i = 0; i < list->size; i++)
    {
        list->data[i] = EMPTY_POS;
        list->next[i] = list->prev[i] = EMPTY_POS;
    }

    free(list->data); list->data = NULL;
    free(list->next); list->next = NULL;
    free(list->prev); list->prev = NULL;

    return OK;
}

err_code_t print_list(my_list *list)
{
    assert(list != NULL);

    CHECK_LIST;

    labels_t          previous_next   = list->next[0];
    while (list->next[previous_next] != list->next[0])
    {
        printf("%d ", list->data[previous_next]);
        previous_next = list->next[previous_next];
    }

    printf("\n");

    return OK;
}

err_code_t list_insert(my_list *list, size_t pos, list_val_t value)
{
    assert(list != NULL);

    CHECK_LIST;
    if (list->size >= list->capacity - 1)
    {
        return ERROR_LIST_OVERFLOW;
    }

    LOG("<pre>------------------------------------------------------------------"
        "\nList before insert:\n");
    list_dump(*list);

    labels_t free_buffer = -list->next[list->free]; // TODO: DSL

    list->data[list->free] = value;

    list->next[list->free] = list->next[pos];
    list->next[pos]        = list->free;

    list->prev[list->free]             = pos;
    list->prev[list->next[list->free]] = list->free;

    list->free = free_buffer;
    list->size += 1;

    LOG("\nList AFTER insert:\n");
    list_dump(*list);
    LOG("\nEnd list dump\n------------------------------------------------------------------"
        "</pre>");

    return OK;
}

err_code_t list_remove(my_list *list, size_t pos)
{
    assert(list != NULL);

    CHECK_LIST;

    if ((int) list->size <= 0)
    {
        return ERROR_LIST_EMPTY;
    }

    LOG("<pre>"
        "List before delete:\n");
    list_dump(*list);

    list->data[pos]             = EMPTY_POS;
    list->next[list->prev[pos]] = list->next[pos];
    list->prev[list->next[pos]] = list->prev[pos];

    list->prev[pos] = FREE_POS;
    list->next[pos] = -list->free;
    list->free      = pos;
    list->size -= 1;

    LOG("\nList AFTER delete:\n");
    list_dump(*list);
    LOG("\nEnd list dump\n------------------------------------------------------------------"
        "</pre>");
    return OK;
}

err_code_t list_verificator(my_list list)
{
    if (list.size >= list.capacity)
    {
        return ERROR_LIST_OVERFLOW;
    }

    if ((int) list.size < 0)
    {
        return ERROR_LIST_EMPTY;
    }

    labels_t curr_pos = list.next[0];
    labels_t previous_index = 0;
    for (size_t i = 0; i < list.size; i++)
    {
        previous_index = curr_pos;
        curr_pos       = list.next[curr_pos];
        if (list.prev[curr_pos] != previous_index)
        {
            return ERROR_LIST_PREV_CORRUPTED;
        }
    }

    if (previous_index != list.prev[0])
    {
        return ERROR_LIST_LOOPED;
    }

    return OK;
}

labels_t get_head(my_list *list)
{
    assert(list != NULL);

    return list->prev[0];
}

labels_t get_tail(my_list *list)
{
    assert(list != NULL);

    return list->next[0];
}
