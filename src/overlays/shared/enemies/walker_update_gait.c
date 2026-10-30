/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/moby.h"

int walker_gait_mask(float *a, float v) __asm__("FUN_L00_002a13f0");

int walker_gait_mask(float *gait_record, float interval_value) {
    int    bit = 1;
    int    result_mask = 0;
    int    i;
    float *interval_cursor = gait_record + 12;
    for (i = 0; i < 2; i++, interval_cursor++) {
        float x = interval_cursor[4], y = interval_cursor[6];
        if (x <= y) {
            if (x <= interval_value && interval_value <= y)
                result_mask |= bit;
        } else {
            if (x <= interval_value || interval_value <= y)
                result_mask |= bit;
        }
        bit <<= 1;
        x = interval_cursor[-2];
        y = interval_cursor[0];
        if (x <= y) {
            if (x <= interval_value && interval_value <= y)
                result_mask |= bit;
        } else {
            if (x <= interval_value || interval_value <= y)
                result_mask |= bit;
        }
        bit <<= 1;
    }
    if (result_mask == 0) {
        if (interval_value < gait_record[6] || gait_record[6] + 1.0f < interval_value)
            result_mask |= bit;
        if (interval_value < gait_record[7] || gait_record[7] + 1.0f < interval_value)
            result_mask |= bit << 1;
    }
    return result_mask;
}

void FUN_L00_001fff28(void *, s32, f32);
void reset_walker_gait_helpers(u8 *gait_data) __asm__("FUN_L00_002a1538");

