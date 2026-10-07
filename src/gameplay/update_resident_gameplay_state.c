#include "types.h"
#include "sda.h"
#include "qcopy.h"

typedef unsigned int u128 __attribute__((mode(TI)));

typedef union {
    u128 quadword;
    f32 components[4];
    u8 bytes[16];
} Vector4;

typedef struct {
    u8 pad0[6];
    u8 unk6;
    u8 pad7[5];
    u8 count; /* 0x0C */
    u8 padD[0x48 - 0xD];
    s32 frames[1]; /* 0x48 */
} RenderModel;

typedef struct ResidentRenderObject {
    u8 pad0[0x10];
    Vector4 position; /* 0x10 */
    u8 state;         /* 0x20 */
    u8 pad21[3];
    RenderModel *model; /* 0x24 */
    u8 pad28[0xA];
    s16 selected_index; /* 0x32 */
    u16 flags;          /* 0x34 */
    u8 pad36[2];
    s64 lifetime_stamp; /* 0x38 */
    Vector4 rotation;   /* 0x40 */
    u8 current_frame;
    u8 next_frame;
    u8 selected_a;
    u8 selected_b;
    f32 blend; /* 0x54 */
    u8 pad58[0x71 - 0x58];
    u8 cached_selector;
    u8 opacity;
    u8 unk73;
    u8 pad74[4];
    u8 *animation_positions; /* 0x78 */
    u8 pad7C[3];
    u8 unk7F;
    u8 pad80[0x10];
    s32 color; /* 0x90 */
    s32 unk94; /* 0x94 */
    u8 pad98[0xE];
    s16 class_id; /* 0xA6 */
    u8 padA8[0xA];
    s16 unkB2; /* 0xB2 */
    u8 padB4[8];
    u8 unkBC; /* 0xBC */
    u8 padBD[3];
    Vector4 transform[4]; /* 0xC0 */
} ResidentRenderObject;

typedef struct {
    Vector4 position;
    f32 rotation[4];
} CameraKeyframe;

typedef struct {
    u8 pad0[0x34];
    s32 time;           /* 0x34 */
    s32 frame;          /* 0x38 */
    s32 sequence_frame; /* 0x3C */
    s16 end;            /* 0x40 */
    u8 pad42[2];
    s16 count; /* 0x44 */
    u8 pad46[0x54 - 0x46];
    CameraKeyframe *frames; /* 0x54 */
    u8 pad58[0x178 - 0x58];
    ResidentRenderObject *objects[1]; /* 0x178 */
} ResidentPlaybackState;

typedef struct {
    ResidentRenderObject *player; /* 0x00 */
    u8 pad4[4];
    ResidentRenderObject *attachment; /* 0x08 */
    u8 padC[4];
    ResidentRenderObject *companion_a; /* 0x10 */
    ResidentRenderObject *companion_b; /* 0x14 */
    u8 pad18[8];
    s32 state;           /* 0x20 */
    s16 timer;           /* 0x24 */
    s16 content_variant; /* 0x26 */
    s16 skip;            /* 0x28 */
    u8 pad2A[2];
    s16 unk2C; /* 0x2C */
    u8 pad2E[2];
    s32 path;                     /* 0x30 */
    s32 source_camera_index;      /* 0x34 */
    s32 destination_camera_index; /* 0x38 */
    f32 path_progress;            /* 0x3C */
    f32 speed;                    /* 0x40 */
    f32 path_segment_length;      /* 0x44 */
    f32 blend;                    /* 0x48 */
    f32 interpolation_velocity;   /* 0x4C */
    s32 trail;                    /* 0x50 */
    s32 history_count;            /* 0x54 */
    u8 pad58[8];
    Vector4 unk60;    /* 0x60 */
    Vector4 unk70;    /* 0x70 */
    Vector4 startPos; /* 0x80 */
    Vector4 pathPos;  /* 0x90 */
    Vector4 startRot; /* 0xA0 */
    f32 unkB0;
    f32 rotY; /* 0xB4 */
    f32 rotZ; /* 0xB8 */
    f32 unkBC;
    Vector4 trailA[32]; /* 0xC0 */
    Vector4 trailB[32]; /* 0x2C0 */
} ResidentCinematicState;

typedef struct {
    s32 point_count;
    s32 pad[3];
    Vector4 p[1]; /* 0x10: x, y, z, angle */
} ScriptedPath;

