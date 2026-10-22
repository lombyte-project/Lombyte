/* Level 02 moby classes: sand_shark, constructorbot, elevator, bulldozer, sand_shark_nest, grab_crane, platform, transport_pod, crane, rocket_launcher, rocket_door1, tresspasser_door1, tresspasser_door2, sandshark_egg, rocks. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/gameplay/entities/moby.h"

#define NOT_SDA

#define MACRO_ADDR

/* Mark every moby of class 0x1B1 in the active level. */
/* Ported from rac1-decomp (src/overlays/l02/initonce_00240A00.c: func_L02_00240A00), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x34];
    unsigned short flags;
    char pad36[0x70];
    short class_id;
} Level02Moby;

typedef struct {
    char pad0[0x44];
    short count;
    char pad46[0x132];
    Level02Moby *mobys[1];
} Level02MobyList;


extern Level02MobyList talk_camera __asm__("D_L02_0016CE60");
void set_level02_class_1b1_moby_flag_0800(void) __asm__("FUN_L02_002400c8");

void set_level02_class_1b1_moby_flag_0800(void) {
    int i;
    if (current_level_index == 2) {
        for (i = 0; i < talk_camera.count; i++) {
            Level02Moby *moby = talk_camera.mobys[i];
            if (moby->class_id == 0x1B1) {
                moby->flags |= 0x800;
            }
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002A59D8), where it is exact; names translated to the US level program. */

extern void prepare_scaled_table_with_byte_128(void *) __asm__("FUN_L02_002a40d0");

void FUN_L02_002a47f8(void);
void FUN_L02_002a47f8(void) {
    char scratch[0x40];
    prepare_scaled_table_with_byte_128(scratch);
}
extern void write_dma_channel(void *, int, int) __asm__("FUN_001f9928");
extern void FUN_L02_002101b8(void *, int, int);

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002A59F8), where it is exact; names translated to the US level program. */

extern char D_L02_001CB400[];
extern void build_interpolated_dma_packet(void *) __asm__("FUN_L02_002a40f0");

void begin_animated_frame_rendering(int fog_red, int fog_green, int fog_blue, int ticks_per_frame, float x, float y, float z, float w, int frame_table_start_index, int frame_count,
                      int frame_packet_address) __asm__("FUN_L02_002a4818");

void begin_animated_frame_rendering(int a, int b, int c, int d, float x, float y, float z, float w, int e, int f,
                      int g) __asm__("FUN_L02_002a4818");
void begin_animated_frame_rendering(int fog_red, int fog_green, int fog_blue, int ticks_per_frame, float x, float y, float z, float w, int frame_table_start_index, int frame_count,
                      int frame_packet_address) {
    char scratch[0x40];
    scratch[0x30] = fog_red;
    scratch[0x31] = fog_green;
    scratch[0x32] = fog_blue;
    *(float *)(scratch + 0x20) = x;
    *(float *)(scratch + 0x24) = y;
    *(float *)(scratch + 0x28) = z;
    *(float *)(scratch + 0x2C) = w;
    *(short *)(scratch + 0x3C) = frame_table_start_index;
    scratch[0x3E] = frame_count;
    scratch[0x33] = ticks_per_frame;
    ((unsigned char *)scratch)[0x3F] = 0x80;
    *(int *)(scratch + 0x1C) = frame_packet_address;
    build_interpolated_dma_packet(scratch);
    FUN_L02_002101b8(D_L02_001CB400, 0x70002800, 0xF8);
}
#include "rnc/overlay/quad.h"
extern float D_L02_001619D4 __attribute__((sda));
extern float D_L02_001619DC __attribute__((sda));
extern unsigned short D_L02_001619E0 __attribute__((sda));
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int random_integer_below(int) __asm__("FUN_00213260");
/* Dust puff emitter, declared in the argument order the other levels use (pos, three floats, n, vel, size, colour). */
extern unsigned char *FUN_L00_0026d000(void *pos, float r, float lo, float hi, int n, void *vel, float size, int color);

/* Emits n dust puffs evenly spaced along the segment from a to b. */
void emit_sand_shark_dust_puffs_along_segment(void *moby, void *segment_start, void *b, int puff_count, float sa, float sb) __asm__("FUN_L02_002d3a50");

void emit_sand_shark_dust_puffs_along_segment(void *moby, void *segment_start, void *b, int puff_count, float sa, float sb) {
    float step[4];
    float dust_puff_position[4];
    float dust_puff_velocity[4];
    int i;
    int r, g, color;
    int spin;
    float scale;
    float size_delta_per_puff;
    unsigned char *dust_puff;
    unsigned char *dust_puff_extension;

    fast_vec_sub(step, b, segment_start);
    fast_vec_scale(step, step, 1.0f / (float)puff_count);
    *(OvlQuad *)dust_puff_velocity = (OvlQuad)0;
    dust_puff_velocity[2] = 0.01f;
    dust_puff_velocity[3] = 1.0f;
    size_delta_per_puff = (sb - sa) / (float)puff_count;
    for (i = 0; i < puff_count; i++) {
        fast_vec_scale(dust_puff_position, step, convert_integer_to_float(i));
        fast_vec_add(dust_puff_position, segment_start, dust_puff_position);
        r = random_int_between(0x40, 0x70);
        g = random_int_between(0x40, 0x7F);
        color = g | (g << 16 | g << 8);
        scale = random_float_between_alt(1.0f, 1.02f);
        spin = random_int_between(-2, 2);
        dust_puff_velocity[0] = randf_sym(0.0f, 0.0025f);
        dust_puff_velocity[1] = randf_sym(0.0f, 0.0025f);
        dust_puff_velocity[2] = random_float_between_alt(D_L02_001619D4 * 0.1f, D_L02_001619D4);
        dust_puff = FUN_L00_0026d000(dust_puff_position, D_L02_001619DC, 1.0f, scale, spin, dust_puff_velocity, size_delta_per_puff + sa * 210000.0f, r << 24 | color);
        if (dust_puff) {
            dust_puff_extension = dust_puff + 0x20;
            if (random_integer_below(2)) {
                dust_puff[3] = 0x44;
            }
            *(unsigned short *)(dust_puff + 0xA) = D_L02_001619E0;
            *(int *)(dust_puff_extension + 4) = 2;
            dust_puff_extension[0xA] = r;
            dust_puff_extension[0xB] = D_L02_001619E0;
        }
    }
}
#include "qcopy.h"

/* Transforms two table vectors by the moby's matrix and passes them on. */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D5098), where it is exact; names translated to the US level program. */

extern float D_L02_001D3230[][4] __attribute__((section(".data")));
float D_L02_001619C0[4] __attribute__((section(".sdata"))) = {-3.25f, 0.0f, 0.2f, 0.0f};
extern short D_L02_001619D0 __attribute__((sda));
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void emit_sand_shark_dust_puffs_along_segment(void *, void *, void *, int, float, float) __asm__("FUN_L02_002d3a50");

void emit_sand_shark_dust_segment(struct Moby *moby, int segment_start_index, int segment_end_index, int puff_count) __asm__("FUN_L02_002d3c98");

void emit_sand_shark_dust_segment(struct Moby *moby, int segment_start_index, int segment_end_index, int puff_count) {
    float va[4];
    float vb[4];
    float *pa = D_L02_001D3230[segment_start_index];
    float *pb;
    qcopy(va, pa);
    va[3] = 1.0f;
    fast_vec_sub(va, va, D_L02_001619C0);
    transform_vector_by_basis(va, va, &moby->rot_row0);
    fast_vec_add(va, va, &moby->pos);
    pb = D_L02_001D3230[segment_end_index];
    qcopy(vb, pb);
    vb[3] = 1.0f;
    fast_vec_sub(vb, vb, D_L02_001619C0);
    transform_vector_by_basis(vb, vb, &moby->rot_row0);
    fast_vec_add(vb, vb, &moby->pos);
    emit_sand_shark_dust_puffs_along_segment(moby, va, vb, puff_count, pa[3] * *(float *)&D_L02_001619D0,
                     pb[3] * *(float *)&D_L02_001619D0);
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D51D0), where it is exact; names translated to the US level program. */

extern void emit_sand_shark_dust_segment(struct Moby *moby, int a, int b, int d) __asm__("FUN_L02_002d3c98");

int D_L02_001619EC = 0xa;
int D_L02_001619F0 = 0xa;
int D_L02_001619F4 = 8;
int D_L02_001619F8 = 8;
void FUN_L02_002d3dd0(void *moby) {
    emit_sand_shark_dust_segment(moby, 0, 1, D_L02_001619F0);
    emit_sand_shark_dust_segment(moby, 2, 3, D_L02_001619EC);
    emit_sand_shark_dust_segment(moby, 4, 5, D_L02_001619F4);
    emit_sand_shark_dust_segment(moby, 6, 7, D_L02_001619F4);
    emit_sand_shark_dust_segment(moby, 8, 9, D_L02_001619F8);
}
/* pvars of the Aridia enemy sand_shark_update updates (it leaves its burrow, walks, jumps and digs back in). */
typedef struct {
    /* 0x000 */ char pad0[0x20];
    /* 0x020 */ float unk20;
    /* 0x024 */ short unk24;
    /* 0x026 */ char pad26[3];
    /* 0x029 */ unsigned char unk29;
    /* 0x02A */ char pad2A[4];
    /* 0x02E */ unsigned char unk2E;
    /* 0x02F */ char pad2F[0x11];
    /* 0x040 */ OvlVec4 vel;
    /* 0x050 */ char pad50[8];
    /* 0x058 */ unsigned char unk58;
    /* 0x059 */ char pad59[3];
    /* 0x05C */ unsigned char unk5C;
    /* 0x05D */ char pad5D[0x6B];
    /* 0x0C8 */ short unkC8;
    /* 0x0CA */ char padCA[6];
    /* 0x0D0 */ void *unkD0;
    /* 0x0D4 */ char padD4[0x4C];
    /* 0x120 */ char unk120[0x60];
    /* 0x180 */ int walk_mode;
    /* 0x184 */ float walk_184;
    /* 0x188 */ float walk_188;
    /* 0x18C */ float walk_18C;
    /* 0x190 */ char pad190[8];
    /* 0x198 */ float fall_speed;
    /* 0x19C */ char pad19C[8];
    /* 0x1A4 */ float walk_step;
    /* 0x1A8 */ char pad1A8[0x28];
    /* 0x1D0 */ OvlVec4 target;
    /* 0x1E0 */ char pad1E0[0x34];
    /* 0x214 */ int unk214;
    /* 0x218 */ char pad218[8];
    /* 0x220 */ OvlVec4 home;
    /* 0x230 */ OvlVec4 unk230;
    /* 0x240 */ float turn_vel;
    /* 0x244 */ float turn_offset;
    /* 0x248 */ float turn_range;
    /* 0x24C */ float unk24C;
    /* 0x250 */ char pad250[4];
    /* 0x254 */ float unk254;
    /* 0x258 */ int unk258;
    /* 0x25C */ short unk25C;
    /* 0x25E */ short unk25E;
    /* 0x260 */ int turn_timer;
    /* 0x264 */ char pad264[8];
    /* 0x26C */ int unk26C;
    /* 0x270 */ int unk270;
    /* 0x274 */ int unk274;
    /* 0x278 */ int unk278;
    /* 0x27C */ int unk27C;
    /* 0x280 */ char pad280[4];
    /* 0x284 */ int unk284;
    /* 0x288 */ int unk288;
    /* 0x28C */ int unk28C;
    /* 0x290 */ int path;
    /* 0x294 */ float unk294;
    /* 0x298 */ int unk298;
    /* 0x29C */ int unk29C;
    /* 0x2A0 */ char unk2A0[0x10];
} Varsd3e50;

