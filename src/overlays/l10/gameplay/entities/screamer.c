/* Level 10 moby classes: screamer, toxic_crab, blarg_ship, nanotech_upgrade_vendor, gun, gate_1, gate_2. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/gameplay/state/item_state.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"

#include "rnc/math/vector.h"

#include "qcopy.h"

/* Update function for moby class 1196 (screamer) on level 10: follows its path, turns to its target, wanders. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E0630), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V_2E0630;

typedef struct { int count; char p4[0xC]; V_2E0630 nodes[1]; } Path_2E0630;

typedef struct D_2E0630 {
    char p0[0x20];
    float f20;
    short h24;
    char p26[2];
    unsigned char b28;
    unsigned char b29;
    char p2A[6];
    float f30;
    char p34[0xC];
    V_2E0630 step;
    char p50[0x10];
    char react[0xA0];
    struct Moby *leader;
    int f104;
    char p108[0x18];
    V_2E0630 vel;
    V_2E0630 home;
    V_2E0630 wander_goal;
    int path;
    char p154[4];
    int node_index;
    char p15C[4];
    int linked[4];
    float yaw_speed;
    int f174;
    char p178[0x24];
    int wander_timer;
    char p1A0[0xD0];
    char sound_state[4];
} D_2E0630;

extern char *D_L10_0015FFD0_g[] __asm__("D_L10_0015FFD0") __attribute__((section(".sdata")));
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float FUN_001f9e90(float, float);
extern float fast_difference_between_rotations_c(float, float) __asm__("func_001FA688");
extern float probe_ground_height_c(void *, int, float) __asm__("func_00213508");
extern int FUN_001f9b80_c() __asm__("FUN_001f9b80");
extern int FUN_L00_001f0d60();
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt_c(int *) __asm__("FUN_001f9740");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void add_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation_c2(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
float FUN_L00_0025be00(float *cur, float *vel, float target, float lim, float accel, float maxv);
int FUN_L00_0025c698_c(void *moby, void *motion) __asm__("FUN_L00_0025c698");
void FUN_L00_00257470_c(unsigned char *m, int flags, int arg) __asm__("FUN_L00_00257470");
void FUN_L00_00258278_c(unsigned char *m, float *ptr, float t, float b, float c, float limit) __asm__("FUN_L00_00258278");
void FUN_L00_00258ad0(int a, int b, float x, float y, int c, int d);
void FUN_L00_0025a120_c(char *o) __asm__("FUN_L00_0025a120");
void FUN_L00_0025bc00(float *p, void *o, float a, float lim);
void FUN_L00_0025f3e8_c(char *owner, char *pos, float scale, float flash) __asm__("FUN_L00_0025f3e8");
void FUN_L00_0025f730(void *a, float x);
void FUN_L00_00263ac8_c(float x, int a, int b, unsigned char *p) __asm__("FUN_L00_00263ac8");
void FUN_L10_002dfc38_c(struct Moby *moby) __asm__("FUN_L10_002dfc38");
void FUN_L10_002e0138_c2(struct Moby *m) __asm__("FUN_L10_002e0138");
extern void func_L10_002E0FF8_2E0630(void *) __asm__("FUN_L10_002dfc38");
extern void func_L00_00264B40_2E0630(float, void *, int, void *) __asm__("FUN_L00_00263ac8");
extern float func_001F9D10_2E0630(void *, void *) __asm__("FUN_001f9b48");
extern void func_L00_0025B178_2E0630(void *) __asm__("FUN_L00_0025a120");
extern void func_0020D678_2E0630(void *) __asm__("FUN_0020c828");
extern int func_001F9850_2E0630(int) __asm__("FUN_001f96f8");
extern void func_001F9BC0_2E0630(void *) __asm__("FUN_001f99f8");
extern float func_00214D88_2E0630(float *, float, float *, float, float, float) __asm__("FUN_00213f38");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float func_L00_001FF860_2E0630(float, float) __asm__("FUN_001f9e90");
extern float func_L00_0025CE58_2E0630(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern float func_001FA850_2E0630(float, float) __asm__("FUN_001fa688");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void func_L00_002607A8_2E0630(void *, float) __asm__("FUN_L00_0025f730");
extern int func_L00_00259B08_2E0630(void *, void *, float, int, float, int) __asm__("FUN_L00_00258ad0");
extern void func_00213DE0_2E0630(void *, int, int, int) __asm__("FUN_00212f90");
extern float func_00214358_2E0630(void *, int, float) __asm__("FUN_00213508");
extern int func_001FA898_2E0630(float) __asm__("FUN_001fa6d0");
extern void func_L10_002E14F8_2E0630(void *) __asm__("FUN_L10_002e0138");
extern float func_L00_0025CC58_2E0630(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern void func_L00_002592B0_2E0630(void *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern float func_001F9F90_2E0630(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2E0630(float) __asm__("FUN_001f9de0");
extern float func_001F9D48_2E0630(void *, void *) __asm__("FUN_001f9b80");
extern int func_001F9908_2E0630(void *) __asm__("FUN_001f9740");
extern float func_002140F8_2E0630(float, float) __asm__("FUN_002132a8");
extern float func_00214158_2E0630(void) __asm__("FUN_00213308");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_L00_001F10E0_2E0630(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int func_L00_0025D6F0_2E0630(void *, void *) __asm__("FUN_L00_0025c698");
extern void func_L00_002584A8_2E0630(void *, int, int) __asm__("FUN_L00_00257470");
extern void func_L00_00260460_2E0630(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern V_2E0630 D_L10_001672C0_2E0630 __asm__("D_L10_00167240");
extern Path_2E0630 *D_L10_001B0C30_2E0630[] __asm__("D_L10_001B0930");
extern V_2E0630 D_L10_00174360_2E0630 __asm__("D_L10_001742E0");

void FUN_L10_002df270(Moby *moby) {
    float waypoint[6];
    D_2E0630 *data = (D_2E0630 *)moby->pvars;
    Moby *leader;

    func_L10_002E0FF8_2E0630(moby);
    leader = data->leader;
    func_L00_00264B40_2E0630(2.5f, moby, 0, data->sound_state);
    if (moby->visible != 0 && func_001F9D10_2E0630(&moby->pos, &D_L10_001672C0_2E0630) < 29.0f) {
        func_L00_0025B178_2E0630(moby);
        moby->shadow_range = 0x17;
    }
    moby->anim_speed = 1.0f;
    switch (moby->state) {
    case 0:
        data->f30 = 0.5f;
        data->f20 = 1.0f;
        data->h24 = 1;
        data->b28 = 0;
        data->b29 = 1;
        moby->flags |= 0x1000;
        qcopy(&data->home, &moby->pos);
        qcopy(&data->wander_goal, &data->home);
        if (data->f174 != 1) {
            func_0020D678_2E0630(moby);
        } else {
            moby->state = 1;
        }
        break;
    case 1:
        if (data->f104 == 2 && data->f174 != 0) break;
        moby->state = 2;
        if (moby->prev_seq != 1) func_00213DE0_2E0630(moby, 1, 0, func_001F9850_2E0630(10));
        break;
    case 2:
        if ((moby->anim_flags & 2) == 0) break;
        moby->state = 4;
        func_001F9BC0_2E0630(&data->vel);
        if (moby->prev_seq != 4) func_00213DE0_2E0630(moby, 4, 0, func_001F9850_2E0630(10));
        break;
    case 3:
        break;
    case 4:
        moby->anim_speed = 4.0f;
        if (func_00214D88_2E0630(&moby->pos.z, data->home.z + 2.0f, &data->vel.z,
                D_0015ED70 * 18.0f, D_0015ED70 * 18.0f, D_0015ED6C * 6.0f) == 0.0f) {
            moby->state = 5;
        }
        break;
    case 5: {
        float distance;
        moby->anim_speed = 4.0f;
        qcopy(waypoint, &D_L10_001B0C30_2E0630[data->path]->nodes[data->node_index]);
        waypoint[5] = fast_vec_length(&data->vel);
        distance = func_001F9D10_2E0630(waypoint, &moby->pos);
        *(int *)&waypoint[4] = 0;
        func_L00_0025CE58_2E0630(&moby->rot.z, func_L00_001FF860_2E0630(waypoint[0] - moby->pos.x, waypoint[1] - moby->pos.y), &data->yaw_speed,
            D_0015ED70 * 25.132742f, D_0015ED70 * 25.132742f, D_0015ED6C * 50.265484f);
        if (func_001FA850_2E0630(func_L00_001FF860_2E0630(waypoint[0] - moby->pos.x, waypoint[1] - moby->pos.y), moby->rot.z) < 0.78539819f) {
            func_00214D88_2E0630(&waypoint[4], distance, &waypoint[5], D_0015ED70 * 20.0f, D_0015ED70 * 20.0f, D_0015ED6C * 20.0f);
            fast_vec_sub(&data->vel, waypoint, &moby->pos);
            func_L00_002607A8_2E0630(&data->vel, waypoint[5]);
        } else {
            func_001F9BC0_2E0630(&data->vel);
        }
        func_L00_00259B08_2E0630(moby, &data->vel, 0.5f, 0x200, 0.0f, 0);
        if (!(distance < 1.0f)) break;
        {
            int node_index = data->node_index++;
            if (0.0f < D_L10_001B0C30_2E0630[data->path]->nodes[node_index].w) {
                moby->state = 6;
                if (moby->prev_seq != 3) func_00213DE0_2E0630(moby, 3, 0, func_001F9850_2E0630(10));
            } else {
                if (node_index + 1 != D_L10_001B0C30_2E0630[data->path]->count) break;
                moby->state = 10;
                if (moby->prev_seq != 4) func_00213DE0_2E0630(moby, 4, 0, func_001F9850_2E0630(10));
                func_00214358_2E0630(&moby->pos, 0, 0.5f);
                qcopy(&data->home, &D_L10_00174360_2E0630);
                qcopy(&data->wander_goal, &moby->pos);
            }
        }
        break;
    }
    case 6:
        func_L00_0025CE58_2E0630(&moby->rot.z, func_L00_001FF860_2E0630(leader->pos.x - moby->pos.x, leader->pos.y - moby->pos.y), &data->yaw_speed,
            D_0015ED70 * 25.132742f, D_0015ED70 * 25.132742f, D_0015ED6C * 50.265484f);
        if ((moby->anim_flags & 2) == 0) break;
        {
            char *linked = D_L10_0015FFD0_g[2] + (data->linked[func_001FA898_2E0630(D_L10_001B0C30_2E0630[data->path]->nodes[data->node_index - 1].w) - 1] << 8);
            if (linked != 0) func_L10_002E14F8_2E0630(linked);
        }
        if (data->node_index == D_L10_001B0C30_2E0630[data->path]->count) {
            moby->state = 10;
            if (moby->prev_seq != 4) func_00213DE0_2E0630(moby, 4, 0, func_001F9850_2E0630(10));
            func_00214358_2E0630(&moby->pos, 0, 0.5f);
            qcopy(&data->home, &D_L10_00174360_2E0630);
            qcopy(&data->wander_goal, &moby->pos);
        } else {
            moby->state = 5;
            if (moby->prev_seq != 4) func_00213DE0_2E0630(moby, 4, 0, func_001F9850_2E0630(10));
        }
        break;
    case 7:
        break;
    case 8: {
        float zero = 0.0f;
        float climb;
        func_L00_0025CC58_2E0630(&moby->rot.y, 0, zero, D_0015ED6C * 12.566371f);
        func_L00_002592B0_2E0630(moby, func_L00_001FF860_2E0630(data->home.x - moby->pos.x, data->home.y - moby->pos.y), &data->yaw_speed,
            0.02f, 0.3f, D_0015ED6C * 6.2831855f);
        data->vel.x = func_001F9F90_2E0630(moby->rot.z) * (D_0015ED6C * 6.0f);
        data->vel.y = func_001F9FA8_2E0630(moby->rot.z) * (D_0015ED6C * 6.0f);
        data->vel.z = zero;
        climb = data->home.z + 2.0f - moby->pos.z;
        if (D_0015ED6C * 6.0f < climb) {
            climb = D_0015ED6C * 6.0f;
        } else if (climb < D_0015ED6C * -6.0f) {
            climb = D_0015ED6C * -6.0f;
        }
        data->vel.z = climb;
        func_L00_00259B08_2E0630(moby, &data->vel, 0.5f, 0x200, 0.0f, 0);
        if (func_001F9D48_2E0630(&moby->pos, &data->home) < 2.0f) moby->state = 7;
        break;
    }
    case 9: {
        float radius;
        float angle;
        float zero;
        if (!func_001F9908_2E0630(&data->wander_timer)) break;
        zero = 0.0f;
        data->wander_timer = func_001F9850_2E0630(0x78);
        radius = func_002140F8_2E0630(zero, 3.0f);
        angle = func_00214158_2E0630();
        waypoint[0] = func_001F9F90_2E0630(angle) * radius;
        waypoint[1] = func_001F9FA8_2E0630(angle) * radius;
        waypoint[2] = zero;
        waypoint[2] = func_002140F8_2E0630(5.0f, 7.0f);
        fast_vec_add(&data->wander_goal, waypoint, &data->home);
        moby->state = 10;
        break;
    }
    case 10:
        func_L00_002592B0_2E0630(moby, func_L00_001FF860_2E0630(data->wander_goal.x - moby->pos.x, data->wander_goal.y - moby->pos.y), &data->yaw_speed,
            0.05f, 0.3f, 0.2f);
        fast_vec_sub(&data->step, &data->wander_goal, &moby->pos);
        func_L00_002607A8_2E0630(&data->step, D_0015ED6C * 4.0f);
        fast_vec_add(waypoint, &data->step, &moby->pos);
        if (func_L00_001F10E0_2E0630(0.4f, waypoint, 0, moby)) {
            func_001F9BC0_2E0630(&data->step);
            moby->state = 9;
        } else {
            qcopy(&moby->pos, waypoint);
        }
        if (fast_vec_length(&data->step) == 0.0f) moby->state = 9;
        break;
    case 11:
        if ((func_L00_0025D6F0_2E0630(moby, data->react) & 1) == 0) break;
        func_L00_002584A8_2E0630(moby, 0x200, -1);
        func_L00_00260460_2E0630(moby, &moby->pos, -1, 0.5f, 10.0f);
        func_0020D678_2E0630(moby);
        break;
    }
}
typedef struct Hit_2DFC38 { float x, y; char pad8[8]; float f10[4]; struct Moby *moby; char pad24[8]; float f2C; int f30; } Hit_2DFC38;
typedef struct MobyData_2DFC38 { char pad0[0x20]; float f20; char pad24[2]; short f26; char pad28[0x10]; int f38; char pad3C[0x24]; char f60[0x10]; float f70, f74, f78, f7C; char pad80[4]; int f84; char pad88[0x15]; unsigned char f9D; char pad9E[0x22]; char fC0[0x40]; int f100, f104; char pad108[8]; char f110[7]; unsigned char f117; char pad118[0x18]; char f130[0x44]; int f174; float f178; int f17C; float f180; int f184; float f188; int f18C; char pad190[4]; int f194; } MobyData_2DFC38;
typedef struct Anim_2DFC38 { char pad0[0x30]; float f30[4], f40[4]; char pad50[0x30]; } Anim_2DFC38;
typedef struct Path_2DFC38 { int count; char pad4[0xC]; float pts[4]; } Path_2DFC38;
extern Anim_2DFC38 *D_L10_001600EC_2DFC38 __asm__("D_L10_001600EC");
extern Path_2DFC38 *D_L10_001B0C30_2DFC38[] __asm__("D_L10_001B0930");
extern char D_0013F3D0[];
extern int D_001413D0[];
extern float D_0015ED6C, D_0015ED70;
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void func_L00_00261B00_2E2B80(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern int ahead_f(float, void *, void *) __asm__("FUN_L01_00274b78");
extern int activate_effect_l08(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern unsigned char D_0013D418 __attribute__((section(".data")));
typedef struct EventRecord_2DFC38 { unsigned char pad[0x10]; unsigned char flag; } EventRecord_2DFC38;
extern EventRecord_2DFC38 D_0013D408_2DFC38 __asm__("D_0013D408");
extern int D_00151F38 __attribute__((sda));
extern float random_float_between_2DFC38(float, float) __asm__("FUN_002132a8");
extern float scale_time_2DFC38(float) __asm__("FUN_001f96b0");
extern int truncate_float_2DFC38(float) __asm__("FUN_001fa6d0");
extern void tick_timer_2DFC38(void *) __asm__("FUN_001f9740");
extern int game_frames_2DFC38(int) __asm__("FUN_001f96f8");
extern int hit_search_2DFC38(void *, int, int) __asm__("FUN_L00_0025a420");
extern void hit_update_2DFC38(void *, void *, void *, int, void *, int, int, int) __asm__("FUN_00213928");
extern void play_sound_2DFC38(int, int) __asm__("FUN_L00_00263d40");
extern void allocate_voice_for_bank_entry_2DFC38(int, int, int) __asm__("FUN_0022db10");

void FUN_L10_002dfc38(struct Moby *moby) {
    MobyData_2DFC38 *data = (MobyData_2DFC38 *)moby->pvars;
    Hit_2DFC38 *r;
    float v[4];
    float w[4];
    int st;
    int r19;
    float ang;

    if (data->f38 != 0) {
        data->f184 = truncate_float_2DFC38(scale_time_2DFC38(random_float_between_2DFC38(180.0f, 240.0f)));
        data->f38 = 0;
    }
    tick_timer_2DFC38(&data->f184);
    if (moby->state != 11) {
        r = (Hit_2DFC38 *)hit_search_2DFC38(moby, 0x330000, 0);
        hit_update_2DFC38(moby, r, &data->f20, 5, &st, 0, 0, 4);
        if (r != 0) {
            if (data->f20 <= r->f2C) {
                if (D_00151F38 != 0 && D_0013D408_2DFC38.flag == 0) {
                    D_0013D408_2DFC38.flag = 1;
                    allocate_voice_for_bank_entry_2DFC38(1, 0, 0);
                    play_sound_2DFC38(0x53d6, -1);
                }
                data->f20 = 0.0f;
                moby->flags &= 0xEFFF;
                if (moby->prev_seq != 5) blend_moby_animation(moby, 5, 0, game_frames_2DFC38(10));
                moby->state = 11;
                data->f70 = D_0015ED70 * 26.0f;
                data->f9D = 3;
                data->f74 = 0.0005f;
                data->f84 = 9;
                data->f78 = D_0015ED6C * 7.0f;
                data->f7C = D_0015ED6C * 10.0f;
                if (r->f30 & 1) ang = angle_atan2(r->f10[0], r->f10[1]);
                else ang = angle_atan2(moby->pos.x - r->x, moby->pos.y - r->y);
                trooper_start_knockback(moby, data->f60, ang, 5, 1, 0);
                data->f117 = 0x78;
                trooper_anim_update(moby, data->f110);
                func_L00_00261B00_2E2B80(moby, 1, 3, 0, -1);
            } else {
                data->f20 = data->f20 - r->f2C;
                data->f117 = 0xfa;
                data->f26 = game_frames_2DFC38(60);
                trooper_anim_update(moby, data->f110);
            }
            moby->hit_slot = 0xff;
        }
    }
    trooper_anim_finish(moby, data->f110);
    if (data->f174 == 2) data->f180 = 42.0f;
    else if (data->f184 != 0) data->f180 = data->f178 + 6.0f;
    else if (moby->state == 1) data->f180 = data->f188;
    else data->f180 = data->f178;

    if (data->f174 == 2) {
        r19 = ahead_f(data->f180, moby, data->fC0);
        if (r19 != 2) {
            Anim_2DFC38 *bp = &D_L10_001600EC_2DFC38[data->f194];
            subtract_vector_xyz(v, D_0013F3D0, bp->f30);
            v[3] = 0.0f;
            transform_vector_by_basis(w, v, bp->f40);
            if (w[0] <= -1.0f || 0.0f <= w[0] || w[1] <= -1.0f || 1.0f <= w[1] || w[2] <= -1.0f || 1.0f <= w[2]) data->f104 = 2;
        }
    } else if (moby->state == 1 && data->f18C >= 0) {
        Path_2DFC38 *path = D_L10_001B0C30_2DFC38[data->f18C];
        r19 = activate_effect_l08(data->f180, moby, data->fC0, 0, 0, path->pts, path->count);
    } else if (moby->state != 1 && data->f17C >= 0) {
        Path_2DFC38 *path = D_L10_001B0C30_2DFC38[data->f17C];
        r19 = activate_effect_l08(data->f180, moby, data->fC0, 0, 0, path->pts, path->count);
    } else {
        r19 = ahead_f(data->f180, moby, data->fC0);
    }
    if (r19 != 2) {
        if (data->f180 < vector_distance(data->f130, data->fC0)) data->f104 = 2;
        else if (moby->visible == 0) data->f104 = 2;
    }
    if (data->f100 == 0) data->f100 = D_001413D0[0];
}
#define NOT_SDA

#define MACRO_ADDR

/* Sets every listed entity in state 1 to state 2. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E14F8), where it is exact; names translated to the US level program. */

