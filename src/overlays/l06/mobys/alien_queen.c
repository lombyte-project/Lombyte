/* Level 06 moby class 0x41b: alien queen. */
#include "types.h"
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
extern char  D_L06_00167540[] NOT_SDA;
extern float FUN_00213f38(float *, float *, float, float, float, float);
int          update_alien_queen_intro_camera(char *moby) __asm__("FUN_L06_002fb2a8");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC8E0), where it is exact; names translated to the US level program. */
extern int *D_L06_001B0C30[];
extern int  hero_set_state(int, int) __asm__("FUN_L06_002356a0");
void        finish_alien_queen_intro(char *moby) __asm__("FUN_L06_002fb4b0");

/* Steers a moby's camera-like orientation toward a target point, returns 1 when done. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FCA98), where it is exact; names translated to the US level program. */
extern char D_L06_00167540[];
extern void FUN_L06_00312a78(void *);
extern void FUN_L06_00312ad0(void *);
int         update_alien_queen_first_recovery_camera(char *moby) __asm__("FUN_L06_002fb668");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002B5990), where it is exact; names translated to the US level program. */
#include "rnc/overlay/cuboid.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002EB140), where it is exact; names translated to the US level program. */
extern float random_angle_radians(void) __asm__("FUN_00213308");
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
void       trooper_anim_update(void *a, void *s) __asm__("FUN_L00_0025d458");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

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

extern char  D_L06_001678D0[];
extern int   D_L06_0016A010[];
extern short D_L06_00161EF0 __attribute__((sda));
extern short D_L06_00161EF4 __attribute__((sda));
extern f32   alien_queen_movement_acceleration_gp __asm__("D_L06_00161EFC") __attribute__((sda));
extern short D_L06_00161F00 __attribute__((sda));
extern short D_L06_00161F04 __attribute__((sda));
extern short D_L06_00161F08 __attribute__((sda));
extern void  FUN_L06_002fa9c0_c(void *) __asm__("FUN_L06_002fa9c0");
s32  allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) __asm__("FUN_0022db10");
void set_moby_list_state_18(int idx) __asm__("FUN_L06_002f9948");
void FUN_L06_002fb870(char *arg);
void FUN_L06_002fbc30(char *arg);
int  FUN_L06_002fba28(char *moby);
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
extern int   allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern void  func_L00_00264DB8_q(int, int) __asm__("FUN_L00_00263d40");
extern void  func_L06_00317770_q(char *) __asm__("FUN_L06_00316330");
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
extern int D_0013CAE0       NOT_SDA;
extern int D_L06_001413D0   NOT_SDA;

#include "rnc/overlay/l06/mobys/alien_queen_data/update_alien_queen_hit_response_and_recovery.h"

int D_L06_00161F10 = 0x80808080;

int D_L06_00161F14 = 0x80804080;

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

extern f32 alien_queen_orbit_speed_gp __asm__("D_L06_00161EF8") __attribute__((sda));
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
        MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
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
    char           *base;
    char           *moby;
    if (moby_index_cursor == 0)
        return 1;
    base = (char *)D_L06_0015FFD8_d;
    for (;;) {
        moby = base + ((*moby_index_cursor & 0x7FFF) << 8);
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
    char  *data = ((char *)MOBY(moby)->pvars);
    char **linked_moby_cursor;
    int    i;
    moby[0xBC] = 1;
    if ((unsigned char)moby[0x20] == 2) {
        allocate_voice_for_target_entry(0, 0, (int)moby);
        moby[0x20] = 4;
        linked_moby_cursor = (char **)(data + 0x24);
        *(int *)(data + 0x14) = 0;
        *(int *)(data + 0x18) = 0;
        for (i = 6; i >= 0; i--) {
            (*linked_moby_cursor++)[0x20] = 5;
        }
    }
}

void set_moby_and_links_to_states_8_and_9(char *moby) {
    char  *data = ((char *)MOBY(moby)->pvars);
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
        *(int *)(data + 0x18) = 0;
        linked_moby_cursor = (char **)(data + 0x24);
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
                MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
                MOBY(moby)->flags = flags;
            }
        } while (*moby_index_cursor++ >= 0);
    }
}

