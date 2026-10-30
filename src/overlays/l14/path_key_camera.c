/* Level 14 path key camera type (camera 21): setup, update, priority check and exit. */
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "types.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/camera/update_cam.h"

extern int D_001413DC NOT_SDA;

extern u8 *D_L14_00167500 NOT_SDA;

void exit_path_key_camera(u8 *target_object) __asm__("FUN_L14_003167e0");

#include "rnc/overlay/l14/path_key_camera_globals.h"

extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");

extern float interpolate_angle(float, float, float) __asm__("FUN_L00_0025b750");

extern char *D_L14_0015EF50 __asm__("D_L14_0015EF50");

extern char *D_L14_0015EF50_59d8 __asm__("D_L14_0015EF50");

extern float D_L14_001F6C80[][1000];

extern float D_L14_001F6488_59d8[] __asm__("D_L14_001F6488");

extern float D_L14_001624A8 __attribute__((sda));

extern void
compute_position_from_spherical_offset(void *, void *, void *) __asm__("FUN_L00_001ed580");

void initialize_path_key_camera(struct UpdateCam *cam) __asm__("FUN_L14_003159d8");

void initialize_path_key_camera(struct UpdateCam *cam) {
    float        camera_placement_parameters[4];
    float        pad[4]; /* unused; keeps the frame layout */
    float        hero_look_direction[4];
    float        up[4];
    char        *rec = *(char **)(D_L14_0015EF50_59d8 + cam->descriptor_index * 32 + 0x1C);
    char        *path;
    LevelSpline *path_record_table;
    char        *camera_saved_state;
    float       *camera_parameters;
    int         *camera_state_block;
    int          j;
    int         *camera_parameter_state_block;
    int         *camera_key_default_cursor;
    float        sum;
    float        hero_heading_radians;
    int          i;

    path_record_table = l14_splines;
    *(int *)(rec + 0x20) = 2;
    *(short *)(rec + 0x30) = 0;
    *(short *)(rec + 0x32) = 0;
    path = (char *)path_record_table[*(int *)(rec + 0x34)].path;
    sum = 0.0f;
    for (i = 0; i < *(int *)path - 1; i++) {
        D_L14_001F6C80[0][i] = sum;
        sum += *(float *)(path + i * 16 + 0x1C);
    }
    D_L14_001F6C80[0][i] = sum;
    path = (char *)l14_splines[*(int *)(rec + 0x38)].path;
    sum = 0.0f;
    for (i = 0; i < *(int *)path - 1; i++) {
        D_L14_001F6C80[1][i] = sum;
        sum += *(float *)(path + i * 16 + 0x1C);
    }
    D_L14_001F6C80[1][i] = sum;
    /* retail reloads the spline table for the third path */
    path = (char *)(*(LevelSpline *volatile *)&l14_splines)[*(int *)(rec + 0x3C)].path;
    sum = 0.0f;
    for (i = 0; i < *(int *)path - 1; i++) {
        D_L14_001F6C80[2][i] = sum;
        sum += *(float *)(path + i * 16 + 0x1C);
    }
    D_L14_001F6C80[2][i] = sum;
    camera_saved_state = cam->saved_state;
    *(int *)(camera_saved_state + 0x88) = 1;
    camera_state_block = (int *)(camera_saved_state + 0x88);
    camera_state_block[1] = 0;
    camera_state_block[2] = 0;
    camera_state_block[3] = 0;
    camera_state_block[4] = 0;
    D_L14_001624A8 = 1.0f;
    camera_parameters = (float *)((char *)cam->saved_state + 0x60);
    camera_parameters[0] = D_L14_001F6488_59d8[0] * 0.017453292f;
    camera_parameters[1] = D_L14_001F6488_59d8[1] * 0.017453292f;
    camera_parameters[2] = D_L14_001F6488_59d8[2];
    camera_parameters[3] = D_L14_001F6488_59d8[3] * 0.017453292f;
    camera_parameters[4] = D_L14_001F6488_59d8[4] * 0.017453292f;
    camera_parameter_state_block = (int *)((char *)cam->saved_state + 0x74);
    *camera_parameter_state_block = 0;
    camera_parameter_state_block[1] = 0;
    camera_parameter_state_block[2] = 0;
    camera_parameter_state_block[3] = 0;
    camera_parameter_state_block[4] = 0;
    hero_heading_radians = fast_arc_tan(hero.moby->rot_row1.x, hero.moby->rot_row1.y);
    camera_placement_parameters[1] = camera_parameters[1];
    camera_placement_parameters[0] = hero_heading_radians;
    camera_placement_parameters[2] = camera_parameters[2];
    compute_position_from_spherical_offset(&cam->pos, camera_placement_parameters, &hero.motion);
    fast_vec_sub(hero_look_direction, &hero.motion, &cam->pos);
    fast_vec_scale(up, &hero.gravity_dir, -1.5f);
    fast_vec_add(hero_look_direction, hero_look_direction, up);
    fast_vec_normalize(&cam->m0, hero_look_direction, 1.0f);
    fast_vec_cross(&cam->m1, &cam->m0, up);
    fast_vec_normalize(&cam->m1, &cam->m1, 1.0f);
    fast_vec_cross(&cam->m2, &cam->m1, &cam->m0);
    /* give every key without a duration (+0x44) the default 10000 */
    camera_key_default_cursor = (int *)&D_L14_001F6488_59d8[0x11];
    for (j = 38; j >= 0; j--) {
        if (*camera_key_default_cursor == 0)
            goto fix;
    next:
        camera_key_default_cursor += 12;
        continue;
    fix:
        *camera_key_default_cursor = 10000;
        goto next;
    }
}

