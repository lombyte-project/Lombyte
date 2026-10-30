/* Level 07 moby classes 0x386, 0x3ad, 0x3af, 0x3b0, 0x3b4, 0x3b5, 0x3b6, 0x3b7, 0x3b8, 0x3b9, 0x3ba, 0x3bb, 0x3be, 0x3c1, 0x3c2, 0x3c7, 0x3c8, 0x3c9, 0x3ca, 0x3cb: water. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"

extern int  is_point_inside_clip_volume() __asm__("FUN_00214720");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern int  is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");
extern f32  random_float_between(f32, f32) __asm__("FUN_002132a8");

/* Ported from rac1-decomp src/overlays/l07_umbris/vendor_002CE470.c (func_L07_0030D8E8) */
#include "rnc/overlay/water_grid.h"
#include "rnc/overlay/shared/rendering/render_water_grids.h"

typedef struct {
    float         f0; /* 0x00 */
    float         f4;
    float         f8;
    float         fC;
    float         f10; /* 0x10 */
    float         f14;
    float         f18;
    float         f1C;
    float         f20; /* 0x20 */
    float         f24;
    int           i28; /* 0x28 */
    int           i2C;
    float         f30; /* 0x30 */
    float         f34;
    float         f38;
    unsigned char b3C; /* 0x3C */
    unsigned char b3D;
} Cab;

typedef struct {
    char  pad0[0x10];
    float f10;
    float f14;
} Dir;

typedef struct {
    float hi;     /* 0x00 */
    float lo;     /* 0x04 */
    int   f8;     /* 0x08 */
    int   fC;     /* 0x0C */
    int   volume; /* 0x10 */
    int   idx;    /* 0x14 */
    int   timer;  /* 0x18 */
    int   f1C;    /* 0x1C */
} Data;

typedef struct {
    char           pad0[0x10];
    float          pos[4]; /* 0x10 */
    unsigned char  state;  /* 0x20 */
    unsigned char  f21;    /* 0x21 */
    char           pad22[0x12];
    unsigned short flags; /* 0x34 */
    char           pad36[0x42];
    Data          *data; /* 0x78 */
    char           pad7C[0x18];
    int            f94; /* 0x94 */
    char           pad98[0x24];
    unsigned char  sub; /* 0xBC */
} Moby_30D8E8;

extern Cab        l07_water_grid_parameters __asm__("D_L07_001CA900");
extern Dir        l07_water_direction_source __asm__("D_L07_0017FF40");
extern WaterGrid *D_L07_001612D0;
extern char       D_0013D4EC[];
extern float      D_0015ED6C;
extern char       D_0013D4C0[];
extern float      D_L07_0016120C;
extern float      D_L07_00161210;
extern float      D_L07_00161214;
extern int        D_L07_00161208;
extern void      *l07_water_grid_geometry_descriptor_base __asm__("D_L07_001612D4");
extern int        D_L07_001612D8;

/* no foreign declaration */
extern int water_grid_override_index __asm__("D_L07_00161B30") __attribute__((sda));

/* no foreign declaration */
extern unsigned short l07_water_grid_override_control_value __asm__("D_L07_00161B34")
    __attribute__((sda));

/* no foreign declaration */
extern int water_shared_byte_clear_enabled __asm__("D_L07_00161B38") __attribute__((sda));

