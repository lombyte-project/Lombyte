/* Shared moby class spawner (0x1C7): takes hits and drops class 0x221 bombs that hatch its pooled children. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "qcopy.h"
#include "rnc/overlay/cuboid.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) Vec4A_2bef68;

typedef struct Data_2C0330 {
    char          pad0[0x20];
    float         hp;
    char          pad24[5];
    unsigned char b29;
    char          pad2A[0x2E];
    unsigned char b58;
    char          pad59;
    unsigned char b5A;
    char          pad5B[5];
    short         f60[3];
    char          b66;
    unsigned char b67;
    char          pad68[8];
    char          f70[0x12];
    unsigned char b82;
    unsigned char b83;
    unsigned char b84;
    unsigned char b85;
    unsigned char count;
    unsigned char max;
    short         timer;
    short         h8A;
    short         h8C;
    char          pad8E[2];
    float         f90;
    char          pad94[8];
    float         f9C;
    int           pts[4];
    char          padB0[0x20];
    int           fD0;
    char          padD4[8];
    float         fDC;
    char          padE0[4];
    float         fE4;
    int           ids[18];
} Data_2C0330;

typedef struct {
    char  pad0[0x2C];
    float f2C;
} Hit_2C0330;

void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern char *D_L03_0015FFE4_macro __asm__("D_L03_0015FFE4") MACRO_ADDR;
extern char *D_L03_0015FFD8_macro __asm__("D_L03_0015FFD8") MACRO_ADDR;

extern void  FUN_L00_0025a120_2bef68(void *) __asm__("FUN_L00_0025a120");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int
fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void  spawn_explosion(void *,
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
extern void  FUN_L00_00263fd8_2bef68(void *,
                                     int,
                                     void *,
                                     void *,
                                     int,
                                     int,
                                     float,
                                     void *,
                                     void *,
                                     void *) __asm__("FUN_L00_00263fd8");
extern void  memset(void *, int, int) __asm__("FUN_001153fc");
extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int   rand(void) __asm__("FUN_001160d8");
extern void  add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");
extern float
FUN_L00_0025abf0_2bef68(void *, void *, void *, float, float) __asm__("FUN_L00_0025abf0");
extern char *spawn_class_221_moby(char *owner, float *pos, float *dir, short ang, int arg) __asm__("FUN_L03_002c45d8");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern char  D_L03_00166EC0[];
extern char  D_0013E533_2bef68[] __asm__("D_0013E533");
extern float D_L03_0015F580_macro[] __asm__("D_L03_0015F580") MACRO_ADDR;
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

extern f32 frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;

extern char *create_moby(int);
extern char *create_moby(int) __asm__("FUN_0020c4f8");

/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002BFF68), where it is exact; names translated to the US level program. */

int sample_random_transformed_point(int transform_index, float *out) __asm__("FUN_L03_002beba0");

int sample_random_transformed_point(int transform_index, float *out) {
    char *selected_transform;
    if (transform_index != -1) {
        selected_transform = (char *)l03_cuboids + transform_index * 128;
        out[0] = random_float_between(-1.0f, 1.0f);
        out[1] = random_float_between(-1.0f, 1.0f);
        out[2] = random_float_between(-1.0f, 1.0f);
        transform_vector_by_basis(out, out, selected_transform);
        fast_vec_add(out, out, selected_transform + 0x30);
        return 0;
    }
    return 1;
}

/* Update of moby class 455 (spawner): counts its living children, takes hits, runs its state machine and spawns class 0x221 children at a point picked by its mode. */

void bomb_spawner_update(struct Moby *moby) __asm__("FUN_L03_002bef68");

