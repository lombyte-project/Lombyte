#include "types.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/audio/music/music_stream_state.h"
extern void request_audio_stream_break() __asm__("FUN_002166e8");

s32 FUN_0021bda0(void) {
    if (music_stream_state.read_state == 0) {
        if (level_map_selection.sel != -1) {
            menu_system.pending_buffer = 0;
            level_map_selection.slot_id[level_map_selection.sel] ^= 0x1000;
            level_map_selection.sel = -1;
        }
    }
    if (music_stream_state.read_state != 0) {
        if (menu_system.pending_buffer != 0) {
            request_audio_stream_break();
            menu_system.pending_buffer = 0;
            level_map_selection.slot_id[level_map_selection.sel] = -1;
            level_map_selection.sel = -1;
        }
    }
    return 0;
}

extern __typeof__(FUN_0021bda0) func_0021BDA0 __attribute__((alias("FUN_0021bda0")));