void alien_queen_update(unsigned char *moby) {
    unsigned char *data = ((unsigned char *)MOBY(moby)->pvars);
    char          *target;
    float          step[4];

    FUN_L06_002fa9c0_c(moby);
    target = *(char **)(data + 0xB0);
    func_L00_00264B40_q(moby, *(int *)&D_L06_00161EF0, 1.9f, data + 0x130);
    if ((unsigned char)moby[0x31] != 0 && fast_vec_dist(moby + 0x10, D_L06_00167540_q) < 45.0f) {
        func_L00_0025B178_q(moby);
        moby[0x7F] = 0x24;
    }
    if ((unsigned char)moby[0x20] >= 3 && (unsigned char)moby[0x20] <= 13) {
        *(int *)(data + 0x10C) = queue_animation_update(0x16,
                                                        0xFFFF,
                                                        func_L00_00236710_q,
                                                        func_L00_00236830_q,
                                                        func_L06_0024A080_q,
                                                        data + 0x110,
                                                        0x1EF);
    }
    switch ((unsigned char)moby[0x20]) {
    case 0: {
        int *path;
        *(float *)(data + 0x20) = 30.0f;
        *(short *)(data + 0x24) = 0x1E;
        data[0x29] = 1;
        data[0x28] = 3;
        data[0x2E] = 1;
        data[0x58] = truncate_float_to_s32(14.0f);
        data[0x5A] = truncate_float_to_s32(18.4f);
        *(int *)(data + 0x120) = scale_game_frames(0x4B0);
        *(int *)(data + 0x178) = 0;
        qcopy(data + 0xC0, moby + 0x10);
        *(int *)(data + 0x10C) = -1;
        *(int *)(data + 0x114) = *D_L06_001B0C30_q[*(int *)(data + 0xEC)];
        path = D_L06_001B0C30_q[*(int *)(data + 0xEC)];
        lerp_vector(data + 0xD0, path + 4, (char *)path + ((*path / 2) * 16 + 16), 0.5f);
        *(float *)(data + 0xDC) = vector_distance_xy(data + 0xC0, data + 0xD0);
        *(float *)(data + 0x128) = random_angle_radians();
        *(int *)(data + 0x12C) = scale_game_frames(0x1E);
        MOBY(moby)->collision = 0;
        MOBY(moby)->flags = (MOBY(moby)->flags | 0x4001) & 0xEFFF;
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + 17.5f;
        set_moby_list_state_18(*(int *)(data + 0xF4));
        if (menu_item_enabled_19_lui[4] != 0 && D_0013D4FD_q[8] == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby[0x20] = 1;
        break;
    }
    case 1:
        if (*(int *)(data + 0xB4) == 2)
            return;
        if (*(short *)(target + 0xA6) != 0)
            return;
        if (*(int *)(data + 0x170) == 0) {
            *(int *)(data + 0x170) = 1;
            *(int *)(data + 0x12C) = scale_game_frames(10);
            break;
        }
        if (*(int *)(data + 0x12C) != 0)
            return;
        func_L06_002F6818_q(level_mobys_q + (*(int *)(data + 0xFC) << 8));
        func_L06_002F6818_q(level_mobys_q + (*(int *)(data + 0x100) << 8));
        set_listed_alien_queen_sliders_state_2_and_data_28_to_one(*(int *)(data + 0x104));
        *(int *)(data + 0x170) = 0;
        moby[0x20] = 2;
        MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        MOBY(moby)->flags = (MOBY(moby)->flags | 0x1000) & 0xFFFE;
        if ((unsigned char)moby[0x53] != 8) {
            blend_moby_animation(moby, 8, 0, scale_game_frames(1));
        }
        func_L06_002FC578_q(moby);
        return;
    case 2: {
        float gravity = frame_time_sq * 30.0f;
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + *(float *)(data + 0x48);
        *(float *)(data + 0x48) = *(float *)(data + 0x48) - gravity;
        if ((unsigned char)moby[0x53] == 8) {
            if (absolute_float((MOBY(moby)->pos.z - *(float *)(data + 0xC8)) /
                               *(float *)(data + 0x48)) <= 6.0f &&
                (unsigned char)moby[0x53] != 9) {
                blend_moby_animation(moby, 9, 0, scale_game_frames(5));
            }
        } else {
            if (MOBY(moby)->pos.z < *(float *)(data + 0xC8)) {
                MOBY(moby)->pos.z = *(float *)(data + 0xC8);
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
    case 3:
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
    case 5:
        if ((unsigned char)moby[0x53] == 6 && MOBY(moby)->anim_time == 0.0f && (moby[0x70] & 2)) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
            clear_moby_list_state_18(*(int *)(data + 0xF4));
        }
        if (FUN_L06_002fba28(moby) != 0) {
            moby[0x20] = 6;
            FUN_L06_002fbc30(moby);
        }
        break;
    case 4:
        func_L06_002FC4C0_q(moby);
    case 6:
        func_L06_002FD0B8_q(moby);
        if (((unsigned char)moby[0x20] == 4 &&
             check_all_listed_frog_states_fe_fd_or_10(*(int *)(data + 0xF0)) != 0) ||
            ((unsigned char)moby[0x20] == 6 &&
             count_active_moby_group_members(*(int *)(data + 0xF4), -1) == 0)) {
            moby[0x20] = 8;
            *(int *)(data + 0x120) = scale_game_frames(0x4B0);
            if ((unsigned char)moby[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
            }
        }
        return;
    case 7:
        if (*(int *)(data + 0xB4) == 2)
            break;
        moby[0x20] = 8;
        if ((unsigned char)moby[0x53] == 3)
            break;
        blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
        return;
    case 8: {
        float dist = vector_distance_xy(moby + 0x10, target + 0x10);
        float turn = fast_difference_between_rotations(
            MOBY(moby)->rot.z,
            fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                         *(float *)(target + 0x14) - MOBY(moby)->pos.y));
        approach_angle((float *)&MOBY(moby)->rot.z,
                       fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                                    *(float *)(target + 0x14) - MOBY(moby)->pos.y),
                       (float *)(data + 0xE0),
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (turn < 0.7853982f && *(float *)&D_L06_00161F00 < dist) {
            approach_value((float *)(data + 0xE8),
                           *(float *)&D_L06_00161EF4 * frame_time,
                           alien_queen_movement_acceleration_gp * frame_time_sq);
        } else if (dist < *(float *)&D_L06_00161F04) {
            approach_value((float *)(data + 0xE8),
                           -(*(float *)&D_L06_00161EF4 * frame_time),
                           alien_queen_movement_acceleration_gp * frame_time_sq);
        } else {
            approach_value((float *)(data + 0xE8), 0.0f, frame_time_sq * 8.0f);
        }
        fast_vec_sub(step, target + 0x10, moby + 0x10);
        step[2] = 0.0f;
        fast_vec_normalize(step, step, *(float *)(data + 0xE8));
        fast_vec_add(moby + 0x10, moby + 0x10, step);
        func_L00_00262DF0_q(*(int *)(data + 0xEC), moby + 0x10, 1.0f, moby + 0x10);
        if (*(int *)(data + 0xB4) == 2) {
            moby[0x20] = 0xD;
            break;
        }
        if (fast_dec_timer(data + 0x120) != 0) {
            *(int *)(data + 0x120) = scale_game_frames(0x168);
            moby[0x20] = 0xC;
            break;
        }
        if (dist < *(float *)&D_L06_00161F00 && turn < *(float *)&D_L06_00161F08 * 0.017453292f) {
            moby[0x20] = 9;
            moby[0xBC] = 7;
            if ((unsigned char)moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(0x14));
                return;
            }
        }
        break;
    }
    case 12:
        func_L06_002FD0B8_q(moby);
        if (fast_dec_timer(data + 0x120) != 0) {
            *(int *)(data + 0x120) = scale_game_frames(0x4B0);
            moby[0x20] = 0xB;
            moby[0xBC] = 7;
            if ((unsigned char)moby[0x53] != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(0x14));
            }
        }
        return;
    case 9:
        fast_dec_timer(data + 0x120);
        approach_angle((float *)&MOBY(moby)->rot.z,
                       MOBY(moby)->rot.z,
                       (float *)(data + 0xE0),
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
    case 10: {
        QueenProbe probe;
        float      buf[4];
        fast_dec_timer(data + 0x120);
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
            func_L00_00250800_q(moby, 0, buf);
            func_L00_001F2BE8_q(buf, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 1, buf);
            func_L00_001F2BE8_q(buf, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 2, buf);
            func_L00_001F2BE8_q(buf, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 3, buf);
            func_L00_001F2BE8_q(buf, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 4, buf);
            func_L00_001F2BE8_q(buf, 0.5f, 0, moby, &probe);
            func_L00_00250800_q(moby, 5, buf);
            func_L00_001F2BE8_q(buf, 1.1f, 0, moby, &probe);
            func_L00_00250800_q(moby, 6, buf);
            func_L00_001F2BE8_q(buf, 1.1f, 0, moby, &probe);
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
    case 11:
        if (moby[0x70] & 2) {
            moby[0x20] = moby[0xBC];
            if ((unsigned char)moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
            }
        }
        return;
    case 13: {
        char *pos = (char *)moby + 0x10;
        char *home = (char *)data + 0xC0;
        float dist = vector_distance_xy(pos, home);
        float turn = fast_difference_between_rotations(
            MOBY(moby)->rot.z,
            fast_arc_tan(*(float *)(data + 0xC0) - MOBY(moby)->pos.x,
                         *(float *)(data + 0xC4) - MOBY(moby)->pos.y));
        approach_angle((float *)&MOBY(moby)->rot.z,
                       fast_arc_tan(*(float *)(data + 0xC0) - MOBY(moby)->pos.x,
                                    *(float *)(data + 0xC4) - MOBY(moby)->pos.y),
                       (float *)(data + 0xE0),
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (turn < 0.7853982f) {
            fast_vec_sub(step, home, pos);
            step[2] = 0.0f;
            fast_vec_normalize(step, step, *(float *)&D_L06_00161EF4 * frame_time);
            fast_vec_add(pos, pos, step);
            func_L00_00262DF0_q(*(int *)(data + 0xEC), pos, 1.0f, pos);
        }
        if (*(int *)(data + 0xB4) != 2) {
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
    case 14: {
        unsigned char *flag;
        menu_item_enabled_19_lui[4] = 1;
        *(int *)(data + 0x110) = 0;
        moby[0x30] = 0xFF;
        if (data[0x2E] == 2) {
            func_L06_002F6790_q(level_mobys_q + (*(int *)(data + 0x100) << 8));
            set_listed_alien_queen_sliders_state_2_and_clear_data_28(*(int *)(data + 0x104));
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
        func_L06_002F6790_q(level_mobys_q + (*(int *)(data + 0x100) << 8));
        set_listed_alien_queen_sliders_state_2_and_clear_data_28(*(int *)(data + 0x104));
        moby[0x20] = 0xF;
        if ((unsigned char)moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
        }
        MOBY(moby)->collision = 0;
        moby[0x31] = 0;
        MOBY(moby)->flags |= 1;
        if (*(int *)(data + 0x178) != 0)
            return;
        flag = D_0013D407_q + 1;
        if (flag[0xA] != 0)
            return;
        flag[0xA] = 1;
        allocate_voice_for_bank_entry(1, 0, 0);
        func_L00_00264DB8_q(0x53D6, -1);
        return;
    }
    case 15: {
        int   i;
        char *other = 0;
        for (i = 0; i < 0x30; i++) {
            if (D_L06_0016A010[i] != 0) {
                other = D_L06_001678D0 + i * 0xA0;
                if (*(short *)(other + 0x86) == 0x12) {
                    func_L06_00317770_q(other);
                    break;
                }
            }
        }
        if (level_game_mode_q == 2) {
            func_L06_002F6790_q(level_mobys_q + (*(int *)(data + 0xFC) << 8));
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
    char *data;
    char *hit;
    int   out;
    float dmg;
    int   kind;
    int   timer;
    float hp;
    float recovery_hp;
    int  *path;
    data = ((char *)MOBY(moby)->pvars);
    if (MOBY(moby)->state == 0)
        return;
    if ((unsigned int)(MOBY(moby)->state - 4) < 10 && *(int *)(data + 0x108) != -1)
        D_L06_0014162A = *(short *)(data + 0x108);
    *(int *)(data + 0x110) = truncate_float_to_s32(*(float *)(data + 0x20) * 16.0f);
    dmg = 0.0f;
    hit = FUN_L00_0025a420(moby, 0x330000, 0);
    kind = resolve_moby_hit_response(moby, hit, data + 0x20, 0, &out, &dmg, 0, 4);
    if (out != 1 && (unsigned char)moby[0x20] != 14 && (unsigned char)moby[0x20] != 3 &&
        (unsigned char)moby[0x20] != 5) {
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
        *(float *)(data + 0x20) -= dmg;
        if (*(int *)(data + 0x170) == 0) {
            if (scale_game_frames(10) <= *(int *)(data + 0x12C)) {
                hp = *(float *)(data + 0x20);
                recovery_hp = 20.0f;
                if (hp <= recovery_hp && hp + dmg > recovery_hp)
                    goto start_recovery;
            }
            if (scale_game_frames(10) <= *(int *)(data + 0x12C)) {
                hp = *(float *)(data + 0x20);
                recovery_hp = 10.0f;
                if (hp <= recovery_hp && hp + dmg > recovery_hp) {
                start_recovery:
                    *(float *)(data + 0x20) = recovery_hp;
                    *(int *)(data + 0x170) = 1;
                    *(float *)(data + 0x174) = recovery_hp;
                    *(int *)(data + 0x12C) = scale_game_frames(30);
                }
            }
        } else if (*(float *)(data + 0x20) < *(float *)(data + 0x174)) {
            *(float *)(data + 0x20) = *(float *)(data + 0x174);
        }
        if (*(float *)(data + 0x20) <= 0.0f)
            kind = 1;
        else if (kind == 1)
            kind = 5;
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
            *(unsigned char *)(data + 0x67) = 200;
            break;
        case HIT_RESPONSE_DEATH:
        case HIT_RESPONSE_DEATH_SEQUENCE: {
            LevelCuboid *base;
            if (*(int *)(data + 0x10C) != -1) {
                set_animation_parameter(*(int *)(data + 0x10C), 0);
                *(int *)(data + 0x10C) = -1;
            }
            moby[0x20] = 14;
            FUN_L00_00257470(moby, 0, -1);
            base = &l06_cuboids[*(int *)(data + 0xF8)];
            jet_reset_camera(&base->pos, &base->rot);
            MOBY(moby)->flags &= 0xEFFF;
            if ((unsigned char)moby[0x53] != 10)
                FUN_00212f90(moby, 10, 0, scale_game_frames(10));
            *(unsigned char *)(data + 0x67) = 0xFA;
            break;
        }
        }
    hit_response_done:
        trooper_anim_update(moby, data + 0x60);
    }
    MOBY(moby)->hit_slot = 0xFF;
    trooper_anim_finish(moby, data + 0x60);
    if ((data[0x2E] & 2) && (unsigned char)moby[0x20] != 15) {
        if (*(int *)(data + 0x10C) != -1) {
            set_animation_parameter(*(int *)(data + 0x10C), 0);
            *(int *)(data + 0x10C) = -1;
        }
        moby[0x20] = 14;
        return;
    }
    if ((unsigned char)moby[0x20] >= 2 && D_L06_00140408 != 8 && (D_0013CAE0 & 0x20))
        *(int *)(data + 0x178) = 1;
    timer = *(int *)(data + 0x12C);
    if (timer != 0 && *(int *)(data + 0x170) != 0) {
        --*(int *)(data + 0x12C);
        if (*(int *)(data + 0x12C) < scale_game_frames(10))
            D_L06_0015F3FC = 1.0f - convert_integer_to_float(*(int *)(data + 0x12C)) /
                                        scale_recovery_time(10.0f);
        if (*(int *)(data + 0x12C) == 0 && (unsigned char)moby[0x20] != 1) {
            if (*(float *)(data + 0x20) > 10.0f) {
                *(float *)(data + 0x20) = 19.9f;
                moby[0x20] = 3;
                *(int *)(data + 0x170) = 0;
                qcopy(moby + 0x10, data + 0xC0);
                MOBY(moby)->rot.z = fast_arc_tan(hero.motion.pos.f[0] - MOBY(moby)->pos.x,
                                                 hero.motion.pos.f[1] - MOBY(moby)->pos.y);
                euler_rotation_basis(moby + 0xC0, moby + 0x40);
                if ((unsigned char)moby[0x53] != 6)
                    FUN_00212f90(moby, 6, 0, scale_game_frames(5));
                FUN_L06_002fb148_recovery_high(moby);
            } else {
                LevelCuboid *base;
                *(float *)(data + 0x20) = 9.9f;
                moby[0x20] = 5;
                *(int *)(data + 0x170) = 0;
                base = &l06_cuboids[*(int *)(data + 0xF8)];
                set_race_camera(&base->pos, &base->rot, 0x72, 1);
                qcopy(moby + 0x10, data + 0xC0);
                MOBY(moby)->rot.z = fast_arc_tan(hero.motion.pos.f[0] - MOBY(moby)->pos.x,
                                                 hero.motion.pos.f[1] - MOBY(moby)->pos.y);
                euler_rotation_basis(moby + 0xC0, moby + 0x40);
                if ((unsigned char)moby[0x53] != 6)
                    FUN_00212f90(moby, 6, 0, scale_game_frames(5));
                FUN_L06_002fb148_recovery_low(moby);
            }
        }
    } else if (*(int *)(data + 0x170) == 0) {
        if (*(int *)(data + 0x12C) < scale_game_frames(10)) {
            D_L06_0015F3FC = 1.0f - convert_integer_to_float(++*(int *)(data + 0x12C)) / 10.0f;
        }
    }
    *(float *)(data + 0x128) =
        fast_add_rotations(*(float *)(data + 0x128),
                           alien_queen_glow_phase_speed_degrees_gp * 0.017453292f * frame_time);
    MOBY(moby)->glow_color = fast_tween_color(
        D_L06_00161F10, D_L06_00161F14, (fast_sin(*(float *)(data + 0x128)) + 1.0f) * 0.5f);
    path = D_L06_001B0C30[*(int *)(data + 0xEC)];
    FUN_L00_0025ff38(moby, data + 0x70, 0, 0, path + 4, *path, 255.0f);
    if (*(int *)(data + 0xB0) == 0)
        *(int *)(data + 0xB0) = D_L06_001413D0;
}

void activate_listed_state_15_mobys_at_raised_positions(char *moby) {
    char  *data = ((char *)MOBY(moby)->pvars);
    short *moby_index_list = D_L06_001ABFC0_c[*(int *)(data + 0xF0)];
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
    float        output_position[4], output_rotation[4], moby_offset_vector[4];
    char        *data = ((char *)MOBY(moby)->pvars);
    LevelCuboid *base = &l06_cuboids[*(int *)(data + 0xF8)];
    *(int *)(data + 0x118) = 0;
    set_race_camera(&base->pos, &base->rot, 0x72, 0);
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
    *D_L06_001B0FB0[*(int *)(data + 0xEC)] = 0;
}

int update_alien_queen_intro_camera(char *moby) {
    float output_position[4], output_rotation[4], moby_offset_position[4], hero_offset_position[4];
    char *data;
    char *pos;
    struct Hero *x;
    float        blend_acceleration_step;
    data = ((char *)MOBY(moby)->pvars);
    clear_u64_value(output_rotation);
    if (MOBY(moby)->prev_seq == 8) {
        qcopy(output_position, D_L06_00167540);
        pos = moby + 0x10;
    } else {
        blend_acceleration_step = *(float *)&frame_time_sq * 3.0f;
        FUN_00213f38((float *)(data + 0x118),
                     (float *)(data + 0x11C),
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
        lerp_vector(
            output_position, moby_offset_position, hero_offset_position, *(float *)(data + 0x118));
    }
    output_rotation[2] = fast_arc_tan(MOBY(moby)->pos.x - output_position[0],
                                      MOBY(moby)->pos.y - output_position[1]);
    output_rotation[1] = -fast_arc_tan(vector_distance_xy(output_position, pos),
                                       MOBY(moby)->pos.z - output_position[2]) -
                         (1.0f - *(float *)(data + 0x118)) * 30.0f * DEG_TO_RAD;
    set_camera_position(output_position);
    set_camera_rotation(output_rotation);
    if (*(float *)(data + 0x118) >= 1.0f && MOBY(moby)->prev_seq == 1)
        return 1;
    return 0;
}

void finish_alien_queen_intro(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    hero_set_state(HERO_STATE_IDLE, 0);
    *D_L06_001B0C30[*(int *)(data + 0xEC)] = *(int *)(data + 0x114);
    jet_end_mount(2);
}

int update_alien_queen_first_recovery_camera(char *moby) {
    float output_position[4], output_rotation[4], moby_offset_position[4], hero_offset_position[4];
    char *data;
    char *pos;
    struct Hero *x;
    float        blend_acceleration_step;
    data = ((char *)MOBY(moby)->pvars);
    clear_u64_value(output_rotation);
    if (MOBY(moby)->prev_seq == 6) {
        qcopy(output_position, D_L06_00167540);
        pos = moby + 0x10;
    } else {
        blend_acceleration_step = frame_time_sq * 0.5f;
        FUN_00213f38((float *)(data + 0x118),
                     (float *)(data + 0x11C),
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
        lerp_vector(
            output_position, moby_offset_position, hero_offset_position, *(float *)(data + 0x118));
    }
    output_rotation[2] = fast_arc_tan(MOBY(moby)->pos.x - output_position[0],
                                      MOBY(moby)->pos.y - output_position[1]);
    output_rotation[1] = -fast_arc_tan(vector_distance_xy(output_position, pos),
                                       MOBY(moby)->pos.z - output_position[2]) -
                         (1.0f - *(float *)(data + 0x118)) * 30.0f * DEG_TO_RAD;
    FUN_L06_00312a78(output_position);
    FUN_L06_00312ad0(output_rotation);
    if (*(float *)(data + 0x118) >= 1.0f && MOBY(moby)->prev_seq == 1)
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
