/* Level 14 minibomber moby class: path following, bombing and exhaust effects. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/math/interpolation/cosine_interpolate.h"
#include "rnc/overlay/level_path.h"
#include "sda.h"
#include "rnc/overlay/l14/mobys/puff.h"
#include "rnc/overlay/l14/mobys/minibomber_globals.h"

extern void release_voice_slot(s32 idx) __asm__("FUN_0022d798");

extern void FUN_L00_0025a120(void *);

extern void fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");

extern void FUN_L00_001fff28(void *, int, float);

extern char D_L14_001674C0[];

extern void FUN_L00_00257470(void *, int, int);

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern float FUN_L00_0025b8c0_c(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");

extern int debug_print() __asm__("FUN_001e93b0");

typedef union PathNode {
    float v[4];
    int   n;
} PathNode;

typedef struct Class_2B5938 {
    char pad00[0x10];
    int  f10;
} Class_2B5938;

typedef struct Moby_2B5938 {
    char           pad00[0x10];
    float          pos[4]; /* 0x10 */
    unsigned char  state;  /* 0x20 */
    char           pad21[3];
    Class_2B5938  *cls; /* 0x24 */
    char           pad28[8];
    unsigned char  f30; /* 0x30 */
    unsigned char  f31; /* 0x31 */
    char           pad32[2];
    unsigned short f34; /* 0x34 */
    char           pad36[0x12];
    float          rot; /* 0x48 */
    char           pad4C[6];
    unsigned char  f52; /* 0x52 */
    unsigned char  f53; /* 0x53 */
    char           pad54[0x1C];
    unsigned char  f70; /* 0x70 */
    char           pad71[7];
    struct Data   *d; /* 0x78 */
    char           pad7C[3];
    unsigned char  f7F; /* 0x7F */
    char           pad80[0x14];
    int            f94; /* 0x94 */
    char           pad98[0x1A];
    short          fB2; /* 0xB2 */
} Moby_2B5938;

typedef struct Data {
    char         pad00[0x40];
    float        f40[2]; /* 0x40 */
    float        f48;    /* 0x48 */
    char         pad4C[0x64];
    Moby_2B5938 *fB0; /* 0xB0 */
    int          fB4; /* 0xB4 */
    char         padB8[0x18];
    int          fD0; /* 0xD0 */
    int          fD4; /* 0xD4 */
    int          fD8; /* 0xD8 */
    int          fDC; /* 0xDC */
    char         padE0[4];
    float        fE4;        /* 0xE4 */
    int          fE8;        /* 0xE8 */
    float        fEC;        /* 0xEC */
    float        fF0[4];     /* 0xF0 */
    short        f100;       /* 0x100 */
    short        f102;       /* 0x102 */
    float        f104;       /* 0x104 */
    int          f108;       /* 0x108 */
    int          f10C;       /* 0x10C */
    char         f110[0x10]; /* 0x110 */
    char         f120[0x30]; /* 0x120 */
    int          f150;       /* 0x150 */
    float        f154;       /* 0x154 */
    float        f158;       /* 0x158 */
    float        f15C;       /* 0x15C */
    float        f160;       /* 0x160 */
    float        f164;       /* 0x164 */
    float        f168;       /* 0x168 */
    float        f16C;       /* 0x16C */
    float        f170;       /* 0x170 */
    float        f174;       /* 0x174 */
    float        f178;       /* 0x178 */
    float        f17C;       /* 0x17C */
    float        f180[4];    /* 0x180 */
} Data;

char D_L14_001FBDF0[] = "Hovertank with no introPath. UID: %d\n";

char D_L14_001FBE18[] = "Hovertank with a intro path that has no points. UID: %d\n";

char D_L14_001FBE58[] = "Hovertank with no boundPath. UID: %d\n";

char D_L14_001FBE80[] = "Hovertank with a bound path that has no points. UID: %d\n";

char D_L14_001FBEC0[] = "Hovertank with no bombPath. UID: %d\n";

char D_L14_001FBEE8[] = "Hovertank with a bomb path that has no points. UID: %d\n";

char D_L14_001FBF20[] = "Hovertank with no exit Path. UID: %d\n";

/* "Hovertank with a exit path that has no points. UID: %d\n" */
char minibomber_empty_exit_path_diagnostic[56] __asm__("D_L14_001FBF48") =
    "Hovertank with a exit path that has no points. UID: %d\n";

extern float D_0015ED60;

extern float minibomber_bomb_gravity_gp __asm__("D_L14_00161568") __attribute__((sda));

extern float minibomber_intro_exit_path_step_distance_gp __asm__("D_L14_0016154C")
    __attribute__((sda));

extern float minibomber_traversal_path_step_distance_gp __asm__("D_L14_00161550")
    __attribute__((sda));

extern float minibomber_retrace_path_step_distance_gp __asm__("D_L14_00161554")
    __attribute__((sda));

extern int minibomber_bomb_aim_duration_frames __asm__("D_L14_0016155C") __attribute__((sda));

extern int minibomber_aim_return_duration_frames __asm__("D_L14_00161560") __attribute__((sda));

extern int minibomber_bomb_flight_duration_frames __asm__("D_L14_00161564") __attribute__((sda));

extern int minibomber_bomb_initial_fade_timer_frames __asm__("D_L14_0016156C") __attribute__((sda));

extern float minibomber_path_traversal_trigger_distance __asm__("D_L14_00161570")
    __attribute__((sda));

