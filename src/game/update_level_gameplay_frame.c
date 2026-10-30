#include "types.h"
#include "rnc/globals.h"
#include "eetypes.h"
#include "sda.h"
#include "qcopy.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/rpc/snd_hold_command_batches.h"
#include "rnc/world/loaders/parse_space_scene_chunk.h"

typedef union {
    u128 quadword;
    f32  components[4];
} Vector4;

typedef struct {
    Vector4 rows[4];
} Matrix4x4;

typedef struct {
    u8  pad00[0xC];
    u8  count; /* 0x0C */
    u8  pad0D[0x24 - 0x0D];
    f32 base_scale; /* 0x24 */
    u8  pad28[0x48 - 0x28];
    s32 frames[1]; /* 0x48 */
} RenderModel;

typedef struct {
    u8           pad00[0x10];
    Vector4      position; /* 0x10 */
    RenderModel *model;    /* 0x20 */
} RenderObjectHeader;

typedef struct {
    u8           pad00[0x10];
    u8           position[0x14]; /* 0x10 */
    RenderModel *model;          /* 0x24 */
    u8           pad28[4];
    f32          fade; /* 0x2C */
    u8           pad30[2];
    s16          selected_index; /* 0x32 */
    u16          flags;          /* 0x34 */
    u8           pad36[0x50 - 0x36];
    u8           state_a; /* 0x50 */
    u8           state_b;
    u8           selected_a;
    u8           selected_b;
    f32          blend; /* 0x54 */
    u8           pad58[0x68 - 0x58];
    u8          *frame_data_a; /* 0x68 */
    u8          *frame_data_b; /* 0x6C */
    u8           pad70;
    u8           cached_selector;
    u8           opacity;
    u8           pad73[5];
    u8          *animation_positions; /* 0x78 */
} LevelRenderObject;

typedef struct {
    u8                 pad00[0x34];
    s32                time;           /* 0x34 */
    s32                frame;          /* 0x38 */
    s32                sequence_frame; /* 0x3C */
    s16                end;            /* 0x40 */
    u8                 pad42[2];
    s16                count; /* 0x44 */
    u8                 pad46[0x58 - 0x46];
    s32                source_begin;          /* 0x58 */
    s32                source_end;            /* 0x5C */
    s32                prepared_frames[0x46]; /* 0x60 */
    LevelRenderObject *objects[1];            /* 0x178 */
} LevelGameplayState;

typedef struct {
    u8      pad00[0x50];
    s32     history_index; /* 0x50 */
    s32     history_count; /* 0x54 */
    s32     mode;          /* 0x58 */
    s32     state;         /* 0x5C */
    u8      pad60[0xC0 - 0x60];
    Vector4 primary_history_positions[32];   /* 0xC0 */
    Vector4 secondary_history_positions[32]; /* 0x2C0 */
} LevelRenderState;

typedef struct {
    u8  pad00[0xB0];
    f32 projection_scale;
} LevelProjectionState;

typedef struct {
    s32 offset;
    s32 available;
} RenderArchiveEntry;

typedef struct {
    u8  pad00[4];
    s32 data_offset; /* 0x04 */
    u8  pad08[0x50 - 0x08];
    s32 scene_offsets[5]; /* 0x50 */
} RenderArchiveTable;

typedef struct {
    u8                  pad00[4];
    s32                 source_begin_offset;
    s32                 source_end_offset;
    u8                  pad0C[8];
    RenderArchiveTable *archive_table; /* 0x14 */
} LevelRenderArchive;

typedef struct {
    u32 w0;
    u32 w1;
} ColorPair;

#include "rnc/game/level_state.h"

extern LevelRenderState level_render_state __asm__("D_0013E030");
extern u8               voice_volume_records_macro[] __asm__("D_0013E5C0") MACRO_ADDR;
extern s32              sound_bank_handle __asm__("D_0015ED5C");
extern s16              D_0015EE48 __attribute__((sda));
extern s16              D_0015EE48_macro __asm__("D_0015EE48") MACRO_ADDR;
extern f32              sequence_macro __asm__("D_0015F43C") MACRO_ADDR;
extern s32              space_load_in_progress __asm__("D_0015F618") MACRO_ADDR;
extern f32 D_00160404   MACRO_ADDR;

#include "rnc/rendering/sky_shell.h"

extern s32 D_001604E0 __attribute__((sda));
extern s32 D_001604F0 __attribute__((sda));

#include "rnc/game/update_level_gameplay_frame_globals.h"

