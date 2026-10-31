/* Shared moby class devastator: the missile launcher held by the hero, and its muzzle particles. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/point_light.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/weapons/devastator.h"

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2b1af0;

typedef struct {
    V_2b1af0 a;
    char     pad[0x30];
    V_2b1af0 b;
} P_2b1af0;

typedef struct {
    Vec_2b2100 row[3];
} Matrix3;

typedef struct {
    Vec_2b2100 row[4];
} Matrix4;

typedef struct Class_2b2100 {
    char  pad0[0x46];
    short category;
} Class_2b2100;

typedef struct Devastator_2b2100 {
    int          cooldown;
    char         pad4[0xC];
    struct Moby *target;
    int          target_timer;
    float        target_height;
    short        light;
    short        glow;
} Devastator_2b2100;

typedef struct UsageStat_2b2100 {
    char           pad0[0x58];
    unsigned short uses;
    unsigned short longest;
    int            levels;
} UsageStat_2b2100;

typedef struct Hit_2b2100 {
    char         pad0[0x18];
    struct Moby *moby;
    char         pad1C[4];
    Vec_2b2100   pos;
} Hit_2b2100;

typedef struct AimInfo {
    char          pad0[0xA];
    unsigned char radius;
    char          padB[5];
    float         height;
} AimInfo;

extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern int  random_int_between(int, int) __asm__("FUN_L00_00257b90");
float       randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
void        spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
void spawn_particle(void *, void *, int, int, int, int, float, float, float, float, float) __asm__("FUN_L00_002712b8");
void         fast_subtract_rotations_cf(void *) __asm__("FUN_L00_001fefc8");
void         scale_2b1af0(void *, void *, float) __asm__("FUN_001f9a68");
extern void  hero_set_state_2b2100(int, int) __asm__("FUN_L00_002223f8");
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void
suck_draw_reticle(int, int, float, void *, float, int, int, float, int) __asm__("FUN_L00_001edf60");
extern int   get_ammo(int) __asm__("FUN_L00_00233e98");
extern void  blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int   use_ammo(int, int) __asm__("FUN_L00_00233db8");
extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int   find_listed_moby_by_data_id(void *) __asm__("FUN_L00_0025f878");
extern void *spawn_devastator_missile(
    void *, void *, void *, float, float, float, float, float) __asm__("FUN_L00_002afc70");
extern void set_hero_animation(int, int, float) __asm__("FUN_L00_002323b8");
extern void start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
extern int
allocate_colored_effect_slot(void *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");

extern P_2b1af0     l00_object_offset_particle_velocity_contributions __asm__("D_0013F450");
extern V_2b1af0     l00_object_offset_burst_reference_direction __asm__("D_0013F9B0");
extern char         usage_stats_2b2100[] __asm__("D_00141680");
extern char         view_origin_2b2100[] __asm__("D_L00_00166DC0");
extern char         coll_hit[] __asm__("D_L00_00173E40");
extern struct Moby *D_L00_001ABA00_2b2100[] __asm__("D_L00_001ABA00");
extern int          D_L00_0015F5CC_2b2100 __asm__("D_L00_0015F5CC");
extern int          D_0015EEA4;
extern float        D_L00_0017ACF4_lui __asm__("D_L00_0017ACF4") NOT_SDA;
extern s32          D_0015EEA4;
extern int          D_L00_0015F5C4; /* no foreign declaration */

float D_L01_0017B174         NOT_SDA = 0;
float D_L02_0017B2F4         NOT_SDA = 0;
unsigned char D_L02_001808C0 NOT_SDA = 0;
float D_L03_0017ADF4         NOT_SDA = 0;
unsigned char D_L03_001803C0 NOT_SDA = 0;
float D_L04_0017AE74         NOT_SDA = 0;
unsigned char D_L04_00180440 NOT_SDA = 0;
float D_L05_0017B154         NOT_SDA = 0;
unsigned char D_L05_00180740 NOT_SDA = 0;
float D_L06_0017B474         NOT_SDA = 0;
unsigned char D_L06_00180A40 NOT_SDA = 0;
float D_L07_0017AD74         NOT_SDA = 0;
float D_L08_0017B4F4         NOT_SDA = 0;
float D_L09_0017AE74         NOT_SDA = 0;
float D_L10_0017B174         NOT_SDA = 0;
float D_L11_0017B6F4         NOT_SDA = 0;
unsigned char D_L11_00180CC0 NOT_SDA = 0;
float D_L12_0017B174         NOT_SDA = 0;
unsigned char D_L12_00180740 NOT_SDA = 0;
float D_L13_0017AFF4         NOT_SDA = 0;
float D_L14_0017B3F4         NOT_SDA = 0;
float D_L15_0017B2F4         NOT_SDA = 0;
float D_L16_0017B154         NOT_SDA = 0;
unsigned char D_L16_00180740 NOT_SDA = 0;
float D_L17_0017B5F4         NOT_SDA = 0;
unsigned char D_L17_00180BC0 NOT_SDA = 0;
float D_L18_0017B6F4         NOT_SDA = 0;
unsigned char D_L18_00180CC0 NOT_SDA = 0;

