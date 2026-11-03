/* Shared moby class floor_button. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/game/item_state.h"

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} SaveFlags;

extern SaveFlags l05_mission_gated_moby_save_flags __asm__("D_L05_001BB6B0");
extern SaveFlags l05_moby_save_flags __asm__("D_L05_001BAA50");

extern unsigned char D_L05_0015FC88[] MACRO_ADDR;
extern u8            D_0014C050[];

void floor_button_update(struct Moby *moby) __asm__("FUN_L05_0030c5b0");

/* Moby class 830 */
void floor_button_update(struct Moby *moby) {
    enum {
        FLOOR_BUTTON_STATE_INITIALIZE = 0,
        FLOOR_BUTTON_STATE_WAIT_FOR_PRESS = 1
    };
    enum {
        FLOOR_BUTTON_STATE_DISABLED = 2
    };
    s32  *floor_button_private_state = (s32 *)moby->pvars;
    s16   save_flag_index;
    s32   i;
    s32   list;
    char *path_key_cursor;
    f32   angle;
    s32   color;
    u32   shade;
    u32   high;

    switch (moby->state) {
    case FLOOR_BUTTON_STATE_INITIALIZE:
        *(f32 *)(floor_button_private_state + 3) = random_angle_radians();
        moby->pos.z -= 0.35f;
        save_flag_index = (s16)moby->save_id;
        if (l05_mission_gated_moby_save_flags.collected[save_flag_index] != 0 ||
            (D_0014C190_257218[current_level_index][save_flag_index >> 5] >> (moby->save_id & 31) & 1) != 0 ||
            (floor_button_private_state[2] != 0 &&
             D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
            moby->command = 2;
            moby->glow_color = 0x80208020;
            moby->state = FLOOR_BUTTON_STATE_DISABLED;
            list = floor_button_private_state[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)l05_paths[list] > 0) {
                    do {
                        path_key_cursor = ((char *)l05_paths[list]) + i * 16;
                        if (*(f32 *)(path_key_cursor + 0x1C) ==
                            ((f32 *)floor_button_private_state)[1])
                            *(s32 *)(path_key_cursor + 0x1C) = 0;
                        list = floor_button_private_state[0];
                        i++;
                    } while (i < *(s32 *)l05_paths[list]);
                }
            }
        } else {
            moby->state = FLOOR_BUTTON_STATE_WAIT_FOR_PRESS;
        }
        break;
    case FLOOR_BUTTON_STATE_WAIT_FOR_PRESS:
        angle =
            fast_add_rotations(*(f32 *)(floor_button_private_state + 3), frame_time * 6.2831855f);
        *(f32 *)(floor_button_private_state + 3) = angle;
        color = truncate_float_to_s32((fast_sin(angle) * 4.0f - 3.0f) * 128.0f);
        if (color > 0x80)
            color = 0x80;
        else if (color < 0x20)
            color = 0x20;
        shade = 0x80000000 | (color << 8);
        high = color << 16;
        moby->glow_color = high | shade | color;
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            l05_moby_save_flags.collected[(s16)moby->save_id] = moby->mission + 2;
            if (moby->mission == 0xFF ||
                (D_L05_0015FC88[moby->mission] != 0xFF &&
                 D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
                l05_mission_gated_moby_save_flags.collected[(s16)moby->save_id] = moby->mission + 2;
            }
            moby->state = FLOOR_BUTTON_STATE_DISABLED;
            moby->command = 1;
            moby->glow_color = 0x80208020;
            allocate_voice_for_target_entry(0, 0, moby);
            list = floor_button_private_state[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)l05_paths[list] > 0) {
                    do {
                        path_key_cursor = ((char *)l05_paths[list]) + i * 16;
                        if (*(f32 *)(path_key_cursor + 0x1C) ==
                            ((f32 *)floor_button_private_state)[1])
                            *(s32 *)(path_key_cursor + 0x1C) = 0;
                        list = floor_button_private_state[0];
                        i++;
                    } while (i < *(s32 *)l05_paths[list]);
                }
            }
        }
        break;
    }
}

unsigned char D_L17_0015FC88 = 0;
