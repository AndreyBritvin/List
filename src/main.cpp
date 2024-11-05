#include "my_list.h"

int main()
{
    my_list test_list = {};
    list_ctor(&test_list, 10);
    enable_logging("list_dump/dump.html");

    list_insert(&test_list, 0, 2);
    print_list(test_list);

    list_insert(&test_list, 1, 4);
    print_list(test_list);

    list_insert(&test_list, 2, 6);
    print_list(test_list);

    list_insert(&test_list, 3, 8);
    print_list(test_list);

    list_insert(&test_list, 2, 5);
    print_list(test_list);

    list_remove(&test_list, 3);
    print_list(test_list);

    list_dtor(&test_list);
    disable_logging();

    return 0;
}
