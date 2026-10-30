#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "sda.h"
#include "qcopy.h"
#include "rnc/globals.h"
#include "rnc/hero/hero.h"
#include "rnc/rendering/level_render_state.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/runtime/dma/calculate_dma_transfer_address.h"
#include "rnc/world/loaders/parse_space_scene_chunk.h"
#include "rnc/audio/voices/pause_all_sounds.h"

typedef union {
    u128 quadword;
    f32  components[4];
    u8   bytes[16];
} Vector4;

typedef struct {
    u8  pad0[6];
    u8  unk6;
    u8  pad7[5];
    u8  count; /* 0x0C */
    u8  padD[0x48 - 0xD];
    s32 frames[1]; /* 0x48 */
} RenderModel;

typedef struct ResidentRenderObject {
    u8           pad0[0x10];
    Vector4      position; /* 0x10 */
    u8           state;    /* 0x20 */
    u8           pad21[3];
    RenderModel *model; /* 0x24 */
    u8           pad28[0xA];
    s16          selected_index; /* 0x32 */
    u16          flags;          /* 0x34 */
    u8           pad36[2];
    s64          lifetime_stamp; /* 0x38 */
    Vector4      rotation;       /* 0x40 */
    u8           current_frame;
    u8           next_frame;
    u8           selected_a;
    u8           selected_b;
    f32          blend; /* 0x54 */
    u8           pad58[0x71 - 0x58];
    u8           cached_selector;
    u8           opacity;
    u8           unk73;
    u8           pad74[4];
    u8          *animation_positions; /* 0x78 */
    u8           pad7C[3];
    u8           unk7F;
    u8           pad80[0x10];
    s32          color; /* 0x90 */
    s32          unk94; /* 0x94 */
    u8           pad98[0xE];
    s16          class_id; /* 0xA6 */
    u8           padA8[0xA];
    s16          unkB2; /* 0xB2 */
    u8           padB4[8];
    u8           unkBC; /* 0xBC */
    u8           padBD[3];
    Vector4      transform[4]; /* 0xC0 */
} ResidentRenderObject;

typedef struct {
    Vector4 position;
    f32     rotation[4];
} CameraKeyframe;

typedef struct {
    u8                    pad0[0x34];
    s32                   time;           /* 0x34 */
    s32                   frame;          /* 0x38 */
    s32                   sequence_frame; /* 0x3C */
    s16                   end;            /* 0x40 */
    u8                    pad42[2];
    s16                   count; /* 0x44 */
    u8                    pad46[0x54 - 0x46];
    CameraKeyframe       *frames; /* 0x54 */
    u8                    pad58[0x178 - 0x58];
    ResidentRenderObject *objects[1]; /* 0x178 */
} ResidentPlaybackState;

typedef struct {
    s32     point_count;
    s32     pad[3];
    Vector4 p[1]; /* 0x10: x, y, z, angle */
} ScriptedPath;

typedef struct {
    u8      pad0[0x30];
    Vector4 position; /* 0x30 */
    u8      pad40[0x34];
    f32     rotY; /* 0x74 */
    f32     rotZ; /* 0x78 */
    u8      pad7C[4];
} CameraBlendNode;

typedef struct {
    Vector4 a;
    Vector4 b;
} PositionPair;

#include "rnc/input/pad_state.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/music/music_stream_state.h"

extern u8                    D_001413F5[];
extern u8                    D_0015EDB4;
extern f32                   sequence __asm__("D_0015F43C");
extern f32                   D_0015F440;
extern s32                   space_load_pending __asm__("D_0015F5B0");
extern s32                   space_load_in_progress[1] __asm__("D_0015F618");
extern ResidentRenderObject *moby_instance_end __asm__("D_0015FF1C");
extern CameraBlendNode      *D_00160034;
extern s32 playback_skip_target_frames[1] __asm__("D_00160510") __attribute__((sda));

#include "rnc/ui/menus/menu_globals.h"

extern ResidentPlaybackState render_sequence __asm__("D_0018CB20");
extern f32                   view_tan_half_fov[4] __asm__("D_0018CDB0");
extern ScriptedPath         *resident_scripted_path_table[] __asm__("D_001CC3B0");
extern PositionPair          resident_path_history_local_offset_pairs[] __asm__("D_001D99B0");
extern Vector4               resident_path_companion_a_local_offsets[] __asm__("D_001D9C80");
extern Vector4               resident_path_companion_b_local_offsets[] __asm__("D_001D9CB0");
extern void                  build_variant_effect_quads() __asm__("FUN_0022e1b0");

