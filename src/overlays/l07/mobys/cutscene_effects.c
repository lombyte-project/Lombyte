/* Level 07 moby class cutscene_effects (0x610): aims the cutscene camera and bursts effects from the cutscene moby's points. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/moby.h"

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_0031BDB8.c: func_L07_0031FF78), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x178];
    int  arr[8];
} L07SparkState;

extern char D_L07_0016C8E0[];
extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");
unsigned char *
FUN_L00_0026d000(void *pos, int n, void *vel, int color, float r, float lo, float hi, float size);
extern char *func_L00_0026DEA0_c(void *, int, void *, int, float, float, float, float) __asm__("FUN_L00_0026d000");
extern char D_L07_0016C960_c[] __asm__("D_L07_0016C8E0");

void cutscene_effects_update(struct Moby *moby) __asm__("FUN_L07_0031eba8");

/* Moby class 1552 */
void cutscene_effects_update(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_UPDATE = 1
    };
    int   state = moby->state;
    int   current_scene_state;
    int   k;
    int   scene_state_offset;
    int   i;
    int   j;
    char *mob;
    char *S;
    float effect_spawn_position[4];
    switch (state) {
    case MOBY_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case MOBY_STATE_UPDATE:
        current_scene_state = scene_state;
        if (current_scene_state != 2)
            return;
        S = D_L07_0016C960_c;
        scene_state_offset = *(int *)(S + 0x30) - 3;
        if ((unsigned)scene_state_offset < 2) {
            int camera_target_index = (unsigned)scene_state_offset > 1 ? 0 : 2;
            cutscene_set_camera_target(((L07SparkState *)S)->arr[camera_target_index]);
        }
        if (*(int *)(S + 0x30) != current_scene_state)
            return;
        k = 7;
        if (*(int *)(S + 0x34) != scale_game_frames(0xA8A))
            k = 0;
        if (*(int *)(S + 0x34) == scale_game_frames(0xACE))
            k = 6;
        if (k != 0) {
            mob = (char *)((L07SparkState *)S)->arr[k];
            for (i = 0; i < 4; i++) {
                moby_point_position(mob, i, effect_spawn_position);
                for (j = 9; j >= 0; j--) {
                    float effect_size = random_float_between(20000.0f, 200000.0f);
                    int   col = (random_int_between(0x20, 0x80) << 24) | 0x7F7F7F;
                    int   signed_random_value = random_int_between(0, 4);
                    char *effect;
                    char *effect_tail;
                    if (random_integer_below(2) != 0)
                        signed_random_value = -signed_random_value;
                    effect = func_L00_0026DEA0_c(effect_spawn_position,
                                                 signed_random_value,
                                                 D_L07_0015F580,
                                                 col,
                                                 0.6f,
                                                 1.0f,
                                                 1.01f,
                                                 effect_size);
                    if (effect != 0) {
                        effect_tail = effect + 0x20;
                        *(short *)(effect + 0xA) =
                            scale_game_frames(random_int_between(0x3C, 0x5A));
                        *(int *)(effect_tail + 4) = 2;
                        *(char *)(effect_tail + 0xA) = col >> 24;
                        *(char *)(effect_tail + 0xB) = *(unsigned char *)(effect + 0xA);
                    }
                }
            }
        }
        break;
    }
}
