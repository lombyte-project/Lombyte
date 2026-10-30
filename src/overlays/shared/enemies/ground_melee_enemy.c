/* Ground melee enemy class (0xC1): reacts to damage, guards its home inside a leash and chases, attacks or returns home. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/mobys/resolve_moby_hit_response.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"

/* Ground enemy update: damage reaction, leash and wake checks, then its chase / attack / return state machine. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c: func_L09_002E39E0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;
typedef struct Hero Hero;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2E39E0;

typedef struct {
    char           pad0[0x20];
    float          health;
    short          f24;
    short          cooldown;
    char           pad28[1];
    unsigned char  f29;
    char           pad2A[0xE];
    int            hurt;
    char           pad3C[0x1C];
    unsigned char  f58;
    char           pad59[1];
    unsigned char  f5A;
    char           pad5B[0x6D];
    short          fC8;
    char           padCA[6];
    unsigned char *settings;
    char           padD4[0x3C];
    char           effects[7];
    unsigned char  f117;
    char           pad118[8];
    char           motion[0x10];
    float          f130;
    float          f134;
    float          up;
    float          back;
    char           pad140[4];
    int            f144;
    char           pad148[0x15];
    unsigned char  f15D;
    char           pad15E[0x12];
    float          f170;
    float          f174;
    char           pad178[8];
    int            steer;
    char           pad184[4];
    float          f188;
    float          f18C;
    float          f190;
    char           pad194[4];
    float          fall;
    char           pad19C[8];
    float          f1A4;
    char           pad1A8[0x28];
    V_2E39E0       home;
    char           pad1E0[0x10];
    char           turn[4];
    float          side;
    float          side_deg;
    float          range;
    char           pad200[4];
    float          speed;
    int            f208;
    char           pad20C[4];
    int            side_timer;
    short          alert;
    short          leash;
    Moby          *anchor;
    int            path;
    char           pad220[4];
    int            home_timer;
    int            hidden;
    char           pad22C[4];
    char           sound[4];
} Data_2E39E0;

typedef struct {
    char           pad0[0x20];
    Moby          *moby;
    char           pad24[6];
    unsigned short kind;
    float          damage;
} Hit_2E39E0;

typedef struct {
    float x, y, z;
    char  padC[0x34];
    Moby *who;
    char  pad44[0xC];
} __attribute__((aligned(16))) Target_2E39E0;

extern int  D_L09_001618D8; /* no foreign declaration */
extern int  FUN_L00_002dc9f0();
extern s32  rand(void);
extern void FUN_L00_00257470_c2(void *, int, int) __asm__("FUN_L00_00257470");
int         select_moby_combat_target(char *a, char *o, float best) __asm__("FUN_L00_0025fcb8");
int         select_list_point_toward_target(char *a0,
                                            void *list,
                                            char *pt,
                                            char *out) __asm__("FUN_L01_0028b5e0");
