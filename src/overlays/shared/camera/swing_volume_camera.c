/* Camera type 17: a volume camera that swings around the hero while it is inside the volume. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/camera/follow_cam.h"
#include "rnc/input/pad_state.h"
#include "rnc/input/pad_globals.h"
#include "rnc/overlay/shared/camera/swing_volume_camera.h"

typedef float CameraVec[4] __attribute__((aligned(16)));

/* Parameters of a swing camera. */
typedef struct {
    f32 swing; /* 0x00: degrees swung around `yaw` over the first 200 frames */
    u8  pad04[4];
    s32 volume_25faf0; /* 0x08: FUN_L00_0025faf0 volume, -1 none */
    s32 clip_volume;   /* 0x0C: is_point_inside_clip_volume id, -1 none */
    s32 volume_25fa38; /* 0x10: FUN_L00_0025fa38 volume, -1 none */
    s32 path;          /* 0x14: point_in_path_xy path index, -1 none */
    u8  pad18[8];
    f32 pitch;        /* 0x20: degrees, second angle for FUN_L00_002e8918 */
    f32 distance;     /* 0x24: follow_camera_set_distance */
    f32 height;       /* 0x28: follow_camera_set_height */
    s16 type;         /* 0x2C: 1..11, picks timing and easing */
    s16 timer;        /* 0x2E: frames since the camera started, 0 when off */
    f32 offset_scale; /* 0x30: follow_camera_set_offset_scale; follow.look_height on a type 5 snap */
    s16 keep_unk10;   /* 0x34: 0 clears FollowCamState.unk10 each frame */
    s16 check_yaw; /* 0x36: nonzero ends the camera when D_L00_00166DD8 is 80 degrees or more off `yaw` */
    f32 lateral_stiffness; /* 0x38: eased in over 120 frames from start_lateral_stiffness */
    f32 lateral_damping;   /* 0x3C: likewise from start_lateral_damping */
    f32 start_lateral_stiffness; /* 0x40: follow.lateral_stiffness of the player camera when the camera started */
    f32 start_lateral_damping; /* 0x44 */
    s16 l_buttons_end; /* 0x48: type 5 ends on L1 or L2; with `done` set it does not restart */
    s16 done;          /* 0x4A: set when the camera ends */
    s32 clip_volume2;  /* 0x4C: second clip volume, -1 none */
    f32 unk50;         /* 0x50: degrees, passed to FUN_L00_002e8540 */
    s16 set_flag1;     /* 0x54: sets tail.ctl.flags bit 1 each frame */
    s16 set_flag0;     /* 0x56: sets tail.ctl.flags bit 0 each frame */
} SwingCameraParams;

extern void follow_camera_set_distance(int, float, float) __asm__("FUN_L00_002e8450");
extern void
set_follow_camera_angle_blend_from_direction(void *, f32, f32) __asm__("FUN_L00_002e8918");
extern s32  is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern s32  test_point_in_transformed_unit_cylinder(void *, s32) __asm__("FUN_L00_0025fa38");
extern s32  test_point_in_transformed_unit_sphere(void *, s32) __asm__("FUN_L00_0025faf0");
extern s32  point_in_path_xy(void *, void *, s32) __asm__("FUN_L00_00259740");
extern s32  try_select_follow_camera_volume(struct UpdateCam *) __asm__("FUN_L00_002e83c0");
extern void clear_follow_camera_volume_if_current(struct UpdateCam *) __asm__("FUN_L00_002e8388");
extern void fun_l00_002e8490(f32) __asm__("FUN_L00_002e8490");
extern void fun_l00_002e8540(f32) __asm__("FUN_L00_002e8540");
extern void set_follow_camera_orbit_clamp_mode(s32) __asm__("FUN_L00_002e8568");
extern void fun_l00_002e8620(void) __asm__("FUN_L00_002e8620");
extern void set_follow_camera_blend_output_flags(void) __asm__("FUN_L00_002e8680");

extern float D_L00_00166DD8 NOT_SDA;
extern CameraDescriptor    *camera_descriptors __asm__("D_L00_0015EF50");
extern f32                  D_L00_00166EB4 __asm__("D_L00_00166EB4") NOT_SDA;
extern u8                   D_001413F4 __asm__("D_001413F4") NOT_SDA;
extern s32                  D_001413DC __asm__("D_001413DC") NOT_SDA;
extern s32                  hero_requested_camera_mode[] __asm__("D_001415D4");
extern u8                  *toad_paths[] __asm__("D_L00_001B04B0");

