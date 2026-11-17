#ifndef RNC_BROWSER_MOBY_ANIMATION_ADVANCE_H
#define RNC_BROWSER_MOBY_ANIMATION_ADVANCE_H

#include "types.h"

/* Called when an animation trigger is crossed: (trigger id, 0, moby). */
typedef s32 (*RncAnimationTrigger)(s32, s32, void *);
/* Called for a moby with a voice: updates its looping sound. */
typedef void (*RncAnimationVoice)(void *);

/* advance_moby_animation (EE 0x20d580) on the 0x100-byte moby at m. frame_counter is the
   program's world/level frame counter; only its low two bits are read. */
s32 rnc_advance_moby_animation(u8 *m, s32 frame_counter, RncAnimationTrigger trigger, RncAnimationVoice voice);

#endif
