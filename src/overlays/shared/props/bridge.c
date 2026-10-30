/* Shared moby class: bridge. */
#include "types.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/mobys/moby.h"

extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32  fast_vec_length(void *) __asm__("FUN_001f9af0");

extern void fast_vec_scale(f32, void *, void *) __asm__("FUN_001f9a68");
extern void scale_vector_xyz_alt_31aee0(f32, void *, void *) __asm__("FUN_001f9bf8");

void bridge_update(u8 *moby) __asm__("FUN_L07_0031aee0");

void bridge_update(u8 *moby) {
    enum {
        BRIDGE_INITIALIZE = 0,
        BRIDGE_WAIT_TO_MOVE_TO_OFFSET_POSITION = 1,
        BRIDGE_MOVE_TO_OFFSET_POSITION = 2,
        BRIDGE_WAIT_TO_RETURN_TO_INITIAL_POSITION = 3,
        BRIDGE_RETURN_TO_INITIAL_POSITION = 4
    };
    u8         *data = ((u8 *)MOBY(moby)->pvars);
    u8         *other;
    f32         delta[4];
    f32         old_rotation[4];
    f32         vectors[8];
    f32         distance;
    u8          state;
    typedef u32 quadword __attribute__((mode(TI), aligned(16)));

    fast_vec_scale(-1.0f, delta, moby + 0x10);
    qcopy(old_rotation, moby + 0x40);

    if (moby[0xb0] != 0xff && visited52[moby[0xb0] + current_level_index * 16] == 0xff)
        *(s32 *)(data + 0xb0) = -1;

    state = moby[0x20];
    switch (state) {
    case BRIDGE_INITIALIZE:
        qcopy(data + 0xa0, moby + 0x10);
        if (*(s32 *)(data + 0xd8) != -1)
            D_L07_00184528[*(s32 *)(data + 0xd8)] = 0;
        moby[0x20] = 1;
        *(s32 *)(data + 0xac) = 0;
        break;
    case BRIDGE_WAIT_TO_MOVE_TO_OFFSET_POSITION:
        if (*(s32 *)(data + 0xb0) != -1) {
            other = level_mobys + *(s32 *)(data + 0xb0) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] != *(u32 *)(data + 0xb4))
                break;
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = *(f32 *)(data + 0xbc);
        vectors[5] = *(f32 *)(data + 0xc0);
        vectors[6] = *(f32 *)(data + 0xc4);
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        *(f32 *)(data + 0xac) = fast_vec_length(vectors);
        if (*(s32 *)(data + 0xd0) != -1)
            allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
        moby[0x20] = 2;
        break;
    case BRIDGE_MOVE_TO_OFFSET_POSITION:
        if (*(s32 *)(data + 0xb0) != -1) {
            other = level_mobys + *(s32 *)(data + 0xb0) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] == *(u32 *)(data + 0xb8)) {
                if (*(s32 *)(data + 0xd0) != -1)
                    allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
                moby[0x20] = 4;
                break;
            }
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = *(f32 *)(data + 0xbc);
        vectors[5] = *(f32 *)(data + 0xc0);
        vectors[6] = *(f32 *)(data + 0xc4);
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        transform_vector_by_basis(vectors, vectors, moby + 0xc0);
        scale_vector_xyz_alt_31aee0(*(f32 *)(data + 0xc8) * frame_time, vectors + 4, vectors);
        fast_vec_add(moby + 0x10, moby + 0x10, vectors + 4);
        *(f32 *)(data + 0xac) -= *(f32 *)(data + 0xc8) * frame_time;
        if (*(f32 *)(data + 0xac) < 0.0f) {
            fast_vec_add(moby + 0x10, data + 0xa0, vectors);
            *(f32 *)(data + 0xac) = 0.0f;
            if (*(s32 *)(data + 0xd8) != -1)
                D_L07_00184528[*(s32 *)(data + 0xd8)] = 1;
            moby[0x20] = 3;
            if (*(s32 *)(data + 0xd4) != -1)
                allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
        }
        break;
    case BRIDGE_WAIT_TO_RETURN_TO_INITIAL_POSITION:
        if (*(s32 *)(data + 0xb0) != -1) {
            other = level_mobys + *(s32 *)(data + 0xb0) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] == *(u32 *)(data + 0xb8)) {
                *(s32 *)(data + 0xac) = 0;
                if (*(s32 *)(data + 0xd0) != -1)
                    allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
                moby[0x20] = 4;
            }
        }
        break;
    case BRIDGE_RETURN_TO_INITIAL_POSITION:
        if (*(s32 *)(data + 0xb0) == -1 ||
            (other = level_mobys + *(s32 *)(data + 0xb0) * 0x100) == 0 || other[0x20] == 0xfe ||
            other[0x20] == 0xfd || other[0x20] == *(u32 *)(data + 0xb4)) {
            if (*(s32 *)(data + 0xd0) != -1)
                allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
            moby[0x20] = 2;
        } else {
            *(quadword *)(vectors + 4) = 0;
            vectors[4] = *(f32 *)(data + 0xbc);
            vectors[5] = *(f32 *)(data + 0xc0);
            vectors[6] = *(f32 *)(data + 0xc4);
            *(quadword *)vectors = *(quadword *)(vectors + 4);
            distance = fast_vec_length(vectors);
            scale_vector_xyz_alt_31aee0(-(*(f32 *)(data + 0xcc) * frame_time), vectors, vectors);
            transform_vector_by_basis(vectors + 4, vectors, moby + 0xc0);
            fast_vec_add(moby + 0x10, moby + 0x10, vectors + 4);
            *(f32 *)(data + 0xac) += *(f32 *)(data + 0xcc) * frame_time;
            if (distance < *(f32 *)(data + 0xac)) {
                qcopy(moby + 0x10, data + 0xa0);
                if (*(s32 *)(data + 0xd0) != -1)
                    allocate_voice_for_target_entry(*(s32 *)(data + 0xd4), 0, moby);
                if (*(s32 *)(data + 0xd8) != -1)
                    D_L07_00184528[*(s32 *)(data + 0xd8)] = 0;
                moby[0x20] = 1;
            }
        }
        break;
    case 5:
        break;
    }
    fast_vec_add(delta, delta, moby + 0x10);
    update_entity_matrix(data + 0x60, delta, old_rotation, moby + 0x40);
}
