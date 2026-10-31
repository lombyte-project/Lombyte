/* Level 15 ultramech unlimited moby class: flocking, hits and targeting. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/enemies/trooper_play_sound.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/world/collision/collision_line.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l15/mobys/ultramech_unlimited_globals.h"

extern char D_0014C190[];

extern int D_L15_001BAAD0[];

extern float D_0015ED6C;

extern char *D_L15_0015FFE4;

extern float D_L15_0015F580[] MACRO_ADDR;

extern int FUN_L00_00257b90(int, int);

extern float spring_turn(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern void *FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);

extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");

extern LevelPath *D_L15_001B0AB0[];

extern LevelSaveState D_L15_001BABD0;

extern void FUN_L00_0025df68(void *, void *, void *, int *, float *, int, float, float, float);

extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");

int select_moby_combat_target(char *a, char *o, float best) __asm__("FUN_L00_0025fcb8");

float compute_ultramech_flocking_steering_point(
    void *m_, void *out, float target_search_range) __asm__("FUN_L15_002cf110");

float compute_ultramech_flocking_steering_point(void *m_,
                                                void *steering_point,

                                                float target_search_range) {
    char *moby = m_;
    float steering_offset[4];
    float target_position[4];
    float pad[4][4]; /* unused, but retail reserves the stack space */
    float target_distance;
    float wsum = 0.0f;
    int   clear = 1;
    float k;
    char *other_moby;
    select_moby_combat_target(moby, target_position, target_search_range);
    target_distance = vector_distance_xy(moby + 0x10, target_position);
    clear_u64_value(steering_point);
    for (other_moby = D_L15_0015FFE4; other_moby != 0;
         other_moby = ((char *)MOBY(other_moby)->next)) {
        if (other_moby == moby)
            continue;
        if (MOBY(other_moby)->oclass != MOBY(moby)->oclass)
            continue;
        if (vector_distance_xy(other_moby + 0x10, moby + 0x10) < 3.0f) {
            wsum += 6.0f;
            fast_vec_sub(steering_offset, moby + 0x10, other_moby + 0x10);
            scale_vector_xyz(steering_offset, steering_offset, 6.0f);
            fast_vec_add(steering_point, steering_point, steering_offset);
        }
        if (vector_distance_xy(other_moby + 0x10, target_position) < target_distance)
            clear = 0;
    }
    if (clear == 0) {
        fast_vec_sub(steering_offset, moby + 0x10, target_position);
        if (target_distance < 9.5f) {
            k = 10.0f;
        } else if (10.5f < target_distance) {
            k = -10.0f;
        } else {
            k = 0.0f;
        }
        wsum += 10.0f;
        normalize_vector_xyz(steering_offset, steering_offset, k * 10.0f);
        fast_vec_add(steering_point, steering_point, steering_offset);
        scale_vector_xyz(steering_point, steering_point, 1.0f / wsum);
        fast_vec_add(steering_point, steering_point, moby + 0x10);
    } else {
        qcopy(steering_point, target_position);
    }
    if (vector_distance_xy(moby + 0x10, steering_point) < 1.0f) {
        qcopy(steering_point, moby + 0x10);
        return fast_arc_tan(target_position[0] - MOBY(moby)->pos.x,
                            target_position[1] - MOBY(moby)->pos.y);
    }
    return fast_arc_tan(((float *)steering_point)[0] - MOBY(moby)->pos.x,
                        ((float *)steering_point)[1] - MOBY(moby)->pos.y);
}

typedef struct UData UData;

typedef struct UMoby UMoby;

struct UMoby {
    char           pad0[0x10];
    Vec4           pos;
    unsigned char  state;
    char           pad21[3];
    char          *cls;
    char           pad28[8];
    unsigned char  b30;
    char           pad31;
    unsigned short h32;
    unsigned short flags;
    char           pad36[0xA];
    Vec4           rot;
    char           pad50[3];
    unsigned char  anim;
    char           pad54[4];
    float          animSpeed;
    char           pad5C[0x14];
    unsigned char  animFlags;
    char           pad71[7];
    UData         *data;
    char           pad7C[0x18];
    int            w94;
    char           pad98[0x1A];
    unsigned short id;
    char           padB4[0xC];
    char           mtx[0x40];
};

struct UData {
    char           pad0[0x20];
    float          f20;
    short          h24;
    char           pad26[2];
    unsigned char  b28;
    unsigned char  b29;
    char           pad2A[6];
    float          f30;
    char           pad34[0xA];
    unsigned short h3E;
    Vec4           vel;
    char           pad50[8];
    unsigned char  b58;
    char           pad59[0x17];
    char           hit[0x78];
    float          fE8;
    char           padEC[0x74];
    UMoby         *target;
    int            w164;
    char           pad168[8];
    int            path;
    float          f174;
    char           pad178[8];
    int            paths[2];
    float          f188;
    char           pad18C[0x14];
    Vec4           v1A0;
    int            side;
    int            point;
    float          f1B8;
    float          turnVel;
    int            timer;
    int            shots;
    int            w1C8;
    int            w1CC;
    int            w1D0;
    int            w1D4;
    int            w1D8;
    int            w1DC;
    int            snd;
    char           pad1E4[0x1C];
    unsigned char  v200[0x10];
};

typedef struct {
    int idx[2];
} UPaths;

extern float FUN_L04_001f1978(float);

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");

extern float FUN_L00_0025abf0(void *a, void *b, void *out, float speed, float g);

extern float approach_angle(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");

extern float
compute_ultramech_flocking_steering_point(void *, void *, float) __asm__("FUN_L15_002cf110");

extern int FUN_L00_00258490(void *, void *, void *, int, float, float, float, float);

extern int sweep_hit_segment(void *a, void *b, void *c, void *d, void *e, void *f, int n) __asm__("FUN_L00_00259d08");

extern int step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");

extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");

extern int is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");

extern int rand(void);

extern short ultramech_path_horizontal_speed_gp __asm__("D_L15_00161B14") __attribute__((sda));

extern short ultramech_path_gravity_gp __asm__("D_L15_00161B18") __attribute__((sda));

extern short ultramech_projectile_speed_gp __asm__("D_L15_00161B20") __attribute__((sda));

extern short ultramech_approach_target_range_gp __asm__("D_L15_00161B24") __attribute__((sda));

extern short ultramech_melee_range_gp __asm__("D_L15_00161B28") __attribute__((sda));

extern short ultramech_firing_animation_speed_gp __asm__("D_L15_00161B34") __attribute__((sda));

extern short ultramech_death_effect_basis_scale_gp __asm__("D_L15_00161B38") __attribute__((sda));

extern short ultramech_death_effect_vertical_offset_gp __asm__("D_L15_00161B3C")
    __attribute__((sda));

int D_L15_00161B1C = 180;

extern void
collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");

extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);

