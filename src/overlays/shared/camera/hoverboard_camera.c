/* Camera type 8, the hoverboard camera (Rilgar and Kalebo III): follows the hero and orbits it through ramp launches, reset from its preset. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"

#include "sda.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/math/rotations/extract_matrix_rotation_quaternion.h"
#include "rnc/overlay/shared/camera/init_camera_system.h"
#include "rnc/camera/camera_spring.h"

/* Sets up a moby's vector from the level table and clamps its height for the current state. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032A868), where it is exact; names translated to the US level program. */

void update_hero_follow_camera_focus(char *moby) __asm__("FUN_L05_00329358");

void update_hero_follow_camera_focus(char *moby) {
    float  hero_focus_offset[4];
    char  *orbit = *(char **)(moby + 0x70);
    float *focus_position = (float *)(orbit + 0x80);
    *(OvlQuad *)hero_focus_offset = 0;
    hero_focus_offset[2] = 0.7f;
    transform_vector(hero_focus_offset, hero_focus_offset, &hero);
    fast_vec_add(focus_position, &hero.motion.pos, hero_focus_offset);
    qcopy(orbit + 0x1D0, focus_position);
    if (current_level_index == 5) {
        float height_reference_position[4];
        *(OvlQuad *)height_reference_position = 0;
        height_reference_position[0] = 286.16f;
        height_reference_position[1] = 447.82f;
        height_reference_position[2] = 63.37f;
        height_reference_position[3] = 1.0f;
        if (vector_distance_xy(height_reference_position, focus_position) < 48.0f) {
            if (focus_position[2] < 62.4f) {
                focus_position[2] = 62.4f;
            }
        } else {
            if (focus_position[2] < 67.2f) {
                focus_position[2] = 67.2f;
            }
        }
    } else if (current_level_index == 0x10) {
        if (focus_position[2] < 77.5f) {
            focus_position[2] = 77.5f;
        }
    }
    fast_vec_normalize(focus_position + 8, &hero.gravity_dir, -1.0f);
}

extern char *D_L05_00167200 NOT_SDA;
void                        set_camera_follow_parameters(float spring_distance,
                                                         float spring_height,
                                                         float focus_distance,
                                                         float airborne_spring_distance,
                                                         float airborne_spring_height,
                                                         float airborne_focus_distance,
                                                         float position_spring_response_coefficient,
                                                         float position_spring_coefficient) __asm__("FUN_L05_003294c0");

void set_camera_follow_parameters(float spring_distance,
                                  float spring_height,
                                  float focus_distance,
                                  float airborne_spring_distance,
                                  float airborne_spring_height,
                                  float airborne_focus_distance,
                                  float position_spring_response_coefficient,

                                  float position_spring_coefficient) {
    char *camera = D_L05_00167200;
    char *orbit = *(char **)(camera + 0x70);

    *(float *)(orbit + 0x184) = spring_distance;
    *(float *)(orbit + 0x188) = spring_height;
    *(float *)(orbit + 0x180) = focus_distance;
    *(float *)(orbit + 0x190) = airborne_spring_distance;
    *(float *)(orbit + 0x194) = airborne_spring_height;
    *(float *)(orbit + 0x18C) = airborne_focus_distance;
    orbit = *(char **)(camera + 0x70);
    if (position_spring_response_coefficient < 0.0f) {
        *(float *)(orbit + 0x10) = 0.01f;
    } else {
        *(float *)(orbit + 0x10) = position_spring_response_coefficient;
    }
    if (position_spring_coefficient < 0.0f) {
        *(float *)(orbit + 0x14) = 0.2f;
    } else {
        *(float *)(orbit + 0x14) = position_spring_coefficient;
    }
}
/* Places the camera moby relative to the hero's target and builds its look-at basis. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032AA50), where it is exact; names translated to the US level program. */

void build_hero_follow_camera_transform(char *camera) __asm__("FUN_L05_00329540");

