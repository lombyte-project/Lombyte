/* Camera type 18, the moby-controlled camera: takes over while the hero is inside its moby's volume, and the release a moby calls to hand it back for a frame. */

#include "types.h"
#include "rnc/camera/update_cam.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/camera/follow_cam.h"
#include "rnc/input/pad_state.h"
#include "rnc/overlay/shared/camera/set_nonzero_follow_camera_parameters.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/shared/camera.h"
#include "rnc/overlay/shared/rendering/culling/test_point_in_transformed_unit_cylinder.h"
#include "rnc/rendering/clip_volume.h"

typedef struct MobyCameraControl {
    u8    pad0[0xc];
    s32   activation_clip_volume;
    s32   activation_cylinder;
    s32   activation_path;
    u8    pad18[0x8];
    s16   active_frames;
    u8    activation_by_distance;
    u8    pad23[0x1];
    f32   activation_distance;
    s32   target_moby_index; /* 0x28 */
    f32   axis_angle_limit_degrees;
    f32   residual_angle_limit_degrees;
    f32   camera_distance;
    f32   camera_height;
    s16   mode;
    s16   idle_frames; /* counts up while active_frames is past the stick hold time */
    f32   heading;
    s32   target_group;
    char *target; /* 0x48: moby the camera follows */
    f32   camera_offset_scale;
    s32   disabled;
} MobyCameraControl;

extern int  test_point_in_transformed_unit_sphere(void *, int) __asm__("FUN_L00_0025faf0");
extern int  point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern void clear_follow_camera_volume_if_current(char *) __asm__("FUN_L00_002e8388");
extern int  try_select_follow_camera_volume(char *) __asm__("FUN_L00_002e83c0");
extern void
set_follow_camera_angle_blend_from_direction(float, float, float *) __asm__("FUN_L00_002e8918");
extern void
set_follow_camera_angle_blend_from_position(float, float, void *) __asm__("FUN_L00_002e8970");

extern char                *D_L02_001B0AB0[];
extern char                *D_L02_00167400_d __asm__("D_L02_00167400") NOT_SDA;
extern char                *l02_moby_record_table __asm__("D_L02_0015EF50");
extern char *D_L02_001674DC NOT_SDA;
extern char                 D_L02_00167410[] NOT_SDA;
extern LevelCamera          l02_camera_lui __asm__("D_L02_00167280") NOT_SDA;
extern int                  D_L02_0015FFD8;
extern int                  D_L02_001ABE40[];
extern struct Hero          hero_lui __asm__("D_0013F350") NOT_SDA;
extern char                *D_L02_00167400_lui __asm__("D_L02_00167400") NOT_SDA;

char *D_L03_00166FDC NOT_SDA = 0;
int D_L03_001AB940   NOT_SDA = 0;
char *D_L06_0016765C NOT_SDA = 0;
char *D_L07_00166F5C NOT_SDA = 0;
int D_L07_001AB8C0   NOT_SDA = 0;
char *D_L08_001676DC NOT_SDA = 0;
char *D_L12_0016735C NOT_SDA = 0;
char *D_L18_001678DC NOT_SDA = 0;

int test_moby_camera_control_activation(char *moby) __asm__("FUN_L02_002fb648");

int test_moby_camera_control_activation(char *moby) {
    char              *special = D_L02_001674DC;
    MobyCameraControl *camera_control_entry = (MobyCameraControl *)(*(
        char **)(l02_moby_record_table + *(short *)(moby + 0x84) * 32 + 0x1c));
    char              *target = camera_control_entry->target;
    int               *activation_path_record;
    int                hero_containment_result;

    if (special != 0 && special == target) {
        return 1;
    }
    if (camera_control_entry->mode == 6) {
        return (char *)hero.ground_moby == target;
    }
    if (camera_control_entry->activation_by_distance == 1) {
        return fast_vec_dist(&hero.motion.pos, target + 0x10) <
               camera_control_entry->activation_distance;
    }
    if (camera_control_entry->activation_clip_volume >= 0) {
        hero_containment_result = is_point_inside_clip_volume(
            &hero.motion.pos, camera_control_entry->activation_clip_volume);
        if (hero_containment_result == 0) {
            goto rejected;
        }
        return 1;
    }
    if (camera_control_entry->activation_cylinder >= 0) {
        hero_containment_result = test_point_in_transformed_unit_cylinder(
            &hero.motion.pos, camera_control_entry->activation_cylinder);
        if (hero_containment_result == 0) {
            goto rejected;
        }
        return 1;
    }
    if (*(int *)((char *)camera_control_entry + 8) >= 0) {
        hero_containment_result = test_point_in_transformed_unit_sphere(
            &hero.motion.pos, *(int *)((char *)camera_control_entry + 8));
        if (hero_containment_result == 0) {
            goto rejected;
        }
        return 1;
    }
    if (camera_control_entry->activation_path < 0) {
        goto rejected;
    }
    activation_path_record = (int *)D_L02_001B0AB0[camera_control_entry->activation_path];
    hero_containment_result =
        point_in_path_xy(&hero.motion.pos, activation_path_record + 4, *activation_path_record);
    if (hero_containment_result == 0) {
        goto rejected;
    }
    return 1;
rejected:
    return 0;
}

