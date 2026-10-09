#include "types.h"
#include "rnc/rendering/graphics_buffer.h"
#include "rnc/storage/disc_table.h"
#include "rnc/ui/menus/menu_system.h"

#include "rnc/audio/music/music_stream_state.h"
#include "rnc/ui/menus/menu_screen.h"

struct WordCell {
    s32 unk0;
};


extern s32 D_001D5CF8[];
extern void initialize_graphics_buffer_descriptors(s32) __asm__("func_00225AC0");
extern s32 start_audio_stream_read(s32, s32, s32) __asm__("FUN_00216788");

s32 FUN_0021d1f8(struct MenuScreen *menu) {
    s32 i;
    struct MenuSystem *g;

    initialize_graphics_buffer_descriptors(1);
    menu->data.raw.unk54 = 0;
    menu->data.raw.unk38 = 0;
    g = &menu_system;
    for (i = 0; i < 5; i++) {
        if (graphics_buffer_descriptors[i].address != 0 && (u32)graphics_buffer_descriptors[i].address < (u32)g->unk10C) {
            graphics_buffer_descriptors[i].flags |= 2;
        }
    }
    menu->data.raw.unk50 = 0;
    if (music_stream_state.read_state == 0) {
        if (start_audio_stream_read(D_001D5CF8[0], disc_table.help_text.sector, disc_table.help_text.size) != 0) {
            menu->data.raw.unk50 = 1;
        } else {
            menu->data.raw.unk50 = 3;
        }
    }
    menu->unk10 |= 4;
    return 0;
}

extern __typeof__(FUN_0021d1f8) func_0021D1F8 __attribute__((alias("FUN_0021d1f8")));
