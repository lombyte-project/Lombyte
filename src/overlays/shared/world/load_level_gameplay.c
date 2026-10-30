/* World: loads the level gameplay and assets, resolves asset blocks and starts cutscene chunk reads. */
#include "types.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/overlay/point_light.h"
#include "rnc/overlay/texture_ref.h"
#include "rnc/overlay/level_spline.h"
#include "asm.h"

#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/globals.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/audio/streaming/start_audio_stream_read.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/rendering/frame/log2dim.h"
#include "rnc/mobys/init_moby_instance.h"
#include "rnc/game/clear_scene_state_buffers.h"
#include "rnc/overlay/shared/mobys/link_and_update_all_mobys.h"
#include "rnc/overlay/shared/world/load_two_int_two_float_header.h"
#include "rnc/overlay/shared/runtime/fast_decompress.h"
#include "rnc/overlay/shared/world/load_level_texture_bank.h"
#include "rnc/rendering/ties/light_ties.h"
#include "rnc/world/loaders/relocate_asset_entry_pointers.h"
#include "rnc/ui/hud/banks/load_hud_banks.h"
#include "rnc/world/loaders/register_moby_class.h"
#include "rnc/world/loaders/relocate_sky_definition.h"
#include "rnc/world/loaders/unpack_point_records.h"
#include "rnc/world/loaders/upload_texture_images.h"

typedef struct {
    s32 a, b, c, off;
} Ent;

typedef struct {
    s32 n;
    s32 pad[3];
    Ent e[1];
} Tbl;

extern u8 *D_L00_00173E40 NOT_SDA;
extern Tbl *l00_level_asset_entry_table __asm__("D_L00_0015F618") __attribute__((sda));
/* Sets D_L00_00173E40 from offset h[0] and turns the table at offset h[1] into pointers. */
void resolve_level_asset_block_pointers(s32 *offset_header) __asm__("FUN_L00_00241728");

