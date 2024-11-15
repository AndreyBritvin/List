#include "my_list.h"
#include "my_log.h"
#include <stdio.h>

int main()
{
    // my_list test_list = {};
    LIST_INIT(test_list);

    list_ctor(&test_list, 10);
    enable_logging("list_dump/dump.html");

    list_insert(&test_list, get_tail(&test_list), 2);
    print_list(&test_list);

    list_insert(&test_list, 1, 4);
    print_list(&test_list);

    list_insert(&test_list, 2, 6);
    print_list(&test_list);

    list_insert(&test_list, 3, 8);
    print_list(&test_list);

    list_insert(&test_list, 2, 5);
    print_list(&test_list);

    list_remove(&test_list, 3);
    print_list(&test_list);

    list_insert(&test_list, get_head(&test_list), 90);
    print_list(&test_list);

    list_remove(&test_list, 1);
    print_list(&test_list);

    list_insert(&test_list, 2, 20);
    print_list(&test_list);
    // list_linearize_very_slow(&test_list);
    for (int i = 1; i < 10; i++)
    {
        list_remove(&test_list, get_head(&test_list));
    }

    for (int i = 1; i < 20; i++)
    {
        list_insert(&test_list, get_head(&test_list), i * 10);
    }

    for (int i = 1; i < 10; i++)
    {
        list_remove(&test_list, get_head(&test_list));
    }

    list_insert(&test_list, get_head(&test_list), 10);
    list_insert(&test_list, get_head(&test_list), 20);
    test_list.next[2] = 0;
    LIST_DUMP(test_list, 2, "broken");

    list_dtor(&test_list);
    disable_logging();

    return 0;
}
