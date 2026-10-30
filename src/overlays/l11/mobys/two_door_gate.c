/* Level 11 moby class two_door_gate: spawns both doors and swings them open. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

#include "rnc/overlay/l11/mobys/two_door_gate_globals.h"

extern float gate_door_z_offset_gp __asm__("D_L11_00162308") __attribute__((sda));
extern float gate_door_x_rotation_degrees_gp __asm__("D_L11_0016230C") __attribute__((sda));
extern float gate_door_y_rotation_degrees_gp __asm__("D_L11_00162310") __attribute__((sda));
extern s32   FUN_00214720(void *, s32);
extern s32   FUN_0022da68(s32, s32, void *);
extern f32   advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");

/* Private variables of the two-door moby FUN_L11_00316320 runs. */
struct L11GateVars {
    struct Moby *left;  /* 0x0 */
    struct Moby *right; /* 0x4 */
    f32          t;     /* 0x8 */
    f32          speed; /* 0xC */
    s32          unk10; /* 0x10 */
};

/* Two-door gate: spawns both 0x4DF doors, then once the hero is close lifts, swings and tilts them open. */
void update_two_door_gate(struct Moby *moby) __asm__("FUN_L11_00316320");

void update_two_door_gate(struct Moby *moby) {
    enum {
        GATE_INITIALIZE_DOORS = 0,
        GATE_WAIT_FOR_HERO_TRIGGER = 1,
        GATE_RAISE_DOORS = 2,
        GATE_ZERO_DOOR_Y_ROTATION = 3,
        GATE_TILT_DOORS_APART = 4,
        GATE_OPEN_IDLE = 5,
        GATE_APPLY_CONFIGURED_DOOR_POSE = 6
    };
    struct L11GateVars *gate_vars = (struct L11GateVars *)moby->pvars;
    float               door_position_offset[4];

    switch (moby->state) {
    case GATE_INITIALIZE_DOORS: {
        struct Moby *door_moby;
        struct Moby *door_rotation_target;
        u8          *door_vars;
        moby->scale *= 2.0f;
        gate_vars->left = create_moby(0x4DF);
        gate_vars->left->draw_dist = 0x40;
        gate_vars->left->visible = 1;
        gate_vars->left->spawn_frame = hero.moby->spawn_frame;
        gate_vars->left->flags = moby->flags | 0x20;
        door_moby = gate_vars->left;
        qcopy(&door_moby->pos, &moby->pos);
        door_moby->pos.z -= 4.0f;
        door_rotation_target = gate_vars->left;
        qcopy(&door_rotation_target->rot, &moby->rot);
        door_rotation_target->rot.x = -(gate_door_x_rotation_degrees_gp * 0.017453292f);
        gate_vars->left->rot.y = gate_door_y_rotation_degrees_gp * 0.017453292f;
        door_vars = gate_vars->left->pvars;
        *(u8 **)(door_vars + 8) = door_vars + 0x20;
        *(u32 *)(door_vars + 0x5C) |= 1;
        gate_vars->right = create_moby(0x4DF);
        gate_vars->right->draw_dist = 0x40;
        gate_vars->right->visible = 1;
        gate_vars->right->spawn_frame = hero.moby->spawn_frame;
        gate_vars->right->flags = moby->flags | 0x8020;
        door_moby = gate_vars->right;
        qcopy(&door_moby->pos, &moby->pos);
        door_moby->pos.z -= 4.0f;
        door_rotation_target = gate_vars->right;
        qcopy(&door_rotation_target->rot, &moby->rot);
        door_rotation_target->rot.x = gate_door_x_rotation_degrees_gp * 0.017453292f;
        gate_vars->right->rot.y = gate_door_y_rotation_degrees_gp * 0.017453292f;
        door_vars = gate_vars->right->pvars;
        *(u8 **)(door_vars + 8) = door_vars + 0x20;
        *(u32 *)(door_vars + 0x5C) |= 1;
        moby->state = 1;
        break;
    }
    case GATE_WAIT_FOR_HERO_TRIGGER:
        if (vector_distance_xy(&moby->pos.x, &hero.motion.pos.f[0]) < 12.0f &&
            FUN_00214720(&hero.motion.pos, gate_vars->unk10) != 0) {
            gate_vars->t = 0.0f;
            moby->state = 2;
            FUN_0022da68(0, 4, gate_vars->left);
        }
        break;
    case GATE_RAISE_DOORS:
        advance_accelerated_scalar(&gate_vars->t,
                                   &gate_vars->speed,
                                   1.0f,
                                   frame_time_sq * 8.0f,
                                   frame_time_sq * 16.0f,
                                   frame_time * 8.0f);
        fast_vec_normalize(door_position_offset, &moby->rot_row0, 0.5f);
        fast_vec_add(&gate_vars->left->pos, &moby->pos, door_position_offset);
        gate_vars->left->pos.z = moby->pos.z + gate_vars->t * 4.0f - 4.0f;
        fast_vec_add(&gate_vars->right->pos, &moby->pos, door_position_offset);
        gate_vars->right->pos.z = moby->pos.z + gate_vars->t * 4.0f - 4.0f;
        update_entity(gate_vars->left);
        update_entity(gate_vars->right);
        if (gate_vars->t >= 1.0f) {
            gate_vars->t = 0.0f;
            moby->state = 3;
        }
        break;
    case GATE_ZERO_DOOR_Y_ROTATION: {
        float a;
        advance_accelerated_scalar(&gate_vars->t,
                                   &gate_vars->speed,
                                   1.0f,
                                   frame_time_sq * 10.0f,
                                   frame_time_sq * 20.0f,
                                   frame_time * 10.0f);
        a = (1.0f - gate_vars->t) * (gate_door_y_rotation_degrees_gp * 0.017453292f);
        gate_vars->left->rot.y = a;
        gate_vars->right->rot.y = a;
        update_entity(gate_vars->left);
        update_entity(gate_vars->right);
        if (gate_vars->t >= 1.0f) {
            gate_vars->t = 0.0f;
            moby->state = 4;
        }
        break;
    }
    case GATE_TILT_DOORS_APART: {
        float a;
        advance_accelerated_scalar(&gate_vars->t,
                                   &gate_vars->speed,
                                   1.0f,
                                   frame_time_sq * 12.0f,
                                   frame_time_sq * 24.0f,
                                   frame_time * 12.0f);
        gate_vars->left->pos.z = moby->pos.z + gate_vars->t * 0.25f;
        gate_vars->right->pos.z = moby->pos.z + gate_vars->t * 0.25f;
        a = gate_vars->t * 1.5707964f;
        gate_vars->left->rot.x = -a;
        gate_vars->right->rot.x = a;
        update_entity(gate_vars->left);
        update_entity(gate_vars->right);
        if (gate_vars->t >= 1.0f) {
            gate_vars->t = 0.0f;
            moby->state = 5;
        }
        break;
    }
    case GATE_OPEN_IDLE:
        break;
    case GATE_APPLY_CONFIGURED_DOOR_POSE:
        fast_vec_normalize(door_position_offset, &moby->rot_row0, 0.5f);
        fast_vec_add(&gate_vars->left->pos, &moby->pos, door_position_offset);
        gate_vars->left->pos.z = moby->pos.z + gate_door_z_offset_gp;
        gate_vars->left->rot.x = -(gate_door_x_rotation_degrees_gp * 0.017453292f);
        gate_vars->left->rot.y = gate_door_y_rotation_degrees_gp * 0.017453292f;
        update_entity(gate_vars->left);
        fast_vec_add(&gate_vars->right->pos, &moby->pos, door_position_offset);
        gate_vars->right->pos.z = moby->pos.z + gate_door_z_offset_gp;
        gate_vars->right->rot.x = gate_door_x_rotation_degrees_gp * 0.017453292f;
        gate_vars->right->rot.y = gate_door_y_rotation_degrees_gp * 0.017453292f;
        update_entity(gate_vars->right);
        break;
    }
}
