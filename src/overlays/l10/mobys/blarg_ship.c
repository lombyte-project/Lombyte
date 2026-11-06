/* Level 10 moby class blarg_ship: rises, follows its path, hovers and fires at its target. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/overlay/l10/mobys/aligned_vector.h"

enum {
    BLARG_SHIP_STATE_INITIALIZE = 0,
    BLARG_SHIP_STATE_WAIT_FOR_TARGET_IN_VOLUME = 1,
    BLARG_SHIP_STATE_RISE_TO_ENTRY_PATH = 2,
    BLARG_SHIP_STATE_FOLLOW_ENTRY_PATH = 3,
    BLARG_SHIP_STATE_FIRE_AT_TARGET = 4,
    BLARG_SHIP_STATE_FIRE_COOLDOWN = 5,
    BLARG_SHIP_STATE_FLY_AWAY = 6,
    BLARG_SHIP_STATE_HIT_REACTION = 7,
    BLARG_SHIP_STATE_EXPLODE = 8
};

enum {
    HIT_STATUS_SKIP_DAMAGE_AND_REACTION = 1
};

typedef struct Moby Moby;
extern float        D_0015ED6C;
extern float        D_0015ED6C, D_0015ED70;
extern void         start_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d458");
extern void         update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern void         FUN_L00_0024f7c8(void *, int, void *);
s32 is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");

extern float FUN_L04_001f1978(float);
extern int   resolve_moby_hit_response() __asm__("FUN_00213928");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void start_hit_color_flash(void *m, void *p) __asm__("FUN_L00_0025d458");

extern char D_L10_00167240[];

#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/game/game_globals.h"

extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);

typedef int u128_2E52B0 __attribute__((mode(TI)));
/* Update for the blarg ship (moby class 1229): rise, follow its path, hover and fire at the target, fly off or explode. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E5E48), where it is exact; names translated to the US level program. */

typedef struct Ship_5E48 {
    char               p00[0x10];
    AlignedVec4f       pos;
    unsigned char      state;
    char               p21[3];
    char              *p24;
    char               p28[4];
    float              scale;
    char               p30;
    unsigned char      b31;
    short              s32;
    unsigned short     s34;
    char               p36[0xE];
    float              pitch;
    float              heading;
    char               p4C[6];
    unsigned char      b52;
    unsigned char      requested_animation_sequence;
    char               p54[4];
    float              animation_speed;
    char               p5C[0x14];
    unsigned char      b70;
    char               p71[7];
    struct ShipD_5E48 *d;
    char               p7C[3];
    unsigned char      b7F;
    char               p80[0x14];
    int                i94;
    char               p98[0xE];
    short              moby_class_id;
    unsigned short     maya_instance_id;
    char               pAA[6];
    unsigned char      mission_index;
    char               pB1[0xB];
    unsigned char      bBC;
} Ship_5E48;

typedef struct ShipD_5E48 {
    char          p0[0x20];
    float         health;
    short         max_health;
    char          p26[2];
    unsigned char hit_response_class;
    unsigned char b29;
    char          p2A[0x16];
    char          a40[0xB0];
    AlignedVec4f  target_aim_position;
    char          p100[0x10];
    Ship_5E48    *tgt;
    int           targeting_status;
    char          p118[8];
    AlignedVec4f  a120;
    char          p130[0x10];
    int           entry_path_index;
    int           firing_path_index;
    int           activation_volume_index;
    char          p14C[0xC];
    int           path_point_index;
    char          p15C[4];
    float         firing_duration_seconds;
    float         cooldown_duration_seconds;
    int           firing_phase_timer;
    char          p16C[0x2A];
    short         s196;
    float         heading_turn_velocity;
    float         pitch_turn_velocity;
    char          p1A0[0x10];
    int           departure_volume_index;
    char          p1B4[0x1C];
    unsigned char a1D0[4];
} ShipD_5E48;

extern Ship_5E48 *D_L10_001742D8 NOT_SDA;
extern char                     *spawn_blarg_ship_rocket(
                        void *vel, void *pos, int a, int b, int c, float scale) __asm__("FUN_L10_002cc1b8");
extern char D_L10_00179890[];

