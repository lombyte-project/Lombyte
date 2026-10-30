/* Hero: handles a hit on the hero. */
#include "types.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/hero.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/entities.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/hero/hero_idle_sequence.h"
#include "rnc/overlay/l08/hero/hero_set_state.h"

void allocate_hero_voice_for_entry(s32, s32) __asm__("FUN_L00_00216de8");
void hero_knockback(void *, s32, f32, f32) __asm__("FUN_L00_00210c80");

#define G hero

extern OvlMobyEntry40 l08_moby_hit_records[] __asm__("D_L08_00178900");
s32                   handle_hero_hit(s32 apply_response) __asm__("FUN_L08_00222800");

s32 handle_hero_hit(s32 apply_response) {
    u8             *hero_moby;
    OvlMobyEntry40 *hero_moby_entry;
    u8             *mob;
    s32             flag;
    s32             id;
    f32             xy_response_scale, z_response_amount, frame_time_scale;
    OvlVec4         response_vector;

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
    if (level_game_mode != 0) {
        return 0;
    }
    G.damage_source_moby = 0;
    hero_moby = (u8 *)G.moby;
    if (hero_moby[0xA4] == 0xFF) {
        return 0;
    }
    hero_moby_entry = &l08_moby_hit_records[hero_moby[0xA4]];
    if (hero_moby_entry->owner != hero_moby) {
        return 0;
    }
    if ((hero_moby_entry->unk24 ^ 1) & 1) {
        return 0;
    }
    D_0013DF88[current_level_index]++;
    G.damage_source_moby = (struct Moby *)hero_moby_entry->target;
    D_0015EEA8++;
    if (G.state.control_mode == 0xF) {
        G.unk5BE = 1;
        return 0;
    }
    if (apply_response == 0) {
        return 1;
    }
    release_and_clear_hero_voice_slots();
    flag = 0;
    if (hero_moby_entry->flags & 1) {
        qcopy(&response_vector, &hero_moby_entry->pos);
        if (*(f32 *)((u8 *)hero_moby_entry + 0x1C) == 5627.9248046875f) {
            flag = 1;
        }
    } else if (hero_moby_entry->target != 0) {
        fast_vec_sub(&response_vector, (u8 *)&G + 0x80, hero_moby_entry->target + 0x10);
    } else {
        response_vector.f[0] = fast_cos(fast_add_rotations(G.motion.rot.f[2], 3.1415927f));
        response_vector.f[1] = fast_sin(fast_add_rotations(G.motion.rot.f[2], 3.1415927f));
        response_vector.i[2] = 0;
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
            G.motion.resolved_velocity_z.f[2] = frame_time * 7.0f;
            return 1;
        }
        if (G.state.control_mode == 0x12) {
            mob = (u8 *)G.damage_source_moby;
            if (mob != 0 && *(s16 *)(mob + 0xA6) == 0x28F) {
                id = 0x82;
                goto call_ret;
            }
            hero_set_state(0x75, 1);
        } else if (G.state.control_mode == 0x11) {
            mob = (u8 *)G.damage_source_moby;
            if (mob != 0 && *(s16 *)(mob + 0xA6) == 0x28F) {
                id = 0x82;
                goto call_ret;
            }
            if (current_level_index == 0xF || current_level_index == 0x11) {
                mob = (u8 *)G.damage_source_moby;
                if (mob != 0 && (*(s16 *)(mob + 0xA6) == 0x28F || *(s16 *)(mob + 0xA6) == 0x7B ||
                                 *(s16 *)(mob + 0xA6) == 0x29D)) {
                    allocate_hero_voice_for_entry(0x1C, 0);
                }
            }
            hero_set_state(0x76, 1);
        } else if (G.state.control_mode == 3) {
            mob = (u8 *)G.damage_source_moby;
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
        frame_time_scale = frame_time;
        xy_response_scale = frame_time_scale * 5.7f;
        z_response_amount = frame_time_scale * 2.4f;
        if (G.on_magnet_ground != 0) {
            xy_response_scale = 0.0f;
            z_response_amount = frame_time_scale * 1.7f;
        }
        hero_knockback(&response_vector, flag, xy_response_scale, z_response_amount);
        if (flag != 0 && hero_moby_entry->unk28 == 4) {
            response_vector.f[2] += response_vector.f[2];
        }
        break;
    case 3:
        hero_set_state(0x56, 1);
        hero_knockback(&response_vector, flag, frame_time * 5.0f, frame_time * 2.4f);
        break;
    }
    fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &response_vector);
    fast_vec_add(&hero.motion.displacement, &hero.motion.displacement, &response_vector);
    return 1;
}
