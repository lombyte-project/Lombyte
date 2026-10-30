/* Level 05 moby class water: the rising water and its grids. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/shared/props/pool_set_scale.h"
#include "rnc/overlay/shared/props/pool_init_tiles.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/water_grid.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"

extern int          random_integer_below(int) __asm__("FUN_00213260");
extern float        random_float_between(float, float) __asm__("FUN_002132a8");
extern float        fast_cos(float);
extern float        fast_sin(float);
extern float        D_0015ED70;
typedef struct Moby Moby;
extern float        fast_vec_dist(void *, void *);
extern float        D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
typedef struct Hero Hero;

typedef struct {
    float raised_z;
    float lowered_z;
    int   starts_raised;
    int   rises;
    int   f10;
    int   record_index;
    int   timer;
    int   f1C;
    float speed;
    int   moving;
} WaterVars;

typedef struct {
    float         f0;
    float         f4;
    float         f8;
    float         fC;
    float         f10;
    float         f14;
    float         f18;
    float         f1C;
    float         f20;
    int           f24;
    int           f28;
    int           f2C;
    float         dir_x;
    float         dir_y;
    float         dir_z;
    unsigned char b3C;
    unsigned char b3D;
} Light;

typedef struct {
    char  pad0[0x10];
    float f10;
    float f14;
} Cam;

extern void *D_L05_001612D0_macro __asm__("D_L05_001612D0") MACRO_ADDR;
extern void  record_grid_entry_neighbor_indices(int) __asm__("FUN_L05_0030d6a0");
extern void  set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");

extern void synchronize_group_water_heights(char *) __asm__("FUN_L05_0030d5c0");
extern void pool_splash(void *, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern int  check_scaled_vector_bsphere(char *, float) __asm__("FUN_L01_00275690");
extern void render_level_water_grids(void) __asm__("FUN_L05_0030d678");

extern Cam                  l05_water_light_heading_source __asm__("D_L05_00180340");
extern Light                light __asm__("D_L05_001CAD00");
extern int                  water_grid_fixed_point_bounds[][4] __asm__("D_L05_0020B440");
extern unsigned short       record_draw_values[] __asm__("D_L05_0020B380");
extern int                  rise_frames[] __asm__("D_L05_0020B3E0");
extern float                rise_speeds[] __asm__("D_L05_0020B3F8");
extern int                  base_rise_frames[] __asm__("D_L05_0020B410");
extern float                base_rise_speeds[] __asm__("D_L05_0020B428");
extern void *D_L05_001612D4 MACRO_ADDR;
extern unsigned char        D_L05_00161204[] MACRO_ADDR;
extern unsigned char        D_L05_00161205[] MACRO_ADDR;
extern unsigned char        D_L05_00161206[] MACRO_ADDR;
extern int D_L05_00161208   MACRO_ADDR;
extern float D_L05_0016120C MACRO_ADDR;
extern float D_L05_00161210 MACRO_ADDR;
extern float D_L05_00161214 MACRO_ADDR;
int                         D_L05_00161D10 = -1;
extern unsigned short       l05_water_record_draw_value_override __asm__("D_L05_00161D14")
    __attribute__((sda));
int                       hero_hits __asm__("D_L05_00161D1C") = 0;
int                       rise_step __asm__("D_L05_00161D20") = 0;
extern float              rise_checkpoint __asm__("D_L05_00161D24") __attribute__((sda));
float                     water __asm__("D_L05_00161D28") = 0;
int                       rise_checkpoint_hit __asm__("D_L05_00161D2C") = 0;
extern float              move_speed __asm__("D_L05_00161D30") __attribute__((sda));
extern float              move_accel __asm__("D_L05_00161D34") __attribute__((sda));
extern s32 D_L05_001612D8 MACRO_ADDR;
/* Update of the level 05 water mobys: sets up the 48-record water chain, moves the surface by state and keeps its record in step. */
void water_update(struct Moby *moby) __asm__("FUN_L05_0030ca80");

