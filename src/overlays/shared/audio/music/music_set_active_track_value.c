/* Music: sets a value of the active music track. */
#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "asm.h"

/* Ported from rac1-decomp (src/overlays/shared/music_002664B0.c: func_L00_002666C8), where it is exact; names translated to the US level program. */

/* Stores v in pause_timer of the three music channels whose pause flag 0x8000 is set. */
void music_set_active_track_value(int v) __asm__("FUN_L00_00265880");

void music_set_active_track_value(int entry_value) {
    struct MusicStreamState *music = &music_stream_state;
    if (music->primary.pause_flags & 0x8000)
        music->primary.pause_timer = entry_value;
    if (music->transition.pause_flags & 0x8000)
        music->transition.pause_timer = entry_value;
    if (music->secondary.pause_flags & 0x8000)
        music->secondary.pause_timer = entry_value;
}
