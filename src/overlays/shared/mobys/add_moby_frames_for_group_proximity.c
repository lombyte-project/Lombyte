/* Adds frames to a moby while a group member is near it. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"

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
