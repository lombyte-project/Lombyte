/* Shared moby class morphoray_chicken: the chicken the morph-o-ray turns enemies into, and its suck cannon methods. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/math/fast_math.h"

typedef union {
    u128 q;
    f32  f[4];
} V4;

typedef struct {
    u8    pad00[0x70];
    void *unk70;
    u8    pad74[0xB0 - 0x74];
} Node;

typedef struct {
    u8    pad00[0x10];
    void *unk10;
    void *unk14;
    u8    pad18[0x64 - 0x18];
    u16   unk64;
    u16   unk66;
    u8    pad68[0x70 - 0x68];
    s32   unk70;
    u32   unk74;
    f32   unk78;
} Sub;

/* Class header view: pclass + 0xE and + 0x10. */
typedef struct {
    u8  pad00[0xE];
    u8  unkE;
    u8  pad0F;
    u32 unk10;
} Part;

typedef struct {
    V4  v;
    f32 t1;
    f32 t2;
    f32 t3;
} Loc;

typedef struct {
    u8 pad00[4];
    u8 f4;
    u8 f5;
    u8 f6;
} Opt;

typedef struct {
    u8   pad00[0xC];
    Opt *unk0C;
} Holder;

typedef struct {
    u8      pad00[0x10];
    V4      pos;
    u8      pad20[0x34 - 0x20];
    u16     flags34;
    u8      pad36[0x38 - 0x36];
    u64     unk38;
    u8      pad40[0x78 - 0x40];
    Holder *unk78;
} State;

typedef struct {
    u8  pad00[0x28];
    f32 f28;
    u8  pad2C[0x40 - 0x2C];
    V4  v40;
} Data;

typedef struct Moby Moby;

/* Four floats aligned to 16 bytes, the layout moved by one lq/sq. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) Vec4A;

/* find_moby_hit result: hit moby in the low word of 0x20, flags in the high word. */
typedef struct {
    u8 pad00[0x20];
    union {
        u64 bits;
        struct {
            struct Moby *moby;
            u32          flags;
        } s;
    } hit;
    u8  pad28[0x2C - 0x28];
    f32 damage;
} Contact;

/* Vars of a morph-o-ray chicken, set up by FUN_L00_002c9348. */
typedef struct {
    u8    pad0[0x10];
    void *unk10;
    char *unk14;
    u8    pad18[8];
    u8    manipulator[0x44]; /* 0x20: scaled by FUN_L00_00263ac8 */
    short sound_timer;       /* 0x64 */
    short flee_timer;        /* 0x66 */
    float turn_speed;        /* 0x68 */
    float fall_speed;        /* 0x6C */
    int   expire_timer;      /* 0x70: -1 keeps the chicken */
    int   gold;              /* 0x74: made by the gold morph-o-ray: grows, explodes when it dies */
    float health;            /* 0x78 */
    int   hit_cooldown;      /* 0x7C */
} ChickenVars;

extern CollisionHit coll_hit __asm__("D_L00_00173E40") NOT_SDA;
extern void         fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");

extern u8           D_0013E520[];
extern Data         D_L00_00173E40;
extern struct Moby *D_L00_001DD100[];
extern Node         l00_morphoray_chicken_slot_records[] __asm__("D_L00_001DD150");
extern u8           morphoray_chicken_slot_buffers[] __asm__("D_L00_001DDF10");
extern s32          D_L00_00161854 __attribute__((sda));
extern s32          D_L00_00161854_gp __asm__("D_L00_00161854") __attribute__((sda));
extern u32          D_L00_00161840[1] __attribute__((sda));

extern s32  FUN_L00_00257b90(s32, s32);
extern void FUN_L00_00257470(void *, int, int);
extern void set_moby_color(void *, u8, u8, u8) __asm__("FUN_L00_002502f0");
extern void spawn_particle(void *, int, int, int, int, f32, f32) __asm__("FUN_L00_00269b70");
extern void fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");
extern void FUN_L00_002c92d8_c(void *) __asm__("FUN_L00_002c92d8");
extern void update_entity(void *) __asm__("FUN_L00_00250df8");

extern int  collision_hit_material(void) __asm__("FUN_001f0b58");
extern void blend_moby_animation(Moby *, int, int, int) __asm__("FUN_00212f90");

extern void FUN_L00_00263ac8(void *, int, void *, float);
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");
extern int FUN_L00_001f2868(void *, float, int, void *, void *);
void       spawn_explosion(char         *m,
                           void         *unused,
                           char         *pos,
                           float         fA,
                           float         fB,
                           int           n,
                           int           k8,
                           int           k9,
                           float         fC,
                           float         fD,
                           float         fE,
                           float         fF,
                           int           k10,
                           float         fG,
                           int           k11,
                           int           s16,
                           int           s18,
                           unsigned char flag) __asm__("FUN_L00_0025e450");
extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
unsigned char *spawn_feather_moby(int unused, void *pos, float *vel) __asm__("FUN_L00_002cbf68");
extern int     FUN_L00_00258490(void *, void *, void *, int, float, float, float, float);
extern void    FUN_L00_00258278(void *, float *, float, float, float, float);
extern float   FUN_L00_00259710(void *);
extern int     FUN_L00_002dc9f0(void *, void *);
extern float   D_L00_0015F5D0;
extern float   D_L00_0015F5D4;
extern int     chicken_manipulator_joint __asm__("D_L00_00161850") __attribute__((sda));
float          feather_pitch_deg __asm__("D_L00_00161858") = 7e+01f;
float          feather_speed_min __asm__("D_L00_0016185C") = 5.0f;
float          feather_speed_max __asm__("D_L00_00161860") = 9.0f;
float          feather_height __asm__("D_L00_00161864") = 0.25f;
float          feather_radius_min __asm__("D_L00_00161868") = 0;
float          feather_radius_max __asm__("D_L00_0016186C") = 0.35f;
extern char    D_L00_00177F00[];

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

extern char *get_moby_suck_cannon_record(char *) __asm__("FUN_L00_002db890");
extern int   launch_suck_cannon_projectile(char *, int, int, float) __asm__("FUN_L00_002dbc30");

extern int   cancel_suck_cannon_suction(void *) __asm__("FUN_L00_002dbe20");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");

