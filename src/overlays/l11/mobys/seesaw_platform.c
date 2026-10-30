/* Level 11 moby classes: seesaw_platform, ammocrate, psytcopus, jet_fighter, puffoid. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/overlay/moby_anim.h"
#include "qcopy.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/usage_stats.h"
#include "rnc/game/item_state.h"
#include "sda.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/refresh_moby_spatial_bounds_from_basis.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/vendor/set_camera_rotation.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/ui/help/try_set_help_message.h"
#include "rnc/overlay/shared/props/pool_set_scale.h"
#include "rnc/overlay/shared/props/pool_init_tiles.h"

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
extern float        D_L11_0015F580[] MACRO_ADDR;
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
extern void FUN_0022da68_c788(int, int, struct Moby *) __asm__("FUN_0022da68");
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
            FUN_0022da68_c788(0, 0, child_sets_controller);
        }
        break;
    case CHILD_SETS_STATE_UNFOLD_AND_BLEND_POSES:
        if (D_L11_00161EC4 < 1.0f) {
            advance_accelerated_scalar(
                &D_L11_00161EC4, &D_L11_00161EE0, 1.0f, frame_time_sq, frame_time_sq, frame_time);
        }
        if (0.9f < D_L11_00161EC4 && D_L11_00161EC8 == 0.0f) {
            FUN_0022da68_c788(1, 0, child_sets_controller);
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
            FUN_0022da68_c788(1, 0, child_sets_controller);
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

#include "rnc/overlay/l11/mobys/seesaw_platform_globals.h"

extern float child_set_base_z_offset_gp __asm__("D_L11_00161EAC") __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_child_set_poses_3.h"

extern float D_L11_00161EB4 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_child_set_poses_2.h"

extern float child_set_base_rotation_y_degrees_gp __asm__("D_L11_00161EBC") __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_child_set_poses.h"

extern float final_child_set_z_offset_gp __asm__("D_L11_00161F08") __attribute__((sda));

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

extern int   D_L11_0015FFD8_d800 __asm__("D_L11_0015FFD8");
int          D_L11_00161F54 = 0;
extern float child_grid_pose_blend_acceleration_gp __asm__("D_L11_00161F58") __attribute__((sda));
extern float child_grid_pose_blend_deceleration_gp __asm__("D_L11_00161F5C") __attribute__((sda));
extern float child_grid_pose_blend_max_speed_gp __asm__("D_L11_00161F60") __attribute__((sda));
extern void  FUN_0022da68_d800(int, int, struct Moby *) __asm__("FUN_0022da68");
void         update_child_grid_poses(struct Moby *grid_controller) __asm__("FUN_L11_0030dd18");

void update_child_grid_poses(struct Moby *m) __asm__("FUN_L11_0030dd18");

/* Spawns the 4x6 child grid, waits for its linked moby, then fades and blends between two l11_cuboids
   points (pos at 0x30, rotation at 0x70) before handing back to the hero. */
void update_child_grid_controller(struct Moby *grid_controller) __asm__("FUN_L11_0030d800");

void update_child_grid_controller(struct Moby *grid_controller) {
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
    float          old;

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
            FUN_0022da68_d800(0, 0, grid_controller);
        } else if (link->command == 2) {
            grid_controller_vars->t = 1.0f;
            grid_controller->state = 5;
        }
        break;
    }
    case CHILD_GRID_STATE_FADE_TO_POINT_POSE:
        old = grid_controller_vars->fade;
        approach_value(&grid_controller_vars->fade, 1.0f, 1.0f / (float)scale_game_frames(20));
        D_L11_0015F3FC = 1.0f - absolute_float(grid_controller_vars->fade);
        if (old < 0.0f && 0.0f <= grid_controller_vars->fade) {
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
        old = grid_controller_vars->fade;
        approach_value(&grid_controller_vars->fade, 1.0f, 1.0f / (float)scale_game_frames(20));
        D_L11_0015F3FC = 1.0f - absolute_float(grid_controller_vars->fade);
        if (old < 0.0f && 0.0f <= grid_controller_vars->fade) {
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
    char *data;
    struct Moby *(*grid)[6];
    float progress;
    float hi;
    float lo;
    float row_angle_offset_degrees;
    float angle;
    int   row;
    int   col;

    data = grid_controller->pvars;
    grid = (struct Moby * (*)[6]) data;
    fast_vec_normalize(
        &parent_axis0_offset, &grid_controller->rot_row0, child_grid_parent_axis0_offset_length_gp);
    fast_vec_normalize(
        &parent_axis1_offset, &grid_controller->rot_row1, child_grid_parent_axis1_offset_length_gp);
    progress = *(float *)(data + 0x6C);
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

/* D_L11_001DA880 rows (0x1190 bytes each); see FUN_L11_0030e730 and FUN_L11_0030e088 */
typedef struct {
    float          x;
    float          y;
    float          z;
    unsigned char  n[8];
    int            unk14;
    int            unk18;
    unsigned char  unk1C;
    unsigned char  unk1D;
    unsigned short unk1E;
    char           rest[0x1170];
} Ent;

extern Ent D_L11_001DA880[];
void populate_water_tile_neighbor_indices(int selected_tile_index) __asm__("FUN_L11_0030e730");

void populate_water_tile_neighbor_indices(int arg) __asm__("FUN_L11_0030e730");

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28;
    s32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8  unk3C;
    u8  unk3D;
} Light_e088;

extern Light_e088 l11_water_tile_light __asm__("D_L11_001CB280");
int               D_L11_00161F64 = 0;
char              D_L11_0021B4C0[] =
    "I'm really sorry, but you can't have more than 128 water tiles in this level";
extern char  water_tile_binding_records[] __asm__("D_L11_001DA740");
extern float water_light_heading_reference[] __asm__("D_L11_001808C0");
extern void *D_L11_001612D0;
extern void *l11_water_tile_binding_record_base __asm__("D_L11_001612D4");
extern int   D_L11_001612D8;
extern float D_L11_0016120C;
extern float D_L11_00161210;
extern float D_L11_00161214;
extern int   D_L11_00161208;
extern void  print_e088(char *) __asm__("FUN_001e93b0");
extern void  set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern void  synchronize_linked_moby_heights(struct Moby *) __asm__("FUN_L11_0030e690");
extern void  pool_splash(float, float, float, float, void *, int, int) __asm__("FUN_L00_002a3ec8");
extern int   check_scaled_vector_bsphere(float, void *) __asm__("FUN_L01_00275690");
extern void  render_level_11_water_grids(void) __asm__("FUN_L11_0030e708");
extern void  add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Tile moby: claims a D_L11_001DA880 row on spawn (row 0 also sets up the shared light block), then moves
   up or down once its switch fires and throws random sparks; row 0 flushes the visible rows each frame. */
void update_water_tile(struct Moby *water_tile_moby) __asm__("FUN_L11_0030e088");

void update_water_tile(struct Moby *water_tile_moby) {
    enum {
        WATER_TILE_STATE_INITIALIZE = 0,
        WATER_TILE_STATE_FIND_NEIGHBORS = 1,
        WATER_TILE_STATE_APPROACH_UPPER_HEIGHT = 2,
        WATER_TILE_STATE_APPROACH_LOWER_HEIGHT = 3
    };
    int  *water_tile_vars = (int *)water_tile_moby->pvars;
    int   i;
    float tile_query_has_result;

    switch (water_tile_moby->state) {
    case WATER_TILE_STATE_INITIALIZE:
        water_tile_moby->update_dist = 0xFF;
        water_tile_vars[1] = D_L11_00161F64;
        D_L11_00161F64 = D_L11_00161F64 + 1;
        if (D_L11_00161F64 >= 0x80) {
            print_e088(D_L11_0021B4C0);
        }
        water_tile_moby->update_dist = 0xFF;
        water_tile_moby->flags |= 1;
        water_tile_moby->scale = water_tile_moby->pclass->scale + water_tile_moby->pclass->scale;
        if (water_tile_vars[1] == 0) {
            Light_e088 *water_tile_light;
            float       light_direction_angle;
            for (i = 19; i >= 0; i--) {
                D_L11_001DA880[i].unk1E = 0;
            }
            pool_init_tiles(D_L11_001DA880, 20);
            pool_set_scale(1.0f);
            water_tile_light = &l11_water_tile_light;
            water_tile_light->unk0 = 16.0f;
            water_tile_light->unk4 = 16.0f;
            water_tile_light->unk8 = -8.0f;
            water_tile_light->unkC = -8.0f;
            water_tile_light->unk10 = 1.0f;
            water_tile_light->unk14 = 1.0f;
            water_tile_light->unk18 = 0.9f;
            water_tile_light->unk20 = 0.1f;
            light_direction_angle =
                fast_arc_tan(water_light_heading_reference[4], water_light_heading_reference[5]);
            water_tile_light->unk30 = fast_cos(light_direction_angle) * 0.57735f;
            water_tile_light->unk34 = fast_sin(light_direction_angle) * 0.57735f;
            water_tile_light->unk38 = -0.57735f;
            water_tile_light->unk28 = 0x38;
            water_tile_light->unk2C = 0x39;
            water_tile_light->unk3C = 0x1C;
            water_tile_light->unk3D = 0x40;
            D_L11_001612D0 = D_L11_001DA880;
            l11_water_tile_binding_record_base = water_tile_binding_records;
            D_L11_001612D8 = 20;
            D_L11_0016120C = 32768.0f;
            D_L11_00161210 = 255.0f;
            D_L11_00161214 = 48.0f;
            D_L11_00161208 = 0;
        }
        D_L11_001DA880[water_tile_vars[1]].x = water_tile_moby->pos.x;
        D_L11_001DA880[water_tile_vars[1]].y = water_tile_moby->pos.y;
        D_L11_001DA880[water_tile_vars[1]].z = water_tile_moby->pos.z;
        D_L11_001DA880[water_tile_vars[1]].unk14 = 0x38;
        D_L11_001DA880[water_tile_vars[1]].unk18 = 0x39;
        D_L11_001DA880[water_tile_vars[1]].unk1C = 0x1C;
        D_L11_001DA880[water_tile_vars[1]].unk1D = 0x44;
        D_L11_001DA880[water_tile_vars[1]].unk1E = 0xFFFF;
        pool_bind_tile(&D_L11_001DA880[water_tile_vars[1]],
                       water_tile_binding_records + water_tile_vars[1] * 16);
        water_tile_moby->state = 1;
        break;
    case WATER_TILE_STATE_FIND_NEIGHBORS:
        D_L11_00161F64 = 0;
        populate_water_tile_neighbor_indices(water_tile_vars[1]);
        water_tile_moby->state = 2;
        break;
    case WATER_TILE_STATE_APPROACH_LOWER_HEIGHT:
        if (water_tile_moby->command & 4) {
            water_tile_vars[2] = 1;
            set_moby_list_state(water_tile_moby->group, 2);
        }
        water_tile_moby->command = 1;
        if (water_tile_vars[2] != 0) {
            advance_accelerated_scalar(&water_tile_moby->pos.z,
                                       water_tile_vars,
                                       132.36f,
                                       frame_time_sq * 4.0f,
                                       frame_time_sq * 4.0f,
                                       frame_time * 4.0f);
            synchronize_linked_moby_heights(water_tile_moby);
        }
        if (random_integer_below(200) == 0) {
            float x = D_L11_001DA880[water_tile_vars[1]].x + random_float_between(-4.0f, 4.0f);
            float y = random_float_between(-4.0f, 4.0f);
            Ent  *t = &D_L11_001DA880[water_tile_vars[1]];
            pool_splash(x, t->y + y, 1.0f, -0.03f, t, 1, 1);
        }
        break;
    case WATER_TILE_STATE_APPROACH_UPPER_HEIGHT:
        if (water_tile_moby->command & 8) {
            water_tile_vars[2] = 1;
            set_moby_list_state(water_tile_moby->group, 3);
        }
        water_tile_moby->command = 2;
        if (water_tile_vars[2] != 0) {
            advance_accelerated_scalar(&water_tile_moby->pos.z,
                                       water_tile_vars,
                                       137.0f,
                                       frame_time_sq * 4.0f,
                                       frame_time_sq * 4.0f,
                                       frame_time * 4.0f);
            synchronize_linked_moby_heights(water_tile_moby);
        }
        if (random_integer_below(200) == 0) {
            float x = D_L11_001DA880[water_tile_vars[1]].x + random_float_between(-4.0f, 4.0f);
            float y = random_float_between(-4.0f, 4.0f);
            Ent  *t = &D_L11_001DA880[water_tile_vars[1]];
            pool_splash(x, t->y + y, 1.0f, -0.03f, t, 1, 1);
        }
        break;
    }
    if (water_tile_vars[1] >= 0) {
        tile_query_has_result = 0.0f;
        if (check_scaled_vector_bsphere(64.0f, water_tile_moby) != -1) {
            tile_query_has_result = 1.0f;
        }
        if (tile_query_has_result == 0.0f) {
            D_L11_001DA880[water_tile_vars[1]].unk1E = 0;
        } else {
            D_L11_001DA880[water_tile_vars[1]].unk1E = 0xFFFF;
        }
        if (water_tile_vars[1] == 0) {
            pool_flush(D_L11_001DA880, 20);
            add_draw_callback(render_level_11_water_grids, water_tile_moby);
        }
    }
}
/* Level overlay code; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

/* records, for entry arg, which of the 48 entries sit one grid step away */
/* Same routine as FUN_L05_0030d6a0, with this level's table and entry count. */

void populate_water_tile_neighbor_indices(int selected_tile_index) {
    int i;
    D_L11_001DA880[selected_tile_index].n[0] = 0xFF;
    D_L11_001DA880[selected_tile_index].n[2] = 0xFF;
    D_L11_001DA880[selected_tile_index].n[1] = 0xFF;
    D_L11_001DA880[selected_tile_index].n[3] = 0xFF;
    D_L11_001DA880[selected_tile_index].n[4] = 0xFF;
    D_L11_001DA880[selected_tile_index].n[5] = 0xFF;
    D_L11_001DA880[selected_tile_index].n[6] = 0xFF;
    D_L11_001DA880[selected_tile_index].n[7] = 0xFF;
    for (i = 0; i < 20; i++) {
        if (selected_tile_index != i) {
            Ent  *tile_table = D_L11_001DA880;
            Ent  *selected_tile = &tile_table[selected_tile_index];
            float x = selected_tile->x - tile_table[i].x;
            float y = selected_tile->y - tile_table[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    selected_tile->n[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    selected_tile->n[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    selected_tile->n[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    selected_tile->n[3] = i;
            } else {
                if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_tile->n[5] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_tile->n[7] = i;
                } else if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_tile->n[4] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_tile->n[6] = i;
                }
            }
        }
    }
}

#include "qcopy.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/usage_stats.h"
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_0030FE40), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x57];
    char f57;
} Rec57;

extern float fast_add_rotations(float, float);
extern int   D_L11_0015FFD8; /* no foreign declaration */
extern int   allocate_voice_for_target_entry();
extern void  func_0022ED80_u(int, int, void *) __asm__("FUN_0022da68");
extern char  D_0013D355_t[] __asm__("D_0013D388");

void update_link_triggered_moby_motion(struct Moby *moving_moby) __asm__("FUN_L11_0030e978");

void update_link_triggered_moby_motion(struct Moby *moving_moby) {
    enum {
        LINKED_MOTION_STATE_INITIALIZE = 0,
        LINKED_MOTION_STATE_WAIT_FOR_LINK_TRIGGER = 1,
        LINKED_MOTION_STATE_MOVE_ALONG_LOCAL_AXIS = 2
    };
    char             *private_data = (char *)moving_moby->pvars;
    float             motion_displacement[4] __attribute__((aligned(16)));
    struct Moby      *spawned_moby;
    struct Moby      *linked_moby;
    char             *spawned_private_data;
    struct MobyClass *source_moby_class;
    switch (moving_moby->state) {
    case LINKED_MOTION_STATE_INITIALIZE:
        moving_moby->pos.z = moving_moby->pos.z + *(float *)(private_data + 0xC) * 1.9f;
        spawned_moby = (struct Moby *)create_moby(0x487);
        spawned_moby->visible = 1;
        spawned_moby->draw_dist = 0x40;
        spawned_moby->spawn_frame = moving_moby->spawn_frame;
        spawned_moby->flags = moving_moby->flags;
        qcopy(&spawned_moby->pos, &moving_moby->pos);
        qcopy(&spawned_moby->rot, &moving_moby->rot);
        spawned_moby->rot.x = fast_add_rotations(spawned_moby->rot.x, 3.1415927f);
        update_entity(spawned_moby);
        spawned_private_data = (char *)spawned_moby->pvars;
        *(int *)(spawned_private_data + 8) = *(int *)(private_data + 8);
        *(float *)(spawned_private_data + 0x10) = *(float *)(private_data + 0x10);
        *(int *)(spawned_private_data + 0x14) = *(int *)(private_data + 0x14);
        source_moby_class = moving_moby->pclass;
        moving_moby->scale = source_moby_class->scale * *(float *)(private_data + 0xC);
        spawned_moby->scale = source_moby_class->scale * *(float *)(private_data + 0xC);
        spawned_moby->state = 1;
        moving_moby->state = 1;
        break;
    case LINKED_MOTION_STATE_WAIT_FOR_LINK_TRIGGER:
        linked_moby = (struct Moby *)((*(int *)(private_data + 8) << 8) + (int)D_L11_0015FFD8);
        if (linked_moby->oclass == 0x267 && linked_moby->state == 4) {
            goto go;
        }
        if (linked_moby->oclass == 0x33E && linked_moby->command != 0) {
        go:
            moving_moby->state = 2;
            ((Rec57 *)(*(int *)(private_data + 0x14) + (int)(D_0013D355_t)))->f57 = 1;
            func_0022ED80_u(0, 0, moving_moby);
        }
        break;
    case LINKED_MOTION_STATE_MOVE_ALONG_LOCAL_AXIS:
        advance_accelerated_scalar((float *)private_data,
                                   (float *)(private_data + 4),
                                   *(float *)(private_data + 0x10),
                                   frame_time_sq,
                                   frame_time_sq,
                                   frame_time + frame_time);
        fast_vec_normalize(
            motion_displacement, &moving_moby->rot_row1, -*(float *)(private_data + 4));
        fast_vec_add(&moving_moby->pos, &moving_moby->pos, motion_displacement);
        break;
    }
}

extern float seesaw_hero_local_x_scale_gp __asm__("D_L11_00161F68") __attribute__((sda));
extern float seesaw_tilt_feedback_scale_gp __asm__("D_L11_00161F6C") __attribute__((sda));
extern float seesaw_spin_feedback_scale_gp __asm__("D_L11_00161F70") __attribute__((sda));
extern float seesaw_rider_spin_change_bias_degrees_gp __asm__("D_L11_00161F74")
    __attribute__((sda));
