/* Level 04 moby classes: big_axebot, circular_saw_enemy, blarg_trooper, eudora_bridge, platform_booster. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/mobys/moby.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/mobys/refresh_moby_spatial_bounds_from_basis.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/mobys/animation/update_moby_animation_state.h"
#include "rnc/math/vectors/normalize_vector_triplet.h"
#include "rnc/overlay/shared/mobys/manipulator_set_scale.h"
#include "rnc/overlay/shared/enemies/trooper_set_knockback.h"
#include "rnc/overlay/shared/mobys/update_object_wander_movement.h"

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002A6C20), where it is exact; names translated to the US level program. */

extern s32 rand();

void update_moby_random_interval_animation(struct Moby *moby) __asm__("FUN_L04_002a5a08");

void update_moby_random_interval_animation(struct Moby *moby) {
    int *animation_countdown;
    if (moby != 0) {
        moby->collision = 0;
        animation_countdown = (int *)moby->pvars;
        if (*animation_countdown == 0) {
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(3));
            }
            *animation_countdown = scale_game_frames((rand() + 0x1E) & 0xFF);
        }
        if ((moby->anim_flags & 2) && moby->prev_seq != 0) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(3));
        }
        *animation_countdown = *animation_countdown - 1;
        refresh_moby_spatial_bounds_from_basis(moby);
    }
}

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB670), where it is exact; names translated to the US level program. */

void blend_big_axebot_and_axe_animation(char *moby,
                                        void *new_seq,
                                        void *frame_index,
                                        void *blend_frames) __asm__("FUN_L04_002ba3e0");

void blend_big_axebot_and_axe_animation(char *moby,
                                        void *new_seq,
                                        void *frame_index,

                                        void *blend_frames) {
    char *data = ((char *)MOBY(moby)->pvars);
    void *other;
    blend_moby_animation(moby, new_seq, frame_index, blend_frames);
    other = *(void **)(data + 0x424);
    if (other != 0) {
        blend_moby_animation(other, new_seq, frame_index, blend_frames);
    }
}
/* Spawns an effect on the moby at its marker vector. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB700), where it is exact; names translated to the US level program. */

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void spawn_explosion(void *,
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

void FUN_L04_002ba470(char *moby) {
    float explosion_position[4];
    float unused_explosion_argument[4];
    clear_u64_value(unused_explosion_argument);
    moby_point_position(moby, 6, explosion_position);
    spawn_explosion(moby,
                    unused_explosion_argument,
                    explosion_position,
                    0.0f,
                    0.0f,
                    10,
                    3,
                    4,
                    2.0f,
                    1.0f,
                    100000.0f,
                    -1,
                    1.5f,
                    15.0f,
                    1,
                    1,
                    -1,
                    0);
}
/* Check that a moby's spot is clear of the other mobys of class 0x154. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C3218), where it is exact; names translated to the US level program. */

extern char *D_L04_0015FFE4;
extern void  select_moby_combat_target(char *, float *, float) __asm__("FUN_L00_0025fcb8");

int check_circular_saw_melee_attack_spacing(struct Moby *moby) __asm__("FUN_L04_002c1e98");

int check_circular_saw_melee_attack_spacing(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float target_selection_output[20];
    float target_distance;
    float target_selection_distance_limit;
    char *other_moby;
    int   flag = 0;
    if (*(int *)(data + 0x38) != 0 || *(short *)(data + 0x136) != 0) {
        flag = 1;
    }
    if (flag) {
        float x = *(float *)(data + 0x140);
        target_selection_distance_limit = x + x;
    } else {
        target_selection_distance_limit = *(float *)(data + 0x140);
    }
    select_moby_combat_target(moby, target_selection_output, target_selection_distance_limit);
    target_distance = vector_distance_xy((float *)&moby->pos, target_selection_output);
    for (other_moby = D_L04_0015FFE4; other_moby != 0;
         other_moby = ((char *)MOBY(other_moby)->next)) {
        if (other_moby != moby && MOBY(other_moby)->oclass == 0x154) {
            float *other_position = (float *)&MOBY(other_moby)->pos;
            if (vector_distance_xy((float *)&moby->pos, other_position) < 3.0f ||
                vector_distance_xy(other_position, target_selection_output) < target_distance) {
                return 0;
            }
        }
    }
    return 1;
}

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C5B88), where it is exact; names translated to the US level program. */

extern void spawn_moby_table_puff_pattern(void *) __asm__("FUN_L04_002c46d0");

void destroy_blarg_trooper(struct Moby *moby) __asm__("FUN_L04_002c4808");

void destroy_blarg_trooper(struct Moby *moby) {
    char *data = ((char *)moby->pvars);
    void *other = *(void **)(data + 0xF8);
    if (other != 0) {
        mark_moby_for_removal(other);
    }
    spawn_moby_table_puff_pattern(moby);
    mark_moby_for_removal(moby);
}

/* Moves a moby along a path by a distance, stepping to the next point when it passes one. */
#include "rnc/math/vector.h"

typedef struct {
    s32  count;
    u8   pad4[0xC];
    Vec4 point[1];
} PathPoints_eb20;

static inline s32 path_next_eb20(PathPoints_eb20 *path, s32 i, s32 dir) {
    if (i == path->count - 1) {
        return dir > 0 ? 0 : path->count - 2;
    }
    if (i == 0) {
        return dir > 0 ? i + 1 : path->count - 1;
    }
    return i + dir;
}

s32 advance_moby_along_path(struct Moby     *moby,
                            PathPoints_eb20 *path,
                            s32              i,
                            s32              path_index_step,
                            f32              advance_distance) __asm__("FUN_L04_0029eb20");

s32 advance_moby_along_path(

    struct Moby *moby, PathPoints_eb20 *path, s32 i, s32 path_index_step, f32 advance_distance) {
    Vec4 seg;
    Vec4 rel;
    s32  next;
    f32  segment_length;
    f32  along;

    next = path_next_eb20(path, i, path_index_step);
    fast_vec_sub(&seg, &path->point[next], &path->point[i]);
    segment_length = fast_vec_length(&seg);
    fast_vec_normalize(&seg, &seg, 1.0f);
    fast_vec_sub(&rel, &moby->pos, &path->point[i]);
    along = fast_vec_dot(&rel, &seg) + advance_distance;
    if (segment_length < along) {
        i = next;
        next = path_next_eb20(path, i, path_index_step);
        fast_vec_sub(&seg, &path->point[next], &path->point[i]);
        along = along - segment_length;
        fast_vec_normalize(&seg, &seg, 1.0f);
    }
    fast_vec_normalize(&seg, &seg, along);
    fast_vec_add(&moby->pos, &seg, &path->point[i]);
    return i;
}

extern void *memset(void *, int, int) __asm__("FUN_001153fc");
extern void
steer_child_moby(void *, s32, struct Moby *, void *, void *) __asm__("FUN_L04_001f30e0");
extern void             update_moby_movement_with_sphere_collision(f32,
                                                                   struct Moby *,
                                                                   void *,
                                                                   void *) __asm__("FUN_L04_001f3a80");
extern PathPoints_eb20 *D_L04_001b0630[];
f32                     D_L04_00166130[8] __attribute__((section(".bss")));

#include "rnc/overlay/l04/mobys/big_axebot_globals.h"
#include "rnc/overlay/level_path.h"

extern s32 swarm_motion_parameters_initialized_gp __asm__("D_L04_001513C0") __attribute__((sda));

/* Variables of a swarm child (class 0x55). */
typedef struct SwarmPartVars {
    u8     pad0[0x10];
    Vec4f  velocity;
    u8     pad20[0x20];
    f32    unk40;
    s16    unk44;
    u8     pad46[2];
    f32    unk48;
    f32    unk4C;
    s32    unk50;
    Vec4f *leader_pos;
    s32    unk58;
    s16    unk5C;
    s16    unk5E;
    s16    oclass;
    s16    unk62;
    u8     pad64[0xC];
    f32    unk70;
    f32    unk74;
    u8     pad78[0x18];
    f32    unk90;
    f32    unk94;
} SwarmPartVars;

typedef struct PVars {
    Vec4f        origin;
    struct Moby *children[32];
    s32          count;
    s32          initialized;
    f32          angle;
    f32          radius;
    s32          path_index;
    s32          direction;
    s32          child_count;
} PVars;

void update_class_55_swarm_controller(struct Moby *moby) __asm__("FUN_L04_0029ecf8");

void update_class_55_swarm_controller(struct Moby *moby) {
    PVars        *data = (PVars *)moby->pvars;
    struct Moby **children;
    struct Moby  *other;
    f32           distance;
    f32           angle;
    f32           angle2;
    f32           speed;
    f32           size;
    f32           average;
    f32           effect[8];

    if (swarm_motion_parameters_initialized_gp == 0) {
        f32 *effect_dest = D_L04_00166130;
        memset(effect, 0, 0x20);
        effect[1] = convert_integer_to_float(data->child_count) * 2.073f;
        effect[3] = 2.0f;
        effect[4] = 1.0f;
        effect[5] = 1.0f;
        swarm_motion_parameters_initialized_gp = 1;
        __builtin_memcpy(effect_dest, effect, 0x20);
    }
    if (data->initialized == 0) {
        qcopy(&data->origin, &moby->pos);
        data->angle = 0.0f;
    } else if (data->child_count == 0) {
        for (other = (struct Moby *)D_L04_0015FFE4; other != 0; other = other->next) {
            if ((s8)other->state >= 0 && other->oclass == 0x55) {
                data->child_count++;
            }
        }
        D_L04_00166130[1] = convert_integer_to_float(data->child_count) * 2.073f;
    }
    if (data->path_index == -1) {
        data->angle = fast_add_rotations(data->angle, frame_scale * 0.0034906587f);
        moby->pos.x = data->origin.x + data->radius * fast_cos(data->angle);
        moby->pos.y = data->origin.y + data->radius * fast_sin(data->angle);
    } else if (data->initialized != 0) {
        s32 i;
        distance = 10000.0f;
        for (i = 0; i < data->count; i++) {
            f32 child_distance = vector_distance_xy(&moby->pos, &data->children[i]->pos);
            if (child_distance < distance)
                distance = child_distance;
        }
        speed = 0.0f;
        if (distance < 3.0f)
            speed = 3.0f - distance;
        data->direction = advance_moby_along_path(
            moby, D_L04_001b0630[data->path_index], data->direction, 1, speed);
    }
    if (data->initialized == 0) {
        s32          i;
        struct Moby *child;
        data->initialized = 1;
        data->child_count = 0;
        for (i = 0; i < data->count; i++) {
            data->children[i] = 0;
            child = create_moby(0x55);
            if (child != 0) {
                SwarmPartVars *part = (SwarmPartVars *)child->pvars;
                data->children[i] = child;
                child->update_dist = 0x7f;
                child->draw_dist = 0x7f;
                child->scale *= random_float_between(0.7f, 1.3f);
                child->rot.z = random_angle_radians();
                child->anim_speed = random_float_between(0.6f, 1.2f);
                clear_u64_value(&part->velocity);
                part->unk44 = 0;
                part->unk4C = 10.0f;
                qcopy(&child->pos, &moby->pos);
                angle = random_angle_radians();
                angle2 = random_angle_radians();
                size = random_float_between(0.6f, 0.9f);
                child->pos.x += fast_cos(angle) * fast_sin(angle2) * size;
                child->pos.y += fast_sin(angle) * fast_sin(angle2) * size;
                child->pos.z += fast_cos(angle2) * size;
                part->unk50 = 0;
                part->leader_pos = &moby->pos;
                part->unk58 = 0;
                part->unk70 = 1.694f;
                part->unk74 = 1.694f;
                part->unk48 = 1.056f;
                part->unk5C = -1;
                part->unk40 = 0.003f;
                part->unk5E = 2;
                part->oclass = 0x55;
                part->unk90 = 1.0f;
                part->unk62 = 0x1ee;
                part->unk94 = 5.0f;
                update_entity(child);
            }
        }
    } else {
        s32          i;
        struct Moby *child;
        for (i = 0; i < data->count; i++) {
            void *part;
            children = data->children;
            child = children[i];
            part = child->pvars;
            D_L04_00166130[6] = ((data->radius * 7.753f) / 1800.0f) * frame_scale *
                                ((child->anim_speed + 1.0f) * 0.5f) * 0.05f * frame_scale_sq *
                                ((child->anim_speed + 1.0f) * 0.5f);
            D_L04_00166130[7] = (((data->radius * 7.753f) / 1800.0f) * frame_scale) *
                                ((child->anim_speed + 1.0f) * 0.5f);
            steer_child_moby(children, data->count, child, part, D_L04_00166130);
            update_moby_movement_with_sphere_collision(0.0f, child, part, D_L04_00166130);
        }
    }
}
/* Sets the float at 0x58 on the moby and on the linked moby at data+0x424, if any. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB6D8), where it is exact; names translated to the US level program. */

void set_big_axebot_and_axe_animation_speed(struct Moby *moby,
                                            float animation_speed) __asm__("FUN_L04_002ba448");

