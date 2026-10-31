/* Level 14 unlock sequence controller moby class. */
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/moby_spawn.h"
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_save_state.h"

extern char *D_L14_0015FFD8;

extern LevelSaveState D_L14_001BB930;

extern int D_L14_001BABD0[];

extern void activate_descriptor(int) __asm__("FUN_L10_002f5a50");

void save_and_lower_unlock_sequence_mobys(struct Moby *moby) __asm__("FUN_L14_00305600");

extern void save_and_lower_unlock_sequence_mobys(struct Moby *) __asm__("FUN_L14_00305600");

void raise_unlock_sequence_mobys(struct Moby *moby) __asm__("FUN_L14_00305680");

extern void raise_unlock_sequence_mobys(struct Moby *) __asm__("FUN_L14_00305680");

void unlock_sequence_controller_update(struct Moby *moby) __asm__("FUN_L14_00305408");

/* Moby class 1352 */
void unlock_sequence_controller_update(struct Moby *moby) {
    enum {
        UNLOCK_SEQUENCE_INITIALIZE = 0,
        UNLOCK_SEQUENCE_WAIT_FOR_TRIGGER = 1,
        UNLOCK_SEQUENCE_COUNTDOWN = 2
    };
    int  *unlock_sequence_data = ((int *)moby->pvars);
    short id;

    switch (moby->state) {
    case UNLOCK_SEQUENCE_INITIALIZE:
        if (unlock_sequence_data[3] < 0 || unlock_sequence_data[0] < 0 ||
            unlock_sequence_data[4] < 0) {
            mark_moby_for_removal(moby);
            return;
        }
        id = ((short)moby->save_id);
        if (D_L14_001BB930.cleared[id] != 0) {
            moby->state = 3;
            return;
        }
        if (D_0014C190_257218[current_level_index][id >> 5] >> (moby->save_id & 0x1F) & 1) {
            moby->state = 3;
            return;
        }
        moby->state = 1;
        save_and_lower_unlock_sequence_mobys((char *)moby);
        break;
    case UNLOCK_SEQUENCE_WAIT_FOR_TRIGGER:
        if (((unsigned char *)D_L14_0015FFD8)[unlock_sequence_data[3] * 0x100 + 0x20] != 2) {
            return;
        }
        D_0014C190_257218[current_level_index][((short)moby->save_id) >> 5] |=
            1 << (moby->save_id & 0x1F);
        D_L14_001BABD0[((short)moby->save_id) >> 5] |= 1 << (moby->save_id & 0x1F);
        moby->state = 2;
        unlock_sequence_data[1] = scale_game_frames(0x78);
        ((float *)unlock_sequence_data)[2] =
            1.0f / convert_integer_to_float(unlock_sequence_data[1]);
        activate_descriptor(unlock_sequence_data[4]);
        break;
    case UNLOCK_SEQUENCE_COUNTDOWN:
        fast_dec_timer(&unlock_sequence_data[1]);
        raise_unlock_sequence_mobys((char *)moby);
        if (unlock_sequence_data[1] == 0) {
            moby->state = 3;
        }
        break;
    }
}

extern float unlock_sequence_saved_moby_heights[] __asm__("D_L14_001EE5E0");

extern unsigned short *D_L14_001ABF40[];

void save_and_lower_unlock_sequence_mobys(struct Moby *moby) {
    unsigned short *moby_id_cursor = D_L14_001ABF40[*(int *)moby->pvars];
    float          *saved_moby_height_write_cursor;
    if (moby_id_cursor != 0) {
        int moby_array_base = D_L14_0015FFD8;
        saved_moby_height_write_cursor = unlock_sequence_saved_moby_heights;
        do {
            float *target_moby = (float *)(moby_array_base + ((*moby_id_cursor & 0x7FFF) << 8));
            *saved_moby_height_write_cursor++ = target_moby[6];
            target_moby[6] -= 20.0f;
            *(unsigned short *)((char *)target_moby + 0x34) |= 0x41;
        } while (*(short *)moby_id_cursor++ >= 0);
    }
}

extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");

void raise_unlock_sequence_mobys(struct Moby *moby) {
    int   *unlock_sequence_data = (int *)moby->pvars;
    short *moby_id_cursor = (short *)D_L14_001ABF40[unlock_sequence_data[0]];
    float  height_blend_factor;
    float *saved_height_cursor;
    char  *level_moby_array;
    float  x;
    if (moby_id_cursor == 0) {
        return;
    }
    x = convert_integer_to_float(unlock_sequence_data[1]) * ((float *)unlock_sequence_data)[2];
    height_blend_factor = cubic_hermite_scalar(-1.0f, 0.0f, 1.0f, 0.0f, 1.0f - x);
    level_moby_array = D_L14_0015FFD8;
    saved_height_cursor = unlock_sequence_saved_moby_heights;
    do {
        char *target_moby = level_moby_array + ((*(unsigned short *)moby_id_cursor & 0x7FFF) << 8);
        float saved_height = *saved_height_cursor++;
        float lo = saved_height - 20.0f;
        MOBY(target_moby)->flags &= 0xFFBE;
        MOBY(target_moby)->pos.z = lo + (saved_height - lo) * height_blend_factor;
    } while (*moby_id_cursor++ >= 0);
}
