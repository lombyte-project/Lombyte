/* Level 11 moby class oil_tanker_front: the chain head, its path poses, damage and links. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

extern float frame_scale_inv_macro[] __asm__("D_0015ED68") MACRO_ADDR;

extern char *D_L11_0015FFD8;

#include "rnc/overlay/level_path.h"

extern short *D_L11_001AC240[];
extern s32    FUN_0022da68(s32, s32, void *);

/* Private variables of the chained moby class 0x527 (oil_tanker_front_update and its helpers). */
struct L11ChainVars {
    u8           pad0[0x60];
    s32          path_index; /* 0x60: index into l11_paths */
    s32          count;      /* 0x64: live links */
    s32          point;      /* 0x68: current path point */
    f32          t;          /* 0x6C: progress between points */
    struct Moby *links[8];   /* 0x70 */
    s16          offsets[8]; /* 0x90 */
    f32          frac[8];    /* 0xA0: position between path points */
    f32          catchup
        [8]; /* 0xC0: path distance a link still trails its slot by after a link dies; eased to 0 */
    f32   health; /* 0xE0 */
    s16   group;  /* 0xE4 */
    u8    padE6[6];
    s32   path_sel; /* 0xEC: index into l11_paths on spawn */
    char *path;     /* 0xF0 */
    s32   sound;    /* 0xF4 */
};

#include "rnc/overlay/l11/mobys/oil_tanker_front_globals.h"

extern float oil_tanker_path_progress_step_gp __asm__("D_L11_00162320") __attribute__((sda));
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
void         update_oil_tanker_front_path_pose(struct Moby *moby) __asm__("FUN_L11_00317678");

void update_oil_tanker_front_path_pose(struct Moby *) __asm__("FUN_L11_00317678");
void update_oil_tanker_link_path_poses(struct Moby *) __asm__("FUN_L11_00317820");
void process_oil_tanker_damage_and_destruction(struct Moby *moby) __asm__("FUN_L11_00317c98");

void process_oil_tanker_damage_and_destruction(struct Moby *) __asm__("FUN_L11_00317c98");
void distribute_group_blarg_ships_along_path(struct Moby *moby,
                                             int          table_index) __asm__("FUN_L11_003180a0");

void distribute_group_blarg_ships_along_path(struct Moby *, int) __asm__("FUN_L11_003180a0");

/* Chain head update: spawns the eight 0x4F1 links, waits for D_001413D4 to reach 0x32, then moves along its path and runs the link helpers. */
void         oil_tanker_front_update(struct Moby *moby) __asm__("FUN_L11_003172c0");
extern float approach_value(float *p, float target, float maxstep);

