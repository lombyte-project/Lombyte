/* Level 11 moby class child_grid_controller: spawns and poses its 4x6 child grid. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

extern float D_L11_0015F3FC;

#include "rnc/overlay/cuboid.h"

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

typedef struct {
    struct Moby *grid[4][6];
    int          link;
    int          from;
    int          to;
    float        t;
    float        vel;
    float        prev;
    float        fade;
} GridVars_d800;

extern int D_L11_0015FFD8_d800 __asm__("D_L11_0015FFD8");
int        D_L11_00161F54 = 0;

#include "rnc/overlay/l11/mobys/child_grid_controller_globals.h"

extern float child_grid_pose_blend_acceleration_gp __asm__("D_L11_00161F58") __attribute__((sda));
extern float child_grid_pose_blend_deceleration_gp __asm__("D_L11_00161F5C") __attribute__((sda));
extern float child_grid_pose_blend_max_speed_gp __asm__("D_L11_00161F60") __attribute__((sda));
extern void  allocate_voice_for_target_entry(int, int, struct Moby *) __asm__("FUN_0022da68");
void         update_child_grid_poses(struct Moby *grid_controller) __asm__("FUN_L11_0030dd18");

void update_child_grid_poses(struct Moby *m) __asm__("FUN_L11_0030dd18");

/* Spawns the 4x6 child grid, waits for its linked moby, then fades and blends between two l11_cuboids
   points (pos at 0x30, rotation at 0x70) before handing back to the hero. */
void child_grid_controller_update(struct Moby *grid_controller) __asm__("FUN_L11_0030d800");

extern float fast_add_rotations(float, float);

/* Moby class 1157 */
void child_grid_controller_update(struct Moby *grid_controller) {
    enum {
        CHILD_GRID_STATE_CREATE_CHILDREN = 0,
        CHILD_GRID_STATE_WAIT_LINK_COMMAND = 1,
        CHILD_GRID_STATE_FADE_TO_POINT_POSE = 2,
        CHILD_GRID_STATE_BLEND_POINT_POSES = 3,
        CHILD_GRID_STATE_FADE_TO_HERO_POSE = 4,
        CHILD_GRID_STATE_IDLE = 5
    };
    GridVars_d800 *grid_controller_vars = (GridVars_d800 *)grid_controller->pvars;
    Vec4f          pos;
    Vec4f          rot;
    int            row;
    int            col;
    float          previous_fade;

    switch (grid_controller->state) {
    case CHILD_GRID_STATE_CREATE_CHILDREN:
        for (row = 0; row < 4; row++) {
            for (col = 0; col < 6; col++) {
                grid_controller_vars->grid[row][col] = create_moby(0x4B7);
                grid_controller_vars->grid[row][col]->draw_dist = 0x40;
                grid_controller_vars->grid[row][col]->visible = 1;
                grid_controller_vars->grid[row][col]->spawn_frame = hero.moby->spawn_frame;
                grid_controller_vars->grid[row][col]->flags = grid_controller->flags;
                qcopy_nc(&grid_controller_vars->grid[row][col]->pos, &grid_controller->pos);
                qcopy(&grid_controller_vars->grid[row][col]->rot, &grid_controller->rot);
            }
        }
        grid_controller_vars->t = 0.0f;
        grid_controller->state = 1;
        break;
    case CHILD_GRID_STATE_WAIT_LINK_COMMAND: {
        struct Moby *link =
            (struct Moby *)((grid_controller_vars->link << 8) + D_L11_0015FFD8_d800);
        if (link->command == 1) {
            grid_controller->state = 2;
            grid_controller_vars->fade = -1.0f;
            D_L11_00161F54 = 0;
            allocate_voice_for_target_entry(0, 0, grid_controller);
        } else if (link->command == 2) {
            grid_controller_vars->t = 1.0f;
            grid_controller->state = 5;
        }
        break;
    }
    case CHILD_GRID_STATE_FADE_TO_POINT_POSE:
        previous_fade = grid_controller_vars->fade;
        approach_value(&grid_controller_vars->fade, 1.0f, 1.0f / (float)scale_game_frames(20));
        D_L11_0015F3FC = 1.0f - absolute_float(grid_controller_vars->fade);
        if (previous_fade < 0.0f && 0.0f <= grid_controller_vars->fade) {
            struct Moby *link =
                (struct Moby *)(D_L11_0015FFD8_d800 + (grid_controller_vars->link << 8));
            char *source_pose_point;
            set_race_camera(&link->pos, &link->rot, 0x72, 1);
            source_pose_point = (char *)((int)l11_cuboids + (grid_controller_vars->from << 7));
            jet_mount(source_pose_point + 0x30, source_pose_point + 0x70, 1, 0, 0);
        } else if (1.0f <= grid_controller_vars->fade) {
            grid_controller->state = 3;
        }
        break;
    case CHILD_GRID_STATE_BLEND_POINT_POSES:
        grid_controller_vars->prev = grid_controller_vars->t;
        advance_accelerated_scalar(&grid_controller_vars->t,
                                   &grid_controller_vars->vel,
                                   1.0f,
                                   child_grid_pose_blend_acceleration_gp * frame_time_sq,
                                   child_grid_pose_blend_deceleration_gp * frame_time_sq,
                                   child_grid_pose_blend_max_speed_gp * frame_time);
        lerp_vector(&pos,
                    (char *)((int)l11_cuboids + (grid_controller_vars->from << 7)) + 0x30,
                    (char *)((int)l11_cuboids + (grid_controller_vars->to << 7)) + 0x30,
                    grid_controller_vars->t);
        clear_u64_value(&rot);
        rot.x = fast_subtract_rotations(
                    ((float *)((grid_controller_vars->to << 7) + (int)l11_cuboids))[28],
                    ((float *)((grid_controller_vars->from << 7) + (int)l11_cuboids))[28]) *
                grid_controller_vars->t;
        rot.x = fast_add_rotations(
            ((float *)((grid_controller_vars->from << 7) + (int)l11_cuboids))[28], rot.x);
        rot.y = fast_subtract_rotations(
                    ((float *)((grid_controller_vars->to << 7) + (int)l11_cuboids))[29],
                    ((float *)((grid_controller_vars->from << 7) + (int)l11_cuboids))[29]) *
                grid_controller_vars->t;
        rot.y = fast_add_rotations(
            ((float *)((grid_controller_vars->from << 7) + (int)l11_cuboids))[29], rot.y);
        rot.z = fast_subtract_rotations(
                    ((float *)((grid_controller_vars->to << 7) + (int)l11_cuboids))[30],
                    ((float *)((grid_controller_vars->from << 7) + (int)l11_cuboids))[30]) *
                grid_controller_vars->t;
        rot.z = fast_add_rotations(
            ((float *)((grid_controller_vars->from << 7) + (int)l11_cuboids))[30], rot.z);
        set_camera_position(&pos);
        set_camera_rotation(&rot);
        if (1.0f <= grid_controller_vars->t) {
            grid_controller->state = 4;
            grid_controller_vars->fade = -1.0f;
        }
        break;
    case CHILD_GRID_STATE_FADE_TO_HERO_POSE:
        previous_fade = grid_controller_vars->fade;
        approach_value(&grid_controller_vars->fade, 1.0f, 1.0f / (float)scale_game_frames(20));
        D_L11_0015F3FC = 1.0f - absolute_float(grid_controller_vars->fade);
        if (previous_fade < 0.0f && 0.0f <= grid_controller_vars->fade) {
            set_race_camera(&hero.motion.pos, &hero.motion.rot, 0, 1);
            jet_end_mount(3);
        } else if (1.0f <= grid_controller_vars->fade) {
            grid_controller->state = 5;
        }
        break;
    case CHILD_GRID_STATE_IDLE:
        break;
    }
    update_child_grid_poses(grid_controller);
}

