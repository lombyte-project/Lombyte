/* Level 16 conveyor_belt: group motion, voice and belt strips. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"

extern float D_0015ED6C;

void update_conveyor_group_motion_transforms(int index, void *conveyor_motion_vector) __asm__("FUN_L16_002c9c38");
extern void  add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float D_0015ED6C, frame_time;
s32          is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");

extern float D_0015ED6C, D_0015EE70;
/* Updates a linked moby's movement and follows it while its state is active. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CAC40), where it is exact; names translated to the US level program. */

extern void FUN_L16_002c9a50_u(unsigned char *) __asm__("FUN_L16_002c9a50");
extern void update_conveyor_group_motion_transforms(int, void *) __asm__("FUN_L16_002c9c38");
void        draw_conveyor_belt_strips(struct Moby *m) __asm__("FUN_L16_002c9cd0");

#include "rnc/overlay/l16/mobys/conveyor_belt_globals.h"

extern float conveyor_belt_base_speed_gp __asm__("D_L16_0016198C") __attribute__((sda));
void         conveyor_belt_update(struct Moby *m) __asm__("FUN_L16_002c9878");

void conveyor_belt_update(struct Moby *moby) {
    enum {
        CONVEYOR_BELT_STATE_INITIALIZE = 0,
        CONVEYOR_BELT_STATE_APPROACH_BASE_SPEED = 1,
        CONVEYOR_BELT_STATE_APPROACH_NEGATED_SPEED = 2
    };
    char *conveyor_data = (char *)moby->pvars;
    float conveyor_motion_vector[4];
    float target;
    int   one = 1;
    switch (moby->state) {
    case CONVEYOR_BELT_STATE_INITIALIZE:
        *(int *)(conveyor_data + 0x5C) |= 4;
        moby->visible = 0;
        moby->flags |= one;
        if (*(int *)(conveyor_data + 0x60) == -1) {
            moby->flags |= 2;
        } else {
            moby->state = 2;
        }
        *(int *)(conveyor_data + 0x74) = -1;
        break;
    case CONVEYOR_BELT_STATE_APPROACH_NEGATED_SPEED:
        target = -(conveyor_belt_base_speed_gp * D_0015ED6C);
        if (*(float *)(conveyor_data + 0x6C) != target) {
            approach_value((float *)(conveyor_data + 0x6C), target, frame_time_sq * 12.0f);
            fast_vec_normalize(
                conveyor_motion_vector, &moby->rot_row0, *(float *)(conveyor_data + 0x6C));
            update_conveyor_group_motion_transforms(moby->group, conveyor_motion_vector);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(conveyor_data + 0x68)))
            moby->state = one;
        add_draw_callback(draw_conveyor_belt_strips, moby);
        break;
    case CONVEYOR_BELT_STATE_APPROACH_BASE_SPEED:
        target = conveyor_belt_base_speed_gp * D_0015ED6C;
        if (*(float *)(conveyor_data + 0x6C) != target) {
            approach_value((float *)(conveyor_data + 0x6C), target, frame_time_sq * 12.0f);
            fast_vec_normalize(
                conveyor_motion_vector, &moby->rot_row0, *(float *)(conveyor_data + 0x6C));
            update_conveyor_group_motion_transforms(moby->group, conveyor_motion_vector);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(conveyor_data + 0x64)))
            moby->state = 2;
        add_draw_callback(draw_conveyor_belt_strips, moby);
        break;
    }
    if (moby->state)
        FUN_L16_002c9a50_u(moby);
}

extern char  D_L16_001671C0[];
extern int  *D_L16_001ABFC0_c9a50[] __asm__("D_L16_001ABCC0");
extern char *D_L16_00160098_c9a50 __asm__("D_L16_0015FFD8");
void         update_conveyor_group_voice(struct Moby *moby) __asm__("FUN_L16_002c9a50");

