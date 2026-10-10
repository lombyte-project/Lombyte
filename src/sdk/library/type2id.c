#include "types.h"

/* {base id, field mask} per type; the mask selects the shift of value. */
struct TypeEntry {
    u64 id;
    u64 type;
};

extern struct TypeEntry D_00132ED8[];

u64 _type2id(s32 id, s32 value) {
    u64 result = 0;
    s32 shift = 0;

    if ((u32)id < 10) {
        switch (D_00132ED8[id].type) {
        case 0xFFFFFFFFFF:
            shift = 0;
            break;
        case 0xFFFF000000:
            shift = 24;
            break;
        case 0xFF00000000:
            shift = 32;
            break;
        }
        result = D_00132ED8[id].id | ((u64)value << shift);
    }
    return result;
}

