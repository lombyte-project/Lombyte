/* Shared moby classes: hydrodisplacer_faucet, hoverboard, lamp, floor_button, blarg_gun. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "sda.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/l05/hero/hero_set_state.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/commands/set_moby_color.h"
#include "rnc/ui/help/try_set_help_message.h"

extern float D_0015EE6C_d4168 __asm__("D_0015ED6C");
char        *create_class_5c3_moby(int   owner,
                                   char *initial_position,
                                   int   arg,
                                   float initial_rotation_z,
                                   float initial_rotation_y) __asm__("FUN_L05_002d4168");

char *create_class_5c3_moby(int   owner,
                            char *initial_position,
                            int   arg,
                            float initial_rotation_z,

                            float initial_rotation_y) {
    char *moby = (char *)create_moby(0x5C3);
    if (moby != 0) {
        char *moby_vars = ((char *)MOBY(moby)->pvars);
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        MOBY(moby)->rot.z = initial_rotation_z;
        MOBY(moby)->rot.y = initial_rotation_y;
        moby[0x20] = 0;
        *(int *)moby_vars = owner;
        qcopy(moby + 0x10, initial_position);
        *(float *)(moby_vars + 0x10) = D_0015EE6C_d4168 * 30.0f * 0.7f;
        *(int *)(moby_vars + 8) = arg;
        *(int *)(moby_vars + 0x14) = 0;
        *(int *)(moby_vars + 0x18) = 0;
        *(short *)(moby_vars + 6) = 0;
        MOBY(moby)->scale *= 2.3f;
        *(short *)(moby_vars + 4) = scale_game_frames(0x12C);
    }
    return moby;
}

extern s32 moby_pool_4f60 __asm__("D_L05_0015FFD8");

void apply_spatial_trigger_to_moby(int index,
                                   int trigger_arg,
                                   int inside) __asm__("FUN_L05_002f4f60");

void apply_spatial_trigger_to_moby(int index, int trigger_arg, int inside) {
    int          moby = moby_pool_4f60 + index * 0x100;
    char        *trigger = (char *)trigger_arg;
    short        kind;
    unsigned int flags;

    if (moby == 0) {
        return;
    }
    if (inside) {
        if (trigger[0x7c] > 0) {
            MOBY(moby)->flags &= 0xfffd;
        } else if (trigger[0x7c] < 0) {
            MOBY(moby)->flags |= 2;
        }
        if (trigger[0x7d] > 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
            kind != 0x4ef && kind != 0x571) {
            MOBY(moby)->visible = 1;
            MOBY(moby)->flags &= 0xfffe;
        } else if (trigger[0x7d] < 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
                   kind != 0x4ef && kind != 0x571) {
            MOBY(moby)->visible = 0;
            flags = MOBY(moby)->flags | 1;
            MOBY(moby)->flags = flags;
        }
        if (trigger[0x7f] > 0 && *(int *)(moby + 0x24) != 0) {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        } else if (trigger[0x7f] < 0 && *(int *)(moby + 0x24) != 0) {
            MOBY(moby)->collision = 0;
        }
    } else {
        if (trigger[0x7c] > 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
            kind != 0x4ef) {
            MOBY(moby)->flags |= 2;
        } else if (trigger[0x7c] < 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
                   kind != 0x4ef) {
            MOBY(moby)->flags &= 0xfffd;
        }
        if (trigger[0x7d] > 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
            kind != 0x4ef && kind != 0x571) {
            MOBY(moby)->visible = 0;
            flags = MOBY(moby)->flags | 1;
            MOBY(moby)->flags = flags;
        } else if (trigger[0x7d] < 0 && (kind = MOBY(moby)->oclass) != 0x386 && kind != 0x3ae &&
                   kind != 0x4ef && kind != 0x571) {
            MOBY(moby)->visible = 1;
            MOBY(moby)->flags &= 0xfffe;
        }
        if (trigger[0x7f] > 0) {
            MOBY(moby)->collision = 0;
        } else if (trigger[0x7f] < 0) {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        }
    }
}

/* Runs func_L05_002F62E8 for each entry of list i until an entry without the top bit set. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_002F6518), where it is exact; names translated to the US level program. */

extern int *D_L05_001ABCC0[];

void apply_spatial_trigger_to_moby_list(int unused_trigger_moby,
                                        int trigger_vars_arg,
                                        int i,
                                        int inside) __asm__("FUN_L05_002f5190");

void apply_spatial_trigger_to_moby_list(int unused_trigger_moby,
                                        int trigger_vars_arg,
                                        int i,

                                        int inside) {
    unsigned short *moby_index_entry = (unsigned short *)D_L05_001ABCC0[i];
    if (moby_index_entry != 0) {
        do {
            apply_spatial_trigger_to_moby(*moby_index_entry & 0x7FFF, trigger_vars_arg, inside);
        } while (*(short *)moby_index_entry++ >= 0);
    }
}

#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_path.h"

/* pvars of the trigger moby run by FUN_L05_002f5200 */
typedef struct {
    s32 path;         /* -1, or an index into D_L05_001B0930 */
    f32 radius;       /* > 0: trigger when the target is this close */
    s32 volumes[6];   /* -1, or a clip volume is_point_inside_clip_volume tests */
    s32 on_lists[12]; /* -1, or a list run by FUN_L05_002f5190 */
    s32 on_mobys[11]; /* -1, or a moby passed to FUN_L05_002f4f60 */
    u8  pad7C[2];
    s8  use_alt_target; /* nonzero: test D_L05_001671C0 instead of the hero */
} TriggerVars;

extern Vec4 D_L05_001671C0_2f5200 __asm__("D_L05_001671C0");
extern s32  point_in_path_xy(void *, void *, s32) __asm__("FUN_L00_00259740");

void update_moby_spatial_trigger(struct Moby *trigger_moby) __asm__("FUN_L05_002f5200");