void set_big_axebot_and_axe_animation_speed(struct Moby *moby, float animation_speed) {
    char *data = ((char *)moby->pvars);
    moby->anim_speed = animation_speed;
    if (*(char **)(data + 0x424) != 0) {
        *(float *)(*(char **)(data + 0x424) + 0x58) = animation_speed;
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Update of the big axebot: hit reaction, state machine (idle, alert, path, chase, return home, swing, stagger, turn, death), knockback, gravity, and its axe moby. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB7B0), where it is exact; names translated to the US level program. */

typedef float V4_B7B0[4] __attribute__((aligned(16)));

typedef struct Manip_B7B0 {
    unsigned char id;
    unsigned char on;
    char          pad02[0xE];
    V4_B7B0       q;
    char          pad20[0x20];
} Manip_B7B0;

typedef struct D_B7B0 {
    char           pad00[0x20];
    float          hp;
    short          f24;
    char           pad26[2];
    unsigned char  f28;
    unsigned char  f29;
    char           pad2A[4];
    unsigned char  f2E;
    char           pad2F;
    float          f30;
    char           pad34[4];
    int            f38;
    char           pad3C[0x1C];
    unsigned char  f58;
    char           pad59;
    unsigned char  f5A;
    char           pad5B[0x55];
    char           fB0[7];
    unsigned char  fB7;
    char           padB8[8];
    float          fC0[4];
    float          fD0;
    float          fD4;
    float          fD8;
    float          fDC;
    int            fE0;
    int            fE4;
    float          fE8;
    char           padEC[0x11];
    unsigned char  fFD;
    char           padFE[0x12];
    float          f110;
    float          f114;
    char           pad118[8];
    Manip_B7B0     m120;
    Manip_B7B0     m160;
    char           f1A0[0x250];
    V4_B7B0        home;
    unsigned char  f400;
    unsigned char  f401;
    short          f402;
    short          f404;
    unsigned char  f406;
    unsigned char  f407;
    float          f408;
    char           pad40C[4];
    float          f410;
    float          f414;
    float          f418;
    int            f41C;
    float          f420;
    struct M_B7B0 *f424;
    float          f428;
    float          f42C;
    int            f430;
    int            f434;
    short          f438;
    short          f43A;
    float          f43C;
    char           pad440[8];
    int            f448;
} D_B7B0;

typedef struct M_B7B0 {
    char           pad00[0x10];
    V4_B7B0        pos;
    unsigned char  state;
    char           pad21[0x10];
    unsigned char  f31;
    char           pad32[2];
    unsigned short f34;
    char           pad36[0xA];
    V4_B7B0        rot;
    char           pad50[2];
    unsigned char  f52;
    unsigned char  f53;
    char           pad54[4];
    float          f58;
    char           pad5C[0x14];
    unsigned char  f70;
    char           pad71[7];
    D_B7B0        *data;
    char           pad7C[3];
    char           f7F;
    char           pad80[0x24];
    unsigned char  fA4;
    char           padA5;
    short          cls;
    char           padA8[0x14];
    unsigned char  fBC;
    char           padBD[3];
    V4_B7B0        fC0;
} M_B7B0;

typedef struct {
    char    pad00[0x80];
    V4_B7B0 pos;
    char    pad90[0x1000];
    M_B7B0 *f1090;
} H_B7B0;

typedef struct {
    V4_B7B0 pos;
    char    pad10[0x30];
    char   *f40;
    char    pad44[0xC];
} T_B7B0;

typedef struct {
    char    pad00[0x10];
    V4_B7B0 pos;
    M_B7B0 *att;
    char    pad24[8];
    float   f2C;
} Hit_B7B0;

typedef struct {
    int     count;
    int     pad[3];
    V4_B7B0 pts[1];
} P_B7B0;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float D_L04_0015F580[] MACRO_ADDR;
extern float D_L04_00166F40[4];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float spring_turn(float cur, float target, float *vel, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int   D_L04_0015F5C4;
extern int   FUN_L00_001fefc8(void *);
extern int   is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int FUN_L04_00295de8_cf(void *, void *, void *, int, float, float, float, float) __asm__("FUN_L00_00258490");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int allocate_voice_for_target_entry(int, int, char *);
extern int mark_moby_for_removal_cf() __asm__("FUN_L04_002ba470");
extern void *
FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
extern void *spawn_moby_attachment(void *, void *) __asm__("FUN_L04_002ce080");
extern void *spawn_class_23a_moby(void *, void *) __asm__("FUN_L04_002d34d8");
extern void *random_integer_below_cf(void *, int, unsigned int) __asm__("FUN_001153fc");
extern void  FUN_L00_001fff28(void *, int, float);
extern void  FUN_L00_00257470(void *, int, int);
extern void  leg_walker_update(void *, void *, float, float, float) __asm__("FUN_L04_00295778");
extern void  FUN_L00_0025a120(void *);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void reset_leg_rig_from_animation(void *, void *) __asm__("FUN_L04_00295de8");
extern void advance_moby_animation(void *);
extern void build_moby_bone_transform(char *, int, char *);
extern void spawn_nearby_effect(char *m, float t, int a, int b, void *pos, void *vel) __asm__("FUN_L00_00259bc8");
extern void copy_matrix3x4(void *, void *);
extern void refresh_moby_spatial_bounds_from_basis(void *);
void        blend_big_axebot_and_axe_animation(char *arg,
                                               void *a,
                                               void *b,
                                               void *c) __asm__("FUN_L04_002ba3e0");

void big_axebot_update(M_B7B0 *moby) __asm__("FUN_L04_002ba520");

void big_axebot_update(M_B7B0 *moby) {
    enum {
        BIG_AXEBOT_STATE_INITIALIZE = 0,
        BIG_AXEBOT_STATE_IDLE = 1,
        BIG_AXEBOT_STATE_ALERT = 2,
        BIG_AXEBOT_STATE_WAIT_FOR_TARGET = 3,
        BIG_AXEBOT_STATE_FOLLOW_PATH = 4,
        BIG_AXEBOT_STATE_CHASE_TARGET = 5,
        BIG_AXEBOT_STATE_RETURN_HOME = 6,
        BIG_AXEBOT_STATE_MELEE_ATTACK = 7,
        BIG_AXEBOT_STATE_HIT_RECOVERY = 8,
        BIG_AXEBOT_STATE_TURN = 9,
        BIG_AXEBOT_STATE_DYING = 10
    };
    enum {
        BIG_AXEBOT_HIT_REACTION_NONE = 0,
        BIG_AXEBOT_HIT_REACTION_FATAL = 1,
        BIG_AXEBOT_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE = 2,
        BIG_AXEBOT_HIT_REACTION_CONDITIONAL_HIT_RECOVERY = 3,
        BIG_AXEBOT_HIT_REACTION_RETURN_TO_IDLE = 4,
        BIG_AXEBOT_HIT_REACTION_IDLE_RECOVERY = 5,
        BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOIL_RECOVERY = 6,
        BIG_AXEBOT_HIT_REACTION_KNOCKBACK = 7,
        BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOVERY = 8,
        BIG_AXEBOT_HIT_REACTION_NO_ADDITIONAL_RESPONSE = 9,
        BIG_AXEBOT_HIT_REACTION_NO_FURTHER_RESPONSE = 10,
        BIG_AXEBOT_HIT_REACTION_IGNORED = 11
    };
    D_B7B0 *data = moby->data;
    T_B7B0  tg;
    V4_B7B0 initial_position_snapshot;
    V4_B7B0 dest;
    V4_B7B0 tpos;
    V4_B7B0 vector_quaternion_scratch;
    int     flag = 0;
    float   rot;
    char   *target_moby;
    float   target_distance;
    float   target_yaw;
    float   hd;

    if (data->f38 != 0 || data->f406 != 0) {
        flag = 1;
    }
    FUN_L00_00263ac8(2.5f, (int)moby, 0, (unsigned char *)&data->m120);
    if (moby->f31 != 0) {
        if (fast_vec_dist(moby->pos, D_L04_00166F40) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby->f7F = 0x15;
        }
    }
    select_moby_combat_target((char *)moby, (float *)&tg, flag ? data->f43C * 2.0f : data->f43C);
    target_moby = tg.f40;
    rot = moby->rot[2];
    target_distance = vector_distance_xy(moby->pos, (float *)&tg);
    target_yaw = fast_arc_tan(tg.pos[0] - moby->pos[0], tg.pos[1] - moby->pos[1]);
    hd = compute_interpolated_record_value(moby);
    if (D_L04_0015F5C4 == 2) {
        moby->f34 |= 0x41;
        if (data->f424 != 0) {
            data->f424->f34 |= 0x41;
        }
    } else if (moby->f34 & 1) {
        moby->f34 &= ~0x41;
        if (data->f424 != 0) {
            data->f424->f34 &= ~0x41;
        }
    }
    qcopy(initial_position_snapshot, moby->pos);
    qcopy(dest, moby->pos);
    qcopy(tpos, tg.pos);
    if (moby->f31 != 0) {
        if (fast_vec_dist(moby->pos, D_L04_00166F40) < 32.0f) {
            FUN_L00_0025a120(moby);
            moby->f7F = 0x1A;
        }
    }
    fast_dec_timer_s16(&data->f404);
    if (moby->state != 10 && moby->state != 0) {
        int       unused_hit_reaction_output;
        float     dmg;
        Hit_B7B0 *hit;
        int       hit_reaction_result;
        float     knockback_yaw;

        dmg = 0.0f;
        hit = (Hit_B7B0 *)find_moby_hit(moby, 0x210000, 0);
        hit_reaction_result =
            hit_reaction(moby, hit, &data->hp, 0, &unused_hit_reaction_output, &dmg, 0, 4);
        if (hit != 0 && dmg != 0.0f && moby->state != 0) {
            if (hit->att != 0) {
                struct Hero *h = &hero;
                if (hit->att != h->items[0].moby) {
                    knockback_yaw = fast_arc_tan(moby->pos[0] - hit->att->pos[0],
                                                 moby->pos[1] - hit->att->pos[1]);
                } else {
                    knockback_yaw = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                                 moby->pos[1] - h->motion.pos.f[1]);
                }
            } else {
                struct Hero *h = &hero;
                knockback_yaw = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                             moby->pos[1] - h->motion.pos.f[1]);
            }
            data->fD4 = frame_time_sq * 12.0f;
            data->fD0 = frame_time_sq * 35.0f;
            data->fD8 = frame_time * 7.0f;
            data->fE4 = 9;
            data->fFD = 0;
            data->hp -= dmg;
            if (data->hp <= 0.0f) {
                hit_reaction_result = 1;
            }
            if (data->f404 != 0 && hit_reaction_result != 1) {
                if (data->f448 <= 0) {
                    data->f408 = -0.5235988f;
                }
                data->f448 = scale_game_frames(0x1E);
                data->fB7 = 0x78;
                trooper_anim_update(moby, (short *)data->fB0);
            } else {
                switch (hit_reaction_result) {
                case BIG_AXEBOT_HIT_REACTION_NONE:
                    break;
                case BIG_AXEBOT_HIT_REACTION_CONDITIONAL_HIT_RECOVERY:
                case BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOIL_RECOVERY:
                case BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOVERY:
                    allocate_voice_for_target_entry(0xB, 0, (char *)moby);
                    if (data->f448 <= 0) {
                        data->f408 = -0.5235988f;
                    }
                    data->f448 = scale_game_frames(0x1E);
                    if ((hit->att->cls == 0 || hit->f2C > 1.0f) &&
                        (moby->f52 != 4 || hd < 17.0f || hd > 20.0f)) {
                        moby->fBC |= 5;
                        blend_big_axebot_and_axe_animation(
                            (char *)moby, (void *)5, 0, (void *)scale_game_frames(6));
                        set_big_axebot_and_axe_animation_speed(moby, 2.0f);
                        moby->state = 8;
                    }
                    data->f42C = 3.4f / scale_time(7.0f) * 0.5f;
                    data->fB7 = 0x78;
                    trooper_anim_update(moby, (short *)data->fB0);
                    data->f404 = scale_game_frames(0x46);
                    data->f406 = scale_game_frames(0xF0);
                    break;
                case BIG_AXEBOT_HIT_REACTION_KNOCKBACK:
                    allocate_voice_for_target_entry(0xB, 0, (char *)moby);
                    if (data->f448 <= 0) {
                        data->f408 = -0.5235988f;
                    }
                    data->f448 = scale_game_frames(0x1E);
                    moby->fBC |= 5;
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)5, 0, (void *)scale_game_frames(6));
                    set_big_axebot_and_axe_animation_speed(moby, 2.0f);
                    moby->state = 8;
                    trooper_start_knockback(knockback_yaw, moby, data->fC0, 5, 1, 0);
                    data->f110 = 5.0f;
                    data->f114 = 10.0f;
                    data->f42C = 3.4f / scale_time(7.0f);
                    data->fB7 = 0x78;
                    trooper_anim_update(moby, (short *)data->fB0);
                    data->f404 = scale_game_frames(0x46);
                    data->f406 = scale_game_frames(0xF0);
                    break;
                case BIG_AXEBOT_HIT_REACTION_RETURN_TO_IDLE:
                case BIG_AXEBOT_HIT_REACTION_IDLE_RECOVERY:
                    allocate_voice_for_target_entry(0xB, 0, (char *)moby);
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)6, (void *)2, (void *)scale_game_frames(4));
                    data->f402 = scale_game_frames(0x28);
                    moby->state = 1;
                    data->f42C = 3.4f / scale_time(7.0f);
                    data->fB7 = 0x78;
                    trooper_anim_update(moby, (short *)data->fB0);
                    data->f404 = scale_game_frames(0x46);
                    data->f406 = scale_game_frames(0xF0);
                    break;
                case BIG_AXEBOT_HIT_REACTION_FATAL:
                    data->fE8 = 1.0f;
                    data->fE0 = 0x400;
                    trooper_set_knockback(data->fC0, 5.7f, 4.0f);
                    *(OvlQuad *)vector_quaternion_scratch = *(OvlQuad *)hit->pos;
                    trooper_knockback_arc(
                        vector_quaternion_scratch, &knockback_yaw, &data->fD8, &data->fDC);
                    trooper_start_knockback(knockback_yaw, moby, data->fC0, 7, 1, 0);
                    data->f110 = 12.0f;
                    data->f114 = 25.0f;
                    moby->state = 10;
                    data->fB7 = 0x78;
                    trooper_anim_update(moby, (short *)data->fB0);
                    FUN_L00_00257470(moby, 0, -1);
                    break;
                case BIG_AXEBOT_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE:
                case BIG_AXEBOT_HIT_REACTION_NO_ADDITIONAL_RESPONSE:
                case BIG_AXEBOT_HIT_REACTION_NO_FURTHER_RESPONSE:
                case BIG_AXEBOT_HIT_REACTION_IGNORED:
                    break;
                }
            }
        }
    }
    moby->fA4 = 0xFF;
    if (data->f38 != 0) {
        data->f406 = scale_game_frames(0xF0);
    }
    data->f38 = 0;
    FUN_L00_001fefc8(&data->f406);
    if (moby->state != 0 && data->f2E != 1) {
        if (data->f424 != 0) {
            mark_moby_for_removal(data->f424);
        }
        mark_moby_for_removal(moby);
        return;
    }
    fast_dec_timer_s16(&data->f402);
    switch (moby->state) {
    case BIG_AXEBOT_STATE_INITIALIZE:
        data->hp = 3.0f;
        data->f24 = 3;
        data->f28 = 2;
        data->f2E = 1;
        data->f29 = 0;
        data->f30 = 1.5f;
        moby->fBC = 0;
        data->f420 = 0.0f;
        data->f424 = 0;
        data->f407 = 0;
        qcopy(data->home, moby->pos);
        if (data->m120.on == 0) {
            attach_manipulator((char *)moby, 0, &data->m120);
        }
        attach_manipulator((char *)moby, 0xC, &data->m160);
        data->f428 = moby->rot[2];
        data->f58 = 0xA;
        data->f5A = 0xC;
        if (data->f400 != 0) {
            moby->state = 3;
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)0xA, 0, (void *)scale_game_frames(0xC));
        } else {
            moby->state = 1;
        }
        break;
    case BIG_AXEBOT_STATE_IDLE: {
        if (moby->f53 == 6 && data->f402 == scale_game_frames(0x25)) {
            allocate_voice_for_target_entry(2, 0, (char *)moby);
        }
        if ((moby->f70 & 2) &&
            (flag ? data->f43C * 2.0f < target_distance : data->f43C < target_distance)) {
            int arr[6];

            random_integer_below_cf(arr, 0, 0x18);
            arr[5] = 1;
            blend_big_axebot_and_axe_animation((char *)moby,
                                               (void *)arr[random_integer_below(6)],
                                               0,
                                               (void *)scale_game_frames(0x18));
        } else if (moby->f53 != 0xD && (moby->f53 != 6 || (moby->f70 & 2))) {
            if (flag ? target_distance < data->f43C * 2.0f : target_distance < data->f43C) {
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)0xD, 0, (void *)scale_game_frames(0x18));
            }
        }
        if (data->f430 != -1 && data->f434 != -1 && data->f43A == 0 &&
            (flag || is_point_inside_clip_volume(&hero.motion.pos, data->f430) != 0)) {
            moby->state = 4;
            moby->fBC |= 1;
            data->f43A = 1;
        } else {
            if (moby->f52 == 0xD && FUN_L00_001fefc8(&data->f407) != 0) {
                allocate_voice_for_target_entry(0xA, 0, (char *)moby);
                data->f407 = scale_game_frames(0xB4);
            }
            if (data->f402 == 0) {
                if (target_distance < 4.5f) {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
                    moby->state = 7;
                } else if (flag ? target_distance < data->f43C * 2.0f
                                : target_distance < data->f43C) {
                    int nf = (moby->fBC & 1) == 0;
                    if (nf) {
                        blend_big_axebot_and_axe_animation(
                            (char *)moby, (void *)2, 0, (void *)scale_game_frames(0xC));
                        moby->state = 2;
                        moby->fBC |= 1;
                    }
                }
            }
        }
        break;
    }
    case BIG_AXEBOT_STATE_ALERT:
        if (moby->f52 == 2 && (moby->f70 & 2)) {
            blend_big_axebot_and_axe_animation(
                (char *)moby,
                (void *)((flag ? target_distance < data->f43C * 2.0f : target_distance < data->f43C)
                             ? 0xD
                             : 0),
                0,
                (void *)scale_game_frames(0xC));
            moby->state = 1;
        }
        break;
    case BIG_AXEBOT_STATE_WAIT_FOR_TARGET:
        if (moby->f52 != 0 && (moby->f70 & 2)) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)0xB, 0, (void *)scale_game_frames(0xC));
        }
        if (data->f402 == 0) {
            if (target_distance < 4.5f) {
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
                moby->state = 7;
            } else if (flag ? target_distance < data->f43C * 2.0f : target_distance < data->f43C) {
                int nf = (moby->fBC & 1) == 0;
                if (nf) {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)2, 0, (void *)scale_game_frames(0xC));
                    moby->state = 2;
                    moby->fBC |= 1;
                }
            }
        }
        break;
    case BIG_AXEBOT_STATE_FOLLOW_PATH: {
        P_B7B0 *path = ((P_B7B0 *)l04_paths[data->f434]);
        float   a;

        if (moby->f53 != 3) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)3, 0, (void *)scale_game_frames(0xC));
        }
        if (target_distance < 4.5f) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
            moby->state = 7;
        }
        if (vector_distance_xy(path->pts[data->f438], moby->pos) < 1.0f) {
            data->f438++;
            if (data->f438 == path->count) {
                if (flag ? data->f43C * 2.0f < target_distance : data->f43C < target_distance) {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, 0, 0, (void *)scale_game_frames(0x18));
                } else {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)0xD, 0, (void *)scale_game_frames(0x18));
                }
                moby->state = 1;
                break;
            }
        }
        a = fast_arc_tan(path->pts[data->f438][0] - moby->pos[0],
                         path->pts[data->f438][1] - moby->pos[1]);
        moby->rot[2] = spring_turn(moby->rot[2],
                                   a,
                                   &data->f410,
                                   frame_time_sq * 12.566371f,
                                   frame_time_sq * 6.2831855f,
                                   frame_time * 3.1415927f);
        vector_quaternion_scratch[0] = fast_cos(moby->rot[2]) * (frame_time * 6.0f);
        vector_quaternion_scratch[1] = fast_sin(moby->rot[2]) * (frame_time * 6.0f);
        vector_quaternion_scratch[2] = 0.0f;
        fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
        FUN_L04_00295de8_cf(
            moby, moby->pos, vector_quaternion_scratch, 1, 0.25f, 1.5f, 0.75f, 0.87266463f);
        break;
    }
    case BIG_AXEBOT_STATE_CHASE_TARGET:
        if (moby->fBC & 8) {
            moby->fBC &= ~8;
            reset_leg_rig_from_animation(moby, data->f1A0);
        }
        data->f414 = spring_turn(moby->rot[2],
                                 target_yaw,
                                 &data->f410,
                                 frame_time_sq * 6.2831855f,
                                 frame_time_sq * 125.663704f,
                                 frame_time * 12.566371f);
        leg_walker_update(moby, data->f1A0, frame_time * 4.0f, data->f414, frame_time_sq * 20.0f);
        if (target_distance < 4.5f) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
            moby->state = 7;
        } else if (data->f41C != -1) {
            vector_quaternion_scratch[0] = fast_cos(moby->rot[2]) * 2.0f;
            vector_quaternion_scratch[1] = fast_sin(moby->rot[2]) * 2.0f;
            vector_quaternion_scratch[2] = 0.0f;
            fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
            if (point_in_path_xy(vector_quaternion_scratch,
                                 ((P_B7B0 *)l04_paths[data->f41C])->pts,
                                 ((P_B7B0 *)l04_paths[data->f41C])->count) == 0) {
                data->f410 = 0.0f;
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)8, 0, (void *)scale_game_frames(0xC));
                moby->state = 9;
            }
            if (point_in_path_xy(tpos,
                                 ((P_B7B0 *)l04_paths[data->f41C])->pts,
                                 ((P_B7B0 *)l04_paths[data->f41C])->count) == 0) {
                moby->state = 6;
            }
        }
        break;
    case BIG_AXEBOT_STATE_RETURN_HOME:
        if (target_distance < 4.5f) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
            moby->state = 7;
        } else {
            float a;

            if (moby->fBC & 8) {
                moby->fBC &= ~8;
                reset_leg_rig_from_animation(moby, data->f1A0);
            }
            a = fast_arc_tan(data->home[0] - moby->pos[0], data->home[1] - moby->pos[1]);
            data->f414 = spring_turn(moby->rot[2],
                                     a,
                                     &data->f410,
                                     frame_time_sq * 6.2831855f,
                                     frame_time_sq * 125.663704f,
                                     frame_time * 12.566371f);
            leg_walker_update(
                moby, data->f1A0, frame_time * 4.0f, data->f414, frame_time_sq * 20.0f);
            if (vector_distance_xy(moby->pos, data->home) < 2.0f) {
                moby->state = 1;
            }
            if (data->f41C != -1) {
                vector_quaternion_scratch[0] = fast_cos(moby->rot[2]) * 2.0f;
                vector_quaternion_scratch[1] = fast_sin(moby->rot[2]) * 2.0f;
                vector_quaternion_scratch[2] = 0.0f;
                fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
                if (point_in_path_xy(vector_quaternion_scratch,
                                     ((P_B7B0 *)l04_paths[data->f41C])->pts,
                                     ((P_B7B0 *)l04_paths[data->f41C])->count) == 0) {
                    data->f410 = 0.0f;
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)8, 0, (void *)scale_game_frames(0xC));
                    moby->state = 9;
                    moby->fBC |= 0x10;
                }
                if (point_in_path_xy(tpos,
                                     ((P_B7B0 *)l04_paths[data->f41C])->pts,
                                     ((P_B7B0 *)l04_paths[data->f41C])->count) != 0) {
                    moby->state = 1;
                }
            }
        }
        break;
    case BIG_AXEBOT_STATE_MELEE_ATTACK:
        if (moby->f52 == moby->f53 && compute_interpolated_record_value(moby) >= 17.0f &&
            compute_interpolated_record_value(moby) < 20.0f &&
            absolute_float(moby->pos[2] - tg.pos[2]) < 2.0f &&
            vector_distance_xy(moby->pos, (float *)&tg) < 4.5f) {
            float p[4];

            vector_quaternion_scratch[0] = fast_cos(target_yaw) * 0.2f;
            vector_quaternion_scratch[1] = fast_sin(target_yaw) * 0.2f;
            vector_quaternion_scratch[2] = 0.0f;
            qcopy(p, tg.pos);
            p[2] += 0.75f;
            if (target_moby != 0) {
                spawn_nearby_effect(target_moby, 1.0f, (int)moby, 1, p, vector_quaternion_scratch);
            }
        }
        if (hd > 20.0f) {
            if (moby->f58 > 1.0f) {
                float animation_speed = moby->f58 - frame_time * 3.0f;

                if (animation_speed < 1.0f) {
                    animation_speed = 1.0f;
                }
                set_big_axebot_and_axe_animation_speed(moby, animation_speed);
            }
        }
        if (moby->f52 == 4 && (moby->f70 & 2)) {
            data->f402 = scale_game_frames(0x5A);
            blend_big_axebot_and_axe_animation(
                (char *)moby,
                (void *)((flag ? target_distance < data->f43C * 2.0f : target_distance < data->f43C)
                             ? 0xD
                             : 0),
                0,
                (void *)scale_game_frames(0xC));
            moby->state = 1;
        }
        break;
    case BIG_AXEBOT_STATE_HIT_RECOVERY:
        if (!(moby->f53 == 5 && (moby->f52 != moby->f53 || hd <= 13.0f))) {
            if (moby->fBC & 4) {
                moby->fBC &= ~4;
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(6));
                set_big_axebot_and_axe_animation_speed(moby, 1.5f);
                moby->state = 7;
            } else if (moby->f53 == 8 && (hd < 28.0f || moby->f52 != moby->f53)) {
                float hit_recovery_displacement_limit = frame_scale * 0.11111111f;

                if (data->f418 < hit_recovery_displacement_limit) {
                    data->f418 += frame_time_sq * 20.0f;
                } else {
                    data->f418 = hit_recovery_displacement_limit;
                }
                build_spherical_offset(
                    vector_quaternion_scratch, data->f418, data->f414, moby->rot[1]);
                fast_vec_add(moby->pos, moby->pos, vector_quaternion_scratch);
                moby->rot[2] = spring_turn(moby->rot[2],
                                           target_yaw,
                                           &data->f410,
                                           frame_time_sq * 3.1415927f,
                                           frame_time_sq * 6.2831855f,
                                           frame_time * 4.1887903f);
            } else if (data->f418 > 0.0f) {
                data->f418 -= frame_time_sq * 200.0f;
                build_spherical_offset(
                    vector_quaternion_scratch, data->f418, data->f414, moby->rot[1]);
                fast_vec_add(moby->pos, moby->pos, vector_quaternion_scratch);
                moby->rot[2] = spring_turn(moby->rot[2],
                                           target_yaw,
                                           &data->f410,
                                           frame_time_sq * 3.1415927f,
                                           frame_time_sq * 6.2831855f,
                                           frame_time * 4.1887903f);
            } else {
                moby->state = 1;
                data->f402 = 0;
            }
        }
        break;
    case BIG_AXEBOT_STATE_TURN:
        if (moby->f52 == 8 && (moby->f70 & 2)) {
            moby->state = 5;
            moby->fBC = (moby->fBC | 8) & ~0x10;
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)9, 0, (void *)scale_game_frames(0xC));
        } else if (moby->fBC & 0x10) {
            float a = fast_arc_tan(data->home[0] - moby->pos[0], data->home[1] - moby->pos[1]);

            moby->rot[2] = spring_turn(moby->rot[2],
                                       a,
                                       &data->f410,
                                       frame_time_sq * 3.1415927f,
                                       frame_time_sq * 6.2831855f,
                                       frame_time * 4.1887903f);
        } else {
            moby->rot[2] = spring_turn(moby->rot[2],
                                       target_yaw,
                                       &data->f410,
                                       frame_time_sq * 3.1415927f,
                                       frame_time_sq * 6.2831855f,
                                       frame_time * 4.1887903f);
        }
        break;
    case BIG_AXEBOT_STATE_DYING:
        if ((mine_fall(moby, data->fC0) & 0x40) || moby->pos[2] < 1.0f) {
            mark_moby_for_removal_cf((char *)moby);
            if (data->f424 != 0) {
                mark_moby_for_removal(data->f424);
            }
            FUN_L00_00257470(moby, 0, -1);
            FUN_L00_00263fd8((char *)moby,
                             0x798,
                             moby->pos,
                             moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L04_0015F580,
                             D_L04_0015F580,
                             D_L04_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x799,
                             moby->pos,
                             moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L04_0015F580,
                             D_L04_0015F580,
                             D_L04_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x79A,
                             moby->pos,
                             moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L04_0015F580,
                             D_L04_0015F580,
                             D_L04_0015F580);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, data->fB0);
    if (data->f42C > 0.0f) {
        struct Hero *h = &hero;

        vector_quaternion_scratch[0] = fast_cos(fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                                             moby->pos[1] - h->motion.pos.f[1])) *
                                       data->f42C;
        vector_quaternion_scratch[1] = fast_sin(fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                                             moby->pos[1] - h->motion.pos.f[1])) *
                                       data->f42C;
        vector_quaternion_scratch[2] = 0.0f;
        fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
        if (data->f420 == 0.0f) {
            data->f42C += -3.4f / (scale_time(7.0f) * scale_time(7.0f));
        }
        FUN_L04_00295de8_cf(
            moby, moby->pos, vector_quaternion_scratch, 1, 0.25f, 1.5f, 0.75f, 0.87266463f);
    }
    data->f408 *= 0.96f;
    fast_dec_timer(&data->f448);
    FUN_L00_001fff28(vector_quaternion_scratch, 1, data->f408);
    FUN_L00_001fff28(data->m120.q, 2, 0.0f);
    multiply_quaternions(data->m120.q, data->m120.q, vector_quaternion_scratch);
    if (moby->state != 8) {
        data->f401 = moby->state;
    }
    data->f420 += frame_time_sq * 29.8f;
    moby->pos[2] -= data->f420 - 2.0f;
    {
        float ground_height = probe_ground_height(moby->pos, 0, 0.5f);

        moby->pos[2] -= 2.0f;
        if (moby->pos[2] < ground_height) {
            moby->pos[2] = ground_height;
            data->f420 = 0.0f;
        }
    }
    if (moby->pos[2] < data->home[2] - 8.0f) {
        mark_moby_for_removal_cf((char *)moby);
        if (data->f424 != 0) {
            mark_moby_for_removal(data->f424);
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (moby->state != 0) {
        float q[4];
        float bone_transform[4][4];
        float bone_euler_angles[4];
        {
            data->f428 = fast_add_rotations(data->f428, fast_subtract_rotations(moby->rot[2], rot));
            build_moby_bone_transform((char *)moby, 0xC, (char *)bone_transform);
            matrix_to_euler(bone_transform, bone_euler_angles);
            FUN_L00_001fff28(q, 2, fast_subtract_rotations(target_yaw, data->f428));
            data->f428 = target_yaw;
            multiply_quaternions(data->m160.q, data->m160.q, q);
        }
    }
    if (data->f424 != 0) {
        float axe_bone_transform[4][4];
        {
            M_B7B0 *axe;

            build_moby_bone_transform((char *)moby, 0xD, (char *)axe_bone_transform);
            axe = data->f424;
            qcopy(axe->pos, axe_bone_transform[3]);
            advance_moby_animation(axe);
            update_entity(data->f424);
            copy_matrix3x4(data->f424->fC0, axe_bone_transform);
            normalize_vector_triplet(data->f424->fC0);
            refresh_moby_spatial_bounds_from_basis(data->f424);
            data->f424->f34 |= 6;
        }
    } else {
        float p[4];

        moby_point_position(moby, 0xB, p);
        data->f424 = spawn_class_23a_moby(moby, p);
    }
}

/* The engine-sound fields of the moby's pvars. */
typedef struct {
    /* 0x000 */ u8  pad0[0x230];
    /* 0x230 */ f32 volume;
    /* 0x234 */ f32 pitch;
    /* 0x238 */ s32 voice;
} EngineSoundVars;

extern s32  voice_is_playing(long moby, s32 voice) __asm__("FUN_L00_0028d8c0");
extern s32  engine_voice_start(s32, s32, struct Moby *m) __asm__("FUN_0022da68");
extern void release_voice_slot(s32 voice) __asm__("FUN_0022d798");
extern f32  approach_value(f32 *value, f32 target, f32 step) __asm__("FUN_00213ed8");
extern void vendor_volume(s32 voice, s32 volume) __asm__("FUN_L01_002a1968");
extern void engine_voice_set_pitch(s32 voice, s32 pitch) __asm__("FUN_L00_0028df38");

/* Keeps the moby's engine voice playing, easing its volume and pitch toward the animation's levels. */
void update_circular_saw_engine_sound(struct Moby *moby) __asm__("FUN_L04_002c1b80");

void update_circular_saw_engine_sound(struct Moby *moby) {
    enum {
        CIRCULAR_SAW_ANIM_IDLE = 0,
        CIRCULAR_SAW_ANIM_MOVE_FORWARD = 1,
        CIRCULAR_SAW_ANIM_MOVE_BACKWARD = 2,
        CIRCULAR_SAW_ANIM_STRAFE_NEGATIVE_YAW = 3,
        CIRCULAR_SAW_ANIM_STRAFE_POSITIVE_YAW = 4,
        CIRCULAR_SAW_ANIM_JUMP = 5,
        CIRCULAR_SAW_ANIM_ALERT = 6,
        CIRCULAR_SAW_ANIM_ATTACK = 7
    };
    EngineSoundVars *engine_sound_vars = (EngineSoundVars *)moby->pvars;
    f32              volume;
    f32              pitch;

    if (moby->state != 9) {
        if (voice_is_playing((long)moby, engine_sound_vars->voice) == 0 && moby->prev_seq < 8) {
            engine_sound_vars->voice = engine_voice_start(0, 4, moby);
            engine_sound_vars->volume = convert_integer_to_float(0x400);
            engine_sound_vars->pitch = 0;
        }
        switch (moby->prev_seq) {
        case CIRCULAR_SAW_ANIM_IDLE:
            pitch = 0.0f;
            volume = convert_integer_to_float(0x400) * 0.6f;
            break;
        case CIRCULAR_SAW_ANIM_MOVE_FORWARD:
            pitch = 3.0f;
            volume = convert_integer_to_float(0x400);
            break;
        case CIRCULAR_SAW_ANIM_MOVE_BACKWARD:
            pitch = -2.0f;
            volume = convert_integer_to_float(0x400);
            break;
        case CIRCULAR_SAW_ANIM_STRAFE_NEGATIVE_YAW:
        case CIRCULAR_SAW_ANIM_STRAFE_POSITIVE_YAW:
            pitch = 4.0f;
            volume = convert_integer_to_float(0x400);
            break;
        case CIRCULAR_SAW_ANIM_JUMP:
        case CIRCULAR_SAW_ANIM_ALERT:
        case CIRCULAR_SAW_ANIM_ATTACK:
            pitch = 0.0f;
            volume = convert_integer_to_float(0x400) * 0.8f;
            break;
        default:
            goto release;
        }
    } else {
    release:
        if (engine_sound_vars->voice != -1) {
            VoicePoolWindow *voice_slot_window =
                (VoicePoolWindow *)((u8 *)&voice_pool + engine_sound_vars->voice * 0x70);
            if (voice_slot_window->voice.owner == (VoiceMoby *)moby &&
                voice_slot_window->voice.state != 0)
                release_voice_slot(engine_sound_vars->voice);
        }
        engine_sound_vars->voice = -1;
        return;
    }
    approach_value(
        &engine_sound_vars->volume, volume, convert_integer_to_float(0x400) * frame_time * 2.0f);
    approach_value(&engine_sound_vars->pitch, pitch, frame_time * 8.0f);
    vendor_volume(engine_sound_vars->voice, truncate_float_to_s32(engine_sound_vars->volume));
    engine_voice_set_pitch(engine_sound_vars->voice,
                           truncate_float_to_s32(engine_sound_vars->pitch));
}

#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C30F0), where it is exact; names translated to the US level program. */

extern unsigned char D_0013E550_u[] __asm__("D_0013E550");
extern void          FUN_0022da68(int, int, char *);
extern void          func_L00_0025F4A8_alt(void *,
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
extern void          release_voice_slot(s32) __asm__("FUN_0022d798");

void spawn_circular_saw_destruction_effects(struct Moby *moby) __asm__("FUN_L04_002c1d70");

void spawn_circular_saw_destruction_effects(struct Moby *moby) {
    float explosion_position[4];
    float unused_destruction_effect_argument[4];
    char *data = (char *)moby->pvars;
    int   engine_voice_index;
    int   invalid_voice_index;

    clear_u64_value(unused_destruction_effect_argument);
    engine_voice_index = *(int *)(data + 0x238);
    if (engine_voice_index != -1) {
        char *voice_slot_window = (char *)D_0013E550_u + engine_voice_index * 0x70;
        if (*(char **)(voice_slot_window + 0x88) == moby) {
            if (*(unsigned char *)(voice_slot_window + 0x74) != 0)
                release_voice_slot(engine_voice_index);
        }
    }
    invalid_voice_index = -1;
    *(int *)(data + 0x238) = invalid_voice_index;
    FUN_L00_00257470(moby, 0, -1);
    moby_point_position(moby, 5, explosion_position);
    func_L00_0025F4A8_alt(moby,
                          unused_destruction_effect_argument,
                          explosion_position,
                          0.0f,
                          0.0f,
                          10,
                          3,
                          4,
                          2.0f,
                          1.0f,
                          100000.0f,
                          1.0f,
                          -1,
                          15.0f,
                          1,
                          1,
                          invalid_voice_index,
                          0);
    FUN_0022da68(1, 0, moby);
}
/* Flock steering for moby m: picks its path target, pushes away from same-class mobys within 3 units,
 * keeps its spacing to the target unless another flock member is closer to it, writes the steer point
 * to out and returns the yaw toward it. (The passed speed is recomputed from the moby's state.) */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C3338), where it is exact; names translated to the US level program. */

extern float circular_saw_flock_separation_weight_gp __asm__("D_L04_00161884") __attribute__((sda));
float        compute_circular_saw_flock_steering(struct Moby *moby,
                                                 void        *out,
                                                 float        speed) __asm__("FUN_L04_002c1fb8");

float compute_circular_saw_flock_steering(struct Moby *moby, void *out, float speed) {
    float away[4];
    float target[4];
    float pad[4][4];
    char *data = (char *)moby->pvars;
    char *other;
    float target_dist;
    float weight;
    int   is_closest;
    int   is_alert = 0;
    if (*(int *)(data + 0x38) != 0 || *(short *)(data + 0x136) != 0)
        is_alert = 1;
    if (is_alert) {
        speed = *(float *)(data + 0x140) + *(float *)(data + 0x140);
    } else {
        speed = *(float *)(data + 0x140);
    }
    select_moby_combat_target(moby, target, speed);
    target_dist = vector_distance_xy((float *)&moby->pos, target);
    clear_u64_value(out);
    is_closest = 1;
    other = D_L04_0015FFE4;
    weight = 0.0f;
    for (; other != 0; other = *(char **)(other + 0x28)) {
        if (other == moby || *(short *)(other + 0xA6) != moby->oclass)
            continue;
        if (vector_distance_xy((float *)(other + 0x10), (float *)&moby->pos) < 3.0f) {
            fast_vec_sub(away, &moby->pos, other + 0x10);
            fast_vec_scale(away, away, circular_saw_flock_separation_weight_gp);
            fast_vec_add(out, out, away);
            weight += circular_saw_flock_separation_weight_gp;
        }
        if (vector_distance_xy((float *)(other + 0x10), target) < target_dist)
            is_closest = 0;
    }
    if (!is_closest) {
        float offset_dir;
        fast_vec_sub(away, &moby->pos, target);
        if (target_dist < 4.5f) {
            offset_dir = 5.0f;
        } else if (5.5f < target_dist) {
            offset_dir = -5.0f;
        } else {
            offset_dir = 0.0f;
        }
        fast_vec_normalize(away, away, offset_dir * 10.0f);
        weight += 10.0f;
        fast_vec_add(out, out, away);
        fast_vec_scale(out, out, 1.0f / weight);
        fast_vec_add(out, out, &moby->pos);
    } else {
        qcopy(out, target);
    }
    if (vector_distance_xy((float *)&moby->pos, (float *)out) < 1.0f) {
        qcopy(out, &moby->pos);
        return fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
    }
    return fast_arc_tan(((float *)out)[0] - moby->pos.x, ((float *)out)[1] - moby->pos.y);
}

#include "qcopy.h"

/* Update of the circular saw enemy: hit reaction, state machine (idle, chase, patrol, attack), movement and gravity. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C35F0), where it is exact; names translated to the US level program. */

typedef float V4_35F0[4] __attribute__((aligned(16)));

typedef struct D_35F0 {
    char           pad00[0x20];
    float          hp;
    short          f24;
    char           pad26[2];
    unsigned char  f28;
    unsigned char  f29;
    char           pad2A[4];
    unsigned char  f2E;
    char           pad2F;
    float          f30;
    char           pad34[4];
    int            f38;
    char           pad3C[0x1C];
    unsigned char  f58;
    char           pad59;
    unsigned char  f5A;
    char           pad5B[5];
    int            f60;
    float          f64;
    float          f68;
    float          f6C;
    char           pad70[8];
    float          f78;
    char           pad7C[8];
    float          f84;
    char           pad88[0x10];
    int            f98;
    char           pad9C[0x14];
    char           fB0[7];
    unsigned char  fB7;
    char           padB8[8];
    char           fC0[0x10];
    float          fD0;
    float          fD4;
    float          fD8;
    char           padDC[8];
    int            fE4;
    char           padE8[0x15];
    unsigned char  fFD;
    char           padFE[0x22];
    V4_35F0        f120;
    float          f130;
    char           pad134[2];
    short          f136;
    int            f138;
    float          f13C;
    float          f140;
    short          f144;
    short          f146;
    char           pad148[4];
    unsigned char  f14C;
    unsigned char  f14D;
    unsigned short f14E;
    int            f150;
    int            f154;
    short          f158;
    short          f15A;
    char           pad15C[4];
    int            f160[4];
    char           pad170[0x78];
    short          f1E8;
    char           pad1EA[6];
    short         *f1F0;
    char           pad1F4[0x44];
    int            f238;
    char           pad23C[4];
    V4_35F0        f240;
    char           pad250[4];
    float          f254;
    float          f258;
    float          f25C;
    char           pad260[0x10];
    unsigned char  f270[4];
} D_35F0;

typedef struct M_35F0 {
    char          pad00[0x10];
    V4_35F0       pos;
    unsigned char state;
    char          pad21[0xF];
    unsigned char f30;
    unsigned char f31;
    char          pad32[0x16];
    float         rot;
    char          pad4C[6];
    unsigned char f52;
    unsigned char f53;
    char          pad54[0x1C];
    unsigned char f70;
    char          pad71[7];
    D_35F0       *data;
    char          pad7C[3];
    char          f7F;
    char          pad80[0x24];
    unsigned char fA4;
    char          padA5;
    union {
        short          s;
        unsigned short u;
    } cls;
    char    padA8[0x18];
    V4_35F0 fC0;
} M_35F0;

typedef struct {
    char    pad00[0x80];
    V4_35F0 pos;
    char    pad90[0x1B0];
    M_35F0 *f240;
    char    pad244[0xE4C];
    M_35F0 *f1090;
} H_35F0;

typedef struct {
    V4_35F0 pos;
    char    pad10[0x30];
    int     f40;
    char    pad44[0xC];
} T_35F0;

typedef struct {
    V4_35F0 v;
    M_35F0 *moby;
    int     f14;
    int     f18;
    float   f1C;
    int     f20;
    char    pad24[0xC];
} S_35F0;

typedef struct {
    char    pad00[0x20];
    M_35F0 *att;
    char    pad24[8];
    float   f2C;
} Hit_35F0;

typedef struct {
    int     count;
    int     pad[3];
    V4_35F0 pts[1];
} P_35F0;

typedef struct {
    int v[4];
} I4_35F0;

#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/collision.h"

extern CollisionHit coll_hit __asm__("D_L04_00173FC0");

I4_35F0 D_L04_001618A0 = {
    {1, 4, 2, 3}
};

I4_35F0 D_L04_00161890 = {
    {1, 2, 3, 4}
};

extern float fast_cos(float);
extern float fast_sin(float);
extern int collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L00_00258490(void *, void *, void *, int, float, float, float, float);
extern int FUN_L00_002dc9f0(void *, void *);
short      D_L04_00161878 = 0x101;
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L01_0026d930(char *p);
extern void update_circular_saw_engine_sound(struct Moby *) __asm__("FUN_L04_002c1b80");
extern void release_voice_slot(int);
extern void transform_vector_by_basis(void *, void *, void *);
extern char D_0013E550[];
extern void func_L00_002592B0_b(
    char *moby, float target, float *vel, float k, float d, float max) __asm__("FUN_L00_00258278");

void circular_saw_enemy_update(M_35F0 *moby) __asm__("FUN_L04_002c2270");

void circular_saw_enemy_update(M_35F0 *moby) {
    enum {
        CIRCULAR_SAW_STATE_INITIALIZE = 0,
        CIRCULAR_SAW_STATE_IDLE = 1,
        CIRCULAR_SAW_STATE_JUMP = 2,
        CIRCULAR_SAW_STATE_CHASE_TARGET = 3,
        CIRCULAR_SAW_STATE_FOLLOW_PATH = 4,
        CIRCULAR_SAW_STATE_POINT_ATTACK = 5,
        CIRCULAR_SAW_STATE_HIT_RECOVERY = 6,
        CIRCULAR_SAW_STATE_MELEE_ATTACK = 7,
        CIRCULAR_SAW_STATE_DYING = 8
    };
    enum {
        CIRCULAR_SAW_HIT_REACTION_NONE = 0,
        CIRCULAR_SAW_HIT_REACTION_FATAL = 1,
        CIRCULAR_SAW_HIT_REACTION_DESTROY_IMMEDIATELY = 2,
        CIRCULAR_SAW_HIT_REACTION_START_HIT_RECOVERY = 3,
        CIRCULAR_SAW_HIT_REACTION_KNOCKBACK_RECOVERY = 4,
        CIRCULAR_SAW_HIT_REACTION_BACKWARD_RECOIL = 5,
        CIRCULAR_SAW_HIT_REACTION_RECOIL_RECOVERY = 6,
        CIRCULAR_SAW_HIT_REACTION_IGNORED = 7,
        CIRCULAR_SAW_HIT_REACTION_NO_ADDITIONAL_REACTION = 8,
        CIRCULAR_SAW_HIT_REACTION_CONTINUE_WITHOUT_RESPONSE = 9,
        CIRCULAR_SAW_HIT_REACTION_NO_FURTHER_RESPONSE = 10,
        CIRCULAR_SAW_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE = 11
    };
    enum {
        CIRCULAR_SAW_POINT_ATTACK_APPROACH_AND_ALIGN = 0,
        CIRCULAR_SAW_POINT_ATTACK_ATTACK = 1,
        CIRCULAR_SAW_POINT_ATTACK_TURN_TO_PATH = 2
    };
    D_35F0 *data = moby->data;
    T_35F0  tg;
    V4_35F0 initial_position_snapshot;
    V4_35F0 dest;
    int     flag = 0;
    float   rot = moby->rot;
    int     target_moby;
    float   hd;
    float   target_distance;
    float   ta;

    if (data->f38 != 0 || data->f136 != 0) {
        flag = 1;
    }
    FUN_L00_00263ac8(2.9f, (int)moby, 7, data->f270);
    if (moby->f31 != 0) {
        if (fast_vec_dist(moby->pos, D_L04_00166F40) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby->f7F = 0x15;
        }
    }
    select_moby_combat_target((char *)moby, (float *)&tg, flag ? data->f140 * 2.0f : data->f140);
    target_moby = tg.f40;
    hd = compute_interpolated_record_value(moby);
    target_distance = vector_distance_xy(moby->pos, (float *)&tg);
    qcopy(initial_position_snapshot, moby->pos);
    qcopy(dest, moby->pos);
    moby->f30 = 0x20;
    update_circular_saw_engine_sound((struct Moby *)moby);
    if (moby->state == 1 || moby->state == 3) {
        H_35F0 *h = (H_35F0 *)(((char *)((char *)&hero)));
        if (h->f240 == moby || fast_vec_dist(moby->pos, h->pos) < 1.5f) {
            blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
            data->f146 = scale_game_frames(0x3C);
            moby->state = 7;
        }
    }
    FUN_L00_001fefc8(&data->f14D);
    if (moby->state != 8 && moby->state != 0) {
        int       unused_hit_reaction_output;
        float     dmg;
        Hit_35F0 *hit;
        int       r;
        float     ang;

        dmg = 0.0f;
        hit = find_moby_hit(moby, 0x210000, 0);
        r = hit_reaction(moby, hit, &data->hp, 0, &unused_hit_reaction_output, &dmg, 0, 4);
        if (hit != 0 && moby->state != 0 && data->f14D == 0) {
            if (hit->att != 0) {
                struct Hero *h = &hero;
                if (hit->att != h->items[0].moby) {
                    ang = fast_arc_tan(moby->pos[0] - hit->att->pos[0],
                                       moby->pos[1] - hit->att->pos[1]);
                } else {
                    ang = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                       moby->pos[1] - h->motion.pos.f[1]);
                }
            } else {
                struct Hero *h = &hero;
                ang = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                   moby->pos[1] - h->motion.pos.f[1]);
            }
            data->fD0 = 0.008f;
            data->fD4 = 0.0005f;
            data->fD8 = frame_time * 4.0f;
            data->fE4 = 9;
            data->fFD = 0;
            if ((unsigned int)(hit->att->cls.u - 0xB0) < 2) {
                hit->f2C = 1.0f;
                dmg = 1.0f;
            }
            data->hp -= dmg;
            if (data->hp <= 0.0f) {
                r = 1;
            }
            data->f136 = scale_game_frames(0xF0);
            switch (r) {
            case CIRCULAR_SAW_HIT_REACTION_NONE:
                break;
            case CIRCULAR_SAW_HIT_REACTION_FATAL:
            case CIRCULAR_SAW_HIT_REACTION_DESTROY_IMMEDIATELY:
                spawn_circular_saw_destruction_effects((char *)moby);
                mark_moby_for_removal(moby);
                return;
            case CIRCULAR_SAW_HIT_REACTION_START_HIT_RECOVERY:
            case CIRCULAR_SAW_HIT_REACTION_KNOCKBACK_RECOVERY:
            case CIRCULAR_SAW_HIT_REACTION_BACKWARD_RECOIL:
            case CIRCULAR_SAW_HIT_REACTION_RECOIL_RECOVERY:
                blend_moby_animation(moby, (void *)8, 0, (void *)scale_game_frames(5));
                if (hit->att->cls.s == 0x131 || hit->att->cls.s == 0xB0) {
                    data->f13C = 10.0f / scale_time(15.0f) * 0.5f;
                } else {
                    data->f13C = 10.0f / scale_time(15.0f);
                }
                moby->state = 6;
                moby->rot = fast_add_rotations(3.1415927f, ang);
                data->fB7 = 0x78;
                trooper_anim_update(moby, (short *)data->fB0);
                data->f14D = scale_game_frames(0x14);
                data->fB7 = 0x78;
                trooper_anim_update(moby, (short *)data->fB0);
                data->f14D = scale_game_frames(0x14);
                break;
            case CIRCULAR_SAW_HIT_REACTION_IGNORED:
            case CIRCULAR_SAW_HIT_REACTION_NO_ADDITIONAL_REACTION:
            case CIRCULAR_SAW_HIT_REACTION_CONTINUE_WITHOUT_RESPONSE:
            case CIRCULAR_SAW_HIT_REACTION_NO_FURTHER_RESPONSE:
            case CIRCULAR_SAW_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE:
                break;
            }
        }
    }
    moby->fA4 = 0xFF;
    if (data->f38 != 0) {
        data->f136 = scale_game_frames(0xF0);
    }
    data->f38 = 0;
    fast_dec_timer_s16(&data->f136);
    if (moby->state == 7 && moby->f52 == 7 && hd > 15.0f && hd < 44.0f) {
        int i;
        for (i = 0; i < 4; i++) {
            S_35F0  melee_hit_parameters;
            V4_35F0 jp;
            I4_35F0 joints = D_L04_00161890;
            {
                moby_point_position(moby, joints.v[i], jp);
                melee_hit_parameters.moby = moby;
                melee_hit_parameters.f14 = 0x10001;
                melee_hit_parameters.f1C = 1.0f;
                fast_vec_sub(melee_hit_parameters.v, &hero.motion.pos, moby->pos);
                fast_vec_normalize(
                    melee_hit_parameters.v, melee_hit_parameters.v, frame_time * 10.0f);
                melee_hit_parameters.f20 = 1;
                collect_mobies_in_sphere(jp, 0x10, moby, &melee_hit_parameters, 0.5f);
                if (coll_hit.moby == target_moby) {
                    data->f146 =
                        data->f146 < scale_game_frames(0x3C) ? data->f146 : scale_game_frames(0x3C);
                }
            }
        }
    }
    if (moby->state != 0 && data->f2E != 1) {
        int engine_voice_index = data->f238;
        if (engine_voice_index != -1) {
            char *e = (char *)((char *)&D_0013E550) + engine_voice_index * 0x70;
            if (*(M_35F0 **)(e + 0x88) == moby) {
                if (*(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(engine_voice_index);
                }
            }
        }
        data->f238 = -1;
        mark_moby_for_removal(moby);
        return;
    }
    fast_dec_timer_s16(&data->f146);
    switch (moby->state) {
    case CIRCULAR_SAW_STATE_INITIALIZE: {
        float k;
        data->hp = 1.0f;
        data->f24 = 1;
        data->f28 = 0;
        data->f2E = 1;
        data->f29 = 0;
        data->f30 = 0.75f;
        data->f58 = 8;
        data->f5A = 6;
        FUN_L01_0026d930((char *)&data->f60);
        k = frame_time;
        data->f6C = 0.15f;
        data->f68 = 0.25f;
        data->f60 = 0x200;
        data->f84 = k * 3.0f;
        data->f64 = 1.0f;
        data->f98 = 4;
        qcopy(data->f240, moby->pos);
        data->f25C = 2.0f;
        data->f254 = k * 2.0f;
        data->f258 = k * 2.9670596f;
        qcopy(data->f120, moby->pos);
        data->f146 = scale_game_frames(0x3C);
        data->f1F0 = &D_L04_00161878;
        data->f136 = 0;
        data->f144 = 0;
        if (data->f150 == -1) {
            moby->state = 1;
        } else {
            moby->state = 4;
        }
        break;
    }
    case CIRCULAR_SAW_STATE_IDLE: {
        V4_35F0 save;
        {
            qcopy(save, moby->pos);
            update_object_wander_movement((char *)moby, (char *)data->f240, 0.35f, 0.25f);
            if (data->f138 != -1) {
                if (point_in_path_xy(moby->pos,
                                     ((P_35F0 *)l04_paths[data->f138])->pts,
                                     ((P_35F0 *)l04_paths[data->f138])->count) == 0) {
                    qcopy(moby->pos, save);
                }
            }
            if (moby->f52 != 0 && (moby->f70 & 2)) {
                blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(5));
            }
            if (flag ? target_distance < data->f140 * 2.0f : target_distance < data->f140) {
                if (flag || data->f138 == -1 ||
                    point_in_path_xy(&tg,
                                     ((P_35F0 *)l04_paths[data->f138])->pts,
                                     ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                    int nf = (data->f14E & 1) == 0;
                    if (nf) {
                        data->f14E |= 1;
                        blend_moby_animation(moby, (void *)6, 0, (void *)scale_game_frames(10));
                        data->f146 = scale_game_frames(0x3C);
                    }
                    ta = fast_arc_tan(tg.pos[0] - moby->pos[0], tg.pos[1] - moby->pos[1]);
                    if (target_distance < 2.0f && data->f146 == 0 &&
                        check_circular_saw_melee_attack_spacing((char *)moby) != 0) {
                        blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
                        data->f146 = scale_game_frames(0x3C);
                        moby->state = 7;
                    } else if (flag ? target_distance < data->f140 * 2.0f
                                    : target_distance < data->f140) {
                        if (data->f146 == 0) {
                            blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
                            data->f14C = 0;
                            moby->state = 3;
                        }
                    }
                }
            }
        }
        break;
    }
    case CIRCULAR_SAW_STATE_JUMP:
        if (moby->f53 != 5) {
            blend_moby_animation(moby, (void *)5, 0, (void *)scale_game_frames(5));
            data->f78 = -(frame_time * 7.668f);
        } else if (moby->f70 & 2) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(10));
            moby->state = 1;
        }
        break;
    case CIRCULAR_SAW_STATE_CHASE_TARGET:
        if ((flag ? target_distance < data->f140 * 2.0f : target_distance < data->f140) &&
            (flag || data->f138 == -1 ||
             point_in_path_xy(&tg,
                              ((P_35F0 *)l04_paths[data->f138])->pts,
                              ((P_35F0 *)l04_paths[data->f138])->count) != 0)) {
            rot = compute_circular_saw_flock_steering(
                moby, dest, flag ? data->f140 * 2.0f : data->f140);
            if (target_distance < 2.0f && data->f146 == 0 &&
                check_circular_saw_melee_attack_spacing((char *)moby) != 0) {
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
                data->f146 = scale_game_frames(0x3C);
                moby->state = 7;
            }
        } else if (flag == 0) {
            qcopy(dest, data->f120);
            if (vector_distance_xy(moby->pos, data->f120) < 1.0f) {
                blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(10));
                moby->state = 1;
            }
        }
        func_L00_002592B0_b((char *)moby,
                            rot,
                            &data->f130,
                            frame_time_sq * 1.0471976f,
                            frame_time_sq * 3.1415927f,
                            frame_time * 3.1415927f);
        break;
    case CIRCULAR_SAW_STATE_FOLLOW_PATH: {
        P_35F0 *path = ((P_35F0 *)l04_paths[data->f150]);
        int     i;
        float  *yaw_velocity;

        if (flag ? target_distance < data->f140 * 2.0f : target_distance < data->f140) {
            if (flag || data->f138 == -1 ||
                point_in_path_xy(&tg,
                                 ((P_35F0 *)l04_paths[data->f138])->pts,
                                 ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                int nf = (data->f14E & 1) == 0;
                if (nf) {
                    data->f14E |= 1;
                    blend_moby_animation(moby, (void *)6, 0, (void *)scale_game_frames(10));
                    data->f146 = scale_game_frames(0x3C);
                }
                ta = fast_arc_tan(tg.pos[0] - moby->pos[0], tg.pos[1] - moby->pos[1]);
                if (target_distance < 2.0f && data->f146 == 0 &&
                    check_circular_saw_melee_attack_spacing((char *)moby) != 0) {
                    blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
                    data->f146 = scale_game_frames(0x3C);
                    moby->state = 7;
                } else if (flag ? target_distance < data->f140 * 2.0f
                                : target_distance < data->f140) {
                    if (data->f146 == 0) {
                        blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
                        data->f14C = 0;
                        moby->state = 3;
                    }
                }
            }
        }
        if (vector_distance_xy(path->pts[data->f154], moby->pos) < 1.0f ||
            fast_difference_between_rotations(
                moby->rot,
                fast_arc_tan(path->pts[data->f154][0] - moby->pos[0],
                             path->pts[data->f154][1] - moby->pos[1])) > 2.3561945f) {
            data->f154++;
            if (data->f154 == path->count) {
                data->f154 = 0;
            }
        }
        i = 0;
        yaw_velocity = &data->f130;
        for (; i < 4; i++) {
            if (data->f146 == 0 && data->f160[i] != -1 &&
                is_point_inside_clip_volume(moby->pos, data->f160[i]) != 0) {
                data->f158 = data->f160[i];
                moby->state = 5;
                data->f15A = 0;
                break;
            }
        }
        {
            int n = data->f154;
            qcopy(dest, path->pts[n]);
            ta = fast_arc_tan(path->pts[n][0] - moby->pos[0], path->pts[n][1] - moby->pos[1]);
            func_L00_002592B0_b((char *)moby,
                                ta,
                                yaw_velocity,
                                frame_time_sq * 1.0471976f,
                                frame_time_sq * 3.1415927f,
                                frame_time * 3.1415927f);
        }
        break;
    }
    case CIRCULAR_SAW_STATE_POINT_ATTACK: {
        P_35F0 *path = ((P_35F0 *)l04_paths[data->f150]);

        if (flag ? target_distance < data->f140 * 2.0f : target_distance < data->f140) {
            if (flag || data->f138 == -1 ||
                point_in_path_xy(&tg,
                                 ((P_35F0 *)l04_paths[data->f138])->pts,
                                 ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                int nf = (data->f14E & 1) == 0;
                if (nf) {
                    data->f14E |= 1;
                    blend_moby_animation(moby, (void *)6, 0, (void *)scale_game_frames(10));
                    data->f146 = scale_game_frames(0x3C);
                }
                ta = fast_arc_tan(tg.pos[0] - moby->pos[0], tg.pos[1] - moby->pos[1]);
                if (target_distance < 2.0f && data->f146 == 0 &&
                    check_circular_saw_melee_attack_spacing((char *)moby) != 0) {
                    blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
                    data->f146 = scale_game_frames(0x3C);
                    moby->state = 7;
                    data->f130 = 0.0f;
                } else if (flag ? target_distance < data->f140 * 2.0f
                                : target_distance < data->f140) {
                    if (data->f146 == 0) {
                        blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
                        data->f14C = 0;
                        moby->state = 3;
                    }
                }
            }
        }
        switch (data->f15A) {
        case CIRCULAR_SAW_POINT_ATTACK_APPROACH_AND_ALIGN: {
            LevelCuboid *e = l04_cuboids;
            int          n = data->f158;
            qcopy(dest, &e[n].pos);
            func_L00_002592B0_b((char *)moby,
                                e[n].rot.z,
                                &data->f130,
                                frame_time_sq * 1.0471976f,
                                frame_time_sq * 3.1415927f,
                                frame_time * 3.1415927f);
            if (moby->f53 != 1) {
                blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
            }
            if (vector_distance_xy(moby->pos, dest) < 1.0f) {
                if (fast_difference_between_rotations(moby->rot, l04_cuboids[data->f158].rot.z) <
                    0.17453292f) {
                    data->f15A = 1;
                }
            }
            break;
        }
        case CIRCULAR_SAW_POINT_ATTACK_ATTACK:
            qcopy(dest, &l04_cuboids[data->f158].pos);
            if (moby->f53 != 7) {
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
            } else if (moby->f70 & 2) {
                blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(10));
                data->f130 = 0.0f;
                data->f15A = 2;
            }
            break;
        case CIRCULAR_SAW_POINT_ATTACK_TURN_TO_PATH: {
            qcopy(dest, &l04_cuboids[data->f158].pos);
            ta = fast_arc_tan(path->pts[data->f154][0] - moby->pos[0],
                              path->pts[data->f154][1] - moby->pos[1]);
            func_L00_002592B0_b((char *)moby,
                                ta,
                                &data->f130,
                                frame_time_sq * 1.0471976f,
                                frame_time_sq * 3.1415927f,
                                frame_time * 3.1415927f);
            if (fast_difference_between_rotations(
                    moby->rot,
                    fast_arc_tan(path->pts[data->f154][0] - moby->pos[0],
                                 path->pts[data->f154][1] - moby->pos[1])) < 0.17453292f) {
                moby->state = 4;
                data->f146 = scale_game_frames(0xF0);
            }
            break;
        }
        }
        break;
    }
    case CIRCULAR_SAW_STATE_MELEE_ATTACK:
        rot =
            compute_circular_saw_flock_steering(moby, dest, flag ? data->f140 * 2.0f : data->f140);
        if (data->f146 == 0 && (hd > 44.0f || (moby->f70 & 2))) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(5));
            data->f146 = scale_game_frames(0x78);
            moby->state = 1;
            data->f14E |= 2;
        }
        func_L00_002592B0_b((char *)moby,
                            rot,
                            &data->f130,
                            frame_time_sq * 1.0471976f,
                            frame_time_sq * 3.1415927f,
                            frame_time * 3.1415927f);
        break;
    case CIRCULAR_SAW_STATE_HIT_RECOVERY:
        if ((moby->f52 == 8 && compute_interpolated_record_value(moby) > 30.0f) ||
            (moby->f70 & 2)) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(10));
            moby->state = 1;
            data->f146 = 0;
        }
        break;
    case CIRCULAR_SAW_STATE_DYING:
        if (moby->f70 & 2) {
            spawn_circular_saw_destruction_effects((char *)moby);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case 9:
        if (FUN_L00_002dc9f0(moby, data->fC0) != 0) {
            moby->state = 0;
            data->f1E8 = 0;
        }
        break;
    }
    if (data->f13C > 0.0f) {
        V4_35F0 v;
        {
            v[0] = fast_cos(fast_add_rotations(moby->rot, 3.1415927f)) * data->f13C;
            v[1] = fast_sin(fast_add_rotations(moby->rot, 3.1415927f)) * data->f13C;
            v[2] = 0.0f;
            fast_vec_add(v, v, moby->pos);
            data->f13C += -10.0f / (scale_time(15.0f) * scale_time(15.0f));
            FUN_L00_00258490(moby, moby->pos, v, 1, 0.25f, 0.57f, 0.5f, 0.5235988f);
        }
    }
    if (moby->state == 9) {
        return;
    }
    if (moby->state == 4) {
        V4_35F0 v;
        V4_35F0 np;
        {
            if (moby->f53 != 1) {
                blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
            }
            v[0] = fast_cos(moby->rot) * (frame_time * 2.0f);
            v[1] = fast_sin(moby->rot) * (frame_time * 2.0f);
            v[2] = 0.0f;
            fast_vec_add(v, v, moby->pos);
            qcopy(np, moby->pos);
            FUN_L00_00258490(moby, np, v, 0, 0.25f, 0.57f, 0.5f, 0.5235988f);
            if (data->f138 == -1 ||
                point_in_path_xy(np,
                                 ((P_35F0 *)l04_paths[data->f138])->pts,
                                 ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                qcopy(moby->pos, np);
            }
        }
    } else if (moby->state == 3) {
        V4_35F0 v;
        V4_35F0 np;
        float   ang = fast_arc_tan(dest[0] - moby->pos[0], dest[1] - moby->pos[1]);
        int     anim = moby->f53;
        I4_35F0 movement_animation_by_direction = D_L04_001618A0;
        int     moved;
        {
            if (vector_distance_xy(moby->pos, dest) >= frame_time * 3.0f * 2.0f) {
                float rel = fast_subtract_rotations(ang, moby->rot);
                float biased_relative_yaw = fast_add_rotations(rel, 0.3926991f);
                float movement_relative_yaw;
                if (biased_relative_yaw < 0.0f) {
                    biased_relative_yaw += 6.2831855f;
                }
                if (fast_difference_between_rotations(
                        fast_normalize_angle(fast_normalize_angle(
                            convert_integer_to_float(data->f14C) * 1.5707964f)),
                        rel) > 1.7453293f) {
                    data->f14C = truncate_float_to_s32(biased_relative_yaw * 1.999f / 3.1415927f);
                    anim = movement_animation_by_direction.v[data->f14C];
                }
                movement_relative_yaw =
                    fast_normalize_angle(convert_integer_to_float(data->f14C) * 1.5707964f);
                v[0] = fast_cos(fast_add_rotations(moby->rot, movement_relative_yaw)) *
                       (frame_time * 3.0f);
                v[1] = fast_sin(fast_add_rotations(moby->rot, movement_relative_yaw)) *
                       (frame_time * 3.0f);
                v[2] = 0.0f;
                fast_vec_add(v, v, moby->pos);
            } else {
                qcopy(v, moby->pos);
                anim = 0;
            }
            qcopy(np, moby->pos);
            moved = FUN_L00_00258490(moby, np, v, 0, 0.25f, 0.57f, 0.5f, 0.5235988f);
            if (data->f138 == -1 ||
                point_in_path_xy(np,
                                 ((P_35F0 *)l04_paths[data->f138])->pts,
                                 ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                qcopy(moby->pos, np);
                if (moved == 0) {
                    anim = 0;
                }
            } else {
                anim = 0;
            }
            if (fast_dec_timer_s16(&data->f144) != 0 && anim != moby->f53) {
                blend_moby_animation(moby, (void *)anim, 0, (void *)scale_game_frames(10));
                data->f144 = scale_game_frames(0xF);
            }
        }
    } else if (moby->state == 5 || (moby->state == 7 && moby->f52 == 7 && hd > 15.0f)) {
        if (vector_distance_xy(moby->pos, dest) > frame_time * 2.0f) {
            V4_35F0 v;
            V4_35F0 np;
            {
                v[0] = fast_cos(fast_arc_tan(dest[0] - moby->pos[0], dest[1] - moby->pos[1])) *
                       (frame_time * 2.0f);
                v[1] = fast_sin(fast_arc_tan(dest[0] - moby->pos[0], dest[1] - moby->pos[1])) *
                       (frame_time * 2.0f);
                v[2] = 0.0f;
                fast_vec_add(v, v, moby->pos);
                qcopy(np, moby->pos);
                FUN_L00_00258490(moby, np, v, 0, 0.25f, 1.25f, 0.5f, 0.5235988f);
                if (data->f138 == -1 ||
                    point_in_path_xy(np,
                                     ((P_35F0 *)l04_paths[data->f138])->pts,
                                     ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                    qcopy(moby->pos, np);
                }
            }
        }
    } else if (moby->state != 6 && moby->state != 8) {
        V4_35F0 v;
        {
            qcopy(v, moby->pos);
            FUN_L00_00258490(moby, moby->pos, v, 0, 0.25f, 0.57f, 0.5f, 0.5235988f);
        }
    }
    data->f78 += frame_time_sq * 9.8f;
    moby->pos[2] -= data->f78 - 2.0f;
    {
        float g = probe_ground_height(moby->pos, 0, 0.5f);
        moby->pos[2] -= 2.0f;
        if (moby->pos[2] < g) {
            moby->pos[2] = g;
            data->f78 = 0.0f;
        }
    }
    if (moby->pos[2] < data->f120[2] - 5.0f) {
        spawn_circular_saw_destruction_effects((char *)moby);
        mark_moby_for_removal(moby);
        return;
    }
    if (vector_distance_xy(moby->pos, D_L04_00166F40) < 20.0f) {
        V4_35F0 b;
        V4_35F0 joint_position_scratch;
        int     alt;
        {
            *(OvlQuad *)joint_position_scratch = 0;
            joint_position_scratch[0] = random_float_between(1.0f, -1.0f) * (frame_time * 0.1f);
            joint_position_scratch[1] = random_float_between(1.0f, -1.0f) * (frame_time * 0.1f);
            joint_position_scratch[2] = random_float_between(0.8f, 1.2f) * (frame_time * -2.5f);
            *(OvlQuad *)b = *(OvlQuad *)joint_position_scratch;
            alt = random_integer_below(100) & 1;
            moby_point_position(moby, 0, joint_position_scratch);
            transform_vector_by_basis(b, b, moby->fC0);
            spawn_effect_particle_with_target_scalar(
                joint_position_scratch,
                b,
                alt ? 0x400F0F7F : 0x407F7F7F,
                0x272727,
                random_float_between(75000.0f, 100000.0f),
                scale_game_frames(alt ? random_int_between(10, 15) : random_int_between(20, 30)));
        }
    }
    trooper_anim_finish(moby, data->fB0);
}

typedef struct {
    u8  pad0[4];
    s32 mode; /* 0x04 */
    u8  pad8[2];
    u8  tint; /* 0x0A */
    u8  fade; /* 0x0B */
} PuffTail;

/* A smoke puff from FUN_L00_0026d000. */
typedef struct {
    u8       pad0[3];
    u8       alpha; /* 0x03 */
    s32      color; /* 0x04 */
    u8       pad8[2];
    s16      life; /* 0x0A */
    u8       padC[0x14];
    PuffTail tail; /* 0x20 */
} Puff;

extern f32   randf_sym(f32, f32) __asm__("FUN_L00_00257c48");
extern Puff *spawn_puff(void *, f32, f32, f32, int, void *, f32, int) __asm__("FUN_L00_0026d000");
extern void  spawn_burst(void *, void *, int, int, int, int, f32) __asm__("FUN_L00_0026cbb0");
extern f32   smoke_trail_puff_vertical_velocity_scale_gp __asm__("D_L04_001618B4")
    __attribute__((sda));

#include "rnc/overlay/l04/mobys/big_axebot_data/spawn_smoke_puff_trail_with_endpoint_bursts.h"

/* Lays a trail of smoke puffs from FROM to TO with a burst at each end. */
void spawn_smoke_puff_trail_with_endpoint_bursts(void *owner,
                                                 void *from,
                                                 void *to,
                                                 int   count,
                                                 float from_puff_scale,
                                                 float to_puff_scale) __asm__("FUN_L04_002c41e0");

void spawn_smoke_puff_trail_with_endpoint_bursts(

    void *owner, void *from, void *to, int count, float from_puff_scale, float to_puff_scale) {
    Vec4      step;
    Vec4      at;
    Vec4      particle_spawn_velocity;
    float     grow;
    int       i;
    int       tint;
    int       grey;
    float     size;
    Puff     *puff;
    PuffTail *tail;
    int       spin;

    fast_vec_sub(&step, to, from);
    fast_vec_scale(&step, &step, 1.0f / (float)count);
    grow = (to_puff_scale - from_puff_scale) / (float)count;
    particle_spawn_velocity.q = 0;
    particle_spawn_velocity.f[3] = 1.0f;
    particle_spawn_velocity.f[2] = 0.01f;
    for (i = 0; i < count; i++) {
        fast_vec_scale(&at, &step, convert_integer_to_float(i));
        fast_vec_add(&at, from, &at);
        particle_spawn_velocity.f[0] = randf_sym(0.0f, 0.005f);
        particle_spawn_velocity.f[1] = randf_sym(0.0f, 0.005f);
        particle_spawn_velocity.f[2] =
            random_float_between(smoke_trail_puff_vertical_velocity_scale_gp * 0.1f,
                                 smoke_trail_puff_vertical_velocity_scale_gp);
        tint = random_int_between(0x40, 0x70);
        grey = random_int_between(0x40, 0x7F);
        grey = grey | (grey << 16 | grey << 8);
        size = random_float_between(1.0f, 1.02f);
        spin = random_int_between(-2, 2);
        puff = spawn_puff(&at,
                          smoke_trail_puff_position_jitter_scale_gp,
                          1.0f,
                          size,
                          spin,
                          &particle_spawn_velocity,
                          grow + from_puff_scale * 210000.0f,
                          tint << 24 | grey);
        if (puff != NULL) {
            tail = &puff->tail;
            if (random_integer_below(2) != 0) {
                puff->alpha = 0x7E;
                grey = random_int_between(0x60, 0xE0);
                grey = grey | (grey << 16 | grey << 8);
                puff->color = tint << 24 | grey;
            }
            puff->life = scale_game_frames(0x5A);
            tail->mode = 2;
            tail->tint = tint;
            tail->fade = scale_game_frames(0x5A);
        }
    }
    particle_spawn_velocity.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    particle_spawn_velocity.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    particle_spawn_velocity.f[2] = 0.0f;
    particle_spawn_velocity.f[2] = random_float_between(0.01f, 0.03f);
    spawn_burst(from,
                &particle_spawn_velocity,
                0x4F007FFF,
                0x1FFFFFFF,
                random_int_between(scale_game_frames(10), scale_game_frames(20)),
                1,
                10000.0f);
    particle_spawn_velocity.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    particle_spawn_velocity.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    particle_spawn_velocity.f[2] = 0.0f;
    particle_spawn_velocity.f[2] = random_float_between(0.01f, 0.03f);
    spawn_burst(to,
                &particle_spawn_velocity,
                0x4F007FFF,
                0x1FFFFFFF,
                random_int_between(scale_game_frames(10), scale_game_frames(20)),
                1,
                20000.0f);
}

#include "qcopy.h"

/* Blarg trooper update: takes hits, walks its path toward the target, fires, and keeps its gun moby attached. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C5BD0), where it is exact; names translated to the US level program. */

struct Path5BD0 {
    int   count;
    int   pad[3];
    float pts[1][4];
};

struct Shot5BD0 {
    float f0;
    float f4;
    float f8;
    float fC;
    void *p10;
    int   v14;
    char  c18;
    char  c19;
    short h1A;
    float f1C;
    int   pad[4];
};

extern char                *D_L04_0015FFD8;
extern char *D_L04_00173FD8 NOT_SDA;
extern char                 D_L04_00166E00[];
extern char                 blarg_trooper_missing_path_fields_format[] __asm__("D_L04_001DBC80");
extern float                vector_length_xy(void *);
extern int                  DebugPrint();
extern int                  rand(void);
extern void                 record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void spawn_class_b8_projectile(void *, void *, void *, int) __asm__("FUN_L04_002aee30");
void        destroy_blarg_trooper(struct Moby *arg) __asm__("FUN_L04_002c4808");
extern int  func_L00_002594C8_o(void *, void *, void *, float, float, float, float, int) __asm__("FUN_L00_00258490");

void blarg_trooper_update(struct Moby *moby) __asm__("FUN_L04_002c4850");

void blarg_trooper_update(struct Moby *moby) {
    enum {
        BLARG_TROOPER_STATE_INITIALIZE = 0,
        BLARG_TROOPER_STATE_IDLE = 1,
        BLARG_TROOPER_STATE_MOVE_TO_PATH_POINT = 2,
        BLARG_TROOPER_STATE_RUN_TO_TARGET_OR_PATH_POINT = 3,
        BLARG_TROOPER_STATE_JUMP = 4,
        BLARG_TROOPER_STATE_AIM = 5,
        BLARG_TROOPER_STATE_FIRE = 6,
        BLARG_TROOPER_STATE_MELEE_ATTACK = 7,
        BLARG_TROOPER_STATE_HIT_RECOVERY = 8,
        BLARG_TROOPER_STATE_BACKWARD_RECOVERY = 9,
        BLARG_TROOPER_STATE_KNOCKBACK_RECOVERY = 10,
        BLARG_TROOPER_STATE_DYING = 11
    };
    enum {
        BLARG_TROOPER_HIT_REACTION_NONE = 0,
        BLARG_TROOPER_HIT_REACTION_FATAL = 1,
        BLARG_TROOPER_HIT_REACTION_FATAL_KNOCKBACK = 2,
        BLARG_TROOPER_HIT_REACTION_START_HIT_RECOVERY = 3,
        BLARG_TROOPER_HIT_REACTION_START_KNOCKBACK_RECOVERY = 4,
        BLARG_TROOPER_HIT_REACTION_KNOCKBACK_RECOVERY = 5,
        BLARG_TROOPER_HIT_REACTION_RECOIL_RECOVERY = 6,
        BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_RESPONSE = 7,
        BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_REACTION = 8,
        BLARG_TROOPER_HIT_REACTION_CONTINUE_WITHOUT_RESPONSE = 9,
        BLARG_TROOPER_HIT_REACTION_NO_FURTHER_RESPONSE = 10,
        BLARG_TROOPER_HIT_REACTION_IGNORED = 11
    };
    struct {
        float tgt[20];
        float v50[4];
        float v60[4];
        union {
            float m[4][4];
            struct {
                float           v70[4];
                float           v80[4];
                float           v90[4];
                struct Shot5BD0 blk;
            } p;
        } w;
    } update_scratch;
    int              hit_reaction_status;
    float            dmg;
    float            fatal_knockback_yaw;
    float            recovery_knockback_yaw;
    float            one;
    char            *data;
    int              flag;
    float           *pos;
    char            *target;
    float            target_distance;
    float            alive;
    float            best;
    float            target_selection_distance_limit;
    int              path_index_when_selector_nonzero;
    int              path_index_when_selector_zero;
    int              bi;
    int              i;
    struct Path5BD0 *path;
    char            *hit;
    int              r;

    flag = 0;
    data = (char *)moby->pvars;
    if (*(int *)(data + 0x38) != 0 || *(unsigned char *)(data + 0xFE) != 0) {
        flag = 1;
    }
    pos = (float *)&moby->pos;
    manipulator_set_scale(data + 0x110, 3.0f);
    if (moby->visible != 0) {
        if (fast_vec_dist(pos, D_L04_00166F40) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby->shadow_range = 0x15;
        }
    }
    if (flag) {
        float x = *(float *)(data + 0xDC);
        target_selection_distance_limit = x + x;
    } else {
        target_selection_distance_limit = *(float *)(data + 0xDC);
    }
    select_moby_combat_target(moby, update_scratch.tgt, target_selection_distance_limit);
    target = *(char **)&update_scratch.tgt[16];
    target_distance = vector_distance_xy(pos, update_scratch.tgt);
    dmg = 0.0f;
    alive = compute_interpolated_record_value(moby);
    if (moby->state != 0 && *(unsigned char *)(data + 0x2E) != 1) {
        char *o = *(char **)(data + 0xF8);
        if (o != 0) {
            mark_moby_for_removal(o);
        }
        mark_moby_for_removal(moby);
        return;
    }
    path_index_when_selector_nonzero = *(int *)(data + 0xE0);
    if (path_index_when_selector_nonzero == -1 ||
        (path_index_when_selector_zero = *(int *)(data + 0xE4),
         path_index_when_selector_zero == -1)) {
        DebugPrint(blarg_trooper_missing_path_fields_format,
                   truncate_float_to_s32((float)(((char *)moby - D_L04_0015FFD8) >> 8)));
        destroy_blarg_trooper(moby);
        return;
    }
    bi = -1;
    best = 0.0f;
    qcopy(update_scratch.v50, &update_scratch.tgt[8]);
    path = ((
        struct Path5BD0 *)l04_paths[*(short *)(data + 0xE8) != 0 ? path_index_when_selector_nonzero
                                                                 : path_index_when_selector_zero]);
    for (i = 0; i < path->count; i++) {
        float d = vector_distance_xy(update_scratch.v50, path->pts[i]);
        if (d > best) {
            best = d;
            bi = i;
        }
    }
    if (bi != -1) {
        qcopy(update_scratch.v60, path->pts[bi]);
        best = vector_distance_xy(update_scratch.v60, pos);
    } else {
        qcopy(update_scratch.v60, pos);
        best = 0.0f;
    }
    hit = find_moby_hit(moby, 0x210000, 0);
    r = hit_reaction(moby, hit, data + 0x20, 0, &hit_reaction_status, &dmg, 0, 4);
    fast_dec_timer_s16(data + 0xF0);
    fast_dec_timer_s16(data + 0xF2);
    if (hit != 0 && moby->state < 0xB && *(short *)(data + 0xF0) == 0) {
        char *o;
        float t;
        o = *(char **)(hit + 0x20);
        one = 1.0f;
        if (o != 0) {
            struct Hero *g = &hero;
            if (o != ((char *)g->items[0].moby)) {
                recovery_knockback_yaw = fast_arc_tan(moby->pos.x - *(float *)(o + 0x10),
                                                      moby->pos.y - *(float *)(o + 0x14));
            } else {
                recovery_knockback_yaw = fast_arc_tan(moby->pos.x - g->motion.pos.f[0],
                                                      moby->pos.y - g->motion.pos.f[1]);
            }
        } else {
            struct Hero *g = &hero;
            recovery_knockback_yaw =
                fast_arc_tan(moby->pos.x - g->motion.pos.f[0], moby->pos.y - g->motion.pos.f[1]);
        }
        t = *(float *)(data + 0x20) - dmg;
        *(float *)(data + 0x20) = t;
        if (t <= 0.0f) {
            r = 1;
        }
        *(unsigned char *)(data + 0xFE) = scale_game_frames(0xF0);
        switch (r) {
        case BLARG_TROOPER_HIT_REACTION_NONE:
            break;
        case BLARG_TROOPER_HIT_REACTION_FATAL:
        case BLARG_TROOPER_HIT_REACTION_FATAL_KNOCKBACK:
            moby->flags &= 0xEFFF;
            *(unsigned char *)(data + 0x67) = 0xFA;
            {
                float a = frame_time_sq * 37.0f;
                float b = frame_time * 17.0f;
                float fatal_knockback_horizontal_speed_adjustment = frame_time_sq * 20.0f;
                float d = frame_time * 13.0f;
                *(float *)(data + 0x80) = a;
                *(float *)(data + 0x88) = b;
                *(float *)(data + 0x84) = fatal_knockback_horizontal_speed_adjustment;
                *(float *)(data + 0x8C) = d;
            }
            *(OvlQuad *)update_scratch.w.p.v70 = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                update_scratch.w.p.v70, &fatal_knockback_yaw, data + 0x88, data + 0x8C);
            trooper_start_knockback(fatal_knockback_yaw, moby, data + 0x70, 0xD, 1, 0);
            *(float *)(data + 0xC0) = 12.0f;
            *(float *)(data + 0xC4) = 25.0f;
            moby->state = 0xB;
            FUN_L00_00257470(moby, 0, -1);
            break;
        case BLARG_TROOPER_HIT_REACTION_START_HIT_RECOVERY:
        case BLARG_TROOPER_HIT_REACTION_RECOIL_RECOVERY: {
            int attacker_class;
            *(unsigned char *)(data + 0x67) = 0x96;
            blend_moby_animation(moby, (void *)8, 0, (void *)scale_game_frames(5));
            moby->state = 8;
            attacker_class = *(short *)(*(char **)(hit + 0x20) + 0xA6);
            if (attacker_class == 0x131 || attacker_class == 0xB0) {
                *(float *)(data + 0xF4) = 2.0f / scale_time(15.0f) * 0.5f;
            } else {
                *(float *)(data + 0xF4) = 2.0f / scale_time(15.0f);
            }
            *(OvlQuad *)update_scratch.w.p.v70 = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                update_scratch.w.p.v70, &recovery_knockback_yaw, data + 0xF4, &one);
            moby->rot.z = fast_add_rotations(3.1415927f, recovery_knockback_yaw);
            break;
        }
        case BLARG_TROOPER_HIT_REACTION_START_KNOCKBACK_RECOVERY:
        case BLARG_TROOPER_HIT_REACTION_KNOCKBACK_RECOVERY:
            *(unsigned char *)(data + 0x67) = 0xFA;
            blend_moby_animation(moby, (void *)0xC, 0, (void *)scale_game_frames(5));
            moby->state = 0xA;
            *(float *)(data + 0xF4) = 2.0f / scale_time(15.0f);
            *(OvlQuad *)update_scratch.w.p.v70 = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                update_scratch.w.p.v70, &recovery_knockback_yaw, data + 0xF4, &one);
            moby->rot.z = fast_add_rotations(3.1415927f, recovery_knockback_yaw);
            break;
        case BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_RESPONSE:
        case BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_REACTION:
        case BLARG_TROOPER_HIT_REACTION_CONTINUE_WITHOUT_RESPONSE:
        case BLARG_TROOPER_HIT_REACTION_NO_FURTHER_RESPONSE:
        case BLARG_TROOPER_HIT_REACTION_IGNORED:
            break;
        }
        trooper_anim_update(moby, (short *)(data + 0x60));
    }
    moby->hit_slot = 0xFF;
    if (*(int *)(data + 0x38) != 0) {
        *(unsigned char *)(data + 0xFE) = scale_game_frames(0xF0);
    }
    *(int *)(data + 0x38) = 0;
    FUN_L00_001fefc8(data + 0xFE);
    fast_dec_timer((int *)(data + 0x108));
    switch (moby->state) {
    case BLARG_TROOPER_STATE_INITIALIZE:
        moby->flags |= 0x1000;
        *(float *)(data + 0x20) = 2.0f;
        *(short *)(data + 0x24) = 2;
        *(unsigned char *)(data + 0x28) = 1;
        *(unsigned char *)(data + 0x2E) = 1;
        *(unsigned char *)(data + 0x29) = 1;
        *(short *)(data + 0xF0) = 0;
        *(unsigned char *)(data + 0xFE) = 0;
        *(unsigned char *)(data + 0xFF) = 0;
        *(float *)(data + 0xD0) = 0.0f;
        {
            float z = moby->pos.z;
            *(unsigned char *)(data + 0x58) = 8;
            *(float *)(data + 0xD4) = z;
            *(unsigned char *)(data + 0x5A) = 0xA;
        }
        if (rand() & 1) {
            moby->flags |= 0x8000;
        }
        if (*(unsigned char *)(data + 0xFC) != 0) {
            blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(0xA));
            moby->state = 5;
        } else {
            blend_moby_animation(
                moby, (void *)((rand() ^ 1) & 1), 0, (void *)scale_game_frames(0xA));
            moby->state = 1;
        }
        break;
    case BLARG_TROOPER_STATE_IDLE:
        if (moby->anim_flags & 2) {
            blend_moby_animation(
                moby, (void *)((rand() ^ 1) & 1), 0, (void *)scale_game_frames(0xA));
        }
        {
            float a = fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                   update_scratch.v50[1] - moby->pos.y);
            FUN_L00_00258278(moby,
                             (float *)(data + 0xEC),
                             a,
                             frame_scale_sq * 0.04f,
                             frame_scale_sq * 0.3f,
                             frame_time * 7.330382823944092f);
        }
        if (flag) {
            blend_moby_animation(moby, (void *)0xB, 0, (void *)scale_game_frames(8));
            moby->state = 3;
            break;
        }
        if (target_distance > 2.5f && best > 1.0f) {
            if (target_distance > 14.0f) {
                blend_moby_animation(moby, (void *)0xA, 0, (void *)scale_game_frames(8));
                moby->state = 2;
                break;
            }
            if (target_distance < 8.0f && *(int *)(data + 0x108) == 0) {
                blend_moby_animation(moby, (void *)0xB, 0, (void *)scale_game_frames(8));
                moby->state = 3;
                break;
            }
        }
        if (target_distance < 14.0f && target_distance >= 2.5f &&
            absolute_float(moby->pos.z - update_scratch.tgt[2]) < 8.0f) {
            if (*(short *)(data + 0xF2) == 0) {
                *(OvlQuad *)update_scratch.w.p.v90 = 0;
                update_scratch.w.p.v90[2] = 0.5f;
                fast_vec_add(update_scratch.w.p.v70, pos, update_scratch.w.p.v90);
                fast_vec_add(update_scratch.w.p.v80, update_scratch.v50, update_scratch.w.p.v90);
                if (collision_line(update_scratch.w.p.v70, update_scratch.w.p.v80, 0, moby, 0) !=
                        0 &&
                    D_L04_00173FD8 == target) {
                    moby->state = 5;
                    blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(7));
                }
            }
        } else if (target_distance <= 2.5f) {
            int t = *(short *)(data + 0xF2);
            if (t <= 0) {
                moby->state = 7;
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
            } else if (scale_game_frames(0x14) < t) {
                *(short *)(data + 0xF2) = scale_game_frames(0x14);
            }
        }
        break;
    case BLARG_TROOPER_STATE_MOVE_TO_PATH_POINT:
        if (target_distance < 13.0f || best < 1.0f) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(0xA));
            moby->state = 1;
        } else {
            float f;
            {
                float a = fast_arc_tan(update_scratch.v60[0] - moby->pos.x,
                                       update_scratch.v60[1] - moby->pos.y);
                FUN_L00_00258278(moby,
                                 (float *)(data + 0xEC),
                                 a,
                                 frame_scale_sq * 0.03f,
                                 frame_scale_sq * 0.3f,
                                 frame_time * 4.712389f);
            }
            f = *(float *)(data + 0xEC) * 30.0f;
            if (f > 0.7853982f) {
                f = 0.7853982f;
            } else if (f < -0.7853982f) {
                f = -0.7853982f;
            }
            *(float *)(data + 0x178) = f * 0.7f;
            *(float *)(data + 0x1F8) = f * 0.4f;
            update_scratch.w.p.v70[0] = fast_cos(moby->rot.z) * (frame_time * 2.5f);
            update_scratch.w.p.v70[1] = fast_sin(moby->rot.z) * (frame_time * 2.5f);
            update_scratch.w.p.v70[2] = 0.0f;
            fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
            func_L00_002594C8_o(moby, pos, update_scratch.w.p.v70, 0.5f, 1.0f, 0.5f, 0.5235988f, 0);
        }
        break;
    case BLARG_TROOPER_STATE_RUN_TO_TARGET_OR_PATH_POINT:
        *(int *)(data + 0x108) = scale_game_frames(0x64);
        if (flag) {
            int k;
            {
                float a = fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                       update_scratch.v50[1] - moby->pos.y);
                FUN_L00_00258278(moby,
                                 (float *)(data + 0xEC),
                                 a,
                                 frame_scale_sq * 0.04f,
                                 frame_scale_sq * 0.3f,
                                 frame_time * 4.886922f);
            }
            update_scratch.w.p.v70[0] = fast_cos(moby->rot.z) * (frame_time * 6.0f);
            update_scratch.w.p.v70[1] = fast_sin(moby->rot.z) * (frame_time * 6.0f);
            update_scratch.w.p.v70[2] = 0.0f;
            fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
            qcopy(update_scratch.w.p.v80, pos);
            if (func_L00_002594C8_o(moby,
                                    update_scratch.w.p.v80,
                                    update_scratch.w.p.v70,
                                    0.5f,
                                    1.0f,
                                    0.5f,
                                    0.5235988f,
                                    0) != 0) {
                if (point_in_path_xy(update_scratch.w.p.v80, path->pts, path->count) != 0) {
                    qcopy(pos, update_scratch.w.p.v80);
                    k = 0xB;
                } else {
                    k = 0;
                }
            } else {
                k = 0;
            }
            if (FUN_L00_001fefc8(data + 0xFF) != 0 && k != moby->prev_seq) {
                blend_moby_animation(moby, (void *)k, 0, (void *)scale_game_frames(0xA));
                *(unsigned char *)(data + 0xFF) = scale_game_frames(0xF);
            }
            if (*(short *)(data + 0xF2) == 0) {
                if (target_distance < 1.0f && fast_difference_between_rotations(
                                                  fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                                               update_scratch.v50[1] - moby->pos.y),
                                                  moby->rot.z) < 0.2617994f) {
                    moby->state = 7;
                    blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
                    *(unsigned char *)(data + 0xFF) = 0;
                } else if (target_distance < 14.0f) {
                    moby->state = 5;
                    blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(7));
                    *(unsigned char *)(data + 0xFF) = 0;
                }
            }
        } else {
            if (target_distance < 1.0f &&
                fast_difference_between_rotations(fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                                               update_scratch.v50[1] - moby->pos.y),
                                                  moby->rot.z) < 0.2617994f) {
                moby->state = 7;
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
            } else if (target_distance > 9.0f) {
                moby->state = 5;
                blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(7));
            } else if (best < 1.0f) {
                blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(0xA));
                moby->state = 1;
            } else {
                {
                    float a = fast_arc_tan(update_scratch.v60[0] - moby->pos.x,
                                           update_scratch.v60[1] - moby->pos.y);
                    FUN_L00_00258278(moby,
                                     (float *)(data + 0xEC),
                                     a,
                                     frame_scale_sq * 0.035f,
                                     frame_scale_sq * 0.3f,
                                     frame_time * 4.712389f);
                }
                update_scratch.w.p.v70[0] = fast_cos(moby->rot.z) * (frame_time * 6.0f);
                update_scratch.w.p.v70[1] = fast_sin(moby->rot.z) * (frame_time * 6.0f);
                update_scratch.w.p.v70[2] = 0.0f;
                fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
                func_L00_002594C8_o(
                    moby, pos, update_scratch.w.p.v70, 0.5f, 1.0f, 0.5f, 0.5235988f, 0);
            }
        }
        break;
    case BLARG_TROOPER_STATE_JUMP:
        if (moby->prev_seq != 2) {
            blend_moby_animation(moby, (void *)2, 0, (void *)scale_game_frames(7));
            *(float *)(data + 0xD0) = -(frame_time * 7.668f);
        } else if (moby->anim_flags & 2) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(0xA));
            moby->state = 1;
        }
        break;
    case BLARG_TROOPER_STATE_AIM: {
        float a =
            fast_arc_tan(update_scratch.v50[0] - moby->pos.x, update_scratch.v50[1] - moby->pos.y);
        FUN_L00_00258278(moby,
                         (float *)(data + 0xEC),
                         a,
                         frame_scale_sq * 0.03f,
                         frame_scale_sq * 0.3f,
                         frame_time * 5.585053443908691f);
    }
        if ((moby->anim_flags & 2) && moby->seq != 4) {
            blend_moby_animation(moby, (void *)4, 0, (void *)scale_game_frames(7));
        }
        if (!flag && target_distance > 2.5f && best > 1.0f) {
            if (target_distance > 14.0f) {
                blend_moby_animation(moby, (void *)0xA, 0, (void *)scale_game_frames(9));
                moby->state = 2;
                break;
            }
            if (target_distance < 8.0f && *(int *)(data + 0x108) == 0) {
                blend_moby_animation(moby, (void *)0xB, 0, (void *)scale_game_frames(5));
                moby->state = 3;
                break;
            }
        }
        if (target_distance < 2.5f) {
            moby->state = 7;
            blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
            break;
        }
        if (*(short *)(data + 0xF2) == 0 && target_distance < 14.0f &&
            absolute_float(moby->pos.z - update_scratch.tgt[2]) < 4.0f) {
            char *g = D_L04_00166E00;
            if (fast_difference_between_rotations(
                    *(float *)(g + 0x158),
                    fast_arc_tan(moby->pos.x - *(float *)(g + 0x140),
                                 moby->pos.y - *(float *)(g + 0x144))) < 0.61086524f &&
                fast_difference_between_rotations(
                    moby->rot.z,
                    fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                 update_scratch.v50[1] - moby->pos.y)) < 0.7853982f) {
                moby->state = 6;
                blend_moby_animation(moby, (void *)5, 0, (void *)scale_game_frames(7));
                *(short *)(data + 0xF2) = scale_game_frames(0x1E);
                *(unsigned char *)(data + 0xFD) = 3;
                break;
            }
        }
        if (target_distance > 14.0f) {
            moby->state = flag ? 3 : 1;
            blend_moby_animation(moby, (void *)6, 0, (void *)scale_game_frames(0xA));
            *(short *)(data + 0xF2) = scale_game_frames(0x1E);
        }
        break;
    case BLARG_TROOPER_STATE_FIRE: {
        char *o;
        {
            float a = fast_arc_tan(update_scratch.tgt[8] - moby->pos.x,
                                   update_scratch.tgt[9] - moby->pos.y);
            FUN_L00_00258278(moby,
                             (float *)(data + 0xEC),
                             a,
                             frame_scale_sq * 0.04f,
                             frame_scale_sq * 0.3f,
                             frame_time * 4.712389f);
        }
        if (target_distance < 2.5f) {
            moby->state = 7;
            blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
        }
        if (moby->prev_seq == 5 && *(float *)(data + 0xD8) < 2.0f && alive >= 2.0f &&
            (o = *(char **)(data + 0xF8)) != 0) {
            float a;
            moby_point_position(o, 0, update_scratch.w.p.v70);
            fast_vec_sub(update_scratch.w.p.v80, update_scratch.v50, update_scratch.w.p.v70);
            a = fast_arc_tan(update_scratch.w.p.v80[0], update_scratch.w.p.v80[1]);
            a = (fast_difference_between_rotations(a, moby->rot.z) > 0.2617994f) ? moby->rot.z : a;
            {
                float p = fast_arc_tan(vector_length_xy(update_scratch.w.p.v80),
                                       update_scratch.w.p.v80[2]);
                build_spherical_offset(update_scratch.w.p.v80, frame_time * 10.0f, a, p);
            }
            allocate_voice_for_target_entry(7, 0, moby);
            spawn_class_b8_projectile(moby,
                                      update_scratch.w.p.v70,
                                      update_scratch.w.p.v80,
                                      truncate_float_to_s32(14.0f / (frame_time * 10.0f)));
            FUN_L00_001fefc8(data + 0xFD);
        } else if ((moby->anim_flags & 2) && *(short *)(data + 0xF2) == 0 &&
                   *(unsigned char *)(data + 0xFD) == 0) {
            *(short *)(data + 0xF2) = scale_game_frames(0xB4);
            moby->state = 5;
            blend_moby_animation(moby, (void *)4, 0, (void *)scale_game_frames(0xA));
        }
        break;
    }
    case BLARG_TROOPER_STATE_MELEE_ATTACK: {
        float a =
            fast_arc_tan(update_scratch.v50[0] - moby->pos.x, update_scratch.v50[1] - moby->pos.y);
        FUN_L00_00258278(moby,
                         (float *)(data + 0xEC),
                         a,
                         frame_scale_sq * 0.04f,
                         frame_scale_sq * 0.3f,
                         frame_time * 5.585053443908691f);
    }
        if (moby->seq == moby->prev_seq && alive >= 12.0f && alive < 13.0f &&
            absolute_float(moby->pos.z - update_scratch.tgt[2]) < 2.0f &&
            vector_distance_xy(pos, update_scratch.v50) < 1.0f &&
            fast_difference_between_rotations(fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                                           update_scratch.v50[1] - moby->pos.y),
                                              moby->rot.z) < 0.2617994f) {
            update_scratch.w.p.blk.f0 = fast_cos(moby->rot.z) * 0.2f;
            update_scratch.w.p.blk.f4 = fast_sin(moby->rot.z) * 0.2f;
            update_scratch.w.p.blk.f8 = 0.0f;
            update_scratch.w.p.blk.p10 = moby;
            update_scratch.w.p.blk.v14 = 1;
            update_scratch.w.p.blk.f1C = 1.0f;
            set_vector_length_xy(&update_scratch.w.p.blk, &update_scratch.w.p.blk, 1.0f);
            update_scratch.w.p.blk.f8 = 1.0f;
            update_scratch.w.p.blk.fC = 5627.925f;
            update_scratch.w.p.blk.c19 = 1;
            update_scratch.w.p.blk.h1A = (u16)moby->oclass;
            update_scratch.w.p.blk.c18 = 0;
            qcopy(update_scratch.w.p.v70, update_scratch.v50);
            update_scratch.w.p.v70[2] = update_scratch.w.p.v70[2] + 0.75f;
            if (target != 0) {
                record_moby_hit(target, &update_scratch.w.p.blk);
            }
        }
        if (moby->anim_flags & 2) {
            moby->state = 5;
            *(short *)(data + 0xF2) = scale_game_frames(0x14);
            blend_moby_animation(moby, (void *)4, 0, (void *)scale_game_frames(7));
        }
        break;
    case BLARG_TROOPER_STATE_HIT_RECOVERY:
    case BLARG_TROOPER_STATE_BACKWARD_RECOVERY:
    case BLARG_TROOPER_STATE_KNOCKBACK_RECOVERY:
        if (*(float *)(data + 0xF4) > 0.0f) {
            update_scratch.w.p.v70[0] =
                fast_cos(fast_add_rotations(moby->rot.z, 3.1415927f)) * *(float *)(data + 0xF4);
            update_scratch.w.p.v70[1] =
                fast_sin(fast_add_rotations(moby->rot.z, 3.1415927f)) * *(float *)(data + 0xF4);
            update_scratch.w.p.v70[2] = 0.0f;
            fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
            *(float *)(data + 0xF4) =
                *(float *)(data + 0xF4) + -2.0f / (scale_time(15.0f) * scale_time(15.0f));
            func_L00_002594C8_o(moby, pos, update_scratch.w.p.v70, 0.5f, 1.0f, 0.5f, 0.5235988f, 1);
        }
        if (moby->anim_flags & 2) {
            *(unsigned short *)(data + 0xE8) ^= 1;
            blend_moby_animation(
                moby, (void *)(moby->state == 9 ? 9 : 0), 0, (void *)scale_game_frames(9));
            moby->state = 1;
            *(int *)(data + 0x108) = scale_game_frames(0x78);
        }
        break;
    case BLARG_TROOPER_STATE_DYING:
        if (mine_fall(moby, data + 0x70) & 0x60) {
            destroy_blarg_trooper(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, data + 0x60);
    {
        int current_state = moby->state;
        if (current_state == 1 || current_state == 3 || current_state == 4 || current_state == 5 ||
            current_state == 6) {
            char *t = target;
            if (t == 0) {
                t = *(char **)((char *)(&hero.moby));
            }
            if (vector_distance_xy(pos, (float *)(t + 0x10)) < 24.0f) {
                float f = fast_subtract_rotations(fast_arc_tan(*(float *)(t + 0x10) - moby->pos.x,
                                                               *(float *)(t + 0x14) - moby->pos.y),
                                                  moby->rot.z);
                if (f > 0.9599311f) {
                    f = 0.9599311f;
                } else if (f < -0.9599311f) {
                    f = -0.9599311f;
                }
                *(float *)(data + 0x178) = f * 0.5f;
                *(float *)(data + 0x1F8) = f * 0.5f;
            }
        }
    }
    if (moby->flags & 0x8000) {
        *(float *)(data + 0x178) = -*(float *)(data + 0x178);
        *(float *)(data + 0x1F8) = -*(float *)(data + 0x1F8);
    }
    update_joint_animation(moby, data + 0x110, 5, frame_scale_sq * 0.018f, frame_scale_sq * 0.3f);
    update_joint_animation(moby, data + 0x190, 6, frame_scale_sq * 0.024f, frame_scale_sq * 0.3f);
    if (*(char **)(data + 0xF8) != 0) {
        char *o;
        build_moby_bone_transform(moby, 1, (char *)update_scratch.w.m);
        o = *(char **)(data + 0xF8);
        qcopy(o + 0x10, update_scratch.w.m[3]);
        advance_moby_animation(o);
        update_entity(*(char **)(data + 0xF8));
        copy_matrix3x4(*(char **)(data + 0xF8) + 0xC0, update_scratch.w.m);
        normalize_vector_triplet(*(char **)(data + 0xF8) + 0xC0);
        {
            char *attached_gun_moby = *(char **)(data + 0xF8);
            if (moby->flags & 1) {
                MOBY(attached_gun_moby)->flags |= 0x41;
            } else {
                MOBY(attached_gun_moby)->flags &= 0xFFBE;
            }
        }
        refresh_moby_spatial_bounds_from_basis(*(char **)(data + 0xF8));
        *(unsigned short *)(*(char **)(data + 0xF8) + 0x34) |= 6;
    } else {
        moby_point_position(moby, 1, update_scratch.w.m);
        *(char **)(data + 0xF8) = spawn_moby_attachment(moby, update_scratch.w.m);
    }
    *(float *)(data + 0xD0) = *(float *)(data + 0xD0) + frame_time_sq * 9.8f;
    moby->pos.z = moby->pos.z - (*(float *)(data + 0xD0) - 2.0f);
    {
        float g = probe_ground_height(pos, 0, 0.5f);
        moby->pos.z = moby->pos.z - 2.0f;
        if (moby->pos.z < g) {
            moby->pos.z = g;
            *(float *)(data + 0xD0) = 0.0f;
        }
    }
    if (moby->pos.z < *(float *)(data + 0xD4) - 5.0f) {
        FUN_L00_00257470(moby, 0, -1);
        destroy_blarg_trooper(moby);
        return;
    }
    *(float *)(data + 0xD8) = alive;
}

