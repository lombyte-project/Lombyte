/* Level 13 robosquawker moby class: target selection, particle, ground height and update. */
#include "types.h"
#include "sda.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/l13/mobys/robosquawker_globals.h"

#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
#include "rnc/overlay/level_labels.h"
#include "qcopy.h"
#include "qzero.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");

extern int FUN_L00_00257b90(int, int);

extern void FUN_L00_00257470(void *, int, int);

extern void FUN_L00_00263ac8(float, int, int, unsigned char *);

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern float D_L13_001670C0[];

extern short       D_L13_001B0B30[] NOT_SDA;
extern char       *D_L13_001D1E90[] NOT_SDA;
extern struct Hero hero_lui __asm__("D_0013F350") NOT_SDA;
int                select_robosquawker_target(char *source_object,
                                              char *selection_output,
                                              float best) __asm__("FUN_L13_0026bd00");

int select_robosquawker_target(char *source_object, char *selection_output, float best) {
    struct Hero *hero = &hero_lui;
    char        *selected_object;
    char        *candidate_object;
    int          selection_status;
    int          i;
    int          j;
    float        candidate_metric;

    selected_object = ((char *)hero->moby);
    selection_status = 0;
    if (hero->state.control_mode == 0x18 || hero->state.current == 0x72) {
        selected_object = 0;
        selection_status = 2;
    }
    for (i = 1; i <= D_L13_001B0B30[0]; i++) {
        candidate_object = (char *)moby_base + (D_L13_001B0B30[i] << 8);
        if (candidate_object == 0)
            continue;
        if (*(short *)(candidate_object + 0xA6) != 0xCB &&
            *(short *)(candidate_object + 0xA6) != 0x76C)
            continue;
        if (*(unsigned char *)(candidate_object + 0x20) == 0xFE)
            continue;
        if (*(unsigned char *)(candidate_object + 0x20) == 0xFD)
            continue;
        if (*(unsigned char *)(candidate_object + 0x20) != 3)
            continue;
        candidate_metric = vector_distance_xy(source_object + 0x10, candidate_object + 0x10);
        if (candidate_metric < best) {
            selected_object = candidate_object;
            best = candidate_metric;
            selection_status = 1;
        }
    }
    for (j = 0; j < 20; j++) {
        candidate_object = D_L13_001D1E90[j];
        if (candidate_object == 0)
            continue;
        if (*(short *)(candidate_object + 0xA6) != 0x10E)
            continue;
        if (*(unsigned char *)(candidate_object + 0x20) == 0xFE)
            continue;
        if (*(unsigned char *)(candidate_object + 0x20) == 0xFD)
            continue;
        if (*(unsigned char *)(candidate_object + 0xBC) == 0)
            continue;
        candidate_metric = vector_distance_xy(source_object + 0x10, candidate_object + 0x10);
        if (candidate_metric < best) {
            selected_object = candidate_object;
            best = candidate_metric;
            selection_status = 1;
        }
    }
    if (selected_object != 0) {
        *(char **)(selection_output + 0x40) = selected_object;
        qcopy(selection_output, selected_object + 0x10);
        qcopy(selection_output + 0x10, selected_object + 0x40);
        hero = &hero_lui;
        if (selected_object == ((char *)hero->moby)) {
            qcopy(selection_output + 0x20, &hero->motion.near_ground_point);
            qcopy(selection_output + 0x30, &hero->motion.body_center);
        } else {
            qcopy(selection_output + 0x30, selection_output);
            *(float *)(selection_output + 0x38) += 0.6f;
            qcopy(selection_output + 0x20, selection_output + 0x30);
        }
    } else {
        *(char **)(selection_output + 0x40) = 0;
        qzero(selection_output);
        qzero(selection_output + 0x10);
        qzero(selection_output + 0x30);
        qzero(selection_output + 0x20);
    }
    *(int *)(selection_output + 0x44) = selection_status;
    return selection_status;
}

/* Allocates and fills a particle-process record attached to a moby. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/partupd_002811B0.c: func_L13_002811B0), where it is exact; names translated to the US level program. */

unsigned char *D_L13_001B2490 NOT_SDA = 0;
unsigned char                *create_moby_joint_particle(char         *moby,
                                                         unsigned char joint_index,
                                                         int           b,
                                                         float         x,
                                                         float         y,
                                                         int           c,
                                                         float         z,
                                                         unsigned char d) __asm__("FUN_L13_00280400");

unsigned char *create_moby_joint_particle(char         *moby,
                                          unsigned char joint_index,
                                          int           b,
                                          float         x,
                                          float         y,
                                          int           c,
                                          float         z,

                                          unsigned char d) {
    unsigned char *particle = create_part(0x44);
    if (particle != 0) {
        char          *world_position;
        int            particle_bias;
        unsigned short moby_a6_value;
        unsigned char *attachment_data;
        *(int *)(particle + 4) = b;
        *(int *)(particle + 0xC) = c;
        world_position = (char *)particle + 0x10;
        particle_bias = truncate_float_to_s32(0.4f);
        particle[9] = (((short)((unsigned short)MOBY(moby)->draw_dist) >> 5) << 4) + particle_bias;
        particle[3] = 0x48;
        particle[1] = 3;
        particle[2] = *D_L13_001B2490;
        moby_point_position(moby, joint_index, world_position);
        qcopy(particle + 0x20, world_position);
        *(float *)(particle + 0x2C) = y;
        *(float *)(particle + 0x1C) = x;
        *(char **)(particle + 0x30) = moby;
        attachment_data = particle + 0x30;
        moby_a6_value = ((unsigned short)MOBY(moby)->oclass);
        *(short *)(attachment_data + 6) = moby_a6_value;
        attachment_data[4] = joint_index;
        *(float *)(attachment_data + 8) = x;
        *(float *)(attachment_data + 0xC) = z;
        attachment_data[5] = d;
    }
    return particle;
}

extern void kill_part(void *) __asm__("FUN_L00_00267a08");

void update_robosquawker_particle(struct Moby *moby, char *data) __asm__("FUN_L13_002b4f28");

