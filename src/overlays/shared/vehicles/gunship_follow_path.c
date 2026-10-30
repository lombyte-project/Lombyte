/* Gunship path following (moby class 0x34D, and the ships that fly paths). */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"
#include "sda.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/shared/vehicles/gunship_follow_path_data/gunship_follow_path.h"

struct DriftEmitter {
    s32 count; /* 0x00 */
    u8  pad4[0x2C];
    f32 spread; /* 0x30 */
};

typedef union {
    u128 q;
    f32  f[4];
    struct {
        f32 x, y, z, w;
    } v;
} QVec;

typedef struct {
    s32  count;
    u8   pad4[0xC];
    QVec node[1];
} PathNodes;

typedef struct {
    s32        idx;
    s8         dir;
    u8         state;
    u8         pad6[2];
    s32        unk8;
    f32        dist;
    PathNodes *nodes;
    s32        path_id;
    f32        heading;
    s32        unk1C;
} PathCursor;

typedef struct {
    u8         pad0[0x60];
    PathCursor path;  /* 0x60 */
    QVec       unk80; /* 0x80 */
    QVec       unk90; /* 0x90 */
    QVec       unkA0; /* 0xA0 */
    QVec       unkB0; /* 0xB0 */
    QVec       unkC0; /* 0xC0 */
    QVec       unkD0; /* 0xD0 */
    QVec       unkE0; /* 0xE0 */
    f32        unkF0;
    f32        unkF4;
    f32        unkF8;
    f32        unkFC;
    f32        unk100;
    f32        unk104;
    f32        unk108;
    f32        unk10C;
    f32        unk110;
    f32        unk114;
    f32        unk118;
    s16        unk11C;
    s16        unk11E;
    u8         unk120;
    u8         unk121;
    s16        unk122;
    f32        unk124;
    f32        unk128;
    f32        unk12C;
    f32        unk130;
    f32        unk134;
    s16        unk138;
    s16        unk13A;
    s16        unk13C;
    s16        unk13E;
    s32        unk140[4];
} Vars;

typedef struct {
    u8   pad0[0x70];
    s32  unk70;
    u8   pad74[0xC];
    QVec unk80;
} ChildVars;

typedef struct {
    u8  pad0[0x28];
    u16 kind;
} HitInfo;

extern char D_L01_00167240[];

extern int              D_0015ED84;
extern void             FUN_0022da68(int, int, char *);
extern int              FUN_00213260(int);
extern float            FUN_00213308(void);
extern float D_0015ED70 MACRO_ADDR;

extern float fast_vec_dist(void *, void *);
extern char *D_L01_0015FFD8;

extern f32 D_0015ED70;

extern s32 level_frame_count __asm__("D_L01_0015F5CC");

extern char  gunship_missing_path_warning_format[] __asm__("D_L01_0020B3E0");
extern s32   D_0015ED84;
extern f32   D_0015ED60;
extern f32   D_0015ED6C;
extern void *D_L01_00160770;

extern s32 check_scaled_vector_bsphere(struct Moby *, f32) __asm__("FUN_L01_00275690");
extern s32 get_first_moby_in_group(struct Moby **, s32, s32, s32) __asm__("FUN_L00_002591d0");
extern s32
get_next_moby_in_group(struct Moby **, struct Moby *, s32, s32) __asm__("FUN_L00_002592b8");
extern s32  FUN_L00_0028d8c0(struct Moby *, s32);
extern s32  FUN_00213260(s32);
extern s32  FUN_001e93b0(const char *, ...);
extern f32  FUN_002132a8(f32, f32);
extern void initialize_path_descriptor(PathCursor *) __asm__("FUN_L01_0028b410");
extern s32  select_point_index_by_distance(void *, PathNodes *, f32) __asm__("FUN_L01_0028b510");
extern void
compute_path_node_tangent(QVec *, PathCursor *, s32, f32, f32) __asm__("FUN_L01_0028b8c8");
extern f32 FUN_00213308(void);
extern f32 compute_sampled_spline_path_lengths(f32, f32, PathCursor *, s32, struct Moby *) __asm__("FUN_L01_0028bb90");
extern void FUN_0022d798(s32);
extern s32  advance_path_node_index(PathCursor *) __asm__("FUN_L01_0028b878");
extern s32  resolve_path_node_index(PathCursor *, s32) __asm__("FUN_L01_0028b828");
extern void evaluate_cubic_hermite_segment(f32, QVec *, QVec *, QVec *, QVec *, QVec *) __asm__("FUN_L01_0028ba80");
extern f32  FUN_L00_00258110(f32 *, f32, f32, f32, f32, f32);
extern f32  normalize_angle(f32) __asm__("FUN_L00_0025e310");
extern void spawn_type_10_particle(QVec *, QVec *) __asm__("FUN_L01_0027f660");
extern void spawn_explosion(struct Moby *,
                            QVec *,
                            QVec *,
                            f32,
                            f32,
                            s32,
                            s32,
                            s32,
                            f32,
                            f32,
                            f32,
                            f32,
                            s32,
                            f32,
                            s32,
                            s32,
                            s32,
                            s32) __asm__("FUN_L00_0025e450");

