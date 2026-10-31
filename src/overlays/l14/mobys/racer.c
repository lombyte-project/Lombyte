/* Level 14 racer moby class. */
#include "types.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/rendering/clip_volume.h"
#include "sda.h"
#include "rnc/overlay/l14/mobys/racer_globals.h"

extern void FUN_0020c828(void *);

extern void fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");

void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

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

extern int scale_game_frames(int);

struct L14RacerVars {
    u8   pad0[0x40];
    Vec4 trail; /* 0x40: effect state (spawn_explosion) */
    u8   pad50[0x10];
    s32  path; /* 0x60: index into l14_splines (-1: none) */
    u8   pad64[8];
    s32  timer;    /* 0x6C */
    f32  inv_time; /* 0x70: 1 / timer at the start of the run */
    u8   pad74[0xC];
    s32  delay;       /* 0x80: frames to wait once the race starts */
    s32  end_point;   /* 0x84: D_L14_001B0BB0 entry the run ends at */
    s32  start_point; /* 0x88: D_L14_001B0BB0 entry the run starts from (-1: none) */
    u8   pad8C[0x64];
    s32  unkF0;
    s32  unkF4;
    u8   padF8[8];
    u8   manips[3][0x40]; /* 0x100: attached as manipulators 2..4 */
    u8   pad1C0[0xC];
    s32  volume; /* 0x1CC: clip volume where the racer leaves (-1: none) */
};

struct L14RaceState {
    u8  pad0[0x86];
    s16 phase; /* 0x86 */
};

extern LevelSpline *l14_splines_gp __asm__("D_L14_0015F70C") __attribute__((sda));

extern struct L14RaceState *race_state __asm__("D_L14_00167500") NOT_SDA;

extern s32 level14_racer_start_interpolation_duration_frames_gp __asm__("D_L14_00161A2C")
    __attribute__((sda));

#include "rnc/overlay/l14/mobys/racer_data/level14_racer_update.h"

extern void attach_manipulator(void *arg0, s32 arg1, void *arg2) __asm__("FUN_0020cb10");

extern void set_moby_animation(struct Moby *, s32, s32) __asm__("FUN_00212ed8");

extern f32 compute_interpolated_record_value(struct Moby *) __asm__("FUN_0020c9e0");

extern unsigned char *spawn_plasmabot_death_debris(char *owner,
                                                   char *pos) __asm__("FUN_L14_002dea98");

extern void spawn_path_or_ballistic_debris(struct Moby *, s32 *, int) __asm__("FUN_L14_002e0170");

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

void initialize_racer_path_tracking(struct Moby *m) __asm__("FUN_L14_002d6358");

int interpolate_racer_to_path_start(struct Moby *m) __asm__("FUN_L14_002d64d0");

void update_racer_path_and_weapon(struct Moby *m) __asm__("FUN_L14_002d67e8");

void advance_moby_along_path(struct Moby *m) __asm__("FUN_L14_002d7198");

void update_moby_joint_visual_effects(struct Moby *m) __asm__("FUN_L14_002d7490");

void level14_racer_update(struct Moby *racer_moby) __asm__("FUN_L14_002d5f40");

void level14_racer_update(struct Moby *m) __asm__("FUN_L14_002d5f40");

