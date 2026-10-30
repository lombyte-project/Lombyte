/* Draws a level's water grids: culls each tile, builds its height grid and submits it. */
#include "types.h"
#include "rnc/ui/text/font_state.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/rendering/vu1/vu1_add_data_ref.h"
#include "qcopy.h"
#include "rnc/rendering/dma_tag.h"
#include "sda.h"

typedef struct {
    u8  pad0[0x190];
    u8  v190[0x10];
    u8  v1A0[0x10];
    u8  pad1B0[0x60];
    f32 f210;
    u8  pad214[0x14];
    f32 f228;
    f32 f22C;
} L01Camera;

extern void FUN_L01_00262388(s32, s32);
extern char D_L01_00167240[];

extern s32 D_L01_0015F5E0 MACRO_ADDR;

extern u32 cached_water_texture_id_gp __asm__("D_L01_00161228") __attribute__((sda));
extern u32 cached_water_second_texture_id_gp __asm__("D_L01_0016122C") __attribute__((sda));
extern u32 D_L01_00161230 __attribute__((sda));
extern u32 cached_water_alpha_1_fix_gp __asm__("D_L01_00161234") __attribute__((sda));
extern union PacketCursor l01_packet __asm__("D_L01_001611C0") MACRO_ADDR;
extern void               FUN_L01_002b4c28(void);

extern L01Camera l01_water_render_camera_state __asm__("D_L01_0016CEC0");

extern void FUN_L01_00261c10(void *, void *) __asm__("FUN_L01_00261c10");
extern void FUN_L01_00261df8(f32 *) __asm__("FUN_L01_00261df8");
extern void update_animated_offset_pairs(f32 *) __asm__("FUN_L01_002b80d0");

void build_height_grid_positions(f32 *grid_base_position) __asm__("FUN_L01_002b8c08");

void build_height_grid_positions(f32 *grid_base_position) {
    volatile f32 *grid_position_cursor = (volatile f32 *)0x70002000;
    f32           y = grid_base_position[1] + pool_light[3];
    s32           i, j;
    for (i = 0; i < 17; i++) {
        f32  x = grid_base_position[0] + pool_light[2];
        f32 *row = (f32 *)0x70000010 + (i + 1) * 24;
        for (j = 0; j < 17; j++) {
            *grid_position_cursor = x;
            grid_position_cursor += 1;
            *grid_position_cursor = y;
            grid_position_cursor += 1;
            *grid_position_cursor = row[j] + grid_base_position[2];
            grid_position_cursor += 1;
            x += pool_light[4];
        }
        y += pool_light[5];
    }
}

void cull_and_submit_water_grid_tile(f32 *grid_center_position,
                                     s32  index,
                                     s32  mode) __asm__("FUN_L01_002b8cb0");