extern void music_pause(s32);
extern void music_unpause(void);
extern void sceVu0UnitMatrix(void *);
extern void SceVu0RotMatrixX(void *, void *, f32);
extern void SceVu0RotMatrixY(void *, void *, f32);
extern void sceVu0RotMatrixZ(void *, void *, f32);
extern s32  sceGsSyncV(s32);
extern void noop_callback_a(void *, void *, s32, s32, s32) __asm__("func_001E93F0");
extern void noop_callback_b(void *) __asm__("func_001E93F8");
extern void noop_callback_c(void *) __asm__("func_001E9400");
extern void noop_callback_d(void) __asm__("func_001E9408");
extern void noop_callback_e(ResidentRenderObject *) __asm__("func_001E9410");
extern void noop_callback_f(ResidentRenderObject *) __asm__("func_001E9418");
extern void noop_callback_g(void) __asm__("func_001E9420");
extern void noop_callback_h(void) __asm__("func_001E9428");
extern void noop_callback_i(void) __asm__("func_001E9430");
extern void noop_callback_j(void) __asm__("func_001E9438");
extern void noop_callback_k(void *, void *, s32, s32) __asm__("func_001E9440");
extern void noop_callback_m(ResidentRenderObject *,
                            ResidentRenderObject *) __asm__("func_001E9450");
