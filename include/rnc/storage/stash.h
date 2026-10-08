#ifndef LOMBYTE_RNC_STORAGE_STASH_H
#define LOMBYTE_RNC_STORAGE_STASH_H

#include "types.h"

/* One stash transfer record: address, count and tag. */
struct StashEntry {
    s32 addr;
    s32 count;
    s32 tag;
    s32 pad;
};

#endif /* LOMBYTE_RNC_STORAGE_STASH_H */
