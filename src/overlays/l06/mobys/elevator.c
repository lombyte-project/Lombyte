/* Level 06 moby class 0x425: elevator; carries the hero between two heights and switches the states of its linked mobys. */
#include "rnc/input/pad_globals.h"
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/ui/help/try_set_help_message.h"
#include "rnc/math/interpolation/advance_accelerated_scalar.h"
#include "rnc/overlay/shared/hero/hero_frame_update.h"
#include "rnc/overlay/shared/hero/update_hero_vehicle_attachments_and_effects.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/effects/fade_to_black.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F6790), where it is exact; names translated to the US level program. */
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

#include "qcopy.h"

/* Registers the moby's draw callbacks, choosing by its four slot ids. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FE4B8), where it is exact; names translated to the US level program. */

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002B5990), where it is exact; names translated to the US level program. */
#include "rnc/overlay/shared/world/cuboid.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002DB0E0), where it is exact; names translated to the US level program. */

/* Puts a moby in state 2 and its seven linked mobys in state 3, resetting each one's heading. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F68D0), where it is exact; names translated to the US level program. */
void                set_moby_and_links_to_states_2_and_3(char *moby) __asm__("FUN_L06_002f54a0");
extern char         D_0013F350[];
extern void         set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern unsigned int fast_tween_color(unsigned int, unsigned int, float) __asm__("FUN_001fa6e0");

#include "sda.h"
#include "rnc/overlay/l06/mobys/alien_queen.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FDA70), where it is exact; names translated to the US level program. */
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2FDA70;

typedef struct {
    char  pad0[0x10];
    int   f10;
    char  pad14[0x10];
    float f24;
} Cls;

typedef struct {
    char  pad0[0x20];
    char  f20[0x40];
    int   f60;
    int   f64;
    int   base_height_linked_moby_index;
    int   f6C;
    int   f70;
    int   f74;
    int   height_cuboid_index;
    int   vehicle_exit_placement_cuboid_index;
    int   vehicle_entry_placement_cuboid_index;
    int   f84;
    int   f88;
    float height_velocity;
    int   f90;
    int   f94;
    int   f98;
    float color_pulse_phase;
} Data_2FDA70;

typedef struct Moby_2FDA70 {
    char           pad0[0x10];
    Vec_2FDA70     pos;
    unsigned char  state;
    char           pad21[3];
    Cls           *cls;
    char           pad28[4];
    float          f2C;
    char           pad30[4];
    unsigned short f34;
    char           pad36[0xA];
    Vec_2FDA70     rot;
    char           pad50[0x28];
    Data_2FDA70   *data;
    char           pad7C[0x14];
    int            f90;
    int            f94;
    char           pad98[0xE];
    short          fA6;
    char           padA8[0x58];
} Moby_2FDA70;

typedef struct {
    char          pad0[0x80];
    Vec_2FDA70    pos;
    char          pad90[0x162];
    short         f1F2;
    char          pad1F4[0x108];
    Moby_2FDA70  *f2FC;
    char          pad300[0xE];
    short         f30E;
    char          pad310[0x1D70];
    Moby_2FDA70  *moby;
    int           f2084;
    char          pad2088[0x1C];
    unsigned char vehicle_mode;
    char          pad20A5[0x226];
    unsigned char f22CB;
} Hero_2FDA70;

typedef struct {
    char          pad0[0x20];
    unsigned char f20;
} Flags;

extern LevelCuboid     *l06_cuboids_macro __asm__("D_L06_001600EC") MACRO_ADDR;
extern char            *D_L06_0015FFD8_macro __asm__("D_L06_0015FFD8") MACRO_ADDR;
extern float D_0015ED6C MACRO_ADDR;
extern float            frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

/* no foreign declaration */
extern int D_L06_0015F5C4_macro __asm__("D_L06_0015F5C4") MACRO_ADDR;

/* no foreign declaration */
extern float D_L06_00161F60 __attribute__((sda));
extern float height_cycle_max_speed_gp __asm__("D_L06_00161F64") __attribute__((sda));
extern float height_cycle_acceleration_gp __asm__("D_L06_00161F68") __attribute__((sda));

int l06_elevator_pulse_dim_color __asm__("D_L06_00161F6C") = 0x80202020;

int l06_elevator_pulse_bright_color __asm__("D_L06_00161F70") = 0x80808080;

int l06_elevator_move_and_clearance_color __asm__("D_L06_00161F74") = 0x80208020;

int l06_elevator_scene_3_latch __asm__("D_L06_00161F78") = -1;