extern void update_camera(void) __asm__("func_001EDAA8");
extern void update_view_context(void) __asm__("func_001F2D98");
extern void enqueue_callback_list_4(void *, ResidentRenderObject *) __asm__("FUN_001f47b8");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern f32  multiply_global_scale(f32) __asm__("func_001F96E8");
extern s32  scale_game_frames(s32) __asm__("func_001F96F8");
extern f32  absolute_float(f32) __asm__("func_001F99C0");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void fast_vec_add(void *, void *, void *) __asm__("func_001F9A10");
extern void lerp_vector(void *, void *, void *, f32) __asm__("func_001F9A40");
extern void scale_vector(void *, void *, f32) __asm__("func_001F9A68");
extern void fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
extern f32  fast_vec_dist(void *, void *) __asm__("func_001F9B48");
extern f32  vector_distance(void *, void *, void *) __asm__("func_001F9B80");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern f32  fast_cos(f32) __asm__("func_001F9DC8");
extern f32  fast_arc_tan(f32, f32) __asm__("func_001F9E90");
extern f32  fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32  fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32  convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void update_all_point_lights(void) __asm__("func_00201A28");
extern void update_moby_animation_state(ResidentRenderObject *) __asm__("func_0020C880");
extern void update_resident_packed_direction(void) __asm__("func_0020CFD0");
extern void refresh_moby_spatial_bounds(ResidentRenderObject *) __asm__("func_0020DEF8");
extern void update_visible_resident_objects(void) __asm__("func_00212E28");
extern void update_moby_shadow_range(ResidentRenderObject *) __asm__("func_00213700");
extern f32  advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern void
sample_camera_path(ScriptedPath *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
extern void continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");
extern void update_audio_stream_until_idle(s32) __asm__("FUN_002168a8");
extern void update_parts(void) __asm__("func_00217B88");
extern void sound_update(void) __asm__("func_0022CA50");
extern void enqueue_voice_request(void) __asm__("func_0022DC50");
extern void update_level_gameplay_frame(void) __asm__("func_0022EAA8");
extern void emit_six_spark_effects(ResidentRenderObject *, f32) __asm__("func_0022F5B0");
extern void draw_light_flare() __asm__("func_0022DF40");
extern void build_resident_indexed_texture_warp_meshes() __asm__("func_0022E420");
extern void render_environment_mapped_object() __asm__("func_002327A0");

void update_resident_gameplay_state(void) __asm__("FUN_0022f778");

extern f32 frame_time_sq_gp __asm__("D_0015ED70") __attribute__((sda));

void update_resident_gameplay_state(void) {
    enum {
        LEVEL_RENDER_STATE_PLAYBACK_TO_MENU = 0,
        LEVEL_RENDER_STATE_SCRIPTED_CAMERA_AND_PATH = 3,
        LEVEL_RENDER_STATE_GAMEPLAY = 4,
        LEVEL_RENDER_STATE_PLAYBACK_WITH_MUSIC_RESUME = 8
    };
    Vector4               scratch_vectors[4];
    s32                   keyframe_flags;
    s32                   ticks_per_bank;
    s32                   skip;
    s32                   object_index;
    s32                   point_index;
    ResidentRenderObject *object;
    ResidentRenderObject *object_cursor;
    ResidentRenderObject *expired_object;
    s32                   release_index;
    s32                   bank_tick_limit;
    ResidentRenderObject *player;
    s32                   path_point_index;
    f32                   camera_blend_step;
    CameraKeyframe       *keyframe;
    f32                  *keyframe_angles;
    u8                   *animation_positions;
    s32                   current_frame;
    ScriptedPath         *path;
    f32                   segment_angle;
    f32                   maximum_turn;
    f32                   turn_scale;
    f32                   blend;

    level_render_state.timer++;
    switch (level_render_state.state) {
    case LEVEL_RENDER_STATE_PLAYBACK_TO_MENU:
    case LEVEL_RENDER_STATE_PLAYBACK_WITH_MUSIC_RESUME:
        noop_callback_i();
        update_visible_resident_objects();
        enqueue_voice_request();
        noop_callback_g();
        update_parts();
        sequence -= 0.125f;
        render_sequence.frame++;
        render_sequence.time++;
        if (sequence < 0.0f) {
            sequence = 0.0f;
        }
        ticks_per_bank = pal_mode ? 0x50 : 0x60;
        skip = level_render_state.skip;
        if ((controller_state.pressed & (PAD_TRIANGLE | PAD_CROSS)) &&
            scale_game_frames(30) < render_sequence.time && sequence == 0.0f) {
            if (level_render_state.state == 0 &&
                render_sequence.time <
                    scale_game_frames(
                        playback_skip_target_frames[level_render_state.content_variant] - 30)) {
                skip = 1;
            }
            if (level_render_state.state == 8 &&
                render_sequence.time < render_sequence.end - scale_game_frames(30)) {
                skip = 1;
            }
        }
        if (skip) {
            if (music_stream_state.secondary.state != 6 &&
                music_stream_state.secondary.state != 7) {
                music_stream_state.secondary.state = 5;
            }
            if (level_render_state.state == 0) {
                render_sequence.time = scale_game_frames(
                    playback_skip_target_frames[level_render_state.content_variant]);
                render_sequence.sequence_frame = render_sequence.time / ticks_per_bank;
                parse_space_scene_chunk(render_sequence.sequence_frame);
                render_sequence.frame = render_sequence.time % ticks_per_bank;
                if (level_render_state.skip) {
                    level_render_state.skip = 0;
                } else {
                    fade_to_black(4);
                }
                sequence = 1.0f;
            } else {
                render_sequence.time = render_sequence.end;
            }
        }
        if (render_sequence.time >= render_sequence.end) {
            if (music_stream_state.secondary.state != 6 &&
                music_stream_state.secondary.state != 7) {
                music_stream_state.secondary.state = 5;
            }
            calculate_dma_transfer_address();
            view_tan_half_fov[0] = 0.63f;
            update_view_context();
            /* Retail releases the attachment once per sequence object, even when that object is null. */
            for (release_index = 0; release_index < render_sequence.count; release_index++) {
                expired_object = render_sequence.objects[release_index];
                if (expired_object != 0) {
                    expired_object->model->count--;
                    expired_object->model->frames[expired_object->model->count] = 0;
                    mark_moby_for_removal(expired_object);
                }
                if (level_render_state.attachment != 0) {
                    mark_moby_for_removal(level_render_state.attachment);
                }
            }
            level_render_state.player->flags &= ~1;
            for (object_cursor = moby_instance_end; object_cursor->state != 0xFF; object_cursor++) {
                if (!(object_cursor->state & 0x80) &&
                    (object_cursor->class_id == 0x4A || object_cursor->class_id == 0xCB)) {
                    object_cursor->flags &= ~0x80;
                }
            }
            if (level_render_state.state == 0) {
                pause_all_sounds(0);
                menu_system.state = 0xE;
                space_load_in_progress[0] = 1;
                return;
            }
            game_mode = 0;
            music_unpause();
            space_load_in_progress[0] = 1;
            D_001413F5[0] = 0;
            noop_callback_j();
            if (level_render_state.unk2C != 0) {
                noop_callback_d();
                return;
            }
            noop_callback_k(&level_render_state.unk60, &level_render_state.unk70, 0, 1);
            return;
        }
        bank_tick_limit = pal_mode ? 0x50 : 0x60;
        if (render_sequence.frame >= bank_tick_limit) {
            parse_space_scene_chunk(++render_sequence.sequence_frame);
        }
        keyframe = &render_sequence.frames[render_sequence.frame];
        keyframe_angles = keyframe->rotation;
        keyframe_flags = keyframe->position.bytes[12];
        view_tan_half_fov[0] = keyframe_angles[3];
        update_view_context();
        qcopy(&preview_camera.position, &keyframe->position);
        transform_vector_by_basis(&preview_camera.position,
                                  &preview_camera.position,
                                  level_render_state.player->transform);
        fast_vec_add(&preview_camera.position,
                     &preview_camera.position,
                     &level_render_state.player->position);
        segment_angle = fast_add_rotations(keyframe_angles[2],
                                           level_render_state.player->rotation.components[2]);
        sceVu0UnitMatrix(scratch_vectors);
        SceVu0RotMatrixX(scratch_vectors, scratch_vectors, keyframe->rotation[0]);
        SceVu0RotMatrixY(scratch_vectors, scratch_vectors, keyframe_angles[1]);
        sceVu0RotMatrixZ(scratch_vectors, scratch_vectors, segment_angle);
        preview_camera.forward.f[0] = -scratch_vectors[2].components[0];
        preview_camera.right.f[0] = -scratch_vectors[0].components[0];
        preview_camera.up.f[0] = scratch_vectors[1].components[0];
        preview_camera.forward.f[1] = -scratch_vectors[2].components[1];
        preview_camera.right.f[1] = -scratch_vectors[0].components[1];
        preview_camera.up.f[1] = scratch_vectors[1].components[1];
        preview_camera.forward.f[2] = -scratch_vectors[2].components[2];
        preview_camera.right.f[2] = -scratch_vectors[0].components[2];
        preview_camera.up.f[2] = scratch_vectors[1].components[2];
        if (D_0015EDB4 != 0) {
            fast_vec_cross(&preview_camera.right, &preview_camera.up, &preview_camera.forward);
        }
        for (object_index = 0; object_index < render_sequence.count; object_index++) {
            object = render_sequence.objects[object_index];
            current_frame = render_sequence.frame >> 1;
            object->current_frame = current_frame;
            object->next_frame = current_frame + 1;
            update_moby_animation_state(object);
            object->blend = convert_integer_to_float(render_sequence.frame & 1) * 0.5f;
            if (keyframe_flags != 0 && (render_sequence.frame & 1)) {
                object->blend = 1.0f;
            }
            animation_positions = object->animation_positions;
            scale_vector(&scratch_vectors[0],
                         animation_positions + object->current_frame * 16,
                         1.0f - object->blend);
            scale_vector(
                &scratch_vectors[1], animation_positions + object->next_frame * 16, object->blend);
            fast_vec_add(&object->position, &scratch_vectors[0], &scratch_vectors[1]);
            transform_vector_by_basis(
                &object->position, &object->position, level_render_state.player->transform);
            fast_vec_add(
                &object->position, &object->position, &level_render_state.player->position);
            object->rotation.components[2] = level_render_state.player->rotation.components[2];
            object->cached_selector = 0xFF;
            refresh_moby_spatial_bounds(object);
            if (object->class_id == 0) {
                if (level_render_state.state == 0) {
                    if (current_level_index == 10 && item_available[6] == 0) {
                        object->unk7F = 0;
                    } else if (render_sequence.time <= scale_game_frames(0x3E)) {
                        object->unk7F = 0x18;
                    } else {
                        object->unk7F = 0;
                    }
                } else if (scale_game_frames(360) < render_sequence.time) {
                    if (current_level_index == 10 && item_available[6] == 0) {
                        object->unk7F = 0;
                    } else {
                        object->unk7F = 0x18;
                    }
                } else {
                    object->unk7F = 0;
                }
            } else if (object->class_id == 10) {
                if (level_render_state.state == 0) {
                    if (render_sequence.time <= scale_game_frames(350)) {
                        object->unk7F = 0x18;
                    } else {
                        object->unk7F = 0;
                    }
                } else if (render_sequence.time <= scale_game_frames(524)) {
                    object->unk7F = 0;
                } else {
                    object->unk7F = 0x18;
                }
            }
            if (object->unk7F != 0) {
                update_moby_shadow_range(object);
            }
            if (object->class_id == 10) {
                noop_callback_f(object);
            }
            if (object->class_id == 0) {
                noop_callback_e(object);
                if ((current_level_index == 10 && item_available[6] != 0) ||
                    current_level_index == 13) {
                    if (level_render_state.attachment == 0) {
                        level_render_state.attachment = create_moby(0x509);
                        level_render_state.attachment->selected_index = 0x40;
                        level_render_state.attachment->flags |= 0x806;
                        level_render_state.attachment->lifetime_stamp = object->lifetime_stamp;
                        if (level_render_state.attachment->model->unk6 != 0) {
                            level_render_state.attachment->unk73 = 0x18;
                        }
                    }
                    noop_callback_m(object, level_render_state.attachment);
                }
            }
            if ((u16)object->class_id - 0x213U < 3) {
                enqueue_callback_list_4(render_environment_mapped_object, object);
                add_draw_callback(draw_light_flare, object);
                if (level_render_state.state == 0) {
                    if (render_sequence.time < scale_game_frames(360)) {
                        s32 color_intensity =
                            (s32)(fast_cos(
                                      convert_integer_to_float((render_sequence.time & 0x3F) - 32) *
                                      0.09817477f) *
                                  80.0f) +
                            120;
                        if (object->class_id == 0x215) {
                            object->color = (color_intensity >> 1) | (color_intensity << 8) |
                                            (color_intensity << 16);
                        } else {
                            object->color =
                                color_intensity | (color_intensity << 8) | (color_intensity << 16);
                        }
                    } else if (render_sequence.time < scale_game_frames(0x1F8)) {
                        s32 color_intensity =
                            (s32)(((f32)render_sequence.time - multiply_global_scale(360.0f)) *
                                  (frame_scale * 1.25f));
                        if (color_intensity >= 256) {
                            color_intensity = 255;
                        }
                        if (object->class_id == 0x215) {
                            object->color = (color_intensity << 8) | (color_intensity << 16);
                        } else {
                            object->color =
                                color_intensity | (color_intensity << 8) | (color_intensity << 16);
                        }
                        if (scale_game_frames(0x1A4) < render_sequence.time) {
                            level_render_state.player->unkBC =
                                (s32)(((f32)render_sequence.time - multiply_global_scale(420.0f)) *
                                      (frame_scale * 1.2f));
                            level_render_state.player->unkB2 = 0;
                            add_draw_callback(build_variant_effect_quads,
                                              level_render_state.player);
                        }
                        if (scale_game_frames(0x1D0) < render_sequence.time) {
                            D_0015F440 += 0.025f;
                            if (D_0015F440 > 1.0f) {
                                D_0015F440 = 1.0f;
                            }
                        }
                    } else {
                        object->color = 0xA0A0A0;
                        D_0015F440 -= 0.025f;
                        if (D_0015F440 < 0.0f) {
                            D_0015F440 = 0.0f;
                        }
                    }
                } else if (render_sequence.time < scale_game_frames(0xF0)) {
                    s32 color_intensity;
                    if (render_sequence.time < scale_game_frames(0xC8)) {
                        emit_six_spark_effects(object, frame_time * -4.0f);
                        emit_six_spark_effects(object, frame_time * -3.0f);
                    }
                    color_intensity = (s32)(fast_cos(convert_integer_to_float(
                                                         (render_sequence.time & 0x3F) - 32) *
                                                     0.09817477f) *
                                            80.0f) +
                                      120;
                    if (object->class_id == 0x215) {
                        object->color = (color_intensity >> 1) | (color_intensity << 8) |
                                        (color_intensity << 16);
                    } else {
                        object->color =
                            color_intensity | (color_intensity << 8) | (color_intensity << 16);
                    }
                    object->unkBC = 0x32;
                    object->unkB2 = 10;
                    add_draw_callback(build_variant_effect_quads, object);
                } else if (render_sequence.time <= scale_game_frames(360)) {
                    s32 color_intensity;
                    if (render_sequence.time < scale_game_frames(300)) {
                        level_render_state.player->unkBC =
                            (s32)((multiply_global_scale(300.0f) - (f32)render_sequence.time) *
                                  (frame_scale * 1.5f));
                        level_render_state.player->unkB2 = 0;
                        add_draw_callback(build_variant_effect_quads, level_render_state.player);
                    }
                    color_intensity =
                        (s32)((multiply_global_scale(360.0f) - (f32)render_sequence.time) *
                              (frame_scale * 1.25f));
                    if (object->class_id == 0x215) {
                        object->color = (color_intensity << 8) | (color_intensity << 16);
                    } else {
                        object->color =
                            color_intensity | (color_intensity << 8) | (color_intensity << 16);
                    }
                } else {
                    s32 color_intensity = (s32)(fast_cos(convert_integer_to_float(
                                                             (render_sequence.time & 0x3F) - 32) *
                                                         0.09817477f) *
                                                80.0f) +
                                          120;
                    if (object->class_id == 0x215) {
                        object->color = (color_intensity >> 1) | (color_intensity << 8) |
                                        (color_intensity << 16);
                    } else {
                        object->color =
                            color_intensity | (color_intensity << 8) | (color_intensity << 16);
                    }
                }
            }
        }
        sound_update();
        update_all_point_lights();
        update_resident_packed_direction();
        noop_callback_h();
        break;

    case LEVEL_RENDER_STATE_SCRIPTED_CAMERA_AND_PATH:
        noop_callback_i();
        update_visible_resident_objects();
        enqueue_voice_request();
        noop_callback_g();
        update_parts();
        if (level_render_state.timer == 0) {
            music_pause(0);
            update_audio_stream_until_idle(0);
            music_stream_state.queued_secondary_track = level_render_state.content_variant + 0x9C4F;
            fade_to_black(scale_game_frames(12));
            while ((s16)music_stream_state.secondary.state != 3) {
                sound_update();
                sceGsSyncV(0);
            }
            continue_audio_stream_if_ready();
            level_render_state.player->selected_a = 1;
            level_render_state.player->selected_b = 2;
            level_render_state.player->blend = 0.0f;
            update_moby_animation_state(level_render_state.player);
            level_render_state.player->opacity = 0xFF;
            level_render_state.player->unk94 = 0;
            if (level_render_state.path >= 0) {
                level_render_state.companion_a = create_moby(0);
                level_render_state.companion_a->selected_index = 0x1FF;
                level_render_state.companion_a->opacity = 0xFF;
                level_render_state.companion_a->unk94 = 0;
                level_render_state.companion_a->flags |= 6;
                level_render_state.companion_a->lifetime_stamp =
                    ((ResidentRenderObject *)hero.moby)->lifetime_stamp;
                level_render_state.companion_b = create_moby(10);
                level_render_state.companion_b->selected_index = 0x1FF;
                level_render_state.companion_b->opacity = 0xFF;
                level_render_state.companion_b->unk94 = 0;
                level_render_state.companion_b->flags |= 6;
                level_render_state.companion_b->lifetime_stamp =
                    ((ResidentRenderObject *)hero.moby)->lifetime_stamp;
            }
            noop_callback_a(&preview_camera.position, &preview_camera.rotation, 1, 0, 0);
            level_render_state.blend = 0.0f;
            level_render_state.interpolation_velocity = 0.0f;
            if (level_render_state.path >= 0) {
                path = resident_scripted_path_table[level_render_state.path];
                maximum_turn = level_render_state.blend;
                path->p[0].components[3] = maximum_turn;
                for (path_point_index = 1; path_point_index < path->point_count - 1;
                     path_point_index++) {
                    segment_angle = fast_arc_tan(path->p[path_point_index].components[0] -
                                                     path->p[path_point_index - 1].components[0],
                                                 path->p[path_point_index].components[1] -
                                                     path->p[path_point_index - 1].components[1]);
                    path->p[path_point_index].components[3] = fast_subtract_rotations(
                        fast_arc_tan(path->p[path_point_index + 1].components[0] -
                                         path->p[path_point_index].components[0],
                                     path->p[path_point_index + 1].components[1] -
                                         path->p[path_point_index].components[1]),
                        segment_angle);
                    if (maximum_turn < absolute_float(path->p[path_point_index].components[3])) {
                        maximum_turn = absolute_float(path->p[path_point_index].components[3]);
                    }
                }
                segment_angle = fast_arc_tan(path->p[path->point_count - 1].components[0] -
                                                 path->p[path->point_count - 2].components[0],
                                             path->p[path->point_count - 1].components[1] -
                                                 path->p[path->point_count - 2].components[1]);
                path->p[path->point_count - 1].components[3] = fast_subtract_rotations(
                    fast_arc_tan(
                        path->p[0].components[0] - path->p[path->point_count - 1].components[0],
                        path->p[0].components[1] - path->p[path->point_count - 1].components[1]),
                    segment_angle);
                turn_scale = 0.34906584f / maximum_turn;
                for (point_index = 0; point_index < path->point_count; point_index++) {
                    path->p[point_index].components[3] *= turn_scale;
                }
                level_render_state.path_segment_length = fast_vec_dist(&path->p[0], &path->p[1]);
                level_render_state.path_progress = 0.0f;
                level_render_state.speed = 0.0f;
                level_render_state.history_index = 0;
                level_render_state.history_count = 0;
                player = level_render_state.player;
                qcopy(&level_render_state.startPos, &player->position);
                qcopy(&level_render_state.pathPos, &path->p[0]);
                qcopy(&level_render_state.startRot, &player->rotation);
                level_render_state.rotY = fast_arc_tan(
                    vector_distance(&path->p[1], &path->p[0], &level_render_state.startPos),
                    path->p[0].components[2] - path->p[1].components[2]);
                level_render_state.rotZ =
                    fast_arc_tan(path->p[1].components[0] - path->p[0].components[0],
                                 path->p[1].components[1] - path->p[0].components[1]);
            }
        }
        if (level_render_state.source_camera_index >= 0 &&
            level_render_state.destination_camera_index >= 0) {
            camera_blend_step = frame_time_sq_gp * 0.666f;
            advance_accelerated_scalar(&level_render_state.blend,
                                       &level_render_state.interpolation_velocity,
                                       1.0f,
                                       camera_blend_step,
                                       camera_blend_step,
                                       frame_time * 0.5f);
            lerp_vector(&scratch_vectors[0],
                        &D_00160034[level_render_state.source_camera_index].position,
                        &D_00160034[level_render_state.destination_camera_index].position,
                        level_render_state.blend);
            clear_u64_value(&scratch_vectors[1]);
            scratch_vectors[1].components[1] =
                fast_subtract_rotations(
                    D_00160034[level_render_state.destination_camera_index].rotY,
                    D_00160034[level_render_state.source_camera_index].rotY) *
                level_render_state.blend;
            scratch_vectors[1].components[1] =
                fast_add_rotations(D_00160034[level_render_state.source_camera_index].rotY,
                                   scratch_vectors[1].components[1]);
            scratch_vectors[1].components[2] =
                fast_subtract_rotations(
                    D_00160034[level_render_state.destination_camera_index].rotZ,
                    D_00160034[level_render_state.source_camera_index].rotZ) *
                level_render_state.blend;
            scratch_vectors[1].components[2] =
                fast_add_rotations(D_00160034[level_render_state.source_camera_index].rotZ,
                                   scratch_vectors[1].components[2]);
            noop_callback_b(&scratch_vectors[0]);
            noop_callback_c(&scratch_vectors[1]);
        }
        if (level_render_state.path >= 0) {
            if (level_render_state.timer < scale_game_frames(150)) {
                blend =
                    (1.0f -
                     fast_cos(convert_integer_to_float(level_render_state.timer) *
                              (3.1415927f / convert_integer_to_float(scale_game_frames(120))))) *
                    0.5f;
                if (level_render_state.timer >= scale_game_frames(120)) {
                    blend = 1.0f;
                }
                level_render_state.player->blend = blend;
                lerp_vector(&level_render_state.player->position,
                            &level_render_state.startPos,
                            &level_render_state.pathPos,
                            blend);
                level_render_state.player->rotation.components[0] = 0.0f;
                level_render_state.player->rotation.components[1] =
                    fast_add_rotations(level_render_state.startRot.f[1],
                                       fast_subtract_rotations(level_render_state.rotY,
                                                               level_render_state.startRot.f[1]) *
                                           blend);
                level_render_state.player->rotation.components[2] =
                    fast_add_rotations(level_render_state.startRot.f[2],
                                       fast_subtract_rotations(level_render_state.rotZ,
                                                               level_render_state.startRot.f[2]) *
                                           blend);
                if (sequence > 0.0f) {
                    sequence -= 0.125f;
                    if (sequence < 0.0f) {
                        sequence = 0.0f;
                    }
                }
                if (blend < 1.0f) {
                    emit_six_spark_effects(level_render_state.player, frame_time * -3.75f);
                }
            } else {
                ScriptedPath *active_path;
                level_render_state.speed += 0.8f;
                active_path = resident_scripted_path_table[level_render_state.path];
                if (level_render_state.speed > 100.0f) {
                    level_render_state.speed = 100.0f;
                }
                if ((controller_state.pressed & (PAD_TRIANGLE | PAD_CROSS)) && sequence < 0.0625f) {
                    sequence = 0.0625f;
                }
                if ((f32)(active_path->point_count - 6) < level_render_state.path_progress &&
                    sequence < 0.0625f) {
                    sequence = 0.0625f;
                }
                if (level_render_state.content_variant < 2) {
                    level_render_state.player->unkBC = 0x32;
                    level_render_state.player->unkB2 = 10;
                    add_draw_callback(build_variant_effect_quads, level_render_state.player);
                }
                level_render_state.path_progress +=
                    level_render_state.speed * frame_time / level_render_state.path_segment_length;
                if ((f32)(active_path->point_count - 1) < level_render_state.path_progress ||
                    sequence >= 1.0f) {
                    space_load_pending = 1;
                    space_load_in_progress[0] = 1;
                } else {
                    sample_camera_path(active_path,
                                       1,
                                       &level_render_state.player->position,
                                       &level_render_state.player->rotation,
                                       0,
                                       level_render_state.path_progress);
                    level_render_state.player->rotation.components[0] =
                        level_render_state.player->rotation.components[3];
                    level_render_state.history_index =
                        (level_render_state.history_index + 1) & 0x1F;
                    if (level_render_state.history_count < 0x20) {
                        level_render_state.history_count++;
                    }
                    transform_vector_by_basis(
                        &level_render_state.primary_history[level_render_state.history_index],
                        &resident_path_history_local_offset_pairs[level_render_state
                                                                      .content_variant]
                             .a,
                        level_render_state.player->transform);
                    fast_vec_add(
                        &level_render_state.primary_history[level_render_state.history_index],
                        &level_render_state.primary_history[level_render_state.history_index],
                        &level_render_state.player->position);
                    level_render_state.primary_history[level_render_state.history_index].f[3] =
                        1.0f;
                    transform_vector_by_basis(
                        &level_render_state.secondary_history[level_render_state.history_index],
                        &resident_path_history_local_offset_pairs[level_render_state
                                                                      .content_variant]
                             .b,
                        level_render_state.player->transform);
                    fast_vec_add(
                        &level_render_state.secondary_history[level_render_state.history_index],
                        &level_render_state.secondary_history[level_render_state.history_index],
                        &level_render_state.player->position);
                    level_render_state.secondary_history[level_render_state.history_index].f[3] =
                        1.0f;
                    add_draw_callback(build_resident_indexed_texture_warp_meshes,
                                      level_render_state.player);
                    if (sequence > 0.0f) {
                        sequence += 0.0625f;
                        if (sequence > 1.0f) {
                            sequence = 1.0f;
                        }
                    }
                }
            }
            {
                ResidentRenderObject *p = level_render_state.player;
                ResidentRenderObject *scratch_vectors = level_render_state.companion_a;
                qcopy(&scratch_vectors->rotation, &p->rotation);
                transform_vector_by_basis(
                    &scratch_vectors->position,
                    &resident_path_companion_a_local_offsets[level_render_state.content_variant],
                    p->transform);
            }
            fast_vec_add(&level_render_state.companion_a->position,
                         &level_render_state.companion_a->position,
                         &level_render_state.player->position);
            refresh_moby_spatial_bounds(level_render_state.companion_a);
            {
                ResidentRenderObject *scratch_vectors = level_render_state.companion_b;
                ResidentRenderObject *p = level_render_state.player;
                qcopy(&scratch_vectors->rotation, &p->rotation);
                transform_vector_by_basis(
                    &scratch_vectors->position,
                    &resident_path_companion_b_local_offsets[level_render_state.content_variant],
                    p->transform);
            }
            fast_vec_add(&level_render_state.companion_b->position,
                         &level_render_state.companion_b->position,
                         &level_render_state.player->position);
            refresh_moby_spatial_bounds(level_render_state.companion_b);
        }
        update_camera();
        sound_update();
        update_all_point_lights();
        update_resident_packed_direction();
        noop_callback_h();
        break;

    case LEVEL_RENDER_STATE_GAMEPLAY:
        update_level_gameplay_frame();
        break;
    case 1:
    case 2:
    case 5:
    case 6:
    case 7:
        break;
    }
}

extern __typeof__(update_resident_gameplay_state) func_0022F778
    __attribute__((alias("FUN_0022f778")));
