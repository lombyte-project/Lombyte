/* Camera type 19, the path flythrough camera: flies once along a path, looking along a second path or at an anchor moby, with field-of-view stops; its trigger and activation helpers. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/runtime/callbacks/noop_main_callback.h"
#include "rnc/math/interpolation/cosine_interpolate.h"
#include "rnc/overlay/shared/rendering/culling/test_point_in_transformed_unit_cylinder.h"

#include "qcopy.h"
#include "rnc/camera/update_cam.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/shared/camera/path_flythrough_camera.h"

extern char *D_L10_0015EF50;

void activate_descriptor(int i) __asm__("FUN_L10_002f5a50");

void activate_descriptor(int i) {
    unsigned char *descriptor_record;
    descriptor_record = *(unsigned char **)(D_L10_0015EF50 + i * 0x20 + 0x1C);
    descriptor_record[0x38] = 1;
    descriptor_record[0x39] = 0;
}

extern int point_in_path_xy(float *, float *, int) __asm__("FUN_L00_00259740");

int check_descriptor_hero_trigger(char *moby) __asm__("FUN_L10_002f5a78");

int check_descriptor_hero_trigger(char *moby) {
    unsigned char *e = *(unsigned char **)(D_L10_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x1C);

    if (*(int *)(e + 0x0C) >= 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(e + 0x0C)))
            return 1;
    } else if (*(int *)(e + 0x10) >= 0) {
        if (test_point_in_transformed_unit_cylinder(&hero.motion.pos, *(int *)(e + 0x10)))
            return 1;
    } else if (*(int *)(e + 0x08) >= 0) {
        if (test_point_in_transformed_unit_sphere((float *)(&hero.motion.pos), *(int *)(e + 0x08)))
            return 1;
    } else if (*(int *)(e + 0x14) >= 0) {
        int i = *(int *)(e + 0x14);
        if (point_in_path_xy((float *)(&hero.motion.pos),
                             (float *)(((char *)l10_paths[i]) + 0x10),
                             *(int *)l10_paths[i]))
            return 1;
    } else if (e[0x38] != 0 && e[0x39] == 0) {
        return 1;
    }
    return 0;
}

typedef struct {
    char  pad0[0xB0];
    float tan_half_fov;
} VendorView_5f18;

typedef struct {
    s16 seg[8];  /* 0x00: path segment of each stop (-1 ends the list) */
    f32 fov[8];  /* 0x10: field of view at each stop, degrees */
    f32 fov_end; /* 0x30: field of view handed back at the end */
} VendorCamStops_5f18;

typedef struct {
    s32 seg;              /* 0x40 */
    f32 t;                /* 0x44 */
    f32 path_length;      /* 0x48 */
    f32 look_path_length; /* 0x4C */
    f32 dist;             /* 0x50: distance travelled */
    s16 started;          /* 0x54 */
    s16 timer;            /* 0x56 */
} VendorCamRun_5f18;

/* Retail retains this global base and loads the four-block source at +0x184. */
extern struct {
    char  pad0[0x184];
    char *source;
} D_L10_00167100;

extern VendorView_5f18 D_L10_0016CEC0;

void initialize_path_camera(struct UpdateCam *path_camera) __asm__("FUN_L10_002f5b58");

