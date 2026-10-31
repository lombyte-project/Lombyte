/* Level 01 moby class door_half. */
#include "types.h"

#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"

#include "rnc/overlay/cuboid.h"

#include "rnc/mobys/moby.h"

extern struct Moby *D_L01_0015FFD8;

/*
 * A moby that slides between two points following another moby's progress:
 * the target (class 0x118) keeps a 0..1 progress in its first pvar. While the
 * progress changes, a looping sound plays; at either end the loop stops and a
 * one-shot sound plays.
 */
struct SliderVars {
    s32 target;  /* moby index of the driver, -1 none */
    f32 start_x; /* position at spawn */
    f32 start_y;
    f32 end_x; /* 0 = not set up */
    f32 end_y;
    s32 sound; /* loop voice handle, -1 none */
    f32 last_progress;
};

/* One voice of the sound system's pool (0x70 bytes, after a 0x70-byte header). */
struct Voice {
    u32          handle;
    u8           state; /* 0 free */
    u8           pad_05[0x13];
    struct Moby *owner; /* 0x18 */
    u8           pad_1C[0x54];
};

/* Retail addresses voice i as pool base + i * 0x70 plus the header and field
   offset (not as &voices[i]), so the pool is viewed through this window. */
struct VoicePoolWindow {
    u8           header[0x70];
    struct Voice voice;
};

/* nonzero while voice `handle` plays (state 1 or 2) for `moby` */

void door_half_update(struct Moby *self) __asm__("FUN_L01_002f4710");

/* Moby class 665 */
void door_half_update(struct Moby *door_half_moby) {
    enum {
        DOOR_HALF_STATE_INITIALIZE = 0,
        DOOR_HALF_STATE_FOLLOW_DRIVER = 1
    };
    struct SliderVars      *door_half_vars;
    struct Moby            *driver;
    f32                    *progress;
    s32                     handle;
    struct VoicePoolWindow *window;

    door_half_vars = (struct SliderVars *)door_half_moby->pvars;
    switch (door_half_moby->state) {
    case DOOR_HALF_STATE_INITIALIZE:
        door_half_vars->start_x = door_half_moby->pos.x;
        door_half_vars->start_y = door_half_moby->pos.y;
        door_half_vars->sound = -1;
        door_half_vars->last_progress = 0.0f;
        door_half_moby->state = 1;
        break;
    case DOOR_HALF_STATE_FOLLOW_DRIVER:
        if (door_half_vars->target == -1 || door_half_vars->end_x == 0.0f ||
            door_half_vars->end_y == 0.0f)
            break;
        driver = &D_L01_0015FFD8[door_half_vars->target];
        if (driver->oclass != 0x118)
            break;

        progress = (f32 *)driver->pvars;
        door_half_moby->pos.x = door_half_vars->start_x +
                                (door_half_vars->end_x - door_half_vars->start_x) * progress[0];
        door_half_moby->pos.y = door_half_vars->start_y +
                                (door_half_vars->end_y - door_half_vars->start_y) * progress[0];
        if (progress[0] != door_half_vars->last_progress) {
            if (progress[0] == 1.0f || progress[0] == 0.0f) {
                /* reached an end: stop the loop, play the stop sound */
                if (voice_is_playing(door_half_moby, door_half_vars->sound)) {
                    handle = door_half_vars->sound;
                    if (handle != -1) {
                        window = (struct VoicePoolWindow *)((u8 *)&voice_pool +
                                                            handle * sizeof(struct Voice));
                        if (window->voice.owner == door_half_moby && window->voice.state != 0)
                            release_voice_slot(handle);
                    }
                    door_half_vars->sound = -1;
                }
                allocate_voice_for_target_entry(1, 0, door_half_moby);
            } else if (!voice_is_playing(door_half_moby, door_half_vars->sound)) {
                door_half_vars->sound = allocate_voice_for_target_entry(0, 4, door_half_moby);
            }
        } else {
            /* not moving: stop the loop */
            if (voice_is_playing(door_half_moby, door_half_vars->sound)) {
                handle = door_half_vars->sound;
                if (handle != -1) {
                    window = (struct VoicePoolWindow *)((u8 *)&voice_pool +
                                                        handle * sizeof(struct Voice));
                    if (window->voice.owner == door_half_moby && window->voice.state != 0)
                        release_voice_slot(handle);
                }
                door_half_vars->sound = -1;
            }
        }
        door_half_vars->last_progress = progress[0];
        break;
    }
}

#include "qcopy.h"
