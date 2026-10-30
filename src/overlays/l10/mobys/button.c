/* Level 10 moby class button: a floor plate that opens its door while something stands on it. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/level_labels.h"

extern struct Moby *D_L10_0015FFD8;
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");

/* Pvars of a floor plate that opens a door while something stands on it. */
typedef struct {
    s32 door; /* 0x00: level moby index of the door, -1 for none */
    s32 link; /* 0x04: level moby index of a plate it holds down, -1 for none */
    u8  pad8[4];
    s32 held;        /* 0x0C: set by a linked plate */
    s32 sound_timer; /* 0x10 */
} FloorPlateVars;

/* Result of the last probe_ground_height: the moby that was hit at 0x18. */
typedef struct {
    u8           pad0[0x18];
    struct Moby *moby; /* 0x18 */
} GroundHit;

extern struct Moby *nearby_mobys[] __asm__("D_L10_00178380");
extern u8           D_0014C050[][16];
extern void         FUN_L00_002502f0_u(void *, int, int, int) __asm__("FUN_L00_002502f0");

/* Floor plate: lit and opening its door while a moby (other than class 0x3EF) rests on it, dark and closing it otherwise. */
void button_update(struct Moby *m) __asm__("FUN_L10_002d92b8");

void button_update(struct Moby *moby) {
    FloorPlateVars *plate_vars = (FloorPlateVars *)moby->pvars;
    s32             pressed = 0;
    struct Moby    *door;
    struct Moby    *other;
    s32             i;
    f32             ground_height;
    s32             nearby_moby_count;

    if (plate_vars->door == -1) {
        FUN_L00_002502f0_u(moby, 8, 8, 8);
        return;
    }
    door = &D_L10_0015FFD8[plate_vars->door];
    nearby_moby_count = collect_mobies_in_sphere(&moby->pos, 1.0f, 0, moby, NULL);
    if (nearby_moby_count != 0) {
        for (i = 0; i < nearby_moby_count; i++) {
            other = nearby_mobys[i];
            ground_height = probe_ground_height(&other->pos, 0, 0.5f);
            if (((GroundHit *)ground_hit)->moby == moby &&
                absolute_float(ground_height - nearby_mobys[i]->pos.z) < 0.1f) {
                if (nearby_mobys[i]->oclass != 0x3EF) {
                    pressed = 1;
                }
            }
        }
    }
    fast_dec_timer(&plate_vars->sound_timer);
    if (D_0014C050[current_level_index][moby->mission] == 0xFF) {
        pressed = 1;
    }
    if (pressed || plate_vars->held != 0) {
        if (pressed && plate_vars->link != -1) {
            ((FloorPlateVars *)D_L10_0015FFD8[plate_vars->link].pvars)->held = 1;
        } else {
            plate_vars->held = 0;
        }
        if (door->state == 1 || door->state == 2) {
            door->state = 3;
            if (plate_vars->sound_timer == 0) {
                allocate_voice_for_target_entry(0, 0, door);
                allocate_voice_for_target_entry(0, 0, moby);
                plate_vars->sound_timer = scale_game_frames(30);
            }
        }
        FUN_L00_002502f0_u(moby, 0x7F, 0x7F, 0x7F);
    } else {
        if (door->state == 4) {
            door->state = 1;
            if (plate_vars->sound_timer == 0) {
                allocate_voice_for_target_entry(1, 0, door);
                allocate_voice_for_target_entry(0, 0, moby);
                plate_vars->sound_timer = scale_game_frames(30);
            }
        }
        FUN_L00_002502f0_u(moby, 8, 8, 8);
    }
}
