/* Ported from rac1-decomp (src/game/mobyutil.c, func_00214550). */
#include "sda.h"
#include "qcopy.h"
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern float D_001CAA80[] NOT_SDA;
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern char coll_output[] __asm__("D_00194100");
extern void set_vector_length_xy(float *, float *, float) __asm__("FUN_001f9c48");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float min_float(float, float) __asm__("FUN_001f99d0");
extern float max_float(float, float) __asm__("FUN_001f99c8");
/* Shadow range probe: cast a ray down (8 units) from the moby position
   shifted against the light direction D_001CAA80 by its size, then a
   second one along the light from just above; +0x84/+0x88 get the lower
   and upper hit height (at most 4 apart), or 0 when nothing is below. */
void update_moby_shadow_range(char *m) __asm__("FUN_00213700");

void update_moby_shadow_range(char *moby) {
    float dir[4];
    float scaled_position[4];
    float segment_start[4];
    float b[4];
    float h2;
    float h1;
    float padded_upper_height;
    char *hit;

    qcopy(dir, D_001CAA80);
    set_vector_length_xy(dir, dir, 1.0f);
    fast_vec_scale(scaled_position, moby, 0.0009765625f);
    qcopy(segment_start, scaled_position);
    segment_start[0] -= dir[0] * *(float *)(moby + 0xC) * 0.000732421875f;
    segment_start[1] -= dir[1] * *(float *)(moby + 0xC) * 0.000732421875f;
    qcopy(b, segment_start);
    b[2] -= 8.0f;
    if (collision_line(segment_start, b, 0x22, 0, 0) != 0) {
        qcopy(segment_start, scaled_position);
        hit = coll_output;
        h1 = *(float *)(hit + 0x28);
        segment_start[2] = segment_start[2] + *(float *)(moby + 0xC) * 0.00048828125f;
        segment_start[0] = segment_start[0] + dir[0] * *(float *)(moby + 0xC) * 0.000732421875f;
        segment_start[1] = segment_start[1] + dir[1] * *(float *)(moby + 0xC) * 0.000732421875f;
        fast_vec_normalize(dir, dir, (segment_start[2] - h1) / -dir[2]);
        add_vector_xyz(b, segment_start, dir);
        h2 = h1;
        if (collision_line(segment_start, b, 0x22, 0, 0) != 0) {
            h2 = *(float *)(hit + 0x28);
        }
        *(float *)(moby + 0x84) = min_float(h1, h2) - 0.25f;
        padded_upper_height = max_float(h1, h2) + 0.25f;
        *(float *)(moby + 0x88) = padded_upper_height;
        if (*(float *)(moby + 0x84) + 4.0f < padded_upper_height) {
            *(float *)(moby + 0x88) = *(float *)(moby + 0x84) + 4.0f;
        }
    } else {
        *(float *)(moby + 0x84) = 0.0f;
        *(float *)(moby + 0x88) = 0.0f;
    }
}

extern __typeof__(update_moby_shadow_range) func_00213700 __attribute__((alias("FUN_00213700")));
