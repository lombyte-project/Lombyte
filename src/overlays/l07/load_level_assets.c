/* Level 07 level load: relocates the sky, uploads textures, loads the HUD banks and flushes the cache. */
#include "types.h"
#include "asm.h"

/* Level load: relocates sky, uploads textures, loads HUD banks, flushes cache; many DebugPrints. */
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/audio/sound_state.h"
#include "rnc/game/game_globals.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "sda.h"
#include "rnc/rendering/frame/log2dim.h"
#include "rnc/game/clear_scene_state_buffers.h"
#include "rnc/overlay/shared/mobys/link_and_update_all_mobys.h"
#include "rnc/overlay/shared/world/load_two_int_two_float_header.h"
#include "rnc/overlay/shared/runtime/fast_decompress.h"
#include "rnc/overlay/shared/world/load_level_gameplay.h"
#include "rnc/overlay/shared/world/load_level_texture_bank.h"
#include "rnc/world/loaders/relocate_asset_entry_pointers.h"
#include "rnc/ui/hud/banks/load_hud_banks.h"
#include "rnc/world/loaders/register_moby_class.h"
#include "rnc/world/loaders/relocate_sky_definition.h"
#include "rnc/world/loaders/unpack_point_records.h"
#include "rnc/world/loaders/upload_texture_images.h"

typedef struct {
    char *f0; /* 0x00 */
    s32  *f4; /* 0x04: level header */
    char  pad8[4];
    char *fC;  /* 0x0C */
    char *f10; /* 0x10 */
    char *f14; /* 0x14: base the header's offsets are taken from */
    char *f18; /* 0x18 */
    char *f1C; /* 0x1C */
} Load26b768;

typedef struct {
    u8  pad0[0x1A];
    s16 sound; /* 0x1A */
    s32 time;  /* 0x1C */
} Slot26b768;

typedef struct {
    u8          pad0[0xC];
    u8          seq_count; /* 0x0C */
    u8          count0D;   /* 0x0D */
    u8          pad0E[0x1A];
    Slot26b768 *f28; /* 0x28 */
    u8          pad2C[0x1C];
    char       *seqs[1]; /* 0x48 */
} Class26b768;

typedef struct {
    s32 offset;
    u16 kind;
    u16 pad6;
    s32 pad8[2];
} TexRef26b768;

typedef struct {
    s16 off;
    s16 count;
} Span26b768;

extern Load26b768 D_L18_00174800_26b768 __asm__("D_L07_00173E80");
extern char      *D_L18_00174840_26b768[] __asm__("D_L07_00173EC0");

typedef struct {
    s16  a;
    s16  b;
    s32 *file;
} Ldg26b768;

extern Ldg26b768 D_0015EE48;

#include "rnc/overlay/l07/load_level_assets_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern u64          D_L07_00182840[3];
extern u64          l07_64x64_texture_state[3] __asm__("D_L07_00182858");
extern u64          D_L07_00182870[3];
extern s32          l07_tfrag_texture_address_words[] __asm__("D_L07_001C4A80");
extern s32          l07_moby_texture_address_words[] __asm__("D_L07_00199B00");
extern u8           l07_moby_texture_reference_copy[] __asm__("D_L07_001AEC40");
extern s32          l07_tie_texture_address_words[] __asm__("D_L07_001C6780");
extern s32          l07_shrub_texture_address_words[] __asm__("D_L07_001BC630");
extern char        *D_L18_0015F600_26b768 __asm__("D_L07_0015F600");
extern Class26b768 *l07_moby_classes[] __asm__("D_L07_00197380");
extern s32          D_L07_0016000C;
extern s32          D_L07_00160010;
extern s32          l07_metadata_moby_class_ids[] __asm__("D_L07_001AFC40");
extern char        *l07_metadata_moby_payload_bases[] __asm__("D_L07_001AFCA0");
extern s32          D_L07_001AFD00[];
typedef int         Q26b768 __attribute__((mode(TI)));
extern Q26b768      l07_metadata_moby_texture_slot_maps[] __asm__("D_L07_001AFD60");
extern u8           l07_moby_registration_indices_by_class_id[] __asm__("D_L07_00197C40");
extern char        *l07_effects_texture_data_base __asm__("D_L07_0015F420");
extern s32          D_L07_0015FC98;
extern s32          D_L07_0015FC9C;
extern s32          D_L07_0015FCA0;
extern s32          D_L07_0015FCA4;
extern char        *D_L07_0015F604;
extern Slot26b768  *D_L07_0015F5F4;
extern s16          l07_moby_class_ids[] __asm__("D_L07_00197A80");
extern u16          l07_metadata_moby_sound_ids[][16] __asm__("D_L07_001AFEE0");
extern char         D_L07_0020FA20[], D_L07_0020FA70[], D_L07_0020FB30[], D_L07_0020FCB8[],
    D_L07_0020FCE8[];
