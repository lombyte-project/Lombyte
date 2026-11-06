/* Level 06 moby class 0x41b: alien queen. */
#include "rnc/input/pad_globals.h"
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "rnc/overlay/l06/mobys/alien_queen.h"
#include "rnc/overlay/l06/mobys/alien_queen_slider.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/math/interpolation/advance_accelerated_scalar.h"
#include "rnc/math/random.h"
#include "rnc/overlay/l06/hero/hero_set_state.h"
#include "rnc/overlay/shared/camera.h"

enum {
    ALIEN_QUEEN_STATE_INITIALIZE = 0,
    ALIEN_QUEEN_STATE_WAIT_TO_START_INTRO = 1,
    ALIEN_QUEEN_STATE_INTRO = 2,
    ALIEN_QUEEN_STATE_FIRST_RECOVERY_CAMERA = 3,
    ALIEN_QUEEN_STATE_FROG_RECOVERY_ORBIT = 4,
    ALIEN_QUEEN_STATE_SECOND_RECOVERY_CAMERA = 5,
    ALIEN_QUEEN_STATE_GROUP_RECOVERY_ORBIT = 6,
    ALIEN_QUEEN_STATE_WAIT_TO_APPROACH_TARGET = 7,
    ALIEN_QUEEN_STATE_APPROACH_TARGET = 8,
    ALIEN_QUEEN_STATE_ATTACK_WINDUP = 9,
    ALIEN_QUEEN_STATE_ATTACK_HIT_QUERIES = 10,
    ALIEN_QUEEN_STATE_WAIT_FOR_ORBIT_ANIMATION_END = 11,
    ALIEN_QUEEN_STATE_TIMED_ORBIT = 12,
    ALIEN_QUEEN_STATE_RETURN_HOME = 13,
    ALIEN_QUEEN_STATE_DEATH = 14,
    ALIEN_QUEEN_STATE_POST_DEATH = 15
};

enum {
    HIT_STATUS_SKIP_DAMAGE_AND_REACTION = 1
};

extern int D_L06_001ABFC0[];
extern int D_L06_0015FFD8_d __asm__("D_L06_0015FFD8");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F6790), where it is exact; names translated to the US level program. */
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
void        set_moby_and_links_to_states_4_and_5(char *moby) __asm__("FUN_L06_002f5360");

/* Switches a moby from state 6 to 8 and sets its ten children to state 9. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F6818), where it is exact; names translated to the US level program. */
void set_moby_and_links_to_states_8_and_9(char *moby) __asm__("FUN_L06_002f53e8");

/* angle from a moby to its owner, clamped to the gaps between obstacles in its list */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FAB28), where it is exact; names translated to the US level program. */
extern char *D_L06_0015FFD8;

/* 0x002f9948, 100 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
void set_moby_list_state_18(int moby_list_index) __asm__("FUN_L06_002f9948");

/* 0x002f99b0, 120 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Resets every moby of a list that is in state 0x12.
void clear_moby_list_state_18(int index) __asm__("FUN_L06_002f99b0");

#include "qcopy.h"

/* raise each listed moby's position z by a fixed amount and notify it */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC4C0), where it is exact; names translated to the US level program. */
extern float  random_float_between(float, float) __asm__("FUN_002132a8");
int           activate_state_15_moby_at_position(void *m_,
                                                 void *activation_position) __asm__("FUN_L06_002e9d10");
int           activate_state_15_moby_at_position(void *m_, void *pos) __asm__("FUN_L06_002e9d10");
extern short *D_L06_001ABFC0_c[] __asm__("D_L06_001ABFC0");
void activate_listed_state_15_mobys_at_raised_positions(char *moby) __asm__("FUN_L06_002fb090");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC6D8), where it is exact; names translated to the US level program. */
extern char D_L06_00167540[] NOT_SDA;
int         update_alien_queen_intro_camera(char *moby) __asm__("FUN_L06_002fb2a8");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC8E0), where it is exact; names translated to the US level program. */
extern int *D_L06_001B0C30[];
void        finish_alien_queen_intro(char *moby) __asm__("FUN_L06_002fb4b0");