void reset_walker_gait_helpers(u8 *gait_data) {
    s32 i;
    u8 *gait_helper_record_130_base = gait_data + 0x130;
    u8 *gait_helper_record_base = gait_data + 0x170;
    for (i = 0; i < 2; i++) {
        u8 *gait_region_base = gait_data + i * 0xB0;
        if (gait_region_base[0x161] != 0)
            FUN_L00_001fff28(gait_helper_record_base + i * 0xB0, 2, 0.0f);
        if (gait_region_base[0x121] != 0)
            FUN_L00_001fff28(gait_helper_record_130_base + i * 0xB0, 2, 0.0f);
        *(s32 *)(gait_region_base + 0x104) = 0;
        *(s32 *)(gait_region_base + 0xFC) = 0;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vuchain_002A21A8.c: func_L00_002A2858), where it is exact; names translated to the US level program. */

float compute_walker_time_to_landing(char *walker_moby,
                                     char *gait_data) __asm__("FUN_L00_002a15c8");

float compute_walker_time_to_landing(char *walker_moby, char *gait_data) {
    float root_denominator = *(float *)(gait_data + 0xDC);
    float gap = MOBY(walker_moby)->pos.z - *(float *)(gait_data + 0x68);
    float root_numerator_base = *(float *)(gait_data + 0xD8) + root_denominator * 0.5f;
    float discriminant_or_root =
        root_numerator_base * root_numerator_base + (root_denominator + root_denominator) * gap;
    float time_to_landing;
    if (discriminant_or_root > 0.0f) {
        discriminant_or_root = square_root(discriminant_or_root);
        if (discriminant_or_root <= root_numerator_base) {
            time_to_landing = root_numerator_base - discriminant_or_root;
        } else {
            time_to_landing = root_numerator_base + discriminant_or_root;
        }
        time_to_landing = time_to_landing / *(float *)(gait_data + 0xDC);
    } else {
        time_to_landing = 100000.0f;
    }
    return time_to_landing;
}
/* Steps the walk cycle state machine: picks the next walk, run, turn, jump or landing sequence from speed, heading and foot height. */
/* Ported from rac1-decomp (src/overlays/shared/vuchain_002A21A8.c: func_L00_002A2900), where it is exact; names translated to the US level program. */

typedef struct WalkSeqClass {
    char  pad00[0x48];
    char *seqs[1]; /* 0x48 */
} WalkSeqClass;

typedef struct WalkMoby {
    char          pad00[0x10];
    float         pos[4]; /* 0x10 */
    char          pad20[4];
    WalkSeqClass *pClass; /* 0x24 */
    char          pad28[0x20];
    float         f48; /* 0x48 */
    char          pad4C[4];
    unsigned char b50; /* 0x50 */
    unsigned char b51;
    unsigned char b52; /* 0x52 */
    unsigned char b53;
    float         f54; /* 0x54 */
    float         f58; /* 0x58 */
} WalkMoby;

typedef struct WalkAnim {
    int   id; /* 0x00 */
    float f04;
    float f08;
    float f0C;
    float f10;
    int   i14;
    char  pad18[0x20];
    float f38;
    float f3C;
    float f40;
    float f44;
    float f48;
    float f4C;
} WalkAnim;

typedef struct WalkData {
    char           pad00[0x10];
    float          m10[8]; /* 0x10 */
    float          m30[8]; /* 0x30 */
    char           pad50[0x10];
    float          v60[3]; /* 0x60 */
    float          f6C;
    WalkAnim      *anim[13]; /* 0x70 */
    char           padA4[0xC];
    unsigned char  bB0;
    unsigned char  bB1;
    unsigned char  bB2;
    unsigned char  bB3;
    unsigned char  bB4;
    unsigned char  bB5;
    unsigned char  bB6;
    unsigned char  bB7;
    float          fB8;
    float          fBC;
    float          fC0;
    char           padC4[0x14];
    float          fD8;
    float          fDC;
    float          fE0;
    float          fE4;
    char           padE8[4];
    short          sEC;
    unsigned short uEE;
} WalkData;

extern char  walker_jump_distance_debug_format[] __asm__("D_L00_001611E0");
extern float FUN_L00_0025abf0(float *a, float *b, float *out, float speed, float g);
extern int   walker_gait_animation_index_by_state[] __asm__("D_L00_001CA760");
extern int   find_nearest_animation_sequence_entry(char *a, float time) __asm__("FUN_L00_0024ed18");
extern void  debug_print() __asm__("FUN_001e93b0");
float        compute_walker_time_to_landing(char *a, char *b) __asm__("FUN_L00_002a15c8");
void         reset_walker_gait_helpers(u8 *a0) __asm__("FUN_L00_002a1538");

void walker_update_gait(WalkMoby *m,
                        WalkData *d,
                        int       flags,
                        float     speed,
                        float     ang,
                        float     accel) __asm__("FUN_L00_002a1670");
void walker_update_gait(WalkMoby *walker_moby,
                        WalkData *gait_data,
                        int       flags,
                        float     speed,
                        float     movement_heading,

                        float run_start_input) {
    enum {
        WALKER_GAIT_IDLE = 0,
        WALKER_GAIT_WALK = 1,
        WALKER_GAIT_TRANSITION_TO_UPHILL = 2,
        WALKER_GAIT_ALTERNATE_TRANSITION_TO_UPHILL = 3,
        WALKER_GAIT_WALK_UPHILL = 4,
        WALKER_GAIT_TRANSITION_FROM_UPHILL_TO_WALK = 5,
        WALKER_GAIT_ALTERNATE_TRANSITION_FROM_UPHILL_TO_WALK = 6,
        WALKER_GAIT_TRANSITION_TO_DOWNHILL = 7,
        WALKER_GAIT_ALTERNATE_TRANSITION_TO_DOWNHILL = 8,
        WALKER_GAIT_WALK_DOWNHILL = 9,
        WALKER_GAIT_TRANSITION_FROM_DOWNHILL_TO_WALK = 10,
        WALKER_GAIT_ALTERNATE_TRANSITION_FROM_DOWNHILL_TO_WALK = 11,
        WALKER_GAIT_RESET_TO_IDLE = 12,
        WALKER_GAIT_RUN = 13,
        WALKER_GAIT_ALTERNATE_TAKEOFF_ANIMATION = 14,
        WALKER_GAIT_TAKEOFF_ANIMATION = 15,
        WALKER_GAIT_WAIT_FOR_LANDING = 16,
        WALKER_GAIT_SYNC_ALTERNATE_LANDING_ANIMATION = 17,
        WALKER_GAIT_SYNC_LANDING_ANIMATION = 18,
        WALKER_GAIT_RESUME_RUN_AFTER_ALTERNATE_LANDING = 19,
        WALKER_GAIT_RESUME_RUN_AFTER_LANDING = 20
    };
    float ground_probe_position[4];

    switch (gait_data->bB7) {
    case WALKER_GAIT_IDLE:
        gait_data->fE4 = 0.0f;
        if (run_start_input > 0.0008f && speed > gait_data->anim[0]->f08 * 1.35f) {
            {
                WalkAnim *a = gait_data->anim[7];
                int f = find_nearest_animation_sequence_entry(walker_moby->pClass->seqs[a->id],
                                                              (float)a->i14);
                blend_moby_animation(walker_moby, a->id, f, 10);
                reset_walker_gait_helpers((char *)gait_data);
            }
            gait_data->bB7 = 0xD;
            gait_data->bB6 = walker_gait_mask(gait_data->anim[7], (float)gait_data->anim[7]->i14);
            gait_data->sEC = 0;
        } else if (speed > frame_time * 0.21f ||
                   fast_difference_between_rotations(walker_moby->f48, movement_heading) >
                       0.10471976f) {
            gait_data->sEC++;
            if (gait_data->sEC >= 4) {
                {
                    WalkAnim *a = gait_data->anim[0];
                    int f = find_nearest_animation_sequence_entry(walker_moby->pClass->seqs[a->id],
                                                                  (float)a->i14);
                    blend_moby_animation(walker_moby, a->id, f, 10);
                    reset_walker_gait_helpers((char *)gait_data);
                }
                gait_data->bB7 = 1;
                gait_data->bB6 =
                    walker_gait_mask(gait_data->anim[0], (float)gait_data->anim[0]->i14);
                gait_data->sEC = 0;
            }
        }
        break;
    case WALKER_GAIT_WALK:
    case WALKER_GAIT_TRANSITION_TO_UPHILL:
    case WALKER_GAIT_ALTERNATE_TRANSITION_TO_UPHILL:
    case WALKER_GAIT_WALK_UPHILL:
    case WALKER_GAIT_TRANSITION_FROM_UPHILL_TO_WALK:
    case WALKER_GAIT_ALTERNATE_TRANSITION_FROM_UPHILL_TO_WALK:
        if (flags & 5) {
            gait_data->uEE &= 0xFFF7;
        }
    case WALKER_GAIT_TRANSITION_TO_DOWNHILL:
    case WALKER_GAIT_ALTERNATE_TRANSITION_TO_DOWNHILL:
    case WALKER_GAIT_WALK_DOWNHILL:
    case WALKER_GAIT_TRANSITION_FROM_DOWNHILL_TO_WALK:
    case WALKER_GAIT_ALTERNATE_TRANSITION_FROM_DOWNHILL_TO_WALK: {
        int gait_animation_index = walker_gait_animation_index_by_state[gait_data->bB7];
        if (walker_moby->f58 < 0.25f &&
            fast_difference_between_rotations(walker_moby->f48, movement_heading) < 0.09239978f &&
            speed < frame_time * 0.19f) {
            blend_moby_animation(walker_moby, gait_data->bB0, 0, 10);
            reset_walker_gait_helpers((char *)gait_data);
            gait_data->bB7 = 0;
        } else if (walker_moby->b52 == gait_data->anim[gait_animation_index]->id) {
            float t = (gait_data->anim[7]->f08 + gait_data->anim[0]->f08) * 0.375f;
            if (speed > t && gait_data->fC0 > t && !(gait_data->bB6 & 0x3A)) {
                float current_animation_position = compute_interpolated_record_value(walker_moby);
                int   r1 = truncate_float_to_s32(gait_data->anim[gait_animation_index]->f48 -
                                               current_animation_position +
                                               gait_data->anim[gait_animation_index]->f10) %
                         truncate_float_to_s32(gait_data->anim[gait_animation_index]->f10);
                int r2 = truncate_float_to_s32(gait_data->anim[gait_animation_index]->f4C -
                                               current_animation_position +
                                               gait_data->anim[gait_animation_index]->f10) %
                         truncate_float_to_s32(gait_data->anim[gait_animation_index]->f10);
                int x;
                int bl;
                if (r1 < r2) {
                    x = truncate_float_to_s32(gait_data->anim[7]->f48 - gait_data->anim[7]->f0C);
                } else {
                    x = truncate_float_to_s32(gait_data->anim[7]->f4C - gait_data->anim[7]->f0C);
                }
                x = (x + truncate_float_to_s32(gait_data->anim[7]->f10)) %
                    truncate_float_to_s32(gait_data->anim[7]->f10);
                bl = truncate_float_to_s32(10.0f / (speed / gait_data->anim[7]->f08));
                {
                    float     ft = (float)x;
                    WalkAnim *a = gait_data->anim[7];
                    int       f =
                        find_nearest_animation_sequence_entry(walker_moby->pClass->seqs[a->id], ft);
                    blend_moby_animation(walker_moby, a->id, f, bl);
                    reset_walker_gait_helpers((char *)gait_data);
                }
                gait_data->bB7 = 0xD;
            } else if (flags & 5) {
                if (gait_data->uEE & 1) {
                    gait_data->uEE = (gait_data->uEE & 0xFFFE) | 2;
                    gait_data->fE4 = walker_moby->pos[2];
                    if (flags & 1) {
                        int bl = truncate_float_to_s32(gait_data->anim[11]->f38 * 0.25f /
                                                       (speed / gait_data->anim[11]->f08));
                        int fr = truncate_float_to_s32(gait_data->anim[11]->f40 +
                                                       gait_data->anim[11]->f38 * 0.125f -
                                                       gait_data->anim[11]->f0C) %
                                 truncate_float_to_s32(gait_data->anim[11]->f10);
                        WalkAnim *a = gait_data->anim[11];
                        int       f = find_nearest_animation_sequence_entry(
                            walker_moby->pClass->seqs[a->id], (float)fr);
                        blend_moby_animation(walker_moby, a->id, f, bl);
                        reset_walker_gait_helpers((char *)gait_data);
                        gait_data->bB7 = 0xF;
                    } else {
                        int bl = truncate_float_to_s32(gait_data->anim[9]->f3C * 0.25f /
                                                       (speed / gait_data->anim[9]->f08));
                        int fr = truncate_float_to_s32(gait_data->anim[9]->f44 +
                                                       gait_data->anim[9]->f3C * 0.125f -
                                                       gait_data->anim[9]->f0C) %
                                 truncate_float_to_s32(gait_data->anim[9]->f10);
                        WalkAnim *a = gait_data->anim[9];
                        int       f = find_nearest_animation_sequence_entry(
                            walker_moby->pClass->seqs[a->id], (float)fr);
                        blend_moby_animation(walker_moby, a->id, f, bl);
                        reset_walker_gait_helpers((char *)gait_data);
                        gait_data->bB7 = 0xE;
                    }
                } else {
                    float ground_height;
                    ground_probe_position[0] = fast_cos(movement_heading) *
                                               gait_data->anim[gait_animation_index]->f04 * 0.5f;
                    ground_probe_position[1] = fast_sin(movement_heading) *
                                               gait_data->anim[gait_animation_index]->f04 * 0.5f;
                    ground_probe_position[2] = 0.0f;
                    if (flags & 1) {
                        fast_vec_add(ground_probe_position, ground_probe_position, gait_data->m10);
                    } else {
                        fast_vec_add(ground_probe_position, ground_probe_position, gait_data->m30);
                    }
                    ground_height = probe_ground_height(ground_probe_position, 0, 0.5f);
                    gait_data->fE4 = ground_height;
                    switch (gait_data->bB7) {
                    case WALKER_GAIT_WALK:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) >
                            gait_data->fE0 * 0.75f) {
                            if (flags & 1) {
                                int bl = truncate_float_to_s32(gait_data->anim[2]->f38 * 0.25f /
                                                               (speed / gait_data->anim[2]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[2]->f40 +
                                                               gait_data->anim[2]->f38 * 0.125f -
                                                               gait_data->anim[2]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[2]->f10);
                                WalkAnim *a = gait_data->anim[2];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 3;
                            } else {
                                int bl = truncate_float_to_s32(gait_data->anim[1]->f3C * 0.25f /
                                                               (speed / gait_data->anim[1]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[1]->f44 +
                                                               gait_data->anim[1]->f3C * 0.125f -
                                                               gait_data->anim[1]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[1]->f10);
                                WalkAnim *a = gait_data->anim[1];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 2;
                            }
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) <
                                   -gait_data->fE0 * 0.75f) {
                            gait_data->uEE |= 8;
                            if (flags & 1) {
                                int bl = truncate_float_to_s32(gait_data->anim[5]->f38 * 0.25f /
                                                               (speed / gait_data->anim[5]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[5]->f40 +
                                                               gait_data->anim[5]->f38 * 0.125f -
                                                               gait_data->anim[5]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[5]->f10);
                                WalkAnim *a = gait_data->anim[5];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 8;
                            } else {
                                int bl = truncate_float_to_s32(gait_data->anim[4]->f3C * 0.25f /
                                                               (speed / gait_data->anim[4]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[4]->f44 +
                                                               gait_data->anim[4]->f3C * 0.125f -
                                                               gait_data->anim[4]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[4]->f10);
                                WalkAnim *a = gait_data->anim[4];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 7;
                            }
                        }
                        break;
                    case WALKER_GAIT_TRANSITION_TO_UPHILL:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) <
                            -gait_data->fE0 * 0.75f) {
                            int       bl;
                            int       fr;
                            WalkAnim *a;
                            int       f;
                            gait_data->uEE |= 8;
                            bl = truncate_float_to_s32(gait_data->anim[5]->f38 * 0.25f /
                                                       (speed / gait_data->anim[5]->f08));
                            fr = truncate_float_to_s32(gait_data->anim[5]->f40 +
                                                       gait_data->anim[5]->f38 * 0.125f -
                                                       gait_data->anim[5]->f0C) %
                                 truncate_float_to_s32(gait_data->anim[5]->f10);
                            a = gait_data->anim[5];
                            f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 8;
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) <
                                   gait_data->fE0 * 0.75f) {
                            gait_data->bB7 = 5;
                        } else {
                            int bl = truncate_float_to_s32(gait_data->anim[3]->f38 * 0.25f /
                                                           (speed / gait_data->anim[3]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[3]->f40 +
                                                           gait_data->anim[3]->f38 * 0.125f -
                                                           gait_data->anim[3]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[3]->f10);
                            WalkAnim *a = gait_data->anim[3];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 4;
                        }
                        break;
                    case WALKER_GAIT_ALTERNATE_TRANSITION_TO_UPHILL:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) <
                            -gait_data->fE0 * 0.75f) {
                            int       bl;
                            int       fr;
                            WalkAnim *a;
                            int       f;
                            gait_data->uEE |= 8;
                            bl = truncate_float_to_s32(gait_data->anim[4]->f3C * 0.25f /
                                                       (speed / gait_data->anim[4]->f08));
                            fr = truncate_float_to_s32(gait_data->anim[4]->f44 +
                                                       gait_data->anim[4]->f3C * 0.125f -
                                                       gait_data->anim[4]->f0C) %
                                 truncate_float_to_s32(gait_data->anim[4]->f10);
                            a = gait_data->anim[4];
                            f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 7;
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) <
                                   gait_data->fE0 * 0.75f) {
                            gait_data->bB7 = 6;
                        } else {
                            int bl = truncate_float_to_s32(gait_data->anim[3]->f3C * 0.25f /
                                                           (speed / gait_data->anim[3]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[3]->f44 +
                                                           gait_data->anim[3]->f3C * 0.125f -
                                                           gait_data->anim[3]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[3]->f10);
                            WalkAnim *a = gait_data->anim[3];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 4;
                        }
                        break;
                    case WALKER_GAIT_WALK_UPHILL:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) <
                            -gait_data->fE0 * 0.75f) {
                            gait_data->uEE |= 8;
                            if (flags & 1) {
                                int bl = truncate_float_to_s32(gait_data->anim[5]->f38 * 0.25f /
                                                               (speed / gait_data->anim[5]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[5]->f40 +
                                                               gait_data->anim[5]->f38 * 0.125f -
                                                               gait_data->anim[5]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[5]->f10);
                                WalkAnim *a = gait_data->anim[5];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 8;
                            } else {
                                int bl = truncate_float_to_s32(gait_data->anim[4]->f3C * 0.25f /
                                                               (speed / gait_data->anim[4]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[4]->f44 +
                                                               gait_data->anim[4]->f3C * 0.125f -
                                                               gait_data->anim[4]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[4]->f10);
                                WalkAnim *a = gait_data->anim[4];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 7;
                            }
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) <
                                   gait_data->fE0 * 0.75f) {
                            if (flags & 1) {
                                int bl = truncate_float_to_s32(gait_data->anim[1]->f38 * 0.25f /
                                                               (speed / gait_data->anim[1]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[1]->f40 +
                                                               gait_data->anim[1]->f38 * 0.125f -
                                                               gait_data->anim[1]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[1]->f10);
                                WalkAnim *a = gait_data->anim[1];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 5;
                            } else {
                                int bl = truncate_float_to_s32(gait_data->anim[2]->f3C * 0.25f /
                                                               (speed / gait_data->anim[2]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[2]->f44 +
                                                               gait_data->anim[2]->f3C * 0.125f -
                                                               gait_data->anim[2]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[2]->f10);
                                WalkAnim *a = gait_data->anim[2];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 6;
                            }
                        }
                        break;
                    case WALKER_GAIT_TRANSITION_FROM_UPHILL_TO_WALK:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) <
                            -gait_data->fE0 * 0.75f) {
                            int       bl;
                            int       fr;
                            WalkAnim *a;
                            int       f;
                            gait_data->uEE |= 8;
                            bl = truncate_float_to_s32(gait_data->anim[4]->f3C * 0.25f /
                                                       (speed / gait_data->anim[4]->f08));
                            fr = truncate_float_to_s32(gait_data->anim[4]->f44 +
                                                       gait_data->anim[4]->f3C * 0.125f -
                                                       gait_data->anim[4]->f0C) %
                                 truncate_float_to_s32(gait_data->anim[4]->f10);
                            a = gait_data->anim[4];
                            f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 7;
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) >
                                   gait_data->fE0 * 0.75f) {
                            gait_data->bB7 = 2;
                        } else {
                            int bl = truncate_float_to_s32(gait_data->anim[0]->f3C * 0.25f /
                                                           (speed / gait_data->anim[0]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[0]->f44 +
                                                           gait_data->anim[0]->f3C * 0.125f -
                                                           gait_data->anim[0]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[0]->f10);
                            WalkAnim *a = gait_data->anim[0];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 1;
                        }
                        break;
                    case WALKER_GAIT_ALTERNATE_TRANSITION_FROM_UPHILL_TO_WALK:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) <
                            -gait_data->fE0 * 0.75f) {
                            int       bl;
                            int       fr;
                            WalkAnim *a;
                            int       f;
                            gait_data->uEE |= 8;
                            bl = truncate_float_to_s32(gait_data->anim[5]->f38 * 0.25f /
                                                       (speed / gait_data->anim[5]->f08));
                            fr = truncate_float_to_s32(gait_data->anim[5]->f40 +
                                                       gait_data->anim[5]->f38 * 0.125f -
                                                       gait_data->anim[5]->f0C) %
                                 truncate_float_to_s32(gait_data->anim[5]->f10);
                            a = gait_data->anim[5];
                            f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 8;
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) >
                                   gait_data->fE0 * 0.75f) {
                            gait_data->bB7 = 3;
                        } else {
                            int bl = truncate_float_to_s32(gait_data->anim[0]->f38 * 0.25f /
                                                           (speed / gait_data->anim[0]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[0]->f40 +
                                                           gait_data->anim[0]->f38 * 0.125f -
                                                           gait_data->anim[0]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[0]->f10);
                            WalkAnim *a = gait_data->anim[0];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 1;
                        }
                        break;
                    case WALKER_GAIT_TRANSITION_TO_DOWNHILL:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) >
                            gait_data->fE0 * 0.75f) {
                            int bl = truncate_float_to_s32(gait_data->anim[2]->f38 * 0.25f /
                                                           (speed / gait_data->anim[2]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[2]->f40 +
                                                           gait_data->anim[2]->f38 * 0.125f -
                                                           gait_data->anim[2]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[2]->f10);
                            WalkAnim *a = gait_data->anim[2];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 3;
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) >
                                   -gait_data->fE0 * 0.75f) {
                            gait_data->bB7 = 10;
                        } else {
                            int bl = truncate_float_to_s32(gait_data->anim[6]->f38 * 0.25f /
                                                           (speed / gait_data->anim[6]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[6]->f40 +
                                                           gait_data->anim[6]->f38 * 0.125f -
                                                           gait_data->anim[6]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[6]->f10);
                            WalkAnim *a = gait_data->anim[6];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 9;
                        }
                        break;
                    case WALKER_GAIT_ALTERNATE_TRANSITION_TO_DOWNHILL:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) >
                            gait_data->fE0 * 0.75f) {
                            int bl = truncate_float_to_s32(gait_data->anim[1]->f3C * 0.25f /
                                                           (speed / gait_data->anim[1]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[1]->f44 +
                                                           gait_data->anim[1]->f3C * 0.125f -
                                                           gait_data->anim[1]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[1]->f10);
                            WalkAnim *a = gait_data->anim[1];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 2;
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) >
                                   -gait_data->fE0 * 0.75f) {
                            gait_data->bB7 = 11;
                        } else {
                            int bl = truncate_float_to_s32(gait_data->anim[6]->f3C * 0.25f /
                                                           (speed / gait_data->anim[6]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[6]->f44 +
                                                           gait_data->anim[6]->f3C * 0.125f -
                                                           gait_data->anim[6]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[6]->f10);
                            WalkAnim *a = gait_data->anim[6];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 9;
                        }
                        break;
                    case WALKER_GAIT_WALK_DOWNHILL:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) >
                            gait_data->fE0 * 0.75f) {
                            if (flags & 1) {
                                int bl = truncate_float_to_s32(gait_data->anim[2]->f38 * 0.25f /
                                                               (speed / gait_data->anim[2]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[2]->f40 +
                                                               gait_data->anim[2]->f38 * 0.125f -
                                                               gait_data->anim[2]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[2]->f10);
                                WalkAnim *a = gait_data->anim[2];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 3;
                            } else {
                                int bl = truncate_float_to_s32(gait_data->anim[1]->f3C * 0.25f /
                                                               (speed / gait_data->anim[1]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[1]->f44 +
                                                               gait_data->anim[1]->f3C * 0.125f -
                                                               gait_data->anim[1]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[1]->f10);
                                WalkAnim *a = gait_data->anim[1];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 2;
                            }
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) >
                                   -gait_data->fE0 * 0.75f) {
                            if (flags & 1) {
                                int bl = truncate_float_to_s32(gait_data->anim[4]->f38 * 0.25f /
                                                               (speed / gait_data->anim[4]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[4]->f40 +
                                                               gait_data->anim[4]->f38 * 0.125f -
                                                               gait_data->anim[4]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[4]->f10);
                                WalkAnim *a = gait_data->anim[4];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 10;
                            } else {
                                int bl = truncate_float_to_s32(gait_data->anim[5]->f3C * 0.25f /
                                                               (speed / gait_data->anim[5]->f08));
                                int fr = truncate_float_to_s32(gait_data->anim[5]->f44 +
                                                               gait_data->anim[5]->f3C * 0.125f -
                                                               gait_data->anim[5]->f0C) %
                                         truncate_float_to_s32(gait_data->anim[5]->f10);
                                WalkAnim *a = gait_data->anim[5];
                                int       f = find_nearest_animation_sequence_entry(
                                    walker_moby->pClass->seqs[a->id], (float)fr);
                                blend_moby_animation(walker_moby, a->id, f, bl);
                                reset_walker_gait_helpers((char *)gait_data);
                                gait_data->bB7 = 11;
                            }
                        }
                        break;
                    case WALKER_GAIT_TRANSITION_FROM_DOWNHILL_TO_WALK:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) >
                            gait_data->fE0 * 0.75f) {
                            int bl = truncate_float_to_s32(gait_data->anim[1]->f3C * 0.25f /
                                                           (speed / gait_data->anim[1]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[1]->f44 +
                                                           gait_data->anim[1]->f3C * 0.125f -
                                                           gait_data->anim[1]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[1]->f10);
                            WalkAnim *a = gait_data->anim[1];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 2;
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) <
                                   -gait_data->fE0 * 0.75f) {
                            gait_data->bB7 = 7;
                        } else {
                            int bl = truncate_float_to_s32(gait_data->anim[0]->f3C /
                                                           (speed / gait_data->anim[0]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[0]->f44 +
                                                           gait_data->anim[0]->f3C * 0.5f -
                                                           gait_data->anim[0]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[0]->f10);
                            WalkAnim *a = gait_data->anim[0];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 1;
                        }
                        break;
                    case WALKER_GAIT_ALTERNATE_TRANSITION_FROM_DOWNHILL_TO_WALK:
                        if ((ground_height - ground_probe_position[2]) +
                                gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                    fast_sin(gait_data->fB8) >
                            gait_data->fE0 * 0.75f) {
                            int bl = truncate_float_to_s32(gait_data->anim[2]->f38 * 0.25f /
                                                           (speed / gait_data->anim[2]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[2]->f40 +
                                                           gait_data->anim[2]->f38 * 0.125f -
                                                           gait_data->anim[2]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[2]->f10);
                            WalkAnim *a = gait_data->anim[2];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 3;
                        } else if ((ground_height - ground_probe_position[2]) +
                                       gait_data->anim[gait_animation_index]->f04 * 0.5f *
                                           fast_sin(gait_data->fB8) <
                                   -gait_data->fE0 * 0.75f) {
                            gait_data->bB7 = 8;
                        } else {
                            int bl = truncate_float_to_s32(gait_data->anim[0]->f38 /
                                                           (speed / gait_data->anim[0]->f08));
                            int fr = truncate_float_to_s32(gait_data->anim[0]->f40 +
                                                           gait_data->anim[0]->f38 * 0.5f -
                                                           gait_data->anim[0]->f0C) %
                                     truncate_float_to_s32(gait_data->anim[0]->f10);
                            WalkAnim *a = gait_data->anim[0];
                            int       f = find_nearest_animation_sequence_entry(
                                walker_moby->pClass->seqs[a->id], (float)fr);
                            blend_moby_animation(walker_moby, a->id, f, bl);
                            reset_walker_gait_helpers((char *)gait_data);
                            gait_data->bB7 = 1;
                        }
                        break;
                    }
                }
            }
        }
        break;
    }
    case WALKER_GAIT_RUN: {
        float t = (gait_data->anim[7]->f08 + gait_data->anim[0]->f08) * 0.35f;
        if (walker_moby->b52 == gait_data->anim[7]->id && !(gait_data->bB6 & 0x30)) {
            if (speed < t && gait_data->fC0 <= t) {
                int fr;
                if (gait_data->bB6 & 3) {
                    fr = truncate_float_to_s32(gait_data->anim[0]->f38 * 2.0f / 3.0f +
                                               gait_data->anim[0]->f40 - gait_data->anim[0]->f0C) %
                         truncate_float_to_s32(gait_data->anim[0]->f10);
                } else {
                    fr = truncate_float_to_s32(gait_data->anim[0]->f3C * 2.0f / 3.0f +
                                               gait_data->anim[0]->f44 - gait_data->anim[0]->f0C) %
                         truncate_float_to_s32(gait_data->anim[0]->f10);
                }
                {
                    WalkAnim *a = gait_data->anim[0];
                    float     ft = (float)fr;
                    int       f =
                        find_nearest_animation_sequence_entry(walker_moby->pClass->seqs[a->id], ft);
                    blend_moby_animation(walker_moby, a->id, f, 10);
                    reset_walker_gait_helpers((char *)gait_data);
                }
                gait_data->bB7 = 1;
            } else if (flags & 5) {
                if (gait_data->uEE & 1) {
                    gait_data->uEE = (gait_data->uEE & 0xFFFE) | 2;
                    gait_data->fE4 = walker_moby->pos[2];
                    if (flags & 1) {
                        int bl = truncate_float_to_s32(gait_data->anim[11]->f38 * 0.25f /
                                                       (speed / gait_data->anim[11]->f08));
                        int fr = truncate_float_to_s32(gait_data->anim[11]->f40 +
                                                       gait_data->anim[11]->f38 * 0.125f -
                                                       gait_data->anim[11]->f0C) %
                                 truncate_float_to_s32(gait_data->anim[11]->f10);
                        WalkAnim *a = gait_data->anim[11];
                        int       f = find_nearest_animation_sequence_entry(
                            walker_moby->pClass->seqs[a->id], (float)fr);
                        blend_moby_animation(walker_moby, a->id, f, bl);
                        reset_walker_gait_helpers((char *)gait_data);
                        gait_data->bB7 = 0xF;
                    } else {
                        int bl = truncate_float_to_s32(gait_data->anim[9]->f3C * 0.25f /
                                                       (speed / gait_data->anim[9]->f08));
                        int fr = truncate_float_to_s32(gait_data->anim[9]->f44 +
                                                       gait_data->anim[9]->f3C * 0.125f -
                                                       gait_data->anim[9]->f0C) %
                                 truncate_float_to_s32(gait_data->anim[9]->f10);
                        WalkAnim *a = gait_data->anim[9];
                        int       f = find_nearest_animation_sequence_entry(
                            walker_moby->pClass->seqs[a->id], (float)fr);
                        blend_moby_animation(walker_moby, a->id, f, bl);
                        reset_walker_gait_helpers((char *)gait_data);
                        gait_data->bB7 = 0xE;
                    }
                } else {
                    ground_probe_position[0] =
                        fast_cos(movement_heading) * gait_data->anim[0]->f04 * 0.5f;
                    ground_probe_position[1] =
                        fast_sin(movement_heading) * gait_data->anim[0]->f04 * 0.5f;
                    ground_probe_position[2] = 0.0f;
                    if (flags & 1) {
                        fast_vec_add(ground_probe_position, ground_probe_position, gait_data->m10);
                    } else {
                        fast_vec_add(ground_probe_position, ground_probe_position, gait_data->m30);
                    }
                    ground_probe_position[2] += 1.0f;
                    gait_data->fE4 = probe_ground_height(ground_probe_position, 0, 0.5f);
                }
            }
        }
        if (gait_data->fC0 < t) {
            gait_data->fC0 = t;
        }
        break;
    }
    case WALKER_GAIT_RESET_TO_IDLE:
        blend_moby_animation(walker_moby, gait_data->bB0, 0, 10);
        reset_walker_gait_helpers((char *)gait_data);
        gait_data->bB7 = 0;
        break;
    case WALKER_GAIT_TAKEOFF_ANIMATION:
        if (flags & 0x30) {
            float dist = vector_distance_xy(walker_moby->pos, gait_data->v60);
            debug_print(walker_jump_distance_debug_format, dist);
            gait_data->fD8 = FUN_L00_0025abf0(
                walker_moby->pos, gait_data->v60, 0, gait_data->fC0, -gait_data->fDC);
            gait_data->uEE |= 4;
            gait_data->f6C = fast_arc_tan(gait_data->v60[0] - walker_moby->pos[0],
                                          gait_data->v60[1] - walker_moby->pos[1]);
        }
        if (compute_interpolated_record_value(walker_moby) >=
            gait_data->anim[11]->f0C + gait_data->anim[11]->f10 - walker_moby->f58) {
            blend_moby_animation(walker_moby, gait_data->bB3, 0, 10);
            reset_walker_gait_helpers((char *)gait_data);
            gait_data->bB7 = 0x10;
        } else if ((gait_data->uEE & 4) && gait_data->fD8 < 0.0f) {
            float t = compute_walker_time_to_landing((char *)walker_moby, (char *)gait_data);
            float len = (gait_data->anim[12]->f44 - gait_data->anim[12]->f0C) * 2.0f;
            if (t < len + 15.0f) {
                int       bl = truncate_float_to_s32(t - len) >> 1;
                WalkAnim *a;
                int       f;
                if (bl < 0) {
                    bl = 0;
                }
                a = gait_data->anim[12];
                f = find_nearest_animation_sequence_entry(walker_moby->pClass->seqs[a->id], 0.0f);
                blend_moby_animation(walker_moby, a->id, f, bl);
                reset_walker_gait_helpers((char *)gait_data);
                gait_data->bB7 = 0x12;
            }
        }
        break;
    case WALKER_GAIT_ALTERNATE_TAKEOFF_ANIMATION:
        if (flags & 0x30) {
            float dist = fast_vec_dist(walker_moby->pos, gait_data->v60);
            debug_print(walker_jump_distance_debug_format, dist);
            gait_data->fD8 = FUN_L00_0025abf0(
                walker_moby->pos, gait_data->v60, 0, gait_data->fC0, -gait_data->fDC);
            gait_data->uEE |= 4;
            gait_data->f6C = fast_arc_tan(gait_data->v60[0] - walker_moby->pos[0],
                                          gait_data->v60[1] - walker_moby->pos[1]);
        }
        if (compute_interpolated_record_value(walker_moby) >=
            gait_data->anim[9]->f0C + gait_data->anim[9]->f10 - walker_moby->f58) {
            blend_moby_animation(walker_moby, gait_data->bB3, 0, 10);
            reset_walker_gait_helpers((char *)gait_data);
            gait_data->bB7 = 0x10;
        } else if ((gait_data->uEE & 4) && gait_data->fD8 < 0.0f) {
            float t = compute_walker_time_to_landing((char *)walker_moby, (char *)gait_data);
            float len = (gait_data->anim[10]->f44 - gait_data->anim[10]->f0C) * 2.0f;
            if (t < len + 15.0f) {
                int       bl = truncate_float_to_s32(t - len) >> 1;
                WalkAnim *a;
                int       f;
                if (bl < 0) {
                    bl = 0;
                }
                a = gait_data->anim[10];
                f = find_nearest_animation_sequence_entry(walker_moby->pClass->seqs[a->id], 0.0f);
                blend_moby_animation(walker_moby, a->id, f, bl);
                reset_walker_gait_helpers((char *)gait_data);
                gait_data->bB7 = 0x11;
            }
        }
        break;
    case WALKER_GAIT_WAIT_FOR_LANDING:
        if (walker_moby->b52 == gait_data->bB3) {
            float t = compute_walker_time_to_landing((char *)walker_moby, (char *)gait_data);
            float len = (gait_data->anim[12]->f44 - gait_data->anim[12]->f0C) * 2.0f;
            if (t < len + 15.0f) {
                int       bl = truncate_float_to_s32(t - len) >> 1;
                WalkAnim *a = gait_data->anim[12];
                int       f =
                    find_nearest_animation_sequence_entry(walker_moby->pClass->seqs[a->id], 0.0f);
                blend_moby_animation(walker_moby, a->id, f, bl);
                reset_walker_gait_helpers((char *)gait_data);
                gait_data->bB7 = 0x12;
            }
        }
        break;
    case WALKER_GAIT_SYNC_LANDING_ANIMATION:
        if (walker_moby->b52 == gait_data->anim[12]->id) {
            float t = compute_walker_time_to_landing((char *)walker_moby, (char *)gait_data);
            if (t > 1.0f) {
                float base = convert_integer_to_float(walker_moby->b50) + walker_moby->f54;
                t = gait_data->anim[12]->f44 + 2.0f - t * 0.5f - gait_data->anim[12]->f0C;
                if (t < base + 0.1f) {
                    t = base + 0.1f;
                }
                walker_moby->f58 = frame_scale_inv * (t - base);
            } else {
                float base = convert_integer_to_float(walker_moby->b50) + walker_moby->f54;
                t = gait_data->anim[12]->f44 - gait_data->anim[12]->f0C;
                if (t < base + 0.1f) {
                    t = base + 0.1f;
                }
                walker_moby->f58 = frame_scale_inv * (t - base);
                {
                    int ns = 0x14;
                    int fl = 4;
                    gait_data->bB6 = fl;
                    gait_data->bB7 = ns;
                }
                gait_data->uEE &= 0xFFFB;
            }
        }
        break;
    case WALKER_GAIT_SYNC_ALTERNATE_LANDING_ANIMATION:
        if (walker_moby->b52 == gait_data->anim[10]->id) {
            float t = compute_walker_time_to_landing((char *)walker_moby, (char *)gait_data);
            if (t > 1.0f) {
                float base = convert_integer_to_float(walker_moby->b50) + walker_moby->f54;
                t = gait_data->anim[10]->f44 + 2.0f - t * 0.5f - gait_data->anim[10]->f0C;
                if (t < base + 0.1f) {
                    t = base + 0.1f;
                }
                walker_moby->f58 = frame_scale_inv * (t - base);
            } else {
                float base = convert_integer_to_float(walker_moby->b50) + walker_moby->f54;
                t = gait_data->anim[10]->f44 - gait_data->anim[10]->f0C;
                if (t < base + 0.1f) {
                    t = base + 0.1f;
                }
                walker_moby->f58 = frame_scale_inv * (t - base);
                {
                    int ns = 0x13;
                    int fl = 4;
                    gait_data->bB6 = fl;
                    gait_data->bB7 = ns;
                }
                gait_data->uEE &= 0xFFFB;
            }
        }
        break;
    case WALKER_GAIT_RESUME_RUN_AFTER_LANDING:
        if (flags & 1) {
            int bl = truncate_float_to_s32((gait_data->anim[7]->f38 * 0.25f + 2.0f) /
                                           (speed / gait_data->anim[7]->f08));
            int fr =
                truncate_float_to_s32(gait_data->anim[7]->f40 + 1.0f +
                                      gait_data->anim[7]->f38 * 0.125f - gait_data->anim[7]->f0C) %
                truncate_float_to_s32(gait_data->anim[7]->f10);
            WalkAnim *a = gait_data->anim[7];
            int       f =
                find_nearest_animation_sequence_entry(walker_moby->pClass->seqs[a->id], (float)fr);
            blend_moby_animation(walker_moby, a->id, f, bl);
            reset_walker_gait_helpers((char *)gait_data);
            gait_data->fD8 = 0.0f;
            gait_data->bB7 = 0xD;
            gait_data->bB6 = 1;
            gait_data->uEE &= 0xFFFD;
            ground_probe_position[0] = fast_cos(movement_heading) * gait_data->anim[7]->f04 * 0.5f;
            ground_probe_position[1] = fast_sin(movement_heading) * gait_data->anim[7]->f04 * 0.5f;
            ground_probe_position[2] = 0.0f;
            fast_vec_add(ground_probe_position, ground_probe_position, gait_data->m10);
            ground_probe_position[2] += 1.0f;
            gait_data->fE4 = probe_ground_height(ground_probe_position, 0, 0.5f);
        }
        break;
    case WALKER_GAIT_RESUME_RUN_AFTER_ALTERNATE_LANDING:
        if (flags & 1) {
            int bl = truncate_float_to_s32((gait_data->anim[7]->f3C * 0.25f + 2.0f) /
                                           (speed / gait_data->anim[7]->f08));
            int fr =
                truncate_float_to_s32(gait_data->anim[7]->f44 + 1.0f +
                                      gait_data->anim[7]->f38 * 0.125f - gait_data->anim[7]->f0C) %
                truncate_float_to_s32(gait_data->anim[7]->f10);
            WalkAnim *a = gait_data->anim[7];
            int       f =
                find_nearest_animation_sequence_entry(walker_moby->pClass->seqs[a->id], (float)fr);
            blend_moby_animation(walker_moby, a->id, f, bl);
            reset_walker_gait_helpers((char *)gait_data);
            gait_data->fD8 = 0.0f;
            gait_data->bB7 = 0xD;
            gait_data->bB6 = 1;
            gait_data->uEE &= 0xFFFD;
            ground_probe_position[0] = fast_cos(movement_heading) * gait_data->anim[7]->f04 * 0.5f;
            ground_probe_position[1] = fast_sin(movement_heading) * gait_data->anim[7]->f04 * 0.5f;
            ground_probe_position[2] = 0.0f;
            fast_vec_add(ground_probe_position, ground_probe_position, gait_data->m10);
            ground_probe_position[2] += 1.0f;
            gait_data->fE4 = probe_ground_height(ground_probe_position, 0, 0.5f);
        }
        break;
    }
}
