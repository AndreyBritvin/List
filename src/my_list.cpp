#include "my_list.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int  FREE_POS = -1;
const int EMPTY_POS =  0;

static FILE * LOG_FILE = NULL;

err_code_t list_ctor(my_list *list, size_t size)
{
    list->size = size;
    list->data = (list_val_t *) calloc(size, sizeof(list_val_t));
    list->next = (labels_t   *) calloc(size, sizeof(labels_t  ));
    list->prev = (labels_t   *) calloc(size, sizeof(labels_t  ));

    if (!(list->data && list->next && list->prev)) // CHECK if any is null
    {
        return ERROR_LIST_ALLOCATION_MEMORY;
    }

    for (size_t i = 1; i < size; i++)
    {
        list->next[i] = -i - 1;
        list->prev[i] = FREE_POS;
    }

    list->next[0] = 0;
    list->prev[0] = 0;
    list->free    = 1;

    return OK;
}

err_code_t list_dtor(my_list *list)
{
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

    size_t graph_num = generate_graph(&list) - 1;

    LOG("<img src = img/%d.png>", graph_num);

    return OK;
}

err_code_t print_list(my_list list)
{
    labels_t previous_next = list.next[0];
    while (list.next[previous_next] != list.next[0])
    {
        printf("%d ", list.data[previous_next]);
        previous_next = list.next[previous_next];
    }

    printf("\n");

    return OK;
}

err_code_t list_insert(my_list *list, size_t pos, list_val_t value)
{
    LOG("<pre>------------------------------------------------------------------"
        "\nList before insert:\n");
    list_dump(*list);

    // list->free = find_first_free(*list);
    labels_t free_buffer = -list->next[list->free];

    list->data[list->free] = value;

    list->next[list->free] = list->next[pos];
    list->next[pos]        = list->free;

    list->prev[list->free]             = pos;
    list->prev[list->next[list->free]] = list->free;

    list->free = free_buffer;

    LOG("\nList AFTER insert:\n");
    list_dump(*list);
    LOG("\nEnd list dump\n------------------------------------------------------------------"
        "</pre>");

    return OK;
}

err_code_t list_remove(my_list *list, size_t pos)
{
    LOG("<pre>"
        "List before delete:\n");
    list_dump(*list);

    list->data[pos]             = EMPTY_POS;
    list->next[list->prev[pos]] = list->next[pos];
    list->prev[list->next[pos]] = list->prev[pos];

    list->prev[pos] = FREE_POS;
    list->next[pos] = -list->free;
    list->free      = pos;

    LOG("List AFTER delete:\n");
    list_dump(*list);
    LOG("</pre>");

    return OK;
}

labels_t get_head(my_list *list)
{
    return list->prev[0];
}

labels_t get_tail(my_list *list)
{
    return list->next[0];
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

#define DOT_(...) fprintf(dot_file, __VA_ARGS__)
err_code_t make_graph(char *filename, my_list *list)
{
    FILE * SAFE_OPEN_FILE(dot_file, filename, "w");

    DOT_("digraph{\n");
    DOT_("rankdir = LR;\n splines=true;\n");

    for (size_t i = 0; i < list->size; i++)
    {
        DOT_("g%zd [shape = record, label = \"<i%zd> index = %zd |"
                                             "<d%zd> data  = %d  |"
                                             "<n%zd> next  = %d  | "
                                             "<p%zd> prev  = %d\"];\n",
                                                i, i, i, i, list->data[i], i, list->next[i], i, list->prev[i]);
    }

    DOT_("edge[weight=10 color=invis];\n");
    for (size_t i = 0; i < list->size - 1; i++)
    {
        DOT_("g%zd:<i%zd> -> g%zd:<i%zd>;\n", i, i, i + 1, i + 1);
    }

    DOT_("edge[weight=1, color=\"#FF0000\"];\n");
    for (size_t i = 0; i < list->size - 1; i++)
    {
        if (list->next[i] >= 0)
        {
            DOT_("g%zd:<p%zd>:s -> g%zd:<p%zd>:s [color = \"#DD%d00%d\"];\n", i, i, list->next[i], list->next[i],
                                                                (i % 2) * 6, (i % 2) * 9);
        }
        else
        {
            DOT_("g%zd:<i%zd>:n -> g%zd:<i%zd>:n [color=\"#0F0FFF\"];\n", i, i, -list->next[i], -list->next[i]);
        }
    }

    DOT_("free_var[shape=record; label=\"free = %zd\"];", list->free);
    DOT_("free_var->g%d [color = blue];", list->free);

    DOT_("edge[weight=1, color=\"#00FF00\", constraint = false];\n");
    for (size_t i = 0; i < list->size - 1; i++)
    {
        if (list->prev[i] != FREE_POS)
        {
            DOT_("g%zd:<i%zd>:n -> g%zd:<i%zd>:n[color=\"#%dFAA0%d\"];\n",
                     i, i, list->prev[i], list->prev[i], (i % 2) * 9, (i % 2) * 9);
        }
    }

    DOT_("}\n");
    fclose(dot_file);

    return OK;
}
#undef DOT_
