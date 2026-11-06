/* Level 07 moby class 0x466: machinegun turret. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
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
extern void  start_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d458");
extern void  update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
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
    u8                   pad_0[0x20];
    u8                   health;
    u8                   pad_21[0x5];
    s16                  stun_timer;
    u8                   pad_28[0x1];
    u8                   wrench_hit_sound;
    u8                   pad_2A[0x36];
    struct HitColorFlash hit_flash;
    s32                  sweep_start_moby;
    s32                  sweep_end_moby;
    f32                  turn_speed_degrees;
    s16                  fire_timer;
    s16                  sweep_reversing;
    f32                  sweep_start_yaw;
    f32                  sweep_end_yaw;
    f32                  sweep_offset;
    f32                  sweep_range;
    f32                  base_z;
    u16                  burst_length;
    u8                   pad_96[0x2];
    s32                  burst_pause;
    u16                  burst_timer;
    u8                   solid_shots; /* solid_collision of the shots it fires */
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
    collision = &turret_instance_vars->health;
    animation = &turret_instance_vars->hit_flash;
    hit = find_moby_hit(moby, 0x230000, 0);
    resolve_moby_hit_response((void *)moby, (MobyHit *)hit, collision, 0, contact, 0, 0, 4);
    if ((u8)MOBY(moby)->state < TURRET_HIT_SPIN && contact[0] > 1) {
        FUN_0022da68(1, 0, moby);
        FUN_L00_00257470(moby, 0, -1);
        MOBY(moby)->flags &= 0xefff;
        turret_instance_vars->hit_flash.flash_red = 0xfa;
        turret_instance_vars->stun_timer = scale_game_frames(60);
        start_hit_color_flash(moby, animation);
        turret_instance_vars->fire_timer = scale_game_frames(60);
        if ((u8)MOBY(moby)->prev_seq != 2)
            FUN_00212f90(moby, 2, 0, 5);
        MOBY(moby)->state = TURRET_HIT_SPIN;
    }
    MOBY(moby)->hit_slot = 0xff;
    update_hit_color_flash(moby, animation);
    switch ((u8)MOBY(moby)->state) {
    case TURRET_INITIALIZE:
        if (turret_instance_vars->sweep_start_moby != -1) {
            char *target =
                (char *)(turret_instance_vars->sweep_start_moby * 0x100 + (int)level_mobys);
            turret_instance_vars->sweep_start_yaw =
                fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(target + 0x14) - MOBY(moby)->pos.y);
            if (turret_instance_vars->sweep_start_yaw < 0.0f)
                turret_instance_vars->sweep_start_yaw += 6.2831855f;
        }
        if (turret_instance_vars->sweep_end_moby != -1) {
            char *target =
                (char *)(turret_instance_vars->sweep_end_moby * 0x100 + (int)level_mobys);
            turret_instance_vars->sweep_end_yaw =
                fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(target + 0x14) - MOBY(moby)->pos.y);
            if (turret_instance_vars->sweep_end_yaw < 0.0f)
                turret_instance_vars->sweep_end_yaw += 6.2831855f;
            if (turret_instance_vars->sweep_end_yaw < turret_instance_vars->sweep_start_yaw)
                turret_instance_vars->sweep_end_yaw += 6.2831855f;
        } else {
            turret_instance_vars->sweep_end_yaw =
                turret_instance_vars->sweep_start_yaw + 6.2831855f;
        }
        turret_instance_vars->sweep_offset = 0.0f;
        turret_instance_vars->sweep_range =
            turret_instance_vars->sweep_end_yaw - turret_instance_vars->sweep_start_yaw;
        MOBY(moby)->rot.z = normalize_angle(turret_instance_vars->sweep_start_yaw);
        turret_instance_vars->base_z = MOBY(moby)->pos.z;
        turret_instance_vars->wrench_hit_sound = 0;
        MOBY(moby)->command = 0;
        turret_instance_vars->burst_timer = turret_instance_vars->burst_length;
        if ((u8)MOBY(moby)->prev_seq != 1)
            FUN_00212f90(moby, 1, 0, 0);
        MOBY(moby)->state = TURRET_SWEEP_AND_FIRE;
        break;
    case TURRET_SWEEP_AND_FIRE:
        if ((u8)MOBY(moby)->seq != 1 && ((u8)MOBY(moby)->anim_flags & 2) != 0 &&
            (u8)MOBY(moby)->prev_seq != 1)
            FUN_00212f90(moby, 1, 0, 1);
        if (turret_instance_vars->sweep_reversing != 0) {
            turret_instance_vars->sweep_offset -=
                turret_instance_vars->turn_speed_degrees * 0.017453292f * frame_time;
            if (turret_instance_vars->sweep_offset < 0.0f) {
                turret_instance_vars->sweep_offset = 0.0f;
                turret_instance_vars->sweep_reversing = 0;
            }
        } else {
            turret_instance_vars->sweep_offset +=
                turret_instance_vars->turn_speed_degrees * 0.017453292f * frame_time;
            if (turret_instance_vars->sweep_range < turret_instance_vars->sweep_offset) {
                turret_instance_vars->sweep_offset = turret_instance_vars->sweep_range;
                turret_instance_vars->sweep_reversing = 1;
            }
        }
        MOBY(moby)->rot.z = normalize_angle(turret_instance_vars->sweep_start_yaw +
                                            turret_instance_vars->sweep_offset);
        if (fast_dec_timer_s16(&turret_instance_vars->fire_timer) == 0)
            return;
        if ((u8)MOBY(moby)->command == 0) {
            if (turret_instance_vars->burst_pause == 0 || (turret_instance_vars->burst_timer & 1)) {
                FUN_L00_0024f7c8(moby, 0, position);
                velocity[0] = fast_cos(MOBY(moby)->rot.z) * (D_0015ED6C * 15.0f);
                velocity[1] = fast_sin(MOBY(moby)->rot.z) * (D_0015ED6C * 15.0f);
                velocity[2] = 0.0f;
                FUN_0022da68(0, 0, moby);
                spawn_machinegun_turret_projectile(
                    position, velocity, moby, (u8)turret_instance_vars->solid_shots);
            }
            if (turret_instance_vars->burst_pause != 0) {
                if (fast_dec_timer_s16(&turret_instance_vars->burst_timer) != 0) {
                    MOBY(moby)->command = 1;
                    turret_instance_vars->burst_timer = *(u16 *)&turret_instance_vars->burst_pause;
                }
            } else {
                turret_instance_vars->burst_timer ^= 1;
            }
        } else if (fast_dec_timer_s16(&turret_instance_vars->burst_timer) != 0) {
            MOBY(moby)->command = 0;
            turret_instance_vars->burst_timer = turret_instance_vars->burst_length;
        }
        turret_instance_vars->fire_timer = scale_game_frames(6);
        distance = vector_distance_xy(moby + 0x10, D_0013F3D0);
        if (!(55.0f < distance)) {
            if (!(35.0f < distance) || (u8)MOBY(moby)->visible != 0)
                return;
        }
        if (distance > 80.0f)
            turret_instance_vars->fire_timer *= 3;
        else if (distance > 120.0f) {
            int timer = turret_instance_vars->fire_timer;
            turret_instance_vars->fire_timer = timer << 2;
        } else {
            int timer = turret_instance_vars->fire_timer;
            turret_instance_vars->fire_timer = timer << 1;
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
        if (fast_dec_timer_s16(&turret_instance_vars->fire_timer) == 0)
            return;
        turret_instance_vars->fire_timer = scale_game_frames(180);
        MOBY(moby)->state = TURRET_LOWER_AND_REALIGN;
        break;
    case TURRET_LOWER_AND_REALIGN:
        if ((u8)MOBY(moby)->seq != 4 && ((u8)MOBY(moby)->anim_flags & 2) &&
            (u8)MOBY(moby)->prev_seq != 4)
            FUN_00212f90(moby, 4, 0, 0);
        if (turret_instance_vars->base_z - 0.7f < MOBY(moby)->pos.z)
            MOBY(moby)->pos.z -= frame_time * 5.0f;
        else
            MOBY(moby)->pos.z = turret_instance_vars->base_z - 0.7f;
        direction = normalize_angle(turret_instance_vars->sweep_start_yaw +
                                    turret_instance_vars->sweep_offset);
        distance = fast_difference_between_rotations(MOBY(moby)->rot.z, direction);
        if (D_0015ED6C * 12.566371f < distance)
            MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, D_0015ED6C * 12.566371f);
        else
            MOBY(moby)->rot.z = normalize_angle(turret_instance_vars->sweep_start_yaw +
                                                turret_instance_vars->sweep_offset);
        if (fast_dec_timer_s16(&turret_instance_vars->fire_timer) != 0) {
            FUN_0022da68(2, 0, moby);
            if ((u8)MOBY(moby)->prev_seq != 3)
                FUN_00212f90(moby, 3, 0, 5);
            MOBY(moby)->state = TURRET_RESTORE_HEIGHT;
        }
        break;
    case TURRET_RESTORE_HEIGHT:
        if (MOBY(moby)->pos.z < turret_instance_vars->base_z) {
            MOBY(moby)->pos.z += D_0015ED6C * 5.0f;
            return;
        }
        MOBY(moby)->pos.z = turret_instance_vars->base_z;
        MOBY(moby)->flags |= 0x1000;
        if ((u8)MOBY(moby)->prev_seq != 1)
            FUN_00212f90(moby, 1, 0, 5);
        MOBY(moby)->state = TURRET_SWEEP_AND_FIRE;
        break;
    case 5:
        break;
    }
}
