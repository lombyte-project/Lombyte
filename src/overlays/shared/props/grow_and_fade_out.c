/* Shared moby class 0x307: a burst that grows and fades out, and the function that spawns it. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"

typedef struct Moby Moby;

/* Ported from rac1-decomp (nonmatching/shared/func_L00_002CF808.c: func_L00_002CF808). */
typedef int OvlQuad_2C9820 __attribute__((mode(TI)));

typedef struct {
    s32 v[7];
} T_2d0538;

typedef struct V2d1428 {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V2d1428;

typedef struct {
    float x, y, z, w;
} V;

f32         random_angle_radians(void) __asm__("FUN_00213308");
extern f32  random_angle_radians(void) __asm__("FUN_00213308");
extern void mark_moby_for_removal() __asm__("FUN_0020c828");

unsigned char *create_moby(int) __asm__("FUN_0020c4f8");
void           copy_hero_moby_field_38(unsigned char *) __asm__("FUN_L00_0025d1b8");
void           set_moby_color(unsigned char *, int, int, int) __asm__("FUN_L00_002502f0");
float          random_angle_radians(void) __asm__("FUN_00213308");
void           update_entity(unsigned char *) __asm__("FUN_L00_00250df8");

void mark_moby_for_removal() __asm__("FUN_0020c828");

unsigned char *spawn_burst_moby(u128 *v, float s) __asm__("FUN_L00_002d7e90");

unsigned char *spawn_burst_moby(u128 *spawn_position, float scale_multiplier) {
    unsigned char *spawned_moby = create_moby(0x307);
    if (spawned_moby) {
        copy_hero_moby_field_38(spawned_moby);
        set_moby_color(spawned_moby, 0x28, 0x28, 0x46);
        spawned_moby[0x30] = 0xff;
        MOBY(spawned_moby)->draw_dist = 0x40;
        MOBY(spawned_moby)->scale *= scale_multiplier;
        qcopy(spawned_moby + 0x10, spawn_position);
        MOBY(spawned_moby)->rot.z = random_angle_radians();
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

/* Grows the float at 0x2C by 2.5% and lowers byte 0x23 by 3; removes the moby below 4. */

void grow_and_fade_out_update(u8 *moby) __asm__("FUN_L00_002d7f38");

void grow_and_fade_out_update(u8 *moby) {
    MOBY(moby)->scale *= 1.025f;
    if ((u8)(moby[0x23] -= 3) < 4)
        mark_moby_for_removal();
}