extern float seesaw_rider_spin_change_gain_degrees_gp __asm__("D_L11_00161F78")
    __attribute__((sda));
extern float seesaw_tilt_feedback_gain_degrees_gp __asm__("D_L11_00161F7C") __attribute__((sda));
extern float seesaw_spin_feedback_gain_degrees_gp __asm__("D_L11_00161F80") __attribute__((sda));
extern float seesaw_fall_tilt_threshold_degrees_gp __asm__("D_L11_00161F84") __attribute__((sda));
extern int   hero_set_state(int, int) __asm__("FUN_L11_0024db50");

/* Private variables of the tilting platform (seesaw_platform_update). */
struct L11TiltVars {
    u8  pad0[0x20];
    u8  unk20[0x3C]; /* 0x20: rider state for update_entity_matrix */
    s32 unk5C;       /* 0x5C */
    f32 spin;        /* 0x60: rot.y speed */
};

/* Tilting platform: rot.y springs back and is damped through its spin; the hero standing on it tilts it toward
   his side, and a tilt past D_L11_00161F84 degrees knocks him into hero state 6. */
void seesaw_platform_update(struct Moby *m) __asm__("FUN_L11_0030eb90");

void seesaw_platform_update(struct Moby *platform) {
    Vec4f               platform_translation_delta;
    Vec4f               rot;
    float               matrix[16] __attribute__((aligned(16)));
    Vec4f               hero_local_offset;
    struct L11TiltVars *platform_vars = (struct L11TiltVars *)platform->pvars;
    float               spin;
    float               change;
    float               scaled;

    fast_vec_scale(&platform_translation_delta, &platform->pos, -1.0f);
    qcopy(&rot, &platform->rot);
    platform_vars->unk5C = 1;
    spin = platform_vars->spin;
    /* two separate updates (not one sum): gives retail's FP register allocation */
    platform_vars->spin = spin + seesaw_tilt_feedback_gain_degrees_gp * 0.017453292f *
                                     frame_time_sq *
                                     (seesaw_tilt_feedback_scale_gp * platform->rot.y);
    platform_vars->spin = platform_vars->spin + seesaw_spin_feedback_gain_degrees_gp *
                                                    0.017453292f * frame_time_sq *
                                                    (seesaw_spin_feedback_scale_gp * spin);
    if (hero.ground_moby == platform && hero.air_frames.s == 0) {
        fast_vec_sub(&hero_local_offset, &hero.motion.pos, &platform->pos);
        transpose_rotation_matrix(matrix, &platform->rot_row0);
        transform_vector(&hero_local_offset, &hero_local_offset, matrix);
        scaled = seesaw_hero_local_x_scale_gp * hero_local_offset.x;
        change = seesaw_rider_spin_change_gain_degrees_gp * 0.017453292f * frame_time_sq * scaled;
        if (platform_vars->spin * change < 0.0f && platform->rot.y * change < 0.0f) {
            change += change;
        }
        if (change < 0.0f) {
            change -= seesaw_rider_spin_change_bias_degrees_gp * 0.017453292f * frame_time_sq;
        } else {
            change += seesaw_rider_spin_change_bias_degrees_gp * 0.017453292f * frame_time_sq;
        }
        platform_vars->spin = platform_vars->spin + change;
        if (absolute_float(platform->rot.y) >
                seesaw_fall_tilt_threshold_degrees_gp * 0.017453292f &&
            (u32)hero.state.control_mode < 2) {
            hero_set_state(HERO_STATE_FALL, 1);
        }
    }
    platform->rot.y = fast_add_rotations(platform->rot.y, platform_vars->spin);
    fast_vec_add(&platform_translation_delta, &platform_translation_delta, &platform->pos);
    update_entity_matrix(platform_vars->unk20, &platform_translation_delta, &rot, &platform->rot);
}

/* Waits for its linked moby, then either hands off to a cutscene or turns toward its target. */
extern char *D_L11_0015FFD8_t __asm__("D_L11_0015FFD8");
extern int   level_frame __asm__("D_L11_0015F5CC");
extern char  D_0013D3E3_t[] __asm__("D_0013D3E3");
extern void  FUN_0022da68_t(int, int, unsigned char *) __asm__("FUN_0022da68");
extern float
approach_angle(float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");

void update_link_triggered_moby_turn(struct Moby *turning_moby) __asm__("FUN_L11_0030f2a8");

void update_link_triggered_moby_turn(struct Moby *turning_moby) {
    enum {
        LINKED_TURN_STATE_INITIALIZE = 0,
        LINKED_TURN_STATE_WAIT_FOR_LINK = 1,
        LINKED_TURN_STATE_ROTATE_TO_TARGET = 2
    };
    int    entry_state = turning_moby->state;
    float *private_data = (float *)turning_moby->pvars;
    float  target_angle;
    float  turn_acceleration_step;
    switch (entry_state) {
    case LINKED_TURN_STATE_INITIALIZE:
        turning_moby->update_dist = 0xFF;
        turning_moby->state = 1;
        private_data[2] = turning_moby->rot.z;
        break;
    case LINKED_TURN_STATE_WAIT_FOR_LINK:
        if (test_linked_moby_turn_trigger(D_L11_0015FFD8_t + (*(int *)private_data << 8))) {
            if (level_frame >= 11) {
                D_0013D3E3_t[0] = entry_state;
                turning_moby->state = 2;
                FUN_0022da68_t(0, 0, turning_moby);
            } else {
                turning_moby->state = 3;
                turning_moby->rot.z = fast_add_rotations(private_data[2], 1.0471976f);
            }
        }
        break;
    case LINKED_TURN_STATE_ROTATE_TO_TARGET:
        target_angle = fast_add_rotations(private_data[2], 1.0471976f);
        turn_acceleration_step = frame_time_sq * 0.34906584f;
        if (approach_angle(&turning_moby->rot.z,
                           target_angle,
                           private_data + 1,
                           turn_acceleration_step,
                           turn_acceleration_step,
                           frame_time * 0.7853982f) == 0.0f) {
            turning_moby->state = 3;
        }
        break;
    }
}

#include "qcopy.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/usage_stats.h"
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_00310A70), where it is exact; names translated to the US level program. */

char *spawn_moby_with_random_yaw(void *position, void *vector, int id) __asm__("FUN_L11_0030f5a8");

char *spawn_moby_with_random_yaw(void *position, void *vector, int id) {
    char *moby = create_moby(id);
    if (moby != 0) {
        char *data;
        float angle;
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        data = ((char *)MOBY(moby)->pvars);
        qcopy(moby + 0x10, position);
        qcopy(data + 0x10, vector);
        angle = random_angle_radians();
        *(int *)(moby + 0x40) = 0;
        MOBY(moby)->rot.z = angle;
        MOBY(moby)->rot.y = 0.7853982f;
        update_entity(moby);
        copy_hero_moby_field_38(moby);
    }
    return moby;
}

#include "sda.h"

#include "qzero.h"

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_00310B28), where it is exact; names translated to the US level program. */

extern char *FUN_L11_0030fd98_c(char *) __asm__("FUN_L11_0030fd98");
extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

int sync_ship_pickup_child_pose(struct Moby *owner, char *data) __asm__("FUN_L11_0030f660");

int sync_ship_pickup_child_pose(struct Moby *owner, char *data) {
    char *child;
    if (!*(char **)(data + 0x34)) {
        *(char **)(data + 0x34) = FUN_L11_0030fd98_c(owner);
        if (!*(char **)(data + 0x34))
            goto missing;
    }
    child = *(char **)(data + 0x34);
    if (*(unsigned char *)(child + 0x20) == 0xFE)
        goto missing;
    if (*(unsigned char *)(child + 0x20) == 0xFD)
        goto missing;
    qcopy(child + 0x10, &owner->pos);
    qcopy(child + 0x40, &owner->rot);
    update_entity(child);
    moby_point_position(*(char **)(data + 0x34), 0, data);
    fast_vec_sub(data + 0x20, *(char **)(data + 0x34) + 0x10, data);
    return 1;
missing:
    qcopy(data, &owner->pos);
    qzero(data + 0x20);
    return 0;
}

extern void FUN_L00_0024f7c8_f728(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void FUN_L00_0025e450_f728(void *,
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
extern void FUN_0022da68_f728(int, int, void *) __asm__("FUN_0022da68");
extern void jet_animate(void *, float) __asm__("FUN_L00_00259fe8");

extern struct Moby *D_L11_00174858 NOT_SDA;
/* Ship ammo / health pickup: falls and bounces, is pulled toward the hero's ship, and refills it on contact. */
void ammocrate_update(struct Moby *m) __asm__("FUN_L11_0030f728");

void ammocrate_update(struct Moby *pickup) {
    enum {
        AMMOCRATE_STATE_INITIALIZE = 0,
        AMMOCRATE_STATE_DAMPED_FALL = 1,
        AMMOCRATE_STATE_ATTRACT_TO_SHIP = 2,
        AMMOCRATE_STATE_FREE_FALL = 3,
        AMMOCRATE_STATE_COLLECTED = 4,
        AMMOCRATE_STATE_REMOVE = 5
    };
    enum {
        OCLASS_SHIP_AMMO_PICKUP = 0x4C2,
        OCLASS_SHIP_HEALTH_PICKUP = 0x4C4
    };
    struct Moby *ship = hero.ship_moby;
    char        *pickup_vars = pickup->pvars;
    float        ship_pickup_distance_metric = 100000.0f;
    float        mid = ship_pickup_distance_metric;
    int          taken = 0;
    Vec4f        pickup_vector_scratch;

    FUN_L00_0024f7c8_f728(pickup, 0, &pickup_vector_scratch);
    if (coll_sphere(0.55f, &pickup_vector_scratch, 0, pickup)) {
        struct Moby *collision_moby = D_L11_00174858;
        if (collision_moby != 0 &&
            (collision_moby->oclass == 0x4DA || collision_moby->oclass == 0x45 ||
             collision_moby->oclass == 0x563)) {
            taken = 1;
        } else {
            FUN_L00_0025e450_f728(pickup,
                                  pickup_vars + 0x10,
                                  &pickup_vector_scratch,
                                  0.0f,
                                  0.0f,
                                  3,
                                  2,
                                  5,
                                  0.0f,
                                  0.0f,
                                  9.0f,
                                  1.0f,
                                  -1,
                                  0.0f,
                                  0,
                                  1,
                                  -1,
                                  0);
            pickup->state = 5;
        }
    }
    if (pickup->state < 4) {
        if (hero.state.current == 0x32 && ship != 0 && ship->state != 0xFE && ship->state != 0xFD &&
            (ship->oclass == 0x4DA || ship->oclass == 0x45 || ship->oclass == 0x563)) {
            ship_pickup_distance_metric = fast_vec_dist(&ship->pos, &pickup->pos);
            mid = vector_distance_xy(&ship->pos, &pickup->pos);
            fast_vec_add(&pickup_vector_scratch, &pickup->pos, pickup_vars);
            fast_vec_scale(&pickup_vector_scratch, &pickup_vector_scratch, 0.5f);
            mid = vector_distance_xy(&ship->pos, &pickup_vector_scratch);
        }
        if (taken || ship_pickup_distance_metric < 4.0f || mid < 4.0f) {
            pickup->collision = 0;
            if (*(struct Moby **)(pickup_vars + 0x34) != 0) {
                (*(struct Moby **)(pickup_vars + 0x34))->collision = 0;
            }
            switch (pickup->oclass) {
            case OCLASS_SHIP_AMMO_PICKUP:
                hero.ship_ammo += 5;
                if (hero.ship_moby != 0 && hero.ship_moby->state != 0xFE &&
                    hero.ship_moby->state != 0xFD) {
                    FUN_0022da68_f728(4, 0, hero.ship_moby);
                }
                if (hero.ship_ammo_max < hero.ship_ammo) {
                    hero.ship_ammo = hero.ship_ammo_max;
                }
                pickup->state = 4;
                break;
            case OCLASS_SHIP_HEALTH_PICKUP:
                hero.ship_hp += hero.ship_hp_max * 0.25f;
                if (hero.ship_moby != 0 && hero.ship_moby->state != 0xFE &&
                    hero.ship_moby->state != 0xFD) {
                    FUN_0022da68_f728(5, 0, hero.ship_moby);
                }
                if (hero.ship_hp_max < hero.ship_hp) {
                    hero.ship_hp = hero.ship_hp_max;
                }
                pickup->state = 4;
                break;
            default:
                pickup->state = 4;
                break;
            }
        }
    }
    switch (pickup->state) {
    case AMMOCRATE_STATE_INITIALIZE:
        *(struct Moby **)(pickup_vars + 0x34) = 0;
        sync_ship_pickup_child_pose(pickup, pickup_vars);
        *(float *)(pickup_vars + 0x30) = 0.0f;
        pickup->state = 1;
        break;
    case AMMOCRATE_STATE_DAMPED_FALL:
        fast_vec_scale(pickup_vars + 0x10, pickup_vars + 0x10, 0.9f);
        *(float *)(pickup_vars + 0x18) -= frame_time_sq * 9.8f;
        fast_vec_add(pickup_vars, pickup_vars, pickup_vars + 0x10);
        fast_vec_add(&pickup->pos, pickup_vars, pickup_vars + 0x20);
        if (sync_ship_pickup_child_pose(pickup, pickup_vars)) {
            if ((*(struct Moby **)(pickup_vars + 0x34))->seq == 1) {
                fast_vec_scale(pickup_vars + 0x10, pickup_vars + 0x10, 0.8f);
                pickup->state = 2;
            }
        }
        break;
    case AMMOCRATE_STATE_ATTRACT_TO_SHIP: {
        float k = 0.8f;
        pickup->rot.z = fast_add_rotations(pickup->rot.z, frame_time * 0.034906585f);
        *(float *)(pickup_vars + 0x30) =
            fast_add_rotations(*(float *)(pickup_vars + 0x30), frame_time * 1.5707964f);
        pickup->rot.y = fast_cos(*(float *)(pickup_vars + 0x30)) * 0.7853982f;
        fast_vec_scale(pickup_vars + 0x10, pickup_vars + 0x10, k);
        *(float *)(pickup_vars + 0x18) -= frame_time_sq * 9.8f;
        if (ship_pickup_distance_metric < 40.0f && pickup->visible != 0) {
            k = k / square_root(ship_pickup_distance_metric);
            fast_vec_sub(&pickup_vector_scratch, &ship->pos, &pickup->pos);
            fast_vec_normalize(&pickup_vector_scratch, &pickup_vector_scratch, k);
            fast_vec_add(pickup_vars + 0x10, pickup_vars + 0x10, &pickup_vector_scratch);
        }
        fast_vec_add(pickup_vars, pickup_vars, pickup_vars + 0x10);
        fast_vec_add(&pickup->pos, pickup_vars, pickup_vars + 0x20);
        if (sync_ship_pickup_child_pose(pickup, pickup_vars)) {
            if ((*(struct Moby **)(pickup_vars + 0x34))->state == 2) {
                pickup->state = (*(struct Moby **)(pickup_vars + 0x34))->state;
            }
        }
        break;
    }
    case AMMOCRATE_STATE_FREE_FALL:
        *(float *)(pickup_vars + 0x18) -= frame_time_sq * 9.8f;
        fast_vec_add(&pickup->pos, &pickup->pos, pickup_vars + 0x10);
        break;
    case AMMOCRATE_STATE_COLLECTED:
        pickup->state = 5;
        break;
    case AMMOCRATE_STATE_REMOVE:
        mark_moby_for_removal(pickup);
        return;
    }
    if (pickup->pos.z < 20.0f) {
        mark_moby_for_removal(pickup);
        return;
    }
    if (pickup->pos.x < 20.0f) {
        pickup->pos.x = 20.0f;
    }
    if (1003.0f < pickup->pos.x) {
        pickup->pos.x = 1003.0f;
    }
    if (pickup->pos.y < 20.0f) {
        pickup->pos.y = 20.0f;
    }
    if (1003.0f < pickup->pos.y) {
        pickup->pos.y = 1003.0f;
    }
    if (pickup->pos.z < 20.0f) {
        pickup->pos.z = 20.0f;
    }
    if (1003.0f < pickup->pos.z) {
        pickup->pos.z = 1003.0f;
    }
    jet_animate(pickup, 0.5f);
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_00311260), where it is exact; names translated to the US level program. */

char *spawn_ship_pickup_child(struct Moby *owner) __asm__("FUN_L11_0030fd98");

char *spawn_ship_pickup_child(struct Moby *owner) {
    char *moby = create_moby(0x4C3);
    if (moby != 0) {
        char *data;
        data = ((char *)MOBY(moby)->pvars);
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        qcopy(moby + 0x10, &owner->pos);
        qcopy(moby + 0x40, &owner->rot);
        *(char **)(data + 0x70) = owner;
        if ((unsigned char)moby[0x53] != 2) {
            blend_moby_animation(moby, 2, 0xD, 0);
        }
        update_entity(moby);
        copy_hero_moby_field_38(moby);
    }
    return moby;
}

/* Runs the moby's hit timer: on a hit it either finishes the stun or counts it down and plays the hurt animation. */
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int   fptodp(float) __asm__("FUN_00120478");
extern void  print_fe50(char *, int, int) __asm__("FUN_001e93b0");
extern int fun_00213928(void *, void *, float *, int, int *, int, int, int) __asm__("FUN_00213928");
extern void sound_fe50(void *, void *) __asm__("FUN_L00_0025d458");
extern void update_fe50(void *, void *) __asm__("FUN_L00_0025d538");
char        D_L11_0021B530[] = "Moby class %d Damage Received: %f\n";

void handle_owner_linked_moby_hits(struct Moby   *moby,
                                   unsigned char *private_data,
                                   float         *health) __asm__("FUN_L11_0030fe50");

void handle_owner_linked_moby_hits(struct Moby *moby, unsigned char *private_data, float *health) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_DEATH = 1,
        HIT_REACTION_DEATH_KNOCKBACK = 2,
        HIT_REACTION_KNOCKBACK = 3,
        HIT_REACTION_KNOCKBACK_AND_CANCEL_PROJECTILE = 4,
        HIT_REACTION_NO_RESPONSE = 11
    };
    int   out;
    char *info;
    if (moby->state != 4) {
        info = find_moby_hit(moby, 0x330000, 0);
        if (info != 0) {
            print_fe50(D_L11_0021B530, moby->oclass, fptodp(*(float *)(info + 0x2C)));
        }
        switch (fun_00213928(moby, info, health, 0, &out, 0, 0, 4)) {
        case HIT_REACTION_DEATH:
        case HIT_REACTION_DEATH_KNOCKBACK:
            *health = 0.0f;
            break;
        case HIT_REACTION_NONE:
        case HIT_REACTION_KNOCKBACK:
        case HIT_REACTION_KNOCKBACK_AND_CANCEL_PROJECTILE:
        case HIT_REACTION_NO_RESPONSE:
            break;
        }
        if (out >= 2) {
            if (*health <= *(float *)(info + 0x2C)) {
                unsigned char *owner_moby;
                *health = 0.0f;
                moby->flags &= 0xEFFF;
                private_data[0x67] = 0x78;
                sound_fe50(moby, private_data + 0x60);
                if (moby->prev_seq != 2) {
                    blend_moby_animation(moby, 2, 0, 3);
                }
                owner_moby = *(unsigned char **)(private_data + 0x70);
                if (owner_moby != 0 && owner_moby[0x20] != 0xFE && owner_moby[0x20] != 0xFD) {
                    owner_moby[0x20] = 3;
                }
                moby->state = 3;
            } else {
                *health -= *(float *)(info + 0x2C);
                private_data[0x67] = 0xFA;
                *(short *)(private_data + 0x26) = scale_game_frames(0x3C);
                sound_fe50(moby, private_data + 0x60);
                if (moby->prev_seq != 4) {
                    blend_moby_animation(moby, 4, 0, 3);
                }
            }
        }
        moby->hit_slot = 0xFF;
    }
    update_fe50(moby, private_data + 0x60);
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_003114F0), where it is exact; names translated to the US level program. */

