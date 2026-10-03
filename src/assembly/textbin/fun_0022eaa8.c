#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022eaa8/FUN_0022eaa8.s", FUN_0022eaa8);
#else
#include "types.h"
#include "eetypes.h"
#include "sda.h"
#include "qcopy.h"

typedef union { u128 quadword; f32 components[4]; } Vector4;
typedef struct { Vector4 rows[4]; } Matrix4x4;

typedef struct {
    u8 pad00[0xC];
    u8 count;          /* 0x0C */
    u8 pad0D[0x24 - 0x0D];
    f32 base_scale;         /* 0x24 */
    u8 pad28[0x48 - 0x28];
    s32 frames[1];     /* 0x48 */
} RenderModel;

typedef struct {
    u8 pad00[0x10];
    Vector4 position;          /* 0x10 */
    RenderModel *model;      /* 0x20 */
} RenderObjectHeader;

typedef struct {
    u8 pad00[0x10];
    u8 position[0x14];      /* 0x10 */
    RenderModel *model;      /* 0x24 */
    u8 pad28[4];
    f32 fade;          /* 0x2C */
    u8 pad30[2];
    s16 selected_index;         /* 0x32 */
    u16 flags;         /* 0x34 */
    u8 pad36[0x50 - 0x36];
    u8 state_a;          /* 0x50 */
    u8 state_b;
    u8 selected_a;
    u8 selected_b;
    f32 blend;         /* 0x54 */
    u8 pad58[0x68 - 0x58];
    u8 *frame_data_a;         /* 0x68 */
    u8 *frame_data_b;         /* 0x6C */
    u8 pad70;
    u8 cached_selector;
    u8 opacity;
    u8 pad73[5];
    u8 *animation_positions;         /* 0x78 */
} LevelRenderObject;

typedef struct {
    u8 pad00[0x34];
    s32 time;          /* 0x34 */
    s32 frame;         /* 0x38 */
    s32 sequence_frame;         /* 0x3C */
    s16 end;           /* 0x40 */
    u8 pad42[2];
    s16 count;         /* 0x44 */
    u8 pad46[0x58 - 0x46];
    s32 source_begin;         /* 0x58 */
    s32 source_end;         /* 0x5C */
    s32 prepared_frames[0x46];   /* 0x60 */
    LevelRenderObject *objects[1];      /* 0x178 */
} LevelGameplayState;

typedef struct {
    u8 pad00[0x50];
    s32 history_index;           /* 0x50 */
    s32 history_count;           /* 0x54 */
    s32 mode;          /* 0x58 */
    s32 state;         /* 0x5C */
    u8 pad60[0xC0 - 0x60];
    Vector4 primary_history_positions[32];   /* 0xC0 */
    Vector4 secondary_history_positions[32];   /* 0x2C0 */
} LevelRenderState;

typedef struct {
    u8 pad00[0xB0];
    f32 projection_scale;
} LevelProjectionState;

typedef struct {
    s32 offset;
    s32 available;
} RenderArchiveEntry;

typedef struct {
    u8 pad00[4];
    s32 data_offset;          /* 0x04 */
    u8 pad08[0x50 - 0x08];
    s32 scene_offsets[5];        /* 0x50 */
} RenderArchiveTable;

typedef struct {
    u8 pad00[4];
    s32 source_begin_offset;
    s32 source_end_offset;
    u8 pad0C[8];
    RenderArchiveTable *archive_table;        /* 0x14 */
} LevelRenderArchive;

typedef struct {
    u32 w0;
    u32 w1;
} ColorPair;

extern u8 D_0013DD40[];
extern u8 D_0013DD43[];
extern LevelRenderState level_render_state __asm__("D_0013E030");
extern u8 D_0013E5C0[];
extern s32 D_0015ED5C;
extern s32 current_level_index __asm__("D_0015ED84");
extern s16 D_0015EE48 __attribute__((sda));
#define D_0015EE4A (*(s16 *)0x0015EE4A)
extern f32 sequence_fade __asm__("D_0015F43C") MACRO_ADDR;
extern s32 D_0015F618 MACRO_ADDR;
extern f32 D_00160404 MACRO_ADDR;
extern u8 D_00160460[] MACRO_ADDR;
extern s32 D_001604E0 __attribute__((sda));
extern s32 D_001604F0 __attribute__((sda));
extern s32 D_00160500 __attribute__((sda));
extern s32 D_00160F0C;
extern LevelGameplayState render_sequence __asm__("D_0018CB20");
extern LevelGameplayState D_0018CB20_b[] __asm__("D_0018CB20");
extern LevelGameplayState D_0018CB20_c[] __asm__("D_0018CB20");
extern LevelRenderObject *D_0018CC98[];
extern LevelProjectionState view_context __asm__("D_0018CD00");
extern LevelRenderArchive D_001940C0;
extern ColorPair D_001D9A30[];
extern u128 D_001D9AE0[];
extern f32 D_001D9B30[];
extern f32 D_001D9B48[];

