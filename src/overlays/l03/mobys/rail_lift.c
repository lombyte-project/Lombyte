/* Level 03 moby class rail_lift (0x3f4): carries the hero between the ends of two rail paths, with its help message and hud. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/ui/help/try_set_help_message.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/overlay/l03/mobys/taxi.h"
#include "rnc/overlay/l03/mobys/rail_lift_globals.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "sda.h"

int is_rail_lift_started(struct Moby *moby) __asm__("FUN_L03_002dcb30");

int is_rail_lift_started(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (moby->oclass != 0x3F4) {
        return 0;
    }
    if (moby->state && *(int *)(data + 0x7C) == 1) {
        return 1;
    }
    return 0;
}
/* Shows a help message for the mode, then keeps a hud queue entry alive. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002DDF90), where it is exact; names translated to the US level program. */

extern void initialize_numeric_hud_element(void) __asm__("FUN_L00_00237190");
extern void hud_keep_visible(int, int) __asm__("FUN_L00_00235e18");
extern void FUN_L00_002371e0(void);
extern void draw_hud_text_panel(void) __asm__("FUN_L00_00237200");
s32         queue_animation_update(s32 chan, s32 id, s32 fn, s32 d, s32 e, s32 c, s32 b) __asm__("FUN_001ff308");

void update_rail_lift_help_and_hud(struct Moby *lift_moby, int mode) __asm__("FUN_L03_002dcbc8");

void update_rail_lift_help_and_hud(struct Moby *lift_moby, int mode) {
    char *lift_vars = (char *)lift_moby->pvars;
    int   hud_key;
    switch (mode) {
    case 1:
        try_set_help_message(8, 0xBCE);
        break;
    case 2:
        try_set_help_message(8, 0xBCF);
        break;
    case 3:
        try_set_help_message(8, 0xBCB);
        break;
    case 4:
        try_set_help_message(8, 0xBCD);
        break;
    default:
        try_set_help_message(8, -1);
        break;
    }
    hud_key = *(int *)(lift_vars + 0xA4);
    if (hud_key == -1) {
        *(int *)(lift_vars + 0xA4) = queue_animation_update(0xC,
                                                            0,
                                                            (int)initialize_numeric_hud_element,
                                                            (int)FUN_L00_002371e0,
                                                            (int)draw_hud_text_panel,
                                                            0,
                                                            0);
    } else {
        hud_keep_visible(hud_key, 0xA);
    }
}

extern s32  D_L03_0015F594 __attribute__((sda));
extern int  is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int  is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern void request_hero_target_pose(void *, float, int) __asm__("FUN_L00_00233ee8");
extern s32  get_hero_target_pose_status(void) __asm__("FUN_L00_00233f38");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

/* Pvars of a rail lift that carries the hero between the ends of two rail paths. */
struct RailLiftVars {
    u8  pad0[0x20];
    u8  coll[0x44]; /* 0x20: collision record moved with the lift */
    s32 unk64;      /* 0x64: set to 1 when a ride starts */
    u8  pad68[0x10];
    s32 snd;       /* 0x78: travel sound voice (FUN_L03_002d3918) */
    s32 started;   /* 0x7C: set once the first ride starts */
    u8  start_end; /* 0x80: path end the lift spawns at */
    u8  pad81[3];
    s32 ready;     /* 0x84: set when the hero walks away from a stopped lift */
    s32 timer;     /* 0x88: frames to wait after a ride */
    s32 autostart; /* 0x8C: start without waiting for the trigger volume */
    s32 path[2];   /* 0x90: rail path index of each end (-1: none) */
    s32 help[2];   /* 0x98: help message mode for each end (FUN_L03_002dcbc8) */
    s32 volume;    /* 0xA0: trigger clip volume */
    s32 hud;       /* 0xA4: hud queue entry (FUN_L03_002dcbc8) */
};

void update_rail_lift_oscillation(struct Moby *moby) __asm__("FUN_L03_002dd0f0");

void update_rail_lift(struct Moby *lift_moby) __asm__("FUN_L03_002dccc0");

