/* Shared moby class: bridge. */
#include "types.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/globals.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/mobys/moby.h"

typedef struct BridgeVars {
    u8  pad0[0x60];
    u8  entity_matrix[0x40];
    f32 initial_position[4]; /* w: distance left to move */
    s32 trigger_moby_index;
    u32 trigger_extend_state;
    u32 trigger_return_state;
    f32 offset_x;
    f32 offset_y;
    f32 offset_z;
    f32 extend_speed;
    f32 return_speed;
    s32 move_sound;
    s32 stop_sound;
    s32 extended_flag_index;
} BridgeVars;

extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32  fast_vec_length(void *) __asm__("FUN_001f9af0");

extern void fast_vec_scale(f32, void *, void *) __asm__("FUN_001f9a68");
extern void scale_vector_xyz_alt_31aee0(f32, void *, void *) __asm__("FUN_001f9bf8");

void bridge_update(u8 *moby) __asm__("FUN_L07_0031aee0");

/* Moby classes 104, 106, 1129 */
void bridge_update(u8 *moby) {
    enum {
        BRIDGE_INITIALIZE = 0,
        BRIDGE_WAIT_TO_MOVE_TO_OFFSET_POSITION = 1,
        BRIDGE_MOVE_TO_OFFSET_POSITION = 2,
        BRIDGE_WAIT_TO_RETURN_TO_INITIAL_POSITION = 3,
        BRIDGE_RETURN_TO_INITIAL_POSITION = 4
    };
    BridgeVars *bridge_data = (BridgeVars *)MOBY(moby)->pvars;
    u8         *linked_state_moby;
    f32         delta[4];
    f32         old_rotation[4];
    f32         vectors[8];
    f32         distance;
    u8          state;
    typedef u32 quadword __attribute__((mode(TI), aligned(16)));

    fast_vec_scale(-1.0f, delta, &MOBY(moby)->pos);
    qcopy(old_rotation, moby + 0x40);

    if (MOBY(moby)->mission != 0xff &&
        visited52[MOBY(moby)->mission + current_level_index * 16] == 0xff)
        bridge_data->trigger_moby_index = -1;

    state = MOBY(moby)->state;
    switch (state) {
    case BRIDGE_INITIALIZE:
        qcopy(&bridge_data->initial_position, &MOBY(moby)->pos);
        if (bridge_data->extended_flag_index != -1)
            D_L07_00184528[bridge_data->extended_flag_index] = 0;
        MOBY(moby)->state = BRIDGE_WAIT_TO_MOVE_TO_OFFSET_POSITION;
        *(s32 *)&bridge_data->initial_position[3] = 0;
        break;
    case BRIDGE_WAIT_TO_MOVE_TO_OFFSET_POSITION:
        if (bridge_data->trigger_moby_index != -1) {
            linked_state_moby = level_mobys + bridge_data->trigger_moby_index * 0x100;
            if (linked_state_moby != 0 && MOBY(linked_state_moby)->state != 0xfe &&
                MOBY(linked_state_moby)->state != 0xfd &&
                MOBY(linked_state_moby)->state != bridge_data->trigger_extend_state)
                break;
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = bridge_data->offset_x;
        vectors[5] = bridge_data->offset_y;
        vectors[6] = bridge_data->offset_z;
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        bridge_data->initial_position[3] = fast_vec_length(vectors);
        if (bridge_data->move_sound != -1)
            allocate_voice_for_target_entry(bridge_data->move_sound, 0, moby);
        MOBY(moby)->state = BRIDGE_MOVE_TO_OFFSET_POSITION;
        break;
    case BRIDGE_MOVE_TO_OFFSET_POSITION:
        if (bridge_data->trigger_moby_index != -1) {
            linked_state_moby = level_mobys + bridge_data->trigger_moby_index * 0x100;
            if (linked_state_moby != 0 && MOBY(linked_state_moby)->state != 0xfe &&
                MOBY(linked_state_moby)->state != 0xfd &&
                MOBY(linked_state_moby)->state == bridge_data->trigger_return_state) {
                if (bridge_data->move_sound != -1)
                    allocate_voice_for_target_entry(bridge_data->move_sound, 0, moby);
                MOBY(moby)->state = BRIDGE_RETURN_TO_INITIAL_POSITION;
                break;
            }
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = bridge_data->offset_x;
        vectors[5] = bridge_data->offset_y;
        vectors[6] = bridge_data->offset_z;
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        transform_vector_by_basis(vectors, vectors, moby + 0xc0);
        scale_vector_xyz_alt_31aee0(bridge_data->extend_speed * frame_time, vectors + 4, vectors);
        fast_vec_add(&MOBY(moby)->pos, &MOBY(moby)->pos, vectors + 4);
        bridge_data->initial_position[3] -= bridge_data->extend_speed * frame_time;
        if (bridge_data->initial_position[3] < 0.0f) {
            fast_vec_add(&MOBY(moby)->pos, &bridge_data->initial_position, vectors);
            bridge_data->initial_position[3] = 0.0f;
            if (bridge_data->extended_flag_index != -1)
                D_L07_00184528[bridge_data->extended_flag_index] = 1;
            MOBY(moby)->state = BRIDGE_WAIT_TO_RETURN_TO_INITIAL_POSITION;
            if (bridge_data->stop_sound != -1)
                allocate_voice_for_target_entry(bridge_data->move_sound, 0, moby);
        }
        break;
    case BRIDGE_WAIT_TO_RETURN_TO_INITIAL_POSITION:
        if (bridge_data->trigger_moby_index != -1) {
            linked_state_moby = level_mobys + bridge_data->trigger_moby_index * 0x100;
            if (linked_state_moby != 0 && MOBY(linked_state_moby)->state != 0xfe &&
                MOBY(linked_state_moby)->state != 0xfd &&
                MOBY(linked_state_moby)->state == bridge_data->trigger_return_state) {
                *(s32 *)&bridge_data->initial_position[3] = 0;
                if (bridge_data->move_sound != -1)
                    allocate_voice_for_target_entry(bridge_data->move_sound, 0, moby);
                MOBY(moby)->state = BRIDGE_RETURN_TO_INITIAL_POSITION;
            }
        }
        break;
    case BRIDGE_RETURN_TO_INITIAL_POSITION:
        if (bridge_data->trigger_moby_index == -1 ||
            (linked_state_moby = level_mobys + bridge_data->trigger_moby_index * 0x100) == 0 ||
            MOBY(linked_state_moby)->state == 0xfe || MOBY(linked_state_moby)->state == 0xfd ||
            MOBY(linked_state_moby)->state == bridge_data->trigger_extend_state) {
            if (bridge_data->move_sound != -1)
                allocate_voice_for_target_entry(bridge_data->move_sound, 0, moby);
            MOBY(moby)->state = BRIDGE_MOVE_TO_OFFSET_POSITION;
        } else {
            *(quadword *)(vectors + 4) = 0;
            vectors[4] = bridge_data->offset_x;
            vectors[5] = bridge_data->offset_y;
            vectors[6] = bridge_data->offset_z;
            *(quadword *)vectors = *(quadword *)(vectors + 4);
            distance = fast_vec_length(vectors);
            scale_vector_xyz_alt_31aee0(
                -(bridge_data->return_speed * frame_time), vectors, vectors);
            transform_vector_by_basis(vectors + 4, vectors, moby + 0xc0);
            fast_vec_add(&MOBY(moby)->pos, &MOBY(moby)->pos, vectors + 4);
            bridge_data->initial_position[3] += bridge_data->return_speed * frame_time;
            if (distance < bridge_data->initial_position[3]) {
                qcopy(&MOBY(moby)->pos, &bridge_data->initial_position);
                if (bridge_data->move_sound != -1)
                    allocate_voice_for_target_entry(bridge_data->stop_sound, 0, moby);
                if (bridge_data->extended_flag_index != -1)
                    D_L07_00184528[bridge_data->extended_flag_index] = 0;
                MOBY(moby)->state = BRIDGE_WAIT_TO_MOVE_TO_OFFSET_POSITION;
            }
        }
        break;
    case 5:
        break;
    }
    fast_vec_add(delta, delta, &MOBY(moby)->pos);
    update_entity_matrix(&bridge_data->entity_matrix, delta, old_rotation, moby + 0x40);
}
