/* Shared moby class taunter: the taunter held by the hero, which lures nearby enemies, and its effect quads. */

#include "types.h"
#include "rnc/overlay/point_light.h"
#include "rnc/globals.h"
#include "rnc/hero/hero.h"
#include "rnc/data_labels.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/weapons/taunter_globals.h"
#include "rnc/input/pad_globals.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    f32 x;
    f32 y;
    u8  pad[0x18];
} Tab;

typedef struct {
    u8   pad0[0xD];
    u8   count;
    u8   padE[0x1A];
    Tab *tab;
} Cfg;

typedef struct {
    s32 idx;
    s32 anim;
    f32 radius;
    s32 retry;
    s32 frame;
    s32 pad14;
    s32 cnt18;
} Vars;

typedef struct {
    u8    pad0[0x20];
    u8    state;
    u8    pad21[3];
    Cfg  *cfg;
    u8    pad28[0x2B];
    u8    type;
    u8    pad54[0x1C];
    u8    flags;
    u8    pad71[7];
    Vars *vars;
} Moby;

typedef struct {
    u8  pad0[0x70];
    u16 cnt;
    u16 last;
    u32 flags;
} Ring;

typedef struct {
    float f;
    short a;
    short b;
    short c;
    short pad;
} E8690;

typedef union {
    OvlQuad q;
    f32     f[4];
} V_2b7568;

typedef struct {
    V_2b7568 corner[4];
    u32      color[4];
    struct {
        f32 u, v;
    } uv[4];
    long unk70, tex, unk80, unk88;
} Q_2b7568;

typedef struct {
    f32 len;
    s16 alpha;
    s16 pad;
    s16 on;
    s16 pad2;
} E_2b7568;

extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern int  random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern s32  FUN_L00_0028d8c0_u(Moby *, s32) __asm__("FUN_L00_0028d8c0");
extern s32  FUN_L00_00257b50_b6bc8(s32) __asm__("FUN_L00_00257b50");
extern u8  *FUN_L00_0025c338_b6bc8(Moby *) __asm__("FUN_L00_0025c338");
extern void
     spawn_nearby_effect(Moby *, Moby *, s32, void *, void *, f32) __asm__("FUN_L00_00259bc8");
void update_taunter_effect_slots(void *source_moby) __asm__("FUN_L00_002b7398");
extern void add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
void        FUN_L00_002b7568_c(void) __asm__("FUN_L00_002b7568");

extern int   D_0015EEA4;
extern s32   D_0015EEA4;
extern s32   D_L00_0015F5CC;
extern u32   D_0013C940[];
extern Ring  D_00141680;
extern Moby *D_001ABA00_b6bc8[] __asm__("D_L00_001ABA00");
extern s16   D_L00_001B07B0[] __asm__("D_L00_001B07B0");
extern E8690 D_L00_001DB440[];
extern int   D_L00_001614D0;
extern float D_L00_001614DC;
extern char  D_L00_001614E0[] MACRO_ADDR;
extern int   taunter_effect_spawn_interval_frames_gp __asm__("D_L00_001614B0") __attribute__((sda));
extern float taunter_effect_travel_distance_gp __asm__("D_L00_001614B4") __attribute__((sda));
extern float taunter_effect_travel_speed_gp __asm__("D_L00_001614B8") __attribute__((sda));
extern unsigned short D_L00_001614BC __attribute__((sda));
extern int            D_L00_001614D0_macro[] __asm__("D_L00_001614D0") MACRO_ADDR;
extern E_2b7568       D_L00_001DB440_lui[] __asm__("D_L00_001DB440") NOT_SDA;
extern V_2b7568       D_L00_001614E0_2b7568[] __asm__("D_L00_001614E0")
    __attribute__((section(".sdata")));
