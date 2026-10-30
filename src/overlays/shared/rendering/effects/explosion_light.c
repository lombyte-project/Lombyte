/* Shared moby class 0x27F: an animated point light (blast flashes), and the function that spawns it. */
#include "types.h"
#include "rnc/overlay/point_light.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    float x, y, z, w;
} Vx __attribute__((aligned(16)));

extern void mark_moby_for_removal() __asm__("FUN_0020c828");

void *create_moby(int) __asm__("FUN_0020c4f8");
void  upd_a(void *) __asm__("FUN_L00_00250df8");
void  FUN_L00_0025d1b8_2d3838(void *) __asm__("FUN_L00_0025d1b8");

extern int FUN_L00_001fefc8(void *);
extern int allocate_colored_effect_slot(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");

unsigned char *
mine_camera_blast(unsigned char *s, void *b, int c, void *d) __asm__("FUN_L00_002d3838");

unsigned char *mine_camera_blast(unsigned char *blast_settings,
                                 void          *spawn_position,
                                 int            position_reference_moby,

                                 void *position_offset_override) {
    unsigned char *spawned_moby = create_moby(0x27F);
    if (spawned_moby) {
        unsigned char *blast_vars;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->flags |= 1;
        spawned_moby[0x31] = 1;
        blast_vars = ((unsigned char *)MOBY(spawned_moby)->pvars);
        spawned_moby[0x20] = 0;
        spawned_moby[0xBC] = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        if (position_offset_override)
            qcopy(blast_vars, position_offset_override);
        else
            qcopy(blast_vars, blast_settings);
        *(int *)(blast_vars + 0x20) = position_reference_moby;
        MOBY(spawned_moby)->draw_dist = blast_settings[0x3C];
        blast_vars[0x24] = blast_settings[0x10];
        blast_vars[0x25] = blast_settings[0x11];
        blast_vars[0x26] = blast_settings[0x12];
        blast_vars[0x27] = blast_settings[0x13];
        blast_vars[0x28] = blast_settings[0x14];
        blast_vars[0x29] = blast_settings[0x15];
        blast_vars[0x68] = blast_settings[0x3D];
        *(unsigned short *)(blast_vars + 0x6C) = *(unsigned short *)(blast_settings + 0x40);
        blast_vars[0x2A] = blast_settings[0x16];
        blast_vars[0x2B] = blast_settings[0x17];
        blast_vars[0x2C] = blast_settings[0x18];
        blast_vars[0x2D] = blast_settings[0x19];
        blast_vars[0x2E] = blast_settings[0x1A];
        blast_vars[0x69] = blast_settings[0x3E];
        *(unsigned short *)(blast_vars + 0x6E) = *(unsigned short *)(blast_settings + 0x42);
        blast_vars[0x2F] = blast_settings[0x1B];
        blast_vars[0x30] = blast_settings[0x1C];
        blast_vars[0x31] = blast_settings[0x1D];
        blast_vars[0x32] = blast_settings[0x1E];
        blast_vars[0x33] = blast_settings[0x1F];
        blast_vars[0x6A] = blast_settings[0x3F];
        *(unsigned short *)(blast_vars + 0x70) = *(unsigned short *)(blast_settings + 0x44);
        *(float *)(blast_vars + 0x34) = *(float *)(blast_settings + 0x20);
        *(float *)(blast_vars + 0x38) = *(float *)(blast_settings + 0x24);
        *(float *)(blast_vars + 0x3C) = *(float *)(blast_settings + 0x28);
        *(float *)(blast_vars + 0x40) = *(float *)(blast_settings + 0x2C);
        *(float *)(blast_vars + 0x44) = *(float *)(blast_settings + 0x30);
        blast_vars[0x6B] = blast_settings[0x48];
        *(unsigned short *)(blast_vars + 0x72) = *(unsigned short *)(blast_settings + 0x46);
        *(int *)(blast_vars + 0x48) = *(int *)(blast_settings + 0x34);
        *(int *)(blast_vars + 0x4C) = *(int *)(blast_settings + 0x38);
        blast_vars[0x1C] = blast_settings[0x49];
        *(int *)(blast_vars + 0x64) = -1;
        upd_a(spawned_moby);
        FUN_L00_0025d1b8_2d3838(spawned_moby);
    }
    return spawned_moby;
}

/* moby class 639 update: runs its state machine and eases its tracked values toward their targets */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D4EC0), where it is exact; names translated to the US level program. */

