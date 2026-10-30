/* Level 07 moby class 0x26: taxi platform. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"

/* Taxi command byte (+0xBC); distinct from Moby.state (+0x20). */
enum TaxiCommand {
    TAXI_SHRINK_AND_RESET_TO_PATH_START = 0x0,
    TAXI_WAIT_FOR_HERO_BOARDING = 0x1,
    TAXI_TRAVERSE_PATH = 0x2,
    TAXI_WAIT_FOR_HERO_TO_LEAVE = 0x3
};

#include "qcopy.h"

#define F(p, o) (*(float *)((p) + (o)))

/* Moves a moby along a path, easing between nodes and tracking the hero. */
extern char *D_L07_001B0530[];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int   is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern int   hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void  set_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L00_00227638");
extern void  update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void  release_voice_slot(s32) __asm__("FUN_0022d798");
extern char  D_0013E550[];
void         update_taxi_platform(unsigned char *moby) __asm__("FUN_L07_002cd2b0");
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
s32          allocate_voice_for_target_entry(s32   entry_index,
                                             s32   flags,
                                             void *target) __asm__("FUN_0022da68");

void update_taxi_platform(unsigned char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    char *path;
    float orig[4], delta[4], path_segment_offset[4];
    if (*(int *)(data + 0xB4) == -1) {
        return;
    }
    path = D_L07_001B0530[*(int *)(data + 0xB4)];
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
    case TAXI_SHRINK_AND_RESET_TO_PATH_START: {
        char *class_defaults = ((char *)MOBY(moby)->pclass);
        float t = F(moby, 0x2C) - F(class_defaults, 0x24) / 60.0f;
        F(moby, 0x2C) = t;
        if (t < 0.0f) {
            F(moby, 0x2C) = F(class_defaults, 0x24);
            qcopy((moby + 0x10), path + 0x10);
            moby[0xBC] = 1;
            F(data, 0xA4) = 0.0f;
        }
        break;
    }
    case TAXI_WAIT_FOR_HERO_BOARDING:
        if (*(short *)(data + 0xB8) != 0) {
            if (is_hero_on_moby(moby)) {
                if (*(int *)(data + 0xC8) == 0) {
                    if (is_hero_on_moby(moby)) {
                        hero_set_state(0x72, 1);
                    }
                }
                moby[0xBC] = 2;
                F(data, 0xAC) = 1.0f / scale_time(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case TAXI_TRAVERSE_PATH:
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
            float segment_fraction_work_value, t;
            char *segment_start_position;
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
                    set_hero_idle_state_for_vehicle_mode();
                }
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
                    unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                    if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                        release_voice_slot(r);
                    }
                }
                *(int *)(data + 0xC4) = -1;
            }
            segment_fraction_work_value = convert_integer_to_float(*(int *)path - 1);
            i = truncate_float_to_s32(segment_fraction_work_value * F(data, 0xA4));
            segment_fraction_work_value = convert_integer_to_float(*(int *)path - 1);
            t = convert_integer_to_float(i);
            segment_fraction_work_value *= F(data, 0xA4);
            segment_fraction_work_value -= t;
            if (i == *(int *)path - 1) {
                qcopy((moby + 0x10), path + 0x10 + i * 16);
            } else {
                segment_start_position = path + 0x10 + i * 16;
                fast_vec_sub(path_segment_offset, path + 0x20 + i * 16, segment_start_position);
                fast_vec_scale(
                    path_segment_offset, path_segment_offset, segment_fraction_work_value);
                fast_vec_add((moby + 0x10), path_segment_offset, segment_start_position);
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
