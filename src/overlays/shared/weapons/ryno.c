/* Shared moby class ryno: target lock and the missile salvo. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/math/fast_math.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/usage_stats.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/game/game_globals.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "rnc/audio/voice_alloc.h"

typedef struct Moby Moby;

typedef unsigned int u128_2cc360 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2cc360 q;
    float       f[4];
} V_2cc360;

typedef struct {
    char  p0[0x46];
    short h46;
} C_2cc360;

typedef struct M_2cc360 {
    char             p0[0x10];
    V_2cc360         pos;
    char             p20[4];
    C_2cc360        *c;
    struct M_2cc360 *next;
    char             p2c[5];
    unsigned char    b31;
    short            h32;
    unsigned short   h34;
} M_2cc360;

typedef struct {
    float m[3][4];
} __attribute__((aligned(16))) Mat3_2cc608;

typedef struct {
    float m[4][4];
} __attribute__((aligned(16))) Mat4_2cc608;

typedef struct {
    struct Moby  *target;
    unsigned char shots;
    unsigned char slot;
    short         shot_timer;
    int           lock_timer;
    float         height;
    short         track_timer;
    short         count;
    float         life;
    struct Moby  *recent[7];
} Ryno_2cc608;

typedef struct {
    float radius;
    char  pad4[0xC];
    float height;
} Bounds;

typedef union {
    OvlQuad q;
    float   f[4];
} V_2ccee0;

extern void  reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern float D_0015ED60;

extern M_2cc360 *D_L00_0015FFE4_2cc360 __asm__("D_L00_0015FFE4");
extern float     fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern int       coll_2cc360(void *, void *, int, void *, int) __asm__("FUN_001efa68");

extern Vec4           camera_rot_2cc608 __asm__("D_L00_00166DD0");
extern int            D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
extern int D_0015ED84 MACRO_ADDR;
extern struct Moby   *FUN_L00_002cc360_c(
      float *, float *, struct Moby **, float, float, float) __asm__("FUN_L00_002cc360");
extern int  get_ammo(int) __asm__("FUN_L00_00233e98");
extern int  use_ammo(int, int) __asm__("FUN_L00_00233db8");
extern void suck_draw_reticle(
    struct Moby *, int, float, float, float *, int, int, float, int) __asm__("FUN_L00_001edf60");
extern void           hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void           start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
extern void           add_2cc608(float *, void *, void *) __asm__("FUN_001f9a10");
extern void           scale_2cc608(float *, float *, float) __asm__("FUN_001f9a68");
extern unsigned char *FUN_L00_002ccee0_c(
    struct Moby *, float *, struct Moby *, float, float, float) __asm__("FUN_L00_002ccee0");
extern int rand(void) __asm__("FUN_001160d8");

extern unsigned char *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern unsigned char  D_L00_00173E60_2ccee0[] __asm__("D_L00_00173E60")
    __attribute__((section(".data")));
extern int  FUN_001f96f8_2ccee0(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00250df8_2ccee0(void *) __asm__("FUN_L00_00250df8");
extern int  FUN_001efa68_2ccee0(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void FUN_001f9bf8_2ccee0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_0025e450_2ccee0(void *,
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

M_2cc360 *select_ryno_target(float     *pos,
                             float     *aim_rotation,
                             M_2cc360 **excl,
                             float      maxYaw,
                             float      maxPitch,
                             float      maxDist) __asm__("FUN_L00_002cc360");

M_2cc360 *select_ryno_target(

    float *pos, float *aim_rotation, M_2cc360 **excl, float maxYaw, float maxPitch, float maxDist) {
    M_2cc360 *best = 0;
    float     bestScore = 1.0e9f;
    M_2cc360 *candidate_moby;
    float    *info;
    V_2cc360  aim_point;
    V_2cc360 *pp;
    float     candidate_distance, ys, ps, score;
    int       i, found;
    for (candidate_moby = D_L00_0015FFE4_2cc360; candidate_moby;
         candidate_moby = candidate_moby->next) {
        if (candidate_moby->h32 == 0)
            continue;
        if (excl) {
            found = 0;
            for (i = 0; i < 7; i++) {
                if (excl[i] == candidate_moby) {
                    found = 1;
                    break;
                }
            }
            if (found)
                continue;
        } else if (!candidate_moby->b31) {
            continue;
        }
        info = suck_get_aim_height(candidate_moby);
        if (!info)
            continue;
        if (info[0] < 0.0f)
            continue;
        if (!(candidate_moby->h34 & 0x1000))
            continue;
        if (!candidate_moby)
            continue;
        if (!candidate_moby->c)
            continue;
        if (candidate_moby->c->h46 != 5)
            continue;
        pp = &candidate_moby->pos;
        candidate_distance = vector_distance_xy(pos, pp);
        if (!(candidate_distance < maxDist))
            continue;
        qcopy(&aim_point, pp);
        aim_point.f[2] += info[4] + 0.1f;
        ys = fast_difference_between_rotations(
            aim_rotation[2], fast_arc_tan(aim_point.f[0] - pos[0], aim_point.f[1] - pos[1]));
        ys = ys * ys;
        if (ys < maxYaw * maxYaw) {
            ps = fast_difference_between_rotations(
                aim_rotation[1], fast_arc_tan(candidate_distance, aim_point.f[2] - pos[2]));
            ps = ps * ps;
            if (!(ps < maxPitch * maxPitch))
                continue;
            if (5.0f < candidate_distance) {
                ys = ys * ps * candidate_distance;
                ys += candidate_distance;
            } else {
                ys = candidate_distance / 5.0f;
            }
            if (ys < bestScore) {
                if (coll_2cc360(pos, &aim_point, 2, candidate_moby, 0))
                    continue;
                bestScore = ys;
                best = candidate_moby;
            }
        }
    }
    return best;
}

/* Ryno (class 454) update: target lock and reticle while held, then the eight-missile salvo state machine. */

