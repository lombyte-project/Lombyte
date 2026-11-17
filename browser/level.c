#include <stddef.h>
#include <stdint.h>
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_class_tables.h"
#include "moby_animation_advance.h"

/*
 * Level actor setup for the browser port.
 *
 * Original C does the per-class and per-instance work:
 *   register_moby_class -> assign_moby_class_data + prepare_resident_class_render_data
 *   init_moby_instance  -> update_moby_animation_state
 *   pack_render_command_fields, store_current_level_table_value
 *
 * load_level_assets_and_gameplay and load_level_gameplay can't be compiled
 * whole yet (VU0 microcode, scratchpad 0x70003000 lighting, ties, sound, text).
 * This adapter walks the same tables those two functions walk; each step names
 * the source line it follows. Original calls it can't make are counted and
 * exported, not skipped silently:
 *   - update_entity (FUN_L00_00250df8, which runs into FUN_L00_00250e30) has no
 *     recovered C; it uses VU0 macro instructions.
 *   - probe_ground_height needs the collision module; Veldin uses no ground snap.
 *   - find_or_assign_bolt_source_slot is unported; Veldin uses no flag 0x10.
 */

enum {
    LEVEL_GUARD_BYTES = 16,
    LEVEL_HEADER_CAPACITY = 0x10000,
    LEVEL_CORE_CAPACITY = 0x1100000,
    LEVEL_GAMEPLAY_CAPACITY = 0x200000,
    LEVEL_MOBY_CAPACITY = 2048,
    LEVEL_CLASS_ENTRY_BYTES = 0x20,
    LEVEL_CLASS_SLOTS = 224,
    LEVEL_CLASS_IDS = 0x800,
    LEVEL_TABLE_CAPACITY = 128,
    LEVEL_UID_CAPACITY = 0x1000,
    LEVEL_SAVE_WORDS = LEVEL_UID_CAPACITY / 32,
    PLACED_MOBY_RECORD_BYTES = 0x78,
    LEVEL_PVAR_CAPACITY = 0x40000,
    LEVEL_PVAR_ENTRIES = 0x1000,
    LEVEL_PATH_CAPACITY = 0x20000,
    LEVEL_PATH_SLOTS = 256
};

/* Error results are distinct so tests can tell which bound failed. */
enum {
    LEVEL_ERROR_STATE = -1,
    LEVEL_ERROR_SIZE = -2,
    LEVEL_ERROR_CLASS_TABLE = -3,
    LEVEL_ERROR_CLASS_ID = -4,
    LEVEL_ERROR_CLASS_DATA = -5,
    LEVEL_ERROR_CLASS_SLOTS = -6,
    LEVEL_ERROR_MOBY_TABLE = -7,
    LEVEL_ERROR_MOBY_RECORD = -8,
    LEVEL_ERROR_MOBY_CAPACITY = -9,
    LEVEL_ERROR_UNPORTED = -10,
    LEVEL_ERROR_GUARDS = -11,
    LEVEL_ERROR_PVARS = -12,
    LEVEL_ERROR_PATHS = -13
};

/* Recovered instance layout in load_level_gameplay.c; the asserts tie it to
   struct Moby so both views of the 0x100-byte instance agree on wasm32. */
typedef struct {
    char pad0[0x24];
    float scale;
    char pad28[0x1C];
    unsigned short flags;
} LevelMobyClass;

typedef struct {
    char pad0[0x10];
    float pos[3];
    char pad1C[4];
    unsigned char state;
    unsigned char group;
    char pad22[2];
    LevelMobyClass *pclass;
    char pad28[4];
    float scale;
    unsigned char update_dist;
    char pad31;
    unsigned short draw_dist;
    unsigned short flags;
    unsigned short occlusion;
    int light;
    char pad3C[4];
    float rot[3];
    char pad4C[0x28];
    void *update;
    int pvar_index;
    char pad7C[4];
    int color;
    char pad84[0x22];
    short moby_class_id;
    char padA8[8];
    unsigned char mission_index;
    unsigned char bolt_source_slot;
    short gameplay_uid;
    short loaded_adjusted_bolt_count;
    short loaded_selected_bolt_count;
    char padB8[0x48];
} LevelMoby;

_Static_assert(sizeof(LevelMoby) == 0x100, "Placed moby instances are 0x100 bytes.");
_Static_assert(sizeof(struct Moby) == 0x100, "struct Moby must keep its EE layout on wasm32.");
_Static_assert(offsetof(LevelMoby, pclass) == offsetof(struct Moby, pclass), "Class pointer layout");
_Static_assert(offsetof(LevelMoby, update) == offsetof(struct Moby, update), "Update pointer layout");
_Static_assert(offsetof(LevelMoby, rot) == offsetof(struct Moby, rot), "Rotation layout");
_Static_assert(offsetof(LevelMoby, moby_class_id) == offsetof(struct Moby, oclass), "Class id layout");
_Static_assert(offsetof(LevelMoby, mission_index) == offsetof(struct Moby, mission), "Mission layout");
_Static_assert(offsetof(LevelMoby, gameplay_uid) == 0xB2, "Gameplay uid layout");

