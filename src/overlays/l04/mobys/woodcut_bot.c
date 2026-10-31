/* Level 04 moby class woodcut_bot with its movement and death-effect helpers. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/level_path.h"
#include "sda.h"
#include "rnc/overlay/quad.h"

extern void initialize_leg_rig_angle_defaults(void *, char *) __asm__("FUN_L04_00292370");
extern void initialize_leg_rig(void *, char *) __asm__("FUN_L04_002923b8");

void initialize_woodcut_bot_movement_data(void *moby) __asm__("FUN_L04_002d1418");

void initialize_woodcut_bot_movement_data(void *moby) {
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    char *movement_data = moby_vars + 0x160;
    float speed;
    float angleHalf;
    float angleFull;
    float angleOther;
    float amount;
    float frameScale;
    float otherScale;
    *(int *)(moby_vars + 0x1d0) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1d4) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1d8) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1dc) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1e0) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1e4) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1e8) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1ec) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x54) + 0x14);
    *(int *)(moby_vars + 0x1f0) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1f4) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1f8) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x1fc) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    *(int *)(moby_vars + 0x200) = *(int *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x70) + 0x14);
    initialize_leg_rig_angle_defaults(moby, movement_data);
    frameScale = frame_time_sq;
    otherScale = frame_time;
    angleFull = frameScale * 6.2831855f;
    angleOther = otherScale * 6.2831855f;
    speed = frameScale * 9.8f;
    angleHalf = frameScale * 3.1415927f;
    amount = frameScale * 10.0f;
    *(unsigned char *)(moby_vars + 0x214) = 7;
    *(unsigned char *)(moby_vars + 0x210) = 0;
    *(unsigned char *)(moby_vars + 0x211) = 0;
    *(float *)(moby_vars + 0x23c) = speed;
    *(float *)(moby_vars + 0x22c) = angleHalf;
    *(float *)(moby_vars + 0x230) = angleFull;
    *(float *)(moby_vars + 0x234) = angleOther;
    *(float *)(moby_vars + 0x224) = amount;
    *(unsigned char *)(moby_vars + 0x212) = 0xff;
    *(unsigned char *)(moby_vars + 0x213) = 0x10;
    *(unsigned char *)(moby_vars + 0x215) = 7;
    *(unsigned char *)(moby_vars + 0x250) = 10;
    *(unsigned char *)(moby_vars + 0x251) = 8;
    *(unsigned char *)(moby_vars + 0x253) = 12;
    *(unsigned char *)(moby_vars + 0x252) = 14;
    *(unsigned char *)(moby_vars + 0x300) = 11;
    *(unsigned char *)(moby_vars + 0x301) = 9;
    *(unsigned char *)(moby_vars + 0x303) = 13;
    *(unsigned char *)(moby_vars + 0x302) = 15;
    *(float *)(moby_vars + 0x240) = 0.5f;
    initialize_leg_rig(moby, movement_data);
}

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002D29E8), where it is exact; names translated to the US level program. */

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
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

void emit_woodcut_bot_death_effects(void *moby) __asm__("FUN_L04_002d1608");

void emit_woodcut_bot_death_effects(void *moby) {
    char joint_world_position[16];
    char zero_vector[16];
    clear_u64_value(zero_vector);
    moby_point_position(moby, 12, joint_world_position);
    spawn_explosion(moby,
                    zero_vector,
                    joint_world_position,
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
                    -1,
                    0);
}

extern float D_L04_0015F580[] MACRO_ADDR;
extern void  attach_manipulator(char *, int, void *) __asm__("FUN_0020cb10");
extern void  FUN_L00_001fff28(void *, int, float);
extern void
collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002D2A98), where it is exact; names translated to the US level program. */

typedef struct {
    int v[6];
} WBJoints;

typedef struct {
    float v[5];
} WBAngles;