void update_rail_lift(struct Moby *lift_moby) {
    enum {
        LIFT_INITIALIZE = 0,
        LIFT_WAIT_FOR_DEPARTURE = 1,
        LIFT_WAIT_FOR_HERO_ANIMATION = 2,
        LIFT_FOLLOW_PATH = 3
    };
    struct RailLiftVars *lift_vars = (struct RailLiftVars *)lift_moby->pvars;
    Vec4                 oldPos, oldRot, move;
    s32                  hero_end, lift_end;

    if (lift_vars->path[0] == -1 || lift_vars->path[1] == -1)
        return;
    set_moby_dialogue_record_value(lift_moby, is_rail_lift_started((unsigned char *)lift_moby));
    qcopy(&oldPos, &lift_moby->pos);
    oldRot.q = *(u128 *)&lift_moby->rot;
    fast_dec_timer(&lift_vars->timer);
    switch (lift_moby->state) {
    case LIFT_INITIALIZE:
        lift_moby->command = lift_vars->start_end;
        qcopy(&lift_moby->pos, &(l03_paths[lift_vars->path[lift_moby->command]])->points[0]);
        lift_vars->hud = -1;
        lift_vars->timer = 0;
        lift_moby->state = 1;
        lift_moby->update_dist = 0xFF;
        lift_moby->draw_dist = 0xFF;
        break;
    case LIFT_WAIT_FOR_DEPARTURE:
        hero_end = fast_vec_dist(&hero.motion.pos, &(l03_paths[lift_vars->path[0]])->points[0]) <
                   fast_vec_dist(&hero.motion.pos, &(l03_paths[lift_vars->path[1]])->points[0]);
        lift_end = fast_vec_dist(&lift_moby->pos, &(l03_paths[lift_vars->path[0]])->points[0]) <
                   fast_vec_dist(&lift_moby->pos, &(l03_paths[lift_vars->path[1]])->points[0]);
        if (!lift_vars->started) {
            if (lift_vars->autostart ||
                is_point_inside_clip_volume(&hero.motion.pos, lift_vars->volume)) {
                lift_moby->collision = lift_moby->pclass->collision;
                lift_moby->command = hero_end;
                lift_moby->flags &= 0xFFBE;
                *(u128 *)&lift_moby->pos =
                    *(u128 *)&(l03_paths[lift_vars->path[hero_end]])->points[0];
                lift_moby->state = 3;
                lift_vars->unk64 = 1;
                lift_vars->started = 1;
                lift_vars->ready = 0;
                break;
            }
        }
        if (!lift_vars->started) {
            lift_moby->collision = 0;
            lift_moby->flags |= 0x41;
        } else if (lift_vars->ready && is_hero_on_moby(lift_moby)) {
            update_rail_lift_help_and_hud((char *)lift_moby,
                                          lift_vars->help[lift_moby->command ^ 1]);
            if ((D_0013CAE4 & 0x10) && D_L03_0015F594 == 8) {
                request_hero_target_pose(&lift_moby->pos, lift_moby->rot.z, 0);
                lift_moby->command ^= 1;
                lift_vars->unk64 = 1;
                lift_vars->ready = 0;
                lift_moby->state = 2;
            }
        } else if (hero_end != lift_end) {
            lift_moby->state = 3;
            lift_moby->command ^= 1;
            lift_vars->unk64 = 1;
            lift_vars->ready = 0;
        }
        break;
    case LIFT_WAIT_FOR_HERO_ANIMATION:
        if (!get_hero_target_pose_status())
            lift_moby->state = 3;
        break;
    case LIFT_FOLLOW_PATH:
        if (is_hero_on_moby(lift_moby)) {
            hero.jump_lock_timer = 4;
            hero.unk1F4 = 4;
        }
        if (advance_moby_along_rail_path(
                lift_moby, lift_vars->path[lift_moby->command], &lift_vars->snd)) {
            lift_moby->state = 1;
            lift_vars->timer = scale_game_frames(0xF);
        }
        break;
    }
    update_rail_lift_oscillation(lift_moby);
    fast_vec_sub(&move, &lift_moby->pos, &oldPos);
    update_entity_matrix(lift_vars->coll, &move, &oldRot, &lift_moby->rot);
    if (!is_hero_on_moby(lift_moby) &&
        vector_distance_xy(&lift_moby->pos, &hero.motion.pos) > 2.0f && lift_vars->timer == 0)
        lift_vars->ready = 1;
}

/* Bobs and rocks the lift: vertical and rotation oscillation from level constants (degrees per second). */
extern float moby_vertical_oscillation_amplitude_gp __asm__("D_L03_00161BD8") __attribute__((sda));
extern float moby_vertical_oscillation_phase_rate_gp __asm__("D_L03_00161BDC") __attribute__((sda));
extern float moby_rotation_oscillation_amplitude_gp __asm__("D_L03_00161BE0") __attribute__((sda));
extern float moby_rotation_oscillation_first_phase_rate_gp __asm__("D_L03_00161BE4")
    __attribute__((sda));
extern float moby_rotation_oscillation_second_phase_rate_gp __asm__("D_L03_00161BE8")
    __attribute__((sda));
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(char *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");

void update_rail_lift_oscillation(struct Moby *moby) {
    char *moby_pvars = (char *)moby->pvars;
    update_moby_vertical_oscillation(moby_vertical_oscillation_amplitude_gp,
                                     moby_vertical_oscillation_phase_rate_gp * DEG_TO_RAD *
                                         frame_time,
                                     moby,
                                     (float *)(moby_pvars + 0xac),
                                     (float *)(moby_pvars + 0xa8));
    update_moby_rotation_oscillation(
        moby,
        moby_pvars + 0xb0,
        moby_pvars + 0xb4,
        moby_rotation_oscillation_amplitude_gp,
        moby_rotation_oscillation_first_phase_rate_gp * DEG_TO_RAD * frame_time,
        moby_rotation_oscillation_second_phase_rate_gp * DEG_TO_RAD * frame_time);
}