void update_moby_spatial_trigger(struct Moby *trigger_moby) {
    TriggerVars *trigger_vars = (TriggerVars *)trigger_moby->pvars;
    Vec4         target;
    s32          inside;
    s32          i;
    s32         *path;

    trigger_moby->update_dist = 0xFF;
    inside = 0;
    if (trigger_vars->use_alt_target != 0) {
        qcopy(&target, &D_L05_001671C0_2f5200);
    } else {
        qcopy(&target, &hero.motion.pos);
    }
    if (0.0f < trigger_vars->radius &&
        fast_vec_dist(&trigger_moby->pos, &target) < trigger_vars->radius) {
        inside = 1;
    }
    if (!inside && trigger_vars->path != -1) {
        path = ((s32 *)l05_paths[trigger_vars->path]);
        inside = point_in_path_xy(&target, path + 4, path[0]) != 0;
    }
    if (!inside) {
        for (i = 0; i < 6; i++) {
            if (trigger_vars->volumes[i] != -1 &&
                is_point_inside_clip_volume(&target, trigger_vars->volumes[i])) {
                inside = 1;
                break;
            }
        }
    }
    if ((inside && trigger_moby->command == 0) || (!inside && trigger_moby->command != 0) ||
        trigger_moby->state == 0) {
        if (inside) {
            trigger_moby->command = 1;
        } else {
            trigger_moby->command = 0;
        }
        trigger_moby->state = trigger_moby->command + 1;
        for (i = 0; i < 12; i++) {
            if (trigger_vars->on_lists[i] != -1) {
                apply_spatial_trigger_to_moby_list(
                    trigger_moby, trigger_vars, trigger_vars->on_lists[i], inside);
            }
        }
        for (i = 0; i < 11; i++) {
            if (trigger_vars->on_mobys[i] != -1) {
                apply_spatial_trigger_to_moby(trigger_vars->on_mobys[i], trigger_vars, inside);
            }
        }
    }
}
/* Moby update: plays an animation when its state and a flag allow, then calls the next stage. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_002F9478), where it is exact; names translated to the US level program. */

extern short D_L05_0015FFD8;
extern void  update_hydrodisplacer_faucet_pulse_color(void *) __asm__("FUN_L05_002f81b8");

void hydrodisplacer_faucet_update(char *m) __asm__("FUN_L05_002f8080");

void hydrodisplacer_faucet_update(char *faucet_moby) {
    enum {
        HYDRODISPLACER_FAUCET_ANIM_WAIT_FOR_LINK_FLAG_1 = 0,
        HYDRODISPLACER_FAUCET_ANIM_TRANSITION_TO_LINK_FLAG_2_WAIT = 1,
        HYDRODISPLACER_FAUCET_ANIM_WAIT_FOR_LINK_FLAG_2 = 2,
        HYDRODISPLACER_FAUCET_ANIM_TRANSITION_TO_LINK_FLAG_1_WAIT = 3
    };
    struct Hero *player_hero = &hero;
    int          hero_on_faucet_in_mode_13 = 0;
    int         *faucet_vars = ((int *)MOBY(faucet_moby)->pvars);
    if (((char *)player_hero->ground_moby) == faucet_moby) {
        hero_on_faucet_in_mode_13 = player_hero->state.control_mode == 0x13;
    }
    switch ((unsigned char)faucet_moby[0x53]) {
    case HYDRODISPLACER_FAUCET_ANIM_WAIT_FOR_LINK_FLAG_1:
        if (faucet_vars[0] >= 0 &&
            (*(unsigned char *)(*(int *)&D_L05_0015FFD8 + faucet_vars[0] * 256 + 0xBC) & 1) &&
            !hero_on_faucet_in_mode_13)
            blend_moby_animation(faucet_moby, 1, 0, 0);
        break;
    case HYDRODISPLACER_FAUCET_ANIM_WAIT_FOR_LINK_FLAG_2:
        if (faucet_vars[0] >= 0 &&
            (*(unsigned char *)(*(int *)&D_L05_0015FFD8 + faucet_vars[0] * 256 + 0xBC) & 2) &&
            !hero_on_faucet_in_mode_13)
            blend_moby_animation(faucet_moby, 3, 0, 0);
        break;
    case HYDRODISPLACER_FAUCET_ANIM_TRANSITION_TO_LINK_FLAG_1_WAIT:
        if (faucet_moby[0x70] & 2)
            blend_moby_animation(faucet_moby, 0, 0, 0);
        break;
    case HYDRODISPLACER_FAUCET_ANIM_TRANSITION_TO_LINK_FLAG_2_WAIT:
        if (faucet_moby[0x70] & 2)
            blend_moby_animation(faucet_moby, 2, 0, 0);
        break;
    }
    update_hydrodisplacer_faucet_pulse_color(faucet_moby);
}
/* eases the pulse level of the moby that holds the focus and mixes its colour from two palettes */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_002F95B0), where it is exact; names translated to the US level program. */

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern short faucet_base_color_gp __asm__("D_L05_001618C0") __attribute__((sda));
extern short faucet_item_16_pulse_start_color_gp __asm__("D_L05_001618C4") __attribute__((sda));
extern short faucet_item_16_pulse_end_color_gp __asm__("D_L05_001618C8") __attribute__((sda));
extern short faucet_fallback_pulse_start_color_gp __asm__("D_L05_001618CC") __attribute__((sda));
extern short faucet_fallback_pulse_end_color_gp __asm__("D_L05_001618D0") __attribute__((sda));
extern short D_0015EE6C_s __asm__("D_0015ED6C");