extern float D_0015ED6C;
extern struct Moby *D_L02_0015FFD8_e50 __asm__("D_L02_0015FFD8") __attribute__((section(".sdata")));
extern int D_L02_0015F5CC_e50 __asm__("D_L02_0015F5CC");
extern int D_001413D4_e50 __asm__("D_001413D4") __attribute__((section(".data")));
extern int *D_L02_001B0AB0_e50[] __asm__("D_L02_001B0AB0");
extern char D_L02_001673C0_e50[] __asm__("D_L02_001673C0");
extern char D_L02_001FA8A0[];
extern char D_L02_001FA8C0[];
extern void FUN_001e93b0_e50(char *, short) __asm__("FUN_001e93b0");
extern int rand_e50(void) __asm__("FUN_001160d8");
extern int scale_ticks_e50(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern float square_root(float) __asm__("FUN_001f9988");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001f9af0_e50(void *) __asm__("FUN_001f9af0");
extern float distance_e50(void *, void *) __asm__("FUN_001f9b48");
extern float FUN_001f9b80_e50(void *, void *) __asm__("FUN_001f9b80");
extern void FUN_001f9bf8_e50(void *, void *, float) __asm__("FUN_001f9bf8");
extern float fast_cos_e50(float) __asm__("FUN_001f9dc8");
extern float fast_sin_e50(float) __asm__("FUN_001f9de0");
extern float atan2_e50(float, float) __asm__("FUN_001f9e90");
extern float add_rotations_e50(float, float) __asm__("FUN_001fa580");
extern float sub_rotations_e50(float, float) __asm__("FUN_001fa5c8");
extern void remove_moby_e50(void *) __asm__("FUN_0020c828");
extern void blend_e50(void *, int, int, int) __asm__("FUN_00212f90");
extern int rand_below_e50(int) __asm__("FUN_00213260");
extern float rand_between_e50(float, float) __asm__("FUN_002132a8");
extern float ground_e50(void *, int, float) __asm__("FUN_00213508");
extern float approach_e50(float *, float, float) __asm__("FUN_00213ed8");
extern int inside_e50(void *, int) __asm__("FUN_00214720");
extern int in_window_e50(void *, float) __asm__("FUN_00214cc8");
extern int sound_e50(int, int, void *) __asm__("FUN_0022da68");
extern void FUN_L00_00257470_e50(void *, int, int) __asm__("FUN_L00_00257470");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void FUN_L00_00257d78_e50(void *, float, float) __asm__("FUN_L00_00257d78");
extern void FUN_L00_00258278_c(void *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern void FUN_L00_00258b50(void *, void *, void *, void *, float);
extern void FUN_L00_002598b0_e50(void *, void *, float, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void FUN_L00_0025a120_e50(void *) __asm__("FUN_L00_0025a120");
extern void turn_angle_e50(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern int FUN_L00_0025c698_e50(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L00_00261b48(int, void *, void *, void *);
extern void FUN_L00_00263ac8_e50(void *, int, void *, float) __asm__("FUN_L00_00263ac8");
extern void FUN_L00_00269958_e50(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern int FUN_L00_002dc9f0_e50(void *, void *) __asm__("FUN_L00_002dc9f0");
extern void FUN_L01_0026d930_e50(void *) __asm__("FUN_L01_0026d930");
extern void FUN_L02_002d3dd0_e50(void *) __asm__("FUN_L02_002d3dd0");
extern int FUN_L02_002d5658_e50(void *) __asm__("FUN_L02_002d5658");
extern void FUN_L02_002d57f8_e50(void *, int) __asm__("FUN_L02_002d57f8");
extern void FUN_L02_002d5a68_e50(void *) __asm__("FUN_L02_002d5a68");
extern void FUN_L02_002d5c88_e50(void *) __asm__("FUN_L02_002d5c88");
extern void FUN_L02_002d63c0_e50(void *) __asm__("FUN_L02_002d63c0");

int D_L02_001619A8 = 0;
extern unsigned char D_L02_00161900 __attribute__((sda));
extern float D_L02_0016191C __attribute__((sda));
int D_L02_001619B0 = 0xa000000;
int D_L02_001619E4 = 0x40306058;
int D_L02_001619E8 = 0x306058;
/* Aridia burrowing enemy: profiled with EE timer 0 around its state machine. */
void sand_shark_update(struct Moby *moby) __asm__("FUN_L02_002d3e50");
void sand_shark_update(struct Moby *moby) {
    enum {
        SAND_SHARK_INITIALIZE = 0,
        SAND_SHARK_BEGIN_RETURN_HOME_WITH_HEIGHT_OFFSET = 1,
        SAND_SHARK_WANDER_AROUND_HOME = 2,
        SAND_SHARK_WAIT_FOR_TARGET_IN_VOLUME = 3,
        SAND_SHARK_APPROACH_TARGET = 5,
        SAND_SHARK_CLOSE_RANGE_ATTACK = 6,
        SAND_SHARK_RETURN_HOME = 7,
        SAND_SHARK_KNOCKBACK_UNTIL_LANDING = 8,
        SAND_SHARK_WAIT_FOR_HERO_LAUNCH_TRIGGER = 12,
        SAND_SHARK_BALLISTIC_FLIGHT = 13,
        SAND_SHARK_APPROACH_TARGET_WITH_PATH_PROXIMITY_CHECK = 14,
        SAND_SHARK_FACE_TARGET = 15,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET_WITH_HEIGHT_OFFSET = 17,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_RETURN_HOME_WITH_HEIGHT_OFFSET = 18,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET = 19,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_RETURN_HOME = 20,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET_WITH_PATH_PROXIMITY_CHECK = 21,
        SAND_SHARK_APPROACH_TARGET_WITH_HEIGHT_OFFSET = 22,
        SAND_SHARK_RETURN_HOME_WITH_HEIGHT_OFFSET = 23,
        SAND_SHARK_DEATH_KNOCKBACK = 24,
        SAND_SHARK_FADE_OUT = 25,
        SAND_SHARK_INACTIVE_WAIT_FOR_RELAUNCH = 26,
        SAND_SHARK_BALLISTIC_FLIGHT_WITH_PARTICLES = 27,
        SAND_SHARK_WAIT_FOR_LINKED_MOBY_BEFORE_INITIALIZE = 29
    };
    Varsd3e50 *vars;
    OvlVec4 position_or_offset_scratch;
    OvlVec4 target_or_particle_vector;
    OvlVec4 target_home_delta;
    OvlVec4 navigation_point;
    OvlVec4 particle_position_or_forward_step;
    float f;

    if (D_L02_001619A8 != 0) {
        return;
    }
    *(volatile unsigned int *)0x10000010 = 0x81;
    *(volatile unsigned int *)0x10000000 = 0;
    vars = (Varsd3e50 *)moby->pvars;
    FUN_L00_00263ac8_e50(moby, 0, vars->unk2A0, 2.1f);
    if (moby->visible != 0 && distance_e50(&moby->pos, D_L02_001673C0_e50) < 28.0f) {
        FUN_L00_0025a120_e50(moby);
        moby->shadow_range = 0x16;
    }
    FUN_L02_002d5c88_e50(moby);
    *(volatile unsigned int *)0x10000000;
    switch (moby->state) {
    case SAND_SHARK_INITIALIZE:
        if (vars->path == -1) {
            FUN_001e93b0_e50(D_L02_001FA8A0, moby->save_id);
            remove_moby_e50(moby);
            return;
        }
        if (*D_L02_001B0AB0_e50[vars->path] == 0) {
            FUN_001e93b0_e50(D_L02_001FA8C0, moby->save_id);
            remove_moby_e50(moby);
            return;
        }
        if (vars->unk288 >= 0) {
            extern struct Moby *D_L02_0015FFD8_e50s __asm__("D_L02_0015FFD8") __attribute__((sda));
            (*(int *)(D_L02_0015FFD8_e50s[vars->unk288].pvars + 0x14C))++;
        }
        qcopy(&vars->home, &moby->pos);
        qcopy(&vars->unk230, &moby->pos);
        vars->unk24 = 1;
        vars->unk20 = 1.0f;
        vars->unk58 = D_L02_00161900;
        vars->unk5C = 1;
        vars->unk2E = 1;
        vars->unk29 = 1;
        vars->unk25E = 0;
        FUN_L01_0026d930_e50(&vars->walk_mode);
        vars->walk_188 = vars->walk_18C = 2.0f;
        vars->walk_mode = 0x200;
        vars->walk_184 = 0.5f;
        vars->walk_step = vars->unk254 * D_0015ED6C;
        if (rand_e50() & 1) {
            moby->flags |= 0x8000;
        }
        vars->unkD0 = &D_L02_001619B0;
        if (vars->unk26C >= 0) {
            if (vars->unk28C == -1) {
                remove_moby_e50(moby);
                return;
            }
            moby->state = 0xC;
            moby->collision = 0;
            moby->pos.z += 10.0f;
            moby->flags = (moby->flags | 1) & ~0x1000;
            if (moby->prev_seq != 0) {
                blend_e50(moby, 0, 0, scale_ticks_e50(3));
            }
        } else if (vars->unk278 >= 0) {
            vars->walk_18C = 0.2f;
            moby->state = 3;
            if (moby->prev_seq != 2) {
                blend_e50(moby, 2, 0, scale_ticks_e50(3));
            }
        } else if (vars->unk258 >= 0) {
            moby->state = 0x1D;
            moby->collision = 0;
            moby->flags = (moby->flags | 0x41) & ~0x1000;
        } else {
            moby->state = 1;
            if (moby->prev_seq != 2) {
                blend_e50(moby, 2, 0, scale_ticks_e50(3));
            }
        }
        break;
    case SAND_SHARK_WAIT_FOR_LINKED_MOBY_BEFORE_INITIALIZE:
        if (((struct Moby *)((char *)D_L02_0015FFD8_e50 + (vars->unk258 << 8)))->state >= 5 && D_001413D4_e50 != 0x72) {
            moby->flags = (moby->flags & 0xFFBE) | 0x1000;
            moby->collision = moby->pclass->collision;
            vars->unk258 = -1;
            vars->unk25C = scale_ticks_e50(0x1518);
            moby->state = 0;
            moby->pos.z -= D_L02_0016191C;
        }
        break;
    case SAND_SHARK_BEGIN_RETURN_HOME_WITH_HEIGHT_OFFSET:
        fast_vec_sub(&position_or_offset_scratch, &hero.motion.pos, &moby->pos);
        moby->state = 0x12;
        if (moby->prev_seq != 0) {
            blend_e50(moby, 0, 0, scale_ticks_e50(3));
        }
        break;
    case SAND_SHARK_WANDER_AROUND_HOME:
        moby->pos.z += D_L02_0016191C;
        qcopy(&position_or_offset_scratch, &vars->home);
        FUN_L00_00258278_c(moby, add_rotations_e50(atan2_e50(position_or_offset_scratch.f[0] - moby->pos.x, position_or_offset_scratch.f[1] - moby->pos.y), vars->turn_offset),
                 &vars->turn_vel, 0.05f, 0.3f, 0.04f);
        moby->pos.x += fast_cos_e50(moby->rot.z) * D_0015ED6C;
        moby->pos.y += fast_sin_e50(moby->rot.z) * D_0015ED6C;
        if (fast_dec_timer(&vars->turn_timer)) {
            vars->turn_timer = scale_ticks_e50(0x3C);
            f = vars->turn_range * 0.017453292f;
            vars->turn_offset = rand_between_e50(-f, f);
        }
        if (moby->visible != 0 && (rand_below_e50(0xFF) & 1)) {
            target_or_particle_vector.q = *(OvlQuad *)&moby->pos;
            FUN_L02_002d5a68_e50(&target_or_particle_vector);
        }
        moby->pos.z -= D_L02_0016191C;
        if (vars->unk214 != 2) {
            if (vars->unk274 >= 0) {
                if ((!inside_e50(&vars->target, vars->unk274) || vars->unk25E != 0) && rand_below_e50(9) == 0) {
                    vars->unk274 = -1;
                }
            } else if (rand_below_e50(4) == 0) {
                vars->turn_offset = vars->turn_range * 0.017453292f;
                if (vars->unk284 != 0) {
                    vars->turn_offset = vars->turn_offset * (float)vars->unk284;
                } else if (rand_below_e50(0x100) & 1) {
                    vars->turn_offset = -vars->turn_offset;
                }
                vars->turn_timer = scale_ticks_e50((int)rand_between_e50(30.0f, 90.0f));
                vars->walk_step = D_0015ED6C * 4.0f;
                moby->state = 0x11;
                if (moby->prev_seq != 0xC) {
                    blend_e50(moby, 0xC, 0, scale_ticks_e50(3));
                }
            }
        } else if (vars->unk25E != 0 && rand_below_e50(0x13) == 0) {
            moby->state = 0x15;
            sound_e50(4, 0, moby);
            if (moby->prev_seq != 0) {
                blend_e50(moby, 0, 0, 0);
            }
            vars->unk274 = -1;
        }
        break;
    case SAND_SHARK_WAIT_FOR_TARGET_IN_VOLUME:
        if (vars->unk214 != 2 && inside_e50(&vars->target, vars->unk278)) {
            moby->state = 5;
            if (moby->prev_seq != 0) {
                blend_e50(moby, 0, 0, scale_ticks_e50(3));
            }
        }
        break;
    case SAND_SHARK_APPROACH_TARGET_WITH_PATH_PROXIMITY_CHECK: {
        int hit14;
        qcopy(&target_or_particle_vector, &vars->target);
        hit14 = 0;
        FUN_L00_00258278_c(moby, atan2_e50(target_or_particle_vector.f[0] - moby->pos.x, target_or_particle_vector.f[1] - moby->pos.y) + vars->turn_offset, &vars->turn_vel,
                 0.05f, 0.3f, 0.2f);
        position_or_offset_scratch.f[0] = 2.0f * fast_cos_e50(moby->rot.z);
        position_or_offset_scratch.f[1] = 2.0f * fast_sin_e50(moby->rot.z);
        position_or_offset_scratch.f[2] = 0.0f;
        fast_vec_add(&position_or_offset_scratch, &position_or_offset_scratch, &moby->pos);
        FUN_L00_00258b50(moby, &vars->walk_mode, &position_or_offset_scratch, &target_home_delta, 1.0f);
        FUN_L02_002d63c0_e50(moby);
        if (FUN_L00_00261b48(vars->path, &moby->pos, &target_or_particle_vector, &navigation_point) && distance_e50(&moby->pos, &navigation_point) < vars->unk294) {
            hit14 = 1;
        }
        if (vars->unk25E != 0) {
            if (hit14) {
                moby->state = 0xF;
                if (moby->prev_seq != 5) {
                    blend_e50(moby, 5, 0, scale_ticks_e50(0xA));
                }
            } else if (FUN_001f9b80_e50(&moby->pos, &target_or_particle_vector) < 1.5f) {
                moby->state = 6;
                if (moby->prev_seq != 1) {
                    blend_e50(moby, 1, 0, scale_ticks_e50(0xA));
                }
            } else if (vars->unk214 != 2) {
                moby->state = 5;
                if (moby->prev_seq != 0) {
                    blend_e50(moby, 0, 0, scale_ticks_e50(0xF));
                }
            }
        } else {
            moby->state = 0x12;
        }
        break;
    }
    case SAND_SHARK_FACE_TARGET:
        turn_angle_e50(&moby->rot.z, atan2_e50(vars->target.f[0] - moby->pos.x, vars->target.f[1] - moby->pos.y),
                       &vars->turn_vel, frame_time_sq * 6.2831855f, frame_time_sq * 6.2831855f, D_0015ED6C * 6.2831855f);
        if (vars->unk25E != 0) {
            if (FUN_001f9b80_e50(&moby->pos, &vars->target) < 1.5f) {
                moby->state = 6;
                if (moby->prev_seq != 1) {
                    blend_e50(moby, 1, 0, scale_ticks_e50(0xA));
                }
            } else if (vars->unk214 != 2) {
                moby->state = 5;
                if (moby->prev_seq != 0) {
                    blend_e50(moby, 0, 0, scale_ticks_e50(0xF));
                }
            }
        } else {
            moby->state = 0x12;
        }
        break;
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET_WITH_HEIGHT_OFFSET:
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_RETURN_HOME_WITH_HEIGHT_OFFSET: {
        float g;
        moby->pos.z += D_L02_0016191C;
        g = ground_e50(&moby->pos, 0, 0.5f);
        if (g < vars->home.f[2]) {
            g = vars->home.f[2];
        }
        qcopy(&position_or_offset_scratch, &moby->pos);
        position_or_offset_scratch.f[2] = g;
        approach_e50(&moby->pos.z, g, D_0015ED6C * 4.0f);
        if (g == moby->pos.z) {
            if (moby->prev_seq != 0xC) {
                blend_e50(moby, 0xC, 0, scale_ticks_e50(5));
            }
            if (moby->state == 0x11) {
                moby->state = 0x16;
            } else {
                moby->state = 0x17;
            }
        }
        moby->pos.z -= D_L02_0016191C;
        if (scale_ticks_e50(0xA) < D_L02_0015F5CC_e50) {
            FUN_L02_002d57f8_e50(&position_or_offset_scratch, 6);
        }
        break;
    }
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET:
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_RETURN_HOME:
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET_WITH_PATH_PROXIMITY_CHECK: {
        float g;
        moby->pos.z += D_L02_0016191C;
        g = ground_e50(&moby->pos, 0, 0.5f);
        qcopy(&position_or_offset_scratch, &moby->pos);
        position_or_offset_scratch.f[2] = g;
        if (approach_e50(&moby->pos.z, g + D_L02_0016191C, D_0015ED6C * 4.0f) == 0.0f) {
            if (moby->state == 0x13) {
                moby->state = 5;
                if (moby->prev_seq != 0) {
                    blend_e50(moby, 0, 0, scale_ticks_e50(3));
                }
            } else if (moby->state == 0x14) {
                moby->state = 7;
                if (moby->prev_seq != 0) {
                    blend_e50(moby, 0, 0, scale_ticks_e50(3));
                }
            } else {
                vars->unk294 = rand_between_e50(2.0f, 3.5f);
                vars->walk_step = D_0015ED6C * 4.0f;
                moby->state = 0xE;
                if (moby->prev_seq != 0) {
                    blend_e50(moby, 0, 0, scale_ticks_e50(3));
                }
            }
        }
        moby->pos.z -= D_L02_0016191C;
        FUN_L02_002d57f8_e50(&position_or_offset_scratch, 6);
        break;
    }
    case SAND_SHARK_APPROACH_TARGET:
    case SAND_SHARK_APPROACH_TARGET_WITH_HEIGHT_OFFSET:
        if (moby->state == 0x16) {
            moby->pos.z += D_L02_0016191C;
            if (rand_below_e50(0xFF) & 1) {
                particle_position_or_forward_step.q = *(OvlQuad *)&moby->pos;
                FUN_L02_002d5a68_e50(&particle_position_or_forward_step);
            }
        }
        qcopy(&navigation_point, &vars->target);
        FUN_L00_00258278_c(moby, atan2_e50(navigation_point.f[0] - moby->pos.x, navigation_point.f[1] - moby->pos.y) + vars->turn_offset, &vars->turn_vel,
                 0.05f, 0.3f, 0.2f);
        position_or_offset_scratch.f[0] = 2.0f * fast_cos_e50(moby->rot.z);
        position_or_offset_scratch.f[1] = 2.0f * fast_sin_e50(moby->rot.z);
        position_or_offset_scratch.f[2] = 0.0f;
        fast_vec_add(&position_or_offset_scratch, &position_or_offset_scratch, &moby->pos);
        FUN_L00_00258b50(moby, &vars->walk_mode, &position_or_offset_scratch, &target_or_particle_vector, 1.0f);
        FUN_L02_002d63c0_e50(moby);
        if (moby->state == 0x16) {
            moby->pos.z -= D_L02_0016191C;
        }
        if (vars->unk214 != 2) {
            if (vars->unk278 >= 0 && !inside_e50(&vars->target, vars->unk278)) {
                moby->state = 7;
            } else {
            fast_vec_sub(&target_home_delta, &vars->target, &vars->home);
            if (FUN_001f9b80_e50(&moby->pos, &navigation_point) < 12.0f && moby->state == 0x16) {
                moby->state = 0x13;
                sound_e50(4, 0, moby);
                if (moby->prev_seq != 0) {
                    blend_e50(moby, 0, 0, scale_ticks_e50(3));
                }
            } else if (FUN_001f9b80_e50(&moby->pos, &navigation_point) < 1.5f && moby->state == 5) {
                moby->state = 6;
                if (moby->prev_seq != 1) {
                    blend_e50(moby, 1, 0, scale_ticks_e50(3));
                }
            }
            }
        } else if (vars->unk278 >= 0) {
            moby->state = 7;
        } else if (vars->unk25E != 0) {
            moby->state = 0xE;
        } else {
            moby->state = 0x12;
        }
        break;
    case SAND_SHARK_CLOSE_RANGE_ATTACK:
        FUN_L00_00258278_c(moby, atan2_e50(vars->target.f[0] - moby->pos.x, vars->target.f[1] - moby->pos.y), &vars->turn_vel,
                 0.05f, 0.3f, 0.2f);
        if (moby->seq == moby->prev_seq && in_window_e50(moby, 13.0f)) {
            FUN_001f9bf8_e50(&position_or_offset_scratch, &moby->rot_row0, 0.75f);
            fast_vec_add(&position_or_offset_scratch, &position_or_offset_scratch, &moby->pos);
            position_or_offset_scratch.f[2] += 0.5f;
            FUN_L00_002598b0_e50(moby, &position_or_offset_scratch, 0.5f, 1, 1.0f, 1.0f, 0, 1, 0);
        }
        if ((moby->anim_flags & 2) && FUN_001f9b80_e50(&moby->pos, &vars->target) > 1.5f) {
            moby->state = 5;
            if (moby->prev_seq != 0) {
                blend_e50(moby, 0, 0, scale_ticks_e50(3));
            }
        }
        {
        float g = ground_e50(&moby->pos, 0, 0.5f);
        vars->fall_speed -= frame_time_sq * 9.8f;
        moby->pos.z += vars->fall_speed;
        if (moby->pos.z < g) {
            vars->fall_speed = 0.0f;
            moby->pos.z = g;
        }
        }
        break;
    case SAND_SHARK_RETURN_HOME:
    case SAND_SHARK_RETURN_HOME_WITH_HEIGHT_OFFSET:
        if (moby->state == 0x17) {
            moby->pos.z += D_L02_0016191C;
            if (rand_below_e50(0xFF) & 1) {
                particle_position_or_forward_step.q = *(OvlQuad *)&moby->pos;
                FUN_L02_002d5a68_e50(&particle_position_or_forward_step);
            }
        }
        qcopy(&navigation_point, &vars->home);
        FUN_L00_00258278_c(moby, atan2_e50(navigation_point.f[0] - moby->pos.x, navigation_point.f[1] - moby->pos.y), &vars->turn_vel, 0.02f, 0.3f, 0.1f);
        position_or_offset_scratch.f[0] = 2.0f * fast_cos_e50(moby->rot.z);
        position_or_offset_scratch.f[1] = 2.0f * fast_sin_e50(moby->rot.z);
        position_or_offset_scratch.f[2] = 0.0f;
        fast_vec_add(&position_or_offset_scratch, &position_or_offset_scratch, &moby->pos);
        FUN_L00_00258b50(moby, &vars->walk_mode, &position_or_offset_scratch, &target_or_particle_vector, 1.0f);
        if (moby->state == 0x17) {
            moby->pos.z -= D_L02_0016191C;
        }
        if (FUN_001f9b80_e50(&moby->pos, &vars->home) < 0.5f) {
            vars->unk298 = 0;
            if (moby->state == 7) {
                if (vars->unk278 >= 0) {
                    moby->state = 3;
                    if (moby->prev_seq != 2) {
                        blend_e50(moby, 2, 0, scale_ticks_e50(3));
                    }
                } else {
                    moby->state = 0x12;
                    vars->walk_step = D_0015ED6C * 1.5f;
                    if (moby->prev_seq != 0) {
                        blend_e50(moby, 0, 0, scale_ticks_e50(3));
                    }
                }
            } else {
                moby->state = 2;
                vars->walk_step = D_0015ED6C * 1.5f;
                if (moby->prev_seq != 0xC) {
                    blend_e50(moby, 0xC, 0, scale_ticks_e50(5));
                }
            }
        }
        if (vars->unk298 == 0) {
            if (vars->unk214 != 2) {
                fast_vec_sub(&target_home_delta, &vars->target, &vars->home);
                if (vars->unk278 >= 0) {
                    if (inside_e50(&vars->target, vars->unk278)) {
                        moby->state = 5;
                        vars->walk_step = vars->unk254 * D_0015ED6C;
                    }
                } else if (FUN_001f9af0_e50(&target_home_delta) < vars->unk24C) {
                    if (moby->state == 7) {
                        moby->state = 5;
                    } else {
                        moby->state = 0x16;
                    }
                    vars->walk_step = vars->unk254 * D_0015ED6C;
                    if (moby->prev_seq != 0) {
                        blend_e50(moby, 0, 0, scale_ticks_e50(3));
                    }
                }
            } else if (vars->unk25E != 0 && rand_below_e50(0x13) == 0) {
                moby->state = 0x15;
                sound_e50(4, 0, moby);
                if (moby->prev_seq != 0) {
                    blend_e50(moby, 0, 0, 0);
                }
                vars->unk274 = -1;
            }
        } else {
            extern float D_0015ED6C_e50s __asm__("D_0015ED6C") __attribute__((sda));
            FUN_001f9bf8_e50(&particle_position_or_forward_step, &moby->rot_row0, D_0015ED6C_e50s * 0.5f);
            fast_vec_add(&moby->pos, &moby->pos, &particle_position_or_forward_step);
        }
        break;
    case SAND_SHARK_KNOCKBACK_UNTIL_LANDING:
        if (FUN_L00_0025c698_e50(moby, vars->unk120) & 1) {
            moby->state = 0xB;
            if (moby->prev_seq != 4) {
                blend_e50(moby, 4, 8, scale_ticks_e50(3));
            }
            return;
        }
        if (moby->pos.z < 0.0f) {
            FUN_L02_002d3dd0_e50(moby);
            remove_moby_e50(moby);
            return;
        }
        break;
    case 9:
        if (FUN_L00_002dc9f0_e50(moby, vars->unk120)) {
            moby->state = 1;
            vars->unkC8 = 0;
        }
        break;
    case SAND_SHARK_WAIT_FOR_HERO_LAUNCH_TRIGGER: {
        int in;
        if (inside_e50(&hero.motion.pos, vars->unk26C)) {
            in = 1;
        } else {
            in = inside_e50(&hero.motion.pos, vars->unk270) != 0;
        }
        if (in) {
            struct Moby *dest = &D_L02_0015FFD8_e50[vars->unk28C];
            float dz = dest->pos.z - vars->home.f[2];
            float up = rand_between_e50(6.0f, 10.0f) * D_0015ED6C;
            float flight_duration = (square_root(2.0f * (frame_time_sq * 10.8f) * dz) + 2.0f * up) / (frame_time_sq * 10.8f);
            float speed = FUN_001f9b80_e50(&vars->home, &dest->pos) / flight_duration;

            fast_vec_sub(&vars->vel, &vars->home, &dest->pos);
            vars->vel.f[2] = 0.0f;
            FUN_001f9bf8_e50(&vars->vel, &vars->vel, speed);
            vars->vel.f[2] = up;
            qcopy(&moby->pos, &dest->pos);
            moby->collision = moby->pclass->collision;
            moby->flags = (moby->flags & 0xFFFE) | 0x1000;
            if (moby->prev_seq != 0xB) {
                blend_e50(moby, 0xB, 0, 0);
            }
            moby->state = 0xD;
        }
        break;
    }
    case SAND_SHARK_BALLISTIC_FLIGHT_WITH_PARTICLES: {
        int life;

        FUN_L00_00257d78_e50(&target_or_particle_vector, 0.0f, 0.2f);
        fast_vec_add(&position_or_offset_scratch, &target_or_particle_vector, &moby->pos);
        fast_vec_scale(&target_or_particle_vector, &target_or_particle_vector, D_0015ED6C * 5.0f);
        life = scale_ticks_e50(random_int_between(0x14, 0x28));
        FUN_L00_00269958_e50(&moby->pos, &target_or_particle_vector, D_L02_001619E4, D_L02_001619E8, life, 0x64,
                             random_int_between(0x78, 0x96), 0);
    }
        /* fallthrough */
    case SAND_SHARK_BALLISTIC_FLIGHT: {
        float sink;
        moby->rot.y = sub_rotations_e50(moby->rot.y, D_0015ED6C * 12.566371f);
        fast_vec_add(&moby->pos, &moby->pos, &vars->vel);
        sink = D_L02_0016191C;
        vars->vel.f[2] -= frame_time_sq * 10.8f;
        if (moby->pos.z < vars->home.f[2] - sink) {
            moby->collision = moby->pclass->collision;
            moby->flags |= 0x1000;
            moby->pos.z = vars->home.f[2] - sink;
            moby->state = 0x17;
            moby->rot.y = 0.0f;
            if (moby->prev_seq != 0xC) {
                blend_e50(moby, 0xC, 0, scale_ticks_e50(1));
            }
        } else if (moby->pos.z < vars->home.f[2] + 0.6f) {
            position_or_offset_scratch.q = *(OvlQuad *)&moby->pos;
            FUN_L02_002d57f8_e50(&position_or_offset_scratch, 6);
        }
        break;
    }
    case SAND_SHARK_DEATH_KNOCKBACK: {
        int knockback_status_flags = FUN_L00_0025c698_e50(moby, vars->unk120);
        if ((knockback_status_flags & 1) && vars->unk29C == 0) {
            vars->unk29C = 1;
            position_or_offset_scratch.q = *(OvlQuad *)&moby->pos;
            FUN_L02_002d57f8_e50(&position_or_offset_scratch, 0x1B);
        }
        if (knockback_status_flags & 0x140) {
            FUN_L00_00257470_e50(moby, 0, -1);
            vars->unk27C = FUN_L02_002d5658_e50(moby);
            if (vars->unk27C != 0) {
                FUN_L02_002d3dd0_e50(moby);
                moby->state = 0x1A;
                moby->anim_speed = 0.0f;
                qcopy(&moby->pos, &vars->home);
                moby->collision = 0;
                moby->flags |= 0x41;
                moby->pos.z -= 10.0f;
            } else {
                FUN_L02_002d3dd0_e50(moby);
                remove_moby_e50(moby);
                return;
            }
        } else if (moby->pos.z < 0.0f) {
            FUN_L02_002d3dd0_e50(moby);
            remove_moby_e50(moby);
            return;
        }
        break;
    }
    case SAND_SHARK_FADE_OUT:
        if (moby->alpha >= 8) {
            moby->alpha -= 8;
        } else {
            vars->unk27C = FUN_L02_002d5658_e50(moby);
            if (vars->unk27C != 0) {
                FUN_L02_002d3dd0_e50(moby);
                moby->alpha = 0x80;
                moby->state = 0x1A;
                qcopy(&moby->pos, &vars->home);
                moby->collision = 0;
                moby->pos.z -= 10.0f;
            } else {
                FUN_L02_002d3dd0_e50(moby);
                remove_moby_e50(moby);
                return;
            }
        }
        break;
    case SAND_SHARK_INACTIVE_WAIT_FOR_RELAUNCH:
        vars->unk27C = FUN_L02_002d5658_e50(moby);
        if (vars->unk27C == 0) {
            remove_moby_e50(moby);
            return;
        }
        break;
    }
    *(volatile unsigned int *)0x10000000;
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D6A90), where it is exact; names translated to the US level program. */

extern int D_L02_0015FFD8_m __asm__("D_L02_0015FFD8");
extern int D_L02_001ABE40[];
extern int random_integer_below(int) __asm__("FUN_00213260");

int FUN_L02_002d5658(void *argp) {
    char *moby = argp;
    int result = 0;
    short *group_ids;
    if (((unsigned char *)moby)[0x21] == 0xFF) {
        return 0;
    }
    group_ids = (short *)D_L02_001ABE40[((unsigned char *)moby)[0x21]];
    do {
        int group_moby_byte_offset = (*(unsigned short *)group_ids & 0x7FFF) << 8;
        char *group_moby = (char *)(group_moby_byte_offset + D_L02_0015FFD8_m);
        if (*(short *)(group_moby + 0xA6) == 0x29C && ((unsigned char *)group_moby)[0x20] < 0x7F) {
            if (result == 0 || (random_integer_below(0xFF) & 1)) {
                result = D_L02_0015FFD8_m + group_moby_byte_offset;
            }
        }
    } while (*group_ids++ >= 0);
    return result;
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D6B60), where it is exact; names translated to the US level program. */

extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int D_L02_0015FFD8; /* no foreign declaration */
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void extend_group_sand_shark_timers(char *moby) __asm__("FUN_L02_002d5728");

void extend_group_sand_shark_timers(char *moby) {
    short *group_ids;
    if (((unsigned char *)moby)[0x21] != 0xFF) {
        group_ids = (short *)D_L02_001ABE40[((unsigned char *)moby)[0x21]];
        do {
            char *group_moby = (char *)D_L02_0015FFD8 + ((*(unsigned short *)group_ids & 0x7FFF) << 8);
            if (*(short *)(group_moby + 0xA6) == 0x244) {
                char *group_moby_vars = *(char **)(group_moby + 0x78);
                float proposed_timer_duration = multiply_global_factor_ed64(random_float_between_alt(240.0f, 300.0f));
                if ((float)*(short *)(group_moby_vars + 0x25E) < proposed_timer_duration) {
                    *(short *)(group_moby_vars + 0x25E) = func_001FA898_r(proposed_timer_duration);
                }
            }
        } while (*group_ids++ >= 0);
    }
}

#include "sda.h"
#include "rnc/overlay/quad.h"


extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float fast_cos_d57f8(float) __asm__("FUN_001f9dc8");
extern float fast_sin_d57f8(float) __asm__("FUN_001f9de0");
extern float rand_angle_d57f8(void) __asm__("FUN_00213308");
extern float rand_between_d57f8(float, float) __asm__("FUN_002132a8");
extern void clear_vec_d57f8(void *) __asm__("FUN_001f99f8");
extern int trunc_d57f8(float) __asm__("FUN_001fa6d0");
extern float D_0015ED6C;
extern int D_L02_00160178;
extern float D_L02_00161920 __attribute__((sda));
extern float D_L02_00161924 __attribute__((sda));
extern float D_L02_00161928 __attribute__((sda));
extern float D_L02_0016192C __attribute__((sda));
extern float D_L02_00161930 __attribute__((sda));
extern float D_L02_00161934 __attribute__((sda));
extern float D_L02_00161938 __attribute__((sda));
extern float D_L02_0016193C __attribute__((sda));
extern int D_L02_00161940 __attribute__((sda));
extern int D_L02_00161944 __attribute__((sda));
extern int D_L02_00161948 __attribute__((sda));
extern int D_L02_0016194C __attribute__((sda));
extern int D_L02_00161950 __attribute__((sda));
extern int D_L02_00161954 __attribute__((sda));
int D_L02_00161958 = 0x60306058;
int D_L02_0016195C = 0x20306058;
int D_L02_00161960 = -1;

/* Same routine as FUN_L12_002e19a8: spawns n particles around pos, fewer when D_L02_00160178 is high. */
void FUN_L02_002d57f8(void *particle_origin, int particle_count) {
    float saved_source_position[4];
    float particle_position[4];
    float vertical_motion[4];
    float radial_motion[4];
    float radial_angle;
    float *source_position;
    int i;
    int frame_count, second_frame_count, third_frame_count;
    source_position = saved_source_position;
    *(OvlQuad *)source_position = *(OvlQuad *)particle_origin;
    if (D_L02_00160178 > 0x5DC) {
        particle_count = particle_count / 4;
    } else if (D_L02_00160178 > 0x400) {
        particle_count = particle_count / 2;
    }
    for (i = 0; i < particle_count; i++) {
        radial_angle = rand_angle_d57f8();
        qcopy(particle_position, source_position);
        particle_position[2] = particle_position[2] + rand_between_d57f8(-0.25f, -0.75f);
        particle_position[0] = particle_position[0] + rand_between_d57f8(-0.25f, 0.25f);
        particle_position[1] = particle_position[1] + rand_between_d57f8(-0.25f, 0.25f);
        clear_vec_d57f8(vertical_motion);
        radial_motion[0] = fast_cos_d57f8(radial_angle) * (rand_between_d57f8(D_L02_00161930, D_L02_00161934) * D_0015ED6C);
        radial_motion[1] = fast_sin_d57f8(radial_angle) * (rand_between_d57f8(D_L02_00161930, D_L02_00161934) * D_0015ED6C);
        *(int *)&radial_motion[2] = 0;
        vertical_motion[2] = rand_between_d57f8(D_L02_00161920, D_L02_00161924) * D_0015ED6C;
        radial_motion[2] = rand_between_d57f8(D_L02_00161928, D_L02_0016192C) * D_0015ED6C;
        vertical_motion[3] = D_L02_00161938;
        radial_motion[3] = D_L02_0016193C;
        frame_count = trunc_d57f8(rand_between_d57f8((float)D_L02_00161940, (float)D_L02_00161944));
        second_frame_count = trunc_d57f8(rand_between_d57f8((float)D_L02_00161948, (float)D_L02_0016194C));
        third_frame_count = trunc_d57f8(rand_between_d57f8((float)D_L02_00161950, (float)D_L02_00161954));
        spawn_spark(particle_position, vertical_motion, radial_motion, D_L02_00161958, D_L02_0016195C, frame_count, second_frame_count, third_frame_count, D_L02_00161960);
    }
}

/* Spawns one particle above pos unless D_L02_00160178 is already high. */
extern float D_L02_00161964 __attribute__((sda));
extern float D_L02_00161968 __attribute__((sda));
extern float D_L02_0016196C __attribute__((sda));
extern float D_L02_00161970 __attribute__((sda));
extern float D_L02_00161974 __attribute__((sda));
extern float D_L02_00161978 __attribute__((sda));
extern float D_L02_0016197C __attribute__((sda));
extern float D_L02_00161980 __attribute__((sda));
extern s32 D_L02_00161984 __attribute__((sda));
extern s32 D_L02_00161988 __attribute__((sda));
extern s32 D_L02_0016198C __attribute__((sda));
extern s32 D_L02_00161990 __attribute__((sda));
extern s32 D_L02_00161994 __attribute__((sda));
extern s32 D_L02_00161998 __attribute__((sda));
s32 D_L02_0016199C = 0x40306058;
s32 D_L02_001619A0 = 0x10306058;
s32 D_L02_001619A4 = -1;

void FUN_L02_002d5a68(Vec4 *from) {
    Vec4 saved_source_position;
    Vec4 at;
    Vec4 vertical_motion;
    Vec4 radial_motion;
    Vec4 *source_position = &saved_source_position;
    Vec4 *particle_position;
    float radial_angle;
    int frame_count, second_frame_count, third_frame_count;

    saved_source_position.q = from->q;
    if (D_L02_00160178 >= 0x6A5)
        return;
    radial_angle = rand_angle_d57f8();
    particle_position = &at;
    qcopy(particle_position, source_position);
    at.f[2] = at.f[2] + rand_between_d57f8(0.125f, 0.0f);
    at.f[0] = at.f[0] + rand_between_d57f8(-0.125f, 0.125f);
    at.f[1] = at.f[1] + rand_between_d57f8(-0.125f, 0.125f);
    clear_vec_d57f8(&vertical_motion);
    radial_motion.f[0] = fast_cos_d57f8(radial_angle) * (rand_between_d57f8(D_L02_00161974, D_L02_00161978) * D_0015ED6C);
    radial_motion.f[1] = fast_sin_d57f8(radial_angle) * (rand_between_d57f8(D_L02_00161974, D_L02_00161978) * D_0015ED6C);
    radial_motion.f[2] = 0.0f;
    vertical_motion.f[2] = rand_between_d57f8(D_L02_00161964, D_L02_00161968) * D_0015ED6C;
    radial_motion.f[2] = rand_between_d57f8(D_L02_0016196C, D_L02_00161970) * D_0015ED6C;
    vertical_motion.f[3] = D_L02_0016197C;
    radial_motion.f[3] = D_L02_00161980;
    frame_count = trunc_d57f8(rand_between_d57f8((float)D_L02_00161984, (float)D_L02_00161988));
    second_frame_count = trunc_d57f8(rand_between_d57f8((float)D_L02_0016198C, (float)D_L02_00161990));
    third_frame_count = trunc_d57f8(rand_between_d57f8((float)D_L02_00161994, (float)D_L02_00161998));
    spawn_spark(&at, &vertical_motion, &radial_motion, D_L02_0016199C, D_L02_001619A0, frame_count, second_frame_count, third_frame_count, D_L02_001619A4);
}

/* pvars of the class FUN_L02_002d5c88 updates (only the fields it touches). */
typedef struct {
    char pad0[0x20];
    float hp;             /* 0x20 */
    char pad24[0xA];
    unsigned char b2E;    /* 0x2E: 2 = respawn requested */
    char pad2F[9];
    int i38;              /* 0x38 */
    char pad3C[0xD4];
    short anim[3];        /* 0x110: passed to trooper_anim_update / trooper_anim_finish */
    char pad116;
    unsigned char b117;   /* 0x117 */
    char pad118[8];
    char knock[0x10];     /* 0x120: passed to trooper_start_knockback */
    float f130;           /* 0x130 */
    float f134;
    float f138;
    float f13C;
    int i140;             /* 0x140 */
    int i144;
    float f148;
    int i14C;             /* 0x14C: count kept by the spawner moby */
    char pad150[0xD];
    unsigned char b15D;   /* 0x15D */
    char pad15E[0xE];
    float f16C;           /* 0x16C */
    float f170;
    float f174;
    char pad178[0x58];
    float tgt[4];         /* 0x1D0: point followed along the path */
    char pad1E0[0x30];
    int i210;             /* 0x210 */
    int i214;
    char pad218[8];
    float home[4];        /* 0x220 */
    char pad230[0x1C];
    float f24C;           /* 0x24C */
    float f250;
    char pad254[8];
    short t25C;           /* 0x25C */
    short t25E;
    char pad260[0x1C];
    int i27C;             /* 0x27C */
    int i280;
    char pad284[4];
    int i288;             /* 0x288: spawner moby index, -1 if none */
    char pad28C[4];
    int i290;             /* 0x290: path index */
    char pad294[8];
    int i29C;             /* 0x29C */
} Vars5c88;

extern float D_L02_00161908 __attribute__((sda));
extern float D_L02_0016190C __attribute__((sda));
extern float D_L02_00161914 __attribute__((sda));
extern float D_L02_00161918 __attribute__((sda));
extern int *D_L02_001B0AB0_5c88[] __asm__("D_L02_001B0AB0");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float absolute_float_5c88(float) __asm__("func_001F99C0");
extern float FUN_00213508(void *, int, float);
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern int FUN_L00_0025ff38(float, void *, void *, int, int, void *, int);
extern int FUN_L02_002d5658(void *argp);
extern void extend_group_sand_shark_timers(char *arg) __asm__("FUN_L02_002d5728");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern int fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

/* Path-following enemy: respawns at its home point on request, takes hits (dies and drops to the
   ground), then follows its path and retargets the hero when it strays. */
void FUN_L02_002d5c88(unsigned char *moby) {
    Vars5c88 *moby_vars = *(Vars5c88 **)(moby + 0x78);
    char *hit_record;
    int *path;
    int hit;
    float dmg;
    float k;
    float ground_height;
    OvlVec4 ground_probe_position;
    OvlVec4 hit_knockback_vector;

    moby_vars->i280 = moby[0x20];
    if (moby[0x20] == 0) {
        return;
    }
    if (moby_vars->b2E == 2) {
        moby_vars->b2E = 1;
        if (moby_vars->i288 >= 0) {
            (*(Vars5c88 **)((moby_vars->i288 << 8) + D_L02_0015FFD8 + 0x78))->i14C -= 1;
        }
        if ((moby_vars->i27C = FUN_L02_002d5658(moby)) == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby[0x20] = 0x1A;
        qcopy(moby + 0x10, moby_vars->home);
        *(int *)(moby + 0x94) = 0;
        *(float *)(moby + 0x18) -= 10.0f;
    }
    if (moby_vars->i38 != 0 ||
        (!fast_dec_timer_s16(&moby_vars->t25C) && vector_distance(moby + 0x10, &hero.motion.pos) < 10.0f)) {
        moby_vars->t25E = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(180.0f, 240.0f)));
    }
    moby_vars->i38 = 0;
    if (fast_dec_timer_s16(&moby_vars->t25E)) {
        moby_vars->f250 = moby_vars->f24C;
    } else {
        moby_vars->f250 = moby_vars->f24C + 5.0f;
    }
    dmg = 0.0f;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    fun_00213928(moby, hit_record, &moby_vars->hp, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && moby[0x20] != 0x18) {
        extend_group_sand_shark_timers((char *)moby);
        moby_vars->i29C = 0;
        moby_vars->hp -= dmg;
        if (moby_vars->hp <= 0.0f) {
            moby_vars->hp -= dmg;
            moby_vars->i140 = 0x200;
            moby_vars->f130 = D_L02_00161914 * frame_time_sq;
            moby_vars->f134 = D_L02_00161918 * frame_time_sq;
            moby_vars->f138 = D_L02_0016190C * D_0015ED6C;
            moby_vars->f13C = D_L02_00161908 * D_0015ED6C;
            moby_vars->b15D = 0;
            moby_vars->i144 = 9;
            moby_vars->f16C = 2.0f * D_0015ED6C;
            moby_vars->f148 = 0.5f;
            qcopy(&ground_probe_position, moby + 0x10);
            ground_probe_position.f[2] += 2.0f;
            ground_height = FUN_00213508(&ground_probe_position, 0, 0.5f);
            if (*(float *)(moby + 0x18) < ground_height) {
                *(float *)(moby + 0x18) = ground_height;
            }
            if (moby_vars->i288 >= 0) {
                extern int D_L02_0015FFD8_s __asm__("D_L02_0015FFD8") __attribute__((sda));
                (*(Vars5c88 **)((moby_vars->i288 << 8) + D_L02_0015FFD8_s + 0x78))->i14C -= 1;
                moby_vars->b15D = 0;
            } else {
                ground_probe_position.f[2] += 0.01f;
            }
            moby_vars->i144 |= 0x20;
            *(unsigned short *)(moby + 0x34) &= 0xEFFF;
            hit_knockback_vector.q = *(OvlQuad *)(hit_record + 0x10);
            trooper_knockback_arc(&hit_knockback_vector, &k, &moby_vars->f138, &moby_vars->f13C);
            trooper_start_knockback(k, moby, moby_vars->knock, 6, 1, 0);
            moby_vars->f148 = 0.85f;
            moby_vars->f170 = 7.0f;
            moby_vars->f174 = 14.0f;
            moby[0x20] = 0x18;
            *(int *)(moby + 0x94) = 0;
            moby_vars->b117 = 0x78;
            trooper_anim_update(moby, moby_vars->anim);
        }
    }
    moby[0xA4] = 0xFF;
    trooper_anim_finish(moby, moby_vars->anim);
    path = D_L02_001B0AB0_5c88[moby_vars->i290];
    if (FUN_L00_0025ff38(moby_vars->f250, moby, moby_vars->tgt, 0, 0, path + 4, *path) != 2 &&
        (moby_vars->f250 < vector_distance(moby_vars->home, moby_vars->tgt) ||
         absolute_float_5c88(*(float *)(moby + 0x18) - moby_vars->tgt[2]) > 3.0f)) {
        moby_vars->i214 = 2;
    }
    if (moby_vars->i210 == 0) {
        moby_vars->i210 = (int)hero.moby;
        qcopy(moby_vars->tgt, &hero.motion.pos);
    }
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D7550), where it is exact; names translated to the US level program. */

extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void FUN_L00_00250df8(void *);
extern void func_00213DE0(void *, int, int, int) __asm__("FUN_00212f90");

void launch_sand_shark(struct Moby *moby, void *position, float *direction) __asm__("FUN_L02_002d6118");

void launch_sand_shark(struct Moby *moby, void *position, float *direction) {
    char *data = (char *)moby->pvars;
    moby->state = 0x1B;
    moby->flags &= 0xFFBE;
    if (moby->prev_seq != 0xB) {
        func_00213DE0(moby, 0xB, 0, 1);
    }
    qcopy(&moby->pos, position);
    qcopy(data + 0x40, direction);
    moby->rot.z = fast_arc_tan(direction[0], direction[1]);
    FUN_L00_00250df8(moby);
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D7648), where it is exact; names translated to the US level program. */

extern char D_L02_0015FFD8_c __asm__("D_L02_0015FFD8");
extern void *store_moby_in_suck_cannon(void *) __asm__("FUN_L00_002dbb20");

void *handle_sand_shark_suck_cannon_storage(struct Moby *moby) __asm__("FUN_L02_002d6210");

void *handle_sand_shark_suck_cannon_storage(struct Moby *moby) {
    void *helper_result = store_moby_in_suck_cannon(moby);
    if (helper_result) {
        int spawner_moby_index;
        moby->state = 9;
        spawner_moby_index = *(int *)((char *)moby->pvars + 0x288);
        if (spawner_moby_index >= 0) {
            char *spawner_vars = *(char **)(*(int *)&D_L02_0015FFD8_c + spawner_moby_index * 256 + 0x78);
            (*(int *)(spawner_vars + 0x14C))--;
        }
    } else if (moby->state == 9) {
        moby->state = 1;
    }
    return helper_result;
}
#include "qcopy.h"
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D7778), where it is exact; names translated to the US level program. */

extern void FUN_L02_002d3dd0(void *);

void prepare_sand_shark_for_relaunch_or_remove(struct Moby *moby) __asm__("FUN_L02_002d6340");

void prepare_sand_shark_for_relaunch_or_remove(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int result = FUN_L02_002d5658(moby);
    *(int *)(data + 0x27C) = result;
    if (result != 0) {
        moby->state = 0x1A;
        moby->anim_speed = 0;
        qcopy(&moby->pos, data + 0x220);
        moby->collision = 0;
        moby->pos.z -= 10.0f;
    } else {
        FUN_L02_002d3dd0(moby);
        mark_moby_for_removal(moby);
    }
}
extern int scale_ticks(int) __asm__("FUN_001f96f8");

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D77F8), where it is exact; names translated to the US level program. */

extern char *D_L02_001B0AB0[];
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
void blend_moby_animation(MobyAnim *moby, int new_seq, int frame_index, int blend_frames) __asm__("FUN_00212f90");

void FUN_L02_002d63c0(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *entry = D_L02_001B0AB0[*(int *)(data + 0x290)];
    if (point_in_path_xy(&moby->pos, entry + 0x10, *(int *)entry) == 0) {
        moby->state = 0x12;
        if (moby->prev_seq) {
            blend_moby_animation(moby, 0, 0, scale_ticks(20));
        }
        *(int *)(data + 0x298) = 1;
    }
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D8B80), where it is exact; names translated to the US level program. */

typedef struct {
    /* 0x000 */ char pad000[0x20];
    /* 0x020 */ float f20;
    /* 0x024 */ char pad024[4];
    /* 0x028 */ char b28;
    /* 0x029 */ char b29;
    /* 0x02A */ char pad02A[6];
    /* 0x030 */ float f30;
    /* 0x034 */ char pad034[0xC];
    /* 0x040 */ float velocity[4];
    /* 0x050 */ char pad050[8];
    /* 0x058 */ char b58;
    /* 0x059 */ char pad059;
    /* 0x05A */ char b5A;
    /* 0x05B */ char pad05B;
    /* 0x05C */ char b5C;
    /* 0x05D */ char pad05D[0x13];
    /* 0x070 */ char knockback[0x60];
    /* 0x0D0 */ char walk[8];
    /* 0x0D8 */ float walk_d8;
    /* 0x0DC */ float walk_dc;
    /* 0x0E0 */ char pad0E0[0x14];
    /* 0x0F4 */ float walk_step;
    /* 0x0F8 */ char pad0F8[0x14];
    /* 0x10C */ float walk_10c;
    /* 0x110 */ float walk_110;
    /* 0x114 */ float walk_114;
    /* 0x118 */ char pad118[0x68];
    /* 0x180 */ float attack_range;
    /* 0x184 */ float wake_range;
    /* 0x188 */ int post;
    /* 0x18C */ int wake_volume;
    /* 0x190 */ char pad190[4];
    /* 0x194 */ int start;
    /* 0x198 */ char pad198[8];
    /* 0x1A0 */ unsigned char blink[0x40];
    /* 0x1E0 */ float wander[4];
    /* 0x1F0 */ char *shots[9];
    /* 0x214 */ float turn_speed;
    /* 0x218 */ char pad218[8];
    /* 0x220 */ char *target;
} L02BotData;

extern char *D_L02_001600EC;
extern char D_L02_001673C0[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C;
extern float D_L02_0015F580[] __attribute__((section(".sdata")));
extern int D_L02_0015F5CC_i __asm__("D_L02_0015F5CC");
extern float D_L02_00161A80 __attribute__((sda));
extern float D_L02_00161A84 __attribute__((sda));
int D_L02_00161A88 = 0x40808080;
int D_L02_00161A90 = 0x3c;
int D_L02_00161A94 = 0x78;
extern float D_L02_00161A98 __attribute__((sda));
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float approach_value_alt(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L00_00259028(char *m, char *p, float *pos, char *v);
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
extern s32 rand(void);
extern s32 random_integer_below_c(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void *FUN_L00_00263fd8(char *src, int cls, float *pos, void *mat, int a8, int a9, float scale, float *v10, float *v11, float *v12);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258278(char *moby, float target, float *vel, float k, float d, float max);
extern void make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");
extern void FUN_L00_0025a120(void *);
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_0025f730(void *a, float x);
extern void FUN_L00_00263ac8(float x, int a, int b, unsigned char *m);
extern void FUN_L00_0026cbb0(void *, void *, int, int, int, int, float);
extern void FUN_L01_0026d930(char *p);
extern void reset_shot_tracking(char *moby) __asm__("FUN_L02_00264e40");
extern void FUN_L02_00264e70(void *, void *, void *, float);
extern void collect_mobies_around_tracked_objects(char **slots, int a, int b) __asm__("FUN_L02_002651d0");
void handle_level02_bot_damage_and_retargeting(void *owner_moby) __asm__("FUN_L02_002d85b8");

extern void handle_level02_bot_damage_and_retargeting(void *) __asm__("FUN_L02_002d85b8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation_c3(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float_c(float input) __asm__("func_001F99C0");

void update_level02_bot(struct Moby *moby) __asm__("FUN_L02_002d7748");
void update_level02_bot(struct Moby *moby) {
    enum {
        L02_BOT_INITIALIZE = 0,
        L02_BOT_WAIT_FOR_WAKE_ANIMATION = 2,
        L02_BOT_ALIGN_TO_GROUND_AT_POST = 3,
        L02_BOT_WAIT_FOR_ANIMATION_BEFORE_MOVE_TO_POST = 4,
        L02_BOT_MOVE_TO_POST = 5,
        L02_BOT_WANDER = 6,
        L02_BOT_SELECT_ATTACK_OR_WANDER = 7,
        L02_BOT_WAIT_FOR_WAKE_TRIGGER_BEFORE_GROUND_ALIGNMENT = 8,
        L02_BOT_WAIT_FOR_WAKE_TRIGGER_WITH_ALTERNATE_WAKE_ANIMATION = 9,
        L02_BOT_WAIT_FOR_WAKE_TRIGGER = 10,
        L02_BOT_WAIT_FOR_ANIMATION_BEFORE_ATTACK = 11,
        L02_BOT_UPDATE_ATTACK_SHOTS = 12,
        L02_BOT_WAIT_FOR_ANIMATION_BEFORE_ATTACK_RECHECK = 13,
        L02_BOT_HIT_KNOCKBACK_RECOVERY = 16,
        L02_BOT_DEATH_KNOCKBACK = 17
    };
    enum {
        L02_BOT_START_SELECT_ATTACK_OR_WANDER = 0,
        L02_BOT_START_WAIT_FOR_WAKE_TRIGGER = 1,
        L02_BOT_START_WAIT_FOR_WAKE_TRIGGER_BEFORE_GROUND_ALIGNMENT = 2,
        L02_BOT_START_WAIT_FOR_WAKE_TRIGGER_WITH_ALTERNATE_WAKE_ANIMATION = 3
    };
    float position[4];
    float direction[4];
    char shot[0x30];
    L02BotData *data = (L02BotData *)moby->pvars;
    char *target;
    float angle;
    float ground;
    float wander_heading;
    float spray_heading;
    float spray_radius;
    float step;
    int triggered;

    FUN_L00_00263ac8(2.5f, (int)moby, 1, data->blink);
    if (moby->visible != 0 && distance_xyz(&moby->pos, D_L02_001673C0) < 30.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x18;
    }
    handle_level02_bot_damage_and_retargeting(moby);
    switch (moby->state) {
    case L02_BOT_INITIALIZE:
        if (rand() & 1) {
            moby->flags |= 0x8000;
        }
        moby->flags |= 0x1000;
        data->b5C = 1;
        data->f30 = 1.0f;
        data->b58 = 0xF;
        data->b5A = 8;
        data->b29 = 0;
        moby->spawn_frame = *(long *)(((char *)hero.moby) + 0x38);
        FUN_L01_0026d930(data->walk);
        step = D_0015ED6C * 4.0f;
        data->b28 = 2;
        data->walk_10c = 0.03f;
        data->walk_110 = 0.3f;
        data->walk_114 = 0.25f;
        data->walk_step = step;
        data->walk_dc = 2.0f;
        data->walk_d8 = 2.0f;
        data->f20 = 3.0f;
        clear_u64_value(data->velocity);
        reset_shot_tracking((char *)data->shots);
        switch (data->start) {
        case L02_BOT_START_SELECT_ATTACK_OR_WANDER:
        default:
            if (moby->prev_seq != 0) {
                blend_moby_animation_c3(moby, 0, 0, 0);
            }
            moby->state = 7;
            break;
        case L02_BOT_START_WAIT_FOR_WAKE_TRIGGER:
            if (moby->prev_seq != 0) {
                blend_moby_animation_c3(moby, 0, 0, 0);
            }
            moby->state = 10;
            break;
        case L02_BOT_START_WAIT_FOR_WAKE_TRIGGER_BEFORE_GROUND_ALIGNMENT:
            if (moby->prev_seq != 2) {
                blend_moby_animation_c3(moby, 2, 0, 0);
            }
            moby->state = 8;
            break;
        case L02_BOT_START_WAIT_FOR_WAKE_TRIGGER_WITH_ALTERNATE_WAKE_ANIMATION:
            if (moby->prev_seq != 15) {
                blend_moby_animation_c3(moby, 15, 0, 0);
            }
            moby->state = 9;
            break;
        }
        break;
    case L02_BOT_WAIT_FOR_WAKE_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = 3;
            if (moby->prev_seq != 4) {
                blend_moby_animation_c3(moby, 4, 0, 0);
            }
        }
        break;
    case L02_BOT_ALIGN_TO_GROUND_AT_POST:
        target = data->target;
        angle = fast_arc_tan(*(float *)(target + 0x10) - moby->pos.x,
                                  *(float *)(target + 0x14) - moby->pos.y);
        FUN_L00_00258278((char *)moby, angle, &data->turn_speed, 0.02f, 0.3f, D_0015ED6C * 1.5707964f);
        subtract_vector_xyz_c(position, D_L02_001600EC + data->post * 0x80 + 0x30, &moby->pos);
        position[2] = 0.0f;
        FUN_L00_0025f730(position, D_0015ED6C * 6.0f);
        add_vector_xyz(&moby->pos, &moby->pos, position);
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        approach_value_alt(&data->velocity[2], D_0015ED6C * 40.0f, D_L02_00161A84 * frame_time_sq);
        approach_value_alt(&moby->pos.z, ground, data->velocity[2]);
        if (moby->pos.z == ground) {
            moby->state = 4;
            if (moby->prev_seq != 5) {
                blend_moby_animation_c3(moby, 5, 4, scale_game_frames(10));
            }
        }
        break;
    case L02_BOT_WAIT_FOR_ANIMATION_BEFORE_MOVE_TO_POST:
        if (moby->anim_flags & 2) {
            moby->state = 5;
            if (moby->prev_seq != 6) {
                blend_moby_animation_c3(moby, 6, 0, scale_game_frames(6));
            }
        }
        break;
    case L02_BOT_MOVE_TO_POST:
        if (FUN_L00_00259028((char *)moby, data->walk, (float *)(D_L02_001600EC + data->post * 0x80 + 0x30), (char *)data->velocity) == 4) {
            if (moby->prev_seq != 0) {
                blend_moby_animation_c3(moby, 0, 0, scale_game_frames(30));
            }
            moby->state = 7;
        }
        break;
    case L02_BOT_WANDER:
        if (FUN_L00_00259028((char *)moby, data->walk, data->wander, (char *)data->velocity) & 6) {
            if (moby->prev_seq != 0) {
                blend_moby_animation_c3(moby, 0, 0, scale_game_frames(10));
            }
            moby->state = 7;
        } else if (distance_xyz(data->target + 0x10, D_L02_001600EC + data->post * 0x80 + 0x30) < data->attack_range
                   && absolute_float_c(*(float *)(data->target + 0x18) - moby->pos.z) < 4.0f) {
            moby->state = 5;
            if (moby->prev_seq != 6) {
                blend_moby_animation_c3(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case L02_BOT_SELECT_ATTACK_OR_WANDER:
        if (distance_xyz(data->target + 0x10, D_L02_001600EC + data->post * 0x80 + 0x30) < data->attack_range
            && absolute_float_c(*(float *)(data->target + 0x18) - moby->pos.z) < 4.0f) {
            if (distance_xyz(&moby->pos, D_L02_001600EC + data->post * 0x80 + 0x30) < 1.0f) {
                target = data->target;
                angle = fast_arc_tan(*(float *)(target + 0x10) - moby->pos.x,
                                          *(float *)(target + 0x14) - moby->pos.y);
                FUN_L00_00258278((char *)moby, angle, &data->turn_speed, 0.02f, 0.3f, 0.1f);
                if (fast_difference_between_rotations(moby->rot.z, angle) < 0.08726646f) {
                    moby->state = 0xB;
                    if (moby->prev_seq != 8) {
                        blend_moby_animation_c3(moby, 8, 0, scale_game_frames(6));
                    }
                }
            } else {
                moby->state = 5;
                if (moby->prev_seq != 6) {
                    blend_moby_animation_c3(moby, 6, 0, scale_game_frames(10));
                }
            }
        } else if (moby->anim_flags & 2) {
            wander_heading = random_angle_radians();
            data->wander[0] = fast_cos_c(wander_heading) * random_float_between(1.0f, 2.0f);
            data->wander[1] = fast_sin_c(wander_heading) * random_float_between(1.0f, 2.0f);
            data->wander[2] = 0.0f;
            add_vector_xyz(data->wander, data->wander, D_L02_001600EC + data->post * 0x80 + 0x30);
            moby->state = 6;
            if (moby->prev_seq != 6) {
                blend_moby_animation_c3(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case L02_BOT_WAIT_FOR_WAKE_TRIGGER_BEFORE_GROUND_ALIGNMENT:
    case L02_BOT_WAIT_FOR_WAKE_TRIGGER_WITH_ALTERNATE_WAKE_ANIMATION:
        if (!(D_L02_0015F5CC_i & 3) && moby->visible != 0) {
            spray_radius = random_float_between(0.0f, D_L02_00161A98) * D_0015ED6C;
            spray_heading = random_angle_radians();
            direction[0] = fast_cos_c(spray_heading) * spray_radius;
            direction[1] = fast_sin_c(spray_heading) * spray_radius;
            direction[2] = 0.0f;
            moby_point_position(moby, 0, position);
            FUN_L00_0026cbb0(position, direction, D_L02_00161A88, D_L02_00161A88,
                              random_int_between(scale_game_frames(D_L02_00161A90), scale_game_frames(D_L02_00161A94)), 1, 40000.0f);
        }
    case L02_BOT_WAIT_FOR_WAKE_TRIGGER:
        triggered = 0;
        if (data->wake_volume >= 0) {
            triggered = is_point_inside_clip_volume(&hero.ground_point, data->wake_volume) != 0;
        } else if (distance_xyz(&hero.motion.pos, D_L02_001600EC + data->post * 0x80 + 0x30) < data->wake_range) {
            triggered = 1;
        }
        if (triggered) {
            if (moby->state == 8) {
                if (moby->prev_seq != 4) {
                    blend_moby_animation_c3(moby, 4, 0, scale_game_frames(10));
                }
                moby->state = 2;
            } else if (moby->state == 9) {
                if (moby->prev_seq != 0x10) {
                    blend_moby_animation_c3(moby, 0x10, 0, scale_game_frames(10));
                }
                moby->state = 2;
            } else {
                moby->state = 5;
                if (moby->prev_seq != 6) {
                    blend_moby_animation_c3(moby, 6, 0, scale_game_frames(6));
                }
            }
        }
        break;
    case L02_BOT_WAIT_FOR_ANIMATION_BEFORE_ATTACK:
        moby->anim_speed = 2.0f;
        target = data->target;
        angle = fast_arc_tan(*(float *)(target + 0x10) - moby->pos.x,
                                  *(float *)(target + 0x14) - moby->pos.y);
        FUN_L00_00258278((char *)moby, angle, &data->turn_speed, 0.02f, 0.3f, 0.1f);
        if (moby->anim_flags & 2) {
            moby->anim_speed = 1.0f;
            moby->state = 0xC;
            if (moby->prev_seq != 9) {
                blend_moby_animation_c3(moby, 9, 0, scale_game_frames(6));
            }
        }
        break;
    case L02_BOT_UPDATE_ATTACK_SHOTS:
        moby->anim_speed = 0.5f;
        moby_point_position(moby, 0, position);
        direction[0] = fast_cos_c(fast_arc_tan(position[0] - moby->pos.x, position[1] - moby->pos.y));
        direction[1] = fast_sin_c(fast_arc_tan(position[0] - moby->pos.x, position[1] - moby->pos.y));
        direction[2] = -0.1f;
        FUN_L02_00264e70(data->shots, position, direction, D_L02_00161A80);
        direction[3] = 5627.925f;
        direction[2] = 1.0f;
        make_hit_descriptor(shot, (int)moby, 0x10001, direction, 1.0f);
        shot[0x18] = 5;
        shot[0x19] = 1;
        *(short *)(shot + 0x1A) = (u16)moby->oclass;
        collect_mobies_around_tracked_objects(data->shots, (int)moby, (int)shot);
        if (moby->anim_flags & 2) {
            moby->anim_speed = 1.0f;
            moby->state = 0xD;
            if (random_integer_below_c(0xFF) & 1) {
                if (moby->prev_seq != 10) {
                    blend_moby_animation_c3(moby, 10, 0, scale_game_frames(6));
                }
            } else if (moby->prev_seq != 8) {
                blend_moby_animation_c3(moby, 8, 0, scale_game_frames(6));
            }
        }
        break;
    case L02_BOT_WAIT_FOR_ANIMATION_BEFORE_ATTACK_RECHECK:
        target = data->target;
        angle = fast_arc_tan(*(float *)(target + 0x10) - moby->pos.x,
                                  *(float *)(target + 0x14) - moby->pos.y);
        FUN_L00_00258278((char *)moby, angle, &data->turn_speed, 0.02f, 0.3f, 0.1f);
        if (moby->anim_flags & 2) {
            if (vector_distance(&moby->pos, data->target + 0x10) < data->attack_range + 1.0f
                && absolute_float_c(*(float *)(data->target + 0x18) - moby->pos.z) < 3.0f) {
                moby->state = 0xC;
                if (moby->prev_seq != 9) {
                    blend_moby_animation_c3(moby, 9, 0, scale_game_frames(6));
                }
            } else {
                moby->state = 7;
                if (moby->prev_seq != 0) {
                    blend_moby_animation_c3(moby, 0, 0, scale_game_frames(6));
                }
            }
        }
        break;
    case L02_BOT_HIT_KNOCKBACK_RECOVERY:
        if (mine_fall(moby, data->knockback) & 0x40) {
            moby->state = 7;
            if (moby->prev_seq != 0) {
                blend_moby_animation_c3(moby, 0, 0, scale_game_frames(6));
            }
        }
        break;
    case L02_BOT_DEATH_KNOCKBACK:
        if (mine_fall(moby, data->knockback) & 0x140) {
            qcopy(position, &moby->pos);
            position[2] += 1.0f;
            FUN_L00_00257470(moby, 0, -1);
            spawn_explosion(moby, data->velocity, position, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, -1, 1.0f, 15.0f, 1, 1, -1, 0);
            FUN_L00_00263fd8((char *)moby, 0x6D7, (float *)(&moby->pos), &moby->rot, 0, 0, 0.0f, D_L02_0015F580, D_L02_0015F580, D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby, 0x6D8, (float *)(&moby->pos), &moby->rot, 0, 0, 0.0f, D_L02_0015F580, D_L02_0015F580, D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby, 0x6D9, (float *)(&moby->pos), &moby->rot, 0, 0, 0.0f, D_L02_0015F580, D_L02_0015F580, D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby, 0x6E9, (float *)(&moby->pos), &moby->rot, 0, 0, 0.0f, D_L02_0015F580, D_L02_0015F580, D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby, 0x782, (float *)(&moby->pos), &moby->rot, 0, 0, 0.0f, D_L02_0015F580, D_L02_0015F580, D_L02_0015F580);
            mark_moby_for_removal_c(moby);
            return;
        }
        break;
    }
    FUN_L00_0025a120(moby);
}

extern float D_L02_00161A64 __attribute__((sda));
extern float D_L02_00161A68 __attribute__((sda));
extern float D_L02_00161A6C __attribute__((sda));
extern float D_L02_00161A70 __attribute__((sda));
extern float D_L02_00161A74 __attribute__((sda));
extern float D_L02_00161A78 __attribute__((sda));
extern float D_L02_00161A9C __attribute__((sda));
extern int fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int trunc_d57f8(float) __asm__("FUN_001fa6d0");
extern int FUN_L02_00261c80(char *a, char *o, float best);
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

/* pvars of the class FUN_L02_002d85b8 serves (only the fields it touches). */
typedef struct {
    char pad0[0x20];
    float hp;            /* 0x20 */
    char pad24[0x3C];
    short anim[3];       /* 0x60: passed to trooper_anim_update / trooper_anim_finish */
    unsigned char pad66;
    unsigned char flash; /* 0x67 */
    char pad68[8];
    char knock[0x10];    /* 0x70: passed to trooper_start_knockback */
    float f80;           /* 0x80 */
    float f84;
    float f88;
    float f8C;
    int i90;             /* 0x90 */
    int i94;
    float f98;
    char pad9C[4];
    float fA0;           /* 0xA0 */
    char padA4[9];
    unsigned char bAD;   /* 0xAD */
    char padAE[0xE];
    float fBC;           /* 0xBC */
    float fC0;
    float fC4;
    char padC8[0x158];
    char *target;        /* 0x220 */
} Vars85b8;

/* Hit reaction: takes damage, flinches or dies (knocked away from the hero), then retargets. */
void handle_level02_bot_damage_and_retargeting(void *owner_moby) {
    char *moby = owner_moby;
    Vars85b8 *moby_vars = *(Vars85b8 **)(moby + 0x78);
    char *hit_record;
    char *current_target;
    int target[20];
    int hit;
    float dmg;

    dmg = 0.0f;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    if (hit_record != 0 && *(short *)(*(char **)(hit_record + 0x20) + 0xA6) == 0x264) {
        hit_record = 0;
    }
    fun_00213928(moby, hit_record, &moby_vars->hp, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && ((unsigned char *)moby)[0x20] != 0x11) {
        moby_vars->hp -= dmg;
        moby_vars->i90 = trunc_d57f8(614.4f);
        moby_vars->f98 = 0.6f;
        if (0.0f < moby_vars->hp) {
            if (dmg < 1.0f) {
                moby_vars->flash = 100;
            } else {
                void *k = moby_vars->knock;
                moby_vars->flash = 0x78;
                moby_vars->f80 = D_L02_00161A84 * frame_time_sq;
                moby_vars->f84 = D_L02_00161A78 * frame_time_sq;
                moby_vars->f88 = D_L02_00161A6C * D_0015ED6C;
                moby_vars->f8C = D_L02_00161A70 * D_0015ED6C;
                moby_vars->fBC = D_L02_00161A9C * D_0015ED6C;
                moby_vars->fA0 = 0.5f;
                moby_vars->i94 = 25;
                moby_vars->bAD = 0;
                trooper_start_knockback(fast_arc_tan(*(float *)(moby + 0x10) - hero.motion.pos.f[0],
                                              *(float *)(moby + 0x14) - hero.motion.pos.f[1]),
                                 moby, k, 0xB, 1, 0);
                moby_vars->fC0 = 6.5f;
                moby_vars->fC4 = 11.0f;
                moby[0x20] = 0x10;
            }
        } else {
            void *k2 = moby_vars->knock;
            moby_vars->f80 = D_L02_00161A84 * frame_time_sq;
            moby_vars->f84 = D_L02_00161A74 * frame_time_sq;
            moby_vars->f88 = D_L02_00161A64 * D_0015ED6C;
            moby_vars->f8C = D_L02_00161A68 * D_0015ED6C;
            moby_vars->fBC = D_L02_00161A9C * D_0015ED6C;
            moby_vars->i94 = 9;
            moby_vars->bAD = 0;
            trooper_start_knockback(fast_arc_tan(*(float *)(moby + 0x10) - hero.motion.pos.f[0],
                                          *(float *)(moby + 0x14) - hero.motion.pos.f[1]),
                             moby, k2, 0xE, 1, 0);
            moby_vars->fC0 = 6.5f;
            moby_vars->fC4 = 13.0f;
            *(unsigned short *)(moby + 0x34) &= 0xEFFF;
            moby_vars->flash = 0xFA;
            moby[0x20] = 0x11;
        }
        trooper_anim_update(moby, moby_vars->anim);
    }
    ((unsigned char *)moby)[0xA4] = 0xFF;
    current_target = moby_vars->target;
    if (current_target == 0 || current_target[0x20] < 0 ||
        (*(short *)(current_target + 0xA6) != 0x10E && *(short *)(current_target + 0xA6) != 0xCB && *(short *)(current_target + 0xA6) != 0) ||
        ((unsigned char *)moby)[0x20] != 0xC) {
        FUN_L02_00261c80(moby, (char *)target, 14.0f);
        moby_vars->target = (char *)target[16];
    }
    if (moby_vars->target == 0) {
        moby_vars->target = ((char *)hero.moby);
    }
    trooper_anim_finish(moby, moby_vars->anim);
}

extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void add_vector_xyz(void *, void *, void *);
extern float absolute_float(float) __asm__("func_001F99C0");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern void update_entity_matrix(char *a, void *b, void *c, void *d) __asm__("FUN_L00_00260738");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern float D_0015ED6C;
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");


typedef union {
    OvlQuad q;
    float v[4];
} L16PlatformVector;

typedef struct {
    char pad00[0x60];
    float basis[4][4];
    int joint;
    int stop;
    float goal;
    int padAC;
    float speed;
    int particle;
} L16PlatformData;

typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0x1F];
    float rotation[4];
    char pad50[0x28];
    L16PlatformData *data;
    char pad7C[0x40];
    unsigned char reverse;
} L16PlatformMoby;

extern char *D_L02_001600FC;
extern float D_L02_00161BD0 __attribute__((sda));
extern unsigned char D_0013D4DA __attribute__((section(".data")));
extern int test_point_in_transformed_unit_cylinder(void *, int) __asm__("FUN_L00_0025fa38");
extern float D_L02_00161BDC __attribute__((sda));
extern float D_L02_00161BE0 __attribute__((sda));
void draw_elevator_rotating_rings(struct Moby *moby) __asm__("FUN_L02_002dc2c8");

extern void draw_elevator_rotating_rings(struct Moby *) __asm__("FUN_L02_002dc2c8");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");

/* Same platform routine as elevator_update, with a waiting state (1) and this level's tables. */
void elevator_update(L16PlatformMoby *moby) __asm__("FUN_L02_002dbd38");
void elevator_update(L16PlatformMoby *moby) {
    enum {
        ELEVATOR_INITIALIZE = 0,
        ELEVATOR_WAIT_FOR_RIDER_OR_TRIGGER = 1,
        ELEVATOR_WAIT_AT_STOP = 2,
        ELEVATOR_MOVE_TO_GOAL = 3
    };
    L16PlatformVector delta, old_rotation, motion;
    L16PlatformData *elevator_vars = moby->data;
    float *position = moby->position;
    float *rotation = moby->rotation;

    scale_vector_xyz(delta.v, position, -1.0f);
    qcopy(old_rotation.v, rotation);
    if (elevator_vars) {
        switch (moby->state) {
        case ELEVATOR_INITIALIZE:
            motion.q = 0;
            motion.v[2] = 1.0f;
            elevator_vars->particle = -1;
            transform_vector_by_basis(motion.v, motion.v, D_L02_001600FC + (elevator_vars->joint << 7));
            elevator_vars->goal = motion.v[2] + *(float *)(D_L02_001600FC + elevator_vars->joint * 0x80 + 0x38);
            moby->position[2] = motion.v[2] + *(float *)(D_L02_001600FC + elevator_vars->joint * 0x80 + 0x38) + D_L02_00161BD0;
            moby->reverse = 1;
            if (elevator_vars->stop) {
                moby->state = 2;
            } else {
                moby->state = 1;
            }
            break;
        case ELEVATOR_WAIT_AT_STOP:
            if (voice_is_playing(moby, elevator_vars->particle)) {
                release_voice_slot(elevator_vars->particle);
                elevator_vars->particle = -1;
            }
            if (vector_distance(position, hero.motion.pos.f) < 20.0f) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                transform_vector(motion.v, motion.v, D_L02_001600FC + (elevator_vars->joint << 7));
                motion.v[2] = motion.v[2] + *(float *)(D_L02_001600FC + elevator_vars->joint * 0x80 + 0x38) + D_L02_00161BD0;
                if (absolute_float(hero.motion.pos.f[2] - motion.v[2]) < 1.0f) {
                    moby->reverse = (moby->reverse + 1) & 1;
                    elevator_vars->goal = motion.v[2];
                    elevator_vars->speed = 0.0f;
                    moby->state = 3;
                    break;
                }
            }
            if (hero.ground_moby == (struct Moby *)moby && hero.air_frames.s == 0) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                moby->reverse = (moby->reverse + 1) & 1;
                transform_vector(motion.v, motion.v, D_L02_001600FC + (elevator_vars->joint << 7));
                elevator_vars->goal = motion.v[2] + *(float *)(D_L02_001600FC + elevator_vars->joint * 0x80 + 0x38) + D_L02_00161BD0;
                elevator_vars->speed = 0.0f;
                moby->state = 3;
            }
            break;
        case ELEVATOR_WAIT_FOR_RIDER_OR_TRIGGER:
            if ((hero.ground_moby == (struct Moby *)moby && hero.air_frames.s == 0) || D_0013D4DA) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                moby->reverse = (moby->reverse + 1) & 1;
                transform_vector(motion.v, motion.v, D_L02_001600FC + (elevator_vars->joint << 7));
                elevator_vars->goal = motion.v[2] + *(float *)(D_L02_001600FC + elevator_vars->joint * 0x80 + 0x38) + D_L02_00161BD0;
                elevator_vars->speed = 0.0f;
                moby->state = 3;
            }
            break;
        case ELEVATOR_MOVE_TO_GOAL: {
            float speed;

            if (!test_point_in_transformed_unit_cylinder(&hero.motion.unkD0, elevator_vars->joint)
                || absolute_float(moby->position[2] - hero.motion.pos.f[2] - 2.0f) > 1.0f
                || moby->position[2] < elevator_vars->goal) {
                speed = advance_accelerated_scalar(&moby->position[2], &elevator_vars->speed, elevator_vars->goal,
                                      D_L02_00161BE0 * frame_time_sq, D_L02_00161BE0 * frame_time_sq,
                                      D_L02_00161BDC * D_0015ED6C);
            } else {
                speed = advance_accelerated_scalar(&moby->position[2], &elevator_vars->speed, moby->position[2],
                                      D_L02_00161BE0 * 4.0f * frame_time_sq, D_L02_00161BE0 * 4.0f * frame_time_sq,
                                      D_L02_00161BDC * D_0015ED6C);
            }
            clear_u64_value(motion.v);
            motion.v[2] = speed;
            update_entity_matrix((char *)elevator_vars->basis, motion.v, moby->rotation, moby->rotation);
            if (motion.v[2] != 0.0f) {
                if (!voice_is_playing(moby, elevator_vars->particle)) {
                    elevator_vars->particle = allocate_voice_for_target_entry_alt(0, 4, (int)moby);
                }
            } else if (voice_is_playing(moby, elevator_vars->particle)) {
                release_voice_slot(elevator_vars->particle);
                elevator_vars->particle = -1;
            }
            if (moby->position[2] == elevator_vars->goal && (hero.ground_moby != (struct Moby *)moby || hero.air_frames.s != 0)) {
                clear_u64_value(elevator_vars->basis[1]);
                moby->state = 2;
                break;
            }
            if (elevator_vars->speed != 0.0f) {
                enqueue_callback_list_1(draw_elevator_rotating_rings, moby);
            }
            break;
        }
        }
        add_vector_xyz(delta.v, delta.v, moby->position);
        update_entity_matrix((char *)elevator_vars->basis, delta.v, old_rotation.v, moby->rotation);
    }
}

#include "qcopy.h"

/* Draw stacked alternating rings with a rotating transformation and interpolated tint. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9960), where it is exact; names translated to the US level program. */

typedef float L16RingVector[4] __attribute__((aligned(16)));

typedef struct {
    L16RingVector position[4];
    int color[4];
    struct {
        float u, v;
    } uv[4];
    long zero, texture, flags, mode;
} L16RingPacket;

typedef struct {
    L16RingVector basis[3];
    L16RingVector position;
} L16RingMatrix;

extern float absolute_float(float) __asm__("func_001F99C0");
extern float D_0015ED6C;
extern float D_L02_001D3F50[][2];
extern float fast_cos(float);
extern float fast_sin(float);
extern float wrap_angle(float);
extern int D_L02_0015F5CC;
extern float D_L02_00161BEC __attribute__((sda));
extern float D_L02_00161BE8 __attribute__((sda));
extern float D_L02_00161BF0 __attribute__((sda));
extern float D_L02_00161C10 __attribute__((sda));
int D_L02_00161BF4 = 0;
int D_L02_00161BF8 = 2;
int D_L02_00161BFC = 0;
int D_L02_00161C00 = 1;
int D_L02_00161C04 = 0x206068;
int D_L02_00161C08 = 0x40107020;
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int get_effect_texture(int) __asm__("FUN_001f44b8");
extern void copy_matrix_rotation(void *, void *) __asm__("FUN_001fa298");
extern void draw_geometry_quad(void *, void *, int);

void draw_elevator_rotating_rings(struct Moby *moby) {
    L16RingPacket packet;
    L16RingMatrix matrix;
    char *moby_vars = (char *)moby->pvars;
    int bound_index = (*(int *)((char *)moby_vars + 0xA0)) << 7;
    char *entry = (char *)(bound_index + (int)D_L02_001600FC);
    float translation = (*(float *)((char *)moby_vars + 0xB8));
    float lower = (*(float *)((char *)entry + 0x38));
    float half = (*(float *)((char *)entry + 0x28));
    float upper = lower + half + translation;
    int color;
    float(*uv)[2];
    L16RingVector *point;
    float *v, *u;
    L16RingMatrix *transform;
    float radius, index, phase;
    int i, j, sign, period;
    lower = lower - half;
    lower += translation;
    color =
        fast_tween_color(D_L02_00161C04, D_L02_00161C08,
                     absolute_float((*(float *)((char *)moby_vars + 0xB0))) / (D_L02_00161BDC * D_0015ED6C));
    copy_matrix_rotation(&matrix, &moby->rot_row0);
    qcopy(matrix.position, &moby->pos);
    matrix.position[2] = lower;
    matrix.position[3] = 1.0f;
    packet.texture = get_effect_texture(14);
    packet.flags = 0xFF9000000260L;
    packet.mode = (long)D_L02_00161BF4 | ((long)D_L02_00161BF8 << 2) | ((long)D_L02_00161BFC << 4) |
                  ((long)D_L02_00161C00 << 6) | 0x8000000000L;
    packet.zero = 0;
    uv = D_L02_001D3F50;
    point = packet.position;
    v = &packet.uv[0].v;
    u = &packet.uv[0].u;
    for (i = 0; i < 4; i++) {
        *u = (*uv)[0];
        *v = (*uv)[1];
        if (i & 1) {
            (*point)[2] = D_L02_00161BF0;
            radius = D_L02_00161BEC;
        } else {
            (*point)[2] = -D_L02_00161BF0;
            radius = D_L02_00161BEC - 0.05f;
        }
        index = (float)(i >> 1);
        (*point)[0] = fast_cos(index * (D_L02_00161C10 * DEG_TO_RAD)) * radius;
        (*point)[1] = fast_sin(index * (D_L02_00161C10 * DEG_TO_RAD)) * radius;
        (*point)[3] = 1.0f;
        v += 2;
        u += 2;
        uv++;
        point++;
    }
    transform = &matrix;
    sign = 1;
    if (matrix.position[2] < upper) {
        period = 120;
        do {
            phase = (float)(D_L02_0015F5CC % period) * 0.02617991715669632f;
            sign = -sign;
            for (j = 0; (float)j < 360.0f / D_L02_00161C10; j++) {
                matrix.basis[0][0] = fast_cos(
                    wrap_angle((D_L02_00161C10 * DEG_TO_RAD) * (float)j + phase * (float)sign));
                matrix.basis[0][1] = fast_sin(
                    wrap_angle((D_L02_00161C10 * DEG_TO_RAD) * (float)j + phase * (float)sign));
                matrix.basis[0][2] = 0.0f;
                matrix.basis[1][0] = fast_cos(wrap_angle(
                    (D_L02_00161C10 * DEG_TO_RAD) * (float)j + 1.5707964f + phase * (float)sign));
                matrix.basis[1][1] = fast_sin(wrap_angle(
                    (D_L02_00161C10 * DEG_TO_RAD) * (float)j + 1.5707964f + phase * (float)sign));
                matrix.basis[1][2] = 0.0f;
                matrix.basis[2][2] = 1.0f;
                packet.color[3] = color;
                packet.color[2] = color;
                packet.color[1] = color;
                packet.color[0] = color;
                draw_geometry_quad(&packet, transform, 0);
            }
            matrix.position[2] += D_L02_00161BE8;
        } while (matrix.position[2] < upper);
    }
}
/* UpdateMoby_656: steps the moby's animation state from 0/2 (when flagged) and 1/3 (when the timer runs out) */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DDE48), where it is exact; names translated to the US level program. */

extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int FUN_L00_001fefc8(void *);
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
void blend_moby_animation(MobyAnim *moby, int new_seq, int frame_index, int blend_frames) __asm__("FUN_00212f90");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void bulldozer_update(struct Moby *moby) __asm__("FUN_L02_002dca10");
void bulldozer_update(struct Moby *moby) {
    enum {
        BULLDOZER_ANIM_TRANSITION_TO_TIMED_WAIT = 0,
        BULLDOZER_ANIM_TIMED_WAIT = 1,
        BULLDOZER_ANIM_TRANSITION_TO_TIMED_WAIT_BEFORE_RESTART = 2,
        BULLDOZER_ANIM_TIMED_WAIT_BEFORE_RESTART = 3
    };
    switch (moby->prev_seq) {
    case BULLDOZER_ANIM_TRANSITION_TO_TIMED_WAIT:
        if (moby->anim_flags & 2) {
            blend_moby_animation(moby, 1, 0, 0);
            moby->command = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(180.0f, 240.0f)));
        }
        break;
    case BULLDOZER_ANIM_TRANSITION_TO_TIMED_WAIT_BEFORE_RESTART:
        if (moby->anim_flags & 2) {
            blend_moby_animation(moby, 3, 0, 0);
            moby->command = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(180.0f, 240.0f)));
        }
        break;
    case BULLDOZER_ANIM_TIMED_WAIT:
        if (FUN_L00_001fefc8(&moby->command) != 0 && moby->prev_seq != 2) {
            blend_moby_animation(moby, 2, 0, 0);
        }
        break;
    case BULLDOZER_ANIM_TIMED_WAIT_BEFORE_RESTART:
        if (FUN_L00_001fefc8(&moby->command) != 0 && moby->prev_seq != 0) {
            blend_moby_animation(moby, 0, 0, 0);
        }
        break;
    }
}
/* pvars of the class sand_shark_nest_update updates (only the fields it touches). */
typedef struct {
    char pad0[0x20];
    float f20;           /* 0x20 */
    short s24;           /* 0x24 */
    char pad26[8];
    unsigned char b2E;   /* 0x2E */
    char pad2F[0x11];
    float vel[4];        /* 0x40: passed to spawn_explosion */
    char pad50[0x20];
    char anim[0x64];     /* 0x70: passed to mine_fall */
    int delay;           /* 0xD4: frames between spawns */
    int timer;           /* 0xD8 */
    char padDC[4];
    int spawner;         /* 0xE0: spawner moby index, -1 if none */
} Varscb38;

/* Counters kept in the spawner moby's pvars. */
typedef struct {
    char pad0[0x14C];
    int i14C;
    int i150;
} Spawnercb38;

extern unsigned short D_0014D590_cb38[][64][2] __asm__("D_0014D590") __attribute__((section(".data")));
extern int D_0014EE90[];
extern int D_L02_0015FFD8;
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern int scale_ticks(int) __asm__("FUN_001f96f8");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
void handle_sand_shark_nest_damage(struct Moby *moby) __asm__("FUN_L02_002dcfe0");

extern void handle_sand_shark_nest_damage(struct Moby *m) __asm__("FUN_L02_002dcfe0");
extern char *FUN_L02_002dd2a8_c(void *) __asm__("FUN_L02_002dd2a8");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float, float, int,
                             float, float, int, int, int, int) __asm__("FUN_L00_0025e450");

/* Spawner-fed moby: counts itself in on spawn, hands itself to a group target on a timer, fires a
   projectile at it (state 2) and bursts into debris when its hit animation flags it (state 4). */
void sand_shark_nest_update(unsigned char *moby) __asm__("FUN_L02_002dcb38");
void sand_shark_nest_update(unsigned char *moby) {
    enum {
        SAND_SHARK_NEST_INITIALIZE = 0,
        SAND_SHARK_NEST_WAIT_FOR_SPAWN = 1,
        SAND_SHARK_NEST_LAUNCH_SAND_SHARK = 2,
        SAND_SHARK_NEST_HIT_RECOVERY = 3,
        SAND_SHARK_NEST_DEATH_KNOCKBACK = 4
    };
    Varscb38 *nest_vars = *(Varscb38 **)(moby + 0x78);
    char *launched_shark;
    int spawn_delay_factor;
    OvlVec4 launch_velocity_or_explosion_position;
    OvlVec4 launch_position;

    handle_sand_shark_nest_damage((struct Moby *)moby);
    if ((signed char)moby[0x20] < 0) {
        return;
    }
    if (moby[0x31] != 0 && distance_xyz(moby + 0x10, D_L02_001673C0) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby[0x7F] = 0x16;
    }
    switch (moby[0x20]) {
    case SAND_SHARK_NEST_INITIALIZE:
        *(short *)(moby + 0xB4) -= D_0014D590_cb38[2][moby[0xB1]][1];
        if (*(short *)(moby + 0xB4) <= 0) {
            *(short *)(moby + 0xB4) = 1;
        }
        moby[0x20] = 1;
        nest_vars->f20 = 3.0f;
        nest_vars->s24 = 3;
        nest_vars->b2E = 1;
        nest_vars->timer = scale_ticks(nest_vars->delay);
        if (nest_vars->spawner >= 0) {
            (*(Spawnercb38 **)((nest_vars->spawner << 8) + D_L02_0015FFD8 + 0x78))->i150 += 1;
        }
        break;
    case SAND_SHARK_NEST_WAIT_FOR_SPAWN:
        if (fast_dec_timer(&nest_vars->timer)) {
            if (FUN_L02_002dd2a8_c(moby)) {
                moby[0x20] = 2;
                if (moby[0x53] != 1) {
                    blend_moby_animation_c3(moby, 1, 0, scale_ticks(10));
                }
            } else {
                nest_vars->timer = scale_ticks(nest_vars->delay);
            }
        }
        break;
    case SAND_SHARK_NEST_LAUNCH_SAND_SHARK:
        if (moby[0x70] & 2) {
            moby[0x20] = 1;
            if (moby[0x53] != 0) {
                blend_moby_animation_c3(moby, 0, 0, scale_ticks(10));
            }
        } else if (compute_interpolated_record_value(moby) == 12.0f) {
            spawn_delay_factor = D_0014EE90[moby[0xB0]];
            if (spawn_delay_factor > 20) {
                spawn_delay_factor = 20;
            }
            nest_vars->timer = scale_ticks(nest_vars->delay + scale_ticks(spawn_delay_factor * 15));
            launched_shark = FUN_L02_002dd2a8_c(moby);
            if (launched_shark) {
                char *od = *(char **)(launched_shark + 0x78);
                if (nest_vars->spawner >= 0) {
                    (*(Spawnercb38 **)((nest_vars->spawner << 8) + D_L02_0015FFD8 + 0x78))->i14C += 1;
                }
                qcopy(&launch_position, moby + 0x10);
                launch_position.f[2] += 1.5f;
                fast_vec_sub(&launch_velocity_or_explosion_position, od + 0x220, moby + 0x10);
                set_vector_length_xy(&launch_velocity_or_explosion_position, &launch_velocity_or_explosion_position, 2.0f * D_0015ED6C);
                launch_velocity_or_explosion_position.f[2] = D_0015ED6C * 6.0f;
                launch_sand_shark((unsigned char *)launched_shark, &launch_position, launch_velocity_or_explosion_position.f);
            }
        }
        break;
    case SAND_SHARK_NEST_HIT_RECOVERY:
        mine_fall(moby, nest_vars->anim);
        if (moby[0x70] & 2) {
            moby[0x20] = 1;
            if (moby[0x53] != 0) {
                blend_moby_animation_c3(moby, 0, 0, scale_ticks(20));
            }
        }
        break;
    case SAND_SHARK_NEST_DEATH_KNOCKBACK:
        if (mine_fall(moby, nest_vars->anim) & 0x40) {
            if (nest_vars->spawner >= 0) {
                extern int D_L02_0015FFD8_s __asm__("D_L02_0015FFD8") __attribute__((sda));
                (*(Spawnercb38 **)((nest_vars->spawner << 8) + D_L02_0015FFD8_s + 0x78))->i150 -= 1;
            }
            qcopy(&launch_velocity_or_explosion_position, moby + 0x10);
            spawn_explosion(moby, nest_vars->vel, &launch_velocity_or_explosion_position, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, -1, 1.0f, 15.0f, 1, 1,
                             -1, 0);
            FUN_L00_00263fd8((char *)moby, 0x6E4, (float *)(moby + 0x10), moby + 0x40, 0, 0, 0.0f, D_L02_0015F580,
                             D_L02_0015F580, D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby, 0x6E4, (float *)(moby + 0x10), moby + 0x40, 0, 0, 0.0f, D_L02_0015F580,
                             D_L02_0015F580, D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby, 0x6E5, (float *)(moby + 0x10), moby + 0x40, 0, 0, 0.0f, D_L02_0015F580,
                             D_L02_0015F580, D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby, 0x6E5, (float *)(moby + 0x10), moby + 0x40, 0, 0, 0.0f, D_L02_0015F580,
                             D_L02_0015F580, D_L02_0015F580);
            mark_moby_for_removal(moby);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DE418), where it is exact; names translated to the US level program. */


extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void FUN_L00_00257470(void *, int, int);
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void blend_moby_animation_c2(void *, s32, s32, s32) __asm__("FUN_00212f90");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

void handle_sand_shark_nest_damage(struct Moby *moby) {
    char *nest_vars = (char *)moby->pvars;
    float hit_knockback_vector[4];
    int hit;
    float dmg;
    float k1;
    float k2;
    char *hit_record;
    dmg = 0.0f;
    if (((unsigned char *)nest_vars)[0x2E] == 2) {
        nest_vars[0x2E] = 1;
        if (*(int *)(nest_vars + 0xE0) >= 0) {
            char *od = *(char **)((*(int *)(nest_vars + 0xE0) << 8) + D_L02_0015FFD8 + 0x78);
            *(int *)(od + 0x150) -= 1;
        }
        mark_moby_for_removal(moby);
        return;
    }
    hit_record = find_moby_hit(moby, 0x330000, 0);
    hit_reaction(moby, hit_record, nest_vars + 0x20, 0, &hit, &dmg, 0, 4);
    if (hit_record != 0 && hit != 1 && moby->state != 4) {
        if (hit == 3) {
            *(float *)(nest_vars + 0x20) = 0.0f;
        } else {
            *(float *)(nest_vars + 0x20) -= dmg;
        }
        *(int *)(nest_vars + 0x94) = 4;
        if (0.0f < *(float *)(nest_vars + 0x20)) {
            {
                float a = D_0015ED6C * 4.0f;
                float b = D_0015ED6C * 3.0f;
                *(float *)(nest_vars + 0x88) = a;
                *(float *)(nest_vars + 0x8C) = b;
            }
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit_record + 0x10);
            trooper_knockback_arc(hit_knockback_vector, &k1, nest_vars + 0x88, nest_vars + 0x8C);
            trooper_start_knockback(k1, moby, nest_vars + 0x70, 3, 1, 0);
            if (1.0f <= dmg) {
                nest_vars[0x67] = 0x78;
                moby->state = 3;
                blend_moby_animation_c2(moby, 2, 0, 0);
            } else {
                nest_vars[0x67] = 0x78;
            }
        } else {
            moby->flags &= 0xEFFF;
            ((unsigned char *)nest_vars)[0x67] = 0xFA;
            moby->state = 4;
            {
                float b = D_0015ED6C * 11.0f;
                float a = D_0015ED6C * 3.0f;
                *(float *)(nest_vars + 0x8C) = a;
                *(float *)(nest_vars + 0x88) = b;
            }
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit_record + 0x10);
            trooper_knockback_arc(hit_knockback_vector, &k2, nest_vars + 0x88, nest_vars + 0x8C);
            trooper_start_knockback(k2, moby, nest_vars + 0x70, 3, 1, 0);
            *(float *)(nest_vars + 0xC0) = 9.0f;
            *(float *)(nest_vars + 0xC4) = 19.0f;
            if (moby->prev_seq != 3)
                blend_moby_animation_c2(moby, 3, 0, 0);
            FUN_L00_00257470(moby, 0, -1);
        }
        trooper_anim_update(moby, (short *)(nest_vars + 0x60));
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, nest_vars + 0x60);
    FUN_L00_001fefc8(&moby->command);
}
typedef struct {
    char pad0[0x20];
    unsigned char state;
    unsigned char group;
    char pad22[0x84];
    short type;
    char padA8[0xC];
    short hp;
    char padB6[2];
    void *target;
    char padBC[0x44];
} Level02GroupMoby;

extern Level02GroupMoby *D_L02_0015FFD8_g __asm__("D_L02_0015FFD8");

/* Finds the group's moby of type 0x244 in state 0x1A, hands it arg as target, and transfers hp to arg; returns it or 0. */
Level02GroupMoby *FUN_L02_002dd2a8(Level02GroupMoby *owner) {
    short *group_ids;
    Level02GroupMoby *mobys;
    if (owner->group == 0xFF) {
        return 0;
    }
    group_ids = (short *)D_L02_001ABE40[owner->group];
    mobys = D_L02_0015FFD8_g;
    do {
        int i = *(unsigned short *)group_ids & 0x7FFF;
        if (mobys[i].type == 0x244 && mobys[i].state == 0x1A) {
            mobys[i].target = owner;
            if (owner->hp < mobys[i].hp) {
                mobys[i].hp = owner->hp;
            }
            owner->hp -= D_L02_0015FFD8_g[i].hp;
            if (owner->hp <= 0) {
                owner->hp = 1;
            }
            return &D_L02_0015FFD8_g[i];
        }
    } while (*group_ids++ >= 0);
    return 0;
}
/* four-state update: waits for a flag, plays a transition, and rolls a random heading on entering state 2 or 7 */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DE7A8), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int) __asm__("FUN_001f96f8");
void blend_moby_animation(MobyAnim *moby, int new_seq, int frame_index, int blend_frames) __asm__("FUN_00212f90");

