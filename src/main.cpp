#include "my_list.h"

int main()
{
    my_list test_list = {};
    list_ctor(&test_list, 10);

    print_list(test_list);
    list_dump(test_list);

    list_dtor(&test_list);

    return 0;
}
