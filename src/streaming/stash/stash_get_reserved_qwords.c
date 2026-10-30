#include "types.h"
#include "rnc/streaming/stash/stash.h"

s32 stash_get_reserved_qwords(s32 slot) {
    if ((u32)slot >= (u32)STASH_SLOT_COUNT) {
        return -3;
    }
    return stash_entries[slot].reserved_qwords;
}
