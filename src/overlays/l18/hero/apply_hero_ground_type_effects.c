/* Level 18 hero: ground type effects and the hurt response. */
#include "types.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/hero/hero_idle_sequence.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/mobys/moby.h"

/* Same source as the exact FUN_L16_002097a0, with a state 3 flag (0x12E6) and its own landing checks (as FUN_L15_002044c8). */
extern int  D_0015ED84_d __asm__("D_0015ED84");
extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");
extern int  hero_set_state(int, int) __asm__("FUN_L18_00227dd0");
float       absolute_float(float input) __asm__("func_001F99C0");

void apply_hero_ground_type_effects(void) __asm__("FUN_L18_00213b40");

void apply_hero_ground_type_effects(void) {
    struct Hero *hero_state = &hero;
    int          keep = hero_state->ground_sound_class;
    int          saved_ground_type = hero_state->ground_type;
    int          collision_check_selector;
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
                char *item_moby = ((char *)hero_state->items[1].moby);
                if (item_moby != 0 && MOBY(item_moby)->oclass == 0xAD)
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
        collision_check_selector = current_level_index;
    } else {
        collision_check_selector = current_level_index;
    }
    if (saved_ground_type == 3) {
        struct Hero *q = &hero;
        *(char *)&q->selector_3 = 1;
        collision_check_selector = D_0015ED84_d;
    }
    if (collision_check_selector == 0xD) {
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
    if (saved_ground_type == 0x2) {
        struct Hero *q = &hero;
        *(char *)&q->on_magnet_ground = 1;
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
#include "rnc/math/fast_math.h"

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

void allocate_hero_voice_for_entry(s32, s32) __asm__("FUN_L00_00216de8");
void remove_giant_clank_charged_blast(void) __asm__("FUN_L00_002a9be0");
void hero_knockback(void *, s32, f32, f32) __asm__("FUN_L00_00210c80");

extern T_210ce8 l18_moby_hurt_response_records[] __asm__("D_L18_00178B00");
s32             process_hero_hurt_response(s32 apply_hurt_response) __asm__("FUN_L18_00218a90");

s32 process_hero_hurt_response(s32 apply_hurt_response) {
    u8       *hero_moby_bytes;
    T_210ce8 *response_record;
    u8       *mob;
    s32       flag;
    s32       id;
    f32       horizontal_response_scale, vertical_response_scale, response_frame_time;
    V_210ce8  response_motion_delta;

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
    if (D_L18_0015F5C4 != 0) {
        return 0;
    }
    hero.damage_source_moby = 0;
    hero_moby_bytes = ((u8 *)hero.moby);
    if (hero_moby_bytes[0xA4] == 0xFF) {
        return 0;
    }
    response_record = &l18_moby_hurt_response_records[hero_moby_bytes[0xA4]];
    if (response_record->p34 != hero_moby_bytes) {
        return 0;
    }
    if (hero.vehicle_mode == 2) {
        if (!(response_record->i24 & 2)) {
            return 0;
        }
    } else if ((response_record->i24 ^ 1) & 1) {
        return 0;
    }
    D_0013DF88[current_level_index]++;
    hero.damage_source_moby = (struct Moby *)response_record->p20;
    D_0015EEA8++;
    if (hero.state.control_mode == 0xF) {
        hero.unk5BE = 1;
        return 0;
    }
    if (hero.vehicle_mode == 2) {
        hero.vehicle_hp -= truncate_float_to_s32(response_record->f2C);
        if (hero.vehicle_hp < 0) {
            hero.vehicle_hp = 0;
        }
    }
    if (apply_hurt_response == 0) {
        return 1;
    }
    release_and_clear_hero_voice_slots();
    flag = 0;
    if (response_record->i30 & 1) {
        qcopy(&response_motion_delta, &response_record->v10);
        if (*(f32 *)((u8 *)response_record + 0x1C) == 5627.9248046875f) {
            flag = 1;
        }
    } else if (response_record->p20 != 0) {
        fast_vec_sub(&response_motion_delta, &hero.motion.pos, response_record->p20 + 0x10);
    } else {
        response_motion_delta.f[0] = fast_cos(fast_add_rotations(hero.motion.rot.f[2], 3.1415927f));
        response_motion_delta.f[1] = fast_sin(fast_add_rotations(hero.motion.rot.f[2], 3.1415927f));
        response_motion_delta.i[2] = 0;
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
        response_frame_time = frame_time;
        horizontal_response_scale = response_frame_time * 5.7f;
        vertical_response_scale = response_frame_time * 2.4f;
        if (hero.on_magnet_ground != 0) {
            horizontal_response_scale = 0.0f;
            vertical_response_scale = response_frame_time * 1.7f;
        }
        hero_knockback(
            &response_motion_delta, flag, horizontal_response_scale, vertical_response_scale);
        if (flag != 0 && response_record->b28 == 4) {
            response_motion_delta.f[2] += response_motion_delta.f[2];
        }
        break;
    case HERO_VEHICLE_MODE_GIANT_CLANK:
        if ((response_record->i24 & 4) || hero.vehicle_hp <= 0) {
            hero_set_state(0x5D, 1);
            remove_giant_clank_charged_blast();
            if (hero.air_frames.s == 0) {
                hero_knockback(&response_motion_delta, flag, frame_time * 7.0f, frame_time * 3.5f);
                break;
            }
        }
        clear_u64_value(&response_motion_delta);
        break;
    case 3:
        hero_set_state(0x56, 1);
        hero_knockback(&response_motion_delta, flag, frame_time * 5.0f, frame_time * 2.4f);
        break;
    }
    fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &response_motion_delta);
    fast_vec_add(&hero.motion.displacement, &hero.motion.displacement, &response_motion_delta);
    return 1;
}