/* Original functions. */
/* Declarations must match the recovered definitions exactly: LTO replaces a
   call through a mismatched IR type with `unreachable`, and wasm-ld cannot
   warn when both types lower to i32. register_moby_class takes the texture
   and material tables as EE addresses (s32). */
extern void register_moby_class(void *, s32, s32, s32) __asm__("FUN_00203640");
extern void init_moby_instance(void *moby, int oclass) __asm__("FUN_0020c5f0");
extern void pack_render_command_fields(u64 *, u64, u64, u64, u64) __asm__("PackRenderCommandFields");
extern void store_current_level_table_value(s32, s32) __asm__("FUN_L00_0024a798");
/* Class updates linked into this module (see runtime_level_tick). */
extern void pterodactyl_update(unsigned char *moby) __asm__("FUN_L00_002e3a88");
extern void level_math_reset(void);

/* The L00 overlay's own copy of assign_moby_class_data (0x264688) reads the
   level table at 0x1EA300; the resident symbol name is D_001E8B80. Data
   symbols cannot alias across objects, so the overlay load is mirrored by
   copying the level table here before registration. */
extern struct MobyClassUpdate l00_moby_classes[101] __asm__("D_L00_001EA300");
struct MobyClassUpdate resident_class_defaults_storage[LEVEL_TABLE_CAPACITY] __asm__("D_001E8B80");
_Static_assert(sizeof(resident_class_defaults_storage) >= sizeof(l00_moby_classes),
               "The resident class table must hold the L00 table.");

/* Data the original chain reads. Values are the zero-initialized ones the
   recovered sources declare; the GS has no browser equivalent yet. */
s32 D_0015FF00 = 0;                               /* next class slot */
s32 gs_texture_allocation_base __asm__("D_0015EE8C") = 0;
u8 *resident_object_pool __asm__("D_0015FF18");
u64 resident_material_templates[48] __asm__("D_0019E540");
u64 special_material_template[3] __asm__("D_0019E6C0");
u64 alternate_special_material_template[3] __asm__("D_0019E6D8");
u8 debug_font_buffer[256 * 0x800] __asm__("D_001AABC0");
s32 current_level_index __asm__("D_0015ED84") = 0;
/* frame_time: 0x3c888889 (1/60) in the boot image; no code on the Veldin path
   that this module runs writes it. */
f32 frame_time __asm__("D_0015ED6C") = 0.0166666675f;
/* toad_paths (D_L00_001B04B0): filled by the hdr->f70 pass below. data.tsv does
   not give its size; Veldin uses 42 and the pass refuses more than 256. */
s32 *toad_paths[LEVEL_PATH_SLOTS] __asm__("D_L00_001B04B0");
/* store_current_level_table_value: per-level slot ranges, copied from the
   L00 image at 0x1C44B8 (level i owns [base[i], base[i + 1])), and the values. */
s32 level_table_value_bases[20] __asm__("D_L00_001C44B8") = {
    0, 2, 15, 19, 34, 37, 46, 50, 54, 58, 60, 65, 69, 74, 86, 93, 100, 107, 114, 121};
s32 level_table_values[121] __asm__("D_L00_001791B8");

/* Classes registered from the core header (the slot tables are original). */
extern void *moby_class_resources[LEVEL_CLASS_SLOTS];
extern s32 moby_class_updates[LEVEL_CLASS_SLOTS];
extern s16 resident_class_id_by_slot[LEVEL_CLASS_SLOTS];
extern u8 resident_class_slot_by_id[LEVEL_CLASS_IDS];

static struct {
    u8 header[LEVEL_HEADER_CAPACITY + LEVEL_GUARD_BYTES];
    u8 core[LEVEL_CORE_CAPACITY + LEVEL_GUARD_BYTES];
    u8 gameplay[LEVEL_GAMEPLAY_CAPACITY + LEVEL_GUARD_BYTES];
    LevelMoby mobys[LEVEL_MOBY_CAPACITY + 1];
    u8 moby_guard[LEVEL_GUARD_BYTES];
    u8 pvars[LEVEL_PVAR_CAPACITY] __attribute__((aligned(64)));
    u8 paths[LEVEL_PATH_CAPACITY] __attribute__((aligned(64)));
} storage __attribute__((aligned(64)));

/* mission_state (D_L00_0015FC88) is the level's 16-byte row of D_0014C050.
   Records with mission -1 read the byte before it: in the L00 image that is
   padding after the "\n" literal at D_L00_0015FC80, which is 0. */
static u8 mission_row[17];
/* mission_state[m] for m in -1..15; index the row with m + 1 rather than
   through a +1 base pointer, because wasm folds that base into an unsigned
   load offset and mission -1 then addresses past 4 GiB. */
