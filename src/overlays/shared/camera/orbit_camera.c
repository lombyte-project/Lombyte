/* Camera type 6: a camera orbiting the hero or a target moby. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/overlay/level_camera.h"

#define F(p, o) (*(float *)((char *)(p) + (o)))
#define F(p, o) (*(float *)((char *)(p) + (o)))

typedef struct {
    u8  pad[0x28];
    f32 f28;
    f32 f2C;
    f32 f30;
    f32 f34;
    f32 f38;
    f32 f3C;
    f32 f40;
    f32 f44;
    f32 f48;
    f32 f4C;
    u8  pad50[0x4C];
    s32 f9C;
    f32 fA0;
    f32 fA4;
    s32 fA8;
    s32 fAC;
    u8  padB0[0x40];
    s32 fF0;
} Moby002eb3b0;

typedef struct {
    u8            pad[0x70];
    Moby002eb3b0 *m;
    u8            pad78[0xA];
    s16           f7E;
} S002eb3b0;

typedef struct {
    u8  pad[0x50];
    u8  x50[0x20];
    u8 *m;
} O002ebb00;

f32          cam_interp_values(void *, f32, f32, f32, f32, f32) __asm__("FUN_001ebd78");
f32          rope_spring_turn(void *, f32, f32, f32, f32, f32) __asm__("FUN_L00_001eb328");
extern char *find_camera_record_by_id(int) __asm__("FUN_L00_001eb1f8");
void         switch_active_camera_record(u8 *) __asm__("FUN_001ebf10");
extern void
     compute_position_from_spherical_offset(void *, void *, void *) __asm__("FUN_L00_001ed580");
void copy_five_floats_with_rotation_offset(void *, void *, float) __asm__("FUN_L00_001eb0c8");
void zero_five_words(void *) __asm__("FUN_L00_001eb0b0");
void clear_twelve_bytes(void *) __asm__("FUN_L00_001ff250");
void FUN_001f9a10_2eb648(void *, void *, void *) __asm__("FUN_001f9a10");

extern LevelCamera          l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;
extern float D_L00_00161DD4 NOT_SDA;
extern short                D_L00_00166EF0_002eb930[] __asm__("D_L00_00166EF0");
extern char                 mode6_orbit_initial_values[] __asm__("D_L00_001E77C0");
extern char                 mode6_orbit_spring_stiffness_values[] __asm__("D_L00_001E77D8");
extern char                 mode6_orbit_spring_damping_values[] __asm__("D_L00_001E77F0");

float D_L01_0016224C = -0.0f;
float D_L02_00162384 = -0.0f;
float D_L03_00161E4C = -0.0f;
float D_L04_00161F44 = -0.0f;
float D_L05_0016219C = -0.0f;
float D_L06_00162564 = -0.0f;
float D_L07_00161E1C = -0.0f;
float D_L08_001625DC = -0.0f;
float D_L09_00161F34 = -0.0f;
float D_L10_00162204 = -0.0f;
float D_L11_0016276C = -0.0f;
float D_L12_00162244 = -0.0f;
float D_L13_001620BC = -0.0f;
float D_L14_00162474 = -0.0f;
float D_L15_001623C4 = -0.0f;
float D_L16_0016218C = -0.0f;
float D_L17_00162674 = -0.0f;
float D_L18_0016278C = -0.0f;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EC728), where it is exact; names translated to the US level program. */
void initialize_orbit_camera_pose(void *camera) __asm__("FUN_L00_002eb278");

void initialize_orbit_camera_pose(void *camera) {
    char *camera_object = camera;
    float rotation_matrix[16];
    char *orbit_origin = *(char **)(camera_object + 0x70) + 0xB0;
    compute_position_from_spherical_offset(
        camera_object + 0x30, camera_object + 0x50, orbit_origin);
    *(float *)(camera_object + 0x50) =
        fast_add_rotations(*(float *)(camera_object + 0x50), *(float *)(orbit_origin + 0x18));
    *(int *)(camera_object + 0x40) = 0;
    *(float *)(camera_object + 0x44) =
        fast_add_rotations(*(float *)(camera_object + 0x54), *(float *)(camera_object + 0x5C));
    *(float *)(camera_object + 0x48) =
        fast_add_rotations(3.14159274101257324f, *(float *)(camera_object + 0x50));
    *(float *)(camera_object + 0x48) =
        fast_add_rotations(*(float *)(camera_object + 0x48), *(float *)(camera_object + 0x60));
    *(int *)(camera_object + 0x4C) = 0;
    euler_rotation_matrix(rotation_matrix, camera_object + 0x40);
    copy_matrix3x4(camera_object, rotation_matrix);
}