extern unsigned char D_0013E535_lui __asm__("D_0013E535") NOT_SDA;
int                  FUN_001160d8_2cbf68(void) __asm__("FUN_001160d8");
void                 update_entity(void *) __asm__("FUN_L00_00250df8");
void                 copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");

int   feather_base_lifetime_gp __asm__("D_L00_001618A4") = 0xf0;
float feather_scale_multiplier_gp __asm__("D_L00_001618A0") = 9.0f;

/* 0x002c92d8, 112 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// resets the fields of a record (if non-null) to their defaults
/* Fills the block from 0x78 with fixed defaults and -1 ids. */

void reset_morphoray_chicken_slot_defaults(char *record) __asm__("FUN_L00_002c92d8");

void reset_morphoray_chicken_slot_defaults(char *record) {
    if (record != 0) {
        *(int *)(record + 0x78) = 0;
        *(float *)(record + 0x7C) = 0.05f;
        *(float *)(record + 0x80) = 0.863f;
        *(int *)(record + 0x84) = 4;
        *(int *)(record + 0x88) = 3;
        *(float *)(record + 0x8C) = 0.625f;
        *(short *)(record + 0x9C) = -1;
        *(short *)(record + 0x9E) = -1;
        *(int *)(record + 0xA0) = -1;
        *(int *)(record + 0xA4) = -1;
        *(int *)(record + 0xA8) = -1;
        *(int *)(record + 0xAC) = -1;
    }
}

struct Moby *spawn_morphoray_chicken(State *source_state) __asm__("FUN_L00_002c9348");

struct Moby *spawn_morphoray_chicken(State *source_state) {
    struct Moby *spawned_moby;
    Sub         *moby_vars;
    Loc          loc;
    struct Moby *previous_slot_moby;
    void        *source_record;
    Opt         *color_options;
    f32          ground_height_pitch_and_variant_scratch;
    f32          yaw_cosine_scratch;
    f32          sn;

