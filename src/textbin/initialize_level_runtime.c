#include "types.h"
#include "asm.h"

void initialize_level_runtime(void) __asm__("FUN_00230f60");

#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "sda.h"

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
    u8 body[0x10];
} MobyEntry;

typedef struct {
    s32 offset;
    s32 size;
} Chunk;

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
    s16 unk24;
    u8 pad26[0x2A];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
} GameState;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[8];
    u8 *buffer;
    s32 unk18;
} MemInfo;

typedef struct {
    u8 pad0[0x58];
    s32 unk58;
    s32 unk5C;
    s32 chunks[70];
} SceneInfo;

typedef struct {
    u8 pad0[0x13B8];
    s32 lsn;
    s32 sectors;
} DiscInfo;

extern GameState D_0013E030;
extern f32 D_0015F43C;
extern s32 D_001413D0[];
extern s32 D_00160F0C;
extern s32 D_0015F618;
extern s32 D_0015F440;
extern s32 D_0015EE8C;
extern s32 D_0015EE78;
extern s32 D_0015EE74;
extern u8 D_00194180[];
extern u8 D_001B3AC0[];
extern u8 D_001B6880[];
extern u8 D_001B6180[];
extern MemInfo D_001940C0;
extern DiscInfo D_00137B80;
extern s64 D_0019E6C0[];
extern s32 D_0015FF08;
extern s32 D_001B5980[];
extern u8 D_001CAAC0[];
extern s32 D_0015FF00;
extern s32 D_0015F460;
extern s32 D_0015ED84;
extern s32 D_0015ED88;
extern s64 D_00160580;
extern s64 D_00160588;
extern u8 D_0019C1C0[];
extern u8 D_0019C3C0[];
extern u8 D_0019BDC0[];
extern u8 D_001D9740[];
extern u8 *D_0015FF18;
extern u8 *D_0015FF1C;
extern u8 *D_0015FF28;
extern u8 *D_001600AC;
extern s32 D_001600B4;
extern u8 *D_0015FF20;
extern s32 D_001600B0;
extern s32 D_001600B8;
extern u8 D_001CD780[];
extern s32 D_0015FF30;
extern s32 D_001600BC;
extern s64 D_001604E0 __attribute__((sda));
extern s64 D_001604F0 __attribute__((sda));
extern u8 D_0013DD43[];
extern SceneInfo D_0018CB20;
extern u8 D_00186310[];
extern u8 D_00186350[];
extern u32 D_0015ED5C;

extern void init_mem_slots(void) __asm__("FUN_002015d8");
extern void FillTransferWords(void *dst, s32 value, s32 size);
extern void init_view_context(void) __asm__("FUN_001f2c60");
extern void update_view_context(void) __asm__("func_001F2D98");
extern void vu1_init_chain(void) __asm__("FUN_002335d0");
extern s32 submit_audio_stream_io_request(void *buf, u32 lsn, u32 sectors) __asm__("FUN_00216728");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void fade_to_black(s32 n) __asm__("FUN_001f4a58");
extern s32 sceCdSync(s32);
extern void FlushCache(s32);
extern s32 func_0020B618(void *, void *);
extern void upload_texture_images(s32 arg0, s32 count, s32 p) __asm__("FUN_00203120");
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");
extern void copy_blocks_16_forward(void *, void *, s32) __asm__("func_001F98D0");
extern void relocate_sky_definition(s32 f) __asm__("FUN_002028e0");
extern void register_moby_class(s32 moby, s32 arg1, s32 arg2, s32 oclass) __asm__("FUN_00203640");
extern void unpack_point_records(s32 src, s32 count) __asm__("FUN_00202800");
extern void parse_particle_textures(s32 hdr, s32 base, s32 src, s32 count) __asm__("FUN_002026c8");
extern s32 upload_mip_texture(s32, void *) __asm__("func_00202270");
extern void clear_blocks_16(void *, s32) __asm__("func_001F9810");
extern void reset_draw_globals(void) __asm__("FUN_001f37e8");
extern s32 rand(void);
extern void parse_space_scene_chunk(s32 index) __asm__("FUN_002049f0");
extern void snd_bank_load_from_ee_cb(s32 cmd, void *arg, s64 data) __asm__("FUN_0012e088");
extern void store_async_sound_bank_handle() __asm__("func_0022DD78");
extern s32 sceGsSyncV(s32);
extern s32 snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern void snd_resolve_bank_xrefs(void) __asm__("FUN_0012e1a8");