#include "rnc/mobys/moby.h"

/* Pvars of a moby that follows a linked switch moby. */
typedef struct {
    s32 link;  /* index of the linked moby in the moby array, -1 for none */
    s32 voice; /* playing loop sound, -1 for none */
} SwitchFollowerVars;

/* The fields of a moby class header read when a moby changes class. */
typedef struct {
    u8  pad0[0xE];
    u8  unkE;
    u8  padF;
    u32 unk10;
    u8  pad14[0x10];
    f32 scale; /* 0x24 */
} MobyClassHeader;

extern char *moby_array __asm__("D_L04_0015FFD8");
extern u8 level04_moby_class_slot_by_oclass[] __asm__("D_L04_00197D40"); /* class slot by oclass */
extern struct MobyClass *D_L04_00197480[];                               /* class header by slot */
extern s32               play_moby_voice(s32, s32, struct Moby *) __asm__("FUN_0022da68");

/* Follows a linked switch moby (class 0x118): mirrors its position value, loops a sound while it moves, plays a click at either end and swaps class 0x41C back to 0x1B0 when it reaches 1.0. */
void eudora_bridge_update(struct Moby *m) __asm__("FUN_L04_002c6858");

void eudora_bridge_update(struct Moby *moby) {
    enum {
        EUDORA_BRIDGE_STATE_INITIALIZE = 0,
        EUDORA_BRIDGE_STATE_FOLLOW_LINKED_SWITCH = 1,
        EUDORA_BRIDGE_STATE_RELEASE_MOVEMENT_VOICE = 2
    };
    SwitchFollowerVars *switch_follower_vars = (SwitchFollowerVars *)moby->pvars;
    f32                 last = moby->anim_time;
    struct Moby        *other;
    f32                *linked_switch_travel;
    f32                 sampled_switch_travel;

    if (switch_follower_vars == NULL) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case EUDORA_BRIDGE_STATE_INITIALIZE:
        blend_moby_animation(moby, 1, 0, scale_game_frames(300));
        moby->state = 1;
        moby->anim_speed = 0;
        switch_follower_vars->voice = -1;
        if (moby->oclass == 0x1B0) {
            moby->oclass = 0x41C;
            moby->class_slot = level04_moby_class_slot_by_oclass[0x41C];
            moby->pclass = D_L04_00197480[moby->class_slot];
            moby->unk71 = 0xFF;
            moby->scale = ((MobyClassHeader *)moby->pclass)->scale;
            update_moby_animation_state(moby);
            moby->unk72 = ((MobyClassHeader *)moby->pclass)->unkE;
            moby->collision = ((MobyClassHeader *)moby->pclass)->unk10;
            refresh_moby_spatial_bounds_from_basis(moby);
        }
        break;
    case EUDORA_BRIDGE_STATE_FOLLOW_LINKED_SWITCH:
        if (switch_follower_vars->link == -1)
            break;
        other = (struct Moby *)(moby_array + (switch_follower_vars->link << 8));
        if (other->oclass != 0x118)
            break;
        linked_switch_travel = (f32 *)other->pvars;
        sampled_switch_travel = *linked_switch_travel;
        moby->anim_time = sampled_switch_travel;
        if (sampled_switch_travel != last) {
            if (*linked_switch_travel == 1.0f || *linked_switch_travel == 0.0f) {
                if (voice_is_playing(moby, switch_follower_vars->voice)) {
                    s32 h = switch_follower_vars->voice;
                    if (h != -1) {
                        char *e = (char *)D_0013E550_u + h * 0x70;
                        if (*(struct Moby **)(e + 0x88) == moby && *(u8 *)(e + 0x74) != 0)
                            release_voice_slot(h);
                    }
                    switch_follower_vars->voice = -1;
                }
                play_moby_voice(1, 0, moby);
                if (moby->oclass == 0x41C && *linked_switch_travel == 1.0f) {
                    moby->oclass = 0x1B0;
                    moby->class_slot = level04_moby_class_slot_by_oclass[0x1B0];
                    moby->pclass = D_L04_00197480[moby->class_slot];
                    moby->unk71 = 0xFF;
                    moby->scale = ((MobyClassHeader *)moby->pclass)->scale;
                    update_moby_animation_state(moby);
                    moby->unk72 = ((MobyClassHeader *)moby->pclass)->unkE;
                    moby->collision = ((MobyClassHeader *)moby->pclass)->unk10;
                    refresh_moby_spatial_bounds_from_basis(moby);
                    moby->state = 2;
                }
            } else if (!voice_is_playing(moby, switch_follower_vars->voice)) {
                switch_follower_vars->voice = play_moby_voice(0, 4, moby);
            }
        } else {
            if (voice_is_playing(moby, switch_follower_vars->voice)) {
                s32 h = switch_follower_vars->voice;
                if (h != -1) {
                    char *e = (char *)D_0013E550_u + h * 0x70;
                    if (*(struct Moby **)(e + 0x88) == moby && *(u8 *)(e + 0x74) != 0)
                        release_voice_slot(h);
                }
                switch_follower_vars->voice = -1;
            }
        }
        break;
    case EUDORA_BRIDGE_STATE_RELEASE_MOVEMENT_VOICE:
        if (voice_is_playing(moby, switch_follower_vars->voice)) {
            s32 h = switch_follower_vars->voice;
            if (h != -1) {
                char *e = (char *)D_0013E550_u + h * 0x70;
                if (*(struct Moby **)(e + 0x88) == moby && *(u8 *)(e + 0x74) != 0)
                    release_voice_slot(h);
            }
            switch_follower_vars->voice = -1;
        }
        break;
    }
}

