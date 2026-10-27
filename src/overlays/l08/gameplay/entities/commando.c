/* Level 08 moby classes: commando. */
#include "types.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "qcopy.h"
#include "qzero.h"
#include "rnc/math/vector.h"
#include "rnc/math/fast_math.h"
#include "rnc/gameplay/entities/moby.h"

#include "rnc/overlay/swarm_path.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/gameplay/state/options.h"
#include "rnc/gameplay/state/game_globals.h"

typedef struct {
    SwarmPathFollower follow[2]; /* 0x00: [0] cruising (state 1), [1] circling (state 2) */
    SwarmPart root;              /* 0x60 */
    SwarmPart parts[16];         /* 0x70 */
    s32 target_index;            /* 0x170: moby slot of the target */
    f32 turn_speed;              /* 0x174 */
    s32 fire_timer;              /* 0x178 */
    s16 pad17C;
    s16 sound;                   /* 0x17E */
} SwarmVars;

#include "rnc/overlay/l08/gameplay/entities/commando_globals.h"
extern s32 swarm_update_disabled_gp __asm__("D_L08_00161D40") __attribute__((sda));
extern f32 swarm_path_movement_speed_gp __asm__("D_L08_00161D34") __attribute__((sda));
extern SwarmPath *D_L08_001B0CB0[];
extern struct Moby *D_L08_0015FFD8;

