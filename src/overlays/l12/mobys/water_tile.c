/* Level 12 moby class water_tile: the tiles of the water grid, the group height copy and water grid draw they use, and their neighbour slots. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/props/pool_set_scale.h"
#include "rnc/overlay/shared/props/pool_init_tiles.h"
#include "rnc/overlay/shared/rendering/render_water_grids.h"
#include "qcopy.h"

typedef struct {
    float position[3];
    char  unk0C[0x12];
    u16   visibility;
    char  unk20[0x1170];
} L12Slot;

extern char D_L12_001CBF40_c[] __asm__("D_L12_001CBF40");
extern char D_L12_001CAD00_c[] __asm__("D_L12_001CAD00");
extern char D_L12_001CBE00_c[] __asm__("D_L12_001CBE00");

extern char water_tile_limit_message[] __asm__("D_L12_0020BC90");
extern char D_L12_00167240[];
void initialize_level_12_slot_grid_neighbors(int selected_slot_index) __asm__("FUN_L12_002bf770");

extern void initialize_level_12_slot_grid_neighbors(int) __asm__("FUN_L12_002bf770");

#include "rnc/overlay/l12/mobys/water_tile_globals.h"

extern int   next_water_tile_index_gp __asm__("D_L12_00151388") __attribute__((sda));
extern void  debug_print(int) __asm__("FUN_001e93b0");
extern float water_tile_direction_inputs[8] __asm__("D_L12_00180340");
extern int   D_L12_001612D0;
extern void *l12_water_tile_position_records __asm__("D_L12_001612D4");
extern int   D_L12_001612D8;
extern float D_L12_0016120C;
extern float D_L12_00161210;
extern float D_L12_00161214;
extern int   D_L12_00161208;
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void pool_splash(float, float, float, float, void *, int, int) __asm__("FUN_L00_002a3ec8");
extern int  FUN_L01_00275690_c(float, void *) __asm__("FUN_L01_00275690");
extern int  is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void register_callback_c(void *, void *) __asm__("FUN_L12_0021be50");
void        copy_moby_z_to_group_records(struct Moby *moby) __asm__("FUN_L12_002bf6d0");
void        render_level_12_water_grids(void) __asm__("FUN_L12_002bf748");

void                update_level_12_water_tile(struct Moby *moby) __asm__("FUN_L12_002bf140");
extern int          random_integer_below(int);
typedef struct Moby Moby;

#include "rnc/overlay/level_globals.h"

extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");

void update_level_12_water_tile(struct Moby *moby) {
    enum {
        SLOT_INITIALIZE = 0,
        SLOT_INITIALIZE_NEIGHBORS_AND_HEIGHT = 1,
        SLOT_UPDATE_HEIGHT_AND_MOTION = 2
    };
    int   state = moby->state;
    int  *data = (int *)moby->pvars;
    int   slot;
    float value;
    float other;
    int   i;
    switch (state) {
    case SLOT_INITIALIZE:
        i = (data[3] = next_water_tile_index_gp) + 1;
        next_water_tile_index_gp = i;
        if (i > 0x7f) {
            debug_print((int)water_tile_limit_message);
        }
        moby->scale = moby->pclass->scale * 2.0f;
        data[0] = 3;
        moby->update_dist = 0xff;
        moby->flags |= 1;
        if (data[3] == 0) {
            {
                int i = 16;
                do {
                    ((L12Slot *)D_L12_001CBF40_c)[i].visibility = 0;
                } while (--i >= 0);
            }
            pool_init_tiles(D_L12_001CBF40_c, 17);
            pool_set_scale(1.0f);
            *(float *)(D_L12_001CAD00_c + 0x00) = 16.0f;
            *(float *)(D_L12_001CAD00_c + 0x04) = 16.0f;
            *(float *)(D_L12_001CAD00_c + 0x08) = -8.0f;
            *(float *)(D_L12_001CAD00_c + 0x0c) = -8.0f;
            *(float *)(D_L12_001CAD00_c + 0x10) = 1.0f;
            *(float *)(D_L12_001CAD00_c + 0x14) = 1.0f;
            *(float *)(D_L12_001CAD00_c + 0x18) = 0.9f;
            *(float *)(D_L12_001CAD00_c + 0x20) = 0.1f;
            value = fast_arc_tan(water_tile_direction_inputs[4], water_tile_direction_inputs[5]);
            *(float *)(D_L12_001CAD00_c + 0x30) = fast_cos(value) * 0.57735f;
            *(float *)(D_L12_001CAD00_c + 0x34) = fast_sin(value) * 0.57735f;
            *(float *)(D_L12_001CAD00_c + 0x38) = -0.57735f;
            *(int *)(D_L12_001CAD00_c + 0x28) = data[0] + 0x28;
            *(int *)(D_L12_001CAD00_c + 0x2c) = data[1] + 0x28;
            *(u8 *)(D_L12_001CAD00_c + 0x3c) = 0x1c;
            *(u8 *)(D_L12_001CAD00_c + 0x3d) = 0x40;
            D_L12_001612D0 = (int)D_L12_001CBF40_c;
            l12_water_tile_position_records = D_L12_001CBE00_c;
            D_L12_001612D8 = 17;
            D_L12_0016120C = 32768.0f;
            D_L12_00161210 = 255.0f;
            D_L12_00161214 = 48.0f;
            D_L12_00161208 = 0;
        }
        *(float *)(D_L12_001CBF40_c + data[3] * 0x1190) = moby->pos.x;
        *(float *)(D_L12_001CBF40_c + data[3] * 0x1190 + 4) = moby->pos.y;
        *(float *)(D_L12_001CBF40_c + data[3] * 0x1190 + 8) = moby->pos.z;
        *(int *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x14) = data[0] + 0x28;
        *(int *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x18) = data[1] + 0x28;
        *(u8 *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x1c) = 0x1c;
        *(u8 *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x1d) = 0x44;
        ((L12Slot *)D_L12_001CBF40_c)[data[3]].visibility = 0xffff;
        pool_bind_tile(D_L12_001CBF40_c + data[3] * 0x1190, D_L12_001CBE00_c + data[3] * 16);
        moby->state = 1;
        break;
    case SLOT_INITIALIZE_NEIGHBORS_AND_HEIGHT:
        next_water_tile_index_gp = 0;
        initialize_level_12_slot_grid_neighbors(data[3]);
        moby->state = 2;
        *(float *)(data + 4) = moby->pos.z;
        break;
    case SLOT_UPDATE_HEIGHT_AND_MOTION:
        if ((moby->command & 4) == 0) {
            moby->command = 1;
        } else {
            data[5] = 1;
            *(float *)(data + 4) += 5.0f;
            moby->command = 1;
        }
        if (data[5] != 0) {
            advance_accelerated_scalar(&moby->pos.z,
                                       (float *)(data + 2),
                                       *(float *)(data + 4),
                                       frame_time_sq * 4.0f,
                                       frame_time_sq * 4.0f,
                                       frame_time * 4.0f);
            copy_moby_z_to_group_records(moby);
        }
        if (absolute_float(hero.motion.pos.f[0] - moby->pos.x) <= 8.0f ||
            absolute_float(hero.motion.pos.f[1] - moby->pos.y) <= 8.0f) {
            hero.water_surface_z = moby->pos.z;
        }
        if (random_integer_below(200) == 0) {
            char *slot_record;
            other = random_float_between(-4.0f, 4.0f);
            value = *(float *)(D_L12_001CBF40_c + data[3] * 0x1190);
            value += other;
            other = random_float_between(-4.0f, 4.0f);
            slot_record = D_L12_001CBF40_c + data[3] * 0x1190;
            pool_splash(
                value, *(float *)(slot_record + 4) + other, 1.0f, -0.05f, slot_record, 1, 1);
        }
        break;
    }
update_slot:
    slot = data[3];
    if (slot >= 0) {
        float visible = 0.0f;
        if (FUN_L01_00275690_c(64.0f, moby) != -1)
            visible = 1.0f;
        if (data[6] != -1 && is_point_inside_clip_volume(D_L12_00167240, data[6]) == 0)
            visible = 0.0f;
        if (visible == 0.0f) {
            ((L12Slot *)D_L12_001CBF40_c)[data[3]].visibility = 0;
        } else {
            ((L12Slot *)D_L12_001CBF40_c)[data[3]].visibility = 0xffff;
        }
        if (data[3] == 0) {
            pool_flush(D_L12_001CBF40_c, 17);
            register_callback_c(render_level_12_water_grids, moby);
        }
    }
}

extern int    D_L12_0015FFD8; /* no foreign declaration */
extern short *group_record_index_lists[] __asm__("D_L12_001ABCC0");

