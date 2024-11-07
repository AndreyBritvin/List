#ifdef LIST_IS_PTR
    #define NEXT list->next
    #define PREV list->prev
    #define DATA list->data
    #define FREE list->free
#else
    #define NEXT list.next
    #define PREV list.prev
    #define DATA list.data
    #define FREE list.free
#endif // LIST_IS_PTR