char blarg_ship_missing_path_error_format[65] __asm__("D_L10_001DD390") =
    "ERROR!! Moby class %d maya instance %d has no path set; deleted\n";

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float fast_vec_dist(void *, void *);
extern s32   D_0015ED84;
extern int   D_L10_00161EE4 __attribute__((sda));

#include "rnc/overlay/l10/mobys/blarg_ship_globals.h"

float                blarg_ship_rocket_speed_gp __asm__("D_L10_00161EE8") = 32.0f;
float                blarg_ship_firing_animation_speed_gp __asm__("D_L10_00161EEC") = 2.0f;
extern unsigned char D_0013D4CB[];
extern unsigned char D_001413F4[];
extern unsigned char D_00141C48[];
extern void         *FUN_L00_00263fd8_c(char *,
                                        int,
                                        float *,
                                        void *,
                                        int,
                                        int,
                                        float,
                                        float *,
                                        float *,
                                        float *) __asm__("FUN_L00_00263fd8");
extern void          DebugPrint(char *, ...);
extern void          spawn_explosion(void *,
                                     void *,
                                     void *,
                                     float,
                                     float,
                                     int,
                                     int,
                                     int,
                                     float,
                                     float,
                                     float,
                                     float,
                                     int,
                                     float,
                                     int,
                                     int,
                                     int,
                                     int) __asm__("FUN_L00_0025e450");
extern void          FUN_L00_0025f730_c(void *, float) __asm__("FUN_L00_0025f730");
extern void          emit_radial_particle_burst(void *, void *, int) __asm__("FUN_L03_00250ae8");
extern void          FUN_L10_002e5968_c(char *m) __asm__("FUN_L10_002e5968");
extern int           D_0015ED84;

void blarg_ship_update(Ship_5E48 *m) __asm__("FUN_L10_002e4a88");