void oil_tanker_front_update(struct Moby *moby) {
    enum {
        OIL_TANKER_INITIALIZE = 0,
        OIL_TANKER_WAIT_FOR_CAMERA_MODE = 1,
        OIL_TANKER_FOLLOW_PATH_WITH_DAMAGE = 2
    };
    struct L11ChainVars *chain_vars = (struct L11ChainVars *)moby->pvars;

    switch (moby->state) {
    case OIL_TANKER_INITIALIZE: {
        int i;

        distribute_group_blarg_ships_along_path(moby, chain_vars->path_index);
        if (visited52[moby->mission + current_level_index * 0x10] == 0xFF) {
            mark_moby_for_removal(moby);
            break;
        }
        moby->update_dist = 0xFF;
        moby->draw_dist = 0x200;
        chain_vars->path = (char *)l11_paths[chain_vars->path_sel];
        qcopy(&moby->pos, chain_vars->path + 0x10);
        chain_vars->health = 80.0f;
        chain_vars->count = 8;
        chain_vars->sound = -1;
        chain_vars->point = 0;
        chain_vars->t = 0.0f;
        moby->state = 1;
        for (i = 0; i < 8; i++) {
            struct Moby *link;
            chain_vars->links[i] = create_moby(0x4F1);
            chain_vars->links[i]->draw_dist = 0x200;
            chain_vars->links[i]->update_dist = 0xFF;
            chain_vars->links[i]->visible = 1;
            chain_vars->links[i]->spawn_frame = moby->spawn_frame;
            chain_vars->links[i]->flags = moby->flags & 0xFFDF;
            chain_vars->links[i]->scale = chain_vars->links[i]->pclass->scale * 2.0f;
            link = chain_vars->links[i];
            qcopy(&link->pos, &moby->pos);
            qcopy(&link->rot, &moby->rot);
            chain_vars->offsets[i] = *(u16 *)chain_vars->path - 100;
            chain_vars->catchup[i] = 0.0f;
        }
        break;
    }
    case OIL_TANKER_WAIT_FOR_CAMERA_MODE:
        if (camera_secondary_mode[0] == 0x32) {
            int i;

            moby->state = 2;
            chain_vars->path = (char *)l11_paths[chain_vars->path_index];
            chain_vars->point = 0;
            for (i = 0; i < 8; i++) {
                chain_vars->offsets[i] = *(u16 *)chain_vars->path - 100;
                chain_vars->catchup[i] = 0.0f;
            }
        }
        /* fallthrough */
    case OIL_TANKER_FOLLOW_PATH_WITH_DAMAGE: {
        int i;

        chain_vars->t += oil_tanker_path_progress_step_gp;
        if (chain_vars->t > 1.0f) {
            chain_vars->t -= 1.0f;
            chain_vars->point = (chain_vars->point + 1) % *(s32 *)chain_vars->path;
        }
        for (i = 0; i < 8; i++) {
            float before = chain_vars->catchup[i];
            approach_value(&chain_vars->catchup[i], 0.0f, frame_time * 5.0f);
            if (before != 0.0f && chain_vars->catchup[i] == 0.0f && chain_vars->links[i] != 0) {
                FUN_0022da68(0, 0, chain_vars->links[i]);
            }
        }
        update_oil_tanker_front_path_pose(moby);
        update_oil_tanker_link_path_poses(moby);
        if (moby->state == 2) {
            process_oil_tanker_damage_and_destruction(moby);
        }
        if (voice_is_playing(moby, chain_vars->sound) == 0) {
            chain_vars->sound = FUN_0022da68(0, 4, moby);
        }
        break;
    }
    }
}

/* Orients a moby along a path of three points read from its data table. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00318AE8), where it is exact; names translated to the US level program. */

void update_oil_tanker_front_path_pose(struct Moby *moby) {
    char  *chain_vars = (char *)moby->pvars;
    int    i = *(int *)(chain_vars + 0x68);
    char  *path = *(char **)(chain_vars + 0xF0);
    int    path_point_count;
    float  current_path_point[4];
    float  next_path_point[4];
    float  lookahead_path_point[4];
    float *pb;
    float *pc;
    float  x, y, z;
    qcopy(current_path_point, path + i * 16 + 0x10);
    path_point_count = *(int *)path;
    qcopy(pb = next_path_point, path + (i + 1) % path_point_count * 16 + 0x10);
    qcopy(pc = lookahead_path_point, path + (i + 2) % path_point_count * 16 + 0x10);
    x = fast_arc_tan(current_path_point[0] - next_path_point[0],
                     current_path_point[1] - next_path_point[1]);
    y = fast_arc_tan(next_path_point[0] - lookahead_path_point[0],
                     next_path_point[1] - lookahead_path_point[1]);
    z = fast_arc_tan(vector_distance_xy(current_path_point, pb),
                     next_path_point[2] - current_path_point[2]);
    moby->rot.y = fast_add_rotations(
        fast_subtract_rotations(
            fast_arc_tan(vector_distance_xy(pb, pc), lookahead_path_point[2] - next_path_point[2]),
            z) *
            *(float *)(chain_vars + 0x6C),
        z);
    moby->rot.z =
        fast_add_rotations(fast_subtract_rotations(y, x) * *(float *)(chain_vars + 0x6C), x);
    lerp_vector(&moby->pos, current_path_point, pb, *(float *)(chain_vars + 0x6C));
}

#include "rnc/overlay/l11/mobys/oil_tanker_front_data/update_oil_tanker_link_path_poses.h"

extern s32 D_L11_0015F5C4;

/* A chain path in l11_paths: a point count, then the points (w = 42 marks a sound point). */
struct L11ChainPath {
    s32   count;
    u8    pad4[0xC];
    Vec4f pts[1];
};

extern int scale_game_frames(int);

