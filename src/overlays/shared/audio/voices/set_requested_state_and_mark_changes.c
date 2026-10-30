/* Voices: sets a voice's requested state values and marks the ones that changed. */
#include "types.h"
#include "asm.h"
#include "rnc/data_labels.h"

/* Ported from rac1-decomp (src/overlays/shared/sound_002A2C90.c: func_L01_002A2C90), where it is exact; names translated to the US level program. */

void set_requested_state_and_mark_changes(
    int moby_address,
    int requested_state_byte,
    int requested_state_value,
    int requested_pair_first_value,
    int requested_pair_second_value) __asm__("FUN_L01_002a1a90");

void set_requested_state_and_mark_changes(int moby_address,
                                          int requested_state_byte,
                                          int requested_state_value,
                                          int requested_pair_first_value,

                                          int requested_pair_second_value) {
    char *state_base = D_0013E533 + 0x1D;
    *(int *)(state_base + 0x60) = moby_address;
    if ((unsigned char)state_base[0x68] != requested_state_byte) {
        state_base[0x68] = requested_state_byte;
        state_base[0x6B] |= 1;
    }
    if (*(int *)(state_base + 0x64) != requested_state_value) {
        *(int *)(state_base + 0x64) = requested_state_value;
        state_base[0x6B] |= 4;
    }
    if ((unsigned char)state_base[0x69] != requested_pair_first_value ||
        (unsigned char)state_base[0x6A] != requested_pair_second_value) {
        state_base[0x6A] = requested_pair_second_value;
        state_base[0x69] = requested_pair_first_value;
        state_base[0x6B] |= 2;
    }
}
