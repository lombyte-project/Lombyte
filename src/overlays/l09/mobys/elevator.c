/* Level 09 moby classes 0x47e, 0x47f: elevator. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/level_path.h"

#define F(p, o) (*(float *)((p) + (o)))

/* Moves a moby along a path between its ends once its collectable flag is set, easing between nodes. */
typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L09LevelState;

extern L09LevelState l09_moby_collection_state __asm__("D_L09_001BB3B0");
extern int           elevator_activation_save_id_bits[] __asm__("D_L09_001BA650");
extern int           is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern float         scale_time(float) __asm__("FUN_001f96b0");
extern int           is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern char          D_0013E550[];
extern s32           level_moby_killed[][64] __asm__("D_0014C190");
void                 elevator_update(unsigned char *moby) __asm__("FUN_L09_003033a0");

void elevator_update(unsigned char *moby) {
    enum {
        ELEVATOR_STATE_AT_PATH_END = 0,
        ELEVATOR_STATE_AT_PATH_START = 1,
        ELEVATOR_STATE_MOVING = 2
    };
    char *data = *(char **)(moby + 0x78);
    char *path;
    float orig[4], delta[4], segment_offset[4];
    int   on = 0;
    if (*(int *)(data + 0xB4) == -1) {
        return;
    }
    path = ((char *)l09_paths[*(int *)(data + 0xB4)]);
    if (*(int *)(data + 0xC0) != -1) {
        unsigned short id;
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xC0)) ||
            l09_moby_collection_state.collected[(short)(id = *(unsigned short *)(moby + 0xB2))] !=
                0 ||
            (level_moby_killed[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            if (*(int *)(data + 0xC0) != -1) {
                level_moby_killed[current_level_index]
                                 [(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
                    1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
                elevator_activation_save_id_bits[(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
                    1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
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
        *(short *)(moby + 0x32) = 0xFF;
        *(int *)(data + 0xC4) = -1;
    }
    switch (moby[0xBC]) {
    case ELEVATOR_STATE_AT_PATH_END:
        if (on) {
            float far;
            if ((*(short *)(data + 0xB8) != 0 && is_hero_on_moby(moby)) ||
                (far = fast_vec_dist(&hero.motion.pos, path + *(int *)path * 16),
                 fast_vec_dist(&hero.motion.pos, path + 0x10) < far)) {
                if (*(int *)(data + 0xC8) == 0 && is_hero_on_moby(moby)) {
                    hero_set_state(0x72, 1);
                }
                moby[0xBC] = 2;
                F(data, 0xAC) = -1.0f / scale_time(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case ELEVATOR_STATE_AT_PATH_START:
        if (on) {
            float near;
            if ((*(short *)(data + 0xB8) != 0 && is_hero_on_moby(moby)) ||
                (near = fast_vec_dist(&hero.motion.pos, path + 0x10),
                 fast_vec_dist(&hero.motion.pos, path + *(int *)path * 16) < near)) {
                if (*(int *)(data + 0xC8) == 0 && is_hero_on_moby(moby)) {
                    hero_set_state(0x72, 1);
                }
                allocate_voice_for_target_entry(1, 0, moby);
                moby[0xBC] = 2;
                F(data, 0xAC) = 1.0f / scale_time(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case ELEVATOR_STATE_MOVING:
        if (*(short *)(data + 0xBA) != 0 ||
            (F(data, 0xBC) < 0.0f && vector_distance_xy((moby + 0x10), &hero.motion.pos) < 2.0f &&
             absolute_float(hero.motion.pos.f[2] - F(moby, 0x18)) < 4.0f &&
             F(moby, 0x18) - hero.motion.pos.f[2] > 1.0f)) {
            int r;
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xBA) = scale_game_frames(30);
            }
            r = *(int *)(data + 0xC4);
            *(int *)(data + 0xA8) = 0;
            if (r != -1) {
                unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                    release_voice_slot(r);
                }
            }
            *(int *)(data + 0xC4) = -1;
        } else {
            int   i;
            float segment_fraction, node_index_as_float;
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
                if (*(int *)(data + 0xC8) == 0 && is_hero_on_moby(moby)) {
                    hero_set_state(HERO_STATE_IDLE, 1);
                }
                if (0.0f < F(data, 0xAC)) {
                    moby[0xBC] = 0;
                    F(data, 0xA4) = 1.0f;
                } else {
                    moby[0xBC] = 1;
                    F(data, 0xA4) = 0.0f;
                }
                allocate_voice_for_target_entry(2, 0, moby);
                *(int *)(data + 0xA8) = 0;
                *(short *)(data + 0xBA) = scale_game_frames(15);
                r = *(int *)(data + 0xC4);
                if (r != -1) {
                    unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                    if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                        release_voice_slot(r);
                    }
                }
                *(int *)(data + 0xC4) = -1;
            }
            segment_fraction = convert_integer_to_float(*(int *)path - 1);
            i = truncate_float_to_s32(segment_fraction * F(data, 0xA4));
            segment_fraction = convert_integer_to_float(*(int *)path - 1);
            node_index_as_float = convert_integer_to_float(i);
            segment_fraction *= F(data, 0xA4);
            segment_fraction -= node_index_as_float;
            if (i == *(int *)path - 1) {
                qcopy((moby + 0x10), path + 0x10 + i * 16);
            } else {
                segment_start = path + 0x10 + i * 16;
                fast_vec_sub(segment_offset, path + 0x20 + i * 16, segment_start);
                fast_vec_scale(segment_offset, segment_offset, segment_fraction);
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
