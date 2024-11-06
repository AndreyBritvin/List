#include "my_log.h"
#include "utils.h"
#include <assert.h>

FILE* LOG_FILE;

err_code_t enable_logging(const char *filename)
{
    assert(filename != NULL);

    SAFE_OPEN_FILE(LOG_FILE, filename, "w");

    return OK;
}

err_code_t disable_logging()
{
    fclose(LOG_FILE); // TODO: check for double close

    return OK;
}