extern void make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");

extern void
play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");

extern void FUN_L00_00263ac8(float, int, int, unsigned char *);

extern void emit_radial_particle_burst(void *, void *, int) __asm__("FUN_L03_00250ae8");

extern void process_ultramech_hits_and_update_target(void *) __asm__("FUN_L15_002d0fa8");

extern void noop_main_callback(void *, void *) __asm__("func_001E93E8");

extern char *func_L10_002CD578_v(void *vel, float scale, void *pos, int a, int b, int c) __asm__("FUN_L10_002cc1b8");

void ultramech_unlimited_update(UMoby *m) __asm__("FUN_L15_002cf3a8");

/* Moby class 491 */
void ultramech_unlimited_update(UMoby *ultramech_moby) {
    enum {
        ULTRAMECH_INITIALIZE = 0,
        ULTRAMECH_WAIT_FOR_ACTIVATION = 1,
        ULTRAMECH_FOLLOW_ENTRY_PATH = 2,
        ULTRAMECH_BALLISTIC_PATH_MOVE = 3,
        ULTRAMECH_AIM_AND_SELECT_ATTACK = 4,
        ULTRAMECH_FIRE_BURST = 5,
        ULTRAMECH_WAIT_BETWEEN_BURSTS = 6,
        ULTRAMECH_APPROACH_TARGET = 7,
        ULTRAMECH_MELEE_ATTACK = 8,
        ULTRAMECH_KNOCKBACK_RECOVERY = 9,
        ULTRAMECH_DEATH_EFFECTS = 10,
        ULTRAMECH_DEAD_WAIT_FOR_RESET = 0x40
    };
    Vec4   vector_or_hit_descriptor;
    Vec4   position_or_motion_vector;
    Vec4   vector_arithmetic_result;
    Vec4   hit_direction;
    Vec4   attack_start;
    Vec4   attack_end;
    int    seg;
    float  frac;
    int    moving;
    UData *ultramech_vars;
    UMoby *combat_target;
    int    idx;

    moving = 0;
    ultramech_vars = ultramech_moby->data;
    process_ultramech_hits_and_update_target(ultramech_moby);
    combat_target = ultramech_vars->target;
    if (ultramech_moby->state != 0 && ultramech_moby->state != 0x40 && hero.vehicle_mode != 2) {
        ultramech_moby->w94 = 0;
        ultramech_moby->flags |= 0x41;
        idx = ultramech_vars->snd;
        if (idx != -1) {
            char *e = (char *)((char *)&voice_pool) + idx * 0x70;
            if (*(UMoby **)(e + 0x88) == ultramech_moby) {
                if (*(unsigned char *)(e + 0x74) != 0)
                    release_voice_slot(idx);
            }
        }
        ultramech_vars->snd = -1;
        ultramech_moby->flags &= 0xEFFF;
        ultramech_vars->b58 = 0;
        ultramech_moby->state = 1;
        return;
    }
    FUN_L00_00263ac8(2.5f, (int)ultramech_moby, 6, ultramech_vars->v200);
    switch (ultramech_moby->state) {
    case ULTRAMECH_INITIALIZE:
        ultramech_vars->f20 = 12.0f;
        ultramech_vars->h24 = 0xC;
        ultramech_vars->b28 = 3;
        ultramech_vars->h3E |= 2;
        ultramech_vars->f30 = 6.0f;
        ultramech_vars->b29 = 0;
        {
            int ff = 0xFF;
            ultramech_moby->h32 = ff;
            ultramech_moby->b30 = ff;
        }
        ultramech_vars->snd = -1;
        ultramech_vars->turnVel = 0.0f;
        ultramech_vars->f174 = 90.0f;
        if (rand() & 1)
            ultramech_moby->flags |= 0x8000;
        ultramech_moby->w94 = 0;
        ultramech_moby->flags = (ultramech_moby->flags | 0x41) & 0xEFFF;
        if (ultramech_vars->paths[0] == -1 || ultramech_vars->paths[1] == -1)
            ultramech_vars->w1D4 = 1;
        if (ultramech_vars->path != -1)
            ultramech_moby->pos.q = D_L15_001B0AB0[ultramech_vars->path]->points[0].q;
        ultramech_moby->state = 1;
        if (ultramech_moby->anim != 0)
            blend_moby_animation(ultramech_moby, 0, 0, 5);
        break;
    case ULTRAMECH_WAIT_FOR_ACTIVATION:
        if (ultramech_vars->w1CC != -1 ||
            is_point_inside_clip_volume(&hero.motion.pos, ultramech_vars->w1DC)) {
            if (ultramech_vars->w1CC != -1 &&
                count_active_moby_group_members(ultramech_vars->w1CC, 0x40))
                break;
            if (hero.vehicle_mode == 2) {
                ultramech_moby->w94 = *(int *)(ultramech_moby->cls + 0x10);
                ultramech_moby->flags = (ultramech_moby->flags & 0xFFBE) | 0x1000;
            }
        }
        if (ultramech_vars->w164 != 2 && ultramech_vars->path != -1) {
            struct Hero *pl = &hero;
            if (pl->vehicle_mode == 2 && (ultramech_moby->flags & 0x1000)) {
                if (ultramech_vars->w1D0 != -1 &&
                    !point_in_path_xy(&pl->motion.pos,
                                      D_L15_001B0AB0[ultramech_vars->w1D0]->points,
                                      D_L15_001B0AB0[ultramech_vars->w1D0]->count)) {
                    ultramech_moby->w94 = 0;
                    ultramech_moby->flags = (ultramech_moby->flags | 0x41) & 0xEFFF;
                } else {
                    LevelPath *path;
                    int        i;
                    ultramech_moby->state = 2;
                    if (ultramech_moby->anim != 4)
                        blend_moby_animation(ultramech_moby, 4, 0, scale_game_frames(0x14));
                    path = D_L15_001B0AB0[ultramech_vars->path];
                    for (i = 0; i < path->count - 2; i++) {
                        path->points[i].f[3] =
                            fast_vec_dist(&path->points[i], &path->points[i + 1]);
                    }
                    ultramech_moby->pos.q = path->points[0].q;
                    fast_vec_sub(
                        &ultramech_vars->vel, (Vec4 *)path + path->count, &ultramech_moby->pos);
                    set_vector_length_xy(&ultramech_vars->vel,
                                         &ultramech_vars->vel,
                                         *(float *)&ultramech_path_horizontal_speed_gp *
                                             frame_time);
                    ultramech_vars->vel.f[2] =
                        FUN_L00_0025abf0(&ultramech_moby->pos,
                                         (Vec4 *)path + path->count,
                                         0,
                                         *(float *)&ultramech_path_horizontal_speed_gp * frame_time,
                                         -(*(float *)&ultramech_path_gravity_gp * frame_time_sq));
                }
            }
        }
        break;
    case ULTRAMECH_FOLLOW_ENTRY_PATH: {
        LevelPath *path = D_L15_001B0AB0[ultramech_vars->path];
        float      speed = frame_time * 12.0f;
        float      entry_path_distance_or_fraction;
        float      entry_path_step_length;
        FUN_L00_0025df68(path,
                         &ultramech_moby->pos,
                         &position_or_motion_vector,
                         &seg,
                         &frac,
                         0,
                         999.0f,
                         5.0f,
                         0.0f);
        entry_path_distance_or_fraction = convert_integer_to_float(seg) * path->points[0].f[3];
        entry_path_distance_or_fraction += frac;
        entry_path_distance_or_fraction += 2.0f;
        seg = truncate_float_to_s32(entry_path_distance_or_fraction / path->points[0].f[3]);
        entry_path_distance_or_fraction -= convert_integer_to_float(seg) * path->points[0].f[3];
        entry_path_distance_or_fraction /= path->points[0].f[3];
        frac = entry_path_distance_or_fraction;
        if (seg >= path->count - 1) {
            float z = probe_ground_height(&ultramech_moby->pos, 0, 0.5f);
            if (ultramech_moby->pos.f[2] < z)
                ultramech_moby->pos.f[2] = z;
            ultramech_moby->state = 4;
            ultramech_vars->timer = scale_game_frames(0xF);
            break;
        }
        entry_path_distance_or_fraction =
            fast_vec_dist(&ultramech_moby->pos, (Vec4 *)path + path->count);
        if (entry_path_distance_or_fraction < speed / FUN_L04_001f1978(26.0f) &&
            ultramech_moby->anim != 5) {
            blend_moby_animation(ultramech_moby, 5, 0, scale_game_frames(6));
        }
        fast_vec_sub(
            &position_or_motion_vector, (Vec4 *)path + (seg + 2), (Vec4 *)path + (seg + 1));
        scale_vector_xyz(&position_or_motion_vector, &position_or_motion_vector, frac);
        fast_vec_add(
            &position_or_motion_vector, &position_or_motion_vector, (Vec4 *)path + (seg + 1));
        if (seg + 1 < path->count - 8) {
            float desired_heading =
                fast_arc_tan(path->points[seg + 1].f[0] - ultramech_moby->pos.f[0],
                             path->points[seg + 1].f[1] - ultramech_moby->pos.f[1]);
            ultramech_moby->rot.f[2] = spring_turn(&ultramech_vars->turnVel,
                                                   ultramech_moby->rot.f[2],
                                                   desired_heading,
                                                   0.01f,
                                                   0.3f,
                                                   0.1f);
        }
        fast_vec_sub(&vector_or_hit_descriptor, &position_or_motion_vector, &ultramech_moby->pos);
        entry_path_step_length = fast_vec_length(&vector_or_hit_descriptor);
        if (speed < entry_path_step_length)
            entry_path_step_length = speed;
        moving = 1;
        normalize_vector_xyz(
            &vector_or_hit_descriptor, &vector_or_hit_descriptor, entry_path_step_length);
        fast_vec_add(&ultramech_moby->pos, &ultramech_moby->pos, &vector_or_hit_descriptor);
        break;
    }
    case ULTRAMECH_BALLISTIC_PATH_MOVE:
        if (ultramech_moby->anim == 3 && (ultramech_moby->animFlags & 2))
            blend_moby_animation(ultramech_moby, 4, 0, scale_game_frames(6));
        fast_vec_add(&ultramech_moby->pos, &ultramech_moby->pos, &ultramech_vars->vel);
        ultramech_vars->vel.f[2] -= *(float *)&ultramech_path_gravity_gp * frame_time_sq;
        if (ultramech_vars->vel.f[2] < 0.0f) {
            float z;
            qcopy(&vector_or_hit_descriptor, &ultramech_moby->pos);
            vector_or_hit_descriptor.f[2] += 1.0f;
            z = probe_ground_height(&vector_or_hit_descriptor, 0, 0.5f);
            if (ultramech_moby->pos.f[2] - z <
                    ultramech_vars->vel.f[2] * FUN_L04_001f1978(26.0f) +
                        *(float *)&ultramech_path_gravity_gp * frame_time_sq * 0.5f *
                            FUN_L04_001f1978(26.0f) * FUN_L04_001f1978(26.0f) &&
                ultramech_moby->anim != 5) {
                blend_moby_animation(ultramech_moby, 5, 0, scale_game_frames(6));
            }
            if (ultramech_moby->pos.f[2] < z) {
                ultramech_moby->pos.f[2] = z;
                ultramech_moby->state = 4;
                ultramech_vars->timer = scale_game_frames(0xF);
            }
        }
        moving = 1;
        break;
    case ULTRAMECH_AIM_AND_SELECT_ATTACK:
        if (ultramech_moby->anim != 0 && (ultramech_moby->animFlags & 2))
            blend_moby_animation(ultramech_moby, 0, 0, scale_game_frames(0x14));
        if (ultramech_vars->w164 != 2) {
            approach_angle(&ultramech_moby->rot.f[2],
                           fast_arc_tan(combat_target->pos.f[0] - ultramech_moby->pos.f[0],
                                        combat_target->pos.f[1] - ultramech_moby->pos.f[1]),
                           &ultramech_vars->turnVel,
                           frame_time_sq * 8.726646f,
                           frame_time_sq * 8.726646f,
                           frame_time * 12.566371f);
            if (fast_dec_timer(&ultramech_vars->timer)) {
                if (ultramech_vars->w1D8 == 0 &&
                    vector_distance_xy(&ultramech_moby->pos, &combat_target->pos) <
                        *(float *)&ultramech_approach_target_range_gp) {
                    ultramech_moby->state = 7;
                    if (ultramech_moby->anim != 1)
                        blend_moby_animation(ultramech_moby, 1, 0, scale_game_frames(0xA));
                } else if (ultramech_vars->w1D8 == 0 &&
                           vector_distance_xy(&ultramech_moby->pos, &combat_target->pos) <
                               *(float *)&ultramech_melee_range_gp) {
                    ultramech_moby->state = 8;
                    if (ultramech_moby->anim != 2)
                        blend_moby_animation(ultramech_moby, 2, 0, scale_game_frames(0xA));
                } else {
                    ultramech_moby->state = 5;
                    if (ultramech_moby->anim != 6)
                        blend_moby_animation(ultramech_moby, 6, 0, scale_game_frames(0x14));
                    ultramech_vars->shots = 0;
                }
            }
        }
        break;
    case ULTRAMECH_FIRE_BURST:
        if (ultramech_moby->anim == 6 && (ultramech_moby->animFlags & 2)) {
            blend_moby_animation(ultramech_moby, 7, 0, scale_game_frames(5));
            ultramech_moby->animSpeed = *(float *)&ultramech_firing_animation_speed_gp;
        }
        if (ultramech_vars->w164 != 2) {
            int fire = -1;
            approach_angle(&ultramech_moby->rot.f[2],
                           fast_arc_tan(combat_target->pos.f[0] - ultramech_moby->pos.f[0],
                                        combat_target->pos.f[1] - ultramech_moby->pos.f[1]),
                           &ultramech_vars->turnVel,
                           frame_time_sq * 8.726646f,
                           frame_time_sq * 8.726646f,
                           frame_time * 12.566371f);
            if (ultramech_moby->anim == 7) {
                if (is_value_within_interpolated_window((char *)ultramech_moby, 0.0f))
                    fire = 1;
            }
            if (fire >= 0) {
                moby_point_position(ultramech_moby, 0, &vector_or_hit_descriptor);
                fast_vec_sub(
                    &position_or_motion_vector, &combat_target->pos, &vector_or_hit_descriptor);
                position_or_motion_vector.f[2] += 0.5f;
                normalize_vector_xyz(&position_or_motion_vector,
                                     &position_or_motion_vector,
                                     *(float *)&ultramech_projectile_speed_gp * frame_time);
                if (0.0f < position_or_motion_vector.f[2])
                    position_or_motion_vector.f[2] = 0.0f;
                if (position_or_motion_vector.f[2] < 0.0f)
                    position_or_motion_vector.f[2] = 0.0f;
                func_L10_002CD578_v(&position_or_motion_vector,
                                    3.0f,
                                    &vector_or_hit_descriptor,
                                    (int)ultramech_moby,
                                    (int)combat_target,
                                    scale_game_frames(D_L15_00161B1C));
                normalize_vector_xyz(&position_or_motion_vector, &position_or_motion_vector, 3.0f);
                add_vectors(&vector_arithmetic_result,
                            &vector_or_hit_descriptor,
                            &position_or_motion_vector);
                emit_radial_particle_burst(ultramech_moby, &vector_or_hit_descriptor, 0);
                ultramech_vars->shots++;
            } else if ((float)ultramech_vars->shots >= 6.0f && (ultramech_moby->animFlags & 2)) {
                ultramech_moby->state = 6;
                if (ultramech_moby->anim != 0)
                    blend_moby_animation(ultramech_moby, 0, 0, scale_game_frames(0xA));
                ultramech_vars->timer =
                    truncate_float_to_s32(FUN_L04_001f1978(ultramech_vars->f188 * 60.0f));
            } else if (ultramech_vars->w1D8 == 0 &&
                       vector_distance_xy(&ultramech_moby->pos, &combat_target->pos) <
                           *(float *)&ultramech_approach_target_range_gp) {
                ultramech_moby->state = 7;
                if (ultramech_moby->anim != 1)
                    blend_moby_animation(ultramech_moby, 1, 0, scale_game_frames(0xA));
            } else if (ultramech_vars->w1D8 == 0 &&
                       vector_distance_xy(&ultramech_moby->pos, &combat_target->pos) <
                           *(float *)&ultramech_melee_range_gp) {
                ultramech_moby->state = 8;
                if (ultramech_moby->anim != 2)
                    blend_moby_animation(ultramech_moby, 2, 0, scale_game_frames(0xA));
            } else if (ultramech_vars->w1D8 == 0 &&
                       vector_distance_xy(&ultramech_moby->pos, &combat_target->pos) > 10.0f &&
                       ultramech_vars->w1C8 != 0 && ultramech_vars->w1D4 == 0) {
                ultramech_moby->state = 3;
                ultramech_vars->timer = 0;
                ultramech_vars->side = (ultramech_vars->side + 1) & 1;
                if (ultramech_moby->anim != 3)
                    blend_moby_animation(ultramech_moby, 3, 0, scale_game_frames(0x14));
                fast_vec_sub(&ultramech_vars->vel,
                             (Vec4 *)D_L15_001B0AB0[((UPaths *)((char *)ultramech_vars + 0x180))
                                                        ->idx[ultramech_vars->side]] +
                                 (ultramech_vars->point + 1),
                             &ultramech_moby->pos);
                set_vector_length_xy(&ultramech_vars->vel,
                                     &ultramech_vars->vel,
                                     *(float *)&ultramech_path_horizontal_speed_gp * frame_time);
                ultramech_vars->vel.f[2] = FUN_L00_0025abf0(
                    &ultramech_moby->pos,
                    (Vec4 *)D_L15_001B0AB0[((UPaths *)((char *)ultramech_vars + 0x180))
                                               ->idx[ultramech_vars->side]] +
                        (ultramech_vars->point + 1),
                    0,
                    *(float *)&ultramech_path_horizontal_speed_gp * frame_time,
                    -(*(float *)&ultramech_path_gravity_gp * frame_time_sq));
            }
        } else {
            ultramech_moby->state = 4;
            if (ultramech_moby->anim != 0)
                blend_moby_animation(ultramech_moby, 0, 0, scale_game_frames(0x14));
        }
        break;
    case ULTRAMECH_WAIT_BETWEEN_BURSTS:
        if (ultramech_vars->w164 != 2) {
            approach_angle(&ultramech_moby->rot.f[2],
                           fast_arc_tan(combat_target->pos.f[0] - ultramech_moby->pos.f[0],
                                        combat_target->pos.f[1] - ultramech_moby->pos.f[1]),
                           &ultramech_vars->turnVel,
                           frame_time_sq * 8.726646f,
                           frame_time_sq * 8.726646f,
                           frame_time * 12.566371f);
            if (ultramech_vars->w1D8 == 0 &&
                vector_distance_xy(&ultramech_moby->pos, &combat_target->pos) < 8.0f &&
                ultramech_vars->w1D4 == 0 &&
                ultramech_vars->point < D_L15_001B0AB0[((UPaths *)((char *)ultramech_vars + 0x180))
                                                           ->idx[ultramech_vars->side]]
                                                ->count -
                                            1) {
                ultramech_moby->state = 3;
                ultramech_vars->timer = 0;
                ultramech_vars->point++;
                if (ultramech_moby->anim != 3)
                    blend_moby_animation(ultramech_moby, 3, 0, scale_game_frames(0x14));
                fast_vec_sub(&ultramech_vars->vel,
                             (Vec4 *)D_L15_001B0AB0[((UPaths *)((char *)ultramech_vars + 0x180))
                                                        ->idx[ultramech_vars->side]] +
                                 (ultramech_vars->point + 1),
                             &ultramech_moby->pos);
                set_vector_length_xy(&ultramech_vars->vel,
                                     &ultramech_vars->vel,
                                     *(float *)&ultramech_path_horizontal_speed_gp * frame_time);
                ultramech_vars->vel.f[2] = FUN_L00_0025abf0(
                    &ultramech_moby->pos,
                    (Vec4 *)D_L15_001B0AB0[((UPaths *)((char *)ultramech_vars + 0x180))
                                               ->idx[ultramech_vars->side]] +
                        (ultramech_vars->point + 1),
                    0,
                    *(float *)&ultramech_path_horizontal_speed_gp * frame_time,
                    -(*(float *)&ultramech_path_gravity_gp * frame_time_sq));
            } else if (fast_dec_timer(&ultramech_vars->timer)) {
                ultramech_moby->state = 5;
                if (ultramech_moby->anim != 6)
                    blend_moby_animation(ultramech_moby, 6, 0, scale_game_frames(0x14));
                ultramech_vars->shots = 0;
            }
        } else {
            ultramech_moby->state = 4;
            if (ultramech_moby->anim != 0)
                blend_moby_animation(ultramech_moby, 0, 0, scale_game_frames(0x14));
        }
        break;
    case ULTRAMECH_APPROACH_TARGET: {
        float desired_heading = fast_arc_tan(combat_target->pos.f[0] - ultramech_moby->pos.f[0],
                                             combat_target->pos.f[1] - ultramech_moby->pos.f[1]);
        if (ultramech_vars->w164 != 2) {
            desired_heading = compute_ultramech_flocking_steering_point(
                ultramech_moby, &position_or_motion_vector, ultramech_vars->f1B8);
            if (vector_distance_xy(&ultramech_moby->pos, &combat_target->pos) <
                *(float *)&ultramech_melee_range_gp) {
                ultramech_moby->state = 8;
                if (ultramech_moby->anim != 2)
                    blend_moby_animation(ultramech_moby, 2, 0, scale_game_frames(0xA));
            }
        } else {
            ultramech_moby->state = 4;
            if (ultramech_moby->anim != 0)
                blend_moby_animation(ultramech_moby, 0, 0, scale_game_frames(0x14));
        }
        FUN_L00_00258278((char *)ultramech_moby,
                         &ultramech_vars->turnVel,
                         desired_heading,
                         frame_time_sq * 1.0471976f,
                         frame_time_sq * 3.1415927f,
                         frame_time * 3.1415927f);
        qcopy(&vector_or_hit_descriptor, &ultramech_moby->pos);
        fast_vec_sub(&vector_arithmetic_result, &position_or_motion_vector, &ultramech_moby->pos);
        position_or_motion_vector.q = vector_arithmetic_result.q;
        normalize_vector_xyz(
            &position_or_motion_vector, &position_or_motion_vector, frame_time * 6.5f);
        add_vectors(&vector_arithmetic_result, &position_or_motion_vector, &ultramech_moby->pos);
        if (!FUN_L00_00258490(ultramech_moby,
                              &vector_or_hit_descriptor,
                              &position_or_motion_vector,
                              1,
                              2.25f,
                              1.75f,
                              0.5f,
                              0.5235988f) ||
            point_in_path_xy(&vector_or_hit_descriptor,
                             D_L15_001B0AB0[ultramech_vars->side]->points,
                             D_L15_001B0AB0[ultramech_vars->side]->count)) {
            qcopy(&ultramech_moby->pos, &vector_or_hit_descriptor);
            ultramech_moby->state = 4;
            if (ultramech_moby->anim != 0)
                blend_moby_animation(ultramech_moby, 0, 0, scale_game_frames(0xC));
        } else {
            ultramech_moby->pos.q = vector_or_hit_descriptor.q;
        }
        break;
    }
    case ULTRAMECH_MELEE_ATTACK:
        approach_angle(&ultramech_moby->rot.f[2],
                       fast_arc_tan(combat_target->pos.f[0] - ultramech_moby->pos.f[0],
                                    combat_target->pos.f[1] - ultramech_moby->pos.f[1]),
                       &ultramech_vars->turnVel,
                       frame_time_sq * 8.726646f,
                       frame_time_sq * 8.726646f,
                       frame_time * 12.566371f);
        if (ultramech_moby->animFlags & 2) {
            if (vector_distance_xy(&ultramech_moby->pos, &combat_target->pos) <
                    *(float *)&ultramech_melee_range_gp &&
                ultramech_vars->w1D4 == 0 &&
                ultramech_vars->point < D_L15_001B0AB0[((UPaths *)((char *)ultramech_vars + 0x180))
                                                           ->idx[ultramech_vars->side]]
                                                ->count -
                                            1) {
                ultramech_moby->state = 3;
                ultramech_vars->timer = 0;
                ultramech_vars->point++;
                if (ultramech_moby->anim != 3)
                    blend_moby_animation(ultramech_moby, 3, 0, scale_game_frames(0x14));
                fast_vec_sub(&ultramech_vars->vel,
                             (Vec4 *)D_L15_001B0AB0[((UPaths *)((char *)ultramech_vars + 0x180))
                                                        ->idx[ultramech_vars->side]] +
                                 (ultramech_vars->point + 1),
                             &ultramech_moby->pos);
                set_vector_length_xy(&ultramech_vars->vel,
                                     &ultramech_vars->vel,
                                     *(float *)&ultramech_path_horizontal_speed_gp * frame_time);
                ultramech_vars->vel.f[2] = FUN_L00_0025abf0(
                    &ultramech_moby->pos,
                    (Vec4 *)D_L15_001B0AB0[((UPaths *)((char *)ultramech_vars + 0x180))
                                               ->idx[ultramech_vars->side]] +
                        (ultramech_vars->point + 1),
                    0,
                    *(float *)&ultramech_path_horizontal_speed_gp * frame_time,
                    -(*(float *)&ultramech_path_gravity_gp * frame_time_sq));
            } else if (*(float *)&ultramech_melee_range_gp <
                       vector_distance_xy(&ultramech_moby->pos, &combat_target->pos)) {
                ultramech_moby->state = 4;
                if (ultramech_moby->anim != 0)
                    blend_moby_animation(ultramech_moby, 0, 0, scale_game_frames(0x14));
            }
        } else {
            float melee_window_value = compute_interpolated_record_value(ultramech_moby);
            moby_point_position(ultramech_moby, 1, &attack_end);
            if (melee_window_value >= 17.0f && melee_window_value <= 24.0f) {
                qcopy(&attack_start, &ultramech_moby->pos);
                attack_start.f[2] += 0.5f;
                hit_direction.f[0] = fast_cos(ultramech_moby->rot.f[2]);
                hit_direction.f[1] = fast_sin(ultramech_moby->rot.f[2]);
                hit_direction.f[2] = 1.0f;
                make_hit_descriptor(
                    &vector_or_hit_descriptor, ultramech_moby, 0x10007, &hit_direction, 4.0f);
                collect_mobies_in_sphere(
                    &attack_end, 9, ultramech_moby, &vector_or_hit_descriptor, 2.5f);
                collision_line(
                    &attack_start, &attack_end, 0, ultramech_moby, &vector_or_hit_descriptor);
                sweep_hit_segment(&attack_start,
                                  &attack_end,
                                  &attack_start,
                                  &ultramech_vars->v1A0,
                                  ultramech_moby,
                                  &vector_or_hit_descriptor,
                                  5);
                noop_main_callback(&attack_start, &attack_end);
                qcopy(&ultramech_vars->v1A0, &attack_end);
            }
        }
        break;
    case ULTRAMECH_KNOCKBACK_RECOVERY: {
        int r = 0;
        if (ultramech_vars->w1D8 == 0) {
            r = step_moby_motion(ultramech_moby, ultramech_vars->hit);
        } else {
            ultramech_moby->animSpeed = 1.0f;
        }
        if ((r & 0x40) || (ultramech_vars->w1D8 != 0 && (ultramech_moby->animFlags & 2))) {
            if (ultramech_vars->w1D4 == 0) {
                ultramech_moby->state = 3;
                ultramech_vars->timer = 0;
                ultramech_vars->side = (ultramech_vars->side + 1) & 1;
                qcopy(&vector_or_hit_descriptor,
                      (Vec4 *)D_L15_001B0AB0[((UPaths *)((char *)ultramech_vars + 0x180))
                                                 ->idx[ultramech_vars->side]] +
                          ultramech_vars->point + 1);
                vector_or_hit_descriptor.f[2] =
                    probe_ground_height(&vector_or_hit_descriptor, 0, 0.5f);
                if (ultramech_moby->anim != 3)
                    blend_moby_animation(ultramech_moby, 3, 0, scale_game_frames(0x14));
                fast_vec_sub(&ultramech_vars->vel, &vector_or_hit_descriptor, &ultramech_moby->pos);
                set_vector_length_xy(&ultramech_vars->vel,
                                     &ultramech_vars->vel,
                                     *(float *)&ultramech_path_horizontal_speed_gp * frame_time);
                ultramech_vars->vel.f[2] =
                    FUN_L00_0025abf0(&ultramech_moby->pos,
                                     &vector_or_hit_descriptor,
                                     0,
                                     *(float *)&ultramech_path_horizontal_speed_gp * frame_time,
                                     -(*(float *)&ultramech_path_gravity_gp * frame_time_sq));
            } else {
                ultramech_moby->state = 4;
                if (ultramech_moby->anim != 0)
                    blend_moby_animation(ultramech_moby, 0, 0, scale_game_frames(0x14));
            }
        }
        break;
    }
    case ULTRAMECH_DEATH_EFFECTS: {
        void  *pp = &ultramech_moby->pos;
        void  *rr = &ultramech_moby->rot;
        float *dd;
        scale_vector_xyz(&vector_or_hit_descriptor,
                         ultramech_moby->mtx,
                         *(float *)&ultramech_death_effect_basis_scale_gp *
                             (dd = D_L15_0015F580, frame_scale));
        vector_or_hit_descriptor.f[2] +=
            *(float *)&ultramech_death_effect_vertical_offset_gp * frame_scale;
        FUN_L00_00263fd8(ultramech_moby,
                         0x64A,
                         pp,
                         rr,
                         scale_game_frames(0x5A),
                         0,
                         frame_time_sq * 12.0f,
                         &vector_or_hit_descriptor,
                         dd,
                         dd);
        FUN_L00_00263fd8(ultramech_moby,
                         0x64B,
                         pp,
                         rr,
                         scale_game_frames(0x5A),
                         0,
                         frame_time_sq * 12.0f,
                         &vector_or_hit_descriptor,
                         dd,
                         dd);
        FUN_L00_00263fd8(ultramech_moby, 0x64C, pp, rr, 0, 0, 0.0f, dd, dd, D_L15_0015F580);
    }
        spawn_moby_break_fragments(ultramech_moby, 0x7A9, 1, 0x7A9, 2, 4, 2);
        ultramech_moby->state = 0x40;
        return;
    case ULTRAMECH_DEAD_WAIT_FOR_RESET:
        ultramech_moby->w94 = 0;
        ultramech_moby->flags = (ultramech_moby->flags | 0x41) & 0xEFFF;
        ultramech_vars->b58 = 0;
        D_L15_001BABD0.cleared[(short)ultramech_moby->id] = 0;
        l15_save_state.cleared[(short)ultramech_moby->id] = 0;
        *(int *)(((char *)&D_0014C190) +
                 (((short)ultramech_moby->id >> 5) * 4 + (current_level_index << 8))) &=
            ~(1 << (ultramech_moby->id & 0x1F));
        D_L15_001BAAD0[(short)ultramech_moby->id >> 5] &= ~(1 << (ultramech_moby->id & 0x1F));
        if (hero.vehicle_mode != 2)
            ultramech_moby->state = 0;
        break;
    }
    if (moving) {
        if (voice_is_playing(ultramech_moby, ultramech_vars->snd))
            goto spawn;
        ultramech_vars->snd = allocate_voice_for_target_entry(7, 4, (int)ultramech_moby);
    } else {
        idx = ultramech_vars->snd;
        if (idx != -1) {
            char *e = (char *)((char *)&voice_pool) + idx * 0x70;
            if (*(UMoby **)(e + 0x88) == ultramech_moby) {
                if (*(unsigned char *)(e + 0x74) != 0)
                    release_voice_slot(idx);
            }
        }
        ultramech_vars->snd = -1;
    }
    if (moving) {
        int i;
        int j;
        int n;
    spawn:
        for (i = 0; i < 10; i = n) {
            n = i + 1;
            for (j = 2; j < 6; j++) {
                int k;
                position_or_motion_vector.q = 0;
                position_or_motion_vector.f[0] =
                    random_float_between(1.0f, -1.0f) * (frame_time * 20.0f);
                position_or_motion_vector.f[1] =
                    random_float_between(1.0f, -1.0f) * (frame_time * 20.0f);
                position_or_motion_vector.f[2] =
                    random_float_between(0.8f, 1.2f) * (frame_time * -20.0f);
                vector_or_hit_descriptor.q = position_or_motion_vector.q;
                k = random_integer_below(100) & 1;
                moby_point_position(ultramech_moby, j, &position_or_motion_vector);
                transform_vector_by_basis(
                    &vector_or_hit_descriptor, &vector_or_hit_descriptor, ultramech_moby->mtx);
                spawn_effect_particle_with_target_scalar(
                    &position_or_motion_vector,
                    &vector_or_hit_descriptor,
                    k ? 0x400F0F7F : 0x407F7F7F,
                    0x272727,
                    random_float_between(1.5f, 3.0f) * 210000.0f,
                    scale_game_frames(k ? FUN_L00_00257b90(0xA, 0xF)
                                        : FUN_L00_00257b90(0x14, 0x1E)));
            }
        }
    } else if (ultramech_moby->state > 1 && ultramech_moby->state != 0x40) {
        float z;
        ultramech_vars->fE8 += frame_time_sq * 30.0f;
        ultramech_moby->pos.f[2] -= ultramech_vars->fE8 - 2.0f;
        z = probe_ground_height(&ultramech_moby->pos, 0, 0.5f);
        ultramech_moby->pos.f[2] -= 2.0f;
        if (ultramech_moby->pos.f[2] < z) {
            ultramech_moby->pos.f[2] = z;
            ultramech_vars->fE8 = 0.0f;
        }
        play_entity_sound(
            (int)ultramech_moby, 2.5f, &ultramech_moby->pos, 0x10000, 40.0f, 1.0f, 0, 1, 0);
    }
}