void update_hydrodisplacer_faucet_pulse_color(void *mv) {
    struct Moby *moby = mv;
    struct Hero *base = &hero;
    char        *data = ((char *)moby->pvars);
    int          pulse_color;
    float        pulse_phase;
    if (((char *)base->ground_moby) == moby) {
        approach_value((float *)(data + 0x1C), 1.0f, frame_time + frame_time);
        if (((int)base->items[0].moby) != 0) {
            if (base->items[0].item_id == 0x16)
                ((unsigned char *)moby)[0xBC] = 1;
            else
                ((unsigned char *)moby)[0xBC] = 0;
        } else {
            ((unsigned char *)moby)[0xBC] = 0;
        }
    } else {
        approach_value((float *)(data + 0x1C), 0.0f, *(float *)&D_0015EE6C_s);
    }
    if (*(float *)(data + 0x1C) != 0.0f) {
        pulse_phase = fast_add_rotations(*(float *)(data + 0x18), frame_time * 6.2831855f);
        *(float *)(data + 0x18) = pulse_phase;
        if (((unsigned char *)moby)[0xBC] == 1) {
            pulse_color = fast_tween_color(*(int *)&faucet_item_16_pulse_start_color_gp,
                                           *(int *)&faucet_item_16_pulse_end_color_gp,
                                           (fast_sin(pulse_phase) + 1.0f) * 0.5f);
        } else {
            pulse_color = fast_tween_color(*(int *)&faucet_fallback_pulse_start_color_gp,
                                           *(int *)&faucet_fallback_pulse_end_color_gp,
                                           (fast_sin(pulse_phase) + 1.0f) * 0.5f);
        }
        moby->glow_color =
            fast_tween_color(*(int *)&faucet_base_color_gp, pulse_color, *(float *)(data + 0x1C));
    } else {
        moby->glow_color = *(int *)&faucet_base_color_gp;
    }
}

#include "rnc/game/item_state.h"

extern void help_request(int, int) __asm__("FUN_L00_00203908");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern int  frame_number_gp __asm__("D_L05_0015F5CC") __attribute__((sda));
extern unsigned short D_00141938 NOT_SDA;
extern unsigned short D_00141C60 NOT_SDA;
extern int                       D_L05_0015F6A8;

#include "rnc/overlay/shared/props/hydrodisplacer_faucet_data/hoverboard_update.h"

extern int D_L05_0016190C __attribute__((sda));
extern int D_L05_00161910 __attribute__((sda));

void hoverboard_update(unsigned char *moby) __asm__("FUN_L05_002f87a8");

void hoverboard_update(unsigned char *moby) {
    enum {
        HOVERBOARD_STATE_WAIT_FOR_PLAYER_APPROACH = 0,
        HOVERBOARD_STATE_WAIT_FOR_CONTROL_MODE_EXIT = 1
    };
    char        *data = ((char *)MOBY(moby)->pvars);
    float        delta[4], position[4], effect_position[4], offset[4], second_position[4];
    float        angle, distance_scale, delta_length;
    int          state = (u8)moby[0x20];
    struct Hero *player_hero;

    switch (state) {
    case HOVERBOARD_STATE_WAIT_FOR_PLAYER_APPROACH: {
        char        *player_position = D_0013F3D0;
        struct Hero *player;
        if (fast_vec_dist(player_position, moby + 0x10) < 5.5f && frame_number_gp > 3 &&
            (player = (struct Hero *)(player_position - 0x80))->state.control_mode != 0x15 &&
            player->state.control_mode != 0x16) {
            player->hoverboard_moby = moby;
            hero_set_state(0x6b, 1);
            if (*(int *)(data + 0x50) == 0) {
                qcopy(data + 0x10, moby + 0x10);
            }
            moby[0x20] = 1;
            MOBY(moby)->update_dist = 0xff;
        }
        break;
    }
    case HOVERBOARD_STATE_WAIT_FOR_CONTROL_MODE_EXIT:
        if (*(int *)(data + 0x50) == 0 && alternate_item_available[0] != 0 && D_00141938 == 0 &&
            D_L05_0015F6A8 != 0 &&
            (player_hero = &hero, scale_game_frames(0x4b0) < player_hero->control_mode_timer) &&
            D_00141C60 == 0) {
            help_request(0x138f, 0x5f);
        }
        if (hero.state.control_mode != 0x16) {
            moby[0x30] = 0x10;
            if (*(int *)(data + 0x50) == 0) {
                qcopy(moby + 0x10, data + 0x10);
            }
            moby[0x20] = 0;
        }
        break;
    case 2:
        /* Fakematch: a dead test in state 2 gives retail's block layout. */
        if (*(int *)(data + 0x50) != 0)
            break;
        break;
    }
    {
        unsigned char *position_ptr = moby + 0x10;
        fast_vec_sub(delta, position_ptr, data);
        qcopy(data, position_ptr);
    }
    delta_length = fast_vec_length(delta);
    if ((unsigned char)moby[0xbc] != 2 && delta_length < 2.0f) {
        moby_point_position(moby, frame_number_gp & 1, position);
        angle = random_angle_radians();
        distance_scale = frame_time * 0.25f;
        fast_vec_normalize(effect_position, moby + 0xe0, distance_scale * fast_cos(angle));
        fast_vec_normalize(offset, moby + 0xd0, distance_scale * fast_sin(angle));
        fast_vec_add(effect_position, effect_position, delta);
        fast_vec_add(effect_position, effect_position, offset);
        qcopy(effect_position, delta);
        effect_position[3] = (float)hoverboard_effect_displacement_w_gp;
        if ((unsigned char)moby[0xbc] == 0) {
            int size = random_int_between(scale_game_frames(10), scale_game_frames(13));
            spawn_effect(
                position, effect_position, D_L05_0016190C, D_L05_00161910, size, 15, 15, 1);
            moby[0xbc] = 0;
            return;
        }
        fast_vec_scale(second_position, delta, 0.920f);
        {
            int size = random_int_between(scale_game_frames(15), scale_game_frames(18));
            spawn_effect(position, second_position, 0x5032f0d2, D_L05_00161910, size, 30, 20, 1);
        }
        {
            int size = random_int_between(scale_game_frames(8), scale_game_frames(17));
            spawn_burst(
                position, effect_position, D_L05_0016190C, D_L05_00161910, size, 1, 15000.0f);
        }
    }
    moby[0xbc] = 0;
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0FC8), where it is exact; names translated to the US level program. */