#include "rnc/mobys/moby.h"

/* Pvars of a door driven by a lever (platform_booster_update update): follows the lever's travel and plays its sounds. */
typedef struct {
    s32 lever;  /* 0x00: moby index of the lever, -1 for none */
    s32 target; /* 0x04: moby index it carries along, -1 for none */
    s32 voice;  /* 0x08: looping sound, -1 for none */
    s32 locked; /* 0x0C */
} DoorVars;

/* Pvars of the lever (oclass 0x118). */
typedef struct {
    f32 travel; /* 0x00: 0..1 */
    u8  pad4[0x14];
    f32 spin; /* 0x18 */
    u8  pad1C[0x14];
    f32 rate; /* 0x30 */
} LeverVars;

extern int          FUN_L00_0028d8c0(void *, int);
extern int          start_moby_sound(int, int, int) __asm__("FUN_0022da68");
extern struct Moby *level_moby_table __asm__("D_L04_0015FFD8") MACRO_ADDR;
extern struct Moby *level_mobys __asm__("D_L04_0015FFD8");
extern u8           voice_slots[] __asm__("D_0013E550");

/* Door update: follows its lever's travel, pushes the lever back while opening, and runs the motor sounds. */
void platform_booster_update(struct Moby *m) __asm__("FUN_L04_002c6bb8");

