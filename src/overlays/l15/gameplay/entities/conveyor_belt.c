/* Level 15 moby classes: conveyor_belt, poison_vessel, bolt_grabber, codebot_slot, ultramech_scientist. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/gameplay/entities/moby.h"

#define NOT_SDA

#define MACRO_ADDR

/* Updates a moby: initialises its vector, then applies an angle-based offset. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002E87B0), where it is exact; names translated to the US level program. */

extern float FUN_001f9dc8(float);
extern float FUN_001f9de0(float);
extern float FUN_001fa580(float, float);
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void FUN_L00_00260738(char *a, void *b, void *c, void *d);

void FUN_L15_002e73c0(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (data != 0) {
        unsigned char state = moby->state;
        switch (state) {
        case 0: {
            char *vec = data + 0x40;
            clear_u64_value(vec);
            *(int *)(data + 0x9C) |= 4;
            moby->state = 3;
            break;
        }
        case 3:
            moby->anim_speed = 1.3888889f;
            *(float *)(data + 0x40) =
                FUN_001f9dc8(FUN_001fa580(moby->rot.z, 1.5707964f)) *
                (frame_time * 2.5f);
            *(float *)(data + 0x44) =
                FUN_001f9de0(FUN_001fa580(moby->rot.z, 1.5707964f)) *
                (frame_time * 2.5f);
            *(int *)(data + 0x48) = 0;
            break;
        }
        FUN_L00_00260738(data + 0x60, data + 0x40, &moby->rot, &moby->rot);
    }
}
#include "qcopy.h"
extern void *create_moby() __asm__("FUN_0020c4f8");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002E92C8), where it is exact; names translated to the US level program. */

extern void FUN_L00_00250df8(void *, void *);

