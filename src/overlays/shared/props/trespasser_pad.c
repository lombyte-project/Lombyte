/* Shared moby class tresspasser_pad: the pad the hero uses the trespasser on, and its puzzle display. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/game/item_state.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/hero/hero_frame_update.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/l02/hero/hero_set_state.h"
#include "rnc/ui/help/force_help_message.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/overlay/shared/props/trespasser_pad_data/draw_tresspasser_puzzle_radial_strip.h"
#include "rnc/overlay/shared/props/trespasser_pad_data/draw_tresspasser_puzzle.h"

typedef float V2d8910[4] __attribute__((aligned(16)));

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2D9F08;

typedef struct Data {
    char           p0[0x10];
    short          h10;
    short          h12;
    int            i14;
    int            i18;
    int            i1C;
    char           p20[0x90];
    int            iB0[12];
    int            iE0;
    float          fE4;
    char           pE8[0x4];
    int            iEC;
    Vec_2D9F08     vF0;
    Vec_2D9F08     v100;
    float          f110;
    float          f114;
    char           p118[0x4];
    short          h11C;
    unsigned short h11E;
} Data;

typedef struct Pad {
    char p0[0x1A4];
    int  i1A4;
    char p1A8[0x18];
    union {
        long l;
        struct {
            int lo;
            int hi;
        } w;
    } b1C0;
} Pad;

typedef struct {
    int  i0;
    char p4[0x20];
    int  i24;
} Idle;

typedef struct {
    char          pad0[0x454];
    unsigned char collected[1];
} Lvl;

typedef unsigned long long u64_t;

typedef struct {
    long a, b, c, d;
} Q_2dae88;

/* One ring of 12 slots: 0 empty, 1 lit, 2 occupied, 3 just reached. */
typedef struct {
    int slot[12];
} Ring12;

extern LevelCamera l02_camera_lui __asm__("D_L02_00167280") NOT_SDA;

extern char D_0013C940[];
void        draw_tresspasser_puzzle(char *moby) __asm__("FUN_L02_002d93e8");

extern int  allocate_voice_for_target_entry(long, int, Moby *) __asm__("FUN_0022da68");
extern void func_L02_002DA728_2D9F08(Moby *) __asm__("FUN_L02_002d92f0");
extern void func_L02_002D9D48_2D9F08(void *, void *, Moby *) __asm__("FUN_L02_002d8910");
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void           func_L02_002DA820_2D9F08(void) __asm__("FUN_L02_002d93e8");
extern void           func_L00_0025E590_2D9F08(Moby *, Data *) __asm__("FUN_L00_0025d538");
extern float          D_L02_001D33B0[4][4];
extern Lvl            D_L02_001BBB40_2D9F08 __asm__("D_L02_001BB830");
extern int            current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern Vec_2D9F08     D_L02_00167440_2D9F08[] __asm__("D_L02_001673C0");
extern float          frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern float          frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern int            D_L02_0017EB08_lui __asm__("D_L02_0017EB08") NOT_SDA;
extern Idle           D_L02_00179A10;
extern int            D_L02_001BADE0_2D9F08[] __asm__("D_L02_001BAAD0");
extern int            D_L02_00161B1C_gp __asm__("D_L02_00161B1C") __attribute__((sda));
extern int            D_L02_00161AA8_gp __asm__("D_L02_00161AA8") __attribute__((sda));
extern int            D_L02_00161AA4_gp __asm__("D_L02_00161AA4") __attribute__((sda));
extern int            D_L02_00161AA0_gp __asm__("D_L02_00161AA0") __attribute__((sda));
extern char           D_0013E633_2D9F08[] __asm__("D_0013E533");
extern int D_0013CAE4 NOT_SDA;

extern void func_L00_002607A8_d92f0(void *a, float x) __asm__("FUN_L00_0025f730");

extern int  tresspasser_ring_arc_first_vertex_uv_gp __asm__("D_L02_00161B0C") __attribute__((sda));
extern int  tresspasser_ring_arc_second_vertex_uv_gp __asm__("D_L02_00161B10") __attribute__((sda));
extern int  tresspasser_ring_arc_third_vertex_uv_gp __asm__("D_L02_00161B14") __attribute__((sda));
extern int  tresspasser_ring_arc_fourth_vertex_uv_gp __asm__("D_L02_00161B18") __attribute__((sda));
extern void FUN_L02_0020bc88(void *, void *, void *, s64, s32);

extern int tresspasser_arrow_newly_reached_first_vertex_uv_gp __asm__("D_L02_00161ABC")
    __attribute__((sda));
extern int tresspasser_arrow_newly_reached_second_vertex_uv_gp __asm__("D_L02_00161AC0")
    __attribute__((sda));
extern int tresspasser_arrow_newly_reached_third_vertex_uv_gp __asm__("D_L02_00161AC4")
    __attribute__((sda));
extern int tresspasser_arrow_newly_reached_fourth_vertex_uv_gp __asm__("D_L02_00161AC8")
    __attribute__((sda));
extern int tresspasser_arrow_occupied_first_vertex_uv_gp __asm__("D_L02_00161ACC")
    __attribute__((sda));
extern int tresspasser_arrow_occupied_second_vertex_uv_gp __asm__("D_L02_00161AD0")
    __attribute__((sda));
extern int tresspasser_arrow_occupied_third_vertex_uv_gp __asm__("D_L02_00161AD4")
    __attribute__((sda));
extern int tresspasser_arrow_occupied_fourth_vertex_uv_gp __asm__("D_L02_00161AD8")
    __attribute__((sda));
extern int tresspasser_arrow_lit_first_vertex_uv_gp __asm__("D_L02_00161ADC") __attribute__((sda));
extern int tresspasser_arrow_lit_second_vertex_uv_gp __asm__("D_L02_00161AE0") __attribute__((sda));
extern int tresspasser_arrow_lit_third_vertex_uv_gp __asm__("D_L02_00161AE4") __attribute__((sda));
extern int tresspasser_arrow_lit_fourth_vertex_uv_gp __asm__("D_L02_00161AE8") __attribute__((sda));
extern int tresspasser_arrow_vertex_color_gp __asm__("D_L02_00161B9C") __attribute__((sda));

extern s32   D_L02_0015F5CC;
extern f32   D_L02_00161B60 __attribute__((sda));
extern char *tresspasser_radial_strip_scrolling_first_vertex_uv_base_gp __asm__("D_L02_00161AEC")
    __attribute__((sda));
extern char *tresspasser_radial_strip_scrolling_second_vertex_uv_base_gp __asm__("D_L02_00161AF0")
    __attribute__((sda));
extern char *tresspasser_radial_strip_scrolling_third_vertex_uv_base_gp __asm__("D_L02_00161AF4")
    __attribute__((sda));
extern char *tresspasser_radial_strip_scrolling_fourth_vertex_uv_base_gp __asm__("D_L02_00161AF8")
    __attribute__((sda));
extern char *tresspasser_radial_strip_fixed_first_vertex_uv_gp __asm__("D_L02_00161AFC")
    __attribute__((sda));
extern char *tresspasser_radial_strip_fixed_second_vertex_uv_gp __asm__("D_L02_00161B00")
    __attribute__((sda));
extern char *tresspasser_radial_strip_fixed_third_vertex_uv_gp __asm__("D_L02_00161B04")
    __attribute__((sda));
extern char *tresspasser_radial_strip_fixed_fourth_vertex_uv_gp __asm__("D_L02_00161B08")
    __attribute__((sda));

extern void FUN_L02_0020bc88_u(void *, void *, void *, s32, s32) __asm__("FUN_L02_0020bc88");

