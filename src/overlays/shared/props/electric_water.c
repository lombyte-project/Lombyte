/* Shared moby class electric_water: the electrified water, its countdown, bolt effect and switch reset. */
#include "rnc/hero/hero.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "types.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/enemies/trooper_play_sound.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/ui/frames/draw_ui_frame.h"
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D7C00.c: func_L15_002D7C00). */

typedef struct {
    int           switches[4];
    short         on_timer;
    short         off_timer;
    int           voice0;
    float         off_seconds;
    int           shock_count;
    int           water_areas[12];
    float         saved_fog[4];
    int           water_level_areas[4];
    int           reset_area_a;
    int           lamps[3];
    int           voice1;
    struct Moby  *owner;
    unsigned char saved_fog_color[4];
    int           armed;
    int           spark_lines[3][8];
    u128          bolt_start;
    u128          bolt_end;
    u128          bolt_aim;
    int           light;
    int           active_area;
    int           reset_area_b;
} ElectricWaterVars;

typedef struct {
    float          v[4];
    struct Moby   *attacker;
    int            flags;
    unsigned char  unk18;
    unsigned char  unk19;
    unsigned short attacker_class;
    float          damage;
    char           reserved[16];
} HitRecord;

typedef struct {
    float unk0;
    float unk4;
    float unk8;
    int   unkC;
    Vec4  pos; /* w: radius */
} Light29FF90;

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V2d8e48;

/* Fakematch: a second name for the hero position in the state 3 reset test keeps that address out of the reload register rotation. */
extern char         D_0013F3D0_lui[] __asm__("D_0013F3D0") NOT_SDA;
extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");
extern char        *D_L15_001600EC;
extern int          current_level_index_gp __asm__("D_0015ED84") __attribute__((sda));

#include "rnc/overlay/shared/props/electric_water_globals.h"

