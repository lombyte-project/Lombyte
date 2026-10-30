/* Level 17 shuttle. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/input/pad_globals.h"
#include "rnc/mobys/moby_class_id.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "sda.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/overlay/l17/hero/hero_set_state.h"
#include "rnc/rendering/view/project_to_screen.h"
#include "rnc/ui/help/force_help_message.h"

/* Shuttle destination selection shared by its route helpers. */
enum ShuttleStop {
    SHUTTLE_STOP_FIRST = 0x0,
    SHUTTLE_STOP_SECOND = 0x1
};

#include "qcopy.h"

extern char *D_L17_001600EC;
extern float fast_add_rotations(float, float);
extern void  set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
extern void  set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern int   remove_help_message_for_selector(int) __asm__("FUN_L00_002644e0");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void  jet_end_mount(int) __asm__("FUN_L00_002eac18");

extern char D_L17_001B0DB0[];
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");

void blend_moby_animation(void *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

/* UpdateMoby 1448: a moby that alternates between two camera points and starts a cutscene on a button press. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F3450), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[0x454];
    unsigned char collected[1];
} LevelState;

typedef struct {
    char  pad0[0x20];
    char  area[0x40];
    int   a[2];
    int   b[2];
    char  pad70[8];
    int   cam[2];
    int   dir;
    int   cut;
    int   w88;
    char  pad8C[8];
    int   sel;
    int   side;
    float f9C;
    float fA0;
    int   wA4;
    int   go;
    int   sfx;
    int   timer;
} Vars1448;

extern LevelState l17_save_id_collection_state __asm__("D_L17_001BBB30");
extern int        shuttle_travel_completion_bits[] __asm__("D_L17_001BADD0");
extern int        scale_game_frames(int);
extern short      shuttle_second_stop_departure_scale_multiplier __asm__("D_L17_001623A8")
    __attribute__((sda));
extern short shuttle_first_stop_departure_scale_multiplier __asm__("D_L17_001623AC")
    __attribute__((sda));
extern void set_selected_object_float_triplets(float, float, float, float, float, float) __asm__("FUN_L02_002f8aa0");
int  update_shuttle_travel_and_camera(char *moby, int idx) __asm__("FUN_L17_002f2180");
void update_shuttle_help_message(struct Moby *moby) __asm__("FUN_L17_002f20d8");

extern char D_0014C190[];
extern char D_0013CAE4[];

void shuttle_update(struct Moby *m) __asm__("FUN_L17_002f1940");

void shuttle_update(struct Moby *shuttle_moby) {
    enum {
        SHUTTLE_STATE_INITIALIZE = 0,
        SHUTTLE_STATE_WAIT_FOR_ACTIVATION = 1,
        SHUTTLE_STATE_TRAVEL = 2,
        SHUTTLE_STATE_RESET_AFTER_TRAVEL = 3
    };

    Vars1448    *shuttle_vars = (Vars1448 *)shuttle_moby->pvars;
    float        old_pos[4];
    float        old_rot[4];
    float        delta[4];
    float        b[4];
    float        rot[4];
    float        a[4];
    int          alternate_stop_index;
    int          route_index;
    char        *p;
    struct Hero *pl;

    qcopy(old_pos, &shuttle_moby->pos);
    qcopy(old_rot, &shuttle_moby->rot);
    shuttle_moby->scale =
        shuttle_moby->pclass->scale * *(float *)&shuttle_first_stop_departure_scale_multiplier;
    switch (shuttle_moby->state) {
    case SHUTTLE_STATE_INITIALIZE:
        if (l17_save_id_collection_state.collected[(short)shuttle_moby->save_id] != 0 ||
            (*(int *)(((char *)&D_0014C190) +
                      (((short)shuttle_moby->save_id >> 5) * 4 + (current_level_index << 8))) >>
             (shuttle_moby->save_id & 0x1F)) &
                1) {
            shuttle_vars->sel = 0;
            shuttle_vars->dir = 0;
            shuttle_vars->side = 0;
        }
        shuttle_moby->update_dist = 0xFF;
        shuttle_moby->draw_dist = 0x80;
        shuttle_moby->spawn_frame = *(long *)(((char *)hero.moby) + 0x38);
        shuttle_moby->state = 1;
        if (shuttle_moby->prev_seq != 2) {
            blend_moby_animation(shuttle_moby, 2, 0, 0);
        }
        {
            int base = (int)D_L17_001600EC;
            shuttle_vars->sel = 1;
            shuttle_vars->side = 1;
            shuttle_vars->dir = 1;
            *(OvlQuad *)&shuttle_moby->pos =
                *(OvlQuad *)((shuttle_vars->cam[1] << 7) + base + 0x30);
            *(OvlQuad *)&shuttle_moby->rot =
                *(OvlQuad *)((shuttle_vars->cam[1] << 7) + base + 0x70);
        }
        break;
    case SHUTTLE_STATE_WAIT_FOR_ACTIVATION:
        if (shuttle_moby->prev_seq == 2 && (shuttle_moby->anim_flags & 2) &&
            shuttle_vars->sfx == 0) {
            allocate_voice_for_target_entry(1, 0, (int)shuttle_moby);
            shuttle_vars->sfx = 0;
        }
        pl = &hero;
        if (((unsigned char *)pl->ground_moby) == shuttle_moby && pl->air_frames.s == 0) {
            update_shuttle_help_message(shuttle_moby);
            if ((*(int *)(((char *)&D_0013CAE4)) & 0x10) && D_L17_0015F594 == 10) {
                int s = shuttle_vars->dir;
                if (s >= 0) {
                    shuttle_vars->go = 1;
                    switch (s) {
                    case SHUTTLE_STOP_FIRST: {
                        float x = *(float *)&shuttle_first_stop_departure_scale_multiplier;
                        float y = *(float *)&shuttle_second_stop_departure_scale_multiplier;
                        shuttle_vars->sel = 1;
                        shuttle_vars->f9C = x;
                        shuttle_vars->fA0 = y;
                        shuttle_vars->side = 0;
                        break;
                    }
                    case SHUTTLE_STOP_SECOND: {
                        float x = *(float *)&shuttle_second_stop_departure_scale_multiplier;
                        float y = *(float *)&shuttle_first_stop_departure_scale_multiplier;
                        shuttle_vars->side = s;
                        shuttle_vars->f9C = x;
                        shuttle_vars->fA0 = y;
                        shuttle_vars->sel = 0;
                        break;
                    }
                    }
                }
            }
        }
        if (shuttle_vars->go != 0) {
            shuttle_vars->go = 0;
            remove_help_message_for_selector(10);
            shuttle_moby->state = 2;
            allocate_voice_for_target_entry(2, 0, (int)shuttle_moby);
            if (shuttle_moby->prev_seq != 0) {
                blend_moby_animation(shuttle_moby, 0, 0, 0);
            }
            fade_to_black(scale_game_frames(10));
            shuttle_vars->w88 = 0;
            route_index = shuttle_vars->side;
            qcopy(a, ((char **)D_L17_001B0DB0)[shuttle_vars->a[route_index]] + 0x10);
            qcopy(b, ((char **)D_L17_001B0DB0)[shuttle_vars->b[route_index]] + 0x10);
            clear_u64_value(rot);
            rot[2] = fast_arc_tan(b[0] - a[0], b[1] - a[1]);
            rot[1] = -fast_arc_tan(vector_distance_xy(a, b), b[2] - a[2]);
            jet_mount(a, rot, 2, scale_game_frames(300), 0);
            set_selected_object_float_triplets(0.001f, 1.0f, 1.0f, 0.001f, 1.0f, 1.0f);
            set_camera_position(a);
            set_camera_rotation(rot);
            set_race_camera(D_L17_001600EC + (shuttle_vars->cut << 7) + 0x30,
                            D_L17_001600EC + (shuttle_vars->cut << 7) + 0x70,
                            0x72,
                            0);
        } else if (l17_save_id_collection_state.collected[(short)shuttle_moby->save_id] != 0 ||
                   (*(int *)(((char *)&D_0014C190) + (((short)shuttle_moby->save_id >> 5) * 4 +
                                                      (current_level_index << 8))) >>
                    (shuttle_moby->save_id & 0x1F)) &
                       1) {
            float d = vector_distance_xy((&hero.motion.pos),
                                         D_L17_001600EC +
                                             (shuttle_vars->cam[shuttle_vars->sel] << 7) + 0x30);
            if (vector_distance_xy(
                    (&hero.motion.pos),
                    D_L17_001600EC + (shuttle_vars->cam[shuttle_vars->sel ^ 1] << 7) + 0x30) < d) {
                int base;
                int ofs;
                alternate_stop_index = shuttle_vars->sel ^ 1;
                shuttle_vars->dir ^= 1;
                shuttle_vars->side ^= 1;
                shuttle_vars->sel = alternate_stop_index;
                base = (int)D_L17_001600EC;
                ofs = shuttle_vars->cam[alternate_stop_index] << 7;
                qcopy(&shuttle_moby->pos, (char *)(ofs + base + 0x30));
                qcopy(&shuttle_moby->rot, (char *)(ofs + base + 0x70));
                euler_rotation_basis(&shuttle_moby->rot_row0, &shuttle_moby->rot);
                shuttle_moby->scale = shuttle_moby->pclass->scale;
            }
        }
        break;
    case SHUTTLE_STATE_TRAVEL:
        if (update_shuttle_travel_and_camera(shuttle_moby, shuttle_vars->side)) {
            int ofs;
            int base;
            shuttle_moby->state = 3;
            shuttle_vars->wA4 = 0;
            ofs = shuttle_vars->cam[shuttle_vars->sel] << 7;
            base = (int)D_L17_001600EC;
            qcopy(&shuttle_moby->pos, (char *)(ofs + base + 0x30));
            qcopy(&shuttle_moby->rot, (char *)(ofs + base + 0x70));
            euler_rotation_basis(&shuttle_moby->rot_row0, &shuttle_moby->rot);
            shuttle_moby->scale = shuttle_moby->pclass->scale;
            qcopy(b, &shuttle_moby->pos);
            fast_vec_normalize(a, &shuttle_moby->rot_row0, -2.0f);
            fast_vec_add(a, a, &shuttle_moby->rot_row2);
            fast_vec_add(b, b, a);
            clear_u64_value(rot);
            rot[2] = fast_add_rotations(shuttle_moby->rot.z, 3.1415927f);
            set_race_camera(b, rot, 0, 0);
            jet_end_mount(0);
            *(int *)(((char *)&D_0014C190) +
                     (((short)shuttle_moby->save_id >> 5) * 4 + (current_level_index << 8))) |=
                1 << (shuttle_moby->save_id & 0x1F);
            shuttle_travel_completion_bits[(short)shuttle_moby->save_id >> 5] |=
                1 << (shuttle_moby->save_id & 0x1F);
        }
        break;
    case SHUTTLE_STATE_RESET_AFTER_TRAVEL:
        shuttle_moby->state = 1;
        if (shuttle_moby->prev_seq != 2) {
            blend_moby_animation(shuttle_moby, 2, 0, scale_game_frames(30));
        }
        allocate_voice_for_target_entry(0, 0, (int)shuttle_moby);
        shuttle_vars->sfx = 1;
        break;
    }
    fast_vec_sub(delta, &shuttle_moby->pos, old_pos);
    update_entity_matrix(shuttle_vars->area, delta, old_rot, &shuttle_moby->rot);
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F3C18), where it is exact; names translated to the US level program. */

s32 try_set_help_message(s32 arg0, s32 arg1) __asm__("FUN_00215130");

void update_shuttle_help_message(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int   mode = *(int *)(data + 0x94);
    switch (mode) {
    case SHUTTLE_STOP_FIRST:
        *(int *)(data + 0x80) = 0;
        break;
    case SHUTTLE_STOP_SECOND:
        *(int *)(data + 0x80) = mode;
        break;
    }
    switch (*(int *)(data + 0x80)) {
    case SHUTTLE_STOP_FIRST:
        try_set_help_message(0xA, 0x426C);
        break;
    case SHUTTLE_STOP_SECOND:
    case 4:
        try_set_help_message(0xA, 0x426B);
        break;
    case 2:
        try_set_help_message(0xA, 0x1786);
        break;
    case 3:
        try_set_help_message(0xA, 0x1784);
        break;
    }
}

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) PathPt_3CC0_f2180;