void            f20bc88_2dae88(Q_2dae88 *, int *, int *, int, int) __asm__("FUN_L02_0020bc88");
extern Q_2dae88 l02_tresspasser_textured_quad_packed_vertices[] __asm__("D_L02_001D3310");
extern int      tresspasser_textured_quad_color_gp __asm__("D_L02_00161B94") __attribute__((sda));
extern int      tresspasser_quad_first_vertex_uv_gp __asm__("D_L02_00161AAC") __attribute__((sda));
extern int      tresspasser_quad_second_vertex_uv_gp __asm__("D_L02_00161AB0") __attribute__((sda));
extern int      tresspasser_quad_third_vertex_uv_gp __asm__("D_L02_00161AB4") __attribute__((sda));
extern int      tresspasser_quad_fourth_vertex_uv_gp __asm__("D_L02_00161AB8") __attribute__((sda));
extern int      D_L02_0015F5CC; /* no foreign declaration */

extern s32 fast_tween_color(s32, s32, f32) __asm__("func_001FA6E0");

extern void
draw_tresspasser_puzzle_ring_arc(int, u32, float, float, float, float) __asm__("FUN_L02_002da3d8");
extern void draw_tresspasser_puzzle_arrow(int, int, int, float, float) __asm__("FUN_L02_002da6c0");
extern void
draw_tresspasser_puzzle_radial_strip(void *, f32, f32, f32, s32) __asm__("FUN_L02_002da938");
extern void draw_tresspasser_puzzle_textured_quads(void) __asm__("FUN_L02_002dae88");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

/* Spawn point for the moby: its slot's stored position and rotation, or, with no slot, a point beside it on the side facing the hero. */

void compute_tresspasser_pad_camera_transform(float *pos,
                                              float *camera_rotation_output,
                                              char  *moby) __asm__("FUN_L02_002d8910");

