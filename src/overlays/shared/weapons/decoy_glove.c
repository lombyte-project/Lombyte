/* Shared moby class decoy_glove. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/input/pad_state.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "qzero.h"
#include "rnc/game/game_globals.h"

typedef struct Hero Hero;

typedef struct PadState PadState;

typedef int u128_2D3608 __attribute__((mode(TI)));

typedef struct Moby_2D3608 {
    char                pad0[0x48];
    float               yaw;
    char                pad4C[4];
    struct Moby_2D3608 *target;
    char                pad54[0x24];
    float              *data;
} Moby_2D3608;

typedef struct Hit_2D3608 {
    char  pad0[0x18];
    void *moby;
    int   pad1C;
    float point[4];
} Hit_2D3608;

typedef float Vec_2D3608[4] __attribute__((aligned(16)));

typedef float Mat_2D3608[4][4] __attribute__((aligned(16)));

typedef float Mat3_2D3608[3][4] __attribute__((aligned(16)));

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4f_2d2670;

typedef int               u128_D3B20 __attribute__((mode(TI)));
extern int D_L00_00173E40 NOT_SDA;

extern int   D_0015ED84;
extern float D_0015ED70;

extern unsigned char D_001413D0_r[] __asm__("D_001413D0");
extern int           D_L00_00173E40;
extern char          D_0013F990_2D3608[] __asm__("D_0013F990");
extern float         func_001F9F90_2D3608(float) __asm__("FUN_001f9dc8");
extern float         func_001F9FA8_2D3608(float) __asm__("FUN_001f9de0");
extern void          func_001F9C30_2D3608(void *, void *, float) __asm__("FUN_001f9a68");
extern int   func_L00_001EFFF0_2D3608(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void  func_L00_002C8680_2D3608(float *pos,
                                      float *tgt,
                                      float *output,
                                      float  speed,
                                      float  unused,
                                      float  range) __asm__("FUN_L00_002c7320");
extern int   D_0015EEA4;
extern int   use_ammo(int, int) __asm__("FUN_L00_00233db8");
extern int   get_ammo(int) __asm__("FUN_L00_00233e98");
extern int   spawn_decoy(void *, void *, void *) __asm__("FUN_L00_002c35c0");
extern short D_L00_001B0830[];
extern void  hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void  FUN_L00_0024f7c8(void *, int, void *);
extern void  mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void  release_moby_with_collision_check(float *, char *, void *) __asm__("FUN_L00_002c3888");
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void  mark_moby_for_removal() __asm__("FUN_0020c828");
extern u8    D_0013E539_lui __asm__("D_0013E539") NOT_SDA;
extern char  D_0013CAE4[];

/* Same camera step as func_L00_002C88A8, writing the following vector in the target data. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D3608), where it is exact; names translated to the US level program. */

void decoy_glove_aim_throw(int          collision_arg,
                           Moby_2D3608 *carrier,
                           float       *position) __asm__("FUN_L00_002d2158");

