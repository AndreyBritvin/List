#ifndef MY_LOG_H_
#define MY_LOG_H_

#include <stdio.h>
#include "utils.h"

#ifndef EXTERN
#define EXTERN extern
#else
#define EXTERN static
#endif // EXTERN

#define LOG(...) if (LOG_FILE != NULL) fprintf(LOG_FILE, __VA_ARGS__)

extern FILE* LOG_FILE;

err_code_t enable_logging (const char *filename);
err_code_t disable_logging();

#endif // MY_LOG_H_
