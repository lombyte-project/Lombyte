/* Level 14 hero motion: update_hero_motion_by_state, with the surface reaction, hit reaction and smoothed parameter helpers. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/hero.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/overlay/hero_tables.h"
#include "rnc/data_labels.h"
#include "rnc/input/pad_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/overlay/shared/hero/adjust_hero_pose_targets_from_motion.h"
#include "rnc/world/collision/collision_hit_material.h"

/* Resolve the hero's surface reaction for the frame from the previous state and flags. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/help_0021E3A8.c: func_L14_0021E3A8), where it is exact; names translated to the US level program. */

extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");
extern int  hero_set_state(int, int) __asm__("FUN_L14_0022ff58");
float       absolute_float(float input) __asm__("func_001F99C0");

void resolve_hero_surface_reactions(void) __asm__("FUN_L14_0021dd30");

void resolve_hero_surface_reactions(void) {
    struct Hero *hero_ptr;
    struct Hero *hero_data = &hero;
    int          keep = hero_data->ground_sound_class;
    int          saved_ground_type = hero_data->ground_type;
    fast_mem_zero16(&hero_data->ground_type, 0x10);
    hero_data->ground_sound_class = keep;
    hero_data->ground_type = -1;
    hero_data->wading = 0;
    hero_data->unk308 = 0;
    if (saved_ground_type == -1)
        return;
    if (saved_ground_type == 2) {
        if (hero_data->frames_off_ground.s == 0 || hero_data->ground_distance < 0.3f) {
            hero_data->on_magnet_ground = 1;
            if (hero_data->vehicle_mode == 0) {
                char *item_moby_bytes = ((char *)hero_data->items[1].moby);
                if (item_moby_bytes != 0 && *(short *)(item_moby_bytes + 0xA6) == 0xAD) {
                    hero_data->unk308 = 1;
                }
            }
        }
    }
    if (current_level_index == 0xD) {
        struct Hero *q = &hero;
        if (q->state.current != 0x7B) {
            if (coll_sphere(q->capsule_radius + 0.03f, &q->motion.body_center, 2, 0)) {
                if (collision_hit_material() == 0xB) {
                    hero_set_state(0x7B, 1);
                    return;
                }
            }
        }
    }
    if (saved_ground_type == 0xB) {
        struct Hero *q = &hero;
        q->selector_11 = 1;
    }
    if (saved_ground_type == 2) {
        struct Hero *q = &hero;
        q->on_magnet_ground = 1;
    }
    if (saved_ground_type == 7) {
        struct Hero *q = &hero;
        q->on_ice_ground = 1;
    }
    if (saved_ground_type == 0xD) {
        struct Hero *q = &hero;
        q->selector_13 = 1;
    }
    if (saved_ground_type == 8) {
        struct Hero *q = &hero;
        q->on_slip_ground = 1;
    }
    if (saved_ground_type == 9) {
        struct Hero *q = &hero;
        q->unk12EE = 1;
    }
    if (saved_ground_type == 0xC) {
        struct Hero *q = &hero;
        q->on_slip_ground = 1;
    }
    hero_ptr = &hero;
    if (hero_ptr->selector_13 != 0) {
        if (hero_ptr->state.current == 0x7F)
            return;
        if (absolute_float(hero_ptr->water_surface_z - (hero_ptr->motion.pos.f[2] + 0.25f)) <
            1.0f) {
            if (hero_ptr->water_surface_z - hero_ptr->motion.pos.f[2] > 0.0f) {
                if (hero_ptr->motion.displacement.f[2] < 0.0f) {
                    exit_hero_vehicle_mode_if_mode_three();
                    hero_set_state(HERO_STATE_SINK, 1);
                }
            }
        }
    }
}

#include "qcopy.h"
#define G hero

/* Same source as the exact FUN_L08_00222800, with only the 0x16 animation in mode 0. */
extern f32 D_0015ED6C_8 __asm__("D_0015ED6C");

