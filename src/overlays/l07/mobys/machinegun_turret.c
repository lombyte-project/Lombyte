/* Level 07 moby class 0x466: machinegun turret. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int   resolve_moby_hit_response(void *, void *, void *, int, int *, int, int, int) __asm__("FUN_00213928");
extern void  FUN_L00_00257470(void *, int, int);
extern void  trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void  FUN_00212f90(void *, int, int, int);
extern int   FUN_0022da68(int, int, void *);
extern float D_0015ED6C;

#include "rnc/game/item_state.h"

extern void FUN_L00_0024f7c8(void *, int, void *);
extern f32  D_0015ED6C;

/* Hit record returned by the hit lookup find_moby_hit. */
typedef struct {
    u8           pad0[0x10];
    Vec4f        pos;  /* 0x10 */
    struct Moby *moby; /* 0x20: moby that dealt the hit */
    u8           pad24[8];
    f32          kind; /* 0x2C */
} MobyHit;

extern float D_0015ED6C MACRO_ADDR;
extern void
     spawn_machinegun_turret_projectile(void *, void *, void *, int) __asm__("FUN_L07_0030b538");
void machinegun_turret_update(unsigned char *moby) __asm__("FUN_L07_0031a250");

/* Moby class 1126 */
void machinegun_turret_update(unsigned char *moby) {
    enum {
        TURRET_INITIALIZE = 0,
        TURRET_SWEEP_AND_FIRE = 1,
        TURRET_HIT_SPIN = 2,
        TURRET_LOWER_AND_REALIGN = 3,
        TURRET_RESTORE_HEIGHT = 4
    };
    char *turret_instance_vars;
    char *hit;
    char *collision;
    char *animation;
    char  position[16];
    float velocity[4];
    int   contact[4];
    float direction;
    float distance;
    if (moby == 0)
        return;
    turret_instance_vars = ((char *)MOBY(moby)->pvars);
    if (turret_instance_vars == 0)
        return;
    collision = turret_instance_vars + 0x20;
    animation = turret_instance_vars + 0x60;
    hit = find_moby_hit(moby, 0x230000, 0);
    resolve_moby_hit_response((void *)moby, (MobyHit *)hit, collision, 0, contact, 0, 0, 4);
    if ((u8)moby[0x20] < 2 && contact[0] > 1) {
        FUN_0022da68(1, 0, moby);
        FUN_L00_00257470(moby, 0, -1);
        MOBY(moby)->flags &= 0xefff;
        *(u8 *)(turret_instance_vars + 0x67) = 0xfa;
        *(s16 *)(turret_instance_vars + 0x26) = scale_game_frames(60);
        trooper_anim_update(moby, animation);
        *(s16 *)(turret_instance_vars + 0x7c) = scale_game_frames(60);
        if ((u8)moby[0x53] != 2)
            FUN_00212f90(moby, 2, 0, 5);
        moby[0x20] = 2;
    }
    MOBY(moby)->hit_slot = 0xff;
    trooper_anim_finish(moby, animation);
    switch ((u8)moby[0x20]) {
    case TURRET_INITIALIZE:
        if (*(int *)(turret_instance_vars + 0x70) != -1) {
            char *target =
                (char *)(*(int *)(turret_instance_vars + 0x70) * 0x100 + (int)level_mobys);
            *(float *)(turret_instance_vars + 0x80) =
                fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(target + 0x14) - MOBY(moby)->pos.y);
            if (*(float *)(turret_instance_vars + 0x80) < 0.0f)
                *(float *)(turret_instance_vars + 0x80) += 6.2831855f;
        }
        if (*(int *)(turret_instance_vars + 0x74) != -1) {
            char *target =
                (char *)(*(int *)(turret_instance_vars + 0x74) * 0x100 + (int)level_mobys);
            *(float *)(turret_instance_vars + 0x84) =
                fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(target + 0x14) - MOBY(moby)->pos.y);
            if (*(float *)(turret_instance_vars + 0x84) < 0.0f)
                *(float *)(turret_instance_vars + 0x84) += 6.2831855f;
            if (*(float *)(turret_instance_vars + 0x84) < *(float *)(turret_instance_vars + 0x80))
                *(float *)(turret_instance_vars + 0x84) += 6.2831855f;
        } else {
            *(float *)(turret_instance_vars + 0x84) =
                *(float *)(turret_instance_vars + 0x80) + 6.2831855f;
        }
        *(float *)(turret_instance_vars + 0x88) = 0.0f;
        *(float *)(turret_instance_vars + 0x8c) =
            *(float *)(turret_instance_vars + 0x84) - *(float *)(turret_instance_vars + 0x80);
        MOBY(moby)->rot.z = normalize_angle(*(float *)(turret_instance_vars + 0x80));
        *(float *)(turret_instance_vars + 0x90) = MOBY(moby)->pos.z;
        turret_instance_vars[0x29] = 0;
        moby[0xbc] = 0;
        *(u16 *)(turret_instance_vars + 0x9c) = *(u16 *)(turret_instance_vars + 0x94);
        if ((u8)moby[0x53] != 1)
            FUN_00212f90(moby, 1, 0, 0);
        moby[0x20] = 1;
        break;
    case TURRET_SWEEP_AND_FIRE:
        if ((u8)moby[0x52] != 1 && ((u8)moby[0x70] & 2) != 0 && (u8)moby[0x53] != 1)
            FUN_00212f90(moby, 1, 0, 1);
        if (*(s16 *)(turret_instance_vars + 0x7e) != 0) {
            *(float *)(turret_instance_vars + 0x88) -=
                *(float *)(turret_instance_vars + 0x78) * 0.017453292f * frame_time;
            if (*(float *)(turret_instance_vars + 0x88) < 0.0f) {
                *(float *)(turret_instance_vars + 0x88) = 0.0f;
                *(s16 *)(turret_instance_vars + 0x7e) = 0;
            }
        } else {
            *(float *)(turret_instance_vars + 0x88) +=
                *(float *)(turret_instance_vars + 0x78) * 0.017453292f * frame_time;
            if (*(float *)(turret_instance_vars + 0x8c) < *(float *)(turret_instance_vars + 0x88)) {
                *(float *)(turret_instance_vars + 0x88) = *(float *)(turret_instance_vars + 0x8c);
                *(s16 *)(turret_instance_vars + 0x7e) = 1;
            }
        }
        MOBY(moby)->rot.z = normalize_angle(*(float *)(turret_instance_vars + 0x80) +
                                            *(float *)(turret_instance_vars + 0x88));
        if (fast_dec_timer_s16(turret_instance_vars + 0x7c) == 0)
            return;
        if ((u8)moby[0xbc] == 0) {
            if (*(int *)(turret_instance_vars + 0x98) == 0 ||
                (*(u16 *)(turret_instance_vars + 0x9c) & 1)) {
                FUN_L00_0024f7c8(moby, 0, position);
                velocity[0] = fast_cos(MOBY(moby)->rot.z) * (D_0015ED6C * 15.0f);
                velocity[1] = fast_sin(MOBY(moby)->rot.z) * (D_0015ED6C * 15.0f);
                velocity[2] = 0.0f;
                FUN_0022da68(0, 0, moby);
                spawn_machinegun_turret_projectile(
                    position, velocity, moby, (u8)turret_instance_vars[0x9e]);
            }
            if (*(int *)(turret_instance_vars + 0x98) != 0) {
                if (fast_dec_timer_s16(turret_instance_vars + 0x9c) != 0) {
                    moby[0xbc] = 1;
                    *(u16 *)(turret_instance_vars + 0x9c) = *(u16 *)(turret_instance_vars + 0x98);
                }
            } else {
                *(u16 *)(turret_instance_vars + 0x9c) ^= 1;
            }
        } else if (fast_dec_timer_s16(turret_instance_vars + 0x9c) != 0) {
            moby[0xbc] = 0;
            *(u16 *)(turret_instance_vars + 0x9c) = *(u16 *)(turret_instance_vars + 0x94);
        }
        *(s16 *)(turret_instance_vars + 0x7c) = scale_game_frames(6);
        distance = vector_distance_xy(moby + 0x10, D_0013F3D0);
        if (!(55.0f < distance)) {
            if (!(35.0f < distance) || (u8)moby[0x31] != 0)
                return;
        }
        if (distance > 80.0f)
            *(s16 *)(turret_instance_vars + 0x7c) *= 3;
        else if (distance > 120.0f) {
            int timer = *(s16 *)(turret_instance_vars + 0x7c);
            *(s16 *)(turret_instance_vars + 0x7c) = timer << 2;
        } else {
            int timer = *(s16 *)(turret_instance_vars + 0x7c);
            *(s16 *)(turret_instance_vars + 0x7c) = timer << 1;
        }
        break;
    case TURRET_HIT_SPIN:
        direction = frame_time;
        if ((u8)moby[0x52] != 4 && ((u8)moby[0x70] & 2) && (u8)moby[0x53] != 4) {
            FUN_00212f90(moby, 4, 0, 1);
            direction = D_0015ED6C;
        }
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, direction * 12.566371f);
        if (fast_dec_timer_s16(turret_instance_vars + 0x7c) == 0)
            return;
        *(s16 *)(turret_instance_vars + 0x7c) = scale_game_frames(180);
        moby[0x20] = 3;
        break;
    case TURRET_LOWER_AND_REALIGN:
        if ((u8)moby[0x52] != 4 && ((u8)moby[0x70] & 2) && (u8)moby[0x53] != 4)
            FUN_00212f90(moby, 4, 0, 0);
        if (*(float *)(turret_instance_vars + 0x90) - 0.7f < MOBY(moby)->pos.z)
            MOBY(moby)->pos.z -= frame_time * 5.0f;
        else
            MOBY(moby)->pos.z = *(float *)(turret_instance_vars + 0x90) - 0.7f;
        direction = normalize_angle(*(float *)(turret_instance_vars + 0x80) +
                                    *(float *)(turret_instance_vars + 0x88));
        distance = fast_difference_between_rotations(MOBY(moby)->rot.z, direction);
        if (D_0015ED6C * 12.566371f < distance)
            MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, D_0015ED6C * 12.566371f);
        else
            MOBY(moby)->rot.z = normalize_angle(*(float *)(turret_instance_vars + 0x80) +
                                                *(float *)(turret_instance_vars + 0x88));
        if (fast_dec_timer_s16(turret_instance_vars + 0x7c) != 0) {
            FUN_0022da68(2, 0, moby);
            if ((u8)moby[0x53] != 3)
                FUN_00212f90(moby, 3, 0, 5);
            moby[0x20] = 4;
        }
        break;
    case TURRET_RESTORE_HEIGHT:
        if (MOBY(moby)->pos.z < *(float *)(turret_instance_vars + 0x90)) {
            MOBY(moby)->pos.z += D_0015ED6C * 5.0f;
            return;
        }
        MOBY(moby)->pos.z = *(float *)(turret_instance_vars + 0x90);
        MOBY(moby)->flags |= 0x1000;
        if ((u8)moby[0x53] != 1)
            FUN_00212f90(moby, 1, 0, 5);
        moby[0x20] = 1;
        break;
    case 5:
        break;
    }
}