typedef struct {
    u8 pad0[0x30];
    Vector4 position; /* 0x30 */
    u8 pad40[0x34];
    f32 rotY; /* 0x74 */
    f32 rotZ; /* 0x78 */
    u8 pad7C[4];
} CameraBlendNode;

typedef struct {
    u8 pad0[0x140];
    Vector4 position; /* 0x140 */
    Vector4 unk150;   /* 0x150 */
    u8 pad160[0x350 - 0x160];
    Vector4 forward; /* 0x350 */
    Vector4 right;   /* 0x360 */
    Vector4 up;      /* 0x370 */
} ResidentCameraState;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[0x3A];
    u16 unk5A;
} DialoguePlaybackState;

typedef struct {
    u8 pad0[0x2080];
    ResidentRenderObject *reference_object;
} LevelObjectState;

typedef struct {
    Vector4 a;
    Vector4 b;
} PositionPair;

extern s32 D_0013CAE4[];
extern u8 D_0013D4C0[];
extern ResidentCinematicState level_render_state __asm__("D_0013E030");
extern LevelObjectState D_0013F350;
extern u8 D_001413F5[];
extern DialoguePlaybackState D_001516D0;
extern f32 D_0015ED60;
extern f32 D_0015ED6C;
extern f32 D_0015ED70 __attribute__((sda));
extern s32 D_0015ED80;
extern s32 current_level_index __asm__("D_0015ED84");
extern u8 D_0015EDB4;
extern f32 sequence_fade __asm__("D_0015F43C");
extern f32 D_0015F440;
extern s32 D_0015F5B0;
extern s32 game_stage __asm__("D_0015F604");
extern s32 D_0015F618[1];
extern ResidentRenderObject *D_0015FF1C;
extern CameraBlendNode *D_00160034;
extern s32 D_00160510[1] __attribute__((sda));
extern ResidentCameraState D_00186F40;
extern ResidentPlaybackState render_sequence __asm__("D_0018CB20");
extern f32 D_0018CDB0[4];
extern ScriptedPath *D_001CC3B0[];
extern s32 D_001D5BF0 NOT_SDA;
extern PositionPair D_001D99B0[];
extern Vector4 D_001D9C80[];
extern Vector4 D_001D9CB0[];
extern void FUN_0022e1b0();

extern void CalculateDmaTransferAddress(void);
extern void music_pause(s32);
extern void music_unpause(void);
extern void sceVu0UnitMatrix(void *);
extern void SceVu0RotMatrixX(void *, void *, f32);
extern void SceVu0RotMatrixY(void *, void *, f32);
extern void sceVu0RotMatrixZ(void *, void *, f32);
extern s32 sceGsSyncV(s32);
extern void func_001E93F0(void *, void *, s32, s32, s32);
extern void func_001E93F8(void *);
extern void func_001E9400(void *);
extern void func_001E9408(void);
extern void func_001E9410(ResidentRenderObject *);
extern void func_001E9418(ResidentRenderObject *);
extern void func_001E9420(void);
extern void func_001E9428(void);
extern void func_001E9430(void);
extern void func_001E9438(void);
extern void func_001E9440(void *, void *, s32, s32);
extern void func_001E9450(ResidentRenderObject *, ResidentRenderObject *);
extern void update_camera(void) __asm__("func_001EDAA8");
extern void update_view_context(void) __asm__("func_001F2D98");
extern void enqueue_callback_list_1(void *, ResidentRenderObject *) __asm__("FUN_001f4600");
extern void enqueue_callback_list_4(void *, ResidentRenderObject *) __asm__("FUN_001f47b8");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern f32 func_001F96E8(f32);
extern s32 scale_ticks(s32) __asm__("func_001F96F8");
extern f32 AbsoluteFloat(f32) __asm__("func_001F99C0");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void add_vectors(void *, void *, void *) __asm__("func_001F9A10");
extern void func_001F9A40(void *, void *, void *, f32);
extern void scale_vector(void *, void *, f32) __asm__("func_001F9A68");
extern void fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 func_001F9B80(void *, void *, void *);
extern void func_001F9CF8(void *, void *, void *);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 func_001F9E90(f32, f32);
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void update_all_point_lights(void) __asm__("func_00201A28");
extern void parse_space_scene_chunk(s32) __asm__("FUN_002049f0");
extern ResidentRenderObject *create_moby(s32) __asm__("func_0020C4F8");
extern void mark_moby_for_removal(ResidentRenderObject *) __asm__("func_0020C828");
extern void update_moby_animation_state(ResidentRenderObject *) __asm__("func_0020C880");
extern void func_0020CFD0(void);
extern void refresh_resident_object_spatial_bounds(ResidentRenderObject *) __asm__("func_0020DEF8");
extern void update_visible_resident_objects(void) __asm__("func_00212E28");
extern void update_moby_shadow_range(ResidentRenderObject *) __asm__("func_00213700");
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern void sample_camera_path(ScriptedPath *, s32, void *, void *, s32,
                               f32) __asm__("func_00214E58");
