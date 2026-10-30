#include "rnc/mobys/moby_class_tables.h"
#include "rnc/rendering/level_render_state.h"
#include "types.h"
#include "asm.h"
#include "rnc/globals.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/audio/banks/snd_resolve_bank_xrefs.h"
#include "rnc/audio/streaming/submit_audio_stream_io_request.h"
#include "rnc/rendering/view/init_view_context.h"
#include "rnc/rendering/vu1/vu1_init_chain.h"
#include "rnc/runtime/memory/init_mem_slots.h"
#include "rnc/world/loaders/parse_space_scene_chunk.h"
#include "rnc/audio/banks/snd_bank_load_from_ee_cb.h"
#include "rnc/rendering/frame/reset_draw_globals.h"

void initialize_level_runtime(void) __asm__("FUN_00230f60");

#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/streaming/disc_table.h"

typedef struct {
    s32 tex;         /* 0x00 */
    s32 data_offset; /* 0x04 */
    s32 texCount;    /* 0x08 */
    s32 texInfo;     /* 0x0C */
    s32 mobyCount;   /* 0x10 */
    s32 mobys;       /* 0x14 */
    s32 classCount;  /* 0x18 */
    s32 classes;     /* 0x1C */
    s32 partCount;   /* 0x20 */
    s32 parts;       /* 0x24 */
    s32 pointCount;  /* 0x28 */
    s32 points;      /* 0x2C */
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 sky; /* 0x48 */
    s32 unk4C;
    s32 scenes[5]; /* 0x50 */
    s32 sound;     /* 0x64 */
} LevelHeader;

typedef struct {
    s32 offset;
    u16 id;
    u16 pad;
    s32 pad8[2];
} ClassEntry;

typedef struct {
    s32 offset;
    s32 oclass;
    s32 pad8[2];
    u8  body[0x10];
} MobyEntry;

typedef struct {
    s32 offset;
    s32 size;
} Chunk;

typedef struct {
    u8  pad0[4];
    s32 unk4;
    s32 unk8;
    u8  padC[8];
    u8 *buffer;
    s32 unk18;
} MemInfo;

typedef struct {
    u8  pad0[0x58];
    s32 unk58;
    s32 unk5C;
    s32 chunks[70];
} SceneInfo;

extern f32 sequence __asm__("D_0015F43C");
extern s32 D_001413D0[];
extern s32 current_vu_chain __asm__("D_00160F0C");
extern s32 space_load_in_progress __asm__("D_0015F618");
extern s32 D_0015F440;
extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");
extern u8  D_00194180[];

#include "rnc/rendering/resident_class.h"

extern s32     D_001B6180[];
extern MemInfo D_001940C0;

#include "rnc/rendering/material_templates.h"

extern s32  D_0015FF08;
extern s32  D_0015FF00;
extern s32  level_texture_payload_address __asm__("D_0015F460");
extern u64  D_00160580;
extern s64  D_00160588;
extern u8   point_lights[] __asm__("D_0019C1C0");
extern u8   point_light_data[] __asm__("D_0019C3C0");
extern char dir_lights[] __asm__("D_0019BDC0");
extern u8   default_directional_light_template[] __asm__("D_001D9740");

#include "rnc/game/game_globals.h"

extern u8 *moby_instance_end __asm__("D_0015FF1C");
extern u8 *moby_vars __asm__("D_0015FF28");
extern u8 *D_001600AC;
extern s32 D_001600B4;
extern u8 *moby_instance_perm_end __asm__("D_0015FF20");
extern s32 D_001600B0;
extern s32 D_001600B8;
extern u8  D_001CD780[];
extern s32 occl_fog_count __asm__("D_0015FF30");
extern s32 part_clip_dist __asm__("D_001600BC");
extern s64 D_001604E0 __attribute__((sda));
extern s64 D_001604F0 __attribute__((sda));

#include "rnc/game/level_state.h"

extern SceneInfo render_sequence __asm__("D_0018CB20");
extern u8        D_00186310[];
extern u8        D_00186350[];
extern u32       sound_bank_handle __asm__("D_0015ED5C");

