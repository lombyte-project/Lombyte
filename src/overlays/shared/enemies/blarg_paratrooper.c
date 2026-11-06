/* Shared moby class blarg_paratrooper. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/mobys/resolve_moby_hit_response.h"
#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/overlay/shared/mobys/ground_move_control.h"
#include "rnc/overlay/shared/mobys/knockback_motion.h"

typedef struct Moby Moby;

typedef struct {
    char  pad0[0x80];
    float pos[4];
    char  pad90[0x108];
    int   timer;
    char  pad19c[0x1EE8];
    int   mode;
} Hero302618;

typedef struct Paratrooper302618 {
    u8                       pad0[0x20];
    float                    health; /* 0x20: hit damage is taken from it */
    short                    max_health;
    u8                       pad26[0x2];
    unsigned char            hit_response_class;
    unsigned char            f29;
    u8                       pad2A[0x4];
    unsigned char            removal_status;
    u8                       pad2F[0x9];
    s32                      hurt; /* 0x38: set when the moby is hit, cleared once handled */
    u8                       pad3C[0x1c];
    unsigned char            f58;
    u8                       pad59[0x1];
    unsigned char            f5A;
    u8                       pad5B[0x5];
    struct HitColorFlash     hit_flash;
    struct KnockbackMotion   knockback;    /* 0x70 */
    struct GroundMoveControl move_control; /* 0xD0 */
    u8                       path_search_point[0x8];
    f32                      path_search_point_z;
    u8                       pad12C[0x34];
    char                    *target_moby;
    int                      target_selection_status;
    u8                       pad168[0x68];
    int                      wake_path_index;
    int                      jump_path_index;
    s16                      target_range_boost_timer;
    short                    hop_count;
    f32                      target_range;
    u8                       pad1E0[0x4];
    f32                      base_target_range;
    float                    projectile_pitch_divisor;
    int                      projectile_fire_timer;
    int                      waypoint_transition_timer;
    int                      destination_waypoint_index;
    char                    *bone_attachment_moby;
    int                      patrol_path_index;
    u8                       pad200[0xc];
    short                    patrol_waypoint_step;
    short                    projectile_pattern_index;
} Paratrooper302618;

extern float D_0015ED6C;
extern float D_0015ED70;
extern void  FUN_L00_00257470(void *, int, int);

extern float scale_time(float) __asm__("FUN_001f96b0");