#define MISSION_STATE(mission) (mission_row[(mission) + 1])
static u8 exclusion_by_uid[LEVEL_UID_CAPACITY]; /* D_L00_001BB684 */
static u32 level_save_bits[LEVEL_SAVE_WORDS];   /* D_0014C190[level] */
static u32 session_save_bits[LEVEL_SAVE_WORDS]; /* D_L00_001BA4D0 */

static s32 class_count;
static s32 classes_ready;
static s32 moby_count;
static s32 placed_count;
static s32 skipped_count;
static s32 extra_slots;
static s32 pending_entity_updates;
static s32 table_value_stores;
static s32 pvars_ready;
static s32 pvar_users;
static s32 pvar_bytes;
static s32 path_count;               /* D_L00_00160104 */
static s32 pending_pvar_fixups;
static u8 pvar_fixup_pending[LEVEL_PVAR_ENTRIES];
static u8 pvar_used[LEVEL_PVAR_ENTRIES];
static u32 pvar_block_bytes[LEVEL_PVAR_ENTRIES];
static s32 relocated_pvar_pointers;
static s32 tick_count;
static s32 ticked_updates;
static s32 unported_update_calls;
static s32 animation_advances;
static s32 animation_sound_calls;

/* advance_moby_animation's two sound callees (allocate_voice_for_target_entry when a trigger is
   crossed, update_moby_voice for a moby with a voice). The page has no sound driver for them,
   so each call is counted and exported, not made. */
static s32 animation_trigger_sound(s32 trigger_id, s32 zero, void *moby) {
    (void)trigger_id;
    (void)zero;
    (void)moby;
    animation_sound_calls++;
    return 0;
}
static void animation_voice_sound(void *moby) {
    (void)moby;
    animation_sound_calls++;
}
static s32 pending_bounds_refreshes;
static const char *unported_name;

static u32 word_at(const u8 *bytes, u32 offset) {
    const u8 *at = bytes + offset;
    return (u32)at[0] | ((u32)at[1] << 8) | ((u32)at[2] << 16) | ((u32)at[3] << 24);
}

static s32 fits(u32 offset, u32 length, u32 limit) {
    return offset <= limit && length <= limit - offset;
}

static s32 storage_contains(const void *destination, u32 bytes) {
    uintptr_t address = (uintptr_t)destination;
    uintptr_t start = (uintptr_t)&storage;
    return address >= start && address - start <= sizeof(storage) && bytes <= sizeof(storage) - (address - start);
}

/* FillTransferWords: word stores over level-owned memory or the slot table. */
/* The linked caller (init_moby_instance) declares it void; wasm traps on a
   signature mismatch, so the link uses --fatal-warnings to catch drift. */
void fill_transfer_words(void *destination, s32 value, s32 bytes) __asm__("FUN_001f97e8");
void fill_transfer_words(void *destination, s32 value, s32 bytes) {
    u32 *word = destination;
    s32 index;
    uintptr_t address = (uintptr_t)destination;
    s32 slot_table = address >= (uintptr_t)resident_class_slot_by_id &&
                     address + (u32)bytes <= (uintptr_t)resident_class_slot_by_id + LEVEL_CLASS_IDS;

    if (bytes < 0 || (bytes & 3) != 0 || (address & 3) != 0 ||
        (!slot_table && !storage_contains(destination, (u32)bytes))) {
        __builtin_trap();
    }
    for (index = 0; index < bytes / 4; index++) {
        word[index] = (u32)value;
    }
}
extern __typeof__(fill_transfer_words) fill_transfer_words_upper __asm__("func_001F97E8")
    __attribute__((alias("FUN_001f97e8")));

s32 log2dim(s32 dimension) __asm__("FUN_001f97a0");
s32 log2dim(s32 dimension) {
    return 31 - __builtin_clz((u32)dimension);
}
extern __typeof__(log2dim) log2dim_upper __asm__("func_001F97A0") __attribute__((alias("FUN_001f97a0")));

/* Unported functions reached through the L00 class table. Calling one is a
   hard stop that names it (see browser/level-unported.c). */
void level_unported(const char *name) {
    unported_name = name;
    __builtin_trap();
}

static void fill_guards(void) {
    u32 index;
    for (index = 0; index < LEVEL_GUARD_BYTES; index++) {
        storage.header[LEVEL_HEADER_CAPACITY + index] = 0xA5;
        storage.core[LEVEL_CORE_CAPACITY + index] = 0xA5;
        storage.gameplay[LEVEL_GAMEPLAY_CAPACITY + index] = 0xA5;
        storage.moby_guard[index] = 0xA5;
    }
}

s32 runtime_level_guards_valid(void) {
    u32 index;
    for (index = 0; index < LEVEL_GUARD_BYTES; index++) {
        if (storage.header[LEVEL_HEADER_CAPACITY + index] != 0xA5 ||
            storage.core[LEVEL_CORE_CAPACITY + index] != 0xA5 ||
            storage.gameplay[LEVEL_GAMEPLAY_CAPACITY + index] != 0xA5 ||
            storage.moby_guard[index] != 0xA5) {
            return 0;
        }
    }
    return 1;
}