void decoy_glove_aim_throw(int collision_arg, Moby_2D3608 *carrier, float *position) {
    Vec_2D3608   anchor;
    Mat3_2D3608  rotation;
    Mat_2D3608   inverse;
    Vec_2D3608   direction, aim, launch, origin_copy, endpoint;
    Moby_2D3608 *target;
    float       *output;
    float        value, distance, heading, vertical, pitch, length, fall_time;
    int          hit;
    float        range;
    float       *anchor_ptr = anchor;

    *(u128_2D3608 *)anchor_ptr = *(u128_2D3608 *)position;
    copy_matrix3x4(rotation, l00_camera.unk0);
    transpose_rotation_matrix(inverse, rotation);
    target = carrier->target;
    if (target == 0)
        return;
    output = target->data;
    *(u128_2D3608 *)direction = 0;
    direction[0] = 1.0f;
    transform_vector_by_basis(direction, direction, D_0013F990_2D3608);
    {
        Hero *hero = (Hero *)(D_0013F990_2D3608 - 0x640);
        anchor_ptr[0] = func_001F9F90_2D3608(fast_add_rotations(
                            -0.361968011f, fast_arc_tan(hero->facing_x, hero->facing_y))) *
                        0.859039009f;
        anchor_ptr[1] = func_001F9FA8_2D3608(fast_add_rotations(
                            -0.361968011f, fast_arc_tan(hero->facing_x, hero->facing_y))) *
                        0.859039009f;
        anchor_ptr[2] = 0.0f;
        fast_vec_add(anchor_ptr, anchor_ptr, D_0013F990_2D3608 - 0x5C0);
        anchor_ptr[2] = anchor_ptr[2] + 0.390819997f;
        fast_arc_tan(direction[0], direction[1]);
        range = (controller_state.held & 5) ? 3.5f : 2.5f;
        func_001F9C30_2D3608(aim, direction, range);
        fast_vec_add(aim, aim, anchor_ptr);
        if ((controller_state.held & 5) && hero->item_use_locked == 0) {
            *(u128_2D3608 *)launch = 0;
            launch[2] = 1.0f;
            transform_vector(launch, launch, inverse);
            hit = 0;
            fast_subtract_rotations(((Moby_2D3608 *)hero->moby)->yaw,
                                    fast_arc_tan(launch[0], launch[1]));
            heading = fast_arc_tan(launch[0], launch[1]);
            pitch = fast_arc_tan(vector_length_xy(launch), launch[2]);
            if (0.593411922f < pitch)
                pitch = 0.593411922f;
            if (pitch < -1.39626336f)
                pitch = -1.39626336f;
            build_spherical_offset(
                launch, (controller_state.held & 5) ? 3.5f : 2.5f, heading, pitch);
            qcopy(origin_copy, &l00_camera.pos);
            length = vector_length_xy(launch);
            {
                float rise = launch[2];
                if (length != 0.0f) {
                    value = rise / length;
                } else {
                    value = rise / 0.00999999978f;
                }
            }
            fall_time =
                square_root(D_0015ED70 * 9.0f * ((controller_state.held & 5) ? 3.5f : 2.5f) * 0.5f);
            distance = (fall_time + fall_time) * fall_time * (1.0f - value) / (D_0015ED70 * 9.0f);
            vertical = value * distance;
            if (0.0f < distance) {
                launch[0] = func_001F9F90_2D3608(heading) * distance;
                launch[1] = func_001F9FA8_2D3608(heading) * distance;
                launch[2] = vertical;
                fast_vec_add(endpoint, g_debug_camera.position, launch);
                if (func_L00_001EFFF0_2D3608(
                        g_debug_camera.position, endpoint, 2, collision_arg, 0) &&
                    ((Hit_2D3608 *)((char *)&D_L00_00173E40))->moby != *(void **)(D_001413D0_r)) {
                    qcopy(aim, ((Hit_2D3608 *)((char *)&D_L00_00173E40))->point);
                    hit = 1;
                    heading = fast_arc_tan(aim[0] - anchor_ptr[0], aim[1] - anchor_ptr[1]);
                }
                if (hit == 0) {
                    value = 5.0f;
                    aim[0] = func_001F9F90_2D3608(heading) * distance * value;
                    aim[1] = func_001F9FA8_2D3608(heading) * distance * value;
                    aim[2] = vertical * value;
                    fast_vec_add(aim, aim, g_debug_camera.position);
                }
            }
        } else {
            *(int *)(D_0013F990_2D3608 + 0x410) = 0;
        }
    }
    func_L00_002C8680_2D3608(anchor_ptr,
                             aim,
                             output + 4,
                             D_0015ED70 * 9.0f,
                             0.00000999999975f,
                             (controller_state.held & 5) ? 3.5f : 2.5f);
}

/* Decoy glove update: state machine on moby[0x20], per-object data block at moby[0x78]. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D3B20), where it is exact; names translated to the US level program. */

void decoy_glove_update(unsigned char *moby) __asm__("FUN_L00_002d2670");