void cull_and_submit_water_grid_tile(f32 *grid_center_position, s32 index, s32 mode) {
    f32 corners[4][4];
    f32 near_clip[4];
    f32 far_clip[4];
    f32 offsets[4][4];
    f32 x_offset, y_offset, x_end, y_end;
    f32 half_x, half_y;
    f32 x, y;
    u32 near_bits, far_bits;
    s32 i, value, row, column;

    if (mode != 0) {
        row = index >> 2;
        column = index & 3;
        half_x = 0.5f * -pool_light[2];
        half_y = 0.5f * -pool_light[3];
        x_offset = half_x * (f32)column;
        y_offset = half_y * (f32)row;
        y_end = y_offset + half_y;
        x_end = x_offset + half_x;
        offsets[0][1] = y_offset;
        offsets[0][0] = x_offset;
        offsets[1][1] = y_offset;
        offsets[1][0] = x_end;
        offsets[2][0] = x_offset;
        offsets[2][1] = y_end;
        offsets[3][0] = x_end;
        offsets[3][1] = y_end;
        x = grid_center_position[0] + pool_light[2];
        y = grid_center_position[1] + pool_light[3];
        corners[0][0] = x + offsets[2][0];
        corners[0][1] = y + offsets[0][1];
        corners[0][2] = grid_center_position[2];
        corners[0][3] = 1.0f;
        corners[1][0] = x + offsets[1][0];
        corners[1][1] = y + offsets[1][1];
        corners[1][2] = grid_center_position[2];
        corners[1][3] = 1.0f;
        corners[2][0] = corners[0][0];
        corners[2][1] = y + offsets[2][1];
        corners[2][2] = grid_center_position[2];
        corners[2][3] = 1.0f;
        corners[3][0] = corners[1][0];
        corners[3][1] = corners[2][1];
        corners[3][2] = grid_center_position[2];
        corners[3][3] = 1.0f;
        near_bits = 0;
        far_bits = 0;
        for (i = 0; i < 4; i++) {
            fast_vec_sub(near_clip, corners[i], D_L01_00167240);
            fast_vec_scale(far_clip, near_clip, 1024.0f);
            fast_vec_scale(near_clip, near_clip, 1024.0f);
            transform_vector(far_clip, far_clip, D_L01_00167240 - 0x80);
            transform_vector(near_clip, near_clip, D_L01_00167240 - 0x40);
            far_bits = (far_bits << 8) | is_vector_outside_clip(far_clip);
            near_bits = (near_bits << 8) | is_vector_outside_clip(near_clip);
        }
        if (mode == 1) {
            if ((near_bits & 0x01010101) == 0x01010101)
                return;
            if ((near_bits & 0x02020202) == 0x02020202)
                return;
            if ((near_bits & 0x04040404) == 0x04040404)
                return;
            if ((near_bits & 0x08080808) == 0x08080808)
                return;
            if (far_bits == 0)
                mode = 0;
            if ((near_bits & 0x20202020) == 0x20202020)
                return;
        } else {
            if ((near_bits & 0x01010101) == 0x01010101)
                return;
            if ((near_bits & 0x02020202) == 0x02020202)
                return;
            if ((near_bits & 0x04040404) == 0x04040404)
                return;
            if ((near_bits & 0x08080808) == 0x08080808)
                return;
            if ((near_bits & 0x20202020) == 0x20202020)
                return;
            mode = 0;
        }
    }
    value = (index & 3) * 4 + (index >> 2) * 0x44;
    if (mode != 0) {
        mode = 0xe;
    } else {
        mode = 0xc;
    }
    FUN_L01_00262388(mode, value);
}

void build_water_render_setup_packet(void) __asm__("FUN_L01_002b8f98");

void build_water_render_setup_packet(void) {
    f32            scaled_translation_matrix[4][4];
    struct DmaTag *base;
    u8            *packet_write_cursor;
    L01Camera     *camera;

    set_scale_matrix(scaled_translation_matrix, 1024.0f);
    fast_vec_scale(scaled_translation_matrix[3], D_L01_00167240, -1024.0f);
    scaled_translation_matrix[3][3] = 1.0f;
    if (D_L01_0015F5E0 != 7) {
        vu1_add_data_ref(font_vu_program, font_vu_program_tag[0]);
        D_L01_0015F5E0 = 7;
    }
    l01_packet.tag->tag = 0x10000000;
    l01_packet.tag->addr = 0;
    l01_packet.tag->vif0 = 0x11000000;
    l01_packet.tag->vif1 = 0x1000404;
    base = l01_packet.tag;
    base[1].tag = 0;
    base[1].addr = 0;
    base[1].vif0 = 0;
    base[1].vif1 = 0x6C0C43A4;
    multiply_matrix(base + 2, D_L01_00167240 - 0x100, scaled_translation_matrix);
    multiply_matrix(base + 6, D_L01_00167240 - 0x80, scaled_translation_matrix);
    base[10].tag = 0x8000;
    base[10].addr = 0x303EC000;
    base[10].vif0 = 0x412;
    camera = &l01_water_render_camera_state;
    *(f32 *)&base[10].vif1 = camera->f210;
    packet_write_cursor = (u8 *)(base + 11);
    qcopy(packet_write_cursor, camera->v190);
    packet_write_cursor = (u8 *)(base + 12);
    qcopy(packet_write_cursor, camera->v1A0);
    *(f32 *)&base[13].tag = camera->f22C;
    *(f32 *)&base[13].addr = camera->f228;
    base[13].vif0 = 0;
    base[13].vif1 = 0;
    base[14].addr = 0x20001D2;
    base[14].vif0 = 0x15000000;
    base[14].tag = 0x3000000;
    base[14].vif1 = 0;
    packet_write_cursor = (u8 *)(base + 15);
    l01_packet.tag->tag |= ((packet_write_cursor - (u8 *)l01_packet.tag) >> 4) - 1;
    l01_packet.bytes = packet_write_cursor;
    FUN_L01_002b4c28();
    l01_packet.tag->tag = 0x30000007;
    l01_packet.tag->addr = (u32)D_L01_001CAC80;
    l01_packet.tag->vif0 = 0;
    l01_packet.tag->vif1 = 0x50000007;
    cached_water_texture_id_gp = 0xFFFFFFFF;
    cached_water_second_texture_id_gp = 0xFFFFFFFF;
    D_L01_00161230 = 0xFFFFFFFF;
    cached_water_alpha_1_fix_gp = 0xFFFFFFFF;
    l01_packet.tag++;
}