void compute_tresspasser_pad_camera_transform(float *pos,
                                              float *camera_rotation_output,

                                              char *moby) {
    V2d8910 midpoint_position;
    float   left;
    float   right;
    float   pick;
    float   to;
    float   dl;
    int     camera_transform_index;
    char   *base;

    camera_transform_index = *(int *)(((char *)MOBY(moby)->pvars) + 0xE8);
    if (camera_transform_index == -1) {
        left = fast_add_rotations(MOBY(moby)->rot.z, 1.57079637f);
        right = fast_add_rotations(MOBY(moby)->rot.z, -1.57079637f);
        to = fast_arc_tan(l02_camera_lui.pos.f[0] - MOBY(moby)->pos.x,
                          l02_camera_lui.pos.f[1] - MOBY(moby)->pos.y);
        dl = fast_difference_between_rotations(to, left);
        pick = dl < fast_difference_between_rotations(to, right) ? left : right;
        fast_vec_add(midpoint_position, &hero.motion.pos, moby + 0x10);
        fast_vec_scale(midpoint_position, midpoint_position, 0.5f);
        qcopy(pos, midpoint_position);
        pos[0] += fast_cos(pick) * 2.0f;
        pos[1] += fast_sin(pick) * 2.0f;
        pos[2] += 1.0f;
        camera_rotation_output[0] = 0;
        camera_rotation_output[1] = 0;
        camera_rotation_output[2] = fast_add_rotations(pick, 3.14159274f);
    } else {
        base = (char *)l02_cuboids;
        qcopy(pos, base + camera_transform_index * 128 + 0x30);
        qcopy(camera_rotation_output, base + camera_transform_index * 128 + 0x70);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002D9F08), where it is exact; names translated to the US level program. */

/* Fakematch: the first parameter is an int; as a long the 0 passed after the 64-bit pad test is set again instead of taken from the mask register, like retail. */

void tresspasser_pad_update(Moby *moby) __asm__("FUN_L02_002d8ad0");

void tresspasser_pad_update(Moby *moby) {
    enum {
        TRESSPASSER_PAD_STATE_INITIALIZE = 0,
        TRESSPASSER_PAD_STATE_WAIT_FOR_TOOL_ACTIVATION = 1,
        TRESSPASSER_PAD_STATE_ACTIVE_PUZZLE = 2,
        TRESSPASSER_PAD_STATE_COMMIT_COMPLETION = 3
    };
    Vec_2D9F08 camera_pos, camera_rot;
    Data      *data = (struct Data *)moby->pvars;
    int        i;

    if (data == 0) {
        return;
    }
    switch (moby->state) {
    case TRESSPASSER_PAD_STATE_INITIALIZE:
        data->h12 = 0;
        moby->state = 1;
        D_L02_001D33B0[2][1] = 17.0f;
        D_L02_001D33B0[0][1] = 17.0f;
        D_L02_001D33B0[1][0] = 17.0f;
        D_L02_001D33B0[1][1] = -17.0f;
        D_L02_001D33B0[2][0] = -17.0f;
        D_L02_001D33B0[3][0] = -17.0f;
        D_L02_001D33B0[3][1] = -17.0f;
        D_L02_001D33B0[0][0] = 17.0f;
        D_L02_00161B1C_gp = data->iEC;
        if (moby->mission != 0xFF) {
            if (D_L02_001BBB40_2D9F08.collected[(short)moby->save_id] != 0 ||
                (((int *)((char *)D_0014C190_257218 +
                          (current_level_index_macro << 8)))[(short)moby->save_id >> 5] >>
                 (moby->save_id & 0x1F)) &
                    1) {
                moby->state = 4;
            }
        }
        break;
    case TRESSPASSER_PAD_STATE_WAIT_FOR_TOOL_ACTIVATION:
        if (((Hero *)(D_0013E633_2D9F08 + 0xE1D))->ground_moby == moby &&
            ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->air_frames.s == 0) {
            int holding_tool;

            if (data->h12 == 0) {
                data->h12 = 1;
                allocate_voice_for_target_entry(3, 0, moby);
            }
            if (((Hero *)(D_0013E633_2D9F08 + 0xE1D))->items[0].moby != 0 &&
                ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->items[0].item_id == 0x1A) {
                moby->glow_color = D_L02_00161AA0_gp;
                holding_tool = 1;
            } else {
                moby->glow_color = D_L02_00161AA4_gp;
                holding_tool = 0;
            }
            if (holding_tool &&
                ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->items[0].moby->command != 0) {
                data->h10 = scale_game_frames(0x12C);
                data->h11C = 0;
                data->h11E = 0;
                hero_set_state(0x72, 0);
                set_hero_animation(0x57, 0, scale_game_frames(6));
                jet_mount(&D_L02_00167440_2D9F08[0], &D_L02_00167440_2D9F08[1], 1, 0, 0);
                moby->state = 2;
                data->h10 = scale_game_frames(0x3C);
                data->f110 = 0.0f;
                qcopy(&data->vF0, &D_L02_00167440_2D9F08[0]);
                qcopy(&data->v100, &D_L02_00167440_2D9F08[1]);
            }
        } else {
            data->h12 = 0;
            moby->glow_color = D_L02_00161AA8_gp;
        }
        break;
    case TRESSPASSER_PAD_STATE_ACTIVE_PUZZLE:
        func_L02_002DA728_2D9F08(moby);
        func_L02_002D9D48_2D9F08(&camera_pos, &camera_rot, moby);
        advance_accelerated_scalar(&data->f110,
                                   &data->f114,
                                   1.0f,
                                   frame_time_sq_macro + frame_time_sq_macro,
                                   frame_time_sq_macro + frame_time_sq_macro,
                                   frame_time_macro + frame_time_macro);
        lerp_vector(&camera_pos, &data->vF0, &camera_pos, data->f110);
        camera_rot.z = fast_add_rotations(
            fast_subtract_rotations(camera_rot.z, data->v100.z) * data->f110, data->v100.z);
        camera_rot.y = fast_add_rotations(
            fast_subtract_rotations(camera_rot.y, data->v100.y) * data->f110, data->v100.y);
        set_camera_position(&camera_pos);
        set_camera_rotation(&camera_rot);
        D_L02_0017EB08_lui = 1;
        force_help_message(6, 0);
        if (l02_camera.unk180 != 0 && l02_camera.unk180->unk86 != 5) {
            jet_mount(&l02_camera.pos, &l02_camera.unk150, 1, 0, 0);
        }
        i = 0;
        if (0.99f <= data->f110 &&
            fast_difference_between_rotations(l02_camera.yaw, camera_rot.z) < 0.017453292f) {
            add_draw_callback(func_L02_002DA820_2D9F08, moby);
            for (i = 0; i < 12; i++) {
                if (data->iB0[i] == 1) {
                    break;
                }
            }
            if (0.0f < data->fE4) {
                data->fE4 = fast_add_rotations(data->fE4, frame_time_macro * 6.2831855f);
                if (0.5235988f < data->fE4) {
                    data->fE4 = 0.0f;
                    if (data->iE0 == 1) {
                        data->i14 += 11;
                        data->i14 %= 12;
                    } else if (data->iE0 == 2) {
                        data->i18 += 11;
                        data->i18 %= 12;
                    } else {
                        data->i1C += 11;
                        data->i1C %= 12;
                    }
                }
            } else if (data->fE4 < 0.0f) {
                data->fE4 = fast_add_rotations(data->fE4, -(frame_time_macro * 6.2831855f));
                if (data->fE4 < -0.5235988f) {
                    data->fE4 = 0.0f;
                    if (data->iE0 == 1) {
                        data->i14 += 13;
                        data->i14 %= 12;
                    } else if (data->iE0 == 2) {
                        data->i18 += 13;
                        data->i18 %= 12;
                    } else {
                        data->i1C += 13;
                        data->i1C %= 12;
                    }
                }
            } else if (i == 12) {
                if (fast_dec_timer_s16(&data->h10)) {
                    moby->state = 3;
                }
            } else if (((Pad *)D_0013C940)->b1C0.l & 0xA00000000000) {
                data->h11E |= 1;
                allocate_voice_for_target_entry(0, 0, moby);
                if (((Pad *)D_0013C940)->b1C0.w.hi & 0x8000) {
                    data->fE4 = -(frame_time_macro * 6.2831855f);
                } else {
                    data->fE4 = frame_time_macro * 6.2831855f;
                }
            } else if (((Pad *)D_0013C940)->i1A4 & 0x1000) {
                data->h11E |= 2;
                data->iE0 = data->iE0 - 1;
                if (data->iE0 == 0) {
                    data->iE0 = 1;
                } else {
                    allocate_voice_for_target_entry(1, 0, moby);
                }
            } else if (((Pad *)D_0013C940)->i1A4 & 0x4000) {
                data->h11E |= 2;
                data->iE0 = data->iE0 + 1;
                if (data->iE0 == 4) {
                    data->iE0 = 3;
                } else {
                    allocate_voice_for_target_entry(1, 0, moby);
                }
            }
        } else if (hero.anim_flags & 2) {
            set_hero_animation(0x58, 0, scale_game_frames(6));
        }
        if (i != 12) {
            if (((D_0013CAE4 & 0x10) && D_L02_00179A10.i0 == 0 && D_L02_00179A10.i24 == -1) ||
                ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->state.current != 0x72 ||
                ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->items[0].item_id != 0x1A) {
                moby->state = 1;
                jet_end_mount(1);
                request_hero_idle_state_for_vehicle_mode();
                data->h10 = scale_game_frames(0x3C);
            }
        }
        break;
    case TRESSPASSER_PAD_STATE_COMMIT_COMPLETION:
        if (hero.anim_flags & 2) {
            if (moby->mission != 0xFF) {
                ((int *)((char *)D_0014C190_257218 +
                         (current_level_index_macro << 8)))[(short)moby->save_id >> 5] |=
                    1 << (moby->save_id & 0x1F);
                D_L02_001BADE0_2D9F08[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
            }
            jet_end_mount(1);
            hero_set_state(0, 1);
            moby->command = 1;
            moby->state = 4;
        }
        break;
    }
    func_L00_0025E590_2D9F08(moby, data);
}

void align_hero_to_tresspasser_pad(struct Moby *moby) __asm__("FUN_L02_002d92f0");

void align_hero_to_tresspasser_pad(struct Moby *moby) {
    float        position_delta[4];
    char        *hero_position;
    float        f;
    float        max_yaw_step;
    struct Hero *hero_ptr;
    moby_point_position(moby, 0, position_delta);
    hero_position = (char *)&hero.motion.pos;
    fast_vec_sub(position_delta, position_delta, hero_position);
    func_L00_002607A8_d92f0(position_delta, frame_time * 4.0f);
    fast_vec_add(hero_position, hero_position, position_delta);
    f = fast_add_rotations(moby->rot.z, 3.14159f);
    f = fast_subtract_rotations(f, *(float *)(hero_position + 0x18));
    max_yaw_step = frame_time * 6.2831855f;
    if (max_yaw_step < f) {
        f = max_yaw_step;
    } else if (f < -max_yaw_step) {
        f = -max_yaw_step;
    }
    hero_ptr = &hero;
    hero_ptr->motion.rot.f[2] = fast_add_rotations(f, hero_ptr->motion.rot.f[2]);
}

#define NEG20(dst, e)                             \
    {                                             \
        s32 q_ = (e);                             \
        q_ = q_ << 20;                            \
        (dst) += (u64_t)(u32)(-q_ + 0x80000000u); \
    }
#define LO4(dst, e)              \
    {                            \
        s32 q_ = (e);            \
        q_ = (q_ << 4) + 0x8000; \
        (dst) += (u64_t)q_;      \
    }

#define draw_tresspasser_puzzle_ring_arc \
    ((void (*)(u64, float, float, float, float, u32))draw_tresspasser_puzzle_ring_arc)
#define draw_tresspasser_puzzle_arrow \
    ((void (*)(u64, int, int, float, float))draw_tresspasser_puzzle_arrow)
#define draw_tresspasser_puzzle_radial_strip \
    ((void (*)(u64, f32, f32, f32, s32))draw_tresspasser_puzzle_radial_strip)

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002DA820), where it is exact; names translated to the US level program. */

void draw_tresspasser_puzzle(char *moby) {
    /* Fakematch: the texture sits at retail's stack offset inside this frame. */
    struct {
        char pad0[0x78];
        u64  texture;
        char pad80[0x10];
    } ctx;
    int     first_step, second_step;
    char   *data = ((char *)MOBY(moby)->pvars);
    Ring12 *markers = (Ring12 *)(data + 0xb0);
    Ring12 *first, *second, *third;

    int   i, state, third_step;
    float phase;

    vu1_add_g_sregister(0x47, 0x5380b);
    ctx.texture = get_effect_texture(0x10);
    draw_tresspasser_puzzle_textured_quads();
    {
        int marker_reset_countdown, *marker_cursor;
        for (marker_reset_countdown = 11, marker_cursor = markers->slot;
             marker_reset_countdown >= 0;
             --marker_reset_countdown, ++marker_cursor) {
            if (*marker_cursor != 0)
                *marker_cursor = 1;
        }
    }
    first_step = 0;
    second_step = 0;
    third_step = 0;
    if (*(float *)(data + 0xe4) > 0.31415927f) {
        state = *(int *)(data + 0xe0);
        if (state == 1)
            first_step = 1;
        else if (state == 2)
            second_step = 1;
        else
            third_step = 1;
    } else if (*(float *)(data + 0xe4) < -0.31415927f) {
        state = *(int *)(data + 0xe0);
        if (state == 1)
            first_step = -1;
        else if (state == 2)
            second_step = -1;
        else
            third_step = -1;
    }

    i = 0;
    first = (Ring12 *)(data + 0x20);
    second = (Ring12 *)(data + 0x50);
    third = (Ring12 *)(data + 0x80);
    {
        int   index, other, slot, draw_second, color;
        float angle, distance, phase;
        for (; i < 12; ++i) {
            draw_second = 1;
            phase = -0.31415927f;
            if (*(int *)(data + 0xe0) == 1)
                phase = fast_add_rotations(phase, *(float *)(data + 0xe4));
            angle = fast_add_rotations(phase, (float)(i * 30) * 0.017453292f);
            index = i + *(int *)(data + 0x14);
            slot = index % 12;
            if (first->slot[slot] == 2) {
                other = i + *(int *)(data + 0x18) + first_step - second_step;
                color = tresspasser_default_radial_strip_color_gp;
                if (second->slot[other % 12] != 0)
                    distance = (float)tresspasser_second_ring_arrow_distance_gp;
                else {
                    int third_index = i + *(int *)(data + 0x1c) + first_step - third_step;
                    if (third->slot[third_index % 12] != 0)
                        distance = (float)tresspasser_third_ring_arrow_distance_gp;
                    else if (third->slot[(third_index + 6) % 12] != 0)
                        distance = (float)-tresspasser_third_ring_arrow_distance_gp;
                    else if (second->slot[(other + 6) % 12] != 0)
                        distance = (float)-tresspasser_second_ring_arrow_distance_gp;
                    else if (first->slot[(index + 6) % 12] != 0)
                        distance = (float)-tresspasser_first_ring_arrow_distance_gp;
                    else {
                        int opposite;
                        distance = (float)-tresspasser_unintercepted_strip_endpoint_distance_gp;
                        draw_second = 0;
                        opposite = first_step + 6;
                        if (markers->slot[(i + opposite) % 12] != 0) {
                            int marker = (i + opposite) % 12;
                            first->slot[slot] = 3;
                            markers->slot[marker] = 2;
                            color = tresspasser_reached_marker_strip_color_gp;
                        }
                    }
                }
                draw_tresspasser_puzzle_radial_strip(
                    ctx.texture,
                    fast_add_rotations(angle, 0.31415927f),
                    (float)tresspasser_first_ring_arrow_distance_gp,
                    distance,
                    color);
                if (draw_second) {
                    draw_tresspasser_puzzle_radial_strip(
                        ctx.texture,
                        fast_add_rotations(angle, 0.31415927f),
                        distance,
                        (float)tresspasser_first_ring_arrow_distance_gp,
                        tresspasser_reverse_radial_strip_color_gp);
                }
            }
        }
    }
    {
        int   i, index, other, slot, draw_second, color;
        float angle, distance, phase;
        for (i = 0; i < 12; ++i) {
            draw_second = 1;
            phase = -0.31415927f;
            color = tresspasser_default_radial_strip_color_gp;
            if (*(int *)(data + 0xe0) == 2)
                phase = fast_add_rotations(phase, *(float *)(data + 0xe4));
            angle = fast_add_rotations(phase, (float)(i * 30) * 0.017453292f);
            index = i + *(int *)(data + 0x18);
            slot = index % 12;
            if (second->slot[slot] == 2) {
                other = i + *(int *)(data + 0x1c) + second_step - third_step;
                if (third->slot[other % 12] != 0)
                    distance = (float)tresspasser_third_ring_arrow_distance_gp;
                else if (third->slot[(other + 6) % 12] != 0)
                    distance = (float)-tresspasser_third_ring_arrow_distance_gp;
                else if (second->slot[(index + 6) % 12] != 0)
                    distance = (float)-tresspasser_second_ring_arrow_distance_gp;
                else if (first->slot[(i + *(int *)(data + 0x14) + second_step - first_step + 6) %
                                     12] != 0)
                    distance = (float)-tresspasser_first_ring_arrow_distance_gp;
                else {
                    int opposite;
                    distance = (float)-tresspasser_unintercepted_strip_endpoint_distance_gp;
                    draw_second = 0;
                    opposite = second_step + 6;
                    if (markers->slot[(i + opposite) % 12] != 0) {
                        int marker = (i + opposite) % 12;
                        second->slot[slot] = 3;
                        markers->slot[marker] = 2;
                        color = tresspasser_reached_marker_strip_color_gp;
                    }
                }
                draw_tresspasser_puzzle_radial_strip(
                    ctx.texture,
                    fast_add_rotations(angle, 0.31415927f),
                    (float)tresspasser_second_ring_arrow_distance_gp,
                    distance,
                    color);
                if (draw_second) {
                    draw_tresspasser_puzzle_radial_strip(
                        ctx.texture,
                        fast_add_rotations(angle, 0.31415927f),
                        distance,
                        (float)tresspasser_second_ring_arrow_distance_gp,
                        tresspasser_reverse_radial_strip_color_gp);
                }
            }
        }
    }
    {
        int   i, index, other, slot, draw_second, color;
        float angle, distance, phase;
        for (i = 0; i < 12; ++i) {
            draw_second = 1;
            phase = -0.31415927f;
            color = tresspasser_default_radial_strip_color_gp;
            if (*(int *)(data + 0xe0) == 3)
                phase = fast_add_rotations(phase, *(float *)(data + 0xe4));
            angle = fast_add_rotations(phase, (float)(i * 30) * 0.017453292f);
            index = i + *(int *)(data + 0x1c);
            slot = index % 12;
            if (third->slot[slot] == 2) {
                if (third->slot[(index + 6) % 12] != 0)
                    distance = (float)-tresspasser_third_ring_arrow_distance_gp;
                else if (second->slot[(i + *(int *)(data + 0x18) + third_step - second_step + 6) %
                                      12] != 0)
                    distance = (float)-tresspasser_second_ring_arrow_distance_gp;
                else if (first->slot[(i + *(int *)(data + 0x14) + third_step - first_step + 6) %
                                     12] != 0)
                    distance = (float)-tresspasser_first_ring_arrow_distance_gp;
                else {
                    int opposite;
                    distance = (float)-tresspasser_unintercepted_strip_endpoint_distance_gp;
                    draw_second = 0;
                    opposite = third_step + 6;
                    if (markers->slot[(i + opposite) % 12] != 0) {
                        int marker = (i + opposite) % 12;
                        third->slot[slot] = 3;
                        markers->slot[marker] = 2;
                        color = tresspasser_reached_marker_strip_color_gp;
                    }
                }
                draw_tresspasser_puzzle_radial_strip(
                    ctx.texture,
                    fast_add_rotations(angle, 0.31415927f),
                    (float)tresspasser_third_ring_arrow_distance_gp,
                    distance,
                    color);
                if (draw_second) {
                    draw_tresspasser_puzzle_radial_strip(
                        ctx.texture,
                        fast_add_rotations(angle, 0.31415927f),
                        distance,
                        (float)tresspasser_third_ring_arrow_distance_gp,
                        tresspasser_reverse_radial_strip_color_gp);
                }
            }
        }
    }
    {
        int   i, color;
        float angle;
        for (i = 0; i < 12; ++i) {
            angle = -0.2617994f;
            if (*(int *)(data + 0xe0) == 1) {
                color = fast_tween_color(
                    tresspasser_selected_ring_base_color_gp,
                    tresspasser_selected_ring_base_color_gp & 0xffffff,
                    fast_sin((float)((D_L02_0015F5CC % 60) * 6) * 0.017453292f) * 0.5f + 0.5f);
                if (*(float *)(data + 0xe4) != 0.0f)
                    angle = fast_add_rotations(angle, *(float *)(data + 0xe4));
            } else
                color = tresspasser_unselected_ring_color_gp;
            angle = fast_add_rotations(angle, (float)(i * 30) * 0.017453292f);
            phase = fast_add_rotations(angle, 0.5235988f);
            draw_tresspasser_puzzle_ring_arc(
                ctx.texture,
                angle,
                phase,
                (float)tresspasser_first_ring_arc_first_boundary_radius_gp,
                (float)tresspasser_first_ring_arc_second_boundary_radius_gp,
                color);
            if (first->slot[(i + *(int *)(data + 0x14)) % 12] != 0) {
                if (first->slot[(i + *(int *)(data + 0x14)) % 12] >= 3) {
                    draw_tresspasser_puzzle_arrow(ctx.texture,
                                                  tresspasser_arrow_unused_argument_gp,
                                                  0,
                                                  fast_add_rotations(angle, 0.2617994f),
                                                  (float)tresspasser_first_ring_arrow_distance_gp);
                    first->slot[(i + *(int *)(data + 0x14)) % 12] = 2;
                } else if (first->slot[(i + *(int *)(data + 0x14)) % 12] == 2) {
                    draw_tresspasser_puzzle_arrow(ctx.texture,
                                                  tresspasser_arrow_unused_argument_gp,
                                                  1,
                                                  fast_add_rotations(angle, 0.2617994f),
                                                  (float)tresspasser_first_ring_arrow_distance_gp);
                } else {
                    draw_tresspasser_puzzle_arrow(ctx.texture,
                                                  tresspasser_arrow_unused_argument_gp,
                                                  2,
                                                  fast_add_rotations(angle, 0.2617994f),
                                                  (float)tresspasser_first_ring_arrow_distance_gp);
                }
            }
        }
    }
    {
        int   i, color;
        float angle;
        for (i = 0; i < 12; ++i) {
            angle = -0.2617994f;
            if (*(int *)(data + 0xe0) == 2) {
                color = fast_tween_color(
                    tresspasser_selected_ring_base_color_gp,
                    tresspasser_selected_ring_base_color_gp & 0xffffff,
                    fast_sin((float)((D_L02_0015F5CC % 60) * 6) * 0.017453292f) * 0.5f + 0.5f);
                if (*(float *)(data + 0xe4) != 0.0f)
                    angle = fast_add_rotations(angle, *(float *)(data + 0xe4));
            } else
                color = tresspasser_unselected_ring_color_gp;
            angle = fast_add_rotations(angle, (float)(i * 30) * 0.017453292f);
            phase = fast_add_rotations(angle, 0.5235988f);
            draw_tresspasser_puzzle_ring_arc(
                ctx.texture,
                angle,
                phase,
                (float)tresspasser_second_ring_arc_first_boundary_radius_gp,
                (float)tresspasser_second_ring_arc_second_boundary_radius_gp,
                color);
            if (second->slot[(i + *(int *)(data + 0x18)) % 12] != 0) {
                if (second->slot[(i + *(int *)(data + 0x18)) % 12] >= 3) {
                    draw_tresspasser_puzzle_arrow(ctx.texture,
                                                  tresspasser_arrow_unused_argument_gp,
                                                  0,
                                                  fast_add_rotations(angle, 0.2617994f),
                                                  (float)tresspasser_second_ring_arrow_distance_gp);
                    second->slot[(i + *(int *)(data + 0x18)) % 12] = 2;
                } else if (second->slot[(i + *(int *)(data + 0x18)) % 12] == 2) {
                    draw_tresspasser_puzzle_arrow(ctx.texture,
                                                  tresspasser_arrow_unused_argument_gp,
                                                  1,
                                                  fast_add_rotations(angle, 0.2617994f),
                                                  (float)tresspasser_second_ring_arrow_distance_gp);
                } else {
                    draw_tresspasser_puzzle_arrow(ctx.texture,
                                                  tresspasser_arrow_unused_argument_gp,
                                                  2,
                                                  fast_add_rotations(angle, 0.2617994f),
                                                  (float)tresspasser_second_ring_arrow_distance_gp);
                }
            }
        }
    }
    {
        int   i, color;
        float angle;
        for (i = 0; i < 12; ++i) {
            angle = -0.2617994f;
            if (*(int *)(data + 0xe0) == 3) {
                color = fast_tween_color(
                    tresspasser_selected_ring_base_color_gp,
                    tresspasser_selected_ring_base_color_gp & 0xffffff,
                    fast_sin((float)((D_L02_0015F5CC % 60) * 6) * 0.017453292f) * 0.5f + 0.5f);
                if (*(float *)(data + 0xe4) != 0.0f)
                    angle = fast_add_rotations(angle, *(float *)(data + 0xe4));
            } else
                color = tresspasser_unselected_ring_color_gp;
            angle = fast_add_rotations(angle, (float)(i * 30) * 0.017453292f);
            phase = fast_add_rotations(angle, 0.5235988f);
            draw_tresspasser_puzzle_ring_arc(
                ctx.texture,
                angle,
                phase,
                (float)tresspasser_third_ring_arc_first_boundary_radius_gp,
                (float)tresspasser_third_ring_arc_second_boundary_radius_gp,
                color);
            if (third->slot[(i + *(int *)(data + 0x1c)) % 12] != 0) {
                if (third->slot[(i + *(int *)(data + 0x1c)) % 12] >= 3) {
                    draw_tresspasser_puzzle_arrow(ctx.texture,
                                                  tresspasser_arrow_unused_argument_gp,
                                                  0,
                                                  fast_add_rotations(angle, 0.2617994f),
                                                  (float)tresspasser_third_ring_arrow_distance_gp);
                    third->slot[(i + *(int *)(data + 0x1c)) % 12] = 2;
                } else if (third->slot[(i + *(int *)(data + 0x1c)) % 12] == 2) {
                    draw_tresspasser_puzzle_arrow(ctx.texture,
                                                  tresspasser_arrow_unused_argument_gp,
                                                  1,
                                                  fast_add_rotations(angle, 0.2617994f),
                                                  (float)tresspasser_third_ring_arrow_distance_gp);
                } else {
                    draw_tresspasser_puzzle_arrow(ctx.texture,
                                                  tresspasser_arrow_unused_argument_gp,
                                                  2,
                                                  fast_add_rotations(angle, 0.2617994f),
                                                  (float)tresspasser_third_ring_arrow_distance_gp);
                }
            }
        }
    }
    {
        int   value, color;
        float angle;
        for (i = 0; i < 12; ++i) {
            angle = fast_add_rotations(-0.08726646f, (float)(i * 30) * 0.017453292f);
            phase = fast_add_rotations(angle, 0.17453292f);
            value = markers->slot[i];
            if (value != 0) {
                if (value == 2)
                    color = tresspasser_reached_marker_arc_color_gp;
                else if (D_L02_0015F5CC & 0x20)
                    color = tresspasser_unreached_marker_arc_flag_set_color_gp;
                else
                    color = tresspasser_unreached_marker_arc_flag_clear_color_gp;
                draw_tresspasser_puzzle_ring_arc(
                    ctx.texture,
                    angle,
                    phase,
                    (float)tresspasser_marker_arc_first_boundary_radius_gp,
                    (float)tresspasser_marker_arc_second_boundary_radius_gp,
                    color);
            }
        }
    }
    vu1_add_g_sregister(0x47, 0x5360b);
}

#undef draw_tresspasser_puzzle_ring_arc
#undef draw_tresspasser_puzzle_arrow
#undef draw_tresspasser_puzzle_radial_strip

/* Draws an elliptical arc from start_angle to end_angle (radii radius_x, radius_y, around the screen centre) as five sprite
 * segments in the given colour. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002DB810), where it is exact; names translated to the US level program. */

void draw_tresspasser_puzzle_ring_arc(int   texture,
                                      u32   color,
                                      float start_angle,
                                      float end_angle,
                                      float radius_x,
                                      float radius_y) __asm__("FUN_L02_002da3d8");

void draw_tresspasser_puzzle_ring_arc(

    int texture, u32 color, float start_angle, float end_angle, float radius_x, float radius_y) {
    long  corners[4];
    u32   colors[4];
    int   uvs[4];
    float angle_step = fast_subtract_rotations(end_angle, start_angle) / 5.0f;
    int   i;
    uvs[0] = tresspasser_ring_arc_first_vertex_uv_gp;
    uvs[1] = tresspasser_ring_arc_second_vertex_uv_gp;
    uvs[2] = tresspasser_ring_arc_third_vertex_uv_gp;
    uvs[3] = tresspasser_ring_arc_fourth_vertex_uv_gp;
    colors[3] = color;
    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    for (i = 0; i < 5; i++) {
        float angle0 = fast_add_rotations(start_angle, angle_step * (float)i);
        float angle1 = fast_add_rotations(start_angle, angle_step * (float)(i + 1));
        corners[3] = 0xFFFFF000000000L;
        corners[2] = 0xFFFFF000000000L;
        corners[1] = 0xFFFFF000000000L;
        corners[0] = 0xFFFFF000000000L;
        corners[0] += (u32)(-(truncate_float_to_s32(fast_cos(angle0) * radius_x * 16.0f) << 16) -
                            (int)0x80000000);
        corners[0] += truncate_float_to_s32(fast_sin(angle0) * radius_x * 16.0f) + 0x8000;
        corners[1] += (u32)(-(truncate_float_to_s32(fast_cos(angle0) * radius_y * 16.0f) << 16) -
                            (int)0x80000000);
        corners[1] += truncate_float_to_s32(fast_sin(angle0) * radius_y * 16.0f) + 0x8000;
        corners[2] += (u32)(-(truncate_float_to_s32(fast_cos(angle1) * radius_x * 16.0f) << 16) -
                            (int)0x80000000);
        corners[2] += truncate_float_to_s32(fast_sin(angle1) * radius_x * 16.0f) + 0x8000;
        corners[3] += (u32)(-(truncate_float_to_s32(fast_cos(angle1) * radius_y * 16.0f) << 16) -
                            (int)0x80000000);
        corners[3] += truncate_float_to_s32(fast_sin(angle1) * radius_y * 16.0f) + 0x8000;
        FUN_L02_0020bc88(corners, uvs, colors, texture, 1);
    }
}

/* Draws the rotating radar arrow sprite (style sel) at angle a and scale s, twice. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002DBAF8), where it is exact; names translated to the US level program. */

void draw_tresspasser_puzzle_arrow(int   unused_texture,
                                   int   unused_parameter,
                                   int   sel,
                                   float angle,
                                   float radial_distance) __asm__("FUN_L02_002da6c0");

void draw_tresspasser_puzzle_arrow(

    int unused_texture, int unused_parameter, int sel, float angle, float radial_distance) {
    enum {
        ARROW_STYLE_NEWLY_REACHED = 0,
        ARROW_STYLE_OCCUPIED = 1,
        ARROW_STYLE_LIT = 2
    };
    long  xy[4];
    u32   col[4];
    int   uv[4];
    float M[4][4];
    float arrow_rotation_angles[4];
    int   x, y;
    long  packed_arrow_center;
    s64   texture_tex0;
    switch (sel) {
    case ARROW_STYLE_NEWLY_REACHED:
        uv[0] = tresspasser_arrow_newly_reached_first_vertex_uv_gp;
        uv[1] = tresspasser_arrow_newly_reached_second_vertex_uv_gp;
        uv[2] = tresspasser_arrow_newly_reached_third_vertex_uv_gp;
        uv[3] = tresspasser_arrow_newly_reached_fourth_vertex_uv_gp;
        break;
    case ARROW_STYLE_OCCUPIED:
        uv[0] = tresspasser_arrow_occupied_first_vertex_uv_gp;
        uv[1] = tresspasser_arrow_occupied_second_vertex_uv_gp;
        uv[2] = tresspasser_arrow_occupied_third_vertex_uv_gp;
        uv[3] = tresspasser_arrow_occupied_fourth_vertex_uv_gp;
        break;
    case ARROW_STYLE_LIT:
    default:
        uv[0] = tresspasser_arrow_lit_first_vertex_uv_gp;
        uv[1] = tresspasser_arrow_lit_second_vertex_uv_gp;
        uv[2] = tresspasser_arrow_lit_third_vertex_uv_gp;
        uv[3] = tresspasser_arrow_lit_fourth_vertex_uv_gp;
        break;
    }
    col[3] = tresspasser_arrow_vertex_color_gp;
    col[2] = tresspasser_arrow_vertex_color_gp;
    col[1] = tresspasser_arrow_vertex_color_gp;
    col[0] = tresspasser_arrow_vertex_color_gp;
    y = truncate_float_to_s32(fast_sin(angle) * radial_distance * 16.0f);
    x = truncate_float_to_s32(fast_cos(angle) * radial_distance * 16.0f);
    packed_arrow_center = (long)y + 0xFFFFF080000000L;
    packed_arrow_center -= x << 16;
    packed_arrow_center += 0x8000;
    xy[3] = packed_arrow_center;
    xy[2] = packed_arrow_center;
    xy[1] = packed_arrow_center;
    xy[0] = packed_arrow_center;
    clear_u64_value(arrow_rotation_angles);
    arrow_rotation_angles[2] = -angle;
    euler_rotation_matrix(M, arrow_rotation_angles);
    multiply_matrix(M, M, D_L02_001D33B0);
    xy[0] += truncate_float_to_s32(M[0][0] * 16.0f) << 16;
    xy[0] += truncate_float_to_s32(M[0][1] * 16.0f);
    xy[1] += truncate_float_to_s32(M[1][0] * 16.0f) << 16;
    xy[1] += truncate_float_to_s32(M[1][1] * 16.0f);
    xy[2] += truncate_float_to_s32(M[2][0] * 16.0f) << 16;
    xy[2] += truncate_float_to_s32(M[2][1] * 16.0f);
    xy[3] += truncate_float_to_s32(M[3][0] * 16.0f) << 16;
    xy[3] += truncate_float_to_s32(M[3][1] * 16.0f);
    texture_tex0 = get_effect_texture(D_L02_00161B1C_gp + 0x29);
    FUN_L02_0020bc88(xy, uv, col, texture_tex0, 1);
    FUN_L02_0020bc88(xy, uv, col, texture_tex0, 1);
}

void draw_tresspasser_puzzle_radial_strip(void *unused,
                                          f32   angle,
                                          f32   start_distance,
                                          f32   end_distance,
                                          s32   color) __asm__("FUN_L02_002da938");

void draw_tresspasser_puzzle_radial_strip(

    void *unused, f32 angle, f32 start_distance, f32 end_distance, s32 color) {
    u64_t packed_vertices[4];
    s32   vertex_colors[4];
    s32   vertex_uvs[4];
    s32   unwrapped_uv_scroll;
    s32   wrapped_uv_scroll_offset;
    s32   strip_texture_tex0;
    f32   start_cosine_component, start_sine_component, start_corner_cosine_component,
        end_cosine_component, end_sine_component, start_corner_sine_component,
        end_corner_cosine_component, end_corner_sine_component;
    f32 trig_value;

    unwrapped_uv_scroll = truncate_float_to_s32((f32)D_L02_0015F5CC * D_L02_00161B60);
    wrapped_uv_scroll_offset = (unwrapped_uv_scroll % 512) << 16;
    vertex_colors[3] = color;
    vertex_colors[2] = color;
    vertex_colors[1] = color;
    vertex_colors[0] = color;
    vertex_uvs[0] = (s32)(tresspasser_radial_strip_scrolling_first_vertex_uv_base_gp +
                          wrapped_uv_scroll_offset);
    vertex_uvs[1] = (s32)(tresspasser_radial_strip_scrolling_second_vertex_uv_base_gp +
                          wrapped_uv_scroll_offset);
    vertex_uvs[2] = (s32)(tresspasser_radial_strip_scrolling_third_vertex_uv_base_gp +
                          wrapped_uv_scroll_offset);
    vertex_uvs[3] = (s32)(tresspasser_radial_strip_scrolling_fourth_vertex_uv_base_gp +
                          wrapped_uv_scroll_offset);

    start_sine_component = fast_sin(angle) * start_distance;
    start_cosine_component = fast_cos(angle) * start_distance;
    end_sine_component = fast_sin(angle) * end_distance;
    end_cosine_component = fast_cos(angle) * end_distance;
    trig_value = fast_cos(angle);
    start_corner_sine_component = start_sine_component + trig_value * 2.0f;
    trig_value = fast_cos(angle);
    start_sine_component = start_sine_component - trig_value * 2.0f;
    trig_value = fast_sin(angle);
    start_corner_cosine_component = start_cosine_component - trig_value * 2.0f;
    trig_value = fast_sin(angle);
    start_cosine_component = start_cosine_component + trig_value * 2.0f;
    trig_value = fast_cos(angle);
    end_corner_sine_component = end_sine_component + trig_value * 2.0f;
    trig_value = fast_cos(angle);
    end_sine_component = end_sine_component - trig_value * 2.0f;
    trig_value = fast_sin(angle);
    end_corner_cosine_component = end_cosine_component - trig_value * 2.0f;
    trig_value = fast_sin(angle);
    end_cosine_component = end_cosine_component + trig_value * 2.0f;

    packed_vertices[3] = 0xFFFFF000000000ULL;
    packed_vertices[2] = 0xFFFFF000000000ULL;
    packed_vertices[1] = 0xFFFFF000000000ULL;
    packed_vertices[0] = 0xFFFFF000000000ULL;

    NEG20(packed_vertices[0], truncate_float_to_s32(start_corner_cosine_component));
    LO4(packed_vertices[0], truncate_float_to_s32(start_corner_sine_component));
    NEG20(packed_vertices[1], truncate_float_to_s32(start_cosine_component));
    LO4(packed_vertices[1], truncate_float_to_s32(start_sine_component));
    NEG20(packed_vertices[2], truncate_float_to_s32(end_corner_cosine_component));
    LO4(packed_vertices[2], truncate_float_to_s32(end_corner_sine_component));
    NEG20(packed_vertices[3], truncate_float_to_s32(end_cosine_component));
    LO4(packed_vertices[3], truncate_float_to_s32(end_sine_component));

    strip_texture_tex0 =
        ((s32(*)(s32))get_effect_texture)(tresspasser_radial_strip_texture_index_gp);
    FUN_L02_0020bc88_u(packed_vertices, vertex_uvs, vertex_colors, strip_texture_tex0, 0);

    vertex_uvs[0] = (s32)tresspasser_radial_strip_fixed_first_vertex_uv_gp;
    vertex_uvs[1] = (s32)tresspasser_radial_strip_fixed_second_vertex_uv_gp;
    vertex_uvs[2] = (s32)tresspasser_radial_strip_fixed_third_vertex_uv_gp;
    vertex_uvs[3] = (s32)tresspasser_radial_strip_fixed_fourth_vertex_uv_gp;

    start_sine_component = fast_sin(angle) * start_distance;
    start_cosine_component = fast_cos(angle) * start_distance;
    end_sine_component = fast_sin(angle) * end_distance;
    end_cosine_component = fast_cos(angle) * end_distance;
    trig_value = fast_cos(angle);
    start_corner_sine_component = start_sine_component + trig_value * 5.0f;
    trig_value = fast_cos(angle);
    start_sine_component = start_sine_component - trig_value * 5.0f;
    trig_value = fast_sin(angle);
    start_corner_cosine_component = start_cosine_component - trig_value * 5.0f;
    trig_value = fast_sin(angle);
    start_cosine_component = start_cosine_component + trig_value * 5.0f;
    trig_value = fast_cos(angle);
    end_corner_sine_component = end_sine_component + trig_value * 5.0f;
    trig_value = fast_cos(angle);
    end_sine_component = end_sine_component - trig_value * 5.0f;
    trig_value = fast_sin(angle);
    end_corner_cosine_component = end_cosine_component - trig_value * 5.0f;
    trig_value = fast_sin(angle);
    end_cosine_component = end_cosine_component + trig_value * 5.0f;

    packed_vertices[3] = 0xFFFFF000000000ULL;
    packed_vertices[2] = 0xFFFFF000000000ULL;
    packed_vertices[1] = 0xFFFFF000000000ULL;
    packed_vertices[0] = 0xFFFFF000000000ULL;

    NEG20(packed_vertices[0], truncate_float_to_s32(start_corner_cosine_component));
    LO4(packed_vertices[0], truncate_float_to_s32(start_corner_sine_component));
    NEG20(packed_vertices[1], truncate_float_to_s32(start_cosine_component));
    LO4(packed_vertices[1], truncate_float_to_s32(start_sine_component));
    NEG20(packed_vertices[2], truncate_float_to_s32(end_corner_cosine_component));
    LO4(packed_vertices[2], truncate_float_to_s32(end_corner_sine_component));
    NEG20(packed_vertices[3], truncate_float_to_s32(end_cosine_component));
    LO4(packed_vertices[3], truncate_float_to_s32(end_sine_component));

    FUN_L02_0020bc88_u(packed_vertices, vertex_uvs, vertex_colors, strip_texture_tex0, 0);
}

void draw_tresspasser_puzzle_textured_quads(void) __asm__("FUN_L02_002dae88");

void draw_tresspasser_puzzle_textured_quads(void) {
    Q_2dae88  packed_vertices;
    int       vertex_colors[4];
    int       vertex_uvs[4];
    Q_2dae88 *vertex_quad_cursor = l02_tresspasser_textured_quad_packed_vertices;
    int       i;
    for (i = 3; i >= 0; i--) {
        vertex_uvs[0] = tresspasser_quad_first_vertex_uv_gp;
        vertex_uvs[1] = tresspasser_quad_second_vertex_uv_gp;
        vertex_uvs[2] = tresspasser_quad_third_vertex_uv_gp;
        vertex_uvs[3] = tresspasser_quad_fourth_vertex_uv_gp;
        vertex_colors[0] = vertex_colors[1] = vertex_colors[2] = vertex_colors[3] =
            tresspasser_textured_quad_color_gp;
        packed_vertices = *vertex_quad_cursor++;
        f20bc88_2dae88(&packed_vertices,
                       vertex_uvs,
                       vertex_colors,
                       ((int (*)(int))get_effect_texture)(D_L02_00161B1C_gp + 0x28),
                       1);
    }
}

f32 D_L04_001D3010[1] __attribute__((section(".data"))) = {0};

f32 D_L04_001D3020[1] __attribute__((section(".data"))) = {0};

f32 D_L04_001D3030[1] __attribute__((section(".data"))) = {0};

f32 D_L04_001D3040[1] __attribute__((section(".data"))) = {0};

f32 D_L06_001D4AC0[1] __attribute__((section(".data"))) = {0};

f32 D_L06_001D4AD0[1] __attribute__((section(".data"))) = {0};

f32 D_L06_001D4AE0[1] __attribute__((section(".data"))) = {0};

f32 D_L06_001D4AF0[1] __attribute__((section(".data"))) = {0};

f32 D_L08_001E93A0[1] __attribute__((section(".data"))) = {0};

f32 D_L08_001E93B0[1] __attribute__((section(".data"))) = {0};

f32 D_L08_001E93C0[1] __attribute__((section(".data"))) = {0};

f32 D_L08_001E93D0[1] __attribute__((section(".data"))) = {0};

f32 D_L11_001D3750[1] __attribute__((section(".data"))) = {0};

f32 D_L11_001D3760[1] __attribute__((section(".data"))) = {0};

f32 D_L11_001D3770[1] __attribute__((section(".data"))) = {0};

f32 D_L11_001D3780[1] __attribute__((section(".data"))) = {0};

f32 D_L13_001D3780[1] __attribute__((section(".data"))) = {0};

f32 D_L13_001D3790[1] __attribute__((section(".data"))) = {0};

f32 D_L13_001D37A0[1] __attribute__((section(".data"))) = {0};

f32 D_L13_001D37B0[1] __attribute__((section(".data"))) = {0};

f32 D_L18_001D3770[1] __attribute__((section(".data"))) = {0};

f32 D_L18_001D3780[1] __attribute__((section(".data"))) = {0};

f32 D_L18_001D3790[1] __attribute__((section(".data"))) = {0};

f32 D_L18_001D37A0[1] __attribute__((section(".data"))) = {0};

int   D_L04_00161A6C = 0x40;
int   D_L04_001619F8 = 0x80808080;
int   D_L04_001619F4 = 0x804040c0;
int   D_L04_001619F0 = 0x8040c040;
int   D_L04_00161A5C = 0x1000200;
int   D_L04_00161A60 = 0x1000000;
int   D_L04_00161A64 = 0x1000200;
int   D_L04_00161A68 = 0x1000000;
int   D_L04_00161A0C = 0x200;
int   D_L04_00161A10 = 0;
int   D_L04_00161A14 = 0x2000200;
int   D_L04_00161A18 = 0x2000000;
int   D_L04_00161A1C = 0x400;
int   D_L04_00161A20 = 0x200;
int   D_L04_00161A24 = 0x2000400;
int   D_L04_00161A28 = 0x2000200;
int   D_L04_00161A2C = 0x2000400;
int   D_L04_00161A30 = 0x2000000;
int   D_L04_00161A34 = 0x4000400;
int   D_L04_00161A38 = 0x4000000;
int   D_L04_00161AEC = 0x60808080;
f32   D_L04_00161AB0 = 16.0f;
char *D_L04_00161A48 = 0;
int   D_L04_00161AE4 = 0x60808080;
int   D_L04_001619FC = 0;
int   D_L04_00161A00 = 0x8000000;
int   D_L04_00161A04 = 0x800;
int   D_L04_00161A08 = 0x8000800;
int   D_L06_001619EC = 0x40;
int   D_L06_00161978 = 0x80808080;
int   D_L06_00161974 = 0x804040c0;
int   D_L06_00161970 = 0x8040c040;
int   D_L06_001619DC = 0x1000200;
int   D_L06_001619E0 = 0x1000000;
int   D_L06_001619E4 = 0x1000200;
int   D_L06_001619E8 = 0x1000000;
int   D_L06_0016198C = 0x200;
int   D_L06_00161990 = 0;
int   D_L06_00161994 = 0x2000200;
int   D_L06_00161998 = 0x2000000;
int   D_L06_0016199C = 0x400;
int   D_L06_001619A0 = 0x200;
int   D_L06_001619A4 = 0x2000400;
int   D_L06_001619A8 = 0x2000200;
int   D_L06_001619AC = 0x2000400;
int   D_L06_001619B0 = 0x2000000;
int   D_L06_001619B4 = 0x4000400;
int   D_L06_001619B8 = 0x4000000;
int   D_L06_00161A6C = 0x60808080;
f32   D_L06_00161A30 = 16.0f;
char *D_L06_001619C8 = 0;
int   D_L06_00161A64 = 0x60808080;
int   D_L06_0016197C = 0;
int   D_L06_00161980 = 0x8000000;
int   D_L06_00161984 = 0x800;
int   D_L06_00161988 = 0x8000800;
int   D_L08_00161EBC = 0x40;
int   D_L08_00161E48 = 0x80808080;
int   D_L08_00161E44 = 0x804040c0;
int   D_L08_00161E40 = 0x8040c040;
int   D_L08_00161EAC = 0x1000200;
int   D_L08_00161EB0 = 0x1000000;
int   D_L08_00161EB4 = 0x1000200;
int   D_L08_00161EB8 = 0x1000000;
int   D_L08_00161E5C = 0x200;
int   D_L08_00161E60 = 0;
int   D_L08_00161E64 = 0x2000200;
int   D_L08_00161E68 = 0x2000000;
int   D_L08_00161E6C = 0x400;
int   D_L08_00161E70 = 0x200;
int   D_L08_00161E74 = 0x2000400;
int   D_L08_00161E78 = 0x2000200;
int   D_L08_00161E7C = 0x2000400;
int   D_L08_00161E80 = 0x2000000;
int   D_L08_00161E84 = 0x4000400;
int   D_L08_00161E88 = 0x4000000;
int   D_L08_00161F3C = 0x60808080;
f32   D_L08_00161F00 = 16.0f;
char *D_L08_00161E98 = 0;
int   D_L08_00161F34 = 0x60808080;
int   D_L08_00161E4C = 0;
int   D_L08_00161E50 = 0x8000000;
int   D_L08_00161E54 = 0x800;
int   D_L08_00161E58 = 0x8000800;

Idle D_L11_00179E10 = {
    0,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1},
    -1
};

