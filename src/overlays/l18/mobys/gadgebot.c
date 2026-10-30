/* Level 18 moby class gadgebot: its update, damage and targeting, movement and glows, the group helpers that launch and count gadgebots, and its suck cannon and hide methods. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/l18/mobys/gadgebot_globals.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"

extern int  D_L18_0015F5C4;
extern int  game_frame __asm__("D_L18_0015F5CC");
extern s32  random_integer_below(s32) __asm__("FUN_00213260");
extern int  D_L18_001AC240[];
extern int  D_L18_0015FFD8;
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FD058), where it is exact; names translated to the US level program. */

typedef unsigned int QVd058 __attribute__((mode(TI), aligned(16)));

extern char  D_L18_001625F0[4] __attribute__((sda));
extern char  D_L18_00162608 __attribute__((sda));
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int   spawn_explosion() __asm__("FUN_L00_0025e450");
extern int   FUN_L00_002dc9f0(void *, void *);
extern int   is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern int   random_integer_below(int);
extern int   scale_game_frames(int);

#include "rnc/overlay/l18/mobys/gadgebot_data/gadgebot_update.h"

extern void collect_mobies_in_sphere_at_moby_point(
    void *, float, float, float, int, int, int, int, int) __asm__("FUN_L00_002599e8");
extern void update_gadgebot_damage_target_and_glow(struct Moby *) __asm__("FUN_L18_002fc100");
void        draw_gadgebot_launch_glow_quad(struct Moby *m) __asm__("FUN_L18_002fc860");