extern f32      D_L00_001614DC;
extern s32      D_L00_001614BC_gp __asm__("D_L00_001614BC") __attribute__((sda));
extern V_2b7568 D_0013F5E0_2b7568 __asm__("D_0013F5E0");
extern V_2b7568 D_L00_001DB400[] NOT_SDA;
extern f32      taunter_effect_quad_final_scale_gp __asm__("D_L00_001614C8") __attribute__((sda));
extern f32      taunter_effect_quad_initial_scale_gp __asm__("D_L00_001614C4") __attribute__((sda));
extern u32      taunter_effect_quad_rgb_gp __asm__("D_L00_001614C0") __attribute__((sda));

f32 D_L15_001D034C[1] __attribute__((section(".data"))) = {0};

E8690 D_L01_001DB8C0 = {0};

int            D_L01_00161500 = 0;
float          D_L01_0016150C = 0;
char           D_L01_00161510 = 0;
int            D_L01_001614E0 = 0xa;
float          D_L01_001614E4 = 1e+01f;
float          D_L01_001614E8 = 2e+01f;
unsigned short D_L01_001614EC = 0x14;
f32            D_L01_001614F8 = 1e+01f;
f32            D_L01_001614F4 = 0.3f;
u32            D_L01_001614F0 = 0x7f5050;
E8690          D_L02_001CFEC0 = {0};

int            D_L02_001614A0 = 0;
float          D_L02_001614AC = 0;
char           D_L02_001614B0 = 0;
int            D_L02_00161480 = 0xa;
float          D_L02_00161484 = 1e+01f;
float          D_L02_00161488 = 2e+01f;
unsigned short D_L02_0016148C = 0x14;
f32            D_L02_00161498 = 1e+01f;
f32            D_L02_00161494 = 0.3f;
u32            D_L02_00161490 = 0x7f5050;
E8690          D_L03_001CF9C0 = {0};

int            D_L03_001614B0 = 0;
float          D_L03_001614BC = 0;
char           D_L03_001614C0 = 0;
int            D_L03_00161490 = 0xa;
float          D_L03_00161494 = 1e+01f;
float          D_L03_00161498 = 2e+01f;
unsigned short D_L03_0016149C = 0x14;
f32            D_L03_001614A8 = 1e+01f;
f32            D_L03_001614A4 = 0.3f;
u32            D_L03_001614A0 = 0x7f5050;
E8690          D_L04_001CFA40 = {0};

int            D_L04_001614A0 = 0;
float          D_L04_001614AC = 0;
char           D_L04_001614B0 = 0;
int            D_L04_00161480 = 0xa;
float          D_L04_00161484 = 1e+01f;
float          D_L04_00161488 = 2e+01f;
unsigned short D_L04_0016148C = 0x14;
f32            D_L04_00161498 = 1e+01f;
f32            D_L04_00161494 = 0.3f;
u32            D_L04_00161490 = 0x7f5050;
E8690          D_L05_001CFD60 = {0};

int            D_L05_001614F0 = 0;
float          D_L05_001614FC = 0;
char           D_L05_00161500 = 0;
int            D_L05_001614D0 = 0xa;
float          D_L05_001614D4 = 1e+01f;
float          D_L05_001614D8 = 2e+01f;
unsigned short D_L05_001614DC = 0x14;
f32            D_L05_001614E8 = 1e+01f;
f32            D_L05_001614E4 = 0.3f;
u32            D_L05_001614E0 = 0x7f5050;
E8690          D_L06_001D0040 = {0};

int            D_L06_001614D0 = 0;
float          D_L06_001614DC = 0;
char           D_L06_001614E0 = 0;
int            D_L06_001614B0 = 0xa;
float          D_L06_001614B4 = 1e+01f;
float          D_L06_001614B8 = 2e+01f;
unsigned short D_L06_001614BC = 0x14;
f32            D_L06_001614C8 = 1e+01f;
f32            D_L06_001614C4 = 0.3f;
u32            D_L06_001614C0 = 0x7f5050;
E8690          D_L07_001CF940 = {0};