void add_moby_frames_for_group_proximity(char *moby) __asm__("FUN_L05_00307010");

void add_moby_frames_for_group_proximity(char *moby) {
    char        *data = ((char *)MOBY(moby)->pvars);
    struct Hero *player_hero = &hero;
    float        position_delta[4];
    char        *group_member_moby;
    char        *proximity_candidate_moby;
    char        *other;
    char        *od;

    if (player_hero->race_time_frames < scale_game_frames(0xB4)) {
        return;
    }
    if (!fast_dec_timer((int *)(data + 0x264))) {
        return;
    }
    other = ((char *)player_hero->hoverboard_moby);
    if (!other) {
        return;
    }
    od = *(char **)(other + 0x78);
    if (*(int *)(od + 0x44) != -1) {
        get_first_moby_in_group((int *)&group_member_moby, *(int *)(od + 0x44), 0, 0);
        while (group_member_moby) {
            if (MOBY(group_member_moby)->oclass == 0x473) {
                float offset_test_position[4];
                float m[16];
                char *candidate_moby = group_member_moby;
                qcopy(offset_test_position, data + 0x200);
                offset_test_position[2] += 0.8f;
                fast_vec_sub(position_delta, candidate_moby + 0x10, offset_test_position);
                if (!(8.0f < fast_vec_length(position_delta))) {
                    set_identity_matrix(m);
                    transpose_rotation_matrix(m, group_member_moby + 0xC0);
                    transform_vector(position_delta, position_delta, m);
                    if (absolute_float(position_delta[0]) < 0.5f &&
                        absolute_float(position_delta[1]) < 2.5f &&
                        absolute_float(position_delta[2]) < 2.5f) {
                        *(unsigned short *)(data + 0x25A) += scale_game_frames(0x78);
                        *(int *)(data + 0x264) = scale_game_frames(0x3C);
                    }
                }
            }
            get_next_moby_in_group((int *)&group_member_moby, group_member_moby, 0, 0);
        }
    }
    if (*(int *)(od + 0x48) != -1) {
        get_first_moby_in_group((int *)&proximity_candidate_moby, *(int *)(od + 0x48), 0, 0);
        while (proximity_candidate_moby) {
            if (MOBY(proximity_candidate_moby)->oclass == 0x474) {
                float m[16];
                fast_vec_sub(position_delta, proximity_candidate_moby + 0x10, data + 0x200);
                if (!(8.0f < fast_vec_length(position_delta))) {
                    set_identity_matrix(m);
                    transpose_rotation_matrix(m, proximity_candidate_moby + 0xC0);
                    transform_vector(position_delta, position_delta, m);
                    if (absolute_float(position_delta[0]) < 2.8f &&
                        absolute_float(position_delta[1]) < 1.4f &&
                        absolute_float(position_delta[2]) < 0.5f) {
                        *(unsigned short *)(data + 0x25A) += scale_game_frames(0x78);
                        *(int *)(data + 0x264) = scale_game_frames(0x3C);
                    }
                }
            }
            get_next_moby_in_group(
                (int *)&proximity_candidate_moby, proximity_candidate_moby, 0, 0);
        }
    }
}
/* UpdateMoby_823: pick random wobble parameters, then animate them each frame */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0030D500), where it is exact; names translated to the US level program. */

extern void FUN_L00_001fff28(void *, int, float);
void        draw_moby_wobble_quads(struct Moby *moby) __asm__("FUN_L05_0030c220");

extern void draw_moby_wobble_quads(struct Moby *m) __asm__("FUN_L05_0030c220");

void update_moby_wobble(unsigned char *moby) __asm__("FUN_L05_0030c0a8");

void update_moby_wobble(unsigned char *m) __asm__("FUN_L05_0030c0a8");

void update_moby_wobble(unsigned char *moby) {
    enum {
        MOBY_WOBBLE_STATE_INITIALIZE = 0,
        MOBY_WOBBLE_STATE_ANIMATE = 1
    };
    char *wobble_vars = ((char *)MOBY(moby)->pvars);
    float random_angular_sample, lo, hi;
    switch (moby[0x20]) {
    case MOBY_WOBBLE_STATE_INITIALIZE:
        *(float *)(wobble_vars + 0x40) = MOBY(moby)->rot.z;
        *(float *)(wobble_vars + 0x48) = random_angle_radians();
        random_angular_sample = random_angle_radians();
        lo = frame_time * 0.47123888f;
        hi = frame_time * 0.5235988f;
        *(float *)(wobble_vars + 0x44) = random_angular_sample;
        random_angular_sample = randf_sym(lo, hi);
        lo = frame_time * 0.20943951f;
        hi = frame_time * 0.2617994f;
        *(float *)(wobble_vars + 0x50) = random_angular_sample;
        *(float *)(wobble_vars + 0x4C) = randf_sym(lo, hi);
        set_moby_color(moby, 0x80, 0x80, 0x80);
        attach_manipulator((char *)moby, 0, wobble_vars);
        moby[0x20] = 1;
        break;
    case MOBY_WOBBLE_STATE_ANIMATE: {
        float k = 0.5235988f;
        float wobble_phase;
        add_draw_callback((void *)draw_moby_wobble_quads, moby);
        wobble_phase =
            fast_add_rotations(*(float *)(wobble_vars + 0x44), *(float *)(wobble_vars + 0x4C));
        *(float *)(wobble_vars + 0x44) = wobble_phase;
        MOBY(moby)->rot.z =
            fast_add_rotations(*(float *)(wobble_vars + 0x40), fast_sin(wobble_phase) * k);
        wobble_phase =
            fast_add_rotations(*(float *)(wobble_vars + 0x48), *(float *)(wobble_vars + 0x50));
        *(float *)(wobble_vars + 0x48) = wobble_phase;
        FUN_L00_001fff28(wobble_vars + 0x10, 1, fast_sin(wobble_phase) * k + k);
        break;
    }
    }
}