/* Moby class 211 */
void level14_racer_update(struct Moby *racer_moby) {
    enum {
        RACER_STATE_INITIALIZE = 0,
        RACER_STATE_WAIT_FOR_RACE_START = 1,
        RACER_STATE_WAIT_FOR_START_DELAY = 2,
        RACER_STATE_WAIT_FOR_START_ANIMATION = 3,
        RACER_STATE_COMPLETE_START_INTERPOLATION = 4,
        RACER_STATE_FOLLOW_PATH = 5,
        RACER_STATE_WAIT_FOR_EXIT_VOLUME = 6
    };
    struct L14RacerVars *racer_vars = (struct L14RacerVars *)racer_moby->pvars;
    u8                   pos[16];
    f32                  spawned_object_planar_velocity[3];
    unsigned char       *spark;
    f32                 *sv;

    switch (racer_moby->state) {
    case RACER_STATE_INITIALIZE:
        if (racer_vars->path == -1 || l14_splines_gp[racer_vars->path].path->count == 0 ||
            racer_vars->start_point == -1 || *((int *)l14_paths[racer_vars->start_point]) != 3 ||
            racer_vars->volume == -1) {
            FUN_0020c828(racer_moby);
            return;
        }
        initialize_racer_path_tracking(racer_moby);
        racer_moby->state = 1;
        racer_moby->flags = (racer_moby->flags & 0xEFFF) | 0x41;
        racer_moby->collision = 0;
        racer_vars->unkF4 = -1;
        racer_vars->unkF0 = -1;
        fill_transfer_words(racer_vars->manips[0], 0, 0x40);
        attach_manipulator(racer_moby, 2, racer_vars->manips[0]);
        fill_transfer_words(racer_vars->manips[1], 0, 0x40);
        attach_manipulator(racer_moby, 3, racer_vars->manips[1]);
        fill_transfer_words(racer_vars->manips[2], 0, 0x40);
        attach_manipulator(racer_moby, 4, racer_vars->manips[2]);
        return;
    case RACER_STATE_WAIT_FOR_RACE_START:
        if (race_state->phase == 0x14) {
            racer_vars->timer = scale_game_frames(racer_vars->delay);
            racer_moby->state = 2;
        }
        return;
    case RACER_STATE_WAIT_FOR_START_DELAY:
        if (!fast_dec_timer(&racer_vars->timer))
            return;
        racer_moby->flags = (racer_moby->flags | 0x1000) & 0xFFBE;
        racer_moby->collision = racer_moby->pclass->collision;
        racer_moby->state = 3;
        set_moby_animation(racer_moby, 2, 0);
        racer_moby->anim_speed = 0.5f;
        racer_vars->timer = scale_game_frames(level14_racer_start_interpolation_duration_frames_gp);
        racer_vars->inv_time = 1.0f / convert_integer_to_float(racer_vars->timer);
        qcopy(&racer_moby->pos, (char *)l14_paths[racer_vars->end_point] + 0x10);
    case RACER_STATE_WAIT_FOR_START_ANIMATION:
        if (compute_interpolated_record_value(racer_moby) >= 3.0f)
            interpolate_racer_to_path_start(racer_moby);
        if ((racer_moby->anim_flags & 2) && racer_moby->seq == racer_moby->prev_seq) {
            racer_moby->state = 4;
            blend_moby_animation((MobyAnim *)racer_moby, 0, 0, 10);
            racer_moby->anim_speed = 1.0f;
        }
        break;
    case RACER_STATE_COMPLETE_START_INTERPOLATION:
        if (interpolate_racer_to_path_start(racer_moby)) {
            racer_moby->state = 5;
            racer_vars->timer =
                scale_game_frames(level14_racer_path_follow_initial_timer_frames_gp);
        }
        break;
    case RACER_STATE_FOLLOW_PATH:
        advance_moby_along_path(racer_moby);
        break;
    case RACER_STATE_WAIT_FOR_EXIT_VOLUME:
        update_racer_path_and_weapon(racer_moby);
        if (is_point_inside_clip_volume(&racer_moby->pos, racer_vars->volume)) {
            moby_point_position(racer_moby, 2, pos);
            spark = spawn_plasmabot_death_debris((char *)racer_moby, (char *)pos);
            if (spark) {
                sv = *(f32 **)(spark + 0x78);
                spawned_object_planar_velocity[0] =
                    fast_cos(racer_moby->rot.z) * (frame_scale * 0.4f);
                spawned_object_planar_velocity[1] =
                    fast_sin(racer_moby->rot.z) * (frame_scale * 0.4f);
                spawned_object_planar_velocity[2] = 0.0f;
                sv[0] = spawned_object_planar_velocity[0];
                sv[1] = spawned_object_planar_velocity[1];
            }
            spawn_path_or_ballistic_debris(
                racer_moby, &level14_racer_exit_debris_path_initialization_scale_gp, 0);
            spawn_explosion(racer_moby,
                            &racer_vars->trail,
                            &racer_moby->pos,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            4.0f,
                            2.0f,
                            9.0f,
                            1,
                            1.0f,
                            15.0f,
                            1,
                            5,
                            -1,
                            0);
            FUN_0020c828(racer_moby);
            return;
        }
        break;
    }
    update_moby_joint_visual_effects(racer_moby);
}
