/* Level 08 moby class 0x26d: landmine. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

#include "rnc/overlay/swarm_path.h"
#include "rnc/data_labels.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/audio/voice_alloc.h"

extern f32                     D_L08_0015F580[] MACRO_ADDR;
extern struct Moby *D_0013F58C NOT_SDA;
/* Retail loads gp - 0x4C90; GP 0x166C00 places this scalar at 0x161F70. */

extern f32 approach_value(void *, f32, f32) __asm__("FUN_00213ed8");
extern void
FUN_L00_00258278_44c0(struct Moby *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_00258278");
extern void spawn_explosion(char *,
                            void *,
                            char *,
                            f32,
                            f32,
                            s32,
                            s32,
                            s32,
                            f32,
                            f32,
                            f32,
                            f32,
                            s32,
                            f32,
                            s32,
                            s32,
                            s32,
                            u8) __asm__("FUN_L00_0025e450");

extern f32 landmine_spin_rate_degrees_gp __asm__("D_L08_00161F70") __attribute__((sda));
void       update_landmine(struct Moby *moby) __asm__("FUN_L08_002f44c0");

void update_landmine(struct Moby *moby) {
    enum {
        PATH_FOLLOWER_INITIALIZE_HEIGHT_OFFSET = 0,
        PATH_FOLLOWER_CHECK_HIT_AND_PROXIMITY = 1,
        PATH_FOLLOWER_FOLLOW_PATH = 2,
        PATH_FOLLOWER_FADE_OUT = 3
    };
    SwarmPathFollower *follower = (SwarmPathFollower *)moby->pvars;
    Vec4               point;
    Vec4               step;
    Vec4               velocity; /* Effect scratch is separate from the waypoint movement step. */
    f32                fade;
    s32                waypoint;
    f32                distance;

    switch (moby->state) {
    case PATH_FOLLOWER_INITIALIZE_HEIGHT_OFFSET: {
        fade = moby->pos.z;
        moby->state = 1;
        moby->pos.z = fade + 0.4f;
        return;
    }
    case PATH_FOLLOWER_CHECK_HIT_AND_PROXIMITY: {
        s32 active = find_moby_hit(moby, 0x330000, 0);
        distance = vector_distance_xy(&moby->pos, D_0013F3D0);
        if ((distance < 2.0f && *(s32 *)(D_0013F3D0 + 0x2004) == 0x42) ||
            *(struct Moby **)(D_0013F3D0 + 0x1bc) == moby) {
            allocate_voice_for_target_entry(0, 0, moby);
            spawn_explosion(moby,
                            D_L08_0015F580,
                            &moby->pos,
                            3.0f,
                            1.0f,
                            10,
                            20,
                            60,
                            3.0f,
                            5.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            mark_moby_for_removal(moby);
            return;
        }
        if (active != 0) {
            allocate_voice_for_target_entry(0, 0, moby);
            spawn_explosion(moby,
                            D_L08_0015F580,
                            &moby->pos,
                            0.0f,
                            0.0f,
                            10,
                            20,
                            60,
                            1.0f,
                            3.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            mark_moby_for_removal(moby);
        }
        return;
    }
    case PATH_FOLLOWER_FOLLOW_PATH: {
        moby->pos.z -= 0.4f;
        if (follower->path_id == -1 || follower->path->count == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->rot.y = fast_add_rotations(moby->rot.y,
                                         landmine_spin_rate_degrees_gp * 0.017453292f * frame_time);
        distance = vector_distance_xy(&moby->pos, D_0013F3D0);
        if (distance < 2.0f && *(s32 *)(D_0013F3D0 + 0x2004) == 0x42) {
            clear_u64_value(&velocity);
            spawn_explosion(moby,
                            &velocity,
                            &moby->pos,
                            0.0f,
                            0.0f,
                            10,
                            20,
                            60,
                            3.0f,
                            5.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            mark_moby_for_removal(moby);
            return;
        }
        waypoint =
            (follower->point + follower->path->count + follower->direction) % follower->path->count;
        FUN_L00_00258278_44c0(moby,
                              fast_arc_tan(follower->path->points[waypoint].f[0] - moby->pos.x,
                                           follower->path->points[waypoint].f[1] - moby->pos.y),
                              (f32 *)((char *)follower + 0x34),
                              0.01f,
                              0.3f,
                              0.1f);
        qcopy(&point, &follower->path->points[waypoint]);
        if (waypoint != 0) {
            fast_vec_sub(&step, &point, &moby->pos);
            fast_vec_normalize(&step, &step, frame_time * 3.0f);
            fast_vec_add(&moby->pos, &moby->pos, &step);
        } else {
            distance = vector_distance_xy(&moby->pos, D_0013F3D0);
            if (distance < 24.0f) {
                clear_u64_value(&velocity);
                spawn_explosion(moby,
                                &velocity,
                                &moby->pos,
                                0.0f,
                                0.0f,
                                10,
                                20,
                                60,
                                3.0f,
                                5.0f,
                                9.0f,
                                1.0f,
                                -1,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                allocate_voice_for_target_entry(0, 0, moby);
                mark_moby_for_removal(moby);
                return;
            }
            moby->state = 3;
        }
        if (fast_vec_dist(&moby->pos, &point) < 0.1f) {
            follower->point = waypoint;
        }
        moby->pos.z += 0.4f;
        return;
    }
    case PATH_FOLLOWER_FADE_OUT: {
        Vec4 timer;
        timer.f[0] = (f32)moby->alpha;
        approach_value(&timer, 0.0f, 128.0f / (f32)scale_game_frames(30));
        moby->alpha = (u8)(s32)timer.f[0];
        if (moby->alpha == 0)
            mark_moby_for_removal(moby);
    }
    }
}