void initialize_level_runtime(void) __asm__("FUN_00230f60");

void initialize_level_runtime(void) {
    s64 texture_packet[4];
    LevelHeader *header;
    s32 data_base;
    s32 class_data_base;
    ClassEntry *class_entries;
    ClassEntry *class_entry;
    s32 *output;
    MobyEntry *moby_entry;
    s32 *texture_offsets;
    s32 language_index;
    s32 archive_size;
    u8 *allocation_cursor;
    s32 *scene_offsets;
    s32 *scene_offset;
    Chunk *scene_chunk;
    s32 i;
    s32 j;
    s32 k;
    s32 texture_cursor;
    s32 scene_variant;
    s32 workspace_offset;

    D_0013E030.unk20 = 4;
    D_0013E030.unk24 = -1;
    D_0015F43C = 1.0f;
    D_001413D0[0] = 0;
    D_00160F0C = 0x100000;
    D_0015F618 = 0;
    D_0015F440 = 0;
    init_mem_slots();
    D_0015EE74 = D_0015EE8C;
    D_0015EE78 = D_0015EE8C;
    FillTransferWords(D_00194180, 0x87654321, 0x10);
    FillTransferWords(D_001B3AC0, -1, 0x800);
    FillTransferWords(D_001B6880, -1, 0xE00);
    FillTransferWords(D_001B6180, 0, 0xE0);
    init_view_context();
    update_view_context();
    vu1_init_chain();
    submit_audio_stream_io_request(D_001940C0.buffer + 0x400000, D_00137B80.lsn,
                                   D_00137B80.sectors);
    fade_to_black(scale_game_frames(0xC));
    sceCdSync(0);
    FlushCache(0);
    archive_size = func_0020B618(D_001940C0.buffer + 0x400000, D_001940C0.buffer);
    FlushCache(0);
    header = (LevelHeader *)D_001940C0.buffer;
    upload_texture_images((s32)header + header->tex, header->texCount,
                          (s32)header + header->texInfo);
    data_base = (s32)header + header->data_offset;
    class_data_base = data_base + header->unk30;
    D_0019E6C0[0] = (s32)((D_0015EE8C + header->unk40) >> 8) | 0x1D308000 | ((s64)0xB800 << 19) |
                    ((s64)((D_0015EE8C + header->unk44) >> 8) << 37) | 0x8000000000000000LL;
    D_0019E6C0[1] = 0x0000FFA0000000E0LL;
    D_0019E6C0[2] = 0x0040000400004000LL;
    class_entries = (ClassEntry *)((s32)header + header->classes);
    D_0015FF08 = header->classCount;
    for (i = 0; i < D_0015FF08; i++) {
        D_001B5980[i] = class_data_base + class_entries[i].offset +
                        (highest_set_bit_index(class_entries[i].id) << 28);
    }
    copy_blocks_16_forward(D_001CAAC0, class_entries, D_0015FF08 * 16);
    relocate_sky_definition(data_base + header->sky);
    moby_entry = (MobyEntry *)((s32)header + header->mobys);
    D_0015FF00 = 0;
    for (j = 0; j < header->mobyCount; j++) {
        register_moby_class(moby_entry->offset == 0 ? 0 : data_base + moby_entry->offset,
                            (s32)header + header->classes, (s32)moby_entry->body,
                            moby_entry->oclass);
        moby_entry++;
    }
    D_0015F460 = data_base + header->unk38;
    unpack_point_records((s32)header + header->points, header->pointCount);
    {
        s32 particle_data_base = data_base + header->unk34;
        s32 particle_entries = (s32)header + header->parts;

        parse_particle_textures((s32)header + header->unk3C, particle_data_base, particle_entries,
                                header->partCount);
    }
    texture_offsets = (s32 *)(data_base + header->unk4C);
    language_index = D_0015ED88 - 1;
    if (language_index < 0) {
        language_index = 0;
    }
    upload_mip_texture((s32)texture_offsets + texture_offsets[D_0015ED84 + 1], texture_packet);
    D_00160580 = texture_packet[0];
    upload_mip_texture((s32)texture_offsets +
                           texture_offsets[language_index * 19 + D_0015ED84 + 0x14],
                       texture_packet);
    texture_cursor = D_0015EE74 + 0x2000;
    allocation_cursor = D_001940C0.buffer + archive_size;
    D_0015EE74 = texture_cursor;
    D_001940C0.unk18 = (s32)allocation_cursor;
    D_0015EE78 = texture_cursor;
    D_00160588 = texture_packet[0];
    clear_blocks_16(D_0019C1C0, 0x100);
    clear_blocks_16(D_0019C3C0, 0x180);
    clear_blocks_16(D_0019BDC0, 0x400);
    copy_blocks_16_forward(D_0019BDC0, D_001D9740, 0x40);
    D_0015FF18 = allocation_cursor;
    FillTransferWords(allocation_cursor, 0, 0x4000);
    allocation_cursor += 0x4000;
    D_0015FF1C = D_0015FF18;
    D_0015FF18[0x20] = 0xFF;
    D_0015FF28 = allocation_cursor;
    allocation_cursor += 0x2000;
    D_001600AC = allocation_cursor;
    D_001600B4 = -1;
    D_0015FF20 = D_0015FF18 + 0x3F00;
    D_001600B0 = 0;
    D_001600B8 = 0;
    clear_blocks_16(D_001CD780, 0x200);
    reset_draw_globals();
    D_0015FF30 = 0x1F4;
    D_001600BC = 0x1F4000;
    scene_variant = (rand() >> 16) & 3;
    D_0013E030.unk5C = 0;
    D_0013E030.unk58 = scene_variant;
    D_0013E030.unk50 = 0;
    D_0013E030.unk54 = 0;
    qcopy(&D_001604F0, &D_001604E0);
    if (D_0015ED84 == 0 || (D_0015ED84 == 1 && D_0013DD43[0] == 0)) {
        D_0013E030.unk58 = 4;
        D_0013E030.unk5C = 2;
    }
    FillTransferWords(&D_0018CB20, 0, 0x1C0);
    FillTransferWords(D_00186310, 0, 0x40);
    FillTransferWords(D_00186350, 0, 0x40);
    scene_offsets = header->scenes;
    workspace_offset = D_00160F0C - 0x60000;
    scene_offset = &scene_offsets[D_0013E030.unk58];
    D_0018CB20.unk58 = D_001940C0.unk4 + workspace_offset;
    D_0018CB20.unk5C = D_001940C0.unk8 + workspace_offset;
    D_00160F0C = workspace_offset;
    scene_chunk = (Chunk *)(data_base + scene_offsets[D_0013E030.unk58]);
    /* The archive list is terminated by a zero size and the resident table holds 70 entries. */
    for (k = 0; k < 70 && scene_chunk->size != 0; k++) {
        {
            s32 chunk_offset = scene_chunk->offset + 0x800;
            D_0018CB20.chunks[k] = data_base + *scene_offset + chunk_offset;
        }
        scene_chunk++;
    }
    parse_space_scene_chunk(0);
    D_0015ED5C = 0xFFFFFFFF;
    snd_bank_load_from_ee_cb(data_base + header->sound, store_async_sound_bank_handle,
                             (u32)&D_0015ED5C);
    do {
        FlushCache(0);
        sceGsSyncV(0);
        snd_flush_sound_commands();
    } while (D_0015ED5C == 0xFFFFFFFF);
    snd_resolve_bank_xrefs();
}

extern __typeof__(initialize_level_runtime) func_00230F60 __attribute__((alias("FUN_00230f60")));