int            D_L07_001614A0 = 0;
float          D_L07_001614AC = 0;
char           D_L07_001614B0 = 0;
int            D_L07_00161480 = 0xa;
float          D_L07_00161484 = 1e+01f;
float          D_L07_00161488 = 2e+01f;
unsigned short D_L07_0016148C = 0x14;
f32            D_L07_00161498 = 1e+01f;
f32            D_L07_00161494 = 0.3f;
u32            D_L07_00161490 = 0x7f5050;
E8690          D_L08_001D00C0 = {0};

int            D_L08_001614F0 = 0;
float          D_L08_001614FC = 0;
char           D_L08_00161500 = 0;
int            D_L08_001614D0 = 0xa;
float          D_L08_001614D4 = 1e+01f;
float          D_L08_001614D8 = 2e+01f;
unsigned short D_L08_001614DC = 0x14;
f32            D_L08_001614E8 = 1e+01f;
f32            D_L08_001614E4 = 0.3f;
u32            D_L08_001614E0 = 0x7f5050;
E8690          D_L09_001CFA40 = {0};

int            D_L09_00161560 = 0;
float          D_L09_0016156C = 0;
char           D_L09_00161570 = 0;
int            D_L09_00161540 = 0xa;
float          D_L09_00161544 = 1e+01f;
float          D_L09_00161548 = 2e+01f;
unsigned short D_L09_0016154C = 0x14;
f32            D_L09_00161558 = 1e+01f;
f32            D_L09_00161554 = 0.3f;
u32            D_L09_00161550 = 0x7f5050;
E8690          D_L10_001CFD40 = {0};

int            D_L10_001614B0 = 0;
float          D_L10_001614BC = 0;
char           D_L10_001614C0 = 0;
int            D_L10_00161490 = 0xa;
float          D_L10_00161494 = 1e+01f;
float          D_L10_00161498 = 2e+01f;
unsigned short D_L10_0016149C = 0x14;
f32            D_L10_001614A8 = 1e+01f;
f32            D_L10_001614A4 = 0.3f;
u32            D_L10_001614A0 = 0x7f5050;
E8690          D_L11_001D02C0 = {0};

int            D_L11_00161530 = 0;
float          D_L11_0016153C = 0;
char           D_L11_00161540 = 0;
int            D_L11_00161510 = 0xa;
float          D_L11_00161514 = 1e+01f;
float          D_L11_00161518 = 2e+01f;
unsigned short D_L11_0016151C = 0x14;
f32            D_L11_00161528 = 1e+01f;
f32            D_L11_00161524 = 0.3f;
u32            D_L11_00161520 = 0x7f5050;
E8690          D_L12_001E2960 = {0};

int            D_L12_001614B0 = 0;
float          D_L12_001614BC = 0;
char           D_L12_001614C0 = 0;
int            D_L12_00161490 = 0xa;
float          D_L12_00161494 = 1e+01f;
float          D_L12_00161498 = 2e+01f;
unsigned short D_L12_0016149C = 0x14;
f32            D_L12_001614A8 = 1e+01f;
f32            D_L12_001614A4 = 0.3f;
u32            D_L12_001614A0 = 0x7f5050;
E8690          D_L13_001D01D0 = {0};

int            D_L13_00161580 = 0;
float          D_L13_0016158C = 0;
char           D_L13_00161590 = 0;
int            D_L13_00161560 = 0xa;
float          D_L13_00161564 = 1e+01f;
float          D_L13_00161568 = 2e+01f;
unsigned short D_L13_0016156C = 0x14;
f32            D_L13_00161578 = 1e+01f;
f32            D_L13_00161574 = 0.3f;
u32            D_L13_00161570 = 0x7f5050;
E8690          D_L14_001DC880 = {0};

int            D_L14_001616C0 = 0;
float          D_L14_001616CC = 0;
char           D_L14_001616D0 = 0;
int            D_L14_001616A0 = 0xa;
float          D_L14_001616A4 = 1e+01f;
float          D_L14_001616A8 = 2e+01f;
unsigned short D_L14_001616AC = 0x14;
f32            D_L14_001616B8 = 1e+01f;
f32            D_L14_001616B4 = 0.3f;
u32            D_L14_001616B0 = 0x7f5050;
E8690          D_L15_001D0340 = {0};