typedef struct {
    char p0[0x10];
    float x;
    float y;
    char p1[8];
    unsigned char state;
    char p2[0xDF];
} Ent;

/* Retail reaches D_L10_0015FFD8 here with the lui/lw macro; FUN_L10_002e1d38 reaches it through $gp. An alias 8 bytes lower keeps the macro form under SN. */
extern Ent *D_L10_0015FFD0_e[] __asm__("D_L10_0015FFD0") __attribute__((section(".sdata")));
extern short *D_L10_001ABCC0[];

void FUN_L10_002e0138(struct Moby *m) {
    short *p;
    if (m->group != 0xFF) {
        p = D_L10_001ABCC0[m->group];
        if (p != 0) {
            do {
                Ent *base = D_L10_0015FFD0_e[2];
                Ent *e = (Ent *)((*(unsigned short *)p & 0x7FFF) * 256 + (int)base);
                if (e->state == 1)
                    e->state = 2;
            } while (*p++ >= 0);
        }
    }
}
/* Update function for moby class 1199 on level 10: rises from its spawn, wanders, chases and dives at its target, then dies. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E1568), where it is exact; names translated to the US level program. */

typedef struct Hero Hero_2e01a8;


typedef int OvlQuad_2e01a8 __attribute__((mode(TI)));


typedef struct Vars_2e01a8 {
    char p0[0x20];
    float f20;
    short h24;
    char p26[2];
    unsigned char b28;
    unsigned char b29;
    char p2A[6];
    float f30;
    char p34[0x24];
    unsigned char b58;
    char p59;
    unsigned char b5A;
    char p5B[5];
    char a60[0x80];
    V_2E0630 vE0;
    char pF0[0x10];
    Moby *target;
    int f104;
    char p108[0x18];
    V_2E0630 velocity;
    V_2E0630 home;
    V_2E0630 goal;
    char p150[0x20];
    float turn_speed;
    int spawn_kind;
    char p178[0x18];
    float side_angle;
    int f194;
    int wait;
    float bob_angle;
    float height_offset;
    int height_timer;
    char p1A8[0xC8];
    char a270[4];
} Vars_2e01a8;

typedef struct Anim_2E1568 {
    char pad0[0x38];
    float f38;
    char pad3C[0x44];
} Anim_2E1568;

