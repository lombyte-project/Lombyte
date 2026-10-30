/* Level 11 moby class child_sets_controller: spawns and poses its child moby sets. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/game/item_state.h"
#include "sda.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

typedef struct {
    int   link;
    int   from;
    int   to;
    float fade;
} CamVars_c788;

extern struct Moby *l11_three_per_side_child_mobys[] __asm__("D_L11_001DA710");
extern struct Moby *D_L11_00161F10[] MACRO_ADDR;
extern struct Moby *D_L11_00161F20[] MACRO_ADDR;
extern struct Moby *D_L11_00161F30[] MACRO_ADDR;
float               D_L11_00161EC4 = 0;
float               D_L11_00161EC8 = 0;
float               D_L11_00161ECC = 0;
float               D_L11_00161ED0 = 0;
float               D_L11_00161ED4 = 0;
float               D_L11_00161ED8 = 0;
float               D_L11_00161EDC = 0;
float               D_L11_00161EE0 = 0;
float               D_L11_00161EE4 = 0;
float               D_L11_00161EE8 = 0;
float               D_L11_00161EEC = 0;
float               D_L11_00161EF0 = 0;
float               D_L11_00161EF4 = 0;
float               D_L11_00161EF8 = 0;
extern float        D_L11_0015F3FC;
extern int          D_L11_0015FFD8_c788 __asm__("D_L11_0015FFD8");

#include "rnc/overlay/cuboid.h"

extern int   test_linked_moby_turn_trigger(void *) __asm__("FUN_L11_0030f270");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
void        update_child_set_poses(struct Moby *parent_moby) __asm__("FUN_L11_0030d1b0");

void update_child_set_poses(struct Moby *m) __asm__("FUN_L11_0030d1b0");

/* Controller for the child moby sets laid out by FUN_L11_0030d1b0: spawns them, waits for its linked
   moby, fades in, then unfolds the sets one after another while blending between two l11_cuboids points. */
void update_child_sets_controller(struct Moby *child_sets_controller) __asm__("FUN_L11_0030c788");