void grab_crane_update(struct Moby *moby) __asm__("FUN_L02_002dd370");
void grab_crane_update(struct Moby *moby) {
    enum {
        GRAB_CRANE_ANIM_TRANSITION_TO_TIMED_WAIT = 0,
        GRAB_CRANE_ANIM_TIMED_WAIT = 2,
        GRAB_CRANE_ANIM_TRANSITION_TO_TIMED_WAIT_BEFORE_RESTART = 3,
        GRAB_CRANE_ANIM_TIMED_WAIT_BEFORE_RESTART = 7
    };
    switch (moby->prev_seq) {
    case GRAB_CRANE_ANIM_TRANSITION_TO_TIMED_WAIT:
        if (moby->anim_flags & 2) {
            blend_moby_animation(moby, 2, 0, scale_game_frames(5));
            moby->command = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(60.0f, 180.0f)));
        }
        break;
    case GRAB_CRANE_ANIM_TIMED_WAIT:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (moby->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(5));
        }
        break;
    case GRAB_CRANE_ANIM_TRANSITION_TO_TIMED_WAIT_BEFORE_RESTART:
        if (moby->anim_flags & 2) {
            blend_moby_animation(moby, 7, 0, scale_game_frames(5));
            moby->command = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(60.0f, 180.0f)));
        }
        break;
    case GRAB_CRANE_ANIM_TIMED_WAIT_BEFORE_RESTART:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(5));
        }
        break;
    }
}
#include "qzero.h"

