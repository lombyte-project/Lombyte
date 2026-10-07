#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/type2id/_type2id.s", _type2id);
#else

#include "types.h"

extern u8 D_00132ED8[];

u64 _type2id(s32 id, u64 value) {
    volatile u64 pad;
    u64 result = 0;
    s32 byte_offset;
    s32 shift = 0;
    u64 field;
    u64 *entry;
    u64 type_mask;
    s32 offset;
    if ((u32)id < 0xA) {
        offset = 0x10;
        offset = id * offset;
        byte_offset = offset;
        /* Keep the address expression in the order used by the retail code. */
        entry = (u64 *)((u8 *)D_00132ED8 - (-offset));
        field = entry[1];
        type_mask = (u64)0xFFFF000000;
        if (field == type_mask)
            goto ca;
        if (type_mask < field)
            goto done;
        if (field == ((u64)0xFF00000000))
            goto cb;
        goto done;
    ca:
        shift = 0x18;
        goto done;
    cb:
        shift = 0x20;
    done:
        result = (*((u64 *)(((u8 *)D_00132ED8) + byte_offset))) | (value << shift);
    }
    return result;
}

#endif /* NON_MATCHING */