/* Moby class 1229 */
void blarg_ship_update(Ship_5E48 *ship) {
    enum {
        BLARG_SHIP_TARGETING_FIRE_BLOCKED = 2
    };
    ShipD_5E48  *d = ship->d;
    Ship_5E48   *tgt;
    AlignedVec4f A;
    AlignedVec4f B;
    AlignedVec4f C;
    AlignedVec4f E;
    AlignedVec4f D;
    float        desired_heading;
    float        aim_elevation;

    FUN_L00_00263ac8(2.5f, (int)ship, 2, d->a1D0);
    if (d->entry_path_index == -1) {
        DebugPrint(
            blarg_ship_missing_path_error_format, ship->moby_class_id, ship->maya_instance_id);
        goto del;
    }
    FUN_L10_002e5968_c((char *)ship);
    tgt = d->tgt;
    switch (ship->state) {
    case BLARG_SHIP_STATE_INITIALIZE:
        ship->s34 |= 0x1000;
        d->health = 9.0f;
        d->max_health = truncate_float_to_s32(9.0f);
        d->hit_response_class = 3;
        d->b29 = 0;
        d->pitch_turn_velocity = 0.0f;
        ship->s32 = 0x7F;
        qcopy(&ship->pos, &l10_paths[d->entry_path_index]->points[0]);
        d->s196 = scale_game_frames(D_L10_00161EE4);
        ship->i94 = 0;
        ship->s34 |= 0x41;
        if (ship->requested_animation_sequence != 0)
            blend_moby_animation(ship, 0, 0, 0);
        ship->state = BLARG_SHIP_STATE_WAIT_FOR_TARGET_IN_VOLUME;
        if (visited52[ship->mission_index + (D_0015ED84 << 4)] != 0xFF)
            break;
        goto del;
    case BLARG_SHIP_STATE_WAIT_FOR_TARGET_IN_VOLUME:
        if (!is_point_inside_clip_volume(&tgt->pos, d->activation_volume_index))
            break;
        ship->state = BLARG_SHIP_STATE_RISE_TO_ENTRY_PATH;
        if (ship->requested_animation_sequence != 1)
            blend_moby_animation(ship, 1, 0, scale_game_frames(20));
        ship->s34 &= 0xFFBE;
        ship->i94 = *(int *)(ship->p24 + 0x10);
        if (*(int *)D_L10_00179890 == 0 && *(int *)(D_L10_00179890 + 0x24) == -1 &&
            D_001413F4[0] == 0 && *(unsigned short *)D_00141C48 == 0 && D_0013D4CB[0] == 0) {
            help_request(0x2715, 0x5C);
        }
        break;
    case BLARG_SHIP_STATE_RISE_TO_ENTRY_PATH:
        desired_heading = fast_arc_tan(tgt->pos.x - ship->pos.x, tgt->pos.y - ship->pos.y);
        FUN_L00_00258278((char *)ship,
                         &d->heading_turn_velocity,
                         desired_heading,
                         0.02f,
                         0.3f,
                         D_0015ED6C * 3.1415927f);
        aim_elevation =
            fast_arc_tan(vector_distance_xy(&ship->pos, &tgt->pos), tgt->pos.z - ship->pos.z);
        ship->pitch = spring_turn(&d->pitch_turn_velocity,
                                  ship->pitch,
                                  -aim_elevation,
                                  0.02f,
                                  0.3f,
                                  D_0015ED6C * 3.1415927f);
        if ((ship->b70 & 2) && ship->requested_animation_sequence == 1)
            blend_moby_animation(ship, 2, 0, scale_game_frames(20));
        ship->pos.z += D_0015ED6C * 4.0f;
        if (fast_vec_dist(&ship->pos, &l10_paths[d->entry_path_index]->points[0]) < 0.5f) {
            ship->state = BLARG_SHIP_STATE_FOLLOW_ENTRY_PATH;
            if (ship->requested_animation_sequence != 2)
                blend_moby_animation(ship, 2, 0, scale_game_frames(10));
        }
        break;
    case BLARG_SHIP_STATE_FOLLOW_ENTRY_PATH:
        qcopy(&A, &l10_paths[d->entry_path_index]->points[d->path_point_index]);
        desired_heading = fast_arc_tan(A.x - ship->pos.x, A.y - ship->pos.y);
        FUN_L00_00258278((char *)ship,
                         &d->heading_turn_velocity,
                         desired_heading,
                         0.02f,
                         0.3f,
                         D_0015ED6C * 12.566371f);
        aim_elevation =
            fast_arc_tan(vector_distance_xy(&ship->pos, &tgt->pos), tgt->pos.z - ship->pos.z);
        ship->pitch = spring_turn(&d->pitch_turn_velocity,
                                  ship->pitch,
                                  -aim_elevation,
                                  0.02f,
                                  0.3f,
                                  D_0015ED6C * 3.1415927f);
        fast_vec_sub(&B, &A, &ship->pos);
        fast_vec_normalize(&d->a120, &B, D_0015ED6C * 8.0f);
        fast_vec_add(&ship->pos, &ship->pos, &d->a120);
        if (fast_vec_length(&B) < 0.5f) {
            int n = d->path_point_index + 1;
            if (n < l10_paths[d->entry_path_index]->count) {
                d->path_point_index = n;
            } else {
                d->path_point_index = 0;
                ship->state = BLARG_SHIP_STATE_FIRE_AT_TARGET;
                d->firing_phase_timer =
                    truncate_float_to_s32(FUN_L04_001f1978(d->firing_duration_seconds * 60.0f));
            }
        }
        break;
    case BLARG_SHIP_STATE_FIRE_AT_TARGET:
        if (is_point_inside_clip_volume(&(&hero)->motion.pos, d->departure_volume_index)) {
            ship->state = BLARG_SHIP_STATE_FLY_AWAY;
            break;
        }
        *(u128_2E52B0 *)&A = *(u128_2E52B0 *)&tgt->pos;
        if ((&hero)->vehicle_mode != 1) {
            *(u128_2E52B0 *)&A = *(u128_2E52B0 *)&d->target_aim_position;
            collision_line(&ship->pos, &A, 0, ship, 0);
        } else {
            if ((Ship_5E48 *)(&hero)->moby == tgt) {
                fast_vec_scale(&C, &(&hero)->motion.displacement, (float)scale_game_frames(20));
                *(u128_2E52B0 *)&B = *(u128_2E52B0 *)&C;
                add_vectors(&C, &A, &B);
                A.z = probe_ground_height(&A, 0, 0.5f);
            }
            A.z += 0.2f;
            D_L10_001742D8 = tgt;
        }
        desired_heading = fast_arc_tan(tgt->pos.x - ship->pos.x, tgt->pos.y - ship->pos.y);
        FUN_L00_00258278((char *)ship,
                         &d->heading_turn_velocity,
                         desired_heading,
                         0.02f,
                         0.3f,
                         D_0015ED6C * 12.566371f);
        aim_elevation = fast_arc_tan(vector_distance_xy(&ship->pos, &A), A.z - ship->pos.z);
        ship->pitch = spring_turn(&d->pitch_turn_velocity,
                                  ship->pitch,
                                  -aim_elevation,
                                  0.02f,
                                  0.3f,
                                  D_0015ED6C * 3.1415927f);
        if (fast_difference_between_rotations(
                ship->heading, fast_arc_tan(tgt->pos.x - ship->pos.x, tgt->pos.y - ship->pos.y)) <
            0.17453292f) {
            if (ship->requested_animation_sequence != 3)
                blend_moby_animation(ship, 3, 0, scale_game_frames(6));
            ship->animation_speed = blarg_ship_firing_animation_speed_gp;
        }
        if (ship->b52 == 3 && d->targeting_status != BLARG_SHIP_TARGETING_FIRE_BLOCKED &&
            D_L10_001742D8 == tgt) {
            int near = is_value_within_interpolated_window((char *)ship, 1.0f);
            int far = is_value_within_interpolated_window((char *)ship, 16.0f);
            if (near || far) {
                FUN_L00_0024f7c8(ship, far == 0, &C);
                build_spherical_offset(&B, 1.0f, ship->heading, -ship->pitch);
                add_vectors(&D, &C, &B);
                fast_vec_sub(&B, &A, &C);
                fast_vec_normalize(&B, &B, blarg_ship_rocket_speed_gp * D_0015ED6C);
                fast_vec_scale(&E, &B, FUN_L04_001f1978(3.0f));
                add_vectors(&D, &E, &C);
                emit_radial_particle_burst(ship, &E, 0);
                spawn_blarg_ship_rocket(&B, &C, (int)ship, (int)tgt, scale_game_frames(300), 1.0f);
            }
        } else if (ship->b52 == 3 && ship->requested_animation_sequence != 2) {
            blend_moby_animation(ship, 2, 0, scale_game_frames(10));
        }
        if (fast_dec_timer(&d->firing_phase_timer)) {
            if (ship->requested_animation_sequence != 2)
                blend_moby_animation(ship, 2, 0, scale_game_frames(10));
            ship->state = BLARG_SHIP_STATE_FIRE_COOLDOWN;
            d->firing_phase_timer =
                truncate_float_to_s32(FUN_L04_001f1978(d->cooldown_duration_seconds * 60.0f));
        }
        qcopy(&B, &l10_paths[d->firing_path_index]->points[d->path_point_index]);
        fast_vec_sub(&C, &B, &ship->pos);
        FUN_L00_0025f730_c(&C, D_0015ED6C * 8.0f);
        fast_vec_add(&ship->pos, &ship->pos, &C);
        if (fast_vec_length(&C) < D_0015ED6C) {
            int n = d->path_point_index + 1;
            if (n < l10_paths[d->firing_path_index]->count)
                d->path_point_index = n;
        }
        break;
    case BLARG_SHIP_STATE_FIRE_COOLDOWN:
        if (is_point_inside_clip_volume(&(&hero)->motion.pos, d->departure_volume_index)) {
            ship->state = BLARG_SHIP_STATE_FLY_AWAY;
            break;
        }
        *(u128_2E52B0 *)&A = *(u128_2E52B0 *)&tgt->pos;
        if ((&hero)->vehicle_mode != 1) {
            *(u128_2E52B0 *)&A = *(u128_2E52B0 *)&d->target_aim_position;
            collision_line(&ship->pos, &A, 0, ship, 0);
        } else {
            if ((Ship_5E48 *)(&hero)->moby == tgt) {
                fast_vec_scale(&C, &(&hero)->motion.displacement, (float)scale_game_frames(20));
                *(u128_2E52B0 *)&B = *(u128_2E52B0 *)&C;
                add_vectors(&C, &A, &B);
                A.z = probe_ground_height(&A, 0, 0.5f);
            }
            A.z += 0.2f;
            D_L10_001742D8 = tgt;
        }
        desired_heading = fast_arc_tan(tgt->pos.x - ship->pos.x, tgt->pos.y - ship->pos.y);
        FUN_L00_00258278((char *)ship,
                         &d->heading_turn_velocity,
                         desired_heading,
                         0.02f,
                         0.3f,
                         D_0015ED6C * 12.566371f);
        aim_elevation = fast_arc_tan(vector_distance_xy(&ship->pos, &A), A.z - ship->pos.z);
        ship->pitch = spring_turn(&d->pitch_turn_velocity,
                                  ship->pitch,
                                  -aim_elevation,
                                  0.02f,
                                  0.3f,
                                  D_0015ED6C * 3.1415927f);
        if (fast_dec_timer(&d->firing_phase_timer)) {
            blend_moby_animation(ship, 3, 0, scale_game_frames(10));
            ship->animation_speed = blarg_ship_firing_animation_speed_gp;
            ship->state = BLARG_SHIP_STATE_FIRE_AT_TARGET;
            d->firing_phase_timer =
                truncate_float_to_s32(FUN_L04_001f1978(d->firing_duration_seconds * 60.0f));
        }
        qcopy(&B, &l10_paths[d->firing_path_index]->points[d->path_point_index]);
        fast_vec_sub(&C, &B, &ship->pos);
        FUN_L00_0025f730_c(&C, D_0015ED6C * 8.0f);
        fast_vec_add(&ship->pos, &ship->pos, &C);
        if (fast_vec_length(&C) < D_0015ED6C) {
            int n = d->path_point_index + 1;
            if (n < l10_paths[d->firing_path_index]->count)
                d->path_point_index = n;
        }
        break;
    case BLARG_SHIP_STATE_FLY_AWAY:
        ship->pos.z += D_0015ED6C * 10.0f;
        ship->scale *= frame_scale * -0.015299976f + 1.0f;
        if (250.0f < ship->pos.z)
            goto del;
        if (!(ship->scale < 0.01f))
            break;
        goto del;
    case BLARG_SHIP_STATE_HIT_REACTION: {
        unsigned char pre_hit_state;
        if (ship->b52 != 4)
            break;
        if (!(ship->b70 & 2))
            break;
        pre_hit_state = ship->bBC;
        if (pre_hit_state == BLARG_SHIP_STATE_FIRE_AT_TARGET) {
            blend_moby_animation(ship, 3, 0, scale_game_frames(10));
            ship->animation_speed = blarg_ship_firing_animation_speed_gp;
            ship->state = pre_hit_state;
            d->firing_phase_timer =
                truncate_float_to_s32(FUN_L04_001f1978(d->firing_duration_seconds * 60.0f));
        } else if (pre_hit_state == BLARG_SHIP_STATE_FLY_AWAY) {
            blend_moby_animation(ship, 2, 0, scale_game_frames(10));
            ship->state = pre_hit_state;
        } else {
            blend_moby_animation(ship, 2, 0, scale_game_frames(10));
            ship->state = BLARG_SHIP_STATE_FIRE_COOLDOWN;
            d->firing_phase_timer =
                truncate_float_to_s32(FUN_L04_001f1978(d->cooldown_duration_seconds * 60.0f));
        }
        break;
    }
    case BLARG_SHIP_STATE_EXPLODE: {
        float z = 0.0f;
        spawn_explosion(
            ship, d->a40, &ship->pos, z, z, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, 5, 15.0f, 1, 1, -1, 0);
        FUN_L00_00257470(ship, 0, -1);
        FUN_L00_00263fd8_c((char *)ship,
                           0x5FD,
                           (float *)&ship->pos,
                           (char *)ship + 0x40,
                           0,
                           0,
                           0.0f,
                           D_L10_0015F580,
                           D_L10_0015F580,
                           D_L10_0015F580);
        FUN_L00_00263fd8_c((char *)ship,
                           0x5FE,
                           (float *)&ship->pos,
                           (char *)ship + 0x40,
                           0,
                           0,
                           0.0f,
                           D_L10_0015F580,
                           D_L10_0015F580,
                           D_L10_0015F580);
        mark_moby_for_removal(ship);
        return;
    del:
        mark_moby_for_removal(ship);
        return;
    }
    }
    if (ship->b31 != 0) {
        if (fast_vec_dist(&ship->pos, D_L10_00167240) < 28.0f) {
            FUN_L00_0025a120(ship);
            ship->b7F = 0x16;
        }
    }
}

