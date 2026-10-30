/* Level 16 moby class linked_pair_ribbons (0x228): pairs of mobys joined by ribbons, off while a wall_button of the group is pressed. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"

/* Updates a moby: waits for trigger, registers a draw callback and a sound handle. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0870), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int  voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
int         is_linked_pair_ribbon_enabled(void *mv) __asm__("FUN_L16_002cf738");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern void FUN_L16_002cf7a8_u(void) __asm__("FUN_L16_002cf7a8");
extern void add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern void func_L16_002D0990_2D0870(void *) __asm__("FUN_L16_002cf5c8");
extern int  allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

void update_linked_pair_ribbons(struct Moby *moby) __asm__("FUN_L16_002cf4a8");

void update_linked_pair_ribbons(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE_LINKED_PAIRS = 0,
        MOBY_STATE_UPDATE_DRAW_AND_SOUND = 1
    };
    int *data = (int *)moby->pvars;
    if ((moby->flags & 2) == 0) {
        unsigned char state = moby->state;
        switch (state) {
        case MOBY_STATE_INITIALIZE_LINKED_PAIRS:
            if (data[0] == 0) {
                func_L16_002D0990_2D0870(moby);
                moby->state = 1;
            }
            break;
        case MOBY_STATE_UPDATE_DRAW_AND_SOUND:
            if (is_linked_pair_ribbon_enabled(moby) != 0) {
                data[2] = state;
                add_draw_callback(FUN_L16_002cf7a8_u, moby);
                if (voice_is_playing(moby, data[3]) == 0) {
                    data[3] = allocate_voice_for_target_entry(0, 4, moby);
                }
            } else if (data[2] != 0) {
                int voice_slot_index = data[3];
                data[2] = 0;
                if (voice_slot_index != -1) {
                    char *voice_entry_base = D_0013E533 + 0x1D + voice_slot_index * 0x70;
                    if (*(char **)(voice_entry_base + 0x88) == moby &&
                        *(unsigned char *)(voice_entry_base + 0x74) != 0) {
                        release_voice_slot(voice_slot_index);
                    }
                }
                data[3] = -1;
            }
            break;
        }
    }
}

extern char *D_L16_0015FFD8;
extern int  *D_L16_001ABCC0_c[] __asm__("D_L16_001ABCC0");
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0990), where it is exact; names translated to the US level program. */

extern int FUN_L16_002cf678_u(char *) __asm__("FUN_L16_002cf678");

void initialize_linked_pair_group(struct Moby *moby) __asm__("FUN_L16_002cf5c8");

void initialize_linked_pair_group(struct Moby *moby) {
    short *group_entry = D_L16_001ABCC0_c[moby->group];
    if (group_entry != 0) {
        do {
            char *other = D_L16_0015FFD8 + (((unsigned short)*group_entry & 0x7FFF) << 8);
            if (*(short *)(other + 0xA6) == 0x228) {
                int *data;
                if (moby != other) {
                    *(unsigned short *)(other + 0x34) |= 2;
                }
                data = *(int **)(other + 0x78);
                *data = FUN_L16_002cf678_u(other);
            }
        } while (*group_entry++ >= 0);
    }
}
/* first other moby of type 0x228 in m's list whose 0x18 is within 0.1 of m's */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0A40), where it is exact; names translated to the US level program. */

extern short *D_L16_001ABFC0_2d0a40[] __asm__("D_L16_001ABCC0");

int find_linked_pair_partner_by_height(struct Moby *moby) __asm__("FUN_L16_002cf678");

int find_linked_pair_partner_by_height(struct Moby *moby) {
    short *group_entry = D_L16_001ABFC0_2d0a40[moby->group];
    if (group_entry == 0)
        return 0;
    while (1) {
        char *candidate_moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        if (MOBY(candidate_moby)->oclass == 0x228 && moby != candidate_moby) {
            if (absolute_float(MOBY(candidate_moby)->pos.z - moby->pos.z) < 0.1f) {
                return (int)candidate_moby;
            }
        }
        if (*group_entry++ < 0)
            return 0;
    }
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0B00), where it is exact; names translated to the US level program. */

extern int *D_L16_001ABCC0[];

int is_linked_pair_ribbon_enabled(void *mv) {
    struct Moby    *moby = mv;
    unsigned short *group_entry = (unsigned short *)D_L16_001ABCC0[moby->group];
    if (group_entry == 0)
        return 1;
    for (;;) {
        unsigned char *other = D_L16_0015FFD8 + ((*group_entry & 0x7FFF) << 8);
        if (*(short *)(other + 0xA6) == 0x222) {
            if (other[0xBC] != 0)
                return 0;
        }
        if ((short)*group_entry++ < 0)
            return 1;
    }
}

