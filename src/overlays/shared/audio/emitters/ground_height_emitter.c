/* Sound emitter type 4: a voice that follows the camera target, louder the more ground around it lies below the reference height. */
#include "types.h"
#include "rnc/math/fast_math.h"

#include "qcopy.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/shared/vendor/vendor_volume.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/rendering/effects/ground_particle_scatter.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00318468), where it is exact; names translated to the US level program. */

extern char  D_0013E533[];
extern char  D_L08_0016C588[];
extern float frame_delta_time __asm__("D_0015ED7C");
extern int   FUN_L00_00263468();
extern int   play_voice(int sound, int flags, int moby, int volume) __asm__("FUN_L00_0028ddd8");
extern float smoothed_below_reference_ground_fraction_gp __asm__("D_L08_001625E8")
    __attribute__((sda));

void update_ground_height_modulated_voice(char *moby) __asm__("FUN_L08_00316fa8");

void update_ground_height_modulated_voice(char *moby) {
    float point[4];
    float radius[4];
    float axis[4];
    char *data = *(char **)(moby + 8);
    float angle = 0.0f;
    float fraction = angle;
    radius[0] = 4.0f;
    radius[1] = fraction;
    radius[2] = fraction;
    radius[3] = 1.0f;
    axis[0] = fraction;
    axis[1] = fraction;
    axis[2] = 1.0f;
    do {
        build_look_at_matrix(point, radius, axis, angle);
        fast_vec_add(point, D_L08_001675C0, point);
        if (ground_height_at(point) <
            ((GroundScatterCamera *)(D_L08_001675C0 - 0x140))->reference_height)
            fraction += 0.05f;
        angle += 0.31415927f;
    } while (angle < 6.2831855f);
    if (smoothed_below_reference_ground_fraction_gp < fraction) {
        smoothed_below_reference_ground_fraction_gp += frame_delta_time * 0.2f;
    } else {
        smoothed_below_reference_ground_fraction_gp -= frame_delta_time * 0.75f;
    }
    if (1.0f < smoothed_below_reference_ground_fraction_gp) {
        smoothed_below_reference_ground_fraction_gp = 1.0f;
    } else if (smoothed_below_reference_ground_fraction_gp < 0.0f) {
        smoothed_below_reference_ground_fraction_gp = 0.0f;
    }
    {
        char *slot = ((char *)&voice_pool) + *(int *)(data + 8) * 0x70;
        if (*(char **)(slot + 0x8C) == moby && ((unsigned char *)slot)[0x74] != 0) {
            vendor_volume(
                *(int *)(data + 8),
                (int)((*(float *)(data + 4) + smoothed_below_reference_ground_fraction_gp) /
                      (*(float *)(data + 4) + 1.0f) * 1024.0f));
        }
    }
    qcopy_nc(((char *)&voice_pool) + 0x90 + *(int *)(data + 8) * 0x70, D_L08_001675C0);
    {
        char *slot = ((char *)&voice_pool) + *(int *)(data + 8) * 0x70;
        if (*(char **)(slot + 0x8C) != moby || ((unsigned char *)slot)[0x74] == 0) {
            *(int *)(data + 8) = play_voice(*(int *)data, 0x15, (int)moby, 0x400);
        }
    }
}

float D_L14_001624D0 = 0;
float smoothed_below_reference_ground_fraction_gp __asm__("D_L08_001625E8") = 0;
