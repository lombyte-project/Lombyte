/* Level 10 moby class screamer: path following, targeting and hit reactions. */
#include "types.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l10/mobys/aligned_vector.h"
#include "rnc/overlay/l10/mobys/screamer.h"

/* Update function for moby class 1196 (screamer) on level 10: follows its path, turns to its target, wanders. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E0630), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct D_2E0630 {
    char          p0[0x20];
    float         f20;
    short         h24;
    char          p26[2];
    unsigned char b28;
    unsigned char b29;
    char          p2A[6];
    float         f30;
    char          p34[0xC];
    AlignedVec4f  step;
    char          p50[0x10];
    char          react[0xA0];
    struct Moby  *leader;
    int           f104;
    char          p108[0x18];
    AlignedVec4f  vel;
    AlignedVec4f  home;
    AlignedVec4f  wander_goal;
    int           path;
    char          p154[4];
    int           node_index;
    char          p15C[4];
    int           linked[4];
    float         yaw_speed;
    int           f174;
    char          p178[0x24];
    int           wander_timer;
    char          p1A0[0xD0];
    char          sound_state[4];
} D_2E0630;

/* Entry 2 is the moby array D_L10_0015FFD8. Retail reaches it with the lui/lw macro in some functions and through $gp in toxic_crab_update; going through this table 8 bytes lower keeps the macro form under SN. */
extern struct Moby *D_L10_0015FFD0_macro[] __asm__("D_L10_0015FFD0") MACRO_ADDR;
extern float        D_0015ED6C;
extern float        D_0015ED70;
extern void         func_L10_002E0FF8_2E0630(void *) __asm__("FUN_L10_002dfc38");
extern void func_L00_00264B40_2E0630(float, void *, int, void *) __asm__("FUN_L00_00263ac8");
extern void func_L00_0025B178_2E0630(void *) __asm__("FUN_L00_0025a120");
extern float
advance_accelerated_scalar(float *, float, float *, float, float, float) __asm__("FUN_00213f38");
extern float
func_L00_0025CE58_2E0630(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern void func_L00_002607A8_2E0630(void *, float) __asm__("FUN_L00_0025f730");
extern int
     func_L00_00259B08_2E0630(void *, void *, float, int, float, int) __asm__("FUN_L00_00258ad0");
void activate_waiting_moby_group_members(struct Moby *moby) __asm__("FUN_L10_002e0138");
extern float func_L00_0025CC58_2E0630(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern void  approach_w(void *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern int   func_L00_001F10E0_2E0630(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void  mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void
spawn_color_swapped_explosion_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern AlignedVec4f D_L10_00167240;
extern AlignedVec4f D_L10_00174360_2E0630 __asm__("D_L10_001742E0");

void         screamer_update(Moby *moby) __asm__("FUN_L10_002df270");
extern float D_0015ED6C, D_0015ED70;
extern float fast_cos(float);
extern float fast_sin(float);
extern float fast_vec_dist(void *, void *);

void screamer_update(Moby *moby) {
    enum {
        SCREAMER_STATE_INITIALIZE = 0,
        SCREAMER_STATE_WAIT_FOR_ACTIVATION = 1,
        SCREAMER_STATE_WAIT_TO_RISE = 2,
        SCREAMER_STATE_RISE_FROM_HOME = 4,
        SCREAMER_STATE_FOLLOW_PATH = 5,
        SCREAMER_STATE_FACE_LEADER_AT_WAYPOINT = 6,
        SCREAMER_STATE_IDLE_AT_HOME = 7,
        SCREAMER_STATE_RETURN_HOME = 8,
        SCREAMER_STATE_WAIT_TO_WANDER = 9,
        SCREAMER_STATE_WANDER = 10,
        SCREAMER_STATE_DEATH_KNOCKBACK = 11
    };
    float     waypoint[6];
    D_2E0630 *data = (D_2E0630 *)moby->pvars;
    Moby     *leader;

    func_L10_002E0FF8_2E0630(moby);
    leader = data->leader;
    func_L00_00264B40_2E0630(2.5f, moby, 0, data->sound_state);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, &D_L10_00167240) < 29.0f) {
        func_L00_0025B178_2E0630(moby);
        moby->shadow_range = 0x17;
    }
    moby->anim_speed = 1.0f;
    switch (moby->state) {
    case SCREAMER_STATE_INITIALIZE:
        data->f30 = 0.5f;
        data->f20 = 1.0f;
        data->h24 = 1;
        data->b28 = 0;
        data->b29 = 1;
        moby->flags |= 0x1000;
        qcopy(&data->home, &moby->pos);
        qcopy(&data->wander_goal, &data->home);
        if (data->f174 != 1) {
            mark_moby_for_removal(moby);
        } else {
            moby->state = 1;
        }
        break;
    case SCREAMER_STATE_WAIT_FOR_ACTIVATION:
        if (data->f104 == 2 && data->f174 != 0)
            break;
        moby->state = 2;
        if (moby->prev_seq != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        break;
    case SCREAMER_STATE_WAIT_TO_RISE:
        if ((moby->anim_flags & 2) == 0)
            break;
        moby->state = 4;
        clear_u64_value(&data->vel);
        if (moby->prev_seq != 4)
            blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        break;
    case 3:
        break;
    case SCREAMER_STATE_RISE_FROM_HOME:
        moby->anim_speed = 4.0f;
        if (advance_accelerated_scalar(&moby->pos.z,
                                       data->home.z + 2.0f,
                                       &data->vel.z,
                                       D_0015ED70 * 18.0f,
                                       D_0015ED70 * 18.0f,
                                       D_0015ED6C * 6.0f) == 0.0f) {
            moby->state = 5;
        }
        break;
    case SCREAMER_STATE_FOLLOW_PATH: {
        float distance;
        moby->anim_speed = 4.0f;
        qcopy(waypoint, &l10_paths[data->path]->points[data->node_index]);
        waypoint[5] = fast_vec_length(&data->vel);
        distance = fast_vec_dist(waypoint, &moby->pos);
        *(int *)&waypoint[4] = 0;
        func_L00_0025CE58_2E0630(&moby->rot.z,
                                 fast_arc_tan(waypoint[0] - moby->pos.x, waypoint[1] - moby->pos.y),
                                 &data->yaw_speed,
                                 D_0015ED70 * 25.132742f,
                                 D_0015ED70 * 25.132742f,
                                 D_0015ED6C * 50.265484f);
        if (fast_difference_between_rotations(
                fast_arc_tan(waypoint[0] - moby->pos.x, waypoint[1] - moby->pos.y), moby->rot.z) <
            0.78539819f) {
            advance_accelerated_scalar(&waypoint[4],
                                       distance,
                                       &waypoint[5],
                                       D_0015ED70 * 20.0f,
                                       D_0015ED70 * 20.0f,
                                       D_0015ED6C * 20.0f);
            fast_vec_sub(&data->vel, waypoint, &moby->pos);
            func_L00_002607A8_2E0630(&data->vel, waypoint[5]);
        } else {
            clear_u64_value(&data->vel);
        }
        func_L00_00259B08_2E0630(moby, &data->vel, 0.5f, 0x200, 0.0f, 0);
        if (!(distance < 1.0f))
            break;
        {
            int node_index = data->node_index++;
            if (0.0f < l10_paths[data->path]->points[node_index].f[3]) {
                moby->state = 6;
                if (moby->prev_seq != 3)
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            } else {
                if (node_index + 1 != l10_paths[data->path]->count)
                    break;
                moby->state = 10;
                if (moby->prev_seq != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
                probe_ground_height(&moby->pos, 0, 0.5f);
                qcopy(&data->home, &D_L10_00174360_2E0630);
                qcopy(&data->wander_goal, &moby->pos);
            }
        }
        break;
    }
    case SCREAMER_STATE_FACE_LEADER_AT_WAYPOINT:
        func_L00_0025CE58_2E0630(
            &moby->rot.z,
            fast_arc_tan(leader->pos.x - moby->pos.x, leader->pos.y - moby->pos.y),
            &data->yaw_speed,
            D_0015ED70 * 25.132742f,
            D_0015ED70 * 25.132742f,
            D_0015ED6C * 50.265484f);
        if ((moby->anim_flags & 2) == 0)
            break;
        {
            char *linked =
                (char *)D_L10_0015FFD0_macro[2] +
                (data->linked[truncate_float_to_s32(
                                  l10_paths[data->path]->points[data->node_index - 1].f[3]) -
                              1]
                 << 8);
            if (linked != 0)
                activate_waiting_moby_group_members(linked);
        }
        if (data->node_index == l10_paths[data->path]->count) {
            moby->state = 10;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            probe_ground_height(&moby->pos, 0, 0.5f);
            qcopy(&data->home, &D_L10_00174360_2E0630);
            qcopy(&data->wander_goal, &moby->pos);
        } else {
            moby->state = 5;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    case SCREAMER_STATE_IDLE_AT_HOME:
        break;
    case SCREAMER_STATE_RETURN_HOME: {
        float zero = 0.0f;
        float climb;
        func_L00_0025CC58_2E0630(&moby->rot.y, 0, zero, D_0015ED6C * 12.566371f);
        approach_w(moby,
                   fast_arc_tan(data->home.x - moby->pos.x, data->home.y - moby->pos.y),
                   &data->yaw_speed,
                   0.02f,
                   0.3f,
                   D_0015ED6C * 6.2831855f);
        data->vel.x = fast_cos(moby->rot.z) * (D_0015ED6C * 6.0f);
        data->vel.y = fast_sin(moby->rot.z) * (D_0015ED6C * 6.0f);
        data->vel.z = zero;
        climb = data->home.z + 2.0f - moby->pos.z;
        if (D_0015ED6C * 6.0f < climb) {
            climb = D_0015ED6C * 6.0f;
        } else if (climb < D_0015ED6C * -6.0f) {
            climb = D_0015ED6C * -6.0f;
        }
        data->vel.z = climb;
        func_L00_00259B08_2E0630(moby, &data->vel, 0.5f, 0x200, 0.0f, 0);
        if (vector_distance_xy(&moby->pos, &data->home) < 2.0f)
            moby->state = 7;
        break;
    }
    case SCREAMER_STATE_WAIT_TO_WANDER: {
        float radius;
        float angle;
        float zero;
        if (!fast_dec_timer(&data->wander_timer))
            break;
        zero = 0.0f;
        data->wander_timer = scale_game_frames(0x78);
        radius = random_float_between(zero, 3.0f);
        angle = random_angle_radians();
        waypoint[0] = fast_cos(angle) * radius;
        waypoint[1] = fast_sin(angle) * radius;
        waypoint[2] = zero;
        waypoint[2] = random_float_between(5.0f, 7.0f);
        fast_vec_add(&data->wander_goal, waypoint, &data->home);
        moby->state = 10;
        break;
    }
    case SCREAMER_STATE_WANDER:
        approach_w(
            moby,
            fast_arc_tan(data->wander_goal.x - moby->pos.x, data->wander_goal.y - moby->pos.y),
            &data->yaw_speed,
            0.05f,
            0.3f,
            0.2f);
        fast_vec_sub(&data->step, &data->wander_goal, &moby->pos);
        func_L00_002607A8_2E0630(&data->step, D_0015ED6C * 4.0f);
        fast_vec_add(waypoint, &data->step, &moby->pos);
        if (func_L00_001F10E0_2E0630(0.4f, waypoint, 0, moby)) {
            clear_u64_value(&data->step);
            moby->state = 9;
        } else {
            qcopy(&moby->pos, waypoint);
        }
        if (fast_vec_length(&data->step) == 0.0f)
            moby->state = 9;
        break;
    case SCREAMER_STATE_DEATH_KNOCKBACK:
        if ((step_moby_motion(moby, data->react) & 1) == 0)
            break;
        mark_moby_for_removal_cf(moby, 0x200, -1);
        spawn_color_swapped_explosion_burst(moby, &moby->pos, -1, 0.5f, 10.0f);
        mark_moby_for_removal(moby);
        break;
    }
}

typedef struct Hit_2DFC38 {
    float        x, y;
    char         pad8[8];
    float        f10[4];
    struct Moby *moby;
    char         pad24[8];
    float        f2C;
    int          f30;
} Hit_2DFC38;

typedef struct MobyData_2DFC38 {
    char          pad0[0x20];
    float         f20;
    char          pad24[2];
    short         f26;
    char          pad28[0x10];
    int           f38;
    char          pad3C[0x24];
    char          f60[0x10];
    float         f70, f74, f78, f7C;
    char          pad80[4];
    int           f84;
    char          pad88[0x15];
    unsigned char f9D;
    char          pad9E[0x22];
    char          fC0[0x40];
    int           f100, f104;
    char          pad108[8];
    char          f110[7];
    unsigned char f117;
    char          pad118[0x18];
    char          f130[0x44];
    int           f174;
    float         f178;
    int           f17C;
    float         f180;
    int           f184;
    float         f188;
    int           f18C;
    char          pad190[4];
    int           f194;
} MobyData_2DFC38;

extern char D_0013F3D0[];
extern int  D_001413D0[];
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void func_L00_00261B00_2E2B80(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
extern int  ahead_f(float, void *, void *) __asm__("FUN_L01_00274b78");
extern int
activate_effect_l08(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");

#include "rnc/overlay/l10/mobys/screamer_data/update_screamer_hit_reactions_and_targeting.h"

extern float scale_time(float) __asm__("FUN_001f96b0");
extern void
hit_update_2DFC38(void *, void *, void *, int, void *, int, int, int) __asm__("FUN_00213928");
extern void allocate_voice_for_bank_entry_2DFC38(int, int, int) __asm__("FUN_0022db10");

void update_screamer_hit_reactions_and_targeting(struct Moby *moby) __asm__("FUN_L10_002dfc38");
extern void trooper_anim_update(void *m, void *p) __asm__("FUN_L00_0025d458");

#include "rnc/data_labels.h"

void update_screamer_hit_reactions_and_targeting(struct Moby *moby) {
    MobyData_2DFC38 *data = (MobyData_2DFC38 *)moby->pvars;
    Hit_2DFC38      *hit_record;
    float            relative_offset[4];
    float            transformed_offset[4];
    int              hit_processing_status;
    int              target_selection_status;
    float            knockback_angle;

    if (data->f38 != 0) {
        data->f184 = truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
        data->f38 = 0;
    }
    fast_dec_timer(&data->f184);
    if (moby->state != 11) {
        hit_record = (Hit_2DFC38 *)find_moby_hit(moby, 0x330000, 0);
        hit_update_2DFC38(moby, hit_record, &data->f20, 5, &hit_processing_status, 0, 0, 4);
        if (hit_record != 0) {
            if (data->f20 <= hit_record->f2C) {
                if (screamer_death_message_gate != 0 && item_unlocked[0x10] == 0) {
                    item_unlocked[0x10] = 1;
                    allocate_voice_for_bank_entry_2DFC38(1, 0, 0);
                    queue_dialogue_message(0x53d6, -1);
                }
                data->f20 = 0.0f;
                moby->flags &= 0xEFFF;
                if (moby->prev_seq != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(10));
                moby->state = 11;
                data->f70 = D_0015ED70 * 26.0f;
                data->f9D = 3;
                data->f74 = 0.0005f;
                data->f84 = 9;
                data->f78 = D_0015ED6C * 7.0f;
                data->f7C = D_0015ED6C * 10.0f;
                if (hit_record->f30 & 1)
                    knockback_angle = fast_arc_tan(hit_record->f10[0], hit_record->f10[1]);
                else
                    knockback_angle =
                        fast_arc_tan(moby->pos.x - hit_record->x, moby->pos.y - hit_record->y);
                trooper_start_knockback(moby, data->f60, knockback_angle, 5, 1, 0);
                data->f117 = 0x78;
                trooper_anim_update(moby, data->f110);
                func_L00_00261B00_2E2B80(moby, 1, 3, 0, -1);
            } else {
                data->f20 = data->f20 - hit_record->f2C;
                data->f117 = 0xfa;
                data->f26 = scale_game_frames(60);
                trooper_anim_update(moby, data->f110);
            }
            moby->hit_slot = 0xff;
        }
    }
    trooper_anim_finish(moby, data->f110);
    if (data->f174 == 2)
        data->f180 = 42.0f;
    else if (data->f184 != 0)
        data->f180 = data->f178 + 6.0f;
    else if (moby->state == 1)
        data->f180 = data->f188;
    else
        data->f180 = data->f178;

    if (data->f174 == 2) {
        target_selection_status = ahead_f(data->f180, moby, data->fC0);
        if (target_selection_status != 2) {
            LevelCuboid *bp = &l10_cuboids[data->f194];
            fast_vec_sub(relative_offset, D_0013F3D0, &bp->pos);
            relative_offset[3] = 0.0f;
            transform_vector_by_basis(transformed_offset, relative_offset, bp->to_local);
            if (transformed_offset[0] <= -1.0f || 0.0f <= transformed_offset[0] ||
                transformed_offset[1] <= -1.0f || 1.0f <= transformed_offset[1] ||
                transformed_offset[2] <= -1.0f || 1.0f <= transformed_offset[2])
                data->f104 = 2;
        }
    } else if (moby->state == 1 && data->f18C >= 0) {
        LevelPath *path = l10_paths[data->f18C];
        target_selection_status =
            activate_effect_l08(data->f180, moby, data->fC0, 0, 0, path->points, path->count);
    } else if (moby->state != 1 && data->f17C >= 0) {
        LevelPath *path = l10_paths[data->f17C];
        target_selection_status =
            activate_effect_l08(data->f180, moby, data->fC0, 0, 0, path->points, path->count);
    } else {
        target_selection_status = ahead_f(data->f180, moby, data->fC0);
    }
    if (target_selection_status != 2) {
        if (data->f180 < vector_distance_xy(data->f130, data->fC0))
            data->f104 = 2;
        else if (moby->visible == 0)
            data->f104 = 2;
    }
    if (data->f100 == 0)
        data->f100 = D_001413D0[0];
}

/* Sets every listed entity in state 1 to state 2. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002E14F8), where it is exact; names translated to the US level program. */

extern short *moby_group_member_lists[] __asm__("D_L10_001ABCC0");

void activate_waiting_moby_group_members(struct Moby *moby) {
    short *group_entry;
    if (moby->group != 0xFF) {
        group_entry = moby_group_member_lists[moby->group];
        if (group_entry != 0) {
            do {
                struct Moby *base = D_L10_0015FFD0_macro[2];
                struct Moby *listed_entity =
                    (struct Moby *)((*(unsigned short *)group_entry & 0x7FFF) * 256 + (int)base);
                if (listed_entity->state == 1)
                    listed_entity->state = 2;
            } while (*group_entry++ >= 0);
        }
    }
}
