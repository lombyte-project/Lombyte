/* Camera type 3: follows a path beside the grind rail the hero rides and turns toward a look-at point on it; its setup and mode check. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/overlay/quad.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/shared/camera/init_camera_system.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/camera/camera_spring.h"
#include "sda.h"

/* Camera blocks FUN_L01_00314e98 resets (camera + 0x70 and its preset row in D_L01_0015EF50) */
typedef struct {
    Vec4 pos; /* 0x00 */
    u8   pad10[0x30];
    s32  state; /* 0x40 */
    u8   pad44[4];
    f32  max_angle; /* 0x48 */
    f32  dist;      /* 0x4C */
    u8   pad50[0x10];
} CameraFocus;

typedef struct {
    Vec4 vel; /* 0x00 */
    u8   pad10[0x10];
    f32  unk20; /* 0x20 */
    f32  unk24; /* 0x24 */
    f32  unk28; /* 0x28 */
    s16  unk2C; /* 0x2C */
    u8   pad2E[0x22];
} CameraView;

typedef struct {
    Vec4 vel; /* 0x00 */
    u8   pad10[0x10];
    Vec4 accel; /* 0x20 */
    f32  unk30; /* 0x30 */
    f32  unk34; /* 0x34 */
    f32  unk38; /* 0x38 */
    s32  unk3C; /* 0x3C */
    f32  unk40; /* 0x40 */
    u8   pad44[0xC];
} CameraTrack;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} CameraBlend;

typedef struct {
    Vec4         axis;  /* 0x00 */
    f32          unk10; /* 0x10 */
    f32          unk14; /* 0x14 */
    s32          unk18; /* 0x18 */
    s16          mode;  /* 0x1C */
    u8           pad1E[2];
    f32          unk20; /* 0x20 */
    s32          unk24; /* 0x24 */
    u8           pad28[8];
    CameraView   view;   /* 0x30 */
    CameraFocus  focus;  /* 0x80 */
    CameraSpring spring; /* 0xE0 */
    CameraTrack  track;  /* 0x100 */
    CameraBlend  blend;  /* 0x150 */
} CameraOrbit;

typedef struct {
    u8  padA[0xA];
    s16 mode; /* 0x7E */
} CameraControl;

typedef struct {
    u8            pad0[0x70];
    CameraOrbit  *orbit;   /* 0x70 */
    CameraControl control; /* 0x74 */
    u8            pad80[4];
    s16           preset; /* 0x84: row of D_L01_0015EF50 */
} Camera;

/* A camera path: the point count, then 16-byte points */
typedef struct {
    s32  count;
    u8   pad4[0xC];
    Vec4 pts[1]; /* 0x10 */
} CameraPath;

typedef struct {
    f32 lead; /* 0x00 */
    u8  pad4[0x1C];
    s32 track_path; /* 0x20: D_L01_001B0930 path the first path is measured along */
    s32 target;     /* 0x24: row of l01_splines */
    s32 path;       /* 0x28: D_L01_001B0930 path */
    s32 look_path;  /* 0x2C: D_L01_001B0930 path measured along the target's path */
    s32 kind;       /* 0x30 */
    s16 ready;      /* 0x34: paths prepared */
    s16 unk36;      /* 0x36: 1 when the camera starts on the far side of a kind 3 path */
    f32 unk38;      /* 0x38 */
} CameraPreset;

typedef struct {
    u8            pad0[0x1C];
    CameraPreset *preset; /* 0x1C */
} CameraPresetRow;

extern CameraPresetRow *D_L01_0015EF50;
extern s32              FUN_L00_0025df68(void *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);

/* Resets the camera's orbit, focus, spring, view and track from fixed values; for a kind 2 preset it
   also prepares its two paths once, measuring each point against the path it follows. */
void reset_camera_state_and_prepare_preset_paths(Camera *destination_camera) __asm__("FUN_L01_00314e98");