void render_water_grids(f32 *water_record, s32 count) __asm__("FUN_L01_002b91c8");

void render_water_grids(f32 *water_record, s32 count) {
    f32  corners[4][4];
    f32  near_clip[4];
    f32  far_clip[4];
    f32  x_offset, y_offset, x_end, y_end, z;
    u32  near_bits, far_bits;
    s32  mask;
    s32  i, j, mode;
    s32  height_index, index;
    f32 *height;
    u32 *packet;
    u64 *data;

    build_water_render_setup_packet();
    i = 0;
    if (count > 0)
        do {
            f32 *next_pos = water_record + 0x464;
            u16  active = *(u16 *)((u8 *)water_record + 0x1e);
            if (active == 0)
                goto next_entity;
            x_offset = water_record[0] + pool_light[2];
            x_end = water_record[0] - pool_light[2];
            corners[0][0] = x_offset;
            y_offset = water_record[1] + pool_light[3];
            y_end = water_record[1] - pool_light[3];
            corners[0][1] = y_offset;
            z = water_record[2];
            corners[0][2] = z;
            corners[0][3] = 1.0f;
            corners[1][0] = x_end;
            corners[1][1] = y_offset;
            corners[1][2] = z;
            corners[1][3] = 1.0f;
            corners[2][0] = x_offset;
            corners[2][1] = y_end;
            corners[2][2] = z;
            corners[2][3] = 1.0f;
            corners[3][0] = x_end;
            corners[3][1] = y_end;
            corners[3][2] = z;
            corners[3][3] = 1.0f;
            near_bits = 0;
            far_bits = 0;
            for (j = 0; j < 4; j++) {
                fast_vec_sub(near_clip, corners[j], D_L01_00167240);
                fast_vec_scale(far_clip, near_clip, 1024.0f);
                fast_vec_scale(near_clip, near_clip, 1024.0f);
                transform_vector(far_clip, far_clip, D_L01_00167240 - 0x80);
                transform_vector(near_clip, near_clip, D_L01_00167240 - 0x40);
                far_bits = (far_bits << 8) | is_vector_outside_clip(far_clip);
                near_bits = (near_bits << 8) | is_vector_outside_clip(near_clip);
            }
            if ((near_bits & 0x01010101) == 0x01010101)
                goto next_entity;
            if ((near_bits & 0x02020202) == 0x02020202)
                goto next_entity;
            if ((near_bits & 0x04040404) == 0x04040404)
                goto next_entity;
            if ((near_bits & 0x08080808) == 0x08080808)
                goto next_entity;
            if ((near_bits & 0x20202020) == 0x20202020)
                goto next_entity;

            FUN_L01_00261c10((void *)0x70000000,
                             (u8 *)water_record + (D_L01_00161220 * 0x5c0 + 0x50));
            FUN_L01_00261df8(water_record);
            build_height_grid_positions(water_record);
            update_animated_offset_pairs(water_record);
            if (*(u32 *)(water_record + 0x11) & 1) {
                height = (f32 *)0x70002008;
                for (height_index = 0x10; height_index >= 0; height_index--, height += 3)
                    *height = water_record[2];
            }
            if (*(u32 *)(water_record + 0x11) & 2) {
                height = (f32 *)0x700020c8;
                for (height_index = 0x10; height_index >= 0; height_index--, height += 0x33)
                    *height = water_record[2];
            }
            if (*(u32 *)(water_record + 0x11) & 4) {
                height = (f32 *)0x70002cc8;
                for (height_index = 0x10; height_index >= 0; height_index--, height += 3)
                    *height = water_record[2];
            }
            if (*(u32 *)(water_record + 0x11) & 8) {
                height = (f32 *)0x70002008;
                for (height_index = 0x10; height_index >= 0; height_index--, height += 0x33)
                    *height = water_record[2];
            }
            mode = (far_bits & 0x2f2f2f2f) != 0;
            if (!mode) {
                mode = 2;
                if (!(near_bits & 0x2f2f2f2f))
                    mode = 0;
            }
            mask = *(u16 *)((u8 *)water_record + 0x1e);
            if (*(u32 *)(water_record + 5) != cached_water_texture_id_gp ||
                *(u32 *)(water_record + 6) != cached_water_second_texture_id_gp ||
                *(u8 *)(water_record + 7) != D_L01_00161230 ||
                *(u8 *)((u8 *)water_record + 0x1d) != cached_water_alpha_1_fix_gp) {
                l01_packet.words[0] = 0x10000005;
                l01_packet.words[1] = 0;
                l01_packet.words[2] = 0x11000000;
                l01_packet.words[3] = 0x50000005;
                packet = l01_packet.words;
                l01_packet.words += 4;
                data = (u64 *)(packet + 4);
                data[0] = 0x4000000000008001ULL;
                data[1] = 0xeeee;
                data[2] = ((u64) * (u8 *)((u8 *)water_record + 0x1d) << 32) | 100;
                data[3] = 0x42;
                data[4] = ((u64) * (u8 *)(water_record + 7) << 32) | 100;
                data[5] = 0x43;
                data[6] = get_effect_texture(*(s32 *)(water_record + 6));
                data[7] = 6;
                data[8] = get_effect_texture(*(s32 *)(water_record + 5));
                data[9] = 7;
                l01_packet.words = packet + 0x18;
                {
                    u32 texture_id = *(u32 *)(water_record + 5);
                    u32 second_texture_id = *(u32 *)(water_record + 6);
                    cached_water_texture_id_gp = texture_id;
                    cached_water_second_texture_id_gp = second_texture_id;
                }
                D_L01_00161230 = *(u8 *)(water_record + 7);
                cached_water_alpha_1_fix_gp = *(u8 *)((u8 *)water_record + 0x1d);
            }
            for (index = 0; index < 16; index++, mask = (s32)mask >> 1) {
                if (mask & 1)
                    cull_and_submit_water_grid_tile(water_record, index, mode);
            }
        next_entity:
            i++;
            water_record = next_pos;
        } while (i < count);
    l01_packet.words[0] = 0x10000000;
    l01_packet.words[1] = 0;
    l01_packet.words[2] = 0x13000000;
    l01_packet.words[3] = 0;
    l01_packet.words += 4;
}