extern void FillTransferWords(void *, s32, s32);
extern void ReadGlobalTableEntry(void);
extern void func_0012DC80(void);
extern void func_0012E308(s32, s32, s32, s32, s32, s32, s32, void *);
extern void func_0012EB00(void);
extern void func_001E9428(void);
extern void func_001E9430(void);
extern void update_view_context(void) __asm__("func_001F2D98");
extern s32 scale_ticks(s32) __asm__("func_001F96F8");
extern u32 func_001F98D0(void *, void *, s32);
extern void add_vectors(void *, void *, void *) __asm__("func_001F9A10");
extern void scale_vector(void *, void *, f32) __asm__("func_001F9A68");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void parse_space_scene_chunk(s32) __asm__("FUN_002049f0");
extern void func_0020C828(LevelRenderObject *);
extern void func_0020C880(LevelRenderObject *, s32);
extern void calculate_object_transform(LevelRenderObject *, s32, void *) __asm__("func_0020CCA8");
extern void func_0020DEF8(LevelRenderObject *);
extern void build_object_rotation_matrix(void) __asm__("func_0022DE10");
extern s32 rand(void);

void update_level_gameplay_frame(void) __asm__("FUN_0022eaa8");

void update_level_gameplay_frame(void) {
    Vector4 first_position;
    Vector4 second_position;
    Matrix4x4 first_transform;
    Matrix4x4 second_transform;
    LevelGameplayState *gameplay_state;
    LevelRenderState *render_state;
    u16 transform_count;
    LevelRenderObject *object;
    LevelRenderObject *expired_object;
    s32 mode_advance;
    RenderModel *model;
    RenderArchiveTable *archive_table;
    u8 *archive_data;
    s32 *scene_offset;
    RenderArchiveEntry *archive_entry;
    s32 object_index;
    s32 expired_object_index;
    s32 prepared_frame_index;
    s32 payload_offset;
    u8 *frame_offsets;
    f32 unused_fraction;
    s32 subframe;
    s32 unused_index;
    s32 color_index;
    s32 history_slot;
    s32 next_frame_payload;
    s32 current_frame_payload;
    s32 frame;
    u8 *animation_positions;
    s32 current_frame;
    s32 next_frame;
    f32 blend;
    f32 scale;

    func_001E9430();
    sequence_fade -= 0.25f;
    render_sequence.frame++;
    render_sequence.time++;
    if (sequence_fade < 0.0f) {
        sequence_fade = 0.0f;
    }
    if (render_sequence.time == 1) {
        if (current_level_index != 0 && (current_level_index != 1 || D_0013DD43[0] != 0)) {
            ReadGlobalTableEntry();
            func_0012E308(D_0015ED5C, level_render_state.mode, 0x400, 0, 0, 0, 0, D_0013E5C0);
            func_0012EB00();
            func_0012DC80();
        }
    }
    if (render_sequence.time >= render_sequence.end) {
        for (expired_object_index = 0; expired_object_index < render_sequence.count; expired_object_index++) {
            expired_object = render_sequence.objects[expired_object_index];
            if (expired_object != 0) {
                expired_object->model->count--;
                expired_object->model->frames[expired_object->model->count] = 0;
                func_0020C828(expired_object);
            }
        }
        if (current_level_index != 0 && (current_level_index != 1 || D_0013DD43[0] != 0) && D_0015EE48 < 3) {
            level_render_state.state = 0;
        }
        if (level_render_state.state < 2) {
            if (level_render_state.state == 0) {
                mode_advance = (rand() >> 16) % 3 + 1;
                level_render_state.mode = (level_render_state.mode + mode_advance) & 3;
            } else {
                level_render_state.mode = 4;
            }
            level_render_state.history_index = 0;
            level_render_state.history_count = 0;
            level_render_state.state++;
            qcopy(&D_001604F0, &D_001604E0);
            FillTransferWords(&render_sequence, 0, 0x1C0);
            render_sequence.source_begin = D_001940C0.source_begin_offset + D_00160F0C;
            render_sequence.source_end = D_001940C0.source_end_offset + D_00160F0C;
            archive_table = D_001940C0.archive_table;
            scene_offset = &archive_table->scene_offsets[0];
            scene_offset += level_render_state.mode;
            archive_data = (u8 *)archive_table + archive_table->data_offset;
            archive_entry = (RenderArchiveEntry *)(archive_data + *scene_offset);
            for (prepared_frame_index = 0; prepared_frame_index < 0x46 && archive_entry->available != 0; prepared_frame_index++, archive_entry++) {
                payload_offset = 0x800;
                render_sequence.prepared_frames[prepared_frame_index] = (s32)(*scene_offset + archive_data) + (archive_entry->offset + payload_offset);
            }
            parse_space_scene_chunk(0);
        } else {
            if (D_0015EE4A != 0) {
                D_0015EE4A = 0;
            }
            D_0015F618 = 1;
            return;
        }
    } else if (render_sequence.frame >= 0x60) {
        parse_space_scene_chunk(++render_sequence.sequence_frame);
    }
    build_object_rotation_matrix();
    if (view_context.projection_scale < D_001D9B48[level_render_state.mode]) {
        view_context.projection_scale = D_001D9B48[level_render_state.mode];
    }
    update_view_context();
    scale = 1.0f;
    if (level_render_state.mode == 4) {
        scale = (f32)(render_sequence.end - render_sequence.time) / (f32)render_sequence.end;
        scale_vector(&first_position, &D_00160500, scale);
        add_vectors(&D_001604F0, &D_001604F0, &first_position);
    }
    scale_vector(D_00160460, &D_001D9AE0[level_render_state.mode],
                  (f32)(render_sequence.time - scale_ticks(0x78)) * 20.0f * scale);
    D_00160404 = D_001D9B30[level_render_state.mode];
    for (object_index = 0; object_index < render_sequence.count; object_index++) {
        object = D_0018CC98[object_index];
        for (subframe = 0; subframe < 2; subframe++) {
            frame = render_sequence.frame;
            current_frame = frame >> 1;
            next_frame = current_frame + 1;
            model = object->model;
            frame_offsets = (u8 *)model->frames[model->count - 1] + 0x1C;
            current_frame_payload = *(s32 *)(frame_offsets + current_frame * 4) + 0x10;
            next_frame_payload = *(s32 *)(frame_offsets + next_frame * 4) + 0x10;
            blend = convert_integer_to_float(frame & 1) * 0.5f + (f32)subframe * 0.25f;
            object->blend = blend;
            animation_positions = object->animation_positions;
            scale_vector(&first_position, animation_positions + current_frame * 16, 1.0f - blend);
            scale_vector(&second_position, animation_positions + next_frame * 16, object->blend);
            transform_count = 2;
            add_vectors(object->position, &first_position, &second_position);
            object->selected_a = 2;
            object->selected_b = transform_count;
            object->state_a = 0;
            object->state_b = 0;
            func_0020C880(object, transform_count);
            func_001F98D0(object->frame_data_a + 0x10, (void *)current_frame_payload, 0x20);
            func_001F98D0(object->frame_data_b + 0x10, (void *)next_frame_payload, 0x20);
            object->selected_index = 0x1FF;
            object->opacity = 0xFF;
            object->cached_selector = 0xFF;
            func_0020DEF8(object);
            object->selected_a = object->model->count - 1;
            object->selected_b = object->model->count - 1;
            calculate_object_transform(object, 1, &first_transform);
            calculate_object_transform(object, 2, &second_transform);
            level_render_state.history_index = (level_render_state.history_index + 1) & 0x1F;
            if (level_render_state.history_count < 0x20) {
                level_render_state.history_count++;
            }
            history_slot = level_render_state.history_index;
            qcopy(&level_render_state.primary_history_positions[history_slot], &first_transform.rows[3]);
            qcopy(&level_render_state.secondary_history_positions[history_slot], &second_transform.rows[3]);
            if (level_render_state.mode == 4) {
                if (current_level_index == 0 || (current_level_index == 1 && D_0013DD40[3] == 0)) {
                    object->flags |= 1;
                    level_render_state.history_count = 0;
                }
                if (render_sequence.time > render_sequence.end - 0x38) {
                    object->fade = object->model->base_scale * ((f32)(render_sequence.end - render_sequence.time) * 0.017857144f);
                    for (color_index = 0; color_index < 3; color_index++) {
                        D_001D9A30[color_index].w0 = (D_001D9A30[color_index].w0 & 0xFFFFFF)
                            | ((D_0018CB20_b[0].end - D_0018CB20_c[0].time) << 24);
                    }
                } else {
                    for (color_index = 0; color_index < 3; color_index++) {
                        D_001D9A30[color_index].w0 = (D_001D9A30[color_index].w0 & 0xFFFFFF) | 0x38000000;
                    }
                }
            }
        }
    }
    func_001E9428();
}

extern __typeof__(update_level_gameplay_frame) func_0022EAA8 __attribute__((alias("FUN_0022eaa8")));

#endif /* NON_MATCHING */