void runtime_level_reset(void) {
    __builtin_memset(&storage, 0, sizeof(storage));
    fill_guards();
    __builtin_memset(moby_class_resources, 0, sizeof(void *) * LEVEL_CLASS_SLOTS);
    __builtin_memset(moby_class_updates, 0, sizeof(s32) * LEVEL_CLASS_SLOTS);
    __builtin_memset(resident_class_id_by_slot, 0, sizeof(s16) * LEVEL_CLASS_SLOTS);
    __builtin_memset(resident_class_slot_by_id, 0xFF, LEVEL_CLASS_IDS);
    __builtin_memset(resident_class_defaults_storage, 0, sizeof(resident_class_defaults_storage));
    __builtin_memset(mission_row, 0, sizeof(mission_row));
    __builtin_memset(exclusion_by_uid, 0, sizeof(exclusion_by_uid));
    __builtin_memset(level_save_bits, 0, sizeof(level_save_bits));
    __builtin_memset(session_save_bits, 0, sizeof(session_save_bits));
    __builtin_memset(level_table_values, 0, sizeof(level_table_values));
    D_0015FF00 = 0;
    current_level_index = 0;
    resident_object_pool = (u8 *)storage.mobys;
    class_count = 0;
    classes_ready = 0;
    moby_count = 0;
    placed_count = 0;
    skipped_count = 0;
    extra_slots = 0;
    pending_entity_updates = 0;
    table_value_stores = 0;
    unported_name = 0;
    pvars_ready = 0;
    pvar_users = 0;
    pvar_bytes = 0;
    path_count = 0;
    pending_pvar_fixups = 0;
    __builtin_memset(pvar_fixup_pending, 0, sizeof(pvar_fixup_pending));
    __builtin_memset(pvar_used, 0, sizeof(pvar_used));
    __builtin_memset(pvar_block_bytes, 0, sizeof(pvar_block_bytes));
    relocated_pvar_pointers = 0;
    __builtin_memset(toad_paths, 0, sizeof(toad_paths));
    tick_count = 0;
    ticked_updates = 0;
    unported_update_calls = 0;
    animation_advances = 0;
    animation_sound_calls = 0;
    pending_bounds_refreshes = 0;
    level_math_reset();
}

u32 runtime_level_header_address(void) { return (u32)(uintptr_t)storage.header; }
u32 runtime_level_header_capacity(void) { return LEVEL_HEADER_CAPACITY; }
u32 runtime_level_core_address(void) { return (u32)(uintptr_t)storage.core; }
u32 runtime_level_core_capacity(void) { return LEVEL_CORE_CAPACITY; }
u32 runtime_level_gameplay_address(void) { return (u32)(uintptr_t)storage.gameplay; }
u32 runtime_level_gameplay_capacity(void) { return LEVEL_GAMEPLAY_CAPACITY; }

/* load_level_gameplay.c:1951: one register_moby_class call per 0x20-byte
   entry of core_header[7], with the moby texture refs at core_header[0xF]
   and the entry's slot map at +0x10. Slot tables start empty
   (fill_transfer_words(resident_class_slot_by_id, -1, 0x800), as in
   transition_load_wad) and the counter starts at 0: resident classes such as
   the hero are not registered by this module. */
static u8 class_seen[LEVEL_CLASS_IDS];

s32 runtime_level_register_classes(s32 header_size, s32 core_size) {
    u32 count;
    u32 table;
    u32 textures;
    u32 index;

    if (classes_ready || header_size < 0x80 || header_size > LEVEL_HEADER_CAPACITY || core_size <= 0 ||
        core_size > LEVEL_CORE_CAPACITY) {
        return LEVEL_ERROR_SIZE;
    }
    if (!runtime_level_guards_valid()) {
        return LEVEL_ERROR_GUARDS;
    }
    count = word_at(storage.header, 0x18);
    table = word_at(storage.header, 0x1C);
    textures = word_at(storage.header, 0x3C);
    if (count > LEVEL_CLASS_SLOTS || !fits(table, count * LEVEL_CLASS_ENTRY_BYTES, (u32)header_size) ||
        textures >= (u32)header_size) {
        return LEVEL_ERROR_CLASS_TABLE;
    }
    __builtin_memset(class_seen, 0, sizeof(class_seen));
    for (index = 0; index < count; index++) {
        const u8 *entry = storage.header + table + index * LEVEL_CLASS_ENTRY_BYTES;
        u32 offset = word_at(entry, 0);
        u32 oclass = word_at(entry, 4);
        if (oclass >= LEVEL_CLASS_IDS || resident_class_slot_by_id[oclass] != 0xFF || class_seen[oclass]) {
            return LEVEL_ERROR_CLASS_ID;
        }
        class_seen[oclass] = 1;
        if (offset != 0 && !fits(offset, 0x50, (u32)core_size)) {
            return LEVEL_ERROR_CLASS_DATA;
        }
    }
    __builtin_memcpy(resident_class_defaults_storage, l00_moby_classes, sizeof(l00_moby_classes));
    for (index = 0; index < count; index++) {
        u8 *entry = storage.header + table + index * LEVEL_CLASS_ENTRY_BYTES;
        u32 offset = word_at(entry, 0);
        register_moby_class(offset ? storage.core + offset : 0, (s32)(uintptr_t)(storage.header + textures),
                            (s32)(uintptr_t)(entry + 0x10), (s32)word_at(entry, 4));
        if (D_0015FF00 > LEVEL_CLASS_SLOTS || !runtime_level_guards_valid()) {
            return LEVEL_ERROR_CLASS_SLOTS;
        }
    }
    class_count = (s32)count;
    classes_ready = 1;
    return class_count;
}