void platform_booster_update(struct Moby *moby) {
    enum {
        PLATFORM_BOOSTER_STATE_INITIALIZE = 0,
        PLATFORM_BOOSTER_STATE_FOLLOW_LEVER = 1,
        PLATFORM_BOOSTER_STATE_ADVANCE_ANIMATION = 2
    };
    DoorVars    *door_vars = (DoorVars *)moby->pvars;
    f32          previous_anim_time;
    struct Moby *lever;
    LeverVars   *lv;
    u8          *slot;

    previous_anim_time = moby->anim_time;
    if (door_vars == NULL) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case PLATFORM_BOOSTER_STATE_INITIALIZE:
        blend_moby_animation(moby, 1, 0, scale_game_frames(300));
        moby->state = 1;
        moby->anim_speed = 0;
        door_vars->voice = -1;
        break;
    case PLATFORM_BOOSTER_STATE_FOLLOW_LEVER:
        if (door_vars->lever == -1) {
            break;
        }
        lever = &level_mobys[door_vars->lever];
        if (lever->oclass != 0x118) {
            break;
        }
        lv = (LeverVars *)lever->pvars;
        if (lever->state == 3) {
            moby->anim_time = 1.0f - lv->travel;
        } else if (lv->travel != 1.0f ||
                   (door_vars->target != -1 && door_vars->locked == 0 &&
                    ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9 ||
                     hero.state.control_mode == 12) &&
                    hero.ground_moby == &level_mobys[door_vars->target])) {
            moby->state = 2;
        }
        if (moby->anim_time != previous_anim_time) {
            if (lv->travel != 1.0f && lv->travel != 0.0f) {
                if (FUN_L00_0028d8c0(moby, door_vars->voice) == 0) {
                    door_vars->voice = start_moby_sound(0, 4, (int)moby);
                }
            } else {
                if (FUN_L00_0028d8c0(moby, door_vars->voice) != 0) {
                    if (door_vars->voice != -1) {
                        slot = voice_slots + door_vars->voice * 0x70;
                        if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0) {
                            release_voice_slot(door_vars->voice);
                        }
                    }
                    door_vars->voice = -1;
                }
                start_moby_sound(1, 0, (int)moby);
            }
        } else if (FUN_L00_0028d8c0(moby, door_vars->voice) != 0) {
            if (door_vars->voice != -1) {
                slot = voice_slots + door_vars->voice * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0) {
                    release_voice_slot(door_vars->voice);
                }
            }
            door_vars->voice = -1;
        }
        break;
    case PLATFORM_BOOSTER_STATE_ADVANCE_ANIMATION: {
        struct Moby *link;
        LeverVars   *link_vars;

        moby->anim_time = previous_anim_time + frame_time * 0.5f;
        if (1.0f < moby->anim_time) {
            moby->anim_time = 1.0f;
        }
        if (door_vars->lever == -1) {
            break;
        }
        link = &level_moby_table[door_vars->lever];
        if (link->oclass != 0x118) {
            break;
        }
        link_vars = (LeverVars *)link->pvars;
        if (link->state == 3) {
            moby->anim_time = 1.0f - link_vars->travel;
            moby->state = 1;
        } else {
            link_vars->travel = 1.0f - moby->anim_time;
            link_vars->spin = link_vars->travel * (link_vars->rate * 6.2831855f);
        }
        if (moby->anim_time != previous_anim_time) {
            if (link_vars->travel == 1.0f || link_vars->travel == 0.0f) {
                if (FUN_L00_0028d8c0(moby, door_vars->voice) != 0) {
                    if (door_vars->voice != -1) {
                        slot = voice_slots + door_vars->voice * 0x70;
                        if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0) {
                            release_voice_slot(door_vars->voice);
                        }
                    }
                    door_vars->voice = -1;
                }
                start_moby_sound(1, 0, (int)moby);
            } else if (FUN_L00_0028d8c0(moby, door_vars->voice) == 0) {
                door_vars->voice = start_moby_sound(0, 4, (int)moby);
            }
        } else if (FUN_L00_0028d8c0(moby, door_vars->voice) != 0) {
            if (door_vars->voice != -1) {
                slot = voice_slots + door_vars->voice * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0) {
                    release_voice_slot(door_vars->voice);
                }
            }
            door_vars->voice = -1;
        }
        break;
    }
    }
    if (door_vars->target != -1) {
        moby_point_position(moby, 0, &level_moby_table[door_vars->target].pos);
    }
}
