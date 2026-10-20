/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "qcopy.h"
#include "qzero.h"
extern char *D_L00_0015FFD8_25fcb8 __asm__("D_L00_0015FFD8");
extern char *D_L00_001DD100_25fcb8[] __asm__("D_L00_001DD100") __attribute__((section(".data")));
extern float FUN_001f9b80_25fcb8(void *, void *) __asm__("FUN_001f9b80");

extern struct Hero hero_lui __asm__("D_0013F350") __attribute__((section(".data")));
extern short D_L00_001B0830_lui[]  __asm__("D_L00_001B0830")__attribute__((section(".data")));
int FUN_L00_0025fcb8(char *query_moby, char *target_output, float best) {
    struct Hero *g = &hero_lui;
    char *selected_moby;
    char *candidate_moby;
    int res;
    int i;
    int j;
    float d;

    selected_moby = ((char *)g->moby);
    res = 0;
    if (g->state.control_mode == 0x18 || g->state.current == 0x72) {
        selected_moby = 0;
        res = 2;
    }
    for (i = 1; i <= D_L00_001B0830_lui[0]; i++) {
        candidate_moby = D_L00_0015FFD8_25fcb8 + (D_L00_001B0830_lui[i] << 8);
        if (candidate_moby == 0)
            continue;
        if (*(short *)(candidate_moby + 0xA6) != 0xCB && *(short *)(candidate_moby + 0xA6) != 0x76C)
            continue;
        if (*(unsigned char *)(candidate_moby + 0x20) == 0xFE)
            continue;
        if (*(unsigned char *)(candidate_moby + 0x20) == 0xFD)
            continue;
        if (*(unsigned char *)(candidate_moby + 0x20) != 3)
            continue;
        d = FUN_001f9b80_25fcb8(query_moby + 0x10, candidate_moby + 0x10);
        if (d < best) {
            selected_moby = candidate_moby;
            best = d;
            res = 1;
        }
    }
    for (j = 0; j < 20; j++) {
        candidate_moby = D_L00_001DD100_25fcb8[j];
        if (candidate_moby == 0)
            continue;
        if (*(short *)(candidate_moby + 0xA6) != 0x10E)
            continue;
        if (*(unsigned char *)(candidate_moby + 0x20) == 0xFE)
            continue;
        if (*(unsigned char *)(candidate_moby + 0x20) == 0xFD)
            continue;
        if (*(unsigned char *)(candidate_moby + 0xBC) == 0)
            continue;
        d = FUN_001f9b80_25fcb8(query_moby + 0x10, candidate_moby + 0x10);
        if (d < best) {
            selected_moby = candidate_moby;
            best = d;
            res = 1;
        }
    }
    if (selected_moby != 0) {
        *(char **)(target_output + 0x40) = selected_moby;
        qcopy(target_output, selected_moby + 0x10);
        qcopy(target_output + 0x10, selected_moby + 0x40);
        g = &hero_lui;
        if (selected_moby == ((char *)g->moby)) {
            qcopy(target_output + 0x20, &g->motion.unkC0);
            qcopy(target_output + 0x30, &g->motion.unkD0);
        } else {
            qcopy(target_output + 0x30, target_output);
            *(float *)(target_output + 0x38) += 0.6f;
            qcopy(target_output + 0x20, target_output + 0x30);
        }
    } else {
        *(char **)(target_output + 0x40) = 0;
        qzero(target_output);
        qzero(target_output + 0x10);
        qzero(target_output + 0x30);
        qzero(target_output + 0x20);
    }
    *(int *)(target_output + 0x44) = res;
    return res;
}
extern int FUN_00214720_25ff38(void *, int) __asm__("FUN_00214720");
extern int FUN_L00_00259740_25ff38(void *, void *, int) __asm__("FUN_L00_00259740");
int FUN_L00_0025ff38(char *a, char *o, float best, int *list, int n, float *v, int nv);
int FUN_L00_0025ff38(char *query_moby, char *target_output, float best, int *list, int n, float *v, int nv) {
    struct Hero *g = &hero_lui;
    char *selected_moby;
    char *indexed_candidate;
    char *listed_candidate;
    int res;
    int i;
    int j;
    int k;
    int ok;
    float d;

    selected_moby = (char *)g->moby;
    res = 0;
    if (g->state.control_mode == 0x18 || g->state.current == 0x72) {
        selected_moby = 0;
        res = 2;
    } else if (n > 0) {
        for (k = 0; k < n - 1; k++) {
            if (FUN_00214720_25ff38(&hero_lui.motion.pos, list[k]) != 0) {
                selected_moby = 0;
                res = 2;
                break;
            }
        }
    } else if (v != 0 && nv > 0) {
        if (FUN_L00_00259740_25ff38(&hero_lui.motion.pos, v, nv) == 0) {
            selected_moby = 0;
            res = 2;
        }
    }
    for (i = 1; i <= D_L00_001B0830_lui[0]; i++) {
        indexed_candidate = D_L00_0015FFD8_25fcb8 + (D_L00_001B0830_lui[i] << 8);
        if (indexed_candidate == 0) continue;
        if (*(short *)(indexed_candidate + 0xA6) != 0xCB && *(short *)(indexed_candidate + 0xA6) != 0x76C) continue;
        if (*(unsigned char *)(indexed_candidate + 0x20) == 0xFE) continue;
        if (*(unsigned char *)(indexed_candidate + 0x20) == 0xFD) continue;
        if (*(unsigned char *)(indexed_candidate + 0x20) != 3) continue;
        ok = 1;
        d = FUN_001f9b80_25fcb8(query_moby + 0x10, indexed_candidate + 0x10);
        if (best <= d) {
            ok = 0;
        } else if (n > 0) {
            for (k = 0; k < n - 1; k++) {
                if (FUN_00214720_25ff38(indexed_candidate + 0x10, list[k]) != 0) {
                    ok = 0;
                    break;
                }
            }
        } else if (v != 0 && nv > 0) {
            if (FUN_L00_00259740_25ff38(indexed_candidate + 0x10, v, nv) == 0) ok = 0;
        }
        if (ok) {
            selected_moby = indexed_candidate;
            res = 1;
            best = d;
        }
    }
    for (j = 0; j < 20; j++) {
        listed_candidate = D_L00_001DD100_25fcb8[j];
        if (listed_candidate == 0) continue;
        if (*(short *)(listed_candidate + 0xA6) != 0x10E) continue;
        if (*(unsigned char *)(listed_candidate + 0x20) == 0xFE) continue;
        if (*(unsigned char *)(listed_candidate + 0x20) == 0xFD) continue;
        if (*(unsigned char *)(listed_candidate + 0xBC) == 0) continue;
        ok = 1;
        d = FUN_001f9b80_25fcb8(query_moby + 0x10, listed_candidate + 0x10);
        if (best <= d) {
            ok = 0;
        } else if (n > 0) {
            for (k = 0; k < n - 1; k++) {
                if (FUN_00214720_25ff38(listed_candidate + 0x10, list[k]) != 0) {
                    ok = 0;
                    break;
                }
            }
        } else if (v != 0 && nv > 0) {
            if (FUN_L00_00259740_25ff38(listed_candidate + 0x10, v, nv) == 0) ok = 0;
        }
        if (ok) {
            selected_moby = listed_candidate;
            res = 1;
            best = d;
        }
    }
    if (selected_moby != 0) {
        *(char **)(target_output + 0x40) = selected_moby;
        qcopy(target_output, selected_moby + 0x10);
        qcopy(target_output + 0x10, selected_moby + 0x40);
        g = &hero_lui;
        if (selected_moby == (char *)g->moby) {
            qcopy(target_output + 0x20, &g->motion.unkC0);
            qcopy(target_output + 0x30, &g->motion.unkD0);
        } else {
            qcopy(target_output + 0x30, target_output);
            *(float *)(target_output + 0x38) += 0.6f;
            qcopy(target_output + 0x20, target_output + 0x30);
        }
    } else {
        *(char **)(target_output + 0x40) = 0;
        qzero(target_output);
        qzero(target_output + 0x10);
        qzero(target_output + 0x30);
        qzero(target_output + 0x20);
    }
    *(int *)(target_output + 0x44) = res;
    return res;
}
typedef float M4_260400[16] __attribute__((aligned(16)));
extern char *FUN_L00_002603d0_c(char *) __asm__("FUN_L00_002603d0");
void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
void matrix_to_euler(void *, void *) __asm__("FUN_00214598");
int FUN_L00_00260400(int u0, char *a, void *b, void *c, float *v, void *out);
int FUN_L00_00260400(int u0, char *a, void *b, void *input_rotation, float *v, void *out) {
    M4_260400 basis_rotation_matrix, composed_rotation_matrix;
    char *o = FUN_L00_002603d0_c(a);
    if (o == 0)
        return 0;
    euler_rotation_matrix(basis_rotation_matrix, o);
    add_vector_xyz(v, b, o + 0x10);
    subtract_vector_xyz(v, v, a + 0x10);
    transform_vector_by_basis(v, v, basis_rotation_matrix);
    add_vector_xyz(v, v, a + 0x10);
    euler_rotation_matrix(composed_rotation_matrix, input_rotation);
    multiply_matrix(composed_rotation_matrix, basis_rotation_matrix, composed_rotation_matrix);
    matrix_to_euler(composed_rotation_matrix, out);
    return 1;
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
typedef float V4_2604f0[4] __attribute__((aligned(16)));
u8 *FUN_L00_002603d0_002604f0(void *) __asm__("FUN_L00_002603d0");
void FUN_001f9cf8_002604f0(void *, void *, void *) __asm__("FUN_001f9cf8");
void FUN_001f9a10_002604f0(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001fa050_002604f0(void *, void *) __asm__("FUN_001fa050");
void FUN_001fa298_002604f0(void *, void *) __asm__("FUN_001fa298");
void FUN_001fa378_002604f0(void *, void *, void *) __asm__("FUN_001fa378");
void FUN_00214598_002604f0(void *, void *) __asm__("FUN_00214598");
void FUN_L00_00260668_002604f0(void *, void *, void *, void *, void *,
                               void *) __asm__("FUN_L00_00260668");
void FUN_L00_0025f730_002604f0(void *, float) __asm__("FUN_L00_0025f730");
int FUN_L00_002604f0(u8 *a, u8 *b, void *c, void *d, void *e, void *f);
int FUN_L00_002604f0(u8 *a, u8 *b, void *position, void *rotation, void *transformed_position, void *transformed_rotation) {
    float m2[16] __attribute__((aligned(16)));
    float m[16] __attribute__((aligned(16)));
    V4_2604f0 translation_adjustment;
    u8 *r = FUN_L00_002603d0_002604f0(b);
    if (!r)
        return 0;
    FUN_001f9cf8_002604f0(transformed_position, position, b + 0xC0);
    FUN_001f9a10_002604f0(transformed_position, transformed_position, b + 0x10);
    FUN_001fa050_002604f0(m, rotation);
    FUN_001fa298_002604f0(m2, b + 0xC0);
    FUN_001fa378_002604f0(m, m2, m);
    FUN_00214598_002604f0(m, transformed_rotation);
    if (*(int *)(r + 0x3C) & 4) {
        FUN_001f9a10_002604f0(transformed_position, transformed_position, r + 0x10);
        FUN_L00_00260668_002604f0(a, b, transformed_position, transformed_rotation, position, rotation);
        return 1;
    }
    if (a[0x22] && a[0x22] < b[0x22]) {
        qcopy(translation_adjustment, r + 0x10);
        FUN_L00_0025f730_002604f0(translation_adjustment, 1.0f);
        FUN_001f9a10_002604f0(transformed_position, transformed_position, translation_adjustment);
    }
    return 1;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_002616E0), where it is exact; names translated to the US level program. */

extern int FUN_L00_002603d0(char *);
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void matrix_to_euler(void *, void *) __asm__("FUN_00214598");

int FUN_L00_00260668(int x, char *o, float *a, float *c, float *b, float *d);
int FUN_L00_00260668(int x, char *reference_object, float *input_position, float *input_rotation, float *b, float *output_rotation) {
    float transposed_rotation_matrix[16];
    float v[4];
    float w[4];
    float pad[12];
    if (FUN_L00_002603d0(reference_object) == 0)
        return 0;
    subtract_vector_xyz(v, input_position, reference_object + 0x10);
    transpose_rotation_matrix(transposed_rotation_matrix, reference_object + 0xC0);
    transform_vector(b, v, transposed_rotation_matrix);
    euler_rotation_matrix(w, input_rotation);
    multiply_matrix(w, transposed_rotation_matrix, w);
    matrix_to_euler(w, output_rotation);
    return 1;
}
typedef float M4[16] __attribute__((aligned(16)));
void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
void matrix_to_euler(void *, void *) __asm__("FUN_00214598");
#include "qcopy.h"
void update_entity_matrix(char *out, OvlQuad *pos, void *m1, void *m2) __asm__("FUN_L00_00260738");
void update_entity_matrix(char *out, OvlQuad *pos, void *previous_rotation, void *current_rotation) {
    M4 transposed_previous_rotation_matrix, b, relative_rotation_matrix;
    euler_rotation_matrix(transposed_previous_rotation_matrix, previous_rotation);
    transpose_rotation_matrix(transposed_previous_rotation_matrix, transposed_previous_rotation_matrix);
    euler_rotation_matrix(b, current_rotation);
    multiply_matrix(relative_rotation_matrix, transposed_previous_rotation_matrix, b);
    matrix_to_euler(relative_rotation_matrix, out);
    qcopy(out + 0x10, pos);
}
extern u8 level_available[] __asm__("D_0013DD40");
extern s32 D_0013D510[] __asm__("D_0013D510");
void FUN_L00_00262d38(s32);
void unlock_planet(s32 id) __asm__("FUN_L00_002607d0");
void unlock_planet(s32 id) {
    s32 available_planet_count, i;
    available_planet_count = 0;
    if (level_available[id] == 0) {
        for (i = 0; i < 20; i++) {
            if (level_available[i])
                available_planet_count++;
        }
        level_available[id] = 1;
        D_0013D510[available_planet_count] = id;
        if (id != *(volatile s32 *)&current_level_index)
            FUN_L00_00262d38(id);
    }
}
/* Gives item ID to the player: plays its sound, raises its count and records it in the slot lists. */
/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_002618D8), where it is exact; names translated to the US level program. */


typedef struct {
    char pad[8];
    int t;
    char pad2[0x40];
} ItemDef;

extern ItemDef item_defs[] __asm__("D_L00_00179AC0") __attribute__((section(".data")));
extern int weapon_ammo_counts[] __asm__("D_0013D428");
extern int pause_iop_sprite_lists[] __asm__("D_00141EA0");
extern int D_L00_001B0340[] __attribute__((section(".data")));
extern int D_L00_001C3E98[] __attribute__((section(".data")));
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern unsigned char D_0013D4C0[];
extern unsigned char items_acquired[] __asm__("D_0013D4E8");
extern int D_00141408 __attribute__((section(".data")));
extern int D_00141410 __attribute__((section(".data")));
extern int D_00141414 __attribute__((section(".data")));
extern void queue_dialogue_message(int arg0, int arg1) __asm__("FUN_L00_00263d40");

extern u8 D_0015EDD0_257218[] __asm__("D_0015EDD0") __attribute__((section(".sdata")));
void FUN_L00_00260860(int id, int flag);
void FUN_L00_00260860(int id, int flag) {
    unsigned char *pa = &D_0013D4C0[id];
    unsigned char *pb = &items_acquired[id];
    int idx = id + 0x25;
    int *dialogue_message_id_ptr;
    int i;

    if (*pa == 0) {
        idx = id;
    }
    dialogue_message_id_ptr = &D_L00_001B0340[idx];
    *pb = 1;
    if (*dialogue_message_id_ptr != -1) {
        queue_dialogue_message(*dialogue_message_id_ptr, scale_game_frames(0x12C));
    }
    if (*pa == 0) {
        VendorItemInfo *r;
        *pa = 1;
        r = &D_L00_001C40B0[id];
        *pb = 1;
        if (r->h8 != 0) {
            int *ammo_count_ptr = &weapon_ammo_counts[id];
            int v = r->h12;
            if (v < *ammo_count_ptr) {
                v = *ammo_count_ptr;
            }
            *ammo_count_ptr = v;
        }
        for (i = 0; i < 20; i++) {
            if (id != 0 && D_L00_001C3E98[i] == id) {
                int j;
                for (j = 0;; j++) {
                    unsigned char b = D_0015EDD0_257218[j];
                    if ((b & 0x3F) == id || b == 0xFF) {
                        D_0015EDD0_257218[j] = id | 0x40;
                        break;
                    }
                }
                break;
            }
        }
        if (flag != 0) {
            int t = item_defs[id].t;
            if (t == 0) {
                D_00141408 = id;
            } else if (t == 3) {
                D_00141414 = id;
            } else if (t == 2) {
                D_00141410 = id;
            }
        }
        if (item_defs[id].t == 0) {
            for (i = 0; i < 8 && pause_iop_sprite_lists[i] != 0; i++) {
            }
            if (i < 8) {
                pause_iop_sprite_lists[i] = id;
            }
        }
    }
}
/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00261B00.c: func_L00_00261B00). */
#include "rnc/gameplay/entities/moby.h"

