#include "types.h"

typedef struct ReferenceImage {
    s32 reserved;
    s32 source_x;
    s32 source_y;
    s32 block_x;
    s32 block_y;
} ReferenceImage;

s32 InitializeReferenceImage(ReferenceImage *image, s32 x, s32 y) {
    image->source_x = x;
    image->source_y = y;
    image->block_x = x >> 4;
    image->block_y = y >> 4;
    return 1;
}
