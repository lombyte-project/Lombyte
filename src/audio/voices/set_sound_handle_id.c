#include "types.h"
struct Handle {
    u32 id;
    u8 pad4[6];
    s16 state;
};
extern char D_00160068[];
extern void DebugPrint(char *, ...);
void set_sound_handle_id(u32 id, s64 arg) __asm__("FUN_00216b68");

void set_sound_handle_id(u32 id, s64 arg) {
    struct Handle *playback_record = (struct Handle *)(s32)arg;

    if (playback_record != 0) {
        if (playback_record->id != 0xFFFFFFFF) {
            DebugPrint(D_00160068);
        } else {
            playback_record->id = id;
            if (id == 0) {
                playback_record->state = 7;
            }
        }
    }
}

extern __typeof__(set_sound_handle_id) func_00216B68 __attribute__((alias("FUN_00216b68")));