extern char  l07_memory_report_newline[] __asm__("D_L07_0015FC80");
extern char  l07_code_memory_end[] __asm__("D_L07_0032C980");
char         D_L07_0020FA40[] = "internal moby sound mismatch on moby %d\n";
char         D_L07_0020FA88[] = "os:             %.1fK\n";
char         D_L07_0020FAA0[] = "code mem:       %.1fK\n";
char         D_L07_0020FAB8[] = "dead space:     %.1fK\n";
char         D_L07_0020FAD0[] = "vu chain bufs:  %.1fK\n";
char         D_L07_0020FAE8[] = "hud:            %.1fK\n";
char         D_L07_0020FB00[] = "gadget buffer:  %.1fK\n";
char         D_L07_0020FB18[] = "tfrag geom:     %.1fK\n";
char         D_L07_0020FB50[] = "occlusion:      %.1fK\n";
char         D_L07_0020FB68[] = "sky:            %.1fK\n";
char         D_L07_0020FB80[] = "collision:      %.1fK\n";
char         D_L07_0020FB98[] = "shared vram:    %.1fK\n";
char         D_L07_0020FBB0[] = "particle vram:  %.1fK\n";
char         D_L07_0020FBC8[] = "effects vram:   %.1fK\n";
char         D_L07_0020FBE0[] = "mobys:          %.1fK\n";
char         D_L07_0020FBF8[] = "ties:           %.1fK\n";
char         D_L07_0020FC10[] = "shrubs:         %.1fK\n";
char         D_L07_0020FC28[] = "ratchett seqs:  %.1fK\n";
char         D_L07_0020FC40[] = "tie insts:      %.1fK\n";
char         D_L07_0020FC58[] = "shrub insts:    %.1fK\n";
char         D_L07_0020FC70[] = "moby insts:     %.1fK\n";
char         D_L07_0020FC88[] = "moby pvars:     %.1fK\n";
char         D_L07_0020FCA0[] = "paths:          %.1fK\n";
char         D_L07_0020FCD0[] = "end of mem:     %.1fM\n";
extern s32   l07_particle_texture_range_words[] __asm__("D_L07_001B1B00");
extern char *D_L18_001C6200_26b768[] __asm__("D_L07_001C5880");
extern char *D_L18_001BCA30_26b768[] __asm__("D_L07_001BC0B0");
extern char *D_L18_00160FC0_26b768 __asm__("D_L07_00160FC0");
extern char *D_L18_00160494_26b768 __asm__("D_L07_00160494");
extern char *D_L18_0015FFE8_26b768 __asm__("D_L07_0015FFE8");
extern char *D_L18_00160108_26b768 __asm__("D_L07_00160108");
extern char *l07_particle_buffer __asm__("D_L07_0016016C");

typedef struct {
    u8  pad[0x10];
    u32 v __attribute__((packed, aligned(1)));
} Una26b768;

extern u8 D_L07_00179180[];