/* Rotates a platform and updates its transform. */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DF038), where it is exact; names translated to the US level program. */

extern float fast_add_rotations_c(float, float) __asm__("func_001FA580");
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");

void platform_update(struct Moby *moby) __asm__("FUN_L02_002ddc00");
void platform_update(struct Moby *moby)
{
    char zero[16];
    char previous_rotation[16];
    char *data = (char *)moby->pvars;
    qzero(zero);
    qcopy(previous_rotation, &moby->rot);
    moby->rot.z = fast_add_rotations_c(moby->rot.z, D_0015ED6C * 0.08726646f);
    update_entity_matrix(data + 0x20, zero, previous_rotation, &moby->rot);
}
#include "rnc/gameplay/state/usage_stats.h"

/* pvars of the class transport_pod_update updates (the lift: two rail mobys 0x421/0x422 ride with it). */
typedef struct {
    char pad0[0x20];
    char riders[0x40];   /* 0x20: passed to update_entity_matrix */
    int node_a;          /* 0x60: D_L02_001600EC node of the lower stop */
    int node_b;          /* 0x64: node of the upper stop */
    int path;            /* 0x68: D_L02_001B0AB0 camera path */
    int pad6C;
    int gear[4];         /* 0x70: moby indices pushed along their axis in state 2 */
    int timer;           /* 0x80 */
    char *rail_a;        /* 0x84: class 0x421 moby */
    char *rail_b;        /* 0x88: class 0x422 moby */
    float rail_a_speed;  /* 0x8C */
    float rail_b_speed;  /* 0x90 */
    float rail_a_off;    /* 0x94 */
    float rail_b_off;    /* 0x98 */
    float speed;         /* 0x9C */
    int cam_goal;        /* 0xA0 */
    float cam_pos;       /* 0xA4 */
    float cam_speed;     /* 0xA8 */
    int ride_timer;      /* 0xAC */
    float cam_accel;     /* 0xB0 */
    float cam_max;       /* 0xB4 */
} Varsdc88;