typedef struct {
    char  pad0[0x24];
    int   m24;
    int   m28;
    int   m2C;
    short n30;
    short n32;
    int   p34;
    int   p38;
} CamRec5d48;

typedef struct {
    int   count;
    int   idx;
    float f8;
    float fC;
    float f10;
} CamState5d48;

typedef struct {
    float f0;
    float f4;
    float f8;
    float fC;
    float f10;
} CamAngles5d48;

typedef struct {
    char          pad0[0x60];
    CamAngles5d48 cur;
    float         vel[5];
    CamState5d48  st;
} CamData5d48;

typedef struct {
    char         pad0[0x70];
    CamData5d48 *data;
    char         pad1[0x10];
    short        slot;
} CamObj5d48;

typedef struct {
    char        pad0[0x1C];
    CamRec5d48 *rec;
} CamSlot5d48;

typedef struct {
    char pad0[0x60];
    int  zone;
    char pad1[0x164];
    int  timer;
} CamMobyData5d48;

typedef struct {
    float a0;
    float a4;
    float a8;
    float aC;
    float a10;
    int   n14;
    float t18;
    float t1C;
    float t20;
    float t24;
    float t28;
    float t2C;
} CamKey5d48;

typedef struct {
    char  pad0[0x274];
    float f274;
    float f278;
} CamBones5d48;

#define MOBYDATA_5d48(m) ((CamMobyData5d48 *)(m)->pvars)

extern CamSlot5d48 *D_L14_0015EF50_macro __asm__("D_L14_0015EF50") MACRO_ADDR;

extern LevelSpline *l14_splines_macro __asm__("D_L14_0015F70C") MACRO_ADDR;

extern char *D_L14_0015FFD8_5d48 __asm__("D_L14_0015FFD8") MACRO_ADDR;

extern int D_L14_001624B4 MACRO_ADDR;

extern int D_L14_001624B8 MACRO_ADDR;

extern float D_L14_001624BC MACRO_ADDR;

extern float D_L14_001624C0 MACRO_ADDR;

extern float D_L14_001624AC __attribute__((sda));

extern float D_L14_001624B0 __attribute__((sda));

extern float path_key_camera_angle_override_progress_thresholds[][3] __asm__("D_L14_001F6C38");

extern float path_key_camera_linked_moby_event_progress_thresholds[][3] __asm__("D_L14_001F6C08");

extern CamKey5d48 D_L14_001F6488[];

extern char D_L14_0017AD80[];

extern float
rope_spring_turn(void *, float, float, float, float, float) __asm__("FUN_L00_001eb328");

extern float cam_interp_values(void *, float, float, float, float, float) __asm__("FUN_001ebd78");

extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

void update_path_key_camera(char *cam) __asm__("FUN_L14_00315d48");