void water_update(struct Moby *moby) {
    enum {
        WATER_INITIALIZE = 0,
        WATER_MOVE_TO_RAISED_HEIGHT = 1,
        WATER_MOVE_TO_LOWERED_HEIGHT = 2,
        WATER_WAIT_FOR_RISE_TRIGGER = 3,
        WATER_RISE_COMPLETE = 7,
        WATER_TIMED_RISE = 8
    };
    WaterVars *water_vars = *(WaterVars **)&moby->pvars;

    switch (moby->state) {
    case WATER_INITIALIZE:
        if (water_vars->record_index == 0) {
            int   i;
            int   j;
            float light_direction_angle;

            water = moby->pos.z;
            rise_step = 0;
            rise_checkpoint_hit = 0;
            pool_init_tiles(l05_water_grids, 0x30);
            pool_set_scale(1.0f);
            light.f0 = 16.0f;
            light.f4 = 16.0f;
            light.f8 = -8.0f;
            light.fC = -8.0f;
            light.f10 = 1.0f;
            light.f14 = 1.0f;
            light.f18 = 0.9f;
            light.f20 = 0.1f;
            light_direction_angle = fast_arc_tan(l05_water_light_heading_source.f10,
                                                 l05_water_light_heading_source.f14);
            light.dir_x = fast_cos(light_direction_angle) * 0.57735002f;
            light.dir_y = fast_sin(light_direction_angle) * 0.57735002f;
            light.dir_z = -0.57735002f;
            light.f28 = 0x29;
            light.f2C = 0x2A;
            light.b3C = 0x1C;
            light.b3D = 0x40;
            D_L05_001612D0_macro = l05_water_grids;
            D_L05_001612D4 = water_grid_fixed_point_bounds;
            D_L05_001612D8 = 0x30;
            D_L05_00161204[0] = 0;
            D_L05_00161205[0] = 0xB;
            D_L05_00161206[0] = 0x3E;
            D_L05_00161208 = 0;
            D_L05_0016120C = 20480.0f;
            D_L05_00161210 = 255.0f;
            D_L05_00161214 = 26.0f;
            for (i = 0; i < 0x30; i++) {
                pool_bind_tile(&l05_water_grids[i].x, water_grid_fixed_point_bounds[i]);
                l05_water_grids[water_vars->record_index].draw_value =
                    record_draw_values[water_vars->record_index];
            }
            for (j = 0; j < 0x30; j++) {
                record_grid_entry_neighbor_indices(j);
            }
        }
        moby->draw_dist = 0;
        moby->flags |= 1;
        if (water_vars->rises != 0) {
            if (water_vars->f10 >= 0) {
                moby->update_dist = 0xFF;
                if (hero_hits >= 2) {
                    int   k;
                    float s = (float)(hero_hits - 1) * 0.014285714f + 1.0f;
                    if (s > 1.4f) {
                        s = 1.4f;
                    }
                    for (k = 0; k < 6; k++) {
                        rise_frames[k] = truncate_float_to_s32((float)base_rise_frames[k] * s);
                        rise_speeds[k] = base_rise_speeds[k] / s;
                    }
                }
            }
            if (visited52[moby->mission + (current_level_index << 4)] == 0xFF) {
                moby->state = 7;
                moby->pos.z = 44.0f;
            } else {
                moby->state = 3;
                water_vars->timer = scale_game_frames(0x3C);
                if (moby->pos.z < water_vars->lowered_z) {
                    l05_water_grids[water_vars->record_index].draw_value = 0;
                }
            }
        } else {
            if (water_vars->f10 < 0) {
                float z = moby->pos.z;
                water_vars->lowered_z = z;
                water_vars->raised_z = z + 4.5f;
                if (water_vars->starts_raised != 0) {
                    moby->state = 1;
                    moby->pos.z = water_vars->raised_z;
                } else {
                    moby->state = 2;
                    moby->pos.z = water_vars->lowered_z;
                }
            } else {
                moby->state = 2;
                moby->pos.z = water_vars->lowered_z;
            }
        }
        if (water_vars->record_index == 0x2F) {
            *(int *)&moby->collision = 0;
        }
        break;
    case WATER_MOVE_TO_LOWERED_HEIGHT:
        if (moby->command & 4) {
            water_vars->moving = 1;
            set_moby_list_state(moby->group, 1);
        } else if (water_vars->f10 >= 0) {
            if (visited52[moby->mission + (current_level_index << 4)] == 0xFF) {
                moby->state = 1;
                moby->pos.z = water_vars->raised_z;
            }
        }
        if (water_vars->moving != 0) {
            advance_accelerated_scalar(&moby->pos.z,
                                       &water_vars->speed,
                                       water_vars->lowered_z,
                                       move_speed * D_0015ED70,
                                       move_speed * D_0015ED70,
                                       move_accel * D_0015ED6C_gp);
            synchronize_group_water_heights((char *)moby);
        }
        moby->command = 1;
        if (random_integer_below(0xC8) == 0) {
            float x =
                l05_water_grids[water_vars->record_index].x + random_float_between(-4.0f, 4.0f);
            float      r = random_float_between(-4.0f, 4.0f);
            WaterGrid *e = &l05_water_grids[water_vars->record_index];
            pool_splash(e, 1, 1, x, e->y + r, 1.0f, -0.05f);
        }
        break;
    case WATER_MOVE_TO_RAISED_HEIGHT:
        if (moby->command & 8) {
            water_vars->moving = 1;
            set_moby_list_state(moby->group, 2);
        } else {
            moby->command = 2;
        }
        if (water_vars->moving != 0) {
            advance_accelerated_scalar(&moby->pos.z,
                                       &water_vars->speed,
                                       water_vars->raised_z,
                                       move_speed * D_0015ED70,
                                       move_speed * D_0015ED70,
                                       move_accel * D_0015ED6C_gp);
            synchronize_group_water_heights((char *)moby);
        }
        if (random_integer_below(0xC8) == 0) {
            float x =
                l05_water_grids[water_vars->record_index].x + random_float_between(-4.0f, 4.0f);
            float      r = random_float_between(-4.0f, 4.0f);
            WaterGrid *e = &l05_water_grids[water_vars->record_index];
            pool_splash(e, 1, 1, x, e->y + r, 1.0f, -0.05f);
        }
        break;
    case WATER_WAIT_FOR_RISE_TRIGGER:
        if (vector_distance_xy(((float *)&moby->pos), hero.motion.pos.f) < 16.0f) {
            Hero *h = &hero;
            if (h->motion.pos.f[2] < moby->pos.z) {
                if (h->state.control_mode == 0x11) {
                    if (h->water_surface_z < moby->pos.z) {
                        h->water_surface_z = moby->pos.z;
                    }
                }
            }
        }
        if (water_vars->f10 >= 0) {
            if (vector_distance_xy(((float *)&moby->pos), hero.motion.pos.f) < 10.0f) {
                Hero *h = &hero;
                if (h->ground_moby != 0 && h->air_frames.s == 0 &&
                    h->ground_moby->oclass == 0x33E) {
                    water_vars->f1C = 0;
                    moby->state = 8;
                    rise_step = 0;
                    water_vars->timer = scale_game_frames(rise_frames[0]);
                }
            }
        }
        if (random_integer_below(0x32) == 0) {
            float x =
                l05_water_grids[water_vars->record_index].x + random_float_between(-4.0f, 4.0f);
            float      r = random_float_between(-4.0f, 4.0f);
            WaterGrid *e = &l05_water_grids[water_vars->record_index];
            pool_splash(e, 1, 1, x, e->y + r, 1.0f, -0.01f);
        }
        break;
    case WATER_RISE_COMPLETE:
        if (water_vars->f10 >= 0) {
            Hero *h = &hero;
            if (h->state.current == 0x6A && h->state_timer == 2) {
                hero_hits++;
            }
        } else if (water_vars->raised_z < moby->pos.z) {
            l05_water_grids[water_vars->record_index].draw_value = 0;
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case WATER_TIMED_RISE:
        if (fast_dec_timer(&water_vars->timer) != 0) {
            if (rise_step < 5) {
                rise_step++;
                water_vars->timer = scale_game_frames(rise_frames[rise_step]);
            }
        }
        moby->pos.z += rise_speeds[rise_step] * D_0015ED6C_gp;
        synchronize_group_water_heights((char *)moby);
        water = moby->pos.z;
        {
            Vec4 rise_checkpoint_position;

            rise_checkpoint_position.q = 0;
            rise_checkpoint_position.f[0] = 211.0f;
            rise_checkpoint_position.f[1] = 338.0f;
            rise_checkpoint_position.f[2] = 25.0f;
            if (fast_vec_dist(rise_checkpoint_position.f, &hero.motion.pos) < 6.0f) {
                if (rise_checkpoint_hit == 0) {
                    rise_checkpoint_hit = 1;
                    if (moby->pos.z > rise_checkpoint) {
                        rise_step = 5;
                    }
                }
            }
        }
        if (water_vars->f10 >= 0) {
            Hero *h = &hero;
            if (h->state.current == 0x6A && h->state_timer == 2) {
                hero_hits++;
            }
        }
        if (moby->pos.z >= 44.0f) {
            set_moby_list_state(moby->group, 7);
        }
        break;
    }
    if (water_vars->record_index >= 0) {
        WaterGrid *e = &l05_water_grids[water_vars->record_index];
        float      restore_record_draw_value;

        e->z = moby->pos.z;
        restore_record_draw_value = 0.0f;
        if (check_scaled_vector_bsphere((char *)moby, 48.0f) != -1 &&
            (!(moby->state >= 3 && moby->state <= 7) ||
             (moby->pos.z > water_vars->lowered_z && moby->pos.z < water_vars->raised_z))) {
            restore_record_draw_value = 1.0f;
        }
        if (restore_record_draw_value == 0.0f) {
            l05_water_grids[water_vars->record_index].draw_value = 0;
        } else {
            l05_water_grids[water_vars->record_index].draw_value =
                record_draw_values[water_vars->record_index];
        }
        if (water_vars->record_index == 0) {
            pool_flush((char *)l05_water_grids, 0x30);
            add_draw_callback(render_level_water_grids, moby);
        }
        if (water_vars->record_index == D_L05_00161D10) {
            l05_water_grids[water_vars->record_index].draw_value =
                l05_water_record_draw_value_override;
            record_draw_values[water_vars->record_index] = l05_water_record_draw_value_override;
        }
    }
}

extern int D_L05_0015FFD8;
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_0030EA88), where it is exact; names translated to the US level program. */

