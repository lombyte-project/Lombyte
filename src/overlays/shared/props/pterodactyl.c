/* Shared moby class pterodactyl: flies along its path. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

typedef struct {
    char  p0[0x60];
    int   path;
    float f64, f68, f6C;
    int   path_loop_enabled;
    float path_progress_increment;
} D_2e3a88;

void sample_camera_path(void *, float, int, void *, void *, int) __asm__("FUN_00214e58");

extern int *toad_paths[] __asm__("D_L00_001B04B0") NOT_SDA;
float       pterodactyl_path_speed_gp __asm__("D_L00_00161C98") = 6.0f;
float       pterodactyl_scale_multiplier_gp __asm__("D_L00_00161C9C") = 2.0f;

float D_L07_00161CD0 = 6.0f;
float D_L07_00161CD4 = 2.0f;
float D_L10_00162054 = 6.0f;
float D_L10_00162058 = 2.0f;
float D_L18_00162570 = 6.0f;
float D_L18_00162574 = 2.0f;

void pterodactyl_update(unsigned char *m) __asm__("FUN_L00_002e3a88");

/* Moby class 1564 */
void pterodactyl_update(unsigned char *moby) {
    enum {
        PTERODACTYL_INITIALIZE = 0,
        PTERODACTYL_FOLLOW_PATH = 1
    };
    D_2e3a88 *moby_data = ((D_2e3a88 *)MOBY(moby)->pvars);
    int      *path = toad_paths[moby_data->path];
    float     y;
    switch (MOBY(moby)->state) {
    case PTERODACTYL_INITIALIZE:
        if (moby_data->path == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        {
            float first_segment_length = fast_vec_dist(path + 4, path + 8);
            float first_to_last_distance = fast_vec_dist(path + 4, path + *path * 4);
            moby_data->path_progress_increment =
                pterodactyl_path_speed_gp * frame_time / first_segment_length;
            moby_data->path_loop_enabled =
                absolute_float(first_segment_length - first_to_last_distance) < 0.1f;
        }
        moby_data->f6C = moby_data->f68 * (float)*path;
        MOBY(moby)->state = PTERODACTYL_FOLLOW_PATH;
        MOBY(moby)->scale *= pterodactyl_scale_multiplier_gp;
        MOBY(moby)->update_dist = 0xFF;
    case PTERODACTYL_FOLLOW_PATH:
        y = MOBY(moby)->pos.z;
        sample_camera_path(
            path, moby_data->f6C, moby_data->path_loop_enabled, &MOBY(moby)->pos, moby + 0x40, 0);
        MOBY(moby)->rot.y *= 0.5f;
        MOBY(moby)->pos.z += moby_data->f64;
        moby_data->f6C += moby_data->path_progress_increment;
        if ((float)*path < moby_data->f6C)
            moby_data->f6C -= (float)*path;
        if (MOBY(moby)->anim_flags & 2) {
            if (y < MOBY(moby)->pos.z || random_integer_below(3) != 0) {
                if (MOBY(moby)->prev_seq)
                    blend_moby_animation(moby, 0, 0, 10);
            } else {
                if (MOBY(moby)->prev_seq != 1)
                    blend_moby_animation(moby, 1, 0, 10);
            }
        }
        break;
    }
}