#include "rnc/overlay/l15/mobys/ultramech_unlimited_data/process_ultramech_hits_and_update_target.h"

extern float ultramech_pre_hit_scale_multiplier_gp __asm__("D_L15_00161B2C") __attribute__((sda));

extern float ultramech_post_hit_scale_multiplier_gp __asm__("D_L15_00161B30") __attribute__((sda));

extern float D_0015ED70;

extern int D_001413D0 NOT_SDA;

extern char D_00178700[] __asm__("D_L15_00178700");

extern int
FUN_00249d60(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");

extern void FUN_0024dcf8(void *,
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
                         int,
                         float,
                         float,
                         int,
                         int,
                         int,
                         int) __asm__("FUN_L00_0025e450");

extern void mark_moby_for_removal_cf(void *, int, long long) __asm__("FUN_L00_00257470");

extern void trooper_knockback_arc(void *, void *, void *, void *) __asm__("FUN_L00_0025ab48");

extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");

extern void trooper_set_knockback(void *, float, float) __asm__("FUN_L03_00250a78");

extern void FUN_0024f560(float, void *, void *) __asm__("FUN_L02_00261c80");

extern void
activate_effect_l08(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");

void process_ultramech_hits_and_update_target(void *ultramech_instance) {
    enum {
        HIT_REACTION_NO_ACTION = 0,
        HIT_REACTION_DEATH = 1,
        HIT_REACTION_DEATH_ALTERNATE = 2,
        HIT_REACTION_KNOCKBACK = 3,
        HIT_REACTION_KNOCKBACK_ALTERNATE = 4,
        HIT_REACTION_DIRECTIONAL_KNOCKBACK = 5,
        HIT_REACTION_DIRECTIONAL_KNOCKBACK_ALTERNATE = 6,
        HIT_REACTION_KNOCKBACK_VARIANT = 7,
        HIT_REACTION_KNOCKBACK_VARIANT_ALTERNATE = 8,
        HIT_REACTION_DAMAGE_WITHOUT_KNOCKBACK = 9,
        HIT_REACTION_DAMAGE_WITHOUT_KNOCKBACK_ALTERNATE = 10,
        HIT_REACTION_NONE = 11
    };
    UMoby        *ultramech_moby = ultramech_instance;
    UData        *ultramech_vars = ultramech_moby->data;
    char         *slot = 0;
    int           kind;
    float         impact;
    int           i;
    int           hit;
    int           code;
    float         angle;
    Vec4          origin;
    Vec4          effect;
    float         direction;
    float         reaction_product;
    unsigned char state;

    *(float *)((char *)ultramech_moby + 0x2C) =
        ultramech_pre_hit_scale_multiplier_gp * *(float *)(ultramech_moby->cls + 0x24);
    impact = 0.0f;
    ultramech_vars->w1C8 = 0;
    for (i = 0; i < 64; i++) {
        char *candidate_hit_record = D_00178700 + i * 0x40;
        if (*(UMoby **)(candidate_hit_record + 0x34) == ultramech_moby) {
            unsigned int flags = *(unsigned int *)(candidate_hit_record + 0x24);
            if (flags & 0xA0000) {
                if (*(int *)(candidate_hit_record + 0x20) == D_001413D0) {
                    if (!(flags & 0x80000)) {
                        *(int *)(candidate_hit_record + 0x34) = 0;
                        continue;
                    }
                }
                slot = candidate_hit_record;
                break;
            }
        }
    }
    code = FUN_00249d60(ultramech_moby, slot, &ultramech_vars->f20, 0, &kind, &impact, 0, 4);
    if (slot != 0) {
        if (kind != 1 && (state = ultramech_moby->state) != 10 && state != 0x40 &&
            *(short *)(*(char **)(slot + 0x20) + 0xA6) != 0x350) {
            ultramech_vars->f20 -= impact;
            if (*(unsigned short *)(slot + 0x2A) == 0x100)
                code = 9;
            if (ultramech_moby->state == 2)
                code = 9;
            if (ultramech_vars->f20 <= 0.0f)
                code = 1;
            hit = truncate_float_to_s32(2560.0f);
            {
                float angle;
                angle = ultramech_knockback_gravity_gp * D_0015ED70;
                *(int *)((char *)ultramech_vars + 0x90) = hit;
                *(float *)((char *)ultramech_vars + 0x98) = 2.5f;
                *(float *)((char *)ultramech_vars + 0x80) = angle;
                *(float *)((char *)ultramech_vars + 0x84) = 0.0005f;
                *(int *)((char *)ultramech_vars + 0x94) = 9;
                *(unsigned char *)((char *)ultramech_vars + 0xAD) = 0;
            }
            switch (code) {
            case HIT_REACTION_NO_ACTION:
            case HIT_REACTION_NONE:
                break;
            case HIT_REACTION_DAMAGE_WITHOUT_KNOCKBACK:
            case HIT_REACTION_DAMAGE_WITHOUT_KNOCKBACK_ALTERNATE:
                ultramech_vars->w1C8 = 1;
                *(unsigned char *)((char *)ultramech_vars + 0x67) = 0xFA;
                break;
            case HIT_REACTION_KNOCKBACK:
            case HIT_REACTION_KNOCKBACK_ALTERNATE:
            case HIT_REACTION_DIRECTIONAL_KNOCKBACK:
            case HIT_REACTION_DIRECTIONAL_KNOCKBACK_ALTERNATE:
            case HIT_REACTION_KNOCKBACK_VARIANT:
            case HIT_REACTION_KNOCKBACK_VARIANT_ALTERNATE:
                angle = ultramech_knockback_horizontal_speed_gp * D_0015ED6C;
                reaction_product = ultramech_knockback_vertical_speed_gp * D_0015ED6C;
                ultramech_vars->w1C8 = 1;
                *(float *)((char *)ultramech_vars + 0x88) = angle;
                *(float *)((char *)ultramech_vars + 0x8C) = reaction_product;
                trooper_set_knockback((char *)ultramech_vars + 0x70, 4.0f, 1.0f);
                *(float *)((char *)ultramech_vars + 0xC0) = 12.0f;
                *(float *)((char *)ultramech_vars + 0xC4) = 16.0f;
                angle = fast_arc_tan(
                    ultramech_moby->pos.f[0] - *(float *)(*(char **)(slot + 0x20) + 0x10),
                    ultramech_moby->pos.f[1] - *(float *)(*(char **)(slot + 0x20) + 0x14));
                origin.q = *(u128 *)(slot + 0x10);
                direction = angle;
                trooper_knockback_arc(&origin,
                                      &direction,
                                      (char *)ultramech_vars + 0x88,
                                      (char *)ultramech_vars + 0x8C);
                trooper_start_knockback(
                    direction, ultramech_moby, (char *)ultramech_vars + 0x70, 8, 1, 0);
                trooper_play_sound(0, 0, ultramech_moby, 0x79);
                ultramech_moby->state = 9;
                *(unsigned char *)((char *)ultramech_vars + 0x67) = 0x78;
                break;
            case HIT_REACTION_DEATH:
            case HIT_REACTION_DEATH_ALTERNATE:
                origin.q = 0;
                ultramech_moby->flags &= 0xEFFF;
                moby_point_position(ultramech_moby, 7, &effect);
                hit = -1;
                FUN_0024dcf8(ultramech_moby,
                             &origin,
                             &effect,
                             0.0f,
                             0.0f,
                             0x14,
                             6,
                             0x20,
                             16.0f,
                             8.0f,
                             9.0f,
                             -1,
                             4.0f,
                             32.0f,
                             1,
                             16,
                             hit,
                             0);
                trooper_play_sound(0, 0, ultramech_moby, 0x79);
                ultramech_moby->state = 10;
                *(unsigned char *)((char *)ultramech_vars + 0x67) = 0xF0;
                mark_moby_for_removal_cf(ultramech_moby, 0x200, -1);
                break;
            }
            trooper_anim_update(ultramech_moby, (char *)ultramech_vars + 0x60);
        }
        if (slot != 0)
            *(int *)(slot + 0x34) = 0;
    }
    *(unsigned char *)((char *)ultramech_moby + 0xA4) = 0xFF;
    trooper_anim_finish(ultramech_moby, (char *)ultramech_vars + 0x60);
    *(float *)((char *)ultramech_moby + 0x2C) =
        *(float *)(ultramech_moby->cls + 0x24) * ultramech_post_hit_scale_multiplier_gp;
    ultramech_vars->w164 = 0;
    if (ultramech_moby->state != 1) {
        if (ultramech_vars->w1D0 != -1)
            goto follow_path;
        ultramech_vars->f1B8 = *(float *)((char *)ultramech_vars + 0x178);
    } else {
        ultramech_vars->f1B8 = ultramech_vars->f174;
    }
    FUN_0024f560(ultramech_vars->f1B8, ultramech_moby, (char *)ultramech_vars + 0x120);
    goto check_path;
follow_path: {
    LevelPath *path;
    ultramech_vars->f1B8 = *(float *)((char *)ultramech_vars + 0x178);
    path = D_L15_001B0AB0[ultramech_vars->w1D0];
    activate_effect_l08(ultramech_vars->f1B8,
                        ultramech_moby,
                        (char *)ultramech_vars + 0x120,
                        0,
                        0,
                        &path->points[0],
                        path->count);
}
check_path:
    if (ultramech_vars->f1B8 < fast_vec_dist(&ultramech_moby->pos, (char *)ultramech_vars + 0x120))
        ultramech_vars->w164 = 2;
    if (ultramech_vars->target == 0)
        ultramech_vars->target = (UMoby *)D_001413D0;
}