static s32 save_bit(const u32 *bits, s32 uid) {
    return (bits[uid >> 5] >> (uid & 0x1F)) & 1;
}

/* load_level_gameplay.c:932-1103: the hdr->f44 placed-moby loop. */
s32 runtime_level_spawn_mobys(s32 gameplay_size) {
    const u8 *base = storage.gameplay;
    u32 table;
    u32 cursor;
    s32 n;
    s32 n2;
    s32 count = 0;
    s32 i;

    if (!classes_ready || moby_count || gameplay_size < 0x60 || gameplay_size > LEVEL_GAMEPLAY_CAPACITY) {
        return LEVEL_ERROR_STATE;
    }
    table = word_at(base, 0x44);
    if (!fits(table, 16, (u32)gameplay_size)) {
        return LEVEL_ERROR_MOBY_TABLE;
    }
    n = (s32)word_at(base, table);
    n2 = (s32)word_at(base, table + 4);
    if (n < 0 || n2 < 0 || n + n2 > LEVEL_MOBY_CAPACITY) {
        return LEVEL_ERROR_MOBY_CAPACITY;
    }
    cursor = table + 16;
    fill_transfer_words(storage.mobys, 0, (n + n2) << 8);
    for (i = 0; i < n; i++) {
        const u8 *p;
        u32 size;
        s32 mission, flags, uid, bolts, bolts2, selected, adjusted;
        u8 vis = 0xFE;
        s32 skip = 0;

        if (!fits(cursor, PLACED_MOBY_RECORD_BYTES, (u32)gameplay_size)) {
            return LEVEL_ERROR_MOBY_RECORD;
        }
        p = base + cursor;
        size = (word_at(p, 0) >> 2) << 2;
        if (size < PLACED_MOBY_RECORD_BYTES || !fits(cursor, size, (u32)gameplay_size)) {
            return LEVEL_ERROR_MOBY_RECORD;
        }
        mission = (s32)word_at(p, 0x04);
        flags = (s32)word_at(p, 0x08);
        uid = (s32)word_at(p, 0x0C);
        bolts = (s32)word_at(p, 0x10);
        bolts2 = (s32)word_at(p, 0x14);
        selected = bolts;
        adjusted = selected;
        if (mission < -1 || mission > 15 || uid < 0 || uid >= LEVEL_UID_CAPACITY) {
            return LEVEL_ERROR_MOBY_RECORD;
        }
        if (flags != 0) {
            if (flags & 0x10) {
                level_unported("find_or_assign_bolt_source_slot (FUN_L00_00284d90)");
                return LEVEL_ERROR_UNPORTED;
            }
            vis = 0xFF;
            if (exclusion_by_uid[uid] != 0) {
                skip = 1;
            } else if (flags & 3) {
                if (MISSION_STATE(mission) == 0xFF) {
                    if (flags & 2) {
                        selected = bolts2;
                        adjusted = save_bit(session_save_bits, uid) ? (selected + 1) / 2 : selected;
                    } else {
                        skip = 1;
                    }
                } else if (flags & 1) {
                    if (save_bit(level_save_bits, uid)) {
                        adjusted = (bolts + 1) / 2;
                    }
                } else {
                    skip = 1;
                }
            } else if (flags & 8) {
                skip = save_bit(session_save_bits, uid);
            } else if ((flags & 0xC) == 4) {
                skip = save_bit(level_save_bits, uid);
            }
        }
        if (skip) {
            skipped_count++;
        } else {
            LevelMoby *m = &storage.mobys[count];
            s32 oclass = (s32)word_at(p, 0x18);
            u32 table_value;

            if (oclass < 0 || oclass >= LEVEL_CLASS_IDS) {
                return LEVEL_ERROR_MOBY_RECORD;
            }
            init_moby_instance(m, oclass);
            m->bolt_source_slot = vis;
            m->gameplay_uid = (short)uid;
            m->mission_index = (unsigned char)mission;
            m->loaded_adjusted_bolt_count = (short)adjusted;
            m->loaded_selected_bolt_count = (short)selected;
            if (m->pclass != 0) {
                union { u32 u; f32 f; } scale = { word_at(p, 0x1C) };
                m->scale = m->pclass->scale * scale.f;
            }
            m->draw_dist = (unsigned short)word_at(p, 0x20);
            m->update_dist = p[0x24];
            __builtin_memcpy(m->pos, p + 0x30, 12);
            __builtin_memcpy(m->rot, p + 0x3C, 12);
            m->group = p[0x48];
            if (word_at(p, 0x4C) != 0) {
                level_unported("probe_ground_height (FUN_00213508) for a ground-snapped placed moby");
                return LEVEL_ERROR_UNPORTED;
            }
            m->occlusion = 0x7F80;
            m->pvar_index = (int)word_at(p, 0x58);
            if (word_at(p, 0x5C) == 0) {
                m->occlusion = 0;
            }
            if (m->pclass != 0) {
                m->pclass->flags |= (unsigned short)word_at(p, 0x60);
            }
            m->flags |= (unsigned short)word_at(p, 0x60);
            if (m->pclass != 0) {
                pending_entity_updates++; /* update_entity(m): unrecovered */
            }
            m->color = (s32)((word_at(p, 0x6C) << 16) + (word_at(p, 0x68) << 8) + word_at(p, 0x64));
            pack_render_command_fields((u64 *)m, (u64)(s64)m->color, 0, 0, 0);
            m->light = (int)word_at(p, 0x70);
            table_value = word_at(p, 0x74);
            if (table_value != 0xFFFFFFFFU) {
                if (table_value >= (u32)(level_table_value_bases[current_level_index + 1] -
                                         level_table_value_bases[current_level_index])) {
                    return LEVEL_ERROR_MOBY_RECORD;
                }
                store_current_level_table_value((s32)(uintptr_t)m, (s32)table_value);
                table_value_stores++;
            }
            count++;
        }
        cursor += size;
        if (!runtime_level_guards_valid()) {
            return LEVEL_ERROR_GUARDS;
        }
    }
    storage.mobys[count].state = 0xFF;
    moby_count = count;
    placed_count = n;
    extra_slots = n2;
    return count;
}