extern s32 voice_is_playing(struct Moby *, s32) __asm__("FUN_L00_0028d8c0");
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern f32 multiply_global_factor_ed64(f32) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern s32 tick_countdown_32_alt(s32 *) __asm__("FUN_001f9740");
extern f32 FUN_001f9b48(void *, void *);
extern float vector_length_xyz(void *);
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *);
extern void moby_point_position(void *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern void cart_burst(void *, void *, s32, f32, f32) __asm__("FUN_L00_0025f090");
extern struct Moby *spawn_class_1b3_projectile(void *, struct Moby *, void *, f32, f32) __asm__("FUN_L08_002de3e0");
extern void spawn_joint_linked_moby_parts(struct Moby *) __asm__("FUN_L08_002e9558");
extern void process_linked_moby_part_hits(struct Moby *) __asm__("FUN_L08_002e9758");
extern void update_commando_parts(struct Moby *) __asm__("FUN_L08_002e9a18");

void update_path_following_moby_swarm(struct Moby *moby) __asm__("FUN_L08_002e9b70");

void update_path_following_moby_swarm(struct Moby *moby) {
    enum {
        SWARM_INITIALIZE = 0,
        SWARM_FOLLOW_PRIMARY_PATH = 1,
        SWARM_FOLLOW_SECONDARY_PATH = 2,
        SWARM_START_PRIMARY_PATH = 3,
        SWARM_DISPERSE_AND_REMOVE = 0x63
    };
    SwarmVars *vars = (SwarmVars *)moby->pvars;
    SwarmPathFollower *follower;
    struct Moby *target;
    struct Moby *shot;
    struct Moby *bolt;
    SwarmTargetVars *target_vars;
    Vec4 point;
    Vec4 step;
    Vec4 muzzle;
    Vec4 velocity;
    s32 waypoint;
    s32 next;
    s32 after;
    s32 i;
    s32 slot;
    f32 distance;
    f32 heading;
    f32 next_heading;
    f32 turn;
    f32 pitch;
    f32 yaw;

    if (swarm_update_disabled_gp != 0) {
        return;
    }
    if (!voice_is_playing(moby, vars->sound)) {
        vars->sound = allocate_voice_for_target_entry(3, 4, moby);
    }
    switch (moby->state) {
    case SWARM_INITIALIZE:
        if (visited52[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        vars->follow[0].path = D_L08_001B0CB0[vars->follow[0].path_id];
        vars->follow[1].path = D_L08_001B0CB0[vars->follow[1].path_id];
        vars->fire_timer = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(300.0f, 600.0f)));
        moby->flags |= 0x4000;
        qcopy(&moby->pos, &vars->follow[1].path->points[0]);
        spawn_joint_linked_moby_parts(moby);
        moby->state = 2;
        moby->rot.z = fast_arc_tan(vars->follow[1].path->points[1].f[0] - moby->pos.x,
                                   vars->follow[1].path->points[1].f[1] - moby->pos.y);
        break;
    case SWARM_FOLLOW_PRIMARY_PATH:
        waypoint = (vars->follow[0].point + vars->follow[0].path->count + vars->follow[0].direction) % vars->follow[0].path->count;
        target = &D_L08_0015FFD8[vars->target_index];
        FUN_L00_00258278(moby,
                         fast_arc_tan(vars->follow[0].path->points[waypoint].f[0] - moby->pos.x,
                                      vars->follow[0].path->points[waypoint].f[1] - moby->pos.y),
                         &vars->turn_speed, 0.004f, 0.3f, 0.02f);
        qcopy(&point, &vars->follow[0].path->points[waypoint]);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            vars->follow[0].point = waypoint;
        }
        subtract_vector_xyz(&step, &point, &moby->pos);
        if (vector_length_xyz(&step) > swarm_path_movement_speed_gp * frame_time) {
            normalize_vector_xyz(&step, &step, swarm_path_movement_speed_gp * frame_time);
        }
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        process_linked_moby_part_hits(moby);
        update_commando_parts(moby);
        if (moby->command == 0) {
            break;
        }
        moby->command = 0;
        moby_point_position(vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(0.17453292f, 0.5235988f);
        yaw = fast_add_rotations(fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                                 3.1415927f);
        yaw = fast_add_rotations(yaw, random_float_between(-0.2617994f, 0.7853982f));
        if (yaw > 0.0f && yaw < 0.34f) {
            yaw = 0.34f;
        } else if (yaw <= 0.0f && yaw > -0.34f) {
            yaw = -0.34f;
        }
        normalize_vector_xyz(&velocity, &step, (swarm_path_movement_speed_gp + swarm_path_movement_speed_gp) * frame_time);
        shot = spawn_class_1b3_projectile(&muzzle, target, &velocity, pitch, yaw);
        shot->scale = shot->pclass->scale / 5.0f;
        target_vars = (SwarmTargetVars *)target->pvars;
        for (slot = 0; slot < 8; slot++) {
            if (target_vars->shots[slot] == NULL) {
                target_vars->shots[slot] = shot;
                break;
            }
        }
        break;
    case SWARM_FOLLOW_SECONDARY_PATH:
        follower = &vars->follow[1];
        next = (vars->follow[1].point + 1) % follower->path->count;
        after = (vars->follow[1].point + 2) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[next].f[0] - follower->path->points[vars->follow[1].point].f[0],
                               follower->path->points[next].f[1] - follower->path->points[vars->follow[1].point].f[1]);
        next_heading = fast_arc_tan(follower->path->points[after].f[0] - follower->path->points[next].f[0],
                                    follower->path->points[after].f[1] - follower->path->points[next].f[1]);
        turn = fast_subtract_rotations(heading, next_heading);
        distance = FUN_001f9b48(&moby->pos, &follower->path->points[next]);
        distance /= FUN_001f9b48(&follower->path->points[vars->follow[1].point],
                                 &follower->path->points[next]);
        moby->rot.z = fast_add_rotations(turn * distance, next_heading);
        qcopy(&point, &follower->path->points[next]);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            vars->follow[1].point = next;
        }
        subtract_vector_xyz(&step, &point, &moby->pos);
        if (vector_length_xyz(&step) > swarm_path_movement_speed_gp * frame_time) {
            normalize_vector_xyz(&step, &step, swarm_path_movement_speed_gp * frame_time);
        }
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        update_commando_parts(moby);
        if (!tick_countdown_32_alt(&vars->fire_timer)) {
            break;
        }
        vars->fire_timer = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(300.0f, 600.0f)));
        moby_point_position(vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(-0.5235988f, -0.2617994f);
        yaw = random_angle_radians();
        normalize_vector_xyz(&velocity, &step, (swarm_path_movement_speed_gp + swarm_path_movement_speed_gp) * frame_time);
        bolt = spawn_class_1b3_projectile(&muzzle, NULL, &velocity, pitch, yaw);
        if (bolt != NULL) {
            bolt->rot.z = vars->parts[3].moby->rot.z;
            bolt->state = 5;
            bolt->rot.y = 0.0f;
        }
        break;
    case SWARM_START_PRIMARY_PATH: {
        SwarmPath *path;

        moby->state = 1;
        path = vars->follow[0].path;
        qcopy(&moby->pos, &path->points[0]);
        moby->rot.z = fast_arc_tan(path->points[1].f[0] - moby->pos.x, path->points[1].f[1] - moby->pos.y);
        update_commando_parts(moby);
        break;
    }
    case SWARM_DISPERSE_AND_REMOVE:
        for (i = 0; i < 16; i++) {
            struct Moby *part = vars->parts[i].moby;
            if (part != NULL) {
                Vec4 *drift = (Vec4 *)part->pvars;
                cart_burst(moby, &part->pos, -1, 3.0f, 13.0f);
                subtract_vector_xyz(drift, &part->pos, &moby->pos);
                normalize_vector_xyz(drift, drift, frame_time * 5.0f);
                part->state = 1;
                vars->parts[i].moby = NULL;
            }
        }
        cart_burst(moby, &moby->pos, -1, 5.0f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}

extern char *D_L08_0015FFD8_g __asm__("D_L08_0015FFD8");
extern int FUN_0022da68_c(int, int, char *) __asm__("FUN_0022da68");
extern void FUN_00213ed8_c(char *, float, float) __asm__("FUN_00213ed8");

void update_moby_height_from_driver(char *moby) __asm__("FUN_L08_002ea398");

void update_moby_height_from_driver(char *moby)
{
    enum {
        POSITION_BASELINE_INITIALIZE = 0,
        POSITION_FOLLOW_DRIVER = 1,
        POSITION_APPROACH_TARGET = 2
    };
    char *position_vars = *(char **)(moby + 0x78);
    char *driver_moby;
    int i;
    switch (*(u8 *)(moby + 0x20)) {
    case POSITION_BASELINE_INITIALIZE:
        *(float *)(position_vars + 4) = *(float *)(moby + 0x18);
        *(u8 *)(moby + 0x20) = 1;
        break;
    case POSITION_FOLLOW_DRIVER:
        i = *(int *)(position_vars + 8);
        if (i < 0) break;
        driver_moby = (char *)((i << 8) + (int)D_L08_0015FFD8_g);
        if (*(short *)(driver_moby + 0xA6) == 0x267 || *(short *)(driver_moby + 0xA6) == 0x23F) {
            if (*(u8 *)(driver_moby + 0x20) != 4) break;
            menu_item_enabled_41 = 1;
            FUN_0022da68_c(0, 0, moby);
            *(u8 *)(moby + 0x20) = 2;
        } else {
            FUN_00213ed8_c(moby + 0x18, (*(float *)position_vars - *(float *)(position_vars + 4)) * **(float **)(driver_moby + 0x78) + *(float *)(position_vars + 4), frame_time * 10.0f);
        }
        break;
    case POSITION_APPROACH_TARGET:
        FUN_00213ed8_c(moby + 0x18, *(float *)position_vars, frame_time * 10.0f);
        break;
    }
}
/* Pvars of a gate that swings with the progress of its driver moby. */
typedef struct {
    s32 driver;          /* 0x0: moby index whose pvars start with the 0..1 progress */
    f32 open_deg;        /* 0x4: yaw at full progress, in degrees */
    f32 shut_deg;        /* 0x8: yaw at zero progress, in degrees */
    s32 flag;            /* 0xC: byte of D_0013D388 set when fully open */
    Vec4 home;           /* 0x10: hinge position */
    s32 voice;           /* 0x20: creak voice slot, -1 if none */
} SwingGateVars;

extern int D_L08_0015F5C4;
/* Voice table entries are 0x70 bytes; the volume sits 0x80 past an entry's start. */
typedef struct {
    u8 pad0[0x80];
    s32 volume;          /* 0x80 */
} VoiceView;
extern char D_0013E550[];
extern u8 D_0013D388[];
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void vendor_volume(s32, s32) __asm__("FUN_L01_002a1968");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");

/* Swings the gate to its driver's progress, fading a creak in while it
 * moves and out when it stops, and latches its flag once fully open. */
void update_swing_gate_from_driver(struct Moby *moby) __asm__("FUN_L08_002ea4c0");

void update_swing_gate_from_driver(struct Moby *moby) {
    SwingGateVars *vars = (SwingGateVars *)moby->pvars;
    f32 progress = *(f32 *)D_L08_0015FFD8[vars->driver].pvars;
    Vec4 gate_rotation_matrix[4];
    Vec4 gate_position_offset;
    f32 yaw;

    gate_position_offset.q = 0;
    gate_position_offset.f[2] = 30.0f;
    if (D_L08_0015F5C4 == 2) {
        moby->visible = 0;
        moby->flags |= 1;
    } else {
        moby->visible = 1;
        moby->flags &= ~1;
    }
    if (moby->state == 0) {
        moby->state = 1;
        qcopy(&vars->home, &moby->pos);
        return;
    }
    if (moby->state != 1)
        return;
    yaw = fast_subtract_rotations(vars->open_deg * DEG_TO_RAD, vars->shut_deg * DEG_TO_RAD);
    yaw = fast_add_rotations(yaw * progress, vars->shut_deg * DEG_TO_RAD);
    if (fast_difference_between_rotations(moby->rot.y, yaw) != 0.0f) {
        moby->rot.y = yaw;
        if (moby->oclass == 0x1D4) {
            if (!voice_is_playing(moby, vars->voice)) {
                vars->voice = allocate_voice_for_target_entry(0, 4, moby);
                vendor_volume(vars->voice, 1);
            } else if (((VoiceView *)(D_0013E550 + vars->voice * 0x70))->volume < 0x400) {
                vendor_volume(vars->voice, ((VoiceView *)(D_0013E550 + vars->voice * 0x70))->volume +
                                                  0x400 / scale_game_frames(60));
            }
        }
    } else if (moby->oclass == 0x1D4 && voice_is_playing(moby, vars->voice)) {
        if (((VoiceView *)(D_0013E550 + vars->voice * 0x70))->volume < 0x100) {
            release_voice_slot(vars->voice);
            vars->voice = -1;
        } else {
            vendor_volume(vars->voice, ((VoiceView *)(D_0013E550 + vars->voice * 0x70))->volume -
                                              0x400 / scale_game_frames(60));
        }
    }
    euler_rotation_matrix(gate_rotation_matrix, &moby->rot);
    transform_vector(&gate_position_offset, &gate_position_offset, gate_rotation_matrix);
    add_vector_xyz(&moby->pos, &vars->home, &gate_position_offset);
    if (progress == 1.0f && moby->oclass == 0x1D4) {
        D_0013D388[vars->flag + 0x30] = 1;
        moby->state = 2;
        allocate_voice_for_target_entry(1, 0, moby);
        if (moby->oclass == 0x1D4 && voice_is_playing(moby, vars->voice))
            release_voice_slot(vars->voice);
    }
}
/* Draws three rows of HUD elements with their colours. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_002F2288), where it is exact; names translated to the US level program. */

extern int scrolled_mesh_unscaled_fixed_alpha_gp __asm__("D_L08_00161DD0") __attribute__((sda));
extern int offset_mesh_first_pass_fixed_alpha_gp __asm__("D_L08_00161DF0") __attribute__((sda));
extern int pair_offset_mesh_fixed_alpha_gp __asm__("D_L08_00161E00") __attribute__((sda));
extern unsigned char D_L08_00161E08_u __asm__("D_L08_00161E08");
extern int get_effect_texture_alt(int) __asm__("FUN_001f44b8");
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void draw_scrolled_mesh_batch(int) __asm__("FUN_L08_002f0b40");
extern void FUN_L08_002f0c18(int);
extern void emit_pair_offset_render_batch(int) __asm__("FUN_L08_002f0cf0");
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

void draw_layered_mesh_effect(void) __asm__("FUN_L08_002f0dc8");

void draw_layered_mesh_effect(void) {
    vu1_add_g_sregister(0x42, 0x8000000044L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1);
    font_queue_vu_state();
    vu1_add_g_sregister(6, get_effect_texture_alt(0x3A));
    vu1_add_g_sregister(0x42, (long)scrolled_mesh_unscaled_fixed_alpha_gp << 32 | 0x44);
    draw_scrolled_mesh_batch(0);
    vu1_add_g_sregister(6, get_effect_texture_alt(0x3B));
    vu1_add_g_sregister(0x42, (long)((scrolled_mesh_subtractive_base_fixed_alpha * D_L08_00161E08_u) >> 8) << 32 | 0x62);
    draw_scrolled_mesh_batch(1);
    vu1_add_g_sregister(0x42, (long)((scrolled_mesh_additive_base_fixed_alpha * D_L08_00161E08_u) >> 8) << 32 | 0x68);
    draw_scrolled_mesh_batch(2);
    vu1_add_g_sregister(6, get_effect_texture_alt(0x3C));
    vu1_add_g_sregister(0x42, (long)offset_mesh_first_pass_fixed_alpha_gp << 32 | 0x48);
    FUN_L08_002f0c18(0);
    vu1_add_g_sregister(0x42, (long)offset_mesh_second_pass_fixed_alpha << 32 | 0x48);
    FUN_L08_002f0c18(1);
    vu1_add_g_sregister(6, get_effect_texture_alt(0x3D));
    vu1_add_g_sregister(0x42, (long)pair_offset_mesh_fixed_alpha_gp << 32 | 0x48);
    emit_pair_offset_render_batch(0);
}

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030AC70), where it is exact; names translated to the US level program. */