/* Copies the moby's height into every moby of its group and into their water grid slots. */
void copy_moby_z_to_group_records(struct Moby *moby) {
    short *group_index_cursor = group_record_index_lists[moby->group];
    int    base = D_L12_0015FFD8;
    do {
        int   i = (*group_index_cursor & 0x7FFF) * 256;
        char *indexed_record = (char *)(i + base);
        char *linked_record;
        *(float *)(indexed_record + 0x18) = moby->pos.z;
        linked_record =
            D_L12_001CBF40_c + *(int *)(*(char **)(indexed_record + 0x78) + 0xc) * 0x1190;
        *(float *)(linked_record + 8) = moby->pos.z;
    } while (*group_index_cursor++ >= 0);
}

/* Draw callback: renders the level's 0x11 water grids. */
void render_level_12_water_grids(void) {
    render_water_grids(D_L12_001CBF40_c, 0x11);
}

/* records, for entry arg, which of the 48 entries sit one grid step away */
/* Same routine as FUN_L05_0030d6a0, with this level's table and entry count. */

typedef struct {
    float         x;
    float         y;
    int           pad;
    unsigned char n[0x14];
    char          rest[0x1170];
} Ent;

extern Ent D_L12_001CBF40[];

void initialize_level_12_slot_grid_neighbors(int selected_slot_index) {
    int i;
    D_L12_001CBF40[selected_slot_index].n[0] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[2] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[1] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[3] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[4] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[5] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[6] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[7] = 0xFF;
    for (i = 0; i < 17; i++) {
        if (selected_slot_index != i) {
            Ent  *slot_table = D_L12_001CBF40;
            Ent  *selected_slot = &slot_table[selected_slot_index];
            float x = selected_slot->x - slot_table[i].x;
            float y = selected_slot->y - slot_table[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    selected_slot->n[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    selected_slot->n[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    selected_slot->n[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    selected_slot->n[3] = i;
            } else {
                if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_slot->n[5] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_slot->n[7] = i;
                } else if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_slot->n[4] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_slot->n[6] = i;
                }
            }
        }
    }
}