static u8 moby_fixup_pending[LEVEL_MOBY_CAPACITY];

/* Marks the pvar blocks a pointer-fixup list in the gameplay header targets.
   The hdr->f50 (moby index remap) and hdr->f4C (shared data) fixups are not
   mirrored yet; a moby whose block one targets is never updated
   (runtime_level_tick). */
static s32 mark_pair_fixups(u32 list, u32 size, u32 entries) {
    if (list == 0) {
        return 0;
    }
    for (;;) {
        s32 k;
        if (!fits(list, 4, size)) {
            return LEVEL_ERROR_PVARS;
        }
        k = (s32)word_at(storage.gameplay, list);
        if (k < 0) {
            return 0;
        }
        if ((u32)k >= entries || !fits(list, 8, size)) {
            return LEVEL_ERROR_PVARS;
        }
        if (pvar_used[k]) {
            pvar_fixup_pending[k] = 1;
            pending_pvar_fixups++;
        }
        list += 8;
    }
}

/* load_level_gameplay.c:1107-1158 (moby part) and 1279-1298: copy each
   placed moby's pvar block into the level heap and point moby->pvars at it,
   then copy the path block and fill toad_paths. Original order is cameras
   (D_L00_0015EF50), sound instances, then mobys; this module loads neither
   cameras nor sounds, so their blocks are not copied. The moby blocks are the
   same bytes; only their heap addresses differ. Indices must be unique: a
   second user of an index would read through the rewritten table entry. */
