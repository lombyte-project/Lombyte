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
extern float         D_L06_00161D78_gp __asm__("D_L06_00161D78") __attribute__((sda));
extern float         D_L06_00161D7C_gp __asm__("D_L06_00161D7C") __attribute__((sda));
extern float         D_L06_00161D80_gp __asm__("D_L06_00161D80") __attribute__((sda));
extern void          set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
void                 jet_clear_persistent(int idx) __asm__("FUN_L00_002502a0");
void         enter_hero_idle_state_for_current_vehicle_mode(void) __asm__("FUN_L06_00239528");
extern float D_L06_0015F3FC;
extern float D_L06_00161D84_gp __asm__("D_L06_00161D84") __attribute__((sda));
extern float D_L06_00161D88_gp __asm__("D_L06_00161D88") __attribute__((sda));
extern float D_L06_00161D8C_gp __asm__("D_L06_00161D8C") __attribute__((sda));
extern float D_L06_00161D90_gp __asm__("D_L06_00161D90") __attribute__((sda));
extern float D_L06_00161D94_gp __asm__("D_L06_00161D94") __attribute__((sda));
extern float D_L06_00161D98_gp __asm__("D_L06_00161D98") __attribute__((sda));

float D_L06_00161D9C = 0;

extern float D_L06_00161DA0_gp __asm__("D_L06_00161DA0") __attribute__((sda));
extern float D_L06_00161DA4_gp __asm__("D_L06_00161DA4") __attribute__((sda));
extern float D_L06_00161DAC_gp __asm__("D_L06_00161DAC") __attribute__((sda));
extern float D_L06_00161DB0_gp __asm__("D_L06_00161DB0") __attribute__((sda));
extern void  update_drawbridge_segment_transforms(Moby *) __asm__("FUN_L06_002f5d78");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern int   func_L00_0028EB98_2F69D0(Moby *, int) __asm__("FUN_L00_0028d8c0");
extern void  release_voice_slot(int) __asm__("FUN_0022d798");
extern void  enter_hero_idle_state_for_current_vehicle_mode(void) __asm__("FUN_L06_00239528");
extern int   help_request(int, int) __asm__("FUN_L00_00203908");
extern void  queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
void         update_drawbridge(Moby *moby) __asm__("FUN_L06_002f55a0");

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
extern float drawbridge_min_segment_distance_base_gp __asm__("D_L06_00161D70") __attribute__((sda));
extern float drawbridge_min_segment_distance_step_gp __asm__("D_L06_00161D74") __attribute__((sda));
extern float D_L06_00161D78 __attribute__((sda));
extern float D_L06_00161D7C __attribute__((sda));
extern float D_L06_00161D80 __attribute__((sda));
void         update_drawbridge_segment_transforms(Moby *moby) __asm__("FUN_L06_002f5d78");

#include "sda.h"

float D_L06_00161D78_gp __asm__("D_L06_00161D78") = 0;

float D_L06_00161D7C_gp __asm__("D_L06_00161D7C") = 0;

float D_L06_00161D80_gp __asm__("D_L06_00161D80") = 0;