void handle_owner_linked_moby_hits(struct Moby   *m,
                                   unsigned char *d,
                                   float         *t) __asm__("FUN_L11_0030fe50");

void update_owner_linked_moby_state(struct Moby *moby) __asm__("FUN_L11_00310028");

void update_owner_linked_moby_state(struct Moby *moby) {
    enum {
        LINKED_MOBY_STATE_START_ANIMATION = 0,
        LINKED_MOBY_STATE_WAIT_FOR_ANIMATION_FLAG = 1,
        LINKED_MOBY_STATE_HANDLE_HITS = 2,
        LINKED_MOBY_STATE_FADE_OUT = 3,
        LINKED_MOBY_STATE_REMOVE = 4
    };
    char        *data = (char *)moby->pvars;
    char        *health_ptr = data + 0x20;
    struct Moby *owner_moby = *(unsigned char **)(data + 0x70);
    if (owner_moby == 0 || owner_moby->state == 0xFE || owner_moby->state == 0xFD) {
        if (moby->state < 3) {
            moby->state = 4;
        }
    }
    switch (moby->state) {
    case LINKED_MOBY_STATE_START_ANIMATION:
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, 0xF);
        }
        moby->state = 1;
        break;
    case LINKED_MOBY_STATE_WAIT_FOR_ANIMATION_FLAG:
        if (moby->anim_flags & 2) {
            moby->hit_slot = 0xFF;
            moby->state = 2;
        }
        break;
    case LINKED_MOBY_STATE_HANDLE_HITS:
        handle_owner_linked_moby_hits(moby, data, health_ptr);
        if ((moby->anim_flags & 2) && moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, 0xA);
        }
        break;
    case LINKED_MOBY_STATE_FADE_OUT:
        if (moby->alpha != 0) {
            moby->alpha--;
        }
        if (compute_interpolated_record_value(moby) > 13.5f) {
            moby->state = 4;
        }
        break;
    case LINKED_MOBY_STATE_REMOVE:
        mark_moby_for_removal(moby);
        break;
    }
}

/* pvars of the moby run by psytcopus_update (FUN_L11_00310698 does its hit handling) */
struct PokiPathVars {
    u8           pad_00[0x20];
    f32          health; /* 0x20 */
    s16          unk24;  /* 0x24 */
    u8           pad_26[2];
    u8           unk28; /* 0x28 */
    u8           pad_29;
    u8           unk2A; /* 0x2A */
    u8           pad_2B[0x2D];
    u8           unk58; /* 0x58 */
    u8           pad_59;
    u8           unk5A; /* 0x5A */
    u8           pad_5B[5];
    u8           unk60[7]; /* 0x60: passed to trooper_anim_update / trooper_anim_finish */
    u8           unk67;    /* 0x67 */
    u8           pad_68[8];
    u8           unk70[0x10]; /* 0x70: passed to trooper_start_knockback / mine_fall */
    f32          unk80;       /* 0x80 */
    f32          unk84;       /* 0x84 */
    f32          unk88;       /* 0x88 */
    f32          unk8C;       /* 0x8C */
    s32          unk90;       /* 0x90 */
    s32          unk94;       /* 0x94 */
    f32          unk98;       /* 0x98 */
    u8           pad_9C[0x11];
    u8           unkAD; /* 0xAD */
    u8           pad_AE[0x12];
    f32          unkC0; /* 0xC0 */
    f32          unkC4; /* 0xC4 */
    u8           pad_C8[8];
    Vec4f        unkD0; /* 0xD0 */
    u8           pad_E0[0x30];
    struct Moby *target; /* 0x110 */
    s32          unk114; /* 0x114 */
    u8           pad_118[0x18];
    s32          unk130; /* 0x130 */
    s32          unk134; /* 0x134 */
    u8           pad_138[4];
    s32          unk13C; /* 0x13C */
    f32          unk140; /* 0x140 */
    u8           pad_144[8];
    f32          unk14C; /* 0x14C */
    u8           pad_150[8];
    s32          unk158; /* 0x158 */
    u8           pad_15C[4];
    void        *unk160; /* 0x160 */
    u8           pad_164[0x7C];
    u8           unk1E0[0x10]; /* 0x1E0 */
};

extern struct Moby *D_001413D0 NOT_SDA;
extern char                    D_L11_001677C0[];

#include "rnc/overlay/level_path.h"

extern float psytcopus_projectile_forward_offset_length_gp __asm__("D_L11_00161FC8")
    __attribute__((sda));
