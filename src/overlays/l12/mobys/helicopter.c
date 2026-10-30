/* Level 12 moby class helicopter: hits, reactivation and rotation. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/level_path.h"

#include "qcopy.h"

extern int          scale_game_frames(int);
typedef struct Moby Moby;
extern float        D_0015ED6C;

#include "rnc/game/item_state.h"

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
                             int,
                             float,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern float fast_subtract_rotations(float, float);
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

/* Hit handler: counts hits toward a help message, bursts debris and blinks the moby until its timer runs out. */
#include "rnc/overlay/l12/mobys/helicopter_globals.h"

extern int  helicopter_help_message_hit_count_gp __asm__("D_0015EE04") __attribute__((sda));
extern s32  D_L12_001619B4 __attribute__((sda));
extern void spawn_randomized_bolt_drop(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
extern void FUN_L00_00263d40(int, int);
s32         allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");

void process_helicopter_hits_and_reactivation(struct Moby *moby,
                                              int          flag) __asm__("FUN_L12_002ebca8");

void process_helicopter_hits_and_reactivation(struct Moby *moby, int flag) {
    char          *moby_vars = (char *)moby->pvars;
    float          explosion_position[4];
    float          explosion_vector[4];
    unsigned char *help_message_state;
    if (find_moby_hit(moby, 0xA30000, 0) != 0) {
        if (hero.state.current != 0x32) {
            helicopter_help_message_hit_count_gp += 1;
            if (helicopter_help_message_hit_count_gp >= 5) {
                help_message_state = item_unlocked;
                if (help_message_state[0x15] == 0) {
                    help_message_state[0x15] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    FUN_L00_00263d40(0x53D6, -1);
                }
            }
        }
        qcopy(explosion_position, &moby->pos);
        clear_u64_value(explosion_vector);
        spawn_randomized_bolt_drop(moby, 0x1F, 0x4A, 2, -1);
        *(int *)(moby_vars + 0xA4) = scale_game_frames(500);
        spawn_explosion(moby,
                        explosion_vector,
                        explosion_position,
                        0.0f,
                        0.0f,
                        0x14,
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
    }
    moby->hit_slot = 0xFF;
    if (*(int *)&D_L12_001619B4 != 0 && flag != 0) {
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
    }
    if (*(int *)(moby_vars + 0xA4) == 0 && (moby->flags & 1) == 0)
        return;
    if (fast_dec_timer((int *)(moby_vars + 0xA4)) != 0 && flag != 0 &&
        *(int *)&D_L12_001619B4 == 0) {
        moby->collision = moby->pclass->collision;
        moby->flags = (moby->flags & 0xFFBE) | 0x1000;
    } else {
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002ED228), where it is exact; names translated to the US level program. */

extern void initialize_path_descriptor(char *p) __asm__("FUN_L01_0028b410");
extern void FUN_L12_002ebca8_c(char *m, int flag) __asm__("FUN_L12_002ebca8");
extern int  func_L01_00276680_i(char *vec, float scale) __asm__("FUN_L01_00275690");

extern float helicopter_path_speed_gp __asm__("D_L12_001619A8") __attribute__((sda));
extern float D_L12_001619AC __attribute__((sda));
extern float helicopter_rotation_smoothing_divisor_gp __asm__("D_L12_001619B0")
    __attribute__((sda));
void helicopter_update(char *m) __asm__("FUN_L12_002ebea8");

void helicopter_update(char *moby) {
    enum {
        HELICOPTER_INITIALIZE_PATH = 0,
        HELICOPTER_FOLLOW_PATH = 1
    };
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    float A[4];
    float B[4];
    float C[4];
    float E[4];
    if (moby_vars != 0) {
        int moby_query_result =
            func_L01_00276680_i(moby, convert_integer_to_float(MOBY(moby)->draw_dist));
        FUN_L12_002ebca8_c(moby, moby_query_result == -1);
        switch ((unsigned char)moby[0x20]) {
        case HELICOPTER_INITIALIZE_PATH:
            if (*(int *)(moby_vars + 0x74) != -1) {
                char *tbl;
                int   initial_node_index;
                int   last_node_index;
                initialize_path_descriptor(moby_vars + 0x60);
                tbl = ((char *)l12_paths[*(int *)(moby_vars + 0x74)]);
                *(char **)(moby_vars + 0x70) = tbl;
                initial_node_index = *(int *)(moby_vars + 0xA0);
                last_node_index = *(int *)tbl - 1;
                if (last_node_index < initial_node_index)
                    initial_node_index = last_node_index;
                qcopy((moby + 0x10), tbl + initial_node_index * 16 + 0x10);
                moby_vars[0x2A] = 0x10;
                moby_vars[0x28] = 2;
                moby_vars[0x2C] = 0x2C;
                *(float *)(moby_vars + 0x30) = 1.4f;
                moby_vars[0x2B] = 1;
                *(int *)(moby_vars + 0x60) = initial_node_index;
                moby[0x20] = 1;
                MOBY(moby)->draw_dist = 0xAA;
                MOBY(moby)->flags |= 0x1000;
            }
            break;
        case HELICOPTER_FOLLOW_PATH: {
            char *path_state = moby_vars + 0x60;
            int   current_node_index = *(int *)path_state;
            char *tbl = *(char **)(path_state + 0x10);
            int   step = *(char *)(path_state + 4);
            int   node_count = *(int *)tbl;
            int   previous_node_index = (current_node_index + node_count - step) % node_count;
            int   next = (current_node_index + node_count + step) % node_count;
            float segment_heading_delta;
            float segment_heading_delta_per_distance;
            qcopy(A, tbl + previous_node_index * 16 + 0x10);
            qcopy(B, tbl + current_node_index * 16 + 0x10);
            qcopy(C, tbl + next * 16 + 0x10);
            fast_vec_sub(A, B, A);
            fast_vec_sub(B, C, B);
            segment_heading_delta = fast_arc_tan(A[0], A[1]);
            segment_heading_delta =
                fast_subtract_rotations(segment_heading_delta, fast_arc_tan(B[0], B[1]));
            segment_heading_delta_per_distance = segment_heading_delta / fast_vec_length(A);
            MOBY(moby)->rot.x = fast_add_rotations(
                MOBY(moby)->rot.x,
                fast_subtract_rotations(segment_heading_delta_per_distance * D_L12_001619AC,
                                        MOBY(moby)->rot.x) /
                    helicopter_rotation_smoothing_divisor_gp);
            if (fast_vec_dist(moby + 0x10, C) < helicopter_path_speed_gp * D_0015ED6C) {
                char *path;
                *(int *)(moby_vars + 0x60) = next;
                path = *(char **)(path_state + 0x10);
                if (next == *(int *)path - 1) {
                    qcopy((moby + 0x10), path + 0x10);
                }
            }
            fast_vec_sub(E, C, moby + 0x10);
            fast_vec_normalize(E, E, helicopter_path_speed_gp * D_0015ED6C);
            fast_vec_add(moby + 0x10, moby + 0x10, E);
            MOBY(moby)->rot.z = fast_add_rotations(
                MOBY(moby)->rot.z,
                fast_subtract_rotations(fast_arc_tan(E[0], E[1]), MOBY(moby)->rot.z) /
                    helicopter_rotation_smoothing_divisor_gp);
            MOBY(moby)->rot.y = 0.52359879f;
            break;
        }
        }
    }
}