char *spawn_paratrooper_projectile(char *pos, char *dir, int a2, int a3, int a4, int a5) __asm__("FUN_L08_00307190");
extern char  D_L08_001675C0[];
extern char  paratrooper_invalid_jump_path_format[] __asm__("D_L08_001FC630");
extern float D_L08_0015F580[4] MACRO_ADDR;
extern float D_L08_0015F5D0;
extern float D_L08_0015F5D4;
void         FUN_L00_0025f730(void *a, float x);
void         update_blarg_paratrooper_damage_and_state(char *m) __asm__("FUN_L08_003024d8");
extern char  D_0013F350[];
extern int   level_game_mode __asm__("D_L08_0015F5C4");
extern char *target302618 __asm__("D_L08_00174658") NOT_SDA;
float        speed302618 __asm__("D_L08_001621C4") = 2e+01f;
float        accel302618 __asm__("D_L08_001621C0") = 1.0f;
int          delay302618 __asm__("D_L08_001621BC") = 5;
int          hops302618 __asm__("D_L08_001621CC") = 5;
float        melee302618 __asm__("D_L08_001621C8") = 3.3f;
float        height302618 __asm__("D_L08_001621D0") = 2.0f;
float        shove302618 __asm__("D_L08_001621B4") = 0.6f;
float        shove_rate302618 __asm__("D_L08_001621B8") = 0.6f;
extern int   random302618(void) __asm__("FUN_001160d8");
extern float turn302618(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");
extern void  FUN_L00_00263ac8_c(float, void *, int, void *) __asm__("FUN_L00_00263ac8");
extern void  FUN_L00_0025a120_c(void *) __asm__("FUN_L00_0025a120");
extern void  debug_print(char *, int) __asm__("FUN_001e93b0");
extern void  step_mover_2e3f20(void *, void *, void *, void *) __asm__("FUN_L00_00259028");
extern void  FUN_L00_0025f730(void *, float);
extern void
           FUN_L00_00258830_c(void *, void *, float, float, float, int) __asm__("FUN_L00_00258830");
extern int damage302618(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void *
FUN_L00_00263fd8_c(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");

extern char paratrooper_invalid_wake_path_format[] __asm__("D_L08_001FC600");

extern void trooper_knockback_arc(void *, void *, float *, float *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void spawn_randomized_bolt_drop(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
extern void start_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d458");
extern void update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern int
activate_effect_l08(void *, void *, float, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern float l08_paratrooper_model_scale_multiplier __asm__("D_L08_00162180") __attribute__((sda));
extern float D_L08_00162184 __attribute__((sda));
extern float l08_paratrooper_recovery_knockback_xy_speed __asm__("D_L08_0016218C")
    __attribute__((sda));
extern float l08_paratrooper_death_knockback_z_speed __asm__("D_L08_00162190") __attribute__((sda));
extern float l08_paratrooper_death_knockback_xy_speed __asm__("D_L08_00162194")
    __attribute__((sda));
extern float D_L08_001621B0 __attribute__((sda));

/* Paratrooper: initializes its path and steering, patrols, attacks its target,
 * and handles hit recovery and death. Shared by Batalia and Gaspar. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00302618), where it is exact; names translated to the US level program. */

void blarg_paratrooper_update(char *moby) __asm__("FUN_L08_00301158");

/* Moby class 1023 */
void blarg_paratrooper_update(char *moby) {
    enum {
        PARATROOPER_STATE_INITIALIZE = 0,
        PARATROOPER_STATE_PATROL_PATH = 1,
        PARATROOPER_STATE_APPROACH_WAYPOINT = 2,
        PARATROOPER_STATE_WAIT_FACING_TARGET = 3,
        PARATROOPER_STATE_HOP_BETWEEN_WAYPOINTS = 4,
        PARATROOPER_STATE_FIRE_PROJECTILES = 5,
        PARATROOPER_STATE_MOVE_TOWARD_FIRST_WAYPOINT = 6,
        PARATROOPER_STATE_MELEE_ATTACK = 7,
        PARATROOPER_STATE_WAIT_FOR_HERO_TRIGGERED_ANIMATION = 8,
        PARATROOPER_STATE_PAUSE_AFTER_HOP_LIMIT = 9,
        PARATROOPER_STATE_KNOCKBACK_RECOVERY = 10,
        PARATROOPER_STATE_DEATH_EFFECTS_AND_REMOVE = 11
    };
    enum {
        PARATROOPER_TARGET_UNAVAILABLE = 2
    };
    Paratrooper302618 *paratrooper_private_state =
        (Paratrooper302618 *)(*(char **)((char *)(moby) + 0x78));
    char       *path;
    char       *target;
    Hero302618 *hero;
    float       pos[4] __attribute__((aligned(16)));
    float       attack_vector_or_ray_endpoint[4] __attribute__((aligned(16)));
    char        hit[0x30] __attribute__((aligned(16)));
    if ((*(unsigned char *)((char *)(moby) + 0x31)) == 0 &&
        fast_vec_dist(&MOBY(moby)->pos, D_L08_001675C0) > 40.0f)
        return;
    FUN_L00_00263ac8_c(2.5f, moby, 6, (char *)paratrooper_private_state + 0x170);
    if ((*(unsigned char *)((char *)(moby) + 0x31)) != 0 &&
        fast_vec_dist(&MOBY(moby)->pos, D_L08_001675C0) < 29.0f) {
        FUN_L00_0025a120_c(moby);
        (*(unsigned char *)((char *)(moby) + 0x7F)) = 23;
    }
    if (level_game_mode == 2)
        return;
    path = 0;
    update_blarg_paratrooper_damage_and_state(moby);
    target = paratrooper_private_state->target_moby;
    if (paratrooper_private_state->patrol_path_index != -1)
        path = ((char *)l08_paths[paratrooper_private_state->patrol_path_index]);
    hero = (Hero302618 *)(((char *)&D_0013F350));
    if (hero->mode == 61 && hero->timer < scale_game_frames(30) &&
        (*(unsigned char *)((char *)(moby) + 0x20)) != 8 &&
        (*(unsigned char *)((char *)(moby) + 0x20)) != 10 &&
        (*(unsigned char *)((char *)(moby) + 0x20)) != 4 &&
        (*(unsigned char *)((char *)(moby) + 0x20)) != 11 &&
        vector_distance_xy(&MOBY(moby)->pos, hero->pos) < 25.0f && !random_integer_below(49)) {
        if ((*(unsigned char *)((char *)(moby) + 0x53)) != 14)
            blend_moby_animation(moby, 14, 0, scale_game_frames(11));
        (*(unsigned char *)((char *)(moby) + 0x20)) = 8;
    }
    if ((*(unsigned char *)((char *)(moby) + 0x20)) != 0 &&
        paratrooper_private_state->removal_status == 2) {
        mark_moby_for_removal(moby);
        if (paratrooper_private_state->bone_attachment_moby)
            mark_moby_for_removal(paratrooper_private_state->bone_attachment_moby);
        return;
    }
    switch ((*(unsigned char *)((char *)(moby) + 0x20))) {
    case PARATROOPER_STATE_INITIALIZE: {
        float turn_rate;
        paratrooper_private_state->f29 = 0;
        paratrooper_private_state->hit_response_class = 1;
        paratrooper_private_state->removal_status = 1;
        paratrooper_private_state->health = 2.0f;
        paratrooper_private_state->max_health = 2;
        paratrooper_private_state->f58 = truncate_float_to_s32(15.0f);
        paratrooper_private_state->f5A = truncate_float_to_s32(12.0f);
        if (paratrooper_private_state->wake_path_index == -1) {
            debug_print(paratrooper_invalid_wake_path_format, (*(short *)((char *)(moby) + 0xB2)));
            goto delete_moby;
        }
        if (paratrooper_private_state->jump_path_index == -1) {
            debug_print(paratrooper_invalid_jump_path_format, (*(short *)((char *)(moby) + 0xB2)));
            goto delete_moby;
        }
        init_ground_move_control(&paratrooper_private_state->move_control);
        paratrooper_private_state->move_control.max_climb = 0.5f;
        paratrooper_private_state->move_control.max_drop = 0.5f;
        paratrooper_private_state->move_control.collision_radius_1024ths =
            truncate_float_to_s32(512.0f);
        turn_rate = D_0015ED70 * 6.981317f;
        paratrooper_private_state->move_control.turn_damping =
            paratrooper_private_state->move_control.turn_spring = turn_rate;
        paratrooper_private_state->move_control.max_turn_step = D_0015ED6C * 8.726646f;
        paratrooper_private_state->move_control.heading_tolerance = 0.34906584f;
        paratrooper_private_state->move_control.arrive_distance = 0.2f;
        if (random302618() & 1)
            (*(unsigned short *)((char *)(moby) + 0x34)) |= 0x8000;
        qcopy(&MOBY(moby)->pos,
              ((char *)l08_paths[paratrooper_private_state->jump_path_index]) + 0x10);
        if (path) {
            int   index;
            short direction;
            paratrooper_private_state->patrol_waypoint_step = 2 * (random302618() & 1) - 1;
            index = select_point_index_by_distance(&MOBY(moby)->pos, path, 0.0f) +
                    paratrooper_private_state->patrol_waypoint_step;
            direction = paratrooper_private_state->patrol_waypoint_step;
            paratrooper_private_state->destination_waypoint_index = index;
            if (index >= (*(int *)((char *)(path) + 0)) || index < 0) {
                paratrooper_private_state->patrol_waypoint_step = -direction;
                paratrooper_private_state->destination_waypoint_index =
                    index + 2 * paratrooper_private_state->patrol_waypoint_step;
            }
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 2)
                blend_moby_animation(moby, 2, 0, random302618() % 4 + 7);
            (*(unsigned char *)((char *)(moby) + 0x20)) = 1;
        } else
            (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
        paratrooper_private_state->hop_count = 0;
        break;
    }
    case PARATROOPER_STATE_PATROL_PATH:
        if (vector_distance_xy(&MOBY(moby)->pos,
                               path + 0x10 +
                                   paratrooper_private_state->destination_waypoint_index * 16) <
            paratrooper_private_state->move_control.speed) {
            short direction = paratrooper_private_state->patrol_waypoint_step;
            int   index = paratrooper_private_state->destination_waypoint_index + direction;
            paratrooper_private_state->destination_waypoint_index = index;
            if (index >= (*(int *)((char *)(path) + 0)) || index < 0) {
                paratrooper_private_state->move_control.speed = 0.0f;
                paratrooper_private_state->patrol_waypoint_step = -direction;
                paratrooper_private_state->destination_waypoint_index =
                    index + 2 * paratrooper_private_state->patrol_waypoint_step;
            }
        }
        step_mover_2e3f20(moby,
                          &paratrooper_private_state->move_control,
                          path + 0x10 + paratrooper_private_state->destination_waypoint_index * 16,
                          pos);
        if (target &&
            paratrooper_private_state->target_selection_status != PARATROOPER_TARGET_UNAVAILABLE &&
            fast_dec_timer((char *)paratrooper_private_state + 0x1F0)) {
            paratrooper_private_state->destination_waypoint_index = select_point_index_by_distance(
                &MOBY(moby)->pos,
                ((char *)l08_paths[paratrooper_private_state->jump_path_index]),
                0.0f);
            (*(unsigned char *)((char *)(moby) + 0x20)) = 2;
        }
        break;
    case PARATROOPER_STATE_APPROACH_WAYPOINT: {
        float distance;
        paratrooper_private_state->destination_waypoint_index = select_point_index_by_distance(
            &MOBY(moby)->pos,
            ((char *)l08_paths[paratrooper_private_state->jump_path_index]),
            0.0f);
        distance = vector_distance_xy(
            &MOBY(moby)->pos,
            ((char *)l08_paths[paratrooper_private_state->jump_path_index]) + 0x10 +
                paratrooper_private_state->destination_waypoint_index * 16);
        if (distance < 1.0f || distance < paratrooper_private_state->move_control.speed) {
            paratrooper_private_state->move_control.speed = 0.0f;
            (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
        } else {
            step_mover_2e3f20(moby,
                              &paratrooper_private_state->move_control,
                              ((char *)l08_paths[paratrooper_private_state->jump_path_index]) +
                                  0x10 + paratrooper_private_state->destination_waypoint_index * 16,
                              pos);
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 2)
                blend_moby_animation(moby, 2, 0, random302618() % 4 + 7);
        }
        break;
    }
    case PARATROOPER_STATE_WAIT_FACING_TARGET: {
        float *rotation = (float *)&MOBY(moby)->rot.z;
        float  heading;
        float  frame;
        heading = fast_arc_tan(
            (*(float *)((char *)(target) + 0x10)) - (*(float *)((char *)(moby) + 0x10)),
            (*(float *)((char *)(target) + 0x14)) - (*(float *)((char *)(moby) + 0x14)));
        turn302618(rotation,
                   heading,
                   (char *)paratrooper_private_state + 0x1E0,
                   D_0015ED70 * 8.726646f,
                   D_0015ED70 * 8.726646f,
                   D_0015ED6C * 12.566371f);
        if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2) {
            int oldanim = (*(unsigned char *)((char *)(moby) + 0x53));
            if (oldanim != (random302618() & 1)) {
                int newanim = random302618() & 1;
                int ticks = scale_game_frames(7);
                blend_moby_animation(moby, newanim, 0, ticks + random302618() % 7);
            }
        }
        if (hops302618 < paratrooper_private_state->hop_count) {
            paratrooper_private_state->hop_count = 0;
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 10) {
                int ticks = scale_game_frames(7);
                blend_moby_animation(
                    moby, 10, 2, ticks + random_int_between(0, scale_game_frames(5)));
            }
            (*(unsigned char *)((char *)(moby) + 0x20)) = 9;
        } else if (target) {
            if (paratrooper_private_state->target_selection_status !=
                    PARATROOPER_TARGET_UNAVAILABLE &&
                fast_dec_timer((char *)paratrooper_private_state + 0x1F0)) {
                qcopy((char *)paratrooper_private_state + 0x1C0, &MOBY(moby)->pos);
                paratrooper_private_state->destination_waypoint_index =
                    (paratrooper_private_state->destination_waypoint_index + 1) & 1;
                (*(unsigned char *)((char *)(moby) + 0x20)) = 4;
                fast_vec_sub(pos,
                             ((char *)l08_paths[paratrooper_private_state->jump_path_index]) +
                                 0x10 + paratrooper_private_state->destination_waypoint_index * 16,
                             &MOBY(moby)->pos);
                if (fast_vec_dot(moby + 0xD0, pos) < 0.0f) {
                    if ((*(unsigned char *)((char *)(moby) + 0x53)) != 18)
                        blend_moby_animation(moby, 18, 0, scale_game_frames(5));
                } else if ((*(unsigned char *)((char *)(moby) + 0x53)) != 16)
                    blend_moby_animation(moby, 16, 0, scale_game_frames(5));
            } else if (target &&
                       vector_distance_xy(&MOBY(moby)->pos, target + 0x10) < melee302618) {
                (*(unsigned char *)((char *)(moby) + 0x20)) = 7;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 13)
                    blend_moby_animation(moby, 13, 0, scale_game_frames(10));
            }
        }
        break;
    }
    case PARATROOPER_STATE_HOP_BETWEEN_WAYPOINTS: {
        float *rotation = (float *)&MOBY(moby)->rot.z;
        float  heading;
        float  frame;
        heading = fast_arc_tan(
            (*(float *)((char *)(target) + 0x10)) - (*(float *)((char *)(moby) + 0x10)),
            (*(float *)((char *)(target) + 0x14)) - (*(float *)((char *)(moby) + 0x14)));
        turn302618(rotation,
                   heading,
                   (char *)paratrooper_private_state + 0x1E0,
                   D_0015ED70 * 8.726646f,
                   D_0015ED70 * 8.726646f,
                   D_0015ED6C * 12.566371f);
        frame = compute_interpolated_record_value(moby);
        if (frame > 12.0f && frame < 23.0f &&
            (*(unsigned char *)((char *)(moby) + 0x52)) ==
                (*(unsigned char *)((char *)(moby) + 0x53))) {
            float progress = 1.0f - (23.0f - frame) / 11.0f;
            float arc;
            if (progress > 1.0f)
                progress = 1.0f;
            else if (progress < 0.0f)
                progress = 0.0f;
            lerp_vector(pos,
                        (char *)paratrooper_private_state + 0x1C0,
                        ((char *)l08_paths[paratrooper_private_state->jump_path_index]) + 0x10 +
                            paratrooper_private_state->destination_waypoint_index * 16,
                        progress);
            arc = 2.0f * (progress - 0.5f);
            pos[2] += height302618 * (1.0f - arc * arc);
            qcopy(&MOBY(moby)->pos, pos);
        } else if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2) {
            qcopy(pos, &MOBY(moby)->pos);
            qcopy(attack_vector_or_ray_endpoint, target + 0x10);
            pos[2] += 1.0f;
            attack_vector_or_ray_endpoint[2] += 0.75f;
            collision_line(pos, attack_vector_or_ray_endpoint, 0, moby, 0);
            if (hops302618 < paratrooper_private_state->hop_count) {
                paratrooper_private_state->hop_count = 0;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 10) {
                    blend_moby_animation(moby, 10, 2, random302618() % 4 + 5);
                }
                (*(unsigned char *)((char *)(moby) + 0x20)) = 9;
            } else if (vector_distance_xy(&MOBY(moby)->pos, target + 0x10) < melee302618) {
                goto melee_state;
            } else if (target302618 != target) {
                (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(5));
                paratrooper_private_state->waypoint_transition_timer =
                    truncate_float_to_s32(scale_time(random_float_between(45.0f, 90.0f)));
            } else {
                paratrooper_private_state->projectile_pitch_divisor = 1.0f;
                (*(unsigned char *)((char *)(moby) + 0x20)) = 5;
                paratrooper_private_state->projectile_pattern_index = 0;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 6)
                    blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    }
    case PARATROOPER_STATE_FIRE_PROJECTILES:
        if (vector_distance_xy(&MOBY(moby)->pos, target + 0x10) < melee302618) {
            (*(unsigned char *)((char *)(moby) + 0x20)) = 7;
            paratrooper_private_state->projectile_fire_timer = 0;
            if ((*(unsigned char *)((char *)(moby) + 0x53)) == 13)
                break;
            goto animate_melee;
        } else if (fast_dec_timer((char *)paratrooper_private_state + 0x1EC)) {
            paratrooper_private_state->projectile_fire_timer = scale_game_frames(
                delay302618 + truncate_float_to_s32(D_L08_0015F5D4 * 1.3f + D_L08_0015F5D0));
            paratrooper_private_state->projectile_pitch_divisor += accel302618;
            if ((D_L08_0015F5D4 < 0.8f && D_L08_0015F5D0 < 0.8f) ||
                !((*(unsigned short *)&paratrooper_private_state->projectile_pattern_index) & 1)) {
                char *shot;
                moby_point_position(paratrooper_private_state->bone_attachment_moby, 0, pos);
                attack_vector_or_ray_endpoint[0] =
                    fast_cos((*(float *)((char *)(moby) + 0x48))) * (speed302618 * D_0015ED6C);
                attack_vector_or_ray_endpoint[1] =
                    fast_sin((*(float *)((char *)(moby) + 0x48))) * (speed302618 * D_0015ED6C);
                attack_vector_or_ray_endpoint[2] = 0.0f;
                attack_vector_or_ray_endpoint[2] =
                    (-1.0f / paratrooper_private_state->projectile_pitch_divisor) *
                    (speed302618 * D_0015ED6C);
                shot = spawn_paratrooper_projectile(
                    attack_vector_or_ray_endpoint,
                    pos,
                    moby,
                    target,
                    scale_game_frames(180),
                    paratrooper_private_state->projectile_pattern_index);
                if (shot)
                    (*(float *)((char *)(shot) + 0x2C)) *= 1.7f;
            }
            paratrooper_private_state->projectile_pattern_index =
                (*(unsigned short *)&paratrooper_private_state->projectile_pattern_index) + 1;
            if (paratrooper_private_state->projectile_pattern_index >= 5)
                paratrooper_private_state->projectile_pattern_index = 0;
            if (paratrooper_private_state->projectile_pitch_divisor > 20.0f) {
                qcopy((char *)paratrooper_private_state + 0x1C0, &MOBY(moby)->pos);
                (*(unsigned short *)&paratrooper_private_state->hop_count)++;
                paratrooper_private_state->destination_waypoint_index =
                    (paratrooper_private_state->destination_waypoint_index + 1) & 1;
                (*(unsigned char *)((char *)(moby) + 0x20)) = 4;
                fast_vec_sub(pos,
                             ((char *)l08_paths[paratrooper_private_state->jump_path_index]) +
                                 0x10 + paratrooper_private_state->destination_waypoint_index * 16,
                             &MOBY(moby)->pos);
                if (fast_vec_dot(moby + 0xD0, pos) < 0.0f) {
                    if ((*(unsigned char *)((char *)(moby) + 0x53)) != 18)
                        blend_moby_animation(moby, 18, 0, scale_game_frames(5));
                } else if ((*(unsigned char *)((char *)(moby) + 0x53)) != 16)
                    blend_moby_animation(moby, 16, 0, scale_game_frames(5));
            }
        }
        break;
    case PARATROOPER_STATE_MOVE_TOWARD_FIRST_WAYPOINT: {
        float *rotation = (float *)&MOBY(moby)->rot.z;
        float  heading;
        float  frame;
        heading = fast_arc_tan(
            (*(float *)((char *)(target) + 0x10)) - (*(float *)((char *)(moby) + 0x10)),
            (*(float *)((char *)(target) + 0x14)) - (*(float *)((char *)(moby) + 0x14)));
        turn302618(rotation,
                   heading,
                   (char *)paratrooper_private_state + 0x1E0,
                   D_0015ED70 * 8.726646f,
                   D_0015ED70 * 8.726646f,
                   D_0015ED6C * 12.566371f);
        frame = compute_interpolated_record_value(moby);
        if (frame > 12.0f && frame < 23.0f && (*(float *)((char *)(moby) + 0x54)) == 0.0f) {
            fast_vec_sub(pos,
                         ((char *)l08_paths[paratrooper_private_state->jump_path_index]) + 0x10,
                         &MOBY(moby)->pos);
            FUN_L00_0025f730(pos, D_0015ED6C * 24.0f);
            FUN_L00_00258830_c(moby, pos, shove302618, shove_rate302618, 0.0f, 0);
        } else if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2) {
            paratrooper_private_state->waypoint_transition_timer =
                truncate_float_to_s32(scale_time(random_float_between(45.0f, 90.0f)));
            (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    }
    case PARATROOPER_STATE_MELEE_ATTACK: {
        float *rotation = (float *)&MOBY(moby)->rot.z;
        float  heading;
        float  frame;
        heading = fast_arc_tan(
            (*(float *)((char *)(target) + 0x10)) - (*(float *)((char *)(moby) + 0x10)),
            (*(float *)((char *)(target) + 0x14)) - (*(float *)((char *)(moby) + 0x14)));
        turn302618(rotation,
                   heading,
                   (char *)paratrooper_private_state + 0x1E0,
                   D_0015ED70 * 8.726646f,
                   D_0015ED70 * 8.726646f,
                   D_0015ED6C * 12.566371f);
        frame = compute_interpolated_record_value(moby);
        if (frame > 16.5f && frame < 20.0f) {
            moby_point_position(moby, 4, pos);
            fast_vec_sub(attack_vector_or_ray_endpoint, pos, &MOBY(moby)->pos);
            attack_vector_or_ray_endpoint[2] = 0.0f;
            fast_vec_normalize(attack_vector_or_ray_endpoint, attack_vector_or_ray_endpoint, 1.0f);
            attack_vector_or_ray_endpoint[2] = 1.0f;
            pos[2] -= 0.25f;
            make_hit_descriptor(hit, moby, 1, 1.0f, attack_vector_or_ray_endpoint);
            damage302618(0.8f, pos, 0, moby, hit);
        }
        if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2)
            (*(unsigned char *)((char *)(moby) + 0x20)) = 2;
        break;
    }
    case PARATROOPER_STATE_WAIT_FOR_HERO_TRIGGERED_ANIMATION:
    case PARATROOPER_STATE_PAUSE_AFTER_HOP_LIMIT:
        if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2) {
            (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    case PARATROOPER_STATE_KNOCKBACK_RECOVERY: {
        int flags = step_moby_motion(moby, (char *)paratrooper_private_state + 0x70);
        if (flags & 0x40) {
            if (vector_distance_xy(&MOBY(moby)->pos, target + 0x10) < melee302618) {
            melee_state:
                (*(unsigned char *)((char *)(moby) + 0x20)) = 7;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 13) {
                animate_melee:
                    blend_moby_animation(moby, 13, 0, random302618() % 4 + 8);
                }
            } else {
                paratrooper_private_state->projectile_pitch_divisor = 1.0f;
                (*(unsigned char *)((char *)(moby) + 0x20)) = 5;
                paratrooper_private_state->projectile_pattern_index = 0;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 6) {
                    int ticks = scale_game_frames(10);
                    blend_moby_animation(
                        moby, 6, 0, ticks + random_int_between(0, scale_game_frames(7)));
                }
            }
        } else if (flags & 0x120) {
        delete_moby:
            mark_moby_for_removal(moby);
        }
    } break;
    case PARATROOPER_STATE_DEATH_EFFECTS_AND_REMOVE:
        if (step_moby_motion(moby, (char *)paratrooper_private_state + 0x70) & 0x160) {
            float *color;
            char  *rotation;
            clear_u64_value(pos);
            color = D_L08_0015F580;
            FUN_L00_00257470(moby, 0, -1);
            cart_burst(moby, &MOBY(moby)->pos, 0, 0.5f, 13.0f);
            rotation = moby + 0x40;
            FUN_L00_00263fd8_c(
                moby, 0x69C, &MOBY(moby)->pos, rotation, 0, 0, 0.0f, color, color, color);
            FUN_L00_00263fd8_c(
                moby, 0x69D, &MOBY(moby)->pos, rotation, 0, 0, 0.0f, color, color, color);
            FUN_L00_00263fd8_c(
                moby, 0x69E, &MOBY(moby)->pos, rotation, 0, 0, 0.0f, color, color, color);
            mark_moby_for_removal(moby);
            if (paratrooper_private_state->bone_attachment_moby)
                mark_moby_for_removal(paratrooper_private_state->bone_attachment_moby);
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00303998), where it is exact; names translated to the US level program. */

void update_blarg_paratrooper_damage_and_state(char *moby) {
    enum {
        PARATROOPER_HIT_REACTION_LETHAL = 1,
        PARATROOPER_HIT_REACTION_DEATH_KNOCKBACK = 2,
        PARATROOPER_HIT_REACTION_RECOVERY_KNOCKBACK = 3,
        PARATROOPER_HIT_REACTION_NONE = 11
    };
    enum {
        PARATROOPER_STATE_INITIALIZE = 0,
        PARATROOPER_STATE_HOP_BETWEEN_WAYPOINTS = 4,
        PARATROOPER_STATE_FIRE_PROJECTILES = 5,
        PARATROOPER_STATE_MELEE_ATTACK = 7,
        PARATROOPER_STATE_KNOCKBACK_RECOVERY = 10,
        PARATROOPER_STATE_DEATH_EFFECTS_AND_REMOVE = 11
    };
    enum {
        PARATROOPER_HIT_STATUS_NO_HIT_OR_SUPPRESSED = 1
    };
    Paratrooper302618 *paratrooper_instance_state;
    char              *target, *child, *attached, *chased;
    float              pos[4] __attribute__((aligned(16)));
    float              matrix[16] __attribute__((aligned(16)));
    float              damage, knock_angle, second_knock_angle;
    int                hit_state, reaction, turn, state;
    float              angle, distance, height, dx, dy;

    paratrooper_instance_state = (Paratrooper302618 *)MOBY(moby)->pvars;
    if (MOBY(moby)->state == PARATROOPER_STATE_INITIALIZE)
        return;
    {
        float timer_scale = D_L08_001621B0 * frame_time;
        ((Moby *)moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
                                l08_paratrooper_model_scale_multiplier;
        paratrooper_instance_state->move_control.max_speed = timer_scale;
    }

    if (paratrooper_instance_state->bone_attachment_moby == 0) {
        child = create_moby(0x401);
        paratrooper_instance_state->bone_attachment_moby = child;
        if (child != 0) {
            *(short *)(child + 0x32) = 0x40;
            *(unsigned char *)(paratrooper_instance_state->bone_attachment_moby + 0x30) = 0x40;
            *(unsigned char *)(paratrooper_instance_state->bone_attachment_moby + 0x31) = 1;
            *(unsigned short *)(paratrooper_instance_state->bone_attachment_moby + 0x34) |= 0x100;
            moby_point_position(moby, 4, pos);
            qcopy(paratrooper_instance_state->bone_attachment_moby + 0x10, pos);
        }
        MOBY(moby)->flags |= 0x1000;
    } else {
        moby_point_position(moby, 4, pos);
        build_moby_bone_transform(moby, 4, matrix);
        copy_matrix3x4(paratrooper_instance_state->bone_attachment_moby + 0xc0, matrix);
        attached = paratrooper_instance_state->bone_attachment_moby;
        qcopy(attached + 0x10, pos);
        update_entity(attached);
    }

    damage = 0.0f;
    target = find_moby_hit(moby, 0x330000, 0);
    reaction = resolve_moby_hit_response(
        moby, target, &paratrooper_instance_state->health, 0, &hit_state, &damage, 0, 4);
    if (hit_state != PARATROOPER_HIT_STATUS_NO_HIT_OR_SUPPRESSED &&
        MOBY(moby)->state != PARATROOPER_STATE_DEATH_EFFECTS_AND_REMOVE) {
        {
            float health = paratrooper_instance_state->health - damage;
            paratrooper_instance_state->health = health;
            if (*(short *)(*(char **)(target + 0x20) + 0xa6) == 0x47)
                reaction = 3;
            if (health <= 0.0f)
                reaction = 1;
        }

        turn = truncate_float_to_s32(512.0f);
        {
            float pose_scale = D_L08_00162184 * frame_time_sq;
            paratrooper_instance_state->knockback.collision_radius_1024ths = turn;
            paratrooper_instance_state->knockback.collision_z_offset = 0.5f;
            paratrooper_instance_state->knockback.flags = 9;
            paratrooper_instance_state->knockback.gravity = pose_scale;
        }
        paratrooper_instance_state->knockback.unk3D = 0;

        if ((unsigned)reaction < 12) {
            switch (reaction) {
            case 0:
            case PARATROOPER_HIT_REACTION_NONE:
                break;
            case 9:
            case 10:
                paratrooper_instance_state->hit_flash.flash_red = 0xfa;
                break;
            case PARATROOPER_HIT_REACTION_RECOVERY_KNOCKBACK:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8: {
                float pose_scale = l08_paratrooper_recovery_knockback_xy_speed * frame_time;
                float timer_scale = frame_time * 3.0f;
                paratrooper_instance_state->knockback.speed = pose_scale;
                paratrooper_instance_state->knockback.vertical_speed = timer_scale;
                paratrooper_instance_state->knockback.apex_animation_frame = 5.0f;
                paratrooper_instance_state->knockback.landing_animation_frame = 10.0f;
            }
                {
                    char *hit_moby = *(char **)(target + 0x20);
                    dx = MOBY(moby)->pos.x - MOBY(hit_moby)->pos.x;
                    dy = MOBY(moby)->pos.y - MOBY(hit_moby)->pos.y;
                }
                angle = fast_arc_tan(dx, dy);
                *(u128 *)pos = *(u128 *)(target + 0x10);
                knock_angle = angle;
                trooper_knockback_arc(
                    pos,
                    &knock_angle,
                    (float *)(&paratrooper_instance_state->knockback.speed),
                    (float *)(&paratrooper_instance_state->knockback.vertical_speed));
                trooper_start_knockback(
                    moby, &paratrooper_instance_state->knockback, knock_angle, 8, 1, 0);
                MOBY(moby)->state = PARATROOPER_STATE_KNOCKBACK_RECOVERY;
                paratrooper_instance_state->hit_flash.flash_red = 0x78;
                break;
            case PARATROOPER_HIT_REACTION_LETHAL:
            case PARATROOPER_HIT_REACTION_DEATH_KNOCKBACK: {
                float push_limit = 16.0f;
                float pose_rate = D_L08_00162184;
                float timer_rate = l08_paratrooper_death_knockback_xy_speed;
                float angle_rate = l08_paratrooper_death_knockback_z_speed;
                float step, pose_scale, timer_scale, angle_scale;
                ((Moby *)moby)->flags &= 0xefff;
                step = frame_time;
                pose_scale = pose_rate * frame_time_sq;
                timer_scale = timer_rate * step;
                angle_scale = angle_rate * step;
                paratrooper_instance_state->knockback.collision_z_offset = 0.25f;
                paratrooper_instance_state->knockback.vertical_speed = angle_scale;
                paratrooper_instance_state->knockback.gravity = pose_scale;
                paratrooper_instance_state->knockback.speed = timer_scale;
                paratrooper_instance_state->knockback.apex_animation_frame = 11.0f;
                paratrooper_instance_state->knockback.landing_animation_frame = push_limit;
                paratrooper_instance_state->knockback.collision_radius_1024ths = 0x200;
            }
                {
                    char *hit_moby = *(char **)(target + 0x20);
                    dx = MOBY(moby)->pos.x - MOBY(hit_moby)->pos.x;
                    dy = MOBY(moby)->pos.y - MOBY(hit_moby)->pos.y;
                }
                angle = fast_arc_tan(dx, dy);
                *(u128 *)pos = *(u128 *)(target + 0x10);
                second_knock_angle = angle;
                trooper_knockback_arc(
                    pos,
                    &second_knock_angle,
                    (float *)(&paratrooper_instance_state->knockback.speed),
                    (float *)(&paratrooper_instance_state->knockback.vertical_speed));
                trooper_start_knockback(
                    moby, &paratrooper_instance_state->knockback, second_knock_angle, 12, 1, 0);
                MOBY(moby)->state = PARATROOPER_STATE_DEATH_EFFECTS_AND_REMOVE;
                paratrooper_instance_state->hit_flash.flash_red = 0xf0;
                spawn_randomized_bolt_drop(moby, 2, 3, 0, -1);
                break;
            default:
                break;
            }
        }
        start_hit_color_flash(moby, &paratrooper_instance_state->hit_flash);
    }
    MOBY(moby)->hit_slot = 0xff;
    update_hit_color_flash(moby, &paratrooper_instance_state->hit_flash);

    if (paratrooper_instance_state->hurt != 0) {
        angle = random_float_between(180.0f, 240.0f);
        angle = scale_time(angle);
        paratrooper_instance_state->target_range_boost_timer = (short)truncate_float_to_s32(angle);
        paratrooper_instance_state->hurt = 0;
    }
    fast_dec_timer_s16((short *)(&paratrooper_instance_state->target_range_boost_timer));
    if (paratrooper_instance_state->target_range_boost_timer != 0)
        paratrooper_instance_state->target_range =
            paratrooper_instance_state->base_target_range + 6.0f;
    else
        paratrooper_instance_state->target_range = paratrooper_instance_state->base_target_range;

    if (random_integer_below(4) == 0) {
        {
            char *path = ((char *)l08_paths[paratrooper_instance_state->wake_path_index]);
            reaction = activate_effect_l08(moby,
                                           &paratrooper_instance_state->path_search_point,
                                           paratrooper_instance_state->target_range,
                                           0,
                                           0,
                                           path + 0x10,
                                           *(int *)path);
        }
        if (reaction != 2) {
            distance = vector_distance_xy(&MOBY(moby)->pos,
                                          &paratrooper_instance_state->path_search_point);
            if (paratrooper_instance_state->target_range < distance ||
                3.0f < absolute_float(MOBY(moby)->pos.z -
                                      paratrooper_instance_state->path_search_point_z))
                paratrooper_instance_state->target_selection_status = 2;
        }
    } else {
        chased = paratrooper_instance_state->target_moby;
        if (chased != 0 && *(unsigned char *)(chased + 0x20) != 0xfe &&
            *(unsigned char *)(chased + 0x20) != 0xfd) {
            qcopy(&paratrooper_instance_state->path_search_point, chased + 0x10);
        } else {
            paratrooper_instance_state->target_moby = 0;
            paratrooper_instance_state->target_selection_status = 2;
        }
    }
    if (*(int *)&paratrooper_instance_state->target_moby == 0)
        paratrooper_instance_state->target_moby = (char *)hero.moby;

    state = MOBY(moby)->state;
    if (state == PARATROOPER_STATE_INITIALIZE || state == PARATROOPER_STATE_FIRE_PROJECTILES ||
        state == PARATROOPER_STATE_MELEE_ATTACK ||
        state == PARATROOPER_STATE_HOP_BETWEEN_WAYPOINTS) {
        height = probe_ground_height(&MOBY(moby)->pos, 0, 0.5f);
        if (height < MOBY(moby)->pos.z)
            MOBY(moby)->pos.z = height;
    }
}

float D_L09_00161C14 = 2e+01f;
float D_L09_00161C10 = 1.0f;
int   D_L09_00161C0C = 5;
int   D_L09_00161C1C = 5;
float D_L09_00161C18 = 3.3f;
float D_L09_00161C20 = 2.0f;
float D_L09_00161C04 = 0.6f;
float D_L09_00161C08 = 0.6f;
float D_L09_00161BD0 = 1.0f;
float D_L09_00161BD4 = 37.0f;
float D_L09_00161BDC = 1e+01f;
float D_L09_00161BE0 = 11.0f;
float D_L09_00161BE4 = 15.0f;
float D_L09_00161C00 = 3.0f;
