#include "my_list.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "my_log.h"
#define LIST_IS_PTR
#include "list_dsl.h"

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
    DATA = (list_val_t *) calloc(capacity, sizeof(list_val_t));
    NEXT = (labels_t   *) calloc(capacity, sizeof(labels_t  ));
    PREV = (labels_t   *) calloc(capacity, sizeof(labels_t  ));

    if (!(DATA && NEXT && PREV)) // CHECK if any is null
    {
        return ERROR_LIST_ALLOCATION_MEMORY;
    }

    for (size_t i = 1; i < capacity; i++)
    {
        NEXT[i] = -i - 1;
        PREV[i] = FREE_POS;
    }

    NEXT[0]    = 0;
    PREV[0]    = 0;
    FREE       = 1;
    list->size = 0;

    return OK;
}

err_code_t list_dtor(my_list *list)
{
    assert(list != NULL);

    CHECK_LIST;

    for (size_t i = 0; i < list->size; i++)
    {
        DATA[i] = EMPTY_POS;
        NEXT[i] = PREV[i] = EMPTY_POS;
    }

    free(DATA); DATA = NULL;
    free(NEXT); NEXT = NULL;
    free(PREV); PREV = NULL;

    return OK;
}

err_code_t print_list(my_list *list)
{
    assert(list != NULL);

    CHECK_LIST;

    labels_t    previous_next   = NEXT[0];
    while (NEXT[previous_next] != NEXT[0])
    {
        printf("%d ", DATA[previous_next]);
        previous_next = NEXT[previous_next];
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

    LOG("<pre>------------------------------------------------------------------\n"
        "Ineserting in list at pos = %zd, value = %d\n"
        "List before insert:\n", pos, value);
    LIST_DUMP(*list, pos, "INSERT AFTER THIS");

    labels_t free_buffer = -NEXT[FREE]; // TODO: DSL

    DATA[FREE] = value;

    NEXT[FREE] = NEXT[pos];
    NEXT[pos]  = FREE;

    PREV[FREE]       = pos;
    PREV[NEXT[FREE]] = FREE;

    FREE = free_buffer;
    list->size += 1;

    LOG("\nList AFTER insert:\n");
    LIST_DUMP(*list, NEXT[pos], "INSERTED HERE");
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
        "Deleting element at pos = %zd\n"
        "List before delete:\n", pos);
    LIST_DUMP(*list, pos, "DELETE THIS ELEMENT");

    DATA[pos]       = EMPTY_POS;
    NEXT[PREV[pos]] = NEXT[pos];
    PREV[NEXT[pos]] = PREV[pos];

    PREV[pos] = FREE_POS;
    NEXT[pos] = -FREE;
    FREE      = pos;
    list->size -= 1;

    LOG("\nList AFTER delete:\n");
    LIST_DUMP(*list, pos, "DELETED");
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

    return PREV[0];
}

labels_t get_tail(my_list *list)
{
    assert(list != NULL);

    return NEXT[0];
}

err_code_t list_linearize_very_slow(my_list *list)
{
    assert(list != NULL);

    CHECK_LIST;

    for (size_t i = 1; i < list->capacity; i++)
    {
        for (size_t j = i; j < list->capacity - 1; j++)
        {
            if (abs(NEXT[j]) > abs(NEXT[j + 1]))
            {
                NEXT[PREV[j + 1]] = NEXT[j + 1];
                labels_t buff_next_1 = NEXT[j + 1];
                NEXT[j + 1] = PREV[j];
                PREV[NEXT[j]] = PREV[j];
                PREV[j] = buff_next_1;
                labels_t   buff_prev = PREV[j];
                labels_t   buff_next = NEXT[j];
                list_val_t buff_data = DATA[j];
                NEXT[j] = NEXT[j + 1];
                PREV[j] = PREV[j + 1];
                DATA[j] = DATA[j + 1];
                NEXT[j + 1] = buff_next;
                PREV[j + 1] = buff_prev;
                DATA[j + 1] = buff_data;
                LIST_DUMP(*list, j, "Changed");
            }
        }
    }

    return OK;
}
#include "undef_dsl.h"