void initialize_orbit_camera_from_parameters(char *camera_object,
                                             void *camera_initialization_values,
                                             u128 *initial_orbit_origin,
                                             float x) __asm__("FUN_L00_002eb320");

void initialize_orbit_camera_from_parameters(char *camera_object,
                                             void *camera_initialization_values,
                                             u128 *initial_orbit_origin,

                                             float x) {
    char *camera_state = *(char **)(camera_object + 0x70);
    char *orbit_origin = camera_state + 0xb0;
    qcopy(orbit_origin, initial_orbit_origin);
    qcopy(camera_state + 0x80, initial_orbit_origin);
    *(float *)(orbit_origin + 0x18) = x;
    copy_five_floats_with_rotation_offset(camera_state + 0x14, camera_initialization_values, x);
    copy_five_floats_with_rotation_offset(camera_object + 0x50, camera_initialization_values, 0.0f);
    zero_five_words(camera_state);
    clear_twelve_bytes(camera_state + 0x90);
    initialize_orbit_camera_pose(camera_object);
}

void initialize_orbit_camera_state(S002eb3b0 *camera_object) __asm__("FUN_L00_002eb3b0");

void initialize_orbit_camera_state(S002eb3b0 *camera_object) {
    Moby002eb3b0 *m;
    camera_object->m->fF0 = 0;
    m = camera_object->m;
    m->f9C = 0;
    m->fA8 = 0;
    m->fAC = 0;
    m->fA0 = 0.3f;
    m->fA4 = 0.3f;
    m->f48 = 0.99f;
    m->f3C = 0.04f;
    m->f40 = 0.04f;
    m->f44 = 0.04f;
    m->f4C = 0.04f;
    m->f28 = 0.07f;
    m->f2C = 1.0f;
    m->f30 = 0.07f;
    m->f34 = 1.0f;
    m->f38 = 0.07f;
    zero_five_words((u8 *)m + 0x50);
    zero_five_words(m);
    clear_twelve_bytes((u8 *)m + 0x90);
    initialize_orbit_camera_pose(camera_object);
    camera_object->f7E = 0;
}

void update_orbit_camera_pose(char *camera_object) __asm__("FUN_L00_002eb480");

void update_orbit_camera_pose(char *camera_object) {
    float rotation_matrix[16];
    char *camera_state = *(char **)(camera_object + 0x70);
    char *orbit_origin = camera_state + 0xB0;
    F(camera_object, 0x50) =
        rope_spring_turn(camera_state,
                         F(camera_object, 0x50),
                         fast_add_rotations(F(orbit_origin, 0x18), F(camera_state, 0x14)),
                         F(camera_state, 0x28),
                         F(camera_state, 0x3C),
                         F(camera_state, 0x50));
    F(camera_object, 0x54) = rope_spring_turn(camera_state + 4,
                                              F(camera_object, 0x54),
                                              F(camera_state, 0x18),
                                              F(camera_state, 0x2C),
                                              F(camera_state, 0x40),
                                              F(camera_state, 0x54));
    F(camera_object, 0x58) = cam_interp_values(camera_state + 8,
                                               F(camera_object, 0x58),
                                               F(camera_state, 0x1C),
                                               F(camera_state, 0x30),
                                               F(camera_state, 0x44),
                                               F(camera_state, 0x58));
    F(camera_object, 0x5C) = rope_spring_turn(camera_state + 0xC,
                                              F(camera_object, 0x5C),
                                              F(camera_state, 0x20),
                                              F(camera_state, 0x34),
                                              F(camera_state, 0x48),
                                              F(camera_state, 0x5C));
    F(camera_object, 0x60) = rope_spring_turn(camera_state + 0x10,
                                              F(camera_object, 0x60),
                                              F(camera_state, 0x24),
                                              F(camera_state, 0x38),
                                              F(camera_state, 0x4C),
                                              F(camera_state, 0x60));
    if (*(int *)(camera_state + 0x9C) == 0) {
        F(orbit_origin, 0) = cam_interp_values(camera_state + 0x90,
                                               F(orbit_origin, 0),
                                               F(camera_state, 0x80),
                                               F(camera_state, 0xA0),
                                               F(camera_state, 0xA4),
                                               F(camera_state, 0xA8));
        F(orbit_origin, 4) = cam_interp_values(camera_state + 0x94,
                                               F(orbit_origin, 4),
                                               F(camera_state, 0x84),
                                               F(camera_state, 0xA0),
                                               F(camera_state, 0xA4),
                                               F(camera_state, 0xA8));
        F(orbit_origin, 8) = cam_interp_values(camera_state + 0x98,
                                               F(orbit_origin, 8),
                                               F(camera_state, 0x88),
                                               F(camera_state, 0xA0),
                                               F(camera_state, 0xA4),
                                               F(camera_state, 0xA8));
    } else {
        qcopy(orbit_origin, camera_state + 0x80);
    }
    compute_position_from_spherical_offset(
        camera_object + 0x30, camera_object + 0x50, orbit_origin);
    F(camera_object, 0x40) = F(camera_state, 0xAC);
    F(camera_object, 0x44) = fast_add_rotations(F(camera_object, 0x54), F(camera_object, 0x5C));
    F(camera_object, 0x48) = fast_add_rotations(3.14159274101257324f, F(camera_object, 0x50));
    F(camera_object, 0x48) = fast_add_rotations(F(camera_object, 0x48), F(camera_object, 0x60));
    euler_rotation_matrix(rotation_matrix, camera_object + 0x40);
    copy_matrix3x4(camera_object, rotation_matrix);
}

