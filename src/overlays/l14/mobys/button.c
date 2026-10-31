/* Level 14 button moby class. */
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/level_save_state.h"

extern LevelSaveState D_L14_001BB930;

extern int D_L14_001BABD0[];

extern u8 D_0014C050[];

void button_update(struct Moby *moby) __asm__("FUN_L14_00306b78");

/* Moby class 1416 */
void button_update(struct Moby *moby) {
    enum {
        BUTTON_INITIALIZE = 0,
        BUTTON_WAIT_FOR_HERO_TO_STAND = 1
    };
    s32  *button_pvars = (s32 *)moby->pvars;
    s16   id;
    s32   i;
    s32   list;
    char *path_entry_base;
    f32   angle;
    s32   color;
    u32   shade;
    u32   high;

    switch (moby->state) {
    case BUTTON_INITIALIZE:
        *(f32 *)(button_pvars + 3) = random_angle_radians();
        moby->pos.z -= 0.35f;
        id = (s16)moby->save_id;
        if (D_L14_001BB930.cleared[id] != 0 ||
            (D_0014C190_257218[current_level_index][id >> 5] >> (moby->save_id & 31) & 1) != 0 ||
            (button_pvars[2] != 0 &&
             D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
            moby->command = 2;
            moby->glow_color = 0x80208020;
            moby->state = 2;
            list = button_pvars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)l14_paths[list] > 0) {
                    do {
                        path_entry_base = ((char *)l14_paths[list]) + i * 16;
                        if (*(f32 *)(path_entry_base + 0x1C) == ((f32 *)button_pvars)[1])
                            *(s32 *)(path_entry_base + 0x1C) = 0;
                        list = button_pvars[0];
                        i++;
                    } while (i < *(s32 *)l14_paths[list]);
                }
            }
        } else {
            moby->state = 1;
        }
        break;
    case BUTTON_WAIT_FOR_HERO_TO_STAND:
        angle = fast_add_rotations(*(f32 *)(button_pvars + 3), frame_time * 6.2831855f);
        *(f32 *)(button_pvars + 3) = angle;
        color = truncate_float_to_s32((fast_sin(angle) * 4.0f - 3.0f) * 128.0f);
        if (color > 0x80)
            color = 0x80;
        else if (color < 0x20)
            color = 0x20;
        shade = 0x80000000 | (color << 8);
        high = color << 16;
        moby->glow_color = high | shade | color;
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            id = (s16)moby->save_id;
            D_0014C190_257218[current_level_index][id >> 5] |= 1 << (moby->save_id & 31);
            D_L14_001BABD0[(s16)moby->save_id >> 5] |= 1 << (moby->save_id & 31);
            moby->state = 2;
            moby->command = 1;
            moby->glow_color = 0x80208020;
            allocate_voice_for_target_entry(0, 0, moby);
            list = button_pvars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)l14_paths[list] > 0) {
                    do {
                        path_entry_base = ((char *)l14_paths[list]) + i * 16;
                        if (*(f32 *)(path_entry_base + 0x1C) == ((f32 *)button_pvars)[1])
                            *(s32 *)(path_entry_base + 0x1C) = 0;
                        list = button_pvars[0];
                        i++;
                    } while (i < *(s32 *)l14_paths[list]);
                }
            }
        }
        break;
    }
}