extern void  draw_geometry_quad(void *, float *, int) __asm__("FUN_001f7d30");
extern float wobble_quad_texcoords[][2] __asm__("D_L05_001D67B0");
extern float wobble_quad_a_base_vertices[][4] __asm__("D_L05_001D67D0");
extern float wobble_quad_b_base_vertices[][4] __asm__("D_L05_001D6810");
extern float D_L05_001671C0_30c220[] __asm__("D_L05_001671C0");

#include "rnc/overlay/shared/props/hydrodisplacer_faucet_globals.h"

extern int wobble_quad_blend_a_selector_gp __asm__("D_L05_00161CF4") __attribute__((sda));
extern int wobble_quad_blend_b_selector_gp __asm__("D_L05_00161CF8") __attribute__((sda));
extern int wobble_quad_blend_c_selector_gp __asm__("D_L05_00161CFC") __attribute__((sda));
extern int wobble_quad_blend_d_selector_gp __asm__("D_L05_00161D00") __attribute__((sda));
extern int wobble_quad_start_color_gp __asm__("D_L05_00161D04") __attribute__((sda));
extern int wobble_quad_end_color_gp __asm__("D_L05_00161D08") __attribute__((sda));
extern int wobble_quad_texture_index_gp __asm__("D_L05_00161D0C") __attribute__((sda));

typedef struct {
    float vertex[4][4];
    int   color[4];
    float texcoord[4][2];
    u64   control[4];
} Quad;

void draw_moby_wobble_quads(struct Moby *moby) {
    Quad  quad_a, quad_b;
    float rotation[4];
    float position[4];
    /* Draw helper reads four consecutive vectors: side, direction, up, position. */
    float  matrix[16] __attribute__((aligned(16)));
    float *side;
    float *direction;
    float *up;
    u64    texture;
    u64    flags;
    float  phase;
    float  sign;
    float  pass;
    float  offset_a_x, offset_a_z, offset_b_x, offset_b_z;
    int    i, j, k;
    float *vertex_a, *vertex_b;
    char  *vars = (char *)moby->pvars;

    moby_point_position(moby, 0, position);
    clear_u64_value(rotation);
    rotation[2] = moby->rot.z;
    rotation[1] = fast_sin(*(float *)(vars + 0x48)) * 0.5235988f + 0.5235988f;
    side = matrix;
    euler_rotation_matrix(side, rotation);
    direction = matrix + 4;
    fast_vec_sub(direction, D_L05_001671C0_30c220, position);
    fast_vec_normalize(direction, direction, 1.0f);
    up = matrix + 8;
    fast_vec_cross(side, direction, up);
    fast_vec_cross(direction, up, side);
    qcopy(matrix + 12, position);

    texture = get_effect_texture(wobble_quad_texture_index_gp);
    flags = (u64)(s64)wobble_quad_blend_a_selector_gp |
            ((u64)(s64)wobble_quad_blend_b_selector_gp << 2) |
            ((u64)(s64)wobble_quad_blend_c_selector_gp << 4) |
            ((u64)(s64)wobble_quad_blend_d_selector_gp << 6) | 0x8000000000ULL;
    quad_b.control[0] = 0;
    quad_b.control[2] = 0xff9000000260ULL;
    quad_a.control[2] = 0xff9000000260ULL;
    quad_b.control[1] = texture;
    quad_a.control[1] = texture;
    quad_a.control[0] = 0;
    quad_b.control[3] = flags;
    quad_a.control[3] = flags;
    for (k = 0; k < 4; k++) {
        float u = wobble_quad_texcoords[k][0];
        float v = wobble_quad_texcoords[k][1];
        quad_a.texcoord[k][0] = u;
        quad_a.texcoord[k][1] = v;
        quad_b.texcoord[k][0] = u;
        quad_b.texcoord[k][1] = v;
    }
    for (i = 0; i < 10; i++) {
        pass = (float)i;
        j = 0;
        do {
            vertex_a = quad_a.vertex[j];
            qcopy(vertex_a, wobble_quad_a_base_vertices[j]);
            qcopy(quad_b.vertex[j], wobble_quad_b_base_vertices[j]);
            phase = (float)(i + (j & 1)) * 0.1f;
            sign = 1.0f;
            if (j > 1)
                sign = -1.0f;
            quad_a.color[j] = quad_b.color[j] =
                fast_tween_color(wobble_quad_start_color_gp, wobble_quad_end_color_gp, phase);
            offset_a_x = phase * 8.0f * sign;
            offset_a_z = pass * 10.0f;
            offset_b_x = phase * 6.0f * sign;
            offset_b_z = pass * 12.0f;
            vertex_a[0] += offset_a_x;
            vertex_a[2] += offset_a_z;
            vertex_b = quad_b.vertex[j];
            vertex_b[0] += offset_b_x;
            vertex_b[2] += offset_b_z;
        } while (++j < 4);
        draw_geometry_quad(&quad_a, matrix, 0);
        draw_geometry_quad(&quad_b, matrix, 0);
    }
}

#include "rnc/overlay/level_globals.h"

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} SaveFlags;

extern SaveFlags     D_L05_001BB6B0;
extern SaveFlags     D_L05_001BAA50_s __asm__("D_L05_001BAA50");
extern unsigned char D_L05_0015FC88[] MACRO_ADDR;
extern u8            D_0014C050[];

void floor_button_update(struct Moby *moby) __asm__("FUN_L05_0030c5b0");

