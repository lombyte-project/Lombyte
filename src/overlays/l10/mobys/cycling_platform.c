/* Level 10 moby class cycling_platform. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/quad.h"

extern int   debug_print() __asm__("FUN_001e93b0");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern char  cycling_platform_missing_pvars_message[] __asm__("D_L10_001DC510");
void         update_cycling_platform(char *platform_moby) __asm__("FUN_L10_00295a38");
extern f32   scale_time(f32) __asm__("FUN_001f96b0");

void update_cycling_platform(char *platform_moby) {
    enum {
        PLATFORM_STATE_INITIALIZE = 0,
        PLATFORM_STATE_RISING = 1,
        PLATFORM_STATE_WAIT_AT_UPPER_HEIGHT = 2,
        PLATFORM_STATE_DESCENDING = 3,
        PLATFORM_STATE_WAIT_AT_ORIGINAL_HEIGHT = 4
    };
    char *platform_vars = ((char *)MOBY(platform_moby)->pvars);
    if (platform_vars == 0) {
        debug_print(cycling_platform_missing_pvars_message,
                    MOBY(platform_moby)->oclass,
                    ((short)MOBY(platform_moby)->save_id));
        mark_moby_for_removal(platform_moby);
        return;
    }
    switch (MOBY(platform_moby)->state) {
    case PLATFORM_STATE_INITIALIZE:
        platform_vars[0x28] = 4;
        *(short *)(platform_vars + 0x3E) = 5;
        *(int *)(platform_vars + 0x20) = 0;
        *(short *)(platform_vars + 0x24) = 0;
        *(OvlQuad *)(platform_vars + 0xA0) = *(OvlQuad *)&MOBY(platform_moby)->pos;
        platform_moby[0x20] = 1;
        break;
    case PLATFORM_STATE_RISING: {
        float lim;
        MOBY(platform_moby)->pos.z = MOBY(platform_moby)->pos.z + 1.5f / scale_time(45.0f);
        lim = *(float *)(platform_vars + 0xA8) + 1.5f;
        if (lim <= MOBY(platform_moby)->pos.z) {
            MOBY(platform_moby)->pos.z = lim;
            platform_moby[0x20] = 2;
            *(int *)(platform_vars + 0xB0) = scale_game_frames(0x78);
        }
        break;
    }
    case PLATFORM_STATE_WAIT_AT_UPPER_HEIGHT:
        if (fast_dec_timer((int *)(platform_vars + 0xB0))) {
            allocate_voice_for_target_entry(0, 0, (int)platform_moby);
            platform_moby[0x20] = 3;
        }
        break;
    case PLATFORM_STATE_DESCENDING: {
        float lim;
        MOBY(platform_moby)->pos.z = MOBY(platform_moby)->pos.z - 1.5f / scale_time(45.0f);
        lim = *(float *)(platform_vars + 0xA8);
        if (MOBY(platform_moby)->pos.z <= lim) {
            MOBY(platform_moby)->pos.z = lim;
            platform_moby[0x20] = 4;
            *(int *)(platform_vars + 0xB0) = scale_game_frames(0x78);
        }
        break;
    }
    case PLATFORM_STATE_WAIT_AT_ORIGINAL_HEIGHT:
        if (fast_dec_timer((int *)(platform_vars + 0xB0))) {
            allocate_voice_for_target_entry(0, 0, (int)platform_moby);
            platform_moby[0x20] = 1;
        }
        break;
    }
}
