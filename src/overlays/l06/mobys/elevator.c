/* Level 06 moby class 0x425: elevator; carries the hero between two heights and switches the states of its linked mobys. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/ui/help/try_set_help_message.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F6790), where it is exact; names translated to the US level program. */
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

#include "qcopy.h"

/* Registers the moby's draw callbacks, choosing by its four slot ids. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FE4B8), where it is exact; names translated to the US level program. */
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002B5990), where it is exact; names translated to the US level program. */
#include "rnc/overlay/cuboid.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002DB0E0), where it is exact; names translated to the US level program. */
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");

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
    int   f68;
    int   f6C;
    int   f70;
    int   f74;
    int   f78;
    int   f7C;
    int   f80;
    int   f84;
    int   f88;
    float f8C;
    int   f90;
    int   f94;
    int   f98;
    float f9C;
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
    unsigned char f20A4;
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

int D_L06_00161F6C = 0x80202020;

int D_L06_00161F70 = 0x80808080;

int D_L06_00161F74 = 0x80208020;

int D_L06_00161F78 = -1;

extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void enter_hero_vehicle_mode_and_set_moby(int, int, int) __asm__("FUN_L01_00231348");
extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");
extern void restore_hero_from_vehicle_mode(void) __asm__("FUN_L06_00227cf8");
extern void fade_to_black(s32 n) __asm__("FUN_001f4a58");
void        set_moby_and_links_to_states_2_and_3(char *a) __asm__("FUN_L06_002f54a0");
extern unsigned char D_0013D5DD_2FDA70[] __asm__("D_0013D4C6");
extern unsigned char D_0013D4AF_2FDA70[] __asm__("D_0013D3A8");
extern unsigned char D_0013D5CA_2FDA70[] __asm__("D_0013D4C6");
extern char          D_0013A5E0_2FDA70[] __asm__("D_0013CAE4");
void                 update_elevator(Moby_2FDA70 *moby) __asm__("FUN_L06_002fc640");