void floor_button_update(struct Moby *moby) {
    s32  *vars = (s32 *)moby->pvars;
    s16   id;
    s32   i;
    s32   list;
    char *entry;
    f32   angle;
    s32   color;
    u32   shade;
    u32   high;

    switch (moby->state) {
    case 0:
        *(f32 *)(vars + 3) = random_angle_radians();
        moby->pos.z -= 0.35f;
        id = (s16)moby->save_id;
        if (D_L05_001BB6B0.collected[id] != 0 ||
            (D_0014C190_257218[current_level_index][id >> 5] >> (moby->save_id & 31) & 1) != 0 ||
            (vars[2] != 0 && D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
            moby->command = 2;
            moby->glow_color = 0x80208020;
            moby->state = 2;
            list = vars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)l05_paths[list] > 0) {
                    do {
                        entry = ((char *)l05_paths[list]) + i * 16;
                        if (*(f32 *)(entry + 0x1C) == ((f32 *)vars)[1])
                            *(s32 *)(entry + 0x1C) = 0;
                        list = vars[0];
                        i++;
                    } while (i < *(s32 *)l05_paths[list]);
                }
            }
        } else {
            moby->state = 1;
        }
        break;
    case 1:
        angle = fast_add_rotations(*(f32 *)(vars + 3), frame_time * 6.2831855f);
        *(f32 *)(vars + 3) = angle;
        color = truncate_float_to_s32((fast_sin(angle) * 4.0f - 3.0f) * 128.0f);
        if (color > 0x80)
            color = 0x80;
        else if (color < 0x20)
            color = 0x20;
        shade = 0x80000000 | (color << 8);
        high = color << 16;
        moby->glow_color = high | shade | color;
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            D_L05_001BAA50_s.collected[(s16)moby->save_id] = moby->mission + 2;
            if (moby->mission == 0xFF ||
                (D_L05_0015FC88[moby->mission] != 0xFF &&
                 D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
                D_L05_001BB6B0.collected[(s16)moby->save_id] = moby->mission + 2;
            }
            moby->state = 2;
            moby->command = 1;
            moby->glow_color = 0x80208020;
            allocate_voice_for_target_entry(0, 0, moby);
            list = vars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)l05_paths[list] > 0) {
                    do {
                        entry = ((char *)l05_paths[list]) + i * 16;
                        if (*(f32 *)(entry + 0x1C) == ((f32 *)vars)[1])
                            *(s32 *)(entry + 0x1C) = 0;
                        list = vars[0];
                        i++;
                    } while (i < *(s32 *)l05_paths[list]);
                }
            }
        }
        break;
    }
}

typedef struct {
    char  pad0[0x44];
    short count;
    char  pad46[0x132];
    char *mobys[1];
} ML;

extern ML            D_L05_0016CC60;
extern int           D_L05_0015F5C4;
extern unsigned char D_L05_0017C8A0[];
extern short         big_head_manipulator_scale_gp __asm__("D_L05_00161EE8") __attribute__((sda));

void attach_big_head_cheat_manipulator(char *moby) __asm__("FUN_L05_00317398");

void attach_big_head_cheat_manipulator(char *moby) {
    int i;

    if (D_L05_0015F5C4 == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < D_L05_0016CC60.count; i++) {
            char *candidate_moby = D_L05_0016CC60.mobys[i];

            if (MOBY(candidate_moby)->oclass == MOBY(moby)->oclass) {
                unsigned char *manipulator = D_L05_0017C8A0;

                if (manipulator[1] == 0) {
                    attach_manipulator(candidate_moby, 0, manipulator);
                    *(float *)(manipulator + 0x20) = *(float *)&big_head_manipulator_scale_gp;
                    *(float *)(manipulator + 0x24) = *(float *)&big_head_manipulator_scale_gp;
                    *(float *)(manipulator + 0x28) = *(float *)&big_head_manipulator_scale_gp;
                }
            }
        }
    }
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_00319B58), where it is exact; names translated to the US level program. */

typedef struct {
    int   id;
    char  pad0[0x1C];
    float s[3];
} Dat;

void initialize_moby_launch_motion(char *moby,
                                   int   unused_source_moby,
                                   void *initial_motion_step,
                                   void *local_rotation_offset,
                                   int   lifetime_frames) __asm__("FUN_L05_00318648");

void initialize_moby_launch_motion(char *moby,
                                   int   unused_source_moby,
                                   void *initial_motion_step,
                                   void *local_rotation_offset,

                                   int lifetime_frames) {
    Dat *moby_vars = ((Dat *)MOBY(moby)->pvars);
    moby[0x20] = 1;
    qcopy((char *)moby_vars + 0x30, initial_motion_step);
    qcopy((char *)moby_vars + 0x10, local_rotation_offset);
    moby_vars->id = scale_game_frames(lifetime_frames);
    moby_vars->s[0] = random_float_between(-(frame_time * 4.1887903f), frame_time * 4.1887903f);
    moby_vars->s[1] = random_float_between(-(frame_time * 4.1887903f), frame_time * 4.1887903f);
    moby_vars->s[2] = random_float_between(-(frame_time * 4.1887903f), frame_time * 4.1887903f);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_00319C08), where it is exact; names translated to the US level program. */

extern char D_L05_00174240[];
extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

void blarg_gun_update(char *moby) __asm__("FUN_L05_003186f8");