void release_and_clear_hero_voice_slots(void) __asm__("FUN_L00_00206c08");
void FUN_001f9a28_8(void *, void *, void *) __asm__("FUN_001f9a28");
f32  FUN_001f9dc8_8(f32) __asm__("FUN_001f9dc8");
f32  FUN_001f9de0_8(f32) __asm__("FUN_001f9de0");
void FUN_001f9a10_8(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_L00_00216de8_8(s32, s32) __asm__("FUN_L00_00216de8");
void hero_knockback(void *, s32, f32, f32) __asm__("FUN_L00_00210c80");

#define G hero

extern OvlMobyEntry40 l14_moby_hit_records[] __asm__("D_L14_00178800");
s32                   process_hero_hit_reaction(s32 apply_hit_reaction) __asm__("FUN_L14_00222aa8");

s32 process_hero_hit_reaction(s32 apply_hit_reaction) {
    u8             *hero_moby_bytes;
    OvlMobyEntry40 *hero_hit_entry;
    u8             *mob;
    s32             flag;
    s32             id;
    f32             horizontal_knockback_scale, vertical_knockback_scale, knockback_scale;
    OvlVec4         knockback_impulse;

    if (G.state.control_mode == 0x14) {
        return 0;
    }
    if (G.state.control_mode == 7) {
        return 0;
    }
    if (G.state.current == 0x32) {
        return 0;
    }
    if (G.invulnerable_frames != 0) {
        return 0;
    }
    if (D_L14_0015F5C4 != 0) {
        return 0;
    }
    G.damage_source_moby = 0;
    hero_moby_bytes = (u8 *)G.moby;
    if (hero_moby_bytes[0xA4] == 0xFF) {
        return 0;
    }
    hero_hit_entry = &l14_moby_hit_records[hero_moby_bytes[0xA4]];
    if (hero_hit_entry->owner != hero_moby_bytes) {
        return 0;
    }
    if ((hero_hit_entry->unk24 ^ 1) & 1) {
        return 0;
    }
    D_0013DF88[current_level_index]++;
    G.damage_source_moby = (struct Moby *)hero_hit_entry->target;
    D_0015EEA8++;
    if (G.state.control_mode == 0xF) {
        G.unk5BE = 1;
        return 0;
    }
    if (apply_hit_reaction == 0) {
        return 1;
    }
    release_and_clear_hero_voice_slots();
    flag = 0;
    if (hero_hit_entry->flags & 1) {
        qcopy(&knockback_impulse, &hero_hit_entry->pos);
        if (*(f32 *)((u8 *)hero_hit_entry + 0x1C) == 5627.9248046875f) {
            flag = 1;
        }
    } else if (hero_hit_entry->target != 0) {
        FUN_001f9a28_8(&knockback_impulse, (u8 *)&G + 0x80, hero_hit_entry->target + 0x10);
    } else {
        knockback_impulse.f[0] = FUN_001f9dc8_8(fast_add_rotations(G.motion.rot.f[2], 3.1415927f));
        knockback_impulse.f[1] = FUN_001f9de0_8(fast_add_rotations(G.motion.rot.f[2], 3.1415927f));
        knockback_impulse.i[2] = 0;
    }
    switch (G.vehicle_mode) {
    case HERO_VEHICLE_MODE_ON_FOOT:
        mob = (u8 *)G.damage_source_moby;
        if (mob != 0 && (*(s16 *)(mob + 0xA6) == 0x4EB || *(s16 *)(mob + 0xA6) == 0x558)) {
            id = 0x80;
        call_ret:
            hero_set_state(id, 1);
            return 1;
        }
        if (G.state.control_mode == 0x16) {
            hero_set_state(0x6D, 1);
            G.motion.resolved_velocity_z.f[2] = D_0015ED6C_8 * 7.0f;
            return 1;
        }
        hero_set_state(HERO_STATE_HURT, 1);
        knockback_scale = D_0015ED6C_8;
        horizontal_knockback_scale = knockback_scale * 5.7f;
        vertical_knockback_scale = knockback_scale * 2.4f;
        if (G.on_magnet_ground != 0) {
            horizontal_knockback_scale = 0.0f;
            vertical_knockback_scale = knockback_scale * 1.7f;
        }
        hero_knockback(
            &knockback_impulse, flag, horizontal_knockback_scale, vertical_knockback_scale);
        if (flag != 0 && hero_hit_entry->unk28 == 4) {
            knockback_impulse.f[2] += knockback_impulse.f[2];
        }
        break;
    case 3:
        hero_set_state(0x56, 1);
        hero_knockback(&knockback_impulse, flag, D_0015ED6C_8 * 5.0f, D_0015ED6C_8 * 2.4f);
        break;
    }
    FUN_001f9a10_8(&hero.motion.velocity, &hero.motion.velocity, &knockback_impulse);
    FUN_001f9a10_8(&hero.motion.displacement, &hero.motion.displacement, &knockback_impulse);
    return 1;
}

/* Same source as the exact FUN_L01_00231ae0, with a b12E2/i300 branch in place of its 0x11/0x12 branch. */

extern f32 D_0015ED6C_231ae0 __asm__("D_0015ED6C");
f32        approach_value_231ae0(f32, f32, f32 *) __asm__("FUN_00213ed8");
void       FUN_L00_0025b8c0_231ae0(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
void       update_hero_smoothed_parameters(void) __asm__("FUN_L14_00222f38");

void update_hero_smoothed_parameters(void) {
    switch (hero.vehicle_mode) {
    case HERO_VEHICLE_MODE_ON_FOOT:
        hero.capsule_top_target = 0.8f;
        hero.capsule_bottom_target = 0.7f;
        hero.capsule_radius_target = 0.45f;
        break;
    case 3:
        hero.capsule_top_target = 0.8f;
        hero.capsule_bottom_target = 0.6f;
        hero.capsule_radius_target = 0.45f;
        break;
    }
    if (hero.state.control_mode == 4) {
        if (hero.state_timer > hero.takeoff_frame && hero.past_jump_apex == 0)
            hero.capsule_bottom_target = hero.unk434;
    } else if (hero.state.current == HERO_STATE_FALL) {
        hero.capsule_bottom_target = 0.5f;
    } else if (hero.state.current == HERO_STATE_CROUCH) {
        hero.capsule_top_target = 0.35000002f;
    } else if (hero.on_ice_ground && hero.grounded_frames) {
        hero.capsule_top_target = 0.8f;
        hero.capsule_bottom_target = 0.9f;
    } else if (hero.state.current == HERO_STATE_SINK) {
        hero.capsule_radius_target = 0.8f;
    }
    if (!hero.wall_hit || hero.state.prev_control_mode == 0x12 || hero.state.control_mode == 0x11 ||
        hero.base_condition ||
        vector_distance_xy(hero.wall_contact_point.f, hero.motion.pos.f) >
            hero.capsule_radius * 0.5f) {
        f32         *smoothed_values = D_0013F570;
        struct Hero *hero_ptr = (struct Hero *)((u8 *)smoothed_values - 0x220);
        approach_value_231ae0(hero_ptr->capsule_top_target, frame_scale * 0.02f, smoothed_values);
        approach_value_231ae0(
            hero_ptr->capsule_bottom_target, frame_scale * 0.02f, smoothed_values + 1);
        FUN_L00_0025b8c0_231ae0(smoothed_values + 5,
                                smoothed_values + 6,
                                hero_ptr->capsule_radius_target,
                                frame_scale_sq * 0.02f,
                                frame_scale_sq * 0.3f,
                                D_0015ED6C_231ae0 * 4.0f);
    }
}

#include "eetypes.h"

#include "rnc/math/vector.h"
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
/* Second name for hero: one call needs it to keep the original address CSE. */
extern struct Hero hero_b __asm__("D_0013F350");

struct PadState {
    u8  pad0[0x128];
    f32 f128;
    f32 f12C;
    u8  pad130[0x70];
    union {
        u64 d1A0;
        s32 i1A0;
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

extern f32 D_0015ED6C_a __asm__("D_0015ED6C");
s32        D_L14_0015F68C = 0;

extern void emit_hero_periodic_surface_effect(s32, s32) __asm__("FUN_L00_00209540");
extern void build_scaled_hero_move_impulse(f32, f32) __asm__("FUN_L00_002132b8");
extern void stop_hero_motion_without_forward_support(f32, f32) __asm__("FUN_L00_00217118");
extern void stick_target(f32, s32) __asm__("FUN_L00_002118c8");
extern void turn_to(s32, f32, f32, f32) __asm__("FUN_L00_00211e30");
extern s32  try_start_hero_item_idle_animation(void) __asm__("FUN_L00_00229a98");
extern void turn_hero_toward_move_direction_for_item(void) __asm__("FUN_L00_00211e98");
extern void set_hero_animation(s32, s32, f32) __asm__("FUN_L00_002323b8");
extern s32  hero_get_gadget_anim_id(s32) __asm__("FUN_L12_00227f10");
extern void
    adjust_hero_animation_speed_for_target_frame(f32, f32, f32, f32) __asm__("FUN_L00_00208f20");
extern void speed_step(f32, f32) __asm__("FUN_L00_00212088");
extern void reduce_and_apply_hero_planar_velocity_addend(f32) __asm__("FUN_L00_00216710");
extern void FUN_L00_002334d0(void *, void *, f32);
extern void clip_hero_horizontal_velocity(s32) __asm__("FUN_L01_00232978");
extern void correct_hero_airborne_wall_velocity(void) __asm__("FUN_L00_002121c0");
extern void spawn_hero_radial_particle_burst(void) __asm__("FUN_L00_00217450");
extern s32  allocate_voice_for_target_entry(s32, s32, struct MobyView *) __asm__("FUN_0022da68");
extern s32  FUN_L00_001ff600(f32, void *, void *);
extern void update_hero_water_height_and_bobbing(void) __asm__("FUN_L00_00227d40");
extern struct PadState D_0013C940;
#define PD D_0013C940
extern struct PlayerJointTable D_L14_0017AD80;
#define AN D_L14_0017AD80
struct VoiceSlot {
    u8               pad0[0x74];
    u8               b74;
    u8               pad75[0x13];
    struct MobyView *pm88;
};

extern void release_voice_slot_cf(s32, struct MobyView *, s32) __asm__("FUN_L00_00216e48");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void emit_hero_randomized_motion_effects(s32, s32) __asm__("FUN_L00_00209a40");
extern void emit_hero_surface_effect_burst(s32, s32, s32) __asm__("FUN_L00_00209ca8");
extern void pool_splash(s32, s32, s32, f32, f32, f32, f32) __asm__("FUN_L00_002a3ec8");
extern f32  spring_angle(f32 *, f32, f32 *, s32, f32, f32, f32) __asm__("FUN_L00_0025bc98");
extern void moby_point_position(struct MobyView *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern u8  *FUN_L00_0026f080(void *, void *, f32, f32);
extern s32  random_int_between(s32, s32) __asm__("FUN_L00_00257b90");
extern void FUN_L00_00262cf0(void *, void *, f32, f32, f32);
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern f32  FUN_L00_00209018(void *, s32);
extern f32  approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern void set_first_hero_easing_parameters(f32, f32) __asm__("FUN_L00_0020a1e0");
extern void FUN_L14_0021c5c8(f32, f32);
extern void FUN_L14_0021c5f8(f32, f32);
extern void FUN_L14_0021c5e0(f32, f32);
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
extern struct HeroTableRow2C D_L14_0017C328[];
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

extern struct PlayerSpeedTuning D_L14_0017C4B8;
extern s32                      allocate_hero_voice_for_entry(s32, s32) __asm__("FUN_L00_00216de8");
extern void add_hero_forward_offset(void *, void *, f32) __asm__("FUN_L00_00233708");
extern void FUN_L00_00233810(void *, void *, f32);
extern s32  collision_line(void *, void *, s32, struct MobyView *, s32) __asm__("FUN_001efa68");
extern void transform_hero_local_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_00233660");
extern s32  is_moby_in_class_range_500_to_540(s32) __asm__("FUN_L00_0025e3b8");
extern void
spawn_nearby_effect(s32, struct MobyView *, s32, void *, void *, f32) __asm__("FUN_L00_00259bc8");
extern s32  l14_sphere_query_moby_results[] __asm__("D_L14_00178600");
extern void emit_hero_timed_effects(void) __asm__("FUN_L00_00209ec8");
extern void gravity_flatten(void *, void *) __asm__("FUN_L00_002338d0");
extern s32  hero_slot_item_id(s32) __asm__("FUN_L00_0020d498");
extern void probe_hero_airborne_wall_contact(void) __asm__("FUN_L00_0020b5b8");
extern void update_hero_ledge_probe(void) __asm__("FUN_L01_0022d090");
extern f32  l14_ledge_shimmy_speed_by_animation_frame[] __asm__("D_L14_001C43B0");
extern void steer_hero_planar_velocity(f32) __asm__("FUN_L00_00213e68");
extern void rotate_hero_about_fixed_local_pivot(f32, f32, f32) __asm__("FUN_L00_002091d8");
extern void hero_update_hop_movement(void) __asm__("FUN_L12_00235c08");
extern void compute_hero_fixed_offset_world_position(void *) __asm__("FUN_L00_0020d2f8");
extern f32  randf_sym(f32, f32) __asm__("FUN_L00_00257c48");
extern s32  FUN_L00_0025df68(void *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);
extern s32  sample_path(void *, void *, s32 *, f32 *, f32, s32) __asm__("FUN_L00_0025d808");
extern s32  offset_path_index(void *, s32, s32, s32) __asm__("FUN_L00_0025d7a0");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void hero_integrate_velocity(void) __asm__("FUN_L00_00214108");
extern u8   D_L14_0017A1C0[];
extern struct HeroAnimRow D_L14_0017A1C0_t[] __asm__("D_L14_0017A1C0");
extern void spawn_random_alpha_rotated_particle(void *, void *, f32) __asm__("FUN_L00_00271b18");
extern void update_hero_lean_and_sway_parameters(void) __asm__("FUN_L00_00215ef8");
extern void turn_hero_toward_move_direction_with_stick_scale(void) __asm__("FUN_L00_00211f38");
extern f32  D_L14_0017C4C4[] __asm__("D_L14_0017C4B8+12");
extern void update_hero_stick_target_magnitude(void) __asm__("FUN_L02_00220b80");
extern void initialize_moby_transform_history(void *, void *) __asm__("FUN_L00_00262500");
extern void create_moby_transform_history_entry(void *, s32, s32) __asm__("FUN_L00_00262528");
extern void update_moby_transform_history(void *, s32) __asm__("FUN_L00_00262608");
extern void emit_filtered_entry_sparkles(void *, s32) __asm__("FUN_L00_00217368");
extern void launch_hero_wrench(void) __asm__("FUN_L00_00217658");
extern f32  set_planar_vel(f32) __asm__("FUN_L00_002120d8");
extern s32  test_hero_animation_frame_window(f32) __asm__("FUN_L00_00211870");

void update_hero_motion_by_state(void) __asm__("FUN_L14_00228cd0");

void update_hero_motion_by_state(void) {
    struct MobyView *moby = MV(hero.moby);
    s32              sub = hero.state.step;
    s32              anim = moby->b53;
    f32              speed;
    Vec4             vector_and_descriptor_scratch;
    Vec4             v10;
    Vec4             vector_and_hit_descriptor_scratch;
    Vec4             motion_vector_scratch;
    Vec4             matrix_and_vector_scratch[4];
    Vec4             transposed_yaw_matrix[4];
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
        if (hero.on_ice_ground != 0) {
            build_scaled_hero_move_impulse(0.7f, frame_time * 0.0f);
        } else {
            build_scaled_hero_move_impulse(0.5f, frame_time * 2.0f);
        }
        stop_hero_motion_without_forward_support(3.7f, 0.0f);
        stick_target(1.0f, 0);
        if (hero.state.current == HERO_STATE_LOOK || hero.state.current == 0x1E) {
            if (hero.unk20A5 != 0) {
                vector_and_descriptor_scratch.q = 0;
                vector_and_descriptor_scratch.f[0] = 1.0f;
                transform_vector_by_basis(
                    &hero.motion.move_dir, &vector_and_descriptor_scratch, &D_L14_001674D8[0x1F8]);
                hero.motion.move_yaw = *(f32 *)D_L14_001674D8;
                turn_to(0, frame_scale_sq, frame_scale_sq * 0.0f, frame_time * 174.53293f);
                if (D_L14_0015F688 > 0 && fast_dec_timer(&D_L14_0015F688) != 0) {
                    s32 t;
                    if (usage_stats.stat[18].count < 0xFFFF) {
                        usage_stats.stat[18].count++;
                    }
                    t = scale_game_frames(D_0015EEA4) / 600;
                    if (usage_stats.stat[18].unk2 < t) {
                        usage_stats.stat[18].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                    D_L14_0015F688 = -1;
                    usage_stats.stat[18].level_mask =
                        usage_stats.stat[18].level_mask | (1 << current_level_index) | 0x80000000;
                    D_L14_0015F68C++;
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
                        anim = hero_get_gadget_anim_id(0);
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
            if (hero.on_ice_ground != 0) {
                speed = frame_time_sq * 1.7f;
            } else {
                speed = frame_time_sq * 6.48f;
            }
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
            s32 an = hero_get_gadget_anim_id(0);
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
        qcopy(&vector_and_descriptor_scratch, &MV(hero.grapple_target)->u10.v10);
        hero.unk970 = fast_vec_dist(&hero.motion.pos, &vector_and_descriptor_scratch);
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
                -fast_arc_tan(vector_distance_xy(&hero.motion.pos, &vector_and_descriptor_scratch),
                              vector_and_descriptor_scratch.f[2] - hero.motion.pos.f[2]),
                1.5707964f);
            step_angle_toward_target(&hero.motion.rot.f[1], 0, an, frame_time * 6.4577184f);
        }
        vector_and_hit_descriptor_scratch.f[0] =
            fast_cos(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * -0.57f;
        vector_and_hit_descriptor_scratch.f[1] =
            fast_sin(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * -0.57f;
        vector_and_hit_descriptor_scratch.f[2] = 0.0f;
        fast_vec_add(&vector_and_hit_descriptor_scratch,
                     &vector_and_hit_descriptor_scratch,
                     &hero.motion.pos);
        fast_vec_sub(&v10, &MV(hero.grapple_target)->u10.v10, &vector_and_hit_descriptor_scratch);
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
        qcopy(&vector_and_descriptor_scratch, &MV(hero.grapple_target)->u10.v10);
        hero.unk970 = fast_vec_dist(&hero.motion.pos, &vector_and_descriptor_scratch);
        fast_vec_add(&v10, &hero.motion.pos, &hero.motion.velocity);
        fast_vec_sub(&motion_vector_scratch, &v10, &vector_and_descriptor_scratch);
        fast_vec_normalize(&motion_vector_scratch, &motion_vector_scratch, hero.tether_radius);
        fast_vec_add(&vector_and_hit_descriptor_scratch,
                     &vector_and_descriptor_scratch,
                     &motion_vector_scratch);
        fast_vec_sub(&hero.motion.velocity, &vector_and_hit_descriptor_scratch, &hero.motion.pos);
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
        fast_vec_sub(
            &vector_and_descriptor_scratch, &MV(hero.swing_target_moby)->u10.v10, &hero.motion.pos);
        fast_vec_normalize(&vector_and_descriptor_scratch, &vector_and_descriptor_scratch, 1.0f);
        if (vector_and_descriptor_scratch.f[2] < 0.23f) {
            vector_and_descriptor_scratch.f[2] = 0.23f;
        }
        clear_u64_value(&vector_and_hit_descriptor_scratch);
        vector_and_hit_descriptor_scratch.f[2] = hero.motion.rot.f[2];
        euler_rotation_matrix(matrix_and_vector_scratch, &vector_and_hit_descriptor_scratch);
        transpose_rotation_matrix(transposed_yaw_matrix, matrix_and_vector_scratch);
        transform_vector(
            &vector_and_descriptor_scratch, &vector_and_descriptor_scratch, transposed_yaw_matrix);
        v10.f[0] = -fast_arc_tan(
            square_root(vector_and_descriptor_scratch.f[0] * vector_and_descriptor_scratch.f[0] +
                        vector_and_descriptor_scratch.f[2] * vector_and_descriptor_scratch.f[2]),
            vector_and_descriptor_scratch.f[1]);
        v10.f[1] =
            fast_arc_tan(vector_and_descriptor_scratch.f[2], vector_and_descriptor_scratch.f[0]);
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
                    motion_vector_scratch.f[0] = fast_cos(hero.motion.move_yaw) * s;
                    motion_vector_scratch.f[1] = fast_sin(hero.motion.move_yaw) * s;
                    motion_vector_scratch.f[2] = 0.0f;
                    fast_vec_add(
                        &hero.motion.velocity, &hero.motion.velocity, &motion_vector_scratch);
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
                    fast_vec_sub(&motion_vector_scratch,
                                 &hero.motion.pos,
                                 &MV(hero.swing_target_moby)->u10.v10);
                    FUN_L00_002334d0(
                        &hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 57.0f);
                }
            }
            s = fast_vec_length(&hero.motion.velocity) * (frame_time * 0.015f);
            fast_vec_scale(&motion_vector_scratch, &hero.motion.velocity, -1.0f);
            fast_vec_normalize(&motion_vector_scratch, &motion_vector_scratch, s);
            fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &motion_vector_scratch);
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
            f32 swing_radius_before_spring;
            f32 d;
            vD0.f[0] = fast_cos(a) * 0.15f;
            vD0.f[1] = fast_sin(a) * 0.15f;
            vD0.f[2] = 0.0f;
            fast_vec_add(&vD0, &vD0, &hero.motion.pos);
            fast_vec_add(&motion_vector_scratch, &vD0, &hero.motion.velocity);
            fast_vec_sub(&vC0, &motion_vector_scratch, &MV(hero.swing_target_moby)->u10.v10);
            fast_vec_normalize(&vC0, &vC0, hero.unk9A4);
            fast_vec_add(&motion_vector_scratch, &MV(hero.swing_target_moby)->u10.v10, &vC0);
            fast_vec_sub(&hero.motion.velocity, &motion_vector_scratch, &vD0);
            swing_radius_before_spring = hero.unk9A4;
            vendor_spring(&hero.unk9A4,
                          &hero.swing_length_vel,
                          hero.unk9A0,
                          hero.swing_rope_spring_k,
                          hero.unk9C8,
                          hero.swing_rope_max_speed);
            d = hero.unk9A4 - swing_radius_before_spring;
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
                motion_vector_scratch.f[0] = fast_cos(hero.swing_yaw) * hero.swing_push_speed;
                motion_vector_scratch.f[1] = fast_sin(hero.swing_yaw) * hero.swing_push_speed;
                motion_vector_scratch.f[2] = 0.0f;
                fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &motion_vector_scratch);
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
                    motion_vector_scratch.q = hero.motion.displacement.q;
                    up = forward_component(&motion_vector_scratch) > 0.0f;
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
            qcopy(&vector_and_descriptor_scratch, &hero.motion.pos);
            vector_and_descriptor_scratch.f[2] -= 0.5f;
            matrix_and_vector_scratch[0].f[0] = fast_cos(hero.motion.rot.f[2]);
            matrix_and_vector_scratch[0].f[1] = fast_sin(hero.motion.rot.f[2]);
            matrix_and_vector_scratch[0].f[2] = 0.0f;
            make_hit_descriptor(&v10, MV(hero.moby), 0x30000, 1.0f, &matrix_and_vector_scratch[0]);
            collect_mobies_in_sphere(
                0.8f, &vector_and_descriptor_scratch, 0x10, MV(hero.moby), &v10);
        }
        if (hero.past_apex != 0.0f && hero.air_frames.s == 0) {
            if (hero.rise_fall_land_frames == 0) {
                l14_camera.shake_amount = 0.2f;
                l14_camera.shake_frames = scale_game_frames(40);
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
        if (hero.anim_changing != 0 || hero.anim_frame < D_L14_0017C328[hero.attack_record].f18) {
            hero.items[0].timer = 2;
        }
        hero.target_speed = 0.0f;
        if (hero.on_ice_ground != 0) {
            if (MV(hero.melee_target) != NULL) {
                f32 d = vector_distance_xy(&hero.motion.pos, &MV(hero.melee_target)->u10.v10);
                if (d > 2.0f && d < 5.0f) {
                    hero.target_speed = frame_time * 4.5f;
                }
                speed_step(frame_time_sq * 4.0f, frame_time_sq * 4.0f);
                hero.motion.velocity.f[0] = fast_cos(hero.motion.rot.f[2]) * hero.ground_speed;
                hero.motion.velocity.f[1] = fast_sin(hero.motion.rot.f[2]) * hero.ground_speed;
            } else {
                speed_step(frame_time_sq * 2.0f, frame_time_sq * 2.0f);
                set_gravity_relative_vector_length_xy(
                    &hero.motion.velocity, &hero.motion.velocity, hero.ground_speed);
            }
        } else {
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
            qcopy(&vector_and_descriptor_scratch, &hero.motion.pos);
            offset_hero_position_against_gravity(
                &vector_and_descriptor_scratch, &vector_and_descriptor_scratch, -0.5f);
            matrix_and_vector_scratch[0].f[0] = fast_cos(hero.motion.rot.f[2]) * 2.0f;
            matrix_and_vector_scratch[0].f[1] = fast_sin(hero.motion.rot.f[2]) * 2.0f;
            matrix_and_vector_scratch[0].f[2] = 0.0f;
            make_hit_descriptor(&v10, MV(hero.moby), 0x10000, 2.0f, &matrix_and_vector_scratch[0]);
            v10.f[3] = 5627.925f;
            ((u8 *)&vector_and_hit_descriptor_scratch)[9] = 1;
            ((s16 *)&vector_and_hit_descriptor_scratch)[5] = 0x47;
            ((u8 *)&vector_and_hit_descriptor_scratch)[8] = 0;
            collect_mobies_in_sphere(
                0.4f, &vector_and_descriptor_scratch, 0x10, MV(hero.moby), &v10);
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
        if (hero.anim_changing != 0 || hero.anim_frame < D_L14_0017C328[hero.attack_record].f18) {
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
            if (hero.on_ice_ground != 0 && hero.motion.speed_xy < frame_time * 2.0f) {
                hero.motion.velocity.f[0] += fast_cos(hero.motion.rot.f[2]) * (frame_time * 1.3f);
                hero.motion.velocity.f[1] += fast_sin(hero.motion.rot.f[2]) * (frame_time * 1.3f);
                qcopy(&hero.motion.resolved_velocity_xy, &hero.motion.velocity);
            }
            launch_hero_wrench();
        }
        if (hero.on_ice_ground != 0) {
            if (test_hero_animation_frame_window(84.0f) != 0) {
                f32 a = fast_add_rotations(hero.motion.rot.f[2], 3.1415927f);
                f32 s = frame_time * 2.5f;
                if (pad_held[0] & 0xA) {
                    s = frame_time * 3.0f;
                }
                hero.motion.velocity.f[0] += fast_cos(a) * s;
                hero.motion.velocity.f[1] += fast_sin(a) * s;
                qcopy(&hero.motion.resolved_velocity_xy, &hero.motion.velocity);
            }
            update_hero_planar_movement_velocity();
        } else {
            speed_step(frame_time_sq * 37.0f, frame_time_sq * 28.0f);
            set_planar_vel(99999.0f);
            if (hero.air_frames.s != 0) {
                FUN_L00_002334d0(
                    &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
            } else {
                FUN_L00_002334d0(
                    &hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
            }
        }
        break;
    }
    case HERO_STATE_WRENCH_COMBO: {
        struct HeroTableRow2C *tb;
        if (hero.on_ice_ground != 0) {
            update_hero_planar_movement_velocity();
            break;
        }
        if (hero.anim_changing != 0 || hero.anim_frame < D_L14_0017C328[hero.attack_record].f20) {
            hero.items[0].timer = 2;
        }
        tb = &D_L14_0017C328[hero.attack_record];
        hero.target_speed = 0.0f;
        if (MV(hero.melee_target) != NULL && scale_game_frames(4) > hero.state_timer) {
            f32 d = vector_distance_xy(&MV(hero.melee_target)->u10.v10, &hero.motion.pos) - 0.5f;
            f32 attack_spacing_offset;
            f32 tol;
            f32 mx;
            f32 attack_spacing_error;
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
            attack_spacing_error = d - attack_spacing_offset;
            if (absolute_float(attack_spacing_error) < tol + 3.0f) {
                f32 s = attack_spacing_error / 0.9f;
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
        if (hero.on_ice_ground != 0) {
            if (t1 < hero.state_timer && hero.state_timer < t1 + scale_game_frames(7) &&
                hero.motion.forward_speed < frame_time * 8.5f) {
                hero.motion.velocity.f[0] += fast_cos(hero.motion.rot.f[2]) * (frame_time * 1.2f);
                hero.motion.velocity.f[1] += fast_sin(hero.motion.rot.f[2]) * (frame_time * 1.2f);
                qcopy(&hero.motion.resolved_velocity_xy, &hero.motion.velocity);
            }
            update_hero_planar_movement_velocity();
        } else {
            if (t1 < hero.state_timer) {
                speed_step(frame_time_sq * 190.0f, frame_time_sq * 90.0f);
            }
            if (hero.heading_locked != 0) {
                set_planar_vel(hero.attack_target_yaw);
            } else {
                set_planar_vel(99999.0f);
            }
        }
        if (hero.state.current == 0x20 && t1 < hero.state_timer &&
            hero.state_timer < t2 + scale_game_frames(4)) {
            struct MobyView *o = MV(hero.items[0].moby);
            if (o != NULL) {
                f32 a;
                motion_vector_scratch.f[0] = fast_cos(hero.motion.rot.f[2]) * 2.0f;
                motion_vector_scratch.f[1] = fast_sin(hero.motion.rot.f[2]) * 2.0f;
                motion_vector_scratch.f[2] = 1.3f;
                make_hit_descriptor(&vector_and_descriptor_scratch,
                                    MV(hero.moby),
                                    0x30000,
                                    3.0f,
                                    &motion_vector_scratch);
                ((u8 *)&v10)[9] = 3;
                vector_and_descriptor_scratch.f[3] = 5627.925f;
                ((u8 *)&v10)[8] = 0;
                a = hero.motion.rot.f[2];
                ((s16 *)&v10)[5] = o->hA6;
                matrix_and_vector_scratch[0].f[0] = fast_cos(a) * 0.8f;
                matrix_and_vector_scratch[0].f[1] = fast_sin(a) * 0.8f;
                matrix_and_vector_scratch[0].f[2] = 0.55f;
                fast_vec_add(
                    &matrix_and_vector_scratch[0], &matrix_and_vector_scratch[0], &hero.motion.pos);
                emit_filtered_entry_sparkles(
                    o,
                    collect_mobies_in_sphere(
                        0.8f, &matrix_and_vector_scratch[0], 0, o, &vector_and_descriptor_scratch));
                a = fast_add_rotations(hero.motion.rot.f[2], -0.87266463f);
                matrix_and_vector_scratch[0].f[0] = fast_cos(a) * 0.8f;
                matrix_and_vector_scratch[0].f[1] = fast_sin(a) * 0.8f;
                matrix_and_vector_scratch[0].f[2] = 0.55f;
                fast_vec_add(
                    &matrix_and_vector_scratch[0], &matrix_and_vector_scratch[0], &hero.motion.pos);
                emit_filtered_entry_sparkles(
                    o,
                    collect_mobies_in_sphere(0.8f,
                                             &matrix_and_vector_scratch[0],
                                             0,
                                             MV(hero.moby),
                                             &vector_and_descriptor_scratch));
                a = fast_add_rotations(hero.motion.rot.f[2], 0.87266463f);
                matrix_and_vector_scratch[0].f[0] = fast_cos(a) * 0.8f;
                matrix_and_vector_scratch[0].f[1] = fast_sin(a) * 0.8f;
                matrix_and_vector_scratch[0].f[2] = 0.55f;
                fast_vec_add(
                    &matrix_and_vector_scratch[0], &matrix_and_vector_scratch[0], &hero.motion.pos);
                emit_filtered_entry_sparkles(
                    o,
                    collect_mobies_in_sphere(
                        0.8f, &matrix_and_vector_scratch[0], 0, o, &vector_and_descriptor_scratch));
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
    case HERO_STATE_DIRECTION_LIMITED_GROUND_MOVE: {
        f32 a;
        f32 f;
        update_hero_stick_target_magnitude();
        transform_vector(
            &vector_and_descriptor_scratch, &hero_b.motion.move_dir, (Vec4 *)hero_b.inv_rot_matrix);
        a = absolute_float(
            fast_arc_tan(vector_and_descriptor_scratch.f[0], vector_and_descriptor_scratch.f[1]));
        if (a > 0.69813168f) {
            f32 x0 = hero.target_speed;
            f32 x1 = hero.ground_speed;
            hero.target_speed = x0;
            hero.ground_speed = x1;
        }
        if (a > 1.2217305f) {
            hero.target_speed = 0.0f;
        } else {
            f = (1.2217305f - a) * 2.0f;
            if (f > 1.0f) {
                f = 1.0f;
            }
            hero.target_speed *= f;
        }
        {
            f32 g = hero.stick_magnitude + 0.35f;
            turn_to(
                0, frame_scale_sq * 0.025f * g, frame_scale_sq * 0.3f, frame_time * 5.2359877f * g);
        }
        speed_step(frame_time_sq * 7.5f, frame_time_sq * 8.5f);
        set_planar_vel(99999.0f);
        if (hero.ground_moby != 0) {
            qcopy(&hero.motion.pos, &hero.ground_point);
            hero.ground_distance = 0.0f;
            hero.air_frames.s = 0;
        }
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity, frame_time_sq * 24.0f);
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    }
    case HERO_STATE_RUN:
    case HERO_STATE_SHALLOW_WATER_RUN: {
        f32 k;
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
        update_hero_stick_target_magnitude();
        if (hero.unk930 > 0.0f) {
            Vec4 *move_impulse_ptr = &hero.move_impulse;
            build_scaled_hero_move_impulse(0.7f, 0.0f);
            vector_and_descriptor_scratch.q = hero.move_impulse.q;
            hero.target_speed += forward_component(&vector_and_descriptor_scratch);
            if (hero.target_speed < frame_time * 0.7f) {
                hero.target_speed = frame_time * 0.7f;
            }
            clear_u64_value(move_impulse_ptr);
        }
        k = 1.0f;
        if (hero.state.step == 1 && hero.unk3BE == 0) {
            turn_to(0, frame_scale_sq * 0.008f, frame_scale_sq * 0.15f, frame_time * 9.948377f);
        } else {
            f32 c = hero.stick_magnitude;
            f32 r = hero.motion.yaw_error;
            if (hero.item_anim_active != 0) {
                k = 1.0f - r * 0.55f;
                if (k < 0.0f) {
                    k = 0.0f;
                }
            } else if (c < 0.8f && r > 0.5235988f) {
                k = 0.0f;
            }
            if (c > D_L14_0017C4C4[0]) {
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
    case HERO_STATE_RUN_CLEAR_VELOCITY_Z: {
        f32 a;
        update_hero_planar_movement_velocity();
        set_first_hero_easing_parameters(0.04f, 0.2f);
        FUN_L14_0021c5c8(0.04f, 0.2f);
        FUN_L14_0021c5e0(0.04f, 0.2f);
        FUN_L14_0021c5f8(0.02f, 0.2f);
        if (moby->b53 == 0x37) {
            a = hero.motion.yaw_error;
            if (a > 1.4f) {
                a = 1.4f;
            } else if (a < -1.4f) {
                a = -1.4f;
            }
            AN.f60 = a * -0.14f;
            AN.f118 = a * 0.3f;
            AN.f1C0 = a * -0.22f;
            AN.f278 = a * 0.4f;
            AN.f274 = absolute_float(a) * -0.09f;
            AN.f270 = absolute_float(a) * 0.08f;
        } else if (moby->b53 == 0x6D) {
            f32 planar_direction_angle = fast_add_rotations(
                fast_subtract_rotations(
                    fast_arc_tan(hero.motion.displacement.f[0], hero.motion.displacement.f[1]),
                    hero.motion.rot.f[2]),
                3.1415927f);
            f32 m = hero.motion.flat_speed / (frame_time * 3.0f) * 0.34906584f;
            f32 c;
            f32 s;
            if (m > 0.43633232f) {
                m = 0.43633232f;
            }
            c = fast_cos(planar_direction_angle) * m;
            s = -fast_sin(planar_direction_angle);
            AN.f114 = c;
            AN.f110 = s * m;
        }
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.displacement, frame_time_sq * 25.0f);
        } else {
            hero.motion.velocity.f[2] = 0.0f;
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 150.0f);
        }
        break;
    }
    case HERO_STATE_GRIND:
    case HERO_STATE_GRIND_JUMP:
    case HERO_STATE_GRIND_RAIL_TRANSFER:
    case HERO_STATE_GRIND_ATTACK:
    case HERO_STATE_GRIND_COLLISION_RECOVERY: {
        s32 has;
        s32 flag;
        s32 near;
        s32 t;
        s32 path_heading_point_index;
        f32 r;
        if (hero.state.current != 0x29 && hero.state.current != 0x2A &&
            hero.state.current != 0x42) {
            release_voice_slot_cf(0, MV(hero.moby), 0);
        } else {
            if (hero.grind_voice != -1) {
                struct VoiceSlot *e =
                    (struct VoiceSlot *)(((u8 *)&voice_pool) + hero.grind_voice * 0x70);
                if (e->pm88 == MV(hero.moby) && e->b74 != 0) {
                    release_voice_slot(hero.grind_voice);
                }
            }
            hero.grind_voice = -1;
        }
        if (hero.state.current == HERO_STATE_GRIND || hero.state.current == 0x2B) {
            f32 planar_direction_angle;
            f32 rr;
            qcopy(&vector_and_descriptor_scratch, &hero.motion.pos);
            planar_direction_angle =
                fast_add_rotations(hero.rail_dir_yaw, randf_sym(0.0f, 0.5235988f));
            rr = random_float_between(frame_time * 4.0f, frame_time * 7.0f);
            v10.f[0] = fast_cos(planar_direction_angle) * rr;
            v10.f[1] = fast_sin(planar_direction_angle) * rr;
            v10.f[2] = 0.0f;
            v10.f[2] = random_float_between(frame_time * 3.0f, frame_time * 5.0f);
            v10.f[3] = -(frame_time_sq * 30.0f);
            spawn_color_decaying_effect_particle(&vector_and_descriptor_scratch, &v10, 0);
        }
        t = hero.items[0].item_id;
        has = D_L14_0017A1C0_t[t].unk18 != 0;
        if (t == 0xC) {
            has = 0;
        }
        flag = 0;
        if (has) {
            flag = hero.unk20AA != 0;
        }
        moby_point_position(MV(hero.moby), 9, &hero.joint9_world_pos);
        moby_point_position(MV(hero.moby), 0xE, &hero.joint14_world_pos);
        moby_point_position(MV(hero.moby), 0, &hero.root_joint_pos);
        if (hero.grind_pose == 2 || hero.grind_pose == 3) {
            hero.grind_pose = -1;
        }
        FUN_L00_0025df68((void *)hero.rail_spline,
                         &hero.motion.pos,
                         &hero.rail_point,
                         &hero.rail_segment,
                         &hero.rail_seg_frac,
                         hero.rail_spline_loops,
                         999.0f,
                         2.0f,
                         2.5f);
        if (hero.rail_switching != 0) {
            FUN_L00_0025df68((void *)hero.transfer_from_spline,
                             &hero.motion.pos,
                             &hero.prev_rail_point,
                             &hero.prev_rail_segment,
                             &hero.prev_rail_seg_frac,
                             hero.unk5A0,
                             999.0f,
                             2.0f,
                             2.5f);
        }
        near = 0;
        if (hero.ground_distance > 0.87f && hero.state.current != 0x29 &&
            hero.state.current != 0x42 && hero.rail_switching == 0) {
            near = scale_game_frames(30) < hero.control_mode_timer;
        }
        if (hero.rail_off_frames != 0 || near) {
            qcopy(&hero.motion.velocity, &hero.motion.resolved_velocity);
            if (hero.state.current == 0x29 || hero.state.current == 0x2A) {
                hero_integrate_velocity();
            } else {
                hero.motion.velocity.f[2] = hero.motion.velocity.f[2] - frame_time_sq * 24.0f;
            }
            hero.rail_off_frames++;
            break;
        }
        r = hero.unk574 * (f32)hero.grind_direction;
        if (hero.state.current == 0x42) {
            r = hero.unk5CC * (f32)hero.grind_direction;
        }
        sample_path((void *)hero.rail_spline,
                    &vector_and_descriptor_scratch,
                    &hero.rail_segment,
                    &hero.rail_seg_frac,
                    r,
                    hero.rail_spline_loops);
        if (hero.rail_switching != 0) {
            sample_path((void *)hero.transfer_from_spline,
                        &v10,
                        &hero.prev_rail_segment,
                        &hero.prev_rail_seg_frac,
                        r,
                        hero.unk5A0);
        }
        if (scale_game_frames(90) < hero.control_mode_timer) {
            s32 h;
            s32 si;
            f32 sf;
            f32 q = scale_time(30.0f) * hero.motion.speed_xy;
            qcopy(&vector_and_hit_descriptor_scratch, &hero.motion.pos);
            si = hero.rail_segment;
            sf = hero.rail_seg_frac;
            h = sample_path((void *)hero.rail_spline,
                            &vector_and_hit_descriptor_scratch,
                            &si,
                            &sf,
                            q,
                            hero.rail_spline_loops);
            qcopy(&motion_vector_scratch, &hero.motion.pos);
            si = hero.rail_segment;
            sf = hero.rail_seg_frac;
            if ((h | sample_path((void *)hero.rail_spline,
                                 &motion_vector_scratch,
                                 &si,
                                 &sf,
                                 q + 0.1f,
                                 hero.rail_spline_loops)) == 0 ||
                hero.rail_spline_loops != 0) {
                f32 a =
                    fast_subtract_rotations(
                        hero.rail_dir_yaw,
                        fast_arc_tan(
                            motion_vector_scratch.f[0] - vector_and_hit_descriptor_scratch.f[0],
                            motion_vector_scratch.f[1] - vector_and_hit_descriptor_scratch.f[1])) *
                    0.37f;
                if (a > 0.7853982f) {
                    a = 0.7853982f;
                }
                if (a < -0.7853982f) {
                    a = -0.7853982f;
                }
                set_first_hero_easing_parameters(0.03f, 0.2f);
                if (hero.grind_anim_variant != 0) {
                    AN.f64 = -a;
                } else {
                    AN.f64 = a;
                }
            }
        }
        if (hero.state.current != 0x29 && hero.state.current != 0x42 &&
            hero.state.current != 0x2A) {
            AN.f60 = fast_arc_tan(vector_length_xy(&hero.motion.displacement),
                                  hero.motion.displacement.f[2]);
        }
        if (has) {
            FUN_L14_0021c5f8(frame_scale_sq * 0.017f, frame_scale_sq * 0.3f);
            FUN_L14_0021c5c8(frame_scale_sq * 0.027f, frame_scale_sq * 0.3f);
            FUN_L14_0021c5e0(frame_scale_sq * 0.027f, frame_scale_sq * 0.3f);
            if (MV(hero.grind_look_moby) != NULL) {
                f32 a;
                f32 k;
                fast_vec_sub(&vector_and_hit_descriptor_scratch,
                             &MV(hero.grind_look_moby)->u10.v10,
                             &hero.motion.pos);
                transform_vector_by_basis(&vector_and_hit_descriptor_scratch,
                                          &vector_and_hit_descriptor_scratch,
                                          (Vec4 *)hero.inv_rot_matrix);
                a = fast_arc_tan(vector_and_hit_descriptor_scratch.f[0],
                                 vector_and_hit_descriptor_scratch.f[1]);
                if (MV(hero.moby)->b52 == 0x31) {
                    a = fast_subtract_rotations(a, 1.5707964f);
                    AN.f278 = a * 0.3f;
                    AN.f1C8 = a * 0.2f;
                } else {
                    a = fast_subtract_rotations(a, -1.5707964f);
                    AN.f278 = a * 0.3f;
                    AN.f1C8 = a * 0.5f;
                }
            } else if (MV(hero.moby)->b53 == 0x31) {
                AN.f278 = fast_add_rotations(-0.82030475f, hero.rail_lean);
                AN.f118 = fast_add_rotations(0.7330383f, hero.rail_lean / 1.7f);
                AN.f1C8 = fast_add_rotations(0.7330383f, hero.rail_lean / 1.7f);
            } else if (MV(hero.moby)->b53 == 0x4B) {
                AN.f278 = fast_add_rotations(0.13962634f, hero.rail_lean);
                AN.f118 = fast_add_rotations(-0.06981317f, hero.rail_lean * 0.5f);
                AN.f1C8 = fast_add_rotations(-0.06981317f, hero.rail_lean * 0.5f);
            } else if (MV(hero.moby)->b53 == 0x4A) {
                AN.f278 = fast_add_rotations(-0.82030475f, hero.rail_lean);
                AN.f118 = fast_add_rotations(0.7330383f, hero.rail_lean / 1.7f);
                AN.f1C8 = fast_add_rotations(0.7330383f, hero.rail_lean / 1.7f);
            }
        }
        path_heading_point_index = offset_path_index((void *)hero.rail_spline,
                                                     hero.rail_segment,
                                                     hero.grind_direction,
                                                     hero.rail_spline_loops);
        fast_vec_sub(&vector_and_hit_descriptor_scratch,
                     (Vec4 *)hero.rail_spline + (path_heading_point_index + 1),
                     &vector_and_descriptor_scratch);
        if (fast_vec_length(&vector_and_hit_descriptor_scratch) < 0.001f) {
            if (hero.rail_segment == path_heading_point_index) {
                hero.rail_off_frames = scale_game_frames(1);
                break;
            }
        } else {
            hero.rail_dir_yaw = fast_arc_tan(vector_and_hit_descriptor_scratch.f[0],
                                             vector_and_hit_descriptor_scratch.f[1]);
            hero.rail_pitch = fast_arc_tan(vector_length_xy(&vector_and_hit_descriptor_scratch),
                                           vector_and_hit_descriptor_scratch.f[2]);
        }
        hero.motion.move_yaw = fast_add_rotations(hero.rail_dir_yaw, hero.grind_yaw_offset);
        turn_to(0, frame_scale_sq * 0.015f, frame_scale_sq * 0.15f, frame_time * 6.2831855f);
        if (((hero.state.current == 0x29 || hero.state.current == 0x2A) &&
             hero.rail_land_frames == 0) ||
            hero.state.current == 0x42) {
            if (hero.rail_gap_jump != 0) {
                f32 a = fast_arc_tan(hero.jump_target_pos.f[0] - hero.motion.pos.f[0],
                                     hero.jump_target_pos.f[1] - hero.motion.pos.f[1]);
                if (fast_difference_between_rotations(hero.motion.rot.f[2], a) < 1.5707964f) {
                    hero.motion.move_yaw = a;
                    turn_to(
                        0, frame_scale_sq * 0.04f, frame_scale_sq * 0.2f, frame_time * 6.2831855f);
                }
            }
            if (hero.rail_switching != 0 && hero.rail_transfer_frames <= scale_game_frames(48)) {
                f32 previous_path_blend_fraction;
                fast_vec_sub(
                    &vector_and_hit_descriptor_scratch, &vector_and_descriptor_scratch, &v10);
                hero.rail_transfer_frames++;
                fast_vec_scale(&vector_and_hit_descriptor_scratch,
                               &vector_and_hit_descriptor_scratch,
                               (f32)hero.rail_transfer_frames / (f32)scale_game_frames(48));
                previous_path_blend_fraction =
                    (f32)(hero.rail_transfer_frames - 1) / (f32)scale_game_frames(48);
                fast_vec_sub(&motion_vector_scratch, &vector_and_descriptor_scratch, &v10);
                fast_vec_scale(
                    &motion_vector_scratch, &motion_vector_scratch, previous_path_blend_fraction);
                hero.motion.pos.f[2] +=
                    vector_and_hit_descriptor_scratch.f[2] - motion_vector_scratch.f[2];
                fast_vec_add(&motion_vector_scratch, &v10, &vector_and_hit_descriptor_scratch);
                fast_vec_sub(
                    &vector_and_hit_descriptor_scratch, &motion_vector_scratch, &hero.motion.pos);
                hero.motion.velocity.f[0] = vector_and_hit_descriptor_scratch.f[0];
                hero.motion.velocity.f[1] = vector_and_hit_descriptor_scratch.f[1];
                fast_vec_sub(&vector_and_hit_descriptor_scratch,
                             &vector_and_descriptor_scratch,
                             &hero.rail_point);
                hero.motion.pos.f[2] += vector_and_hit_descriptor_scratch.f[2];
                if (hero.motion.pos.f[2] < hero.rail_point.f[2]) {
                    if (hero.rail_transfer_frames <= scale_game_frames(48) &&
                        hero.motion.pos.f[2] < hero.rail_point.f[2]) {
                        hero.motion.pos.f[2] = hero.rail_point.f[2];
                    }
                }
            } else {
                fast_vec_sub(&vector_and_hit_descriptor_scratch,
                             &vector_and_descriptor_scratch,
                             &hero.rail_point);
                hero.motion.velocity.f[0] = vector_and_hit_descriptor_scratch.f[0];
                hero.motion.velocity.f[1] = vector_and_hit_descriptor_scratch.f[1];
                hero.motion.pos.f[2] += vector_and_hit_descriptor_scratch.f[2];
                fast_vec_sub(&motion_vector_scratch, &hero.rail_point, &hero.motion.pos);
                motion_vector_scratch.f[2] = 0.0f;
                if (fast_vec_length(&motion_vector_scratch) > frame_time) {
                    fast_vec_normalize(&motion_vector_scratch, &motion_vector_scratch, frame_time);
                }
                hero.motion.pos.f[0] += motion_vector_scratch.f[0];
                hero.motion.pos.f[1] += motion_vector_scratch.f[1];
            }
            if (hero.state.current == 0x42) {
                hero.motion.velocity.f[2] = hero.motion.velocity.f[2] - frame_time_sq * 23.0f;
            } else {
                hero_integrate_velocity();
            }
        } else {
            f32 base_grind_speed_target = frame_time * 12.0f;
            f32 tt;
            f32 e;
            f32 sp;
            s32 si;
            f32 sf;
            hero.rail_switching = 0;
            if (hero.stick_magnitude > 0.5f && !flag) {
                f32 a;
                stick_target(1.0f, 0);
                a = fast_difference_between_rotations(hero.motion.move_yaw, l14_camera.yaw);
                if (fast_difference_between_rotations(a, 1.5707964f) < 1.2217305f ||
                    fast_difference_between_rotations(a, -1.5707964f) < 1.2217305f) {
                    hero.grind_pose =
                        fast_difference_between_rotations(
                            fast_subtract_rotations(hero.motion.move_yaw, l14_camera.yaw),
                            1.5707964f) < 0.7853982f
                            ? 2
                            : 3;
                }
            }
            tt = 0.0f;
            if (hero.stick_magnitude > 0.2f && flag) {
                stick_target(1.0f, 0);
                tt = fast_subtract_rotations(hero.motion.move_yaw, hero.rail_dir_yaw);
                if (tt > 0.7853982f) {
                    tt = 0.7853982f;
                } else if (tt < -0.7853982f) {
                    tt = -0.7853982f;
                }
            }
            spring_angle(&hero.rail_lean,
                         tt,
                         &hero.grind_lean_vel,
                         0,
                         frame_scale_sq * 0.015f,
                         frame_scale_sq * 0.3f,
                         0.0f);
            approach_value(&hero.grind_base_speed, base_grind_speed_target, frame_time_sq * 15.0f);
            si = hero.rail_segment;
            sf = hero.rail_seg_frac;
            qcopy(&vector_and_hit_descriptor_scratch, &hero.motion.pos);
            sample_path((void *)hero.rail_spline,
                        &vector_and_hit_descriptor_scratch,
                        &si,
                        &sf,
                        0.3f,
                        hero.rail_spline_loops);
            e = fast_arc_tan(
                vector_distance_xy(&hero.rail_point, &vector_and_hit_descriptor_scratch),
                vector_and_hit_descriptor_scratch.f[2] - hero.rail_point.f[2]);
            if (e > 0.0f) {
                sp = e * 1.9098593f * (frame_time * -2.5f);
                if (e > 0.2617994f && hero.grind_pose != 2 && hero.grind_pose != 3) {
                    hero.grind_pose = 0;
                } else if (e < 0.13962634f && hero.grind_pose == 0) {
                    hero.grind_pose = -1;
                }
            } else {
                sp = -e * 2.864789f * (frame_time * 8.0f);
                if (e < -0.19198622f && hero.grind_pose != 2 && hero.grind_pose != 3) {
                    hero.grind_pose = 1;
                } else if (e > -0.12217305f && hero.grind_pose == 1) {
                    hero.grind_pose = -1;
                }
            }
            if (hero.unk5AC.f < sp) {
                approach_value(&hero.unk5AC.f, sp, frame_time_sq * 8.0f);
            } else {
                approach_value(&hero.unk5AC.f, sp, frame_time_sq * 2.0f);
            }
            if (hero.unk5AC.f > frame_time * 12.0f) {
                hero.unk5AC.f = frame_time * 12.0f;
            }
            if (hero.unk5AC.f < D_0015ED6C_a * -2.5f) {
                hero.unk5AC.f = D_0015ED6C_a * -2.5f;
            }
            hero.unk574 = hero.grind_base_speed + hero.unk5AC.f;
            if (hero.unk574 < 0.0f) {
                hero.unk574 = 0.0f;
            }
            fast_vec_sub(&hero.motion.velocity, &vector_and_descriptor_scratch, &hero.rail_point);
            fast_vec_sub(&vector_and_hit_descriptor_scratch, &hero.rail_point, &hero.motion.pos);
            {
                f32 mx = frame_time * 7.0f;
                f32 fl;
                f32 len;
                fl = -fast_vec_length(&vector_and_hit_descriptor_scratch);
                vendor_spring(&fl,
                              &hero.rail_snap_dist,
                              0.0f,
                              frame_scale_sq * 0.03f,
                              frame_scale_sq * 0.3f,
                              mx);
                len = fast_vec_length(&vector_and_hit_descriptor_scratch);
                if (len < hero.rail_snap_dist) {
                    hero.rail_snap_dist = len;
                }
                if (hero.rail_snap_dist < len) {
                    fast_vec_normalize(&vector_and_hit_descriptor_scratch,
                                       &vector_and_hit_descriptor_scratch,
                                       hero.rail_snap_dist);
                }
                fast_vec_add(
                    &hero.motion.pos, &hero.motion.pos, &vector_and_hit_descriptor_scratch);
            }
            if (hero.motion.pos.f[2] < hero.rail_point.f[2] && hero.ground_distance < 1.0f &&
                hero.motion.pos.f[2] < hero.rail_point.f[2]) {
                hero.motion.pos.f[2] = hero.rail_point.f[2];
            }
        }
        if (hero.state.current == 0x2B && hero.state_timer < scale_game_frames(20)) {
            f32 a = fast_add_rotations(hero.rail_dir_yaw, randf_sym(0.34906584f, 0.61086524f));
            matrix_and_vector_scratch[1].f[0] = fast_cos(a);
            matrix_and_vector_scratch[1].f[1] = fast_sin(a);
            matrix_and_vector_scratch[1].f[2] = 0.0f;
            make_hit_descriptor(&vector_and_hit_descriptor_scratch,
                                MV(hero.moby),
                                0x10000,
                                1.0f,
                                &matrix_and_vector_scratch[1]);
            qcopy(&matrix_and_vector_scratch[2], &hero.motion.pos);
            matrix_and_vector_scratch[2].f[2] += 0.6f;
            collect_mobies_in_sphere(1.0f,
                                     &matrix_and_vector_scratch[2],
                                     0x10,
                                     MV(hero.moby),
                                     &vector_and_hit_descriptor_scratch);
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
        qcopy(&vector_and_hit_descriptor_scratch, &hero.motion.pos);
        if (hero.state.current == 0x1A) {
            matrix_and_vector_scratch[0].f[0] =
                fast_cos(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
            matrix_and_vector_scratch[0].f[1] =
                fast_sin(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
            matrix_and_vector_scratch[0].f[2] = 0.0f;
        } else {
            matrix_and_vector_scratch[0].f[0] =
                fast_cos(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
            matrix_and_vector_scratch[0].f[1] =
                fast_sin(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
            matrix_and_vector_scratch[0].f[2] = 0.0f;
        }
        fast_vec_add(&motion_vector_scratch,
                     &vector_and_hit_descriptor_scratch,
                     &matrix_and_vector_scratch[0]);
        update_hero_ledge_probe();
        if (hero.ledge_found == 0) {
            break;
        }
        qcopy(&vector_and_descriptor_scratch, &hero.ledge_hang_pos);
        a = fast_add_rotations(hero.ledge_normal_yaw, 3.1415927f);
        qcopy(&hero.motion.pos, &motion_vector_scratch);
        update_hero_ledge_probe();
        qcopy(&hero.motion.pos, &vector_and_hit_descriptor_scratch);
        if (hero.ledge_found == 0) {
            break;
        }
        qcopy(&v10, &hero.ledge_hang_pos);
        b = fast_add_rotations(hero.ledge_normal_yaw, 3.1415927f);
        if (fast_difference_between_rotations(a, b) > 0.5235988f) {
            break;
        }
        a = fast_add_rotations(fast_subtract_rotations(a, b) * 0.5f, a);
        fast_vec_add(&matrix_and_vector_scratch[1], &vector_and_descriptor_scratch, &v10);
        fast_vec_scale(&matrix_and_vector_scratch[1], &matrix_and_vector_scratch[1], 0.5f);
        hero.motion.move_yaw = a;
        turn_to(0, frame_scale_sq * 0.04f, frame_scale_sq * 0.2f, frame_time * 6.2831855f);
        fl = hero.motion.flat_speed;
        approach_value(&fl, hero.target_speed * frame_time, frame_time_sq * 5.0f);
        fast_vec_sub(&hero.motion.velocity, &matrix_and_vector_scratch[1], &hero.motion.pos);
        set_vector_length_xy(
            &hero.motion.velocity,
            &hero.motion.velocity,
            l14_ledge_shimmy_speed_by_animation_frame[truncate_float_to_s32(hero.anim_frame)] *
                0.5f);
        break;
    }
    case HERO_STATE_SLIP:
        hero.motion.move_yaw = hero.downhill_yaw;
        hero.target_speed = frame_time * 5.5f;
        steer_hero_planar_velocity(frame_time_sq * 25.0f);
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
            qcopy(&vector_and_descriptor_scratch, &hero.motion.rot);
            spring_angle(&vector_and_descriptor_scratch.f[1],
                         t,
                         &hero.motion.rot_spring_vel.f[1],
                         0,
                         frame_scale_sq * 0.02f,
                         frame_scale_sq * 0.3f,
                         frame_time * 2.6179938f);
            spring_angle(&vector_and_descriptor_scratch.f[0],
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
        update_hero_stick_target_magnitude();
        if (hero.move_lock_timer == 0) {
            hero_update_hop_movement();
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
            hero.motion.move_yaw = *(f32 *)D_L14_001674D8;
            turn_to(0, frame_scale_sq * 0.017f, frame_scale_sq * 0.3f, frame_time * 4.712389f);
            hero.motion.move_yaw = o;
        }
        {
            f32 g = frame_time_sq * 7.0f;
            if (hero.stick_magnitude < 0.2f) {
                g = frame_time_sq * 3.5f;
            }
            if (hero.on_ice_ground != 0) {
                g = frame_time_sq * 3.7f;
                if (hero.stick_magnitude < 0.2f) {
                    g = frame_time_sq * 1.2f;
                }
            }
            steer_hero_planar_velocity(g);
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
        s32 state_timer_or_trail_value;
        s32 state_timer_threshold;
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
        state_timer_or_trail_value = hero.state_timer;
        state_timer_threshold = hero.takeoff_frame;
        if (state_timer_or_trail_value == state_timer_threshold) {
            qcopy(&hero.takeoff_pos, &hero.motion.pos);
        }
        if (hero.rail_land_frames != 0 ||
            (state_timer_or_trail_value < state_timer_threshold &&
             hero.state.current != HERO_STATE_HIGH_JUMP &&
             hero.state.current != HERO_STATE_PACK_LONG_JUMP && hero.state.current != 0xC &&
             hero.state.current != 0x11 && hero.state.current != 0x1C &&
             hero.state.current != 0x4C)) {
            if (hero.unk4A8 != 0) {
                update_hero_planar_movement_velocity();
                if (hero.state.current == HERO_STATE_PACK_JUMP &&
                    hero.state_timer < scale_game_frames(10) &&
                    hero.motion.forward_speed < frame_time * 7.0f) {
                    hero.motion.velocity.f[0] +=
                        fast_cos(hero.motion.rot.f[2]) * (frame_time * 0.7f);
                    hero.motion.velocity.f[1] +=
                        fast_sin(hero.motion.rot.f[2]) * (frame_time * 0.7f);
                    qcopy(&hero.motion.resolved_velocity_xy, &hero.motion.velocity);
                }
            } else {
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
                    qcopy(&vector_and_descriptor_scratch, &hero.motion.pos);
                    FUN_L00_00233810(
                        &vector_and_descriptor_scratch, &vector_and_descriptor_scratch, 0.5f);
                    qcopy(&v10, &vector_and_descriptor_scratch);
                    v10.f[0] += fast_cos(hero.motion.rot.f[2]) * 0.85f;
                    v10.f[1] += fast_sin(hero.motion.rot.f[2]) * 0.85f;
                    if (collision_line(&vector_and_descriptor_scratch, &v10, 4, MV(hero.moby), 0) !=
                        0) {
                        hero.jump_blocked_ahead = 1;
                    }
                }
            }
        } else {
            hero_update_hop_movement();
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
            state_timer_or_trail_value = (scale_game_frames(12) < hero.state_timer) * 2;
            update_moby_transform_history(&hero.trail.pos[0], state_timer_or_trail_value);
        }
        if (hero.state.current == HERO_STATE_PACK_LONG_JUMP &&
            scale_game_frames(8) < hero.state_timer && hero.anim_frame < 28.0f) {
            s32 nearby_moby_count;
            transform_hero_local_offset_to_world(&vector_and_descriptor_scratch, 0.8f, 0.0f, 0.3f);
            nearby_moby_count = collect_mobies_in_sphere(
                0.9f, &vector_and_descriptor_scratch, 0, MV(hero.moby), NULL);
            if (nearby_moby_count != 0) {
                s32 i;
                fast_vec_scale(&v10, &hero.motion.displacement, 7.0f);
                for (i = 0; i < nearby_moby_count; i++) {
                    if (is_moby_in_class_range_500_to_540(l14_sphere_query_moby_results[i]) != 0) {
                        spawn_nearby_effect(l14_sphere_query_moby_results[i],
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
        fast_vec_sub(&vector_and_descriptor_scratch, &hero.target_pose, &hero.motion.pos);
        hero.motion.move_yaw =
            fast_arc_tan(vector_and_descriptor_scratch.f[0], vector_and_descriptor_scratch.f[1]);
        l = vector_length_xy(&vector_and_descriptor_scratch);
        if (l > 0.2f) {
            hero.stick_magnitude = 1.0f;
            hero.motion.move_yaw = fast_arc_tan(vector_and_descriptor_scratch.f[0],
                                                vector_and_descriptor_scratch.f[1]);
            hero.target_speed = l;
            if (l < D_L14_0017C4B8.x0C) {
                hero.target_speed = D_L14_0017C4B8.x08 * frame_time;
            } else {
                hero.target_speed = D_L14_0017C4B8.x18 * frame_time;
            }
            if (hero.vehicle_mode == 3) {
                hero.target_speed = D_0015ED6C_a * 1.5f;
            } else if (hero.vehicle_mode == 1) {
                hero.target_speed = D_0015ED6C_a * 0.9f;
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
        fast_vec_sub(&vector_and_descriptor_scratch, &hero.target_pose, &hero.motion.pos);
        if (fast_vec_dot(&vector_and_descriptor_scratch, &hero.motion.momentum) < 0.0f) {
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
        steer_hero_planar_velocity(frame_time_sq * 25.0f);
        FUN_L00_002334d0(
            &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 18.0f);
        break;
    case HERO_STATE_VEHICLE_MODE_3_FALL:
        stick_target(frame_time * 1.5f, 0);
        if (hero.move_lock_timer == 0) {
            hero_update_hop_movement();
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
        f32 planar_speed_before_damping = len;
        if (hero.air_frames.s != 0) {
            len = frame_scale * -0.00999999f * planar_speed_before_damping +
                  planar_speed_before_damping;
        } else {
            len = frame_scale * -0.07999998f * planar_speed_before_damping +
                  planar_speed_before_damping;
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

f32 D_L14_0017AFF0[1] __attribute__((section(".data"))) = {0};
