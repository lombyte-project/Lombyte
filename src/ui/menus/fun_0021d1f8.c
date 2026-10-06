#include "types.h"
#include "rnc/ui/menus/fun_0021d1f8.h"

typedef struct {
    u32 key;
    s32 flags;
} PadBind;

extern struct Globals_00137B80 D_00137B80;
extern struct MusicStreamState D_001516D0;
extern struct Globals_001D5BF0 D_001D5BF0;
extern s32 D_001D5CF8[];
extern PadBind D_001D60B8[];
extern void initialize_graphics_buffer_descriptors(s32) __asm__("func_00225AC0");
extern s32 start_audio_stream_read(s32, s32, s32) __asm__("FUN_00216788");

s32 FUN_0021d1f8(struct MenuScreen *menu) {
    s32 i;
    struct Globals_001D5BF0 *g;

    initialize_graphics_buffer_descriptors(1);
    menu->unk54 = 0;
    menu->unk38 = 0;
    g = &D_001D5BF0;
    for (i = 0; i < 5; i++) {
        if (D_001D60B8[i].key != 0 && D_001D60B8[i].key < (u32)g->unk10C) {
            D_001D60B8[i].flags |= 2;
        }
    }
    menu->unk50 = 0;
    if (D_001516D0.pending_start_state == 0) {
        if (start_audio_stream_read(D_001D5CF8[0], D_00137B80.unk1528, D_00137B80.unk152C) != 0) {
            menu->unk50 = 1;
        } else {
            menu->unk50 = 3;
        }
    }
    menu->unk10 |= 4;
    return 0;
}

extern __typeof__(FUN_0021d1f8) func_0021D1F8 __attribute__((alias("FUN_0021d1f8")));
