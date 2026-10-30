/* Level 13 Blarg space fighter moby class: damage, steering and path following. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/audio/voice_alloc.h"

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

extern char D_0013F350[];

extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");

extern float D_0015ED6C;

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern void func_L00_0025F4A8_alt(void *,
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

extern float D_0015ED60;

extern char D_00140940[];

#include "rnc/overlay/l13/effect_suppression_levels.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern char *D_L13_001B07B0[];

extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");

extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

void process_blarg_space_fighter_damage(struct Moby   *moby,
                                        unsigned char *moby_data,
                                        float         *fp) __asm__("FUN_L13_002c37e0");

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
        int   hit_processing_status;
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
                fast_vec_normalize(hit_position_offset, hit_record + 0x10, 0.1f);
                fast_vec_add(&moby->pos, &moby->pos, hit_position_offset);
                random_vector(hit_position_offset, -0.1f, 0.1f);
                fast_vec_add(&moby->pos, &moby->pos, hit_position_offset);
            }
            moby->hit_slot = 0xFF;
        } else {
            moby->hit_slot = 0xFF;
        }
    }
    trooper_anim_finish(moby, moby_data + 0x60);
}

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);

float absolute_float(float input) __asm__("func_001F99C0");

void steer_blarg_space_fighter(struct Moby *moby,
                               char        *moby_data,
                               float        target_yaw,
                               float        target_pitch) __asm__("FUN_L13_002c3988");

void steer_blarg_space_fighter(struct Moby *moby,
                               char        *moby_data,
                               float        target_yaw,

                               float target_pitch) {
    float roll_attenuation_factor;
    float unclamped_roll;
    FUN_L00_00258278(moby,
                     (float *)(moby_data + 0x88),
                     target_yaw,
                     frame_time_sq * 25.132742f,
                     frame_time_sq * 6.2831855f,
                     frame_time * 25.132742f);
    moby->rot.y = spring_turn((float *)(moby_data + 0x84),
                              moby->rot.y,
                              target_pitch,
                              frame_time_sq * 25.132742f,
                              frame_time_sq * 6.2831855f,
                              frame_time * 25.132742f);
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

int select_blarg_space_fighter_path_point_by_distance(char *pt,
                                                      int  *path_point_table,
                                                      int   start,
                                                      float ref) __asm__("FUN_L13_002c3ac8");

int select_blarg_space_fighter_path_point_by_distance(char *pt,
                                                      int  *path_point_table,
                                                      int   start,

                                                      float ref) {
    int   best = start;
    int   i = 0;
    float bestd = 1e11f;
    while (i < *path_point_table / 2) {
        int   j = (start + i) % *path_point_table;
        float candidate_distance = fast_vec_dist(path_point_table + 4 + j * 4, pt);
        float candidate_score =
            absolute_float(candidate_distance - ref) + convert_integer_to_float(i) * 0.4f;
        if (candidate_score < bestd + convert_integer_to_float(i) * 0.1f) {
            bestd = candidate_distance;
            best = j;
        }
        j = (*path_point_table + start - i) % *path_point_table;
        candidate_distance = fast_vec_dist(path_point_table + 4 + j * 4, pt);
        candidate_score =
            absolute_float(candidate_distance - ref) + convert_integer_to_float(i) * 0.4f;
        if (candidate_score < bestd) {
            bestd = candidate_distance;
            best = j;
        }
        i++;
    }
    return best;
}

void FUN_L13_002c3988_c(char *moby, char *d, float p2, float p3) __asm__("FUN_L13_002c3988");

int advance_blarg_space_fighter_along_path(struct Moby *moby,
                                           char        *moby_data,
                                           int          aim) __asm__("FUN_L13_002c3c88");

int advance_blarg_space_fighter_along_path(struct Moby *moby, char *moby_data, int aim) {
    char *path = D_L13_001B07B0[*(int *)(moby_data + 0xA0)];
    float distance_from_previous_node, seg, segment_distance_shortfall, steering_blend_factor;
    int   wrapped = 0;
    float target_node_steering_vector[4];
    distance_from_previous_node =
        fast_vec_dist(path + (*(short *)(moby_data + 0xB6) * 16 + 0x10), &moby->pos);
    seg = fast_vec_dist(path + (*(short *)(moby_data + 0xB6) * 16 + 0x10),
                        path + (*(short *)(moby_data + 0xB4) * 16 + 0x10));
    segment_distance_shortfall = seg - distance_from_previous_node;
    if ((0.0f <= segment_distance_shortfall &&
         segment_distance_shortfall < *(float *)(moby_data + 0xCC)) ||
        seg < *(float *)(moby_data + 0xCC)) {
        *(short *)(moby_data + 0xB6) = *(unsigned short *)(moby_data + 0xB4);
        *(short *)(moby_data + 0xB4) =
            *(unsigned short *)(moby_data + 0xB4) + ((signed char *)moby_data)[0xD0];
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
    fast_vec_scale(moby_data + 0x70, moby_data + 0x70, 1.0f - steering_blend_factor);
    fast_vec_sub(
        target_node_steering_vector, path + (*(short *)(moby_data + 0xB4) * 16 + 0x10), &moby->pos);
    fast_vec_normalize(
        target_node_steering_vector, target_node_steering_vector, *(float *)(moby_data + 0xCC));
    fast_vec_scale(target_node_steering_vector, target_node_steering_vector, steering_blend_factor);
    fast_vec_add(moby_data + 0x70, moby_data + 0x70, target_node_steering_vector);
    fast_vec_normalize(moby_data + 0x70, moby_data + 0x70, *(float *)(moby_data + 0xCC));
    fast_vec_add(&moby->pos, &moby->pos, moby_data + 0x70);
    if (aim != 0) {
        struct Hero *hero_state = &hero;
        float        yaw = fast_arc_tan(hero_state->motion.pos.f[0] - moby->pos.x,
                                 hero_state->motion.pos.f[1] - moby->pos.y);
        FUN_L13_002c3988_c(moby,
                           moby_data,
                           yaw,
                           -fast_arc_tan(vector_distance_xy(&moby->pos, &hero_state->motion.pos),
                                         hero_state->motion.pos.f[2] - moby->pos.z));
    }
    return wrapped;
}

void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

void predict_hero_vehicle_position(char *out, float scale) __asm__("FUN_L13_002c3f08");

void predict_hero_vehicle_position(char *out, float scale) {
    char *x = D_0013E533 + 0xE1D;
    if (*(char **)(x + 0x15F0) == 0) {
        qcopy(out, x + 0x80);
    } else {
        char *vehicle_data = *(char **)(*(char **)(x + 0x15F0) + 0x78);
        float a = fast_add_rotations(*(float *)(*(char **)(x + 0x15F0) + 0x48),
                                     *(float *)(vehicle_data + 0x90));
        float b = fast_add_rotations(*(float *)(*(char **)(x + 0x15F0) + 0x44),
                                     *(float *)(vehicle_data + 0x94));
        build_spherical_offset(out, scale * *(float *)(vehicle_data + 0x64), a, -b);
        fast_vec_add(out, out, *(char **)(x + 0x15F0) + 0x10);
    }
}

extern char *D_L13_0015FFD8;

extern char D_0013F3D0[];

int update_blarg_space_fighter_path_following(struct Moby *moby,
                                              char        *vars,
                                              int          aim) __asm__("FUN_L13_002c3fc0");

int update_blarg_space_fighter_path_following(struct Moby *moby, char *vars, int aim) {
    char *path = D_L13_001B07B0[*(int *)(vars + 0xA0)];
    char *linked;
    char *veh;
    int   result = 0;
    int   point_count;
    int   forward;
    int   backward;
    int   index;
    int   step;
    int   scans;
    int   direct_distance;
    int   wrap_distance;
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
            fast_vec_sub(point, veh + 0x10, linked + 0x10);
            fast_vec_scale(point, point, 0.5f);
            clamp_vector_length(point, point, 100.0f);
            fast_vec_add(point, point, linked + 0x10);
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
    backward = select_blarg_space_fighter_path_point_by_distance(
        (char *)point, (int *)path, *(short *)(vars + 0xB4), 0.0f);
    forward = backward + 5;
    point_count = *(int *)path;
    if (forward >= point_count) {
        forward -= point_count;
    }
    qcopy(ahead, path + 0x10 + forward * 16);
    backward = backward - 5;
    forward_distance = fast_vec_dist(ahead, D_0013F3D0);
    if (backward < 0) {
        backward += *(int *)path;
    }
    qcopy(behind, path + 0x10 + backward * 16);
    backward_distance = fast_vec_dist(behind, D_0013F3D0);
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
    while (fast_vec_dist(point, path + 0x10 + index * 16) < 30.0f && scans < *(int *)path) {
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
    target_distance = fast_vec_dist(&moby->pos, path + 0x10 + index * 16);
    if (step != *(signed char *)(vars + 0xD0)) {
        if (fast_dec_timer_s16((short *)(vars + 0xD2)) != 0) {
            float speed = D_0015ED60_gp * -0.04000002f + 1.0f;
            *(short *)(vars + 0xD2) = 10;
            *(float *)(vars + 0xCC) = *(float *)(vars + 0xCC) * speed;
            distance = vector_distance_xy(&moby->pos, D_0013F3D0);
            if ((*(float *)(vars + 0xCC) < D_0015ED60 * 0.1f) | (moby->visible == 0) |
                (distance < 40.0f)) {
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

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V4_2C56B0;

typedef float Vec_2C56B0[4] __attribute__((aligned(16)));

typedef struct Cls_2C56B0 {
    char pad0[0x10];
    int  f10;
} Cls_2C56B0;

typedef struct Veh {
    char      pad0[0x10];
    V4_2C56B0 vel;
    char      pad20[0x44];
    float     f64;
} Veh;

typedef struct Boss {
    char          pad0[0x10E];
    unsigned char b10E;
    unsigned char b10F;
} Boss;

typedef struct Moby_2C56B0 {
    char           pad0[0x10];
    V4_2C56B0      pos;
    unsigned char  state;
    char           pad21[3];
    Cls_2C56B0    *cls;
    char           pad28[8];
    unsigned char  b30;
    unsigned char  b31;
    short          h32;
    unsigned short flags;
    char           pad36[0x42];
    void          *data;
    char           pad7C[0x18];
    int            f94;
    char           pad98[0xE];
    short          hA6;
    char           padA8[0xA];
    short          hB2;
    char           padB4[8];
    unsigned char  bBC;
} Moby_2C56B0;

typedef struct Data_2C56B0 {
    char        pad0[0x20];
    float       f20;
    short       h24;
    char        pad26[0x4A];
    V4_2C56B0   vel;
    char        pad80[0x10];
    V4_2C56B0   v90;
    int         fA0;
    int         fA4;
    int         fA8;
    float       fAC;
    float       fB0;
    short       hB4;
    short       hB6;
    int         fB8;
    int         fBC;
    char        padC0[0x10];
    signed char bD0;
    char        padD1[3];
    int         fD4;
} Data_2C56B0;

typedef struct Hero_2C56B0 {
    char         pad0[0x15F0];
    Moby_2C56B0 *veh;
    char         pad15F4[0xA90];
    int          f2084;
} Hero_2C56B0;

typedef struct Snd {
    char          pad0[0x74];
    unsigned char b74;
    char          pad75[0x13];
    void         *f88;
} Snd;

extern char *spawn_blarg_ship_shot(
    int owner, float *dir, float *pos, float size, float len, float z) __asm__("FUN_L11_00308f48");

extern char blarg_space_fighter_missing_path_debug_format[] __asm__("D_L13_001F4700");

extern float D_L13_001670C0[];

extern int update_blarg_space_fighter_path_following();

extern s32 rand(void);

extern unsigned char *spawn_ship_pickup(char *position, int moby_class) __asm__("FUN_L13_002e1c50");

extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");

extern void release_voice_slot(s32) __asm__("FUN_0022d798");

void FUN_L13_002c37e0_c(struct Moby *moby, unsigned char *d, float *fp) __asm__("FUN_L13_002c37e0");

extern char D_0013E550[];

extern int func_001E9730_d(void *, ...) __asm__("FUN_001e93b0");

extern int func_L13_002C5248_56B0(void *, void *, int) __asm__("FUN_L13_002c3fc0");

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
    Veh         *vehd = 0;
    int          close = 0;
    char        *path;
    float        hpos[4];
    float        scratch_vector[4];
    float        spawn_position[4];
    float        vehicle_lead_offset[4];

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
            func_001E9730_d(blarg_space_fighter_missing_path_debug_format, moby->hB2);
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
        fighter_data->fA8 = scale_game_frames(0x14) + 5;
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
        if (fighter_data->fBC == -1 ||
            (p = (Moby_2C56B0 *)(D_L13_0015FFD8 + (fighter_data->fBC << 8))) == 0 ||
            p->state == 0xFE || p->state == 0xFD || p->bBC == 4) {
            fast_vec_normalize(scratch_vector, D_L13_001670C0 + 0x84, 40.0f);
            fast_vec_sub(scratch_vector, D_L13_001670C0, scratch_vector);
            fighter_data->hB6 = fighter_data->hB4 =
                select_point_index_by_distance(scratch_vector, path, 0.0f);
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
        Boss        *bossd;
        float        fighter_to_vehicle_distance;

        FUN_L13_002c37e0_c(
            (unsigned char *)moby, (unsigned char *)fighter_data, &fighter_data->f20);
        if (veh != 0 && veh->state != 0xFE && veh->state != 0xFD) {
            vehd = veh->data;
            qcopy(hpos, &((Hero_2C56B0 *)(((char *)&D_0013F350)))->veh->pos);
            close = 0;
            fighter_to_vehicle_distance = fast_vec_dist(&moby->pos, hpos);
            if (((Hero_2C56B0 *)(((char *)&D_0013F350)))->f2084 == 0x32 &&
                (veh->hA6 == 0x45 || veh->hA6 == 0x4DA)) {
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
            Snd *e;
            if (fighter_data->fD4 == -1 ||
                (e = (Snd *)(((char *)&D_0013E550) + fighter_data->fD4 * 0x70))->f88 != moby ||
                e->b74 == 0) {
                fighter_data->fD4 = -1;
                fighter_data->fD4 = allocate_voice_for_target_entry(1, 4, moby);
            }
        }
        func_L13_002C5248_56B0(moby, fighter_data, 1);
        switch (moby->bBC) {
        case FIGHTER_ATTACK_WAIT_TO_FIRE:
            if (vehd != 0 && veh->state == 4 && moby->b31 != 0 && close != 0 && boss != 0 &&
                bossd->b10F < 3) {
                fighter_data->fB8 = scale_game_frames(0x258);
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
                if (moby->b31 == 0 || close == 0 || scale_game_frames(0x14) <= fighter_data->fA8) {
                    fighter_data->fB8 = scale_game_frames(0x1E);
                    moby->bBC = 2;
                }
                if (fast_dec_timer(&fighter_data->fB8) != 0) {
                    fighter_data->fB8 = scale_game_frames(0x78);
                    moby->bBC = 2;
                }
                if (fast_dec_timer(&fighter_data->fA4) != 0 &&
                    optional_glow_suppression_level < 0.9f &&
                    blarg_space_fighter_fire_suppression_level < 0.9f) {
                    fighter_data->fA4 =
                        scale_game_frames(6) +
                        truncate_float_to_s32((optional_glow_suppression_level +
                                               blarg_space_fighter_fire_suppression_level) *
                                              1.3f);
                    if (vehd != 0) {
                        Vec_2C56B0 up;
                        float      lateral_aim_offset[4];
                        float      lead;

                        *(u128 *)up = 0;
                        up[2] = 1.0f;

                        fast_vec_cross(lateral_aim_offset, &vehd->vel, up);
                        fast_vec_cross(up, lateral_aim_offset, &vehd->vel);
                        fast_vec_normalize(
                            up, up, fighter_data->fAC * 5.0f * fast_sin(fighter_data->fB0));
                        fast_vec_normalize(lateral_aim_offset,
                                           lateral_aim_offset,
                                           fighter_data->fAC * 5.0f * fast_cos(fighter_data->fB0));
                        fast_vec_add(scratch_vector, hpos, up);
                        fast_vec_add(scratch_vector, scratch_vector, lateral_aim_offset);
                        add_random_offset_to_vector_xyz(scratch_vector, 0.6f);
                        lead = vector_distance_xy(&moby->pos, ((char *)&D_0013F3D0));
                        fast_vec_normalize(vehicle_lead_offset,
                                           &vehd->vel,
                                           vehd->f64 * (lead / (vehd->f64 + D_0015ED6C * 50.0f)));
                        fast_vec_add(scratch_vector, scratch_vector, vehicle_lead_offset);
                        fast_vec_sub(scratch_vector, scratch_vector, &moby->pos);
                        fast_vec_normalize(spawn_position, scratch_vector, 2.6f);
                        fast_vec_add(spawn_position, spawn_position, &moby->pos);
                        spawn_blarg_ship_shot((int)moby,
                                              scratch_vector,
                                              spawn_position,
                                              121.0f,
                                              D_0015ED6C * 50.0f,
                                              5.0f);
                        allocate_voice_for_target_entry(0, 0, moby);
                    }
                    fighter_data->fAC =
                        fighter_data->fAC *
                        ((random_float_between(0.92f, 0.94f) - 1.0f) * D_0015ED60 + 1.0f);
                    if (fighter_data->fB0 > 1.5707964f) {
                        fighter_data->fB0 = fighter_data->fB0 - random_float_between(20.0f, 40.0f) *
                                                                    0.017453292f * D_0015ED6C;
                    } else {
                        fighter_data->fB0 = fighter_data->fB0 + random_float_between(20.0f, 40.0f) *
                                                                    0.017453292f * D_0015ED6C;
                    }
                }
            }
            break;
        case FIGHTER_ATTACK_COOLDOWN:
            if (fast_dec_timer(&fighter_data->fB8) != 0) {
                moby->bBC = 0;
            }
            break;
        }
        break;
    }
    case FIGHTER_EMIT_DESTRUCTION_EFFECTS: {
        Moby_2C56B0 *boss;
        Boss        *bossd;

        if (fighter_data->fD4 != -1) {
            Snd *e = (Snd *)(((char *)&D_0013E550) + fighter_data->fD4 * 0x70);
            if (e->f88 == moby && e->b74 != 0) {
                release_voice_slot(fighter_data->fD4);
            }
            fighter_data->fD4 = -1;
        }
        fast_vec_sub(scratch_vector, D_L13_001670C0, &moby->pos);
        func_L00_0025F4A8_alt(moby,
                              scratch_vector,
                              0,
                              0.0f,
                              0.0f,
                              5,
                              3,
                              10,
                              4.0f,
                              3.0f,
                              9.0f,
                              1.1f,
                              -1,
                              20.0f,
                              0,
                              1,
                              -1,
                              0);
        fast_vec_normalize(scratch_vector, scratch_vector, 4.0f);
        fast_vec_sub(spawn_position, &moby->pos, scratch_vector);
        fast_vec_scale(scratch_vector, &fighter_data->vel, 0.5f);
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
        if (fighter_data->fBC != -1 &&
            (p = (Moby_2C56B0 *)(D_L13_0015FFD8 + (fighter_data->fBC << 8))) != 0 &&
            p->state != 0xFE && p->state != 0xFD && p->bBC != 4) {
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

extern void release_voice_slot(int) __asm__("FUN_0022d798");

extern s32 rand();
