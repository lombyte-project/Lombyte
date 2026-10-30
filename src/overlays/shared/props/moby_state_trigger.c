/* Shared moby class 0x102: a spatial trigger that sets the state of the mobys in its lists while the hero is inside. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/level_path.h"
#include "rnc/game/item_state.h"

/* pvars of the trigger moby run by FUN_L05_002f5200 */
typedef struct {
    s32 path;         /* -1, or an index into D_L05_001B0930 */
    f32 radius;       /* > 0: trigger when the target is this close */
    s32 volumes[6];   /* -1, or a clip volume is_point_inside_clip_volume tests */
    s32 on_lists[12]; /* -1, or a list run by FUN_L05_002f5190 */
    s32 on_mobys[11]; /* -1, or a moby passed to FUN_L05_002f4f60 */
    u8  pad7C[2];
    s8  use_alt_target; /* nonzero: test D_L05_001671C0 instead of the hero */
} TriggerVars;

extern s32 moby_pool_4f60 __asm__("D_L05_0015FFD8");

extern int *D_L05_001ABCC0[];

extern Vec4 D_L05_001671C0_2f5200 __asm__("D_L05_001671C0");
extern s32  point_in_path_xy(void *, void *, s32) __asm__("FUN_L00_00259740");

void apply_spatial_trigger_to_moby(int index,
                                   int trigger_arg,
                                   int inside) __asm__("FUN_L05_002f4f60");

void apply_spatial_trigger_to_moby(int index, int trigger_arg, int inside) {
    int          moby = moby_pool_4f60 + index * 0x100;
    char        *trigger = (char *)trigger_arg;
    short        kind;
    unsigned int flags;

    if (moby == 0) {
        return;
    }
    if (inside) {
        if (trigger[0x7c] > 0) {
            MOBY(moby)->flags &= 0xfffd;
        } else if (trigger[0x7c] < 0) {
            MOBY(moby)->flags |= 2;
        }
        if (trigger[0x7d] > 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
            kind != 0x4ef && kind != 0x571) {
            MOBY(moby)->visible = 1;
            MOBY(moby)->flags &= 0xfffe;
        } else if (trigger[0x7d] < 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
                   kind != 0x4ef && kind != 0x571) {
            MOBY(moby)->visible = 0;
            flags = MOBY(moby)->flags | 1;
            MOBY(moby)->flags = flags;
        }
        if (trigger[0x7f] > 0 && *(int *)(moby + 0x24) != 0) {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        } else if (trigger[0x7f] < 0 && *(int *)(moby + 0x24) != 0) {
            MOBY(moby)->collision = 0;
        }
    } else {
        if (trigger[0x7c] > 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
            kind != 0x4ef) {
            MOBY(moby)->flags |= 2;
        } else if (trigger[0x7c] < 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
                   kind != 0x4ef) {
            MOBY(moby)->flags &= 0xfffd;
        }
        if (trigger[0x7d] > 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
            kind != 0x4ef && kind != 0x571) {
            MOBY(moby)->visible = 0;
            flags = MOBY(moby)->flags | 1;
            MOBY(moby)->flags = flags;
        } else if (trigger[0x7d] < 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
                   kind != 0x4ef && kind != 0x571) {
            MOBY(moby)->visible = 1;
            MOBY(moby)->flags &= 0xfffe;
        }
        if (trigger[0x7f] > 0) {
            MOBY(moby)->collision = 0;
        } else if (trigger[0x7f] < 0) {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        }
    }
}

/* Runs func_L05_002F62E8 for each entry of list i until an entry without the top bit set. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_002F6518), where it is exact; names translated to the US level program. */

void apply_spatial_trigger_to_moby_list(int unused_trigger_moby,
                                        int trigger_vars_arg,
                                        int i,
                                        int inside) __asm__("FUN_L05_002f5190");

void apply_spatial_trigger_to_moby_list(int unused_trigger_moby,
                                        int trigger_vars_arg,
                                        int i,

                                        int inside) {
    unsigned short *moby_index_entry = (unsigned short *)D_L05_001ABCC0[i];
    if (moby_index_entry != 0) {
        do {
            apply_spatial_trigger_to_moby(*moby_index_entry & 0x7FFF, trigger_vars_arg, inside);
        } while (*(short *)moby_index_entry++ >= 0);
    }
}

void update_moby_spatial_trigger(struct Moby *trigger_moby) __asm__("FUN_L05_002f5200");

void update_moby_spatial_trigger(struct Moby *trigger_moby) {
    TriggerVars *trigger_vars = (TriggerVars *)trigger_moby->pvars;
    Vec4         target;
    s32          inside;
    s32          i;
    s32         *path;

    trigger_moby->update_dist = 0xFF;
    inside = 0;
    if (trigger_vars->use_alt_target != 0) {
        qcopy(&target, &D_L05_001671C0_2f5200);
    } else {
        qcopy(&target, &hero.motion.pos);
    }
    if (0.0f < trigger_vars->radius &&
        fast_vec_dist(&trigger_moby->pos, &target) < trigger_vars->radius) {
        inside = 1;
    }
    if (!inside && trigger_vars->path != -1) {
        path = ((s32 *)l05_paths[trigger_vars->path]);
        inside = point_in_path_xy(&target, path + 4, path[0]) != 0;
    }
    if (!inside) {
        for (i = 0; i < 6; i++) {
            if (trigger_vars->volumes[i] != -1 &&
                is_point_inside_clip_volume(&target, trigger_vars->volumes[i])) {
                inside = 1;
                break;
            }
        }
    }
    if ((inside && trigger_moby->command == 0) || (!inside && trigger_moby->command != 0) ||
        trigger_moby->state == 0) {
        if (inside) {
            trigger_moby->command = 1;
        } else {
            trigger_moby->command = 0;
        }
        trigger_moby->state = trigger_moby->command + 1;
        for (i = 0; i < 12; i++) {
            if (trigger_vars->on_lists[i] != -1) {
                apply_spatial_trigger_to_moby_list(
                    trigger_moby, trigger_vars, trigger_vars->on_lists[i], inside);
            }
        }
        for (i = 0; i < 11; i++) {
            if (trigger_vars->on_mobys[i] != -1) {
                apply_spatial_trigger_to_moby(trigger_vars->on_mobys[i], trigger_vars, inside);
            }
        }
    }
}

int *D_L09_001AB9C0 NOT_SDA = 0;
