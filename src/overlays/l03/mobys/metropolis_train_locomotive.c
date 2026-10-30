/* Level 03 moby class metropolis_train_locomotive: the train engine that pulls the carriages along its path. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/cuboid.h"

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

extern int  D_L03_0015FDFC;
extern char train_missing_master_warning_format[] __asm__("D_L03_001E2448");
extern char train_end_path_missing_stop_node_warning[] __asm__("D_L03_001E2478");
extern int  FUN_L00_0025df68(void *, void *, void *, int *, float *, int, float, float, float);
extern int  is_hero_on_moby(char *) __asm__("FUN_L01_00277fb8");
extern int  is_moby_class_23e_in_state_14(char *) __asm__("FUN_L03_002c6c60");
extern void set_moby_dialogue_record_value(void *, int) __asm__("FUN_L00_00266858");
extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");
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
            request_music_track(4, 7);
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
        request_music_track(0, 8);
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