void blarg_gun_update(char *moby) {
    enum {
        BLARG_GUN_STATE_INITIALIZE = 0,
        BLARG_GUN_STATE_MOVING = 1
    };
    char *data = ((char *)MOBY(moby)->pvars);
    char  rotation_workspace[0x70];
    char *hs;
    int   joint_index;
    float fade_decrement;

    switch ((unsigned char)moby[0x20]) {
    case BLARG_GUN_STATE_INITIALIZE:
        if (((unsigned short)MOBY(moby)->draw_dist) & 1)
            MOBY(moby)->flags |= 0x8000;
        moby[0x20] = 2;
        break;
    case BLARG_GUN_STATE_MOVING:
        joint_index = *(int *)(data + 4);
        if (joint_index > 0) {
            moby_point_position(moby, joint_index, rotation_workspace + 0x40);
            MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, *(float *)(data + 0x20));
            MOBY(moby)->rot.y = fast_add_rotations(MOBY(moby)->rot.y, *(float *)(data + 0x24));
            MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, *(float *)(data + 0x28));
            euler_rotation_matrix(rotation_workspace, moby + 0x40);
            copy_matrix3x4(moby + 0xC0, rotation_workspace);
            moby_point_position(moby, *(int *)(data + 4), rotation_workspace + 0x50);
            fast_vec_sub(
                rotation_workspace + 0x60, rotation_workspace + 0x40, rotation_workspace + 0x50);
        } else {
            transform_vector_by_basis(rotation_workspace + 0x40, data + 0x10, moby + 0xC0);
            MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, *(float *)(data + 0x20));
            MOBY(moby)->rot.y = fast_add_rotations(MOBY(moby)->rot.y, *(float *)(data + 0x24));
            MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, *(float *)(data + 0x28));
            euler_rotation_matrix(rotation_workspace, moby + 0x40);
            if (MOBY(moby)->flags & 0x8000)
                fast_vec_scale(rotation_workspace + 0x10, rotation_workspace + 0x10, -1.0f);
            copy_matrix3x4(moby + 0xC0, rotation_workspace);
            transform_vector_by_basis(rotation_workspace + 0x50, data + 0x10, moby + 0xC0);
            fast_vec_sub(
                rotation_workspace + 0x60, rotation_workspace + 0x40, rotation_workspace + 0x50);
            fast_vec_add(rotation_workspace + 0x50, rotation_workspace + 0x50, moby + 0x10);
        }

        if (coll_sphere(convert_integer_to_float(*(int *)(data + 0xC)) * 0.0009765625f,
                        rotation_workspace + 0x50,
                        0,
                        moby)) {
            hs = D_L05_00174240;
            if (*(int *)(hs + 0x18) == 0 || *(int *)(hs + 0x18) != *(int *)(data + 8)) {
                *(int *)data = 0;
                if (fast_vec_dot(data + 0x30, hs + 0x40) < 0.0f) {
                    reflect_vector(data + 0x30, data + 0x30, hs + 0x40);
                    fast_vec_scale(data + 0x30, data + 0x30, 0.5f);
                }
            }
        }
        if (fast_dec_timer((int *)data)) {
            fade_decrement = 128.0f / (float)scale_game_frames(20);
            *(int *)data = 0;
            if (fade_decrement <= (float)((unsigned char *)moby)[0x23]) {
                ((unsigned char *)moby)[0x23] = ((unsigned char *)moby)[0x23] - (int)fade_decrement;
            } else {
                mark_moby_for_removal(moby);
            }
        }
        *(float *)(data + 0x38) = *(float *)(data + 0x38) - frame_time_sq * 10.8f;
        fast_vec_add(moby + 0x10, moby + 0x10, data + 0x30);
        fast_vec_add(moby + 0x10, moby + 0x10, rotation_workspace + 0x60);
        if (MOBY(moby)->pos.z < 5.0f)
            mark_moby_for_removal(moby);
        break;
    }
}

extern char *func_L05_0031AAA8_19510(void *, int) __asm__("FUN_L05_00319598");
extern void  initialize_taxi_stop_state(void *) __asm__("FUN_L05_00319208");

#include "rnc/overlay/cuboid.h"

void reset_selected_moby_to_current_placement(void *moby_v) __asm__("FUN_L05_00319510");

void reset_selected_moby_to_current_placement(void *moby_v) {
    struct Moby *moby = moby_v;
    char        *data = ((char *)moby->pvars);
    char        *spawn = func_L05_0031AAA8_19510(moby, *(short *)(data + 0xB6));
    char        *position;
    if (spawn != 0) {
        int   index;
        char *slot = data;
        char *entry;
        position = spawn + 0x10;
        slot += *(short *)(data + 0xB4) << 2;
        index = *(int *)(slot + 0x80) << 7;
        entry = (char *)l05_cuboids;
        qcopy(position, (void *)(index + (int)entry + 0x30));
        qcopy(spawn + 0x40, (void *)(index + (int)entry + 0x70));
        initialize_taxi_stop_state(spawn);
    }
    *(short *)(data + 0xB4) = *(unsigned short *)(data + 0xB6);
}

/* Finds the pool moby in this moby's class list whose data id at 0xB4 matches. */
extern unsigned short *D_L05_001ABCC0_9598[] __asm__("D_L05_001ABCC0");
extern char           *D_L05_0015FFD8_9598 __asm__("D_L05_0015FFD8");

typedef struct {
    char  pad[0x78];
    char *data;
    char  pad2[0x84];
} M_9598;

char *find_moby_in_owner_list_by_id(unsigned char *list_owner_moby,
                                    int            id) __asm__("FUN_L05_00319598");

char *find_moby_in_owner_list_by_id(unsigned char *list_owner_moby, int id) {
    unsigned short *moby_index_entry = D_L05_001ABCC0_9598[list_owner_moby[0x21]];
    char           *pool;
    int             idx;
    if (moby_index_entry != 0) {
        pool = D_L05_0015FFD8_9598;
        do {
            idx = *moby_index_entry & 0x7FFF;
            if (*(short *)(((M_9598 *)pool)[idx].data + 0xB4) == id)
                goto found;
        } while ((short)*moby_index_entry++ >= 0);
    }
    return 0;
found:
    return (char *)&((M_9598 *)D_L05_0015FFD8_9598)[idx];
}

/* Advances a moby's path: steps the path position and returns whether it has reached the end. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031AB08), where it is exact; names translated to the US level program. */

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern void sample_camera_path(int *, int, void *, float *, int, float) __asm__("FUN_00214e58");

int advance_moby_path(char *moby) __asm__("FUN_L05_003195f8");

int advance_moby_path(char *moby) {
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    int  *path;
    advance_accelerated_scalar(1.0f,
                               *(float *)(moby_vars + 0xD8),
                               *(float *)(moby_vars + 0xD8),
                               *(float *)(moby_vars + 0xD4),
                               (float *)(moby_vars + 0xC0),
                               (float *)(moby_vars + 0xC4));
    path = *(int **)(moby_vars + 0xAC);
    sample_camera_path(path,
                       0,
                       moby_vars + 0x60,
                       (float *)(moby_vars + 0x70),
                       0,
                       *(float *)(moby_vars + 0xC0) * (float)(*path - 1));
    return *(float *)(moby_vars + 0xC0) >= 1.0f;
}
/* Plays the sound that goes with the moby's current variant. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031ABA0), where it is exact; names translated to the US level program. */

