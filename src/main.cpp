#include "my_list.h"

int main()
{
    my_list test_list = {};
    list_ctor(&test_list, 10);

    list_insert(&test_list, 0, 2);
    print_list(test_list);
    list_dump(test_list);

    list_insert(&test_list, 1, 4);
    print_list(test_list);
    list_dump(test_list);

    list_insert(&test_list, 2, 6);
    print_list(test_list);
    list_dump(test_list);

    list_dtor(&test_list);

    return 0;
}