/* Checks whether the target's direction is within a and b degrees of the reference axis. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FCBC0), where it is exact; names translated to the US level program. */
int test_moby_camera_target_angle_limits(char *moby,
                                         float residual_angle_limit_degrees,
                                         float b) __asm__("FUN_L02_002fb788");

int test_moby_camera_target_angle_limits(char *moby, float residual_angle_limit_degrees, float b) {
    float target_displacement[4];
    float scaled_axis_vector[4];
    float target_axis_residual[4];
    float r[4];
    char *moby_camera_control_entry;
    char *shared_axis_data = D_L02_00167410;
    char *g;
    float dot1;
    float dot2;
    float lenq;
    float lenr;
    float dot3;
    float den;

    char *moby_record_table = l02_moby_record_table;
    char *target_moby;

    moby_camera_control_entry = *(char **)(moby_record_table + *(short *)(moby + 0x84) * 32 + 0x1C);
    target_moby = *(char **)(moby_camera_control_entry + 0x48);
    if (residual_angle_limit_degrees == 0.0f && b == 0.0f) {
        return 1;
    }
    g = (char *)&l02_camera_lui;
    fast_vec_sub(target_displacement, &MOBY(target_moby)->pos, *(char **)(g + 0x180) + 0x30);
    dot1 = fast_vec_dot(target_displacement, (shared_axis_data + 0x30));
    fast_vec_scale(scaled_axis_vector, (shared_axis_data + 0x30), dot1);
    fast_vec_sub(target_axis_residual, target_displacement, scaled_axis_vector);
    lenq = fast_vec_length(target_axis_residual);
    dot2 = fast_vec_dot(*(char **)(g + 0x180), (shared_axis_data + 0x30));
    fast_vec_scale(scaled_axis_vector, (shared_axis_data + 0x30), dot2);
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
    if (residual_angle_limit_degrees == 0.0f) {
        return 1;
    }
    dot3 = fast_vec_dot(target_axis_residual, r);
    den = lenr * lenq;
    if (den == 0.0f) {
        return 0;
    }
    if (absolute_float(1.5707964f - fast_arc_sin(dot3 / den)) <
        residual_angle_limit_degrees * DEG_TO_RAD) {
        return 1;
    }
    return 0;
}

void apply_moby_camera_control(char *moby) __asm__("FUN_L02_002fb9c8");