extern float minibomber_intro_exit_position_error_gain_gp __asm__("D_L14_00161574")
    __attribute__((sda));

extern float minibomber_position_velocity_damping_gp __asm__("D_L14_00161578") __attribute__((sda));

extern float minibomber_position_snap_threshold_scale_gp __asm__("D_L14_0016157C")
    __attribute__((sda));

extern float minibomber_intro_exit_heading_error_gain_gp __asm__("D_L14_00161580")
    __attribute__((sda));

extern float minibomber_heading_velocity_damping_gp __asm__("D_L14_00161584") __attribute__((sda));

extern float minibomber_heading_velocity_limit_gp __asm__("D_L14_00161588") __attribute__((sda));

extern float minibomber_retrace_and_traversal_position_error_gain_gp __asm__("D_L14_0016158C")
    __attribute__((sda));

extern float minibomber_retrace_and_traversal_heading_error_gain_gp __asm__("D_L14_00161590")
    __attribute__((sda));

extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

extern float D_L14_00161540 __attribute__((sda));

extern unsigned char *
FUN_L14_002bb310_c(char *owner, float angle, char *posp) __asm__("FUN_L14_002bb310");

extern void FUN_L14_002b5290_c(unsigned char *m) __asm__("FUN_L14_002b5290");

extern void FUN_L14_002b5428_c(char *moby) __asm__("FUN_L14_002b5428");

extern void FUN_L14_002b5530_c(char *moby) __asm__("FUN_L14_002b5530");

extern void FUN_L14_002b55b0_c(char *moby) __asm__("FUN_L14_002b55b0");

extern void ease_minibomber_position_and_heading(char  *moby,
                                                 float *target,
                                                 float  a,
                                                 float  b,
                                                 float  c,
                                                 float  d,
                                                 float  e,
                                                 float  f,
                                                 float  g) __asm__("FUN_L14_002b5850");

extern void FUN_L14_002b5968_u(char *m, float *input, float angle) __asm__("FUN_L14_002b5968");

extern void FUN_L14_002b5c30_c(char *moby) __asm__("FUN_L14_002b5c30");

extern void FUN_L14_002b5c98_c(char *moby) __asm__("FUN_L14_002b5c98");

extern void FUN_L14_002b5d18_c(char *moby) __asm__("FUN_L14_002b5d18");

extern void FUN_L14_002b5dd8_c(char *m) __asm__("FUN_L14_002b5dd8");

extern void release_voice_slot(s32) __asm__("FUN_0022d798");

extern void set_moby_animation_c(void *, s32, s32) __asm__("func_00212ED8");

int advance_minibomber_along_path(struct Moby *moby,
                                  char        *a1,
                                  int          a2,
                                  int          a3,
                                  float       *v,
                                  float       *vel,
                                  float        f12,
                                  float        f13,
                                  float        f14,
                                  float        f15,
                                  float        f16,
                                  float        f17,
                                  float        f18) __asm__("FUN_L14_002b5730");

s32 is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");

extern char *
func_L14_00302968_2B5938(int a0, float *a1, float *a2, float f, int a3) __asm__("FUN_L14_003014e0");