extern float electric_water_on_duration_seconds_gp __asm__("D_L15_00161BC8") __attribute__((sda));
extern float electric_water_off_duration_seconds_gp __asm__("D_L15_00161BCC") __attribute__((sda));
extern unsigned char D_L15_001BB024[];
extern unsigned char D_L15_001BBC84[];
extern int           D_L15_001BAAD0[];
extern V2d8e48       D_L15_001673C0 __asm__("D_L15_001673C0") NOT_SDA;
extern Light29FF90   D_L15_001808C0[];
extern unsigned char l15_fog_color[4] __asm__("D_L15_0015F444");
extern float         D_L15_0015F448;
extern float         D_L15_0015F44C;
extern float         D_L15_0015F450;
extern float         D_L15_0015F454;
extern float         scale_time(float) __asm__("FUN_001f96b0");
extern int           hero_is_near_ground(void) __asm__("FUN_L15_002d6770");
extern int           random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int           random_integer_below(int) __asm__("FUN_00213260");
extern int
allocate_colored_effect_slot(void *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
void       reset_electric_water_switch(char *moby) __asm__("FUN_L15_002d95a0");
void       initialize_electric_water_bolt_effect(char *moby) __asm__("FUN_L15_002d79e8");

void initialize_electric_water_bolt_effect(char *m) __asm__("FUN_L15_002d79e8");
void update_electric_water_bolt_effect(struct Moby *moby) __asm__("FUN_L15_002d7bb0");
void draw_electric_water_bolt_effect(char *moby) __asm__("FUN_L15_002d8710");

void draw_electric_water_countdown(char *moby) __asm__("FUN_L15_002d77c0");

extern void  add_draw_callback(void *, void *) __asm__("FUN_001f4600");
extern f32   random_float_between(f32, f32) __asm__("func_002132A8");
extern f32   random_angle_radians(void) __asm__("func_00213308");
extern f32   fast_cos(f32) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void  record_moby_hit(struct Moby *moby, void *hit) __asm__("FUN_L00_00259a88");
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int   test_linked_moby_turn_trigger(void *) __asm__("FUN_L11_0030f270");
extern int   hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");

void electric_water_update(struct Moby *moby) __asm__("FUN_L15_002d6810");

/* Moby class 655 */
void electric_water_update(struct Moby *moby) {
    enum {
        ELECTRIC_WATER_INITIALIZE = 0,
        ELECTRIC_WATER_ON = 1,
        ELECTRIC_WATER_OFF = 2,
        ELECTRIC_WATER_SWITCH_DISABLED = 3
    };
    float              unused_vector_argument[4];
    HitRecord          hit;
    ElectricWaterVars *electric_water_controller_vars = (ElectricWaterVars *)moby->pvars;
    int                armed = 1;
    int                wet, inside, remaining, brightness, length, j;
    int                reset_sw, area, dim_lamp, lamp, line_offset, near_lamp, sw, i, level_area;
    int               *best, *line;
    struct Moby       *nearest;
    unsigned short     save_id;
    float              distance, theta, phi, radius;

    if (moby->state != 0 &&
        is_point_inside_clip_volume(D_0013F3D0, electric_water_controller_vars->active_area) == 0) {
        if (electric_water_controller_vars->owner != 0) {
            if (electric_water_controller_vars->voice0 != -1) {
                VoicePoolWindow *voice =
                    (VoicePoolWindow *)((unsigned char *)&voice_pool +
                                        electric_water_controller_vars->voice0 * 0x70);
                if ((struct Moby *)voice->voice.owner == electric_water_controller_vars->owner &&
                    voice->voice.state != 0) {
                    release_voice_slot(electric_water_controller_vars->voice0);
                }
            }
            electric_water_controller_vars->voice0 = -1;
            if (electric_water_controller_vars->voice1 != -1) {
                VoicePoolWindow *voice =
                    (VoicePoolWindow *)((unsigned char *)&voice_pool +
                                        electric_water_controller_vars->voice1 * 0x70);
                if ((struct Moby *)voice->voice.owner == electric_water_controller_vars->owner &&
                    voice->voice.state != 0) {
                    release_voice_slot(electric_water_controller_vars->voice1);
                }
            }
            electric_water_controller_vars->voice1 = -1;
        }
        if (electric_water_controller_vars->light != -1) {
            release_point_light_slot(electric_water_controller_vars->light);
            electric_water_controller_vars->light = -1;
        }
        return;
    }

    switch (moby->state) {
    case ELECTRIC_WATER_INITIALIZE:
        moby->update_dist = 0xFF;
        for (reset_sw = 0; reset_sw < 4; reset_sw++) {
            if (electric_water_controller_vars->switches[reset_sw] == -1) {
                continue;
            }
            if (l15_moby_array[electric_water_controller_vars->switches[reset_sw]].oclass ==
                0x33E) {
                D_L15_001BB024
                [(short)l15_moby_array[electric_water_controller_vars->switches[reset_sw]]
                        .save_id] = 0;
                D_L15_001BBC84
                [(short)l15_moby_array[electric_water_controller_vars->switches[reset_sw]]
                        .save_id] = 0;
                save_id =
                    l15_moby_array[electric_water_controller_vars->switches[reset_sw]].save_id;
                D_0014C190_257218[current_level_index_gp][(short)save_id >> 5] &=
                    ~(1 << (save_id & 0x1F));
                save_id =
                    l15_moby_array[electric_water_controller_vars->switches[reset_sw]].save_id;
                D_L15_001BAAD0[(short)save_id >> 5] &= ~(1 << (save_id & 0x1F));
            } else {
                reset_electric_water_switch(
                    (char *)&l15_moby_array[electric_water_controller_vars->switches[reset_sw]]);
            }
        }
        electric_water_controller_vars->on_timer =
            truncate_float_to_s32(scale_time(electric_water_on_duration_seconds_gp * 60.0f));
        electric_water_controller_vars->off_timer = scale_game_frames(200);
        electric_water_controller_vars->shock_count = 0;
        electric_water_controller_vars->light = -1;
        moby->state = 1;
        electric_water_controller_vars->voice0 = -1;
        electric_water_controller_vars->voice1 = -1;
        break;

    case ELECTRIC_WATER_ON:
        inside = 0;
        wet = hero_is_near_ground();
        if (fast_dec_timer_s16(&electric_water_controller_vars->on_timer) != 0) {
            electric_water_controller_vars->off_timer =
                truncate_float_to_s32(scale_time(electric_water_off_duration_seconds_gp * 60.0f));
            moby->state = 2;
            if (electric_water_controller_vars->owner != 0) {
                if (electric_water_controller_vars->voice1 != -1) {
                    VoicePoolWindow *voice =
                        (VoicePoolWindow *)((unsigned char *)&voice_pool +
                                            electric_water_controller_vars->voice1 * 0x70);
                    if ((struct Moby *)voice->voice.owner ==
                            electric_water_controller_vars->owner &&
                        voice->voice.state != 0) {
                        release_voice_slot(electric_water_controller_vars->voice1);
                    }
                }
                electric_water_controller_vars->voice1 = -1;
            }
        }
        for (area = 0; area < 12; area++) {
            if (is_point_inside_clip_volume(
                    D_0013F3D0, electric_water_controller_vars->water_areas[area]) != 0) {
                inside = 1;
                break;
            }
        }
        if (moby->state == 2) {
            for (dim_lamp = 0; dim_lamp < 3; dim_lamp++) {
                if (electric_water_controller_vars->lamps[dim_lamp] != -1) {
                    set_moby_color(&l15_moby_array[electric_water_controller_vars->lamps[dim_lamp]],
                                   0x40,
                                   0x40,
                                   0x40);
                }
            }
        } else {
            for (lamp = 0; lamp < 3; lamp++) {
                brightness = random_int_between(0x40, 0x7F);
                if (electric_water_controller_vars->lamps[lamp] != -1) {
                    set_moby_color(&l15_moby_array[electric_water_controller_vars->lamps[lamp]],
                                   brightness,
                                   brightness,
                                   brightness);
                }
            }
        }

        if ((!inside || !wet) && random_integer_below(1) == 0 &&
            electric_water_path_arc_inhibit_gp == 0.0f) {
            best = 0;
            for (line_offset = 0; line_offset < 24; line_offset += 8) {
                line = electric_water_controller_vars->spark_lines[0] + line_offset;
                if (line[0] == -1) {
                    continue;
                }
                if (best != 0) {
                    distance = fast_vec_dist((D_L15_001600EC + (best[0] << 7)) + 0x30, D_0013F3D0);
                    if (!(fast_vec_dist((D_L15_001600EC + (line[0] << 7)) + 0x30, D_0013F3D0) <
                          distance)) {
                        continue;
                    }
                }
                best = line;
            }
            if (best != 0) {
                length = 8;
                for (j = 0; j < 8; j++) {
                    if (best[j] == -1) {
                        length = j;
                        break;
                    }
                }
                if (length >= 2) {
                    int pick = random_int_between(0, length - 2);
                    electric_water_controller_vars->bolt_start =
                        *(u128 *)((D_L15_001600EC + best[pick] * 128) + 0x30);
                    electric_water_controller_vars->bolt_aim =
                        electric_water_controller_vars->bolt_end =
                            *(u128 *)((D_L15_001600EC + best[pick + 1] * 128) + 0x30);
                    if (electric_water_controller_vars->light == -1) {
                        electric_water_controller_vars->light = allocate_colored_effect_slot(
                            &electric_water_controller_vars->bolt_start,
                            8.0f,
                            0.0f,
                            1.0f,
                            1.0f,
                            0.5f);
                    } else {
                        Light29FF90 *light = &D_L15_001808C0[electric_water_controller_vars->light];
                        qcopy(&light->pos, &electric_water_controller_vars->bolt_start);
                        light->unk0 = 1.0f;
                        light->pos.f[3] = 8.0f;
                        light->unk4 = 1.0f;
                        light->unk8 = 0.5f;
                        light->unkC = 0;
                    }
                    initialize_electric_water_bolt_effect((char *)moby);
                    update_electric_water_bolt_effect(moby);
                    add_draw_callback(draw_electric_water_bolt_effect, moby);
                }
            }
        }

        if (inside && wet) {
            if (electric_water_controller_vars->owner != 0 &&
                electric_water_controller_vars->owner != hero.moby) {
                if (electric_water_controller_vars->voice0 != -1) {
                    VoicePoolWindow *voice =
                        (VoicePoolWindow *)((unsigned char *)&voice_pool +
                                            electric_water_controller_vars->voice0 * 0x70);
                    if ((struct Moby *)voice->voice.owner ==
                            electric_water_controller_vars->owner &&
                        voice->voice.state != 0) {
                        release_voice_slot(electric_water_controller_vars->voice0);
                    }
                }
                electric_water_controller_vars->voice0 = -1;
                if (electric_water_controller_vars->voice1 != -1) {
                    VoicePoolWindow *voice =
                        (VoicePoolWindow *)((unsigned char *)&voice_pool +
                                            electric_water_controller_vars->voice1 * 0x70);
                    if ((struct Moby *)voice->voice.owner ==
                            electric_water_controller_vars->owner &&
                        voice->voice.state != 0) {
                        release_voice_slot(electric_water_controller_vars->voice1);
                    }
                }
                electric_water_controller_vars->voice1 = -1;
            }
            electric_water_controller_vars->owner = hero.moby;
            electric_water_controller_vars->voice0 = trooper_play_sound(0, 4, hero.moby, 0x28F);
            electric_water_controller_vars->voice1 = trooper_play_sound(1, 4, hero.moby, 0x28F);
            if (electric_water_controller_vars->shock_count == 0) {
                electric_water_controller_vars->saved_fog[0] = D_L15_0015F448;
                electric_water_controller_vars->saved_fog[1] = D_L15_0015F44C;
                electric_water_controller_vars->saved_fog[2] = D_L15_0015F450;
                electric_water_controller_vars->saved_fog[3] = D_L15_0015F454;
                electric_water_controller_vars->saved_fog_color[0] = l15_fog_color[0];
                electric_water_controller_vars->saved_fog_color[1] = l15_fog_color[1];
                electric_water_controller_vars->saved_fog_color[2] = l15_fog_color[2];
            }
            electric_water_controller_vars->shock_count =
                electric_water_controller_vars->shock_count + 1;
            l15_fog_color[0] = 0x7F;
            l15_fog_color[1] = 0x7F;
            l15_fog_color[2] = 0x7F;
            D_L15_0015F44C = 131072.0f;
            D_L15_0015F448 = 0.0f;
            D_L15_0015F450 = random_float_between(128.0f, 255.0f);
            D_L15_0015F454 = 0.0f;
            radius = random_float_between(5.0f, 8.0f);
            theta = random_angle_radians();
            phi = random_angle_radians();
            build_spherical_offset(&electric_water_controller_vars->bolt_aim, radius, theta, phi);
            add_vectors(unused_vector_argument,
                        &electric_water_controller_vars->bolt_aim,
                        &hero.motion.body_center);
            electric_water_controller_vars->bolt_end = electric_water_controller_vars->bolt_aim;
            electric_water_controller_vars->bolt_start = hero.motion.body_center.q;
            initialize_electric_water_bolt_effect((char *)moby);
            update_electric_water_bolt_effect(moby);
            add_draw_callback(draw_electric_water_bolt_effect, moby);
            hit.attacker = moby;
            hit.flags = 0x10001;
            hit.damage = 1.0f;
            hit.v[0] = fast_cos(moby->rot.z) * 0.2f;
            hit.v[1] = fast_sin(moby->rot.z) * 0.2f;
            hit.v[2] = 0.0f;
            set_vector_length_xy(hit.v, hit.v, 1.0f);
            hit.unk19 = 1;
            hit.attacker_class = moby->oclass;
            hit.v[2] = 1.0f;
            hit.v[3] = 5627.9248f;
            hit.unk18 = 0;
            record_moby_hit(hero.moby, &hit);
            if (moby->state == 2) {
                D_L15_0015F448 = electric_water_controller_vars->saved_fog[0];
                D_L15_0015F44C = electric_water_controller_vars->saved_fog[1];
                D_L15_0015F450 = electric_water_controller_vars->saved_fog[2];
                D_L15_0015F454 = electric_water_controller_vars->saved_fog[3];
                l15_fog_color[0] = electric_water_controller_vars->saved_fog_color[0];
                l15_fog_color[1] = electric_water_controller_vars->saved_fog_color[1];
                l15_fog_color[2] = electric_water_controller_vars->saved_fog_color[2];
            }
        }
        /* fall through: the hum and the switches, as in state 2 */
    case ELECTRIC_WATER_OFF:
        nearest = 0;
        for (near_lamp = 0; near_lamp < 3; near_lamp++) {
            if (electric_water_controller_vars->lamps[near_lamp] == -1) {
                continue;
            }
            if (nearest != 0) {
                distance = vector_distance_xy(&nearest->pos, &D_L15_001673C0);
                if (!(vector_distance_xy(
                          &l15_moby_array[electric_water_controller_vars->lamps[near_lamp]].pos,
                          &D_L15_001673C0) < distance)) {
                    continue;
                }
            }
            nearest = &l15_moby_array[electric_water_controller_vars->lamps[near_lamp]];
        }
        if (electric_water_controller_vars->owner == 0 ||
            (distance =
                 vector_distance_xy(&electric_water_controller_vars->owner->pos, &D_L15_001673C0),
             vector_distance_xy(&nearest->pos, &D_L15_001673C0) < distance)) {
            if (electric_water_controller_vars->owner != 0) {
                if (electric_water_controller_vars->voice0 != -1) {
                    VoicePoolWindow *voice =
                        (VoicePoolWindow *)((unsigned char *)&voice_pool +
                                            electric_water_controller_vars->voice0 * 0x70);
                    if ((struct Moby *)voice->voice.owner ==
                            electric_water_controller_vars->owner &&
                        voice->voice.state != 0) {
                        release_voice_slot(electric_water_controller_vars->voice0);
                    }
                }
                electric_water_controller_vars->voice0 = -1;
                if (electric_water_controller_vars->voice1 != -1) {
                    VoicePoolWindow *voice =
                        (VoicePoolWindow *)((unsigned char *)&voice_pool +
                                            electric_water_controller_vars->voice1 * 0x70);
                    if ((struct Moby *)voice->voice.owner ==
                            electric_water_controller_vars->owner &&
                        voice->voice.state != 0) {
                        release_voice_slot(electric_water_controller_vars->voice1);
                    }
                }
                electric_water_controller_vars->voice1 = -1;
            }
            electric_water_controller_vars->owner = nearest;
        }
        if (voice_is_playing(electric_water_controller_vars->owner,
                             electric_water_controller_vars->voice0) == 0) {
            electric_water_controller_vars->voice0 =
                trooper_play_sound(0, 5, electric_water_controller_vars->owner, 0x28F);
        }
        if (moby->state == 1 && voice_is_playing(electric_water_controller_vars->owner,
                                                 electric_water_controller_vars->voice1) == 0) {
            electric_water_controller_vars->voice1 =
                trooper_play_sound(1, 5, electric_water_controller_vars->owner, 0x28F);
        }
        if (fast_dec_timer_s16(&electric_water_controller_vars->off_timer) != 0 &&
            moby->state == 2) {
            electric_water_controller_vars->on_timer =
                truncate_float_to_s32(scale_time(electric_water_on_duration_seconds_gp * 60.0f));
            electric_water_controller_vars->off_timer = scale_game_frames(200);
            electric_water_controller_vars->shock_count = 0;
            moby->state = 1;
        }
        for (sw = 0; sw < 4; sw++) {
            /* Fakematch: the press action is written in both arms so CSE keeps the second sw load. */
            if (electric_water_controller_vars->switches[sw] != -1 &&
                l15_moby_array[electric_water_controller_vars->switches[sw]].oclass == 0x33E &&
                l15_moby_array[electric_water_controller_vars->switches[sw]].state == 2) {
                electric_water_controller_vars->off_timer = truncate_float_to_s32(
                    scale_time(electric_water_controller_vars->off_seconds * 60.0f));
                moby->state = 3;
                trooper_play_sound(
                    2, 0, &l15_moby_array[electric_water_controller_vars->switches[sw]], 0x28F);
                electric_water_controller_vars->armed = 0;
            } else if (electric_water_controller_vars->switches[sw] != -1 &&
                       test_linked_moby_turn_trigger(
                           &l15_moby_array[electric_water_controller_vars->switches[sw]]) != 0) {
                electric_water_controller_vars->off_timer = truncate_float_to_s32(
                    scale_time(electric_water_controller_vars->off_seconds * 60.0f));
                moby->state = 3;
                trooper_play_sound(
                    2, 0, &l15_moby_array[electric_water_controller_vars->switches[sw]], 0x28F);
                electric_water_controller_vars->armed = 0;
            }
        }
        break;

    case ELECTRIC_WATER_SWITCH_DISABLED:
        for (i = 0; i < 3; i++) {
            if (electric_water_controller_vars->lamps[i] != -1) {
                set_moby_color(
                    &l15_moby_array[electric_water_controller_vars->lamps[i]], 0x40, 0x40, 0x40);
            }
        }
        if (electric_water_controller_vars->light != -1) {
            release_point_light_slot(electric_water_controller_vars->light);
            electric_water_controller_vars->light = -1;
        }
        if (electric_water_controller_vars->off_timer != 0) {
            add_draw_callback(draw_electric_water_countdown, moby);
        }
        if (electric_water_controller_vars->owner != 0) {
            if (electric_water_controller_vars->voice0 != -1) {
                VoicePoolWindow *voice =
                    (VoicePoolWindow *)((unsigned char *)&voice_pool +
                                        electric_water_controller_vars->voice0 * 0x70);
                if ((struct Moby *)voice->voice.owner == electric_water_controller_vars->owner &&
                    voice->voice.state != 0) {
                    release_voice_slot(electric_water_controller_vars->voice0);
                }
            }
            electric_water_controller_vars->voice0 = -1;
            if (electric_water_controller_vars->voice1 != -1) {
                VoicePoolWindow *voice =
                    (VoicePoolWindow *)((unsigned char *)&voice_pool +
                                        electric_water_controller_vars->voice1 * 0x70);
                if ((struct Moby *)voice->voice.owner == electric_water_controller_vars->owner &&
                    voice->voice.state != 0) {
                    release_voice_slot(electric_water_controller_vars->voice1);
                }
            }
            electric_water_controller_vars->voice1 = -1;
            electric_water_controller_vars->owner = 0;
        }
        for (i = 0; i < 4; i++) {
            if (electric_water_controller_vars->switches[i] == -1) {
                continue;
            }
            if (hero.ground_moby != &l15_moby_array[electric_water_controller_vars->switches[i]] ||
                hero.ground_moby->oclass != 0x33E || hero.air_frames.s != 0) {
                continue;
            }
            if (electric_water_controller_vars->armed != 0) {
                remaining = electric_water_controller_vars->off_timer;
                if (remaining < truncate_float_to_s32(scale_time(
                                    electric_water_controller_vars->off_seconds * 60.0f)) -
                                    scale_game_frames(0x3C)) {
                    electric_water_controller_vars->off_timer = truncate_float_to_s32(
                        scale_time(electric_water_controller_vars->off_seconds * 60.0f));
                    trooper_play_sound(
                        2, 0, &l15_moby_array[electric_water_controller_vars->switches[i]], 0x28F);
                }
            }
            armed = 0;
        }
        electric_water_controller_vars->armed = armed;
        if (fast_dec_timer_s16(&electric_water_controller_vars->off_timer) == 0) {
            if (is_point_inside_clip_volume(D_0013F3D0_lui,
                                            electric_water_controller_vars->reset_area_a) == 0 ||
                *(short *)(D_0013F3D0_lui + 0x28E) != 0) {
                if (is_point_inside_clip_volume(
                        D_0013F3D0_lui, electric_water_controller_vars->reset_area_b) == 0) {
                    break;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            if (electric_water_controller_vars->switches[i] == -1) {
                continue;
            }
            if (l15_moby_array[electric_water_controller_vars->switches[i]].oclass == 0x33E) {
                l15_moby_array[electric_water_controller_vars->switches[i]].state = 1;
            } else {
                reset_electric_water_switch(
                    (char *)&l15_moby_array[electric_water_controller_vars->switches[i]]);
            }
        }
        electric_water_controller_vars->on_timer =
            truncate_float_to_s32(scale_time(electric_water_on_duration_seconds_gp * 60.0f));
        electric_water_controller_vars->off_timer = scale_game_frames(200);
        electric_water_controller_vars->shock_count = 0;
        moby->state = 1;
        break;
    }

    {
        char        *hero_pos = D_0013F3D0;
        struct Hero *player = (struct Hero *)(hero_pos - 0x80);
        for (level_area = 0; level_area < 4; level_area++) {
            if (is_point_inside_clip_volume(
                    hero_pos, electric_water_controller_vars->water_level_areas[level_area]) != 0 &&
                hero_is_in_special_state() != 0) {
                player->water_surface_z = *(
                    float *)((D_L15_001600EC +
                              electric_water_controller_vars->water_level_areas[level_area] * 128) +
                             0x38);
            }
        }
    }
}

extern int   D_L15_0015F5CC;
extern short electric_water_countdown_text_x __asm__("D_L15_00161BD4") __attribute__((sda));
extern short electric_water_countdown_text_y __asm__("D_L15_00161BD8") __attribute__((sda));
extern short electric_water_countdown_text_color_start __asm__("D_L15_00161BDC")
    __attribute__((sda));
extern short electric_water_countdown_text_color_end __asm__("D_L15_00161BE0") __attribute__((sda));
extern short electric_water_countdown_frame_x_offset __asm__("D_L15_00161BE4") __attribute__((sda));
extern short electric_water_countdown_frame_y_offset __asm__("D_L15_00161BE8") __attribute__((sda));
extern short electric_water_countdown_frame_left_x_offset __asm__("D_L15_00161BEC")
    __attribute__((sda));
extern short electric_water_countdown_frame_right_x_offset __asm__("D_L15_00161BF0")
    __attribute__((sda));
extern short electric_water_countdown_frame_color __asm__("D_L15_00161BF4") __attribute__((sda));
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void  font_print_center_large(int a, int b, int c, int d, int e) __asm__("FUN_001f6af0");
extern void
font_print_center_large_001f6c20(int a, int b, int c, int d, int e) __asm__("FUN_001f6c20");

#define W(x) (*(int *)&(x))

void draw_electric_water_countdown(char *moby) {
    char  countdown_text[8];
    char *electric_water_vars = ((char *)MOBY(moby)->pvars);
    float color_blend_factor;
    int   col;

    draw_ui_frame(
        W(electric_water_countdown_frame_x_offset) + W(electric_water_countdown_text_y),
        W(electric_water_countdown_frame_y_offset) + W(electric_water_countdown_text_y),
        W(electric_water_countdown_frame_left_x_offset) + W(electric_water_countdown_text_x),
        W(electric_water_countdown_frame_right_x_offset) + W(electric_water_countdown_text_x),
        W(electric_water_countdown_frame_color));
    color_blend_factor =
        fast_sin(convert_integer_to_float(D_L15_0015F5CC % 60) / 60.0f * 6.28318f) * 3.0f + 0.5f;
    if (color_blend_factor > 1.0f) {
        color_blend_factor = 1.0f;
    } else if (color_blend_factor < 0.0f) {
        color_blend_factor = 0.0f;
    }
    col = fast_tween_color(W(electric_water_countdown_text_color_start),
                           W(electric_water_countdown_text_color_end),
                           color_blend_factor);
    countdown_text[0] = 0x30;
    countdown_text[1] = *(short *)(electric_water_vars + 0x12) / scale_game_frames(0xE10) + 0x30;
    countdown_text[2] = 0x3A;
    countdown_text[3] = *(short *)(electric_water_vars + 0x12) % scale_game_frames(0xE10) /
                            scale_game_frames(0x258) +
                        0x30;
    countdown_text[4] = *(short *)(electric_water_vars + 0x12) % scale_game_frames(0x258) /
                            scale_game_frames(0x3C) +
                        0x30;
    countdown_text[5] = 0x3A;
    countdown_text[6] =
        *(short *)(electric_water_vars + 0x12) % scale_game_frames(0x3C) / scale_game_frames(6) +
        0x30;
    countdown_text[7] = truncate_float_to_s32((float)random_integer_below(10)) + 0x30;
    font_print_center_large_001f6c20(W(electric_water_countdown_text_x),
                                     W(electric_water_countdown_text_y),
                                     col,
                                     (int)countdown_text,
                                     8);
}

/* Starts the moby's trail when requested: resets the trail points and the counters. */
extern int   D_L15_00161C14_gp __asm__("D_L15_00161C14") __attribute__((sda));
extern int   D_L15_00161C1C_gp __asm__("D_L15_00161C1C") __attribute__((sda));
extern int   D_L15_00161C30_gp __asm__("D_L15_00161C30") __attribute__((sda));
extern int   D_L15_00161C60_lui __asm__("D_L15_00161C60");
extern short D_L15_00161CD0_r __asm__("D_L15_00161CD0");
extern short D_L15_00161CD8_r __asm__("D_L15_00161CD8");
extern short D_L15_00161CC0_a[4] __asm__("D_L15_00161CC0");
extern float D_L15_001D3740_r[][4] __asm__("D_L15_001D3740");
extern float D_L15_001D3750_r[][4] __asm__("D_L15_001D3750");
extern int   D_L15_001D39D0_r[] __asm__("D_L15_001D39D0");
extern float D_L15_001D3A20_r[][4] __asm__("D_L15_001D3A20");
extern int   D_L15_001D3B60_r[] __asm__("D_L15_001D3B60");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");

extern int   electric_water_bolt_init_requested_gp __asm__("D_L15_00161BF8") __attribute__((sda));
extern float electric_water_bolt_initial_length_scale_gp __asm__("D_L15_00161BFC")
    __attribute__((sda));
extern int electric_water_bolt_initial_frame_count_gp __asm__("D_L15_00161C8C")
    __attribute__((sda));
extern short second_side_arc_reset_frame_count __asm__("D_L15_00161CD2");
extern short third_side_arc_reset_frame_count __asm__("D_L15_00161CD4");
extern short fourth_side_arc_reset_frame_count __asm__("D_L15_00161CD6");
extern short second_side_arc_fade_peak_timer_value __asm__("D_L15_00161CDA");
extern short third_side_arc_fade_peak_timer_value __asm__("D_L15_00161CDC");
extern short fourth_side_arc_fade_peak_timer_value __asm__("D_L15_00161CDE");
extern int   D_L15_00161CE4;

void initialize_electric_water_bolt_effect(char *moby) {
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    float bolt_displacement[4];
    float step[4];
    int   i;
    int   j;
    if (electric_water_bolt_init_requested_gp != 0) {
        electric_water_bolt_init_requested_gp = 0;
        D_L15_00161CE4 = 0;
        electric_water_bolt_initial_scaled_frame_count =
            scale_game_frames(electric_water_bolt_initial_frame_count_gp);
        D_L15_00161C60_lui = 0x7F2020;
        fast_vec_sub(bolt_displacement, moby_vars + 0x100, moby_vars + 0xF0);
        fast_vec_normalize(
            step, bolt_displacement, electric_water_bolt_initial_length_scale_gp / 20.0f);
        qcopy(D_L15_001D3750_r, moby_vars + 0xF0);
        for (i = 1; i < 20; i++) {
            fast_vec_add(D_L15_001D3750_r[i], D_L15_001D3740_r[i], step);
            D_L15_001D39D0_r[i] = 0;
            clear_u64_value(D_L15_001D3A20_r[i]);
            D_L15_001D3B60_r[i] = 0;
        }
        for (j = 0; j < 4; j++)
            D_L15_00161CC0_a[j] = -1;
        D_L15_00161CD0_r = 8;
        D_L15_00161CD8_r = 4;
        second_side_arc_reset_frame_count = 4;
        second_side_arc_fade_peak_timer_value = 2;
        third_side_arc_fade_peak_timer_value = 4;
        third_side_arc_reset_frame_count = 8;
        fourth_side_arc_reset_frame_count = 4;
        fourth_side_arc_fade_peak_timer_value = 2;
        D_L15_00161C14_gp = 0;
        D_L15_00161C1C_gp = 0;
        D_L15_00161C30_gp = 0;
    }
}

/* Quartu arc emitter: rebuilds the 20-point bolt and its mirror between the data block's end points, regrows or jitters four side arcs, then spawns three particles. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D7C00.c: func_L15_002D8FA0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2d8fa0;

typedef struct {
    V_2d8fa0 p[5];
} P_2d8fa0;

typedef struct {
    V_2d8fa0 hdr;
    V_2d8fa0 p[4];
} H_2d8fa0;

typedef struct {
    char     pad[0xF0];
    V_2d8fa0 a;
    V_2d8fa0 b;
    V_2d8fa0 c;
} Data;

extern H_2d8fa0 l15_electric_water_side_arc_predecessor_points[] __asm__("D_L15_001D3BA0");
extern V_2d8fa0 D_L15_001D3740[];
extern V_2d8fa0 D_L15_001D3750[2][20];
extern V_2d8fa0 D_L15_001D3A20[];
extern char     D_0013E533[];
extern char     D_L15_001D3BC0[];
extern f32      fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float    D_0015ED60;
extern float    D_L15_0015F580[] MACRO_ADDR;
extern float    D_L15_001D3B60[];
extern float    D_L15_001D39D0[];
extern float    FUN_L00_00257c48(float lo, float hi);
extern float    normalize_angle(float) __asm__("FUN_L00_0025e310");
extern short    electric_water_bolt_height_wave_point_phase_step_gp __asm__("D_L15_00161C08")
    __attribute__((sda));
extern short electric_water_bolt_height_wave_start_amplitude_gp __asm__("D_L15_00161C0C")
    __attribute__((sda));
extern short electric_water_bolt_height_wave_end_amplitude_gp __asm__("D_L15_00161C10")
    __attribute__((sda));
extern s32   D_L15_00161C14 __attribute__((sda));
extern short electric_water_bolt_height_wave_frame_phase_step_gp __asm__("D_L15_00161C18")
    __attribute__((sda));
extern s32   D_L15_00161C1C __attribute__((sda));
extern short electric_water_bolt_paired_path_frame_phase_decrement_gp __asm__("D_L15_00161C20")
    __attribute__((sda));
extern short electric_water_bolt_paired_path_point_phase_step_gp __asm__("D_L15_00161C24")
    __attribute__((sda));
extern short electric_water_bolt_paired_path_height_amplitude_gp __asm__("D_L15_00161C28")
    __attribute__((sda));
extern short electric_water_bolt_paired_path_lateral_amplitude_gp __asm__("D_L15_00161C2C")
    __attribute__((sda));
extern s32   D_L15_00161C30 __attribute__((sda));
extern short electric_water_bolt_paired_height_frame_phase_step_gp __asm__("D_L15_00161C34")
    __attribute__((sda));
extern short electric_water_bolt_paired_height_point_phase_step_gp __asm__("D_L15_00161C38")
    __attribute__((sda));
extern short electric_water_bolt_height_jitter_lower_bound_gp __asm__("D_L15_00161C4C")
    __attribute__((sda));
extern short electric_water_bolt_height_jitter_upper_bound_gp __asm__("D_L15_00161C50")
    __attribute__((sda));
extern short electric_water_bolt_particle_motion_length_scale_gp __asm__("D_L15_00161C90")
    __attribute__((sda));
extern short electric_water_bolt_random_step_blend_factor_gp __asm__("D_L15_00161C98")
    __attribute__((sda));
extern short D_L15_00161CC0[] MACRO_ADDR;
extern short D_L15_00161CC8[] MACRO_ADDR;
extern short D_L15_00161CD0[] MACRO_ADDR;
extern short D_L15_00161CD8[] MACRO_ADDR;
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
unsigned char *
FUN_L00_00272060(void *pos, int s, int color, int mode, int b, float *v, float x, float y, float z);
extern void
spawn_2d8fa0(void *, int, float, float, int, int, int, void *, float) __asm__("FUN_L00_00272060");
extern char     D_0013F6E0_2d8fa0[] __asm__("D_0013F5E0");
extern s32      D_L15_00161C60_gp __asm__("D_L15_00161C60") __attribute__((sda));
extern P_2d8fa0 D_L15_001D3E80_2d8fa0[] __asm__("D_L15_001D3BB0");
extern float    D_L15_00167440_2d8fa0[] __asm__("D_L15_001673C0");

void update_electric_water_bolt_effect(Moby *moby) {
    V_2d8fa0      vectors[10];
    V_2d8fa0      jitter[5];
    Data         *bolt_endpoints = (Data *)moby->pvars;
    unsigned char flag = *(unsigned char *)D_0013E533;
    float         scale;
    int           i;
    int           k;
    float         angle;
    int           spin;

    *(float *)&D_L15_00161C14 = fast_add_rotations(
        *(float *)&D_L15_00161C14, *(float *)&electric_water_bolt_height_wave_frame_phase_step_gp);
    *(float *)&D_L15_00161C1C = fast_subtract_rotations(
        *(float *)&D_L15_00161C1C,
        *(float *)&electric_water_bolt_paired_path_frame_phase_decrement_gp);
    *(float *)&D_L15_00161C30 =
        fast_add_rotations(*(float *)&D_L15_00161C30,
                           *(float *)&electric_water_bolt_paired_height_frame_phase_step_gp);
    scale = fast_vec_dist(&bolt_endpoints->a, &bolt_endpoints->c) * 0.05f;
    if (flag) {
        *(int *)&D_L15_00161C60_gp = 0x7F2040;
    } else {
        *(int *)&D_L15_00161C60_gp = 0x7F2020;
    }
    fast_vec_sub(&vectors[0], &bolt_endpoints->b, &bolt_endpoints->a);
    fast_vec_normalize(&vectors[0], &vectors[0], 1.0f);
    fast_vec_cross(&vectors[1], &vectors[0], D_0013F6E0_2d8fa0);
    build_look_at_matrix(&vectors[0], &vectors[0], &vectors[1], 0.17453292f);
    scale_vector_xyz(&vectors[3], &vectors[0], scale);
    qcopy(D_L15_001D3750[0], &bolt_endpoints->a);
    qcopy(D_L15_001D3750[1], &bolt_endpoints->a);
    for (i = 1; i < 20; i++) {
        float wave;
        float remaining;
        float step_length;
        float yaw;
        float pitch;
        wave = *(float *)&electric_water_bolt_height_wave_start_amplitude_gp +
               (*(float *)&electric_water_bolt_height_wave_end_amplitude_gp -
                *(float *)&electric_water_bolt_height_wave_start_amplitude_gp) *
                   (convert_integer_to_float(i) * 0.05f);
        wave =
            wave * fast_sin(normalize_angle(
                       *(float *)&D_L15_00161C14 +
                       (float)i * *(float *)&electric_water_bolt_height_wave_point_phase_step_gp));
        wave = wave + FUN_L00_00257c48(*(float *)&electric_water_bolt_height_jitter_lower_bound_gp,
                                       *(float *)&electric_water_bolt_height_jitter_upper_bound_gp);
        D_L15_001D3750[0][i].f[2] = D_L15_001D3750[0][i].f[2] - D_L15_001D39D0[i];
        D_L15_001D39D0[i] = wave;
        yaw = random_angle_radians();
        pitch = random_angle_radians();
        build_spherical_offset(&vectors[1], scale, yaw, pitch);
        vectors[1].f[2] = vectors[1].f[2] * 0.5f;
        lerp_vector(&vectors[2],
                    &vectors[3],
                    &vectors[1],
                    *(float *)&electric_water_bolt_random_step_blend_factor_gp);
        step_length = fast_vec_length(&vectors[2]);
        if (step_length == 0.0f) {
            qcopy(&D_L15_001D3750[0][i], &D_L15_001D3750[0][i - 1]);
        } else {
            scale_vector_xyz(&vectors[2], &vectors[2], scale / step_length);
            fast_vec_add(&D_L15_001D3750[0][i], &D_L15_001D3740[i], &vectors[2]);
            qcopy(&vectors[3], &vectors[2]);
        }
        D_L15_001D3750[0][i].f[2] = D_L15_001D3750[0][i].f[2] + wave;
        fast_vec_sub(&vectors[1], &D_L15_001D3750[0][i], &D_L15_001D3740[i]);
        fast_vec_sub(&vectors[5], &bolt_endpoints->c, &D_L15_001D3750[0][i]);
        remaining = fast_vec_length(&vectors[5]);
        if (remaining != 0.0f) {
            float blend;
            if (i == 19) {
                blend = 0.5f;
                scale = remaining;
            } else if (i >= 16) {
                blend = 0.5f;
                scale = remaining / convert_integer_to_float(19 - i);
            } else {
                blend = 0.1f;
            }
            scale_vector_xyz(&vectors[5], &vectors[5], scale / remaining);
            lerp_vector(&vectors[3], &vectors[3], &vectors[5], blend);
        }
        if (i < 10) {
            float phase;
            float height;
            float side_angle;
            float swing;
            phase = normalize_angle(
                *(float *)&D_L15_00161C1C +
                (float)i * *(float *)&electric_water_bolt_paired_path_point_phase_step_gp);
            height =
                *(float *)&electric_water_bolt_paired_path_height_amplitude_gp *
                fast_sin(normalize_angle(
                    *(float *)&D_L15_00161C30 +
                    (float)i * *(float *)&electric_water_bolt_paired_height_point_phase_step_gp));
            qcopy(&D_L15_001D3750[1][i], &D_L15_001D3750[0][i]);
            height = height * fast_sin(phase) + FUN_L00_00257c48(0.0f, 0.1f);
            D_L15_001D3750[1][i].f[2] = D_L15_001D3750[1][i].f[2] + height;
            D_L15_001D3B60[i] = height;
            side_angle =
                fast_add_rotations(fast_arc_tan(vectors[1].f[0], vectors[1].f[1]), 1.5707964f);
            swing =
                *(float *)&electric_water_bolt_paired_path_lateral_amplitude_gp * fast_cos(phase);
            vectors[4].f[0] = fast_cos(side_angle) * swing;
            vectors[4].f[1] = fast_sin(side_angle) * swing;
            vectors[4].f[2] = 0.0f;
            qcopy(&D_L15_001D3A20[i], &vectors[4]);
            fast_vec_add(&D_L15_001D3750[1][i], &D_L15_001D3750[1][i], &vectors[4]);
        } else {
            int mirror = 21 - i;
            qcopy(&D_L15_001D3750[1][i], &D_L15_001D3750[0][i]);
            D_L15_001D3750[1][i].f[2] = D_L15_001D3750[1][i].f[2] + D_L15_001D3B60[mirror];
            fast_vec_add(&D_L15_001D3750[1][i], &D_L15_001D3750[1][i], &D_L15_001D3A20[mirror]);
            qcopy(&vectors[4], &D_L15_001D3A20[i]);
        }
    }
    fast_vec_cross(&vectors[6], &vectors[0], D_0013F6E0_2d8fa0);
    fast_vec_normalize(&vectors[6], &vectors[6], 1.0f);
    fast_vec_normalize(&vectors[7], D_0013F6E0_2d8fa0, 1.0f);
    for (k = 0; k < 4; k++) {
        float bend;
        if (k < 2) {
            qcopy(&vectors[8], &bolt_endpoints->a);
            bend = 0.4f;
        } else {
            qcopy(&vectors[8], &bolt_endpoints->b);
            bend = 0.5f;
        }
        if (fast_dec_timer_s16(D_L15_00161CC0 + k)) {
            int j;
            D_L15_00161CC0[k] = scale_game_frames(D_L15_00161CD0[k]);
            D_L15_00161CC8[k] = 0;
            scale = FUN_L00_00257c48(0.52359879f, 1.04719758f);
            angle = random_float_between(-1.04719758f, 0.17453292f);
            scale_vector_xyz(&vectors[5], &vectors[0], bend);
            build_look_at_matrix(&vectors[5], &vectors[5], &vectors[6], angle);
            build_look_at_matrix(&vectors[5], &vectors[5], &vectors[7], scale);
            qcopy(&D_L15_001D3E80_2d8fa0[k].p[0], &vectors[8]);
            fast_vec_add(&D_L15_001D3E80_2d8fa0[k].p[1], &vectors[8], &vectors[5]);
            bend = 1.0f;
            for (j = 2; j < 5; j++) {
                fast_vec_sub(&vectors[9],
                             &l15_electric_water_side_arc_predecessor_points[k].p[j - 1],
                             D_L15_00167440_2d8fa0);
                angle = random_float_between(0.17453292f, 0.959931076f) * bend;
                bend = -bend;
                build_look_at_matrix(&vectors[5], &vectors[5], &vectors[9], angle);
                fast_vec_add((D_L15_001D3E80_2d8fa0[k].p + j),
                             &l15_electric_water_side_arc_predecessor_points[k].p[j - 1],
                             &vectors[5]);
            }
        } else {
            int j;
            for (j = 1; j < 5; j++) {
                fast_vec_sub(&jitter[j],
                             (D_L15_001D3E80_2d8fa0[k].p + j),
                             &l15_electric_water_side_arc_predecessor_points[k].p[j - 1]);
                jitter[j].f[0] += FUN_L00_00257c48(0.0f, 0.1f);
                jitter[j].f[1] += FUN_L00_00257c48(0.0f, 0.1f);
                jitter[j].f[2] += FUN_L00_00257c48(0.0f, 0.1f);
            }
            qcopy(&D_L15_001D3E80_2d8fa0[k].p[0], &vectors[8]);
            for (j = 1; j < 5; j++) {
                fast_vec_add((D_L15_001D3E80_2d8fa0[k].p + j),
                             &l15_electric_water_side_arc_predecessor_points[k].p[j - 1],
                             &jitter[j]);
            }
            {
                int   elapsed = D_L15_00161CD8[k];
                int   timer = D_L15_00161CC0[k];
                float fade;
                if (elapsed < timer) {
                    elapsed = timer - elapsed;
                } else {
                    elapsed = elapsed - timer;
                }
                fade = convert_integer_to_float(elapsed);
                fade = fade / convert_integer_to_float(scale_game_frames(15));
                D_L15_00161CC8[k] = truncate_float_to_s32((1.0f - fade) * 32.0f);
            }
        }
    }
    scale = FUN_L00_00257c48(0.52359879f, 1.04719758f);
    angle = random_float_between(-0.52359879f, 1.04719758f);
    scale_vector_xyz(&vectors[5],
                     &vectors[0],
                     *(float *)&electric_water_bolt_particle_motion_length_scale_gp * D_0015ED60);
    build_look_at_matrix(&vectors[5], &vectors[5], &vectors[6], angle);
    build_look_at_matrix(&vectors[5], &vectors[5], &vectors[7], scale);
    spin = random_int_between(1, 8);
    if (random_integer_below(2))
        spin = -spin;
    angle = random_float_between(0.2f, 0.6f);
    spawn_2d8fa0(&bolt_endpoints->a,
                 scale_game_frames(15),
                 angle * 0.3f,
                 angle,
                 *(int *)&D_L15_00161C60_gp | 0x7F000000,
                 0,
                 spin,
                 &vectors[5],
                 0.0f);
    spin = -spin;
    spawn_2d8fa0(&bolt_endpoints->a,
                 scale_game_frames(15),
                 angle * 0.15f,
                 angle * 0.5f,
                 0x307F7F7F,
                 0,
                 spin,
                 &vectors[5],
                 0.0f);
    spin = random_int_between(1, 8);
    if (random_integer_below(2))
        spin = -spin;
    angle = random_float_between(0.15f, 3.0f);
    spawn_2d8fa0(&bolt_endpoints->c,
                 scale_game_frames(12),
                 angle * 0.1f,
                 angle,
                 *(int *)&D_L15_00161C60_gp | 0x7F000000,
                 0,
                 spin,
                 D_L15_0015F580,
                 0.0f);
}

extern float frame_scale __asm__("D_0015ED60");
extern char  D_L15_001D3750_8710[] __asm__("D_L15_001D3750");
extern char  D_L15_001D3BB0_8710[] __asm__("D_L15_001D3BB0");
extern int   D_L15_00161CC8_8710 __asm__("D_L15_00161CC8");
extern short D_L15_00161C58_gp __asm__("D_L15_00161C58") __attribute__((sda));
extern short D_L15_00161C54_gp __asm__("D_L15_00161C54") __attribute__((sda));
extern short D_L15_00161C3C_gp __asm__("D_L15_00161C3C") __attribute__((sda));
extern short D_L15_00161C44_gp __asm__("D_L15_00161C44") __attribute__((sda));
extern short D_L15_00161C64_gp __asm__("D_L15_00161C64") __attribute__((sda));
extern short D_L15_00161C68_gp __asm__("D_L15_00161C68") __attribute__((sda));
extern short D_L15_00161C9C_gp __asm__("D_L15_00161C9C") __attribute__((sda));
extern short D_L15_00161C6C_gp __asm__("D_L15_00161C6C") __attribute__((sda));
extern short D_L15_00161C70_gp __asm__("D_L15_00161C70") __attribute__((sda));
extern short D_L15_00161C74_gp __asm__("D_L15_00161C74") __attribute__((sda));
extern short D_L15_00161C78_gp __asm__("D_L15_00161C78") __attribute__((sda));
extern short D_L15_00161CA0_gp __asm__("D_L15_00161CA0") __attribute__((sda));
extern void  draw_beam_and_arc_two_pass_quads(char *, int, int, int, float, float, float) __asm__("FUN_L00_002baae0");
extern void FUN_L15_002d8e48_8710(char *) __asm__("FUN_L15_002d8e48");
#define F(x) (*(float *)&(x))
void draw_electric_water_bolt_effect(char *moby) {
    float decremented_accumulator = F(D_L15_00161C54_gp) - F(D_L15_00161C58_gp) * frame_scale;
    F(D_L15_00161C54_gp) = decremented_accumulator;
    if (decremented_accumulator <= -8.0f) {
        F(D_L15_00161C54_gp) = decremented_accumulator + 8.0f;
    }
    draw_beam_and_arc_two_pass_quads(D_L15_001D3750_8710,
                                     W(D_L15_00161C3C_gp),
                                     W(D_L15_00161C44_gp),
                                     0x14,
                                     F(D_L15_00161C64_gp),
                                     F(D_L15_00161C68_gp),
                                     F(D_L15_00161C9C_gp));
    draw_beam_and_arc_two_pass_quads(D_L15_001D3750_8710 + 0x140,
                                     W(D_L15_00161C6C_gp),
                                     W(D_L15_00161C70_gp),
                                     0x14,
                                     F(D_L15_00161C74_gp),
                                     F(D_L15_00161C78_gp),
                                     F(D_L15_00161CA0_gp));
    draw_beam_and_arc_two_pass_quads(D_L15_001D3BB0_8710,
                                     *(short *)&D_L15_00161CC8_8710,
                                     *(short *)&D_L15_00161CC8_8710,
                                     5,
                                     F(D_L15_00161C74_gp),
                                     F(D_L15_00161C78_gp),
                                     0.2f);
    draw_beam_and_arc_two_pass_quads(D_L15_001D3BB0_8710 + 0x50,
                                     *(short *)&second_side_arc_fade_level,
                                     *(short *)&second_side_arc_fade_level,
                                     5,
                                     F(D_L15_00161C74_gp),
                                     F(D_L15_00161C78_gp),
                                     0.2f);
    draw_beam_and_arc_two_pass_quads(D_L15_001D3BB0_8710 + 0xA0,
                                     *(short *)&third_side_arc_fade_level,
                                     *(short *)&third_side_arc_fade_level,
                                     5,
                                     F(D_L15_00161C74_gp),
                                     F(D_L15_00161C78_gp),
                                     0.2f);
    draw_beam_and_arc_two_pass_quads(D_L15_001D3BB0_8710 + 0xF0,
                                     *(short *)&fourth_side_arc_fade_level,
                                     *(short *)&fourth_side_arc_fade_level,
                                     5,
                                     F(D_L15_00161C74_gp),
                                     F(D_L15_00161C78_gp),
                                     0.2f);
    FUN_L15_002d8e48_8710(moby);
}

typedef struct {
    float u, v;
} UV2d8e48;

typedef struct {
    V2d8e48      corner[4];
    unsigned int color[4];
    UV2d8e48     uv[4];
    long         unk70, tex, unk80, unk88;
} Q2d8e48;

extern unsigned int l15_electric_water_bolt_start_first_quad_color __asm__("D_L15_00161CAC")
    __attribute__((sda));
extern int   D_L15_00161C60 __asm__("D_L15_00161C60") __attribute__((sda));
extern float l15_electric_water_bolt_start_first_quad_base_scale __asm__("D_L15_00161CB4")
    __attribute__((sda));
extern float l15_electric_water_bolt_start_second_quad_base_scale __asm__("D_L15_00161CB8")
    __attribute__((sda));
extern V2d8e48  D_L15_001673C0 __asm__("D_L15_001673C0") NOT_SDA;
extern V2d8e48  D_L15_001D36F0[] __asm__("D_L15_001D36F0") NOT_SDA;
extern UV2d8e48 D_L15_001D3730[] __asm__("D_L15_001D3730") NOT_SDA;
extern void     fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void     fast_vec_scale(float, void *, void *) __asm__("FUN_001f9a68");
extern long     get_effect_texture(int) __asm__("FUN_001f44b8");
extern float    randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern void     draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

void draw_electric_water_bolt_start_quads(void *mv) __asm__("FUN_L15_002d8e48");

void draw_electric_water_bolt_start_quads(void *mv) {
    Q2d8e48      quad;
    V2d8e48      quad_transform[4];
    V2d8e48     *bolt_start_position;
    unsigned int uniform_quad_color;
    float        quad_scale;
    int          i;
    bolt_start_position = *(V2d8e48 **)((char *)mv + 0x78);
    quad.tex = get_effect_texture(0xB);
    bolt_start_position = (V2d8e48 *)((char *)bolt_start_position + 0xF0);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    fast_vec_sub(&quad_transform[3], &D_L15_001673C0, bolt_start_position);
    fast_vec_normalize(&quad_transform[3], &quad_transform[3], 0.3f);
    fast_vec_add(&quad_transform[3], &quad_transform[3], bolt_start_position);
    quad_transform[3].f[3] = 1.0f;
    fast_vec_sub(&quad_transform[0], &D_L15_001673C0, &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    fast_vec_cross(&quad_transform[1], &quad_transform[0], (V2d8e48 *)&hero.gravity_dir);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    fast_vec_cross(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    uniform_quad_color = l15_electric_water_bolt_start_first_quad_color;
    quad.color[3] = uniform_quad_color;
    quad.color[2] = uniform_quad_color;
    quad.color[1] = uniform_quad_color;
    quad.color[0] = uniform_quad_color;
    quad_scale = l15_electric_water_bolt_start_first_quad_base_scale + randf_sym(0.0f, 0.025f);
    for (i = 0; i < 4; i++) {
        fast_vec_scale(quad_scale, &quad.corner[i], &D_L15_001D36F0[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &quad_transform[0]);
        quad.uv[i].u = D_L15_001D3730[i].u;
        quad.uv[i].v = D_L15_001D3730[i].v;
    }
    draw_geometry_quad(&quad, 0, 0);
    uniform_quad_color = D_L15_00161C60;
    quad.color[3] = uniform_quad_color;
    quad.color[2] = uniform_quad_color;
    quad.color[1] = uniform_quad_color;
    quad.color[0] = uniform_quad_color;
    quad_scale = l15_electric_water_bolt_start_second_quad_base_scale + randf_sym(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        fast_vec_scale(quad_scale, &quad.corner[i], &D_L15_001D36F0[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &quad_transform[0]);
        quad.uv[i].u = D_L15_001D3730[i].u;
        quad.uv[i].v = D_L15_001D3730[i].v;
    }
    draw_geometry_quad(&quad, 0, 0);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D7C00.c: func_L15_002DA990), where it is exact; names translated to the US level program. */

extern int is_moby_class_23e_in_state_14(void *) __asm__("FUN_L03_002c6c60");

/* A class 0x23E moby in state 14 goes to state 1 and rises 1.5. */
void reset_electric_water_switch(char *moby) __asm__("FUN_L15_002d95a0");

void reset_electric_water_switch(char *moby) {
    if (is_moby_class_23e_in_state_14(moby)) {
        moby[0x20] = 1;
        MOBY(moby)->pos.z += 1.5f;
    }
}

float                        D_L17_00161A58 = 2.0f;
float                        D_L17_00161A5C = 0;
unsigned char D_L17_001BB324 NOT_SDA = 0;
unsigned char D_L17_001BBF84 NOT_SDA = 0;
float                        D_L17_0015F44C = 0;
float                        D_L17_0015F450 = 0;
float                        D_L17_0015F454 = 0;
short                        D_L17_00161A64 = 0xfa;
short                        D_L17_00161A68 = 0x15e;
short                        D_L17_00161A6C = -16192;
short                        D_L17_00161A70 = 0x40ff;
short                        D_L17_00161A74 = -5;
short                        D_L17_00161A78 = 0x14;
short                        D_L17_00161A7C = -50;
short                        D_L17_00161A80 = 0x34;
short                        D_L17_00161A84 = 0x60;
int                          D_L17_00161AA4 = 0;
int                          D_L17_00161AAC = 0;
int                          D_L17_00161AC0 = 0;
int                          D_L17_00161AF0 = 0x7f2020;
short                        D_L17_00161B60 = 0;
short                        D_L17_00161B68 = 0;
short                        D_L17_00161B50[4] = {0};

int   D_L17_00161A88 = 1;
float D_L17_00161A8C = 15.0f;
int   D_L17_00161B1C = 0x5a;
short D_L17_00161B62 = 0;
short D_L17_00161B64 = 0;
short D_L17_00161B66 = 0;
short D_L17_00161B6A = 0;
short D_L17_00161B6C = 0;
short D_L17_00161B6E = 0;
int   D_L17_00161B74 = 0;

V_2d8fa0 D_L17_001D39C0 = {
    {1.0f, 0.0f, 1.0f, 1.0f}
};

V_2d8fa0 D_L17_001D39D0[2][20] = {0};

V_2d8fa0 D_L17_001D3CA0 = {0};

char D_L17_001D3E40  NOT_SDA = 0;
float D_L17_001D3DE0 NOT_SDA = 0;
float D_L17_001D3C50 NOT_SDA = 0;
short                D_L17_00161A98 = -1483;
short                D_L17_00161A9C = -15729;
short                D_L17_00161AA0 = -15729;
short                D_L17_00161AA8 = -18238;
short                D_L17_00161AB0 = -30804;
short                D_L17_00161AB4 = -21074;
short                D_L17_00161AB8 = -13107;
short                D_L17_00161ABC = 0;
short                D_L17_00161AC4 = 0x7770;
short                D_L17_00161AC8 = 0x7750;
short                D_L17_00161ADC = 0;
short                D_L17_00161AE0 = -13107;
short                D_L17_00161B20 = 0x51ec;
short                D_L17_00161B28 = -26214;
short                D_L17_00161B58 = 0;
short                D_L17_00161AE8 = -26214;
short                D_L17_00161AE4 = 0;
short                D_L17_00161ACC = 0x80;
short                D_L17_00161AD4 = 0x40;
short                D_L17_00161AF4 = -13107;
short                D_L17_00161AF8 = -13107;
short                D_L17_00161B2C = 0x6666;
short                D_L17_00161AFC = 0x10;
short                D_L17_00161B00 = 0x20;
short                D_L17_00161B04 = -13107;
short                D_L17_00161B08 = -26214;
short                D_L17_00161B30 = -13107;
unsigned int         D_L17_00161B3C = 0x307f7f7f;
float                D_L17_00161B44 = 0.1f;
float                D_L17_00161B48 = 0.35f;

V2d8e48 D_L17_001D3970 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV2d8e48 D_L17_001D39B0 NOT_SDA = {0};

short D_L18_0016204C = -26214;
short D_L18_00162048 = 0;
short D_L18_00162030 = 0x80;
short D_L18_00162038 = 0x40;
short D_L18_00162058 = -13107;
short D_L18_0016205C = -13107;
short D_L18_00162090 = 0x6666;
short D_L18_00162060 = 0x10;
short D_L18_00162064 = 0x20;
short D_L18_00162068 = -13107;
short D_L18_0016206C = -26214;
short D_L18_00162094 = -13107;