extern Pair D_L08_00161E10[] __attribute__((section(".sdata")));
extern Pair D_L08_00161E14[] __attribute__((section(".sdata")));
extern Pair D_L08_00161E24[] __attribute__((section(".sdata")));
extern Pair D_L08_001E8348[];
extern Pair D_L08_00161E28;
extern char D_L08_001675C0[];
extern s32 D_L08_001DDF40[];
extern s32 D_L08_001E2828[];
extern s32 D_L08_001E2858[];
extern s32 D_L08_001E8310[];
extern char D_L08_001E8350[];
extern float vector_length_xyz(void *);
extern int D_L08_00161E08;
extern int fast_bsphere_check(char *, float) __asm__("FUN_001fa728");
extern int truncate_float_to_s32() __asm__("FUN_001fa6d0");
extern short D_L08_00161DC0 __attribute__((sda));
extern short D_L08_00161E04 __attribute__((sda));
extern s32 D_L08_001DDF20[];
extern s32 D_L08_001DB518[];
extern void draw_layered_mesh_effect(void) __asm__("FUN_L08_002f0dc8");
extern void scale_rgba_tables(void *, int, void *, float, float, float, float, int) __asm__("FUN_L08_002f0f68");
extern void advance_wrapped_sine_pair(void *, float, float, float, void *) __asm__("FUN_L08_002f1158");
extern void advance_scroll_offsets(int) __asm__("FUN_L08_002f12a0");
extern void advance_scroll_entry(int) __asm__("FUN_L08_002f1378");
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
extern void subtract_vector_xyz(void *, void *, void *);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

extern Pair D_L08_00161E20[] __attribute__((section(".sdata")));
void update_layered_mesh_effect(struct Moby *moby) __asm__("FUN_L08_002f1548");

void update_layered_mesh_effect(struct Moby *moby) {
    enum {
        DRAW_ELEMENTS_INITIALIZE = 0,
        DRAW_ELEMENTS_UPDATE = 1
    };
    int i;
    float culling_sphere[4];
    float distance_vector[4];
    float distance;

    switch (moby->state) {
    case DRAW_ELEMENTS_INITIALIZE:
        if (*(int *)&D_L08_00161E04 == 0) {
            scale_rgba_tables(D_L08_001E8310, 0xD, D_L08_001E2858, 0.5f, 0.5f, 0.65f, 0.5f, 1);
            scale_rgba_tables(D_L08_001DDF20, 8, D_L08_001DB518, 0.5f, 0.5f, 0.5f, 1.0f, 1);
            scale_rgba_tables(D_L08_001E2828, 0xB, D_L08_001DDF40, 0.25f, 0.25f, 0.25f, 1.0f, 1);
            *(int *)&D_L08_00161E04 = 1;
        }
        moby->state = 1;
        for (i = 0; i < 3; i++) {
            D_L08_001E8348[i].a = 0;
            D_L08_001E8348[i].b = 0;
            D_L08_00161E28.a = 0;
            D_L08_00161E28.b = 0;
        }
        for (i = 0; i < 2; i++) {
            D_L08_00161E10[i].a = 0;
            *(float *)&D_L08_00161E14[i].a = i * 0.4f;
        }
        for (i = 0; i < 1; i++) {
            D_L08_00161E20[i].a = 0;
            *(float *)&D_L08_00161E24[i].a = i * 0.4f;
        }
        break;
    case DRAW_ELEMENTS_UPDATE:
        culling_sphere[0] = *(float *)&D_L08_00161DC0;
        culling_sphere[1] = layered_mesh_effect_center_y;
        culling_sphere[2] = layered_mesh_effect_center_z;
        culling_sphere[3] = layered_mesh_effect_culling_radius;
        if (fast_bsphere_check((char *)culling_sphere, 1000.0f) >= 0) {
            distance_vector[0] = *(float *)&D_L08_00161DC0;
            distance_vector[1] = layered_mesh_effect_center_y;
            distance_vector[2] = layered_mesh_effect_center_z;
            distance_vector[3] = 1.0f;
            subtract_vector_xyz(distance_vector, D_L08_001675C0, distance_vector);
            distance = vector_length_xyz(distance_vector);
            if (distance <= 84.0f) {
                advance_scroll_offsets(0);
                advance_wrapped_sine_pair(D_L08_001E8350, 0.52f, 0.62f, 0.06f, (void *)&D_L08_00161E28);
                advance_scroll_offsets(2);
                if (distance < 20.0f) {
                    *(unsigned char *)&D_L08_00161E08 = 0xFF;
                } else if (distance < 84.0f) {
                    *(char *)&D_L08_00161E08 =
                        func_001FA898_r((1.0f - (distance - 20.0f) * 0.015625f) * 255.0f);
                } else {
                    *(char *)&D_L08_00161E08 = 0;
                }
            }
            advance_scroll_entry(0);
            advance_scroll_entry(0);
            advance_scroll_entry(1);
            enqueue_callback_list_1(draw_layered_mesh_effect, moby);
        }
        break;
    }
}
extern f32 D_L08_0015F580[] __attribute__((section(".sdata")));
extern struct Moby *D_0013F58C __attribute__((section(".data")));
/* Retail loads gp - 0x4C90; GP 0x166C00 places this scalar at 0x161F70. */

