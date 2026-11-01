/* Shared moby class: door (and hangar_door). */
#include "types.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/globals.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

typedef struct DoorVars {
    u8  pad0[0xc];
    f32 unkC;
    s32 linked_moby_index;
    u32 unk14;
    u32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s32 sound_entry;
    s32 unk34;
    s32 unk38;
} DoorVars;

extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32  fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");

void door_update(u8 *moby) __asm__("FUN_L07_00310df0");

/* Moby classes 129, 130, 182, 183, 360, 1063, 1078, 1079, 1131 */
void door_update(u8 *moby) {
    enum {
        DOOR_INITIALIZE = 0,
        DOOR_WAIT_TO_MOVE_TO_OFFSET_POSITION = 1,
        DOOR_MOVE_TO_OFFSET_POSITION = 2,
        DOOR_WAIT_TO_RETURN_TO_INITIAL_POSITION = 3,
        DOOR_RETURN_TO_INITIAL_POSITION = 4
    };
    DoorVars   *door_data = (DoorVars *)MOBY(moby)->pvars;
    u8         *linked_moby;
    f32         vectors[8];
    f32         distance;
    u8          state;
    typedef u32 quadword __attribute__((mode(TI), aligned(16)));

    if (MOBY(moby)->mission != 0xff &&
        visited52[MOBY(moby)->mission + current_level_index * 16] == 0xff)
        door_data->linked_moby_index = -1;

    state = MOBY(moby)->state;
    switch (state) {
    case DOOR_INITIALIZE:
        qcopy(door_data, &MOBY(moby)->pos);
        if (door_data->unk38 != -1)
            D_L07_00184528[door_data->unk38] = 0;
        MOBY(moby)->state = DOOR_WAIT_TO_MOVE_TO_OFFSET_POSITION;
        *(s32 *)&door_data->unkC = 0;
        return;
    case DOOR_WAIT_TO_MOVE_TO_OFFSET_POSITION:
        if (door_data->unk38 != -1)
            D_L07_00184528[door_data->unk38] = 0;
        if (door_data->linked_moby_index != -1) {
            linked_moby = level_mobys + door_data->linked_moby_index * 0x100;
            if (linked_moby != 0 && MOBY(linked_moby)->state != 0xfe &&
                MOBY(linked_moby)->state != 0xfd && MOBY(linked_moby)->state != door_data->unk14)
                return;
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = door_data->unk1C;
        vectors[5] = door_data->unk20;
        vectors[6] = door_data->unk24;
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        door_data->unkC = fast_vec_length(vectors);
        if (door_data->sound_entry != -1)
            allocate_voice_for_target_entry(door_data->sound_entry, 0, moby);
        MOBY(moby)->state = DOOR_MOVE_TO_OFFSET_POSITION;
        return;
    case DOOR_MOVE_TO_OFFSET_POSITION:
        if (door_data->unk38 != -1)
            D_L07_00184528[door_data->unk38] = 1;
        if (door_data->linked_moby_index != -1) {
            linked_moby = level_mobys + door_data->linked_moby_index * 0x100;
            if (linked_moby != 0 && MOBY(linked_moby)->state != 0xfe &&
                MOBY(linked_moby)->state != 0xfd && MOBY(linked_moby)->state == door_data->unk18) {
                if (door_data->sound_entry != -1)
                    allocate_voice_for_target_entry(door_data->sound_entry, 0, moby);
                MOBY(moby)->state = DOOR_RETURN_TO_INITIAL_POSITION;
                return;
            }
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = door_data->unk1C;
        vectors[5] = door_data->unk20;
        vectors[6] = door_data->unk24;
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        transform_vector_by_basis(vectors, vectors, moby + 0xc0);
        fast_vec_normalize(vectors + 4, vectors, door_data->unk28 * frame_time);
        fast_vec_add(&MOBY(moby)->pos, &MOBY(moby)->pos, vectors + 4);
        door_data->unkC -= door_data->unk28 * frame_time;
        if (!(door_data->unkC < 0.0f))
            return;
        fast_vec_add(&MOBY(moby)->pos, door_data, vectors);
        door_data->unkC = 0.0f;
        MOBY(moby)->state = DOOR_WAIT_TO_RETURN_TO_INITIAL_POSITION;
        if (door_data->unk34 != -1)
            allocate_voice_for_target_entry(door_data->sound_entry, 0, moby);
        return;
    case DOOR_WAIT_TO_RETURN_TO_INITIAL_POSITION:
        if (door_data->unk38 != -1)
            D_L07_00184528[door_data->unk38] = 1;
        if (door_data->linked_moby_index == -1)
            return;
        linked_moby = level_mobys + door_data->linked_moby_index * 0x100;
        if (linked_moby == 0 || MOBY(linked_moby)->state == 0xfe ||
            MOBY(linked_moby)->state == 0xfd || MOBY(linked_moby)->state != door_data->unk18)
            return;
        *(s32 *)&door_data->unkC = 0;
        if (door_data->sound_entry != -1)
            allocate_voice_for_target_entry(door_data->sound_entry, 0, moby);
        MOBY(moby)->state = DOOR_RETURN_TO_INITIAL_POSITION;
        return;
    case DOOR_RETURN_TO_INITIAL_POSITION:
        if (door_data->unk38 != -1)
            D_L07_00184528[door_data->unk38] = 0;
        if (door_data->linked_moby_index == -1 ||
            (linked_moby = level_mobys + door_data->linked_moby_index * 0x100) == 0 ||
            MOBY(linked_moby)->state == 0xfe || MOBY(linked_moby)->state == 0xfd ||
            MOBY(linked_moby)->state == door_data->unk14) {
            if (door_data->sound_entry != -1)
                allocate_voice_for_target_entry(door_data->sound_entry, 0, moby);
            MOBY(moby)->state = DOOR_MOVE_TO_OFFSET_POSITION;
        } else {
            *(quadword *)(vectors + 4) = 0;
            vectors[4] = door_data->unk1C;
            vectors[5] = door_data->unk20;
            vectors[6] = door_data->unk24;
            *(quadword *)vectors = *(quadword *)(vectors + 4);
            distance = fast_vec_length(vectors);
            fast_vec_normalize(vectors, vectors, -(door_data->unk2C * frame_time));
            transform_vector_by_basis(vectors + 4, vectors, moby + 0xc0);
            fast_vec_add(&MOBY(moby)->pos, &MOBY(moby)->pos, vectors + 4);
            door_data->unkC += door_data->unk2C * frame_time;
            if (!(distance < door_data->unkC))
                return;
            qcopy(&MOBY(moby)->pos, door_data);
            if (door_data->sound_entry != -1)
                allocate_voice_for_target_entry(door_data->unk34, 0, moby);
            if (door_data->unk38 != -1)
                D_L07_00184528[door_data->unk38] = 0;
            MOBY(moby)->state = DOOR_WAIT_TO_MOVE_TO_OFFSET_POSITION;
        }
        break;
    case 5:
    default:
        return;
    }
}