/* Moby class 562 */
void decoy_glove_update(unsigned char *moby) {
    enum {
        DECOY_GLOVE_INITIALIZE = 0,
        DECOY_GLOVE_WAIT_FOR_MOBY_FLAG = 1,
        DECOY_GLOVE_TARGET_FOLLOW_AND_SPAWN = 2,
        DECOY_GLOVE_RELEASE_DECOY = 3,
        DECOY_GLOVE_WAIT_AFTER_RELEASE = 4,
        DECOY_GLOVE_ENTER_SKIP_TARGET_FOLLOW_AND_SPAWN = 5,
        DECOY_GLOVE_SKIP_TARGET_FOLLOW_AND_SPAWN = 6
    };
    extern void mark_moby_for_removal();
    extern void transform_vector() __asm__("FUN_001f9d20");
    extern int  allocate_voice_for_target_entry(int, int, void *);
    char       *glove_vars;
    char       *tracked_target_moby;
    char       *hero_data;
    float       vb[4];
    float       va[4];
    float       vc[4];
    float       vd[4];
    float       pad0[4];
    float       pad1[4];
    float       pad2[4];
    u128_D3B20  loc;

    if (moby == 0)
        return;
    glove_vars = ((char *)MOBY(moby)->pvars);
    if (glove_vars == 0)
        return;
    *(u128_D3B20 *)va = 0;
    va[1] = -0.19f;
    va[2] = -0.11f;
    FUN_L00_0024f7c8(moby, 0, vb);
    build_moby_bone_transform(moby, 0, vd);
    transform_vector(vc, va, vd);
    fast_vec_add(glove_vars + 0x40, vb, vc);

    tracked_target_moby = *(char **)(glove_vars + 0x50);
    if (tracked_target_moby) {
        MOBY(tracked_target_moby)->collision_disable = 1;
        if (D_0013E539_lui && *(short *)(*(char **)(glove_vars + 0x50) + 0xA6) != 0x76C) {
            qzero(&loc);
            mine_spawn_drops(*(char **)(glove_vars + 0x50), D_L00_001B0830);
            ((void (*)(void *))mark_moby_for_removal)(*(char **)(glove_vars + 0x50));
            *(int *)(glove_vars + 0x50) = spawn_decoy(moby, glove_vars + 0x40, &loc);
        }
        if (*(char **)(glove_vars + 0x50) != 0 &&
            *(unsigned char *)(*(char **)(glove_vars + 0x50) + 0x20) != 0xFE &&
            *(unsigned char *)(*(char **)(glove_vars + 0x50) + 0x20) != 0xFD)
            goto target_valid;
    }
    *(int *)(glove_vars + 0x50) = 0;
target_valid:
    hero_data = ((char *)&hero);
    if (*(int *)(hero_data + 0x2084) == 1)
        hero_set_state(0x1E, 1);
    if (fast_dec_timer((int *)(glove_vars + 0x54))) {
        if (*(unsigned char *)(hero_data + 0x20AC) == 0) {
            if (*(unsigned char *)(hero_data + 0x20A8) &&
                *(int *)(hero_data + 0x1BC) == scale_game_frames(0x11))
                goto L3D10;
            if (*(int *)(((char *)&D_0013CAE4)) & *(int *)(hero_data + 0x10A0)) {
                if (*(int *)(hero_data + 0x2084) == 0x1E && get_ammo(-1))
                    goto L3D10;
            }
            hero_data = ((char *)&hero);
            if (*(int *)(hero_data + 0x2084) == 0x23 &&
                *(int *)(hero_data + 0x198) == scale_game_frames(0x10))
                goto L3D10;
        }
    }
    goto L3D34;
L3D10:
    allocate_hero_voice_for_entry(0x1A, 0);
    use_ammo(-1, 1);
    moby[0x20] = 3;
L3D34:
    *(u128_D3B20 *)&loc = *(u128_D3B20 *)(glove_vars + 0x40);
    decoy_glove_aim_throw((int)moby, (void *)glove_vars, (float *)&loc);

    switch (moby[0x20]) {
    case DECOY_GLOVE_INITIALIZE:
        *(int *)(glove_vars + 0x50) = 0;
        if (!(moby[0x70] & 2))
            moby[0x20] = 1;
        else
            moby[0x20] = 2;
    case DECOY_GLOVE_WAIT_FOR_MOBY_FLAG:
        if (moby[0x70] & 2)
            moby[0x20] = 2;
        break;
    case DECOY_GLOVE_TARGET_FOLLOW_AND_SPAWN:
        break;
    case DECOY_GLOVE_RELEASE_DECOY:
        if (*(char **)(glove_vars + 0x50)) {
            char *stats = ((char *)&item_usage);
            if (*(unsigned short *)(stats + 0xC8) <= 0xFFFE)
                *(unsigned short *)(stats + 0xC8) = *(unsigned short *)(stats + 0xC8) + 1;
            if (scale_game_frames(D_0015EEA4) / 600 > (int)*(unsigned short *)(stats + 0xCA))
                *(unsigned short *)(stats + 0xCA) = scale_game_frames(D_0015EEA4) / 600;
            *(unsigned int *)(stats + 0xCC) =
                (*(unsigned int *)(stats + 0xCC) | (1 << D_0015ED84)) | 0x80000000;
            release_moby_with_collision_check((float *)(glove_vars + 0x40),
                                              *(char **)(glove_vars + 0x50),
                                              *(char **)(*(char **)(glove_vars + 0x50) + 0x78) +
                                                  0x10);
            *(int *)(glove_vars + 0x50) = 0;
        } else {
            allocate_voice_for_target_entry(0, 0, (int)moby);
        }
        moby[0x20] = 4;
        *(int *)(glove_vars + 0x54) = scale_game_frames(0x14);
        break;
    case DECOY_GLOVE_WAIT_AFTER_RELEASE: {
        char *h = ((char *)&hero);
        if (*(int *)(h + 0x2084) != 0x23 && *(unsigned char *)(h + 0x20A8) == 0)
            moby[0x20] = 2;
        break;
    }
    case DECOY_GLOVE_ENTER_SKIP_TARGET_FOLLOW_AND_SPAWN:
        moby[0x20] = 6;
        break;
    case DECOY_GLOVE_SKIP_TARGET_FOLLOW_AND_SPAWN:
        return;
    default:
        break;
    }

    if (*(char **)(glove_vars + 0x50)) {
        qcopy(*(char **)(glove_vars + 0x50) + 0x10, glove_vars + 0x40);
        return;
    }
    if (get_ammo(-1)) {
        qzero(&loc);
        *(int *)(glove_vars + 0x50) = spawn_decoy(moby, glove_vars + 0x40, &loc);
    } else {
        char *h = ((char *)&hero);
        if (*(unsigned char *)(h + 0x20A8)) {
            qzero(&loc);
            *(int *)(glove_vars + 0x50) = spawn_decoy(moby, glove_vars + 0x40, &loc);
        }
    }
}