/* Hit handling and path following for a level 10 vendor moby. */
extern void select_hero_or_gadgebot_target(float, void *, void *) __asm__("FUN_L06_002f34d8");
extern int  point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int  follow_5968(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");

void update_blarg_ship_hit_reactions_and_targeting(struct Moby *moby) __asm__("FUN_L10_002e5968");

typedef struct BlargShipVars {
    u8                   pad_0[0x20];
    f32                  health;
    u8                   pad_24[0x2];
    s16                  stun_timer;
    u8                   pad_28[0x98];
    struct HitColorFlash hit_flash;
    u8                   path_search_point;
    u8                   pad_D1[0x3F];
    s32                  target;
    s32                  target_selection_status;
    u8                   pad_118[0x78];
    s32                  path_190;
} BlargShipVars;

void update_blarg_ship_hit_reactions_and_targeting(struct Moby *moby) {
    enum {
        LETHAL_HIT_RESPONSE = 1,
        COLOR_UPDATE_ONLY_HIT_RESPONSE = 9,
        COLOR_UPDATE_WITHOUT_STATE_CHANGE_HIT_RESPONSE = 10,
        COLOR_EFFECT_ONLY_HIT_RESPONSE = 12,
        NO_REACTION_HIT_RESPONSE = 13
    };
    BlargShipVars *pvars = (BlargShipVars *)moby->pvars;
    char          *ship_hit_record = find_moby_hit(moby, 0x370000, 0);
    int            ship_hit_processing_status;
    float          dmg;
    int            hit_response_code;
    dmg = 0.0f;
    hit_response_code = resolve_moby_hit_response(
        moby, ship_hit_record, &pvars->health, 5, &ship_hit_processing_status, &dmg, 0, 4);
    if (ship_hit_record != 0 && ship_hit_processing_status != HIT_STATUS_SKIP_DAMAGE_AND_REACTION &&
        moby->state != BLARG_SHIP_STATE_EXPLODE) {
        if ((pvars->health -= dmg) <= 0.0f) {
            hit_response_code = 1;
        }
        if (*(int *)(ship_hit_record + 0x24) & 0x40000) {
            hit_response_code = 1;
        }
        switch (hit_response_code) {
        case COLOR_UPDATE_ONLY_HIT_RESPONSE:
        case COLOR_UPDATE_WITHOUT_STATE_CHANGE_HIT_RESPONSE:
        case COLOR_EFFECT_ONLY_HIT_RESPONSE:
            pvars->hit_flash.flash_red = 0xFA;
            start_hit_color_flash(moby, &pvars->hit_flash);
            break;
        case LETHAL_HIT_RESPONSE:
            pvars->hit_flash.flash_red = 0xFA;
            start_hit_color_flash(moby, &pvars->hit_flash);
            moby->state = BLARG_SHIP_STATE_EXPLODE;
            break;
        case NO_REACTION_HIT_RESPONSE:
            break;
        default:
            pvars->hit_flash.flash_red = 0xFA;
            pvars->stun_timer = scale_game_frames(0x3C);
            start_hit_color_flash(moby, &pvars->hit_flash);
            blend_moby_animation(moby, 4, 0, scale_game_frames(6));
            moby->command = moby->state;
            moby->state = BLARG_SHIP_STATE_HIT_REACTION;
            break;
        }
    }
    moby->hit_slot = 0xFF;
    update_hit_color_flash(moby, &pvars->hit_flash);
    if (hero.vehicle_mode == HERO_VEHICLE_MODE_CLANK) {
        select_hero_or_gadgebot_target(101.0f, moby, &pvars->target);
        if (*(char **)&pvars->target == 0) {
            pvars->target_selection_status = 2;
        } else {
            char *path = ((char *)l10_paths[pvars->path_190]);
            if (point_in_path_xy(*(char **)&pvars->target + 0x10, path + 0x10, *(int *)path) == 0) {
                pvars->target = 0;
                pvars->target_selection_status = 2;
            } else {
                pvars->target_selection_status = 0;
            }
        }
    } else {
        char *path = ((char *)l10_paths[pvars->path_190]);
        if (follow_5968(101.0f, moby, &pvars->path_search_point, 0, 0, path + 0x10, *(int *)path) !=
                2 &&
            moby->visible == 0) {
            pvars->target_selection_status = 2;
        }
    }
    if (pvars->target == 0) {
        pvars->target = (int)hero.moby;
    }
}

int D_L10_00161EE4 = 0x5a;
