/* Level 15 moby follow camera type: region and angle checks, update and dispatch. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/camera/follow_cam.h"
#include "rnc/overlay/shared/rendering/culling/test_point_in_transformed_unit_cylinder.h"

extern char *D_L15_001600EC;

extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

extern char D_0013E533[];

extern char *D_L15_00167400_lui __asm__("D_L15_00167400") NOT_SDA;

extern char *moby_slot_entry_table __asm__("D_L15_0015EF50");

extern int *D_L15_001B0AB0[];

extern int point_in_path_xy(float *p, float *v, int n) __asm__("FUN_L00_00259740");

extern char *D_L15_0015EF48[] MACRO_ADDR;

int is_hero_in_moby_follow_camera_region(char *moby) __asm__("FUN_L15_002f85a8");

int is_hero_in_moby_follow_camera_region(char *moby) {
    int *slot_data = *(int **)(D_L15_0015EF48[2] + *(short *)(moby + 0x84) * 32 + 0x1C);
    int  r;
    int  i;
    if (slot_data[10] >= 0 &&
        test_point_in_transformed_unit_cylinder(D_0013E533 + 0xE9D, slot_data[10])) {
        return 0;
    }
    if (slot_data[3] >= 0) {
        if (is_point_inside_clip_volume(D_0013E533 + 0xE9D, slot_data[3])) {
            return 1;
        }
    } else if (slot_data[4] >= 0) {
        if (test_point_in_transformed_unit_cylinder(D_0013E533 + 0xE9D, slot_data[4])) {
            return 1;
        }
    } else if (slot_data[2] >= 0) {
        if (test_point_in_transformed_unit_sphere((float *)(D_0013E533 + 0xE9D), slot_data[2])) {
            return 1;
        }
    } else if (slot_data[5] >= 0) {
        i = slot_data[5];
        if (point_in_path_xy((float *)(D_0013E533 + 0xE9D),
                             (float *)(D_L15_001B0AB0[i] + 4),
                             D_L15_001B0AB0[i][0])) {
            return 1;
        }
    }
    return 0;
}

extern char angular_limit_projection_axis_record[] __asm__("D_L15_00167410");

int check_moby_target_angular_limits(char *moby,
                                     float projected_angle_limit_degrees,
                                     float b) __asm__("FUN_L15_002f8698");

int check_moby_target_angular_limits(char *moby, float projected_angle_limit_degrees, float b) {
    float target_displacement[4];
    float scaled_axis_vector[4];
    float target_axis_residual[4];
    float r[4];
    char *data;
    char *projection_axis_record = angular_limit_projection_axis_record;
    char *g;
    float dot1;
    float dot2;
    float lenq;
    float lenr;
    float dot3;
    float den;
    char *moby_slot_table = D_L15_0015EF48[2];
    char *target_record;

    data = *(char **)(moby_slot_table + *(short *)(moby + 0x84) * 32 + 0x1C);
    target_record = D_L15_001600EC + *(int *)(data + 0x48) * 128;
    if (projected_angle_limit_degrees == 0.0f && b == 0.0f) {
        return 1;
    }
    g = (char *)&l15_camera;
    fast_vec_sub(target_displacement, target_record + 0x30, *(char **)(g + 0x180) + 0x30);
    dot1 = fast_vec_dot(target_displacement, (projection_axis_record + 0x30));
    fast_vec_scale(scaled_axis_vector, (projection_axis_record + 0x30), dot1);
    fast_vec_sub(target_axis_residual, target_displacement, scaled_axis_vector);
    lenq = fast_vec_length(target_axis_residual);
    dot2 = fast_vec_dot(*(char **)(g + 0x180), (projection_axis_record + 0x30));
    fast_vec_scale(scaled_axis_vector, (projection_axis_record + 0x30), dot2);
    fast_vec_sub(r, *(char **)(g + 0x180), scaled_axis_vector);
    lenr = fast_vec_length(r);
    if (b != 0.0f) {
        float target_axis_angle = fast_arc_tan(lenq, dot1);
        float reference_axis_angle = fast_arc_tan(lenr, dot2);
        float axis_angle_difference =
            fast_difference_between_rotations(target_axis_angle, reference_axis_angle);
        if (b * DEG_TO_RAD < axis_angle_difference) {
            return 0;
        }
    }
    if (projected_angle_limit_degrees == 0.0f) {
        return 1;
    }
    dot3 = fast_vec_dot(target_axis_residual, r);
    den = lenr * lenq;
    if (den == 0.0f) {
        return 0;
    }
    if (absolute_float(1.5707964f - fast_arc_sin(dot3 / den)) <
        projected_angle_limit_degrees * DEG_TO_RAD) {
        return 1;
    }
    return 0;
}

typedef struct {
    char  pad[0x100];
    float x;
    float y;
} G_88e8;

extern G_88e8 D_0013C940_lui __asm__("D_0013C940") NOT_SDA;

extern int try_select_follow_camera_volume(char *) __asm__("FUN_L00_002e83c0");

extern void clear_follow_camera_volume_if_current(char *) __asm__("FUN_L00_002e8388");

extern void set_nonzero_follow_camera_parameters(float, float, float) __asm__("FUN_L02_002f6598");

extern void
set_follow_camera_angle_blend_from_position(void *, float, float) __asm__("FUN_L00_002e8970");

extern void FUN_L00_002e8620(void);

void update_moby_follow_camera(char *moby) __asm__("FUN_L15_002f88e8");

void update_moby_follow_camera(char *moby) {
    char *slot_data = *(char **)(moby_slot_entry_table + *(short *)(moby + 0x84) * 32 + 0x1C);
    char *target_record = D_L15_001600EC + (*(int *)(slot_data + 0x48) << 7);
    int   done;
    int   hold_start_frames;
    int   cone_check_delay_frames;
    int   cycle_reset_frames;
    int   spin_ramp_frames;
    float spin_ramp_ratio;
    float spin_angle_radians;
    if (try_select_follow_camera_volume(moby) <= 0) {
        *(short *)(slot_data + 0x20) = 0;
        return;
    }
    if (is_hero_in_moby_follow_camera_region(moby) == 0) {
        *(short *)(slot_data + 0x20) = 0;
        clear_follow_camera_volume_if_current(moby);
        return;
    }
    *(int *)(*(char **)(D_L15_00167400_lui + 0x70) + 0x230) = 2;
    done = 1;
    (*(short *)(slot_data + 0x20))++;
    hold_start_frames = scale_game_frames(300);
    cone_check_delay_frames = scale_game_frames(400);
    cycle_reset_frames = scale_game_frames(560);
    spin_ramp_frames = scale_game_frames(200);
    set_nonzero_follow_camera_parameters(1.0f, 12.0f, 0.11f);
    if (D_0013C940_lui.x != 0.0f)
        *(short *)(slot_data + 0x20) = hold_start_frames;
    else if (D_0013C940_lui.y != 0.0f)
        *(short *)(slot_data + 0x20) = hold_start_frames;
    if (*(short *)(slot_data + 0x20) >= hold_start_frames) {
        if (++*(int *)(slot_data + 0x3C) >= cone_check_delay_frames &&
            check_moby_target_angular_limits(moby, 30.0f, 0.0f))
            *(short *)(slot_data + 0x20) = cycle_reset_frames;
        else
            done = 0;
    }
    if (*(short *)(slot_data + 0x20) >= cycle_reset_frames) {
        done = 1;
        *(int *)(slot_data + 0x3C) = 0;
        *(short *)(slot_data + 0x20) = 1;
    }
    if (spin_ramp_frames < *(short *)(slot_data + 0x20) &&
        *(short *)(slot_data + 0x20) < hold_start_frames) {
        *(int *)(slot_data + 0x3C) = hold_start_frames;
        *(short *)(slot_data + 0x20) = spin_ramp_frames;
    }
    spin_ramp_ratio = convert_integer_to_float(*(short *)(slot_data + 0x20)) /
                      convert_integer_to_float(spin_ramp_frames);
    spin_angle_radians = *(float *)slot_data * DEG_TO_RAD * spin_ramp_ratio;
    if (done)
        set_follow_camera_angle_blend_from_position(target_record + 0x30, spin_angle_radians, 0.0f);
    if (*(float *)(slot_data + 0x34) != 0.0f) {
        follow_camera_set_distance(0, *(float *)(slot_data + 0x34), 0.003f);
        FUN_L00_002e8620();
    }
    if (*(float *)(slot_data + 0x38) != 0.0f)
        follow_camera_set_height(*(float *)(slot_data + 0x38), 0.003f);
    if (*(float *)(slot_data + 0x44) != 0.0f)
        follow_camera_set_offset_scale(0, *(float *)(slot_data + 0x44), 0.005f);
}

extern unsigned char D_001413F4 NOT_SDA;

void update_moby_follow_camera(char *m) __asm__("FUN_L15_002f88e8");

int dispatch_moby_follow_camera_update(char *moby) __asm__("FUN_L15_002f8ba8");

int dispatch_moby_follow_camera_update(char *moby) {
    char *slot_data = *(char **)(D_L15_0015EF48[2] + *(short *)(moby + 0x84) * 32 + 0x1C);
    if (*(short *)(D_L15_00167400_lui + 0x86) == 0 && *(short *)(slot_data + 0x20) >= 0 &&
        D_001413F4 == 2)
        update_moby_follow_camera(moby);
    return -1;
}