extern void func_L00_0025F4A8_2B5938(void *,
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

void minibomber_update(Moby_2B5938 *moby) __asm__("FUN_L14_002b46e8");

void minibomber_update(Moby_2B5938 *moby) {
    enum {
        MINIBOMBER_STATE_INITIALIZE = 0,
        MINIBOMBER_STATE_WAIT_TO_FOLLOW_PATH = 1,
        MINIBOMBER_STATE_FOLLOW_INTRO_OR_EXIT_PATH = 2,
        MINIBOMBER_STATE_RETRACE_INTRO_OR_EXIT_PATH = 3,
        MINIBOMBER_STATE_HOLD_AT_PATH_END = 4,
        MINIBOMBER_STATE_AIM_AT_BOMB_POINT = 5,
        MINIBOMBER_STATE_RELEASE_BOMB = 6,
        MINIBOMBER_STATE_TRAVERSE_PATH = 7,
        MINIBOMBER_STATE_DESTROY = 9
    };
    Data  *d = moby->d;
    float  v[12];
    float *path_endpoint_target_position;
    char  *manipulator_state;
    float  tg;
    float  blended_aim_angle;
    float  n;
    float  bomb_gravity;
    float  spd;

    if (moby->f70 & 2) {
        if (moby->f52 == moby->f53 && moby->f52 != 0) {
            if (moby->f52 == 2) {
                blend_moby_animation(moby, 0, 0, 10);
            } else {
                set_moby_animation_c(moby, 0, 0);
            }
        }
    }
    path_endpoint_target_position = d->f180;
    FUN_L14_002b5290_c((unsigned char *)moby);
    FUN_L14_002b5530_c((char *)moby);
    ease_minibomber_position_and_heading((char *)moby,
                                         path_endpoint_target_position,
                                         d->f17C,
                                         d->f170,
                                         d->f174,
                                         d->f178,
                                         d->f154,
                                         d->f158,
                                         d->f15C);
    FUN_L14_002b5428_c((char *)moby);
    FUN_L14_002b5dd8_c((char *)moby);
    if (moby->f31 != 0) {
        if (fast_vec_dist(moby->pos, D_L14_001674C0) < 40.0f && moby->state != 0 &&
            moby->state != 1 && moby->state != 9) {
            FUN_L00_0025a120(moby);
        }
    }
    switch (moby->state) {
    case MINIBOMBER_STATE_INITIALIZE: {
        PathNode *path;
        if (d->fD0 == -1) {
            debug_print(D_L14_001FBDF0, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        path = ((PathNode *)l14_paths[d->fD0]);
        if (path->n == 0) {
            debug_print(D_L14_001FBE18, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (d->fD4 == -1) {
            debug_print(D_L14_001FBE58, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (((PathNode *)l14_paths[d->fD4])->n == 0) {
            debug_print(D_L14_001FBE80, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (d->fD8 == -1) {
            debug_print(D_L14_001FBEC0, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (((PathNode *)l14_paths[d->fD8])->n == 0) {
            debug_print(D_L14_001FBEE8, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (d->f150 == -1) {
            debug_print(D_L14_001FBF20, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (((PathNode *)l14_paths[d->f150])->n == 0) {
            debug_print(minibomber_empty_exit_path_diagnostic, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        qcopy(moby->pos, path + 1);
        fast_vec_sub(v, path + 2, moby->pos);
        manipulator_state = d->f110;
        moby->rot = fast_arc_tan(v[0], v[1]);
        d->f160 = *(float *)&D_L14_00161540;
        d->f164 = 0.0f;
        d->f168 = 0.0f;
        d->f16C = 0.0f;
        FUN_L14_002b55b0_c((char *)moby);
        d->f108 = -1;
        moby->f30 = 0x80;
        moby->state = 1;
        FUN_L14_002b5c98_c((char *)moby);
        fill_transfer_words(manipulator_state, 0, 0x40);
        attach_manipulator(moby, 0, manipulator_state);
        moby->f7F = 0x20;
        break;
    }
    case MINIBOMBER_STATE_WAIT_TO_FOLLOW_PATH:
        if (d->fB4 != 2) {
            moby->state = 2;
            moby->f34 &= 0xFFBE;
            moby->f94 = moby->cls->f10;
            FUN_L14_002b5d18_c((char *)moby);
        }
        break;
    case MINIBOMBER_STATE_FOLLOW_INTRO_OR_EXIT_PATH: {
        PathNode *path;
        if (d->f10C == 0) {
            path = ((PathNode *)l14_paths[d->fD0]);
        } else {
            path = ((PathNode *)l14_paths[d->f150]);
        }
        if (d->fB4 == 2) {
            moby->state = 3;
        } else if (advance_minibomber_along_path((char *)moby,
                                                 (char *)path,
                                                 (int)&d->fE8,
                                                 (int)&d->fEC,
                                                 d->fF0,
                                                 &d->fE4,
                                                 minibomber_intro_exit_path_step_distance_gp *
                                                     D_0015ED60,
                                                 minibomber_intro_exit_position_error_gain_gp,
                                                 minibomber_position_velocity_damping_gp,
                                                 minibomber_position_snap_threshold_scale_gp,
                                                 minibomber_intro_exit_heading_error_gain_gp,
                                                 minibomber_heading_velocity_damping_gp,
                                                 minibomber_heading_velocity_limit_gp) != 0) {
            qcopy(d->f180, path + path->n);
            path = ((PathNode *)l14_paths[d->fDC]);
            if (d->f10C == 0) {
                qcopy(v, path + 1);
                qcopy(v + 4, path + path->n);
            } else {
                qcopy(v, path + path->n);
                qcopy(v + 4, path + 1);
            }
            fast_vec_sub(v + 8, v + 4, v);
            d->f17C = fast_arc_tan(v[8], v[9]);
            d->f170 = minibomber_intro_exit_position_error_gain_gp;
            d->f174 = minibomber_position_velocity_damping_gp;
            d->f178 = minibomber_position_snap_threshold_scale_gp;
            d->f154 = minibomber_intro_exit_heading_error_gain_gp;
            d->f158 = minibomber_heading_velocity_damping_gp;
            d->f15C = minibomber_heading_velocity_limit_gp;
            moby->state = 4;
            d->f104 = 0.0f;
            d->f102 = 0;
        }
        break;
    }
    case MINIBOMBER_STATE_RETRACE_INTRO_OR_EXIT_PATH: {
        PathNode *path;
        if (d->f10C == 0) {
            path = ((PathNode *)l14_paths[d->fD0]);
        } else {
            path = ((PathNode *)l14_paths[d->f150]);
        }
        if (d->fB4 != 2) {
            moby->state = 2;
        } else if (advance_minibomber_along_path(
                       (char *)moby,
                       (char *)path,
                       (int)&d->fE8,
                       (int)&d->fEC,
                       d->fF0,
                       &d->fE4,
                       -(minibomber_retrace_path_step_distance_gp * D_0015ED60),
                       minibomber_retrace_and_traversal_position_error_gain_gp,
                       minibomber_position_velocity_damping_gp,
                       minibomber_position_snap_threshold_scale_gp,
                       minibomber_retrace_and_traversal_heading_error_gain_gp,
                       minibomber_heading_velocity_damping_gp,
                       minibomber_heading_velocity_limit_gp) != 0) {
            moby->state = 1;
            FUN_L14_002b5c98_c((char *)moby);
        }
        break;
    }
    case MINIBOMBER_STATE_HOLD_AT_PATH_END:
        tg = 0.0f;
        blended_aim_angle = cosine_interpolate(
            tg,
            d->f104,
            (float)d->f102 /
                convert_integer_to_float(scale_game_frames(minibomber_aim_return_duration_frames)));
        FUN_L00_001fff28(d->f120, 2, blended_aim_angle);
        if (vector_distance_xy(moby->pos, d->fB0->pos) <
            minibomber_path_traversal_trigger_distance) {
            if (d->f10C == 0) {
                d->fE8 = 0;
                d->fEC = 0.0f;
            } else {
                PathNode *q;
                d->fE8 = ((PathNode *)l14_paths[d->fDC])->n - 2;
                q = ((PathNode *)l14_paths[d->fDC]) + d->fE8;
                d->fEC = q[1].v[3];
            }
            moby->state = 7;
            d->fE4 = 0.0f;
            clear_u64_value(d->fF0);
        } else if (d->fB4 == 2) {
            moby->state = 3;
            FUN_L14_002b5c30_c((char *)moby);
        } else if (fast_dec_timer_s16(&d->f102) != 0) {
            if (moby->f53 != 2) {
                moby->state = 5;
                d->f102 = scale_game_frames(minibomber_bomb_aim_duration_frames);
                d->f104 = 0.0f;
                d->f100 = 0;
            }
        }
        break;
    case MINIBOMBER_STATE_TRAVERSE_PATH:
        spd = minibomber_traversal_path_step_distance_gp * D_0015ED60;
        if (d->f10C == 1) {
            spd = -spd;
        }
        if (advance_minibomber_along_path((char *)moby,
                                          (char *)l14_paths[d->fDC],
                                          (int)&d->fE8,
                                          (int)&d->fEC,
                                          d->fF0,
                                          &d->fE4,
                                          spd,
                                          minibomber_retrace_and_traversal_position_error_gain_gp,
                                          minibomber_position_velocity_damping_gp,
                                          minibomber_position_snap_threshold_scale_gp,
                                          minibomber_retrace_and_traversal_heading_error_gain_gp,
                                          minibomber_heading_velocity_damping_gp,
                                          minibomber_heading_velocity_limit_gp) != 0) {
            PathNode *path;
            moby->state = 4;
            if (d->f10C == 0) {
                d->f10C = 1;
            } else {
                d->f10C = 0;
            }
            path = ((PathNode *)l14_paths[d->fDC]);
            if (d->f10C == 0) {
                qcopy(v, path + 1);
                qcopy(v + 4, path + path->n);
            } else {
                qcopy(v, path + path->n);
                qcopy(v + 4, path + 1);
            }
            d->f170 = minibomber_retrace_and_traversal_position_error_gain_gp;
            d->f174 = minibomber_position_velocity_damping_gp;
            d->f178 = minibomber_position_snap_threshold_scale_gp;
            d->f154 = minibomber_retrace_and_traversal_heading_error_gain_gp;
            d->f158 = minibomber_heading_velocity_damping_gp;
            d->f15C = minibomber_heading_velocity_limit_gp;
            fast_vec_sub(v + 8, v + 4, v);
            d->f17C = fast_arc_tan(v[8], v[9]);
            qcopy(d->f180, v);
        }
        break;
    case MINIBOMBER_STATE_AIM_AT_BOMB_POINT: {
        PathNode *q = ((PathNode *)l14_paths[d->fD8]) + d->f100;
        tg = fast_subtract_rotations(
            fast_arc_tan(q[1].v[0] - moby->pos[0], q[1].v[1] - moby->pos[1]), moby->rot);
        blended_aim_angle = cosine_interpolate(
            tg,
            d->f104,
            (float)d->f102 /
                convert_integer_to_float(scale_game_frames(minibomber_bomb_aim_duration_frames)));
        FUN_L00_001fff28(d->f120, 2, blended_aim_angle);
        if (moby->f53 == 2) {
            moby->state = 4;
            d->f102 = scale_game_frames(minibomber_aim_return_duration_frames);
        } else if (fast_dec_timer_s16(&d->f102) != 0) {
            d->f104 = blended_aim_angle;
            moby->state = 6;
            set_moby_animation_c(moby, 1, 0);
        }
        break;
    }
    case MINIBOMBER_STATE_RELEASE_BOMB:
        if (is_value_within_interpolated_window(moby, 3.0f) != 0) {
            PathNode *q;
            n = (float)scale_game_frames(minibomber_bomb_flight_duration_frames);
            FUN_L00_001fff28(d->f120, 2, d->f104);
            moby_point_position(moby, 1, v);
            fast_vec_sub(v + 4, ((PathNode *)l14_paths[d->fD8]) + (d->f100 + 1), v);
            v[6] = 0.0f;
            fast_vec_scale(v + 4, v + 4, 1.0f / n);
            bomb_gravity = minibomber_bomb_gravity_gp * frame_time_sq;
            q = ((PathNode *)l14_paths[d->fD8]) + d->f100;
            d->f48 = -((v[2] - q[1].v[2]) + -bomb_gravity * (n * n) * 0.5f) / n;
            func_L14_00302968_2B5938((int)moby,
                                     v,
                                     v + 4,
                                     bomb_gravity,
                                     scale_game_frames(minibomber_bomb_initial_fade_timer_frames));
            FUN_L14_002b5968_u((char *)moby, v, d->f104);
            d->f100++;
            if (d->f100 >= ((PathNode *)l14_paths[d->fD8])->n) {
                moby->state = 4;
                d->f102 = scale_game_frames(minibomber_aim_return_duration_frames);
            } else {
                d->f102 = scale_game_frames(minibomber_bomb_aim_duration_frames);
                moby->state = 5;
            }
        }
        break;
    case MINIBOMBER_STATE_DESTROY: {
        int i;
        moby_point_position(moby, 2, v);
        FUN_L14_002bb310_c((char *)moby, moby->rot, (char *)v);
        moby_point_position(moby, 3, v);
        FUN_L14_002bb310_c((char *)moby, fast_add_rotations(moby->rot, 3.1415927f), (char *)v);
        func_L00_0025F4A8_2B5938(moby,
                                 d->f40,
                                 moby->pos,
                                 0.0f,
                                 0.0f,
                                 5,
                                 2,
                                 6,
                                 4.0f,
                                 1.0f,
                                 9.0f,
                                 1.0f,
                                 1,
                                 15.0f,
                                 1,
                                 1,
                                 -1,
                                 0);
        i = d->f108;
        if (i != -1) {
            char *voice_slot = (char *)&voice_pool + i * 0x70;
            if (*(Moby_2B5938 **)(voice_slot + 0x88) == moby &&
                *(unsigned char *)(voice_slot + 0x74) != 0) {
                release_voice_slot(i);
            }
        }
        d->f108 = -1;
        mark_moby_for_removal(moby);
        return;
    }
    }
    if (moby->state > 1 && moby->state != 9) {
        if (voice_is_playing(moby, d->f108) == 0) {
            d->f108 = allocate_voice_for_target_entry(0, 4, (int)moby);
        }
    }
}

extern int FUN_L00_0025a478_c(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");

extern void FUN_00212f90(void *, int, int, int);

extern void FUN_L00_0025d458_c(void *, void *) __asm__("FUN_L00_0025d458");

void process_minibomber_hit_reactions(struct Moby *hit_owner_moby) __asm__("FUN_L14_002b5290");

void process_minibomber_hit_reactions(struct Moby *hit_owner_moby) {
    enum {
        HIT_REACTION_NO_ACTION = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_DESTROY = 2,
        HIT_REACTION_PLAY_HIT_ANIMATION = 3,
        HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION = 4,
        HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION = 5,
        HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH = 6,
        HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION = 7,
        HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH = 8,
        HIT_REACTION_RED_FLASH_ONLY = 9,
        HIT_REACTION_FLASH_RED = 10,
        HIT_REACTION_NONE = 11
    };
    unsigned char *moby_vars = hit_owner_moby->pvars;
    int            loc0;
    float          damage_amount;
    char          *hit_record;
    int            k;
    float          remaining_health;
    if (hit_owner_moby->state == 0 || hit_owner_moby->state == 9)
        return;
    hit_owner_moby->scale = hit_owner_moby->pclass->scale;
    damage_amount = 0.0f;
    hit_record = find_moby_hit(hit_owner_moby, 0x330000, 0);
    if (hit_record != 0) {
        if (*(int *)(hit_record + 0x20) != 0) {
            int hit_object_class = *(short *)(*(int *)(hit_record + 0x20) + 0xA6);
            if (hit_object_class == hit_owner_moby->oclass || hit_object_class == 0x370)
                hit_record = 0;
        }
    }
    k = FUN_L00_0025a478_c(
        hit_owner_moby, hit_record, moby_vars + 0x20, 0, &loc0, &damage_amount, 0, 4);
    if (loc0 != 1 && hit_owner_moby->state != 9) {
        remaining_health = *(float *)(moby_vars + 0x20) - damage_amount;
        *(float *)(moby_vars + 0x20) = remaining_health;
        if (remaining_health <= 0.0f)
            k = 1;
        switch (k) {
        case HIT_REACTION_NO_ACTION:
        case HIT_REACTION_NONE:
            break;
        case HIT_REACTION_PLAY_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH:
            FUN_00212f90(hit_owner_moby, 2, 0, 5);
            hit_owner_moby->anim_speed = 1.75f;
        case HIT_REACTION_RED_FLASH_ONLY:
        case HIT_REACTION_FLASH_RED:
            moby_vars[0x67] = 0xFA;
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_DESTROY:
            hit_owner_moby->state = 9;
            hit_owner_moby->flags &= 0xEFFF;
            moby_vars[0x67] = 0xFA;
            FUN_L00_00257470(hit_owner_moby, 0, -1);
            break;
        }
        FUN_L00_0025d458_c(hit_owner_moby, moby_vars + 0x60);
    }
    hit_owner_moby->hit_slot = 0xFF;
    trooper_anim_finish(hit_owner_moby, moby_vars + 0x60);
}

extern f32 D_L14_00161540_gp __asm__("D_L14_00161540") __attribute__((sda));

extern void FUN_L00_00262b00_c(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");

extern void
FUN_L00_00262b80_c(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");

extern short minibomber_vertical_oscillation_amplitude_gp __asm__("D_L14_00161538")
    __attribute__((sda));

extern short minibomber_vertical_oscillation_phase_increment_target_gp __asm__("D_L14_0016153C")
    __attribute__((sda));

extern short minibomber_rotation_oscillation_first_phase_increment_gp __asm__("D_L14_00161544")
    __attribute__((sda));

extern short minibomber_rotation_oscillation_second_phase_increment_gp __asm__("D_L14_00161548")
    __attribute__((sda));

void update_minibomber_oscillation(struct Moby *moby) __asm__("FUN_L14_002b5428");

void update_minibomber_oscillation(struct Moby *moby) {
    int           raw = moby->state;
    unsigned char state = raw;
    char         *data = (char *)moby->pvars;
    float         spring_target;
    if (state == 0 || state == 9 || state == 1)
        return;
    spring_target = *(float *)&D_L14_00161540_gp;
    if ((raw >= 2 && raw <= 3) || state == 7) {
        spring_target = spring_target * 1.5f;
    } else {
        FUN_L00_0025b8c0_c((float *)(data + 0x168),
                           (float *)(data + 0x16C),
                           *(float *)&minibomber_vertical_oscillation_phase_increment_target_gp,
                           0.002f,
                           0.2f,
                           0.0f);
        FUN_L00_00262b00_c(*(float *)&minibomber_vertical_oscillation_amplitude_gp,
                           *(float *)(data + 0x168),
                           moby,
                           (float *)(data + 0xC0),
                           (float *)(data + 0xC4));
    }
    FUN_L00_0025b8c0_c(
        (float *)(data + 0x160), (float *)(data + 0x164), spring_target, 0.003f, 0.2f, 0.0f);
    FUN_L00_00262b80_c(moby,
                       data + 0xC8,
                       data + 0xCC,
                       *(float *)(data + 0x160),
                       *(float *)&minibomber_rotation_oscillation_first_phase_increment_gp,
                       *(float *)&minibomber_rotation_oscillation_second_phase_increment_gp);
}

extern int FUN_L00_0025ff38(float a, char *m, char *b, int c, int d, int *e, int f);

void update_minibomber_target(struct Moby *moby) __asm__("FUN_L14_002b5530");

void update_minibomber_target(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (moby->state != 0 && moby->state != 9) {
        int *bound_path = ((int *)l14_paths[*(int *)(data + 0xD4)]);
        FUN_L00_0025ff38(255.0f, moby, data + 0x70, 0, 0, bound_path + 4, *bound_path);
        if (*(int *)(data + 0xB0) == 0) {
            *(int *)(data + 0xB0) = *(int *)(D_0013E533 + 0x2E9D);
        }
    }
}

void initialize_minibomber_path_segment_lengths(struct Moby *moby) __asm__("FUN_L14_002b55b0");

void initialize_minibomber_path_segment_lengths(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *path;
    int   i;
    path = ((int *)l14_paths[*(int *)(data + 0xD0)]);
    for (i = 0; i < *(int *)path - 1; i++) {
        *(float *)(path + 0x1C + i * 0x10) =
            fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
    }
    path = ((int *)l14_paths[*(int *)(data + 0x150)]);
    for (i = 0; i < *(int *)path - 1; i++) {
        *(float *)(path + 0x1C + i * 0x10) =
            fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
    }
    path = ((int *)l14_paths[*(int *)(data + 0xDC)]);
    for (i = 0; i < *(int *)path - 1; i++) {
        *(float *)(path + 0x1C + i * 0x10) =
            fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
    }
    *(int *)(data + 0x10C) = 0;
}

extern float FUN_L00_0025b8c0_5730(
    float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");

extern void steer_5730(char *moby, float *vel, float target, float k, float d, float max) __asm__("FUN_L00_00258278");

extern int FUN_L00_0025d808_5730() __asm__("FUN_L00_0025d808");

int advance_minibomber_along_path(struct Moby *moby,
                                  char        *path,
                                  int          segment_index_address,
                                  int          segment_distance_address,
                                  float       *position_easing_velocity,
                                  float       *heading_easing_velocity,
                                  float        f12,
                                  float        f13,
                                  float        f14,
                                  float        f15,
                                  float        f16,
                                  float        f17,
                                  float        f18) __asm__("FUN_L14_002b5730");

int advance_minibomber_along_path(struct Moby *moby,
                                  char        *path,
                                  int          segment_index_address,
                                  int          segment_distance_address,
                                  float       *position_easing_velocity,
                                  float       *heading_easing_velocity,
                                  float        path_step_distance,
                                  float        position_error_gain,
                                  float        position_velocity_damping,
                                  float        position_snap_threshold_scale,
                                  float        heading_error_gain,
                                  float        heading_velocity_damping,

                                  float heading_velocity_limit) {
    float path_target_position[3];
    int   path_sample_result = FUN_L00_0025d808_5730(
        path, path_target_position, segment_index_address, segment_distance_address, 0);
    FUN_L00_0025b8c0_5730(&moby->pos.x,
                          position_easing_velocity,
                          path_target_position[0],
                          position_error_gain,
                          position_velocity_damping,
                          position_snap_threshold_scale);
    FUN_L00_0025b8c0_5730(&moby->pos.y,
                          position_easing_velocity + 1,
                          path_target_position[1],
                          position_error_gain,
                          position_velocity_damping,
                          position_snap_threshold_scale);
    FUN_L00_0025b8c0_5730(&moby->pos.z,
                          position_easing_velocity + 2,
                          path_target_position[2],
                          position_error_gain,
                          position_velocity_damping,
                          position_snap_threshold_scale);
    steer_5730(moby,
               heading_easing_velocity,
               fast_arc_tan(position_easing_velocity[0], position_easing_velocity[1]),
               heading_error_gain,
               heading_velocity_damping,
               heading_velocity_limit);
    return path_sample_result != 0;
}

extern void approach_w(char *moby, float target, float *vel, float k, float d, float max) __asm__("FUN_L00_00258278");

void ease_minibomber_position_and_heading(char  *moby,
                                          float *target,
                                          float  target_heading,
                                          float  position_error_gain,
                                          float  position_velocity_damping,
                                          float  position_snap_threshold_scale,
                                          float  heading_error_gain,
                                          float  heading_velocity_damping,

                                          float heading_velocity_limit) {
    int data;
    int moby_state = moby[0x20] & 0xFF;
    if ((moby_state >= 4 && moby_state <= 6) || moby_state == 8) {
        data = *(int *)(moby + 0x78);
        vendor_spring((float *)&MOBY(moby)->pos,
                      (float *)(data + 0xF0),
                      target[0],
                      position_error_gain,
                      position_velocity_damping,
                      position_snap_threshold_scale);
        vendor_spring((float *)&MOBY(moby)->pos.y,
                      (float *)(data + 0xF4),
                      target[1],
                      position_error_gain,
                      position_velocity_damping,
                      position_snap_threshold_scale);
        vendor_spring((float *)&MOBY(moby)->pos.z,
                      (float *)(data + 0xF8),
                      target[2],
                      position_error_gain,
                      position_velocity_damping,
                      position_snap_threshold_scale);
        approach_w(moby,
                   target_heading,
                   (float *)(data + 0xE4),
                   heading_error_gain,
                   heading_velocity_damping,
                   heading_velocity_limit);
    }
}

extern int random_integer_below(int);

extern float random_float_between(float, float);

extern int scale_game_frames(int);

extern void build_look_at_matrix(void *, void *, void *, float);

extern void spawn_two_point_effect_particle(void *, void *) __asm__("FUN_L00_0026c6e8");

extern void mine_camera_blast(int, void *, int, int) __asm__("FUN_L00_002d3838");

/* Settings mine_camera_blast (FUN_L00_002d3838) copies into its blast moby. It reads bytes up to 0x49 and
 * qcopies the first quad, so the object is quad aligned: 0x50 bytes (the l03 mine copies its template as 0x50). */
typedef struct MinibomberBlastSettings {
    f32 position_offset[4]; /* 0x00: qcopied when no override is passed */
    u8  colors[0x10];       /* 0x10: three colour groups (0x10-0x15, 0x16-0x1A, 0x1B-0x1F) */
    f32 values[5];          /* 0x20 */
    s32 word34;             /* 0x34 */
    s32 word38;             /* 0x38 */
    u8  draw_dist;          /* 0x3C */
    u8  byte3D;
    u8  byte3E;
    u8  byte3F;
    u16 half40;
    u16 half42;
    u16 half44;
    u16 half46;
    u8  byte48;
    u8  byte49;
    u8  pad4A[6];
} MinibomberBlastSettings;

MinibomberBlastSettings l14_minibomber_bomb_release_blast_template __asm__("D_L14_001D88D0") = {
    {0},
    {0, 0, 0xff, 0, 0x19, 0x19, 0, 0xff, 0, 0x19, 0x19, 0, 0x64, 0, 0x19, 0x19},
    {2.0f, 2.0f, 2.0f, 0.01f, 0.01f},
    0x14,
    0x80000,
    0xff,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    {0}
};

extern float D_L14_0015F580_macro[] __asm__("D_L14_0015F580") MACRO_ADDR;

void emit_minibomber_bomb_release_effects(struct Moby *moby,
                                          float       *point,
                                          float        angle) __asm__("FUN_L14_002b5968");

void emit_minibomber_bomb_release_effects(struct Moby *moby, float *point, float angle) {
    float        offset[4];
    float        effect_spawn_position[4];
    float        small_offset[4];
    float        direction[4];
    float        original_pos[4];
    float        value;
    float        puff_size;
    float        step;
    L14Puff     *part;
    L14PuffTail *tail;
    int          i;
    int          j;
    int          signed_puff_random_value;
    int          color;
    int          packed_color;
    int          low_color;
    int          high_color;

    value = fast_add_rotations(angle, moby->rot.z);
    offset[0] = fast_cos(value) * 0.8f;
    offset[1] = fast_sin(value) * 0.8f;
    offset[2] = 0.0f;
    fast_vec_add(effect_spawn_position, point, offset);
    for (i = 5; i >= 0; i--) {
        signed_puff_random_value = random_integer_below(2);
        if (random_integer_below(2))
            signed_puff_random_value = -signed_puff_random_value;
        puff_size = random_float_between(40000.0f, 150000.0f);
        color = random_int_between(10, 0x7f);
        low_color = (color << 8) | 0x7f000000;
        high_color = color << 16;
        packed_color = color | (high_color | low_color);
        part = spawn_puff(effect_spawn_position,
                          0.2f,
                          1.0f,
                          1.01f,
                          signed_puff_random_value,
                          D_L14_0015F580_macro,
                          puff_size,
                          packed_color);
        if (part) {
            tail = &part->tail;
            part->life = scale_game_frames(120);
            tail->mode = 2;
            tail->tint = 0x7f;
            tail->fade = part->life;
        }
    }
    qcopy(small_offset, offset);
    fast_vec_scale(offset, offset, 0.0625f);
    offset[2] = random_float_between(0.025f, 0.06f);
    qcopy(original_pos, effect_spawn_position);
    step = 0.6283f;
    for (j = 0; j < 16; j++) {
        value = randf_sym(0.0f, 0.5f);
        build_look_at_matrix(direction, offset, small_offset, (float)j * step + value);
        effect_spawn_position[0] = original_pos[0] + randf_sym(0.0f, 0.2f);
        effect_spawn_position[1] = original_pos[1] + randf_sym(0.0f, 0.2f);
        effect_spawn_position[2] = original_pos[2] + randf_sym(0.0f, 0.2f);
        spawn_two_point_effect_particle(effect_spawn_position, direction);
    }
    mine_camera_blast((int)&l14_minibomber_bomb_release_blast_template, original_pos, 0, 0);
}

typedef struct {
    char  pad00[0xD0];
    int   pathA;
    char  padD4[0xE4 - 0xD4];
    int   t;
    int   node;
    float w;
    float off[4];
    char  pad100[0x10C - 0x100];
    int   alt;
    char  pad110[0x150 - 0x110];
    int   pathB;
} Mover_5c30;

void initialize_minibomber_path_retrace(struct Moby *moby) __asm__("FUN_L14_002b5c30");

void initialize_minibomber_path_retrace(struct Moby *moby) {
    Mover_5c30 *mover_vars = (Mover_5c30 *)moby->pvars;
    int        *active_path;
    int         penultimate_node_index;
    if (mover_vars->alt == 0)
        active_path = ((int *)l14_paths[mover_vars->pathA]);
    else
        active_path = ((int *)l14_paths[mover_vars->pathB]);
    penultimate_node_index = *active_path - 2;
    mover_vars->node = penultimate_node_index;
    mover_vars->w = *(float *)((penultimate_node_index << 4) + (char *)active_path + 0x1C);
    mover_vars->t = 0;
    clear_u64_value(mover_vars->off);
}

void hide_minibomber_and_release_voice(struct Moby *moby) __asm__("FUN_L14_002b5c98");

void hide_minibomber_and_release_voice(struct Moby *moby) {
    u8 *minibomber_vars = ((u8 *)moby->pvars);
    s32 voice_slot_index;
    moby->flags |= 0x41;
    moby->collision = 0;
    voice_slot_index = *(s32 *)(minibomber_vars + 0x108);
    if (voice_slot_index != -1) {
        u8 *voice_slot = (u8 *)&voice_pool + voice_slot_index * 0x70;
        if (*(u8 **)(voice_slot + 0x88) == moby && *(u8 *)(voice_slot + 0x74) != 0) {
            release_voice_slot(voice_slot_index);
        }
    }
    *(s32 *)(minibomber_vars + 0x108) = -1;
}

void initialize_minibomber_path_following(struct Moby *moby) __asm__("FUN_L14_002b5d18");

void initialize_minibomber_path_following(struct Moby *moby) {
    float initial_spring_value = D_L14_00161540 * 1.5f;
    char *moby_vars = (char *)moby->pvars;
    char *active_path;
    float initial_path_direction[4] __attribute__((aligned(16)));
    *(int *)(moby_vars + 0xE8) = 0;
    *(float *)(moby_vars + 0x160) = initial_spring_value;
    *(int *)(moby_vars + 0xEC) = 0;
    *(int *)(moby_vars + 0x164) = 0;
    *(int *)(moby_vars + 0x168) = 0;
    *(int *)(moby_vars + 0x16C) = 0;
    *(int *)(moby_vars + 0xC0) = 0;
    *(int *)(moby_vars + 0xC4) = 0;
    if (*(int *)(moby_vars + 0x10C) == 0)
        active_path = (char *)l14_paths[*(int *)(moby_vars + 0xD0)];
    else
        active_path = (char *)l14_paths[*(int *)(moby_vars + 0x150)];
    qcopy(&moby->pos, active_path + 0x10);
    fast_vec_sub(initial_path_direction, active_path + 0x20, &moby->pos);
    moby->rot.z = fast_arc_tan(initial_path_direction[0], initial_path_direction[1]);
    *(int *)(moby_vars + 0xE4) = 0;
    clear_u64_value(moby_vars + 0xF0);
}

void emit_minibomber_exhaust_puffs(struct Moby *moby) __asm__("FUN_L14_002b5dd8");

void emit_minibomber_exhaust_puffs(struct Moby *moby) {
    float exhaust_position_offset[4];
    float pos[4];
    int   i;
    int   j;
    int   signed_puff_value;
    int   life;
    float size;
    if (moby->state < 2 || moby->state == 9)
        return;
    fast_vec_scale(exhaust_position_offset, &moby->rot_row0, -1.25f);
    fast_vec_add(pos, &moby->pos, exhaust_position_offset);
    for (i = 0; i < 2; i++) {
        L14Puff     *part;
        L14PuffTail *tail;
        signed_puff_value = random_integer_below(0x10);
        part = spawn_puff(pos,
                          0.2f,
                          1.0f,
                          0.75f,
                          random_integer_below(2) == 0 ? signed_puff_value : -signed_puff_value,
                          D_L14_0015F580_macro,
                          160000.0f,
                          0x7F3030FF);
        if (part) {
            tail = &part->tail;
            part->life = scale_game_frames(6);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = part->life;
        }
    }
    fast_vec_scale(exhaust_position_offset, &moby->rot_row0, -1.0f);
    fast_vec_add(pos, &moby->pos, exhaust_position_offset);
    signed_puff_value = 0x10;
    life = scale_game_frames(2);
    size = 100000.0f;
    for (j = 0; j < 3; j++) {
        L14Puff     *part;
        L14PuffTail *tail;
        part = spawn_puff(
            pos, 0.05f, 1.0f, 1.0f, signed_puff_value, D_L14_0015F580_macro, size, 0x7FFFFFFF);
        if (part) {
            tail = &part->tail;
            part->life = life;
            part->shade = random_integer_below(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = part->life;
        }
        signed_puff_value = -signed_puff_value;
        life *= 2;
        size -= 20000.0f;
    }
}

extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

int minibomber_aim_return_duration_frames __asm__("D_L14_00161560") = 0xb4;

int minibomber_bomb_flight_duration_frames __asm__("D_L14_00161564") = 0x2d;