/* Builds two scaled offset vectors from the moby's matrix and adds them to out. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_0029DA60), where it is exact; names translated to the US level program. */

void apply_gunship_path_position_offsets(char *moby, float *out) __asm__("FUN_L01_002f5040");

void apply_gunship_path_position_offsets(char *moby, float *out) {
    float unit_x_axis[4];
    float rotated_x_axis[4];
    float horizontal_offset[4];
    float vertical_plane_offset[4];
    float rotated_x_axis_minus_unit_z[4];
    float rotation_matrix[16];
    char *data;
    *(OvlQuad *)unit_x_axis = 0;
    unit_x_axis[0] = 1.0f;
    data = ((char *)MOBY(moby)->pvars);
    if (*(float *)(data + 0x130) != 0.0f || *(float *)(data + 0x134) != 0.0f) {
        euler_rotation_matrix(rotation_matrix, (float *)&MOBY(moby)->rot);
        transform_vector(rotated_x_axis, unit_x_axis, rotation_matrix);
        *(OvlQuad *)rotated_x_axis_minus_unit_z = *(OvlQuad *)rotated_x_axis;
        rotated_x_axis_minus_unit_z[2] = rotated_x_axis_minus_unit_z[2] - 1.0f;
        fast_vec_cross(horizontal_offset, rotated_x_axis, rotated_x_axis_minus_unit_z);
        fast_vec_cross(vertical_plane_offset, rotated_x_axis, horizontal_offset);
        fast_vec_scale(horizontal_offset, horizontal_offset, *(float *)(data + 0x130));
        fast_vec_scale(vertical_plane_offset, vertical_plane_offset, *(float *)(data + 0x134));
        fast_vec_add(out, out, horizontal_offset);
        fast_vec_add(out, out, vertical_plane_offset);
    }
}

void gunship_follow_path(struct Moby *m) __asm__("FUN_L01_002f5168");

