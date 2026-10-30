/* Shared moby class visibomb: the visibomb gun held by the hero. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/input/pad_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

typedef struct {
    u8  pad0[0x1A4];
    s32 i1A4;
} C_b3058;

typedef struct {
    u8  pad0[0x140];
    f32 v140[4];
    u8  pad150[0x180 - 0x150];
    u8 *p180;
    u8  pad184[0x350 - 0x184];
    f32 f350;
    f32 f354;
    f32 f358;
    u8  pad35C[0x370 - 0x35C];
    f32 v370[4];
} S_b3058;

typedef struct {
    u8  pad0[0x68];
    u16 u68;
    u16 u6A;
    s32 i6C;
} R_b3058;

typedef struct {
    s32 i0;
    s32 i4;
} V_b3058;

typedef struct {
    u8       pad0[0x10];
    f32      v10[4];
    u8       pad20[0x34 - 0x20];
    u16      u34;
    u8       pad36[0x53 - 0x36];
    u8       b53;
    u8       pad54[0x70 - 0x54];
    u8       b70;
    u8       pad71[0x78 - 0x71];
    V_b3058 *v;
    u8       pad7C[0xD0 - 0x7C];
    f32      vD0[2];
    f32      fD8;
} O_b3058;

s32  FUN_L00_00233db8_b3058(s32, s32) __asm__("FUN_L00_00233db8");
void jet_camera_mode(void) __asm__("FUN_L00_00203b18");
s32  FUN_L00_002b58d8_b3058(void *, void *, f32, f32) __asm__("FUN_L00_002b58d8");

extern int D_0015EEA4;
extern s32 D_0015EEA4;

void visibomb_update(O_b3058 *o) __asm__("FUN_L00_002b3058");

void visibomb_update(O_b3058 *visibomb_moby) {
    V_b3058     *v;
    struct Hero *g;
    C_b3058     *controller;
    LevelCamera *camera;
    R_b3058     *r;
    f32          launch_yaw, b;
    s32          k;
    f32          launch_position[4] __attribute__((aligned(16)));
    f32          u[4] __attribute__((aligned(16)));

    if (visibomb_moby == 0) {
        return;
    }
    v = visibomb_moby->v;
    fast_dec_timer(v);
    controller = (C_b3058 *)&controller_state;
    g = &hero;
    if (!(controller->i1A4 & g->items[0].button_mask))
        goto tail;
    if (v->i0 != 0)
        goto tail;
    if (v->i4 != 0)
        goto tail;
    if (g->item_use_locked != 0)
        goto tail;
    k = g->state.control_mode;
    if (k == 7 || k == 2 || k == 3 || k == 4 || k == 5 || k == 6 || k == 0x10 || k == 0xE ||
        k == 9 || k == 0xA || k == 0xB || k >= 0xD)
        goto tail;
    if (g->state.current == 0x72 || g->state.current == 0x65)
        goto tail;
    camera = &l00_camera;
    if (camera->unk180 != 0 && *(s16 *)((u8 *)camera->unk180 + 0x86) == 5)
        goto tail;
    if (g->grounded_frames == 0)
        goto tail;
    if (((u8 *)g->ground_moby) != 0 && (*(s16 *)(((u8 *)g->ground_moby) + 0xA6) == 0x13E ||
                                        *(s16 *)(((u8 *)g->ground_moby) + 0xA6) == 0x46F))
        goto tail;
    if (FUN_L00_00233db8_b3058(-1, 1) != 0) {
        b = 0.0f;
        launch_yaw = g->held_pose_yaw;
        if (g->state.current == 1 && (*(u16 *)((u8 *)g->moby + 0x34) & 1)) {
            launch_yaw = fast_arc_tan(camera->unk350[0], camera->unk350[1]);
            b = -fast_arc_tan(vector_length_xy(camera->unk350), camera->unk350[2]);
            fast_vec_normalize(launch_position, camera->unk350, 0.3f);
            fast_vec_normalize(u, &camera->unk350[8], -0.35f);
            fast_vec_add(launch_position, launch_position, u);
            fast_vec_add(launch_position, launch_position, &camera->pos);
        } else if (hero.unk22C8 == 1) {
            b = fast_arc_tan(vector_length_xy(visibomb_moby->vD0), visibomb_moby->fD8);
            build_spherical_offset(launch_position, 0.6f, launch_yaw, -b);
            fast_vec_add(launch_position, launch_position, visibomb_moby->v10);
        } else {
            build_spherical_offset(launch_position, 0.6f, launch_yaw, -b);
            fast_vec_add(launch_position, launch_position, visibomb_moby->v10);
        }
        if (visibomb_moby->b53 != 3) {
            blend_moby_animation(visibomb_moby, 3, 0, 0);
        }
        jet_camera_mode();
        v->i4 = FUN_L00_002b58d8_b3058(visibomb_moby, launch_position, launch_yaw, b);
        r = (R_b3058 *)item_usage;
        if (r->u68 < 0xFFFF) {
            r->u68++;
        }
        if (scale_game_frames(D_0015EEA4) / 600 > r->u6A) {
            r->u6A = scale_game_frames(D_0015EEA4) / 600;
        }
        r->i6C = r->i6C | (1 << current_level_index) | 0x80000000;
        v->i0 = scale_game_frames(0x3C);
    } else if (controller->i1A4 & g->items[0].button_mask) {
        allocate_voice_for_target_entry(0, 0, visibomb_moby);
    }
tail:
    if ((visibomb_moby->b70 & 2) && visibomb_moby->b53 != 1) {
        blend_moby_animation(visibomb_moby, 1, 0, 0);
    }
    visibomb_moby->u34 |= 4;
}