/* Sums n steps of a value easing toward 20 * D_0015ED6C by a tenth (times D_0015ED64) per step. */
float calculate_eased_step_sum(int step_count) __asm__("FUN_L00_002aff50");

float calculate_eased_step_sum(int step_count) {
    float sum = 0.0f, eased_value = sum;
    if (step_count > 0) {
        float target_value = frame_time * 20.0f;
        do {
            eased_value += (target_value - eased_value) / 10.0f * frame_scale_sq;
            step_count--;
            sum += eased_value;
        } while (step_count);
    }
    return sum;
}

void emit_object_offset_particles(unsigned char *source_object) __asm__("FUN_L00_002b1af0");

void emit_object_offset_particles(unsigned char *source_object) {
    V_2b1af0 b;
    V_2b1af0 a;
    V_2b1af0 emission_position;
    V_2b1af0 particle_velocity;
    int      i, j;
    int      particle_byte_input_or_min_frames;
    if (source_object[0xBC] == 0)
        return;
    *(OvlQuad *)&a = 0;
    *(OvlQuad *)&b = 0;
    a.f[0] = 0.23f;
    a.f[1] = 0.18f;
    b.f[1] = 1.0f;
    b.f[3] = 1.0f;
    a.f[3] = 1.0f;
    fast_subtract_rotations_cf(source_object + 0xBC);
    transform_vector_by_basis(&a, &a, source_object + 0xC0);
    fast_vec_add(&emission_position, source_object + 0x10, &a);
    transform_vector_by_basis(&b, &b, source_object + 0xC0);
    for (i = 1; i >= 0; i--) {
        fast_vec_normalize(&particle_velocity, &b, random_float_between(0.01f, 0.06f));
        scale_2b1af0(&b, &l00_object_offset_particle_velocity_contributions.a, 0.7f);
        b.f[2] = 0.0f;
        fast_vec_add(&particle_velocity, &particle_velocity, &b);
        scale_2b1af0(&b, &l00_object_offset_particle_velocity_contributions.b, 0.7f);
        b.f[2] = 0.0f;
        fast_vec_add(&particle_velocity, &particle_velocity, &b);
        particle_byte_input_or_min_frames = truncate_float_to_s32(randf_sym(0.0f, 6.0f));
        spawn_particle(&emission_position,
                       &particle_velocity,
                       scale_game_frames(0x1E),
                       0x7F,
                       0xFFFFFF,
                       particle_byte_input_or_min_frames,
                       25000.0f,
                       300.0f,
                       0.98f,
                       0.0f,
                       0.85f);
    }
    for (j = 0; j >= 0; j--) {
        fast_vec_normalize(&particle_velocity, &b, random_float_between(0.01f, 0.06f));
        scale_2b1af0(&b, &l00_object_offset_particle_velocity_contributions.a, 0.55f);
        b.f[2] = 0.0f;
        fast_vec_add(&particle_velocity, &particle_velocity, &b);
        scale_2b1af0(&b, &l00_object_offset_particle_velocity_contributions.b, 0.55f);
        b.f[2] = 0.0f;
        fast_vec_add(&particle_velocity, &particle_velocity, &b);
        particle_byte_input_or_min_frames = scale_game_frames(0x14);
        spawn_burst(&emission_position,
                    &particle_velocity,
                    0x4F007FFF,
                    0x1FFFFFFF,
                    random_int_between(particle_byte_input_or_min_frames, scale_game_frames(0x28)),
                    1,
                    10000.0f);
    }
}

void emit_object_offset_particle_burst(unsigned char *source_object) __asm__("FUN_L00_002b1de0");