extern void          start_scene(int) __asm__("FUN_L00_00298840");
extern void          request_music_track(int, int) __asm__("FUN_L01_0027a248");
void                 set_moby_and_links_to_states_2_and_3(char *a) __asm__("FUN_L06_002f54a0");
extern unsigned char D_0013D5DD_2FDA70[] __asm__("D_0013D4C6");
extern unsigned char D_0013D4AF_2FDA70[] __asm__("D_0013D3A8");
extern unsigned char D_0013D5CA_2FDA70[] __asm__("D_0013D4C6");
void                 elevator_update(Moby_2FDA70 *moby) __asm__("FUN_L06_002fc640");

void set_moby_and_links_to_states_2_and_3(char *moby) {
    char  *moby_data = ((char *)MOBY(moby)->pvars);
    char **linked_mobys;
    int    i;
    MOBY(moby)->command = 1;
    if (MOBY(moby)->state == 6) {
        MOBY(moby)->state = 2;
        MOBY(moby)->visible = 1;
        MOBY(moby)->flags &= 0xFFFE;
        *(int *)(moby_data + 0x14) = 0;
        MOBY(moby)->rot.x =
            fast_subtract_rotations(*(float *)(moby_data + 0x10), *(float *)(moby_data + 0x14));
        update_entity(moby);
        linked_mobys = (char **)(moby_data + 0x24);
        for (i = 0; i < 7; i++) {
            char *linked_moby = linked_mobys[i];
            char *linked_data;
            linked_moby[0x20] = 3;
            linked_data = ((char *)MOBY(linked_moby)->pvars);
            *(unsigned short *)(linked_mobys[i] + 0x34) &= 0xFFFE;
            linked_mobys[i][0x31] = 1;
            *(int *)(linked_data + 0x14) = 0;
            *(float *)(linked_mobys[i] + 0x40) = fast_subtract_rotations(
                *(float *)(linked_data + 0x10), *(float *)(linked_data + 0x14));
            update_entity(linked_mobys[i]);
        }
    }
}