typedef struct WBData {
    /* 0x000 */ char          pad000[0x20];
    /* 0x020 */ float         hp;
    /* 0x024 */ short         f24;
    /* 0x026 */ char          pad026[2];
    /* 0x028 */ unsigned char f28;
    /* 0x029 */ unsigned char f29;
    /* 0x02A */ char          pad02A[6];
    /* 0x030 */ float         f30;
    /* 0x034 */ char          pad034[4];
    /* 0x038 */ int           f38;
    /* 0x03C */ char          pad03C[0x1C];
    /* 0x058 */ unsigned char f58;
    /* 0x059 */ char          pad059;
    /* 0x05A */ unsigned char f5A;
    /* 0x05B */ char          pad05B[0x55];
    /* 0x0B0 */ char          fB0[7];
    /* 0x0B7 */ unsigned char fB7;
    /* 0x0B8 */ char          pad0B8[8];
    /* 0x0C0 */ char          fC0[0x10];
    /* 0x0D0 */ float         fD0;
    /* 0x0D4 */ float         fD4;
    /* 0x0D8 */ float         fD8;
    /* 0x0DC */ float         fDC;
    /* 0x0E0 */ int           fE0;
    /* 0x0E4 */ int           fE4;
    /* 0x0E8 */ float         fE8;
    /* 0x0EC */ char          pad0EC[0x11];
    /* 0x0FD */ unsigned char fFD;
    /* 0x0FE */ char          pad0FE[0x12];
    /* 0x110 */ float         f110;
    /* 0x114 */ float         f114;
    /* 0x118 */ char          pad118[8];
    /* 0x120 */ unsigned char f120;
    /* 0x121 */ unsigned char f121;
    /* 0x122 */ char          pad122[0xE];
    /* 0x130 */ char          f130[0x30];
    /* 0x160 */ char          f160[0xB7];
    /* 0x217 */ unsigned char f217;
    /* 0x218 */ char          pad218[0x198];
    /* 0x3B0 */ float         f3B0[4];
    /* 0x3C0 */ unsigned char f3C0;
    /* 0x3C1 */ unsigned char f3C1;
    /* 0x3C2 */ short         t3C2;
    /* 0x3C4 */ short         t3C4;
    /* 0x3C6 */ short         t3C6;
    /* 0x3C8 */ float         f3C8;
    /* 0x3CC */ char          pad3CC[4];
    /* 0x3D0 */ float         f3D0;
    /* 0x3D4 */ float         f3D4;
    /* 0x3D8 */ float         f3D8;
    /* 0x3DC */ int           f3DC;
    /* 0x3E0 */ float         f3E0;
    /* 0x3E4 */ float         f3E4;
    /* 0x3E8 */ float         f3E8;
} WBData;

typedef struct WBMoby {
    /* 0x00 */ char          pad00[0x10];
    /* 0x10 */ float         pos[4];
    /* 0x20 */ unsigned char state;
    /* 0x21 */ char          pad21[0x10];
    /* 0x31 */ unsigned char f31;
    /* 0x32 */ char          pad32[0xE];
    /* 0x40 */ float         rot[4];
    /* 0x50 */ char          pad50[2];
    /* 0x52 */ unsigned char anim;
    /* 0x53 */ unsigned char anim2;
    /* 0x54 */ char          pad54[4];
    /* 0x58 */ float         f58;
    /* 0x5C */ char          pad5C[0x14];
    /* 0x70 */ unsigned char f70;
    /* 0x71 */ char          pad71[7];
    /* 0x78 */ WBData       *d;
    /* 0x7C */ char          pad7C[3];
    /* 0x7F */ unsigned char f7F;
    /* 0x80 */ char          pad80[0x24];
    /* 0xA4 */ unsigned char fA4;
    /* 0xA5 */ char          padA5[0x17];
    /* 0xBC */ unsigned char fBC;
} WBMoby;

typedef struct WBQuery {
    /* 0x00 */ float v[4];
    /* 0x10 */ void *m;
    /* 0x14 */ int   f14;
    /* 0x18 */ int   f18;
    /* 0x1C */ float f1C;
    /* 0x20 */ int   f20;
    /* 0x24 */ int   pad24[3];
} WBQuery;

WBAngles D_L04_001DC468 = {
    {0.5235988f, 0.7853982f, 0.2617994f, 1.0471976f, 0.0f}
};

WBJoints D_L04_001DC450 = {
    {0, 2, 1, 3, 4, 5}
};