s32         FUN_L00_00258b50(void *m, void *c, void *v, Vec4 *out, f32 f);
void        FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
void        FUN_L00_0025a120_c(char *o) __asm__("FUN_L00_0025a120");
void        FUN_L00_00263ac8_c(float x, int a, int b, unsigned char *p) __asm__("FUN_L00_00263ac8");
void        FUN_L01_0026d930_c(char *p) __asm__("FUN_L01_0026d930");
void        FUN_L01_0026e090_c2(int i, int v) __asm__("FUN_L01_0026e090");
extern char D_0013E633_2E39E0[] __asm__("D_0013F3D0");
extern unsigned char D_001413F4[];
extern float         D_L09_00166FC0_2E39E0[] __asm__("D_L09_00166F40");
extern Moby         *D_L09_00160064_2E39E0 __asm__("D_L09_0015FFE4");
float                impulse __asm__("D_L09_001618C8") = 7.0f;
float                lift_2E39E0 __asm__("D_L09_001618CC") = 3.0f;
extern unsigned char settings_2E39E0 __asm__("D_L09_001618D8") __attribute__((sda));
extern void          FUN_L01_0026e090(int, int);
extern void          knock_2E39E0(Moby *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void          explode_2E39E0(Moby *, int, int) __asm__("FUN_L00_00257470");
extern void          effects_start_2E39E0(Moby *, void *) __asm__("FUN_L00_0025d458");
extern void          sound_2E39E0(float, Moby *, int, void *) __asm__("FUN_L00_00263ac8");
extern void          wake_2E39E0(Moby *) __asm__("FUN_L00_0025a120");
extern int           FUN_L04_00252e18_c(Moby *, void *, float) __asm__("FUN_L04_00252e18");
extern void          steer_init_2E39E0(int *) __asm__("FUN_L01_0026d930");
extern int           rand_2E39E0(void) __asm__("FUN_001160d8");
extern void approach_w(Moby *, float, void *, float, float, float) __asm__("FUN_L00_00258278");
extern void FUN_L01_0028b5e0_c(Moby *, char *, void *, void *) __asm__("FUN_L01_0028b5e0");
extern int  move_w(Moby *, int *, void *, void *, float) __asm__("FUN_L00_00258b50");
extern int  recover_2E39E0(Moby *, void *) __asm__("FUN_L00_002dc9f0");
extern void effects_update_2E39E0(Moby *, void *) __asm__("FUN_L00_0025d538");

/* Suck cannon methods of the ground melee enemy (and the peckbot, robosquawker and chomper). */
extern int cancel_suck_cannon_suction(void *) __asm__("FUN_L00_002dbe20");

typedef struct {
    char pad[0x78];
    s32  f;
} T6;

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

/* Moby state 7 while FUN_L00_002db8f8 reports a hit, back to 1 when it stops. */
int moby_hold_state_if_grabbed(struct Moby *a) __asm__("FUN_L01_002f1c78");

int moby_hold_state_if_grabbed(struct Moby *suction_moby) {
    int suction_result = process_moby_suck_cannon_suction(suction_moby);
    if (suction_result == 0) {
        if (suction_moby->state == 0x7)
            suction_moby->state = 1;
    } else {
        suction_moby->state = 0x7;
    }
    return suction_result;
}

/* Runs FUN_L00_002dbe20 on the moby and sets its state to 7. */
int run_moby_helper_and_set_state_7(struct Moby *moby) __asm__("FUN_L01_002f1dc8");

int run_moby_helper_and_set_state_7(struct Moby *moby) {
    int helper_result = cancel_suck_cannon_suction(moby);
    moby->state = 0x7;
    return helper_result;
}

/* Returns the moby data pointer plus 0x60, or 0 when either is null. */
s32 get_moby_data_plus_0x60(T6 *moby) __asm__("FUN_L01_002f1df8");

s32 get_moby_data_plus_0x60(T6 *moby) {
    s32 moby_data_address;
    if (moby != 0) {
        moby_data_address = moby->f;
        if (moby_data_address != 0) {
            return moby_data_address + 0x60;
        }
        return 0;
    }
    return 0;
}

void update_ground_melee_enemy(Moby *moby) __asm__("FUN_L09_002e27d8");

void update_ground_melee_enemy(Moby *moby) {
    enum {
        GROUND_ENEMY_STATE_INITIALIZE = 0,
        GROUND_ENEMY_STATE_IDLE = 1,
        GROUND_ENEMY_STATE_ALERT_TURN = 2,
        GROUND_ENEMY_STATE_CHASE = 3,
        GROUND_ENEMY_STATE_MELEE_ATTACK = 4,
        GROUND_ENEMY_STATE_RETURN_HOME = 5,
        GROUND_ENEMY_STATE_HURT_KNOCKBACK = 6,
        GROUND_ENEMY_STATE_RECOVER_TO_IDLE = 7,
        GROUND_ENEMY_STATE_HURT_RECOVERY = 9,
        GROUND_ENEMY_STATE_BLOCKED_RETURN_HOME = 12,
        GROUND_ENEMY_STATE_LEASHED_FORWARD_WALK = 13,
        GROUND_ENEMY_STATE_FATAL_KNOCKBACK = 99
    };
    V_2E39E0      previous;
    Target_2E39E0 query;
    V_2E39E0      step;
    V_2E39E0      goal;
    V_2E39E0      away;
    int           hit_index;
    float         damage;
    Data_2E39E0  *ground_enemy_vars = (Data_2E39E0 *)moby->pvars;
    int           kind = 2;
    int           expired;
    float         ground;

    if (moby->state != 99 && moby->state != 7) {
        Hit_2E39E0 *hit;
        damage = 0.0f;
        hit = find_moby_hit(moby, 0x330000, 0);
        resolve_moby_hit_response(moby, hit, &ground_enemy_vars->health, 0, &hit_index, &damage, 0, 4);
        fast_dec_timer_s16(&ground_enemy_vars->cooldown);
        if (ground_enemy_vars->cooldown < scale_game_frames(45) && hit != 0) {
            if ((unsigned short)(hit->moby->oclass - 0xB0) < 2) {
                if (ground_enemy_vars->cooldown != 0)
                    hit->damage = 0.0f;
                else
                    hit->damage = 1.0f;
            }
            if (moby->group != 0xFF)
                FUN_L01_0026e090(moby->group, 1);
            if (hit->damage != 0.0f) {
                float health = ground_enemy_vars->health - hit->damage;
                float lift = lift_2E39E0 * frame_time;
                float back = impulse * frame_time;
                ground_enemy_vars->f144 = 9;
                ground_enemy_vars->f130 = 0.008f;
                ground_enemy_vars->f134 = 0.0005f;
                ground_enemy_vars->health = health;
                ground_enemy_vars->up = lift;
                ground_enemy_vars->back = back;
                ground_enemy_vars->f15D = 0;
                if (health <= 0.0f) {
                    float up_die = frame_time * 8.0f;
                    float back_die = frame_time * 10.0f;
                    moby->flags &= 0xEFFF;
                    ground_enemy_vars->up = up_die;
                    ground_enemy_vars->back = back_die;
                    if (hit->kind == 180) {
                        ground_enemy_vars->up = up_die * 1.35f;
                        ground_enemy_vars->back = back_die * 1.35f;
                    }
                    knock_2E39E0(moby,
                                 ground_enemy_vars->motion,
                                 fast_arc_tan(moby->pos.x - query.x, moby->pos.y - query.y),
                                 5,
                                 1,
                                 0);
                    ground_enemy_vars->f170 = 11.0f;
                    ground_enemy_vars->f174 = 18.0f;
                    if (D_001413F4[0] == 2)
                        explode_2E39E0(moby, 0x200, -1);
                    else
                        explode_2E39E0(moby, 0, -1);
                    moby->collision = 0;
                    moby->state = 99;
                    ground_enemy_vars->f117 = 120;
                    effects_start_2E39E0(moby, ground_enemy_vars->effects);
                } else {
                    if (hit->kind == 180) {
                        ground_enemy_vars->up = lift * 1.35f;
                        ground_enemy_vars->back = back * 1.35f;
                    }
                    knock_2E39E0(moby,
                                 ground_enemy_vars->motion,
                                 fast_arc_tan(moby->pos.x - query.x, moby->pos.y - query.y),
                                 6,
                                 1,
                                 0);
                    ground_enemy_vars->f170 = 5.0f;
                    ground_enemy_vars->f174 = 10.0f;
                    moby->state = 6;
                    ground_enemy_vars->f117 = 250;
                    ground_enemy_vars->cooldown = scale_game_frames(60);
                    effects_start_2E39E0(moby, ground_enemy_vars->effects);
                }
            }
        }
        moby->hit_slot = 0xFF;
        sound_2E39E0(2.1f, moby, 0, ground_enemy_vars->sound);
        if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L09_00166FC0_2E39E0) < 38.0f) {
            wake_2E39E0(moby);
            moby->shadow_range = 30;
        }
        if (vector_distance_xy(&moby->pos, D_0013E633_2E39E0) > 40.0f)
            return;
        if (ground_enemy_vars->hidden != 0) {
            if ((*(unsigned char *)&((Hero *)D_0013E633_2E39E0)->unk2020[1]) != 2) {
                moby->update_dist = 0xFF;
                moby->collision = 0;
                moby->flags = (moby->flags | 0x41) & 0xEFFF;
                return;
            }
            moby->flags &= 0xFFBE;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
            moby->update_dist = 0x40;
            moby->flags |= 0x1000;
        }
        if (ground_enemy_vars->hurt != 0)
            ground_enemy_vars->alert = scale_game_frames(240);
        ground_enemy_vars->hurt = 0;
        fast_dec_timer(&ground_enemy_vars->f208);
        if (fast_dec_timer_s16(&ground_enemy_vars->alert))
            ground_enemy_vars->range = 12.0f;
        else
            ground_enemy_vars->range = 20.0f;
        kind = FUN_L04_00252e18_c(moby, &query, ground_enemy_vars->range);
    }
    qcopy(&previous, &moby->pos);
    expired = fast_dec_timer(&ground_enemy_vars->home_timer) != 0;
    ground = -1.0f;
    switch (moby->state) {
    case GROUND_ENEMY_STATE_INITIALIZE:
        qcopy(&ground_enemy_vars->home, &moby->pos);
        moby->state = 1;
        ground_enemy_vars->f29 = 0;
        ground_enemy_vars->health = 1.0f;
        ground_enemy_vars->f24 = 1;
        ground_enemy_vars->f5A = 2;
        ground_enemy_vars->f58 = 8;
        ground_enemy_vars->alert = 0;
        ground_enemy_vars->f208 = 0;
        steer_init_2E39E0(&ground_enemy_vars->steer);
        ground_enemy_vars->steer = 0x38D;
        ground_enemy_vars->f188 = ground_enemy_vars->f18C = 2.0f;
        ground_enemy_vars->f1A4 = ground_enemy_vars->speed * frame_time;
        if (rand_2E39E0() & 1)
            moby->flags |= 0x8000;
        ground_enemy_vars->settings = &settings_2E39E0;
        if (moby->prev_seq != 2)
            blend_moby_animation(moby, 2, 0, scale_game_frames(random_int_between(10, 13)));
        ground = moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f);
        break;
    case GROUND_ENEMY_STATE_IDLE:
        fast_vec_sub(&step, &query, &moby->pos);
        if ((kind < 2 || fast_vec_length(&step) < ground_enemy_vars->range + 4.0f) &&
            absolute_float(moby->pos.z - query.z) < 8.0f && !random_integer_below(19)) {
            moby->state = 2;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (moby->command == 1 && !random_integer_below(19)) {
            if (random_integer_below(0x100) & 1)
                ground_enemy_vars->side_deg = -ground_enemy_vars->side_deg;
            ground_enemy_vars->side = ground_enemy_vars->side_deg * 0.017453292f;
            ground_enemy_vars->side_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
            moby->command = 0;
        }
        break;
    case GROUND_ENEMY_STATE_ALERT_TURN:
    case 8:
    case 10:
    case 11:
        approach_w(moby,
                   fast_arc_tan(query.x - moby->pos.x, query.y - moby->pos.y),
                   ground_enemy_vars->turn,
                   0.02f,
                   0.3f,
                   0.1f);
        fast_vec_sub(&step, &query, &moby->pos);
        if ((kind < 2 || fast_vec_length(&step) < ground_enemy_vars->range) &&
            absolute_float(moby->pos.z - query.z) < 8.0f && !random_integer_below(19)) {
            if (moby->group != 0xFF)
                FUN_L01_0026e090(moby->group, 1);
            if (random_integer_below(0x100) & 1)
                ground_enemy_vars->side_deg = -ground_enemy_vars->side_deg;
            ground_enemy_vars->side = ground_enemy_vars->side_deg * 0.017453292f;
            ground_enemy_vars->side_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
            moby->command = 0;
        } else if (moby->command == 1 && !random_integer_below(19)) {
            if (random_integer_below(0x100) & 1)
                ground_enemy_vars->side_deg = -ground_enemy_vars->side_deg;
            ground_enemy_vars->side = ground_enemy_vars->side_deg * 0.017453292f;
            ground_enemy_vars->side_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
            moby->command = 0;
        }
        if (moby->prev_seq == 4 && (moby->anim_flags & 2))
            blend_moby_animation(moby, 2, 0, scale_game_frames(random_int_between(10, 13)));
        break;
    case GROUND_ENEMY_STATE_CHASE: {
        int   flags;
        float steep;
        steep = absolute_float(moby->pos.z - query.z);
        steep /= vector_distance_xy(&moby->pos, &query);
        if (ground_enemy_vars->path != -1)
            FUN_L01_0028b5e0_c(moby, ((char *)l09_paths[ground_enemy_vars->path]), &query, &goal);
        else
            qcopy(&goal, &query);
        approach_w(moby,
                   fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y) + ground_enemy_vars->side,
                   ground_enemy_vars->turn,
                   0.05f,
                   0.3f,
                   0.2f);
        step.x = 2.0f * fast_cos(moby->rot.z);
        step.y = 2.0f * fast_sin(moby->rot.z);
        step.z = 0.0f;
        fast_vec_add(&goal, &moby->pos, &step);
        flags = move_w(moby, &ground_enemy_vars->steer, &goal, &step, 1.0f);
        if (fast_dec_timer(&ground_enemy_vars->side_timer)) {
            ground_enemy_vars->side_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            ground_enemy_vars->side = -ground_enemy_vars->side;
        }
        fast_vec_sub(&away, &query, &ground_enemy_vars->home);
        if ((flags & 4) || (vector_distance_xy(&moby->pos, &query) < 1.5f && !(flags & 2))) {
            moby->state = 4;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (ground_enemy_vars->f190 < frame_time || 1.0f < steep || (flags & 2)) {
            ground_enemy_vars->home_timer = scale_game_frames(50);
            moby->state = 5;
        } else if (fast_vec_length(&away) < ground_enemy_vars->range + 4.0f &&
                   absolute_float(moby->pos.z - query.z) < 8.0f) {
            if (moby->group != 0xFF)
                FUN_L01_0026e090(moby->group, 1);
        } else if (moby->command != 1) {
            moby->state = 5;
        }
        moby->command = 0;
        break;
    }
    case GROUND_ENEMY_STATE_MELEE_ATTACK:
        approach_w(moby,
                   fast_arc_tan(query.x - moby->pos.x, query.y - moby->pos.y),
                   ground_enemy_vars->turn,
                   0.05f,
                   0.3f,
                   0.2f);
        if (moby->seq == moby->prev_seq && compute_interpolated_record_value(moby) == 13.0f &&
            absolute_float(moby->pos.z - query.z) < 0.5f &&
            vector_distance_xy(&moby->pos, &query) < 2.0f &&
            fast_difference_between_rotations(
                fast_arc_tan(query.x - moby->pos.x, query.y - moby->pos.y), moby->rot.z) <
                0.2617994f) {
            step.x = fast_cos(moby->rot.z) * 0.2f;
            step.y = fast_sin(moby->rot.z) * 0.2f;
            step.z = 0.0f;
            qcopy(&goal, &query);
            goal.z += 0.75f;
            spawn_nearby_effect(query.who, moby, 1, &goal, &step, 1.0f);
        }
        if ((moby->anim_flags & 2) && vector_distance_xy(&moby->pos, &query) > 1.5f) {
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        }
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        ground_enemy_vars->fall = ground_enemy_vars->fall - frame_time_sq * 9.8f;
        moby->pos.z = moby->pos.z + ground_enemy_vars->fall;
        if (moby->pos.z < ground) {
            ground_enemy_vars->fall = 0.0f;
            moby->pos.z = ground;
        }
        break;
    case GROUND_ENEMY_STATE_RETURN_HOME: {
        int flags;
        if (ground_enemy_vars->path != -1)
            FUN_L01_0028b5e0_c(moby, ((char *)l09_paths[ground_enemy_vars->path]), &ground_enemy_vars->home, &goal);
        else
            qcopy(&goal, &ground_enemy_vars->home);
        approach_w(moby,
                   fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y),
                   ground_enemy_vars->turn,
                   0.02f,
                   0.3f,
                   0.1f);
        step.x = 2.0f * fast_cos(moby->rot.z);
        step.y = 2.0f * fast_sin(moby->rot.z);
        step.z = 0.0f;
        fast_vec_add(&goal, &moby->pos, &step);
        flags = move_w(moby, &ground_enemy_vars->steer, &goal, &step, 1.0f);
        if (vector_distance_xy(&moby->pos, &ground_enemy_vars->home) < 2.0f) {
            moby->state = 1;
            if (moby->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(random_int_between(10, 13)));
        }
        fast_vec_sub(&away, &query, &ground_enemy_vars->home);
        if (expired && fast_vec_length(&away) < ground_enemy_vars->range) {
            if (moby->group != 0xFF)
                FUN_L01_0026e090(moby->group, 1);
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (expired && moby->command == 1) {
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (flags & 2) {
            moby->pos.x = previous.x;
            moby->pos.y = previous.y;
            moby->state = 12;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(random_int_between(10, 13)));
        }
        moby->command = 0;
        break;
    }
    case GROUND_ENEMY_STATE_HURT_KNOCKBACK:
        if (step_moby_motion(moby, ground_enemy_vars->motion) & 1) {
            moby->state = 9;
            if (moby->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(random_int_between(10, 13)));
            return;
        }
        if (moby->pos.z < 0.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case GROUND_ENEMY_STATE_RECOVER_TO_IDLE:
        if (recover_2E39E0(moby, ground_enemy_vars->motion)) {
            moby->state = 1;
            if (moby->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, 0);
            ground_enemy_vars->fC8 = 0;
        }
        break;
    case GROUND_ENEMY_STATE_HURT_RECOVERY:
        if (compute_interpolated_record_value(moby) > 29.0f) {
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
            return;
        }
        break;
    case GROUND_ENEMY_STATE_FATAL_KNOCKBACK:
        if (step_moby_motion(moby, ground_enemy_vars->motion) & 0x40) {
            clear_u64_value(&step);
            cart_burst(moby, &moby->pos, 6, 0.5f, 13.0f);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->pos.z < 0.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case GROUND_ENEMY_STATE_BLOCKED_RETURN_HOME: {
        int   flags;
        float steep;
        steep = absolute_float(moby->pos.z - query.z);
        steep /= vector_distance_xy(&moby->pos, &query);
        if (ground_enemy_vars->path != -1)
            FUN_L01_0028b5e0_c(moby, ((char *)l09_paths[ground_enemy_vars->path]), &ground_enemy_vars->home, &goal);
        else
            qcopy(&goal, &ground_enemy_vars->home);
        approach_w(moby,
                   fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y),
                   ground_enemy_vars->turn,
                   0.05f,
                   0.3f,
                   0.2f);
        step.x = 2.0f * fast_cos(moby->rot.z);
        step.y = 2.0f * fast_sin(moby->rot.z);
        step.z = 0.0f;
        fast_vec_add(&goal, &moby->pos, &step);
        flags = move_w(moby, &ground_enemy_vars->steer, &goal, &step, 1.0f);
        fast_vec_sub(&away, &query, &ground_enemy_vars->home);
        if ((flags & 4) || (vector_distance_xy(&moby->pos, &query) < 1.5f && steep <= 1.0f)) {
            moby->state = 4;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (steep <= 1.0f && !(flags & 2)) {
            moby->state = 5;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        } else if ((flags & 2) && expired) {
            moby->state = 1;
            if (moby->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (moby->anim_flags & 2) {
            int animation;
            random_integer_below(4);
            animation = random_integer_below(4) ? 2 : 4;
            blend_moby_animation(moby, animation, 0, scale_game_frames(random_int_between(10, 13)));
        }
        moby->command = 0;
        break;
    }
    case GROUND_ENEMY_STATE_LEASHED_FORWARD_WALK: {
        Moby *avoidance_candidate;
        if (moby->seq != 0 && moby->prev_seq != 0)
            blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        step.x = fast_cos(moby->rot.z) * (ground_enemy_vars->speed * frame_time);
        step.y = fast_sin(moby->rot.z) * (ground_enemy_vars->speed * frame_time);
        step.z = 0.0f;
        fast_vec_add(&goal, &moby->pos, &step);
        for (avoidance_candidate = D_L09_00160064_2E39E0; avoidance_candidate != 0; avoidance_candidate = avoidance_candidate->next) {
            if (avoidance_candidate != moby && (signed char)avoidance_candidate->state >= 0 &&
                (avoidance_candidate->oclass == 0 || avoidance_candidate->oclass == 193)) {
                if (vector_distance_xy(&goal, &avoidance_candidate->pos) < 2.0f) {
                    float old_z = moby->pos.z;
                    float bearing =
                        fast_arc_tan(avoidance_candidate->pos.x - moby->pos.x, avoidance_candidate->pos.y - moby->pos.y);
                    float turn_side = fast_subtract_rotations(moby->rot.z, bearing);
                    float turn = frame_time * 1.5707964f;
                    if (!(turn_side > 0.0f))
                        turn = -turn;
                    moby->rot.z = fast_add_rotations(moby->rot.z, turn);
                    fast_vec_sub(&goal, &moby->pos, &avoidance_candidate->pos);
                    fast_vec_normalize(&goal, &goal, 2.0f);
                    fast_vec_add(&goal, &goal, &avoidance_candidate->pos);
                    goal.z = old_z;
                }
            }
        }
        qcopy(&moby->pos, &goal);
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        ground_enemy_vars->fall = ground_enemy_vars->fall - frame_time_sq * 9.8f;
        moby->pos.z = moby->pos.z + ground_enemy_vars->fall;
        if (moby->pos.z < ground) {
            ground_enemy_vars->fall = 0.0f;
            moby->pos.z = ground;
        }
        if (ground_enemy_vars->anchor == 0 || vector_distance_xy(&moby->pos, &ground_enemy_vars->anchor->pos) >
                                     convert_integer_to_float(ground_enemy_vars->leash)) {
            moby->state = 5;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        }
        moby->command = 0;
        break;
    }
    }
    if (ground == -1.0f)
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
    if (ground != 0.0f && moby->pos.z - ground < 0.1f && collision_hit_material() == 1) {
        cart_burst(moby, &moby->pos, 6, 0.5f, 13.0f);
        mark_moby_for_removal(moby);
        return;
    }
    effects_update_2E39E0(moby, ground_enemy_vars->effects);
}

int   D_L15_001619D8 = 0x7000000;
float D_L15_001619C8 = 7.0f;
float D_L15_001619CC = 3.0f;