f32 D_L01_00167334 NOT_SDA = 0;
f32 D_L02_001674B4 NOT_SDA = 0;
f32 D_L03_00166FB4 NOT_SDA = 0;
f32 D_L04_00167034 NOT_SDA = 0;
f32 D_L05_001672B4 NOT_SDA = 0;
f32 D_L06_00167634 NOT_SDA = 0;
f32 D_L09_00167034 NOT_SDA = 0;
f32 D_L10_00167334 NOT_SDA = 0;
f32 D_L11_001678B4 NOT_SDA = 0;
f32 D_L12_00167334 NOT_SDA = 0;
f32 D_L14_001675B4 NOT_SDA = 0;
f32 D_L15_001674B4 NOT_SDA = 0;
f32 D_L17_001677B4 NOT_SDA = 0;

/* Nonzero while the hero is in one of the swing camera's volumes. */
/*
 * Camera volumes. Each camera record (UpdateCam) has a descriptor in
 * camera_descriptors (UpdateCam.descriptor_index); the descriptor points at
 * the parameters of its camera type. Two types are handled here:
 *  - swing cameras: volume_camera_swing_check -> volume_camera_swing;
 *  - aim cameras: volume_camera_check -> volume_camera_aim.
 * Both run while the hero stands in one of the volumes the parameters name
 * (clip volumes and the FUN_L00_0025fa38 / FUN_L00_0025faf0 / path tests),
 * then end the camera with FUN_L00_002e8388 and set `done`.
 */
s32 volume_camera_swing_active(struct UpdateCam *cam) __asm__("FUN_L00_002ec7e8");

s32 volume_camera_swing_active(struct UpdateCam *swing_camera_record) {
    SwingCameraParams *params;
    s32                forced = 0;
    u16                type;

    params = camera_descriptors[swing_camera_record->descriptor_index].params;
    if (current_follow_cam->unk86 != 0) {
        return 0;
    }
    type = *(u16 *)&params->type;
    if (params->type == 2 && D_001413DC == 0x1A && params->timer > 0) {
        forced = 1;
    } else if ((s16)type == 4 && hero_requested_camera_mode[0] == 3) {
        forced = 0 < params->timer;
    }
    if (params->clip_volume >= 0 &&
        (forced || is_point_inside_clip_volume(&hero.motion.pos, params->clip_volume)))
        return 1;
    if (params->clip_volume2 >= 0 &&
        (forced || is_point_inside_clip_volume(&hero.motion.pos, params->clip_volume2)))
        return 1;
    if (params->volume_25fa38 >= 0 && (forced || test_point_in_transformed_unit_cylinder(
                                                     &hero.motion.pos, params->volume_25fa38)))
        return 1;
    if (params->volume_25faf0 >= 0 &&
        (forced || test_point_in_transformed_unit_sphere(&hero.motion.pos, params->volume_25faf0)))
        return 1;
    if (params->path >= 0 && (forced || point_in_path_xy(&hero.motion.pos,
                                                         toad_paths[params->path] + 0x10,
                                                         *(s32 *)toad_paths[params->path])))
        return 1;
    return 0;
}

/* Runs a swing camera for one frame: by its type, sets the follow camera's
   distance, height, offset scale and springs, and swings it around the
   descriptor's yaw over `timer`. */
void volume_camera_swing(struct UpdateCam *cam) __asm__("FUN_L00_002ec988");