int            D_L15_00161660 = 0;
float          D_L15_0016166C = 0;
char           D_L15_00161670 = 0;
int            D_L15_00161640 = 0xa;
float          D_L15_00161644 = 1e+01f;
float          D_L15_00161648 = 2e+01f;
unsigned short D_L15_0016164C = 0x14;
f32            D_L15_00161658 = 1e+01f;
f32            D_L15_00161654 = 0.3f;
u32            D_L15_00161650 = 0x7f5050;
E8690          D_L16_001CFD40 = {0};

int            D_L16_00161560 = 0;
float          D_L16_0016156C = 0;
char           D_L16_00161570 = 0;
int            D_L16_00161540 = 0xa;
float          D_L16_00161544 = 1e+01f;
float          D_L16_00161548 = 2e+01f;
unsigned short D_L16_0016154C = 0x14;
f32            D_L16_00161558 = 1e+01f;
f32            D_L16_00161554 = 0.3f;
u32            D_L16_00161550 = 0x7f5050;
E8690          D_L17_001D05C0 = {0};

int            D_L17_001615B0 = 0;
float          D_L17_001615BC = 0;
char           D_L17_001615C0 = 0;
int            D_L17_00161590 = 0xa;
float          D_L17_00161594 = 1e+01f;
float          D_L17_00161598 = 2e+01f;
unsigned short D_L17_0016159C = 0x14;
f32            D_L17_001615A8 = 1e+01f;
f32            D_L17_001615A4 = 0.3f;
u32            D_L17_001615A0 = 0x7f5050;
E8690          D_L18_001D02C0 = {0};

int            D_L18_00161540 = 0;
float          D_L18_0016154C = 0;
char           D_L18_00161550 = 0;
int            D_L18_00161520 = 0xa;
float          D_L18_00161524 = 1e+01f;
float          D_L18_00161528 = 2e+01f;
unsigned short D_L18_0016152C = 0x14;
f32            D_L18_00161538 = 1e+01f;
f32            D_L18_00161534 = 0.3f;
u32            D_L18_00161530 = 0x7f5050;

s32 apply_taunter_to_nearby_mobies(Moby *taunter_moby, f32 radius) __asm__("FUN_L00_002b6bc8");