void initialize_path_camera(struct UpdateCam *path_camera) {
    char              *rec = *(char **)(D_L10_0015EF50 + path_camera->descriptor_index * 32 + 0x1C);
    VendorCamRun_5f18 *run;
    VendorCamStops_5f18 *stops;
    f32                 *fov;
    Vec4f               *m1, *m2, *pos;
    char                *saved;

    rec[0x38] = 1;
    run = (VendorCamRun_5f18 *)((char *)path_camera->saved_state + 0x40);
    run->seg = 0;
    run->t = 0.0f;
    run->path_length = 0.0f;
    run->look_path_length = 0.0f;
    run->dist = 0.0f;
    run->started = 0;
    run->timer = scale_game_frames(*(s16 *)(rec + 0x3A));
    stops = (VendorCamStops_5f18 *)((char *)path_camera->saved_state + 0x58);
    fov = stops->fov;
    {
        int k;
        for (k = 0; k < 8; k++) {
            stops->fov[k] = 0.0f;
            stops->seg[k] = -1;
        }
    }
    pos = &path_camera->pos;
    m1 = (Vec4f *)&path_camera->m1;
    m2 = (Vec4f *)&path_camera->m2;
    stops->fov_end = fast_arc_tan(1.0f, D_L10_0016CEC0.tan_half_fov) * 2.0f;
    if (*(int *)(rec + 0x20) >= 0) {
        char *path = ((char *)l10_paths[*(int *)(rec + 0x20)]);
        int   i = 0;
        if (i < *(int *)path) {
            f32 *stop_fov = fov;
            s16 *stop_seg = stops->seg;
            int  next_path_point_index;
            do {
                int offset = i * 16;
                f32 angle = *(f32 *)(path + offset + 0x1C);
                f32 length;
                if (angle > 0.0f) {
                    *stop_fov = angle;
                    if (angle > 180.0f)
                        *stop_fov = stops->fov_end * 57.295776f;
                    *stop_seg = i;
                    stop_fov++;
                    stop_seg++;
                }
                next_path_point_index = i + 1;
                length =
                    fast_vec_dist(&((Vec4f *)(path + 0x10))[i],
                                  &((Vec4f *)(path + 0x10))[next_path_point_index % *(int *)path]);
                {
                    char *point = path + i * 16;
                    *(f32 *)(point + 0x1C) = length;
                }
                if (i < *(int *)path - 1)
                    run->path_length += length;
                i = next_path_point_index;
            } while (i < *(int *)path);
        }
    }
    {
        s16 frames = *(s16 *)(rec + 0x3A);
        if (frames > 0)
            *(f32 *)(rec + 0x3C) =
                run->path_length / convert_integer_to_float(scale_game_frames(frames)) * 60.0f;
    }
    if (*(int *)(rec + 0x24) >= 0) {
        char *path = ((char *)l10_paths[*(int *)(rec + 0x24)]);
        int   i;
        for (i = 0; i < *(int *)path; i++) {
            int offset = i * 16;
            f32 length =
                fast_vec_dist(path + (offset + 0x10), path + ((i + 1) % *(int *)path * 16 + 0x10));
            *(f32 *)(path + offset + 0x1C) = length;
            if (i < *(int *)path - 1)
                run->look_path_length += length;
        }
    }
    saved = (char *)path_camera->saved_state;
    *(f32 *)(saved + 0x00) = 0.03f;
    *(f32 *)(saved + 0x04) = 0.2f;
    clear_u64_value(saved + 0x10);
    saved = (char *)path_camera->saved_state;
    *(f32 *)(saved + 0x20) = 0.03f;
    *(f32 *)(saved + 0x24) = 0.2f;
    clear_u64_value(saved + 0x30);
    {
        char *source = D_L10_00167100.source;
        qcopy(pos, source + 0x30);
        qcopy(path_camera, source);
        qcopy(m1, source + 0x10);
        qcopy(m2, source + 0x20);
    }
    if (*(int *)(rec + 0x4C) == 0)
        hero_set_state(0x72, 1);
}

extern char                *D_L10_0015FFD8_5f18 __asm__("D_L10_0015FFD8");
extern f32                  D_L10_0015F3FC;
extern s32                  D_L10_0015F404;
extern char                 D_L10_00167450[];
extern float D_L10_00167358 NOT_SDA;
extern float                D_0013F5E0_5f18[] __asm__("D_0013F5E0");
extern int sample_path(char *path, float *out, void *seg, void *t, int dir, float step) __asm__("FUN_L00_0025d808");
extern void FUN_L00_002223f8(s32, s32);
extern void debug_point_5f18(void *, float) __asm__("FUN_001e93e8");