extern char *D_L00_001B04B0[];
extern char D_L00_001600A0[];
extern char D_L00_001600B0[];
extern char D_L00_001600C0[];
extern f32 frame_time_macro __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern f32 frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern s32 alternate_animation_sequence_macro __asm__("D_0015EE20") __attribute__((section(".sdata")));
extern s32 D_0015EE2C_macro __asm__("D_0015EE2C") __attribute__((section(".sdata")));
extern int D_0015EE28_macro __asm__("D_0015EE28") __attribute__((section(".sdata")));
extern s32 D_0015EEA0_macro __asm__("D_0015EEA0") __attribute__((section(".sdata")));
extern int D_L00_0015F598;
extern unsigned char D_L00_0015FC88_macro[] __asm__("D_L00_0015FC88") __attribute__((section(".sdata")));
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern void debug_print() __asm__("FUN_001e93b0");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern int FUN_L00_002595a0(float *, float *, float, float, float);
extern int FUN_L00_00261b48(int, void *, void *, void *);
extern void FUN_L00_002a7438(void *, void *, void *, int, int, int);

void FUN_L00_00260a88(struct Moby *moby, int min_bolts, int max_bolts, int flags, int path_index)
{
    float base_velocity[4], position[4], velocity[4], spread[4], landing[4], path_point[4], quarter[4];
    float first_root, second_root;
    int counted;
    int total, remaining, sum, rate, roots, ones;
    int up_total, up_fives, down_total, up_twenties, down_twenties, down_fives, up_ones, down_fifties;
    int down_ones, up_fifties, twenties, fives, fifties;
    unsigned char *aim;
    float time;
    float gravity, rise, height, angle;

    fives = 0;
    twenties = 0;
    fifties = 0;
    clear_u64_value(base_velocity);
    if (!(flags & 0x20)) {
        if (moby->mission != 0xFF
            && (unsigned)((unsigned short)moby->oclass - 0x1F4) >= 0x29) {
            D_L00_0015F598 = D_L00_0015FC88_macro[moby->mission] == 0xFF;
        }
    }
    counted = 0;
    if (D_L00_0015F598) {
        if (!(flags & 0x20)) {
            counted = 1;
        }
    }
    if (flags == 0) {
        return;
    }
    if (min_bolts == 0 && max_bolts == 0) {
        return;
    }
    if (flags & 4) {
        aim = (unsigned char *)suck_get_aim_height(moby);
        if (aim) {
            qcopy(base_velocity, aim + 0x20);
        }
    }
    if (min_bolts <= 0) {
        min_bolts = 1;
    }
    total = random_integer_below(max_bolts - min_bolts + 1) + min_bolts;
    if (D_0015EEA0_macro || alternate_animation_sequence_macro) {
        total <<= 1;
    }
    if (total < 0x1F4) {
        if (counted && D_0015EE28_macro) {
            rate = (int)((float)scale_game_frames(0x3C) * 60.0f) * D_0015EE2C_macro / D_0015EE28_macro;
            if (D_0015EEA0_macro || alternate_animation_sequence_macro) {
                rate /= 2;
            }
            if (rate < 0x96) {
                if (rate < 0x5B) {
                    debug_print(D_L00_001600A0);
                    total *= 5;
                } else {
                    debug_print(D_L00_001600B0, (0xBE - rate) / 20);
                    total *= (0xBE - rate) / 20;
                }
                if (D_0015EEA0_macro) {
                    if (total > 0x3E8) total = 0x3E8;
                } else {
                    if (total > 0x1F4) total = 0x1F4;
                }
            } else if (rate >= 0x12D) {
                if (rate >= 0x160) {
                    debug_print(D_L00_001600C0, 0x3FB99999A0000000L);
                    total /= 10;
                } else {
                    debug_print(D_L00_001600C0, convert_integer_to_float(0x168 - rate) / 20.0f);
                    total *= (0x168 - rate) / 80;
                }
                if (total <= 0) {
                    total = 1;
                }
            }
        }
    }
    remaining = total;
    if (total >= 0x12C) {
        do {
            remaining -= 50;
            fifties++;
        } while (remaining >= 0x12C);
    }
    while (remaining >= 0xC8) {
        remaining -= 20;
        twenties++;
    }
    if (remaining >= 0x1E) {
        do {
            remaining -= 25;
            twenties++;
            fives++;
        } while (remaining >= 0x1E);
    }
    if (remaining >= 7) {
        do {
            remaining -= 5;
            fives++;
        } while (remaining >= 7);
    }
    ones = remaining;
    sum = fifties + twenties + fives + ones;
    if (sum >= 4 && (flags & 8)) {
        down_ones = 0;
        down_fives = 0;
        down_twenties = 0;
        down_fifties = 3;
        up_ones = 3;
        up_fives = 0;
        up_twenties = 0;
        up_fifties = 0;
        down_total = 0x96;
        up_total = 3;
        if (ones + fives + twenties + fifties >= 4) {
        if (total < down_total) {
            do {
                if (down_fifties) {
                    down_fifties--;
                    down_twenties++;
                } else if (down_twenties) {
                    down_twenties--;
                    down_fives++;
                } else if (down_fives) {
                    down_fives--;
                    down_ones++;
                }
                down_total = down_fifties * 50 + down_twenties * 20 + down_fives * 5 + down_ones;
            } while (total < down_total && down_total >= 4);
        }
        up_total = 3;
        if (up_total < total) {
            do {
                if (up_ones) {
                    up_ones--;
                    up_fives++;
                } else if (up_fives) {
                    up_fives--;
                    up_twenties++;
                } else if (up_twenties) {
                    up_twenties--;
                    up_fifties++;
                }
                up_total = up_fifties * 50 + up_twenties * 20 + up_fives * 5 + up_ones;
            } while (up_total < total && up_total < 0x96);
        }
        }
        if ((down_total - total) < (total - up_total)) {
            ones = down_ones;
            fives = down_fives;
            twenties = down_twenties;
            fifties = down_fifties;
        } else {
            ones = up_ones;
            fives = up_fives;
            twenties = up_twenties;
            fifties = up_fifties;
        }
    }
    while (ones || fives || twenties || fifties) {
        qcopy(position, &moby->pos);
        position[2] = position[2] + 0.5f;
        angle = random_angle_radians();
        velocity[0] = fast_cos(angle) * (random_float_between(0.0f, 3.0f) * frame_time_macro);
        velocity[1] = fast_sin(angle) * (random_float_between(0.0f, 3.0f) * frame_time_macro);
        velocity[2] = 0.0f;
        velocity[2] = random_float_between(3.7f, 6.0f) * frame_time_macro;
        spread[0] = velocity[0];
        spread[1] = velocity[1];
        spread[2] = 0.0f;
        fast_vec_normalize(spread, spread, 0.5f);
        add_vector_xyz(position, position, spread);
        if (path_index >= 0) {
            gravity = frame_time_sq_macro * 10.8f * -0.5f;
            rise = velocity[2] - gravity;
            height = position[2] - *(float *)(D_L00_001B04B0[path_index] + 0x18);
            time = 120.0f;
            roots = FUN_L00_002595a0(&first_root, &second_root, gravity, rise, height);
            if (1.0f <= (float)roots) {
                if (0.0f < first_root) {
                    if (first_root < time) {
                        time = first_root;
                    }
                }
            }
            fast_vec_scale(landing, velocity, time);
            add_vector_xyz(landing, landing, &moby->pos);
            roots = FUN_L00_00261b48(path_index, &moby->pos, landing, path_point);
            subtract_vector_xyz(landing, path_point, position);
            landing[2] = 0.0f;
            fast_vec_normalize(quarter, landing, 0.25f);
            subtract_vector_xyz(landing, landing, quarter);
            fast_vec_scale(landing, landing, 1.0f / time);
            velocity[0] = landing[0];
            velocity[1] = landing[1];
        }
        add_vector_xyz(velocity, velocity, base_velocity);
        if (fifties) {
            FUN_L00_002a7438(moby, position, velocity, flags, 50, counted);
            fifties--;
        } else if (twenties) {
            FUN_L00_002a7438(moby, position, velocity, flags, 20, counted);
            twenties--;
        } else if (fives) {
            FUN_L00_002a7438(moby, position, velocity, flags, 5, counted);
            fives--;
        } else {
            FUN_L00_002a7438(moby, position, velocity, flags, 1, counted);
            ones--;
        }
    }
}
