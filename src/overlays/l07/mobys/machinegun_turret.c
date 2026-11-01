/* Level 07 moby class 0x466: machinegun turret. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/l07/mobys.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"

extern int resolve_moby_hit_response(void *, void *, void *, int, int *, int, int, int) __asm__("FUN_00213928");
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
void                    machinegun_turret_update(unsigned char *moby) __asm__("FUN_L07_0031a250");

typedef struct MachinegunTurretVars {
    u8  pad_0[0x20];
    u8  unk20;
    u8  pad_21[0x5];
    s16 unk26;
    u8  pad_28[0x1];
    u8  unk29;
    u8  pad_2A[0x36];
    u8  anim_state;
    u8  pad_61[0x6];
    u8  unk67;
    u8  pad_68[0x8];
    s32 unk70;
    s32 unk74;
    f32 turn_speed_degrees;
    s16 unk7C;
    s16 unk7E;
    f32 unk80[4];
    f32 unk90;
    u16 unk94;
    u8  pad_96[0x2];
    s32 unk98;
    u16 unk9C;
    u8  unk9E;
} MachinegunTurretVars;

/* Moby class 1126 */
void machinegun_turret_update(unsigned char *moby) {
    enum {
        TURRET_INITIALIZE = 0,
        TURRET_SWEEP_AND_FIRE = 1,
        TURRET_HIT_SPIN = 2,
        TURRET_LOWER_AND_REALIGN = 3,
        TURRET_RESTORE_HEIGHT = 4
    };
    MachinegunTurretVars *turret_instance_vars;
    char                 *hit;
    char                 *collision;
    char                 *animation;
    char                  position[16];
    float                 velocity[4];
    int                   contact[4];
    float                 direction;
    float                 distance;
    if (moby == 0)
        return;
    turret_instance_vars = ((char *)MOBY(moby)->pvars);
    if (turret_instance_vars == 0)
        return;
    collision = &turret_instance_vars->unk20;
    animation = &turret_instance_vars->anim_state;
    hit = find_moby_hit(moby, 0x230000, 0);
    resolve_moby_hit_response((void *)moby, (MobyHit *)hit, collision, 0, contact, 0, 0, 4);
    if ((u8)MOBY(moby)->state < 2 && contact[0] > 1) {
        FUN_0022da68(1, 0, moby);
        FUN_L00_00257470(moby, 0, -1);
        MOBY(moby)->flags &= 0xefff;
        turret_instance_vars->unk67 = 0xfa;
        turret_instance_vars->unk26 = scale_game_frames(60);
        trooper_anim_update(moby, animation);
        turret_instance_vars->unk7C = scale_game_frames(60);
        if ((u8)MOBY(moby)->prev_seq != 2)
            FUN_00212f90(moby, 2, 0, 5);
        MOBY(moby)->state = 2;
    }
    MOBY(moby)->hit_slot = 0xff;
    trooper_anim_finish(moby, animation);
    switch ((u8)MOBY(moby)->state) {
    case TURRET_INITIALIZE:
        if (turret_instance_vars->unk70 != -1) {
            char *target = (char *)(turret_instance_vars->unk70 * 0x100 + (int)level_mobys);
            turret_instance_vars->unk80[0] =
                fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(target + 0x14) - MOBY(moby)->pos.y);
            if (turret_instance_vars->unk80[0] < 0.0f)
                turret_instance_vars->unk80[0] += 6.2831855f;
        }
        if (turret_instance_vars->unk74 != -1) {
            char *target = (char *)(turret_instance_vars->unk74 * 0x100 + (int)level_mobys);
            turret_instance_vars->unk80[1] =
                fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(target + 0x14) - MOBY(moby)->pos.y);
            if (turret_instance_vars->unk80[1] < 0.0f)
                turret_instance_vars->unk80[1] += 6.2831855f;
            if (turret_instance_vars->unk80[1] < turret_instance_vars->unk80[0])
                turret_instance_vars->unk80[1] += 6.2831855f;
        } else {
            turret_instance_vars->unk80[1] = turret_instance_vars->unk80[0] + 6.2831855f;
        }
        turret_instance_vars->unk80[2] = 0.0f;
        turret_instance_vars->unk80[3] =
            turret_instance_vars->unk80[1] - turret_instance_vars->unk80[0];
        MOBY(moby)->rot.z = normalize_angle(turret_instance_vars->unk80[0]);
        turret_instance_vars->unk90 = MOBY(moby)->pos.z;
        turret_instance_vars->unk29 = 0;
        MOBY(moby)->command = 0;
        turret_instance_vars->unk9C = turret_instance_vars->unk94;
        if ((u8)MOBY(moby)->prev_seq != 1)
            FUN_00212f90(moby, 1, 0, 0);
        MOBY(moby)->state = 1;
        break;
    case TURRET_SWEEP_AND_FIRE:
        if ((u8)MOBY(moby)->seq != 1 && ((u8)MOBY(moby)->anim_flags & 2) != 0 &&
            (u8)MOBY(moby)->prev_seq != 1)
            FUN_00212f90(moby, 1, 0, 1);
        if (turret_instance_vars->unk7E != 0) {
            turret_instance_vars->unk80[2] -=
                turret_instance_vars->turn_speed_degrees * 0.017453292f * frame_time;
            if (turret_instance_vars->unk80[2] < 0.0f) {
                turret_instance_vars->unk80[2] = 0.0f;
                turret_instance_vars->unk7E = 0;
            }
        } else {
            turret_instance_vars->unk80[2] +=
                turret_instance_vars->turn_speed_degrees * 0.017453292f * frame_time;
            if (turret_instance_vars->unk80[3] < turret_instance_vars->unk80[2]) {
                turret_instance_vars->unk80[2] = turret_instance_vars->unk80[3];
                turret_instance_vars->unk7E = 1;
            }
        }
        MOBY(moby)->rot.z =
            normalize_angle(turret_instance_vars->unk80[0] + turret_instance_vars->unk80[2]);
        if (fast_dec_timer_s16(&turret_instance_vars->unk7C) == 0)
            return;
        if ((u8)MOBY(moby)->command == 0) {
            if (turret_instance_vars->unk98 == 0 || (turret_instance_vars->unk9C & 1)) {
                FUN_L00_0024f7c8(moby, 0, position);
                velocity[0] = fast_cos(MOBY(moby)->rot.z) * (D_0015ED6C * 15.0f);
                velocity[1] = fast_sin(MOBY(moby)->rot.z) * (D_0015ED6C * 15.0f);
                velocity[2] = 0.0f;
                FUN_0022da68(0, 0, moby);
                spawn_machinegun_turret_projectile(
                    position, velocity, moby, (u8)turret_instance_vars->unk9E);
            }
            if (turret_instance_vars->unk98 != 0) {
                if (fast_dec_timer_s16(&turret_instance_vars->unk9C) != 0) {
                    MOBY(moby)->command = 1;
                    turret_instance_vars->unk9C = *(u16 *)&turret_instance_vars->unk98;
                }
            } else {
                turret_instance_vars->unk9C ^= 1;
            }
        } else if (fast_dec_timer_s16(&turret_instance_vars->unk9C) != 0) {
            MOBY(moby)->command = 0;
            turret_instance_vars->unk9C = turret_instance_vars->unk94;
        }
        turret_instance_vars->unk7C = scale_game_frames(6);
        distance = vector_distance_xy(moby + 0x10, D_0013F3D0);
        if (!(55.0f < distance)) {
            if (!(35.0f < distance) || (u8)MOBY(moby)->visible != 0)
                return;
        }
        if (distance > 80.0f)
            turret_instance_vars->unk7C *= 3;
        else if (distance > 120.0f) {
            int timer = turret_instance_vars->unk7C;
            turret_instance_vars->unk7C = timer << 2;
        } else {
            int timer = turret_instance_vars->unk7C;
            turret_instance_vars->unk7C = timer << 1;
        }
        break;
    case TURRET_HIT_SPIN:
        direction = frame_time;
        if ((u8)MOBY(moby)->seq != 4 && ((u8)MOBY(moby)->anim_flags & 2) &&
            (u8)MOBY(moby)->prev_seq != 4) {
            FUN_00212f90(moby, 4, 0, 1);
            direction = D_0015ED6C;
        }
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, direction * 12.566371f);
        if (fast_dec_timer_s16(&turret_instance_vars->unk7C) == 0)
            return;
        turret_instance_vars->unk7C = scale_game_frames(180);
        MOBY(moby)->state = 3;
        break;
    case TURRET_LOWER_AND_REALIGN:
        if ((u8)MOBY(moby)->seq != 4 && ((u8)MOBY(moby)->anim_flags & 2) &&
            (u8)MOBY(moby)->prev_seq != 4)
            FUN_00212f90(moby, 4, 0, 0);
        if (turret_instance_vars->unk90 - 0.7f < MOBY(moby)->pos.z)
            MOBY(moby)->pos.z -= frame_time * 5.0f;
        else
            MOBY(moby)->pos.z = turret_instance_vars->unk90 - 0.7f;
        direction =
            normalize_angle(turret_instance_vars->unk80[0] + turret_instance_vars->unk80[2]);
        distance = fast_difference_between_rotations(MOBY(moby)->rot.z, direction);
        if (D_0015ED6C * 12.566371f < distance)
            MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, D_0015ED6C * 12.566371f);
        else
            MOBY(moby)->rot.z =
                normalize_angle(turret_instance_vars->unk80[0] + turret_instance_vars->unk80[2]);
        if (fast_dec_timer_s16(&turret_instance_vars->unk7C) != 0) {
            FUN_0022da68(2, 0, moby);
            if ((u8)MOBY(moby)->prev_seq != 3)
                FUN_00212f90(moby, 3, 0, 5);
            MOBY(moby)->state = 4;
        }
        break;
    case TURRET_RESTORE_HEIGHT:
        if (MOBY(moby)->pos.z < turret_instance_vars->unk90) {
            MOBY(moby)->pos.z += D_0015ED6C * 5.0f;
            return;
        }
        MOBY(moby)->pos.z = turret_instance_vars->unk90;
        MOBY(moby)->flags |= 0x1000;
        if ((u8)MOBY(moby)->prev_seq != 1)
            FUN_00212f90(moby, 1, 0, 5);
        MOBY(moby)->state = 1;
        break;
    case 5:
        break;
    }
}