extern short *D_L05_001ABCC0[];

void synchronize_group_water_heights(char *moby) {
    char  *data = ((char *)MOBY(moby)->pvars);
    short *group_member_list = D_L05_001ABCC0[MOBY(moby)->group];
    char(*base)[256] = (char(*)[256])D_L05_0015FFD8;
    do {
        int   group_member_moby_index = *group_member_list & 0x7FFF;
        char *linked_water_moby = (char *)(group_member_moby_index * 256 + (int)base);
        if (MOBY(linked_water_moby)->oclass == 0x33F) {
            char      *linked_water_data = ((char *)MOBY(linked_water_moby)->pvars);
            WaterGrid *water_record;
            float      linked_water_height;
            if (*(int *)(data + 0xC) != 0 ||
                *(float *)(linked_water_data + 4) == *(float *)(data + 4)) {
                linked_water_height = MOBY(moby)->pos.z;
            } else {
                linked_water_height =
                    *(float *)(linked_water_data + 4) + (MOBY(moby)->pos.z - *(float *)(data + 4));
            }
            MOBY(linked_water_moby)->pos.z = linked_water_height;
            water_record = &l05_water_grids[*(int *)(linked_water_data + 0x14)];
            water_record->z = *(float *)(base[group_member_moby_index] + 0x18);
        }
    } while (*group_member_list++ >= 0);
}