s32 runtime_level_attach_pvars_and_paths(s32 gameplay_size) {
    u8 *hdr = storage.gameplay;
    u32 size = (u32)gameplay_size;
    u32 pv, tbl, entries, mem = 0, k_list;
    s32 i, result;
    static s16 moby_pvar_k[LEVEL_MOBY_CAPACITY];

    if (!moby_count || pvars_ready || gameplay_size < 0x78 || gameplay_size > LEVEL_GAMEPLAY_CAPACITY) {
        return LEVEL_ERROR_STATE;
    }
    __builtin_memset(moby_fixup_pending, 0, sizeof(moby_fixup_pending));
    pv = word_at(hdr, 0x58);
    tbl = word_at(hdr, 0x54);
    if (pv != 0) {
        /* The index table runs up to the pvar data in every level file seen. */
        entries = tbl < pv ? (pv - tbl) / 8 : 0;
        if (entries == 0 || entries > LEVEL_PVAR_ENTRIES || !fits(tbl, entries * 8, size) || pv >= size) {
            return LEVEL_ERROR_PVARS;
        }
        for (i = 0; i < moby_count; i++) {
            LevelMoby *m = &storage.mobys[i];
            s32 k = m->pvar_index;
            u32 from, bytes;

            moby_pvar_k[i] = -1;
            if (k < 0) {
                m->pvar_index = 0;
                continue;
            }
            if ((u32)k >= entries || pvar_used[k]) {
                return LEVEL_ERROR_PVARS;
            }
            from = word_at(hdr, tbl + (u32)k * 8);
            bytes = word_at(hdr, tbl + (u32)k * 8 + 4);
            if (!fits(pv, from, size) || !fits(pv + from, bytes, size) || bytes > LEVEL_PVAR_CAPACITY - mem) {
                return LEVEL_ERROR_PVARS;
            }
            pvar_used[k] = 1;
            pvar_block_bytes[k] = bytes;
            moby_pvar_k[i] = (s16)k;
            m->pvar_index = (int)(uintptr_t)(storage.pvars + mem);
            __builtin_memcpy(storage.pvars + mem, hdr + pv + from, bytes);
            /* tbl[k].f0 = mem: later passes test it against the header address. */
            {
                u32 at = (u32)(uintptr_t)(storage.pvars + mem);
                __builtin_memcpy(hdr + tbl + (u32)k * 8, &at, 4);
            }
            mem += bytes;
            pvar_users++;
        }
        mem = (mem + 0x3F) & ~0x3FU;
        pvar_bytes = (s32)mem;
        if ((result = mark_pair_fixups(word_at(hdr, 0x50), size, entries)) < 0) {
            return result;
        }
        /* load_level_gameplay.c:1177-1192 (hdr->f5C): for each (k, offset) whose
           block was copied, the word at block + offset is relative to the
           block; add the block address. */
        k_list = word_at(hdr, 0x5C);
        for (; k_list != 0; k_list += 8) {
            s32 k;
            u32 offset;
            if (!fits(k_list, 4, size)) {
                return LEVEL_ERROR_PVARS;
            }
            k = (s32)word_at(hdr, k_list);
            if (k < 0) {
                break;
            }
            if ((u32)k >= entries || !fits(k_list, 8, size)) {
                return LEVEL_ERROR_PVARS;
            }
            offset = word_at(hdr, k_list + 4);
            if (pvar_used[k]) {
                u32 block = word_at(hdr, tbl + (u32)k * 8);
                u32 value;
                if (!fits(offset, 4, pvar_block_bytes[k])) {
                    return LEVEL_ERROR_PVARS;
                }
                __builtin_memcpy(&value, (u8 *)(uintptr_t)block + offset, 4);
                value += block;
                __builtin_memcpy((u8 *)(uintptr_t)block + offset, &value, 4);
                relocated_pvar_pointers++;
            }
        }
        k_list = word_at(hdr, 0x4C);
        if (k_list != 0) {
            u32 shared, n, q;
            if (!fits(k_list, 0x10, size)) {
                return LEVEL_ERROR_PVARS;
            }
            shared = word_at(hdr, k_list);
            n = word_at(hdr, k_list + 4);
            q = k_list + 0x10;
            if (!fits(q, shared, size) || !fits(q + shared, n * 8, size) || n > size / 8) {
                return LEVEL_ERROR_PVARS;
            }
            for (q += shared; n != 0; n--, q += 8) {
                u32 k = word_at(hdr, q) & 0xFFFF;
                if (k < entries && pvar_used[k]) {
                    pvar_fixup_pending[k] = 1;
                    pending_pvar_fixups++;
                }
            }
        }
        for (i = 0; i < moby_count; i++) {
            moby_fixup_pending[i] = moby_pvar_k[i] >= 0 && pvar_fixup_pending[moby_pvar_k[i]];
        }
    } else {
        for (i = 0; i < moby_count; i++) {
            storage.mobys[i].pvar_index = 0;
        }
    }

    {
        u32 q = word_at(hdr, 0x70);
        if (q != 0) {
            u32 count, data, bytes;
            if (!fits(q, 0x10, size)) {
                return LEVEL_ERROR_PATHS;
            }
            count = word_at(hdr, q);
            data = q + word_at(hdr, q + 4);
            bytes = word_at(hdr, q + 8);
            if (count > LEVEL_PATH_SLOTS || bytes > LEVEL_PATH_CAPACITY || !fits(data, bytes, size) ||
                !fits(q + 0x10, count * 4, size)) {
                return LEVEL_ERROR_PATHS;
            }
            __builtin_memcpy(storage.paths, hdr + data, bytes);
            for (i = 0; i < (s32)count; i++) {
                u32 offset = word_at(hdr, q + 0x10 + (u32)i * 4);
                u32 points;
                /* A path is a point count, 12 bytes of padding, then 16-byte points. */
                if (!fits(offset, 0x10, bytes)) {
                    return LEVEL_ERROR_PATHS;
                }
                points = word_at(storage.paths, offset);
                if (points == 0 || points > (bytes - offset - 0x10) / 0x10) {
                    return LEVEL_ERROR_PATHS;
                }
                toad_paths[i] = (s32 *)(storage.paths + offset);
            }
            path_count = (s32)count;
        }
    }
    pvars_ready = 1;
    return runtime_level_guards_valid() ? pvar_users : LEVEL_ERROR_GUARDS;
}

