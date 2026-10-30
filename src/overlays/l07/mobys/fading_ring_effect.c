/* Level 07 moby class 0x416: fading ring effect. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"

extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

#include "qcopy.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030E820), where it is exact; names translated to the US level program. */
extern char D_L07_00166E40[];

/* Slows and steers a moby toward another when it is below and close in height. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F4C0), where it is exact; names translated to the US level program. */
extern void spawn_nearby_effect(char *m, float t, int a, int b, void *pos, void *vel) __asm__("FUN_L00_00259bc8");
void register_fading_ring_hit(char *affected_moby,
                              char *effect_moby,
                              char *effect_data) __asm__("FUN_L07_0030e0e0");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F5D8), where it is exact; names translated to the US level program. */
extern char *ring_hit_candidate_list_head __asm__("D_L07_0015FFE4");
extern int   FUN_L07_0030e0e0_c() __asm__("FUN_L07_0030e0e0");
extern void  draw_fading_ring_effect(struct Moby *) __asm__("FUN_L07_0030dcf0");
void         mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");
void         update_fading_ring_effect(unsigned char *moby) __asm__("FUN_L07_0030e1f8");

/* steps a hit-effect emitter: orient, spawn a puff, and reseed the timer */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00313690), where it is exact; names translated to the US level program. */
extern int                FUN_0022da68(int, int, int);
extern int D_L07_0015F5CC MACRO_ADDR;

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030EF90), where it is exact; names translated to the US level program. */
unsigned char *spawn_fading_ring_effect(int   a,
                                        void *pos,
                                        int   b,
                                        float f0,
                                        float f1,
                                        float f2,
                                        float f3,
                                        float f4) __asm__("FUN_L07_0030dbb0");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F0D0), where it is exact; names translated to the US level program. */
typedef u32 u128_0F0D0 __attribute__((mode(TI), aligned(16)));

typedef struct {
    char b[0x90];
} __attribute__((aligned(16))) Pkt_0F0D0;

extern float split_float_fraction(void *, float) __asm__("FUN_L00_00200228");
extern int   D_L07_0015F5CC;
extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern void  draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");

unsigned char *spawn_fading_ring_effect(int   spawning_moby,
                                        void *spawn_position,
                                        int   b,
                                        float f0,
                                        float f1,
                                        float f2,
                                        float spawn_speed,

                                        float f4) {
    unsigned char *moby = (unsigned char *)create_moby(0x416);
    if (moby != 0) {
        char *data;
        char *distance_reference_position;
        char *distance_response_state;
        data = ((char *)MOBY(moby)->pvars);
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        qcopy(moby + 0x10, spawn_position);
        *(int *)(data + 0x14) = spawning_moby;
        *(float *)(data + 4) = f1;
        *(float *)(data + 0x10) = f2;
        *(float *)(data + 0xC) = spawn_speed;
        *(float *)data = f0;
        *(float *)(data + 8) = f0;
        *(int *)(data + 0x18) = b;
        *(float *)(data + 0x1C) = f4;
        MOBY(moby)->flags |= 0x200;
        distance_reference_position = D_L07_00166E40;
        distance_response_state = distance_reference_position - 0x140;
        *(float *)(distance_response_state + 0x160) =
            0.3f - vector_distance_xy(moby + 0x10, distance_reference_position) / 100.0f;
        *(int *)(distance_response_state + 0x168) = scale_game_frames(0x14);
        update_entity(moby);
        FUN_0022da68(0, 0, (int)moby);
    }
    return moby;
}

