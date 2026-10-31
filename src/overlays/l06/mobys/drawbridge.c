/* Level 06 moby class 0x404: drawbridge. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F6790), where it is exact; names translated to the US level program. */

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002B5990), where it is exact; names translated to the US level program. */
#include "rnc/overlay/cuboid.h"

extern int D_L06_0015F5C4;

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002DB0E0), where it is exact; names translated to the US level program. */
extern f32  advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");

/* Level 6 drawbridge (class 1028): spawns its two rows of deck pieces, waits for the hero's switch, then
   lowers through three hinge stages (D_L06_00161E08/0C/10) while the camera follows the path nodes. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F69D0), where it is exact; names translated to the US level program. */
typedef struct Hero Hero;
typedef struct Moby Moby;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2F69D0;

typedef struct Data {
    char  p0[0x60];
    int   node_start;
    int   node_mid;
    int   node_end;
    float fade;
    Moby *a[20];
    Moby *b[20];
    Moby *c[10];
    Moby *d[10];
} Data;

typedef struct {
    char          p0[0x74];
    unsigned char b74;
    char          p75[0x13];
    struct Moby  *p88;
} Slot;

extern unsigned char D_001413F4_r[] __asm__("D_001413F4");
extern char         *create_moby(int);
extern unsigned char D_0013D4D6[];
extern float         drawbridge_extension_gp __asm__("D_L06_00161D78") __attribute__((sda));
extern float         drawbridge_pitch_degrees_gp __asm__("D_L06_00161D7C") __attribute__((sda));
extern float         drawbridge_yaw_degrees_gp __asm__("D_L06_00161D80") __attribute__((sda));
extern void          set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
void                 jet_clear_persistent(int idx) __asm__("FUN_L00_002502a0");
void         enter_hero_idle_state_for_current_vehicle_mode(void) __asm__("FUN_L06_00239528");
extern float D_L06_0015F3FC;
float        drawbridge_extension_max_speed_gp __asm__("D_L06_00161D84") = 1e+01f;
float        drawbridge_pitch_max_speed_gp __asm__("D_L06_00161D88") = 3e+02f;
float        drawbridge_yaw_max_speed_gp __asm__("D_L06_00161D8C") = 3e+02f;
float        drawbridge_extension_acceleration_gp __asm__("D_L06_00161D90") = 15.0f;
float        drawbridge_pitch_acceleration_gp __asm__("D_L06_00161D94") = 2e+02f;
float        drawbridge_yaw_acceleration_gp __asm__("D_L06_00161D98") = 2e+02f;

float drawbridge_extension_speed __asm__("D_L06_00161D9C") = 0;

float        drawbridge_pitch_speed_gp __asm__("D_L06_00161DA0") = 0;
float        drawbridge_yaw_speed_gp __asm__("D_L06_00161DA4") = 0;
float        drawbridge_pitch_start_extension_gp __asm__("D_L06_00161DAC") = 5.0f;
float        drawbridge_yaw_start_pitch_gp __asm__("D_L06_00161DB0") = 3e+02f;
extern void  update_drawbridge_segment_transforms(Moby *) __asm__("FUN_L06_002f5d78");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern int   func_L00_0028EB98_2F69D0(Moby *, int) __asm__("FUN_L00_0028d8c0");
extern void  release_voice_slot(int) __asm__("FUN_0022d798");
extern void  enter_hero_idle_state_for_current_vehicle_mode(void) __asm__("FUN_L06_00239528");
extern int   help_request(int, int) __asm__("FUN_L00_00203908");
extern void  queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
void         drawbridge_update(Moby *moby) __asm__("FUN_L06_002f55a0");

/* Blarg drawbridge pieces: places and rotates the 20 + 10 chained segment mobys. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F71A8), where it is exact; names translated to the US level program. */
typedef struct DrawbridgeVars {
    char  p0[0x60];
    int   i60;
    int   i64;
    int   i68;
    float f6C;
    Moby *a[20];
    Moby *b[20];
    Moby *c[10];
    Moby *d[10];
} DrawbridgeVars;