int   D_L11_00161B4C = 0x40;
int   D_L11_00161AD8 = 0x80808080;
int   D_L11_00161AD4 = 0x804040c0;
int   D_L11_00161AD0 = 0x8040c040;
int   D_L11_00161B3C = 0x1000200;
int   D_L11_00161B40 = 0x1000000;
int   D_L11_00161B44 = 0x1000200;
int   D_L11_00161B48 = 0x1000000;
int   D_L11_00161AEC = 0x200;
int   D_L11_00161AF0 = 0;
int   D_L11_00161AF4 = 0x2000200;
int   D_L11_00161AF8 = 0x2000000;
int   D_L11_00161AFC = 0x400;
int   D_L11_00161B00 = 0x200;
int   D_L11_00161B04 = 0x2000400;
int   D_L11_00161B08 = 0x2000200;
int   D_L11_00161B0C = 0x2000400;
int   D_L11_00161B10 = 0x2000000;
int   D_L11_00161B14 = 0x4000400;
int   D_L11_00161B18 = 0x4000000;
int   D_L11_00161BCC = 0x60808080;
f32   D_L11_00161B90 = 16.0f;
char *D_L11_00161B28 = 0;
int   D_L11_00161BC4 = 0x60808080;
int   D_L11_00161ADC = 0;
int   D_L11_00161AE0 = 0x8000000;
int   D_L11_00161AE4 = 0x800;
int   D_L11_00161AE8 = 0x8000800;
int   D_L13_00161AAC = 0x40;
int   D_L13_00161A38 = 0x80808080;
int   D_L13_00161A34 = 0x804040c0;
int   D_L13_00161A30 = 0x8040c040;
int   D_L13_00161A9C = 0x1000200;
int   D_L13_00161AA0 = 0x1000000;
int   D_L13_00161AA4 = 0x1000200;
int   D_L13_00161AA8 = 0x1000000;
int   D_L13_00161A4C = 0x200;
int   D_L13_00161A50 = 0;
int   D_L13_00161A54 = 0x2000200;
int   D_L13_00161A58 = 0x2000000;
int   D_L13_00161A5C = 0x400;
int   D_L13_00161A60 = 0x200;
int   D_L13_00161A64 = 0x2000400;
int   D_L13_00161A68 = 0x2000200;
int   D_L13_00161A6C = 0x2000400;
int   D_L13_00161A70 = 0x2000000;
int   D_L13_00161A74 = 0x4000400;
int   D_L13_00161A78 = 0x4000000;
int   D_L13_00161B2C = 0x60808080;
f32   D_L13_00161AF0 = 16.0f;
char *D_L13_00161A88 = 0;
int   D_L13_00161B24 = 0x60808080;
int   D_L13_00161A3C = 0;
int   D_L13_00161A40 = 0x8000000;
int   D_L13_00161A44 = 0x800;
int   D_L13_00161A48 = 0x8000800;
int   D_L18_00161B3C = 0x40;
int   D_L18_00161AC8 = 0x80808080;
int   D_L18_00161AC4 = 0x804040c0;
int   D_L18_00161AC0 = 0x8040c040;
int   D_L18_00161B2C = 0x1000200;
int   D_L18_00161B30 = 0x1000000;
int   D_L18_00161B34 = 0x1000200;
int   D_L18_00161B38 = 0x1000000;
int   D_L18_00161ADC = 0x200;
int   D_L18_00161AE0 = 0;
int   D_L18_00161AE4 = 0x2000200;
int   D_L18_00161AE8 = 0x2000000;
int   D_L18_00161AEC = 0x400;
int   D_L18_00161AF0 = 0x200;
int   D_L18_00161AF4 = 0x2000400;
int   D_L18_00161AF8 = 0x2000200;
int   D_L18_00161AFC = 0x2000400;
int   D_L18_00161B00 = 0x2000000;
int   D_L18_00161B04 = 0x4000400;
int   D_L18_00161B08 = 0x4000000;
int   D_L18_00161BBC = 0x60808080;
f32   D_L18_00161B80 = 16.0f;
char *D_L18_00161B18 = 0;
int   D_L18_00161BB4 = 0x60808080;
int   D_L18_00161ACC = 0;
int   D_L18_00161AD0 = 0x8000000;
int   D_L18_00161AD4 = 0x800;
int   D_L18_00161AD8 = 0x8000800;

float D_L02_001D33B0[4][4] = {0};

int tresspasser_arrow_newly_reached_fourth_vertex_uv_gp __asm__("D_L02_00161AC8") = 0x2000000;

int tresspasser_arrow_occupied_fourth_vertex_uv_gp __asm__("D_L02_00161AD8") = 0x2000200;

int tresspasser_arrow_lit_first_vertex_uv_gp __asm__("D_L02_00161ADC") = 0x2000400;