int update_orbit_camera_from_target_moby(char *camera_object,
                                         char *target_moby) __asm__("FUN_L00_002eb648");

int update_orbit_camera_from_target_moby(char *camera_object, char *target_moby) {
    u128         target_orbit_origin_offset[1];
    u128         target_orbit_origin[1];
    char        *camera_state;
    float       *camera_initialization_values;
    char        *target_private_data;
    u128        *pw;
    struct Hero *hero_state;
    char        *camera_globals;
    char        *pp;
    char        *cp;
    float        hero_to_camera_bearing_difference, hero_to_camera_distance, x;
    camera_state = *(char **)(camera_object + 0x70);
    camera_initialization_values = (float *)(camera_state + 0x64);
    if (!target_moby)
        goto one;
    target_orbit_origin_offset[0] = 0;
    ((float *)target_orbit_origin_offset)[2] = 0.12f;
    ((float *)target_orbit_origin_offset)[0] = D_L00_00161DD4;
    target_private_data = ((char *)MOBY(target_moby)->pvars);
    transform_vector_by_basis(
        target_orbit_origin_offset, target_orbit_origin_offset, target_moby + 0xC0);
    FUN_001f9a10_2eb648(target_orbit_origin, target_moby + 0x10, target_orbit_origin_offset);
    pw = target_orbit_origin;
    if (*(int *)(target_private_data + 0x10) == -1) {
        copy_five_floats_with_rotation_offset(
            camera_initialization_values, camera_object + 0x50, 0.0f);
        camera_initialization_values[2] = 6.0f;
        camera_initialization_values[1] = camera_initialization_values[3];
        camera_initialization_values[3] = 0.0f;
        initialize_orbit_camera_from_parameters(
            camera_object, camera_initialization_values, target_orbit_origin, 0.0f);
    }
    if (*(int *)(target_private_data + 0x10) > 0)
        goto pos;
    *(float *)(camera_state + 0xAC) =
        fast_add_rotations(*(float *)(camera_state + 0xAC), *(float *)(target_private_data + 0x18));
    *(float *)(target_private_data + 0x18) *= 0.97f;
    hero_state = &hero;
    camera_globals = (char *)&l00_camera_lui;
    pp = &hero_state->motion.pos;
    cp = camera_globals + 0x140;
    hero_to_camera_bearing_difference =
        fast_arc_tan(*(float *)(camera_globals + 0x140) - hero_state->motion.pos.f[0],
                     *(float *)(camera_globals + 0x144) - hero_state->motion.pos.f[1]);
    hero_to_camera_bearing_difference = fast_difference_between_rotations(
        hero_to_camera_bearing_difference, *(float *)(camera_globals + 0x158));
    hero_to_camera_distance = vector_distance_xy(pp, cp);
    if (hero_to_camera_distance <
        vector_distance_xy(((char *)hero_state->items[0].moby) + 0x10, cp)) {
        x = vector_distance_xy(pp, cp);
    } else {
        x = vector_distance_xy(((char *)hero_state->items[0].moby) + 0x10, cp);
    }
    if (x < 0.77f) {
        if (hero_to_camera_bearing_difference < 1.5707964f) {
            *(float *)(camera_state + 0x1C) -= 0.7f - x;
            *(int *)(target_private_data + 4) = 0;
        } else {
            *(float *)(camera_state + 0x1C) += 0.7f - x;
        }
    } else {
        *(float *)(camera_state + 0x1C) += *(float *)(target_private_data + 4);
        *(float *)(target_private_data + 4) *= 0.97f;
    }
    qcopy(camera_state + 0x80, pw);
    return 0;
pos:
    *(float *)(camera_state + 0xAC) = MOBY(target_moby)->rot.x * 0.5f;
    *(float *)(camera_state + 0x20) +=
        (MOBY(target_moby)->rot.y - *(float *)(camera_state + 0x20)) * 0.05f;
    qcopy(camera_state + 0x80, target_orbit_origin);
    qcopy(camera_state + 0xC0, target_moby + 0x40);
one:
    return 1;
}