void emit_object_offset_particle_burst(unsigned char *source_object) {
    V_2b1af0 particle_velocity;
    V_2b1af0 emission_offset;
    V_2b1af0 emission_position;
    V_2b1af0 scaled_global_vector;
    int      i;
    int      j;
    int      signed_random_particle_argument;
    float    angle;

    *(OvlQuad *)&emission_offset = 0;
    emission_offset.f[0] = 0.3f;
    emission_offset.f[1] = -1.2f;
    emission_offset.f[3] = 1.0f;
    transform_vector_by_basis(&emission_offset, &emission_offset, source_object + 0xC0);
    fast_vec_add(&emission_position, source_object + 0x10, &emission_offset);
    for (i = 0; i < truncate_float_to_s32(24.0f); i++) {
        angle = randf_sym(0.0f, 0.1308997f);
        build_look_at_matrix(&particle_velocity,
                             &l00_object_offset_burst_reference_direction,
                             (char *)&l00_object_offset_burst_reference_direction - 0x20,
                             (float)i * 0.2617994f + angle);
        fast_vec_normalize(
            &particle_velocity, &particle_velocity, random_float_between(0.05f, 0.1f));
        signed_random_particle_argument = random_int_between(0, 3);
        if (random_integer_below(2) != 0)
            signed_random_particle_argument = -signed_random_particle_argument;
        if (*(int *)((char *)&l00_object_offset_burst_reference_direction - 0x360) != 0) {
            scale_2b1af0(&scaled_global_vector,
                         (char *)&l00_object_offset_burst_reference_direction - 0x560,
                         1.7f);
            fast_vec_add(&particle_velocity, &particle_velocity, &scaled_global_vector);
        }
        {
            int low = scale_game_frames(30);
            int high = scale_game_frames(90);
            spawn_particle(&emission_position,
                           &particle_velocity,
                           random_int_between(low, high),
                           30,
                           0xFFFFFF,
                           signed_random_particle_argument,
                           60000.0f,
                           3000.0f,
                           0.85f,
                           -0.001f,
                           0.0f);
        }
    }
    for (j = 0; j < truncate_float_to_s32(6.0f); j++) {
        angle = random_float_between(0.0f, 0.7853982f);
        build_look_at_matrix(&particle_velocity,
                             &l00_object_offset_burst_reference_direction,
                             (char *)&l00_object_offset_burst_reference_direction - 0x20,
                             (float)j * 1.0471976f + angle);
        fast_vec_normalize(
            &particle_velocity, &particle_velocity, random_float_between(0.05f, 0.1f));
        {
            int low = scale_game_frames(20);
            int high = scale_game_frames(60);
            spawn_burst(&emission_position,
                        &particle_velocity,
                        0x4F007FFF,
                        0x1FFFFFFF,
                        random_int_between(low, high),
                        1,
                        10000.0f);
        }
    }
    source_object[0xBC] = scale_game_frames(5);
}

/* Devastator (class 157) update: tracks the aimed target, draws the reticle, picks a target and fires,
   keeps its usage statistics and moves the muzzle light. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B33E8), where it is exact; names translated to the US level program. */
void devastator_update(struct Moby *m) __asm__("FUN_L00_002b2100");

