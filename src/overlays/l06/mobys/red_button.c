/* Level 06 moby class 0x45e: red button. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/l06/scripted_camera_sequence.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/ui/help/try_set_help_message.h"

extern char D_0013F3D0[];

#include "rnc/overlay/cuboid.h"

extern float D_L06_0015F3FC __asm__("D_L06_0015F3FC");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void  set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void  set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
extern void  jet_end_mount(int) __asm__("FUN_L00_002eac18");

extern void set_selected_object_float_triplets(float, float, float, float, float, float) __asm__("FUN_L02_002f8aa0");
extern int  remove_help_message_for_selector(int) __asm__("FUN_L00_002644e0");
extern void fade_to_black(int) __asm__("FUN_001f4a58");

extern int  random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void spawn_particle(void *, void *, int, int, float, int, int, int, int, float) __asm__("FUN_L00_0026a9f0");

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vector_304098;

typedef struct Data_304098 {
    int   cut;
    int   cut2;
    int   list[9];
    float t;
    float vel;
    int   moby3;
    int   moby1;
    int   moby2;
    int   timer;
    int   timer2;
    int   f48;
} Data_304098;

extern struct Moby *D_L06_0015FFD8_304098 __asm__("D_L06_0015FFD8");
extern char         D_0013F3D0[] NOT_SDA;
extern LevelCamera  l06_camera_lui __asm__("D_L06_00167400") NOT_SDA;

extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void allocate_voice_for_group_entry(int, int, void *) __asm__("FUN_0022dba0");
extern void update_moby_304098(void *) __asm__("FUN_L00_00250df8");
void emit_red_button_particle_burst(int unused, void *effect_position) __asm__("FUN_L06_00304818");
extern void            emit_red_button_particle_burst(int, void *) __asm__("FUN_L06_00304818");
extern void            set_moby_state_two(unsigned char *) __asm__("FUN_L06_00303150");
extern char            D_0013A5E0[] NOT_SDA;
extern char D_0013D3AA NOT_SDA;
extern float           D_L06_00162198 __attribute__((sda));
extern float           D_L06_0016219C __attribute__((sda));
extern float           D_L06_001621A0 __attribute__((sda));
extern float           D_L06_001621A4 __attribute__((sda));
extern float           D_L06_001621A8 __attribute__((sda));
extern float           D_L06_001621B0 __attribute__((sda));
extern char            D_0013E633[], D_0014171B[], D_0013D24D[], D_L16_00167240[] NOT_SDA;
void                   red_button_update(struct Moby *moby) __asm__("FUN_L06_00304098");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00305C58), where it is exact; names translated to the US level program. */
typedef struct {
    int v[6];
} V6;

V6 D_L06_002019A0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

extern V6    l06_red_button_particle_second_color_palette __asm__("D_L06_002019B8");
extern float D_L06_001621B4 __attribute__((sda));
int          red_button_particle_emission_count_gp __asm__("D_L06_001621B8") = 8;

V6 l06_red_button_particle_second_color_palette __asm__("D_L06_002019B8") = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