extern void flush_cache(s32) __asm__("FUN_00118a80");
extern void initialize_tfrag_render_data(char *, void *) __asm__("FUN_002040e0");
extern void resolve_level_asset_block_pointers(char *) __asm__("FUN_L00_00241728");
extern void print_moby_class_sizes(void *, s32) __asm__("FUN_L00_00241788");
extern void register_object_render_class(char *, void *, void *, s32) __asm__("FUN_00203730");
extern void noop_main_callback(void *, s32) __asm__("FUN_001e93e8");
extern void
register_shrub_render_class(char *, void *, void *, void *, s32) __asm__("FUN_00203b08");
extern void parse_particle_textures(void *, char *, void *, s32) __asm__("FUN_002026c8");
extern void debug_print(const char *, ...) __asm__("FUN_001e93b0");
extern void fill_transfer_words(void *, s32, s32) __asm__("FUN_001f97e8");
extern void restore_or_clear_level_state(void) __asm__("FUN_L00_00285008");

#define GS_TEX0(tbp0, tbw, psm, tw, th, tcc, tfx, cbp, cpsm, csm, csa, cld)             \
    ((u64)(tbp0) | ((u64)(tbw) << 14) | ((u64)(psm) << 20) | ((u64)(tw) << 26) |        \
     ((u64)(th) << 30) | ((u64)(tcc) << 34) | ((u64)(tfx) << 35) | ((u64)(cbp) << 37) | \
     ((u64)(cpsm) << 51) | ((u64)(csm) << 55) | ((u64)(csa) << 56) | ((u64)(cld) << 61))
#define KB(x) (convert_integer_to_float((s32)(x)) * (1.0f / 1024.0f))

extern u8 D_L07_0015FC88_macro[] __asm__("D_L07_0015FC88") MACRO_ADDR;
/* Reads the level's core data. Its index lists the object classes:
 *   0x18  moby classes, 0x20 bytes per entry
 *   0x20  tie classes, 0x20 bytes per entry
 *   0x28  shrub classes, 0x30 bytes per entry
 * Each entry holds the offset of the class data, the class number, and at 0x10
 * sixteen bytes that say which level texture each texture slot uses (0xff
 * means unused). Each class stores a scale (ties at 0x40, shrubs at 0x20,
 * mobies at 0x24 of their data); a stored coordinate times scale / 1024 gives
 * world units. Index 0x10 gives where the sky data starts and 0x14 where the
 * collision data starts. */
s32 load_level_assets(s32 full, s32 keep) __asm__("FUN_L07_0026b768");

