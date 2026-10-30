#include "rnc/sdk/library/sdk_state.h"

extern void GetRomName(void) __asm__("GetRomName");

int IsT10K(void) __asm__("IsT10K");

int IsT10K(void) {
    register struct RomNameState *rom_name_state = &RomNameStateData;
    if (!rom_name_state->loaded) {
        GetRomName();
    }
    return rom_name_state->model_code == 'T';
}