void gunship_follow_path(struct Moby *moby) {
    enum {
        GUNSHIP_PATH_STATE_INITIALIZE = 0,
        GUNSHIP_PATH_STATE_FOLLOW_PATH = 1,
        GUNSHIP_PATH_STATE_WAIT_FOR_PATH = 2,
        GUNSHIP_PATH_STATE_WAIT_FOR_RESUME_TIMER = 3
    };
    QVec         path_displacement;
    QVec         zero;
    QVec         previous_rotation;
    QVec         previous_position;
    QVec         attachment_and_hit_vectors[4];
    QVec         dA0;
    struct Moby *it;
    Vars        *v;
    PathCursor  *path_cursor;
    s32          frozen;
    s32          i;
    s32          never = 0;
    f32          path_speed;
    f32          segment_progress_step;

    zero.q = 0;
    frozen = 0;
    v = (Vars *)moby->pvars;
    if (moby->oclass != 0x336) {
        frozen = check_scaled_vector_bsphere(moby, convert_integer_to_float(moby->draw_dist)) == -1;
    }
    if (v == 0 || gunship_path_updates_disabled != 0) {
        return;
    }
    path_cursor = &v->path;
    if (moby->group != 0xFF && moby->state != 0 && moby->state != 3) {
        s32 bad = 0;
        get_first_moby_in_group(&it, moby->group, 0, 0);
        if (it == moby) {
            goto check;
            do {
                get_next_moby_in_group(&it, it, 0, 0);
            check:
                if (it == 0)
                    goto done;
            } while (check_scaled_vector_bsphere(it, convert_integer_to_float(it->draw_dist)) ==
                     -1);
            bad = 1;
        done:
            if (bad == 0) {
                get_first_moby_in_group(&it, moby->group, 0, 0);
                while (it != 0) {
                    Vars *iv = (Vars *)it->pvars;
                    bad = 3;
                    it->state = bad;
                    iv->unk11C = scale_game_frames(5);
                    get_next_moby_in_group(&it, it, 0, 0);
                }
            }
        }
    }
    if ((moby->oclass == 0x33 || moby->oclass == 0x46A || moby->oclass == 0x473 ||
         moby->oclass == 0x474) &&
        (level_frame_count & 7) == (((s32)moby >> 8) & 7) && v->unk13A <= 0 && moby->state != 0 &&
        FUN_L00_0028d8c0(moby, v->unk11E) == 0) {
        v->unk11E = allocate_voice_for_target_entry(0, 4, moby);
    }
    switch ((s16)(moby->oclass - 0x33)) {
    case 0:
    case 9:
    case 23:
        if (v->unk138 != -1) {
            if (!fast_dec_timer_s16(&v->unk138))
                break;
            if (fast_vec_dist(&moby->pos, D_L01_00167240) < 30.0f)
                allocate_voice_for_target_entry(1, 0, moby);
        }
        v->unk138 = scale_game_frames(random_int_between(v->unk13C, v->unk13C + v->unk13E));
        break;
    case 13:
    case 17:
        /* Fakematch: duplicating the reset preserves retail's voice-call argument scheduling. */
        if (v->unk138 != -1) {
            if (!fast_dec_timer_s16(&v->unk138))
                break;
            FUN_0022da68((FUN_00213260(100) & 1) ? 1 : 2, 0, (char *)moby);
            v->unk138 = scale_game_frames(random_int_between(v->unk13C, v->unk13C + v->unk13E));
        } else {
            v->unk138 = scale_game_frames(random_int_between(v->unk13C, v->unk13C + v->unk13E));
        }
        break;
    }

    switch (moby->state) {
    case GUNSHIP_PATH_STATE_INITIALIZE:
        if (path_cursor->path_id != -1) {
            PathNodes *pn = (PathNodes *)l01_paths[path_cursor->path_id];
            if (fast_vec_dist(&pn->node[0], &pn->node[pn->count - 1]) < 0.5f)
                pn->count--;
        } else {
            FUN_001e93b0(gunship_missing_path_warning_format,
                         moby->oclass,
                         truncate_float_to_s32((f32)(moby - (struct Moby *)D_L01_0015FFD8)));
        }
        moby->update_dist = 0xFF;
        v->unk11E = -1;
        if (500.0f < v->unk130)
            v->unk130 = FUN_002132a8(-1.0f, 1.0f) * v->unk130 / 1000.0f;
        if (500.0f < v->unk134)
            v->unk134 = FUN_002132a8(-1.0f, 1.0f) * v->unk134 / 1000.0f;
        if (path_cursor->path_id == -1) {
            moby->state = 2;
            return;
        }
        v->unk121 = (moby - (struct Moby *)D_L01_0015FFD8) / 2 % 7;
        initialize_path_descriptor(path_cursor);
        if (path_cursor->state == 0) {
            if (path_cursor->idx <= 0) {
                s32 selected_node_index =
                    select_point_index_by_distance(&moby->pos, path_cursor->nodes, 0.0f);
                path_cursor->idx = selected_node_index;
                if ((path_cursor->dir >= 0 &&
                     selected_node_index == path_cursor->nodes->count - 1) ||
                    (path_cursor->dir < 0 && selected_node_index == 0))
                    path_cursor->idx = 0;
            }
            path_cursor->dist =
                fast_vec_dist(&moby->pos, &path_cursor->nodes->node[path_cursor->idx]);
            path_cursor->heading =
                fast_arc_tan(path_cursor->nodes->node[path_cursor->idx].v.x - moby->pos.x,
                             path_cursor->nodes->node[path_cursor->idx].v.y - moby->pos.y);
            path_cursor->state = 2;
            path_cursor->unk8 = 0;
            path_cursor->unk1C = 0;
        }
        v->unkE0.q = (path_cursor->nodes->node + path_cursor->idx)->q;
        v->unkF0 = 1.0f;
        compute_path_node_tangent(&v->unkA0, path_cursor, 0, v->unkF4, v->unkF8);
        v->unkB0.q = v->unkC0.q = (path_cursor->nodes->node + path_cursor->idx)->q;
        *(s32 *)&moby->rot.x = 0;
        moby->rot.y = -fast_arc_tan(vector_length_xy(&v->unkA0), v->unkA0.v.z);
        moby->rot.z = fast_arc_tan(v->unkA0.v.x - zero.v.x, v->unkA0.v.y - zero.v.y);
        v->unk104 = 0;
        v->unk10C = 0;
        *(u128 *)&moby->pos = v->unkE0.q;
        apply_gunship_path_position_offsets((char *)moby, (float *)&moby->pos);
        v->unk121 = 7;
        if (6.2831855f < v->unk12C) {
            v->unk12C = v->unk12C * 0.017453292f;
        } else if (v->unk124 != 0.0f && v->unk12C == -1.0f) {
            v->unk12C = FUN_00213308();
        }
        v->unkD0.q = v->unkC0.q;
        compute_sampled_spline_path_lengths(
            v->unkF4, v->unkF8, path_cursor, v->unk120 & 0x20, moby);
        if ((v->unk120 & 0x1E) && !(v->unk120 & 1)) {
            s32 i;
            for (i = 0; i < 4; i++) {
                struct Moby *c;
                ChildVars   *cv;
                if (!(v->unk120 & (2 << i)) || v->unk140[i] == -1)
                    continue;
                c = (struct Moby *)D_L01_0015FFD8 + v->unk140[i];
                if (c == 0 || (cv = (ChildVars *)c->pvars) == 0)
                    continue;
                cv->unk70 |= 0x1000;
                moby_point_position(moby, i, &attachment_and_hit_vectors[i]);
                fast_vec_sub(&dA0, &attachment_and_hit_vectors[i], &moby->pos);
                fast_vec_normalize(&dA0, &dA0, 1.0f);
                *(u128 *)&c->pos = attachment_and_hit_vectors[i].q;
                cv->unk80.q = dA0.q;
            }
        }
        if (v->unk11C > 0) {
            moby->state = 3;
            return;
        }
        if (D_0015ED84 == 3 || D_0015ED84 == 9)
            v->unk13A = 0;
        moby->state = 1;
        return;

    case GUNSHIP_PATH_STATE_WAIT_FOR_PATH:
        if (path_cursor->path_id == -1)
            return;
        moby->state = 0;
        if (v->unk11E != -1) {
            u8 *e = (u8 *)((char *)&voice_pool) + v->unk11E * 0x70;
            if (*(struct Moby **)(e + 0x88) == moby && e[0x74] != 0)
                FUN_0022d798(v->unk11E);
        }
        v->unk11E = -1;
        return;

    case GUNSHIP_PATH_STATE_WAIT_FOR_RESUME_TIMER:
        if (fast_dec_timer_s16(&v->unk11C)) {
            v->unk11C = 0;
            moby->state = 1;
        }
        return;

    case GUNSHIP_PATH_STATE_FOLLOW_PATH:
        break;

    default:
        return;
    }

    previous_rotation.q = *(u128 *)&moby->rot;
    previous_position.q = *(u128 *)&moby->pos;
    if (1.0f <= v->unkF0) {
        s32 advanced_node_index;
        v->unkF0 -= 1.0f;
        advanced_node_index = advance_path_node_index(path_cursor);
        v->unk90.q = v->unkA0.q;
        compute_path_node_tangent(&v->unkA0, path_cursor, 1, v->unkF4, v->unkF8);
        v->unkB0.q = v->unkC0.q;
        v->unkC0.q = (path_cursor->nodes->node +
                      resolve_path_node_index(path_cursor, advanced_node_index + 1))
                         ->q;
    }
    {
        f32 speed_modulation_sine = fast_sin(v->unk12C);
        path_speed = v->unkFC + v->unk124 * D_0015ED60 * speed_modulation_sine * D_0015ED60;
        segment_progress_step = path_speed / v->unkB0.v.w;
        v->unk12C = fast_add_rotations(v->unk12C, v->unk128 * D_0015ED60 * 0.017453292f);
    }
    v->unkE0.q = v->unkD0.q;
    evaluate_cubic_hermite_segment(
        v->unkF0 + segment_progress_step, &v->unkD0, &v->unkB0, &v->unkC0, &v->unk90, &v->unkA0);
    if (!(v->unk120 & 0x40)) {
        f32 dd = fast_vec_dist(&v->unkE0, &v->unkD0);
        if (dd != 0.0f) {
            segment_progress_step = segment_progress_step * (path_speed / dd);
            evaluate_cubic_hermite_segment(v->unkF0 + segment_progress_step,
                                           &v->unkD0,
                                           &v->unkB0,
                                           &v->unkC0,
                                           &v->unk90,
                                           &v->unkA0);
        }
    }
    if (!frozen) {
        f32 k = 0.017453292f;
        f32 steering_angle;
        f32 turn;
        f32 yaw_offset_radians = (f32)v->unk122 * k;
        fast_subtract_rotations(moby->rot.z, yaw_offset_radians);
        steering_angle = fast_arc_tan(v->unkD0.v.x - v->unkE0.v.x, v->unkD0.v.y - v->unkE0.v.y);
        steering_angle = fast_add_rotations(steering_angle, yaw_offset_radians);
        steering_angle = fast_subtract_rotations(steering_angle, moby->rot.z);
        turn = steering_angle * (1.0f / v->unk110 * D_0015ED60);
        moby->rot.z = fast_add_rotations(turn, moby->rot.z);
        fast_vec_sub(&path_displacement, &v->unkD0, &v->unkE0);
        steering_angle = -fast_arc_tan(vector_length_xy(&path_displacement), path_displacement.v.z);
        moby->rot.y = FUN_L00_00258110(&v->unk10C,
                                       moby->rot.y,
                                       steering_angle,
                                       v->unk108 * 10.0f * k * D_0015ED70,
                                       v->unk108 * 5.0f * k * D_0015ED70,
                                       v->unk108 * k * D_0015ED6C);
        steering_angle = normalize_angle(turn * v->unk114);
        steering_angle = fast_subtract_rotations(moby->rot.x, steering_angle);
        steering_angle = normalize_angle(steering_angle * v->unk118);
        moby->rot.x = FUN_L00_00258110(&v->unk104,
                                       moby->rot.x,
                                       steering_angle,
                                       v->unk100 * 10.0f * k * D_0015ED70,
                                       v->unk100 * 5.0f * k * D_0015ED70,
                                       v->unk100 * k * D_0015ED6C);
    } else {
        clear_u64_value(&moby->rot);
        moby->rot.z = fast_arc_tan(v->unkD0.v.x - v->unkE0.v.x, v->unkD0.v.y - v->unkE0.v.y);
    }
    v->unkF0 += segment_progress_step;
    *(u128 *)&moby->pos = v->unkE0.q;
    if (!frozen)
        apply_gunship_path_position_offsets((char *)moby, (float *)&moby->pos);
    if (v->unk120 & 0x1F) {
        s32 attachment_point_index = 0;
        s32 i;
        for (i = 0; i < 5; i++) {
            if (!((v->unk120 >> i) & 1))
                continue;
            moby_point_position(moby, attachment_point_index, &attachment_and_hit_vectors[0]);
            attachment_point_index++;
            fast_vec_sub(
                &attachment_and_hit_vectors[1], &attachment_and_hit_vectors[0], &moby->pos);
            if (!(v->unk120 & 1)) {
                struct Moby *c;
                ChildVars   *cv;
                if (v->unk140[i - 1] == -1)
                    continue;
                c = (struct Moby *)D_L01_0015FFD8 + v->unk140[i - 1];
                if (c == 0 || (cv = (ChildVars *)c->pvars) == 0)
                    continue;
                fast_vec_normalize(
                    &attachment_and_hit_vectors[1], &attachment_and_hit_vectors[1], 1.0f);
                cv->unk70 |= 0x1000;
                *(u128 *)&c->pos = attachment_and_hit_vectors[0].q;
                cv->unk80.q = attachment_and_hit_vectors[1].q;
            } else if (v->unk121 == 0) {
                fast_vec_normalize(&attachment_and_hit_vectors[1],
                                   &attachment_and_hit_vectors[1],
                                   FUN_002132a8(0.016f, 0.031f) * D_0015ED60);
                spawn_type_10_particle(&attachment_and_hit_vectors[0],
                                       &attachment_and_hit_vectors[1]);
                fast_vec_normalize(&attachment_and_hit_vectors[1],
                                   &attachment_and_hit_vectors[1],
                                   FUN_002132a8(0.016f, 0.031f) * D_0015ED60);
                spawn_type_10_particle(&attachment_and_hit_vectors[0],
                                       &attachment_and_hit_vectors[1]);
            }
        }
        if (v->unk121 == 0)
            v->unk121 = scale_game_frames(7);
        v->unk121--;
    }
    if (D_0015ED84 != 3 && D_0015ED84 != 9)
        return;
    {
        HitInfo *hit = (HitInfo *)find_moby_hit(moby, 0x210000, 0);
        if (hit != 0 && hit->kind != 0x102) {
            QVec *current_position;
            s32   k;
            QVec *dir;
            clear_u64_value(&attachment_and_hit_vectors[0]);
            if (moby->oclass == 0x33) {
                attachment_and_hit_vectors[0].v.x = fast_cos(moby->rot.z) * -2.7470f;
                attachment_and_hit_vectors[0].v.y = fast_sin(moby->rot.z) * -2.7470f;
                *(s32 *)&attachment_and_hit_vectors[0].v.z = 0;
            }
            current_position = (QVec *)&moby->pos;
            fast_vec_sub(&attachment_and_hit_vectors[2], current_position, &previous_position);
            attachment_and_hit_vectors[1].q = attachment_and_hit_vectors[2].q;
            fast_vec_scale(&attachment_and_hit_vectors[1], &attachment_and_hit_vectors[1], 0.5f);
            fast_vec_add(
                &attachment_and_hit_vectors[0], &attachment_and_hit_vectors[0], current_position);
            v->unk13A = scale_game_frames(600);
            dir = &attachment_and_hit_vectors[1];
            /* Fakematch: the never-taken call keeps gcse from propagating &off[1] into the call, so dir
               gets a1 before the class tests and &off[1] gets s0, as in retail. */
            if (never)
                dir = (QVec *)scale_game_frames(0);
            k = -1;
            if (moby->oclass == 0x532 || moby->oclass == 0x53B || moby->oclass == 0x53C)
                k = 1;
            spawn_explosion(moby,
                            dir,
                            &attachment_and_hit_vectors[0],
                            0.0f,
                            0.0f,
                            0x14,
                            3,
                            4,
                            4.0f,
                            2.0f,
                            100000.0f,
                            3.0f,
                            k,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
        }
    }
    D_L01_00160770 = (void *)gunship_follow_path;
    moby->hit_slot = 0xFF;
    if (v->unk13A == 0)
        return;
    if (fast_dec_timer_s16(&v->unk13A) && frozen) {
        moby->collision = ((u32 *)moby->pclass)[4];
        moby->flags = (moby->flags & 0xFFBE) | 0x1000;
        return;
    }
    moby->collision = 0;
    moby->flags |= 0x41;
    if (v->unk11E != -1) {
        u8 *e = (u8 *)((char *)&voice_pool) + v->unk11E * 0x70;
        if (*(struct Moby **)(e + 0x88) == moby && e[0x74] != 0)
            FUN_0022d798(v->unk11E);
    }
    v->unk11E = -1;
    moby->flags &= 0xEFFF;
}
