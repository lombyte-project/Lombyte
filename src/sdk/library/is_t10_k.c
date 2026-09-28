typedef signed char s8;

/* The target keeps this ROM-name record out of the EE small-data area. */
struct RomNameState {
    s8 loaded;
    s8 padding[3];
    s8 model_code;
    s8 reserved[16];
};

extern struct RomNameState RomNameStateData __asm__("D_001330D8");
extern void GetRomName(void) __asm__("GetRomName");

int IsT10K(void) __asm__("IsT10K");

int IsT10K(void) {
    register struct RomNameState *rom_name_state = &RomNameStateData;
    if (!rom_name_state->loaded) {
        GetRomName();
    }
    return rom_name_state->model_code == 'T';
}