s32 D_L05_0015F5E0 = 0;
u32 D_L05_00161228 = 0xffffffff;
u32 D_L05_0016122C = 0xffffffff;
u32 D_L05_00161230 = 0xffffffff;
u32 D_L05_00161234 = 0xffffffff;
s32 D_L07_0015F5E0 = 0;
u32 D_L07_00161228 = 0xffffffff;
u32 D_L07_0016122C = 0xffffffff;
u32 D_L07_00161230 = 0xffffffff;
u32 D_L07_00161234 = 0xffffffff;
s32 D_L11_0015F5E0 = 0;
u32 D_L11_00161228 = 0xffffffff;
u32 D_L11_0016122C = 0xffffffff;
u32 D_L11_00161230 = 0xffffffff;
u32 D_L11_00161234 = 0xffffffff;
s32 D_L12_0015F5E0 = 0;
u32 D_L12_00161228 = 0xffffffff;
u32 D_L12_0016122C = 0xffffffff;
u32 D_L12_00161230 = 0xffffffff;
u32 D_L12_00161234 = 0xffffffff;
s32 D_L13_0015F5E0 = 0;
u32 D_L13_00161228 = 0xffffffff;
u32 D_L13_0016122C = 0xffffffff;
u32 D_L13_00161230 = 0xffffffff;
u32 D_L13_00161234 = 0xffffffff;