/* Moby class 157 */
void devastator_update(struct Moby *devastator_moby) {
    Matrix3            rotation;
    Matrix4            inverse;
    Vec_2b2100         velocity;
    Vec_2b2100         launch;
    Vec_2b2100         point;
    Vec_2b2100         probe;
    Devastator_2b2100 *devastator_vars = (Devastator_2b2100 *)devastator_moby->pvars;
    struct Moby       *best;
    struct Moby       *candidate_moby;
    AimInfo           *aim_info;
    UsageStat_2b2100  *stats;
    Hit_2b2100        *hit;
    float              height, limit, yaw, pitch, yaw0, pitch0;
    float              size;
    float              muzzle_light_color_x, muzzle_light_color_y, muzzle_light_color_z;
    int                i;

    emit_object_offset_particles(devastator_moby);
    height = 0.75f;
    limit = 10000.0f;
    if ((unsigned char)devastator_moby->state == 0) {
        devastator_moby->state = 1;
        devastator_vars->light = -1;
        devastator_vars->glow = 0;
    }
    copy_matrix3x4(&rotation, l00_camera.unk0);
    transpose_rotation_matrix(&inverse, &rotation);
    fast_dec_timer(&devastator_vars->cooldown);
    if (fast_dec_timer(&devastator_vars->target_timer) ||
        (devastator_vars->target && ((unsigned char)devastator_vars->target->state & 0x80)))
        devastator_vars->target = 0;
    if (hero.state.current == 1)
        hero_set_state_2b2100(30, 1);
    if (hero.state.current == 30 && (hero.moby->flags & 1) && !hero.item_use_locked) {
        velocity.q = 0;
        velocity.f.z = 70.0f;
        transform_vector(&velocity, &velocity, &inverse);
        fast_vec_add(&launch, view_origin_2b2100, &velocity);
        if (collision_line(view_origin_2b2100, &launch, 0, (int)devastator_moby, 0)) {
            hit = (Hit_2b2100 *)coll_hit;
            if (hit->moby && (hit->moby->flags & 0x1000)) {
                devastator_vars->target = hit->moby;
                devastator_vars->target_timer = scale_game_frames(20);
                devastator_vars->target_height = hit->pos.f.z - devastator_vars->target->pos.z;
            }
        }
        suck_draw_reticle((int)devastator_moby,
                          0xFF180A65,
                          1.0f,
                          0,
                          fast_normalize_angle(convert_integer_to_float(D_L00_0015F5CC_2b2100 %
                                                                        scale_game_frames(240)) *
                                               360.0f / scale_time(240.0f) * 0.017453292f),
                          0x27,
                          -1,
                          90.0f,
                          4);
        if (devastator_vars->target && !devastator_vars->cooldown && get_ammo(-1)) {
            float scaled_target_distance =
                fast_vec_dist(view_origin_2b2100, &devastator_vars->target->pos) / 100.0f;
            qcopy(&point, &devastator_vars->target->pos);
            point.f.z += devastator_vars->target_height;
            size = absolute_float(0.9f - scaled_target_distance * 0.57f);
            suck_draw_reticle(
                (int)devastator_moby,
                0xFF0FFF0F,
                size,
                &point,
                fast_normalize_angle(
                    convert_integer_to_float(D_L00_0015F5CC_2b2100 % scale_game_frames(240)) *
                    -360.0f / scale_time(240.0f) * 0.017453292f),
                0x27,
                -1,
                90.0f,
                4);
        }
    }
    if (hero.items[6].locked_on) {
        devastator_vars->target = hero.items[6].lock_target;
        devastator_vars->target_timer = scale_game_frames(20);
        devastator_vars->target_height = collision_hit_z - devastator_vars->target->pos.z;
    }
    if (D_L00_0015F5C4 != 2 && !devastator_vars->cooldown && !hero.item_use_locked &&
        devastator_moby->seq < 2 && (controller_state.held & hero.items[0].button_mask) &&
        scale_game_frames(22) < hero.items[0].ready_frames) {
        velocity.q = 0;
        if (devastator_moby->prev_seq == 0)
            blend_moby_animation(devastator_moby, 1, 0, scale_game_frames(4));
        if (hero.state.current == 30 && (hero.moby->flags & 1)) {
            velocity.f.z = frame_time * 10.0f;
            transform_vector(&velocity, &velocity, &inverse);
        } else {
            velocity.f.x = frame_time * 10.0f;
            transform_vector_by_basis(&velocity, &velocity, &hero.basis);
        }
        if (use_ammo(-1, 1)) {
            best = 0;
            if (hero.items[6].locked_on)
                yaw = hero.items[6].lock_yaw;
            else
                yaw = fast_arc_tan(velocity.f.x, velocity.f.y);
            pitch = -fast_arc_tan(vector_length_xy(&velocity), velocity.f.z);
            yaw0 = yaw;
            pitch0 = pitch;
            if (hero.state.current == 30 && (hero.moby->flags & 1)) {
                clear_u64_value(&launch);
                launch.f.x += 0.15f;
                launch.f.z += 0.75f;
                launch.f.y += 0.3f;
                transform_vector(&launch, &launch, &inverse);
                fast_vec_add(&launch, &launch, view_origin_2b2100);
                if (devastator_vars->target)
                    height = devastator_vars->target_height;
            } else {
                moby_point_position(devastator_moby, 0, &launch);
                i = 0;
                candidate_moby = D_L00_001ABA00_2b2100[i];
                while (candidate_moby) {
                    if ((*(signed char *)&candidate_moby->state) >= 0) {
                        qcopy(&point, &candidate_moby->pos);
                        aim_info = get_moby_target_info(candidate_moby);
                        if (aim_info)
                            point.f.z += aim_info->height;
                        else
                            point.f.z += 0.5f;
                        if ((candidate_moby->flags & 0x1000) && candidate_moby &&
                            ((Class_2b2100 *)candidate_moby->pclass) &&
                            ((Class_2b2100 *)candidate_moby->pclass)->category == 5) {
                            float tyaw, tpitch, distance, spread, aim_angle_error;
                            tyaw = fast_arc_tan(point.f.x - launch.f.x, point.f.y - launch.f.y);
                            tpitch = fast_arc_tan(vector_distance_xy(&launch, &point),
                                                  point.f.z - launch.f.z);
                            distance = fast_vec_dist(&launch, &point);
                            if (distance < 2.5f) {
                                struct Moby *hero_moby = hero.moby;
                                if (fast_difference_between_rotations(
                                        hero_moby->rot.z,
                                        fast_arc_tan(candidate_moby->pos.x - hero.motion.pos.f[0],
                                                     candidate_moby->pos.y -
                                                         hero.motion.pos.f[1])) < 1.0471976f &&
                                    absolute_float(tpitch) < 0.7853982f) {
                                    limit = distance;
                                    pitch = -tpitch;
                                    best = candidate_moby;
                                    yaw = tyaw;
                                    break;
                                }
                            }
                            if (!(limit < distance)) {
                                spread =
                                    convert_integer_to_float(item_text_variant[11]) * 0.6981317f;
                                build_spherical_offset(
                                    &probe, distance, yaw, pitch + hero.ground_pitch.f * 0.5f);
                                fast_vec_add(&probe, &probe, &launch);
                                aim_angle_error = fast_arc_sin(fast_vec_dist(&probe, &point) /
                                                               (distance + distance));
                                aim_angle_error = aim_angle_error + aim_angle_error;
                                if (spread + 0.17453292f < aim_angle_error) {
                                    if (aim_info) {
                                        float radius =
                                            convert_integer_to_float(aim_info->radius) * 0.125f;
                                        if (radius < distance) {
                                            aim_angle_error -= fast_arc_sin(radius / distance);
                                            if (aim_angle_error < 0.0f)
                                                aim_angle_error = 0.0f;
                                        }
                                    }
                                }
                                if (aim_angle_error < spread + 0.17453292f) {
                                    if (!collision_line(view_origin_2b2100,
                                                        &point,
                                                        6,
                                                        (int)devastator_moby,
                                                        0)) {
                                        best = candidate_moby;
                                        limit = distance;
                                        pitch = -tpitch;
                                        yaw = tyaw;
                                        if (!find_listed_moby_by_data_id(best)) {
                                            height = 0.5f;
                                            devastator_vars->target = best;
                                            if (aim_info)
                                                height = aim_info->height;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    i++;
                    candidate_moby = D_L00_001ABA00_2b2100[i];
                }
            }
            allocate_voice_for_target_entry(0, 0, (int)devastator_moby);
            devastator_vars->glow = scale_game_frames(5) + scale_game_frames(10);
            if (hero.state.current != 30)
                emit_object_offset_particle_burst(devastator_moby);
            stats = (UsageStat_2b2100 *)usage_stats_2b2100;
            if (stats->uses < 65535)
                stats->uses++;
            if (scale_game_frames(D_0015EEA4) / 600 > stats->longest)
                stats->longest = scale_game_frames(D_0015EEA4) / 600;
            stats->levels = (stats->levels | (1 << current_level_index)) | 0x80000000;
            if (hero.unk22C8 == 1 && hero.state.current != 30) {
                pitch = D_L00_0017ACF4_lui;
                yaw = devastator_moby->rot.z;
                aim_info = get_moby_target_info(devastator_vars->target);
                if (aim_info)
                    height = aim_info->height;
                spawn_devastator_missile(devastator_moby,
                                         &launch,
                                         devastator_vars->target,
                                         yaw,
                                         pitch,
                                         height,
                                         yaw,
                                         pitch);
            } else if (limit < 2.0f) {
                aim_info = get_moby_target_info(devastator_vars->target);
                if (aim_info)
                    height = aim_info->height;
                spawn_devastator_missile(devastator_moby,
                                         &launch,
                                         devastator_vars->target,
                                         yaw,
                                         pitch,
                                         height,
                                         yaw,
                                         pitch);
            } else {
                if (!devastator_vars->target) {
                    if (best) {
                        limit = calculate_eased_step_sum(6);
                        qcopy(&point, &best->pos);
                        aim_info = get_moby_target_info(best);
                        if (aim_info)
                            point.f.z += aim_info->height;
                        else
                            point.f.z += 0.5f;
                        pitch0 -= hero.ground_pitch.f * 0.5f;
                        build_spherical_offset(&probe, limit, yaw0, pitch0);
                        fast_vec_add(&probe, &probe, &launch);
                        yaw = fast_arc_tan(point.f.x - probe.f.x, point.f.y - probe.f.y);
                        pitch = -fast_arc_tan(vector_distance_xy(&probe, &point),
                                              point.f.z - probe.f.z);
                    } else if (hero.state.current != 30) {
                        pitch -= hero.ground_pitch.f * 0.5f;
                        pitch0 -= hero.ground_pitch.f * 0.5f;
                    }
                }
                spawn_devastator_missile(devastator_moby,
                                         &launch,
                                         devastator_vars->target,
                                         yaw,
                                         pitch,
                                         height,
                                         yaw0,
                                         pitch0);
            }
            if (hero.state.control_mode == 0 ||
                (hero.state.current == 2 && hero.stick_magnitude < 0.7f)) {
                set_hero_animation(0x36, 1, (float)scale_game_frames(10));
            } else {
                start_equipped_item_animation();
            }
        } else {
            allocate_voice_for_target_entry(1, 0, (int)devastator_moby);
        }
        devastator_vars->cooldown = scale_game_frames(35);
    }
    if (devastator_moby->seq != 2 && devastator_vars->glow > 0) {
        if (devastator_vars->glow > scale_game_frames(10)) {
            muzzle_light_color_x =
                convert_integer_to_float(scale_game_frames(5) + scale_game_frames(10) -
                                         devastator_vars->glow) /
                (float)scale_game_frames(5);
            muzzle_light_color_y =
                convert_integer_to_float(scale_game_frames(5) + scale_game_frames(10) -
                                         devastator_vars->glow) *
                0.8f / (float)scale_game_frames(5);
            muzzle_light_color_z =
                convert_integer_to_float(scale_game_frames(5) + scale_game_frames(10) -
                                         devastator_vars->glow) *
                0.5f / (float)scale_game_frames(5);
        } else {
            muzzle_light_color_x =
                convert_integer_to_float(devastator_vars->glow) / (float)scale_game_frames(10);
            muzzle_light_color_y = convert_integer_to_float(devastator_vars->glow) * 0.8f /
                                   (float)scale_game_frames(10);
            muzzle_light_color_z = convert_integer_to_float(devastator_vars->glow) * 0.5f /
                                   (float)scale_game_frames(10);
        }
        moby_point_position(devastator_moby, 0, &velocity);
        velocity.f.x += fast_cos(fast_arc_tan(hero.facing_x, hero.facing_y));
        velocity.f.y += fast_sin(fast_arc_tan(hero.facing_x, hero.facing_y));
        velocity.f.z += 0.3f;
        if (devastator_vars->light == -1) {
            devastator_vars->light = allocate_colored_effect_slot(&velocity,
                                                                  7.0f,
                                                                  0.0f,
                                                                  muzzle_light_color_x,
                                                                  muzzle_light_color_y,
                                                                  muzzle_light_color_z);
        } else {
            PointLight *muzzle_light = &point_lights[devastator_vars->light];
            qcopy(&muzzle_light->position, &velocity);
            muzzle_light->position.f[3] = 7.0f;
            muzzle_light->color[0] = muzzle_light_color_x;
            muzzle_light->color[1] = muzzle_light_color_y;
            muzzle_light->color[2] = muzzle_light_color_z;
        }
        fast_dec_timer_s16(&devastator_vars->glow);
    } else if (devastator_vars->light != -1) {
        release_point_light_slot(devastator_vars->light);
        devastator_vars->light = -1;
        devastator_vars->glow = 0;
    }
    if (devastator_moby->prev_seq == 2) {
        release_point_light_slot(devastator_vars->light);
        devastator_vars->light = -1;
        devastator_vars->glow = 0;
    }
}