void update_child_sets_controller(struct Moby *child_sets_controller) {
    enum {
        CHILD_SETS_STATE_CREATE_CHILDREN = 0,
        CHILD_SETS_STATE_WAIT_FOR_LINK_TRIGGER = 1,
        CHILD_SETS_STATE_FADE_BEFORE_ACTIVATION = 2,
        CHILD_SETS_STATE_UNFOLD_AND_BLEND_POSES = 3,
        CHILD_SETS_STATE_FADE_AFTER_HERO_HANDOFF = 4
    };
    CamVars_c788 *child_sets_controller_vars = (CamVars_c788 *)child_sets_controller->pvars;
    Vec4f         pos;
    Vec4f         rot;
    int           i;

    switch (child_sets_controller->state) {
    case CHILD_SETS_STATE_CREATE_CHILDREN:
        child_sets_controller->visible = 0;
        child_sets_controller->collision = 0;
        child_sets_controller->flags |= 1;
        D_L11_00161EC4 = 0.0f;
        D_L11_00161EC8 = 0.0f;
        D_L11_00161ECC = 0.0f;
        D_L11_00161ED0 = 0.0f;
        D_L11_00161ED4 = 0.0f;
        D_L11_00161ED8 = 0.0f;
        D_L11_00161EDC = 0.0f;
        child_sets_controller->command = 0;
        for (i = 0; i < 6; i++) {
            l11_three_per_side_child_mobys[i] = create_moby(0x480);
            l11_three_per_side_child_mobys[i]->draw_dist = 0x40;
            l11_three_per_side_child_mobys[i]->visible = 1;
            l11_three_per_side_child_mobys[i]->spawn_frame = hero.moby->spawn_frame;
            qcopy_nc(&l11_three_per_side_child_mobys[i]->pos, &child_sets_controller->pos);
            qcopy_nc(&l11_three_per_side_child_mobys[i]->rot, &child_sets_controller->rot);
            l11_three_per_side_child_mobys[i]->flags |= 1;
            l11_three_per_side_child_mobys[i]->visible = 0;
            l11_three_per_side_child_mobys[i]->collision = 0;
        }
        for (i = 0; i < 4; i++) {
            D_L11_00161F20[i] = create_moby(0x481);
            D_L11_00161F20[i]->draw_dist = 0x40;
            D_L11_00161F20[i]->visible = 1;
            D_L11_00161F20[i]->spawn_frame = hero.moby->spawn_frame;
            qcopy_nc(&D_L11_00161F20[i]->pos, &child_sets_controller->pos);
            qcopy_nc(&D_L11_00161F20[i]->rot, &child_sets_controller->rot);
            D_L11_00161F20[i]->flags |= 1;
            D_L11_00161F20[i]->visible = 0;
            D_L11_00161F20[i]->collision = 0;
        }
        for (i = 0; i < 4; i++) {
            D_L11_00161F30[i] = create_moby(0x482);
            D_L11_00161F30[i]->draw_dist = 0x40;
            D_L11_00161F30[i]->visible = 1;
            D_L11_00161F30[i]->spawn_frame = hero.moby->spawn_frame;
            qcopy_nc(&D_L11_00161F30[i]->pos, &child_sets_controller->pos);
            qcopy_nc(&D_L11_00161F30[i]->rot, &child_sets_controller->rot);
            D_L11_00161F30[i]->flags |= 1;
            D_L11_00161F30[i]->visible = 0;
            D_L11_00161F30[i]->collision = 0;
        }
        for (i = 0; i < 4; i++) {
            char *cv;
            D_L11_00161F10[i] = create_moby(0x483);
            D_L11_00161F10[i]->draw_dist = 0x40;
            D_L11_00161F10[i]->visible = 1;
            D_L11_00161F10[i]->spawn_frame = hero.moby->spawn_frame;
            qcopy_nc(&D_L11_00161F10[i]->pos, &child_sets_controller->pos);
            qcopy_nc(&D_L11_00161F10[i]->rot, &child_sets_controller->rot);
            D_L11_00161F10[i]->flags |= 0x20;
            cv = D_L11_00161F10[i]->pvars;
            *(char **)(cv + 8) = cv + 0x20;
            *(int *)(cv + 0x5C) |= 1;
            D_L11_00161F10[i]->flags |= 1;
            D_L11_00161F10[i]->visible = 0;
            D_L11_00161F10[i]->collision = 0;
        }
        update_child_set_poses(child_sets_controller);
        child_sets_controller->state = 1;
        break;
    case CHILD_SETS_STATE_WAIT_FOR_LINK_TRIGGER:
        if (child_sets_controller_vars->link != -1 &&
            test_linked_moby_turn_trigger(
                (void *)(D_L11_0015FFD8_c788 + (child_sets_controller_vars->link << 8)))) {
            child_sets_controller->state = 2;
        }
        break;
    case CHILD_SETS_STATE_FADE_BEFORE_ACTIVATION:
        approach_value(&child_sets_controller_vars->fade, 1.0f, frame_time * 4.0f);
        D_L11_0015F3FC = child_sets_controller_vars->fade;
        if (1.0f <= child_sets_controller_vars->fade) {
            int j;
            child_sets_controller->state = 3;
            qcopy(&pos,
                  &((struct Moby *)(D_L11_0015FFD8_c788 + (child_sets_controller_vars->link << 8)))
                       ->pos);
            pos.z += 5.0f;
            pos.z = probe_ground_height(&pos, 0, 0.5f);
            clear_u64_value(&rot);
            rot.z = fast_arc_tan(child_sets_controller->pos.x - pos.x,
                                 child_sets_controller->pos.y - pos.y);
            set_race_camera(&pos, &rot, 0x72, 1);
            jet_mount((char *)((int)l11_cuboids + (child_sets_controller_vars->from << 7)) + 0x30,
                      (char *)((int)l11_cuboids + (child_sets_controller_vars->to << 7)) + 0x70,
                      1,
                      0,
                      0);
            for (j = 0; j < 6; j++) {
                l11_three_per_side_child_mobys[j]->flags &= ~1;
                l11_three_per_side_child_mobys[j]->visible = 1;
                l11_three_per_side_child_mobys[j]->collision =
                    l11_three_per_side_child_mobys[j]->pclass->collision;
            }
            for (j = 0; j < 4; j++) {
                D_L11_00161F20[j]->flags &= ~1;
                D_L11_00161F20[j]->visible = 1;
                D_L11_00161F20[j]->collision = D_L11_00161F20[j]->pclass->collision;
            }
            for (j = 0; j < 4; j++) {
                D_L11_00161F30[j]->flags &= ~1;
                D_L11_00161F30[j]->visible = 1;
                D_L11_00161F30[j]->collision = D_L11_00161F30[j]->pclass->collision;
            }
            for (j = 0; j < 4; j++) {
                D_L11_00161F10[j]->flags &= ~1;
                D_L11_00161F10[j]->visible = 1;
                D_L11_00161F10[j]->collision = D_L11_00161F10[j]->pclass->collision;
            }
            allocate_voice_for_target_entry(0, 0, child_sets_controller);
        }
        break;
    case CHILD_SETS_STATE_UNFOLD_AND_BLEND_POSES:
        if (D_L11_00161EC4 < 1.0f) {
            advance_accelerated_scalar(
                &D_L11_00161EC4, &D_L11_00161EE0, 1.0f, frame_time_sq, frame_time_sq, frame_time);
        }
        if (0.9f < D_L11_00161EC4 && D_L11_00161EC8 == 0.0f) {
            allocate_voice_for_target_entry(1, 0, child_sets_controller);
        }
        if (0.9f < D_L11_00161EC4 && D_L11_00161EC8 < 1.0f) {
            advance_accelerated_scalar(&D_L11_00161EC8,
                                       &D_L11_00161EE4,
                                       1.0f,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq + frame_time_sq,
                                       frame_time + frame_time);
        }
        if (0.5f < D_L11_00161EC8 && D_L11_00161ECC == 0.0f) {
            allocate_voice_for_target_entry(1, 0, child_sets_controller);
        }
        if (0.5f < D_L11_00161EC8 && D_L11_00161ECC < 1.0f) {
            advance_accelerated_scalar(&D_L11_00161ECC,
                                       &D_L11_00161EE8,
                                       1.0f,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq + frame_time_sq,
                                       frame_time + frame_time);
        }
        if (1.0f <= D_L11_00161EC8 && D_L11_00161ED0 < 1.0f) {
            advance_accelerated_scalar(&D_L11_00161ED0,
                                       &D_L11_00161EEC,
                                       1.0f,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq + frame_time_sq,
                                       frame_time + frame_time);
        }
        if (1.0f <= D_L11_00161ECC && D_L11_00161ED4 < 1.0f) {
            advance_accelerated_scalar(&D_L11_00161ED4,
                                       &D_L11_00161EF0,
                                       1.0f,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq + frame_time_sq,
                                       frame_time + frame_time);
        }
        if (0.9f < D_L11_00161ED4 && D_L11_00161ED8 < 1.0f) {
            advance_accelerated_scalar(&D_L11_00161ED8,
                                       &D_L11_00161EF4,
                                       1.0f,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq * 4.0f,
                                       frame_time + frame_time);
        }
        update_child_set_poses(child_sets_controller);
        if (alternate_item_available[0x56] != 0 && D_0013CAE4 != 0) {
            child_sets_controller->command = 1;
        }
        if (D_L11_00161ED8 < 1.0f && child_sets_controller->command == 0) {
            approach_value(&child_sets_controller_vars->fade, 0.0f, frame_time * 4.0f);
        } else {
            approach_value(&child_sets_controller_vars->fade, 1.0f, frame_time * 4.0f);
        }
        D_L11_0015F3FC = child_sets_controller_vars->fade;
        advance_accelerated_scalar(&D_L11_00161EDC,
                                   &D_L11_00161EF8,
                                   1.0f,
                                   frame_time_sq * 0.2f,
                                   frame_time_sq * 0.2f,
                                   frame_time * 0.2f);
        lerp_vector(&pos,
                    (char *)((int)l11_cuboids + (child_sets_controller_vars->from << 7)) + 0x30,
                    (char *)((int)l11_cuboids + (child_sets_controller_vars->to << 7)) + 0x30,
                    D_L11_00161EDC);
        lerp_vector(&rot,
                    (char *)((int)l11_cuboids + (child_sets_controller_vars->from << 7)) + 0x70,
                    (char *)((int)l11_cuboids + (child_sets_controller_vars->to << 7)) + 0x70,
                    D_L11_00161EDC);
        set_camera_position(&pos);
        set_camera_rotation(&rot);
        if (1.0f <= child_sets_controller_vars->fade) {
            D_L11_00161EC4 = 1.0f;
            D_L11_00161EC8 = 1.0f;
            D_L11_00161ECC = 1.0f;
            D_L11_00161ED0 = 1.0f;
            D_L11_00161ED4 = 1.0f;
            D_L11_00161ED8 = 1.0f;
            update_child_set_poses(child_sets_controller);
            child_sets_controller->state = 4;
            jet_end_mount(3);
            set_race_camera(&hero.motion.pos, &hero.motion.rot, 0, 1);
            alternate_item_available[0x56] = 1;
        }
        break;
    case CHILD_SETS_STATE_FADE_AFTER_HERO_HANDOFF:
        approach_value(&child_sets_controller_vars->fade, 0.0f, frame_time * 4.0f);
        D_L11_0015F3FC = child_sets_controller_vars->fade;
        if (child_sets_controller_vars->fade == 0.0f) {
            child_sets_controller->state = 5;
        }
        break;
    }
}