extern float D_L02_00161C20 __attribute__((sda));
extern float D_L02_00161C24 __attribute__((sda));
extern float D_L02_00161C40 __attribute__((sda));
extern float D_L02_00161C44 __attribute__((sda));
extern float D_L02_00161C48 __attribute__((sda));
extern float D_L02_00161C4C __attribute__((sda));
extern float D_L02_00161C50 __attribute__((sda));
extern float D_L02_00161C54 __attribute__((sda));
extern float D_L02_00161C58 __attribute__((sda));
extern float D_L02_00161C60 __attribute__((sda));
extern float D_L02_00161C30_gp __asm__("D_L02_00161C30") __attribute__((sda));
extern float D_L02_00161C34_gp __asm__("D_L02_00161C34") __attribute__((sda));
extern int *D_L02_001B0AB0_dc88[] __asm__("D_L02_001B0AB0");
extern unsigned char D_0014C050_dc88[] __asm__("D_0014C050");
extern unsigned int D_00141BAC __attribute__((section(".data")));
extern float D_0015ED6C;
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern float absolute_float_dc88(float) __asm__("func_001F99C0");
extern float FUN_00213f38(float *, float *, float, float, float, float);
extern void sample_camera_path(void *, int, void *, void *, int, float) __asm__("FUN_00214e58");
extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern int scale_ticks(int) __asm__("FUN_001f96f8");
extern void FUN_0022da68_dc88(int, int, void *) __asm__("FUN_0022da68");
extern void help_request(int, int) __asm__("FUN_L00_00203908");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void FUN_L00_00233ee8(void *, int, float);
extern int FUN_L00_00233f38();
extern void FUN_L00_00250df8(void *);
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eac18(int);
extern void path_marker_dc88(void *, void *, int, int, int, int, int, float) __asm__("FUN_L01_00285768");
extern void FUN_L02_0022b728(int, int);
extern void enter_hero_idle_or_stop(void) __asm__("FUN_L02_00237478");
void advance_transport_pod_rails_to_offsets(struct Moby *moby) __asm__("FUN_L02_002def80");