extern char drawbridge_first_row_local_offset_gp[4] __asm__("D_L06_00161D50") __attribute__((sda));
extern char drawbridge_second_row_local_offset_gp[4] __asm__("D_L06_00161D60") __attribute__((sda));
float       drawbridge_min_segment_distance_base_gp __asm__("D_L06_00161D70") = -0.6f;
float       drawbridge_min_segment_distance_step_gp __asm__("D_L06_00161D74") = 0.25f;
extern float drawbridge_extension __asm__("D_L06_00161D78") __attribute__((sda));
extern float drawbridge_pitch_degrees __asm__("D_L06_00161D7C") __attribute__((sda));
extern float drawbridge_yaw_degrees __asm__("D_L06_00161D80") __attribute__((sda));
void         update_drawbridge_segment_transforms(Moby *moby) __asm__("FUN_L06_002f5d78");

#include "sda.h"

float drawbridge_extension_gp __asm__("D_L06_00161D78") = 0;

float drawbridge_pitch_degrees_gp __asm__("D_L06_00161D7C") = 0;

float drawbridge_yaw_degrees_gp __asm__("D_L06_00161D80") = 0;

/* Moby class 1028 */
void drawbridge_update(Moby *moby) {
    enum {
        INITIALIZE_DRAWBRIDGE_SEGMENTS = 0,
        WAIT_FOR_DRAWBRIDGE_ACTIVATION = 1,
        FADE_BEFORE_DRAWBRIDGE_ANIMATION = 3,
        ANIMATE_DRAWBRIDGE_AND_CAMERA = 4,
        FADE_AFTER_DRAWBRIDGE_ANIMATION = 5,
        DRAWBRIDGE_COMPLETE = 6
    };
    Vec_2F69D0 position, rotation, from_pos, from_rot, to_pos, to_rot;
    Data      *drawbridge_control_vars = (struct Data *)moby->pvars;
    float      previous_fade, blend;
    int        i, j, slot;

    switch (moby->state) {
    case INITIALIZE_DRAWBRIDGE_SEGMENTS: {
        int status = visited52[moby->mission + (current_level_index << 4)];
        if (status == 0xFF) {
            moby->state = 6;
            drawbridge_extension_gp = 20.0f;
            drawbridge_pitch_degrees_gp = 1000.0f;
            drawbridge_yaw_degrees_gp = 1000.0f;
        } else {
            moby->state = 1;
        }
        for (i = 0; i < 20; i++) {
            drawbridge_control_vars->a[i] = create_moby(0x405);
            drawbridge_control_vars->a[i]->draw_dist = 0x40;
            drawbridge_control_vars->a[i]->visible = 1;
            drawbridge_control_vars->a[i]->spawn_frame = (*(long *)&hero.moby->spawn_frame);
            drawbridge_control_vars->a[i]->flags = moby->flags;
            qcopy_nc(&drawbridge_control_vars->a[i]->pos, &moby->pos);
            qcopy_nc(&drawbridge_control_vars->a[i]->rot, &moby->rot);
            drawbridge_control_vars->b[i] = create_moby(0x405);
            drawbridge_control_vars->b[i]->draw_dist = 0x40;
            drawbridge_control_vars->b[i]->visible = 1;
            drawbridge_control_vars->b[i]->spawn_frame = (*(long *)&moby->spawn_frame);
            drawbridge_control_vars->b[i]->flags = moby->flags;
            qcopy_nc(&drawbridge_control_vars->b[i]->pos, &moby->pos);
            qcopy_nc(&drawbridge_control_vars->b[i]->rot, &moby->rot);
        }
        for (j = 0; j < 10; j++) {
            drawbridge_control_vars->c[j] = create_moby(0x406);
            drawbridge_control_vars->c[j]->draw_dist = 0x40;
            drawbridge_control_vars->c[j]->visible = 1;
            drawbridge_control_vars->c[j]->spawn_frame = (*(long *)&hero.moby->spawn_frame);
            drawbridge_control_vars->c[j]->flags = moby->flags;
            qcopy_nc(&drawbridge_control_vars->c[j]->pos, &moby->pos);
            qcopy_nc(&drawbridge_control_vars->c[j]->rot, &moby->rot);
            drawbridge_control_vars->d[j] = create_moby(0x406);
            drawbridge_control_vars->d[j]->draw_dist = 0x40;
            drawbridge_control_vars->d[j]->visible = 1;
            drawbridge_control_vars->d[j]->spawn_frame = (*(long *)&moby->spawn_frame);
            drawbridge_control_vars->d[j]->flags = moby->flags;
            qcopy_nc(&drawbridge_control_vars->d[j]->pos, &moby->pos);
            qcopy_nc(&drawbridge_control_vars->d[j]->rot, &moby->rot);
        }
        update_drawbridge_segment_transforms(moby);
        break;
    }
    case WAIT_FOR_DRAWBRIDGE_ACTIVATION:
        if (D_001413F4_r[0] == 1 && D_0013D4D6[0] != 0 && D_L06_0015F5C4 == 0) {
            D_L06_0015F3FC = 1.0f;
            moby->state = 3;
            drawbridge_control_vars->fade = -0.01f;
        }
        break;
    case 2:
        break;
    case FADE_BEFORE_DRAWBRIDGE_ANIMATION:
        previous_fade = drawbridge_control_vars->fade;
        approach_value(&drawbridge_control_vars->fade, 1.0f, 1.0f / scale_game_frames(20));
        D_L06_0015F3FC = 1.0f - absolute_float(drawbridge_control_vars->fade);
        if (previous_fade < 0.0f && 0.0f <= drawbridge_control_vars->fade) {
            set_race_camera((unsigned char *)&hero + 0x80, (unsigned char *)&hero + 0x90, 0x72, 1);
            jet_mount(&l06_cuboids[drawbridge_control_vars->node_start].pos,
                      &l06_cuboids[drawbridge_control_vars->node_start].rot,
                      1,
                      0,
                      0);
        } else if (1.0f <= drawbridge_control_vars->fade) {
            moby->state = 4;
            moby->command = allocate_voice_for_target_entry(0, 4, drawbridge_control_vars->c[9]);
        }
        break;
    case ANIMATE_DRAWBRIDGE_AND_CAMERA:
        jet_clear_persistent(moby->mission);
        if (drawbridge_extension_gp < 20.0f) {
            advance_accelerated_scalar(&drawbridge_extension_gp,
                                       &drawbridge_extension_speed,
                                       20.0f,
                                       drawbridge_extension_acceleration_gp * frame_time_sq,
                                       drawbridge_extension_acceleration_gp * frame_time_sq,
                                       drawbridge_extension_max_speed_gp * frame_time);
        }
        if (drawbridge_extension_gp > drawbridge_pitch_start_extension_gp &&
            drawbridge_pitch_degrees_gp < 1000.0f) {
            advance_accelerated_scalar(&drawbridge_pitch_degrees_gp,
                                       &drawbridge_pitch_speed_gp,
                                       1000.0f,
                                       drawbridge_pitch_acceleration_gp * frame_time_sq,
                                       drawbridge_pitch_acceleration_gp * frame_time_sq,
                                       drawbridge_pitch_max_speed_gp * frame_time);
        }
        if (drawbridge_pitch_degrees_gp > drawbridge_yaw_start_pitch_gp &&
            drawbridge_yaw_degrees_gp < 1000.0f) {
            advance_accelerated_scalar(&drawbridge_yaw_degrees_gp,
                                       &drawbridge_yaw_speed_gp,
                                       1000.0f,
                                       drawbridge_yaw_acceleration_gp * frame_time_sq,
                                       drawbridge_yaw_acceleration_gp * frame_time_sq,
                                       drawbridge_yaw_max_speed_gp * frame_time);
        }
        if (1000.0f <= drawbridge_yaw_degrees_gp) {
            moby->state = 5;
            drawbridge_control_vars->fade = -1.0f;
        }
        slot = moby->command;
        if (20.0f <= drawbridge_extension_gp) {
            if (func_L00_0028EB98_2F69D0(drawbridge_control_vars->c[9], slot) != 0) {
                if (slot != -1 &&
                    ((Slot *)((char *)&voice_pool + slot * 0x70))->p88 ==
                        drawbridge_control_vars->c[9] &&
                    ((Slot *)((char *)&voice_pool + slot * 0x70))->b74 != 0) {
                    release_voice_slot(slot);
                }
                slot = -1;
                moby->command = slot;
            }
        }
        update_drawbridge_segment_transforms(moby);
        if (drawbridge_extension_gp < 20.0f) {
            blend = drawbridge_extension_gp / 20.0f;
            qcopy_nc(&from_pos, &l06_cuboids[drawbridge_control_vars->node_start].pos);
            qcopy_nc(&to_pos, &l06_cuboids[drawbridge_control_vars->node_mid].pos);
            qcopy_nc(&from_rot, &l06_cuboids[drawbridge_control_vars->node_start].rot);
            qcopy_nc(&to_rot, &l06_cuboids[drawbridge_control_vars->node_mid].rot);
        } else {
            blend = drawbridge_yaw_degrees_gp / 1000.0f;
            qcopy_nc(&from_pos, &l06_cuboids[drawbridge_control_vars->node_mid].pos);
            qcopy_nc(&to_pos, &l06_cuboids[drawbridge_control_vars->node_end].pos);
            qcopy_nc(&from_rot, &l06_cuboids[drawbridge_control_vars->node_mid].rot);
            qcopy_nc(&to_rot, &l06_cuboids[drawbridge_control_vars->node_end].rot);
        }
        lerp_vector(&position, &from_pos, &to_pos, blend);
        rotation.x = fast_subtract_rotations(to_rot.x, from_rot.x) * blend;
        rotation.x = fast_add_rotations(from_rot.x, rotation.x);
        rotation.y = fast_subtract_rotations(to_rot.y, from_rot.y) * blend;
        rotation.y = fast_add_rotations(from_rot.y, rotation.y);
        rotation.z = fast_subtract_rotations(to_rot.z, from_rot.z) * blend;
        rotation.z = fast_add_rotations(from_rot.z, rotation.z);
        set_camera_position(&position);
        set_camera_rotation(&rotation);
        break;
    case FADE_AFTER_DRAWBRIDGE_ANIMATION:
        previous_fade = drawbridge_control_vars->fade;
        approach_value(&drawbridge_control_vars->fade, 1.0f, 1.0f / scale_game_frames(20));
        D_L06_0015F3FC = 1.0f - absolute_float(drawbridge_control_vars->fade);
        if (previous_fade < 0.0f && 0.0f <= drawbridge_control_vars->fade) {
            enter_hero_idle_state_for_current_vehicle_mode();
            jet_end_mount(3);
        } else if (1.0f <= drawbridge_control_vars->fade) {
            moby->state = 6;
            help_request(0x1776, 0x2D);
            queue_dialogue_message(0x177C, -1);
            memcard_save_data(0, -1);
        }
        break;
    case DRAWBRIDGE_COMPLETE:
        break;
    }
}

