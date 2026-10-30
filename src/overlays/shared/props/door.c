/* Shared moby class: door (and hangar_door). */
#include "types.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32  fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");

void door_update(u8 *moby) __asm__("FUN_L07_00310df0");

void door_update(u8 *moby) {
    enum {
        DOOR_INITIALIZE = 0,
        DOOR_WAIT_TO_MOVE_TO_OFFSET_POSITION = 1,
        DOOR_MOVE_TO_OFFSET_POSITION = 2,
        DOOR_WAIT_TO_RETURN_TO_INITIAL_POSITION = 3,
        DOOR_RETURN_TO_INITIAL_POSITION = 4
    };
    u8         *data = ((u8 *)MOBY(moby)->pvars);
    u8         *other;
    f32         vectors[8];
    f32         distance;
    u8          state;
    typedef u32 quadword __attribute__((mode(TI), aligned(16)));

    if (moby[0xb0] != 0xff && visited52[moby[0xb0] + current_level_index * 16] == 0xff)
        *(s32 *)(data + 0x10) = -1;

    state = moby[0x20];
    switch (state) {
    case DOOR_INITIALIZE:
        qcopy(data, moby + 0x10);
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 0;
        moby[0x20] = 1;
        *(s32 *)(data + 0x0c) = 0;
        return;
    case DOOR_WAIT_TO_MOVE_TO_OFFSET_POSITION:
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 0;
        if (*(s32 *)(data + 0x10) != -1) {
            other = level_mobys + *(s32 *)(data + 0x10) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] != *(u32 *)(data + 0x14))
                return;
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = *(f32 *)(data + 0x1c);
        vectors[5] = *(f32 *)(data + 0x20);
        vectors[6] = *(f32 *)(data + 0x24);
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        *(f32 *)(data + 0x0c) = fast_vec_length(vectors);
        if (*(s32 *)(data + 0x30) != -1)
            allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
        moby[0x20] = 2;
        return;
    case DOOR_MOVE_TO_OFFSET_POSITION:
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 1;
        if (*(s32 *)(data + 0x10) != -1) {
            other = level_mobys + *(s32 *)(data + 0x10) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] == *(u32 *)(data + 0x18)) {
                if (*(s32 *)(data + 0x30) != -1)
                    allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
                moby[0x20] = 4;
                return;
            }
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = *(f32 *)(data + 0x1c);
        vectors[5] = *(f32 *)(data + 0x20);
        vectors[6] = *(f32 *)(data + 0x24);
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        transform_vector_by_basis(vectors, vectors, moby + 0xc0);
        fast_vec_normalize(vectors + 4, vectors, *(f32 *)(data + 0x28) * frame_time);
        fast_vec_add(moby + 0x10, moby + 0x10, vectors + 4);
        *(f32 *)(data + 0x0c) -= *(f32 *)(data + 0x28) * frame_time;
        if (!(*(f32 *)(data + 0x0c) < 0.0f))
            return;
        fast_vec_add(moby + 0x10, data, vectors);
        *(f32 *)(data + 0x0c) = 0.0f;
        moby[0x20] = 3;
        if (*(s32 *)(data + 0x34) != -1)
            allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
        return;
    case DOOR_WAIT_TO_RETURN_TO_INITIAL_POSITION:
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 1;
        if (*(s32 *)(data + 0x10) == -1)
            return;
        other = level_mobys + *(s32 *)(data + 0x10) * 0x100;
        if (other == 0 || other[0x20] == 0xfe || other[0x20] == 0xfd ||
            other[0x20] != *(u32 *)(data + 0x18))
            return;
        *(s32 *)(data + 0x0c) = 0;
        if (*(s32 *)(data + 0x30) != -1)
            allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
        moby[0x20] = 4;
        return;
    case DOOR_RETURN_TO_INITIAL_POSITION:
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 0;
        if (*(s32 *)(data + 0x10) == -1 ||
            (other = level_mobys + *(s32 *)(data + 0x10) * 0x100) == 0 || other[0x20] == 0xfe ||
            other[0x20] == 0xfd || other[0x20] == *(u32 *)(data + 0x14)) {
            if (*(s32 *)(data + 0x30) != -1)
                allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
            moby[0x20] = 2;
        } else {
            *(quadword *)(vectors + 4) = 0;
            vectors[4] = *(f32 *)(data + 0x1c);
            vectors[5] = *(f32 *)(data + 0x20);
            vectors[6] = *(f32 *)(data + 0x24);
            *(quadword *)vectors = *(quadword *)(vectors + 4);
            distance = fast_vec_length(vectors);
            fast_vec_normalize(vectors, vectors, -(*(f32 *)(data + 0x2c) * frame_time));
            transform_vector_by_basis(vectors + 4, vectors, moby + 0xc0);
            fast_vec_add(moby + 0x10, moby + 0x10, vectors + 4);
            *(f32 *)(data + 0x0c) += *(f32 *)(data + 0x2c) * frame_time;
            if (!(distance < *(f32 *)(data + 0x0c)))
                return;
            qcopy(moby + 0x10, data);
            if (*(s32 *)(data + 0x30) != -1)
                allocate_voice_for_target_entry(*(s32 *)(data + 0x34), 0, moby);
            if (*(s32 *)(data + 0x38) != -1)
                D_L07_00184528[*(s32 *)(data + 0x38)] = 0;
            moby[0x20] = 1;
        }
        break;
    case 5:
    default:
        return;
    }
}