void reset_camera_state_and_prepare_preset_paths(Camera *destination_camera) {
    Vec4           at;
    s32            seg;
    f32            path_fraction_or_length;
    CameraPreset  *preset = D_L01_0015EF50[destination_camera->preset].preset;
    CameraControl *control;
    CameraFocus   *focus;
    CameraSpring  *spring;
    CameraOrbit   *orbit;
    CameraView    *view;
    CameraTrack   *track;
    CameraBlend   *blend;
    CameraPath    *preset_path, *track_path, *look_path, *target_path;
    s32            i, j, k, segment_count, previous_path_node_index;

    focus = &destination_camera->orbit->focus;
    focus->dist = 1.5f;
    focus->state = 0;
    if (preset->kind == 3) {
        focus->dist = 0.5f;
    }
    control = &destination_camera->control;
    focus->max_angle = 0.20943952f;
    spring = &destination_camera->orbit->spring;
    spring->dist = 6.0f;
    spring->unk4 = 0.5f;
    spring->height = 2.0f;
    orbit = destination_camera->orbit;
    orbit->unk10 = 0.01f;
    orbit->unk14 = 0.175f;
    orbit->unk18 = 0;
    orbit->mode = 0;
    orbit->unk20 = 0.0005f;
    orbit->unk24 = 0;
    clear_u64_value(orbit);
    view = &destination_camera->orbit->view;
    view->unk20 = 0.01f;
    view->unk24 = 0.3f;
    view->unk28 = 0.2f;
    view->unk2C = 0;
    clear_u64_value(view);
    track = &destination_camera->orbit->track;
    clear_u64_value(track);
    clear_u64_value(&track->accel);
    track->unk40 = 3.0f;
    track->unk30 = 0.003f;
    track->unk34 = 0.1f;
    track->unk38 = 0.2f;
    track->unk3C = 0;
    blend = &destination_camera->orbit->blend;
    blend->unk0 = 0;
    blend->unk4 = 0;
    blend->unk8 = 0;
    blend->unkC = 0;
    blend->unk10 = 1;
    if (preset->kind == 2 && preset->ready == 0) {
        track_path = ((CameraPath *)l01_paths[preset->track_path]);
        target_path = (CameraPath *)l01_splines[preset->target].path;
        preset_path = ((CameraPath *)l01_paths[preset->path]);
        look_path = ((CameraPath *)l01_paths[preset->look_path]);
        preset->ready = 1;
        for (i = 0; i < preset_path->count; i++) {
            FUN_L00_0025df68(track_path,
                             &preset_path->pts[i],
                             &at,
                             &seg,
                             &path_fraction_or_length,
                             0,
                             20.0f,
                             5.0f,
                             0.0f);
            preset_path->pts[i].f[0] = preset_path->pts[i].f[3];
            preset_path->pts[i].f[3] = seg;
        }
        previous_path_node_index = preset_path->count - 1;
        for (i = 0; i < preset_path->count; i++) {
            j = truncate_float_to_s32(preset_path->pts[previous_path_node_index].f[3]);
            segment_count = truncate_float_to_s32(preset_path->pts[i].f[3]) - j;
            if (segment_count < 0) {
                segment_count += track_path->count;
            }
            path_fraction_or_length = 0.0f;
            for (k = 0; k < segment_count; k++) {
                path_fraction_or_length += track_path->pts[(j + i) % track_path->count].f[3];
            }
            preset_path->pts[i].f[1] = path_fraction_or_length;
            previous_path_node_index = i;
        }
        for (i = 0; i < look_path->count; i++) {
            FUN_L00_0025df68(target_path,
                             &look_path->pts[i],
                             &at,
                             &seg,
                             &path_fraction_or_length,
                             0,
                             20.0f,
                             5.0f,
                             0.0f);
            look_path->pts[i].f[0] = look_path->pts[i].f[3];
            look_path->pts[i].f[3] = seg;
        }
        previous_path_node_index = look_path->count - 1;
        for (i = 0; i < look_path->count; i++) {
            j = truncate_float_to_s32(look_path->pts[previous_path_node_index].f[3]);
            segment_count = truncate_float_to_s32(look_path->pts[i].f[3]) - j;
            if (segment_count < 0) {
                segment_count += target_path->count;
            }
            path_fraction_or_length = 0.0f;
            for (k = 0; k < segment_count; k++) {
                path_fraction_or_length += target_path->pts[(j + i) % target_path->count].f[3];
            }
            look_path->pts[i].f[1] = path_fraction_or_length;
            previous_path_node_index = i;
        }
    }
    control->mode = 0;
}