void bomb_spawner_update(struct Moby *moby) {
    enum {
        SPAWNER_INITIALIZE = 0,
        SPAWNER_WAIT_FOR_PROXIMITY = 1,
        SPAWNER_SPAWN_ENABLED = 2,
        SPAWNER_SPAWN_ENABLED_SHARED = 3,
        SPAWNER_ALLOW_SPAWNING = 4,
        SPAWNER_SPAWN_ENABLED_AFTER_PROXIMITY = 5,
        SPAWNER_SPAWN_ENABLED_ALIAS = 6,
        SPAWNER_SPAWN_ENABLED_SHARED_ALIAS = 7,
        SPAWNER_DESTROY = 8,
        SPAWNER_RESET_TO_PROXIMITY_WAIT = 9
    };
    enum {
        SPAWNER_TRIGGER_DISTANCE_ONLY = 0,
        SPAWNER_TRIGGER_VOLUME_ONLY = 1,
        SPAWNER_TRIGGER_VOLUME_OR_DISTANCE = 2
    };
    enum {
        SPAWNER_TARGET_COUNT_ANGULAR_SPREAD = 0,
        SPAWNER_TARGET_HALF_REFERENCE_OFFSET = 1,
        SPAWNER_TARGET_UNUSED_POINT = 2,
        SPAWNER_TARGET_HELPER_INDEXED_POINT = 3,
        SPAWNER_TARGET_COUNT_INDEXED_POINT = 4,
        SPAWNER_TARGET_SKIP_SETUP = 5
    };
    enum {
        SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED = 0,
        SPAWNER_ACTIVATION_ENTER_SHARED_SPAWN_ENABLED = 1,
        SPAWNER_ACTIVATION_ENTER_SHARED_SPAWN_ENABLED_ALIAS = 2,
        SPAWNER_ACTIVATION_SET_SPAWN_ENABLED = 3,
        SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED_AFTER_PROXIMITY = 4,
        SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED_AFTER_PROXIMITY_ALIAS = 5,
        SPAWNER_ACTIVATION_STAY_IN_PROXIMITY_WAIT = 6
    };
    int          used[4];
    Vec4A_2bef68 launch_origin;
    Vec4A_2bef68 launch_velocity;
    Vec4A_2bef68 target_position;
    Vec4A_2bef68 spawn_position;
    Vec4A_2bef68 sampled_point_scratch;
    int          damage_query_status;
    Data_2C0330 *spawner_data;
    Hit_2C0330  *r;
    int         *ids;
    int          i;
    int          near1;
    int          near2;
    int          timerDone;
    int          spawn;
    int          inVol;
    int          room;
    int          cond;
    float        reference_distance_xy;
    float        target_ground_height;
    int          kids = 18;

    if (moby == 0)
        return;
    spawner_data = (Data_2C0330 *)moby->pvars;
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L03_00166EC0) < 28.0f) {
        FUN_L00_0025a120_2bef68(moby);
        moby->shadow_range = 0x16;
    }
    if (spawner_data == 0)
        return;
    ids = spawner_data->ids;
    spawner_data->count = 0;
    for (i = 0; i < kids; i++) {
        struct Moby *o;
        int          id = ids[i];
        if (id == -1)
            continue;
        o = (struct Moby *)(D_L03_0015FFD8_macro + (id << 8));
        if (o != 0 && o->state != 0xFE && o->state != 0xFD) {
            spawner_data->count++;
        }
    }
    {
        struct Moby *o;
        for (o = (struct Moby *)D_L03_0015FFE4_macro; o != 0; o = o->next) {
            if (o->oclass == 0x221 && o->state != 0xFE && o->state != 0xFD) {
                spawner_data->count++;
            }
        }
    }
    reference_distance_xy = vector_distance_xy(&moby->pos, D_0013E533_2bef68 + 0xE9D);
    near1 = reference_distance_xy < spawner_data->f9C;
    near2 = reference_distance_xy < spawner_data->f90;
    spawn = 0;
    timerDone = fast_dec_timer_s16(&spawner_data->timer) != 0;
    inVol = 0;
    room = spawner_data->count < spawner_data->max;
    if (spawner_data->fD0 != -1) {
        inVol = is_point_inside_clip_volume(D_0013E533_2bef68 + 0xE9D, spawner_data->fD0) != 0;
    }
    cond = inVol | near2;
    switch (spawner_data->b82) {
    case SPAWNER_TRIGGER_DISTANCE_ONLY:
        cond = near1;
        break;
    case SPAWNER_TRIGGER_VOLUME_ONLY:
        cond = inVol;
        break;
    case SPAWNER_TRIGGER_VOLUME_OR_DISTANCE:
        break;
    }
    if (spawner_data->h8A == 0 && spawner_data->count != 0) {
        room = 0;
    }
    r = (Hit_2C0330 *)find_moby_hit(moby, 0xC30000, 0);
    fun_00213928(moby, r, &spawner_data->hp, 0, &damage_query_status, 0, 0, 4);
    if (damage_query_status >= 2) {
        float remaining_health = spawner_data->hp - r->f2C;
        spawner_data->hp = remaining_health;
        if (remaining_health > 0.0f) {
            spawner_data->b67 = 0x5A;
            trooper_anim_update(moby, spawner_data->f60);
        } else {
            mark_moby_for_removal_cf(moby, 0, -1);
            moby->state = 8;
            spawner_data->b67 = 0x78;
            trooper_anim_update(moby, spawner_data->f60);
        }
    }
    moby->hit_slot = 0xFF;
    switch (moby->state) {
    case SPAWNER_INITIALIZE: {
        int k;
        for (k = 0; k < kids; k++) {
            struct Moby *o;
            int          id = ids[k];
            if (id == -1)
                continue;
            o = (struct Moby *)(D_L03_0015FFD8_macro + (id << 8));
            if (o != 0 && o->state != 0xFE && o->state != 0xFD) {
                mark_moby_for_removal(o);
            }
        }
        moby->flags |= 0x1000;
        spawner_data->b5A = 3;
        spawner_data->b58 = 8;
        spawner_data->b29 = 0;
        moby->state = 1;
        break;
    }
    case SPAWNER_WAIT_FOR_PROXIMITY:
        if (near1) {
            switch (spawner_data->b84) {
            case SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED:
                moby->state = 2;
                break;
            case SPAWNER_ACTIVATION_ENTER_SHARED_SPAWN_ENABLED:
                moby->state = 3;
                break;
            case SPAWNER_ACTIVATION_ENTER_SHARED_SPAWN_ENABLED_ALIAS:
                moby->state = 7;
                break;
            case SPAWNER_ACTIVATION_SET_SPAWN_ENABLED:
                moby->state = 2;
                break;
            case SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED_AFTER_PROXIMITY:
                moby->state = 5;
                break;
            case SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED_AFTER_PROXIMITY_ALIAS:
                moby->state = 5;
                break;
            case SPAWNER_ACTIVATION_STAY_IN_PROXIMITY_WAIT:
            default:
                moby->state = 1;
                break;
            }
        }
        break;
    case SPAWNER_SPAWN_ENABLED:
        spawn = 1;
        break;
    case SPAWNER_SPAWN_ENABLED_SHARED:
        spawn = 1;
        break;
    case SPAWNER_ALLOW_SPAWNING:
        spawn = 1;
        break;
    case SPAWNER_SPAWN_ENABLED_AFTER_PROXIMITY:
        spawn = 1;
        break;
    case SPAWNER_SPAWN_ENABLED_ALIAS:
        spawn = 1;
        break;
    case SPAWNER_SPAWN_ENABLED_SHARED_ALIAS:
        spawn = 1;
        break;
    case SPAWNER_DESTROY: {
        float         z;
        float        *p;
        Vec4A_2bef68 *spawner_rotation;
        z = 0.0f;
        p = D_L03_0015F580_macro;
        spawn_explosion(moby,
                        spawner_data->f70,
                        0,
                        0.0f,
                        0.0f,
                        0x1E,
                        8,
                        0x14,
                        8.0f,
                        4.0f,
                        18.0f,
                        1.0f,
                        1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        spawner_rotation = (Vec4A_2bef68 *)&moby->rot;
        FUN_L00_00263fd8_2bef68(moby, 0x6C5, &moby->pos, spawner_rotation, 0, 0, z, p, p, p);
        FUN_L00_00263fd8_2bef68(moby, 0x6C6, &moby->pos, spawner_rotation, 0, 0, z, p, p, p);
        FUN_L00_00263fd8_2bef68(moby, 0x6C7, &moby->pos, spawner_rotation, 0, 0, z, p, p, p);
        mark_moby_for_removal(moby);
        return;
    }
    case SPAWNER_RESET_TO_PROXIMITY_WAIT:
    default:
        moby->state = 1;
        break;
    }
    if (spawn && cond && timerDone && room) {
        int *used_point_flags = used;
        memset(used_point_flags, 0, 0x10);
        while (spawner_data->h8A == 0 ? spawner_data->count < spawner_data->max
                                      : spawner_data->timer == 0) {
            moby_point_position(moby, 0, &launch_origin);
            switch (spawner_data->b85) {
            case SPAWNER_TARGET_COUNT_ANGULAR_SPREAD: {
                float target_spread_angle;
                float rad;
                target_spread_angle = normalize_angle(
                    convert_integer_to_float(spawner_data->count) * 2.0f * 3.1415927f /
                        convert_integer_to_float(spawner_data->max + 1) -
                    3.1415927f);
                rad = vector_distance_xy(&moby->pos, D_0013E533_2bef68 + 0xE9D) * 0.5f;
                if (spawner_data->b83 & 8) {
                    rad = random_float_between(rad, 3.0f);
                }
                if (rad < 3.0f) {
                    rad = 3.0f;
                }
                launch_velocity.x = fast_cos(target_spread_angle) * rad;
                launch_velocity.y = fast_sin(target_spread_angle) * rad;
                launch_velocity.z = 0.0f;
                clamp_vector_length(&launch_velocity, &launch_velocity, spawner_data->f90);
                fast_vec_add(&target_position, &launch_origin, &launch_velocity);
                target_ground_height = probe_ground_height(&target_position, 0, 0.5f);
                if (target_ground_height != 0.0f) {
                    target_position.z = target_ground_height;
                }
                break;
            }
            case SPAWNER_TARGET_HALF_REFERENCE_OFFSET: {
                float len;
                fast_vec_sub(&launch_velocity, D_0013E533_2bef68 + 0xE9D, &launch_origin);
                launch_velocity.z = 0.0f;
                fast_vec_scale(&launch_velocity, &launch_velocity, 0.5f);
                len = fast_vec_length(&launch_velocity);
                if (spawner_data->b83 & 8) {
                    fast_vec_normalize(
                        &launch_velocity, &launch_velocity, random_float_between(len, 3.0f));
                }
                if (fast_vec_length(&launch_velocity) < 3.0f) {
                    fast_vec_normalize(&launch_velocity, &launch_velocity, 3.0f);
                }
                clamp_vector_length(&launch_velocity, &launch_velocity, spawner_data->f90);
                fast_vec_add(&target_position, &launch_origin, &launch_velocity);
                target_ground_height = probe_ground_height(&target_position, 0, 0.5f);
                if (target_ground_height != 0.0f) {
                    target_position.z = target_ground_height;
                }
                break;
            }
            case SPAWNER_TARGET_UNUSED_POINT: {
                float best = 100000.0f;
                int   bi = -1;
                int   j;
                float len;
                for (j = 0; j < 4; j++) {
                    float dd;
                    if (used[j] == 1)
                        continue;
                    if (sample_random_transformed_point(spawner_data->pts[j],
                                                        (float *)&sampled_point_scratch) != 0)
                        continue;
                    dd = vector_distance_xy(&moby->pos, D_0013E533_2bef68 + 0xE9D);
                    if (dd < best) {
                        qcopy(&spawn_position, &sampled_point_scratch);
                        best = dd;
                        bi = j;
                    }
                }
                if (bi != -1) {
                    used[bi] = 1;
                }
                fast_vec_sub(&launch_velocity, &spawn_position, &launch_origin);
                len = fast_vec_length(&launch_velocity);
                if (spawner_data->b83 & 8) {
                    fast_vec_normalize(
                        &launch_velocity, &launch_velocity, random_float_between(len, 3.0f));
                }
                qcopy(&target_position, &spawn_position);
                break;
            }
            case SPAWNER_TARGET_HELPER_INDEXED_POINT: {
                int  *p = spawner_data->pts;
                int   n = 0;
                int   k;
                int   c;
                int   j;
                float len;
                for (c = 0; c < 4; c++) {
                    if (p[c] != -1)
                        n++;
                }
                k = rand() % n + 1;
                for (j = 0; j < 4; j++) {
                    if (p[j] != -1)
                        k--;
                    if (k != 0)
                        continue;
                    if (sample_random_transformed_point(p[j], (float *)&spawn_position) != 0)
                        continue;
                    break;
                }
                fast_vec_sub(&launch_velocity, &spawn_position, &launch_origin);
                len = fast_vec_length(&launch_velocity);
                if (spawner_data->b83 & 8) {
                    fast_vec_normalize(
                        &launch_velocity, &launch_velocity, random_float_between(len, 3.0f));
                }
                qcopy(&target_position, &spawn_position);
                break;
            }
            case SPAWNER_TARGET_COUNT_INDEXED_POINT: {
                int  *p = spawner_data->pts;
                int   point_selection_spawn_count = spawner_data->count;
                int   n = 0;
                int   k;
                int   c;
                int   j;
                float len;
                for (c = 0; c < 4; c++) {
                    if (p[c] != -1)
                        n++;
                }
                k = point_selection_spawn_count % n + 1;
                for (j = 0; j < 4; j++) {
                    if (p[j] != -1)
                        k--;
                    if (k != 0)
                        continue;
                    if (sample_random_transformed_point(p[j], (float *)&spawn_position) != 0)
                        continue;
                    break;
                }
                fast_vec_sub(&launch_velocity, &spawn_position, &launch_origin);
                len = fast_vec_length(&launch_velocity);
                if (spawner_data->b83 & 8) {
                    fast_vec_normalize(
                        &launch_velocity, &launch_velocity, random_float_between(len, 3.0f));
                }
                qcopy(&target_position, &spawn_position);
                break;
            }
            case SPAWNER_TARGET_SKIP_SETUP:
                break;
            }
            {
                float speed;
                char *spawned_moby;
                speed = fast_vec_length(&launch_velocity) * (spawner_data->fE4 * frame_scale_macro);
                launch_velocity.z = 0.0f;
                fast_vec_normalize(&launch_velocity, &launch_velocity, speed);
                fast_vec_normalize(&spawn_position, &launch_velocity, 0.05f);
                add_random_offset_to_vector_xyz((float *)&spawn_position, 0.025f);
                add_vectors(&sampled_point_scratch, &spawn_position, &launch_origin);
                launch_velocity.z =
                    FUN_L00_0025abf0_2bef68(&spawn_position,
                                            &target_position,
                                            0,
                                            speed,
                                            -(spawner_data->fDC * frame_time_sq_macro));
                spawned_moby = spawn_class_221_moby((char *)moby,
                                                    (float *)&spawn_position,
                                                    (float *)&launch_velocity,
                                                    spawner_data->h8C,
                                                    spawner_data->b83);
                if (spawned_moby != 0) {
                    MOBY(spawned_moby)->update_dist = 0xFF;
                }
                spawner_data->timer = scale_game_frames(spawner_data->h8A);
                spawner_data->count++;
            }
        }
    }
    trooper_anim_finish(moby, spawner_data->f60);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002C59A0), where it is exact; names translated to the US level program. */

