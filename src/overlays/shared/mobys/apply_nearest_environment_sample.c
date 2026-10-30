/* Mobys: applies the nearest environment sample, and clears a persistent slot. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"

/* Finds the table entry nearest the object's position (within 8 units) and applies its fields to the object and globals. */
/* Ported from rac1-decomp (src/overlays/shared/mobyfunc_0024FD50.c: func_L00_002510F0), where it is exact; names translated to the US level program. */

extern char no_environment_sample_message[] __asm__("D_L00_001E8ED8");
extern void debug_print() __asm__("FUN_001e93b0");
extern void pack_render_command_fields(struct RenderEntry *, s32, s32, s32, s32) __asm__("PackRenderCommandFields");

void apply_nearest_environment_sample(char *object) __asm__("FUN_L00_002500b8");

void apply_nearest_environment_sample(char *environment_sample_target) {
    float          position_delta[4];
    float          best = 999999.0f;
    float          entry_distance;
    int            nearest_entry_index = -1;
    int            i;
    int            selected_entry_byte_offset;
    char          *packing_source_entry;
    char          *global_update_source_entry;
    char          *conditional_update_source_entry;
    char          *global_update_destination;
    unsigned char *entry_update_destination;

    for (i = 0; i < D_L00_00160014; i++) {
        fast_vec_sub(position_delta, environment_sample_target + 0x10, D_L00_00160040 + i * 0x30);
        entry_distance = fast_vec_length(position_delta);
        if (entry_distance <= 8.0f && entry_distance < best) {
            best = entry_distance;
            nearest_entry_index = i;
        }
    }
    if (nearest_entry_index == -1) {
        debug_print(no_environment_sample_message);
        return;
    }
    selected_entry_byte_offset = nearest_entry_index * 0x30;
    packing_source_entry = (char *)(selected_entry_byte_offset + (int)D_L00_00160040);
    pack_render_command_fields(environment_sample_target,
                               *(int *)(environment_sample_target + 0x80) =
                                   (*(int *)(packing_source_entry + 0x18) << 16) +
                                   (*(int *)(packing_source_entry + 0x14) << 8) +
                                   *(int *)(packing_source_entry + 0x10),
                               0,
                               0,
                               0);
    *(int *)(environment_sample_target + 0x38) =
        *(int *)((char *)(selected_entry_byte_offset + (int)D_L00_00160040) + 0x1C);
    global_update_source_entry = (char *)(selected_entry_byte_offset + (int)D_L00_00160040);
    if (*(unsigned char *)(global_update_source_entry + 0x27) != 0) {
        global_update_destination = D_0013E533 + 0x1D;
        *(int *)(global_update_destination + 0x64) = *(int *)(global_update_source_entry + 0x20);
        global_update_destination[0x68] = global_update_source_entry[0x24];
        global_update_destination[0x69] = global_update_source_entry[0x25];
        global_update_destination[0x6A] = global_update_source_entry[0x26];
        global_update_destination[0x6B] |= 7;
    }
    conditional_update_source_entry = (char *)(selected_entry_byte_offset + (int)D_L00_00160040);
    if (*(int *)(conditional_update_source_entry + 0x28) >= 0) {
        entry_update_destination = D_0014161B + 0x100B5;
        if (*(short *)(entry_update_destination + 0x3E) == 0) {
            if ((*(int *)(entry_update_destination + 0x20) & 0xFFFFFF) == 0xFF0000) {
                *(unsigned short *)(entry_update_destination + 0x38) =
                    *(int *)(conditional_update_source_entry + 0x28);
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/mobyfunc_0024FD50.c: func_L00_002512D8), where it is exact; names translated to the US level program. */

/* Clears slot idx of this level's 16-entry table to 0xFF. */
void jet_clear_persistent(int idx) __asm__("FUN_L00_002502a0");

void jet_clear_persistent(int persistent_slot_index) {
    if (persistent_slot_index != 0xFF) {
        visited52[persistent_slot_index + current_level_index * 16] = 0xFF;
    }
}