typedef struct {
    int               n;
    int               pad[3];
    PathPt_3CC0_f2180 p[1];
} Path_3CC0_f2180;

struct Dat_3CC0_f2180 {
    char  pad0[0x60];
    int   a[2];
    int   b[2];
    int   c[2];
    int   pad1[3];
    int   e84;
    float f88;
    float f8C;
    char  pad2[0x0C];
    float f9C;
    float fA0;
    float fA4;
};

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");

int update_shuttle_travel_and_camera(char *moby, int route_index) {
    float                  camera_rotation[4], camera_position[4];
    struct Dat_3CC0_f2180 *shuttle_vars = ((struct Dat_3CC0_f2180 *)MOBY(moby)->pvars);
    Path_3CC0_f2180       *camera_position_path =
        ((Path_3CC0_f2180 **)D_L17_001B0DB0)[shuttle_vars->a[route_index]];
    Path_3CC0_f2180 *camera_aim_path =
        ((Path_3CC0_f2180 **)D_L17_001B0DB0)[shuttle_vars->b[route_index]];
    Path_3CC0_f2180 *shuttle_movement_path =
        ((Path_3CC0_f2180 **)D_L17_001B0DB0)[shuttle_vars->c[route_index]];
    float scaled_path_position, camera_position_index_float, camera_position_blend,
        camera_aim_blend, moby_path_blend, interpolation_start_angle, turn;
    int i1, i2, i3;

    approach_value(&shuttle_vars->fA4, 1.0f, 1.0f / (float)scale_game_frames(0x384));
    scaled_path_position = shuttle_vars->fA4 * (float)(camera_position_path->n - 1);
    i1 = truncate_float_to_s32(scaled_path_position);
    camera_position_index_float = (float)i1;
    camera_position_blend = scaled_path_position - camera_position_index_float;
    camera_aim_blend = shuttle_vars->fA4 * 1.02f;
    if (camera_aim_blend > 1.0f)
        camera_aim_blend = 1.0f;
    scaled_path_position = camera_aim_blend * (float)(camera_aim_path->n - 1);
    i2 = truncate_float_to_s32(scaled_path_position);
    camera_aim_blend = scaled_path_position - (float)i2;
    scaled_path_position = shuttle_vars->fA4 * (float)(shuttle_movement_path->n - 1);
    moby_path_blend = scaled_path_position - camera_position_index_float;
    i3 = truncate_float_to_s32(scaled_path_position);
    lerp_vector(camera_position,
                &camera_position_path->p[i1],
                &camera_position_path->p[i1 + 1],
                camera_position_blend);
    lerp_vector(moby + 0x10,
                &shuttle_movement_path->p[i3],
                &shuttle_movement_path->p[i3 + 1],
                moby_path_blend);
    set_camera_position(camera_position);
    {
        char *base = D_L17_001600EC + (shuttle_vars->e84 << 7);
        set_race_camera(base + 0x30, base + 0x70, 0x72, 0);
    }
    if (i3 > 0) {
        interpolation_start_angle =
            fast_arc_tan(shuttle_movement_path->p[i3].x - shuttle_movement_path->p[i3 - 1].x,
                         shuttle_movement_path->p[i3].y - shuttle_movement_path->p[i3 - 1].y);
        MOBY(moby)->rot.z = fast_add_rotations(
            fast_subtract_rotations(
                fast_arc_tan(shuttle_movement_path->p[i3 + 1].x - shuttle_movement_path->p[i3].x,
                             shuttle_movement_path->p[i3 + 1].y - shuttle_movement_path->p[i3].y),
                interpolation_start_angle) *
                moby_path_blend,
            interpolation_start_angle);
        interpolation_start_angle = -fast_arc_tan(
            vector_distance_xy(&shuttle_movement_path->p[i3 - 1], &shuttle_movement_path->p[i3]),
            shuttle_movement_path->p[i3].z - shuttle_movement_path->p[i3 - 1].z);
        MOBY(moby)->rot.y = fast_add_rotations(
            fast_subtract_rotations(
                -fast_arc_tan(vector_distance_xy(&shuttle_movement_path->p[i3],
                                                 &shuttle_movement_path->p[i3 + 1]),
                              shuttle_movement_path->p[i3 + 1].z - shuttle_movement_path->p[i3].z),
                interpolation_start_angle) *
                moby_path_blend,
            interpolation_start_angle);
        MOBY(moby)->rot.y = MOBY(moby)->rot.y * fast_cos(shuttle_vars->f88);
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, shuttle_vars->f88);
    } else {
        MOBY(moby)->rot.y = -fast_arc_tan(
            vector_distance_xy(&shuttle_movement_path->p[0], &shuttle_movement_path->p[1]),
            shuttle_movement_path->p[1].z - shuttle_movement_path->p[0].z);
        MOBY(moby)->rot.z =
            fast_arc_tan(shuttle_movement_path->p[1].x - shuttle_movement_path->p[0].x,
                         shuttle_movement_path->p[1].y - shuttle_movement_path->p[0].y);
    }
    if (i1 > 0 && i2 > 0) {
        clear_u64_value(camera_rotation);
        interpolation_start_angle = fast_arc_tan(camera_aim_path->p[i2].x - camera_position[0],
                                                 camera_aim_path->p[i2].y - camera_position[1]);
        camera_rotation[2] = fast_add_rotations(
            fast_subtract_rotations(fast_arc_tan(camera_aim_path->p[i2 + 1].x - camera_position[0],
                                                 camera_aim_path->p[i2 + 1].y - camera_position[1]),
                                    interpolation_start_angle) *
                camera_aim_blend,
            interpolation_start_angle);
        interpolation_start_angle =
            -fast_arc_tan(vector_distance_xy(camera_position, &camera_aim_path->p[i2]),
                          camera_aim_path->p[i2].z - camera_position[2]);
        camera_rotation[1] = fast_add_rotations(
            fast_subtract_rotations(
                -fast_arc_tan(vector_distance_xy(camera_position, &camera_aim_path->p[i2 + 1]),
                              camera_aim_path->p[i2 + 1].z - camera_position[2]),
                interpolation_start_angle) *
                camera_aim_blend,
            interpolation_start_angle);
        set_camera_rotation(camera_rotation);
    }
    MOBY(moby)->scale =
        *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
        (shuttle_vars->f9C - (shuttle_vars->f9C - shuttle_vars->fA0) * shuttle_vars->fA4);
    if (shuttle_vars->fA4 > 0.8f) {
        turn = frame_time_sq * 12.566371f;
        approach_angle(&shuttle_vars->f88,
                       &shuttle_vars->f8C,
                       3.1415927f,
                       turn,
                       turn,
                       frame_time * 6.2831855f);
    }
    return shuttle_vars->fA4 == 1.0f;
}

f32 D_L17_001DD100[1] __attribute__((section(".data"))) = {0};

f32 D_L17_001DD104[1] __attribute__((section(".data"))) = {0};

f32 D_L17_001DD320[1] __attribute__((section(".data"))) = {0};

f32 D_L17_001DD328[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001DD6A8[1] __attribute__((section(".data"))) = {0};
