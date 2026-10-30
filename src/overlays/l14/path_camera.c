/* Level 14 path camera type (camera 20): flies along one path looking along another or at an anchor moby, with field-of-view stops; setup, update and priority check. */
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/view/update_view_context.h"
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/camera/update_cam.h"

typedef struct {
    char  pad0[0xB0];
    float fB0; /* tan of half the field of view */
} VendorView_5290;

extern VendorView_5290 l14_camera_projection_state __asm__("D_L14_0016D140");

#include "rnc/overlay/l14/path_camera_globals.h"

extern int sample_path(char *path, float *out, void *pos, void *t, int dir, float step) __asm__("FUN_L00_0025d808");

extern char *l14_camera_preset_rows __asm__("D_L14_0015EF50");

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

typedef struct {
    s16 seg[8];
    f32 fov[8];
    f32 fov_end;
} Stops_4f00;

void setup_path_camera(char *camera) __asm__("FUN_L14_00314f00");

void setup_path_camera(char *camera) {
    u128        stand;
    char       *rec = *(char **)(l14_camera_preset_rows + *(short *)(camera + 0x84) * 32 + 0x1C);
    char       *camera_saved_state;
    char       *fov_stops_state_base;
    char       *path;
    char       *initial_camera_transform;
    int        *camera_run;
    int        *w;
    short      *s;
    Stops_4f00 *fov_stops;
    float      *wf;
    short      *ss;
    float       segment_length;
    int         i;
    int         fov_stop_count;

    (*&rec)[0x38] = 3;
    camera_saved_state = *(char **)(camera + 0x70);
    *(int *)(camera_saved_state + 0x40) = 0;
    camera_run = (int *)(camera_saved_state + 0x40);
    camera_run[1] = 0;
    camera_run[2] = 0;
    camera_run[3] = 0;
    camera_run[4] = 0;
    *(short *)(camera_run + 5) = 0;
    *((short *)(camera_run + 5) + 1) = scale_game_frames(*(short *)(rec + 0x3A));
    fov_stops_state_base = *(char **)(camera + 0x70);
    fov_stops = (Stops_4f00 *)(fov_stops_state_base + 0x58);
    {
        int k;
        for (k = 0; k < 8; k++) {
            fov_stops->fov[k] = 0.0f;
            fov_stops->seg[k] = -1;
        }
    }
    fov_stops->fov_end = fast_arc_tan(1.0f, l14_camera_projection_state.fB0) * 2.0f;
    if (*(int *)(rec + 0x20) >= 0) {
        path = ((char *)l14_paths[*(int *)(rec + 0x20)]);
        fov_stop_count = 0;
        for (i = 0; i < *(int *)path; i++) {
            char *seg = path + i * 16;
            if (0.0f < *(float *)(seg + 0x1C)) {
                fov_stops->fov[fov_stop_count] = *(float *)(seg + 0x1C);
                fov_stops->seg[fov_stop_count] = i;
                fov_stop_count++;
            }
            segment_length = fast_vec_dist(path + (i * 16 + 0x10),
                                           path + (((i + 1) % *(int *)path) * 16 + 0x10));
            *(float *)(seg + 0x1C) = segment_length;
            if (i < *(int *)path - 1)
                *(float *)(camera_run + 2) += segment_length;
        }
    }
    if (*(short *)(rec + 0x3A) > 0) {
        *(float *)(rec + 0x3C) =
            *(float *)(camera_run + 2) /
            convert_integer_to_float(scale_game_frames(*(short *)(rec + 0x3A))) * 60.0f;
    }
    if (*(int *)(rec + 0x24) >= 0) {
        path = ((char *)l14_paths[*(int *)(rec + 0x24)]);
        for (i = 0; i < *(int *)path; i++) {
            char *seg;
            segment_length = fast_vec_dist(path + (i * 16 + 0x10),
                                           path + (((i + 1) % *(int *)path) * 16 + 0x10));
            seg = path + i * 16;
            *(float *)(seg + 0x1C) = segment_length;
            if (i < *(int *)path - 1)
                *(float *)(camera_run + 3) += segment_length;
        }
    }
    camera_saved_state = *(char **)(camera + 0x70);
    *(float *)(camera_saved_state + 0x0) = 0.03f;
    *(float *)(camera_saved_state + 0x4) = 0.2f;
    clear_u64_value(camera_saved_state + 0x10);
    camera_saved_state = *(char **)(camera + 0x70);
    *(float *)(camera_saved_state + 0x20) = 0.03f;
    *(float *)(camera_saved_state + 0x24) = 0.2f;
    clear_u64_value(camera_saved_state + 0x30);
    initial_camera_transform = l14_camera.unk184;
    qcopy(camera + 0x30, initial_camera_transform + 0x30);
    qcopy(camera, initial_camera_transform);
    qcopy(camera + 0x10, initial_camera_transform + 0x10);
    qcopy(camera + 0x20, initial_camera_transform + 0x20);
    path = (char *)l14_cuboids + *(int *)(rec + 0x54) * 128 + 0x30;
    stand = 0;
    ((float *)&stand)[3] = 1.0f;
    set_race_camera(path, &stand, 0x72, 0);
}