extern void advance_transport_pod_rails_to_offsets(struct Moby *moby) __asm__("FUN_L02_002def80");
extern void snap_transport_pod_rails_to_offsets(struct Moby *moby) __asm__("FUN_L02_002df0b0");
extern void set_vendor_camera_mode(int, int) __asm__("FUN_L02_002f8a18");
extern void FUN_L02_002f8aa0(float, float, float, float, float, float);

/* D_L02_001600EC entries (0x80 bytes): lift stops. */
typedef struct {
    char pad0[0x30];
    float pos[4];        /* 0x30 */
    char pad40[0x30];
    float look[4];       /* 0x70: camera target */
} Nodedc88;

extern Nodedc88 *D_L02_001600EC_n __asm__("D_L02_001600EC");

extern int D_0015EEA4_gp __asm__("D_0015EEA4") __attribute__((sda));
/* Aridia lift: carries the hero between two stops, with its two rail mobys, gears and a camera path. */
void transport_pod_update(unsigned char *moby) __asm__("FUN_L02_002ddc88");
void transport_pod_update(unsigned char *moby) {
    enum {
        TRANSPORT_POD_INITIALIZE = 0,
        TRANSPORT_POD_WAIT_FOR_GEAR_MOVEMENT_TRIGGER = 1,
        TRANSPORT_POD_TIMED_GEAR_MOVEMENT = 2,
        TRANSPORT_POD_RAISE_FOR_BOARDING = 3,
        TRANSPORT_POD_POSITION_RAILS_FOR_BOARDING = 4,
        TRANSPORT_POD_WAIT_FOR_RIDER_OR_RECALL = 5,
        TRANSPORT_POD_WAIT_FOR_RIDER_READY_FOR_DEPARTURE = 6,
        TRANSPORT_POD_POSITION_RAILS_FOR_DEPARTURE = 7,
        TRANSPORT_POD_LOWER_AND_TRANSFER_TO_OTHER_STOP = 8,
        TRANSPORT_POD_ADVANCE_RIDE_CAMERA = 9,
        TRANSPORT_POD_RAISE_FOR_DISEMBARKING = 10,
        TRANSPORT_POD_POSITION_RAILS_FOR_DISEMBARKING = 11,
        TRANSPORT_POD_RAISE_TO_STOP_HEIGHT = 12,
        TRANSPORT_POD_WAIT_FOR_RIDER_TO_LEAVE = 13,
        TRANSPORT_POD_CREATE_RAILS = 14,
        TRANSPORT_POD_POSITION_RAILS_FROM_CONFIGURED_OFFSETS = 15,
        TRANSPORT_POD_CYCLE_RAILS_TO_SPLIT_HEIGHTS = 16,
        TRANSPORT_POD_CYCLE_RAILS_TO_SHARED_LOWER_HEIGHT = 17
    };
    Varsdc88 *transport_pod_vars = *(Varsdc88 **)(moby + 0x78);
    int i;
    int *path;
    int help_request_threshold_ticks;
    int adjusted_help_ticks;
    float s;
    OvlVec4 delta;
    OvlVec4 previous_rotation;
    OvlVec4 a;
    OvlVec4 b;
    OvlVec4 camera_position_or_stop_look;
    OvlVec4 initial_camera_rotation;

    fast_vec_scale(&delta, moby + 0x10, -1.0f);
    qcopy(&previous_rotation, moby + 0x40);
    if (D_L02_00161C44 != 0.0f) {
        for (i = 0; i < *D_L02_001B0AB0_dc88[transport_pod_vars->path]; i++) {
            qcopy(&a, (char *)D_L02_001B0AB0_dc88[transport_pod_vars->path] + i * 0x10 + 0x10);
            path_marker_dc88(&a, moby, 4, 0x800000FF, 0x7F, 2, 0xFF, 0.1f);
        }
    }
    switch (moby[0x20]) {
    case TRANSPORT_POD_INITIALIZE:
        moby[0x30] = 0xFF;
        moby[0xBC] = 1;
        qcopy(moby + 0x10, D_L02_001600EC_n[transport_pod_vars->node_b].pos);
        moby[0x20] = 1;
        ((struct Moby *)moby)->pos.z -= 6.0f;
        transport_pod_vars->rail_a = create_moby(0x421);
        *(short *)(transport_pod_vars->rail_a + 0x32) = 0x40;
        transport_pod_vars->rail_a[0x31] = 1;
        *(u64 *)(transport_pod_vars->rail_a + 0x38) = *(u64 *)(moby + 0x38);
        *(unsigned short *)(transport_pod_vars->rail_a + 0x34) = *(unsigned short *)(moby + 0x34);
        qcopy_nc(transport_pod_vars->rail_a + 0x10, moby + 0x10);
        qcopy_nc(transport_pod_vars->rail_a + 0x40, moby + 0x40);
        transport_pod_vars->rail_b = create_moby(0x422);
        *(short *)(transport_pod_vars->rail_b + 0x32) = 0x40;
        transport_pod_vars->rail_b[0x31] = 1;
        *(u64 *)(transport_pod_vars->rail_b + 0x38) = *(u64 *)(moby + 0x38);
        *(unsigned short *)(transport_pod_vars->rail_b + 0x34) = *(unsigned short *)(moby + 0x34);
        qcopy_nc(transport_pod_vars->rail_b + 0x10, moby + 0x10);
        qcopy_nc(transport_pod_vars->rail_b + 0x40, moby + 0x40);
        transport_pod_vars->rail_a_off = -1.0f;
        transport_pod_vars->rail_b_off = -1.0f;
        transport_pod_vars->cam_max = frame_time_sq * 0.1f * (float)*D_L02_001B0AB0_dc88[transport_pod_vars->path];
        transport_pod_vars->cam_accel = D_0015ED6C * 0.3f * (float)*D_L02_001B0AB0_dc88[transport_pod_vars->path];
        snap_transport_pod_rails_to_offsets((struct Moby *)moby);
        break;
    case TRANSPORT_POD_WAIT_FOR_GEAR_MOVEMENT_TRIGGER:
        if (D_0014C050_dc88[moby[0xB0] + current_level_index * 16] == 0xFF) {
            transport_pod_vars->timer = scale_ticks(30);
            moby[0x20] = 2;
        }
        break;
    case TRANSPORT_POD_TIMED_GEAR_MOVEMENT:
        s = D_0015ED6C * 5.0f;
        fast_vec_normalize(&a, (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[0] << 8) + 0xC0, -s);
        fast_vec_add((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[0] << 8) + 0x10,
                     (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[0] << 8) + 0x10, &a);
        fast_vec_normalize(&a, (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[1] << 8) + 0xC0, s);
        fast_vec_add((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[1] << 8) + 0x10,
                     (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[1] << 8) + 0x10, &a);
        fast_vec_normalize(&a, (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[2] << 8) + 0xC0, -s);
        fast_vec_add((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[2] << 8) + 0x10,
                     (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[2] << 8) + 0x10, &a);
        fast_vec_normalize(&a, (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[3] << 8) + 0xC0, s);
        fast_vec_add((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[3] << 8) + 0x10,
                     (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[3] << 8) + 0x10, &a);
        FUN_L00_00250df8((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[0] << 8));
        FUN_L00_00250df8((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[1] << 8));
        FUN_L00_00250df8((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[2] << 8));
        FUN_L00_00250df8((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[3] << 8));
        if (fast_dec_timer(&transport_pod_vars->timer)) {
            moby[0x20] = 3;
            FUN_0022da68_dc88(1, 0, moby);
        }
        break;
    case TRANSPORT_POD_RAISE_FOR_BOARDING:
        FUN_00213f38((float *)(moby + 0x18), &transport_pod_vars->speed, D_L02_001600EC_n[transport_pod_vars->node_a].pos[2],
                     D_L02_00161C34_gp * frame_time_sq, D_L02_00161C34_gp * frame_time_sq,
                     D_L02_00161C30_gp * D_0015ED6C);
        snap_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (D_L02_001600EC_n[transport_pod_vars->node_a].pos[2] - 0.0001f <= ((struct Moby *)moby)->pos.z) {
            moby[0x20] = 4;
            FUN_0022da68_dc88(0, 0, moby);
        }
        break;
    case TRANSPORT_POD_POSITION_RAILS_FOR_BOARDING:
        transport_pod_vars->rail_a_off = 2.0f;
        transport_pod_vars->rail_b_off = -4.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (absolute_float_dc88(*(float *)(transport_pod_vars->rail_a + 0x18) - ((struct Moby *)moby)->pos.z - 2.0f) < 0.0001f &&
            absolute_float_dc88(*(float *)(transport_pod_vars->rail_b + 0x18) - ((struct Moby *)moby)->pos.z + 4.0f) < 0.0001f) {
            moby[0x20] = 5;
        }
        break;
    case TRANSPORT_POD_WAIT_FOR_RIDER_OR_RECALL:
        if (moby[0xBC] == 0 &&
            vector_distance(&hero.motion.pos, D_L02_001600EC_n[transport_pod_vars->node_b].pos) < 16.0f) {
            qcopy(moby + 0x10, D_L02_001600EC_n[transport_pod_vars->node_b].pos);
            ((struct Moby *)moby)->pos.z -= 6.0f;
            transport_pod_vars->rail_a_off = -1.0f;
            transport_pod_vars->rail_b_off = -1.0f;
            snap_transport_pod_rails_to_offsets((struct Moby *)moby);
            moby[0x20] = 3;
            moby[0xBC] = 1;
            FUN_0022da68_dc88(1, 0, moby);
        } else if (moby[0xBC] == 1 &&
                   vector_distance(&hero.motion.pos, D_L02_001600EC_n[transport_pod_vars->node_a].pos) < 16.0f) {
            qcopy(moby + 0x10, D_L02_001600EC_n[transport_pod_vars->node_a].pos);
            ((struct Moby *)moby)->pos.z -= 6.0f;
            transport_pod_vars->rail_a_off = -1.0f;
            transport_pod_vars->rail_b_off = -1.0f;
            snap_transport_pod_rails_to_offsets((struct Moby *)moby);
            moby[0xBC] = 0;
            moby[0x20] = 3;
            FUN_0022da68_dc88(1, 0, moby);
        } else if ((unsigned char *)hero.ground_moby == (struct Moby *)moby && hero.air_frames.s == 0 && hero.vehicle_mode == 0) {
            qcopy(&b, moby + 0x10);
            b.f[2] += 2.0f;
            if (moby[0xBC] == 0) {
                transport_pod_vars->cam_goal = 0;
                transport_pod_vars->cam_pos = (float)(*D_L02_001B0AB0_dc88[transport_pod_vars->path] - 1);
                qcopy_nc(&a, D_L02_001600EC_n[transport_pod_vars->node_a].look);
                qcopy(&camera_position_or_stop_look, (char *)D_L02_001B0AB0_dc88[transport_pod_vars->path] + trunc_d57f8(transport_pod_vars->cam_pos) * 0x10 + 0x10);
                clear_vec_d57f8(&initial_camera_rotation);
                initial_camera_rotation.f[2] = fast_arc_tan(b.f[0] - camera_position_or_stop_look.f[0], b.f[1] - camera_position_or_stop_look.f[1]);
                initial_camera_rotation.f[1] = -fast_arc_tan(vector_distance(&camera_position_or_stop_look, &b), b.f[2] - camera_position_or_stop_look.f[2]);
            } else {
                transport_pod_vars->cam_goal = *D_L02_001B0AB0_dc88[transport_pod_vars->path] - 1;
                transport_pod_vars->cam_pos = 0.0f;
                qcopy_nc(&a, D_L02_001600EC_n[transport_pod_vars->node_b].look);
                qcopy(&camera_position_or_stop_look, (char *)D_L02_001B0AB0_dc88[transport_pod_vars->path] + trunc_d57f8(transport_pod_vars->cam_pos) * 0x10 + 0x10);
                clear_vec_d57f8(&initial_camera_rotation);
                initial_camera_rotation.f[2] = fast_arc_tan(b.f[0] - camera_position_or_stop_look.f[0], b.f[1] - camera_position_or_stop_look.f[1]);
                initial_camera_rotation.f[1] = -fast_arc_tan(vector_distance(&camera_position_or_stop_look, &b), b.f[2] - camera_position_or_stop_look.f[2]);
            }
            moby[0x20] = 6;
            transport_pod_vars->ride_timer = scale_ticks(0xB4);
            FUN_L00_00233ee8(moby + 0x10, 0, a.f[2]);
            jet_mount(D_L02_001673C0, D_L02_001673C0 + 0x10, 3, scale_ticks(0xF0), 0);
            FUN_L02_002f8aa0(D_L02_00161C48, D_L02_00161C4C, D_L02_00161C50, D_L02_00161C54, D_L02_00161C58,
                             D_L02_00161C60);
            FUN_L00_002ea9d8(&camera_position_or_stop_look);
            FUN_L00_002eaa30(&initial_camera_rotation);
        }
        break;
    case TRANSPORT_POD_WAIT_FOR_RIDER_READY_FOR_DEPARTURE: {
        int rider_readiness_status = FUN_L00_00233f38();
        if (rider_readiness_status == 2) {
            moby[0x20] = 7;
            FUN_0022da68_dc88(2, 0, moby);
            FUN_L02_0022b728(0x72, 0);
        } else if (rider_readiness_status == 0) {
            FUN_L00_002eac18(2);
            moby[0x20] = 0xD;
        } else if (fast_dec_timer(&transport_pod_vars->ride_timer)) {
            FUN_L00_002eac18(2);
            moby[0x20] = 0xD;
            enter_hero_idle_or_stop();
        }
        break;
    }
    case TRANSPORT_POD_POSITION_RAILS_FOR_DEPARTURE:
        transport_pod_vars->rail_a_off = -1.0f;
        transport_pod_vars->rail_b_off = -1.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (absolute_float_dc88(*(float *)(transport_pod_vars->rail_a + 0x18) - ((struct Moby *)moby)->pos.z + 1.0f) < 0.01f &&
            absolute_float_dc88(*(float *)(transport_pod_vars->rail_b + 0x18) - ((struct Moby *)moby)->pos.z + 1.0f) < 0.01f) {
            moby[0x20] = 8;
        }
        break;
    case TRANSPORT_POD_LOWER_AND_TRANSFER_TO_OTHER_STOP:
        FUN_00213f38((float *)(moby + 0x18), &transport_pod_vars->speed,
                     D_L02_001600EC_n[transport_pod_vars->node_a].pos[2] - 6.0f,
                     D_L02_00161C34_gp * frame_time_sq, D_L02_00161C34_gp * frame_time_sq,
                     D_L02_00161C30_gp * D_0015ED6C);
        snap_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (((struct Moby *)moby)->pos.z < D_L02_001600EC_n[transport_pod_vars->node_a].pos[2] - 5.999f) {
            if (moby[0xBC] == 0) {
                moby[0xBC] = 1;
                qcopy_nc(moby + 0x10, D_L02_001600EC_n[transport_pod_vars->node_b].pos);
                qcopy_nc(&camera_position_or_stop_look, D_L02_001600EC_n[transport_pod_vars->node_b].look);
                fast_vec_sub(&a, D_L02_001600EC_n[transport_pod_vars->node_b].pos, D_L02_001600EC_n[transport_pod_vars->node_a].pos);
            } else {
                moby[0xBC] = 0;
                qcopy_nc(moby + 0x10, D_L02_001600EC_n[transport_pod_vars->node_a].pos);
                qcopy_nc(&camera_position_or_stop_look, D_L02_001600EC_n[transport_pod_vars->node_a].look);
                fast_vec_sub(&a, D_L02_001600EC_n[transport_pod_vars->node_a].pos, D_L02_001600EC_n[transport_pod_vars->node_b].pos);
            }
            ((struct Moby *)moby)->pos.z -= 6.0f;
            fast_vec_add(&b, &hero.motion.pos, &a);
            FUN_L00_00216f90(&b, &camera_position_or_stop_look, 0x72, 0);
            transport_pod_vars->speed = -transport_pod_vars->speed;
            moby[0x20] = 9;
            set_vendor_camera_mode(2, scale_ticks(0xB4));
        }
        break;
    case TRANSPORT_POD_ADVANCE_RIDE_CAMERA:
        FUN_00213f38(&transport_pod_vars->cam_pos, &transport_pod_vars->cam_speed, (float)transport_pod_vars->cam_goal, transport_pod_vars->cam_max, transport_pod_vars->cam_max, transport_pod_vars->cam_accel);
        if (transport_pod_vars->cam_goal != 0 ? (float)transport_pod_vars->cam_goal <= transport_pod_vars->cam_pos : transport_pod_vars->cam_pos <= (float)transport_pod_vars->cam_goal) {
            moby[0x20] = 10;
        }
        sample_camera_path(D_L02_001B0AB0_dc88[transport_pod_vars->path], 0, &camera_position_or_stop_look, &b, 0, transport_pod_vars->cam_pos);
        qcopy(&a, moby + 0x10);
        a.f[2] += 8.0f;
        clear_vec_d57f8(&b);
        b.f[2] = fast_arc_tan(a.f[0] - camera_position_or_stop_look.f[0], a.f[1] - camera_position_or_stop_look.f[1]);
        b.f[1] = -fast_arc_tan(vector_distance(&camera_position_or_stop_look, &a), a.f[2] - camera_position_or_stop_look.f[2]);
        FUN_L00_002ea9d8(&camera_position_or_stop_look);
        FUN_L00_002eaa30(&b);
        break;
    case TRANSPORT_POD_RAISE_FOR_DISEMBARKING:
        FUN_00213f38((float *)(moby + 0x18), &transport_pod_vars->speed, D_L02_001600EC_n[transport_pod_vars->node_a].pos[2],
                     D_L02_00161C34_gp * frame_time_sq, D_L02_00161C34_gp * frame_time_sq,
                     D_L02_00161C30_gp * D_0015ED6C);
        snap_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (D_L02_001600EC_n[transport_pod_vars->node_a].pos[2] <= ((struct Moby *)moby)->pos.z) {
            moby[0x20] = 0xB;
            FUN_0022da68_dc88(0, 0, moby);
        }
        break;
    case TRANSPORT_POD_POSITION_RAILS_FOR_DISEMBARKING:
        transport_pod_vars->rail_a_off = 2.0f;
        transport_pod_vars->rail_b_off = -4.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (absolute_float_dc88(*(float *)(transport_pod_vars->rail_a + 0x18) - ((struct Moby *)moby)->pos.z - 2.0f) < 0.01f &&
            absolute_float_dc88(*(float *)(transport_pod_vars->rail_b + 0x18) - ((struct Moby *)moby)->pos.z + 4.0f) < 0.01f) {
            {
                adjusted_help_ticks = scale_ticks(D_0015EEA4_gp) - usage_stats.stat[9].unk2 * 600;
            }
            help_request_threshold_ticks = scale_ticks(3600) * 60.0f;
            if (help_request_threshold_ticks < adjusted_help_ticks) {
                extern int D_0015ED84_dc88 __asm__("D_0015ED84") __attribute__((sda));
                if (!(D_00141BAC & (1 << D_0015ED84_dc88))) {
                    help_request(0x7DD, 0x48);
                }
            }
            moby[0x20] = 0xD;
            FUN_L00_002eac18(2);
            FUN_L02_0022b728(0, 1);
        }
        break;
    case TRANSPORT_POD_RAISE_TO_STOP_HEIGHT:
        ((struct Moby *)moby)->pos.z += D_0015ED6C * 4.0f;
        if (D_L02_001600EC_n[transport_pod_vars->node_a].pos[2] <= ((struct Moby *)moby)->pos.z) {
            moby[0x20] = 5;
        }
        break;
    case TRANSPORT_POD_WAIT_FOR_RIDER_TO_LEAVE:
        if ((unsigned char *)hero.ground_moby != (struct Moby *)moby) {
            moby[0x20] = 5;
        }
        break;
    case TRANSPORT_POD_CREATE_RAILS:
        transport_pod_vars->rail_a = create_moby(0x421);
        *(short *)(transport_pod_vars->rail_a + 0x32) = 0x40;
        transport_pod_vars->rail_a[0x31] = 1;
        *(u64 *)(transport_pod_vars->rail_a + 0x38) = *(u64 *)(moby + 0x38);
        *(unsigned short *)(transport_pod_vars->rail_a + 0x34) = *(unsigned short *)(moby + 0x34);
        qcopy_nc(transport_pod_vars->rail_a + 0x10, moby + 0x10);
        qcopy_nc(transport_pod_vars->rail_a + 0x40, moby + 0x40);
        transport_pod_vars->rail_b = create_moby(0x422);
        *(short *)(transport_pod_vars->rail_b + 0x32) = 0x40;
        transport_pod_vars->rail_b[0x31] = 1;
        *(u64 *)(transport_pod_vars->rail_b + 0x38) = *(u64 *)(moby + 0x38);
        *(unsigned short *)(transport_pod_vars->rail_b + 0x34) = *(unsigned short *)(moby + 0x34);
        qcopy_nc(transport_pod_vars->rail_b + 0x10, moby + 0x10);
        qcopy_nc(transport_pod_vars->rail_b + 0x40, moby + 0x40);
        moby[0x20] = 0xF;
        break;
    case TRANSPORT_POD_POSITION_RAILS_FROM_CONFIGURED_OFFSETS:
        transport_pod_vars->rail_a_off = D_L02_00161C20;
        transport_pod_vars->rail_b_off = D_L02_00161C24;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (D_L02_00161C40 != 0.0f) {
            moby[0x20] = 0x10;
        }
        break;
    case TRANSPORT_POD_CYCLE_RAILS_TO_SPLIT_HEIGHTS:
        transport_pod_vars->rail_a_off = 2.0f;
        transport_pod_vars->rail_b_off = -4.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (D_L02_00161C40 == 0.0f) {
            moby[0x20] = 0xF;
        } else if (absolute_float_dc88(*(float *)(transport_pod_vars->rail_a + 0x18) - ((struct Moby *)moby)->pos.z - 2.0f) < 0.01f &&
                   absolute_float_dc88(*(float *)(transport_pod_vars->rail_b + 0x18) - ((struct Moby *)moby)->pos.z + 4.0f) < 0.01f) {
            moby[0x20] = 0x11;
        }
        break;
    case TRANSPORT_POD_CYCLE_RAILS_TO_SHARED_LOWER_HEIGHT:
        transport_pod_vars->rail_a_off = -1.0f;
        transport_pod_vars->rail_b_off = -1.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (D_L02_00161C40 == 0.0f) {
            moby[0x20] = 0xF;
        } else if (absolute_float_dc88(*(float *)(transport_pod_vars->rail_a + 0x18) - ((struct Moby *)moby)->pos.z + 1.0f) < 0.01f &&
                   absolute_float_dc88(*(float *)(transport_pod_vars->rail_b + 0x18) - ((struct Moby *)moby)->pos.z + 1.0f) < 0.01f) {
            moby[0x20] = 0x10;
        }
        break;
    }
    fast_vec_add(&delta, &delta, moby + 0x10);
    if (fast_vec_length(&delta) > 4.0f) {
        clear_vec_d57f8(&delta);
    }
    update_entity_matrix(transport_pod_vars->riders, &delta, &previous_rotation, moby + 0x40);
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E03B8), where it is exact; names translated to the US level program. */

extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern float D_0015ED6C;
extern f32 D_L02_00161C30_gp __asm__("D_L02_00161C30") __attribute__((sda));
extern f32 D_L02_00161C34_gp __asm__("D_L02_00161C34") __attribute__((sda));

extern short D_L02_00161C38 __attribute__((sda));
extern short D_L02_00161C3C __attribute__((sda));
void advance_transport_pod_rails_to_offsets(struct Moby *moby) {
    char *transport_pod_vars = (char *)moby->pvars;
    advance_accelerated_scalar(
        (float *)(*(char **)(transport_pod_vars + 0x84) + 0x18), (float *)(transport_pod_vars + 0x8C),
        moby->pos.z + *(float *)(transport_pod_vars + 0x94), *(float *)&D_L02_00161C34_gp * frame_time_sq,
        *(float *)&D_L02_00161C38 * frame_time_sq, *(float *)&D_L02_00161C30_gp * D_0015ED6C);
    advance_accelerated_scalar(
        (float *)(*(char **)(transport_pod_vars + 0x88) + 0x18), (float *)(transport_pod_vars + 0x90),
        moby->pos.z + *(float *)(transport_pod_vars + 0x98), *(float *)&D_L02_00161C34_gp * frame_time_sq,
        *(float *)&D_L02_00161C38 * frame_time_sq, *(float *)&D_L02_00161C30_gp * D_0015ED6C);
    {
        float r0 =
            fast_add_rotations(moby->rot.z,
                               (*(float *)(*(char **)(transport_pod_vars + 0x84) + 0x18) - moby->pos.z) *
                                   *(float *)&D_L02_00161C3C * DEG_TO_RAD);
        float k = *(float *)&D_L02_00161C3C;
        *(float *)(*(char **)(transport_pod_vars + 0x84) + 0x48) = r0;
        *(float *)(*(char **)(transport_pod_vars + 0x88) + 0x48) = fast_add_rotations(
            moby->rot.z,
            (*(float *)(*(char **)(transport_pod_vars + 0x88) + 0x18) - moby->pos.z) * k * DEG_TO_RAD);
    }
    FUN_L00_00250df8(*(void **)(transport_pod_vars + 0x84));
    FUN_L00_00250df8(*(void **)(transport_pod_vars + 0x88));
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E04E8), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");

void snap_transport_pod_rails_to_offsets(struct Moby *moby) __asm__("FUN_L02_002df0b0");

void snap_transport_pod_rails_to_offsets(struct Moby *moby) {
    char *transport_pod_vars = (char *)moby->pvars;
    char *rail_a = *(char **)(transport_pod_vars + 0x84);
    float k, k2;
    float rail_a_rotation_z;
    qcopy(rail_a + 0x10, &moby->pos);
    qcopy(*(char **)(transport_pod_vars + 0x88) + 0x10, &moby->pos);
    k = *(float *)&D_L02_00161C3C;
    *(float *)(rail_a + 0x18) = moby->pos.z + *(float *)(transport_pod_vars + 0x94);
    *(int *)(transport_pod_vars + 0x8C) = 0;
    *(float *)(*(char **)(transport_pod_vars + 0x88) + 0x18) = moby->pos.z + *(float *)(transport_pod_vars + 0x98);
    *(int *)(transport_pod_vars + 0x90) = 0;
    rail_a_rotation_z = fast_add_rotations(moby->rot.z,
                     (*(float *)(*(char **)(transport_pod_vars + 0x84) + 0x18) - moby->pos.z) * k *
                         DEG_TO_RAD);
    k2 = *(float *)&D_L02_00161C3C;
    *(float *)(*(char **)(transport_pod_vars + 0x84) + 0x48) = rail_a_rotation_z;
    *(float *)(*(char **)(transport_pod_vars + 0x88) + 0x48) = fast_add_rotations(
        moby->rot.z,
        (*(float *)(*(char **)(transport_pod_vars + 0x88) + 0x18) - moby->pos.z) * k2 * DEG_TO_RAD);
    FUN_L00_00250df8(*(char **)(transport_pod_vars + 0x84));
    FUN_L00_00250df8(*(char **)(transport_pod_vars + 0x88));
}
/* State machine for a moby that picks a new animation each time the previous one finishes. */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E05E0), where it is exact; names translated to the US level program. */

extern char D_0013E550[];
extern int FUN_0022da68(int, int, int);
extern int random_integer_below(int) __asm__("FUN_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
void blend_moby_animation(MobyAnim *moby, int new_seq, int frame_index, int blend_frames) __asm__("FUN_00212f90");

extern short D_L02_00161C68 __attribute__((sda));
void crane_update(struct Moby *moby) __asm__("FUN_L02_002df1a8");
void crane_update(struct Moby *moby) {
    enum {
        CRANE_ANIM_RESTART_WAIT = 0,
        CRANE_ANIM_TRANSITION_TO_RANDOM_WAIT = 1,
        CRANE_ANIM_WAIT_FOR_RANDOM_TRANSITION = 2,
        CRANE_ANIM_TRANSITION_TO_WAIT_FOR_TRANSITION_TO_RANDOM_WAIT = 3,
        CRANE_ANIM_WAIT_FOR_TRANSITION_TO_RANDOM_WAIT = 4,
        CRANE_ANIM_TRANSITION_BACK_TO_RANDOM_WAIT = 5,
        CRANE_ANIM_TRANSITION_TO_RESTART_WAIT = 7
    };
    int *crane_vars = (int *)moby->pvars;
    moby->anim_speed = *(float *)&D_L02_00161C68;
    switch (moby->prev_seq) {
    case CRANE_ANIM_TRANSITION_TO_RANDOM_WAIT:
    case CRANE_ANIM_TRANSITION_TO_WAIT_FOR_TRANSITION_TO_RANDOM_WAIT:
    case CRANE_ANIM_TRANSITION_BACK_TO_RANDOM_WAIT:
    case CRANE_ANIM_TRANSITION_TO_RESTART_WAIT:
        if (moby->anim_flags & 2) {
            int voice_slot_index;
            moby->anim_time = 0;
            if (moby->prev_seq != moby->command) {
                blend_moby_animation(moby, moby->command, 0, 0);
            }
            voice_slot_index = crane_vars[0];
            if (voice_slot_index != -1) {
                char *voice_slot = D_0013E550 + voice_slot_index * 0x70;
                if (*(unsigned char **)(voice_slot + 0x88) == moby && ((unsigned char *)voice_slot)[0x74] != 0) {
                    release_voice_slot(voice_slot_index);
                }
            }
            crane_vars[0] = -1;
            FUN_0022da68(1, 0, (int)moby);
            moby->command = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(180.0f, 240.0f)));
        }
        break;
    case CRANE_ANIM_RESTART_WAIT:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
            moby->command = 2;
            crane_vars[0] = FUN_0022da68(0, 4, (int)moby);
        }
        break;
    case CRANE_ANIM_WAIT_FOR_RANDOM_TRANSITION:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (random_integer_below(0xFF) & 1) {
                if (moby->prev_seq != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
                moby->command = 4;
            } else {
                if (moby->prev_seq != 7) {
                    blend_moby_animation(moby, 7, 0, scale_game_frames(10));
                }
                moby->command = 0;
            }
            crane_vars[0] = FUN_0022da68(0, 4, (int)moby);
        }
        break;
    case CRANE_ANIM_WAIT_FOR_TRANSITION_TO_RANDOM_WAIT:
        if (FUN_L00_001fefc8(&moby->command)) {
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
            moby->command = 2;
            crane_vars[0] = FUN_0022da68(0, 4, (int)moby);
        }
        break;
    }
}

/* Pvars of the l02 swivel turret: turns to a random heading, then fires one shot. */
typedef struct {
    f32 yaw_vel;          /* 0x00 */
    f32 target_yaw;       /* 0x04 */
    s16 wait_timer;       /* 0x08 */
    u8 padA[2];
    s16 voice;            /* 0x0C: turning loop sound, -1 for none */
    u8 padE[2];
    f32 recoil;           /* 0x10 */
} SwivelTurretVars;

typedef struct {
    u8 pad0[0x74];
    u8 active;
    u8 pad75[0x13];
    struct Moby *owner;
} TurretVoiceSlot;

extern float D_0015ED6C __attribute__((section(".sdata")));
extern f32 D_L02_00161C6C __attribute__((sda));
extern f32 D_L02_00161C70 __attribute__((sda));
extern f32 D_L02_00161C74 __attribute__((sda));
s32 D_L02_00161C78 = 0x800000ff;
s32 D_L02_00161C7C = 0x80329600;
extern f32 D_L02_00161C84 __attribute__((sda));
void play_rocket_launcher_delayed_shot_sounds(struct Moby *moby) __asm__("FUN_L02_002df730");

