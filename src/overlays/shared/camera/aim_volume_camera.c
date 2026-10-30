/* Camera type 23: a volume camera that aims at the hero while it is inside the volume. */

#include "types.h"
#include "rnc/input/pad_globals.h"
#include "rnc/hero/hero.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/camera/follow_cam.h"
#include "rnc/input/pad_state.h"
#include "rnc/overlay/shared/camera/swing_volume_camera.h"

typedef float CameraVec[4] __attribute__((aligned(16)));

/* Parameters of an aim camera: it looks from the descriptor's position at the hero. */
typedef struct {
    u8  pad00[8];
    s32 volume_25faf0; /* 0x08: FUN_L00_0025faf0 volume, -1 none */
    s32 clip_volume;   /* 0x0C: is_point_inside_clip_volume id, -1 none */
    s32 volume_25fa38; /* 0x10: FUN_L00_0025fa38 volume, -1 none */
    u8  pad14[0xC];
    s32 timer;      /* 0x20: frames since the camera started (capped at 200), 0 when off */
    s16 keep_unk10; /* 0x24: 0 clears FollowCamState.unk10 each frame */
    s16 done;       /* 0x26: set when the camera ends */
    f32 unk28;      /* 0x28: degrees, passed to FUN_L00_002e8540 */
    f32 angle;      /* 0x2C: radians; with height and distance gives offset_scale */
    u8  pad30[4];
    s32 need_still;     /* 0x34: start only while tail.unk164 and tail.unk1B0 are (nearly) 0 */
    f32 distance;       /* 0x38: horizontal distance from the hero to the position */
    f32 height;         /* 0x3C: height of the hero below the position */
    f32 offset_scale;   /* 0x40 */
    s16 not_in_mode_1;  /* 0x44: nonzero blocks the camera while D_001413F4 is 1 */
    s16 only_in_mode_1; /* 0x46: nonzero allows it only while D_001413F4 is 1 */
} AimCameraParams;

extern void follow_camera_set_distance(int, float, float) __asm__("FUN_L00_002e8450");
extern s32  is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern s32  test_point_in_transformed_unit_cylinder(void *, s32) __asm__("FUN_L00_0025fa38");
extern s32  test_point_in_transformed_unit_sphere(void *, s32) __asm__("FUN_L00_0025faf0");
extern s32  try_select_follow_camera_volume(struct UpdateCam *) __asm__("FUN_L00_002e83c0");
extern void clear_follow_camera_volume_if_current(struct UpdateCam *) __asm__("FUN_L00_002e8388");
extern void fun_l00_002e8540(f32) __asm__("FUN_L00_002e8540");
extern void set_follow_camera_orbit_clamp_mode(s32) __asm__("FUN_L00_002e8568");
extern void fun_l00_002e8620(void) __asm__("FUN_L00_002e8620");

extern CameraDescriptor *camera_descriptors __asm__("D_L00_0015EF50");
extern u8                D_001413F4 __asm__("D_001413F4") NOT_SDA;

/* Nonzero while the hero is in one of the aim camera's volumes; sets `done` otherwise. */
s32 volume_camera_aim_active(struct UpdateCam *cam) __asm__("FUN_L00_002ed348");

s32 volume_camera_aim_active(struct UpdateCam *aim_camera_record) {
    struct UpdateCam *rec = current_follow_cam;
    AimCameraParams  *params = camera_descriptors[aim_camera_record->descriptor_index].params;
    FollowCamState   *state = FOLLOW_CAM_STATE(rec);
    FollowCamTail    *tail = &state->tail;
    FollowCamFlags   *ctl = &state->tail.ctl;
    if (rec->unk86 != 0)
        return 0;
    if (params->only_in_mode_1 != 0 && D_001413F4 != 1)
        return 0;
    if (params->not_in_mode_1 != 0 && D_001413F4 == 1)
        return 0;
    if (params->need_still != 0) {
        if (0.01f < absolute_float(tail->unk164))
            return 0;
        if (0.01f < absolute_float(ctl->unk1B0))
            return 0;
    }
    if (params->clip_volume >= 0 &&
        is_point_inside_clip_volume(&hero.motion.pos, params->clip_volume))
        return 1;
    if (params->volume_25fa38 >= 0 &&
        test_point_in_transformed_unit_cylinder(&hero.motion.pos, params->volume_25fa38))
        return 1;
    if (params->volume_25faf0 >= 0 &&
        test_point_in_transformed_unit_sphere(&hero.motion.pos, params->volume_25faf0))
        return 1;
    params->done = 1;
    return 0;
}

/* Runs an aim camera for one frame: on its first two frames places the
   camera at the descriptor's position and fits distance, height and offset
   scale to the hero; then eases them each frame. */
void volume_camera_aim(struct UpdateCam *cam) __asm__("FUN_L00_002ed498");