extern s32                mode4_render_vector_delta_gp __asm__("D_00160500") __attribute__((sda));
extern s32                current_vu_chain __asm__("D_00160F0C");
extern LevelGameplayState render_sequence __asm__("D_0018CB20");
/* Fakematch: a second name for render_sequence keeps the fade loop's own lui/addiu. */
extern LevelGameplayState   render_sequence_lui __asm__("D_0018CB20");
extern LevelRenderObject   *D_0018CC98[];
extern LevelProjectionState view_context __asm__("D_0018CD00");
extern LevelRenderArchive   D_001940C0;
extern ColorPair            warp_trail_colors[] __asm__("D_001D9A30");
extern u128                 level_render_motion_vectors_by_mode[] __asm__("D_001D9AE0");
extern f32                  level_render_depth_by_mode[] __asm__("D_001D9B30");
extern f32                  cam_mode_min_fov[] __asm__("D_001D9B48");

extern void fill_transfer_words(void *, s32, s32) __asm__("FUN_001f97e8");
extern void snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern void
snd_play_sound_vol_pan_pmpb(s32, s32, s32, s32, s32, s32, s32, void *) __asm__("func_0012E308");
extern void snd_reset_state_and_flush_commands(void) __asm__("func_0012EB00");
extern void noop_callback_h(void) __asm__("func_001E9428");
extern void noop_callback_i(void) __asm__("func_001E9430");
extern void update_view_context(void) __asm__("func_001F2D98");
extern s32  scale_game_frames(s32) __asm__("func_001F96F8");
extern u32  fast_mem_copy(void *, void *, s32) __asm__("func_001F98D0");
extern void fast_vec_add(void *, void *, void *) __asm__("func_001F9A10");
extern void scale_vector(void *, void *, f32) __asm__("func_001F9A68");
extern f32  convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void update_moby_animation_state(LevelRenderObject *) __asm__("func_0020C880");
extern void build_moby_bone_transform(LevelRenderObject *, s32, void *) __asm__("func_0020CCA8");
extern void refresh_moby_spatial_bounds(LevelRenderObject *) __asm__("func_0020DEF8");
extern void build_object_rotation_matrix(void) __asm__("func_0022DE10");
extern s32  rand(void);

void update_level_gameplay_frame(void) __asm__("FUN_0022eaa8");

