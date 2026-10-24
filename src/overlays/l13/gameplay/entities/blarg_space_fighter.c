/* Level 13 moby classes: rocket, mine, blarg_space_fighter, forcefield_tower, asteroid. */
#include "types.h"

#include "qzero.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"

/* Rotation-axis selection shared by the asteroid update helpers. */
enum AsteroidSpinAxis {
    SPIN_AXIS_X = 0x0,
    SPIN_AXIS_Y = 0x1,
    SPIN_AXIS_Z = 0x2
};

extern char *create_moby_c13(int) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8(void *);
extern char *spawn_source_attached_fading_effect_particle(char *, int, int, int, float) __asm__("FUN_L00_0026daa0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation_c13(void *, int, int, int) __asm__("FUN_00212f90");

unsigned char *spawn_level_13_homing_rocket(char *owner_moby, char *pos, char *target, char *initial_rotation, int arg, float scale,
                                float max_speed) __asm__("FUN_L13_002c13b0");

unsigned char *spawn_level_13_homing_rocket(char *owner_moby, char *rocket_spawn_position, char *target, char *initial_rotation, int initial_expiry_countdown, float scale,
                                float max_speed) {
    unsigned char *moby = (unsigned char *)create_moby_c13(0x52);
    if (moby != 0) {
        char *data = *(char **)(moby + 0x78);
        moby[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(char **)(data + 0x24) = target;
        *(char **)(data + 0x20) = owner_moby;
        qcopy(moby + 0x10, rocket_spawn_position);
        qcopy(moby + 0x40, initial_rotation);
        {
            float *sp = &scale;
            *(float *)(data + 0x2C) = *sp;
        }
        *(float *)(data + 0x38) = max_speed;
        *(int *)(data + 0x28) = initial_expiry_countdown;
        *(int *)(data + 0x30) = 0;
        *(int *)(data + 0x34) = 0;
        qcopy(data, *(char **)(owner_moby + 0x78) + 0xC0);
        if (moby[0x53] != 1) {
            blend_moby_animation_c13(moby, 1, 0, 10);
        }
        add_vector_xyz(moby + 0x10, moby + 0x10, data);
        if (*(char **)(data + 0x24) != 0) {
            qcopy(data + 0x10, *(char **)(data + 0x24) + 0x10);
        }
        *(float *)(moby + 0x2C) = *(float *)(*(char **)(moby + 0x24) + 0x24);
        FUN_L00_00250df8(moby);
        spawn_source_attached_fading_effect_particle((char *)moby, 0x60808080, initial_expiry_countdown, 0, 500000.0f);
    }
    return moby;
}
/* Ported from rac1-decomp src/overlays/l13_gemlik/vendor_002C2638.c (func_L13_002C27B0) */

/* Update for the homing rocket (class 82): smoke trail, steering toward its target, collision and expiry. */

typedef int Q_2C27B0 __attribute__((mode(TI)));

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V4_2C27B0;

typedef struct Cls_2C27B0 { char pad0[0x24]; float scale; } Cls_2C27B0;

typedef struct Moby_2C27B0 {
    char pad0[0x10];
    V4_2C27B0 pos;
    unsigned char state;
    char pad21[3];
    Cls_2C27B0 *cls;
    char pad28[4];
    float scale;
    char pad30;
    unsigned char b31;
    char pad32[0xE];
    V4_2C27B0 rot;
    char pad50[0x28];
    void *data;
    char pad7C[0x2A];
    short cls_id;
    char padA8[0x18];
    V4_2C27B0 mat;
} Moby_2C27B0;

typedef struct Rocket_2C27B0 {
    V4_2C27B0 vel;
    V4_2C27B0 tpos;
    Moby_2C27B0 *owner;
    Moby_2C27B0 *target;
    int timer;
    float speed;
    float yawv;
    float pitchv;
    float maxspeed;
} Rocket_2C27B0;

typedef struct Hit_2C27B0 { char pad0[0x20]; Moby_2C27B0 *owner; } Hit_2C27B0;

typedef struct Veh_2C27B0 { char pad0[0x64]; float f64; char pad68[0x28]; float f90; float f94; } Veh_2C27B0;

typedef struct Hero_2C27B0 { char pad0[0x15F0]; Moby_2C27B0 *veh; } Hero_2C27B0;

typedef struct Tex_2C27B0 { char pad0[0x5C]; unsigned char *p5C; } Tex_2C27B0;

extern Moby_2C27B0 *D_L13_00174158 __attribute__((section(".data")));
extern Tex_2C27B0 D_L13_001B2380;
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *FUN_L00_0026cbb0(void *pos, void *dir, int c, int d, int n, int k, float f);
extern char D_0013F350[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 random_angle_radians_c(void) __asm__("func_00213308");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C;
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int spawn_explosion() __asm__("FUN_L00_0025e450");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern unsigned char *FUN_L00_002712b8(void *pos, float *vec, int s, int a, int col, int n, float x, float y, float z, float w, float pw);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void build_spherical_offset_c(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

void update_level_13_homing_rocket(Moby_2C27B0 *rocket_moby) __asm__("FUN_L13_002c1528");

void update_level_13_homing_rocket(Moby_2C27B0 *m) __asm__("FUN_L13_002c1528");
void update_level_13_homing_rocket(Moby_2C27B0 *rocket_moby) {
    Rocket_2C27B0 *rocket_data = rocket_moby->data;
    V4_2C27B0 old;
    V4_2C27B0 hitpos;
    V4_2C27B0 scratch_vector;
    V4_2C27B0 particle_initial_velocity;
    V4_2C27B0 particle_direction_scratch;
    Hit_2C27B0 *hit;
    float trail_segment_start_fraction;
    float yaw;
    float pitch;

    qcopy(&old, &rocket_moby->pos);
    rocket_moby->scale += (rocket_moby->cls->scale - rocket_moby->scale) * 0.1f;
    hit = (Hit_2C27B0 *)find_moby_hit(rocket_moby, 0xA30000, 0);
    if (hit != 0 && hit->owner != 0 && hit->owner->cls_id != 0x52) {
        func_L00_0025F4A8_alt(rocket_moby, rocket_data, 0, 0.0f, 0.0f, 5, 2, 7, 0.0f, 0.0f, 0.0f, 1.0f, 1, 0.0f, 0, 1, -1, 0);
        mark_moby_for_removal(rocket_moby);
        return;
    }
    *(Q_2C27B0 *)&particle_direction_scratch = 0;
    particle_direction_scratch.x = random_float_between(-1.0f, 1.0f);
    particle_direction_scratch.y = random_float_between(-1.0f, 1.0f);
    particle_direction_scratch.z = random_float_between(-1.0f, 1.0f);
    *(Q_2C27B0 *)&particle_initial_velocity = *(Q_2C27B0 *)&particle_direction_scratch;
    normalize_vector_xyz(&particle_initial_velocity, &particle_initial_velocity, random_float_between(0.1f, 0.2f) * D_0015ED6C);
    normalize_vector_xyz(&scratch_vector, rocket_data, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
    add_vector_xyz(&particle_initial_velocity, &particle_initial_velocity, &scratch_vector);
    FUN_L00_002712b8(&rocket_moby->pos, (float *)&particle_initial_velocity, scale_game_frames_alt(6), 0x7F, 0xB0B0B0, 3, 80000.0f, 1000.0f, 1.0f, -0.0004f, 0.0f);
    for (trail_segment_start_fraction = 0.0f; trail_segment_start_fraction < 1.0f; trail_segment_start_fraction += 0.33333334f) {
        unsigned char *particle;
        normalize_vector_xyz(&scratch_vector, rocket_data, random_float_between(trail_segment_start_fraction, trail_segment_start_fraction + 0.33333334f) * rocket_data->speed);
        add_vector_xyz(&scratch_vector, &scratch_vector, &rocket_moby->pos);
        particle = FUN_L00_002712b8(&scratch_vector, (float *)&particle_initial_velocity, scale_game_frames_alt(0x3C), 0x7F, 0x606060, 3, 80000.0f, 1000.0f, 1.0f, -0.0002f, 0.0f);
        if (particle != 0) {
            Tex_2C27B0 *trail_particle_header_source = &D_L13_001B2380;
            particle[2] = *trail_particle_header_source->p5C;
            particle[3] = 0x44;
        }
        *(Q_2C27B0 *)&particle_direction_scratch = 0;
        particle_direction_scratch.z = 0.02f;
        transform_vector_by_basis(&particle_direction_scratch, &particle_direction_scratch, &rocket_moby->mat);
        build_look_at_matrix(&particle_direction_scratch, &particle_direction_scratch, &rocket_moby->mat, random_angle_radians_c());
        FUN_L00_0026cbb0(&scratch_vector, &particle_direction_scratch, 0x4F007FFF, 0x1FFFFFFF, scale_game_frames_alt(5), 1, 40000.0f);
    }
    if (rocket_data->target != 0 && rocket_data->target->state != 0xFE && rocket_data->target->state != 0xFD
        && distance_xyz(&rocket_data->tpos, &rocket_data->target->pos) < 3.0f) {
        Hero_2C27B0 *hero_state = (Hero_2C27B0 *)(D_0013F350);
        if (hero_state->veh != 0 && hero_state->veh->cls_id == 0x45 && rocket_data->owner->state != 5) {
            Veh_2C27B0 *vd = hero_state->veh->data;
            float vehicle_lead_yaw = fast_add_rotations_c(hero_state->veh->rot.z, vd->f90);
            float vehicle_lead_pitch = fast_add_rotations_c(hero_state->veh->rot.y, vd->f94);
            build_spherical_offset_c(&scratch_vector, distance_xyz(&rocket_moby->pos, &hero_state->veh->pos) / (rocket_data->speed + 24.0f) * vd->f64, vehicle_lead_yaw, -vehicle_lead_pitch);
            add_vector_xyz(&scratch_vector, &scratch_vector, &hero_state->veh->pos);
            yaw = fast_arc_tan(scratch_vector.x - rocket_moby->pos.x, scratch_vector.y - rocket_moby->pos.y);
            pitch = -fast_arc_tan(vector_distance(&rocket_moby->pos, &scratch_vector), scratch_vector.z - rocket_moby->pos.z);
        } else {
            yaw = fast_arc_tan(rocket_data->target->pos.x - rocket_moby->pos.x, rocket_data->target->pos.y - rocket_moby->pos.y);
            pitch = -fast_arc_tan(vector_distance(&rocket_moby->pos, &rocket_data->target->pos), rocket_data->target->pos.z - rocket_moby->pos.z);
        }
        if (fast_difference_between_rotations(yaw, rocket_moby->rot.z) < 1.0471976f) {
            rocket_data->speed += (rocket_data->maxspeed - rocket_data->speed) * 0.05f;
        }
        approach_angle(&rocket_moby->rot.z, &rocket_data->yawv, yaw, frame_time_sq * 12.566371f, frame_time_sq * 12.566371f, D_0015ED6C * 25.132742f);
        approach_angle(&rocket_moby->rot.y, &rocket_data->pitchv, pitch, frame_time_sq * 12.566371f, frame_time_sq * 12.566371f, D_0015ED6C * 25.132742f);
        qcopy(&rocket_data->tpos, &rocket_data->target->pos);
    } else {
        approach_angle(&rocket_moby->rot.z, &rocket_data->yawv, rocket_moby->rot.z, frame_time_sq * 12.566371f, frame_time_sq * 12.566371f, D_0015ED6C * 25.132742f);
        approach_angle(&rocket_moby->rot.y, &rocket_data->pitchv, rocket_moby->rot.y, frame_time_sq * 12.566371f, frame_time_sq * 12.566371f, D_0015ED6C * 25.132742f);
        rocket_data->target = 0;
    }
    if (scale_game_frames_alt(0x78) < rocket_data->timer) {
        build_spherical_offset_c(rocket_data, rocket_data->speed, rocket_moby->rot.z, -rocket_moby->rot.y);
    } else {
        float k = convert_integer_to_float(rocket_data->timer) / convert_integer_to_float(scale_game_frames_alt(0x78));
        if (1.0f < k) {
            k = 1.0f;
        } else if (k < 0.0f) {
            k = 0.0f;
        }
        k = k * k;
        build_spherical_offset_c(&scratch_vector, rocket_data->speed * k, rocket_moby->rot.z, -rocket_moby->rot.y);
        scale_vector_xyz(rocket_data, rocket_data, 1.0f - k);
        add_vector_xyz(rocket_data, rocket_data, &scratch_vector);
    }
    add_vector_xyz(&rocket_moby->pos, &rocket_moby->pos, rocket_data);
    if (rocket_moby->pos.x < 2.0f || 1021.0f < rocket_moby->pos.x || rocket_moby->pos.y < 2.0f || 1021.0f < rocket_moby->pos.y
        || rocket_moby->pos.z < 2.0f || 1021.0f < rocket_moby->pos.z) {
        mark_moby_for_removal(rocket_moby);
        return;
    }
    moby_point_position(rocket_moby, 1, &hitpos);
    if (collision_line(&old, &hitpos, 0, rocket_moby, 0) != 0) {
        Moby_2C27B0 *o = D_L13_00174158;
        if (o == 0 || (o != rocket_data->owner && o->cls_id != 0x504 && o->cls_id != 0x534 && o->cls_id != 0x535
                       && o->cls_id != 0x536 && o->cls_id != 0x537 && o->cls_id != 0x538 && o->cls_id != 0x539
                       && o->cls_id != 0x53A && o->cls_id != 0x52 && o->cls_id != 0x53 && o->cls_id != 0x191
                       && o->cls_id != 0x184 && o->cls_id != 0x185 && o->cls_id != 0x186 && o->cls_id != 0x187
                       && o->cls_id != 0x188 && o->cls_id != 0x189 && o->cls_id != 0x18A && o->cls_id != 0x18B
                       && o->cls_id != 0x18C && o->cls_id != 0x18F && o->cls_id != 0x190)) {
            func_L00_0025F4A8_alt(rocket_moby, rocket_data, 0, 5.0f, 10.0f, 0x14, 5, 0x10, 7.0f, 4.0f, 1.0f, 1.0f, 0, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal(rocket_moby);
            return;
        }
    } else if (coll_sphere(1.5f, &rocket_moby->pos, 0, rocket_moby) != 0) {
        Moby_2C27B0 *o = D_L13_00174158;
        if (o == 0 || (o != rocket_data->owner && o->cls_id != 0x504 && o->cls_id != 0x534 && o->cls_id != 0x535
                       && o->cls_id != 0x536 && o->cls_id != 0x537 && o->cls_id != 0x538 && o->cls_id != 0x539
                       && o->cls_id != 0x53A && o->cls_id != 0x52 && o->cls_id != 0x53 && o->cls_id != 0x191
                       && o->cls_id != 0x184 && o->cls_id != 0x185 && o->cls_id != 0x186 && o->cls_id != 0x187
                       && o->cls_id != 0x188 && o->cls_id != 0x189 && o->cls_id != 0x18A && o->cls_id != 0x18B
                       && o->cls_id != 0x18C && o->cls_id != 0x18F && o->cls_id != 0x190)) {
            func_L00_0025F4A8_alt(rocket_moby, rocket_data, 0, 5.0f, 10.0f, 0x14, 5, 0x10, 7.0f, 4.0f, 1.0f, 1.0f, 0, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal(rocket_moby);
            return;
        }
    }
    if (tick_countdown_32_alt(&rocket_data->timer) != 0 || (rocket_moby->b31 == 0 && rocket_data->owner != 0 && rocket_data->owner->b31 != 0)) {
        mark_moby_for_removal(rocket_moby);
    }
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Spawns a moby of class 0x53 at a position, with its data block filled in. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002C31B0), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00250df8(void *);
extern void copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

unsigned char *spawn_class_53_projectile(char *owner, char *spawn_position, char *initial_velocity) __asm__("FUN_L13_002c1f28");

unsigned char *spawn_class_53_projectile(char *owner, char *spawn_position, char *initial_velocity) {
    unsigned char *moby = (unsigned char *)func_0020D348_m(0x53);
    if (moby != 0) {
        char *data;
        moby[0x30] = 0xFF;
        *(short *)((char *)moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        *(float *)((char *)moby + 0x2C) = *(float *)(*(char **)((char *)moby + 0x24) + 0x24) * 0.1f;
        data = *(char **)((char *)moby + 0x78);
        qcopy((char *)moby + 0x10, spawn_position);
        qcopy(data + 0x60, initial_velocity);
        *(char **)(data + 0x74) = owner;
        data[0x28] = 0;
        *(float *)(data + 0x20) = 20.0f;
        *(short *)(data + 0x24) = 0x14;
        *(float *)(data + 0x30) = 0.5f;
        data[0x29] = 1;
        *(int *)(data + 0x70) = scale_game_frames(0x168);
        if (moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 10);
        }
        FUN_L00_00250df8(moby);
        copy_hero_moby_field_38(moby);
    }
    return moby;
}
extern float D_0015ED60;
extern float D_0015ED6C;
extern char D_00140940[];
extern Moby_2C27B0 *D_L13_00174158;
extern struct { char pad[0x18]; Moby_2C27B0 *moby; } D_L13_00174140;
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");
extern float distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern void blend_moby_animation_c13(void *, int, int, int) __asm__("FUN_00212f90");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void FUN_L00_00259888(float, void *, void *, int, void *);
extern int FUN_L00_001f0d60_2048(void *, float, int, void *) __asm__("FUN_L00_001f0d60");
extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern char *FUN_L00_0025a420(void *, int, int);
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

void update_level_13_class_53_projectile(Moby_2C27B0 *moby) __asm__("FUN_L13_002c2048");

void update_level_13_class_53_projectile(Moby_2C27B0 *moby) {
    char *moby_data = (char *)moby->data;
    V4_2C27B0 steering_and_collision_vector;
    float hitpos[12];
    V4_2C27B0 *pos;
    V4_2C27B0 *velocity;
    Hero_2C27B0 *hero_state;
    Moby_2C27B0 *target;
    Moby_2C27B0 *hit;
    int flags;
    float distance;
    float factor;
    float magnitude;
    short cls_id;

    moby->scale += ((moby->cls->scale + moby->cls->scale) - moby->scale) * (D_0015ED60 * 0.1f);
    if ((moby->pad50[0x20] & 2) != 0 && (unsigned char)moby->pad50[2] != 2 && (unsigned char)moby->pad50[3] != 2) {
        blend_moby_animation_c13(moby, 2, 0, 10);
    }
    pos = &moby->pos;
    velocity = (V4_2C27B0 *)(moby_data + 0x60);
    hero_state = (Hero_2C27B0 *)D_0013F350;
    if (hero_state->veh != 0 &&
        (distance = distance_xyz(pos, &hero_state->veh->pos)) < 60.0f && moby->b31 != 0) {
        factor = 1.0f - distance / 60.0f;
        if (factor > 1.0f) factor = 1.0f;
        magnitude = D_0015ED6C * 1.5f;
        factor *= magnitude;
        subtract_vector_xyz(&steering_and_collision_vector, &hero_state->veh->pos, pos);
        normalize_vector_xyz(&steering_and_collision_vector, &steering_and_collision_vector, factor);
        add_vector_xyz(velocity, velocity, &steering_and_collision_vector);
    }
    if (*(int *)(moby_data + 0x70) < scale_game_frames_alt(0x168) - 9) {
        add_vector_xyz(pos, pos, velocity);
        scale_vector_xyz(velocity, velocity, 0.945f);
    } else {
        subtract_vector_xyz(&steering_and_collision_vector, pos, &(*(Moby_2C27B0 **)(moby_data + 0x74))->pos);
        normalize_vector_xyz(&steering_and_collision_vector, &steering_and_collision_vector, D_0015ED6C * 20.0f);
        add_vector_xyz(pos, pos, velocity);
    }
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f || moby->pos.y > 1021.0f ||
        moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    target = *(Moby_2C27B0 **)(moby_data + 0x74);
    if (target != 0 && target->state != 0xFE && target->state != 0xFD &&
        distance_xyz(pos, &target->pos) > 8.0f) {
        FUN_L00_00259888(16.0f, &hitpos, moby, 0x30000, velocity);
        qcopy(&steering_and_collision_vector, pos);
        steering_and_collision_vector.z += 0.75f;
        flags = collect_mobies_in_sphere(1.7f, &steering_and_collision_vector, 0x10, moby, &hitpos);
        flags = flags | FUN_L00_001f0d60_2048(&steering_and_collision_vector, 1.7f, 4, moby);
        hit = (Moby_2C27B0 *)FUN_L00_0025a420(moby, -1, 0);
        if (hit != 0) {
            float health = *(float *)(moby_data + 0x20) - *(float *)((char *)hit + 0x2C);
            *(float *)(moby_data + 0x20) = health;
            if (health <= 0.0f) {
                flags |= 0x2000;
                D_L13_00174140.moby = 0;
            }
        }
        if (D_L13_00174140.moby != 0) {
            cls_id = D_L13_00174140.moby->cls_id;
            if (cls_id == 0x504 || cls_id == 0x534 || cls_id == 0x535 || cls_id == 0x536 ||
                cls_id == 0x537 || cls_id == 0x538 || cls_id == 0x52 || cls_id == 0x53 ||
                cls_id == 0x539 || cls_id == 0x53A || cls_id == 0x191 || cls_id == 0x184 ||
                cls_id == 0x185 || cls_id == 0x186 || cls_id == 0x187 || cls_id == 0x188 ||
                cls_id == 0x189 || cls_id == 0x18A || cls_id == 0x18B || cls_id == 0x18C ||
                cls_id == 0x18F || cls_id == 0x190) flags = 0;
        }
        if (flags != 0) {
            if (moby->b31 != 0) {
                if ((flags & 0x2000) != 0) {
                    func_L00_0025F4A8_alt(moby, velocity, 0, 0.0f, 0.0f, 5, 2, 7,
                                            0.0f, 0.0f, 0.0f, 1.0f, 3, 0.0f, 1, 1, -1, 0);
                } else {
                    func_L00_0025F4A8_alt(moby, velocity, 0, 4.0f, 16.0f, 0x14, 5, 0x10,
                                            7.0f, 4.0f, 1.0f, 1.0f, 0, 15.0f, 1, 1, -1, 0);
                }
            }
            mark_moby_for_removal(moby);
            return;
        }
    }
    if (tick_countdown_32_alt(moby_data + 0x70) != 0 && moby->b31 == 0) {
        mark_moby_for_removal(moby);
    }
}
void update_hovering_rocket_attacker_common(unsigned char *moby) __asm__("FUN_L13_002c2d30");

extern void update_hovering_rocket_attacker_common(unsigned char *) __asm__("FUN_L13_002c2d30");
extern unsigned char *create_attached_rocket(void *, int) __asm__("FUN_L13_003058a8");
extern void launch_attached_rocket(void *, void *, void *, float) __asm__("FUN_L13_00305988");
extern char *D_L13_001600EC;
#include "rnc/overlay/l13/gameplay/entities/blarg_space_fighter_globals.h"
extern float hovering_rocket_attacker_launch_vector_speed_gp __asm__("D_L13_0016149C") __attribute__((sda));
extern float advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

void update_hovering_rocket_attacker(unsigned char *moby) __asm__("FUN_L13_002c25b8");

void update_hovering_rocket_attacker(unsigned char *moby) {
    enum {
        UPDATE_INITIALIZE = 0x0,
        TRACK_TARGET_BEFORE_ATTACK = 0x2,
        UPDATE_ATTACK_SEQUENCE = 0x4,
        WAIT_TO_MOVE_TO_TABLE_POINT = 0x5,
        MOVE_TO_TABLE_POINT = 0x6,
        WAIT_TO_RESUME_ATTACK_SEQUENCE = 7,
        EMIT_EFFECT_AND_REMOVE = 0x8
    };
    enum {
        START_ATTACK_ANIMATION_SEQUENCE = 0,
        WAIT_FOR_ATTACK_START_ANIMATION = 1,
        WAIT_TO_BEGIN_ALTERNATING_FIRE = 2,
        FIRE_ALTERNATING_SHOTS = 0x3,
        WAIT_TO_ADVANCE_ATTACK_EXIT_SEQUENCE = 4,
        SELECT_ATTACK_RESTART_OR_EXIT = 5,
        WAIT_TO_COMPLETE_ATTACK_EXIT_SEQUENCE = 6
    };
    unsigned char *moby_data = *(unsigned char **)(moby + 0x78);
    unsigned char *target;
    float scratch_vector[4], shot_direction_vector[4], z[4];
    float distance_or_target_yaw;
    float *moby_yaw;
    float *moby_position;
    int index;
    update_hovering_rocket_attacker_common(moby);
    target = *(unsigned char **)(moby_data + 0xB0);
    switch (moby[0x20]) {
    case UPDATE_INITIALIZE:
        qcopy(moby_data + 0xC0, moby + 0x10);
        *(unsigned short *)(moby + 0x34) |= 0x5000;
        *(float *)(moby_data + 0x20) = 4.0f;
        *(short *)(moby_data + 0x24) = 4;
        moby_data[0x28] = 3;
        *(float *)(moby_data + 0xEC) = 15.0f;
        moby_data[0x58] = 50;
        moby_data[0x5A] = 0;
        *(short *)(moby + 0x32) = 255;
        *(float *)(moby + 0x18) += 1.0f;
        if (moby[0x53] != 0) blend_moby_animation_c13(moby, 0, 0, 10);
        moby[0xBC] = 0;
        *(unsigned char **)(moby_data + 0xFC) = create_attached_rocket(moby, 0);
        *(unsigned char **)(moby_data + 0xF8) = create_attached_rocket(moby, 1);
        if (*(int *)(moby_data + 0xD4) != -1) moby[0x20] = 5;
        else moby[0x20] = 2;
        *(float *)(moby_data + 0x100) = random_angle_radians_c();
        *(float *)(moby_data + 0x108) = random_angle_radians_c();
        *(float *)(moby_data + 0x10C) = random_angle_radians_c();
        *(int *)(moby_data + 0x110) = -1;
        break;
    case WAIT_TO_MOVE_TO_TABLE_POINT:
        if (*(int *)(moby_data + 0xB4) == 2) return;
        moby[0xBC] = 0;
        moby[0x20] = 6;
        break;
    case MOVE_TO_TABLE_POINT:
        index = *(int *)(moby_data + 0xD4) * 128;
        moby_position = (float *)(moby + 0x10);
        z[0] = 0.0f;
        distance_or_target_yaw = distance_xyz(moby_position, D_L13_001600EC + index + 0x30);
        subtract_vector_xyz(scratch_vector, D_L13_001600EC + *(int *)(moby_data + 0xD4) * 128 + 0x30, moby_position);
        advance_accelerated_scalar(distance_or_target_yaw, frame_time_sq * 10.0f, frame_time_sq * 10.0f, frame_time * 20.0f, z, (float *)(moby_data + 0xF0));
        normalize_vector_xyz(scratch_vector, scratch_vector, *(float *)(moby_data + 0xF0));
        add_vector_xyz(moby_position, moby_position, scratch_vector);
        if (distance_or_target_yaw < 0.1f && *(float *)(moby_data + 0xF0) < 0.01f) {
            if (moby[0x53] != 0) blend_moby_animation_c13(moby, 0, 0, 10);
            moby[0xBC] = 0;
            moby[0x20] = 2;
        }
        break;
    case TRACK_TARGET_BEFORE_ATTACK:
        moby_yaw = (float *)(moby + 0x48);
        distance_or_target_yaw = fast_arc_tan(*(float *)(target + 0x10) - *(float *)(moby + 0x10), *(float *)(target + 0x14) - *(float *)(moby + 0x14));
        approach_angle(moby_yaw, (float *)(moby_data + 0xDC), distance_or_target_yaw, frame_time_sq * 4.712389f, frame_time_sq * 4.712389f, frame_time * 10.12291f);
        if (*(int *)(moby_data + 0xB4) == 2) return;
        *(float *)(moby_data + 0xEC) = 15.0f;
        moby[0xBC] = 1;
        if (moby[0x53] != 1) blend_moby_animation_c13(moby, 1, 0, 0);
        moby[0x20] = 4;
        break;
    case UPDATE_ATTACK_SEQUENCE:
        moby_yaw = (float *)(moby + 0x48);
        distance_or_target_yaw = fast_arc_tan(*(float *)(target + 0x10) - *(float *)(moby + 0x10), *(float *)(target + 0x14) - *(float *)(moby + 0x14));
        approach_angle(moby_yaw, (float *)(moby_data + 0xDC), distance_or_target_yaw, frame_time_sq * 12.566371f, frame_time_sq * 25.132742f, frame_time * 6.2831855f);
        switch (moby[0xBC]) {
        case WAIT_FOR_ATTACK_START_ANIMATION:
            if (!(moby[0x70] & 2)) return;
            moby[0xBC] = 2;
            if (moby[0x53] != 2) blend_moby_animation_c13(moby, 2, 0, 2);
            *(int *)(moby_data + 0xE8) = 0;
            break;
        case WAIT_TO_BEGIN_ALTERNATING_FIRE:
            if (!(moby[0x70] & 2)) return;
            moby[0xBC] = 3;
            if (moby[0x53] != 3) blend_moby_animation_c13(moby, 3, 0, 2);
            break;
        case FIRE_ALTERNATING_SHOTS:
            if (tick_countdown_32_alt(moby_data + 0xE8)) {
                *(int *)(moby_data + 0xE8) = scale_game_frames_alt(60);
                if (*(int *)(moby_data + 0xF4) != 0) moby_point_position(moby, 1, scratch_vector);
                else moby_point_position(moby, 0, scratch_vector);
                subtract_vector_xyz(shot_direction_vector, target + 0x10, scratch_vector);
                normalize_vector_xyz(shot_direction_vector, shot_direction_vector, hovering_rocket_attacker_launch_vector_speed_gp * frame_time);
                if (*(int *)(moby_data + 0xF4) != 0) {
                    if (*(void **)(moby_data + 0xF8)) {
                        launch_attached_rocket(*(void **)(moby_data + 0xF8), shot_direction_vector, target, *(float *)(moby_data + 0xEC));
                        allocate_voice_for_target_entry(0, 0, (int)moby);
                    }
                    *(unsigned char **)(moby_data + 0xF8) = create_attached_rocket(moby, 1);
                    *(int *)(moby_data + 0xF4) = 0;
                } else {
                    if (*(void **)(moby_data + 0xFC)) {
                        launch_attached_rocket(*(void **)(moby_data + 0xFC), shot_direction_vector, target, *(float *)(moby_data + 0xEC));
                        allocate_voice_for_target_entry(0, 0, (int)moby);
                    }
                    *(unsigned char **)(moby_data + 0xFC) = create_attached_rocket(moby, 0);
                    *(int *)(moby_data + 0xF4) = 1;
                }

                *(float *)(moby_data + 0xEC) += 5.0f;
                if (vector_distance(moby + 0x10, target + 0x10) < *(float *)(moby_data + 0xEC))
                    *(float *)(moby_data + 0xEC) = vector_distance(moby + 0x10, target + 0x10);
            }
            if (*(int *)(moby_data + 0xB4) != 2) return;
            if (moby[0x53] != 4) blend_moby_animation_c13(moby, 4, 0, 2);
            moby[0xBC] = 4;
            break;
        case WAIT_TO_ADVANCE_ATTACK_EXIT_SEQUENCE:
            if (!(moby[0x70] & 2)) return;
            moby[0xBC] = 5;
            if (moby[0x53] != 5) blend_moby_animation_c13(moby, 5, 0, 2);
            break;
        case SELECT_ATTACK_RESTART_OR_EXIT:
            if (*(int *)(moby_data + 0xB4) == 2) {
                if (moby[0x53] != 6) blend_moby_animation_c13(moby, 6, 0, 2);
                moby[0xBC] = 6;
            } else {
                moby[0xBC] = 2;
                if (moby[0x53] != 2) blend_moby_animation_c13(moby, 2, 0, 2);
            }
            break;
        case WAIT_TO_COMPLETE_ATTACK_EXIT_SEQUENCE:
            if (!(moby[0x70] & 2)) return;
            if (*(int *)(moby_data + 0xB4) == 2) {
                if (moby[0x53] != 0) blend_moby_animation_c13(moby, 0, 0, 2);
                moby[0x20] = 2;
                moby[0xBC] = 0;
                break;
            }
        case START_ATTACK_ANIMATION_SEQUENCE:
            moby[0xBC] = 1;
            if (moby[0x53] != 1) blend_moby_animation_c13(moby, 1, 0, 2);
            break;
        }
        break;
    case EMIT_EFFECT_AND_REMOVE:
        clear_u64_value(scratch_vector);
        func_L00_0025F4A8_alt(moby, scratch_vector, moby + 0x10, 0.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 9.0f, 1.0f, 2, 15.0f, 1, 1, -1, 0);
        if (*(void **)(moby_data + 0xF8)) mark_moby_for_removal(*(void **)(moby_data + 0xF8));
        if (*(void **)(moby_data + 0xFC)) mark_moby_for_removal(*(void **)(moby_data + 0xFC));
        mark_moby_for_removal(moby);
        break;
    case WAIT_TO_RESUME_ATTACK_SEQUENCE:
        if (!(moby[0x70] & 2)) return;
        if (moby[0x53] != moby[0xBC]) blend_moby_animation_c13(moby, moby[0xBC], 0, 10);
        moby[0x20] = 4;
        break;
    }
}
extern int FUN_L13_002c2d30_update(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L13_002c2d30_allocate(int, int, void *) __asm__("FUN_L13_00297fa0");
extern void update_moby_vertical_oscillation(float, float, void *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, void *, void *, float, float, float) __asm__("FUN_L00_00262b80");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern int FUN_L00_00257b90(int, int);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern char *FUN_L00_0025a420(void *, int, int);
extern int FUN_L13_002c2d30_collision(struct Moby *, void *, float *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void FUN_L00_00257470(struct Moby *, int, int);
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern int FUN_L00_0025ff38(float, void *, void *, int, int, void *, int);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f99c0(float);
extern int collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern char *D_L13_001B07B0[];
extern int frame_count __asm__("D_L13_0015F5CC");
extern float hovering_rocket_attacker_scale_multiplier_gp __asm__("D_L13_00161490") __attribute__((sda));

void update_hovering_rocket_attacker_common(unsigned char *moby) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_REMOVE = 2,
        HIT_REACTION_LOW_INTERPOLATION_TARGET = 3,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE = 4,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE = 5,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE = 6,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE = 7,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE = 8,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET = 9,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE = 10,
        HIT_REACTION_NO_HIT = 11
    };
    unsigned char *moby_data;
    char *hit_record;
    int hit_reaction_code;
    int flag;
    int target_selection_status;
    float damage_amount;
    float downward_effect_z;
    float fl;
    char point_2_position[16];
    char point_3_position[16];
    char downward_effect_vector[16];
    float z[4];

    moby_data = *(unsigned char **)(moby + 0x78);
    if (moby[0x20] == 0) return;
    if (*(int *)(moby_data + 0x110) == -1 || FUN_L13_002c2d30_update(moby, *(int *)(moby_data + 0x110)) == 0) {
        *(int *)(moby_data + 0x110) = -1;
        *(int *)(moby_data + 0x110) = FUN_L13_002c2d30_allocate(1, 4, moby);
    }
    update_moby_vertical_oscillation(0.5f, frame_time * 1.39626336f, moby, (float *)(moby_data + 0x100), (float *)(moby_data + 0x104));
    update_moby_rotation_oscillation(moby, moby_data + 0x108, moby_data + 0x10C, 0.104719758f, frame_time * 0.314159274f, frame_time * 0.366519153f);
    *(float *)(moby + 0x10) = *(float *)(moby_data + 0xC0) + fast_sin(*(float *)(moby_data + 0x108));
    *(float *)(moby + 0x14) = *(float *)(moby_data + 0xC4) + fast_sin(*(float *)(moby_data + 0x10C));
    *(Q_2C27B0 *)z = 0;
    fl = random_float_between(4.5f, 5.5f);
    downward_effect_z = -(fl * frame_time);
    z[2] = downward_effect_z;
    *(Q_2C27B0 *)downward_effect_vector = *(Q_2C27B0 *)z;
    FUN_L00_0024f7c8(moby, 2, point_2_position);
    FUN_L00_0024f7c8(moby, 3, point_3_position);
    if (frame_count & 1) {
        FUN_L00_00269958(point_2_position, downward_effect_vector, 0xCF0000FF, 0xCF, scale_game_frames(FUN_L00_00257b90(0x12, 0x16)), 0x32, -0xA, 1);
        FUN_L00_00269958(point_3_position, downward_effect_vector, 0xCF0000FF, 0xCF, scale_game_frames(FUN_L00_00257b90(0x12, 0x16)), 0x32, -0xA, 1);
    } else {
        FUN_L00_00269958(point_2_position, downward_effect_vector, 0x6000FFFF, 0x80, scale_game_frames(FUN_L00_00257b90(0x14, 0x18)), 0x64, -0xA, 1);
        FUN_L00_00269958(point_3_position, downward_effect_vector, 0x6000FFFF, 0x80, scale_game_frames(FUN_L00_00257b90(0x14, 0x18)), 0x64, -0xA, 1);
    }
    {
        int ra = FUN_L00_00257b90(0xA, 0x19);
        int rb = FUN_L00_00257b90(0xA, 0xF);
        FUN_L00_00269958(point_2_position, downward_effect_vector, 0xEFFF7F4F, 0xFF0000, scale_game_frames(rb), ra, -ra, 1);
        ra = FUN_L00_00257b90(0xA, 0x19);
        rb = FUN_L00_00257b90(0xA, 0xF);
        FUN_L00_00269958(point_3_position, downward_effect_vector, 0xEFFF7F4F, 0xFF0000, scale_game_frames(rb), ra, -ra, 1);
    }
    *(float *)(moby + 0x2C) = *(float *)(*(char **)(moby + 0x24) + 0x24) * hovering_rocket_attacker_scale_multiplier_gp;
    damage_amount = 0.0f;
    hit_record = FUN_L00_0025a420(moby, 0x330000, 0);
    hit_reaction_code = FUN_L13_002c2d30_collision(moby, hit_record, moby_data + 0x20, 0, &flag, &damage_amount, 0, 4);
    if (hit_record != 0) {
        char *hit_moby = *(char **)(hit_record + 0x20);
        if (hit_moby != 0 && *(short *)(hit_moby + 0xA6) == 0x4D1) {
            flag = 1;
            damage_amount = 0.0f;
        }
    }
    if (flag != 1) {
        *(float *)(moby_data + 0x20) = *(float *)(moby_data + 0x20) - damage_amount;
        if (moby[0x53] != 7) blend_moby_animation_c13(moby, 7, 0, 5);
        if (*(float *)(moby_data + 0x20) <= 0.0f) {
            hit_reaction_code = 1;
        } else {
            moby[0x20] = 7;
        }
        switch (hit_reaction_code) {
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET:
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE:
            moby_data[0x67] = 0xFA;
            break;
        case HIT_REACTION_LOW_INTERPOLATION_TARGET:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE:
            moby_data[0x67] = 0x78;
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_REMOVE:
            FUN_L00_00257470(moby, 0, -1);
            moby[0x20] = 8;
            moby_data[0x67] = 0xF0;
            break;
        case HIT_REACTION_NONE:
        case HIT_REACTION_NO_HIT:
        default:
            break;
        }
        trooper_anim_update(moby, moby_data + 0x60);
    }
    moby[0xA4] = 0xFF;
    trooper_anim_finish(moby, moby_data + 0x60);
    if (moby[0x20] == 4) {
        *(float *)(moby_data + 0xD8) = 50.0f;
    } else {
        *(float *)(moby_data + 0xD8) = 40.0f;
    }
    {
        char *target_selection_point_table = D_L13_001B07B0[*(int *)(moby_data + 0xD0)];
        target_selection_status = FUN_L00_0025ff38(*(float *)(moby_data + 0xD8), moby, moby_data + 0x70, 0, 0, target_selection_point_table + 0x10, *(int *)target_selection_point_table);
    }
    if (target_selection_status != 2) {
        if (*(float *)(moby_data + 0xD8) < vector_distance(moby + 0x10, moby_data + 0x70) ||
            8.0f < FUN_001f99c0(*(float *)(moby + 0x18) - *(float *)(moby_data + 0x78))) {
            *(int *)(moby_data + 0xB4) = 2;
        } else if (moby[0x20] != 5 && collision_line(moby + 0x10, D_0013F350 + 0xD0, 2, moby, 0) != 0) {
            *(int *)(moby_data + 0xB4) = 2;
        }
    }
    if (*(int *)(moby_data + 0xB0) == 0) {
        *(int *)(moby_data + 0xB0) = D_001413D0[0];
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002C4A68), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

void process_blarg_space_fighter_damage(struct Moby *moby, unsigned char *moby_data, float *fp) __asm__("FUN_L13_002c37e0");

void process_blarg_space_fighter_damage(struct Moby *moby, unsigned char *moby_data, float *fp) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_REMOVE = 2,
        HIT_REACTION_LOW_INTERPOLATION_TARGET = 3,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE = 4,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE = 5,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE = 6,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE = 7,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE = 8,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET = 9,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE = 10,
        HIT_REACTION_NO_HIT = 11
    };
    if (moby->state != 3) {
        int hit_processing_status;
        float hit_position_offset[4];
        char *hit_record = find_moby_hit(moby, 0x330000, 0);
        switch (hit_reaction(moby, hit_record, fp, 0, &hit_processing_status, 0, 0, 4)) {
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_REMOVE:
            *fp = 0;
            break;
        case HIT_REACTION_NONE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE:
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET:
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE:
        case HIT_REACTION_NO_HIT:
            break;
        }
        if (hit_processing_status >= 2) {
            if (*fp <= *(float *)(hit_record + 0x2C)) {
                *fp = 0;
                moby->flags &= 0xEFFF;
                moby_data[0x67] = 0x78;
                trooper_anim_update(moby, (short *)(moby_data + 0x60));
                moby->state = 3;
            } else {
                *fp = *fp - *(float *)(hit_record + 0x2C);
                moby_data[0x67] = 0xFA;
                *(short *)(moby_data + 0x26) = scale_game_frames(0x3C);
                trooper_anim_update(moby, (short *)(moby_data + 0x60));
                normalize_vector_xyz(hit_position_offset, hit_record + 0x10, 0.1f);
                add_vector_xyz(&moby->pos, &moby->pos, hit_position_offset);
                random_vector(hit_position_offset, -0.1f, 0.1f);
                add_vector_xyz(&moby->pos, &moby->pos, hit_position_offset);
            }
            moby->hit_slot = 0xFF;
        } else {
            moby->hit_slot = 0xFF;
        }
    }
    trooper_anim_finish(moby, moby_data + 0x60);
}
/* steers a moby's pitch toward a target with a spring, then damps and clamps it */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002C4C10), where it is exact; names translated to the US level program. */

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
float absolute_float(float input) __asm__("func_001F99C0");

void steer_blarg_space_fighter(struct Moby *moby, char *moby_data, float target_yaw, float target_pitch) __asm__("FUN_L13_002c3988");

void steer_blarg_space_fighter(struct Moby *moby, char *moby_data, float target_yaw, float target_pitch) {
    float roll_attenuation_factor;
    float unclamped_roll;
    FUN_L00_00258278(moby, (float *)(moby_data + 0x88), target_yaw, frame_time_sq * 25.132742f,
                     frame_time_sq * 6.2831855f, frame_time * 25.132742f);
    moby->rot.y =
        spring_turn((float *)(moby_data + 0x84), moby->rot.y, target_pitch, frame_time_sq * 25.132742f,
                         frame_time_sq * 6.2831855f, frame_time * 25.132742f);
    roll_attenuation_factor = (1.5707964f - absolute_float(moby->rot.x)) / 1.5707964f;
    unclamped_roll = moby->rot.x + -*(float *)(moby_data + 0x88) * roll_attenuation_factor;
    moby->rot.x = unclamped_roll;
    if (1.5707964f < unclamped_roll)
        moby->rot.x = 1.5707964f;
    if (moby->rot.x < -1.5707964f)
        moby->rot.x = -1.5707964f;
    moby->rot.x = moby->rot.x * 0.98f;
    normalize_angle(moby->rot.x);
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002C4D50), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float FUN_001f9b48(void *, void *);
float absolute_float(float input) __asm__("func_001F99C0");

int select_blarg_space_fighter_path_point_by_distance(char *pt, int *path_point_table, int start, float ref) __asm__("FUN_L13_002c3ac8");

int select_blarg_space_fighter_path_point_by_distance(char *pt, int *path_point_table, int start, float ref) {
    int best = start;
    int i = 0;
    float bestd = 1e11f;
    while (i < *path_point_table / 2) {
        int j = (start + i) % *path_point_table;
        float candidate_distance = FUN_001f9b48(path_point_table + 4 + j * 4, pt);
        float candidate_score = absolute_float(candidate_distance - ref) + convert_integer_to_float(i) * 0.4f;
        if (candidate_score < bestd + convert_integer_to_float(i) * 0.1f) {
            bestd = candidate_distance;
            best = j;
        }
        j = (*path_point_table + start - i) % *path_point_table;
        candidate_distance = FUN_001f9b48(path_point_table + 4 + j * 4, pt);
        candidate_score = absolute_float(candidate_distance - ref) + convert_integer_to_float(i) * 0.4f;
        if (candidate_score < bestd) {
            bestd = candidate_distance;
            best = j;
        }
        i++;
    }
    return best;
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002C4F10), where it is exact; names translated to the US level program. */

extern char *D_L13_001B07B0[];
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void add_vector_xyz(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, float);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
void FUN_L13_002c3988_c(char *moby, char *d, float p2, float p3) __asm__("FUN_L13_002c3988");

int advance_blarg_space_fighter_along_path(struct Moby *moby, char *moby_data, int aim) __asm__("FUN_L13_002c3c88");

int advance_blarg_space_fighter_along_path(struct Moby *moby, char *moby_data, int aim) {
    char *path = D_L13_001B07B0[*(int *)(moby_data + 0xA0)];
    float distance_from_previous_node, seg, segment_distance_shortfall, steering_blend_factor;
    int wrapped = 0;
    float target_node_steering_vector[4];
    distance_from_previous_node = FUN_001f9b48(path + (*(short *)(moby_data + 0xB6) * 16 + 0x10), &moby->pos);
    seg = FUN_001f9b48(path + (*(short *)(moby_data + 0xB6) * 16 + 0x10),
                       path + (*(short *)(moby_data + 0xB4) * 16 + 0x10));
    segment_distance_shortfall = seg - distance_from_previous_node;
    if ((0.0f <= segment_distance_shortfall && segment_distance_shortfall < *(float *)(moby_data + 0xCC)) || seg < *(float *)(moby_data + 0xCC)) {
        *(short *)(moby_data + 0xB6) = *(unsigned short *)(moby_data + 0xB4);
        *(short *)(moby_data + 0xB4) = *(unsigned short *)(moby_data + 0xB4) + ((signed char *)moby_data)[0xD0];
        if (*(short *)(moby_data + 0xB4) >= *(int *)path) {
            *(short *)(moby_data + 0xB4) = 0;
            wrapped = 1;
        }
        if (*(short *)(moby_data + 0xB4) < 0) {
            wrapped = 1;
            *(short *)(moby_data + 0xB4) = *(unsigned short *)path - 1;
        }
        steering_blend_factor = 0.0f;
    } else {
        steering_blend_factor = distance_from_previous_node / seg;
    }
    if (steering_blend_factor < 0.05f)
        steering_blend_factor = 0.05f;
    if (1.0f < steering_blend_factor)
        steering_blend_factor = 1.0f;
    scale_vector_xyz(moby_data + 0x70, moby_data + 0x70, 1.0f - steering_blend_factor);
    subtract_vector_xyz(target_node_steering_vector, path + (*(short *)(moby_data + 0xB4) * 16 + 0x10), &moby->pos);
    normalize_vector_xyz(target_node_steering_vector, target_node_steering_vector, *(float *)(moby_data + 0xCC));
    scale_vector_xyz(target_node_steering_vector, target_node_steering_vector, steering_blend_factor);
    add_vector_xyz(moby_data + 0x70, moby_data + 0x70, target_node_steering_vector);
    normalize_vector_xyz(moby_data + 0x70, moby_data + 0x70, *(float *)(moby_data + 0xCC));
    add_vector_xyz(&moby->pos, &moby->pos, moby_data + 0x70);
    if (aim != 0) {
        struct Hero *hero_state = &hero;
        float yaw = fast_arc_tan(hero_state->motion.pos.f[0] - moby->pos.x,
                                 hero_state->motion.pos.f[1] - moby->pos.y);
        FUN_L13_002c3988_c(moby, moby_data, yaw,
                           -fast_arc_tan(vector_distance(&moby->pos, &hero_state->motion.pos),
                                         hero_state->motion.pos.f[2] - moby->pos.z));
    }
    return wrapped;
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002C5190), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

void predict_hero_vehicle_position(char *out, float scale) __asm__("FUN_L13_002c3f08");

void predict_hero_vehicle_position(char *out, float scale) {
    char *x = D_0013E533 + 0xE1D;
    if (*(char **)(x + 0x15F0) == 0) {
        qcopy(out, x + 0x80);
    } else {
        char *vehicle_data = *(char **)(*(char **)(x + 0x15F0) + 0x78);
        float a =
            fast_add_rotations(*(float *)(*(char **)(x + 0x15F0) + 0x48), *(float *)(vehicle_data + 0x90));
        float b =
            fast_add_rotations(*(float *)(*(char **)(x + 0x15F0) + 0x44), *(float *)(vehicle_data + 0x94));
        build_spherical_offset(out, scale * *(float *)(vehicle_data + 0x64), a, -b);
        fast_vec_add(out, out, *(char **)(x + 0x15F0) + 0x10);
    }
}
extern char *D_L13_0015FFD8;
extern char D_0013F3D0[];
extern float D_0015ED60;
extern float D_0015ED6C;
extern char D_00140940[];
extern int fast_dec_timer_s16(short *) __asm__("FUN_001f9770");
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");

int update_blarg_space_fighter_path_following(struct Moby *moby, char *vars, int aim) __asm__("FUN_L13_002c3fc0");

int update_blarg_space_fighter_path_following(struct Moby *moby, char *vars, int aim) {
    char *path = D_L13_001B07B0[*(int *)(vars + 0xA0)];
    char *linked;
    char *veh;
    int result = 0;
    int point_count;
    int forward;
    int backward;
    int index;
    int step;
    int scans;
    int direct_distance;
    int wrap_distance;
    float forward_distance;
    float backward_distance;
    float target_distance;
    float distance;
    float speed;
    float maximum;
    float point[4] __attribute__((aligned(16)));
    float ahead[4] __attribute__((aligned(16)));
    float behind[4] __attribute__((aligned(16)));

    if (path == 0) {
        goto done;
    }
    veh = *(char **)D_00140940;
    if (veh != 0 && *(short *)(veh + 0xA6) == 0x45) {
        if (*(int *)(vars + 0xBC) != -1 &&
            (linked = D_L13_0015FFD8 + (*(int *)(vars + 0xBC) << 8)) != 0 &&
            (unsigned char)linked[0x20] != 0xFE && (unsigned char)linked[0x20] != 0xFD) {
            subtract_vector_xyz(point, veh + 0x10, linked + 0x10);
            scale_vector_xyz(point, point, 0.5f);
            clamp_vector_length(point, point, 100.0f);
            add_vector_xyz(point, point, linked + 0x10);
        } else {
            predict_hero_vehicle_position((char *)point, 140.0f);
        }
    } else {
        if (*(int *)(vars + 0xBC) == -1 ||
            (linked = D_L13_0015FFD8 + (*(int *)(vars + 0xBC) << 8)) == 0 ||
            (unsigned char)linked[0x20] == 0xFE || (unsigned char)linked[0x20] == 0xFD) {
            qcopy(point, D_0013F3D0);
        } else {
            qcopy(point, linked + 0x10);
        }
    }
    backward = select_blarg_space_fighter_path_point_by_distance((char *)point, (int *)path, *(short *)(vars + 0xB4), 0.0f);
    forward = backward + 5;
    point_count = *(int *)path;
    if (forward >= point_count) {
        forward -= point_count;
    }
    qcopy(ahead, path + 0x10 + forward * 16);
    backward = backward - 5;
    forward_distance = FUN_001f9b48(ahead, D_0013F3D0);
    if (backward < 0) {
        backward += *(int *)path;
    }
    qcopy(behind, path + 0x10 + backward * 16);
    backward_distance = FUN_001f9b48(behind, D_0013F3D0);
    if (backward_distance < forward_distance) {
        backward = forward;
        step = 1;
    } else {
        step = -1;
        if (!(forward_distance < backward_distance) && *(signed char *)(vars + 0xD0) == 1) {
            backward = forward;
            step = 1;
        }
    }
    index = backward;
    scans = 0;
    while (FUN_001f9b48(point, path + 0x10 + index * 16) < 30.0f && scans < *(int *)path) {
        index = (index + step) % *(int *)path;
        scans++;
    }
    backward = *(short *)(vars + 0xB4);
    if (backward < index) {
        direct_distance = index - backward;
        wrap_distance = *(int *)path - index + backward;
        step = direct_distance < wrap_distance ? 1 : -1;
    } else {
        direct_distance = backward - index;
        wrap_distance = *(int *)path - backward + index;
        step = direct_distance < wrap_distance ? -1 : 1;
    }
    target_distance = FUN_001f9b48(&moby->pos, path + 0x10 + index * 16);
    if (step != *(signed char *)(vars + 0xD0)) {
        if (fast_dec_timer_s16((short *)(vars + 0xD2)) != 0) {
            float speed = D_0015ED60_gp * -0.04000002f + 1.0f;
            *(short *)(vars + 0xD2) = 10;
            *(float *)(vars + 0xCC) = *(float *)(vars + 0xCC) * speed;
            distance = vector_distance(&moby->pos, D_0013F3D0);
            if ((*(float *)(vars + 0xCC) < D_0015ED60 * 0.1f) | (moby->visible == 0) | (distance < 40.0f)) {
                short old = *(short *)(vars + 0xB4);
                *(signed char *)(vars + 0xD0) = step;
                *(short *)(vars + 0xB4) = *(short *)(vars + 0xB6);
                *(short *)(vars + 0xB6) = old;
            }
            goto update_path;
        }
    }
    if (step != *(signed char *)(vars + 0xD0)) {
        goto update_path;
    }
    maximum = D_0015ED6C * 100.0f;
    speed = *(float *)(vars + 0xCC);
    speed = speed + (target_distance / 60.0f - speed) * (D_0015ED60 * 0.07f);
    *(float *)(vars + 0xCC) = speed;
    if (maximum < speed) {
        *(float *)(vars + 0xCC) = maximum;
    }
update_path:
    result = advance_blarg_space_fighter_along_path(moby, vars, aim);
done:
    return result;
}
/* Ported from rac1-decomp src/overlays/l13_gemlik/vendor_002C2638.c (func_L13_002C56B0) */
#include "eetypes.h"

/* Update for the blarg space fighter (class 111): joins its path, follows it while its carrier lives, fires at the hero's ship, and explodes. */

typedef struct MobyClass MobyClass;

typedef struct Moby Moby;

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V4_2C56B0;

typedef float Vec_2C56B0[4] __attribute__((aligned(16)));

typedef struct Cls_2C56B0 { char pad0[0x10]; int f10; } Cls_2C56B0;

typedef struct Veh_2C56B0 { char pad0[0x10]; V4_2C56B0 vel; char pad20[0x44]; float f64; } Veh_2C56B0;

typedef struct Boss_2C56B0 { char pad0[0x10E]; unsigned char b10E; unsigned char b10F; } Boss_2C56B0;

typedef struct Moby_2C56B0 {
    char pad0[0x10];
    V4_2C56B0 pos;
    unsigned char state;
    char pad21[3];
    Cls_2C56B0 *cls;
    char pad28[8];
    unsigned char b30;
    unsigned char b31;
    short h32;
    unsigned short flags;
    char pad36[0x42];
    void *data;
    char pad7C[0x18];
    int f94;
    char pad98[0xE];
    short hA6;
    char padA8[0xA];
    short hB2;
    char padB4[8];
    unsigned char bBC;
} Moby_2C56B0;

typedef struct Data_2C56B0 {
    char pad0[0x20];
    float f20;
    short h24;
    char pad26[0x4A];
    V4_2C56B0 vel;
    char pad80[0x10];
    V4_2C56B0 v90;
    int fA0;
    int fA4;
    int fA8;
    float fAC;
    float fB0;
    short hB4;
    short hB6;
    int fB8;
    int fBC;
    char padC0[0x10];
    signed char bD0;
    char padD1[3];
    int fD4;
} Data_2C56B0;

typedef struct Hero_2C56B0 { char pad0[0x15F0]; Moby_2C56B0 *veh; char pad15F4[0xA90]; int f2084; } Hero_2C56B0;

typedef struct Snd_2C56B0 { char pad0[0x74]; unsigned char b74; char pad75[0x13]; void *f88; } Snd_2C56B0;

extern char *D_L13_0015FFD8;
extern char *spawn_class_3f9_moby(int owner, float *dir, float *pos, float size, float len, float z) __asm__("FUN_L11_00308f48");
extern char D_L13_001F4700[];
extern float D_L13_001670C0[];
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_0015ED60;
extern int select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");
extern int update_blarg_space_fighter_path_following();
extern s32 rand(void);
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern unsigned char *spawn_ship_pickup(char *position, int moby_class) __asm__("FUN_L13_002e1c50");
extern void DebugPrint(char *, ...);
extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");
extern void allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
void FUN_L13_002c37e0_c(struct Moby *moby, unsigned char *d, float *fp) __asm__("FUN_L13_002c37e0");
extern char D_0013F3D0[];
extern char D_0013E550[];
extern int func_001E9730_d(void *, ...) __asm__("FUN_001e93b0");
extern int func_0022ED80_s(int, int, int) __asm__("FUN_0022da68");
extern int func_L13_002C5248_56B0(void *, void *, int) __asm__("FUN_L13_002c3fc0");
extern int func_001FA898_s(float) __asm__("FUN_001fa6d0");

void update_blarg_space_fighter(Moby_2C56B0 *moby) __asm__("FUN_L13_002c4428");

void update_blarg_space_fighter(Moby_2C56B0 *moby) {
    enum {
        FIGHTER_INITIALIZE = 0,
        FIGHTER_WAIT_TO_JOIN_PATH = 1,
        FIGHTER_FOLLOW_PATH_AND_ATTACK = 2,
        FIGHTER_EMIT_DESTRUCTION_EFFECTS = 3,
        FIGHTER_WAIT_TO_REINITIALIZE = 4
    };
    enum {
        FIGHTER_ATTACK_WAIT_TO_FIRE = 0,
        FIGHTER_ATTACK_FIRE = 1,
        FIGHTER_ATTACK_COOLDOWN = 2
    };
    enum {
        FIGHTER_DESTRUCTION_SPAWN_CLASS_E0 = 0,
        FIGHTER_DESTRUCTION_SPAWN_CLASS_E4 = 1,
        FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_ALTERNATE = 2,
        FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_SECOND_ALTERNATE = 3,
        FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_THIRD_ALTERNATE = 4,
        FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_FOURTH_ALTERNATE = 5,
        FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_FIFTH_ALTERNATE = 6,
        FIGHTER_DESTRUCTION_SKIP_SPAWN = 7,
        FIGHTER_DESTRUCTION_SKIP_SPAWN_ALTERNATE = 8,
        FIGHTER_DESTRUCTION_SKIP_SPAWN_SECOND_ALTERNATE = 9
    };
    Data_2C56B0 *fighter_data;
    Moby_2C56B0 *veh;
    Veh_2C56B0 *vehd = 0;
    int close = 0;
    char *path;
    float hpos[4];
    float scratch_vector[4];
    float spawn_position[4];
    float vehicle_lead_offset[4];

    fighter_data = moby->data;
    veh = ((Hero_2C56B0 *)(((char *)&D_0013F350)))->veh;
    path = 0;
    if (fighter_data->fA0 != -1) {
        path = D_L13_001B07B0[fighter_data->fA0];
    }
    switch (moby->state) {
    case FIGHTER_INITIALIZE:
        if (moby->b30 == 0 && moby->h32 == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        if (path == 0) {
            func_001E9730_d(D_L13_001F4700, moby->hB2);
            mark_moby_for_removal(moby);
            return;
        }
        {
            int path_table_index = fighter_data->fA0;
            qcopy(&moby->pos, D_L13_001B07B0[path_table_index] + 0x10);
            fighter_data->hB4 = 1;
            fighter_data->hB6 = *(unsigned short *)D_L13_001B07B0[path_table_index] - 1;
        }
        qcopy(&fighter_data->v90, path + 0x20);
        qcopy(&moby->pos, &fighter_data->v90);
        fighter_data->fA8 = scale_game_frames_alt(0x14) + 5;
        moby->b30 = 0xFF;
        moby->h32 = 0xFF;
        moby->state = 1;
        moby->bBC = 0;
        moby->flags = (moby->flags | 1) & 0xEFFF;
        moby->f94 = 0;
        moby->b31 = 0;
        fighter_data->bD0 = 1;
        break;
    case FIGHTER_WAIT_TO_JOIN_PATH: {
        Moby_2C56B0 *p;
        if (fighter_data->fBC == -1 || (p = (Moby_2C56B0 *)(D_L13_0015FFD8 + (fighter_data->fBC << 8))) == 0 || p->state == 0xFE
            || p->state == 0xFD || p->bBC == 4) {
            normalize_vector_xyz(scratch_vector, D_L13_001670C0 + 0x84, 40.0f);
            subtract_vector_xyz_c(scratch_vector, D_L13_001670C0, scratch_vector);
            fighter_data->hB6 = fighter_data->hB4 = select_point_index_by_distance(scratch_vector, path, 0.0f);
            qcopy(&fighter_data->v90, path + (fighter_data->hB6 * 16 + 0x10));
            qcopy(&moby->pos, &fighter_data->v90);
            moby->f94 = moby->cls->f10;
            moby->b31 = 1;
            moby->flags = (moby->flags & 0xFFFE) | 0x1000;
            moby->state = 2;
        }
        break;
    }
    case FIGHTER_FOLLOW_PATH_AND_ATTACK: {
        Moby_2C56B0 *boss;
        Boss_2C56B0 *bossd;
        float fighter_to_vehicle_distance;

        FUN_L13_002c37e0_c((unsigned char *)moby, (unsigned char *)fighter_data, &fighter_data->f20);
        if (veh != 0 && veh->state != 0xFE && veh->state != 0xFD) {
            vehd = veh->data;
            qcopy(hpos, &((Hero_2C56B0 *)(((char *)&D_0013F350)))->veh->pos);
            close = 0;
            fighter_to_vehicle_distance = distance_xyz(&moby->pos, hpos);
            if (((Hero_2C56B0 *)(((char *)&D_0013F350)))->f2084 == 0x32 && (veh->hA6 == 0x45 || veh->hA6 == 0x4DA)) {
                if (fighter_to_vehicle_distance < 110.0f) {
                    close = 1;
                }
            }
        } else {
            qcopy(hpos, ((char *)&D_0013F3D0));
        }
        if (moby->b31 == 0) {
            fighter_data->fA8++;
        } else if (close != 0) {
            fighter_data->fA8 = 0;
        }
        boss = 0;
        bossd = 0;
        if (fighter_data->fBC != -1) {
            Moby_2C56B0 *p = (Moby_2C56B0 *)(D_L13_0015FFD8 + (fighter_data->fBC << 8));
            if (p != 0 && p->state != 0xFE && p->state != 0xFD) {
                boss = p;
                bossd = p->data;
            }
        }
        if (boss == 0 || boss->state == 0xFE || boss->state == 0xFD || boss->state == 1) {
            moby->state = 4;
            moby->f94 = 0;
            moby->flags = (moby->flags | 0x41) & 0xEFFF;
            return;
        }
        {
            Snd_2C56B0 *e;
            if (fighter_data->fD4 == -1 || (e = (Snd_2C56B0 *)(((char *)&D_0013E550) + fighter_data->fD4 * 0x70))->f88 != moby || e->b74 == 0) {
                fighter_data->fD4 = -1;
                fighter_data->fD4 = func_0022ED80_s(1, 4, (int)moby);
            }
        }
        func_L13_002C5248_56B0(moby, fighter_data, 1);
        switch (moby->bBC) {
        case FIGHTER_ATTACK_WAIT_TO_FIRE:
            if (vehd != 0 && veh->state == 4 && moby->b31 != 0 && close != 0 && boss != 0 && bossd->b10F < 3) {
                fighter_data->fB8 = scale_game_frames_alt(0x258);
                fighter_data->fAC = 1.0f;
                fighter_data->fA8 = 0;
                moby->bBC = 1;
            }
            break;
        case FIGHTER_ATTACK_FIRE:
            if (boss != 0) {
                bossd->b10F++;
            }
            if (bossd->b10F < 4) {
                if (moby->b31 == 0) {
                    fighter_data->fA8++;
                }
                if (moby->b31 == 0 || close == 0 || scale_game_frames_alt(0x14) <= fighter_data->fA8) {
                    fighter_data->fB8 = scale_game_frames_alt(0x1E);
                    moby->bBC = 2;
                }
                if (tick_countdown_32_alt(&fighter_data->fB8) != 0) {
                    fighter_data->fB8 = scale_game_frames_alt(0x78);
                    moby->bBC = 2;
                }
                if (tick_countdown_32_alt(&fighter_data->fA4) != 0 && optional_glow_suppression_level < 0.9f && blarg_space_fighter_fire_suppression_level < 0.9f) {
                    fighter_data->fA4 = scale_game_frames_alt(6) + func_001FA898_s((optional_glow_suppression_level + blarg_space_fighter_fire_suppression_level) * 1.3f);
                    if (vehd != 0) {
                        Vec_2C56B0 up;
                        float lateral_aim_offset[4];
                        float lead;

                        *(u128 *)up = 0;
                        up[2] = 1.0f;

                        cross_vectors_xyz(lateral_aim_offset, &vehd->vel, up);
                        cross_vectors_xyz(up, lateral_aim_offset, &vehd->vel);
                        normalize_vector_xyz(up, up, fighter_data->fAC * 5.0f * fast_sin(fighter_data->fB0));
                        normalize_vector_xyz(lateral_aim_offset, lateral_aim_offset, fighter_data->fAC * 5.0f * fast_cos(fighter_data->fB0));
                        add_vector_xyz(scratch_vector, hpos, up);
                        add_vector_xyz(scratch_vector, scratch_vector, lateral_aim_offset);
                        add_random_offset_to_vector_xyz(scratch_vector, 0.6f);
                        lead = vector_distance(&moby->pos, ((char *)&D_0013F3D0));
                        normalize_vector_xyz(vehicle_lead_offset, &vehd->vel, vehd->f64 * (lead / (vehd->f64 + D_0015ED6C * 50.0f)));
                        add_vector_xyz(scratch_vector, scratch_vector, vehicle_lead_offset);
                        subtract_vector_xyz_c(scratch_vector, scratch_vector, &moby->pos);
                        normalize_vector_xyz(spawn_position, scratch_vector, 2.6f);
                        add_vector_xyz(spawn_position, spawn_position, &moby->pos);
                        spawn_class_3f9_moby((int)moby, scratch_vector, spawn_position, 121.0f, D_0015ED6C * 50.0f, 5.0f);
                        func_0022ED80_s(0, 0, (int)moby);
                    }
                    fighter_data->fAC = fighter_data->fAC * ((random_float_between(0.92f, 0.94f) - 1.0f) * D_0015ED60 + 1.0f);
                    if (fighter_data->fB0 > 1.5707964f) {
                        fighter_data->fB0 = fighter_data->fB0 - random_float_between(20.0f, 40.0f) * 0.017453292f * D_0015ED6C;
                    } else {
                        fighter_data->fB0 = fighter_data->fB0 + random_float_between(20.0f, 40.0f) * 0.017453292f * D_0015ED6C;
                    }
                }
            }
            break;
        case FIGHTER_ATTACK_COOLDOWN:
            if (tick_countdown_32_alt(&fighter_data->fB8) != 0) {
                moby->bBC = 0;
            }
            break;
        }
        break;
    }
    case FIGHTER_EMIT_DESTRUCTION_EFFECTS: {
        Moby_2C56B0 *boss;
        Boss_2C56B0 *bossd;

        if (fighter_data->fD4 != -1) {
            Snd_2C56B0 *e = (Snd_2C56B0 *)(((char *)&D_0013E550) + fighter_data->fD4 * 0x70);
            if (e->f88 == moby && e->b74 != 0) {
                release_voice_slot(fighter_data->fD4);
            }
            fighter_data->fD4 = -1;
        }
        subtract_vector_xyz_c(scratch_vector, D_L13_001670C0, &moby->pos);
        func_L00_0025F4A8_alt(moby, scratch_vector, 0, 0.0f, 0.0f, 5, 3, 10, 4.0f, 3.0f, 9.0f, 1.1f, -1, 20.0f, 0, 1, -1, 0);
        normalize_vector_xyz(scratch_vector, scratch_vector, 4.0f);
        subtract_vector_xyz_c(spawn_position, &moby->pos, scratch_vector);
        scale_vector_xyz(scratch_vector, &fighter_data->vel, 0.5f);
        switch (rand() % 10) {
        case FIGHTER_DESTRUCTION_SPAWN_CLASS_E0:
            spawn_ship_pickup((char *)spawn_position, 0xE0);
            break;
        case FIGHTER_DESTRUCTION_SPAWN_CLASS_E4:
        case FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_ALTERNATE:
        case FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_SECOND_ALTERNATE:
        case FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_THIRD_ALTERNATE:
        case FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_FOURTH_ALTERNATE:
        case FIGHTER_DESTRUCTION_SPAWN_CLASS_E4_FIFTH_ALTERNATE:
            spawn_ship_pickup((char *)spawn_position, 0xE4);
            break;
        case FIGHTER_DESTRUCTION_SKIP_SPAWN:
        case FIGHTER_DESTRUCTION_SKIP_SPAWN_ALTERNATE:
        case FIGHTER_DESTRUCTION_SKIP_SPAWN_SECOND_ALTERNATE:
            break;
        }
        boss = 0;
        bossd = 0;
        if (fighter_data->fBC != -1) {
            Moby_2C56B0 *p = (Moby_2C56B0 *)(D_L13_0015FFD8 + (fighter_data->fBC << 8));
            if (p != 0 && p->state != 0xFE && p->state != 0xFD) {
                boss = p;
                bossd = p->data;
            }
        }
        if (boss != 0) {
            if (bossd->b10E != 0) {
                bossd->b10E--;
            }
        }
        moby->state = 4;
        moby->f94 = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
        break;
    }
    case FIGHTER_WAIT_TO_REINITIALIZE: {
        Moby_2C56B0 *p;
        if (fighter_data->fBC != -1 && (p = (Moby_2C56B0 *)(D_L13_0015FFD8 + (fighter_data->fBC << 8))) != 0 && p->state != 0xFE
            && p->state != 0xFD && p->bBC != 4) {
            fighter_data->f20 = fighter_data->h24;
            moby->state = 0;
        }
        break;
    }
    }
    if (moby->pos.x < 20.0f) {
        moby->pos.x = 20.0f;
    }
    if (moby->pos.x > 1003.0f) {
        moby->pos.x = 1003.0f;
    }
    if (moby->pos.y < 20.0f) {
        moby->pos.y = 20.0f;
    }
    if (moby->pos.y > 1003.0f) {
        moby->pos.y = 1003.0f;
    }
    if (moby->pos.z < 20.0f) {
        moby->pos.z = 20.0f;
    }
    if (moby->pos.z > 1003.0f) {
        moby->pos.z = 1003.0f;
    }
}
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern void allocate_voice_for_target_entry_7f38(int, int, struct Moby *) __asm__("FUN_0022da68");

typedef struct Data_2C7F38 {
    float initial_rot[3];
    float progress;
    int linked_index;
    int active_state;
    int inactive_state;
    float angle[3];
    int unknown_28;
    float duration;
    int start_sound;
    int end_sound;
    float initial_scale;
    int start_active;
} Data_2C7F38;

void update_linked_moby_rotation(struct Moby *moby) __asm__("FUN_L13_002c7f38");

void update_linked_moby_rotation(struct Moby *moby) {
    enum {
        INITIALIZE_ROTATION = 0,
        WAIT_TO_ADVANCE_ROTATION = 1,
        ADVANCE_ROTATION = 2,
        HOLD_ROTATION_OFFSET = 3,
        REVERSE_ROTATION = 4
    };
    Data_2C7F38 *data = (Data_2C7F38 *)moby->pvars;
    struct Moby *linked;
    float step;
    float radians;

    if (moby->mission != 0xFF &&
        visited52[moby->mission + current_level_index * 16] == 0xFF) {
        data->linked_index = -1;
    }

    switch (moby->state) {
    case INITIALIZE_ROTATION:
        qcopy(data, &moby->rot);
        moby->state = data->start_active == 0 ? 1 : 2;
        data->progress = 0.0f;
        if (data->progress < data->initial_scale) {
            moby->scale = moby->pclass->scale * data->initial_scale;
        }
        return;
    case WAIT_TO_ADVANCE_ROTATION:
        if (data->linked_index != -1 &&
            (linked = (struct Moby *)(D_L13_0015FFD8 + data->linked_index * 0x100)) != 0 &&
            linked->state != 0xFE && linked->state != 0xFD && linked->state != data->active_state) {
            return;
        }
        if (data->start_sound != -1) {
            allocate_voice_for_target_entry_7f38(data->start_sound, 0, moby);
        }
        data->progress = 0.0f;
        moby->state = 2;
        break;
    case ADVANCE_ROTATION:
        if (data->linked_index != -1 &&
            (linked = (struct Moby *)(D_L13_0015FFD8 + data->linked_index * 0x100)) != 0 &&
            linked->state != 0xFE && linked->state != 0xFD && linked->state == data->inactive_state) {
            if (data->start_sound != -1) {
                allocate_voice_for_target_entry_7f38(data->start_sound, 0, moby);
            }
            moby->state = 4;
            break;
        }
        {
            step = data->duration;
            step *= 60.0f;
            step = 1.0f / step;
            radians = 0.017453292f;
            data->progress += step;
            moby->rot.x = fast_add_rotations(moby->rot.x, data->angle[0] * radians * step);
            moby->rot.y = fast_add_rotations(moby->rot.y, data->angle[1] * radians * step);
            moby->rot.z = fast_add_rotations(moby->rot.z, data->angle[2] * radians * step);
            if (!(1.0f <= data->progress)) return;
            moby->rot.x = fast_add_rotations(data->initial_rot[0], data->angle[0] * radians);
            moby->rot.y = fast_add_rotations(data->initial_rot[1], data->angle[1] * radians);
            moby->rot.z = fast_add_rotations(data->initial_rot[2], data->angle[2] * radians);
            data->progress = 1.0f;
            moby->state = 3;
            if (data->end_sound != -1) {
                allocate_voice_for_target_entry_7f38(data->start_sound, 0, moby);
            }
            return;
        }
        break;
    case HOLD_ROTATION_OFFSET:
        if (data->linked_index == -1) return;
        linked = (struct Moby *)(D_L13_0015FFD8 + data->linked_index * 0x100);
        if (linked == 0 || linked->state == 0xFE || linked->state == 0xFD || linked->state != data->inactive_state) return;
        data->progress = 1.0f;
        if (data->start_sound != -1) {
            allocate_voice_for_target_entry_7f38(data->start_sound, 0, moby);
        }
        moby->state = 4;
        break;
    case REVERSE_ROTATION:
        if (data->linked_index == -1 ||
            (linked = (struct Moby *)(D_L13_0015FFD8 + data->linked_index * 0x100)) == 0 ||
            linked->state == 0xFE || linked->state == 0xFD || linked->state == data->active_state) {
            if (data->start_sound != -1) {
                allocate_voice_for_target_entry_7f38(data->start_sound, 0, moby);
            }
            moby->state = 2;
        } else {
            int invalid_index = -1;
            radians = 0.017453292f;
            step = 1.0f;
            step /= data->duration * 60.0f;
            data->progress -= step;
            moby->rot.x = fast_subtract_rotations(moby->rot.x, data->angle[0] * radians * step);
            moby->rot.y = fast_subtract_rotations(moby->rot.y, data->angle[1] * radians * step);
            moby->rot.z = fast_subtract_rotations(moby->rot.z, data->angle[2] * radians * step);
            if (!(data->progress <= 0.0f)) return;
            moby->rot.x = data->initial_rot[0];
            moby->rot.y = data->initial_rot[1];
            moby->rot.z = data->initial_rot[2];
            data->progress = 0.0f;
            if (data->start_sound != invalid_index) {
                allocate_voice_for_target_entry_7f38(data->end_sound, 0, moby);
            }
            moby->state = 1;
        }
        break;
    case 5:
        break;
    }
}
/* Ported from rac1-decomp src/overlays/l13_gemlik/vendor_002C2638.c (func_L13_002CF960) */

/* Gemlik spark emitter: builds a spark frame from a1, runs two burst loops of effect calls, and with a2 set, the extra burst set. */

typedef int u128z __attribute__((mode(TI)));

typedef struct { unsigned char b[24]; } Blk24;

Blk24 D_L13_001F4950 = {{0xff, 0x8f, 0, 0x4f, 0xff, 0x8f, 0, 0x4f, 0xff, 0x7f, 0, 0x4f, 0xff, 0x6f, 0, 0x4f, 0xff, 0xff, 0xff, 0x2f, 0xff, 0xff, 0xff, 0x2f}};
Blk24 D_L13_001F4968 = {{0x7f, 0x5f, 0, 0x2f, 0x7f, 0x4f, 0, 0x2f, 0x7f, 0x3f, 0, 0x2f, 0x4f, 0, 0, 0x2f, 0, 0, 0, 0x2f, 0, 0, 0, 0x3f}};
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_L13_0015F580;
extern int FUN_L00_00257b90(int, int);
extern int spawn_glow_particle(void *, void *, void *, float, int, int, int, int, int) __asm__("FUN_L00_002ac910");
extern s32 random_integer_below_c(s32) __asm__("func_00213260");
extern unsigned char *spawn_spinning_fade_in_out_effect_particle(void *pos, int spin, int col, float range, float f1, float f2, float f3, float scale) __asm__("FUN_L00_0026b790");
extern void FUN_L00_0026a9f0(void *, void *, int, int, int, int, int, float, int, float);
extern void spawn_crate_debris(void *, void *, void *, void *, int, int, float, float) __asm__("FUN_L00_002af8b0");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");

void emit_forcefield_tower_destruction_effects(unsigned char *source_moby, void *effect_position, int emit_extra_effects) __asm__("FUN_L13_002ce688");

void emit_forcefield_tower_destruction_effects(unsigned char *source_moby, void *effect_position, int emit_extra_effects)
{
    int k;
    int j;
    float vA[4];
    float vB[4];
    float vE[4];
    float vD[4];
    float vC[4];
    Blk24 vG;
    Blk24 vH;
    int extra_emission_count;
    int debris_variant_index;
    int glow_frame_count;
    float emission_reference_distance;
    float emission_distance_shortfall;
    float random_emission_base;
    float yv;
    unsigned char *emission_table_bytes;
    int *pa;
    int *pb;
    int ra;
    int rb;
    int A;
    int B;
    int C;
    int D;
    int E;
    int F;

    char *pA;
    pA = (char *)vA;
    *(u128z *)vA = *(u128z *)effect_position;
    qcopy(vB, pA);
    vB[2] = vB[2] + 0.3f;
    spawn_spinning_fade_in_out_effect_particle(vB, 1, 0x40808080, 0.3f, 1.01f, 1.205f, 0.05f, 150000.0f);
    for (k = 0; k < 5; k++) {
        spawn_spinning_fade_in_out_effect_particle(vB, 1, 0x40808080, 0.3f, 1.01f, 1.1800001f, 0.05f, 150000.0f);
    }
    for (k = 0; k < 4; k++) {
        *(u128z *)vC = 0;
        vC[0] = random_float_between(-1.875f, 1.875f);
        vC[1] = random_float_between(-1.875f, 1.875f);
        vC[2] = random_float_between(0.0f, 3.75f);
        *(u128z *)vD = *(u128z *)vC;
        add_vector_xyz(vD, vD, vB);
        subtract_vector_xyz_c2(vE, vD, pA);
        normalize_vector_xyz(vE, vE, random_float_between(0.025f, 0.23f));
        spawn_crate_debris(source_moby, vE, vD, &D_L13_0015F580, 0x162, 0, 0.90000004f, 2.1f);
    }
    for (k = 0; k < 4; k++) {
        *(u128z *)vC = 0;
        vC[0] = random_float_between(-1.875f, 1.875f);
        vC[1] = random_float_between(-1.875f, 1.875f);
        vC[2] = random_float_between(0.0f, 3.75f);
        *(u128z *)vD = *(u128z *)vC;
        add_vector_xyz(vD, vD, vB);
        subtract_vector_xyz_c2(vE, vD, pA);
        normalize_vector_xyz(vE, vE, random_float_between(0.025f, 0.23f));
        debris_variant_index = random_integer_below_c(2);
        spawn_crate_debris(source_moby, vE, vD, &D_L13_0015F580, debris_variant_index + 0x163, 0, 1.2f, 2.4f);
    }
    if (emit_extra_effects == 0) {
        return;
    }
    subtract_vector_xyz_c2(vD, D_L13_001670C0, pA);
    clear_u64_value(vC);
    emission_reference_distance = vector_length_xyz(vD);
    extra_emission_count = (emission_reference_distance < 8.0f) ? func_001FA898_s(emission_reference_distance) + 2 : 10;
    emission_distance_shortfall = (emission_reference_distance < 7.0f) ? 7.0f - emission_reference_distance : 0.0f;
    if (extra_emission_count > 0) {
        emission_table_bytes = vG.b;
        j = extra_emission_count;
        do {
            random_emission_base = random_float_between(8.0f, 10.0f);
            j--;
            *(Blk24 *)vG.b = *(Blk24 *)&D_L13_001F4950;
            yv = random_emission_base * D_0015ED6C;
            *(Blk24 *)vH.b = *(Blk24 *)&D_L13_001F4968;
            yv = yv - emission_distance_shortfall * D_0015ED6C;
            ra = random_integer_below_c(6);
            pa = (int *)(emission_table_bytes + ra * 4);
            rb = random_integer_below_c(6);
            pb = (int *)(vH.b + rb * 4);
            A = scale_game_frames_alt(15);
            B = scale_game_frames_alt(20);
            C = FUN_L00_00257b90(A, B);
            D = scale_game_frames_alt(25);
            E = scale_game_frames_alt(30);
            F = FUN_L00_00257b90(D, E);
            FUN_L00_0026a9f0(pA, vC, *pa, *pb, C, F, 0, 1200000.0f, 0, yv);
        } while (j != 0);
    }
    if (optional_glow_suppression_level < 0.95f) {
        glow_frame_count = scale_game_frames_alt(15);
        spawn_glow_particle(source_moby, pA, vC, 9.0f, glow_frame_count, 0x7F, 0x7F, 0x7F, 0x20);
        glow_frame_count = scale_game_frames_alt(24);
        spawn_glow_particle(source_moby, pA, vC, 6.0f, glow_frame_count, 0x7F, 0x20, 0, 0x20);
    }
    glow_frame_count = scale_game_frames_alt(20);
    spawn_glow_particle(source_moby, pA, vC, 9.0f, glow_frame_count, 0x7F, 0x40, 0, 0x30);
    glow_frame_count = scale_game_frames_alt(27);
    spawn_glow_particle(source_moby, pA, vC, 6.0f, glow_frame_count, 0x60, 0x10, 0, 0x40);
    glow_frame_count = scale_game_frames_alt(29);
    spawn_glow_particle(source_moby, pA, vC, 3.0f, glow_frame_count, 0x20, 0, 0, 0x20);
}
#include "rnc/overlay/quad.h"
extern int fptodp(float) __asm__("FUN_00120478");
extern int FUN_00213928(struct Moby *, void *, float *, int, int *, int, int, int);
extern void FUN_L00_00257470(struct Moby *, int, int);
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern void emit_effect_l13(void *, void *, void *, float, float, int, int, int, float, float,
                            float, float, int, float, int, int, int,
                            int) __asm__("FUN_L00_0025e450");
extern char D_L13_001f4980[];
extern char D_L13_001f49a8[];
#include "rnc/audio/voice_pool.h"

void process_forcefield_tower_damage(struct Moby *moby, unsigned char *moby_data, float *health) __asm__("FUN_L13_002cecb8");

void process_forcefield_tower_damage(struct Moby *moby, unsigned char *moby_data, float *health) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_REMOVE = 2,
        HIT_REACTION_LOW_INTERPOLATION_TARGET = 3,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE = 4,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE = 5,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE = 6,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE = 7,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE = 8,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET = 9,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE = 10,
        HIT_REACTION_NO_HIT = 11
    };
    char *hit_record;
    int hit;
    float position[4];
    float velocity[4];
    int link;
    VoicePoolWindow *record;

    if (moby->state != 2) {
        hit_record = find_moby_hit(moby, 0x330000, 0);
        if (hit_record != 0) {
            DebugPrint(D_L13_001f4980, moby->oclass, fptodp(*(float *)(hit_record + 0x2c)));
            DebugPrint(D_L13_001f49a8, *(int *)(hit_record + 0x38), fptodp(*(float *)(hit_record + 0)),
                       fptodp(*(float *)(hit_record + 4)), fptodp(*(float *)(hit_record + 8)));
        }
        switch (FUN_00213928(moby, hit_record, health, 0, &hit, 0, 0, 4)) {
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_REMOVE:
            *health = 0.0f;
            break;
        case HIT_REACTION_NONE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE:
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET:
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE:
        case HIT_REACTION_NO_HIT:
            break;
        }
        if (hit >= 2) {
            if (hit_record == 0) {
                goto mark;
            }
            if (*(char **)(hit_record + 0x20) != 0 &&
                moby->pos.z + 8.7f < *(float *)(*(char **)(hit_record + 0x20) + 0x18)) {
                if (*health <= *(float *)(hit_record + 0x2c)) {
                    *(OvlQuad *)velocity = 0;
                    velocity[2] = frame_time * 8.0f;
                    *health = 0.0f;
                    moby->flags &= 0xefff;
                    *(OvlQuad *)position = *(OvlQuad *)velocity;
                    FUN_L00_00257470(moby, 0, -1);
                    moby_data[0x67] = 0x78;
                    trooper_anim_update(moby, (short *)(moby_data + 0x60));
                    moby_point_position(moby, 0, velocity);
                    emit_effect_l13(moby, position, velocity, 0.0f, 0.0f,
                                    10, 3, 16, 4.0f, 2.0f, 0.0f, 1.0f,
                                    1, 20.0f, 1, 1, -1, 0);
                    if (moby->prev_seq != 1) {
                        blend_moby_animation((MobyAnim *)moby, 1, 0, 0);
                    }
                    link = *(int *)(moby_data + 0x70);
                    if (link != -1) {
                        record = (VoicePoolWindow *)((u8 *)&voice_pool + link * 0x70);
                        if ((struct Moby *)record->voice.owner == moby &&
                            record->voice.state != 0) {
                            release_voice_slot(link);
                        }
                    }
                    *(int *)(moby_data + 0x70) = -1;
                    moby->state = 2;
                } else {
                    *health -= *(float *)(hit_record + 0x2c);
                    moby_data[0x67] = 0xfa;
                    *(short *)(moby_data + 0x26) = scale_game_frames(0x3c);
                    trooper_anim_update(moby, (short *)(moby_data + 0x60));
                }
            }
        }
mark:
        moby->hit_slot = 0xff;
    }
    trooper_anim_finish(moby, moby_data + 0x60);
}
extern int FUN_001160d8(void);
extern int start_moby_voice(int, int, void *) __asm__("FUN_L13_00297fa0");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int is_value_within_interpolated_window(void *, float) __asm__("FUN_00214cc8");
extern void spawn_particle(void *, int, int, int, float, int, float) __asm__("FUN_L00_00269b70");
extern char *spawn_scaled_owned_moby(char *, void *, float) __asm__("FUN_L13_0030bfb0");
extern void set_voice_position(int, void *) __asm__("FUN_L13_002982e8");
extern int D_0014C190[];

/* Indexed by data[0x74 / 4]; state 1 clears the entry and removal sets it. */
extern int linked_state_flags[] __asm__("D_L13_001847A8");

void forcefield_tower_update(struct Moby *moby) __asm__("FUN_L13_002cefc0");
void forcefield_tower_update(struct Moby *moby) {
    enum {
        TOWER_STATE_INITIALIZE = 0,
        TOWER_STATE_ACTIVE = 1,
        TOWER_STATE_DESTRUCTION = 2
    };
    enum {
        TOWER_DESTRUCTION_WAIT_FOR_POINT_1_EFFECT = 0,
        TOWER_DESTRUCTION_WAIT_FOR_POINT_2_EFFECT = 1,
        TOWER_DESTRUCTION_WAIT_FOR_POINT_3_EFFECT = 2
    };
    unsigned char *moby_bytes = (unsigned char *)moby;
    int *data = *(int **)(moby_bytes + 0x78);
    float position[4];
    float velocity[4];
    float rotated[4];
    int linked_index;
    int i;
    int red;
    int green;
    int blue;
    float scale;
    float threshold;
    int state = moby_bytes[0x20];

    switch (state) {
    case TOWER_STATE_INITIALIZE: {
        moby_bytes[0xBC] = 0;
        moby_bytes[0x20] = 1;
        if (moby_bytes[0x30] < *(short *)(moby_bytes + 0x32)) moby_bytes[0x30] = moby_bytes[0x32];
        data[0x70 / 4] = -1;
        if ((*(int *)((char *)D_0014C190 + (current_level_index << 8) + (((short)*(unsigned short *)(moby_bytes + 0xB2) >> 5) << 2))
             >> (*(unsigned short *)(moby_bytes + 0xB2) & 31)) & 1) {
            linked_index = data[0x74 / 4];
            if (linked_index != -1) linked_state_flags[linked_index] = 1;
            mark_moby_for_removal(moby);
            return;
        }
        moby_point_position(moby, 0, position);
        position[2] += 1.1f;
        spawn_scaled_owned_moby((char *)moby, position, 2.7f);
        *(unsigned short *)(moby_bytes + 0x34) &= 0xEFFF;
        return;
    }
    case TOWER_STATE_ACTIVE: {
        linked_index = data[0x74 / 4];
        if (linked_index != -1) linked_state_flags[linked_index] = 0;
        if (data[0x70 / 4] == -1) goto allocate_voice;
        if (voice_is_playing(moby, data[0x70 / 4])) goto voice_done;
        if (data[0x70 / 4] != -1) goto play_voice;
allocate_voice:
        data[0x70 / 4] = start_moby_voice(0, 4, moby);
        if (data[0x70 / 4] == -1) goto voice_done;
play_voice:
        qzero(position);
        position[2] += 8.7f;
        set_voice_position(data[0x70 / 4], position);
voice_done:
        if (moby_bytes[0x31] != 0 && random_integer_below(9) == 0) {
            red = (FUN_001160d8() + 0x30) & 0x3F;
            green = (FUN_001160d8() + 0x20) & 0x3F;
            blue = FUN_001160d8() & 0x2F;
            moby_point_position(moby, 0, position);
            position[2] += 0.5f;
            scale = random_float_between(900000.0f, 1800000.0f);
            spawn_particle(position, red, green, blue, scale,
                                      scale_game_frames(FUN_001160d8() % 40 + 40), 0.0f);
        }
        process_forcefield_tower_damage(moby, (unsigned char *)data, (float *)((char *)data + 0x20));
        return;
    }
    case TOWER_STATE_DESTRUCTION: {
        *(u128z *)velocity = 0;
        velocity[2] = D_0015ED6C * 8.0f;
        *(u128z *)position = *(u128z *)velocity;
        threshold = 1000.0f;
        switch ((int)moby_bytes[0xBC]) {
        case TOWER_DESTRUCTION_WAIT_FOR_POINT_1_EFFECT: threshold = 8.5f; break;
        case TOWER_DESTRUCTION_WAIT_FOR_POINT_2_EFFECT: threshold = 17.0f; break;
        case TOWER_DESTRUCTION_WAIT_FOR_POINT_3_EFFECT: threshold = 17.0f; break;
        }
        if (threshold < 100.0f && is_value_within_interpolated_window(moby, threshold)) {
            moby_point_position(moby, moby_bytes[0xBC] + 1, velocity);
            emit_effect_l13(moby, position, velocity, 0.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 0.0f, 1.0f, -1, 20.0f, 1, 1, -1, 0);
            moby_bytes[0xBC]++;
        }
        if ((moby_bytes[0x70] & 2) == 0) return;
        emit_effect_l13(moby, position, 0, 0.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 0.0f, 1.0f, -1, 20.0f, 1, 1, -1, 0);
        moby_point_position(moby, 3, velocity);
        subtract_vector_xyz(velocity, velocity, moby_bytes + 0x10);
        i = 4;
        do {
            i--;
            scale = random_float_between(0.0f, 3.0f);
            scale_vector_xyz(rotated, velocity, scale);
            add_vector_xyz(rotated, rotated, moby_bytes + 0x10);
            emit_forcefield_tower_destruction_effects(moby_bytes, rotated, 1);
        } while (i >= 0);
        linked_index = data[0x74 / 4];
        if (linked_index != -1) linked_state_flags[linked_index] = 1;
        mark_moby_for_removal(moby);
        return;
    }
    default: return;
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002E2488), where it is exact; names translated to the US level program. */

extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern s32 rand();

char *spawn_asteroid_fragment(char *owner, char *spawn_position, char *initial_velocity, int remaining_split_count, float scale) __asm__("FUN_L13_002e1140");

char *spawn_asteroid_fragment(char *owner, char *spawn_position, char *initial_velocity, int remaining_split_count, float scale) {
    char *moby = func_0020D348_m(0x584);
    if (moby != 0) {
        char *moby_data;
        float spin_increment;
        ((unsigned char *)moby)[0x30] = 0xFF;
        moby_data = *(char **)(moby + 0x78);
        *(unsigned short *)(moby + 0x32) = *(unsigned short *)(owner + 0x32);
        moby[0x31] = 1;
        moby[0x20] = 2;
        moby[0xBC] = 0;
        moby[0x72] = 0;
        qcopy(moby + 0x10, spawn_position);
        qcopy(moby_data, initial_velocity);
        *(float *)(moby + 0x2C) = scale;
        *(char **)(moby_data + 0x48) = owner;
        *(int *)(moby_data + 0x40) = 1;
        *(short *)(moby_data + 0x4C) = scale_game_frames(0x1E);
        *(short *)(moby_data + 0x4E) = remaining_split_count;
        *(int *)(moby + 0x94) = 0;
        spin_increment = random_float_between_alt(-30.0f, 30.0f) * DEG_TO_RAD * frame_time;
        switch (rand() % 3) {
        case SPIN_AXIS_X:
            *(float *)(moby_data + 0x30) = spin_increment;
            break;
        case SPIN_AXIS_Y:
            *(float *)(moby_data + 0x34) = spin_increment;
            break;
        case SPIN_AXIS_Z:
            *(float *)(moby_data + 0x38) = spin_increment;
            break;
        }
        spin_increment = random_float_between_alt(-180.0f, 180.0f) * DEG_TO_RAD * frame_time;
        switch (rand() % 3) {
        case SPIN_AXIS_X:
            *(float *)(moby_data + 0x30) = spin_increment;
            break;
        case SPIN_AXIS_Y:
            *(float *)(moby_data + 0x34) = spin_increment;
            break;
        case SPIN_AXIS_Z:
            *(float *)(moby_data + 0x38) = spin_increment;
            break;
        }
        *(float *)(moby + 0x40) = random_angle_radians();
        *(float *)(moby + 0x44) = random_angle_radians();
        *(float *)(moby + 0x48) = random_angle_radians();
        FUN_L00_00250df8(moby);
        copy_hero_moby_field_38(moby);
    }
    return moby;
}

#define NOT_SDA

#define MACRO_ADDR

/* Blob hit: splashes (bigger when the player is near), then splits off one or two smaller blobs while
 * pieces are left, or turns into its pop state / is deleted when none are. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002E2690), where it is exact; names translated to the US level program. */

char *spawn_asteroid_fragment(char *owner, char *pos, char *vec, int a3, float f) __asm__("FUN_L13_002e1140");
extern char D_L13_00160680[] __attribute__((section(".sdata")));
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern int FUN_L00_00257b90(int, int);
extern int spawn_explosion() __asm__("FUN_L00_0025e450");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern int func_0022ED80_s(int, int, int) __asm__("FUN_0022da68");
extern int func_001FA898_s(float) __asm__("FUN_001fa6d0");
extern void func_L00_0025F4A8_s(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

void process_asteroid_hit(struct Moby *moby, char *blob_data, float scale_ratio) __asm__("FUN_L13_002e1348");

void process_asteroid_hit(struct Moby *moby, char *blob_data, float scale_ratio) {
    float split_velocity[4];
    float split_spawn_position[4];
    float nearby_effect_scale = scale_ratio * 7.0f;
    float proximity_gated_effect_scale;
    int i;
    proximity_gated_effect_scale = scale_ratio * 10.0f;
    if (20.0f < distance_xyz(&moby->pos, &hero.motion.pos)) {
        nearby_effect_scale = proximity_gated_effect_scale = 0.0f;
    }
    func_0022ED80_s(0, 0, (int)moby);
    if (*(short *)(blob_data + 0x4C) == 0) {
        func_L00_0025F4A8_s(moby, D_L13_00160680, 0, nearby_effect_scale, proximity_gated_effect_scale, func_001FA898_s(scale_ratio * 10.0f), func_001FA898_s(scale_ratio * 3.0f),
                            func_001FA898_s(scale_ratio * 16.0f), scale_ratio * 4.0f, scale_ratio + scale_ratio, 9.0f, 1.0f, -1, scale_ratio * 15.0f, 0, 1, -1, 0);
    }
    moby->unk72 = 0;
    if (*(short *)(blob_data + 0x4E) > 0) {
        for (i = 0; i < FUN_L00_00257b90(1, 2); i++) {
            float part = moby->scale * random_float_between(0.25f, 0.75f);
            random_vector(split_velocity, frame_time * 5.0f, frame_time * 10.0f);
            scale_vector_xyz(blob_data, blob_data, 0.75f);
            add_vector_xyz(split_velocity, split_velocity, blob_data);
            normalize_vector_xyz(split_spawn_position, split_velocity, 0.5f);
            add_vector_xyz(split_spawn_position, split_spawn_position, &moby->pos);
            add_vector_xyz(split_spawn_position, split_spawn_position, split_velocity);
            spawn_asteroid_fragment(moby, (char *)split_spawn_position, (char *)split_velocity, *(short *)(blob_data + 0x4E) - 1, part);
            moby->scale -= part;
            if (--*(short *)(blob_data + 0x4E) == 0) break;
            if (moby->scale / moby->pclass->scale <= 0.2f) break;
        }
        moby->collision = 0;
        *(short *)(blob_data + 0x4C) = scale_game_frames(0x1E);
    } else if (*(int *)(blob_data + 0x40) == 0) {
        moby->state = 4;
        moby->flags |= 1;
        moby->collision = 0;
        moby->visible = 0;
        moby->scale = *(float *)(blob_data + 0x2C);
    } else {
        mark_moby_for_removal(moby);
    }
}
#include "rnc/overlay/collision.h"

/* Vars of a Gemlik blob (class 0x584, FUN_L13_002e1140). */
struct BlobVars {
    Vec4f vel;                     /* 0x00: moved by it each frame once launched */
    Vec4f home;                    /* 0x10: respawn point; w: 20 */
    f32 dir[3];                    /* 0x20: launch direction (rot_row0 at spawn) */
    f32 start_scale;               /* 0x2C */
    Vec4f spin;                    /* 0x30: x/y/z rotation per frame */
    s32 child;                     /* 0x40: 1 when split off a bigger blob */
    s32 timer;                     /* 0x44: frames left before it respawns */
    struct Moby *owner;            /* 0x48: blob it split off */
    s16 cooldown;                  /* 0x4C: frames before it can hit again */
    s16 pieces;                    /* 0x4E: blobs left to split off */
};

extern f32 distance_xyz_1638(void *, void *) __asm__("FUN_001f9b48");
extern s32 random_int_between_1638(s32, s32) __asm__("FUN_L00_00257b90");
extern f32 random_float_between_1638(f32, f32) __asm__("FUN_002132a8");
extern f32 random_angle_1638(void) __asm__("FUN_00213308");
extern s32 rand_1638(void) __asm__("FUN_001160d8");
extern void normalize_vector_xyz_1638(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_1638(void *, void *, f32) __asm__("FUN_001f9c90");
extern void add_vector_xyz_1638(void *, void *, void *) __asm__("FUN_001f9a10");
extern void reflect_vector_1638(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern s32 tick_countdown_16_1638(s16 *) __asm__("FUN_001f9770");
extern s32 tick_countdown_32_1638(s32 *) __asm__("FUN_001f9740");
extern s32 scale_game_frames_1638(s32) __asm__("FUN_001f96f8");
extern f32 fast_add_rotations_1638(f32, f32) __asm__("FUN_001fa580");
extern s32 FUN_001fa728_1638(f32, void *) __asm__("FUN_001fa728");
extern s32 FUN_L00_001f0d60_1638(f32, void *, s32, struct Moby *) __asm__("FUN_L00_001f0d60");
extern char *find_moby_hit_1638(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void mark_moby_for_removal_1638(struct Moby *) __asm__("FUN_0020c828");
extern void FUN_L13_002e1348_1638(struct Moby *, struct BlobVars *, f32) __asm__("FUN_L13_002e1348");
extern struct Hero hero_1638 __asm__("D_0013F350");
extern Vec4f hero_pos_1638 __asm__("D_0013F3D0");
extern CollisionHit coll_hit_1638 __asm__("D_L13_00174140");
extern s32 frame_count __asm__("D_L13_0015F5CC");
extern char *moby_base_1638 __asm__("D_L13_0015FFD8");

/* Blob update: waits on its home point, jumps out at the hero when the ship comes near, splits when it hits something, and respawns. */
void asteroid_update(struct Moby *moby) __asm__("FUN_L13_002e1638");
void asteroid_update(struct Moby *moby) {
    enum {
        ASTEROID_INITIALIZE = 0,
        ASTEROID_WAIT_TO_LAUNCH = 1,
        ASTEROID_MOVE_AND_COLLIDE = 2,
        ASTEROID_RESET_AT_HOME = 3,
        ASTEROID_WAIT_TO_RESPAWN = 4
    };
    struct BlobVars *vars = (struct BlobVars *)moby->pvars;
    f32 distance = distance_xyz_1638(&moby->pos, &hero_pos_1638);
    f32 size;
    f32 spin;
    char *hit;

    switch (moby->state) {
    case ASTEROID_INITIALIZE:
        qcopy(&vars->home, &moby->pos);
        vars->home.w = 20.0f;
        qcopy(vars->dir, &moby->rot_row0);
        vars->start_scale = moby->scale;
        vars->pieces = random_int_between_1638(2, 3);
        if (moby->scale < moby->pclass->scale) {
            moby->unk72 = 0;
        }
        moby->state = 3;
        break;
    case ASTEROID_WAIT_TO_LAUNCH:
        if (hero_1638.state.current == 0x32 && hero_1638.ship_moby != 0 && hero_1638.ship_moby->oclass == 0x45
            && (moby->visible != 0 || distance < 40.0f)) {
            normalize_vector_xyz_1638(vars, vars->dir, random_float_between_1638(1.0f, 10.0f) * frame_time);
            moby->state = 2;
        }
        break;
    case ASTEROID_MOVE_AND_COLLIDE:
        tick_countdown_16_1638(&vars->cooldown);
        add_vector_xyz_1638(&moby->pos, &moby->pos, vars);
        if (moby->visible != 0 || distance < 40.0f) {
            if (vars->child == 0) {
                vars->timer = scale_game_frames_1638(0x168);
            } else {
                vars->timer = scale_game_frames_1638(0x78);
            }
            if (vars->cooldown == 0) {
                moby->collision = moby->pclass->collision;
                size = moby->scale / moby->pclass->scale;
                if ((frame_count & 1) == (((char *)moby - moby_base_1638) >> 8 & 1)
                    && FUN_L00_001f0d60_1638(size * 6.2f, &moby->pos, 0, moby) != 0) {
                    if ((struct Moby *)coll_hit_1638.moby != vars->owner) {
                        reflect_vector_1638(vars, vars, &coll_hit_1638.normal_x);
                        qcopy(&moby->pos, coll_hit_1638.pad30);
                        if (coll_hit_1638.moby == 0 || ((struct Moby *)coll_hit_1638.moby)->oclass != 0xD4) {
                            FUN_L13_002e1348_1638(moby, vars, size);
                        }
                    }
                } else {
                    hit = find_moby_hit_1638(moby, 0x330000, 0);
                    if (hit != 0 && (*(struct Moby **)(hit + 0x20))->oclass != 0xD4) {
                        hit += 0x10;
                        size = moby->scale;
                        size /= moby->pclass->scale;
                        scale_vector_xyz_1638(hit, hit, frame_time * 20.0f);
                        add_vector_xyz_1638(vars, vars, hit);
                        FUN_L13_002e1348_1638(moby, vars, size);
                    }
                    moby->hit_slot = 0xFF;
                }
            }
        } else if (tick_countdown_32_1638(&vars->timer) != 0) {
            if (vars->child == 1) {
                mark_moby_for_removal_1638(moby);
                return;
            }
            moby->state = 4;
            moby->collision = 0;
            moby->flags |= 1;
            moby->visible = 0;
            moby->scale = vars->start_scale;
        }
        break;
    case ASTEROID_RESET_AT_HOME:
        qcopy(&moby->pos, &vars->home);
        qzero(&vars->spin);
        spin = random_float_between_1638(-30.0f, 30.0f) * DEG_TO_RAD * frame_time;
        switch (rand_1638() % 3) {
        case SPIN_AXIS_X:
            vars->spin.x = spin;
            break;
        case SPIN_AXIS_Y:
            vars->spin.y = spin;
            break;
        case SPIN_AXIS_Z:
            vars->spin.z = spin;
            break;
        }
        spin = random_float_between_1638(-180.0f, 180.0f) * DEG_TO_RAD * frame_time;
        switch (rand_1638() % 3) {
        case SPIN_AXIS_X:
            vars->spin.x = spin;
            break;
        case SPIN_AXIS_Y:
            vars->spin.y = spin;
            break;
        case SPIN_AXIS_Z:
            vars->spin.z = spin;
            break;
        }
        moby->rot.x = random_angle_1638();
        moby->rot.y = random_angle_1638();
        moby->rot.z = random_angle_1638();
        qzero(&vars->vel);
        moby->scale = vars->start_scale;
        if (moby->pclass->scale < moby->scale) {
            moby->unk72 = 100;
        }
        moby->state = 1;
        break;
    case ASTEROID_WAIT_TO_RESPAWN:
        if (FUN_001fa728_1638(moby->draw_dist, &vars->home) < 0) {
            qcopy(&moby->pos, &vars->home);
            moby->collision = moby->pclass->collision;
            moby->visible = 1;
            moby->flags &= ~1;
            moby->state = 3;
        }
        break;
    }
    if (moby->pos.x < 20.0f) {
        moby->pos.x = 20.0f;
    }
    if (moby->pos.x > 1003.0f) {
        moby->pos.x = 1003.0f;
    }
    if (moby->pos.y < 20.0f) {
        moby->pos.y = 20.0f;
    }
    if (moby->pos.y > 1003.0f) {
        moby->pos.y = 1003.0f;
    }
    if (moby->pos.z < 20.0f) {
        moby->pos.z = 20.0f;
    }
    if (moby->pos.z > 1003.0f) {
        moby->pos.z = 1003.0f;
    }
    moby->rot.x = fast_add_rotations_1638(moby->rot.x, vars->spin.x);
    moby->rot.y = fast_add_rotations_1638(moby->rot.y, vars->spin.y);
    moby->rot.z = fast_add_rotations_1638(moby->rot.z, vars->spin.z);
}
/* runs one step of a moby's timed state and reports whether the timer ran out */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002E56C8), where it is exact; names translated to the US level program. */

int process_linked_switch_damage(struct Moby *moby, char *moby_data, float *health) __asm__("FUN_L13_002e4300");

int process_linked_switch_damage(struct Moby *moby, char *moby_data, float *health) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_REMOVE = 2,
        HIT_REACTION_LOW_INTERPOLATION_TARGET = 3,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE = 4,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE = 5,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE = 6,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE = 7,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE = 8,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET = 9,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE = 10,
        HIT_REACTION_NO_HIT = 11
    };
    char *hit_record = find_moby_hit(moby, 0x330000, 0);
    int hit_processing_status;
    int result = 0;
    char *color_animation_state;
    switch (hit_reaction(moby, hit_record, health, 0, &hit_processing_status, 0, 0, 4)) {
    case HIT_REACTION_NONE:
        break;
    case HIT_REACTION_LETHAL:
        *(int *)health = 0;
        break;
    case HIT_REACTION_REMOVE:
        *(int *)health = 0;
        break;
    case HIT_REACTION_LOW_INTERPOLATION_TARGET:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE:
    case HIT_REACTION_HIGH_INTERPOLATION_TARGET:
    case HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE:
    case HIT_REACTION_NO_HIT:
        break;
    }
    color_animation_state = moby_data + 0x60;
    if (hit_processing_status >= 2) {
        float damage_amount = *(float *)(hit_record + 0x2C);
        unsigned short fl;
        if (*health <= damage_amount) {
            *(int *)health = 0;
            fl = moby->flags;
            moby->flags = fl & 0xEFFF;
            *(unsigned char *)(moby_data + 0x67) = 0x78;
            trooper_anim_update(moby, (short *)color_animation_state);
            result = 1;
        } else {
            *health = *health - damage_amount;
            *(unsigned char *)(moby_data + 0x67) = 0xFA;
            *(short *)(moby_data + 0x26) = scale_game_frames(0x3C);
            trooper_anim_update(moby, (short *)color_animation_state);
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, color_animation_state);
    return result;
}
extern void get_first_moby_in_group(char **, int, int, int) __asm__("FUN_L00_002591d0");
extern void get_next_moby_in_group(char **, char *, int, int) __asm__("FUN_L00_002592b8");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern int fast_dec_timer_s16(short *) __asm__("FUN_001f9770");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int FUN_0022da68(int, int, void *) __asm__("FUN_0022da68");
extern void set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern void debug_print(char *, short, short, int) __asm__("FUN_001e93b0");
char D_L13_001F4D90[] = "moby:%d(%d) opened at %d\n";
char D_L13_001F4DB0[] = "moby:%d(%d) failed at %d\n";
extern char D_L13_001F4DD0[];
extern int frame_count __asm__("D_L13_0015F5CC");

typedef struct {
    u8 pad0[0x20];
    float unk20;
    short unk24;
    u8 pad26[0x4A];
    short unk70;
    short unk72;
    short unk74;
    short unk76;
    u8 unk78;
    u8 unk79;
    u8 unk7A;
    u8 unk7B;
    int unk7C;
    char *unk80;
    short unk84;
} SwitchVars;
#define MIN(a, b) ((a) < (b) ? (a) : (b))
void update_linked_switch_group(char *moby) __asm__("FUN_L13_002e4448");

void update_linked_switch_group(char *moby) {
    enum {
        INITIALIZE_LINKED_GROUP = 0,
        MONITOR_LINKED_GROUP_HITS = 1,
        WAIT_FOR_LINKED_GROUP_COMPLETION = 2,
        RESET_HIT_STATE = 3,
        UPDATE_OPENED_STATE = 4
    };
    SwitchVars *data = *(SwitchVars **)(moby + 0x78);
    int hit = 0;

    if (data->unk74 == 0 && (unsigned char)(moby[0x20] - 1) < 2) {
        hit = process_linked_switch_damage((struct Moby *)moby, data, &data->unk20);
    }
    switch ((unsigned char)moby[0x20]) {
    case INITIALIZE_LINKED_GROUP:
        if (data->unk79 == 0) {
            char *iter[4];
            char *linked;

            data->unk7B = 0;
            data->unk80 = moby;
            get_first_moby_in_group(iter, (unsigned char)moby[0x21], 0, 0);
            while ((linked = *iter) != 0) {
                linked = iter[0];
                if (linked != moby && *(short *)(linked + 0xA6) == 0xE7) {
                    SwitchVars *other_data = *(SwitchVars **)(linked + 0x78);

                    other_data->unk80 = moby;
                    other_data->unk79 = 1;
                }
                data->unk7B++;
                get_next_moby_in_group(iter, iter[0], 0, 0);
            }
        }
        data->unk70 = 0;
        data->unk72 = 0;
        data->unk7A = 0;
        data->unk74 = 0;
        data->unk76 = 0;

        set_moby_color(moby, 0x80, 0x80, 0x80);
        moby[0x20] = 1;
        break;
    case MONITOR_LINKED_GROUP_HITS:
    case WAIT_FOR_LINKED_GROUP_COMPLETION:
        if (data->unk74 == 0 && hit != 0) {
            char *linked;
            SwitchVars *linked_data;
            linked = data->unk80;
            linked_data = *(SwitchVars **)(linked + 0x78);
            if ((unsigned char)linked[0x20] == 1) {
                data->unk7A = 0;
                linked[0x20] = 2;
                if (linked_data->unk7C > 0) {
                    linked_data->unk70 = scale_game_frames(data->unk7C);
                }
            }
            linked_data->unk7A++;
            FUN_0022da68(3, 0, moby);
            set_moby_color(moby, 0, 0xFF, 0);
            data->unk74 = 1;
        }
        if ((unsigned char)moby[0x20] == 2) {
            if (data->unk7A >= data->unk7B) {
                set_moby_list_state((unsigned char)moby[0x21], 4);
                FUN_0022da68(1, 0, moby);
                data->unk70 = data->unk84;
                debug_print(D_L13_001F4D90, *(short *)(moby + 0xB2), *(short *)(moby + 0xA6), frame_count);
                moby[0x20] = 4;
            } else {
                if (data->unk7C >= 0) {
                    if (fast_dec_timer_s16(&data->unk70) != 0) {

                        data->unk7A = 0;
                        data->unk74 = 0;

                        FUN_0022da68(2, 0, moby);
                        set_moby_list_state((unsigned char)moby[0x21], 3);
                        debug_print(D_L13_001F4DB0, *(short *)(moby + 0xB2), *(short *)(moby + 0xA6), frame_count);
                        moby[0x20] = 3;
                    } else if (fast_dec_timer_s16(&data->unk72) != 0) {
                        FUN_0022da68(0, 0, moby);
                        data->unk72 = MIN(scale_game_frames(0x2D), scale_game_frames(5) + data->unk70 / scale_game_frames(0x28));
                    }
                }
            }
        }
        break;
    case RESET_HIT_STATE:
        set_moby_color(moby, 0x80, 0x80, 0x80);

        data->unk7A = 0;
        data->unk74 = 0;
        data->unk20 = (float)data->unk24;
        moby[0x20] = 1;

        break;
    case UPDATE_OPENED_STATE:
        if (data->unk79 == 0 && data->unk70 != -1) {
            if (fast_dec_timer_s16(&data->unk70) != 0) {

                data->unk7A = 0;
                data->unk74 = 0;

                FUN_0022da68(2, 0, moby);
                set_moby_list_state((unsigned char)moby[0x21], 3);
                debug_print(D_L13_001F4DD0, *(short *)(moby + 0xB2), *(short *)(moby + 0xA6), frame_count);
                moby[0x20] = 3;
            } else if (fast_dec_timer_s16(&data->unk72) != 0) {
                FUN_0022da68(0, 0, moby);
                data->unk72 = MIN(scale_game_frames(0x2D), scale_game_frames(5) + data->unk70 / scale_game_frames(0x28));
            }
        }
        if (fast_dec_timer_s16(&data->unk76) != 0) {
            data->unk76 = scale_game_frames(0x14);
            if (data->unk78 != 0) {
                set_moby_color(moby, 0, 0xFF, 0);
                data->unk78 = 0;
            } else {
                set_moby_color(moby, 0, 0, 0xFF);
                data->unk78 = 1;
            }
        }
        break;
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
extern void *create_moby() __asm__("FUN_0020c4f8");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002E9870), where it is exact; names translated to the US level program. */

extern void FUN_L00_00250df8(void *);
extern void copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");

char *spawn_owner_linked_class_160_moby(char *owner) __asm__("FUN_L13_002e8438");

char *spawn_owner_linked_class_160_moby(char *owner) {
    char *moby = create_moby(0x160);
    if (moby != 0) {
        char *data;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0x3FF;
        moby[0x31] = 1;
        data = *(char **)(moby + 0x78);
        moby[0x20] = 0;
        moby[0xBC] = 0;
        qcopy(moby + 0x10, owner + 0x10);
        *(char **)data = owner;
        *(int *)(data + 4) = 0;
        *(unsigned short *)(moby + 0x34) |= 0x200;
        FUN_L00_00250df8(moby);
        copy_hero_moby_field_38(moby);
        *(float *)(moby + 0x2C) = *(float *)(*(char **)(moby + 0x24) + 0x24);
    }
    return moby;
}
extern void offset_moby_class_point_pairs(unsigned char *, int, int) __asm__("FUN_L00_0024efb0");

/* Fades the moby in (state 1), holds until its owner is gone or dying (state 2), fades it out (state 3) and removes it (state 4), easing its scale toward the class scale and spinning it all the while. */
void update_owner_linked_class_160_moby(struct Moby *moby) __asm__("FUN_L13_002e84d8");

void update_owner_linked_class_160_moby(struct Moby *moby) {
    enum {
        INITIALIZE_COLLISION = 0,
        FADE_IN = 1,
        WAIT_FOR_OWNER_REMOVAL = 2,
        FADE_OUT = 3,
        REMOVE_MOBY = 4
    };
    int *moby_data = (int *)moby->pvars;
    int old;
    int step;

    switch (moby->state) {
    case INITIALIZE_COLLISION:
        moby->collision = moby->pclass->collision;
        moby->state = 1;
        break;
    case FADE_IN: {
        float fade = frame_scale * 0.02f;
        moby->scale +=
            (moby->pclass->scale * 4.0f - moby->scale) * 0.1f;
        ((float *)moby_data)[1] += fade;
        if (((float *)moby_data)[1] >= 1.0f) {
            ((float *)moby_data)[1] = 1.0f;
            moby->state = 2;
        }
        moby->alpha = truncate_float_to_s32(((float *)moby_data)[1] * 60.0f);
        break;
    }
    case WAIT_FOR_OWNER_REMOVAL:
        moby->scale +=
            (moby->pclass->scale * 4.0f - moby->scale) * 0.1f;
        if (moby_data[0] == 0 || ((unsigned char *)moby_data[0])[0x20] == 0xFE || ((unsigned char *)moby_data[0])[0x20] == 0xFD) {
            moby->state = 3;
        }
        break;
    case FADE_OUT: {
        float fade = frame_scale * 0.02f;
        moby->scale +=
            (moby->pclass->scale - moby->scale) * 0.1f;
        ((float *)moby_data)[1] -= fade;
        if (((float *)moby_data)[1] <= 0.0f) {
            ((float *)moby_data)[1] = 0.0f;
            moby->state = 4;
        }
        moby->alpha = truncate_float_to_s32(((float *)moby_data)[1] * 60.0f);
        break;
    }
    case REMOVE_MOBY:
        mark_moby_for_removal(moby);
        return;
    }
    old = moby_data[2];
    step = -0x40;
    moby_data[2] = old + step;
    if (moby_data[2] > 0x1000) {
        step = -0x1040;
        moby_data[2] = old + step;
    } else if (moby_data[2] < 0) {
        step = 0xFC0;
        moby_data[2] = old + step;
    }
    offset_moby_class_point_pairs(moby->pclass, 0, step);
}
/* Picks a line to play from a group, starting at a random entry and skipping ones already used; returns 1 when one was queued. */
extern int rand_q(void) __asm__("FUN_001160d8");
extern int below_q(int) __asm__("FUN_00213260");

extern unsigned char D_L13_001D3558[];
int queue_starfighter_voice_line(void *unused, unsigned char *moby_data, int base, int line_count) __asm__("FUN_L13_002e86f8");

int queue_starfighter_voice_line(void *unused, unsigned char *moby_data, int base, int line_count) {
    int j = rand_q() % line_count;
    int id = D_L13_001D3558[base + j];
    int i;
    int lt;
    if (base < 0x1D) {
        if (base < 0xC) {
            if (moby_data[0x148] != 0)
                return 0;
            if (*(short *)(moby_data + 0x144) > 0)
                return 0;
        } else {
            if (*(short *)(moby_data + 0x146) > 0)
                return 0;
            if (moby_data[0x148] != 0 && base >= 0x17 && base < 0x1B)
                return 0;
        }
    }
    lt = base < 0x1D;
    if (music_stream_state.secondary.handle != 0 || music_stream_state.queued_secondary_track != -1) {
        if (lt)
            return 0;
    }
    if (base < 0x1D) {
        for (i = 0; i < line_count; i++) {
            if ((*(int *)(moby_data + 0x13C) >> id) & 1) {
                if (base < 0xC && below_q(4) == 0)
                    *(int *)(moby_data + 0x13C) &= ~(1 << id);
                j++;
                j %= line_count;
                id = D_L13_001D3558[base + j];
            } else {
                music_stream_state.queued_secondary_track = id + 50000;
                moby_data[0x13A] = 1;
                *(int *)(moby_data + 0x13C) |= 1 << id;
                return 1;
            }
        }
        if (base < 0xC)
            *(int *)(moby_data + 0x13C) &= ~(1 << id);
    } else {
        music_stream_state.queued_secondary_track = id + 50000;
        moby_data[0x13A] = 1;
        return 1;
    }
    return 0;
}
/* Throws sparks from each of the moby's six vent bones that are open. */
extern int D_L13_001D3478[] __asm__("D_L13_001D3478");
extern int frame_count __asm__("D_L13_0015F5CC");
extern int rnd_v(int, int) __asm__("FUN_L00_00257b90");
extern int below_v(int) __asm__("FUN_00213260");
extern int frames_v(int) __asm__("FUN_001f96f8");
extern int ftoi_v(float) __asm__("FUN_001fa6d0");
extern float rndf_v(float, float) __asm__("FUN_002132a8");
extern void bone_v(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);

typedef struct {
    char pad[0xC4];
    int on[1];
} Vd_v;

extern int D_L13_001D3508[];
extern int D_L13_001D3520[];
void emit_gated_moby_joint_effects(struct Moby *source_moby, char *emission_gate_data) __asm__("FUN_L13_002e8920");

void emit_gated_moby_joint_effects(struct Moby *source_moby, char *emission_gate_data) {
    OvlQuad zv;
    float bone_position[4];
    int i;
    for (i = 0; i < 6; i++) {
        if (((Vd_v *)emission_gate_data)->on[D_L13_001D3508[i]] != 0) {
            float scale_ratio;
            int effect_frame_count, k;
            zv = 0;
            scale_ratio = source_moby->scale / (source_moby->pclass->scale * 4.0f);
            bone_v(source_moby, D_L13_001D3520[i], bone_position);
            if (frame_count & 1) {
                effect_frame_count = frames_v(rnd_v(0xF, 0x14));
                FUN_L00_00269958(bone_position, &zv, 0xCF0000FF, 0xCF, effect_frame_count, ftoi_v(scale_ratio * 350.0f), -100, 1);
            } else if (source_moby->command == 0 || D_L13_001D3478[source_moby->command - 1] != D_L13_001D3508[i] ||
                       below_v(9) == 0) {
                effect_frame_count = frames_v(rnd_v(0xF, 0x16));
                FUN_L00_00269958(bone_position, &zv, 0x6000FFFF, 0x80, effect_frame_count, ftoi_v(scale_ratio * 600.0f), -100, 1);
            }
            k = ftoi_v(rndf_v(100.0f, 250.0f) * scale_ratio);
            effect_frame_count = frames_v(rnd_v(8, 0xC));
            FUN_L00_00269958(bone_position, &zv, 0xEFFF7F4F, 0xFF0000, effect_frame_count, k, -k, 1);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002E9F90), where it is exact; names translated to the US level program. */

extern int frame_count __asm__("D_L13_0015F5CC");
extern int D_L13_001D3538[];
extern int FUN_L00_00257b90(int, int);
extern int random_integer_below(int) __asm__("FUN_00213260");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void emit_randomized_moby_joint_effects(struct Moby *moby) __asm__("FUN_L13_002e8b58");

void emit_randomized_moby_joint_effects(struct Moby *moby) {
    OvlQuad zv;
    float effect_position[4];
    int i;
    for (i = 0; D_L13_001D3538[i] != -1; i++) {
        float randomized_scale_ratio;
        int effect_frame_count, k;
        OvlQuad *z = &zv;
        zv = 0;
        randomized_scale_ratio = moby->scale / (moby->pclass->scale * 4.0f);
        randomized_scale_ratio = randomized_scale_ratio * random_float_between_alt(0.8f, 1.1f);
        moby_point_position(moby, D_L13_001D3538[i], effect_position);
        add_random_offset_to_vector_xyz(effect_position, randomized_scale_ratio * 1.4f);
        if (frame_count & 1) {
            effect_frame_count = scale_game_frames(FUN_L00_00257b90(0x14, 0x3C));
            k = func_001FA898_r(random_float_between_alt(450.0f, 550.0f) * randomized_scale_ratio);
            FUN_L00_00269958(effect_position, z, 0xDF000FFF, 0xCF, effect_frame_count, k, -100, 1);
        } else {
            if (moby->command != 0) {
                if (random_integer_below(0xC) != 0)
                    continue;
            }
            effect_frame_count = scale_game_frames(FUN_L00_00257b90(0x14, 0x32));
            k = func_001FA898_r(random_float_between_alt(600.0f, 800.0f) * randomized_scale_ratio);
            FUN_L00_00269958(effect_position, z, 0x8000FFFF, 0x80, effect_frame_count, k, -100, 1);
        }
    }
}
/* Places the attached pieces on their bones, keeping each position inside a box, and copies the moby's pose to them. */
extern int D_L13_001D34A8[];
extern char *spawn_moby_joint_attachment(char *, int, int) __asm__("FUN_L13_002ecd10");
extern void FUN_0020c828(char *);

void update_moby_joint_attachments(struct Moby *parent_moby, char *attachment_data) __asm__("FUN_L13_002e8d28");

void update_moby_joint_attachments(struct Moby *parent_moby, char *attachment_data) {
    int i;
    int attachment_joint_index;
    char *attached_moby;
    char *pose_follower_moby;
    for (i = 0; i < 10; i++) {
        attached_moby = ((char **)(attachment_data + 0xC4))[i];
        if (attached_moby != 0) {
            moby_point_position(parent_moby, i, attached_moby + 0x10);
            if (*(float *)(attached_moby + 0x10) < 22.0f)
                *(float *)(attached_moby + 0x10) = 22.0f;
            if (*(float *)(attached_moby + 0x10) > 1001.0f)
                *(float *)(attached_moby + 0x10) = 1001.0f;
            if (*(float *)(attached_moby + 0x14) < 22.0f)
                *(float *)(attached_moby + 0x14) = 22.0f;
            if (*(float *)(attached_moby + 0x14) > 1001.0f)
                *(float *)(attached_moby + 0x14) = 1001.0f;
            if (*(float *)(attached_moby + 0x18) < 22.0f)
                *(float *)(attached_moby + 0x18) = 22.0f;
            if (*(float *)(attached_moby + 0x18) > 1001.0f)
                *(float *)(attached_moby + 0x18) = 1001.0f;
            qcopy(attached_moby + 0x40, &parent_moby->rot);
            *(OvlQuad *)(attached_moby + 0xC0) = *(OvlQuad *)&parent_moby->rot_row0;
            *(OvlQuad *)(attached_moby + 0xD0) = *(OvlQuad *)&parent_moby->rot_row1;
            *(OvlQuad *)(attached_moby + 0xE0) = *(OvlQuad *)&parent_moby->rot_row2;
        }
    }
    attachment_joint_index = D_L13_001D34A8[parent_moby->command];
    if (parent_moby->state == 5)
        attachment_joint_index = -1;
    if (attachment_joint_index != -1) {
        if (*(char **)(attachment_data + 0xF0) == 0 || *(unsigned char *)(*(char **)(attachment_data + 0xF0) + 0x20) == 0xFE ||
            *(unsigned char *)(*(char **)(attachment_data + 0xF0) + 0x20) == 0xFD)
            *(char **)(attachment_data + 0xF0) = spawn_moby_joint_attachment(parent_moby, attachment_joint_index, 0x191);
        attached_moby = *(char **)(attachment_data + 0xF0);
        if (attached_moby != 0 && *(unsigned char *)(attached_moby + 0x20) != 0xFE && *(unsigned char *)(attached_moby + 0x20) != 0xFD) {
            moby_point_position(parent_moby, attachment_joint_index, attached_moby + 0x10);
            if (*(float *)(attached_moby + 0x10) < 22.0f)
                *(float *)(attached_moby + 0x10) = 22.0f;
            if (*(float *)(attached_moby + 0x10) > 1001.0f)
                *(float *)(attached_moby + 0x10) = 1001.0f;
            if (*(float *)(attached_moby + 0x14) < 22.0f)
                *(float *)(attached_moby + 0x14) = 22.0f;
            if (*(float *)(attached_moby + 0x14) > 1001.0f)
                *(float *)(attached_moby + 0x14) = 1001.0f;
            if (*(float *)(attached_moby + 0x18) < 22.0f)
                *(float *)(attached_moby + 0x18) = 22.0f;
            if (*(float *)(attached_moby + 0x18) > 1001.0f)
                *(float *)(attached_moby + 0x18) = 1001.0f;
            qcopy(attached_moby + 0x40, &parent_moby->rot);
            *(OvlQuad *)(attached_moby + 0xC0) = *(OvlQuad *)&parent_moby->rot_row0;
            *(OvlQuad *)(attached_moby + 0xD0) = *(OvlQuad *)&parent_moby->rot_row1;
            *(OvlQuad *)(attached_moby + 0xE0) = *(OvlQuad *)&parent_moby->rot_row2;
        }
    } else {
        if (*(char **)(attachment_data + 0xF0) != 0 && *(unsigned char *)(*(char **)(attachment_data + 0xF0) + 0x20) != 0xFE &&
            *(unsigned char *)(*(char **)(attachment_data + 0xF0) + 0x20) != 0xFD) {
            FUN_0020c828(*(char **)(attachment_data + 0xF0));
            *(char **)(attachment_data + 0xF0) = 0;
        }
    }
    pose_follower_moby = *(char **)(attachment_data + 0x110);
    if (pose_follower_moby != 0) {
        qcopy(pose_follower_moby + 0x10, &parent_moby->pos);
        qcopy(pose_follower_moby + 0x40, &parent_moby->rot);
        *(OvlQuad *)(pose_follower_moby + 0xC0) = *(OvlQuad *)&parent_moby->rot_row0;
        *(OvlQuad *)(pose_follower_moby + 0xD0) = *(OvlQuad *)&parent_moby->rot_row1;
        *(OvlQuad *)(pose_follower_moby + 0xE0) = *(OvlQuad *)&parent_moby->rot_row2;
    }
}
/* springs a moby's pitch and yaw toward targets and clamps the pitch */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002EA450), where it is exact; names translated to the US level program. */

float absolute_float(float input) __asm__("func_001F99C0");

void update_moby_spring_steering_and_banking(struct Moby *moby, char *moby_data, float target_yaw, float target_pitch) __asm__("FUN_L13_002e9018");

void update_moby_spring_steering_and_banking(struct Moby *moby, char *moby_data, float target_yaw, float target_pitch) {
    float absolute_roll;
    FUN_L00_00258278(moby, (float *)(moby_data + 0xA8), target_yaw, frame_time_sq * 3.1415927f, frame_time_sq * 6.2831855f,
                     frame_time * 6.2831855f);
    moby->rot.y =
        spring_turn((float *)(moby_data + 0xA4), moby->rot.y, target_pitch, frame_time_sq * 3.1415927f,
                         frame_time_sq * 6.2831855f, frame_time * 6.2831855f);
    absolute_roll = absolute_float(moby->rot.x);
    moby->rot.x += -*(float *)(moby_data + 0xA8) * ((1.5707964f - absolute_roll) / 1.5707964f) * 0.5f;
    if (moby->rot.x > 1.5707964f)
        moby->rot.x = 1.5707964f;
    if (moby->rot.x < -1.5707964f)
        moby->rot.x = -1.5707964f;
    moby->rot.x *= 0.985f;
    normalize_angle(moby->rot.x);
}
/* Level overlay code after unclassified; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002C4F10), where it is exact; names translated to the US level program. */

extern char *D_L13_001B07B0[];
extern float FUN_001f9b48(void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L13_002e9018_c(void *, void *, float, float) __asm__("FUN_L13_002e9018");

int advance_moby_along_selected_path(struct Moby *moby, char *moby_data, int path_slot_index, int aim) __asm__("FUN_L13_002e9160");

int advance_moby_along_selected_path(struct Moby *moby, char *moby_data, int path_slot_index, int aim) {
    char *path = D_L13_001B07B0[*(int *)(&moby_data[path_slot_index << 2] + 0xB0)];
    float distance_from_previous_node, seg, segment_distance_shortfall, steering_blend_factor, yaw, pitch;
    int wrapped = 0;
    float target_node_steering_vector[4];
    distance_from_previous_node = FUN_001f9b48(path + (*(short *)(moby_data + 0x9E) * 16 + 0x10), &moby->pos);
    seg = FUN_001f9b48(path + (*(short *)(moby_data + 0x9E) * 16 + 0x10),
                       path + (*(short *)(moby_data + 0x9C) * 16 + 0x10));
    segment_distance_shortfall = seg - distance_from_previous_node;
    if (0.0f <= segment_distance_shortfall && segment_distance_shortfall < *(float *)(moby_data + 0xAC)) {
        *(short *)(moby_data + 0x9E) = *(unsigned short *)(moby_data + 0x9C);
        *(short *)(moby_data + 0x9C) = *(unsigned short *)(moby_data + 0x9C) + ((signed char *)moby_data)[0x10C];
        if (*(short *)(moby_data + 0x9C) >= *(int *)path) {
            *(short *)(moby_data + 0x9C) = 0;
            wrapped = 1;
        }
        if (*(short *)(moby_data + 0x9C) < 0) {
            wrapped = 1;
            *(short *)(moby_data + 0x9C) = *(unsigned short *)path - 1;
        }
        steering_blend_factor = 0.0f;
    } else {
        steering_blend_factor = distance_from_previous_node / seg;
    }
    if (steering_blend_factor < 0.05f)
        steering_blend_factor = 0.05f;
    if (1.0f < steering_blend_factor)
        steering_blend_factor = 1.0f;
    fast_vec_scale(moby_data + 0x70, moby_data + 0x70, 1.0f - steering_blend_factor);
    fast_vec_sub(target_node_steering_vector, path + (*(short *)(moby_data + 0x9C) * 16 + 0x10), &moby->pos);
    fast_vec_normalize(target_node_steering_vector, target_node_steering_vector, *(float *)(moby_data + 0xAC));
    fast_vec_scale(target_node_steering_vector, target_node_steering_vector, steering_blend_factor);
    fast_vec_add(moby_data + 0x70, moby_data + 0x70, target_node_steering_vector);
    fast_vec_normalize(moby_data + 0x70, moby_data + 0x70, *(float *)(moby_data + 0xAC));
    fast_vec_add(&moby->pos, &moby->pos, moby_data + 0x70);
    yaw = fast_arc_tan(target_node_steering_vector[0], target_node_steering_vector[1]);
    pitch = -fast_arc_tan(vector_length_xy(target_node_steering_vector), target_node_steering_vector[2]);
    if (moby->state == 5) {
        struct Hero *hero_state = &hero;
        yaw = fast_arc_tan(hero_state->motion.pos.f[0] - moby->pos.x,
                           hero_state->motion.pos.f[1] - moby->pos.y);
        pitch = -fast_arc_tan(vector_distance(&moby->pos, &hero_state->motion.pos),
                              hero_state->motion.pos.f[2] - moby->pos.z);
    }
    if (aim != 0)
        FUN_L13_002e9018_c(moby, moby_data, yaw, pitch);
    return wrapped;
}
/* Picks the racer's direction along its path from where the hero is, eases its speed, then steps it; bit 1 of the result is set past a marked node. */
extern char *D_L13_001B07B0_9408[] __asm__("D_L13_001B07B0");
extern float D_0015ED60_9408 __asm__("D_0015ED60");
extern float dist_9408(void *, void *) __asm__("FUN_001f9b48");
extern int FUN_L13_002e9160_c(char *, char *, int, int) __asm__("FUN_L13_002e9160");

int update_moby_path_following_away_from_hero(struct Moby *moby, char *path_following_data, int path_slot_index, int aim) __asm__("FUN_L13_002e9408");

int update_moby_path_following_away_from_hero(struct Moby *moby, char *path_following_data, int path_slot_index, int aim) {
    char *path = D_L13_001B07B0_9408[*(int *)(&path_following_data[path_slot_index << 2] + 0xB0)];
    int result_flags = 0;
    float ahead[4];
    float behind[4];
    float ahead_distance_to_hero, behind_distance_to_hero;
    int next_node_index;
    int k;
    int desired_path_direction;
    if (path != 0) {
        next_node_index = *(short *)(path_following_data + 0x9C) + 1;
        if (next_node_index >= *(int *)path) {
            next_node_index = 0;
        }
        qcopy(ahead, path + next_node_index * 16 + 0x10);
        ahead_distance_to_hero = dist_9408(ahead, &hero.motion.pos);
        k = *(short *)(path_following_data + 0x9C) - 1;
        if (k < 0) {
            k = *(int *)path - 1;
        }
        qcopy(behind, path + k * 16 + 0x10);
        behind_distance_to_hero = dist_9408(behind, &hero.motion.pos);
        if (ahead_distance_to_hero < behind_distance_to_hero) {
            desired_path_direction = -1;
        } else if (behind_distance_to_hero < ahead_distance_to_hero) {
            desired_path_direction = 1;
        } else {
            desired_path_direction = path_following_data[0x10C] == 1 ? 1 : -1;
        }
        if (desired_path_direction != (signed char)path_following_data[0x10C]) {
            float k = D_0015ED60_9408;
            *(float *)(path_following_data + 0xAC) *= k * -0.100000024f + 1.0f;
            if (*(float *)(path_following_data + 0xAC) < k * 0.15f) {
                short cur = *(short *)(path_following_data + 0x9C);
                path_following_data[0x10C] = desired_path_direction;
                *(short *)(path_following_data + 0x9C) = *(unsigned short *)(path_following_data + 0x9E);
                *(short *)(path_following_data + 0x9E) = cur;
            }
        } else {
            float distance_to_hero = dist_9408(&moby->pos, &hero.motion.pos);
            float target_speed_multiplier = 1.0f;
            float sp;
            if (moby->command == 4) {
                target_speed_multiplier = 3.0f;
            }
            sp = *(float *)(path_following_data + 0xAC);
            sp += (target_speed_multiplier * 30.0f / distance_to_hero - sp) * (D_0015ED60_9408 * 0.12f);
            *(float *)(path_following_data + 0xAC) = sp;
            if (sp < 0.0f) {
                *(float *)(path_following_data + 0xAC) = 0.0f;
            }
        }
        result_flags = FUN_L13_002e9160_c(moby, path_following_data, path_slot_index, aim);
    }
    if (0.0f < *(float *)(path + (*(short *)(path_following_data + 0x9C) << 4) + 0x1C)) {
        result_flags |= 2;
    }
    return result_flags;
}
/* Launches the child moby held in slot idx of p: spins it off with random-ish velocities and clears the slot. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002EAAB8), where it is exact; names translated to the US level program. */

extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");

void detach_moby_joint_attachment(void *unused, char *parent_data, int child_slot_index) __asm__("FUN_L13_002e9680");

void detach_moby_joint_attachment(void *unused, char *parent_data, int child_slot_index) {
    char *base = parent_data + 0xC4;
    int child_slot_byte_offset = child_slot_index * 4;
    char **slot = (char **)(base + child_slot_byte_offset);
    char *moby = *slot;
    if (moby != 0) {
        char *data = *(char **)(moby + 0x78);
        float *child_launch_vector = (float *)(data + 0x10);
        float random_half_range, random_small_angle_step, random_second_angle_step, random_third_angle_step;
        cart_burst(unused, moby + 0x10, -1, 2.0f, 13.0f);
        fast_vec_scale(child_launch_vector, parent_data + 0x70, 0.95f);
        add_random_offset_to_vector_xyz(child_launch_vector, frame_time * 30.0f);
        moby[0x20] = 3;
        fast_vec_add(moby + 0x10, moby + 0x10, child_launch_vector);
        random_half_range = frame_time;
        random_half_range = random_half_range * 0.34906585f;
        random_small_angle_step = random_float_between_alt(-random_half_range, random_half_range);
        random_half_range = frame_time;
        *(float *)(data + 0x30) = random_small_angle_step;
        random_half_range = random_half_range * 3.1415927f;
        random_second_angle_step = random_float_between_alt(-random_half_range, random_half_range);
        random_half_range = frame_time;
        *(float *)(data + 0x34) = random_second_angle_step;
        random_half_range = random_half_range * 3.1415927f;
        random_third_angle_step = random_float_between_alt(-random_half_range, random_half_range);
        *(float *)(data + 0x38) = random_third_angle_step;
        *(long *)(moby + 0x38) = 0x40404000000000L;
        *(unsigned char *)(moby + 0xA4) = 0xFF;
        *slot = 0;
    }
}
extern void FUN_L00_00257470(struct Moby *, int, int);
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void activate_descriptor(int) __asm__("FUN_L10_002f5a50");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void set_animation_parameter(int, int) __asm__("FUN_001ff570");
extern char *D_L13_001B07B0[];
extern float D_0015ED6C;

extern char *D_L13_001600EC;

void begin_starfighter_death_sequence(struct Moby *moby, char *vars, int *result) __asm__("FUN_L13_002e97e0");

void begin_starfighter_death_sequence(struct Moby *moby, char *vars, int *result) {
    int path_ids = (int)vars + 0xB0;
    struct Moby *mobys;
    float dx;
    float dy;
    float dz;
    float horizontal;
    float scale;
    char *velocity;

    *result = 0;
    moby->flags &= 0xEFFF;
    FUN_L00_00257470(moby, 0, -1);
    vars[0x67] = 0x78;
    trooper_anim_update(moby, (short *)(vars + 0x60));
    vars[0x13B] = 4;
    scale = D_0015ED6C * 10.0f;
    velocity = D_L13_001600EC + (*(int *)(vars + 0x130) << 7);
    *(short *)(vars + 0x9C) = 0;
    fast_vec_normalize(vars + 0x70, velocity, scale);
    {
        int index = *(short *)(vars + 0x9C);
        int *path_id = (int *)path_ids + (unsigned char)vars[0x13B];
        char *path = D_L13_001B07B0[*path_id];
        char *point;
        char *next;

        qcopy(&moby->pos, (char *)((unsigned int)(index * 16 + 0x10) + (unsigned int)path));
        point = (char *)((int)path + index * 16);
        next = path + ++index * 16;
        dx = *(float *)(next + 0x10) - *(float *)(point + 0x10);
        dy = *(float *)(next + 0x14) - *(float *)(point + 0x14);
        moby->rot.z = fast_arc_tan(dx, dy);
    }
    {
        int index = *(short *)(vars + 0x9C);
        int *path_id = (int *)path_ids + (unsigned char)vars[0x13B];
        char *path = D_L13_001B07B0[*path_id];
        int point = index * 16 + 0x10;
        int next = index * 16 + 0x20;

        horizontal = vector_distance(path + point, path + next);
    }
    {
        int index = *(short *)(vars + 0x9C);
        char *path;
        char *point;
        char *next;

        path_ids += (unsigned char)vars[0x13B] * 4;
        path = D_L13_001B07B0[*(int *)path_ids];
        next = path + (index + 1) * 16;
        point = path + index * 16;
        dz = *(float *)(next + 0x18) - *(float *)(point + 0x18);
        moby->rot.y = -fast_arc_tan(horizontal, dz);
    }
    vars[0x10C] = 1;
    mobys = (struct Moby *)D_L13_0015FFD8;
    mobys[*(int *)(vars + 0x134)].flags |= 2;
    mobys[*(int *)(vars + 0x134)].flags |= 1;
    mobys[*(int *)(vars + 0x134)].visible = 0;
    activate_descriptor(*(int *)(vars + 0x12C));
    moby->state = 7;
    *(float *)(vars + 0xAC) = D_0015ED6C * 80.0f;
    *(int *)(vars + 0xF8) = scale_game_frames(600);
    queue_starfighter_voice_line(moby, (unsigned char *)vars, 0x22, 1);
    *(short *)(vars + 0x138) = 0;
    vars[0x148] = 0;
    if (*(int *)(vars + 0x104) != -1) {
        set_animation_parameter(*(int *)(vars + 0x104), 0);
        *(int *)(vars + 0x104) = -1;
    }
    *(int *)(vars + 0x13C) = -1;
}
/* Per-frame update of a breakable vehicle part (level 13): probes the first attached object and then the
   eleven attached children for hits, takes damage from them, advances the damage stage (unkBC) with its
   effects and sounds, and runs the stage's follow-up action. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002EAE80), where it is exact; names translated to the US level program. */

typedef struct Moby_2EAE80 Moby_2EAE80;

typedef struct {
    char pad0[0x88];
    int f88;
    int f8C;
    char pad90[0x5C];
    int fEC;
    char padF0[0x28];
    int f118;
} Vars_2EAE80;

struct Moby_2EAE80 {
    char pad0[0x20];
    unsigned char state;
    char pad21[0x57];
    Vars_2EAE80 *vars;
    char pad7C[0x28];
    unsigned char bA4;
    char padA5;
    short oclass;
    char padA8[0x14];
    unsigned char bBC;
};

typedef struct {
    float f0;
    short h4;
    short h6;
} Sub_2EAE80;

typedef struct {
    char pad0[0x20];
    Sub_2EAE80 sub;
    char pad28[0x38];
    char q60[7];
    unsigned char b67;
    char pad68[0x5C];
    Moby_2EAE80 *list[11];
    Moby_2EAE80 *first;
    char padF4[0xC];
    int f100;
    char pad104[0x18];
    int i11C;
    char pad120[0x18];
    unsigned short h138;
    char pad13A[0xE];
    unsigned char b148;
} Data_2EAE80;

typedef struct {
    char pad0[0x20];
    Moby_2EAE80 *target;
    char pad24[6];
    unsigned short h2A;
    float f2C;
} Hit_2EAE80;

typedef struct {
    char pad0[0x74];
    unsigned char b74;
    char pad75[0x13];
    Moby_2EAE80 *f88;
} Voice_2EAE80;

typedef struct {
    char pad0[0x15F0];
    Moby_2EAE80 *f15F0;
} Hero_2EAE80;

extern char D_00140940[];
extern void begin_starfighter_death_sequence();
int FUN_L13_002e86f8_c(void *unused, unsigned char *p, int base, int n) __asm__("FUN_L13_002e86f8");
void FUN_L13_002e9680_c(void *unused, char *p, int idx) __asm__("FUN_L13_002e9680");
extern char D_0013F450_2EAE80[] __asm__("D_0013F350");
extern char D_00140940[];
extern int D_L13_001D3478[] __asm__("D_L13_001D3478");
extern float func_001FA888_2EAE80(int) __asm__("FUN_001fa6c0");
extern Hit_2EAE80 *func_L00_0025B478_2EAE80(void *, int, int) __asm__("FUN_L00_0025a420");
extern int func_L00_0025B4D0_2EAE80(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_00213928");
extern void func_L13_002EAC18_2EAE80(void *, void *, void *, void *) __asm__("FUN_L13_002e97e0");
extern int func_001F9850_2EAE80(int) __asm__("FUN_001f96f8");
extern void func_L00_0025E4B0_2EAE80(void *, void *) __asm__("FUN_L00_0025d458");
extern void func_0022ED80_2EAE80(int, int, void *) __asm__("FUN_0022da68");
extern void func_L13_002EAAB8_2EAE80(void *, void *, int) __asm__("FUN_L13_002e9680");
extern void func_L00_0028EBF0_2EAE80(int) __asm__("FUN_0022d798");
extern void func_L00_0025E590_2EAE80(void *, void *) __asm__("FUN_L00_0025d538");
extern int func_L13_002E9B30_2EAE80(void *, void *, int, int) __asm__("FUN_L13_002e86f8");

void process_moby_attachment_damage(Moby_2EAE80 *moby, Data_2EAE80 *moby_data) __asm__("FUN_L13_002e9a48");

void process_moby_attachment_damage(Moby_2EAE80 *moby, Data_2EAE80 *moby_data) {
    enum {
        DAMAGE_STAGE_SKIP_LINE_QUEUE = 0,
        DAMAGE_STAGE_QUEUE_RANDOM_LINE = 1,
        DAMAGE_STAGE_START_TIMER_AND_QUEUE_LINE = 2,
        DAMAGE_STAGE_QUEUE_SINGLE_LINE = 3,
        DAMAGE_STAGE_SKIP_LINE_QUEUE_ALTERNATE = 4,
        DAMAGE_STAGE_QUEUE_ALTERNATE_SINGLE_LINE = 5,
        DAMAGE_STAGE_QUEUE_LATE_SINGLE_LINE = 6,
        DAMAGE_STAGE_SKIP_LINE_QUEUE_SECOND_ALTERNATE = 7
    };
    enum {
        CHILD_HIT_HALF_DAMAGE = 0x127,
        CHILD_HIT_FIXED_DAMAGE = 0x3F1
    };
    int hit;
    int rf;
    int mode;
    int i;
    int k;
    int j;
    float lo;
    float hi;
    float k6;
    float step;
    Hit_2EAE80 *r;
    Sub_2EAE80 *dd;

    if (moby->state < 6) {
        dd = &moby_data->sub;
        mode = 0;
        rf = 0;
        k6 = 6.0f;
        step = (float)dd->h4 / k6;
        hi = (k6 - func_001FA888_2EAE80(moby->bBC)) * step;
        if (moby->bBC >= 5) {
            hi = (k6 - func_001FA888_2EAE80(moby->bBC - 1)) * step;
        }
        lo = hi + step * 0.3f;
        if (moby_data->first != 0) {
            r = func_L00_0025B478_2EAE80(moby_data->first, 0x230000, 0);
            func_L00_0025B4D0_2EAE80(moby, r, dd, 0, &hit, 0, 0, 4);
            if (r != 0 && r->target != 0 &&
                (r->target->oclass == 0x52 || r->target->oclass == 0x53 || r->target->oclass == 0xD4)) {
                moby_data->first->bA4 = 0xFF;
            } else if (hit >= 2) {
                if (0.0f < r->f2C && r->h2A == 0x3F1) {
                    r->f2C = 0.3f;
                }
                if (r != 0 && moby->bBC == 7) {
                    r->f2C = r->f2C * 1.5f;
                }
                if (dd->f0 <= r->f2C) {
                    func_L13_002EAC18_2EAE80(moby, moby_data, dd, r);
                    mode = 3;
                    moby_data->first->bA4 = 0xFF;
                } else {
                    dd->f0 = dd->f0 - r->f2C;
                    moby_data->b67 = 0xFA;
                    mode = 1;
                    moby_data->sub.h6 = func_001F9850_2EAE80(0);
                    func_L00_0025E4B0_2EAE80(moby, moby_data->q60);
                    moby_data->first->bA4 = 0xFF;
                }
                if (moby->state == 7) {
                    moby->bBC++;
                    rf = 1;
                    {
                        Vars_2EAE80 *v = (*(Moby_2EAE80 **)D_00140940)->vars;

                        v->f88 = 0;
                        v->fEC = 0;
                        v->f8C = 0;
                        v->f118 = 0;
                    }
                } else if (dd->f0 < hi) {
                    int tv = D_L13_001D3478[moby->bBC - 1];

                    if (tv != -1) {
                        func_0022ED80_2EAE80(7, 0, moby);
                        func_L13_002EAAB8_2EAE80(moby, moby_data, tv);
                    }
                    if (moby->state == 5) {
                        moby->state = 4;
                    }
                    moby->bBC++;
                    if (moby_data->i11C != -1) {
                        if (((Voice_2EAE80 *)(D_0013E550 + moby_data->i11C * 0x70))->f88 == moby &&
                            ((Voice_2EAE80 *)(D_0013E550 + moby_data->i11C * 0x70))->b74 != 0) {
                            func_L00_0028EBF0_2EAE80(moby_data->i11C);
                        }
                        moby_data->i11C = -1;
                    }
                    rf = 1;
                    {
                        Vars_2EAE80 *v = (*(Moby_2EAE80 **)D_00140940)->vars;

                        v->f88 = 0;
                        v->fEC = 0;
                        v->f8C = 0;
                        v->f118 = 0;
                    }
                    moby_data->h138 = 0;
                    moby_data->b148 = 0;
                    mode = 2;
                    if (moby->bBC == 7) {
                        for (j = moby->bBC - 1; j < 10; j++) {
                            func_L13_002EAAB8_2EAE80(moby, moby_data, D_L13_001D3478[j]);
                        }
                        func_0022ED80_2EAE80(7, 0, moby);
                    }
                } else if (dd->f0 < lo) {
                    moby_data->b148 = 1;
                }
            }
        }
        for (i = 0; i < 11 && mode == 0; i++) {
            if (moby_data->list[i] == 0) {
                continue;
            }
            r = func_L00_0025B478_2EAE80(moby_data->list[i], 0x230000, 0);
            func_L00_0025B4D0_2EAE80(moby, r, dd, 0, &hit, 0, 0, 4);
            if (r != 0 && r->target != 0 &&
                (r->target->oclass == 0x52 || r->target->oclass == 0x53 || r->target->oclass == 0xD4)) {
                moby_data->list[i]->bA4 = 0xFF;
                continue;
            }
            if (hit < 2) {
                continue;
            }
            if (0.0f < r->f2C) {
                switch (r->h2A) {
                case CHILD_HIT_FIXED_DAMAGE:
                    r->f2C = 0.1f;
                    break;
                case CHILD_HIT_HALF_DAMAGE:
                    r->f2C = r->f2C * 0.5f;
                    break;
                }
            }
            if (r != 0 && moby->bBC == 7) {
                r->f2C = r->f2C * 1.5f;
            }
            if (dd->f0 <= r->f2C) {
                func_L13_002EAC18_2EAE80(moby, moby_data, dd, r);
                mode = 3;
                moby_data->list[i]->bA4 = 0xFF;
            } else {
                dd->f0 = dd->f0 - r->f2C;
                moby_data->b67 = 0xFA;
                mode = 1;
                moby_data->sub.h6 = func_001F9850_2EAE80(0);
                func_L00_0025E4B0_2EAE80(moby, moby_data->q60);
                moby_data->list[i]->bA4 = 0xFF;
            }
            if (moby->state == 7) {
                moby->bBC++;
                rf = 1;
                {
                    Vars_2EAE80 *v = ((Hero_2EAE80 *)D_0013F450_2EAE80)->f15F0->vars;

                    v->f88 = 0;
                    v->fEC = 0;
                    v->f8C = 0;
                    v->f118 = 0;
                }
            } else if (dd->f0 < hi) {
                int tv = D_L13_001D3478[moby->bBC - 1];

                if (tv != -1) {
                    func_L13_002EAAB8_2EAE80(moby, moby_data, tv);
                }
                func_0022ED80_2EAE80(7, 0, moby);
                if (moby->state == 5) {
                    moby->state = 4;
                }
                moby->bBC++;
                if (moby_data->i11C != -1) {
                    if (((Voice_2EAE80 *)(D_0013E550 + moby_data->i11C * 0x70))->f88 == moby &&
                        ((Voice_2EAE80 *)(D_0013E550 + moby_data->i11C * 0x70))->b74 != 0) {
                        func_L00_0028EBF0_2EAE80(moby_data->i11C);
                    }
                    moby_data->i11C = -1;
                }
                rf = 1;
                {
                    Vars_2EAE80 *v = ((Hero_2EAE80 *)D_0013F450_2EAE80)->f15F0->vars;

                    v->f88 = 0;
                    v->fEC = 0;
                    v->f8C = 0;
                    v->f118 = 0;
                }
                moby_data->h138 = 0;
                moby_data->b148 = 0;
                mode = 2;
                if (moby->bBC == 7) {
                    for (k = moby->bBC - 1; k < 10; k++) {
                        func_L13_002EAAB8_2EAE80(moby, moby_data, D_L13_001D3478[k]);
                    }
                    func_0022ED80_2EAE80(7, 0, moby);
                }
            } else if (dd->f0 < lo) {
                moby_data->b148 = 1;
            }
        }
        if (mode > 0) {
            if (rf != 0) {
                switch (moby->bBC - 1) {
                case DAMAGE_STAGE_SKIP_LINE_QUEUE:
                    break;
                case DAMAGE_STAGE_QUEUE_RANDOM_LINE:
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0xC, 4);
                    break;
                case DAMAGE_STAGE_START_TIMER_AND_QUEUE_LINE:
                    moby_data->f100 = func_001F9850_2EAE80(0x78);
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x10, 1);
                    break;
                case DAMAGE_STAGE_QUEUE_SINGLE_LINE:
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x11, 1);
                    break;
                case DAMAGE_STAGE_SKIP_LINE_QUEUE_ALTERNATE:
                    break;
                case DAMAGE_STAGE_QUEUE_ALTERNATE_SINGLE_LINE:
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x13, 1);
                    break;
                case DAMAGE_STAGE_QUEUE_LATE_SINGLE_LINE:
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x16, 1);
                    break;
                case DAMAGE_STAGE_SKIP_LINE_QUEUE_SECOND_ALTERNATE:
                    break;
                }
            } else if (moby->bBC == 7 && moby_data->b148 != 0) {
                if (!(moby_data->h138 & 1) && func_L13_002E9B30_2EAE80(moby, moby_data, 0x14, 2) != 0) {
                    moby_data->h138 |= 1;
                }
                if (!(moby_data->h138 & 2) && func_L13_002E9B30_2EAE80(moby, moby_data, 0x14, 2) != 0) {
                    moby_data->h138 |= 2;
                }
            }
        }
    }
    func_L00_0025E590_2EAE80(moby, moby_data->q60);
}
/* Steers a moby toward the hero: scales by distance and orients it with the hero's angles. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002EB768), where it is exact; names translated to the US level program. */

extern float FUN_001f9b48(void *, void *);
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

void predict_hero_ship_projectile_lead_position(char *predicted_target_position, char *projectile_origin_position, float projectile_speed) __asm__("FUN_L13_002ea330");

void predict_hero_ship_projectile_lead_position(char *predicted_target_position, char *projectile_origin_position, float projectile_speed) {
    char *base = D_0013E533 + 0xE1D;
    char *vehicle_data = *(char **)(*(char **)(base + 0x15F0) + 0x78);
    float y =
        fast_add_rotations(*(float *)(*(char **)(base + 0x15F0) + 0x48), *(float *)(vehicle_data + 0x90));
    float x =
        fast_add_rotations(*(float *)(*(char **)(base + 0x15F0) + 0x44), *(float *)(vehicle_data + 0x94));
    float projectile_origin_to_vehicle_distance = FUN_001f9b48(projectile_origin_position, *(char **)(base + 0x15F0) + 0x10);
    float k = *(float *)(vehicle_data + 0x64);
    build_spherical_offset(predicted_target_position, projectile_origin_to_vehicle_distance / (k + projectile_speed) * k, y, -x);
    fast_vec_add(predicted_target_position, predicted_target_position, *(char **)(base + 0x15F0) + 0x10);
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002EB838), where it is exact; names translated to the US level program. */

extern void predict_hero_ship_projectile_lead_position(char *a, char *b, float f) __asm__("FUN_L13_002ea330");

float calculate_hero_aim_angles(float *pos, float *pitch, float *yaw, float lead) __asm__("FUN_L13_002ea400");

float calculate_hero_aim_angles(float *pos, float *pitch, float *yaw, float lead) {
    struct Hero *base = &hero;
    char *hero_ship_moby = ((char *)base->ship_moby);
    float target_position[4];
    float y;
    float pitch_angle;
    if (hero_ship_moby != 0 && *(short *)(hero_ship_moby + 0xA6) == 0x45) {
        if (lead > 0.0f) {
            predict_hero_ship_projectile_lead_position((char *)target_position, (char *)pos, lead);
        } else {
            qcopy(target_position, hero_ship_moby + 0x10);
        }
        y = fast_arc_tan(target_position[0] - pos[0], target_position[1] - pos[1]);
        pitch_angle = -fast_arc_tan(vector_distance(pos, target_position), target_position[2] - pos[2]);
    } else {
        struct Hero *hero_state = &hero;
        y = fast_arc_tan(hero_state->motion.pos.f[0] - pos[0], hero_state->motion.pos.f[1] - pos[1]);
        pitch_angle = -fast_arc_tan(vector_distance(pos, &hero_state->motion.pos), hero_state->motion.pos.f[2] - pos[2]);
    }
    if (yaw != 0)
        *yaw = y;
    if (pitch != 0)
        *pitch = pitch_angle;
    return y;
}
/* Fires a shot from one of two mount points, alternating, toward the hero. */
extern float D_0015ED6C_e5 __asm__("D_0015ED6C");
extern int FUN_001160d8_e5(void) __asm__("FUN_001160d8");
extern void FUN_L00_0024f7c8_e5(char *, int, float *) __asm__("FUN_L00_0024f7c8");
extern unsigned char *FUN_L13_002c13b0_e5(char *, char *, char *, char *, float, int, float) __asm__("FUN_L13_002c13b0");
extern void FUN_L13_002ea400_c(float *, float *, float *, float) __asm__("FUN_L13_002ea400");
extern int FUN_001f96f8_e5(int) __asm__("FUN_001f96f8");
extern void FUN_0022da68_e5(int, int, char *) __asm__("FUN_0022da68");

unsigned char *fire_starfighter_homing_rocket(struct Moby *firing_moby, unsigned char *starfighter_data) __asm__("FUN_L13_002ea540");

unsigned char *fire_starfighter_homing_rocket(struct Moby *firing_moby, unsigned char *starfighter_data) {
    float muzzle_position[4];
    struct {
        int a;
        float b;
        float c;
    } s;
    int muzzle_joint_index;
    float projectile_speed;
    unsigned char *spawned_rocket_moby;
    struct Hero *hero_state;
    muzzle_joint_index = (starfighter_data[0xFC] & 1) ? 0x12 : 0x10;
    if (firing_moby->command != 6 || (FUN_001160d8_e5() & 1)) {
        starfighter_data[0xFC] ^= 1;
    }
    FUN_L00_0024f7c8_e5(firing_moby, muzzle_joint_index, muzzle_position);
    s.a = 0;
    if (firing_moby->state != 5) {
        FUN_L13_002ea400_c(muzzle_position, &s.b, &s.c, D_0015ED6C_e5 * 17.0f);
    } else {
        FUN_L13_002ea400_c(muzzle_position, &s.b, &s.c, -1.0f);
    }
    hero_state = &hero;
    projectile_speed = D_0015ED6C_e5 * 18.0f + *(float *)(starfighter_data + 0xAC);
    s.c = firing_moby->rot.z;
    s.b = 0.43633232f;
    spawned_rocket_moby = FUN_L13_002c13b0_e5(firing_moby, (char *)muzzle_position, ((char *)hero_state->ship_moby), (char *)&s,
                         projectile_speed, FUN_001f96f8_e5(300), D_0015ED6C_e5 * 18.0f);
    if (spawned_rocket_moby != 0) {
        FUN_0022da68_e5(2, 0, firing_moby);
    }
    return spawned_rocket_moby;
}
/* computes a velocity vector for a moby from the camera/player and spawns a projectile */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002C2638.c: func_L13_002EBAF0), where it is exact; names translated to the US level program. */

extern unsigned char *spawn_class_53_projectile(char *owner, char *pos,
                                         char *vec) __asm__("FUN_L13_002c1f28");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void predict_hero_ship_projectile_lead_position(char *a, char *b, float f) __asm__("FUN_L13_002ea330");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");

unsigned char *fire_class_53_projectile(char *firing_moby, char *velocity_source_data) __asm__("FUN_L13_002ea6b8");

unsigned char *fire_class_53_projectile(char *firing_moby, char *velocity_source_data) {
    float spawn_position[4], projectile_velocity[4], predicted_target_position[4], scratch_vector[4], random_velocity_offset[4];
    struct Hero *x;
    unsigned char *spawned_projectile_moby;
    moby_point_position(firing_moby, 0xE, spawn_position);
    x = &hero;
    if (((char *)x->ship_moby) != 0 && *(short *)(((char *)x->ship_moby) + 0xA6) == 0x45) {
        predict_hero_ship_projectile_lead_position((char *)predicted_target_position, (char *)spawn_position, frame_time * 20.0f);
        subtract_vector_xyz(scratch_vector, predicted_target_position, spawn_position);
        normalize_vector_xyz(projectile_velocity, scratch_vector, frame_time * 20.0f);
        normalize_vector_xyz(random_velocity_offset, ((char *)x->ship_moby) + 0xD0,
                             random_float_between_alt(-(frame_time * 30.0f), frame_time * 30.0f));
        normalize_vector_xyz(scratch_vector, ((char *)x->ship_moby) + 0xE0,
                             random_float_between_alt(-(frame_time * 30.0f), frame_time * 30.0f));
        add_vector_xyz(random_velocity_offset, random_velocity_offset, scratch_vector);
        normalize_vector_xyz(random_velocity_offset, random_velocity_offset,
                             random_float_between_alt(frame_time * 5.0f, frame_time * 30.0f));
        add_vector_xyz(projectile_velocity, projectile_velocity, random_velocity_offset);
    } else {
        random_vector(projectile_velocity, frame_time * 0.0f, frame_time * 30.0f);
    }
    add_vector_xyz(projectile_velocity, projectile_velocity, velocity_source_data + 0x70);
    spawned_projectile_moby = spawn_class_53_projectile(firing_moby, (char *)spawn_position, (char *)projectile_velocity);
    if (spawned_projectile_moby)
        allocate_voice_for_target_entry(3, 0, (int)firing_moby);
    return spawned_projectile_moby;
}