    spawned_moby = (struct Moby *)create_moby(0x10E);
    if (spawned_moby != NULL) {
        spawned_moby->update_dist = 0x7F;
        spawned_moby->draw_dist = 0x7F;
        spawned_moby->visible = 1;
        moby_vars = (Sub *)spawned_moby->pvars;
        if ((source_state->flags34 & 0x20) != 0) {
            color_options = source_state->unk78->unk0C;
            if (color_options != NULL) {
                set_moby_color(
                    spawned_moby, color_options->f4, color_options->f5, color_options->f6);
            } else {
                spawned_moby->spawn_frame = source_state->unk38;
            }
        } else {
            spawned_moby->spawn_frame = source_state->unk38;
        }
        spawned_moby->anim_speed = random_float_between(0.85f, 1.15f);
        qcopy(&spawned_moby->pos, &source_state->pos);
        clear_u64_value(&spawned_moby->rot);
        spawned_moby->rot.z = fast_arc_tan(spawned_moby->pos.x - hero.motion.pos.f[0],
                                           spawned_moby->pos.y - hero.motion.pos.f[1]);
        ground_height_pitch_and_variant_scratch = probe_ground_height(&spawned_moby->pos, 0, 0.5f);
        if (spawned_moby->pos.z <= ground_height_pitch_and_variant_scratch) {
            spawned_moby->pos.z = ground_height_pitch_and_variant_scratch;
            spawned_moby->pos.z = D_L00_00173E40.f28;
            qcopy(&loc.v, (u8 *)&D_L00_00173E40 + 0x40);
            yaw_cosine_scratch = fast_cos(spawned_moby->rot.z);
            sn = fast_sin(spawned_moby->rot.z);
            loc.t1 = loc.v.f[0] * yaw_cosine_scratch + loc.v.f[1] * sn;
            yaw_cosine_scratch = fast_cos(spawned_moby->rot.z);
            sn = fast_sin(spawned_moby->rot.z);
            loc.t2 = loc.v.f[1] * yaw_cosine_scratch - loc.v.f[0] * sn;
            loc.t3 = loc.v.f[2];
            ground_height_pitch_and_variant_scratch =
                fast_arc_tan(square_root(loc.t1 * loc.t1 + loc.t3 * loc.t3), loc.t2);
            spawned_moby->rot.x = -ground_height_pitch_and_variant_scratch;
            spawned_moby->rot.y = fast_arc_tan(loc.t3, loc.t1);
        }
        spawned_moby->collision = ((Part *)spawned_moby->pclass)->unk10;
        allocate_voice_for_target_entry(1, 0, spawned_moby);
        spawned_moby->state = 1;
        moby_vars->unk66 = 0;
        moby_vars->unk64 = FUN_L00_00257b90(scale_game_frames(0x78), scale_game_frames(0x12C));
        ground_height_pitch_and_variant_scratch = convert_integer_to_float(D_0013E520[0x15]);
        moby_vars->unk70 = -1;
        moby_vars->unk78 = ground_height_pitch_and_variant_scratch * 3.0f + 1.0f;
        moby_vars->unk74 = D_0013E520[0x15];
        spawned_moby->command = moby_vars->unk74;
        FUN_L00_00257470(source_state, 0, -1);
        source_record = get_moby_target_info(source_state);
        if (source_record != NULL && *(u8 *)((u8 *)source_record + 0xE) == 1) {
            *(u8 *)((u8 *)source_record + 0xE) = 2;
        } else {
            mark_moby_for_removal(source_state);
        }
        yaw_cosine_scratch = 0.0f;
        spawn_particle(&spawned_moby->pos,
                       0x7F,
                       0x7F,
                       0x7F,
                       scale_game_frames(0xF),
                       1000000.0f,
                       yaw_cosine_scratch);
        fill_transfer_words(moby_vars, 0, 0x20);
        previous_slot_moby = D_L00_001DD100[D_L00_00161854];
        if (previous_slot_moby != NULL && previous_slot_moby->oclass == 0x10E &&
            previous_slot_moby->state != 0xFE && previous_slot_moby->state != 0xFD) {
            if (previous_slot_moby->state == 5) {
                moby_vars->unk78 = yaw_cosine_scratch;
                spawned_moby->flags &= ~0x20;
            } else if (!moby_vars->unk74) {
                moby_vars->unk14 = (void *)&l00_morphoray_chicken_slot_records[D_L00_00161854];
                moby_vars->unk10 = (void *)(morphoray_chicken_slot_buffers + D_L00_00161854 * 0x60);
                spawned_moby->flags |= 0x1020;
                ((Node *)moby_vars->unk14)->unk70 = (void *)D_L00_00161840;
            }
            ((Sub *)D_L00_001DD100[D_L00_00161854]->pvars)->unk70 = (void *)1;
        } else if (!moby_vars->unk74) {
            moby_vars->unk14 = (void *)&l00_morphoray_chicken_slot_records[D_L00_00161854];
            moby_vars->unk10 = (void *)(morphoray_chicken_slot_buffers + D_L00_00161854 * 0x60);
            spawned_moby->flags |= 0x1020;
            ((Node *)moby_vars->unk14)->unk70 = (void *)D_L00_00161840;
        }
        FUN_L00_002c92d8_c(moby_vars->unk14);
        D_L00_001DD100[D_L00_00161854] = spawned_moby;
        D_L00_00161854_gp = D_L00_00161854 + 1;
        D_L00_00161854 = (D_L00_00161854 + 1) % 20;
        if (moby_vars->unk74) {
            spawned_moby->flags &= ~0x20;
        }
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

/* Update for moby class 270 (morphoray chicken): wanders, pecks and flees from the hero, grows, and bursts into feathers when its health runs out. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CABC0), where it is exact; names translated to the US level program. */

/* Argument order as the chicken passes them: the order of their definitions changes the code. */

void update_morphoray_chicken(Moby *chicken_moby) __asm__("FUN_L00_002c97e0");

void update_morphoray_chicken(Moby *chicken_moby) {
    enum {
        CHICKEN_STATE_INITIALIZE = 0,
        CHICKEN_STATE_IDLE = 1,
        CHICKEN_STATE_TURN_AWAY_FROM_HERO = 2,
        CHICKEN_STATE_FLEE_FROM_HERO = 3,
        CHICKEN_STATE_CLOSE_RANGE_FLEE_FROM_HERO = 4,
        CHICKEN_STATE_EMIT_FEATHERS = 5
    };
    Vec4A        next_pos;
    Vec4A        ahead;
    Vec4A        motion_and_normal_scratch;
    Vec4A        point;
    ChickenVars *vars;
    struct Hero *player;
    float        anim_speed;
    float        flee_angle;
    float        size;
    float        speed;
    float        radius;
    float        angle;
    int          path_clear;
    int          i;
    int          j;
    int          count;
    Contact     *hit;

    path_clear = 0;
    player = &hero;
    vars = (ChickenVars *)chicken_moby->pvars;
    anim_speed = chicken_moby->anim_speed;
    flee_angle = fast_arc_tan(chicken_moby->pos.x - player->motion.pos.f[0],
                              chicken_moby->pos.y - player->motion.pos.f[1]);
    if (chicken_moby->state == 0) {
        chicken_moby->state = 1;
        vars->flee_timer = 0;
        vars->expire_timer = -1;
        vars->sound_timer = random_int_between(scale_game_frames(0x78), scale_game_frames(0x12C));
        vars->health = 1.0f;
    }
    if (chicken_moby->state == 6) {
        return;
    }
    if (D_0013E520[0x15] != 0) {
        FUN_L00_00263ac8(chicken_moby, chicken_manipulator_joint, vars->manipulator, 5.0f);
    } else {
        FUN_L00_00263ac8(chicken_moby, chicken_manipulator_joint, vars->manipulator, 7.7f);
    }
    qcopy(&next_pos, &chicken_moby->pos);
    if (vars->expire_timer >= 0 && fast_dec_timer(&vars->expire_timer) != 0 &&
        chicken_moby->visible == 0) {
        mark_moby_for_removal(chicken_moby);
        return;
    }
    if (vars->expire_timer >= 0) {
        if ((0.9f < D_L00_0015F5D4 || 0.9f < D_L00_0015F5D0) && chicken_moby->visible == 0 &&
            vars->expire_timer != -1) {
            if (random_integer_below(0xC7) == 0) {
                mark_moby_for_removal(chicken_moby);
                return;
            }
            vars->expire_timer = random_int_between(0x14, 0x1E);
        }
        if (1.0f < D_L00_0015F5D4 || 1.0f < D_L00_0015F5D0) {
            if (chicken_moby->visible != 0) {
                if (random_integer_below(0x63) == 0) {
                    vars->health = 0.0f;
                } else {
                    vars->expire_timer = random_int_between(0x14, 0x1E);
                }
            } else if (random_integer_below(0x18) == 0) {
                mark_moby_for_removal(chicken_moby);
                return;
            }
        }
    }
    if (vars->gold > 0) {
        chicken_moby->scale +=
            ((convert_integer_to_float(vars->gold) * 3.0f + 1.0f) * chicken_moby->pclass->scale -
             chicken_moby->scale) *
            0.1f;
    }
    size = chicken_moby->scale / chicken_moby->pclass->scale;
    if (fast_dec_timer(&vars->hit_cooldown) != 0) {
        hit = find_moby_hit(chicken_moby, -1, 0);
        if (hit != 0) {
            allocate_voice_for_target_entry(1, 0, (char *)chicken_moby);
            vars->health -= hit->damage;
            vars->hit_cooldown = scale_game_frames(0x19);
        }
    }
    chicken_moby->hit_slot = 0xFF;
    if (vars->health < 1.0f) {
        if (vars->gold == 0) {
            for (i = 0; i < truncate_float_to_s32(size) * 20; i++) {
                radius = random_float_between(0.0f, 0.37f) * size;
                angle = random_angle_radians();
                build_spherical_offset(&point, radius, angle, random_angle_radians());
                point.z += random_float_between(0.25f, 0.5f) * size;
                fast_vec_add(&point, &point, &next_pos);
                radius = random_float_between(frame_time * 0.1f, frame_time) * size;
                angle = random_angle_radians();
                build_spherical_offset(
                    &motion_and_normal_scratch, radius, angle, random_angle_radians());
                spawn_effect_particle_with_target_scalar(
                    &point,
                    &motion_and_normal_scratch,
                    0x3F7F7F7F,
                    0x2F2F2F,
                    size * 150000.0f,
                    random_int_between(scale_game_frames(0x1E), scale_game_frames(0x39)));
            }
        } else {
            *(u128 *)&point = 0;
            point.x = frame_scale * 0.5f;
            point.z = frame_scale * 0.7f;
            point.y = frame_scale * 0.5f;
            *(u128 *)&motion_and_normal_scratch = *(u128 *)&point;
            spawn_explosion((char *)chicken_moby,
                            &motion_and_normal_scratch,
                            0,
                            0.0f,
                            2.0f,
                            0,
                            0,
                            0,
                            4.0f,
                            3.0f,
                            9.0f,
                            1.0f,
                            -1,
                            12.0f,
                            1,
                            1,
                            -1,
                            (unsigned char)vars->gold);
            count = FUN_L00_001f2868(&chicken_moby->pos, 3.0f, 0x10, chicken_moby, 0);
            *(u128 *)&point = *(u128 *)&chicken_moby->pos;
            send_blast_hits(
                chicken_moby, &point, D_L00_00177F00, count, 0, 0x830000, 2, 1, 1.0f, 1.0f, 1.0f);
        }
        count = random_int_between(5, 8);
        if (0.85f < D_L00_0015F5D0 || 0.85f < D_L00_0015F5D4) {
            if (1.0f < D_L00_0015F5D0 || 1.0f < D_L00_0015F5D4) {
                count = 1;
            } else {
                count /= 2;
            }
        }
        for (j = 0; j < count; j++) {
            radius = random_float_between(feather_radius_min, feather_radius_max) * size;
            angle = random_angle_radians();
            build_spherical_offset(&point, radius, angle, random_angle_radians());
            point.z += feather_height * size;
            fast_vec_add(&point, &point, &next_pos);
            angle = random_float_between(feather_speed_min * frame_time,
                                         feather_speed_max * frame_time);
            build_spherical_offset(&motion_and_normal_scratch,
                                   angle,
                                   random_angle_radians(),
                                   feather_pitch_deg * 0.017453292f);
            motion_and_normal_scratch.z = random_float_between(3.5f, 5.5f) * frame_time;
            spawn_feather_moby((int)chicken_moby, &point, &motion_and_normal_scratch.x);
        }
        allocate_voice_for_target_entry(1, 0, (char *)chicken_moby);
        if (vars->expire_timer >= 0) {
            mark_moby_for_removal(chicken_moby);
            return;
        }
        chicken_moby->collision = 0;
        chicken_moby->flags = (chicken_moby->flags & 0xEFFF) | 0x41;
        vars->gold = 0;
        chicken_moby->state = 6;
        chicken_moby->command = 0;
        return;
    }
    if (fast_dec_timer_s16(&vars->sound_timer) != 0) {
        allocate_voice_for_target_entry(random_integer_below(100) >= 75, 0, (char *)chicken_moby);
        vars->sound_timer = random_int_between(scale_game_frames(0x78), scale_game_frames(0x12C));
    }
    if (vars->expire_timer >= 0) {
        speed = (frame_time + frame_time) * size;
    } else {
        speed = 0.333f;
    }
    switch (chicken_moby->state) {
    case CHICKEN_STATE_INITIALIZE:
        break;
    case CHICKEN_STATE_IDLE:
        if (chicken_moby->anim_flags & 2) {
            blend_moby_animation(
                chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
        }
        if (fast_dec_timer_s16(&vars->flee_timer) != 0 &&
            vector_distance_xy(&chicken_moby->pos, &hero.motion.pos) < size * 5.0f) {
            ahead.x = fast_cos(chicken_moby->rot.z) * frame_time * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * frame_time * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(
                chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear != 0 ||
                (fast_difference_between_rotations(chicken_moby->rot.z,
                                                   fast_arc_tan(next_pos.x - chicken_moby->pos.x,
                                                                next_pos.y - chicken_moby->pos.y)) <
                     1.5707964f &&
                 fast_vec_dist(&chicken_moby->pos, &next_pos) > size * 0.01f)) {
                chicken_moby->state = 3;
                blend_moby_animation(chicken_moby, 3, 0, scale_game_frames(10));
            } else {
                vars->flee_timer =
                    random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
            }
        } else if (vector_distance_xy(&chicken_moby->pos, &hero.motion.pos) < size * 5.0f &&
                   0.5235988f <
                       fast_difference_between_rotations(flee_angle, chicken_moby->rot.z)) {
            FUN_L00_00258490(
                chicken_moby, &next_pos, &next_pos, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            chicken_moby->state = 2;
            blend_moby_animation(chicken_moby, 3, 0, scale_game_frames(10));
        } else {
            FUN_L00_00258490(
                chicken_moby, &next_pos, &next_pos, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
        }
        break;
    case CHICKEN_STATE_TURN_AWAY_FROM_HERO:
        FUN_L00_00258278(chicken_moby,
                         &vars->turn_speed,
                         flee_angle,
                         frame_time_sq * 0.7853982f,
                         frame_time_sq * 1.5707964f,
                         frame_time * 0.7853982f);
        FUN_L00_00258490(
            chicken_moby, &next_pos, &next_pos, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
        if (fast_dec_timer_s16(&vars->flee_timer) != 0 &&
            vector_distance_xy(&chicken_moby->pos, &hero.motion.pos) < size * 5.0f) {
            ahead.x = fast_cos(chicken_moby->rot.z) * frame_time * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * frame_time * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            if (absolute_float(probe_ground_height(&ahead, 0, 0.5f) - ahead.z) <=
                (frame_time + frame_time) * size) {
                chicken_moby->state = 3;
                blend_moby_animation(chicken_moby, 3, 0, scale_game_frames(10));
            }
        } else if (fast_difference_between_rotations(flee_angle, chicken_moby->rot.z) <
                       0.08726646f ||
                   vector_distance_xy(&chicken_moby->pos, &hero.motion.pos) > size * 6.0f) {
            chicken_moby->state = 1;
            vars->flee_timer = random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
            blend_moby_animation(
                chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
        }
        break;
    case CHICKEN_STATE_FLEE_FROM_HERO:
        if (vector_distance_xy(&chicken_moby->pos, &hero.motion.pos) > size * 6.0f) {
            chicken_moby->state = 1;
            vars->flee_timer = random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
            blend_moby_animation(
                chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            FUN_L00_00258490(
                chicken_moby, &next_pos, &next_pos, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
        } else if (vector_distance_xy(&chicken_moby->pos, &hero.motion.pos) < size * 3.0f) {
            FUN_L00_00258278(chicken_moby,
                             &vars->turn_speed,
                             flee_angle,
                             frame_time_sq * 0.7853982f,
                             frame_time_sq * 1.5707964f,
                             frame_time * 0.7853982f);
            ahead.x = fast_cos(chicken_moby->rot.z) * (frame_time + frame_time);
            ahead.y = fast_sin(chicken_moby->rot.z) * (frame_time + frame_time);
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(
                chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear == 0 &&
                (1.5707964f < fast_difference_between_rotations(
                                  chicken_moby->rot.z,
                                  fast_arc_tan(next_pos.x - chicken_moby->pos.x,
                                               next_pos.y - chicken_moby->pos.y)) ||
                 fast_vec_dist(&chicken_moby->pos, &next_pos) < size * 0.01f)) {
                chicken_moby->state = 1;
                vars->flee_timer =
                    random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
                blend_moby_animation(
                    chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            } else {
                chicken_moby->state = 4;
                blend_moby_animation(chicken_moby, 2, 0, scale_game_frames(10));
            }
        } else {
            FUN_L00_00258278(chicken_moby,
                             &vars->turn_speed,
                             flee_angle,
                             frame_time_sq * 0.7853982f,
                             frame_time_sq * 1.5707964f,
                             frame_time * 0.7853982f);
            ahead.x = fast_cos(chicken_moby->rot.z) * frame_time * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * frame_time * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(
                chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear == 0 &&
                (1.5707964f < fast_difference_between_rotations(
                                  chicken_moby->rot.z,
                                  fast_arc_tan(next_pos.x - chicken_moby->pos.x,
                                               next_pos.y - chicken_moby->pos.y)) ||
                 fast_vec_dist(&chicken_moby->pos, &next_pos) < size * 0.01f)) {
                chicken_moby->state = 1;
                vars->flee_timer =
                    random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
                blend_moby_animation(
                    chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            }
        }
        break;
    case CHICKEN_STATE_CLOSE_RANGE_FLEE_FROM_HERO:
        if (fast_dec_timer_s16(&vars->flee_timer) != 0 &&
            vector_distance_xy(&chicken_moby->pos, &hero.motion.pos) > size * 4.0f) {
            FUN_L00_00258278(chicken_moby,
                             &vars->turn_speed,
                             flee_angle,
                             frame_time_sq * 0.7853982f,
                             frame_time_sq * 1.5707964f,
                             frame_time * 0.7853982f);
            ahead.x = fast_cos(chicken_moby->rot.z) * frame_time * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * frame_time * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(
                chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear == 0 &&
                (1.5707964f < fast_difference_between_rotations(
                                  chicken_moby->rot.z,
                                  fast_arc_tan(next_pos.x - chicken_moby->pos.x,
                                               next_pos.y - chicken_moby->pos.y)) ||
                 fast_vec_dist(&chicken_moby->pos, &next_pos) < size * 0.01f)) {
                chicken_moby->state = 1;
                vars->flee_timer =
                    random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
                blend_moby_animation(
                    chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            } else {
                chicken_moby->state = 3;
                blend_moby_animation(chicken_moby, 3, 0, scale_game_frames(10));
            }
        } else {
            FUN_L00_00258278(chicken_moby,
                             &vars->turn_speed,
                             flee_angle,
                             frame_time_sq * 0.7853982f,
                             frame_time_sq * 1.5707964f,
                             frame_time * 0.7853982f);
            ahead.x = fast_cos(chicken_moby->rot.z) * (frame_time + frame_time) * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * (frame_time + frame_time) * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(
                chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear == 0 &&
                (1.5707964f < fast_difference_between_rotations(
                                  chicken_moby->rot.z,
                                  fast_arc_tan(next_pos.x - chicken_moby->pos.x,
                                               next_pos.y - chicken_moby->pos.y)) ||
                 fast_vec_dist(&chicken_moby->pos, &next_pos) < size * 0.01f)) {
                chicken_moby->state = 1;
                vars->flee_timer =
                    random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
                blend_moby_animation(
                    chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            }
        }
        break;
    case CHICKEN_STATE_EMIT_FEATHERS:
        if (FUN_L00_002dc9f0(chicken_moby, vars->unk10) != 0) {
            chicken_moby->state = 0;
            *(short *)(vars->unk14 + 0x68) = 0;
        }
        if (random_integer_below(4) != 0) {
            return;
        }
        {
            Vec4A *spawn_point;
            Vec4A *spawn_velocity;
            spawn_point = &point;
            radius = random_float_between(feather_radius_min, feather_radius_max) * size;
            angle = random_angle_radians();
            build_spherical_offset(spawn_point, radius, angle, random_angle_radians());
            point.z += feather_height * size;
            fast_vec_add(spawn_point, spawn_point, &next_pos);
            spawn_velocity = &motion_and_normal_scratch;
            angle = random_float_between(feather_speed_min * frame_time,
                                         feather_speed_max * frame_time);
            build_spherical_offset(
                spawn_velocity, angle, random_angle_radians(), feather_pitch_deg * 0.017453292f);
            motion_and_normal_scratch.z = random_float_between(3.5f, 5.5f) * frame_time;
            spawn_feather_moby((int)chicken_moby, spawn_point, &spawn_velocity->x);
        }
        return;
    }
    if (path_clear == 0) {
        float movement_distance;
        float max;
        fast_vec_sub(&motion_and_normal_scratch, &next_pos, &chicken_moby->pos);
        movement_distance = fast_vec_length(&motion_and_normal_scratch);
        max = frame_time * size;
        if (max < movement_distance) {
            fast_vec_normalize(&motion_and_normal_scratch, &motion_and_normal_scratch, max);
        }
        fast_vec_add(&next_pos, &chicken_moby->pos, &motion_and_normal_scratch);
        if (absolute_float(FUN_L00_00259710(&next_pos) - chicken_moby->pos.z) <=
            frame_time * size) {
            qcopy(&chicken_moby->pos, &next_pos);
        }
    } else {
        qcopy(&chicken_moby->pos, &next_pos);
    }
    vars->fall_speed += frame_time_sq * 9.8f;
    chicken_moby->pos.z -= vars->fall_speed;
    {
        float ground = probe_ground_height(&chicken_moby->pos, 0, 0.5f);
        if (chicken_moby->pos.z < ground && ground < chicken_moby->pos.z + size * 0.25f) {
            vars->fall_speed = 0.0f;
            chicken_moby->pos.z = ground;
        }
    }
    if (collision_hit_material() == 0) {
        vars->health = 0.0f;
    }
    if (chicken_moby->state != 5) {
        float pitch;
        float roll;
        chicken_moby->pos.z = coll_hit.point.f[2];
        qcopy(&motion_and_normal_scratch, &coll_hit.normal_x);
        point.x = motion_and_normal_scratch.x * fast_cos(chicken_moby->rot.z) +
                  motion_and_normal_scratch.y * fast_sin(chicken_moby->rot.z);
        point.y = motion_and_normal_scratch.y * fast_cos(chicken_moby->rot.z) -
                  motion_and_normal_scratch.x * fast_sin(chicken_moby->rot.z);
        point.z = motion_and_normal_scratch.z;
        pitch = -fast_arc_tan(square_root(point.x * point.x + point.z * point.z), point.y);
        roll = fast_arc_tan(point.z, point.x);
        chicken_moby->rot.x = fast_add_rotations(
            chicken_moby->rot.x,
            fast_subtract_rotations(pitch, chicken_moby->rot.x) * (frame_time * 6.2831855f));
        chicken_moby->rot.y = fast_add_rotations(
            chicken_moby->rot.y,
            fast_subtract_rotations(roll, chicken_moby->rot.y) * (frame_time * 6.2831855f));
    }
    if (chicken_moby->pos.z < 0.0f) {
        mark_moby_for_removal(chicken_moby);
    } else {
        chicken_moby->anim_speed = anim_speed;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC210), where it is exact; names translated to the US level program. */

/* Moby state 5 while FUN_L00_002db8f8 reports a hit, back to 1 when it stops. */

int moby_set_state_from_check(struct Moby *a) __asm__("FUN_L00_002cadc0");

int moby_set_state_from_check(struct Moby *checked_moby) {
    int suction_result = process_moby_suck_cannon_suction(checked_moby);
    if (suction_result == 0) {
        if (checked_moby->state == 5)
            checked_moby->state = 1;
    } else {
        checked_moby->state = 5;
    }
    return suction_result;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC2B0), where it is exact; names translated to the US level program. */

int launch_suck_cannon_projectile_and_update_moby_state(
    struct Moby *moby,
    int          a,
    int          target_moby,
    float        target_height_offset) __asm__("FUN_L00_002cae60");

int launch_suck_cannon_projectile_and_update_moby_state(struct Moby *moby,
                                                        int          launch_vector_address,
                                                        int          target_moby,

                                                        float target_height_offset) {
    char *class_callback_data = get_moby_suck_cannon_record(moby);
    int   projectile_launch_result = launch_suck_cannon_projectile(
        moby, launch_vector_address, target_moby, target_height_offset);
    if (projectile_launch_result != 0) {
        moby->state = 5;
    } else if ((unsigned char)((char)moby->state) == 5) {
        moby->state = 1;
    }
    if (*(short *)(class_callback_data + 0x68) == 6) {
        allocate_voice_for_target_entry(1, 0, moby);
    }
    return projectile_launch_result;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC360), where it is exact; names translated to the US level program. */

/* Runs FUN_L00_002dbe20 on the moby and sets its state to 5. */

int cancel_suck_cannon_suction_and_set_moby_state_5(struct Moby *moby) __asm__("FUN_L00_002caf10");

int cancel_suck_cannon_suction_and_set_moby_state_5(struct Moby *moby) {
    int helper_result = cancel_suck_cannon_suction(moby);
    moby->state = 5;
    return helper_result;
}

/* Returns the word at +0x14 of the moby data, or 0 when either pointer is null. */

s32 FUN_L00_002caf40(s32 moby_address) {
    s32 moby_vars_address;
    if (moby_address != 0) {
        moby_vars_address = *(s32 *)((u8 *)moby_address + 0x78);
        if (moby_vars_address != 0) {
            return *(s32 *)((u8 *)moby_vars_address + 0x14);
        }
        return 0;
    }
    return 0;
}

unsigned char *spawn_feather_moby(int unused, void *spawn_position, float *initial_velocity) {
    unsigned char *spawned_moby = create_moby(0x1AC);
    if (spawned_moby) {
        unsigned char *moby_vars;
        int            lifetime_lower_bound, lifetime_upper_bound;
        spawned_moby[0x30] = 0x40;
        *(unsigned short *)(spawned_moby + 0x34) |= 0x1000;
        *(short *)(spawned_moby + 0x32) = 0x20;
        spawned_moby[0x31] = 1;
        moby_vars = *(unsigned char **)(spawned_moby + 0x78);
        spawned_moby[0x20] = 0;
        spawned_moby[0xBC] = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        *(float *)(spawned_moby + 0x18) += random_float_between(0.2f, 0.7f);
        *(float *)(spawned_moby + 0x10) += initial_velocity[0] * 3.0f;
        *(float *)(spawned_moby + 0x14) += initial_velocity[1] * 3.0f;
        qcopy(moby_vars, initial_velocity);
        *(float *)(spawned_moby + 0x44) = randf_sym(0.17453292f, 0.5235988f);
        *(float *)(spawned_moby + 0x40) = random_float_between(0.17453292f, 0.7853982f);
        {
            float x_rotation_step =
                random_float_between(200.0f, 370.0f) * DEG_TO_RAD * D_0015ED6C_lui;
            *(int *)(moby_vars + 0x24) = -1;
            *(float *)(moby_vars + 0x2C) = x_rotation_step;
        }
        if (FUN_001160d8_2cbf68() & 1) {
            *(int *)(moby_vars + 0x24) = 1;
            *(float *)(spawned_moby + 0x44) = -*(float *)(spawned_moby + 0x44);
        }
        *(float *)(spawned_moby + 0x48) = random_angle_radians();
        *(float *)(moby_vars + 0x28) =
            -(random_float_between(200.0f, 550.0f) * DEG_TO_RAD * D_0015ED6C_lui);
        *(float *)(moby_vars + 0x10) = random_angle_radians();
        {
            float initial_random_angle = random_angle_radians();
            float base_lifetime = (float)feather_base_lifetime_gp;
            *(float *)(moby_vars + 0x14) = initial_random_angle;
            lifetime_lower_bound = truncate_float_to_s32(base_lifetime * 0.7f);
        }
        lifetime_upper_bound = truncate_float_to_s32((float)feather_base_lifetime_gp * 1.2f);
        *(int *)(moby_vars + 0x1C) =
            scale_game_frames(random_int_between(lifetime_lower_bound, lifetime_upper_bound));
        {
            float k = feather_scale_multiplier_gp;
            *(float *)(spawned_moby + 0x2C) *= k;
        }
        if (D_0013E535_lui)
            *(float *)(spawned_moby + 0x2C) *= 2.0f;
        *(float *)(moby_vars + 0x20) = random_float_between(-1.2f, -0.4f) * D_0015ED6C_lui;
        update_entity(spawned_moby);
        copy_hero_moby_field_38(spawned_moby);
    }
    return spawned_moby;
}

s32 D_L01_00161884 = 0;
u32 D_L01_00161870[1] = {0x20202};

float D_L01_0015F5D4 = 0;
int   D_L01_00161880 = 0;
float D_L01_00161888 = 7e+01f;
float D_L01_0016188C = 5.0f;
float D_L01_00161890 = 9.0f;
float D_L01_00161894 = 0.25f;
float D_L01_00161898 = 0;
float D_L01_0016189C = 0.35f;
int   D_L01_001618DC = 0xf0;
float D_L01_001618D8 = 9.0f;
s32   D_L02_00161824 = 0;
u32   D_L02_00161810[1] = {0x20202};

float D_L02_0015F5D0 = 0;
float D_L02_0015F5D4 = 0;
int   D_L02_00161820 = 0;
float D_L02_00161828 = 7e+01f;
float D_L02_0016182C = 5.0f;
float D_L02_00161830 = 9.0f;
float D_L02_00161834 = 0.25f;
float D_L02_00161838 = 0;
float D_L02_0016183C = 0.35f;
int   D_L02_00161874 = 0xf0;
float D_L02_00161870 = 9.0f;
s32   D_L03_00161834 = 0;
u32   D_L03_00161820[1] = {0x20202};

float D_L03_0015F5D0 = 0;
float D_L03_0015F5D4 = 0;
int   D_L03_00161830 = 0;
float D_L03_00161838 = 7e+01f;
float D_L03_0016183C = 5.0f;
float D_L03_00161840 = 9.0f;
float D_L03_00161844 = 0.25f;
float D_L03_00161848 = 0;
float D_L03_0016184C = 0.35f;
int   D_L03_00161884 = 0xf0;
float D_L03_00161880 = 9.0f;
s32   D_L04_00161824 = 0;
u32   D_L04_00161810[1] = {0x20202};

float D_L04_0015F5D0 = 0;
float D_L04_0015F5D4 = 0;
int   D_L04_00161820 = 0;
float D_L04_00161828 = 7e+01f;
float D_L04_0016182C = 5.0f;
float D_L04_00161830 = 9.0f;
float D_L04_00161834 = 0.25f;
float D_L04_00161838 = 0;
float D_L04_0016183C = 0.35f;
int   D_L04_001618E4 = 0xf0;
float D_L04_001618E0 = 9.0f;

struct Moby *D_L05_001D1A20 NOT_SDA = 0;
s32                         D_L05_00161874 = 0;
u32                         D_L05_00161860[1] = {0x20202};

float                       D_L05_0015F5D0 = 0;
float                       D_L05_0015F5D4 = 0;
int                         D_L05_00161870 = 0;
float                       D_L05_00161878 = 7e+01f;
float                       D_L05_0016187C = 5.0f;
float                       D_L05_00161880 = 9.0f;
float                       D_L05_00161884 = 0.25f;
float                       D_L05_00161888 = 0;
float                       D_L05_0016188C = 0.35f;
int                         D_L05_001618D8 = 0xf0;
float                       D_L05_001618D4 = 9.0f;
struct Moby *D_L06_001D1D00 NOT_SDA = 0;
s32                         D_L06_00161854 = 0;
u32                         D_L06_00161840[1] = {0x20202};

float D_L06_0015F5D0 = 0;
float D_L06_0015F5D4 = 0;
int   D_L06_00161850 = 0;
float D_L06_00161858 = 7e+01f;
float D_L06_0016185C = 5.0f;
float D_L06_00161860 = 9.0f;
float D_L06_00161864 = 0.25f;
float D_L06_00161868 = 0;
float D_L06_0016186C = 0.35f;
int   D_L06_001618C4 = 0xf0;
float D_L06_001618C0 = 9.0f;

s32 D_L07_00161824 = 0;
u32 D_L07_00161810[1] = {0x20202};

float D_L07_0015F5D0 = 0;
float D_L07_0015F5D4 = 0;
int   D_L07_00161820 = 0;
float D_L07_00161828 = 7e+01f;
float D_L07_0016182C = 5.0f;
float D_L07_00161830 = 9.0f;
float D_L07_00161834 = 0.25f;
float D_L07_00161838 = 0;
float D_L07_0016183C = 0.35f;
int   D_L07_00161888 = 0xf0;
float D_L07_00161884 = 9.0f;

s32 D_L08_00161914 = 0;
u32 D_L08_00161900[1] = {0x20202};

int                         D_L08_00161910 = 0;
float                       D_L08_00161918 = 7e+01f;
float                       D_L08_0016191C = 5.0f;
float                       D_L08_00161920 = 9.0f;
float                       D_L08_00161924 = 0.25f;
float                       D_L08_00161928 = 0;
float                       D_L08_0016192C = 0.35f;
int                         D_L08_001619DC = 0xf0;
float                       D_L08_001619D8 = 9.0f;
struct Moby *D_L09_001D1700 NOT_SDA = 0;
s32                         D_L09_00161904 = 0;
u32                         D_L09_001618F0[1] = {0x20202};

int                         D_L09_00161900 = 0;
float                       D_L09_00161908 = 7e+01f;
float                       D_L09_0016190C = 5.0f;
float                       D_L09_00161910 = 9.0f;
float                       D_L09_00161914 = 0.25f;
float                       D_L09_00161918 = 0;
float                       D_L09_0016191C = 0.35f;
int                         D_L09_00161954 = 0xf0;
float                       D_L09_00161950 = 9.0f;
struct Moby *D_L10_001D1A00 NOT_SDA = 0;
s32                         D_L10_00161834 = 0;
u32                         D_L10_00161820[1] = {0x20202};

float                       D_L10_0015F5D0 = 0;
float                       D_L10_0015F5D4 = 0;
int                         D_L10_00161830 = 0;
float                       D_L10_00161838 = 7e+01f;
float                       D_L10_0016183C = 5.0f;
float                       D_L10_00161840 = 9.0f;
float                       D_L10_00161844 = 0.25f;
float                       D_L10_00161848 = 0;
float                       D_L10_0016184C = 0.35f;
int                         D_L10_001618B0 = 0xf0;
float                       D_L10_001618AC = 9.0f;
struct Moby *D_L11_001D1F80 NOT_SDA = 0;
s32                         D_L11_001618B4 = 0;
u32                         D_L11_001618A0[1] = {0x20202};

float D_L11_0015F5D0 = 0;
float D_L11_0015F5D4 = 0;
int   D_L11_001618B0 = 0;
float D_L11_001618B8 = 7e+01f;
float D_L11_001618BC = 5.0f;
float D_L11_001618C0 = 9.0f;
float D_L11_001618C4 = 0.25f;
float D_L11_001618C8 = 0;
float D_L11_001618CC = 0.35f;
int   D_L11_0016195C = 0xf0;
float D_L11_00161958 = 9.0f;

struct Moby *D_L12_001E4620 NOT_SDA = 0;
s32                         D_L12_00161944 = 0;
u32                         D_L12_00161930[1] = {0x20202};

float D_L12_0015F5D0 = 0;
float D_L12_0015F5D4 = 0;
int   D_L12_00161940 = 0;
float D_L12_00161948 = 7e+01f;
float D_L12_0016194C = 5.0f;
float D_L12_00161950 = 9.0f;
float D_L12_00161954 = 0.25f;
float D_L12_00161958 = 0;
float D_L12_0016195C = 0.35f;
int   D_L12_00161A34 = 0xf0;
float D_L12_00161A30 = 9.0f;
s32   D_L13_00161904 = 0;
u32   D_L13_001618F0[1] = {0x20202};

int   D_L13_00161900 = 0;
float D_L13_00161908 = 7e+01f;
float D_L13_0016190C = 5.0f;
float D_L13_00161910 = 9.0f;
float D_L13_00161914 = 0.25f;
float D_L13_00161918 = 0;
float D_L13_0016191C = 0.35f;
int   D_L13_0016197C = 0xf0;
float D_L13_00161978 = 9.0f;
s32   D_L14_00161AA4 = 0;
u32   D_L14_00161A90[1] = {0x20202};

float                       D_L14_0015F5D0 = 0;
float                       D_L14_0015F5D4 = 0;
int                         D_L14_00161AA0 = 0;
float                       D_L14_00161AA8 = 7e+01f;
float                       D_L14_00161AAC = 5.0f;
float                       D_L14_00161AB0 = 9.0f;
float                       D_L14_00161AB4 = 0.25f;
float                       D_L14_00161AB8 = 0;
float                       D_L14_00161ABC = 0.35f;
int                         D_L14_00161B50 = 0xf0;
float                       D_L14_00161B4C = 9.0f;
struct Moby *D_L15_001D2000 NOT_SDA = 0;
s32                         D_L15_00161A24 = 0;
u32                         D_L15_00161A10[1] = {0x20202};

float D_L15_0015F5D4 = 0;
int   D_L15_00161A20 = 0;
float D_L15_00161A28 = 7e+01f;
float D_L15_00161A2C = 5.0f;
float D_L15_00161A30 = 9.0f;
float D_L15_00161A34 = 0.25f;
float D_L15_00161A38 = 0;
float D_L15_00161A3C = 0.35f;
int   D_L15_00161ADC = 0xf0;
float D_L15_00161AD8 = 9.0f;

struct Moby *D_L16_001D1A00 NOT_SDA = 0;
s32                         D_L16_001618E4 = 0;
u32                         D_L16_001618D0[1] = {0x20202};

float D_L16_0015F5D4 = 0;
int   D_L16_001618E0 = 0;
int   D_L16_00161934 = 0xf0;
float D_L16_00161930 = 9.0f;

struct Moby *D_L17_001D2280 NOT_SDA = 0;
s32                         D_L17_00161934 = 0;
u32                         D_L17_00161920[1] = {0x20202};

float D_L17_0015F5D0 = 0;
float D_L17_0015F5D4 = 0;
int   D_L17_00161930 = 0;
float D_L17_00161938 = 7e+01f;
float D_L17_0016193C = 5.0f;
float D_L17_00161940 = 9.0f;
float D_L17_00161944 = 0.25f;
float D_L17_00161948 = 0;
float D_L17_0016194C = 0.35f;
int   D_L17_001619B0 = 0xf0;
float D_L17_001619AC = 9.0f;

struct Moby *D_L18_001D1F80 NOT_SDA = 0;
s32                         D_L18_001618C4 = 0;
u32                         D_L18_001618B0[1] = {0x20202};

float D_L18_0015F5D0 = 0;
float D_L18_0015F5D4 = 0;
int   D_L18_001618C0 = 0;
float D_L18_001618C8 = 7e+01f;
float D_L18_001618CC = 5.0f;
float D_L18_001618D0 = 9.0f;
float D_L18_001618D4 = 0.25f;
float D_L18_001618D8 = 0;
float D_L18_001618DC = 0.35f;
int   D_L18_00161928 = 0xf0;
float D_L18_00161924 = 9.0f;

s32 D_L00_00161854_gp __asm__("D_L00_00161854") = 0;
int chicken_manipulator_joint __asm__("D_L00_00161850") = 0;