/* Moby class 1118 */
void red_button_update(struct Moby *moby) {
    enum {
        INITIALIZE_RED_BUTTON = 0,
        IDLE_AFTER_INITIALIZATION = 1,
        ROTATE_BUTTON_AND_LINKED_MOBYS = 2,
        WAIT_FOR_NEARBY_HELP_TRIGGER = 3,
        WAIT_CAMERA_START_DELAY = 4,
        RAISE_CAMERA_AND_TRIGGER_LISTED_EFFECTS = 5,
        HOLD_OFFSET_CAMERA_UNTIL_PROGRESS_COMPLETE = 6
    };
    Data_304098  *red_button_vars = *(struct Data_304098 **)&moby->pvars;
    Vector_304098 camera_position;
    Vector_304098 camera_rotation;
    Vector_304098 next_position;
    float         previous_value;
    float         current_angle;
    int           i;
    int           help_message_available;
    int          *particle_burst_cut_index_entry;
    int           duration;
    int           timer;

    switch (moby->state) {
    case INITIALIZE_RED_BUTTON:
        moby->update_dist = 0xFF;
        moby->rot.x = fast_subtract_rotations(moby->rot.x, 0.78539819f);
        moby->state = 1;
        red_button_vars->f48 = -1;
        break;
    case IDLE_AFTER_INITIALIZATION:
        break;
    case ROTATE_BUTTON_AND_LINKED_MOBYS: {
        float angle;
        advance_accelerated_scalar(&red_button_vars->t,
                                   &red_button_vars->vel,
                                   1.0f,
                                   frame_time_sq,
                                   frame_time_sq,
                                   frame_time);
        angle = red_button_vars->vel * 0.78539819f;
        moby->rot.x = fast_add_rotations(moby->rot.x, angle);
        D_L06_0015FFD8_304098[red_button_vars->moby1].rot.x =
            fast_subtract_rotations(D_L06_0015FFD8_304098[red_button_vars->moby1].rot.x, angle);
        D_L06_0015FFD8_304098[red_button_vars->moby2].rot.x =
            fast_subtract_rotations(D_L06_0015FFD8_304098[red_button_vars->moby2].rot.x, angle);
        update_moby_304098(&D_L06_0015FFD8_304098[red_button_vars->moby1]);
        update_moby_304098(&D_L06_0015FFD8_304098[red_button_vars->moby2]);
        if (1.0f <= red_button_vars->t) {
            moby->state = 3;
            allocate_voice_for_target_entry(1, 0, moby);
            D_0013D3AA = 1;
        }
        break;
    }
    case WAIT_FOR_NEARBY_HELP_TRIGGER:
        if (vector_distance_xy(D_0013F3D0, &moby->pos) < 3.7f) {
            help_message_available = try_set_help_message(4, 0x1783) != 0;
            if ((*(int *)(D_0013A5E0 + 0x2504) & 0x10) && help_message_available) {
                allocate_voice_for_target_entry(2, 0, moby);
                remove_help_message_for_selector(4);
                moby->state = 4;
                red_button_vars->timer = scale_game_frames(0x1E);
            }
        }
        break;
    case WAIT_CAMERA_START_DELAY:
        if (fast_dec_timer(&red_button_vars->timer)) {
            qcopy_nc(&camera_position, &l06_cuboids[red_button_vars->cut].pos);
            qcopy(&camera_rotation, &l06_cuboids[red_button_vars->cut].rot);
            moby->state = 5;
            red_button_vars->t = 0.0f;
            red_button_vars->vel = 0.0f;
            fade_to_black(scale_game_frames(0xF));
            jet_mount(&camera_position, &camera_rotation, 2, scale_game_frames(0x12C), 0);
            set_selected_object_float_triplets(D_L06_00162198,
                                               D_L06_0016219C,
                                               D_L06_001621A0,
                                               D_L06_001621A4,
                                               D_L06_001621A8,
                                               D_L06_001621B0);
            set_camera_position(&camera_position);
            set_camera_rotation(&camera_rotation);
            scripted_camera_sequence_active = 1;
        }
        if (red_button_vars->timer < scale_game_frames(10)) {
            previous_value = convert_integer_to_float(red_button_vars->timer);
            previous_value = previous_value / (float)scale_game_frames(10);
            D_L06_0015F3FC = 1.0f - previous_value;
        }
        break;
    case RAISE_CAMERA_AND_TRIGGER_LISTED_EFFECTS:
        duration = scale_game_frames(10);
        timer = red_button_vars->timer;
        if (timer < duration) {
            red_button_vars->timer = timer + 1;
            previous_value = convert_integer_to_float(timer + 1);
            previous_value = previous_value / (float)scale_game_frames(10);
            D_L06_0015F3FC = 1.0f - previous_value;
        }
        previous_value = red_button_vars->t;
        advance_accelerated_scalar(&red_button_vars->t,
                                   &red_button_vars->vel,
                                   2.0f,
                                   frame_time_sq,
                                   frame_time_sq,
                                   frame_time * 0.5f);
        qcopy(&camera_position, &l06_cuboids[red_button_vars->cut].pos);
        camera_position.z = camera_position.z + red_button_vars->t * 18.0f;
        set_camera_position(&camera_position);
        set_camera_rotation(&l06_cuboids[red_button_vars->cut].rot);
        previous_value = previous_value * 100.0f;
        current_angle = red_button_vars->t * 100.0f * 0.0174532925f;
        previous_value = previous_value * 0.0174532925f;
        particle_burst_cut_index_entry = red_button_vars->list;
        for (i = 8; i >= 0; i--, particle_burst_cut_index_entry++) {
            if (previous_value < l06_cuboids[*particle_burst_cut_index_entry].rot.z &&
                l06_cuboids[*particle_burst_cut_index_entry].rot.z < current_angle) {
                emit_red_button_particle_burst((int)moby,
                                               &l06_cuboids[*particle_burst_cut_index_entry].pos);
                allocate_voice_for_group_entry(0, 0, moby);
            }
        }
        if (1.075f <= red_button_vars->t) {
            set_race_camera(&l06_cuboids[red_button_vars->cut2].pos,
                            &l06_cuboids[red_button_vars->cut2].rot,
                            0x72,
                            0);
            fade_to_black(scale_game_frames(10));
            moby->state = 6;
            jet_end_mount(3);
            qcopy(&next_position, &l06_cuboids[red_button_vars->cut2].pos);
            next_position.z = next_position.z + 1.0f;
            next_position.y = next_position.y - 2.5f;
            clear_u64_value(&camera_rotation);
            camera_rotation.y = -0.2617993878f;
            camera_rotation.z = *(float *)(D_0013E633 + 0xDB5);
            jet_mount(&next_position, &camera_rotation, 2, scale_game_frames(0x12C), 0);
            l06_camera_lui.shake_amount = 0.3f;
            l06_camera_lui.shake_frames = scale_game_frames(0x2D);
            allocate_voice_for_group_entry(0, 0, moby);
        }
        break;
    case HOLD_OFFSET_CAMERA_UNTIL_PROGRESS_COMPLETE:
        advance_accelerated_scalar(&red_button_vars->t,
                                   &red_button_vars->vel,
                                   2.0f,
                                   frame_time_sq,
                                   frame_time_sq,
                                   frame_time + frame_time);
        qcopy(&camera_rotation, &l06_cuboids[red_button_vars->cut2].pos);
        camera_rotation.z = camera_rotation.z + 1.0f;
        camera_rotation.y = camera_rotation.y - 2.5f;
        clear_u64_value(&camera_position);
        camera_position.y = -0.2617993878f;
        camera_position.z = *(float *)(D_0013E633 + 0xDB5);
        set_camera_position(&camera_rotation);
        set_camera_rotation(&camera_position);
        if (2.0f <= red_button_vars->t) {
            moby->state = 7;
            red_button_vars->timer2 = scale_game_frames(0xE10);
            set_race_camera(&l06_cuboids[red_button_vars->cut2].pos,
                            &l06_cuboids[red_button_vars->cut2].rot,
                            0,
                            0);
            jet_end_mount(3);
            set_moby_state_two((unsigned char *)&D_L06_0015FFD8_304098[red_button_vars->moby3]);
        }
        break;
    }
}