void FUN_L10_002f5f18(struct UpdateCam *cam) {
    float                dir[4];
    float                offset[4];
    float                point[4];
    float                look_point[4];
    s32                  look_seg;
    f32                  look_t;
    char                *saved;
    VendorCamRun_5f18   *run;
    char                *rec;
    VendorCamStops_5f18 *stops;
    char                *path;
    int                  done;
    int                  i;

    saved = (char *)cam->saved_state;
    rec = *(char **)(D_L10_0015EF50 + cam->descriptor_index * 32 + 0x1C);
    run = (VendorCamRun_5f18 *)(saved + 0x40);
    stops = (VendorCamStops_5f18 *)(saved + 0x58);

    if (run->started == 0) {
        D_L10_0015F3FC += 0.05f;
        if (D_L10_0015F3FC >= 1.0f) {
            char *start_path = ((char *)l10_paths[*(int *)(rec + 0x20)]);
            char *look_path = ((char *)l10_paths[*(int *)(rec + 0x24)]);
            D_L10_0015F3FC = 1.0f;
            D_L10_0015F404 = 1;
            run->started = 1;
            qcopy(&cam->pos, start_path + 0x10);
            if (*(int *)(rec + 0x28) >= 0) {
                struct Moby *anchor =
                    (struct Moby *)(D_L10_0015FFD8_5f18 + *(int *)(rec + 0x28) * 256);
                float *ofs = (float *)(rec + 0x2C);
                offset[0] = ofs[0];
                offset[1] = ofs[1];
                offset[2] = ofs[2];
                offset[3] = 0.0f;
                transform_vector_by_basis(offset, offset, D_L10_00167450);
                fast_vec_add(point, &anchor->pos, offset);
                fast_vec_sub(dir, point, &cam->pos);
            } else {
                fast_vec_sub(dir, look_path + 0x10, &cam->pos);
            }
            fast_vec_scale(offset, D_0013F5E0_5f18, -1.0f);
            fast_vec_normalize(&cam->m0, dir, 1.0f);
            fast_vec_cross(&cam->m1, &cam->m0, offset);
            fast_vec_normalize(&cam->m1, &cam->m1, 1.0f);
            fast_vec_cross(&cam->m2, &cam->m1, &cam->m0);
        }
        return;
    }
    if (run->started == 2) {
        D_L10_0015F3FC += 0.05f;
        if (D_L10_0015F3FC >= 1.0f) {
            float ang;
            run->started = 3;
            D_L10_0015F3FC = 1.0f;
            ang = stops->fov_end * 0.5f;
            D_L10_0016CEC0.tan_half_fov = fast_sin(ang) / fast_cos(ang);
            update_view_context();
            *(u8 *)(rec + 0x39) = 1;
            D_L10_00167358 = 0.05f;
            cam->transition_state = 4;
            D_L10_0015F404 = 0;
            if (*(int *)(rec + 0x4C) == 0)
                FUN_L00_002223f8(0, 1);
        }
        if (*(int *)(rec + 0x40) == 0)
            return;
    } else if (run->started == 3) {
        return;
    } else {
        D_L10_0015F3FC -= 0.05f;
        if (D_L10_0015F3FC < 0.0f)
            D_L10_0015F3FC = 0.0f;
    }
    path = ((char *)l10_paths[*(int *)(rec + 0x20)]);
    qcopy(dir, &cam->pos);
    done =
        sample_path(path, &cam->pos.x, &run->seg, &run->t, 0, *(float *)(rec + 0x3C) * frame_time);
    if (run->timer != 0) {
        if (fast_dec_timer_s16(&run->timer) != 0)
            done = 1;
        else if (*(int *)(rec + 0x40) != 0 &&
                 run->timer < scale_game_frames(truncate_float_to_s32(20.0f)))
            done = 1;
    }
    if (*(u8 *)(rec + 0x39) != 0)
        done = 1;
    if (done) {
        run->started = 2;
        if (*(int *)(rec + 0x40) == 0)
            return;
    }
    for (i = 0; i < 7 && stops->seg[i] >= 0 && stops->seg[i + 1] >= 0; i++) {
        int first_seg = stops->seg[i];
        if (run->seg >= first_seg && run->seg < stops->seg[i + 1]) {
            float sum = 0.0f;
            float total;
            float ang;
            int   before;
            int   after;
            for (before = first_seg; before < run->seg; before++)
                sum += *(float *)(path + (before * 16 + 0x1C));
            total = sum;
            sum += run->t;
            for (after = run->seg; after < stops->seg[i + 1]; after++)
                total += *(float *)(path + (after * 16 + 0x1C));
            ang = cosine_interpolate(stops->fov[i], stops->fov[i + 1], sum / total) * 0.017453292f *
                  0.5f;
            D_L10_0016CEC0.tan_half_fov = fast_sin(ang) / fast_cos(ang);
            update_view_context();
        }
    }
    run->dist += fast_vec_dist(dir, &cam->pos);
    if (*(int *)(rec + 0x28) >= 0) {
        struct Moby *anchor = (struct Moby *)(D_L10_0015FFD8_5f18 + *(int *)(rec + 0x28) * 256);
        float       *ofs = (float *)(rec + 0x2C);
        point[0] = ofs[0];
        point[1] = ofs[1];
        point[2] = ofs[2];
        point[3] = 0.0f;
        transform_vector_by_basis(point, point, D_L10_00167450);
        fast_vec_add(look_point, &anchor->pos, point);
        fast_vec_sub(offset, look_point, &cam->pos);
    } else {
        char *look = ((char *)l10_paths[*(int *)(rec + 0x24)]);
        look_seg = 0;
        look_t = 0.0f;
        sample_path(look,
                    point,
                    &look_seg,
                    &look_t,
                    0,
                    run->dist / run->path_length * run->look_path_length);
        debug_point_5f18(point, 0.2f);
        noop_main_callback(look + 0x10, look + *(int *)look * 16);
        fast_vec_sub(offset, point, &cam->pos);
    }
    fast_vec_scale(point, D_0013F5E0_5f18, -1.0f);
    fast_vec_normalize(&cam->m0, offset, 1.0f);
    fast_vec_cross(&cam->m1, &cam->m0, point);
    fast_vec_normalize(&cam->m1, &cam->m1, 1.0f);
    fast_vec_cross(&cam->m2, &cam->m1, &cam->m0);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00299AF0.c: func_L10_002F79B8), where it is exact; names translated to the US level program. */

