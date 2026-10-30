/* Vendor: initializes the vendor screen. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/audio/music/music_stream_state.h"
#include "asm.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/audio/streaming/continue_audio_stream_if_ready.h"
#include "rnc/audio/voices/snd_pause_all_sounds_in_group.h"
#include "rnc/overlay/l01/hero/hero_set_state.h"
#include "rnc/rendering/vu1/select_level_vu_chain.h"
#include "rnc/world/select_world_object_resource_tables.h"
#include "rnc/world/loaders/parse_space_scene_chunk.h"
#include "rnc/overlay/shared/ui/hud/draw_weapon_ammo_hud_panel.h"

/* Opens the vendor screen for moby m in level lvl: sets up its camera, resets the screen state and runs it until it closes. */
/* Ported from rac1-decomp (src/overlays/shared/update_002AFD00.c: func_L01_002AFD00), where it is exact; names translated to the US level program. */

typedef struct {
    char p0[0x10];
    int  f10;
    char p14[0x4C - 0x14];
} R_2afd00;

extern R_2afd00 D_L01_00179F40[];
extern char     fx_cutscene[] __asm__("D_L01_0016CCE0");
extern char     D_L01_0017C7C0[];
extern char     vendor_screen_state_storage[] __asm__("D_L01_001CA940");
extern float    D_L01_0015F3FC;
extern int      cutscene_mode __asm__("D_L01_0015F5C4");

#include "rnc/overlay/shared/vendor/initialize_vendor_screen_globals.h"
#include "rnc/mobys/moby.h"

extern int  vendor_relocation_offsets[] __asm__("D_L01_00174280");
extern int  level_table_pair_indices[] __asm__("D_L01_001CA4A0");
extern int  sceGsSyncV_alt(int) __asm__("FUN_00122298");
extern void sound_update(void) __asm__("FUN_0022ca50");
extern void clear_active_help_record(void) __asm__("FUN_L00_002039a0");
extern void update_item_tint_from_hero(int, int) __asm__("FUN_L00_00207330");
extern void clear_hero_item_slot(int, int) __asm__("FUN_L00_0020fca8");
extern void FUN_L00_00232fe8(void);
extern void load_scene_chunks_with_fade(int) __asm__("FUN_L00_002454c8");
extern void fill_transfer_words() __asm__("FUN_001f97e8");
extern void music_pause(s32);
extern void snd_flush_sound_commands(void) __asm__("FUN_0012dc80");
extern void transform_vector_by_basis(void *, void *, void *);

extern char  vendor_camera_local_position_offset __asm__("D_L01_00161100") __attribute__((sda));
extern int   D_L01_0015F5D8 __attribute__((sda));
extern short D_L01_0015F400 __attribute__((sda));
void         initialize_vendor_screen(char *vendor_moby, int lvl) __asm__("FUN_L01_002aea70");

void initialize_vendor_screen(char *vendor_moby, int lvl) {
    struct Hero             *hero_state = &hero;
    char                    *vendor_screen_state = vendor_screen_state_storage;
    char                    *relocated_address_block = fx_cutscene;
    char                    *sc;
    char                    *relocated_address_source_block;
    struct Hero             *hero_moby_owner;
    struct MusicStreamState *music_state;
    struct MusicStreamState *qq;
    int                      adjusted_address_base;
    int                      level_table_pair_index;
    snd_pause_all_sounds_in_group(0x1D);
    music_pause(0);
    snd_flush_sound_commands();
    D_L01_0015F5D8 = 1;
    fill_transfer_words(vendor_screen_state, 0, 0x220);
    *(int *)vendor_screen_state_storage = 3;
    cutscene_mode = 5;
    *(int *)(vendor_screen_state + 4) = 0;
    *(char **)(vendor_screen_state + 0x28) = vendor_moby;
    sc = vendor_screen_state + 0xC0;
    reset_hud_and_suspend_weapon_updates();
    clear_active_help_record();
    l01_hero_set_state(100, 1);
    *(char *)&hero_state->item_use_locked = 1;
    *(char *)&hero_state->unk20A5 = 1;
    FUN_L00_00232fe8();
    transform_vector_by_basis(sc, &vendor_camera_local_position_offset, vendor_moby + 0xC0);
    fast_vec_add(sc, sc, vendor_moby + 0x10);
    *(float *)(vendor_screen_state + 0xC8) = probe_ground_height(sc, 0, 0.5f);
    clear_u64_value(vendor_screen_state + 0xB0);
    *(float *)(vendor_screen_state + 0xB8) =
        fast_add_rotations(MOBY(vendor_moby)->rot.z, -1.5707964f);
    euler_rotation_basis(vendor_screen_state + 0x80, vendor_screen_state + 0xB0);
    fill_transfer_words(relocated_address_block, 0, 0x1C0);
    fill_transfer_words(D_L01_0017C7C0, 0, 0x40);
    select_level_vu_chain();
    adjusted_address_base = relocated_address_base + (int)0xFFFC0000;
    *(int *)(relocated_address_block + 0x58) = vendor_relocation_offsets[1] + adjusted_address_base;
    level_table_pair_index = level_table_pair_indices[lvl];
    *(int *)(relocated_address_block + 0x5C) = vendor_relocation_offsets[2] + adjusted_address_base;
    D_L01_0015F3FC = 1.0f;
    relocated_address_base = adjusted_address_base;
    *(int *)&D_L01_0015F400 = 0;
    clear_hero_item_slot(0, 0);
    hero_state->pending_item[0] = lvl;
    select_world_object_resource_tables(D_L01_00179F40[lvl].f10, -1);
    load_scene_chunks_with_fade(level_table_pair_index);
    music_state = &music_stream_state;
    music_state->queued_secondary_track = 0x2734;
    parse_space_scene_chunk(0);
    while (qq = &music_stream_state, qq->secondary.state != 3) {
        sound_update();
        sceGsSyncV_alt(0);
    }
    relocated_address_source_block = fx_cutscene;
    hero_moby_owner = &hero;
    update_item_tint_from_hero(*(int *)(relocated_address_source_block + 0x17C),
                               ((int)hero_moby_owner->moby));
    continue_audio_stream_if_ready();
}

R_2afd00 D_L13_00179DC0 = {
    {0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1},
    -1,
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  0,  0,  0,  0,  0,
     -1, -1, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
};
