/* Level 03 moby classes: metropolis_train_locomotive, moving_platform_small, metropolis_train_carriage, red_ship_1, robot_dog. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/mobys/moby_class_id.h"
#include "rnc/overlay/collision.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/math/vectors/compute_transformed_delta_component.h"
#include "rnc/rendering/effects/fade_to_black.h"

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_0029E370), where it is exact; names translated to the US level program. */

extern char D_L03_00166EC0[];

void update_emitter_intermittent_sound(struct Moby *moby) __asm__("FUN_L03_0029d1b8");

void update_emitter_intermittent_sound(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int   unused_local;
    float distance_to_reference;
    switch (moby->oclass) {
    case 0x33:
    case 0x40:
    case MOBY_CLASS_ALTERNATE_COLOR_CHILD_EFFECT_EMITTER:
    case MOBY_CLASS_TEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_TWO_JOINT_PARTICLE_EMITTER:
    case MOBY_CLASS_FIFTEEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_DEFAULT_COLOR_CHILD_EFFECT_EMITTER:
        if (*(short *)(data + 0x148) == -1) {
            int lo = *(short *)(data + 0x14C);
            *(short *)(data + 0x148) =
                scale_game_frames(random_int_between(lo, lo + *(short *)(data + 0x14E)));
        } else {
            int x, y;
            if (!fast_dec_timer_s16(data + 0x148)) {
                break;
            }
            distance_to_reference = fast_vec_dist(&moby->pos, D_L03_00166EC0);
            if (distance_to_reference > 15.0f && distance_to_reference < 90.0f) {
                allocate_voice_for_target_entry(0, 0, (int)moby);
            }
            x = scale_game_frames(0x898);
            y = scale_game_frames(0x898);
            *(short *)(data + 0x148) =
                scale_game_frames(random_int_between(x, y + scale_game_frames(0x3E8)));
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C6F40), where it is exact; names translated to the US level program. */

int is_moby_class_23d_in_state_5(struct Moby *moby) __asm__("FUN_L03_002c5b78");

int is_moby_class_23d_in_state_5(struct Moby *moby) {
    if (moby && moby->state != 0xFE && moby->state != 0xFD && moby->oclass == 0x23D &&
        moby->state == 5) {
        return 1;
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C8068), where it is exact; names translated to the US level program. */

int can_release_linked_robot_dog(struct Moby *moby) __asm__("FUN_L03_002c6ca0");

int can_release_linked_robot_dog(struct Moby *moby) {
    unsigned char state;
    char         *data;
    data = (char *)moby->pvars;
    if (!moby)
        return 0;
    state = moby->state;
    if (state == 0xFE)
        return 0;
    if (state == 0xFD)
        return 0;
    if (moby->oclass != 0x23E)
        return 0;
    if (moby->seq == 10 && compute_interpolated_record_value(moby) >= 10.0f)
        return 1;
    if (moby->state == 10 || moby->state == 13 || *(int *)(data + 0x284) == -1)
        return 1;
    return 0;
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_path.h"

/* Places a train: puts the engine at `at` facing ang (offset back by its length), then lays each linked
 * car behind the previous one along ang, updating their motion deltas, and enables the coupling moby. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_00293720), where it is exact; names translated to the US level program. */

extern int   D_L03_0015FFD8; /* no foreign declaration */
extern short metropolis_train_carriage_spacing __asm__("D_L03_00161368") __attribute__((sda));
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");
extern void  update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void  advance_moby_animation(void *) __asm__("FUN_0020d580");

void place_metropolis_train(struct Moby *moby,
                            float       *pos,
                            float        heading) __asm__("FUN_L03_00292578");

void place_metropolis_train(struct Moby *moby, float *pos, float heading) {
    float start[4];
    float offset[4];
    float delta[4];
    float old_pos[4];
    float old_rot[4];
    float car_pos[4];
    float car_off[4];
    char *data;
    char *coupling;
    int   next_id;
    *(OvlQuad *)start = *(OvlQuad *)pos;
    data = (char *)moby->pvars;
    coupling = D_L03_0015FFD8 + (*(int *)(data + 0xC4) << 8);
    next_id = *(int *)(data + 0xA0);
    *(OvlQuad *)old_pos = *(OvlQuad *)&moby->pos;
    *(OvlQuad *)old_rot = *(OvlQuad *)&moby->rot;
    qcopy(coupling + 0x10, start);
    offset[0] = fast_cos(heading) * -*(float *)(data + 0xDC);
    offset[1] = fast_sin(heading) * -*(float *)(data + 0xDC);
    offset[2] = 0.0f;
    fast_vec_add(&moby->pos, start, offset);
    moby->rot.z = fast_add_rotations(heading, 3.1415927f);
    moby->rot.y = 0.0f;
    fast_vec_sub(delta, &moby->pos, old_pos);
    update_entity_matrix(data + 0x60, delta, old_rot, &moby->rot);
    *(int *)(data + 0xCC) = 0;
    *(int *)(data + 0xD0) = 0;
    offset[0] = fast_cos(heading) * -*(float *)(data + 0xBC);
    offset[1] = fast_sin(heading) * -*(float *)(data + 0xBC);
    offset[2] = 0.0f;
    fast_vec_add(offset, offset, &moby->pos);
    while (next_id != -1) {
        char *car = D_L03_0015FFD8 + (next_id << 8);
        char *car_data = *(char **)(car + 0x78);
        qcopy(car_data + 0xA0, car + 0x10);
        qcopy(car_data + 0xB0, car + 0x40);
        car_pos[0] = fast_cos(heading) * -*(float *)&metropolis_train_carriage_spacing;
        car_pos[1] = fast_sin(heading) * -*(float *)&metropolis_train_carriage_spacing;
        car_pos[2] = 0.0f;
        fast_vec_add(car_pos, car_pos, offset);
        car_off[0] = fast_cos(heading) * -*(float *)(car_data + 0xC4);
        car_off[1] = fast_sin(heading) * -*(float *)(car_data + 0xC4);
        car_off[2] = 0.0f;
        fast_vec_add(car + 0x10, car_off, car_pos);
        *(float *)(car + 0x18) = moby->pos.z;
        *(float *)(car + 0x48) = fast_add_rotations(
            fast_arc_tan(car_pos[0] - *(float *)(car + 0x10), car_pos[1] - *(float *)(car + 0x14)),
            0.0f);
        *(float *)(car + 0x44) = 0.0f;
        advance_moby_animation(car);
        update_entity(car);
        *(unsigned short *)(car + 0x34) |= 6;
        moby_point_position(car, 1, offset);
        fast_vec_sub(delta, car + 0x10, car_data + 0xA0);
        update_entity_matrix(car_data + 0x20, delta, car_data + 0xB0, car + 0x40);
        qcopy(car_data + 0xA0, car + 0x10);
        qcopy(car_data + 0xB0, car + 0x40);
        next_id = *(int *)(car_data + 0xC0);
    }
    coupling[0x20] = 0;
    *(unsigned short *)(coupling + 0x34) |= 6;
}

extern char *D_L03_0015FFD8_292890 __asm__("D_L03_0015FFD8");
extern char  D_L03_001E2410[];
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern void debug_print(void *, int) __asm__("FUN_001e93b0");
extern void gunship_follow_path(char *) __asm__("FUN_L01_002f5168");

typedef struct {
    char  p00[0x60];
    char  motion[0x40];
    int   iA0;
    char  pA4[0x14];
    int   iB8;
    char  pBC[4];
    float fC0;
    int   iC4;
    float fC8;
    float fCC;
    float fD0;
    char  pD4[8];
    float fDC;
} Train292890;

typedef struct {
    char  p00[0x20];
    char  motion[0x80];
    float prev_pos[4];
    float prev_rot[4];
    int   iC0;
    float fC4;
} Car292890;

void update_metropolis_train_motion(char *moby) __asm__("FUN_L03_00292890");

void update_metropolis_train_motion(char *moby) {
    float        old_pos[4];
    float        old_rot[4];
    float        delta[4];
    float        front[4];
    float        hitch[4];
    float        car_position_adjustment[4];
    float        rear[4];
    char        *coupling;
    Train292890 *data;
    char        *coupling_data;
    char        *car;
    Car292890   *car_data;
    char        *target;
    int          next_id;
    int          id;
    float        distance_margin_or_target_angle;

    data = ((Train292890 *)MOBY(moby)->pvars);
    coupling = D_L03_0015FFD8_292890 + (data->iC4 << 8);
    coupling_data = *(char **)(coupling + 0x78);
    ((unsigned char *)moby)[0x72] = 0xFF;
    qcopy(old_pos, moby + 0x10);
    qcopy(old_rot, moby + 0x40);
    next_id = data->iA0;
    if (next_id != -1) {
        /* Fakematch: writing the unchanged next car id back is a no-op, but the
         * test-and-store survives until after register allocation (reload's cse
         * drops the redundant store, then the empty branch is deleted). Until
         * then the branch's label stops reload from reusing the $4 copy of
         * `coupling` below, which gives retail's reload of it from the stack. */
        if (next_id != 0) {
            data->iA0 = next_id;
        }
        id = next_id;
        target = coupling + 0x10;
        distance_margin_or_target_angle = data->fDC - vector_distance_xy(moby + 0x10, target);
        *(float *)(coupling_data + 0xFC) = distance_margin_or_target_angle;
        if (distance_margin_or_target_angle < 0.0f) {
            *(float *)(coupling_data + 0xFC) = 0.0f;
        }
        gunship_follow_path(coupling);
        build_spherical_offset(
            delta, data->fC8, fast_add_rotations(MOBY(moby)->rot.z, 3.1415927f), MOBY(moby)->rot.y);
        fast_vec_normalize(delta, delta, data->fC8);
        fast_vec_add(moby + 0x10, moby + 0x10, delta);
        distance_margin_or_target_angle =
            fast_add_rotations(fast_arc_tan(*(float *)(coupling + 0x10) - MOBY(moby)->pos.x,
                                            *(float *)(coupling + 0x14) - MOBY(moby)->pos.y),
                               3.1415927f);
        MOBY(moby)->rot.z =
            spring_turn(&data->fCC,
                        MOBY(moby)->rot.z,
                        distance_margin_or_target_angle,
                        (data->fC8 * 60.0f / data->fC0) * 0.25f * DEG_TO_RAD * frame_time_sq,
                        (data->fC8 * 60.0f / data->fC0) * 0.5f * DEG_TO_RAD * frame_time_sq,
                        (data->fC8 * 60.0f / data->fC0) * frame_time);
        distance_margin_or_target_angle =
            fast_arc_tan(vector_distance_xy(moby + 0x10, target),
                         *(float *)(coupling + 0x18) - MOBY(moby)->pos.z);
        MOBY(moby)->rot.y =
            spring_turn(&data->fD0,
                        MOBY(moby)->rot.y,
                        distance_margin_or_target_angle,
                        (data->fC8 * 60.0f / data->fC0) * 0.25f * DEG_TO_RAD * frame_time_sq,
                        (data->fC8 * 60.0f / data->fC0) * 0.5f * DEG_TO_RAD * frame_time_sq,
                        (data->fC8 * 60.0f / data->fC0) * frame_time);
        moby_point_position(moby, 0, front);
        do {
            car = D_L03_0015FFD8_292890 + (id << 8);
            ((unsigned char *)car)[0x72] = 0xFF;
            car_data = *(Car292890 **)(car + 0x78);
            moby_point_position(car, 0, rear);
            fast_vec_sub(hitch, rear, front);
            set_vector_length_xy(hitch, hitch, *(float *)&metropolis_train_carriage_spacing);
            fast_vec_add(hitch, hitch, front);
            hitch[2] = front[2];
            fast_vec_sub(car_position_adjustment, hitch, car + 0x10);
            fast_vec_normalize(car_position_adjustment,
                               car_position_adjustment,
                               fast_vec_length(car_position_adjustment) - car_data->fC4);
            fast_vec_add(car + 0x10, car_position_adjustment, car + 0x10);
            *(float *)(car + 0x48) = fast_add_rotations(
                fast_arc_tan(hitch[0] - *(float *)(car + 0x10), hitch[1] - *(float *)(car + 0x14)),
                0.0f);
            *(float *)(car + 0x44) = -fast_arc_tan(vector_distance_xy(car + 0x10, front),
                                                   front[2] - *(float *)(car + 0x18));
            advance_moby_animation(car);
            update_entity(car);
            *(unsigned short *)(car + 0x34) |= 6;
            moby_point_position(car, 1, front);
            fast_vec_sub(delta, car + 0x10, car_data->prev_pos);
            update_entity_matrix(car_data->motion, delta, car_data->prev_rot, car + 0x40);
            qcopy(car_data->prev_pos, car + 0x10);
            qcopy(car_data->prev_rot, car + 0x40);
            id = car_data->iC0;
        } while (id != -1);
        fast_vec_sub(delta, moby + 0x10, old_pos);
        update_entity_matrix(data->motion, delta, old_rot, moby + 0x40);
    } else {
        debug_print(D_L03_001E2410, data->iB8);
        mark_moby_for_removal(moby);
        return;
    }
    *(unsigned short *)(coupling + 0x34) |= 6;
}

extern char *D_L03_0015FFD8_292d10 __asm__("D_L03_0015FFD8");

/* Drags each linked car to follow the train after it moved, then flags the coupling moby; with no cars it reports and deletes the train. */
void update_metropolis_train_idle_motion(struct Moby *moby) __asm__("FUN_L03_00292d10");

void update_metropolis_train_idle_motion(struct Moby *moby) {
    float  old_pos[4];
    float  old_rot[4];
    float  delta[4];
    char  *data;
    char  *car;
    char  *car_data;
    char  *moby_position;
    char  *moby_rotation;
    int    next_id;
    int    id;
    float *dp;
    char  *motion;

    data = (char *)moby->pvars;
    moby->unk72 = 0x40;
    moby_position = &moby->pos;
    qcopy(old_pos, moby_position);
    moby_rotation = &moby->rot;
    qcopy(old_rot, moby_rotation);
    next_id = *(int *)(data + 0xA0);
    if (next_id != -1) {
        id = next_id;
        dp = delta;
        motion = data + 0x60;
        do {
            car = D_L03_0015FFD8_292d10 + (id << 8);
            car_data = *(char **)(car + 0x78);
            car[0x72] = 0x40;
            fast_vec_sub(delta, car + 0x10, car_data + 0xA0);
            update_entity_matrix(car_data + 0x20, delta, car_data + 0xB0, car + 0x40);
            qcopy(car_data + 0xA0, car + 0x10);
            qcopy(car_data + 0xB0, car + 0x40);
            id = *(int *)(car_data + 0xC0);
        } while (id != -1);
        fast_vec_sub(dp, &moby->pos, old_pos);
        update_entity_matrix(motion, dp, old_rot, &moby->rot);
    } else {
        debug_print(D_L03_001E2410, *(int *)(data + 0xB8));
        mark_moby_for_removal(moby);
        return;
    }
    *(unsigned short *)(D_L03_0015FFD8_292d10 + (*(int *)(data + 0xC4) << 8) + 0x34) |= 6;
}

#include "rnc/mobys/moby.h"

typedef struct {
    char  p00[0x20];
    int   i20;
    short s24;
    char  p26[2];
    u8    b28;
    char  p29[0x15];
    short s3E;
    char  p40[0x5C];
    int   i9C;
    char  pA0[4];
    int   iA4;
    int   iA8;
    int   iAC;
    int   iB0;
    int   iB4;
    int   iB8;
    float fBC;
    float fC0;
    int   iC4;
    float fC8;
    char  pCC[8];
    int   iD4;
    int   iD8;
    float fDC;
    int   ids[8];
    int   i100;
    int   i104;
    int   i108;
    int   i10C;
} Engine292e98;

typedef struct {
    char  p00[0x60];
    int   i60;
    char  p64[0xC];
    char *path;
    int   i74;
    char  p78[0x78];
    float fF0;
    char  pF4[8];
    float fFC;
} Coupling292e98;

typedef struct {
    char  p00[0x14];
    float f14;
    float f18;
    char  p1C[0xC];
    int   i28;
    int   i2C;
} Turret292e98;

extern char        *D_L03_0015FFD8_292e98 __asm__("D_L03_0015FFD8");
extern struct Moby *D_L03_0015FFD8_m292 __asm__("D_L03_0015FFD8");

#include "rnc/overlay/cuboid.h"

extern int  D_L03_0015FDFC;
extern char train_missing_master_warning_format[] __asm__("D_L03_001E2448");
extern char train_end_path_missing_stop_node_warning[] __asm__("D_L03_001E2478");
extern int  FUN_L00_0025df68(void *, void *, void *, int *, float *, int, float, float, float);
extern int  is_hero_on_moby(char *) __asm__("FUN_L01_00277fb8");
extern int  is_moby_class_23e_in_state_14(char *) __asm__("FUN_L03_002c6c60");
extern void set_moby_dialogue_record_value(void *, int) __asm__("FUN_L00_00266858");
extern void jet_game_state(int, int) __asm__("FUN_L01_0027a248");
extern void set_moby_state_14(char *) __asm__("FUN_L03_002c6c90");
extern void debug_printf_292e98(char *, ...) __asm__("FUN_001e93b0");

/* Heading of a path's first segment. */
#define PATH_HEADING(p)                                           \
    fast_arc_tan(*(float *)((p) + 0x20) - *(float *)((p) + 0x10), \
                 *(float *)((p) + 0x24) - *(float *)((p) + 0x14))

/* Points a turret moby's data (yaw 0x18, pitch 0x14) at the hero, seen from the level camera. */
#define AIM_AT_HERO(t)                                                                    \
    {                                                                                     \
        (t)->f18 = fast_arc_tan(hero.motion.pos.f[0] - l03_camera.pos.f[0],               \
                                hero.motion.pos.f[1] - l03_camera.pos.f[1]);              \
        (t)->f14 = -fast_arc_tan(vector_distance_xy(l03_camera.pos.f, hero.motion.pos.f), \
                                 hero.motion.pos.f[2] - l03_camera.pos.f[2]);             \
    }

/* Keeps the engine loop sound playing and restarts the horn at random intervals. */
#define ENGINE_SOUND()                                                                       \
    {                                                                                        \
        if (!voice_is_playing(moby, data->i108)) {                                           \
            data->i108 = allocate_voice_for_target_entry(1, 4, (int)moby);                   \
        }                                                                                    \
        if (fast_dec_timer(&data->i104)) {                                                   \
            allocate_voice_for_target_entry(0, 0, (int)moby);                                \
            data->i104 = random_int_between(scale_game_frames(900), scale_game_frames(400)); \
        }                                                                                    \
    }

#define RELEASE_VOICE(v)                                                   \
    {                                                                      \
        int idx_ = (v);                                                    \
        if (idx_ != -1) {                                                  \
            unsigned char *e_ = (unsigned char *)voice_pool + idx_ * 0x70; \
            if (*(char **)(e_ + 0x88) == moby && e_[0x74]) {               \
                release_voice_slot(idx_);                                  \
            }                                                              \
        }                                                                  \
    }

extern char voice_pool[] __asm__("D_0013E550");

/* Kerwan train engine: waits for the hero to board (clip volume 0xB4), then runs the ride: accelerates
 * along its path pulling the cars, keeps the turret cars aimed at the hero, throws him off if he falls
 * behind, switches to the second path when the cars are done and brakes at its end (state 6 resets). */
void metropolis_train_locomotive_update(char *moby) __asm__("FUN_L03_00292e98");

void metropolis_train_locomotive_update(char *moby) {
    enum {
        TRAIN_INITIALIZE = 0,
        TRAIN_SETUP_COUPLING_PATH = 1,
        TRAIN_POSITION_FOR_BOARDING = 2,
        TRAIN_WAIT_FOR_BOARDING = 3,
        TRAIN_ACCELERATE_AND_FOLLOW_FIRST_PATH = 4,
        TRAIN_FOLLOW_SECOND_PATH_AND_BRAKE = 5,
        TRAIN_WAIT_FOR_RESET_VOLUME = 6
    };
    float           start[4];
    int             seg;
    float           frac;
    char           *coupling;
    Engine292e98   *data;
    Coupling292e98 *cdata;
    Turret292e98   *td;
    Turret292e98   *tdb;
    char           *car;
    char           *path;
    int             i;
    int             i6;
    float           braking_speed_floor;
    struct Moby    *linked_moby;
    int             second_path_node_index;
    char           *second_path;
    u32             mode;
    int             ident;

    data = ((Engine292e98 *)MOBY(moby)->pvars);
    ident = *(unsigned short *)(moby + 0xA8);
    data->iB8 = ident;
    if (data->iA4 == -1) {
        data->i20 = 0;
        data->s24 = 0;
        data->b28 = 4;
        data->s3E = 0xD;
        return;
    }
    if (data->iC4 == -1) {
        debug_printf_292e98(train_missing_master_warning_format, ident);
        mark_moby_for_removal(moby);
    }
    coupling = D_L03_0015FFD8_292e98 + (data->iC4 << 8);
    cdata = *(Coupling292e98 **)(coupling + 0x78);
    D_L03_0015FDFC = 0;
    switch (((unsigned char *)moby)[0xBC]) {
    case TRAIN_INITIALIZE:
        data->i20 = 0;
        data->s24 = 0;
        data->b28 = 4;
        data->s3E = 0xD;
        moby_point_position(moby, 0, start);
        data->fBC = vector_distance_xy(start, moby + 0x10);
        data->fDC = 20.0f;
        data->fC8 = data->fC0 = data->fC0 * frame_time;
        if (data->iAC != -1) {
            second_path = ((char *)l03_paths[data->iAC]);
            data->iD4 = -1;
            second_path_node_index = 0;
            if (*(int *)second_path > 0) {
                if (*(float *)(second_path + second_path_node_index * 0x10 + 0x1C) == 1.0f) {
                    data->iD4 = second_path_node_index;
                } else {
                next:
                    second_path_node_index++;
                    if (second_path_node_index < *(int *)second_path) {
                        if (*(float *)((second_path_node_index << 4) + second_path + 0x1C) !=
                            1.0f) {
                            goto next;
                        }
                        data->iD4 = second_path_node_index;
                    }
                }
            }
        }
        moby[0xBC] = 1;
        data->i9C = 3;
        *(unsigned short *)(coupling + 0x34) |= 6;
        break;
    case TRAIN_SETUP_COUPLING_PATH:
        *(OvlQuad *)start = *(OvlQuad *)(((char *)l03_paths[data->iA4]) + 0x10);
        place_metropolis_train(moby, start, 0.0f);
        cdata->i74 = data->iA4;
        cdata->fFC = data->fC0;
        gunship_follow_path(coupling);
        moby[0xBC] = 2;
        break;
    case TRAIN_POSITION_FOR_BOARDING:
        *(OvlQuad *)start = *(OvlQuad *)(((char *)l03_paths[data->iA8]) + 0x10);
        place_metropolis_train(moby, start, PATH_HEADING(((char *)l03_paths[data->iA8])));
        moby[0xBC] = 3;
        cdata->i74 = data->iA4;
        cdata->path = ((char *)l03_paths[cdata->i74]);
        cdata->i60 = 1;
        data->fC8 = 0.0f;
        break;
    case TRAIN_WAIT_FOR_BOARDING:
        update_metropolis_train_idle_motion(moby);
        if (!is_point_inside_clip_volume(&hero.motion, data->iB4)) {
            break;
        }
        if (data->i100 == -1) {
            break;
        }
        mode = hero.state.control_mode;
        if (mode < 2 || mode == 9 || mode == 12) {
            moby[0xBC] = 4;
            D_L03_0015FDFC = 1;
            jet_game_state(4, 7);
            set_moby_dialogue_record_value(moby, 1);
            ((char *)&D_L03_0015FFD8_m292[data->i100])[0xBC] = 1;
            td = *(Turret292e98 **)((char *)&D_L03_0015FFD8_m292[data->i100] + 0x78);
            AIM_AT_HERO(td);
            td->i2C = scale_game_frames(860);
            data->i104 = scale_game_frames(30);
            data->i108 = -1;
        }
        break;
    case TRAIN_ACCELERATE_AND_FOLLOW_FIRST_PATH:
        data->fC8 += data->fC0 / 5.0f * frame_time;
        if (data->fC0 < data->fC8) {
            data->fC8 = data->fC0;
        }
        D_L03_0015FDFC = 1;
        update_metropolis_train_motion(moby);
        ENGINE_SOUND();
        if (hero.air_frames.s != 0 && hero.state.control_mode != 3) {
            FUN_L00_0025df68(
                cdata->path, hero.motion.pos.f, start, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
            data->i10C += 1;
            if (scale_game_frames(60) < data->i10C && hero.motion.pos.f[2] < start[2] - 10.0f) {
                RELEASE_VOICE(data->i108);
                data->i108 = -1;
                fade_to_black(scale_game_frames(16));
                hero.fall_fade_pending = 1;
                break;
            }
        } else {
            data->i10C = 0;
        }
        if (((unsigned char *)(char *)&D_L03_0015FFD8_m292[data->i100])[0xBC] != 0) {
            td = *(Turret292e98 **)((char *)&D_L03_0015FFD8_m292[data->i100] + 0x78);
            AIM_AT_HERO(td);
        }
        if (hero.ground_moby != 0 && (*(short *)((char *)hero.ground_moby + 0xA6) == 0x336 ||
                                      *(short *)((char *)hero.ground_moby + 0xA6) == 0x4BA)) {
            set_moby_dialogue_record_value(hero.ground_moby, 3);
        }
        if (!is_hero_on_moby(moby)) {
            break;
        }
        if (data->iAC == -1) {
            break;
        }
        if (data->iD8 == -1) {
            break;
        }
        if (((unsigned char *)(char *)&D_L03_0015FFD8_m292[data->i100])[0xBC] != 0) {
            break;
        }
        for (i = 0; i < 8; i++) {
            car = D_L03_0015FFD8_292e98 + (data->ids[i] << 8);
            if (data->ids[i] != -1 && car != 0 && ((unsigned char *)car)[0x20] != 0xFE &&
                ((unsigned char *)car)[0x20] != 0xFD && !is_moby_class_23e_in_state_14(car)) {
                set_moby_state_14(car);
                break;
            }
        }
        if (data->iD4 == -1) {
            debug_printf_292e98(train_end_path_missing_stop_node_warning);
            break;
        }
        ((char *)&D_L03_0015FFD8_m292[data->iD8])[0xBC] = 1;
        tdb = *(Turret292e98 **)((char *)&D_L03_0015FFD8_m292[data->iD8] + 0x78);
        path = ((char *)l03_paths[data->iAC]);
        tdb->f18 = fast_arc_tan(*(float *)(path + 0x10) - l03_camera.pos.f[0],
                                *(float *)(path + 0x14) - l03_camera.pos.f[1]);
        tdb->f14 = -fast_arc_tan(
            vector_distance_xy(l03_camera.pos.f, ((char *)l03_paths[data->iAC]) + 0x10),
            *(float *)(((char *)l03_paths[data->iAC]) + 0x18) - l03_camera.pos.f[2]);
        l03_cuboids[tdb->i28].pos.x =
            fast_cos(PATH_HEADING(((char *)l03_paths[data->iAC]))) * -(data->fDC + 4.0f);
        l03_cuboids[tdb->i28].pos.y =
            fast_sin(PATH_HEADING(((char *)l03_paths[data->iAC]))) * -(data->fDC + 4.0f);
        l03_cuboids[tdb->i28].pos.z = 0.0f;
        fast_vec_add(&l03_cuboids[tdb->i28].pos,
                     &l03_cuboids[tdb->i28].pos,
                     ((char *)l03_paths[data->iAC]) + 0x10);
        l03_cuboids[tdb->i28].pos.z += 2.7f;
        clear_u64_value(&l03_cuboids[tdb->i28].rot);
        l03_cuboids[tdb->i28].rot.z = PATH_HEADING(((char *)l03_paths[data->iAC]));
        tdb->i2C = scale_game_frames(10000);
        *(OvlQuad *)start = *(OvlQuad *)(((char *)l03_paths[data->iAC]) + 0x10);
        place_metropolis_train(moby, start, PATH_HEADING(((char *)l03_paths[data->iAC])));
        cdata->i74 = data->iAC;
        cdata->i60 = 1;
        cdata->fF0 = 1.0f;
        data->fC0 = 2.0f * data->fC0;
        cdata->fFC = data->fC0;
        gunship_follow_path(coupling);
        moby[0xBC] = 5;
        jet_game_state(0, 8);
        break;
    case TRAIN_FOLLOW_SECOND_PATH_AND_BRAKE:
        td = *(Turret292e98 **)((char *)&D_L03_0015FFD8_m292[data->iD8] + 0x78);
        update_metropolis_train_motion(moby);
        D_L03_0015FDFC = 1;
        ENGINE_SOUND();
        if (cdata->i60 >= data->iD4) {
            data->fDC = 0.0f;
            braking_speed_floor = data->fDC;
            data->fC8 -= data->fC0 / 1.9f * frame_time;
            if (data->fC8 < braking_speed_floor) {
                data->fC8 = braking_speed_floor;
            }
            if (data->fC8 == braking_speed_floor) {
                ((char *)&D_L03_0015FFD8_m292[data->iD8])[0xBC] = 0;
                moby[0xBC] = 6;
                data->fC0 *= 0.5f;
                RELEASE_VOICE(data->i108);
                data->i108 = -1;
            }
        }
        AIM_AT_HERO(td);
        break;
    case TRAIN_WAIT_FOR_RESET_VOLUME:
        if (is_point_inside_clip_volume(&hero.motion, data->iB0)) {
            for (i6 = 0; i6 < 8; i6++) {
                linked_moby = D_L03_0015FFD8_292e98 + (data->ids[i6] << 8);
                if (data->ids[i6] != -1 && linked_moby != 0 &&
                    ((unsigned char *)linked_moby)[0x20] != 0xFE &&
                    ((unsigned char *)linked_moby)[0x20] != 0xFD && linked_moby->oclass == 0x23E &&
                    ((unsigned char *)linked_moby)[0x20] == 0x10) {
                    linked_moby->state = 0x11;
                }
            }
            data->fC8 = data->fC0;
            data->fDC = 20.0f;
            moby[0xBC] = 1;
        } else {
            update_metropolis_train_idle_motion(moby);
        }
        break;
    case 7:
        break;
    }
}

extern char  D_0013E550_294c08[] __asm__("D_0013E550");
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern int   trooper_play_sound(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern void  noop_main_callback(void *) __asm__("FUN_001e93e8");
extern void  FUN_L00_0025f730(void *, float);

/* Path platform update: waits, then glides along its path one way and back (ease in/out over 240 frames),
 * or steps node to node towards the path end, pausing between runs and playing its loop sound. */
void moving_platform_small_update(char *moby) __asm__("FUN_L03_00294c08");

void moving_platform_small_update(char *moby) {
    enum {
        PLATFORM_INITIALIZE_AND_WAIT = 0,
        PLATFORM_EASED_ENDPOINT_TRAVEL = 1,
        PLATFORM_PAUSE_BEFORE_EASED_TRAVEL = 2,
        PLATFORM_FOLLOW_PATH_NODES = 3,
        PLATFORM_PAUSE_BEFORE_NODE_TRAVEL = 4
    };
    float          delta[4];
    float          old_rot[4];
    float          step[4];
    float          endpoint_delta_or_position[4];
    char          *data;
    char          *pa;
    char          *pb;
    char          *pc;
    char          *initial_path_position;
    int           *path;
    char          *platform_position;
    char          *vel;
    char          *target;
    int            voice_slot_index;
    int            easing_frame_count;
    int            next;
    int            end;
    int            count;
    int            next_path_node_byte_offset;
    int            node_off;
    signed char    path_node_step;
    float          speed;
    float          base;
    float          d;
    unsigned char *voice_slot_entry;

    data = ((char *)MOBY(moby)->pvars);
    fast_vec_scale(delta, moby + 0x10, -1.0f);
    qcopy(old_rot, moby + 0x40);
    noop_main_callback(moby);
    switch (((unsigned char *)moby)[0x20]) {
    case PLATFORM_INITIALIZE_AND_WAIT:
        pa = data + 0xB0;
        *(char **)(data + 0xC0) = ((char *)l03_paths[*(int *)(data + 0xE0)]);
        if (*(int *)(data + 0xF8) != 0) {
            pa[4] = -1;
        } else {
            pa[4] = 1;
        }
        initial_path_position = *(char **)(data + 0xC0) + 0x10;
        if (pa[4] >= 0) {
            initial_path_position += (**(int **)(pa + 0x10) - 1) * 0x10;
        }
        qcopy_nc(moby + 0x10, initial_path_position);
        *(int *)pa = pa[4] < 0 ? 0 : **(int **)(pa + 0x10) - 1;
        MOBY(moby)->scale =
            *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * *(float *)(data + 0xFC);
        *(int *)(data + 0x104) = -1;
        if (fast_dec_timer(data + 0x100)) {
            *(float *)(data + 0x110) = fast_vec_dist(
                *(char **)(pa + 0x10) + 0x10, *(char **)(pa + 0x10) + **(int **)(pa + 0x10) * 0x10);
            easing_frame_count = scale_game_frames(240);
            *(float *)(data + 0x114) =
                *(float *)(data + 0x110) * 4.0f /
                convert_integer_to_float(easing_frame_count * scale_game_frames(240));
            *(int *)(data + 0x10C) = 0;
            moby[0x20] = 1;
        }
        break;
    case PLATFORM_EASED_ENDPOINT_TRAVEL:
        pb = data + 0xB0;
        speed = *(float *)(data + 0x114);
        base = 0.0f;
        if ((MOBY(moby)->oclass == 0x389 || MOBY(moby)->oclass == 0x364) &&
            !voice_is_playing(moby, *(int *)(data + 0x104))) {
            *(int *)(data + 0x104) = trooper_play_sound(0, 4, (int)moby, 0x389);
        }
        easing_frame_count = ++*(int *)(data + 0x10C);
        if (scale_game_frames(240) / 2 < *(int *)(data + 0x10C)) {
            speed = -speed;
            easing_frame_count = scale_game_frames(240) - *(int *)(data + 0x10C);
            base = *(float *)(data + 0x110);
        }
        base += speed * 0.5f * convert_integer_to_float(easing_frame_count * easing_frame_count);
        if (pb[4] < 0) {
            base = *(float *)(data + 0x110) - base;
        }
        fast_vec_sub(endpoint_delta_or_position,
                     *(char **)(pb + 0x10) + **(int **)(pb + 0x10) * 0x10,
                     *(char **)(pb + 0x10) + 0x10);
        *(OvlQuad *)step = *(OvlQuad *)endpoint_delta_or_position;
        fast_vec_normalize(step, step, base);
        fast_vec_add(endpoint_delta_or_position, *(char **)(pb + 0x10) + 0x10, step);
        *(OvlQuad *)&MOBY(moby)->pos = *(OvlQuad *)endpoint_delta_or_position;
        if (*(int *)(data + 0x10C) >= scale_game_frames(240)) {
            pb[4] = pb[4] * -1;
            voice_slot_index = *(int *)(data + 0x104);
            if (voice_slot_index != -1) {
                voice_slot_entry = (unsigned char *)D_0013E550_294c08 + voice_slot_index * 0x70;
                if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                    release_voice_slot(voice_slot_index);
                }
            }
            *(int *)(data + 0x104) = -1;
            moby[0x20] = 2;
            *(int *)(data + 0xE8) = scale_game_frames(*(int *)(data + 0xF0));
        }
        break;
    case PLATFORM_PAUSE_BEFORE_EASED_TRAVEL:
        if (fast_dec_timer(data + 0xE8)) {
            *(int *)(data + 0x10C) = 0;
            moby[0x20] = 1;
        }
        break;
    case PLATFORM_FOLLOW_PATH_NODES:
        pc = data + 0xB0;
        path = *(int **)(pc + 0x10);
        path_node_step = pc[4];
        count = *path;
        end = path_node_step < 0 ? 0 : count - 1;
        next = (*(int *)(data + 0xB0) + count + path_node_step) % count;
        node_off = next * 0x10;
        platform_position = moby + 0x10;
        fast_vec_sub(data + 0xA0, (char *)path + (node_off + 0x10), platform_position);
        next_path_node_byte_offset = node_off;
        target = *(char **)(pc + 0x10) + 0x10;
        if (*(int *)(data + 0x108) != 0) {
            target += next_path_node_byte_offset;
        } else {
            target += end * 0x10;
            node_off =
                0; /* dead store: unused afterwards, but needed for the retail register allocation */
        }
        if (fast_vec_dist(platform_position, target) < *(float *)(data + 0xF4)) {
            approach_value((float *)(data + 0xE4),
                           frame_time * 0.5f,
                           *(float *)(data + 0xEC) * *(float *)(data + 0xEC) /
                               *(float *)(data + 0xF4) * frame_time_sq * 0.5f);
        } else {
            approach_value((float *)(data + 0xE4),
                           *(float *)(data + 0xEC) * frame_time,
                           *(float *)(data + 0xEC) * *(float *)(data + 0xEC) /
                               *(float *)(data + 0xF4) * frame_time_sq * 0.5f);
        }
        FUN_L00_0025f730(data + 0xA0, *(float *)(data + 0xE4));
        fast_vec_add(moby + 0x10, moby + 0x10, data + 0xA0);
        if (fast_vec_dist(moby + 0x10,
                          *(char **)(pc + 0x10) + (next_path_node_byte_offset + 0x10)) <
            *(float *)(data + 0xE4) * 2.0f) {
            *(int *)pc = next;
            if ((next == end || *(int *)(data + 0x108) != 0) && *(int *)(data + 0xF0) > 0) {
                voice_slot_index = *(int *)(data + 0x104);
                if (voice_slot_index != -1) {
                    voice_slot_entry = (unsigned char *)D_0013E550_294c08 + voice_slot_index * 0x70;
                    if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                        release_voice_slot(voice_slot_index);
                    }
                }
                *(int *)(data + 0x104) = -1;
                moby[0x20] = 4;
                *(int *)(data + 0xE8) = scale_game_frames(*(int *)(data + 0xF0));
            }
        }
        break;
    case PLATFORM_PAUSE_BEFORE_NODE_TRAVEL:
        if (fast_dec_timer(data + 0xE8)) {
            moby[0x20] = 3;
        }
        break;
    }
    fast_vec_add(delta, delta, moby + 0x10);
    update_entity_matrix(data + 0x60, delta, old_rot, moby + 0x40);
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002965A0), where it is exact; names translated to the US level program. */

void metropolis_train_carriage_update(struct Moby *moby) __asm__("FUN_L03_002953f8");

void metropolis_train_carriage_update(struct Moby *moby) {
    float position_delta[4];
    float joint_zero_world_position[4];
    char *data;
    char *current_position;
    char *current_rotation;
    char *previous_rotation;
    char *previous_position;
    data = (char *)moby->pvars;
    if ((moby->flags & 2) == 0) {
        current_position = &moby->pos;
        previous_position = data + 0xA0;
        previous_rotation = data + 0xB0;
        current_rotation = &moby->rot;
        if (moby->state == 0) {
            *(int *)(data + 0x60) = 0;
            *(short *)(data + 0x64) = 0;
            data[0x68] = 4;
            *(short *)(data + 0x7E) = 0xD;
            moby_point_position(moby, 0, joint_zero_world_position);
            *(float *)(data + 0xC4) = fast_vec_dist(current_position, joint_zero_world_position);
            qcopy(previous_position, current_position);
            qcopy(previous_rotation, current_rotation);
            *(int *)(data + 0x5C) = 3;
            moby->update_dist = moby->draw_dist = 0xFF;
        }
        fast_vec_sub(position_delta, current_position, previous_position);
        update_entity_matrix(data + 0x20, position_delta, previous_rotation, current_rotation);
        qcopy(previous_position, current_position);
        qcopy(previous_rotation, current_rotation);
    }
}
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_0029DB88), where it is exact; names translated to the US level program. */

extern int D_L03_0015F5CC; /* no foreign declaration */

void maintain_emitter_sound_voice(struct Moby *moby) __asm__("FUN_L03_0029c9d0");

void maintain_emitter_sound_voice(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (moby->state == 0) {
        *(int *)(data + 0x150) = -1;
        return;
    }
    if ((D_L03_0015F5CC & 7) != (((int)moby >> 8) & 7))
        return;
    if (*(short *)(data + 0x14A) != 0)
        return;
    if (*(int *)(data + 0x150) >= 0) {
        if (voice_is_playing(moby, *(int *)(data + 0x150)) != 0)
            return;
    }
    switch (moby->oclass) {
    case MOBY_CLASS_JOINT_ZERO_TEN_PARTICLE_EMITTER:
    case MOBY_CLASS_GRAY_JOINT_ZERO_TEN_PARTICLE_EMITTER:
        *(int *)(data + 0x150) = allocate_voice_for_target_entry(0, 4, (int)moby);
        break;
    case MOBY_CLASS_ALTERNATE_COLOR_CHILD_EFFECT_EMITTER:
    case MOBY_CLASS_TEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_TWO_JOINT_PARTICLE_EMITTER:
    case MOBY_CLASS_FIFTEEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_DEFAULT_COLOR_CHILD_EFFECT_EMITTER:
        *(int *)(data + 0x150) = allocate_voice_for_target_entry(1, 4, (int)moby);
        break;
    }
}

extern int  FUN_L00_001fefc8(void *);
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, int, float) __asm__("FUN_L00_0026ced0");
struct Moby;
char *spawn_emitter_child_effect(struct Moby *parent,
                                 int          unused,
                                 int          color,
                                 int          secondary_color,
                                 int          timer_frames,
                                 float        f,
                                 float        scale) __asm__("FUN_L03_002bad40");

/* Hit effects for the Kerwan train and turret classes: sparks or smoke puffs pushed along the hit velocity,
 * with countdowns in the moby data (0x12E, 0x12F) limiting how often they spawn. */
void update_moby_emitter_effects(char *moby, void *vel_in, int quiet) __asm__("FUN_L03_0029caa8");

void update_moby_emitter_effects(char *moby, void *vel_in, int quiet) {
    float  emitter_motion_delta[4];
    float *vp;
    float  joint_world_position[4];
    float  particle_velocity[4];
    char  *data;
    int    i;
    int    j;
    int    k;
    float  azimuth_angle;
    float  elevation_angle;
    int    color;
    int    color2;

    data = ((char *)MOBY(moby)->pvars);
    vp = emitter_motion_delta;
    *(OvlQuad *)vp = *(OvlQuad *)vel_in;
    if (MOBY(moby)->flags & 1) {
        return;
    }
    switch (MOBY(moby)->oclass) {
    case MOBY_CLASS_TWO_JOINT_PARTICLE_EMITTER:
        if (quiet == 0) {
            fast_vec_scale(vp, vp, 0.35f);
            for (i = 0; i < 2; i++) {
                moby_point_position(moby, i, joint_world_position);
                azimuth_angle = random_angle_radians();
                elevation_angle = random_angle_radians();
                build_spherical_offset(
                    particle_velocity, frame_time * 0.15f, azimuth_angle, elevation_angle);
                fast_vec_add(particle_velocity, particle_velocity, vp);
                spawn_effect_particle_with_target_scalar(joint_world_position,
                                                         particle_velocity,
                                                         0x8040C0F0,
                                                         0x808080,
                                                         scale_game_frames(20),
                                                         84000.0f);
            }
        }
        break;
    case MOBY_CLASS_TEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_JOINT_ZERO_TEN_PARTICLE_EMITTER:
    case MOBY_CLASS_GRAY_JOINT_ZERO_TEN_PARTICLE_EMITTER:
    case MOBY_CLASS_TEN_GRAY_PARTICLE_BURST_EMITTER:
        if (quiet == 0 && FUN_L00_001fefc8(data + 0x12E)) {
            if (fast_vec_length(vp) < frame_time * 40.0f &&
                fast_vec_dist(moby + 0x10, D_L03_00166EC0) < 75.0f) {
                fast_vec_scale(vp, vp, 0.7f);
                for (j = 0; j < 10; j++) {
                    moby_point_position(moby, 0, joint_world_position);
                    azimuth_angle = random_angle_radians();
                    elevation_angle = random_angle_radians();
                    build_spherical_offset(
                        particle_velocity, frame_time * 0.6f, azimuth_angle, elevation_angle);
                    fast_vec_add(particle_velocity, particle_velocity, vp);
                    spawn_effect_particle_with_target_scalar(joint_world_position,
                                                             particle_velocity,
                                                             0x80808080,
                                                             0x808080,
                                                             scale_game_frames(35),
                                                             105000.0f);
                }
            }
            data[0x12E] = random_int_between(scale_game_frames(7), scale_game_frames(20));
        }
        break;
    case MOBY_CLASS_FIFTEEN_PARTICLE_BURST_EMITTER:
        if (quiet == 0 && FUN_L00_001fefc8(data + 0x12E)) {
            if (fast_vec_length(vp) < frame_time * 40.0f &&
                fast_vec_dist(moby + 0x10, D_L03_00166EC0) < 90.0f) {
                fast_vec_scale(vp, vp, 0.5f);
                for (k = 0; k < 15; k++) {
                    moby_point_position(moby, 0, joint_world_position);
                    azimuth_angle = random_angle_radians();
                    elevation_angle = random_angle_radians();
                    build_spherical_offset(
                        particle_velocity, frame_time * 1.2f, azimuth_angle, elevation_angle);
                    fast_vec_add(particle_velocity, particle_velocity, vp);
                    spawn_effect_particle_with_target_scalar(joint_world_position,
                                                             particle_velocity,
                                                             0x80808080,
                                                             0x808080,
                                                             scale_game_frames(45),
                                                             168000.0f);
                }
            }
            data[0x12E] = random_int_between(scale_game_frames(7), scale_game_frames(20));
        }
        break;
    case MOBY_CLASS_ALTERNATE_COLOR_CHILD_EFFECT_EMITTER:
    case MOBY_CLASS_DEFAULT_COLOR_CHILD_EFFECT_EMITTER:
        FUN_L00_001fefc8(data + 0x12F);
        if (quiet != 0 && ((unsigned char *)data)[0x12F] == 0) {
            break;
        }
        FUN_L00_001fefc8(data + 0x12E);
        if (((unsigned char *)data)[0x12E] == 0 || ((unsigned char *)data)[0x12F] != 0) {
            moby_point_position(moby, 0, data + 0xF0);
        }
        if (((unsigned char *)data)[0x12E] != 0) {
            break;
        }
        if (fast_vec_length(vp) < frame_time * 40.0f &&
            fast_vec_dist(moby + 0x10, D_L03_00166EC0) < 90.0f) {
            color = 0xA040;
            color2 = 0xA080;
            if (MOBY(moby)->oclass == 0x4B) {
                color = 0x30A0;
                color2 = 0x90A0;
            }
            spawn_emitter_child_effect((struct Moby *)moby,
                                       (int)(data + 0xF0),
                                       color,
                                       color2,
                                       scale_game_frames(50),
                                       frame_time * 2.5f,
                                       0.15f);
            ((unsigned char *)data)[0x12F] = scale_game_frames(50);
        }
        data[0x12E] = scale_game_frames(13);
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_0029E1A8), where it is exact; names translated to the US level program. */

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

void update_spline_moby_orientation(struct Moby *moby) __asm__("FUN_L03_0029cff0");

void update_spline_moby_orientation(struct Moby *moby) {
    char *moby_data = (char *)moby->pvars;
    float position_delta[4];
    float yaw_offset_radians = (float)*(short *)(moby_data + 0x132) * DEG_TO_RAD;
    float yaw_step, x, y, z, w;
    fast_subtract_rotations(moby->rot.z, yaw_offset_radians);
    x = fast_arc_tan(*(float *)(moby_data + 0xD0) - *(float *)(moby_data + 0xE0),
                     *(float *)(moby_data + 0xD4) - *(float *)(moby_data + 0xE4));
    x = fast_add_rotations(x, yaw_offset_radians);
    y = fast_subtract_rotations(x, moby->rot.z);
    yaw_step = y * (1.0f / *(float *)(moby_data + 0x120) * frame_scale);
    moby->rot.z = fast_add_rotations(yaw_step, moby->rot.z);
    fast_vec_sub(position_delta, moby_data + 0xD0, moby_data + 0xE0);
    z = fast_arc_tan(vector_length_xy(position_delta), position_delta[2]);
    moby->rot.y = spring_turn((float *)(moby_data + 0x11C),
                              moby->rot.y,
                              -z,
                              *(float *)(moby_data + 0x118) * 10.0f * DEG_TO_RAD * frame_time_sq,
                              *(float *)(moby_data + 0x118) * 5.0f * DEG_TO_RAD * frame_time_sq,
                              *(float *)(moby_data + 0x118) * DEG_TO_RAD * frame_time);
    w = normalize_angle(yaw_step * *(float *)(moby_data + 0x124));
    w = fast_subtract_rotations(moby->rot.x, w);
    w = normalize_angle(w * *(float *)(moby_data + 0x128));
    moby->rot.x = spring_turn((float *)(moby_data + 0x114),
                              moby->rot.x,
                              w,
                              *(float *)(moby_data + 0x110) * 10.0f * DEG_TO_RAD * frame_time_sq,
                              *(float *)(moby_data + 0x110) * 5.0f * DEG_TO_RAD * frame_time_sq,
                              *(float *)(moby_data + 0x110) * DEG_TO_RAD * frame_time);
}

extern char          D_0013E550_29d2e0[] __asm__("D_0013E550");
extern unsigned char D_0013D408_29d2e0[] __asm__("D_0013D408");
extern float         D_L03_0015F580_macro[] __asm__("D_L03_0015F580") MACRO_ADDR;
extern void         *D_L03_00160770;
extern void          allocate_voice_for_group_entry(int, int, void *) __asm__("FUN_0022dba0");
extern void spawn_randomized_bolt_drop(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
extern void spawn_explosion(void *,
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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern void spawn_debris_29d2e0(char *,
                                int,
                                float *,
                                void *,
                                int,
                                int,
                                float,
                                float *,
                                float *,
                                float *) __asm__("FUN_L00_00263fd8");
void        red_ship_1_update(char *moby) __asm__("FUN_L03_0029dba8");

#define SPAWN_DEBRIS(id)                           \
    spawn_debris_29d2e0(moby,                      \
                        (id),                      \
                        (float *)&MOBY(moby)->pos, \
                        moby + 0x40,               \
                        scale_game_frames(90),     \
                        0,                         \
                        frame_time_sq * 12.0f,     \
                        push,                      \
                        D_L03_0015F580_macro,      \
                        D_L03_0015F580_macro)

/* Wreck reaction for the Kerwan train and turret classes: when hit by a qualifying attack it bursts into
 * the class debris, starts the shake/smoke effects and arms the respawn countdown (0x14A); once that runs
 * out the moby is hidden again unless the caller keeps it, and switches its update to the car update. */
void update_spline_moby_destruction_and_respawn(char *moby,
                                                void *vel_in,
                                                int   flag) __asm__("FUN_L03_0029d2e0");

void update_spline_moby_destruction_and_respawn(char *moby, void *vel_in, int flag) {
    float          spot[4];
    float          halved_motion_delta[4];
    float          push[4];
    char          *data;
    char          *hit;
    int            voice_slot_index;
    unsigned char *voice_slot_entry;

    data = ((char *)MOBY(moby)->pvars);
    hit = find_moby_hit(moby, 0xA30000, 0);
    if (hit != 0 && *(unsigned short *)(hit + 0x28) != 0x102) {
        clear_u64_value(spot);
        if (MOBY(moby)->oclass == 0x4B) {
            spot[0] = fast_cos(MOBY(moby)->rot.z) * -2.747f;
            spot[1] = fast_sin(MOBY(moby)->rot.z) * -2.747f;
            spot[2] = 0.0f;
        }
        *(OvlQuad *)halved_motion_delta = *(OvlQuad *)vel_in;
        fast_vec_scale(halved_motion_delta, halved_motion_delta, 0.5f);
        fast_vec_add(spot, spot, moby + 0x10);
        if (*(int *)(hit + 0x24) & 0x800000) {
            spawn_randomized_bolt_drop(moby, 10, 20, 2, -1);
        } else {
            spawn_randomized_bolt_drop(moby, 3, 5, 2, -1);
        }
        *(short *)(data + 0x14A) = scale_game_frames(600);
        spawn_explosion(moby,
                        halved_motion_delta,
                        spot,
                        0.0f,
                        0.0f,
                        20,
                        3,
                        4,
                        4.0f,
                        2.0f,
                        100000.0f,
                        -1,
                        3.0f,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        fast_vec_scale(push, moby + 0xC0, frame_scale * 0.075f);
        push[2] += frame_scale * 0.08f;
        switch (MOBY(moby)->oclass) {
        case MOBY_CLASS_TEN_PARTICLE_BURST_EMITTER:
            SPAWN_DEBRIS(0x6A9);
            SPAWN_DEBRIS(0x6AA);
            SPAWN_DEBRIS(0x6AB);
            break;
        case MOBY_CLASS_TWO_JOINT_PARTICLE_EMITTER:
            SPAWN_DEBRIS(0x6AC);
            SPAWN_DEBRIS(0x6AD);
            SPAWN_DEBRIS(0x6AE);
            break;
        case MOBY_CLASS_JOINT_ZERO_TEN_PARTICLE_EMITTER:
            SPAWN_DEBRIS(0x6AF);
            SPAWN_DEBRIS(0x6B0);
            SPAWN_DEBRIS(0x6B1);
            break;
        case MOBY_CLASS_FIFTEEN_PARTICLE_BURST_EMITTER:
            SPAWN_DEBRIS(0x6B2);
            SPAWN_DEBRIS(0x6B3);
            SPAWN_DEBRIS(0x6B4);
            break;
        case MOBY_CLASS_DEFAULT_COLOR_CHILD_EFFECT_EMITTER:
            SPAWN_DEBRIS(0x6B5);
            SPAWN_DEBRIS(0x6B6);
            SPAWN_DEBRIS(0x6B7);
            break;
        case MOBY_CLASS_GRAY_JOINT_ZERO_TEN_PARTICLE_EMITTER:
            SPAWN_DEBRIS(0x6B8);
            SPAWN_DEBRIS(0x6B9);
            SPAWN_DEBRIS(0x6BA);
            break;
        case MOBY_CLASS_TEN_GRAY_PARTICLE_BURST_EMITTER:
            SPAWN_DEBRIS(0x6BC);
            SPAWN_DEBRIS(0x6BE);
            SPAWN_DEBRIS(0x6BF);
            break;
        }
        if (MOBY(moby)->oclass == 0x31B && D_0013D408_29d2e0[4] == 0) {
            D_0013D408_29d2e0[4] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
        allocate_voice_for_group_entry(0, 0, moby);
    }
    D_L03_00160770 = (void *)red_ship_1_update;
    ((unsigned char *)moby)[0xA4] = 0xFF;
    if (*(short *)(data + 0x14A) != 0) {
        if (fast_dec_timer_s16(data + 0x14A) && flag) {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            MOBY(moby)->flags = (MOBY(moby)->flags & 0xFFBE) | 0x1000;
        } else {
            MOBY(moby)->collision = 0;
            MOBY(moby)->flags |= 0x41;
            voice_slot_index = *(int *)(data + 0x150);
            if (voice_slot_index != -1) {
                voice_slot_entry = (unsigned char *)D_0013E550_29d2e0 + voice_slot_index * 0x70;
                if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                    release_voice_slot(voice_slot_index);
                }
            }
            *(int *)(data + 0x150) = -1;
            MOBY(moby)->flags &= 0xEFFF;
        }
    }
}
#undef SPAWN_DEBRIS
extern char *D_L03_0015FFD8_29dba8 __asm__("D_L03_0015FFD8");
extern char  D_0013E550_29dba8[] __asm__("D_0013E550");
extern char  spline_moby_missing_path_warning_format[] __asm__("D_L03_001E2A28");
extern int   get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int   get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern int   check_scaled_vector_bsphere(char *, float) __asm__("FUN_L01_00275690");
extern int   select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");
extern int   resolve_path_node_index(void *, int) __asm__("FUN_L01_0028b828");
extern int   advance_path_node_index(char *) __asm__("FUN_L01_0028b878");
extern void  initialize_path_descriptor(char *) __asm__("FUN_L01_0028b410");
extern void
compute_path_node_tangent(void *out, int *l, int idx, float a, float b) __asm__("FUN_L01_0028b8c8");
extern void evaluate_cubic_hermite_segment(
    float t, void *out, void *p1, void *p2, void *p3, void *p4) __asm__("FUN_L01_0028ba80");
extern void
compute_sampled_spline_path_lengths(void *, int, void *, float, float) __asm__("FUN_L01_0028bb90");
extern void add_moby_rotation_based_offsets(char *, char *) __asm__("FUN_L03_0029c8a8");
extern void
update_spline_moby_destruction_and_respawn(char *, void *, int) __asm__("FUN_L03_0029d2e0");
extern void report_29dba8(void *, int, int) __asm__("FUN_001e93b0");

/* Train car update: keeps the cars of a group in step (stops them all when none can move), and on its
 * first frame snaps to the nearest path node and sets up the spline; then runs along the path spline,
 * steering, banking and spawning its hit/smoke effects; state 2 waits without a path, 3 waits a timer. */
void red_ship_1_update(char *moby) {
    enum {
        RED_SHIP_INITIALIZE = 0,
        RED_SHIP_FOLLOW_PATH_SPLINE = 1,
        RED_SHIP_WAIT_FOR_PATH = 2,
        RED_SHIP_WAIT_FOR_MOVEMENT_TIMER = 3
    };
    float          zero[4];
    float          old_rot[4];
    float          old_pos[4];
    float          culling_sphere_or_motion_delta[4];
    float          position_delta[4];
    char          *item;
    char          *data;
    char          *pm;
    char          *path_tangent_or_moby_position;
    char          *initial_path_tangent;
    char          *moby_position;
    char          *path_node_base;
    char          *group_member_data;
    int           *path;
    int            flag;
    int            found;
    int            advanced_path_node_index;
    int            initial_path_node_index;
    int            voice_slot_index;
    unsigned char *voice_slot_entry;
    float          initial_speed_oscillation_phase;
    float          step;

    *(OvlQuad *)zero = 0;
    flag = 0;
    data = ((char *)MOBY(moby)->pvars);
    if (((unsigned char *)moby)[0x31] == 0) {
        *(OvlQuad *)culling_sphere_or_motion_delta = *(OvlQuad *)&MOBY(moby)->pos;
        culling_sphere_or_motion_delta[3] = 7.0f;
        flag = fast_bsphere_check(culling_sphere_or_motion_delta, MOBY(moby)->draw_dist) == -1;
    }
    if (data == 0) {
        return;
    }
    pm = data + 0x60;
    if (((unsigned char *)moby)[0x21] != 0xFF && ((unsigned char *)moby)[0x20] != 0 &&
        ((unsigned char *)moby)[0x20] != 3) {
        found = 0;
        get_first_moby_in_group((int *)&item, ((unsigned char *)moby)[0x21], 0, 0);
        if (item == moby) {
            goto test;
            for (;;) {
                get_next_moby_in_group((int *)&item, (int)item, 0, 0);
            test:
                if (item == 0) {
                    break;
                }
                if (check_scaled_vector_bsphere(
                        item, convert_integer_to_float(*(short *)(item + 0x32))) != -1) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                get_first_moby_in_group((int *)&item, ((unsigned char *)moby)[0x21], 0, 0);
                while (item != 0) {
                    group_member_data = *(char **)(item + 0x78);
                    item[0x20] = 3;
                    *(short *)(group_member_data + 0x12C) = scale_game_frames(5);
                    get_next_moby_in_group((int *)&item, (int)item, 0, 0);
                }
            }
        }
    }
    maintain_emitter_sound_voice((unsigned char *)moby);
    update_emitter_intermittent_sound(moby);
    switch (((unsigned char *)moby)[0x20]) {
    case RED_SHIP_INITIALIZE:
        MOBY(moby)->flags |= 0x1000;
        if (*(int *)(pm + 0x14) != -1) {
            path = (int *)l03_paths[*(int *)(pm + 0x14)];
            if (fast_vec_dist((char *)path + 0x10, (char *)path + *path * 0x10) < 0.5f) {
                *path -= 1;
            }
        } else {
            report_29dba8(spline_moby_missing_path_warning_format,
                          MOBY(moby)->oclass,
                          truncate_float_to_s32((moby - D_L03_0015FFD8_29dba8) >> 8));
        }
        ((unsigned char *)moby)[0x30] = 0xFF;
        data[0x2B] = 1;
        data[0x2C] = 0x25;
        if (*(float *)(data + 0x140) > 500.0f) {
            *(float *)(data + 0x140) =
                random_float_between(-1.0f, 1.0f) * *(float *)(data + 0x140) / 1000.0f;
        }
        if (*(float *)(data + 0x144) > 500.0f) {
            *(float *)(data + 0x144) =
                random_float_between(-1.0f, 1.0f) * *(float *)(data + 0x144) / 1000.0f;
        }
        if (*(int *)(pm + 0x14) == -1) {
            moby[0x20] = 2;
            break;
        }
        moby_position = moby + 0x10;
        data[0x131] = ((moby - D_L03_0015FFD8_29dba8) >> 8) / 2 % 7;
        initialize_path_descriptor(pm);
        if (((unsigned char *)pm)[5] == 0) {
            if (*(int *)pm <= 0) {
                initial_path_node_index =
                    select_point_index_by_distance(moby_position, *(void **)(pm + 0x10), 0.0f);
                *(int *)pm = initial_path_node_index;
                if (pm[4] >= 0 ? initial_path_node_index == **(int **)(pm + 0x10) - 1
                               : initial_path_node_index == 0) {
                    *(int *)pm = 0;
                }
            }
            *(float *)(pm + 0xC) =
                fast_vec_dist(moby_position, *(char **)(pm + 0x10) + (*(int *)pm * 0x10 + 0x10));
            path_node_base = *(char **)(pm + 0x10) + *(int *)pm * 0x10;
            *(float *)(pm + 0x18) =
                fast_arc_tan(*(float *)(path_node_base + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(path_node_base + 0x14) - MOBY(moby)->pos.y);
            pm[5] = 2;
            *(int *)(pm + 0x8) = 0;
            *(int *)(pm + 0x1C) = 0;
        }
        initial_path_tangent = data + 0xA0;
        *(OvlQuad *)(data + 0xE0) = *(OvlQuad *)(*(char **)(pm + 0x10) + *(int *)pm * 0x10 + 0x10);
        *(float *)(data + 0x100) = 1.0f;
        compute_path_node_tangent(
            initial_path_tangent, (int *)pm, 0, *(float *)(data + 0x104), *(float *)(data + 0x108));
        *(OvlQuad *)(data + 0xB0) = *(OvlQuad *)(data + 0xC0) =
            *(OvlQuad *)(*(char **)(pm + 0x10) + *(int *)pm * 0x10 + 0x10);
        MOBY(moby)->rot.x = 0.0f;
        MOBY(moby)->rot.y =
            -fast_arc_tan(vector_length_xy(initial_path_tangent), *(float *)(data + 0xA8));
        MOBY(moby)->rot.z =
            fast_arc_tan(*(float *)(data + 0xA0) - zero[0], *(float *)(data + 0xA4) - zero[1]);
        *(float *)(data + 0x114) = 0.0f;
        *(float *)(data + 0x11C) = 0.0f;
        *(OvlQuad *)&MOBY(moby)->pos = *(OvlQuad *)(data + 0xE0);
        add_moby_rotation_based_offsets(moby, moby_position);
        data[0x131] = 7;
        initial_speed_oscillation_phase = *(float *)(data + 0x13C);
        if (initial_speed_oscillation_phase > 6.2831855f) {
            *(float *)(data + 0x13C) = initial_speed_oscillation_phase * DEG_TO_RAD;
        } else if (*(float *)(data + 0x134) != 0.0f && initial_speed_oscillation_phase == -1.0f) {
            *(float *)(data + 0x13C) = random_angle_radians();
        }
        *(OvlQuad *)(data + 0xD0) = *(OvlQuad *)(data + 0xC0);
        compute_sampled_spline_path_lengths(
            pm, 0, moby, *(float *)(data + 0x104), *(float *)(data + 0x108));
        if (*(short *)(data + 0x12C) > 0) {
            moby[0x20] = 3;
        } else {
            *(short *)(data + 0x14A) = 0;
            moby[0x20] = 1;
        }
        break;
    case RED_SHIP_WAIT_FOR_PATH:
        if (*(int *)(pm + 0x14) != -1) {
            moby[0x20] = 0;
        }
        voice_slot_index = *(int *)(data + 0x150);
        if (voice_slot_index != -1) {
            voice_slot_entry = (unsigned char *)D_0013E550_29dba8 + voice_slot_index * 0x70;
            if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                release_voice_slot(voice_slot_index);
            }
        }
        *(int *)(data + 0x150) = -1;
        break;
    case RED_SHIP_WAIT_FOR_MOVEMENT_TIMER:
        if (fast_dec_timer_s16(data + 0x12C)) {
            moby[0x20] = 1;
        }
        break;
    case RED_SHIP_FOLLOW_PATH_SPLINE:
        *(OvlQuad *)old_rot = *(OvlQuad *)&MOBY(moby)->rot;
        *(OvlQuad *)old_pos = *(OvlQuad *)&MOBY(moby)->pos;
        path_tangent_or_moby_position = data + 0xA0;
        if (*(float *)(data + 0x100) >= 1.0f) {
            *(float *)(data + 0x100) -= 1.0f;
            advanced_path_node_index = advance_path_node_index(pm);
            *(OvlQuad *)(data + 0x90) = *(OvlQuad *)(data + 0xA0);
            compute_path_node_tangent(path_tangent_or_moby_position,
                                      (int *)pm,
                                      1,
                                      *(float *)(data + 0x104),
                                      *(float *)(data + 0x108));
            *(OvlQuad *)(data + 0xB0) = *(OvlQuad *)(data + 0xC0);
            *(OvlQuad *)(data + 0xC0) = *(
                OvlQuad *)(*(char **)(pm + 0x10) +
                           resolve_path_node_index(pm, advanced_path_node_index + 1) * 0x10 + 0x10);
        }
        step = (*(float *)(data + 0x10C) +
                *(float *)(data + 0x134) * fast_sin(*(float *)(data + 0x13C))) /
               *(float *)(data + 0xBC);
        *(float *)(data + 0x13C) =
            fast_add_rotations(*(float *)(data + 0x13C), *(float *)(data + 0x138) * DEG_TO_RAD);
        *(OvlQuad *)(data + 0xE0) = *(OvlQuad *)(data + 0xD0);
        evaluate_cubic_hermite_segment(*(float *)(data + 0x100) + step,
                                       data + 0xD0,
                                       data + 0xB0,
                                       data + 0xC0,
                                       data + 0x90,
                                       path_tangent_or_moby_position);
        if (!flag) {
            update_spline_moby_orientation(moby);
        } else {
            clear_u64_value(moby + 0x40);
            MOBY(moby)->rot.z = fast_arc_tan(*(float *)(data + 0xD0) - *(float *)(data + 0xE0),
                                             *(float *)(data + 0xD4) - *(float *)(data + 0xE4));
        }
        path_tangent_or_moby_position = moby + 0x10;
        *(float *)(data + 0x100) += step;
        *(OvlQuad *)&MOBY(moby)->pos = *(OvlQuad *)(data + 0xE0);
        if (!flag) {
            add_moby_rotation_based_offsets(moby, path_tangent_or_moby_position);
        }
        fast_vec_sub(position_delta, path_tangent_or_moby_position, old_pos);
        *(OvlQuad *)culling_sphere_or_motion_delta = *(OvlQuad *)position_delta;
        update_moby_emitter_effects(moby, culling_sphere_or_motion_delta, flag);
        update_spline_moby_destruction_and_respawn(moby, culling_sphere_or_motion_delta, flag);
        break;
    }
}

void set_moby_color(char *, int, int, int) __asm__("FUN_L00_002502f0");
void f250df8_2bad40(char *) __asm__("FUN_L00_00250df8");
/* Spawns a child effect (moby 0xEB) on the parent: copies its position and rotation, then sets the colour, timers and scale. */
char *spawn_emitter_child_effect(struct Moby *parent,
                                 int          unused,
                                 int          color,
                                 int          secondary_color,
                                 int          timer_frames,
                                 float        f,

                                 float scale) {
    char *child_moby = create_moby(0xEB);
    if (child_moby) {
        char *child_data = ((char *)MOBY(child_moby)->pvars);
        int   r, g, b;
        MOBY(child_moby)->update_dist = 0xFF;
        MOBY(child_moby)->draw_dist = 0x7F;
        child_moby[0x31] = 1;
        MOBY(child_moby)->scale *= scale;
        *(char **)child_data = parent;
        qcopy(child_moby + 0x10, (char *)parent->pvars + 0xF0);
        *(OvlQuad *)&MOBY(child_moby)->rot = *(OvlQuad *)&parent->rot;
        MOBY(child_moby)->rot.x = random_angle_radians();
        *(short *)(child_data + 0xC) = timer_frames;
        b = (color >> 16) & 0xFF;
        g = (color >> 8) & 0xFF;
        r = color & 0xFF;
        *(short *)(child_data + 0xE) = timer_frames;
        *(float *)(child_data + 0x14) = f;
        *(int *)(child_data + 0x4) = color;
        *(int *)(child_data + 0x8) = secondary_color;
        set_moby_color(child_moby, r, g, b);
        f250df8_2bad40(child_moby);
    }
    return child_moby;
}

typedef struct {
    float pos[4];
    float v10[4];
    float dir[4];
    float v30[4];
    void *obj;
    int   pad[3];
} Tgt2c5bb8;

typedef struct {
    char  p00[0x20];
    float f20;
    short s24;
    char  p26[3];
    u8    b29;
    char  p2A[0x16];
    float v40[4];
    char  p50[8];
    u8    b58;
    char  p59;
    u8    b5A;
    char  p5B[0x6D];
    short sC8;
    char  pCA[6];
    void *pD0;
    char  pD4[0x3C];
    char  m110[0x10];
    char  m120[0x60];
    int   i180;
    char  p184[4];
    float f188;
    float f18C;
    char  p190[0x14];
    float f1A4;
    float f1A8;
    float f1AC;
    char  p1B0[8];
    int   i1B8;
    float f1BC;
    float f1C0;
    float f1C4;
    char  p1C8[0x38];
    float home[4];
    char  p210[0x10];
    float v220[4];
    float f230;
    float f234;
    char  p238[4];
    int   i23C;
    int   i240;
    int   i244;
    int   i248;
    char  p24C[0x14];
    int   i260;
    float f264;
    char  p268[4];
    int   i26C;
    int   i270;
    float f274;
    int   i278;
    int   i27C;
    char  m280[0x10];
} Data2c5bb8;

extern char *D_L03_0015FFD8_c5bb8 __asm__("D_L03_0015FFD8");
char         D_L03_001618D0 = 3;
extern float D_L03_0015F580_macro[] __asm__("D_L03_0015F580") MACRO_ADDR;
extern float FUN_L04_001f1978(float);
extern int   rand(void) __asm__("FUN_001160d8");
extern int   FUN_L00_00258b50(void *, void *, void *, void *, float);
extern int   FUN_L00_00259028(char *m, char *p, char *pos, char *v);
extern int   FUN_L00_002dc9f0(void *, void *);
extern int select_moby_target_with_hero_fallback(char *, char *, float) __asm__("FUN_L03_0024e830");
extern int is_point_inside_clip_volume_cf(int id) __asm__("FUN_L03_002c6d98");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_0025a120(void *);
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_00263ac8_c5bb8(void *, int, void *, float) __asm__("FUN_L00_00263ac8");
extern void
spawn_moby_break_fragments(void *, int, int, int, int, int, int) __asm__("FUN_L00_00263e30");
extern void FUN_L01_0026d930(char *p, void *);
void        update_robot_dog_hit_reaction(struct Moby *moby) __asm__("FUN_L03_002c6a20");

extern void update_robot_dog_hit_reaction(struct Moby *moby) __asm__("FUN_L03_002c6a20");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void spawn_debris_c5bb8(char *,
                               int,
                               float *,
                               void *,
                               int,
                               int,
                               float,
                               float *,
                               float *,
                               float *) __asm__("FUN_L00_00263fd8");

/* Kerwan enemy update: waits at home, walks its path or chases the target picked by FUN_L03_0024e830
 * (its partner moby data->0x248 is a class 0x23E), attacks in range, returns home, dies (states 8, 9, 21). */
void robot_dog_update(char *moby) __asm__("FUN_L03_002c5bb8");

void robot_dog_update(char *moby) {
    enum {
        ROBOT_DOG_INITIALIZE = 0,
        ROBOT_DOG_WAIT_FOR_ACTIVATION = 1,
        ROBOT_DOG_WAIT_FOR_PARTNER_RELEASE = 2,
        ROBOT_DOG_TURN_TO_TARGET = 3,
        ROBOT_DOG_CHASE_TARGET = 4,
        ROBOT_DOG_FOLLOW_PATH = 5,
        ROBOT_DOG_MELEE_ATTACK = 6,
        ROBOT_DOG_RETURN_HOME = 7,
        ROBOT_DOG_FALL_RECOVERY = 8,
        ROBOT_DOG_WAIT_TO_REINITIALIZE = 9,
        ROBOT_DOG_DEATH_FALL = 21
    };
    Tgt2c5bb8   selected_target;
    float       forward_vector_or_path_point[4];
    float       push[4];
    float       at[4];
    int         seg;
    float       frac;
    Data2c5bb8 *data;
    int        *path;
    char       *activation_path;
    char       *partner_moby;
    int         partner_moby_index;
    float       target_distance;
    float       initialization_multiplier_or_desired_heading;

    data = ((Data2c5bb8 *)MOBY(moby)->pvars);
    select_moby_target_with_hero_fallback(moby, (char *)&selected_target, data->f230);
    FUN_L00_00263ac8_c5bb8(moby, 0, data->m280, 2.1f);
    update_robot_dog_hit_reaction((struct Moby *)moby);
    fast_dec_timer(&data->i244);
    switch (((unsigned char *)moby)[0x20]) {
    case ROBOT_DOG_INITIALIZE:
        MOBY(moby)->flags |= 0x1000;
        qcopy(data->home, moby + 0x10);
        moby[0x20] = 1;
        moby[0x7F] = 0x18;
        data->f20 = 1.0f;
        data->b29 = 0;
        data->i240 = 0;
        data->s24 = 1;
        FUN_L01_0026d930((char *)&data->i180, data->home);
        data->f1BC = frame_time_sq * 6.2831855f;
        data->f1C0 = frame_time_sq * 12.566371f;
        data->f1C4 = frame_time * 3.1415927f;
        data->f188 = data->f18C = 2.0f;
        data->i180 = 0x38D;
        data->f1A4 = data->f234 * frame_time;
        data->i1B8 |= 0x20;
        data->f1A8 = 2.0954423f;
        initialization_multiplier_or_desired_heading = FUN_L04_001f1978(6.0f);
        data->f264 = 1.0f;
        data->i244 = 0;
        data->i26C = 0;
        data->i260 = 0;
        data->f1AC = data->f1A4 * initialization_multiplier_or_desired_heading;
        data->b58 = 8;
        data->b5A = 7;
        if (rand() & 1) {
            MOBY(moby)->flags |= 0x8000;
        }
        data->pD0 = &D_L03_001618D0;
        if (data->i248 != -1 && data->i270 == -1) {
            if (((unsigned char *)moby)[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, 0);
            }
            moby[0x20] = 2;
        } else if (((unsigned char *)moby)[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 0);
        }
        break;
    case ROBOT_DOG_WAIT_FOR_ACTIVATION:
        if (data->i270 != -1) {
            activation_path = ((char *)l03_paths[data->i270]);
            *(OvlQuad *)&MOBY(moby)->pos = *(OvlQuad *)(activation_path + 0x10);
            MOBY(moby)->rot.z = fast_arc_tan(
                *(float *)(activation_path + 0x20) - *(float *)(activation_path + 0x10),
                *(float *)(activation_path + 0x24) - *(float *)(activation_path + 0x14));
            if ((random_integer_below(14) == 0 &&
                 is_point_inside_clip_volume_cf(((unsigned char *)moby)[0x21])) ||
                is_point_inside_clip_volume(&selected_target, data->i27C) ||
                (fast_vec_dist(&selected_target, moby + 0x10) < data->f274 &&
                 absolute_float(selected_target.pos[2] - MOBY(moby)->pos.z) < 4.0f)) {
                data->i278 = 0;
                moby[0x20] = 5;
                if (((unsigned char *)moby)[0x53] != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
            }
        } else if (random_integer_below(14) == 0 &&
                   fast_vec_dist(&selected_target, data->home) < data->f230 &&
                   absolute_float(selected_target.pos[2] - MOBY(moby)->pos.z) < 3.0f) {
            moby[0x20] = 3;
            if (((unsigned char *)moby)[0x53] != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
            MOBY(moby)->anim_speed = 0.8780488f;
        }
        break;
    case ROBOT_DOG_WAIT_FOR_PARTNER_RELEASE:
        if (random_integer_below(14) == 0) {
            partner_moby_index = data->i248;
            if (partner_moby_index == -1 ||
                (partner_moby = D_L03_0015FFD8_c5bb8 + (partner_moby_index << 8)) == 0 ||
                ((unsigned char *)partner_moby)[0x20] == 0xFE ||
                ((unsigned char *)partner_moby)[0x20] == 0xFD ||
                can_release_linked_robot_dog((struct Moby *)partner_moby)) {
                data->i23C = scale_game_frames((int)random_float_between(30.0f, 90.0f));
                moby[0x20] = 4;
                if (((unsigned char *)moby)[0x53] != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case ROBOT_DOG_TURN_TO_TARGET:
        initialization_multiplier_or_desired_heading = fast_arc_tan(
            selected_target.pos[0] - MOBY(moby)->pos.x, selected_target.pos[1] - MOBY(moby)->pos.y);
        FUN_L00_00258278(
            moby, data->v220, initialization_multiplier_or_desired_heading, 0.02f, 0.3f, 0.1f);
        if (((unsigned char *)moby)[0x70] & 2) {
            data->i23C = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby[0x20] = 4;
            if (((unsigned char *)moby)[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
        }
        break;
    case ROBOT_DOG_CHASE_TARGET:
        initialization_multiplier_or_desired_heading = fast_arc_tan(
            selected_target.pos[0] - MOBY(moby)->pos.x, selected_target.pos[1] - MOBY(moby)->pos.y);
        FUN_L00_00258278(
            moby, data->v220, initialization_multiplier_or_desired_heading, 0.05f, 0.3f, 0.2f);
        forward_vector_or_path_point[0] = 2.0f * fast_cos(MOBY(moby)->rot.z);
        forward_vector_or_path_point[1] = 2.0f * fast_sin(MOBY(moby)->rot.z);
        forward_vector_or_path_point[2] = 0.0f;
        FUN_L00_00258b50(moby, &data->i180, forward_vector_or_path_point, push, 1.0f);
        if (fast_dec_timer(&data->i23C)) {
            data->i23C = scale_game_frames((int)random_float_between(30.0f, 90.0f));
        }
        target_distance = vector_distance_xy(moby + 0x10, &selected_target);
        if (target_distance < data->f234 * frame_time * 0.5f * FUN_L04_001f1978(40.0f) +
                                  data->f234 * frame_time * FUN_L04_001f1978(10.0f) + 1.5f) {
            moby[0x20] = 6;
            if (((unsigned char *)moby)[0x53] != 5) {
                blend_moby_animation(moby, 5, 0, truncate_float_to_s32(FUN_L04_001f1978(10.0f)));
            }
        } else if (data->f230 < fast_vec_dist(&selected_target, data->home) ||
                   absolute_float(selected_target.pos[2] - MOBY(moby)->pos.z) > 3.0f) {
            moby[0x20] = 7;
        }
        break;
    case ROBOT_DOG_FOLLOW_PATH:
        path = (int *)l03_paths[data->i270];
        FUN_L00_0025df68(
            path, moby + 0x10, forward_vector_or_path_point, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
        if (data->i278 < seg) {
            data->i278 = seg;
        }
        if (FUN_L00_00259028(moby,
                             (char *)&data->i180,
                             (char *)path + (data->i278 * 0x10 + 0x10),
                             (char *)data->v40) &
            0x14) {
            if (data->i278 < *path - 1) {
                data->i278++;
            } else {
                data->i270 = -1;
                moby[0x20] = 1;
                if (((unsigned char *)moby)[0x53] != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
                if (data->i248 != -1) {
                    if (((unsigned char *)moby)[0x53] != 0) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                    }
                    moby[0x20] = 2;
                }
            }
        }
        if (random_integer_below(14) == 0) {
            partner_moby_index = data->i248;
            if (partner_moby_index == -1 ||
                (partner_moby = D_L03_0015FFD8_c5bb8 + (partner_moby_index << 8)) == 0 ||
                ((unsigned char *)partner_moby)[0x20] == 0xFE ||
                ((unsigned char *)partner_moby)[0x20] == 0xFD ||
                can_release_linked_robot_dog((struct Moby *)partner_moby)) {
                data->i270 = -1;
                data->i23C = scale_game_frames((int)random_float_between(30.0f, 90.0f));
                moby[0x20] = 4;
                if (((unsigned char *)moby)[0x53] != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case ROBOT_DOG_MELEE_ATTACK:
        initialization_multiplier_or_desired_heading = fast_arc_tan(
            selected_target.pos[0] - MOBY(moby)->pos.x, selected_target.pos[1] - MOBY(moby)->pos.y);
        FUN_L00_00258278(
            moby, data->v220, initialization_multiplier_or_desired_heading, 0.05f, 0.3f, 0.2f);
        forward_vector_or_path_point[0] = 2.0f * fast_cos(MOBY(moby)->rot.z);
        forward_vector_or_path_point[1] = 2.0f * fast_sin(MOBY(moby)->rot.z);
        forward_vector_or_path_point[2] = 0.0f;
        FUN_L00_00258b50(moby, &data->i180, forward_vector_or_path_point, data->v40, 1.0f);
        if (((unsigned char *)moby)[0x52] == 5) {
            approach_value(&data->f1A4, 0.0f, data->f234 * frame_time / FUN_L04_001f1978(40.0f));
        }
        if (absolute_float(compute_interpolated_record_value(moby) - 25.0f) <= 0.25f &&
            absolute_float(selected_target.pos[2] - MOBY(moby)->pos.z) < 1.0f &&
            fast_difference_between_rotations(
                fast_arc_tan(selected_target.pos[0] - MOBY(moby)->pos.x,
                             selected_target.pos[1] - MOBY(moby)->pos.y),
                MOBY(moby)->rot.z) < 0.2617994f &&
            selected_target.obj != 0 && vector_distance_xy(moby + 0x10, &selected_target) < 2.0f) {
            push[0] = fast_cos(MOBY(moby)->rot.z) * 0.2f;
            push[1] = fast_sin(MOBY(moby)->rot.z) * 0.2f;
            push[2] = 0.0f;
            qcopy(at, &selected_target);
            at[2] += 0.75f;
            spawn_nearby_effect(selected_target.obj, moby, 1, at, push, 1.0f);
        } else if (((unsigned char *)moby)[0x70] & 2) {
            data->f1A4 = data->f234 * frame_time;
            moby[0x20] = 7;
            if (((unsigned char *)moby)[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(3));
            }
            data->i244 = scale_game_frames(120);
        }
        break;
    case ROBOT_DOG_RETURN_HOME:
        initialization_multiplier_or_desired_heading =
            fast_arc_tan(data->home[0] - MOBY(moby)->pos.x, data->home[1] - MOBY(moby)->pos.y);
        FUN_L00_00258278(
            moby, data->v220, initialization_multiplier_or_desired_heading, 0.02f, 0.3f, 0.1f);
        forward_vector_or_path_point[0] = 2.0f * fast_cos(MOBY(moby)->rot.z);
        forward_vector_or_path_point[1] = 2.0f * fast_sin(MOBY(moby)->rot.z);
        forward_vector_or_path_point[2] = 0.0f;
        FUN_L00_00258b50(moby, &data->i180, forward_vector_or_path_point, push, 1.0f);
        if (vector_distance_xy(moby + 0x10, data->home) < 1.0f) {
            moby[0x20] = 1;
            if (((unsigned char *)moby)[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(3));
            }
        } else if (data->i244 == 0) {
            target_distance = vector_distance_xy(moby + 0x10, &selected_target);
            if (target_distance < data->f234 * frame_time * 0.5f * FUN_L04_001f1978(40.0f) +
                                      data->f234 * frame_time * FUN_L04_001f1978(10.0f) + 1.5f) {
                moby[0x20] = 6;
                if (((unsigned char *)moby)[0x53] != 5) {
                    blend_moby_animation(
                        moby, 5, 0, truncate_float_to_s32(FUN_L04_001f1978(10.0f)));
                }
            }
        }
        break;
    case ROBOT_DOG_FALL_RECOVERY:
        if (mine_fall(moby, data->m120) & 0x60) {
            moby[0x20] = 4;
            if (((unsigned char *)moby)[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
            break;
        }
        goto low;
    case ROBOT_DOG_WAIT_TO_REINITIALIZE:
        if (FUN_L00_002dc9f0(moby, data->m120)) {
            moby[0x20] = 0;
            data->sC8 = 0;
        }
        break;
    case ROBOT_DOG_DEATH_FALL:
        MOBY(moby)->flags &= 0xEFFF;
        if (mine_fall(moby, data->m120) & 0x60) {
            clear_u64_value(forward_vector_or_path_point);
            cart_burst(moby, moby + 0x10, -1, 0.5f, 13.0f);
            spawn_debris_c5bb8(moby,
                               0x6CE,
                               (float *)&MOBY(moby)->pos,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L03_0015F580_macro,
                               D_L03_0015F580_macro,
                               D_L03_0015F580_macro);
            spawn_debris_c5bb8(moby,
                               0x6CF,
                               (float *)&MOBY(moby)->pos,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L03_0015F580_macro,
                               D_L03_0015F580_macro,
                               D_L03_0015F580_macro);
            spawn_moby_break_fragments(moby, 0x77D, 1, 0x77D, 1, 3, 2);
            FUN_L00_00257470(moby, 0, -1);
            mark_moby_for_removal(moby);
            return;
        }
    low:
        if (MOBY(moby)->pos.z < 2.0f) {
            FUN_L00_00257470(moby, 0, -1);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    if (((unsigned char *)moby)[0x31] != 0 && fast_vec_dist(moby + 0x10, D_L03_00166EC0) < 32.0f) {
        FUN_L00_0025a120(moby);
    }
    trooper_anim_finish(moby, data->m110);
    ((unsigned char *)moby)[0xBC] = 0;
}
/* Applies damage reaction, knockback parameters and animation state. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C7DE8), where it is exact; names translated to the US level program. */

extern char *D_L03_0015FFD8_c __asm__("D_L03_0015FFD8");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void FUN_L01_0026e090(int, int);
extern void trooper_set_knockback(float *, float, float) __asm__("FUN_L03_00250a78");

void update_robot_dog_hit_reaction(struct Moby *moby) {
    float vector[4];
    int   result;
    float damage;
    float angle;
    char *data = (char *)moby->pvars;
    char *hit;
    if (*(int *)(data + 0x38) != 0 ||
        (*(int *)(data + 0x248) != -1 &&
         can_release_linked_robot_dog(
             (unsigned char *)(D_L03_0015FFD8_c + (*(int *)(data + 0x248) << 8)))))
        *(int *)(data + 0x240) = scale_game_frames(240);
    if (fast_dec_timer((int *)(data + 0x240)))
        *(float *)(data + 0x230) = *(float *)(data + 0x22C);
    else
        *(float *)(data + 0x230) = *(float *)(data + 0x22C) * 2.0f;
    damage = 0.0f;
    hit = find_moby_hit(moby, 0x330000, 0);
    hit_reaction(moby, hit, data + 0x20, 0, &result, &damage, 0, 4);
    if (result != 1 && moby->state != 21) {
        char *other;
        if (moby->group != 255)
            FUN_L01_0026e090(moby->group, 1);
        {
            float health = *(float *)(data + 0x20) - damage;
            float speed = frame_time_sq * 50.0f;
            *(int *)(data + 0x144) = 9;
            data[0x15D] = 0;
            *(float *)(data + 0x130) = speed;
            *(float *)(data + 0x20) = health;
        }
        other = *(char **)(hit + 0x20);
        angle = fast_arc_tan(moby->pos.x - *(float *)(other + 0x10),
                             moby->pos.y - *(float *)(other + 0x14));
        {
            float rotationSpeed = frame_time * 10.0f;
            moby->flags &= 0xEFFF;
            *(float *)(data + 0x13C) = rotationSpeed;
        }
        trooper_set_knockback((float *)(data + 0x120), 4.5f, 2.0f);
        *(float *)(data + 0x170) = 5.0f;
        *(float *)(data + 0x174) = 12.0f;
        *(OvlQuad *)vector = *(OvlQuad *)(hit + 0x10);
        trooper_knockback_arc(vector, &angle, data + 0x138, data + 0x13C);
        trooper_start_knockback(angle, moby, data + 0x120, 4, 1, 0);
        moby->state = 21;
        *(unsigned char *)(data + 0x117) = 240;
        trooper_anim_update(moby, (short *)(data + 0x110));
    }
    moby->hit_slot = 255;
}
/* scans class instances for either predicate */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C8160), where it is exact; names translated to the US level program. */

extern int is_moby_class_23e_in_state_9(unsigned char *) __asm__("FUN_L03_002c6d58");

int is_point_inside_clip_volume_cf(int id) __asm__("FUN_L03_002c6d98");

int is_point_inside_clip_volume_cf(int id) {
    unsigned char *item;
    if (id == 0xFF)
        goto absent;
    get_first_moby_in_group((int *)&item, id, 0, 0);
    while (item != 0) {
        if (is_moby_class_23e_in_state_9(item))
            return 1;
        if (is_moby_class_23d_in_state_5(item))
            return 1;
        get_next_moby_in_group((int *)&item, (int)item, 0, 0);
    }
absent:
    return 0;
}
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C81E0), where it is exact; names translated to the US level program. */

extern short D_L03_0015FFD8_u __asm__("D_L03_0015FFD8");

int try_start_kerwan_path_enemy_heading_turn(float        desired_heading,
                                             struct Moby *moby,
                                             struct Moby *target) __asm__("FUN_L03_002c6e18");

int try_start_kerwan_path_enemy_heading_turn(float        desired_heading,
                                             struct Moby *moby,

                                             struct Moby *target) {
    char         *data = (char *)moby->pvars;
    int           linked_moby_index;
    float         heading_delta;
    unsigned char previous_state;
    if (*(int *)&target->rot != 0) {
        linked_moby_index = *(int *)(data + 0x264);
        if (linked_moby_index != -1) {
            char *linked_moby = (char *)(linked_moby_index << 8) + (int)*(char **)&D_L03_0015FFD8_u;
            if (MOBY(linked_moby)->oclass == 0x336) {
                if ((unsigned char)linked_moby[0xBC] != 4)
                    return 0;
            }
        }
        heading_delta = fast_subtract_rotations(desired_heading, moby->rot.z);
        if (absolute_float(heading_delta) >= 0.7853982f && *(unsigned char *)(data + 0x263) == 0 &&
            moby->prev_seq != 1) {
            goto go;
        }
        if (absolute_float(heading_delta) > 1.5707964f) {
        go:
            if (vector_distance_xy(&moby->pos, target) < 20.0f) {
                if (absolute_float(heading_delta) > 1.1780972f) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                    moby->anim_speed = 1.0f;
                } else {
                    blend_moby_animation(moby, 9, 0, scale_game_frames(10));
                }
                *(float *)(data + 0x28C) = desired_heading;
                previous_state = moby->state;
                moby->state = 0xC;
                moby->command = previous_state;
                return 1;
            }
        }
    }
    return 0;
}

#include "qcopy.h"

/* Update for moby class 574 on Kerwan: a path-walking enemy that turns to, chases and attacks its target, jumps, falls under gravity and is replaced by debris when it dies. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C8398), where it is exact; names translated to the US level program. */

typedef struct {
    float pos[4];
    float v10[4];
    float dir[4];
    float v30[4];
    void *obj;
    int   pad[3];
} Tgt2C8398;

typedef struct {
    float          v[4];
    void          *m;
    int            flags;
    unsigned char  b0;
    unsigned char  b1;
    unsigned short id;
    float          f;
} Hit2C8398;

typedef struct {
    char          p0[8];
    float         f8;
    char          pC[4];
    float         f10;
    float         f14;
    float         f18;
    float         f1C;
    int           i20;
    int           i24;
    float         f28;
    char          p2C[0x11];
    unsigned char b3D;
    short         s3E;
    char          p40[0xC];
    float         f4C;
    float         f50;
    float         f54;
    char          p58[8];
} Jump2C8398;

typedef struct {
    char  p0[8];
    float f8;
    float fC;
    char  p10[8];
    float f18;
    char  p1C[8];
    float f24;
    float f28;
    float f2C;
    char  p30[8];
    int   i38;
    float f3C;
    float f40;
    float f44;
    char  p48[8];
} Mover2C8398;

typedef struct {
    int         idx;
    signed char step;
    char        p5[0xB];
    char       *path;
    int         id;
} PathF2C8398;

typedef struct {
    char  p0[0x68];
    float f68;
    char  p6C[0x14];
} Look2C8398;

typedef struct Data2C8398 {
    char          p00[0x20];
    float         f20;
    short         s24;
    char          p26[2];
    unsigned char b28;
    unsigned char b29;
    char          p2A[6];
    float         f30;
    char          p34[4];
    int           i38;
    char          p3C[4];
    float         v40[4];
    char          p50[8];
    unsigned char b58;
    char          p59;
    unsigned char b5A;
    char          p5B[0xC];
    unsigned char b67;
    char          p68[8];
    Jump2C8398    q;
    Mover2C8398   mv;
    PathF2C8398   pf;
    char          p138[0x18];
    Look2C8398    look0;
    Look2C8398    look1;
    short         s250;
    short         s252;
    unsigned char b254;
    unsigned char b255;
    short         s256;
    float         f258;
    char          p25C[6];
    unsigned char b262;
    unsigned char b263;
    int           i264;
    char          p268[4];
    float         f26C;
    float         f270;
    int           i274;
    float         f278;
    float         f27C;
    int           i280;
    int           i284;
    short         s288;
    short         s28A;
    float         f28C;
    int           i290;
    float         f294;
    int           i298;
    int           i29C;
} Data2C8398;

typedef struct Moby2C8398 {
    char           p00[0x10];
    float          pos[4];
    unsigned char  state;
    unsigned char  b21;
    char           p22[2];
    char          *p24;
    char           p28[9];
    unsigned char  b31;
    unsigned short s32;
    unsigned short flags;
    char           p36[2];
    long           l38;
    float          rot[4];
    char           p50[2];
    unsigned char  b52;
    unsigned char  anim;
    char           p54[4];
    float          f58;
    char           p5C[0x14];
    unsigned char  b70;
    char           p71[7];
    Data2C8398    *data;
    char           p7C[3];
    unsigned char  b7F;
    char           p80[0x14];
    int            i94;
    char           p98[0xC];
    unsigned char  bA4;
    char           pA5;
    unsigned short cls;
    char           pA8[8];
    unsigned char  bB0;
    char           pB1[0xB];
    unsigned char  bBC;
    char           pBD[0x43];
} Moby2C8398;

extern CollisionHit D_L03_00173F40;
extern char         sphere_collision_resolved_position[] __asm__("D_L03_00173F70");
extern char         D_L03_00173F80[];
extern float        allocate_voice_for_target_entry_cf(void *) __asm__("FUN_L00_00259710");
extern float        truncate_float_to_s32_cf(float) __asm__("FUN_001f96b0");
extern int          collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int          D_L03_0015F5C4;
extern int          D_L03_0015F5CC;
extern Moby2C8398  *D_L03_0015FFD8_m __asm__("D_L03_0015FFD8");
int                 D_L03_001618F4_i __asm__("D_L03_001618F4") = 0;
extern int          coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int          is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern int FUN_L00_0025df68_cf(char *m, char *p, float *pos, char *v) __asm__("FUN_L00_00259028");
extern int fast_subtract_rotations_cf(void *) __asm__("FUN_L00_001fefc8");
extern int is_point_inside_clip_volume_cf(int) __asm__("FUN_L03_002c6d98");
extern int probe_ground_height_cf(int unused, char *m, char *a, void *b, char *c, void *d) __asm__("FUN_L00_00260400");
extern short          D_L03_001618F4 __attribute__((sda));
extern unsigned char *FUN_L03_0024fba8_cf(int) __asm__("FUN_L00_002603d0");
extern void          *FUN_L00_00257470_cf(char *,
                                          int,
                                          float *,
                                          void *,
                                          int,
                                          int,
                                          float,
                                          float *,
                                          float *,
                                          float *) __asm__("FUN_L00_00263fd8");
extern void           FUN_001f96b0_cf(char *p) __asm__("FUN_L01_0026d930");
extern void           FUN_001f9c48_cf(
              char *moby, float *vel, float target, float k, float d, float max) __asm__("FUN_L00_00258278");
extern void FUN_L00_001fefc8_cf(void *) __asm__("FUN_L03_002c95a8");
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void FUN_L00_00259710_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void FUN_L00_0025c698_cf(
    void *, void *, void *, int *, float *, int, float, float, float) __asm__("FUN_L00_0025df68");
extern void select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");
extern void FUN_L00_00260400_cf(void *) __asm__("FUN_L00_0025a120");
extern void manipulator_set_scale(void *, float) __asm__("FUN_L00_00263b38");
extern void spawn_explosion(void *,
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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern void FUN_L03_00250ae8_cf(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void emit_radial_particle_burst(void *, void *, int) __asm__("FUN_L03_00250ae8");
extern void compute_interpolated_record_value_cf(
    void *, void *, void *, int, float, float, float) __asm__("FUN_L03_002d4288");
extern void is_value_within_interpolated_window_cf(void *) __asm__("FUN_L03_002c9ca0");
extern void mark_moby_for_removal_cf(void *, void *, float) __asm__("FUN_L03_0024fba8");

void update_kerwan_path_enemy(Moby2C8398 *m) __asm__("FUN_L03_002c6fd0");

void update_kerwan_path_enemy(Moby2C8398 *m) {
    enum {
        ENEMY_INITIALIZE = 0,
        ENEMY_WAIT_FOR_ACTIVATION = 1,
        ENEMY_WAIT_FOR_LINKED_MOBY = 2,
        ENEMY_PROJECTILE_ATTACK_WINDUP = 4,
        ENEMY_PROJECTILE_ATTACK = 5,
        ENEMY_MELEE_ATTACK = 6,
        ENEMY_MOVE_TO_PATH_NODE = 7,
        ENEMY_FOLLOW_PATH_TO_END = 8,
        ENEMY_FOLLOW_ALTERNATE_PATH = 9,
        ENEMY_KNOCKBACK_RECOVERY = 10,
        ENEMY_FALL_UNDER_GRAVITY = 11,
        ENEMY_TURN_TO_HEADING = 12,
        ENEMY_DEATH_KNOCKBACK = 13,
        ENEMY_HIDDEN_AT_PATH_START = 14,
        ENEMY_RESPAWN_AT_PATH_START = 15
    };
    enum {
        ENEMY_ANIM_LOCOMOTION = 1,
        ENEMY_ANIM_HEADING_TURN = 9
    };
    Tgt2C8398   selected_target;
    Data2C8398 *moby_data = m->data;
    float       target_distance;

    select_moby_combat_target(m, &selected_target, moby_data->f270);
    target_distance = vector_distance_xy(m->pos, &selected_target);
    manipulator_set_scale(&moby_data->look0, 2.5f);
    if (moby_data->b262 == 1) {
        if ((visited52)[m->bB0 + (current_level_index << 4)] == 0xFF) {
            m->flags |= 0x41;
            m->i94 = 0;
        }
        return;
    }
    if (D_L03_0015F5C4 == 2) {
        m->flags |= 0x41;
    } else if (m->state != 0xE) {
        m->flags &= 0xFFBE;
    }
    if (m->pos[0] < 2.0f || 1021.0f < m->pos[0] || m->pos[1] < 2.0f || 1021.0f < m->pos[1] ||
        m->pos[2] < 2.0f || 1021.0f < m->pos[2] || (moby_data->pf.id == -1 && m->state != 0xD)) {
        mark_moby_for_removal(m);
        return;
    }
    FUN_L00_001fefc8_cf(m);
    fast_subtract_rotations_cf(&moby_data->b263);
    if (m->b31 != 0 || target_distance < 24.0f) {
        if (m->state == 0xC) {
            moby_data->look0.f68 = fast_subtract_rotations(moby_data->f28C, m->rot[2]) * 0.25f;
            moby_data->look1.f68 = fast_subtract_rotations(moby_data->f28C, m->rot[2]) * 0.75f;
        } else if (m->state < 7 && target_distance < moby_data->f270) {
            moby_data->look1.f68 =
                fast_subtract_rotations(fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                                     selected_target.pos[1] - m->pos[1]),
                                        m->rot[2]);
        }
        if (moby_data->look0.f68 > 0.7853982f) {
            moby_data->look0.f68 = 0.7853982f;
        } else if (moby_data->look0.f68 < -0.7853982f) {
            moby_data->look0.f68 = -0.7853982f;
        }
        if (moby_data->look1.f68 > 0.7853982f) {
            moby_data->look1.f68 = 0.7853982f;
        } else if (moby_data->look1.f68 < -0.7853982f) {
            moby_data->look1.f68 = -0.7853982f;
        }
        update_joint_animation(
            (char *)m, (char *)&moby_data->look0, 2, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
        update_joint_animation(
            (char *)m, (char *)&moby_data->look1, 3, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
    }
    switch (m->state) {
    case ENEMY_INITIALIZE:
        m->l38 = (((Moby2C8398 *)hero.moby))->l38;
        m->b7F = 0x1C;
        FUN_001f96b0_cf((char *)&moby_data->mv);
        moby_data->mv.fC = 3.0f;
        moby_data->mv.f18 = 0.0f;
        moby_data->mv.f3C = frame_time_sq * 6.2831855f;
        moby_data->mv.f40 = frame_time_sq * 12.566371f;
        moby_data->mv.f44 = frame_time * 3.1415927f;
        moby_data->mv.f24 = frame_time * 6.0f;
        moby_data->mv.f8 = 2.0f;
        moby_data->mv.f28 = 3.1415927f;
        moby_data->mv.f2C = moby_data->mv.f24 * truncate_float_to_s32_cf(15.0f);
        moby_data->mv.i38 = 9;
        moby_data->f20 = 3.0f;
        moby_data->s24 = truncate_float_to_s32(3.0f);
        moby_data->b29 = 1;
        moby_data->b28 = 2;
        moby_data->f30 = 1.75f;
        moby_data->q.f28 = 0.7f;
        moby_data->q.i20 = truncate_float_to_s32(716.8f);
        moby_data->q.f4C = 0.0f;
        moby_data->b58 = 0xA;
        moby_data->b5A = 0xC;
        clear_u64_value(moby_data->v40);
        m->flags |= 0x1000;
        moby_data->s250 = 0;
        {
            char *np = ((char *)l03_paths[moby_data->pf.id]);
            moby_data->pf.idx = 0;
            moby_data->pf.path = np;
            qcopy(m->pos, np + 0x10);
        }
        m->state = 1;
        moby_data->s288 = m->s32;
        moby_data->s28A = D_L03_001618F4 & 7;
        D_L03_001618F4_i++;
        moby_data->b263 = 0;
        {
            char *path = moby_data->pf.path;
            if (path != 0) {
                int i;
                for (i = 0; i < *(int *)path - 2; i++) {
                    *(float *)(path + 0x1C + i * 0x10) =
                        fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
                }
            }
        }
        if (moby_data->i284 != -1 && moby_data->i290 == -1) {
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
            m->state = 2;
        }
        break;
    case ENEMY_WAIT_FOR_ACTIVATION:
        if ((m->b70 & 2) && m->b52 != 0) {
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
        }
        if (moby_data->i290 != -1) {
            char *np = ((char *)l03_paths[moby_data->i290]);
            *(OvlQuad *)m->pos = *(OvlQuad *)(np + 0x10);
            m->rot[2] = fast_arc_tan(*(float *)(np + 0x20) - *(float *)(np + 0x10),
                                     *(float *)(np + 0x24) - *(float *)(np + 0x14));
            if (is_point_inside_clip_volume_cf(m->b21) ||
                is_point_inside_clip_volume(&selected_target, moby_data->i29C) ||
                (visited52)[m->bB0 + (current_level_index << 4)] == 0xFF ||
                (target_distance < moby_data->f294 &&
                 absolute_float(selected_target.pos[2] - m->pos[2]) < 4.0f)) {
                moby_data->i298 = 0;
                m->state = 9;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        } else if (moby_data->i284 != -1) {
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
            m->state = 2;
        } else {
            if (target_distance < moby_data->f270 || target_distance < moby_data->f27C) {
                if (try_start_kerwan_path_enemy_heading_turn(
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1]),
                        (unsigned char *)m,
                        (char *)&selected_target)) {
                    break;
                }
            }
            if (target_distance < moby_data->f27C &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 4.0f) {
                moby_data->f27C = 0.0f;
                moby_data->s256 = 1;
                m->state = 8;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
            if (target_distance < 3.5f &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 3.0f) {
                m->state = 6;
                if (m->anim != 2) {
                    blend_moby_animation(m, 2, 0, scale_game_frames(10));
                }
            } else if (target_distance < moby_data->f270 &&
                       absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f &&
                       fast_dec_timer_s16(&moby_data->s250)) {
                if (fast_difference_between_rotations(
                        m->rot[2],
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1])) < 0.7853982f) {
                    if (m->b31 != 0) {
                        m->state = 4;
                        if (m->anim != 3) {
                            blend_moby_animation(m, 3, 0, scale_game_frames(10));
                        }
                    }
                }
            }
        }
        break;
    case ENEMY_WAIT_FOR_LINKED_MOBY:
        if (target_distance < moby_data->f270 &&
            absolute_float(selected_target.pos[2] - m->pos[2]) < 4.0f && m->anim != 0xA) {
            blend_moby_animation(m, 0xA, 0, scale_game_frames(10));
            break;
        }
        {
            Moby2C8398 *o = &D_L03_0015FFD8_m[moby_data->i284];
            if (o == 0 || o->state == 0xFE || o->state == 0xFD || (m->b52 == 0xA && (m->b70 & 2))) {
                m->state = 1;
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
                moby_data->s250 = scale_game_frames(0x3C);
                moby_data->i284 = -1;
            } else {
                try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target);
            }
        }
        break;
    case ENEMY_PROJECTILE_ATTACK_WINDUP:
        if (m->b70 & 2) {
            m->state = 5;
            moby_data->b254 = 0;
            if (m->anim != 4) {
                blend_moby_animation(m, 4, 0, 0);
            }
        } else if (target_distance < 3.5f &&
                   absolute_float(selected_target.pos[2] - m->pos[2]) < 3.0f) {
            m->state = 6;
            if (m->anim != 2) {
                blend_moby_animation(m, 2, 0, scale_game_frames(10));
            }
        } else if (target_distance < moby_data->f278 &&
                   absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f) {
            if (moby_data->i274 != 0) {
                Data2C8398 *d2 = m->data;
                if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                    d2->pf.idx++;
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                }
            } else {
                is_value_within_interpolated_window_cf(m);
                m->state = 7;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case ENEMY_PROJECTILE_ATTACK: {
        if (is_value_within_interpolated_window((char *)m, 8.0f)) {
            float a[4];
            float b[4];
            float scaled_launch_vector[4];
            float launch_endpoint[4];
            float ang;
            float df;
            moby_data->b254++;
            moby_point_position(m, 0, a);
            fast_vec_sub(b, selected_target.dir, a);
            fast_vec_normalize(b, b, frame_time * 6.0f);
            b[2] *= 0.5f;
            ang = fast_arc_tan(b[0], b[1]);
            df = fast_subtract_rotations(ang, m->rot[2]);
            if (df > 0.5235988f) {
                ang = fast_add_rotations(m->rot[2], 0.5235988f);
                b[0] = fast_cos(ang) * (frame_time * 6.0f);
                b[1] = fast_sin(ang) * (frame_time * 6.0f);
                b[2] = 0.0f;
            } else if (df < -0.5235988f) {
                ang = fast_add_rotations(m->rot[2], -0.5235988f);
                b[0] = fast_cos(ang) * (frame_time * 6.0f);
                b[1] = fast_sin(ang) * (frame_time * 6.0f);
                b[2] = 0.0f;
            }
            fast_vec_scale(scaled_launch_vector, b, truncate_float_to_s32_cf(10.0f));
            FUN_L03_00250ae8_cf(launch_endpoint, scaled_launch_vector, a);
            emit_radial_particle_burst(m, scaled_launch_vector, 0);
            compute_interpolated_record_value_cf(
                b, a, m, scale_game_frames(0x82), ang, frame_time * 6.0f, 0.75f);
        }
        if (m->b70 & 2) {
            if (moby_data->b254 == 0) {
                break;
            }
            if (target_distance < moby_data->f27C &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 4.0f) {
                moby_data->f27C = 0.0f;
                m->state = 8;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
            if (try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target)) {
                break;
            }
            if (target_distance < moby_data->f270 &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f &&
                fast_dec_timer_s16(&moby_data->s250) &&
                fast_difference_between_rotations(
                    m->rot[2],
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1])) < 0.08726646f &&
                m->b31 != 0) {
                break;
            }
            if (target_distance < 3.5f &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 3.0f) {
                m->state = 6;
                if (m->anim != 2) {
                    blend_moby_animation(m, 2, 0, scale_game_frames(10));
                }
            }
        } else if (target_distance < 3.5f &&
                   absolute_float(selected_target.pos[2] - m->pos[2]) < 3.0f) {
            if (try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target)) {
                break;
            }
            m->state = 6;
            if (m->anim != 2) {
                blend_moby_animation(m, 2, 0, scale_game_frames(10));
            }
        } else if (target_distance < moby_data->f278 &&
                   absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f) {
            if (moby_data->i274 != 0) {
                Data2C8398 *d2 = m->data;
                if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                    d2->pf.idx++;
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                }
            } else {
                is_value_within_interpolated_window_cf(m);
                m->state = 7;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        }
        break;
    }
    case ENEMY_MELEE_ATTACK: {
        {
            float     v[4];
            Hit2C8398 hit;
            float     fr;
            float     ang;
            fr = compute_interpolated_record_value(m);
            ang = fast_arc_tan(selected_target.pos[0] - m->pos[0],
                               selected_target.pos[1] - m->pos[1]);
            FUN_001f9c48_cf((char *)m, &moby_data->f258, ang, 0.03f, 0.3f, 0.3f);
            moby_point_position(m, 0, v);
            if (25.0f <= fr && fr <= 31.0f &&
                absolute_float(m->pos[2] - selected_target.pos[2]) < 2.0f &&
                vector_distance_xy(m->pos, &selected_target) < 3.5f &&
                fast_difference_between_rotations(fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                                               selected_target.pos[1] - m->pos[1]),
                                                  m->rot[2]) < 0.2617994f) {
                hit.m = m;
                hit.flags = 0x10001;
                hit.f = 1.0f;
                hit.v[0] = fast_cos(m->rot[2]) * 0.2f;
                hit.v[1] = fast_sin(m->rot[2]) * 0.2f;
                hit.v[2] = 0.0f;
                set_vector_length_xy(hit.v, hit.v, 1.0f);
                hit.v[2] = 1.0f;
                hit.v[3] = 5627.925f;
                hit.b0 = 0;
                hit.b1 = 1;
                hit.id = m->cls;
                if (selected_target.obj != 0) {
                    record_moby_hit(selected_target.obj, &hit);
                }
            } else if (m->b70 & 2) {
                if (moby_data->i274 != 0) {
                    Data2C8398 *d2 = m->data;
                    if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                        d2->pf.idx++;
                        m->state = 7;
                        if (m->anim != 1) {
                            blend_moby_animation(m, 1, 0, scale_game_frames(10));
                        }
                    } else {
                        m->state = 1;
                        if (m->anim != 0) {
                            blend_moby_animation(m, 0, 0, scale_game_frames(10));
                        }
                        try_start_kerwan_path_enemy_heading_turn(
                            fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                         selected_target.pos[1] - m->pos[1]),
                            (unsigned char *)m,
                            (char *)&selected_target);
                    }
                } else {
                    is_value_within_interpolated_window_cf(m);
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                }
            }
        }
        break;
    }
    case ENEMY_MOVE_TO_PATH_NODE: {
        PathF2C8398 *pf = &moby_data->pf;
        if (moby_data->s256 != 0) {
            m->state = 8;
            break;
        }
        {
            int path_node_byte_offset = pf->idx * 0x10;
            if (try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(*(float *)(pf->path + path_node_byte_offset + 0x10) - m->pos[0],
                                 *(float *)(pf->path + path_node_byte_offset + 0x14) - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target)) {
                break;
            }
        }
        if (!(FUN_L00_0025df68_cf((char *)m,
                                  (char *)&moby_data->mv,
                                  (float *)(pf->path + 0x10 + pf->idx * 0x10),
                                  (char *)moby_data->v40) &
              0x14)) {
            break;
        }
        if (target_distance < moby_data->f278 &&
            absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f) {
            if (moby_data->i274 != 0) {
                Data2C8398 *d2 = m->data;
                if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                    d2->pf.idx++;
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                } else {
                    m->state = 1;
                    if (m->anim != 0) {
                        blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    }
                    try_start_kerwan_path_enemy_heading_turn(
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1]),
                        (unsigned char *)m,
                        (char *)&selected_target);
                }
            } else {
                is_value_within_interpolated_window_cf(m);
                m->state = 7;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        } else {
            m->state = 1;
            if (m->anim != 0) {
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
            }
            try_start_kerwan_path_enemy_heading_turn(
                fast_arc_tan(selected_target.pos[0] - m->pos[0],
                             selected_target.pos[1] - m->pos[1]),
                (unsigned char *)m,
                (char *)&selected_target);
        }
        break;
    }
    case ENEMY_FOLLOW_PATH_TO_END: {
        {
            PathF2C8398 *pf = &moby_data->pf;
            float        v[4];
            int          seg;
            float        frac;
            FUN_L00_0025c698_cf(pf->path, m->pos, v, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
            if (pf->idx < seg) {
                pf->idx = seg;
            }
            if (!(FUN_L00_0025df68_cf((char *)m,
                                      (char *)&moby_data->mv,
                                      (float *)(pf->path + 0x10 + pf->idx * 0x10),
                                      (char *)moby_data->v40) &
                  0x14)) {
                break;
            }
            if (pf->idx < *(int *)pf->path - 1) {
                pf->idx++;
            } else {
                m->state = 1;
                if (m->anim != 0) {
                    blend_moby_animation(m, 0, 0, scale_game_frames(10));
                }
                try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target);
            }
        }
        break;
    }
    case ENEMY_FOLLOW_ALTERNATE_PATH: {
        {
            char *path = ((char *)l03_paths[moby_data->i290]);
            float v[4];
            int   seg;
            float frac;
            FUN_L00_0025c698_cf(path, m->pos, v, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
            if (moby_data->i298 < seg) {
                moby_data->i298 = seg;
            }
            if (!(FUN_L00_0025df68_cf((char *)m,
                                      (char *)&moby_data->mv,
                                      (float *)(path + 0x10 + moby_data->i298 * 0x10),
                                      (char *)moby_data->v40) &
                  0x14)) {
                break;
            }
            if (moby_data->i298 < *(int *)path - 1) {
                moby_data->i298++;
            } else {
                moby_data->i290 = -1;
                m->state = 1;
                if (m->anim != 0) {
                    blend_moby_animation(m, 0, 0, scale_game_frames(10));
                }
                if (moby_data->i284 != -1) {
                    blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    m->state = 2;
                } else {
                    try_start_kerwan_path_enemy_heading_turn(
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1]),
                        (unsigned char *)m,
                        (char *)&selected_target);
                }
            }
        }
        break;
    }
    case ENEMY_KNOCKBACK_RECOVERY: {
        int   fl;
        float g;
        if (moby_data->q.s3E != 2) {
            fl = mine_fall(m, &moby_data->q);
        } else {
            m->f58 = 1.0f;
            fl = (m->b70 << 5) & 0x40;
        }
        moby_data->i290 = -1;
        if (fl & 0x40) {
            if (moby_data->i274 != 0) {
                Data2C8398 *d2 = m->data;
                if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                    d2->pf.idx++;
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                } else {
                    m->state = 1;
                    if (m->anim != 0) {
                        blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    }
                    try_start_kerwan_path_enemy_heading_turn(
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1]),
                        (unsigned char *)m,
                        (char *)&selected_target);
                }
            } else {
                is_value_within_interpolated_window_cf(m);
                m->state = 7;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        } else if (m->pos[2] < 5.0f || (fl & 0x20)) {
            {
                float v[4];
                qcopy(v, m->pos);
                v[2] += 1.0f;
                spawn_explosion(m,
                                moby_data->v40,
                                v,
                                0.0f,
                                0.0f,
                                5,
                                2,
                                4,
                                2.0f,
                                1.0f,
                                9.0f,
                                6,
                                1.0f,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D0,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D1,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D2,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00259710_cf(m, 0, -1);
                if (moby_data->i264 == -1) {
                    mark_moby_for_removal(m);
                    return;
                }
                *(OvlQuad *)m->pos = *(OvlQuad *)(moby_data->pf.path + 0x10);
                clear_u64_value(moby_data->v40);
                moby_data->mv.f18 = 0.0f;
                m->state = 0xE;
                m->flags |= 0x41;
                m->i94 = 0;
                return;
            }
        }
        m->pos[2] = m->pos[2] + 2.0f;
        g = allocate_voice_for_target_entry_cf(m->pos);
        m->pos[2] = m->pos[2] - 2.0f;
        if (g + 5.0f < m->pos[2]) {
            allocate_voice_for_target_entry(0xA, 0x21, (int)m);
            blend_moby_animation(m, 0xB, 0, scale_game_frames(10));
            m->state = 0xB;
        }
        break;
    }
    case ENEMY_FALL_UNDER_GRAVITY: {
        {
            Jump2C8398 *q = &moby_data->q;
            float       a[4];
            float       b[4];
            float       speed;
            float       want;
            q->f8 = q->f8 - q->f10;
            speed = vector_length_xy(q);
            want = 0.0f;
            if (q->s3E != 2) {
                want = q->f4C;
            }
            approach_value(&speed, want, q->f14);
            set_vector_length_xy(q, q, speed);
            qcopy(a, m->pos);
            a[2] += q->f28;
            fast_vec_add(m->pos, m->pos, q);
            qcopy(b, m->pos);
            b[2] += q->f28;
            if (fast_vec_length(q) > (float)q->i20) {
                collision_line(a, b, 0x24, m, 0);
                qcopy(b, &D_L03_00173F40.point);
            }
            if (coll_sphere(convert_integer_to_float(q->i20) * 0.0009765625f, b, 0x24, m)) {
                qcopy(m->pos, sphere_collision_resolved_position);
                m->pos[2] = a[2] + q->f8 - q->f28;
            }
            if (m->pos[2] < 5.0f) {
                {
                    float v[4];
                    qcopy(v, m->pos);
                    v[2] += 1.0f;
                    spawn_explosion(m,
                                    moby_data->v40,
                                    v,
                                    0.0f,
                                    0.0f,
                                    5,
                                    2,
                                    4,
                                    2.0f,
                                    1.0f,
                                    9.0f,
                                    6,
                                    1.0f,
                                    15.0f,
                                    1,
                                    1,
                                    -1,
                                    0);
                    FUN_L00_00257470_cf((char *)m,
                                        0x6D0,
                                        m->pos,
                                        m->rot,
                                        0,
                                        0,
                                        0.0f,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro);
                    FUN_L00_00257470_cf((char *)m,
                                        0x6D1,
                                        m->pos,
                                        m->rot,
                                        0,
                                        0,
                                        0.0f,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro);
                    FUN_L00_00257470_cf((char *)m,
                                        0x6D2,
                                        m->pos,
                                        m->rot,
                                        0,
                                        0,
                                        0.0f,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro);
                    FUN_L00_00259710_cf(m, 0x200, -1);
                    if (moby_data->i264 == -1) {
                        mark_moby_for_removal(m);
                        return;
                    }
                    *(OvlQuad *)m->pos = *(OvlQuad *)(moby_data->pf.path + 0x10);
                    clear_u64_value(moby_data->v40);
                    moby_data->mv.f18 = 0.0f;
                    m->state = 0xE;
                    m->flags |= 0x41;
                    m->i94 = 0;
                    return;
                }
            }
        }
    }
    case ENEMY_TURN_TO_HEADING:
        switch (m->anim) {
        case ENEMY_ANIM_LOCOMOTION:
            FUN_001f9c48_cf((char *)m, &moby_data->f258, moby_data->f28C, 0.09f, 0.27f, 0.15f);
            if (fast_difference_between_rotations(m->rot[2], moby_data->f28C) < 0.034906585f) {
                moby_data->b263 = scale_game_frames(0x3C);
                if (m->bBC >= 7 && m->bBC <= 8) {
                    if (m->bBC == 7) {
                        is_value_within_interpolated_window_cf(m);
                    }
                    m->state = m->bBC;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                    m->f58 = 1.0f;
                } else {
                    m->state = 1;
                    if (m->anim != 0) {
                        blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    }
                    moby_data->s250 = scale_game_frames(10);
                }
            }
            break;
        case ENEMY_ANIM_HEADING_TURN:
            FUN_001f9c48_cf((char *)m, &moby_data->f258, moby_data->f28C, 0.01f, 0.3f, 0.2f);
            if (fast_difference_between_rotations(m->rot[2], moby_data->f28C) < 0.034906585f) {
                moby_data->b263 = scale_game_frames(0x3C);
                if (m->bBC >= 7 && m->bBC <= 8) {
                    if (m->bBC == 7) {
                        is_value_within_interpolated_window_cf(m);
                    }
                    m->state = m->bBC;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                } else {
                    m->state = 1;
                    if (m->anim != 0) {
                        blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    }
                    moby_data->s250 = scale_game_frames(10);
                }
            }
            break;
        }
        break;
    case ENEMY_DEATH_KNOCKBACK: {
        int   fl = mine_fall(m, &moby_data->q);
        float g;
        moby_data->b255++;
        g = allocate_voice_for_target_entry_cf(m->pos);
        if ((fl & 0x60) && g + 5.0f < m->pos[2] && m->bBC != 0xB) {
            allocate_voice_for_target_entry(0xA, 0, (int)m);
            m->bBC = 0xB;
        }
        m->flags &= 0xEFFF;
        if (fl & 1) {
            moby_data->b255 = 0;
        }
        if (fl & 0x60) {
            {
                float v[4];
                qcopy(v, m->pos);
                v[2] += 1.0f;
                spawn_explosion(m,
                                moby_data->v40,
                                v,
                                0.0f,
                                0.0f,
                                5,
                                2,
                                4,
                                2.0f,
                                1.0f,
                                9.0f,
                                6,
                                1.0f,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D0,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D1,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D2,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00259710_cf(m, 0, -1);
                if (moby_data->i264 == -1) {
                    mark_moby_for_removal(m);
                    return;
                }
                *(OvlQuad *)m->pos = *(OvlQuad *)(moby_data->pf.path + 0x10);
                clear_u64_value(moby_data->v40);
                moby_data->mv.f18 = 0.0f;
                m->state = 0xE;
                m->flags |= 0x41;
                m->i94 = 0;
                return;
            }
        }
        if (m->pos[2] < 5.0f) {
            FUN_L00_00259710_cf(m, 0, -1);
            if (moby_data->i264 == -1) {
                mark_moby_for_removal(m);
                return;
            }
            *(OvlQuad *)m->pos = *(OvlQuad *)(moby_data->pf.path + 0x10);
            clear_u64_value(moby_data->v40);
            moby_data->mv.f18 = 0.0f;
            m->state = 0xE;
            m->flags |= 0x41;
            m->i94 = 0;
            return;
        }
        break;
    }
    case ENEMY_HIDDEN_AT_PATH_START: {
        PathF2C8398 *pf = &moby_data->pf;
        if (m->anim != 0) {
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
        }
        m->flags &= 0xEFFF;
        *(OvlQuad *)m->pos = *(OvlQuad *)(pf->path + 0x10);
        m->flags |= 0x41;
        m->i94 = 0;
        break;
    }
    case ENEMY_RESPAWN_AT_PATH_START:
        if ((visited52)[m->bB0 + (current_level_index << 4)] == 0xFF) {
            mark_moby_for_removal(m);
            return;
        }
        m->i94 = *(int *)(m->p24 + 0x10);
        m->flags = (m->flags & 0xFFBE) | 0x1000;
        moby_data->f20 = convert_integer_to_float(moby_data->s24);
        qcopy(m->pos, moby_data->pf.path + 0x10);
        m->state = 1;
        break;
    }
    if (m->b31 != 0) {
        if (fast_vec_dist(m->pos, D_L03_00166EC0) < 36.0f) {
            FUN_L00_00260400_cf(m);
        }
    }
    {
        int         idx = moby_data->i280;
        Moby2C8398 *o;
        if (idx == -1 || (o = &D_L03_0015FFD8_m[idx]) == 0 || o->state == 0xFE ||
            o->state == 0xFD) {
            moby_data->i280 = -1;
        }
    }
    if (moby_data->i280 != -1) {
        {
            float        out[4];
            Moby2C8398  *base;
            Moby2C8398  *other;
            PathF2C8398 *pf;
            int          i;
            *(OvlQuad *)out = 0;
            base = D_L03_0015FFD8_m;
            other = &base[moby_data->i280];
            pf = &moby_data->pf;
            if ((short)other->cls == 0x335 || (short)other->cls == 0x4BA) {
                for (i = 0; i < *(int *)pf->path; i++) {
                    float path_node_rotation[4];
                    *(OvlQuad *)path_node_rotation = 0;
                    probe_ground_height_cf((int)m,
                                           (char *)other,
                                           pf->path + 0x10 + i * 0x10,
                                           path_node_rotation,
                                           pf->path + 0x10 + i * 0x10,
                                           path_node_rotation);
                }
            }
            probe_ground_height_cf(
                (int)m, (char *)other, (char *)m->pos, m->rot, (char *)m->pos, out);
            m->rot[2] = out[2];
        }
    }
    if (!(m->state >= 0xB && m->state <= 0xE)) {
        float g;
        if (m->state == 0xA) {
            moby_data->mv.f18 = 0.0f;
        } else {
            moby_data->mv.f18 = moby_data->mv.f18 + frame_time_sq * 9.8f;
        }
        m->pos[2] = m->pos[2] - (moby_data->mv.f18 - 2.0f);
        g = probe_ground_height(m->pos, 0, 0.5f);
        m->pos[2] = m->pos[2] - 2.0f;
        if (m->pos[2] < g) {
            CollisionHit *hit = &D_L03_00173F40;
            if (hit->moby != 0 && FUN_L03_0024fba8_cf(hit->moby) != 0) {
                moby_data->i280 = (Moby2C8398 *)hit->moby - D_L03_0015FFD8_m;
            } else if (moby_data->i280 != -1) {
                short cls = D_L03_0015FFD8_m[moby_data->i280].cls;
                if (cls != 0x335 && cls != 0x4BA) {
                    moby_data->i280 = -1;
                }
            }
            m->pos[2] = g;
            moby_data->mv.f18 = 0.0f;
            if (moby_data->i280 != -1) {
                float a[4];
                float b[4];
                *(OvlQuad *)a = *(OvlQuad *)D_L03_00173F80;
                *(OvlQuad *)b = *(OvlQuad *)m->rot;
                mark_moby_for_removal_cf(a, b, m->rot[2]);
            }
        } else if (g + 5.0f < m->pos[2]) {
            blend_moby_animation(m, 0xB, 0, scale_game_frames(10));
            m->state = 0xB;
            clear_u64_value(&moby_data->q);
            moby_data->q.f8 = -moby_data->mv.f18;
            moby_data->q.f10 = frame_time_sq * 19.6f;
        }
    }
    if (moby_data->b262 == 0) {
        if (vector_distance_xy(m->pos, D_L03_00166EC0) < 70.0f &&
            (D_L03_0015F5CC & 7) == moby_data->s28A) {
            PathF2C8398 *pf = &moby_data->pf;
            float        v[4];
            int          seg;
            float        frac;
            FUN_L00_0025c698_cf(pf->path, m->pos, v, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
            if (m->pos[2] < 5.0f || m->pos[2] < v[2] - 10.0f) {
                float burst_position[4];
                qcopy(burst_position, m->pos);
                burst_position[2] += 1.0f;
                spawn_explosion(m,
                                moby_data->v40,
                                burst_position,
                                0.0f,
                                0.0f,
                                5,
                                2,
                                4,
                                2.0f,
                                1.0f,
                                9.0f,
                                6,
                                1.0f,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D0,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D1,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D2,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00259710_cf(m, 0x200, -1);
                if (moby_data->i264 == -1) {
                    mark_moby_for_removal(m);
                    return;
                }
                *(OvlQuad *)m->pos = *(OvlQuad *)(pf->path + 0x10);
                clear_u64_value(moby_data->v40);
                moby_data->mv.f18 = 0.0f;
                m->state = 0xE;
                m->flags |= 0x41;
                m->i94 = 0;
            }
        }
    }
}

extern float D_L03_001618E0 __attribute__((sda));

#include "rnc/overlay/l03/mobys/metropolis_train_locomotive_data/update_kerwan_path_enemy_hit_reaction.h"

extern int
fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");

/* Launch speed for a jump of height h at gravity g (per-frame units). */
#define JUMP_SPEED(g, h) square_root(2.0f * (g) * (frame_time_sq * (h)))

/* Hit reaction for class 574 (the path-walking enemy): takes damage from the hit, turns towards the
 * attacker (the hero when hit by the wrench or by nothing) and starts a knock-back jump whose size depends
 * on the hit kind; a lethal hit starts the death jump. */
void update_kerwan_path_enemy_hit_reaction(Moby2C8398 *moby) __asm__("FUN_L03_002c95a8");

void update_kerwan_path_enemy_hit_reaction(Moby2C8398 *moby) {
    enum {
        DEATH_KNOCKBACK_RESPONSE = 1,
        KNOCKBACK_WITHOUT_FACING_CHANGE_RESPONSE = 3,
        ALTERNATE_ANIMATION_KNOCKBACK_RESPONSE = 4,
        ANIMATION_SIX_KNOCKBACK_RESPONSE = 5,
        TURN_AWAY_KNOCKBACK_RESPONSE = 6,
        ANIMATION_FIVE_KNOCKBACK_RESPONSE = 7,
        ANIMATION_FIVE_KNOCKBACK_WITHOUT_FACING_CHANGE_RESPONSE = 8,
        NO_KNOCKBACK_HIT_RESPONSE = 12,
        ZERO_DAMAGE_NO_KNOCKBACK_RESPONSE = 13
    };
    float       hit_knockback_vector[4];
    int         kind;
    float       damage;
    float       angle;
    Data2C8398 *moby_data;
    char       *hit;
    char       *other;
    int         result;

    damage = 0.0f;
    moby_data = moby->data;
    if (moby_data->i38 != 0) {
        moby_data->i38 = 0;
        moby_data->s252 = scale_game_frames(240);
    }
    if (fast_dec_timer_s16(&moby_data->s252)) {
        moby_data->f270 = moby_data->f26C;
    } else {
        moby_data->f270 = moby_data->f26C + 5.0f;
    }
    hit = find_moby_hit(moby, 0x330000, 0);
    result = fun_00213928(moby, hit, &moby_data->f20, 0, &kind, &damage, 0, 4);
    if (hit != 0 && kind != 1 && moby->state != 0xD) {
        other = *(char **)(hit + 0x20);
        if (other != 0) {
            if (other != (char *)hero.items[0].moby) {
                angle = fast_arc_tan(moby->pos[0] - *(float *)(other + 0x10),
                                     moby->pos[1] - *(float *)(other + 0x14));
            } else {
                angle = fast_arc_tan(moby->pos[0] - hero.motion.pos.f[0],
                                     moby->pos[1] - hero.motion.pos.f[1]);
            }
        } else {
            angle = fast_arc_tan(moby->pos[0] - hero.motion.pos.f[0],
                                 moby->pos[1] - hero.motion.pos.f[1]);
        }
        moby_data->f20 -= damage;
        if (moby_data->f20 <= 0.0f) {
            result = 1;
        }
        switch (result) {
        case NO_KNOCKBACK_HIT_RESPONSE:
        case ZERO_DAMAGE_NO_KNOCKBACK_RESPONSE:
            moby_data->b67 = 0x78;
            break;
        case TURN_AWAY_KNOCKBACK_RESPONSE:
            moby_data->b67 = 0x78;
            moby_data->q.f10 = frame_time_sq * 80.0f;
            moby_data->q.f18 =
                (kerwan_path_enemy_knockback_horizontal_distance /
                     (2.0f * square_root(2.0f * D_L03_001618E0 * moby_data->q.f10) /
                      (frame_time_sq * 80.0f)) +
                 kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq * 0.5f *
                     (2.0f * JUMP_SPEED(D_L03_001618E0, 80.0f) / (frame_time_sq * 80.0f))) *
                0.5f;
            moby_data->q.f1C = JUMP_SPEED(D_L03_001618E0, 80.0f);
            moby_data->q.i24 = 9;
            moby_data->q.b3D = 0;
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                hit_knockback_vector, &angle, &moby_data->q.f18, &moby_data->q.f1C);
            if (fast_difference_between_rotations(moby->rot[2], angle) < 1.0471976f) {
                moby->rot[2] = fast_add_rotations(angle, 3.1415927f);
            }
            trooper_start_knockback(angle, moby, (char *)moby_data + 0x70, 5, 5, 0);
            set_moby_animation(moby, 5, 5);
            moby_data->q.f50 = 14.0f;
            moby_data->q.f54 = 28.0f;
            moby->state = 10;
            allocate_voice_for_target_entry(1, 0, (int)moby);
            break;
        case KNOCKBACK_WITHOUT_FACING_CHANGE_RESPONSE:
        case ANIMATION_FIVE_KNOCKBACK_RESPONSE:
        case ANIMATION_FIVE_KNOCKBACK_WITHOUT_FACING_CHANGE_RESPONSE:
            moby_data->b67 = 0x78;
            moby_data->q.f10 = frame_time_sq * 80.0f;
            moby_data->q.f14 = kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq;
            moby_data->q.f18 =
                kerwan_path_enemy_knockback_horizontal_distance /
                    (2.0f * square_root(2.0f * D_L03_001618E0 * moby_data->q.f10) /
                     (frame_time_sq * 80.0f)) +
                kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq * 0.5f *
                    (2.0f * JUMP_SPEED(D_L03_001618E0, 80.0f) / (frame_time_sq * 80.0f));
            moby_data->q.f1C = JUMP_SPEED(D_L03_001618E0, 80.0f);
            moby_data->q.i24 = 9;
            moby_data->q.b3D = 0;
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                hit_knockback_vector, &angle, &moby_data->q.f18, &moby_data->q.f1C);
            trooper_start_knockback(angle, moby, (char *)moby_data + 0x70, 5, 5, 0);
            moby_data->q.f50 = 6.0f;
            moby_data->q.f54 = 13.0f;
            moby->state = 10;
            allocate_voice_for_target_entry(1, 0, (int)moby);
            break;
        case ALTERNATE_ANIMATION_KNOCKBACK_RESPONSE:
        case ANIMATION_SIX_KNOCKBACK_RESPONSE:
            moby_data->b67 = 0x78;
            moby_data->q.f10 = frame_time_sq * 80.0f;
            moby_data->q.f14 = kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq;
            moby_data->q.f18 =
                kerwan_path_enemy_knockback_horizontal_distance /
                    (2.0f * square_root(2.0f * D_L03_001618E0 * moby_data->q.f10) /
                     (frame_time_sq * 80.0f)) +
                kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq * 0.5f *
                    (2.0f * JUMP_SPEED(D_L03_001618E0, 80.0f) / (frame_time_sq * 80.0f));
            moby_data->q.f1C = JUMP_SPEED(D_L03_001618E0, 80.0f);
            moby_data->q.i24 = 9;
            moby_data->q.b3D = 0;
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                hit_knockback_vector, &angle, &moby_data->q.f18, &moby_data->q.f1C);
            trooper_start_knockback(angle, moby, (char *)moby_data + 0x70, 6, 5, 2);
            moby_data->q.f50 = 5.0f;
            moby_data->q.f54 = 10.0f;
            moby->state = 10;
            allocate_voice_for_target_entry(1, 0, (int)moby);
            break;
        case DEATH_KNOCKBACK_RESPONSE:
            moby_data->q.f10 = frame_time_sq * 40.0f;
            moby_data->q.f14 = kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq;
            moby_data->q.f18 =
                3.0f /
                    (2.0f * square_root(2.0f * 2.0f * moby_data->q.f10) / (frame_time_sq * 40.0f)) +
                kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq * 0.5f *
                    (2.0f * JUMP_SPEED(2.0f, 40.0f) / (frame_time_sq * 40.0f));
            moby_data->q.f1C = JUMP_SPEED(2.0f, 40.0f);
            moby_data->q.i24 = 9;
            moby_data->q.b3D = 0;
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                hit_knockback_vector, &angle, &moby_data->q.f18, &moby_data->q.f1C);
            trooper_start_knockback(angle, moby, (char *)moby_data + 0x70, 7, 5, 2);
            moby_data->q.f50 = 10.0f;
            moby_data->q.f54 = 20.0f;
            moby_data->q.f4C = 2.0f * frame_time;
            moby_data->b255 = 0;
            moby->flags &= 0xEFFF;
            moby_data->b67 = 0xFA;
            moby->state = 0xD;
            allocate_voice_for_target_entry(3, 0, (int)moby);
            if (moby->anim != 7) {
                blend_moby_animation(moby, 7, 0, scale_game_frames(6));
            }
            break;
        }
        trooper_anim_update(moby, (short *)((char *)moby_data + 0x60));
    }
    moby->bA4 = 0xFF;
    trooper_anim_finish(moby, (char *)moby_data + 0x60);
}

/* Picks the path node to head for next: walks the path ring from the start and keeps the node best aligned
 * with the target direction, else the one nearest the target (but at least 16 units away). */
void FUN_L03_002c9ca0(Moby2C8398 *moby) {
    Tgt2C8398    target;
    float        near[4];
    float        pt[4];
    int          seg;
    float        path_fraction_or_node_target_distance;
    Data2C8398  *moby_data;
    PathF2C8398 *pf;
    int          best;
    float        best_ang;
    float        best_dist;
    float        node_target_heading_difference;

    moby_data = moby->data;
    best = 0;
    best_ang = 0.0f;
    pf = &moby_data->pf;
    best_dist = best_ang;
    select_moby_target_with_hero_fallback((char *)moby, (char *)&target, moby_data->f270);
    seg = pf->idx;
    FUN_L00_0025df68(pf->path,
                     moby->pos,
                     near,
                     &seg,
                     &path_fraction_or_node_target_distance,
                     0,
                     999.0f,
                     5.0f,
                     0.0f);
    pf->idx = 0;
    do {
        if (pf->idx != seg) {
            qcopy(pt, pf->path + 0x10 + pf->idx * 0x10);
            node_target_heading_difference =
                fast_arc_tan(pt[0] - moby->pos[0], pt[1] - moby->pos[1]);
            node_target_heading_difference = fast_difference_between_rotations(
                node_target_heading_difference,
                fast_arc_tan(target.pos[0] - moby->pos[0], target.pos[1] - moby->pos[1]));
            path_fraction_or_node_target_distance = fast_vec_dist(&target, pt);
            if (node_target_heading_difference < 1.5707964f) {
                if (best_ang < node_target_heading_difference) {
                    best = pf->idx;
                    best_dist = path_fraction_or_node_target_distance;
                    best_ang = node_target_heading_difference;
                }
            } else if ((path_fraction_or_node_target_distance >= 16.0f &&
                        (path_fraction_or_node_target_distance < best_dist || best_dist < 16.0f)) ||
                       (path_fraction_or_node_target_distance < 16.0f &&
                        best_dist < path_fraction_or_node_target_distance)) {
                best = pf->idx;
                best_dist = path_fraction_or_node_target_distance;
                best_ang = node_target_heading_difference;
            }
        }
    } while ((pf->idx = (pf->idx + *(int *)pf->path + pf->step) % *(int *)pf->path) != 0);
    pf->idx = best;
}