extern void fill_transfer_words(void *dst, s32 value, s32 size) __asm__("func_001F97E8");
extern void update_view_context(void) __asm__("func_001F2D98");
extern s32  scale_game_frames(s32) __asm__("func_001F96F8");
extern s32  sceCdSync(s32);
extern void FlushCache(s32);
extern s32  decompress_wad(void *, void *) __asm__("func_0020B618");
extern void upload_texture_images(s32 arg0, s32 count, s32 p) __asm__("FUN_00203120");
extern s32  log2dim(s32) __asm__("func_001F97A0");
extern void fast_mem_copy(void *, void *, s32) __asm__("func_001F98D0");
extern void relocate_sky_definition(s32 f) __asm__("FUN_002028e0");
extern void register_moby_class(s32 moby, s32 arg1, s32 arg2, s32 oclass) __asm__("FUN_00203640");
extern void unpack_point_records(s32 src, s32 count) __asm__("FUN_00202800");
extern void parse_particle_textures(s32 hdr, s32 base, s32 src, s32 count) __asm__("FUN_002026c8");
extern s32  upload_mip_texture(s32, void *) __asm__("func_00202270");
extern void clear_blocks_16(void *, s32) __asm__("func_001F9810");
extern s32  rand(void);
extern void store_sound_bank_handle_callback() __asm__("func_0022DD78");
extern s32  sceGsSyncV(s32);
extern s32  snd_flush_sound_commands(void) __asm__("func_0012DC80");