extern char  D_L04_00166F40[];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float spring_turn(float, float, void *, float, float, float) __asm__("FUN_L00_00258110");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int   leg_walker_update(void *, void *, float, float, float) __asm__("FUN_L04_00295778");
extern int   is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern void *memset(void *, int, unsigned int);
extern void  coll_line_fix(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void  add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void  FUN_L00_00257470(void *, int, int);
extern void  FUN_L00_00258490(void *, void *, void *, int, float, float, float, float);
extern void  FUN_L00_0025a120(void *);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");
extern void FUN_L00_00263ac8(float, void *, int, void *);
extern void reset_walker_gait_helpers(void *) __asm__("FUN_L00_002a1538");
extern void spawn_crate_debris(void *, void *, void *, void *, int, int, float, float) __asm__("FUN_L00_002af8b0");
extern void reset_leg_rig_from_animation(void *, void *) __asm__("FUN_L04_00295de8");
extern void initialize_woodcut_bot_movement_data(void *) __asm__("FUN_L04_002d1418");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void
transform_scaled_vertex_batch(char *arg0, int arg1, void *arg2, char *arg3) __asm__("FUN_0020cd48");
void emit_woodcut_bot_death_effects(void *m) __asm__("FUN_L04_002d1608");

void woodcut_bot_update(WBMoby *m) __asm__("FUN_L04_002d16b8");

/* Moby class 563 */
void woodcut_bot_update(WBMoby *m) {
    enum {
        WOODCUT_BOT_INITIALIZE = 0,
        WOODCUT_BOT_IDLE = 1,
        WOODCUT_BOT_WAIT_FOR_ANIMATION_BEFORE_PURSUIT = 2,
        WOODCUT_BOT_PLAY_ANIMATION_AND_EMIT_DEBRIS = 3,
        WOODCUT_BOT_SELECT_ACTION_WHILE_EXCITED = 4,
        WOODCUT_BOT_MOVE_TOWARD_TARGET = 5,
        WOODCUT_BOT_RETURN_TO_SAVED_POSITION = 6,
        WOODCUT_BOT_STOP_MOVEMENT = 7,
        WOODCUT_BOT_MELEE_ATTACK = 8,
        WOODCUT_BOT_HIT_RECOVERY_AND_REPOSITION = 9,
        WOODCUT_BOT_TURN_BEFORE_MOVEMENT = 10,
        WOODCUT_BOT_DEATH_KNOCKBACK = 11
    };
    enum {
        WOODCUT_BOT_REACTION_NO_ADDITIONAL_RESPONSE = 0,
        WOODCUT_BOT_REACTION_START_DEATH_KNOCKBACK = 1,
        WOODCUT_BOT_REACTION_DEATH_KNOCKBACK = 2,
        WOODCUT_BOT_REACTION_HIT_RECOVERY_AND_REPOSITION = 3,
        WOODCUT_BOT_REACTION_RECOIL_TO_IDLE = 4,
        WOODCUT_BOT_REACTION_RECOIL_TO_IDLE_WITH_COOLDOWNS = 5,
        WOODCUT_BOT_REACTION_HIT_RECOVERY_WITH_MELEE_WINDOW_GUARD = 6
    };
    int            excited = 0;
    WBData        *d = m->d;
    unsigned char *manipulator;
    short         *tm;
    float         *pos;
    unsigned char *p;
    float          selected_target_position[4];
    float          unused[16];
    float          position_snapshot[4];
    float          duplicate_position_snapshot[4];
    float          adiff;
    float          target_distance;
    float          hd;

    if (d->f38 != 0 || d->t3C6 != 0)
        excited = 1;
    select_moby_combat_target(m, selected_target_position, excited ? d->f3E8 * 2.0f : d->f3E8);
    p = &d->f120;
    FUN_L00_00263ac8(2.5f, m, 6, p);
    manipulator = p;
    pos = m->pos;
    if (m->f31 != 0 && fast_vec_dist(pos, D_L04_00166F40) < 27.0f) {
        FUN_L00_0025a120(m);
        m->f7F = 0x15;
    }
    adiff =
        fast_difference_between_rotations(m->rot[2],
                                          fast_arc_tan(selected_target_position[0] - m->pos[0],
                                                       selected_target_position[1] - m->pos[1]));
    target_distance = vector_distance_xy(pos, selected_target_position);
    hd = compute_interpolated_record_value(m);
    qcopy(position_snapshot, pos);
    qcopy(duplicate_position_snapshot, pos);
    fast_dec_timer_s16(&d->t3C4);

    if (m->state != 0xB && m->state != 0) {
        int   r;
        int   htype;
        float damage;
        float angle;
        float one;
        float v[4];

        damage = 0.0f;
        p = (unsigned char *)find_moby_hit(m, 0xA10000, 0);
        r = hit_reaction(m, p, &d->hp, 0, &htype, &damage, 0, 4);
        d->fE0 = 0x15E;
        d->fE8 = 0.35f;
        if (p != 0 && damage != 0.0f && m->state != 0 && d->t3C4 == 0) {
            unsigned char *hit_source_moby;
            float          knockback_gravity_step;
            float          b;
            float          default_knockback_horizontal_step;

            hit_source_moby = *(unsigned char **)(p + 0x20);
            one = 1.0f;
            if (hit_source_moby != 0) {
                struct Hero *h = &hero;
                if (hit_source_moby != ((unsigned char *)h->items[0].moby)) {
                    angle = fast_arc_tan(m->pos[0] - MOBY(hit_source_moby)->pos.x,
                                         m->pos[1] - MOBY(hit_source_moby)->pos.y);
                } else {
                    angle = fast_arc_tan(m->pos[0] - h->motion.pos.f[0],
                                         m->pos[1] - h->motion.pos.f[1]);
                }
            } else {
                struct Hero *h = &hero;
                angle =
                    fast_arc_tan(m->pos[0] - h->motion.pos.f[0], m->pos[1] - h->motion.pos.f[1]);
            }
            knockback_gravity_step = frame_time_sq * 30.0f;
            b = frame_time_sq * 10.0f;
            default_knockback_horizontal_step = frame_time * 5.5f;
            d->fE4 = 9;
            d->fD0 = knockback_gravity_step;
            d->fFD = 0;
            d->fD4 = b;
            d->fD8 = default_knockback_horizontal_step;
            d->hp -= damage;
            if (d->hp <= 0.0f)
                r = 1;
            switch (r) {
            case WOODCUT_BOT_REACTION_NO_ADDITIONAL_RESPONSE:
                break;
            case WOODCUT_BOT_REACTION_START_DEATH_KNOCKBACK:
            case WOODCUT_BOT_REACTION_DEATH_KNOCKBACK:
                d->fD4 = frame_time_sq * 17.0f;
                d->fD8 = frame_time * 7.0f;
                d->fDC = frame_time * 13.0f;
                *(OvlQuad *)v = *(OvlQuad *)(p + 0x10);
                trooper_knockback_arc(v, &angle, &d->fD8, &d->fDC);
                trooper_start_knockback(angle, m, d->fC0, 8, 1, 0);
                d->f110 = 14.0f;
                d->f114 = 24.0f;
                m->state = 0xB;
                FUN_L00_00257470(m, 0, -1);
                d->fB7 = 0x78;
                trooper_anim_update(m, d->fB0);
                d->f3E4 = (2.0f / scale_time(15.0f)) * 2.0f;
                break;
            case WOODCUT_BOT_REACTION_HIT_RECOVERY_AND_REPOSITION:
            case WOODCUT_BOT_REACTION_HIT_RECOVERY_WITH_MELEE_WINDOW_GUARD:
                d->f3C8 = -0.5235988f;
                if (m->anim != 4 || hd < 16.0f || hd > 27.0f) {
                    short hit_source_moby_class;

                    m->fBC |= 5;
                    blend_moby_animation(m, 5, 0, scale_game_frames(3));
                    reset_walker_gait_helpers(d->f160);
                    m->state = 9;
                    hit_source_moby_class = *(short *)(*(char **)(p + 0x20) + 0xA6);
                    if (hit_source_moby_class == 0x131 || hit_source_moby_class == 0xB0) {
                        d->f3E4 = (2.0f / scale_time(15.0f)) * 0.5f;
                    } else {
                        d->f3E4 = 2.0f / scale_time(15.0f);
                    }
                    *(OvlQuad *)v = *(OvlQuad *)(p + 0x10);
                    trooper_knockback_arc(v, &angle, &d->f3E4, &one);
                    d->fB7 = 0x78;
                    trooper_anim_update(m, d->fB0);
                    d->t3C4 = scale_game_frames(0xF);
                    d->t3C6 = scale_game_frames(0xF0);
                }
                break;
            case WOODCUT_BOT_REACTION_RECOIL_TO_IDLE:
            case WOODCUT_BOT_REACTION_RECOIL_TO_IDLE_WITH_COOLDOWNS:
                blend_moby_animation(m, 6, 0, scale_game_frames(10));
                reset_walker_gait_helpers(d->f160);
                m->state = 1;
                d->f3E4 = 2.0f / scale_time(15.0f);
                *(OvlQuad *)v = *(OvlQuad *)(p + 0x10);
                trooper_knockback_arc(v, &angle, &d->f3E4, &one);
                m->rot[2] = fast_add_rotations(3.1415927f, angle);
                d->fB7 = 0x78;
                trooper_anim_update(m, d->fB0);
                d->t3C4 = scale_game_frames(0x1E);
                d->t3C2 = scale_game_frames(0xB4);
                d->t3C6 = scale_game_frames(0xF0);
                break;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                break;
            }
        }
    }
    m->fA4 = 0xFF;
    if (d->f38 != 0)
        d->t3C6 = scale_game_frames(0xF0);
    tm = &d->t3C2;
    d->f38 = 0;
    fast_dec_timer_s16(&d->t3C6);

    if (m->state == 8) {
        if (m->anim == 4 && hd > 16.0f && hd < 27.0f) {
            WBJoints joints = D_L04_001DC450;
            float    pts[6][4];
            WBQuery  melee_hit_query;
            float    mid[4];
            int      i;

            melee_hit_query.f14 = 0x10001;
            melee_hit_query.f1C = 1.0f;
            melee_hit_query.m = m;
            fast_vec_sub(&melee_hit_query, &hero.motion.pos, pos);
            fast_vec_normalize(&melee_hit_query, &melee_hit_query, frame_time * 5.0f);
            melee_hit_query.f20 = 1;
            transform_scaled_vertex_batch(m, 6, &joints, pts);
            for (i = 0; i < 5; i++)
                coll_line_fix(pts[i], pts[i + 1], 9, m, &melee_hit_query);
            fast_vec_add(mid, pts[0], pts[4]);
            fast_vec_scale(mid, mid, 0.5f);
            collect_mobies_in_sphere(
                mid, fast_vec_dist(pts[0], pts[4]) * 0.5f, 0x10, m, &melee_hit_query);
            moby_point_position(m, 0x11, mid);
            collect_mobies_in_sphere(mid, 0.5f, 0x10, m, &melee_hit_query);
        }
    }
    fast_dec_timer_s16(tm);

    switch (m->state) {
    case WOODCUT_BOT_INITIALIZE:
        d->hp = 2.0f;
        d->f24 = 2;
        d->f30 = 1.0f;
        d->f28 = 1;
        d->f29 = 0;
        m->fBC = 0;
        d->f3E0 = 0.0f;
        d->t3C6 = 0;
        d->f58 = 8;
        d->f5A = 8;
        qcopy(d->f3B0, pos);
        if (d->f121 == 0)
            attach_manipulator(m, 6, manipulator);
        initialize_woodcut_bot_movement_data(m);
        if (d->f3C0 != 0) {
            m->state = 3;
            blend_moby_animation(m, 0xB, 0, scale_game_frames(10));
        } else {
            m->state = 1;
        }
        break;
    case WOODCUT_BOT_IDLE:
        if (m->f70 & 2) {
            if (excited ? (d->f3E8 * 2.0f < target_distance) : (d->f3E8 < target_distance)) {
                int anims[6];

                memset(anims, 0, 0x18);
                anims[5] = 1;
                blend_moby_animation(m, anims[random_integer_below(6)], 0, scale_game_frames(0x14));
            } else {
                blend_moby_animation(m, 0, 0, scale_game_frames(0x14));
            }
        }
        if (d->t3C2 == 0) {
            if (target_distance < 3.0f && adiff < 1.5707964f) {
                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                m->state = 8;
            } else if (excited ? (target_distance < d->f3E8 * 2.0f) : (target_distance < d->f3E8)) {
                if (m->fBC & 1) {
                    if (excited || d->f3DC == -1 ||
                        point_in_path_xy(selected_target_position,
                                         ((int *)l04_paths[d->f3DC]) + 4,
                                         *((int *)l04_paths[d->f3DC])) != 0) {
                        m->state = 5;
                        m->fBC |= 8;
                    }
                } else {
                    d->f3D0 = 0.0f;
                    blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                    reset_walker_gait_helpers(d->f160);
                    m->state = 0xA;
                    m->fBC |= 1;
                }
            }
            if (!excited) {
                if (vector_distance_xy(pos, d->f3B0) > 2.0f) {
                    if (d->f3E8 < target_distance ||
                        (d->f3DC != -1 && point_in_path_xy(selected_target_position,
                                                           ((int *)l04_paths[d->f3DC]) + 4,
                                                           *((int *)l04_paths[d->f3DC])) == 0)) {
                        m->state = 6;
                        m->fBC |= 8;
                    }
                }
            }
        }
        break;
    case WOODCUT_BOT_WAIT_FOR_ANIMATION_BEFORE_PURSUIT:
        if (m->anim == 2 && (m->f70 & 2)) {
            if (excited || d->f3DC == -1 ||
                point_in_path_xy(selected_target_position,
                                 ((int *)l04_paths[d->f3DC]) + 4,
                                 *((int *)l04_paths[d->f3DC])) != 0) {
                m->state = 5;
                m->fBC |= 8;
            } else {
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
                m->state = 1;
            }
        }
        break;
    case WOODCUT_BOT_PLAY_ANIMATION_AND_EMIT_DEBRIS: {
        if (m->anim != 0 && (m->f70 & 2)) {
            blend_moby_animation(m, 0xC, 0, scale_game_frames(10));
            m->f58 = random_float_between(0.8f, 1.2f);
        }
        if (is_value_within_interpolated_window(m, 36.0f) != 0) {
            int i;

            for (i = 0; i < 20; i++) {
                float p[4];
                float debris_horizontal_step[4];
                float upward_biased_random_direction[4];
                float rot[4];
                float mtx[16];

                *(OvlQuad *)rot = 0;
                rot[0] = random_float_between(-1.0f, 1.0f);
                rot[1] = random_float_between(-1.0f, 1.0f);
                rot[2] = random_float_between(-1.0f, 1.0f);
                *(OvlQuad *)upward_biased_random_direction = *(OvlQuad *)rot;
                moby_point_position(m, 0, p);
                debris_horizontal_step[0] =
                    fast_cos(m->rot[2]) * -(random_float_between(5.0f, 10.0f) * frame_time);
                debris_horizontal_step[1] =
                    fast_sin(m->rot[2]) * -(random_float_between(5.0f, 10.0f) * frame_time);
                debris_horizontal_step[2] = 0.0f;
                fast_vec_normalize(upward_biased_random_direction,
                                   upward_biased_random_direction,
                                   frame_time * 3.0f);
                upward_biased_random_direction[2] += frame_time * 3.0f;
                add_vectors(mtx, debris_horizontal_step, upward_biased_random_direction);
                rot[0] = random_angle_radians();
                rot[1] = random_angle_radians();
                rot[2] = random_angle_radians();
                spawn_crate_debris(m, debris_horizontal_step, p, rot, 0x5EC, 0, 0.35f, 0.7f);
            }
        }
        if (d->t3C2 == 0) {
            if (target_distance < 3.0f && adiff < 1.5707964f) {
                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                m->state = 8;
            } else if (excited ? (target_distance < d->f3E8 * 2.0f) : (target_distance < d->f3E8)) {
                if (m->fBC & 1) {
                    if (excited || d->f3DC == -1 ||
                        point_in_path_xy(selected_target_position,
                                         ((int *)l04_paths[d->f3DC]) + 4,
                                         *((int *)l04_paths[d->f3DC])) != 0) {
                        m->state = 5;
                        m->fBC |= 8;
                    }
                } else {
                    d->f3D0 = 0.0f;
                    blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                    reset_walker_gait_helpers(d->f160);
                    m->state = 0xA;
                    m->fBC |= 1;
                }
            }
        }
        break;
    }
    case WOODCUT_BOT_SELECT_ACTION_WHILE_EXCITED:
        if (excited) {
            if (target_distance < 3.0f && adiff < 1.5707964f) {
                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                reset_walker_gait_helpers(d->f160);
                m->state = 8;
            } else if (d->f3DC == -1 || point_in_path_xy(selected_target_position,
                                                         ((int *)l04_paths[d->f3DC]) + 4,
                                                         *((int *)l04_paths[d->f3DC])) != 0) {
                m->state = 5;
                m->fBC |= 8;
            } else if (adiff > 1.5707964f) {
                d->f3D0 = 0.0f;
                blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                reset_walker_gait_helpers(d->f160);
                m->state = 0xA;
            }
        } else {
            m->state = 1;
        }
        break;
    case WOODCUT_BOT_MOVE_TOWARD_TARGET: {
        if (m->fBC & 8) {
            m->fBC &= ~8;
            reset_leg_rig_from_animation(m, d->f160);
        }
        leg_walker_update(m,
                          d->f160,
                          frame_time * 4.0f,
                          fast_arc_tan(selected_target_position[0] - m->pos[0],
                                       selected_target_position[1] - m->pos[1]),
                          frame_time_sq * 20.0f);
        if (target_distance < 3.0f && adiff < 1.5707964f) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            reset_walker_gait_helpers(d->f160);
            m->state = 8;
        } else if (d->f3DC != -1) {
            float v[4];

            v[0] = fast_cos(m->rot[2]) * 2.0f;
            v[1] = fast_sin(m->rot[2]) * 2.0f;
            v[2] = 0.0f;
            p = (unsigned char *)v;
            fast_vec_add(p, p, pos);
            if (point_in_path_xy(
                    p, ((int *)l04_paths[d->f3DC]) + 4, *((int *)l04_paths[d->f3DC])) == 0) {
                if (excited && point_in_path_xy(selected_target_position,
                                                ((int *)l04_paths[d->f3DC]) + 4,
                                                *((int *)l04_paths[d->f3DC])) == 0) {
                    m->state = 7;
                } else if (point_in_path_xy(selected_target_position,
                                            ((int *)l04_paths[d->f3DC]) + 4,
                                            *((int *)l04_paths[d->f3DC])) != 0) {
                    d->f3D0 = 0.0f;
                    blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                    reset_walker_gait_helpers(d->f160);
                    m->state = 0xA;
                }
            }
            if (!excited) {
                if (point_in_path_xy(selected_target_position,
                                     ((int *)l04_paths[d->f3DC]) + 4,
                                     *((int *)l04_paths[d->f3DC])) == 0) {
                    m->state = 6;
                }
            }
        }
        break;
    }
    case WOODCUT_BOT_RETURN_TO_SAVED_POSITION:
        if (m->fBC & 8) {
            m->fBC &= ~8;
            reset_leg_rig_from_animation(m, d->f160);
        }
        if (target_distance < 3.0f && adiff < 1.5707964f) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            reset_walker_gait_helpers(d->f160);
            m->state = 8;
        } else {
            if (m->fBC & 8) {
                m->fBC &= ~8;
                reset_leg_rig_from_animation(m, d->f160);
            }
            if (vector_distance_xy(pos, d->f3B0) < 2.0f) {
                m->state = 7;
            } else {
                leg_walker_update(m,
                                  d->f160,
                                  frame_time * 4.0f,
                                  fast_arc_tan(d->f3B0[0] - m->pos[0], d->f3B0[1] - m->pos[1]),
                                  frame_time_sq * 20.0f);
                if (d->f3DC != -1) {
                    float v[4];

                    v[0] = fast_cos(m->rot[2]) * 2.0f;
                    v[1] = fast_sin(m->rot[2]) * 2.0f;
                    v[2] = 0.0f;
                    fast_vec_add(v, v, pos);
                    if (point_in_path_xy(v,
                                         ((int *)l04_paths[d->f3DC]) + 4,
                                         *((int *)l04_paths[d->f3DC])) == 0) {
                        d->f3D0 = 0.0f;
                        blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                        reset_walker_gait_helpers(d->f160);
                        m->state = 0xA;
                        m->fBC |= 0x10;
                    } else if (point_in_path_xy(selected_target_position,
                                                ((int *)l04_paths[d->f3DC]) + 4,
                                                *((int *)l04_paths[d->f3DC])) != 0) {
                        m->state = 7;
                    }
                }
            }
        }
        break;
    case WOODCUT_BOT_STOP_MOVEMENT:
        if (target_distance < 3.0f && adiff < 1.5707964f) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            reset_walker_gait_helpers(d->f160);
            m->state = 8;
        } else {
            leg_walker_update(m, d->f160, frame_time * 0.0f, m->rot[2], frame_time_sq * 20.0f);
            if (d->f217 == 0) {
                if (excited) {
                    m->state = 4;
                } else {
                    m->state = 1;
                }
            }
        }
        break;
    case WOODCUT_BOT_MELEE_ATTACK:
        if (m->anim == 4 && (m->f70 & 2)) {
            d->t3C2 = 0;
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
            m->state = 1;
        }
        break;
    case WOODCUT_BOT_HIT_RECOVERY_AND_REPOSITION:
        if (m->anim2 != 5 || (m->anim == 5 && (m->f70 & 2))) {
            if (m->fBC & 4) {
                m->fBC &= ~4;
                if (d->f3DC == -1) {
                    blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    m->state = 1;
                    d->t3C2 = 0;
                } else {
                    WBAngles tab = D_L04_001DC468;
                    float    v[4];
                    float    sign = 1.0f;
                    int      found = 0;
                    int      i;

                    for (i = 0; i < 9; i++) {
                        struct Hero *h = &hero;
                        float        candidate_reposition_bearing;

                        if ((i & 1) == 0) {
                            sign = 1.0f;
                            if (random_integer_below(100) & 1)
                                sign = -1.0f;
                        } else {
                            sign = -sign;
                        }
                        candidate_reposition_bearing =
                            fast_add_rotations(sign * tab.v[i >> 1],
                                               fast_arc_tan(m->pos[0] - h->motion.pos.f[0],
                                                            m->pos[1] - h->motion.pos.f[1]));
                        build_spherical_offset(v, 6.0f, candidate_reposition_bearing, m->rot[1]);
                        fast_vec_add(v, v, pos);
                        if (point_in_path_xy(v,
                                             ((int *)l04_paths[d->f3DC]) + 4,
                                             *((int *)l04_paths[d->f3DC])) != 0) {
                            found = 1;
                            blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                            d->f3D4 = candidate_reposition_bearing;
                            d->f3D8 = 0.0f;
                            break;
                        }
                    }
                    if (!found) {
                        if (m->anim == 5) {
                            if (target_distance < 3.0f && adiff < 1.5707964f) {
                                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                                m->state = 8;
                            } else {
                                blend_moby_animation(m, 0, 0, scale_game_frames(10));
                                m->state = 1;
                            }
                        } else {
                            blend_moby_animation(m, 0, 0, scale_game_frames(10));
                            m->state = 1;
                            d->t3C2 = 0;
                        }
                    }
                }
            } else {
                float v[4];

                if (hd < 28.0f && m->anim == 0x10) {
                    float reposition_step_limit = frame_scale * 0.11111111f;

                    if (d->f3D8 < reposition_step_limit) {
                        d->f3D8 = d->f3D8 + frame_time_sq * 20.0f;
                    } else {
                        d->f3D8 = reposition_step_limit;
                    }
                    build_spherical_offset(v, d->f3D8, d->f3D4, m->rot[1]);
                    fast_vec_add(pos, pos, v);
                    m->rot[2] = spring_turn(m->rot[2],
                                            fast_arc_tan(selected_target_position[0] - m->pos[0],
                                                         selected_target_position[1] - m->pos[1]),
                                            &d->f3D0,
                                            frame_time_sq * 3.1415927f,
                                            frame_time_sq * 6.2831855f,
                                            frame_time * 4.1887903f);
                } else if (d->f3D8 > 0.0f) {
                    d->f3D8 -= frame_time_sq * 200.0f;
                    build_spherical_offset(v, d->f3D8, d->f3D4, m->rot[1]);
                    fast_vec_add(pos, pos, v);
                    m->rot[2] = spring_turn(m->rot[2],
                                            fast_arc_tan(selected_target_position[0] - m->pos[0],
                                                         selected_target_position[1] - m->pos[1]),
                                            &d->f3D0,
                                            frame_time_sq * 3.1415927f,
                                            frame_time_sq * 6.2831855f,
                                            frame_time * 4.1887903f);
                } else {
                    m->state = 1;
                    d->t3C2 = 0;
                }
            }
        }
        break;
    case WOODCUT_BOT_TURN_BEFORE_MOVEMENT:
        if (m->anim == 0x10 && (m->f70 & 2)) {
            if ((m->fBC & 0x10) || point_in_path_xy(selected_target_position,
                                                    ((int *)l04_paths[d->f3DC]) + 4,
                                                    *((int *)l04_paths[d->f3DC])) == 0) {
                m->state = 6;
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
            } else {
                m->state = 5;
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
            }
            m->fBC = (m->fBC | 8) & ~0x10;
        } else if (m->fBC & 0x10) {
            m->rot[2] = spring_turn(m->rot[2],
                                    fast_arc_tan(d->f3B0[0] - m->pos[0], d->f3B0[1] - m->pos[1]),
                                    &d->f3D0,
                                    frame_time_sq * 3.1415927f,
                                    frame_time_sq * 6.2831855f,
                                    frame_time * 4.1887903f);
        } else {
            m->rot[2] = spring_turn(m->rot[2],
                                    fast_arc_tan(selected_target_position[0] - m->pos[0],
                                                 selected_target_position[1] - m->pos[1]),
                                    &d->f3D0,
                                    0.03f,
                                    0.3f,
                                    frame_time * 6.4577184f);
        }
        break;
    case WOODCUT_BOT_DEATH_KNOCKBACK:
        if ((step_moby_motion(m, d->fC0) & 0x40) || m->pos[2] < 1.0f) {
            emit_woodcut_bot_death_effects(m);
            FUN_L00_00263fd8(
                m, 0x6CB, pos, m->rot, 0, 0, 0.0f, D_L04_0015F580, D_L04_0015F580, D_L04_0015F580);
            FUN_L00_00263fd8(
                m, 0x6CC, pos, m->rot, 0, 0, 0.0f, D_L04_0015F580, D_L04_0015F580, D_L04_0015F580);
            FUN_L00_00263fd8(
                m, 0x6CD, pos, m->rot, 0, 0, 0.0f, D_L04_0015F580, D_L04_0015F580, D_L04_0015F580);
            mark_moby_for_removal(m);
            return;
        }
        break;
    }

    {
        float recoil_scratch[16];

        trooper_anim_finish(m, d->fB0);
        if (d->f3E4 > 0.0f) {
            struct Hero *h = &hero;

            recoil_scratch[0] = fast_cos(fast_arc_tan(m->pos[0] - h->motion.pos.f[0],
                                                      m->pos[1] - h->motion.pos.f[1])) *
                                d->f3E4;
            recoil_scratch[1] = fast_sin(fast_arc_tan(m->pos[0] - h->motion.pos.f[0],
                                                      m->pos[1] - h->motion.pos.f[1])) *
                                d->f3E4;
            recoil_scratch[2] = 0.0f;
            fast_vec_add(recoil_scratch, recoil_scratch, pos);
            d->f3E4 += -2.0f / (scale_time(15.0f) * scale_time(15.0f));
            FUN_L00_00258490(m, pos, recoil_scratch, 1, 0.25f, 1.0f, 0.75f, 0.87266463f);
        }
        d->f3C8 *= 0.96f;
        FUN_L00_001fff28(recoil_scratch, 1, d->f3C8);
        FUN_L00_001fff28(d->f130, 2, 0.0f);
        multiply_quaternions(d->f130, d->f130, recoil_scratch);
        if (m->state != 9)
            d->f3C1 = m->state;
        if (m->state != 5 && m->fBC != 6) {
            float g;

            d->f3E0 += frame_time_sq * 9.8f;
            m->pos[2] -= d->f3E0 - 2.0f;
            g = probe_ground_height(pos, 0, 0.5f);
            m->pos[2] -= 2.0f;
            if (m->pos[2] < g) {
                m->pos[2] = g;
                d->f3E0 = 0.0f;
            }
            if (m->pos[2] < 2.0f) {
                FUN_L00_00257470(m, 0, -1);
                mark_moby_for_removal(m);
            }
        }
    }
}