void apply_moby_camera_control(char *moby) {
    MobyCameraControl *moby_camera_control_record = (MobyCameraControl *)(*(
        char **)(l02_moby_record_table + *(short *)(moby + 0x84) * 32 + 0x1c));
    char              *target;
    unsigned short    *group;
    int                encoded_group_target;
    int                index;
    int                duration;
    int                count;
    int                end;
    int                frames;
    int                active;
    int                hit;
    float              scale;
    float              angle;
    float              heading;
    float              amount;
    float              heading_vector[3];

    if (moby_camera_control_record->disabled != 0) {
        moby_camera_control_record->active_frames = 0;
        clear_follow_camera_volume_if_current(moby);
        return;
    }
    index = moby_camera_control_record->target_group;
    if (index >= 0) {
        group = (unsigned short *)D_L02_001ABE40[index];
        if (group == 0) {
            moby_camera_control_record->active_frames = -1;
            clear_follow_camera_volume_if_current(moby);
            return;
        }
        {
            char *first_group_target;
            char *group_target_candidate;
            int   moby_table_base;
            {
                int e = *group;
                moby_table_base = D_L02_0015FFD8;
                first_group_target = (char *)(moby_table_base + ((e & 0x7fff) << 8));
                encoded_group_target = e;
            }
            if (*(signed char *)(first_group_target + 0x20) < 0) {
                do {
                    if ((id << 16) < 0) {
                        moby_camera_control_record->active_frames = -1;
                        clear_follow_camera_volume_if_current(moby);
                        return;
                    }
                    group++;
                    {
                        int e = *group;
                        moby_table_base = D_L02_0015FFD8;
                        group_target_candidate = (char *)(moby_table_base + ((e & 0x7fff) << 8));
                        encoded_group_target = e;
                    }
                } while (*(signed char *)(group_target_candidate + 0x20) < 0);
                target = group_target_candidate;
            } else {
                target = first_group_target;
            }
        }
    } else {
        target = (char *)(D_L02_0015FFD8 + (moby_camera_control_record->target_moby_index << 8));
        if (*(signed char *)(target + 0x20) < 0) {
            moby_camera_control_record->active_frames = -1;
            clear_follow_camera_volume_if_current(moby);
            return;
        }
    }
    moby_camera_control_record->target = target;
    if (moby_camera_control_record->mode == 6 && (char *)hero_lui.ground_moby != target) {
        moby_camera_control_record->active_frames = 0;
        clear_follow_camera_volume_if_current(moby);
        return;
    }
    if (try_select_follow_camera_volume(moby) <= 0) {
        moby_camera_control_record->active_frames = 0;
        return;
    }
    if (test_moby_camera_control_activation(moby) == 0) {
        moby_camera_control_record->active_frames = 0;
        clear_follow_camera_volume_if_current(moby);
        return;
    }
    if (moby_camera_control_record->mode == 5) {
        angle = fast_arc_tan(*(float *)D_L02_00167400_d, *(float *)(D_L02_00167400_d + 4));
        if (moby_camera_control_record->active_frames < 2)
            moby_camera_control_record->heading =
                *(float *)(l02_moby_record_table + *(short *)(moby + 0x84) * 32 + 0x18);
        else if (absolute_float(controller_state.analog[0]) >= 0.3f ||
                 absolute_float(controller_state.analog[1]) >= 0.3f)
            moby_camera_control_record->heading = angle;
        heading = moby_camera_control_record->heading;
        if (fast_difference_between_rotations(heading, angle) > 1.5707964f)
            fast_add_rotations(heading, 3.1415927f);
    }
    if (moby_camera_control_record->mode == 4) {
        heading = *(float *)(l02_moby_record_table + *(short *)(moby + 0x84) * 32 + 0x18);
        angle = fast_arc_tan(*(float *)D_L02_00167400_d, *(float *)(D_L02_00167400_d + 4));
        if (fast_difference_between_rotations(heading, angle) > 1.5707964f)
            heading = fast_add_rotations(heading, 3.1415927f);
        moby_camera_control_record->heading = heading;
    }
    active = 1;
    scale = 1.0f;
    moby_camera_control_record->active_frames += 1;
    if (moby_camera_control_record->mode == 1) {
        duration = scale_game_frames(300);
        count = scale_game_frames(400);
        end = scale_game_frames(560);
        frames = scale_game_frames(200);
    } else {
        duration = scale_game_frames(300);
        count = scale_game_frames(350);
        end = scale_game_frames(400);
        frames = scale_game_frames(200);
    }
    if (moby_camera_control_record->mode == 7) {
        if (controller_state.analog[0] != 0.0f)
            moby_camera_control_record->active_frames = duration;
        count = moby_camera_control_record->active_frames;
        if (count >= duration) {
            active = 0;
            moby_camera_control_record->idle_frames += 1;
            if (duration + scale_game_frames(30) < count) {
                scale = 0.1f;
                active = 1;
            }
        }
        if (moby_camera_control_record->active_frames >= end) {
            active = 1;
            moby_camera_control_record->idle_frames = 0;
            moby_camera_control_record->active_frames = 1;
        }
        if (frames < moby_camera_control_record->active_frames &&
            moby_camera_control_record->active_frames < duration) {
            moby_camera_control_record->idle_frames = duration;
            moby_camera_control_record->active_frames = frames;
        }
    } else if ((unsigned short)(moby_camera_control_record->mode - 1) < 2) {
        set_follow_camera_control_word(176);
        set_nonzero_follow_camera_parameters(1.0f, 12.0f, 0.11f);
        if (controller_state.analog[0] != 0.0f || controller_state.analog[1] != 0.0f)
            moby_camera_control_record->active_frames = duration;
        if (moby_camera_control_record->active_frames >= duration) {
            int waited = (unsigned short)moby_camera_control_record->idle_frames + 1;
            moby_camera_control_record->idle_frames = waited;
            if ((short)waited >= count) {
                if (test_moby_camera_target_angle_limits(moby, 30.0f, 0.0f) != 0)
                    moby_camera_control_record->active_frames = end;
                else
                    active = 0;
            } else {
                active = 0;
            }
        }
        if (moby_camera_control_record->active_frames >= end) {
            active = 1;
            moby_camera_control_record->idle_frames = 0;
            moby_camera_control_record->active_frames = 1;
        }
        if (frames < moby_camera_control_record->active_frames &&
            moby_camera_control_record->active_frames < duration) {
            moby_camera_control_record->idle_frames = duration;
            moby_camera_control_record->active_frames = frames;
        }
    } else if (frames < moby_camera_control_record->active_frames) {
        moby_camera_control_record->active_frames = frames;
    }
    hit = 0;
    amount = *(float *)moby_camera_control_record * scale * 0.017453292f *
             ((float)moby_camera_control_record->active_frames / convert_integer_to_float(frames));
    if (active) {
        if (test_moby_camera_target_angle_limits(
                moby,
                moby_camera_control_record->residual_angle_limit_degrees,
                moby_camera_control_record->axis_angle_limit_degrees) != 0) {
            hit = 1;
            if ((unsigned short)(moby_camera_control_record->mode - 4) < 2) {
                heading_vector[0] = fast_cos(moby_camera_control_record->heading);
                heading_vector[1] = fast_sin(moby_camera_control_record->heading);
                heading_vector[2] = 0.0f;
                set_follow_camera_angle_blend_from_direction(amount, 0.0f, heading_vector);
            } else if (moby_camera_control_record->mode == 3) {
                float speed;
                if (moby_camera_control_record->active_frames < scale_game_frames(90))
                    speed = (amount - 0.0f) * ((float)moby_camera_control_record->active_frames /
                                               convert_integer_to_float(scale_game_frames(90))) +
                            0.0f;
                else
                    speed = amount;
                set_follow_camera_angle_blend_from_position(speed, 0.0f, target + 0x10);
            } else {
                set_follow_camera_angle_blend_from_position(amount, 0.0f, target + 0x10);
            }
        }
    }
    if (moby_camera_control_record->camera_distance != 0.0f) {
        follow_camera_set_distance(0, moby_camera_control_record->camera_distance, 0.003f);
        lock_follow_camera_run_toward();
    }
    if (moby_camera_control_record->camera_height != 0.0f)
        follow_camera_set_height(moby_camera_control_record->camera_height, 0.003f);
    if (moby_camera_control_record->camera_offset_scale != 0.0f)
        follow_camera_set_offset_scale(0, moby_camera_control_record->camera_offset_scale, 0.005f);
    if (moby_camera_control_record->mode == 3) {
        float heading;
        set_follow_camera_orbit_clamp_mode(0);
        lock_follow_camera_run_toward();
        follow_camera_start_slide();
        follow_camera_set_up_spring(0.01f, 0.2f);
        if (moby_camera_control_record->active_frames >= scale_game_frames(120))
            heading = 0.03f;
        else
            heading = ((float)moby_camera_control_record->active_frames /
                       convert_integer_to_float(scale_game_frames(120))) *
                          0.02f +
                      0.01f;
        follow_camera_set_lateral_spring(heading, 0.2f);
    }
    if ((unsigned short)(moby_camera_control_record->mode - 4) < 2) {
        set_follow_camera_orbit_clamp_mode(0);
        lock_follow_camera_run_toward();
        follow_camera_start_slide();
        follow_camera_set_up_spring(0.02f, 0.2f);
        follow_camera_set_distance(0, 5.84f, 0.003f);
        if (moby_camera_control_record->mode == 4) {
            float angle;
            fast_vec_sub(heading_vector, target + 0x10, &hero.motion.pos);
            angle = fast_arc_tan(heading_vector[0], heading_vector[1]);
            if (hit && fast_difference_between_rotations(
                           angle, moby_camera_control_record->heading) > 1.9198622f) {
                if (moby_camera_control_record->camera_distance != 0.0f)
                    follow_camera_set_distance(
                        0, moby_camera_control_record->camera_distance + 3.36f, 0.003f);
                else
                    follow_camera_set_distance(0, 8.0f, 0.003f);
            }
        }
        set_follow_camera_control_word(176);
    }
    return;
}

extern char *target_reference_table __asm__("D_L06_0015EF50");

/* Releases the camera for one frame: update_moby_camera_control clears the flag it sets. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00317770), where it is exact; names translated to the US level program. */
void release_moby_camera_control(char *moby) __asm__("FUN_L06_00316330");

void release_moby_camera_control(char *moby) {
    if (((struct UpdateCam *)moby)->camera_class == 0x12) {
        *(int *)(*(char **)(target_reference_table + *(short *)(moby + 0x84) * 32 + 0x1C) + 0x50) =
            1;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FD6D0), where it is exact; names translated to the US level program. */
int update_moby_camera_control(char *moby) __asm__("FUN_L02_002fc298");

int update_moby_camera_control(char *moby) {
    char *moby_camera_control_data =
        *(char **)(l02_moby_record_table + *(short *)(moby + 0x84) * 32 + 0x1C);
    if (*(short *)(D_L02_00167400_lui + 0x86) == 0 &&
        *(short *)(moby_camera_control_data + 0x20) >= 0)
        apply_moby_camera_control(moby);
    *(int *)(moby_camera_control_data + 0x50) = 0;
    return 0;
}

char *D_L02_001674DC NOT_SDA = 0;
