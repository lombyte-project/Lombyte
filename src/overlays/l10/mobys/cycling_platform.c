/* Level 10 moby class cycling_platform. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/quad.h"

extern int   debug_print() __asm__("FUN_001e93b0");
extern float scale_time(float) __asm__("FUN_001f96b0");

char cycling_platform_missing_pvars_message[79] __asm__("D_L10_001DC510") =
    "Moby Class %d Instance %d has no pVars!  May be in the art file.\n\tMobyDeleted\n";

void       cycling_platform_update(char *platform_moby) __asm__("FUN_L10_00295a38");
extern f32 scale_time(f32) __asm__("FUN_001f96b0");

typedef struct CyclingPlatformVars {
    u8  pad_0[0x20];
    s32 health;
    s16 max_health;
    u8  pad_26[0x2];
    s8  hit_weight;
    u8  pad_29[0x15];
    s16 target_flags; /* 0x3E: MobyTargetInfo.flags; target info at pvars+0x20 */
    u8  pad_40[0x60];
    f32 home_position[4];
    s32 timer_B0;
} CyclingPlatformVars;

/* Moby class 1033 */
void cycling_platform_update(char *platform_moby) {
    enum {
        PLATFORM_STATE_INITIALIZE = 0,
        PLATFORM_STATE_RISING = 1,
        PLATFORM_STATE_WAIT_AT_UPPER_HEIGHT = 2,
        PLATFORM_STATE_DESCENDING = 3,
        PLATFORM_STATE_WAIT_AT_ORIGINAL_HEIGHT = 4
    };
    CyclingPlatformVars *platform_vars = (CyclingPlatformVars *)MOBY(platform_moby)->pvars;
    if (platform_vars == 0) {
        debug_print(cycling_platform_missing_pvars_message,
                    MOBY(platform_moby)->oclass,
                    ((short)MOBY(platform_moby)->save_id));
        mark_moby_for_removal(platform_moby);
        return;
    }
    switch (MOBY(platform_moby)->state) {
    case PLATFORM_STATE_INITIALIZE:
        platform_vars->hit_weight = 4;
        platform_vars->target_flags = 5;
        platform_vars->health = 0;
        platform_vars->max_health = 0;
        *(OvlQuad *)&platform_vars->home_position[0] = *(OvlQuad *)&MOBY(platform_moby)->pos;
        platform_moby[0x20] = 1;
        break;
    case PLATFORM_STATE_RISING: {
        float lim;
        MOBY(platform_moby)->pos.z = MOBY(platform_moby)->pos.z + 1.5f / scale_time(45.0f);
        lim = platform_vars->home_position[2] + 1.5f;
        if (lim <= MOBY(platform_moby)->pos.z) {
            MOBY(platform_moby)->pos.z = lim;
            platform_moby[0x20] = 2;
            platform_vars->timer_B0 = scale_game_frames(0x78);
        }
        break;
    }
    case PLATFORM_STATE_WAIT_AT_UPPER_HEIGHT:
        if (fast_dec_timer(&platform_vars->timer_B0)) {
            allocate_voice_for_target_entry(0, 0, (int)platform_moby);
            platform_moby[0x20] = 3;
        }
        break;
    case PLATFORM_STATE_DESCENDING: {
        float lim;
        MOBY(platform_moby)->pos.z = MOBY(platform_moby)->pos.z - 1.5f / scale_time(45.0f);
        lim = platform_vars->home_position[2];
        if (MOBY(platform_moby)->pos.z <= lim) {
            MOBY(platform_moby)->pos.z = lim;
            platform_moby[0x20] = 4;
            platform_vars->timer_B0 = scale_game_frames(0x78);
        }
        break;
    }
    case PLATFORM_STATE_WAIT_AT_ORIGINAL_HEIGHT:
        if (fast_dec_timer(&platform_vars->timer_B0)) {
            allocate_voice_for_target_entry(0, 0, (int)platform_moby);
            platform_moby[0x20] = 1;
        }
        break;
    }
}