extern int D_L16_0015F5CC;
/* Draw each linked pair once per generation. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0B70), where it is exact; names translated to the US level program. */

extern int *D_L16_001ABCC0_c3[] __asm__("D_L16_001ABCC0");
void        draw_linked_pair_ribbons(char *moby, char *partner) __asm__("FUN_L16_002cf850");

void draw_linked_pair_group_ribbons(struct Moby *moby) __asm__("FUN_L16_002cf7a8");

void draw_linked_pair_group_ribbons(struct Moby *moby) {
    short *group_entry = (short *)D_L16_001ABCC0_c3[moby->group];
    if (group_entry) {
        do {
            char *group_moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
            if (MOBY(group_moby)->oclass == 0x228) {
                char *data = ((char *)MOBY(group_moby)->pvars);
                int   generation = D_L16_0015F5CC;
                if (*(int *)(data + 4) != generation) {
                    char *partner = *(char **)data;
                    if (partner) {
                        char *otherData = *(char **)(partner + 0x78);
                        *(int *)(data + 4) = generation;
                        *(int *)(otherData + 4) = generation;
                        draw_linked_pair_ribbons(group_moby, partner);
                    }
                }
            }
        } while (*group_entry++ >= 0);
    }
}

#include "qcopy.h"

extern s64 get_effect_texture(s32) __asm__("func_001F44B8");

/* Draws the textured quad that links a moby to its partner, four times, each one a step higher than the last. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0C18), where it is exact; names translated to the US level program. */

typedef struct {
    float point[4][4];
    int   color[4];
    struct {
        float u, v;
    } uv[4];
    long zero, texture, flags, mode;
} L16RibbonPacket;

extern float linked_pair_ribbon_vertex_positions[][4] __asm__("D_L16_001D3170");
extern float linked_pair_ribbon_uvs[][2] __asm__("D_L16_001D31B0");
extern int   linked_pair_ribbon_alpha_blend_b_selector __asm__("D_L16_001619EC")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_alpha_blend_c_selector __asm__("D_L16_001619F0")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_alpha_blend_a_selector __asm__("D_L16_001619E8")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_alpha_blend_d_selector __asm__("D_L16_001619F4")
    __attribute__((sda)); /* no foreign declaration */
extern float linked_pair_ribbon_origin_z_offset __asm__("D_L16_001619F8")
    __attribute__((sda)); /* no foreign declaration */
extern float linked_pair_ribbon_pass_z_step __asm__("D_L16_001619FC")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_texture_id __asm__("D_L16_00161A00")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_vertex_color __asm__("D_L16_00161A04")
    __attribute__((sda)); /* no foreign declaration */
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");

/* Draws a four-pass textured ribbon quad from the moby toward its partner moby. */
void draw_linked_pair_ribbons(char *moby, char *partner) {
    L16RibbonPacket packet;
    float           frame[4][4]; /* direction, side, up, origin */
    int             color;
    int             corner;
    int             pass;

    fast_vec_sub(frame[0], partner + 0x10, moby + 0x10);
    clear_u64_value(frame[2]);
    frame[2][2] = 1.0f;
    fast_vec_cross(frame[1], frame[0], frame[2]);
    qcopy(frame[3], moby + 0x10);
    frame[3][2] += linked_pair_ribbon_origin_z_offset;
    frame[3][3] = 1.0f;
    packet.texture = get_effect_texture(linked_pair_ribbon_texture_id);
    packet.mode = (long)linked_pair_ribbon_alpha_blend_a_selector |
                  ((long)linked_pair_ribbon_alpha_blend_b_selector << 2) |
                  ((long)linked_pair_ribbon_alpha_blend_c_selector << 4) |
                  ((long)linked_pair_ribbon_alpha_blend_d_selector << 6) | 0x8000000000L;
    packet.flags = 0xFF9000000260L;
    packet.zero = 0;
    color = linked_pair_ribbon_vertex_color;
    for (corner = 0; corner < 4; corner++) {
        packet.uv[corner].u = linked_pair_ribbon_uvs[corner][0];
        packet.uv[corner].v = linked_pair_ribbon_uvs[corner][1];
        packet.color[corner] = color;
        qcopy(packet.point[corner], linked_pair_ribbon_vertex_positions[corner]);
    }
    for (pass = 0; pass < 4; pass++) {
        draw_geometry_quad(&packet, frame[0], 0);
        frame[3][2] += linked_pair_ribbon_pass_z_step;
    }
}
