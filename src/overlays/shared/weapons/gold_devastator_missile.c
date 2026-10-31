/* Shared moby class gold_devastator_missile: the homing missile the devastator fires, its target selection, and the function that spawns it. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/overlay/shared/rendering/effects/explosion_light.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/shared/weapons/mine.h"
#include "rnc/overlay/shared/weapons/bomb_glove_bomb.h"
#include "rnc/game/item_state.h"

typedef struct {
    char  p0[0x80];
    f32   x80;
    f32   x84;
    char  p1[0x2E4 - 0x88];
    f32   f2E4;
    char  p2[0x2080 - 0x2E8];
    char *cam;
} P;

typedef union {
    OvlQuad q;
    float   f[4];
} V_2afc70;

typedef int ti_t __attribute__((mode(TI), aligned(16)));

typedef union {
    ti_t q;
    f32  f[4];
} VU;

typedef struct {
    VU    v;
    s32   w10;
    s32   w14;
    char *owner;
    f32   f1C;
    s32   w20;
    s32   w24;
    s32   w28;
    f32   f2C;
    f32   f30;
    s32   w34;
    s32   w38;
} O;

typedef u32 VQ1688 __attribute__((mode(TI), aligned(16)));

typedef union {
    VQ1688 q;
    f32    f[4];
    s32    i[4];
} V1688;

typedef int Q1688 __attribute__((mode(TI)));

typedef struct {
    V1688 previous_target;
    M910 *owner;
    int   life;
    M910 *target;
    float target_height;
    float speed;
    float yaw_velocity;
    float pitch_velocity;
    float yaw;
    float pitch;
    float desired_speed;
    int   track_velocity;
    short turn_time;
    short turn_period;
    int   bounce_count;
    int   bounce_timer;
    float limit;
} Missile1688;

typedef struct {
    int colors[6];
} Colors1688;

typedef union {
    Qry910 collision;
    struct {
        V1688 direction;
        V1688 relative;
        V1688 target;
    } vectors;
} MissilePhase1688;

typedef struct {
    char  pad0[0x18];
    M910 *moby;
    int   kind;
    V1688 position;
    char  pad30[0x10];
    V1688 normal, velocity_end, velocity_start;
} Impact1688;

extern float fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern int collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern int FUN_L00_00257b90(int, int);
extern unsigned swap_packed_color_channels(unsigned c, int mask) __asm__("FUN_L00_0025c0e8");
extern void    *spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
       void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_sub(void *dst, void *a, void *b) __asm__("FUN_001f9a28");
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002AE278). */
extern f32     fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern int     FUN_001f96f8_2afc70(int) __asm__("FUN_001f96f8");
extern float   FUN_001f9af0_2afc70(void *) __asm__("FUN_001f9af0");
extern int     FUN_001efa68_2afc70(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void    FUN_L00_00250df8_2afc70(void *) __asm__("FUN_L00_00250df8");
extern void    append_unique_moby_index(void *, void *, int) __asm__("FUN_L00_0025f780");
extern f32     FUN_001f99c0(f32);
extern void    FUN_00214db0(void *, f32, f32, f32);
extern s32     FUN_001efa68_u(void *, void *, s32, char *, void *) __asm__("FUN_001efa68");
extern s32     find_listed_moby_by_data_id(char *) __asm__("FUN_L00_0025f878");
extern char   *FUN_L00_002af450(char *, char *, int, int, int);
unsigned char *spawn_glow_particle_u8(void         *owner,
                                      void         *pos,
                                      void         *vel,
                                      float         scale,
                                      int           lifetime,
                                      unsigned char red,
                                      unsigned char green,
                                      unsigned char blue,
                                      unsigned char alpha) __asm__("FUN_L00_002ac910");
extern void FUN_L00_0025a9f8(void *, void *, void *, int, int, int, int, int, float, float, float);
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, int, int, float);
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
void        spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern float FUN_L00_001ff430_c(void *) __asm__("FUN_L00_001ff430");
extern void  FUN_L00_001ff2c0_c(void *, void *, void *) __asm__("FUN_L00_001ff2c0");
extern float spring_turn(void *, float, float, float, float, float) __asm__("FUN_L00_00258110");
extern unsigned char *
smoke1688(void *, void *, int, int, int, int, float, float, float, float, float) __asm__("FUN_L00_002712b8");
extern void FUN_L00_001ff290_c(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern int  collision1688(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void FUN_L00_001ffd48_c(void *, void *, float) __asm__("FUN_L00_001ffd48");
extern void FUN_L00_001ffe20_c(void *, void *, void *) __asm__("FUN_L00_001ffe20");
extern void FUN_L00_0026a4c8_c(void *, void *, int, int, float, int) __asm__("FUN_L00_0026a4c8");

extern E             D_0013E520;
extern char         *active_mobies[] __asm__("D_L00_001ABA00");
extern int          *D_L00_00177F00[];
extern s32           D_L00_00166DC0[];
extern s32           D_L00_00173E40[0x1C];
extern int           D_L00_0015F5CC;
extern V_2afc70      hero_pos __asm__("D_0013F3D0") NOT_SDA;
extern unsigned char D_0013F490_lui[] __asm__("D_0013F490") NOT_SDA;
extern void         *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern unsigned char D_L00_00173E60_2afc70[] __asm__("D_L00_00173E60")
    __attribute__((section(".data")));
extern s16        D_L00_001B0770_lui[] __asm__("D_L00_001B0770") NOT_SDA;
extern char      *active_mobies[] __asm__("D_L00_001ABA00") NOT_SDA;
extern s32        D_L00_00166DC0[] NOT_SDA;
extern s32        D_L00_00173E40[0x1C] NOT_SDA;
extern B910       l00_devastator_missile_camera_blast_parameters __asm__("D_L00_001E9950");
extern B910       l00_gold_devastator_missile_camera_blast_parameters __asm__("D_L00_001E99A0");
extern Colors1688 l00_devastator_missile_impact_color_palette __asm__("D_L00_001E99F0");
extern Colors1688 l00_devastator_missile_impact_fade_palette __asm__("D_L00_001E9A08");
extern P          D_0013F350;
extern V1688 D_L00_00160660          MACRO_ADDR;
extern V1688 D_L00_00160680          MACRO_ADDR;
extern float                         D_0015ED6C;
extern float                         D_0015ED70;
extern float                         D_L00_0015F5D0;
extern float                         D_L00_0015F5D4;
extern short                         D_L00_001B0770[];
extern unsigned char *D_L00_001B20DC NOT_SDA;
extern Impact1688                    impact1688 __asm__("D_L00_00173E40");

Colors1688 D_L01_0020AA50 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L01_0020AA68 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L01_001B255C NOT_SDA = 0;

Colors1688 D_L02_001FA2F0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L02_001FA308 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L02_001B26DC NOT_SDA = 0;

Colors1688 D_L03_001E2B80 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L03_001E2B98 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L03_001B21DC NOT_SDA = 0;

Colors1688 D_L04_001DB790 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L04_001DB7A8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L04_001B225C NOT_SDA = 0;

Colors1688 D_L05_00218100 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L05_00218118 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L05_001B255C NOT_SDA = 0;

Colors1688 D_L06_00200DF0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L06_00200E08 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L06_001B285C NOT_SDA = 0;

Colors1688 D_L07_00210C90 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L07_00210CA8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L07_001B215C NOT_SDA = 0;

Colors1688 D_L08_001FB950 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L08_001FB968 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L08_001B28DC NOT_SDA = 0;

Colors1688 D_L09_002085E0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L09_002085F8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L09_001B225C NOT_SDA = 0;

Colors1688 D_L10_001DC7E0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L10_001DC7F8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L10_001B255C NOT_SDA = 0;

Colors1688 D_L11_0021AC70 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L11_0021AC88 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L11_001B2ADC NOT_SDA = 0;

Colors1688 D_L12_0020BF40 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L12_0020BF58 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L12_001B255C NOT_SDA = 0;

Colors1688 D_L13_001F48E0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L13_001F48F8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L13_001B23DC NOT_SDA = 0;

Colors1688 D_L14_001FC220 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L14_001FC238 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L14_001B27DC NOT_SDA = 0;

Colors1688 D_L15_001E28B0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L15_001E28C8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

Colors1688 D_L16_001E7D40 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L16_001E7D58 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

Colors1688 D_L17_001E6480 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L17_001E6498 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L17_001B29DC NOT_SDA = 0;

Colors1688 D_L18_001F2120 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L18_001F2138 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

float D_L03_00173F68 NOT_SDA = 0;
float D_L07_00173EE8 NOT_SDA = 0;
float D_L08_00174668 NOT_SDA = 0;
float D_L10_001742E8 NOT_SDA = 0;
float D_L11_00174868 NOT_SDA = 0;
float D_L12_001742E8 NOT_SDA = 0;
float D_L13_00174168 NOT_SDA = 0;
float D_L15_00174468 NOT_SDA = 0;
float D_L16_00174268 NOT_SDA = 0;
float D_L17_00174768 NOT_SDA = 0;
float D_L18_00174868 NOT_SDA = 0;
s32                  D_L03_00173F64[1] __attribute__((section(".data"))) = {0};

s32 D_L09_00173FE4[1] __attribute__((section(".data"))) = {0};

unsigned char *spawn_devastator_missile(unsigned char *owner,
                                        V_2afc70      *spawn_position,
                                        unsigned char *target_moby,
                                        float          guidance_yaw,
                                        float          guidance_pitch,
                                        float          target_height_offset,
                                        float          initial_yaw,
                                        float          initial_pitch) __asm__("FUN_L00_002afc70");

unsigned char *spawn_devastator_missile(unsigned char *missile_source_moby,
                                        V_2afc70      *spawn_position,
                                        unsigned char *target_moby,
                                        float          guidance_yaw,
                                        float          guidance_pitch,
                                        float          target_height_offset,
                                        float          initial_yaw,

                                        float initial_pitch) {
    V_2afc70       collision_start;
    unsigned char *missile_moby;
    unsigned char *missile_data;
    unsigned char *target_aim_record;
    float          default_desired_speed;
    int            turn_period;
    float          x;

    missile_moby = create_moby(0x99);
    if (missile_moby != 0) {
        missile_data = ((unsigned char *)MOBY(missile_moby)->pvars);
        missile_moby[0x30] = 0xFF;
        MOBY(missile_moby)->draw_dist = 0xFF;
        missile_moby[0x31] = 1;
        MOBY(missile_moby)->rot.z = initial_yaw;
        MOBY(missile_moby)->rot.y = initial_pitch;
        missile_moby[0x20] = 0;
        *(unsigned char **)(missile_data + 0x10) = missile_source_moby;
        qcopy(missile_moby + 0x10, spawn_position);
        default_desired_speed = frame_time * 20.0f;
        *(int *)(missile_data + 0x38) = 1;
        *(float *)(missile_data + 0x48) = 70.0f;
        *(float *)(missile_data + 0x34) = default_desired_speed;
        *(int *)(missile_data + 0x24) = 0;
        *(int *)(missile_data + 0x28) = 0;
        *(unsigned char **)(missile_data + 0x18) = target_moby;
        *(float *)(missile_data + 0x20) = default_desired_speed / 3.0f;
        if (target_moby != 0) {
            target_aim_record = get_moby_target_info(target_moby);
            if (target_aim_record != 0) {
                *(unsigned short *)(target_aim_record + 0x1E) |= 0x80;
                if (target_aim_record[0xB]) {
                    *(int *)(missile_data + 0x38) = 0;
                }
                if (target_aim_record[0xC]) {
                    *(float *)(missile_data + 0x34) = (float)target_aim_record[0xC] * frame_time;
                }
                if (frame_time * 20.0f < *(float *)(missile_data + 0x34)) {
                    *(float *)(missile_data + 0x48) = 140.0f;
                }
            }
        }
        *(float *)(missile_data + 0x2C) = guidance_yaw;
        *(float *)(missile_data + 0x30) = guidance_pitch;
        *(int *)(missile_data + 0x14) = FUN_001f96f8_2afc70(300);
        if (target_moby != 0) {
            qcopy(missile_data, target_moby + 0x10);
            *(float *)(missile_data + 0x1C) = target_height_offset;
            *(float *)(missile_data + 8) += target_height_offset;
        }
        x = FUN_001f9af0_2afc70(D_0013F490_lui);
        turn_period = FUN_001f96f8_2afc70(frame_time * 0.1f < x ? 0x3C : 0xF);
        *(short *)(missile_data + 0x3E) = turn_period;
        *(short *)(missile_data + 0x3C) = turn_period;
        if (missile_source_moby != 0 && missile_source_moby[0x20] != 0xFE &&
            missile_source_moby[0x20] != 0xFD) {
            collision_start.q = ((V_2afc70 *)(missile_source_moby + 0x10))->q;
        } else {
            qcopy(&collision_start, &hero_pos);
            collision_start.f[2] += 0.6f;
        }
        collision_start.f[2] = spawn_position->f[2];
        if (FUN_001efa68_2afc70(&collision_start, spawn_position, 0, D_001413D0_lui, 0)) {
            missile_moby[0xBC] = 1;
            qcopy(missile_moby + 0x10, D_L00_00173E60_2afc70);
        }
        FUN_L00_00250df8_2afc70(missile_moby);
        append_unique_moby_index(missile_moby, D_L00_001B0770_lui, 0x1F);
        MOBY(missile_moby)->scale += MOBY(missile_moby)->scale;
    }
    return missile_moby;
}

void select_gold_devastator_missile_target(char *self,
                                           O    *out,
                                           char *skip) __asm__("FUN_L00_002affa8");

void select_gold_devastator_missile_target(char *gold_missile_moby,
                                           O    *gold_missile_data,

                                           char *skip) {
    VU    candidate_aim_position;
    VU    lv;
    char *candidate_moby;
    s32   candidate_aim_record_address;
    s32   i;
    s32   flag;
    f32 best, yaw, pitch, candidate_aim_distance, aim_angle_limit_or_asin_numerator, z, angh, angv;
    char        *spos;
    char        *opos;
    struct Hero *hero_context;

    i = 0;
    best = 10000.0f;
    yaw = *(f32 *)(gold_missile_moby + 0x48);
    pitch = *(f32 *)(gold_missile_moby + 0x44);
    candidate_moby = active_mobies[i];
    if (candidate_moby == 0)
        return;
    hero_context = &hero;
    while (candidate_moby != 0) {
        if (((s8)MOBY(candidate_moby)->state) < 0)
            goto next;
        if (candidate_moby == skip)
            goto next;
        qcopy(&candidate_aim_position, candidate_moby + 0x10);
        spos = gold_missile_moby + 0x10;
        opos = candidate_moby + 0x10;
        if (40.0f < fast_vec_dist(&candidate_aim_position, spos))
            goto next;
        candidate_aim_record_address = get_moby_target_info(candidate_moby);
        if (candidate_aim_record_address == 0)
            goto np;
        if (candidate_aim_record_address <= 0x100000)
            goto np;
        if (0x3FFFFF < candidate_aim_record_address)
            goto np;
        {
            f32 pf = *(f32 *)(candidate_aim_record_address + 0x10);
            z = candidate_aim_position.f[2] + pf;
        }
        goto zset;
    np:
        candidate_aim_record_address = 0;
        z = candidate_aim_position.f[2] + 0.5f;
    zset:
        candidate_aim_position.f[2] = z;
        if (candidate_moby == 0)
            goto next;
        if (((char *)MOBY(candidate_moby)->pclass) == 0)
            goto next;
        if (*(s16 *)(((char *)MOBY(candidate_moby)->pclass) + 0x46) != 5)
            goto next;
        angh = fast_arc_tan(candidate_aim_position.f[0] - *(f32 *)(gold_missile_moby + 0x10),
                            candidate_aim_position.f[1] - *(f32 *)(gold_missile_moby + 0x14));
        angv = fast_arc_tan(vector_distance_xy(spos, &candidate_aim_position),
                            candidate_aim_position.f[2] - *(f32 *)(gold_missile_moby + 0x18));
        candidate_aim_distance = fast_vec_dist(spos, &candidate_aim_position);
        if (candidate_aim_distance < 2.5f) {
            char *hero_moby = *(char **)&hero_context->moby;
            if (fast_difference_between_rotations(
                    MOBY(hero_moby)->rot.z,
                    fast_arc_tan(MOBY(candidate_moby)->pos.x - hero_context->motion.pos.f[0],
                                 MOBY(candidate_moby)->pos.y - hero_context->motion.pos.f[1])) <
                1.0471976f) {
                if (FUN_001f99c0(angv) < 0.7853982f)
                    return;
            }
        }
        if (best < candidate_aim_distance)
            goto next;
        aim_angle_limit_or_asin_numerator =
            convert_integer_to_float(item_text_variant[0xB]) * 0.6981317f;
        FUN_00214db0(&lv, candidate_aim_distance, yaw, pitch + hero_context->ground_pitch.f * 0.5f);
        fast_vec_add(&lv, &lv, spos);
        z = fast_arc_sin(fast_vec_dist(&lv, &candidate_aim_position) /
                         (candidate_aim_distance + candidate_aim_distance));
        aim_angle_limit_or_asin_numerator = aim_angle_limit_or_asin_numerator + 1.5707964f;
        if (aim_angle_limit_or_asin_numerator < z + z && candidate_aim_record_address != 0) {
            aim_angle_limit_or_asin_numerator =
                convert_integer_to_float(*(u8 *)(candidate_aim_record_address + 0xA) << 3) * 0.125f;
            fast_arc_sin(aim_angle_limit_or_asin_numerator / candidate_aim_distance);
        }
        flag = 1;
        if (FUN_001efa68_u(D_L00_00166DC0, &candidate_aim_position, 6, gold_missile_moby, 0) != 0) {
            s32 hit = D_L00_00173E40[6];
            if (hit == 0 || hit != (s32)candidate_moby)
                flag = 0;
        } else {
            best = candidate_aim_distance;
            pitch = -angv;
            yaw = angh;
            if (find_listed_moby_by_data_id(candidate_moby) != 0)
                goto next;
            z = 0.5f;
            gold_missile_data->owner = candidate_moby;
            if (candidate_aim_record_address != 0)
                z = *(f32 *)(candidate_aim_record_address + 0x10);
            gold_missile_data->f2C = angh;
            gold_missile_data->w38 = flag;
            gold_missile_data->f30 = pitch;
            gold_missile_data->w24 = 0;
            gold_missile_data->w28 = 0;
            gold_missile_data->w20 = 0;
            qcopy(gold_missile_data, opos);
            gold_missile_data->f1C = z;
        }
    next:
        i++;
        candidate_moby = active_mobies[i];
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002B1688), where it is exact; names translated to the US level program. */
void gold_devastator_missile_update(M910 *m) __asm__("FUN_L00_002b03a0");

/* Moby class 153 */
void gold_devastator_missile_update(M910 *missile_moby) {
    B910             effect = l00_devastator_missile_camera_blast_parameters;
    B910             gold_effect = l00_gold_devastator_missile_camera_blast_parameters;
    V1688            camera_delta;
    V1688            previous;
    V1688            blast_velocity;
    V1688            normal;
    V1688            smoke_position;
    V1688            motion;
    V1688            surface_motion;
    V1688            hero_velocity;
    MissilePhase1688 phase;
    V1688            predicted;
    float            basis[4][4];
    float            basis_squared[4][4];
    float            rotation[4][4];
    float            diagonal[4][4];
    float            rotation_part[4][4];
    M910            *skip = 0;
    int              reduced = 0;
    Missile1688     *missile_data;
    M910            *target;
    float            size, yaw, pitch;
    normal.q = 0;
    normal.f[2] = D_0015ED6C * 8.0f;
    blast_velocity.q = normal.q;
    normal.q = 0;
    normal.f[2] = 1.0f;
    missile_data = (Missile1688 *)missile_moby->v;
    target = missile_data->target;
    if (D_L00_0015F5D0 > 0.9f || D_L00_0015F5D4 > 0.9f)
        reduced = 1;
    missile_data->speed =
        missile_data->speed +
        (missile_data->desired_speed - missile_data->speed) / 10.0f * frame_scale_sq;
    fast_dec_timer_s16(&missile_data->turn_time);
    {
        float turn = convert_integer_to_float(missile_data->turn_time);
        turn = turn / convert_integer_to_float(missile_data->turn_period);
        fast_vec_add(phase.collision.dir, &hero.motion.platform_carry, &hero.motion.displacement);
        hero_velocity.q = phase.vectors.direction.q;
        scale_vector_xyz(&hero_velocity, &hero_velocity, turn);
    }
    size = convert_integer_to_float(D_0013E520.bB) + 1.0f;
    if (target && missile_data->life < scale_game_frames(300) - scale_game_frames(5) &&
        (signed char)target->state >= 0) {
        float time = 0.0f;
        qcopy(&phase.vectors.target, &target->pos);
        phase.vectors.target.f[2] += missile_data->target_height;
        fast_vec_sub(phase.collision.dir, &phase.vectors.target, (&missile_moby->pos));
        if (missile_data->track_velocity) {
            float intercept_linear_coefficient, a, root, intercept_time_plus_root,
                intercept_time_minus_root, denominator, negative_b;
            fast_vec_sub(
                &phase.vectors.relative, &phase.vectors.target, &missile_data->previous_target);
            FUN_L00_001ff2c0_c(&predicted, &phase.vectors.relative, &hero_velocity);
            qcopy(&missile_data->previous_target, &phase.vectors.target);
            a = FUN_L00_001ff430_c(&phase.vectors.relative);
            intercept_linear_coefficient = (phase.vectors.relative.f[0] * phase.collision.dir[0] +
                                            phase.vectors.relative.f[1] * phase.collision.dir[1] +
                                            phase.vectors.relative.f[2] * phase.collision.dir[2]) *
                                           -2.0f;
            a = missile_data->speed * missile_data->speed - a;
            root = square_root(intercept_linear_coefficient * intercept_linear_coefficient -
                               a * 4.0f * -FUN_L00_001ff430_c(phase.collision.dir));
            negative_b = -intercept_linear_coefficient;
            denominator = a + a;
            intercept_time_plus_root = (negative_b + root) / denominator;
            intercept_time_minus_root = (negative_b - root) / denominator;
            if (intercept_time_plus_root > 0.0f && intercept_time_minus_root > 0.0f) {
                if (intercept_time_minus_root < intercept_time_plus_root)
                    time = intercept_time_plus_root;
                else
                    time = intercept_time_minus_root;
            } else {
                if (intercept_time_plus_root > 0.0f)
                    time = intercept_time_plus_root;
                else {
                    time = -1.0f;
                    if (intercept_time_minus_root > 0.0f)
                        time = intercept_time_minus_root;
                }
            }
        }
        if (time > 0.0f && time < (float)scale_game_frames(300)) {
            scale_vector_xyz(&predicted, &phase.vectors.relative, time);
            fast_vec_add(&predicted, &predicted, phase.collision.dir);
            yaw = fast_arc_tan(predicted.f[0], predicted.f[1]);
            pitch = -fast_arc_tan(vector_length_xy(&predicted), predicted.f[2]);
        } else {
            yaw = fast_arc_tan(phase.collision.dir[0], phase.collision.dir[1]);
            pitch = -fast_arc_tan(vector_length_xy(phase.collision.dir), phase.collision.dir[2]);
        }
    } else {
        if (missile_data->life < scale_game_frames(300) - scale_game_frames(5)) {
            yaw = missile_data->yaw;
            pitch = missile_data->pitch;
        } else {
            yaw = missile_moby->rz;
            pitch = missile_moby->ry;
        }
    }
    if (target) {
        missile_moby->rz = spring_turn(&missile_data->yaw_velocity,
                                       missile_moby->rz,
                                       yaw,
                                       D_0015ED70 * 6.2831855f,
                                       D_0015ED70 * 3.1415927f,
                                       D_0015ED6C * 6.2831855f);
        missile_moby->ry = spring_turn(&missile_data->pitch_velocity,
                                       missile_moby->ry,
                                       pitch,
                                       D_0015ED70 * 6.2831855f,
                                       D_0015ED70 * 3.1415927f,
                                       D_0015ED6C * 6.2831855f);
    } else {
        missile_moby->rz = spring_turn(&missile_data->yaw_velocity,
                                       missile_moby->rz,
                                       yaw,
                                       D_0015ED70 * 6.2831855f,
                                       D_0015ED70 * 3.1415927f,
                                       D_0015ED6C * 4.71238899f);
        missile_moby->ry = spring_turn(&missile_data->pitch_velocity,
                                       missile_moby->ry,
                                       pitch,
                                       D_0015ED70 * 6.2831855f,
                                       D_0015ED70 * 3.1415927f,
                                       D_0015ED6C * 4.71238899f);
    }
    build_spherical_offset(&motion, missile_data->speed, missile_moby->rz, -missile_moby->ry);
    fast_vec_normalize(&impact1688.normal, &motion, 1.0f);
    scale_vector_xyz(&impact1688.normal, &impact1688.normal, -1.0f);
    if (!reduced || !(D_L00_0015F5CC & 1)) {
        unsigned char *smoke_particle;
        phase.vectors.relative.q = 0;
        phase.vectors.relative.f[0] = random_float_between(-1.0f, 1.0f);
        phase.vectors.relative.f[1] = random_float_between(-1.0f, 1.0f);
        phase.vectors.relative.f[2] = random_float_between(-1.0f, 1.0f);
        phase.vectors.direction.q = phase.vectors.relative.q;
        fast_vec_normalize(phase.collision.dir,
                           phase.collision.dir,
                           random_float_between(0.1f, 0.2f) * D_0015ED6C);
        fast_vec_normalize(
            &smoke_position, &motion, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
        fast_vec_add(phase.collision.dir, phase.collision.dir, &smoke_position);
        fast_vec_normalize(
            &smoke_position, &motion, random_float_between(0.0f, 1.0f) * missile_data->speed);
        fast_vec_add(&smoke_position, &smoke_position, (&missile_moby->pos));
        smoke_particle = smoke1688(&smoke_position,
                                   phase.collision.dir,
                                   scale_game_frames(40),
                                   50,
                                   0x505050,
                                   3,
                                   40000.0f,
                                   1000.0f,
                                   1.0f,
                                   -0.0002f,
                                   0.0f);
        if (smoke_particle) {
            unsigned char texture = *D_L00_001B20DC;
            smoke_particle[3] = 0x44;
            smoke_particle[2] = texture;
        }
        smoke1688((&missile_moby->pos),
                  phase.collision.dir,
                  scale_game_frames(7),
                  127,
                  0xB0B0B0,
                  3,
                  40000.0f,
                  1000.0f,
                  1.0f,
                  -0.0004f,
                  0.0f);
        phase.vectors.relative.q = 0;
        phase.vectors.relative.f[2] = 0.02f;
        transform_vector_by_basis(
            &phase.vectors.relative, &phase.vectors.relative, missile_moby->mtx);
        build_look_at_matrix(&phase.vectors.relative,
                             &phase.vectors.relative,
                             missile_moby->mtx,
                             random_angle_radians());
        {
            int color = swap_packed_color_channels(0x4F007FFF, D_0013E520.bB);
            spawn_burst(&smoke_position,
                        &phase.vectors.relative,
                        color,
                        0x1FFFFFFF,
                        scale_game_frames(15),
                        1,
                        25000.0f);
        }
    }
    qcopy(&previous, (&missile_moby->pos));
    FUN_L00_001ff290_c(phase.collision.dir, &motion, &hero_velocity);
    fast_vec_add((&missile_moby->pos), (&missile_moby->pos), &motion);
    if (missile_moby->pos.f[0] < 0.0f || missile_moby->pos.f[1] < 0.0f ||
        missile_moby->pos.f[2] < 0.0f) {
        mine_spawn_drops(missile_moby, D_L00_001B0770);
        mark_moby_for_removal(missile_moby);
        return;
    }
    phase.collision.moby = missile_moby;
    phase.collision.flags = 0x830000;
    phase.collision.fC = 3.0f;
    phase.collision.i10 = 1;
    qcopy(phase.collision.dir, &motion);
    set_vector_length_xy(phase.collision.dir, phase.collision.dir, 1.0f);
    phase.collision.dir[2] = 1.0f;
    phase.collision.dir[3] = 5627.925f;
    phase.collision.b8 = 3;
    phase.collision.cls = missile_moby->cls;
    phase.collision.b9 = 1;
    if (collision1688(&previous, (&missile_moby->pos), 0, D_0013F350.cam, &phase.collision)) {
        Impact1688 *hit = &impact1688;
        V1688      *hit_normal;
        if (hit->moby) {
            if (hit->moby != (M910 *)D_0013F350.cam && hit->moby != missile_data->owner) {
                missile_moby->bBC = 2;
                qcopy((&missile_moby->pos), &hit->position);
                fast_vec_sub(&surface_motion, &hit->velocity_end, &hit->velocity_start);
                hit_normal = &hit->normal;
                skip = missile_moby;
                fast_vec_normalize(&surface_motion, &surface_motion, 1.0f);
                reflect_vector(&blast_velocity, &motion, hit_normal);
                fast_vec_normalize(&blast_velocity, &blast_velocity, D_0015ED6C + D_0015ED6C);
                fast_vec_normalize(&normal, hit_normal, 1.0f);
            }
        } else if (hit->kind > 0) {
            missile_moby->bBC = 1;
            qcopy((&missile_moby->pos), &hit->position);
            fast_vec_sub(&surface_motion, &hit->velocity_end, &hit->velocity_start);
            hit_normal = &hit->normal;
            fast_vec_normalize(&surface_motion, &surface_motion, 1.0f);
            reflect_vector(&blast_velocity, &motion, hit_normal);
            fast_vec_normalize(&blast_velocity, &blast_velocity, D_0015ED6C + D_0015ED6C);
            fast_vec_normalize(&normal, hit_normal, 1.0f);
        }
    } else if (fast_dec_timer(&missile_data->life) ||
               missile_data->limit < vector_distance_xy((&missile_moby->pos), &D_0013F350.x80)) {
        qcopy(&normal, &D_L00_00160680);
        qcopy(&surface_motion, &D_L00_00160660);
        transform_vector_by_basis(&surface_motion, &surface_motion, missile_moby->mtx);
        transform_vector_by_basis(&normal, &normal, missile_moby->mtx);
        if (!D_0013E520.bB) {
            mine_spawn_drops(missile_moby, D_L00_001B0770);
            mark_moby_for_removal(missile_moby);
            return;
        }
        missile_moby->bBC = 1;
        fast_vec_add(&blast_velocity, &blast_velocity, &motion);
    }
    if (missile_moby->bBC) {
        float distance, near_adjust;
        int   particles;
        int   particle_reduction;
        int   i, j;
        int   hit =
            collect_mobies_in_sphere((&missile_moby->pos), 16, missile_moby, 0, (size + size));
        particles = 10;
        predicted.q = missile_moby->pos.q;
        FUN_L00_0025a9f8(missile_moby,
                         &predicted,
                         D_L00_00177F00,
                         hit,
                         (int)skip,
                         0x830000,
                         3,
                         1,
                         3.0f,
                         1.0f,
                         1.0f);
        particle_reduction = 0;
        basis[0][0] = 0.0f;
        basis[1][0] = -normal.f[2];
        basis[2][0] = normal.f[1];
        basis[3][0] = 0.0f;
        basis[0][1] = normal.f[2];
        basis[1][1] = 0.0f;
        basis[2][1] = -normal.f[0];
        basis[3][1] = 0.0f;
        basis[0][2] = -normal.f[1];
        basis[1][2] = normal.f[0];
        basis[2][2] = 0.0f;
        basis[3][2] = 0.0f;
        basis[0][3] = 0.0f;
        basis[1][3] = 0.0f;
        basis[2][3] = 0.0f;
        basis[3][3] = 0.0f;
        multiply_matrix(basis_squared, basis, basis);
        if (reduced) {
            particle_reduction = 1;
            particles /= 3;
        }
        {
            int remaining;
            for (remaining = 0; remaining < particles; remaining++) {
                if (missile_moby->bBC == 1) {
                    float a = random_angle_radians();
                    FUN_L00_001ffd48_c(rotation_part, basis_squared, 1.0f - fast_cos(a));
                    FUN_L00_001ffd48_c(diagonal, basis, fast_sin(a));
                    FUN_L00_001ffe20_c(diagonal, diagonal, rotation_part);
                    set_identity_matrix(rotation_part);
                    FUN_L00_001ffe20_c(rotation, diagonal, rotation_part);
                    transform_vector(&predicted, &surface_motion, rotation);
                    fast_vec_normalize(&predicted,
                                       &predicted,
                                       random_float_between(8.5f, 16.5f) * D_0015ED6C * size);
                } else {
                    float speed = random_float_between(8.5f, 16.5f) * D_0015ED6C * size;
                    float a = random_angle_radians();
                    build_spherical_offset(&predicted, speed, a, random_angle_radians());
                }
                predicted.f[2] += D_0015ED6C * 5.0f;
                fast_vec_add(&predicted, &predicted, &blast_velocity);
                {
                    int   color = swap_packed_color_channels(0x4F007FFF, D_0013E520.bB);
                    int   fade = swap_packed_color_channels(0x1F00007F, D_0013E520.bB);
                    int   life;
                    float particle_scale;
                    if (fast_vec_length(&predicted) > 1.0f)
                        fast_vec_normalize(&predicted, &predicted, D_0015ED6C * 8.0f);
                    particle_scale = size * 40000.0f;
                    life = FUN_L00_00257b90(scale_game_frames(60), scale_game_frames(120)) -
                           particle_reduction * 35;
                    spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
                        (&missile_moby->pos),
                        &predicted,
                        color,
                        fade,
                        particle_scale,
                        life,
                        1,
                        -1,
                        -1);
                }
            }
        }
        {
            V1688 random;
            random.q = 0;
            random.f[0] = random_float_between(-1.0f, 1.0f);
            random.f[1] = random_float_between(-1.0f, 1.0f);
            random.f[2] = random_float_between(-1.0f, 1.0f);
            predicted.q = random.q;
        }
        fast_vec_sub(&camera_delta, D_L00_00166DC0, (&missile_moby->pos));
        distance = fast_vec_length(&camera_delta);
        camera_delta.f[2] += distance * 0.5f;
        fast_vec_normalize(&predicted, &predicted, distance / 5.0f * D_0015ED6C * size);
        fast_vec_normalize(&camera_delta, &camera_delta, (distance + distance) * D_0015ED6C);
        fast_vec_add(&predicted, &predicted, &camera_delta);
        clamp_vector_length(&predicted, &predicted, D_0015ED6C * 10.0f);
        FUN_L00_002af450((char *)(&missile_moby->pos),
                         (char *)&predicted,
                         FUN_L00_00257b90(scale_game_frames(60), scale_game_frames(90)),
                         0,
                         0);
        if (distance < 6.0f)
            j = truncate_float_to_s32(distance) / 2;
        else
            j = 3;
        near_adjust = 0.0f;
        if (distance < 7.0f)
            near_adjust = 7.0f - distance;
        if (reduced)
            j /= 2;
        for (i = 0; i < j; i++) {
            float speed =
                random_float_between(8.0f, 10.0f) * size * D_0015ED6C - near_adjust * D_0015ED6C;
            Colors1688 color = l00_devastator_missile_impact_color_palette;
            Colors1688 fade = l00_devastator_missile_impact_fade_palette;
            int        c =
                swap_packed_color_channels(color.colors[random_integer_below(6)], D_0013E520.bB);
            int f = swap_packed_color_channels(fade.colors[random_integer_below(6)], D_0013E520.bB);
            int life = FUN_L00_00257b90(scale_game_frames(15), scale_game_frames(20)) -
                       particle_reduction * 7;
            int end = FUN_L00_00257b90(scale_game_frames(25), scale_game_frames(30)) -
                      particle_reduction * 10;
            int white_life, white_end;
            FUN_L00_0026a9f0(
                (&missile_moby->pos), &blast_velocity, c, f, 400000.0f, life, end, 0, 0, speed);
            white_life = FUN_L00_00257b90(scale_game_frames(5), scale_game_frames(10)) -
                         particle_reduction * 5;
            white_end = FUN_L00_00257b90(scale_game_frames(15), scale_game_frames(20)) -
                        particle_reduction * 7;
            FUN_L00_0026a9f0((&missile_moby->pos),
                             &blast_velocity,
                             0x7FFFFFFF,
                             0xFFFFFF,
                             400000.0f,
                             white_life,
                             white_end,
                             0,
                             0,
                             speed * 0.5f);
        }
        for (i = 0; i < particles; i++) {
            Colors1688 color = l00_devastator_missile_impact_color_palette;
            Colors1688 fade = l00_devastator_missile_impact_fade_palette;
            V1688      v;
            int        c, f, life;
            float      debris_scale;
            {
                V1688 random;
                random.q = 0;
                random.f[0] = random_float_between(-1.0f, 1.0f);
                random.f[1] = random_float_between(-1.0f, 1.0f);
                random.f[2] = random_float_between(-1.0f, 1.0f);
                v.q = random.q;
            }
            fast_vec_normalize(&v, &v, random_float_between(0.0f, 3.0f) * D_0015ED6C * size);
            c = swap_packed_color_channels(color.colors[random_integer_below(6)], D_0013E520.bB);
            f = swap_packed_color_channels(fade.colors[random_integer_below(6)], D_0013E520.bB);
            debris_scale = size * 200000.0f;
            life = FUN_L00_00257b90(scale_game_frames(20), scale_game_frames(35)) -
                   particle_reduction * 10;
            FUN_L00_0026a4c8_c((&missile_moby->pos), &v, c, f, debris_scale, life);
        }
        if (D_L00_0015F5D0 < 0.95f && distance > 9.0f) {
            int c = 127, f = 127;
            if (D_0013E520.bB) {
                c = 60;
                f = 60;
            }
            spawn_glow_particle_u8(missile_moby,
                                   (&missile_moby->pos),
                                   &blast_velocity,
                                   size * 4.0f,
                                   scale_game_frames(15),
                                   c,
                                   127,
                                   f,
                                   32);
            spawn_glow_particle_u8(missile_moby,
                                   (&missile_moby->pos),
                                   &blast_velocity,
                                   size * 4.0f,
                                   scale_game_frames(24),
                                   c,
                                   127,
                                   f,
                                   32);
        }
        {
            unsigned char color = 127;
            int           life;
            float         flash_scale = size * 4.0f;
            if (D_0013E520.bB)
                color = 32;
            life = scale_game_frames(20);
            spawn_glow_particle_u8(missile_moby,
                                   (&missile_moby->pos),
                                   &blast_velocity,
                                   flash_scale,
                                   life,
                                   color,
                                   127,
                                   0,
                                   48);
        }
        if (!reduced)
            spawn_glow_particle_u8(missile_moby,
                                   (&missile_moby->pos),
                                   &blast_velocity,
                                   (size + size),
                                   scale_game_frames(19),
                                   255,
                                   255,
                                   255,
                                   32);
        motion.f[0] = 0.0f;
        missile_moby->bBC = 0;
        l00_camera.shake_amount =
            distance < 20.0f ? 0.4f - distance * 0.0175f : 0.4f - 20.0f * 0.0175f;
        l00_camera.shake_frames = scale_game_frames(25);
        allocate_voice_for_target_entry(0, 0, (int)missile_moby);
        if (!reduced) {
            if (D_0013E520.bB)
                mine_camera_blast((char *)&gold_effect, (char *)(&missile_moby->pos), 0, 0);
            else
                mine_camera_blast((char *)&effect, (char *)(&missile_moby->pos), 0, 0);
        }
        missile_data->bounce_count++;
        if (!D_0013E520.bB || missile_data->bounce_count == 3) {
            mine_spawn_drops(missile_moby, D_L00_001B0770);
            mark_moby_for_removal(missile_moby);
            return;
        }
        {
            V1688 bounce[2];
            missile_data->target = 0;
            missile_moby->bBC = 0;
            fast_vec_normalize(&impact1688.normal, &impact1688.normal, 1.0f);
            reflect_vector(&bounce[0], &motion, &impact1688.normal);
            missile_moby->rz = fast_arc_tan(bounce[0].f[0], bounce[0].f[1]);
            missile_moby->ry = -fast_arc_tan(vector_length_xy(&bounce[0]), bounce[0].f[2]);
            missile_data->yaw_velocity = 0.0f;
            missile_data->pitch_velocity = 0.0f;
            fast_vec_normalize(&bounce[1], &impact1688.normal, 0.05f);
            fast_vec_add((&missile_moby->pos), (&missile_moby->pos), &bounce[1]);
            if (impact1688.moby)
                missile_data->owner = impact1688.moby;
            if (!missile_data->bounce_timer)
                missile_data->bounce_timer = scale_game_frames(90);
            select_gold_devastator_missile_target(
                (char *)missile_moby, (O *)missile_data, (char *)impact1688.moby);
            allocate_voice_for_target_entry(1, 0, (int)missile_moby);
        }
    }
    if (fast_dec_timer(&missile_data->bounce_timer))
        missile_data->bounce_count = 0;
}

Colors1688 l00_devastator_missile_impact_color_palette __asm__("D_L00_001E99F0") = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 l00_devastator_missile_impact_fade_palette __asm__("D_L00_001E9A08") = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};
