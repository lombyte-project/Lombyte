/* Moby class 0xB3: a flame that follows the moby that spawned it (the visibomb missile, the pyrocitor), flickering in size and spinning, and the function that spawns it. */

#include "types.h"
#include "rnc/overlay/point_light.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/refresh_moby_spatial_bounds_from_basis.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

typedef struct {
    char pad[0x10B0];
    int  i10B0;
} G_2bb400;

typedef struct {
    int            pad;
    int            ang;
    unsigned char *o;
    float          fC;
} V_2bb400;

extern void  clear_u64_value() __asm__("FUN_001f99f8");
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
void         refresh_moby_spatial_bounds_from_basis() __asm__("FUN_0020e098");
void         update_entity() __asm__("FUN_L00_00250df8");
extern void  offset_moby_class_point_pairs(void *, int, int) __asm__("FUN_L00_0024efb0");
extern float FUN_001fa580_2bb400(float, float) __asm__("FUN_001fa580");

u8 *spawn_linked_moby_from_basis(u8 *basis_source_moby) __asm__("FUN_L00_002bb360");

u8 *spawn_linked_moby_from_basis(u8 *basis_source_moby) {
    u8 *spawned_moby = create_moby(0xB3);
    if (spawned_moby) {
        u8 *spawned_moby_vars = ((u8 *)MOBY(spawned_moby)->pvars);
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        MOBY(spawned_moby)->flags = 0x204;
        spawned_moby[0x23] = 0x50;
        spawned_moby[0x20] = 0;
        *(u8 **)(spawned_moby_vars + 8) = basis_source_moby;
        clear_u64_value(spawned_moby + 0x40);
        *(OvlQuad *)&MOBY(spawned_moby)->rot_row0 = *(OvlQuad *)&MOBY(basis_source_moby)->rot_row0;
        *(OvlQuad *)&MOBY(spawned_moby)->rot_row1 = *(OvlQuad *)&MOBY(basis_source_moby)->rot_row1;
        *(OvlQuad *)&MOBY(spawned_moby)->rot_row2 = *(OvlQuad *)&MOBY(basis_source_moby)->rot_row2;
        refresh_moby_spatial_bounds_from_basis(spawned_moby);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

void update_linked_moby_scale_and_rotation(struct Moby *moby) __asm__("FUN_L00_002bb400");

void update_linked_moby_scale_and_rotation(struct Moby *moby) {
    V_2bb400      *moby_vars = ((V_2bb400 *)moby->pvars);
    unsigned char *o = moby_vars->o;
    int            old, unwrapped_angle, angle_delta;
    if (o[0x20] == 0 || (o[0x20] & 0xF0) == 0xF0) {
        mark_moby_for_removal(moby);
        return;
    }
    if (o != 0 && (moby->flags & 1) != (*(u16 *)(o + 0x34) & 1)) {
        moby->flags &= ~1;
        moby->flags |= *(u16 *)(moby_vars->o + 0x34) & 1;
    }
    if (hero.items[0].ready_frames < scale_game_frames(0x19)) {
        moby->flags |= 0x41;
    }
    old = moby_vars->ang;
    angle_delta = -0xC0;
    unwrapped_angle = old + angle_delta;
    moby_vars->ang = unwrapped_angle;
    if (unwrapped_angle > 0x1000) {
        angle_delta = -0x10C0;
        moby_vars->ang = old + angle_delta;
    } else if (unwrapped_angle < 0) {
        angle_delta = 0xF40;
        moby_vars->ang = old + angle_delta;
    }
    offset_moby_class_point_pairs(((void *)moby->pclass), 0, angle_delta);
    moby->scale = *(float *)(((char *)moby->pclass) + 0x24) * moby_vars->fC *
                  random_float_between(0.5f, 1.0f);
    moby->rot.y = FUN_001fa580_2bb400(moby->rot.y, normalize_angle(moby_vars->fC));
    refresh_moby_spatial_bounds_from_basis(moby);
}
