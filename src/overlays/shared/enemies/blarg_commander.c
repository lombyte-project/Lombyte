/* Shared moby classes: blarg_commander. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/quad.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

char *create_class_55b_moby_from_source(char *source_object) __asm__("FUN_L12_00308670");

char *create_class_55b_moby_from_source(char *source_object) {
    unsigned char *allocated_object = (unsigned char *)create_moby(0x55B);
    if (allocated_object != 0) {
        char *object_data = *(char **)(allocated_object + 0x78);
        allocated_object[0x30] = 0xFF;
        *(short *)(allocated_object + 0x32) = 0x7F;
        allocated_object[0x31] = 1;
        allocated_object[0x20] = 0;
        *(OvlQuad *)(allocated_object + 0x10) = *(OvlQuad *)(source_object + 0x10);
        *(OvlQuad *)(allocated_object + 0x40) = *(OvlQuad *)(source_object + 0x40);
        *(char **)(allocated_object + 0xB8) = source_object;
        fast_mem_zero16(object_data, 0x20);
        *(int *)(object_data + 0x10) = (int)(object_data + 0x20);
        update_entity(allocated_object);
    }
    return (char *)allocated_object;
}

extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");

#include "rnc/mobys/moby.h"

/* Pvars of a lobbed projectile, as set up at launch. */
typedef struct {
    u8    pad0[0x20];
    Vec4f arc;          /* 0x20: filled by trooper_start_knockback */
    f32   rise;         /* 0x30: D_0015ED70 * 40 */
    f32   fall;         /* 0x34: D_0015ED70 * 100 */
    f32   launch_speed; /* 0x38 */
    f32   unk3C;        /* 0x3C */
    s32   timer;        /* 0x40 */
    s32   unk44;        /* 0x44: set to 1 on launch */
    f32   unk48;        /* 0x48: 0.7 on launch */
    u8    pad4C[0x11];
    u8    unk5D;
    u8    pad5E[0xE];
    f32   unk6C; /* 0x6C */
    f32   unk70; /* 0x70: -1 on launch */
    f32   unk74; /* 0x74: -1 on launch */
} LobbedVars;

void start_blarg_commander_knockback(void *unused, struct Moby *moby) __asm__("FUN_L12_00308708");

void start_blarg_commander_knockback(void *unused, struct Moby *moby) {
    LobbedVars *lobbed_vars = (LobbedVars *)moby->pvars;
    float       knockback_input_vector[4];
    float       knockback_heading;
    float       rise_sqrt_term, frame_rise_sqrt_term;
    float       frame_scaled_rise;
    float       frame_time_squared;

    if (random_integer_below(3) == 0) {
        allocate_voice_for_target_entry(1, 0x20, moby);
    }
    lobbed_vars->unk48 = 0.7f;
    lobbed_vars->timer = truncate_float_to_s32(716.8f);
    lobbed_vars->unk6C = 0.0f;
    moby->update_dist = 0xFF;
    moby->flags &= ~6;
    knockback_heading = fast_add_rotations(moby->rot.z, 3.1415927f);
    knockback_input_vector[0] = fast_cos(knockback_heading);
    knockback_input_vector[1] = fast_sin(knockback_heading);
    knockback_input_vector[2] = 1.0f;
    knockback_input_vector[3] = 5627.925f;
    frame_time_squared = frame_time_sq;
    lobbed_vars->rise = frame_time_squared * 40.0f;
    lobbed_vars->fall = frame_time_squared * 100.0f;
    rise_sqrt_term = square_root(lobbed_vars->rise * 4.0f);
    frame_rise_sqrt_term = square_root(frame_time_sq * 40.0f * 4.0f);
    frame_scaled_rise = frame_time_sq * 40.0f;
    lobbed_vars->launch_speed =
        3.0f / ((rise_sqrt_term + rise_sqrt_term) / frame_scaled_rise) +
        frame_time_sq * 100.0f * 0.5f *
            ((frame_rise_sqrt_term + frame_rise_sqrt_term) / frame_scaled_rise);
    lobbed_vars->unk3C = square_root(frame_scaled_rise * 4.0f);
    lobbed_vars->unk5D = 0;
    lobbed_vars->unk44 = 1;
    trooper_knockback_arc(knockback_input_vector,
                          &knockback_heading,
                          &lobbed_vars->launch_speed,
                          &lobbed_vars->unk3C);
    trooper_start_knockback(knockback_heading, moby, &lobbed_vars->arc, 1, 5, 2);
    lobbed_vars->unk6C = frame_time + frame_time;
    lobbed_vars->unk70 = -1.0f;
    lobbed_vars->unk74 = -1.0f;
    moby->flags &= ~0x1000;
    blend_moby_animation(moby, 1, 0, 0);
}

#include "sda.h"

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BD3D0.c: func_L12_00309CF8), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad[0x40];
    float vx, vy;
} MO;

extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

void blarg_commander_update(char *m) __asm__("FUN_L12_00308918");

/* Moby class 1371 */
void blarg_commander_update(char *moby) {
    int   moby_state;
    char *moby_pvars;
    char *motion_state;
    int   motion_result_flags;
    MO   *mo = (MO *)moby;
    char  raised_position[16];
    moby_state = MOBY(moby)->prev_seq;
    moby_pvars = ((char *)MOBY(moby)->pvars);
    if (moby_state == 1) {
        motion_state = moby_pvars + 0x20;
        motion_result_flags = step_moby_motion(moby, motion_state);
        mo->vx = fast_add_rotations(mo->vx, frame_time * 4.3633232f);
        mo->vy = fast_add_rotations(mo->vy, frame_time * 4.3633232f);
        MOBY(moby)->flags &= 0xEFFF;
        if (motion_result_flags & 0x61) {
            qcopy(raised_position, moby + 0x10);
            ((float *)raised_position)[2] += 1.0f;
            spawn_explosion(moby,
                            motion_state,
                            raised_position,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            2.0f,
                            1.0f,
                            9.0f,
                            1.0f,
                            0,
                            15.0f,
                            1,
                            moby_state,
                            -1,
                            0);
            mark_moby_for_removal(moby);
        } else if (MOBY(moby)->pos.z < 5.0f) {
            mark_moby_for_removal(moby);
        }
    }
}