s32 apply_taunter_to_nearby_mobies(Moby *taunter_moby, f32 radius) {
    u128 velocity __attribute__((aligned(16)));
    union {
        u128 q;
        f32  f[4];
    } pos __attribute__((aligned(16)));
    Moby        *other;
    u8          *state;
    Vars        *vars;
    struct Hero *player;
    s32          limit;
    s32          count = 0;
    s32          i;
    f32          distance;
    f32          angle;

    pos.q = *(u128 *)((u8 *)taunter_moby + 0x10);
    vars = taunter_moby->vars;
    limit = FUN_L00_00257b50_b6bc8(vars->pad14 > 0 ? vars->pad14 : 1);
    vars->pad14 = 0;
    other = D_001ABA00_b6bc8[0];
    if (other != 0) {
        i = 0;
        do {
            /* Fakematch: the extra block keeps retail's scheduling of the moby lookup. */
            do {
                state = FUN_L00_0025c338_b6bc8(other);
            } while (0);
            if (state != 0) {
                /* Fakematch: &hero through the motion label keeps retail's &hero + 0x98 load below. */
                player = (struct Hero *)(D_0013F3D0 - 0x80);
                distance = vector_distance_xy(&pos, (u8 *)other + 0x10);
                if (distance < 12.0f && ((u16)(*(u16 *)((u8 *)other + 0xa6) - 500) < 2 ||
                                         *(s16 *)((u8 *)other + 0xa6) == 0x1f9 ||
                                         *(s16 *)((u8 *)other + 0xa6) == 0x1ff)) {
                    struct Hero *g = &hero;
                    angle = fast_difference_between_rotations(
                        g->motion.rot.f[2],
                        fast_arc_tan(*(f32 *)((u8 *)other + 0x10) - g->motion.pos.f[0],
                                     *(f32 *)((u8 *)other + 0x14) - g->motion.pos.f[1]));
                    if (angle < 0.9599311f) {
                        if (vars->pad14 == limit) {
                            fast_vec_sub(&velocity, (u8 *)other + 0x10, (u8 *)taunter_moby + 0x10);
                            fast_vec_normalize(&velocity, &velocity, 1.5f);
                            spawn_nearby_effect(
                                other, taunter_moby, 0x10000, &g->motion.pos, &velocity, 1.0f);
                            vars->frame = scale_game_frames(random_int_between(5, 0x2d));
                        }
                        vars->pad14++;
                    }
                }
                if (distance < radius) {
                    struct Hero *g = &hero;
                    angle = fast_arc_tan(*(f32 *)((u8 *)other + 0x10) - g->motion.pos.f[0],
                                         *(f32 *)((u8 *)other + 0x14) - g->motion.pos.f[1]);
                    angle = fast_difference_between_rotations(player->motion.rot.f[2], angle);
                    if (angle < 0.9599311f) {
                        Moby *current = *(Moby **)(state + 0x18);
                        if (current == 0 || distance < vector_distance_xy((u8 *)current + 0x10,
                                                                          (u8 *)other + 0x10)) {
                            *(Moby **)(state + 0x18) = taunter_moby;
                            count++;
                        }
                    }
                }
            }
            i++;
            other = D_001ABA00_b6bc8[i];
        } while (other != 0);
    }
    for (i = 1; i <= D_L00_001B07B0[0]; i++) {
        other = (Moby *)(moby_table + D_L00_001B07B0[i] * 0x100);
        if (other != 0 && (s8)other->state >= 0) {
            Vars *vars = other->vars;
            distance = vector_distance_xy(&pos, (u8 *)other + 0x10);
            if (distance < radius) {
                angle = fast_difference_between_rotations(
                    hero.motion.rot.f[2],
                    fast_arc_tan(*(f32 *)((u8 *)other + 0x10) - hero.motion.pos.f[0],
                                 *(f32 *)((u8 *)other + 0x14) - hero.motion.pos.f[1]));
                if (angle < 0.9599311f) {
                    *(Moby **)((u8 *)vars + 0x78) = taunter_moby;
                    count++;
                }
            }
        }
    }
    return count;
}

void taunter_update(Moby *m) __asm__("FUN_L00_002b6f10");

