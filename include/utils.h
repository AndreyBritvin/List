#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>

#define SAFE_OPEN_FILE(file_ptr, filename, mode)                                   \
                        file_ptr = fopen(filename, mode);                    \
                        if (file_ptr == NULL)                                      \
                        {                                                          \
                            printf("Unable open output file '%s'\n", filename);    \
                            return ERROR_FILE;                                     \
                        }

// header utils
typedef int err_code_t;

enum errors
{
    OK,
    ERROR_FILE,
    ERROR_LIST_ALLOCATION_MEMORY,
    ERROR_LIST_EMPTY,
    ERROR_LIST_OVERFLOW,
    ERROR_LIST_PREV_CORRUPTED,
    ERROR_LIST_LOOPED,
};

#endif //UTILS_H_
