/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"

#include "sda.h"
typedef struct {
    s32 a, b, c, off;
} Ent;
typedef struct {
    s32 n;
    s32 pad[3];
    Ent e[1];
} Tbl;
extern u8 *D_L00_00173E40 NOT_SDA;
extern Tbl *D_L00_0015F618 __attribute__((sda));
/* Sets D_L00_00173E40 from offset h[0] and turns the table at offset h[1] into pointers. */
void FUN_L00_00241728(s32 *h) {
    s32 i;
    Tbl *t;
    if (h[0])
        D_L00_00173E40 = (u8 *)h + h[0];
    if (h[1]) {
        t = (Tbl *)((u8 *)h + h[1]);
        D_L00_0015F618 = t;
        for (i = 0; i < t->n; i++) {
            t->e[i].off = (s32)((u8 *)t + t->e[i].off);
        }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00241788.s", FUN_L00_00241788);
#else
typedef struct {
    f32 id;
    f32 length;
} P_241788;

extern f32 FUN_001fa6c0(s32);
extern s32 FUN_001fa6d0(f32);
extern s32 fptodp(f32) __asm__("FUN_00120478");
extern void debug_print(void *, s32, s32) __asm__("FUN_001e93b0");
extern u8 D_L00_001E8640[];

void FUN_L00_00241788(u8 *src, s32 n) {
    P_241788 entries[512];
    s32 i;
    s32 done;
    s32 k;
    s32 j;

    for (i = 0; i < n; i++) {
        entries[i].id = (f32)*(s32 *)(src + 4);
        if (*(s32 *)(src + 0) == 0) {
            entries[i].length = 0.0f;
        } else {
            s32 next_start = *(s32 *)(src + 0x20);

            if (next_start == 0) {
                next_start = *(s32 *)(src + 0x40);
            }
            entries[i].length = FUN_001fa6c0(next_start - *(s32 *)(src + 0)) * 0.0009765625f;
            entries[i + 1].id = -1.0f;
            entries[i + 1].length = 0.0f;
        }
        src += 0x20;
    }
    do {
        done = 1;
        for (j = 0; j < n - 1; j++) {
            if (entries[j].length < entries[j + 1].length) {
                f32 id = entries[j].id;
                f32 length = entries[j].length;

                entries[j].id = entries[j + 1].id;
                entries[j].length = entries[j + 1].length;
                entries[j + 1].id = id;
                entries[j + 1].length = length;
                done = 0;
            }
        }
    } while (done == 0);
    for (k = 0; k < n; k++) {
        s32 entry_id = FUN_001fa6d0(entries[k].id);
        s32 entry_length = fptodp(entries[k].length);

        debug_print(D_L00_001E8640, entry_id, entry_length);
    }

}
#endif
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
    char pad0[4];
    LevelHeader *f4;
    char pad8[0x10];
    char *f18;
} LevelLoad173E00;

typedef struct {
    int f0;
    char *f4;
    char *f8;
    int fC;
    int f10;
    int f14;
} LevelLoad173E40;

typedef struct {
    char pad0[0x23C];
    int f23C;
    int f240;
    int f244;
} LevelState16CB40;

typedef struct {
    int f0;
    int f4;
} PvarEntry;

typedef int Qword __attribute__((mode(TI)));

typedef struct {
    char pad0[0x2970];
    int f2970;
    int f2974;
    int f2978;
    int f297C;
} GameState137B80;

typedef struct TieMatrix {
    float m0[3];
    float fC;
    float m1[3];
    float f1C;
    float m2[3];
    float f2C;
    float m3[3];
    float f3C;
    char pad40[0x100];
    char f140[0x80];
} TieMatrix;

typedef struct TieInst {
    float f0[3];
    float fC;
    TieMatrix *f10;
    float f14;
    unsigned short f18;
    unsigned char f1A;
    unsigned char f1B;
    unsigned short f1C;
    unsigned short f1E;
} TieInst;

typedef struct {
    char pad0[0x26];
    unsigned short f26;
    TieInst *f28;
    char pad2C[4];
    float f30[3];
    float f3C;
    float f40;
} TieClass;

typedef struct ShrubMatrix {
    float m0[3];
    int fC;
    float m1[3];
    int f1C;
    float m2[3];
    int f2C;
    float m3[3];
    float f3C;
} ShrubMatrix;

typedef struct ShrubInst {
    float f0[3];
    float fC;
    float f10;
    char pad14[3];
    unsigned char f17;
    short f18;
    unsigned char f1A;
    unsigned char f1B;
    unsigned short f1C;
    unsigned short f1E;
} ShrubInst;

typedef struct {
    char pad0[0xC];
    float fC;
    char pad10[6];
    unsigned short f16;
    ShrubInst *f18;
    float *f1C;
    float f20;
} ShrubClass;

typedef struct {
    char pad0[0x24];
    float f24;
    char pad28[0x1C];
    unsigned short f44;
} MobyClass;

typedef struct MobyInst {
    char pad0[0x10];
    float f10;
    float f14;
    float f18;
    char pad1C[4];
    unsigned char f20;
    unsigned char f21;
    char pad22[2];
    MobyClass *f24;
    char pad28[4];
    float f2C;
    unsigned char f30;
    char pad31;
    unsigned short f32;
    unsigned short f34;
    unsigned short f36;
    int f38;
    char pad3C[4];
    float f40;
    float f44;
    float f48;
    char pad4C[0x28];
    void *f74;
    int f78;
    char pad7C[4];
    int f80;
    char pad84[0x22];
    short fA6;
    char padA8[8];
    unsigned char fB0;
    unsigned char fB1;
    short fB2;
    short fB4;
    short fB6;
    char padB8[0x48];
} MobyInst;

typedef struct {
    char pad0[0x36];
    unsigned short f36;
    char pad38[2];
    unsigned short f3A;
    char pad3C;
    unsigned char f3D;
    char pad3E[2];
} Tfrag;

typedef struct {
    char pad0[0x48];
    int f48;
    int f4C;
    char pad50[0x30];
} Light;

typedef struct {
    int f0;
    int pad4[3];
} TextTable;

typedef struct {
    char pad0[0x2C];
    int f2C;
} TextHeader;

typedef struct {
    float f0;
    float f4;
    float f8;
    int fC;
    float f10;
    float f14;
    float f18;
    int f1C;
} Cuboid;

typedef struct {
    char pad0[8];
    int f8;
    char padC[0x84];
} SoundInst;

typedef struct {
    char pad0[0xD90];
    int fD90;
    SoundInst *fD94;
} SoundGlobals;

typedef struct {
    Qword f0;
    char *f10;
    int f14;
    char pad18[8];
} Spline;

typedef struct {
    MobyInst *f0;
    char pad4[0x22];
    short f26;
    char pad28[8];
    int f30;
    int f34;
    int f38;
} ShipSpawn;

typedef struct {
    char pad0[0x2080];
    MobyInst *f2080;
    char pad2084[0x28C];
} HeroSpawn;

typedef struct { s32 a, b; } P2_245610;

typedef struct { u8 pad[0x12E8]; P2_245610 x[10]; P2_245610 y[1]; } Tab_245610;

extern int D_L00_001600E8  MACRO_ADDR;
extern int D_L00_001600F0  MACRO_ADDR;
extern int D_L00_001600F8  MACRO_ADDR;
extern int D_L00_00160100  MACRO_ADDR;
extern char *D_L00_0015EF80;
extern char *D_L00_0015FBC0  MACRO_ADDR;
extern char *D_L00_0015FB80  MACRO_ADDR;
extern int D_L00_0015FB50  MACRO_ADDR;
extern char *D_L00_00160040  MACRO_ADDR;
extern int D_L00_00160014  MACRO_ADDR;
extern int D_L00_0015F63C  MACRO_ADDR;
extern Cuboid *D_L00_0015EF50 MACRO_ADDR;
extern HeroSpawn D_0013F350;
extern Light D_L00_0017EEC0[];
extern LevelState16CB40 D_L00_0016CA40;
extern LevelLoad173E00 D_L00_00173E00;
/* Level load state at D_L00_00173E40; this function sets its header pointers (+0x4, +0x8) and clears +0xC..+0x14. */
extern LevelLoad173E40 level_load __asm__("D_L00_00173E40");
extern MobyInst *D_L00_0015FFD8 MACRO_ADDR;
extern MobyInst *D_L00_0015FFDC MACRO_ADDR;
extern MobyInst *D_L00_0015FFE0 MACRO_ADDR;
extern ShipSpawn D_0013E030;
extern ShrubClass *D_L00_001BC030[];
extern ShrubInst *D_L00_00160494 MACRO_ADDR;
extern ShrubInst *D_L00_00160498 MACRO_ADDR;
extern ShrubMatrix *D_L00_0016049C MACRO_ADDR;
extern SoundGlobals D_0013E550;
extern Spline *D_L00_0015F70C MACRO_ADDR;
extern TextHeader D_L00_00179410;
extern TextTable *D_L00_0015F660  MACRO_ADDR;
extern Tfrag *D_L00_00160F4C MACRO_ADDR;
extern TieClass *D_L00_001C5800[];
extern TieInst *D_L00_00160FC0 MACRO_ADDR;
extern TieInst *D_L00_00160FC4 MACRO_ADDR;
extern TieMatrix *D_L00_00160FC8 MACRO_ADDR;
extern char *D_L00_0015F310  MACRO_ADDR;
extern char *D_L00_0015FFE8_macro __asm__("D_L00_0015FFE8") MACRO_ADDR;
extern char *D_L00_001600E4 MACRO_ADDR;
extern char *D_L00_001600EC MACRO_ADDR;
extern char *D_L00_001600F4 MACRO_ADDR;
extern char *D_L00_001600FC MACRO_ADDR;
extern char *D_L00_00160108_macro __asm__("D_L00_00160108") MACRO_ADDR;
extern char *D_L00_0016016C_macro __asm__("D_L00_0016016C") MACRO_ADDR;
extern char *D_L00_0015F318  MACRO_ADDR;
extern char *D_L00_001B04B0[];
extern char *func_0020D348_m(int);
extern char D_L00_00166C80[];
extern char D_L00_0016C780[];
extern char D_L00_0016C7B0[];
extern char D_L00_00173EC0[];
extern char D_L00_00173F00[];
extern char D_L00_0017ECC0[];
extern char D_L00_0017FEC0[];
extern char D_L00_0018EB40[];
extern char D_L00_0019B7E0[];
extern char D_L00_001AB7E0[];
extern char D_L00_001B1880[];
extern char D_L00_001E8658[];
extern char D_L00_001E86A8[];
extern char D_L00_001E86E0[];
extern char D_L00_001E8708[];
extern char D_L00_001E8740[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_L00_0015F448 MACRO_ADDR;
extern float D_L00_0015F44C MACRO_ADDR;
extern float D_L00_0015F450 MACRO_ADDR;
extern float D_L00_0015F454 MACRO_ADDR;
extern float D_L00_0015F638  MACRO_ADDR;
extern float D_L00_001604A4 MACRO_ADDR;
extern float D_L00_00160F80 MACRO_ADDR;
extern float D_L00_00160FE0 MACRO_ADDR;
extern float max_float(float, float) __asm__("FUN_001f99c8");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int D_0015ED84 MACRO_ADDR;
extern int D_L00_0015EF54 MACRO_ADDR;
extern char *level_root[2] __asm__("D_0015EE4C") MACRO_ADDR;
extern int D_L00_0015F444 MACRO_ADDR;
extern int D_L00_0015F445 MACRO_ADDR;
extern int D_L00_0015F446 MACRO_ADDR;
extern int D_L00_0015F5CC MACRO_ADDR;
extern int D_L00_0015F600;
extern int D_L00_0015F710 MACRO_ADDR;
extern int D_L00_0015FBC4 MACRO_ADDR;
extern int D_L00_0015FFF0 MACRO_ADDR;
extern int D_L00_0015FFF4;
extern int D_L00_00160104 MACRO_ADDR;
extern int D_L00_00160170 MACRO_ADDR;
extern int D_L00_00160174 MACRO_ADDR;
extern int D_L00_00160178 MACRO_ADDR;
extern int D_L00_0016017C MACRO_ADDR;
extern int D_L00_00160490 MACRO_ADDR;
extern int D_L00_00160540 MACRO_ADDR;
extern int first_class_ids[] __asm__("D_L00_00160548");
extern int D_L00_00160B70[];
extern int D_L00_00160F50 MACRO_ADDR;
extern int D_L00_00160FCC MACRO_ADDR;
extern int D_L00_0016C058[];
extern int D_L00_001AB840[];
extern int D_L00_001BA4D0[];
extern int debug_print_alt() __asm__("FUN_001e93b0");
extern int FUN_L00_0024d8d0(void *, void *);
extern int FUN_L00_00284d90(int, int);
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern short D_L00_001AC780[];
extern short D_L00_001B0770[];
extern short D_L00_001B07B0[];
extern short D_L00_001B0830[];
extern u8 D_00100AE0[];
extern unsigned char *D_L00_001604A0 MACRO_ADDR;
extern unsigned char mission_state[] __asm__("D_L00_0015FC88") MACRO_ADDR;
extern unsigned char D_L00_001802C0[];
extern unsigned char D_L00_001804C0[];
extern unsigned char D_L00_001BC1B0[];
extern unsigned char D_L00_001C5B00[];
extern void enable_global_state_flag(void) __asm__("func_001F61E8");
extern void copy_memory_aligned(void *, void *, int) __asm__("FUN_001f9838");
extern void scale_vector(void *, void *, float) __asm__("FUN_001f9a80");
extern void FUN_0022a5e0(void *);
extern void light_tfrags(void *) __asm__("FUN_00234f98");
extern void light_ties(void *) __asm__("FUN_00237370");
extern void setup_level_cameras(void) __asm__("FUN_L00_001ed358");
extern void FUN_L00_00204c60(void);
extern void FUN_L00_00204f80(void);
extern void FUN_L00_0024a798(int, int);
extern void FUN_L00_00250df8(void *);
extern void FUN_L00_0028df58(short *);
extern void FUN_L00_0028e180();
extern void FUN_L00_0028e8a0(char *);
extern void FillTransferWords(void *, s32, s32);
extern void FlushCache(s32);
extern void PackDmaTag(unsigned long, unsigned long, unsigned long);
extern void PackRenderCommandFields(void *, s32, s32, s32, s32);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_blocks_16(void *, s32) __asm__("func_001F9810");
extern void copy_blocks_16_forward(void *dst, void *src, s32 size) __asm__("func_001F98D0");
extern void reset_draw_globals(void) __asm__("FUN_001f37e8");
extern void set_moby_animation(void *, s32, s32) __asm__("func_00212ED8");
extern void set_video_timing(s32) __asm__("func_00214970");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern void update_camera(void) __asm__("func_001EDAA8");
extern void update_fog(void) __asm__("func_001F2588");
extern void vu0_load_micro_program(void *) __asm__("func_002334D8");
void reset_global_state_fields(void) __asm__("func_00217020");
void set_global_state_slot(int value) __asm__("func_001160C8");
void init_moby_instance(void *moby_mem, int oClass) __asm__("FUN_0020c5f0");
extern void load(void *, int, int) __asm__("FUN_00216828");
extern GameState137B80 D_00137B80;

char *load_level_gameplay(int arg0) __asm__("FUN_L00_00241940");
extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
extern unsigned char D_L00_001BB684[];
extern f32 frame_scale __asm__("D_0015ED60") MACRO_ADDR;
extern s32 pal_mode __asm__("D_0015ED80") MACRO_ADDR;
extern s32 game_language __asm__("D_0015ED88") MACRO_ADDR;
char *load_level_gameplay(int arg0) {
    LevelHeader *hdr;
    char *mem;
    float *ship;
    short *idx = D_L00_001AC780; /* moby index by file order; a pointer local, so its address is formed at each use */
    int tmp; /* scratch int: the level number in the moby loop, a group's offset and end flag in the group loop */

    hdr = D_L00_00173E00.f4;
    mem = D_L00_00173E00.f18;
    if (arg0 == 0) {
        if (pal_mode != 0) {
            load(mem, D_00137B80.f2978, D_00137B80.f297C);
        } else {
            load(mem, D_00137B80.f2970, D_00137B80.f2974);
        }
        level_root[1] = mem; /* D_0015EE50 */
    }
    FlushCache(0);
    FUN_L00_0024d8d0(level_root[1], hdr);
    FlushCache(0);
    FillTransferWords(&D_0013F350, 0, 0x2310);
    FillTransferWords(D_L00_00166C80, 0, 0x3A0);
    FillTransferWords(D_L00_0018EB40, 0, 0x180);
    set_global_state_slot(0x4D2);
    if (pal_mode != 0) {
        if (frame_scale == 1.0f) {
            set_video_timing(1);
        }
    } else {
        if (frame_scale != 1.0f) {
            set_video_timing(0);
        }
    }
    level_load.f4 = D_L00_00173F00;
    level_load.f8 = D_L00_00173EC0;
    level_load.fC = 0;
    level_load.f10 = 0;
    level_load.f14 = 0;
    clear_blocks_16(D_L00_0019B7E0, 0x10000);
    clear_blocks_16(D_L00_001AB7E0, 0x60);
    {
        int *p = (int *)((char *)hdr + hdr->f0);
        int *c;

        D_L00_0016CA40.f23C = *p++;
        D_L00_0016CA40.f240 = *p++;
        D_L00_0016CA40.f244 = *p++;
        *(unsigned char *)&D_L00_0015F444 = *p++;
        *(unsigned char *)&D_L00_0015F445 = *p++;
        *(unsigned char *)&D_L00_0015F446 = *p++;
        ship = (float *)(p + 5);
        c = p + 9;
        D_L00_0015F448 = *(float *)p;
        D_L00_0015F44C = *(float *)(p + 1);
        D_L00_0015F450 = *(float *)(p + 2);
        D_L00_0015F454 = *(float *)(p + 3);
        D_L00_0015F638 = *(float *)(p + 4);
        p += 12;
        D_0013E030.f30 = c[0];
        D_0013E030.f34 = c[1];
        D_0013E030.f38 = c[2];
    }
    update_fog();
    D_L00_00160F80 = 512000.0f;
    D_L00_00160FE0 = 720.0f;
    D_L00_001604A4 = 500.0f;
    D_L00_0015FFF0 = 500;
    D_L00_0016017C = 0x1F4000;
    PackDmaTag(D_L00_0016CA40.f23C, D_L00_0016CA40.f240, D_L00_0016CA40.f244);
    vu0_load_micro_program(D_00100AE0);
    clear_blocks_16(D_L00_001802C0, 0x100);
    clear_blocks_16(D_L00_001804C0, 0x180);
    clear_blocks_16(D_L00_0017FEC0, 0x400);
    {
        char *p = (char *)hdr + hdr->f4;
        int n = *(int *)p;

        p += 0x10;
        if (n >= 12) {
            debug_print_alt(D_L00_001E8658);
            n = 12;
        }
        if (n != 0) {
            copy_memory_aligned(D_L00_0017FEC0, p, n << 6);
        }
    }
    if (hdr->f80 != 0) {
        char *p = (char *)hdr + hdr->f80;

        D_L00_0015FBC4 = *(int *)p;
        p += 0x10;
        if (D_L00_0015FBC4 != 0) {
            Light *l;
            int i;

            copy_memory_aligned(D_L00_0017ECC0, p, D_L00_0015FBC4 << 4);
            copy_memory_aligned(D_L00_0017EEC0, p + (D_L00_0015FBC4 << 4), D_L00_0015FBC4 << 7);
            for (i = 0; i < D_L00_0015FBC4; i++) {
                l = &D_L00_0017EEC0[i];
                if (l->f48 < 0) {
                    l->f48 = 11;
                }
                if (l->f4C < 0) {
                    l->f4C = 11;
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
            int j;

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
        int lang = game_language * 4;
        char *p = (char *)hdr + *(int *)((char *)hdr + lang + 0x10);
        int size;
        int i;

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
        int n = *(int *)p;
        int i;
        int last;

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
            TieInst *inst = D_L00_00160FC0 + i;
            TieMatrix *mtx = D_L00_00160FC8 + i;
            int id;
            float scale;

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
            scale = vector_length_xyz(mtx);
            scale = max_float(scale, vector_length_xyz(mtx->m1));
            inst->fC = D_L00_001C5800[inst->f1A]->f3C * max_float(scale, vector_length_xyz(mtx->m2));
            scale_vector(inst, inst, mtx->f3C);
            add_vector_xyz(inst, inst, mtx->m3);
            D_L00_00160FC8[i].fC = 1.0f / vector_length_xyz(&D_L00_00160FC8[i]);
            D_L00_00160FC8[i].f1C = 1.0f / vector_length_xyz(D_L00_00160FC8[i].m1);
            D_L00_00160FC8[i].f2C = 1.0f / vector_length_xyz(D_L00_00160FC8[i].m2);
            copy_memory_aligned(mtx->f140, p, 0x80);
            p += 0x80;
            inst->f1C = *(unsigned short *)p;
            p += 0x10;
        }
    }
    {
        unsigned short *q = (unsigned short *)0x70000000;
        int i;

        for (i = 0; i < D_L00_00160FCC; i++) {
            *q++ = i;
        }
        *q = 0xFFFF;
        light_ties((void *)0x70000000);
    }
    {
        char *p = (char *)hdr + hdr->f3C;
        int n = *(int *)p;
        int last;
        int i;

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
        D_L00_00160B70[1] = (int)D_L00_001604A0;
        D_L00_00160498 = D_L00_00160494 + D_L00_00160490;
        for (i = 0; i < D_L00_00160490; i++) {
            ShrubInst *inst = D_L00_00160494 + i;
            ShrubMatrix *mtx = D_L00_0016049C + i;
            int cls = D_L00_001BC1B0[*(int *)p];
            float scale;

            inst->f1A = cls;
            if (cls != last) {
                last = cls;
                D_L00_001BC030[cls]->f18 = inst;
                D_L00_001BC030[cls]->f16 = 0;
            }
            D_L00_001BC030[cls]->f16++;
            inst->f18 = i;
            inst->f10 = *(float *)(p + 4);
            if (inst->f10 < 16.0f) {
                inst->f10 = 16.0f;
            }
            inst->f1B = 0;
            inst->f1E = 0xFFFF;
            if (D_L00_001BC030[cls]->f1C != 0) {
                float d;

                inst->f17 = truncate_float_to_s32(*D_L00_001BC030[cls]->f1C);
                d = convert_integer_to_float(inst->f17) + 24.0f;
                if (inst->f10 < d) {
                    inst->f10 = d;
                }
            }
            qcopy(mtx, p + 0x10);
            qcopy(mtx->m1, p + 0x20);
            qcopy(mtx->m2, p + 0x30);
            qcopy(mtx->m3, p + 0x40);
            p += 0x50;
            mtx->f3C = D_L00_001BC030[cls]->f20;
            {
                float a = (vector_length_xyz(mtx) + vector_length_xyz(mtx->m1)) * 0.5f;
                float b = vector_length_xyz(mtx->m2);
                int ia = truncate_float_to_s32(a * 4096.0f);
                int ib = truncate_float_to_s32(b * 4096.0f);

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
            transform_vector_by_basis(inst, D_L00_001BC030[cls], mtx);
            scale = vector_length_xyz(mtx);
            scale = max_float(scale, vector_length_xyz(mtx->m1));
            inst->fC = D_L00_001BC030[cls]->fC * max_float(scale, vector_length_xyz(mtx->m2));
            scale_vector(inst, inst, mtx->f3C);
            add_vector_xyz(inst, inst, mtx->m3);
        }
    }
    {
        unsigned short *q = (unsigned short *)0x70000000;
        int i;

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
            int r = 0;
            int g = 0;
            int b = 0;
            int n = 24;
            int j;

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
        int n = *(int *)p;
        int i;

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
        int n = *(int *)p;

        p += 0x10;
        D_0013E550.fD90 = n;
        if (n != 0) {
            int size = n * 0x90;
            int i;

            D_0013E550.fD94 = (SoundInst *)mem;
            mem += size;
            mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
            FillTransferWords(D_0013E550.fD94, 0, size);
            for (i = 0; i < D_0013E550.fD90; i++) {
                SoundInst *e = D_0013E550.fD94 + i;

                copy_memory_aligned(e, p, 0x90);
                p += 0x90;
                FUN_L00_0028df58((short *)e);
            }
        } else {
            D_0013E550.fD94 = 0;
        }
    }
    {
        char *p = (char *)hdr + hdr->f44;
        int n2;
        int count = 0;
        int n;
        char *next;
        int i;

        n = *(int *)p;
        p += 4;
        n2 = *(int *)p;
        p += 0xC;
        D_L00_0015FFD8 = (MobyInst *)mem;
        FillTransferWords(mem, 0, (n + n2) << 8);
        for (i = 0; i < n; i++) {
            int mission;
            int flags;
            int uid;
            int bolts;
            int bolts2;
            int b1;
            int b2;
            unsigned char vis = 0xFE;
            int skip = 0;

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
            b1 = bolts;
            b2 = b1;
            if (flags != 0) {
                tmp = D_0015ED84; /* read before the test: see the group loop */
                if (flags & 0x10) {
                    vis = FUN_L00_00284d90(tmp, uid);
                } else {
                    vis = 0xFF;
                }
                if (D_L00_001BB684[uid] != 0) {
                    skip = 1;
                } else if (flags & 3) {
                    if (mission_state[mission] == 0xFF) {
                        if (flags & 2) {
                            b1 = bolts2;
                            if ((D_L00_001BA4D0[uid >> 5] >> (uid & 0x1F)) & 1) {
                                b2 = (b1 + 1) / 2;
                            } else {
                                b2 = b1;
                            }
                        } else {
                            skip = 1;
                        }
                    } else {
                        if (flags & 1) {
                            if ((D_0014C190_257218[D_0015ED84][uid >> 5] >> (uid & 0x1F)) & 1) {
                                b2 = (bolts + 1) / 2;
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
                idx[i] = -1;
            } else {
                MobyInst *m;
                int a;
                float h;
                int pv;
                int c0;
                int c1;
                int c2;
                int upd;

                {
                    short *slot = idx + i;
                    *slot = count;
                }
                m = D_L00_0015FFD8 + count;
                init_moby_instance(m, *(int *)p);
                p += 4;
                m->fB1 = vis;
                m->fB2 = uid;
                m->fB0 = mission;
                m->fB4 = b2;
                m->fB6 = b1;
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
                h = *(float *)p;
                p += 4;
                if (a != 0) {
                    float z = probe_ground_height(&m->f10, 0, 0.5f);

                    if (0.0f < z) {
                        m->f18 = z + h;
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
                    FUN_L00_00250df8(m);
                }
                c0 = *(int *)p;
                p += 4;
                c1 = *(int *)p;
                p += 4;
                c2 = *(int *)p;
                p += 4;
                upd = *(int *)p;
                p += 4;
                m->f80 = (c2 << 16) + (c1 << 8) + c0;
                PackRenderCommandFields(m, m->f80, 0, 0, 0);
                m->f38 = upd;
                if (*(int *)p != -1) {
                    FUN_L00_0024a798((int)m, *(int *)p);
                }
                count++;
            }
            p = next;
        }
        D_L00_0015FFDC = D_L00_0015FFD8 + count;
        D_L00_0015FFDC->f20 = 0xFF;
        mem += (count + n2) << 8;
        D_L00_0015FFE8_macro = mem;
        mem += n2 << 7;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        D_L00_0015FFE0 = (MobyInst *)((int)D_L00_0015FFD8 + ((count + n2) << 8) - 0x100);
    }
    if (hdr->f58 != 0) {
        char *pv = (char *)hdr + hdr->f58;
        PvarEntry *tbl = (PvarEntry *)((char *)hdr + hdr->f54);
        int i;
        MobyInst *m;

        for (i = 0; i < D_L00_0015EF54; i++) {
            int k = D_L00_0015EF50[i].f1C;

            if (k < 0) {
                D_L00_0015EF50[i].f1C = 0;
            } else {
                int size = tbl[k].f4;
                char *src = pv + tbl[k].f0;

                D_L00_0015EF50[i].f1C = (int)mem;
                copy_blocks_16_forward(mem, src, size);
                tbl[k].f0 = (int)mem;
                mem += size;
            }
        }
        for (i = 0; i < D_0013E550.fD90; i++) {
            int k = D_0013E550.fD94[i].f8;

            if (k < 0) {
                D_0013E550.fD94[i].f8 = 0;
            } else {
                int size = tbl[k].f4;
                char *src = pv + tbl[k].f0;

                D_0013E550.fD94[i].f8 = (int)mem;
                copy_blocks_16_forward(mem, src, size);
                tbl[k].f0 = (int)mem;
                mem += size;
            }
        }
        for (m = D_L00_0015FFD8; m != D_L00_0015FFDC; m++) {
            int k = m->f78;

            if (k < 0) {
                m->f78 = 0;
            } else {
                int size = tbl[k].f4;
                char *src = pv + tbl[k].f0;

                m->f78 = (int)mem;
                copy_blocks_16_forward(mem, src, size);
                tbl[k].f0 = (int)mem;
                mem += size;
            }
        }
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
    }
    if (hdr->f50 != 0) {
        int *q = (int *)((char *)hdr + hdr->f50);
        PvarEntry *tbl = (PvarEntry *)((char *)hdr + hdr->f54);

        while (q[0] >= 0) {
            int x = tbl[q[0]].f0;
            int off = q[1];

            if (x >= (int)hdr) {
                int *r = (int *)(x + off);

                if (*r >= 0) {
                    *r = idx[*r];
                }
            }
            q += 2;
        }
    }
    if (hdr->f5C != 0) {
        int *q = (int *)((char *)hdr + hdr->f5C);
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
        int *offs;
        int n;
        int size;
        int i;

        n = *(int *)q;
        q += 4;
        size = *(int *)q;
        offs = (int *)(q + 0xC);
        mem += size;
        q = (char *)offs + (n << 2);
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (size != 0) {
            copy_blocks_16_forward(base, q, size);
        }
        FillTransferWords(D_L00_001AB840, 0, 0x1C0);
        for (i = 0; i < n; i++, offs++) {
            tmp = *offs;
            if (tmp >= 0) {
                int g = (int)(base + tmp);
                unsigned short *src;
                short *dst;

                D_L00_001AB840[i] = g;
                src = (unsigned short *)g;
                dst = (short *)src;
                D_L00_0015FFF4 = i + 1;
                do {
                    int id = *src;
                    short t;

                    tmp = *(short *)src;
                    g = (id & 0x7FFF) << 1; /* the entry's slot in the moby index, in two steps */
                    g += (int)idx;
                    tmp &= 0x8000;
                    t = *(short *)g;
                    if (t >= 0) {
                        *dst++ = t;
                    }
                    if (tmp) {
                        if (D_L00_001AB840[i] < (int)dst) {
                            dst[-1] |= 0x8000;
                        } else {
                            D_L00_001AB840[i] = 0;
                        }
                        break;
                    }
                    src++;
                } while (!tmp);
            }
        }
    }
    if (hdr->f4C != 0) {
        char *q = (char *)hdr + hdr->f4C;
        char *base = mem;
        PvarEntry *tbl;
        int size;
        int n;
        int i;

        size = *(int *)q;
        q += 4;
        n = *(int *)q;
        q += 0xC;
        mem += size;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (size != 0) {
            copy_blocks_16_forward(base, q, size);
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
        int size;
        int i;

        D_L00_00160104 = *(int *)q;
        src = q + *(int *)(q + 4);
        size = *(int *)(q + 8);
        D_L00_00160108_macro = mem;
        mem += size;
        q += 0x10;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (size != 0) {
            copy_blocks_16_forward(D_L00_00160108_macro, src, size);
        }
        for (i = 0; i < D_L00_00160104; i++) {
            D_L00_001B04B0[i] = D_L00_00160108_macro + *(int *)q;
            q += 4;
        }
    }
    if (hdr->f74 != 0) {
        char *q = (char *)hdr + hdr->f74;
        char *src;
        char *base;
        int n;
        int size;
        int i;

        n = *(int *)q;
        D_L00_0015F710 = n;
        src = q + *(int *)(q + 4);
        size = *(int *)(q + 8);
        D_L00_0015F70C = (Spline *)mem;
        mem += n << 5;
        q += 0x10;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (n > 0) {
            Spline *e = D_L00_0015F70C;

            i = n;
            do {
                qcopy(e, q);
                q += 0x10;
                e->f14 = *(int *)q;
                q += 0x10;
                e++;
            } while (--i != 0);
        }
        base = mem;
        mem += size;
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
        if (size != 0) {
            copy_blocks_16_forward(base, src, size);
        }
        for (i = 0; i < D_L00_0015F710; i++) {
            D_L00_0015F70C[i].f10 = base + *(int *)q;
            q += 4;
        }
    }
    if (hdr->f6C != 0) {
        char *q = (char *)hdr + hdr->f6C;
        int n = *(int *)q;
        int i;

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
        int n = *(int *)q;
        int i;

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
        int n = *(int *)q;
        int i;

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
        int n = *(int *)q;
        int i;

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
        int size = *(int *)q;

        D_L00_0015EF80 = mem;
        q += 0x10;
        size += 0x4000;
        mem += size;
        copy_memory_aligned(D_L00_0015EF80, q, size);
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
    }
    if (hdr->f78 != 0) {
        char *q = (char *)hdr + hdr->f78;
        int size = *(int *)q;

        D_L00_0015FBC0 = mem;
        q += 0x10;
        size += 0x4000;
        mem += size;
        copy_memory_aligned(D_L00_0015FBC0, q, size);
        mem = (char *)(((u32)mem + 0x3F) & 0xFFFFFFC0);
    }
    if (hdr->f7C != 0) {
        char *q = (char *)hdr + hdr->f7C;
        int n = *(int *)q;

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
        int n = *(int *)q;

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
    clear_blocks_16(D_L00_001B1880, 0x200);
    D_0013E030.f0 = 0;
    if (0.0f < ship[0]) {
        MobyInst *m;

        if (D_0015ED84 == 13 && D_0013E030.f26 == 2) {
            if (D_L00_00160540 != 0) {
                for (m = D_L00_0015FFD8; m->f20 != 0xFF; m++) {
                    if (m->fA6 == first_class_ids[D_0013E030.f26]) {
                        FUN_L00_0028e8a0((char *)m);
                        set_moby_animation(m, 1, 0);
                        break;
                    }
                }
            }
        }
        if (D_0013E030.f0 == 0) {
            float rot;

            m = func_0020D348_m(first_class_ids[D_0013E030.f26]);
            m->f10 = ship[0];
            m->f14 = ship[1];
            m->f18 = ship[2];
            rot = ship[3];
            m->f74 = FUN_L00_0028e180;
            m->f32 = 0xFF;
            m->f48 = rot;
            m->f30 = 0x10;
            m->f34 &= ~2;
            set_moby_animation(m, 1, 0);
            FUN_L00_00250df8(m);
            D_0013E030.f0 = m;
            FUN_L00_0024a798((int)m, 0);
        }
    }
    if (D_L00_0015F600 != 0 && hdr->f8C != 0) {
        int *q = (int *)((char *)hdr + hdr->f8C);
        int *q0;
        int n1;
        int n2;
        int n3;
        int bad1 = 1;
        int bad2;

        q0 = q;
        n1 = q[0];
        n2 = q[1];
        n3 = q[2];
        q += 4;
        if (D_L00_00160F50 == n1) {
            int i;

            bad1 = 0;
            for (i = 0; i < n1; i++) {
                if (D_L00_00160F4C[i].f3D != q[1]) {
                    bad1 = 1;
                }
                q += 2;
            }
        } else {
            q += n1 * 2;
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
            int flag = 0;
            int cnt = 0;
            int k = 0;

            for (m = D_L00_0015FFD8; m->f20 != 0xFF; m++) {
                if (m->f36 != 0x7F80) {
                    int uid = m->fB2;
                    int j;

                    k++;
                    for (j = 0; j < n3; j++) {
                        if (q[j * 2 + 1] == uid) {
                            break;
                        }
                    }
                    if (j == n3) {
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
                debug_print_alt(D_L00_001E86A8, cnt, k);
            }
        }
        if (bad1) {
            int i;

            debug_print_alt(D_L00_001E86E0);
            for (i = 0; i < D_L00_00160F50; i++) {
                D_L00_00160F4C[i].f3A = 0x7F80;
            }
        } else {
            int i;

            q = q0 + 4;
            for (i = 0; i < n1; i++) {
                int w = q[0];

                D_L00_00160F4C[i].f3A = ((w >> 3) << 8) | (1 << (w & 7));
                q += 2;
            }
        }
        if (bad2) {
            TieInst *t;
            int cnt = 0;
            int k = 0;

            q = &q0[n1 * 2 + 4];
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
            debug_print_alt(D_L00_001E8708, cnt, k);
        } else {
            int i;

            q = &q0[n1 * 2 + 4];
            for (i = 0; i < n2; i++) {
                int w = q[0];

                D_L00_00160FC0[i].f18 = ((w >> 3) << 8) | (1 << (w & 7));
                q += 2;
            }
        }
        D_L00_0016C058[7] = 2;
    } else {
        MobyInst *m;
        TieInst *t;
        int i;

        debug_print_alt(D_L00_001E8740);
        for (m = D_L00_0015FFD8; m != D_L00_0015FFE0; m++) {
            m->f36 = 0x7F80;
        }
        for (t = D_L00_00160FC0; t != D_L00_00160FC4; t++) {
            t->f18 = 0x7F80;
        }
        for (i = 0; i < D_L00_00160F50; i++) {
            D_L00_00160F4C[i].f3A = 0x7F80;
        }
        D_L00_0016C058[7] = 0;
    }
    reset_draw_globals();
    FUN_L00_00204c60();
    FUN_L00_00204f80();
    setup_level_cameras();
    update_camera();
    reset_global_state_fields();
    enable_global_state_flag();
    D_L00_001B0770[0] = 0;
    D_L00_001B07B0[0] = 0;
    D_L00_001B0830[0] = 0;
    D_L00_0015F5CC = 0;
    D_L00_0015F63C = 0;
    if (D_0013E030.f0 != 0) {
        *(long *)&D_0013E030.f0->f38 = *(long *)&D_0013F350.f2080->f38;
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
    char *f0;     /* 0x00 */
    s32 *f4;      /* 0x04: level header */
    char pad8[4];
    char *fC;     /* 0x0C */
    char *f10;    /* 0x10 */
    char *f14;    /* 0x14: base the header's offsets are taken from */
    char *f18;    /* 0x18 */
    char *f1C;    /* 0x1C */
} Load244110;

typedef struct {
    u8 pad0[0x1A];
    s16 sound;   /* 0x1A */
    s32 time;    /* 0x1C */
} Slot244110;

typedef struct {
    u8 pad0[0xC];
    u8 seq_count;   /* 0x0C */
    u8 count0D;     /* 0x0D */
    u8 pad0E[0x1A];
    Slot244110 *f28; /* 0x28 */
    u8 pad2C[0x1C];
    char *seqs[1];  /* 0x48 */
} Class244110;

typedef struct {
    s32 offset;
    u16 kind;
    u16 pad6;
    s32 pad8[2];
} TexRef244110;


typedef struct {
    s16 off;
    s16 count;
} Span244110;

extern Load244110 D_L00_00173E00_244110 __asm__("D_L00_00173E00");
extern char *D_L00_00173E40_244110[] __asm__("D_L00_00173E40");
typedef struct { s16 a; s16 b; s32 *file; } Ldg244110;
extern Ldg244110 D_0015EE48;
extern s32 current_level_index __asm__("D_0015ED84");
extern u8 D_0014C050[];
extern u8 mission_state_macro[] __asm__("D_L00_0015FC88") __attribute__((section(".sdata")));
extern s32 D_L00_0015F40C;
extern s32 sound_bank_handle __asm__("D_0015ED5C");
extern s32 D_L00_00160004;
extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");
extern u64 D_L00_001827C0[3];
extern u64 D_L00_001827D8[3];
extern u64 D_L00_001827F0[3];
extern s32 D_L00_00160F54;
extern s32 D_L00_001C4A00[];
extern s32 texture_count __asm__("D_L00_0015FFC8");
extern u32 D_L00_00199A80[];
extern u8 D_L00_001AEBC0[];
extern s32 D_L00_00160FD4;
extern s32 D_L00_001C6700[];
extern s32 D_L00_001604AC;
extern s32 D_L00_001BC5B0[];
extern char *D_L00_0015F600_244110 __asm__("D_L00_0015F600");
extern s32 D_L00_0016051C;
extern Class244110 *D_L00_00197300[];
extern s32 D_L00_0016000C;
extern s32 D_L00_00160010;
extern s32 D_L00_00160008;
extern s32 D_L00_001AFBC0[];
extern char *D_L00_001AFC20[];
extern s32 D_L00_001AFC80[];
typedef int Q244110 __attribute__((mode(TI)));
extern Q244110 D_L00_001AFCE0[];
extern u8 D_L00_00197BC0[];
extern char *D_L00_0015F420;
extern s32 D_L00_0015F438;
extern u32 D_L00_0015F43C;
extern u32 D_L00_0015F440;
extern s32 D_L00_0015FC98;
extern s32 D_L00_0015FC9C;
extern s32 D_L00_0015FCA0;
extern s32 D_L00_0015FCA4;
extern s32 D_L00_0015FCA8;
extern char *D_L00_0015F604;
extern Slot244110 *D_L00_0015F5F4;
extern s32 D_L00_0015F5F0;
extern s32 D_L00_0015FFC0;
extern s16 D_L00_00197A00[];
extern u16 D_L00_001AFE60[][16];
extern char D_L00_001E8780[], D_L00_001E87A0[], D_L00_001E87D0[], D_L00_001E87E8[],
    D_L00_001E8800[], D_L00_001E8818[], D_L00_001E8830[], D_L00_001E8848[],
    D_L00_001E8860[], D_L00_001E8878[], D_L00_001E8890[], D_L00_001E88B0[],
    D_L00_001E88C8[], D_L00_001E88E0[], D_L00_001E88F8[], D_L00_001E8910[],
    D_L00_001E8928[], D_L00_001E8940[], D_L00_001E8958[], D_L00_001E8970[],
    D_L00_001E8988[], D_L00_001E89A0[], D_L00_001E89B8[], D_L00_001E89D0[],
    D_L00_001E89E8[], D_L00_001E8A00[], D_L00_001E8A18[], D_L00_001E8A30[],
    D_L00_001E8A48[];
extern char D_L00_0015FC80[];
extern char D_L00_002F0CD0[];
extern s32 D_L00_001B1A80[];
extern char *D_L00_001C5800_244110[] __asm__("D_L00_001C5800");
extern char *D_L00_001BC030_244110[] __asm__("D_L00_001BC030");
extern char *D_L00_00160FC0_244110 __asm__("D_L00_00160FC0");
extern char *D_L00_00160494_244110 __asm__("D_L00_00160494");
extern char *D_L00_0015FFD8_244110 __asm__("D_L00_0015FFD8");
extern char *D_L00_0015FFE8;
extern char *D_L00_00160108;
extern char *D_L00_0016016C;
extern s32 D_0014EE90[16];
typedef struct { u8 pad[0x10]; u32 v __attribute__((packed, aligned(1))); } Una244110;
extern u8 D_L00_00179100[];
extern s32 game_frame __asm__("D_L00_0015F5CC");

extern void load_hud_banks(void) __asm__("FUN_00202a98");
extern void fast_mem_copy(void *, void *, s32) __asm__("FUN_001f98d0");
extern void FUN_001f9838_244110(void *, void *, s32) __asm__("FUN_001f9838");
extern void upload_texture_images(void *, s32, void *) __asm__("FUN_00203120");
extern void flush_cache(s32) __asm__("FUN_00118a80");
extern s32 FUN_L00_0024d8d0_244110(void *, char *) __asm__("FUN_L00_0024d8d0");
extern s32 log2dim(s32) __asm__("FUN_001f97a0");
extern void initialize_tfrag_render_data(char *, void *) __asm__("FUN_002040e0");
extern void relocate_sky_definition(char *) __asm__("FUN_002028e0");
extern void FUN_L00_00241728_244110(char *) __asm__("FUN_L00_00241728");
extern void register_moby_class(char *, void *, void *, s32) __asm__("FUN_00203640");
extern void FUN_L00_00241788_244110(void *, s32) __asm__("FUN_L00_00241788");
extern void relocate_asset_entry_pointers(Class244110 *, s32) __asm__("FUN_002032e0");
extern void register_object_render_class(char *, void *, void *, s32) __asm__("FUN_00203730");
extern void noop_main_callback(void *, s32) __asm__("FUN_001e93e8");
extern void register_shrub_render_class(char *, void *, void *, void *, s32) __asm__("FUN_00203b08");
extern void unpack_point_records(void *, s32) __asm__("FUN_00202800");
extern void FUN_L00_0023f120(char *);
extern void FUN_L00_0023f0d8(char *);
extern void parse_particle_textures(void *, char *, void *, s32) __asm__("FUN_002026c8");
extern void printf_1e93b0(const char *, ...) __asm__("FUN_001e93b0");
extern void fill_transfer_words(void *, s32, s32) __asm__("FUN_001f97e8");
extern char *FUN_L00_0023fa00(char *, s32 *);
extern char *FUN_L00_00241940_244110(s32) __asm__("FUN_L00_00241940");
extern f32 int_to_float(s32) __asm__("FUN_001fa6c0");
extern void FUN_L00_00285008(void);
extern void clear_scene_state_buffers(void) __asm__("FUN_00226f50");
extern void FUN_L00_00264740(void);

#define GS_TEX0(tbp0, tbw, psm, tw, th, tcc, tfx, cbp, cpsm, csm, csa, cld) \
    ((u64)(tbp0) | ((u64)(tbw) << 14) | ((u64)(psm) << 20) | ((u64)(tw) << 26) \
     | ((u64)(th) << 30) | ((u64)(tcc) << 34) | ((u64)(tfx) << 35) | ((u64)(cbp) << 37) \
     | ((u64)(cpsm) << 51) | ((u64)(csm) << 55) | ((u64)(csa) << 56) | ((u64)(cld) << 61))
#define KB(x) (int_to_float((s32)(x)) * (1.0f / 1024.0f))

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
s32 FUN_L00_00244110(s32 full, s32 keep) {
    Load244110 *ld = &D_L00_00173E00_244110;
    s32 *h = ld->f4;
    s32 *src = D_0015EE48.file;
    char *base;
    s32 used;
    char *e;
    char *free;
    char *end;
    Span244110 *span;
    TexRef244110 *tex;
    TexRef244110 *t1;
    TexRef244110 *t3;
    TexRef244110 *t4;
    char *tbl;
    s32 i;
    s32 j;
    s32 n;
    s32 k;
    s32 c1, c2, c3, c4;
    s32 m;
    char *p;
    s32 sq;
    char *sp;
    char *q;
    s32 z;
    s32 ci;
    char *x;
    char *y;
    s32 a1, a2, a3;
    char *q1, *q2;
    u64 *t;

    if (full)
        load_hud_banks();
    base = ld->f14;
    if (full) {
        fast_mem_copy(h, (char *)(src[4] + (s32)src), src[5]);
        D_L00_0015F40C = 0;
        FUN_001f9838_244110(mission_state_macro, D_0014C050 + current_level_index * 16, 0x10);
        upload_texture_images((char *)(src[6] + (s32)src), h[0], (char *)h + h[1]);
        D_L00_00160004 = sound_bank_handle;
        flush_cache(0);
        used = FUN_L00_0024d8d0_244110((char *)(src[0x14] + (s32)src), base);
        D_L00_0015F40C = (s32)(base + h[0x18]);

        D_L00_001827C0[0] = GS_TEX0((gs_texture_allocation_base + h[0x24]) >> 8, 2, 0x13, 7, 7, 1, 0,
            (gs_texture_allocation_base + h[0x25]) >> 8, 0, 0, 0, 4);
        D_L00_001827C0[1] = 0x0000FFA0000000E0;
        D_L00_001827C0[2] = 0x0040000400004000;
        D_L00_001827D8[0] = GS_TEX0((gs_texture_allocation_base + h[0x26]) >> 8, 1, 0x13, 6, 6, 1, 0,
            (gs_texture_allocation_base + h[0x27]) >> 8, 0, 0, 0, 4);
        D_L00_001827D8[1] = 0x0000FFA0000000E0;
        D_L00_001827D8[2] = 0x0040000400004000;
        D_L00_001827F0[0] = GS_TEX0((gs_texture_allocation_base + h[0x30]) >> 8, 2, 0x13, 7, 7, 1, 0,
            (gs_texture_allocation_base + h[0x31]) >> 8, 0, 0, 0, 4);
        D_L00_001827F0[1] = 0x0000FFA0000000E0;
        D_L00_001827F0[2] = 0x0040000400004000;

        t1 = (TexRef244110 *)((char *)h + h[0xD]);
        D_L00_00160F54 = h[0xC];
        for (c1 = 0; c1 < D_L00_00160F54; c1++)
            D_L00_001C4A00[c1] = D_L00_0015F40C + t1[c1].offset + (log2dim(t1[c1].kind) << 28);
        tex = (TexRef244110 *)((char *)h + h[0xF]);
        texture_count = h[0xE];
        for (c2 = 0; c2 < texture_count; c2++)
            D_L00_00199A80[c2] = D_L00_0015F40C + tex[c2].offset + (log2dim(tex[c2].kind) << 28);
        fast_mem_copy(D_L00_001AEBC0, tex, texture_count * 16);
        t3 = (TexRef244110 *)((char *)h + h[0x11]);
        D_L00_00160FD4 = h[0x10];
        for (c3 = 0; c3 < D_L00_00160FD4; c3++)
            D_L00_001C6700[c3] = D_L00_0015F40C + t3[c3].offset + (log2dim(t3[c3].kind) << 28);
        t4 = (TexRef244110 *)((char *)h + h[0x13]);
        D_L00_001604AC = h[0x12];
        for (c4 = 0; c4 < D_L00_001604AC; c4++)
            D_L00_001BC5B0[c4] = D_L00_0015F40C + t4[c4].offset + (log2dim(t4[c4].kind) << 28);

        initialize_tfrag_render_data(base + h[2], (char *)h + h[0xD]);
        if (h[3])
            D_L00_0015F600_244110 = base + h[3];
        else
            D_L00_0015F600_244110 = 0;
        if (h[4])
            relocate_sky_definition(base + h[4]);
        else
            D_L00_0016051C = 0;
        FUN_L00_00241728_244110(base + h[5]);
        for (i = 0, q2 = (char *)h + h[7]; i < h[6]; i++, q2 += 0x20)
            register_moby_class(*(s32 *)q2 ? base + *(s32 *)q2 : 0, (char *)h + h[0xF], q2 + 0x10, *(s32 *)(q2 + 4));
        FUN_L00_00241788_244110((char *)h + h[7], h[7]);
        for (sq = 0, p = (char *)h + h[0x1E]; sq < D_L00_00197300[0]->seq_count; sq++, p += 4) {
            if (*(s32 *)p) {
                D_L00_00197300[0]->seqs[sq] = base + *(s32 *)p;
                relocate_asset_entry_pointers(D_L00_00197300[0], sq);
            }
        }
        D_L00_0016000C = -1;
        q = (char *)h + h[0x21];
        D_L00_00160010 = 0;
        D_L00_00160008 = h[0x20];
        for (m = 0; m < h[0x20]; m++, q += 0x10) {
            D_L00_001AFBC0[m] = *(s32 *)(q + 4);
            D_L00_001AFC20[m] = base + *(s32 *)q;
            D_L00_001AFC80[m] = *(s32 *)(q + 8);
            { s32 o = D_L00_00197BC0[*(s32 *)(q + 4)] * 32 + 0x10;
            qcopy(&D_L00_001AFCE0[m], (char *)h + h[7] + o); }
        }
        for (i = 0, q2 = (char *)h + h[9]; i < h[8]; i++, q2 += 0x20)
            register_object_render_class(base + *(s32 *)q2, (char *)h + h[0x11], q2 + 0x10, *(s32 *)(q2 + 4));
        noop_main_callback((char *)h + h[9], h[8]);
        for (i = 0, q2 = (char *)h + h[0xB]; i < h[0xA]; i++, q2 += 0x30)
            register_shrub_render_class(base + *(s32 *)q2, (char *)h + h[0x13], q2 + 0x10, q2 + 0x20, *(s32 *)(q2 + 4));
        D_L00_0015F420 = base + h[0x1A];
        unpack_point_records((char *)h + h[0x17], h[0x16]);
        if (h[0x28]) {
            FUN_L00_0023f120(base + h[0x28]);
        } else {
            D_L00_0015F438 = 0;
            D_L00_0015F43C = 0;
            D_L00_0015F440 = 0;
        }
        if (h[0x29]) {
            FUN_L00_0023f0d8(base + h[0x29]);
        } else {
            D_L00_0015FC98 = 0;
            D_L00_0015FC9C = 0;
            D_L00_0015FCA4 = 0;
            D_L00_0015FCA0 = 0;
            D_L00_0015FCA8 = 0;
        }
        if (h[0x2A])
            D_L00_0015F604 = base + h[0x2A];
        else
            D_L00_0015F604 = 0;
        {
            char *x1 = base + h[0x19];
            char *x2 = (char *)h + h[0x15];
            parse_particle_textures((char *)h + h[0x1B], x1, x2, h[0x14]);
        }

        tbl = (char *)h + h[0x1C];
        span = (Span244110 *)tbl;
        free = (char *)((u32)(base + used + 0x3F) & ~0x3F);
        n = span->count;
        D_L00_0015F5F4 = (Slot244110 *)free;
        D_L00_0015F5F0 = n;
        FUN_001f9838_244110(free, tbl + span->off, n * 32);
        span++;
        free += n * 32;
        e = tbl + span->off;
        for (a3 = 0; a3 < D_L00_0015F5F0; a3++) {
            if (D_L00_0015F5F4[a3].sound >= span->count) {
                printf_1e93b0(D_L00_001E8780);
                D_L00_0015F5F4[a3].sound = -1;
            } else {
                D_L00_0015F5F4[a3].sound = *(u16 *)(e + D_L00_0015F5F4[a3].sound * 4);
            }
            D_L00_0015F5F4[a3].time = sound_bank_handle;
        }
        end = free + 0x3F;
        span++;
        for (ci = 0; ci < D_L00_0015FFC0; ci++, span++) {
            Class244110 *cls = D_L00_00197300[ci];
            if (cls == 0) {
                for (j = 0; j < D_L00_00160008; j++)
                    if (D_L00_001AFBC0[j] == D_L00_00197A00[ci])
                        break;
                if (j < D_L00_00160008) {
                    fill_transfer_words(D_L00_001AFE60[j], -1, 0x20);
                    e = tbl + span->off;
                    for (k = 0; k < span->count && k < 15; k++, e += 4)
                        D_L00_001AFE60[j][k] = *(u16 *)e;
                }
            } else {
                if (span->count != cls->count0D)
                    printf_1e93b0(D_L00_001E87A0, D_L00_00197A00[ci]);
                e = tbl + span->off;
                for (k = 0; k < D_L00_00197300[ci]->count0D; k++, e += 4) {
                    D_L00_00197300[ci]->f28[k].sound = *(u16 *)e;
                    D_L00_00197300[ci]->f28[k].time = sound_bank_handle;
                }
            }
        }
        D_L00_00173E00_244110.f18 = FUN_L00_0023fa00((char *)((u32)end & ~0x3F), h);
    }
    D_L00_00173E00_244110.f1C = FUN_L00_00241940_244110(full);
    if (full) {
        char *top;
        printf_1e93b0(D_L00_001E87D0);
        printf_1e93b0(D_L00_001E87E8, KB(0x100000));
        top = D_L00_002F0CD0;
        printf_1e93b0(D_L00_001E8800, KB(top - 0x100000));
        printf_1e93b0(D_L00_001E8818, KB(D_L00_00173E00_244110.f0 - top));
        printf_1e93b0(D_L00_001E8830, KB(D_L00_00173E00_244110.fC - D_L00_00173E00_244110.f0));
        printf_1e93b0(D_L00_001E8848, KB(D_L00_00173E00_244110.f10 - D_L00_00173E00_244110.fC));
        printf_1e93b0(D_L00_001E8860, KB(D_L00_00173E00_244110.f14 - D_L00_00173E00_244110.f10));
        if (D_L00_0015F600_244110 != 0)
            printf_1e93b0(D_L00_001E8878, KB(D_L00_0015F600_244110 - D_L00_00173E00_244110.f14));
        else if (D_L00_0016051C != 0)
            printf_1e93b0(D_L00_001E8878, KB((char *)D_L00_0016051C - D_L00_00173E00_244110.f14));
        else
            printf_1e93b0(D_L00_001E8890);
        if (D_L00_0015F600_244110 != 0) {
            if (D_L00_0016051C != 0)
                printf_1e93b0(D_L00_001E88B0, KB(D_L00_0016051C - (s32)D_L00_0015F600_244110));
            else
                printf_1e93b0(D_L00_001E88B0, KB(D_L00_00173E40_244110[0] - D_L00_0015F600_244110));
        }
        if (h[4])
            printf_1e93b0(D_L00_001E88C8, KB(D_L00_00173E40_244110[0] - (char *)D_L00_0016051C));
        printf_1e93b0(D_L00_001E88E0, KB(D_L00_0015F40C - (s32)D_L00_00173E40_244110[0]));
        printf_1e93b0(D_L00_001E88F8, KB((D_L00_001B1A80[1] >> 4) - D_L00_0015F40C));
        printf_1e93b0(D_L00_001E8910, KB((s32)D_L00_0015F420 - (D_L00_001B1A80[1] >> 4)));
        printf_1e93b0(D_L00_001E8928, KB((char *)D_L00_00197300[0] - D_L00_0015F420));
        printf_1e93b0(D_L00_001E8940, KB(D_L00_001C5800_244110[0] - (char *)D_L00_00197300[0]));
        if (h[0xA])
            printf_1e93b0(D_L00_001E8958, KB(D_L00_001BC030_244110[0] - D_L00_001C5800_244110[0]));
        else
            printf_1e93b0(D_L00_001E8958, KB(D_L00_00160FC0_244110 - D_L00_001C5800_244110[0]));
        if (h[0xA])
            printf_1e93b0(D_L00_001E8970, KB(D_L00_00197300[0]->seqs[0] - D_L00_001BC030_244110[0]));
        printf_1e93b0(D_L00_001E8988, KB(D_L00_00160FC0_244110 - D_L00_00197300[0]->seqs[0]));
        printf_1e93b0(D_L00_001E89A0, KB(D_L00_00160494_244110 - D_L00_00160FC0_244110));
        printf_1e93b0(D_L00_001E89B8, KB(D_L00_0015FFD8_244110 - D_L00_00160494_244110));
        printf_1e93b0(D_L00_001E89D0, KB(D_L00_0015FFE8 - D_L00_0015FFD8_244110));
        printf_1e93b0(D_L00_001E89E8, KB(D_L00_00160108 - D_L00_0015FFE8));
        printf_1e93b0(D_L00_001E8A00, KB(D_L00_0016016C - D_L00_00160108));
        printf_1e93b0(D_L00_001E8A18, 128.0);
        printf_1e93b0(D_L00_0015FC80);
        printf_1e93b0(D_L00_001E8A30, int_to_float((s32)D_L00_0016016C + 0x20000) * (1.0f / 1048576.0f));
        printf_1e93b0(D_L00_0015FC80);
        {
            char *lo = D_L00_00173E00_244110.f0 - 0x2D0000;
            printf_1e93b0(D_L00_001E8A48, int_to_float(D_L00_00173E00_244110.f1C - lo) * (1.0f / 1048576.0f));
        }
        printf_1e93b0(D_L00_0015FC80);
    }
    if (keep) {
        FUN_L00_00285008();
    } else {
        clear_scene_state_buffers();
        for (z = 15; z >= 0; z--)
            D_0014EE90[z] = 0;
    }
    ((Una244110 *)D_L00_00179100)->v = 0;
    FUN_L00_00264740();
    game_frame++;
    return 0;
}
struct SectorListPair {
    s32 x[71];                  /* 0x000 */
    s32 y[77];                  /* 0x11C: used while D_0015ED80 is set */
};
struct SectorListTable {
    u8 pad0[0x2AFC];
    struct SectorListPair lists[1]; /* 0x2AFC, 0x250 apart */
};
struct StreamLoader {
    u8 pad0[0x30];
    s32 list;                   /* 0x30 */
    u8 pad34[0x28];
    u8 *buf;                    /* 0x5C */
};
extern struct SectorListTable disc_table __asm__("D_00137B80");
extern struct StreamLoader D_L00_0016C860;
extern s32 start_audio_stream_read(void *, s32, s32) __asm__("FUN_00216788");
extern void update_audio_stream_until_idle(s32) __asm__("FUN_002168a8");

/* streams entry i of the current sector list into the loader buffer */
s32 FUN_L00_002451b8(s32 i) {
    u8 *buf = D_L00_0016C860.buf;
    s32 start;
    s32 size;

    if (pal_mode != 0) {
        size = disc_table.lists[D_L00_0016C860.list].y[i + 1] - disc_table.lists[D_L00_0016C860.list].y[i];
        start = disc_table.lists[D_L00_0016C860.list].y[i];
    } else {
        size = disc_table.lists[D_L00_0016C860.list].x[i + 1] - disc_table.lists[D_L00_0016C860.list].x[i];
        start = disc_table.lists[D_L00_0016C860.list].x[i];
    }
    if (size > 0) {
        start_audio_stream_read(buf, start, size);
        update_audio_stream_until_idle(0);
    }
    return 1;
}