char *FUN_L15_002e7ed8(void *unused, void *vector) {
    char *moby = create_moby(0x4E9);
    if (moby != 0) {
        char *data = *(char **)(moby + 0x78);
        char *field;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0x7F;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        *(int *)(data + 0x10) = 0;
        clear_u64_value(data);
        qcopy(moby + 0x10, vector);
        field = *(char **)(moby + 0x24);
        *(int *)(moby + 0x94) = *(int *)(field + 0x10);
        FUN_L00_00250df8(moby, field);
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002E9358). */
#include "sda.h"

/* Poison vessel update (moby class 1257): tracks its target and fires its effect. */

typedef struct {
    char _pad00[0x10];
    unsigned char nframes; /* 0x10 */
} AnimSeq;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4f_A4D88;

struct MobyClass_A4D88 {
    u8 pad_0[0xC];
    u8 seq_count; /* number of entries in seqs[] (menu previews clamp the sequence to it) */
    u8 pad_D[3];
    u32 unk10; /* copied into Moby_A4D88.unk94 when a moby is (re)classed */
    u8 pad_14[0x8];
    void *unk1C; /* word table, indexed id * 4 + 4 (0024eec0) */
    struct GifEntry *gifs; /* patched by patch_moby_gifs */
    f32 scale; /* default draw scale: copied into Moby_A4D88.scale, divides it */
    s32 unk28;
    void **callbacks; /* function-pointer table, called with the moby */
    u8 pad_30[0x14];
    u16 flags; /* initial Moby_A4D88.flags */
    s16 unk46; /* class category; 5 is tested by targeting code */
    struct AnimSeq *seqs[1]; /* animation sequences, indexed by Moby_A4D88.seq */
};

struct Moby_A4D88 {
    Vec4f_A4D88 bsphere;
    Vec4f_A4D88 pos;
    u8 state; /* >= 0xFE: dead, waiting to respawn */
    u8 group; /* linked group: index into the level moby-list table D_Lxx_001ABCC0 (0xFF: none) */
    u8 unk22; /* class slot: pclass = D_L00_00197300[unk22] (crate_update) */
    u8 unk23; /* 0x40 for the smoke trail FUN_L09_00307ba8 spawns */
    struct MobyClass_A4D88 *pclass;
    struct Moby_A4D88 *next;
    f32 scale; /* draw scale (FUN_L01_002fa068 halves it, FUN_L00_00215ef8 divides by it) */
    u8 unk30; /* set to 0xFF (0x7F for beams) by spawners */
    u8 unk31; /* set to 1 by spawners */
    s16 unk32; /* set to 0xFF (0x7F for beams) by spawners */
    u16 flags;
    u16 unk36; /* set to 0x7F80 by spawners */
    u64 spawn_frame; /* frame count at which it may respawn */
    Vec4f_A4D88 rot; /* z: yaw (FUN_L00_00266448 compares it with atan2 to the hero) */
    u8 frame; /* animation frame */
    u8 prev_frame; /* frame index in prev_seq */
    u8 seq; /* animation sequence id */
    u8 prev_seq;
    f32 unk54;
    f32 unk58;
    u8 pad5C[8];
    struct Manip *manips;
    void *cur_frame_data;
    void *prev_frame_data;
    u8 unk70;
    u8 unk71; /* set to 0xFF when a moby changes class */
    u8 unk72;
    u8 unk73;
    void (*update)(struct Moby_A4D88 *moby);
    u8 *pvars;
    u8 unk7C;
    u8 pad7D;
    u8 unk7E;
    u8 unk7F; /* set to 0x17 by FUN_L09_002c5990 near D_L09_00166F40 */
    u8 pad80[0x10];
    s32 unk90;
    u32 unk94; /* set from the class header's word 0x10 */
    s32 unk98; /* set to 1 while a carrier holds the moby (FUN_L00_002c7a58) */
    u8 pad9C[8];
    u8 unkA4;
    u8 padA5;
    s16 oclass;
    u8 padA8[8];
    u8 unkB0; /* 0xB0: index into the level's D_0014C050 row (0xFF: not spawned) */
    u8 padB1;
    u16 save_id; /* index into the level collected[]/killed[] tables and save bits D_0014C190[level][id >> 5] */
    s16 unkB4;
    u8 padB6[2];
    void *unkB8; /* 0xB8: bolt source record; its byte 0xB1 is a per-level id (FUN_L00_002a6b70) */
    u8 unkBC;
    u8 padBD[3];
    Vec4f_A4D88 unkC0; /* 0xC0: first row of a matrix built from rot (euler_rotation_basis) */
    Vec4f_A4D88 unkD0;
    Vec4f_A4D88 unkE0;
    u8 padF0[0x10];
};

#include "rnc/overlay/quad.h"

typedef union Vec_2E9358 {
    OvlQuad q;
    float f[4];
} Vec_2E9358;

typedef struct Data_2E9358 {
    float f0;
    float f4;
    float f8;
    float fC;
    struct Moby_2E9358 *target; /* 0x10 */
} Data_2E9358;

typedef struct Moby_2E9358 {
    char pad0[0x10];
    OvlQuad pos; /* 0x10 */
    unsigned char state; /* 0x20 */
    char pad21[0x1F];
    OvlQuad v40; /* 0x40 */
    char pad50[0x28];
    Data_2E9358 *data; /* 0x78 */
    char pad7C[0x28];
    unsigned char bA4;
    char padA5;
    short hA6;
    char padA8[0x14];
    unsigned char bBC;
} Moby_2E9358;

typedef struct Res_2E9358 {
    char pad0[0x20];
    Moby_2E9358 *owner; /* 0x20 */
} Res_2E9358;

typedef struct Tb_2E9358 {
    char pad0[0x18];
    int f18;
    int f1C;
    OvlQuad v20;
    OvlQuad v30;
    OvlQuad v40;
} Tb_2E9358;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern float D_0015ED6C;
extern float D_L15_00161FBC __attribute__((sda));
extern int coll_line_fix(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int FUN_L00_001f0d60();
extern int FUN_L00_001ff660();
extern int FUN_L00_0025e450();
extern void *FUN_002141f8(struct Moby_A4D88 *);
extern void collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
int FUN_L00_00260400(int u0, char *a, void *b, void *c, float *v, void *out);
int FUN_L00_0028dc90(int i, int a1, int v, int k);
void send_blast_hits(u8 *self, OvlQuad *pos, u8 **list, s32 n, u8 *skip, s32 k, s32 c0, s32 c1, f32 f, f32 spd, f32 z) __asm__("FUN_L00_0025a9f8");
extern char D_L15_001744C0_2E9358[] __asm__("D_L15_00174440");
extern int D_L15_001744D8_2E9358 __asm__("D_L15_00174458") __attribute__((section(".data")));
extern int D_001414D0_2E9358 __asm__("D_001413D0") __attribute__((section(".data")));
extern s32 D_L15_00178500[] __asm__("D_L15_00178500");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern Res_2E9358 *trooper_find_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *c338_p(int) __asm__("FUN_L00_002603d0");
extern void func_L00_001FF240_2E9358(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void func_L00_001FF610_2E9358(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void func_L00_001FF4B0_2E9358(void *, void *, float) __asm__("FUN_001f9bf8");
extern int func_L00_00261478_2E9358(void *, void *, void *, void *, void *, void *) __asm__("FUN_L00_00260400");
extern void func_L00_0025F4A8_2E9358(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern int trooper_play_sound(int, int, void *, int) __asm__("FUN_L00_0028dc90");
extern int probe_world_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern void mine_blast_mobys(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");

void FUN_L15_002e7f68(Moby_2E9358 *moby) {
    Vec_2E9358 s20;
    Vec_2E9358 s30;
    Vec_2E9358 s40;
    Data_2E9358 *data = moby->data;
    char *p20 = 0;
    unsigned char state;
    int ret;
    Res_2E9358 *r;

    s20.q = moby->pos;
    clear_u64_value(&s30);
    if (coll_sphere(0.5f, &moby->pos, 1, moby)) {
        if (D_L15_001744D8_2E9358 == 0 || c338_p(D_L15_001744D8_2E9358) == 0) {
            moby->bBC = 1;
        }
    }
    r = trooper_find_hit(moby, 0x830000, 0);
    if (r != 0 && r->owner != 0 && r->owner->hA6 != moby->hA6) {
        moby->bBC = 1;
    }
    moby->bA4 = 0xFF;

    state = moby->state;
    switch (state) {
    case 0:
        data->f8 = data->f8 - D_L15_00161FBC * frame_time_sq;
        func_L00_001FF240_2E9358(&s40, &moby->pos, data);
        if (collision_line(&s20, &moby->pos, 0x10, moby, 0)) {
            if (((Tb_2E9358 *)D_L15_001744C0_2E9358)->f18 != 0 && c338_p(((Tb_2E9358 *)D_L15_001744C0_2E9358)->f18) != 0) {
                data->target = (Moby_2E9358 *)((Tb_2E9358 *)D_L15_001744C0_2E9358)->f18;
                moby->state = 1;
                data->f8 = 0.0f;
                moby->pos = ((Tb_2E9358 *)D_L15_001744C0_2E9358)->v20;
            } else if (((Tb_2E9358 *)D_L15_001744C0_2E9358)->f18 == D_001414D0_2E9358 || data->f8 < D_0015ED6C * -9.8f) {
                moby->bBC = 1;
                qcopy(&moby->pos, &((Tb_2E9358 *)D_L15_001744C0_2E9358)->v20);
                func_L00_001FF610_2E9358(&s30, data, &((Tb_2E9358 *)D_L15_001744C0_2E9358)->v40);
                func_L00_001FF4B0_2E9358(&s30, &s30, D_0015ED6C + D_0015ED6C);
            }
        }
        break;
    case 1:
        probe_ground_height(&moby->pos, 0, 0.5f);
        if (data->target != 0) {
            Moby_2E9358 *t = data->target;

            if (t->state == 0xFE || data->target->state == 0xFD) {
                moby->bBC = state;
                data->target = 0;
            } else if (D_L15_001744D8_2E9358 != 0) {
                p20 = c338_p(D_L15_001744D8_2E9358);
                if (p20 != 0) {
                    func_L00_00261478_2E9358(moby, data->target, &moby->pos, &moby->v40, &moby->pos, &moby->v40);
                    subtract_vector_xyz(&s40, &moby->pos, &s20);
                    *(OvlQuad *)data = s40.q;
                }
            }
        }
        if (p20 == 0) {
            moby->state = 0;
            data->f8 = 0.0f;
            data->target = 0;
        }
        break;
    }

    if (moby->bBC != 0) {
        func_L00_0025F4A8_2E9358(moby, data, 0, 0.0f, 0.0f, 3, 3, 5, 2.0f, 1.0f, 4.0f, 1.0f, -1, 7.0f, 0, 1, -1, 0);
        trooper_play_sound(0, 0, moby, 0x79);
        ret = probe_world_sphere(&moby->pos, 0x10, moby, 0, 2.0f);
        s40.q = moby->pos;
        mine_blast_mobys(moby, &s40, D_L15_00178500, ret, 0, 0x830001, 2, 1, 1.0f, 1.0f, 1.0f);
        mark_moby_for_removal(moby);
    }
}
/* UpdateMoby_1388 (names.tsv role). Every frame: func_001FA748 on the heading at +0x48 with
 * frame time * pi/2, and the +0x2C scale from the model's +0x24 value. State 0: runs
 * func_L00_002D80A0, then deletes the moby if flag byte 0xD is set, else raises it by 1.0
 * and goes to state 1. State 1: when the hero (D_0013E633 + 0xE9D) is within 3.0, sets
 * flags 0x41, calls func_L00_00299B68(5) and goes to state 2. State 2: once the level flag
 * is no longer 2, starts the exit sequence and goes to state 3. State 3: deletes the moby. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002EBB38), where it is exact; names translated to the US level program. */

extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern float FUN_001f9b80(void *, void *);
extern int D_L15_0015F5C4;
extern int D_L15_0015F640;
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern unsigned char D_0013D4FD[];
extern void FUN_L00_00260860(int, int);
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void FUN_L00_00298840(int);
extern void FUN_L00_002d6bf0(char *);
extern void FUN_L10_00298940(char *);
int memcard_save_data(int slot, int flags) __asm__("FUN_0020b178");

extern short D_L15_00162078 __attribute__((sda));
void FUN_L15_002ea748(struct Moby *m) {
    m->rot.z = fast_add_rotations(m->rot.z, frame_time * 1.5707964f);
    m->scale = m->pclass->scale * *(float *)&D_L15_00162078;
    switch (m->state) {
    case 0:
        FUN_L00_002d6bf0((char *)m);
        if (D_0013D4FD[0xD] != 0) {
            mark_moby_for_removal(m);
            return;
        }
        m->state = 1;
        m->pos.z = m->pos.z + 1.0f;
        break;
    case 1:
        FUN_L10_00298940((char *)m);
        if (FUN_001f9b80(&m->pos, &hero.motion.pos) < 3.0f) {
            m->flags |= 0x41;
            FUN_L00_00298840(5);
            m->state = 2;
        }
        break;
    case 2:
        if (D_L15_0015F5C4 != 2) {
            queue_dialogue_message(0x3AA3, -1);
            D_L15_0015F640 = scale_game_frames(0xB4);
            FUN_L00_00260860(0x22, 1);
            memcard_save_data(0, -1);
            m->state = 3;
        }
        break;
    case 3:
        mark_moby_for_removal(m);
        break;
    }
}
/* Per-frame update: picks a random value on spawn, then re-rolls and spawns a child when triggered. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002EBFC8), where it is exact; names translated to the US level program. */

extern char *FUN_L15_002e7ed8_c(void *, void *) __asm__("FUN_L15_002e7ed8");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int FUN_001f9740(int *);
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern short D_L15_0016207C;
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L15_002eabd8(struct Moby *moby) {
    int *data = (int *)moby->pvars;
    switch (moby->state) {
    case 0:
        data[0] = func_001FA898_r(multiply_global_factor_ed64(*(float *)(data + 1) * 60.0f));
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case 1:
        if (FUN_001f9740(data)) {
            data[0] = func_001FA898_r(multiply_global_factor_ed64(*(float *)&D_L15_0016207C * 60.0f));
            FUN_L15_002e7ed8_c(moby, &moby->pos);
        }
        break;
    }
}
#include "sda.h"

/* Builds the two rippling vertex strips around the moby (template points moved to its position, z waved by distance from the centre), then draws them twice with scrolled texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1FE0), where it is exact; names translated to the US level program. */

typedef struct {
    float f0;
    float f4;
} RippleData;

typedef struct {
    float v[2];
} P2;

typedef struct {
    P2 p[2];
} T4;

T4 D_L15_00162108 = {{{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}};
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float square_root(float) __asm__("FUN_001f9988");
extern float FUN_L00_00200260(float, float);
extern float fast_add_rotations(float, float);
extern float wrap_angle(float);
extern int D_L15_0015F5CC;
int D_L15_001620EC = 128;
int D_L15_001620F0 = 45;
int D_L15_001620F4 = 64;
extern float D_L15_001620F8 __attribute__((sda));
extern float D_L15_001620FC __attribute__((sda));
extern float D_L15_00162100 __attribute__((sda));
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern float D_L15_00162104 __attribute__((sda));
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void FUN_L00_00250320(void *, void *, void *, void *);
extern void vu1_add_gs_register(s32, unsigned long) __asm__("func_00233980");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");

int D_L15_00162080[2] = {0x94, 0x94};
extern char D_L15_001DBCF0[];
extern char D_L15_001DCAD0[];
int D_L15_00162090[2] = {(int)D_L15_001DBCF0, (int)D_L15_001DCAD0};
extern char D_L15_001DC3E0[];
extern char D_L15_001DD1C0[];
int D_L15_001620A0[2] = {(int)D_L15_001DC3E0, (int)D_L15_001DD1C0};
extern char D_L15_001DC880[];
extern char D_L15_001DD660[];
int D_L15_001620B0[2] = {(int)D_L15_001DC880, (int)D_L15_001DD660};
extern char D_L15_001D9E40[];
extern char D_L15_001DA530[];
int D_L15_001620C0[2] = {(int)D_L15_001D9E40, (int)D_L15_001DA530};
extern char D_L15_001DAD98[];
extern char D_L15_001DB238[];
int D_L15_001620D0[2] = {(int)D_L15_001DAD98, (int)D_L15_001DB238};

void FUN_L15_002eac90(struct Moby *moby) {
    RippleData *d = (RippleData *)moby->pvars;
    float scale;
    int r, g, b;
    int color;
    int i, j, m, n, q;

    scale = convert_integer_to_float(D_L15_0015F5CC) * (D_L15_00162104 * frame_time);
    vu1_add_gs_register(6, get_effect_texture(D_L15_001620F0));
    vu1_add_gs_register(0x42, ((long)D_L15_001620EC << 32) | 0x44);
    vu1_add_gs_register(8, 0);
    vu1_add_gs_register(0x14, ((long)0xFF90 << 32) | 0x260);
    d->f0 = d->f4;
    d->f4 = fast_add_rotations(d->f4,
                               360.0f / multiply_global_factor_ed64(D_L15_001620FC) * DEG_TO_RAD * frame_time);
    FUN_L00_00250320(moby, &r, &g, &b);
    color = (D_L15_001620F4 << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < D_L15_00162080[i]; j++) {
            int *e = D_L15_001620C0 + i;
            float *p = (float *)(*e + j * 12);
            float t = D_L15_00162100 *
                      fast_sin(fast_add_rotations(
                          wrap_angle(FUN_L00_00200260(square_root(p[0] * p[0] + p[1] * p[1]),
                                                      D_L15_001620F8) *
                                     6.2831855f / D_L15_001620F8),
                          d->f4));
            *(float *)(j * 12 + D_L15_00162090[i]) = p[0] + moby->pos.x;
            *(float *)(j * 12 + D_L15_00162090[i] + 4) = p[1] + moby->pos.y;
            *(float *)(j * 12 + D_L15_00162090[i] + 8) = p[2] + moby->pos.z + t;
            *(int *)(j * 4 + D_L15_001620B0[i]) = color;
        }
    }
    for (i = 0; i < 2; i++) {
        for (m = 0; m < 3; m++) {
            for (n = 0; n < D_L15_00162080[m]; n++) {
                T4 tbl = D_L15_00162108;
                *(float *)(n * 8 + D_L15_001620A0[m]) =
                    *(float *)(n * 8 + D_L15_001620D0[m]) +
                    FUN_L00_00200260(scale * tbl.p[i].v[0], 1.0f);
                *(float *)(n * 8 + D_L15_001620A0[m] + 4) =
                    *(float *)(n * 8 + D_L15_001620D0[m] + 4) +
                    FUN_L00_00200260(scale * tbl.p[i].v[1], 1.0f);
            }
        }
        font_queue_vu_state();
        for (q = 0; q < 3; q++) {
            FUN_L00_001fde98(D_L15_00162080[q], D_L15_00162090[q], D_L15_001620B0[q],
                             (void *)D_L15_001620A0[q], 1);
        }
    }
}
#include "sda.h"

/* Builds and draws the animated ring of lights around a moby: 68 vertices and colours, then two passes of 68 texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F2BD8), where it is exact; names translated to the US level program. */

typedef struct {
    float f[4];
} Quad4;

typedef struct {
    float prev;
    float angle;
} RingVars;

Quad4 D_L15_00162170 = {{1.0f, -0.62831855f, 1.0f, 0.62831855f}};
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float D_L15_0016216C __attribute__((sda));
extern float D_L15_001DDBA0[];
extern float D_L15_001DDED0[][2];
extern float D_L15_001DE200[];
extern float D_L15_001DE530[][2];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float square_root(float) __asm__("FUN_001f9988");
extern float FUN_L00_00200260(float, float);
extern float fast_add_rotations(float, float);
extern float wrap_angle(float);
extern int D_L15_0015F5CC;
int D_L15_00162120[2] = {0x44, 0};
int D_L15_00162140[2] = {(int)D_L15_001DE200, 0};
int D_L15_00162148[2] = {(int)D_L15_001DE530, 0};
extern int D_L15_001DE750[];
int D_L15_00162150[1] = {(int)D_L15_001DE750};
int D_L15_00162154 = 128;
int D_L15_00162158 = 45;
int D_L15_0016215C = 64;
extern float D_L15_00162160 __attribute__((sda));
extern float D_L15_00162164 __attribute__((sda));
extern float D_L15_00162168 __attribute__((sda));
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void FUN_L00_00250320(void *, void *, void *, void *);
void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void FUN_L15_002eb108(struct Moby *moby) {
    unsigned int i = 0;
    unsigned int off = 0;
    RingVars *data = (RingVars *)moby->pvars;
    unsigned int n;
    int j;
    int k;
    int color;
    Quad4 t;
    int r;
    int g;
    int b;
    float scale;
    Quad4 *p;
    float *o;
    scale = convert_integer_to_float(D_L15_0015F5CC) * (D_L15_0016216C * frame_time);
    vu1_add_gs_register(6, get_effect_texture(D_L15_00162158));
    vu1_add_gs_register(0x42, ((long)D_L15_00162154 << 32) | 0x44);
    vu1_add_gs_register(8, 0);
    vu1_add_gs_register(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle = fast_add_rotations(data->angle, 360.0f / multiply_global_factor_ed64(D_L15_00162164) *
                                                      DEG_TO_RAD * frame_time);
    FUN_L00_00250320(moby, &r, &g, &b);
    /* The second float of each pair in t: p->f[0] is t.f[1], p->f[2] is t.f[3]. */
    p = (Quad4 *)&t.f[1];
    color = (D_L15_0016215C << 24) | (b << 16) | (g << 8) | r;
    o = D_L15_001DE200;
    do {
        float *s = &D_L15_001DDBA0[i * 3];
        float x = s[0];
        float y = s[1];
        float rad = square_root(x * x + y * y);
        float a = FUN_L00_00200260(rad, D_L15_00162160);
        float w = wrap_angle(a * 6.2831855f / D_L15_00162160);
        float z = fast_sin(fast_add_rotations(w, data->angle));
        o[0] = s[0] + moby->pos.x;
        *(float *)((char *)D_L15_001DE200 + 4 + off) = s[1] + moby->pos.y;
        o[2] = s[2] + moby->pos.z + D_L15_00162168 * z;
        D_L15_001DE750[i] = color;
        i++;
        off += 12;
        o += 3;
    } while (i < 68);
    for (j = 0; j < 2; j++) {
        for (n = 0; n < 68; n++) {
            t = D_L15_00162170;
            D_L15_001DE530[n][0] =
                D_L15_001DDED0[n][0] + FUN_L00_00200260(scale * t.f[j * 2], 1.0f);
            D_L15_001DE530[n][1] =
                D_L15_001DDED0[n][1] + FUN_L00_00200260(scale * p->f[j * 2], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(D_L15_00162120[k], D_L15_00162140[k], D_L15_00162150[k],
                             (void *)D_L15_00162148[k], 1);
        }
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"
/* Cutscene trigger moby: waits for the hero in its clip volume, then steps through the scene stages and saves at the end. */
extern int D_L15_0015F5C4;
extern int D_L15_00161AC0 __asm__("D_L15_00161AC0");
extern unsigned char *D_L15_0015FFD8_v __asm__("D_L15_0015FFD8");
extern char *D_L15_001600EC;
extern unsigned char D_0014C050_v[] __asm__("D_0014C050");
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void start_scene_p(int) __asm__("FUN_L00_00298840");
extern void FUN_L00_002502a0_v(int) __asm__("FUN_L00_002502a0");
extern void FUN_L00_00232fe8_v(void) __asm__("FUN_L00_00232fe8");
extern void FUN_L00_00299460_v(int) __asm__("FUN_L00_00299460");
extern void FUN_L00_002330d0_v(void) __asm__("FUN_L00_002330d0");
extern void FUN_L00_00216f90_v(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void FUN_L00_002607d0_v(int) __asm__("FUN_L00_002607d0");
extern void FUN_L00_00284e50_v(void *, void *) __asm__("FUN_L00_00284e50");
extern void FUN_L00_00262d38_v(int) __asm__("FUN_L00_00262d38");
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
void FUN_L15_002eb6b8(void);

void FUN_L15_002eb4c0(struct Moby *m) {
    char *d = (char *)m->pvars;
    m->update_dist = 0x80;
    switch (m->state) {
    case 0:
        if (D_0014C050_v[m->mission + current_level_index * 16] == 0xFF)
            break;
        if (D_L15_00161AC0 != 0)
            break;
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)d) == 0)
            break;
        if (D_L15_0015FFD8_v[(*(int *)(d + 8) << 8) + 0x20] != 3)
            break;
        start_scene_p(3);
        m->state = 2;
        FUN_L00_002502a0_v(m->mission);
        FUN_L00_00232fe8_v();
        break;
    case 2:
        if (D_L15_0015F5C4 != 2) {
            FUN_L00_00299460_v(0x11);
            m->state = 1;
        }
        break;
    case 1:
        if (D_L15_0015F5C4 != 2) {
            start_scene_p(4);
            m->state = 3;
            FUN_L00_002330d0_v();
            if (*(int *)(d + 4) != -1) {
                char *o = D_L15_001600EC + (*(int *)(d + 4) << 7);
                FUN_L00_00216f90_v(o + 0x30, o + 0x70, 0, 1);
            }
        }
        break;
    case 3:
        if (D_L15_0015F5C4 != 2) {
            FUN_L00_002607d0_v(0x11);
            if (*(int *)(d + 4) != -1) {
                char *o = D_L15_001600EC + (*(int *)(d + 4) << 7);
                FUN_L00_00284e50_v(o + 0x30, o + 0x70);
            }
            FUN_L00_00262d38_v(0x11);
            memcard_save_data(0, -1);
            m->state = 4;
        }
        break;
    case 4:
        break;
    }
    if (D_L15_0015F5C4 == 2)
        enqueue_callback_list_1(FUN_L15_002eb6b8, m);
}
#define NOT_SDA __attribute__((section(".data")))

#define MACRO_ADDR __attribute__((section(".sdata")))

/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002ECAA8), where it is exact; names translated to the US level program. */

extern char *msg_string(s32) __asm__("func_001FDD10");
extern char D_L15_00162190[];
extern char D_L15_0016CE60[];
extern int sprintf(char *str, const char *fmt, ...) __asm__("FUN_00116248");
extern int strlen_alt(char *) __asm__("FUN_001166cc");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
void font_print_large(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_001f6530");
extern void font_print_large_001fb470(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_L00_001fb470");

int D_L15_00162180 = 150;
int D_L15_00162184 = 150;
int D_L15_00162188 = 0x3800ffff;
int D_L15_0016218C = 0x38800000;
void FUN_L15_002eb6b8(void) {
    char buf[0x50];
    int flag = 0;
    int val = 0;
    char *s = D_L15_0016CE60;
    int n;
    char *s2;
    char *s3;
    if (*(int *)(s + 0x30) == 3) {
        if (scale_game_frames(0xB5) < *(int *)(s + 0x34)) {
            if (*(int *)(s + 0x34) < scale_game_frames(0x104)) {
                flag = 1;
                sprintf(buf, D_L15_00162190, msg_string(0x526A));
                val = (*(int *)(s + 0x34) - scale_game_frames(0xB5)) >> 2;
            }
        }
        s2 = D_L15_0016CE60;
        if (scale_game_frames(0x1C2) < *(int *)(s2 + 0x34)) {
            if (*(int *)(s2 + 0x34) < scale_game_frames(0x280)) {
                flag = 1;
                sprintf(buf, D_L15_00162190, msg_string(0x526B));
                val = (*(int *)(s2 + 0x34) - scale_game_frames(0x1C2)) >> 2;
            }
        }
        s3 = D_L15_0016CE60;
        if (scale_game_frames(0x2EE) < *(int *)(s3 + 0x34)) {
            if (*(int *)(s3 + 0x34) < scale_game_frames(0x3A2)) {
                flag = 1;
                sprintf(buf, D_L15_00162190, msg_string(0x526C));
                val = (*(int *)(s3 + 0x34) - scale_game_frames(0x2EE)) >> 2;
            }
        }
    } else if (*(int *)(s + 0x30) == 4) {
        if (scale_game_frames(0x8FC) < *(int *)(s + 0x34)) {
            if (*(int *)(s + 0x34) < scale_game_frames(0x960)) {
                flag = 1;
                sprintf(buf, D_L15_00162190, msg_string(0x526E));
                val = (*(int *)(s + 0x34) - scale_game_frames(0x8FC)) >> 2;
            }
        }
    }
    if (flag == 0)
        return;
    n = strlen_alt(buf);
    if (val < n) {
        n = val;
    } else if (n < val) {
        if ((n - val) & 4)
            n = n - 1;
    }
    font_print_large_001fb470((void *)(D_L15_00162180 + 1),
                     (void *)(D_L15_00162184 + 1), (void *)D_L15_0016218C,
                     buf, (void *)n);
    font_print_large_001fb470((void *)D_L15_00162180, (void *)D_L15_00162184,
                     (void *)D_L15_00162188, buf, (void *)n);
}
/* Per-frame update: when the counter check is zero, builds a vector and calls the movement helper. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002ECD18), where it is exact; names translated to the US level program. */

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_0026f080(float, float, char *, float *);
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

extern short D_L15_00162198 __attribute__((sda));
int D_L15_0016219C = 3;
void FUN_L15_002eb928(struct Moby *moby) {
    float vec[4];
    float *data = (float *)moby->pvars;
    if (random_integer_below(D_L15_0016219C - 1) == 0) {
        char *pos = &moby->pos;
        clear_u64_value(vec);
        vec[2] = random_float_between(0.75f, 1.5f) * *(float *)&D_L15_00162198 * frame_time;
        FUN_L00_0026f080(random_float_between(0.06f, 0.12f) * 210000.0f, *data, pos, vec);
    }
}
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002ECDD0), where it is exact; names translated to the US level program. */


extern char *func_0020D348_m(int);
extern int FUN_L00_0025d1b8();
extern char *func_0020D348_c(int) __asm__("FUN_0020c4f8");
extern void func_L00_0025E210_u(void *) __asm__("FUN_L00_0025d1b8");
extern void upd_a(void *) __asm__("FUN_L00_00250df8");

char *FUN_L15_002eb9e0(char *pos, char *vec) {
    char tmp[32] __attribute__((aligned(16)));
    char *p = tmp;
    char *m;
    *(OvlQuad *)tmp = *(OvlQuad *)pos;
    *(OvlQuad *)(tmp + 0x10) = *(OvlQuad *)vec;
    m = func_0020D348_c(0x594);
    if (m != 0) {
        func_L00_0025E210_u(m);
        *(unsigned char *)(m + 0x30) = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        qcopy(m + 0x10, p);
        *(OvlQuad *)(m + 0x40) = *(OvlQuad *)(tmp + 0x10);
        upd_a(m);
    }
    return m;
}
/* Shows or hides every listed moby. */

extern int D_L15_001ABE40[];
extern int D_L15_0015FFD8;

void FUN_L15_002ebf28(int idx, int show) {
    short *p = (short *)D_L15_001ABE40[idx];
    if (p == 0)
        return;
    while (1) {
        unsigned char *moby = (unsigned char *)(D_L15_0015FFD8 + ((*p & 0x7FFF) << 8));
        if (show) {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
            moby[0x31] = 1;
            *(unsigned short *)(moby + 0x34) &= 0xFFFC;
        } else {
            *(int *)(moby + 0x94) = 0;
            moby[0x31] = 0;
            *(unsigned short *)(moby + 0x34) |= 3;
        }
        if (*p++ < 0)
            return;
    }
}
/* Counts the listed mobys that are missing or dying. */

extern int D_L15_001ABE40[];
extern int D_L15_0015FFD8;

int FUN_L15_002ebfb0(int idx) {
    unsigned short *p = (unsigned short *)D_L15_001ABE40[idx];
    int n = 0;
    int base;
    if (p == 0)
        return 0;
    base = D_L15_0015FFD8;
    do {
        unsigned char *moby = (unsigned char *)(base + ((p[0] & 0x7FFF) << 8));
        if (moby == 0 || moby[0x20] == 0xFE || moby[0x20] == 0xFD)
            n++;
    } while ((short)*p++ >= 0);
    return n;
}
typedef struct {
    u8 pad[0x454];
    u8 killed[1]; /* per spawn id */
} KillFlags;
extern KillFlags D_L15_001BB830;
extern unsigned int D_L15_001BAAD0[];
extern unsigned int D_L15_0014C190[];
extern int D_L15_0015ED84;
extern int D_L15_0015ED98;
extern volatile int D_L15_00184AAC __attribute__((section(".data")));
extern int D_L15_0015FFD8;
extern float D_0013F3D0[];
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_L00_001ff290(void *, void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void FUN_L15_002f78b8(int);
extern int FUN_L15_002f78e0(int);
extern int FUN_L15_002ebfb0(int);
extern void FUN_L15_002ebf28(int, int);
extern char *FUN_L15_002eb9e0(char *, char *);
extern void FUN_L00_00216f90(void *, void *, int, int);

/* Bind this call to its checked overlay instance rather than a duplicate. */
extern float scale_time(float) __asm__("FUN_L15_001f8648");

#include "rnc/gameplay/state/item_state.h"

void FUN_L15_002ec030(struct Moby *moby) {
    int *data = (int *)moby->pvars;
    float vec[4] __attribute__((aligned(16)));
    float tmp[4] __attribute__((aligned(16)));
    OvlQuad position;
    float angle;
    char *effect;
    if (data[3] != -1) {
        if (moby->state != 0) {
            subtract_vector_xyz(tmp, (char *)D_L15_0015FFD8 + data[3] * 0x100 + 0x10, data + 4);
            *(OvlQuad *)vec = *(OvlQuad *)tmp;
            FUN_L00_001ff290(tmp, &moby->pos, vec);
        }
        *(OvlQuad *)(data + 4) = *(OvlQuad *)((char *)D_L15_0015FFD8 + data[3] * 0x100 + 0x10);
    }
    switch (moby->state) {
    case 0: {
        int save_index;
        int i;
        save_index = ((int)((unsigned int)moby->save_id << 16)) >> 16;
        if (D_L15_001BB830.killed[save_index] != 0 ||
            ((*(int *)(D_L15_0015ED84 * 0x100 + (save_index >> 5) * 4 + (char *)D_L15_0014C190) >> (moby->save_id & 31) & 1) != 0)) {
            moby->state = 4;
            break;
        }
        D_L15_00184AAC = 0;
        moby->state = 1;
        for (i = 0; i < 24; i++) {
            if (data[i + 8] != -1)
                FUN_L15_002ebf28(data[i + 8], 0);
        }
        break;
    }
    case 1:
        if (alternate_item_available[2] != 0 && vector_distance(D_0013F3D0, &moby->pos) < 3.0f && *((unsigned char *)D_0013F3D0 + 0x2024) == 0) {
            FUN_L15_002f78b8(data[0]);
            moby->state = 2;
        }
        break;
    case 2:
        if (FUN_L15_002f78e0(data[0])) {
            vec[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 2.0f;
            vec[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 2.0f;
            vec[2] = 0.0f;
            FUN_L00_001ff290(&position, vec, &moby->pos);
            clear_u64_value(tmp);
            tmp[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            tmp[1] = 1.5707964f;
            data[1] = (int)FUN_L15_002eb9e0((char *)vec, (char *)tmp);
            data[2] = 0;
            vec[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 3.0f;
            vec[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 3.0f;
            vec[2] = 0.0f;
            FUN_L00_001ff290(&position, vec, &moby->pos);
            vec[2] = probe_ground_height(vec, 0, 0.5f);
            clear_u64_value(tmp);
            tmp[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            FUN_L00_00216f90(vec, tmp, 0x72, 0);
            if (data[8] != -1)
                FUN_L15_002ebf28(data[8], 1);
            moby->state = 3;
        }
        break;
    case 3:
        if (data[1] != 0) {
            *(float *)&data[2] = fast_add_rotations(*(float *)&data[2], (360.0f / scale_time(360.0f)) * 0.017453292f);
            angle = fast_cos(*(float *)&data[2]) + 1.0f;
            vec[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * angle;
            vec[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * angle;
            vec[2] = 0.0f;
            effect = (char *)data[1];
            add_vector_xyz(tmp, &moby->pos, vec);
            *(OvlQuad *)(effect + 0x10) = *(OvlQuad *)tmp;
            if (angle < 0.5f) {
                unsigned int *bits;
                bits = (unsigned int *)(((short)moby->save_id >> 5) * 4 + D_L15_0015ED84 * 0x100 + (char *)D_L15_0014C190);
                *bits |= 1u << (moby->save_id & 31);
                {
                    unsigned int *bits = &D_L15_001BAAD0[(short)moby->save_id >> 5];
                    *bits |= 1u << (moby->save_id & 31);
                }
                moby->state = 4;
            }
        }
        break;
    case 4:
        D_L15_00184AAC = 1;
        if (*(volatile int *)(data + 1) == 0) {
            vec[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            vec[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            vec[2] = 0.0f;
            FUN_L00_001ff290(&position, vec, &moby->pos);
            clear_u64_value(tmp);
            tmp[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            tmp[1] = 1.5707964f;
            *(OvlQuad *)&position = *(OvlQuad *)&moby->pos;
            data[1] = (int)FUN_L15_002eb9e0((char *)&position, (char *)tmp);
        } else {
            vec[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            vec[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            vec[2] = 0.0f;
            FUN_L00_001ff290(&position, vec, &moby->pos);
            clear_u64_value(tmp);
            tmp[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            tmp[1] = 1.5707964f;
            *(OvlQuad *)((char *)data[1] + 0x10) = *(OvlQuad *)vec;
            *(OvlQuad *)((char *)data[1] + 0x40) = *(OvlQuad *)tmp;
        }
        if (alternate_item_available[0x6D] == 0 && FUN_L15_002ebfb0(data[8]) > 9) {
            alternate_item_available[0x6D] = 1;
            D_L15_0015ED98 += 15000;
        }
        break;
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L15_002ec760.s", FUN_L15_002ec760);
#else
extern int D_L15_0015F5C4, D_L15_0015FFD8, D_L15_0015FFE4;
extern int D_L15_0015ED84;
extern char *D_L15_001600EC;
extern unsigned int D_001413DC;
extern unsigned char D_0013DD50;
extern unsigned char D_001413F4 __attribute__((section(".data")));
extern float D_0013F420, D_0013F424, D_L15_0015ED64;
extern unsigned char D_L15_0015EDB0;
extern float D_0015ED68;
extern float frame_time;
extern void FUN_L15_002ec678(void *);
extern void FUN_002499d8(void *) __asm__("FUN_L02_0025c758");
extern void FUN_00255e88(void *, void *) __asm__("FUN_L00_002668a0");
extern int FUN_00255a30(void *, void *) __asm__("FUN_L00_00266448");
extern int FUN_002489f0(int, int) __asm__("FUN_L01_0026e008");
extern int FUN_001f8658(int) __asm__("FUN_001f96f8");
extern int FUN_001f86a0(void *) __asm__("FUN_001f9740");
extern void FUN_00246e30(void *, int, int) __asm__("FUN_00212ed8");
extern void FUN_00254c50(int, int) __asm__("FUN_L01_0027a248");
extern void FUN_0023ca30(int, int) __asm__("FUN_0020b178");
extern float FUN_001f8c00(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9028(float, float) __asm__("FUN_001f9e90");
extern float FUN_001f9968(float, float) __asm__("FUN_001fa688");
extern float FUN_001f8b50(void *) __asm__("FUN_001f9af0");
extern int FUN_001f99c8(float) __asm__("FUN_001fa6d0");
extern float FUN_001f9860(float, float) __asm__("FUN_001fa580");
extern void FUN_00252348(float, float, float, void *) __asm__("FUN_00214db0");
extern void FUN_001f89f0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f8a20(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001f98a8(float, float) __asm__("FUN_001fa5c8");
extern float FUN_001f8b80(void *) __asm__("FUN_001f9b20");
extern void FUN_00251fd0(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

void FUN_L15_002ec760(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *other;
    int index;
    float near_distance, yaw, pitch, turn;
    float fast, slow;
    FUN_L15_002ec678(moby);
    index = *(int *)(data + 0x14c);
    if (index == -1) return;
    if (D_001413F4 != 2) {
        other = (char *)(D_L15_0015FFD8 + index * 0x100);
        *(int *)(other + 0x94) = 0;
        other = (char *)(D_L15_0015FFD8 + *(int *)(data + 0x14c) * 0x100);
        *(unsigned short *)(other + 0x34) |= 0x41;
    }
    if (moby->state == 9) {
        moby->update_dist = 0;
        moby->collision = 0;
        moby->flags |= 0x41;
        return;
    }
    if (D_L15_0015F5C4 == 2) {
        moby->collision = 0;
        moby->flags |= 0x41;
    } else {
        moby->collision = moby->pclass->collision;
        moby->flags &= 0xffbe;
        FUN_002499d8(moby);
    }
    switch (moby->state) {
    case 0:
        moby->update_dist = 0xff;
        moby->state = 1;
        *(float *)(data + 0xc) = 20.0f;
        FUN_00255e88(moby, data);
        if (D_0014C050_v[*(int *)(data + 0x280) + D_L15_0015ED84 * 0x10] == 0xff) {
            if (D_0013DD50 == 0) {
                moby->state = 3;
                if (D_L15_0015FFE4 != 0) {
                    other = (char *)D_L15_0015FFE4;
                    while (other != 0) {
                        if ((unsigned char)other[0xb0] == *(unsigned int *)(data + 0x280) && other != (char *)moby) {
                            if ((signed char)other[0x20] != -2 && (signed char)other[0x20] != -3)
                                mark_moby_for_removal(other);
                        }
                        other = *(char **)(other + 0x28);
                    }
                }
            } else moby->state = 9;
        }
        break;
    case 1:
        if (is_point_inside_clip_volume((void *)0x13f3d0, *(int *)(data + 0x27c)) &&
            (D_001413DC < 2 || D_001413DC == 9) && FUN_00255a30(moby, data)) {
            FUN_L00_002502a0_v(*(int *)(data + 0x280));
            moby->anim_speed = 0.0f;
            moby->state = 2;
            if (D_L15_0015FFE4 != 0) {
                other = (char *)D_L15_0015FFE4;
                while (other != 0) {
                    if ((unsigned char)other[0xb0] == *(unsigned int *)(data + 0x280) && other != (char *)moby) {
                        if ((signed char)other[0x20] != -2 && (signed char)other[0x20] != -3)
                            mark_moby_for_removal(other);
                    }
                    other = *(char **)(other + 0x28);
                }
            }
        }
        break;
    case 2:
        if (D_L15_0015F5C4 != 2) {
            moby->anim_speed = 1.0f;
            if (*(short *)(data + 4) == 0) {
                if (*(int *)(data + 0x150) != -1) {
                    other = D_L15_001600EC + *(int *)(data + 0x150) * 0x80;
                    FUN_L00_00284e50_v(other + 0x30, other + 0x70);
                }
                moby->state = 3;
            } else {
                moby->state = 9;
            }
        }
        break;
    case 3:
        if (D_001413F4 == 2) {
            index = *(int *)(data + 0x158);
            if (index != -1) {
                other = (char *)(D_L15_0015FFD8 + (index << 8));
                if (other != 0 && (signed char)other[0x20] != -2 && (signed char)other[0x20] != -3) {
                    mark_moby_for_removal(other);
                    *(int *)(data + 0x158) = -1;
                }
            }
            if (!FUN_002489f0(*(int *)(data + 0x144), 0x40) &&
                !FUN_002489f0(*(int *)(data + 0x148), 0x40)) {
                moby->state = 4;
                *(int *)(data + 0x15c) = FUN_001f8658(0x78);
            }
        }
        break;
    case 4:
        if (FUN_001f86a0(data + 0x15c)) {
            FUN_L15_00208ca8();
            FUN_00246e30((char *)D_L15_0015FFD8 + (*(int *)(data + 0x14c) << 8), 0, 0);
            *(short *)(data + 0x36) = 1;
            data[8] = 1;
            *(int *)(data + 0x140) = 1;
            FUN_L00_002607d0_v(0x10);
            FUN_L00_002502a0_v(*(int *)(data + 0x284));
            FUN_00254c50(0, 5);
            if (*(int *)(data + 0x154) != -1) {
                other = D_L15_001600EC + (*(int *)(data + 0x154) << 7);
                FUN_L00_00216f90_v(other + 0x30, other + 0x70, 0, 1);
                other = D_L15_001600EC + (*(int *)(data + 0x154) << 7);
                FUN_L00_00284e50_v(other + 0x30, other + 0x70);
            }
            moby->state = 5;
        }
        break;
    case 5: start_scene_p(1); moby->state = 6; break;
    case 6:
        if (D_L15_0015F5C4 != 2) { FUN_L00_00299460_v(0x10); moby->state = 7; }
        break;
    case 7:
        if (D_L15_0015F5C4 != 2) {
            FUN_L00_00262d38_v(0x10);
            start_scene_p(2);
            moby->state = 8;
        }
    case 8:
        FUN_0023ca30(0, -1);
        moby->state = 9;
        break;
    }
    {
    struct { OvlQuad target, pos, diff; } vecs;
    char *pos = (char *)&moby->pos;
    fast = 0.02f;
    slow = 0.3f;
    near_distance = FUN_001f8c00(pos, (void *)0x13f3d0);
    if (near_distance < 8.0f) {
        yaw = FUN_001f9028(D_0013F420 - *(float *)(pos), D_0013F424 - *(float *)((char *)moby + 0x14));
        if (FUN_001f9968(moby->rot.z, yaw) < 1.5707964f) {
            if (FUN_001f8b50((void *)0x13f450) > 0.01f)
                *(int *)(data + 0x274) = FUN_001f8658(0x78);
            else FUN_001f86a0(data + 0x274);
            goto ready;
        }
    }
    if (*(int *)(data + 0x274) != 0) {
        *(int *)(data + 0x274) = 0;
        *(OvlQuad *)(data + 0x260) = *(OvlQuad *)0x13f420;
    }
ready:
    if (FUN_001f86a0(data + 0x278)) {
        float angle = 0.017453292f;
        *(int *)(data + 0x278) = FUN_001f99c8(scale_time(random_float_between(180.0f, 300.0f)));
        turn = FUN_001f9860(moby->rot.z, random_float_between(-90.0f, 90.0f) * angle);
        FUN_00252348(6.0f, turn, random_float_between(0.0f, 30.0f) * angle, data + 0x260);
        FUN_001f89f0(data + 0x260, data + 0x260, pos);
    }
    if (*(int *)(data + 0x274) == 0) {
        vecs.target = *(OvlQuad *)(data + 0x260);
    } else {
        vecs.target = *(OvlQuad *)0x13f420;
        fast = 0.04f;
    }
    vecs.pos = *(OvlQuad *)(pos);
    ((float *)&vecs.pos)[2] += 1.0f;
    FUN_001f8a20(&vecs.diff, &vecs.target, &vecs.pos);
    yaw = FUN_001f98a8(FUN_001f9028(((float *)&vecs.diff)[0], ((float *)&vecs.diff)[1]), moby->rot.z);
    pitch = -FUN_001f9028(FUN_001f8b80(&vecs.diff), ((float *)&vecs.diff)[2]);
    if (yaw > 1.5707964f) yaw = 1.5707964f;
    else if (yaw < -1.5707964f) yaw = -1.5707964f;
    if (pitch > 0.5235988f) pitch = 0.5235988f;
    else if (pitch < -0.5235988f) pitch = -0.5235988f;
    {
    float half_yaw = yaw * 0.5f;
    *(volatile float *)(data + 0x248) = half_yaw;
    *(volatile float *)(data + 0x1c4) = pitch * 1.25f;
    *(volatile float *)(data + 0x1c8) = half_yaw;
    }
    {
    if (D_L15_0015EDB0 != 0) {
        *(float *)(data + 0x1d0) = 2.75f;
    }
    FUN_00251fd0(fast * D_L15_0015ED64, slow * D_L15_0015ED64, moby, data + 0x160, 1);
    FUN_00251fd0(fast * D_L15_0015ED64, slow * D_L15_0015ED64, moby, data + 0x1e0, 0);
    }
    }
}
#endif

typedef struct Talk_2ed398 {
    int zone[4];
    int npc[3];
    int vol1C;
    int vol20[3];
    int vol2C;
    int pad30;
    int wait;
    int hold38;
    int hold3C;
    int last;
    char pad44[0x54 - 0x44];
    int vol54;
} Talk_2ed398;
typedef struct Help_2ed398 {
    int busy;
    char p0[0x24 - 4];
    int cur;
} Help_2ed398;
typedef struct Stats_2ed398 {
    char p0[0x3DA];
    unsigned short t3DA;
    char p1[0x3E0 - 0x3DC];
    unsigned short n3E0;
    unsigned short t3E2;
    int lv3E4;
} Stats_2ed398;
typedef struct Items_2ed398 {
    char p0[0x6E];
    unsigned char f6E;
    unsigned char f6F;
} Items_2ed398;

extern int scale_game_frames_2ed398(int) __asm__("FUN_001f96f8");
extern int is_point_inside_clip_volume_2ed398(void *, int) __asm__("FUN_00214720");
extern int show_help_message_2ed398(int, int) __asm__("FUN_L00_00203908");
extern int FUN_L00_0020d568_2ed398(void) __asm__("FUN_L00_0020d568");
extern unsigned char D_0013D4DF_2ed398[] __asm__("D_0013D4DF");
extern int D_L15_00161AC0_macro __asm__("D_L15_00161AC0") __attribute__((section(".sdata")));
extern char *moby_list_2ed398 __asm__("D_L15_0015FFD8") __attribute__((section(".sdata")));
extern char D_00141680_2ed398[] __asm__("D_00141680");
extern char D_00141968_2ed398[] __asm__("D_00141968");
extern unsigned char D_0013D388_2ed398[] __asm__("D_0013D388");

extern int D_L15_0015F5A4 __attribute__((section(".sdata")));
extern char D_L15_00179A10[];
extern unsigned char D_0013D4C4[];
extern int D_0015EEA4_macro __asm__("D_0015EEA4") __attribute__((section(".sdata")));
extern s32 current_level_index_macro __asm__("D_0015ED84") __attribute__((section(".sdata")));
/* Quartu moby update (class 1469): hint and dialogue triggers from the hero's zones, with the play-time
   records of the two hint messages. */
void FUN_L15_002ed398(struct Moby *m) {
    Talk_2ed398 *t = (struct Talk_2ed398 *)m->pvars;
    int st = m->state;
    struct Moby *e;
    int i, j, found, npc, pick;

    switch (st) {
    case 0:
        m->update_dist = 0xFF;
        t->wait = scale_game_frames_2ed398(0x4B0);
        m->state = 1;
        break;
    case 1:
        if (t->npc[0] != -1) {
            e = (struct Moby *)(moby_list_2ed398 + (t->npc[0] << 8));
            if (e != 0 && e->oclass == 0x4E && e->state != 0xFE && e->state != 0xFD && e->state != 3
                && is_point_inside_clip_volume_2ed398(&hero.motion.pos, t->zone[3]) != 0
                && (hero.state.control_mode < 2u || hero.state.control_mode == 9)
                && ((Help_2ed398 *)D_L15_00179A10)->busy == 0 && ((Help_2ed398 *)D_L15_00179A10)->cur == -1) {
                if (D_0013D4DF_2ed398[0] == 0) {
                    int lim, now;
                    now = scale_game_frames_2ed398(D_0015EEA4_macro);
                    lim = now - ((Stats_2ed398 *)D_00141968_2ed398)->t3DA * 600;
                    if ((int)((float)scale_game_frames_2ed398(0x12) * 60.0f) < lim || ((Stats_2ed398 *)D_00141968_2ed398)->t3DA * 600 == 0) {
                        show_help_message_2ed398(0x3A98, 0x7B);
                    } else {
                        now = scale_game_frames_2ed398(D_0015EEA4_macro);
                        if (now / 600 > ((Stats_2ed398 *)D_00141968_2ed398)->t3DA) {
                            ((Stats_2ed398 *)D_00141968_2ed398)->t3DA = scale_game_frames_2ed398(D_0015EEA4_macro) / 600;
                        }
                    }
                } else if (((Stats_2ed398 *)D_00141968_2ed398)->n3E0 != 0) {
                    int lim, now;
                    now = scale_game_frames_2ed398(D_0015EEA4_macro);
                    lim = now - ((Stats_2ed398 *)D_00141968_2ed398)->t3E2 * 600;
                    if ((int)((float)scale_game_frames_2ed398(0x12) * 60.0f) < lim || ((Stats_2ed398 *)D_00141968_2ed398)->t3E2 * 600 == 0) {
                        show_help_message_2ed398(0x3A99, 0x7C);
                    } else {
                        now = scale_game_frames_2ed398(D_0015EEA4_macro);
                        if (now / 600 > ((Stats_2ed398 *)D_00141968_2ed398)->t3E2) {
                            ((Stats_2ed398 *)D_00141968_2ed398)->t3E2 = scale_game_frames_2ed398(D_0015EEA4_macro) / 600;
                        }
                    }
                } else {
                    int now;
                    ((Stats_2ed398 *)D_00141968_2ed398)->n3E0++;
                    now = scale_game_frames_2ed398(D_0015EEA4_macro);
                    if (now / 600 > ((Stats_2ed398 *)D_00141968_2ed398)->t3E2) {
                        ((Stats_2ed398 *)D_00141968_2ed398)->t3E2 = scale_game_frames_2ed398(D_0015EEA4_macro) / 600;
                    }
                    ((Stats_2ed398 *)D_00141968_2ed398)->lv3E4 = ((Stats_2ed398 *)D_00141968_2ed398)->lv3E4 | (1 << current_level_index_macro) | 0x80000000;
                }
            }
        }
        if (D_L15_0015F5A4 != 0 && ((Help_2ed398 *)D_L15_00179A10)->busy == 0
            && ((Help_2ed398 *)D_L15_00179A10)->cur == -1 && *(unsigned short *)(D_00141680_2ed398 + 0x730) == 0) {
            show_help_message_2ed398(0x3A9F, 0x89);
            D_L15_0015F5A4 = 0;
        }
        if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, t->vol54) != 0
            && hero.vehicle_mode == 3 && hero.unk22E0 == 0
            && D_L15_00161AC0_macro == 0 && *(unsigned short *)(D_00141680_2ed398 + 0x728) == 0) {
            show_help_message_2ed398(0x3A9E, 0x88);
        }
        pick = -1;
        npc = -1;
        found = 0;
        for (i = 0; i < 1; i++) {
            if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, t->zone[i]) != 0) {
                pick = t->zone[i];
                npc = t->npc[i];
                found = 1;
                break;
            }
        }
        if (found != 0) {
            t->hold3C = t->hold3C + 1;
        } else {
            t->hold3C = 0;
            t->last = -1;
        }
        if (npc != -1) {
            e = (struct Moby *)(moby_list_2ed398 + (npc << 8));
            if (e != 0 && e->oclass == 0x4E && e->state != 0xFE && e->state != 0xFD && e->state != 3
                && t->hold3C > t->wait && pick != -1 && pick != t->last
                && (hero.state.control_mode < 2u || hero.state.control_mode == 9)
                && ((Help_2ed398 *)D_L15_00179A10)->busy == 0 && ((Help_2ed398 *)D_L15_00179A10)->cur == -1
                && D_0013D4DF_2ed398[0] != 0) {
                t->wait = scale_game_frames_2ed398(0x708);
                if (*(int *)(D_00141680_2ed398 + 0x6D4) >= 0) {
                    show_help_message_2ed398(0x3A9A, 0x7D);
                }
                t->last = pick;
                t->hold3C = 0;
            }
        }
        if (hero.moby->seq == 0xC && hero.vehicle_mode == 2) {
            ((Items_2ed398 *)D_0013D388_2ed398)->f6F = 1;
        }
        if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, t->vol1C) != 0 && hero.vehicle_mode == 2
            && ((Items_2ed398 *)D_0013D388_2ed398)->f6F == 0 && ((Help_2ed398 *)D_L15_00179A10)->busy == 0
            && ((Help_2ed398 *)D_L15_00179A10)->cur == -1 && *(int *)(D_00141680_2ed398 + 0x6EC) >= 0) {
            show_help_message_2ed398(0x3A9C, 0x80);
        }
        if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, t->vol20[0]) != 0 && ((Help_2ed398 *)D_L15_00179A10)->busy == 0
            && ((Help_2ed398 *)D_L15_00179A10)->cur == -1 && D_0013D4C4[0] == 0
            && !(*(unsigned int *)(D_00141680_2ed398 + 0x6FC) & (1 << current_level_index_macro))) {
            show_help_message_2ed398(0x3A9D, 0x82);
        }
        pick = -1;
        found = 0;
        for (j = 0; j < 3; j++) {
            if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, t->vol20[j]) != 0) {
                found = 1;
                if (t->last != t->vol20[j]) {
                    pick = t->vol20[j];
                }
                break;
            }
        }
        if (found == 0) {
            t->hold38 = 0;
            t->last = -1;
        } else {
            t->hold38 = t->hold38 + 1;
            if (hero.state.current == 0x35) {
                t->hold38 = 0;
                t->last = -1;
            }
        }
        if (scale_game_frames_2ed398(0xE10) < t->hold38 && pick != -1 && FUN_L00_0020d568_2ed398() != 0
            && ((Help_2ed398 *)D_L15_00179A10)->busy == 0 && ((Help_2ed398 *)D_L15_00179A10)->cur == -1
            && D_0013D4C4[0] != 0) {
            show_help_message_2ed398(0x4E2E, 0x78);
            t->last = pick;
            t->hold38 = 0;
        }
        if (((Items_2ed398 *)D_0013D388_2ed398)->f6E == 0 && is_point_inside_clip_volume_2ed398(&hero.motion.pos, t->vol2C) != 0) {
            ((Items_2ed398 *)D_0013D388_2ed398)->f6E = 1;
        }
        break;
    }
}
/* Waits for state 1, then passes the slot for the level's current mode to FUN_L00_002637f8. */
struct L15MissionState {
    char pad0[0x30];
    int mode;
    char pad1[0x178 - 0x34];
    int slot[5];
};

extern int D_L15_0015F5C4;
extern struct L15MissionState mission_2edb20 __asm__("D_L15_0016CE60");
extern void FUN_L00_002637f8(int);

void FUN_L15_002edb20(struct Moby *m) {
    switch (m->state) {
    case 0:
        m->update_dist = 0xFF;
        m->state = 1;
        break;
    case 1:
        if (D_L15_0015F5C4 == 2) {
            if (mission_2edb20.mode == 1 || mission_2edb20.mode == 2 || mission_2edb20.mode == 3 ||
                mission_2edb20.mode == 4 || mission_2edb20.mode == 6) {
                int v = mission_2edb20.mode;
                int i;
                i = 0;
                if (v == 1)
                    i = 4;
                else if (v == 2 || v == 3 || v == 4)
                    i = 2;
                else if (v == 6)
                    i = 1;
                FUN_L00_002637f8(mission_2edb20.slot[i]);
            }
        }
        break;
    }
}
extern float FUN_001f9988_2edbe0(float) __asm__("FUN_001f9988");
extern float FUN_001fa6c0_2edbe0(int) __asm__("FUN_001fa6c0");
extern int FUN_001f44b8_2edbe0(int) __asm__("FUN_001f44b8");
extern void FUN_00233980_2edbe0(int, long) __asm__("FUN_00233980");
extern float FUN_001f96b0_2edbe0(float) __asm__("FUN_001f96b0");
extern float FUN_001fa580_2edbe0(float, float) __asm__("FUN_001fa580");
extern float FUN_001f9de0_2edbe0(float) __asm__("FUN_001f9de0");
extern void FUN_L00_00250320_2edbe0(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
extern float FUN_L00_00200260_2edbe0(float, float) __asm__("FUN_L00_00200260");
extern float FUN_001fa610_2edbe0(float x) __asm__("FUN_001fa610");
extern void FUN_001f76a0_2edbe0(void) __asm__("FUN_001f76a0");
extern void FUN_L00_001fde98_2edbe0(int, int, int, void *, int) __asm__("FUN_L00_001fde98");
extern float D_0015ED6C_2edbe0 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern int D_L15_0015F5CC_macro __asm__("D_L15_0015F5CC") __attribute__((section(".sdata")));
typedef struct { float f[2][2]; } Quad4_2edbe0;
typedef struct {
    float prev;
    float angle;
} RingVars_2edbe0;

extern float D_L15_001DECB0[][3];
extern float D_L15_001DE860[][3];
extern int D_L15_001DF048[];
extern float D_L15_001DEA88[][2];
extern float D_L15_001DEED8[][2];
extern Quad4_2edbe0 D_L15_00162208;
int D_L15_001621B0[1] = {0x2e};
int D_L15_001621D0[1] = {(int)D_L15_001DECB0};
int D_L15_001621D8[1] = {(int)D_L15_001DEED8};
int D_L15_001621E0[1] = {(int)D_L15_001DF048};
int D_L15_001621E4 = 128;
int D_L15_001621E8 = 45;
int D_L15_001621EC = 64;
extern float D_L15_001621F0 __attribute__((sda));
extern float D_L15_001621F4 __attribute__((sda));
extern float D_L15_001621F8 __attribute__((sda));
extern float D_L15_001621FC __attribute__((sda));
extern float D_L15_00162200 __attribute__((sda));
/* Builds and draws the animated ring of lights around a moby: 46 scaled vertices and colours, then two passes of 46 texture coordinates. */
void FUN_L15_002edbe0(struct Moby *moby) {
    RingVars_2edbe0 *data = (RingVars_2edbe0 *)moby->pvars;
    unsigned int i;
    unsigned int n;
    int j;
    int k;
    int r;
    int g;
    int b;
    int color;
    float scale;
    scale = FUN_001fa6c0_2edbe0(D_L15_0015F5CC_macro) * (D_L15_001621FC * D_0015ED6C_2edbe0);
    FUN_00233980_2edbe0(6, FUN_001f44b8_2edbe0(D_L15_001621E8));
    FUN_00233980_2edbe0(0x42, ((long)D_L15_001621E4 << 32) | 0x44);
    FUN_00233980_2edbe0(8, 0);
    FUN_00233980_2edbe0(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle = FUN_001fa580_2edbe0(data->angle, 360.0f / FUN_001f96b0_2edbe0(D_L15_001621F4) * 0.017453292f * D_0015ED6C_2edbe0);
    FUN_L00_00250320_2edbe0(moby, &r, &g, &b);
    color = (D_L15_001621EC << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 46; i++) {
        float *s = D_L15_001DE860[i];
        float x = s[0];
        float y = s[1];
        float rad = FUN_001f9988_2edbe0(x * x + y * y);
        float a = FUN_L00_00200260_2edbe0(rad, D_L15_001621F0);
        float w = FUN_001fa610_2edbe0(a * 6.2831855f / D_L15_001621F0);
        float z = FUN_001f9de0_2edbe0(FUN_001fa580_2edbe0(w, data->angle));
        D_L15_001DECB0[i][0] = D_L15_00162200 * s[0] + moby->pos.x;
        D_L15_001DECB0[i][1] = D_L15_00162200 * s[1] + moby->pos.y;
        D_L15_001DECB0[i][2] = D_L15_00162200 * s[2] + moby->pos.z + D_L15_001621F8 * z;
        D_L15_001DF048[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (n = 0; n < 46; n++) {
            Quad4_2edbe0 t = D_L15_00162208;
            D_L15_001DEED8[n][0] = D_L15_001DEA88[n][0] + FUN_L00_00200260_2edbe0(scale * t.f[j][0], 1.0f);
            D_L15_001DEED8[n][1] = D_L15_001DEA88[n][1] + FUN_L00_00200260_2edbe0(scale * t.f[j][1], 1.0f);
        }
        FUN_001f76a0_2edbe0();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98_2edbe0(D_L15_001621B0[k], D_L15_001621D0[k], D_L15_001621E0[k], (void *)D_L15_001621D8[k], 1);
        }
    }
}
extern float D_0015ED6C_2edfc0 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern int D_L15_0015F5CC_macro __asm__("D_L15_0015F5CC") __attribute__((section(".sdata")));
extern int FUN_001f44b8_2edfc0(int) __asm__("FUN_001f44b8");
extern void FUN_001f76a0_2edfc0(void) __asm__("FUN_001f76a0");
extern float FUN_001f96b0_2edfc0(float) __asm__("FUN_001f96b0");
extern float FUN_001f9988_2edfc0(float) __asm__("FUN_001f9988");
extern float FUN_001f9de0_2edfc0(float) __asm__("FUN_001f9de0");
extern float FUN_001fa580_2edfc0(float, float) __asm__("FUN_001fa580");
extern float FUN_001fa610_2edfc0(float x) __asm__("FUN_001fa610");
extern float FUN_001fa6c0_2edfc0(int) __asm__("FUN_001fa6c0");
extern void FUN_00233980_2edfc0(int, long) __asm__("FUN_00233980");
extern void FUN_L00_001fde98_2edfc0(int, int, int, void *, int) __asm__("FUN_L00_001fde98");
extern float FUN_L00_00200260_2edfc0(float, float) __asm__("FUN_L00_00200260");
extern void FUN_L00_00250320_2edfc0(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
typedef struct { float f[2][2]; } Quad4_2edfc0;
typedef struct {
    float prev;
    float angle;
} RingVars_2edfc0;

extern float D_L15_001DF940[][3];
extern float D_L15_001DF100[][3];
extern int D_L15_001E0020[];
extern float D_L15_001DF520[][2];
extern float D_L15_001DFD60[][2];
extern Quad4_2edfc0 D_L15_00162278;
int D_L15_00162220[1] = {0x58};
int D_L15_00162240[1] = {(int)D_L15_001DF940};
int D_L15_00162248[1] = {(int)D_L15_001DFD60};
int D_L15_00162250[1] = {(int)D_L15_001E0020};
int D_L15_00162254 = 128;
int D_L15_00162258 = 45;
int D_L15_0016225C = 64;
extern float D_L15_00162260 __attribute__((sda));
extern float D_L15_00162264 __attribute__((sda));
extern float D_L15_00162268 __attribute__((sda));
extern float D_L15_0016226C __attribute__((sda));
extern float D_L15_00162270 __attribute__((sda));
/* Builds and draws the animated ring of lights around a moby: 88 scaled vertices and colours, then two passes of 88 texture coordinates. */
void FUN_L15_002edfc0(struct Moby *moby) {
    RingVars_2edfc0 *data = (RingVars_2edfc0 *)moby->pvars;
    unsigned int i;
    unsigned int n;
    int j;
    int k;
    int r;
    int g;
    int b;
    int color;
    float scale;
    scale = FUN_001fa6c0_2edfc0(D_L15_0015F5CC_macro) * (D_L15_0016226C * D_0015ED6C_2edfc0);
    FUN_00233980_2edfc0(6, FUN_001f44b8_2edfc0(D_L15_00162258));
    FUN_00233980_2edfc0(0x42, ((long)D_L15_00162254 << 32) | 0x44);
    FUN_00233980_2edfc0(8, 0);
    FUN_00233980_2edfc0(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle = FUN_001fa580_2edfc0(data->angle, 360.0f / FUN_001f96b0_2edfc0(D_L15_00162264) * 0.017453292f * D_0015ED6C_2edfc0);
    FUN_L00_00250320_2edfc0(moby, &r, &g, &b);
    color = (D_L15_0016225C << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 88; i++) {
        float *s = D_L15_001DF100[i];
        float x = s[0];
        float y = s[1];
        float rad = FUN_001f9988_2edfc0(x * x + y * y);
        float a = FUN_L00_00200260_2edfc0(rad, D_L15_00162260);
        float w = FUN_001fa610_2edfc0(a * 6.2831855f / D_L15_00162260);
        float z = FUN_001f9de0_2edfc0(FUN_001fa580_2edfc0(w, data->angle));
        D_L15_001DF940[i][0] = D_L15_00162270 * s[0] + moby->pos.x;
        D_L15_001DF940[i][1] = D_L15_00162270 * s[1] + moby->pos.y;
        D_L15_001DF940[i][2] = D_L15_00162270 * s[2] + moby->pos.z + D_L15_00162268 * z;
        D_L15_001E0020[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (n = 0; n < 88; n++) {
            Quad4_2edfc0 t = D_L15_00162278;
            D_L15_001DFD60[n][0] = D_L15_001DF520[n][0] + FUN_L00_00200260_2edfc0(scale * t.f[j][0], 1.0f);
            D_L15_001DFD60[n][1] = D_L15_001DF520[n][1] + FUN_L00_00200260_2edfc0(scale * t.f[j][1], 1.0f);
        }
        FUN_001f76a0_2edfc0();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98_2edfc0(D_L15_00162220[k], D_L15_00162240[k], D_L15_00162250[k], (void *)D_L15_00162248[k], 1);
        }
    }
}
/* Tells whether the vendor menu lets this slot's item be bought (1) or not (0). */

extern char *D_L15_00167400_lui __asm__("D_L15_00167400") __attribute__((section(".data")));
extern char *D_L15_0015EF50;
extern float D_L15_0015F3FC;

int FUN_L15_002f78e0(int slot) {
    char *o;
    if (*(short *)(D_L15_00167400_lui + 0x86) != 0x13)
        return 1;
    o = *(char **)(*(char **)(*(char **)(D_L15_0015EF50 + slot * 32 + 0x1C) + 4) + 0x70) + 0x40;
    if (*(short *)(o + 0x14) == 1 && D_L15_0015F3FC != 0.0f)
        return 1;
    return *(short *)(o + 0x14) == 3;
}
/* tests several indexed conditions of a moby's entry in the table */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_002EDB50.c: func_L15_002F99F8), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int *D_L15_001B0AB0[];
extern int point_in_path_xy(float *p, float *v, int n) __asm__("FUN_L00_00259740");
extern int FUN_L00_0025fa38(void *p, int i);
extern int FUN_L00_0025faf0(float *p, int idx);

extern char *D_L15_0015EF48[] __attribute__((section(".sdata")));
int FUN_L15_002f85a8(char *moby) {
    int *e = *(int **)(D_L15_0015EF48[2] + *(short *)(moby + 0x84) * 32 + 0x1C);
    int r;
    int i;
    if (e[10] >= 0 && FUN_L00_0025fa38(D_0013E533 + 0xE9D, e[10])) {
        return 0;
    }
    if (e[3] >= 0) {
        if (is_point_inside_clip_volume(D_0013E533 + 0xE9D, e[3])) {
            return 1;
        }
    } else if (e[4] >= 0) {
        if (FUN_L00_0025fa38(D_0013E533 + 0xE9D, e[4])) {
            return 1;
        }
    } else if (e[2] >= 0) {
        if (FUN_L00_0025faf0((float *)(D_0013E533 + 0xE9D), e[2])) {
            return 1;
        }
    } else if (e[5] >= 0) {
        i = e[5];
        if (point_in_path_xy((float *)(D_0013E533 + 0xE9D), (float *)(D_L15_001B0AB0[i] + 4),
                             D_L15_001B0AB0[i][0])) {
            return 1;
        }
    }
    return 0;
}
/* tests whether the target lies within a cone of the player's axis */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_002EDB50.c: func_L15_002F9AE8), where it is exact; names translated to the US level program. */

extern char *D_L15_001600EC;
extern char D_L15_00167410[];
extern char D_L15_00167280[];
extern float fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern float FUN_001f9df8(float);
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void FUN_001f9a28(void *dst, void *a, void *b);
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001f99c0(float);

int FUN_L15_002f8698(char *moby, float a, float b) {
    float d[4];
    float p[4];
    float q[4];
    float r[4];
    char *data;
    char *vec = D_L15_00167410;
    char *g;
    float dot1;
    float dot2;
    float lenq;
    float lenr;
    float dot3;
    float den;
    char *tbl = D_L15_0015EF48[2];
    char *src;

    data = *(char **)(tbl + *(short *)(moby + 0x84) * 32 + 0x1C);
    src = D_L15_001600EC + *(int *)(data + 0x48) * 128;
    if (a == 0.0f && b == 0.0f) {
        return 1;
    }
    g = D_L15_00167280;
    FUN_001f9a28(d, src + 0x30, *(char **)(g + 0x180) + 0x30);
    dot1 = fast_vec_dot(d, (vec + 0x30));
    fast_vec_scale(p, (vec + 0x30), dot1);
    FUN_001f9a28(q, d, p);
    lenq = fast_vec_length(q);
    dot2 = fast_vec_dot(*(char **)(g + 0x180), (vec + 0x30));
    fast_vec_scale(p, (vec + 0x30), dot2);
    FUN_001f9a28(r, *(char **)(g + 0x180), p);
    lenr = fast_vec_length(r);
    if (b != 0.0f) {
        float t1 = fast_arc_tan(lenq, dot1);
        float t2 = fast_arc_tan(lenr, dot2);
        float t3 = fast_difference_between_rotations(t1, t2);
        if (b * DEG_TO_RAD < t3) {
            return 0;
        }
    }
    if (a == 0.0f) {
        return 1;
    }
    dot3 = fast_vec_dot(q, r);
    den = lenr * lenq;
    if (den == 0.0f) {
        return 0;
    }
    if (FUN_001f99c0(1.5707964f - FUN_001f9df8(dot3 / den)) < a * DEG_TO_RAD) {
        return 1;
    }
    return 0;
}
extern char *D_L15_00167400_lui __asm__("D_L15_00167400") __attribute__((section(".data")));
typedef struct { char pad[0x100]; float x; float y; } G_88e8;
extern G_88e8 D_0013C940_s __asm__("D_0013C940") __attribute__((section(".data")));
extern int ready_s(char *) __asm__("FUN_L00_002e83c0");
extern void release_s(char *) __asm__("FUN_L00_002e8388");
extern int frames_s(int) __asm__("FUN_001f96f8");
extern void cam_s(float, float, float) __asm__("FUN_L02_002f6598");
extern float itof_s(int) __asm__("FUN_001fa6c0");
extern void spin_s(void *, float, float) __asm__("FUN_L00_002e8970");
extern void pitch_s(int, float, float) __asm__("FUN_L00_002e8450");
extern void pitch2_s(void) __asm__("FUN_L00_002e8620");
extern void yaw_s(float, float) __asm__("FUN_L00_002e84b8");
extern void zoom_s(int, float, float) __asm__("FUN_L00_002e84f0");

/* Runs the aim timer while the player holds the target in view; spins the target once aimed. */
void FUN_L15_002f88e8(char *m) {
    char *s = *(char **)(D_L15_0015EF50 + *(short *)(m + 0x84) * 32 + 0x1C);
    char *o = D_L15_001600EC + (*(int *)(s + 0x48) << 7);
    int done;
    int t1;
    int t2;
    int t3;
    int t4;
    float r;
    float a;
    if (ready_s(m) <= 0) {
        *(short *)(s + 0x20) = 0;
        return;
    }
    if (FUN_L15_002f85a8(m) == 0) {
        *(short *)(s + 0x20) = 0;
        release_s(m);
        return;
    }
    *(int *)(*(char **)(D_L15_00167400_lui + 0x70) + 0x230) = 2;
    done = 1;
    (*(short *)(s + 0x20))++;
    t1 = frames_s(300);
    t2 = frames_s(400);
    t3 = frames_s(560);
    t4 = frames_s(200);
    cam_s(1.0f, 12.0f, 0.11f);
    if (D_0013C940_s.x != 0.0f)
        *(short *)(s + 0x20) = t1;
    else if (D_0013C940_s.y != 0.0f)
        *(short *)(s + 0x20) = t1;
    if (*(short *)(s + 0x20) >= t1) {
        if (++*(int *)(s + 0x3C) >= t2 && FUN_L15_002f8698(m, 30.0f, 0.0f))
            *(short *)(s + 0x20) = t3;
        else
            done = 0;
    }
    if (*(short *)(s + 0x20) >= t3) {
        done = 1;
        *(int *)(s + 0x3C) = 0;
        *(short *)(s + 0x20) = 1;
    }
    if (t4 < *(short *)(s + 0x20) && *(short *)(s + 0x20) < t1) {
        *(int *)(s + 0x3C) = t1;
        *(short *)(s + 0x20) = t4;
    }
    r = itof_s(*(short *)(s + 0x20)) / itof_s(t4);
    a = *(float *)s * DEG_TO_RAD * r;
    if (done)
        spin_s(o + 0x30, a, 0.0f);
    if (*(float *)(s + 0x34) != 0.0f) {
        pitch_s(0, *(float *)(s + 0x34), 0.003f);
        pitch2_s();
    }
    if (*(float *)(s + 0x38) != 0.0f)
        yaw_s(*(float *)(s + 0x38), 0.003f);
    if (*(float *)(s + 0x44) != 0.0f)
        zoom_s(0, *(float *)(s + 0x44), 0.005f);
}
/* Hands a vendor moby to the shop handler when the menu is idle and its slot is live. */

extern char *D_L15_00167400_lui __asm__("D_L15_00167400") __attribute__((section(".data")));
extern unsigned char D_001413F4 __attribute__((section(".data")));
void FUN_L15_002f88e8(char *m);

int FUN_L15_002f8ba8(char *moby) {
    char *e = *(char **)(D_L15_0015EF48[2] + *(short *)(moby + 0x84) * 32 + 0x1C);
    if (*(short *)(D_L15_00167400_lui + 0x86) == 0 && *(short *)(e + 0x20) >= 0 &&
        D_001413F4 == 2)
        FUN_L15_002f88e8(moby);
    return -1;
}