void emit_red_button_particle_burst(int unused, void *effect_position) {
    float zero_effect_vector[4];
    V6    first_color_palette;
    V6    second_color_palette;
    int   i = 0;
    clear_u64_value(zero_effect_vector);
    if (red_button_particle_emission_count_gp > 0) {
        do {
            float frame_scaled_random_factor;
            float scaled_effect_factor;
            int  *pa;
            int  *pb;
            int   random_frame_count;
            int   r3;
            i++;
            frame_scaled_random_factor = random_float_between(8.0f, 10.0f);
            first_color_palette = D_L06_002019A0;
            second_color_palette = l06_red_button_particle_second_color_palette;
            frame_scaled_random_factor = frame_scaled_random_factor * frame_time;
            pa = first_color_palette.v + random_integer_below(6);
            pb = second_color_palette.v + random_integer_below(6);
            scaled_effect_factor = D_L06_001621B4 * 400000.0f;
            random_frame_count =
                random_int_between(scale_game_frames(0xF), scale_game_frames(0x14));
            spawn_particle(effect_position,
                           zero_effect_vector,
                           *pa,
                           *pb,
                           scaled_effect_factor,
                           random_frame_count,
                           random_int_between(scale_game_frames(0x19), scale_game_frames(0x1E)),
                           0,
                           0,
                           frame_scaled_random_factor * D_L06_001621B4);
        } while (i < red_button_particle_emission_count_gp);
    }
}