float       move_gadgebot_toward_target(char *self, float *src) __asm__("FUN_L18_002fc4e0");
extern void func_L00_0025F4A8_alt(void *,
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

void gadgebot_update(unsigned char *m) __asm__("FUN_L18_002fbb98");

void gadgebot_update(unsigned char *moby) {
    enum {
        GADGEBOT_INITIALIZE = 0,
        GADGEBOT_LAUNCH_FLIGHT = 1,
        GADGEBOT_WAIT_TO_APPROACH = 2,
        GADGEBOT_APPROACH_TARGET = 3,
        GADGEBOT_FACE_TARGET = 4,
        GADGEBOT_DEATH_FALL = 5,
        GADGEBOT_EXPLODE = 6
    };
    char           position_scratch[16];
    unsigned char *moby_vars = ((unsigned char *)MOBY(moby)->pvars);
    unsigned char *moby_transform_ptr;
    update_gadgebot_damage_target_and_glow((struct Moby *)moby);
    switch (moby[0x20]) {
    case GADGEBOT_INITIALIZE: {
        unsigned short moby_flags;
        moby[0x20] = 8;
        if (random_integer_below(2))
            MOBY(moby)->flags |= 0x8000;
        else
            MOBY(moby)->flags &= 0x7FFF;
        MOBY(moby)->flags |= 0x100;
        moby[0x73] = D_L18_00162608;
        *(void **)(moby_vars + 0x140) = &D_L18_001625F0;
        *(float *)(moby_vars + 0x20) = 1.0f;
        moby_vars[0x58] = 8;
        *(short *)(moby_vars + 0x24) = 1;
        moby_vars[0x5C] = 1;
        *(float *)(moby_vars + 0x30) = 0.5f;
        *(float *)(moby_vars + 0x1F8) = random_angle_radians();
        moby[0x30] = 0xFF;
        moby_flags = MOBY(moby)->flags;
        MOBY(moby)->draw_dist = 0xFF;
        moby_flags |= 1;
        moby[0x31] = 0;
        moby_flags &= 0xEFFF;
        MOBY(moby)->collision = 0;
        MOBY(moby)->flags = moby_flags;
        break;
    }
    case GADGEBOT_LAUNCH_FLIGHT: {
        float launch_gravity_and_minimum_z = 10.0f;
        float r;
        moby_transform_ptr = moby + 0x10;
        *(float *)(moby_vars + 0x1D8) =
            *(float *)(moby_vars + 0x1D8) - frame_time_sq * launch_gravity_and_minimum_z;
        fast_vec_add(moby_transform_ptr, moby_transform_ptr, moby_vars + 0x1D0);
        qcopy(position_scratch, moby_transform_ptr);
        ((float *)position_scratch)[2] += 1.0f;
        r = probe_ground_height(position_scratch, 0, 0.5f);
        if (MOBY(moby)->pos.z < r) {
            MOBY(moby)->pos.z = r;
            moby[0x20] = 2;
            if (moby[0x53])
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        } else if (MOBY(moby)->pos.z < launch_gravity_and_minimum_z) {
            moby[0x20] = 6;
        }
        break;
    }
    case 7:
        if (FUN_L00_002dc9f0(moby, moby_vars + 0x70)) {
            moby[0x20] = 2;
            *(short *)(moby_vars + 0x138) = 0;
        }
        break;
    case GADGEBOT_WAIT_TO_APPROACH:
        if (*(int *)(moby_vars + 0x1C4) == 2) {
            if (!(10.0f < vector_distance_xy(moby + 0x10, moby_vars + 0x180)))
                break;
        }
        moby[0x20] = 3;
        if (moby[0x53] != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        break;
    case GADGEBOT_APPROACH_TARGET: {
        float target_distance;
        *(QVd058 *)position_scratch = *(QVd058 *)(moby_vars + 0x180);
        target_distance = move_gadgebot_toward_target(moby, position_scratch);
        if (*(int *)(moby_vars + 0x1C4) == 2 && target_distance < 5.0f) {
            moby[0x20] = 2;
            if (moby[0x53])
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            break;
        }
        if (target_distance < 1.5f) {
            moby[0x20] = 4;
            if (moby[0x53] != 2) {
                int animation_frame_index = (int)gadgebot_sequence_2_start_frame_gp;
                blend_moby_animation(moby, 2, animation_frame_index, scale_game_frames(5));
            }
        }
        break;
    }
    case GADGEBOT_FACE_TARGET: {
        float r, k, b, max_turn_step;
        moby_transform_ptr = moby + 0x48;
        r = fast_arc_tan(*(float *)(moby_vars + 0x180) - MOBY(moby)->pos.x,
                         *(float *)(moby_vars + 0x184) - MOBY(moby)->pos.y);
        k = 6.2831855f;
        b = frame_time_sq * k;
        max_turn_step = frame_time * k;
        approach_angle(
            (float *)moby_transform_ptr, (float *)(moby_vars + 0x1E8), r, b, b, max_turn_step);
        if (is_value_within_interpolated_window((char *)moby, 14.0f))
            collect_mobies_in_sphere_at_moby_point(moby, 0.5f, 1.0f, 1.0f, 1, 1, 0, 1, 0);
        if (moby[0x70] & 2) {
            if (*(int *)(moby_vars + 0x1C4) == 2) {
                moby[0x20] = 2;
                if (moby[0x53])
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                break;
            }
            if (!(2.5f < vector_distance_xy(moby + 0x10, moby_vars + 0x180)))
                break;
            moby[0x20] = 3;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            break;
        }
        break;
    }
    case GADGEBOT_DEATH_FALL:
        if (!(step_moby_motion(moby, moby_vars + 0x70) & 0x140))
            break;
    case GADGEBOT_EXPLODE:
        qcopy(position_scratch, moby + 0x10);
        ((float *)position_scratch)[2] += 0.5f;
        func_L00_0025F4A8_alt(moby,
                              moby_vars + 0x40,
                              position_scratch,
                              0.0f,
                              0.0f,
                              5,
                              2,
                              4,
                              1.0f,
                              0.5f,
                              9.0f,
                              0.5f,
                              4,
                              0.0f,
                              0,
                              1,
                              -1,
                              0);
        moby[0x20] = 8;
        if (moby[0x53])
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        MOBY(moby)->collision = 0;
        MOBY(moby)->flags = (MOBY(moby)->flags | 0x41) & 0xEFFF;
        break;
    }
    moby_transform_ptr = moby + 0xC0;
    euler_rotation_basis(moby_transform_ptr, moby + 0x40);
    if (*(float *)(moby_vars + 0x1F4) != 0.0f) {
        float g;
        approach_value((float *)(moby_vars + 0x1F4), 0.0f, frame_time * 4.0f);
        g = 1.0f;
        fast_vec_scale(moby + 0xE0, moby + 0xE0, *(float *)(moby_vars + 0x1F4) + g);
        g = g - *(float *)(moby_vars + 0x1F4);
        fast_vec_scale(moby_transform_ptr, moby_transform_ptr, g);
        fast_vec_scale(moby + 0xD0, moby + 0xD0, g);
        add_draw_callback(draw_gadgebot_launch_glow_quad, moby);
    }
}

extern int *D_L18_001B0EB0[] __asm__("D_L18_001B0EB0");
void        draw_group_gadgebot_joint_glows(struct Moby *moby) __asm__("FUN_L18_002fca98");

extern void draw_group_gadgebot_joint_glows(struct Moby *moby) __asm__("FUN_L18_002fca98");
extern int
fun_00213928(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_00213928");
extern int  random_integer_below(int) __asm__("FUN_00213260");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern int
FUN_L00_0025ff38_c100(float, void *, void *, int, int, int *, int) __asm__("FUN_L00_0025ff38");

typedef struct {
    char          pad0[0x20];
    float         health; /* 0x20 */
    char          pad24[0x3C];
    char          anim[0x7];  /* 0x60 */
    unsigned char anim_flags; /* 0x67 */
    char          pad68[8];
    char          fall[0x10]; /* 0x70 */
    float         unk80;
    float         unk84;
    float         unk88;
    float         unk8C;
    int           unk90;
    int           unk94;
    float         unk98;
    char          pad9C[0x11];
    unsigned char unkAD; /* 0xAD */
    char          padAE[0xE];
    float         unkBC;
    float         unkC0;
    float         unkC4;
    char          padC8[0xB8];
    u128          target; /* 0x180 */
    char          pad190[0x30];
    struct Moby  *target_moby; /* 0x1C0 */
    int           target_mode; /* 0x1C4 */
    char          pad1C8[0x18];
    int           path; /* 0x1E0 */
    char          pad1E4[0x14];
    float         spin; /* 0x1F8 */
} SentryVars_c100;

extern int gadgebot_joint_glows_last_queued_frame_gp __asm__("D_L18_00162604") __attribute__((sda));
extern int gadgebot_glow_tint_start_gp __asm__("D_L18_0016260C") __attribute__((sda));
extern int gadgebot_glow_tint_end_gp __asm__("D_L18_00162610") __attribute__((sda));
extern float D_L18_001625FC __attribute__((sda));
extern float gadgebot_glow_default_phase_speed_degrees_per_second_gp __asm__("D_L18_00162614")
    __attribute__((sda));
extern float gadgebot_glow_state_4_or_5_phase_speed_degrees_per_second_gp __asm__("D_L18_00162618")
    __attribute__((sda));

#include "rnc/overlay/l18/mobys/gadgebot_data/update_gadgebot_damage_target_and_glow.h"

/* Sentry update: spins and tints the moby, takes hits (knockback, then a fall), and follows its path toward the hero. */
void update_gadgebot_damage_target_and_glow(struct Moby *moby) {
    SentryVars_c100 *sentry_vars = (SentryVars_c100 *)moby->pvars;
    union {
        u128  q;
        float f[4];
    } v;
    int   hit;
    float dmg;
    float angle;
    float speed;
    char *coll;
    int   reaction_value_or_path_address; /* the reaction kind, later the path */
    int fall_setup_value_or_path_slot_address; /* the reaction flags, later the path's slot in the table */

    if (moby->state == 0 || moby->state == 8) {
        return;
    }
    if (moby->state != 7 && game_frame != gadgebot_joint_glows_last_queued_frame_gp) {
        gadgebot_joint_glows_last_queued_frame_gp = game_frame;
        add_draw_callback(draw_group_gadgebot_joint_glows, moby);
    }
    speed = gadgebot_glow_default_phase_speed_degrees_per_second_gp;
    if (moby->state == 4 || moby->state == 5) {
        speed = gadgebot_glow_state_4_or_5_phase_speed_degrees_per_second_gp;
    }
    sentry_vars->spin = fast_add_rotations(sentry_vars->spin, speed * DEG_TO_RAD * frame_time);
    moby->glow_color = fast_tween_color(gadgebot_glow_tint_start_gp,
                                        gadgebot_glow_tint_end_gp,
                                        fast_sin(sentry_vars->spin) * 0.5f + 0.5f);
    if (moby->pos.z < 100.0f && moby->state != 7) {
        moby->state = 6;
    }
    moby->scale = moby->pclass->scale * D_L18_001625FC;
    if (hero.state.current == 0x72 || D_L18_0015F5C4 == 2) {
        if (moby->state != 7) {
            moby->state = 8;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
            moby->collision = 0;
            moby->flags = (moby->flags | 0x41) & ~0x1000;
            return;
        }
    }
    dmg = 0.0f;
    coll = find_moby_hit(moby, 0x330000, 0);
    fun_00213928(moby, coll, &sentry_vars->health, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && moby->state != 6 && dmg != 0.0f) {
        sentry_vars->health -= dmg;
        if (sentry_vars->health <= 0.0f) {
            if (random_integer_below(2) != 0) {
                moby->state = 6;
            } else {
                fall_setup_value_or_path_slot_address = 0x200;
                sentry_vars->unk90 = fall_setup_value_or_path_slot_address;
                sentry_vars->unk98 = 0.5f;
                sentry_vars->unk80 = gadgebot_death_fall_gravity_gp * frame_time_sq;
                sentry_vars->unk84 =
                    gadgebot_death_fall_horizontal_speed_approach_rate_gp * frame_time_sq;
                sentry_vars->unk88 = gadgebot_death_fall_base_horizontal_speed_gp * frame_time;
                sentry_vars->unk8C = gadgebot_death_fall_base_vertical_speed_gp * frame_time;
                reaction_value_or_path_address = 0x29;
                sentry_vars->unk94 = reaction_value_or_path_address;
                sentry_vars->unkAD = 0;
                sentry_vars->unkBC = frame_time + frame_time;
                moby->flags &= ~0x1000;
                v.q = *(u128 *)(coll + 0x10);
                trooper_knockback_arc(&v, &angle, &sentry_vars->unk88, &sentry_vars->unk8C);
                trooper_start_knockback(moby, sentry_vars->fall, 3, 1, 0, angle);
                sentry_vars->unkC0 = 7.5f;
                sentry_vars->unkC4 = 15.0f;
                moby->state = 5;
                moby->collision = 0;
                sentry_vars->anim_flags = 0x78;
                trooper_anim_update(moby, sentry_vars->anim);
            }
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, sentry_vars->anim);
    fall_setup_value_or_path_slot_address = (int)&D_L18_001B0EB0[sentry_vars->path];
    reaction_value_or_path_address = *(int *)fall_setup_value_or_path_slot_address;
    FUN_L00_0025ff38_c100(512.0f,
                          moby,
                          &sentry_vars->target,
                          0,
                          0,
                          (int *)(reaction_value_or_path_address + 0x10),
                          *(int *)reaction_value_or_path_address);
    if (sentry_vars->target_mode == 2) {
        if (hero.state.control_mode == 0xF) {
            int *list = (int *)hero.rail_spline;
            qcopy(&sentry_vars->target, (char *)list + *list * 16);
        } else {
            qcopy(&sentry_vars->target, &hero.motion.pos);
        }
    }
    if (sentry_vars->target_moby == 0) {
        sentry_vars->target_moby = hero.moby;
    }
}

/* Turns and accelerates the moby toward a target point and returns the remaining distance. */
typedef struct {
    float v[4];
} __attribute__((aligned(16))) QVec;

extern f32  advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern void FUN_L00_00258830(int, int, float, float, float, int);
extern void
resolve_point_path_segment_xy_clearance(float, void *, void *, void *) __asm__("FUN_L00_00261d78");

float move_gadgebot_toward_target(char *self, float *target_position_source) {
    QVec  target_position;
    float v[4];
    float movement_distance_scratch[4];
    float z = 0.0f;
    float movement_clearance = 0.5f;
    char *moby_vars = *(char **)(self + 0x78);
    float initial_target_distance_xy;
    *(u128 *)&target_position = *(u128 *)target_position_source;
    {
        float d = fast_add_rotations(fast_arc_tan(target_position.v[0] - *(float *)(self + 0x10),
                                                  target_position.v[1] - *(float *)(self + 0x14)),
                                     *(float *)(moby_vars + 0x1EC));
        approach_angle((float *)(self + 0x48),
                       (float *)(moby_vars + 0x1E8),
                       d,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 6.2831855f);
    }
    movement_distance_scratch[0] = z;
    initial_target_distance_xy = vector_distance_xy((float *)(self + 0x10), target_position.v);
    advance_accelerated_scalar(movement_distance_scratch,
                               (float *)(moby_vars + 0x1F0),
                               initial_target_distance_xy,
                               frame_time_sq * 6.0f,
                               frame_time_sq * 6.0f,
                               frame_time * 5.0f);
    v[0] = fast_cos(*(float *)(self + 0x48)) * *(float *)(moby_vars + 0x1F0);
    v[1] = fast_sin(*(float *)(self + 0x48)) * *(float *)(moby_vars + 0x1F0);
    v[2] = -(frame_time + frame_time);
    FUN_L00_00258830((int)self, (int)v, movement_clearance, movement_clearance, z, 0);
    resolve_point_path_segment_xy_clearance(
        movement_clearance, *(void **)(moby_vars + 0x1E0), self + 0x10, self + 0x10);
    return initial_target_distance_xy;
}

extern struct Moby *moby_table __asm__("D_L18_0015FFD8");

/* Pvars fields set when a group moby is launched. */
typedef struct {
    u8  pad0[0x1D0];
    f32 velocity[4]; /* 0x1D0: launch direction scaled to speed; [2] replaced by the arc speed */
    u8  pad1E0[0xC];
    f32 spin; /* 0x1EC: random 15..45 degrees, in radians */
    u8  pad1F0[4];
    f32 unk1F4; /* 0x1F4: set to 1.0 on launch */
} LaunchedMobyVars;

extern float FUN_L00_0025abf0(void *, void *, float, float, int);

/* Launches the first idle (state 8) class-0x772 moby of group `group` from `from` towards `to` at `speed`.
   Returns 1 when one was launched. */
int launch_first_idle_group_class_0772_moby(int   group,
                                            void *from,
                                            void *to,
                                            float speed) __asm__("FUN_L18_002fc668");

int launch_first_idle_group_class_0772_moby(int group, void *from, void *to, float speed) {
    unsigned short *group_moby_entry = (unsigned short *)D_L18_001AC240[group];
    struct Moby    *table;
    struct Moby    *moby;

    if (group_moby_entry == 0)
        return 0;
    table = moby_table;
    for (;;) {
        moby = &table[*group_moby_entry & 0x7FFF];
        if (moby->oclass == 0x772) {
            if (moby->state == 8) {
                LaunchedMobyVars *launched_moby_vars = (LaunchedMobyVars *)moby->pvars;
                moby->flags &= ~0x41;
                moby->collision = moby->pclass->collision;
                moby->flags |= 0x1000;
                qcopy(&moby->pos, from);
                moby->state = 1;
                launched_moby_vars->unk1F4 = 1.0f;
                fast_vec_sub(launched_moby_vars->velocity, to, from);
                launched_moby_vars->velocity[2] = 0.0f;
                fast_vec_normalize(
                    launched_moby_vars->velocity, launched_moby_vars->velocity, speed);
                launched_moby_vars->velocity[2] =
                    FUN_L00_0025abf0(from, to, speed, -(frame_time_sq * 10.0f), 0);
                launched_moby_vars->spin = randf_sym(15.0f, 45.0f) * 0.017453292f;
                return 1;
            }
        }
        if ((short)*group_moby_entry++ < 0)
            return 0;
    }
}
/* Counts the listed mobys of class 0x772 that are not in state 8. */

short count_group_gadgebots_not_in_state_8(int group_index) __asm__("FUN_L18_002fc7e0");

short count_group_gadgebots_not_in_state_8(int group_index) {
    unsigned short *group_moby_entry = (unsigned short *)D_L18_001AC240[group_index];
    short           n = 0;
    int             base;
    if (group_moby_entry == 0)
        return 0;
    base = D_L18_0015FFD8;
    while (1) {
        unsigned char *moby = (unsigned char *)(base + ((group_moby_entry[0] & 0x7FFF) << 8));
        if (MOBY(moby)->oclass == 0x772 && moby[0x20] != 8)
            n++;
        if ((short)*group_moby_entry++ < 0)
            return n;
    }
    return n;
}

/* Draws the moby's glow as a camera-facing quad sized by its charge, coloured between two tints. */

typedef struct {
    float         m[4][4];
    int           col[4];
    float         uv[4][2];
    unsigned long a, b, c, d;
} Pk_c860;

extern char D_L18_001677C0_g[] __asm__("D_L18_001677C0");

extern int gadgebot_launch_glow_blend_minuend_selector_gp __asm__("D_L18_0016262C")
    __attribute__((sda));
extern int gadgebot_launch_glow_blend_subtrahend_selector_gp __asm__("D_L18_00162630")
    __attribute__((sda));
extern int gadgebot_launch_glow_blend_alpha_factor_selector_gp __asm__("D_L18_00162634")
    __attribute__((sda));
extern int gadgebot_launch_glow_blend_addend_selector_gp __asm__("D_L18_00162638")
    __attribute__((sda));
extern int   gadgebot_launch_glow_tint_start_gp __asm__("D_L18_0016263C") __attribute__((sda));
extern int   gadgebot_launch_glow_tint_end_gp __asm__("D_L18_00162640") __attribute__((sda));
extern int   gadgebot_launch_glow_texture_index_gp __asm__("D_L18_00162644") __attribute__((sda));
extern float gadgebot_launch_glow_z_offset_gp __asm__("D_L18_00162648") __attribute__((sda));
extern float gadgebot_launch_glow_vertical_scale_adjustment_gp __asm__("D_L18_0016264C")
    __attribute__((sda));
extern float gadgebot_launch_glow_horizontal_scale_adjustment_gp __asm__("D_L18_00162650")
    __attribute__((sda));
extern float gadgebot_launch_glow_quad_uvs[4][2] __asm__("D_L18_001EFCD0");

void draw_gadgebot_launch_glow_quad(struct Moby *m) {
    char   *d = (char *)m->pvars;
    Pk_c860 pk;
    float   quad_transform[4][4];
    float   horizontal_axis_scale;
    float   vertical_axis_scale;
    int     c;
    int     i;
    float  *quad_vertex;
    vertical_axis_scale =
        *(float *)(d + 0x1F4) + 1.0f + gadgebot_launch_glow_vertical_scale_adjustment_gp;
    horizontal_axis_scale =
        1.0f - *(float *)(d + 0x1F4) + gadgebot_launch_glow_horizontal_scale_adjustment_gp;
    qcopy(quad_transform[3], &m->pos);
    quad_transform[3][2] -= gadgebot_launch_glow_z_offset_gp;
    clear_u64_value(quad_transform[2]);
    quad_transform[2][2] = vertical_axis_scale;
    fast_vec_sub(quad_transform[0], D_L18_001677C0_g, &m->pos);
    fast_vec_cross(quad_transform[1], quad_transform[0], quad_transform[2]);
    fast_vec_cross(quad_transform[0], quad_transform[1], quad_transform[2]);
    fast_vec_normalize(quad_transform[0], quad_transform[0], horizontal_axis_scale);
    fast_vec_normalize(quad_transform[1], quad_transform[1], horizontal_axis_scale);
    pk.b = get_effect_texture(gadgebot_launch_glow_texture_index_gp);
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 0;
    pk.d = (unsigned long)gadgebot_launch_glow_blend_minuend_selector_gp |
           ((unsigned long)gadgebot_launch_glow_blend_subtrahend_selector_gp << 2) |
           ((unsigned long)gadgebot_launch_glow_blend_alpha_factor_selector_gp << 4) |
           ((unsigned long)gadgebot_launch_glow_blend_addend_selector_gp << 6) | 0x8000000000ULL;
    c = fast_tween_color(gadgebot_launch_glow_tint_start_gp,
                         gadgebot_launch_glow_tint_end_gp,
                         *(float *)(d + 0x1F4));
    for (i = 0; i < 4; i++) {
        pk.uv[i][0] = gadgebot_launch_glow_quad_uvs[i][0];
        pk.uv[i][1] = gadgebot_launch_glow_quad_uvs[i][1];
        quad_vertex = pk.m[i];
        clear_u64_value(quad_vertex);
        if (i & 1)
            quad_vertex[2] = 1.0f;
        else
            quad_vertex[2] = 0.0f;
        if (i < 2)
            quad_vertex[1] = 1.0f;
        else
            quad_vertex[1] = -1.0f;
        quad_vertex[3] = 1.0f;
        pk.col[i] = c;
    }
    draw_geometry_quad(&pk, quad_transform, 0);
}

/* For each listed moby of matching type, spawns effects at five points. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FDF58), where it is exact; names translated to the US level program. */

extern short           gadgebot_joint_glow_alpha_gp __asm__("D_L18_00162654");
extern unsigned short *D_L18_001AC240_c[] __asm__("D_L18_001AC240");

void draw_group_gadgebot_joint_glows(struct Moby *moby) {
    unsigned short *group_moby_entry = D_L18_001AC240_c[moby->group];
    if (group_moby_entry != 0) {
        while (1) {
            unsigned char  *group_moby = D_L18_0015FFD8 + ((group_moby_entry[0] & 0x7FFF) << 8);
            unsigned short *next = group_moby_entry + 1;
            if (MOBY(group_moby)->oclass == 0x772 && group_moby[0x20] != 0 &&
                group_moby[0x20] < 6) {
                int i;
                int col = (MOBY(group_moby)->glow_color & 0xFFFFFF) +
                          (*(int *)&gadgebot_joint_glow_alpha_gp << 24);
                float joint_world_position[4];
                for (i = 2; i < 5; i++) {
                    moby_point_position(group_moby, i, joint_world_position);
                    draw_offset_textured_effect_quad(joint_world_position, col, 0.175f, 0.08f);
                }
            }
            if (*(short *)group_moby_entry < 0)
                break;
            group_moby_entry = next;
        }
    }
}

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

int update_moby_suck_cannon_state(struct Moby *a) __asm__("FUN_L18_002fcbb0");

/* Method: holds the gadgebot in state 7 while the suck cannon pulls it, and puts it back to state 2 when it lets go. */
int update_moby_suck_cannon_state(struct Moby *moby) {
    int suction_result = process_moby_suck_cannon_suction(moby);
    if (suction_result == 0) {
        if (moby->state == 0x7)
            moby->state = 0x2;
    } else {
        moby->state = 0x7;
    }
    return suction_result;
}

void hide_moby_and_clear_collision(struct Moby *moby) __asm__("FUN_L18_002fcd00");

/* Method: sends the gadgebot back to its idle state 8, hidden and without collision. */
void hide_moby_and_clear_collision(struct Moby *moby) {
    unsigned short flags;
    moby->state = 0x8;
    if (moby->prev_seq != 0) {
        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
    }
    flags = moby->flags;
    moby->visible = 0;
    flags |= 1;
    moby->collision = 0;
    flags &= 0xEFFF;
    moby->flags = flags;
}

float gadgebot_launch_glow_quad_uvs[4][2] __asm__("D_L18_001EFCD0") = {
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
};

short gadgebot_joint_glow_alpha_gp __asm__("D_L18_00162654") = 0x20;

int gadgebot_launch_glow_tint_end_gp __asm__("D_L18_00162640") = 0x8000ffff;

int gadgebot_launch_glow_texture_index_gp __asm__("D_L18_00162644") = 6;