extern void continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");
extern void update_audio_stream_until_idle(s32) __asm__("FUN_002168a8");
extern void func_00217B88(void);
extern void pause_all_sounds(s32) __asm__("FUN_00218d78");
extern void sound_update(void) __asm__("func_0022CA50");
extern void enqueue_voice_request(void) __asm__("func_0022DC50");
extern void update_level_gameplay_frame(void) __asm__("func_0022EAA8");
extern void func_0022F5B0(ResidentRenderObject *, f32);
extern void draw_light_flare() __asm__("func_0022DF40");
extern void build_resident_indexed_texture_warp_meshes() __asm__("func_0022E420");
extern void render_environment_mapped_object() __asm__("func_002327A0");

void update_resident_gameplay_state(void) __asm__("FUN_0022f778");

void update_resident_gameplay_state(void) {
    Vector4 scratch_vectors[4];
    s32 keyframe_flags;
    s32 ticks_per_bank;
    s32 skip;
    s32 object_index;
    s32 point_index;
    ResidentRenderObject *object;
    ResidentRenderObject *object_cursor;
    ResidentRenderObject *expired_object;
    s32 release_index;
    s32 bank_tick_limit;
    ResidentRenderObject *player;
    s32 path_point_index;
    f32 camera_blend_step;
    CameraKeyframe *keyframe;
    f32 *keyframe_angles;
    u8 *animation_positions;
    s32 current_frame;
    ScriptedPath *path;
    f32 segment_angle;
    f32 maximum_turn;
    f32 turn_scale;
    f32 blend;

    level_render_state.timer++;
    switch (level_render_state.state) {
    case 0:
    case 8:
        func_001E9430();
        update_visible_resident_objects();
        enqueue_voice_request();
        func_001E9420();
        func_00217B88();
        sequence_fade -= 0.125f;
        render_sequence.frame++;
        render_sequence.time++;
        if (sequence_fade < 0.0f) {
            sequence_fade = 0.0f;
        }
        ticks_per_bank = D_0015ED80 ? 0x50 : 0x60;
        skip = level_render_state.skip;
        if ((D_0013CAE4[0] & 0x50) && scale_ticks(30) < render_sequence.time &&
            sequence_fade == 0.0f) {
            if (level_render_state.state == 0 &&
                render_sequence.time <
                    scale_ticks(D_00160510[level_render_state.content_variant] - 30)) {
                skip = 1;
            }
            if (level_render_state.state == 8 &&
                render_sequence.time < render_sequence.end - scale_ticks(30)) {
                skip = 1;
            }
        }
        if (skip) {
            if (D_001516D0.unk5A != 6 && D_001516D0.unk5A != 7) {
                D_001516D0.unk5A = 5;
            }
            if (level_render_state.state == 0) {
                render_sequence.time = scale_ticks(D_00160510[level_render_state.content_variant]);
                render_sequence.sequence_frame = render_sequence.time / ticks_per_bank;
                parse_space_scene_chunk(render_sequence.sequence_frame);
                render_sequence.frame = render_sequence.time % ticks_per_bank;
                if (level_render_state.skip) {
                    level_render_state.skip = 0;
                } else {
                    fade_to_black(4);
                }
                sequence_fade = 1.0f;
            } else {
                render_sequence.time = render_sequence.end;
            }
        }
        if (render_sequence.time >= render_sequence.end) {
            if (D_001516D0.unk5A != 6 && D_001516D0.unk5A != 7) {
                D_001516D0.unk5A = 5;
            }
            CalculateDmaTransferAddress();
            D_0018CDB0[0] = 0.63f;
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
            for (object_cursor = D_0015FF1C; object_cursor->state != 0xFF; object_cursor++) {
                if (!(object_cursor->state & 0x80) &&
                    (object_cursor->class_id == 0x4A || object_cursor->class_id == 0xCB)) {
                    object_cursor->flags &= ~0x80;
                }
            }
            if (level_render_state.state == 0) {
                pause_all_sounds(0);
                D_001D5BF0 = 0xE;
                D_0015F618[0] = 1;
                return;
            }
            game_stage = 0;
            music_unpause();
            D_0015F618[0] = 1;
            D_001413F5[0] = 0;
            func_001E9438();
            if (level_render_state.unk2C != 0) {
                func_001E9408();
                return;
            }
            func_001E9440(&level_render_state.unk60, &level_render_state.unk70, 0, 1);
            return;
        }
        bank_tick_limit = D_0015ED80 ? 0x50 : 0x60;
        if (render_sequence.frame >= bank_tick_limit) {
            parse_space_scene_chunk(++render_sequence.sequence_frame);
        }
        keyframe = &render_sequence.frames[render_sequence.frame];
        keyframe_angles = keyframe->rotation;
        keyframe_flags = keyframe->position.bytes[12];
        D_0018CDB0[0] = keyframe_angles[3];
        update_view_context();
        qcopy(&D_00186F40.position, &keyframe->position);
        func_001F9CF8(&D_00186F40.position, &D_00186F40.position,
                      level_render_state.player->transform);
        add_vectors(&D_00186F40.position, &D_00186F40.position,
                    &level_render_state.player->position);
        segment_angle = fast_add_rotations(keyframe_angles[2],
                                           level_render_state.player->rotation.components[2]);
        sceVu0UnitMatrix(scratch_vectors);
        SceVu0RotMatrixX(scratch_vectors, scratch_vectors, keyframe->rotation[0]);
        SceVu0RotMatrixY(scratch_vectors, scratch_vectors, keyframe_angles[1]);
        sceVu0RotMatrixZ(scratch_vectors, scratch_vectors, segment_angle);
        D_00186F40.forward.components[0] = -scratch_vectors[2].components[0];
        D_00186F40.right.components[0] = -scratch_vectors[0].components[0];
        D_00186F40.up.components[0] = scratch_vectors[1].components[0];
        D_00186F40.forward.components[1] = -scratch_vectors[2].components[1];
        D_00186F40.right.components[1] = -scratch_vectors[0].components[1];
        D_00186F40.up.components[1] = scratch_vectors[1].components[1];
        D_00186F40.forward.components[2] = -scratch_vectors[2].components[2];
        D_00186F40.right.components[2] = -scratch_vectors[0].components[2];
        D_00186F40.up.components[2] = scratch_vectors[1].components[2];
        if (D_0015EDB4 != 0) {
            fast_vec_cross(&D_00186F40.right, &D_00186F40.up, &D_00186F40.forward);
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
            scale_vector(&scratch_vectors[0], animation_positions + object->current_frame * 16,
                         1.0f - object->blend);
            scale_vector(&scratch_vectors[1], animation_positions + object->next_frame * 16,
                         object->blend);
            add_vectors(&object->position, &scratch_vectors[0], &scratch_vectors[1]);
            func_001F9CF8(&object->position, &object->position,
                          level_render_state.player->transform);
            add_vectors(&object->position, &object->position, &level_render_state.player->position);
            object->rotation.components[2] = level_render_state.player->rotation.components[2];
            object->cached_selector = 0xFF;
            refresh_resident_object_spatial_bounds(object);
            if (object->class_id == 0) {
                if (level_render_state.state == 0) {
                    if (current_level_index == 10 && D_0013D4C0[6] == 0) {
                        object->unk7F = 0;
                    } else if (render_sequence.time <= scale_ticks(0x3E)) {
                        object->unk7F = 0x18;
                    } else {
                        object->unk7F = 0;
                    }
                } else if (scale_ticks(360) < render_sequence.time) {
                    if (current_level_index == 10 && D_0013D4C0[6] == 0) {
                        object->unk7F = 0;
                    } else {
                        object->unk7F = 0x18;
                    }
                } else {
                    object->unk7F = 0;
                }
            } else if (object->class_id == 10) {
                if (level_render_state.state == 0) {
                    if (render_sequence.time <= scale_ticks(350)) {
                        object->unk7F = 0x18;
                    } else {
                        object->unk7F = 0;
                    }
                } else if (render_sequence.time <= scale_ticks(524)) {
                    object->unk7F = 0;
                } else {
                    object->unk7F = 0x18;
                }
            }
            if (object->unk7F != 0) {
                update_moby_shadow_range(object);
            }
            if (object->class_id == 10) {
                func_001E9418(object);
            }
            if (object->class_id == 0) {
                func_001E9410(object);
                if ((current_level_index == 10 && D_0013D4C0[6] != 0) ||
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
                    func_001E9450(object, level_render_state.attachment);
                }
            }
            if ((u16)object->class_id - 0x213U < 3) {
                enqueue_callback_list_4(render_environment_mapped_object, object);
                enqueue_callback_list_1(draw_light_flare, object);
                if (level_render_state.state == 0) {
                    if (render_sequence.time < scale_ticks(360)) {
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
                    } else if (render_sequence.time < scale_ticks(0x1F8)) {
                        s32 color_intensity =
                            (s32)(((f32)render_sequence.time - func_001F96E8(360.0f)) *
                                  (D_0015ED60 * 1.25f));
                        if (color_intensity >= 256) {
                            color_intensity = 255;
                        }
                        if (object->class_id == 0x215) {
                            object->color = (color_intensity << 8) | (color_intensity << 16);
                        } else {
                            object->color =
                                color_intensity | (color_intensity << 8) | (color_intensity << 16);
                        }
                        if (scale_ticks(0x1A4) < render_sequence.time) {
                            level_render_state.player->unkBC =
                                (s32)(((f32)render_sequence.time - func_001F96E8(420.0f)) *
                                      (D_0015ED60 * 1.2f));
                            level_render_state.player->unkB2 = 0;
                            enqueue_callback_list_1(FUN_0022e1b0, level_render_state.player);
                        }
                        if (scale_ticks(0x1D0) < render_sequence.time) {
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
                } else if (render_sequence.time < scale_ticks(0xF0)) {
                    s32 color_intensity;
                    if (render_sequence.time < scale_ticks(0xC8)) {
                        func_0022F5B0(object, D_0015ED6C * -4.0f);
                        func_0022F5B0(object, D_0015ED6C * -3.0f);
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
                    enqueue_callback_list_1(FUN_0022e1b0, object);
                } else if (render_sequence.time <= scale_ticks(360)) {
                    s32 color_intensity;
                    if (render_sequence.time < scale_ticks(300)) {
                        level_render_state.player->unkBC =
                            (s32)((func_001F96E8(300.0f) - (f32)render_sequence.time) *
                                  (D_0015ED60 * 1.5f));
                        level_render_state.player->unkB2 = 0;
                        enqueue_callback_list_1(FUN_0022e1b0, level_render_state.player);
                    }
                    color_intensity = (s32)((func_001F96E8(360.0f) - (f32)render_sequence.time) *
                                            (D_0015ED60 * 1.25f));
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
        func_0020CFD0();
        func_001E9428();
        break;

    case 3:
        func_001E9430();
        update_visible_resident_objects();
        enqueue_voice_request();
        func_001E9420();
        func_00217B88();
        if (level_render_state.timer == 0) {
            music_pause(0);
            update_audio_stream_until_idle(0);
            D_001516D0.unk1C = level_render_state.content_variant + 0x9C4F;
            fade_to_black(scale_ticks(12));
            while ((s16)D_001516D0.unk5A != 3) {
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
                    D_0013F350.reference_object->lifetime_stamp;
                level_render_state.companion_b = create_moby(10);
                level_render_state.companion_b->selected_index = 0x1FF;
                level_render_state.companion_b->opacity = 0xFF;
                level_render_state.companion_b->unk94 = 0;
                level_render_state.companion_b->flags |= 6;
                level_render_state.companion_b->lifetime_stamp =
                    D_0013F350.reference_object->lifetime_stamp;
            }
            func_001E93F0(&D_00186F40.position, &D_00186F40.unk150, 1, 0, 0);
            level_render_state.blend = 0.0f;
            level_render_state.interpolation_velocity = 0.0f;
            if (level_render_state.path >= 0) {
                path = D_001CC3B0[level_render_state.path];
                maximum_turn = level_render_state.blend;
                path->p[0].components[3] = maximum_turn;
                for (path_point_index = 1; path_point_index < path->point_count - 1;
                     path_point_index++) {
                    segment_angle = func_001F9E90(path->p[path_point_index].components[0] -
                                                      path->p[path_point_index - 1].components[0],
                                                  path->p[path_point_index].components[1] -
                                                      path->p[path_point_index - 1].components[1]);
                    path->p[path_point_index].components[3] = fast_subtract_rotations(
                        func_001F9E90(path->p[path_point_index + 1].components[0] -
                                          path->p[path_point_index].components[0],
                                      path->p[path_point_index + 1].components[1] -
                                          path->p[path_point_index].components[1]),
                        segment_angle);
                    if (maximum_turn < AbsoluteFloat(path->p[path_point_index].components[3])) {
                        maximum_turn = AbsoluteFloat(path->p[path_point_index].components[3]);
                    }
                }
                segment_angle = func_001F9E90(path->p[path->point_count - 1].components[0] -
                                                  path->p[path->point_count - 2].components[0],
                                              path->p[path->point_count - 1].components[1] -
                                                  path->p[path->point_count - 2].components[1]);
                path->p[path->point_count - 1].components[3] = fast_subtract_rotations(
                    func_001F9E90(
                        path->p[0].components[0] - path->p[path->point_count - 1].components[0],
                        path->p[0].components[1] - path->p[path->point_count - 1].components[1]),
                    segment_angle);
                turn_scale = 0.34906584f / maximum_turn;
                for (point_index = 0; point_index < path->point_count; point_index++) {
                    path->p[point_index].components[3] *= turn_scale;
                }
                level_render_state.path_segment_length = distance_xyz(&path->p[0], &path->p[1]);
                level_render_state.path_progress = 0.0f;
                level_render_state.speed = 0.0f;
                level_render_state.trail = 0;
                level_render_state.history_count = 0;
                player = level_render_state.player;
                qcopy(&level_render_state.startPos, &player->position);
                qcopy(&level_render_state.pathPos, &path->p[0]);
                qcopy(&level_render_state.startRot, &player->rotation);
                level_render_state.rotY = func_001F9E90(
                    func_001F9B80(&path->p[1], &path->p[0], &level_render_state.startPos),
                    path->p[0].components[2] - path->p[1].components[2]);
                level_render_state.rotZ =
                    func_001F9E90(path->p[1].components[0] - path->p[0].components[0],
                                  path->p[1].components[1] - path->p[0].components[1]);
            }
        }
        if (level_render_state.source_camera_index >= 0 &&
            level_render_state.destination_camera_index >= 0) {
            camera_blend_step = D_0015ED70 * 0.666f;
            advance_accelerated_scalar(&level_render_state.blend,
                                       &level_render_state.interpolation_velocity, 1.0f,
                                       camera_blend_step, camera_blend_step, D_0015ED6C * 0.5f);
            func_001F9A40(&scratch_vectors[0],
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
            func_001E93F8(&scratch_vectors[0]);
            func_001E9400(&scratch_vectors[1]);
        }
        if (level_render_state.path >= 0) {
            if (level_render_state.timer < scale_ticks(150)) {
                blend =
                    (1.0f - fast_cos(convert_integer_to_float(level_render_state.timer) *
                                     (3.1415927f / convert_integer_to_float(scale_ticks(120))))) *
                    0.5f;
                if (level_render_state.timer >= scale_ticks(120)) {
                    blend = 1.0f;
                }
                level_render_state.player->blend = blend;
                func_001F9A40(&level_render_state.player->position, &level_render_state.startPos,
                              &level_render_state.pathPos, blend);
                level_render_state.player->rotation.components[0] = 0.0f;
                level_render_state.player->rotation.components[1] = fast_add_rotations(
                    level_render_state.startRot.components[1],
                    fast_subtract_rotations(level_render_state.rotY,
                                            level_render_state.startRot.components[1]) *
                        blend);
                level_render_state.player->rotation.components[2] = fast_add_rotations(
                    level_render_state.startRot.components[2],
                    fast_subtract_rotations(level_render_state.rotZ,
                                            level_render_state.startRot.components[2]) *
                        blend);
                if (sequence_fade > 0.0f) {
                    sequence_fade -= 0.125f;
                    if (sequence_fade < 0.0f) {
                        sequence_fade = 0.0f;
                    }
                }
                if (blend < 1.0f) {
                    func_0022F5B0(level_render_state.player, D_0015ED6C * -3.75f);
                }
            } else {
                ScriptedPath *active_path;
                level_render_state.speed += 0.8f;
                active_path = D_001CC3B0[level_render_state.path];
                if (level_render_state.speed > 100.0f) {
                    level_render_state.speed = 100.0f;
                }
                if ((D_0013CAE4[0] & 0x50) && sequence_fade < 0.0625f) {
                    sequence_fade = 0.0625f;
                }
                if ((f32)(active_path->point_count - 6) < level_render_state.path_progress &&
                    sequence_fade < 0.0625f) {
                    sequence_fade = 0.0625f;
                }
                if (level_render_state.content_variant < 2) {
                    level_render_state.player->unkBC = 0x32;
                    level_render_state.player->unkB2 = 10;
                    enqueue_callback_list_1(FUN_0022e1b0, level_render_state.player);
                }
                level_render_state.path_progress +=
                    level_render_state.speed * D_0015ED6C / level_render_state.path_segment_length;
                if ((f32)(active_path->point_count - 1) < level_render_state.path_progress ||
                    sequence_fade >= 1.0f) {
                    D_0015F5B0 = 1;
                    D_0015F618[0] = 1;
                } else {
                    sample_camera_path(active_path, 1, &level_render_state.player->position,
                                       &level_render_state.player->rotation, 0,
                                       level_render_state.path_progress);
                    level_render_state.player->rotation.components[0] =
                        level_render_state.player->rotation.components[3];
                    level_render_state.trail = (level_render_state.trail + 1) & 0x1F;
                    if (level_render_state.history_count < 0x20) {
                        level_render_state.history_count++;
                    }
                    func_001F9CF8(&level_render_state.trailA[level_render_state.trail],
                                  &D_001D99B0[level_render_state.content_variant].a,
                                  level_render_state.player->transform);
                    add_vectors(&level_render_state.trailA[level_render_state.trail],
                                &level_render_state.trailA[level_render_state.trail],
                                &level_render_state.player->position);
                    level_render_state.trailA[level_render_state.trail].components[3] = 1.0f;
                    func_001F9CF8(&level_render_state.trailB[level_render_state.trail],
                                  &D_001D99B0[level_render_state.content_variant].b,
                                  level_render_state.player->transform);
                    add_vectors(&level_render_state.trailB[level_render_state.trail],
                                &level_render_state.trailB[level_render_state.trail],
                                &level_render_state.player->position);
                    level_render_state.trailB[level_render_state.trail].components[3] = 1.0f;
                    enqueue_callback_list_1(build_resident_indexed_texture_warp_meshes,
                                            level_render_state.player);
                    if (sequence_fade > 0.0f) {
                        sequence_fade += 0.0625f;
                        if (sequence_fade > 1.0f) {
                            sequence_fade = 1.0f;
                        }
                    }
                }
            }
            {
                ResidentRenderObject *p = level_render_state.player;
                ResidentRenderObject *scratch_vectors = level_render_state.companion_a;
                qcopy(&scratch_vectors->rotation, &p->rotation);
                func_001F9CF8(&scratch_vectors->position,
                              &D_001D9C80[level_render_state.content_variant], p->transform);
            }
            add_vectors(&level_render_state.companion_a->position,
                        &level_render_state.companion_a->position,
                        &level_render_state.player->position);
            refresh_resident_object_spatial_bounds(level_render_state.companion_a);
            {
                ResidentRenderObject *scratch_vectors = level_render_state.companion_b;
                ResidentRenderObject *p = level_render_state.player;
                qcopy(&scratch_vectors->rotation, &p->rotation);
                func_001F9CF8(&scratch_vectors->position,
                              &D_001D9CB0[level_render_state.content_variant], p->transform);
            }
            add_vectors(&level_render_state.companion_b->position,
                        &level_render_state.companion_b->position,
                        &level_render_state.player->position);
            refresh_resident_object_spatial_bounds(level_render_state.companion_b);
        }
        update_camera();
        sound_update();
        update_all_point_lights();
        func_0020CFD0();
        func_001E9428();
        break;

    case 4:
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