extern OvlQuad_2e01a8 D_L10_00167240_2e01a8 __asm__("D_L10_00167240");
extern f32 fast_add_rotations_2e01a8(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos_2e01a8(f32) __asm__("func_001F9DC8");
extern f32 random_float_between_2e01a8(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_L10_00161DFC __attribute__((sda));
extern float D_L10_00161E04 __attribute__((sda));
extern float D_L10_00161E08 __attribute__((sda));
extern float D_L10_00161E0C __attribute__((sda));
extern float D_L10_00161E10 __attribute__((sda));
extern float D_L10_00161E14 __attribute__((sda));
extern float D_L10_00161E18 __attribute__((sda));
extern float D_L10_00161E1C __attribute__((sda));
extern float D_L10_00161E20 __attribute__((sda));
extern float FUN_001f96b0_2e01a8(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b80_2e01a8(void *, void *) __asm__("FUN_001f9b80");
extern float fast_difference_between_rotations_2e01a8(float, float) __asm__("func_001FA688");
extern float probe_ground_height_2e01a8(void *, int, float) __asm__("func_00213508");
extern int D_L10_001600EC_2e01a8; /* no foreign declaration */
extern int coll_sphere_mobys(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int FUN_001efa68_2e01a8(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_L00_001fefc8(void *);
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern int tick_countdown_32_alt_2e01a8(int *) __asm__("FUN_001f9740");
extern s32 random_integer_below_2e01a8(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_2e01a8(f32) __asm__("func_001FA6D0");
extern void FUN_L00_001ff290_2e01a8(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void add_vector_xyz_2e01a8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation_2e01a8(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void mark_moby_for_removal_2e01a8(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz_2e01a8(void *, void *, f32) __asm__("FUN_001f9bf8");
float absolute_float_2e01a8(float input) __asm__("func_001F99C0");
int FUN_L00_0025c698_2e01a8(void *moby, void *motion) __asm__("FUN_L00_0025c698");
s32 is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
void FUN_L00_00257470_2e01a8(unsigned char *m, int flags, int arg) __asm__("FUN_L00_00257470");
void FUN_L00_00258278_2e01a8(unsigned char *m, float *ptr, float t, float b, float c, float limit) __asm__("FUN_L00_00258278");
void FUN_L00_0025a120_2e01a8(char *o) __asm__("FUN_L00_0025a120");
void FUN_L00_0025f3e8_2e01a8(char *owner, char *pos, float scale, float flash) __asm__("FUN_L00_0025f3e8");
void FUN_L00_00263ac8_2e01a8(float x, int a, int b, unsigned char *p) __asm__("FUN_L00_00263ac8");
void FUN_L10_002e17c0_2e01a8(Moby *moby) __asm__("FUN_L10_002e17c0");
void FUN_L10_002e1cb0_2e01a8(unsigned char *moby) __asm__("FUN_L10_002e1cb0");
extern float frame_time_x __asm__("D_0015ED6C");
extern char D_0013F350[];
extern float D_0015ED70;
extern float func_001F9CB8_2E0630(void *) __asm__("FUN_001f9af0");
extern void func_001F9BF0_2E0630(void *, void *, void *) __asm__("FUN_001f9a28");
extern void func_001F9BD8_2E0630(void *, void *, void *) __asm__("FUN_001f9a10");
extern Anim_2E1568 *D_L10_001600EC_2E1568 __asm__("D_L10_001600EC");
extern int func_0022ED80_2E1568(int, int, void *) __asm__("FUN_0022da68");
extern int func_L00_001EFFF0_2E1568(void *, void *, int, void *, void *) __asm__("FUN_001efa68");

void FUN_L10_002e01a8(Moby *moby) {
    float scratch[20];
    Vars_2e01a8 *vars = (Vars_2e01a8 *)moby->pvars;
    Moby *target;

    FUN_L10_002e17c0_2e01a8(moby);
    target = vars->target;
    func_L00_00264B40_2E0630(2.5f, moby, 1, vars->a270);
    if (moby->visible != 0 && func_001F9D10_2E0630(&moby->pos, &D_L10_001672C0_2E0630) < 29.0f) {
        func_L00_0025B178_2E0630(moby);
        moby->shadow_range = 0x17;
    }
    moby->anim_speed = 1.0f;
    if (moby->state != 0 && moby->state != 13) {
        moby->pos.z = moby->pos.z - D_L10_00161E0C * func_001F9F90_2E0630(vars->bob_angle);
    }
    switch (moby->state) {
    case 0:
        vars->f30 = 0.25f;
        vars->f20 = 1.0f;
        vars->h24 = 1;
        vars->b28 = 0;
        vars->b29 = 1;
        moby->flags |= 0x1000;
        qcopy(&vars->home, &moby->pos);
        qcopy(&vars->goal, &vars->home);
        vars->bob_angle = func_00214158_2E0630();
        vars->turn_speed = 0.0f;
        vars->b58 = func_001FA898_2E0630(D_L10_00161E1C);
        vars->b5A = func_001FA898_2E0630(D_L10_00161E20 * 8.0f);
        switch (vars->spawn_kind) {
        case 0:
            moby->state = 3;
            moby->pos.z = moby->pos.z + 2.0f;
            if (moby->prev_seq != 3) func_00213DE0_2E0630(moby, 3, 0, func_001F9850_2E0630(10));
            break;
        case 1:
            func_0020D678_2E0630(moby);
            return;
        case 2:
            scratch[0] = func_001F9F90_2E0630(func_00214158_2E0630()) * 10.0f;
            scratch[1] = func_001F9FA8_2E0630(func_00214158_2E0630()) * 10.0f;
            moby->state = 9;
            scratch[2] = 0.0f;
            moby->pos.z = D_L10_001600EC_2E1568[vars->f194].f38;
            FUN_L00_001ff290_2e01a8(&scratch[4], &moby->pos, scratch);
            vars->height_offset = func_002140F8_2E0630(0.0f, D_L10_00161E14);
            vars->height_timer = func_001FA898_2E0630(FUN_001f96b0_2e01a8(D_L10_00161E18 * 60.0f) * func_002140F8_2E0630(0.75f, 1.25f));
            if (moby->prev_seq != 3) func_00213DE0_2E0630(moby, 3, 0, func_001F9850_2E0630(10));
            break;
        }
        break;
    case 1:
        break;
    case 2:
        moby->anim_speed = 4.0f;
        if (func_00214D88_2E0630(&moby->pos.z, vars->home.z + 2.0f, &vars->velocity.z,
                D_0015ED70 * 18.0f, D_0015ED70 * 18.0f, frame_time_x * 6.0f) == 0.0f) {
            moby->state = 3;
        }
        break;
    case 3: {
        float dist;
        float travelled;
        float speed;
        float zero;
        moby->anim_speed = 4.0f;
        speed = func_001F9CB8_2E0630(&vars->velocity);
        dist = func_001F9D10_2E0630(&vars->goal, &moby->pos);
        travelled = 0.0f;
        func_L00_0025CE58_2E0630(&moby->rot.z, func_L00_001FF860_2E0630(vars->goal.x - moby->pos.x, vars->goal.y - moby->pos.y), &vars->turn_speed,
            D_0015ED70 * 25.132742f, D_0015ED70 * 25.132742f, frame_time_x * 50.265484f);
        if (func_001FA850_2E0630(func_L00_001FF860_2E0630(vars->goal.x - moby->pos.x, vars->goal.y - moby->pos.y), moby->rot.z) < 0.78539819f) {
            func_00214D88_2E0630(&travelled, dist, &speed, D_0015ED70 * 20.0f, D_0015ED70 * 20.0f, frame_time_x * 20.0f);
            func_001F9BF0_2E0630(&vars->velocity, &vars->goal, &moby->pos);
            func_L00_002607A8_2E0630(&vars->velocity, speed);
        } else {
            func_001F9BC0_2E0630(&vars->velocity);
        }
        zero = 0.0f;
        if (func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, zero, 0) == 1) {
            float radius = func_002140F8_2E0630(4.0f, 6.0f);
            float angle = func_00214158_2E0630();
            scratch[0] = func_001F9F90_2E0630(angle) * radius;
            scratch[1] = func_001F9FA8_2E0630(angle) * radius;
            scratch[2] = zero;
            scratch[2] = func_002140F8_2E0630(D_L10_00161E04, D_L10_00161E08);
            func_001F9BD8_2E0630(&vars->goal, scratch, &vars->home);
        } else if (dist < 0.1f) {
            moby->state = 4;
            vars->wait = func_001F9850_2E0630(func_001FA898_2E0630(func_002140F8_2E0630(30.0f, 90.0f)));
        }
        if (vars->f104 == 2) break;
        if (!(3.0f < func_001F9D48_2E0630(&moby->pos, &target->pos))) break;
        if (FUN_L00_001fefc8(&moby->command) == 0) break;
        if (moby->group != 0xFF) FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
        moby->state = 6;
        if (moby->prev_seq != 1) func_00213DE0_2E0630(moby, 1, 0, func_001F9850_2E0630(10));
        break;
    }
    case 4:
        moby->anim_speed = 4.0f;
        if (vars->f104 != 2 && 3.0f < func_001F9D48_2E0630(&moby->pos, &target->pos) && FUN_L00_001fefc8(&moby->command) != 0) {
            if (moby->group != 0xFF) FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
            moby->state = 6;
            if (moby->prev_seq != 1) func_00213DE0_2E0630(moby, 1, 0, func_001F9850_2E0630(10));
            break;
        }
        if (func_001F9908_2E0630(&vars->wait) == 0) break;
        {
            float radius = func_002140F8_2E0630(4.0f, 6.0f);
            float angle = func_00214158_2E0630();
            scratch[0] = func_001F9F90_2E0630(angle) * radius;
            scratch[1] = func_001F9FA8_2E0630(angle) * radius;
            scratch[2] = 0.0f;
            scratch[2] = func_002140F8_2E0630(D_L10_00161E04, D_L10_00161E08);
            func_001F9BD8_2E0630(&vars->goal, scratch, &vars->home);
            moby->state = 3;
        }
        break;
    case 6:
        func_L00_002592B0_2E0630(moby, func_L00_001FF860_2E0630(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), &vars->turn_speed,
            0.02f, 0.3f, frame_time_x * 6.2831855f);
        if ((moby->anim_flags & 2) == 0) break;
        moby->state = 7;
        if (moby->prev_seq != 3) func_00213DE0_2E0630(moby, 3, 0, func_001F9850_2E0630(10));
        if (random_integer_below_2e01a8(0xFF) & 1) {
            vars->side_angle = 0.2617994f;
        } else {
            vars->side_angle = -0.2617994f;
        }
        break;
    case 7: {
        float lim;
        func_L00_002592B0_2E0630(moby, fast_add_rotations_2e01a8(func_L00_001FF860_2E0630(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), vars->side_angle), &vars->turn_speed,
            0.02f, 0.3f, frame_time_x * 6.2831855f);
        lim = func_00214358_2E0630(&moby->pos, 0, 0.5f);
        qcopy(scratch, &target->pos);
        if (scratch[2] < moby->pos.z) {
            scratch[2] = scratch[2] - (moby->pos.z - scratch[2]) * 4.0f;
        } else {
            scratch[2] = moby->pos.z;
        }
        scratch[2] = scratch[2] + 0.5f;
        if (scratch[2] < lim) scratch[2] = lim;
        func_001F9BF0_2E0630(&vars->velocity, scratch, &moby->pos);
        normalize_vector_xyz_2e01a8(&vars->velocity, &vars->velocity, frame_time_x * 10.0f);
        vars->velocity.x = func_001F9F90_2E0630(moby->rot.z) * vector_length_xy(&vars->velocity);
        vars->velocity.y = func_001F9FA8_2E0630(moby->rot.z) * vector_length_xy(&vars->velocity);
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, 0.0f, 0);
        func_L00_0025CC58_2E0630(&moby->rot.y, 0, -func_L00_001FF860_2E0630(vector_length_xy(&vars->velocity), vars->velocity.z), frame_time_x * 6.2831855f);
        if (func_001F9D48_2E0630(&moby->pos, &target->pos) < D_L10_00161DFC) {
            if (absolute_float_2e01a8(moby->pos.z - ((Hero_2e01a8 *)D_0013F350)->motion.pos.f[2]) < 2.0f) {
                func_0022ED80_2E1568(0, 0, moby);
                moby->state = 8;
                func_001F9BC0_2E0630(&vars->velocity);
                if (moby->prev_seq != 2) func_00213DE0_2E0630(moby, 2, 4, func_001F9850_2E0630(5));
                break;
            }
        } else if (vars->f104 != 2) {
            break;
        }
        if (moby->prev_seq != 3) func_00213DE0_2E0630(moby, 3, 0, func_001F9850_2E0630(0x14));
        moby->state = 5;
        break;
    }
    case 8:
        func_L00_002592B0_2E0630(moby, func_L00_001FF860_2E0630(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), &vars->turn_speed,
            0.02f, 0.3f, frame_time_x * 6.2831855f);
        func_L00_0025CC58_2E0630(&moby->rot.y, 0, -func_L00_001FF860_2E0630(func_001F9D48_2E0630(&moby->pos, &vars->vE0), vars->vE0.z - moby->pos.z),
            frame_time_x * 12.566371f);
        if (is_value_within_interpolated_window((char *)moby, 20.0f)) {
            float one = 1.0f;
            *(int *)&scratch[13] = 1;
            *(int *)&scratch[16] = 1;
            *(Moby **)&scratch[12] = moby;
            scratch[15] = one;
            scratch[8] = func_001F9F90_2E0630(moby->rot.z);
            scratch[9] = func_001F9FA8_2E0630(moby->rot.z);
            ((unsigned char *)scratch)[0x39] = 1;
            scratch[10] = one;
            scratch[11] = 5627.9248f;
            *(short *)((char *)scratch + 0x3A) = moby->oclass;
            ((unsigned char *)scratch)[0x38] = 0;
            FUN_L00_0024f7c8(moby, 0, scratch);
            coll_sphere_mobys(0.4f, scratch, 0, moby, &scratch[8]);
            func_L00_001EFFF0_2E1568(&moby->pos, scratch, 9, moby, &scratch[8]);
        }
        if (vars->f104 != 2 && (moby->anim_flags & 2) == 0) break;
        if (moby->prev_seq != 3) func_00213DE0_2E0630(moby, 3, 0, func_001F9850_2E0630(0x14));
        moby->state = 5;
        break;
    case 5: {
        float zero = 0.0f;
        float climb;
        func_L00_0025CC58_2E0630(&moby->rot.y, 0, zero, frame_time_x * 12.566371f);
        func_L00_002592B0_2E0630(moby, func_L00_001FF860_2E0630(vars->home.x - moby->pos.x, vars->home.y - moby->pos.y), &vars->turn_speed,
            0.02f, 0.3f, frame_time_x * 6.2831855f);
        vars->velocity.x = func_001F9F90_2E0630(moby->rot.z) * (frame_time_x * 6.0f);
        vars->velocity.y = func_001F9FA8_2E0630(moby->rot.z) * (frame_time_x * 6.0f);
        vars->velocity.z = zero;
        climb = vars->home.z + 2.0f - moby->pos.z;
        if (frame_time_x * 6.0f < climb) {
            climb = frame_time_x * 6.0f;
        } else if (climb < frame_time_x * -6.0f) {
            climb = frame_time_x * -6.0f;
        }
        vars->velocity.z = climb;
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, 0.0f, 0);
        if (func_001F9D48_2E0630(&moby->pos, &vars->home) < 2.0f) moby->state = 3;
        break;
    }
    case 9: {
        float dist = func_001F9D10_2E0630(&moby->pos, &target->pos);
        float climb;
        func_L00_002592B0_2E0630(moby, func_L00_001FF860_2E0630(vars->home.x - moby->pos.x, vars->home.y - moby->pos.y), &vars->turn_speed,
            0.02f, 0.3f, frame_time_x * 0.78539819f);
        vars->velocity.x = func_001F9F90_2E0630(moby->rot.z) * (frame_time_x * 4.0f);
        vars->velocity.y = func_001F9FA8_2E0630(moby->rot.z) * (frame_time_x * 4.0f);
        vars->velocity.z = 0.0f;
        if (func_001F9908_2E0630(&vars->height_timer)) {
            vars->height_offset = func_002140F8_2E0630(0.0f, D_L10_00161E14);
            vars->height_timer = func_001FA898_2E0630(FUN_001f96b0_2e01a8(D_L10_00161E18 * 60.0f) * func_002140F8_2E0630(0.75f, 1.25f));
        }
        climb = D_L10_001600EC_2E1568[vars->f194].f38 - moby->pos.z - vars->height_offset;
        if (frame_time_x + frame_time_x < climb) {
            climb = frame_time_x + frame_time_x;
        } else if (climb < frame_time_x * -2.0f) {
            climb = frame_time_x * -2.0f;
        }
        vars->velocity.z = climb;
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, 0.0f, 0);
        if (vars->f104 == 2) break;
        if (FUN_L00_001fefc8(&moby->command) == 0) break;
        if (!(D_L10_00161DFC < dist)) break;
        if (moby->group != 0xFF) FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
        moby->state = 10;
        if (moby->prev_seq != 3) func_00213DE0_2E0630(moby, 3, 0, func_001F9850_2E0630(10));
        break;
    }
    case 10: {
        float dist = func_001F9D10_2E0630(&moby->pos, &target->pos);
        float climb;
        if (moby->group != 0xFF) FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
        func_L00_002592B0_2E0630(moby, func_L00_001FF860_2E0630(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), &vars->turn_speed,
            0.02f, 0.3f, frame_time_x * 1.5707964f);
        vars->velocity.x = func_001F9F90_2E0630(moby->rot.z) * (frame_time_x * 4.0f);
        vars->velocity.y = func_001F9FA8_2E0630(moby->rot.z) * (frame_time_x * 4.0f);
        vars->velocity.z = 0.0f;
        climb = D_L10_001600EC_2E1568[vars->f194].f38 - moby->pos.z;
        if (frame_time_x * 4.0f < climb) {
            climb = frame_time_x * 4.0f;
        } else if (climb < frame_time_x * -4.0f) {
            climb = frame_time_x * -4.0f;
        }
        vars->velocity.z = climb;
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, 0.0f, 0);
        if (dist < D_L10_00161DFC
            && absolute_float_2e01a8(D_L10_001600EC_2E1568[vars->f194].f38 - moby->pos.z) < 1.0f) {
            func_0022ED80_2E1568(0, 0, moby);
            moby->state = 11;
            if (moby->prev_seq != 2) func_00213DE0_2E0630(moby, 2, 0, func_001F9850_2E0630(2));
            break;
        }
        if (vars->f104 != 2) break;
        moby->state = 9;
        vars->height_offset = func_002140F8_2E0630(0.0f, D_L10_00161E14);
        vars->height_timer = func_001FA898_2E0630(FUN_001f96b0_2e01a8(D_L10_00161E18 * 60.0f) * func_002140F8_2E0630(0.75f, 1.25f));
        break;
    }
    case 11: {
        float climb;
        float zero;
        float one;
        if (moby->group != 0xFF) FUN_L10_002e1cb0_2e01a8((unsigned char *)moby);
        func_L00_002592B0_2E0630(moby, func_L00_001FF860_2E0630(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), &vars->turn_speed,
            0.02f, 0.3f, frame_time_x * 6.2831855f);
        vars->velocity.x = func_001F9F90_2E0630(moby->rot.z) * (frame_time_x * 6.0f);
        vars->velocity.y = func_001F9FA8_2E0630(moby->rot.z) * (frame_time_x * 6.0f);
        vars->velocity.z = 0.0f;
        climb = D_L10_001600EC_2E1568[vars->f194].f38 - moby->pos.z;
        if (frame_time_x * 3.0f < climb) {
            climb = frame_time_x * 3.0f;
        } else if (climb < frame_time_x * -3.0f) {
            climb = frame_time_x * -3.0f;
        }
        vars->velocity.z = climb;
        zero = 0.0f;
        func_L00_00259B08_2E0630(moby, &vars->velocity, 0.5f, 0x200, zero, 0);
        one = 1.0f;
        *(int *)&scratch[9] = 0x10001;
        *(int *)&scratch[12] = 1;
        *(Moby **)&scratch[8] = moby;
        scratch[11] = one;
        func_001F9BC0_2E0630(&scratch[4]);
        FUN_L00_0024f7c8(moby, 0, scratch);
        coll_sphere_mobys(0.2f, scratch, 0, moby, &scratch[4]);
        if (func_001F9D10_2E0630(&moby->pos, &target->pos) < one
            || *(int *)(D_0013F350 + 0x208C) == 7
            || (moby->anim_flags & 2)) {
            moby->state = 12;
            if (moby->prev_seq != 3) func_00213DE0_2E0630(moby, 3, 0, func_001F9850_2E0630(2));
            break;
        }
        if (vars->f104 != 2) break;
        moby->state = 9;
        vars->height_offset = func_002140F8_2E0630(zero, D_L10_00161E14);
        vars->height_timer = func_001FA898_2E0630(FUN_001f96b0_2e01a8(D_L10_00161E18 * 60.0f) * func_002140F8_2E0630(0.75f, 1.25f));
        break;
    }
    case 12: {
        float climb;
        func_L00_002592B0_2E0630(moby, func_L00_001FF860_2E0630(vars->home.x - moby->pos.x, vars->home.y - moby->pos.y), &vars->turn_speed,
            0.02f, 0.3f, frame_time_x * 1.0471976f);
        vars->velocity.x = func_001F9F90_2E0630(moby->rot.z) * (frame_time_x * 4.0f);
        vars->velocity.y = func_001F9FA8_2E0630(moby->rot.z) * (frame_time_x * 4.0f);
        vars->velocity.z = 0.0f;
        climb = D_L10_001600EC_2E1568[vars->f194].f38 - moby->pos.z;
        if (frame_time_x * 3.0f < climb) {
            climb = frame_time_x * 3.0f;
        } else if (climb < frame_time_x * -3.0f) {
            climb = frame_time_x * -3.0f;
        }
        vars->velocity.z = climb;
        FUN_L00_001ff290_2e01a8(scratch, &moby->pos, &vars->velocity);
        if (func_001F9D48_2E0630(&moby->pos, &vars->home) < 2.0f) {
            moby->state = 9;
            moby->command = func_001F9850_2E0630(0x78);
            vars->height_offset = func_002140F8_2E0630(0.0f, D_L10_00161E14);
            vars->height_timer = func_001FA898_2E0630(FUN_001f96b0_2e01a8(D_L10_00161E18 * 60.0f) * func_002140F8_2E0630(0.75f, 1.25f));
        }
        break;
    }
    case 13:
        if ((func_L00_0025D6F0_2E0630(moby, vars->a60) & 1) == 0) break;
        func_L00_002584A8_2E0630(moby, 0x200, -1);
        func_L00_00260460_2E0630(moby, &moby->pos, -1, 0.5f, 10.0f);
        func_0020D678_2E0630(moby);
        return;
    }
    if (moby->state != 0 && moby->state != 13) {
        vars->bob_angle = fast_add_rotations_2e01a8(vars->bob_angle,
            360.0f / (float)func_001FA898_2E0630(FUN_001f96b0_2e01a8(D_L10_00161E10 * 60.0f)) * 0.017453292f);
        moby->pos.z = moby->pos.z + D_L10_00161E0C * func_001F9F90_2E0630(vars->bob_angle);
    }
}
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E2B80), where it is exact; names translated to the US level program. */
#include "rnc/overlay/quad.h"

/* Level 10 moby update: tracks the owner's height and fires a shot, then sets the moby's display state. */


typedef struct Hit_2E2B80 {
    float x;
    float y;
    char pad8[8];
    float f10[4];
    struct Moby *moby;
    char pad24[8];
    float f2C;
    int f30;
} Hit_2E2B80;

typedef struct MobyData_2E2B80 {
    char pad0[0x20];
    float f20;
    char pad24[2];
    short f26;
    char pad28[0x10];
    int f38;
    char pad3C[0x24];
    char f60[0x10];
    float f70;
    float f74;
    float f78;
    float f7C;
    char pad80[4];
    int f84;
    char pad88[0x15];
    unsigned char f9D;
    char pad9E[0x22];
    char fC0[0x40];
    int f100;
    int f104;
    char pad108[8];
    char f110[7];
    unsigned char f117;
    char pad118[0x18];
    char f130[0x44];
    int f174;
    float f178;
    int f17C;
    float f180;
    int f184;
    float f188;
    int f18C;
    char pad190[4];
    int f194;
} MobyData_2E2B80;

typedef struct Anim_2E2B80 {
    char pad0[0x30];
    float f30[4];
    float f40[4];
    char pad50[0x30];
} Anim_2E2B80;

extern Anim_2E2B80 *D_L10_001600EC;

typedef struct Path_2E2B80 {
    int count;
    char pad4[0xC];
    float pts[4];
} Path_2E2B80;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char D_0013F3D0[];
extern float FUN_001f96b0(float);
extern float FUN_001f9e90(float, float);
extern float truncate_float_to_s32_alt(float, float) __asm__("FUN_001fa6d0");
extern float D_0015ED6C;
extern float D_0015ED70;
extern int FUN_00213928();
extern int FUN_L00_0025ff38();
extern int FUN_L00_00260a88();
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int blend_moby_animation_alt(int) __asm__("FUN_00212f90");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern s32 random_float_between_alt(f32) __asm__("FUN_002132a8");
extern s32 scale_game_frames_c() __asm__("func_001F96F8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
void FUN_L00_0025ab48(OvlQuad *v, float *ang, float *s1, float *s2);
void FUN_L00_0025d538(s32 id, void *s);
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int func_001FA898_2E2B80(float) __asm__("FUN_001fa6d0");
extern int func_001F9908_2E2B80(int *) __asm__("FUN_001f9740");
extern Hit_2E2B80 *trooper_find_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int func_0022ED80_2E2B80(int, int, void *) __asm__("FUN_0022da68");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, void *p) __asm__("FUN_L00_0025d458");
extern void func_L00_00261B00_2E2B80(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern int ahead_f(float, void *, void *) __asm__("FUN_L01_00274b78");
extern int activate_effect_l08(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern char D_0013F3D0[];
extern int D_001413D0[];
extern Path_2E2B80 *D_L10_001B0C30_2E2B80[] __asm__("D_L10_001B0930");

void FUN_L10_002e17c0(struct Moby *moby)
{
    MobyData_2E2B80 *data = ((struct MobyData_2E2B80 *)moby->pvars);
    float v[4];
    float w[4];
    int st;
    float ang;
    Hit_2E2B80 *r;
    int r19;

    if (data->f38 != 0) {
        data->f184 = func_001FA898_2E2B80(scale_time(random_float_between(180.0f, 240.0f)));
        data->f38 = 0;
    }
    func_001F9908_2E2B80(&data->f184);
    if (moby->state != 13) {
        r = trooper_find_hit(moby, 0x330000, 0);
        fun_00213928(moby, r, &data->f20, 5, &st, 0, 0, 4);
        if (r != 0 && r->moby->oclass != moby->oclass) {
            if (data->f20 <= r->f2C) {
                data->f20 = 0.0f;
                moby->flags &= 0xEFFF;
                if (moby->prev_seq != 4) blend_moby_animation(moby, 4, 0, scale_game_frames(10));
                moby->state = 13;
                data->f70 = D_0015ED70 * 26.0f;
                data->f9D = 3;
                data->f84 = 9;
                data->f74 = 0.0005f;
                data->f78 = D_0015ED6C * 7.0f;
                data->f7C = D_0015ED6C * 10.0f;
                func_0022ED80_2E2B80(2, 0, moby);
                if (r->f30 & 1) {
                    ang = angle_atan2(r->f10[0], r->f10[1]);
                } else {
                    ang = angle_atan2(moby->pos.x - r->x, moby->pos.y - r->y);
                }
                *(OvlQuad *)v = *(OvlQuad *)r->f10;
                trooper_knockback_arc(v, &ang, &data->f78, &data->f7C);
                trooper_start_knockback(moby, data->f60, ang, 4, 1, 0);
                data->f117 = 0x78;
                trooper_anim_update(moby, data->f110);
                func_L00_00261B00_2E2B80(moby, 1, 3, 0, -1);
            } else {
                data->f20 = data->f20 - r->f2C;
                data->f117 = 0xFA;
                data->f26 = scale_game_frames(60);
                trooper_anim_update(moby, data->f110);
            }
        }
        moby->hit_slot = 0xFF;
    }
    trooper_anim_finish(moby, data->f110);
    if (data->f174 == 2) {
        data->f180 = 42.0f;
    } else if (data->f184 != 0) {
        data->f180 = data->f178 + 6.0f;
    } else if (moby->state == 1) {
        data->f180 = data->f188;
    } else {
        data->f180 = data->f178;
    }
    if (data->f174 == 2) {
        r19 = ahead_f(data->f180, moby, data->fC0);
        if (r19 != 2) {
            Anim_2E2B80 *bp = &D_L10_001600EC[data->f194];
            subtract_vector_xyz(v, D_0013F3D0, bp->f30);
            v[3] = 0.0f;
            transform_vector_by_basis(w, v, bp->f40);
            if (w[0] <= -1.0f || 1.0f <= w[0] || w[1] <= -1.0f || 1.0f <= w[1] || w[2] <= -1.0f || 1.0f <= w[2]) {
                data->f104 = 2;
            }
        }
    } else if (moby->state == 1 && data->f18C >= 0) {
        r19 = activate_effect_l08(data->f180, moby, data->fC0, 0, 0, D_L10_001B0C30_2E2B80[data->f18C]->pts, D_L10_001B0C30_2E2B80[data->f18C]->count);
    } else if (moby->state != 1 && data->f17C >= 0) {
        r19 = activate_effect_l08(data->f180, moby, data->fC0, 0, 0, D_L10_001B0C30_2E2B80[data->f17C]->pts, D_L10_001B0C30_2E2B80[data->f17C]->count);
    } else {
        r19 = ahead_f(data->f180, moby, data->fC0);
    }
    if (r19 != 2) {
        if (data->f180 < vector_distance(data->f130, data->fC0)) {
            data->f104 = 2;
        } else if (moby->visible == 0) {
            data->f104 = 2;
        }
    }
    if (data->f100 == 0) {
        data->f100 = D_001413D0[0];
    }
}
/* Stores a value into each table entry listed for this moby's index. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E3070), where it is exact; names translated to the US level program. */


extern short D_L10_00161E00 __attribute__((sda));
void FUN_L10_002e1cb0(struct Moby *moby) {
    short *p;
    if (moby->group != 0xFF) {
        p = D_L10_001ABCC0[moby->group];
        if (p != 0) {
            do {
                int idx = *(unsigned short *)p & 0x7FFF;
                ((char *)D_L10_0015FFD0_e[2])[(idx << 8) + 0xBC] =
                    scale_game_frames(*(int *)&D_L10_00161E00);
            } while (*p++ >= 0);
        }
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Update for the toxic crab (moby class 1202): line-of-sight test, then a 13-state walk/chase/attack/death machine. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E30F8), where it is exact; names translated to the US level program. */


typedef struct {
    float v[4];
    char *m;
    int flags;
    unsigned char b0;
    unsigned char b1;
    unsigned short id;
    float f;
    int n;
    int pad[3];
} Hit2E30F8;

typedef struct {
    char p0[0x1C0];
    int f1C0;
    char p1[0x20A4 - 0x1C4];
    unsigned char f20A4;
} Hero2E30F8;

typedef struct Crab2E30F8 {
    char p000[0x20];
    float f20;
    short s24;
    char p26[2];
    unsigned char b28;
    unsigned char b29;
    char p2A[6];
    float f30;
    char p34[0x24];
    unsigned char b58;
    char p59;
    unsigned char b5A;
    char p5B[5];
    char q60[7];
    unsigned char b67;
    char p68[8];
    float v70[2];
    float f78;
    char p7C[4];
    float f80;
    char p84[0xC];
    int i90;
    int i94;
    float f98;
    char p9C[0x11];
    unsigned char bAD;
    char pAE[0x12];
    float fC0;
    float fC4;
    char pC8[8];
    int iD0;
    float fD4;
    float fD8;
    float fDC;
    char pE0[0x14];
    float fF4;
    char pF8[0x28];
    float f120;
    float f124;
    char p128[0x38];
    char *tgt;
    int i164;
    char p168[8];
    float v170[4];
    char p180[0x10];
    float f190;
    float f194;
    float f198;
    char p19C[4];
    float f1A0;
    float f1A4;
    int i1A8;
    char p1AC[8];
    int i1B4;
    int i1B8;
    int i1BC;
    float v1C0[4];
    char p1D0[8];
    int i1D8;
    int i1DC;
    char p1E0[0xD0];
    unsigned char q2B0[4];
} Crab2E30F8;

typedef struct Moby2E30F8 {
    char p00[0x10];
    float pos[4];
    unsigned char state;
    unsigned char b21;
    char p22[2];
    char *p24;
    char p28[4];
    float f2C;
    char p30;
    unsigned char b31;
    char p32[2];
    unsigned short s34;
    char p36[0x12];
    float f48;
    char p4C[7];
    unsigned char anim;
    char p54[4];
    float f58;
    char p5C[0x14];
    unsigned char b70;
    char p71[7];
    Crab2E30F8 *data;
    char p7C[3];
    unsigned char b7F;
    char p80[0x26];
    unsigned short cls;
    char pA8[8];
    unsigned char bB0;
    char pB1;
    unsigned short id;
} Moby2E30F8;

extern char D_L10_00167240[];
extern char D_L10_001742C0[];
extern float absolute_float(float) __asm__("func_001F99C0");
extern float D_L10_0015F580[] __attribute__((section(".sdata")));
extern float FUN_001f96b0(float);
extern float FUN_001f9b48(void *, void *);
extern float FUN_001f9b80(void *, void *);
extern float FUN_001f9e90(float, float);
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern float FUN_L10_002e4840(struct Moby *self);
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_cos(float);
extern float fast_sin(float);
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern int D_L10_0015F5CC;
extern int D_L10_0015FFD8;
extern char *D_L10_0015FFD0_w[] __asm__("D_L10_0015FFD0") __attribute__((section(".sdata")));
extern int FUN_001efa68(void *, void *, int, void *, void *);
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern int FUN_L00_00258b50(void *, void *, void *, void *, float);
extern int FUN_L00_0025c698(void *, void *);
extern int random_integer_below(int) __asm__("FUN_00213260");
extern int tick_countdown_32(void *);
extern s32 FUN_L00_0025e3b8(s32);
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern short D_L10_00161E24 __attribute__((sda));
extern short D_L10_00161E28 __attribute__((sda));
extern short D_L10_00161E34 __attribute__((sda));
extern f32 D_L10_00161E40_gp __asm__("D_L10_00161E40") __attribute__((sda));
extern short D_L10_00161E44 __attribute__((sda));
extern short D_L10_00161E4C __attribute__((sda));
extern unsigned char D_0013E533[];
extern unsigned char D_0014C050[];
extern void *FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *,
                              float *);
extern void collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_0025c558(float, void *, void *, int, int, int);
extern void FUN_L00_0025d458(void *m, short *p);
extern void FUN_L00_0025f3e8(void *, void *, int, float, float);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void FUN_L01_0026d930(char *p);
extern void FUN_L03_00250a78(float *p, float a, float b);
extern void FUN_L10_002e0138(struct Moby *m);
extern void FUN_L10_002e3a90_u(void *) __asm__("FUN_L10_002e3a90");
extern void FUN_L10_002e3de8(char *moby);
extern void FUN_L10_002e3ef0_u(void *) __asm__("FUN_L10_002e3ef0");
extern void add_vector_xyz(void *, void *, void *);
extern void clear_vector(void *);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern char D_0014C190[];
extern void func_L00_0025D5B0_b(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void func_L00_002592B0_b(char *moby, float target, float k, float d, float max,
                                float *vel) __asm__("FUN_L00_00258278");

void FUN_L10_002e1d38(Moby2E30F8 *m) {
    int see = 0;
    Crab2E30F8 *d = m->data;
    char *t;
    float from[4];
    float to[4];

    FUN_L10_002e3ef0_u(m);
    t = d->tgt;
    m->f2C = *(float *)(m->p24 + 0x24) * *(float *)&D_L10_00161E24;
    FUN_L00_00263ac8(2.5f, (int)m, 2, d->q2B0);
    *(OvlQuad *)from = *(OvlQuad *)m->pos;
    from[2] += 1.0f;
    *(OvlQuad *)to = *(OvlQuad *)(t + 0x10);
    to[2] += 1.0f;
    if (FUN_001f9b48(from, to) < 15.0f &&
        !(from[0] < 2.0f || 1021.0f < from[0] || from[1] < 2.0f || 1021.0f < from[1] ||
          from[2] < 2.0f || 1021.0f < from[2]) &&
        !(to[0] < 2.0f || 1021.0f < to[0] || to[1] < 2.0f || 1021.0f < to[1] || to[2] < 2.0f ||
          1021.0f < to[2])) {
        see = FUN_001efa68(from, to, 2, m, 0) == 0;
    }
    if (!see || (!(absolute_float(*(float *)(t + 0x18) - m->pos[2]) < 1.0f) &&
                 FUN_L00_0025e3b8(((int)hero.ground_moby)) == 0)) {
        see = 0;
    }
    switch (m->state) {
    case 0:
        if (d->i1DC != 0 && D_0013E533[0x2EC1] == 1) {
            mark_moby_for_removal(m);
            return;
        }
        d->f30 = 0.5f;
        d->s24 = 5;
        d->b29 = 1;
        d->f20 = 5.0f;
        d->b28 = 3;
        d->b58 = truncate_float_to_s32(10.0f);
        d->b5A = truncate_float_to_s32(16.0f);
        m->s34 |= 0x5000;
        qcopy(d->v170, m->pos);
        d->v170[3] = m->f48;
        FUN_L01_0026d930((char *)&d->iD0);
        {
            float k = *(float *)&D_L10_00161E24 * 1024.0f;
            float s;
            float e;
            int r;
            d->fDC = 0.5f;
            d->fD8 = 2.0f;
            r = truncate_float_to_s32(k);
            s = d->f1A4 * frame_time;
            e = *(float *)&D_L10_00161E24;
            d->iD0 = r;
            d->fD4 = e;
            d->fF4 = s;
        }
        {
            int k = d->i1BC;
            if (k != -1) {
                Moby2E30F8 *p = (Moby2E30F8 *)(D_L10_0015FFD0_w[2] + (k << 8));
                if (p == 0 || p->state == 0xFE || p->state == 0xFD ||
                    (((*(int *)(((char *)&D_0014C190) +
                                (((short)p->id >> 5) * 4 + (current_level_index << 8))) >>
                       (p->id & 0x1F)) &
                      1) &&
                     D_0014C050[p->bB0 + (current_level_index << 4)] == 0xFF)) {
                    d->i1BC = -1;
                }
            }
        }
        m->state = 1;
        if (m->anim != 0) {
            blend_moby_animation(m, 0, 0, 0);
        }
        d->f1A0 = *(float *)&D_L10_00161E40_gp;
        break;
    case 1:
        FUN_L00_00258278((char *)m, &d->f190, d->v170[3], 0.02f, 0.3f, frame_time * 6.2831855f);
        if (d->i1BC != -1) {
            char *base = D_L10_0015FFD8;
            Moby2E30F8 *p = (Moby2E30F8 *)(base + (d->i1BC << 8));
            if (p == 0 || p->state == 0xFE || p->state == 0xFD ||
                ((*(int *)(((char *)&D_0014C190) +
                           (((short)p->id >> 5) * 4 + (current_level_index << 8))) >>
                  (p->id & 0x1F)) &
                 1)) {
                m->state = 2;
                d->i1A8 = scale_game_frames(1);
                if (m->b21 != 0xFF) {
                    FUN_L10_002e0138((struct Moby *)m);
                }
            }
        }
        if (d->i164 != 2 || d->i1B4 != 0) {
            m->state = 2;
            d->i1A8 = scale_game_frames(1);
            if (m->b21 != 0xFF) {
                FUN_L10_002e0138((struct Moby *)m);
            }
        }
        break;
    case 2:
        if (m->anim == 0) {
            if (d->i1A8 == 0) {
                d->i1A8 = truncate_float_to_s32(FUN_001f96b0(random_float_between(0.0f, 20.0f)));
            } else if (tick_countdown_32(&d->i1A8)) {
                if (d->i1B4 != 0) {
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                } else {
                    if (m->anim != 2) {
                        blend_moby_animation(m, 2, 0, scale_game_frames(10));
                    }
                }
            }
        } else if (m->b70 & 2) {
            if (FUN_001f9b80(m->pos, &hero.motion.pos) < *(float *)&D_L10_00161E40_gp) {
                float x;
                m->state = 5;
                d->i1D8 = 0;
                qcopy(d->v1C0, m->pos);
                d->fF4 = 0.0f;
                x = d->f198 * DEG_TO_RAD;
                d->f194 = random_float_between(x, -x);
                if (m->anim != 3) {
                    blend_moby_animation(m, 3, 0, scale_game_frames(5));
                }
            } else {
                m->state = 3;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case 3: {
        float ang = FUN_001f9e90(d->f120 - m->pos[0], d->f124 - m->pos[1]);
        FUN_L00_00258278((char *)m, &d->f190, ang, 0.02f, 0.3f, frame_time * 6.2831855f);
        if (d->i164 != 2) {
            m->state = 4;
            d->i1A8 = truncate_float_to_s32(random_float_between(0.0f, (float)d->i1B8));
        }
        break;
    }
    case 4:
        if (tick_countdown_32(&d->i1A8)) {
            float x;
            m->state = 5;
            d->i1D8 = 0;
            qcopy(d->v1C0, m->pos);
            d->fF4 = 0.0f;
            x = d->f198 * DEG_TO_RAD;
            d->f194 = random_float_between(x, -x);
            if (m->anim != 3) {
                blend_moby_animation(m, 3, 0, scale_game_frames(5));
            }
        }
        break;
    case 5: {
        {
            float dir[4];
            float out[4];
            float old[4];
            float v[4];
            float dist;
            float ang;
            float diff;
            float aim;

            dist = FUN_001f9b48(m->pos, t + 0x10);
            if (dist > (*(short *)(t + 0xA6) == 0x359 ? 2.0f : 3.57f)) {
                approach_value(&d->fF4, d->f1A4 * frame_time, frame_time_sq * 8.0f);
            } else {
                approach_value(&d->fF4, d->f1A4 * frame_time * 0.3f, frame_time_sq * 20.0f);
            }
            *(OvlQuad *)old = *(OvlQuad *)m->pos;
            if (see &&
                absolute_float(dist - (*(short *)(t + 0xA6) == 0x359 ? 2.0f : 3.57f)) < 0.35f) {
                approach_value(&d->fF4, 0.0f, frame_time_sq * 30.0f);
            } else if (d->i1D8 != 0) {
                approach_value(&d->fF4, 0.0f, frame_time_sq * 10.0f);
            }
            ang = FUN_001f9e90(*(float *)(t + 0x10) - m->pos[0], *(float *)(t + 0x14) - m->pos[1]);
            diff = fast_difference_between_rotations(ang, m->f48);
            if (0.87266463f < diff) {
                float lim = d->f1A4 * frame_time * 0.25f;
                if (lim < d->fF4) {
                    d->fF4 = lim;
                }
            }
            if (*(float *)&D_L10_00161E4C * DEG_TO_RAD < diff || d->i1D8 == 0) {
                FUN_L00_00258278((char *)m, &d->f190, ang, 0.02f, 0.3f, frame_time * 6.2831855f);
            }
            aim = FUN_L10_002e4840((struct Moby *)m);
            v[0] = fast_cos(aim) * (*(short *)(t + 0xA6) == 0x359 ? 2.0f : 3.57f);
            v[1] = fast_sin(aim) * (*(short *)(t + 0xA6) == 0x359 ? 2.0f : 3.57f);
            v[2] = 0.0f;
            add_vector_xyz(v, v, t + 0x10);
            if (hero.vehicle_mode != 1) {
                float rot = m->f48;
                int r;
                m->f48 = FUN_001f9e90(v[0] - m->pos[0], v[1] - m->pos[1]);
                dir[0] = fast_cos(m->f48) * 2.0f;
                dir[1] = fast_sin(m->f48) * 2.0f;
                dir[2] = 0.0f;
                clear_vector(out);
                {
                    float from2[4];
                    float to2[4];
                    *(OvlQuad *)from2 = *(OvlQuad *)m->pos;
                    from2[2] += 1.0f;
                    *(OvlQuad *)to2 = *(OvlQuad *)(t + 0x10);
                    to2[2] += 1.0f;
                    if (FUN_001efa68(from2, to2, 2, m, 0)) {
                        char *h = D_L10_001742C0;
                        if ((*(char **)(h + 0x18) == 0 ||
                             *(short *)(*(char **)(h + 0x18) + 0xA6) != 0x31A) &&
                            FUN_001f9b48(from2, h + 0x20) < *(float *)&D_L10_00161E34) {
                            m->state = 6;
                            d->i1A8 = scale_game_frames(0x3C);
                            if (m->anim != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(7));
                            }
                        }
                    }
                }
                r = FUN_L00_00258b50(m, &d->iD0, dir, out, 1.0f);
                m->f48 = rot;
                if (r & 3) {
                    if (FUN_001f9b80(m->pos, old) < d->fF4 * 0.25f) {
                        d->i1D8 = 1;
                    } else {
                        d->i1D8 = 0;
                    }
                    if (D_L10_0015F5CC % 7 == 0 && d->i1A8 == 0) {
                        if (FUN_001f9b80(d->v1C0, m->pos) < d->fF4 &&
                            diff <= *(float *)&D_L10_00161E4C * DEG_TO_RAD) {
                            m->state = 6;
                            d->i1A8 = scale_game_frames(0x3C);
                            if (m->anim != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(7));
                            }
                        } else if (m->anim == 1) {
                            blend_moby_animation(m, 3, 0, scale_game_frames(7));
                        }
                        qcopy(d->v1C0, m->pos);
                    }
                } else {
                    d->i1D8 = 0;
                }
                if (m->state == 5 && tick_countdown_32(&d->i1A8) && 0.0f < d->fF4 && d->i1D8 == 0 &&
                    m->anim == 1) {
                    blend_moby_animation(m, 3, 0, scale_game_frames(7));
                }
            } else {
                if (absolute_float(d->f190) > frame_time * 0.08726646f) {
                    if (m->anim != 3) {
                        blend_moby_animation(m, 3, 0, scale_game_frames(7));
                    }
                } else {
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(7));
                    }
                }
            }
            m->f58 = 1.2f;
            {
                float home = FUN_001f9b48(m->pos, d->v170);
                if ((hero.vehicle_mode == 1 ? 1.0f : 30.0f) < home ||
                    d->i164 == 2) {
                    m->state = 8;
                    blend_moby_animation(m, 4, 0, scale_game_frames(0xC));
                } else if (FUN_001f9b80(m->pos, t + 0x10) <
                           (*(short *)(t + 0xA6) == 0x359 ? 2.45f : 4.02f)) {
                    if (hero.invulnerable_frames < scale_game_frames(10) &&
                        diff < 0.34906584f && see) {
                        int anim = 7;
                        if (random_integer_below(100) & 1) {
                            anim = 6;
                        }
                        m->state = 7;
                        blend_moby_animation(m, anim, 0, scale_game_frames(7));
                    }
                }
            }
        }
        break;
    }
    case 6: {
        float diff;

        diff = fast_difference_between_rotations(
            FUN_001f9e90(*(float *)(t + 0x10) - m->pos[0], *(float *)(t + 0x14) - m->pos[1]),
            m->f48);
        if (0.87266463f < diff) {
            float lim = d->f1A4 * frame_time * 0.25f;
            if (lim < d->fF4) {
                d->fF4 = lim;
            }
        }
        if (tick_countdown_32(&d->i1A8) || *(float *)&D_L10_00161E4C * DEG_TO_RAD < diff) {
            float from2[4];
            float to2[4];
            int ok = 1;
            *(OvlQuad *)from2 = *(OvlQuad *)m->pos;
            from2[2] += 1.0f;
            *(OvlQuad *)to2 = *(OvlQuad *)(t + 0x10);
            to2[2] += 1.0f;
            if (FUN_001efa68(from2, to2, 2, m, 0)) {
                char *h = D_L10_001742C0;
                if ((*(char **)(h + 0x18) == 0 ||
                     *(short *)(*(char **)(h + 0x18) + 0xA6) != 0x31A) &&
                    FUN_001f9b48(from2, h + 0x20) < *(float *)&D_L10_00161E34) {
                    ok = 0;
                }
            }
            if (ok) {
                m->state = 5;
                d->i1D8 = 0;
                qcopy(d->v1C0, m->pos);
                d->fF4 = 0.0f;
                d->i1A8 = scale_game_frames(0xF);
            }
        }
        {
            float home = FUN_001f9b48(m->pos, d->v170);
            if ((hero.vehicle_mode == 1 ? 1.0f : 30.0f) < home ||
                d->i164 == 2) {
                m->state = 8;
                blend_moby_animation(m, 4, 0, scale_game_frames(0xC));
            } else if (FUN_001f9b80(m->pos, t + 0x10) <
                       (*(short *)(t + 0xA6) == 0x359 ? 2.45f : 4.02f)) {
                if (hero.invulnerable_frames < scale_game_frames(10) &&
                    diff < *(float *)&D_L10_00161E4C * DEG_TO_RAD && see) {
                    int anim = 7;
                    if (random_integer_below(100) & 1) {
                        anim = 6;
                    }
                    m->state = 7;
                    blend_moby_animation(m, anim, 0, scale_game_frames(7));
                }
            }
        }
    }
    case 7: {
        float fr;
        float ang;

        fr = compute_interpolated_record_value(m);
        ang = FUN_001f9e90(*(float *)(t + 0x10) - m->pos[0], *(float *)(t + 0x14) - m->pos[1]);
        FUN_L00_00258278((char *)m, &d->f190, ang, 0.03f, 0.3f, frame_time * 6.2831855f);
        if (m->anim >= 6 && m->anim <= 7 && 4.0f <= fr && fr <= 7.0f) {
            Hit2E30F8 hit;
            float v[4];
            float pos[4];
            *(OvlQuad *)pos = *(OvlQuad *)m->pos;
            pos[2] += 0.25f;
            hit.m = (char *)m;
            hit.flags = 0x10001;
            hit.f = 3.1f;
            hit.n = 1;
            hit.v[0] = fast_cos(m->f48);
            hit.v[1] = fast_sin(m->f48);
            hit.v[2] = 1.0f;
            hit.v[3] = 5627.925f;
            hit.b0 = 0;
            hit.b1 = 1;
            hit.id = m->cls;
            if (m->anim == 6) {
                moby_point_position(m, 0, v);
                collect_mobies_in_sphere(v, 0.75f, 0, m, &hit);
                v[2] += 0.5f;
                collect_mobies_in_sphere(v, 0.75f, 0, m, &hit);
                FUN_001efa68(pos, v, 9, m, &hit);
            } else {
                moby_point_position(m, 1, v);
                collect_mobies_in_sphere(v, 0.75f, 0, m, &hit);
                v[2] += 0.5f;
                collect_mobies_in_sphere(v, 0.75f, 0, m, &hit);
                FUN_001efa68(pos, v, 9, m, &hit);
            }
        } else if (m->anim >= 6 && m->anim <= 7 && (m->b70 & 2)) {
            float home = FUN_001f9b48(m->pos, d->v170);
            if ((D_0013E533[0x2EC1] == 1 ? 1.0f : 30.0f) < home || d->i164 == 2) {
                m->state = 8;
                blend_moby_animation(m, 4, 0, scale_game_frames(0xC));
            } else {
                m->state = 5;
                d->i1D8 = 0;
                qcopy(d->v1C0, m->pos);
                d->fF4 = 0.0f;
                if (m->anim != 3) {
                    blend_moby_animation(m, 3, 0, scale_game_frames(7));
                }
            }
        }
        break;
    }
    case 9: {
        {
            float dir[4];
            float out[4];
            float rot;
            float ang;

            approach_value(&d->fF4, d->f1A4 * 3.0f * frame_time, frame_time_sq * 20.0f);
            ang = FUN_001f9e90(*(float *)(t + 0x10) - m->pos[0], *(float *)(t + 0x14) - m->pos[1]);
            FUN_L00_00258278((char *)m, &d->f190, ang, 0.02f, 0.3f, frame_time * 6.2831855f);
            rot = m->f48;
            m->f48 = fast_add_rotations(
                FUN_001f9e90(*(float *)(t + 0x10) - m->pos[0], *(float *)(t + 0x14) - m->pos[1]),
                3.14159f);
            dir[0] = fast_cos(m->f48) * 2.0f;
            dir[1] = fast_sin(m->f48) * 2.0f;
            dir[2] = 0.0f;
            FUN_L00_00258b50(m, &d->iD0, dir, out, 1.0f);
            m->f48 = rot;
            if (see) {
                if (tick_countdown_32(&d->i1A8) ||
                    *(float *)&D_L10_00161E44 < FUN_001f9b48(m->pos, t + 0x10)) {
                    int anim;
                    m->state = 7;
                    d->fF4 = frame_time * 5.0f;
                    anim = 7;
                    if (random_integer_below(100) & 1) {
                        anim = 6;
                    }
                    blend_moby_animation(m, anim, 0, scale_game_frames(6));
                }
            }
        }
        break;
    }
    case 8: {
        {
            float dir[4];
            float out[4];
            float ang;

            approach_value(&d->fF4, d->f1A4 * frame_time, frame_time_sq * 5.0f);
            ang = FUN_001f9e90(d->v170[0] - m->pos[0], d->v170[1] - m->pos[1]);
            func_L00_002592B0_b((char *)m, ang, 0.02f, 0.3f, frame_time * 6.2831855f, &d->f190);
            if (0.87266463f < fast_difference_between_rotations(ang, m->f48)) {
                float lim = d->f1A4 * frame_time * 0.25f;
                if (lim < d->fF4) {
                    d->fF4 = lim;
                }
            }
            dir[0] = fast_cos(m->f48) * 2.0f;
            dir[1] = fast_sin(m->f48) * 2.0f;
            dir[2] = 0.0f;
            FUN_L00_00258b50(m, &d->iD0, dir, out, 1.0f);
            if (FUN_001f9b48(m->pos, d->v170) < 1.0f) {
                m->state = 3;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(0x14));
                }
            } else if (d->i164 != 2) {
                m->state = 5;
                d->i1D8 = 0;
                qcopy(d->v1C0, m->pos);
                d->fF4 = 0.0f;
                if (m->anim != 3) {
                    blend_moby_animation(m, 3, 0, scale_game_frames(7));
                }
            }
        }
        break;
    }
    case 10:
        if (FUN_L00_0025c698(m, d->v70) & 0x41) {
            int anim;
            m->state = 7;
            d->fF4 = frame_time * 5.0f;
            anim = 7;
            if (random_integer_below(100) & 1) {
                anim = 6;
            }
            blend_moby_animation(m, anim, 0, scale_game_frames(6));
        } else if (m->pos[2] < d->v170[2] - 0.2f) {
            if (m->anim != 0xE) {
                blend_moby_animation(m, 0xE, 0, scale_game_frames(6));
            }
            m->state = 0xC;
            d->f78 = d->f78 / 3.0f;
        }
        break;
    case 12: {
        {
            float tmp[4];
            int r;

            d->f78 = d->f78 - d->f80;
            FUN_L00_001ff290(tmp, m->pos, d->v70);
            if (m->pos[2] < probe_ground_height(m->pos, 0, 0.5f)) {
                int q;
                float x;
                r = collision_hit_material();
                if (r == 0 || r == 1 || r == 3) {
                    FUN_L00_00257470(m, 0x200, -1);
                    mark_moby_for_removal(m);
                    return;
                }
                m->s34 &= 0xEFFF;
                m->state = 0xB;
                q = truncate_float_to_s32(768.0f);
                x = *(float *)&D_L10_00161E28 * frame_time_sq;
                d->bAD = 0;
                d->i90 = q;
                d->f98 = 0.75f;
                d->i94 = 9;
                d->f80 = x;
                FUN_L03_00250a78(d->v70, 2.0f, 1.5f);
                d->fC0 = 7.0f;
                d->fC4 = 13.0f;
                func_L00_0025D5B0_b(m, d->v70, 0xD, 1, 0, fast_add_rotations(m->f48, 3.1415927f));
                d->b67 = 0xFA;
                FUN_L00_0025d458(m, (short *)d->q60);
                FUN_L00_00257470(m, 0, -1);
                break;
            }
        }
    }
    case 11: {
        {
            float p[4];

            {
                FUN_L10_002e3de8((char *)m);
                {
                    float *dp = D_L10_0015F580;
                    float zz = 0.0f;
                    char *pb;
                    FUN_L10_002e3a90_u(m);
                    pb = (char *)m + 0x40;
                    FUN_L10_002e3a90_u(m);
                    FUN_L00_0025f3e8(m, m->pos, -1, 1.25f, 20.0f);
                    FUN_L00_00263fd8((char *)m, 0x775, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00263fd8((char *)m, 0x77A, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00263fd8((char *)m, 0x77B, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00263fd8((char *)m, 0x77C, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    qcopy(p, m->pos);
                    p[2] += 0.55f;
                    p[0] += randf_sym(0.0f, 0.3f);
                    p[1] += randf_sym(0.0f, 0.3f);
                    FUN_L00_00263fd8((char *)m, 0x623, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00263fd8((char *)m, 0x624, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00257470(m, 0, -1);
                    mark_moby_for_removal(m);
                    return;
                }
            }
        }
    }
    }
    if (m->state == 3 || m->state == 5 || m->state == 7 || m->state == 8) {
        float g = probe_ground_height(m->pos, 0, 0.5f);
        if (g < m->pos[2]) {
            d->f78 = d->f78 - d->f80;
            if (d->f78 < frame_time * -7.0f) {
                d->f78 = frame_time * -7.0f;
            }
            m->pos[2] = m->pos[2] + d->f78;
            if (m->pos[2] < g) {
                m->pos[2] = g;
            }
            if (g < d->v170[2] - 2.0f) {
                if (m->anim != 0xE) {
                    blend_moby_animation(m, 0xE, 0, scale_game_frames(6));
                }
                m->state = 0xC;
            }
        }
    }
    if (m->b31 != 0) {
        if (FUN_001f9b48(m->pos, D_L10_00167240) < 18.0f) {
            FUN_L00_0025a120(m);
            m->b7F = 0xE;
        }
    }
}
extern int D_L10_00161E74 __attribute__((sda));
extern int D_L10_00161E78 __attribute__((sda));
extern float D_L10_00161E7C __attribute__((sda));
extern float D_L10_00161E80 __attribute__((sda));
extern float D_L10_00161E84 __attribute__((sda));
extern float D_L10_00161E88 __attribute__((sda));
extern float D_L10_00161E8C __attribute__((sda));
extern float D_L10_00161E90 __attribute__((sda));
extern float D_L10_00161E94 __attribute__((sda));
extern float D_L10_00161E98 __attribute__((sda));
extern float D_L10_00161E9C __attribute__((sda));
extern float D_L10_00161EA0 __attribute__((sda));
extern int D_L10_00161EA4 __attribute__((sda));
extern int D_L10_00161EA8 __attribute__((sda));
extern int D_L10_00161EAC __attribute__((sda));
extern int D_L10_00161EB0 __attribute__((sda));
extern int D_L10_00161EB4 __attribute__((sda));
extern int D_L10_00161EB8 __attribute__((sda));
extern int D_L10_00161EBC __attribute__((sda));
extern void FUN_L00_0025f8e0_3a90(void *, float) __asm__("FUN_L00_0025f8e0");
extern float random_float_between_3a90(float, float) __asm__("FUN_002132a8");
extern float random_angle_radians_3a90(void) __asm__("FUN_00213308");
extern float fast_cos_3a90(float) __asm__("FUN_001f9dc8");
extern float fast_sin_3a90(float) __asm__("FUN_001f9de0");
extern void scale_vector_xyz_3a90(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_vector_xyz_3a90(void *, void *, void *) __asm__("FUN_001f9a10");
extern int scale_game_frames_3a90(int) __asm__("FUN_001f96f8");
extern float scale_time_3a90(float) __asm__("FUN_001f96b0");
extern int truncate_float_3a90(float) __asm__("FUN_001fa6d0");
extern int fast_tween_color_3a90(int, int, float) __asm__("FUN_001fa6e0");
extern char *spawn_particle_3a90(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

void FUN_L10_002e3a90(struct Moby *moby) {
    float pos[4];
    float vel[4];
    float accel[4];
    float push[4];
    char *vars;
    int count;
    float *pos_ptr;
    void *src;
    int i;
    int j;
    float angle;
    float speed;
    float rise;
    float size;
    float time_base;
    int start_color;
    int end_color;
    int life;
    int fade;

    count = D_L10_00161E74;
    vars = (char *)moby->pvars;
    for (i = 0; i < count; i++) {
        pos_ptr = pos;
        src = &moby->pos;
        qcopy_nc(pos, src);
        FUN_L00_0025f8e0_3a90(pos_ptr, D_L10_00161E7C);
        pos[2] += 0.55f;
        angle = random_angle_radians_3a90();
        speed = random_float_between_3a90(D_L10_00161E88, D_L10_00161E8C);
        rise = random_float_between_3a90(D_L10_00161E90, D_L10_00161E94);
        if (*((u8 *)moby + 0x20) == 11) {
            speed *= 0.55f;
            rise *= 0.55f;
            pos[2] -= 0.2f;
        }
        vel[0] = fast_cos_3a90(angle) * (speed * D_0015ED6C);
        vel[1] = fast_sin_3a90(angle) * (speed * D_0015ED6C);
        vel[2] = 0.0f + rise * D_0015ED6C;
        scale_vector_xyz_3a90(push, vars + 0x70, 0.5f);
        push[2] = 0.0f;
        add_vector_xyz_3a90(vel, vel, push);
        qcopy(accel, vel);
        accel[2] -= D_L10_00161E98 * D_0015ED70 * (float)scale_game_frames_3a90(D_L10_00161EB8);
        for (j = 0; j < D_L10_00161E78; j++) {
            size = random_float_between_3a90(0.5f, 1.5f);
            vel[3] = size * D_L10_00161E9C;
            accel[3] = size * D_L10_00161EA0;
            FUN_L00_0025f8e0_3a90(pos, D_L10_00161E80);
            FUN_L00_0025f8e0_3a90(accel, D_L10_00161E84 * D_0015ED6C);
            start_color = fast_tween_color_3a90(D_L10_00161EA4, D_L10_00161EAC, random_float_between_3a90(0.0f, 1.0f));
            end_color = fast_tween_color_3a90(D_L10_00161EA8, D_L10_00161EB0, random_float_between_3a90(0.0f, 1.0f));
            life = scale_game_frames_3a90(D_L10_00161EB4);
            fade = scale_game_frames_3a90(D_L10_00161EB8);
            time_base = (float)D_L10_00161EBC;
            spawn_particle_3a90(pos, vel, accel, start_color, end_color, life, fade,
                truncate_float_3a90(scale_time_3a90(random_float_between_3a90(time_base * 0.5f, time_base * 2.5f))), -1);
        }
    }
}
/* Level 10 toxic crab: per-frame sight, target and state update. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E52B0), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;


typedef int u128_2E52B0 __attribute__((mode(TI)));

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V_2E52B0;

typedef struct { int count; char p4[0xC]; V_2E52B0 nodes[1]; } Path_2E52B0;

typedef struct D_2E52B0 {
    char p0[0x10];
    struct Moby *f10;
    char p14[0xC];
    float f20;
    char p24[2];
    short h26;
    char p28[0x10];
    int f38;
    char p3C[0x24];
    char a60[7];
    unsigned char b67;
    char p68[8];
    float a70[4];
    float f80;
    char p84[4];
    float f88;
    float f8C;
    int f90;
    int f94;
    float f98;
    char p9C[0x11];
    unsigned char bAD;
    char pAE[0x12];
    float fC0;
    float fC4;
    char pC8[0x58];
    V_2E52B0 a120;
    char p130[0x30];
    struct Moby *f160;
    int f164;
    char p168[8];
    V_2E52B0 a170;
    char p180[0x20];
    float f1A0;
    char p1A4[8];
    int f1AC;
    int f1B0;
    char p1B4[0x20];
    int f1D4;
} D_2E52B0;

typedef struct Hit_2E52B0 {
    char p0[0x10];
    V_2E52B0 pos;
    Moby *f20;
    char p24[4];
    unsigned char b28;
    char p29;
    unsigned short h2A;
} Hit_2E52B0;

typedef struct { char p0[0x11]; unsigned char b11; } Flags_2E52B0;

extern char *FUN_L00_0025a420(void *,int,int);
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern float dot_vectors_xyz(float) __asm__("FUN_001f9ab0");
extern float D_L10_00161E2C __attribute__((sda));
extern float D_L10_00161E40_gp __asm__("D_L10_00161E40") __attribute__((sda));
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_00213928_c(void *,void *,void *,int,int *,float *,int,int) __asm__("FUN_00213928");
extern int FUN_L00_0025ff38_c(float,char *,void *,int,int,void *,int) __asm__("FUN_L00_0025ff38");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern unsigned char D_0013D408[];
extern void FUN_L00_0025d538_c(void *,void *) __asm__("FUN_L00_0025d538");
extern void FUN_L10_002e0138_c(unsigned char *m) __asm__("FUN_L10_002e0138");
extern void blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
float absolute_float_c(float input) __asm__("func_001F99C0");
int FUN_L01_00274b78(char *a, char *o, float best);
s32 allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) __asm__("FUN_0022db10");
void FUN_L00_0025ab48_c(void *v, float *ang, float *s1, float *s2) __asm__("FUN_L00_0025ab48");
void FUN_L00_00263d40(int a, int b);
extern void func_L10_002E14F8_2E52B0(void *) __asm__("FUN_L10_002e0138");
extern float func_002140F8_2E52B0(float, float) __asm__("FUN_002132a8");
extern float func_001F9878_2E52B0(float) __asm__("FUN_001f96b0");
extern int func_001FA898_2E52B0(float) __asm__("FUN_001fa6d0");
extern int func_001F9908_2E52B0(void *) __asm__("FUN_001f9740");
extern Hit_2E52B0 *func_L00_0025B478_2E52B0(void *, int, int) __asm__("FUN_L00_0025a420");
extern int update_collision_l08(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern int func_001F9938_2E52B0(void *) __asm__("FUN_001f9770");
extern void func_0022EE28_2E52B0(int, int, int) __asm__("FUN_0022db10");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern int func_001F9850_2E52B0(int) __asm__("FUN_001f96f8");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void func_001F9BF0_2E52B0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float func_001F9B88_2E52B0(float) __asm__("FUN_001f99c0");
extern float func_L00_001FF860_2E52B0(float, float) __asm__("FUN_001f9e90");
extern void trooper_set_knockback(float *, float, float) __asm__("FUN_L03_00250a78");
extern void func_L00_0025BBA0_2E52B0(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void func_L00_0025D5B0_2E52B0(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void func_L00_0025E4B0_2E52B0(void *, void *) __asm__("FUN_L00_0025d458");
extern void func_L00_0025E590_2E52B0(void *, void *) __asm__("FUN_L00_0025d538");
extern int func_L00_00260D30_2E52B0(void *, void *, float) __asm__("FUN_L01_00274b78");
extern int func_L00_00260FB0_2E52B0(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern float func_001F9D48_2E52B0(void *, void *) __asm__("FUN_001f9b80");
extern float func_001F9D10_2E52B0(void *, void *) __asm__("FUN_001f9b48");
extern void func_00213DE0_2E52B0(void *, int, int, int) __asm__("FUN_00212f90");
extern char D_0013F350[];
extern char D_0013D50F_2E52B0[] __asm__("D_0013D408");
extern V_2E52B0 D_L10_00174380_2E52B0 __asm__("D_L10_00174300");
extern Path_2E52B0 *D_L10_001B0C30_2E52B0[] __asm__("D_L10_001B0930");

void FUN_L10_002e3ef0(void *mv) {
    Moby *m = mv;
    V_2E52B0 vA;
    V_2E52B0 vB;
    V_2E52B0 W;
    V_2E52B0 X;
    V_2E52B0 dst;
    int A;
    float B;
    float fx;
    D_2E52B0 *d = (D_2E52B0 *)m->pvars;
    Hit_2E52B0 *r;
    Moby *q;
    int v21;

    if (((Hero *)(D_0013F350))->items[0].item_id == 0x15) {
        if (((D_2E52B0 *)((Hero *)(D_0013F350))->items[0].moby->pvars)->f10 == m) {
            func_L10_002E14F8_2E52B0(m);
            if (m->state == 1) m->state = 2;
        }
    }
    if (d->f38 != 0) {
        func_L10_002E14F8_2E52B0(m);
        d->f1AC = func_001FA898_2E52B0(func_001F9878_2E52B0(func_002140F8_2E52B0(180.0f, 240.0f)));
        d->f38 = 0;
    }
    func_001F9908_2E52B0(&d->f1AC);
    A = 1;
    B = 0.0f;
    r = func_L00_0025B478_2E52B0(m, 0xB30000, 0);
    v21 = update_collision_l08(m, r, &d->f20, 0, &A, &B, 0, 4);
    func_001F9938_2E52B0(&d->h26);
    if ((unsigned int)A >= 2 && m->state != 0xB && m->state != 0xC) {
        if (current_level_index == 10 && r != 0 && r->f20 != 0 && r->f20->oclass == 0x31A) {
            if (d->f1AC != 0 && ((Flags_2E52B0 *)D_0013D50F_2E52B0)->b11 == 0) {
                ((Flags_2E52B0 *)D_0013D50F_2E52B0)->b11 = 1;
                func_0022EE28_2E52B0(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        if (d->h26 != 0 && r->b28 == 0) {
            v21 = 11;
            if (B < 3.0f) B = 0.0f;
        }
        if (((Hero *)(D_0013F350))->state.current == 32
            || (((Hero *)(D_0013F350))->state.prev == 32
                && ((Hero *)(D_0013F350))->state_timer < func_001F9850_2E52B0(30))) {
            if (2.0f < B) B = 2.0f;
        }
        if (r->h2A == m->oclass) {
            B = 0.0f;
            v21 = 11;
        }
        if ((unsigned int)(m->prev_seq - 6) < 2) {
            if (((Hero *)(D_0013F350))->vehicle_mode == 1
                && r->f20 == ((Hero *)(D_0013F350))->moby) {
                B = 0.0f;
                v21 = 11;
            }
        }
        if (r->h2A == 0x47) {
            *(u128_2E52B0 *)&vA = *(u128_2E52B0 *)(D_0013F350 + 0x80);
            *(u128_2E52B0 *)&vB = *(u128_2E52B0 *)((V_2E52B0 *)&m->pos);
            vA.z += 1.0f;
            vB.z += 1.0f;
            if (collision_line(&vA, &vB, 2, m, 0) != 0) {
                v21 = 11;
                B = 0.0f;
            }
        } else if ((q = r->f20) != 0 && q->oclass != 0x359) {
            *(u128_2E52B0 *)&vA = *(u128_2E52B0 *)((V_2E52B0 *)&q->pos);
            *(u128_2E52B0 *)&vB = *(u128_2E52B0 *)((V_2E52B0 *)&m->pos);
            vB.z += 1.0f;
            if (r->f20->oclass == 0xBA || r->f20->oclass == 0) vA.z += 0.25f;
            if (collision_line(&vA, &vB, 0x12, m, 0) != 0) {
                func_001F9BF0_2E52B0(&dst, &vB, &vA);
                *(u128_2E52B0 *)&W = *(u128_2E52B0 *)&dst;
                fast_vec_normalize(&W, &W, 1.0f);
                fast_vec_normalize(&X, &D_L10_00174380_2E52B0, 1.0f);
                if (0.5f < func_001F9B88_2E52B0(fast_vec_dot(&W, &X))) {
                    B = 0.0f;
                    v21 = 11;
                }
            }
        }
        d->f20 -= B;
        if (d->f20 <= 0.0f) v21 = 1;
        func_L10_002E14F8_2E52B0(m);
        d->f90 = func_001FA898_2E52B0(768.0f);
        d->f98 = 0.75f;
        d->f94 = 9;
        d->bAD = 0;
        fx = func_L00_001FF860_2E52B0(r->pos.x, r->pos.y);
        switch (v21) {
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            m->state = 10;
            d->h26 = func_001F9850_2E52B0(60);
            d->f80 = D_L10_00161E2C * D_0015ED70;
            trooper_set_knockback(d->a70, 0.5f, 0.5f);
            d->fC0 = 8.0f;
            d->fC4 = 10.0f;
            *(u128_2E52B0 *)&vA = *(u128_2E52B0 *)&r->pos;
            func_L00_0025BBA0_2E52B0(&vA, &fx, &d->f88, &d->f8C);
            func_L00_0025D5B0_2E52B0(m, d->a70, 9, 1, 0, fx);
            d->b67 = 0xFA;
            func_L00_0025E4B0_2E52B0(m, d->a60);
            break;
        case 4:
        case 5: {
            float s;
            m->state = 10;
            d->h26 = func_001F9850_2E52B0(60);
            d->f80 = D_L10_00161E2C * D_0015ED70;
            s = ((Hero *)(D_0013F350))->vehicle_mode == 1 ? 1.0f : 3.0f;
            trooper_set_knockback(d->a70, s, 0.5f);
            d->fC0 = 3.5f;
            d->fC4 = 7.0f;
            *(u128_2E52B0 *)&vA = *(u128_2E52B0 *)&r->pos;
            func_L00_0025BBA0_2E52B0(&vA, &fx, &d->f88, &d->f8C);
            func_L00_0025D5B0_2E52B0(m, d->a70, 9, 1, 0, func_L00_001FF860_2E52B0(r->pos.x, r->pos.y));
            d->b67 = 0xFA;
            func_L00_0025E4B0_2E52B0(m, d->a60);
            break;
        }
        case 1:
        case 2:
            m->state = 11;
            m->flags &= 0xEFFF;
            break;
        case 0:
        case 11:
            break;
        }
    }
    m->hit_slot = 0xFF;
    func_L00_0025E590_2E52B0(m, d->a60);
    if ((unsigned char)(m->state - 10) < 3) {
        d->f1D4 = 0;
        return;
    }
    if (d->f1AC != 0) {
        d->f1A0 = D_L10_00161E40_gp + 6.0f;
    } else if (m->state == 1) {
        if (m == ((Moby *)((Hero *)(D_0013F350))->unk240) || m == ((Moby *)((Hero *)(D_0013F350))->coll_hit_moby)) {
            d->f1A0 = 6.0f;
        } else {
            d->f1A0 = 0.0f;
        }
        if (((Hero *)(D_0013F350))->vehicle_mode == 1) func_L10_002E14F8_2E52B0(m);
    } else {
        d->f1A0 = D_L10_00161E40_gp;
    }
    if (d->f1AC != 0) {
        func_L00_00260D30_2E52B0(m, &d->a120, d->f1A0);
        if (d->f1A0 < func_001F9D48_2E52B0(&d->a170, &d->a120) || 3.0f < func_001F9B88_2E52B0(m->pos.z - d->a120.z)) {
            d->f164 = 2;
        }
    } else if (d->f1A0 < func_001F9D48_2E52B0(&d->a170, &d->a120) || 3.0f < func_001F9B88_2E52B0(m->pos.z - d->a120.z)) {
        func_L00_00260D30_2E52B0(m, &d->a120, d->f1A0);
        d->f164 = 2;
    } else {
        func_L00_00260FB0_2E52B0(d->f1A0, m, &d->a120, 0, 0, D_L10_001B0C30_2E52B0[d->f1B0]->nodes,
            D_L10_001B0C30_2E52B0[d->f1B0]->count);
    }
    if (m->state != 1 && d->f164 == 2) {
        d->f1D4++;
        if (func_001F9850_2E52B0(300) < d->f1D4) {
            if (1.0f < func_001F9D10_2E52B0(&m->pos, &d->a170)) {
                m->state = 8;
                if (m->prev_seq != 4) func_00213DE0_2E52B0(m, 4, 0, func_001F9850_2E52B0(12));
            } else if (((Hero *)(D_0013F350))->vehicle_mode != 1) {
                m->state = 1;
                if (m->prev_seq != 0) func_00213DE0_2E52B0(m, 0, 0, func_001F9850_2E52B0(20));
            }
        }
    } else {
        d->f1D4 = 0;
    }
    if (d->f160 == 0) d->f160 = ((Hero *)(D_0013F350))->moby;
}
#define NOT_SDA

#define MACRO_ADDR

/* Computes the aim angle of a moby relative to its owner, narrowed by nearby table entries. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E5C00), where it is exact; names translated to the US level program. */

extern Ent *D_L10_0015FFD0_e[] __asm__("D_L10_0015FFD0") __attribute__((section(".sdata")));
extern float FUN_001f9b48(void *, void *);
extern float FUN_001f9dc8(float);
extern float FUN_001f9de0(float);
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float FUN_001f9e90(float, float);
extern unsigned char D_0013E533[];
extern void FUN_001f9a10(void *, void *, void *);

extern short D_L10_00161E48 __attribute__((sda));
float FUN_L10_002e4840(struct Moby *self) {
    char *owner = *(char **)(self->pvars + 0x160);
    float hi, lo, ang, dist, v[3];
    short *p;
    if (self->group == 0xFF) {
        return FUN_001f9e90(self->pos.x - *(float *)(owner + 0x10),
                            self->pos.y - *(float *)(owner + 0x14));
    }
    lo = -(*(float *)&D_L10_00161E48 * DEG_TO_RAD);
    hi = *(float *)&D_L10_00161E48 * DEG_TO_RAD;
    ang = FUN_001f9e90(self->pos.x - *(float *)(owner + 0x10),
                       self->pos.y - *(float *)(owner + 0x14));
    dist = FUN_001f9b48(owner + 0x10, &self->pos);
    v[0] = FUN_001f9dc8(ang) * 2.0f;
    v[1] = FUN_001f9de0(ang) * 2.0f;
    v[2] = 0.0f;
    FUN_001f9a10(v, v, D_0013E533 + 0xE9D);
    p = D_L10_001ABCC0[self->group];
    if (p != 0) {
        do {
            int idx = *(unsigned short *)p & 0x7FFF;
            if (D_L10_0015FFD0_e[2][idx].state == 5 ||
                FUN_001f9b48(&D_L10_0015FFD0_e[2][idx].x, owner + 0x10) < dist) {
                float d = FUN_001f9e90(D_L10_0015FFD0_e[2][idx].x - *(float *)(owner + 0x10),
                                       D_L10_0015FFD0_e[2][idx].y - *(float *)(owner + 0x14));
                float r = fast_subtract_rotations(d, ang);
                if (r > 0.0f && r < hi) {
                    hi = r;
                } else if (r < 0.0f && r > lo) {
                    lo = r;
                }
            }
        } while (*p++ >= 0);
    }
    return fast_add_rotations(
        fast_add_rotations(lo, fast_difference_between_rotations(hi, lo) * 0.5f), ang);
}
/* Update for the blarg ship (moby class 1229): rise, follow its path, hover and fire at the target, fly off or explode. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E5E48), where it is exact; names translated to the US level program. */

typedef struct Ship_5E48 {
    char p00[0x10];
    V_2E52B0 pos;
    unsigned char state;
    char p21[3];
    char *p24;
    char p28[4];
    float f2C;
    char p30;
    unsigned char b31;
    short s32;
    unsigned short s34;
    char p36[0xE];
    float f44;
    float f48;
    char p4C[6];
    unsigned char b52;
    unsigned char b53;
    char p54[4];
    float f58;
    char p5C[0x14];
    unsigned char b70;
    char p71[7];
    struct ShipD_5E48 *d;
    char p7C[3];
    unsigned char b7F;
    char p80[0x14];
    int i94;
    char p98[0xE];
    short sA6;
    unsigned short hA8;
    char pAA[6];
    unsigned char bB0;
    char pB1[0xB];
    unsigned char bBC;
} Ship_5E48;

typedef struct ShipD_5E48 {
    char p0[0x20];
    float f20;
    short s24;
    char p26[2];
    unsigned char b28;
    unsigned char b29;
    char p2A[0x16];
    char a40[0xB0];
    V_2E52B0 vF0;
    char p100[0x10];
    Ship_5E48 *tgt;
    int i114;
    char p118[8];
    V_2E52B0 a120;
    char p130[0x10];
    int i140;
    int i144;
    int i148;
    char p14C[0xC];
    int i158;
    char p15C[4];
    float f160;
    float f164;
    int i168;
    char p16C[0x2A];
    short s196;
    float f198;
    float f19C;
    char p1A0[0x10];
    int i1B0;
    char p1B4[0x1C];
    unsigned char a1D0[4];
} ShipD_5E48;

extern Ship_5E48 *D_L10_001742D8 __attribute__((section(".data")));
extern char *FUN_L10_002cc1b8(void *vel, void *pos, int a, int b, int c, float scale);
extern char D_L10_00179890[];
extern char D_L10_001DD390[];
extern f32 fast_arc_tan(f32, f32) __asm__("func_001F9E90");
extern f32 fast_vec_length_c(void *) __asm__("func_001F9AF0");
extern float D_0015ED60;
extern float FUN_L00_00258110_c(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float fast_difference_between_rotations_c2(float, float) __asm__("func_001FA688");
extern float fast_vec_dist(void *, void *);
extern float probe_ground_height_c2(void *, int, float) __asm__("func_00213508");
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int FUN_L00_00203908_c(int a, int b) __asm__("FUN_L00_00203908");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
extern s32 D_0015ED84;
extern s32 scale_game_frames_c2(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32_c2(f32) __asm__("func_001FA6D0");
extern short D_L10_00161EE4 __attribute__((sda));
extern short D_L10_00161EE8 __attribute__((sda));
extern short D_L10_00161EEC __attribute__((sda));
extern unsigned char D_0013D4CB[];
extern unsigned char D_001413F4[];
extern unsigned char D_0014161B[];
extern unsigned char D_00141C48[];
extern void *FUN_L00_00263fd8_c(char *, int, float *, void *, int, int, float, float *, float *, float *) __asm__("FUN_L00_00263fd8");
extern void DebugPrint(char *, ...);
extern void FUN_L00_0025e450(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int);
extern void FUN_L00_0025f730_c(void *, float) __asm__("FUN_L00_0025f730");
extern void FUN_L03_00250ae8(void *, void *, int);
extern void FUN_L10_002e5968_c(char *m) __asm__("FUN_L10_002e5968");
extern void add_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void build_spherical_offset_c(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void fast_vec_normalize_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
s32 is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
extern int D_0015ED84;
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void FUN_L10_002e4a88(Ship_5E48 *m) {
    ShipD_5E48 *d = m->d;
    Ship_5E48 *tgt;
    V_2E52B0 A;
    V_2E52B0 B;
    V_2E52B0 C;
    V_2E52B0 E;
    V_2E52B0 D;
    float a;
    float b;

    FUN_L00_00263ac8(2.5f, (int)m, 2, d->a1D0);
    if (d->i140 == -1) {
        DebugPrint(D_L10_001DD390, m->sA6, m->hA8);
        goto del;
    }
    FUN_L10_002e5968_c((char *)m);
    tgt = d->tgt;
    switch (m->state) {
    case 0:
        m->s34 |= 0x1000;
        d->f20 = 9.0f;
        d->s24 = truncate_float_to_s32_c2(9.0f);
        d->b28 = 3;
        d->b29 = 0;
        d->f19C = 0.0f;
        m->s32 = 0x7F;
        qcopy(&m->pos, &D_L10_001B0C30_2E52B0[d->i140]->nodes[0]);
        d->s196 = scale_game_frames_c2(*(int *)&D_L10_00161EE4);
        m->i94 = 0;
        m->s34 |= 0x41;
        if (m->b53 != 0) blend_moby_animation(m, 0, 0, 0);
        m->state = 1;
        if (D_0014C050[m->bB0 + (D_0015ED84 << 4)] != 0xFF) break;
        goto del;
    case 1:
        if (!is_point_inside_clip_volume(&tgt->pos, d->i148)) break;
        m->state = 2;
        if (m->b53 != 1) blend_moby_animation(m, 1, 0, scale_game_frames_c2(20));
        m->s34 &= 0xFFBE;
        m->i94 = *(int *)(m->p24 + 0x10);
        if (*(int *)D_L10_00179890 == 0 && *(int *)(D_L10_00179890 + 0x24) == -1
            && D_001413F4[0] == 0 && *(unsigned short *)D_00141C48 == 0
            && D_0013D4CB[0] == 0) {
            FUN_L00_00203908_c(0x2715, 0x5C);
        }
        break;
    case 2:
        a = fast_arc_tan(tgt->pos.x - m->pos.x, tgt->pos.y - m->pos.y);
        FUN_L00_00258278((char *)m, &d->f198, a, 0.02f, 0.3f, D_0015ED6C * 3.1415927f);
        b = fast_arc_tan(vector_distance_xy_alt(&m->pos, &tgt->pos), tgt->pos.z - m->pos.z);
        m->f44 = FUN_L00_00258110_c(&d->f19C, m->f44, -b, 0.02f, 0.3f, D_0015ED6C * 3.1415927f);
        if ((m->b70 & 2) && m->b53 == 1) blend_moby_animation(m, 2, 0, scale_game_frames_c2(20));
        m->pos.z += D_0015ED6C * 4.0f;
        if (fast_vec_dist(&m->pos, &D_L10_001B0C30_2E52B0[d->i140]->nodes[0]) < 0.5f) {
            m->state = 3;
            if (m->b53 != 2) blend_moby_animation(m, 2, 0, scale_game_frames_c2(10));
        }
        break;
    case 3:
        qcopy(&A, &D_L10_001B0C30_2E52B0[d->i140]->nodes[d->i158]);
        a = fast_arc_tan(A.x - m->pos.x, A.y - m->pos.y);
        FUN_L00_00258278((char *)m, &d->f198, a, 0.02f, 0.3f, D_0015ED6C * 12.566371f);
        b = fast_arc_tan(vector_distance_xy_alt(&m->pos, &tgt->pos), tgt->pos.z - m->pos.z);
        m->f44 = FUN_L00_00258110_c(&d->f19C, m->f44, -b, 0.02f, 0.3f, D_0015ED6C * 3.1415927f);
        func_001F9BF0_2E52B0(&B, &A, &m->pos);
        fast_vec_normalize(&d->a120, &B, D_0015ED6C * 8.0f);
        add_vector_xyz_c3(&m->pos, &m->pos, &d->a120);
        if (fast_vec_length_c(&B) < 0.5f) {
            int n = d->i158 + 1;
            if (n < D_L10_001B0C30_2E52B0[d->i140]->count) {
                d->i158 = n;
            } else {
                d->i158 = 0;
                m->state = 4;
                d->i168 = truncate_float_to_s32_c2(FUN_001f96b0(d->f160 * 60.0f));
            }
        }
        break;
    case 4:
        if (is_point_inside_clip_volume(&(&hero)->motion.pos, d->i1B0)) {
            m->state = 6;
            break;
        }
        *(u128_2E52B0 *)&A = *(u128_2E52B0 *)&tgt->pos;
        if ((&hero)->vehicle_mode != 1) {
            *(u128_2E52B0 *)&A = *(u128_2E52B0 *)&d->vF0;
            FUN_001efa68(&m->pos, &A, 0, m, 0);
        } else {
            if ((Ship_5E48 *)(&hero)->moby == tgt) {
                fast_vec_scale(&C, &(&hero)->motion.displacement, (float)scale_game_frames_c2(20));
                *(u128_2E52B0 *)&B = *(u128_2E52B0 *)&C;
                FUN_L00_001ff290(&C, &A, &B);
                A.z = probe_ground_height_c2(&A, 0, 0.5f);
            }
            A.z += 0.2f;
            D_L10_001742D8 = tgt;
        }
        a = fast_arc_tan(tgt->pos.x - m->pos.x, tgt->pos.y - m->pos.y);
        FUN_L00_00258278((char *)m, &d->f198, a, 0.02f, 0.3f, D_0015ED6C * 12.566371f);
        b = fast_arc_tan(vector_distance_xy_alt(&m->pos, &A), A.z - m->pos.z);
        m->f44 = FUN_L00_00258110_c(&d->f19C, m->f44, -b, 0.02f, 0.3f, D_0015ED6C * 3.1415927f);
        if (fast_difference_between_rotations_c2(m->f48, fast_arc_tan(tgt->pos.x - m->pos.x, tgt->pos.y - m->pos.y)) < 0.17453292f) {
            if (m->b53 != 3) blend_moby_animation(m, 3, 0, scale_game_frames_c2(6));
            m->f58 = *(float *)&D_L10_00161EEC;
        }
        if (m->b52 == 3 && d->i114 != 2 && D_L10_001742D8 == tgt) {
            int near = is_value_within_interpolated_window((char *)m, 1.0f);
            int far = is_value_within_interpolated_window((char *)m, 16.0f);
            if (near || far) {
                FUN_L00_0024f7c8(m, far == 0, &C);
                build_spherical_offset_c(&B, 1.0f, m->f48, -m->f44);
                FUN_L00_001ff290(&D, &C, &B);
                func_001F9BF0_2E52B0(&B, &A, &C);
                fast_vec_normalize(&B, &B, *(float *)&D_L10_00161EE8 * D_0015ED6C);
                fast_vec_scale(&E, &B, FUN_001f96b0(3.0f));
                FUN_L00_001ff290(&D, &E, &C);
                FUN_L03_00250ae8(m, &E, 0);
                FUN_L10_002cc1b8(&B, &C, (int)m, (int)tgt, scale_game_frames_c2(300), 1.0f);
            }
        } else if (m->b52 == 3 && m->b53 != 2) {
            blend_moby_animation(m, 2, 0, scale_game_frames_c2(10));
        }
        if (tick_countdown_32_alt(&d->i168)) {
            if (m->b53 != 2) blend_moby_animation(m, 2, 0, scale_game_frames_c2(10));
            m->state = 5;
            d->i168 = truncate_float_to_s32_c2(FUN_001f96b0(d->f164 * 60.0f));
        }
        qcopy(&B, &D_L10_001B0C30_2E52B0[d->i144]->nodes[d->i158]);
        func_001F9BF0_2E52B0(&C, &B, &m->pos);
        FUN_L00_0025f730_c(&C, D_0015ED6C * 8.0f);
        add_vector_xyz_c3(&m->pos, &m->pos, &C);
        if (fast_vec_length_c(&C) < D_0015ED6C) {
            int n = d->i158 + 1;
            if (n < D_L10_001B0C30_2E52B0[d->i144]->count) d->i158 = n;
        }
        break;
    case 5:
        if (is_point_inside_clip_volume(&(&hero)->motion.pos, d->i1B0)) {
            m->state = 6;
            break;
        }
        *(u128_2E52B0 *)&A = *(u128_2E52B0 *)&tgt->pos;
        if ((&hero)->vehicle_mode != 1) {
            *(u128_2E52B0 *)&A = *(u128_2E52B0 *)&d->vF0;
            FUN_001efa68(&m->pos, &A, 0, m, 0);
        } else {
            if ((Ship_5E48 *)(&hero)->moby == tgt) {
                fast_vec_scale(&C, &(&hero)->motion.displacement, (float)scale_game_frames_c2(20));
                *(u128_2E52B0 *)&B = *(u128_2E52B0 *)&C;
                FUN_L00_001ff290(&C, &A, &B);
                A.z = probe_ground_height_c2(&A, 0, 0.5f);
            }
            A.z += 0.2f;
            D_L10_001742D8 = tgt;
        }
        a = fast_arc_tan(tgt->pos.x - m->pos.x, tgt->pos.y - m->pos.y);
        FUN_L00_00258278((char *)m, &d->f198, a, 0.02f, 0.3f, D_0015ED6C * 12.566371f);
        b = fast_arc_tan(vector_distance_xy_alt(&m->pos, &A), A.z - m->pos.z);
        m->f44 = FUN_L00_00258110_c(&d->f19C, m->f44, -b, 0.02f, 0.3f, D_0015ED6C * 3.1415927f);
        if (tick_countdown_32_alt(&d->i168)) {
            blend_moby_animation(m, 3, 0, scale_game_frames_c2(10));
            m->f58 = *(float *)&D_L10_00161EEC;
            m->state = 4;
            d->i168 = truncate_float_to_s32_c2(FUN_001f96b0(d->f160 * 60.0f));
        }
        qcopy(&B, &D_L10_001B0C30_2E52B0[d->i144]->nodes[d->i158]);
        func_001F9BF0_2E52B0(&C, &B, &m->pos);
        FUN_L00_0025f730_c(&C, D_0015ED6C * 8.0f);
        add_vector_xyz_c3(&m->pos, &m->pos, &C);
        if (fast_vec_length_c(&C) < D_0015ED6C) {
            int n = d->i158 + 1;
            if (n < D_L10_001B0C30_2E52B0[d->i144]->count) d->i158 = n;
        }
        break;
    case 6:
        m->pos.z += D_0015ED6C * 10.0f;
        m->f2C *= D_0015ED60 * -0.015299976f + 1.0f;
        if (250.0f < m->pos.z) goto del;
        if (!(m->f2C < 0.01f)) break;
        goto del;
    case 7: {
        unsigned char next;
        if (m->b52 != 4) break;
        if (!(m->b70 & 2)) break;
        next = m->bBC;
        if (next == 4) {
            blend_moby_animation(m, 3, 0, scale_game_frames_c2(10));
            m->f58 = *(float *)&D_L10_00161EEC;
            m->state = next;
            d->i168 = truncate_float_to_s32_c2(FUN_001f96b0(d->f160 * 60.0f));
        } else if (next == 6) {
            blend_moby_animation(m, 2, 0, scale_game_frames_c2(10));
            m->state = next;
        } else {
            blend_moby_animation(m, 2, 0, scale_game_frames_c2(10));
            m->state = 5;
            d->i168 = truncate_float_to_s32_c2(FUN_001f96b0(d->f164 * 60.0f));
        }
        break;
    }
    case 8: {
        float z = 0.0f;
        FUN_L00_0025e450(m, d->a40, &m->pos, z, z, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, 5, 15.0f, 1, 1, -1, 0);
        FUN_L00_00257470(m, 0, -1);
        FUN_L00_00263fd8_c((char *)m, 0x5FD, (float *)&m->pos, (char *)m + 0x40, 0, 0, 0.0f, D_L10_0015F580, D_L10_0015F580, D_L10_0015F580);
        FUN_L00_00263fd8_c((char *)m, 0x5FE, (float *)&m->pos, (char *)m + 0x40, 0, 0, 0.0f, D_L10_0015F580, D_L10_0015F580, D_L10_0015F580);
        mark_moby_for_removal_c2(m);
        return;
    del:
        mark_moby_for_removal_c2(m);
        return;
    }
    }
    if (m->b31 != 0) {
        if (fast_vec_dist(&m->pos, D_L10_00167240) < 28.0f) {
            FUN_L00_0025a120(m);
            m->b7F = 0x16;
        }
    }
}
/* Hit handling and path following for a level 10 vendor moby. */
extern char *info_5968(void *, int, int) __asm__("FUN_L00_0025a420");
extern void seek_5968(float, void *, void *) __asm__("FUN_L06_002f34d8");
extern int absolute_float_cf(void *, void *, int) __asm__("FUN_L00_00259740");
extern int follow_5968(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern char *D_L10_001B0930_5968[] __asm__("D_L10_001B0930");

void FUN_L10_002e5968(struct Moby *m) {
    unsigned char *d = m->pvars;
    char *info = info_5968(m, 0x370000, 0);
    int out;
    float dmg;
    int r;
    dmg = 0.0f;
    r = fun_00213928(m, info, d + 0x20, 5, &out, &dmg, 0, 4);
    if (info != 0 && out != 1 && m->state != 8) {
        if ((*(float *)(d + 0x20) -= dmg) <= 0.0f) {
            r = 1;
        }
        if (*(int *)(info + 0x24) & 0x40000) {
            r = 1;
        }
        switch (r) {
        case 9:
        case 10:
        case 12:
            d[0xC7] = 0xFA;
            trooper_anim_update(m, d + 0xC0);
            break;
        case 1:
            d[0xC7] = 0xFA;
            trooper_anim_update(m, d + 0xC0);
            m->state = 8;
            break;
        case 13:
            break;
        default:
            d[0xC7] = 0xFA;
            *(short *)(d + 0x26) = scale_game_frames(0x3C);
            trooper_anim_update(m, d + 0xC0);
            blend_moby_animation(m, 4, 0, scale_game_frames(6));
            m->command = m->state;
            m->state = 7;
            break;
        }
    }
    m->hit_slot = 0xFF;
    trooper_anim_finish(m, d + 0xC0);
    if (hero.vehicle_mode == 1) {
        seek_5968(101.0f, m, d + 0x110);
        if (*(char **)(d + 0x110) == 0) {
            *(int *)(d + 0x114) = 2;
        } else {
            char *path = D_L10_001B0930_5968[*(int *)(d + 0x190)];
            if (absolute_float_cf(*(char **)(d + 0x110) + 0x10, path + 0x10, *(int *)path) == 0) {
                *(int *)(d + 0x110) = 0;
                *(int *)(d + 0x114) = 2;
            } else {
                *(int *)(d + 0x114) = 0;
            }
        }
    } else {
        char *path = D_L10_001B0930_5968[*(int *)(d + 0x190)];
        if (follow_5968(101.0f, m, d + 0xD0, 0, 0, path + 0x10, *(int *)path) != 2 && m->visible == 0) {
            *(int *)(d + 0x114) = 2;
        }
    }
    if (*(int *)(d + 0x110) == 0) {
        *(int *)(d + 0x110) = (int)hero.moby;
    }
}
extern s32 D_L10_00161EF0 __attribute__((sda));
extern s32 D_L10_00161EF4 __attribute__((sda));
extern f32 D_L10_00161EF8 __attribute__((sda));
extern f32 D_L10_00161EFC __attribute__((sda));
extern f32 D_L10_00161F00 __attribute__((sda));
extern s32 D_L10_00161F04 __attribute__((sda));
extern f32 frame_time_absolute_4a88 __asm__("D_L10_0015ED6C") __attribute__((section(".sdata")));
extern f32 multiply_global_factor_ed64(f32) __asm__("FUN_L10_001f2578");
extern void scale_vector_xyz(f32 *, f32 *, f32) __asm__("FUN_001f9a68");
extern void build_spherical_offset(f32 *, f32, f32, f32) __asm__("FUN_00214db0");
extern s32 fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern unsigned char *FUN_L00_002738e8(void *, void *, s32, f32, f32, f32, s32, s32, s32);

void FUN_L10_002e5be0(struct Moby *moby) {
    f32 *pvars = (f32 *)moby->pvars;
    Vec4 *moby_position;
    s32 i = 0;

    moby->update_dist = 0x80;
    if (((s32 *)pvars)[3] > 0) {
        moby_position = (Vec4 *)&moby->pos.x;
        for (; i < ((s32 *)pvars)[3]; ++i) {
            Vec4 position;
            Vec4 velocity;
            Vec4 offset;
            f32 amount;
            f32 radius;
            f32 x;
            f32 y;
            f32 z;
            s32 color;
            s32 fade_color;
            s32 first_color;
            s32 life;
            s32 variant;

            /* Save the loop counter while selecting the particle variant. */
            life = i;
            i = random_integer_below(100);
            variant = i < D_L10_00161F04;
            i = life;
            qcopy(&position, moby_position);
            offset.f[0] = fast_cos(fast_add_rotations(moby->rot.z, 1.5707964f)) *
                            random_float_between(-pvars[4], pvars[4]);
            offset.f[1] = fast_sin(fast_add_rotations(moby->rot.z, 1.5707964f)) *
                            random_float_between(-pvars[4], pvars[4]);
            *(volatile f32 *)&offset.f[2] = 0.0f;
            offset.f[2] = random_float_between(-pvars[5], pvars[5]);
            add_vector_xyz(&position, &position, &offset);

            x = random_float_between(D_L10_00161EFC, D_L10_00161EF8);
            y = random_float_between(D_L10_00161EFC, D_L10_00161EF8);
            z = random_float_between(D_L10_00161EFC, D_L10_00161EF8);
            radius = x;
            if (radius < y) radius = y;
            if (radius < z) radius = z;
            build_spherical_offset(velocity.f, radius * frame_time_absolute_4a88, moby->rot.z,
                                   -moby->rot.y);

            if (variant) {
                scale_vector_xyz(velocity.f, velocity.f, random_float_between(0.5f, 1.5f));
                x = random_float_between(0.24f, 0.36f);
                amount = pvars[1] * x;
            } else {
                x = random_float_between(0.8f, 1.2f);
                amount = pvars[1] * x;
            }
            x = random_float_between(0.5f, 1.0f);
            color = fast_tween_color(x, D_L10_00161EF0, D_L10_00161EF4);
            first_color = D_L10_00161EF4 & 0xff000000;
            x = random_float_between(0.25f, 0.5f);
            fade_color = fast_tween_color(x, D_L10_00161EF4, first_color);
            life = truncate_float_to_s32(multiply_global_factor_ed64(pvars[2] * 60.0f));
            FUN_L00_002738e8(&position, &velocity, life, amount, pvars[0],
                               D_L10_00161F00 * frame_time_sq, color, fade_color, variant);
        }
    }
}

/* Health upgrade vendor: sells two upgrades (4000 bolts: 5 hp, 30000 bolts: 8 hp) once each,
   then saves the game and shows the purchase message. */
extern void tick_countdown_32_vendor(void *) __asm__("FUN_001f9740");
extern void FUN_L00_00266858_vendor(struct Moby *, int) __asm__("FUN_L00_00266858");
extern int try_set_help_message_vendor(int, int) __asm__("FUN_00215130");
extern void FUN_L00_00298840_vendor(int) __asm__("FUN_L00_00298840");
extern int memcard_save_data_vendor(int, int) __asm__("FUN_0020b178");
extern void FUN_L00_00263d40_vendor(int, int) __asm__("FUN_L00_00263d40");
extern int D_0013ED98 __attribute__((section(".sdata")));
extern int D_0013EDA0 __attribute__((sda));
extern int D_L10_0013F640 __asm__("D_L10_0013F640") __attribute__((section(".sdata")));

void FUN_L10_002e8358(struct Moby *moby) {
    int *timer;
    float angle;
    int purchase;
    int upgrade_level;

    timer = (int *)moby->pvars;
    tick_countdown_32_vendor(timer + 1);
    if (moby->state != 0) {
        if (moby->state == 1) goto state_one;
        return;
    }
    {
        angle = FUN_001f9e90(hero.motion.pos.f[0] - moby->pos.x,
                              hero.motion.pos.f[1] - moby->pos.y);
        if (fast_difference_between_rotations(moby->rot.z, angle) < 1.5707964f &&
            FUN_001f9b80(&moby->pos, hero.motion.pos.f) < 4.0f &&
            timer[1] == 0 && D_L10_0015F5C4_vendor != 2) {
            FUN_L00_00266858_vendor(moby, 1);
            if (alternate_item_available[4] == 0) {
                if (current_bolt_count >= 4001) {
                    int accepted = try_set_help_message_vendor(1, 0x271e) != 0;
                    if (!(D_0013CAE4 & 0x10)) return;
                    if (accepted == 0) return;
                    alternate_item_available[4] = 1;
                    upgrade_level = 5;
                    hero.health.hp = upgrade_level;
                    current_bolt_count = D_0013ED98 - 4000;
                    D_0013EDA0 = upgrade_level;
                    FUN_L00_00298840_vendor(0);
                    moby->state = 1;
                } else {
                    try_set_help_message_vendor(1, 0x2720);
                    return;
                }
            } else {
                if (alternate_item_available[5] != 0 || timer[1] != 0) return;
                if (current_bolt_count >= 30001) {
                    int accepted = try_set_help_message_vendor(1, 0x271f) != 0;
                    if (!(D_0013CAE4 & 0x10)) return;
                    if (accepted == 0) return;
                    alternate_item_available[5] = 1;
                    upgrade_level = 8;
                    hero.health.hp = upgrade_level;
                    current_bolt_count = D_0013ED98 - 30000;
                    D_0013EDA0 = upgrade_level;
                    FUN_L00_00298840_vendor(1);
                    moby->state = 1;
                } else {
                    try_set_help_message_vendor(1, 0x2721);
                    return;
                }
            }

        }
    }
    return;
state_one:
    if (D_L10_0015F5C4_vendor != 2) {
        memcard_save_data_vendor(0, -1);
        switch (alternate_item_available[5]) {
        default: purchase = 0x271d; break;
        case 1: purchase = 0x271d; break;
        case 0: purchase = 0x271c; break;
        }
        FUN_L00_00263d40_vendor(purchase, -1);
        D_L10_0013F640 = 0xb4;
        timer[1] = scale_game_frames(0xf0);
        moby->state = 0;
    }
}
/* Level 10 moby update (class 1344): tracks nearby targets and fires the level's event messages. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E9978), where it is exact; names translated to the US level program. */

typedef struct EvData_2E9978 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
} EvData_2E9978;

typedef struct HeroPos_2E9978 {
    char pad0[0x200C];
    unsigned int f200C;
} HeroPos_2E9978;

typedef struct Msg_2E9978 {
    int f0;
    char pad4[0x20];
    int f24;
} Msg_2E9978;

typedef struct Save_2E9978 {
    char pad0[0x358];
    unsigned short f358;
    unsigned short f35A;
    int f35C;
} Save_2E9978;

typedef struct Flags_2E9978 {
    char pad0[4];
    unsigned char f4;
    unsigned char f5;
} Flags_2E9978;

extern unsigned char D_0013F3D0_r[] __asm__("D_0013F3D0");
extern unsigned char D_0013D388_r[] __asm__("D_0013D388");
extern unsigned char D_00141B20_r[] __asm__("D_00141B20");
extern unsigned char D_00141B28_r[] __asm__("D_00141B28");
extern unsigned char D_0013D504_r[] __asm__("D_0013D504");
extern unsigned char D_00141968_r[] __asm__("D_00141968");
extern Moby *D_L10_0015FFD8_c2 __asm__("D_L10_0015FFD8");
extern Moby *D_L10_0015FFD0_m[] __asm__("D_L10_0015FFD0") __attribute__((section(".sdata")));
extern int D_0013D388; /* no foreign declaration */
extern int D_00141B28; /* no foreign declaration */
extern int D_0015EEA4_gp __asm__("D_0015EEA4") __attribute__((sda)); /* no foreign declaration */
extern Moby *D_L10_0015FFE4;
extern int D_L10_00161F38 __attribute__((sda)); /* no foreign declaration */
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
extern unsigned char D_0013D504[];
extern unsigned char D_00141B20[];
s32 FUN_L00_00203908(s32 a, s32 b);
extern int func_00215570_2E9978(void *arg0, int arg1) __asm__("FUN_00214720");
extern int func_L00_00203F20_2E9978(int a, int b) __asm__("FUN_L00_00203908");
extern float func_001F9D48_2E9978(void *, void *) __asm__("FUN_001f9b80");
extern int func_001F9850_2E9978(int) __asm__("FUN_001f96f8");
extern char D_L10_00179890[];
extern char D_0013E633_2E9978[] __asm__("D_00141B20");
extern unsigned char D_0014171B_2E9978[] __asm__("D_0013D504");
extern unsigned char D_0013D5CA_2E9978[] __asm__("D_0015EEA4") __attribute__((sda));
extern unsigned char D_0013D605_2E9978[] __asm__("D_0013D388");

extern unsigned char D_00141B10[];
extern unsigned char D_00141B18[];
extern unsigned char D_0013D4CB[];
extern unsigned char D_00141C48[];
extern unsigned char D_00141CB8[];
void FUN_L10_002e85b8(Moby *moby)
{
    EvData_2E9978 *data = (EvData_2E9978 *)((int *)moby->pvars);
    Moby *other;
    int any_active;
    int elapsed;

    if (func_00215570_2E9978(D_0013F3D0_r, data->f1C)) {
        D_L10_00161F38 = 1;
    } else {
        D_L10_00161F38 = 0;
    }
    switch (moby->state) {
    case 0:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case 1:
        if (func_00215570_2E9978(D_0013F3D0_r, data->f0) != 0
            && (((HeroPos_2E9978 *)(D_0013F3D0_r))->f200C < 2 || ((HeroPos_2E9978 *)(D_0013F3D0_r))->f200C == 9)
            && ((Msg_2E9978 *)D_L10_00179890)->f0 == 0
            && ((Msg_2E9978 *)D_L10_00179890)->f24 == -1
            && *(unsigned short *)(D_00141B10) == 0) {
            func_L00_00203F20_2E9978(0x2710, 0x35);
        }
        if (func_00215570_2E9978(D_0013F3D0_r, data->f4) != 0
            && (((HeroPos_2E9978 *)(D_0013F3D0_r))->f200C < 2 || ((HeroPos_2E9978 *)(D_0013F3D0_r))->f200C == 9)
            && ((Msg_2E9978 *)D_L10_00179890)->f0 == 0
            && ((Msg_2E9978 *)D_L10_00179890)->f24 == -1) {
            if (((Flags_2E9978 *)(D_0013D388_r))->f4 == 0) {
                if (*(unsigned short *)(D_00141B20_r) == 0) {
                    func_L00_00203F20_2E9978(0x2712, 0x37);
                }
            } else if (((Flags_2E9978 *)(D_0013D388_r))->f5 == 0) {
                if (*(unsigned short *)(D_00141B28_r) == 0) {
                    func_L00_00203F20_2E9978(0x2713, 0x38);
                }
            }
        }
        if (D_L10_0015F5C4_vendor == 0 && D_0013D504_r[0] != 0
            && ((Msg_2E9978 *)D_L10_00179890)->f0 == 0
            && ((Msg_2E9978 *)D_L10_00179890)->f24 == -1
            && *(unsigned short *)(D_00141B18) == 0) {
            func_L00_00203F20_2E9978(0x2711, 0x36);
        }
        if (func_00215570_2E9978(D_0013F3D0_r, data->f8) != 0
            && (((HeroPos_2E9978 *)(D_0013F3D0_r))->f200C < 2 || ((HeroPos_2E9978 *)(D_0013F3D0_r))->f200C == 9)
            && D_0013D4CB[0] == 0
            && ((Msg_2E9978 *)D_L10_00179890)->f0 == 0
            && ((Msg_2E9978 *)D_L10_00179890)->f24 == -1
            && *(unsigned short *)(D_00141C48) == 0) {
            func_L00_00203F20_2E9978(0x2715, 0x5C);
        }
        any_active = 0;
        for (other = D_L10_0015FFE4; other != 0; other = other->next) {
            if (other->oclass == 0x359 && other->state != 1) any_active = 1;
        }
        if (any_active == 0) break;
        if (data->f10 != -1
            && func_001F9D48_2E9978(D_0013F3D0_r, &D_L10_0015FFD0_m[2][data->f10].pos) < 15.0f
            && data->f14 != -1
            && func_001F9D48_2E9978(D_0013F3D0_r, &D_L10_0015FFD0_m[2][data->f14].pos) < 15.0f
            && *(unsigned short *)(D_00141CB8) == 0) {
            func_L00_00203F20_2E9978(0x2717, 0x6A);
        }
        if (any_active == 0) break;
        if (func_00215570_2E9978(D_0013F3D0_r, data->f18) == 0) break;
        if (data->f20 == -1) break;
        if (D_L10_0015FFD8_c2[data->f20].oclass != 0x3F7) break;
        if (D_L10_0015FFD8_c2[data->f20].state == 4) break;
        if (((Save_2E9978 *)(D_00141968_r))->f358 != 0) {
            elapsed = func_001F9850_2E9978(D_0015EEA4_gp) - ((Save_2E9978 *)(D_00141968_r))->f35A * 600;
            if ((int)((float)func_001F9850_2E9978(0x12) * 60.0f) < elapsed || ((Save_2E9978 *)(D_00141968_r))->f35A * 600 == 0) {
                func_L00_00203F20_2E9978(0x2718, 0x6B);
                break;
            }
            if (func_001F9850_2E9978(D_0015EEA4_gp) / 600 > ((Save_2E9978 *)(D_00141968_r))->f35A) {
                ((Save_2E9978 *)(D_00141968_r))->f35A = func_001F9850_2E9978(D_0015EEA4_gp) / 600;
            }
            break;
        }
        ((Save_2E9978 *)(D_00141968_r))->f358++;
        if (func_001F9850_2E9978(D_0015EEA4_gp) / 600 > ((Save_2E9978 *)(D_00141968_r))->f35A) {
            ((Save_2E9978 *)(D_00141968_r))->f35A = func_001F9850_2E9978(D_0015EEA4_gp) / 600;
        }
        ((Save_2E9978 *)(D_00141968_r))->f35C = ((Save_2E9978 *)(D_00141968_r))->f35C | (1 << current_level_index) | 0x80000000;
        break;
    }
}
typedef struct {
    int linked;
    int targets[3];
    float turn_velocity;
    int wake;
} Vars_2E8AB0;

extern struct Moby *D_L10_0015FFD8_8ab0 __asm__("D_L10_0015FFD8") __attribute__((sda));
typedef struct { unsigned char pad[0x454]; unsigned char collected[1]; } SaveFlags_2E8AB0;
extern SaveFlags_2E8AB0 D_L10_001BB6B0_8ab0 __asm__("D_L10_001BB6B0");
extern int D_L10_001BA950[] __asm__("D_L10_001BA950");
extern int D_0014C190_8ab0[] __asm__("D_0014C190");
extern float FUN_L00_00258110(float *, float, float, float, float, float);
extern void build_spherical_offset(f32 *, f32, f32, f32) __asm__("FUN_00214db0");
extern struct Moby *FUN_L09_00307ba8(void *, void *, void *, int);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float FUN_001f9e90(float, float);
extern float FUN_001f9b80(void *, void *);
extern void FUN_L00_001ff290(void *, void *, void *);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

void FUN_L10_002e8ab0(struct Moby *moby) {
    Vars_2E8AB0 *vars = (Vars_2E8AB0 *)moby->pvars;
    int *bits;
    float rotation[4];
    float position[4];
    float displacement[4];
    float angle;
    int state;

    state = moby->state;
    if (state == 1) return;
    if (state < 2) {
        if (state == 0) goto state_zero;
        return;
    }
    if (state == 2) goto state_two;
    if (state == 3) goto state_three;
    return;
state_zero:
    {
        unsigned short save_id;
        int signed_save_id;
        save_id = moby->save_id;
        signed_save_id = (short)save_id;
        if (D_L10_001BB6B0_8ab0.collected[(short)save_id] != 0 ||
            ((*(int *)((char *)D_0014C190_8ab0 + ((signed_save_id >> 5) * 4 + (current_level_index << 8))) >> (save_id & 31)) & 1) != 0) {
            moby->state = 3;
            return;
        }
        {
            if (vars->targets[0] == -1) {
                mark_moby_for_removal(moby);
                return;
            }
            vars->turn_velocity = 0.0f;
            moby->state = 1;
            return;
        }
    }
state_two:
    {
        unsigned short save_id;
        int signed_save_id;
        struct Moby *target;
        struct Moby *candidate;
        save_id = moby->save_id;
        signed_save_id = (short)save_id;
        if (D_L10_001BB6B0_8ab0.collected[(short)save_id] != 0 ||
            ((*(int *)((char *)D_0014C190_8ab0 + ((signed_save_id >> 5) * 4 + (current_level_index << 8))) >> (save_id & 31)) & 1) != 0) {
            moby->state = 3;
            return;
        }
        {
            struct Moby *candidate;
            candidate = 0;
            if (vars->targets[0] != -1) {
                struct Moby *target;
                target = (struct Moby *)((char *)D_L10_0015FFD0_e[2] + (vars->targets[0] << 8));
                candidate = (target != 0 && target->state != 0xfe && target->state != 0xfd) ? target : 0;
            }
            {
                struct Moby *target;
                target = candidate;
                if (candidate == 0) {
                    if (vars->targets[1] != -1) {
                        struct Moby *candidate;
                        candidate = (struct Moby *)((char *)D_L10_0015FFD8_8ab0 + (vars->targets[1] << 8));
                        if (candidate != 0 && candidate->state != 0xfe && candidate->state != 0xfd)
                            target = candidate;
                    }
                }
                candidate = target;
            }
            target = candidate;
        }
        if (target == 0) {
            if (vars->targets[2] != -1) {
                candidate = (struct Moby *)((char *)D_L10_0015FFD8_8ab0 + (vars->targets[2] << 8));
                if (candidate != 0 && candidate->state != 0xfe && candidate->state != 0xfd)
                    target = candidate;
            }
        }
        if (target != 0) {
            moby->rot.z = fast_add_rotations(moby->rot.z, 3.1415927f);
            angle = FUN_001f9e90(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
            moby->rot.z = FUN_L00_00258110(&vars->turn_velocity, moby->rot.z, angle,
                                             D_0015ED70 * 1.5707964f, D_0015ED70 * 3.1415927f,
                                             D_0015ED6C * 0.7853982f);
            angle = FUN_001f9e90(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
            if (fast_difference_between_rotations(moby->rot.z, angle) < 0.04363323f) {
                position[0] = fast_cos(moby->rot.z) * 1.5f;
                position[1] = fast_sin(moby->rot.z) * 1.5f;
                position[2] = 0.0f;
                FUN_L00_001ff290(displacement, position, &moby->pos);
                build_spherical_offset(rotation, D_0015ED6C * 40.0f,
                    FUN_001f9e90(target->pos.x - position[0], target->pos.y - position[1]),
                    FUN_001f9e90(FUN_001f9b80(position, &target->pos), target->pos.z - position[2]));
                FUN_L09_00307ba8(moby, position, rotation, scale_game_frames(30));
                if (target == (struct Moby *)((char *)D_L10_0015FFD0_e[2] + (vars->targets[0] << 8)))
                    vars->targets[0] = -1;
                else if (target == (struct Moby *)((char *)D_L10_0015FFD0_e[2] + (vars->targets[1] << 8)))
                    vars->targets[1] = -1;
                else
                    vars->targets[2] = -1;
            }
            moby->rot.z = fast_add_rotations(moby->rot.z, 3.1415927f);
        } else {
            moby->state = 3;
            if (vars->wake != -1) {
                ((struct Moby *)((vars->wake << 8) + (int)D_L10_0015FFD8_8ab0))->state = 1;
                vars->wake = -1;
            }
        }
        if (vars->linked != -1) {
            ((struct Moby *)((vars->linked << 8) + (int)D_L10_0015FFD8_8ab0))->command = 1;
            vars->linked = -1;
        }
        return;
    }
state_three:
    {
        unsigned short save_id;
        struct Moby *target;
        struct Moby *candidate;
        target = 0;
        if (vars->targets[0] != -1) {
            struct Moby *candidate = (struct Moby *)((char *)D_L10_0015FFD0_e[2] + (vars->targets[0] << 8));
            if (candidate != 0 && candidate->state != 0xfe && candidate->state != 0xfd)
                target = candidate;
        }
        /* Retail removes a valid first target twice. */
        if (target != 0) {
            mark_moby_for_removal(target);
            candidate = target;
            goto remove_candidate;
        }
        if (vars->targets[1] == -1) goto no_second_target;
        candidate = (struct Moby *)((char *)D_L10_0015FFD8_8ab0 + (vars->targets[1] << 8));
        if (candidate == 0 || candidate->state == 0xfe || candidate->state == 0xfd)
            goto no_second_target;
        goto remove_candidate;
no_second_target:
        candidate = target;
remove_candidate:
        if (candidate != 0)
            mark_moby_for_removal(candidate);
        save_id = moby->save_id;
        bits = (int *)((char *)D_0014C190_8ab0 + (((int)((unsigned int)save_id << 16) >> 21) * 4 + (current_level_index << 8)));
        *bits |= 1 << (save_id & 31);
        save_id = moby->save_id;
        D_L10_001BA950[(int)((unsigned int)save_id << 16) >> 21] |= 1 << (save_id & 31);
        return;
    }
}
typedef struct {
    int unk0;
    float home_yaw;
    int timer;
    int clip_volume;
    int sound;
} DoorVars_e9648;

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} LevelState_e9648;

extern LevelState_e9648 D_L10_001BB6B0_e9648 __asm__("D_L10_001BB6B0");
extern int D_L10_001BA950[] __asm__("D_L10_001BA950");
extern int D_0014C190_e9648[][64] __asm__("D_0014C190");
extern unsigned char D_001413F4_e9648[] __asm__("D_001413F4");
extern unsigned char D_0013DD4C_e9648[] __asm__("D_0013DD4C");
extern int inside_clip_volume_e9648(void *, int) __asm__("FUN_00214720");
extern void FUN_L10_002f5a50(int);
extern int truncate_float_e9648(float) __asm__("FUN_001fa6d0");
extern int tick_countdown_e9648(int *) __asm__("FUN_001f9740");

/* Level 10 door: swings open once the hero enters its clip volume and sets its save bit; starts open when the bit is already set. */
void FUN_L10_002e9648(struct Moby *moby) {
    DoorVars_e9648 *vars = (DoorVars_e9648 *)moby->pvars;
    unsigned short save_id;

    switch (moby->state) {
    case 0:
        moby->update_dist = 0xFF;
        if (D_001413F4_e9648[0] == 0) {
            save_id = moby->save_id;
            if (D_L10_001BB6B0_e9648.collected[(short)save_id] == 0 &&
                ((D_0014C190_e9648[current_level_index][(short)save_id >> 5] >> (save_id & 31)) & 1) == 0) {
                vars->home_yaw = moby->rot.y;
                moby->state = 1;
                return;
            }
        }
        moby->rot.y = fast_add_rotations(moby->rot.y, -1.3962634f);
        moby->state = 3;
        return;
    case 1:
        if (hero.vehicle_mode != 0)
            goto open;
        save_id = moby->save_id;
        if (D_L10_001BB6B0_e9648.collected[(short)save_id] != 0 ||
            ((D_0014C190_e9648[current_level_index][(short)save_id >> 5] >> (save_id & 31)) & 1) != 0) {
        open:
            moby->rot.y = fast_add_rotations(moby->rot.y, -1.3962634f);
            moby->state = 3;
            return;
        }
        if (inside_clip_volume_e9648(&hero.motion.pos, vars->clip_volume) == 0 && D_0013DD4C_e9648[0] == 0)
            return;
        if (vars->unk0 <= 0 || vars->sound <= 0)
            return;
        save_id = moby->save_id;
        D_0014C190_e9648[current_level_index][(short)save_id >> 5] |= 1 << (save_id & 31);
        save_id = moby->save_id;
        D_L10_001BA950[(short)save_id >> 5] |= 1 << (save_id & 31);
        if (inside_clip_volume_e9648(&hero.motion.pos, vars->clip_volume))
            FUN_L10_002f5a50(vars->sound);
        vars->timer = truncate_float_e9648(scale_time(90.0f));
        moby->state = 2;
        return;
    case 2:
        moby->rot.y = fast_add_rotations(moby->rot.y, -1.3962634f / scale_time(90.0f));
        if (tick_countdown_e9648(&vars->timer))
            moby->state = 3;
        return;
    case 3:
        if (D_001413F4_e9648[0] != 0)
            return;
        save_id = moby->save_id;
        if (D_L10_001BB6B0_e9648.collected[(short)save_id] != 0)
            return;
        if ((((D_0014C190_e9648[current_level_index][(short)save_id >> 5] >> (save_id & 31)) ^ 1) & 1) == 0)
            return;
        moby->rot.y = fast_add_rotations(moby->rot.y, 1.3962634f);
        moby->state = 0;
        return;
    }
}
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002EAD50), where it is exact; names translated to the US level program. */

typedef OvlQuad Vec_2EAD50;

typedef struct {
    Vec_2EAD50 home;        /* 0x00 */
    int sound;              /* 0x10 */
    int timer;              /* 0x14 */
    char pad18[0x4];
    int door;               /* 0x1C: index of the linked moby */
} GateVars_2EAD50;

typedef struct {
    char pad00[0x10];
    Vec_2EAD50 pos;         /* 0x10 */
    unsigned char state;    /* 0x20 */
    char pad21[0xF];
    unsigned char f30;      /* 0x30 */
    char pad31[0x3];
    unsigned short flags;   /* 0x34 */
    char pad36[0x12];
    float rotz;             /* 0x48 */
    char pad4C[0x2C];
    GateVars_2EAD50 *vars;  /* 0x78 */
    char pad7C[0x36];
    unsigned short uid;     /* 0xB2 */
    char padB4[0x4C];
} GateMoby_2EAD50;

typedef struct { char pad0[0x454]; unsigned char collected[1]; } L10State_2EAD50;

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float fast_add_rotations_alt(float) __asm__("FUN_001fa580");
extern float fast_cos_local(float, float) __asm__("FUN_001f9dc8");
extern int D_0013DD4C; /* no foreign declaration */
extern unsigned char D_0014C190_c[] __asm__("D_0014C190");
extern GateMoby_2EAD50 *D_L10_0015FFD8_c __asm__("D_L10_0015FFD8") __attribute__((sda));
extern s32 FUN_001f96b0_c(f32) __asm__("FUN_001f96b0");
extern unsigned char D_001413F4[];
extern void add_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_L10_002f5a50(int i);
extern unsigned char D_001413F4[];
extern unsigned char D_0013DE4B_2EAD50[] __asm__("D_0013DD4C");
extern int D_L10_001BA950[] __asm__("D_L10_001BA950");
extern L10State_2EAD50 D_L10_001BB9C0_2EAD50 __asm__("D_L10_001BB6B0");
extern void func_L10_002F6E10_2EAD50(int) __asm__("FUN_L10_002f5a50");
extern float func_001F9F90_2EAD50(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2EAD50(float) __asm__("FUN_001f9de0");
extern void func_001F9BD8_2EAD50(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_001F9878_2EAD50(float) __asm__("FUN_001fa6d0");
extern float func_001FA888_2EAD50(int) __asm__("FUN_001fa6c0");
extern int func_001F9908_2EAD50(int *) __asm__("FUN_001f9740");

void FUN_L10_002e9990(GateMoby_2EAD50 *m)
{
    GateVars_2EAD50 *d = m->vars;
    float v[3];
    Vec_2EAD50 out;
    unsigned char st = m->state;

    switch (st) {
    case 0:
        m->f30 = 0xFF;
        d->home = m->pos;
        if (D_001413F4[0] != 0 ||
            D_L10_001BB9C0_2EAD50.collected[(short)m->uid] != 0 ||
            ((*(int *)(D_0014C190_c + ((((short)m->uid >> 5) * 4) + (current_level_index << 8))) >> (m->uid & 0x1F)) & 1) != 0) {
            v[0] = func_001F9F90_2EAD50(fast_add_rotations(m->rotz, 3.14159265f)) * 1.5f;
            v[1] = func_001F9FA8_2EAD50(fast_add_rotations(m->rotz, 3.14159265f)) * 1.5f;
            v[2] = 0.0f;
            func_001F9BD8_2EAD50(&out, &d->home, v);
            m->state = 3;
            m->pos = out;
        } else {
            if (d->door > 0) {
                D_L10_0015FFD8_c[d->door].flags |= 2;
            }
            m->state = 1;
        }
        break;
    case 1: {
        unsigned char *bits;
        if (D_001413F4[0] != 0 ||
            D_L10_001BB9C0_2EAD50.collected[(short)m->uid] != 0 ||
            ((*(int *)((bits = (char *)D_0014C190_c) + ((((short)m->uid >> 5) * 4) + (current_level_index << 8))) >> (m->uid & 0x1F)) & 1) != 0) {
            m->pos = d->home;
            m->state = 3;
            if (d->door > 0) {
                D_L10_0015FFD8_c[d->door].flags &= 0xFFFD;
            }
        } else if (D_0013DE4B_2EAD50[0] != 0) {
            if (d->door > 0) {
                D_L10_0015FFD8_c[d->door].flags &= 0xFFFD;
            }
            *(int *)(bits + ((((short)m->uid >> 5) * 4) + (current_level_index << 8))) |= st << (m->uid & 0x1F);
            D_L10_001BA950[(short)m->uid >> 5] |= st << (m->uid & 0x1F);
            if (d->sound > 0) {
                func_L10_002F6E10_2EAD50(d->sound);
            }
            d->timer = func_001F9878_2EAD50(scale_time(90.0f));
            m->state = 2;
        }
        break;
    }
    case 2:
        v[0] = func_001F9F90_2EAD50(fast_add_rotations(m->rotz, 3.14159265f)) * 1.5f *
               func_001FA888_2EAD50(func_001F9878_2EAD50(scale_time(90.0f)) - d->timer) /
               scale_time(90.0f);
        v[1] = func_001F9FA8_2EAD50(fast_add_rotations(m->rotz, 3.14159265f)) * 1.5f *
               func_001FA888_2EAD50(func_001F9878_2EAD50(scale_time(90.0f)) - d->timer) /
               scale_time(90.0f);
        v[2] = 0.0f;
        func_001F9BD8_2EAD50(&out, &d->home, v);
        m->pos = out;
        if (func_001F9908_2EAD50(&d->timer)) {
            m->state = 3;
        }
        break;
    case 3:
        if (D_001413F4[0] == 0 &&
            D_L10_001BB9C0_2EAD50.collected[(short)m->uid] == 0 &&
            (((*(int *)(D_0014C190_c + ((((short)m->uid >> 5) * 4) + (current_level_index << 8))) >> (m->uid & 0x1F)) ^ 1) & 1)) {
            m->pos = d->home;
            m->state = 0;
        }
        break;
    }
}