void ryno_update(struct Moby *m) __asm__("FUN_L00_002cc608");

void ryno_update(struct Moby *ryno_moby) {
    enum {
        RYNO_STATE_INITIALIZE = 0,
        RYNO_STATE_WAIT_ANIMATION = 1,
        RYNO_STATE_READY_TO_FIRE = 2,
        RYNO_STATE_READY_TO_FIRE_WITH_RETICLE = 3,
        RYNO_STATE_FIRE_SALVO = 4,
        RYNO_STATE_COOLDOWN = 5
    };
    Mat3_2cc608  launch_direction_basis;
    Mat4_2cc608  view;
    Vec4         aim;
    Vec4         muzzle_position_and_launch_direction;
    Vec4         ofs;
    Ryno_2cc608 *ryno_vars = *(Ryno_2cc608 **)&ryno_moby->pvars;
    struct Moby *acquired_target;
    Bounds      *target_bounds;
    float        yaw, pitch;
    int          i;

    copy_matrix3x4(&launch_direction_basis, l00_camera.unk0);
    transpose_rotation_matrix(&view, &launch_direction_basis);
    if ((unsigned int)(ryno_moby->state - 2) < 2) {
        if (fast_dec_timer(&ryno_vars->lock_timer) || ryno_vars->target == 0 ||
            ryno_vars->target->state == 0xFE || ryno_vars->target->state == 0xFD) {
            ryno_vars->target = 0;
            ryno_vars->height = 0;
        }
        qcopy(aim.f, camera_rot_2cc608.f);
        aim.f[1] = -aim.f[1];
        moby_point_position(ryno_moby, ryno_vars->slot, muzzle_position_and_launch_direction.f);
        acquired_target = FUN_L00_002cc360_c(
            muzzle_position_and_launch_direction.f, aim.f, 0, 1.6981583f, 1.6981583f, 80.0f);
        if ((acquired_target != 0 && ryno_vars->target == 0) || ryno_vars->target == 0 ||
            ryno_vars->target->state == 0xFE || ryno_vars->target->state == 0xFD ||
            ryno_vars->target->visible == 0) {
            ryno_vars->height = 0;
            ryno_vars->target = acquired_target;
            ryno_vars->lock_timer = scale_game_frames(20);
            ryno_vars->life = 1.0f;
            target_bounds = suck_get_aim_height(ryno_vars->target);
            if (target_bounds != 0) {
                ryno_vars->height = target_bounds->height;
                ryno_vars->life = target_bounds->radius;
            }
        }
        if (ryno_vars->target != 0 && fast_dec_timer_s16(&ryno_vars->track_timer) && get_ammo(-1)) {
            if (ryno_vars->target == 0 || ryno_vars->target->state == 0xFE ||
                ryno_vars->target->state == 0xFD) {
                ryno_vars->target = 0;
                ryno_vars->height = 0;
            } else {
                float f =
                    fast_vec_dist(g_debug_camera.position, ((float *)&ryno_vars->target->pos)) /
                    150.0f;
                qcopy(aim.f, ((float *)&ryno_vars->target->pos));
                f = f * 0.65f;
                aim.f[2] = aim.f[2] + ryno_vars->height;
                suck_draw_reticle(ryno_moby,
                                  0xFF0FFF0F,
                                  absolute_float(0.9f - f),
                                  0.0f,
                                  aim.f,
                                  0x23,
                                  -1,
                                  90.0f,
                                  4);
            }
        }
    }
    switch (ryno_moby->state) {
    case RYNO_STATE_INITIALIZE:
        ryno_vars->target = 0;
        hero.items[0].switch_lock = 0;
        ryno_moby->state = 1;
        ryno_vars->track_timer = 0;
        break;
    case RYNO_STATE_WAIT_ANIMATION:
        if (ryno_moby->anim_flags & 2)
            ryno_moby->state = 2;
        break;
    case RYNO_STATE_READY_TO_FIRE:
        if (hero.state.current == 1)
            hero_set_state(0x1E, 1);
        if ((controller_state.pressed & hero.items[0].button_mask) == 0)
            return;
        if (hero.item_use_locked != 0)
            return;
        if (use_ammo(-1, 1) == 0)
            return;
        {
            int                per;
            struct UsageStats *s = (struct UsageStats *)item_usage;
            if (s->stat[23].count <= 0xFFFE)
                s->stat[23].count++;
            per = 600;
            if (scale_game_frames(D_0015EEA4_macro) / per > s->stat[23].unk2)
                s->stat[23].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
            s->stat[23].level_mask = s->stat[23].level_mask | (1 << D_0015ED84) | 0x80000000;
        }
        hero.items[0].switch_lock = 2;
        ryno_moby->state = 4;
        ryno_vars->shot_timer = 0;
        ryno_vars->slot = 0;
        ryno_vars->shots = 0;
        start_equipped_item_animation();
        {
            int k;
            for (k = 6; k >= 0; k--)
                ryno_vars->recent[k] = 0;
        }
        ryno_vars->count = 0;
        break;
    case RYNO_STATE_READY_TO_FIRE_WITH_RETICLE:
        suck_draw_reticle(ryno_moby, 0xFF0F0FFF, 1.0f, 0.0f, 0, 0x23, -1, 90.0f, 4);
        if ((controller_state.held & 5) == 0) {
            ryno_moby->state = 2;
            return;
        }
        if ((controller_state.pressed & hero.items[0].button_mask) == 0)
            return;
        if (hero.item_use_locked != 0)
            return;
        if (use_ammo(-1, 1) == 0)
            return;
        {
            int                per;
            struct UsageStats *s = (struct UsageStats *)item_usage;
            if (s->stat[23].count <= 0xFFFE)
                s->stat[23].count++;
            per = 600;
            if (scale_game_frames(D_0015EEA4_macro) / per > s->stat[23].unk2)
                s->stat[23].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
            s->stat[23].level_mask = s->stat[23].level_mask | (1 << D_0015ED84) | 0x80000000;
        }
        hero.items[0].switch_lock = 2;
        ryno_moby->state = 4;
        ryno_vars->shot_timer = 0;
        ryno_vars->slot = 0;
        ryno_vars->shots = 0;
        start_equipped_item_animation();
        {
            int k;
            for (k = 6; k >= 0; k--)
                ryno_vars->recent[k] = 0;
        }
        ryno_vars->count = 0;
        break;
    case RYNO_STATE_FIRE_SALVO:
        if (fast_dec_timer_s16(&ryno_vars->shot_timer) == 0)
            return;
        ryno_vars->shot_timer = scale_game_frames(9);
        ryno_vars->slot++;
        ryno_vars->slot = ryno_vars->slot % 9;
        ryno_vars->shots++;
        muzzle_position_and_launch_direction.q = 0;
        if (hero.state.current == 30) {
            muzzle_position_and_launch_direction.f[2] = 1.0f;
            transform_vector(muzzle_position_and_launch_direction.f,
                             muzzle_position_and_launch_direction.f,
                             &view);
        } else {
            muzzle_position_and_launch_direction.f[0] = 1.0f;
            transform_vector_by_basis(muzzle_position_and_launch_direction.f,
                                      muzzle_position_and_launch_direction.f,
                                      hero.basis);
        }
        add_2cc608(ofs.f, &hero.motion.displacement, &hero.motion.platform_carry);
        scale_2cc608(ofs.f, ofs.f, 3.0f);
        add_2cc608(
            muzzle_position_and_launch_direction.f, muzzle_position_and_launch_direction.f, ofs.f);
        if (hero.items[6].locked_on != 0) {
            yaw = hero.items[6].lock_yaw;
        } else {
            yaw = fast_arc_tan(muzzle_position_and_launch_direction.f[0],
                               muzzle_position_and_launch_direction.f[1]);
        }
        pitch = -fast_arc_tan(vector_length_xy(muzzle_position_and_launch_direction.f),
                              muzzle_position_and_launch_direction.f[2]);
        moby_point_position(ryno_moby, ryno_vars->slot, aim.f);
        start_equipped_item_animation();
        FUN_L00_002ccee0_c(ryno_moby, aim.f, ryno_vars->target, ryno_vars->height, yaw, pitch);
        if (ryno_vars->shots >= 7) {
            hero.items[0].switch_lock = 0;
            cancel_hero_item_animation();
            ryno_moby->state = 5;
            ryno_vars->shot_timer = scale_game_frames(60);
            ryno_vars->track_timer = scale_game_frames(60);
        }
        ryno_vars->life = ryno_vars->life - 1.0f;
        if (ryno_vars->life <= 0.0f || ryno_vars->target == 0 || ryno_vars->target->state == 0xFE ||
            ryno_vars->target->state == 0xFD) {
            ryno_vars->recent[ryno_vars->count] = ryno_vars->target;
            ryno_vars->count++;
            ryno_vars->count = ryno_vars->count % 7;
            qcopy(aim.f, camera_rot_2cc608.f);
            aim.f[1] = -aim.f[1];
            ryno_vars->target = FUN_L00_002cc360_c(((float *)&ryno_moby->pos),
                                                   aim.f,
                                                   ryno_vars->recent,
                                                   3.1415927f,
                                                   3.1415927f,
                                                   100.0f);
            if (ryno_vars->target == 0 && ryno_vars->count > 0 &&
                (rand() & (ryno_vars->count + 1)) == 0) {
                ryno_vars->target = ryno_vars->recent[rand() % ryno_vars->count];
            }
            ryno_vars->height = 0;
            ryno_vars->lock_timer = scale_game_frames(20);
            ryno_vars->life = 1.0f;
            target_bounds = suck_get_aim_height(ryno_vars->target);
            if (target_bounds != 0) {
                ryno_vars->height = target_bounds->height;
                ryno_vars->life = target_bounds->radius;
            }
        }
        break;
    case RYNO_STATE_COOLDOWN:
        if (fast_dec_timer_s16(&ryno_vars->shot_timer) == 0)
            return;
        ryno_vars->shots = 0;
        ryno_vars->target = 0;
        ryno_moby->state = 2;
        break;
    case 6:
    case 7:
    case 8:
        break;
    }
}