void taunter_update(Moby *taunter_moby) {
    Vars *taunter_vars;
    s32   taunter_state;
    s32   voice_slot_index;
    s32   activation_button_bits;
    s32   i;
    s32   next_entry_scaled_x;
    s32   next_entry_scaled_y;
    u8   *voice_slot_window;
    s32   play_time_stat_value;

    if (taunter_moby == 0)
        return;
    taunter_vars = taunter_moby->vars;
    if (taunter_vars == 0)
        return;
    if (taunter_vars->radius == 0.0f)
        taunter_vars->radius = 30.0f;
    if (taunter_moby->type == 2)
        taunter_moby->state = 3;
    taunter_effect_emission_disabled = 1;
    if (taunter_moby->state == 0) {
        taunter_vars->anim = -1;
        taunter_moby->state++;
        taunter_effect_reset_pending = 1;
    }
    taunter_state = taunter_moby->state;
    if ((u8)taunter_state == 1) {
        if (taunter_moby->flags & 2) {
            taunter_moby->state = taunter_state + 1;
            taunter_vars->retry = 0;
        }
        goto end;
    }
    if ((u8)taunter_state != 2)
        goto end;
    if (taunter_moby->type == 3 && hero.item_use_locked == 0) {
        taunter_effect_emission_disabled = 0;
        taunter_vars->cnt18++;
    } else {
        taunter_vars->cnt18 = 0;
    }
    if (FUN_L00_0028d8c0_u(taunter_moby, taunter_vars->anim) == 0) {
        activation_button_bits = D_0013C940[0x1A4 / 4] & hero.items[0].button_mask;
        if ((activation_button_bits != 0 ||
             ((D_0013C940[0x1A0 / 4] & hero.items[0].button_mask) != 0 &&
              taunter_vars->retry >= 0x11)) &&
            hero.item_use_locked == 0) {
            if (activation_button_bits != 0) {
                taunter_vars->frame = scale_game_frames(random_int_between(0xF, 0x2D));
            }
            if (D_00141680.cnt < 0xFFFF)
                D_00141680.cnt++;
            play_time_stat_value = scale_game_frames(D_0015EEA4) / 600;
            if (D_00141680.last < play_time_stat_value) {
                D_00141680.last = scale_game_frames(D_0015EEA4) / 600;
            }
            D_00141680.flags |= 1 << current_level_index;
            D_00141680.flags |= 0x80000000;
            if (taunter_moby->type != 3)
                blend_moby_animation(taunter_moby, 3, 0, scale_game_frames(3));
            taunter_vars->anim =
                allocate_voice_for_target_entry(taunter_vars->idx, 1, taunter_moby);
            i = taunter_vars->idx + 1;
            taunter_vars->idx = i;
            next_entry_scaled_x = truncate_float_to_s32(taunter_moby->cfg->tab[i].x * 10.0f);
            next_entry_scaled_y = truncate_float_to_s32(taunter_moby->cfg->tab[i].y * 10.0f);
            if (!(taunter_vars->idx < taunter_moby->cfg->count && next_entry_scaled_x == 0x1E &&
                  next_entry_scaled_y == next_entry_scaled_x))
                taunter_vars->idx = 0;
            apply_taunter_to_nearby_mobies(taunter_moby, taunter_vars->radius);
            taunter_vars->retry = 0;
            goto end;
        }
        if (taunter_vars->anim < 0) {
            if ((D_0013C940[0x1A0 / 4] & hero.items[0].button_mask) != 0 &&
                hero.item_use_locked == 0)
                taunter_vars->retry++;
            else
                taunter_vars->retry = 0;
            if (taunter_moby->type != 1)
                blend_moby_animation(taunter_moby, 1, 0, scale_game_frames(5));
            goto end;
        }
    } else {
        taunter_vars->retry++;
        if ((pad_held[0] & hero.items[0].button_mask) != 0 ||
            scale_game_frames(0x1E) >= taunter_vars->cnt18) {
            if (hero.item_use_locked == 0)
                goto L7340;
        }
    }
    if (taunter_moby->type != 1)
        blend_moby_animation(taunter_moby, 1, 0, scale_game_frames(5));
    voice_slot_index = taunter_vars->anim;
    if (voice_slot_index != -1) {
        voice_slot_window = (u8 *)&voice_pool + voice_slot_index * 0x70;
        if (*(Moby **)(voice_slot_window + 0x88) == taunter_moby && voice_slot_window[0x74] != 0)
            release_voice_slot(voice_slot_index);
    }
    taunter_vars->retry = 0;
    taunter_vars->anim = -1;
    goto end;
L7340:
    if (!(D_L00_0015F5CC & 3))
        apply_taunter_to_nearby_mobies(taunter_moby, taunter_vars->radius);
end:
    if ((s32)(hero.items[0].state ^ 3) != 0)
        update_taunter_effect_slots(taunter_moby);
}