typedef struct {
    s16 seg[8];  /* 0x00: path segment of each stop (-1 ends the list) */
    f32 fov[8];  /* 0x10: field of view at each stop, degrees */
    f32 fov_end; /* 0x30: field of view handed back at the end */
} VendorCamStops_5290;

typedef struct {
    s32 seg;     /* 0x40 */
    f32 t;       /* 0x44 */
    f32 len_a;   /* 0x48: length of the camera path */
    f32 len_b;   /* 0x4C: length of the look path */
    f32 dist;    /* 0x50: distance travelled */
    s16 started; /* 0x54 */
    s16 timer;   /* 0x56 */
} VendorCamRun_5290;

extern char *D_L14_0015FFD8_5290 __asm__("D_L14_0015FFD8");

extern char path_camera_target_offset_basis[] __asm__("D_L14_001676D0");

extern float D_L14_001675D8 NOT_SDA;

extern float D_0013F5E0_5290[] __asm__("D_0013F5E0");

extern int sample_path(char *path, float *out, void *seg, void *t, int dir, float step) __asm__("FUN_L00_0025d808");

extern float cosine_interpolate(float, float, float) __asm__("FUN_002133d0");

extern void reset_racer_group_to_hero_path_position(struct Moby *m) __asm__("FUN_L14_002d6570");

void update_path_camera(struct UpdateCam *path_camera) __asm__("FUN_L14_00315290");