/* Steers a moby's camera-like orientation toward a target point, returns 1 when done. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FCA98), where it is exact; names translated to the US level program. */
extern char D_L06_00167540[];
extern void FUN_L06_00312a78(void *);
extern void FUN_L06_00312ad0(void *);
int         update_alien_queen_first_recovery_camera(char *moby) __asm__("FUN_L06_002fb668");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002B5990), where it is exact; names translated to the US level program. */
#include "rnc/overlay/shared/world/cuboid.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002EB140), where it is exact; names translated to the US level program. */
extern void
func_L00_0025D5B0_2eb140(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");

/* Returns 0 while any listed class-0x33B moby is in a state other than 0xFE, 0xFD or 0x10; else 1. */
int check_all_listed_frog_states_fe_fd_or_10(int index) __asm__("FUN_L06_002e9ea0");

/* Sparks thrown off the swinging hazard while the camera is near it. */
extern f32          random_float_between(f32, f32) __asm__("FUN_002132a8");
extern void         set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern float        D_L06_0015F3FC;
extern float        approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern unsigned int fast_tween_color(unsigned int, unsigned int, float) __asm__("FUN_001fa6e0");
extern void
resolve_point_path_segment_xy_clearance(f32, s32, void *, void *) __asm__("FUN_L00_00261d78");
extern float
approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");

extern int resolve_moby_hit_response() __asm__("FUN_00213928");
void       start_hit_color_flash(void *a, void *s) __asm__("FUN_L00_0025d458");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void start_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d458");
extern void update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");

/* Alien queen update: wakes, rises, circles and chases the hero, attacks with probes and dies through its states. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FAE58), where it is exact; names translated to the US level program. */
typedef struct {
    float dir[4];
    char *owner;
    int   flags;
    int   pad_18;
    float scale;
    int   count;
} QueenProbe;

extern char  l06_post_death_scan_records[] __asm__("D_L06_001678D0");
extern int   l06_post_death_scan_record_gates[] __asm__("D_L06_0016A010");
extern short D_L06_00161EF0 __attribute__((sda));
extern short l06_alien_queen_movement_speed __asm__("D_L06_00161EF4") __attribute__((sda));
f32          alien_queen_movement_acceleration_gp __asm__("D_L06_00161EFC") = 8.0f;
extern short l06_alien_queen_attack_distance __asm__("D_L06_00161F00") __attribute__((sda));
extern short l06_alien_queen_retreat_distance __asm__("D_L06_00161F04") __attribute__((sda));
extern short l06_alien_queen_attack_heading_tolerance_degrees __asm__("D_L06_00161F08")
    __attribute__((sda));
extern void FUN_L06_002fa9c0_c(void *) __asm__("FUN_L06_002fa9c0");
void        set_moby_list_state_18(int idx) __asm__("FUN_L06_002f9948");
void        FUN_L06_002fb870(char *arg);
void        FUN_L06_002fbc30(char *arg);
int         FUN_L06_002fba28(char *moby);
extern void func_L00_00264B40_q(void *, int, float, void *) __asm__("FUN_L00_00263ac8");
extern void func_L00_0025B178_q(void *) __asm__("FUN_L00_0025a120");
extern int
queue_animation_update(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern void  func_L00_00236710_q(void) __asm__("FUN_L00_00235d80");
extern void  func_L00_00236830_q(void) __asm__("FUN_L00_00235ea0");
extern char  func_L06_0024A080_q[] __asm__("FUN_L06_00249738");
extern void  set_moby_list_state_18(int) __asm__("FUN_L06_002f9948");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void  func_L06_002F6818_q(void *) __asm__("FUN_L06_002f53e8");
extern void  func_L06_002FC578_q(void *) __asm__("FUN_L06_002fb148");
extern int   func_L06_002FC6D8_q(void *) __asm__("FUN_L06_002fb2a8");
extern void  func_L06_002FC8E0_q(void *) __asm__("FUN_L06_002fb4b0");
extern void  func_L06_002FC4C0_q(void *) __asm__("FUN_L06_002fb090");
extern int   func_L06_002FCA98_q(void *) __asm__("FUN_L06_002fb668");
extern void  clear_moby_list_state_18(int) __asm__("FUN_L06_002f99b0");
extern void  func_L06_002FD0B8_q(void *) __asm__("FUN_L06_002fbc88");
extern int   check_all_listed_frog_states_fe_fd_or_10(int) __asm__("FUN_L06_002e9ea0");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void  func_L00_00262DF0_q(int, void *, float, void *) __asm__("FUN_L00_00261d78");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern void  func_L00_00250800_q(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int   func_L00_001F2BE8_q(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void  func_L06_002F6790_q(void *) __asm__("FUN_L06_002f5360");
extern void  func_L00_00260460_q(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern char  D_L06_00167540_q[] __asm__("D_L06_00167540");
extern int  *D_L06_001B0C30_q[] __asm__("D_L06_001B0C30");
extern char *level_mobys_q __asm__("D_L06_0015FFD8");
extern int   level_game_mode_q __asm__("D_L06_0015F5C4");
extern unsigned char        menu_item_enabled_19_lui[] __asm__("D_0013D3A7");
extern unsigned char        D_0013D4FD_q[] __asm__("D_0013D4FD");
extern unsigned char        D_0013D407_q[] __asm__("D_0013D407");
void                        alien_queen_update(unsigned char *moby) __asm__("FUN_L06_002f9a28");
extern char                *FUN_L00_0025a420(void *, int, int);
extern void                 FUN_L00_00257470(void *, int, int);
extern void                 FUN_00212f90(void *, int, int, int);
extern float                scale_recovery_time(float) __asm__("FUN_L06_00216fa8");
extern int                  FUN_L00_0025ff38(void *, void *, int, int, void *, int, float);
extern void                 FUN_L06_002fb148_recovery_high(char *) __asm__("FUN_L06_002fb508");
extern void                 FUN_L06_002fb148_recovery_low(char *) __asm__("FUN_L06_002fb8c8");
extern short D_L06_0014162A NOT_SDA;
extern int D_L06_00140408   NOT_SDA;
extern int D_L06_001413D0   NOT_SDA;

#include "rnc/overlay/l06/mobys/alien_queen_data/update_alien_queen_hit_response_and_recovery.h"

int l06_alien_queen_glow_tween_start_color __asm__("D_L06_00161F10") = 0x80808080;

int l06_alien_queen_glow_tween_end_color __asm__("D_L06_00161F14") = 0x80804080;

/* Updates the object's health, hit response, recovery timer and path motion. */
void update_alien_queen_hit_response_and_recovery(char *moby) __asm__("FUN_L06_002fa9c0");

#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC578), where it is exact; names translated to the US level program. */
extern int *D_L06_001B0FB0[] __asm__("D_L06_001B0C30");
void        start_alien_queen_intro(char *moby) __asm__("FUN_L06_002fb148");

/* Pvars of a moby that circles a centre point, keeping to the side away from the hero. */
typedef struct {
    u8           pad0[0xB0];
    struct Moby *focus; /* 0xB0: moby it keeps facing */
    u8           padB4[0x1C];
    Vec3         center;  /* 0xD0 */
    f32          radius;  /* 0xDC */
    f32          yaw_vel; /* 0xE0 */
    u8           padE4[4];
    f32          speed;  /* 0xE8 */
    s32          ground; /* 0xEC */
} CirclerVars;

f32 alien_queen_orbit_speed_gp __asm__("D_L06_00161EF8") = 5.0f;
extern f32
approach_angle(f32 *angle, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025be00");

/* Circles its centre point on the side away from the hero (at most 45 degrees per step), faces its focus moby, moves while the hero is within 16 units and snaps to the ground. */
void update_alien_queen_orbit_movement(struct Moby *moby) __asm__("FUN_L06_002fbc88");

int activate_state_15_moby_at_position(void *m_, void *activation_position) {
    char *moby = (char *)m_;
    char *moby_data = ((char *)MOBY(moby)->pvars);
    float doubled_frame_time, scaled_frame_time_squared;
    if (((unsigned char *)moby)[0x20] == 0xF) {
        qcopy(moby + 0x10, activation_position);
        MOBY(moby)->collision = MOBY(moby)->pclass->collision;
        MOBY(moby)->flags = (MOBY(moby)->flags & 0xFFBE) | 0x1000;
        moby[0x20] = 6;
        clear_u64_value(moby + 0x40);
        MOBY(moby)->rot.z = random_angle_radians();
        euler_rotation_basis(moby + 0xC0, moby + 0x40);
        scaled_frame_time_squared = frame_time_sq * 20.0f;
        doubled_frame_time = frame_time + frame_time;
        *(float *)(moby_data + 0x130) = scaled_frame_time_squared;
        *(int *)(moby_data + 0x134) = 0;
        *(float *)(moby_data + 0x138) = doubled_frame_time;
        *(float *)(moby_data + 0x13C) = doubled_frame_time;
        moby_data[0x15D] = 3;
        *(int *)(moby_data + 0x144) = 5;
        func_L00_0025D5B0_2eb140(moby, moby_data + 0x120, 2, 1, 0, random_angle_radians());
        *(float *)(moby_data + 0x128) = -*(float *)(moby_data + 0x128);
        return 1;
    }
    return 0;
}

int check_all_listed_frog_states_fe_fd_or_10(int index) {
    unsigned short *moby_index_cursor = (unsigned short *)D_L06_001ABFC0[index];
    char           *moby_array_base;
    char           *moby;
    if (moby_index_cursor == 0)
        return 1;
    moby_array_base = (char *)D_L06_0015FFD8_d;
    for (;;) {
        moby = moby_array_base + ((*moby_index_cursor & 0x7FFF) << 8);
        if (moby != 0 && MOBY(moby)->oclass == 0x33B) {
            unsigned char moby_state = moby[0x20];
            if (moby_state != 0xFE) {
                if (moby_state != 0xFD) {
                    if (moby_state != 0x10)
                        return 0;
                }
            }
        }
        if ((short)*moby_index_cursor++ < 0)
            return 1;
    }
}

void set_moby_and_links_to_states_4_and_5(char *moby) {
    char  *source_moby_vars = ((char *)MOBY(moby)->pvars);
    char **linked_moby_cursor;
    int    i;
    moby[0xBC] = 1;
    if ((unsigned char)moby[0x20] == 2) {
        allocate_voice_for_target_entry(0, 0, (int)moby);
        moby[0x20] = 4;
        linked_moby_cursor = (char **)(source_moby_vars + 0x24);
        *(int *)(source_moby_vars + 0x14) = 0;
        *(int *)(source_moby_vars + 0x18) = 0;
        for (i = 6; i >= 0; i--) {
            (*linked_moby_cursor++)[0x20] = 5;
        }
    }
}

void set_moby_and_links_to_states_8_and_9(char *moby) {
    char  *parent_moby_vars = ((char *)MOBY(moby)->pvars);
    int    i;
    char **linked_moby_cursor;
    moby[0xBC] = 1;
    if ((unsigned char)moby[0x20] == 6) {
        unsigned short fl;
        allocate_voice_for_target_entry(0, 0, (int)moby);
        fl = MOBY(moby)->flags;
        moby[0x31] = 1;
        fl &= 0xFFFE;
        moby[0x20] = 8;
        MOBY(moby)->flags = fl;
        *(int *)(parent_moby_vars + 0x18) = 0;
        linked_moby_cursor = (char **)(parent_moby_vars + 0x24);
        for (i = 6; i >= 0; i--) {
            char *linked_moby = *linked_moby_cursor;
            linked_moby[0x20] = 9;
            *(unsigned short *)(*linked_moby_cursor + 0x34) &= 0xFFFE;
            (*linked_moby_cursor)[0x31] = 1;
            linked_moby_cursor++;
        }
    }
}

void set_moby_list_state_18(int moby_list_index) {
    short *moby_index_cursor = D_L06_001ABFC0[moby_list_index];
    if (moby_index_cursor != 0) {
        do {
            char          *moby = D_L06_0015FFD8_d + ((*moby_index_cursor & 0x7FFF) << 8);
            unsigned short flags = MOBY(moby)->flags;
            moby[0x20] = 0x12;
            flags |= 1;
            MOBY(moby)->collision = 0;
            flags &= 0xEFFF;
            MOBY(moby)->flags = flags;
        } while (*moby_index_cursor++ >= 0);
    }
}

void clear_moby_list_state_18(int index) {
    short *moby_index_cursor = D_L06_001ABFC0[index];
    if (moby_index_cursor != 0) {
        do {
            char *moby = (char *)((*(unsigned short *)moby_index_cursor & 0x7FFF) << 8) +
                         (int)D_L06_0015FFD8_d;
            if ((unsigned char)moby[0x20] == 0x12) {
                unsigned short flags = MOBY(moby)->flags;
                moby[0x20] = 0;
                flags &= 0xFFFE;
                flags |= 0x1000;
                MOBY(moby)->collision = MOBY(moby)->pclass->collision;
                MOBY(moby)->flags = flags;
            }
        } while (*moby_index_cursor++ >= 0);
    }
}

typedef struct AlienQueenVars {
    u8                   pad_0[0x20];
    f32                  health;
    s16                  max_health;
    u8                   pad_26[0x2];
    u8                   hit_response_class;
    u8                   unk29;
    u8                   pad_2A[0x4];
    u8                   death_control_flags;
    u8                   pad_2F[0x19];
    f32                  z_velocity;
    u8                   pad_4C[0xC];
    u8                   unk58;
    u8                   pad_59[0x1];
    u8                   unk5A;
    u8                   pad_5B[0x5];
    struct HitColorFlash hit_flash;
    u8                   goal; /* target record (FUN_L00_0025ff38): target at +0x40 */
    u8                   pad_71[0x3F];
    char                *target;
    s32                  target_selection_status;
    u8                   pad_B8[0x8];
    f32                  home_position[4];
    f32                  path_midpoint[4];
    u8                   turn_velocity;
    u8                   pad_E1[0x7];
    f32                  move_speed;
    s32                  path;
    s32                  frog_list;
    s32                  state18_list;
    s32                  cuboid;
    s32                  unkFC;
    s32                  unk100;
    s32                  slider_list;
    s32                  unk108;
    s32                  animation_update;
    s32                  health_sixteenths;
    s32                  saved_path_point_count;
    f32                  camera_blend_fraction;
    u8                   camera_blend_speed;
    u8                   pad_11D[0x3];
    s32                  timer_120;
    u8                   pad_124[0x4];
    f32                  glow_phase;
    s32                  timer_12C;
    u8                   big_head_manipulator;
    u8                   pad_131[0x3F];
    s32                  intro_or_recovery_delay_active;
    f32                  recovery_health;
    s32                  suppress_death_dialogue;
} AlienQueenVars;

/* Moby class 1051 */
void alien_queen_update(unsigned char *moby) {
    AlienQueenVars *alien_queen_update_vars = (AlienQueenVars *)MOBY(moby)->pvars;
    char           *target;
    float           step[4];

    FUN_L06_002fa9c0_c(moby);
    target = alien_queen_update_vars->target;
    func_L00_00264B40_q(
        moby, *(int *)&D_L06_00161EF0, 1.9f, &alien_queen_update_vars->big_head_manipulator);
    if ((unsigned char)moby[0x31] != 0 && fast_vec_dist(moby + 0x10, D_L06_00167540_q) < 45.0f) {
        func_L00_0025B178_q(moby);
        moby[0x7F] = 0x24;
    }
    if ((unsigned char)moby[0x20] >= 3 && (unsigned char)moby[0x20] <= 13) {
        alien_queen_update_vars->animation_update =
            queue_animation_update(0x16,
                                   0xFFFF,
                                   func_L00_00236710_q,
                                   func_L00_00236830_q,
                                   func_L06_0024A080_q,
                                   &alien_queen_update_vars->health_sixteenths,
                                   0x1EF);
    }
    switch ((unsigned char)moby[0x20]) {
    case ALIEN_QUEEN_STATE_INITIALIZE: {
        int *path;
        alien_queen_update_vars->health = 30.0f;
        alien_queen_update_vars->max_health = 0x1E;
        alien_queen_update_vars->unk29 = 1;
        alien_queen_update_vars->hit_response_class = 3;
        alien_queen_update_vars->death_control_flags = 1;
        alien_queen_update_vars->unk58 = truncate_float_to_s32(14.0f);
        alien_queen_update_vars->unk5A = truncate_float_to_s32(18.4f);
        alien_queen_update_vars->timer_120 = scale_game_frames(0x4B0);
        alien_queen_update_vars->suppress_death_dialogue = 0;
        qcopy(alien_queen_update_vars->home_position, moby + 0x10);
        alien_queen_update_vars->animation_update = -1;
        alien_queen_update_vars->saved_path_point_count =
            *D_L06_001B0C30_q[alien_queen_update_vars->path];
        path = D_L06_001B0C30_q[alien_queen_update_vars->path];
        lerp_vector(alien_queen_update_vars->path_midpoint,
                    path + 4,
                    (char *)path + ((*path / 2) * 16 + 16),
                    0.5f);
        alien_queen_update_vars->path_midpoint[3] = vector_distance_xy(
            alien_queen_update_vars->home_position, alien_queen_update_vars->path_midpoint);
        alien_queen_update_vars->glow_phase = random_angle_radians();
        alien_queen_update_vars->timer_12C = scale_game_frames(0x1E);
        MOBY(moby)->collision = 0;
        MOBY(moby)->flags = (MOBY(moby)->flags | 0x4001) & 0xEFFF;
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + 17.5f;
        set_moby_list_state_18(alien_queen_update_vars->state18_list);
        if (menu_item_enabled_19_lui[4] != 0 && D_0013D4FD_q[8] == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby[0x20] = 1;
        break;
    }
    case ALIEN_QUEEN_STATE_WAIT_TO_START_INTRO:
        if (alien_queen_update_vars->target_selection_status == 2)
            return;
        if (*(short *)(target + 0xA6) != 0)
            return;
        if (alien_queen_update_vars->intro_or_recovery_delay_active == 0) {
            alien_queen_update_vars->intro_or_recovery_delay_active = 1;
            alien_queen_update_vars->timer_12C = scale_game_frames(10);
            break;
        }
        if (alien_queen_update_vars->timer_12C != 0)
            return;
        func_L06_002F6818_q(level_mobys_q + (alien_queen_update_vars->unkFC << 8));
        func_L06_002F6818_q(level_mobys_q + (alien_queen_update_vars->unk100 << 8));
        set_listed_alien_queen_sliders_state_2_and_data_28_to_one(
            alien_queen_update_vars->slider_list);
        alien_queen_update_vars->intro_or_recovery_delay_active = 0;
        moby[0x20] = 2;
        MOBY(moby)->collision = MOBY(moby)->pclass->collision;
        MOBY(moby)->flags = (MOBY(moby)->flags | 0x1000) & 0xFFFE;
        if ((unsigned char)moby[0x53] != 8) {
            blend_moby_animation(moby, 8, 0, scale_game_frames(1));
        }
        func_L06_002FC578_q(moby);
        return;
    case ALIEN_QUEEN_STATE_INTRO: {
        float gravity = frame_time_sq * 30.0f;
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + alien_queen_update_vars->z_velocity;
        alien_queen_update_vars->z_velocity = alien_queen_update_vars->z_velocity - gravity;
        if ((unsigned char)moby[0x53] == 8) {
            if (absolute_float((MOBY(moby)->pos.z - alien_queen_update_vars->home_position[2]) /
                               alien_queen_update_vars->z_velocity) <= 6.0f &&
                (unsigned char)moby[0x53] != 9) {
                blend_moby_animation(moby, 9, 0, scale_game_frames(5));
            }
        } else {
            if (MOBY(moby)->pos.z < alien_queen_update_vars->home_position[2]) {
                MOBY(moby)->pos.z = alien_queen_update_vars->home_position[2];
            }
            if (MOBY(moby)->anim_time == 0.0f && (moby[0x70] & 2)) {
                if ((unsigned char)moby[0x53] == 9) {
                    blend_moby_animation(moby, 6, 0, scale_game_frames(0x1E));
                } else if ((unsigned char)moby[0x53] != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(0x1E));
                }
            }
        }
        if (func_L06_002FC6D8_q(moby) != 0) {
            moby[0x20] = 7;
            func_L06_002FC8E0_q(moby);
        }
        break;
    }
    case ALIEN_QUEEN_STATE_FIRST_RECOVERY_CAMERA:
        if ((unsigned char)moby[0x53] == 6 && MOBY(moby)->anim_time == 0.0f && (moby[0x70] & 2)) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
            func_L06_002FC4C0_q(moby);
        }
        if (func_L06_002FCA98_q(moby) != 0) {
            moby[0x20] = 4;
            FUN_L06_002fb870(moby);
            return;
        }
        break;
    case ALIEN_QUEEN_STATE_SECOND_RECOVERY_CAMERA:
        if ((unsigned char)moby[0x53] == 6 && MOBY(moby)->anim_time == 0.0f && (moby[0x70] & 2)) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
            clear_moby_list_state_18(alien_queen_update_vars->state18_list);
        }
        if (FUN_L06_002fba28(moby) != 0) {
            moby[0x20] = 6;
            FUN_L06_002fbc30(moby);
        }
        break;
    case ALIEN_QUEEN_STATE_FROG_RECOVERY_ORBIT:
        func_L06_002FC4C0_q(moby);
    case ALIEN_QUEEN_STATE_GROUP_RECOVERY_ORBIT:
        func_L06_002FD0B8_q(moby);
        if (((unsigned char)moby[0x20] == 4 &&
             check_all_listed_frog_states_fe_fd_or_10(alien_queen_update_vars->frog_list) != 0) ||
            ((unsigned char)moby[0x20] == 6 &&
             count_active_moby_group_members(alien_queen_update_vars->state18_list, -1) == 0)) {
            moby[0x20] = 8;
            alien_queen_update_vars->timer_120 = scale_game_frames(0x4B0);
            if ((unsigned char)moby[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
            }
        }
        return;
    case ALIEN_QUEEN_STATE_WAIT_TO_APPROACH_TARGET:
        if (alien_queen_update_vars->target_selection_status == 2)
            break;
        moby[0x20] = 8;
        if ((unsigned char)moby[0x53] == 3)
            break;
        blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
        return;
    case ALIEN_QUEEN_STATE_APPROACH_TARGET: {
        float dist = vector_distance_xy(moby + 0x10, target + 0x10);
        float turn = fast_difference_between_rotations(
            MOBY(moby)->rot.z,
            fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                         *(float *)(target + 0x14) - MOBY(moby)->pos.y));
        approach_angle((float *)&MOBY(moby)->rot.z,
                       fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                                    *(float *)(target + 0x14) - MOBY(moby)->pos.y),
                       (float *)&alien_queen_update_vars->turn_velocity,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (turn < 0.7853982f && *(float *)&l06_alien_queen_attack_distance < dist) {
            approach_value(&alien_queen_update_vars->move_speed,
                           *(float *)&l06_alien_queen_movement_speed * frame_time,
                           alien_queen_movement_acceleration_gp * frame_time_sq);
        } else if (dist < *(float *)&l06_alien_queen_retreat_distance) {
            approach_value(&alien_queen_update_vars->move_speed,
                           -(*(float *)&l06_alien_queen_movement_speed * frame_time),
                           alien_queen_movement_acceleration_gp * frame_time_sq);
        } else {
            approach_value(&alien_queen_update_vars->move_speed, 0.0f, frame_time_sq * 8.0f);
        }
        fast_vec_sub(step, target + 0x10, moby + 0x10);
        step[2] = 0.0f;
        fast_vec_normalize(step, step, alien_queen_update_vars->move_speed);
        fast_vec_add(moby + 0x10, moby + 0x10, step);
        func_L00_00262DF0_q(alien_queen_update_vars->path, moby + 0x10, 1.0f, moby + 0x10);
        if (alien_queen_update_vars->target_selection_status == 2) {
            moby[0x20] = 0xD;
            break;
        }
        if (fast_dec_timer(&alien_queen_update_vars->timer_120) != 0) {
            alien_queen_update_vars->timer_120 = scale_game_frames(0x168);
            moby[0x20] = 0xC;
            break;
        }
        if (dist < *(float *)&l06_alien_queen_attack_distance &&
            turn < *(float *)&l06_alien_queen_attack_heading_tolerance_degrees * 0.017453292f) {
            moby[0x20] = 9;
            moby[0xBC] = 7;
            if ((unsigned char)moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(0x14));
                return;
            }
        }
        break;
    }
    case ALIEN_QUEEN_STATE_TIMED_ORBIT:
        func_L06_002FD0B8_q(moby);
        if (fast_dec_timer(&alien_queen_update_vars->timer_120) != 0) {
            alien_queen_update_vars->timer_120 = scale_game_frames(0x4B0);
            moby[0x20] = 0xB;
            moby[0xBC] = 7;
            if ((unsigned char)moby[0x53] != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(0x14));
            }
        }
        return;
    case ALIEN_QUEEN_STATE_ATTACK_WINDUP:
        fast_dec_timer(&alien_queen_update_vars->timer_120);
        approach_angle((float *)&MOBY(moby)->rot.z,
                       MOBY(moby)->rot.z,
                       (float *)&alien_queen_update_vars->turn_velocity,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (moby[0x70] & 2) {
            moby[0x20] = 0xA;
            if ((unsigned char)moby[0x53] != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(0x14));
                return;
            }
        }
        break;
    case ALIEN_QUEEN_STATE_ATTACK_HIT_QUERIES: {
        QueenProbe probe;
        float      hit_query_position[4];
        fast_dec_timer(&alien_queen_update_vars->timer_120);
        if (1.0f < compute_interpolated_record_value(moby) &&
            compute_interpolated_record_value(moby) < 8.0f) {
            step[0] = fast_cos(MOBY(moby)->rot.z) * 0.45f;
            step[1] = fast_sin(MOBY(moby)->rot.z) * 0.45f;
            step[2] = 0.0f;
            probe.owner = (char *)moby;
            probe.flags = 1;
            probe.scale = 1.0f;
            probe.count = 1;
            probe.dir[0] = fast_cos(MOBY(moby)->rot.z);
            probe.dir[1] = fast_sin(MOBY(moby)->rot.z);
            probe.dir[2] = 0.0f;
            func_L00_00250800_q(moby, 0, hit_query_position);
            func_L00_001F2BE8_q(hit_query_position, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 1, hit_query_position);
            func_L00_001F2BE8_q(hit_query_position, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 2, hit_query_position);
            func_L00_001F2BE8_q(hit_query_position, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 3, hit_query_position);
            func_L00_001F2BE8_q(hit_query_position, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 4, hit_query_position);
            func_L00_001F2BE8_q(hit_query_position, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 5, hit_query_position);
            func_L00_001F2BE8_q(hit_query_position, 1.1f, 0, moby, &probe);
            func_L00_00250800_q(moby, 6, hit_query_position);
            func_L00_001F2BE8_q(hit_query_position, 1.1f, 0, moby, &probe);
            return;
        }
        if (moby[0x70] & 2) {
            moby[0x20] = moby[0xBC];
            if ((unsigned char)moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
            }
        }
        return;
    }
    case ALIEN_QUEEN_STATE_WAIT_FOR_ORBIT_ANIMATION_END:
        if (moby[0x70] & 2) {
            moby[0x20] = moby[0xBC];
            if ((unsigned char)moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
            }
        }
        return;
    case ALIEN_QUEEN_STATE_RETURN_HOME: {
        char *pos = (char *)moby + 0x10;
        char *home = alien_queen_update_vars->home_position;
        float dist = vector_distance_xy(pos, home);
        float turn = fast_difference_between_rotations(
            MOBY(moby)->rot.z,
            fast_arc_tan(alien_queen_update_vars->home_position[0] - MOBY(moby)->pos.x,
                         alien_queen_update_vars->home_position[1] - MOBY(moby)->pos.y));
        approach_angle((float *)&MOBY(moby)->rot.z,
                       fast_arc_tan(alien_queen_update_vars->home_position[0] - MOBY(moby)->pos.x,
                                    alien_queen_update_vars->home_position[1] - MOBY(moby)->pos.y),
                       (float *)&alien_queen_update_vars->turn_velocity,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (turn < 0.7853982f) {
            fast_vec_sub(step, home, pos);
            step[2] = 0.0f;
            fast_vec_normalize(step, step, *(float *)&l06_alien_queen_movement_speed * frame_time);
            fast_vec_add(pos, pos, step);
            func_L00_00262DF0_q(alien_queen_update_vars->path, pos, 1.0f, pos);
        }
        if (alien_queen_update_vars->target_selection_status != 2) {
            moby[0x20] = 7;
            break;
        }
        if (dist < 1.0f) {
            moby[0x20] = 7;
            if ((unsigned char)moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
                return;
            }
        }
        break;
    }
    case ALIEN_QUEEN_STATE_DEATH: {
        unsigned char *flag;
        menu_item_enabled_19_lui[4] = 1;
        alien_queen_update_vars->health_sixteenths = 0;
        moby[0x30] = 0xFF;
        if (alien_queen_update_vars->death_control_flags == 2) {
            func_L06_002F6790_q(level_mobys_q + (alien_queen_update_vars->unk100 << 8));
            set_listed_alien_queen_sliders_state_2_and_clear_data_28(
                alien_queen_update_vars->slider_list);
            MOBY(moby)->flags |= 1;
            moby[0x20] = 0xF;
            moby[0x31] = 0;
            MOBY(moby)->collision = 0;
            if ((unsigned char)moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
                return;
            }
            break;
        }
        if (!(moby[0x70] & 2))
            break;
        qcopy(step, moby + 0x10);
        step[2] = step[2] + 1.0f;
        func_L00_00260460_q(moby, step, -1, 2.0f, 20.0f);
        spawn_moby_break_fragments(moby, 0x6A2, 1, 0x6A3, 3, 6, 2);
        func_L06_002F6790_q(level_mobys_q + (alien_queen_update_vars->unk100 << 8));
        set_listed_alien_queen_sliders_state_2_and_clear_data_28(
            alien_queen_update_vars->slider_list);
        moby[0x20] = 0xF;
        if ((unsigned char)moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
        }
        MOBY(moby)->collision = 0;
        moby[0x31] = 0;
        MOBY(moby)->flags |= 1;
        if (alien_queen_update_vars->suppress_death_dialogue != 0)
            return;
        flag = D_0013D407_q + 1;
        if (flag[0xA] != 0)
            return;
        flag[0xA] = 1;
        allocate_voice_for_bank_entry(1, 0, 0);
        queue_dialogue_message(0x53D6, -1);
        return;
    }
    case ALIEN_QUEEN_STATE_POST_DEATH: {
        int   i;
        char *post_death_scan_record = 0;
        for (i = 0; i < 0x30; i++) {
            if (l06_post_death_scan_record_gates[i] != 0) {
                post_death_scan_record = l06_post_death_scan_records + i * 0xA0;
                if (*(short *)(post_death_scan_record + 0x86) == 0x12) {
                    release_moby_camera_control(post_death_scan_record);
                    break;
                }
            }
        }
        if (level_game_mode_q == 2) {
            func_L06_002F6790_q(level_mobys_q + (alien_queen_update_vars->unkFC << 8));
            mark_moby_for_removal(moby);
        }
        break;
    }
    }
}

void update_alien_queen_hit_response_and_recovery(char *moby) {
    enum {
        HIT_RESPONSE_SKIP_REACTION = 0,
        HIT_RESPONSE_DEATH = 1,
        HIT_RESPONSE_DEATH_SEQUENCE = 2,
        HIT_RESPONSE_NONLETHAL_REACTION = 3,
        HIT_RESPONSE_NONLETHAL_ANIMATION_REACTION = 4,
        HIT_RESPONSE_NONLETHAL_FALLBACK = 5,
        HIT_RESPONSE_NONLETHAL_ANIMATION_UPDATE = 6,
        HIT_RESPONSE_NONLETHAL_FEEDBACK = 7,
        HIT_RESPONSE_NONLETHAL_ANIMATION_MAINTENANCE = 8,
        HIT_RESPONSE_NONLETHAL_ANIMATION_FEEDBACK = 9,
        HIT_RESPONSE_NONLETHAL_ANIMATION_RESPONSE = 10,
        HIT_RESPONSE_NO_REACTION = 11
    };
    enum {
        DAMAGE_AMPLIFYING_SOURCE_CLASS = 0x79,
        AMPLIFIED_DAMAGE_SOURCE_CLASS = 0xB0,
        REDUCED_DAMAGE_SOURCE_CLASS = 0x131,
        DAMAGE_BOOSTING_SOURCE_CLASS = 0x33B
    };
    AlienQueenVars *alien_queen_hit_recovery_vars;
    char           *hit;
    int             hit_status;
    float           dmg;
    int             kind;
    int             timer;
    float           hp;
    float           recovery_hp;
    int            *path;
    alien_queen_hit_recovery_vars = ((char *)MOBY(moby)->pvars);
    if (MOBY(moby)->state == ALIEN_QUEEN_STATE_INITIALIZE)
        return;
    if ((unsigned int)(MOBY(moby)->state - 4) < 10 && alien_queen_hit_recovery_vars->unk108 != -1)
        D_L06_0014162A = *(short *)&alien_queen_hit_recovery_vars->unk108;
    alien_queen_hit_recovery_vars->health_sixteenths =
        truncate_float_to_s32(alien_queen_hit_recovery_vars->health * 16.0f);
    dmg = 0.0f;
    hit = FUN_L00_0025a420(moby, 0x330000, 0);
    kind = resolve_moby_hit_response(
        moby, hit, &alien_queen_hit_recovery_vars->health, 0, &hit_status, &dmg, 0, 4);
    if (hit_status != HIT_STATUS_SKIP_DAMAGE_AND_REACTION && (unsigned char)moby[0x20] != 14 &&
        (unsigned char)moby[0x20] != 3 && (unsigned char)moby[0x20] != 5) {
        if (*(char **)(hit + 0x20) != 0) {
            int oclass = *(short *)(*(char **)(hit + 0x20) + 0xA6);
            switch (oclass) {
            case REDUCED_DAMAGE_SOURCE_CLASS:
                dmg *= 0.666f;
                break;
            case AMPLIFIED_DAMAGE_SOURCE_CLASS:
            case DAMAGE_AMPLIFYING_SOURCE_CLASS:
            case DAMAGE_BOOSTING_SOURCE_CLASS:
                dmg *= 1.5f;
                break;
            }
        }
        alien_queen_hit_recovery_vars->health -= dmg;
        if (alien_queen_hit_recovery_vars->intro_or_recovery_delay_active == 0) {
            if (scale_game_frames(10) <= alien_queen_hit_recovery_vars->timer_12C) {
                hp = alien_queen_hit_recovery_vars->health;
                recovery_hp = 20.0f;
                if (hp <= recovery_hp && hp + dmg > recovery_hp)
                    goto start_recovery;
            }
            if (scale_game_frames(10) <= alien_queen_hit_recovery_vars->timer_12C) {
                hp = alien_queen_hit_recovery_vars->health;
                recovery_hp = 10.0f;
                if (hp <= recovery_hp && hp + dmg > recovery_hp) {
                start_recovery:
                    alien_queen_hit_recovery_vars->health = recovery_hp;
                    alien_queen_hit_recovery_vars->intro_or_recovery_delay_active = 1;
                    alien_queen_hit_recovery_vars->recovery_health = recovery_hp;
                    alien_queen_hit_recovery_vars->timer_12C = scale_game_frames(30);
                }
            }
        } else if (alien_queen_hit_recovery_vars->health <
                   alien_queen_hit_recovery_vars->recovery_health) {
            alien_queen_hit_recovery_vars->health = alien_queen_hit_recovery_vars->recovery_health;
        }
        if (alien_queen_hit_recovery_vars->health <= 0.0f)
            kind = HIT_RESPONSE_DEATH;
        else if (kind == HIT_RESPONSE_DEATH)
            kind = HIT_RESPONSE_NONLETHAL_FALLBACK;
        switch (kind) {
        case HIT_RESPONSE_SKIP_REACTION:
        case HIT_RESPONSE_NO_REACTION:
        default:
            goto hit_response_done;
        case HIT_RESPONSE_NONLETHAL_REACTION:
        case HIT_RESPONSE_NONLETHAL_ANIMATION_REACTION:
        case HIT_RESPONSE_NONLETHAL_FALLBACK:
        case HIT_RESPONSE_NONLETHAL_ANIMATION_UPDATE:
        case HIT_RESPONSE_NONLETHAL_FEEDBACK:
        case HIT_RESPONSE_NONLETHAL_ANIMATION_MAINTENANCE:
        case HIT_RESPONSE_NONLETHAL_ANIMATION_FEEDBACK:
        case HIT_RESPONSE_NONLETHAL_ANIMATION_RESPONSE:
            alien_queen_hit_recovery_vars->hit_flash.flash_red = 200;
            break;
        case HIT_RESPONSE_DEATH:
        case HIT_RESPONSE_DEATH_SEQUENCE: {
            LevelCuboid *base;
            if (alien_queen_hit_recovery_vars->animation_update != -1) {
                set_animation_parameter(alien_queen_hit_recovery_vars->animation_update, 0);
                alien_queen_hit_recovery_vars->animation_update = -1;
            }
            moby[0x20] = 14;
            FUN_L00_00257470(moby, 0, -1);
            base = &l06_cuboids[alien_queen_hit_recovery_vars->cuboid];
            jet_reset_camera(&base->pos, &base->rot);
            MOBY(moby)->flags &= 0xEFFF;
            if ((unsigned char)moby[0x53] != 10)
                FUN_00212f90(moby, 10, 0, scale_game_frames(10));
            alien_queen_hit_recovery_vars->hit_flash.flash_red = 0xFA;
            break;
        }
        }
    hit_response_done:
        start_hit_color_flash(moby, &alien_queen_hit_recovery_vars->hit_flash);
    }
    MOBY(moby)->hit_slot = 0xFF;
    update_hit_color_flash(moby, &alien_queen_hit_recovery_vars->hit_flash);
    if ((alien_queen_hit_recovery_vars->death_control_flags & 2) &&
        (unsigned char)moby[0x20] != 15) {
        if (alien_queen_hit_recovery_vars->animation_update != -1) {
            set_animation_parameter(alien_queen_hit_recovery_vars->animation_update, 0);
            alien_queen_hit_recovery_vars->animation_update = -1;
        }
        moby[0x20] = 14;
        return;
    }
    if ((unsigned char)moby[0x20] >= 2 && D_L06_00140408 != 8 && (pad_held_lui & 0x20))
        alien_queen_hit_recovery_vars->suppress_death_dialogue = 1;
    timer = alien_queen_hit_recovery_vars->timer_12C;
    if (timer != 0 && alien_queen_hit_recovery_vars->intro_or_recovery_delay_active != 0) {
        --alien_queen_hit_recovery_vars->timer_12C;
        if (alien_queen_hit_recovery_vars->timer_12C < scale_game_frames(10))
            D_L06_0015F3FC =
                1.0f - convert_integer_to_float(alien_queen_hit_recovery_vars->timer_12C) /
                           scale_recovery_time(10.0f);
        if (alien_queen_hit_recovery_vars->timer_12C == 0 && (unsigned char)moby[0x20] != 1) {
            if (alien_queen_hit_recovery_vars->health > 10.0f) {
                alien_queen_hit_recovery_vars->health = 19.9f;
                moby[0x20] = 3;
                alien_queen_hit_recovery_vars->intro_or_recovery_delay_active = 0;
                qcopy(moby + 0x10, alien_queen_hit_recovery_vars->home_position);
                MOBY(moby)->rot.z = fast_arc_tan(hero.motion.pos.f[0] - MOBY(moby)->pos.x,
                                                 hero.motion.pos.f[1] - MOBY(moby)->pos.y);
                euler_rotation_basis(moby + 0xC0, moby + 0x40);
                if ((unsigned char)moby[0x53] != 6)
                    FUN_00212f90(moby, 6, 0, scale_game_frames(5));
                FUN_L06_002fb148_recovery_high(moby);
            } else {
                LevelCuboid *base;
                alien_queen_hit_recovery_vars->health = 9.9f;
                moby[0x20] = 5;
                alien_queen_hit_recovery_vars->intro_or_recovery_delay_active = 0;
                base = &l06_cuboids[alien_queen_hit_recovery_vars->cuboid];
                set_race_camera(&base->pos, &base->rot, 0x72, 1);
                qcopy(moby + 0x10, alien_queen_hit_recovery_vars->home_position);
                MOBY(moby)->rot.z = fast_arc_tan(hero.motion.pos.f[0] - MOBY(moby)->pos.x,
                                                 hero.motion.pos.f[1] - MOBY(moby)->pos.y);
                euler_rotation_basis(moby + 0xC0, moby + 0x40);
                if ((unsigned char)moby[0x53] != 6)
                    FUN_00212f90(moby, 6, 0, scale_game_frames(5));
                FUN_L06_002fb148_recovery_low(moby);
            }
        }
    } else if (alien_queen_hit_recovery_vars->intro_or_recovery_delay_active == 0) {
        if (alien_queen_hit_recovery_vars->timer_12C < scale_game_frames(10)) {
            D_L06_0015F3FC =
                1.0f - convert_integer_to_float(++alien_queen_hit_recovery_vars->timer_12C) / 10.0f;
        }
    }
    alien_queen_hit_recovery_vars->glow_phase =
        fast_add_rotations(alien_queen_hit_recovery_vars->glow_phase,
                           alien_queen_glow_phase_speed_degrees_gp * 0.017453292f * frame_time);
    MOBY(moby)->glow_color =
        fast_tween_color(l06_alien_queen_glow_tween_start_color,
                         l06_alien_queen_glow_tween_end_color,
                         (fast_sin(alien_queen_hit_recovery_vars->glow_phase) + 1.0f) * 0.5f);
    path = D_L06_001B0C30[alien_queen_hit_recovery_vars->path];
    FUN_L00_0025ff38(moby, &alien_queen_hit_recovery_vars->goal, 0, 0, path + 4, *path, 255.0f);
    if (*(int *)&alien_queen_hit_recovery_vars->target == 0)
        *(int *)&alien_queen_hit_recovery_vars->target = D_L06_001413D0;
}

void activate_listed_state_15_mobys_at_raised_positions(char *moby) {
    char  *activation_source_moby_vars = ((char *)MOBY(moby)->pvars);
    short *moby_index_list = D_L06_001ABFC0_c[*(int *)(activation_source_moby_vars + 0xF0)];
    short *moby_index_cursor;
    if (moby_index_list != 0) {
        moby_index_cursor = moby_index_list;
        do {
            int   listed_moby_byte_offset = (*(unsigned short *)moby_index_cursor & 0x7FFF) << 8;
            float raised_position[4];
            qcopy(raised_position, (char *)(listed_moby_byte_offset + (int)D_L06_0015FFD8) + 0x10);
            raised_position[2] = raised_position[2] + random_float_between(15.0f, 20.0f);
            activate_state_15_moby_at_position(
                (char *)((int)D_L06_0015FFD8 + listed_moby_byte_offset), raised_position);
        } while (*moby_index_cursor++ >= 0);
    }
}

void start_alien_queen_intro(char *moby) {
    float           output_position[4], output_rotation[4], moby_offset_vector[4];
    AlienQueenVars *alien_queen_vars = (AlienQueenVars *)MOBY(moby)->pvars;
    LevelCuboid    *hero_placement_node = &l06_cuboids[alien_queen_vars->cuboid];
    *(int *)&alien_queen_vars->camera_blend_fraction = 0;
    set_race_camera(&hero_placement_node->pos, &hero_placement_node->rot, 0x72, 0);
    qcopy(output_position, moby + 0x10);
    output_position[2] = hero.motion.pos.f[2] + 2.0f;
    fast_vec_normalize(moby_offset_vector, moby + 0xC0, 8.0f);
    fast_vec_add(output_position, output_position, moby_offset_vector);
    clear_u64_value(output_rotation);
    output_rotation[2] = fast_arc_tan(MOBY(moby)->pos.x - output_position[0],
                                      MOBY(moby)->pos.y - output_position[1]);
    output_rotation[1] = -fast_arc_tan(vector_distance_xy(output_position, moby + 0x10),
                                       MOBY(moby)->pos.z - output_position[2]) -
                         0.5235988f;
    jet_mount(output_position, output_rotation, 1, 0, 0);
    set_camera_position(output_position);
    set_camera_rotation(output_rotation);
    *D_L06_001B0FB0[alien_queen_vars->path] = 0;
}

int update_alien_queen_intro_camera(char *moby) {
    float output_position[4], output_rotation[4], moby_offset_position[4], hero_offset_position[4];
    AlienQueenVars *alien_queen_state;
    char           *pos;
    struct Hero    *x;
    float           blend_acceleration_step;
    alien_queen_state = ((char *)MOBY(moby)->pvars);
    clear_u64_value(output_rotation);
    if (MOBY(moby)->prev_seq == 8) {
        qcopy(output_position, D_L06_00167540);
        pos = moby + 0x10;
    } else {
        blend_acceleration_step = *(float *)&frame_time_sq * 3.0f;
        advance_accelerated_scalar(&alien_queen_state->camera_blend_fraction,
                                   (float *)&alien_queen_state->camera_blend_speed,
                                   1.0f,
                                   blend_acceleration_step,
                                   blend_acceleration_step,
                                   frame_time + frame_time);
        fast_vec_normalize(moby_offset_position, moby + 0xC0, 8.0f);
        fast_vec_add(moby_offset_position, moby_offset_position, moby + 0x10);
        x = &hero;
        moby_offset_position[2] = x->motion.pos.f[2] + 2.0f;
        fast_vec_normalize(hero_offset_position, x->rot_matrix, -7.0f);
        fast_vec_add(hero_offset_position, hero_offset_position, &x->motion.pos);
        hero_offset_position[2] += 4.0f;
        pos = moby + 0x10;
        lerp_vector(output_position,
                    moby_offset_position,
                    hero_offset_position,
                    alien_queen_state->camera_blend_fraction);
    }
    output_rotation[2] = fast_arc_tan(MOBY(moby)->pos.x - output_position[0],
                                      MOBY(moby)->pos.y - output_position[1]);
    output_rotation[1] = -fast_arc_tan(vector_distance_xy(output_position, pos),
                                       MOBY(moby)->pos.z - output_position[2]) -
                         (1.0f - alien_queen_state->camera_blend_fraction) * 30.0f * DEG_TO_RAD;
    set_camera_position(output_position);
    set_camera_rotation(output_rotation);
    if (alien_queen_state->camera_blend_fraction >= 1.0f && MOBY(moby)->prev_seq == 1)
        return 1;
    return 0;
}

void finish_alien_queen_intro(char *moby) {
    char *queen_vars = ((char *)MOBY(moby)->pvars);
    hero_set_state(HERO_STATE_IDLE, 0);
    *D_L06_001B0C30[*(int *)(queen_vars + 0xEC)] = *(int *)(queen_vars + 0x114);
    jet_end_mount(2);
}

int update_alien_queen_first_recovery_camera(char *moby) {
    float output_position[4], output_rotation[4], moby_offset_position[4], hero_offset_position[4];
    char *queen_recovery_vars;
    char *pos;
    struct Hero *x;
    float        blend_acceleration_step;
    queen_recovery_vars = ((char *)MOBY(moby)->pvars);
    clear_u64_value(output_rotation);
    if (MOBY(moby)->prev_seq == 6) {
        qcopy(output_position, D_L06_00167540);
        pos = moby + 0x10;
    } else {
        blend_acceleration_step = frame_time_sq * 0.5f;
        advance_accelerated_scalar((float *)(queen_recovery_vars + 0x118),
                                   (float *)(queen_recovery_vars + 0x11C),
                                   1.0f,
                                   blend_acceleration_step,
                                   blend_acceleration_step,
                                   frame_time * 0.5f);
        fast_vec_normalize(moby_offset_position, moby + 0xC0, 8.0f);
        fast_vec_add(moby_offset_position, moby_offset_position, moby + 0x10);
        x = &hero;
        moby_offset_position[2] = x->motion.pos.f[2] + 2.0f;
        fast_vec_normalize(hero_offset_position, x->rot_matrix, -7.0f);
        fast_vec_add(hero_offset_position, hero_offset_position, &x->motion.pos);
        hero_offset_position[2] += 4.0f;
        pos = moby + 0x10;
        lerp_vector(output_position,
                    moby_offset_position,
                    hero_offset_position,
                    *(float *)(queen_recovery_vars + 0x118));
    }
    output_rotation[2] = fast_arc_tan(MOBY(moby)->pos.x - output_position[0],
                                      MOBY(moby)->pos.y - output_position[1]);
    output_rotation[1] = -fast_arc_tan(vector_distance_xy(output_position, pos),
                                       MOBY(moby)->pos.z - output_position[2]) -
                         (1.0f - *(float *)(queen_recovery_vars + 0x118)) * 30.0f * DEG_TO_RAD;
    FUN_L06_00312a78(output_position);
    FUN_L06_00312ad0(output_rotation);
    if (*(float *)(queen_recovery_vars + 0x118) >= 1.0f && MOBY(moby)->prev_seq == 1)
        return 1;
    return 0;
}

void update_alien_queen_orbit_movement(struct Moby *moby) {
    CirclerVars *circler_vars = (CirclerVars *)moby->pvars;
    struct Moby *focus = circler_vars->focus;
    Vec4f        goal, step;
    f32          hero_ang;
    f32          self_ang;
    f32          orbit_angle_radians;

    hero_ang = fast_arc_tan(hero.motion.pos.f[0] - circler_vars->center.x,
                            hero.motion.pos.f[1] - circler_vars->center.y);
    self_ang =
        fast_arc_tan(moby->pos.x - circler_vars->center.x, moby->pos.y - circler_vars->center.y);
    orbit_angle_radians = fast_subtract_rotations(fast_add_rotations(hero_ang, 3.14159f), self_ang);
    if (orbit_angle_radians > 0.7853982f)
        orbit_angle_radians = 0.7853982f;
    else if (orbit_angle_radians < -0.7853982f)
        orbit_angle_radians = -0.7853982f;
    orbit_angle_radians = fast_add_rotations(orbit_angle_radians, self_ang);
    goal.x = fast_cos(orbit_angle_radians) * circler_vars->radius;
    goal.y = fast_sin(orbit_angle_radians) * circler_vars->radius;
    goal.z = 0.0f;
    fast_vec_add(&goal, &goal, &circler_vars->center);
    approach_angle(&moby->rot.z,
                   fast_arc_tan(focus->pos.x - moby->pos.x, focus->pos.y - moby->pos.y),
                   &circler_vars->yaw_vel,
                   frame_time_sq * 6.2831855f,
                   frame_time_sq * 6.2831855f,
                   frame_time * 3.1415927f);
    if (fast_vec_dist(&moby->pos, &goal) > 1.0f &&
        vector_distance_xy(&moby->pos, &hero.motion.pos) < 16.0f)
        approach_value(&circler_vars->speed,
                       alien_queen_orbit_speed_gp * frame_time,
                       alien_queen_movement_acceleration_gp * frame_time_sq);
    else
        approach_value(
            &circler_vars->speed, 0.0f, alien_queen_movement_acceleration_gp * frame_time_sq);
    if (circler_vars->speed == 0.0f) {
        if (moby->prev_seq != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(20));
    } else if (moby->prev_seq != 0) {
        blend_moby_animation(moby, 0, 0, scale_game_frames(20));
    }
    fast_vec_sub(&step, &goal, &moby->pos);
    step.z = 0.0f;
    fast_vec_normalize(&step, &step, circler_vars->speed);
    fast_vec_add(&moby->pos, &moby->pos, &step);
    resolve_point_path_segment_xy_clearance(1.0f, circler_vars->ground, &moby->pos, &moby->pos);
}