extern float psytcopus_projectile_z_offset_gp __asm__("D_L11_00161FCC") __attribute__((sda));
void         handle_psytcopus_hits(struct Moby *m) __asm__("FUN_L11_00310698");
extern void  FUN_L00_00263ac8_0180(struct Moby *, int, void *, float) __asm__("FUN_L00_00263ac8");
extern void  FUN_L00_0025a120_p(struct Moby *) __asm__("FUN_L00_0025a120");
extern int   is_value_within_interpolated_window(struct Moby *, float) __asm__("FUN_00214cc8");
extern void *
spawn_class_511_moby_with_sparks(struct Moby *, void *, int) __asm__("FUN_L11_00318d10");
extern void set_moby_state_two_data_and_timer(void *, void *) __asm__("FUN_L11_00318e98");
extern int  FUN_L00_0025c698_p(struct Moby *, void *) __asm__("FUN_L00_0025c698");
extern int  FUN_L00_00259740_p(void *, void *, int) __asm__("FUN_L00_00259740");
extern void FUN_L00_00257470_p(struct Moby *, int, int) __asm__("FUN_L00_00257470");
extern void
FUN_L00_0025f3e8_p(struct Moby *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");

#include "rnc/overlay/l11/mobys/seesaw_platform_data/psytcopus_update.h"

extern float D_L11_00161F9C __attribute__((sda));
extern void set_moby_position_and_refresh_removal_timer(void *, void *) __asm__("FUN_L11_00318e78");
extern void FUN_L00_0025ab48_p(void *, float *, float *, float *) __asm__("FUN_L00_0025ab48");
extern void
FUN_L00_0025c558_p(struct Moby *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void FUN_L00_002371e0_p(void *) __asm__("FUN_L00_002371e0");
extern void FUN_L00_0025d458_p(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void FUN_L00_0025d538_p(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern int
FUN_L00_0025ff38_p(struct Moby *, void *, int, int, void *, int, float) __asm__("FUN_L00_0025ff38");

/* Path moby update: hit handling first, then a state machine that turns toward the player or its target,
   spawns and fires a projectile (pvars 0x160) and dies by explosion or by reaching the end of its path. */
void psytcopus_update(struct Moby *m) __asm__("FUN_L11_00310180");

void psytcopus_update(struct Moby *psytcopus) {
    enum {
        PSYTCOPUS_STATE_INITIALIZE = 0,
        PSYTCOPUS_STATE_FACE_PLAYER = 3,
        PSYTCOPUS_STATE_CREATE_PROJECTILE = 4,
        PSYTCOPUS_STATE_FIRE_PROJECTILE = 5,
        PSYTCOPUS_STATE_ATTACK_RECOVERY = 6,
        PSYTCOPUS_STATE_KNOCKBACK = 7,
        PSYTCOPUS_STATE_DEATH_KNOCKBACK = 8
    };
    struct PokiPathVars *psytcopus_vars = (struct PokiPathVars *)psytcopus->pvars;
    struct Moby         *target;
    Vec4f                projectile_vector_scratch;
    float                angle;
    s32                 *path;
    int                  knockback_result_flags;

    handle_psytcopus_hits(psytcopus);
    target = psytcopus_vars->target;
    FUN_L00_00263ac8_0180(psytcopus, 0, psytcopus_vars->unk1E0, 2.5f);
    if (psytcopus->visible != 0 && fast_vec_dist(&psytcopus->pos, D_L11_001677C0) < 28.0f) {
        FUN_L00_0025a120_p(psytcopus);
        psytcopus->shadow_range = 0x16;
    }
    switch (psytcopus->state) {
    case PSYTCOPUS_STATE_INITIALIZE:
        psytcopus_vars->unk24 = 3;
        psytcopus_vars->health = 3.0f;
        psytcopus_vars->unk28 = 2;
        psytcopus_vars->unk2A = 6;
        psytcopus_vars->unk58 = truncate_float_to_s32(8.0f);
        psytcopus_vars->unk5A = truncate_float_to_s32(8.0f);
        psytcopus->state = 3;
        if (psytcopus->prev_seq != 0) {
            blend_moby_animation(psytcopus, 0, 0, scale_game_frames(10));
        }
        if (psytcopus_vars->unk130 != 0) {
            psytcopus->state = 1;
            qcopy(&psytcopus->pos, (char *)(s32 *)l11_paths[psytcopus_vars->unk134] + 0x10);
            break;
        }
        psytcopus->state = 3;
        if (psytcopus->prev_seq != 0) {
            blend_moby_animation(psytcopus, 0, 0, scale_game_frames(10));
        }
        break;
    case PSYTCOPUS_STATE_FACE_PLAYER: {
        float *yaw = &psytcopus->rot.z;
        angle = fast_arc_tan(D_001413D0->pos.x - psytcopus->pos.x,
                             D_001413D0->pos.y - psytcopus->pos.y);
        approach_angle(yaw,
                       angle,
                       &psytcopus_vars->unk14C,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 6.2831855f);
    }
        if (psytcopus_vars->unk114 != 2) {
            psytcopus->state = 4;
            if (psytcopus->prev_seq != 2) {
                blend_moby_animation(psytcopus, 2, 0, scale_game_frames(10));
            }
        }
        break;
    case PSYTCOPUS_STATE_CREATE_PROJECTILE: {
        float *yaw = &psytcopus->rot.z;
        angle = fast_arc_tan(target->pos.x - psytcopus->pos.x, target->pos.y - psytcopus->pos.y);
        approach_angle(yaw,
                       angle,
                       &psytcopus_vars->unk14C,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 6.2831855f);
    }
        if (is_value_within_interpolated_window(psytcopus, 15.0f) != 0) {
            fast_vec_normalize(&projectile_vector_scratch,
                               &psytcopus->rot_row0,
                               psytcopus_projectile_forward_offset_length_gp);
            fast_vec_add(&projectile_vector_scratch, &projectile_vector_scratch, &psytcopus->pos);
            projectile_vector_scratch.z += psytcopus_projectile_z_offset_gp;
            psytcopus_vars->unk160 =
                spawn_class_511_moby_with_sparks(psytcopus, &projectile_vector_scratch, 5);
            break;
        }
        if (psytcopus->anim_flags & 2) {
            psytcopus->state = 5;
            if (psytcopus->prev_seq != 3) {
                blend_moby_animation(psytcopus, 3, 0, scale_game_frames(10));
            }
        }
        break;
    case PSYTCOPUS_STATE_FIRE_PROJECTILE: {
        float *yaw = &psytcopus->rot.z;
        angle = fast_arc_tan(target->pos.x - psytcopus->pos.x, target->pos.y - psytcopus->pos.y);
        approach_angle(yaw,
                       angle,
                       &psytcopus_vars->unk14C,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 6.2831855f);
    }
        if (psytcopus_vars->unk160 != 0 &&
            is_value_within_interpolated_window(psytcopus, 33.0f) != 0) {
            fast_vec_normalize(
                &projectile_vector_scratch, &psytcopus->rot_row0, frame_time * 30.0f);
            set_moby_state_two_data_and_timer(psytcopus_vars->unk160, &projectile_vector_scratch);
            psytcopus_vars->unk160 = 0;
        }
        if (psytcopus->anim_flags & 2) {
            psytcopus->state = 6;
            if (psytcopus->prev_seq != 4) {
                blend_moby_animation(psytcopus, 4, 0, scale_game_frames(10));
            }
        }
        break;
    case PSYTCOPUS_STATE_ATTACK_RECOVERY:
        if (psytcopus->anim_flags & 2) {
            psytcopus->state = 3;
            if (psytcopus->prev_seq != 0) {
                blend_moby_animation(psytcopus, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case PSYTCOPUS_STATE_KNOCKBACK:
        knockback_result_flags = FUN_L00_0025c698_p(psytcopus, psytcopus_vars->unk70);
        if (knockback_result_flags & 0x41) {
            path = (s32 *)l11_paths[psytcopus_vars->unk13C];
            if (FUN_L00_00259740_p(&psytcopus->pos, path + 4, *path) != 0) {
                psytcopus->state = 3;
                if (psytcopus->prev_seq != 0) {
                    blend_moby_animation(psytcopus, 0, 0, scale_game_frames(10));
                }
                break;
            }
            knockback_result_flags = 0x100;
        }
        if (knockback_result_flags & 0x120) {
            FUN_L00_00257470_p(psytcopus, 0, -1);
            FUN_L00_0025f3e8_p(psytcopus, &psytcopus->pos, -1, 0.5f, 10.0f);
            mark_moby_for_removal(psytcopus);
        }
        break;
    case PSYTCOPUS_STATE_DEATH_KNOCKBACK:
        if (FUN_L00_0025c698_p(psytcopus, psytcopus_vars->unk70) & 0x160) {
            FUN_L00_0025f3e8_p(psytcopus, &psytcopus->pos, -1, 1.0f, 10.0f);
            FUN_L00_00257470_p(psytcopus, 0, -1);
            mark_moby_for_removal(psytcopus);
        }
        break;
    }
}

/* Hit handling for the psytcopus_update moby: takes damage, picks the hurt or death reaction, keeps it on its path. */
void handle_psytcopus_hits(struct Moby *psytcopus) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_DEATH = 1,
        HIT_REACTION_DEATH_KNOCKBACK = 2,
        HIT_REACTION_KNOCKBACK = 3,
        HIT_REACTION_KNOCKBACK_AND_CANCEL_PROJECTILE = 4,
        HIT_REACTION_KNOCKBACK_WITH_TIMER_EXTENSION = 5,
        HIT_REACTION_KNOCKBACK_WITH_PROJECTILE_CANCELLATION_AND_TIMER_EXTENSION = 6,
        HIT_REACTION_KNOCKBACK_WITH_SOURCE_RELATIVE_HEADING = 7,
        HIT_REACTION_KNOCKBACK_WITH_SOURCE_RELATIVE_HEADING_AND_TIMER_EXTENSION = 8,
        HIT_REACTION_HURT_WITHOUT_KNOCKBACK = 9,
        HIT_REACTION_HURT_WITHOUT_STATE_CHANGE = 10,
        HIT_REACTION_NO_RESPONSE = 11
    };
    struct PokiPathVars *psytcopus_vars = (struct PokiPathVars *)psytcopus->pvars;
    Vec4f                projectile_and_hit_vector_scratch;
    int                  hits;
    float                damage;
    float                angle1;
    float                angle2;
    char                *info;
    s32                 *path;
    u32                  hit_reaction;
    float                source_to_psytcopus_heading;

    fast_dec_timer(&psytcopus_vars->unk158);
    psytcopus->scale = psytcopus->pclass->scale * D_L11_00161F9C;
    if (psytcopus_vars->unk160 != 0) {
        fast_vec_normalize(&projectile_and_hit_vector_scratch,
                           &psytcopus->rot_row0,
                           psytcopus_projectile_forward_offset_length_gp);
        fast_vec_add(&projectile_and_hit_vector_scratch,
                     &projectile_and_hit_vector_scratch,
                     &psytcopus->pos);
        projectile_and_hit_vector_scratch.z += psytcopus_projectile_z_offset_gp;
        set_moby_position_and_refresh_removal_timer(psytcopus_vars->unk160,
                                                    &projectile_and_hit_vector_scratch);
    }
    damage = 0.0f;
    info = find_moby_hit(psytcopus, 0x330000, 0);
    hit_reaction = fun_00213928(psytcopus, info, &psytcopus_vars->health, 0, &hits, &damage, 0, 4);
    if (hits != 1 && psytcopus->state != 8) {
        psytcopus_vars->health -= damage;
        if (psytcopus_vars->health <= 0.0f) {
            hit_reaction = 1;
        }
        psytcopus_vars->unk90 = truncate_float_to_s32(512.0f);
        psytcopus_vars->unk98 = 0.5f;
        psytcopus_vars->unk80 = psytcopus_knockback_base_gravity_gp * frame_time_sq;
        psytcopus_vars->unk84 = 0.0005f;
        psytcopus_vars->unk94 = 9;
        psytcopus_vars->unkAD = 0;
        switch (hit_reaction) {
        case HIT_REACTION_NONE:
        case HIT_REACTION_NO_RESPONSE:
            break;
        case HIT_REACTION_HURT_WITHOUT_KNOCKBACK:
        case HIT_REACTION_HURT_WITHOUT_STATE_CHANGE:
            psytcopus_vars->unk67 = 0xFA;
            break;
        case HIT_REACTION_KNOCKBACK:
        case HIT_REACTION_KNOCKBACK_AND_CANCEL_PROJECTILE:
        case HIT_REACTION_KNOCKBACK_WITH_TIMER_EXTENSION:
        case HIT_REACTION_KNOCKBACK_WITH_PROJECTILE_CANCELLATION_AND_TIMER_EXTENSION:
        case HIT_REACTION_KNOCKBACK_WITH_SOURCE_RELATIVE_HEADING:
        case HIT_REACTION_KNOCKBACK_WITH_SOURCE_RELATIVE_HEADING_AND_TIMER_EXTENSION:
            psytcopus_vars->unk88 = psytcopus_hurt_knockback_base_horizontal_speed_gp * frame_time;
            psytcopus_vars->unk8C = psytcopus_hurt_knockback_base_vertical_speed_gp * frame_time;
            psytcopus_vars->unkC0 = 3.0f;
            psytcopus_vars->unkC4 = 6.0f;
            source_to_psytcopus_heading =
                fast_arc_tan(psytcopus->pos.x - (*(struct Moby **)(info + 0x20))->pos.x,
                             psytcopus->pos.y - (*(struct Moby **)(info + 0x20))->pos.y);
            *(u128 *)&projectile_and_hit_vector_scratch = *(u128 *)(info + 0x10);
            angle1 = source_to_psytcopus_heading;
            FUN_L00_0025ab48_p(&projectile_and_hit_vector_scratch,
                               &angle1,
                               &psytcopus_vars->unk88,
                               &psytcopus_vars->unk8C);
            FUN_L00_0025c558_p(psytcopus, psytcopus_vars->unk70, 5, 1, 0, angle1);
            psytcopus->state = 7;
            psytcopus_vars->unk67 = 0x78;
            if (psytcopus_vars->unk160 != 0) {
                FUN_L00_002371e0_p(psytcopus_vars->unk160);
                psytcopus_vars->unk160 = 0;
            }
            psytcopus_vars->unk158 += scale_game_frames(60);
            break;
        case HIT_REACTION_DEATH:
        case HIT_REACTION_DEATH_KNOCKBACK:
            psytcopus->flags &= 0xEFFF;
            psytcopus_vars->unk80 = psytcopus_knockback_base_gravity_gp * frame_time_sq;
            psytcopus_vars->unk88 = psytcopus_death_knockback_base_horizontal_speed_gp * frame_time;
            psytcopus_vars->unk8C = psytcopus_death_knockback_base_vertical_speed_gp * frame_time;
            psytcopus_vars->unkC0 = 4.0f;
            psytcopus_vars->unkC4 = 10.0f;
            source_to_psytcopus_heading =
                fast_arc_tan(psytcopus->pos.x - (*(struct Moby **)(info + 0x20))->pos.x,
                             psytcopus->pos.y - (*(struct Moby **)(info + 0x20))->pos.y);
            *(u128 *)&projectile_and_hit_vector_scratch = *(u128 *)(info + 0x10);
            angle2 = source_to_psytcopus_heading;
            FUN_L00_0025ab48_p(&projectile_and_hit_vector_scratch,
                               &angle2,
                               &psytcopus_vars->unk88,
                               &psytcopus_vars->unk8C);
            FUN_L00_0025c558_p(psytcopus, psytcopus_vars->unk70, 6, 1, 0, angle2);
            psytcopus->state = 8;
            psytcopus_vars->unk67 = 0xF0;
            if (psytcopus_vars->unk160 != 0) {
                FUN_L00_002371e0_p(psytcopus_vars->unk160);
                psytcopus_vars->unk160 = 0;
            }
            FUN_L00_00257470_p(psytcopus, 0, -1);
            break;
        }
        FUN_L00_0025d458_p(psytcopus, psytcopus_vars->unk60);
    }
    psytcopus->hit_slot = 0xFF;
    FUN_L00_0025d538_p(psytcopus, psytcopus_vars->unk60);
    psytcopus_vars->unk140 = 24.0f;
    path = (s32 *)l11_paths[psytcopus_vars->unk13C];
    if (FUN_L00_0025ff38_p(
            psytcopus, &psytcopus_vars->unkD0, 0, 0, path + 4, *path, psytcopus_vars->unk140) !=
            2 &&
        (psytcopus_vars->unk140 < vector_distance_xy(&psytcopus->pos, &psytcopus_vars->unkD0) ||
         absolute_float(psytcopus->pos.z - psytcopus_vars->unkD0.z) > 3.0f)) {
        psytcopus_vars->unk114 = 2;
    }
    if (psytcopus_vars->target == 0) {
        psytcopus_vars->target = D_001413D0;
    }
}

#include "sda.h"
#include "qcopy.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/usage_stats.h"
#include "sda.h"
#include "qzero.h"

typedef struct {
    short tex;
    short u;
    short v;
    short w;
    short h;
    short fA;
    short x;
    short y;
} IconEC5D0;

extern IconEC5D0     l11_ship_radar_icon_definitions[] __asm__("D_L11_001F1058");
extern s64           get_effect_texture(s32) __asm__("func_001F44B8");
extern unsigned char frame_size[] __asm__("D_0013E504");
extern unsigned char D_0015EDB4[4];
extern void
    draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64) __asm__("func_001F5450");

/* Draws a map icon for a moby relative to the hero, mirrored when the view is flipped. */
void draw_ship_radar_moby_icon(char *radar_reference_rotation,
                               char *from,
                               char *moby,
                               int   icon,
                               long  color) __asm__("FUN_L11_00311710");

void draw_ship_radar_moby_icon(

    char *radar_reference_rotation, char *from, char *moby, int icon, long color) {
    float      turn[4];
    float      radar_rotation_matrix[16];
    float      delta[4];
    float      radar_offset[4];
    float      radar_distance;
    float      side;
    int        x, y;
    IconEC5D0 *icon_definition;

    qzero(turn);
    turn[2] = *(float *)(radar_reference_rotation + 8);
    if (*(float *)(radar_reference_rotation + 4) > 1.5707964f ||
        *(float *)(radar_reference_rotation + 4) < -1.5707964f) {
        turn[2] = -*(float *)(radar_reference_rotation + 8);
    }
    turn[2] = fast_subtract_rotations(1.5707964f, turn[2]);
    euler_rotation_matrix(radar_rotation_matrix, turn);
    fast_vec_sub(delta, moby + 0x10, from);
    delta[2] = 0.0f;
    transform_vector(radar_offset, delta, radar_rotation_matrix);
    fast_vec_scale(radar_offset, radar_offset, 0.142857149f);
    radar_distance = fast_vec_length(radar_offset);
    side = 1.0f;
    if (D_0015EDB4[0] != 0) {
        side = -1.0f;
    }
    x = truncate_float_to_s32(side * radar_offset[0]);
    y = truncate_float_to_s32(-radar_offset[1]);
    if (radar_distance < 46.0f) {
        if (radar_distance > 38.0f) {
            float alpha_distance_product;
            radar_distance = 46.0f - radar_distance;
            alpha_distance_product =
                convert_integer_to_float(((unsigned long)color >> 24) & 0xFF) * radar_distance;
            color &= 0xFFFFFF;
            color = (truncate_float_to_s32(alpha_distance_product * 0.125f) << 24) | color;
        }
        icon_definition = &l11_ship_radar_icon_definitions[icon];
        y -= 0x50;
        y += *(int *)(frame_size);
        x += 0x1B0;
        draw_textured_quad(x - icon_definition->x,
                           y - icon_definition->y,
                           icon_definition->w,
                           icon_definition->h,
                           icon_definition->u,
                           icon_definition->v,
                           icon_definition->w,
                           icon_definition->h,
                           color,
                           get_effect_texture(icon_definition->tex));
    }
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00312E10), where it is exact; names translated to the US level program. */

extern float  compute_ship_target_reticle_distance_squared(void *,
                                                           void *) __asm__("FUN_L11_00311a88");
extern int    D_L11_0015FFD8; /* no foreign declaration */
extern int    collect_oil_tanker_chain_mobys(char *moby, void **out) __asm__("FUN_L11_00318050");
extern short *D_L11_001AC240[];
void          count_active_and_visible_listed_mobys(void *moby,
                                                    void *active_moby_count,
                                                    void *visible_moby_count) __asm__("FUN_L11_00311be8");

void *select_ship_lock_on_target(void  *moby,
                                 char  *ship_vars,
                                 float *reference_position,
                                 float *reference_rotation,
                                 float  unused_float_parameter_0,
                                 float  unused_float_parameter_1,
                                 float  unused_float_parameter_2,
                                 void  *current_target,
                                 int    unused_parameter) __asm__("FUN_L11_00311948");

void *select_ship_lock_on_target(void  *moby,
                                 char  *ship_vars,
                                 float *reference_position,
                                 float *reference_rotation,
                                 float  unused_float_parameter_0,
                                 float  unused_float_parameter_1,
                                 float  unused_float_parameter_2,
                                 void  *current_target,

                                 int unused_parameter) {
    void  *best = 0;
    float  bestd = 1000.0f;
    short *moby_list_entry;
    void  *list[12];
    count_active_and_visible_listed_mobys(moby, ship_vars + 0xE8, ship_vars + 0xEA);
    if (current_target != 0) {
        if (compute_ship_target_reticle_distance_squared(moby, current_target) < bestd)
            return current_target;
    }
    moby_list_entry = D_L11_001AC240[*(short *)(ship_vars + 0x108)];
    if (moby_list_entry == 0)
        return 0;
    do {
        char *listed_moby =
            (char *)(((*(unsigned short *)moby_list_entry & 0x7FFF) << 8) + D_L11_0015FFD8);
        if (listed_moby[0x20] >= 0) {
            int moby_count = collect_oil_tanker_chain_mobys(listed_moby, list);
            int i;
            for (i = 1; i < moby_count; i++) {
                float reticle_distance_sq =
                    compute_ship_target_reticle_distance_squared(moby, list[i]);
                if (reticle_distance_sq < bestd) {
                    best = list[i];
                    bestd = reticle_distance_sq;
                }
            }
        }
    } while (*moby_list_entry++ >= 0);
    return best;
}
/* squared distance from a point to a projected target, or a large value when out of range */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00312F50), where it is exact; names translated to the US level program. */

extern char ship_target_projection_offset_reference[] __asm__("D_L11_001679E0");
extern void
project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");

float compute_ship_target_reticle_distance_squared(void *moby_v, void *o_v) {
    struct Moby *moby = moby_v;
    char        *target_moby = o_v;
    char        *data = ((char *)moby->pvars);
    if (MOBY(target_moby)->visible != 0) {
        char target_projection_point[16];
        int  ax, ay, cx, cy;
        int  dx, dy, reticle_axis_limit;
        project_world_point_to_screen_pixels(target_moby + 0x10, &ax, &ay, 0);
        dx = ax - *(int *)(data + 0xE0);
        dy = ay - *(int *)(data + 0xE4);
        if (dx < 0)
            dx = -dx;
        if (dy < 0)
            dy = -dy;
        qcopy(target_projection_point, ship_target_projection_offset_reference);
        fast_vec_scale(target_projection_point,
                       ship_target_projection_offset_reference,
                       MOBY(target_moby)->bsphere.w / 1280.0f);
        fast_vec_add(target_projection_point, target_projection_point, target_moby + 0x10);
        project_world_point_to_screen_pixels(target_projection_point, &cx, &cy, 0);
        cx = cx - ax;
        cy = cy - ay;
        reticle_axis_limit =
            truncate_float_to_s32(square_root(convert_integer_to_float(cx * cx + cy * cy))) + 0x1C;
        if (dx < reticle_axis_limit && dy < reticle_axis_limit)
            return (float)(dx * dx + dy * dy);
    }
    return 10000.0f;
}

/* counts the listed mobys of the type that are active, and of those the ones with 0x31 set */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_003130B0), where it is exact; names translated to the US level program. */

extern int D_L11_0015FFD8; /* no foreign declaration */

void count_active_and_visible_listed_mobys(void *moby,
                                           void *active_moby_count,

                                           void *visible_moby_count) {
    char  *moby_private_data = *(char **)((char *)moby + 0x78);
    int    type = *(short *)(moby_private_data + 0x108);
    short *moby_list_entry;
    *(unsigned short *)active_moby_count = 0;
    *(unsigned short *)visible_moby_count = 0;
    moby_list_entry = D_L11_001AC240[type];
    if (moby_list_entry == 0)
        return;
    do {
        int listed_moby_byte_offset = (*(unsigned short *)moby_list_entry & 0x7FFF) << 8;
        if (((char *)D_L11_0015FFD8)[listed_moby_byte_offset + 0x20] >= 0) {
            (*(unsigned short *)active_moby_count)++;
            if (((unsigned char *)D_L11_0015FFD8)[listed_moby_byte_offset + 0x31] != 0) {
                (*(unsigned short *)visible_moby_count)++;
            }
        }
    } while (*moby_list_entry++ >= 0);
}
/* Keeps the moby's current target, picking a new one when it is lost and restarting the timer. */

int          lock_hold_frames __asm__("D_L11_00162160") = 500;
int          lock_fade_frames __asm__("D_L11_00162164") = 100;
extern float D_L11_001677D0[];

void update_ship_lock_on_target(void *self, char *moby) __asm__("FUN_L11_00311c80");

void update_ship_lock_on_target(void *self, char *moby) {
    float          reference_rotation_copy[4] __attribute__((aligned(16)));
    float         *reference_rotation_source;
    unsigned char *current_target;
    void          *selected_target;
    if (hero.ship_ammo == 0) {
        *(void **)(moby + 0x88) = 0;
    } else {
        current_target = *(unsigned char **)(moby + 0x88);
        if (current_target != 0) {
            if (current_target[0x20] == 0xFE)
                *(void **)(moby + 0x88) = 0;
            else if (current_target[0x20] == 0xFD)
                *(void **)(moby + 0x88) = 0;
        }
        reference_rotation_source = D_L11_001677D0;
        qcopy(reference_rotation_copy, reference_rotation_source);
        selected_target = select_ship_lock_on_target(self,
                                                     moby,
                                                     reference_rotation_source - 4,
                                                     reference_rotation_copy,
                                                     0.19634955f,
                                                     0.19634955f,
                                                     255.0f,
                                                     *(void **)(moby + 0x88),
                                                     0);
        if (selected_target != *(void **)(moby + 0x88)) {
            int initial_lock_timer_frames = lock_hold_frames + lock_fade_frames;
            *(void **)(moby + 0x88) = selected_target;
            *(int *)(moby + 0x8C) = initial_lock_timer_frames;
        } else if (selected_target != 0) {
            fast_dec_timer(moby + 0x8C);
        }
    }
}

#include "rnc/mobys/moby.h"
#include "rnc/game/usage_stats.h"
#include "sda.h"
#include "rnc/rendering/screen.h"

/* pvars of the space-level ship: lock-on, reticle and HUD state. */
typedef struct {
    u8           pad0[0x88];
    struct Moby *target; /* 0x88: lock-on candidate picked by FUN_L11_00311c80 */
    s32 lock_timer; /* 0x8C: counts down from lock_hold_frames + lock_fade_frames once targeted */
    u8  pad90[0x50];
    s32 reticle_x; /* 0xE0: screen position of the aim reticle (FUN_L11_003125a8) */
    s32 reticle_y; /* 0xE4 */
    u8  padE8[4];
    struct Moby *locked;      /* 0xEC: target once the lock completed */
    f32          shown_hp;    /* 0xF0: health bar value, eased towards hero.ship_hp */
    s32          hp_bar_fill; /* 0xF4: palette entries currently lit in the health bar */
    u8           padF8[0x10];
    s16          target_class; /* 0x108: index into the lock-on lists D_L11_001AC240 */
    u8           pad10A[0x12];
    s32          low_hp_frames;   /* 0x11C: frames spent on low health since the last hint */
    s32          ammo_hint_timer; /* 0x120: delay before the out-of-ammo hint repeats */
    u8           pad124[4];
    s32          hint_frames; /* 0x128: frames until help 0x56 shows */
} ShipVars;

/* Help message records, laid out like the usage counters: FUN_L00_00203908
   shows message a for record b while b's count is below 0xFFFF. The ship
   uses 0x56 (message 0x2AFD), 0x57 (0x2AFE, low ammo) and 0x58 (0x2AFF, low
   health). */
extern struct UsageStat help_records[] __asm__("D_00141968");
extern s32              D_0015EEA4;
extern short ship_ammo_pip_points[] __asm__("D_L11_001F0CF8"); /* outline of an ammo pip */

/* The health bar's palette: jet_fighter_update points clut at it in texture
   memory and, the first time (saved), copies it to D_L11_001F1088. */
typedef struct {
    s32 *clut;
    s32  saved;
} HpBarPalette;

HpBarPalette hp_bar_palette __asm__("D_L11_0016216C") = {0};

extern s32
    ship_hp_bar_saved_palette[] __asm__("D_L11_001F1088"); /* the health bar palette as loaded */

extern void vu1_add_g_sregister(s32, u64) __asm__("FUN_00233980");
extern void append_rotated_sprite_quad(
    f32, f32, f32, f32, f32, s32, s32, s64, s32, s32, s32, s32, f32, f32) __asm__("FUN_001f5ab0");
extern void append_scaled_screen_triangle_strip(
    short *pts, int n, f32 x, f32 y, u32 col, u64 prim, f32 scale) __asm__("FUN_L11_00310ad0");
extern void draw_quarter_turn_textured_quads(
    f32 x, f32 y, f32 scale, f32 angle, u8 r, u8 g, u8 b, u8 a) __asm__("FUN_L11_00311048");
extern f32   normalize_angle(f32) __asm__("FUN_L00_0025e310");
extern s32   help_request(s32 message, s32 help) __asm__("FUN_L00_00203908");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void
font_print_center_large(s32 x, s32 y, u64 color, char *text, s32 scale) __asm__("FUN_001f6c20");

/* Ship HUD: radar, ammo pips, reticle, lock-on marker, help hints and the health bar. */
void draw_ship_hud(struct Moby *moby) __asm__("FUN_L11_00311d50");

void draw_ship_hud(struct Moby *moby) {
    ShipVars *vars = (ShipVars *)moby->pvars;
    short    *list;
    void     *found[12];
    s32       sx;
    s32       sy;
    s32       i;
    s32       x;
    s32       y;
    u8        pips;
    s32       rx;
    s32       ry;
    s64       reticle_texture_tex0;
    f32       remaining_lock_fade_fraction;
    f32       grow;
    f32       fade;
    u8        alpha;
    s32       blink;
    f32       step;
    f32       size;
    s32       fill;
    s32       lo;
    s32       hi;
    s32       entry;
    s32       j;

    vu1_add_g_sregister(0x42, 0x8000000044);
    draw_textured_quad(0x170,
                       screen_extent.height - 0x90,
                       0x80,
                       0x80,
                       0,
                       0,
                       0x80,
                       0x80,
                       0x70808080,
                       get_effect_texture(0x3C));
    draw_textured_quad(0x170,
                       screen_extent.height - 0x90,
                       0x80,
                       0x80,
                       0,
                       0,
                       0x80,
                       0x80,
                       0x70808080,
                       get_effect_texture(0x3D));
    vu1_add_g_sregister(0x42, 0x8000000048);
    get_effect_texture(0x3E); /* fetched, but nothing below draws with it */

    list = D_L11_001AC240[vars->target_class];
    if (list != NULL) {
        do {
            /* D_L11_0015FFD8 holds the address of the level's moby array */
            struct Moby *listed_moby = (struct Moby *)(((*list & 0x7FFF) << 8) + D_L11_0015FFD8);

            if (listed_moby->state < 0x80) {
                s32 count = collect_oil_tanker_chain_mobys((char *)listed_moby, found);

                for (i = 0; i < count; i++) {
                    draw_ship_radar_moby_icon((char *)D_L11_001677D0,
                                              (char *)(D_L11_001677D0 - 4),
                                              found[i],
                                              0,
                                              0x75808080);
                }
            }
        } while (*list++ >= 0);
    }

    y = 0x40;
    x = 0x18;
    for (i = 0; i < hero.ship_ammo_max; i++) {
        if (i < hero.ship_ammo) {
            append_scaled_screen_triangle_strip(
                ship_ammo_pip_points, 0x19, x, y, 0xFFFFF3, 0x50008F00, 0.5f);
        } else {
            append_scaled_screen_triangle_strip(
                ship_ammo_pip_points, 0x19, x, y, 0xFFFFF3, 0x20004F00, 0.5f);
        }
        pips = hero.ship_ammo_max;
        if (i == pips >> 1) {
            y = 0x2E;
            x += 0x1E;
        }
        y += 0x12;
    }

    rx = vars->reticle_x;
    ry = vars->reticle_y;
    reticle_texture_tex0 = get_effect_texture(0x11);
    append_rotated_sprite_quad(rx,
                               ry,
                               40.0f,
                               40.0f,
                               0.0f,
                               0x3F,
                               0x3F,
                               reticle_texture_tex0,
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);
    reticle_texture_tex0 = get_effect_texture(0x12);
    append_rotated_sprite_quad(rx,
                               ry,
                               40.0f,
                               40.0f,
                               0.0f,
                               0x3F,
                               0x3F,
                               reticle_texture_tex0,
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);
    reticle_texture_tex0 = get_effect_texture(8);
    append_rotated_sprite_quad(rx,
                               ry,
                               10.0f,
                               10.0f,
                               0.0f,
                               0x1F,
                               0x1F,
                               reticle_texture_tex0,
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);

    update_ship_lock_on_target(moby, (char *)vars);
    if (vars->target != NULL) {
        project_world_point_to_screen_pixels(&vars->target->pos, &sx, &sy, 0);
        if (vars->lock_timer > lock_hold_frames) {
            remaining_lock_fade_fraction =
                convert_integer_to_float(vars->lock_timer - lock_hold_frames) /
                convert_integer_to_float(lock_fade_frames);
            fade = 2.0f * (1.0f - remaining_lock_fade_fraction);
            grow = remaining_lock_fade_fraction * 5.0f + 1.0f;
            if (fade > 1.0f) {
                fade = 1.0f;
            }
            alpha = truncate_float_to_s32(fade * 96.0f);
            draw_quarter_turn_textured_quads(
                convert_integer_to_float(sx),
                convert_integer_to_float(sy),
                grow,
                normalize_angle(convert_integer_to_float(level_frame) / 30.0f),
                0,
                0xFF,
                0,
                alpha);
            vars->locked = NULL;
        } else {
            blink = 0xFF;
            if ((vars->lock_timer / scale_game_frames(0x14)) & 1) {
                blink = 0;
            }
            draw_quarter_turn_textured_quads(convert_integer_to_float(sx),
                                             convert_integer_to_float(sy),
                                             1.0f,
                                             0.0f,
                                             0xFF,
                                             blink,
                                             0,
                                             0x60);
            vars->locked = vars->target;
        }
    }

    if (hero.ship_hp < hero.ship_hp_max * 0.25f) {
        s32 shown = help_records[0x58].count;

        if (shown == 0) {
            help_request(0x2AFF, 0x58);
        } else if (shown < 2) {
            vars->low_hp_frames++;
            if (vars->low_hp_frames > scale_game_frames(1800)) {
                help_request(0x2AFF, 0x58);
            }
        }
    } else if (vars->low_hp_frames != 0) {
        if (help_records[0x58].count < 0xFFFF) {
            help_records[0x58].count++;
        }
        if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x58].unk2) {
            help_records[0x58].unk2 = scale_game_frames(D_0015EEA4) / 600;
        }
        help_records[0x58].level_mask =
            help_records[0x58].level_mask | (1 << current_level_index) | 0x80000000;
        vars->low_hp_frames = 0;
    }

    fast_dec_timer(&vars->ammo_hint_timer);
    if ((hero.ship_ammo < 5 && help_records[0x57].count == 0) ||
        (hero.ship_ammo == 0 && help_records[0x57].count < 2 && vars->ammo_hint_timer == 0)) {
        help_request(0x2AFE, 0x57);
        vars->ammo_hint_timer = scale_game_frames(1200);
    }
    if (help_records[0x56].count == 0 && vars->hint_frames++ > scale_game_frames(3600)) {
        help_request(0x2AFD, 0x56);
    }

    if (moby->state == 8) {
        font_print_center_large(0x100, 0xC8, 0x80005080, get_help_message_text(0x523E), 0x11);
    }

    step = hero.ship_hp - vars->shown_hp;
    step *= 0.2f;
    size = absolute_float(step);
    if (size > 1.0f) {
        step /= size;
    }
    vars->shown_hp += step;
    fill = truncate_float_to_s32(vars->shown_hp * 251.0f / hero.ship_hp_max) + 2;
    if (fill > 0xFD) {
        fill = 0xFD;
    }
    if (fill < 2) {
        fill = 2;
    }
    lo = vars->hp_bar_fill <= fill ? vars->hp_bar_fill : fill;
    hi = vars->hp_bar_fill < fill ? fill : vars->hp_bar_fill;
    for (j = lo; j <= hi; j++) {
        /* CSM1 palette order: bits 3 and 4 of the entry swap */
        entry = (j & 0xE7) | ((j & 0x10) >> 1) | ((j & 8) << 1);
        if (j < fill) {
            hp_bar_palette.clut[entry] = ship_hp_bar_saved_palette[entry];
        } else {
            hp_bar_palette.clut[entry] = 0x80000000;
        }
    }
    vars->hp_bar_fill = fill;
}