void update_level_gameplay_frame(void) {
    Vector4             first_position;
    Vector4             second_position;
    Matrix4x4           first_transform;
    Matrix4x4           second_transform;
    u16                 transform_count;
    LevelRenderObject  *object;
    LevelRenderObject  *expired_object;
    s32                 mode_advance;
    RenderModel        *model;
    RenderArchiveTable *archive_table;
    u8                 *archive_data;
    RenderArchiveEntry *archive_entry;
    s32                 expired_object_index;
    s32                 prepared_frame_index;
    s32                 payload_offset;
    u8                 *frame_offsets;
    s32                *current_frame_pointer;
    s32                *next_frame_pointer;
    s32                 subframe;
    s32                 color_index;
    s32                 fill_index;
    s32                 history_slot;
    s32                 next_frame_payload;
    s32                 current_frame_payload;
    s32                 frame;
    u8                 *animation_positions;
    s32                 current_frame;
    s32                 next_frame;
    f32                 blend;
    f32                 scale;
    s32                 object_index;

    noop_callback_i();
    sequence_macro -= 0.25f;
    render_sequence.frame++;
    render_sequence.time++;
    if (sequence_macro < 0.0f) {
        sequence_macro = 0.0f;
    }
    if (render_sequence.time == 1 && current_level_index != 0 &&
        (current_level_index != 1 || level_available[3] != 0)) {
        snd_hold_command_batches();
        snd_play_sound_vol_pan_pmpb(sound_bank_handle,
                                    level_render_state.mode,
                                    0x400,
                                    0,
                                    0,
                                    0,
                                    0,
                                    voice_volume_records_macro);
        snd_reset_state_and_flush_commands();
        snd_flush_sound_commands();
    }
    if (render_sequence.time >= render_sequence.end) {
        for (expired_object_index = 0; expired_object_index < render_sequence.count;
             expired_object_index++) {
            expired_object = render_sequence.objects[expired_object_index];
            if (expired_object != 0) {
                expired_object->model->count--;
                expired_object->model->frames[expired_object->model->count] = 0;
                mark_moby_for_removal(expired_object);
            }
        }
        if (current_level_index != 0 && (current_level_index != 1 || level_available[3] != 0) &&
            D_0015EE48 < 3) {
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
            fill_transfer_words(&render_sequence, 0, 0x1C0);
            render_sequence.source_begin = D_001940C0.source_begin_offset + current_vu_chain;
            render_sequence.source_end = D_001940C0.source_end_offset + current_vu_chain;
            archive_table = D_001940C0.archive_table;
            archive_data = (u8 *)archive_table + archive_table->data_offset;
            archive_entry =
                (RenderArchiveEntry *)(archive_data +
                                       archive_table->scene_offsets[level_render_state.mode]);
            for (prepared_frame_index = 0;
                 prepared_frame_index < 0x46 && archive_entry->available != 0;
                 prepared_frame_index++, archive_entry++) {
                payload_offset = archive_entry->offset + 0x800;
                render_sequence.prepared_frames[prepared_frame_index] =
                    (s32)(archive_data + archive_table->scene_offsets[level_render_state.mode]) +
                    payload_offset;
            }
            parse_space_scene_chunk(0);
        } else {
            if ((&D_0015EE48_macro)[1] != 0) {
                (&D_0015EE48_macro)[1] = 0;
            }
            space_load_in_progress = 1;
            return;
        }
    } else if (render_sequence.frame >= 0x60) {
        parse_space_scene_chunk(++render_sequence.sequence_frame);
    }
    build_object_rotation_matrix();
    if (view_context.projection_scale < cam_mode_min_fov[level_render_state.mode]) {
        view_context.projection_scale = cam_mode_min_fov[level_render_state.mode];
    }
    update_view_context();
    scale = 1.0f;
    if (level_render_state.mode == 4) {
        scale = (f32)(render_sequence.end - render_sequence.time) / (f32)render_sequence.end;
        scale_vector(&first_position, &mode4_render_vector_delta_gp, scale);
        fast_vec_add(&D_001604F0, &D_001604F0, &first_position);
    }
    scale_vector(&D_00160460,
                 &level_render_motion_vectors_by_mode[level_render_state.mode],
                 (f32)(render_sequence.time - scale_game_frames(0x78)) * 20.0f * scale);
    D_00160404 = level_render_depth_by_mode[level_render_state.mode];
    for (object_index = 0; object_index < render_sequence.count; object_index++) {
        object = D_0018CC98[object_index];
        for (subframe = 0; subframe < 2; subframe++) {
            frame = render_sequence.frame;
            current_frame = frame >> 1;
            next_frame = current_frame + 1;
            model = object->model;
            frame_offsets = (u8 *)model->frames[model->count - 1] + 0x1C;
            current_frame_pointer = (s32 *)(frame_offsets + current_frame * 4);
            next_frame_pointer = (s32 *)(frame_offsets + next_frame * 4);
            current_frame_payload = *current_frame_pointer + 0x10;
            next_frame_payload = *next_frame_pointer + 0x10;
            blend = convert_integer_to_float(frame & 1) * 0.5f + (f32)subframe * 0.25f;
            object->blend = blend;
            animation_positions = object->animation_positions;
            scale_vector(&first_position, animation_positions + current_frame * 16, 1.0f - blend);
            scale_vector(&second_position, animation_positions + next_frame * 16, object->blend);
            /* Fakematch: the u16 copy gives the second 2 its own register. */
            transform_count = 2;
            fast_vec_add(object->position, &first_position, &second_position);
            object->selected_a = 2;
            object->selected_b = transform_count;
            object->state_a = 0;
            object->state_b = 0;
            update_moby_animation_state(object);
            fast_mem_copy(object->frame_data_a + 0x10, (void *)current_frame_payload, 0x20);
            fast_mem_copy(object->frame_data_b + 0x10, (void *)next_frame_payload, 0x20);
            object->opacity = 0xFF;
            object->selected_index = 0x1FF;
            object->cached_selector = 0xFF;
            refresh_moby_spatial_bounds(object);
            object->selected_a = object->model->count - 1;
            object->selected_b = object->model->count - 1;
            build_moby_bone_transform(object, 1, &first_transform);
            build_moby_bone_transform(object, 2, &second_transform);
            level_render_state.history_index = (level_render_state.history_index + 1) & 0x1F;
            if (level_render_state.history_count < 0x20) {
                level_render_state.history_count++;
            }
            history_slot = level_render_state.history_index;
            qcopy(&level_render_state.primary_history_positions[history_slot],
                  &first_transform.rows[3]);
            qcopy(&level_render_state.secondary_history_positions[history_slot],
                  &second_transform.rows[3]);
            if (level_render_state.mode == 4) {
                if (current_level_index == 0 ||
                    (current_level_index == 1 && level_available[3] == 0)) {
                    object->flags |= 1;
                    level_render_state.history_count = 0;
                }
                if (render_sequence.time > render_sequence.end - 0x38) {
                    object->fade =
                        object->model->base_scale *
                        ((f32)(render_sequence.end - render_sequence.time) * 0.017857144f);
                    for (color_index = 0; color_index < 3; color_index++) {
                        /* Fakematch: render_sequence_lui, see its declaration. */
                        warp_trail_colors[color_index].w0 =
                            (warp_trail_colors[color_index].w0 & 0xFFFFFF) |
                            ((render_sequence_lui.end - render_sequence.time) << 24);
                    }
                } else {
                    for (fill_index = 0; fill_index < 3; fill_index++) {
                        warp_trail_colors[fill_index].w0 =
                            (warp_trail_colors[fill_index].w0 & 0xFFFFFF) | 0x38000000;
                    }
                }
            }
        }
    }
    noop_callback_h();
}

extern __typeof__(update_level_gameplay_frame) func_0022EAA8 __attribute__((alias("FUN_0022eaa8")));
