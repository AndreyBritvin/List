#include "utils.h"
#include "my_list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "my_log.h"
#include "list_dsl.h"

static err_code_t generate_nodes    (FILE* dot_file, my_list list);
static err_code_t place_nodes_in_row(FILE* dot_file, my_list list);
static err_code_t connect_next      (FILE* dot_file, my_list list);
static err_code_t connect_prev      (FILE* dot_file, my_list list);
static err_code_t place_free_node   (FILE* dot_file, my_list list);
static err_code_t init_graph        (FILE* dot_file, my_list list);
static err_code_t end_graph         (FILE* dot_file, my_list list);


err_code_t list_dump(my_list list)
{
    // printf("File ptr = %p\n", LOG_FILE);
    LOG("\nList index =     ");
    for (size_t i = 0; i < list.capacity; i++)
    {
        LOG("%02lu ", i);
    }

    LOG("\nPrinting [data]: ");
    for (size_t i = 0; i < list.capacity; i++)
    {
        LOG("%02d ", DATA[i]);
    }

    LOG("\nPrinting [next]: ");
    for (size_t i = 0; i < list.capacity; i++)
    {
        LOG("%02d ", NEXT[i]);
    }

    LOG("\nPrinting [prev]: ");
    for (size_t i = 0; i < list.capacity; i++) // TODO: 1 f0r
    {
        LOG("%02d ", PREV[i]);
    }

    LOG("\n");
    LOG("List size = %zd\n", list.size);

    size_t graph_num = generate_graph(&list) - 1;

    LOG("<img src = img/%d.png>", graph_num);

    return OK;
}

size_t generate_graph(my_list *list)
{
    assert(list != NULL);

    static size_t graphs_counter = 0;

    const char *txt_filename = "list_dump/txt/%lu.dot";
    const char *base_command = "dot list_dump/txt/%lu.dot -o list_dump/img/%lu.png -Tpng";
    char *implementation     = (char *) calloc(strlen(base_command) + 20, sizeof(char));
    char *txt_full_filename  = (char *) calloc(strlen(txt_filename) + 20, sizeof(char));

    sprintf(implementation, base_command, graphs_counter, graphs_counter);
    sprintf(txt_full_filename, txt_filename, graphs_counter);

    printf("File to create:  %s\n", txt_full_filename);
    printf("Command to call: %s\n", implementation);
    make_graph(txt_full_filename, *list);
    system(implementation);

    free(implementation);
    free(txt_full_filename);

    graphs_counter++;

    return graphs_counter;
}

#define DOT_(...) fprintf(dot_file, __VA_ARGS__)
err_code_t make_graph(char *filename, my_list list)
{
    assert(filename != NULL);

    FILE * SAFE_OPEN_FILE(dot_file, filename, "w");

    init_graph(dot_file, list);

    generate_nodes(dot_file, list);

    place_nodes_in_row(dot_file, list);

    connect_next(dot_file, list);

    place_free_node(dot_file, list);
    connect_prev(dot_file, list);
    end_graph(dot_file, list);

    fclose(dot_file);

    return OK;
}

static err_code_t generate_nodes(FILE* dot_file, my_list list)
{
    for (size_t i = 0; i < list.capacity; i++)
    {
        DOT_("g%zd [shape = record, label = \"<i%zd> index = %zd |"
                                             "<d%zd> data  = %d  |"
                                             "<n%zd> next  = %d  | "
                                             "<p%zd> prev  = %d\"];\n",
                                                i, i, i, i, DATA[i], i, NEXT[i], i, PREV[i]);
    }

    return OK;
}

static err_code_t place_nodes_in_row(FILE* dot_file, my_list list)
{
    DOT_("edge[weight=10 color=invis];\n");
    for (size_t i = 0; i < list.capacity - 1; i++)
    {
        DOT_("g%zd:<i%zd> -> g%zd:<i%zd>;\n", i, i, i + 1, i + 1);
    }

    return OK;
}

static err_code_t connect_next(FILE* dot_file, my_list list)
{
    DOT_("edge[weight=1, color=\"#FF0000\"];\n");
    for (size_t i = 0; i < list.capacity - 1; i++)
    {
        if (NEXT[i] >= 0)
        {
            DOT_("g%zd:<p%zd>:s -> g%zd:<p%zd>:s [color = \"#DD%d00%d\"];\n", i, i, NEXT[i], NEXT[i],
                                                                (i % 2) * 6, (i % 2) * 9);
        }
        else
        {
            DOT_("g%zd:<i%zd>:n -> g%zd:<i%zd>:n [color=\"#0F0FFF\"];\n", i, i, -NEXT[i], -NEXT[i]);
        }
    }

    return OK;
}

static err_code_t connect_prev(FILE* dot_file, my_list list)
{
    DOT_("edge[weight=1, color=\"#00FF00\", constraint = false];\n");
    for (size_t i = 0; i < list.capacity - 1; i++)
    {
        if (PREV[i] != FREE_POS)
        {
            DOT_("g%zd:<i%zd>:n -> g%zd:<i%zd>:n[color=\"#%dFAA0%d\"];\n",
                     i, i, PREV[i], PREV[i], (i % 2) * 9, (i % 2) * 9);
        }
    }

    return OK;
}

static err_code_t place_free_node(FILE* dot_file, my_list list)
{
    DOT_("free_var[shape=record; label=\"free = %zd\"];", FREE);
    DOT_("free_var->g%d [color = blue];", FREE);

    return OK;
}

static err_code_t init_graph(FILE* dot_file, my_list list)
{
    DOT_("digraph{\n");
    DOT_("rankdir = LR;\n splines=true;\n");

    return OK;
}

static err_code_t end_graph(FILE* dot_file, my_list list)
{
    DOT_("}\n");

    return OK;
}
#undef DOT_
#include "undef_dsl.h"
