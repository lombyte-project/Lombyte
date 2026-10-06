#include "types.h"
typedef struct HelpMsg {
    s32 unk0;
    s32 id;
    s32 unk8;
    s32 unkC;
} HelpMsg;

struct HelpMessageState {
    u8 pad_0[0x2C];
    s32 unk2C;
};

extern struct HelpMessageState D_001996D0;
extern HelpMsg *D_0015F6A0;
s32 find_help_message_index(s32 message_id) __asm__("FUN_001fdca0");

s32 find_help_message_index(s32 message_id) {
    s32 i;
    s32 found = -1;

    i = 0;
    if (D_001996D0.unk2C > 0) {
        if (D_0015F6A0[0].id == message_id) {
            found = 0;
        } else {
        next:
            i++;
            if (i < D_001996D0.unk2C) {
                if (D_0015F6A0[i].id == message_id) {
                    found = i;
                } else {
                    goto next;
                }
            }
        }
    }
    return found;
}

extern __typeof__(find_help_message_index) func_001fdca0 __attribute__((alias("FUN_001fdca0")));
