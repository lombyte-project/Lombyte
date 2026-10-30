/* Level 15 hero motion: update_hero_motion_for_current_state, with the ground type effect and incoming hit helpers. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/game/usage_stats.h"
#include "rnc/overlay/hero_tables.h"
#include "rnc/data_labels.h"
#include "rnc/input/pad_globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/overlay/shared/hero/adjust_hero_pose_targets_from_motion.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/mobys/moby.h"

/* Same source as the exact FUN_L16_002097a0, with a state 3 flag (0x12E6) and its own landing checks. */
extern int  D_0015ED84_d __asm__("D_0015ED84");
extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");
extern int  hero_set_state(int, int) __asm__("FUN_L15_00216c38");
float       absolute_float(float input) __asm__("func_001F99C0");

void apply_hero_ground_type_effects(void) __asm__("FUN_L15_002044c8");

void apply_hero_ground_type_effects(void) {
    struct Hero *hero_state = &hero;
    int          keep = hero_state->ground_sound_class;
    int          saved_ground_type = hero_state->ground_type;
    int          collision_check_level_index;
    fast_mem_zero16(&hero_state->ground_type, 0x10);
    hero_state->ground_sound_class = keep;
    hero_state->ground_type = -1;
    hero_state->wading = 0;
    hero_state->unk308 = 0;
    if (saved_ground_type == -1)
        return;
    if (saved_ground_type == 2) {
        if (hero_state->frames_off_ground.s == 0 || hero_state->ground_distance < 0.3f) {
            hero_state->on_magnet_ground = 1;
            if (hero_state->vehicle_mode == 0) {
                char *item_slot_one_moby = ((char *)hero_state->items[1].moby);
                if (item_slot_one_moby != 0 && MOBY(item_slot_one_moby)->oclass == 0xAD)
                    hero_state->unk308 = 1;
            }
        }
    }
    if (saved_ground_type == 0xE) {
        struct Hero *q = &hero;
        *(char *)&q->base_condition = 1;
        q->water_depth = 0.2f;
        q->water_surface_z = q->ground_z.f + 0.2f;
    }
    if (saved_ground_type == 0) {
        struct Hero *q = &hero;
        float        water_depth = q->water_surface_z - q->ground_z.f;
        q->water_depth = water_depth;
        if (water_depth < 0.85f && 0.25f < water_depth)
            *(char *)&q->wading = 1;
        {
            struct Hero *flag = &hero;
            *(char *)&flag->base_condition = 1;
        }
        collision_check_level_index = current_level_index;
    } else {
        collision_check_level_index = current_level_index;
    }
    if (saved_ground_type == 3) {
        struct Hero *q = &hero;
        *(char *)&q->selector_3 = 1;
        collision_check_level_index = D_0015ED84_d;
    }
    if (collision_check_level_index == 0xD) {
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
        *(char *)&q->selector_11 = 1;
    }
    if (saved_ground_type == 0x4) {
        struct Hero *q = &hero;
        *(char *)&q->selector_4 = 1;
    }
    if (saved_ground_type == 0x8) {
        struct Hero *q = &hero;
        *(char *)&q->on_slip_ground = 1;
    }
    if (saved_ground_type == 0x9) {
        struct Hero *q = &hero;
        *(char *)&q->unk12EE = 1;
    }
    if (saved_ground_type == 0xC) {
        struct Hero *q = &hero;
        *(char *)&q->on_slip_ground = 1;
    }
    {
        struct Hero *q = &hero;
        struct Hero *hero_ptr;
        if (q->selector_4 != 0 && q->grounded_frames != 0) {
            int k = q->state.control_mode;
            if (k != 0x10 && k != 0x14 && k != 7) {
                hero_set_state(0x31, 1);
                return;
            }
        }
        hero_ptr = &hero;
        if (hero_ptr->selector_3 != 0 && hero_ptr->state.current != 0x68 &&
            hero_ptr->state.current != 0x7B) {
            if (absolute_float(hero_ptr->sink_surface_z - (hero_ptr->motion.pos.f[2] + 0.25f)) <
                1.0f) {
                if (hero_ptr->sink_surface_z - hero_ptr->motion.pos.f[2] > 0.0f) {
                    if (hero_ptr->state.current != 0x69 || hero_ptr->past_jump_apex != 0) {
                        if (hero_ptr->motion.displacement.f[2] < 0.0f) {
                            exit_hero_vehicle_mode_if_mode_three();
                            if (hero_ptr->health.hp == 0)
                                hero_set_state(0x7B, 1);
                            else
                                hero_set_state(0x68, 1);
                            return;
                        }
                    }
                }
            }
        }
        hero_ptr = &hero;
        if (hero_ptr->selector_11 != 0 && hero_ptr->state.current != 0x7B) {
            if (absolute_float(hero_ptr->sink_surface_z - (hero_ptr->motion.pos.f[2] + 0.25f)) <
                1.0f) {
                if (hero_ptr->sink_surface_z - hero_ptr->motion.pos.f[2] > 0.0f) {
                    if (hero_ptr->motion.displacement.f[2] < 0.0f) {
                        exit_hero_vehicle_mode_if_mode_three();
                        hero_set_state(0x7B, 1);
                    }
                }
            }
        }
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

typedef struct {
    u8      pad0[0x10];
    OvlQuad v10;
    u8     *p20;
    s32     i24;
    u8      b28;
    u8      pad29[3];
    f32     f2C;
    s32     i30;
    u8     *p34;
    u8      pad38[8];
} T_210ce8;

typedef union {
    OvlQuad q;
    f32     f[4];
    s32     i[4];
} V_210ce8;

extern s32      D_L15_0015F5C4_210ce8 __asm__("D_L15_0015F5C4");
extern T_210ce8 D_L15_00178700_210ce8[] __asm__("D_L15_00178700");

void release_and_clear_hero_voice_slots(void) __asm__("FUN_L00_00206c08");
void FUN_001f9a28_210ce8(void *, void *, void *) __asm__("FUN_001f9a28");
f32  FUN_001f9dc8_210ce8(f32) __asm__("FUN_001f9dc8");
f32  FUN_001f9de0_210ce8(f32) __asm__("FUN_001f9de0");
void FUN_001f9a10_210ce8(void *, void *, void *) __asm__("FUN_001f9a10");
void allocate_hero_voice_for_entry(s32, s32) __asm__("FUN_L00_00216de8");
void remove_giant_clank_charged_blast(void) __asm__("FUN_L00_002a9be0");
void hero_knockback(void *, s32, f32, f32) __asm__("FUN_L00_00210c80");

s32 process_hero_incoming_hit(s32 apply_hit_reaction) __asm__("FUN_L15_00208e10");

s32 process_hero_incoming_hit(s32 apply_hit_reaction) {
    u8       *hero_moby;
    T_210ce8 *incoming_hit_record;
    u8       *mob;
    s32       flag;
    s32       id;
    f32       horizontal_knockback_scale, vertical_knockback_scale, knockback_scale_factor;
    V_210ce8  knockback_vector;

    if (hero.state.control_mode == 0x14) {
        return 0;
    }
    if (hero.state.control_mode == 7) {
        return 0;
    }
    if (hero.state.current == 0x32) {
        return 0;
    }
    if (hero.invulnerable_frames != 0) {
        return 0;
    }
    if (D_L15_0015F5C4_210ce8 != 0) {
        return 0;
    }
    hero.damage_source_moby = 0;
    hero_moby = ((u8 *)hero.moby);
    if (hero_moby[0xA4] == 0xFF) {
        return 0;
    }
    incoming_hit_record = &D_L15_00178700_210ce8[hero_moby[0xA4]];
    if (incoming_hit_record->p34 != hero_moby) {
        return 0;
    }
    if (hero.vehicle_mode == 2) {
        if (!(incoming_hit_record->i24 & 2)) {
            return 0;
        }
    } else if ((incoming_hit_record->i24 ^ 1) & 1) {
        return 0;
    }
    D_0013DF88[current_level_index]++;
    hero.damage_source_moby = (struct Moby *)incoming_hit_record->p20;
    D_0015EEA8++;
    if (hero.vehicle_mode == 2) {
        hero.vehicle_hp -= truncate_float_to_s32(incoming_hit_record->f2C);
        if (hero.vehicle_hp < 0) {
            hero.vehicle_hp = 0;
        }
    }
    if (apply_hit_reaction == 0) {
        return 1;
    }
    release_and_clear_hero_voice_slots();
    flag = 0;
    if (incoming_hit_record->i30 & 1) {
        qcopy(&knockback_vector, &incoming_hit_record->v10);
        if (*(f32 *)((u8 *)incoming_hit_record + 0x1C) == 5627.9248046875f) {
            flag = 1;
        }
    } else if (incoming_hit_record->p20 != 0) {
        FUN_001f9a28_210ce8(&knockback_vector, &hero.motion.pos, incoming_hit_record->p20 + 0x10);
    } else {
        knockback_vector.f[0] =
            FUN_001f9dc8_210ce8(fast_add_rotations(hero.motion.rot.f[2], 3.1415927f));
        knockback_vector.f[1] =
            FUN_001f9de0_210ce8(fast_add_rotations(hero.motion.rot.f[2], 3.1415927f));
        knockback_vector.i[2] = 0;
    }
    switch (hero.vehicle_mode) {
    case HERO_VEHICLE_MODE_ON_FOOT:
        mob = ((u8 *)hero.damage_source_moby);
        if (mob != 0 && (*(s16 *)(mob + 0xA6) == 0x4EB || *(s16 *)(mob + 0xA6) == 0x558)) {
            id = 0x80;
        call_ret:
            hero_set_state(id, 1);
            return 1;
        }
        if (hero.state.control_mode == 0x16) {
            hero_set_state(0x6D, 1);
            hero.motion.resolved_velocity_z.f[2] = frame_time * 7.0f;
            return 1;
        }
        if (hero.state.control_mode == 0x12) {
            mob = ((u8 *)hero.damage_source_moby);
            if (mob != 0 && *(s16 *)(mob + 0xA6) == 0x28F) {
                id = 0x82;
                goto call_ret;
            }
            hero_set_state(0x75, 1);
        } else if (hero.state.control_mode == 0x11) {
            mob = ((u8 *)hero.damage_source_moby);
            if (mob != 0 && *(s16 *)(mob + 0xA6) == 0x28F) {
                id = 0x82;
                goto call_ret;
            }
            if (current_level_index == 0xF || current_level_index == 0x11) {
                mob = ((u8 *)hero.damage_source_moby);
                if (mob != 0 && (*(s16 *)(mob + 0xA6) == 0x28F || *(s16 *)(mob + 0xA6) == 0x7B ||
                                 *(s16 *)(mob + 0xA6) == 0x29D)) {
                    allocate_hero_voice_for_entry(0x1C, 0);
                }
            }
            hero_set_state(0x76, 1);
        } else if (hero.state.control_mode == 3) {
            mob = ((u8 *)hero.damage_source_moby);
            if (mob != 0) {
                id = 0x82;
                if (*(s16 *)(mob + 0xA6) == 0x28F) {
                    goto call_ret;
                }
            }
            hero_set_state(HERO_STATE_HURT, 1);
        } else {
            hero_set_state(HERO_STATE_HURT, 1);
        }
        knockback_scale_factor = frame_time;
        horizontal_knockback_scale = knockback_scale_factor * 5.7f;
        vertical_knockback_scale = knockback_scale_factor * 2.4f;
        if (hero.on_magnet_ground != 0) {
            horizontal_knockback_scale = 0.0f;
            vertical_knockback_scale = knockback_scale_factor * 1.7f;
        }
        hero_knockback(
            &knockback_vector, flag, horizontal_knockback_scale, vertical_knockback_scale);
        if (flag != 0 && incoming_hit_record->b28 == 4) {
            knockback_vector.f[2] += knockback_vector.f[2];
        }
        break;
    case HERO_VEHICLE_MODE_GIANT_CLANK:
        if ((incoming_hit_record->i24 & 4) || hero.vehicle_hp <= 0) {
            hero_set_state(0x5D, 1);
            remove_giant_clank_charged_blast();
            if (hero.air_frames.s == 0) {
                hero_knockback(&knockback_vector, flag, frame_time * 7.0f, frame_time * 3.5f);
                break;
            }
        }
        clear_u64_value(&knockback_vector);
        break;
    case 3:
        hero_set_state(0x56, 1);
        hero_knockback(&knockback_vector, flag, frame_time * 5.0f, frame_time * 2.4f);
        break;
    }
    FUN_001f9a10_210ce8(&hero.motion.velocity, &hero.motion.velocity, &knockback_vector);
    FUN_001f9a10_210ce8(&hero.motion.displacement, &hero.motion.displacement, &knockback_vector);
    return 1;
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

extern s32 D_L15_0015F688 __attribute__((sda));
s32        D_L15_0015F68C = 0;
extern u8  D_L15_001673D8[];

extern void emit_hero_periodic_surface_effect(s32, s32) __asm__("FUN_L00_00209540");
extern void build_scaled_hero_move_impulse(f32, f32) __asm__("FUN_L00_002132b8");
extern void stop_hero_motion_without_forward_support(f32, f32) __asm__("FUN_L00_00217118");
extern void stick_target(f32, s32) __asm__("FUN_L00_002118c8");
extern void turn_to(s32, f32, f32, f32) __asm__("FUN_L00_00211e30");
extern s32  try_start_hero_item_idle_animation(void) __asm__("FUN_L00_00229a98");
extern void turn_hero_toward_move_direction_for_item(void) __asm__("FUN_L00_00211e98");
extern void set_hero_animation(s32, s32, f32) __asm__("FUN_L00_002323b8");
extern s32  get_hero_help_value(s32) __asm__("FUN_L06_0021d0a0");
extern void
    adjust_hero_animation_speed_for_target_frame(f32, f32, f32, f32) __asm__("FUN_L00_00208f20");
extern void speed_step(f32, f32) __asm__("FUN_L00_00212088");
extern void reduce_and_apply_hero_planar_velocity_addend(f32) __asm__("FUN_L00_00216710");
extern void FUN_L00_002334d0(void *, void *, f32);
extern void adjust_hero_planar_velocity_from_probes(s32) __asm__("FUN_L15_0020a328");
extern void correct_hero_airborne_wall_velocity(void) __asm__("FUN_L00_002121c0");
extern void spawn_hero_radial_particle_burst(void) __asm__("FUN_L00_00217450");
extern s32  FUN_L00_001ff600(f32, void *, void *);
extern void update_hero_water_height_and_bobbing(void) __asm__("FUN_L00_00227d40");
extern struct PadState D_0013C940;
#define PD D_0013C940
extern struct PlayerJointTable D_L15_0017AC80;
#define AN D_L15_0017AC80
struct VoiceSlot {
    u8               pad0[0x74];
    u8               b74;
    u8               pad75[0x13];
    struct MobyView *pm88;
};

extern s32 D_L15_001612D0;
extern s32 D_L15_001612D8;
extern s32 D_L15_0015F5CC;
extern u8  hero_swim_speed_by_animation_frame[] __asm__("D_L15_0017C5C0");

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
extern void FUN_L15_00202d78(f32, f32);
extern void FUN_L15_00202d60(f32, f32);
extern void FUN_L15_00202d90(f32, f32);
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
extern struct HeroTableRow2C D_L15_0017C228[];
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

extern struct PlayerSpeedTuning D_L15_0017C3B8;
extern void add_hero_forward_offset(void *, void *, f32) __asm__("FUN_L00_00233708");
extern void FUN_L00_00233810(void *, void *, f32);
extern s32  collision_line(void *, void *, s32, struct MobyView *, s32) __asm__("FUN_001efa68");
extern void transform_hero_local_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_00233660");
extern s32  is_moby_in_class_range_500_to_540(s32) __asm__("FUN_L00_0025e3b8");
extern void
spawn_nearby_effect(s32, struct MobyView *, s32, void *, void *, f32) __asm__("FUN_L00_00259bc8");
extern s32  D_L15_00178500[];
extern void emit_hero_timed_effects(void) __asm__("FUN_L00_00209ec8");
extern void gravity_flatten(void *, void *) __asm__("FUN_L00_002338d0");
extern s32  hero_slot_item_id(s32) __asm__("FUN_L00_0020d498");
extern void probe_hero_airborne_wall_contact(void) __asm__("FUN_L00_0020b5b8");
extern void update_hero_ledge_probe(void) __asm__("FUN_L01_0022d090");
extern f32  level15_hero_ledge_shimmy_speed_by_animation_frame[] __asm__("D_L15_001C42B0");
extern void air_accel(f32) __asm__("FUN_L01_00234358");
extern void rotate_hero_about_fixed_local_pivot(f32, f32, f32) __asm__("FUN_L00_002091d8");
extern void hero_update_hop_velocity(void) __asm__("FUN_L02_00223450");
extern void compute_hero_fixed_offset_world_position(void *) __asm__("FUN_L00_0020d2f8");
extern f32  randf_sym(f32, f32) __asm__("FUN_L00_00257c48");
extern s32  FUN_L00_0025df68(void *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);
extern s32  sample_path(void *, void *, s32 *, f32 *, f32, s32) __asm__("FUN_L00_0025d808");
extern s32  offset_path_index(void *, s32, s32, s32) __asm__("FUN_L00_0025d7a0");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void hero_integrate_velocity(void) __asm__("FUN_L00_00214108");
extern void spawn_random_alpha_rotated_particle(void *, void *, f32) __asm__("FUN_L00_00271b18");
extern void update_hero_lean_and_sway_parameters(void) __asm__("FUN_L00_00215ef8");
extern void turn_hero_toward_move_direction_with_stick_scale(void) __asm__("FUN_L00_00211f38");
extern f32  hero_turn_input_or_approach_distance_threshold[] __asm__("D_L15_0017C3C4");
extern void hero_speed_from_table(void) __asm__("FUN_L01_00232290");
extern void initialize_moby_transform_history(void *, void *) __asm__("FUN_L00_00262500");
extern void create_moby_transform_history_entry(void *, s32, s32) __asm__("FUN_L00_00262528");
extern void update_moby_transform_history(void *, s32) __asm__("FUN_L00_00262608");
extern void emit_filtered_entry_sparkles(void *, s32) __asm__("FUN_L00_00217368");
extern void launch_hero_wrench(void) __asm__("FUN_L00_00217658");
extern f32  set_planar_vel(f32) __asm__("FUN_L00_002120d8");
extern s32  test_hero_animation_frame_window(f32) __asm__("FUN_L00_00211870");

void update_hero_motion_for_current_state(void) __asm__("FUN_L15_0020ed50");

void update_hero_motion_for_current_state(void) {
    enum {
        HERO_ATTACK_RECORD_DUAL_POINT_HIT_QUERY = 2
    };
    enum {
        HERO_ATTACK_RECORD_POINT_3_HIT_QUERY = 0,
        HERO_ATTACK_RECORD_POINT_4_HIT_QUERY = 1
    };
    enum {
        HERO_COMBO_KIND_ANIMATION_GATED_TARGET_SPEED = 0,
        HERO_COMBO_KIND_STAGED_TARGET_SPEED_REDUCTION = 1,
        HERO_COMBO_KIND_FLAG_OR_ANIMATION_GATED_TARGET_SPEED = 2
    };

    struct MobyView *moby = (struct MobyView *)hero.moby;
    s32              sub = hero.state.step;
    s32              anim = moby->b53;
    f32              speed;
    Vec4             spatial_or_hit_descriptor_work;
    Vec4             v10;
    Vec4             spatial_or_hit_descriptor_tail_work;
    Vec4             motion_work_vector;
    Vec4             rotation_matrix_or_vector_work[4];
    Vec4             transposed_yaw_rotation_matrix[4];
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
                spatial_or_hit_descriptor_work.q = 0;
                spatial_or_hit_descriptor_work.f[0] = 1.0f;
                transform_vector_by_basis(
                    &hero.motion.move_dir, &spatial_or_hit_descriptor_work, &D_L15_001673D8[0x1F8]);
                hero.motion.move_yaw = *(f32 *)D_L15_001673D8;
                turn_to(0, frame_scale_sq, frame_scale_sq * 0.0f, frame_time * 174.53293f);
                if (D_L15_0015F688 > 0 && fast_dec_timer(&D_L15_0015F688) != 0) {
                    s32 t;
                    if (usage_stats.stat[18].count < 0xFFFF) {
                        usage_stats.stat[18].count++;
                    }
                    t = scale_game_frames(D_0015EEA4) / 600;
                    if (usage_stats.stat[18].unk2 < t) {
                        usage_stats.stat[18].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                    D_L15_0015F688 = -1;
                    usage_stats.stat[18].level_mask =
                        usage_stats.stat[18].level_mask | (1 << current_level_index) | 0x80000000;
                    D_L15_0015F68C++;
                }
            }
        }
        if (hero.state.current == HERO_STATE_STOP) {
            if (try_start_hero_item_idle_animation() != 0) {
                break;
            }
        }
        if (hero.state.current == HERO_STATE_CROUCH || hero.state.current == 0x17) {
            if (hero.stick_magnitude > 0.2f || ((struct MobyView *)hero.moby)->b53 == 0xE ||
                ((struct MobyView *)hero.moby)->b53 == 0xF) {
                if (hero.state.current == HERO_STATE_CROUCH) {
                    turn_to(
                        0, frame_scale_sq * 0.002f, frame_scale_sq * 0.07f, frame_time * 6.981317f);
                } else if (hero.state.current == 0x17) {
                    turn_hero_toward_move_direction_for_item();
                }
                if (absolute_float(hero.motion.yaw_rate) > frame_time * 0.34906584f) {
                    s32 neg = hero.motion.yaw_rate < 0.0f;
                    s32 anim = neg + 0xE;
                    if (((struct MobyView *)hero.moby)->b53 != anim) {
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
                        anim = get_hero_help_value(0);
                    }
                    if (((struct MobyView *)hero.moby)->b53 != anim) {
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
            if (((struct MobyView *)hero.moby)->b52 == ((struct MobyView *)hero.moby)->b53 &&
                ((struct MobyView *)hero.moby)->b52 != 0x14) {
                f32 r = hero.motion.flat_speed / speed;
                if (scale_game_frames(5) < r && ((struct MobyView *)hero.moby)->b51 < 5.5f) {
                    adjust_hero_animation_speed_for_target_frame(13.0f, r, 0.5f, -1.0f);
                    if (hero.anim_speed > 1.1f) {
                        hero.anim_speed = 1.1f;
                    }
                } else if (((struct MobyView *)hero.moby)->b51 > 6.5f) {
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
            adjust_hero_planar_velocity_from_probes(0);
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
    case HERO_STATE_SWIM:
    case HERO_STATE_SWIM_IDLE:
    case HERO_STATE_SWIM_WITH_CROUCH_INPUT:
        if (hero.state.current == 0x35) {
            release_voice_slot_cf(5, (struct MobyView *)hero.moby, 0x13);
        } else {
            s32 voice_slot_index = hero.pack_swim_voice;
            if (voice_slot_index != -1) {
                struct VoiceSlot *e =
                    (struct VoiceSlot *)(((u8 *)&voice_pool) + voice_slot_index * 0x70);
                if (e->pm88 == (struct MobyView *)hero.moby && e->b74 != 0) {
                    release_voice_slot(voice_slot_index);
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
                if (D_L15_001612D0 != 0) {
                    pool_splash(D_L15_001612D0,
                                D_L15_001612D8,
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
            f32          swim_pitch_smoothing_coefficient;
            f32          steer;
            f32          old;
            f32          swim_pitch_delta_or_combo_spacing_error;
            f32          k;
            f32          yaw;
            f32          t;
            struct Hero *pl;

            steer = 0.0f;
            m = PD.f128 < PD.f12C ? PD.f12C : PD.f128;
            rate = frame_scale_sq * 0.2f;
            swim_pitch_smoothing_coefficient = frame_scale_sq * 0.015f;
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
                if (D_L15_0015F5CC - hero.last_cross_press_frame < scale_game_frames(70)) {
                    hero.swim_cross_tap_count++;
                }
                hero.last_cross_press_frame = D_L15_0015F5CC;
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
                         swim_pitch_smoothing_coefficient,
                         rate,
                         lo);
            swim_pitch_delta_or_combo_spacing_error =
                fast_subtract_rotations(hero.motion.rot.f[1], old);
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
                moby_point_position(
                    (struct MobyView *)pl->moby, 4, &spatial_or_hit_descriptor_work);
                spatial_or_hit_descriptor_tail_work.q = 0;
                spatial_or_hit_descriptor_tail_work.f[0] = 0.4f;
                transform_vector_by_basis(&spatial_or_hit_descriptor_tail_work,
                                          &spatial_or_hit_descriptor_tail_work,
                                          &((struct MobyView *)pl->moby)->vC0);
                fast_vec_add(&spatial_or_hit_descriptor_work,
                             &spatial_or_hit_descriptor_work,
                             &spatial_or_hit_descriptor_tail_work);
                qcopy(&v10, &hero.motion.displacement);
                fast_vec_normalize(&spatial_or_hit_descriptor_tail_work,
                                   &spatial_or_hit_descriptor_tail_work,
                                   frame_time * 1.5f);
                fast_vec_add(&v10, &v10, &spatial_or_hit_descriptor_tail_work);
                v10.f[2] += frame_time * 2.0f;
                FUN_L00_0026f080(&spatial_or_hit_descriptor_work, &v10, 25200.0f, -1.0f);
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
                struct MobyView *gm = (struct MobyView *)hero.items[3].moby;
                if (gm != NULL && gm->hA6 == 0x261) {
                    s32 i;
                    for (i = 0; i < 4; i++) {
                        moby_point_position(gm, i % 2, &spatial_or_hit_descriptor_work);
                        spatial_or_hit_descriptor_tail_work.q = 0;
                        spatial_or_hit_descriptor_tail_work.f[0] = -0.15f;
                        transform_vector_by_basis(&spatial_or_hit_descriptor_tail_work,
                                                  &spatial_or_hit_descriptor_tail_work,
                                                  &((struct MobyView *)hero.moby)->vC0);
                        fast_vec_add(&spatial_or_hit_descriptor_work,
                                     &spatial_or_hit_descriptor_work,
                                     &spatial_or_hit_descriptor_tail_work);
                        spatial_or_hit_descriptor_work.f[0] += random_float_between(0.0f, 0.03f);
                        spatial_or_hit_descriptor_work.f[1] += random_float_between(0.0f, 0.03f);
                        spatial_or_hit_descriptor_work.f[2] += random_float_between(0.0f, 0.03f);
                        qcopy(&v10, &hero.motion.displacement);
                        FUN_L00_00262cf0(&v10,
                                         &v10,
                                         frame_time * -0.75f,
                                         hero.motion.rot.f[2],
                                         hero.motion.rot.f[1]);
                        hero.pack_particle_timer =
                            truncate_float_to_s32((frame_time * 7.0f - hero.ground_speed) /
                                                  (frame_time * 7.0f) * scale_game_frames(5));
                        FUN_L00_0026f080(&spatial_or_hit_descriptor_work,
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
                         FUN_L00_00209018(hero_swim_speed_by_animation_frame, 0x1E) * 5.0f;
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
            adjust_hero_planar_velocity_from_probes(0);
            t = hero.unk8EC * fast_cos(yaw) * -7.0f;
            spring_angle(&hero.motion.rot.f[0],
                         t,
                         &hero.swim_lean_vel,
                         0,
                         0.007f,
                         0.17f,
                         frame_time * 1.2217305f);
            FUN_L15_00202d60(0.009f, 0.22f);
            FUN_L15_00202d78(0.04f, 0.2f);
            FUN_L15_00202d90(0.02f, 0.2f);
            AN.f114 = swim_pitch_delta_or_combo_spacing_error * 47.0f;
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
            AN.f324 = swim_pitch_delta_or_combo_spacing_error * 40.0f;
            AN.f418 = 0.22f;
            AN.f3D4 = swim_pitch_delta_or_combo_spacing_error * 40.0f;
            AN.f328 = t * 1.1f;
            AN.f3D8 = t * 1.1f;
        }
        break;
    case HERO_STATE_HURT_WITH_DISPLACEMENT_VELOCITY: {
        s32 n = 10 - hero.state_timer / 3;
        emit_hero_randomized_motion_effects(n > 8 ? 8 : n, 0);
        GROUND_FRICTION();
        adjust_hero_planar_velocity_from_probes(0);
        break;
    }
    case HERO_STATE_WATER_SURFACE_HURT:
        emit_hero_periodic_surface_effect(0xF, 0x1E);
        GROUND_FRICTION();
        adjust_hero_planar_velocity_from_probes(0);
        update_hero_water_height_and_bobbing();
        break;
    case HERO_STATE_DROWN:
    case HERO_STATE_DYING_ZERO_HEALTH: {
        f32 len = fast_vec_length(&hero.motion.momentum);
        approach_value(&len, 0.0f, frame_time_sq * 4.0f);
        fast_vec_normalize(&hero.motion.momentum, &hero.motion.momentum, len);
        qcopy(&hero.motion.velocity, &hero.motion.momentum);
        if (fast_dec_timer(&hero.puff_timer) == 2) {
            moby_point_position((struct MobyView *)hero.moby, 4, &spatial_or_hit_descriptor_work);
            spatial_or_hit_descriptor_tail_work.q = 0;
            spatial_or_hit_descriptor_tail_work.f[0] = 0.4f;
            transform_vector_by_basis(&spatial_or_hit_descriptor_tail_work,
                                      &spatial_or_hit_descriptor_tail_work,
                                      &((struct MobyView *)hero.moby)->vC0);
            fast_vec_add(&spatial_or_hit_descriptor_work,
                         &spatial_or_hit_descriptor_work,
                         &spatial_or_hit_descriptor_tail_work);
            qcopy(&v10, &hero.motion.displacement);
            fast_vec_normalize(&spatial_or_hit_descriptor_tail_work,
                               &spatial_or_hit_descriptor_tail_work,
                               frame_time * 1.5f);
            fast_vec_add(&v10, &v10, &spatial_or_hit_descriptor_tail_work);
            v10.f[2] += frame_time * 2.0f;
            FUN_L00_0026f080(&spatial_or_hit_descriptor_work, &v10, 25200.0f, -1.0f);
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
                         FUN_L00_00209018(hero_swim_speed_by_animation_frame, 0x1E) * 5.5f;
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
        adjust_hero_planar_velocity_from_probes(0);
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
        if (anim != -1 && ((struct MobyView *)hero.moby)->b52 != 0x36) {
            set_hero_animation(anim, 0, frames);
        } else if ((((struct MobyView *)hero.moby)->b70 & 2) &&
                   ((struct MobyView *)hero.moby)->b52 == 0x36) {
            s32 an = get_hero_help_value(0);
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
        if ((struct MobyView *)hero.grapple_target != NULL) {
            hero.motion.move_yaw =
                fast_arc_tan(((struct MobyView *)hero.grapple_target)->u10.f10 - moby->u10.f10,
                             ((struct MobyView *)hero.grapple_target)->u10.f14.v - moby->u10.f14.v);
            turn_to(0, frame_scale_sq * 0.02f, frame_scale_sq * 0.15f, frame_time * 6.2831855f);
        }
        break;
    case HERO_STATE_APPROACH_TARGET_MOBY: {
        f32 hit;
        f32 hit2;
        qcopy(&spatial_or_hit_descriptor_work, &((struct MobyView *)hero.grapple_target)->u10.v10);
        hero.unk970 = fast_vec_dist(&hero.motion.pos, &spatial_or_hit_descriptor_work);
        hero.target_arrival_frames = scale_game_frames(60);
        if (hero.grapple_pull_speed > 0.0f) {
            hero.target_arrival_frames = hero.unk970 / hero.motion.speed_xy;
        }
        if (hero.state.step == 0) {
            approach_value(&hero.grapple_pull_speed, frame_time * 27.0f, frame_time_sq * 80.0f);
            hero.motion.move_yaw =
                fast_arc_tan(((struct MobyView *)hero.grapple_target)->u10.f10 - moby->u10.f10,
                             ((struct MobyView *)hero.grapple_target)->u10.f14.v - moby->u10.f14.v);
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
                -fast_arc_tan(vector_distance_xy(&hero.motion.pos, &spatial_or_hit_descriptor_work),
                              spatial_or_hit_descriptor_work.f[2] - hero.motion.pos.f[2]),
                1.5707964f);
            step_angle_toward_target(&hero.motion.rot.f[1], 0, an, frame_time * 6.4577184f);
        }
        spatial_or_hit_descriptor_tail_work.f[0] =
            fast_cos(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * -0.57f;
        spatial_or_hit_descriptor_tail_work.f[1] =
            fast_sin(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * -0.57f;
        spatial_or_hit_descriptor_tail_work.f[2] = 0.0f;
        fast_vec_add(&spatial_or_hit_descriptor_tail_work,
                     &spatial_or_hit_descriptor_tail_work,
                     &hero.motion.pos);
        fast_vec_sub(&v10,
                     &((struct MobyView *)hero.grapple_target)->u10.v10,
                     &spatial_or_hit_descriptor_tail_work);
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
        qcopy(&spatial_or_hit_descriptor_work, &((struct MobyView *)hero.grapple_target)->u10.v10);
        hero.unk970 = fast_vec_dist(&hero.motion.pos, &spatial_or_hit_descriptor_work);
        fast_vec_add(&v10, &hero.motion.pos, &hero.motion.velocity);
        fast_vec_sub(&motion_work_vector, &v10, &spatial_or_hit_descriptor_work);
        fast_vec_normalize(&motion_work_vector, &motion_work_vector, hero.tether_radius);
        fast_vec_add(&spatial_or_hit_descriptor_tail_work,
                     &spatial_or_hit_descriptor_work,
                     &motion_work_vector);
        fast_vec_sub(&hero.motion.velocity, &spatial_or_hit_descriptor_tail_work, &hero.motion.pos);
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
        if ((struct MobyView *)hero.swing_target_moby != NULL &&
            ((struct MobyView *)hero.swing_target_moby)->pm78 != NULL &&
            ((struct MobyView *)hero.swing_target_moby)->pm78->u34.i34 != 0) {
            f32 a = ((struct MobyView *)hero.swing_target_moby)->u40.f48.v;
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
        fast_vec_sub(&spatial_or_hit_descriptor_work,
                     &((struct MobyView *)hero.swing_target_moby)->u10.v10,
                     &hero.motion.pos);
        fast_vec_normalize(&spatial_or_hit_descriptor_work, &spatial_or_hit_descriptor_work, 1.0f);
        if (spatial_or_hit_descriptor_work.f[2] < 0.23f) {
            spatial_or_hit_descriptor_work.f[2] = 0.23f;
        }
        clear_u64_value(&spatial_or_hit_descriptor_tail_work);
        spatial_or_hit_descriptor_tail_work.f[2] = hero.motion.rot.f[2];
        euler_rotation_matrix(rotation_matrix_or_vector_work, &spatial_or_hit_descriptor_tail_work);
        transpose_rotation_matrix(transposed_yaw_rotation_matrix, rotation_matrix_or_vector_work);
        transform_vector(&spatial_or_hit_descriptor_work,
                         &spatial_or_hit_descriptor_work,
                         transposed_yaw_rotation_matrix);
        v10.f[0] = -fast_arc_tan(
            square_root(spatial_or_hit_descriptor_work.f[0] * spatial_or_hit_descriptor_work.f[0] +
                        spatial_or_hit_descriptor_work.f[2] * spatial_or_hit_descriptor_work.f[2]),
            spatial_or_hit_descriptor_work.f[1]);
        v10.f[1] =
            fast_arc_tan(spatial_or_hit_descriptor_work.f[2], spatial_or_hit_descriptor_work.f[0]);
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
        if (!(hero.motion.pos.f[2] < ((struct MobyView *)hero.swing_target_moby)->u10.f18.v)) {
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
                    motion_work_vector.f[0] = fast_cos(hero.motion.move_yaw) * s;
                    motion_work_vector.f[1] = fast_sin(hero.motion.move_yaw) * s;
                    motion_work_vector.f[2] = 0.0f;
                    fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &motion_work_vector);
                }
            }
        }
        if (hero.unk99C != 0) {
            f32 s;
            if (vector_distance_xy(&hero.motion.pos,
                                   &((struct MobyView *)hero.swing_target_moby)->u10.v10) <
                    hero.unk9A0 * 1.57f ||
                hero.ground_distance > 1.5f) {
                FUN_L00_002334d0(
                    &hero.motion.velocity, &hero.motion.displacement, frame_time_sq * 40.0f);
                if (hero.swing_entry_push != 0 && gravity_z(&hero.motion.velocity) > 0.0f) {
                    fast_vec_sub(&motion_work_vector,
                                 &hero.motion.pos,
                                 &((struct MobyView *)hero.swing_target_moby)->u10.v10);
                    FUN_L00_002334d0(
                        &hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 57.0f);
                }
            }
            s = fast_vec_length(&hero.motion.velocity) * (frame_time * 0.015f);
            fast_vec_scale(&motion_work_vector, &hero.motion.velocity, -1.0f);
            fast_vec_normalize(&motion_work_vector, &motion_work_vector, s);
            fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &motion_work_vector);
        }
        {
            struct MobyView *r;
            if ((struct MobyView *)hero.swing_target_moby != NULL &&
                (r = ((struct MobyView *)hero.swing_target_moby)->pm78) != NULL) {
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
            fast_vec_add(&motion_work_vector, &vD0, &hero.motion.velocity);
            fast_vec_sub(
                &vC0, &motion_work_vector, &((struct MobyView *)hero.swing_target_moby)->u10.v10);
            fast_vec_normalize(&vC0, &vC0, hero.unk9A4);
            fast_vec_add(
                &motion_work_vector, &((struct MobyView *)hero.swing_target_moby)->u10.v10, &vC0);
            fast_vec_sub(&hero.motion.velocity, &motion_work_vector, &vD0);
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
                motion_work_vector.f[0] = fast_cos(hero.swing_yaw) * hero.swing_push_speed;
                motion_work_vector.f[1] = fast_sin(hero.swing_yaw) * hero.swing_push_speed;
                motion_work_vector.f[2] = 0.0f;
                fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &motion_work_vector);
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
                    motion_work_vector.q = hero.motion.displacement.q;
                    up = forward_component(&motion_work_vector) > 0.0f;
                }
                if (up) {
                    if (moby->b53 != 0x35) {
                        set_hero_animation(0x35, 6, scale_game_frames(17));
                        hero.swing_anim_dir = 1;
                    } else if (((struct MobyView *)hero.moby)->b52 ==
                               ((struct MobyView *)hero.moby)->b53) {
                        adjust_hero_animation_speed_for_target_frame(42.0f, t, 0.5f, -1.0f);
                        hero.swing_anim_dir = 0;
                    }
                } else {
                    if (moby->b53 != 0x35) {
                        set_hero_animation(0x35, 0x15, scale_game_frames(9));
                        hero.swing_anim_dir = 2;
                    } else if (((struct MobyView *)hero.moby)->b52 ==
                               ((struct MobyView *)hero.moby)->b53) {
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
            qcopy(&spatial_or_hit_descriptor_work, &hero.motion.pos);
            spatial_or_hit_descriptor_work.f[2] -= 0.5f;
            rotation_matrix_or_vector_work[0].f[0] = fast_cos(hero.motion.rot.f[2]);
            rotation_matrix_or_vector_work[0].f[1] = fast_sin(hero.motion.rot.f[2]);
            rotation_matrix_or_vector_work[0].f[2] = 0.0f;
            make_hit_descriptor(&v10,
                                (struct MobyView *)hero.moby,
                                0x30000,
                                1.0f,
                                &rotation_matrix_or_vector_work[0]);
            collect_mobies_in_sphere(
                0.8f, &spatial_or_hit_descriptor_work, 0x10, (struct MobyView *)hero.moby, &v10);
        }
        if (hero.past_apex != 0.0f && hero.air_frames.s == 0) {
            if (hero.rise_fall_land_frames == 0) {
                l15_camera.shake_amount = 0.2f;
                l15_camera.shake_frames = scale_game_frames(40);
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
        if (hero.anim_changing != 0 || hero.anim_frame < D_L15_0017C228[hero.attack_record].f18) {
            hero.items[0].timer = 2;
        }
        hero.target_speed = 0.0f;
        if ((struct MobyView *)hero.melee_target != NULL) {
            f32 d = vector_distance_xy(&hero.motion.pos,
                                       &((struct MobyView *)hero.melee_target)->u10.v10);
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
        if (hero.heading_locked != 0 || (struct MobyView *)hero.melee_target != NULL) {
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
            qcopy(&spatial_or_hit_descriptor_work, &hero.motion.pos);
            offset_hero_position_against_gravity(
                &spatial_or_hit_descriptor_work, &spatial_or_hit_descriptor_work, -0.5f);
            rotation_matrix_or_vector_work[0].f[0] = fast_cos(hero.motion.rot.f[2]) * 2.0f;
            rotation_matrix_or_vector_work[0].f[1] = fast_sin(hero.motion.rot.f[2]) * 2.0f;
            rotation_matrix_or_vector_work[0].f[2] = 0.0f;
            make_hit_descriptor(&v10,
                                (struct MobyView *)hero.moby,
                                0x10000,
                                2.0f,
                                &rotation_matrix_or_vector_work[0]);
            v10.f[3] = 5627.925f;
            ((u8 *)&spatial_or_hit_descriptor_tail_work)[9] = 1;
            ((s16 *)&spatial_or_hit_descriptor_tail_work)[5] = 0x47;
            ((u8 *)&spatial_or_hit_descriptor_tail_work)[8] = 0;
            collect_mobies_in_sphere(
                0.4f, &spatial_or_hit_descriptor_work, 0x10, (struct MobyView *)hero.moby, &v10);
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
        if (hero.anim_changing != 0 || hero.anim_frame < D_L15_0017C228[hero.attack_record].f18) {
            hero.items[0].timer = 2;
        }
        jumping = hero.state.prev == 1 && scale_game_frames(20) < hero.state.prev_timer;
        if (!jumping) {
            if (hero.heading_locked != 0 || (struct MobyView *)hero.melee_target != NULL) {
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
        if (hero.anim_changing != 0 || hero.anim_frame < D_L15_0017C228[hero.attack_record].f20) {
            hero.items[0].timer = 2;
        }
        tb = &D_L15_0017C228[hero.attack_record];
        hero.target_speed = 0.0f;
        if ((struct MobyView *)hero.melee_target != NULL &&
            scale_game_frames(4) > hero.state_timer) {
            f32 d = vector_distance_xy(&((struct MobyView *)hero.melee_target)->u10.v10,
                                       &hero.motion.pos) -
                    0.5f;
            f32 off;
            f32 tol;
            f32 mx;
            f32 swim_pitch_delta_or_combo_spacing_error;
            if (d < 0.0f) {
                d = 0.0f;
            }
            if (tb->kind == 0) {
                off = 0.75f;
                tol = 0.8f;
                mx = 4.0f;
            } else if (tb->kind == 1) {
                off = 0.6f;
                tol = 1.7f;
                mx = 4.0f;
            } else {
                off = 1.3f;
                tol = 1.7f;
                mx = 1.9f;
            }
            swim_pitch_delta_or_combo_spacing_error = d - off;
            if (absolute_float(swim_pitch_delta_or_combo_spacing_error) < tol + 3.0f) {
                f32 s = swim_pitch_delta_or_combo_spacing_error / 0.9f;
                if (s > mx) {
                    s = mx;
                }
                if (s > 1.0f) {
                    hero.speed_scale = s;
                }
            }
        }
        switch (tb->kind) {
        case HERO_COMBO_KIND_ANIMATION_GATED_TARGET_SPEED:
            if (hero.anim_frame < 18.0f) {
                hero.target_speed = frame_time * 4.4f * hero.speed_scale;
            }
            break;
        case HERO_COMBO_KIND_STAGED_TARGET_SPEED_REDUCTION:
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
        case HERO_COMBO_KIND_FLAG_OR_ANIMATION_GATED_TARGET_SPEED:
            if (hero.anim_changing != 0 || hero.anim_frame < 12.0f) {
                hero.target_speed = frame_time * 3.7f * hero.speed_scale;
            }
            break;
        }
        approach_value(&hero.anim_speed, 1.0f, 0.2f);
        if (hero.heading_locked != 0 || (struct MobyView *)hero.melee_target != NULL) {
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
        if (hero.heading_locked != 0 || (struct MobyView *)hero.melee_target != NULL) {
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
            initialize_moby_transform_history((struct MobyView *)hero.moby, &hero.trail.pos[0]);
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
            struct MobyView *o = (struct MobyView *)hero.items[0].moby;
            if (o != NULL) {
                f32 a;
                motion_work_vector.f[0] = fast_cos(hero.motion.rot.f[2]) * 2.0f;
                motion_work_vector.f[1] = fast_sin(hero.motion.rot.f[2]) * 2.0f;
                motion_work_vector.f[2] = 1.3f;
                make_hit_descriptor(&spatial_or_hit_descriptor_work,
                                    (struct MobyView *)hero.moby,
                                    0x30000,
                                    3.0f,
                                    &motion_work_vector);
                ((u8 *)&v10)[9] = 3;
                spatial_or_hit_descriptor_work.f[3] = 5627.925f;
                ((u8 *)&v10)[8] = 0;
                a = hero.motion.rot.f[2];
                ((s16 *)&v10)[5] = o->hA6;
                rotation_matrix_or_vector_work[0].f[0] = fast_cos(a) * 0.8f;
                rotation_matrix_or_vector_work[0].f[1] = fast_sin(a) * 0.8f;
                rotation_matrix_or_vector_work[0].f[2] = 0.55f;
                fast_vec_add(&rotation_matrix_or_vector_work[0],
                             &rotation_matrix_or_vector_work[0],
                             &hero.motion.pos);
                emit_filtered_entry_sparkles(
                    o,
                    collect_mobies_in_sphere(0.8f,
                                             &rotation_matrix_or_vector_work[0],
                                             0,
                                             o,
                                             &spatial_or_hit_descriptor_work));
                a = fast_add_rotations(hero.motion.rot.f[2], -0.87266463f);
                rotation_matrix_or_vector_work[0].f[0] = fast_cos(a) * 0.8f;
                rotation_matrix_or_vector_work[0].f[1] = fast_sin(a) * 0.8f;
                rotation_matrix_or_vector_work[0].f[2] = 0.55f;
                fast_vec_add(&rotation_matrix_or_vector_work[0],
                             &rotation_matrix_or_vector_work[0],
                             &hero.motion.pos);
                emit_filtered_entry_sparkles(
                    o,
                    collect_mobies_in_sphere(0.8f,
                                             &rotation_matrix_or_vector_work[0],
                                             0,
                                             (struct MobyView *)hero.moby,
                                             &spatial_or_hit_descriptor_work));
                a = fast_add_rotations(hero.motion.rot.f[2], 0.87266463f);
                rotation_matrix_or_vector_work[0].f[0] = fast_cos(a) * 0.8f;
                rotation_matrix_or_vector_work[0].f[1] = fast_sin(a) * 0.8f;
                rotation_matrix_or_vector_work[0].f[2] = 0.55f;
                fast_vec_add(&rotation_matrix_or_vector_work[0],
                             &rotation_matrix_or_vector_work[0],
                             &hero.motion.pos);
                emit_filtered_entry_sparkles(
                    o,
                    collect_mobies_in_sphere(0.8f,
                                             &rotation_matrix_or_vector_work[0],
                                             0,
                                             o,
                                             &spatial_or_hit_descriptor_work));
            }
        }
        if (scale_game_frames(10) < hero.state_timer) {
            stop_hero_motion_without_forward_support(3.7f, 0.0f);
        }
        adjust_hero_planar_velocity_from_probes(1);
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
            spatial_or_hit_descriptor_work.q = hero.move_impulse.q;
            hero.target_speed += forward_component(&spatial_or_hit_descriptor_work);
            if (hero.target_speed < frame_time * 0.7f) {
                hero.target_speed = frame_time * 0.7f;
            }
            clear_u64_value(move_impulse_ptr);
        }
        k = 1.0f;
        if (hero.state.step == 1 && hero.unk3BE == 0) {
            turn_to(0, frame_scale_sq * 0.008f, frame_scale_sq * 0.15f, frame_time * 9.948377f);
        } else {
            f32 turn_response_input = hero.stick_magnitude;
            f32 r = hero.motion.yaw_error;
            if (hero.item_anim_active != 0) {
                k = 1.0f - r * 0.55f;
                if (k < 0.0f) {
                    k = 0.0f;
                }
            } else if (turn_response_input < 0.8f && r > 0.5235988f) {
                k = 0.0f;
            }
            if (turn_response_input > hero_turn_input_or_approach_distance_threshold[0]) {
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
        adjust_hero_planar_velocity_from_probes(1);
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
            release_voice_slot_cf(2, (struct MobyView *)hero.moby, 0x1D);
        } else {
            release_voice_slot_cf(2, (struct MobyView *)hero.moby, 6);
        }
        if (current_level_index != 0xF) {
            emit_hero_randomized_motion_effects(4, 2);
            spatial_or_hit_descriptor_work.f[0] =
                hero.motion.pos.f[0] + random_float_between(-0.25f, 0.25f);
            spatial_or_hit_descriptor_work.f[1] =
                hero.motion.pos.f[1] + random_float_between(-0.25f, 0.25f);
            spatial_or_hit_descriptor_work.f[2] =
                hero.motion.pos.f[2] + random_float_between(-0.1f, 0.4f);
            fast_vec_scale(&v10, &hero.motion.displacement, 0.9f);
            v10.f[0] += random_float_between(frame_time * -0.7f, frame_time * 0.7f);
            v10.f[1] += random_float_between(frame_time * -0.7f, frame_time * 0.7f);
            v10.f[2] += random_float_between(frame_time, frame_time * 2.0f);
            spawn_random_alpha_rotated_particle(
                &spatial_or_hit_descriptor_work, &v10, random_float_between(37800.0f, 75600.0f));
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
        FUN_L15_00202d60(0.009f, 0.22f);
        FUN_L15_00202d78(0.04f, 0.2f);
        FUN_L15_00202d90(0.02f, 0.2f);
        AN.f118 = d * 2.1f;
        AN.f110 = d * 3.5f;
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
        qcopy(&spatial_or_hit_descriptor_tail_work, &hero.motion.pos);
        if (hero.state.current == 0x1A) {
            rotation_matrix_or_vector_work[0].f[0] =
                fast_cos(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
            rotation_matrix_or_vector_work[0].f[1] =
                fast_sin(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
            rotation_matrix_or_vector_work[0].f[2] = 0.0f;
        } else {
            rotation_matrix_or_vector_work[0].f[0] =
                fast_cos(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
            rotation_matrix_or_vector_work[0].f[1] =
                fast_sin(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
            rotation_matrix_or_vector_work[0].f[2] = 0.0f;
        }
        fast_vec_add(&motion_work_vector,
                     &spatial_or_hit_descriptor_tail_work,
                     &rotation_matrix_or_vector_work[0]);
        update_hero_ledge_probe();
        if (hero.ledge_found == 0) {
            break;
        }
        qcopy(&spatial_or_hit_descriptor_work, &hero.ledge_hang_pos);
        a = fast_add_rotations(hero.ledge_normal_yaw, 3.1415927f);
        qcopy(&hero.motion.pos, &motion_work_vector);
        update_hero_ledge_probe();
        qcopy(&hero.motion.pos, &spatial_or_hit_descriptor_tail_work);
        if (hero.ledge_found == 0) {
            break;
        }
        qcopy(&v10, &hero.ledge_hang_pos);
        b = fast_add_rotations(hero.ledge_normal_yaw, 3.1415927f);
        if (fast_difference_between_rotations(a, b) > 0.5235988f) {
            break;
        }
        a = fast_add_rotations(fast_subtract_rotations(a, b) * 0.5f, a);
        fast_vec_add(&rotation_matrix_or_vector_work[1], &spatial_or_hit_descriptor_work, &v10);
        fast_vec_scale(
            &rotation_matrix_or_vector_work[1], &rotation_matrix_or_vector_work[1], 0.5f);
        hero.motion.move_yaw = a;
        turn_to(0, frame_scale_sq * 0.04f, frame_scale_sq * 0.2f, frame_time * 6.2831855f);
        fl = hero.motion.flat_speed;
        approach_value(&fl, hero.target_speed * frame_time, frame_time_sq * 5.0f);
        fast_vec_sub(&hero.motion.velocity, &rotation_matrix_or_vector_work[1], &hero.motion.pos);
        set_vector_length_xy(
            &hero.motion.velocity,
            &hero.motion.velocity,
            level15_hero_ledge_shimmy_speed_by_animation_frame[truncate_float_to_s32(
                hero.anim_frame)] *
                0.5f);
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
            qcopy(&spatial_or_hit_descriptor_work, &hero.motion.rot);
            spring_angle(&spatial_or_hit_descriptor_work.f[1],
                         t,
                         &hero.motion.rot_spring_vel.f[1],
                         0,
                         frame_scale_sq * 0.02f,
                         frame_scale_sq * 0.3f,
                         frame_time * 2.6179938f);
            spring_angle(&spatial_or_hit_descriptor_work.f[0],
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
        release_voice_slot_cf(4, (struct MobyView *)hero.moby, 0x12);
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
            hero.motion.move_yaw = *(f32 *)D_L15_001673D8;
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
        adjust_hero_planar_velocity_from_probes(1);
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
                if (e->pm88 == (struct MobyView *)hero.moby && e->b74 != 0) {
                    release_voice_slot(hero.voice_slot4);
                }
            }
            hero.voice_slot4 = -1;
            release_voice_slot_cf(3, (struct MobyView *)hero.moby, 2);
        } else {
            if (hero.unk2224 != -1) {
                struct VoiceSlot *e =
                    (struct VoiceSlot *)(((u8 *)&voice_pool) + hero.unk2224 * 0x70);
                if (e->pm88 == (struct MobyView *)hero.moby && e->b74 != 0) {
                    release_voice_slot(hero.unk2224);
                }
            }
            hero.unk2224 = -1;
            release_voice_slot_cf(4, (struct MobyView *)hero.moby, 0x12);
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
        s32 jump_timer_or_trail_control;
        s32 jump_frame_threshold;
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
                if (D_L15_001612D0 != 0) {
                    pool_splash(D_L15_001612D0,
                                D_L15_001612D8,
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
        jump_timer_or_trail_control = hero.state_timer;
        jump_frame_threshold = hero.takeoff_frame;
        if (jump_timer_or_trail_control == jump_frame_threshold) {
            qcopy(&hero.takeoff_pos, &hero.motion.pos);
        }
        if (hero.rail_land_frames != 0 ||
            (jump_timer_or_trail_control < jump_frame_threshold &&
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
                qcopy(&spatial_or_hit_descriptor_work, &hero.motion.pos);
                FUN_L00_00233810(
                    &spatial_or_hit_descriptor_work, &spatial_or_hit_descriptor_work, 0.5f);
                qcopy(&v10, &spatial_or_hit_descriptor_work);
                v10.f[0] += fast_cos(hero.motion.rot.f[2]) * 0.85f;
                v10.f[1] += fast_sin(hero.motion.rot.f[2]) * 0.85f;
                if (collision_line(&spatial_or_hit_descriptor_work,
                                   &v10,
                                   4,
                                   (struct MobyView *)hero.moby,
                                   0) != 0) {
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
            jump_timer_or_trail_control = (scale_game_frames(12) < hero.state_timer) * 2;
            update_moby_transform_history(&hero.trail.pos[0], jump_timer_or_trail_control);
        }
        if (hero.state.current == HERO_STATE_PACK_LONG_JUMP &&
            scale_game_frames(8) < hero.state_timer && hero.anim_frame < 28.0f) {
            s32 n;
            transform_hero_local_offset_to_world(&spatial_or_hit_descriptor_work, 0.8f, 0.0f, 0.3f);
            n = collect_mobies_in_sphere(
                0.9f, &spatial_or_hit_descriptor_work, 0, (struct MobyView *)hero.moby, NULL);
            if (n != 0) {
                s32 i;
                fast_vec_scale(&v10, &hero.motion.displacement, 7.0f);
                for (i = 0; i < n; i++) {
                    if (is_moby_in_class_range_500_to_540(D_L15_00178500[i]) != 0) {
                        spawn_nearby_effect(D_L15_00178500[i],
                                            (struct MobyView *)hero.moby,
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
        fast_vec_sub(&spatial_or_hit_descriptor_work, &hero.target_pose, &hero.motion.pos);
        hero.motion.move_yaw =
            fast_arc_tan(spatial_or_hit_descriptor_work.f[0], spatial_or_hit_descriptor_work.f[1]);
        l = vector_length_xy(&spatial_or_hit_descriptor_work);
        if (l > 0.2f) {
            hero.stick_magnitude = 1.0f;
            hero.motion.move_yaw = fast_arc_tan(spatial_or_hit_descriptor_work.f[0],
                                                spatial_or_hit_descriptor_work.f[1]);
            hero.target_speed = l;
            if (l < D_L15_0017C3B8.x0C) {
                hero.target_speed = D_L15_0017C3B8.x08 * frame_time;
            } else {
                hero.target_speed = D_L15_0017C3B8.x18 * frame_time;
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
            adjust_hero_planar_velocity_from_probes(1);
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
        fast_vec_sub(&spatial_or_hit_descriptor_work, &hero.target_pose, &hero.motion.pos);
        if (fast_vec_dot(&spatial_or_hit_descriptor_work, &hero.motion.momentum) < 0.0f) {
            clear_u64_value(&hero.motion.momentum);
        }
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.0f);
        adjust_hero_planar_velocity_from_probes(0);
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
    case HERO_STATE_WAIT_FOR_JUMP_OR_RESPAWN: {
        clear_u64_value(&hero.motion.velocity);
        if ((f32)(hero.sink_jump_count + 1) * 0.5f < hero.sink_surface_z - hero.motion.pos.f[2]) {
            hero.motion.velocity.f[2] = -(frame_time * 0.25f);
        } else {
            hero.motion.velocity.f[2] = -frame_time;
        }
        break;
    }
    case HERO_STATE_HEIGHT_OR_TIMEOUT_RESPAWN_WAIT:
        clear_u64_value(&hero.motion.velocity);
        hero.motion.velocity.f[2] = -frame_time;
        break;
    case HERO_STATE_VEHICLE_MODE_2_IDLE:
    case HERO_STATE_VEHICLE_MODE_2_DYING:
        stop_hero_motion_without_forward_support(3.7f, 0.0f);
        stick_target(1.0f, 0);
        hero.target_speed = 0.0f;
        speed_step(0.0f, frame_time_sq * 12.6f);
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.6f);
        adjust_hero_planar_velocity_from_probes(0);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    case HERO_STATE_VEHICLE_MODE_2_REPEATABLE_SQUARE_ACTION:
        stick_target(1.0f, 0);
        turn_to(0, frame_scale_sq * 0.02f, frame_scale_sq * 0.2f, frame_time * 1.9198622f);
        hero.target_speed = 0.0f;
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.6f);
        if (hero.anim_changing == 0) {
            s32 i;
            rotation_matrix_or_vector_work[0].f[0] = fast_cos(hero.motion.rot.f[2]);
            rotation_matrix_or_vector_work[0].f[1] = fast_sin(hero.motion.rot.f[2]);
            rotation_matrix_or_vector_work[0].f[2] = 0.0f;
            make_hit_descriptor(&v10,
                                (struct MobyView *)hero.moby,
                                0xB0000,
                                4.0f,
                                &rotation_matrix_or_vector_work[0]);
            v10.f[2] = 1.0f;
            v10.f[3] = 5627.925f;
            ((u8 *)&spatial_or_hit_descriptor_tail_work)[8] = 7;
            ((u8 *)&spatial_or_hit_descriptor_tail_work)[9] = 1;
            ((s16 *)&spatial_or_hit_descriptor_tail_work)[5] = ((struct MobyView *)hero.moby)->hA6;
            switch (hero.attack_record) {
            case HERO_ATTACK_RECORD_POINT_3_HIT_QUERY:
                if (hero.anim_frame > 7.0f && hero.anim_frame < 14.0f) {
                    moby_point_position(
                        (struct MobyView *)hero.moby, 3, &spatial_or_hit_descriptor_work);
                    collect_mobies_in_sphere(2.0f,
                                             &spatial_or_hit_descriptor_work,
                                             0x10,
                                             (struct MobyView *)hero.moby,
                                             &v10);
                }
                break;
            case HERO_ATTACK_RECORD_POINT_4_HIT_QUERY:
                if (hero.anim_frame > 7.0f && hero.anim_frame < 14.0f) {
                    moby_point_position(
                        (struct MobyView *)hero.moby, 4, &spatial_or_hit_descriptor_work);
                    collect_mobies_in_sphere(2.0f,
                                             &spatial_or_hit_descriptor_work,
                                             0x10,
                                             (struct MobyView *)hero.moby,
                                             &v10);
                }
                break;
            case HERO_ATTACK_RECORD_DUAL_POINT_HIT_QUERY:
                if (hero.anim_frame > 14.0f && hero.anim_frame < 21.0f) {
                    f32 hit_radius = 2.0f;
                    moby_point_position(
                        (struct MobyView *)hero.moby, 3, &spatial_or_hit_descriptor_work);
                    collect_mobies_in_sphere(hit_radius,
                                             &spatial_or_hit_descriptor_work,
                                             0x10,
                                             (struct MobyView *)hero.moby,
                                             &v10);
                    hit_radius = 0.0f;
                    moby_point_position(
                        (struct MobyView *)hero.moby, 4, &spatial_or_hit_descriptor_work);
                    collect_mobies_in_sphere(2.0f,
                                             &spatial_or_hit_descriptor_work,
                                             0x10,
                                             (struct MobyView *)hero.moby,
                                             &v10);
                }
                break;
            }
            for (i = 0; i < 3; i++) {
                transform_hero_polar_offset_to_world(
                    &rotation_matrix_or_vector_work[1],
                    1.8f,
                    fast_add_rotations(-0.7853982f, (f32)i * 0.7853982f),
                    1.4f);
                play_entity_sound((struct MobyView *)hero.moby,
                                  2.2f,
                                  &rotation_matrix_or_vector_work[1],
                                  0x30000,
                                  40.0f,
                                  1.0f,
                                  0,
                                  1,
                                  0);
            }
        }
        switch (hero.attack_record) {
        case HERO_ATTACK_RECORD_POINT_3_HIT_QUERY:
        case HERO_ATTACK_RECORD_POINT_4_HIT_QUERY:
            if (hero.anim_changing == 0 && hero.anim_frame >= 7.0f && hero.anim_frame < 15.0f) {
                hero.target_speed = frame_time * 21.0f;
            }
            speed_step(frame_time_sq * 180.0f, frame_time_sq * 140.0f);
            set_planar_vel(99999.0f);
            break;
        case HERO_ATTACK_RECORD_DUAL_POINT_HIT_QUERY:
            if (hero.anim_changing == 0 && hero.anim_frame >= 11.0f && hero.anim_frame < 20.0f) {
                hero.target_speed = frame_time * 30.0f;
            }
            speed_step(frame_time_sq * 150.0f, frame_time_sq * 120.0f);
            set_planar_vel(99999.0f);
            break;
        }
        adjust_hero_planar_velocity_from_probes(1);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        stop_hero_motion_without_forward_support(3.7f, 0.0f);
        break;
    case HERO_STATE_VEHICLE_MODE_2_COOLDOWN_ANIMATION_WAIT:
        hero.target_speed = 0.0f;
        speed_step(hero.target_speed, frame_time_sq * 12.6f);
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.6f);
        if (hero.anim_changing == 0) {
            if (hero.anim_frame > 22.0f && hero.anim_frame < 55.0f) {
                hero.anim_speed = 1.0f;
            } else if (hero.anim_changing == 0 && hero.anim_frame <= 22.0f) {
                fast_vec_normalize(&spatial_or_hit_descriptor_work, &hero, 3.0f);
                spatial_or_hit_descriptor_work.f[2] += 5.0f;
                add_vectors(&v10, &spatial_or_hit_descriptor_work, &moby->u10.v10);
                set_giant_clank_charged_blast_position(moby, &spatial_or_hit_descriptor_work);
                hero.anim_speed = 0.5f;
            }
        }
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    case HERO_STATE_WAIT_FOR_CIRCLE_RELEASE_AND_ANIMATION: {
        f32 z;
        hero.target_speed = 0.0f;
        z = hero.target_speed;
        speed_step(z, frame_time_sq * 12.6f);
        stick_target(1.0f, 0);
        turn_to(0, frame_scale_sq * 0.01f, frame_scale_sq * 0.2f, frame_time * 1.2217305f);
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.6f);
        if (test_hero_animation_frame_window(4.0f) != 0) {
            s32 i;
            f32 w = z;
            z = 1.0f;
            for (i = 0; i < 2; i++) {
                moby_point_position(
                    (struct MobyView *)hero.moby, i + 3, &spatial_or_hit_descriptor_work);
                spatial_or_hit_descriptor_work.f[0] += fast_cos(hero.motion.rot.f[2]) * 1.4f;
                spatial_or_hit_descriptor_work.f[1] += fast_sin(hero.motion.rot.f[2]) * 1.4f;
                spawn_giant_clank_missile((struct MobyView *)hero.moby,
                                          &spatial_or_hit_descriptor_work,
                                          hero.motion.rot.f[2],
                                          w,
                                          0,
                                          0.5f,
                                          hero.motion.rot.f[2],
                                          w);
            }
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
    case HERO_STATE_VEHICLE_MODE_2_FALL:
        stick_target(frame_time * 6.0f, 0);
        qcopy(&spatial_or_hit_descriptor_work, &hero.motion.pos);
        spatial_or_hit_descriptor_work.f[2] -= 1.0f;
        play_entity_sound((struct MobyView *)hero.moby,
                          3.5f,
                          &spatial_or_hit_descriptor_work,
                          0x30000,
                          40.0f,
                          1.0f,
                          0,
                          1,
                          0);
        if (hero.move_lock_timer == 0) {
            hero_update_hop_velocity();
        }
        FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 12.0f);
        if (hero.motion.velocity.f[2] < frame_time * -10.0f) {
            hero.motion.velocity.f[2] = frame_time * -10.0f;
        }
        break;
    case HERO_STATE_VEHICLE_MODE_2_RUN: {
        f32 d;
        f32 k;
        f32 g;
        f32 a;
        s32 i;
        d = fast_difference_between_rotations(hero.motion.rot.f[2], hero.motion.move_yaw);
        for (i = 0; i < 3; i++) {
            transform_hero_polar_offset_to_world(
                &spatial_or_hit_descriptor_work,
                1.8f,
                fast_add_rotations(-0.7853982f, (f32)i * 0.7853982f),
                1.4f);
            play_entity_sound((struct MobyView *)hero.moby,
                              2.7f,
                              &spatial_or_hit_descriptor_work,
                              0x30000,
                              40.0f,
                              1.0f,
                              0,
                              1,
                              0);
        }
        if (test_hero_animation_frame_window(30.0f) != 0 ||
            test_hero_animation_frame_window(0.0f) != 0) {
            l15_camera.shake_amount = 0.1f;
            l15_camera.shake_frames = scale_game_frames(20);
        }
        k = 1.0f;
        stick_target(frame_time * 6.0f, 0);
        k = k - d * 0.3f;
        g = hero.stick_magnitude + 0.35f;
        if (k < 0.0f) {
            k = 0.0f;
        }
        turn_to(0, frame_scale_sq * 0.021f * g, frame_scale_sq * 0.3f, frame_time * 2.268928f * g);
        speed_step(frame_time_sq * 7.5f * k, frame_time_sq * 8.5f);
        a = hero.motion.yaw_error;
        if (a > 1.7f) {
            a = 1.7f;
        } else if (a < -1.7f) {
            a = -1.7f;
        }
        {
            f32 t;
            AN.f1494 = frame_scale_sq * 0.01f;
            t = frame_scale_sq * 0.3f;
            AN.f1334 = frame_scale_sq * 0.015f;
            AN.f1498 = t;
            AN.f13E4 = frame_scale_sq * 0.02f;
            AN.f1338 = t;
            AN.f13E8 = t;
            AN.f12F0 = -a * 0.14f;
            AN.f12F8 = a * 0.25f;
            AN.f13A8 = a * 0.57f;
        }
        if (hero.unk3B8 != 0 && hero.unk308 == 0) {
            set_planar_vel(hero.motion.move_yaw);
        } else {
            set_planar_vel(99999.0f);
        }
        adjust_hero_planar_velocity_from_probes(1);
        if (hero.air_frames.s != 0) {
            FUN_L00_002334d0(
                &hero.motion.velocity, &hero.motion.resolved_velocity_z, frame_time_sq * 25.0f);
            correct_hero_airborne_wall_velocity();
        } else {
            FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 54.0f);
        }
        break;
    }
    case HERO_STATE_VEHICLE_MODE_2_HURT: {
        f32 len = vector_length_xy(&hero.motion.velocity);
        f32 l2 = len;
        if (hero.air_frames.s != 0) {
            len = frame_scale * -0.00999999f * l2 + l2;
        } else {
            len = frame_scale * -0.07999998f * l2 + l2;
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
    case HERO_STATE_VEHICLE_MODE_3_IDLE:
    case HERO_STATE_VEHICLE_MODE_3_DYING:
    case HERO_STATE_DELAYED_CIRCLE_RELEASE_WAIT:
        stop_hero_motion_without_forward_support(3.7f, 0.0f);
        stick_target(1.0f, 0);
        hero.target_speed = 0.0f;
        speed_step(0.0f, frame_time_sq * 12.6f);
        clear_u64_value(&hero.motion.velocity);
        reduce_and_apply_hero_planar_velocity_addend(frame_time_sq * 12.6f);
        adjust_hero_planar_velocity_from_probes(0);
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
        adjust_hero_planar_velocity_from_probes(1);
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
        f32 l2 = len;
        if (hero.air_frames.s != 0) {
            len = frame_scale * -0.00999999f * l2 + l2;
        } else {
            len = frame_scale * -0.07999998f * l2 + l2;
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