void build_hero_follow_camera_transform(char *camera) {
    char        *orbit = *(char **)(camera + 0x70);
    char        *at = camera + 0x30;
    char        *up = camera + 0x10;
    char        *side = camera + 0x20;
    struct Hero *hero_state;
    char        *focus;
    char        *spring;
    float        target_height_offset[4];
    float        look_direction[4];
    float        look_target[4];
    float        camera_offset[4];
    float        gravity_up[4];
    update_hero_follow_camera_focus(camera);
    hero_state = &hero;
    focus = orbit + 0x80;
    spring = orbit + 0xE0;
    fast_vec_normalize(camera_offset, ((char *)hero_state->moby) + 0xC0, 1.0f);
    fast_vec_normalize(camera_offset, camera_offset, *(float *)(orbit + 0xE0));
    fast_vec_sub(at, focus, camera_offset);
    fast_vec_normalize(gravity_up, &hero_state->gravity_dir, -1.0f);
    fast_vec_normalize(camera_offset, gravity_up, *(float *)(spring + 0x10));
    fast_vec_add(at, at, camera_offset);
    fast_vec_normalize(target_height_offset, gravity_up, *(float *)(focus + 0x4C));
    fast_vec_add(look_target, target_height_offset, focus);
    fast_vec_sub(look_direction, look_target, at);
    fast_vec_normalize(camera, look_direction, 1.0f);
    fast_vec_cross(up, camera, gravity_up);
    fast_vec_cross(side, up, camera);
    fast_vec_normalize(up, up, 1.0f);
    fast_vec_normalize(side, side, 1.0f);
    qcopy(camera + 0x40, camera);
}

typedef struct {
    Vec4 pos; /* 0x00 */
    u8   pad10[0x20];
    Vec4 side;      /* 0x30 */
    s32  state;     /* 0x40: 5 after a preset reset */
    s32  unk44;     /* 0x44 */
    f32  max_angle; /* 0x48 */
    f32  dist;      /* 0x4C */
    u8   pad50[8];
    f32  unk58; /* 0x58 */
    f32  unk5C; /* 0x5C */
} CameraFocus;

/* View block at orbit + 0x30. */
typedef struct {
    Vec4 vel; /* 0x00 */
    u8   pad10[0x10];
    f32  unk20; /* 0x20 */
    f32  unk24; /* 0x24 */
    s32  unk28; /* 0x28 */
    s16  unk2C; /* 0x2C */
    u8   pad2E[0x12];
    f32  aspect; /* 0x40 */
    u8   pad44[0xC];
} CameraView;

typedef struct {
    Vec4 vel;   /* 0x00 */
    f32  unk10; /* 0x10 */
    u8   pad14[0xC];
    Vec4 accel; /* 0x20 */
    u8   pad30[4];
    f32  unk34; /* 0x34 */
    f32  unk38; /* 0x38 */
    s32  unk3C; /* 0x3C */
    s32  unk40; /* 0x40 */
    u8   pad44[0xC];
} CameraTrack;

/* Orbit state the camera keeps while it circles the hero. */
typedef struct {
    Vec4         axis;  /* 0x00 */
    f32          unk10; /* 0x10: 0.01 unless the preset gives one (FUN_L05_003294c0) */
    f32          unk14; /* 0x14: 0.2 unless the preset gives one */
    s32          unk18; /* 0x18 */
    s16          mode;  /* 0x1C: 0 waiting, 1 orbiting */
    u8           pad1E[0x12];
    CameraView   view;          /* 0x30 */
    CameraFocus  focus;         /* 0x80 */
    CameraSpring spring;        /* 0xE0 */
    CameraTrack  track;         /* 0x100 */
    f32          goal[3];       /* 0x150: yaw, pitch, distance */
    f32          vel[3];        /* 0x15C */
    f32          accel[3];      /* 0x168 */
    f32          damp[3];       /* 0x174 */
    f32          dist;          /* 0x180 */
    f32          spring_dist;   /* 0x184 */
    f32          spring_height; /* 0x188 */
    f32          unk18C;        /* 0x18C */
    f32          unk190;        /* 0x190 */
    f32          unk194;        /* 0x194 */
    u8           pad198[0x18];
    Vec4         unk1B0; /* 0x1B0 */
    Vec4         unk1C0; /* 0x1C0 */
} CameraOrbit;