void update_path_camera(struct UpdateCam *path_camera) {
    float initial_look_direction_or_previous_position[4];
    float t[4];
    float u[4];
    float v[4];
    s32   seg;
    f32   tt;
    s32   seg2;
    f32   tt2;
    char *camera_saved_state = (char *)path_camera->saved_state;
    char *rec = *(char **)(l14_camera_preset_rows + path_camera->descriptor_index * 32 + 0x1C);
    VendorCamRun_5290   *camera_run = (VendorCamRun_5290 *)(camera_saved_state + 0x40);
    VendorCamStops_5290 *fov_stops = (VendorCamStops_5290 *)(camera_saved_state + 0x58);
    char                *path;
    int                  done;
    int                  i;

    if (camera_run->started == 0) {
        char *pa = ((char *)l14_paths[*(int *)(rec + 0x20)]);
        char *pb = ((char *)l14_paths[*(int *)(rec + 0x24)]);
        path_camera_active = 1;
        camera_run->started = 1;
        qcopy(&path_camera->pos, pa + 0x10);
        if (*(int *)(rec + 0x28) >= 0) {
            struct Moby *o = (struct Moby *)(D_L14_0015FFD8_5290 + *(int *)(rec + 0x28) * 256);
            float       *ofs = (float *)(rec + 0x2C);
            t[0] = ofs[0];
            t[1] = ofs[1];
            t[2] = ofs[2];
            t[3] = 0.0f;
            transform_vector_by_basis(t, t, path_camera_target_offset_basis);
            fast_vec_add(u, &o->pos, t);
            fast_vec_sub(initial_look_direction_or_previous_position, u, &path_camera->pos);
        } else {
            fast_vec_sub(initial_look_direction_or_previous_position, pb + 0x10, &path_camera->pos);
        }
        fast_vec_scale(t, D_0013F5E0_5290, -1.0f);
        fast_vec_normalize(&path_camera->m0, initial_look_direction_or_previous_position, 1.0f);
        fast_vec_cross(&path_camera->m1, &path_camera->m0, t);
        fast_vec_normalize(&path_camera->m1, &path_camera->m1, 1.0f);
        fast_vec_cross(&path_camera->m2, &path_camera->m1, &path_camera->m0);
        return;
    }
    path = ((char *)l14_paths[*(int *)(rec + 0x20)]);
    qcopy(initial_look_direction_or_previous_position, &path_camera->pos);
    done = sample_path(path,
                       &path_camera->pos.x,
                       &camera_run->seg,
                       &camera_run->t,
                       0,
                       *(float *)(rec + 0x3C) * frame_time);
    if (camera_run->timer != 0 && fast_dec_timer_s16(&camera_run->timer) != 0)
        done = 1;
    if (*(u8 *)(rec + 0x39) != 0)
        done = 1;
    if (done) {
        float ang = fov_stops->fov_end * 0.5f;
        char *hp;
        char *mb;
        l14_camera_projection_state.fB0 = fast_sin(ang) / fast_cos(ang);
        update_view_context();
        D_L14_001675D8 = 0.25f;
        path_camera->transition_state = 4;
        path_camera_active = 0;
        mb = D_L14_0015FFD8_5290;
        reset_racer_group_to_hero_path_position((struct Moby *)(mb + *(int *)(rec + 0x50) * 256));
        hp = (char *)hero.rail_spline;
        seg = hero.rail_segment;
        tt = hero.rail_seg_frac;
        sample_path(hp, &path_camera->pos.x, &seg, &tt, 0, 10.0f);
        path_camera->pos.z += 2.0f;
        seg = hero.rail_segment;
        tt = hero.rail_seg_frac;
        sample_path(hp, u, &seg, &tt, 0, 0.0f);
        u[2] += 1.5f;
        fast_vec_sub(t, u, &path_camera->pos);
        fast_vec_normalize(&path_camera->m0, t, 1.0f);
        fast_vec_cross(&path_camera->m1, &path_camera->m0, &hero.gravity_dir);
        fast_vec_normalize(&path_camera->m1, &path_camera->m1, -1.0f);
        fast_vec_cross(&path_camera->m2, &path_camera->m1, &path_camera->m0);
        return;
    }
    for (i = 0; i < 7 && fov_stops->seg[i] >= 0 && fov_stops->seg[i + 1] >= 0; i++) {
        int lo = fov_stops->seg[i];
        if (camera_run->seg >= lo && camera_run->seg < fov_stops->seg[i + 1]) {
            float sum = 0.0f;
            float total;
            float ang;
            int   traversed_segment_index;
            int   remaining_segment_index;
            for (traversed_segment_index = lo; traversed_segment_index < camera_run->seg;
                 traversed_segment_index++)
                sum += *(float *)(path + (traversed_segment_index * 16 + 0x1C));
            total = sum;
            sum += camera_run->t;
            for (remaining_segment_index = camera_run->seg;
                 remaining_segment_index < fov_stops->seg[i + 1];
                 remaining_segment_index++)
                total += *(float *)(path + (remaining_segment_index * 16 + 0x1C));
            ang = cosine_interpolate(fov_stops->fov[i], fov_stops->fov[i + 1], sum / total) *
                  0.017453292f * 0.5f;
            l14_camera_projection_state.fB0 = fast_sin(ang) / fast_cos(ang);
            update_view_context();
        }
    }
    camera_run->dist +=
        fast_vec_dist(initial_look_direction_or_previous_position, &path_camera->pos);
    if (*(int *)(rec + 0x28) >= 0) {
        struct Moby *o = (struct Moby *)(D_L14_0015FFD8_5290 + *(int *)(rec + 0x28) * 256);
        float       *ofs = (float *)(rec + 0x2C);
        u[0] = ofs[0];
        u[1] = ofs[1];
        u[2] = ofs[2];
        u[3] = 0.0f;
        transform_vector_by_basis(u, u, path_camera_target_offset_basis);
        fast_vec_add(v, &o->pos, u);
        fast_vec_sub(t, v, &path_camera->pos);
    } else {
        char *look = ((char *)l14_paths[*(int *)(rec + 0x24)]);
        seg2 = 0;
        tt2 = 0.0f;
        sample_path(
            look, u, &seg2, &tt2, 0, camera_run->dist / camera_run->len_a * camera_run->len_b);
        fast_vec_sub(t, u, &path_camera->pos);
    }
    fast_vec_scale(u, D_0013F5E0_5290, -1.0f);
    fast_vec_normalize(&path_camera->m0, t, 1.0f);
    fast_vec_cross(&path_camera->m1, &path_camera->m0, u);
    fast_vec_normalize(&path_camera->m1, &path_camera->m1, 1.0f);
    fast_vec_cross(&path_camera->m2, &path_camera->m1, &path_camera->m0);
}

extern int check_descriptor_hero_trigger(void *) __asm__("FUN_L10_002f5a78");

int check_path_camera_priority(char *camera,
                               char *control_mode_gate_object) __asm__("FUN_L14_00315920");

int check_path_camera_priority(char *camera, char *control_mode_gate_object) {
    char *rec = *(char **)(l14_camera_preset_rows + *(short *)(camera + 0x84) * 32 + 0x1C);
    int   required_control_mode, required_hero_state;
    if (*(unsigned char *)(rec + 0x38) != 0)
        return -1;
    if (control_mode_gate_object != 0 && *(short *)(control_mode_gate_object + 0x7E) == 0) {
        if (hero.state.control_mode != 15)
            return 0;
    }
    if (check_descriptor_hero_trigger(camera) == 0)
        return 0;
    required_control_mode = *(int *)(rec + 0x48);
    if (required_control_mode >= 0 && hero.state.control_mode != required_control_mode)
        return 0;
    required_hero_state = *(int *)(rec + 0x44);
    if (required_hero_state >= 0 && hero.state.current != required_hero_state)
        return 0;
    return 1;
}