void volume_camera_aim(struct UpdateCam *aim_camera_record) {
    CameraVec         pos;
    CameraVec         to_pos;
    CameraVec         gravity_offset;
    CameraVec         level;
    AimCameraParams  *params;
    struct UpdateCam *rec;
    Vec4f            *rec_pos;
    FollowCamState   *rig;
    FollowCamState   *state;
    FollowCamBlock   *follow;
    FollowCamTail    *tail;
    FollowCamSnap    *snap;
    FollowCamReset   *reset;
    f32               up;
    f32               angle;

    params = camera_descriptors[aim_camera_record->descriptor_index].params;
    if (volume_camera_aim_active(aim_camera_record)) {
        if (try_select_follow_camera_volume(aim_camera_record) <= 0) {
            if (params->timer) {
                params->done = 1;
            }
            params->timer = 0;
            return;
        }
        if (!(pad_held[0] & (PAD_L2 | PAD_L1))) {
            goto run;
        }
    }
    if (params->timer) {
        params->done = 1;
    }
    params->timer = 0;
    clear_follow_camera_volume_if_current(aim_camera_record);
    return;
run:
    if (params->timer < 2) {
        {
            pos[0] = camera_descriptors[aim_camera_record->descriptor_index].pos[0];
            pos[1] = camera_descriptors[aim_camera_record->descriptor_index].pos[1];
            pos[2] = camera_descriptors[aim_camera_record->descriptor_index].pos[2];
        }
        *(s32 *)&pos[3] = 0;
        fast_vec_sub(to_pos, pos, &hero.motion.pos);
        up = fast_vec_dot(to_pos, &hero.gravity_dir);
        fast_vec_scale(gravity_offset, &hero.gravity_dir, up);
        fast_vec_sub(level, to_pos, gravity_offset);
        params->distance = fast_vec_length(level);
        params->height = -up;
        angle = params->angle;
        if (absolute_float(angle) == 1.5707964f) {
            *(s32 *)&params->offset_scale = 0;
        } else {
            params->offset_scale =
                params->height - params->distance * (fast_sin(angle) / fast_cos(angle));
        }
        rec = current_follow_cam;
        rig = FOLLOW_CAM_STATE(rec);
        rec_pos = &rec->pos;
        follow = &rig->follow;
        tail = &rig->tail;
        snap = &rig->tail.snap;
        /* A second pointer to the same state: retail keeps both. */
        state = rig;
        reset = &state->reset;
        qcopy(rec_pos, pos);
        tail->unk15C = params->distance;
        tail->distance = params->height;
        follow->look_height = params->offset_scale;
        fast_vec_scale(gravity_offset, &hero.gravity_dir, -tail->distance);
        fast_vec_add(state->follow.eye, &hero.motion.pos, gravity_offset);
        fast_vec_sub(tail->unk130, pos, state->follow.eye);
        qcopy(state->tail.unk140, tail->unk130);
        fast_vec_scale(gravity_offset, &hero.gravity_dir, -follow->look_height);
        fast_vec_add(state->follow.look_target, &hero.motion.pos, gravity_offset);
        qcopy(state->follow.unkD0, state->follow.look_target);
        qcopy(follow->focus, &hero.motion.pos);
        qcopy(state->follow.offset, follow->focus);
        qcopy(state->position, rec_pos);
        qcopy(state->tail.snap.unk1F0, rec_pos);
        *(s32 *)&snap->unk200 = 0;
        *(s32 *)&reset->unk34 = 0;
        reset->look_height = follow->look_height;
        {
            f32 zero = reset->unk34;
            f32 distance = tail->distance;
            reset->distance = distance;
            reset->unk22 = 0;
            reset->unk38 = zero;
            reset->unk30 = zero;
            reset->unk2C = zero;
        }
    }
    params->timer += 1;
    if (scale_game_frames(200) < params->timer) {
        params->timer = scale_game_frames(200);
    }
    if (params->distance != 0.0f) {
        follow_camera_set_distance(0, params->distance, 0.003f);
        fun_l00_002e8620();
    }
    if (params->height != 0.0f) {
        follow_camera_set_height(params->height, 0.003f);
    }
    if (params->offset_scale != 0.0f) {
        follow_camera_set_offset_scale(0, params->offset_scale, 0.005f);
    }
    if (params->unk28 != 0.0f) {
        fun_l00_002e8540(params->unk28 * 0.017453292f);
    }
    if (params->keep_unk10 == 0) {
        set_follow_camera_orbit_clamp_mode(0);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EED60), where it is exact; names translated to the US level program. */

/* Runs volume_camera_aim unless the camera is done. While D_L00_0015F5C4 is
   set and the hero is in the volumes, resets the camera instead. */
s32 volume_camera_check(struct UpdateCam *cam) __asm__("FUN_L00_002ed8b0");

s32 volume_camera_check(struct UpdateCam *aim_camera_record) {
    AimCameraParams *params = camera_descriptors[aim_camera_record->descriptor_index].params;
    if (params->done != 0)
        return 0;
    if (D_L00_0015F5C4 != 0) {
        if (volume_camera_aim_active(aim_camera_record) != 0) {
            params->done = 0;
            params->timer = 0;
            return 0;
        }
    }
    volume_camera_aim(aim_camera_record);
    return 0;
}
