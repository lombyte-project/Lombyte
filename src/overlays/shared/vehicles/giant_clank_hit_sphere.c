/* Giant Clank's hit sphere (class 0x593, Quartu and Veldin 2): grows and fades out around the hero, and the function that spawns it. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "qcopy.h"
#include "rnc/hero/hero.h"
#include "rnc/math/asm_helpers.h"

/* Creates a moby at a position with a scale, colour and a few data fields. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AAC50), where it is exact; names translated to the US level program. */

void *spawn_giant_clank_hit_sphere(int   unused,
                                   void *spawn_position,
                                   int   initial_paired_value,
                                   float scale,
                                   float initial_private_scalar,
                                   float b) __asm__("FUN_L00_002a99b0");

void *spawn_giant_clank_hit_sphere(int   unused,
                                   void *spawn_position,
                                   int   initial_paired_value,
                                   float scale,
                                   float initial_private_scalar,

                                   float initial_private_value) {
    char *spawned_moby = create_moby(0x593);
    if (spawned_moby != 0) {
        char *moby_vars;
        float doubled_scale;
        qcopy(spawned_moby + 0x10, spawn_position);
        set_moby_color(spawned_moby, 0x80, 0x80, 0x70);
        moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        doubled_scale = scale + scale;
        *(float *)moby_vars = initial_private_scalar;
        MOBY(spawned_moby)->scale = doubled_scale;
        MOBY(spawned_moby)->scale =
            doubled_scale * *(float *)(((char *)MOBY(spawned_moby)->pclass) + 0x24);
        *(int *)(moby_vars + 4) = initial_paired_value;
        *(int *)(moby_vars + 8) = initial_paired_value;
        *(float *)(moby_vars + 0xC) = initial_private_value;
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        MOBY(spawned_moby)->visible = 1;
        MOBY(spawned_moby)->state = 0;
    }
    return spawned_moby;
}

extern int scale_game_frames(int);

extern void play_entity_sound(float, char *, char *, float, float, int, int, int, int) __asm__("FUN_L00_002598b0");

/* update: scale and fade a moby, spawn an effect, delete it when done */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029FCB0), where it is exact; names translated to the US level program. */

void update_giant_clank_hit_sphere(struct Moby *moby) __asm__("FUN_L15_0029ead0");

void update_giant_clank_hit_sphere(struct Moby *moby) {
    char        *moby_class = moby->pclass;
    char        *state = (char *)moby->pvars;
    float        half_relative_scale = moby->scale / *(float *)(moby_class + 0x24) * 0.5f;
    char        *pos;
    struct Hero *globals;
    half_relative_scale = half_relative_scale + *(float *)state;
    moby->scale = half_relative_scale + half_relative_scale;
    moby->scale = moby->scale * *(float *)(moby_class + 0x24);
    if (*(int *)(state + 8) < scale_game_frames(0x28))
        moby->alpha = *(int *)(state + 8) * 127 / scale_game_frames(0x28);
    globals = &hero;
    pos = &moby->pos;
    play_entity_sound(half_relative_scale,
                      ((char *)globals->moby),
                      pos,
                      (float)truncate_float_to_s32(*(float *)(state + 0xC)),
                      1.0f,
                      0x30000,
                      0,
                      1,
                      0);
    if (fast_dec_timer((int *)(state + 8)))
        mark_moby_for_removal(moby);
}