void update_robosquawker_particle(struct Moby *moby, char *data) {
    float  opacity = 0.2f;
    void **handle = (void **)(data + 0x228);
    if (moby->visible) {
        if (!*handle) {
            *handle = create_moby_joint_particle(
                moby, 0, 0x80808080, opacity, opacity, 0x10808080, 0.8f, 0x19);
        }
    } else if (*handle) {
        kill_part(*handle);
        *handle = 0;
    }
}

typedef struct {
    char  pad0[0x228];
    void *value;
} Level13VendorMoby;

void release_robosquawker_particle(Level13VendorMoby *moby) __asm__("FUN_L13_002b4fb8");

void release_robosquawker_particle(Level13VendorMoby *moby) {
    if (moby->value != 0) {
        kill_part(moby->value);
        moby->value = 0;
    }
}

extern f32 D_0015ED70;

void update_robosquawker_ground_height(struct Moby *moby,
                                       char        *moby_vars) __asm__("FUN_L13_002b4ff0");

void update_robosquawker_ground_height(struct Moby *moby, char *moby_vars) {
    float ground = probe_ground_height(&moby->pos, 0, 0.5f);
    float z = moby->pos.z;
    if (ground < z + 0.3f) {
        float vertical_step = *(float *)(moby_vars + 0x128) - *(float *)&D_0015ED70 * 9.8f;
        *(float *)(moby_vars + 0x128) = vertical_step;
        moby->pos.z = moby->pos.z + vertical_step;
        if (moby->pos.z < ground)
            moby->pos.z = ground;
    } else if (z < ground) {
        moby->pos.z = z + *(float *)&D_0015ED70 * 9.8f;
        qzero(moby_vars + 0x120);
    } else {
        qzero(moby_vars + 0x120);
    }
}

typedef struct {
    float f[4];
} RsVec __attribute__((aligned(16)));

typedef struct {
    char          pad0[0x20];
    unsigned char state;
    char          pad21[0x85];
    short         hA6;
} RsTargetMoby;

typedef struct {
    RsVec         pos;
    RsVec         v10;
    RsVec         v20;
    RsVec         v30;
    RsTargetMoby *moby;
    int           kind;
} RsTarget;

typedef struct {
    char          pad0[0x20];
    float         f20;
    short         h24;
    short         h26;
    unsigned char b28;
    unsigned char b29;
    char          pad2A[0xE];
    int           w38;
    char          pad3C[4];
    RsVec         v40;
    char          pad50[8];
    unsigned char b58;
    char          pad59;
    unsigned char b5A;
    char          pad5B[0x6D];
    short         hC8;
    char          padCA[6];
    void         *pD0;
    char          padD4[0x3C];
    char          x110[0x10];
    char          x120[0x10];
    float         f130;
    float         f134;
    float         f138;
    float         f13C;
    int           w140;
    int           w144;
    float         f148;
    char          pad14C[0x11];
    unsigned char b15D;
    char          pad15E[0x12];
    float         f170;
    float         f174;
    char          pad178[8];
    int           w180;
    int           w184;
    float         f188;
    float         f18C;
    char          pad190[8];
    float         f198;
    char          pad19C[8];
    float         f1A4;
    char          pad1A8[0x10];
    int           w1B8;
    char          pad1BC[0x14];
    RsVec         v1D0;
    RsVec         v1E0;
    float         f1F0;
    float         f1F4;
    float         f1F8;
    float         f1FC;
    float         f200;
    float         f204;
    short         h208;
    short         h20A;
    int           w20C;
    int           w210;
    short         h214;
    short         h216;
    char         *p218;
    int           w21C;
    int           w220;
    int           w224;
    void         *p228;
    int           w22C;
    unsigned char x230[1];
} RsVars;

typedef struct {
    char           pad0[0x10];
    RsVec          pos;
    unsigned char  state;
    unsigned char  b21;
    char           pad22[2];
    char          *w24;
    char           pad28[9];
    unsigned char  b31;
    char           pad32[2];
    unsigned short h34;
    char           pad36[0xA];
    RsVec          rot;
    char           pad50[2];
    unsigned char  b52;
    unsigned char  b53;
    char           pad54[4];
    float          f58;
    char           pad5C[0x14];
    unsigned char  b70;
    char           pad71[7];
    RsVars        *vars;
    char           pad7C[3];
    unsigned char  b7F;
    char           pad80[0x14];
    int            w94;
    char           pad98[0xC];
    unsigned char  bA4;
    char           padA5;
    unsigned short hA6;
    char           padA8[0x14];
    unsigned char  bBC;
} RsMoby;

typedef struct RsOther {
    char            pad0[0x10];
    RsVec           pos;
    signed char     state;
    char            pad21[7];
    struct RsOther *next;
    char            pad2C[0x7A];
    short           hA6;
} RsOther;

typedef struct {
    char    pad0[0x10];
    RsVec   pos;
    RsMoby *src;
    char    pad24[8];
    float   f2C;
} RsHit;

extern char *level13_moby_list_head __asm__("D_L13_0015FFE4");

extern char *FUN_L00_0025a420(void *, int, int);

extern float robosquawker_damage_knockback_vertical_speed_gp __asm__("D_L13_00161390")
    __attribute__((sda));

extern float robosquawker_damage_knockback_horizontal_speed_gp __asm__("D_L13_00161394")
    __attribute__((sda));

extern float robosquawker_home_hop_horizontal_speed_gp __asm__("D_L13_00161398")
    __attribute__((sda));

#include "rnc/overlay/l13/mobys/robosquawker_data/robosquawker_update.h"

extern float FUN_L04_001f1978(float);

extern float FUN_L00_0025abf0(void *, void *, void *, float, float);

extern float approach_value(float *, float, float);

extern int D_L13_0015F5C4;

extern int D_L13_001613B0[] __attribute__((section(".sdata")));

extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");

extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

extern int FUN_L00_00258830(void *, void *, float, float, float, int);

extern int FUN_L00_00258b50(void *, void *, void *, void *, float);

extern int FUN_L00_002dc9f0(void *, void *);

extern int select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");

extern int
select_list_point_toward_target(void *, void *, void *, void *) __asm__("FUN_L01_0028b5e0");

extern int reserve_expired_frame_slot(int *, int, int) __asm__("FUN_L01_002efbf8");

extern s32 rand(void);

short D_L13_001613A0 = 0x505;

extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);