/* Moves each chain link along the path to a fixed distance behind the one before it and turns it to follow the curve. */
void update_oil_tanker_link_path_poses(struct Moby *moby) {
    struct L11ChainVars *chain_vars = (struct L11ChainVars *)moby->pvars;
    char                *path = chain_vars->path;
    struct L11ChainPath *chain_path = (struct L11ChainPath *)chain_vars->path;
    float                spacing_reference_position[4];
    int                  i;

    fast_vec_normalize(
        spacing_reference_position, &moby->rot_row0, oil_tanker_spacing_reference_offset_gp);
    fast_vec_add(spacing_reference_position, spacing_reference_position, &moby->pos);
    for (i = 0; i < chain_vars->count; i++) {
        int   segment_start_index = chain_vars->offsets[i];
        int   segment_end_index = (segment_start_index + 1) % chain_path->count;
        int   lookahead_point_index = (segment_start_index + 2) % chain_path->count;
        float segment_start_distance =
            fast_vec_dist(&chain_path->pts[segment_start_index], spacing_reference_position);
        float segment_end_distance =
            fast_vec_dist(&chain_path->pts[segment_end_index], spacing_reference_position);
        float        segment_fraction;
        struct Moby *link;
        float        yaw0, yaw1, pitch0;
        float       *qa, *qb, *qc; /* the points as raw rows: [4], [5], [6] = x, y, z */

        while (segment_end_distance > oil_tanker_link_spacing_threshold_gp) {
            segment_start_index = segment_end_index;
            segment_end_index = (segment_start_index + 1) % chain_path->count;
            segment_start_distance = segment_end_distance;
            lookahead_point_index = (segment_end_index + 1) % chain_path->count;
            segment_end_distance =
                fast_vec_dist(&chain_path->pts[segment_end_index], spacing_reference_position);
        }
        {
            float *segment_start_marker_base = (float *)(path + segment_start_index * 16);

            if (segment_start_marker_base[7] == 42.0f) {
                u8 *lv = chain_vars->links[i]->pvars;
                if (*(s32 *)(lv + 0x28) == 0) {
                    if (D_L11_0015F5C4 == 0) {
                        FUN_0022da68(2, 0, chain_vars->links[i]);
                    }
                    *(s32 *)(lv + 0x28) =
                        scale_game_frames(oil_tanker_link_marker_cooldown_frames_gp);
                }
            }
        }
        if (segment_start_distance > oil_tanker_link_spacing_threshold_gp) {
            segment_fraction = (segment_start_distance - oil_tanker_link_spacing_threshold_gp) /
                               (segment_start_distance - segment_end_distance);
        } else {
            segment_fraction = 0.0f;
        }
        chain_vars->offsets[i] = segment_start_index;
        chain_vars->frac[i] = segment_fraction;
        if (chain_vars->catchup[i] != 0.0f) {
            float catchup_adjusted_progress = segment_fraction - chain_vars->catchup[i];
            int   k = truncate_float_to_s32(catchup_adjusted_progress);
            catchup_adjusted_progress -= (float)truncate_float_to_s32(catchup_adjusted_progress);
            if (catchup_adjusted_progress < 0.0f) {
                k--;
                catchup_adjusted_progress += 1.0f;
            }
            segment_start_index = (segment_start_index + k + chain_path->count) % chain_path->count;
            segment_end_index = (segment_end_index + k + chain_path->count) % chain_path->count;
            lookahead_point_index =
                (lookahead_point_index + k + chain_path->count) % chain_path->count;
            segment_fraction = catchup_adjusted_progress;
        }
        link = chain_vars->links[i];
        lerp_vector(&link->pos,
                    &chain_path->pts[segment_start_index],
                    &chain_path->pts[segment_end_index],
                    segment_fraction);
        qcopy(spacing_reference_position, &chain_vars->links[i]->pos);
        qa = (float *)(path + segment_start_index * 16);
        qb = (float *)(path + segment_end_index * 16);
        qc = (float *)(path + lookahead_point_index * 16);
        yaw0 = fast_arc_tan(qa[4] - qb[4], qa[5] - qb[5]);
        yaw1 = fast_arc_tan(qb[4] - qc[4], qb[5] - qc[5]);
        pitch0 = -fast_arc_tan(vector_distance_xy(&chain_path->pts[segment_end_index].x,
                                                  &chain_path->pts[segment_start_index].x),
                               qa[6] - qb[6]);
        chain_vars->links[i]->rot.y = fast_add_rotations(
            fast_subtract_rotations(
                -fast_arc_tan(vector_distance_xy(&chain_path->pts[lookahead_point_index].x,
                                                 &chain_path->pts[segment_end_index].x),
                              qb[6] - qc[6]),
                pitch0) *
                segment_fraction,
            pitch0);
        chain_vars->links[i]->rot.z =
            fast_add_rotations(fast_subtract_rotations(yaw1, yaw0) * segment_fraction, yaw0);
        update_entity(chain_vars->links[i]);
        fast_vec_normalize(spacing_reference_position,
                           &chain_vars->links[i]->rot_row0,
                           oil_tanker_spacing_reference_offset_gp);
        fast_vec_add(
            spacing_reference_position, spacing_reference_position, &chain_vars->links[i]->pos);
    }
}