void update_drawbridge(Moby *moby) {
    enum {
        INITIALIZE_DRAWBRIDGE_SEGMENTS = 0,
        WAIT_FOR_DRAWBRIDGE_ACTIVATION = 1,
        FADE_BEFORE_DRAWBRIDGE_ANIMATION = 3,
        ANIMATE_DRAWBRIDGE_AND_CAMERA = 4,
        FADE_AFTER_DRAWBRIDGE_ANIMATION = 5,
        DRAWBRIDGE_COMPLETE = 6
    };
    Vec_2F69D0 position, rotation, from_pos, from_rot, to_pos, to_rot;
    Data      *data = (struct Data *)moby->pvars;
    float      previous_fade, blend;
    int        i, j, slot;

    switch (moby->state) {
    case INITIALIZE_DRAWBRIDGE_SEGMENTS: {
        int status = visited52[moby->mission + (current_level_index << 4)];
        if (status == 0xFF) {
            moby->state = 6;
            D_L06_00161D78_gp = 20.0f;
            D_L06_00161D7C_gp = 1000.0f;
            D_L06_00161D80_gp = 1000.0f;
        } else {
            moby->state = 1;
        }
        for (i = 0; i < 20; i++) {
            data->a[i] = create_moby(0x405);
            data->a[i]->draw_dist = 0x40;
            data->a[i]->visible = 1;
            data->a[i]->spawn_frame = (*(long *)&hero.moby->spawn_frame);
            data->a[i]->flags = moby->flags;
            qcopy_nc(&data->a[i]->pos, &moby->pos);
            qcopy_nc(&data->a[i]->rot, &moby->rot);
            data->b[i] = create_moby(0x405);
            data->b[i]->draw_dist = 0x40;
            data->b[i]->visible = 1;
            data->b[i]->spawn_frame = (*(long *)&moby->spawn_frame);
            data->b[i]->flags = moby->flags;
            qcopy_nc(&data->b[i]->pos, &moby->pos);
            qcopy_nc(&data->b[i]->rot, &moby->rot);
        }
        for (j = 0; j < 10; j++) {
            data->c[j] = create_moby(0x406);
            data->c[j]->draw_dist = 0x40;
            data->c[j]->visible = 1;
            data->c[j]->spawn_frame = (*(long *)&hero.moby->spawn_frame);
            data->c[j]->flags = moby->flags;
            qcopy_nc(&data->c[j]->pos, &moby->pos);
            qcopy_nc(&data->c[j]->rot, &moby->rot);
            data->d[j] = create_moby(0x406);
            data->d[j]->draw_dist = 0x40;
            data->d[j]->visible = 1;
            data->d[j]->spawn_frame = (*(long *)&moby->spawn_frame);
            data->d[j]->flags = moby->flags;
            qcopy_nc(&data->d[j]->pos, &moby->pos);
            qcopy_nc(&data->d[j]->rot, &moby->rot);
        }
        update_drawbridge_segment_transforms(moby);
        break;
    }
    case WAIT_FOR_DRAWBRIDGE_ACTIVATION:
        if (D_001413F4_r[0] == 1 && D_0013D4D6[0] != 0 && D_L06_0015F5C4 == 0) {
            D_L06_0015F3FC = 1.0f;
            moby->state = 3;
            data->fade = -0.01f;
        }
        break;
    case 2:
        break;
    case FADE_BEFORE_DRAWBRIDGE_ANIMATION:
        previous_fade = data->fade;
        approach_value(&data->fade, 1.0f, 1.0f / scale_game_frames(20));
        D_L06_0015F3FC = 1.0f - absolute_float(data->fade);
        if (previous_fade < 0.0f && 0.0f <= data->fade) {
            set_race_camera((unsigned char *)&hero + 0x80, (unsigned char *)&hero + 0x90, 0x72, 1);
            jet_mount(
                &l06_cuboids[data->node_start].pos, &l06_cuboids[data->node_start].rot, 1, 0, 0);
        } else if (1.0f <= data->fade) {
            moby->state = 4;
            moby->command = allocate_voice_for_target_entry(0, 4, data->c[9]);
        }
        break;
    case ANIMATE_DRAWBRIDGE_AND_CAMERA:
        jet_clear_persistent(moby->mission);
        if (D_L06_00161D78_gp < 20.0f) {
            advance_accelerated_scalar(&D_L06_00161D78_gp,
                                       &D_L06_00161D9C,
                                       20.0f,
                                       D_L06_00161D90_gp * frame_time_sq,
                                       D_L06_00161D90_gp * frame_time_sq,
                                       D_L06_00161D84_gp * frame_time);
        }
        if (D_L06_00161D78_gp > D_L06_00161DAC_gp && D_L06_00161D7C_gp < 1000.0f) {
            advance_accelerated_scalar(&D_L06_00161D7C_gp,
                                       &D_L06_00161DA0_gp,
                                       1000.0f,
                                       D_L06_00161D94_gp * frame_time_sq,
                                       D_L06_00161D94_gp * frame_time_sq,
                                       D_L06_00161D88_gp * frame_time);
        }
        if (D_L06_00161D7C_gp > D_L06_00161DB0_gp && D_L06_00161D80_gp < 1000.0f) {
            advance_accelerated_scalar(&D_L06_00161D80_gp,
                                       &D_L06_00161DA4_gp,
                                       1000.0f,
                                       D_L06_00161D98_gp * frame_time_sq,
                                       D_L06_00161D98_gp * frame_time_sq,
                                       D_L06_00161D8C_gp * frame_time);
        }
        if (1000.0f <= D_L06_00161D80_gp) {
            moby->state = 5;
            data->fade = -1.0f;
        }
        slot = moby->command;
        if (20.0f <= D_L06_00161D78_gp) {
            if (func_L00_0028EB98_2F69D0(data->c[9], slot) != 0) {
                if (slot != -1 &&
                    ((Slot *)((char *)&voice_pool + slot * 0x70))->p88 == data->c[9] &&
                    ((Slot *)((char *)&voice_pool + slot * 0x70))->b74 != 0) {
                    release_voice_slot(slot);
                }
                slot = -1;
                moby->command = slot;
            }
        }
        update_drawbridge_segment_transforms(moby);
        if (D_L06_00161D78_gp < 20.0f) {
            blend = D_L06_00161D78_gp / 20.0f;
            qcopy_nc(&from_pos, &l06_cuboids[data->node_start].pos);
            qcopy_nc(&to_pos, &l06_cuboids[data->node_mid].pos);
            qcopy_nc(&from_rot, &l06_cuboids[data->node_start].rot);
            qcopy_nc(&to_rot, &l06_cuboids[data->node_mid].rot);
        } else {
            blend = D_L06_00161D80_gp / 1000.0f;
            qcopy_nc(&from_pos, &l06_cuboids[data->node_mid].pos);
            qcopy_nc(&to_pos, &l06_cuboids[data->node_end].pos);
            qcopy_nc(&from_rot, &l06_cuboids[data->node_mid].rot);
            qcopy_nc(&to_rot, &l06_cuboids[data->node_end].rot);
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
        previous_fade = data->fade;
        approach_value(&data->fade, 1.0f, 1.0f / scale_game_frames(20));
        D_L06_0015F3FC = 1.0f - absolute_float(data->fade);
        if (previous_fade < 0.0f && 0.0f <= data->fade) {
            enter_hero_idle_state_for_current_vehicle_mode();
            jet_end_mount(3);
        } else if (1.0f <= data->fade) {
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
    Vec_2F69D0      result, first, second;
    DrawbridgeVars *data = (DrawbridgeVars *)moby->pvars;
    int             i, j;
    float           length, angle, left, min_length, right, limit;

    for (i = 0; i < 20; i++) {
        length = D_L06_00161D78 - (float)i;
        if (length < 0.0f)
            length = 0.0f;
        fast_vec_normalize(&first, &moby->rot_row0, length);
        transform_vector_by_basis(
            &result, (Vec_2F69D0 *)drawbridge_first_row_local_offset_gp, &moby->rot_row0);
        fast_vec_add(&result, &result, &moby->pos);
        fast_vec_add(&result, &result, &first);
        qcopy(&data->a[i]->pos, &result);
        transform_vector_by_basis(
            &result, (Vec_2F69D0 *)drawbridge_second_row_local_offset_gp, &moby->rot_row0);
        fast_vec_add(&result, &result, &moby->pos);
        fast_vec_add(&result, &result, &first);
        qcopy(&data->b[i]->pos, &result);
        angle = D_L06_00161D7C * 0.017453292f - (float)(i * 45) * 0.017453292f;
        if (angle > 1.5707964f)
            angle = 1.5707964f;
        else if (angle < 0.0f)
            angle = 0.0f;
        data->a[i]->rot.x = -angle;
        data->b[i]->rot.x = angle;
        update_entity(data->a[i]);
        update_entity(data->b[i]);
    }

    for (j = 0; j < 10; j++) {
        min_length = drawbridge_min_segment_distance_base_gp +
                     (float)(4 - j) * drawbridge_min_segment_distance_step_gp;
        length = D_L06_00161D78 - j * 2.0f;
        right = length - 1.0f;
        if (length < min_length)
            length = min_length;
        if (right < min_length)
            right = min_length;
        fast_vec_normalize(&first, &moby->rot_row0, length);
        fast_vec_normalize(&second, &moby->rot_row0, right);
        transform_vector_by_basis(
            &result, (Vec_2F69D0 *)drawbridge_first_row_local_offset_gp, &moby->rot_row0);
        fast_vec_add(&result, &result, &moby->pos);
        fast_vec_add(&result, &result, &first);
        qcopy(&data->c[j]->pos, &result);
        transform_vector_by_basis(
            &result, (Vec_2F69D0 *)drawbridge_second_row_local_offset_gp, &moby->rot_row0);
        fast_vec_add(&result, &result, &moby->pos);
        fast_vec_add(&result, &result, &second);
        qcopy(&data->d[j]->pos, &result);

        left = D_L06_00161D7C * 0.017453292f - (float)(j * 2) * 0.7853982f;
        limit = left - D_L06_00161DA0_gp * 0.017453292f;
        if (left > 1.5707964f)
            left = 1.5707964f;
        else if (left < 0.0f)
            left = 0.0f;
        if (limit > 1.5707964f)
            limit = 1.5707964f;
        else if (limit < 0.0f)
            limit = 0.0f;
        if (left != 0.0f && limit == 0.0f)
            allocate_voice_for_target_entry(1, 0, data->c[j]);

        right = D_L06_00161D7C * 0.017453292f - (float)(j * 2) * 0.7853982f - 0.7853982f;
        limit = right - D_L06_00161DA0_gp * 0.017453292f;
        if (right > 1.5707964f)
            right = 1.5707964f;
        else if (right < 0.0f)
            right = 0.0f;
        if (limit > 1.5707964f)
            limit = 1.5707964f;
        else if (limit < 0.0f)
            limit = 0.0f;
        if (right != 0.0f && limit == 0.0f)
            allocate_voice_for_target_entry(1, 0, data->d[j]);

        data->c[j]->rot.x = -left;
        data->d[j]->rot.x = right;

        left = D_L06_00161D80 * 0.017453292f - (float)(j * 2) * 0.7853982f;
        limit = left - D_L06_00161DA4_gp * 0.017453292f;
        if (left > 1.5707964f)
            left = 1.5707964f;
        else if (left < 0.0f)
            left = 0.0f;
        if (limit > 1.5707964f)
            limit = 1.5707964f;
        else if (limit < 0.0f)
            limit = 0.0f;
        if (left != 0.0f && limit == 0.0f)
            allocate_voice_for_target_entry(2, 0, data->c[j]);

        right = D_L06_00161D80 * 0.017453292f - (float)(j * 2) * 0.7853982f - 0.7853982f;
        limit = left - D_L06_00161DA4_gp * 0.017453292f;
        if (right > 1.5707964f)
            right = 1.5707964f;
        else if (right < 0.0f)
            right = 0.0f;
        if (limit > 1.5707964f)
            limit = 1.5707964f;
        else if (limit < 0.0f)
            limit = 0.0f;
        if (right != 0.0f && limit == 0.0f)
            allocate_voice_for_target_entry(2, 0, data->d[j]);

        data->c[j]->rot.y = -left;
        data->d[j]->rot.y = -right;
        update_entity(data->c[j]);
        update_entity(data->d[j]);
    }
}