extern int spawn_class_3f1_moby(void *, void *, void *, float, float) __asm__("FUN_L11_00308848");
extern f32 D_L11_00162090_gp __asm__("D_L11_00162090") __attribute__((sda));
extern unsigned char D_0013A4E0[];

extern short ship_shot_direction_base_scale_gp __asm__("D_L11_0016205C") __attribute__((sda));
int          D_L11_00162060 = 4;
void         update_ship_shot_firing_and_reticle(struct Moby *moby,
                                                 char        *ship_vars,
                                                 float        aim_azimuth,
                                                 float        aim_elevation) __asm__("FUN_L11_003125a8");

void update_ship_shot_firing_and_reticle(struct Moby *moby,
                                         char        *ship_vars,
                                         float        aim_azimuth,

                                         float aim_elevation) {
    char shot_direction[16];
    char projectile_spawn_position[16];
    char muzzle_forward_offset[16];
    char reticle_world_position[16];

    build_spherical_offset((f32 *)shot_direction,
                           *(float *)&ship_shot_direction_base_scale_gp *
                               *(float *)&D_L11_00162090_gp,
                           aim_azimuth,
                           aim_elevation);
    fast_vec_add(shot_direction, shot_direction, ship_vars);
    if (fast_dec_timer(ship_vars + 0x80) && (*(int *)(D_0013A4E0 + 0x2610) & 0x84)) {
        fast_vec_normalize(muzzle_forward_offset, shot_direction, 2.2f);
        moby_point_position(moby, 3 + (ship_vars[0x61] & 1), projectile_spawn_position);
        fast_vec_add(projectile_spawn_position, projectile_spawn_position, muzzle_forward_offset);
        spawn_class_3f1_moby(moby, shot_direction, projectile_spawn_position, 200.0f, -1.0f);
        *(int *)(ship_vars + 0x80) = D_L11_00162060;
        ship_vars[0x61] ^= 1;
        allocate_voice_for_target_entry(3, 0, (int)moby);
    }
    fast_vec_normalize(shot_direction, shot_direction, 23.0f);
    fast_vec_add(reticle_world_position, shot_direction, &moby->pos);
    project_world_point_to_screen_pixels(
        reticle_world_position, ship_vars + 0xE0, ship_vars + 0xE4, 0);
}
/* on a pad press, fires the aimed shot and resets the cooldown */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00313BC0), where it is exact; names translated to the US level program. */

extern int
spawn_ship_projectile(void *, void *, int, void *, float, int) __asm__("FUN_L11_00309378");
extern void FUN_0022da68(int, int, int);

extern short ship_projectile_base_scale_gp __asm__("D_L11_00162064") __attribute__((sda));
int          D_L11_00162068 = 30;
void try_fire_ship_projectile(struct Moby *moby, char *ship_vars) __asm__("FUN_L11_003126f8");

void try_fire_ship_projectile(struct Moby *moby, char *ship_vars) {
    if (fast_dec_timer(ship_vars + 0x84) && (*(int *)(D_0013A4E0 + 0x2610) & 0x28)) {
        if (hero.ship_ammo != 0) {
            char projectile_spawn_position[16];
            char projectile_spawn_offset[16];
            int  projectile_target = 0;
            int  projectile_spawn_slot = (*(unsigned short *)(ship_vars + 0x6A) + 1) & 3;
            *(unsigned short *)(ship_vars + 0x6A) = projectile_spawn_slot;
            moby_point_position(moby, projectile_spawn_slot + 7, projectile_spawn_position);
            fast_vec_normalize(projectile_spawn_offset, ship_vars, 0.3f);
            fast_vec_add(
                projectile_spawn_position, projectile_spawn_position, projectile_spawn_offset);
            if (*(int *)(ship_vars + 0x88) != 0 && !(500 < *(int *)(ship_vars + 0x8C)))
                projectile_target = *(int *)(ship_vars + 0x88);
            if (spawn_ship_projectile(moby,
                                      projectile_spawn_position,
                                      projectile_target,
                                      &moby->rot,
                                      *(float *)&ship_projectile_base_scale_gp *
                                          *(float *)&D_L11_00162090_gp,
                                      scale_game_frames(300))) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                hero.ship_ammo -= 1;
                *(int *)(ship_vars + 0x84) = D_L11_00162068;
            }
        }
    }
}
/* Flies the player's ship on Pokitaru: throttle and stick input, camera, and collision response.
   Same source as the exact FUN_L17_002ec360 without the path steering. */

typedef float        W4_2838[4] __attribute__((aligned(16)));
typedef unsigned int Q_2838 __attribute__((mode(TI), aligned(16)));

typedef union {
    Q_2838  q;
    W4_2838 v;
} UQ_2838;

typedef struct {
    UQ_2838 e[2];
} T2_2838;

typedef struct {
    char          pad00[0x10];
    W4_2838       v10;
    float         f20, f24, f28, f2C;
    float         f30, f34, f38, f3C;
    W4_2838       home_pos;
    W4_2838       home_rot;
    unsigned char sel;
    unsigned char flip;
    short         s62;
    float         speed;
    short         count;
    short         s6A;
    float         f6C;
    float         f70;
    char          pad74[0xC];
    int           i80;
    int           i84;
    char          pad88[8];
    float         f90, f94, f98, f9C;
    float         fA0, fA4, fA8, fAC;
    float         fB0, fB4, fB8, fBC;
    float         fC0, fC4, fC8, fCC;
    float         fD0, fD4, fD8;
    int           iDC;
    char          padE0[8];
    short         sE8;
    short         sEA;
    char          padEC[4];
    float         shown_hp;
    int           hp_bar_fill;
    int           iF8;
    int           exit_path;
    int           exit_path2;
    char          pad104[8];
    int           i10C;
    char          pad110[4];
    int           i114;
    int           i118;
    char          pad11C[0x10];
    short         voice;
    short         voice2;
    float         fade;
} ShipVars_2838;

extern unsigned short D_00141C10 NOT_SDA;
extern float D_L11_0016D4F0      NOT_SDA;
extern char                      jet_fighter_collision_query_result[] __asm__("D_L11_00174840");
int                              D_L11_00161FF0 = 120;
int                              D_L11_00162004 = 5;
int                              D_L11_0016214C = 10;

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_15.h"

extern float D_L11_00161FF4_gp __asm__("D_L11_00161FF4") __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_14.h"

extern float D_L11_00162014 __attribute__((sda));
extern float D_L11_00162018 __attribute__((sda));
extern float D_L11_0016201C __attribute__((sda));
extern float D_L11_00162020 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_13.h"

extern float D_L11_00162034 __attribute__((sda));
extern float D_L11_00162038 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_12.h"

extern float D_L11_0016204C __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_11.h"

extern float D_L11_0016207C __attribute__((sda));
extern float D_L11_00162080 __attribute__((sda));
extern float D_L11_0016208C __attribute__((sda));
extern float D_L11_00162090_gp __asm__("D_L11_00162090") __attribute__((sda));
extern float D_L11_0016209C __attribute__((sda));
extern float D_L11_001620A0 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_10.h"

extern float D_L11_001620AC __attribute__((sda));
extern float D_L11_001620B0 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_9.h"

extern float D_L11_001620BC __attribute__((sda));
extern float D_L11_001620C0 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_8.h"

extern float D_L11_001620E0 __attribute__((sda));
extern float D_L11_001620E4 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_7.h"

extern float D_L11_001620EC __attribute__((sda));
extern float D_L11_001620F0 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_6.h"

extern float D_L11_001620F8 __attribute__((sda));
extern float D_L11_001620FC __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_5.h"

extern float D_L11_00162104 __attribute__((sda));
extern float D_L11_00162108 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_4.h"

extern float D_L11_00162110 __attribute__((sda));
extern float D_L11_00162114 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_3.h"

extern float D_L11_0016211C __attribute__((sda));
extern float D_L11_00162120 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight_2.h"

extern float D_L11_00162128 __attribute__((sda));
extern float D_L11_0016212C __attribute__((sda));
extern float D_L11_00162130 __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/update_player_ship_flight.h"