char *initialize_mode6_orbit_camera(void *target_moby) __asm__("FUN_L00_002eb930");

char *initialize_mode6_orbit_camera(void *target_moby) {
    char *camera_record = find_camera_record_by_id(6);
    char *camera_state;
    D_L00_00166EF0_002eb930[0] = 0;
    switch_active_camera_record(camera_record);
    update_orbit_camera_from_target_moby(camera_record, target_moby);
    camera_state = *(char **)(camera_record + 0x70);
    qcopy(camera_state + 0xB0, camera_state + 0x80);
    copy_five_floats_with_rotation_offset(camera_state + 0x14, mode6_orbit_initial_values, 0.0f);
    copy_five_floats_with_rotation_offset(camera_record + 0x50, camera_state + 0x14, 0.0f);
    initialize_orbit_camera_state(camera_record);
    *(int *)(camera_state + 0x9C) = 1;
    copy_five_floats_with_rotation_offset(
        camera_state + 0x28, mode6_orbit_spring_stiffness_values, 0.0f);
    copy_five_floats_with_rotation_offset(
        camera_state + 0x3C, mode6_orbit_spring_damping_values, 0.0f);
    zero_five_words(camera_state + 0x50);
    return camera_record;
}

void select_camera_mode_by_hero_alignment_and_distance(char *camera_object) __asm__("FUN_L00_002eba18");

void select_camera_mode_by_hero_alignment_and_distance(char *camera_object) {
    struct Hero *hero_data = &hero;
    char        *camera_globals = (char *)&l00_camera;
    char        *hero_moby = ((char *)hero_data->moby);
    float        a = fast_arc_tan(MOBY(hero_moby)->pos.x - *(float *)(camera_globals + 0x140),
                           MOBY(hero_moby)->pos.y - *(float *)(camera_globals + 0x144));
    if (fast_difference_between_rotations(*(float *)(camera_globals + 0x158), a) <
            0.5585053563117981f &&
        fast_vec_dist(&hero_data->motion.pos, camera_globals + 0x140) < 8.0f) {
        *(char *)(camera_globals + 0x273) = 0;
        *(short *)(camera_object + 0x7E) = 2;
        *(float *)(camera_globals + 0x294) = 0.018f;
        *(float *)(camera_globals + 0x288) = 0.018f;
    } else {
        *(short *)(camera_object + 0x7E) = 4;
    }
    *(char *)(camera_object + 0x7D) = 0;
}

void initialize_orbit_camera_from_hero(O002ebb00 *camera_object) __asm__("FUN_L00_002ebb00");

void initialize_orbit_camera_from_hero(O002ebb00 *camera_object) {
    u8 *m = camera_object->m;
    u8 *raised_hero_position = m + 0xB0;
    u8 *camera_state;
    qcopy(raised_hero_position, &hero.motion.pos);
    *(f32 *)(raised_hero_position + 8) += 2.0f;
    qcopy(m + 0xC0, &hero.motion.rot);
    camera_state = camera_object->m;
    qcopy(camera_state + 0x80, raised_hero_position);
    zero_five_words(camera_state + 0x14);
    *(f32 *)(camera_state + 0x1C) = 4.64f;
    copy_five_floats_with_rotation_offset(camera_object->x50, camera_state + 0x14, 0.0f);
    initialize_orbit_camera_state(camera_object);
}
