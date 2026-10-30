#include "types.h"
#include "rnc/globals.h"
#include "eetypes.h"
#include "qcopy.h"
#include "sda.h"

#include "rnc/math/vector.h"
#include "rnc/rendering/shrub_instance.h"
#include "rnc/rendering/shrub_render_class.h"
#include "rnc/rendering/tie_instance.h"
#include "rnc/rendering/object_render_class.h"
#include "rnc/rendering/image_clear_buffer.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/ties/light_ties.h"
#include "rnc/rendering/tfrags/light_tfrags.h"
#include "rnc/runtime/state/enable_global_flag.h"

typedef struct FRow {
    f32 v[3];
    f32 w;
} FRow;

typedef struct IRow {
    f32 v[3];
    s32 w;
} IRow;

typedef struct TieData {
    FRow m[4];
    u8   pad40[0x100];
    u16  colors[64]; /* 0x140: copied from the gameplay file */
} TieData;

typedef struct ShrubData {
    IRow m[3];
    FRow m3;
} ShrubData;

typedef struct LevelHeader {
    s32 x0;
    s32 directional_lights_offset; /* 0x04 */
    u8  pad08[0x2C];
    s32 ties_offset; /* 0x34: 0xE0 bytes per tie */
    u8  pad38[4];
    s32 shrubs_offset; /* 0x3C: 0x70 bytes per shrub */
} LevelHeader;

typedef struct Ent100 {
    u8  pad00[0x20];
    u8  x20;
    u8  pad21[0x15];
    s16 x36;
    u8  pad38[0xC8];
} Ent100;

typedef struct Ent40 {
    u8  pad00[0x36];
    u16 x36;
    u8  pad38[2];
    s16 x3A;
    u8  pad3C[4];
} Ent40;

typedef struct Display {
    u8  pad00[0x23C];
    s32 x23C;
    s32 x240;
    s32 x244;
} Display;

typedef struct Buf194100 {
    s32 x0;
    u8 *x4;
    u8 *x8;
    s32 xC;
    s32 x10;
    s32 x14;
} Buf194100;

typedef struct OcclCamState {
    u8  pad00[0x1C];
    s32 x1C;
} OcclCamState;

typedef struct S160AB0 {
    s32 x0;
    u8 *x4;
} S160AB0;

extern u8                 D_00100AE0[];
extern u8                 D_0013F350[];
extern s32                D_0015ED80;
extern Ent100            *D_0015FF18;
extern Ent100            *moby_instance_end __asm__("D_0015FF1C");
extern Ent100            *moby_instance_perm_end __asm__("D_0015FF20");
extern u8                *moby_vars __asm__("D_0015FF28");
extern s32                occl_fog_count __asm__("D_0015FF30");
extern u8                *D_001600AC;
extern s32                D_001600B0;
extern s32                D_001600B4;
extern s32                D_001600B8;
extern ShrubData         *shrub_data_table __asm__("D_001603DC");
extern u8                *shrub_colors __asm__("D_001603E0"); /* 24 RGBA colours per shrub */
extern f32                occl_fog_density_set __asm__("D_001603E4");
extern S160AB0 D_00160AB0 NOT_SDA;
extern Ent40             *tfrag_render_records __asm__("D_00160E8C") __attribute__((sda));
extern s32                tfrag_render_record_count __asm__("D_00160E90");
extern f32                D_00160EC0;
extern TieData           *tie_data_table __asm__("D_00160F58");
extern s32                tie_count __asm__("D_00160F5C");
extern f32                D_00160F70;
extern u8                 D_00186F40[];
extern Display            D_0018CD00;
extern u8 *D_001940D8     NOT_SDA;
extern Buf194100          coll_output __asm__("D_00194100");
extern u8                 D_00194180[];
extern char               dir_lights[] __asm__("D_0019BDC0");
extern u8                 point_lights[] __asm__("D_0019C1C0");
extern u8                 D_001AAA40[];
extern u8                 moby_grids[] __asm__("D_001B76E0");
extern u8                 D_001C76E0[];
extern u8                 D_001CD780[];
extern u8                 object_render_class_slot_by_id[] __asm__("D_001E1A00");
extern char               too_many_directional_lights_message[] __asm__("D_001E76D8");

