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
#include "rnc/overlay/level_path.h"
#include "rnc/game/item_state.h"

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} SaveFlags;

extern SaveFlags D_L05_001BB6B0;
extern SaveFlags l05_moby_save_flags __asm__("D_L05_001BAA50");

extern unsigned char D_L05_0015FC88[] MACRO_ADDR;
extern u8            D_0014C050[];

void floor_button_update(struct Moby *moby) __asm__("FUN_L05_0030c5b0");

/* Moby class 830 */
void floor_button_update(struct Moby *moby) {
    s32  *vars = (s32 *)moby->pvars;
    s16   id;
    s32   i;
    s32   list;
    char *entry;
    f32   angle;
    s32   color;
    u32   shade;
    u32   high;

    switch (moby->state) {
    case 0:
        *(f32 *)(vars + 3) = random_angle_radians();
        moby->pos.z -= 0.35f;
        id = (s16)moby->save_id;
        if (D_L05_001BB6B0.collected[id] != 0 ||
            (D_0014C190_257218[current_level_index][id >> 5] >> (moby->save_id & 31) & 1) != 0 ||
            (vars[2] != 0 && D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
            moby->command = 2;
            moby->glow_color = 0x80208020;
            moby->state = 2;
            list = vars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)l05_paths[list] > 0) {
                    do {
                        entry = ((char *)l05_paths[list]) + i * 16;
                        if (*(f32 *)(entry + 0x1C) == ((f32 *)vars)[1])
                            *(s32 *)(entry + 0x1C) = 0;
                        list = vars[0];
                        i++;
                    } while (i < *(s32 *)l05_paths[list]);
                }
            }
        } else {
            moby->state = 1;
        }
        break;
    case 1:
        angle = fast_add_rotations(*(f32 *)(vars + 3), frame_time * 6.2831855f);
        *(f32 *)(vars + 3) = angle;
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
                D_L05_001BB6B0.collected[(s16)moby->save_id] = moby->mission + 2;
            }
            moby->state = 2;
            moby->command = 1;
            moby->glow_color = 0x80208020;
            allocate_voice_for_target_entry(0, 0, moby);
            list = vars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)l05_paths[list] > 0) {
                    do {
                        entry = ((char *)l05_paths[list]) + i * 16;
                        if (*(f32 *)(entry + 0x1C) == ((f32 *)vars)[1])
                            *(s32 *)(entry + 0x1C) = 0;
                        list = vars[0];
                        i++;
                    } while (i < *(s32 *)l05_paths[list]);
                }
            }
        }
        break;
    }
}

unsigned char D_L17_0015FC88 = 0;