s32 load_level_assets(s32 full, s32 keep) {
    Load26b768   *ld = &D_L18_00174800_26b768;
    s32          *level_header = ld->f4;
    s32          *level_file_base = D_0015EE48.file;
    char         *level_asset_base;
    s32           used;
    char         *sound_id_cursor;
    char         *free;
    char         *end;
    Span26b768   *span;
    TexRef26b768 *moby_texture_refs;
    TexRef26b768 *tfrag_texture_refs;
    TexRef26b768 *tie_texture_refs;
    TexRef26b768 *shrub_texture_refs;
    char         *sound_table_base;
    s32           i;
    s32           j;
    s32           sound_slot_count;
    s32           k;
    s32           tfrag_texture_index, moby_texture_index, tie_texture_index, shrub_texture_index;
    s32           class_metadata_index;
    char         *sequence_offset_cursor;
    s32           sq;
    char         *sp;
    char         *class_metadata_entry_cursor;
    s32           z;
    s32           ci;
    char         *x;
    char         *y;
    s32           a1, a2, sound_slot_index;
    char         *q1, *class_entry_cursor;
    u64          *t;

    if (full)
        load_hud_banks();
    level_asset_base = ld->f14;
    if (full) {
        fast_mem_copy(
            level_header, (char *)(level_file_base[4] + (s32)level_file_base), level_file_base[5]);
        l07_texture_data_base = 0;
        copy_memory_aligned(D_L07_0015FC88_macro, visited52 + current_level_index * 16, 0x10);
        upload_texture_images((char *)(level_file_base[6] + (s32)level_file_base),
                              level_header[0],
                              (char *)level_header + level_header[1]);
        l07_asset_sound_bank_handle = sound_bank_handle;
        flush_cache(0);
        used = fast_decompress((char *)(level_file_base[0x14] + (s32)level_file_base),
                               level_asset_base);
        l07_texture_data_base = (s32)(level_asset_base + level_header[0x18]);

        D_L07_00182840[0] = GS_TEX0((gs_texture_allocation_base + level_header[0x24]) >> 8,
                                    2,
                                    0x13,
                                    7,
                                    7,
                                    1,
                                    0,
                                    (gs_texture_allocation_base + level_header[0x25]) >> 8,
                                    0,
                                    0,
                                    0,
                                    4);
        D_L07_00182840[1] = 0x0000FFA0000000E0;
        D_L07_00182840[2] = 0x0040000400004000;
        l07_64x64_texture_state[0] = GS_TEX0((gs_texture_allocation_base + level_header[0x26]) >> 8,
                                             1,
                                             0x13,
                                             6,
                                             6,
                                             1,
                                             0,
                                             (gs_texture_allocation_base + level_header[0x27]) >> 8,
                                             0,
                                             0,
                                             0,
                                             4);
        l07_64x64_texture_state[1] = 0x0000FFA0000000E0;
        l07_64x64_texture_state[2] = 0x0040000400004000;
        D_L07_00182870[0] = GS_TEX0((gs_texture_allocation_base + level_header[0x30]) >> 8,
                                    2,
                                    0x13,
                                    7,
                                    7,
                                    1,
                                    0,
                                    (gs_texture_allocation_base + level_header[0x31]) >> 8,
                                    0,
                                    0,
                                    0,
                                    4);
        D_L07_00182870[1] = 0x0000FFA0000000E0;
        D_L07_00182870[2] = 0x0040000400004000;

        tfrag_texture_refs = (TexRef26b768 *)((char *)level_header + level_header[0xD]);
        l07_tfrag_texture_count = level_header[0xC];
        for (tfrag_texture_index = 0; tfrag_texture_index < l07_tfrag_texture_count;
             tfrag_texture_index++)
            l07_tfrag_texture_address_words[tfrag_texture_index] =
                l07_texture_data_base + tfrag_texture_refs[tfrag_texture_index].offset +
                (log2dim(tfrag_texture_refs[tfrag_texture_index].kind) << 28);
        moby_texture_refs = (TexRef26b768 *)((char *)level_header + level_header[0xF]);
        l07_moby_texture_count = level_header[0xE];
        for (moby_texture_index = 0; moby_texture_index < l07_moby_texture_count;
             moby_texture_index++)
            l07_moby_texture_address_words[moby_texture_index] =
                l07_texture_data_base + moby_texture_refs[moby_texture_index].offset +
                (log2dim(moby_texture_refs[moby_texture_index].kind) << 28);
        fast_mem_copy(
            l07_moby_texture_reference_copy, moby_texture_refs, l07_moby_texture_count * 16);
        tie_texture_refs = (TexRef26b768 *)((char *)level_header + level_header[0x11]);
        l07_tie_texture_count = level_header[0x10];
        for (tie_texture_index = 0; tie_texture_index < l07_tie_texture_count; tie_texture_index++)
            l07_tie_texture_address_words[tie_texture_index] =
                l07_texture_data_base + tie_texture_refs[tie_texture_index].offset +
                (log2dim(tie_texture_refs[tie_texture_index].kind) << 28);
        shrub_texture_refs = (TexRef26b768 *)((char *)level_header + level_header[0x13]);
        l07_shrub_texture_count = level_header[0x12];
        for (shrub_texture_index = 0; shrub_texture_index < l07_shrub_texture_count;
             shrub_texture_index++)
            l07_shrub_texture_address_words[shrub_texture_index] =
                l07_texture_data_base + shrub_texture_refs[shrub_texture_index].offset +
                (log2dim(shrub_texture_refs[shrub_texture_index].kind) << 28);

        initialize_tfrag_render_data(level_asset_base + level_header[2],
                                     (char *)level_header + level_header[0xD]);
        if (level_header[3])
            D_L18_0015F600_26b768 = level_asset_base + level_header[3];
        else
            D_L18_0015F600_26b768 = 0;
        if (level_header[4])
            relocate_sky_definition(level_asset_base + level_header[4]);
        else
            l07_sky_definition_base = 0;
        resolve_level_asset_block_pointers(level_asset_base + level_header[5]);
        for (i = 0, class_entry_cursor = (char *)level_header + level_header[7];
             i < level_header[6];
             i++, class_entry_cursor += 0x20)
            register_moby_class(
                *(s32 *)class_entry_cursor ? level_asset_base + *(s32 *)class_entry_cursor : 0,
                (char *)level_header + level_header[0xF],
                class_entry_cursor + 0x10,
                *(s32 *)(class_entry_cursor + 4));
        print_moby_class_sizes((char *)level_header + level_header[7], level_header[7]);
        for (sq = 0, sequence_offset_cursor = (char *)level_header + level_header[0x1E];
             sq < l07_moby_classes[0]->seq_count;
             sq++, sequence_offset_cursor += 4) {
            if (*(s32 *)sequence_offset_cursor) {
                l07_moby_classes[0]->seqs[sq] = level_asset_base + *(s32 *)sequence_offset_cursor;
                relocate_asset_entry_pointers(l07_moby_classes[0], sq);
            }
        }
        D_L07_0016000C = -1;
        class_metadata_entry_cursor = (char *)level_header + level_header[0x21];
        D_L07_00160010 = 0;
        l07_class_metadata_count = level_header[0x20];
        for (class_metadata_index = 0; class_metadata_index < level_header[0x20];
             class_metadata_index++, class_metadata_entry_cursor += 0x10) {
            l07_metadata_moby_class_ids[class_metadata_index] =
                *(s32 *)(class_metadata_entry_cursor + 4);
            l07_metadata_moby_payload_bases[class_metadata_index] =
                level_asset_base + *(s32 *)class_metadata_entry_cursor;
            D_L07_001AFD00[class_metadata_index] = *(s32 *)(class_metadata_entry_cursor + 8);
            {
                s32 moby_texture_slot_map_offset = l07_moby_registration_indices_by_class_id[*(
                                                       s32 *)(class_metadata_entry_cursor + 4)] *
                                                       32 +
                                                   0x10;
                qcopy(&l07_metadata_moby_texture_slot_maps[class_metadata_index],
                      (char *)level_header + level_header[7] + moby_texture_slot_map_offset);
            }
        }
        for (i = 0, class_entry_cursor = (char *)level_header + level_header[9];
             i < level_header[8];
             i++, class_entry_cursor += 0x20)
            register_object_render_class(level_asset_base + *(s32 *)class_entry_cursor,
                                         (char *)level_header + level_header[0x11],
                                         class_entry_cursor + 0x10,
                                         *(s32 *)(class_entry_cursor + 4));
        noop_main_callback((char *)level_header + level_header[9], level_header[8]);
        for (i = 0, class_entry_cursor = (char *)level_header + level_header[0xB];
             i < level_header[0xA];
             i++, class_entry_cursor += 0x30)
            register_shrub_render_class(level_asset_base + *(s32 *)class_entry_cursor,
                                        (char *)level_header + level_header[0x13],
                                        class_entry_cursor + 0x10,
                                        class_entry_cursor + 0x20,
                                        *(s32 *)(class_entry_cursor + 4));
        l07_effects_texture_data_base = level_asset_base + level_header[0x1A];
        unpack_point_records((char *)level_header + level_header[0x17], level_header[0x16]);
        if (level_header[0x28]) {
            relocate_counted_pointer_table(level_asset_base + level_header[0x28]);
        } else {
            l07_counted_pointer_table_count = 0;
            l07_counted_pointer_table_records_base = 0;
            l07_counted_pointer_table_base = 0;
        }
        if (level_header[0x29]) {
            load_two_int_two_float_header(level_asset_base + level_header[0x29]);
        } else {
            D_L07_0015FC98 = 0;
            D_L07_0015FC9C = 0;
            D_L07_0015FCA4 = 0;
            D_L07_0015FCA0 = 0;
            l07_two_int_two_float_payload_base = 0;
        }
        if (level_header[0x2A])
            D_L07_0015F604 = level_asset_base + level_header[0x2A];
        else
            D_L07_0015F604 = 0;
        {
            char *x1 = level_asset_base + level_header[0x19];
            char *x2 = (char *)level_header + level_header[0x15];
            parse_particle_textures(
                (char *)level_header + level_header[0x1B], x1, x2, level_header[0x14]);
        }

        sound_table_base = (char *)level_header + level_header[0x1C];
        span = (Span26b768 *)sound_table_base;
        free = (char *)((u32)(level_asset_base + used + 0x3F) & ~0x3F);
        sound_slot_count = span->count;
        D_L07_0015F5F4 = (Slot26b768 *)free;
        l07_sound_slot_count = sound_slot_count;
        copy_memory_aligned(free, sound_table_base + span->off, sound_slot_count * 32);
        span++;
        free += sound_slot_count * 32;
        sound_id_cursor = sound_table_base + span->off;
        for (sound_slot_index = 0; sound_slot_index < l07_sound_slot_count; sound_slot_index++) {
            if (D_L07_0015F5F4[sound_slot_index].sound >= span->count) {
                debug_print(D_L07_0020FA20);
                D_L07_0015F5F4[sound_slot_index].sound = -1;
            } else {
                D_L07_0015F5F4[sound_slot_index].sound =
                    *(u16 *)(sound_id_cursor + D_L07_0015F5F4[sound_slot_index].sound * 4);
            }
            D_L07_0015F5F4[sound_slot_index].time = sound_bank_handle;
        }
        end = free + 0x3F;
        span++;
        for (ci = 0; ci < l07_moby_class_count; ci++, span++) {
            Class26b768 *moby_class = l07_moby_classes[ci];
            if (moby_class == 0) {
                for (j = 0; j < l07_class_metadata_count; j++)
                    if (l07_metadata_moby_class_ids[j] == l07_moby_class_ids[ci])
                        break;
                if (j < l07_class_metadata_count) {
                    fill_transfer_words(l07_metadata_moby_sound_ids[j], -1, 0x20);
                    sound_id_cursor = sound_table_base + span->off;
                    for (k = 0; k < span->count && k < 15; k++, sound_id_cursor += 4)
                        l07_metadata_moby_sound_ids[j][k] = *(u16 *)sound_id_cursor;
                }
            } else {
                if (span->count != moby_class->count0D)
                    debug_print(D_L07_0020FA40, l07_moby_class_ids[ci]);
                sound_id_cursor = sound_table_base + span->off;
                for (k = 0; k < l07_moby_classes[ci]->count0D; k++, sound_id_cursor += 4) {
                    l07_moby_classes[ci]->f28[k].sound = *(u16 *)sound_id_cursor;
                    l07_moby_classes[ci]->f28[k].time = sound_bank_handle;
                }
            }
        }
        D_L18_00174800_26b768.f18 =
            load_level_texture_bank((char *)((u32)end & ~0x3F), level_header);
    }
    D_L18_00174800_26b768.f1C = load_level_gameplay(full);
    if (full) {
        char *top;
        debug_print(D_L07_0020FA70);
        debug_print(D_L07_0020FA88, KB(0x100000));
        top = l07_code_memory_end;
        debug_print(D_L07_0020FAA0, KB(top - 0x100000));
        debug_print(D_L07_0020FAB8, KB(D_L18_00174800_26b768.f0 - top));
        debug_print(D_L07_0020FAD0, KB(D_L18_00174800_26b768.fC - D_L18_00174800_26b768.f0));
        debug_print(D_L07_0020FAE8, KB(D_L18_00174800_26b768.f10 - D_L18_00174800_26b768.fC));
        debug_print(D_L07_0020FB00, KB(D_L18_00174800_26b768.f14 - D_L18_00174800_26b768.f10));
        if (D_L18_0015F600_26b768 != 0)
            debug_print(D_L07_0020FB18, KB(D_L18_0015F600_26b768 - D_L18_00174800_26b768.f14));
        else if (l07_sky_definition_base != 0)
            debug_print(D_L07_0020FB18,
                        KB((char *)l07_sky_definition_base - D_L18_00174800_26b768.f14));
        else
            debug_print(D_L07_0020FB30);
        if (D_L18_0015F600_26b768 != 0) {
            if (l07_sky_definition_base != 0)
                debug_print(D_L07_0020FB50,
                            KB(l07_sky_definition_base - (s32)D_L18_0015F600_26b768));
            else
                debug_print(D_L07_0020FB50, KB(D_L18_00174840_26b768[0] - D_L18_0015F600_26b768));
        }
        if (level_header[4])
            debug_print(D_L07_0020FB68,
                        KB(D_L18_00174840_26b768[0] - (char *)l07_sky_definition_base));
        debug_print(D_L07_0020FB80, KB(l07_texture_data_base - (s32)D_L18_00174840_26b768[0]));
        debug_print(D_L07_0020FB98,
                    KB((l07_particle_texture_range_words[1] >> 4) - l07_texture_data_base));
        debug_print(
            D_L07_0020FBB0,
            KB((s32)l07_effects_texture_data_base - (l07_particle_texture_range_words[1] >> 4)));
        debug_print(D_L07_0020FBC8,
                    KB((char *)l07_moby_classes[0] - l07_effects_texture_data_base));
        debug_print(D_L07_0020FBE0, KB(D_L18_001C6200_26b768[0] - (char *)l07_moby_classes[0]));
        if (level_header[0xA])
            debug_print(D_L07_0020FBF8, KB(D_L18_001BCA30_26b768[0] - D_L18_001C6200_26b768[0]));
        else
            debug_print(D_L07_0020FBF8, KB(D_L18_00160FC0_26b768 - D_L18_001C6200_26b768[0]));
        if (level_header[0xA])
            debug_print(D_L07_0020FC10,
                        KB(l07_moby_classes[0]->seqs[0] - D_L18_001BCA30_26b768[0]));
        debug_print(D_L07_0020FC28, KB(D_L18_00160FC0_26b768 - l07_moby_classes[0]->seqs[0]));
        debug_print(D_L07_0020FC40, KB(D_L18_00160494_26b768 - D_L18_00160FC0_26b768));
        debug_print(D_L07_0020FC58, KB(level_mobys - D_L18_00160494_26b768));
        debug_print(D_L07_0020FC70, KB(D_L18_0015FFE8_26b768 - level_mobys));
        debug_print(D_L07_0020FC88, KB(D_L18_00160108_26b768 - D_L18_0015FFE8_26b768));
        debug_print(D_L07_0020FCA0, KB(l07_particle_buffer - D_L18_00160108_26b768));
        debug_print(D_L07_0020FCB8, 128.0);
        debug_print(l07_memory_report_newline);
        debug_print(D_L07_0020FCD0,
                    convert_integer_to_float((s32)l07_particle_buffer + 0x20000) *
                        (1.0f / 1048576.0f));
        debug_print(l07_memory_report_newline);
        {
            char *lo = D_L18_00174800_26b768.f0 - 0x2D0000;
            debug_print(D_L07_0020FCE8,
                        convert_integer_to_float(D_L18_00174800_26b768.f1C - lo) *
                            (1.0f / 1048576.0f));
        }
        debug_print(l07_memory_report_newline);
    }
    if (keep) {
        restore_or_clear_level_state();
    } else {
        clear_scene_state_buffers();
        for (z = 15; z >= 0; z--)
            D_0014EE90[z] = 0;
    }
    ((Una26b768 *)D_L07_00179180)->v = 0;
    link_and_update_all_mobys();
    D_L07_0015F5CC++;
    return 0;
}

s32 D_L07_00173E9C[1] __attribute__((section(".data"))) = {0};