void resolve_level_asset_block_pointers(s32 *offset_header) {
    s32  i;
    Tbl *entry_table;
    if (offset_header[0])
        D_L00_00173E40 = (u8 *)offset_header + offset_header[0];
    if (offset_header[1]) {
        entry_table = (Tbl *)((u8 *)offset_header + offset_header[1]);
        l00_level_asset_entry_table = entry_table;
        for (i = 0; i < entry_table->n; i++) {
            entry_table->e[i].off = (s32)((u8 *)entry_table + entry_table->e[i].off);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/shared/loaders_00240398.c: func_L00_00242120), where it is exact; names translated to the US level program. */

typedef struct {
    f32 class_id;
    f32 size;
} MobyClassSize;

extern char moby_class_size_format[] __asm__("D_L00_001E8640");
extern void debug_print(const char *, ...) __asm__("FUN_001e93b0");
extern int  fptodp(float) __asm__("FUN_00120478");

void print_moby_class_sizes(int *record, int n) __asm__("FUN_L00_00241788");

void print_moby_class_sizes(int *record, int n) {
    MobyClassSize entries[512];
    int           i, j, k, done;

    for (i = 0; i < n; i++, record += 8) {
        entries[i].class_id = (float)record[1];
        if (record[0] == 0) {
            entries[i].size = 0;
        } else {
            if (record[8] != 0) {
                entries[i].size = convert_integer_to_float(record[8] - record[0]) * 0.0009765625f;
            } else {
                entries[i].size =
                    convert_integer_to_float(record[0x10] - record[0]) * 0.0009765625f;
            }
            entries[i + 1].class_id = -1.0f;
            entries[i + 1].size = 0;
        }
    }
    do {
        done = 1;
        for (j = 0; j < n; j++) {
            if (entries[j].size < entries[j + 1].size) {
                float class_id = entries[j].class_id;
                float size = entries[j].size;
                done = 0;
                entries[j].class_id = entries[j + 1].class_id;
                entries[j].size = entries[j + 1].size;
                entries[j + 1].class_id = class_id;
                entries[j + 1].size = size;
            }
        }
    } while (!done);
    for (k = 0; k < n; k++) {
        int class_id = truncate_float_to_s32(entries[k].class_id);
        debug_print(moby_class_size_format, class_id, fptodp(entries[k].size));
    }
}
/* Reads the gameplay file of a level, which lists the objects placed in the
 * world. The file header holds offsets to lists. Each list starts with the
 * number of entries, then 12 bytes of padding, then the entries:
 *   0x34  ties (large static objects), 0xe0 bytes each: class, then a 4x4 matrix at 0x10
 *   0x3c  shrubs (small plants), 0x70 bytes each, same start as ties
 *   0x44  mobies (moving objects), 0x78 bytes each: class at 0x18, scale at 0x1c,
 *         position at 0x30, rotation angles at 0x3c
 * The moby of class 0 marks where the hero starts.
 * Colours: a tie has 64 colours at 0x50 (2 bytes each, 5 bits per channel),
 * copied for the renderer. A shrub has red, green and blue as whole numbers at
 * 0x50, 0x54, 0x58; they are packed with alpha 0x80 into one colour for the
 * whole shrub. A moby has red, green and blue at 0x64, 0x68, 0x6c. */

#include "qcopy.h"
#include "rnc/audio/voice_pool.h"

/* Ported from rac1-decomp (src/overlays/shared/loaders_00240398.c: func_L00_002422D8), where it is exact; names translated to the US level program. */

typedef struct {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10[9];
    int f34;
    int f38;
    int f3C;
    int f40;
    int f44;
    int f48;
    int f4C;
    int f50;
    int f54;
    int f58;
    int f5C;
    int f60;
    int f64;
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
} LevelHeader;

typedef struct {
    char         pad0[4];
    LevelHeader *f4;
    char         pad8[0x10];
    char        *f18;
} LevelLoad173E00;

typedef struct {
    int   f0;
    char *f4;
    char *f8;
    int   fC;
    int   f10;
    int   f14;
} LevelLoad173E40;

typedef struct {
    char pad0[0x23C];
    int  f23C;
    int  f240;
    int  f244;
} LevelState16CB40;

typedef struct {
    int f0;
    int f4;
} PvarEntry;

typedef int Qword __attribute__((mode(TI)));

typedef struct TieMatrix {
    float m0[3];
    float fC;
    float m1[3];
    float f1C;
    float m2[3];
    float f2C;
    float m3[3];
    float f3C;
    char  pad40[0x100];
    char  f140[0x80];
} TieMatrix;

typedef struct TieInst {
    float          f0[3];
    float          fC;
    TieMatrix     *f10;
    float          f14;
    unsigned short f18;
    unsigned char  f1A;
    unsigned char  f1B;
    unsigned short f1C;
    unsigned short f1E;
} TieInst;

typedef struct {
    char           pad0[0x26];
    unsigned short f26;
    TieInst       *f28;
    char           pad2C[4];
    float          f30[3];
    float          f3C;
    float          f40;
} TieClass;

typedef struct ShrubMatrix {
    float m0[3];
    int   fC;
    float m1[3];
    int   f1C;
    float m2[3];
    int   f2C;
    float m3[3];
    float f3C;
} ShrubMatrix;

typedef struct ShrubInst {
    float          f0[3];
    float          fC;
    float          f10;
    char           pad14[3];
    unsigned char  f17;
    short          f18;
    unsigned char  f1A;
    unsigned char  f1B;
    unsigned short f1C;
    unsigned short f1E;
} ShrubInst;

typedef struct {
    char           pad0[0xC];
    float          fC;
    char           pad10[6];
    unsigned short f16;
    ShrubInst     *f18;
    float         *f1C;
    float          f20;
} ShrubClass;

typedef struct {
    char           pad0[0x24];
    float          f24;
    char           pad28[0x1C];
    unsigned short f44;
} MobyClass;

typedef struct MobyInst {
    char           pad0[0x10];
    float          f10;
    float          f14;
    float          f18;
    char           pad1C[4];
    unsigned char  f20;
    unsigned char  f21;
    char           pad22[2];
    MobyClass     *f24;
    char           pad28[4];
    float          f2C;
    unsigned char  f30;
    char           pad31;
    unsigned short f32;
    unsigned short f34;
    unsigned short f36;
    int            f38;
    char           pad3C[4];
    float          f40;
    float          f44;
    float          f48;
    char           pad4C[0x28];
    void          *f74;
    int            f78;
    char           pad7C[4];
    int            f80;
    char           pad84[0x22];
    short          fA6;
    char           padA8[8];
    unsigned char  fB0;
    unsigned char  fB1;
    short          fB2;
    short          fB4;
    short          fB6;
    char           padB8[0x48];
} MobyInst;

typedef struct {
    char           pad0[0x36];
    unsigned short f36;
    char           pad38[2];
    unsigned short f3A;
    char           pad3C;
    unsigned char  f3D;
    char           pad3E[2];
} Tfrag;

typedef struct {
    char pad0[0x48];
    int  f48;
    int  f4C;
    char pad50[0x30];
} Light;

typedef struct {
    int f0;
    int pad4[3];
} TextTable;

typedef struct {
    char pad0[0x2C];
    int  f2C;
} TextHeader;

typedef struct {
    float f0;
    float f4;
    float f8;
    int   fC;
    float f10;
    float f14;
    float f18;
    int   f1C;
} Cuboid;

typedef struct {
    char      pad0[0x2080];
    MobyInst *f2080;
    char      pad2084[0x28C];
} HeroSpawn;

typedef struct {
    s32 a, b;
} P2;

typedef struct {
    u8 pad[0x12E8];
    P2 x[10];
    P2 y[1];
} Tab;

extern int D_L00_001600E8     MACRO_ADDR;
extern int D_L00_001600F0     MACRO_ADDR;
extern int D_L00_001600F8     MACRO_ADDR;
extern int D_L00_00160100     MACRO_ADDR;
extern char                  *D_L00_0015EF80;
extern char *D_L00_0015FBC0   MACRO_ADDR;
extern char *D_L00_0015FB80   MACRO_ADDR;
extern int D_L00_0015FB50     MACRO_ADDR;
extern char *D_L00_00160040   MACRO_ADDR;
extern int D_L00_00160014     MACRO_ADDR;
extern int D_L00_0015F63C     MACRO_ADDR;
extern Cuboid *D_L00_0015EF50 MACRO_ADDR;
extern HeroSpawn              D_0013F350;
extern Light                  l00_gameplay_lights[] __asm__("D_L00_0017EEC0");
extern LevelState16CB40       D_L00_0016CA40;
extern LevelLoad173E00        D_L00_00173E00;
/* Level load state at D_L00_00173E40; this function sets its header pointers (+0x4, +0x8) and clears +0xC..+0x14. */
extern LevelLoad173E40          coll_hit __asm__("D_L00_00173E40");
extern MobyInst                *moby_table __asm__("D_L00_0015FFD8") MACRO_ADDR;
extern MobyInst *D_L00_0015FFDC MACRO_ADDR;
extern MobyInst *D_L00_0015FFE0 MACRO_ADDR;

#include "rnc/rendering/level_render_state.h"

extern ShrubClass                 *D_L00_001BC030[];
extern ShrubInst *D_L00_00160494   MACRO_ADDR;
extern ShrubInst *D_L00_00160498   MACRO_ADDR;
extern ShrubMatrix *D_L00_0016049C MACRO_ADDR;
extern LevelSpline                *l00_splines_macro __asm__("D_L00_0015F70C") MACRO_ADDR;
extern TextHeader                  D_L00_00179410;
extern TextTable *D_L00_0015F660   MACRO_ADDR;
extern Tfrag *D_L00_00160F4C       MACRO_ADDR;
extern TieClass                   *D_L00_001C5800[];
extern TieInst *D_L00_00160FC0     MACRO_ADDR;
extern TieInst *D_L00_00160FC4     MACRO_ADDR;
extern TieMatrix *D_L00_00160FC8   MACRO_ADDR;
extern char *D_L00_0015F310        MACRO_ADDR;
extern char                       *D_L00_0015FFE8_macro __asm__("D_L00_0015FFE8") MACRO_ADDR;
extern char *D_L00_001600E4        MACRO_ADDR;
extern char *D_L00_001600EC        MACRO_ADDR;
extern char *D_L00_001600F4        MACRO_ADDR;
extern char *D_L00_001600FC        MACRO_ADDR;
extern char                       *D_L00_00160108_macro __asm__("D_L00_00160108") MACRO_ADDR;
extern char                       *D_L00_0016016C_macro __asm__("D_L00_0016016C") MACRO_ADDR;
extern char *D_L00_0015F318        MACRO_ADDR;
extern char                       *toad_paths[] __asm__("D_L00_001B04B0");
extern char                       *create_moby(int);

#include "rnc/overlay/level_camera.h"

extern char D_L00_0016C780[];
extern char D_L00_0016C7B0[];
extern char D_L00_00173EC0[];
extern char D_L00_00173F00[];
extern char D_L00_0017ECC0[];
extern char level_directional_lights[] __asm__("D_L00_0017FEC0");
extern char D_L00_0018EB40[];
extern char D_L00_0019B7E0[];
extern char D_L00_001AB7E0[];
extern char D_L00_001B1880[];
extern char level_gameplay_directional_light_overflow_message[] __asm__("D_L00_001E8658");
extern char level_gameplay_moby_occlusion_outdated_format[] __asm__("D_L00_001E86A8");
extern char level_gameplay_tfrag_occlusion_outdated_message[] __asm__("D_L00_001E86E0");
extern char level_gameplay_tie_occlusion_outdated_format[] __asm__("D_L00_001E8708");
extern char level_gameplay_no_occlusion_message[] __asm__("D_L00_001E8740");
extern float D_L00_0015F448 MACRO_ADDR;
extern float D_L00_0015F44C MACRO_ADDR;
extern float D_L00_0015F450 MACRO_ADDR;
extern float D_L00_0015F454 MACRO_ADDR;
extern float D_L00_0015F638 MACRO_ADDR;
extern float D_L00_001604A4 MACRO_ADDR;
extern float D_L00_00160F80 MACRO_ADDR;
extern float D_L00_00160FE0 MACRO_ADDR;
extern int D_0015ED84       MACRO_ADDR;
extern int D_L00_0015EF54   MACRO_ADDR;
extern char                *level_root[2] __asm__("D_0015EE4C") MACRO_ADDR;
extern int D_L00_0015F444   MACRO_ADDR;
extern int D_L00_0015F445   MACRO_ADDR;
extern int D_L00_0015F446   MACRO_ADDR;
extern int D_L00_0015F5CC   MACRO_ADDR;
extern int                  D_L00_0015F600;
extern int D_L00_0015F710   MACRO_ADDR;
extern int D_L00_0015FBC4   MACRO_ADDR;
extern int D_L00_0015FFF0   MACRO_ADDR;
extern int                  D_L00_0015FFF4;
extern int D_L00_00160104   MACRO_ADDR;
extern int D_L00_00160170   MACRO_ADDR;
extern int D_L00_00160174   MACRO_ADDR;
extern int D_L00_00160178   MACRO_ADDR;
extern int D_L00_0016017C   MACRO_ADDR;
extern int D_L00_00160490   MACRO_ADDR;
extern int D_L00_00160540   MACRO_ADDR;
extern int                  first_class_ids[] __asm__("D_L00_00160548");
extern int                  shrub_color_buffer_descriptor[] __asm__("D_L00_00160B70");
extern int D_L00_00160F50   MACRO_ADDR;
extern int D_L00_00160FCC   MACRO_ADDR;

#include "rnc/rendering/debug/debug_menu.h"

extern int   D_L00_001AB840[];
extern int   D_L00_001BA4D0[];
extern int   find_or_assign_bolt_source_slot(int, int) __asm__("FUN_L00_00284d90");
extern short gameplay_moby_index_remap[] __asm__("D_L00_001AC780");
extern short D_L00_001B0770[];
extern short D_L00_001B07B0[];
extern short D_L00_001B0830[];
extern u8    D_00100AE0[];
extern unsigned char *D_L00_001604A0 MACRO_ADDR;
extern unsigned char                 mission_state[] __asm__("D_L00_0015FC88") MACRO_ADDR;
extern unsigned char                 D_L00_001804C0[];
extern unsigned char                 D_L00_001BC1B0[];
extern unsigned char                 D_L00_001C5B00[];
extern void                          enable_global_flag(void) __asm__("func_001F61E8");
extern void                          scale_vector(void *, void *, float) __asm__("FUN_001f9a80");
extern void                          FUN_0022a5e0(void *);
extern void                          light_tfrags(void *) __asm__("FUN_00234f98");
extern void                          setup_level_cameras(void) __asm__("FUN_L00_001ed358");
extern void                          initialize_hero(void) __asm__("FUN_L00_00204c60");
extern void                          reset_hero_to_idle(void) __asm__("FUN_L00_00204f80");
extern void store_current_level_table_value(int, int) __asm__("FUN_L00_0024a798");
extern void resolve_lookup_record(short *) __asm__("FUN_L00_0028df58");
extern void update_moby_proximity_interaction() __asm__("FUN_L00_0028e180");
extern void initialize_moby_state_record(char *) __asm__("FUN_L00_0028e8a0");
extern void FillTransferWords(void *, s32, s32);
extern void FlushCache(s32);
extern void PackDmaTag(unsigned long, unsigned long, unsigned long);
extern void PackRenderCommandFields(void *, s32, s32, s32, s32);
extern void reset_draw_globals(void) __asm__("FUN_001f37e8");
extern void set_moby_animation(void *, s32, s32) __asm__("func_00212ED8");
extern void set_video_timing(s32) __asm__("func_00214970");
extern void update_camera(void) __asm__("func_001EDAA8");
extern void update_fog(void) __asm__("func_001F2588");
extern void vu0_load_micro_program(void *) __asm__("func_002334D8");
void        reset_global_state_fields(void) __asm__("func_00217020");
void        srand(int value) __asm__("func_001160C8");
extern void load(void *, int, int) __asm__("FUN_00216828");

char                *load_level_gameplay(int skip_gameplay_read) __asm__("FUN_L00_00241940");
extern unsigned char l00_moby_load_exclusion_flags_by_uid[] __asm__("D_L00_001BB684");
extern f32           frame_scale __asm__("D_0015ED60") MACRO_ADDR;
extern s32           pal_mode __asm__("D_0015ED80") MACRO_ADDR;
extern s32           game_language __asm__("D_0015ED88") MACRO_ADDR;

char *load_level_gameplay(int skip_gameplay_read) {
    LevelHeader *hdr;
    char        *mem;
    float       *ship;
    short       *moby_index_remap =
        gameplay_moby_index_remap; /* moby index by file order; a pointer local, so its address is formed at each use */
    int level_or_group_scratch; /* scratch int: the level number in the moby loop, a group's offset and end flag in the group loop */

    hdr = D_L00_00173E00.f4;
    mem = D_L00_00173E00.f18;
    if (skip_gameplay_read == 0) {
        if (pal_mode != 0) {
            load(mem, disc_table.sound_archive_alt.sector, disc_table.sound_archive_alt.size);
        } else {
            load(mem, disc_table.sound_archive.sector, disc_table.sound_archive.size);
        }
        level_root[1] = mem; /* D_0015EE50 */
    }
    FlushCache(0);
    fast_decompress(level_root[1], hdr);
    FlushCache(0);
    FillTransferWords(&D_0013F350, 0, 0x2310);
    FillTransferWords(&l00_camera, 0, 0x3A0);
    FillTransferWords(D_L00_0018EB40, 0, 0x180);
    srand(0x4D2);
    if (pal_mode != 0) {
        if (frame_scale == 1.0f) {
            set_video_timing(1);
        }
    } else {
        if (frame_scale != 1.0f) {
            set_video_timing(0);
        }
    }
    coll_hit.f4 = D_L00_00173F00;
    coll_hit.f8 = D_L00_00173EC0;
    coll_hit.fC = 0;
    coll_hit.f10 = 0;
    coll_hit.f14 = 0;
    fast_mem_zero16(D_L00_0019B7E0, 0x10000);
    fast_mem_zero16(D_L00_001AB7E0, 0x60);
    {
        int *p = (int *)((char *)hdr + hdr->f0);
        int *ship_spawn_settings;

        D_L00_0016CA40.f23C = *p++;
        D_L00_0016CA40.f240 = *p++;
        D_L00_0016CA40.f244 = *p++;
        *(unsigned char *)&D_L00_0015F444 = *p++;
        *(unsigned char *)&D_L00_0015F445 = *p++;
        *(unsigned char *)&D_L00_0015F446 = *p++;
        ship = (float *)(p + 5);
        ship_spawn_settings = p + 9;
        D_L00_0015F448 = *(float *)p;
        D_L00_0015F44C = *(float *)(p + 1);
        D_L00_0015F450 = *(float *)(p + 2);
        D_L00_0015F454 = *(float *)(p + 3);
        D_L00_0015F638 = *(float *)(p + 4);
        p += 12;
        level_render_state.path = ship_spawn_settings[0];
        level_render_state.source_camera_index = ship_spawn_settings[1];
        level_render_state.destination_camera_index = ship_spawn_settings[2];
    }
    update_fog();
    D_L00_00160F80 = 512000.0f;
    D_L00_00160FE0 = 720.0f;
    D_L00_001604A4 = 500.0f;
    D_L00_0015FFF0 = 500;
    D_L00_0016017C = 0x1F4000;
    PackDmaTag(D_L00_0016CA40.f23C, D_L00_0016CA40.f240, D_L00_0016CA40.f244);
    vu0_load_micro_program(D_00100AE0);
    fast_mem_zero16(point_lights, 0x100);
    fast_mem_zero16(D_L00_001804C0, 0x180);
    fast_mem_zero16(level_directional_lights, 0x400);
    {
        char *p = (char *)hdr + hdr->f4;
        int   n = *(int *)p;

        p += 0x10;
        if (n >= 12) {
            debug_print(level_gameplay_directional_light_overflow_message);
            n = 12;
        }
        if (n != 0) {
            copy_memory_aligned(level_directional_lights, p, n << 6);
        }
    }
    if (hdr->f80 != 0) {
        char *p = (char *)hdr + hdr->f80;

        D_L00_0015FBC4 = *(int *)p;
        p += 0x10;
        if (D_L00_0015FBC4 != 0) {
            Light *current_light;
            int    i;

            copy_memory_aligned(D_L00_0017ECC0, p, D_L00_0015FBC4 << 4);
            copy_memory_aligned(
                l00_gameplay_lights, p + (D_L00_0015FBC4 << 4), D_L00_0015FBC4 << 7);
            for (i = 0; i < D_L00_0015FBC4; i++) {
                current_light = &l00_gameplay_lights[i];
                if (current_light->f48 < 0) {
                    current_light->f48 = 11;
                }
                if (current_light->f4C < 0) {
                    current_light->f4C = 11;
                }
            }
        }
    } else {
        D_L00_0015FBC4 = 0;
    }
    {
        int i;

        for (i = 0; i < D_L00_00160F50; i++) {
            D_L00_00160F4C[i].f36 = 0xFFFF;
        }
    }
    {
        int i = 0;

        while (i < D_L00_00160F50) {
            unsigned short *q = (unsigned short *)0x70003000;
            int             j;

            for (j = 0; j < 0x3FF && i < D_L00_00160F50; j++) {
                *q = i;
                q++;
                i++;
            }
            *q = 0xFFFF;
            light_tfrags((void *)0x70003000);
        }
    }
    if (hdr->f10[0] != 0) {
        int   lang = game_language * 4;
        char *p = (char *)hdr + *(int *)((char *)hdr + lang + 0x10);
        int   size;
        int   i;

        D_L00_00179410.f2C = *(int *)p;
        p += 4;
        size = *(int *)p;
        p += 4;
        D_L00_0015F660 = (TextTable *)mem;
        size += 3;
        size &= ~3;
        size -= 8;
        copy_memory_aligned(mem, p, size);
        for (i = 0; i < D_L00_00179410.f2C; i++) {
            int delta = (int)D_L00_0015F660 - 8;

            D_L00_0015F660[i].f0 += delta;
        }
        mem = (char *)(((u32)mem + size + 0x3F) & 0xFFFFFFC0);
    }
    {
        char *p = (char *)hdr + hdr->f34;
        int   n = *(int *)p;
        int   i;
        int   last;

        D_L00_00160FC0 = (TieInst *)mem;
        p += 0x10;
        D_L00_00160FCC = n;
        mem += n << 5;
        if (n != 0) {
            FillTransferWords(D_L00_00160FC0, 0, n << 5);
        }
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        D_L00_00160FC8 = (TieMatrix *)mem;
        mem += D_L00_00160FCC * 0x1C0;
        if (D_L00_00160FCC != 0) {
            FillTransferWords(D_L00_00160FC8, 0, D_L00_00160FCC * 0x1C0);
        }
        last = -1;
        D_L00_00160FC4 = D_L00_00160FC0 + D_L00_00160FCC;
        for (i = 0; i < D_L00_00160FCC; i++) {
            TieInst   *inst = D_L00_00160FC0 + i;
            TieMatrix *mtx = D_L00_00160FC8 + i;
            int        id;
            float      scale;

            inst->f1A = D_L00_001C5B00[*(int *)p];
            if (inst->f1A != last) {
                D_L00_001C5800[inst->f1A]->f28 = inst;
                D_L00_001C5800[inst->f1A]->f26 = 0;
                last = inst->f1A;
            }
            D_L00_001C5800[inst->f1A]->f26++;
            inst->f10 = mtx;
            inst->f14 = (float)*(int *)(p + 4);
            inst->f1E = 0xFFFF;
            inst->f1B = 0;
            inst->f1C = 0;
            id = *(int *)(p + 0xC);
            qcopy(mtx, p + 0x10);
            qcopy(mtx->m1, p + 0x20);
            qcopy(mtx->m2, p + 0x30);
            qcopy(mtx->m3, p + 0x40);
            inst->f18 = id;
            p += 0x50;
            mtx->f3C = D_L00_001C5800[inst->f1A]->f40;
            transform_vector_by_basis(inst, D_L00_001C5800[inst->f1A]->f30, mtx);
            scale = fast_vec_length(mtx);
            scale = max_float(scale, fast_vec_length(mtx->m1));
            inst->fC = D_L00_001C5800[inst->f1A]->f3C * max_float(scale, fast_vec_length(mtx->m2));
            scale_vector(inst, inst, mtx->f3C);
            fast_vec_add(inst, inst, mtx->m3);
            D_L00_00160FC8[i].fC = 1.0f / fast_vec_length(&D_L00_00160FC8[i]);
            D_L00_00160FC8[i].f1C = 1.0f / fast_vec_length(D_L00_00160FC8[i].m1);
            D_L00_00160FC8[i].f2C = 1.0f / fast_vec_length(D_L00_00160FC8[i].m2);
            copy_memory_aligned(mtx->f140, p, 0x80);
            p += 0x80;
            inst->f1C = *(unsigned short *)p;
            p += 0x10;
        }
    }
    {
        unsigned short *q = (unsigned short *)0x70000000;
        int             i;

        for (i = 0; i < D_L00_00160FCC; i++) {
            *q++ = i;
        }
        *q = 0xFFFF;
        light_ties((void *)0x70000000);
    }
    {
        char *p = (char *)hdr + hdr->f3C;
        int   n = *(int *)p;
        int   last;
        int   i;

        D_L00_00160494 = (ShrubInst *)mem;
        p += 0x10;
        D_L00_00160490 = n;
        mem += n << 5;
        if (n != 0) {
            FillTransferWords(D_L00_00160494, 0, n << 5);
        }
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        D_L00_0016049C = (ShrubMatrix *)mem;
        mem += D_L00_00160490 << 6;
        if (D_L00_00160490 != 0) {
            FillTransferWords(D_L00_0016049C, 0, D_L00_00160490 << 6);
        }
        D_L00_001604A0 = (unsigned char *)mem;
        mem += D_L00_00160490 * 0x60;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (D_L00_00160490 != 0) {
            FillTransferWords(D_L00_001604A0, 0, D_L00_00160490 * 0x60);
        }
        last = -1;
        shrub_color_buffer_descriptor[1] = (int)D_L00_001604A0;
        D_L00_00160498 = D_L00_00160494 + D_L00_00160490;
        for (i = 0; i < D_L00_00160490; i++) {
            ShrubInst   *inst = D_L00_00160494 + i;
            ShrubMatrix *mtx = D_L00_0016049C + i;
            int          shrub_class_index = D_L00_001BC1B0[*(int *)p];
            float        scale;

            inst->f1A = shrub_class_index;
            if (shrub_class_index != last) {
                last = shrub_class_index;
                D_L00_001BC030[shrub_class_index]->f18 = inst;
                D_L00_001BC030[shrub_class_index]->f16 = 0;
            }
            D_L00_001BC030[shrub_class_index]->f16++;
            inst->f18 = i;
            inst->f10 = *(float *)(p + 4);
            if (inst->f10 < 16.0f) {
                inst->f10 = 16.0f;
            }
            inst->f1B = 0;
            inst->f1E = 0xFFFF;
            if (D_L00_001BC030[shrub_class_index]->f1C != 0) {
                float shrub_class_lower_bound;

                inst->f17 = truncate_float_to_s32(*D_L00_001BC030[shrub_class_index]->f1C);
                shrub_class_lower_bound = convert_integer_to_float(inst->f17) + 24.0f;
                if (inst->f10 < shrub_class_lower_bound) {
                    inst->f10 = shrub_class_lower_bound;
                }
            }
            qcopy(mtx, p + 0x10);
            qcopy(mtx->m1, p + 0x20);
            qcopy(mtx->m2, p + 0x30);
            qcopy(mtx->m3, p + 0x40);
            p += 0x50;
            mtx->f3C = D_L00_001BC030[shrub_class_index]->f20;
            {
                float a = (fast_vec_length(mtx) + fast_vec_length(mtx->m1)) * 0.5f;
                float b = fast_vec_length(mtx->m2);
                int   ia = truncate_float_to_s32(a * 4096.0f);
                int   ib = truncate_float_to_s32(b * 4096.0f);

                if (ib > 0x10000) {
                    ib = 0x10000;
                }
                if (ia > 0x10000) {
                    ia = 0x10000;
                }
                mtx->f2C = ia | (ib << 16);
            }
            {
                int r;
                int g;
                int b;

                r = *(int *)p;
                p += 4;
                g = *(int *)p;
                p += 4;
                b = *(int *)p;
                p += 8;
                g = (g << 8) | 0x80000000;
                mtx->fC = (b << 16) | g | r;
            }
            inst->f1C = *(unsigned short *)p;
            p += 0x10;
            transform_vector_by_basis(inst, D_L00_001BC030[shrub_class_index], mtx);
            scale = fast_vec_length(mtx);
            scale = max_float(scale, fast_vec_length(mtx->m1));
            inst->fC =
                D_L00_001BC030[shrub_class_index]->fC * max_float(scale, fast_vec_length(mtx->m2));
            scale_vector(inst, inst, mtx->f3C);
            fast_vec_add(inst, inst, mtx->m3);
        }
    }
    {
        unsigned short *q = (unsigned short *)0x70000000;
        int             i;

        for (i = 0; i < D_L00_00160490; i++) {
            *q++ = i;
        }
        *q = 0xFFFF;
        FUN_0022a5e0((void *)0x70000000);
    }
    {
        int i;

        for (i = 0; i < D_L00_00160490; i++) {
            unsigned char *q = D_L00_001604A0 + i * 0x60;
            int            r = 0;
            int            g = 0;
            int            b = 0;
            int            n = 24;
            int            j;

            for (j = 0; j < n; j++) {
                r += q[0];
                g += q[1];
                b += q[2];
                q += 4;
            }
            g /= n;
            b /= n;
            r /= n;
            *(int *)((char *)D_L00_0016049C + i * 0x40 + 0x1C) = (b << 16) | (g << 8) | r;
        }
    }
    if (hdr->f8 != 0) {
        char *p = (char *)hdr + hdr->f8;
        int   n = *(int *)p;
        int   i;

        D_L00_0015EF50 = (Cuboid *)mem;
        p += 0x10;
        D_L00_0015EF54 = n;
        mem += n << 5;
        if (n != 0) {
            FillTransferWords(D_L00_0015EF50, 0, n << 5);
        }
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        for (i = 0; i < D_L00_0015EF54; i++) {
            D_L00_0015EF50[i].fC = *(int *)p;
            p += 4;
            D_L00_0015EF50[i].f0 = ((float *)p)[0];
            D_L00_0015EF50[i].f4 = ((float *)p)[1];
            D_L00_0015EF50[i].f8 = ((float *)p)[2];
            p += 0xC;
            D_L00_0015EF50[i].f10 = ((float *)p)[0];
            D_L00_0015EF50[i].f14 = ((float *)p)[1];
            D_L00_0015EF50[i].f18 = ((float *)p)[2];
            p += 0xC;
            D_L00_0015EF50[i].f1C = *(int *)p;
            p += 4;
        }
    }
    if (hdr->fC != 0) {
        char *p = (char *)hdr + hdr->fC;
        int   n = *(int *)p;

        p += 0x10;
        voice_pool.sound_instance_count = n;
        if (n != 0) {
            int size = n * 0x90;
            int i;

            voice_pool.sound_instances = (SoundInstance *)mem;
            mem += size;
            mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
            FillTransferWords(voice_pool.sound_instances, 0, size);
            for (i = 0; i < voice_pool.sound_instance_count; i++) {
                SoundInstance *e = voice_pool.sound_instances + i;

                copy_memory_aligned(e, p, 0x90);
                p += 0x90;
                resolve_lookup_record((short *)e);
            }
        } else {
            voice_pool.sound_instances = 0;
        }
    }
    {
        char *p = (char *)hdr + hdr->f44;
        int   n2;
        int   count = 0;
        int   n;
        char *next;
        int   i;

        n = *(int *)p;
        p += 4;
        n2 = *(int *)p;
        p += 0xC;
        moby_table = (MobyInst *)mem;
        FillTransferWords(mem, 0, (n + n2) << 8);
        for (i = 0; i < n; i++) {
            int           mission;
            int           flags;
            int           uid;
            int           bolts;
            int           bolts2;
            int           selected_bolt_count;
            int           adjusted_bolt_count;
            unsigned char vis = 0xFE;
            int           skip = 0;

            next = p + ((*(int *)p >> 2) << 2);
            p += 4;
            mission = *(int *)p;
            p += 4;
            flags = *(int *)p;
            p += 4;
            uid = *(int *)p;
            p += 4;
            bolts = *(int *)p;
            p += 4;
            bolts2 = *(int *)p;
            p += 4;
            selected_bolt_count = bolts;
            adjusted_bolt_count = selected_bolt_count;
            if (flags != 0) {
                level_or_group_scratch = D_0015ED84; /* read before the test: see the group loop */
                if (flags & 0x10) {
                    vis = find_or_assign_bolt_source_slot(level_or_group_scratch, uid);
                } else {
                    vis = 0xFF;
                }
                if (l00_moby_load_exclusion_flags_by_uid[uid] != 0) {
                    skip = 1;
                } else if (flags & 3) {
                    if (mission_state[mission] == 0xFF) {
                        if (flags & 2) {
                            selected_bolt_count = bolts2;
                            if ((D_L00_001BA4D0[uid >> 5] >> (uid & 0x1F)) & 1) {
                                adjusted_bolt_count = (selected_bolt_count + 1) / 2;
                            } else {
                                adjusted_bolt_count = selected_bolt_count;
                            }
                        } else {
                            skip = 1;
                        }
                    } else {
                        if (flags & 1) {
                            if ((D_0014C190_257218[D_0015ED84][uid >> 5] >> (uid & 0x1F)) & 1) {
                                adjusted_bolt_count = (bolts + 1) / 2;
                            }
                        } else {
                            skip = 1;
                        }
                    }
                } else if (flags & 8) {
                    if ((D_L00_001BA4D0[uid >> 5] >> (uid & 0x1F)) & 1) {
                        skip = 1;
                    }
                } else if ((flags & 0xC) == 4) {
                    if ((D_0014C190_257218[D_0015ED84][uid >> 5] >> (uid & 0x1F)) & 1) {
                        skip = 1;
                    }
                }
            }
            if (skip) {
                moby_index_remap[i] = -1;
            } else {
                MobyInst *m;
                int       a;
                float     ground_height_offset;
                int       pv;
                int       moby_red;
                int       moby_green;
                int       moby_blue;
                int       upd;

                {
                    short *slot = moby_index_remap + i;
                    *slot = count;
                }
                m = moby_table + count;
                init_moby_instance(m, *(int *)p);
                p += 4;
                m->fB1 = vis;
                m->fB2 = uid;
                m->fB0 = mission;
                m->fB4 = adjusted_bolt_count;
                m->fB6 = selected_bolt_count;
                if (m->f24 != 0) {
                    m->f2C = m->f24->f24 * *(float *)p;
                }
                p += 4;
                m->f32 = *(unsigned short *)p;
                p += 4;
                m->f30 = *(unsigned char *)p;
                p += 0xC;
                m->f10 = ((float *)p)[0];
                m->f14 = ((float *)p)[1];
                m->f18 = ((float *)p)[2];
                p += 0xC;
                m->f40 = ((float *)p)[0];
                m->f44 = ((float *)p)[1];
                m->f48 = ((float *)p)[2];
                p += 0xC;
                m->f21 = *(unsigned char *)p;
                p += 4;
                a = *(int *)p;
                p += 4;
                ground_height_offset = *(float *)p;
                p += 4;
                if (a != 0) {
                    float z = probe_ground_height(&m->f10, 0, 0.5f);

                    if (0.0f < z) {
                        m->f18 = z + ground_height_offset;
                    }
                }
                p += 4;
                pv = *(int *)p;
                m->f36 = 0x7F80;
                p += 4;
                m->f78 = pv;
                if (*(int *)p == 0) {
                    m->f36 = 0;
                }
                p += 4;
                if (m->f24 != 0) {
                    m->f24->f44 |= *(unsigned short *)p;
                }
                m->f34 |= *(unsigned short *)p;
                p += 4;
                if (m->f24 != 0) {
                    update_entity(m);
                }
                moby_red = *(int *)p;
                p += 4;
                moby_green = *(int *)p;
                p += 4;
                moby_blue = *(int *)p;
                p += 4;
                upd = *(int *)p;
                p += 4;
                m->f80 = (moby_blue << 16) + (moby_green << 8) + moby_red;
                PackRenderCommandFields(m, m->f80, 0, 0, 0);
                m->f38 = upd;
                if (*(int *)p != -1) {
                    store_current_level_table_value((int)m, *(int *)p);
                }
                count++;
            }
            p = next;
        }
        D_L00_0015FFDC = moby_table + count;
        D_L00_0015FFDC->f20 = 0xFF;
        mem += (count + n2) << 8;
        D_L00_0015FFE8_macro = mem;
        mem += n2 << 7;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        D_L00_0015FFE0 = (MobyInst *)((int)moby_table + ((count + n2) << 8) - 0x100);
    }
    if (hdr->f58 != 0) {
        char      *pv = (char *)hdr + hdr->f58;
        PvarEntry *tbl = (PvarEntry *)((char *)hdr + hdr->f54);
        int        i;
        MobyInst  *m;

        for (i = 0; i < D_L00_0015EF54; i++) {
            int k = D_L00_0015EF50[i].f1C;

            if (k < 0) {
                D_L00_0015EF50[i].f1C = 0;
            } else {
                int   size = tbl[k].f4;
                char *src = pv + tbl[k].f0;

                D_L00_0015EF50[i].f1C = (int)mem;
                fast_mem_copy(mem, src, size);
                tbl[k].f0 = (int)mem;
                mem += size;
            }
        }
        for (i = 0; i < voice_pool.sound_instance_count; i++) {
            int k = voice_pool.sound_instances[i].data;

            if (k < 0) {
                voice_pool.sound_instances[i].data = 0;
            } else {
                int   size = tbl[k].f4;
                char *src = pv + tbl[k].f0;

                voice_pool.sound_instances[i].data = (int)mem;
                fast_mem_copy(mem, src, size);
                tbl[k].f0 = (int)mem;
                mem += size;
            }
        }
        for (m = moby_table; m != D_L00_0015FFDC; m++) {
            int k = m->f78;

            if (k < 0) {
                m->f78 = 0;
            } else {
                int   size = tbl[k].f4;
                char *src = pv + tbl[k].f0;

                m->f78 = (int)mem;
                fast_mem_copy(mem, src, size);
                tbl[k].f0 = (int)mem;
                mem += size;
            }
        }
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
    }
    if (hdr->f50 != 0) {
        int       *q = (int *)((char *)hdr + hdr->f50);
        PvarEntry *tbl = (PvarEntry *)((char *)hdr + hdr->f54);

        while (q[0] >= 0) {
            int x = tbl[q[0]].f0;
            int off = q[1];

            if (x >= (int)hdr) {
                int *r = (int *)(x + off);

                if (*r >= 0) {
                    *r = moby_index_remap[*r];
                }
            }
            q += 2;
        }
    }
    if (hdr->f5C != 0) {
        int       *q = (int *)((char *)hdr + hdr->f5C);
        PvarEntry *tbl = (PvarEntry *)((char *)hdr + hdr->f54);

        while (q[0] >= 0) {
            int x = tbl[q[0]].f0;
            int off = q[1];

            if (x >= (int)hdr) {
                int *r = (int *)(x + off);

                *r += x;
            }
            q += 2;
        }
    }
    if (hdr->f48 != 0) {
        char *q = (char *)hdr + hdr->f48;
        char *base = mem;
        int  *offs;
        int   n;
        int   size;
        int   i;

        n = *(int *)q;
        q += 4;
        size = *(int *)q;
        offs = (int *)(q + 0xC);
        mem += size;
        q = (char *)offs + (n << 2);
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (size != 0) {
            fast_mem_copy(base, q, size);
        }
        FillTransferWords(D_L00_001AB840, 0, 0x1C0);
        for (i = 0; i < n; i++, offs++) {
            level_or_group_scratch = *offs;
            if (level_or_group_scratch >= 0) {
                int             g = (int)(base + level_or_group_scratch);
                unsigned short *src;
                short          *group_write_cursor;

                D_L00_001AB840[i] = g;
                src = (unsigned short *)g;
                group_write_cursor = (short *)src;
                D_L00_0015FFF4 = i + 1;
                do {
                    int   id = *src;
                    short t;

                    level_or_group_scratch = *(short *)src;
                    g = (id & 0x7FFF) << 1; /* the entry's slot in the moby index, in two steps */
                    g += (int)moby_index_remap;
                    level_or_group_scratch &= 0x8000;
                    t = *(short *)g;
                    if (t >= 0) {
                        *group_write_cursor++ = t;
                    }
                    if (level_or_group_scratch) {
                        if (D_L00_001AB840[i] < (int)group_write_cursor) {
                            group_write_cursor[-1] |= 0x8000;
                        } else {
                            D_L00_001AB840[i] = 0;
                        }
                        break;
                    }
                    src++;
                } while (!level_or_group_scratch);
            }
        }
    }
    if (hdr->f4C != 0) {
        char      *q = (char *)hdr + hdr->f4C;
        char      *base = mem;
        PvarEntry *tbl;
        int        size;
        int        n;
        int        i;

        size = *(int *)q;
        q += 4;
        n = *(int *)q;
        q += 0xC;
        mem += size;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (size != 0) {
            fast_mem_copy(base, q, size);
        }
        q += size;
        tbl = (PvarEntry *)((char *)hdr + hdr->f54);
        for (i = 0; i < n; i++) {
            int w = *(int *)q;
            int hi = w >> 16;
            int x = tbl[w & 0xFFFF].f0;

            if (x >= (int)hdr) {
                *(int *)(x + hi) = (int)(base + *(int *)(q + 4));
            }
            q += 8;
        }
    }
    if (hdr->f70 != 0) {
        char *q = (char *)hdr + hdr->f70;
        char *src;
        int   size;
        int   i;

        D_L00_00160104 = *(int *)q;
        src = q + *(int *)(q + 4);
        size = *(int *)(q + 8);
        D_L00_00160108_macro = mem;
        mem += size;
        q += 0x10;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (size != 0) {
            fast_mem_copy(D_L00_00160108_macro, src, size);
        }
        for (i = 0; i < D_L00_00160104; i++) {
            toad_paths[i] = D_L00_00160108_macro + *(int *)q;
            q += 4;
        }
    }
    if (hdr->f74 != 0) {
        char *q = (char *)hdr + hdr->f74;
        char *src;
        char *base;
        int   n;
        int   size;
        int   i;

        n = *(int *)q;
        D_L00_0015F710 = n;
        src = q + *(int *)(q + 4);
        size = *(int *)(q + 8);
        l00_splines_macro = (LevelSpline *)mem;
        mem += n << 5;
        q += 0x10;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (n > 0) {
            LevelSpline *e = l00_splines_macro;

            i = n;
            do {
                qcopy(e, q);
                q += 0x10;
                e->kind = *(int *)q;
                q += 0x10;
                e++;
            } while (--i != 0);
        }
        base = mem;
        mem += size;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (size != 0) {
            fast_mem_copy(base, src, size);
        }
        for (i = 0; i < D_L00_0015F710; i++) {
            l00_splines_macro[i].path = (LevelPath *)(base + *(int *)q);
            q += 4;
        }
    }
    if (hdr->f6C != 0) {
        char *q = (char *)hdr + hdr->f6C;
        int   n = *(int *)q;
        int   i;

        D_L00_001600E4 = mem;
        D_L00_001600E8 = n;
        q += 0x10;
        mem += n * 0x90;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (n != 0) {
            FillTransferWords(D_L00_001600E4, 0, n * 0x90);
        }
        for (i = 0; i < D_L00_001600E8; i++) {
            copy_memory_aligned(D_L00_001600E4 + i * 0x90, q, 0x90);
            q += 0x80;
        }
    }
    if (hdr->f60 != 0) {
        char *q = (char *)hdr + hdr->f60;
        int   n = *(int *)q;
        int   i;

        D_L00_001600EC = mem;
        q += 0x10;
        D_L00_001600F0 = n;
        mem += n << 7;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (n != 0) {
            FillTransferWords(D_L00_001600EC, 0, n << 7);
        }
        for (i = 0; i < D_L00_001600F0; i++) {
            copy_memory_aligned(D_L00_001600EC + (i << 7), q, 0x80);
            q += 0x80;
        }
    }
    if (hdr->f64 != 0) {
        char *q = (char *)hdr + hdr->f64;
        int   n = *(int *)q;
        int   i;

        D_L00_001600F4 = mem;
        q += 0x10;
        D_L00_001600F8 = n;
        mem += n << 7;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (n != 0) {
            FillTransferWords(D_L00_001600F4, 0, n << 7);
        }
        for (i = 0; i < D_L00_001600F8; i++) {
            copy_memory_aligned(D_L00_001600F4 + (i << 7), q, 0x80);
            q += 0x80;
        }
    }
    if (hdr->f68 != 0) {
        char *q = (char *)hdr + hdr->f68;
        int   n = *(int *)q;
        int   i;

        D_L00_001600FC = mem;
        q += 0x10;
        D_L00_00160100 = n;
        mem += n << 7;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (n != 0) {
            FillTransferWords(D_L00_001600FC, 0, n << 7);
        }
        for (i = 0; i < D_L00_00160100; i++) {
            copy_memory_aligned(D_L00_001600FC + (i << 7), q, 0x80);
            q += 0x80;
        }
    }
    if (hdr->f84 != 0) {
        char *q = (char *)hdr + hdr->f84;
        int   size = *(int *)q;

        D_L00_0015EF80 = mem;
        q += 0x10;
        size += 0x4000;
        mem += size;
        copy_memory_aligned(D_L00_0015EF80, q, size);
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
    }
    if (hdr->f78 != 0) {
        char *q = (char *)hdr + hdr->f78;
        int   size = *(int *)q;

        D_L00_0015FBC0 = mem;
        q += 0x10;
        size += 0x4000;
        mem += size;
        copy_memory_aligned(D_L00_0015FBC0, q, size);
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
    }
    if (hdr->f7C != 0) {
        char *q = (char *)hdr + hdr->f7C;
        int   n = *(int *)q;

        D_L00_0015FB80 = mem;
        q += 0x10;
        D_L00_0015FB50 = n;
        mem += n << 5;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (n != 0) {
            copy_memory_aligned(D_L00_0015FB80, q, n << 5);
        }
    }
    if (hdr->f88 != 0) {
        char *q = (char *)hdr + hdr->f88;
        int   n = *(int *)q;

        D_L00_00160040 = mem;
        D_L00_00160014 = n;
        q += 0x10;
        mem += n * 0x30;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (n != 0) {
            copy_memory_aligned(D_L00_00160040, q, n * 0x30);
        }
    }
    D_L00_0016016C_macro = mem;
    mem += 0x20000;
    D_L00_00160174 = -1;
    D_L00_00160170 = 0;
    D_L00_00160178 = 0;
    fast_mem_zero16(D_L00_001B1880, 0x200);
    level_render_state.player = 0;
    if (0.0f < ship[0]) {
        MobyInst *m;

        if (D_0015ED84 == 13 && level_render_state.content_variant == 2) {
            if (D_L00_00160540 != 0) {
                for (m = moby_table; m->f20 != 0xFF; m++) {
                    if (m->fA6 == first_class_ids[level_render_state.content_variant]) {
                        initialize_moby_state_record((char *)m);
                        set_moby_animation(m, 1, 0);
                        break;
                    }
                }
            }
        }
        if (level_render_state.player == 0) {
            float ship_spawn_rotation;

            m = create_moby(first_class_ids[level_render_state.content_variant]);
            m->f10 = ship[0];
            m->f14 = ship[1];
            m->f18 = ship[2];
            ship_spawn_rotation = ship[3];
            m->f74 = update_moby_proximity_interaction;
            m->f32 = 0xFF;
            m->f48 = ship_spawn_rotation;
            m->f30 = 0x10;
            m->f34 &= ~2;
            set_moby_animation(m, 1, 0);
            update_entity(m);
            level_render_state.player = (struct ResidentRenderObject *)m;
            store_current_level_table_value((int)m, 0);
        }
    }
    if (D_L00_0015F600 != 0 && hdr->f8C != 0) {
        int *q = (int *)((char *)hdr + hdr->f8C);
        int *object_mapping_header;
        int  tfrag_mapping_count;
        int  n2;
        int  moby_mapping_count;
        int  bad1 = 1;
        int  bad2;

        object_mapping_header = q;
        tfrag_mapping_count = q[0];
        n2 = q[1];
        moby_mapping_count = q[2];
        q += 4;
        if (D_L00_00160F50 == tfrag_mapping_count) {
            int i;

            bad1 = 0;
            for (i = 0; i < tfrag_mapping_count; i++) {
                if (D_L00_00160F4C[i].f3D != q[1]) {
                    bad1 = 1;
                }
                q += 2;
            }
        } else {
            q += tfrag_mapping_count * 2;
        }
        bad2 = 1;
        if (D_L00_00160FC4 - D_L00_00160FC0 == n2) {
            int i;

            bad2 = 0;
            for (i = 0; i < n2; i++) {
                int id = (unsigned short)q[1];

                if (D_L00_00160FC0[i].f18 != id) {
                    bad2 = 1;
                }
                q += 2;
            }
        } else {
            q += n2 * 2;
        }
        {
            MobyInst *m;
            int       flag = 0;
            int       cnt = 0;
            int       k = 0;

            for (m = moby_table; m->f20 != 0xFF; m++) {
                if (m->f36 != 0x7F80) {
                    int uid = m->fB2;
                    int j;

                    k++;
                    for (j = 0; j < moby_mapping_count; j++) {
                        if (q[j * 2 + 1] == uid) {
                            break;
                        }
                    }
                    if (j == moby_mapping_count) {
                        m->f36 = 0x7F80;
                        flag = 1;
                        cnt++;
                    } else {
                        int w = q[j * 2];

                        m->f36 = ((w >> 3) << 8) | (1 << (w & 7));
                    }
                }
            }
            if (flag) {
                debug_print(level_gameplay_moby_occlusion_outdated_format, cnt, k);
            }
        }
        if (bad1) {
            int i;

            debug_print(level_gameplay_tfrag_occlusion_outdated_message);
            for (i = 0; i < D_L00_00160F50; i++) {
                D_L00_00160F4C[i].f3A = 0x7F80;
            }
        } else {
            int i;

            q = object_mapping_header + 4;
            for (i = 0; i < tfrag_mapping_count; i++) {
                int w = q[0];

                D_L00_00160F4C[i].f3A = ((w >> 3) << 8) | (1 << (w & 7));
                q += 2;
            }
        }
        if (bad2) {
            TieInst *t;
            int      cnt = 0;
            int      k = 0;

            q = &object_mapping_header[tfrag_mapping_count * 2 + 4];
            for (t = D_L00_00160FC0; t != D_L00_00160FC4; t++) {
                int id = t->f18;
                int j;

                k++;
                for (j = 0; j < n2; j++) {
                    if (q[j * 2 + 1] == id) {
                        break;
                    }
                }
                if (j == n2) {
                    t->f18 = 0x7F80;
                    cnt++;
                } else {
                    int w = q[j * 2];

                    t->f18 = ((w >> 3) << 8) | (1 << (w & 7));
                }
            }
            debug_print(level_gameplay_tie_occlusion_outdated_format, cnt, k);
        } else {
            int i;

            q = &object_mapping_header[tfrag_mapping_count * 2 + 4];
            for (i = 0; i < n2; i++) {
                int w = q[0];

                D_L00_00160FC0[i].f18 = ((w >> 3) << 8) | (1 << (w & 7));
                q += 2;
            }
        }
        g_debug_menu.occlusion_mode = 2;
    } else {
        MobyInst *m;
        TieInst  *t;
        int       i;

        debug_print(level_gameplay_no_occlusion_message);
        for (m = moby_table; m != D_L00_0015FFE0; m++) {
            m->f36 = 0x7F80;
        }
        for (t = D_L00_00160FC0; t != D_L00_00160FC4; t++) {
            t->f18 = 0x7F80;
        }
        for (i = 0; i < D_L00_00160F50; i++) {
            D_L00_00160F4C[i].f3A = 0x7F80;
        }
        g_debug_menu.occlusion_mode = 0;
    }
    reset_draw_globals();
    initialize_hero();
    reset_hero_to_idle();
    setup_level_cameras();
    update_camera();
    reset_global_state_fields();
    enable_global_flag();
    D_L00_001B0770[0] = 0;
    D_L00_001B07B0[0] = 0;
    D_L00_001B0830[0] = 0;
    D_L00_0015F5CC = 0;
    D_L00_0015F63C = 0;
    if (level_render_state.player != 0) {
        *(long *)&((MobyInst *)level_render_state.player)->f38 = *(long *)&D_0013F350.f2080->f38;
    }
    switch (D_0015ED84) {
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
    case 10:
        D_L00_0015F318 = D_L00_0016C7B0;
        break;
    default:
        D_L00_0015F318 = D_L00_0016C780;
        break;
    }
    D_L00_0015F310 = D_L00_0015F318;
    return mem;
}

typedef struct {
    char *f0; /* 0x00 */
    s32  *f4; /* 0x04: level header */
    char  pad8[4];
    char *fC;  /* 0x0C */
    char *f10; /* 0x10 */
    char *f14; /* 0x14: base the header's offsets are taken from */
    char *f18; /* 0x18 */
    char *f1C; /* 0x1C */
} Load244110;

typedef struct {
    u8  pad0[0x1A];
    s16 sound; /* 0x1A */
    s32 time;  /* 0x1C */
} Slot244110;

typedef struct {
    u8          pad0[0xC];
    u8          seq_count; /* 0x0C */
    u8          count0D;   /* 0x0D */
    u8          pad0E[0x1A];
    Slot244110 *f28; /* 0x28 */
    u8          pad2C[0x1C];
    char       *seqs[1]; /* 0x48 */
} Class244110;

typedef struct {
    s16 off;
    s16 count;
} Span244110;

extern Load244110 D_L00_00173E00_244110 __asm__("D_L00_00173E00");
extern char      *D_L00_00173E40_244110[] __asm__("D_L00_00173E40");

typedef struct {
    s16  a;
    s16  b;
    s32 *file;
} Ldg244110;

extern Ldg244110 D_0015EE48;
extern u8        D_0014C050[];
extern u8        mission_state_macro[] __asm__("D_L00_0015FC88") MACRO_ADDR;
extern s32       D_L00_0015F40C;
extern s32       sound_bank_handle __asm__("D_0015ED5C");

#include "rnc/overlay/shared/world/load_level_gameplay_globals.h"

extern u64          level_texture_128x128_descriptor[3] __asm__("D_L00_001827C0");
extern u64          level_texture_64x64_descriptor[3] __asm__("D_L00_001827D8");
extern u64          D_L00_001827F0[3];
extern s32          tfrag_packed_texture_refs[] __asm__("D_L00_001C4A00");
extern s32          texture_count __asm__("D_L00_0015FFC8");
extern u32          D_L00_00199A80[];
extern s32          tie_packed_texture_refs[] __asm__("D_L00_001C6700");
extern s32          shrub_packed_texture_refs[] __asm__("D_L00_001BC5B0");
extern char        *D_L00_0015F600_244110 __asm__("D_L00_0015F600");
extern s32          D_L00_0016051C;
extern Class244110 *D_L00_00197300[];
extern s32          D_L00_0016000C;
extern s32          D_L00_00160010;
extern s32          moby_reference_class_ids[] __asm__("D_L00_001AFBC0");
extern char        *moby_reference_data_bases[] __asm__("D_L00_001AFC20");
extern s32          D_L00_001AFC80[];
typedef int         Q244110 __attribute__((mode(TI)));
extern Q244110      l00_moby_reference_texture_slot_maps[] __asm__("D_L00_001AFCE0");
extern u8           D_L00_00197BC0[];
extern char        *effect_texture_data_base __asm__("D_L00_0015F420");
extern s32          D_L00_0015F438;
extern u32          D_L00_0015F43C;
extern u32          D_L00_0015F440;
extern s32          D_L00_0015FC98;
extern s32          D_L00_0015FC9C;
extern s32          D_L00_0015FCA0;
extern s32          D_L00_0015FCA4;
extern s32          D_L00_0015FCA8;
extern char        *D_L00_0015F604;
extern Slot244110  *D_L00_0015F5F4;
extern s32          D_L00_0015F5F0;
extern s16          D_L00_00197A00[];
extern u16          moby_reference_sound_ids[][16] __asm__("D_L00_001AFE60");
extern char         D_L00_001E8780[], D_L00_001E87A0[], D_L00_001E87D0[], D_L00_001E87E8[],
    D_L00_001E8800[], D_L00_001E8818[], D_L00_001E8830[], D_L00_001E8848[], D_L00_001E8860[],
    D_L00_001E8878[], D_L00_001E8890[], D_L00_001E88B0[], D_L00_001E88C8[], D_L00_001E88E0[],
    D_L00_001E88F8[], D_L00_001E8910[], D_L00_001E8928[], D_L00_001E8940[], D_L00_001E8958[],
    D_L00_001E8970[], D_L00_001E8988[], D_L00_001E89A0[], D_L00_001E89B8[], D_L00_001E89D0[],
    D_L00_001E89E8[], D_L00_001E8A00[], D_L00_001E8A18[], D_L00_001E8A30[], D_L00_001E8A48[];
extern char  level_memory_map_newline[] __asm__("D_L00_0015FC80");
extern char  level_code_memory_end[] __asm__("D_L00_002F0CD0");
extern s32   particle_texture_ranges[] __asm__("D_L00_001B1A80");
extern char *D_L00_001C5800_244110[] __asm__("D_L00_001C5800");
extern char *D_L00_001BC030_244110[] __asm__("D_L00_001BC030");
extern char *D_L00_00160FC0_244110 __asm__("D_L00_00160FC0");
extern char *D_L00_00160494_244110 __asm__("D_L00_00160494");
extern char *D_L00_0015FFE8;
extern char *D_L00_00160108;
extern char *D_L00_0016016C;
extern s32   D_0014EE90[16];

typedef struct {
    u8  pad[0x10];
    u32 v __attribute__((packed, aligned(1)));
} Una244110;

extern u8  D_L00_00179100[];
extern s32 game_frame __asm__("D_L00_0015F5CC");

extern void flush_cache(s32) __asm__("FUN_00118a80");
extern void initialize_tfrag_render_data(char *, void *) __asm__("FUN_002040e0");
extern void register_object_render_class(char *, void *, void *, s32) __asm__("FUN_00203730");
extern void noop_main_callback(void *, s32) __asm__("FUN_001e93e8");
extern void
register_shrub_render_class(char *, void *, void *, void *, s32) __asm__("FUN_00203b08");
extern void  parse_particle_textures(void *, char *, void *, s32) __asm__("FUN_002026c8");
extern void  fill_transfer_words(void *, s32, s32) __asm__("FUN_001f97e8");
extern char *FUN_L00_00241940_244110(s32) __asm__("FUN_L00_00241940");
extern void  restore_or_clear_level_state(void) __asm__("FUN_L00_00285008");

#define GS_TEX0(tbp0, tbw, psm, tw, th, tcc, tfx, cbp, cpsm, csm, csa, cld)             \
    ((u64)(tbp0) | ((u64)(tbw) << 14) | ((u64)(psm) << 20) | ((u64)(tw) << 26) |        \
     ((u64)(th) << 30) | ((u64)(tcc) << 34) | ((u64)(tfx) << 35) | ((u64)(cbp) << 37) | \
     ((u64)(cpsm) << 51) | ((u64)(csm) << 55) | ((u64)(csa) << 56) | ((u64)(cld) << 61))
#define KB(x) (convert_integer_to_float((s32)(x)) * (1.0f / 1024.0f))

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
s32 load_level_assets_and_gameplay(s32 full, s32 keep) __asm__("FUN_L00_00244110");

s32 load_level_assets_and_gameplay(s32 full, s32 keep) {
    Load244110 *ld = &D_L00_00173E00_244110;
    s32        *core_header = ld->f4;
    s32        *level_file_base = D_0015EE48.file;
    char       *base;
    s32         used;
    char       *sound_reference_cursor;
    char       *free;
    char       *end;
    Span244110 *span;
    TextureRef *moby_texture_refs;
    TextureRef *tfrag_texture_refs;
    TextureRef *tie_texture_refs;
    TextureRef *shrub_texture_refs;
    char       *sound_table_base;
    s32         i;
    s32         j;
    s32         sound_slot_count;
    s32         k;
    s32         tfrag_texture_ref_index, moby_texture_ref_index, tie_texture_ref_index,
        shrub_texture_ref_index;
    s32   moby_class_reference_index;
    char *sequence_offset_cursor;
    s32   sq;
    char *sp;
    char *moby_class_reference_cursor;
    s32   z;
    s32   ci;
    char *x;
    char *y;
    s32   a1, a2, sound_slot_index;
    char *q1, *class_entry_cursor;
    u64  *t;

    if (full)
        load_hud_banks();
    base = ld->f14;
    if (full) {
        fast_mem_copy(
            core_header, (char *)(level_file_base[4] + (s32)level_file_base), level_file_base[5]);
        D_L00_0015F40C = 0;
        copy_memory_aligned(mission_state_macro, D_0014C050 + current_level_index * 16, 0x10);
        upload_texture_images((char *)(level_file_base[6] + (s32)level_file_base),
                              core_header[0],
                              (char *)core_header + core_header[1]);
        saved_sound_bank_handle = sound_bank_handle;
        flush_cache(0);
        used = fast_decompress((char *)(level_file_base[0x14] + (s32)level_file_base), base);
        D_L00_0015F40C = (s32)(base + core_header[0x18]);

        level_texture_128x128_descriptor[0] =
            GS_TEX0((gs_texture_allocation_base + core_header[0x24]) >> 8,
                    2,
                    0x13,
                    7,
                    7,
                    1,
                    0,
                    (gs_texture_allocation_base + core_header[0x25]) >> 8,
                    0,
                    0,
                    0,
                    4);
        level_texture_128x128_descriptor[1] = 0x0000FFA0000000E0;
        level_texture_128x128_descriptor[2] = 0x0040000400004000;
        level_texture_64x64_descriptor[0] =
            GS_TEX0((gs_texture_allocation_base + core_header[0x26]) >> 8,
                    1,
                    0x13,
                    6,
                    6,
                    1,
                    0,
                    (gs_texture_allocation_base + core_header[0x27]) >> 8,
                    0,
                    0,
                    0,
                    4);
        level_texture_64x64_descriptor[1] = 0x0000FFA0000000E0;
        level_texture_64x64_descriptor[2] = 0x0040000400004000;
        D_L00_001827F0[0] = GS_TEX0((gs_texture_allocation_base + core_header[0x30]) >> 8,
                                    2,
                                    0x13,
                                    7,
                                    7,
                                    1,
                                    0,
                                    (gs_texture_allocation_base + core_header[0x31]) >> 8,
                                    0,
                                    0,
                                    0,
                                    4);
        D_L00_001827F0[1] = 0x0000FFA0000000E0;
        D_L00_001827F0[2] = 0x0040000400004000;

        tfrag_texture_refs = (TextureRef *)((char *)core_header + core_header[0xD]);
        tfrag_texture_ref_count = core_header[0xC];
        for (tfrag_texture_ref_index = 0; tfrag_texture_ref_index < tfrag_texture_ref_count;
             tfrag_texture_ref_index++)
            tfrag_packed_texture_refs[tfrag_texture_ref_index] =
                D_L00_0015F40C + tfrag_texture_refs[tfrag_texture_ref_index].offset +
                (log2dim(tfrag_texture_refs[tfrag_texture_ref_index].kind) << 28);
        moby_texture_refs = (TextureRef *)((char *)core_header + core_header[0xF]);
        texture_count = core_header[0xE];
        for (moby_texture_ref_index = 0; moby_texture_ref_index < texture_count;
             moby_texture_ref_index++)
            D_L00_00199A80[moby_texture_ref_index] =
                D_L00_0015F40C + moby_texture_refs[moby_texture_ref_index].offset +
                (log2dim(moby_texture_refs[moby_texture_ref_index].kind) << 28);
        fast_mem_copy(moby_texture_table, moby_texture_refs, texture_count * 16);
        tie_texture_refs = (TextureRef *)((char *)core_header + core_header[0x11]);
        tie_texture_ref_count = core_header[0x10];
        for (tie_texture_ref_index = 0; tie_texture_ref_index < tie_texture_ref_count;
             tie_texture_ref_index++)
            tie_packed_texture_refs[tie_texture_ref_index] =
                D_L00_0015F40C + tie_texture_refs[tie_texture_ref_index].offset +
                (log2dim(tie_texture_refs[tie_texture_ref_index].kind) << 28);
        shrub_texture_refs = (TextureRef *)((char *)core_header + core_header[0x13]);
        shrub_texture_ref_count = core_header[0x12];
        for (shrub_texture_ref_index = 0; shrub_texture_ref_index < shrub_texture_ref_count;
             shrub_texture_ref_index++)
            shrub_packed_texture_refs[shrub_texture_ref_index] =
                D_L00_0015F40C + shrub_texture_refs[shrub_texture_ref_index].offset +
                (log2dim(shrub_texture_refs[shrub_texture_ref_index].kind) << 28);

        initialize_tfrag_render_data(base + core_header[2], (char *)core_header + core_header[0xD]);
        if (core_header[3])
            D_L00_0015F600_244110 = base + core_header[3];
        else
            D_L00_0015F600_244110 = 0;
        if (core_header[4])
            relocate_sky_definition(base + core_header[4]);
        else
            D_L00_0016051C = 0;
        resolve_level_asset_block_pointers(base + core_header[5]);
        for (i = 0, class_entry_cursor = (char *)core_header + core_header[7]; i < core_header[6];
             i++, class_entry_cursor += 0x20)
            register_moby_class(*(s32 *)class_entry_cursor ? base + *(s32 *)class_entry_cursor : 0,
                                (char *)core_header + core_header[0xF],
                                class_entry_cursor + 0x10,
                                *(s32 *)(class_entry_cursor + 4));
        print_moby_class_sizes((int *)((char *)core_header + core_header[7]), core_header[7]);
        for (sq = 0, sequence_offset_cursor = (char *)core_header + core_header[0x1E];
             sq < D_L00_00197300[0]->seq_count;
             sq++, sequence_offset_cursor += 4) {
            if (*(s32 *)sequence_offset_cursor) {
                D_L00_00197300[0]->seqs[sq] = base + *(s32 *)sequence_offset_cursor;
                relocate_asset_entry_pointers(D_L00_00197300[0], sq);
            }
        }
        D_L00_0016000C = -1;
        moby_class_reference_cursor = (char *)core_header + core_header[0x21];
        D_L00_00160010 = 0;
        moby_class_reference_count = core_header[0x20];
        for (moby_class_reference_index = 0; moby_class_reference_index < core_header[0x20];
             moby_class_reference_index++, moby_class_reference_cursor += 0x10) {
            moby_reference_class_ids[moby_class_reference_index] =
                *(s32 *)(moby_class_reference_cursor + 4);
            moby_reference_data_bases[moby_class_reference_index] =
                base + *(s32 *)moby_class_reference_cursor;
            D_L00_001AFC80[moby_class_reference_index] = *(s32 *)(moby_class_reference_cursor + 8);
            {
                s32 o = D_L00_00197BC0[*(s32 *)(moby_class_reference_cursor + 4)] * 32 + 0x10;
                qcopy(&l00_moby_reference_texture_slot_maps[moby_class_reference_index],
                      (char *)core_header + core_header[7] + o);
            }
        }
        for (i = 0, class_entry_cursor = (char *)core_header + core_header[9]; i < core_header[8];
             i++, class_entry_cursor += 0x20)
            register_object_render_class(base + *(s32 *)class_entry_cursor,
                                         (char *)core_header + core_header[0x11],
                                         class_entry_cursor + 0x10,
                                         *(s32 *)(class_entry_cursor + 4));
        noop_main_callback((char *)core_header + core_header[9], core_header[8]);
        for (i = 0, class_entry_cursor = (char *)core_header + core_header[0xB];
             i < core_header[0xA];
             i++, class_entry_cursor += 0x30)
            register_shrub_render_class(base + *(s32 *)class_entry_cursor,
                                        (char *)core_header + core_header[0x13],
                                        class_entry_cursor + 0x10,
                                        class_entry_cursor + 0x20,
                                        *(s32 *)(class_entry_cursor + 4));
        effect_texture_data_base = base + core_header[0x1A];
        unpack_point_records((char *)core_header + core_header[0x17], core_header[0x16]);
        if (core_header[0x28]) {
            relocate_counted_pointer_table(base + core_header[0x28]);
        } else {
            D_L00_0015F438 = 0;
            D_L00_0015F43C = 0;
            D_L00_0015F440 = 0;
        }
        if (core_header[0x29]) {
            load_two_int_two_float_header(base + core_header[0x29]);
        } else {
            D_L00_0015FC98 = 0;
            D_L00_0015FC9C = 0;
            D_L00_0015FCA4 = 0;
            D_L00_0015FCA0 = 0;
            D_L00_0015FCA8 = 0;
        }
        if (core_header[0x2A])
            D_L00_0015F604 = base + core_header[0x2A];
        else
            D_L00_0015F604 = 0;
        {
            char *x1 = base + core_header[0x19];
            char *x2 = (char *)core_header + core_header[0x15];
            parse_particle_textures(
                (char *)core_header + core_header[0x1B], x1, x2, core_header[0x14]);
        }

        sound_table_base = (char *)core_header + core_header[0x1C];
        span = (Span244110 *)sound_table_base;
        free = (char *)((u32)(base + used + 0x3F) & ~0x3F);
        sound_slot_count = span->count;
        D_L00_0015F5F4 = (Slot244110 *)free;
        D_L00_0015F5F0 = sound_slot_count;
        copy_memory_aligned(free, sound_table_base + span->off, sound_slot_count * 32);
        span++;
        free += sound_slot_count * 32;
        sound_reference_cursor = sound_table_base + span->off;
        for (sound_slot_index = 0; sound_slot_index < D_L00_0015F5F0; sound_slot_index++) {
            if (D_L00_0015F5F4[sound_slot_index].sound >= span->count) {
                debug_print(D_L00_001E8780);
                D_L00_0015F5F4[sound_slot_index].sound = -1;
            } else {
                D_L00_0015F5F4[sound_slot_index].sound =
                    *(u16 *)(sound_reference_cursor + D_L00_0015F5F4[sound_slot_index].sound * 4);
            }
            D_L00_0015F5F4[sound_slot_index].time = sound_bank_handle;
        }
        end = free + 0x3F;
        span++;
        for (ci = 0; ci < moby_class_count; ci++, span++) {
            Class244110 *moby_class = D_L00_00197300[ci];
            if (moby_class == 0) {
                for (j = 0; j < moby_class_reference_count; j++)
                    if (moby_reference_class_ids[j] == D_L00_00197A00[ci])
                        break;
                if (j < moby_class_reference_count) {
                    fill_transfer_words(moby_reference_sound_ids[j], -1, 0x20);
                    sound_reference_cursor = sound_table_base + span->off;
                    for (k = 0; k < span->count && k < 15; k++, sound_reference_cursor += 4)
                        moby_reference_sound_ids[j][k] = *(u16 *)sound_reference_cursor;
                }
            } else {
                if (span->count != moby_class->count0D)
                    debug_print(D_L00_001E87A0, D_L00_00197A00[ci]);
                sound_reference_cursor = sound_table_base + span->off;
                for (k = 0; k < D_L00_00197300[ci]->count0D; k++, sound_reference_cursor += 4) {
                    D_L00_00197300[ci]->f28[k].sound = *(u16 *)sound_reference_cursor;
                    D_L00_00197300[ci]->f28[k].time = sound_bank_handle;
                }
            }
        }
        D_L00_00173E00_244110.f18 =
            load_level_texture_bank((char *)((u32)end & ~0x3F), core_header);
    }
    D_L00_00173E00_244110.f1C = FUN_L00_00241940_244110(full);
    if (full) {
        char *top;
        debug_print(D_L00_001E87D0);
        debug_print(D_L00_001E87E8, KB(0x100000));
        top = level_code_memory_end;
        debug_print(D_L00_001E8800, KB(top - 0x100000));
        debug_print(D_L00_001E8818, KB(D_L00_00173E00_244110.f0 - top));
        debug_print(D_L00_001E8830, KB(D_L00_00173E00_244110.fC - D_L00_00173E00_244110.f0));
        debug_print(D_L00_001E8848, KB(D_L00_00173E00_244110.f10 - D_L00_00173E00_244110.fC));
        debug_print(D_L00_001E8860, KB(D_L00_00173E00_244110.f14 - D_L00_00173E00_244110.f10));
        if (D_L00_0015F600_244110 != 0)
            debug_print(D_L00_001E8878, KB(D_L00_0015F600_244110 - D_L00_00173E00_244110.f14));
        else if (D_L00_0016051C != 0)
            debug_print(D_L00_001E8878, KB((char *)D_L00_0016051C - D_L00_00173E00_244110.f14));
        else
            debug_print(D_L00_001E8890);
        if (D_L00_0015F600_244110 != 0) {
            if (D_L00_0016051C != 0)
                debug_print(D_L00_001E88B0, KB(D_L00_0016051C - (s32)D_L00_0015F600_244110));
            else
                debug_print(D_L00_001E88B0, KB(D_L00_00173E40_244110[0] - D_L00_0015F600_244110));
        }
        if (core_header[4])
            debug_print(D_L00_001E88C8, KB(D_L00_00173E40_244110[0] - (char *)D_L00_0016051C));
        debug_print(D_L00_001E88E0, KB(D_L00_0015F40C - (s32)D_L00_00173E40_244110[0]));
        debug_print(D_L00_001E88F8, KB((particle_texture_ranges[1] >> 4) - D_L00_0015F40C));
        debug_print(D_L00_001E8910,
                    KB((s32)effect_texture_data_base - (particle_texture_ranges[1] >> 4)));
        debug_print(D_L00_001E8928, KB((char *)D_L00_00197300[0] - effect_texture_data_base));
        debug_print(D_L00_001E8940, KB(D_L00_001C5800_244110[0] - (char *)D_L00_00197300[0]));
        if (core_header[0xA])
            debug_print(D_L00_001E8958, KB(D_L00_001BC030_244110[0] - D_L00_001C5800_244110[0]));
        else
            debug_print(D_L00_001E8958, KB(D_L00_00160FC0_244110 - D_L00_001C5800_244110[0]));
        if (core_header[0xA])
            debug_print(D_L00_001E8970, KB(D_L00_00197300[0]->seqs[0] - D_L00_001BC030_244110[0]));
        debug_print(D_L00_001E8988, KB(D_L00_00160FC0_244110 - D_L00_00197300[0]->seqs[0]));
        debug_print(D_L00_001E89A0, KB(D_L00_00160494_244110 - D_L00_00160FC0_244110));
        debug_print(D_L00_001E89B8, KB((char *)moby_table - D_L00_00160494_244110));
        debug_print(D_L00_001E89D0, KB(D_L00_0015FFE8 - (char *)moby_table));
        debug_print(D_L00_001E89E8, KB(D_L00_00160108 - D_L00_0015FFE8));
        debug_print(D_L00_001E8A00, KB(D_L00_0016016C - D_L00_00160108));
        debug_print(D_L00_001E8A18, 128.0);
        debug_print(level_memory_map_newline);
        debug_print(D_L00_001E8A30,
                    convert_integer_to_float((s32)D_L00_0016016C + 0x20000) * (1.0f / 1048576.0f));
        debug_print(level_memory_map_newline);
        {
            char *lo = D_L00_00173E00_244110.f0 - 0x2D0000;
            debug_print(D_L00_001E8A48,
                        convert_integer_to_float(D_L00_00173E00_244110.f1C - lo) *
                            (1.0f / 1048576.0f));
        }
        debug_print(level_memory_map_newline);
    }
    if (keep) {
        restore_or_clear_level_state();
    } else {
        clear_scene_state_buffers();
        for (z = 15; z >= 0; z--)
            D_0014EE90[z] = 0;
    }
    ((Una244110 *)D_L00_00179100)->v = 0;
    link_and_update_all_mobys();
    game_frame++;
    return 0;
}

struct StreamLoader {
    u8  pad0[0x30];
    s32 list; /* 0x30 */
    u8  pad34[0x28];
    u8 *buf; /* 0x5C */
};

extern struct StreamLoader D_L00_0016C860;
extern void                update_audio_stream_until_idle(s32) __asm__("FUN_002168a8");

/* streams entry i of the current sector list into the loader buffer */
s32 start_cutscene_chunk_read(s32 i) __asm__("FUN_L00_002451b8");

s32 start_cutscene_chunk_read(s32 i) {
    u8 *buf = D_L00_0016C860.buf;
    s32 start;
    s32 size;

    if (pal_mode != 0) {
        size = disc_table.sector_lists[D_L00_0016C860.list].sectors_alt[i + 1] -
               disc_table.sector_lists[D_L00_0016C860.list].sectors_alt[i];
        start = disc_table.sector_lists[D_L00_0016C860.list].sectors_alt[i];
    } else {
        size = disc_table.sector_lists[D_L00_0016C860.list].sectors[i + 1] -
               disc_table.sector_lists[D_L00_0016C860.list].sectors[i];
        start = disc_table.sector_lists[D_L00_0016C860.list].sectors[i];
    }
    if (size > 0) {
        start_audio_stream_read(buf, start, size);
        update_audio_stream_until_idle(0);
    }
    return 1;
}

s64 D_L01_00182C60[1] __attribute__((section(".data"))) = {0};

s64 D_L01_00182C68[1] __attribute__((section(".data"))) = {0};

s64 D_L01_00182C78[1] __attribute__((section(".data"))) = {0};

s32 D_L02_0017441C[1] __attribute__((section(".data"))) = {0};

s64 D_L02_00182DE0[1] __attribute__((section(".data"))) = {0};

s64 D_L02_00182DE8[1] __attribute__((section(".data"))) = {0};

s64 D_L02_00182DF8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B0220[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B0280[1] __attribute__((section(".data"))) = {0};

s32 D_L03_00173F1C[1] __attribute__((section(".data"))) = {0};

s64 D_L03_001828E0[1] __attribute__((section(".data"))) = {0};

s64 D_L03_001828E8[1] __attribute__((section(".data"))) = {0};

s64 D_L03_001828F8[1] __attribute__((section(".data"))) = {0};

s32 D_L04_00173F9C[1] __attribute__((section(".data"))) = {0};

s64 D_L04_00182960[1] __attribute__((section(".data"))) = {0};

s64 D_L04_00182968[1] __attribute__((section(".data"))) = {0};

s64 D_L04_00182978[1] __attribute__((section(".data"))) = {0};

s32 D_L05_0017421C[1] __attribute__((section(".data"))) = {0};

s64 D_L05_00182C60[1] __attribute__((section(".data"))) = {0};

s64 D_L05_00182C68[1] __attribute__((section(".data"))) = {0};

s64 D_L05_00182C78[1] __attribute__((section(".data"))) = {0};

s32 D_L08_0017461C[1] __attribute__((section(".data"))) = {0};

s64 D_L08_00182FE0[1] __attribute__((section(".data"))) = {0};

s64 D_L08_00182FE8[1] __attribute__((section(".data"))) = {0};

s64 D_L08_00182FF8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B0420[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B0480[1] __attribute__((section(".data"))) = {0};

s32 D_L09_00173F9C[1] __attribute__((section(".data"))) = {0};

s64 D_L09_00182960[1] __attribute__((section(".data"))) = {0};

s64 D_L09_00182968[1] __attribute__((section(".data"))) = {0};

s64 D_L09_00182978[1] __attribute__((section(".data"))) = {0};

s32 D_L10_0017429C[1] __attribute__((section(".data"))) = {0};

s64 D_L10_00182C60[1] __attribute__((section(".data"))) = {0};

s64 D_L10_00182C68[1] __attribute__((section(".data"))) = {0};

s64 D_L10_00182C78[1] __attribute__((section(".data"))) = {0};

s64 D_L11_001831E0[1] __attribute__((section(".data"))) = {0};

s64 D_L11_001831E8[1] __attribute__((section(".data"))) = {0};

s64 D_L11_001831F8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B0620[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B0680[1] __attribute__((section(".data"))) = {0};

s32 D_L12_0017429C[1] __attribute__((section(".data"))) = {0};

s64 D_L12_00182C60[1] __attribute__((section(".data"))) = {0};

s64 D_L12_00182C68[1] __attribute__((section(".data"))) = {0};

s64 D_L12_00182C78[1] __attribute__((section(".data"))) = {0};

s32 D_L13_0017411C[1] __attribute__((section(".data"))) = {0};

s64 D_L13_00182AE0[1] __attribute__((section(".data"))) = {0};

s64 D_L13_00182AE8[1] __attribute__((section(".data"))) = {0};

s64 D_L13_00182AF8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_0017451C[1] __attribute__((section(".data"))) = {0};

s64 D_L14_00182EE0[1] __attribute__((section(".data"))) = {0};

s64 D_L14_00182EE8[1] __attribute__((section(".data"))) = {0};

s64 D_L14_00182EF8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B0320[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B0380[1] __attribute__((section(".data"))) = {0};

s32 D_L15_0017441C[1] __attribute__((section(".data"))) = {0};

s64 D_L15_00182DE0[1] __attribute__((section(".data"))) = {0};

s64 D_L15_00182DE8[1] __attribute__((section(".data"))) = {0};

s64 D_L15_00182DF8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B0220[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B0280[1] __attribute__((section(".data"))) = {0};

s32 D_L16_0017421C[1] __attribute__((section(".data"))) = {0};

s64 D_L16_00182C60[1] __attribute__((section(".data"))) = {0};

s64 D_L16_00182C68[1] __attribute__((section(".data"))) = {0};

s64 D_L16_00182C78[1] __attribute__((section(".data"))) = {0};

s64 D_L17_001830E0[1] __attribute__((section(".data"))) = {0};

s64 D_L17_001830E8[1] __attribute__((section(".data"))) = {0};

s64 D_L17_001830F8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B0520[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B0580[1] __attribute__((section(".data"))) = {0};

Tbl                         *D_L01_0015F618 = 0;
int                          D_L01_001600E8 = 0;
int                          D_L01_001600F0 = 0;
int                          D_L01_001600F8 = 0;
int                          D_L01_00160100 = 0;
char                        *D_L01_0015FBC0 = 0;
char                        *D_L01_0015FB80 = 0;
int                          D_L01_0015FB50 = 0;
char                        *D_L01_00160040 = 0;
int                          D_L01_00160014 = 0;
ShrubClass *D_L01_001BC4B0   NOT_SDA = 0;
ShrubInst                   *D_L01_00160494 = 0;
ShrubInst                   *D_L01_00160498 = 0;
ShrubMatrix                 *D_L01_0016049C = 0;
TextTable                   *D_L01_0015F660 = 0;
Tfrag                       *D_L01_00160F4C = 0;
TieClass *D_L01_001C5C80     NOT_SDA = 0;
TieInst                     *D_L01_00160FC0 = 0;
TieInst                     *D_L01_00160FC4 = 0;
TieMatrix                   *D_L01_00160FC8 = 0;
char                        *D_L01_001600E4 = 0;
char                        *D_L01_00160108 = 0;
char                        *D_L01_0015F318 = 0;
char D_L01_0016CC30          NOT_SDA = 0;
char D_L01_00174380          NOT_SDA = 0;
char D_L01_0017F140          NOT_SDA = 0;
char D_L01_0019BC60          NOT_SDA = 0;
char D_L01_001ABC60          NOT_SDA = 0;
char D_L01_001B1D00          NOT_SDA = 0;
float                        D_L01_001604A4 = 0;
float                        D_L01_00160F80 = 0;
float                        D_L01_00160FE0 = 0;
int                          D_L01_0015EF54 = 0;
int                          D_L01_0015FBC4 = 0;
int                          D_L01_0015FFF0 = 0;
int                          D_L01_00160104 = 0;
int                          D_L01_00160170 = 0;
int                          D_L01_00160174 = 0;
int                          D_L01_00160178 = 0;
int                          D_L01_0016017C = 0;
int                          D_L01_00160490 = 0;
int                          D_L01_00160F50 = 0;
int                          D_L01_00160FCC = 0;
unsigned char               *D_L01_001604A0 = 0;
unsigned char D_L01_001BC630 NOT_SDA = 0;
unsigned char D_L01_001C5F80 NOT_SDA = 0;
u64                          D_L01_00182C40[3] = {0};

u64 D_L01_00182C58[3] = {0};

u64 D_L01_00182C70[3] = {0};

s32                          D_L01_0015FFC8 = 0;
u32 D_L01_00199F00           NOT_SDA = 0;
u8 D_L01_001AF040            NOT_SDA = 0;
s32                          D_L01_00160010 = 0;
s32 D_L01_001B0100           NOT_SDA = 0;
char                        *D_L01_0015F420 = 0;
char                        *D_L01_0015F604 = 0;
s32                          D_L01_0015F5F0 = 0;
char D_L01_002097E0          NOT_SDA = 0x69;
Tbl                         *D_L02_0015F618 = 0;
int                          D_L02_001600E8 = 0;
int                          D_L02_001600F0 = 0;
int                          D_L02_001600F8 = 0;
int                          D_L02_00160100 = 0;
char                        *D_L02_0015FBC0 = 0;
char                        *D_L02_0015FB80 = 0;
int                          D_L02_0015FB50 = 0;
char                        *D_L02_00160040 = 0;
int                          D_L02_00160014 = 0;
ShrubClass *D_L02_001BC630   NOT_SDA = 0;
ShrubInst                   *D_L02_00160494 = 0;
ShrubInst                   *D_L02_00160498 = 0;
ShrubMatrix                 *D_L02_0016049C = 0;
TextTable                   *D_L02_0015F660 = 0;
Tfrag                       *D_L02_00160F4C = 0;
TieClass *D_L02_001C5E00     NOT_SDA = 0;
TieInst                     *D_L02_00160FC0 = 0;
TieInst                     *D_L02_00160FC4 = 0;
TieMatrix                   *D_L02_00160FC8 = 0;
char                        *D_L02_001600E4 = 0;
char                        *D_L02_00160108 = 0;
char                        *D_L02_0015F318 = 0;
char D_L02_0016CDB0          NOT_SDA = 0;
char D_L02_00174500          NOT_SDA = 0;
char D_L02_0017F2C0          NOT_SDA = 0;
char D_L02_0019BDE0          NOT_SDA = 0;
char D_L02_001ABDE0          NOT_SDA = 0;
char D_L02_001B1E80          NOT_SDA = 0;
float                        D_L02_001604A4 = 0;
float                        D_L02_00160F80 = 0;
float                        D_L02_00160FE0 = 0;
int                          D_L02_0015EF54 = 0;
int                          D_L02_0015FBC4 = 0;
int                          D_L02_0015FFF0 = 0;
int                          D_L02_00160104 = 0;
int                          D_L02_00160170 = 0;
int                          D_L02_00160174 = 0;
int                          D_L02_0016017C = 0;
int                          D_L02_00160490 = 0;
int                          D_L02_00160F50 = 0;
int                          D_L02_00160FCC = 0;
unsigned char               *D_L02_001604A0 = 0;
unsigned char D_L02_001BC7B0 NOT_SDA = 0;
unsigned char D_L02_001C6100 NOT_SDA = 0;
u64                          D_L02_00182DC0[3] = {0};

u64 D_L02_00182DD8[3] = {0};

u64 D_L02_00182DF0[3] = {0};

s32                          D_L02_0015FFC8 = 0;
u32 D_L02_0019A080           NOT_SDA = 0;
u8 D_L02_001AF1C0            NOT_SDA = 0;
s32                          D_L02_00160010 = 0;
char                        *D_L02_0015F604 = 0;
s32                          D_L02_0015F5F0 = 0;
char D_L02_001F9080          NOT_SDA = 0x69;
Tbl                         *D_L03_0015F618 = 0;
int                          D_L03_001600E8 = 0;
int                          D_L03_001600F0 = 0;
int                          D_L03_001600F8 = 0;
int                          D_L03_00160100 = 0;
char                        *D_L03_0015FBC0 = 0;
char                        *D_L03_0015FB80 = 0;
int                          D_L03_0015FB50 = 0;
char                        *D_L03_00160040 = 0;
int                          D_L03_00160014 = 0;
ShrubClass *D_L03_001BC130   NOT_SDA = 0;
ShrubInst                   *D_L03_00160494 = 0;
ShrubInst                   *D_L03_00160498 = 0;
ShrubMatrix                 *D_L03_0016049C = 0;
TextTable                   *D_L03_0015F660 = 0;
Tfrag                       *D_L03_00160F4C = 0;
TieClass *D_L03_001C5900     NOT_SDA = 0;
TieInst                     *D_L03_00160FC0 = 0;
TieInst                     *D_L03_00160FC4 = 0;
TieMatrix                   *D_L03_00160FC8 = 0;
char                        *D_L03_001600E4 = 0;
char                        *D_L03_00160108 = 0;
char                        *D_L03_0015F318 = 0;
char D_L03_0016C8B0          NOT_SDA = 0;
char D_L03_00174000          NOT_SDA = 0;
char D_L03_0017EDC0          NOT_SDA = 0;
char D_L03_0019B8E0          NOT_SDA = 0;
char D_L03_001AB8E0          NOT_SDA = 0;
char D_L03_001B1980          NOT_SDA = 0;
float                        D_L03_001604A4 = 0;
float                        D_L03_00160F80 = 0;
float                        D_L03_00160FE0 = 0;
int                          D_L03_0015EF54 = 0;
int                          D_L03_0015FBC4 = 0;
int                          D_L03_0015FFF0 = 0;
int                          D_L03_00160104 = 0;
int                          D_L03_00160170 = 0;
int                          D_L03_00160174 = 0;
int                          D_L03_00160178 = 0;
int                          D_L03_0016017C = 0;
int                          D_L03_00160490 = 0;
int                          D_L03_00160F50 = 0;
int                          D_L03_00160FCC = 0;
unsigned char               *D_L03_001604A0 = 0;
unsigned char D_L03_001BC2B0 NOT_SDA = 0;
unsigned char D_L03_001C5C00 NOT_SDA = 0;
u64                          D_L03_001828C0[3] = {0};

u64 D_L03_001828D8[3] = {0};

u64 D_L03_001828F0[3] = {0};

s32                          D_L03_0015FFC8 = 0;
u32 D_L03_00199B80           NOT_SDA = 0;
u8 D_L03_001AECC0            NOT_SDA = 0;
s32                          D_L03_00160010 = 0;
s32 D_L03_001AFD80           NOT_SDA = 0;
char                        *D_L03_0015F604 = 0;
s32                          D_L03_0015F5F0 = 0;
char D_L03_001E1400          NOT_SDA = 0x69;
Tbl                         *D_L04_0015F618 = 0;
int                          D_L04_001600E8 = 0;
int                          D_L04_001600F0 = 0;
int                          D_L04_001600F8 = 0;
int                          D_L04_00160100 = 0;
char                        *D_L04_0015FBC0 = 0;
char                        *D_L04_0015FB80 = 0;
int                          D_L04_0015FB50 = 0;
char                        *D_L04_00160040 = 0;
int                          D_L04_00160014 = 0;
ShrubClass *D_L04_001BC1B0   NOT_SDA = 0;
ShrubInst                   *D_L04_00160494 = 0;
ShrubInst                   *D_L04_00160498 = 0;
ShrubMatrix                 *D_L04_0016049C = 0;
TextTable                   *D_L04_0015F660 = 0;
Tfrag                       *D_L04_00160F4C = 0;
TieClass *D_L04_001C5980     NOT_SDA = 0;
TieInst                     *D_L04_00160FC0 = 0;
TieInst                     *D_L04_00160FC4 = 0;
TieMatrix                   *D_L04_00160FC8 = 0;
char                        *D_L04_001600E4 = 0;
char                        *D_L04_00160108 = 0;
char                        *D_L04_0015F318 = 0;
char D_L04_0016C930          NOT_SDA = 0;
char D_L04_00174080          NOT_SDA = 0;
char D_L04_0017EE40          NOT_SDA = 0;
char D_L04_0019B960          NOT_SDA = 0;
char D_L04_001AB960          NOT_SDA = 0;
char D_L04_001B1A00          NOT_SDA = 0;
float                        D_L04_001604A4 = 0;
float                        D_L04_00160F80 = 0;
float                        D_L04_00160FE0 = 0;
int                          D_L04_0015EF54 = 0;
int                          D_L04_0015FBC4 = 0;
int                          D_L04_0015FFF0 = 0;
int                          D_L04_00160104 = 0;
int                          D_L04_00160170 = 0;
int                          D_L04_00160174 = 0;
int                          D_L04_00160178 = 0;
int                          D_L04_0016017C = 0;
int                          D_L04_00160490 = 0;
int                          D_L04_00160F50 = 0;
int                          D_L04_00160FCC = 0;
unsigned char               *D_L04_001604A0 = 0;
unsigned char D_L04_001BC330 NOT_SDA = 0;
unsigned char D_L04_001C5C80 NOT_SDA = 0;
u64                          D_L04_00182940[3] = {0};

u64 D_L04_00182958[3] = {0};

u64 D_L04_00182970[3] = {0};

s32                          D_L04_0015FFC8 = 0;
u32 D_L04_00199C00           NOT_SDA = 0;
u8 D_L04_001AED40            NOT_SDA = 0;
s32                          D_L04_00160010 = 0;
s32 D_L04_001AFE00           NOT_SDA = 0;
char                        *D_L04_0015F420 = 0;
char                        *D_L04_0015F604 = 0;
s32                          D_L04_0015F5F0 = 0;
char D_L04_001DA520          NOT_SDA = 0x69;
Tbl                         *D_L05_0015F618 = 0;
int                          D_L05_001600E8 = 0;
int                          D_L05_001600F0 = 0;
int                          D_L05_001600F8 = 0;
int                          D_L05_00160100 = 0;
char                        *D_L05_0015FBC0 = 0;
char                        *D_L05_0015FB80 = 0;
int                          D_L05_0015FB50 = 0;
char                        *D_L05_00160040 = 0;
int                          D_L05_00160014 = 0;
ShrubClass *D_L05_001BC4B0   NOT_SDA = 0;
ShrubInst                   *D_L05_00160494 = 0;
ShrubInst                   *D_L05_00160498 = 0;
ShrubMatrix                 *D_L05_0016049C = 0;
TextTable                   *D_L05_0015F660 = 0;
Tfrag                       *D_L05_00160F4C = 0;
TieClass *D_L05_001C5C80     NOT_SDA = 0;
TieInst                     *D_L05_00160FC0 = 0;
TieInst                     *D_L05_00160FC4 = 0;
TieMatrix                   *D_L05_00160FC8 = 0;
char                        *D_L05_001600E4 = 0;
char                        *D_L05_00160108 = 0;
char                        *D_L05_0015F318 = 0;
char D_L05_0016CBB0          NOT_SDA = 0;
char D_L05_00174300          NOT_SDA = 0;
char D_L05_0017F140          NOT_SDA = 0;
char D_L05_0019BC60          NOT_SDA = 0;
char D_L05_001ABC60          NOT_SDA = 0;
char D_L05_001B1D00          NOT_SDA = 0;
float                        D_L05_001604A4 = 0;
float                        D_L05_00160F80 = 0;
float                        D_L05_00160FE0 = 0;
int                          D_L05_0015EF54 = 0;
int                          D_L05_0015FBC4 = 0;
int                          D_L05_0015FFF0 = 0;
int                          D_L05_00160104 = 0;
int                          D_L05_00160170 = 0;
int                          D_L05_00160174 = 0;
int                          D_L05_00160178 = 0;
int                          D_L05_0016017C = 0;
int                          D_L05_00160490 = 0;
int                          D_L05_00160F50 = 0;
int                          D_L05_00160FCC = 0;
unsigned char               *D_L05_001604A0 = 0;
unsigned char D_L05_001BC630 NOT_SDA = 0;
unsigned char D_L05_001C5F80 NOT_SDA = 0;
u64                          D_L05_00182C40[3] = {0};

u64 D_L05_00182C58[3] = {0};

u64 D_L05_00182C70[3] = {0};

s32                          D_L05_0015FFC8 = 0;
u32 D_L05_00199F00           NOT_SDA = 0;
u8 D_L05_001AF040            NOT_SDA = 0;
s32                          D_L05_00160010 = 0;
s32 D_L05_001B0100           NOT_SDA = 0;
char                        *D_L05_0015F604 = 0;
s32                          D_L05_0015F5F0 = 0;
char D_L05_00216E90          NOT_SDA = 0x69;
Tbl                         *D_L06_0015F618 = 0;
int                          D_L06_001600E8 = 0;
int                          D_L06_001600F0 = 0;
int                          D_L06_001600F8 = 0;
int                          D_L06_00160100 = 0;
char                        *D_L06_0015FBC0 = 0;
char                        *D_L06_0015FB80 = 0;
int                          D_L06_0015FB50 = 0;
char                        *D_L06_00160040 = 0;
int                          D_L06_00160014 = 0;
ShrubInst                   *D_L06_00160498 = 0;
ShrubMatrix                 *D_L06_0016049C = 0;
TextTable                   *D_L06_0015F660 = 0;
Tfrag                       *D_L06_00160F4C = 0;
TieInst                     *D_L06_00160FC4 = 0;
TieMatrix                   *D_L06_00160FC8 = 0;
char                        *D_L06_001600E4 = 0;
char                        *D_L06_0015F318 = 0;
char D_L06_0016CF30          NOT_SDA = 0;
char D_L06_00174680          NOT_SDA = 0;
char D_L06_0017F440          NOT_SDA = 0;
char D_L06_0019BF60          NOT_SDA = 0;
char D_L06_001ABF60          NOT_SDA = 0;
char D_L06_001B2000          NOT_SDA = 0;
float                        D_L06_001604A4 = 0;
float                        D_L06_00160F80 = 0;
float                        D_L06_00160FE0 = 0;
int                          D_L06_0015EF54 = 0;
int                          D_L06_0015FBC4 = 0;
int                          D_L06_0015FFF0 = 0;
int                          D_L06_00160104 = 0;
int                          D_L06_00160170 = 0;
int                          D_L06_00160174 = 0;
int                          D_L06_00160178 = 0;
int                          D_L06_0016017C = 0;
int                          D_L06_00160490 = 0;
int                          D_L06_00160F50 = 0;
int                          D_L06_00160FCC = 0;
unsigned char               *D_L06_001604A0 = 0;
unsigned char D_L06_001BC930 NOT_SDA = 0;
unsigned char D_L06_001C6280 NOT_SDA = 0;
Tbl                         *D_L07_0015F618 = 0;
int                          D_L07_001600E8 = 0;
int                          D_L07_001600F0 = 0;
int                          D_L07_001600F8 = 0;
int                          D_L07_00160100 = 0;
char                        *D_L07_0015FBC0 = 0;
char                        *D_L07_0015FB80 = 0;
int                          D_L07_0015FB50 = 0;
char                        *D_L07_00160040 = 0;
int                          D_L07_00160014 = 0;
ShrubInst                   *D_L07_00160498 = 0;
ShrubMatrix                 *D_L07_0016049C = 0;
TextTable                   *D_L07_0015F660 = 0;
Tfrag                       *D_L07_00160F4C = 0;
TieInst                     *D_L07_00160FC4 = 0;
TieMatrix                   *D_L07_00160FC8 = 0;
char                        *D_L07_001600E4 = 0;
char                        *D_L07_0015F318 = 0;
char D_L07_0016C830          NOT_SDA = 0;
char D_L07_00173F80          NOT_SDA = 0;
char D_L07_0017ED40          NOT_SDA = 0;
char D_L07_0019B860          NOT_SDA = 0;
char D_L07_001AB860          NOT_SDA = 0;
char D_L07_001B1900          NOT_SDA = 0;
float                        D_L07_001604A4 = 0;
float                        D_L07_00160F80 = 0;
float                        D_L07_00160FE0 = 0;
int                          D_L07_0015EF54 = 0;
int                          D_L07_0015FBC4 = 0;
int                          D_L07_0015FFF0 = 0;
int                          D_L07_00160104 = 0;
int                          D_L07_00160170 = 0;
int                          D_L07_00160174 = 0;
int                          D_L07_00160178 = 0;
int                          D_L07_0016017C = 0;
int                          D_L07_00160490 = 0;
int                          D_L07_00160F50 = 0;
int                          D_L07_00160FCC = 0;
unsigned char               *D_L07_001604A0 = 0;
unsigned char D_L07_001BC230 NOT_SDA = 0;
unsigned char D_L07_001C5B80 NOT_SDA = 0;
Tbl                         *D_L08_0015F618 = 0;
int                          D_L08_001600E8 = 0;
int                          D_L08_001600F0 = 0;
int                          D_L08_001600F8 = 0;
int                          D_L08_00160100 = 0;
char                        *D_L08_0015FBC0 = 0;
char                        *D_L08_0015FB80 = 0;
int                          D_L08_0015FB50 = 0;
char                        *D_L08_00160040 = 0;
int                          D_L08_00160014 = 0;
ShrubClass *D_L08_001BC830   NOT_SDA = 0;
ShrubInst                   *D_L08_00160494 = 0;
ShrubInst                   *D_L08_00160498 = 0;
ShrubMatrix                 *D_L08_0016049C = 0;
TextTable                   *D_L08_0015F660 = 0;
Tfrag                       *D_L08_00160F4C = 0;
TieClass *D_L08_001C6000     NOT_SDA = 0;
TieInst                     *D_L08_00160FC0 = 0;
TieInst                     *D_L08_00160FC4 = 0;
TieMatrix                   *D_L08_00160FC8 = 0;
char                        *D_L08_001600E4 = 0;
char                        *D_L08_00160108 = 0;
char                        *D_L08_0015F318 = 0;
char D_L08_0016CFB0          NOT_SDA = 0;
char D_L08_0017F4C0          NOT_SDA = 0;
char D_L08_0019BFE0          NOT_SDA = 0;
char D_L08_001ABFE0          NOT_SDA = 0;
char D_L08_001B2080          NOT_SDA = 0;
float                        D_L08_001604A4 = 0;
float                        D_L08_00160F80 = 0;
float                        D_L08_00160FE0 = 0;
int                          D_L08_0015EF54 = 0;
int                          D_L08_0015FBC4 = 0;
int                          D_L08_0015FFF0 = 0;
int                          D_L08_00160104 = 0;
int                          D_L08_00160170 = 0;
int                          D_L08_00160174 = 0;
int                          D_L08_00160178 = 0;
int                          D_L08_0016017C = 0;
int                          D_L08_00160490 = 0;
int                          D_L08_00160F50 = 0;
int                          D_L08_00160FCC = 0;
unsigned char               *D_L08_001604A0 = 0;
unsigned char D_L08_001BC9B0 NOT_SDA = 0;
unsigned char D_L08_001C6300 NOT_SDA = 0;
u64                          D_L08_00182FC0[3] = {0};

u64 D_L08_00182FD8[3] = {0};

u64 D_L08_00182FF0[3] = {0};

s32                          D_L08_0015FFC8 = 0;
u32 D_L08_0019A280           NOT_SDA = 0;
u8 D_L08_001AF3C0            NOT_SDA = 0;
s32                          D_L08_00160010 = 0;
char                        *D_L08_0015F604 = 0;
s32                          D_L08_0015F5F0 = 0;
char D_L08_001FA6C0          NOT_SDA = 0x69;
Tbl                         *D_L09_0015F618 = 0;
int                          D_L09_001600E8 = 0;
int                          D_L09_001600F0 = 0;
int                          D_L09_001600F8 = 0;
int                          D_L09_00160100 = 0;
char                        *D_L09_0015FBC0 = 0;
char                        *D_L09_0015FB80 = 0;
int                          D_L09_0015FB50 = 0;
char                        *D_L09_00160040 = 0;
int                          D_L09_00160014 = 0;
ShrubClass *D_L09_001BC1B0   NOT_SDA = 0;
ShrubInst                   *D_L09_00160494 = 0;
ShrubInst                   *D_L09_00160498 = 0;
ShrubMatrix                 *D_L09_0016049C = 0;
TextTable                   *D_L09_0015F660 = 0;
Tfrag                       *D_L09_00160F4C = 0;
TieClass *D_L09_001C5980     NOT_SDA = 0;
TieInst                     *D_L09_00160FC0 = 0;
TieInst                     *D_L09_00160FC4 = 0;
TieMatrix                   *D_L09_00160FC8 = 0;
char                        *D_L09_001600E4 = 0;
char                        *D_L09_00160108 = 0;
char                        *D_L09_0015F318 = 0;
char D_L09_0016C930          NOT_SDA = 0;
char D_L09_00174080          NOT_SDA = 0;
char D_L09_0017EE40          NOT_SDA = 0;
char D_L09_0019B960          NOT_SDA = 0;
char D_L09_001AB960          NOT_SDA = 0;
char D_L09_001B1A00          NOT_SDA = 0;
float                        D_L09_001604A4 = 0;
float                        D_L09_00160F80 = 0;
float                        D_L09_00160FE0 = 0;
int                          D_L09_0015EF54 = 0;
int                          D_L09_0015FBC4 = 0;
int                          D_L09_0015FFF0 = 0;
int                          D_L09_00160104 = 0;
int                          D_L09_00160170 = 0;
int                          D_L09_00160174 = 0;
int                          D_L09_00160178 = 0;
int                          D_L09_0016017C = 0;
int                          D_L09_00160490 = 0;
int                          D_L09_00160F50 = 0;
int                          D_L09_00160FCC = 0;
unsigned char               *D_L09_001604A0 = 0;
unsigned char D_L09_001BC330 NOT_SDA = 0;
unsigned char D_L09_001C5C80 NOT_SDA = 0;
u64                          D_L09_00182940[3] = {0};

u64 D_L09_00182958[3] = {0};

u64 D_L09_00182970[3] = {0};

s32                          D_L09_0015FFC8 = 0;
u32 D_L09_00199C00           NOT_SDA = 0;
u8 D_L09_001AED40            NOT_SDA = 0;
s32                          D_L09_00160010 = 0;
s32 D_L09_001AFE00           NOT_SDA = 0;
char                        *D_L09_0015F604 = 0;
s32                          D_L09_0015F5F0 = 0;
char D_L09_00207290          NOT_SDA = 0x69;
Tbl                         *D_L10_0015F618 = 0;
int                          D_L10_001600E8 = 0;
int                          D_L10_001600F0 = 0;
int                          D_L10_001600F8 = 0;
int                          D_L10_00160100 = 0;
char                        *D_L10_0015FBC0 = 0;
char                        *D_L10_0015FB80 = 0;
int                          D_L10_0015FB50 = 0;
char                        *D_L10_00160040 = 0;
int                          D_L10_00160014 = 0;
ShrubClass *D_L10_001BC4B0   NOT_SDA = 0;
ShrubInst                   *D_L10_00160494 = 0;
ShrubInst                   *D_L10_00160498 = 0;
ShrubMatrix                 *D_L10_0016049C = 0;
TextTable                   *D_L10_0015F660 = 0;
Tfrag                       *D_L10_00160F4C = 0;
TieClass *D_L10_001C5C80     NOT_SDA = 0;
TieInst                     *D_L10_00160FC0 = 0;
TieInst                     *D_L10_00160FC4 = 0;
TieMatrix                   *D_L10_00160FC8 = 0;
char                        *D_L10_001600E4 = 0;
char                        *D_L10_00160108 = 0;
char                        *D_L10_0015F318 = 0;
char D_L10_0016CC30          NOT_SDA = 0;
char D_L10_0017F140          NOT_SDA = 0;
char D_L10_0019BC60          NOT_SDA = 0;
char D_L10_001ABC60          NOT_SDA = 0;
char D_L10_001B1D00          NOT_SDA = 0;
float                        D_L10_001604A4 = 0;
float                        D_L10_00160F80 = 0;
float                        D_L10_00160FE0 = 0;
int                          D_L10_0015EF54 = 0;
int                          D_L10_0015FBC4 = 0;
int                          D_L10_0015FFF0 = 0;
int                          D_L10_00160104 = 0;
int                          D_L10_00160170 = 0;
int                          D_L10_00160174 = 0;
int                          D_L10_00160178 = 0;
int                          D_L10_0016017C = 0;
int                          D_L10_00160490 = 0;
int                          D_L10_00160F50 = 0;
int                          D_L10_00160FCC = 0;
unsigned char               *D_L10_001604A0 = 0;
unsigned char D_L10_001BC630 NOT_SDA = 0;
unsigned char D_L10_001C5F80 NOT_SDA = 0;
u64                          D_L10_00182C40[3] = {0};

u64 D_L10_00182C58[3] = {0};

u64 D_L10_00182C70[3] = {0};

s32                          D_L10_0015FFC8 = 0;
u32 D_L10_00199F00           NOT_SDA = 0;
u8 D_L10_001AF040            NOT_SDA = 0;
s32                          D_L10_00160010 = 0;
s32 D_L10_001B0100           NOT_SDA = 0;
char                        *D_L10_0015F420 = 0;
char                        *D_L10_0015F604 = 0;
s32                          D_L10_0015F5F0 = 0;
char D_L10_001DB500          NOT_SDA = 0x69;
Tbl                         *D_L11_0015F618 = 0;
int                          D_L11_001600E8 = 0;
int                          D_L11_001600F0 = 0;
int                          D_L11_001600F8 = 0;
int                          D_L11_00160100 = 0;
char                        *D_L11_0015FBC0 = 0;
char                        *D_L11_0015FB80 = 0;
int                          D_L11_0015FB50 = 0;
char                        *D_L11_00160040 = 0;
int                          D_L11_00160014 = 0;
ShrubClass *D_L11_001BCA30   NOT_SDA = 0;
ShrubInst                   *D_L11_00160494 = 0;
ShrubInst                   *D_L11_00160498 = 0;
ShrubMatrix                 *D_L11_0016049C = 0;
TextTable                   *D_L11_0015F660 = 0;
Tfrag                       *D_L11_00160F4C = 0;
TieClass *D_L11_001C6200     NOT_SDA = 0;
TieInst                     *D_L11_00160FC0 = 0;
TieInst                     *D_L11_00160FC4 = 0;
TieMatrix                   *D_L11_00160FC8 = 0;
char                        *D_L11_001600E4 = 0;
char                        *D_L11_00160108 = 0;
char                        *D_L11_0015F318 = 0;
char D_L11_0016D1B0          NOT_SDA = 0;
char D_L11_00174900          NOT_SDA = 0;
char D_L11_0017F6C0          NOT_SDA = 0;
char D_L11_0019C1E0          NOT_SDA = 0;
char D_L11_001AC1E0          NOT_SDA = 0;
char D_L11_001B2280          NOT_SDA = 0;
float                        D_L11_001604A4 = 0;
float                        D_L11_00160F80 = 0;
float                        D_L11_00160FE0 = 0;
int                          D_L11_0015EF54 = 0;
int                          D_L11_0015FBC4 = 0;
int                          D_L11_0015FFF0 = 0;
int                          D_L11_00160104 = 0;
int                          D_L11_00160170 = 0;
int                          D_L11_00160174 = 0;
int                          D_L11_00160178 = 0;
int                          D_L11_0016017C = 0;
int                          D_L11_00160490 = 0;
int                          D_L11_00160F50 = 0;
int                          D_L11_00160FCC = 0;
unsigned char               *D_L11_001604A0 = 0;
unsigned char D_L11_001BCBB0 NOT_SDA = 0;
unsigned char D_L11_001C6500 NOT_SDA = 0;
u64                          D_L11_001831C0[3] = {0};

u64 D_L11_001831D8[3] = {0};

u64 D_L11_001831F0[3] = {0};

s32                          D_L11_0015FFC8 = 0;
u32 D_L11_0019A480           NOT_SDA = 0;
u8 D_L11_001AF5C0            NOT_SDA = 0;
s32                          D_L11_00160010 = 0;
char                        *D_L11_0015F604 = 0;
s32                          D_L11_0015F5F0 = 0;
char D_L11_002198D0          NOT_SDA = 0x69;
Tbl                         *D_L12_0015F618 = 0;
int                          D_L12_001600E8 = 0;
int                          D_L12_001600F0 = 0;
int                          D_L12_001600F8 = 0;
int                          D_L12_00160100 = 0;
char                        *D_L12_0015FBC0 = 0;
char                        *D_L12_0015FB80 = 0;
int                          D_L12_0015FB50 = 0;
char                        *D_L12_00160040 = 0;
int                          D_L12_00160014 = 0;
ShrubClass *D_L12_001BC4B0   NOT_SDA = 0;
ShrubInst                   *D_L12_00160494 = 0;
ShrubInst                   *D_L12_00160498 = 0;
ShrubMatrix                 *D_L12_0016049C = 0;
TextTable                   *D_L12_0015F660 = 0;
Tfrag                       *D_L12_00160F4C = 0;
TieClass *D_L12_001C5C80     NOT_SDA = 0;
TieInst                     *D_L12_00160FC0 = 0;
TieInst                     *D_L12_00160FC4 = 0;
TieMatrix                   *D_L12_00160FC8 = 0;
char                        *D_L12_001600E4 = 0;
char                        *D_L12_00160108 = 0;
char                        *D_L12_0015F318 = 0;
char D_L12_0016CC30          NOT_SDA = 0;
char D_L12_00174380          NOT_SDA = 0;
char D_L12_0017F140          NOT_SDA = 0;
char D_L12_0019BC60          NOT_SDA = 0;
char D_L12_001ABC60          NOT_SDA = 0;
char D_L12_001B1D00          NOT_SDA = 0;
float                        D_L12_001604A4 = 0;
float                        D_L12_00160F80 = 0;
float                        D_L12_00160FE0 = 0;
int                          D_L12_0015EF54 = 0;
int                          D_L12_0015FBC4 = 0;
int                          D_L12_0015FFF0 = 0;
int                          D_L12_00160104 = 0;
int                          D_L12_00160170 = 0;
int                          D_L12_00160174 = 0;
int                          D_L12_00160178 = 0;
int                          D_L12_0016017C = 0;
int                          D_L12_00160490 = 0;
int                          D_L12_00160F50 = 0;
int                          D_L12_00160FCC = 0;
unsigned char               *D_L12_001604A0 = 0;
unsigned char D_L12_001BC630 NOT_SDA = 0;
unsigned char D_L12_001C5F80 NOT_SDA = 0;
u64                          D_L12_00182C40[3] = {0};

u64 D_L12_00182C58[3] = {0};

u64 D_L12_00182C70[3] = {0};

s32                          D_L12_0015FFC8 = 0;
u32 D_L12_00199F00           NOT_SDA = 0;
u8 D_L12_001AF040            NOT_SDA = 0;
s32                          D_L12_00160010 = 0;
s32 D_L12_001B0100           NOT_SDA = 0;
char                        *D_L12_0015F420 = 0;
char                        *D_L12_0015F604 = 0;
s32                          D_L12_0015F5F0 = 0;
char D_L12_0020AC60          NOT_SDA = 0x69;
Tbl                         *D_L13_0015F618 = 0;
int                          D_L13_001600E8 = 0;
int                          D_L13_001600F0 = 0;
int                          D_L13_001600F8 = 0;
int                          D_L13_00160100 = 0;
char                        *D_L13_0015FBC0 = 0;
char                        *D_L13_0015FB80 = 0;
int                          D_L13_0015FB50 = 0;
char                        *D_L13_00160040 = 0;
int                          D_L13_00160014 = 0;
ShrubClass *D_L13_001BC330   NOT_SDA = 0;
ShrubInst                   *D_L13_00160494 = 0;
ShrubInst                   *D_L13_00160498 = 0;
ShrubMatrix                 *D_L13_0016049C = 0;
TextTable                   *D_L13_0015F660 = 0;
Tfrag                       *D_L13_00160F4C = 0;
TieClass *D_L13_001C5B00     NOT_SDA = 0;
TieInst                     *D_L13_00160FC0 = 0;
TieInst                     *D_L13_00160FC4 = 0;
TieMatrix                   *D_L13_00160FC8 = 0;
char                        *D_L13_001600E4 = 0;
char                        *D_L13_00160108 = 0;
char                        *D_L13_0015F318 = 0;
char D_L13_0016CAB0          NOT_SDA = 0;
char D_L13_00174200          NOT_SDA = 0;
char D_L13_0017EFC0          NOT_SDA = 0;
char D_L13_0019BAE0          NOT_SDA = 0;
char D_L13_001ABAE0          NOT_SDA = 0;
char D_L13_001B1B80          NOT_SDA = 0;
float                        D_L13_001604A4 = 0;
float                        D_L13_00160F80 = 0;
float                        D_L13_00160FE0 = 0;
int                          D_L13_0015EF54 = 0;
int                          D_L13_0015FBC4 = 0;
int                          D_L13_0015FFF0 = 0;
int                          D_L13_00160104 = 0;
int                          D_L13_00160170 = 0;
int                          D_L13_00160174 = 0;
int                          D_L13_00160178 = 0;
int                          D_L13_0016017C = 0;
int                          D_L13_00160490 = 0;
int                          D_L13_00160F50 = 0;
int                          D_L13_00160FCC = 0;
unsigned char               *D_L13_001604A0 = 0;
unsigned char D_L13_001BC4B0 NOT_SDA = 0;
unsigned char D_L13_001C5E00 NOT_SDA = 0;
u64                          D_L13_00182AC0[3] = {0};

u64 D_L13_00182AD8[3] = {0};

u64 D_L13_00182AF0[3] = {0};

s32                          D_L13_0015FFC8 = 0;
u32 D_L13_00199D80           NOT_SDA = 0;
u8 D_L13_001AEEC0            NOT_SDA = 0;
s32                          D_L13_00160010 = 0;
s32 D_L13_001AFF80           NOT_SDA = 0;
char                        *D_L13_0015F604 = 0;
s32                          D_L13_0015F5F0 = 0;
char D_L13_001F31F0          NOT_SDA = 0x69;
Tbl                         *D_L14_0015F618 = 0;
int                          D_L14_001600E8 = 0;
int                          D_L14_001600F0 = 0;
int                          D_L14_001600F8 = 0;
int                          D_L14_00160100 = 0;
char                        *D_L14_0015FBC0 = 0;
char                        *D_L14_0015FB80 = 0;
int                          D_L14_0015FB50 = 0;
char                        *D_L14_00160040 = 0;
int                          D_L14_00160014 = 0;
ShrubClass *D_L14_001BC730   NOT_SDA = 0;
ShrubInst                   *D_L14_00160494 = 0;
ShrubInst                   *D_L14_00160498 = 0;
ShrubMatrix                 *D_L14_0016049C = 0;
TextTable                   *D_L14_0015F660 = 0;
Tfrag                       *D_L14_00160F4C = 0;
TieClass *D_L14_001C5F00     NOT_SDA = 0;
TieInst                     *D_L14_00160FC0 = 0;
TieInst                     *D_L14_00160FC4 = 0;
TieMatrix                   *D_L14_00160FC8 = 0;
char                        *D_L14_001600E4 = 0;
char                        *D_L14_00160108 = 0;
char                        *D_L14_0015F318 = 0;
char D_L14_0016CEB0          NOT_SDA = 0;
char D_L14_00174600          NOT_SDA = 0;
char D_L14_0017F3C0          NOT_SDA = 0;
char D_L14_0019BEE0          NOT_SDA = 0;
char D_L14_001ABEE0          NOT_SDA = 0;
char D_L14_001B1F80          NOT_SDA = 0;
float                        D_L14_001604A4 = 0;
float                        D_L14_00160F80 = 0;
float                        D_L14_00160FE0 = 0;
int                          D_L14_0015EF54 = 0;
int                          D_L14_0015FBC4 = 0;
int                          D_L14_0015FFF0 = 0;
int                          D_L14_00160104 = 0;
int                          D_L14_00160170 = 0;
int                          D_L14_00160174 = 0;
int                          D_L14_00160178 = 0;
int                          D_L14_0016017C = 0;
int                          D_L14_00160490 = 0;
int                          D_L14_00160F50 = 0;
int                          D_L14_00160FCC = 0;
unsigned char               *D_L14_001604A0 = 0;
unsigned char D_L14_001BC8B0 NOT_SDA = 0;
unsigned char D_L14_001C6200 NOT_SDA = 0;
u64                          D_L14_00182EC0[3] = {0};

u64 D_L14_00182ED8[3] = {0};

u64 D_L14_00182EF0[3] = {0};

s32                          D_L14_0015FFC8 = 0;
u32 D_L14_0019A180           NOT_SDA = 0;
u8 D_L14_001AF2C0            NOT_SDA = 0;
s32                          D_L14_00160010 = 0;
char                        *D_L14_0015F604 = 0;
s32                          D_L14_0015F5F0 = 0;
char D_L14_001FAAD0          NOT_SDA = 0x69;
Tbl                         *D_L15_0015F618 = 0;
int                          D_L15_001600E8 = 0;
int                          D_L15_001600F0 = 0;
int                          D_L15_001600F8 = 0;
int                          D_L15_00160100 = 0;
char                        *D_L15_0015FBC0 = 0;
char                        *D_L15_0015FB80 = 0;
int                          D_L15_0015FB50 = 0;
char                        *D_L15_00160040 = 0;
int                          D_L15_00160014 = 0;
ShrubClass *D_L15_001BC630   NOT_SDA = 0;
ShrubInst                   *D_L15_00160494 = 0;
ShrubInst                   *D_L15_00160498 = 0;
ShrubMatrix                 *D_L15_0016049C = 0;
TextTable                   *D_L15_0015F660 = 0;
Tfrag                       *D_L15_00160F4C = 0;
TieClass *D_L15_001C5E00     NOT_SDA = 0;
TieInst                     *D_L15_00160FC0 = 0;
TieInst                     *D_L15_00160FC4 = 0;
TieMatrix                   *D_L15_00160FC8 = 0;
char                        *D_L15_001600E4 = 0;
char                        *D_L15_00160108 = 0;
char                        *D_L15_0015F318 = 0;
char D_L15_0016CDB0          NOT_SDA = 0;
char D_L15_00174500          NOT_SDA = 0;
char D_L15_0017F2C0          NOT_SDA = 0;
char D_L15_0019BDE0          NOT_SDA = 0;
char D_L15_001ABDE0          NOT_SDA = 0;
char D_L15_001B1E80          NOT_SDA = 0;
float                        D_L15_001604A4 = 0;
float                        D_L15_00160F80 = 0;
float                        D_L15_00160FE0 = 0;
int                          D_L15_0015EF54 = 0;
int                          D_L15_0015FBC4 = 0;
int                          D_L15_0015FFF0 = 0;
int                          D_L15_00160104 = 0;
int                          D_L15_00160170 = 0;
int                          D_L15_00160174 = 0;
int                          D_L15_00160178 = 0;
int                          D_L15_0016017C = 0;
int                          D_L15_00160490 = 0;
int                          D_L15_00160F50 = 0;
int                          D_L15_00160FCC = 0;
unsigned char               *D_L15_001604A0 = 0;
unsigned char D_L15_001BC7B0 NOT_SDA = 0;
unsigned char D_L15_001C6100 NOT_SDA = 0;
u64                          D_L15_00182DC0[3] = {0};

u64 D_L15_00182DD8[3] = {0};

u64 D_L15_00182DF0[3] = {0};

s32                          D_L15_0015FFC8 = 0;
u32 D_L15_0019A080           NOT_SDA = 0;
u8 D_L15_001AF1C0            NOT_SDA = 0;
s32                          D_L15_00160010 = 0;
char                        *D_L15_0015F420 = 0;
char                        *D_L15_0015F604 = 0;
s32                          D_L15_0015F5F0 = 0;
char D_L15_001E1340          NOT_SDA = 0x69;
Tbl                         *D_L16_0015F618 = 0;
int                          D_L16_001600E8 = 0;
int                          D_L16_001600F0 = 0;
int                          D_L16_001600F8 = 0;
int                          D_L16_00160100 = 0;
char                        *D_L16_0015FBC0 = 0;
char                        *D_L16_0015FB80 = 0;
int                          D_L16_0015FB50 = 0;
char                        *D_L16_00160040 = 0;
int                          D_L16_00160014 = 0;
ShrubClass *D_L16_001BC4B0   NOT_SDA = 0;
ShrubInst                   *D_L16_00160494 = 0;
ShrubInst                   *D_L16_00160498 = 0;
ShrubMatrix                 *D_L16_0016049C = 0;
TextTable                   *D_L16_0015F660 = 0;
Tfrag                       *D_L16_00160F4C = 0;
TieClass *D_L16_001C5C80     NOT_SDA = 0;
TieInst                     *D_L16_00160FC0 = 0;
TieInst                     *D_L16_00160FC4 = 0;
TieMatrix                   *D_L16_00160FC8 = 0;
char                        *D_L16_001600E4 = 0;
char                        *D_L16_00160108 = 0;
char                        *D_L16_0015F318 = 0;
char D_L16_0016CBB0          NOT_SDA = 0;
char D_L16_00174300          NOT_SDA = 0;
char D_L16_0017F140          NOT_SDA = 0;
char D_L16_0019BC60          NOT_SDA = 0;
char D_L16_001ABC60          NOT_SDA = 0;
char D_L16_001B1D00          NOT_SDA = 0;
float                        D_L16_001604A4 = 0;
float                        D_L16_00160F80 = 0;
float                        D_L16_00160FE0 = 0;
int                          D_L16_0015EF54 = 0;
int                          D_L16_0015FBC4 = 0;
int                          D_L16_0015FFF0 = 0;
int                          D_L16_00160104 = 0;
int                          D_L16_00160170 = 0;
int                          D_L16_00160174 = 0;
int                          D_L16_00160178 = 0;
int                          D_L16_0016017C = 0;
int                          D_L16_00160490 = 0;
int                          D_L16_00160F50 = 0;
int                          D_L16_00160FCC = 0;
unsigned char               *D_L16_001604A0 = 0;
unsigned char D_L16_001BC630 NOT_SDA = 0;
unsigned char D_L16_001C5F80 NOT_SDA = 0;
u64                          D_L16_00182C40[3] = {0};

u64 D_L16_00182C58[3] = {0};

u64 D_L16_00182C70[3] = {0};

s32                          D_L16_0015FFC8 = 0;
u32 D_L16_00199F00           NOT_SDA = 0;
u8 D_L16_001AF040            NOT_SDA = 0;
s32                          D_L16_00160010 = 0;
s32 D_L16_001B0100           NOT_SDA = 0;
char                        *D_L16_0015F420 = 0;
char                        *D_L16_0015F604 = 0;
s32                          D_L16_0015F5F0 = 0;
char D_L16_001E6A50          NOT_SDA = 0x69;
Tbl                         *D_L17_0015F618 = 0;
int                          D_L17_001600E8 = 0;
int                          D_L17_001600F0 = 0;
int                          D_L17_001600F8 = 0;
int                          D_L17_00160100 = 0;
char                        *D_L17_0015FBC0 = 0;
char                        *D_L17_0015FB80 = 0;
int                          D_L17_0015FB50 = 0;
char                        *D_L17_00160040 = 0;
int                          D_L17_00160014 = 0;
ShrubClass *D_L17_001BC930   NOT_SDA = 0;
ShrubInst                   *D_L17_00160494 = 0;
ShrubInst                   *D_L17_00160498 = 0;
ShrubMatrix                 *D_L17_0016049C = 0;
TextTable                   *D_L17_0015F660 = 0;
Tfrag                       *D_L17_00160F4C = 0;
TieClass *D_L17_001C6100     NOT_SDA = 0;
TieInst                     *D_L17_00160FC0 = 0;
TieInst                     *D_L17_00160FC4 = 0;
TieMatrix                   *D_L17_00160FC8 = 0;
char                        *D_L17_001600E4 = 0;
char                        *D_L17_00160108 = 0;
char                        *D_L17_0015F318 = 0;
char D_L17_0016D0B0          NOT_SDA = 0;
char D_L17_00174800          NOT_SDA = 0;
char D_L17_0017F5C0          NOT_SDA = 0;
char D_L17_0019C0E0          NOT_SDA = 0;
char D_L17_001AC0E0          NOT_SDA = 0;
char D_L17_001B2180          NOT_SDA = 0;
float                        D_L17_001604A4 = 0;
float                        D_L17_00160F80 = 0;
float                        D_L17_00160FE0 = 0;
int                          D_L17_0015EF54 = 0;
int                          D_L17_0015FBC4 = 0;
int                          D_L17_0015FFF0 = 0;
int                          D_L17_00160104 = 0;
int                          D_L17_00160170 = 0;
int                          D_L17_00160174 = 0;
int                          D_L17_00160178 = 0;
int                          D_L17_0016017C = 0;
int                          D_L17_00160490 = 0;
int                          D_L17_00160F50 = 0;
int                          D_L17_00160FCC = 0;
unsigned char               *D_L17_001604A0 = 0;
unsigned char D_L17_001BCAB0 NOT_SDA = 0;
unsigned char D_L17_001C6400 NOT_SDA = 0;
u64                          D_L17_001830C0[3] = {0};

u64 D_L17_001830D8[3] = {0};

u64 D_L17_001830F0[3] = {0};

s32                          D_L17_0015FFC8 = 0;
u32 D_L17_0019A380           NOT_SDA = 0;
u8 D_L17_001AF4C0            NOT_SDA = 0;
s32                          D_L17_00160010 = 0;
char                        *D_L17_0015F604 = 0;
s32                          D_L17_0015F5F0 = 0;
char D_L17_001E5140          NOT_SDA = 0x69;
Tbl                         *D_L18_0015F618 = 0;
int                          D_L18_001600E8 = 0;
int                          D_L18_001600F0 = 0;
int                          D_L18_001600F8 = 0;
int                          D_L18_00160100 = 0;
char                        *D_L18_0015FBC0 = 0;
char                        *D_L18_0015FB80 = 0;
int                          D_L18_0015FB50 = 0;
char                        *D_L18_00160040 = 0;
int                          D_L18_00160014 = 0;
ShrubInst                   *D_L18_00160498 = 0;
ShrubMatrix                 *D_L18_0016049C = 0;
TextTable                   *D_L18_0015F660 = 0;
Tfrag                       *D_L18_00160F4C = 0;
TieInst                     *D_L18_00160FC4 = 0;
TieMatrix                   *D_L18_00160FC8 = 0;
char                        *D_L18_001600E4 = 0;
char                        *D_L18_0015F318 = 0;
char D_L18_0016D1B0          NOT_SDA = 0;
char D_L18_00174900          NOT_SDA = 0;
char D_L18_0017F6C0          NOT_SDA = 0;
char D_L18_0019C1E0          NOT_SDA = 0;
char D_L18_001AC1E0          NOT_SDA = 0;
char D_L18_001B2280          NOT_SDA = 0;
float                        D_L18_001604A4 = 0;
float                        D_L18_00160F80 = 0;
float                        D_L18_00160FE0 = 0;
int                          D_L18_0015EF54 = 0;
int                          D_L18_0015FBC4 = 0;
int                          D_L18_0015FFF0 = 0;
int                          D_L18_00160104 = 0;
int                          D_L18_00160170 = 0;
int                          D_L18_00160174 = 0;
int                          D_L18_00160178 = 0;
int                          D_L18_0016017C = 0;
int                          D_L18_00160490 = 0;
int                          D_L18_00160F50 = 0;
int                          D_L18_00160FCC = 0;
unsigned char               *D_L18_001604A0 = 0;
unsigned char D_L18_001BCBB0 NOT_SDA = 0;

int   D_L00_001600E8 = 0;
int   D_L00_001600F0 = 0;
int   D_L00_001600F8 = 0;
int   D_L00_00160100 = 0;
int   D_L00_0015FB50 = 0;
char *D_L00_00160108_macro __asm__("D_L00_00160108") = 0;
char *D_L00_0015F318 = 0;
int   D_L00_00160104 = 0;
int   D_L00_00160490 = 0;
int   D_L00_00160FCC = 0;