extern char  D_00141C18[];
extern float D_L11_0015F580[] MACRO_ADDR;
extern float oil_tanker_head_fragment_velocity_scale_gp __asm__("D_L11_00162330")
    __attribute__((sda));
void *FUN_L00_00263fd8(char  *src,
                       int    cls,
                       float *pos,
                       void  *mat,
                       int    a8,
                       int    a9,
                       float *v10,
                       float *v11,
                       float  scale,
                       float *v12);
void  start_oil_tanker_link_destruction(struct Moby *link_moby) __asm__("FUN_L11_00318488");

void         start_oil_tanker_link_destruction(struct Moby *) __asm__("FUN_L11_00318488");
extern float D_L11_0015F580[];

/* Drops the chain's damaged links: takes the hits, shrinks the chain as its health falls and blows the head up when no link is left. */
void process_oil_tanker_damage_and_destruction(struct Moby *moby) {
    struct L11ChainVars *chain_vars = (struct L11ChainVars *)moby->pvars;
    int                  i;
    int                  last_hit_link_index = -1;
    float                total = 0.0f;
    float                fragment_initial_velocity[4];

    for (i = 0; i < chain_vars->count; i++) {
        struct Moby *hit = find_moby_hit(chain_vars->links[i], -1, 0);
        if (hit != 0) {
            last_hit_link_index = i;
            total += hit->scale;
            chain_vars->links[i]->hit_slot = 0xFF;
        }
    }
    approach_value(&chain_vars->health, 0.0f, total);
    if (total != 0.0f && chain_vars->health / 80.0f * 8.0f <= (float)(chain_vars->count - 1)) {
        struct Moby *last_hit_link;
        struct Moby *detached_tail_link;
        *(u16 *)D_00141C18 = 0xFFFF;
        chain_vars->count--;
        start_oil_tanker_link_destruction(chain_vars->links[chain_vars->count]);
        last_hit_link = chain_vars->links[last_hit_link_index];
        detached_tail_link = chain_vars->links[chain_vars->count];
        qcopy(&detached_tail_link->pos, &last_hit_link->pos);
        qcopy(&detached_tail_link->rot, &last_hit_link->rot);
        FUN_0022da68(1, 0, detached_tail_link);
        if (last_hit_link_index < chain_vars->count) {
            float path_point_offset_delta = chain_vars->offsets[last_hit_link_index] -
                                            chain_vars->offsets[last_hit_link_index + 1];
            int j;
            for (j = last_hit_link_index; j < chain_vars->count; j++) {
                chain_vars->catchup[j] = chain_vars->catchup[j + 1];
            }
            chain_vars->catchup[last_hit_link_index] += path_point_offset_delta;
            if (chain_vars->catchup[last_hit_link_index] < 0.0f) {
                chain_vars->catchup[last_hit_link_index] += (float)*(s32 *)chain_vars->path;
            }
        }
        {
            int k;
            for (k = last_hit_link_index; k < chain_vars->count; k++) {
                chain_vars->offsets[k] = chain_vars->offsets[k + 1];
            }
        }
        if (chain_vars->count == 0) {
            char  *head_position = (char *)&moby->pos;
            float *dv;
            char  *head_rotation;
            fast_vec_scale(fragment_initial_velocity,
                           &moby->rot_row0,
                           (dv = D_L11_0015F580,
                            head_rotation = (char *)&moby->rot,
                            oil_tanker_head_fragment_velocity_scale_gp * frame_scale));
            fragment_initial_velocity[2] += frame_scale * 0.08f;
            FUN_0022da68(2, 0, moby);
            cart_burst(moby, head_position, -1, 3.0f, 13.0f);
            FUN_L00_00263fd8((char *)moby,
                             0x602,
                             (float *)head_position,
                             head_rotation,
                             scale_game_frames(0x5A),
                             0,
                             fragment_initial_velocity,
                             dv,
                             frame_time_sq * 12.0f,
                             dv);
            FUN_L00_00263fd8((char *)moby,
                             0x603,
                             (float *)head_position,
                             head_rotation,
                             scale_game_frames(0x5A),
                             0,
                             fragment_initial_velocity,
                             dv,
                             frame_time_sq * 12.0f,
                             dv);
            FUN_L00_00263fd8((char *)moby,
                             0x784,
                             (float *)head_position,
                             head_rotation,
                             scale_game_frames(0x5A),
                             0,
                             fragment_initial_velocity,
                             dv,
                             frame_time_sq * 12.0f,
                             dv);
            mark_moby_for_removal(moby);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_003194C0), where it is exact; names translated to the US level program. */

int collect_oil_tanker_chain_mobys(struct Moby *moby, void **out) __asm__("FUN_L11_00318050");

int collect_oil_tanker_chain_mobys(struct Moby *moby, void **out) {
    char *data = (char *)moby->pvars;
    int   i;
    int   count = *(int *)(data + 0x64) + 1;
    out[0] = moby;
    for (i = 0; i < *(int *)(data + 0x64); i++) {
        out[i + 1] = ((void **)(data + 0x70))[i];
    }
    return count;
}

/* Spreads the 0x527 chain mobys of a D_L11_001AC240 group evenly along path table_index: each gets a start point and its path index. */
/* idx is shared by both loops (one multi-block pseudo), which gives the second loop retail's register choice. */
void distribute_group_blarg_ships_along_path(struct Moby *moby, int table_index) {
    short group = ((struct L11ChainVars *)moby->pvars)->group;
    u16  *list = (u16 *)D_L11_001AC240[group];
    short count = 0;
    short offset = 0;
    short stride;
    char *table;
    u16   entry;
    int   moby_array_index;

    if (list == 0)
        return;
    do {
        entry = *list;
        moby_array_index = entry & 0x7FFF;
        if (((struct Moby *)D_L11_0015FFD8)[moby_array_index].oclass == 0x527)
            count++;
        list++;
    } while ((short)entry >= 0);

    table = (char *)l11_paths[table_index];
    stride = *(int *)table / count;
    list = (u16 *)D_L11_001AC240[group];
    do {
        struct Moby *group_moby;
        moby_array_index = *list & 0x7FFF;
        group_moby = (struct Moby *)((moby_array_index << 8) + (u32)D_L11_0015FFD8);
        if (group_moby->oclass == 0x527) {
            struct L11ChainVars *data = (struct L11ChainVars *)group_moby->pvars;
            /* no memory clobber: lets the D_L11_0015FFD8 load be hoisted out of the loop as in retail */
            qcopy_nc(&group_moby->pos, table + offset * 16 + 0x10);
            data->count = offset;
            data->path_index = table_index;
            data->point = 0;
            offset += stride;
        }
    } while (*(short *)list++ >= 0);
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_003198F8), where it is exact; names translated to the US level program. */

void start_oil_tanker_link_destruction(struct Moby *link_moby) {
    char *link_vars = (char *)link_moby->pvars;
    link_moby->state = 1;
    link_moby->collision = 0;
    fast_vec_sub(link_vars + 0x10, &link_moby->pos, link_vars);
    *(float *)(link_vars + 0x20) =
        random_float_between(20.0f, 60.0f) * DEG_TO_RAD * frame_scale_inv_macro[1];
    *(float *)(link_vars + 0x24) =
        random_float_between(20.0f, 60.0f) * DEG_TO_RAD * frame_scale_inv_macro[1];
}
