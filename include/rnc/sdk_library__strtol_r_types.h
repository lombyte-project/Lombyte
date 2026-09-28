#ifndef RNC_SDK_LIBRARY__STRTOL_R_TYPES_H
#define RNC_SDK_LIBRARY__STRTOL_R_TYPES_H

#include "types.h"

struct malloc_chunk {
    INTERNAL_SIZE_T prev_size;
    INTERNAL_SIZE_T size;
    struct malloc_chunk *fd;
    struct malloc_chunk *bk;
};

#endif /* RNC_SDK_LIBRARY__STRTOL_R_TYPES_H */
