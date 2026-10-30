/* Shared moby class novalis_elevator: a path mover gated by an activation trigger. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/level_save_state.h"

extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern void  update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
float        absolute_float(float input) __asm__("func_001F99C0");
#define F(p, o) (*(float *)((p) + (o)))

/* Path mover gated by an activation trigger and its collectable flag; plays the arrival cue on the way back. */

void novalis_elevator_update(unsigned char *moby) __asm__("FUN_L01_002b9eb0");

void novalis_elevator_update(unsigned char *moby) {
    enum {
        NOVALIS_ELEVATOR_WAIT_AT_PATH_END = 0,
        NOVALIS_ELEVATOR_WAIT_AT_PATH_START = 1,
        NOVALIS_ELEVATOR_TRAVERSE_PATH = 2
    };
    char *data = ((char *)MOBY(moby)->pvars);
    char *path;
    float orig[4], delta[4], segment_offset[4];
    int   on = 0;
    if (*(int *)(data + 0xB4) == -1) {
        return;
    }
    path = ((char *)l01_paths[*(int *)(data + 0xB4)]);
    if (*(int *)(data + 0xCC) != -1) {
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xCC)) == 0) {
            moby[0x30] = 0xFF;
            MOBY(moby)->collision = 0;
            MOBY(moby)->flags |= 0x41;
            return;
        }
        if (*(int *)(data + 0xCC) != -1 &&
            is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xCC))) {
            MOBY(moby)->flags &= 0xFFBE;
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            *(int *)(data + 0xCC) = -1;
        }
    }
    if (*(int *)(data + 0xC0) != -1) {
        unsigned short id;
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xC0)) ||
            l01_save_state.cleared[(short)(id = MOBY(moby)->save_id)] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            if (*(int *)(data + 0xC0) != -1) {
                D_0014C190_257218[current_level_index][(short)MOBY(moby)->save_id >> 5] |=
                    1 << (MOBY(moby)->save_id & 0x1F);
                moby_killed[(short)MOBY(moby)->save_id >> 5] |= 1 << (MOBY(moby)->save_id & 0x1F);
                *(int *)(data + 0xC0) = -1;
            }
            on = 1;
        }
    } else {
        on = 1;
    }
    qcopy(orig, moby + 0x10);
    fast_dec_timer_s16(data + 0xBA);
    if (moby[0x20] == 0) {
        float f;
        char *pts;
        data[0x28] = 4;
        *(short *)(data + 0x3E) = 5;
        *(int *)(data + 0x20) = 0;
        *(short *)(data + 0x24) = 0;
        f = 0.0f;
        if ((moby[0xBC] = data[0xA0]) == 0) {
            f = 1.0f;
        }
        F(data, 0xA4) = f;
        pts = path + 0x10;
        qcopy((moby + 0x10), truncate_float_to_s32(f) ? pts + (*(int *)path - 1) * 16 : pts);
        *(short *)(data + 0xBA) = 0;
        *(int *)(data + 0xA8) = 0;
        *(int *)(data + 0xBC) = 0;
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        *(int *)(data + 0xC4) = -1;
    }
    switch (moby[0xBC]) {
    case NOVALIS_ELEVATOR_WAIT_AT_PATH_END:
        if (on) {
            float far;
            if ((*(short *)(data + 0xB8) != 0 && is_hero_on_moby(moby)) ||
                (far = fast_vec_dist(&hero.motion.pos, path + *(int *)path * 16),
                 fast_vec_dist(&hero.motion.pos, path + 0x10) < far)) {
                moby[0xBC] = 2;
                F(data, 0xAC) = -1.0f / scale_time(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case NOVALIS_ELEVATOR_WAIT_AT_PATH_START:
        if (on) {
            float near;
            if ((*(short *)(data + 0xB8) != 0 && is_hero_on_moby(moby)) ||
                (near = fast_vec_dist(&hero.motion.pos, path + 0x10),
                 fast_vec_dist(&hero.motion.pos, path + *(int *)path * 16) < near)) {
                moby[0xBC] = 2;
                F(data, 0xAC) = 1.0f / scale_time(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case NOVALIS_ELEVATOR_TRAVERSE_PATH:
        if (*(int *)(data + 0xC8) == 0 && is_hero_on_moby(moby)) {
            struct Hero *player = &hero;
            player->jump_lock_timer = 4;
            player->unk1F4 = 4;
        }
        if (*(short *)(data + 0xBA) != 0 ||
            (F(data, 0xBC) < 0.0f && vector_distance_xy((moby + 0x10), &hero.motion.pos) < 2.83f &&
             absolute_float(hero.motion.pos.f[2] - F(moby, 0x18)) < 4.0f &&
             F(moby, 0x18) - hero.motion.pos.f[2] > 0.9f)) {
            int r;
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xBA) = scale_game_frames(30);
            }
            r = *(int *)(data + 0xC4);
            *(int *)(data + 0xA8) = 0;
            if (r != -1) {
                unsigned char *s = (unsigned char *)((char *)&voice_pool) + r * 0x70;
                if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                    release_voice_slot(r);
                }
            }
            *(int *)(data + 0xC4) = -1;
        } else {
            int   i;
            float segment_interpolation_fraction, path_point_index_float;
            char *segment_start;
            if (!voice_is_playing(moby, *(int *)(data + 0xC4))) {
                *(int *)(data + 0xC4) = allocate_voice_for_target_entry(0, 4, (int)moby);
            }
            F(data, 0xA8) += F(data, 0xAC) * frame_time;
            if (absolute_float(F(data, 0xA8)) > absolute_float(F(data, 0xAC))) {
                F(data, 0xA8) = F(data, 0xAC);
            }
            F(data, 0xA4) = F(data, 0xA4) + F(data, 0xA8);
            if (0.5f < absolute_float(F(data, 0xA4) - 0.5f)) {
                int r;
                if (0.0f < F(data, 0xAC)) {
                    moby[0xBC] = 0;
                    F(data, 0xA4) = 1.0f;
                } else {
                    moby[0xBC] = 1;
                    F(data, 0xA4) = 0.0f;
                }
                *(int *)(data + 0xA8) = 0;
                *(short *)(data + 0xBA) = scale_game_frames(15);
                r = *(int *)(data + 0xC4);
                if (r != -1) {
                    unsigned char *s = (unsigned char *)((char *)&voice_pool) + r * 0x70;
                    if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                        release_voice_slot(r);
                    }
                }
                *(int *)(data + 0xC4) = -1;
            }
            segment_interpolation_fraction = convert_integer_to_float(*(int *)path - 1);
            i = truncate_float_to_s32(segment_interpolation_fraction * F(data, 0xA4));
            segment_interpolation_fraction = convert_integer_to_float(*(int *)path - 1);
            path_point_index_float = convert_integer_to_float(i);
            segment_interpolation_fraction *= F(data, 0xA4);
            segment_interpolation_fraction -= path_point_index_float;
            if (i == *(int *)path - 1) {
                qcopy((moby + 0x10), path + 0x10 + i * 16);
            } else {
                segment_start = path + 0x10 + i * 16;
                fast_vec_sub(segment_offset, path + 0x20 + i * 16, segment_start);
                fast_vec_scale(segment_offset, segment_offset, segment_interpolation_fraction);
                fast_vec_add((moby + 0x10), segment_offset, segment_start);
            }
            F(data, 0xBC) = F(moby, 0x18) - orig[2];
        }
        break;
    }
    fast_vec_sub(delta, (moby + 0x10), orig);
    update_entity_matrix(data + 0x60, delta, moby + 0x40, moby + 0x40);
    if (!is_hero_on_moby(moby)) {
        if (2.0f < vector_distance_xy((moby + 0x10), &hero.motion.pos)) {
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xB8) = 1;
            }
        }
    }
}