/* Moby class 1061 */
void elevator_update(Moby_2FDA70 *moby) {
    enum {
        INITIALIZE_POSITION_AND_LINKED_MOBYS = 0,
        WAIT_FOR_INITIAL_HERO_ACTIVATION = 1,
        WAIT_FOR_SCENE_GAME_MODE_CLEAR = 2,
        WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT = 3,
        WAIT_FOR_HERO_TRIGGER_AT_NODE_BASE_HEIGHT = 4,
        MOVE_TO_NODE_BASE_HEIGHT = 5,
        MOVE_TO_NODE_OFFSET_HEIGHT = 6,
        WAIT_FOR_HERO_CLEARANCE_AT_NODE_OFFSET_HEIGHT = 7,
        WAIT_FOR_HERO_CLEARANCE_AT_NODE_BASE_HEIGHT = 8
    };
    Vec_2FDA70   position_delta;
    Vec_2FDA70   initial_rotation;
    Data_2FDA70 *moby_data = moby->data;

    fast_vec_scale(&position_delta, &moby->pos, -1.0f);
    qcopy(&initial_rotation, &moby->rot);
    moby->f2C = moby->cls->f24 * D_L06_00161F60;
    switch (moby->state) {
    case INITIALIZE_POSITION_AND_LINKED_MOBYS:
        if (l06_elevator_scene_3_latch == -1) {
            if (D_0013D5DD_2FDA70[0x10] != 0) {
                l06_elevator_scene_3_latch = 1;
            } else {
                l06_elevator_scene_3_latch = 0;
            }
        }
        moby_data->f94 = 0;
        moby_data->f88 = -1;
        if (D_0013D4AF_2FDA70[0] == 0) {
            moby->state = WAIT_FOR_INITIAL_HERO_ACTIVATION;
        } else {
            moby->state = WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT;
        }
        moby->pos.z =
            l06_cuboids_macro[moby_data->height_cuboid_index].pos.z + l06_cuboids_macro[moby_data->height_cuboid_index].axes[2].z;
        if (((Hero_2FDA70 *)(D_0013F350))->vehicle_mode == 0) {
            ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 |= 1;
            ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 |= 2;
            ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f94 = 0;
        } else if (((Hero_2FDA70 *)(D_0013F350))->vehicle_mode == 1) {
            set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f60 << 8));
        }
        set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f64 << 8));
        set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->base_height_linked_moby_index << 8));
        break;
    case WAIT_FOR_INITIAL_HERO_ACTIVATION:
        if (is_point_inside_clip_volume(&((Hero_2FDA70 *)(D_0013F350))->pos, moby_data->f70) != 0) {
            if (((Flags *)(alternate_item_available))->f20 == 0) {
                ((Flags *)(alternate_item_available))->f20 = 1;
            }
            moby->state = WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT;
            moby_data->f90 = 0;
        } else if (((Hero_2FDA70 *)(D_0013F350))->moby->fA6 == 0x57) {
            moby->state = WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT;
            moby_data->f84 = 1;
        }
        break;
    case WAIT_FOR_SCENE_GAME_MODE_CLEAR:
        if (D_L06_0015F5C4_macro == 0) {
            moby->state = WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT;
        }
        break;
    case WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT:
    case WAIT_FOR_HERO_TRIGGER_AT_NODE_BASE_HEIGHT:
        if (fast_dec_timer(&moby_data->f90) != 0 &&
            is_point_inside_clip_volume(&((Hero_2FDA70 *)(D_0013F350))->pos, moby_data->f70) != 0) {
            int ok;
            if (((Hero_2FDA70 *)(D_0013F350))->vehicle_mode == 1) {
                ok = try_set_help_message(9, 0x1781) != 0;
            } else if (D_0013D5CA_2FDA70[0] == 0) {
                ok = try_set_help_message(9, 0x177E) != 0;
            } else if (moby_data->f84 != 0) {
                ok = try_set_help_message(9, 0x1785) != 0;
            } else {
                ok = try_set_help_message(9, 0x1789) != 0;
            }
            if ((pad_pressed_words[0] & 0x10) && ((Hero_2FDA70 *)(D_0013F350))->f2084 != 100 &&
                ok) {
                ((Hero_2FDA70 *)(D_0013F350))->f1F2 = scale_game_frames(0x3C);
                moby_data->f90 = scale_game_frames(0x78);
                if (((Hero_2FDA70 *)(D_0013F350))->vehicle_mode == 1) {
                    fade_to_black(scale_game_frames(0x10));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->f64 << 8));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->base_height_linked_moby_index << 8));
                    set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro +
                                                         (moby_data->f60 << 8));
                    moby_data->f84 = 0;
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f94 = 0;
                    restore_hero_from_vehicle_mode();
                    {
                        LevelCuboid *b = &l06_cuboids_macro[moby_data->vehicle_exit_placement_cuboid_index];
                        set_race_camera(&b->pos, &b->rot, 0, 1);
                    }
                    request_music_track(0, 10);
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 |= 1;
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 |= 2;
                    if (D_0013D5DD_2FDA70[0x10] != 0 && l06_elevator_scene_3_latch == 0) {
                        l06_elevator_scene_3_latch = 1;
                        start_scene(3);
                        moby->state = WAIT_FOR_SCENE_GAME_MODE_CLEAR;
                        break;
                    }
                } else if (D_0013D5CA_2FDA70[0] == 0) {
                    fade_to_black(scale_game_frames(0x10));
                    set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro +
                                                         (moby_data->f64 << 8));
                    set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro +
                                                         (moby_data->base_height_linked_moby_index << 8));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->f60 << 8));
                    moby_data->f84 = 1;
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f94 =
                        ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].cls->f10;
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 &= 0xFFFE;
                    {
                        LevelCuboid *b = &l06_cuboids_macro[moby_data->vehicle_entry_placement_cuboid_index];
                        set_race_camera(&b->pos, &b->rot, 0, 1);
                    }
                    enter_hero_vehicle_mode_and_set_moby(
                        1, 0x43, (int)(D_L06_0015FFD8_macro + (moby_data->f6C << 8)));
                    request_music_track(2, 7);
                } else if (moby_data->f84 == 0) {
                    fade_to_black(scale_game_frames(0x10));
                    set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro +
                                                         (moby_data->f64 << 8));
                    set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro +
                                                         (moby_data->base_height_linked_moby_index << 8));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->f60 << 8));
                    moby_data->f84 = 1;
                    moby_data->f94 = 1;
                    {
                        LevelCuboid *b = &l06_cuboids_macro[moby_data->vehicle_entry_placement_cuboid_index];
                        set_race_camera(&b->pos, &b->rot, 0, 1);
                    }
                } else {
                    fade_to_black(scale_game_frames(0x10));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->f64 << 8));
                    set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro +
                                                         (moby_data->base_height_linked_moby_index << 8));
                    set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro +
                                                         (moby_data->f60 << 8));
                    moby_data->f84 = 0;
                    moby_data->f94 = 0;
                    {
                        LevelCuboid *b = &l06_cuboids_macro[moby_data->vehicle_exit_placement_cuboid_index];
                        set_race_camera(&b->pos, &b->rot, 0, 1);
                    }
                }
            }
        }
        if (((Hero_2FDA70 *)(D_0013F350))->f2FC == moby &&
            ((Hero_2FDA70 *)(D_0013F350))->f30E == 0 && fast_dec_timer(&moby_data->f90) != 0 &&
            moby->pos.z + 0.29f < ((Hero_2FDA70 *)(D_0013F350))->pos.z &&
            vector_distance_xy(&((Hero_2FDA70 *)(D_0013F350))->pos, &moby->pos) < 1.0f) {
            allocate_voice_for_target_entry(0, 0, (int)moby);
            if (moby->state == WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT) {
                moby->state = MOVE_TO_NODE_BASE_HEIGHT;
                set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f64 << 8));
                moby_data->f90 = scale_game_frames(0x78);
            } else {
                moby->state = MOVE_TO_NODE_OFFSET_HEIGHT;
                set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->base_height_linked_moby_index << 8));
                moby_data->f90 = scale_game_frames(0x78);
            }
        } else if (vector_distance_xy(&moby->pos, &((Hero_2FDA70 *)(D_0013F350))->pos) < 24.0f &&
                   moby->state == WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT &&
                   ((Hero_2FDA70 *)(D_0013F350))->pos.z + 5.0f < moby->pos.z) {
            moby_data->f84 = 1;
            moby_data->f94 = 1;
            moby->state = MOVE_TO_NODE_BASE_HEIGHT;
            set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f64 << 8));
            set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f60 << 8));
            moby_data->f90 = scale_game_frames(0x78);
        } else if (vector_distance_xy(&moby->pos, &((Hero_2FDA70 *)(D_0013F350))->pos) < 24.0f &&
                   moby->state == WAIT_FOR_HERO_TRIGGER_AT_NODE_BASE_HEIGHT &&
                   moby->pos.z < ((Hero_2FDA70 *)(D_0013F350))->pos.z - 5.0f) {
            moby->state = MOVE_TO_NODE_OFFSET_HEIGHT;
            set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->base_height_linked_moby_index << 8));
            moby_data->f90 = scale_game_frames(0x78);
        }
        break;
    case MOVE_TO_NODE_BASE_HEIGHT:
    case MOVE_TO_NODE_OFFSET_HEIGHT: {
        float target;
        float height_acceleration_step;
        target = l06_cuboids_macro[moby_data->height_cuboid_index].pos.z;
        if (moby->state == MOVE_TO_NODE_OFFSET_HEIGHT) {
            target += l06_cuboids_macro[moby_data->height_cuboid_index].axes[2].z;
        }
        height_acceleration_step = height_cycle_acceleration_gp * frame_time_sq_macro;
        advance_accelerated_scalar(&moby->pos.z,
                                   &moby_data->height_velocity,
                                   target,
                                   height_acceleration_step,
                                   height_acceleration_step,
                                   height_cycle_max_speed_gp * D_0015ED6C);
        if (absolute_float(moby->pos.z - target) < 0.01f) {
            if (moby->state == MOVE_TO_NODE_OFFSET_HEIGHT) {
                moby->state = WAIT_FOR_HERO_CLEARANCE_AT_NODE_OFFSET_HEIGHT;
                set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro + (moby_data->f64 << 8));
            } else {
                set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro + (moby_data->base_height_linked_moby_index << 8));
                moby->state = WAIT_FOR_HERO_CLEARANCE_AT_NODE_BASE_HEIGHT;
            }
        }
        break;
    }
    case WAIT_FOR_HERO_CLEARANCE_AT_NODE_OFFSET_HEIGHT:
    case WAIT_FOR_HERO_CLEARANCE_AT_NODE_BASE_HEIGHT:
        if (vector_distance_xy(&((Hero_2FDA70 *)(D_0013F350))->pos, &moby->pos) >= 1.0f) {
            if (moby->state == WAIT_FOR_HERO_CLEARANCE_AT_NODE_OFFSET_HEIGHT) {
                moby->state = WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT;
            } else {
                moby->state = WAIT_FOR_HERO_TRIGGER_AT_NODE_BASE_HEIGHT;
            }
        }
        break;
    }
    if (moby_data->f94 != 0) {
        ((Hero_2FDA70 *)(D_0013F350))->f22CB = 1;
    } else {
        ((Hero_2FDA70 *)(D_0013F350))->f22CB = 0;
    }
    if (moby->state >= MOVE_TO_NODE_BASE_HEIGHT &&
        moby->state <= WAIT_FOR_HERO_CLEARANCE_AT_NODE_BASE_HEIGHT) {
        moby_data->color_pulse_phase = 0.0f;
        moby->f90 = l06_elevator_move_and_clearance_color;
    } else {
        float color_blend;
        moby_data->color_pulse_phase = fast_add_rotations(moby_data->color_pulse_phase, D_0015ED6C * 5.2359877f);
        color_blend = fast_sin(moby_data->color_pulse_phase) * 4.0f - 3.0f;
        if (color_blend > 1.0f) {
            color_blend = 1.0f;
        } else if (color_blend < 0.0f) {
            color_blend = 0.0f;
        }
        moby->f90 = fast_tween_color(l06_elevator_pulse_dim_color, l06_elevator_pulse_bright_color, color_blend);
    }
    fast_vec_add(&position_delta, &position_delta, &moby->pos);
    update_entity_matrix(moby_data->f20, &position_delta, &initial_rotation, &moby->rot);
}