unsigned char *spawn_ryno_rocket(int            owner_moby,
                                 V_2ccee0      *spawn_position,
                                 unsigned char *target_moby,
                                 float          target_height_offset,
                                 float          yaw,
                                 float          pitch) __asm__("FUN_L00_002ccee0");

unsigned char *spawn_ryno_rocket(int            owner_moby,
                                 V_2ccee0      *spawn_position,
                                 unsigned char *target_moby,
                                 float          target_height_offset,
                                 float          yaw,

                                 float pitch) {
    V_2ccee0       forward_step;
    V_2ccee0       collision_start;
    V_2ccee0       explosion_direction;
    V_2ccee0       z;
    unsigned char *spawned_moby;
    unsigned char *rocket_vars;
    unsigned char *target_aim_data;
    struct Moby   *collision_ignore_moby;
    float          random_range_limit;
    float          f;

    spawned_moby = create_moby(0x1C9);
    if (spawned_moby != 0) {
        rocket_vars = *(unsigned char **)(spawned_moby + 0x78);
        spawned_moby[0x30] = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        *(float *)(spawned_moby + 0x48) = yaw;
        *(float *)(spawned_moby + 0x44) = pitch;
        spawned_moby[0x20] = 0;
        *(int *)(rocket_vars + 0x20) = owner_moby;
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(rocket_vars + 0x10, spawn_position);
        *(int *)(rocket_vars + 0x34) = 0;
        *(int *)(rocket_vars + 0x38) = 0;
        *(int *)(rocket_vars + 0x30) = 0;
        *(unsigned char **)(rocket_vars + 0x28) = target_moby;
        *(float *)(spawned_moby + 0x2C) =
            *(float *)(*(unsigned char **)(spawned_moby + 0x24) + 0x24) * 0.25f;
        if (target_moby != 0) {
            *(int *)(rocket_vars + 0x58) = *(short *)(target_moby + 0xA6);
            target_aim_data = suck_get_aim_height(target_moby);
            if (target_aim_data != 0) {
                *(unsigned short *)(target_aim_data + 0x1E) |= 0x80;
            }
            qcopy(rocket_vars, target_moby + 0x10);
            *(float *)(rocket_vars + 0x2C) = target_height_offset;
            *(float *)(rocket_vars + 8) += target_height_offset;
        }
        *(int *)(rocket_vars + 0x24) = FUN_001f96f8_2ccee0(0xFA);
        *(int *)(rocket_vars + 0x44) = 0;
        *(float *)(rocket_vars + 0x48) = random_float_between(-3.1415927f, 3.1415927f);
        f = random_float_between(-3.1415927f, 3.1415927f);
        random_range_limit = D_0015ED60;
        random_range_limit *= 0.00967f;
        *(float *)(rocket_vars + 0x4C) = f;
        f = random_float_between(-random_range_limit, random_range_limit);
        random_range_limit = D_0015ED60;
        random_range_limit *= 0.02406f;
        *(float *)(rocket_vars + 0x50) = f;
        *(float *)(rocket_vars + 0x54) =
            random_float_between(-random_range_limit, random_range_limit);
        FUN_L00_00250df8_2ccee0(spawned_moby);
        allocate_voice_for_target_entry(1, 0, spawned_moby);
        z.q = 0;
        z.f[2] = D_0015ED6C_lui * 8.0f;
        collision_ignore_moby = D_001413D0_lui;
        explosion_direction.q = z.q;
        collision_start.q = ((V_2ccee0 *)&collision_ignore_moby->pos)->q;
        collision_start.f[2] = spawn_position->f[2];
        if (FUN_001efa68_2ccee0(&collision_start, spawn_position, 0, collision_ignore_moby, 0)) {
            spawned_moby[0xBC] = 2;
            qcopy(spawned_moby + 0x10, D_L00_00173E60_2ccee0);
            build_spherical_offset(&forward_step,
                                   *(float *)(rocket_vars + 0x30),
                                   *(float *)(spawned_moby + 0x48),
                                   -*(float *)(spawned_moby + 0x44));
            reflect_vector(&explosion_direction, &forward_step, D_L00_00173E60_2ccee0 + 0x20);
            FUN_001f9bf8_2ccee0(
                &explosion_direction, &explosion_direction, D_0015ED6C_lui + D_0015ED6C_lui);
            FUN_L00_0025e450_2ccee0(spawned_moby,
                                    &explosion_direction,
                                    0,
                                    0.0f,
                                    0.0f,
                                    10,
                                    3,
                                    9,
                                    4.0f,
                                    2.0f,
                                    9.0f,
                                    1.0f,
                                    0,
                                    15.0f,
                                    1,
                                    1,
                                    -1,
                                    0);
            mark_moby_for_removal(spawned_moby);
            return 0;
        }
    }
    return spawned_moby;
}

Vec4 D_L01_00174300 = {0};

Vec4 D_L01_00167250 = {0};

Vec4 D_L02_001673D0 = {0};

Vec4 D_L03_00166ED0 = {0};

Vec4 D_L04_00166F50 = {0};

Vec4 D_L05_00174280 = {0};

Vec4 D_L06_00167550 = {0};

Vec4 D_L07_00166E50 = {0};

Vec4 D_L08_001675D0 = {0};

Vec4 D_L09_00166F50 = {0};

Vec4 D_L10_00167250 = {0};

Vec4 D_L12_00174300 = {0};

Vec4 D_L12_00167250 = {0};

Vec4 D_L14_001674D0 = {0};

Vec4 D_L15_001673D0 = {0};

Vec4 D_L16_00174280 = {0};

Vec4 D_L16_001671D0 = {0};

Vec4 D_L17_00174780 = {0};

Vec4 D_L18_00174880 = {0};

Vec4 D_L18_001677D0 = {0};