extern void FUN_L00_0025a120(void *);

extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");

extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");

extern void FUN_L00_0025f730(void *, float);

extern void FUN_L01_0026d930(char *p);

extern void FUN_L01_0026e090(int, int);

void update_robosquawker_ground_height(struct Moby *m, char *d) __asm__("FUN_L13_002b4ff0");

void robosquawker_update(RsMoby *m) __asm__("FUN_L13_002b50d8");

void robosquawker_update(RsMoby *robosquawker_moby) {
    enum {
        ROBOSQUAWKER_STATE_INITIALIZE = 0,
        ROBOSQUAWKER_STATE_GROUND_IDLE = 1,
        ROBOSQUAWKER_STATE_FACE_AWAY_WAIT_FOR_CHASE = 2,
        ROBOSQUAWKER_STATE_CHASE_TARGET = 3,
        ROBOSQUAWKER_STATE_PECK_ATTACK = 4,
        ROBOSQUAWKER_STATE_RETURN_HOME = 5,
        ROBOSQUAWKER_STATE_DAMAGE_KNOCKBACK = 6,
        ROBOSQUAWKER_STATE_RECOVER_TO_FLIGHT_IDLE = 7,
        ROBOSQUAWKER_STATE_WAIT_FOR_CLOSE_TARGET = 8,
        ROBOSQUAWKER_STATE_PREPARE_HOME_HOP = 0xA,
        ROBOSQUAWKER_STATE_HOME_HOP = 0xB,
        ROBOSQUAWKER_STATE_BLOCKED_RETURN_HOME = 0xC,
        ROBOSQUAWKER_STATE_WALK_FORWARD_AVOID_MOBYS = 0xD,
        ROBOSQUAWKER_STATE_FLIGHT_IDLE = 0xE,
        ROBOSQUAWKER_STATE_FLY_TO_RANDOM_POINT = 0xF,
        ROBOSQUAWKER_STATE_TAKEOFF = 0x10,
        ROBOSQUAWKER_STATE_LAND = 0x11,
        ROBOSQUAWKER_STATE_FOLLOW_FLIGHT_PATH = 0x12,
        ROBOSQUAWKER_STATE_DEATH_KNOCKBACK = 0x63
    };
    RsVars       *robosquawker_vars;
    RsVec         old;
    RsTarget      selected_target_record;
    RsVec         va;
    RsVec         vb;
    RsVec         vc;
    int           hits;
    float         dmg;
    float         knockback_yaw;
    float         flight_step_length;
    float         out;
    RsHit        *hit;
    RsTargetMoby *tm;

    robosquawker_vars = robosquawker_moby->vars;
    FUN_L00_00263ac8(2.5f, (int)robosquawker_moby, 0, robosquawker_vars->x230);
    if (robosquawker_moby->b31 != 0) {
        if (fast_vec_dist(&robosquawker_moby->pos, D_L13_001670C0) < 28.0f) {
            FUN_L00_0025a120(robosquawker_moby);
            robosquawker_moby->b7F = 0x16;
        }
    }
    if (D_L13_0015F5C4 == 2) {
        robosquawker_moby->w94 = 0;
        robosquawker_moby->h34 |= 0x41;
        return;
    }
    robosquawker_moby->w94 = *(int *)(robosquawker_moby->w24 + 0x10);
    robosquawker_moby->h34 &= 0xFFBE;
    select_robosquawker_target(
        (char *)robosquawker_moby, (char *)&selected_target_record, robosquawker_vars->f1FC + 4.0f);
    tm = selected_target_record.moby;
    if (tm == 0 ||
        (robosquawker_vars->h214 == 0 && robosquawker_vars->w22C != -1 &&
         is_point_inside_clip_volume(&selected_target_record, robosquawker_vars->w22C) == 0)) {
        selected_target_record.moby = 0;
        tm = 0;
        qzero(&selected_target_record);
    }
    qcopy(&old, &robosquawker_moby->pos);
    if (robosquawker_vars->w38 != 0) {
        robosquawker_vars->h214 = scale_game_frames(0xF0);
    }
    robosquawker_vars->w38 = 0;
    fast_dec_timer_s16(&robosquawker_vars->h208);
    if (robosquawker_moby->state != 0) {
        if (fast_dec_timer_s16(&robosquawker_vars->h214) != 0) {
            robosquawker_vars->f1FC = robosquawker_vars->v1D0.f[3];
        } else {
            robosquawker_vars->f1FC = robosquawker_vars->f200;
        }
    }
    dmg = 0.0f;
    hit = FUN_L00_0025a420(robosquawker_moby, 0x330000, 0);
    resolve_moby_hit_response(
        robosquawker_moby, hit, &robosquawker_vars->f20, 0, &hits, &dmg, 0, 4);
    fast_dec_timer_s16(&robosquawker_vars->h26);
    if (robosquawker_vars->h26 < scale_game_frames(0x2D) && hit != 0 &&
        robosquawker_moby->state != 0x63) {
        if (hit->src->hA6 >= 0xB0 && hit->src->hA6 <= 0xB1 && robosquawker_vars->h26 != 0) {
            hit->f2C = 0.0f;
        }
        if (robosquawker_moby->b21 != 0xFF) {
            FUN_L01_0026e090(robosquawker_moby->b21, 1);
        }
        if (hit->f2C != 0.0f) {
            knockback_yaw =
                fast_arc_tan(robosquawker_moby->pos.f[0] - selected_target_record.pos.f[0],
                             robosquawker_moby->pos.f[1] - selected_target_record.pos.f[1]);
            robosquawker_vars->f148 = 0.5f;
            robosquawker_vars->w140 = 0x202;
            robosquawker_vars->f20 -= hit->f2C;
            robosquawker_vars->f130 = 0.0055f;
            robosquawker_vars->f134 = 0.0011f;
            robosquawker_vars->f138 =
                robosquawker_damage_knockback_horizontal_speed_gp * frame_time;
            robosquawker_vars->f13C = robosquawker_damage_knockback_vertical_speed_gp * frame_time;
            robosquawker_vars->w144 = 9;
            robosquawker_vars->b15D = 0;
            if (robosquawker_vars->f20 <= 0.0f) {
                robosquawker_moby->h34 &= 0xEFFF;
                robosquawker_vars->f138 = frame_time * 10.0f;
                robosquawker_vars->f13C = frame_time * 5.5f;
                *(u128 *)&va = *(u128 *)&hit->pos;
                trooper_knockback_arc(
                    &va, &knockback_yaw, &robosquawker_vars->f138, &robosquawker_vars->f13C);
                trooper_start_knockback(robosquawker_moby,
                                        robosquawker_vars->x120,
                                        knockback_yaw,
                                        (rand() & 1) + 9,
                                        1,
                                        0);
                robosquawker_vars->f170 = 11.0f;
                robosquawker_vars->f174 = 18.0f;
                robosquawker_moby->state = 0x63;
                release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
                robosquawker_vars->x110[7] = 0x78;
                trooper_anim_update(robosquawker_moby, (short *)robosquawker_vars->x110);
                FUN_L00_00257470(robosquawker_moby, 0, -1);
            } else {
                *(u128 *)&va = *(u128 *)&hit->pos;
                trooper_knockback_arc(
                    &va, &knockback_yaw, &robosquawker_vars->f138, &robosquawker_vars->f13C);
                trooper_start_knockback(robosquawker_moby,
                                        robosquawker_vars->x120,
                                        knockback_yaw,
                                        (rand() & 1) + 9,
                                        1,
                                        0);
                robosquawker_vars->f170 = 5.0f;
                robosquawker_vars->f174 = 10.0f;
                robosquawker_moby->state = 6;
                release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
                *(unsigned char *)&robosquawker_vars->x110[7] = 0xFA;
                robosquawker_vars->h26 = scale_game_frames(0x3C);
                trooper_anim_update(robosquawker_moby, (short *)robosquawker_vars->x110);
            }
        }
    }
    robosquawker_moby->bA4 = 0xFF;
    switch (robosquawker_moby->state) {
    case ROBOSQUAWKER_STATE_INITIALIZE:
        qcopy(&robosquawker_vars->v1D0, &robosquawker_moby->pos);
        robosquawker_vars->v1D0.f[3] = robosquawker_vars->f1FC;
        if (robosquawker_vars->h20A != 0) {
            robosquawker_moby->state = 1;
            if (robosquawker_moby->b53 != 3) {
                blend_moby_animation(robosquawker_moby, 3, 0, 0);
            }
        } else {
            robosquawker_moby->state = 0xE;
            robosquawker_moby->pos.f[2] += 6.0f;
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, 0);
            }
        }
        robosquawker_vars->h214 = 0;
        robosquawker_vars->h208 = 0;
        robosquawker_vars->b29 = 0;
        robosquawker_vars->f20 = 1.0f;
        robosquawker_vars->h24 = 1;
        robosquawker_vars->b58 = 8;
        robosquawker_vars->b5A = 4;
        FUN_L01_0026d930((char *)&robosquawker_vars->w180);
        robosquawker_vars->w180 = 0x38D;
        robosquawker_vars->f18C = 2.0f;
        robosquawker_vars->f188 = 2.0f;
        robosquawker_vars->f1A4 = robosquawker_vars->f204 * frame_time;
        robosquawker_vars->w1B8 |= 0x30;
        if (rand() & 1) {
            robosquawker_moby->h34 |= 0x8000;
        }
        robosquawker_vars->pD0 = &D_L13_001613A0;
        if (robosquawker_vars->w220 != -1) {
            robosquawker_vars->w224 = select_point_index_by_distance(
                &robosquawker_moby->pos, walker_paths[robosquawker_vars->w220], 0.0f);
        }
        break;
    case ROBOSQUAWKER_STATE_FLIGHT_IDLE: {
        float d;

        update_robosquawker_particle((unsigned char *)robosquawker_moby, (char *)robosquawker_vars);
        fast_vec_sub(&va, &selected_target_record, &robosquawker_moby->pos);
        d = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        if (d > 1.0f && vector_length_xy(&va) < robosquawker_vars->f1FC + 4.0f &&
            absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 10.0f &&
            random_integer_below(0x13) == 0) {
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
            }
            robosquawker_moby->state = 0x11;
            release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
        } else if (d > 1.0f && robosquawker_moby->bBC == 1 && random_integer_below(4) == 0) {
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
            }
            release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
            robosquawker_moby->state = 0x11;
            clear_u64_value(&robosquawker_vars->v40);
            robosquawker_moby->bBC = 0;
        } else if (robosquawker_vars->w220 != -1) {
            char *path;

            path = walker_paths[robosquawker_vars->w220];
            if (fast_vec_dist(&robosquawker_moby->pos, path + (robosquawker_vars->w224 * 16 + 16)) <
                0.25f) {
                robosquawker_vars->w224 += 1;
                if (robosquawker_vars->w224 >= *(int *)path) {
                    robosquawker_vars->w224 = 0;
                }
            }
            qcopy(&robosquawker_vars->v1E0, path + robosquawker_vars->w224 * 16 + 16);
            robosquawker_moby->state = 0x12;
        } else if (fast_dec_timer(&robosquawker_vars->w20C) != 0) {
            int i;

            robosquawker_vars->w20C = scale_game_frames(FUN_L00_00257b90(0x28, 0x78));
            for (i = 0; i < 10; i++) {
                float r;
                float a;

                r = random_float_between(0.0f, 4.0f);
                a = random_angle_radians();
                vb.f[0] = fast_cos(a) * r;
                vb.f[1] = fast_sin(a) * r;
                vb.f[2] = 0.0f;
                vb.f[2] = random_float_between(5.0f, 7.0f);
                fast_vec_add(&robosquawker_vars->v1E0, &vb, &robosquawker_vars->v1D0);
                if (probe_ground_height(&robosquawker_vars->v1E0, 0x120, 0.5f) > 1.0f) {
                    break;
                }
            }
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
            }
            robosquawker_moby->state = 0xF;
        }
        break;
    }
    case ROBOSQUAWKER_STATE_FLY_TO_RANDOM_POINT: {
        float a;

        a = fast_arc_tan(robosquawker_vars->v1E0.f[0] - robosquawker_moby->pos.f[0],
                         robosquawker_vars->v1E0.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278(
            (char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.03f, 0.6f, 0.09f);
        flight_step_length = fast_vec_length(&robosquawker_vars->v40);
        robosquawker_moby->f58 = flight_step_length * 20.0f + 1.0f;
        fast_vec_sub(&robosquawker_vars->v40, &robosquawker_vars->v1E0, &robosquawker_moby->pos);
        if (fast_vec_length(&robosquawker_vars->v40) > 1.0f) {
            approach_value(&flight_step_length, frame_time * 4.0f, frame_time_sq * 6.0f);
        } else {
            approach_value(&flight_step_length, frame_time, frame_time_sq * 8.0f);
        }
        FUN_L00_0025f730(&robosquawker_vars->v40, flight_step_length);
        fast_vec_add(&va, &robosquawker_vars->v40, &robosquawker_moby->pos);
        if (coll_sphere(1.2f, &va, 0, robosquawker_moby) != 0) {
            clear_u64_value(&robosquawker_vars->v40);
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
            }
            robosquawker_moby->state = 0xE;
        } else {
            qcopy(&robosquawker_moby->pos, &va);
        }
        if (fast_vec_dist(&robosquawker_moby->pos, &robosquawker_vars->v1E0) < flight_step_length ||
            flight_step_length == 0.0f) {
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
            }
            robosquawker_moby->state = 0xE;
        }
        update_robosquawker_particle((unsigned char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_FOLLOW_FLIGHT_PATH: {
        char *path;
        float a;
        float d;

        path = walker_paths[robosquawker_vars->w220];
        a = fast_arc_tan(robosquawker_vars->v1E0.f[0] - robosquawker_moby->pos.f[0],
                         robosquawker_vars->v1E0.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278(
            (char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.018f, 0.8f, 0.09f);
        fast_vec_sub(&robosquawker_vars->v40, &robosquawker_vars->v1E0, &robosquawker_moby->pos);
        FUN_L00_0025f730(&robosquawker_vars->v40, frame_time * 4.0f);
        fast_vec_add(&va, &robosquawker_vars->v40, &robosquawker_moby->pos);
        robosquawker_moby->f58 = fast_vec_length(&robosquawker_vars->v40) * 20.0f + 1.0f;
        if (coll_sphere(1.2f, &va, 0, robosquawker_moby) != 0) {
            int i;

            robosquawker_vars->w20C = scale_game_frames(0x78);
            for (i = 0; i < 10; i++) {
                float r;
                float an;

                r = random_float_between(0.0f, 3.0f);
                an = random_angle_radians();
                vb.f[0] = fast_cos(an) * r;
                vb.f[1] = fast_sin(an) * r;
                vb.f[2] = 0.0f;
                vb.f[2] = random_float_between(5.0f, 7.0f);
                fast_vec_add(&robosquawker_vars->v1E0, &vb, &robosquawker_vars->v1D0);
                if (probe_ground_height(&robosquawker_vars->v1E0, 0x120, 0.5f) > 1.0f) {
                    break;
                }
            }
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
            }
            robosquawker_moby->state = 0xF;
        } else {
            qcopy(&robosquawker_moby->pos, &va);
        }
        d = fast_vec_dist(&robosquawker_moby->pos, &robosquawker_vars->v1E0);
        if (d < fast_vec_length(&robosquawker_vars->v40)) {
            float g;

            robosquawker_vars->w224 += 1;
            if (robosquawker_vars->w224 >= *(int *)path) {
                robosquawker_vars->w224 = 0;
            }
            qcopy(&robosquawker_vars->v1E0, path + robosquawker_vars->w224 * 16 + 16);
            robosquawker_vars->v1E0.f[2] =
                robosquawker_vars->v1E0.f[2] + random_float_between(-0.2f, 0.2f);
            g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
            if (g > 1.0f && vector_length_xy(&vb) < robosquawker_vars->f1FC + 4.0f &&
                absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) <
                    10.0f &&
                random_integer_below(0x13) == 0) {
                if (robosquawker_moby->b53 != 0) {
                    blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
                }
                qzero(&robosquawker_vars->v40);
                robosquawker_moby->state = 0x11;
                release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
            } else if (g > 1.0f && robosquawker_moby->bBC == 1 && random_integer_below(4) == 0) {
                if (random_integer_below(0x100) & 1) {
                    robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
                }
                robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
                robosquawker_vars->w210 =
                    scale_game_frames((int)random_float_between(30.0f, 90.0f));
                if (robosquawker_moby->b53 != 0) {
                    blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
                }
                release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
                qzero(&robosquawker_vars->v40);
                robosquawker_moby->state = 0x11;
                clear_u64_value(&robosquawker_vars->v40);
                robosquawker_moby->bBC = 0;
            }
        }
        update_robosquawker_particle((unsigned char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_LAND: {
        float g;

        g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        approach_value(&robosquawker_vars->v40.f[2], -(frame_time * 8.0f), frame_time_sq * 10.8f);
        FUN_L00_00258830(robosquawker_moby, &robosquawker_vars->v40, 0.6f, 0.5f, 0.0f, 0);
        if (robosquawker_moby->b53 != 2 &&
            robosquawker_moby->pos.f[2] - g <= frame_time * 8.0f * FUN_L04_001f1978(45.0f) &&
            robosquawker_moby->b53 != 2) {
            blend_moby_animation(robosquawker_moby, 2, 0, scale_game_frames(5));
        }
        if (robosquawker_moby->pos.f[2] <= g) {
            robosquawker_moby->pos.f[2] = g;
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            robosquawker_moby->state = 3;
            clear_u64_value(&robosquawker_vars->v40);
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
        }
        break;
    }
    case ROBOSQUAWKER_STATE_TAKEOFF: {
        float g;

        g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        if (robosquawker_moby->pos.f[2] - g < 3.0f) {
            approach_value(&robosquawker_vars->v40.f[2], frame_time * 8.0f, frame_time_sq * 10.8f);
        } else {
            approach_value(&robosquawker_vars->v40.f[2], frame_time, frame_time_sq * 10.8f);
        }
        FUN_L00_00258830(robosquawker_moby, &robosquawker_vars->v40, 0.6f, 0.5f, 0.0f, 0);
        robosquawker_moby->f58 = 6.0f - (robosquawker_moby->pos.f[2] - g) / 6.0f * 5.0f;
        if (g + 6.0f <= robosquawker_moby->pos.f[2]) {
            robosquawker_moby->pos.f[2] = g + 6.0f;
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
            }
            robosquawker_moby->state = 0xE;
            clear_u64_value(&robosquawker_vars->v40);
        }
        update_robosquawker_particle((unsigned char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_GROUND_IDLE:
        fast_vec_sub(&va, &selected_target_record, &robosquawker_moby->pos);
        if (fast_vec_length(&va) < robosquawker_vars->f1FC + 4.0f &&
            absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            robosquawker_moby->state = 2;
            if (robosquawker_moby->b53 != 4) {
                blend_moby_animation(robosquawker_moby, 4, 0, scale_game_frames(3));
            }
        } else if (robosquawker_moby->bBC == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
            robosquawker_moby->bBC = 0;
        } else if (fast_vec_length(&va) > robosquawker_vars->f1FC + 6.0f &&
                   robosquawker_vars->h20A == 0) {
            qzero(&robosquawker_vars->v40);
            robosquawker_moby->state = 0x10;
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
            }
        }
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    case ROBOSQUAWKER_STATE_WAIT_FOR_CLOSE_TARGET:
        fast_vec_sub(&va, &selected_target_record, &robosquawker_moby->pos);
        if (fast_vec_length(&va) < robosquawker_vars->f1FC - 4.0f &&
            absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            robosquawker_moby->state = 2;
            robosquawker_moby->pos.f[2] += 1.0f;
            if (robosquawker_moby->b53 != 4) {
                blend_moby_animation(robosquawker_moby, 4, 0, scale_game_frames(3));
            }
        }
        break;
    case ROBOSQUAWKER_STATE_PREPARE_HOME_HOP: {
        float a;

        a = fast_arc_tan(robosquawker_vars->v1D0.f[0] - robosquawker_moby->pos.f[0],
                         robosquawker_vars->v1D0.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.05f, 0.3f, 0.2f);
        fast_vec_sub(&va, &selected_target_record, &robosquawker_vars->v1D0);
        if ((fast_vec_length(&va) < robosquawker_vars->f1FC &&
             absolute_float(robosquawker_vars->v1D0.f[2] - selected_target_record.pos.f[2]) <
                 8.0f) ||
            robosquawker_moby->bBC == 2) {
            if (robosquawker_moby->b21 != 0xFF) {
                FUN_L01_0026e090(robosquawker_moby->b21, 2);
            }
            if (robosquawker_moby->b53 != (rand() & 1) + 9) {
                blend_moby_animation(robosquawker_moby, (rand() & 1) + 9, 0, scale_game_frames(3));
            }
            robosquawker_vars->f134 = 0.0f;
            robosquawker_vars->f130 = robosquawker_home_hop_gravity_gp;
            robosquawker_vars->f138 = robosquawker_home_hop_horizontal_speed_gp * frame_time;
            robosquawker_vars->f13C =
                FUN_L00_0025abf0(&robosquawker_moby->pos,
                                 &robosquawker_vars->v1D0,
                                 &out,
                                 robosquawker_home_hop_horizontal_speed_gp * frame_time,
                                 -robosquawker_home_hop_gravity_gp);
            robosquawker_vars->w144 = 0xD;
            robosquawker_vars->b15D = 0;
            trooper_start_knockback(robosquawker_moby,
                                    robosquawker_vars->x120,
                                    robosquawker_moby->rot.f[2],
                                    (rand() & 1) + 9,
                                    1,
                                    0);
            robosquawker_vars->f170 = 5.0f;
            robosquawker_vars->f174 = 10.0f;
            update_robosquawker_particle((unsigned char *)robosquawker_moby,
                                         (char *)robosquawker_vars);
            robosquawker_moby->state = 0xB;
        }
        break;
    }
    case ROBOSQUAWKER_STATE_HOME_HOP:
        if (step_moby_motion(robosquawker_moby, robosquawker_vars->x120) & 1) {
            release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
            qzero(robosquawker_vars->x120);
            robosquawker_moby->state = 3;
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 8, scale_game_frames(3));
            }
            return;
        } else if (robosquawker_moby->pos.f[2] < 5.0f) {
            FUN_L00_00257470(robosquawker_moby, 0, -1);
            mark_moby_for_removal(robosquawker_moby);
            return;
        }
        break;
    case ROBOSQUAWKER_STATE_FACE_AWAY_WAIT_FOR_CHASE: {
        float a;

        a = fast_arc_tan(robosquawker_moby->pos.f[0] - selected_target_record.pos.f[0],
                         robosquawker_moby->pos.f[1] - selected_target_record.pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.02f, 0.3f, 0.1f);
        fast_vec_sub(&va, &selected_target_record, &robosquawker_moby->pos);
        if (fast_vec_length(&va) < robosquawker_vars->f1FC &&
            absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            if (robosquawker_moby->b21 != 0xFF) {
                FUN_L01_0026e090(robosquawker_moby->b21, 1);
            }
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
            robosquawker_moby->bBC = 0;
        } else if (robosquawker_moby->bBC == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
            robosquawker_moby->bBC = 0;
        }
        if (robosquawker_moby->b53 == 4 && (robosquawker_moby->b70 & 2)) {
            blend_moby_animation(robosquawker_moby, 3, 0, scale_game_frames(3));
        }
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_CHASE_TARGET: {
        float r;
        int   res;
        float a;
        float d;

        r = absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]);
        r = r / vector_distance_xy(&robosquawker_moby->pos, &selected_target_record);
        if (robosquawker_vars->w21C != -1) {
            select_list_point_toward_target(robosquawker_moby,
                                            walker_paths[robosquawker_vars->w21C],
                                            &selected_target_record,
                                            &vb);
        } else {
            qcopy(&vb, &selected_target_record);
        }
        a = fast_arc_tan(vb.f[0] - robosquawker_moby->pos.f[0],
                         vb.f[1] - robosquawker_moby->pos.f[1]) +
            robosquawker_vars->f1F4;
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.05f, 0.3f, 0.2f);
        va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[2] = 0.0f;
        res = FUN_L00_00258b50(robosquawker_moby, &robosquawker_vars->w180, &va, &vb, 1.0f);
        if (fast_dec_timer(&robosquawker_vars->w210) != 0) {
            robosquawker_vars->w210 = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            robosquawker_vars->f1F4 = -robosquawker_vars->f1F4;
        }
        fast_vec_sub(&vc, &selected_target_record, &robosquawker_vars->v1D0);
        d = vector_distance_xy(&robosquawker_moby->pos, &selected_target_record);
        if ((d < 1.5f ||
             (selected_target_record.moby != 0 && selected_target_record.moby->hA6 == 0x10E &&
              selected_target_record.moby->state != 0xFE &&
              selected_target_record.moby->state != 0xFD && d < 2.3f)) &&
            !(res & 2)) {
            robosquawker_moby->state = 4;
            if (robosquawker_moby->b53 != 6) {
                blend_moby_animation(robosquawker_moby, 6, 0, scale_game_frames(3));
            }
        } else {
            if (r > 1.0f || (res & 2)) {
                robosquawker_moby->pos.f[0] = old.f[0];
                robosquawker_moby->pos.f[1] = old.f[1];
            }
            if (fast_vec_length(&vc) < robosquawker_vars->f1FC + 4.0f &&
                absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) <
                    8.0f) {
                if (robosquawker_moby->b21 != 0xFF) {
                    FUN_L01_0026e090(robosquawker_moby->b21, 1);
                }
            } else if (robosquawker_moby->bBC != 1) {
                robosquawker_moby->state = 5;
            }
        }
        robosquawker_moby->bBC = 0;
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_PECK_ATTACK: {
        float a;
        int   same;
        float hh;
        float dz;
        float d;
        float da;
        float g;

        a = fast_arc_tan(selected_target_record.pos.f[0] - robosquawker_moby->pos.f[0],
                         selected_target_record.pos.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.05f, 0.3f, 0.2f);
        same = robosquawker_moby->b52 == robosquawker_moby->b53;
        hh = compute_interpolated_record_value(robosquawker_moby);
        dz = absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]);
        d = vector_distance_xy(&robosquawker_moby->pos, &selected_target_record);
        da = fast_difference_between_rotations(
            fast_arc_tan(selected_target_record.pos.f[0] - robosquawker_moby->pos.f[0],
                         selected_target_record.pos.f[1] - robosquawker_moby->pos.f[1]),
            robosquawker_moby->rot.f[2]);
        if (same && absolute_float(hh - 13.0f) < 0.2f && dz < 1.5f &&
            (d < 2.0f ||
             (selected_target_record.moby != 0 && selected_target_record.moby->hA6 == 0x10E &&
              selected_target_record.moby->state != 0xFE &&
              selected_target_record.moby->state != 0xFD && d < 2.6f)) &&
            da < 0.2617994f) {
            va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * 0.2f;
            va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * 0.2f;
            va.f[2] = 0.0f;
            qcopy(&vb, &selected_target_record);
            vb.f[2] += 0.75f;
            if (tm != 0) {
                spawn_nearby_effect(tm, (int)robosquawker_moby, 1, &vb, &va, 1.0f);
            }
        }
        if ((robosquawker_moby->b70 & 2) &&
            vector_distance_xy(&robosquawker_moby->pos, &selected_target_record) > 1.5f) {
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
        }
        g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        robosquawker_vars->f198 -= frame_time_sq * 9.8f;
        robosquawker_moby->pos.f[2] += robosquawker_vars->f198;
        if (robosquawker_moby->pos.f[2] < g) {
            robosquawker_vars->f198 = 0.0f;
            robosquawker_moby->pos.f[2] = g;
        }
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_RETURN_HOME: {
        int   res;
        float a;

        if (robosquawker_vars->w21C != -1) {
            select_list_point_toward_target(robosquawker_moby,
                                            walker_paths[robosquawker_vars->w21C],
                                            &robosquawker_vars->v1D0,
                                            &vb);
        } else {
            qcopy(&vb, &robosquawker_vars->v1D0);
        }
        a = fast_arc_tan(vb.f[0] - robosquawker_moby->pos.f[0],
                         vb.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.02f, 0.3f, 0.1f);
        va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[2] = 0.0f;
        res = FUN_L00_00258b50(robosquawker_moby, &robosquawker_vars->w180, &va, &vb, 1.0f);
        if (vector_distance_xy(&robosquawker_moby->pos, &robosquawker_vars->v1D0) < 2.0f) {
            robosquawker_moby->state = 1;
            if (robosquawker_moby->b53 != 3) {
                blend_moby_animation(robosquawker_moby, 3, 0, scale_game_frames(3));
            }
        }
        fast_vec_sub(&vc, &selected_target_record, &robosquawker_vars->v1D0);
        if (fast_vec_length(&vc) < robosquawker_vars->f1FC) {
            if (robosquawker_moby->b21 != 0xFF) {
                FUN_L01_0026e090(robosquawker_moby->b21, 1);
            }
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
        } else if (robosquawker_moby->bBC == 1) {
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
        } else if (res & 2) {
            robosquawker_moby->pos.f[0] = old.f[0];
            robosquawker_moby->pos.f[1] = old.f[1];
            robosquawker_moby->state = 0xC;
            if (robosquawker_moby->b53 != 4) {
                blend_moby_animation(robosquawker_moby, 4, 0, scale_game_frames(3));
            }
        }
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        robosquawker_moby->bBC = 0;
        break;
    }
    case ROBOSQUAWKER_STATE_DAMAGE_KNOCKBACK:
        if (step_moby_motion(robosquawker_moby, robosquawker_vars->x120) & 1) {
            qzero(robosquawker_vars->x120);
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
            return;
        } else if (robosquawker_moby->pos.f[2] < 5.0f) {
            FUN_L00_00257470(robosquawker_moby, 0, -1);
            mark_moby_for_removal(robosquawker_moby);
            return;
        }
        break;
    case ROBOSQUAWKER_STATE_RECOVER_TO_FLIGHT_IDLE:
        release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
        if (FUN_L00_002dc9f0(robosquawker_moby, robosquawker_vars->x120) != 0) {
            robosquawker_moby->state = 0xE;
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation(robosquawker_moby, 0, 0, 0);
            }
            robosquawker_vars->hC8 = 0;
        }
        break;
    case ROBOSQUAWKER_STATE_DEATH_KNOCKBACK: {
        int slot_reuse_delay_frames;

        robosquawker_moby->h34 &= 0xEFFF;
        if (step_moby_motion(robosquawker_moby, robosquawker_vars->x120) & 0x140) {
            clear_u64_value(&va);
            slot_reuse_delay_frames = (int)((float)scale_game_frames(1) * 60.0f);
            if (reserve_expired_frame_slot(D_L13_001613B0, 3, slot_reuse_delay_frames) >= 0) {
                cart_burst(robosquawker_moby, &robosquawker_moby->pos, 4, 0.5f, 13.0f);
            }
            mark_moby_for_removal(robosquawker_moby);
            return;
        } else if (robosquawker_moby->pos.f[2] < 0.0f) {
            mark_moby_for_removal(robosquawker_moby);
            return;
        }
        break;
    }
    case ROBOSQUAWKER_STATE_BLOCKED_RETURN_HOME: {
        float r;
        int   res;
        float a;

        r = absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]);
        r = r / vector_distance_xy(&robosquawker_moby->pos, &selected_target_record);
        if (robosquawker_vars->w21C != -1) {
            select_list_point_toward_target(robosquawker_moby,
                                            walker_paths[robosquawker_vars->w21C],
                                            &robosquawker_vars->v1D0,
                                            &vb);
        } else {
            qcopy(&vb, &robosquawker_vars->v1D0);
        }
        a = fast_arc_tan(vb.f[0] - robosquawker_moby->pos.f[0],
                         vb.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.05f, 0.3f, 0.2f);
        va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[2] = 0.0f;
        res = FUN_L00_00258b50(robosquawker_moby, &robosquawker_vars->w180, &va, &vb, 1.0f);
        fast_vec_sub(&vc, &selected_target_record, &robosquawker_vars->v1D0);
        if (vector_distance_xy(&robosquawker_moby->pos, &selected_target_record) < 1.5f &&
            r <= 1.0f) {
            robosquawker_moby->state = 4;
            if (robosquawker_moby->b53 != 6) {
                blend_moby_animation(robosquawker_moby, 6, 0, scale_game_frames(3));
            }
        } else if (r <= 1.0f && !(res & 2)) {
            robosquawker_moby->state = 5;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
        } else {
            if (robosquawker_moby->b70 & 2) {
                int anim;

                anim = 4;
                random_integer_below(4);
                if (random_integer_below(4) != 0) {
                    anim = 3;
                }
                blend_moby_animation(robosquawker_moby, anim, 0, scale_game_frames(3));
            }
            if (random_integer_below(0x63) == 0) {
                qzero(&robosquawker_vars->v40);
                robosquawker_moby->state = 0x10;
                if (robosquawker_moby->b53 != 0) {
                    blend_moby_animation(robosquawker_moby, 0, 0, scale_game_frames(3));
                }
            }
        }
        robosquawker_moby->bBC = 0;
        break;
    }
    case ROBOSQUAWKER_STATE_WALK_FORWARD_AVOID_MOBYS: {
        RsOther *candidate_moby;
        float    g;

        if (robosquawker_moby->b52 != 5 && robosquawker_moby->b53 != 5) {
            blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
        }
        va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * (robosquawker_vars->f204 * frame_time);
        va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * (robosquawker_vars->f204 * frame_time);
        va.f[2] = 0.0f;
        fast_vec_add(&vb, &robosquawker_moby->pos, &va);
        for (candidate_moby = (RsOther *)level13_moby_list_head; candidate_moby != 0;
             candidate_moby = candidate_moby->next) {
            if (candidate_moby == (RsOther *)robosquawker_moby) {
                continue;
            }
            if (candidate_moby->state < 0) {
                continue;
            }
            if (candidate_moby->hA6 != 0 && candidate_moby->hA6 != 0x3F) {
                continue;
            }
            if (vector_distance_xy(&vb, &candidate_moby->pos) < 2.0f) {
                float z;
                float a;
                float d;

                z = robosquawker_moby->pos.f[2];
                a = fast_arc_tan(candidate_moby->pos.f[0] - robosquawker_moby->pos.f[0],
                                 candidate_moby->pos.f[1] - robosquawker_moby->pos.f[1]);
                a = fast_subtract_rotations(robosquawker_moby->rot.f[2], a);
                d = frame_time * 1.5707964f;
                if (!(a > 0.0f)) {
                    d = -d;
                }
                robosquawker_moby->rot.f[2] = fast_add_rotations(robosquawker_moby->rot.f[2], d);
                fast_vec_sub(&vb, &robosquawker_moby->pos, &candidate_moby->pos);
                fast_vec_normalize(&vb, &vb, 2.0f);
                fast_vec_add(&vb, &vb, &candidate_moby->pos);
                vb.f[2] = z;
            }
        }
        qcopy(&robosquawker_moby->pos, &vb);
        g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        robosquawker_vars->f198 -= frame_time_sq * 9.8f;
        robosquawker_moby->pos.f[2] += robosquawker_vars->f198;
        if (robosquawker_moby->pos.f[2] < g) {
            robosquawker_vars->f198 = 0.0f;
            robosquawker_moby->pos.f[2] = g;
        }
        if (robosquawker_vars->p218 == 0 ||
            vector_distance_xy(&robosquawker_moby->pos, robosquawker_vars->p218 + 0x10) >
                convert_integer_to_float(robosquawker_vars->h216)) {
            robosquawker_moby->state = 5;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation(robosquawker_moby, 5, 0, scale_game_frames(3));
            }
        }
        robosquawker_moby->bBC = 0;
        break;
    }
    }
    trooper_anim_finish(robosquawker_moby, robosquawker_vars->x110);
    if (robosquawker_moby->pos.f[0] < 2.0f || robosquawker_moby->pos.f[0] > 1021.0f ||
        robosquawker_moby->pos.f[1] < 2.0f || robosquawker_moby->pos.f[1] > 1021.0f ||
        robosquawker_moby->pos.f[2] < 2.0f || robosquawker_moby->pos.f[2] > 1021.0f) {
        mark_moby_for_removal(robosquawker_moby);
        return;
    }
}

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