#include "rnc/overlay/l11/mobys/child_sets_controller_globals.h"

extern float child_set_base_z_offset_gp __asm__("D_L11_00161EAC") __attribute__((sda));

#include "rnc/overlay/l11/mobys/child_sets_controller_data/update_child_set_poses_3.h"

extern float D_L11_00161EB4 __attribute__((sda));

#include "rnc/overlay/l11/mobys/child_sets_controller_data/update_child_set_poses_2.h"

extern float child_set_base_rotation_y_degrees_gp __asm__("D_L11_00161EBC") __attribute__((sda));

#include "rnc/overlay/l11/mobys/child_sets_controller_data/update_child_set_poses.h"

extern float final_child_set_z_offset_gp __asm__("D_L11_00161F08") __attribute__((sda));

extern float fast_add_rotations(float, float);

/* Places the child moby sets spawned by FUN_L11_0030c788 around this one, opened by the progress values
   D_L11_00161EC4..ED8. */
void update_child_set_poses(struct Moby *parent_moby) {
    Vec4f child_placement_vector_scratch;
    Vec4f child_position_scratch;
    Vec4f parent_axis1_offset;
    int   i;
    int   j;
    int   attached_child_index;
    int   final_child_index;

    for (i = 0; i < 6; i++) {
        int   s = (i >= 3) ? 1 : -1;
        float ang =
            (float)(i % 3) * 0.5235988f -
            (child_set_fold_rotation_x_degrees_gp * (1.0f - D_L11_00161EC4) + 30.0f) * 0.017453292f;
        qcopy_nc(&l11_three_per_side_child_mobys[i]->rot, &parent_moby->rot);
        l11_three_per_side_child_mobys[i]->rot.z =
            fast_add_rotations(l11_three_per_side_child_mobys[i]->rot.z, (float)s * 1.5707964f);
        l11_three_per_side_child_mobys[i]->rot.y =
            child_set_base_rotation_y_degrees_gp * 0.017453292f;
        l11_three_per_side_child_mobys[i]->rot.x = ang;
        euler_rotation_basis(&l11_three_per_side_child_mobys[i]->rot_row0,
                             &l11_three_per_side_child_mobys[i]->rot);
        clear_u64_value(&child_placement_vector_scratch);
        fast_vec_normalize(&child_placement_vector_scratch, &parent_moby->rot_row0, 14.0f);
        child_placement_vector_scratch.z += child_set_base_z_offset_gp;
        fast_vec_add(
            &child_placement_vector_scratch, &child_placement_vector_scratch, &parent_moby->pos);
        fast_vec_normalize(&child_position_scratch,
                           &l11_three_per_side_child_mobys[i]->rot_row2,
                           child_set_child_axis2_offset_length_gp);
        fast_vec_normalize(&parent_axis1_offset, &parent_moby->rot_row1, (float)s * D_L11_00161EB4);
        fast_vec_add(&child_position_scratch, &child_position_scratch, &parent_axis1_offset);
        fast_vec_add(
            &child_position_scratch, &child_position_scratch, &child_placement_vector_scratch);
        qcopy_nc(&l11_three_per_side_child_mobys[i]->pos, &child_position_scratch);
        update_entity(l11_three_per_side_child_mobys[i]);
    }
    for (j = 0; j < 4; j++) {
        int   s = (j >= 2) ? 1 : -1;
        int   k = (j & 1) ? -1 : 1;
        float ang =
            (float)(j % 2) * 0.5235988f -
            (child_set_fold_rotation_x_degrees_gp * (1.0f - D_L11_00161EC4) + 15.0f) * 0.017453292f;
        float r;
        qcopy_nc(&D_L11_00161F20[j]->rot, &parent_moby->rot);
        D_L11_00161F20[j]->rot.z =
            fast_add_rotations(D_L11_00161F20[j]->rot.z, (float)s * 1.5707964f);
        D_L11_00161F20[j]->rot.y = child_set_base_rotation_y_degrees_gp * 0.017453292f;
        D_L11_00161F20[j]->rot.x = ang;
        euler_rotation_basis(&D_L11_00161F20[j]->rot_row0, &D_L11_00161F20[j]->rot);
        clear_u64_value(&child_placement_vector_scratch);
        fast_vec_normalize(&child_placement_vector_scratch, &parent_moby->rot_row0, 14.0f);
        child_placement_vector_scratch.z += child_set_base_z_offset_gp;
        fast_vec_add(
            &child_placement_vector_scratch, &child_placement_vector_scratch, &parent_moby->pos);
        fast_vec_normalize(&child_position_scratch,
                           &D_L11_00161F20[j]->rot_row2,
                           child_set_child_axis2_offset_length_gp);
        fast_vec_normalize(&parent_axis1_offset, &parent_moby->rot_row1, (float)s * D_L11_00161EB4);
        fast_vec_add(&child_position_scratch, &child_position_scratch, &parent_axis1_offset);
        fast_vec_add(
            &child_position_scratch, &child_position_scratch, &child_placement_vector_scratch);
        qcopy(&D_L11_00161F20[j]->pos, &child_position_scratch);
        r = fast_subtract_rotations((float)k * 0.2617994f, 1.3962634f);
        r = r * ((k == -1) ? D_L11_00161EC8 : D_L11_00161ECC);
        r = fast_add_rotations(r, 1.3962634f);
        D_L11_00161F20[j]->rot.x = fast_add_rotations(D_L11_00161F20[j]->rot.x, r);
        update_entity(D_L11_00161F20[j]);
    }
    for (attached_child_index = 0; attached_child_index < 4; attached_child_index++) {
        float rotation_y_progress;
        fast_vec_normalize(&child_placement_vector_scratch,
                           &D_L11_00161F20[attached_child_index]->rot_row2,
                           attached_child_source_axis2_offset_length_gp);
        fast_vec_normalize(&child_position_scratch,
                           &D_L11_00161F20[attached_child_index]->rot_row1,
                           attached_child_source_axis1_offset_length_gp);
        fast_vec_add(&child_placement_vector_scratch,
                     &child_placement_vector_scratch,
                     &child_position_scratch);
        fast_vec_add(&D_L11_00161F30[attached_child_index]->pos,
                     &D_L11_00161F20[attached_child_index]->pos,
                     &child_placement_vector_scratch);
        qcopy(&D_L11_00161F30[attached_child_index]->rot,
              &D_L11_00161F20[attached_child_index]->rot);
        rotation_y_progress = (attached_child_index & 1) ? D_L11_00161ED0 : D_L11_00161ED4;
        D_L11_00161F30[attached_child_index]->rot.y = fast_add_rotations(
            D_L11_00161F30[attached_child_index]->rot.y,
            -(attached_child_rotation_y_degrees_gp * 0.017453292f) * rotation_y_progress);
        update_entity(D_L11_00161F30[attached_child_index]);
    }
    for (final_child_index = 0; final_child_index < 4; final_child_index++) {
        float sgn;
        float signed_axis0_offset;
        sgn = -1.0f;
        if (final_child_index >= 2) {
            sgn = 1.0f;
        }
        signed_axis0_offset =
            (float)(final_child_index & 1) * 7.5f + 3.75f + (1.0f - D_L11_00161ED8) * 13.25f;
        if (20.0f < signed_axis0_offset) {
            signed_axis0_offset = 20.0f;
        }
        signed_axis0_offset *= sgn;
        fast_vec_normalize(
            &child_placement_vector_scratch, &parent_moby->rot_row0, signed_axis0_offset + 14.0f);
        child_placement_vector_scratch.z = final_child_set_z_offset_gp;
        fast_vec_add(&D_L11_00161F10[final_child_index]->pos,
                     &child_placement_vector_scratch,
                     &parent_moby->pos);
        qcopy_nc(&D_L11_00161F10[final_child_index]->rot, &parent_moby->rot);
        update_entity(D_L11_00161F10[final_child_index]);
    }
}
