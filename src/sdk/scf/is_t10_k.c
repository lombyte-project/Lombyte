#include "rnc/sdk/library/sdk_state.h"
#include "rnc/sdk/scf/GetRomName.h"

int IsT10K(void) __asm__("IsT10K");

int IsT10K(void) {
    register struct RomNameState *rom_name_state = &RomNameStateData;
    if (!rom_name_state->loaded) {
        GetRomName();
    }
    return rom_name_state->model_code == 'T';
}