/* Update function of camera 3: follows a path beside the hero's and turns toward a look-at point on it.
   The four `= 0` initialisers in the mode 2 block are dead and leave no code: they make up for the insns
   qcopy()'s parameters add, which decide the size of gcse's hash table and so the order of the spill slots. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00316730), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16))) Node_316730;

typedef struct {
    int         n;
    int         pad[3];
    Node_316730 nodes[1];
} Path;

typedef struct {
    char  p00[0x20];
    float unk20;
    char  p24[0x10];
    float unk34;
    char  p38[0x18];
} SubA;

typedef struct {
    float pos[4];
    char  p10[0x3C];
    float unk4C;
    char  p50[0x80];
} SubB;

typedef struct {
    int   idx;
    float t;
    int   p08[2];
    int   unk10;
} SubC;

typedef struct {
    float vel[3];
    float unk0C;
    float unk10;
    char  p14[0x1C];
    SubA  a;
    SubB  b;
    SubC  c;
} Data;

typedef struct {
    float unk0[4];
    float unk10[4];
    float unk20[4];
    float pos[4];
    float unk40[4];
    char  p50[0x20];
    Data *data;
    char  p74[0x10];
    short unk84;
} Cam;

extern CameraPresetRow *D_L01_0015EF50_macro __asm__("D_L01_0015EF50") MACRO_ADDR;
extern LevelSpline     *l01_splines_macro __asm__("D_L01_0015F70C") MACRO_ADDR;
extern float            camera_basis_reference_direction[] __asm__("D_L01_001672B0");
extern float            cam_interp_values(float a, float b, float *p, float c, float d, float e);
extern int sample_path(void *, void *, int *, float *, float, int) __asm__("FUN_L00_0025d808");
extern int current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;

void update_camera_position_and_orientation_from_paths(Cam *cam) __asm__("FUN_L01_00315358");

void update_camera_position_and_orientation_from_paths(Cam *camera_to_update) {
    float         camera_goal[4];
    float         look_offset[4];
    float         look_dir[4];
    float         path_point[4];
    float         look_target[4];
    float         delta[4];
    float         rail_point[4];
    int           segment;
    float         fraction;
    Data         *data = camera_to_update->data;
    SubA         *look = &data->a;
    SubB         *follow = &data->b;
    SubC         *progress = &data->c;
    CameraPreset *params = D_L01_0015EF50_macro[camera_to_update->unk84].preset;
    float         rate = 0.01f;
    int           mode = params->kind;
    int           path_index = params->track_path;
    float         lead = params->lead;
    float         pitch_error;
    float         look_length;
    float         pitch_step;

    if (mode == 0) {
        Path *path;
        float dist;

        segment = ((Hero *)(((char *)((char *)&hero))))->rail_segment;
        fraction = ((Hero *)(((char *)((char *)&hero))))->rail_seg_frac;
        progress->t = fraction;
        progress->idx = segment;
        sample_path(
            ((Path *)l01_paths[path_index]), camera_goal, &progress->idx, &progress->t, 1.0f, 0);
        qcopy(follow->pos, camera_goal);
        path = ((Path *)l01_paths[path_index]);
        dist = path->nodes[segment].w *
               (fraction /
                ((Path *)((Hero *)(((char *)((char *)&hero))))->rail_spline)->nodes[segment].w);
        fast_vec_sub(delta, &path->nodes[segment + 1], &path->nodes[segment]);
        fast_vec_normalize(look_target, delta, dist);
        fast_vec_add(rail_point, &path->nodes[segment], look_target);
        camera_to_update->pos[0] = cam_interp_values(
            camera_to_update->pos[0], rail_point[0], &data->vel[0], rate, 0.175f, 0.0f);
        camera_to_update->pos[1] = cam_interp_values(
            camera_to_update->pos[1], rail_point[1], &data->vel[1], rate, 0.175f, 0.0f);
        camera_to_update->pos[2] = cam_interp_values(
            camera_to_update->pos[2], rail_point[2], &data->vel[2], rate, 0.175f, 0.0f);
    } else if (mode == 2) {
        Path *pathA = ((Path *)l01_paths[path_index]);
        Path *pathD = (Path *)l01_splines_macro[params->target].path;
        Path *pathC = ((Path *)l01_paths[params->path]);
        Path *pathB = ((Path *)l01_paths[params->look_path]);
        int   i;
        int   previous_path_node_index = 0;
        int   range_start = 0;
        int   range_end = 0;
        int   j = 0;
        int   count;
        int   base;
        int   node;
        float node_fraction;
        float sum;
        float hero_length;
        float camera_length;
        float speed_scale;
        float dist;

        previous_path_node_index = pathB->n - 1;
        for (i = 0; i < pathB->n; i++) {
            range_start = truncate_float_to_s32(pathB->nodes[previous_path_node_index].w);
            range_end = truncate_float_to_s32(pathB->nodes[i].w);
            if (range_end < range_start) {
                if (((Hero *)(((char *)((char *)&hero))))->rail_segment >= range_start ||
                    ((Hero *)(((char *)((char *)&hero))))->rail_segment < range_end) {
                    break;
                }
            } else {
                if (((Hero *)(((char *)((char *)&hero))))->rail_segment >= range_start &&
                    ((Hero *)(((char *)((char *)&hero))))->rail_segment < range_end) {
                    break;
                }
            }
            previous_path_node_index = i;
        }
        progress->unk10 = i;
        sum = 0.0f;
        j = (i - 1 + pathB->n) % pathB->n;
        hero_length = pathB->nodes[i].y;
        camera_length = pathC->nodes[i].y;
        if (pathB->nodes[j].x != 0.0f) {
            lead = pathB->nodes[j].x;
        }
        base = truncate_float_to_s32(pathB->nodes[j].w);
        count = ((Hero *)(((char *)((char *)&hero))))->rail_segment - base;
        if (count < 0) {
            count += pathD->n;
        }
        for (i = 0; i < count; i++) {
            sum += pathD->nodes[(base + i) % pathD->n].w;
        }
        sum += ((Hero *)(((char *)((char *)&hero))))->rail_seg_frac;
        dist = camera_length * (sum / hero_length);
        node = truncate_float_to_s32(pathC->nodes[j].w);
        node_fraction = 0.0f;
        sample_path(pathA, look_target, &node, &node_fraction, dist, 1);
        speed_scale = pathC->nodes[j].x;
        if (speed_scale <= 0.0f) {
            speed_scale = 0.01f;
        } else {
            rate *= speed_scale;
            speed_scale *= 0.01f;
        }
        if (absolute_float(data->unk10 - speed_scale) < 0.00015f) {
            data->unk10 = speed_scale;
        } else if (data->unk10 < speed_scale) {
            data->unk10 = data->unk10 + 0.00015f;
        } else {
            data->unk10 = data->unk10 - 0.00015f;
        }
        camera_to_update->pos[0] = cam_interp_values(
            camera_to_update->pos[0], look_target[0], &data->vel[0], data->unk10, 0.175f, 0.0f);
        camera_to_update->pos[1] = cam_interp_values(
            camera_to_update->pos[1], look_target[1], &data->vel[1], data->unk10, 0.175f, 0.0f);
        camera_to_update->pos[2] = cam_interp_values(
            camera_to_update->pos[2], look_target[2], &data->vel[2], data->unk10, 0.175f, 0.0f);
    } else {
        Path *path = ((Path *)l01_paths[path_index]);

        if (params->target < 0) {
            float lead_offset;

            FUN_L00_0025df68(path,
                             ((Hero *)(((char *)((char *)&hero))))->motion.pos.f,
                             path_point,
                             &segment,
                             &fraction,
                             0,
                             20.0f,
                             1.0f,
                             0.0f);
            if (params->unk36 == 1) {
                lead_offset = -(params->unk38 * 4.0f);
            } else {
                lead_offset = params->unk38;
            }
            sample_path(path, path_point, &segment, &fraction, lead_offset, 1);
            qcopy(camera_goal, path_point);
        } else {
            int   hero_segment;
            float hero_fraction;

            hero_segment = ((Hero *)(((char *)((char *)&hero))))->rail_segment;
            hero_fraction = ((Hero *)(((char *)((char *)&hero))))->rail_seg_frac;
            sample_path((Path *)((Hero *)(((char *)((char *)&hero))))->rail_spline,
                        path_point,
                        &hero_segment,
                        &hero_fraction,
                        params->unk38,
                        1);
            FUN_L00_0025df68(
                path, path_point, camera_goal, &segment, &fraction, 0, 20.0f, 1.0f, 0.0f);
        }
        camera_to_update->pos[0] = cam_interp_values(
            camera_to_update->pos[0], camera_goal[0], &data->vel[0], rate, 0.175f, 0.0f);
        camera_to_update->pos[1] = cam_interp_values(
            camera_to_update->pos[1], camera_goal[1], &data->vel[1], rate, 0.175f, 0.0f);
        camera_to_update->pos[2] = cam_interp_values(
            camera_to_update->pos[2], camera_goal[2], &data->vel[2], rate, 0.175f, 0.0f);
    }
    if (params->target < 0) {
        Path *path = ((Path *)l01_paths[params->track_path]);

        FUN_L00_0025df68(path,
                         ((Hero *)(((char *)((char *)&hero))))->motion.pos.f,
                         look_target,
                         &segment,
                         &fraction,
                         0,
                         20.0f,
                         1.0f,
                         0.0f);
        if (current_level_index_macro == 0xE && params->kind == 0) {
            sample_path(path, delta, &segment, &fraction, lead, 0);
        } else {
            sample_path(path, delta, &segment, &fraction, lead, 1);
        }
        fast_vec_sub(delta, delta, look_target);
        fast_vec_add(look_target, ((Hero *)(((char *)((char *)&hero))))->motion.pos.f, delta);
    } else {
        int   hero_segment;
        float hero_fraction;

        hero_segment = ((Hero *)(((char *)((char *)&hero))))->rail_segment;
        hero_fraction = ((Hero *)(((char *)((char *)&hero))))->rail_seg_frac;
        sample_path((Path *)((Hero *)(((char *)((char *)&hero))))->rail_spline,
                    look_target,
                    &hero_segment,
                    &hero_fraction,
                    lead,
                    1);
    }
    fast_vec_normalize(look_offset, camera_basis_reference_direction, follow->unk4C);
    fast_vec_add(path_point, look_offset, look_target);
    fast_vec_sub(look_dir, path_point, camera_to_update->pos);
    look_length = fast_vec_length(look_dir);
    if (look_length != 0.0f) {
        fast_vec_scale(look_dir, look_dir, 1.0f / look_length);
        pitch_error = 1.5707964f - fast_arc_sin(fast_vec_dot(look_dir, camera_to_update->unk0));
    } else {
        pitch_error = 0.0f;
    }
    if (absolute_float(look->unk20 - rate) < 0.00015f) {
        look->unk20 = rate;
    } else if (look->unk20 < rate) {
        look->unk20 = look->unk20 + 0.00015f;
    } else {
        look->unk20 = look->unk20 - 0.00015f;
    }
    pitch_step = rope_spring_turn(&look->unk34, 0.0f, pitch_error, look->unk20, 0.175f, 0.0f);
    fast_vec_cross(delta, look_dir, camera_to_update->unk40);
    build_look_at_matrix(camera_to_update->unk0, camera_to_update->unk40, delta, pitch_step);
    fast_vec_normalize(camera_to_update->unk40, camera_to_update->unk0, 1.0f);
    fast_vec_normalize(camera_to_update->unk0, camera_to_update->unk0, 1.0f);
    fast_vec_cross(
        camera_to_update->unk10, camera_to_update->unk0, camera_basis_reference_direction);
    fast_vec_normalize(camera_to_update->unk10, camera_to_update->unk10, 1.0f);
    fast_vec_cross(camera_to_update->unk20, camera_to_update->unk10, camera_to_update->unk0);
}

extern Camera *D_L01_00167284 NOT_SDA;
extern OvlVec4                D_0013F3D0_315de0 __asm__("D_0013F3D0");

void initialize_camera_transform_and_paths(Camera *destination_camera) __asm__("FUN_L01_00315de0");

void initialize_camera_transform_and_paths(Camera *destination_camera) {
    CameraPresetRow *rows = D_L01_0015EF50;
    Camera          *source_camera = D_L01_00167284;
    CameraPreset    *camera_preset = rows[destination_camera->preset].preset;
    OvlVec4          at, delta, to_camera;
    s32              seg;
    f32              path_fraction;

    qcopy((char *)destination_camera + 0x30, (char *)source_camera + 0x30);
    qcopy(destination_camera, source_camera);
    qcopy((char *)destination_camera + 0x10, (char *)source_camera + 0x10);
    qcopy((char *)destination_camera + 0x20, (char *)source_camera + 0x20);
    qcopy((char *)destination_camera + 0x40, destination_camera);

    if (camera_preset->track_path >= 0) {
        CameraPath *path;
        s32         i;
        s32         next;
        path = ((CameraPath *)l01_paths[camera_preset->track_path]);
        for (i = 0; i < path->count; i++) {
            next = (i + 1) % path->count;
            path->pts[i].f[3] = fast_vec_dist(&path->pts[i], &path->pts[next]);
        }
    }

    camera_preset->unk36 = 0;
    if (camera_preset->kind == 3) {
        CameraPath *path;
        s32         next;
        path = ((CameraPath *)l01_paths[camera_preset->track_path]);
        seg = 0;
        path_fraction = 0.0f;
        if (FUN_L00_0025df68(
                path, &D_0013F3D0_315de0, &at, &seg, &path_fraction, 0, 20.0f, 1.0f, 0.0f)) {
            s32 count = path->count;
            next = seg + 1;
            if (next >= count)
                next = count - 1;
            fast_vec_sub(&delta, &path->pts[next], &path->pts[seg]);
            if (fast_vec_length(&delta) != 0.0f && next != seg) {
                fast_vec_normalize(&delta, &delta, 1.0f);
                fast_vec_sub(&to_camera, (char *)destination_camera + 0x30, &D_0013F3D0_315de0);
                if (0.0f < fast_vec_dot(&to_camera, &delta))
                    camera_preset->unk36 = 1;
            }
        }
    }
    reset_camera_state_and_prepare_preset_paths(destination_camera);
}

/* The object FUN_L01_00316030 updates */
typedef struct {
    u8  pad00[0x7E];
    s16 mode; /* 0x7E: set to 3 or 5 */
    u8  pad80[4];
    s16 slot;  /* 0x84: index into D_L01_0015EF50 */
    s16 unk86; /* 0x86: compared with hero.unk2284 */
} CamUser;

void update_camera_mode_from_preset(CamUser *camera_user) __asm__("FUN_L01_00316030");

void update_camera_mode_from_preset(CamUser *camera_user) {
    CameraPreset *camera_preset = D_L01_0015EF50[camera_user->slot].preset;

    if (hero.unk2284 == camera_user->unk86) {
        if (camera_preset->target < 0) {
            return;
        }
        if (hero.rail_spline == (s32)l01_splines[camera_preset->target].path &&
            hero.rail_off_frames == 0) {
            return;
        }
    }
    if (camera_preset->kind == 3) {
        camera_user->mode = 5;
    } else {
        camera_user->mode = 3;
    }
}

s32          D_L05_0015F71C = 0;
LevelSpline *l08_splines __asm__("D_L08_0015F70C") = 0;
s32          D_L16_0015F71C = 0;
LevelSpline *l17_splines __asm__("D_L17_0015F70C") = 0;