void update_drawbridge_segment_transforms(Moby *moby) {
    Vec_2F69D0 segment_position, segment_longitudinal_offset, staggered_segment_longitudinal_offset;
    DrawbridgeVars *drawbridge_vars = (DrawbridgeVars *)moby->pvars;
    int             i, j;
    float           length, angle, left, min_length, right, limit;

    for (i = 0; i < 20; i++) {
        length = drawbridge_extension - (float)i;
        if (length < 0.0f)
            length = 0.0f;
        fast_vec_normalize(&segment_longitudinal_offset, &moby->rot_row0, length);
        transform_vector_by_basis(
            &segment_position, (Vec_2F69D0 *)drawbridge_first_row_local_offset_gp, &moby->rot_row0);
        fast_vec_add(&segment_position, &segment_position, &moby->pos);
        fast_vec_add(&segment_position, &segment_position, &segment_longitudinal_offset);
        qcopy(&drawbridge_vars->a[i]->pos, &segment_position);
        transform_vector_by_basis(&segment_position,
                                  (Vec_2F69D0 *)drawbridge_second_row_local_offset_gp,
                                  &moby->rot_row0);
        fast_vec_add(&segment_position, &segment_position, &moby->pos);
        fast_vec_add(&segment_position, &segment_position, &segment_longitudinal_offset);
        qcopy(&drawbridge_vars->b[i]->pos, &segment_position);
        angle = drawbridge_pitch_degrees * 0.017453292f - (float)(i * 45) * 0.017453292f;
        if (angle > 1.5707964f)
            angle = 1.5707964f;
        else if (angle < 0.0f)
            angle = 0.0f;
        drawbridge_vars->a[i]->rot.x = -angle;
        drawbridge_vars->b[i]->rot.x = angle;
        update_entity(drawbridge_vars->a[i]);
        update_entity(drawbridge_vars->b[i]);
    }

    for (j = 0; j < 10; j++) {
        min_length = drawbridge_min_segment_distance_base_gp +
                     (float)(4 - j) * drawbridge_min_segment_distance_step_gp;
        length = drawbridge_extension - j * 2.0f;
        right = length - 1.0f;
        if (length < min_length)
            length = min_length;
        if (right < min_length)
            right = min_length;
        fast_vec_normalize(&segment_longitudinal_offset, &moby->rot_row0, length);
        fast_vec_normalize(&staggered_segment_longitudinal_offset, &moby->rot_row0, right);
        transform_vector_by_basis(
            &segment_position, (Vec_2F69D0 *)drawbridge_first_row_local_offset_gp, &moby->rot_row0);
        fast_vec_add(&segment_position, &segment_position, &moby->pos);
        fast_vec_add(&segment_position, &segment_position, &segment_longitudinal_offset);
        qcopy(&drawbridge_vars->c[j]->pos, &segment_position);
        transform_vector_by_basis(&segment_position,
                                  (Vec_2F69D0 *)drawbridge_second_row_local_offset_gp,
                                  &moby->rot_row0);
        fast_vec_add(&segment_position, &segment_position, &moby->pos);
        fast_vec_add(&segment_position, &segment_position, &staggered_segment_longitudinal_offset);
        qcopy(&drawbridge_vars->d[j]->pos, &segment_position);

        left = drawbridge_pitch_degrees * 0.017453292f - (float)(j * 2) * 0.7853982f;
        limit = left - drawbridge_pitch_speed_gp * 0.017453292f;
        if (left > 1.5707964f)
            left = 1.5707964f;
        else if (left < 0.0f)
            left = 0.0f;
        if (limit > 1.5707964f)
            limit = 1.5707964f;
        else if (limit < 0.0f)
            limit = 0.0f;
        if (left != 0.0f && limit == 0.0f)
            allocate_voice_for_target_entry(1, 0, drawbridge_vars->c[j]);

        right = drawbridge_pitch_degrees * 0.017453292f - (float)(j * 2) * 0.7853982f - 0.7853982f;
        limit = right - drawbridge_pitch_speed_gp * 0.017453292f;
        if (right > 1.5707964f)
            right = 1.5707964f;
        else if (right < 0.0f)
            right = 0.0f;
        if (limit > 1.5707964f)
            limit = 1.5707964f;
        else if (limit < 0.0f)
            limit = 0.0f;
        if (right != 0.0f && limit == 0.0f)
            allocate_voice_for_target_entry(1, 0, drawbridge_vars->d[j]);

        drawbridge_vars->c[j]->rot.x = -left;
        drawbridge_vars->d[j]->rot.x = right;

        left = drawbridge_yaw_degrees * 0.017453292f - (float)(j * 2) * 0.7853982f;
        limit = left - drawbridge_yaw_speed_gp * 0.017453292f;
        if (left > 1.5707964f)
            left = 1.5707964f;
        else if (left < 0.0f)
            left = 0.0f;
        if (limit > 1.5707964f)
            limit = 1.5707964f;
        else if (limit < 0.0f)
            limit = 0.0f;
        if (left != 0.0f && limit == 0.0f)
            allocate_voice_for_target_entry(2, 0, drawbridge_vars->c[j]);

        right = drawbridge_yaw_degrees * 0.017453292f - (float)(j * 2) * 0.7853982f - 0.7853982f;
        limit = left - drawbridge_yaw_speed_gp * 0.017453292f;
        if (right > 1.5707964f)
            right = 1.5707964f;
        else if (right < 0.0f)
            right = 0.0f;
        if (limit > 1.5707964f)
            limit = 1.5707964f;
        else if (limit < 0.0f)
            limit = 0.0f;
        if (right != 0.0f && limit == 0.0f)
            allocate_voice_for_target_entry(2, 0, drawbridge_vars->d[j]);

        drawbridge_vars->c[j]->rot.y = -left;
        drawbridge_vars->d[j]->rot.y = -right;
        update_entity(drawbridge_vars->c[j]);
        update_entity(drawbridge_vars->d[j]);
    }
}
