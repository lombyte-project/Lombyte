/* Shared moby class spawn_point: respawns its pickup unless it was collected. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/overlay/level_labels.h"

/* pvars of the respawning pickup spawn_point_update updates */
typedef struct {
    s32 volume; /* 0x00: clip volume that arms it */
    u8  pad4[4];
    u32 flags; /* 0x08: 2/4: needs a D_001413F4 mode; 8: disabled; 0x10: needs a D_001413DC state */
    s32 armed; /* 0x0C */
    s32 rearm; /* 0x10 */
    s32 in_volume;  /* 0x14 */
    s32 spawn_slot; /* 0x18: slot in the level's D_0014C050 row, -1 for none */
} PickupVars;

/* Per-level pickup state; collected[] is indexed by the moby's unkB2 id. */
typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L01LevelState;

extern s32            D_0014C190[][64];
extern L01LevelState  D_L01_001BAA50;
extern u8             D_0014C050[];
extern u8 D_001413F4  NOT_SDA;
extern u32 D_001413DC NOT_SDA;
extern unsigned char  D_0014C050[];

void spawn_point_update(struct Moby *moby) __asm__("FUN_L01_00300220");

void spawn_point_update(struct Moby *moby) {
    PickupVars *vars = (PickupVars *)moby->pvars;
    s32         armed = 0;
    Vec4f       pos;
    u32         state;

    if (vars->flags & 8) {
        return;
    }
    if (moby->command == 0) {
        moby->command = 1;
        vars->in_volume = 0;
        return;
    }
    if (vars->spawn_slot != -1 && D_0014C050[vars->spawn_slot + current_level_index * 16] != 0xFF) {
        return;
    }
    if ((vars->flags & 2) && D_001413F4 != 1) {
        return;
    }
    if ((vars->flags & 4) && D_001413F4 != 3 && D_001413F4 != 0) {
        return;
    }
    if (vars->flags & 0x10) {
        state = D_001413DC;
        if (state > 1 && state != 9) {
            return;
        }
    }
    if (is_point_inside_clip_volume(&hero.motion.pos, vars->volume)) {
        if (vars->in_volume == 0) {
            armed = 1;
        }
        vars->in_volume = 1;
    } else {
        vars->in_volume = 0;
    }
    if (armed || l01_save_state.cleared[(s16)moby->save_id] != 0 ||
        ((D_0014C190[current_level_index][(s16)moby->save_id >> 5] >> (moby->save_id & 0x1F)) &
         1)) {
        if (vars->armed == 0 || vars->rearm != 0) {
            armed = 1;
            vars->armed = armed;
            jet_clear_persistent(moby->mission);
            qcopy(&pos, &moby->pos);
            pos.z = probe_ground_height(&pos, 0, 0.5f);
            if (absolute_float(moby->pos.z - pos.z) > 2.0f) {
                pos.z = moby->pos.z;
            }
            jet_reset_camera(&pos, &moby->rot);
            D_L01_001BAA50.collected[(s16)moby->save_id] = 0;
            l01_save_state.cleared[(s16)moby->save_id] = 0;
            D_0014C190[current_level_index][(s16)moby->save_id >> 5] &=
                ~(armed << (moby->save_id & 0x1F));
            moby_killed[(s16)moby->save_id >> 5] &= ~(armed << (moby->save_id & 0x1F));
        }
    } else {
        vars->armed = 0;
    }
    if (moby->state == 0 && D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
        moby->state = 1;
    }
}