extern float D_L11_00162150 __attribute__((sda));
extern float D_L11_00162154 __attribute__((sda));
extern float D_L11_00162158 __attribute__((sda));
extern float vendor_spring(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern float
spring_angle(float *, float *, int, float, float, float, float) __asm__("FUN_L00_0025bc98");
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
extern void draw_ship_hud();
void        adjust_ship_steering_for_flight_bounds(
           struct Moby *moby,
           float       *horizontal_steering_input,
           float       *vertical_steering_input) __asm__("FUN_L11_00313f60");

void adjust_ship_steering_for_flight_bounds(struct Moby *moby,
                                            float       *a,
                                            float       *b) __asm__("FUN_L11_00313f60");

void update_player_ship_flight(struct Moby   *ship_moby,
                               ShipVars_2838 *ship_vars) __asm__("FUN_L11_00312838");

void update_player_ship_flight(struct Moby *ship_moby, ShipVars_2838 *ship_vars) {
    W4_2838          old;
    W4_2838          death_explosion_velocity;
    UQ_2838          up;
    W4_2838          ship_movement_vector;
    W4_2838          side;
    W4_2838          camera_rotation;
    W4_2838          target;
    W4_2838          camera_local_offset;
    float            horizontal_steering_input;
    float            vertical_steering_input;
    float            ka;
    float            kb;
    float            x, y;
    float            half_fov, tn;
    float            ratio;
    float            collision_adjusted_pitch;
    float            k;
    float            fov;
    struct PadState *pad;
    struct Hero     *hero_input_source;
    char            *collision_result;
    int              hc;

    up.q = 0;
    ship_moby->flags &= 0xFEFF;
    up.v[2] = frame_time * 8.0f * D_L11_00162090_gp;
    *(Q_2838 *)death_explosion_velocity = up.q;
    up.q = 0;
    up.v[2] = 1.0f;
    if (controller_state.raw_pressed & PAD_LEFT) {
        if (++ship_vars->sel >= 2) {
            ship_vars->sel = 0;
        }
    }
    if (D_L11_00161FF0 < ship_vars->count) {
        pad = &controller_state;
        if (pad->raw_held & 0x40) {
            float accelerated_speed;
            float max;
            if (usage_stats.stat[28].count < 0xFFFF) {
                usage_stats.stat[28].count++;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > usage_stats.stat[28].unk2) {
                usage_stats.stat[28].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            usage_stats.stat[28].level_mask =
                usage_stats.stat[28].level_mask | (1 << current_level_index) | 0x80000000;
            accelerated_speed =
                ship_vars->speed + jet_fighter_boost_speed_increment_gp * D_L11_00162090_gp;
            max = jet_fighter_boost_speed_limit_gp * D_L11_00162090_gp;
            ship_vars->speed = accelerated_speed;
            if (max < accelerated_speed) {
                ship_vars->speed = max;
            }
        } else {
            float hi = D_L11_00161FF4_gp * D_L11_00162090_gp;
            float step = jet_fighter_cruise_speed_increment_gp * D_L11_00162090_gp;
            float current_speed = ship_vars->speed;
            if (current_speed < hi - step) {
                ship_vars->speed = current_speed + step;
            } else if (hi < current_speed) {
                ship_vars->speed =
                    current_speed +
                    (hi - current_speed) * jet_fighter_cruise_speed_relaxation_factor_gp;
            } else {
                ship_vars->speed = hi;
            }
        }
    } else {
        ship_vars->speed += jet_fighter_startup_speed_increment_gp * D_L11_00162090_gp;
        ship_vars->count += 1;
    }

    if (D_L11_00162004 < ship_vars->count) {
        hero_input_source = &hero;
        x = hero_input_source->stick_x;
        y = hero_input_source->stick_y;
        ka = jet_fighter_horizontal_steering_snap_threshold_scale_gp;
        kb = jet_fighter_vertical_steering_snap_threshold_scale_gp;
        horizontal_steering_input = x;
        vertical_steering_input = y;
        if (controller_state.held & PAD_L3) {
            horizontal_steering_input = x / D_L11_00162018 * D_L11_00162020;
            vertical_steering_input = y / D_L11_00162014 * D_L11_0016201C;
            ka = jet_fighter_l3_horizontal_steering_snap_threshold_scale_gp;
            kb = jet_fighter_l3_vertical_steering_snap_threshold_scale_gp;
        }
        adjust_ship_steering_for_flight_bounds(
            ship_moby, &horizontal_steering_input, &vertical_steering_input);
        vendor_spring(&ship_vars->f6C,
                      &ship_vars->f90,
                      horizontal_steering_input,
                      D_L11_0016209C,
                      D_L11_001620A0,
                      ka);
        vendor_spring(&ship_vars->f70,
                      &ship_vars->f94,
                      vertical_steering_input,
                      D_L11_001620AC,
                      D_L11_001620B0,
                      kb);
        spring_angle(&ship_vars->f30,
                     &ship_vars->f98,
                     0,
                     ship_vars->f6C * D_L11_0016207C,
                     D_L11_001620BC,
                     D_L11_001620C0,
                     jet_fighter_rotation_x_snap_threshold_scale_gp);
        ship_vars->f34 = fast_subtract_rotations(ship_vars->f34, ship_vars->f70 * D_L11_00162014);
        if (ship_vars->f34 > 1.3962634f) {
            ship_vars->f34 = 1.3962634f;
        } else if (ship_vars->f34 < -1.3962634f) {
            ship_vars->f34 = -1.3962634f;
        }
        ship_vars->f38 = fast_subtract_rotations(ship_vars->f38, ship_vars->f6C * D_L11_00162018);
    }

    build_spherical_offset(ship_movement_vector, ship_vars->speed, ship_vars->f38, -ship_vars->f34);
    fast_vec_normalize(ship_vars->v10, ship_movement_vector, 1.0f);
    qcopy(old, &ship_moby->pos);
    fast_vec_add(&ship_moby->pos, &ship_moby->pos, ship_movement_vector);
    {
        float lo = D_L11_00161FF4_gp;
        float range = jet_fighter_boost_speed_limit_gp - lo;
        float sp = ship_vars->speed - lo;
        if (sp < 0.0f) {
            ratio = 0.0f / range;
        } else {
            ratio = sp / range;
        }
    }
    ship_vars->fC4 = ratio;
    {
        T2_2838 camera_offset_targets;
        T2_2838 wr;
        float   ship_rotation_basis[12];
        wr.e[0].q = 0;
        wr.e[1].q = 0;
        wr.e[0].v[0] = (jet_fighter_camera_base_local_x_offset_gp -
                        ship_vars->f70 * D_L11_00162128 - ratio * D_L11_00162130) *
                       D_L11_0016208C;
        wr.e[0].v[1] = ship_vars->f6C * D_L11_00162080 * D_L11_0016208C;
        wr.e[0].v[2] =
            (jet_fighter_camera_base_local_z_offset_gp + ship_vars->f70 * D_L11_0016212C) *
            D_L11_0016208C;
        wr.e[1].v[0] = jet_fighter_alternate_camera_local_x_offset_gp * D_L11_0016208C;
        wr.e[1].v[2] = jet_fighter_alternate_camera_local_z_offset_gp * D_L11_0016208C;
        camera_offset_targets.e[0].q = wr.e[0].q;
        camera_offset_targets.e[1].q = wr.e[1].q;
        vendor_spring(&ship_vars->fB4,
                      &ship_vars->fC8,
                      camera_offset_targets.e[ship_vars->sel].v[0],
                      D_L11_00162104,
                      D_L11_00162108,
                      jet_fighter_camera_local_x_snap_threshold_scale_gp);
        vendor_spring(&ship_vars->fB8,
                      &ship_vars->fCC,
                      camera_offset_targets.e[ship_vars->sel].v[1],
                      D_L11_00162110,
                      D_L11_00162114,
                      jet_fighter_camera_local_y_snap_threshold_scale_gp);
        vendor_spring(&ship_vars->fBC,
                      &ship_vars->fD0,
                      camera_offset_targets.e[ship_vars->sel].v[2],
                      D_L11_0016211C,
                      D_L11_00162120,
                      jet_fighter_camera_local_z_snap_threshold_scale_gp);
        camera_local_offset[0] = ship_vars->fB4;
        camera_local_offset[1] = ship_vars->fB8;
        camera_local_offset[2] = ship_vars->fBC;
        camera_local_offset[3] = 0.0f;
        euler_rotation_basis(ship_rotation_basis, &ship_vars->f30);
        transform_vector_by_basis(target, camera_local_offset, ship_rotation_basis);
        fast_vec_add(wr.e[0].v, &ship_moby->pos, target);
        set_camera_position(wr.e[0].v);
        wr.e[1].v[0] =
            fast_add_rotations(ship_vars->f30 * D_L11_00162038, ship_vars->f6C * D_L11_00162034);
        wr.e[1].v[1] = fast_subtract_rotations(
            fast_add_rotations(ship_vars->f34, jet_fighter_camera_pitch_offset_radians_gp),
            ship_vars->f70 * D_L11_00162014);
        wr.e[1].v[2] = ship_vars->f38;
        spring_angle(&ship_vars->f20,
                     &ship_vars->fA0,
                     0,
                     wr.e[1].v[0],
                     D_L11_001620E0,
                     D_L11_001620E4,
                     jet_fighter_camera_rotation_x_snap_threshold_scale_gp);
        spring_angle(&ship_vars->f24,
                     &ship_vars->fA4,
                     0,
                     wr.e[1].v[1],
                     D_L11_001620EC,
                     D_L11_001620F0,
                     jet_fighter_camera_rotation_y_snap_threshold_scale_gp);
        spring_angle(&ship_vars->f28,
                     &ship_vars->fA8,
                     0,
                     wr.e[1].v[2],
                     D_L11_001620F8,
                     D_L11_001620FC,
                     jet_fighter_camera_rotation_z_snap_threshold_scale_gp);
    }
    camera_rotation[0] = ship_vars->f20;
    camera_rotation[2] = fast_subtract_rotations(ship_vars->f28, ship_vars->fD4);
    camera_rotation[1] = fast_subtract_rotations(ship_vars->f24, ship_vars->fD8);
    set_camera_rotation(camera_rotation);
    if (controller_state.raw_held & PAD_CROSS) {
        fov = jet_fighter_boost_camera_fov_radians_gp;
    } else {
        fov = jet_fighter_cruise_camera_fov_radians_gp;
    }
    advance_accelerated_scalar(&ship_vars->fB0,
                               &ship_vars->fC0,
                               fov,
                               frame_time_sq + frame_time_sq,
                               frame_time_sq + frame_time_sq,
                               frame_time + frame_time);
    half_fov = ship_vars->fB0 * 0.5f;
    tn = fast_sin(half_fov) / fast_cos(half_fov);
    D_L11_0016D4F0 = tn;
    update_view_context();
    ship_moby->rot.x = fast_add_rotations(ship_vars->f30, ship_vars->f6C * D_L11_00162150);
    ship_moby->rot.y = fast_add_rotations(ship_vars->f34, -ship_vars->f70 * D_L11_00162154);
    ship_moby->rot.z = fast_add_rotations(ship_vars->f38, -ship_vars->f6C * D_L11_00162158);
    if (coll_sphere(2.0f, &ship_moby->pos, 0, ship_moby)) {
        collision_result = jet_fighter_collision_query_result;
        if (*(char **)(collision_result + 0x18) == 0) {
            spawn_explosion(ship_moby,
                            D_L11_0015F580,
                            0,
                            8.0f,
                            100.0f,
                            0x1E,
                            0xA,
                            0x18,
                            16.0f,
                            8.0f,
                            9.0f,
                            1.0f,
                            -1,
                            50.0f,
                            1,
                            1,
                            -1,
                            0);
            ship_moby->visible = 0;
            ship_moby->state = 7;
            ship_moby->flags |= 1;
            ship_vars->iF8 = scale_game_frames(0x5A);
        } else {
            hc = *(short *)(*(char **)(collision_result + 0x18) + 0xA6);
            if (hc != 0 && hc != 0x192 && hc != 0x4C2 && hc != 0x4C3) {
                qcopy(&ship_moby->pos, collision_result + 0x30);
                reflect_vector(ship_movement_vector, ship_movement_vector, collision_result + 0x40);
                ship_vars->f38 = fast_add_rotations(
                    ship_vars->f38,
                    fast_subtract_rotations(
                        fast_arc_tan(ship_movement_vector[0], ship_movement_vector[1]),
                        ship_vars->f38) *
                        jet_fighter_yaw_deflection_scale_gp);
                collision_adjusted_pitch = fast_add_rotations(
                    ship_vars->f34,
                    fast_subtract_rotations(-fast_arc_tan(vector_length_xy(ship_movement_vector),
                                                          ship_movement_vector[2]),
                                            ship_vars->f34) *
                        jet_fighter_pitch_deflection_scale_gp);
                k = ship_vars->speed * D_L11_0016204C;
                ship_vars->f34 = collision_adjusted_pitch;
                ship_vars->speed = k;
                if (ship_vars->iDC == 0) {
                    hero.ship_hp = hero.ship_hp - jet_fighter_collision_damage_gp;
                    if (hero.ship_hp < 0.0f) {
                        fast_vec_sub(side, collision_result + 0x50, collision_result + 0x60);
                        fast_vec_normalize(side, side, 1.0f);
                        reflect_vector(death_explosion_velocity,
                                       ship_movement_vector,
                                       collision_result + 0x40);
                        fast_vec_normalize(death_explosion_velocity,
                                           death_explosion_velocity,
                                           frame_time + frame_time);
                        fast_vec_normalize(up.v, collision_result + 0x40, 1.0f);
                        spawn_explosion(ship_moby,
                                        death_explosion_velocity,
                                        0,
                                        8.0f,
                                        100.0f,
                                        0x1E,
                                        0xA,
                                        0x18,
                                        16.0f,
                                        8.0f,
                                        9.0f,
                                        1.0f,
                                        -1,
                                        50.0f,
                                        1,
                                        1,
                                        -1,
                                        0);
                        ship_moby->state = 7;
                    }
                    ship_vars->iDC = D_L11_0016214C;
                }
            }
        }
    }
    if (D_00141C10 == 0) {
        help_request(0x2AFC, 0x55);
    }
    add_draw_callback(draw_ship_hud, ship_moby);
    try_fire_ship_projectile(ship_moby, (char *)ship_vars);
    update_ship_shot_firing_and_reticle(
        ship_moby,
        (char *)ship_vars,
        fast_add_rotations(ship_vars->f28, -ship_vars->f6C / 7.0f),
        -fast_add_rotations(ship_vars->f24, -(ship_vars->f70 / 7.0f) - 0.1f));
}
/* Per-frame update for the player's ship moby on Pokitaru: takes hits, boards and leaves the ship,
   and runs the flight states. Same source as the exact fighter_jet_update with Pokitaru's states. */

typedef struct {
    char    pad00[0x10];
    W4_2838 push;
    char    pad20[0xC];
    float   damage;
    int     flags;
} ShipHit_3290;

typedef struct {
    char           pad0[0xA];
    unsigned short idx;
    char           padC[4];
} ShipLevelEntry_3290;

typedef struct {
    char b[0x400];
} Block400_3290;

extern ShipLevelEntry_3290 D_L11_0016DA80[];
extern char               *ship_hp_bar_palette_address_base __asm__("D_L11_0015F420");
extern unsigned char       D_0014C050[][16];
extern char jet_fighter_missing_victory_jet_destination_message[] __asm__("D_L11_0021B610");
extern char jet_fighter_missing_victory_hero_destination_message[] __asm__("D_L11_0021B648");
extern int  D_L11_0015F608;

#include "rnc/overlay/l11/mobys/seesaw_platform_data/jet_fighter_update_2.h"

unsigned char l11_jet_fighter_initial_ammo __asm__("D_L11_00162054") = 0xa;
unsigned char l11_jet_fighter_max_ammo __asm__("D_L11_00162058") = 0x14;

#include "rnc/overlay/l11/mobys/seesaw_platform_data/jet_fighter_update.h"

extern float D_L11_00162078 __attribute__((sda));
int          D_L11_00162168 = 180;
extern float D_L11_0016D4F0_a[] __asm__("D_L11_0016D4F0");
extern void  print_3290(char *) __asm__("FUN_001e93b0");
extern void  FUN_L11_0024db50(int, int);
extern float jet_distance(void *, void *) __asm__("FUN_L00_00250430");
extern void  jet_game_state(int, int) __asm__("FUN_L01_0027a248");
extern void  hero_respawn(void) __asm__("FUN_L00_00211250");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void  emit_type_74_particles_from_moby_joints(void *, void *) __asm__("FUN_L11_003112b8");
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int   count_listed_blarg_ships_in_state_5(int) __asm__("FUN_L11_0031a758");
extern int   activate_listed_blarg_ship(int) __asm__("FUN_L11_0031a7d8");

s32  hp_bar_saved __asm__("D_L11_00162170") = 0;
void jet_fighter_update(struct Moby *moby) __asm__("FUN_L11_00313290");

void jet_fighter_update(struct Moby *moby) {
    enum {
        JET_FIGHTER_STATE_INITIALIZE = 0,
        JET_FIGHTER_STATE_WAIT_FOR_BOARDING = 1,
        JET_FIGHTER_STATE_BOARD = 2,
        JET_FIGHTER_STATE_FLIGHT = 4,
        JET_FIGHTER_STATE_DISMOUNT = 5,
        JET_FIGHTER_STATE_WAIT_FOR_RESPAWN = 7,
        JET_FIGHTER_STATE_DELAYED_EXIT = 8,
        JET_FIGHTER_STATE_INACTIVE = 9
    };
    ShipVars_2838 *ship_vars;
    ShipHit_3290  *hit;
    W4_2838        ship_velocity_scratch;
    char          *voice_entry;
    int            idx;
    int            frames_since_help_timestamp;
    float          k;
    float          fov;

    if (moby != 0) {
        ship_vars = (ShipVars_2838 *)moby->pvars;
        fast_dec_timer(&ship_vars->iDC);
        if (moby->state != 7 && moby->state != 1 && moby->state != 2) {
            hit = find_moby_hit(moby, 0x30001, 0);
            if (hit != 0 && moby->state == 4) {
                struct Hero *g = &hero;
                if (hit->damage < g->ship_hp) {
                    g->ship_hp -= hit->damage;
                    if (hit->flags & 1) {
                        k = hit->damage * D_L11_00162078;
                        build_spherical_offset(ship_velocity_scratch,
                                               ship_vars->speed,
                                               ship_vars->f38,
                                               -ship_vars->f34);
                        fast_vec_add(&moby->pos, &moby->pos, hit->push);
                        reflect_vector(ship_velocity_scratch, ship_velocity_scratch, hit->push);
                        ship_vars->f38 = fast_add_rotations(
                            ship_vars->f38,
                            fast_subtract_rotations(
                                fast_arc_tan(ship_velocity_scratch[0], ship_velocity_scratch[1]),
                                ship_vars->f38) *
                                jet_fighter_yaw_deflection_scale_gp * k);
                        ship_vars->f34 = fast_add_rotations(
                            ship_vars->f34,
                            fast_subtract_rotations(
                                fast_arc_tan(vector_length_xy(ship_velocity_scratch),
                                             ship_velocity_scratch[2]),
                                ship_vars->f34) *
                                jet_fighter_pitch_deflection_scale_gp * k);
                    }
                } else {
                    build_spherical_offset(
                        ship_velocity_scratch, ship_vars->speed, ship_vars->f38, -ship_vars->f34);
                    g->ship_hp = 0.0f;
                    moby->state = 7;
                    ship_vars->s62 = 0xB4;
                    g->motion.pos.f[2] += 0.2f;
                    g->moby->collision_disable = 0;
                    FUN_L11_0024db50(0, 1);
                    g->ship_moby = 0;
                    g->ship_oclass = -1;
                    spawn_explosion(moby,
                                    ship_velocity_scratch,
                                    0,
                                    0.0f,
                                    0.0f,
                                    10,
                                    3,
                                    16,
                                    4.0f,
                                    2.0f,
                                    9.0f,
                                    1.0f,
                                    -1,
                                    15.0f,
                                    1,
                                    1,
                                    -1,
                                    0);
                }
            }
        }
        moby->hit_slot = 0xFF;
        {
            struct Hero *g = &hero;
            if (g->ship_moby == moby) {
                g->ship_flags &= ~2;
                g->ship_hp_percent = g->ship_hp / jet_fighter_max_health_gp * 200.0f;
            }
        }
        switch (moby->state) {
        case JET_FIGHTER_STATE_INITIALIZE:
            moby->state = 1;
            ship_vars->sel = 0;
            moby->scale = *(float *)((char *)moby->pclass + 0x24);
            ship_vars->f20 = ship_vars->f30;
            ship_vars->f24 = ship_vars->f34;
            fov = fast_arc_tan(1.0f, D_L11_0016D4F0_a[0]) * 2.0f;
            ship_vars->fC0 = 0.0f;
            ship_vars->fAC = fov;
            ship_vars->fB0 = fov;
            qcopy_nc(&ship_vars->f30, &moby->rot);
            qcopy_nc(ship_vars->home_rot, &moby->rot);
            qcopy_nc(ship_vars->home_pos, &moby->pos);
            hp_bar_palette.clut =
                (s32 *)(ship_hp_bar_palette_address_base + D_L11_0016DA80[0x4C].idx * 16);
            if (hp_bar_saved == 0) {
                *(Block400_3290 *)ship_hp_bar_saved_palette = *(Block400_3290 *)hp_bar_palette.clut;
            }
            hp_bar_saved = 1;
            ship_vars->iF8 = 1;
            ship_vars->sE8 = 1;
            ship_vars->fade = 0.0f;
            if (moby->mission != 0xFF && D_0014C050[current_level_index][moby->mission] == 0xFF) {
                moby->state = 9;
                moby->collision = 0;
                moby->flags |= 1;
                moby->visible = 0;
            }
            break;
        case JET_FIGHTER_STATE_WAIT_FOR_BOARDING: {
            struct Hero *g = &hero;
            if (jet_distance(moby, g->moby) < jet_fighter_boarding_distance_gp) {
                if (item_available[7] != 0) {
                    try_set_help_message(7, 0x2B09);
                } else {
                    frames_since_help_timestamp =
                        scale_game_frames(D_0015EEA4) - help_records[0x3C].unk2 * 600;
                    if (frames_since_help_timestamp >
                            (int)((float)scale_game_frames(0x12) * 60.0f) ||
                        help_records[0x3C].unk2 * 600 == 0) {
                        help_request(0x2AFA, 0x3C);
                    } else if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x3C].unk2) {
                        help_records[0x3C].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                }
                if (item_available[7] != 0 && (D_0013CAE4 & 0x10)) {
                    moby->state = 2;
                    ship_vars->i118 = -1;
                }
            } else {
                ship_vars->i118 = -1;
                if (ship_vars->fade != 0.0f) {
                    approach_value(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                    D_L11_0015F3FC = ship_vars->fade;
                }
            }
            break;
        }
        case JET_FIGHTER_STATE_BOARD:
            approach_value(&ship_vars->fade, 1.0f, frame_time * 4.0f);
            D_L11_0015F3FC = ship_vars->fade;
            if (ship_vars->fade == 1.0f) {
                struct Hero *g;
                FUN_L11_0024db50(0x32, 1);
                moby->pos.z += 30.0f;
                jet_game_state(2, 6);
                clear_u64_value(&ship_vars->f30);
                clear_u64_value(&ship_vars->f20);
                ship_vars->f38 = ship_vars->f28 = moby->rot.z;
                ship_vars->i114 = scale_game_frames(0x708);
                if (help_records[0x55].count == 0) {
                    help_request(0x2AFC, 0x55);
                }
                g = &hero;
                moby->state = 4;
                g->ship_moby = moby;
                g->ship_oclass = moby->oclass;
                g->ship_ammo = l11_jet_fighter_initial_ammo;
                g->unk160E = 1;
                g->ship_ammo_max = l11_jet_fighter_max_ammo;
                g->unk15F8 = l11_jet_fighter_max_ammo;
                g->ship_hp = jet_fighter_initial_health_gp;
                ship_vars->shown_hp = 0.0f;
                ship_vars->hp_bar_fill = 0xFF;
                g->ship_hp_max = jet_fighter_max_health_gp;
                g->unk1604 = 100.0f;
                g->unk15F9 = 3;
                g->unk15FA = 3;
                g->unk15FB = 3;
                g->ship_flags = 0;
                jet_mount(D_L11_001677C0, D_L11_001677C0 + 0x10, 1, 0, 0);
                ship_vars->count = 0;
                ship_vars->speed = jet_fighter_initial_flight_speed_gp;
                ship_vars->f6C = 0.0f;
                ship_vars->f70 = 0.0f;
                ship_vars->i84 = 0;
                ship_vars->i80 = 0;
                ship_vars->fD4 = 0.0f;
                ship_vars->fD8 = 0.0f;
                moby->draw_dist = 0xFF;
                moby->update_dist = 0xFF;
                ship_vars->fB4 = jet_fighter_camera_base_local_x_offset_gp * D_L11_0016208C;
                ship_vars->fBC = jet_fighter_camera_base_local_z_offset_gp * D_L11_0016208C;
                ship_vars->fB8 = 0.0f;
                g->moby->collision_disable = -1;
                moby->scale = *(float *)((char *)moby->pclass + 0x24) * D_L11_0016208C;
                g->unk20A5 = 1;
            }
            break;
        case JET_FIGHTER_STATE_FLIGHT:
            if (ship_vars->fade != 0.0f) {
                approach_value(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                D_L11_0015F3FC = ship_vars->fade;
            }
            D_L11_0015F608 = 2;
            update_player_ship_flight(moby, ship_vars);
            {
                int alive = count_listed_blarg_ships_in_state_5(ship_vars->i10C);
                if (ship_vars->sEA != 0 && alive < 5 - ship_vars->sE8 &&
                    fast_dec_timer(&ship_vars->i114) &&
                    activate_listed_blarg_ship(ship_vars->i10C)) {
                    ship_vars->i114 = scale_game_frames(0xF0);
                }
            }
            if (ship_vars->sE8 <= 0) {
                ship_vars->iF8 = D_L11_00162168;
                moby->state = 8;
            }
            if (hero.ship_flags & 1) {
                moby->state = 5;
            }
            break;
        case JET_FIGHTER_STATE_DISMOUNT: {
            char        *p;
            struct Hero *g = &hero;
            g->unk20A5 = 0;
            g->moby->collision_disable = 0;
            FUN_L11_0024db50(0, 1);
            jet_end_mount(0);
            set_animation_parameter(g->unk160C, 0);
            jet_game_state(0, 8);
            qcopy_nc(&moby->pos, ship_vars->home_pos);
            qcopy_nc(&moby->rot, ship_vars->home_rot);
            moby->scale = *(float *)((char *)moby->pclass + 0x24);
            refresh_moby_spatial_bounds_from_basis(moby);
            p = (char *)l11_cuboids + (ship_vars->exit_path << 7);
            set_race_camera(p + 0x30, p + 0x70, 0, 1);
            idx = ship_vars->voice;
            if (idx != -1) {
                voice_entry = ((char *)&voice_pool) + idx * 0x70;
                if (*(struct Moby **)(voice_entry + 0x88) == moby &&
                    *(unsigned char *)(voice_entry + 0x74) != 0) {
                    release_voice_slot(idx);
                }
            }
            ship_vars->voice = -1;
            idx = ship_vars->voice2;
            if (idx != -1) {
                voice_entry = ((char *)&voice_pool) + idx * 0x70;
                if (*(struct Moby **)(voice_entry + 0x88) == moby &&
                    *(unsigned char *)(voice_entry + 0x74) != 0) {
                    release_voice_slot(idx);
                }
            }
            ship_vars->voice2 = -1;
            moby->state = 1;
            ship_vars->fade = 0.99f;
            D_L11_0015F3FC = 0.99f;
            break;
        }
        case JET_FIGHTER_STATE_WAIT_FOR_RESPAWN:
            D_L11_0015F608 = 2;
            if (fast_dec_timer(&ship_vars->iF8)) {
                hero_respawn();
            }
            break;
        case JET_FIGHTER_STATE_DELAYED_EXIT:
            D_L11_0015F608 = 2;
            update_player_ship_flight(moby, ship_vars);
            if (fast_dec_timer(&ship_vars->iF8)) {
                approach_value(&ship_vars->fade, 1.0f, frame_time * 4.0f);
                D_L11_0015F3FC = ship_vars->fade;
                if (ship_vars->fade >= 1.0f) {
                    char        *p;
                    struct Hero *g = &hero;
                    g->unk20A5 = 0;
                    g->moby->collision_disable = 0;
                    FUN_L11_0024db50(0, 1);
                    jet_end_mount(0);
                    jet_game_state(0, 8);
                    moby->visible = 0;
                    moby->collision = 0;
                    moby->flags |= 1;
                    if (ship_vars->exit_path2 == -1) {
                        print_3290(jet_fighter_missing_victory_jet_destination_message);
                        qcopy_nc(&moby->pos, ship_vars->home_pos);
                        qcopy_nc(&moby->rot, ship_vars->home_rot);
                    } else {
                        qcopy_nc(&moby->pos,
                                 ((char(*)[128])(char *)l11_cuboids)[ship_vars->exit_path2] + 0x30);
                        qcopy_nc(&moby->rot,
                                 ((char(*)[128])(char *)l11_cuboids)[ship_vars->exit_path2] + 0x70);
                    }
                    refresh_moby_spatial_bounds_from_basis(moby);
                    if (ship_vars->exit_path == -1) {
                        print_3290(jet_fighter_missing_victory_hero_destination_message);
                    } else {
                        p = (char *)l11_cuboids + (ship_vars->exit_path << 7);
                        set_race_camera(p + 0x30, p + 0x70, 0, 1);
                    }
                    if (moby->mission != 0xFF) {
                        jet_clear_persistent(moby->mission);
                    }
                    moby->state = 9;
                }
            }
            break;
        case JET_FIGHTER_STATE_INACTIVE:
            if (ship_vars->fade != 0.0f) {
                approach_value(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                D_L11_0015F3FC = ship_vars->fade;
            }
            break;
        case 10:
            break;
        }
        if (moby->pos.x < 15.0f) {
            moby->pos.x = 15.0f;
        }
        if (moby->pos.x > 1008.0f) {
            moby->pos.x = 1008.0f;
        }
        if (moby->pos.y < 15.0f) {
            moby->pos.y = 15.0f;
        }
        if (moby->pos.y > 1008.0f) {
            moby->pos.y = 1008.0f;
        }
        if (moby->pos.z < 15.0f) {
            moby->pos.z = 15.0f;
        }
        if (moby->pos.z > 1008.0f) {
            moby->pos.z = 1008.0f;
        }
        if (moby->state == 4) {
            qcopy_nc(&hero.motion.pos, &moby->pos);
            qcopy_nc(&hero.motion.rot, &ship_vars->f30);
        }
        if (moby->state == 3 || moby->state == 4) {
            emit_type_74_particles_from_moby_joints(moby, ship_vars);
            if (voice_is_playing(moby, ship_vars->voice) == 0) {
                ship_vars->voice = allocate_voice_for_target_entry(0, 4, moby);
            }
        } else {
            idx = ship_vars->voice;
            if (idx != -1) {
                voice_entry = ((char *)&voice_pool) + idx * 0x70;
                if (*(struct Moby **)(voice_entry + 0x88) == moby &&
                    *(unsigned char *)(voice_entry + 0x74) != 0) {
                    release_voice_slot(idx);
                }
            }
            ship_vars->voice = -1;
        }
    }
}

#include "sda.h"
/* Same source as the exact FUN_L17_002edf10, with its path index at 0x104 and a fixed floor of 250. */

extern LevelCuboid *l11_cuboids_macro __asm__("D_L11_001600EC") MACRO_ADDR;

void adjust_ship_steering_for_flight_bounds(struct Moby *moby,
                                            float       *horizontal_steering_input,

                                            float *vertical_steering_input) {
    char *ship_vars = (char *)moby->pvars;
    float predicted_position_scratch[4];
    float zz, predicted_boundary_distance_xy, boundary_radius, hi, lo, boundary_center_heading,
        upper_altitude_limit, k, radial_steering_blend_weight, heading_difference;
    fast_vec_scale(predicted_position_scratch,
                   ship_vars + 0x10,
                   *(float *)(ship_vars + 0x64) * (float)scale_game_frames(0x3C));
    fast_vec_add(predicted_position_scratch, predicted_position_scratch, &moby->pos);
    zz = predicted_position_scratch[2];
    fast_vec_sub(predicted_position_scratch,
                 predicted_position_scratch,
                 (*(int *)(ship_vars + 0x104) << 7) + (char *)l11_cuboids + 0x30);
    predicted_boundary_distance_xy = vector_length_xy(predicted_position_scratch);
    boundary_radius = vector_length_xy((*(int *)(ship_vars + 0x104) << 7) + (char *)l11_cuboids);
    hi = l11_cuboids_macro[*(int *)(ship_vars + 0x104)].axes[2].z;
    lo = l11_cuboids_macro[*(int *)(ship_vars + 0x104)].pos.z;
    upper_altitude_limit = hi + lo;
    boundary_center_heading = fast_add_rotations(
        fast_arc_tan(predicted_position_scratch[0], predicted_position_scratch[1]),
        3.141590118408203f);
    if (boundary_radius - 10.0f < predicted_boundary_distance_xy) {
        radial_steering_blend_weight =
            (predicted_boundary_distance_xy - boundary_radius + 10.0f) / 10.0f;
        heading_difference =
            fast_subtract_rotations(boundary_center_heading, *(float *)(ship_vars + 0x38));
        if (radial_steering_blend_weight > 1.0f)
            radial_steering_blend_weight = 1.0f;
        else if (radial_steering_blend_weight < 0.0f)
            radial_steering_blend_weight = 0.0f;
        if (heading_difference > 0.0f) {
            *horizontal_steering_input =
                (-1.0f - *horizontal_steering_input) * radial_steering_blend_weight +
                *horizontal_steering_input;
        } else {
            *horizontal_steering_input =
                (1.0f - *horizontal_steering_input) * radial_steering_blend_weight +
                *horizontal_steering_input;
        }
        hero.ship_flags |= 2;
    }
    if (upper_altitude_limit - 10.0f < zz) {
        float q, w;
        q = (zz - upper_altitude_limit + 10.0f) / 10.0f;
        w = (0.5235987901687622f - *(float *)(ship_vars + 0x34)) / 0.5235987901687622f;
        if (q > 1.0f)
            q = 1.0f;
        else if (q < 0.0f)
            q = 0.0f;
        if (w > 1.0f)
            w = 1.0f;
        else if (w < 0.0f)
            w = 0.0f;
        q = q * w;
        *vertical_steering_input =
            (-1.0f - *vertical_steering_input) * q + *vertical_steering_input;
        hero.ship_flags |= 2;
    } else if (zz < 260.0f) {
        float q, w;
        q = (250.0f - zz + 10.0f) / 10.0f;
        w = (*(float *)(ship_vars + 0x34) + 0.5235987901687622f) / 0.5235987901687622f;
        if (q > 1.0f)
            q = 1.0f;
        else if (q < 0.0f)
            q = 0.0f;
        if (w > 1.0f)
            w = 1.0f;
        else if (w < 0.0f)
            w = 0.0f;
        q = q * w;
        *vertical_steering_input = (1.0f - *vertical_steering_input) * q + *vertical_steering_input;
        hero.ship_flags |= 2;
    }
}

/* pvars of the class 0x4DE creature run by puffoid_update. */
typedef struct {
    u8           pad00[0x20];
    f32          unk20; /* 0x20 */
    s16          unk24; /* 0x24 */
    u8           pad26[2];
    u8           unk28; /* 0x28 */
    u8           unk29; /* 0x29 */
    u8           pad2A[6];
    f32          unk30; /* 0x30 */
    u8           pad34[0xC];
    f32          vel[4]; /* 0x40: jump velocity; [2] falls by D_L11_001622F0 * frame_time_sq */
    u8           pad50[8];
    u8           unk58; /* 0x58 */
    u8           pad59;
    u8           unk5A; /* 0x5A */
    u8           pad5B[0x6D];
    s16          unkC8; /* 0xC8 */
    u8           padCA[6];
    void        *unkD0; /* 0xD0 */
    u8           padD4[0x4C];
    u8           unk120[0x5C]; /* 0x120: mine_fall / FUN_L00_002dc9f0 state */
    f32          smoke;        /* 0x17C: nonzero: dying states give off smoke */
    s32          unk180;       /* 0x180: block set up by FUN_L01_0026d930 */
    f32          unk184;       /* 0x184 */
    f32          unk188;       /* 0x188 */
    f32          unk18C;       /* 0x18C */
    u8           pad190[0x28];
    s32          unk1B8; /* 0x1B8 */
    u8           pad1BC[0x14];
    f32          unk1D0[4]; /* 0x1D0 */
    u8           pad1E0[0x30];
    struct Moby *target; /* 0x210 */
    s32          unk214; /* 0x214: 2 = target in range */
    u8           pad218[8];
    f32          home[4]; /* 0x220 */
    s32          unk230;  /* 0x230: path index for FUN_L00_00261968 / the landing check */
    s32          path;    /* 0x234: index into l11_paths */
    s32          mode;    /* 0x238: start mode */
    u8           pad23C[8];
    s32          unk244; /* 0x244 */
    u8           pad248[4];
    f32          unk24C;    /* 0x24C: heading offset for FUN_L11_00315f58 */
    f32          unk250;    /* 0x250: turn speed */
    s32          carrier;   /* 0x254: moby index of the carrier, or < 0 */
    f32          unk258;    /* 0x258 */
    s32          point;     /* 0x25C: current path point */
    f32          unk260[4]; /* 0x260 */
    u8           pad270[0x10];
    u8           unk280[0x10]; /* 0x280 */
} CreatureVars_4318;

extern char *D_L11_0015FFD8_4318 __asm__("D_L11_0015FFD8");
extern char  D_L11_001677C0_4318[] __asm__("D_L11_001677C0");
extern char  D_0013F3D0_4318[] __asm__("D_0013F3D0");
float        D_L11_001622C0 = 0;
extern float D_L11_001622CC __attribute__((sda));

#include "rnc/overlay/l11/mobys/seesaw_platform_data/puffoid_update.h"

extern float D_L11_001622F0 __attribute__((sda));
extern void  update_puffoid_damage_and_path_goal(struct Moby *) __asm__("FUN_L11_00315968");
extern void  update_moby_target_heading_and_conditional_movement(
     char *moby, float *target_position) __asm__("FUN_L11_00315f58");
extern short
count_group_class_4de_mobys_in_states_13_to_17(struct Moby *self) __asm__("FUN_L11_00316090");
extern void  spawn_4318(struct Moby *, int, void *, float) __asm__("FUN_L00_00263ac8");
extern void  FUN_L00_0025a120_4318(struct Moby *) __asm__("FUN_L00_0025a120");
extern void  FUN_L01_0026d930_4318(void *) __asm__("FUN_L01_0026d930");
extern int   rand(void) __asm__("FUN_001160d8");
extern float FUN_L00_00259710(void *);
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern void  play_entity_sound(
     struct Moby *, void *, float, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern float jump_speed_4318(void *, void *, int, float, float) __asm__("FUN_L00_0025abf0");
extern float turn_to_4318(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern int   FUN_L00_0025c698_4318(struct Moby *, void *) __asm__("FUN_L00_0025c698");
extern void  spawn_color_swapped_explosion_burst(struct Moby *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern void follow_carrier_4318(struct Moby *, void *, void *, void *, void *, void *) __asm__("FUN_L00_00260400");
extern int   test_path_segment_strict_crossing(int, void *, void *) __asm__("FUN_L00_00261968");
extern void  move_4318(struct Moby *, void *, float, float, float, int) __asm__("FUN_L00_00258830");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern void  smoke_4318(void *, void *, float, float) __asm__("FUN_L00_0026f080");
extern int   FUN_L00_002dc9f0_4318(struct Moby *, void *) __asm__("FUN_L00_002dc9f0");

/* Update for the class 0x4DE creature: spawns on a path or a carrier moby, wanders, chases and attacks
   the target, jumps between path points, and smokes while it dies. */
void puffoid_update(struct Moby *moby) __asm__("FUN_L11_00314318");

void puffoid_update(struct Moby *moby) {
    enum {
        PUFFOID_STATE_INITIALIZE = 0,
        PUFFOID_STATE_WAIT_FOR_HOME_JUMP = 1,
        PUFFOID_STATE_IDLE = 2,
        PUFFOID_STATE_WANDER_NEAR_HOME = 3,
        PUFFOID_STATE_CHASE_TARGET = 4,
        PUFFOID_STATE_SPRAY_ATTACK = 5,
        PUFFOID_STATE_RETURN_HOME = 6,
        PUFFOID_STATE_JUMP_HOME = 7,
        PUFFOID_STATE_JUMP_THEN_WAIT_FOR_HOME_JUMP = 8,
        PUFFOID_STATE_KNOCKBACK = 9,
        PUFFOID_STATE_REPOSITION_AND_FACE_TARGET = 10,
        PUFFOID_STATE_DEATH_KNOCKBACK = 11,
        PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP = 12,
        PUFFOID_STATE_CARRIER_JUMP = 13,
        PUFFOID_STATE_CARRIER_IDLE = 14,
        PUFFOID_STATE_CARRIER_CHASE_TARGET = 15,
        PUFFOID_STATE_CARRIER_SPRAY_ATTACK = 16,
        PUFFOID_STATE_CARRIER_KNOCKBACK = 17,
        PUFFOID_STATE_CARRIER_DEATH_KNOCKBACK = 18,
        PUFFOID_STATE_FOLLOW_PATH = 19,
        PUFFOID_STATE_WAIT_TO_RESUME_COMMAND = 20
    };
    enum {
        PUFFOID_PATH_ANIM_TRAVEL = 4,
        PUFFOID_PATH_ANIM_ENTER_SPRAY_ATTACK = 5,
        PUFFOID_PATH_ANIM_SPRAY_ATTACK = 6,
        PUFFOID_PATH_ANIM_EXIT_SPRAY_ATTACK = 7
    };
    CreatureVars_4318 *puffoid_vars = (CreatureVars_4318 *)moby->pvars;
    struct Moby       *target;
    float              pos[4];
    int                i;

    if (moby->state != 0x14) {
        puffoid_vars->unkC8 = 0;
        update_puffoid_damage_and_path_goal(moby);
    }
    target = puffoid_vars->target;
    spawn_4318(moby, 0, puffoid_vars->unk280, 2.5f);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L11_001677C0_4318) < 28.0f) {
        FUN_L00_0025a120_4318(moby);
        moby->shadow_range = 0x16;
    }
    switch (moby->state) {
    case PUFFOID_STATE_INITIALIZE:
        puffoid_vars->unk20 = 2.0f;
        puffoid_vars->unk24 = 2;
        puffoid_vars->unk28 = 1;
        puffoid_vars->unk30 = 0.25f;
        puffoid_vars->unk58 = truncate_float_to_s32(8.0f);
        puffoid_vars->unk5A = truncate_float_to_s32(2.0f);
        puffoid_vars->unk29 = 1;
        puffoid_vars->unkD0 = &D_L11_001622C0;
        FUN_L01_0026d930_4318(&puffoid_vars->unk180);
        puffoid_vars->unk18C = 0.5f;
        puffoid_vars->unk188 = 0.5f;
        puffoid_vars->unk180 = truncate_float_to_s32(512.0f);
        puffoid_vars->unk184 = 0.5f;
        puffoid_vars->unk1B8 |= 0x10;
        if (rand() & 1) {
            moby->flags |= 0x8000;
        }
        if (puffoid_vars->mode == 2) {
            moby->state = 0x13;
            if (moby->prev_seq != 4) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            }
            qcopy(&moby->pos, (char *)l11_paths[puffoid_vars->path] + 0x10);
            puffoid_vars->point = 1;
        } else if (puffoid_vars->mode == 1) {
            moby->state = 1;
            qcopy(&moby->pos, (char *)l11_paths[puffoid_vars->path] + 0x10);
            moby->pos.z -= 1.0f;
            qcopy(puffoid_vars->home, (char *)l11_paths[puffoid_vars->path] + 0x20);
        } else if (puffoid_vars->carrier >= 0) {
            moby->state = 0xC;
            puffoid_vars->unk258 = random_float_between(-3.0f, 3.0f);
            moby->visible = 0;
            moby->collision = 0;
            moby->flags |= 1;
        } else {
            moby->pos.z = FUN_L00_00259710(&moby->pos);
            moby->state = 2;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 5);
            }
            qcopy(puffoid_vars->home, &moby->pos);
        }
        break;
    case PUFFOID_STATE_WAIT_FOR_HOME_JUMP: {
        char *path = (char *)l11_paths[puffoid_vars->path];
        float ang = fast_arc_tan(*(float *)(path + 0x20) - moby->pos.x,
                                 *(float *)(path + 0x24) - moby->pos.y);
        turn_to_4318(&moby->rot.z,
                     &puffoid_vars->unk250,
                     ang,
                     frame_time_sq * 8.726646f,
                     frame_time_sq * 8.726646f,
                     frame_time * 12.566371f);
        if (puffoid_vars->unk214 != 2 && random_integer_below(9) == 0) {
            moby->state = 7;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 5);
            }
            fast_vec_sub(
                puffoid_vars->vel, (char *)l11_paths[puffoid_vars->path] + 0x20, &moby->pos);
            set_vector_length_xy(puffoid_vars->vel,
                                 puffoid_vars->vel,
                                 puffoid_jump_horizontal_speed_gp * frame_time);
            puffoid_vars->vel[2] = jump_speed_4318(&moby->pos,
                                                   (char *)l11_paths[puffoid_vars->path] + 0x20,
                                                   0,
                                                   puffoid_jump_horizontal_speed_gp * frame_time,
                                                   -(D_L11_001622F0 * frame_time_sq));
        }
        break;
    }
    case PUFFOID_STATE_JUMP_HOME:
    case PUFFOID_STATE_JUMP_THEN_WAIT_FOR_HOME_JUMP:
        fast_vec_add(&moby->pos, &moby->pos, puffoid_vars->vel);
        puffoid_vars->vel[2] -= D_L11_001622F0 * frame_time_sq;
        if (puffoid_vars->vel[2] < 0.0f) {
            float h;
            qcopy(pos, &moby->pos);
            pos[2] += 1.0f;
            h = probe_ground_height(pos, 0, 0.5f);
            if (moby->pos.z < h) {
                if (moby->state == 7) {
                    moby->pos.z = h;
                    moby->state = 2;
                } else {
                    moby->state = 1;
                    moby->pos.z = h - 1.0f;
                }
            }
        }
        break;
    case PUFFOID_STATE_IDLE:
        if (puffoid_vars->unk214 != 2) {
            puffoid_vars->unk24C =
                random_float_between(-puffoid_chase_heading_offset_limit_degrees_gp,
                                     puffoid_chase_heading_offset_limit_degrees_gp) *
                0.017453292f;
            moby->state = 4;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if ((moby->anim_flags & 2) && random_integer_below(3) != 0 && moby->visible != 0 &&
                   fast_vec_dist(&moby->pos, D_L11_001677C0_4318) < 32.0f) {
            float a = random_angle_radians();
            float wander_radius = random_float_between(1.0f, 3.0f);
            puffoid_vars->unk260[0] = fast_cos(a) * wander_radius;
            puffoid_vars->unk260[1] = fast_sin(a) * wander_radius;
            puffoid_vars->unk260[2] = 0.0f;
            fast_vec_add(puffoid_vars->unk260, puffoid_vars->unk260, puffoid_vars->home);
            moby->state = 3;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case PUFFOID_STATE_WANDER_NEAR_HOME:
        update_moby_target_heading_and_conditional_movement((char *)moby, puffoid_vars->unk260);
        if (puffoid_vars->unk214 != 2) {
            puffoid_vars->unk24C =
                random_float_between(-puffoid_chase_heading_offset_limit_degrees_gp,
                                     puffoid_chase_heading_offset_limit_degrees_gp) *
                0.017453292f;
            moby->state = 4;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if ((moby->anim_flags & 2) && random_integer_below(3) == 0) {
            moby->state = 2;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case PUFFOID_STATE_CHASE_TARGET:
        if ((moby->anim_flags & 2) && vector_distance_xy(&moby->pos, &target->pos) <
                                          puffoid_spray_target_distance_metric_threshold_gp) {
            moby->state = 5;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        } else {
            update_moby_target_heading_and_conditional_movement((char *)moby,
                                                                (float *)&target->pos);
        }
        if (puffoid_vars->unk214 == 2) {
            moby->state = 6;
        }
        break;
    case PUFFOID_STATE_REPOSITION_AND_FACE_TARGET:
        fast_vec_sub(pos, puffoid_vars->unk1D0, &moby->pos);
        pos[2] = 0.0f;
        fast_vec_normalize(pos, pos, 2.5f);
        fast_vec_add(pos, pos, &moby->pos);
        if (test_path_segment_strict_crossing(puffoid_vars->unk230, &moby->pos, pos)) {
            float *rz = &moby->rot.z;
            float  ang = fast_arc_tan(puffoid_vars->target->pos.x - moby->pos.x,
                                     puffoid_vars->target->pos.y - moby->pos.y);
            turn_to_4318(rz,
                         &puffoid_vars->unk250,
                         ang,
                         frame_time_sq * 12.566371f,
                         frame_time_sq * 12.566371f,
                         frame_time * 12.566371f);
        } else {
            update_moby_target_heading_and_conditional_movement((char *)moby, puffoid_vars->unk1D0);
        }
        if (moby->anim_flags & 2) {
            if (puffoid_vars->unk214 != 2) {
                moby->state = 4;
            } else if (puffoid_vars->unk244 == 0) {
                moby->state = 6;
            }
        }
        break;
    case PUFFOID_STATE_SPRAY_ATTACK:
        update_moby_target_heading_and_conditional_movement((char *)moby, (float *)&target->pos);
        if ((moby->anim_flags & 2) && puffoid_spray_target_distance_metric_threshold_gp <
                                          vector_distance_xy(&moby->pos, &target->pos)) {
            moby->state = 4;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (puffoid_vars->unk214 == 2) {
            moby->state = 6;
        }
        qcopy(pos, &moby->pos);
        pos[2] += 0.333f;
        play_entity_sound(moby, pos, D_L11_001622CC * 0.6f, 1, 1.0f, 1.0f, 0, 1, 0);
        break;
    case PUFFOID_STATE_RETURN_HOME:
        if (vector_distance_xy(&moby->pos, puffoid_vars->home) < 1.0f) {
            moby->state = 2;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        } else if (puffoid_vars->unk214 != 2) {
            puffoid_vars->unk24C =
                random_float_between(-puffoid_chase_heading_offset_limit_degrees_gp,
                                     puffoid_chase_heading_offset_limit_degrees_gp) *
                0.017453292f;
            moby->state = 4;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else {
            update_moby_target_heading_and_conditional_movement((char *)moby, puffoid_vars->home);
        }
        if ((moby->anim_flags & 2) && moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, 5);
        }
        break;
    case PUFFOID_STATE_KNOCKBACK: {
        int hit = FUN_L00_0025c698_4318(moby, puffoid_vars->unk120);
        if (hit & 0x40) {
            char *path;
            if (moby->pos.z < 222.0f) {
                mark_moby_for_removal(moby);
                return;
            }
            path = (char *)l11_paths[puffoid_vars->unk230];
            if (point_in_path_xy(&moby->pos, path + 0x10, *(int *)path) == 0) {
                mark_moby_for_removal(moby);
                return;
            }
            moby->state = 2;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
            return;
        }
        if (hit & 0x120) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case PUFFOID_STATE_DEATH_KNOCKBACK:
        if (FUN_L00_0025c698_4318(moby, puffoid_vars->unk120) & 0x160) {
            clear_u64_value(pos);
            spawn_color_swapped_explosion_burst(moby, &moby->pos, -1, 0.5f, 10.0f);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP: {
        float basis[16];
        float dest[4];
        if (puffoid_vars->unk214 != 2 && count_group_class_4de_mobys_in_states_13_to_17(moby) < 5) {
            struct Moby *carrier_moby =
                (struct Moby *)(D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8));
            transpose_rotation_matrix(basis, &carrier_moby->rot_row0);
            fast_vec_sub(pos, &moby->pos, &carrier_moby->pos);
            transform_vector(pos, pos, basis);
            if (pos[0] < puffoid_vars->unk258 && pos[0] > -3.1f) {
                float lo;
                float hi;
                int   scaled_animation_frames;
                dest[0] = pos[0];
                if (pos[1] > 0.0f) {
                    dest[1] = random_float_between(1.5f, 2.0f);
                } else {
                    dest[1] = random_float_between(-2.0f, -1.5f);
                }
                dest[2] = 1.0f;
                transform_vector_by_basis(dest, dest, &carrier_moby->rot_row0);
                fast_vec_add(dest, dest, &carrier_moby->pos);
                moby->state = 0xD;
                if (moby->prev_seq != 1) {
                    blend_moby_animation(moby, 1, 0, 5);
                }
                fast_vec_sub(puffoid_vars->vel, dest, &moby->pos);
                scaled_animation_frames = scale_game_frames(0x3C);
                moby->anim_speed = (float)scaled_animation_frames *
                                   (puffoid_jump_horizontal_speed_gp * frame_time) /
                                   fast_vec_length(puffoid_vars->vel);
                set_vector_length_xy(puffoid_vars->vel,
                                     puffoid_vars->vel,
                                     puffoid_jump_horizontal_speed_gp * frame_time);
                puffoid_vars->vel[2] =
                    jump_speed_4318(&moby->pos,
                                    dest,
                                    0,
                                    puffoid_jump_horizontal_speed_gp * frame_time,
                                    -(D_L11_001622F0 * frame_time_sq));
                moby->rot.z = fast_arc_tan(puffoid_vars->vel[0], puffoid_vars->vel[1]);
            }
        }
        break;
    }
    case PUFFOID_STATE_CARRIER_JUMP: {
        char *cv =
            (char *)((struct Moby *)(D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8)))->pvars;
        fast_vec_add(&moby->pos, &moby->pos, puffoid_vars->vel);
        fast_vec_add(&moby->pos, &moby->pos, cv + 0x70);
        puffoid_vars->vel[2] -= D_L11_001622F0 * frame_time_sq;
        if (puffoid_vars->vel[2] < 0.0f) {
            qcopy(pos, &moby->pos);
            pos[2] += 1.0f;
            if (moby->pos.z < probe_ground_height(pos, 0, 0.5f)) {
                moby->anim_speed = 1.0f;
                moby->state = 0xE;
            }
        }
        break;
    }
    case PUFFOID_STATE_CARRIER_IDLE:
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        if (puffoid_vars->unk214 != 2) {
            puffoid_vars->unk24C =
                random_float_between(-puffoid_chase_heading_offset_limit_degrees_gp,
                                     puffoid_chase_heading_offset_limit_degrees_gp) *
                0.017453292f;
            moby->state = 0xF;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case PUFFOID_STATE_CARRIER_CHASE_TARGET:
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        update_moby_target_heading_and_conditional_movement((char *)moby, (float *)&target->pos);
        if ((moby->anim_flags & 2) && vector_distance_xy(&moby->pos, &target->pos) <
                                          puffoid_spray_target_distance_metric_threshold_gp) {
            moby->state = 0x10;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        }
        if (puffoid_vars->unk214 == 2) {
            moby->state = 0xE;
        }
        break;
    case PUFFOID_STATE_CARRIER_SPRAY_ATTACK:
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        update_moby_target_heading_and_conditional_movement((char *)moby, (float *)&target->pos);
        if ((moby->anim_flags & 2) && puffoid_spray_target_distance_metric_threshold_gp <
                                          vector_distance_xy(&moby->pos, &target->pos)) {
            moby->state = 0xF;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (puffoid_vars->unk214 == 2) {
            moby->state = 0xE;
        }
        qcopy(pos, &moby->pos);
        pos[2] += 0.333f;
        play_entity_sound(moby, pos, D_L11_001622CC * 0.6f, 1, 1.0f, 0.0f, 0, 1, 0);
        break;
    case PUFFOID_STATE_CARRIER_KNOCKBACK: {
        int hit;
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        hit = FUN_L00_0025c698_4318(moby, puffoid_vars->unk120);
        if (hit & 0x40) {
            char *path;
            if (moby->pos.z < 222.0f) {
                mark_moby_for_removal(moby);
                return;
            }
            path = (char *)l11_paths[puffoid_vars->unk230];
            if (point_in_path_xy(&moby->pos, path + 0x10, *(int *)path) == 0) {
                mark_moby_for_removal(moby);
                return;
            }
            moby->state = 0xE;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
            return;
        }
        if (hit & 0x120) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case PUFFOID_STATE_CARRIER_DEATH_KNOCKBACK:
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        if (FUN_L00_0025c698_4318(moby, puffoid_vars->unk120) & 0x160) {
            clear_u64_value(pos);
            spawn_color_swapped_explosion_burst(moby, &moby->pos, -1, 0.5f, 10.0f);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case PUFFOID_STATE_FOLLOW_PATH: {
        float vel[4];
        float path_vertical_step_limit;
        float path_spray_distance_metric;
        qcopy(pos, (char *)l11_paths[puffoid_vars->path] + puffoid_vars->point * 16 + 0x10);
        if (puffoid_vars->carrier >= 0) {
            float h = ((struct Moby *)D_L11_0015FFD8_4318)[puffoid_vars->carrier].pos.z - 1.0f;
            if (h < pos[2]) {
                pos[2] = h;
            }
        }
        {
            float *rz = &moby->rot.z;
            float  ang = fast_arc_tan(pos[0] - moby->pos.x, pos[1] - moby->pos.y);
            turn_to_4318(rz,
                         &puffoid_vars->unk250,
                         ang,
                         frame_time_sq * 12.566371f,
                         frame_time_sq * 12.566371f,
                         frame_time * 12.566371f);
        }
        vel[0] = fast_cos(moby->rot.z) * (frame_time + frame_time);
        vel[1] = fast_sin(moby->rot.z) * (frame_time + frame_time);
        vel[2] = pos[2] - moby->pos.z;
        path_vertical_step_limit = frame_time * 4.0f;
        if (vel[2] > path_vertical_step_limit) {
            vel[2] = path_vertical_step_limit;
        } else if (vel[2] < -path_vertical_step_limit) {
            vel[2] = -path_vertical_step_limit;
        }
        move_4318(moby, vel, 0.5f, 0.5f, 0.0f, 0x10);
        if (fast_vec_dist(pos, &moby->pos) < 1.0f) {
            puffoid_vars->point =
                (puffoid_vars->point + 1) % *(int *)(char *)l11_paths[puffoid_vars->path];
        }
        if (moby->anim_time == 0.0f) {
            path_spray_distance_metric = vector_distance_xy(&moby->pos, D_0013F3D0_4318);
            switch (moby->prev_seq) {
            case PUFFOID_PATH_ANIM_TRAVEL:
                if (path_spray_distance_metric < 6.0f) {
                    blend_moby_animation(moby, 5, 0, scale_game_frames(10));
                }
                break;
            case PUFFOID_PATH_ANIM_SPRAY_ATTACK:
                if (path_spray_distance_metric > 8.0f) {
                    blend_moby_animation(moby, 7, 0, scale_game_frames(10));
                } else if (path_spray_distance_metric < 4.0f) {
                    float spray_position[4];
                    qcopy(spray_position, &moby->pos);
                    spray_position[2] += 0.5f;
                    play_entity_sound(
                        moby, spray_position, D_L11_001622CC * 0.55f, 1, 1.0f, 1.0f, 0, 1, 0);
                }
                break;
            case PUFFOID_PATH_ANIM_ENTER_SPRAY_ATTACK:
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
                break;
            case PUFFOID_PATH_ANIM_EXIT_SPRAY_ATTACK:
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
                break;
            }
        }
        break;
    }
    case PUFFOID_STATE_WAIT_TO_RESUME_COMMAND:
        if (puffoid_vars->unkC8 != 0 && FUN_L00_002dc9f0_4318(moby, puffoid_vars->unk120)) {
            moby->state = moby->command;
            puffoid_vars->unkC8 = 0;
        }
        break;
    }
    if (puffoid_vars->smoke != 0.0f) {
        if (moby->state == 9 || moby->state == 0xB || moby->state == 0x11 || moby->state == 0x12) {
            for (i = 0; i < 3; i++) {
                float vel[4];
                float a;
                float smoke_horizontal_step;
                pos[0] = moby->pos.x + randf_sym(0.0f, 0.2f);
                pos[1] = moby->pos.y + randf_sym(0.0f, 0.2f);
                pos[2] = moby->pos.z + randf_sym(0.0f, 0.2f) + 0.8f;
                a = random_angle_radians();
                smoke_horizontal_step = random_float_between(0.2f, 1.3f) * frame_time;
                vel[0] = fast_cos(a) * smoke_horizontal_step;
                vel[1] = fast_sin(a) * smoke_horizontal_step;
                vel[2] = 0.0f;
                vel[2] = random_float_between(-0.2f, 0.4f) * frame_time;
                smoke_4318(pos, vel, random_float_between(2100.0f, 14700.0f), 222.5f);
            }
        }
    }
}