void update_path_key_camera(char *path_key_camera) {
    CamData5d48   *data = ((CamObj5d48 *)path_key_camera)->data;
    CamState5d48  *st = &data->st;
    CamRec5d48    *rec = D_L14_0015EF50_macro[((CamObj5d48 *)path_key_camera)->slot].rec;
    CamAngles5d48 *cur = &data->cur;
    float         *vel = data->vel;
    int            side;
    float          key_interpolation_fraction;
    float          k;
    float          x;
    float          camera_placement_heading;
    float          current_key_heading_override;
    float          heading_blend_target;
    float          camera_placement_parameters[4];
    float          unused[4];
    CamKey5d48    *keys;
    int            next;
    float          look_direction[4];
    float          scaled_up_vector[4];

    D_L14_001624B4 = data->st.count;
    D_L14_001624B8 = st->idx;
    if (hero.rail_spline == (s32)l14_splines_macro[rec->p34].path) {
        side = 0;
    } else if (hero.rail_spline == (s32)l14_splines_macro[rec->p38].path) {
        side = 1;
    } else {
        side = 2;
    }
    D_L14_001624C0 =
        (D_L14_001F6C80[side][hero.rail_segment] + hero.rail_seg_frac) / D_L14_001F6C80[side][999];
    if (rec->n32 < 2 &&
        path_key_camera_angle_override_progress_thresholds[rec->n32][side] <= D_L14_001624C0) {
        rec->n32++;
    }
    if (rec->n32 == 1) {
        ((CamBones5d48 *)D_L14_0017AD80)->f274 = D_L14_001624B0 * 0.017453292f;
        ((CamBones5d48 *)D_L14_0017AD80)->f278 = D_L14_001624AC * 0.017453292f;
    }
    if (rec->n30 < 3 &&
        path_key_camera_linked_moby_event_progress_thresholds[rec->n30][side] <= D_L14_001624C0) {
        struct Moby *first_linked_moby = (struct Moby *)(D_L14_0015FFD8_5d48 + (rec->m24 << 8));
        struct Moby *second_linked_moby = (struct Moby *)(D_L14_0015FFD8_5d48 + (rec->m28 << 8));
        struct Moby *third_linked_moby = (struct Moby *)(D_L14_0015FFD8_5d48 + (rec->m2C << 8));

        if (hero.rail_spline ==
            (s32)l14_splines_macro[MOBYDATA_5d48(first_linked_moby)->zone].path) {
            first_linked_moby->command = 1;
            if (rec->n30 > 0) {
                {
                    CamMobyData5d48 *d = MOBYDATA_5d48(second_linked_moby);

                    second_linked_moby->command = 5;
                    if (rec->n30 == 2) {
                        CamMobyData5d48 *d2;

                        d->timer = scale_game_frames(0x2D);
                        third_linked_moby->command = 5;
                        d2 = MOBYDATA_5d48(third_linked_moby);
                        d2->timer = scale_game_frames(0x6E);
                    } else if (rec->n30 == 3) {
                        d->timer = scale_game_frames(0x2D);
                    } else {
                        d->timer = scale_game_frames(0x50);
                    }
                }
            }
        } else if (hero.rail_spline ==
                   (s32)l14_splines_macro[MOBYDATA_5d48(second_linked_moby)->zone].path) {
            second_linked_moby->command = 1;
            if (rec->n30 > 0) {
                if (random_integer_below(2)) {
                    CamMobyData5d48 *d = MOBYDATA_5d48(first_linked_moby);

                    first_linked_moby->command = 5;
                    if (rec->n30 == 2) {
                        CamMobyData5d48 *d2;

                        d->timer = scale_game_frames(0x2D);
                        third_linked_moby->command = 5;
                        d2 = MOBYDATA_5d48(third_linked_moby);
                        d2->timer = scale_game_frames(0x6E);
                    } else if (rec->n30 == 3) {
                        d->timer = scale_game_frames(0x2D);
                    } else {
                        d->timer = scale_game_frames(0x50);
                    }
                } else {
                    CamMobyData5d48 *d = MOBYDATA_5d48(third_linked_moby);

                    third_linked_moby->command = 5;
                    if (rec->n30 == 2) {
                        CamMobyData5d48 *d2;

                        d->timer = scale_game_frames(0x2D);
                        first_linked_moby->command = 5;
                        d2 = MOBYDATA_5d48(first_linked_moby);
                        d2->timer = scale_game_frames(0x6E);
                    } else if (rec->n30 == 3) {
                        d->timer = scale_game_frames(0x2D);
                    } else {
                        d->timer = scale_game_frames(0x50);
                    }
                }
            }
        } else {
            third_linked_moby->command = 1;
            if (rec->n30 > 0) {
                {
                    CamMobyData5d48 *d = MOBYDATA_5d48(second_linked_moby);

                    second_linked_moby->command = 5;
                    if (rec->n30 == 2) {
                        CamMobyData5d48 *d2;

                        d->timer = scale_game_frames(0x2D);
                        first_linked_moby->command = 5;
                        d2 = MOBYDATA_5d48(first_linked_moby);
                        d2->timer = scale_game_frames(0x6E);
                    } else if (rec->n30 == 3) {
                        d->timer = scale_game_frames(0x2D);
                    } else {
                        d->timer = scale_game_frames(0x50);
                    }
                }
            }
        }
        rec->n30++;
    }
    /* Unused reads (not in rac1-decomp's SN build): with them the game compiler computes the key table's
     * high half and the next key index above the three side tests, as retail does. */
    keys = D_L14_001F6488;
    next = st->idx + 1;
    if (side == 0 && D_L14_001F6488[st->idx + 1].t24 <= D_L14_001624C0) {
        st->idx++;
    } else if (side == 1 && D_L14_001F6488[st->idx + 1].t28 <= D_L14_001624C0) {
        st->idx++;
    } else if (side == 2 && D_L14_001F6488[st->idx + 1].t2C <= D_L14_001624C0) {
        st->idx++;
    }
    if (D_L14_001F6488[st->idx + 1].n14 == 0x2710) {
        key_interpolation_fraction = 1.0f;
        st->idx--;
    } else {
        float num;
        float den;

        if (side == 0) {
            den = D_L14_001F6488[st->idx + 1].t24 - D_L14_001F6488[st->idx].t24;
            num = D_L14_001624C0 - D_L14_001F6488[st->idx].t24;
        } else if (side == 1) {
            den = D_L14_001F6488[st->idx + 1].t28 - D_L14_001F6488[st->idx].t28;
            num = D_L14_001624C0 - D_L14_001F6488[st->idx].t28;
        } else {
            den = D_L14_001F6488[st->idx + 1].t2C - D_L14_001F6488[st->idx].t2C;
            num = D_L14_001624C0 - D_L14_001F6488[st->idx].t2C;
        }
        key_interpolation_fraction = num / den;
    }
    k = path_key_camera_heading_offset_spring_gain;
    x = interpolate_angle(D_L14_001F6488[st->idx].a0 * 0.017453292f,
                          D_L14_001F6488[st->idx + 1].a0 * 0.017453292f,
                          key_interpolation_fraction);
    cur->f0 = rope_spring_turn(vel, cur->f0, x, k, 0.2f, 0.0f);
    k = path_key_camera_placement_elevation_spring_gain;
    x = interpolate_angle(D_L14_001F6488[st->idx].a4 * 0.017453292f,
                          D_L14_001F6488[st->idx + 1].a4 * 0.017453292f,
                          key_interpolation_fraction);
    cur->f4 = rope_spring_turn(vel + 1, cur->f4, x, k, 0.2f, 0.0f);
    cur->f8 = cam_interp_values(vel + 2,
                                cur->f8,
                                D_L14_001F6488[st->idx].a8 +
                                    (D_L14_001F6488[st->idx + 1].a8 - D_L14_001F6488[st->idx].a8) *
                                        key_interpolation_fraction,
                                path_key_camera_placement_distance_spring_gain,
                                0.2f,
                                0.0f);
    k = path_key_camera_lateral_axis_rotation_spring_gain;
    x = interpolate_angle(D_L14_001F6488[st->idx].aC * 0.017453292f,
                          D_L14_001F6488[st->idx + 1].aC * 0.017453292f,
                          key_interpolation_fraction);
    cur->fC = rope_spring_turn(vel + 3, cur->fC, x, k, 0.2f, 0.0f);
    k = path_key_camera_up_axis_rotation_spring_gain;
    x = interpolate_angle(D_L14_001F6488[st->idx].a10 * 0.017453292f,
                          D_L14_001F6488[st->idx + 1].a10 * 0.017453292f,
                          key_interpolation_fraction);
    cur->f10 = rope_spring_turn(vel + 4, cur->f10, x, k, 0.2f, 0.0f);
    camera_placement_heading =
        fast_add_rotations(fast_arc_tan(hero.moby->rot_row1.x, hero.moby->rot_row1.y), cur->f0);
    D_L14_001624BC = camera_placement_heading;
    if (side == 0) {
        current_key_heading_override = D_L14_001F6488[st->idx].t18;
        heading_blend_target = D_L14_001F6488[st->idx + 1].t18;
    } else if (side == 1) {
        current_key_heading_override = D_L14_001F6488[st->idx].t1C;
        heading_blend_target = D_L14_001F6488[st->idx + 1].t1C;
    } else {
        current_key_heading_override = D_L14_001F6488[st->idx].t20;
        heading_blend_target = D_L14_001F6488[st->idx + 1].t20;
    }
    if (heading_blend_target == 0.0f && current_key_heading_override == 0.0f) {
        heading_blend_target = (st->fC != 0.0f) ? st->fC : st->f8;
        st->f10 += (0.0f - st->f10) * path_key_camera_heading_blend_step_factor;
        camera_placement_heading =
            interpolate_angle(camera_placement_heading,
                              heading_blend_target,
                              cubic_hermite_scalar(1.0f, 0.0f, 1.0f, 0.0f, st->f10));
    } else {
        if (heading_blend_target != 0.0f) {
            st->f10 += (1.0f - st->f10) * path_key_camera_heading_blend_step_factor;
            camera_placement_heading =
                interpolate_angle(camera_placement_heading,
                                  heading_blend_target,
                                  cubic_hermite_scalar(1.0f, 0.0f, 1.0f, 0.0f, st->f10));
            st->f8 = current_key_heading_override;
            st->fC = heading_blend_target;
        } else {
            st->f10 += (0.0f - st->f10) * path_key_camera_heading_blend_step_factor;
            camera_placement_heading =
                interpolate_angle(camera_placement_heading,
                                  current_key_heading_override,
                                  cubic_hermite_scalar(1.0f, 0.0f, 1.0f, 0.0f, st->f10));
            st->f8 = current_key_heading_override;
            st->fC = heading_blend_target;
        }
    }
    camera_placement_parameters[1] = cur->f4;
    camera_placement_parameters[0] = camera_placement_heading;
    camera_placement_parameters[2] = cur->f8;
    compute_position_from_spherical_offset(
        path_key_camera + 0x30, camera_placement_parameters, &hero.motion.pos);
    fast_vec_sub(look_direction, &hero.motion.pos, path_key_camera + 0x30);
    fast_vec_scale(scaled_up_vector, &hero.gravity_dir, -1.5f);
    fast_vec_add(look_direction, look_direction, scaled_up_vector);
    fast_vec_normalize(path_key_camera, look_direction, 1.0f);
    fast_vec_cross(path_key_camera + 0x10, path_key_camera, scaled_up_vector);
    if (cur->fC != 0.0f) {
        build_look_at_matrix(path_key_camera, path_key_camera, path_key_camera + 0x10, cur->fC);
        fast_vec_normalize(path_key_camera, path_key_camera, 1.0f);
        fast_vec_cross(path_key_camera + 0x10, path_key_camera, scaled_up_vector);
    }
    if (cur->f10 != 0.0f) {
        build_look_at_matrix(path_key_camera, path_key_camera, scaled_up_vector, cur->f10);
        fast_vec_normalize(path_key_camera, path_key_camera, 1.0f);
        fast_vec_cross(path_key_camera + 0x10, path_key_camera, scaled_up_vector);
    }
    fast_vec_normalize(path_key_camera + 0x10, path_key_camera + 0x10, 1.0f);
    fast_vec_cross(path_key_camera + 0x20, path_key_camera + 0x10, path_key_camera);
    st->count++;
}

extern char *D_L14_0015EF50;

int check_path_key_camera_priority(char *moby, char *other) __asm__("FUN_L14_00316748");

int check_path_key_camera_priority(char *moby, char *other) {
    char *slot_record = *(char **)(D_L14_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x1C);
    if (*(int *)(slot_record + 0x20) == 0 && *(short *)(D_L14_00167500 + 0x86) == 0x14) {
        *(int *)(slot_record + 0x20) = 1;
    } else if (*(int *)(slot_record + 0x20) == 1) {
        if (*(short *)(D_L14_00167500 + 0x86) != 0x14)
            return 1;
        if (other == 0 || *(short *)(other + 0x7E) != 0)
            return 1;
    }
    return 0;
}

void exit_path_key_camera(u8 *target_object) {
    if (D_001413DC != 0x0F) {
        *(s16 *)(target_object + 0x7E) = 3;
    }
}

float D_L14_001624A8 = 0;

int D_L14_001624B4 = 0;

int D_L14_001624B8 = 0;

float D_L14_001624BC = 0;

float D_L14_001624C0 = 0;
