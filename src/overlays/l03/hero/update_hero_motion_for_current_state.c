/* Level 03 hero motion: update_hero_motion_for_current_state, and find_first_nearby_registered_path. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero_state.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/game/usage_stats.h"
#include "rnc/overlay/hero_tables.h"
#include "rnc/math/vector.h"
#include "rnc/data_labels.h"
#include "rnc/input/pad_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/hero/hero_frame_update.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/overlay/shared/hero/adjust_hero_pose_targets_from_motion.h"

#include "rnc/overlay/l03/hero/update_hero_motion_for_current_state_globals.h"

extern f32 absolute_float(f32) __asm__("func_001F99C0");
extern s32 FUN_L00_0025df68(void *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);

s32 find_first_nearby_registered_path(Vec4 *from,
                                      s32 **outOwner,
                                      Vec4 *to,
                                      s32  *outI,
                                      f32  *outF,
                                      s32  *outKind,
                                      s32  *skip,
                                      s32  *only) __asm__("FUN_L03_00205830");

s32 find_first_nearby_registered_path(Vec4 *from,
                                      s32 **outOwner,
                                      Vec4 *to,
                                      s32  *outI,
                                      f32  *outF,
                                      s32  *outKind,
                                      s32  *skip,

                                      s32 *only) {
    Vec4  source_position;
    s32   gotI;
    f32   gotF;
    s32   i;
    Vec4 *source_position_ptr = &source_position;

    *(u128 *)source_position_ptr = *(u128 *)from;
    for (i = 0; i < registered_path_count; i++) {
        if (skip != 0 && (s32 *)l03_splines[i].path == skip)
            continue;
        if (only != 0 && (s32 *)l03_splines[i].path != only)
            continue;
        if (l03_splines[i].path->count == 0)
            continue;
        if (fast_vec_dist(&l03_splines[i].bounds, source_position_ptr) > l03_splines[i].bounds.f[3])
            continue;
        if (!FUN_L00_0025df68(l03_splines[i].path,
                              source_position_ptr,
                              to,
                              &gotI,
                              &gotF,
                              l03_splines[i].kind,
                              12.0f,
                              10.0f,
                              0.0f))
            continue;
        if (!(vector_distance_xy(source_position_ptr, to) < 1.7f))
            continue;
        if (!(absolute_float(source_position_ptr->f[2] - to->f[2]) < 1.5f))
            continue;
        *outOwner = (s32 *)l03_splines[i].path;
        *outI = gotI;
        *outF = gotF;
        *outKind = l03_splines[i].kind;
        return 1;
    }
    return 0;
}

#include "eetypes.h"
#include "qcopy.h"

#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"

struct MobyView {
    u8  pad0[0xC];
    f32 fC;
    union {
        Vec4 v10;
        f32  f10;
        struct {
            u8  pad[0x4];
            f32 v;
        } f14;
        struct {
            u8  pad[0x8];
            f32 v;
        } f18;
    } u10;
    u8  pad20[0x3];
    u8  b23;
    u8  pad24[0xC];
    u8  b30;
    u8  b31;
    s16 h32;
    union {
        s32 i34;
        u16 uh34;
    } u34;
    u8 pad38[0x8];
    union {
        Vec4 v40;
        struct {
            u8  pad[0x8];
            f32 v;
        } f48;
    } u40;
    u8               pad50[0x1];
    u8               b51;
    u8               b52;
    u8               b53;
    u8               pad54[0x1C];
    u8               b70;
    u8               pad71[0x7];
    struct MobyView *pm78;
    u8               pad7C[0x2A];
    s16              hA6;
    u8               padA8[0x18];
    Vec4             vC0;
};
#define MV(p) ((struct MobyView *)(p))
struct PadState {
    u8  pad0[0x128];
    f32 f128;
    f32 f12C;
    u8  pad130[0x70];
    union {
        unsigned long d1A0;
        s32           i1A0;
        struct {
            u8  pad[0x4];
            s32 v;
        } i1A4;
    } u1A0;
    u8  pad1A8[0x2C];
    s32 i1D4;
    u8  pad1D8[0x4];
    s32 i1DC;
};

struct PlayerJointTable {
    u8  pad0[0x60];
    f32 f60;
    f32 f64;
    u8  pad68[0xA8];
    f32 f110;
    f32 f114;
    f32 f118;
    u8  pad11C[0xA4];
    f32 f1C0;
    u8  pad1C4[0x4];
    f32 f1C8;
    u8  pad1CC[0xA4];
    f32 f270;
    f32 f274;
    f32 f278;
    u8  pad27C[0xA8];
    f32 f324;
    f32 f328;
    u8  pad32C[0x38];
    f32 f364;
    f32 f368;
    u8  pad36C[0x68];
    f32 f3D4;
    f32 f3D8;
    u8  pad3DC[0x38];
    f32 f414;
    f32 f418;
    u8  pad41C[0xED4];
    f32 f12F0;
    u8  pad12F4[0x4];
    f32 f12F8;
    u8  pad12FC[0x38];
    f32 f1334;
    f32 f1338;
    u8  pad133C[0x6C];
    f32 f13A8;
    u8  pad13AC[0x38];
    f32 f13E4;
    f32 f13E8;
    u8  pad13EC[0xA8];
    f32 f1494;
    f32 f1498;
};

extern void emit_hero_periodic_surface_effect(s32, s32) __asm__("FUN_L00_00209540");
extern void build_scaled_hero_move_impulse(f32, f32) __asm__("FUN_L00_002132b8");
extern void stop_hero_motion_without_forward_support(f32, f32) __asm__("FUN_L00_00217118");
extern void stick_target(f32, s32) __asm__("FUN_L00_002118c8");
extern void turn_to(s32, f32, f32, f32) __asm__("FUN_L00_00211e30");
extern s32  try_start_hero_item_idle_animation(void) __asm__("FUN_L00_00229a98");
extern void turn_hero_toward_move_direction_for_item(void) __asm__("FUN_L00_00211e98");
extern void set_hero_animation(s32, s32, f32) __asm__("FUN_L00_002323b8");
extern void
    adjust_hero_animation_speed_for_target_frame(f32, f32, f32, f32) __asm__("FUN_L00_00208f20");
extern void speed_step(f32, f32) __asm__("FUN_L00_00212088");
extern void reduce_and_apply_hero_planar_velocity_addend(f32) __asm__("FUN_L00_00216710");
extern void FUN_L00_002334d0(void *, void *, f32);
extern void clip_hero_horizontal_velocity(s32) __asm__("FUN_L01_00232978");
extern void correct_hero_airborne_wall_velocity(void) __asm__("FUN_L00_002121c0");
extern void spawn_hero_radial_particle_burst(void) __asm__("FUN_L00_00217450");
extern s32  FUN_L00_001ff600(f32, void *, void *);
extern void update_hero_water_height_and_bobbing(void) __asm__("FUN_L00_00227d40");
extern struct PadState D_0013C940;
#define PD D_0013C940
extern struct PlayerJointTable D_L03_0017A780;
#define AN D_L03_0017A780
struct VoiceSlot {
    u8               pad0[0x74];
    u8               b74;
    u8               pad75[0x13];
    struct MobyView *pm88;
};

extern u8 swim_animation_speed_scale_table[] __asm__("D_L03_0017C0C0");

extern void release_voice_slot_cf(s32, struct MobyView *, s32) __asm__("FUN_L00_00216e48");
extern void emit_hero_randomized_motion_effects(s32, s32) __asm__("FUN_L00_00209a40");
extern void emit_hero_surface_effect_burst(s32, s32, s32) __asm__("FUN_L00_00209ca8");
extern void pool_splash(s32, s32, s32, f32, f32, f32, f32) __asm__("FUN_L00_002a3ec8");
extern f32  spring_angle(f32 *, f32, f32 *, s32, f32, f32, f32) __asm__("FUN_L00_0025bc98");
extern void moby_point_position(struct MobyView *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern u8  *FUN_L00_0026f080(void *, void *, f32, f32);
extern s32  random_int_between(s32, s32) __asm__("FUN_L00_00257b90");
extern void FUN_L00_00262cf0(void *, void *, f32, f32, f32);
extern f32  FUN_L00_00209018(void *, s32);
extern f32  approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern void set_first_hero_easing_parameters(f32, f32) __asm__("FUN_L00_0020a1e0");
extern void FUN_L03_002030a0(f32, f32);
extern void FUN_L03_00203088(f32, f32);
extern void FUN_L03_00203070(f32, f32);
extern void emit_hero_periodic_surface_particles(s32, s32) __asm__("FUN_L00_00209848");
extern void fast_sin_cf(s32, s32) __asm__("FUN_L00_00209638");

#define GROUND_FRICTION()                                                                          \
    {                                                                                              \
        f32 len;                                                                                   \
        hero.motion.velocity.f[2] = 0.0f;                                                          \
        len = vector_length_xy(&hero.motion.velocity);                                             \
        FUN_L00_001ff600(                                                                          \
            frame_scale * -0.07999998f * len + len, &hero.motion.velocity, &hero.motion.velocity); \
    }
extern void FUN_L00_0025f730(void *, f32);
extern void step_angle_toward_target(f32 *, s32, f32, f32) __asm__("FUN_L00_0025bc00");
extern f32  gravity_z(void *) __asm__("FUN_L00_00233a78");
extern f32  vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
extern f32  forward_component(void *) __asm__("FUN_L00_00213350");
extern struct HeroTableRow2C D_L03_0017BD28[];
extern void set_gravity_relative_vector_length_xy(void *, void *, f32) __asm__("FUN_L00_00233ba0");
extern void gravity_set_z(void *, void *, f32) __asm__("FUN_L00_00233b20");
extern void offset_hero_position_against_gravity(void *, void *, f32) __asm__("FUN_L00_002336e8");
extern void
make_hit_descriptor(void *, struct MobyView *, s32, f32, void *) __asm__("FUN_L00_00259888");
extern s32
collect_mobies_in_sphere(f32, void *, s32, struct MobyView *, void *) __asm__("FUN_L00_001f2868");
extern f32  compute_directed_angle_delta(s32, f32, f32) __asm__("FUN_L00_0025bb38");
extern void update_hero_target_heading(f32, f32, f32) __asm__("FUN_L00_00214ed8");
extern void update_hero_planar_movement_velocity(void) __asm__("FUN_L00_002167d0");
extern void set_giant_clank_charged_blast_position(struct MobyView *,
                                                   void *) __asm__("FUN_L00_002a9b80");
extern s32  spawn_giant_clank_missile(
     struct MobyView *, void *, f32, f32, s32, f32, f32, f32) __asm__("FUN_L00_002a96f8");
extern void transform_hero_polar_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_002335c8");
extern s32  play_entity_sound(struct MobyView *, f32, void *, s32, f32, f32, s32, s32, s32) __asm__("FUN_L00_002598b0");

struct PlayerSpeedTuning {
    u8  pad0[8];
    f32 x08;
    f32 x0C;
    u8  pad10[8];
    f32 x18;
};

extern struct PlayerSpeedTuning D_L03_0017BEB8;
extern s32                      allocate_hero_voice_for_entry(s32, s32) __asm__("FUN_L00_00216de8");
extern void add_hero_forward_offset(void *, void *, f32) __asm__("FUN_L00_00233708");
extern void FUN_L00_00233810(void *, void *, f32);
extern s32  collision_line(void *, void *, s32, struct MobyView *, s32) __asm__("FUN_001efa68");
extern void transform_hero_local_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_00233660");
extern s32  is_moby_in_class_range_500_to_540(s32) __asm__("FUN_L00_0025e3b8");
extern void
spawn_nearby_effect(s32, struct MobyView *, s32, void *, void *, f32) __asm__("FUN_L00_00259bc8");
extern void emit_hero_timed_effects(void) __asm__("FUN_L00_00209ec8");
extern void gravity_flatten(void *, void *) __asm__("FUN_L00_002338d0");
extern s32  hero_slot_item_id(s32) __asm__("FUN_L00_0020d498");
extern void probe_hero_airborne_wall_contact(void) __asm__("FUN_L00_0020b5b8");
extern void update_hero_ledge_probe(void) __asm__("FUN_L01_0022d090");
extern f32  l03_ledge_shimmy_planar_speed_by_frame[] __asm__("D_L03_001C3DB0");
extern void air_accel(f32) __asm__("FUN_L01_00234358");
extern void rotate_hero_about_fixed_local_pivot(f32, f32, f32) __asm__("FUN_L00_002091d8");
extern void hero_update_hop_velocity(void) __asm__("FUN_L02_00223450");
extern void compute_hero_fixed_offset_world_position(void *) __asm__("FUN_L00_0020d2f8");
extern f32  randf_sym(f32, f32) __asm__("FUN_L00_00257c48");
extern s32  sample_path(void *, void *, s32 *, f32 *, f32, s32) __asm__("FUN_L00_0025d808");
extern s32  offset_path_index(void *, s32, s32, s32) __asm__("FUN_L00_0025d7a0");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void hero_integrate_velocity(void) __asm__("FUN_L00_00214108");
extern void spawn_random_alpha_rotated_particle(void *, void *, f32) __asm__("FUN_L00_00271b18");
extern void update_hero_lean_and_sway_parameters(void) __asm__("FUN_L00_00215ef8");
extern void turn_hero_toward_move_direction_with_stick_scale(void) __asm__("FUN_L00_00211f38");
extern f32  D_L03_0017BEB8_f[] __asm__("D_L03_0017BEB8");
extern void hero_speed_from_table(void) __asm__("FUN_L01_00232290");
extern void initialize_moby_transform_history(void *, void *) __asm__("FUN_L00_00262500");
extern void create_moby_transform_history_entry(void *, s32, s32) __asm__("FUN_L00_00262528");
extern void update_moby_transform_history(void *, s32) __asm__("FUN_L00_00262608");
extern void emit_filtered_entry_sparkles(void *, s32) __asm__("FUN_L00_00217368");
extern void launch_hero_wrench(void) __asm__("FUN_L00_00217658");
extern f32  set_planar_vel(f32) __asm__("FUN_L00_002120d8");
extern s32  test_hero_animation_frame_window(f32) __asm__("FUN_L00_00211870");

extern s32 D_L03_0015F688_gp __asm__("D_L03_0015F688") __attribute__((sda));
extern s32 D_L03_0015F68C_gp __asm__("D_L03_0015F68C") __attribute__((sda));
void       update_hero_motion_for_current_state(void) __asm__("FUN_L03_0020ef60");

void update_hero_motion_for_current_state(void) {
    struct MobyView *moby = MV(hero.moby);
    s32              sub = hero.state.step;
    s32              anim = moby->b53;
    f32              speed;
    Vec4             spatial_scratch_or_hit_descriptor;
    Vec4             v10;
    Vec4             spatial_scratch_or_hit_descriptor_tail;
    Vec4             motion_offset_or_position;
    Vec4             yaw_matrix_or_spatial_vectors[4];
    Vec4             inverse_yaw_matrix[4];
    Vec4             vC0;
    Vec4             vD0;

    switch (hero.state.current) {
    case HERO_STATE_IDLE:
    case HERO_STATE_LOOK:
    case HERO_STATE_STOP:
    case HERO_STATE_CROUCH:
    case HERO_STATE_ITEM_BUTTON_RELEASE_WAIT:
    case HERO_STATE_BLEND_TO_IDLE_ANIMATION:
    case HERO_STATE_LOOK_RELEASE_WAIT:
    case HERO_STATE_IDLE_ANIMATION_ENTRY:
    case HERO_STATE_DYING:
    case HERO_STATE_ATTACK_RECORD_FRAME_EXIT_WAIT:
    case HERO_STATE_ANIMATION_FRAME_EXIT_WAIT:
    case HERO_STATE_IDLE_ANIMATION_AFTER_ITEM_CANCELLATION:
    case HERO_STATE_DYING_SPEED_LIMITED:
        if (hero.wading != 0) {
            emit_hero_periodic_surface_effect(0xF, 0x1E);
        }
        build_scaled_hero_move_impulse(0.5f, frame_time * 2.0f);
        stop_hero_motion_without_forward_support(3.7f, 0.0f);
        stick_target(1.0f, 0);
        if (hero.state.current == HERO_STATE_LOOK || hero.state.current == 0x1E) {
            if (hero.unk20A5 != 0) {
                spatial_scratch_or_hit_descriptor.q = 0;
                spatial_scratch_or_hit_descriptor.f[0] = 1.0f;
                transform_vector_by_basis(&hero.motion.move_dir,
                                          &spatial_scratch_or_hit_descriptor,
                                          &D_L03_00166ED8[0x1F8]);
                hero.motion.move_yaw = *(f32 *)D_L03_00166ED8;
                turn_to(0, frame_scale_sq, frame_scale_sq * 0.0f, frame_time * 174.53293f);
                if (D_L03_0015F688_gp > 0 && fast_dec_timer(&D_L03_0015F688_gp) != 0) {
                    s32 t;
                    if (usage_stats.stat[18].count < 0xFFFF) {
                        usage_stats.stat[18].count++;
                    }
                    t = scale_game_frames(D_0015EEA4) / 600;
                    if (usage_stats.stat[18].unk2 < t) {
                        usage_stats.stat[18].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                    D_L03_0015F688_gp = -1;
                    usage_stats.stat[18].level_mask =
                        usage_stats.stat[18].level_mask | (1 << current_level_index) | 0x80000000;
                    D_L03_0015F68C_gp++;
                }
            }
        }
        if (hero.state.current == HERO_STATE_STOP) {
            if (try_start_hero_item_idle_animation() != 0) {
                break;
            }
        }
        if (hero.state.current == HERO_STATE_CROUCH || hero.state.current == 0x17) {
            if (hero.stick_magnitude > 0.2f || MV(hero.moby)->b53 == 0xE ||
                MV(hero.moby)->b53 == 0xF) {
                if (hero.state.current == HERO_STATE_CROUCH) {
                    turn_to(
                        0, frame_scale_sq * 0.002f, frame_scale_sq * 0.07f, frame_time * 6.981317f);
                } else if (hero.state.current == 0x17) {
                    turn_hero_toward_move_direction_for_item();
                }
                if (absolute_float(hero.motion.yaw_rate) > frame_time * 0.34906584f) {
                    s32 neg = hero.motion.yaw_rate < 0.0f;
                    s32 anim = neg + 0xE;
                    if (MV(hero.moby)->b53 != anim) {
                        set_hero_animation(anim, 0, scale_game_frames(5));
                    }
                    hero.anim_speed =
                        absolute_float(hero.motion.yaw_rate) / (frame_time * 3.3161256f);
                    if (hero.anim_speed < 0.55f) {
                        hero.anim_speed = 0.55f;
                    }
                    if (hero.anim_speed > 2.5f) {
                        hero.anim_speed = 2.5f;
                    }
                } else if (hero.motion.yaw_rate == 0.0f) {
                    s32 anim = 0xD;
                    if (hero.state.current == 0x17) {
                        anim = get_hero_idle_animation_index(0);
                    }
                    if (MV(hero.moby)->b53 != anim) {
                        set_hero_animation(anim, 0, scale_game_frames(7));
                    }
                }
            }
        }
        hero.target_speed = 0.0f;
        if (hero.state.current == HERO_STATE_STOP) {
            f32 s = hero.motion.move_slope + 1.0f;
            if (s > 1.0f) {
                s = 1.0f;
            }
            speed = frame_time_sq * 12.0f * s;
            hero.unk2290 = speed;
            if (MV(hero.moby)->b52 == MV(hero.moby)->b53 && MV(hero.moby)->b52 != 0x14) {
                f32 r = hero.motion.flat_speed / speed;
                if (scale_game_frames(5) < r && MV(hero.moby)->b51 < 5.5f) {
                    adjust_hero_animation_speed_for_target_frame(13.0f, r, 0.5f, -1.0f);
                    if (hero.anim_speed > 1.1f) {
                        hero.anim_speed = 1.1f;
                    }
                } else if (MV(hero.moby)->b51 > 6.5f) {
                    hero.anim_speed = 1.0f;
                }
            }
        } else if (hero.state.current == HERO_STATE_CROUCH) {
            speed = frame_time_sq * 6.48f;
        } else {
            speed = frame_time_sq * 12.6f;
        }
        speed_step(0.0f, speed);
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(speed);
        if (hero.on_magnet_ground != 0 && D_0013D4DC[0] != 0) {
            if (hero.ground_distance > 0.2f) {
                FUN_L00_002334d0(
                    &hero.motion.velocity, &hero.motion.resolved_velocity, frame_time_sq * 24.0f);
            } else {
                FUN_L00_002334d0(
                    &hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
                if (hero.ground_moby != NULL) {
                    qcopy(&hero.motion.pos, &hero.ground_point);
                    hero.ground_distance = 0.0f;
                    hero.air_frames.s = 0;
                }
            }
        } else {
            clip_hero_horizontal_velocity(0);
            if (hero.air_frames.s != 0) {
                FUN_L00_002334d0(
                    &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
                correct_hero_airborne_wall_velocity();
            } else {
                FUN_L00_002334d0(
                    &hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
            }
        }
        break;
    case HERO_STATE_SINK:
        if (MV(hero.sink_effect_moby) == NULL) {
            struct MobyView *m = create_moby(0x52A);
            s32              h40 = 0x40;
            hero.sink_effect_moby = (struct Moby *)m;
            if (m == NULL) {
                goto drop;
            }
            qcopy(&m->u10.v10, &hero.motion.pos);
            qcopy(&m->u40.v40, &hero.motion.rot);
            m->h32 = h40;
            MV(hero.sink_effect_moby)->b31 = 1;
            MV(hero.sink_effect_moby)->b30 = 0;
            MV(hero.sink_effect_moby)->b23 = 0;
            MV(hero.sink_effect_moby)->u34.uh34 |= 0x800;
            update_entity(MV(hero.sink_effect_moby));
            allocate_voice_for_target_entry(0, 0, MV(hero.sink_effect_moby));
        }
        if (MV(hero.sink_effect_moby) != NULL) {
            struct MobyView *m;
            struct MobyView *o;
            MV(hero.sink_effect_moby)->b23 += truncate_float_to_s32(frame_scale * 5.0f);
            if (MV(hero.sink_effect_moby)->b23 > 0x80) {
                MV(hero.sink_effect_moby)->b23 = 0x80;
            }
            m = MV(hero.sink_effect_moby);
            o = MV(hero.moby);
            qcopy(&m->u10.v10, &o->u10.v10);
            qcopy(&m->u40.v40, &o->u40.v40);
            update_entity(m);
        }
    drop:
        hero.motion.velocity.f[2] = 0.0f;
        {
            f32 len = vector_length_xy(&hero.motion.velocity);
            FUN_L00_001ff600(frame_scale * -0.07999998f * len + len,
                             &hero.motion.velocity,
                             &hero.motion.velocity);
        }
        update_hero_water_height_and_bobbing();
        break;
    case HERO_STATE_SWIM:
    case HERO_STATE_SWIM_IDLE:
    case HERO_STATE_SWIM_WITH_CROUCH_INPUT:
        if (hero.state.current == 0x35) {
            release_voice_slot_cf(5, MV(hero.moby), 0x13);
        } else {
            s32 idx = hero.pack_swim_voice;
            if (idx != -1) {
                struct VoiceSlot *e = (struct VoiceSlot *)(((u8 *)&voice_pool) + idx * 0x70);
                if (e->pm88 == MV(hero.moby) && e->b74 != 0) {
                    release_voice_slot(idx);
                }
            }
            hero.pack_swim_voice = -1;
        }
        if (hero.splash_effect_timer != 0) {
            s32 n;
            fast_dec_timer_s16(&hero.splash_effect_timer);
            n = (s16)hero.splash_effect_timer >> 2;
            emit_hero_randomized_motion_effects(n > 8 ? 8 : n, 0);
        }
        if (hero.state.current == 0x33 && hero.anim_changing == 0 && hero.anim_frame > 0.0f &&
            hero.anim_frame < 10.0f) {
            emit_hero_randomized_motion_effects(4, 1);
        }
        if (hero.state.current == 0x34 && hero.state.prev == 0x35 &&
            scale_game_frames(15) > hero.state_timer) {
            emit_hero_randomized_motion_effects((scale_game_frames(20) - hero.state_timer) / 3 + 1,
                                                1);
        }
        if (hero.state.step == 1) {
            if (hero.state_timer == 1) {
                emit_hero_surface_effect_burst(3, 0x10, 0);
                if (D_L03_001612D0 != 0) {
                    pool_splash(D_L03_001612D0,
                                D_L03_001612D8,
                                0,
                                hero.motion.pos.f[0],
                                hero.motion.pos.f[1],
                                0.4f,
                                -0.3f);
                }
                hero.splash_effect_timer = scale_game_frames(70);
            }
            if (++hero.unk8E0 > scale_game_frames(40)) {
                hero.state.step = 0;
            }
        }
        {
            f32          lo = frame_time * 0.6981317f;
            f32          m;
            f32          rate;
            f32          swim_pitch_easing_coefficient;
            f32          steer;
            f32          old;
            f32          diff;
            f32          k;
            f32          yaw;
            f32          t;
            struct Hero *pl;

            steer = 0.0f;
            m = PD.f128 < PD.f12C ? PD.f12C : PD.f128;
            rate = frame_scale_sq * 0.2f;
            swim_pitch_easing_coefficient = frame_scale_sq * 0.015f;
            lo = lo + (frame_time * 1.2217305f - lo) * m;
            if ((PD.u1A0.d1A0 & 0xC0) == 0x80 || hero.state.step == 1) {
                f32 s = PD.f12C * 1.5f;
                if (s > 1.0f) {
                    s = 1.0f;
                }
                if (s < 0.2f) {
                    s = 0.2f;
                }
                if (PD.i1DC != 0x79) {
                    hero.cw_turn_frames = 0;
                    hero.swim_square_hold_frames++;
                    s = hero.swim_square_hold_frames * 0.5f / scale_game_frames(20) + 0.5f;
                    if (s > 1.0f) {
                        s = 1.0f;
                    }
                }
                if (hero.state.step == 1 && s < 0.35f) {
                    s = 0.35f;
                }
                steer = s * 1.3613569f;
            } else if (PD.u1A0.i1A0 & 0x40) {
                f32 s = PD.f128 * 1.5f;
                if (s > 1.0f) {
                    s = 1.0f;
                }
                if (s < 0.2f) {
                    s = 0.2f;
                }
                if (PD.i1DC != 0x79) {
                    hero.swim_square_hold_frames = 0;
                    hero.cw_turn_frames++;
                    s = hero.cw_turn_frames * 0.5f / scale_game_frames(20) + 0.5f;
                    if (s > 1.0f) {
                        s = 1.0f;
                    }
                }
                if (hero.underwater_air < 1500.0f) {
                    s = 1.0f;
                }
                steer = s * -1.3613569f;
            } else {
                hero.cw_turn_frames = 0;
                hero.swim_square_hold_frames = 0;
            }
            if (PD.u1A0.i1A0 & 0x40) {
                hero.jump_button_frames++;
                if (PD.f128 > 0.75f) {
                    s32 t2;
                    if (++hero.swim_hard_press_frames > scale_game_frames(90) &&
                        usage_stats.stat[11].count < 0xFFFF) {
                        usage_stats.stat[11].count++;
                    }
                    t2 = scale_game_frames(D_0015EEA4) / 600;
                    if (usage_stats.stat[11].unk2 < t2) {
                        usage_stats.stat[11].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                    usage_stats.stat[11].level_mask =
                        usage_stats.stat[11].level_mask | (1 << current_level_index) | 0x80000000;
                } else {
                    hero.swim_hard_press_frames = 0;
                }
            } else {
                hero.jump_button_frames = 0;
                hero.swim_hard_press_frames = 0;
            }
            if (PD.u1A0.i1A4.v & 0x40) {
                if (D_L03_0015F5CC - hero.last_cross_press_frame < scale_game_frames(70)) {
                    hero.swim_cross_tap_count++;
                }
                hero.last_cross_press_frame = D_L03_0015F5CC;
            }
            if (hero.jump_button_frames > scale_game_frames(70)) {
                hero.swim_cross_tap_count = 0;
            }
            if (PD.u1A0.i1A0 & 0x80) {
                hero.swim_cross_tap_count = 0;
            }
            if (hero.state.current == 0x34) {
                steer = 0.0f;
            }
            old = hero.motion.rot.f[1];
            spring_angle(&hero.motion.rot.f[1],
                         steer,
                         &hero.swim_roll_velocity,
                         0,
                         swim_pitch_easing_coefficient,
                         rate,
                         lo);
            diff = fast_subtract_rotations(hero.motion.rot.f[1], old);
            stick_target(1.0f, 0);
            turn_to(0, frame_scale_sq * 0.007f, frame_scale_sq * 0.08f, frame_time * 5.2359877f);
            hero.unk8EC = hero.motion.yaw_rate;
            fast_dec_timer(&hero.unk8F4);
            if (hero.state.step == 1) {
                hero.target_speed = frame_time * 6.0f;
            } else if (hero.state.current == 0x35) {
                if (PD.u1A0.i1A0 & 0xA) {
                    hero.target_speed = frame_time * 7.0f;
                } else {
                    hero.target_speed = frame_time * 7.0f * 0.4f;
                }
            } else {
                hero.target_speed = frame_time * 3.0f;
            }
            k = hero.stick_magnitude;
            if ((PD.u1A0.i1A0 & 0xC0) || hero.state.step == 1) {
                if (k < 1.0f) {
                    k = 1.0f;
                }
            }
            if (hero.stick_magnitude > 0.15f) {
                f32 mn = 0.35f;
                if (hero.state.current == 0x35) {
                    mn = 0.4f;
                }
                if (k < mn) {
                    k = mn;
                }
            }
            if (hero.state.current != 0x35 || !(PD.u1A0.i1A0 & 0xA)) {
                hero.target_speed *= k;
            };
            if (fast_dec_timer(&hero.puff_timer) != 0) {
                s32 lo2;
                s32 hi2;
                pl = &hero;
                moby_point_position(MV(pl->moby), 4, &spatial_scratch_or_hit_descriptor);
                spatial_scratch_or_hit_descriptor_tail.q = 0;
                spatial_scratch_or_hit_descriptor_tail.f[0] = 0.4f;
                transform_vector_by_basis(&spatial_scratch_or_hit_descriptor_tail,
                                          &spatial_scratch_or_hit_descriptor_tail,
                                          &MV(pl->moby)->vC0);
                fast_vec_add(&spatial_scratch_or_hit_descriptor,
                             &spatial_scratch_or_hit_descriptor,
                             &spatial_scratch_or_hit_descriptor_tail);
                qcopy(&v10, &hero.motion.displacement);
                fast_vec_normalize(&spatial_scratch_or_hit_descriptor_tail,
                                   &spatial_scratch_or_hit_descriptor_tail,
                                   frame_time * 1.5f);
                fast_vec_add(&v10, &v10, &spatial_scratch_or_hit_descriptor_tail);
                v10.f[2] += frame_time * 2.0f;
                FUN_L00_0026f080(&spatial_scratch_or_hit_descriptor, &v10, 25200.0f, -1.0f);
                if (random_integer_below(100) < 40.0f) {
                    lo2 = scale_game_frames(4);
                    hi2 = 11;
                } else {
                    lo2 = scale_game_frames(40);
                    hi2 = 90;
                }
                pl->puff_timer = random_int_between(lo2, scale_game_frames(hi2));
            }
            if (hero.state.current == 0x35 && fast_dec_timer_s16(&hero.pack_particle_timer) != 0) {
                struct MobyView *gm = MV(hero.items[3].moby);
                if (gm != NULL && gm->hA6 == 0x261) {
                    s32 i;
                    for (i = 0; i < 4; i++) {
                        moby_point_position(gm, i % 2, &spatial_scratch_or_hit_descriptor);
                        spatial_scratch_or_hit_descriptor_tail.q = 0;
                        spatial_scratch_or_hit_descriptor_tail.f[0] = -0.15f;
                        transform_vector_by_basis(&spatial_scratch_or_hit_descriptor_tail,
                                                  &spatial_scratch_or_hit_descriptor_tail,
                                                  &MV(hero.moby)->vC0);
                        fast_vec_add(&spatial_scratch_or_hit_descriptor,
                                     &spatial_scratch_or_hit_descriptor,
                                     &spatial_scratch_or_hit_descriptor_tail);
                        spatial_scratch_or_hit_descriptor.f[0] += random_float_between(0.0f, 0.03f);
                        spatial_scratch_or_hit_descriptor.f[1] += random_float_between(0.0f, 0.03f);
                        spatial_scratch_or_hit_descriptor.f[2] += random_float_between(0.0f, 0.03f);
                        qcopy(&v10, &hero.motion.displacement);
                        FUN_L00_00262cf0(&v10,
                                         &v10,
                                         frame_time * -0.75f,
                                         hero.motion.rot.f[2],
                                         hero.motion.rot.f[1]);
                        hero.pack_particle_timer =
                            truncate_float_to_s32((frame_time * 7.0f - hero.ground_speed) /
                                                  (frame_time * 7.0f) * scale_game_frames(5));
                        FUN_L00_0026f080(&spatial_scratch_or_hit_descriptor,
                                         &v10,
                                         random_float_between(6300.0f, 9450.0f),
                                         -1.0f);
                    }
                }
            }
            if (hero.state.current == 0x35) {
                speed_step(frame_time_sq * 13.0f, frame_time_sq * 8.0f);
            } else {
                speed_step(frame_time_sq * 7.0f, frame_time_sq * 4.0f);
            }
            yaw = fast_add_rotations(-hero.motion.rot.f[1], 0.0f);
            if (hero.state.current == 0x35) {
                build_spherical_offset(
                    &hero.motion.velocity, hero.ground_speed, hero.motion.rot.f[2], yaw);
            } else if (hero.state.current == 0x33) {
                f32 sp = hero.ground_speed *
                         FUN_L00_00209018(swim_animation_speed_scale_table, 0x1E) * 5.0f;
                if (hero.state.prev == 0x35 && scale_game_frames(15) > hero.state_timer) {
                    if (sp < frame_time * 4.5f) {
                        sp = frame_time * 4.5f;
                    }
                }
                build_spherical_offset(&hero.motion.velocity, sp, hero.motion.rot.f[2], yaw);
            } else if (hero.state.current == 0x34) {
                f32 len = fast_vec_length(&hero.motion.momentum);
                if (hero.state.prev == 0x35) {
                    approach_value(&len, 0.0f, frame_time_sq * 8.0f);
                } else {
                    approach_value(&len, 0.0f, frame_time_sq * 4.0f);
                }
                fast_vec_normalize(&hero.motion.momentum, &hero.motion.momentum, len);
                qcopy(&hero.motion.velocity, &hero.motion.momentum);
            }
            clip_hero_horizontal_velocity(0);
            t = hero.unk8EC * fast_cos(yaw) * -7.0f;
            spring_angle(&hero.motion.rot.f[0],
                         t,
                         &hero.swim_lean_vel,
                         0,
                         0.007f,
                         0.17f,
                         frame_time * 1.2217305f);
            FUN_L03_00203070(0.009f, 0.22f);
            FUN_L03_00203088(0.04f, 0.2f);
            FUN_L03_002030a0(0.02f, 0.2f);
            AN.f114 = diff * 47.0f;
            AN.f118 = t * 1.5f;
            AN.f110 = t * 1.4f;
            if (AN.f114 > 0.87266463f) {
                AN.f114 = 0.87266463f;
            } else if (AN.f114 < -0.87266463f) {
                AN.f114 = -0.87266463f;
            }
            AN.f364 = 0.03f;
            AN.f414 = 0.03f;
            AN.f368 = 0.22f;
            AN.f324 = diff * 40.0f;
            AN.f418 = 0.22f;
            AN.f3D4 = diff * 40.0f;
            AN.f328 = t * 1.1f;
            AN.f3D8 = t * 1.1f;
        }
        break;
    case HERO_STATE_HURT_WITH_DISPLACEMENT_VELOCITY: {
        s32 n = 10 - hero.state_timer / 3;
        emit_hero_randomized_motion_effects(n > 8 ? 8 : n, 0);
        GROUND_FRICTION();
        clip_hero_horizontal_velocity(0);
        break;
    }
    case HERO_STATE_WATER_SURFACE_HURT:
        emit_hero_periodic_surface_effect(0xF, 0x1E);
        GROUND_FRICTION();
        clip_hero_horizontal_velocity(0);
        update_hero_water_height_and_bobbing();
        break;
    case HERO_STATE_DROWN:
    case HERO_STATE_DYING_ZERO_HEALTH: {
        f32 len = fast_vec_length(&hero.motion.momentum);
        approach_value(&len, 0.0f, frame_time_sq * 4.0f);
        fast_vec_normalize(&hero.motion.momentum, &hero.motion.momentum, len);
        qcopy(&hero.motion.velocity, &hero.motion.momentum);
        if (fast_dec_timer(&hero.puff_timer) == 2) {
            moby_point_position(MV(hero.moby), 4, &spatial_scratch_or_hit_descriptor);
            spatial_scratch_or_hit_descriptor_tail.q = 0;
            spatial_scratch_or_hit_descriptor_tail.f[0] = 0.4f;
            transform_vector_by_basis(&spatial_scratch_or_hit_descriptor_tail,
                                      &spatial_scratch_or_hit_descriptor_tail,
                                      &MV(hero.moby)->vC0);
            fast_vec_add(&spatial_scratch_or_hit_descriptor,
                         &spatial_scratch_or_hit_descriptor,
                         &spatial_scratch_or_hit_descriptor_tail);
            qcopy(&v10, &hero.motion.displacement);
            fast_vec_normalize(&spatial_scratch_or_hit_descriptor_tail,
                               &spatial_scratch_or_hit_descriptor_tail,
                               frame_time * 1.5f);
            fast_vec_add(&v10, &v10, &spatial_scratch_or_hit_descriptor_tail);
            v10.f[2] += frame_time * 2.0f;
            FUN_L00_0026f080(&spatial_scratch_or_hit_descriptor, &v10, 25200.0f, -1.0f);
            if (scale_game_frames(40) > hero.state_timer) {
                hero.puff_timer = random_int_between(scale_game_frames(2), scale_game_frames(5)) +
                                  hero.state_timer / 5;
            } else if (scale_game_frames(70) < hero.state_timer) {
                hero.puff_timer = random_int_between(scale_game_frames(10), scale_game_frames(20));
            }
        }
        if (hero.drown_sink_speed > 0.0f) {
            hero.drown_sink_speed = 0.0f;
        }
        approach_value(&hero.drown_sink_speed, -(frame_time * 1.5f), frame_time_sq * 1.5f);
        hero.motion.velocity.f[2] = hero.drown_sink_speed;
        break;
    }
    case HERO_STATE_WATER_SURFACE_MOVE:
    case HERO_STATE_WATER_SURFACE_ENTRY:
        if (hero.state.current == 0x37) {
            emit_hero_periodic_surface_effect(0xF, 0x1E);
        }
        if (frame_time * 1.5f < hero.motion.flat_speed) {
            emit_hero_periodic_surface_particles(4, 0xC);
        }
        if (hero.state.current == 0x36 && frame_time < hero.motion.flat_speed) {
            fast_sin_cf(0, 1);
        }
        if (hero.splash_effect_timer != 0) {
            s32 n = --hero.splash_effect_timer >> 3;
            emit_hero_randomized_motion_effects(n > 6 ? 6 : n, 0);
        }
        hero.anim_speed = 0.6f;
        stick_target(frame_time * 3.0f, 0);
        if (PD.i1D4 == 0) {
            if (hero.target_speed < frame_time * 3.0f * 0.5f) {
                hero.target_speed = frame_time * 3.0f * 0.5f;
            }
        }
        turn_to(0, frame_scale_sq * 0.007f, frame_scale_sq * 0.08f, frame_time * 5.2359877f);
        fast_dec_timer(&hero.unk698);
        if (hero.state.current == 0x36) {
            if (PD.i1D4 != 0 && hero.unk698 != 0) {
                hero.target_speed = frame_time * 3.0f;
            }
            if (absolute_float(hero.motion.yaw_error) > 0.7853982f) {
                hero.target_speed = 0.0f;
            }
            speed_step(frame_time_sq * 4.0f, frame_time_sq * 5.0f);
            {
                f32 sp = hero.ground_speed *
                         FUN_L00_00209018(swim_animation_speed_scale_table, 0x1E) * 5.5f;
                hero.motion.velocity.f[0] = fast_cos(hero.motion.rot.f[2]) * sp;
                hero.motion.velocity.f[1] = fast_sin(hero.motion.rot.f[2]) * sp;
                hero.motion.velocity.f[2] = 0.0f;
            }
        } else {
            hero.ground_speed = 0.0f;
            clear_u64_value(&hero.motion.velocity);
        }
        if (hero.state.current == 0x36 && hero.anim_changing == 0 && hero.anim_frame > 0.0f) {
            if (hero.anim_frame < 10.0f) {
                emit_hero_randomized_motion_effects(4, 1);
            }
        }
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 4.2f);
        clip_hero_horizontal_velocity(0);
        update_hero_water_height_and_bobbing();
        break;
    case HERO_STATE_WAIT_FOR_ITEM_RELEASE_THEN_IDLE: {
        s32 anim = -1;
        s32 frames = 0;
        if (hero.on_ice_ground != 0) {
            build_scaled_hero_move_impulse(0.7f, frame_time * 0.0f);
        } else {
            build_scaled_hero_move_impulse(0.5f, frame_time * 2.0f);
        }
        stop_hero_motion_without_forward_support(3.7f, 0.0f);
        stick_target(1.0f, 0);
        speed_step(frame_time_sq * 12.6f, frame_time_sq * 12.6f);
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.6f);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.displacement, frame_time_sq * 25.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        if (anim != -1 && MV(hero.moby)->b52 != 0x36) {
            set_hero_animation(anim, 0, frames);
        } else if ((MV(hero.moby)->b70 & 2) && MV(hero.moby)->b52 == 0x36) {
            s32 an = get_hero_idle_animation_index(0);
            set_hero_animation(an, 0, scale_game_frames(5));
        }
        break;
    }
    case HERO_STATE_GRAPPLE_TARGET_ENTRY:
    case HERO_STATE_ITEM_BUTTON_HOLD_ANIMATION_WAIT:
        stop_hero_motion_without_forward_support(3.7f, 0.0f);
        hero.target_speed = 0.0f;
        speed_step(frame_time_sq * 12.6f, frame_time_sq * 12.6f);
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.6f);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity, frame_time_sq * 24.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        if (MV(hero.grapple_target) != NULL) {
            hero.motion.move_yaw =
                fast_arc_tan(MV(hero.grapple_target)->u10.f10 - moby->u10.f10,
                             MV(hero.grapple_target)->u10.f14.v - moby->u10.f14.v);
            turn_to(0, frame_scale_sq * 0.02f, frame_scale_sq * 0.15f, frame_time * 6.2831855f);
        }
        break;
    case HERO_STATE_APPROACH_TARGET_MOBY: {
        f32 hit;
        f32 hit2;
        qcopy(&spatial_scratch_or_hit_descriptor, &MV(hero.grapple_target)->u10.v10);
        hero.unk970 = fast_vec_dist(&hero.motion.pos, &spatial_scratch_or_hit_descriptor);
        hero.target_arrival_frames = scale_game_frames(60);
        if (hero.grapple_pull_speed > 0.0f) {
            hero.target_arrival_frames = hero.unk970 / hero.motion.speed_xy;
        }
        if (hero.state.step == 0) {
            approach_value(&hero.grapple_pull_speed, frame_time * 27.0f, frame_time_sq * 80.0f);
            hero.motion.move_yaw =
                fast_arc_tan(MV(hero.grapple_target)->u10.f10 - moby->u10.f10,
                             MV(hero.grapple_target)->u10.f14.v - moby->u10.f14.v);
            turn_to(0, frame_scale_sq * 0.03f, frame_scale_sq * 0.15f, frame_time * 7.330383f);
        } else if (hero.state.step == 1) {
            approach_value(&hero.grapple_pull_speed, 0.0f, frame_time_sq * 42.0f);
        }
        if (solve_quadratic_equation(&hit,
                                     &hit2,
                                     -(frame_time_sq * 42.0f) * 0.5f,
                                     hero.grapple_pull_speed,
                                     -(hero.unk970 - 1.9f)) > 0 &&
            hit > 0.0f) {
            hero.target_arrival_frames = hit;
            hero.state.step = 1;
        }
        if (scale_game_frames(20) > hero.target_arrival_frames) {
            hero.grapple_pull_ending = 1;
        }
        if (hero.grapple_pull_ending != 0) {
            step_angle_toward_target(&hero.motion.rot.f[1], 0, 0.0f, frame_time * 3.8397243f);
        } else {
            f32 an = fast_add_rotations(
                -fast_arc_tan(
                    vector_distance_xy(&hero.motion.pos, &spatial_scratch_or_hit_descriptor),
                    spatial_scratch_or_hit_descriptor.f[2] - hero.motion.pos.f[2]),
                1.5707964f);
            step_angle_toward_target(&hero.motion.rot.f[1], 0, an, frame_time * 6.4577184f);
        }
        spatial_scratch_or_hit_descriptor_tail.f[0] =
            fast_cos(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * -0.57f;
        spatial_scratch_or_hit_descriptor_tail.f[1] =
            fast_sin(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * -0.57f;
        spatial_scratch_or_hit_descriptor_tail.f[2] = 0.0f;
        fast_vec_add(&spatial_scratch_or_hit_descriptor_tail,
                     &spatial_scratch_or_hit_descriptor_tail,
                     &hero.motion.pos);
        fast_vec_sub(
            &v10, &MV(hero.grapple_target)->u10.v10, &spatial_scratch_or_hit_descriptor_tail);
        fast_vec_normalize(&hero.motion.velocity, &v10, hero.grapple_pull_speed);
        if (scale_game_frames(30) > hero.target_arrival_frames) {
            f32 t = scale_game_frames(30) - hero.target_arrival_frames;
            hero.motion.velocity.f[2] -= frame_time_sq * 29.0f * t;
        }
        break;
    }
    case HERO_STATE_CONSTRAIN_TO_TARGET_RADIUS: {
        approach_value(&hero.tether_radius, hero.grapple_target_radius, frame_time * 10.0f);
        qcopy(&hero.motion.velocity, &hero.motion.displacement);
        fast_vec_scale(&hero.motion.velocity, &hero.motion.velocity, 0.9f);
        hero.motion.velocity.f[2] -= frame_time_sq * 15.0f;
        qcopy(&spatial_scratch_or_hit_descriptor, &MV(hero.grapple_target)->u10.v10);
        hero.unk970 = fast_vec_dist(&hero.motion.pos, &spatial_scratch_or_hit_descriptor);
        fast_vec_add(&v10, &hero.motion.pos, &hero.motion.velocity);
        fast_vec_sub(&motion_offset_or_position, &v10, &spatial_scratch_or_hit_descriptor);
        fast_vec_normalize(
            &motion_offset_or_position, &motion_offset_or_position, hero.tether_radius);
        fast_vec_add(&spatial_scratch_or_hit_descriptor_tail,
                     &spatial_scratch_or_hit_descriptor,
                     &motion_offset_or_position);
        fast_vec_sub(
            &hero.motion.velocity, &spatial_scratch_or_hit_descriptor_tail, &hero.motion.pos);
        break;
    }
    case HERO_STATE_GRAPPLE_SWING_ENTRY: {
        f32 vz;
        s32 above;
        if ((hero.anim_flags & 2) && moby->b53 == 0x34) {
            set_hero_animation(0x35, 10, scale_game_frames(12));
            hero.anim_speed = 0.3f;
        }
        vz = hero.motion.velocity.f[2];
        if (MV(hero.swing_target_moby) != NULL && MV(hero.swing_target_moby)->pm78 != NULL &&
            MV(hero.swing_target_moby)->pm78->u34.i34 != 0) {
            f32 a = MV(hero.swing_target_moby)->u40.f48.v;
            if (fast_difference_between_rotations(a, hero.motion.rot.f[2]) > 90.0f) {
                a = fast_add_rotations(a, 3.1415927f);
            }
            spring_angle(&hero.swing_yaw,
                         a,
                         &hero.swing_yaw_vel,
                         0,
                         frame_scale_sq * 0.02f,
                         frame_scale_sq * 0.3f,
                         0.0f);
        }
        hero.motion.move_yaw = hero.swing_yaw;
        turn_to(0, frame_scale_sq * 0.02f, frame_scale_sq * 0.3f, frame_time * 4.712389f);
        fast_vec_sub(&spatial_scratch_or_hit_descriptor,
                     &MV(hero.swing_target_moby)->u10.v10,
                     &hero.motion.pos);
        fast_vec_normalize(
            &spatial_scratch_or_hit_descriptor, &spatial_scratch_or_hit_descriptor, 1.0f);
        if (spatial_scratch_or_hit_descriptor.f[2] < 0.23f) {
            spatial_scratch_or_hit_descriptor.f[2] = 0.23f;
        }
        clear_u64_value(&spatial_scratch_or_hit_descriptor_tail);
        spatial_scratch_or_hit_descriptor_tail.f[2] = hero.motion.rot.f[2];
        euler_rotation_matrix(yaw_matrix_or_spatial_vectors,
                              &spatial_scratch_or_hit_descriptor_tail);
        transpose_rotation_matrix(inverse_yaw_matrix, yaw_matrix_or_spatial_vectors);
        transform_vector(&spatial_scratch_or_hit_descriptor,
                         &spatial_scratch_or_hit_descriptor,
                         inverse_yaw_matrix);
        v10.f[0] = -fast_arc_tan(
            square_root(
                spatial_scratch_or_hit_descriptor.f[0] * spatial_scratch_or_hit_descriptor.f[0] +
                spatial_scratch_or_hit_descriptor.f[2] * spatial_scratch_or_hit_descriptor.f[2]),
            spatial_scratch_or_hit_descriptor.f[1]);
        v10.f[1] = fast_arc_tan(spatial_scratch_or_hit_descriptor.f[2],
                                spatial_scratch_or_hit_descriptor.f[0]);
        spring_angle(&hero.motion.rot.f[1],
                     v10.f[1],
                     &hero.motion.rot_spring_vel.f[1],
                     0,
                     frame_scale_sq * 0.04f,
                     frame_scale_sq * 0.2f,
                     frame_time * 9.599311f);
        spring_angle(&hero.motion.rot.f[0],
                     v10.f[0],
                     &hero.motion.rot_spring_vel.f[0],
                     0,
                     frame_scale_sq * 0.04f,
                     frame_scale_sq * 0.2f,
                     frame_time * 9.599311f);
        above = 1;
        if (!(hero.motion.pos.f[2] < MV(hero.swing_target_moby)->u10.f18.v)) {
            above = 0;
        }
        if (hero.unk99C != 0) {
            if (above && scale_game_frames(40) < hero.state_timer) {
                if (hero.stick_magnitude > 0.25f) {
                    f32 s;
                    stick_target(1.0f, 0);
                    s = frame_time * 0.21f * hero.stick_magnitude;
                    if (s > frame_time * 5.0f) {
                        s = frame_time * 5.0f;
                    }
                    motion_offset_or_position.f[0] = fast_cos(hero.motion.move_yaw) * s;
                    motion_offset_or_position.f[1] = fast_sin(hero.motion.move_yaw) * s;
                    motion_offset_or_position.f[2] = 0.0f;
                    fast_vec_add(
                        &hero.motion.velocity, &hero.motion.velocity, &motion_offset_or_position);
                }
            }
        }
        if (hero.unk99C != 0) {
            f32 s;
            if (vector_distance_xy(&hero.motion.pos, &MV(hero.swing_target_moby)->u10.v10) <
                    hero.unk9A0 * 1.57f ||
                hero.ground_distance > 1.5f) {
                FUN_L00_002334d0(
                    &hero.motion.velocity, &hero.motion.displacement, frame_time_sq * 40.0f);
                if (hero.swing_entry_push != 0 && gravity_z(&hero.motion.velocity) > 0.0f) {
                    fast_vec_sub(&motion_offset_or_position,
                                 &hero.motion.pos,
                                 &MV(hero.swing_target_moby)->u10.v10);
                    FUN_L00_002334d0(
                        &hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 57.0f);
                }
            }
            s = fast_vec_length(&hero.motion.velocity) * (frame_time * 0.015f);
            fast_vec_scale(&motion_offset_or_position, &hero.motion.velocity, -1.0f);
            fast_vec_normalize(&motion_offset_or_position, &motion_offset_or_position, s);
            fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &motion_offset_or_position);
        }
        {
            struct MobyView *r;
            if (MV(hero.swing_target_moby) != NULL &&
                (r = MV(hero.swing_target_moby)->pm78) != NULL) {
                f32 len = fast_vec_length(&hero.motion.velocity);
                if (r->fC < len) {
                    approach_value(&len, r->fC, frame_time_sq * 15.0f);
                    fast_vec_normalize(&hero.motion.velocity, &hero.motion.velocity, len);
                }
            }
        }
        {
            f32 a = fast_add_rotations(hero.motion.rot.f[2], -1.5707964f);
            f32 old;
            f32 d;
            vD0.f[0] = fast_cos(a) * 0.15f;
            vD0.f[1] = fast_sin(a) * 0.15f;
            vD0.f[2] = 0.0f;
            fast_vec_add(&vD0, &vD0, &hero.motion.pos);
            fast_vec_add(&motion_offset_or_position, &vD0, &hero.motion.velocity);
            fast_vec_sub(&vC0, &motion_offset_or_position, &MV(hero.swing_target_moby)->u10.v10);
            fast_vec_normalize(&vC0, &vC0, hero.unk9A4);
            fast_vec_add(&motion_offset_or_position, &MV(hero.swing_target_moby)->u10.v10, &vC0);
            fast_vec_sub(&hero.motion.velocity, &motion_offset_or_position, &vD0);
            old = hero.unk9A4;
            vendor_spring(&hero.unk9A4,
                          &hero.swing_length_vel,
                          hero.unk9A0,
                          hero.swing_rope_spring_k,
                          hero.unk9C8,
                          hero.swing_rope_max_speed);
            d = hero.unk9A4 - old;
            if (fast_vec_length(&vC0) > hero.unk9A4) {
                fast_vec_normalize(&vC0, &vC0, d);
                fast_vec_add(&hero.motion.pos, &hero.motion.pos, &vC0);
            }
        }
        if (hero.swing_entry_push != 0 && above) {
            f32 mx = frame_time * 14.0f;
            f32 sp = vector_length_xy(&hero.motion.velocity);
            if (sp < mx) {
                vendor_spring(&sp,
                              &hero.swing_push_speed,
                              mx,
                              frame_scale_sq * 0.047f,
                              frame_scale_sq * 0.3f,
                              0.0f);
                motion_offset_or_position.f[0] = fast_cos(hero.swing_yaw) * hero.swing_push_speed;
                motion_offset_or_position.f[1] = fast_sin(hero.swing_yaw) * hero.swing_push_speed;
                motion_offset_or_position.f[2] = 0.0f;
                fast_vec_add(
                    &hero.motion.velocity, &hero.motion.velocity, &motion_offset_or_position);
            }
        }
        if (hero.unk99C != 0 && hero.air_frames.s != 0) {
            if ((vz < 0.0f && hero.motion.velocity.f[2] > 0.0f) || hero.swing_anim_dir != 0) {
                f32 t;
                s32 up;
                hero.swing_entry_push = 0;
                t = square_root(hero.unk9A0 / hero.swing_gravity) * 3.1415927f;
                if (hero.swing_anim_dir != 0) {
                    up = hero.swing_anim_dir == 1;
                } else {
                    motion_offset_or_position.q = hero.motion.displacement.q;
                    up = forward_component(&motion_offset_or_position) > 0.0f;
                }
                if (up) {
                    if (moby->b53 != 0x35) {
                        set_hero_animation(0x35, 6, scale_game_frames(17));
                        hero.swing_anim_dir = 1;
                    } else if (MV(hero.moby)->b52 == MV(hero.moby)->b53) {
                        adjust_hero_animation_speed_for_target_frame(42.0f, t, 0.5f, -1.0f);
                        hero.swing_anim_dir = 0;
                    }
                } else {
                    if (moby->b53 != 0x35) {
                        set_hero_animation(0x35, 0x15, scale_game_frames(9));
                        hero.swing_anim_dir = 2;
                    } else if (MV(hero.moby)->b52 == MV(hero.moby)->b53) {
                        adjust_hero_animation_speed_for_target_frame(12.0f, t, 0.5f, -1.0f);
                        hero.swing_anim_dir = 0;
                    }
                }
            }
        }
        break;
    }
    case HERO_STATE_RISE_THEN_FALL_WITH_AIRBORNE_HIT_QUERY: {
        s32 t1;
        s32 t2;
        f32 r;
        f32 mx;
        hero.target_speed = 0.0f;
        speed_step(frame_time_sq * 30.0f, frame_time_sq * 30.0f);
        set_gravity_relative_vector_length_xy(
            &hero.motion.velocity, &hero.motion.velocity, hero.ground_speed);
        t1 = scale_game_frames(12);
        r = frame_time * 5.7f;
        mx = frame_time_sq * 70.0f;
        t2 = scale_game_frames(33);
        if (hero.state_timer < t1) {
            f32 h = gravity_z(&hero.motion.velocity);
            approach_value(&h, r, mx);
            gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, h);
        } else if (t2 < hero.state_timer) {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, hero.rise_fall_gravity);
            if (gravity_z(&hero.motion.velocity) < 0.0f) {
                hero.past_apex = 1.0f;
            }
        }
        if (hero.past_apex != 0.0f && hero.air_frames.s != 0) {
            qcopy(&spatial_scratch_or_hit_descriptor, &hero.motion.pos);
            spatial_scratch_or_hit_descriptor.f[2] -= 0.5f;
            yaw_matrix_or_spatial_vectors[0].f[0] = fast_cos(hero.motion.rot.f[2]);
            yaw_matrix_or_spatial_vectors[0].f[1] = fast_sin(hero.motion.rot.f[2]);
            yaw_matrix_or_spatial_vectors[0].f[2] = 0.0f;
            make_hit_descriptor(
                &v10, MV(hero.moby), 0x30000, 1.0f, &yaw_matrix_or_spatial_vectors[0]);
            collect_mobies_in_sphere(
                0.8f, &spatial_scratch_or_hit_descriptor, 0x10, MV(hero.moby), &v10);
        }
        if (hero.past_apex != 0.0f && hero.air_frames.s == 0) {
            if (hero.rise_fall_land_frames == 0) {
                l03_camera.shake_amount = 0.2f;
                l03_camera.shake_frames = scale_game_frames(40);
            }
            hero.rise_fall_land_frames++;
        }
        break;
    }
    case HERO_STATE_WRENCH_SLAM: {
        s32 t1;
        s32 t2;
        f32 r;
        f32 mx;
        if (hero.anim_changing != 0 || hero.anim_frame < D_L03_0017BD28[hero.attack_record].f18) {
            hero.items[0].timer = 2;
        }
        hero.target_speed = 0.0f;
        if (MV(hero.melee_target) != NULL) {
            f32 d = vector_distance_xy(&hero.motion.pos, &MV(hero.melee_target)->u10.v10);
            if (d > 2.0f && d < 5.0f) {
                hero.target_speed = frame_time * 4.5f;
            }
            speed_step(frame_time_sq * 25.0f, frame_time_sq * 25.0f);
            hero.motion.velocity.f[0] = fast_cos(hero.motion.rot.f[2]) * hero.ground_speed;
            hero.motion.velocity.f[1] = fast_sin(hero.motion.rot.f[2]) * hero.ground_speed;
        } else {
            speed_step(frame_time_sq * 15.0f, frame_time_sq * 15.0f);
            set_gravity_relative_vector_length_xy(
                &hero.motion.velocity, &hero.motion.velocity, hero.ground_speed);
        }
        if (hero.heading_locked != 0 || MV(hero.melee_target) != NULL) {
            hero.motion.move_yaw = hero.attack_target_yaw;
            turn_to(0, frame_scale_sq * 0.05f, frame_scale_sq * 0.2f, frame_time * 15.009831f);
        } else if (hero.anim_frame < 25.0f) {
            update_hero_target_heading(8.0f, 1.0471976f, 1.0471976f);
        }
        t1 = scale_game_frames(9);
        r = frame_time * 7.7f;
        if (hero.ground_distance > 2.25f) {
            r *= 0.5f;
        } else if (hero.ground_distance > 1.8f) {
            r *= 0.7f;
        }
        mx = frame_time_sq * 150.0f;
        t2 = scale_game_frames(14);
        if (hero.anim_changing == 0 && hero.anim_frame > 23.5f &&
            hero.motion.displacement.f[2] < 0.0f && hero.air_frames.s != 0) {
            qcopy(&spatial_scratch_or_hit_descriptor, &hero.motion.pos);
            offset_hero_position_against_gravity(
                &spatial_scratch_or_hit_descriptor, &spatial_scratch_or_hit_descriptor, -0.5f);
            yaw_matrix_or_spatial_vectors[0].f[0] = fast_cos(hero.motion.rot.f[2]) * 2.0f;
            yaw_matrix_or_spatial_vectors[0].f[1] = fast_sin(hero.motion.rot.f[2]) * 2.0f;
            yaw_matrix_or_spatial_vectors[0].f[2] = 0.0f;
            make_hit_descriptor(
                &v10, MV(hero.moby), 0x10000, 2.0f, &yaw_matrix_or_spatial_vectors[0]);
            v10.f[3] = 5627.925f;
            ((u8 *)&spatial_scratch_or_hit_descriptor_tail)[9] = 1;
            ((s16 *)&spatial_scratch_or_hit_descriptor_tail)[5] = 0x47;
            ((u8 *)&spatial_scratch_or_hit_descriptor_tail)[8] = 0;
            collect_mobies_in_sphere(
                0.4f, &spatial_scratch_or_hit_descriptor, 0x10, MV(hero.moby), &v10);
        }
        test_hero_animation_frame_window(27.0f);
        if (hero.state_timer < t1) {
            f32 h = gravity_z(&hero.motion.velocity);
            approach_value(&h, r, mx);
            gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, h);
        } else if (t2 < hero.state_timer) {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 130.0f);
            if (gravity_z(&hero.motion.velocity) < 0.0f) {
                hero.slam_descending = 1.0f;
            }
            if (gravity_z(&hero.motion.velocity) < frame_time * -37.0f) {
                gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, frame_time * -37.0f);
            }
        }
        break;
    }
    case HERO_STATE_CROUCH_SWING: {
        s32 jumping;
        hero.target_speed = 0.0f;
        if (hero.anim_changing != 0 || hero.anim_frame < D_L03_0017BD28[hero.attack_record].f18) {
            hero.items[0].timer = 2;
        }
        jumping = hero.state.prev == 1 && scale_game_frames(20) < hero.state.prev_timer;
        if (!jumping) {
            if (hero.heading_locked != 0 || MV(hero.melee_target) != NULL) {
                s32 ok = 1;
                if (hero.unk308 == 0) {
                    f32 d = fast_subtract_rotations(hero.attack_target_yaw, hero.motion.rot.f[2]);
                    f32 a = compute_directed_angle_delta(
                        1, hero.attack_target_yaw, hero.motion.rot.f[2]);
                    if (a > 4.363323f) {
                        ok = 0;
                    }
                    if (d > 0.0f) {
                        hero.anim_speed = 1.0f / (a + 1.0f);
                    }
                } else {
                    ok = 0;
                }
                hero.motion.move_yaw = hero.attack_target_yaw;
                if (ok) {
                    turn_to(
                        1, frame_scale_sq * 0.05f, frame_scale_sq * 0.2f, frame_time * 15.009831f);
                } else {
                    turn_to(
                        0, frame_scale_sq * 0.05f, frame_scale_sq * 0.2f, frame_time * 15.009831f);
                }
            } else {
                s32 ok = 1;
                if (hero.anim_changing == 0 && hero.anim_frame > 6.0f) {
                    ok = 0;
                }
                if (ok && hero.gravity_mode == 0) {
                    update_hero_target_heading(14.0f, 0.5235988f, 0.7853982f);
                }
            }
        }
        if (hero.anim_changing == 0 && test_hero_animation_frame_window(33.0f) != 0) {
            if (0 && hero.motion.speed_xy < frame_time * 2.0f) {
                hero.motion.velocity.f[0] += fast_cos(hero.motion.rot.f[2]) * (frame_time * 1.3f);
                hero.motion.velocity.f[1] += fast_sin(hero.motion.rot.f[2]) * (frame_time * 1.3f);
                qcopy(&hero.motion.resolved_velocity_xy, &hero.motion.velocity);
            }
            launch_hero_wrench();
        }
        speed_step(frame_time_sq * 37.0f, frame_time_sq * 28.0f);
        set_planar_vel(99999.0f);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    }
    case HERO_STATE_WRENCH_COMBO: {
        struct HeroTableRow2C *tb;
        if (hero.anim_changing != 0 || hero.anim_frame < D_L03_0017BD28[hero.attack_record].f20) {
            hero.items[0].timer = 2;
        }
        tb = &D_L03_0017BD28[hero.attack_record];
        hero.target_speed = 0.0f;
        if (MV(hero.melee_target) != NULL && scale_game_frames(4) > hero.state_timer) {
            f32 d = vector_distance_xy(&MV(hero.melee_target)->u10.v10, &hero.motion.pos) - 0.5f;
            f32 attack_spacing_offset;
            f32 tol;
            f32 mx;
            f32 diff;
            if (d < 0.0f) {
                d = 0.0f;
            }
            if (tb->kind == 0) {
                attack_spacing_offset = 0.75f;
                tol = 0.8f;
                mx = 4.0f;
            } else if (tb->kind == 1) {
                attack_spacing_offset = 0.6f;
                tol = 1.7f;
                mx = 4.0f;
            } else {
                attack_spacing_offset = 1.3f;
                tol = 1.7f;
                mx = 1.9f;
            }
            diff = d - attack_spacing_offset;
            if (absolute_float(diff) < tol + 3.0f) {
                f32 s = diff / 0.9f;
                if (s > mx) {
                    s = mx;
                }
                if (s > 1.0f) {
                    hero.speed_scale = s;
                }
            }
        }
        switch (tb->kind) {
        case HERO_COMBO_KIND_FRAME_GATED_FORWARD_ADVANCE:
            if (hero.anim_frame < 18.0f) {
                hero.target_speed = frame_time * 4.4f * hero.speed_scale;
            }
            break;
        case HERO_COMBO_KIND_STAGED_FORWARD_DECELERATION:
            if (hero.anim_changing != 0 || hero.anim_frame < 9.0f) {
                if (hero.anim_frame > 8.0f) {
                    hero.target_speed = frame_time * 3.0f * hero.speed_scale;
                } else if (hero.anim_frame > 7.0f) {
                    hero.target_speed = frame_time * 4.5f * hero.speed_scale;
                } else {
                    hero.target_speed = frame_time * 5.7f * hero.speed_scale;
                }
            }
            break;
        case HERO_COMBO_KIND_CONDITIONALLY_FRAME_GATED_FORWARD_ADVANCE:
            if (hero.anim_changing != 0 || hero.anim_frame < 12.0f) {
                hero.target_speed = frame_time * 3.7f * hero.speed_scale;
            }
            break;
        }
        approach_value(&hero.anim_speed, 1.0f, 0.2f);
        if (hero.heading_locked != 0 || MV(hero.melee_target) != NULL) {
            if (tb->kind == 1 &&
                fast_subtract_rotations(hero.attack_target_yaw, hero.motion.rot.f[2]) > 0.0f) {
                hero.anim_speed = 1.0f / (fast_difference_between_rotations(hero.attack_target_yaw,
                                                                            hero.motion.rot.f[2]) +
                                          1.0f);
            }
            hero.motion.move_yaw = hero.attack_target_yaw;
            turn_to(0, frame_scale_sq * 0.05f, frame_scale_sq * 0.2f, frame_time * 15.009831f);
        } else {
            s32 ok = 1;
            if (tb->kind == 1 && hero.anim_changing == 0 && hero.anim_frame > 7.0f) {
                ok = 0;
            }
            if (ok) {
                update_hero_target_heading(5.0f, 0.7853982f, 0.7853982f);
            }
        }
        speed_step(frame_time_sq * 37.0f, frame_time_sq * 28.0f);
        if (hero.heading_locked != 0) {
            set_planar_vel(hero.attack_target_yaw);
        } else {
            set_planar_vel(99999.0f);
        }
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        stop_hero_motion_without_forward_support(3.7f, 0.0f);
        break;
    }
    case HERO_STATE_WAIT_FOR_ANIMATION_THEN_IDLE:
    case HERO_STATE_WALL_BONK:
        approach_value(&hero.ground_speed, 0.0f, frame_time_sq * 24.0f);
        hero.motion.velocity.f[0] = fast_cos(hero.pushback_yaw) * hero.ground_speed;
        hero.motion.velocity.f[1] = fast_sin(hero.pushback_yaw) * hero.ground_speed;
        if (hero.ground_distance > 0.0f) {
            hero.motion.velocity.f[2] = hero.motion.resolved_velocity.f[2] - frame_time_sq * 25.0f;
        } else {
            hero.motion.velocity.f[2] = 0.0f;
        }
        hero.motion.move_yaw = fast_add_rotations(hero.pushback_yaw, 3.1415927f);
        turn_to(0, frame_scale_sq * 0.05f, frame_scale_sq * 0.2f, frame_time * 15.009831f);
        break;
    case HERO_STATE_HELD_ITEM_ATTACK: {
        s32 t1;
        s32 t2;
        if (hero.heading_locked != 0 || MV(hero.melee_target) != NULL) {
            hero.motion.move_yaw = hero.attack_target_yaw;
            turn_to(0, frame_scale_sq * 0.05f, frame_scale_sq * 0.2f, frame_time * 15.009831f);
        } else if (hero.state_timer < scale_game_frames(4)) {
            update_hero_target_heading(11.0f, 0.87266463f, -1.0f);
        }
        t1 = scale_game_frames(10);
        hero.target_speed = 0.0f;
        t2 = t1 + scale_game_frames(8);
        if (t1 < hero.state_timer && hero.state_timer < t2) {
            hero.target_speed = frame_time * 18.0f;
        }
        if (hero.state_timer == scale_game_frames(8)) {
            initialize_moby_transform_history(MV(hero.moby), &hero.trail.pos[0]);
            create_moby_transform_history_entry(&hero.trail.pos[0], 0x30, 2);
            create_moby_transform_history_entry(&hero.trail.pos[0], 0x17, 4);
            create_moby_transform_history_entry(&hero.trail.pos[0], 0xC, 6);
        }
        update_moby_transform_history(&hero.trail.pos[0], t2 < hero.state_timer ? 5 : 0);
        if (t1 < hero.state_timer) {
            speed_step(frame_time_sq * 190.0f, frame_time_sq * 90.0f);
        }
        if (hero.heading_locked != 0) {
            set_planar_vel(hero.attack_target_yaw);
        } else {
            set_planar_vel(99999.0f);
        }
        if (hero.state.current == 0x20 && t1 < hero.state_timer &&
            hero.state_timer < t2 + scale_game_frames(4)) {
            struct MobyView *o = MV(hero.items[0].moby);
            if (o != NULL) {
                f32 a;
                motion_offset_or_position.f[0] = fast_cos(hero.motion.rot.f[2]) * 2.0f;
                motion_offset_or_position.f[1] = fast_sin(hero.motion.rot.f[2]) * 2.0f;
                motion_offset_or_position.f[2] = 1.3f;
                make_hit_descriptor(&spatial_scratch_or_hit_descriptor,
                                    MV(hero.moby),
                                    0x30000,
                                    3.0f,
                                    &motion_offset_or_position);
                ((u8 *)&v10)[9] = 3;
                spatial_scratch_or_hit_descriptor.f[3] = 5627.925f;
                ((u8 *)&v10)[8] = 0;
                a = hero.motion.rot.f[2];
                ((s16 *)&v10)[5] = o->hA6;
                yaw_matrix_or_spatial_vectors[0].f[0] = fast_cos(a) * 0.8f;
                yaw_matrix_or_spatial_vectors[0].f[1] = fast_sin(a) * 0.8f;
                yaw_matrix_or_spatial_vectors[0].f[2] = 0.55f;
                fast_vec_add(&yaw_matrix_or_spatial_vectors[0],
                             &yaw_matrix_or_spatial_vectors[0],
                             &hero.motion.pos);
                emit_filtered_entry_sparkles(
                    o,
                    collect_mobies_in_sphere(0.8f,
                                             &yaw_matrix_or_spatial_vectors[0],
                                             0,
                                             o,
                                             &spatial_scratch_or_hit_descriptor));
                a = fast_add_rotations(hero.motion.rot.f[2], -0.87266463f);
                yaw_matrix_or_spatial_vectors[0].f[0] = fast_cos(a) * 0.8f;
                yaw_matrix_or_spatial_vectors[0].f[1] = fast_sin(a) * 0.8f;
                yaw_matrix_or_spatial_vectors[0].f[2] = 0.55f;
                fast_vec_add(&yaw_matrix_or_spatial_vectors[0],
                             &yaw_matrix_or_spatial_vectors[0],
                             &hero.motion.pos);
                emit_filtered_entry_sparkles(
                    o,
                    collect_mobies_in_sphere(0.8f,
                                             &yaw_matrix_or_spatial_vectors[0],
                                             0,
                                             MV(hero.moby),
                                             &spatial_scratch_or_hit_descriptor));
                a = fast_add_rotations(hero.motion.rot.f[2], 0.87266463f);
                yaw_matrix_or_spatial_vectors[0].f[0] = fast_cos(a) * 0.8f;
                yaw_matrix_or_spatial_vectors[0].f[1] = fast_sin(a) * 0.8f;
                yaw_matrix_or_spatial_vectors[0].f[2] = 0.55f;
                fast_vec_add(&yaw_matrix_or_spatial_vectors[0],
                             &yaw_matrix_or_spatial_vectors[0],
                             &hero.motion.pos);
                emit_filtered_entry_sparkles(
                    o,
                    collect_mobies_in_sphere(0.8f,
                                             &yaw_matrix_or_spatial_vectors[0],
                                             0,
                                             o,
                                             &spatial_scratch_or_hit_descriptor));
            }
        }
        if (scale_game_frames(10) < hero.state_timer) {
            stop_hero_motion_without_forward_support(3.7f, 0.0f);
        }
        clip_hero_horizontal_velocity(1);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity, frame_time_sq * 24.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    }
    case HERO_STATE_ANIMATION_RECOVERY_WITH_JUMP_CANCEL:
        hero.target_speed = 0.0f;
        if (hero.heading_locked != 0) {
            hero.motion.move_yaw = hero.attack_target_yaw;
            turn_to(0, frame_scale_sq * 0.04f, frame_scale_sq * 0.2f, frame_time * 15.009831f);
        } else {
            update_hero_target_heading(11.0f, 0.87266463f, -1.0f);
        }
        speed_step(frame_time_sq * 30.0f, frame_time_sq * 35.0f);
        set_planar_vel(99999.0f);
        if (hero.air_frames.s != 0) {
            hero.motion.velocity.f[2] = hero.motion.displacement.f[2] - frame_time_sq * 25.0f;
            correct_hero_airborne_wall_velocity();
        } else {
            hero.motion.velocity.f[2] = hero.motion.velocity.f[2] - frame_time_sq * 54.0f;
        }
        break;
    case HERO_STATE_RUN:
    case HERO_STATE_SHALLOW_WATER_RUN: {
        s32 slide = 0;
        f32 k;
        if (hero.base_condition != 0 && hero.water_depth < 0.25f) {
            slide = 1;
        }
        if (hero.wading != 0 || slide) {
            if (hero.motion.flat_speed > frame_time) {
                fast_sin_cf(0, 2);
            }
            if (hero.motion.flat_speed > frame_time * 0.5f) {
                emit_hero_periodic_surface_particles(2, 4);
            }
            emit_hero_randomized_motion_effects(5, 2);
        }
        if (hero.motion.flat_speed < frame_time * 0.3f && hero.stick_magnitude > 0.85f) {
            hero.unk3B8 = 1;
        }
        if (absolute_float(hero.motion.yaw_error) < 0.05235988f || hero.item_anim_active != 0 ||
            hero.state.step == 1) {
            hero.unk3B8 = 0;
        }
        if (hero.unk3BE != 0 &&
            (absolute_float(hero.motion.yaw_error) < 0.06981317f || hero.item_anim_active != 0)) {
            hero.unk3BE = 0;
        }
        hero_speed_from_table();
        if (hero.unk930 > 0.0f) {
            Vec4 *move_impulse_ptr = &hero.move_impulse;
            build_scaled_hero_move_impulse(0.7f, 0.0f);
            spatial_scratch_or_hit_descriptor.q = hero.move_impulse.q;
            hero.target_speed += forward_component(&spatial_scratch_or_hit_descriptor);
            if (hero.target_speed < frame_time * 0.7f) {
                hero.target_speed = frame_time * 0.7f;
            }
            clear_u64_value(move_impulse_ptr);
        }
        k = 1.0f;
        if (hero.state.step == 1 && hero.unk3BE == 0) {
            turn_to(0, frame_scale_sq * 0.008f, frame_scale_sq * 0.15f, frame_time * 9.948377f);
        } else {
            f32 movement_scale = hero.stick_magnitude;
            f32 r = hero.motion.yaw_error;
            if (hero.item_anim_active != 0) {
                k = 1.0f - r * 0.55f;
                if (k < 0.0f) {
                    k = 0.0f;
                }
            } else if (movement_scale < 0.8f && r > 0.5235988f) {
                k = 0.0f;
            }
            if (movement_scale > D_L03_0017BEB8_f[3]) {
                turn_hero_toward_move_direction_with_stick_scale();
            } else {
                f32 f = hero.stick_magnitude + 0.35f;
                turn_to(0,
                        frame_scale_sq * 0.005f * f,
                        frame_scale_sq * 0.1f,
                        frame_time * 4.712389f * f);
            }
        }
        adjust_hero_pose_targets_from_motion();
        update_hero_lean_and_sway_parameters();
        {
            f32 a = frame_time_sq * 7.5f * k;
            f32 b = frame_time_sq * 8.5f;
            if (hero.unk3BE != 0) {
                f32 m = 2.6179938f - hero.motion.yaw_error;
                if (m > 1.0f) {
                    m = 1.0f;
                }
                if (m < 0.2f) {
                    m = 0.2f;
                }
                hero.target_speed *= m;
                b = frame_time_sq * 17.0f;
            }
            speed_step(a, b);
        }
        if (hero.unk3B8 != 0 && hero.unk308 == 0) {
            set_planar_vel(hero.motion.move_yaw);
        } else {
            set_planar_vel(99999.0f);
        }
        clip_hero_horizontal_velocity(1);
        if (hero.unk1F4 != 0) {
            stop_hero_motion_without_forward_support(5.0f, 0.2f);
        }
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    }
    case HERO_STATE_VOICE_SLOT_CLEANUP_IDLE_EXIT: {
        f32 f;
        f32 old;
        f32 d;
        if (current_level_index == 0xF) {
            release_voice_slot_cf(2, MV(hero.moby), 0x1D);
        } else {
            release_voice_slot_cf(2, MV(hero.moby), 6);
        }
        if (current_level_index != 0xF) {
            emit_hero_randomized_motion_effects(4, 2);
            spatial_scratch_or_hit_descriptor.f[0] =
                hero.motion.pos.f[0] + random_float_between(-0.25f, 0.25f);
            spatial_scratch_or_hit_descriptor.f[1] =
                hero.motion.pos.f[1] + random_float_between(-0.25f, 0.25f);
            spatial_scratch_or_hit_descriptor.f[2] =
                hero.motion.pos.f[2] + random_float_between(-0.1f, 0.4f);
            fast_vec_scale(&v10, &hero.motion.displacement, 0.9f);
            v10.f[0] += random_float_between(frame_time * -0.7f, frame_time * 0.7f);
            v10.f[1] += random_float_between(frame_time * -0.7f, frame_time * 0.7f);
            v10.f[2] += random_float_between(frame_time, frame_time * 2.0f);
            spawn_random_alpha_rotated_particle(
                &spatial_scratch_or_hit_descriptor, &v10, random_float_between(37800.0f, 75600.0f));
        }
        f = fast_vec_length(&hero.motion.platform_delta);
        if (f > frame_time * 0.5f) {
            hero.motion.move_yaw =
                fast_arc_tan(hero.motion.platform_delta.f[0], hero.motion.platform_delta.f[1]);
        } else {
            hero.motion.move_yaw = hero.motion.rot.f[2];
        }
        old = hero.motion.rot.f[2];
        turn_to(0, f * 0.125f * frame_scale_sq, frame_scale_sq * 0.2f, frame_time * 4.1887903f);
        d = fast_difference_between_rotations(hero.motion.rot.f[2], old);
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 5.0f);
        hero.motion.velocity.f[2] = hero.motion.displacement.f[2] - frame_time_sq * 50.0f;
        if (hero.motion.velocity.f[2] > frame_time_sq * -40.0f) {
            hero.motion.velocity.f[2] = frame_time_sq * -40.0f;
        }
        /* Fakematch: cc1 drops the self-store, but it puts frame_time in $f2 as in retail. */
        hero.target_speed = hero.target_speed;
        if (d > frame_time * 1.3962634f) {
            d = frame_time * 1.3962634f;
        }
        d *= 9.0f;
        spring_angle(&hero.motion.rot.f[0],
                     d * 0.5f,
                     &hero.roll_spring_velocity,
                     0,
                     0.007f,
                     0.17f,
                     frame_time * 1.2217305f);
        if (scale_game_frames(100) < hero.state_timer) {
            spring_angle(&hero.motion.rot.f[1],
                         -hero.rail_ride_pitch,
                         &hero.rail_ride_pitch_vel,
                         0,
                         frame_scale_sq * 0.035f,
                         frame_scale_sq * 0.3f,
                         frame_time * 1.9198622f);
        }
        FUN_L03_00203070(0.009f, 0.22f);
        FUN_L03_00203088(0.04f, 0.2f);
        FUN_L03_002030a0(0.02f, 0.2f);
        AN.f118 = d * 2.1f;
        AN.f110 = d * 3.5f;
        break;
    }
    case HERO_STATE_FOLLOW_PATH: {
        s32 idx;
        s32 n;
        compute_hero_fixed_offset_world_position(&spatial_scratch_or_hit_descriptor);
        if (MV(hero.moby)->b53 == 0x66) {
            release_voice_slot_cf(0, MV(hero.moby), 0);
            if (D_L03_0015F5CC & 1) {
                f32 a;
                f32 r;
                qcopy(&v10, &spatial_scratch_or_hit_descriptor);
                fast_vec_scale(
                    &spatial_scratch_or_hit_descriptor_tail, &hero.motion.displacement, 0.4f);
                a = fast_add_rotations(hero.grab_rail_yaw, randf_sym(0.0f, 0.5235988f));
                r = random_float_between(frame_time * 0.0f, frame_time * 4.0f);
                spatial_scratch_or_hit_descriptor_tail.f[0] += fast_cos(a) * r;
                spatial_scratch_or_hit_descriptor_tail.f[1] += fast_sin(a) * r;
                spatial_scratch_or_hit_descriptor_tail.f[2] =
                    random_float_between(frame_time * 3.0f, frame_time * 5.0f);
                spatial_scratch_or_hit_descriptor_tail.f[3] = -(frame_time_sq * 35.0f);
                spawn_color_decaying_effect_particle(
                    &v10, &spatial_scratch_or_hit_descriptor_tail, 0);
            }
        }
        FUN_L00_0025df68(hero.follow_path_spline,
                         &spatial_scratch_or_hit_descriptor,
                         &hero.grab_rail_point,
                         &hero.follow_path_segment,
                         &hero.rail_candidate_frac,
                         hero.rail_candidate_kind,
                         999.0f,
                         8.0f,
                         2.5f);
        n = hero.follow_path_end_frames;
        if (n != 0) {
            qcopy(&hero.motion.velocity, &hero.motion.resolved_velocity);
            hero.follow_path_end_frames = n + 1;
            hero.motion.velocity.f[2] = hero.motion.velocity.f[2] - frame_time_sq * 24.0f;
            break;
        }
        sample_path(hero.follow_path_spline,
                    &v10,
                    &hero.follow_path_segment,
                    &hero.rail_candidate_frac,
                    hero.follow_path_speed,
                    hero.rail_candidate_kind);
        idx = offset_path_index(
            hero.follow_path_spline, hero.follow_path_segment, 1, hero.rail_candidate_kind);
        fast_vec_sub(&spatial_scratch_or_hit_descriptor_tail,
                     (Vec4 *)hero.follow_path_spline + (idx + 1),
                     &v10);
        if (fast_vec_length(&spatial_scratch_or_hit_descriptor_tail) < 0.001f) {
            if (hero.follow_path_segment == idx) {
                hero.follow_path_end_frames = scale_game_frames(1);
                break;
            }
        } else {
            hero.grab_rail_yaw = fast_arc_tan(spatial_scratch_or_hit_descriptor_tail.f[0],
                                              spatial_scratch_or_hit_descriptor_tail.f[1]);
            hero.follow_path_pitch =
                fast_arc_tan(vector_length_xy(&spatial_scratch_or_hit_descriptor_tail),
                             spatial_scratch_or_hit_descriptor_tail.f[2]);
        }
        hero.motion.move_yaw = hero.grab_rail_yaw;
        turn_to(0, frame_scale_sq * 0.018f, frame_scale_sq * 0.2f, frame_time * 6.4577184f);
        approach_value(&hero.follow_path_speed, frame_time * 14.0f, frame_time_sq * 9.0f);
        fast_vec_sub(&hero.motion.velocity, &v10, &hero.grab_rail_point);
        fast_vec_sub(&spatial_scratch_or_hit_descriptor_tail,
                     &hero.grab_rail_point,
                     &spatial_scratch_or_hit_descriptor);
        {
            f32 fl;
            f32 len;
            fl = -fast_vec_length(&spatial_scratch_or_hit_descriptor_tail);
            vendor_spring(&fl,
                          &hero.unk618.f,
                          0.0f,
                          frame_scale_sq * 0.05f,
                          frame_scale_sq * 0.3f,
                          frame_time * 10.0f);
            len = fast_vec_length(&spatial_scratch_or_hit_descriptor_tail);
            if (len < hero.unk618.f) {
                hero.unk618.f = len;
            }
            if (hero.unk618.f < len) {
                fast_vec_normalize(&spatial_scratch_or_hit_descriptor_tail,
                                   &spatial_scratch_or_hit_descriptor_tail,
                                   hero.unk618.f);
            }
            fast_vec_add(
                &hero.motion.pos, &hero.motion.pos, &spatial_scratch_or_hit_descriptor_tail);
        }
        break;
    }
    case HERO_STATE_LEDGE_HANG_ENTRY:
    case HERO_STATE_LEDGE_HANG:
        hero.motion.move_yaw = fast_add_rotations(hero.ledge_normal_yaw, 3.1415927f);
        turn_to(0, frame_scale_sq * 0.04f, frame_scale_sq * 0.2f, frame_time * 6.2831855f);
        step_angle_toward_target(&hero.motion.rot.f[1], 0, 0.0f, frame_time * 4.712389f);
        fast_vec_sub(&hero.motion.velocity, &hero.ledge_hang_pos, &hero.motion.pos);
        FUN_L00_0025f730(&hero.motion.velocity, frame_time * 4.0f);
        approach_value(&hero.unk4F0, hero.motion.pos.f[2], frame_time * 3.0f);
        break;
    case HERO_STATE_LEDGE_SHIMMY_POSITIVE_YAW:
    case HERO_STATE_LEDGE_SHIMMY_NEGATIVE_YAW: {
        f32 s;
        f32 a;
        f32 b;
        f32 fl;
        hero.motion.velocity.f[0] = 0.0f;
        hero.motion.velocity.f[1] = 0.0f;
        hero.motion.velocity.f[2] = 0.0f;
        qcopy(&spatial_scratch_or_hit_descriptor_tail, &hero.motion.pos);
        if (hero.state.current == 0x1A) {
            yaw_matrix_or_spatial_vectors[0].f[0] =
                fast_cos(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
            yaw_matrix_or_spatial_vectors[0].f[1] =
                fast_sin(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
            yaw_matrix_or_spatial_vectors[0].f[2] = 0.0f;
        } else {
            yaw_matrix_or_spatial_vectors[0].f[0] =
                fast_cos(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
            yaw_matrix_or_spatial_vectors[0].f[1] =
                fast_sin(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
            yaw_matrix_or_spatial_vectors[0].f[2] = 0.0f;
        }
        fast_vec_add(&motion_offset_or_position,
                     &spatial_scratch_or_hit_descriptor_tail,
                     &yaw_matrix_or_spatial_vectors[0]);
        update_hero_ledge_probe();
        if (hero.ledge_found == 0) {
            break;
        }
        qcopy(&spatial_scratch_or_hit_descriptor, &hero.ledge_hang_pos);
        a = fast_add_rotations(hero.ledge_normal_yaw, 3.1415927f);
        qcopy(&hero.motion.pos, &motion_offset_or_position);
        update_hero_ledge_probe();
        qcopy(&hero.motion.pos, &spatial_scratch_or_hit_descriptor_tail);
        if (hero.ledge_found == 0) {
            break;
        }
        qcopy(&v10, &hero.ledge_hang_pos);
        b = fast_add_rotations(hero.ledge_normal_yaw, 3.1415927f);
        if (fast_difference_between_rotations(a, b) > 0.5235988f) {
            break;
        }
        a = fast_add_rotations(fast_subtract_rotations(a, b) * 0.5f, a);
        fast_vec_add(&yaw_matrix_or_spatial_vectors[1], &spatial_scratch_or_hit_descriptor, &v10);
        fast_vec_scale(&yaw_matrix_or_spatial_vectors[1], &yaw_matrix_or_spatial_vectors[1], 0.5f);
        hero.motion.move_yaw = a;
        turn_to(0, frame_scale_sq * 0.04f, frame_scale_sq * 0.2f, frame_time * 6.2831855f);
        fl = hero.motion.flat_speed;
        approach_value(&fl, hero.target_speed * frame_time, frame_time_sq * 5.0f);
        fast_vec_sub(&hero.motion.velocity, &yaw_matrix_or_spatial_vectors[1], &hero.motion.pos);
        set_vector_length_xy(
            &hero.motion.velocity,
            &hero.motion.velocity,
            l03_ledge_shimmy_planar_speed_by_frame[truncate_float_to_s32(hero.anim_frame)] * 0.5f);
        break;
    }
    case HERO_STATE_SLIP:
        hero.motion.move_yaw = hero.downhill_yaw;
        hero.target_speed = frame_time * 5.5f;
        air_accel(frame_time_sq * 25.0f);
        FUN_L00_002334d0(
            &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 18.0f);
        break;
    case HERO_STATE_FALL:
    case HERO_STATE_ITEM_RELEASE_FALL:
        if (hero.state.current == 0x2D) {
            f32 t = 0.0f;
            if (hero.ground_distance > 4.0f) {
                t = 0.43633232f;
            }
            qcopy(&spatial_scratch_or_hit_descriptor, &hero.motion.rot);
            spring_angle(&spatial_scratch_or_hit_descriptor.f[1],
                         t,
                         &hero.motion.rot_spring_vel.f[1],
                         0,
                         frame_scale_sq * 0.02f,
                         frame_scale_sq * 0.3f,
                         frame_time * 2.6179938f);
            spring_angle(&spatial_scratch_or_hit_descriptor.f[0],
                         0.0f,
                         &hero.motion.rot_spring_vel.f[0],
                         0,
                         frame_scale_sq * 0.02f,
                         frame_scale_sq * 0.3f,
                         frame_time * 2.6179938f);
            rotate_hero_about_fixed_local_pivot(
                hero.motion.rot_spring_vel.f[0], hero.motion.rot_spring_vel.f[1], 0.0f);
            if (hero.ground_distance > 1.0f) {
                hero.state.step = 1;
            }
        }
        hero_speed_from_table();
        if (hero.move_lock_timer == 0) {
            hero_update_hop_velocity();
        }
        adjust_hero_pose_targets_from_motion();
        build_scaled_hero_move_impulse(0.7f, frame_time * 0.0f);
        FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, hero.fall_state_gravity);
        if (gravity_z(&hero.motion.velocity) < -(frame_time * 50.0f)) {
            gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, -(frame_time * 50.0f));
        }
        break;
    case HERO_STATE_FATAL_FALL: {
        f32 len;
        hero.item_use_locked = 1;
        approach_value(
            &hero.fatal_fall_roll_rate, hero.pivot_rot_x_target, frame_time_sq * 8.726646f);
        approach_value(
            &hero.fixed_pivot_rot_y, hero.fatal_fall_tumble_target_y, frame_time_sq * 8.726646f);
        rotate_hero_about_fixed_local_pivot(
            hero.fatal_fall_roll_rate, hero.fixed_pivot_rot_y, 0.0f);
        len = vector_length_xy(&hero.motion.velocity);
        FUN_L00_001ff600(
            frame_scale * -0.100000024f * len + len, &hero.motion.velocity, &hero.motion.velocity);
        FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, hero.fall_state_gravity);
        if (gravity_z(&hero.motion.velocity) < -(frame_time * 50.0f)) {
            gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, -(frame_time * 50.0f));
        }
        break;
    }
    case HERO_STATE_DELAYED_JUMP_OR_R1_REPEAT_EXIT_WAIT: {
        s32 big;
        f32 sp;
        f32 m;
        release_voice_slot_cf(4, MV(hero.moby), 0x12);
        big = 1;
        if (!(hero.ground_distance > 0.7f)) {
            big = 0;
        }
        sp = frame_time * 4.0f;
        m = frame_time * 5.75f;
        if (big) {
            m = sp;
        }
        stick_target(hero.stick_magnitude * m, 0);
        if (hero.stick_magnitude > 0.2f && !(PD.u1A0.i1A0 & 3)) {
            turn_to(0,
                    frame_scale_sq * 0.027f,
                    frame_scale_sq * 0.3f,
                    frame_time * 5.934119f * hero.stick_magnitude);
        }
        if (PD.u1A0.i1A0 & 3) {
            f32 o = hero.motion.move_yaw;
            hero.motion.move_yaw = *(f32 *)D_L03_00166ED8;
            turn_to(0, frame_scale_sq * 0.017f, frame_scale_sq * 0.3f, frame_time * 4.712389f);
            hero.motion.move_yaw = o;
        }
        {
            f32 g = frame_time_sq * 7.0f;
            if (hero.stick_magnitude < 0.2f) {
                g = frame_time_sq * 3.5f;
            }
            air_accel(g);
        }
        adjust_hero_pose_targets_from_motion();
        clip_hero_horizontal_velocity(1);
        if (big) {
            gravity_flatten(&hero.motion.velocity, &hero.motion.velocity);
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, sp * 0.72f);
            hero.ground_spring_velocity = hero.motion.velocity.f[2];
        } else {
            vendor_spring(&hero.motion.pos.f[2],
                          &hero.ground_spring_velocity,
                          hero.ground_z.f + 0.17f,
                          frame_scale_sq * 0.02f,
                          frame_scale_sq * 0.35f,
                          frame_time * 2.5f);
        }
        break;
    }
    case HERO_STATE_LAND: {
        f32 sp;
        if (hero_slot_item_id(3) == 2) {
            if (hero.voice_slot4 != -1) {
                struct VoiceSlot *e =
                    (struct VoiceSlot *)(((u8 *)&voice_pool) + hero.voice_slot4 * 0x70);
                if (e->pm88 == MV(hero.moby) && e->b74 != 0) {
                    release_voice_slot(hero.voice_slot4);
                }
            }
            hero.voice_slot4 = -1;
            release_voice_slot_cf(3, MV(hero.moby), 2);
        } else {
            if (hero.unk2224 != -1) {
                struct VoiceSlot *e =
                    (struct VoiceSlot *)(((u8 *)&voice_pool) + hero.unk2224 * 0x70);
                if (e->pm88 == MV(hero.moby) && e->b74 != 0) {
                    release_voice_slot(hero.unk2224);
                }
            }
            hero.unk2224 = -1;
            release_voice_slot_cf(4, MV(hero.moby), 0x12);
        }
        sp = frame_time * 3.0f;
        if (hero.items[3].item_id == 3) {
            sp = frame_time * 5.0f;
        }
        stick_target(sp, 0);
        turn_to(0, frame_scale_sq * 0.025f, frame_scale_sq * 0.3f, frame_time * 12.566371f);
        speed_step(frame_time_sq * 15.0f, frame_time_sq * 7.0f);
        set_planar_vel(99999.0f);
        adjust_hero_pose_targets_from_motion();
        gravity_flatten(&hero.motion.velocity, &hero.motion.velocity);
        FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, sp * 0.72f);
        if (hero.motion.pos.f[2] < hero.smoothed_ground_z) {
            approach_value(&hero.smoothed_ground_z, hero.motion.pos.f[2], frame_time * 4.0f);
        }
        probe_hero_airborne_wall_contact();
        break;
    }
    case HERO_STATE_JUMP:
    case HERO_STATE_RUN_JUMP:
    case HERO_STATE_PACK_JUMP:
    case HERO_STATE_HIGH_JUMP:
    case HERO_STATE_STORED_HEADING_JUMP:
    case HERO_STATE_JUMP_WITH_TRANSFORM_TRAIL:
    case HERO_STATE_DOUBLE_JUMP:
    case HERO_STATE_JUMP_WITH_VELOCITY_KEY_PROFILE:
    case HERO_STATE_PACK_LONG_JUMP:
    case HERO_STATE_REPEATABLE_JUMP:
    case HERO_STATE_WATER_SURFACE_JUMP:
    case HERO_STATE_LEDGE_JUMP:
    case HERO_STATE_JUMP_WITH_HEALTH_DAMAGE:
    case HERO_STATE_VEHICLE_MODE_1_JUMP:
    case HERO_STATE_ALT_LEDGE_JUMP:
    case HERO_STATE_VEHICLE_MODE_2_JUMP:
    case HERO_STATE_JUMP_FROM_RESPAWN_WAIT: {
        s32 state_timer_or_trail_mode;
        s32 position_capture_frame;
        if (hero.state.current == 0x1C && hero.pack_disabled == 0 && hero.items[3].item_id == 3) {
            if (test_hero_animation_frame_window(10.0f) != 0) {
                allocate_hero_voice_for_entry(4, 0);
            }
        }
        if (hero.unk4A8 != 0 && hero.on_ice_ground != 0) {
            hero.target_speed = hero.ground_speed;
        } else {
            if (hero.rail_land_frames != 0) {
                stick_target(frame_time * 5.7f, 0);
            }
            stick_target(hero.air_stick_speed, 0);
        }
        if (hero.state.current == 0x12) {
            if (hero.state_timer == scale_game_frames(20)) {
                emit_hero_surface_effect_burst(3, 0x10, 1);
                if (D_L03_001612D0 != 0) {
                    pool_splash(D_L03_001612D0,
                                D_L03_001612D8,
                                0,
                                hero.motion.pos.f[0],
                                hero.motion.pos.f[1],
                                0.4f,
                                0.35f);
                }
            }
            if (hero.anim_frame >= 4.0f && hero.anim_frame <= 7.0f) {
                emit_hero_randomized_motion_effects(7, 0);
            }
        }
        state_timer_or_trail_mode = hero.state_timer;
        position_capture_frame = hero.takeoff_frame;
        if (state_timer_or_trail_mode == position_capture_frame) {
            qcopy(&hero.takeoff_pos, &hero.motion.pos);
        }
        if (hero.rail_land_frames != 0 ||
            (state_timer_or_trail_mode < position_capture_frame &&
             hero.state.current != HERO_STATE_HIGH_JUMP &&
             hero.state.current != HERO_STATE_PACK_LONG_JUMP && hero.state.current != 0xC &&
             hero.state.current != 0x11 && hero.state.current != 0x1C &&
             hero.state.current != 0x4C)) {
            f32 sv;
            turn_to(0, frame_scale_sq * 0.015f, frame_scale_sq * 0.2f, frame_time * 15.009831f);
            sv = hero.ground_speed;
            if (hero.rail_land_frames >= 2) {
                hero.ground_speed = sv * 0.4f;
            }
            set_planar_vel(99999.0f);
            hero.ground_speed = sv;
            if (hero.rail_land_frames != 1) {
                speed_step(hero.air_speed_step_up, hero.jump_decel);
            }
            if (hero.unk308 != 0) {
                hero.ground_speed = 0.0f;
            }
            if (hero.rail_land_frames != 0) {
                if (D_0013CB14[0] != 0 && hero.on_flagged_ground_moby != 0) {
                    hero.ground_speed = 0.0f;
                }
                if (hero.rail_land_frames != 0) {
                    reduce_and_apply_hero_planar_velocity_addend(hero.jump_decel);
                }
            }
            build_scaled_hero_move_impulse(0.5f, frame_time * 2.0f);
            if (hero.state.current == HERO_STATE_RUN_JUMP &&
                hero.state_timer < hero.takeoff_frame + scale_game_frames(4)) {
                add_hero_forward_offset(&hero.motion.velocity,
                                        &hero.motion.velocity,
                                        frame_time_sq * 16.0f * (f32)hero.state_timer);
            }
            if (hero.state.current == HERO_STATE_PACK_JUMP &&
                hero.state_timer < scale_game_frames(34)) {
                add_hero_forward_offset(&hero.motion.velocity,
                                        &hero.motion.velocity,
                                        frame_time_sq * 53.0f * (f32)hero.state_timer);
            }
            if (hero.state_timer < scale_game_frames(10) && hero.stick_magnitude > 0.2f) {
                qcopy(&spatial_scratch_or_hit_descriptor, &hero.motion.pos);
                FUN_L00_00233810(
                    &spatial_scratch_or_hit_descriptor, &spatial_scratch_or_hit_descriptor, 0.5f);
                qcopy(&v10, &spatial_scratch_or_hit_descriptor);
                v10.f[0] += fast_cos(hero.motion.rot.f[2]) * 0.85f;
                v10.f[1] += fast_sin(hero.motion.rot.f[2]) * 0.85f;
                if (collision_line(&spatial_scratch_or_hit_descriptor, &v10, 4, MV(hero.moby), 0) !=
                    0) {
                    hero.jump_blocked_ahead = 1;
                }
            }
        } else {
            hero_update_hop_velocity();
            if (hero.state.current == 0x12) {
                f32 l = vector_length_xy(&hero.motion.velocity);
                if (l > frame_time * 3.7f) {
                    l = frame_time * 3.7f;
                }
                set_vector_length_xy(&hero.motion.velocity, &hero.motion.velocity, l);
            }
        }
        if (hero.ground_z.f > hero.smoothed_ground_z) {
            hero.smoothed_ground_z = hero.smoothed_ground_z + frame_time * 6.0f;
            if (hero.smoothed_ground_z > hero.ground_z.f) {
                hero.smoothed_ground_z = hero.ground_z.f;
            }
        }
        if (hero.motion.pos.f[2] < hero.smoothed_ground_z) {
            hero.smoothed_ground_z = hero.motion.pos.f[2];
        }
        if (hero.state_timer > hero.takeoff_frame && gravity_z(&hero.motion.velocity) < 0.001f &&
            (hero.velocity_keys_active == 0 || hero.state_timer > hero.velocity_keys_end_frame)) {
            hero.past_jump_apex = 1;
            if (hero.fall_gravity != 0.0f) {
                hero.jump_gravity = hero.fall_gravity;
            }
        }
        if (hero.vehicle_mode == 0) {
            adjust_hero_pose_targets_from_motion();
        }
        if (hero.state.current == HERO_STATE_PACK_LONG_JUMP || hero.state.current == 0xD) {
            state_timer_or_trail_mode = (scale_game_frames(12) < hero.state_timer) * 2;
            update_moby_transform_history(&hero.trail.pos[0], state_timer_or_trail_mode);
        }
        if (hero.state.current == HERO_STATE_PACK_LONG_JUMP &&
            scale_game_frames(8) < hero.state_timer && hero.anim_frame < 28.0f) {
            s32 n;
            transform_hero_local_offset_to_world(
                &spatial_scratch_or_hit_descriptor, 0.8f, 0.0f, 0.3f);
            n = collect_mobies_in_sphere(
                0.9f, &spatial_scratch_or_hit_descriptor, 0, MV(hero.moby), NULL);
            if (n != 0) {
                s32 i;
                fast_vec_scale(&v10, &hero.motion.displacement, 7.0f);
                for (i = 0; i < n; i++) {
                    if (is_moby_in_class_range_500_to_540(D_L03_00178000[i]) != 0) {
                        spawn_nearby_effect(D_L03_00178000[i],
                                            MV(hero.moby),
                                            0x10000,
                                            &hero.motion.pos,
                                            &v10,
                                            1.0f);
                    }
                }
            }
        }
        hero_integrate_velocity();
        probe_hero_airborne_wall_contact();
        break;
    }
    case HERO_STATE_HURT: {
        f32 l = vector_length_xy(&hero.motion.velocity);
        f32 d = frame_time_sq * 4.0f;
        if (hero.air_frames.s == 0 && scale_game_frames(10) < hero.state_timer) {
            d = frame_time_sq * 10.0f;
        }
        l -= d;
        if (l < 0.0f) {
            l = 0.0f;
        }
        if (hero.on_magnet_ground != 0) {
            stop_hero_motion_without_forward_support(3.7f, 0.0f);
        }
        set_vector_length_xy(&hero.motion.velocity, &hero.motion.velocity, l);
        FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, 0.004f);
        break;
    }
    case HERO_STATE_APPROACH_TARGET_POSE: {
        f32 l;
        f32 k;
        fast_vec_sub(&spatial_scratch_or_hit_descriptor, &hero.target_pose, &hero.motion.pos);
        hero.motion.move_yaw = fast_arc_tan(spatial_scratch_or_hit_descriptor.f[0],
                                            spatial_scratch_or_hit_descriptor.f[1]);
        l = vector_length_xy(&spatial_scratch_or_hit_descriptor);
        if (l > 0.2f) {
            hero.stick_magnitude = 1.0f;
            hero.motion.move_yaw = fast_arc_tan(spatial_scratch_or_hit_descriptor.f[0],
                                                spatial_scratch_or_hit_descriptor.f[1]);
            hero.target_speed = l;
            if (l < D_L03_0017BEB8.x0C) {
                hero.target_speed = D_L03_0017BEB8.x08 * frame_time;
            } else {
                hero.target_speed = D_L03_0017BEB8.x18 * frame_time;
            }
            if (hero.vehicle_mode == 3) {
                hero.target_speed = frame_time * 1.5f;
            } else if (hero.vehicle_mode == 1) {
                hero.target_speed = frame_time * 0.9f;
            }
        } else {
            hero.stick_magnitude = 0.5f;
            hero.motion.move_yaw = hero.target_pose.f[3];
            hero.target_speed = 0.0f;
        }
        k = 1.0f;
        if (hero.vehicle_mode == 3) {
            turn_to(0, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f, frame_time * 5.5850534f);
        } else if (hero.vehicle_mode == 1) {
            turn_to(0, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f, frame_time * 7.8539815f);
        } else if (hero.vehicle_mode == 0) {
            if (hero.state.step == 1) {
                turn_to(0, frame_scale_sq * 0.016f, frame_scale_sq * 0.15f, frame_time * 9.948377f);
            } else {
                turn_hero_toward_move_direction_with_stick_scale();
            }
        }
        if (hero.vehicle_mode == 0) {
            adjust_hero_pose_targets_from_motion();
        }
        speed_step(frame_time_sq * 7.5f * k, frame_time_sq * 8.5f);
        set_planar_vel(99999.0f);
        if (hero.vehicle_mode == 0) {
            clip_hero_horizontal_velocity(1);
        }
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.displacement, frame_time_sq * 25.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    }
    case HERO_STATE_BRAKE_AT_TARGET_POSITION:
        fast_vec_sub(&spatial_scratch_or_hit_descriptor, &hero.target_pose, &hero.motion.pos);
        if (fast_vec_dot(&spatial_scratch_or_hit_descriptor, &hero.motion.momentum) < 0.0f) {
            clear_u64_value(&hero.motion.momentum);
        }
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.0f);
        clip_hero_horizontal_velocity(0);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.displacement, frame_time_sq * 25.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    case HERO_STATE_FIDGET:
    case HERO_STATE_AT_TARGET_POSE:
        clear_u64_value(&hero.motion.velocity);
        break;
    case HERO_STATE_VEHICLE_MODE_3_IDLE:
    case HERO_STATE_VEHICLE_MODE_3_DYING:
    case HERO_STATE_DELAYED_CIRCLE_RELEASE_WAIT:
        stop_hero_motion_without_forward_support(3.7f, 0.0f);
        stick_target(1.0f, 0);
        hero.target_speed = 0.0f;
        speed_step(0.0f, frame_time_sq * 12.6f);
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.6f);
        clip_hero_horizontal_velocity(0);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    case HERO_STATE_VEHICLE_MODE_3_SLIP:
        hero.motion.move_yaw = hero.downhill_yaw;
        hero.target_speed = frame_time * 5.5f;
        air_accel(frame_time_sq * 25.0f);
        FUN_L00_002334d0(
            &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 18.0f);
        break;
    case HERO_STATE_VEHICLE_MODE_3_FALL:
        stick_target(frame_time * 1.5f, 0);
        if (hero.move_lock_timer == 0) {
            hero_update_hop_velocity();
        }
        FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 21.0f);
        if (hero.motion.velocity.f[2] < frame_time * -15.0f) {
            hero.motion.velocity.f[2] = frame_time * -15.0f;
        }
        break;
    case HERO_STATE_VEHICLE_MODE_3_RUN: {
        f32 d;
        f32 k;
        f32 g;
        d = fast_difference_between_rotations(hero.motion.rot.f[2], hero.motion.move_yaw);
        stick_target(frame_time * 1.5f, 0);
        if (hero.target_speed < frame_time * 1.5f * 0.55f) {
            hero.target_speed = frame_time * 1.5f * 0.55f;
        }
        k = 1.0f;
        k = k - d * 0.3f;
        g = hero.stick_magnitude + 0.35f;
        if (k < 0.0f) {
            k = 0.0f;
        }
        turn_to(0, frame_scale_sq * 0.035f * g, frame_scale_sq * 0.1f, frame_time * 2.268928f * g);
        speed_step(frame_time_sq * 7.5f * k, frame_time_sq * 8.5f);
        if (hero.unk3B8 != 0 && hero.unk308 == 0) {
            set_planar_vel(hero.motion.move_yaw);
        } else {
            set_planar_vel(99999.0f);
        }
        clip_hero_horizontal_velocity(1);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_HURT: {
        f32 len = vector_length_xy(&hero.motion.velocity);
        f32 pre_damping_planar_speed = len;
        if (hero.air_frames.s != 0) {
            len = frame_scale * -0.00999999f * pre_damping_planar_speed + pre_damping_planar_speed;
        } else {
            len = frame_scale * -0.07999998f * pre_damping_planar_speed + pre_damping_planar_speed;
        }
        FUN_L00_001ff600(len, &hero.motion.velocity, &hero.motion.velocity);
        hero.motion.velocity.f[2] = 0.0f;
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity, frame_time_sq * 24.0f);
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, 0.004f);
        }
        break;
    }
    case HERO_STATE_CANCEL_ITEM_WITH_OPTIONAL_IDLE_ANIMATION:
    case HERO_STATE_IDLE_ANIMATION_WITH_DYING_OVERRIDE:
        clear_u64_value(&hero.motion.velocity);
        break;
    }
    if (sub != hero.state.step) {
        hero.step_frames = 0;
    }
    if (anim != moby->b53) {
        hero.anim_seq_frames = 0;
    }
}
