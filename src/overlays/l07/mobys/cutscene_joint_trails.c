/* Level 07 moby class cutscene_joint_trails (0x6fd): during the cutscene draws three trails between joint pairs of the current cutscene moby. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/mobys/moby.h"

extern char D_L07_0016C8E0[];

/* Level moby state machine: UpdateMoby_1789: spawn state then position three points.
 * Matched using lever: g - (-(idx * 4)) + 0x178 fixes addu operand order. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_0031BDB8.c: func_L07_00320CD0), where it is exact; names translated to the US level program. */

extern short scene_trail_velocity_length __asm__("D_L07_00161CD8") __attribute__((sda));
extern short scene_trail_first_spawn_particle_value __asm__("D_L07_00161CDC") __attribute__((sda));
extern short scene_trail_second_spawn_particle_value __asm__("D_L07_00161CE0") __attribute__((sda));
void         update_cutscene_joint_trails(struct Moby *moby) __asm__("FUN_L07_0031f900");

void update_cutscene_joint_trails(struct Moby *moby) {
    enum {
        TRAIL_STATE_INITIALIZE = 0,
        TRAIL_STATE_UPDATE = 1
    };
    switch (moby->state) {
    case TRAIL_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case TRAIL_STATE_UPDATE:
        if (scene_state == 2) {
            char *trail_source_state = D_L07_0016C8E0;
            int   trail_source_selector = *(int *)(trail_source_state + 0x30);
            if (trail_source_selector == 1 || trail_source_selector == 2) {
                float point_0_position[4], point_1_position[4], point_2_position[4],
                    point_3_position[4], point_4_position[4], point_5_position[4],
                    point_0_trail_velocity[4], point_1_trail_velocity[4], point_2_trail_velocity[4];
                int   trail_source_moby_index = 0;
                char *trail_source_moby;
                if (trail_source_selector == 1)
                    trail_source_moby_index = 3;
                if (trail_source_selector == 2)
                    trail_source_moby_index = 4;
                trail_source_moby =
                    *(char **)(trail_source_state - (-(trail_source_moby_index * 4)) + 0x178);
                moby_point_position(trail_source_moby, 0, point_0_position);
                moby_point_position(trail_source_moby, 3, point_3_position);
                fast_vec_sub(point_0_trail_velocity, point_0_position, point_3_position);
                fast_vec_normalize(point_0_trail_velocity,
                                   point_0_trail_velocity,
                                   *(float *)&scene_trail_velocity_length);
                draw_trail(point_0_position,
                           point_0_position,
                           point_0_trail_velocity,
                           *(float *)&scene_trail_first_spawn_particle_value,
                           *(float *)&scene_trail_second_spawn_particle_value);
                moby_point_position(trail_source_moby, 1, point_1_position);
                moby_point_position(trail_source_moby, 4, point_4_position);
                fast_vec_sub(point_1_trail_velocity, point_1_position, point_4_position);
                fast_vec_normalize(point_1_trail_velocity,
                                   point_1_trail_velocity,
                                   *(float *)&scene_trail_velocity_length);
                draw_trail(point_1_position,
                           point_1_position,
                           point_1_trail_velocity,
                           *(float *)&scene_trail_first_spawn_particle_value,
                           *(float *)&scene_trail_second_spawn_particle_value);
                moby_point_position(trail_source_moby, 2, point_2_position);
                moby_point_position(trail_source_moby, 5, point_5_position);
                fast_vec_sub(point_2_trail_velocity, point_2_position, point_5_position);
                fast_vec_normalize(point_2_trail_velocity,
                                   point_2_trail_velocity,
                                   *(float *)&scene_trail_velocity_length);
                draw_trail(point_2_position,
                           point_2_position,
                           point_2_trail_velocity,
                           *(float *)&scene_trail_first_spawn_particle_value,
                           *(float *)&scene_trail_second_spawn_particle_value);
            }
        }
        break;
    }
}