void set_moby_and_links_to_states_2_and_3(char *moby) {
    char  *data = ((char *)MOBY(moby)->pvars);
    char **linked_mobys;
    int    i;
    moby[0xBC] = 1;
    if (MOBY(moby)->state == 6) {
        moby[0x20] = 2;
        moby[0x31] = 1;
        MOBY(moby)->flags &= 0xFFFE;
        *(int *)(data + 0x14) = 0;
        MOBY(moby)->rot.x =
            fast_subtract_rotations(*(float *)(data + 0x10), *(float *)(data + 0x14));
        update_entity(moby);
        linked_mobys = (char **)(data + 0x24);
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

void update_elevator(Moby_2FDA70 *moby) {
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
        if (D_L06_00161F78 == -1) {
            if (D_0013D5DD_2FDA70[0x10] != 0) {
                D_L06_00161F78 = 1;
            } else {
                D_L06_00161F78 = 0;
            }
        }
        moby_data->f94 = 0;
        moby_data->f88 = -1;
        if (D_0013D4AF_2FDA70[0] == 0) {
            moby->state = 1;
        } else {
            moby->state = 3;
        }
        moby->pos.z =
            l06_cuboids_macro[moby_data->f78].pos.z + l06_cuboids_macro[moby_data->f78].axes[2].z;
        if (((Hero_2FDA70 *)(D_0013F350))->f20A4 == 0) {
            ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 |= 1;
            ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 |= 2;
            ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f94 = 0;
        } else if (((Hero_2FDA70 *)(D_0013F350))->f20A4 == 1) {
            set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f60 << 8));
        }
        set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f64 << 8));
        set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f68 << 8));
        break;
    case WAIT_FOR_INITIAL_HERO_ACTIVATION:
        if (is_point_inside_clip_volume(&((Hero_2FDA70 *)(D_0013F350))->pos, moby_data->f70) != 0) {
            if (((Flags *)(alternate_item_available))->f20 == 0) {
                ((Flags *)(alternate_item_available))->f20 = 1;
            }
            moby->state = 3;
            moby_data->f90 = 0;
        } else if (((Hero_2FDA70 *)(D_0013F350))->moby->fA6 == 0x57) {
            moby->state = 3;
            moby_data->f84 = 1;
        }
        break;
    case WAIT_FOR_SCENE_GAME_MODE_CLEAR:
        if (D_L06_0015F5C4_macro == 0) {
            moby->state = 3;
        }
        break;
    case WAIT_FOR_HERO_TRIGGER_AT_NODE_OFFSET_HEIGHT:
    case WAIT_FOR_HERO_TRIGGER_AT_NODE_BASE_HEIGHT:
        if (fast_dec_timer(&moby_data->f90) != 0 &&
            is_point_inside_clip_volume(&((Hero_2FDA70 *)(D_0013F350))->pos, moby_data->f70) != 0) {
            int ok;
            if (((Hero_2FDA70 *)(D_0013F350))->f20A4 == 1) {
                ok = try_set_help_message(9, 0x1781) != 0;
            } else if (D_0013D5CA_2FDA70[0] == 0) {
                ok = try_set_help_message(9, 0x177E) != 0;
            } else if (moby_data->f84 != 0) {
                ok = try_set_help_message(9, 0x1785) != 0;
            } else {
                ok = try_set_help_message(9, 0x1789) != 0;
            }
            if ((*(int *)D_0013A5E0_2FDA70 & 0x10) && ((Hero_2FDA70 *)(D_0013F350))->f2084 != 100 &&
                ok) {
                ((Hero_2FDA70 *)(D_0013F350))->f1F2 = scale_game_frames(0x3C);
                moby_data->f90 = scale_game_frames(0x78);
                if (((Hero_2FDA70 *)(D_0013F350))->f20A4 == 1) {
                    fade_to_black(scale_game_frames(0x10));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->f64 << 8));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->f68 << 8));
                    set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro +
                                                         (moby_data->f60 << 8));
                    moby_data->f84 = 0;
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f94 = 0;
                    restore_hero_from_vehicle_mode();
                    {
                        LevelCuboid *b = &l06_cuboids_macro[moby_data->f7C];
                        set_race_camera(&b->pos, &b->rot, 0, 1);
                    }
                    request_music_track(0, 10);
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 |= 1;
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 |= 2;
                    if (D_0013D5DD_2FDA70[0x10] != 0 && D_L06_00161F78 == 0) {
                        D_L06_00161F78 = 1;
                        start_scene(3);
                        moby->state = 2;
                        break;
                    }
                } else if (D_0013D5CA_2FDA70[0] == 0) {
                    fade_to_black(scale_game_frames(0x10));
                    set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro +
                                                         (moby_data->f64 << 8));
                    set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro +
                                                         (moby_data->f68 << 8));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->f60 << 8));
                    moby_data->f84 = 1;
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f94 =
                        ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].cls->f10;
                    ((Moby_2FDA70 *)D_L06_0015FFD8_macro)[moby_data->f6C].f34 &= 0xFFFE;
                    {
                        LevelCuboid *b = &l06_cuboids_macro[moby_data->f80];
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
                                                         (moby_data->f68 << 8));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->f60 << 8));
                    moby_data->f84 = 1;
                    moby_data->f94 = 1;
                    {
                        LevelCuboid *b = &l06_cuboids_macro[moby_data->f80];
                        set_race_camera(&b->pos, &b->rot, 0, 1);
                    }
                } else {
                    fade_to_black(scale_game_frames(0x10));
                    set_moby_and_links_to_states_2_and_3(D_L06_0015FFD8_macro +
                                                         (moby_data->f64 << 8));
                    set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro +
                                                         (moby_data->f68 << 8));
                    set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro +
                                                         (moby_data->f60 << 8));
                    moby_data->f84 = 0;
                    moby_data->f94 = 0;
                    {
                        LevelCuboid *b = &l06_cuboids_macro[moby_data->f7C];
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
            if (moby->state == 3) {
                moby->state = 5;
                set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f64 << 8));
                moby_data->f90 = scale_game_frames(0x78);
            } else {
                moby->state = 6;
                set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f68 << 8));
                moby_data->f90 = scale_game_frames(0x78);
            }
        } else if (vector_distance_xy(&moby->pos, &((Hero_2FDA70 *)(D_0013F350))->pos) < 24.0f &&
                   moby->state == 3 && ((Hero_2FDA70 *)(D_0013F350))->pos.z + 5.0f < moby->pos.z) {
            moby_data->f84 = 1;
            moby_data->f94 = 1;
            moby->state = 5;
            set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f64 << 8));
            set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f60 << 8));
            moby_data->f90 = scale_game_frames(0x78);
        } else if (vector_distance_xy(&moby->pos, &((Hero_2FDA70 *)(D_0013F350))->pos) < 24.0f &&
                   moby->state == 4 && moby->pos.z < ((Hero_2FDA70 *)(D_0013F350))->pos.z - 5.0f) {
            moby->state = 6;
            set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_macro + (moby_data->f68 << 8));
            moby_data->f90 = scale_game_frames(0x78);
        }
        break;
    case MOVE_TO_NODE_BASE_HEIGHT:
    case MOVE_TO_NODE_OFFSET_HEIGHT: {
        float target;
        float height_acceleration_step;
        target = l06_cuboids_macro[moby_data->f78].pos.z;
        if (moby->state == 6) {
            target += l06_cuboids_macro[moby_data->f78].axes[2].z;
        }
        height_acceleration_step = height_cycle_acceleration_gp * frame_time_sq_macro;
        advance_accelerated_scalar(&moby->pos.z,
                                   &moby_data->f8C,
                                   target,
                                   height_acceleration_step,
                                   height_acceleration_step,
                                   height_cycle_max_speed_gp * D_0015ED6C);
        if (absolute_float(moby->pos.z - target) < 0.01f) {
            if (moby->state == 6) {
                moby->state = 7;
                set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro + (moby_data->f64 << 8));
            } else {
                set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_macro + (moby_data->f68 << 8));
                moby->state = 8;
            }
        }
        break;
    }
    case WAIT_FOR_HERO_CLEARANCE_AT_NODE_OFFSET_HEIGHT:
    case WAIT_FOR_HERO_CLEARANCE_AT_NODE_BASE_HEIGHT:
        if (vector_distance_xy(&((Hero_2FDA70 *)(D_0013F350))->pos, &moby->pos) >= 1.0f) {
            if (moby->state == 7) {
                moby->state = 3;
            } else {
                moby->state = 4;
            }
        }
        break;
    }
    if (moby_data->f94 != 0) {
        ((Hero_2FDA70 *)(D_0013F350))->f22CB = 1;
    } else {
        ((Hero_2FDA70 *)(D_0013F350))->f22CB = 0;
    }
    if (moby->state >= 5 && moby->state <= 8) {
        moby_data->f9C = 0.0f;
        moby->f90 = D_L06_00161F74;
    } else {
        float color_blend;
        moby_data->f9C = fast_add_rotations(moby_data->f9C, D_0015ED6C * 5.2359877f);
        color_blend = fast_sin(moby_data->f9C) * 4.0f - 3.0f;
        if (color_blend > 1.0f) {
            color_blend = 1.0f;
        } else if (color_blend < 0.0f) {
            color_blend = 0.0f;
        }
        moby->f90 = fast_tween_color(D_L06_00161F6C, D_L06_00161F70, color_blend);
    }
    fast_vec_add(&position_delta, &position_delta, &moby->pos);
    update_entity_matrix(moby_data->f20, &position_delta, &initial_rotation, &moby->rot);
}