extern float child_grid_parent_axis0_offset_length_gp __asm__("D_L11_00161F40")
    __attribute__((sda));
extern float child_grid_parent_axis1_offset_length_gp __asm__("D_L11_00161F44")
    __attribute__((sda));
extern float child_grid_child_axis1_offset_length_gp __asm__("D_L11_00161F48") __attribute__((sda));

/* Lays out the 4x6 grid of child mobys around this one, fanned by the progress value at pvars+0x6C. */
void update_child_grid_poses(struct Moby *grid_controller) {
    Vec4f parent_axis0_offset;
    Vec4f parent_axis1_offset;
    Vec4f child_position_offset;
    char *child_grid_vars;
    struct Moby *(*grid)[6];
    float progress;
    float hi;
    float lo;
    float row_angle_offset_degrees;
    float angle;
    int   row;
    int   col;

    child_grid_vars = grid_controller->pvars;
    grid = (struct Moby * (*)[6]) child_grid_vars;
    fast_vec_normalize(
        &parent_axis0_offset, &grid_controller->rot_row0, child_grid_parent_axis0_offset_length_gp);
    fast_vec_normalize(
        &parent_axis1_offset, &grid_controller->rot_row1, child_grid_parent_axis1_offset_length_gp);
    progress = *(float *)(child_grid_vars + 0x6C);
    hi = 90.0f - progress * 180.0f;
    if (hi < -90.0f) {
        hi = -90.0f;
    }
    lo = 90.0f - (progress - 0.5f) * 180.0f;
    if (90.0f < lo) {
        lo = 90.0f;
    } else if (lo < 0.0f) {
        lo = 0.0f;
    }
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            row_angle_offset_degrees = (row < 2) ? hi : lo;
            if (row < 2) {
                fast_vec_add(&grid[row][col]->pos, &grid_controller->pos, &parent_axis0_offset);
            } else {
                fast_vec_sub(&grid[row][col]->pos, &grid_controller->pos, &parent_axis0_offset);
            }
            angle = ((float)col - 2.5f) * 0.2617994f;
            if (row & 1) {
                grid[row][col]->rot.z = fast_add_rotations(grid_controller->rot.z, 0.0f);
                angle = fast_add_rotations(angle, row_angle_offset_degrees * 0.017453292f);
                if (angle < -0.7853982f) {
                    angle += 1.5707964f;
                }
            } else {
                grid[row][col]->rot.z = fast_add_rotations(grid_controller->rot.z, 3.1415927f);
                angle = fast_subtract_rotations(angle, row_angle_offset_degrees * 0.017453292f);
                if (0.7853982f < angle) {
                    angle -= 1.5707964f;
                }
            }
            if (0.7853982f < angle) {
                angle = 0.7853982f;
            } else if (angle < -0.7853982f) {
                angle = -0.7853982f;
            }
            grid[row][col]->rot.z = fast_add_rotations(grid[row][col]->rot.z, angle);
            euler_rotation_basis(&grid[row][col]->rot_row0, &grid[row][col]->rot);
            fast_vec_normalize(&child_position_offset,
                               &grid[row][col]->rot_row1,
                               -child_grid_child_axis1_offset_length_gp);
            if (row & 1) {
                fast_vec_add(&child_position_offset, &child_position_offset, &parent_axis1_offset);
            } else {
                fast_vec_sub(&child_position_offset, &child_position_offset, &parent_axis1_offset);
            }
            fast_vec_add(&grid[row][col]->pos, &grid[row][col]->pos, &child_position_offset);
            update_entity(grid[row][col]);
        }
    }
}