void record_grid_entry_neighbor_indices(int selected_entry_index) {
    int        i;
    WaterGrid *entry_table = l05_water_grids;
    for (i = 0; i < 48; i++) {
        if (selected_entry_index != i) {
            WaterGrid *selected_entry = &entry_table[selected_entry_index];
            float      x = selected_entry->x - entry_table[i].x;
            float      y = selected_entry->y - entry_table[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    selected_entry->neighbors[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    selected_entry->neighbors[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    selected_entry->neighbors[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    selected_entry->neighbors[3] = i;
            } else {
                if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(x - 16.0f) < 0.1f)
                        selected_entry->neighbors[5] = i;
                    if (absolute_float(x + 16.0f) < 0.1f)
                        selected_entry->neighbors[7] = i;
                } else if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(x - 16.0f) < 0.1f)
                        selected_entry->neighbors[4] = i;
                    if (absolute_float(x + 16.0f) < 0.1f)
                        selected_entry->neighbors[6] = i;
                }
            }
        }
    }
}

s32 D_L05_001CAD28[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CAD2C[1] __attribute__((section(".data"))) = {0};

f32 D_L05_001CAD30[1] __attribute__((section(".data"))) = {0};

f32 D_L05_001CAD34[1] __attribute__((section(".data"))) = {0};

f32 D_L05_001CAD38[1] __attribute__((section(".data"))) = {0};
