#ifndef LOMBYTE_RNC_GAME_TRANSITIONS_TRANSITION_DO_TRANSITION_H
#define LOMBYTE_RNC_GAME_TRANSITIONS_TRANSITION_DO_TRANSITION_H

#include "types.h"

/* A sound transition_do_transition plays while render_sequence.time is in
   [start, end] (end -1: once from start on); voice is the voice playing it. */
typedef struct {
    s32 start;
    s32 end;
    s32 sound;
    s32 voice;
} SoundCue;

/* Five cues and the start == -1 entry that ends the loop over them. */
extern SoundCue transition_sound_cues[6] __asm__("D_001862B0");

#endif /* LOMBYTE_RNC_GAME_TRANSITIONS_TRANSITION_DO_TRANSITION_H */
