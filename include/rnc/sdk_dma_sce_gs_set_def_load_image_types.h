#ifndef RNC_SDK_DMA_SCE_GS_SET_DEF_LOAD_IMAGE_TYPES_H
#define RNC_SDK_DMA_SCE_GS_SET_DEF_LOAD_IMAGE_TYPES_H

#include "types.h"

struct malloc_chunk {
    INTERNAL_SIZE_T prev_size;
    INTERNAL_SIZE_T size;
    struct malloc_chunk *fd;
    struct malloc_chunk *bk;
};

#endif /* RNC_SDK_DMA_SCE_GS_SET_DEF_LOAD_IMAGE_TYPES_H */
