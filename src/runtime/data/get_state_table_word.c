#include "types.h"
struct StateTable {
    u8 pad_0[0x50004];
    s32 unk50004;
};

s32 GetStateTableWord(struct StateTable *table, s32 requested) {
    s32 taken;
    s32 result;

    taken = table->unk50004;
    result = taken;
    if (requested < taken) {
        taken = requested;
    }
    result -= taken;
    table->unk50004 = result;
    return taken;
}
