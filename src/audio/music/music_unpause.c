#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

void music_unpause(void) __asm__("music_unpause");

void music_unpause(void) {
    music_stream_state.primary.fade_flags = 4;
    music_stream_state.transition.fade_flags = 4;
    music_stream_state.secondary.fade_flags = 4;
}