void request_moby_variant_help_message(char *moby) __asm__("FUN_L05_00319690");

void request_moby_variant_help_message(char *moby) {
    switch (*(short *)(((char *)MOBY(moby)->pvars) + 0xB6)) {
    case 0:
        try_set_help_message(8, 0x13A5);
        break;
    case 1:
        try_set_help_message(8, 0x1398);
        break;
    case 2:
        try_set_help_message(8, 0x139A);
        break;
    case 3:
        try_set_help_message(8, 0x139C);
        break;
    case 4:
        try_set_help_message(8, 0x1399);
        break;
    default:
        try_set_help_message(8, 0x139B);
        break;
    }
}

#include "qcopy.h"

/* Wobbles a moby's position and rotation from its sine-driven state. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031AC50), where it is exact; names translated to the US level program. */

void apply_moby_position_and_rotation_wobble(char *moby) __asm__("FUN_L05_00319740");

void apply_moby_position_and_rotation_wobble(char *moby) {
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    float wobble_phase, wobble_result, wobble_phase_step;
    qcopy(moby + 0x10, moby_vars + 0x60);
    qcopy(moby + 0x40, moby_vars + 0x70);
    wobble_phase = fast_add_rotations(*(float *)(moby_vars + 0xC8), frame_time * 2.0943951f);
    *(float *)(moby_vars + 0xC8) = wobble_phase;
    wobble_result = fast_sin(wobble_phase);
    wobble_phase_step = frame_time * 1.6929693f;
    MOBY(moby)->pos.z += wobble_result * 0.25f;
    wobble_phase = fast_add_rotations(*(float *)(moby_vars + 0xCC), wobble_phase_step);
    *(float *)(moby_vars + 0xCC) = wobble_phase;
    wobble_result = fast_add_rotations(MOBY(moby)->rot.y, fast_sin(wobble_phase) * 0.08726646f);
    wobble_phase_step = frame_time * 0.9250245f;
    MOBY(moby)->rot.y = wobble_result;
    wobble_phase = fast_add_rotations(*(float *)(moby_vars + 0xD0), wobble_phase_step);
    *(float *)(moby_vars + 0xD0) = wobble_phase;
    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, fast_sin(wobble_phase) * 0.08726646f);
    if (MOBY(moby)->rot.x > 0.17453292f)
        MOBY(moby)->rot.x = 0.17453292f;
    else if (MOBY(moby)->rot.x < -0.17453292f)
        MOBY(moby)->rot.x = -0.17453292f;
    if (MOBY(moby)->rot.y > 0.17453292f)
        MOBY(moby)->rot.y = 0.17453292f;
    else if (MOBY(moby)->rot.y < -0.17453292f)
        MOBY(moby)->rot.y = -0.17453292f;
}
/* Transforms the 7 vectors of a level table entry into the moby's space and records its index. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031ADF8), where it is exact; names translated to the US level program. */

extern char moby_vector_table_local_vectors[] __asm__("D_L05_00215C00");

void transform_moby_vector_table_to_world_space(char *moby) __asm__("FUN_L05_003198e8");

void transform_moby_vector_table_to_world_space(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    int   vector_table_index = *(int *)(data + 0xDC);
    if (vector_table_index != -1) {
        char *vector_table_entry = ((char *)l05_paths[vector_table_index]);
        if (*(int *)vector_table_entry == 7) {
            int          i;
            struct Hero *player_hero;
            for (i = 0; i < 7; i++) {
                transform_vector_by_basis(vector_table_entry + 0x10 + i * 16,
                                          moby_vector_table_local_vectors + i * 16,
                                          moby + 0xC0);
                fast_vec_add(vector_table_entry + 0x10 + i * 16,
                             vector_table_entry + 0x10 + i * 16,
                             moby + 0x10);
            }
            player_hero = &hero;
            *(unsigned short *)&player_hero->path_constrain_pending =
                *(unsigned short *)(data + 0xDC);
        }
    }
}

short               D_L07_00161870 = -32640;
short               D_L07_00161874 = -32640;
short               D_L07_00161878 = -16288;
short               D_L07_0016187C = -32640;
short               D_L07_00161880 = 0x60c0;
int                 D_L07_00161A98 = 0;
int                 D_L07_00161A9C = 2;
int                 D_L07_00161AA0 = 0;
int                 D_L07_00161AA4 = 1;
int                 D_L07_00161AA8 = 0x40807070;
int                 D_L07_00161AAC = 0x804040;
int                 D_L07_00161AB0 = 0x13;
int *D_L09_001AB9C0 NOT_SDA = 0;
short               D_L11_00161944 = -32640;
short               D_L11_00161948 = -32640;
short               D_L11_0016194C = -16288;
short               D_L11_00161950 = -32640;
short               D_L11_00161954 = 0x60c0;
short               D_L11_001613C0 = 0x6666;
short               D_L12_001619B8 = -32640;
short               D_L12_001619BC = -32640;
short               D_L12_001619C0 = -16288;
short               D_L12_001619C4 = -32640;
short               D_L12_001619C8 = 0x60c0;
int                 D_L16_0015F6A8 = 0;
int                 D_L16_00161968 = 0x4f0090f0;
int                 D_L16_0016196C = 0x4f0000f0;
unsigned char       D_L17_0015FC88 = 0;
short               D_L18_00161910 = -32640;
short               D_L18_00161914 = -32640;
short               D_L18_00161918 = -16288;
short               D_L18_0016191C = -32640;
short               D_L18_00161920 = 0x60c0;

short faucet_base_color_gp __asm__("D_L05_001618C0") = -32640;

int wobble_quad_end_color_gp __asm__("D_L05_00161D08") = 0x804040;

int wobble_quad_texture_index_gp __asm__("D_L05_00161D0C") = 0x13;