void initialize_level_runtime(void) {
    s64          texture_packet[4];
    LevelHeader *header;
    s32          data_base;
    s32          class_data_base;
    ClassEntry  *class_entries;
    ClassEntry  *class_entry;
    s32         *output;
    MobyEntry   *moby_entry;
    s32         *texture_offsets;
    s32          language_index;
    s32          archive_size;
    u8          *allocation_cursor;
    s32         *scene_offsets;
    s32         *scene_offset;
    Chunk       *scene_chunk;
    s32          i;
    s32          j;
    s32          k;
    s32          texture_cursor;
    s32          scene_variant;
    s32          workspace_offset;

    level_render_state.state = 4;
    level_render_state.timer = -1;
    sequence = 1.0f;
    D_001413D0[0] = 0;
    current_vu_chain = 0x100000;
    space_load_in_progress = 0;
    D_0015F440 = 0;
    init_mem_slots();
    gs_texture_allocation_cursor = gs_texture_allocation_base;
    gs_texture_allocation_start = gs_texture_allocation_base;
    fill_transfer_words(D_00194180, 0x87654321, 0x10);
    fill_transfer_words(resident_class_slot_by_id, -1, 0x800);
    fill_transfer_words(resident_class_material_maps, -1, 0xE00);
    fill_transfer_words(D_001B6180, 0, 0xE0);
    init_view_context();
    update_view_context();
    vu1_init_chain();
    submit_audio_stream_io_request(
        D_001940C0.buffer + 0x400000, disc_table.unk13B8.sector, disc_table.unk13B8.size);
    fade_to_black(scale_game_frames(0xC));
    sceCdSync(0);
    FlushCache(0);
    archive_size = decompress_wad(D_001940C0.buffer + 0x400000, D_001940C0.buffer);
    FlushCache(0);
    header = (LevelHeader *)D_001940C0.buffer;
    upload_texture_images(
        (s32)header + header->tex, header->texCount, (s32)header + header->texInfo);
    data_base = (s32)header + header->data_offset;
    class_data_base = data_base + header->unk30;
    special_material_template[0] =
        (s32)((gs_texture_allocation_base + header->unk40) >> 8) | 0x1D308000 |
        ((s64)0xB800 << 19) | ((s64)((gs_texture_allocation_base + header->unk44) >> 8) << 37) |
        0x8000000000000000LL;
    special_material_template[1] = 0x0000FFA0000000E0LL;
    special_material_template[2] = 0x0040000400004000LL;
    class_entries = (ClassEntry *)((s32)header + header->classes);
    D_0015FF08 = header->classCount;
    for (i = 0; i < D_0015FF08; i++) {
        moby_texture_lookup_table[i] =
            class_data_base + class_entries[i].offset + (log2dim(class_entries[i].id) << 28);
    }
    fast_mem_copy(resident_indexed_textures, class_entries, D_0015FF08 * 16);
    relocate_sky_definition(data_base + header->sky);
    moby_entry = (MobyEntry *)((s32)header + header->mobys);
    D_0015FF00 = 0;
    for (j = 0; j < header->mobyCount; j++) {
        register_moby_class(moby_entry->offset == 0 ? 0 : data_base + moby_entry->offset,
                            (s32)header + header->classes,
                            (s32)moby_entry->body,
                            moby_entry->oclass);
        moby_entry++;
    }
    level_texture_payload_address = data_base + header->unk38;
    unpack_point_records((s32)header + header->points, header->pointCount);
    {
        s32 particle_data_base = data_base + header->unk34;
        s32 particle_entries = (s32)header + header->parts;

        parse_particle_textures(
            (s32)header + header->unk3C, particle_data_base, particle_entries, header->partCount);
    }
    texture_offsets = (s32 *)(data_base + header->unk4C);
    language_index = game_language - 1;
    if (language_index < 0) {
        language_index = 0;
    }
    upload_mip_texture((s32)texture_offsets + texture_offsets[current_level_index + 1],
                       texture_packet);
    D_00160580 = texture_packet[0];
    upload_mip_texture((s32)texture_offsets +
                           texture_offsets[language_index * 19 + current_level_index + 0x14],
                       texture_packet);
    texture_cursor = gs_texture_allocation_cursor + 0x2000;
    allocation_cursor = D_001940C0.buffer + archive_size;
    gs_texture_allocation_cursor = texture_cursor;
    D_001940C0.unk18 = (s32)allocation_cursor;
    gs_texture_allocation_start = texture_cursor;
    D_00160588 = texture_packet[0];
    clear_blocks_16(point_lights, 0x100);
    clear_blocks_16(point_light_data, 0x180);
    clear_blocks_16(dir_lights, 0x400);
    fast_mem_copy(dir_lights, default_directional_light_template, 0x40);
    resident_object_pool = allocation_cursor;
    fill_transfer_words(allocation_cursor, 0, 0x4000);
    allocation_cursor += 0x4000;
    moby_instance_end = resident_object_pool;
    resident_object_pool[0x20] = 0xFF;
    moby_vars = allocation_cursor;
    allocation_cursor += 0x2000;
    D_001600AC = allocation_cursor;
    D_001600B4 = -1;
    moby_instance_perm_end = resident_object_pool + 0x3F00;
    D_001600B0 = 0;
    D_001600B8 = 0;
    clear_blocks_16(D_001CD780, 0x200);
    reset_draw_globals();
    occl_fog_count = 0x1F4;
    part_clip_dist = 0x1F4000;
    scene_variant = (rand() >> 16) & 3;
    level_render_state.scene_state = 0;
    level_render_state.scene_mode = scene_variant;
    level_render_state.history_index = 0;
    level_render_state.history_count = 0;
    qcopy(&D_001604F0, &D_001604E0);
    if (current_level_index == 0 || (current_level_index == 1 && level_available[3] == 0)) {
        level_render_state.scene_mode = 4;
        level_render_state.scene_state = 2;
    }
    fill_transfer_words(&render_sequence, 0, 0x1C0);
    fill_transfer_words(D_00186310, 0, 0x40);
    fill_transfer_words(D_00186350, 0, 0x40);
    scene_offsets = header->scenes;
    workspace_offset = current_vu_chain - 0x60000;
    scene_offset = &scene_offsets[level_render_state.scene_mode];
    render_sequence.unk58 = D_001940C0.unk4 + workspace_offset;
    render_sequence.unk5C = D_001940C0.unk8 + workspace_offset;
    current_vu_chain = workspace_offset;
    scene_chunk = (Chunk *)(data_base + scene_offsets[level_render_state.scene_mode]);
    /* The archive list is terminated by a zero size and the resident table holds 70 entries. */
    for (k = 0; k < 70 && scene_chunk->size != 0; k++) {
        {
            s32 chunk_offset = scene_chunk->offset + 0x800;
            render_sequence.chunks[k] = data_base + *scene_offset + chunk_offset;
        }
        scene_chunk++;
    }
    parse_space_scene_chunk(0);
    sound_bank_handle = 0xFFFFFFFF;
    snd_bank_load_from_ee_cb(
        data_base + header->sound, store_sound_bank_handle_callback, (u32)&sound_bank_handle);
    do {
        FlushCache(0);
        sceGsSyncV(0);
        snd_flush_sound_commands();
    } while (sound_bank_handle == 0xFFFFFFFF);
    snd_resolve_bank_xrefs();
}

extern __typeof__(initialize_level_runtime) func_00230F60 __attribute__((alias("FUN_00230f60")));

char resident_indexed_textures[4096] = {0};