extern void play_rocket_launcher_delayed_shot_sounds(struct Moby *moby) __asm__("FUN_L02_002df730");
extern char *spawn_projectile_moby(char *src, void *pos, float *dir) __asm__("FUN_L02_002ec228");
extern s32 turret_count_down(s16 *) __asm__("FUN_001f9770");
extern f32 turret_rand_range(f32, f32) __asm__("FUN_002132a8");
extern f32 turret_scale_time(f32) __asm__("FUN_001f96b0");
extern s32 turret_float_to_int(f32) __asm__("FUN_001fa6d0");
extern s32 turret_play_sound(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern s32 turret_sound_playing(struct Moby *, s32) __asm__("FUN_L00_0028d8c0");
extern void turret_release_voice(s32) __asm__("FUN_0022d798");
extern f32 turret_approach_angle(f32 *angle, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025be00");
extern f32 turret_angle_diff(f32, f32) __asm__("FUN_001fa688");
extern void turret_blend_anim(struct Moby *, s32, s32, s32) __asm__("FUN_00212f90");
extern void turret_joint_pos(struct Moby *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void turret_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void turret_vec_set_len(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void turret_spawn_debris(struct Moby *, void *, void *, f32, f32, s32, s32, s32, f32, f32, f32, f32, s32,
                                f32, s32, s32, s32, s32) __asm__("FUN_L00_0025e450");
extern f32 turret_anim_progress(struct Moby *) __asm__("FUN_0020c9e0");
extern s32 turret_lerp_colour(f32, s32, s32) __asm__("FUN_001fa6e0");

extern f32 frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
/* Swivel turret update: waits a random time, turns to a random heading with a loop sound, then fires one shot with debris and fades its colour while the recoil animation plays. */
void rocket_launcher_update(struct Moby *m) __asm__("FUN_L02_002df3d8");
void rocket_launcher_update(struct Moby *moby) {
    enum {
        ROCKET_LAUNCHER_WAIT_BETWEEN_SHOTS = 0,
        ROCKET_LAUNCHER_TURN_AND_FIRE = 1,
        ROCKET_LAUNCHER_WAIT_FOR_POST_SHOT_ANIMATION = 2
    };
    SwivelTurretVars *rocket_launcher_vars = (SwivelTurretVars *)moby->pvars;
    Vec4f projectile_velocity, muzzle;
    f32 glow_blend_fraction;

    play_rocket_launcher_delayed_shot_sounds(moby);
    switch (moby->state) {
    case ROCKET_LAUNCHER_WAIT_BETWEEN_SHOTS:
        if (turret_count_down(&rocket_launcher_vars->wait_timer)) {
            rocket_launcher_vars->target_yaw = turret_rand_range(0.0f, 3.14159f);
            moby->state = 1;
            turret_play_sound(4, 0, moby);
        }
        break;
    case ROCKET_LAUNCHER_TURN_AND_FIRE:
        turret_approach_angle(&moby->rot.z, rocket_launcher_vars->target_yaw, &rocket_launcher_vars->yaw_vel,
                              D_L02_00161C70 * DEG_TO_RAD * frame_time_sq_macro,
                              D_L02_00161C70 * DEG_TO_RAD * frame_time_sq_macro,
                              D_L02_00161C74 * DEG_TO_RAD * D_0015ED6C);
        if (!turret_sound_playing(moby, rocket_launcher_vars->voice))
            rocket_launcher_vars->voice = turret_play_sound(2, 4, moby);
        if (turret_angle_diff(moby->rot.z, rocket_launcher_vars->target_yaw) < DEG_TO_RAD && rocket_launcher_vars->yaw_vel == 0.0f) {
            s32 turning_voice_index = rocket_launcher_vars->voice;
            if (turning_voice_index != -1) {
                TurretVoiceSlot *voice_slot = (TurretVoiceSlot *)(D_0013E550 + turning_voice_index * 0x70);
                if (voice_slot->owner == moby && voice_slot->active)
                    turret_release_voice(turning_voice_index);
            }
            rocket_launcher_vars->voice = -1;
            turret_play_sound(3, 0, moby);
            rocket_launcher_vars->recoil = D_L02_00161C84;
            moby->state = 2;
            if (moby->prev_seq != 1)
                turret_blend_anim(moby, 1, 0, 1);
            turret_joint_pos(moby, 1, &muzzle);
            turret_joint_pos(moby, 0, &projectile_velocity);
            turret_vec_sub(&projectile_velocity, &muzzle, &projectile_velocity);
            turret_vec_set_len(&projectile_velocity, &projectile_velocity, D_L02_00161C6C * D_0015ED6C);
            turret_spawn_debris(moby, &projectile_velocity, &muzzle, 0.0f, 0.0f, 0, 6, 0x20, 7.0f, 4.0f, 1.0f, 3.0f, -1, 30.0f,
                                0, 1, -1, 0);
            spawn_projectile_moby((char *)moby, &muzzle, (float *)&projectile_velocity);
        }
        break;
    case ROCKET_LAUNCHER_WAIT_FOR_POST_SHOT_ANIMATION:
        moby->flags |= 0x10;
        glow_blend_fraction = turret_anim_progress(moby) / 10.0f;
        if (glow_blend_fraction > 1.0f)
            glow_blend_fraction = 1.0f;
        moby->glow_color = turret_lerp_colour(glow_blend_fraction, D_L02_00161C78, D_L02_00161C7C);
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 0)
                turret_blend_anim(moby, 0, 0, 1);
            moby->state = 0;
            rocket_launcher_vars->wait_timer = turret_float_to_int(turret_scale_time(turret_rand_range(60.0f, 180.0f)));
        }
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E0B68), where it is exact; names translated to the US level program. */

extern char D_L02_001673C0[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern float D_0015ED6C;
extern int D_001413D4 __attribute__((section(".data")));
extern int engine_voice_set_pitch(int, int) __asm__("FUN_L00_0028df38");
extern int FUN_L02_0028d618(int, int);
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern short D_L02_00161C80 __attribute__((sda));
extern short D_L02_00161C88 __attribute__((sda));
extern short D_L02_00161C8C __attribute__((sda));
extern short D_L02_00161C90 __attribute__((sda));
extern short D_L02_00161C94 __attribute__((sda));
s32 allocate_voice_for_target_entry(s32 entry_index, s32 flags,
                                    void *target) __asm__("FUN_0022da68");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void play_rocket_launcher_delayed_shot_sounds(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float distance;
    int sound_a, sound_b, gain_a, gain_b;
    if (*(float *)(data + 0x10) >= 0.0f) {
        distance = distance_xyz(D_L02_001673C0, &moby->pos);
        *(float *)(data + 0x10) += *(float *)&D_L02_00161C80 * D_0015ED6C;
        if (distance < *(float *)(data + 0x10) && D_001413D4 != 0x72) {
            sound_a = allocate_voice_for_target_entry(0, 0x10, (int)moby);
            sound_b = allocate_voice_for_target_entry(1, 0x10, (int)moby);
            gain_a = func_001FA898_r(
                (1.0f - (distance - *(float *)&D_L02_00161C88) /
                            (*(float *)&D_L02_00161C8C - *(float *)&D_L02_00161C88)) *
                1024.0f);
            gain_b = func_001FA898_r(((distance - *(float *)&D_L02_00161C90) /
                                      (*(float *)&D_L02_00161C94 - *(float *)&D_L02_00161C90)) *
                                     1024.0f);
            if (gain_a > 1024)
                gain_a = 1024;
            else if (gain_a < 0)
                gain_a = 0;
            if (gain_b > 1024)
                gain_b = 1024;
            else if (gain_b < 0)
                gain_b = 0;
            FUN_L02_0028d618(sound_a, gain_a);
            FUN_L02_0028d618(sound_b, gain_b);
            *(float *)(data + 0x10) = -1.0f;
        }
    }
}
#include "rnc/gameplay/entities/moby.h"

/* Pvars of a moby that turns from its spawn rotation to a goal rotation. */
typedef struct {
    Vec4 start;         /* 0x00: spawn rotation, then its quaternion */
    Vec4 goal;          /* 0x10: goal angles in degrees, then their quaternion */
    f32 speed;          /* 0x20 */
    f32 t;              /* 0x24: 0..1 along the turn */
} TurnerVars;

extern s32 level_game_mode __asm__("D_L02_0015F5C4");
extern s32 D_0015ED84;
extern u8 D_0014C050[][16];
extern void FUN_L00_001fff28(void *, int, float);
extern void multiply_quaternions(void *, void *, void *) __asm__("FUN_001fa3c0");
extern float approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern void quaternion_nlerp(void *, void *, void *, float) __asm__("FUN_001fa400");
extern void quaternion_rotation_basis(void *, void *) __asm__("FUN_001fa480");

/* Turns the moby from its spawn rotation to the goal angles once it is triggered (unkBC). */
void rocket_door1_update(struct Moby *m) __asm__("FUN_L02_002df948");
void rocket_door1_update(struct Moby *moby) {
    enum {
        ROCKET_DOOR1_INITIALIZE_ROTATION = 0,
        ROCKET_DOOR1_WAIT_FOR_COMMAND = 1,
        ROCKET_DOOR1_TURN_TO_GOAL_ROTATION = 2
    };
    Vec4 qx;
    Vec4 qy;
    Vec4 qz;
    Vec4 qxy;
    Vec4 rxy;
    TurnerVars *door_vars = (TurnerVars *)moby->pvars;

    if (level_game_mode == 2 && moby->command == 0) {
        moby->visible = 0;
        moby->flags |= 1;
    } else {
        moby->visible = 1;
        moby->flags &= ~1;
    }
    switch (moby->state) {
    case ROCKET_DOOR1_INITIALIZE_ROTATION:
        qcopy(&door_vars->start, &moby->rot);
        door_vars->start.f[3] = 0.0f;
        if (D_0014C050[D_0015ED84][moby->mission] == 0xFF) {
            moby->rot.x = door_vars->goal.f[0] * 0.017453292f;
            moby->rot.y = door_vars->goal.f[1] * 0.017453292f;
            moby->rot.z = door_vars->goal.f[2] * 0.017453292f;
            moby->state = 3;
        } else {
            moby->state = 1;
            moby->flags |= 0x100;
            FUN_L00_001fff28(&qx, 0, door_vars->goal.f[0] * 0.017453292f);
            FUN_L00_001fff28(&qy, 1, door_vars->goal.f[1] * 0.017453292f);
            FUN_L00_001fff28(&qz, 2, door_vars->goal.f[2] * 0.017453292f);
            multiply_quaternions(&qxy, &qx, &qy);
            multiply_quaternions(&door_vars->goal, &qxy, &qz);
            FUN_L00_001fff28(&qx, 0, moby->rot.x);
            FUN_L00_001fff28(&qy, 1, moby->rot.y);
            FUN_L00_001fff28(&qz, 2, moby->rot.z);
            multiply_quaternions(&rxy, &qx, &qy);
            multiply_quaternions(&door_vars->start, &rxy, &qz);
        }
        break;
    case ROCKET_DOOR1_WAIT_FOR_COMMAND:
        if (moby->command != 0) {
            moby->state = 2;
            door_vars->speed = 0.0f;
            door_vars->t = 0.0f;
        }
        break;
    case ROCKET_DOOR1_TURN_TO_GOAL_ROTATION:
        approach_angle(&door_vars->t, &door_vars->speed, 1.0f, frame_time_sq_macro * 0.5f, frame_time_sq_macro * 0.5f, D_0015ED6C);
        quaternion_nlerp(&qx, &door_vars->start, &door_vars->goal, door_vars->t);
        quaternion_rotation_basis(&qx, &moby->rot_row0);
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E1400), where it is exact; names translated to the US level program. */

extern char D_L02_0015FFD8_c2 __asm__("D_L02_0015FFD8");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern void FUN_L00_0025f730(void *a, float x);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");

void tresspasser_door1_update(struct Moby *moby) __asm__("FUN_L02_002dffc8");
void tresspasser_door1_update(struct Moby *moby) {
    enum {
        TRESSPASSER_DOOR1_CAPTURE_START_POSITION = 0,
        TRESSPASSER_DOOR1_WAIT_FOR_LINKED_MOBY = 1,
        TRESSPASSER_DOOR1_SLIDE_TO_OFFSET_POSITION = 2
    };
    char *door_vars = (char *)moby->pvars;
    float destination_position[4];
    float movement_step[4];
    switch (moby->state) {
    case TRESSPASSER_DOOR1_CAPTURE_START_POSITION:
        qcopy(door_vars, &moby->pos);
        moby->state = 1;
        break;
    case TRESSPASSER_DOOR1_WAIT_FOR_LINKED_MOBY:
        if (*(int *)(door_vars + 0x10) >= 0) {
            unsigned char *o =
                (unsigned char *)((*(int *)(door_vars + 0x10) << 8) + *(int *)&D_L02_0015FFD8_c2);
            short c = *(short *)(o + 0xA6);
            if (c == 0x267 || c == 0x23F) {
                if (o[0x20] == 4) {
                    moby->state = 2;
                    FUN_0022da68(0, 0, (int)moby);
                }
            }
        }
        break;
    case TRESSPASSER_DOOR1_SLIDE_TO_OFFSET_POSITION:
        normalize_vector_xyz(destination_position, &moby->rot_row1, 2.0f);
        add_vector_xyz(destination_position, door_vars, destination_position);
        subtract_vector_xyz(movement_step, destination_position, &moby->pos);
        FUN_L00_0025f730(movement_step, D_0015ED6C + D_0015ED6C);
        add_vector_xyz(&moby->pos, &moby->pos, movement_step);
        if (vector_length_xyz(movement_step) == 0.0f) {
            FUN_0022da68(1, 0, (int)moby);
            moby->state = 3;
        }
        break;
    }
}
/* moves a moby through three states: latch position, wait for a peer to reach state 4, then slide toward it */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E1570), where it is exact; names translated to the US level program. */

extern char D_L02_0015FFD8_c3 __asm__("D_L02_0015FFD8");

void tresspasser_door2_update(struct Moby *moby) __asm__("FUN_L02_002e0138");
void tresspasser_door2_update(struct Moby *moby) {
    enum {
        TRESSPASSER_DOOR2_CAPTURE_START_POSITION = 0,
        TRESSPASSER_DOOR2_WAIT_FOR_LINKED_MOBY = 1,
        TRESSPASSER_DOOR2_SLIDE_TO_OFFSET_POSITION = 2
    };
    char *data = (char *)moby->pvars;
    switch (moby->state) {
    case TRESSPASSER_DOOR2_CAPTURE_START_POSITION:
        qcopy(data, &moby->pos);
        moby->state = 1;
        break;
    case TRESSPASSER_DOOR2_WAIT_FOR_LINKED_MOBY: {
        int id = *(int *)(data + 0x10);
        if (id >= 0) {
            char *linked_moby = (char *)((id << 8) + *(int *)&D_L02_0015FFD8_c3);
            int linked_moby_class = *(short *)(linked_moby + 0xA6);
            if (linked_moby_class == 0x267 || linked_moby_class == 0x23F) {
                if (((unsigned char *)linked_moby)[0x20] == 4)
                    moby->state = 2;
            }
        }
        break;
    }
    case TRESSPASSER_DOOR2_SLIDE_TO_OFFSET_POSITION: {
        float destination_position[4];
        float movement_step[4];
        char *pos = &moby->pos;
        normalize_vector_xyz(destination_position, &moby->rot_row1, -2.0f);
        add_vector_xyz(destination_position, data, destination_position);
        subtract_vector_xyz(movement_step, destination_position, pos);
        FUN_L00_0025f730(movement_step, D_0015ED6C + D_0015ED6C);
        add_vector_xyz(pos, pos, movement_step);
        if (vector_length_xyz(movement_step) == 0.0f)
            moby->state = 3;
        break;
    }
    }
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E16B8), where it is exact; names translated to the US level program. */

extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern void clear_vector(void *) __asm__("func_001F99F8");
void blend_moby_animation_c(void *moby, int new_seq, int frame_index, int blend_frames) __asm__("FUN_00212f90");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

int D_L02_00161C9C = 0x6f707070;
int D_L02_00161CA0 = 0x707070;
void sandshark_egg_update(struct Moby *moby) __asm__("FUN_L02_002e0280");
void sandshark_egg_update(struct Moby *moby) {
    enum {
        SANDSHARK_EGG_FALL_UNTIL_LANDING = 1,
        SANDSHARK_EGG_HANDOFF_TO_LINKED_MOBY = 2
    };
    float u[4];
    float v[4];
    char *egg_vars = (char *)moby->pvars;
    int i;
    float *vp;
    int effect_frame_count;
    int random_effect_value;
    char *linked_moby;
    switch (moby->state) {
    case SANDSHARK_EGG_FALL_UNTIL_LANDING:
        clear_vector(u);
        if (mine_fall(moby, egg_vars) & 1) {
            moby->state = 2;
        }
        vp = v;
        i = 2;
        do {
            qcopy(vp, &moby->pos);
            v[2] += random_float_between_alt(0.0f, 0.5f);
            v[0] += random_float_between_alt(-0.15f, 0.15f);
            v[1] += random_float_between_alt(-0.15f, 0.15f);
            effect_frame_count = scale_game_frames(random_int_between(0x14, 0x28));
            random_effect_value = random_int_between(0x14, 0x28);
            FUN_L00_00269958(vp, u, D_L02_00161C9C, D_L02_00161CA0, effect_frame_count, 0x28,
                             random_effect_value, 1);
            i--;
        } while (i >= 0);
        break;
    case SANDSHARK_EGG_HANDOFF_TO_LINKED_MOBY:
        linked_moby = *(char **)(egg_vars + 0x60);
        qcopy(linked_moby + 0x10, &moby->pos);
        linked_moby[0x20] = 0x11;
        blend_moby_animation_c(*(char **)(egg_vars + 0x60), 5, 0, 1);
        *(int *)(*(char **)(egg_vars + 0x60) + 0x94) =
            *(int *)(*(char **)(*(char **)(egg_vars + 0x60) + 0x24) + 0x10);
        *(float *)(*(char **)(egg_vars + 0x60) + 0x58) = 1.0f;
        FUN_L00_00250df8(*(char **)(egg_vars + 0x60));
        mark_moby_for_removal(moby);
        break;
    }
}
typedef struct {
    u32 c[3];
} Colors0720;

extern unsigned char menu_item_enabled_9 __asm__("D_0013D39D") __attribute__((section(".data")));
int D_L02_00161CC4 = 0x5f243632;
int D_L02_00161CC8 = 0x61311;
extern Colors0720 D_L02_00161CD0;
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void FUN_L00_0026ced0(void *, void *, int, int, float, int);
/* Spark emitter; the size float is declared before the int lifetime (same registers) so the calls
 * run in retail order: colour index, size, lifetime. */
extern void spawn_debris_chunk(void *, void *, u32, f32, s32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");
extern int FUN_0022da68(int, int, int);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern int scale_ticks(int) __asm__("FUN_001f96f8");

/* One-shot burst: when hit, flags D_0013D39D, sprays 200 dust puffs and 50 sparks, then removes itself. */
void rocks_update(struct Moby *moby) __asm__("FUN_L02_002e0720");
void rocks_update(struct Moby *moby) {
    enum {
        ROCKS_INITIALIZE = 0,
        ROCKS_WAIT_FOR_HIT = 1
    };
    char *hit_record;
    int i;
    int j;

    hit_record = find_moby_hit(moby, 0x800000, 0);
    switch (moby->state) {
    case ROCKS_INITIALIZE:
        if (menu_item_enabled_9) {
            mark_moby_for_removal(moby);
        } else {
            moby->state = 1;
        }
        break;
    case ROCKS_WAIT_FOR_HIT:
        if (hit_record) {
            menu_item_enabled_9 = 1;
            FUN_0022da68(0, 0, (int)moby);
            /* Statement-expression bodies: their temporaries share stack slots with the second loop's, as in retail. */
            for (i = 0; i < 200; i++) ({
                OvlVec4 dir;
                OvlVec4 pos;
                OvlVec4 tmp;
                pos.q = 0;
                pos.f[0] = random_float_between_alt(-1.0f, 1.0f);
                pos.f[1] = random_float_between_alt(-1.0f, 1.0f);
                pos.f[2] = random_float_between_alt(-1.0f, 1.0f);
                dir.q = pos.q;
                tmp.q = 0;
                tmp.f[0] = random_float_between_alt(-2.0f, 2.0f) * fast_cos_d57f8(moby->rot.z);
                tmp.f[1] = random_float_between_alt(-1.0f, 1.0f) * fast_sin_d57f8(moby->rot.z);
                tmp.f[2] = random_float_between_alt(0.0f, 4.0f);
                pos.q = tmp.q;
                fast_vec_add(&pos, &pos, &moby->pos);
                fast_vec_normalize(&dir, &dir, random_float_between_alt(1.5f, 3.5f) * D_0015ED6C);
                FUN_L00_0026ced0(&pos, &dir, D_L02_00161CC4, D_L02_00161CC8,
                                 random_float_between_alt(1.5f, 3.5f) * 210000.0f,
                                 scale_ticks(random_int_between(0x78, 0xF0)));
            });
            for (j = 0; j < 50; j++) {
                Colors0720 colors;
                OvlVec4 dir;
                OvlVec4 pos;
                OvlVec4 tmp;
                colors = D_L02_00161CD0;
                pos.q = 0;
                pos.f[0] = random_float_between_alt(-1.0f, 1.0f);
                pos.f[1] = random_float_between_alt(-1.0f, 1.0f);
                pos.f[2] = random_float_between_alt(-1.0f, 1.0f);
                dir.q = pos.q;
                tmp.q = 0;
                tmp.f[0] = random_float_between_alt(-2.0f, 2.0f) * fast_cos_d57f8(moby->rot.z);
                tmp.f[1] = random_float_between_alt(-1.0f, 1.0f) * fast_sin_d57f8(moby->rot.z);
                tmp.f[2] = random_float_between_alt(1.0f, 4.0f);
                pos.q = tmp.q;
                fast_vec_add(&pos, &pos, &moby->pos);
                fast_vec_normalize(&dir, &dir, random_float_between_alt(1.0f, 5.0f) * D_0015ED6C);
                spawn_debris_chunk(&pos, &dir, colors.c[random_integer_below(3)],
                                 random_float_between_alt(0.05f, 0.15f), random_int_between(0x3C, 0xB4),
                                 1.0f, 1.0f, 0.75f, 0);
            }
            mark_moby_for_removal(moby);
        }
        break;
    }
}