/* per-frame update of the ten slot table: spawn one, age the rest, then draw */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B8690), where it is exact; names translated to the US level program. */
void update_taunter_effect_slots(void *source_moby) {
    E8690         *effect_slot;
    unsigned short av;
    int            i;
    int            effect_lifetime_frames;
    if (taunter_effect_reset_pending != 0) {
        for (i = 9; i >= 0; i--)
            D_L00_001DB440[i].c = 0;
        taunter_effect_reset_pending = 0;
        D_L00_001614D0 = 0;
        effect_lifetime_frames = truncate_float_to_s32(
            taunter_effect_travel_distance_gp / (taunter_effect_travel_speed_gp * frame_time));
        taunter_effect_lifetime_frames = effect_lifetime_frames;
        D_L00_001614DC = 1.0f / convert_integer_to_float(effect_lifetime_frames);
    }
    if (taunter_effect_emission_disabled == 0) {
        if (fast_dec_timer(&D_L00_001614D0) != 0) {
            i = 0;
            do {
                effect_slot = &D_L00_001DB440[i];
                if (effect_slot->c == 0) {
                    D_L00_001614D0 = scale_game_frames(taunter_effect_spawn_interval_frames_gp);
                    av = *(unsigned short *)&D_L00_001614D0_macro[2];
                    effect_slot->b = D_L00_001614BC;
                    effect_slot->c = 1;
                    effect_slot->a = av;
                    effect_slot->f = 0;
                    break;
                }
            } while (++i < 10);
        }
    }
    for (i = 0; i < 10; i++) {
        effect_slot = &D_L00_001DB440[i];
        if (effect_slot->c != 0) {
            if (fast_dec_timer_s16(&effect_slot->a) != 0)
                effect_slot->c = 0;
            else
                effect_slot->f += taunter_effect_travel_speed_gp * frame_time;
        }
    }
    moby_point_position(source_moby, 0, D_L00_001614E0);
    add_draw_callback(FUN_L00_002b7568_c, source_moby);
}

void draw_taunter_effect_quads(u8 *source_moby) __asm__("FUN_L00_002b7568");

void draw_taunter_effect_quads(u8 *source_moby) {
    Q_2b7568  quad;
    V_2b7568  quad_basis_direction;
    V_2b7568  up;
    V_2b7568  side;
    V_2b7568  quad_origin;
    V_2b7568  corner;
    V_2b7568  quad_offset_direction;
    V_2b7568  distance_offset;
    E_2b7568 *e;
    s32       i, j;
    f32       fade_factor, sz;
    u32       col;
    quad.tex = get_effect_texture(8);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    quad.uv[0].u = 0.0f;
    quad.uv[0].v = 0.0f;
    quad.uv[1].u = 0.0f;
    quad.uv[1].v = 1.0f;
    quad.uv[2].u = 1.0f;
    quad.uv[2].v = 0.0f;
    quad.uv[3].u = 1.0f;
    quad.uv[3].v = 1.0f;
    fast_vec_normalize(&quad_offset_direction, source_moby + 0xD0, -1.0f);
    for (i = 0; i < 10; i++) {
        if (D_L00_001DB440_lui[i].on == 0)
            continue;
        fast_vec_scale(&distance_offset, &quad_offset_direction, D_L00_001DB440_lui[i].len);
        fast_vec_add(&quad_origin, D_L00_001614E0_2b7568, &distance_offset);
        qcopy(&quad_basis_direction, &quad_offset_direction);
        fast_vec_scale(&side, &D_0013F5E0_2b7568, -1.0f);
        fast_vec_cross(&up, &quad_basis_direction, &side);
        fast_vec_normalize(&up, &up, 1.0f);
        fast_vec_cross(&side, &up, &quad_basis_direction);
        fade_factor = convert_integer_to_float(D_L00_001DB440_lui[i].alpha) * D_L00_001614DC;
        if (1.0f < fade_factor)
            fade_factor = 1.0f;
        sz = taunter_effect_quad_final_scale_gp +
             (taunter_effect_quad_initial_scale_gp - taunter_effect_quad_final_scale_gp) *
                 fade_factor;
        col = (taunter_effect_quad_rgb_gp & 0xFFFFFF) |
              (truncate_float_to_s32(
                   (convert_integer_to_float(D_L00_001614BC_gp) - 0.0f) * fade_factor + 0.0f)
               << 24);
        quad.color[3] = col;
        quad.color[2] = col;
        quad.color[1] = col;
        quad.color[0] = col;
        for (j = 0; j < 4; j++) {
            fast_vec_scale(&corner, &D_L00_001DB400[j], sz);
            transform_vector(&quad.corner[j], &corner, &quad_basis_direction);
        }
        draw_geometry_quad(&quad, 0, 0);
    }
}