/* no foreign declaration */
extern int            water_grid_geometry_descriptors[][4] __asm__("D_L07_00204700");
extern s32            random_integer_below(s32) __asm__("func_00213260");
extern unsigned short l07_water_grid_saved_control_values[] __asm__("D_L07_00204680");
void                  set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");
void                  pool_set_scale(float s) __asm__("FUN_L01_002b7ef0");
void                  render_level07_water_grids(void) __asm__("FUN_L07_0030cd20");
void                  initialize_level07_water_grid_neighbors(int arg) __asm__("FUN_L07_0030cd48");
extern void           FUN_L01_002b7a48_2cd2b0(void *, int) __asm__("FUN_L01_002b7a48");
extern void           pool_set_scale(float) __asm__("FUN_L01_002b7ef0");
extern void           FUN_L01_002b7c68_2cd2b0(void *, void *) __asm__("FUN_L01_002b7c68");
extern void
FUN_L01_002b8288_2cd2b0(void *, int, float, float, float, float) __asm__("FUN_L01_002b8288");
extern void  initialize_level07_water_grid_neighbors(int) __asm__("FUN_L07_0030cd48");
extern void  set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern int   FUN_L07_00281ef8_2cd2b0(void *, int, int, int) __asm__("FUN_L07_00281ef8");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void  FUN_L01_0026e090_2cd2b0(int, int) __asm__("FUN_L01_0026e090");
extern void
FUN_L00_002a3ec8_2cd2b0(void *, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern int        FUN_L01_00275690_2cd2b0(void *, float) __asm__("FUN_L01_00275690");
extern void       FUN_L01_002b7fe0_2cd2b0(void *, int) __asm__("FUN_L01_002b7fe0");
void              water_update(Moby_30D8E8 *m) __asm__("FUN_L07_0030c508");
void              mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern WaterGrid *D_L07_001612D0;
extern int        D_L07_001612D8;
extern float      random_float_between(float, float) __asm__("FUN_002132a8");

s32 D_L07_001CA928[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CA92C[1] __attribute__((section(".data"))) = {0};

f32 D_L07_001CA930[1] __attribute__((section(".data"))) = {0};

f32 D_L07_001CA934[1] __attribute__((section(".data"))) = {0};

f32 D_L07_001CA938[1] __attribute__((section(".data"))) = {0};

int water_shared_byte_clear_enabled __asm__("D_L07_00161B38") = 0;

void water_update(Moby_30D8E8 *water_moby) {
    enum {
        WATER_INITIALIZE = 0,
        WATER_APPROACH_HIGH_HEIGHT = 1,
        WATER_APPROACH_LOW_HEIGHT = 2,
        WATER_WAIT_FOR_HERO_IN_VOLUME = 3,
        WATER_WAIT_BEFORE_RISING = 4,
        WATER_TIMED_RISE = 5,
        WATER_RISE_TO_HEIGHT_54 = 6,
        WATER_REMOVE_ABOVE_HIGH_HEIGHT = 7
    };
    Data *water_data = water_moby->data;
    int   i;
    int   j;
    int   k;
    int   water_record_index;
    float water_direction_xy_angle;
    float on;

    if (water_data == 0) {
        return;
    }
    if (water_shared_byte_clear_enabled != 0) {
        D_0013D4C0[4] = 0;
        D_0013D4EC[0] = 0;
    }
    switch (water_moby->state) {
    case WATER_INITIALIZE:
        water_moby->flags |= 0x81;
        if (water_data->idx == 0) {
            Cab *cab;
            Dir *water_direction_source;
            int(*p)[4];
            FUN_L01_002b7a48_2cd2b0(l07_water_grids, 0x24);
            pool_set_scale(1.0f);
            cab = &l07_water_grid_parameters;
            water_direction_source = &l07_water_direction_source;
            cab->f0 = 16.0f;
            cab->f4 = 16.0f;
            cab->f8 = -8.0f;
            cab->fC = -8.0f;
            cab->f10 = 1.0f;
            cab->f14 = 1.0f;
            cab->f18 = 0.9f;
            cab->f20 = 0.1f;
            water_direction_xy_angle =
                fast_arc_tan(water_direction_source->f10, water_direction_source->f14);
            cab->f30 = fast_cos(water_direction_xy_angle) * 0.57735f;
            cab->f34 = fast_sin(water_direction_xy_angle) * 0.57735f;
            cab->f38 = -0.57735f;
            cab->i28 = 0x28;
            cab->i2C = 0x29;
            p = water_grid_geometry_descriptors;
            D_L07_001612D8 = 0x24;
            D_L07_0016120C = 32768.0f;
            D_L07_00161210 = 255.0f;
            D_L07_00161214 = 48.0f;
            cab->b3C = 0x40;
            cab->b3D = 0x40;
            D_L07_001612D0 = l07_water_grids;
            l07_water_grid_geometry_descriptor_base = p;
            D_L07_00161208 = 0;
            for (i = 0; i < 0x24; i++) {
                FUN_L01_002b7c68_2cd2b0(&l07_water_grids[i], p[i]);
                l07_water_grids[water_data->idx].draw_value =
                    l07_water_grid_saved_control_values[water_data->idx];
            }
            for (k = 0; k < 0x24; k++) {
                WaterGrid *water_records = l07_water_grids;
                WaterGrid *r = &water_records[k];
                for (j = 0; j < 2; j++) {
                    float x = r->x + random_float_between(-6.0f, 6.0f);
                    float y = r->y + random_float_between(-6.0f, 6.0f);
                    FUN_L01_002b8288_2cd2b0(water_records, 0x24, x, y, 2.0f, 0.2f);
                }
            }
            for (water_record_index = 0; water_record_index < 0x24; water_record_index++) {
                initialize_level07_water_grid_neighbors(water_record_index);
            }
        }
        if (water_data->fC != 0) {
            water_moby->state = 3;
            water_data->timer = scale_game_frames(0x3C);
            if (water_moby->pos[2] < water_data->lo) {
                l07_water_grids[water_data->idx].draw_value = 0;
            }
        } else if (water_data->f8 != 0) {
            water_moby->state = 1;
        } else {
            water_moby->state = 2;
        }
        break;
    case WATER_APPROACH_LOW_HEIGHT:
        if (water_moby->sub & 4) {
            set_moby_list_state(water_moby->f21, 1);
        } else if (FUN_L07_00281ef8_2cd2b0(water_moby, water_moby->f21, 1, 0) &&
                   water_moby->sub != 1) {
            approach_value(&water_moby->pos[2], water_data->lo, D_0015ED6C * 4.0f);
            FUN_L01_0026e090_2cd2b0(water_moby->f21, 1);
        } else if (water_moby->sub == 1) {
            approach_value(&water_moby->pos[2], water_data->lo, D_0015ED6C * 4.0f);
        }
        if (random_integer_below(200) == 0) {
            float      x = l07_water_grids[water_data->idx].x + random_float_between(-4.0f, 4.0f);
            float      y = random_float_between(-4.0f, 4.0f);
            WaterGrid *r = &l07_water_grids[water_data->idx];
            FUN_L00_002a3ec8_2cd2b0(r, 1, 1, x, r->y + y, 1.0f, -0.05f);
        }
        break;
    case WATER_APPROACH_HIGH_HEIGHT:
        if (water_moby->sub & 8) {
            set_moby_list_state(water_moby->f21, 2);
        } else if (FUN_L07_00281ef8_2cd2b0(water_moby, water_moby->f21, 1, 0) &&
                   water_moby->sub != 2) {
            approach_value(&water_moby->pos[2], water_data->hi, D_0015ED6C * 4.0f);
            FUN_L01_0026e090_2cd2b0(water_moby->f21, 2);
        } else if (water_moby->sub == 2) {
            approach_value(&water_moby->pos[2], water_data->hi, D_0015ED6C * 4.0f);
        }
        if (random_integer_below(200) == 0) {
            float      x = l07_water_grids[water_data->idx].x + random_float_between(-4.0f, 4.0f);
            float      y = random_float_between(-4.0f, 4.0f);
            WaterGrid *r = &l07_water_grids[water_data->idx];
            FUN_L00_002a3ec8_2cd2b0(r, 1, 1, x, r->y + y, 1.0f, -0.05f);
        }
        break;
    case WATER_WAIT_FOR_HERO_IN_VOLUME:
        if (water_data->volume >= 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, water_data->volume)) {
            water_data->f1C = 0;
            set_moby_list_state(water_moby->f21, 4);
        }
        break;
    case WATER_WAIT_BEFORE_RISING:
        if (fast_dec_timer(&water_data->timer)) {
            water_data->timer = scale_game_frames(0x708);
            water_moby->state = 5;
        }
        break;
    case WATER_TIMED_RISE:
        water_moby->pos[2] += D_0015ED6C * 0.32f;
        if (fast_dec_timer(&water_data->timer)) {
            water_moby->state = 6;
        }
        break;
    case WATER_RISE_TO_HEIGHT_54:
        water_moby->pos[2] += D_0015ED6C * 0.42f;
        if (water_moby->pos[2] >= 54.0f) {
            water_moby->state = 7;
        }
        break;
    case WATER_REMOVE_ABOVE_HIGH_HEIGHT:
        if (water_data->hi < water_moby->pos[2]) {
            l07_water_grids[water_data->idx].draw_value = 0;
            mark_moby_for_removal(water_moby);
            return;
        }
        break;
    }
    if (water_data->idx < 0) {
        return;
    }
    {
        Cab *cab = &l07_water_grid_parameters;
        l07_water_grids[water_data->idx].unk1C = cab->b3C;
        l07_water_grids[water_data->idx].unk1D = cab->b3D;
    }
    l07_water_grids[water_data->idx].z = water_moby->pos[2];
    on = 0.0f;
    if (water_moby->f94 != 0 && FUN_L01_00275690_2cd2b0(water_moby, 48.0f) != -1) {
        if (!(water_moby->state >= 3 && water_moby->state <= 7) ||
            (water_moby->pos[2] > water_data->lo && water_moby->pos[2] < water_data->hi)) {
            on = 1.0f;
        }
    }
    if (on == 0.0f) {
        l07_water_grids[water_data->idx].draw_value = 0;
    } else {
        l07_water_grids[water_data->idx].draw_value =
            l07_water_grid_saved_control_values[water_data->idx];
    }
    if (water_data->idx == 0) {
        FUN_L01_002b7fe0_2cd2b0(l07_water_grids, 0x24);
        add_draw_callback(render_level07_water_grids, water_moby);
    }
    if (water_data->idx == water_grid_override_index) {
        l07_water_grids[water_data->idx].draw_value = l07_water_grid_override_control_value;
        l07_water_grid_saved_control_values[water_data->idx] =
            l07_water_grid_override_control_value;
    }
}

/* Draw callback of the water mobys: draws this level's water grids. */
void render_level07_water_grids(void) {
    render_water_grids(l07_water_grids, 0x24);
}

/* Level overlay code; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */
/* records, for entry arg, which of the 48 entries sit one grid step away */
/* Same routine as FUN_L05_0030d6a0, with this level's table and entry count. */
void initialize_level07_water_grid_neighbors(int selected_entry_index) {
    int i;
    l07_water_grids[selected_entry_index].neighbors[0] = 0xFF;
    l07_water_grids[selected_entry_index].neighbors[2] = 0xFF;
    l07_water_grids[selected_entry_index].neighbors[1] = 0xFF;
    l07_water_grids[selected_entry_index].neighbors[3] = 0xFF;
    l07_water_grids[selected_entry_index].neighbors[4] = 0xFF;
    l07_water_grids[selected_entry_index].neighbors[5] = 0xFF;
    l07_water_grids[selected_entry_index].neighbors[6] = 0xFF;
    l07_water_grids[selected_entry_index].neighbors[7] = 0xFF;
    for (i = 0; i < 36; i++) {
        if (selected_entry_index != i) {
            WaterGrid *entry_table = l07_water_grids;
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
                if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_entry->neighbors[5] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_entry->neighbors[7] = i;
                } else if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_entry->neighbors[4] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_entry->neighbors[6] = i;
                }
            }
        }
    }
}