typedef struct {
    u8           pad0[0x30];
    Vec4         pos; /* 0x30 */
    u8           pad40[0x10];
    f32          yaw;   /* 0x50 */
    f32          pitch; /* 0x54 */
    f32          dist;  /* 0x58 */
    u8           pad5C[0x14];
    CameraOrbit *orbit; /* 0x70 */
    u8           pad74[0xA];
    s16          unk7E; /* 0x7E: cleared after a preset reset */
    u8           pad80[4];
    s16          preset; /* 0x84: row of D_L05_0015EF50 */
} Camera;

/* Camera preset: distances, angle limits and damping. */
typedef struct {
    f32 unk0; /* 0x00 */
    u8  pad4[0x1C];
    f32 unk20;         /* 0x20 */
    f32 unk24;         /* 0x24 */
    f32 spring_dist;   /* 0x28 */
    f32 spring_height; /* 0x2C */
    f32 dist;          /* 0x30 */
    f32 unk34;         /* 0x34 */
    f32 unk38;         /* 0x38 */
    f32 unk3C;         /* 0x3C */
    f32 unk40;         /* 0x40 */
    f32 unk44;         /* 0x44 */
    f32 height_vel;    /* 0x48: spring velocity of the camera height */
} CameraPreset;

typedef struct {
    u8            pad0[0x1C];
    CameraPreset *preset; /* 0x1C */
} CameraPresetRow;

extern CameraPresetRow *D_L05_0015EF50;

/* Resets the camera from its preset (row cam->preset of D_L05_0015EF50): distances,
   angle limits and damping, cleared velocities, then places it. */
void reset_hero_follow_camera_from_preset(Camera *camera_to_reset) __asm__("FUN_L05_003296e0");

void reset_hero_follow_camera_from_preset(Camera *camera_to_reset) {
    CameraPreset *preset = D_L05_0015EF50[camera_to_reset->preset].preset;
    CameraOrbit  *orbit_state;
    CameraFocus  *focus;
    CameraSpring *spring;
    CameraOrbit  *orbit;
    CameraTrack  *track;
    CameraView   *view;

    preset->height_vel = 0;
    orbit_state = camera_to_reset->orbit;
    orbit_state->spring_dist = preset->spring_dist;
    orbit_state->spring_height = preset->spring_height;
    orbit_state->dist = preset->dist;
    orbit_state->unk190 = preset->unk34;
    orbit_state->unk194 = preset->unk38;
    orbit_state->unk18C = preset->unk3C;
    clear_u64_value(&orbit_state->unk1B0);
    clear_u64_value(&orbit_state->unk1C0);
    focus = &camera_to_reset->orbit->focus;
    focus->state = 5;
    focus->unk44 = D_001413D0[0];
    focus->dist = orbit_state->dist;
    focus->max_angle = 0.209439516f;
    focus->unk58 = 0.00174532924f;
    focus->unk5C = -0.0043633231f;
    spring = &camera_to_reset->orbit->spring;
    spring->dist = orbit_state->spring_dist;
    spring->unk4 = 0.5f;
    spring->height = orbit_state->spring_height;
    orbit = camera_to_reset->orbit;
    orbit->unk10 = 0.01f;
    orbit->unk14 = 0.2f;
    orbit->unk18 = 0;
    orbit->mode = 0;
    clear_u64_value(orbit);
    track = &camera_to_reset->orbit->track;
    clear_u64_value(track);
    clear_u64_value(&track->accel);
    track->unk40 = 0;
    track->unk10 = preset->unk0;
    track->unk34 = preset->unk20;
    track->unk38 = preset->unk24;
    track->unk3C = 0;
    view = &camera_to_reset->orbit->view;
    view->unk20 = 0.8f;
    view->unk24 = 0.3f;
    view->aspect = 1.3333334f;
    view->unk28 = 0;
    view->unk2C = 0;
    clear_u64_value(view);
    set_camera_follow_parameters(preset->spring_dist,
                                 preset->spring_height,
                                 preset->dist,
                                 preset->unk34,
                                 preset->unk38,
                                 preset->unk3C,
                                 preset->unk40,
                                 preset->unk44);
    build_hero_follow_camera_transform((char *)camera_to_reset);
    camera_to_reset->unk7E = 0;
}

