#ifndef RCP2P_RNC_SDK_LIBRARY_MALLOC_CHUNK_H
#define RCP2P_RNC_SDK_LIBRARY_MALLOC_CHUNK_H

#include "types.h"

/* Heap chunk header of the newlib malloc. */
struct malloc_chunk {
    u32                  prev_size;
    u32                  size;
    struct malloc_chunk *fd;
    struct malloc_chunk *bk;
};

#endif /* RCP2P_RNC_SDK_LIBRARY_MALLOC_CHUNK_H */