void volume_camera_swing(struct UpdateCam *swing_camera_record) {
    CameraVec          gravity_offset_or_yaw_direction;
    SwingCameraParams *params;
    struct UpdateCam  *rec;
    Vec4f             *pos;
    FollowCamState    *rig;
    FollowCamState    *state;
    FollowCamBlock    *follow;
    FollowCamTail     *tail;
    FollowCamSnap     *snap;
    FollowCamReset    *reset;
    s32                at_end;
    f32                total;
    f32                swing;
    f32                lateral_spring_blend_fraction;

    params = camera_descriptors[swing_camera_record->descriptor_index].params;
    if (params->type == 8 && FOLLOW_CAM_STATE(current_follow_cam)->tail.mode == 0) {
        return;
    }
    if (params->type == 6 && FOLLOW_CAM_STATE(current_follow_cam)->tail.mode != 0) {
        return;
    }
    if (volume_camera_swing_active(swing_camera_record) == 0) {
        goto stop;
    }
    if (try_select_follow_camera_volume(swing_camera_record) <= 0) {
        if (params->timer != 0) {
            params->done = 1;
        }
        params->timer = 0;
        return;
    }
    if (params->check_yaw != 0 && fast_difference_between_rotations(
                                      camera_descriptors[swing_camera_record->descriptor_index].yaw,
                                      D_L00_00166DD8) >= 1.3962634f) {
        goto stop;
    }
    if (params->l_buttons_end != 0 && params->type == 5 && (pad_held[0] & (PAD_L2 | PAD_L1))) {
    stop:
        if (params->timer != 0) {
            params->done = 1;
        }
        params->timer = 0;
        clear_follow_camera_volume_if_current(swing_camera_record);
        return;
    }
    if (params->timer == 0) {
        u8 *camera_globals = (u8 *)&l00_camera;
        params->start_lateral_stiffness =
            FOLLOW_CAM_STATE(*(struct UpdateCam **)(camera_globals + 0x180))
                ->follow.lateral_stiffness;
        params->start_lateral_damping =
            FOLLOW_CAM_STATE(*(struct UpdateCam **)(camera_globals + 0x180))
                ->follow.lateral_damping;
    }
    /* One word over type and timer: type 5 on its first frame snaps the camera. */
    if (*(s32 *)&params->type == 5) {
        *(s32 *)&params->swing = 0;
        rec = current_follow_cam;
        rig = FOLLOW_CAM_STATE(rec);
        follow = &rig->follow;
        tail = &rig->tail;
        snap = &rig->tail.snap;
        /* A second pointer to the same state: retail keeps both. */
        state = rig;
        reset = &state->reset;
        fast_vec_normalize(tail->unk130, tail->unk130, params->distance);
        tail->unk15C = params->distance;
        tail->distance = params->height;
        follow->look_height = params->offset_scale;
        qcopy(state->tail.unk140, tail->unk130);
        fast_vec_scale(gravity_offset_or_yaw_direction, &hero.gravity_dir, -tail->distance);
        pos = &rec->pos;
        fast_vec_add(state->follow.eye, &hero.motion.pos, gravity_offset_or_yaw_direction);
        fast_vec_add(pos, state->follow.eye, tail->unk130);
        fast_vec_scale(gravity_offset_or_yaw_direction, &hero.gravity_dir, -follow->look_height);
        fast_vec_add(state->follow.look_target, &hero.motion.pos, gravity_offset_or_yaw_direction);
        qcopy(state->follow.unkD0, state->follow.look_target);
        qcopy(follow->focus, &hero.motion.pos);
        qcopy(state->follow.offset, follow->focus);
        qcopy(state->position, pos);
        qcopy(state->tail.snap.unk1F0, pos);
        snap->unk200 = tail->unk15C - fast_vec_length(tail->unk130);
        *(s32 *)&reset->unk34 = 0;
        reset->look_height = follow->look_height;
        {
            f32 zero = reset->unk34;
            f32 distance = tail->distance;
            reset->distance = distance;
            reset->unk38 = zero;
            reset->unk30 = zero;
            reset->unk2C = zero;
        }
    }
    at_end = 0;
    total = convert_integer_to_float(scale_game_frames(200));
    params->timer++;
    if (params->type == 10) {
        set_follow_camera_blend_output_flags();
        FOLLOW_CAM_STATE(current_follow_cam)->tail.mode = 0x14D;
    }
    if (params->type == 11) {
        set_follow_camera_blend_output_flags();
        FOLLOW_CAM_STATE(current_follow_cam)->tail.mode = 2;
    }
    if (params->type == 2 || params->type == 4) {
        {
            FollowCamFlags *ctl = &FOLLOW_CAM_STATE(current_follow_cam)->tail.ctl;
            ctl->flags |= 3;
        }
        if (params->timer > scale_game_frames(200)) {
            at_end = 1;
            params->timer = scale_game_frames(200);
        }
        fun_l00_002e8620();
    } else if (params->type == 1 || params->type == 7 || params->type == 10 || params->type == 9) {
        /* Moving the left stick skips to the end of the swing. */
        struct PadState *pad = &controller_state;
        if (pad->analog[0] != 0.0f || pad->analog[1] != 0.0f) {
            params->timer = scale_game_frames(200);
        }
        if (params->timer >= scale_game_frames(200)) {
            at_end = 1;
            if (D_L00_00166EB4 > 0.0f) {
                params->timer = scale_game_frames(200);
            }
        }
        if (params->timer >= scale_game_frames(400)) {
            at_end = 0;
            params->timer = 1;
        }
    } else if (params->type == 3) {
        total = convert_integer_to_float(scale_game_frames(1));
        if (params->timer > scale_game_frames(1)) {
            at_end = 1;
            params->timer = scale_game_frames(1);
        }
    } else {
        if (params->timer > scale_game_frames(200)) {
            at_end = 1;
            params->timer = scale_game_frames(200);
        }
    }
    gravity_offset_or_yaw_direction[0] =
        fast_cos(camera_descriptors[swing_camera_record->descriptor_index].yaw);
    gravity_offset_or_yaw_direction[1] =
        fast_sin(camera_descriptors[swing_camera_record->descriptor_index].yaw);
    *(s32 *)&gravity_offset_or_yaw_direction[2] = 0;
    swing = params->swing * 0.017453292f * (params->timer / total);
    if ((params->type != 1 && params->type != 7 && params->type != 9) || at_end == 0) {
        set_follow_camera_angle_blend_from_direction(
            gravity_offset_or_yaw_direction, swing, params->pitch * 0.017453292f);
    }
    if (params->distance != 0.0f) {
        if (params->type == 6 || params->type == 8) {
            follow_camera_set_distance(0, params->distance, 0.002f);
            fun_l00_002e8490(params->distance + 1.36f);
        } else {
            follow_camera_set_distance(0, params->distance, 0.003f);
            fun_l00_002e8620();
        }
    }
    if (params->height != 0.0f) {
        if (params->type == 6 || params->type == 8) {
            follow_camera_set_height(params->height, 0.002f);
        } else {
            follow_camera_set_height(params->height, 0.003f);
        }
    }
    if (params->offset_scale != 0.0f) {
        if (params->type == 6 || params->type == 8) {
            follow_camera_set_offset_scale(0, params->offset_scale, 0.002f);
        } else {
            follow_camera_set_offset_scale(0, params->offset_scale, 0.005f);
        }
    }
    if (params->unk50 != 0.0f) {
        fun_l00_002e8540(params->unk50 * 0.017453292f);
    }
    if (params->set_flag1 != 0) {
        {
            FollowCamFlags *ctl = &FOLLOW_CAM_STATE(current_follow_cam)->tail.ctl;
            ctl->flags |= 2;
        }
    }
    if (params->set_flag0 != 0) {
        {
            FollowCamFlags *ctl = &FOLLOW_CAM_STATE(current_follow_cam)->tail.ctl;
            ctl->flags |= 1;
        }
    }
    if (params->keep_unk10 == 0) {
        set_follow_camera_orbit_clamp_mode(0);
    }
    if (params->lateral_stiffness != 0.0f || params->lateral_damping != 0.0f) {
        if (params->timer >= scale_game_frames(120)) {
            follow_camera_set_lateral_spring(params->lateral_stiffness, params->lateral_damping);
        } else {
            lateral_spring_blend_fraction =
                params->timer / convert_integer_to_float(scale_game_frames(120));
            follow_camera_set_lateral_spring(
                params->start_lateral_stiffness +
                    (params->lateral_stiffness - params->start_lateral_stiffness) *
                        lateral_spring_blend_fraction,
                params->start_lateral_damping +
                    (params->lateral_damping - params->start_lateral_damping) *
                        lateral_spring_blend_fraction);
        }
    }
    if (params->type == 1 || params->type == 7 || params->type == 10) {
        fun_l00_002e8620();
        follow_camera_start_slide();
        follow_camera_set_up_spring(0.01f, 0.2f);
    }
}

/* Runs volume_camera_swing unless its type is blocked right now (by
   D_001413F4, D_001413DC, or the hero being off the ground). */
s32 volume_camera_swing_check(struct UpdateCam *cam) __asm__("FUN_L00_002ed230");

s32 volume_camera_swing_check(struct UpdateCam *swing_camera_record) {
    SwingCameraParams *params = camera_descriptors[swing_camera_record->descriptor_index].params;
    if (params->l_buttons_end == 1 && params->done != 0)
        return 0;
    if (params->type == 11 && D_001413F4 != 2)
        return 0;
    if ((params->type == 1 || params->type == 10) && params->timer == 0) {
        struct Hero *hero_data = &hero;
        if (hero_data->frames_off_ground.s != 0) {
            if (current_level_index != 14)
                return 0;
            if (hero_data->unk2284 != 13)
                return 0;
        }
    }
    if (params->type == 2 && D_001413DC != 0x1a) {
        params->timer = 0;
        return 0;
    }
    volume_camera_swing(swing_camera_record);
    return 0;
}

f32 D_L00_00166EB4 NOT_SDA = 0;
