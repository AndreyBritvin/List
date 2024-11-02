#include "my_list.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int FREE_POS = -1;

static FILE * LOG_FILE = NULL;

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
    LOG("\nList index =     ");
    for (size_t i = 0; i < list.size; i++)
    {
        LOG("%02lu ", i);
    }

    LOG("\nPrinting [data]: ");
    for (size_t i = 0; i < list.size; i++)
    {
        LOG("%02d ", list.data[i]);
    }

    LOG("\nPrinting [next]: ");
    for (size_t i = 0; i < list.size; i++)
    {
        LOG("%02d ", list.next[i]);
    }

    LOG("\nPrinting [prev]: ");
    for (size_t i = 0; i < list.size; i++)
    {
        LOG("%02d ", list.prev[i]);
    }

    LOG("\n");

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

    size_t graph_num = generate_graph(&list) - 1;

    LOG("<img src = img/%d.png>", graph_num);

    return OK;
}

err_code_t list_insert(my_list *list, size_t pos, list_val_t value)
{
    pos += 1;
    LOG("<pre>"
        "List before insert:\n");
    list_dump(*list);

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
        // printf("\n\nPos = %lu, buffer = %d, free = %d \n", pos, buffer, list->free);
        // list_dump(*list);
        // list->prev[pos - 1]    = list->free;
        list->prev[list->free] = pos - 1;
        list->prev[list->next[list->free]] = list->free;
    }

    LOG("List AFTER insert:\n");
    list_dump(*list);
    LOG("</pre>");

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

//TODO: MOVE TO ANOTHER FILE
err_code_t enable_logging(const char *filename)
{
    SAFE_OPEN_FILE(LOG_FILE, filename, "w");

    return OK;
}

err_code_t disable_logging()
{
    fclose(LOG_FILE);

    return OK;
}

size_t generate_graph(my_list *list)
{
    static size_t graphs_counter = 0;

    char *txt_filename      = "list_dump/txt/%lu.dot";
    char *base_command      = "dot list_dump/txt/%lu.dot -o list_dump/img/%lu.png -Tpng";
    char *implementation     = (char *) calloc(strlen(base_command) + 20, sizeof(char));
    char *txt_full_filename = (char *) calloc(strlen(txt_filename) + 20, sizeof(char));

    sprintf(implementation, "dot list_dump/txt/%lu.dot -o list_dump/img/%lu.png -Tpng", graphs_counter, graphs_counter);
    sprintf(txt_full_filename, "list_dump/txt/%lu.dot", graphs_counter);

    printf("File to create:  %s\n", txt_full_filename);
    printf("Command to call: %s\n", implementation);
    make_graph(txt_full_filename, list);
    system(implementation);

    free(implementation);
    free(txt_full_filename);

    graphs_counter++;

    return graphs_counter;
}

err_code_t make_graph(char *filename, my_list *list)
{
#define DOT_(...) fprintf(dot_file, __VA_ARGS__)
    FILE * SAFE_OPEN_FILE(dot_file, filename, "w");

    DOT_("digraph{\n");
    DOT_("rankdir = LR\n");

    for (size_t i = 0; i < list->size; i++)
    {
        DOT_("g%zd [shape = record, label = \"index = %zd|data = %d |next = %d|prev = %d\"];\n",
                                                i, i, list->data[i], list->next[i], list->prev[i]);
    }
    for (size_t i = 0; i < list->size - 1; i++)
    {
        DOT_("g%zd -> g%zd;\n", i, i + 1);
    }

    DOT_("}\n");
    fclose(dot_file);

    return OK;
#undef DOT_
}