extern int FUN_L10_002f5a78_u(char *, char *) __asm__("FUN_L10_002f5a78");

int evaluate_descriptor_eligibility(char *descriptor_owner,
                                    char *comparison_object) __asm__("FUN_L10_002f65f8");

int evaluate_descriptor_eligibility(char *descriptor_owner, char *comparison_object) {
    unsigned char *descriptor_record =
        *(unsigned char **)(D_L10_0015EF50 + *(short *)(descriptor_owner + 0x84) * 32 + 0x1C);
    int x;
    int y;
    if (descriptor_record[0x39] != 0) {
        return -1;
    }
    if (descriptor_record[0x38] != 0 && *(short *)(comparison_object + 0x86) != 0x13) {
        return 1;
    }
    if (comparison_object != 0 && *(short *)(comparison_object + 0x7E) == 0 &&
        (unsigned char)comparison_object[0x7C] >= (unsigned char)descriptor_owner[0x7C]) {
        return 0;
    }
    if (FUN_L10_002f5a78_u(descriptor_owner, comparison_object) == 0) {
        return 0;
    }
    x = *(int *)(descriptor_record + 0x48);
    if (x >= 0 && hero.state.control_mode != x) {
        return 0;
    }
    y = *(int *)(descriptor_record + 0x44);
    if (y >= 0 && hero.state.current != y) {
        return 0;
    }
    return 1;
}

float D_L13_001671D8 NOT_SDA = 0;
float D_L15_001674D8 NOT_SDA = 0;
