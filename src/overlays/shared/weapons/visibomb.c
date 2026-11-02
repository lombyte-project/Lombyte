/* Shared moby class visibomb: the visibomb gun held by the hero. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/input/pad_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/hero/hero_idle_sequence.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/weapons.h"

typedef struct {
    u8  pad0[0x1A4];
    s32 input_button_mask;
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
    u16 visibomb_use_count;
    u16 visibomb_max_use_time;
    s32 visibomb_usage_flags;
} R_b3058;

typedef struct {
    s32 launch_cooldown_frames;
    s32 active_missile_moby;
} V_b3058;

typedef struct {
    u8       pad0[0x10];
    f32      v10[4];
    u8       pad20[0x34 - 0x20];
    u16      moby_flags;
    u8       pad36[0x53 - 0x36];
    u8       animation_index;
    u8       pad54[0x70 - 0x54];
    u8       b70;
    u8       pad71[0x78 - 0x71];
    V_b3058 *v;
    u8       pad7C[0xD0 - 0x7C];
    f32      launch_direction_xy[2];
    f32      launch_direction_z;
} O_b3058;

extern int D_0015EEA4;
extern s32 D_0015EEA4;

void visibomb_update(O_b3058 *o) __asm__("FUN_L00_002b3058");

/* Moby class 163 */
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
    if (!(controller->input_button_mask & g->items[0].button_mask))
        goto tail;
    if (v->launch_cooldown_frames != 0)
        goto tail;
    if (v->active_missile_moby != 0)
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
    if (use_ammo(-1, 1) != 0) {
        b = 0.0f;
        launch_yaw = g->held_pose_yaw;
        if (g->state.current == 1 && (g->moby->flags & 1)) {
            launch_yaw = fast_arc_tan(camera->unk350[0], camera->unk350[1]);
            b = -fast_arc_tan(vector_length_xy(camera->unk350), camera->unk350[2]);
            fast_vec_normalize(launch_position, camera->unk350, 0.3f);
            fast_vec_normalize(u, &camera->unk350[8], -0.35f);
            fast_vec_add(launch_position, launch_position, u);
            fast_vec_add(launch_position, launch_position, &camera->pos);
        } else if (hero.unk22C8 == 1) {
            b = fast_arc_tan(vector_length_xy(visibomb_moby->launch_direction_xy), visibomb_moby->launch_direction_z);
            build_spherical_offset(launch_position, 0.6f, launch_yaw, -b);
            fast_vec_add(launch_position, launch_position, visibomb_moby->v10);
        } else {
            build_spherical_offset(launch_position, 0.6f, launch_yaw, -b);
            fast_vec_add(launch_position, launch_position, visibomb_moby->v10);
        }
        if (visibomb_moby->animation_index != 3) {
            blend_moby_animation(visibomb_moby, 3, 0, 0);
        }
        jet_camera_mode();
        v->active_missile_moby = spawn_visibomb_missile(visibomb_moby, launch_position, launch_yaw, b);
        r = (R_b3058 *)item_usage;
        if (r->visibomb_use_count < 0xFFFF) {
            r->visibomb_use_count++;
        }
        if (scale_game_frames(D_0015EEA4) / 600 > r->visibomb_max_use_time) {
            r->visibomb_max_use_time = scale_game_frames(D_0015EEA4) / 600;
        }
        r->visibomb_usage_flags = r->visibomb_usage_flags | (1 << current_level_index) | 0x80000000;
        v->launch_cooldown_frames = scale_game_frames(0x3C);
    } else if (controller->input_button_mask & g->items[0].button_mask) {
        allocate_voice_for_target_entry(0, 0, visibomb_moby);
    }
tail:
    if ((visibomb_moby->b70 & 2) && visibomb_moby->animation_index != 1) {
        blend_moby_animation(visibomb_moby, 1, 0, 0);
    }
    visibomb_moby->moby_flags |= 4;
}