extern float FUN_001ebd78(void *, float, float, float, float, float);

/* Camera that orbits the hero while hero.vert_ramp_launch is set, easing yaw, pitch and distance toward their goals. */
void update_hero_follow_camera_orbit(Camera *cam) __asm__("FUN_L05_003298d8");

void update_hero_follow_camera_orbit(Camera *hero_follow_camera) {
    Vec4         up;
    Vec4         side;
    Vec4         fwd;
    Vec4         rel;
    Vec4         along;
    Vec4         flat;
    Vec4         unit;
    Vec4         yaw_rotated_forward;
    CameraOrbit *orbit = hero_follow_camera->orbit;
    CameraFocus *focus = &orbit->focus;
    f32         *spring = orbit->goal;
    f32          dot;
    f32          angle_normalization_length;
    f32          initial_yaw;
    f32          pitch;
    f32          tilt;

    fast_vec_normalize(&up, &hero.gravity_dir, -1.0f);
    if (orbit->mode == 0 && hero.vert_ramp_launch != 0) {
        orbit->mode = 1;
        fast_vec_cross(&side, &hero.moby->rot_row2, &up);
        fast_vec_normalize(&side, &side, 1.0f);
        fast_vec_cross(&fwd, &up, &side);
        fast_vec_normalize(&fwd, &fwd, 1.0f);
        fast_vec_sub(&rel, &hero_follow_camera->pos, focus);
        dot = fast_vec_dot(&rel, &up);
        fast_vec_normalize(&along, &up, dot);
        fast_vec_sub(&flat, &rel, &along);
        dot = fast_vec_dot(&fwd, &flat);
        angle_normalization_length = fast_vec_length(&flat);
        if (angle_normalization_length == 0.0f) {
            angle_normalization_length = 0.0001f;
        }
        initial_yaw = 1.5707964f - fast_arc_sin(dot / angle_normalization_length);
        fast_vec_normalize(&unit, &flat, 1.0f);
        if (fast_vec_dot(&side, &unit) < 0.0f) {
            initial_yaw = -initial_yaw;
        }
        hero_follow_camera->yaw = initial_yaw;
        build_look_at_matrix(&yaw_rotated_forward, &fwd, &up, initial_yaw);
        dot = fast_vec_dot(&yaw_rotated_forward, &rel);
        angle_normalization_length = fast_vec_length(&rel);
        if (angle_normalization_length == 0.0f) {
            angle_normalization_length = 0.0001f;
        }
        tilt = 1.5707964f - fast_arc_sin(dot / angle_normalization_length);
        fast_vec_normalize(&unit, &rel, 1.0f);
        pitch = -tilt;
        if (fast_vec_dot(&up, &unit) < 0.0f) {
            pitch = tilt;
        }
        hero_follow_camera->pitch = pitch;
        hero_follow_camera->dist = fast_vec_length(&rel);
        qcopy(&orbit->focus.side, &fwd);
        spring[0] = 3.1415927f;
        spring[1] = -1.5358897f;
        spring[2] = 5.0f;
        spring[3] = 0.0f;
        spring[4] = 0.0f;
        spring[5] = 0.0f;
        spring[6] = 0.005f;
        spring[9] = 0.2f;
        spring[7] = 0.005f;
        spring[10] = 0.2f;
        spring[8] = 0.005f;
        spring[11] = 0.2f;
    }
    if (orbit->mode == 1) {
        fast_vec_normalize(&side, &focus->side, hero_follow_camera->dist);
        build_look_at_matrix(&side, &side, &up, hero_follow_camera->yaw);
        fast_vec_cross(&fwd, &side, &up);
        fast_vec_normalize(&fwd, &fwd, 1.0f);
        build_look_at_matrix(&side, &side, &fwd, hero_follow_camera->pitch);
        fast_vec_add(&hero_follow_camera->pos, focus, &side);
        hero_follow_camera->yaw = rope_spring_turn(
            &spring[3], hero_follow_camera->yaw, spring[0], spring[6], spring[9], 0.0f);
        hero_follow_camera->pitch = rope_spring_turn(
            &spring[4], hero_follow_camera->pitch, spring[1], spring[7], spring[10], 0.0f);
        hero_follow_camera->dist = FUN_001ebd78(
            &spring[5], hero_follow_camera->dist, spring[2], spring[8], spring[11], 0.0f);
        if (hero.vert_ramp_launch == 0) {
            orbit->mode = 0;
            fast_vec_scale(orbit, orbit, -1.0f);
        }
    }
}
/* Camera follow: springs the position toward the focus, keeps clear of walls, eases distance and height, then rebuilds the basis. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032B1F8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_32B1F8;

typedef struct {
    V_32B1F8 pos;
    char     pad10[0x10];
    V_32B1F8 axis;
    char     pad30[0x1C];
    float    dist;
} Focus;

typedef struct {
    float dist;
    char  pad4[0xC];
    float height;
} Spring;

typedef struct {
    V_32B1F8 vel;
    char     pad10[0x10];
    float    state[4];
    char     pad30[4];
    float    k34, k38, k3C;
    float    blend;
} Track;

typedef struct {
    float    vel[4];
    float    k10, k14, k18;
    short    mode;
    char     pad1E[0x62];
    Focus    focus;
    char     padD0[0x10];
    Spring   spring;
    char     padF4[0xC];
    Track    track;
    char     pad150[0x30];
    float    dist;
    float    spring_dist;
    float    spring_height;
    float    focus_dist;
    float    spring_dist2;
    float    spring_height2;
    char     pad198[8];
    V_32B1F8 aim;
    V_32B1F8 bias;
    float    bias_vel[4];
    V_32B1F8 next;
} Orbit_32B1F8;

typedef struct {
    V_32B1F8      fwd;
    V_32B1F8      up;
    V_32B1F8      side;
    V_32B1F8      pos;
    char          pad40[0x24];
    float         vel[3];
    Orbit_32B1F8 *orbit;
    char          pad74[0x10];
    short         preset;
} Camera_32B1F8;

extern int              coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern char             D_0013E633_32B1F8[] __asm__("D_0013F5E0");
extern CameraPresetRow *D_L05_0015EF50_macro __asm__("D_L05_0015EF50") MACRO_ADDR;
extern float            D_L05_001621A8_gp __asm__("D_L05_001621A8") __attribute__((sda));
extern float cam_interp_values(float, float, float *, float, float, float) __asm__("FUN_001ebd78");

extern float D_L05_00174278 NOT_SDA;
void update_orbit_camera_position(Camera_32B1F8 *camera) __asm__("FUN_L05_00329ce8");

void update_orbit_camera_position(Camera_32B1F8 *camera) {
    V_32B1F8      down;
    V_32B1F8      focus;
    V_32B1F8      rel;
    V_32B1F8      lateral;
    V_32B1F8      radial;
    V_32B1F8      base;
    V_32B1F8      goal;
    V_32B1F8      offset;
    V_32B1F8      probe;
    V_32B1F8      step;
    Orbit_32B1F8 *orbit = camera->orbit;
    char         *up_axis = D_0013E633_32B1F8;
    CameraPreset *preset;
    Focus        *focus_state;
    Track        *track;
    float         focus_distance;
    Spring       *spring;
    float        *vel;
    float         along;
    V_32B1F8     *pos;
    int           probe_step;
    int           orbiting = 0;

    preset = D_L05_0015EF50_macro[camera->preset].preset;
    track = &orbit->track;
    focus_distance = orbit->dist;
    focus_state = &orbit->focus;
    spring = &orbit->spring;
    fast_vec_normalize(&down, up_axis, -1.0f);
    qcopy(&focus, &focus_state->pos);
    if (orbit->mode == 0) {
        fast_vec_sub(&rel, pos = &camera->pos, &focus);
        probe_step = 0;
        along = fast_vec_dot(&rel, &focus_state->axis);
        fast_vec_normalize(&offset, &focus_state->axis, along);
        fast_vec_sub(&lateral, &rel, &offset);
        fast_vec_normalize(&radial, &lateral, spring->dist);
        fast_vec_add(&base, &focus, &radial);
        fast_vec_normalize(&offset, &focus_state->axis, spring->height);
        fast_vec_add(&goal, &base, &offset);
        fast_vec_add(&goal, &goal, &orbit->bias);
        camera->pos.x = cam_interp_values(
            camera->pos.x, goal.x, &orbit->vel[0], orbit->k10, orbit->k14, orbit->k18);
        camera->pos.y = cam_interp_values(
            camera->pos.y, goal.y, &orbit->vel[1], orbit->k10, orbit->k14, orbit->k18);
        camera->pos.z = cam_interp_values(
            camera->pos.z, goal.z, &orbit->vel[2], orbit->k10, orbit->k14, orbit->k18);
        vel = camera->vel;
        probe.x = vel[0];
        probe.y = vel[1];
        probe.z = vel[2];
        *(int *)&probe.w = 0;
        fast_vec_sub(&step, pos, &probe);
        fast_vec_scale(&step, &step, 0.16666667f);
        while (probe_step < 6) {
            if (coll_sphere(0.5f, &probe, 18, 0) != 0) {
                spring->height = cam_interp_values(spring->height,
                                                   D_L05_00174278 - focus.z,
                                                   &preset->height_vel,
                                                   0.05f,
                                                   0.2f,
                                                   0.0f);
                break;
            }
            probe_step++;
            fast_vec_add(&probe, &probe, &step);
        }
        if (probe_step == 6) {
            orbit->bias.x =
                cam_interp_values(orbit->bias.x, 0.0f, &orbit->bias_vel[0], 0.02f, 0.2f, 0.05f);
            orbit->bias.y =
                cam_interp_values(orbit->bias.y, 0.0f, &orbit->bias_vel[1], 0.02f, 0.2f, 0.05f);
            orbit->bias.z =
                cam_interp_values(orbit->bias.z, 0.0f, &orbit->bias_vel[2], 0.02f, 0.2f, 0.05f);
            spring->height = cam_interp_values(
                spring->height, orbit->spring_height, &preset->height_vel, 0.005f, 0.2f, 0.0f);
        }
        if (((Hero *)(D_0013E633_32B1F8 - 0x290))->air_frames.s != 0 &&
            2.0f < ((Hero *)(D_0013E633_32B1F8 - 0x290))->ground_distance) {
            if (absolute_float(spring->dist - orbit->spring_dist2) < 0.015f)
                spring->dist = orbit->spring_dist2;
            else if (spring->dist < orbit->spring_dist2)
                spring->dist = spring->dist + 0.015f;
            else
                spring->dist = spring->dist - 0.015f;
            if (probe_step == 6) {
                if (absolute_float(spring->height - orbit->spring_height2) < 0.05f)
                    spring->height = orbit->spring_height2;
                else if (spring->height < orbit->spring_height2)
                    spring->height = spring->height + 0.05f;
                else
                    spring->height = spring->height - 0.05f;
            }
            if (absolute_float(focus_state->dist - orbit->focus_dist) < 0.02f)
                focus_state->dist = orbit->focus_dist;
            else if (focus_state->dist < orbit->focus_dist)
                focus_state->dist = focus_state->dist + 0.02f;
            else
                focus_state->dist = focus_state->dist - 0.02f;
        } else {
            if (absolute_float(spring->dist - orbit->spring_dist) < 0.03f)
                spring->dist = orbit->spring_dist;
            else if (spring->dist < orbit->spring_dist)
                spring->dist = spring->dist + 0.03f;
            else
                spring->dist = spring->dist - 0.03f;
            if (absolute_float(spring->height - orbit->spring_height) < 0.1f)
                spring->height = orbit->spring_height;
            else if (spring->height < orbit->spring_height)
                spring->height = spring->height + 0.1f;
            else
                spring->height = spring->height - 0.1f;
            if (absolute_float(focus_state->dist - focus_distance) < 0.04f)
                focus_state->dist = focus_distance;
            else if (focus_state->dist < focus_distance)
                focus_state->dist = focus_state->dist + 0.04f;
            else
                focus_state->dist = focus_state->dist - 0.04f;
        }
        orbiting = 1;
    } else {
        qcopy(&orbit->aim, *(char **)(up_axis + 0x1DF0) + 0xE0);
        if (absolute_float(spring->dist - orbit->spring_dist) < 0.015f)
            spring->dist = orbit->spring_dist;
        else if (spring->dist < orbit->spring_dist)
            spring->dist = spring->dist + 0.015f;
        else
            spring->dist = spring->dist - 0.015f;
        if (absolute_float(spring->height - orbit->spring_height) < 0.05f)
            spring->height = orbit->spring_height;
        else if (spring->height < orbit->spring_height)
            spring->height = spring->height + 0.05f;
        else
            spring->height = spring->height - 0.05f;
        if (absolute_float(focus_state->dist - focus_distance) < 0.02f)
            focus_state->dist = focus_distance;
        else if (focus_state->dist < focus_distance)
            focus_state->dist = focus_state->dist + 0.02f;
        else
            focus_state->dist = focus_state->dist - 0.02f;
    }
    update_hero_follow_camera_orbit(camera);
    qcopy(&focus_state->pos, &orbit->next);
    qcopy(&focus, &focus_state->pos);
    fast_vec_normalize(&offset, &focus_state->axis, focus_state->dist);
    fast_vec_add(&goal, &focus, &offset);
    fast_vec_sub(&rel, &goal, &camera->pos);
    fast_vec_normalize(&rel, &rel, 1.0f);
    along = fast_vec_dot(&camera->fwd, &rel);
    fast_vec_cross(&camera->up, &rel, &down);
    fast_vec_normalize(&camera->up, &camera->up, 1.0f);
    fast_vec_cross(&camera->side, &camera->up, &rel);
    if (orbiting == 0 || absolute_float(along) < 0.5f ||
        (vel = (float *)(D_0013E633_32B1F8 - 0x290), ((Hero *)vel)->trick_input != 0)) {
        track->blend =
            cam_interp_values(track->blend, 0.0f, track->state, track->k34, track->k38, track->k3C);
    } else {
        float tilt;
        fast_vec_normalize(&probe, ((char *)((Hero *)vel)->moby) + 0xC0, 1.0f);
        tilt = fast_vec_dot(&probe, &camera->side);
        fast_vec_normalize(&step, &camera->side, tilt);
        fast_vec_sub(&probe, &probe, &step);
        tilt = fast_vec_dot(&probe, &camera->up);
        track->blend = cam_interp_values(track->blend,
                                         tilt * D_L05_001621A8_gp,
                                         track->state,
                                         track->k34,
                                         track->k38,
                                         track->k3C);
    }
    if (0.00001f < absolute_float(track->blend)) {
        fast_vec_normalize(&probe, &camera->up, track->blend);
        fast_vec_add(&goal, &goal, &probe);
    }
    fast_vec_sub(&rel, &goal, &camera->pos);
    fast_vec_normalize(&camera->fwd, &rel, 1.0f);
    fast_vec_cross(&camera->up, &camera->fwd, &down);
    fast_vec_normalize(&camera->up, &camera->up, 1.0f);
    fast_vec_cross(&camera->side, &camera->up, &camera->fwd);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032BBF0), where it is exact; names translated to the US level program. */

extern void reset_hero_follow_camera_from_preset(Camera *) __asm__("FUN_L05_003296e0");

extern char *D_L05_00167204 NOT_SDA;
void reset_camera_from_source_transform(char *out) __asm__("FUN_L05_0032a6e0");

void reset_camera_from_source_transform(char *out) {
    char *source_camera_transform = D_L05_00167204;
    qcopy(out + 0x30, source_camera_transform + 0x30);
    qcopy(out, source_camera_transform);
    qcopy(out + 0x10, source_camera_transform + 0x10);
    qcopy(out + 0x20, source_camera_transform + 0x20);
    extract_matrix_rotation_quaternion(out + 0x40, out);
    reset_hero_follow_camera_from_preset((Camera *)out);
}

float                D_L16_00162198 = 2.0f;
float D_L16_00174278 NOT_SDA = 0;
