/* Shared moby class underwater_ring: a group of rings to swim through before the timer runs out. */

#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"

typedef struct {
    s16          timer;
    s16          delay;
    s16          triggered;
    s16          flash_timer;
    u8           flash_color;
    u8           initialized;
    u8           child_count;
    u8           notified_count;
    s32          frame_count;
    struct Moby *parent;
    f32          distance;
    s16          restart_timer;
    s16          pad26;
    s32          enabled;
} VendorVars;

extern void set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");

extern u8  D_0013F3D0[];
extern u32 D_0014C190[];
extern s32 D_L15_001BAAD0[];

void underwater_ring_update(struct Moby *moby) __asm__("FUN_L15_002e5958");

/* Moby class 1209 */
void underwater_ring_update(struct Moby *moby) {
    VendorVars  *vars = (VendorVars *)moby->pvars;
    u8           delta[16];
    u8           matrix[64];
    struct Moby *child;
    u16          save_id;
    u8          *save_byte;
    s32          first;
    s32          second;
    s32          divisor;
    s32          near_parent = 0;

    if ((u8)(moby->state - 1) < 2) {
        fast_vec_sub(delta, &moby->pos, D_0013F3D0);
        if (fast_vec_length(delta) < vars->distance) {
            transpose_rotation_matrix(matrix, &moby->rot_row0);
            transform_vector(delta, delta, matrix);
            if (*(f32 *)(delta + 4) < 0.2f)
                near_parent = 1;
        }
    }
    switch (moby->state) {
    case 0:
        if (vars->initialized == 0) {
            moby_point_position(moby, 0, delta);
            vars->distance = fast_vec_dist(&moby->pos, delta);
            vars->notified_count = 0;
            vars->parent = moby;
            get_first_moby_in_group(&child, moby->group, 0, 0);
            while (child != 0) {
                if (child != moby && child->oclass == 0x4b9) {
                    VendorVars *other = (VendorVars *)child->pvars;
                    other->parent = moby;
                    other->initialized = 1;
                    other->distance = vars->distance;
                }
                vars->notified_count++;
                get_next_moby_in_group(&child, child, 0, 0);
            }
        }
        vars->timer = 0;
        vars->delay = 0;
        vars->child_count = 0;
        vars->triggered = 0;
        vars->flash_timer = 0;
        set_moby_color(moby, 0, 0, 0);
        moby->state = 1;
        break;
    case 1:
    case 2:
        save_id = moby->save_id;
        save_byte = (u8 *)&l15_save_state;
        save_byte -= -(s16)save_id;
        if (save_byte[0x454] != 0 ||
            ((((s32) * (u32 *)(((((s16)save_id >> 5) << 2) + (current_level_index << 8)) +
                               (u8 *)D_0014C190)) >>
              (save_id & 31)) &
             1) != 0) {
            if (vars->enabled != 0) {
                set_moby_list_state(moby->group, 4);
                vars->timer = vars->restart_timer;
                moby->state = 4;
                break;
            }
        }
        if (vars->triggered == 0 && near_parent) {
            struct Moby *parent = vars->parent;
            VendorVars  *parent_vars = (VendorVars *)parent->pvars;
            if (parent->state == 1) {
                vars->child_count = 0;
                parent->state = 2;
                parent_vars->timer =
                    parent_vars->frame_count < 1 ? 900 : scale_game_frames(vars->frame_count);
            }
            parent_vars->child_count++;
            allocate_voice_for_target_entry(3, 0x31, moby);
            set_moby_color(moby, 0, 0xff, 0);
            vars->triggered = 1;
        }
        if (moby->state == 2) {
            if (vars->child_count >= vars->notified_count) {
                set_moby_list_state(moby->group, 4);
                allocate_voice_for_target_entry(1, 0x31, moby);
                vars->timer = vars->restart_timer;
                moby->state = 4;
                break;
            } else {
                if (fast_dec_timer_s16(&vars->timer) != 0) {
                    vars->child_count = 0;
                    vars->triggered = 0;
                    allocate_voice_for_target_entry(2, 0x31, moby);
                    set_moby_list_state(moby->group, 3);
                } else if (fast_dec_timer_s16(&vars->delay) != 0) {
                    allocate_voice_for_target_entry(0, 0x31, moby);
                    first = scale_game_frames(0x2d);
                    second = scale_game_frames(5);
                    divisor = scale_game_frames(0x28);
                    if (first < second + vars->timer / divisor)
                        vars->delay = scale_game_frames(0x2d);
                    else {
                        s32 delay = scale_game_frames(5);
                        s32 delay_divisor = scale_game_frames(0x28);
                        delay_divisor = delay + vars->timer / delay_divisor;
                        vars->delay = delay_divisor;
                    }
                }
            }
        }
        break;
    case 3:
        set_moby_color(moby, 0, 0, 0);
        vars->child_count = 0;
        vars->triggered = 0;
        moby->state = 1;
        break;
    case 4: {
        u16  save_id;
        u32 *bits;
        if (fast_dec_timer_s16(&vars->flash_timer) != 0) {
            vars->flash_timer = scale_game_frames(0x14);
            if (vars->flash_color != 0) {
                set_moby_color(moby, 0, 0xff, 0);
                vars->flash_color = 0;
            } else {
                set_moby_color(moby, 0xff, 0xff, 0xff);
                vars->flash_color = 1;
            }
        }
        save_id = moby->save_id;
        bits =
            (u32 *)(((((s16)save_id >> 5) << 2) + (current_level_index << 8)) + (u8 *)D_0014C190);
        *bits |= 1U << (save_id & 31);
        save_id = moby->save_id;
        D_L15_001BAAD0[(s16)save_id >> 5] |= 1U << (save_id & 31);
        break;
    }
    }
}
