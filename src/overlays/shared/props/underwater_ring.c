/* Shared moby classes: underwater_ring, codebot, giant_clank_pad. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/commands/set_moby_color.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"

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
extern u8   D_0013F3D0[];
extern u32  D_0014C190[];
extern s32  D_L15_001BAAD0[];

void underwater_ring_update(struct Moby *moby) __asm__("FUN_L15_002e5958");

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

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D7C00.c: func_L15_002EC490), where it is exact; names translated to the US level program. */

extern void draw_moby_radial_ripple_mesh_buffers(void) __asm__("FUN_L15_002eac90");

void ripple_strips_moby_update(struct Moby *moby) __asm__("FUN_L15_002eb0a0");

void ripple_strips_moby_update(struct Moby *moby) {
    if (moby->state == 0) {
        set_moby_color(moby, 0x40, 0x40, 0x40);
        moby->state = 1;
    } else {
        add_draw_callback((void *)draw_moby_radial_ripple_mesh_buffers, moby);
        moby->draw_dist = 0;
        moby->flags |= 1;
    }
}

#include "sda.h"

/* Update for moby class 1428: a four-state trigger that spins and then deletes itself. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D7C00.c: func_L15_002ED168), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            f90;
} Obj;

extern int           D_L15_0015F5C4;
extern unsigned char D_0013D388_bd78[] __asm__("D_0013D388");
extern void          start_scene(int) __asm__("FUN_L00_00298840");
extern void          initialize_collectible_sprite_state(char *) __asm__("FUN_L02_002ea7d0");
extern void          update_collectible_sprite_effect(char *) __asm__("FUN_L02_002ea8b0");

extern float D_L15_001621A0 __attribute__((sda));
void         codebot_update(struct Moby *moby) __asm__("FUN_L15_002ebd78");

void codebot_update(struct Moby *moby) {
    enum {
        CODEBOT_INITIALIZE = 0,
        CODEBOT_CHECK_HERO_PROXIMITY = 1,
        CODEBOT_RECORD_COLLECTION = 2,
        CODEBOT_REMOVE = 3
    };
    unsigned char *collection_flags;
    if (current_level_index != 0xF) {
        moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 1.5707964f);
        moby->scale = moby->pclass->scale * D_L15_001621A0;
        switch (moby->state) {
        case CODEBOT_INITIALIZE:
            initialize_collectible_sprite_state(moby);
            if (D_0013D388_bd78[2] != 0) {
                mark_moby_for_removal(moby);
            } else {
                moby->state = 1;
                moby->pos.z += 1.0f;
            }
            break;
        case CODEBOT_CHECK_HERO_PROXIMITY:
            update_collectible_sprite_effect(moby);
            if (fast_vec_dist(&moby->pos, &hero.motion.pos) < 3.0f) {
                if (hero.state.current != 0x32) {
                    moby->flags |= 0x41;
                    start_scene(1);
                    moby->state = 2;
                }
            }
            break;
        case CODEBOT_RECORD_COLLECTION:
            if (D_L15_0015F5C4 != 2) {
                queue_dialogue_message(0x426A, -1);
                D_L15_0015F640 = scale_game_frames(0xB4);
                collection_flags = D_0013D388_bd78;
                collection_flags[2] = 1;
                collection_flags[0x78] = 1;
                moby->state = 3;
                memcard_save_data(0, -1);
            }
            break;
        case CODEBOT_REMOVE:
            mark_moby_for_removal(moby);
            break;
        }
    }
}

extern struct Moby   *l15_moby_array __asm__("D_L15_0015FFD8");
extern int            D_L15_0015F594 __attribute__((sda));
extern u8             D_0014C050[];
extern u32 D_0013CAE4 NOT_SDA;
extern void           try_set_help_message(int, int) __asm__("FUN_00215130");
extern void           jet_game_state(int, int) __asm__("FUN_L01_0027a248");
extern void           set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void           enter_hero_vehicle_mode(int, int, struct Moby *) __asm__("FUN_L00_00210a08");

void giant_clank_pad_update(struct Moby *moby) __asm__("FUN_L15_002ed068");

void giant_clank_pad_update(struct Moby *moby) {
    enum {
        GIANT_CLANK_PAD_INITIALIZE = 0,
        GIANT_CLANK_PAD_ARM_ACTIVATION = 1,
        GIANT_CLANK_PAD_CHECK_ACTIVATION = 2
    };
    struct Moby *linked = moby->pvars;
    struct Moby *entry;
    struct Moby *pad;
    u8           state = moby->state;
    u8           position[16] __attribute__((aligned(16)));

    switch (state) {
    case GIANT_CLANK_PAD_INITIALIZE: {
        s32 level;
        if (*(int *)((u8 *)linked + 0x60) == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        entry = (struct Moby *)((char *)l15_moby_array + (*(int *)((u8 *)linked + 0x60) << 8));
        entry->flags |= 1;
        entry->collision = 0;
        entry->visible = 0;
        level = current_level_index;
        moby->state = (level != 0x12 || D_0014C050[0x120 + moby->mission] != 0xff) ? 1 : 3;
        break;
    }
    case GIANT_CLANK_PAD_ARM_ACTIVATION:
        /* Fakematch: the second store gives retail's register for moby. */
        if (moby)
            moby->state = 2;
        else
            moby->state = 2;
        return;
    case GIANT_CLANK_PAD_CHECK_ACTIVATION:
        if (hero.ground_moby == moby && hero.air_frames.s == 0 && D_L15_0015F5C4 != 2 &&
            hero.state.current != 0x1d) {
            if (hero.vehicle_mode == 0) {
                try_set_help_message(4, 0x3aa4);
            } else if (hero.vehicle_mode == 2 && current_level_index != 0x12) {
                try_set_help_message(4, 0x3aa5);
            }
            if (!(D_0013CAE4 & 0x10))
                return;
            if (D_L15_0015F594 != 4)
                return;
            if (hero.vehicle_mode == 2 && current_level_index == 0xf) {
                pad = l15_moby_array;
                pad += *(int *)((u8 *)linked + 0x60);
                pad->flags |= 1;
                pad->collision = 0;
                pad->visible = 0;
                restore_saved_hero_moby_and_clear_vehicle_mode();
                set_moby_animation(pad, 0, 0);
                if (current_level_index != 0xf)
                    return;
                start_scene(9);
                jet_game_state(0, 5);
                return;
            }
            if (hero.vehicle_mode != 0)
                return;
            pad = l15_moby_array;
            pad += *(int *)((u8 *)linked + 0x60);
            *(u128 *)position = *(u128 *)&moby->pos;
            *(float *)(position + 8) = probe_ground_height(position, 0, 0.5f) + 0.1f;
            set_race_camera(position, &moby->rot, 0, 1);
            pad->collision = pad->pclass->collision;
            pad->visible = 1;
            pad->flags &= ~1;
            enter_hero_vehicle_mode(2, 0x5a, pad);
            if (current_level_index == 0xf) {
                start_scene(8);
                *(u128 *)hero.saved_pos_rot = *(u128 *)&hero.motion.pos;
                *(u128 *)(hero.saved_pos_rot + 16) = *(u128 *)&hero.motion.rot;
                jet_game_state(2, 4);
                return;
            }
            if (current_level_index != 0x12)
                return;
            start_scene(6);
            *(u128 *)hero.saved_pos_rot = *(u128 *)&hero.motion.pos;
            *(u128 *)(hero.saved_pos_rot + 16) = *(u128 *)&hero.motion.rot;
            moby->state = 3;
            return;
        }
        break;
    default:
        return;
    }
    *(int *)((u8 *)linked + 0x68) = -1;
}

float D_L17_001623A0 = 1.0f;