void update_animated_point_light_moby(unsigned char *moby) __asm__("FUN_L00_002d3a10");

void update_animated_point_light_moby(unsigned char *moby) {
    unsigned char *vars;
    Vx             tp;
    Vx             base_position;
    Vx             position_offset;
    unsigned char *moby_position;
    void          *base_position_ptr;
    void          *position_offset_ptr;
    int            radius_reset_this_update;
    int            first_color_randomization_timer_bypass = 0;
    int            second_color_randomization_timer_bypass = 0;
    int            third_color_randomization_timer_bypass = 0;
    int            sphere_check_result_or_light_slot_index;

    if (moby == 0)
        return;
    vars = ((unsigned char *)MOBY(moby)->pvars);
    radius_reset_this_update = 0;
    if (vars == 0)
        return;
    if (moby[0x20] == 0) {
        unsigned char b1c;
        moby[0x30] = 0xFF;
        b1c = vars[0x1C];
        moby[0xBC] = b1c;
        qcopy(vars + 0x10, moby + 0x10);
        *(int *)(vars + 0x50) = -1;
        vars[0x1C] = b1c;
        *(int *)(vars + 0xC) = *(int *)(vars + 0x4C);
        *(short *)(vars + 0x74) = *(unsigned short *)(vars + 0x6C);
        *(short *)(vars + 0x76) = *(unsigned short *)(vars + 0x6E);
        *(short *)(vars + 0x78) = *(unsigned short *)(vars + 0x70);
        *(short *)(vars + 0x7A) = *(unsigned short *)(vars + 0x72);
        *(float *)(vars + 0x54) = convert_integer_to_float(vars[0x25]) / 100.0f;
        *(float *)(vars + 0x58) = convert_integer_to_float(vars[0x2A]) / 100.0f;
        {
            float initial_color_channel_value = convert_integer_to_float(vars[0x2F]) / 100.0f;
            *(float *)(vars + 0x60) = *(float *)(vars + 0x34);
            *(float *)(vars + 0x5C) = initial_color_channel_value;
        }
        if (*(int *)(vars + 0x64) != -1) {
            *(char **)(vars + 0x20) = moby_table + (*(int *)(vars + 0x64) << 8);
        }
        if (*(float *)(vars + 0x40) < 0.0f)
            *(float *)(vars + 0x40) = -*(float *)(vars + 0x40);
        if (*(float *)(vars + 0x44) < 0.0f)
            *(float *)(vars + 0x44) = -*(float *)(vars + 0x44);
        moby[0x20] = 1;
    }
    if (moby[0xBC] != 0) {
        moby[0xBC] = moby[0xBC] - 1;
        return;
    }
    if (*(int *)(vars + 0x48) != -1 && fast_dec_timer((int *)(vars + 0x48))) {
        if (*(int *)(vars + 0x50) != -1) {
            release_point_light_slot(*(int *)(vars + 0x50));
            *(int *)(vars + 0x50) = -1;
        }
        mark_moby_for_removal(moby);
        return;
    }
    {
        unsigned char *position_or_sphere_ptr = moby + 0x10;
        qcopy(&tp, position_or_sphere_ptr);
        tp.w = *(float *)(vars + 0x60);
        moby_position = position_or_sphere_ptr;
        position_or_sphere_ptr = (unsigned char *)&tp;
        sphere_check_result_or_light_slot_index =
            fast_bsphere_check(position_or_sphere_ptr, 255.0f);
    }
    if (sphere_check_result_or_light_slot_index != -1) {
        if (*(unsigned char **)(vars + 0x20) != 0) {
            qcopy(&base_position, *(unsigned char **)(vars + 0x20) + 0x10);
        } else {
            qcopy(&base_position, vars + 0x10);
        }
        base_position_ptr = &base_position;
        if (!(*(int *)(vars + 0x4C) & 0x800000)) {
            if (*(int *)(vars + 0x4C) & 0x100000) {
                if (*(int *)(vars + 0x4C) & 0x200000) {
                    position_offset.x =
                        random_float_between(-*(float *)(vars + 0x0), *(float *)(vars + 0x0));
                    position_offset.y =
                        random_float_between(-*(float *)(vars + 0x4), *(float *)(vars + 0x4));
                    position_offset.z =
                        random_float_between(-*(float *)(vars + 0x8), *(float *)(vars + 0x8));
                    position_offset_ptr = &position_offset;
                } else {
                    int   c = *(int *)(vars + 0xC);
                    void *p = &position_offset;
                    *(int *)(vars + 0xC) = 0;
                    fast_vec_scale(p, vars, random_float_between(*(float *)(vars + 0xC), 1.0f));
                    *(int *)(vars + 0xC) = c;
                    position_offset_ptr = p;
                }
            } else {
                int   c = *(int *)(vars + 0xC);
                void *p = &position_offset;
                *(int *)(vars + 0xC) = 0;
                qcopy(p, vars);
                *(int *)(vars + 0xC) = c;
                position_offset_ptr = p;
            }
            if (*(int *)(vars + 0x4C) & 0x400000) {
                if (*(unsigned char **)(vars + 0x20) != 0) {
                    transform_vector_by_basis(position_offset_ptr,
                                              position_offset_ptr,
                                              *(unsigned char **)(vars + 0x20) + 0xC0);
                }
            }
            fast_vec_add(moby_position, base_position_ptr, position_offset_ptr);
        } else {
            qcopy(moby_position, &base_position);
        }
        if (*(unsigned short *)(vars + 0x72) != 0) {
            if (fast_dec_timer_s16(vars + 0x7A)) {
                *(short *)(vars + 0x7A) = scale_game_frames(*(unsigned short *)(vars + 0x72));
                if (*(int *)(vars + 0x4C) & 4) {
                    radius_reset_this_update = 1;
                    *(float *)(vars + 0x60) = *(float *)(vars + 0x3C);
                    *(int *)(vars + 0x4C) &= 0xFFFFFFF7;
                }
            }
        }
        if (!(*(int *)(vars + 0x4C) & 0x80000)) {
            s64 q = *(s64 *)(vars + 0x48);
            if ((q & (0xC000L << 18)) == (0x8000L << 18))
                *(int *)(vars + 0x4C) ^= 0x2;
            if (!(*(int *)(vars + 0x4C) & 0x2)) {
                if (*(int *)(vars + 0x4C) & 0x8) {
                    float t = *(float *)(vars + 0x60) - *(float *)(vars + 0x44) * frame_scale;
                    *(float *)(vars + 0x60) = t;
                    if (t <= *(float *)(vars + 0x3C)) {
                        *(float *)(vars + 0x60) = *(float *)(vars + 0x3C);
                        if (*(int *)(vars + 0x4C) & 0x4) {
                            if (scale_game_frames(*(unsigned short *)(vars + 0x72)) == 0)
                                *(int *)(vars + 0x4C) ^= 0x8;
                        }
                    }
                } else {
                    float t = *(float *)(vars + 0x60) + *(float *)(vars + 0x40) * frame_scale;
                    *(float *)(vars + 0x60) = t;
                    if (*(float *)(vars + 0x38) <= t) {
                        *(float *)(vars + 0x60) = *(float *)(vars + 0x38);
                        if (!(*(int *)(vars + 0x4C) & 0x1)) {
                            *(int *)(vars + 0x4C) ^= 0x8;
                        } else {
                            *(unsigned char *)(vars + 0x7F) =
                                scale_game_frames(*(unsigned char *)(vars + 0x6B));
                            *(int *)(vars + 0x4C) = (*(int *)(vars + 0x4C) | 0x2) ^ 0x8;
                        }
                    }
                }
            } else {
                if (*(unsigned char *)(vars + 0x6B) != 0 && FUN_L00_001fefc8(vars + 0x7F))
                    *(int *)(vars + 0x4C) ^= 0x2;
            }
        } else {
            if (*(unsigned short *)(vars + 0x72) == 0 || radius_reset_this_update == 1) {
                *(float *)(vars + 0x60) =
                    random_float_between(*(float *)(vars + 0x3C), *(float *)(vars + 0x38));
            }
        }
        if (*(unsigned short *)(vars + 0x6C) != 0) {
            if (fast_dec_timer_s16(vars + 0x74)) {
                *(short *)(vars + 0x74) = scale_game_frames(*(unsigned short *)(vars + 0x6C));
                *(int *)(vars + 0x4C) &= 0xFFFFFF7F;
                *(float *)(vars + 0x54) = (float)*(unsigned char *)(vars + 0x27);
            }
        }
        if (!(*(int *)(vars + 0x4C) & 0x10000)) {
            s64 q = *(s64 *)(vars + 0x48);
            if ((q & (0xC000L << 22)) == (0x8000L << 22))
                *(int *)(vars + 0x4C) ^= 0x20;
            if (!(*(int *)(vars + 0x4C) & 0x20)) {
                if (*(int *)(vars + 0x4C) & 0x80) {
                    float a = convert_integer_to_float(vars[0x27]) / 100.0f;
                    float b = convert_integer_to_float(vars[0x29]) / 100.0f;
                    float t = *(float *)(vars + 0x54) - b * frame_scale;
                    *(float *)(vars + 0x54) = t;
                    if (t <= a) {
                        *(float *)(vars + 0x54) = a;
                        if (*(int *)(vars + 0x4C) & 0x40) {
                            if (*(unsigned short *)(vars + 0x6C) == 0)
                                *(int *)(vars + 0x4C) ^= 0x80;
                        }
                    }
                } else {
                    float a = convert_integer_to_float(vars[0x26]) / 100.0f;
                    float b = convert_integer_to_float(vars[0x28]) / 100.0f;
                    float t = *(float *)(vars + 0x54) + b * frame_scale;
                    *(float *)(vars + 0x54) = t;
                    if (a <= t) {
                        *(float *)(vars + 0x54) = a;
                        if (!(*(int *)(vars + 0x4C) & 0x10)) {
                            *(int *)(vars + 0x4C) ^= 0x80;
                        } else {
                            *(unsigned char *)(vars + 0x7C) = scale_game_frames(vars[0x68]);
                            *(int *)(vars + 0x4C) = (*(int *)(vars + 0x4C) | 0x20) ^ 0x80;
                        }
                    }
                }
            } else {
                if (vars[0x68] != 0 && FUN_L00_001fefc8(vars + 0x7C))
                    *(int *)(vars + 0x4C) ^= 0x20;
            }
        } else {
            if (*(unsigned short *)(vars + 0x6C) == 0 ||
                first_color_randomization_timer_bypass == 1) {
                float a = (float)truncate_float_to_s32((float)vars[0x27]) / 100.0f;
                float b = (float)truncate_float_to_s32((float)vars[0x26]) / 100.0f;
                *(float *)(vars + 0x54) = random_float_between(a, b);
            }
        }
        if (*(unsigned short *)(vars + 0x6E) != 0) {
            if (fast_dec_timer_s16(vars + 0x76)) {
                *(short *)(vars + 0x76) = scale_game_frames(*(unsigned short *)(vars + 0x6E));
                *(int *)(vars + 0x4C) &= 0xFFFFF7FF;
                *(float *)(vars + 0x58) = (float)vars[0x2C];
            }
        }
        if (!(*(int *)(vars + 0x4C) & 0x20000)) {
            s64 q = *(s64 *)(vars + 0x48);
            if ((q & (0xC000L << 26)) == (0x8000L << 26))
                *(int *)(vars + 0x4C) ^= 0x200;
            if (!(*(int *)(vars + 0x4C) & 0x200)) {
                if (*(int *)(vars + 0x4C) & 0x800) {
                    float a = convert_integer_to_float(vars[0x2C]) / 100.0f;
                    float b = convert_integer_to_float(vars[0x2E]) / 100.0f;
                    float t = *(float *)(vars + 0x58) - b * frame_scale;
                    *(float *)(vars + 0x58) = t;
                    if (t <= a) {
                        *(float *)(vars + 0x58) = a;
                        if (*(int *)(vars + 0x4C) & 0x400) {
                            if (*(unsigned short *)(vars + 0x6E) == 0)
                                *(int *)(vars + 0x4C) ^= 0x800;
                        }
                    }
                } else {
                    float a = convert_integer_to_float(vars[0x2B]) / 100.0f;
                    float b = convert_integer_to_float(vars[0x2D]) / 100.0f;
                    float t = *(float *)(vars + 0x58) + b * frame_scale;
                    *(float *)(vars + 0x58) = t;
                    if (a <= t) {
                        *(float *)(vars + 0x58) = a;
                        if (!(*(int *)(vars + 0x4C) & 0x100)) {
                            *(int *)(vars + 0x4C) ^= 0x800;
                        } else {
                            *(unsigned char *)(vars + 0x7D) = scale_game_frames(vars[0x69]);
                            *(int *)(vars + 0x4C) = (*(int *)(vars + 0x4C) | 0x200) ^ 0x800;
                        }
                    }
                }
            } else {
                if (vars[0x69] != 0 && FUN_L00_001fefc8(vars + 0x7D))
                    *(int *)(vars + 0x4C) ^= 0x200;
            }
        } else {
            if (*(unsigned short *)(vars + 0x6E) == 0 ||
                second_color_randomization_timer_bypass == 1) {
                float a = (float)truncate_float_to_s32((float)vars[0x2C]) / 100.0f;
                float b = (float)truncate_float_to_s32((float)vars[0x2B]) / 100.0f;
                *(float *)(vars + 0x58) = random_float_between(a, b);
            }
        }
        if (*(unsigned short *)(vars + 0x70) != 0) {
            if (fast_dec_timer_s16(vars + 0x78)) {
                *(short *)(vars + 0x78) = scale_game_frames(*(unsigned short *)(vars + 0x70));
                *(int *)(vars + 0x4C) &= 0xFFFF7FFF;
                *(float *)(vars + 0x5C) = (float)vars[0x31];
            }
        }
        if (!(*(int *)(vars + 0x4C) & 0x40000)) {
            s64 q = *(s64 *)(vars + 0x48);
            if ((q & (0xC000L << 30)) == (0x8000L << 30))
                *(int *)(vars + 0x4C) ^= 0x2000;
            if (!(*(int *)(vars + 0x4C) & 0x2000)) {
                if (*(int *)(vars + 0x4C) & 0x8000) {
                    float a = convert_integer_to_float(vars[0x31]) / 100.0f;
                    float b = convert_integer_to_float(vars[0x33]) / 100.0f;
                    float t = *(float *)(vars + 0x5C) - b * frame_scale;
                    *(float *)(vars + 0x5C) = t;
                    if (t <= a) {
                        *(float *)(vars + 0x5C) = a;
                        if (*(int *)(vars + 0x4C) & 0x4000) {
                            if (*(unsigned short *)(vars + 0x70) == 0)
                                *(int *)(vars + 0x4C) ^= 0x8000;
                        }
                    }
                } else {
                    float a = convert_integer_to_float(vars[0x30]) / 100.0f;
                    float b = convert_integer_to_float(vars[0x32]) / 100.0f;
                    float t = *(float *)(vars + 0x5C) + b * frame_scale;
                    *(float *)(vars + 0x5C) = t;
                    if (a <= t) {
                        *(float *)(vars + 0x5C) = a;
                        if (!(*(int *)(vars + 0x4C) & 0x1000)) {
                            *(int *)(vars + 0x4C) ^= 0x8000;
                        } else {
                            *(unsigned char *)(vars + 0x7E) = scale_game_frames(vars[0x6A]);
                            *(int *)(vars + 0x4C) = (*(int *)(vars + 0x4C) | 0x2000) ^ 0x8000;
                        }
                    }
                }
            } else {
                if (vars[0x6A] != 0 && FUN_L00_001fefc8(vars + 0x7E))
                    *(int *)(vars + 0x4C) ^= 0x2000;
            }
        } else {
            if (*(unsigned short *)(vars + 0x70) == 0 ||
                third_color_randomization_timer_bypass == 1) {
                float a = (float)truncate_float_to_s32((float)vars[0x31]) / 100.0f;
                float b = (float)truncate_float_to_s32((float)vars[0x30]) / 100.0f;
                *(float *)(vars + 0x5C) = random_float_between(a, b);
            }
        }
        sphere_check_result_or_light_slot_index = *(int *)(vars + 0x50);
        if (sphere_check_result_or_light_slot_index == -1) {
            float kk = convert_integer_to_float(vars[0x24]) / 100.0f;
            *(int *)(vars + 0x50) = allocate_colored_effect_slot((float *)moby_position,
                                                                 *(float *)(vars + 0x60),
                                                                 kk,
                                                                 *(float *)(vars + 0x54),
                                                                 *(float *)(vars + 0x58),
                                                                 *(float *)(vars + 0x5C));
        } else {
            PointLight *point_light = &point_lights[sphere_check_result_or_light_slot_index];
            qcopy(&point_light->position, moby_position);
            point_light->position.f[3] = *(float *)(vars + 0x60);
            point_light->color[0] = *(float *)(vars + 0x54);
            point_light->color[1] = *(float *)(vars + 0x58);
            point_light->color[2] = *(float *)(vars + 0x5C);
            point_light->unkC = convert_integer_to_float(vars[0x24]) / 100.0f;
        }
    } else {
        if (*(int *)(vars + 0x50) != -1) {
            release_point_light_slot(*(int *)(vars + 0x50));
            *(int *)(vars + 0x50) = sphere_check_result_or_light_slot_index;
        }
    }
}