extern void DebugPrint(char *fmt, ...);
extern void fill_transfer_words(void *, s32, s32) __asm__("FUN_001f97e8");
extern void PackDmaTag(s32 arg0, u64 arg1, u64 arg2);
extern void update_fog(void) __asm__("func_001F2588");
extern void reset_draw_globals(void) __asm__("func_001F37E8");
extern void scale_vector(void *, void *, f32) __asm__("FUN_001f9a80");
extern void set_video_timing(s32) __asm__("func_00214970");
extern void reset_global_state_fields(void) __asm__("func_00217020");
extern void FUN_0022a5e0(void *);
extern void vu0_load_micro_program(void *) __asm__("func_002334D8");

u8 *initialize_level_render_state(LevelHeader *header) __asm__("FUN_001e9b10");

u8 *initialize_level_render_state(LevelHeader *header) {
    u8            *pool;
    u8            *environment;
    u8            *light_record;
    u8            *tie_record;
    u8            *shrub_record;
    s32            light_count;
    s32            shrub_index;
    s32            tie_index;
    s32            tfrag_index;
    s32            tfrag_batch_start;
    s32            tie_light_index;
    s32            shrub_light_index;
    s32            shrub_color_index;
    s32            batch_count;
    s32            tie_unk18;
    s32            last_tie_class;
    s32            last_shrub_class;
    u16           *tfrag_list;
    u16           *tie_list;
    u16           *shrub_list;
    TieInstance   *tie;
    TieData       *tie_data;
    ShrubInstance *shrub;
    ShrubData     *shrub_data;
    s32            shrub_class;
    s32            tie_size;
    s32            tie_data_size;
    s32            shrub_size;
    s32            shrub_data_size;
    s32            shrub_color_size;
    f32            max_axis_length;
    f32            horizontal_scale;
    f32            vertical_scale;
    s32            packed_horizontal;
    s32            packed_vertical;
    s32            red;
    s32            green;
    s32            blue;
    Ent100        *moby;
    TieInstance   *tie_cursor;

    pool = D_001940D8;
    fill_transfer_words(D_0013F350, 0, 0x2310);
    fill_transfer_words(D_00186F40, 0, 0x3A0);
    fill_transfer_words(D_001AAA40, 0, 0x180);
    if (D_0015ED80 != 0) {
        if (frame_scale == 1.0f) {
            set_video_timing(1);
        }
    } else if (frame_scale != 1.0f) {
        set_video_timing(0);
    }
    requested_video_mode = *(u8 *)&D_0015ED80;
    coll_output.x4 = image_clear_buffer;
    coll_output.x8 = D_00194180;
    coll_output.xC = 0;
    coll_output.x10 = 0;
    coll_output.x14 = 0;
    fast_mem_zero16(moby_grids, 0x10000);
    fast_mem_zero16(D_001C76E0, 0x60);

    environment = (u8 *)header + header->x0;
    D_0018CD00.x23C = *(s32 *)environment;
    environment += 4;
    D_0018CD00.x240 = *(s32 *)environment;
    environment += 4;
    D_0018CD00.x244 = *(s32 *)environment;
    environment += 4;
    fog_color_red = *environment;
    environment += 4;
    fog_color_green = *environment;
    environment += 4;
    fog_color_blue = *environment;
    environment += 4;
    fog_near_distance = *(f32 *)environment;
    fog_far_distance = *(f32 *)(environment + 4);
    fog_near_intensity = *(f32 *)(environment + 8);
    fog_far_intensity = *(f32 *)(environment + 12);
    update_fog();
    D_00160EC0 = 512000.0f;
    D_00160F70 = 720.0f;
    occl_fog_density_set = 500.0f;
    occl_fog_count = 500;
    part_clip_dist = 0x1F4000;
    PackDmaTag(D_0018CD00.x23C, D_0018CD00.x240, D_0018CD00.x244);
    vu0_load_micro_program(D_00100AE0);
    fast_mem_zero16(point_lights, 0x100);
    fast_mem_zero16(point_light_data, 0x180);
    fast_mem_zero16(dir_lights, 0x400);

    light_record = (u8 *)header + header->directional_lights_offset;
    light_count = *(s32 *)light_record;
    light_record += 0x10;
    if (light_count >= 12) {
        DebugPrint(too_many_directional_lights_message);
        light_count = 12;
    }
    if (light_count != 0) {
        copy_memory_aligned(dir_lights, light_record, light_count << 6);
    }

    for (tfrag_index = 0; tfrag_index < tfrag_render_record_count; tfrag_index++) {
        tfrag_render_records[tfrag_index].x36 = 0xFFFF;
    }
    tfrag_batch_start = 0;
    while (tfrag_batch_start < tfrag_render_record_count) {
        tfrag_list = (u16 *)0x70003000;
        for (batch_count = 0; batch_count < 0x3FF && tfrag_batch_start < tfrag_render_record_count;
             batch_count++, tfrag_batch_start++) {
            *tfrag_list++ = tfrag_batch_start;
        }
        *tfrag_list = 0xFFFF;
        light_tfrags((void *)0x70003000);
    }

    tie_record = (u8 *)header + header->ties_offset;
    tie_count = *(s32 *)tie_record;
    tie_record += 0x10;
    ties = (TieInstance *)pool;
    tie_size = tie_count << 5;
    pool += tie_size;
    if (tie_count != 0) {
        fill_transfer_words(ties, 0, tie_size);
    }
    tie_data_size = tie_count * 0x1C0;
    pool = (u8 *)(((u32)pool + 0x3F) & ~0x3F);
    tie_data_table = (TieData *)pool;
    pool += tie_data_size;
    if (tie_count != 0) {
        fill_transfer_words(tie_data_table, 0, tie_data_size);
    }
    last_tie_class = -1;
    ties_end = &ties[tie_count];
    for (tie_index = 0; tie_index < tie_count; tie_index++) {
        tie = &ties[tie_index];
        tie_data = &tie_data_table[tie_index];
        tie->cls = object_render_class_slot_by_id[*(s32 *)tie_record];
        if (tie->cls != last_tie_class) {
            object_render_classes[tie->cls]->first_instance = tie;
            object_render_classes[tie->cls]->instance_count = 0;
            last_tie_class = tie->cls;
        }
        object_render_classes[tie->cls]->instance_count++;
        tie->data = tie_data;
        tie->x14 = *(s32 *)(tie_record + 4);
        tie->lights = 0xFFFF;
        tie->x1B = 0;
        tie->x1C = 0;
        tie_unk18 = *(s32 *)(tie_record + 0xC);
        qcopy(&tie_data->m[0], tie_record + 0x10);
        qcopy(&tie_data->m[1], tie_record + 0x20);
        qcopy(&tie_data->m[2], tie_record + 0x30);
        qcopy(&tie_data->m[3], tie_record + 0x40);
        tie->x18 = tie_unk18;
        tie_record += 0x50;
        tie_data->m[3].w = object_render_classes[tie->cls]->unk40;
        transform_vector_by_basis(tie, object_render_classes[tie->cls]->bounding_sphere, tie_data);
        max_axis_length = fast_vec_length(&tie_data->m[0]);
        max_axis_length = max_float(max_axis_length, fast_vec_length(&tie_data->m[1]));
        max_axis_length = max_float(max_axis_length, fast_vec_length(&tie_data->m[2]));
        tie->scale = object_render_classes[tie->cls]->bounding_sphere[3] * max_axis_length;
        scale_vector(tie, tie, tie_data->m[3].w);
        fast_vec_add(tie, tie, &tie_data->m[3]);
        tie_data_table[tie_index].m[0].w = 1.0f / fast_vec_length(&tie_data_table[tie_index].m[0]);
        tie_data_table[tie_index].m[1].w = 1.0f / fast_vec_length(&tie_data_table[tie_index].m[1]);
        tie_data_table[tie_index].m[2].w = 1.0f / fast_vec_length(&tie_data_table[tie_index].m[2]);
        copy_memory_aligned(tie_data->colors, tie_record, 0x80);
        tie_record += 0x80;
        tie->x1C = *(u16 *)tie_record;
        tie_record += 0x10;
    }
    tie_list = (u16 *)0x70000000;
    for (tie_light_index = 0; tie_light_index < tie_count; tie_light_index++) {
        *tie_list++ = tie_light_index;
    }
    *tie_list = 0xFFFF;
    light_ties((void *)0x70000000);

    shrub_record = (u8 *)header + header->shrubs_offset;
    shrub_count = *(s32 *)shrub_record;
    shrub_record += 0x10;
    shrubs = (ShrubInstance *)pool;
    shrub_size = shrub_count << 5;
    pool += shrub_size;
    if (shrub_count != 0) {
        fill_transfer_words(shrubs, 0, shrub_size);
    }
    pool = (u8 *)(((u32)pool + 0x3F) & ~0x3F);
    shrub_data_table = (ShrubData *)pool;
    shrub_data_size = shrub_count << 6;
    pool += shrub_data_size;
    if (shrub_count != 0) {
        fill_transfer_words(shrub_data_table, 0, shrub_data_size);
    }
    shrub_color_size = shrub_count * 0x60;
    shrub_colors = pool;
    pool += shrub_color_size;
    pool = (u8 *)(((u32)pool + 0x3F) & ~0x3F);
    if (shrub_count != 0) {
        fill_transfer_words(shrub_colors, 0, shrub_color_size);
    }
    last_shrub_class = -1;
    /* Retail 0x001ea2a4-0x001ea2c4 publishes the shrub data pointer,
       the end of the shrub array, and the next pool base in this order. */
    D_00160AB0.x4 = shrub_colors;
    shrubs_end = &shrubs[shrub_count];
    for (shrub_index = 0; shrub_index < shrub_count; shrub_index++) {
        shrub = &shrubs[shrub_index];
        shrub_class = shrub_render_class_slot_by_id[*(s32 *)shrub_record];
        shrub_data = &shrub_data_table[shrub_index];
        shrub->cls = shrub_class;
        if (shrub_class != last_shrub_class) {
            last_shrub_class = shrub_class;
            shrub_render_classes[shrub_class]->first_instance = shrub;
            shrub_render_classes[shrub_class]->instance_count = 0;
        }
        shrub_render_classes[shrub_class]->instance_count++;
        shrub->index = shrub_index;
        shrub->radius = *(f32 *)(shrub_record + 4);
        if (shrub->radius < 16.0f) {
            shrub->radius = 16.0f;
        }
        shrub->x1B = 0;
        shrub->lights = 0xFFFF;
        if (shrub_render_classes[shrub_class]->mip_packet != 0) {
            f32 min_radius;

            shrub->x17 =
                truncate_float_to_s32(*(f32 *)shrub_render_classes[shrub_class]->mip_packet);
            min_radius = convert_integer_to_float(shrub->x17) + 24.0f;
            if (shrub->radius < min_radius) {
                shrub->radius = min_radius;
            }
        }
        qcopy(&shrub_data->m[0], shrub_record + 0x10);
        qcopy(&shrub_data->m[1], shrub_record + 0x20);
        qcopy(&shrub_data->m[2], shrub_record + 0x30);
        qcopy(&shrub_data->m3, shrub_record + 0x40);
        shrub_record += 0x50;
        shrub_data->m3.w = shrub_render_classes[shrub_class]->unk20;
        horizontal_scale =
            (fast_vec_length(&shrub_data->m[0]) + fast_vec_length(&shrub_data->m[1])) * 0.5f;
        vertical_scale = fast_vec_length(&shrub_data->m[2]);
        packed_horizontal = truncate_float_to_s32(horizontal_scale * 4096.0f);
        packed_vertical = truncate_float_to_s32(vertical_scale * 4096.0f);
        if (packed_horizontal > 0x10000) {
            packed_horizontal = 0x10000;
        }
        if (packed_vertical > 0x10000) {
            packed_vertical = 0x10000;
        }
        shrub_data->m[2].w = packed_horizontal | (packed_vertical << 16);
        red = *(s32 *)shrub_record;
        shrub_record += 4;
        green = *(s32 *)shrub_record;
        shrub_record += 4;
        blue = *(s32 *)shrub_record;
        shrub_record += 8;
        green = (green << 8) | 0x80000000;
        shrub_data->m[0].w = (blue << 16) | green | red;
        shrub->x1C = *(u16 *)shrub_record;
        shrub_record += 0x10;
        transform_vector_by_basis(shrub, shrub_render_classes[shrub_class], shrub_data);
        max_axis_length = fast_vec_length(&shrub_data->m[0]);
        max_axis_length = max_float(max_axis_length, fast_vec_length(&shrub_data->m[1]));
        max_axis_length = max_float(max_axis_length, fast_vec_length(&shrub_data->m[2]));
        shrub->scale = shrub_render_classes[shrub_class]->bounding_sphere[3] * max_axis_length;
        scale_vector(shrub, shrub, shrub_data->m3.w);
        fast_vec_add(shrub, shrub, &shrub_data->m3);
    }
    shrub_list = (u16 *)0x70000000;
    for (shrub_light_index = 0; shrub_light_index < shrub_count; shrub_light_index++) {
        *shrub_list++ = shrub_light_index;
    }
    *shrub_list = 0xFFFF;
    FUN_0022a5e0((void *)0x70000000);

    for (shrub_color_index = 0; shrub_color_index < shrub_count; shrub_color_index++) {
        s32 red_sum;
        s32 green_sum;
        s32 blue_sum;
        u8 *vertex_color;
        s32 color_index;

        vertex_color = shrub_colors + shrub_color_index * 0x60;
        red_sum = 0;
        green_sum = 0;
        blue_sum = 0;
        for (color_index = 0; color_index < 24; color_index++) {
            red_sum += vertex_color[0];
            green_sum += vertex_color[1];
            blue_sum += vertex_color[2];
            vertex_color += 4;
        }
        red_sum /= 24;
        green_sum /= 24;
        blue_sum /= 24;
        *(s32 *)((u8 *)&shrub_data_table[shrub_color_index] + 0x1C) =
            (blue_sum << 16) | (green_sum << 8) | red_sum;
    }

    D_0015FF18 = (Ent100 *)pool;
    fill_transfer_words(pool, 0, 0x4000);
    pool += 0x4000;
    moby_instance_end = D_0015FF18;
    *((u8 *)D_0015FF18 + 0x20) = 0xFF;
    moby_vars = pool;
    pool += 0x2000;
    D_001600AC = pool;
    moby_instance_perm_end = &D_0015FF18[0x3F];
    pool += 0x20000;
    D_001600B4 = -1;
    D_001600B0 = 0;
    D_001600B8 = 0;
    fast_mem_zero16(D_001CD780, 0x200);
    for (moby = D_0015FF18; moby != moby_instance_perm_end; moby++) {
        moby->x36 = 0x7F80;
    }
    for (tie_cursor = ties; tie_cursor != ties_end; tie_cursor++) {
        tie_cursor->x18 = 0x7F80;
    }
    for (tfrag_index = 0; tfrag_index < tfrag_render_record_count; tfrag_index++) {
        tfrag_render_records[tfrag_index].x3A = 0x7F80;
    }
    ((OcclCamState *)occl_cam_state)->x1C = 0;
    reset_draw_globals();
    reset_global_state_fields();
    enable_global_flag();
    world_update_time = 0;
    sequence = 1.0f;
    return pool;
}

extern __typeof__(initialize_level_render_state) func_001E9B10
    __attribute__((alias("FUN_001e9b10")));

s32 shrub_count = 0;