/* Class updates this module runs; any other update counts as unported and is
   not called. */
static s32 update_is_ported(const LevelMoby *m) {
    return m->update == (void *)pterodactyl_update;
}

/* One frame of update_visible_mobys (src/mobys/update_visible_mobys.c). Its
   list comes from build_resident_visibility_list (FUN_0020d868, assembly
   only), so this links every live moby the way link_and_update_all_mobys
   does (state without 0x80, flags without 2). Per linked moby with
   state < 0x80:
     advance_moby_animation unless flags & 0x40 -> called (browser/moby-animation-advance.c);
     the class update                          -> called if ported;
     refresh_moby_spatial_bounds unless flags & 4 -> not ported, counted.
   Returns the number of class updates called. */
s32 runtime_level_tick(void) {
    struct Moby *head = 0;
    struct Moby *last = 0;
    struct Moby *current;
    s32 i;
    s32 called = 0;

    if (!pvars_ready) {
        return LEVEL_ERROR_STATE;
    }
    for (i = 0; storage.mobys[i].state != 0xFF; i++) {
        struct Moby *m = (struct Moby *)&storage.mobys[i];
        if (!(m->state & 0x80) && !(m->flags & 2)) {
            if (last != 0) {
                last->next = m;
            } else {
                head = m;
            }
            last = m;
        }
    }
    if (last != 0) {
        last->next = 0;
    }
    for (current = head; current != 0; current = current->next) {
        LevelMoby *m = (LevelMoby *)current;
        if ((s8)current->state < 0) {
            continue;
        }
        if (!(current->flags & 0x40)) {
            rnc_advance_moby_animation((u8 *)current, tick_count, animation_trigger_sound,
                                       animation_voice_sound);
            animation_advances++;
        }
        if (m->update != 0) {
            if (update_is_ported(m) && !moby_fixup_pending[m - storage.mobys]) {
                current->update(current);
                called++;
            } else {
                unported_update_calls++;
            }
        }
        if (!(((volatile struct Moby *)current)->flags & 4)) {
            pending_bounds_refreshes++;
        }
    }
    if (!runtime_level_guards_valid()) {
        return LEVEL_ERROR_GUARDS;
    }
    tick_count++;
    ticked_updates += called;
    return called;
}

s32 runtime_level_pvar_users(void) { return pvar_users; }
s32 runtime_level_pvar_bytes(void) { return pvar_bytes; }
s32 runtime_level_pending_pvar_fixups(void) { return pending_pvar_fixups; }
s32 runtime_level_relocated_pvar_pointers(void) { return relocated_pvar_pointers; }
s32 runtime_level_path_count(void) { return path_count; }
u32 runtime_level_path_address(s32 index) {
    return index >= 0 && index < path_count ? (u32)(uintptr_t)toad_paths[index] : 0;
}
s32 runtime_level_moby_fixup_pending(s32 index) {
    return index >= 0 && index < moby_count ? moby_fixup_pending[index] : -1;
}
s32 runtime_level_tick_count(void) { return tick_count; }
s32 runtime_level_ticked_updates(void) { return ticked_updates; }
s32 runtime_level_unported_update_calls(void) { return unported_update_calls; }
s32 runtime_level_animation_advances(void) { return animation_advances; }
s32 runtime_level_animation_sound_calls(void) { return animation_sound_calls; }
s32 runtime_level_pending_bounds_refreshes(void) { return pending_bounds_refreshes; }

s32 runtime_level_class_count(void) { return class_count; }
s32 runtime_level_class_slot(s32 oclass) {
    return oclass >= 0 && oclass < LEVEL_CLASS_IDS ? resident_class_slot_by_id[oclass] : -1;
}
s32 runtime_level_class_id(s32 slot) {
    return slot >= 0 && slot < class_count ? resident_class_id_by_slot[slot] : -1;
}
s32 runtime_level_class_has_update(s32 slot) {
    return slot >= 0 && slot < class_count ? moby_class_updates[slot] != 0 : -1;
}
s32 runtime_level_class_has_data(s32 slot) {
    return slot >= 0 && slot < class_count ? moby_class_resources[slot] != 0 : -1;
}
s32 runtime_level_moby_count(void) { return moby_count; }
s32 runtime_level_placed_count(void) { return placed_count; }
s32 runtime_level_skipped_count(void) { return skipped_count; }
s32 runtime_level_extra_slots(void) { return extra_slots; }
s32 runtime_level_pending_entity_updates(void) { return pending_entity_updates; }
s32 runtime_level_table_value_stores(void) { return table_value_stores; }
u32 runtime_level_moby_address(s32 index) {
    return index >= 0 && index <= moby_count ? (u32)(uintptr_t)&storage.mobys[index] : 0;
}
u32 runtime_level_moby_stride(void) { return sizeof(LevelMoby); }
u32 runtime_level_unported_name(void) { return (u32)(uintptr_t)unported_name; }