extern s32 find_moby_hit_44c0(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern f32 FUN_001f9b80_44c0(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_add_rotations_44c0(f32, f32) __asm__("FUN_001fa580");
extern f32 FUN_001f9e90_44c0(f32, f32) __asm__("FUN_001f9e90");
extern f32 FUN_001f9b48_44c0(void *, void *) __asm__("FUN_001f9b48");
extern void clear_u64_value_44c0(void *) __asm__("FUN_001f99f8");
extern void subtract_vector_xyz_44c0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz_44c0(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_vector_xyz_44c0(void *, void *, void *) __asm__("FUN_001f9a10");
extern s32 scale_game_frames_44c0(s32) __asm__("FUN_001f96f8");
extern f32 approach_value_44c0(void *, f32, f32) __asm__("FUN_00213ed8");
extern void mark_moby_for_removal_44c0(struct Moby *) __asm__("FUN_0020c828");
extern void allocate_voice_for_target_entry_44c0(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern void FUN_L00_00258278_44c0(struct Moby *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_00258278");
extern void FUN_L00_0025e450_44c0(char *, void *, char *, f32, f32, s32, s32, s32,
                         f32, f32, f32, f32, s32, f32, s32, s32, s32, u8) __asm__("FUN_L00_0025e450");

extern f32 path_follower_y_rotation_rate_degrees_gp __asm__("D_L08_00161F70") __attribute__((sda));
void update_path_following_moby_with_fade(struct Moby *moby) __asm__("FUN_L08_002f44c0");

void update_path_following_moby_with_fade(struct Moby *moby)
{
    enum {
        PATH_FOLLOWER_INITIALIZE_HEIGHT_OFFSET = 0,
        PATH_FOLLOWER_CHECK_HIT_AND_PROXIMITY = 1,
        PATH_FOLLOWER_FOLLOW_PATH = 2,
        PATH_FOLLOWER_FADE_OUT = 3
    };
    SwarmPathFollower *follower = (SwarmPathFollower *)moby->pvars;
    Vec4 point;
    Vec4 step;
    Vec4 velocity; /* Effect scratch is separate from the waypoint movement step. */
    f32 fade;
    s32 waypoint;
    f32 distance;

    switch (moby->state) {
    case PATH_FOLLOWER_INITIALIZE_HEIGHT_OFFSET: {
        fade = moby->pos.z;
        moby->state = 1;
        moby->pos.z = fade + 0.4f;
        return;
    }
    case PATH_FOLLOWER_CHECK_HIT_AND_PROXIMITY: {
        s32 active = find_moby_hit_44c0(moby, 0x330000, 0);
        distance = FUN_001f9b80_44c0(&moby->pos, D_0013F3D0);
        if ((distance < 2.0f && *(s32 *)(D_0013F3D0 + 0x2004) == 0x42) || *(struct Moby **)(D_0013F3D0 + 0x1bc) == moby) {
            allocate_voice_for_target_entry_44c0(0, 0, moby);
            FUN_L00_0025e450_44c0(moby, D_L08_0015F580, &moby->pos, 3.0f, 1.0f, 10, 20, 60,
                          3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_44c0(moby);
            return;
        }
        if (active != 0) {
            allocate_voice_for_target_entry_44c0(0, 0, moby);
            FUN_L00_0025e450_44c0(moby, D_L08_0015F580, &moby->pos, 0.0f, 0.0f, 10, 20, 60,
                          1.0f, 3.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_44c0(moby);
        }
        return;
    }
    case PATH_FOLLOWER_FOLLOW_PATH: {
        moby->pos.z -= 0.4f;
        if (follower->path_id == -1 || follower->path->count == 0) {
            mark_moby_for_removal_44c0(moby);
            return;
        }
        moby->rot.y = fast_add_rotations_44c0(moby->rot.y, path_follower_y_rotation_rate_degrees_gp * 0.017453292f * frame_time);
        distance = FUN_001f9b80_44c0(&moby->pos, D_0013F3D0);
        if (distance < 2.0f && *(s32 *)(D_0013F3D0 + 0x2004) == 0x42) {
            clear_u64_value_44c0(&velocity);
            FUN_L00_0025e450_44c0(moby, &velocity, &moby->pos, 0.0f, 0.0f, 10, 20, 60,
                          3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_44c0(moby);
            return;
        }
        waypoint = (follower->point + follower->path->count + follower->direction) % follower->path->count;
        FUN_L00_00258278_44c0(moby, FUN_001f9e90_44c0(follower->path->points[waypoint].f[0] - moby->pos.x,
                                   follower->path->points[waypoint].f[1] - moby->pos.y),
                     (f32 *)((char *)follower + 0x34), 0.01f, 0.3f, 0.1f);
        qcopy(&point, &follower->path->points[waypoint]);
        if (waypoint != 0) {
            subtract_vector_xyz_44c0(&step, &point, &moby->pos);
            normalize_vector_xyz_44c0(&step, &step, frame_time * 3.0f);
            add_vector_xyz_44c0(&moby->pos, &moby->pos, &step);
        } else {
            distance = FUN_001f9b80_44c0(&moby->pos, D_0013F3D0);
            if (distance < 24.0f) {
                clear_u64_value_44c0(&velocity);
                FUN_L00_0025e450_44c0(moby, &velocity, &moby->pos, 0.0f, 0.0f, 10, 20, 60,
                              3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
                allocate_voice_for_target_entry_44c0(0, 0, moby);
                mark_moby_for_removal_44c0(moby);
                return;
            }
            moby->state = 3;
        }
        if (FUN_001f9b48_44c0(&moby->pos, &point) < 0.1f) {
            follower->point = waypoint;
        }
        moby->pos.z += 0.4f;
        return;
    }
    case PATH_FOLLOWER_FADE_OUT: {
        Vec4 timer;
        timer.f[0] = (f32)moby->alpha;
        approach_value_44c0(&timer, 0.0f, 128.0f / (f32)scale_game_frames_44c0(30));
        moby->alpha = (u8)(s32)timer.f[0];
        if (moby->alpha == 0) mark_moby_for_removal_44c0(moby);
    }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Smooth noise: linearly interpolates between two entries of an 8-float ring at arg+0x90. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_002F7258), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32_u(f32) __asm__("FUN_001fa6d0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

float interpolate_sample_ring(float x, char *sample_ring_data) __asm__("FUN_L08_002f5d98");

float interpolate_sample_ring(float x, char *sample_ring_data) {
    float sample_position = x * 8.0f;
    int i = func_001FA898_r(sample_position);
    float fr = sample_position - convert_integer_to_float(i);
    char *samples_base;
    int j;
    i = i % 8;
    j = i + 1;
    j = j % 8;
    samples_base = sample_ring_data + 0x90;
    return *(float *)(samples_base - (-(i * 4))) * (1.0f - fr) + *(float *)(samples_base - (-(j * 4))) * fr;
}
/* A point light slot, 0x20 bytes apart. */
typedef struct {
    f32 red;
    f32 green;
    f32 blue;
    u8 padC[4];
    Vec4 pos;
} PointLight;

extern PointLight D_L08_00180AC0[];

/* Builds the animated grid, draws its quads, and updates attached effects. */
/* Ported from rac1-decomp (nonmatching/l08_batalia/func_L08_002F7318.c: func_L08_002F7318). */

/* Flickering mesh / point light pvars, 0x160 bytes (update_flickering_mesh_effect
   initialises the ring and the light slot, draw_flickering_mesh_effect draws). */
typedef struct {
    f32 size_x;              /* 0x00: quad width; the larger of size_x/size_y sets the draw scale */
    f32 size_y;              /* 0x04 */
    u8 nx;                   /* 0x08: grid columns */
    u8 ny;                   /* 0x09: grid rows */
    u16 flags;               /* 0x0A: bit 0 = texture +0x28, bit 1 = particles, bit 2 = puffs, bit 15 = ... */
    u8 texture;              /* 0x0C */
    u8 frames;               /* 0x0D: animation frames (0: static) */
    u8 frame_ticks;          /* 0x0E */
    u8 current;              /* 0x0F: current texture */
    u8 red;                  /* 0x10: quad colour */
    u8 blue;                 /* 0x11 */
    u8 green;                /* 0x12 */
    u8 alpha;                /* 0x13 */
    f32 phase_start;         /* 0x14 */
    f32 phase_end;           /* 0x18 */
    f32 amp_start;           /* 0x1C */
    f32 amp2_start;          /* 0x20 */
    f32 amp_end;             /* 0x24 */
    f32 amp2_end;            /* 0x28 */
    f32 wave_u;              /* 0x2C */
    f32 wave_v;              /* 0x30 */
    f32 wave2_v;             /* 0x34 */
    f32 wave2_u;             /* 0x38 */
    u8 blend0;               /* 0x3C: GS blend fields */
    u8 blend1;
    u8 blend2;
    u8 blend3;
    f32 u_rate;              /* 0x40: texture scroll */
    f32 u_amp;               /* 0x44 */
    f32 u_phase;             /* 0x48 */
    f32 v_rate;              /* 0x4C */
    f32 v_amp;               /* 0x50 */
    f32 v_phase;             /* 0x54 */
    f32 noise_amp;           /* 0x58 */
    f32 noise_speed;         /* 0x5C */
    f32 phase0;              /* 0x60 */
    f32 phase1;              /* 0x64 */
    f32 phase0_end;          /* 0x68 */
    f32 phase1_end;          /* 0x6C */
    f32 texture_w_count;     /* 0x70: must be in (0, 8] */
    f32 texture_h_count;     /* 0x74: must be in (0, 8] */
    f32 normal_u;            /* 0x78 */
    f32 normal_v;            /* 0x7C */
    f32 z0;                  /* 0x80: bottom edge height */
    f32 z1;                  /* 0x84: top edge height */
    f32 jitter;              /* 0x88: lower bound of the random flicker levels */
    f32 noise_rate;          /* 0x8C */
    f32 noise[8];            /* 0x90: flicker ring (interpolate_sample_ring), chases goal */
    f32 goal[8];             /* 0xB0 */
    f32 noise_u;             /* 0xD0 */
    f32 noise_v;             /* 0xD4 */
    f32 noise_bias;          /* 0xD8 */
    f32 particle_xy;         /* 0xDC */
    f32 particle_z;          /* 0xE0 */
    f32 particle_size0;      /* 0xE4 */
    f32 particle_size1;      /* 0xE8 */
    u8 particle_r;           /* 0xEC */
    u8 particle_g;
    u8 particle_b;
    u8 particle_a;
    s16 particle_life;       /* 0xF0 */
    u8 particle_count;       /* 0xF2 */
    u8 particle_ticks;       /* 0xF3 */
    f32 particle_range;      /* 0xF4 */
    s32 particle_timer;      /* 0xF8 */
    f32 particle_offset;     /* 0xFC */
    f32 particle_height;     /* 0x100 */
    f32 puff_xy;             /* 0x104 */
    f32 puff_z;              /* 0x108 */
    f32 puff_size0;          /* 0x10C */
    f32 puff_size1;          /* 0x110 */
    u8 puff_c0[4];           /* 0x114 */
    u8 puff_c1[4];           /* 0x118 */
    s16 puff_life0;          /* 0x11C */
    s16 puff_life1;          /* 0x11E */
    u8 puff_count;           /* 0x120 */
    u8 puff_ticks;           /* 0x121 */
    s16 puff_timer;          /* 0x122 */
    f32 puff_range;          /* 0x124 */
    f32 puff_blend;          /* 0x128 */
    f32 puff_offset;         /* 0x12C */
    f32 puff_height;         /* 0x130 */
    u8 light_red;            /* 0x134: point light colour */
    u8 light_green;          /* 0x135 */
    u8 light_blue;           /* 0x136 */
    u8 radius;               /* 0x137 */
    u8 falloff;              /* 0x138 */
    u8 light_range_x;        /* 0x139 */
    u8 light_range_y;        /* 0x13A */
    s8 light;                /* 0x13B: point light handle (-1: none) */
    f32 light_speed;         /* 0x13C */
    Vec4 light_pos;          /* 0x140 */
    f32 fade_range;          /* 0x150: distance over which the light fades out */
    f32 intensity;           /* 0x154 */
    f32 alpha_u;             /* 0x158 */
    f32 alpha_v;             /* 0x15C */
} FlickerLightVars;

/* A textured quad for draw_geometry_quad: four vertices, colours, uvs, GS registers. */
typedef struct {
    Vec4 vertex[4];
    u32 color[4];
    f32 uv[8];
    s64 gs[4];
} FlickerQuad;

/* interpolate_sample_ring(float, char *) with the arguments the other way round: the
   real prototype evaluates the operands in the other order and moves two words. */
extern float interpolate_sample_ring_data_first(char *, float) __asm__("FUN_L08_002f5d98");
extern float D_0015ED60;
extern int D_L08_0015F3F8;
extern f32 fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern f32 square_root(f32) __asm__("FUN_001f9988");
extern f32 split_float_fraction(void *, f32) __asm__("FUN_L00_00200228");
extern s32 random_int_between(s32, s32) __asm__("FUN_L00_00257b90");
extern f32 normalize_angle(f32) __asm__("FUN_L00_0025e310");
extern s32 fast_dec_timer_s16(s16 *) __asm__("FUN_001f9770");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
/* spawn_colored_effect (rendering/00279f00.c) with size before life: the real order
   (size last) reorders the argument evaluation and moves six words. */
extern unsigned char *spawn_colored_effect_size_first(void *, void *, void *, float, short, unsigned char, unsigned char, unsigned char, unsigned char) __asm__("FUN_L08_0027b450");
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");

void draw_flickering_mesh_effect(struct Moby *m) __asm__("FUN_L08_002f5e58");
void draw_flickering_mesh_effect(struct Moby *moby) {
    Vec4 corner[4],step,left_step,right_step,left,right,point,edge,normal;
    float matrix[16];
    FlickerQuad quad;
    Vec4 sv0,sv1,sv2,sv3,sv4;
    float fraction;
    FlickerLightVars *data;
    if (!moby || !(data=(FlickerLightVars *)moby->pvars)) return;
    {
        int row_bytes=(data->nx+1)*sizeof(Vec4);
        Vec4 grid[data->ny+1][data->nx+1];
        unsigned char *points=(unsigned char*)grid;
        int remaining;
        float inv_x,inv_y;
        inv_x=1.0f/convert_integer_to_float(data->nx);
        inv_y=1.0f/convert_integer_to_float(data->ny);
        {
        float u[data->nx+1],v[data->ny+1];
        float phase,phase_step,amp,amp_step,amp2,amp2_step;
        float phase0_step,phase1_step,time,z0,z1,bias;
        int alpha,frame;
        float row_noise,phase0,phase1;
        int i,j;
        unsigned int color;
        time=convert_integer_to_float(D_L08_0015F3F8);
        for (remaining=0;remaining<8;remaining++) {
            float *goal;
            if (data->noise[remaining]<*(goal=&data->goal[remaining])) {
                data->noise[remaining]+=data->jitter*data->noise_rate;
                if (!(*goal<=data->noise[remaining])) continue;
            } else {
                data->noise[remaining]-=data->jitter*data->noise_rate;
                if (!(data->noise[remaining]<=*goal)) continue;
            }
            *goal=random_float_between(1.0f-data->jitter,1.0f);
        }
        alpha=func_001FA898_r(data->intensity*convert_integer_to_float(data->alpha)*convert_integer_to_float(moby->alpha)*0.0078125f)&0xFF;
        if (data->frames) { frame=D_L08_0015F3F8; frame/=data->frame_ticks+1; data->current=data->texture+frame%data->frames; }
        else data->current=data->texture;
        if (data->flags&1) quad.gs[1]=get_effect_texture_alt(data->current);
        else quad.gs[1]=get_effect_texture_alt(data->current+0x28);
        quad.gs[0]=0;
        quad.gs[2]=0xFF9000000260L;
        quad.gs[3]=(long)data->blend0|((long)data->blend1<<2)|((long)data->blend2<<4)|((long)data->blend3<<6)|((long)data->alpha<<32);
        color=(data->alpha<<24)|(data->blue<<16)|(data->green<<8)|data->red;
        quad.color[0]=color; quad.color[1]=color; quad.color[2]=color; quad.color[3]=color;
        qzero(&corner[0]); qzero(&corner[1]); qzero(&corner[2]); qzero(&corner[3]);
        {
        float half_width=data->size_x*0.5f;
        corner[1].f[0]=corner[3].f[0]=half_width;
        corner[0].f[0]=corner[2].f[0]=-half_width;
        }
        corner[0].f[2]=data->z0+0.01f; corner[1].f[2]=data->z1+0.01f;
        euler_rotation_matrix(matrix,&moby->rot);
        transform_vector(&corner[0],&corner[0],matrix);
        transform_vector(&corner[1],&corner[1],matrix);
        transform_vector(&corner[2],&corner[2],matrix);
        transform_vector(&corner[3],&corner[3],matrix);
        fast_vec_sub(&left_step,&corner[2],&corner[0]);
        fast_vec_scale(&left_step,&left_step,inv_y);
        fast_vec_sub(&right_step,&corner[3],&corner[1]);
        fast_vec_scale(&right_step,&right_step,inv_y);
        phase_step=(data->phase_end-data->phase_start)*inv_y;
        amp_step=(data->amp_end-data->amp_start)*inv_y;
        amp2_step=(data->amp2_end-data->amp2_start)*inv_y;
        phase0_step=(data->phase0_end-data->phase0)*inv_x;
        phase1_step=(data->phase1_end-data->phase1)*inv_y;
        z0=data->size_y-data->z0; z1=data->size_y-data->z1;
        fast_vec_sub(&edge,&corner[1],&corner[0]);
        fast_vec_cross(&normal,&edge,&left_step);
        normalize_vector_xyz(&normal,&normal,1.0f);
        qcopy(&left,&corner[0]); qcopy(&right,&corner[1]);
        bias=data->noise[0];
        phase=data->phase_start; amp=data->amp_start; amp2=data->amp2_start;
        phase0=data->phase0; phase1=data->phase1;
        {
        /* Fakematch: 30 unused locals before i, so i+1 hashes after &data->puff_timer. */
        int pad0,pad1,pad2,pad3,pad4,pad5,pad6,pad7,pad8,pad9,pad10,pad11,pad12,pad13,pad14,pad15,pad16,pad17,pad18,pad19,pad20,pad21,pad22,pad23,pad24,pad25,pad26,pad27,pad28,pad29;
        int i;
        register int j;
        for (i=0;i<=data->ny;phase+=phase_step,amp+=amp_step,amp2+=amp2_step,phase0+=phase0_step,phase1+=phase1_step,i++) {
            float y=convert_integer_to_float(i)/convert_integer_to_float(data->ny);
            qcopy_nc(&point,&left);
            j=0;
            fast_vec_sub(&step,&right,&left);
            fast_vec_scale(&step,&step,inv_x);
            row_noise=interpolate_sample_ring_data_first((char*)data,y);
            for (;j<=data->nx;j++) {
                float x=convert_integer_to_float(j)/convert_integer_to_float(data->nx);
                float theta;
                float wave=time*phase+data->noise_amp*fast_sin(normalize_angle(data->noise_speed*time));
                float noise=interpolate_sample_ring_data_first((char*)data,x);
                float angle0,angle1,shape,drop,a,b,normal_angle0,normal_angle1;
                drop=z0*x+z1*(1.0f-x);
                theta=x*3.1415927f;
                wave+=noise*data->noise_u+row_noise*data->noise_v+bias*data->noise_bias;
                angle0=normalize_angle(wave+phase0+data->wave_u*x+data->wave2_v*y);
                angle1=normalize_angle(wave+phase1+data->wave_v*y+data->wave2_u*x);
                shape=fast_sin(theta);
                shape=square_root(shape);
                drop*=shape;
                drop*=1.0f-y;
                normalize_vector_xyz(&sv0,&step,amp*fast_cos(angle0)*shape*noise);
                normalize_vector_xyz(&sv1,&left_step,amp2*fast_sin(angle1)*shape*noise);
                a=fast_sin(y*3.1415927f); b=fast_sin(theta);
                normal_angle0=fast_sin(normalize_angle(wave+data->wave_u*x));
                normal_angle1=fast_sin(normalize_angle(wave+data->wave2_v*y));
                normalize_vector_xyz(&sv3,&normal,a*b*(data->normal_u*normal_angle0+data->normal_v*normal_angle1));
                normalize_vector_xyz(&sv4,&left_step,drop);
                add_vector_xyz(&sv2,&sv0,&sv1);
                add_vector_xyz(&sv2,&sv2,&sv3);
                fast_vec_sub(&sv2,&sv2,&sv4);
                add_vector_xyz((Vec4*)(points+i*row_bytes)+j,&point,&sv2);
                *(float*)(points+(i*row_bytes+j*sizeof(Vec4))+12)=1.0f/((1.0f-data->alpha_u+data->alpha_u*shape*shape*shape)*(1.0f-data->alpha_v+data->alpha_v*y));
                add_vector_xyz(&point,&point,&step);
            }
            add_vector_xyz(&left,&left,&left_step);
            add_vector_xyz(&right,&right,&right_step);
        }
        }
        {
            int i;
            float f=split_float_fraction(&fraction,time*data->u_rate+data->u_amp*fast_sin(normalize_angle(time*data->u_phase)));
            for (i=0;i<=data->nx;i++) { u[i]=f; f+=inv_x/data->texture_w_count; }
            f=split_float_fraction(&fraction,time*data->v_rate+data->v_amp*fast_sin(normalize_angle(time*data->v_phase)));
            for (i=0;i<=data->ny;i++) { v[i]=f; f+=inv_y/data->texture_h_count; }
        }
        {
        int i;
        for (i=0;i<data->ny;i++) {
            int j;
            for (j=0;j<data->nx;j++) {
                int j1=j+1;
                int i1=i+1;
                float *vi=&v[i],*vi1=&v[i1];
                FlickerQuad *q=&quad;
                int row_offset=i*row_bytes,next_offset=i1*row_bytes;
                Vec4 *row=(Vec4*)(points+row_offset),*next_row=(Vec4*)(points+next_offset);
                quad.uv[0]=u[j]; quad.uv[1]=*vi;
                quad.uv[2]=u[j1]; quad.uv[3]=*vi;
                quad.uv[4]=u[j]; quad.uv[5]=*vi1;
                quad.uv[6]=u[j1]; quad.uv[7]=*vi1;
                add_vector_xyz(&q->vertex[0],&row[j],&moby->pos);
                add_vector_xyz(&quad.vertex[1],&row[j+1],&moby->pos);
                add_vector_xyz(&quad.vertex[2],&next_row[j],&moby->pos);
                add_vector_xyz(&quad.vertex[3],&next_row[j+1],&moby->pos);
                quad.color[0]=((alpha/func_001FA898_r(*(float*)(points+(row_offset+(j)*sizeof(Vec4))+12)))<<24)|(data->blue<<16)|(data->green<<8)|data->red;
                quad.color[1]=((alpha/func_001FA898_r(*(float*)(points+(row_offset+j1*sizeof(Vec4))+12)))<<24)|(data->blue<<16)|(data->green<<8)|data->red;
                quad.color[2]=((alpha/func_001FA898_r(*(float*)(points+(next_offset+(j)*sizeof(Vec4))+12)))<<24)|(data->blue<<16)|(data->green<<8)|data->red;
                quad.color[3]=((alpha/func_001FA898_r(*(float*)(points+(next_offset+j1*sizeof(Vec4))+12)))<<24)|(data->blue<<16)|(data->green<<8)|data->red;
                draw_geometry_quad(q,0,0);
            }
        }
        }
        {
            /* Fakematch: a never-taken branch (the cmov folds at combine, jump2 deletes it).
               Retail had about 50 more insns at gcse time that vanish later; they size the PRE
               expression hash table (n_insns/2|1 = 583 buckets), whose walk order numbers the
               PRE pseudos and so orders the spill slots. 42 one-insn calls (41..44 equal). */
            int dead = 0;
            if (data->flags & 0x8000) dead = 0;
            if (dead) {
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
                font_queue_vu_state();
            }
        }
        if (data->flags&2) {
            if (tick_countdown_32_alt(&data->particle_timer)) {
                data->particle_timer=scale_game_frames(data->particle_ticks);
                for (i=0;i<data->particle_count;i++) {
                    float width=data->size_x*0.5f;
                    float x=random_float_between(-1.0f,1.0f)*data->particle_range;
                    float fraction=random_float_between(0.0f,x);
                    float size;
                    short life;
                    int r,g,b,a;
                    Vec4 *p0=&sv0;
                    normalize_vector_xyz(&sv2,&left_step,-(data->particle_offset+square_root(fast_cos(fraction*3.1415927f))*data->particle_height));
                    normalize_vector_xyz(&sv1,&step,fraction*width);
                    add_vector_xyz(&sv1,&sv1,&moby->pos);
                    add_vector_xyz(&sv1,&sv1,&sv2);
                    sv0.f[0]=random_float_between(-data->particle_xy,data->particle_xy)*D_0015ED60;
                    sv0.f[1]=random_float_between(-data->particle_xy,data->particle_xy)*D_0015ED60;
                    sv0.f[2]=random_float_between(0.0f,data->particle_z)*D_0015ED60;
                    size=random_float_between(data->particle_size0,data->particle_size1);
                    life=scale_game_frames(data->particle_life);
                    r=data->particle_r; g=data->particle_g; b=data->particle_b;
                    a=func_001FA898_r(convert_integer_to_float(data->particle_a)*fraction);
                    spawn_colored_effect_size_first(moby,&sv1,p0,size,life,r,g,b,a);
                }
            }
            if (fast_dec_timer_s16(&data->puff_timer)) {
                data->puff_timer=scale_game_frames(data->puff_ticks);
                for (i=0;i<data->puff_count;i++) {
                    int c0=(data->puff_c0[3]<<24)|(data->puff_c0[2]<<16)|(data->puff_c0[1]<<8)|data->puff_c0[0];
                    int c1=(data->puff_c1[3]<<24)|(data->puff_c1[2]<<16)|(data->puff_c1[1]<<8)|data->puff_c1[0];
                    float width=data->size_x*0.5f;
                    float size=random_float_between(data->puff_size0,data->puff_size1);
                    int life=scale_game_frames(random_int_between(data->puff_life0,data->puff_life1));
                    float fraction=random_float_between(-1.0f,1.0f)*data->puff_range;
                    float shape;
                    fraction=random_float_between(-fraction,fraction);
                    shape=square_root(fast_cos(fraction*3.1415927f));
                    normalize_vector_xyz(&sv2,&left_step,-(data->puff_offset+shape*data->puff_height));
                    normalize_vector_xyz(&sv1,&step,fraction*width);
                    add_vector_xyz(&sv1,&sv1,&moby->pos);
                    add_vector_xyz(&sv1,&sv1,&sv2);
                    sv0.f[0]=random_float_between(-data->puff_xy,data->puff_xy)*D_0015ED60;
                    sv0.f[1]=random_float_between(-data->puff_xy,data->puff_xy)*D_0015ED60;
                    sv0.f[2]=random_float_between(0.0f,data->puff_z)*D_0015ED60;
                    fast_vec_scale(&sv0,&sv0,1.0f-data->puff_blend+shape*data->puff_blend);
                    spawn_effect_particle_with_target_scalar(&sv1,&sv0,c0,c1,size,life);
                }
            }
            if (data->light!=-1) {
                PointLight *light=&D_L08_00180AC0[data->light];
                if (fast_vec_dist(&data->light_pos,&light->pos)<data->light_speed*D_0015ED60) {
                    float x=(random_float_between(-(int)data->light_range_x,data->light_range_x)+data->size_x)*0.5f;
                    float y=(random_float_between(-(int)data->light_range_y,data->light_range_y)+data->size_y)*0.5f;
                    normalize_vector_xyz(&sv0,&step,x);
                    normalize_vector_xyz(&sv1,&left_step,-y);
                    add_vector_xyz(&data->light_pos,&sv0,&sv1);
                    add_vector_xyz(&data->light_pos,&data->light_pos,&corner[0]);
                    add_vector_xyz(&data->light_pos,&data->light_pos,&moby->pos);
                } else {
                    fast_vec_sub(&sv0,&data->light_pos,&light->pos);
                    normalize_vector_xyz(&sv0,&sv0,data->light_speed*D_0015ED60);
                    add_vector_xyz(&light->pos,&light->pos,&sv0);
                    light->pos.f[3]=convert_integer_to_float(data->radius);
                }
            }
            data->flags^=2;
        }
        }
    }
}
extern char D_L08_001FC220[];
char D_L08_001FC250[] = "M671 Warning: texture h count (%f) is greater than max texture tile count (8)\n";
extern char D_L08_001FC2A0[];
extern char D_L08_001FC2D0[];
extern s32 DebugPrint();
extern int allocate_colored_effect_slot(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");
void draw_flickering_mesh_effect(struct Moby *m) __asm__("FUN_L08_002f5e58");

void update_flickering_mesh_effect(struct Moby *moby) __asm__("FUN_L08_002f70a0");

void update_flickering_mesh_effect(struct Moby *moby) {
    FlickerLightVars *light_vars;
    Vec4 pos;
    f32 view, draw_fade_distance, fade;
    s32 hidden;
    PointLight *point_light_slot;
    Vec4f *at;
    int i;

    if (moby == NULL)
        return;
    light_vars = (FlickerLightVars *)moby->pvars;
    if (light_vars == NULL)
        return;
    hidden = 0;
    at = &moby->pos;
    if (moby->state == 0) {
        light_vars->light = -1;
        light_vars->particle_timer = light_vars->particle_ticks;
        qcopy(&light_vars->light_pos, at);
        if (light_vars->texture_w_count == 0.0f) {
            DebugPrint(D_L08_001FC220);
            light_vars->texture_w_count = 1.0f;
        } else if (8.0f < light_vars->texture_h_count) {
            DebugPrint(D_L08_001FC250, light_vars->texture_h_count);
        }
        if (light_vars->texture_h_count == 0.0f) {
            DebugPrint(D_L08_001FC2A0);
            light_vars->texture_h_count = 1.0f;
        } else if (8.0f < light_vars->texture_w_count) {
            DebugPrint(D_L08_001FC2D0, light_vars->texture_w_count);
        }
        for (i = 0; i < 8; i++) {
            light_vars->noise[i] = random_float_between(light_vars->jitter, 1.0f);
            light_vars->goal[i] = random_float_between(light_vars->jitter, 1.0f);
        }
        moby->state = 1;
    }
    moby->scale = moby->pclass->scale * (light_vars->size_x > light_vars->size_y ? light_vars->size_x : light_vars->size_y) * 0.25f;
    view = convert_integer_to_float(moby->draw_dist);
    draw_fade_distance = FUN_001f9b48(at, D_L08_001675C0);
    fade = view - light_vars->fade_range;
    if (fade < 0.0f)
        fade = 0.0f;
    if (view < draw_fade_distance) {
        light_vars->intensity = 0.0f;
        hidden = 1;
    } else {
        if (fade < draw_fade_distance) {
            draw_fade_distance -= fade;
            draw_fade_distance /= light_vars->fade_range;
            fade = 1.0f - draw_fade_distance;
        } else
            fade = 1.0f;
        light_vars->intensity = fade;
    }
    pos.q = *(u128 *)at;
    pos.f[3] = convert_integer_to_float(light_vars->radius);
    if ((moby->visible != 0 || fast_bsphere_check((char *)&pos, 255.0f) != -1) && !hidden) {
        enqueue_callback_list_1(draw_flickering_mesh_effect, moby);
        if (light_vars->flags & 4) {
            if (light_vars->light == -1) {
                light_vars->light = allocate_colored_effect_slot(light_vars->light_pos.f, (f32)light_vars->radius,
                                            convert_integer_to_float(light_vars->falloff) / 100.0f,
                                            light_vars->intensity * convert_integer_to_float(light_vars->light_red) / 100.0f,
                                            light_vars->intensity * convert_integer_to_float(light_vars->light_green) / 100.0f,
                                            light_vars->intensity * convert_integer_to_float(light_vars->light_blue) / 100.0f);
            } else {
                point_light_slot = &D_L08_00180AC0[light_vars->light];
                point_light_slot->red = light_vars->intensity * convert_integer_to_float(light_vars->light_red) / 100.0f;
                point_light_slot->green = light_vars->intensity * convert_integer_to_float(light_vars->light_green) / 100.0f;
                point_light_slot->blue = light_vars->intensity * convert_integer_to_float(light_vars->light_blue) / 100.0f;
            }
        }
    } else if (light_vars->light != -1) {
        release_point_light_slot(light_vars->light);
        light_vars->light = -1;
    }
    light_vars->flags |= 2;
}

extern int D_L08_0015F5C4;

#include "qcopy.h"
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
#include "sda.h"
typedef struct {
    char pad00[4];
    unsigned short sub;
    char pad06[0x1A];
    void *ptr20;
    char pad24[0x1C];
    char body[0x64];
    float pitch;
    float yaw;
    char padAC[4];
    float height;
    char padB4[0xC];
    char head[0x68];
    float head_yaw;
    char pad12C[0x14];
    float target[4];
    char pad150[4];
    int idle_timer;
    int moving_timer;
    int target_timer;
} WD;
typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0x10];
    unsigned char opacity;
    char pad32[0x16];
    float yaw;
    char pad4C[7];
    unsigned char animation;
    char pad54[0x1C];
    unsigned char flags70;
    char pad71[7];
    WD *data;
    char pad7C[3];
    unsigned char b7F;
} WM;
extern void begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9e90_cf(float, float) __asm__("FUN_001fa688");
extern float FUN_001fa688_cf(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float truncate_float_to_s32_cf(float, float) __asm__("FUN_002132a8");
extern float vector_length_xy(void *);
extern float vector_length_xyz_c(void *) __asm__("vector_length_xyz");
extern int update_npc_dialogue(void *, void *) __asm__("FUN_L00_00266448");
extern int initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");
extern int random_float_between_cf(float) __asm__("FUN_001fa6d0");
extern int tick_countdown_32(int *);
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("subtract_vector_xyz");
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");
typedef struct {
    char pad00[0x24];
    short h24;
    char pad26[2];
    unsigned char b28;
    char pad29[3];
    float f2C;
    char pad30[0x10];
    void *p40;
    char pad44[0x1C];
    char body[0x64];
    float pitch;
    float yaw;
    char padCC[4];
    float height;
    char padD4[0xC];
    char head[0x68];
    float head_yaw;
    char pad14C[0x14];
    float target[4];
    char pad170[0x10];
    int joint;
    char pad184[4];
    int moving_timer;
    int target_timer;
} WD8;
extern unsigned char D_0013D3B4 __attribute__((section(".data")));
extern char D_L08_001621E0[];
extern int commando_height_probe_distance_limit_gp __asm__("D_L08_001621D8") __attribute__((sda));
extern char *D_L08_001600EC;
extern void queue_planet_unlock_message(int) __asm__("FUN_L00_00262d38");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern float D_L08_001675C0_c[] __asm__("D_L08_001675C0");
extern void attach_big_head_cheat_manipulator_to_matching_moby(void *) __asm__("FUN_L02_002e0cd8");
extern void FUN_L00_0025a120(void *);
extern float FUN_001f9b48(void *, void *);
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void unlock_planet(int) __asm__("FUN_L00_002607d0");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");

/* Watcher update, same routine as batalia_deserter_update with two more states and the joint pose copy. */
void commando_update(WM *m) __asm__("FUN_L08_00302ce8");
void commando_update(WM *moby) {
    enum {
        COMMANDO_INITIALIZE = 0,
        COMMANDO_DIALOGUE_BEFORE_REPOSITION = 1,
        COMMANDO_DIALOGUE_AFTER_REPOSITION = 2,
        COMMANDO_POST_DIALOGUE = 3
    };
    L16WatchScratch scratch;
    WD8 *commando_vars = (WD8 *)moby->data;
    float rate;
    float head_rate;
    int tracking;
    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    if (moby->opacity) {
        if (FUN_001f9b48(moby->position, D_L08_001675C0_c) < (float)commando_height_probe_distance_limit_gp) {
            FUN_L00_0025a120(moby);
            moby->b7F = (commando_height_probe_distance_limit_gp * 13) >> 4;
        }
    }
    switch (moby->state) {
    case COMMANDO_INITIALIZE:
        ((unsigned char *)moby)[0x30] = 0xFF;
        if (moby->animation != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        if (D_0013DD4A && visited52[((unsigned char *)moby)[0xB0] + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        commando_vars->f2C = 3.0f;
        commando_vars->p40 = D_L08_001621E0;
        if (!D_0013D3B4) {
            moby->state = 1;
        } else {
            char *b = D_L08_001600EC;
            int j = commando_vars->joint;
            qcopy(moby->position, b + j * 0x80 + 0x30);
            qcopy((char *)moby + 0x40, b + j * 0x80 + 0x70);
            moby->state = 2;
        }
        initialize_npc_dialogue(moby, (char *)commando_vars + 0x20);
        break;
    case COMMANDO_DIALOGUE_BEFORE_REPOSITION:
        if (update_npc_dialogue(moby, (char *)commando_vars + 0x20)) {
            char *b = D_L08_001600EC;
            int j = commando_vars->joint;
            qcopy(moby->position, b + j * 0x80 + 0x30);
            qcopy((char *)moby + 0x40, b + j * 0x80 + 0x70);
            moby->state = 2;
            D_0013D3B4 = 1;
        }
        break;
    case COMMANDO_DIALOGUE_AFTER_REPOSITION:
        if (moby->flags70 & 2) {
            if (moby->animation != random_integer_below(2) + 1)
                blend_moby_animation(moby, random_integer_below(2) + 1, 0, scale_game_frames(20));
        }
        if (update_npc_dialogue(moby, (char *)commando_vars + 0x20)) {
            moby->state = 3;
            begin_dialogue_camera_transition(moby, 2.2f);
        }
        break;
    case COMMANDO_POST_DIALOGUE:
        if (D_L08_0015F5C4 != 2) {
            commando_vars->b28 = 1;
            if (commando_vars->h24 == 3) {
                unlock_planet(10);
                queue_planet_unlock_message(10);
                jet_clear_persistent(((unsigned char *)moby)[0xB0]);
                jet_reset_camera((char *)&D_0013E633_watch + 0xE9D, (char *)&D_0013E633_watch + 0xEAD);
                memcard_save_data(0, -1);
                mark_moby_for_removal(moby);
                return;
            }
            moby->state = 2;
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 1) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (vector_distance(moby->position, player) < 8.0f &&
            FUN_001f9e90_cf(
                moby->yaw,
                FUN_001fa688_cf(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                    moby->position[0],
                                ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                    moby->position[1])) < 1.5707964f) {
            if (vector_length_xyz_c(player + 0x80) > 0.01f)
                commando_vars->moving_timer = scale_game_frames_c(120);
            else
                tick_countdown_32(&commando_vars->moving_timer);
        } else if (commando_vars->moving_timer) {
            commando_vars->moving_timer = 0;
            qcopy(commando_vars->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (tick_countdown_32(&commando_vars->target_timer)) {
            float heading;
            commando_vars->target_timer =
                random_float_between_cf(multiply_global_factor_ed64(truncate_float_to_s32_cf(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, truncate_float_to_s32_cf(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(commando_vars->target, 6.0f, heading,
                                   truncate_float_to_s32_cf(0.0f, 30.0f) * DEG_TO_RAD);
            add_vector_xyz(commando_vars->target, commando_vars->target, moby->position);
        }
        if (commando_vars->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, commando_vars->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        subtract_vector_xyz_c(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(FUN_001fa688_cf(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -FUN_001fa688_cf(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        commando_vars->pitch = pitch;
        commando_vars->yaw = yaw * 0.6f;
        commando_vars->head_yaw = yaw * 0.4f;
    }
    if (big_head_cheat_enabled)
        commando_vars->height = 2.75f;
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, moby, commando_vars->body, 0);
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, moby, commando_vars->head, 1);
}