void draw_fading_ring_effect(struct Moby *moby) {
    char         packet[0x90] __attribute__((aligned(16)));
    char         second[0x90] __attribute__((aligned(16)));
    char         phase_work[0x10] __attribute__((aligned(16)));
    unsigned int alpha = (unsigned int)moby->alpha / 5u;
    int          index = 0;
    unsigned int green = 0x80;
    float        band = 0.0f;
    char        *data = ((char *)moby->pvars);
    unsigned int green_step = (green / 11) & 0xFF;
    float        radius_threshold = 0.9f;
    float        inner_scale = 0.96f;
    float        pi = 3.1415927f;
    *(long *)(packet + 0x78) = get_effect_texture(*(int *)(data + 0x18));
    *(long *)(packet + 0x80) = 0xFF9000000260L;
    *(long *)(packet + 0x70) = 4;
    *(long *)(packet + 0x88) = 0x8000000048L;
    do {
        float        zero, radius, side, center, angle, inner_radius, width, phase, ft;
        unsigned int gb = (green << 8) | 0x80;
        int          color = ((alpha & 0xFF) << 24) | gb | (green << 16);
        *(int *)(packet + 0x4C) = color;
        *(int *)(packet + 0x48) = color;
        *(int *)(packet + 0x44) = color;
        *(int *)(packet + 0x40) = color;
        if (*(float *)(data + 8) < radius_threshold)
            radius = *(float *)(data + 8) * band;
        else
            radius = *(float *)(data + 8) - band * radius_threshold;
        side = (1.0f - band * 0.5f) * *(float *)(data + 0x10);
        ft = convert_integer_to_float(D_L07_0015F5CC);
        phase = split_float_fraction(phase_work, ft / (band * 40.0f + 40.0f));
        if ((index & 1) == 0)
            phase = -phase;
        *(float *)(packet + 0x50) = phase;
        *(float *)(packet + 0x60) = phase;
        width = convert_integer_to_float((index + 1) < 6 ? index + 1 : 5);
        center = moby->pos.x;
        *(float *)(packet + 0x58) = *(float *)(packet + 0x50) + width;
        {
            int iz = 0;
            zero = *(float *)&iz;
        }
        *(float *)(packet + 0x54) = zero;
        *(float *)(packet + 0x64) = 1.0f;
        *(float *)(packet + 0x14) = moby->pos.y;
        *(float *)(packet + 0x18) = side + moby->pos.z;
        *(float *)(packet + 0x38) = moby->pos.z;
        *(float *)(packet + 0x10) = center - radius * (inner_scale - band * 0.03f);
        *(float *)(packet + 0x6C) = 1.0f;
        *(float *)(packet + 0x34) = *(float *)(packet + 0x14);
        *(float *)(packet + 0x30) = center - radius;
        *(float *)(packet + 0x68) = *(float *)(packet + 0x58);
        *(float *)(packet + 0x5C) = zero;
        *(float *)(packet + 0x08) = side + moby->pos.z;
        *(float *)(packet + 0x28) = moby->pos.z;
        *(Pkt_0F0D0 *)second = *(Pkt_0F0D0 *)packet;
        *(float *)(second + 0x18) = moby->pos.z - 5.0f;
        *(float *)(second + 8) = *(float *)(second + 0x18);
        angle = -2.8274336f;
        do {
            float x, y, rr;
            *(u128_0F0D0 *)(packet + 0) = *(u128_0F0D0 *)(packet + 0x10);
            *(u128_0F0D0 *)(packet + 0x20) = *(u128_0F0D0 *)(packet + 0x30);
            x = fast_cos(angle);
            rr = band * 0.03f;
            inner_radius = radius * (inner_scale - rr);
            *(float *)(packet + 0x10) = moby->pos.x + inner_radius * x;
            x = fast_cos(angle);
            *(float *)(packet + 0x30) = moby->pos.x + radius * x;
            y = fast_sin(angle);
            *(float *)(packet + 0x14) = moby->pos.y + inner_radius * y;
            y = fast_sin(angle);
            angle = angle + 0.31415927f;
            *(float *)(packet + 0x34) = moby->pos.y + radius * y;
            draw_geometry_quad(packet, 0, 1);
            *(u128_0F0D0 *)(second + 0) = *(u128_0F0D0 *)(second + 0x10);
            *(u128_0F0D0 *)(second + 0x20) = *(u128_0F0D0 *)(second + 0x30);
            *(float *)(second + 0x30) = *(float *)(packet + 0x30);
            *(float *)(second + 0x34) = *(float *)(packet + 0x34);
            *(float *)(second + 0x10) = *(float *)(packet + 0x10);
            *(float *)(second + 0x14) = *(float *)(packet + 0x14);
            draw_geometry_quad(second, 0, 1);
        } while (angle < pi);
        index++;
        band = band + 0.1f;
        green = green - green_step;
        green &= 0xFF;
    } while (band < 1.0f);
}

void register_fading_ring_hit(char *affected_moby, char *effect_moby, char *effect_data) {
    float hit_direction[4];
    float radial_offset[4];
    if (MOBY(affected_moby)->pos.z <
        MOBY(effect_moby)->pos.z + *(float *)(effect_data + 0x10) * 0.3f) {
        float moby_distance = vector_distance_xy(effect_moby + 0x10, affected_moby + 0x10);
        if (absolute_float(moby_distance - *(float *)(effect_data + 8)) < 0.3f) {
            fast_vec_sub(hit_direction, affected_moby + 0x10, effect_moby + 0x10);
            fast_vec_normalize(radial_offset, hit_direction, *(float *)(effect_data + 8) - 0.05f);
            fast_vec_normalize(hit_direction, hit_direction, 1.0f);
            hit_direction[2] = 1.0f;
            spawn_nearby_effect(affected_moby,
                                *(float *)(effect_data + 0x1c),
                                (int)effect_moby,
                                0x10001,
                                radial_offset,
                                hit_direction);
        }
    }
}

void update_fading_ring_effect(unsigned char *moby) {
    char *moby_data = ((char *)MOBY(moby)->pvars);
    float radius_progress;
    char *affected_moby;
    *(float *)(moby_data + 8) = *(float *)(moby_data + 8) + *(float *)(moby_data + 0xC);
    MOBY(moby)->scale = *(float *)(moby_data + 8);
    radius_progress = (*(float *)(moby_data + 8) - *(float *)moby_data) /
                      (*(float *)(moby_data + 4) - *(float *)moby_data);
    if (radius_progress < 0.2f) {
        moby[0x23] = truncate_float_to_s32(radius_progress * 5.0f * 127.0f);
    } else if (radius_progress > 0.8f) {
        moby[0x23] = truncate_float_to_s32((1.0f - radius_progress) * 5.0f * 127.0f);
    } else {
        moby[0x23] = 0x80;
    }
    if (radius_progress >= 1.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    add_draw_callback((void *)draw_fading_ring_effect, moby);
    FUN_L07_0030e0e0_c(*(void **)(D_0013E533 + 0x2E9D), moby, moby_data);
    for (affected_moby = ring_hit_candidate_list_head; affected_moby;
         affected_moby = ((char *)MOBY(affected_moby)->next)) {
        FUN_L07_0030e0e0_c(affected_moby, moby, moby_data);
    }
}
