#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

void music_unpause(void) __asm__("music_unpause");

void music_unpause(void) {
    music_stream_state.primary.pause_flags = 4;
    music_stream_state.transition.pause_flags = 4;
    music_stream_state.secondary.pause_flags = 4;
}