void update_conveyor_group_voice(struct Moby *moby) {
    short *group_entry = (short *)D_L16_001ABFC0_c9a50[moby->group];
    char  *moby_data = (char *)moby->pvars;
    char  *nearest = 0;
    float  best = 1024.0f;
    float  voice_position_offset[4];
    if (group_entry == 0)
        return;
    do {
        char *other = D_L16_00160098_c9a50 + (((unsigned short)*group_entry & 0x7FFF) << 8);
        if (*(short *)(other + 0xA6) == 0x1D7) {
            float candidate_distance = fast_vec_dist(D_L16_001671C0, other + 0x10);
            if (candidate_distance < best) {
                nearest = other;
                best = candidate_distance;
            }
        }
    } while (*group_entry++ >= 0);
    if (fast_vec_dist(nearest + 0x10, D_L16_001671C0) < 64.0f) {
        int   voice_slot_index = *(int *)(moby_data + 0x74);
        char *slot;
        if (voice_slot_index >= 0) {
            char *target;
            slot = (char *)&voice_pool + voice_slot_index * 0x70;
            target = *(char **)(slot + 0x88);
            if (target && *(short *)(target + 0xA6) == 0x1D7) {
                fast_vec_normalize(voice_position_offset, nearest + 0xE0, 2.5f);
                fast_vec_add((char *)&voice_pool + 0x90 + *(int *)(moby_data + 0x74) * 0x70,
                             voice_position_offset,
                             nearest + 0x10);
                *(char **)((char *)&voice_pool + 0x88 + *(int *)(moby_data + 0x74) * 0x70) =
                    nearest;
                return;
            }
        }
        voice_slot_index = allocate_voice_for_target_entry(0, 13, nearest);
        *(int *)(moby_data + 0x74) = voice_slot_index;
        if (voice_slot_index >= 0) {
            fast_vec_normalize(voice_position_offset, &moby->rot_row2, 2.5f);
            fast_vec_add((char *)&voice_pool + 0x90 + *(int *)(moby_data + 0x74) * 0x70,
                         voice_position_offset,
                         &moby->pos);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CB000), where it is exact; names translated to the US level program. */

extern char  *D_L16_0015FFD8;
extern short *D_L16_001ABFC0_2CB000[] __asm__("D_L16_001ABCC0");

void update_conveyor_group_motion_transforms(int index, void *conveyor_motion_vector) {
    short *group_entry = D_L16_001ABFC0_2CB000[index];
    if (group_entry) {
        do {
            char *moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
            if (MOBY(moby)->oclass == 0x1D7) {
                update_entity_matrix(((char *)MOBY(moby)->pvars) + 0x20,
                                     conveyor_motion_vector,
                                     moby + 0x40,
                                     moby + 0x40);
            }
        } while (*group_entry++ >= 0);
    }
}

#include "qcopy.h"

extern int D_L16_0015F5CC;
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

/* Draw paired textured strips along an indexed pose, fading the end caps. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CB098), where it is exact; names translated to the US level program. */

typedef struct {
    float point[4][4];
    int   color[4];
    struct {
        float u, v;
    } uv[4];
    long zero, texture, flags, mode;
} L16RibbonPacket_u;

extern float D_L16_001D2FF0[][2], D_L16_001D32E0[][2], D_L16_001D3300[][4];
extern float D_L16_001D32C0[][2], D_L16_001D3010[][2], D_L16_001D3300[][4];
extern float D_L16_001D32C0[][2], D_L16_001D32E0[][2], D_L16_001D3030[][4];
extern int   conveyor_belt_pulse_texture_id_gp __asm__("D_L16_00161990") __attribute__((sda));
extern int   conveyor_belt_scrolling_texture_id_gp __asm__("D_L16_00161994") __attribute__((sda));

#include "rnc/overlay/l16/mobys/conveyor_belt_data/draw_conveyor_belt_strips.h"

extern int  conveyor_belt_pulse_color_start_gp __asm__("D_L16_001619A8") __attribute__((sda));
extern int  conveyor_belt_pulse_color_end_gp __asm__("D_L16_001619AC") __attribute__((sda));
extern int  conveyor_belt_scrolling_vertex_color_gp __asm__("D_L16_001619B0") __attribute__((sda));
extern int  D_L16_001600EC_m __asm__("D_L16_001600EC");
extern s64  get_effect_texture(s32) __asm__("func_001F44B8");
extern void draw_geometry_quad_u(void *, void *, int) __asm__("draw_geometry_quad");

void draw_conveyor_belt_strips(struct Moby *moby) {
    L16RibbonPacket_u packets[2];
    char             *conveyor_data = ((char *)moby->pvars);
    float             scale =
        fast_vec_length(D_L16_001600EC_m + ((*(int *)(conveyor_data + 0x60)) << 7)) * 2.001f;
    int   count = truncate_float_to_s32(scale);
    float step = 2.0f / scale;
    int   period, tint, color, limit, j;
    float phase;
    long  modebits, flags;
    (*(float *)((char *)conveyor_data + 0x70)) =
        (*(float *)((char *)conveyor_data + 0x70)) + (*(float *)((char *)conveyor_data + 0x6C));
    if ((*(float *)((char *)conveyor_data + 0x70)) > 1.0f)
        (*(float *)((char *)conveyor_data + 0x70)) -= 1.0f;
    else if ((*(float *)((char *)conveyor_data + 0x70)) < 0.0f)
        (*(float *)((char *)conveyor_data + 0x70)) += 1.0f;
    modebits = 0x8000000000L;
    period = scale_game_frames(120);
    flags = 0xFF9000000260L;
    limit = count + 2;
    phase = convert_integer_to_float(D_L16_0015F5CC % period);
    phase = phase / convert_integer_to_float(period);
    phase = fast_sin(phase * 6.28318f - 3.14159f) * 0.5f + 0.5f;
    tint = fast_tween_color(
        conveyor_belt_pulse_color_start_gp, conveyor_belt_pulse_color_end_gp, phase);
    color = conveyor_belt_scrolling_vertex_color_gp;
    packets[0].texture = get_effect_texture(conveyor_belt_pulse_texture_id_gp);
    packets[0].mode = (long)conveyor_belt_alpha_blend_a_selector |
                      ((long)conveyor_belt_alpha_blend_b_selector << 2) |
                      ((long)conveyor_belt_alpha_blend_c_selector << 4) |
                      ((long)conveyor_belt_alpha_blend_d_selector << 6) | modebits;
    packets[0].flags = flags;
    packets[0].zero = 0;
    packets[1].texture = get_effect_texture(conveyor_belt_scrolling_texture_id_gp);
    packets[1].mode = (long)conveyor_belt_alpha_blend_a_selector |
                      ((long)conveyor_belt_alpha_blend_b_selector << 2) |
                      ((long)conveyor_belt_alpha_blend_c_selector << 4) |
                      ((long)conveyor_belt_alpha_blend_d_selector << 6) | modebits;
    packets[1].flags = flags;
    packets[1].zero = 0;
    {
        int vertex;
        for (vertex = 0; vertex < 4; vertex++) {
            packets[0].uv[vertex].u = D_L16_001D2FF0[vertex][0];
            packets[0].uv[vertex].v = D_L16_001D2FF0[vertex][1];
            packets[1].uv[vertex].u =
                D_L16_001D3010[vertex][0] - (*(float *)(conveyor_data + 0x70));
            packets[1].uv[vertex].v = D_L16_001D3010[vertex][1];
            packets[0].color[vertex] = tint;
            packets[1].color[vertex] = color;
            qcopy(packets[0].point[vertex], D_L16_001D3030[vertex]);
            qcopy(packets[1].point[vertex], D_L16_001D3030[vertex]);
            /* the same vertex of both packets, one packet apart */
            if (vertex < 2) {
                float *pt = packets[0].point[vertex];
                pt[0] -= step;
                pt[sizeof(L16RibbonPacket_u) / sizeof(float)] -= step;
            }
        }
    }
    {
        int strip;
        for (strip = 0; strip < limit; strip++) {
            if (strip == 0) {
                packets[0].color[1] = 0;
                packets[0].color[0] = 0;
                packets[1].color[1] = 0;
                packets[1].color[0] = 0;
            } else if (strip == count + 1) {
                packets[0].color[3] = 0;
                packets[0].color[2] = 0;
                packets[1].color[3] = 0;
                packets[1].color[2] = 0;
            } else if (strip == 1) {
                packets[0].color[1] = tint;
                packets[0].color[0] = tint;
                packets[1].color[1] = color;
                packets[1].color[0] = color;
            }
            draw_geometry_quad_u(
                &packets[0], D_L16_001600EC_m + ((*(int *)(conveyor_data + 0x60)) << 7), 0);
            draw_geometry_quad_u(
                &packets[1], D_L16_001600EC_m + ((*(int *)(conveyor_data + 0x60)) << 7), 0);
            {
                float *point = packets[0].point[0];
                for (j = 3; j >= 0; j--) {
                    float a = *point + step,
                          b = point[sizeof(L16RibbonPacket_u) / sizeof(float)] + step;
                    *point = a;
                    point[sizeof(L16RibbonPacket_u) / sizeof(float)] = b;
                    point += 4;
                }
            }
        }
    }
}

int conveyor_belt_scrolling_vertex_color_gp __asm__("D_L16_001619B0") = 0x50808070;