char *spawn_class_221_moby(

    char *owner, float *pos, float *launch_velocity, short ang, int spawn_options) {
    char *spawned_moby;
    char *moby_pvars;
    spawned_moby = create_moby(0x221);
    if (spawned_moby != 0) {
        moby_pvars = ((char *)MOBY(spawned_moby)->pvars);
        *(char **)(moby_pvars + 0x10) = owner;
        qcopy(spawned_moby + 0x10, pos);
        qcopy(moby_pvars, launch_velocity);
        MOBY(spawned_moby)->rot.x = random_float_between(-3.1415927f, 3.1415927f);
        MOBY(spawned_moby)->rot.y = random_float_between(-3.1415927f, 3.1415927f);
        *(int *)(moby_pvars + 0x18) = 0;
        *(short *)(moby_pvars + 0x14) = scale_game_frames(500);
        *(short *)(moby_pvars + 0x16) = ang;
        MOBY(spawned_moby)->update_dist = (*(unsigned char **)(moby_pvars + 0x10))[0x30];
        MOBY(spawned_moby)->draw_dist = *(unsigned short *)(*(char **)(moby_pvars + 0x10) + 0x32);
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        MOBY(spawned_moby)->scale =
            *(float *)(((char *)MOBY(spawned_moby)->pclass) + 0x24) * 0.001f;
        *(int *)(moby_pvars + 0x1C) = spawn_options;
        *(int *)(moby_pvars + 0x20) = 0;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}
