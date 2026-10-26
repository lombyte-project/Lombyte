/* Shared moby classes: novalis_elevator, bolt_crank, gold_weapon_case, sand_mouse, large_amoeboid, sandmouse_house. */
#include "types.h"
#include "rnc/gameplay/hero_state.h"

#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/collision.h"
#include "rnc/data_labels.h"
#include "rnc/ui/text/font_state.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/overlay/level_labels.h"

/* Bolt-crank Moby.state, including the callback-only state. */
enum BoltCrankState {
    BOLT_CRANK_INIT = 0x0,
    BOLT_CRANK_IDLE = 0x1,
    BOLT_CRANK_CALLBACK_ONLY = 0x2,
    BOLT_CRANK_TURNING = 0x3,
    BOLT_CRANK_LOWERING = 0x4,
    BOLT_CRANK_DONE = 0x5
};

void build_height_grid_positions(f32 *grid_base_position) __asm__("FUN_L01_002b8c08");

void build_height_grid_positions(f32 *grid_base_position) {
    volatile f32 *grid_position_cursor = (volatile f32 *)0x70002000;
    f32 y = grid_base_position[1] + pool_light[3];
    s32 i, j;
    for (i = 0; i < 17; i++) {
        f32 x = grid_base_position[0] + pool_light[2];
        f32 *row = (f32 *)0x70000010 + (i + 1) * 24;
        for (j = 0; j < 17; j++) {
            *grid_position_cursor = x;
            grid_position_cursor += 1;
            *grid_position_cursor = y;
            grid_position_cursor += 1;
            *grid_position_cursor = row[j] + grid_base_position[2];
            grid_position_cursor += 1;
            x += pool_light[4];
        }
        y += pool_light[5];
    }
}
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern u32 is_vector_outside_clip(void *) __asm__("FUN_001f9958");
extern void FUN_L01_00262388(s32, s32);
extern char D_L01_00167240[];

void cull_and_submit_water_grid_tile(f32 *grid_center_position, s32 index, s32 mode) __asm__("FUN_L01_002b8cb0");

void cull_and_submit_water_grid_tile(f32 *grid_center_position, s32 index, s32 mode) {
    f32 corners[4][4];
    f32 near_clip[4];
    f32 far_clip[4];
    f32 offsets[4][4];
    f32 x_offset, y_offset, x_end, y_end;
    f32 half_x, half_y;
    f32 x, y;
    u32 near_bits, far_bits;
    s32 i, value, row, column;

    if (mode != 0) {
        row = index >> 2;
        column = index & 3;
        half_x = 0.5f * -pool_light[2];
        half_y = 0.5f * -pool_light[3];
        x_offset = half_x * (f32)column;
        y_offset = half_y * (f32)row;
        y_end = y_offset + half_y;
        x_end = x_offset + half_x;
        offsets[0][1] = y_offset;
        offsets[0][0] = x_offset;
        offsets[1][1] = y_offset;
        offsets[1][0] = x_end;
        offsets[2][0] = x_offset;
        offsets[2][1] = y_end;
        offsets[3][0] = x_end;
        offsets[3][1] = y_end;
        x = grid_center_position[0] + pool_light[2];
        y = grid_center_position[1] + pool_light[3];
        corners[0][0] = x + offsets[2][0];
        corners[0][1] = y + offsets[0][1];
        corners[0][2] = grid_center_position[2];
        corners[0][3] = 1.0f;
        corners[1][0] = x + offsets[1][0];
        corners[1][1] = y + offsets[1][1];
        corners[1][2] = grid_center_position[2];
        corners[1][3] = 1.0f;
        corners[2][0] = corners[0][0];
        corners[2][1] = y + offsets[2][1];
        corners[2][2] = grid_center_position[2];
        corners[2][3] = 1.0f;
        corners[3][0] = corners[1][0];
        corners[3][1] = corners[2][1];
        corners[3][2] = grid_center_position[2];
        corners[3][3] = 1.0f;
        near_bits = 0;
        far_bits = 0;
        for (i = 0; i < 4; i++) {
            subtract_vector_xyz(near_clip, corners[i], D_L01_00167240);
            scale_vector_xyz(far_clip, near_clip, 1024.0f);
            scale_vector_xyz(near_clip, near_clip, 1024.0f);
            transform_vector(far_clip, far_clip, D_L01_00167240 - 0x80);
            transform_vector(near_clip, near_clip, D_L01_00167240 - 0x40);
            far_bits = (far_bits << 8) | is_vector_outside_clip(far_clip);
            near_bits = (near_bits << 8) | is_vector_outside_clip(near_clip);
        }
        if (mode == 1) {
            if ((near_bits & 0x01010101) == 0x01010101) return;
            if ((near_bits & 0x02020202) == 0x02020202) return;
            if ((near_bits & 0x04040404) == 0x04040404) return;
            if ((near_bits & 0x08080808) == 0x08080808) return;
            if (far_bits == 0) mode = 0;
            if ((near_bits & 0x20202020) == 0x20202020) return;
        } else {
            if ((near_bits & 0x01010101) == 0x01010101) return;
            if ((near_bits & 0x02020202) == 0x02020202) return;
            if ((near_bits & 0x04040404) == 0x04040404) return;
            if ((near_bits & 0x08080808) == 0x08080808) return;
            if ((near_bits & 0x20202020) == 0x20202020) return;
            mode = 0;
        }
    }
    value = (index & 3) * 4 + (index >> 2) * 0x44;
    if (mode != 0) {
        mode = 0xe;
    } else {
        mode = 0xc;
    }
    FUN_L01_00262388(mode, value);
}
#include "qcopy.h"
#include "rnc/rendering/dma_tag.h"
typedef struct {
    u8 pad0[0x190];
    u8 v190[0x10];
    u8 v1A0[0x10];
    u8 pad1B0[0x60];
    f32 f210;
    u8 pad214[0x14];
    f32 f228;
    f32 f22C;
} L01Camera;

extern s32 D_L01_0015F5E0 __attribute__((section(".sdata")));
#include "rnc/overlay/shared/gameplay/entities/novalis_elevator_globals.h"
extern u32 cached_water_texture_id_gp __asm__("D_L01_00161228") __attribute__((sda));
extern u32 cached_water_second_texture_id_gp __asm__("D_L01_0016122C") __attribute__((sda));
extern u32 D_L01_00161230 __attribute__((sda));
extern u32 cached_water_alpha_1_fix_gp __asm__("D_L01_00161234") __attribute__((sda));
extern union PacketCursor l01_packet __asm__("D_L01_001611C0") __attribute__((section(".sdata")));
extern char D_L01_00167240[];
extern void set_scale_matrix(f32 (*)[4], f32) __asm__("FUN_001f9ff8");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void vu1_add_data_ref(void *, s32) __asm__("FUN_00233830");
extern void vu1_gs_regs_font(void) __asm__("FUN_00233c90");
extern void FUN_L01_002b4c28(void);

extern L01Camera D_L01_0016CEC0;
void build_water_render_setup_packet(void) __asm__("FUN_L01_002b8f98");

void build_water_render_setup_packet(void) {
    f32 scaled_translation_matrix[4][4];
    struct DmaTag *base;
    u8 *packet_write_cursor;
    L01Camera *camera;

    set_scale_matrix(scaled_translation_matrix, 1024.0f);
    scale_vector_xyz(scaled_translation_matrix[3], D_L01_00167240, -1024.0f);
    scaled_translation_matrix[3][3] = 1.0f;
    if (D_L01_0015F5E0 != 7) {
        vu1_add_data_ref(font_vu_program, font_vu_program_tag[0]);
        D_L01_0015F5E0 = 7;
    }
    l01_packet.tag->tag = 0x10000000;
    l01_packet.tag->addr = 0;
    l01_packet.tag->vif0 = 0x11000000;
    l01_packet.tag->vif1 = 0x1000404;
    base = l01_packet.tag;
    base[1].tag = 0;
    base[1].addr = 0;
    base[1].vif0 = 0;
    base[1].vif1 = 0x6C0C43A4;
    multiply_matrix(base + 2, D_L01_00167240 - 0x100, scaled_translation_matrix);
    multiply_matrix(base + 6, D_L01_00167240 - 0x80, scaled_translation_matrix);
    base[10].tag = 0x8000;
    base[10].addr = 0x303EC000;
    base[10].vif0 = 0x412;
    camera = &D_L01_0016CEC0;
    *(f32 *)&base[10].vif1 = camera->f210;
    packet_write_cursor = (u8 *)(base + 11);
    qcopy(packet_write_cursor, camera->v190);
    packet_write_cursor = (u8 *)(base + 12);
    qcopy(packet_write_cursor, camera->v1A0);
    *(f32 *)&base[13].tag = camera->f22C;
    *(f32 *)&base[13].addr = camera->f228;
    base[13].vif0 = 0;
    base[13].vif1 = 0;
    base[14].addr = 0x20001D2;
    base[14].vif0 = 0x15000000;
    base[14].tag = 0x3000000;
    base[14].vif1 = 0;
    packet_write_cursor = (u8 *)(base + 15);
    l01_packet.tag->tag |= ((packet_write_cursor - (u8 *)l01_packet.tag) >> 4) - 1;
    l01_packet.bytes = packet_write_cursor;
    FUN_L01_002b4c28();
    l01_packet.tag->tag = 0x30000007;
    l01_packet.tag->addr = (u32)D_L01_001CAC80;
    l01_packet.tag->vif0 = 0;
    l01_packet.tag->vif1 = 0x50000007;
    cached_water_texture_id_gp = 0xFFFFFFFF;
    cached_water_second_texture_id_gp = 0xFFFFFFFF;
    D_L01_00161230 = 0xFFFFFFFF;
    cached_water_alpha_1_fix_gp = 0xFFFFFFFF;
    l01_packet.tag++;
}
#include "qcopy.h"
#define F(p, o) (*(float *)((p) + (o)))

/* Path mover gated by an activation trigger and its collectable flag; plays the arrival cue on the way back. */
extern char *D_L01_001B0930[];
typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L01PathLevelState;
extern L01PathLevelState D_L01_001BB6B0;
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9b48(void *, void *);
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("func_0022DA68");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void set_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L00_00227638");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
extern char D_0013E550[];
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

#define F(p, o) (*(float *)((p) + (o)))

extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
void novalis_elevator_update(unsigned char *moby) __asm__("FUN_L01_002b9eb0");
void novalis_elevator_update(unsigned char *moby) {
    enum {
        NOVALIS_ELEVATOR_WAIT_AT_PATH_END = 0,
        NOVALIS_ELEVATOR_WAIT_AT_PATH_START = 1,
        NOVALIS_ELEVATOR_TRAVERSE_PATH = 2
    };
    char *data = *(char **)(moby + 0x78);
    char *path;
    float orig[4], delta[4], segment_offset[4];
    int on = 0;
    if (*(int *)(data + 0xB4) == -1) {
        return;
    }
    path = D_L01_001B0930[*(int *)(data + 0xB4)];
    if (*(int *)(data + 0xCC) != -1) {
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xCC)) == 0) {
            moby[0x30] = 0xFF;
            *(int *)(moby + 0x94) = 0;
            *(unsigned short *)(moby + 0x34) |= 0x41;
            return;
        }
        if (*(int *)(data + 0xCC) != -1 &&
            is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xCC))) {
            *(unsigned short *)(moby + 0x34) &= 0xFFBE;
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
            *(int *)(data + 0xCC) = -1;
        }
    }
    if (*(int *)(data + 0xC0) != -1) {
        unsigned short id;
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xC0)) ||
            D_L01_001BB6B0.collected[(short)(id = *(unsigned short *)(moby + 0xB2))] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            if (*(int *)(data + 0xC0) != -1) {
                D_0014C190_257218[current_level_index][(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
                    1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
                moby_killed[(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
                    1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
                *(int *)(data + 0xC0) = -1;
            }
            on = 1;
        }
    } else {
        on = 1;
    }
    qcopy(orig, moby + 0x10);
    fast_dec_timer_s16(data + 0xBA);
    if (moby[0x20] == 0) {
        float f;
        char *pts;
        data[0x28] = 4;
        *(short *)(data + 0x3E) = 5;
        *(int *)(data + 0x20) = 0;
        *(short *)(data + 0x24) = 0;
        f = 0.0f;
        if ((moby[0xBC] = data[0xA0]) == 0) {
            f = 1.0f;
        }
        F(data, 0xA4) = f;
        pts = path + 0x10;
        qcopy((moby + 0x10), func_001FA898_r(f) ? pts + (*(int *)path - 1) * 16 : pts);
        *(short *)(data + 0xBA) = 0;
        *(int *)(data + 0xA8) = 0;
        *(int *)(data + 0xBC) = 0;
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        *(int *)(data + 0xC4) = -1;
    }
    switch (moby[0xBC]) {
    case NOVALIS_ELEVATOR_WAIT_AT_PATH_END:
        if (on) {
            float far;
            if ((*(short *)(data + 0xB8) != 0 && is_hero_on_moby(moby)) ||
                (far = FUN_001f9b48(&hero.motion.pos, path + *(int *)path * 16),
                 FUN_001f9b48(&hero.motion.pos, path + 0x10) < far)) {
                moby[0xBC] = 2;
                F(data, 0xAC) = -1.0f / multiply_global_factor_ed64(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case NOVALIS_ELEVATOR_WAIT_AT_PATH_START:
        if (on) {
            float near;
            if ((*(short *)(data + 0xB8) != 0 && is_hero_on_moby(moby)) ||
                (near = FUN_001f9b48(&hero.motion.pos, path + 0x10),
                 FUN_001f9b48(&hero.motion.pos, path + *(int *)path * 16) < near)) {
                moby[0xBC] = 2;
                F(data, 0xAC) = 1.0f / multiply_global_factor_ed64(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case NOVALIS_ELEVATOR_TRAVERSE_PATH:
        if (*(int *)(data + 0xC8) == 0 && is_hero_on_moby(moby)) {
            struct Hero *player = &hero;
            player->jump_lock_timer = 4;
            player->unk1F4 = 4;
        }
        if (*(short *)(data + 0xBA) != 0 ||
            (F(data, 0xBC) < 0.0f &&
             vector_distance((moby + 0x10), &hero.motion.pos) < 2.83f &&
             absolute_float(hero.motion.pos.f[2] - F(moby, 0x18)) < 4.0f &&
             F(moby, 0x18) - hero.motion.pos.f[2] > 0.9f)) {
            int r;
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xBA) = scale_game_frames(30);
            }
            r = *(int *)(data + 0xC4);
            *(int *)(data + 0xA8) = 0;
            if (r != -1) {
                unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                    release_voice_slot(r);
                }
            }
            *(int *)(data + 0xC4) = -1;
        } else {
            int i;
            float segment_interpolation_fraction, path_point_index_float;
            char *segment_start;
            if (!voice_is_playing(moby, *(int *)(data + 0xC4))) {
                *(int *)(data + 0xC4) = allocate_voice_for_target_entry(0, 4, (int)moby);
            }
            F(data, 0xA8) += F(data, 0xAC) * frame_time;
            if (absolute_float(F(data, 0xA8)) > absolute_float(F(data, 0xAC))) {
                F(data, 0xA8) = F(data, 0xAC);
            }
            F(data, 0xA4) = F(data, 0xA4) + F(data, 0xA8);
            if (0.5f < absolute_float(F(data, 0xA4) - 0.5f)) {
                int r;
                if (0.0f < F(data, 0xAC)) {
                    moby[0xBC] = 0;
                    F(data, 0xA4) = 1.0f;
                } else {
                    moby[0xBC] = 1;
                    F(data, 0xA4) = 0.0f;
                }
                *(int *)(data + 0xA8) = 0;
                *(short *)(data + 0xBA) = scale_game_frames(15);
                r = *(int *)(data + 0xC4);
                if (r != -1) {
                    unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                    if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                        release_voice_slot(r);
                    }
                }
                *(int *)(data + 0xC4) = -1;
            }
            segment_interpolation_fraction = convert_integer_to_float(*(int *)path - 1);
            i = func_001FA898_r(segment_interpolation_fraction * F(data, 0xA4));
            segment_interpolation_fraction = convert_integer_to_float(*(int *)path - 1);
            path_point_index_float = convert_integer_to_float(i);
            segment_interpolation_fraction *= F(data, 0xA4);
            segment_interpolation_fraction -= path_point_index_float;
            if (i == *(int *)path - 1) {
                qcopy((moby + 0x10), path + 0x10 + i * 16);
            } else {
                segment_start = path + 0x10 + i * 16;
                subtract_vector_xyz(segment_offset, path + 0x20 + i * 16, segment_start);
                scale_vector_xyz(segment_offset, segment_offset, segment_interpolation_fraction);
                add_vector_xyz((moby + 0x10), segment_offset, segment_start);
            }
            F(data, 0xBC) = F(moby, 0x18) - orig[2];
        }
        break;
    }
    subtract_vector_xyz(delta, (moby + 0x10), orig);
    update_entity_matrix(data + 0x60, delta, moby + 0x40, moby + 0x40);
    if (!is_hero_on_moby(moby)) {
        if (2.0f < vector_distance((moby + 0x10), &hero.motion.pos)) {
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xB8) = 1;
            }
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* spawns two bursts of particles around a point */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002C84D8), where it is exact; names translated to the US level program. */

extern char *FUN_L00_0026cbb0(void *, void *, int, int, int, int, float);
extern char D_L01_00167240[];
extern char D_L01_001742E0[];
extern float D_L01_001742C0[];
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int FUN_L00_00257b90(int, int);
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void spawn_two_point_effect_particle(char *, char *) __asm__("FUN_L00_0026c6e8");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

void spawn_collision_particle_bursts(void) __asm__("FUN_L01_002c72c8");

void spawn_collision_particle_bursts(void) {
    float offset_vector[4];
    float spawn_position[4];
    float base_angle, b;
    int i;
    fast_vec_sub(offset_vector, D_L01_00167240, D_L01_001742E0);
    base_angle = -fast_arc_tan(vector_length_xy(offset_vector), offset_vector[2]);
    fast_arc_tan(offset_vector[0], offset_vector[1]);
    for (i = 19; i >= 0; i--) {
        float x, y;
        x = fast_add_rotations(randf_sym(0.0f, 3.1415927f), base_angle);
        y = fast_add_rotations(randf_sym(0.0f, 3.1415927f), base_angle);
        build_spherical_offset(offset_vector, 0.2f, y, x);
        fast_vec_scale(spawn_position, offset_vector, random_float_between_alt(1.0f, 3.0f));
        fast_vec_add(spawn_position, spawn_position, D_L01_001742E0);
        spawn_two_point_effect_particle((char *)spawn_position, (char *)offset_vector);
    }
    for (i = 9; i >= 0; i--) {
        float x, y;
        spawn_position[0] = D_L01_001742C0[8] + randf_sym(0.0f, 0.1f);
        spawn_position[1] = D_L01_001742C0[9] + randf_sym(0.0f, 0.1f);
        spawn_position[2] = D_L01_001742C0[10] + randf_sym(0.0f, 0.1f);
        x = fast_add_rotations(randf_sym(0.0f, 3.1415927f), base_angle);
        y = fast_add_rotations(randf_sym(0.0f, 3.1415927f), base_angle);
        build_spherical_offset(offset_vector, 0.05f, y, x);
        FUN_L00_0026cbb0(spawn_position, offset_vector, 0x4F007FFF, 0x1FFFFFFF,
                         FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)), 1, 20000.0f);
    }
}
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern int FUN_001e9448(void *);
extern int collect_mobies_in_sphere(float, void *, int, int, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_00259bc8_2c7530(float, void *, void *, int, void *, void *) __asm__("FUN_L00_00259bc8");
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void spawn_type_14_particle(int, void *) __asm__("FUN_L01_00280970");
extern void FUN_0020c828(void *);

extern s32 moby_motion_particle_random_sign_base_gp __asm__("D_L01_00161494") __attribute__((sda));
extern f32 moby_motion_collision_radius_gp __asm__("D_L01_0016149C") __attribute__((sda));
void update_moby_motion_collision_and_lifetime(char *moby) __asm__("FUN_L01_002c7530");

void update_moby_motion_collision_and_lifetime(char *moby) {
    enum {
        UPDATE_TRANSFORM_ONLY = 0,
        COLLISION_CHECK_AND_COUNTDOWN = 1,
        TIMED_REMOVAL = 2
    };
    int *data = *(int **)(moby + 0x78);
    char *camera = D_L01_00167240;
    char hit[0x30];
    float normal[4];
    float distance;

    distance = vector_distance(camera, moby + 0x10);
    camera -= 0x140;
    *(float *)(moby + 0x44) = -fast_arc_tan(distance, *(float *)(camera + 0x148) - *(float *)(moby + 0x18));
    *(float *)(moby + 0x48) = fast_arc_tan(*(float *)(camera + 0x140) - *(float *)(moby + 0x10),
                                             *(float *)(camera + 0x144) - *(float *)(moby + 0x14));
    *(float *)(moby + 0x40) = fast_add_rotations(*(float *)(moby + 0x40), *(float *)(data + 3));
    fast_vec_add(moby + 0x10, moby + 0x10, data + 8);
    switch ((u8)moby[0x20]) {
    case COLLISION_CHECK_AND_COUNTDOWN:
        spawn_type_14_particle(moby_motion_particle_random_sign_base_gp, moby);
        fast_vec_normalize(normal, data + 8, 1.0f);
        make_hit_descriptor(hit, moby, 0, 0.0f, normal);
        if (collect_mobies_in_sphere(moby_motion_collision_radius_gp, moby + 0x10, 0x10, data[6], hit) &&
            !FUN_001e9448(moby)) {
            CollisionHit *contact = (CollisionHit *)D_L01_001742C0;
            if (contact->moby != 0)
                FUN_L00_00259bc8_2c7530(1.0f, contact->moby, moby, 0x10001, &contact->point, normal);
            ((void (*)(void *))spawn_collision_particle_bursts)(moby);
            goto remove;
        }
        if (fast_dec_timer(data)) {
            data[0] = scale_game_frames(0x1e);
            moby[0x20] = 2;
            if (data[4] != 0)
                data[4] = 0;
        }
        break;
    case UPDATE_TRANSFORM_ONLY:
        break;
    case TIMED_REMOVAL:
        if (fast_dec_timer(data)) {
        remove:
            FUN_0020c828(moby);
        } else {
            int duration = scale_game_frames(0x1e);
            moby[0x23] = truncate_float_to_s32((float)data[0] * (127.0f / (float)duration));
        }
        break;
    }
}
extern int D_0015ED84;
extern unsigned char menu_item_enabled_1_lui __asm__("D_0013D395") __attribute__((section(".data")));
/* On level 1, when a state-5 moby of type 0x118 has sub-id 0x34 or 0x35, sets the matching flag byte (D_0013D394 / D_0013D395). */
void set_novalis_bolt_crank_completion_flags(char *moby) __asm__("FUN_L01_002e0bd8");

void set_novalis_bolt_crank_completion_flags(char *moby)
{
    int level = current_level_index;
    if (level != 1) return;
    if (*(short *)(moby + 0xB2) == 0x34 &&
            *(short *)(moby + 0xA6) == 0x118 &&
            (unsigned char)moby[0x20] == 5) {
            menu_item_enabled_0 = level;
            level = D_0015ED84;
    }
    if (level == 1 && *(short *)(moby + 0xB2) == 0x35 &&
            *(short *)(moby + 0xA6) == 0x118 &&
            (unsigned char)moby[0x20] == 5) {
            menu_item_enabled_1_lui = level;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E1FE0). */
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/gameplay/state/game_globals.h"
#include "rnc/gameplay/state/options.h"

typedef struct {
    f32 progress;          /* 0x00: turned / (turns * 2 pi), 0..1 */
    u8 pad_04[0x10];
    s32 idle_frames;       /* 0x14 */
    f32 turned;            /* 0x18 */
    f32 speed;             /* 0x1C */
    s32 camera_point;      /* 0x20 */
    f32 fall_speed;        /* 0x24 */
    f32 camera_params[2];  /* 0x28 */
    f32 turns;             /* 0x30 */
    s32 exit_camera_point; /* 0x34: -1 none */
    s32 reusable;          /* 0x38: back to idle when done */
    f32 rest_height;       /* 0x3C */
} BoltCrankVars;

extern char *D_L01_001600EC;
extern char D_L01_001BAAA0[];
extern f32 D_L01_00167258_lui __asm__("D_L01_00167258") __attribute__((section(".data")));
extern char *D_L01_00167280_lui __asm__("D_L01_00167280") __attribute__((section(".data")));
extern f32 bolt_crank_radial_correction_step_gp __asm__("D_L01_001618A0") __attribute__((sda));
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32 fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void euler_rotation_matrix(float *, void *) __asm__("FUN_001fa050");
extern f32 probe_ground_height(void *, int, f32) __asm__("FUN_00213508");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern void fast_mem_zero16(void *, int) __asm__("FUN_001f9810");
extern void divide_vector_xyz(void *, void *, f32) __asm__("FUN_L00_001ff378");
extern void set_hero_animation(int, int, f32) __asm__("FUN_L00_002323b8");
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
extern f32 step_angle_toward_target(f32 *, f32, int, f32) __asm__("FUN_L00_0025bc00");
extern int pad_pressed_within(int, int, int *) __asm__("FUN_L00_00266d60");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
extern void set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eac18(int);
extern int hero_set_state(int, int) __asm__("FUN_L01_0023cf98");
extern void record_moby_context_bytes(void *, int, void *, int, void *) __asm__("FUN_L01_0029b0a0");
extern void set_camera_5_track_acceleration_yz(f32, f32) __asm__("FUN_L01_00316e88");

/* Turned by the hero (state 0x3B) until progress reaches 1, then lowered and saved as done. */
void bolt_crank_update(struct Moby *moby) __asm__("FUN_L01_002e0c68");
void bolt_crank_update(struct Moby *moby) {
    BoltCrankVars *crank = (BoltCrankVars *)moby->pvars;
    float matrix[16];
    float stick[4], step[4], facing[4];
    Vec4 rotation, target;
    float offset[4], spring_pos;
    float distance, yaw_diff, angle, old_angle, height_diff, max_speed, height, ground;
    int save_id, area_clear;
    struct Moby *other;

    if (crank == 0) {
        moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 6.2831855f);
        return;
    }

    if (hero.state.current != HERO_STATE_CRANK) {
        if (moby->state == BOLT_CRANK_TURNING) {
            moby->state = BOLT_CRANK_IDLE;
            if (D_L01_00167280_lui == 0) {
                return;
            }
            FUN_L00_002eac18(4);
            return;
        }
    }
    switch (moby->state) {
    case BOLT_CRANK_INIT:
        crank->rest_height = moby->pos.z;
        moby->state = BOLT_CRANK_IDLE;
        goto done;
    case BOLT_CRANK_IDLE: {
        save_id = moby->save_id;
        if (D_L01_001BB6B0.collected[(short)save_id] != 0 ||
            ((D_0014C190_257218[current_level_index][(short)save_id >> 5] >> (save_id & 0x1F)) & 1)) {
            moby->pos.z = moby->pos.z - 0.5f;
            moby->state = BOLT_CRANK_DONE;
            crank->progress = 1.0f;
            goto done;
        }
        distance = vector_distance(&moby->pos, &hero.motion.pos);
        if (distance < 1.75f) {
            if (hero.vehicle_mode == 0) {
                angle = fast_arc_tan(moby->pos.x - hero.motion.pos.f[0], moby->pos.y - hero.motion.pos.f[1]);
                yaw_diff = fast_difference_between_rotations(hero.motion.rot.f[2], angle);
                if (yaw_diff < 1.5707964f) {
                    height_diff = absolute_float((hero.motion.pos.f[2] - 0.3f) - moby->pos.z);
                    if (height_diff < 0.2f && hero.items[0].item_id == 8 && hero.items[0].moby != 0) {
                        int pressed = pad_pressed_within(0x80, scale_game_frames(7), 0);
                        if (pressed != 0 && (unsigned int)scale_game_frames(0x1E) < (unsigned int)crank->idle_frames &&
                            moby->pos.z == crank->rest_height) {
                            if (crank->progress < 1.0f) {
                                area_clear = 1;
                                crank->idle_frames = 0;
                                bolt_crank_radial_correction_step_gp = 0.0f;
                                for (other = (struct Moby *)D_L01_0015FFE4; other != 0; other = other->next) {
                                    if (other->flags & 0x1000) {
                                        if (vector_distance(&moby->pos, &other->pos) < 3.25f) {
                                            area_clear = 0;
                                            break;
                                        }
                                    }
                                }
                                if (area_clear == 0) {
                                    goto done;
                                }
                                hero_set_state(HERO_STATE_CRANK, 1);
                                jet_mount(D_L01_00167240, D_L01_00167240 + 0x10, 3, scale_game_frames(0xB4), 0);
                                set_camera_rotation(D_L01_001600EC + (crank->camera_point << 7) + 0x70);
                                set_camera_position(D_L01_001600EC + (crank->camera_point << 7) + 0x30);
                                set_camera_5_track_acceleration_yz(crank->camera_params[0], crank->camera_params[1]);
                                moby->state = BOLT_CRANK_TURNING;
                                crank->fall_speed = 0.0f;
                                crank->turned = crank->progress * (crank->turns * 6.2831855f);
                                goto done;
                            }
                        }
                    }
                }
            }
        }
        if (crank->progress < 1.0f) {
            crank->idle_frames = crank->idle_frames + 1;
            approach_value(&crank->progress, 0.0f, 1.0f / (float)scale_game_frames(0x3C));
            if (0.0f < crank->progress) {
                moby->rot.z = fast_subtract_rotations(moby->rot.z,
                                                        crank->turns * 6.2831855f / (float)scale_game_frames(0x3C));
            }
            moby->pos.z = moby->pos.z + frame_time * 0.5f;
            if (crank->rest_height <= moby->pos.z) {
                moby->pos.z = crank->rest_height;
            }
        } else {
            if (crank->reusable == 0) {
                goto done;
            }
            if (crank->exit_camera_point != -1 &&
                visited52[moby->mission + (current_level_index << 4)] != 0xFF) {
                record_moby_context_bytes(crank, 4, moby, 2, D_L01_001BAAA0);
                jet_clear_persistent(moby->mission);
                if (crank->exit_camera_point != -1) {
                    char *point = D_L01_001600EC + (crank->exit_camera_point << 7);
                    jet_reset_camera(point + 0x30, point + 0x70);
                }
            }
            moby->pos.z = moby->pos.z - frame_time * 0.5f;
            {
                float lowest = crank->rest_height - 0.5f;
                if (moby->pos.z <= lowest) {
                    moby->pos.z = lowest;
                }
            }
        }
        goto done;
    }
    case BOLT_CRANK_CALLBACK_ONLY:
        goto done;
    case BOLT_CRANK_TURNING:
        target.q = 0;
        target.f[2] = D_L01_00167258_lui;
        rotation.q = target.q;
        old_angle = fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y);
        if ((pad_pressed_within(0x80, scale_game_frames(7), 0) != 0 && scale_game_frames(0x3C) < hero.state_timer) ||
            crank->progress == 1.0f) {
            hero_set_state(0, 1);
            if (crank->reusable != 0) {
                moby->state = BOLT_CRANK_IDLE;
            } else if (crank->progress == 1.0f) {
                record_moby_context_bytes(crank, 4, moby, 2, D_L01_001BAAA0);
                moby->state = BOLT_CRANK_LOWERING;
                D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] |=
                    1 << (moby->save_id & 0x1F);
                moby_killed[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                allocate_voice_for_target_entry(0, 0, moby);
                if (crank->exit_camera_point != -1 &&
                    visited52[moby->mission + (current_level_index << 4)] != 0xFF) {
                    record_moby_context_bytes(crank, 4, moby, 2, D_L01_001BAAA0);
                    jet_clear_persistent(moby->mission);
                    if (crank->exit_camera_point != -1) {
                        char *point = D_L01_001600EC + (crank->exit_camera_point << 7);
                        jet_reset_camera(point + 0x30, point + 0x70);
                    }
                }
            }
            FUN_L00_002eac18(4);
            return;
        }
        stick[1] = -hero.stick_x;
        stick[0] = -hero.stick_y;
        distance = vector_length_xy(stick);
        if (1.0f < distance) {
            divide_vector_xyz(stick, stick, distance);
        }
        euler_rotation_matrix(matrix, &rotation);
        transform_vector(stick, stick, matrix);
        facing[0] = fast_cos(hero.moby->rot.z);
        facing[1] = fast_sin(hero.moby->rot.z);
        facing[2] = 0.0f;
        distance = fast_vec_dot(facing, stick);
        {
            float max_speed_change = frame_time_sq * 5.5f;
            float change = distance - crank->speed;
            if (max_speed_change < change) change = max_speed_change;
            if (change < -max_speed_change) change = -max_speed_change;
            max_speed = frame_time * 3.7f;
            crank->speed = crank->speed + change;
        }
        if (max_speed < crank->speed) crank->speed = max_speed;
        if (crank->speed < 0.0f) crank->speed = 0.0f;
        if (absolute_float(crank->speed) < frame_time_sq * 5.5f * 0.25f) crank->speed = 0.0f;
        fast_vec_normalize(step, facing, crank->speed);

        if (hero.anim_flags & 2) {
            if (hero.moby->prev_seq == 0x44) {
                set_hero_animation(0x3F, 0, (float)scale_game_frames(7));
            }
        }
        if (hero.moby->prev_seq != 0x44) {
            if (hero.anim_changing == 0) {
                if (hero.moby->seq == 0x40 && absolute_float(crank->speed) == 0.0f) {
                    set_hero_animation(0x3F, 0, (float)scale_game_frames(0xE));
                } else {
                    if (hero.moby->seq == 0x3F) {
                        if (0.0f < crank->speed) {
                            set_hero_animation(0x40, 0, (float)scale_game_frames(0xE));
                        }
                    } else if (hero.moby->seq == 0x40 && frame_time * 3.7f * 0.6f < crank->speed) {
                        set_hero_animation(0x41, 0, (float)scale_game_frames(0xE));
                    } else if (hero.moby->seq == 0x41 && crank->speed < frame_time * 3.7f * 0.4f) {
                        set_hero_animation(0x40, 0, (float)scale_game_frames(0x10));
                    }
                }
            }
        }

        {
            struct Hero *player = &hero;
            height = player->motion.pos.f[2];
            add_vector_xyz(&player->motion.pos, &player->motion.pos, step);
            angle = fast_arc_tan(player->motion.pos.f[0] - moby->pos.x, player->motion.pos.f[1] - moby->pos.y);
            step[0] = fast_cos(angle) * 1.05f;
            step[1] = fast_sin(angle) * 1.05f;
            step[2] = 0.0f;
            add_vector_xyz(&target, &moby->pos, step);
            subtract_vector_xyz(offset, &target, &player->motion.pos);
            offset[2] = 0.0f;
            distance = vector_length_xy(offset);
            spring_pos = -distance;
            vendor_spring(&spring_pos, &bolt_crank_radial_correction_step_gp, 0.0f, frame_scale_sq * 0.015f,
                          frame_scale_sq * 0.3f, frame_time * 7.0f);
            fast_vec_normalize(offset, offset, bolt_crank_radial_correction_step_gp);
            add_vector_xyz(&player->motion.pos, &player->motion.pos, offset);
            player->motion.pos.f[2] = height;
            if (opt_mirror_anim) {
                player->motion.move_yaw = fast_add_rotations(-1.5707964f, angle);
            } else {
                player->motion.move_yaw = fast_add_rotations(1.5707964f, angle);
            }
        }
        step_angle_toward_target(&hero.motion.rot.f[2], hero.motion.move_yaw, 0, frame_time * 6.2831855f);
        crank->fall_speed = crank->fall_speed + frame_time_sq * 9.8f;
        hero.motion.pos.f[2] = hero.motion.pos.f[2] - (crank->fall_speed - 2.0f);
        ground = probe_ground_height(&hero.motion.pos, 0, 0.5f);
        hero.motion.pos.f[2] = hero.motion.pos.f[2] - 2.0f;
        if (hero.motion.pos.f[2] < ground) {
            hero.motion.pos.f[2] = ground;
            crank->fall_speed = 0.0f;
        }
        if (hero.moby->prev_seq != 0x44) {
            float limit = 1.0471976f;
            float handle_yaw = fast_add_rotations(0.2617994f, angle);
            step_angle_toward_target(&moby->rot.z, handle_yaw, 0, frame_time * 3.1415927f);
            if (limit < fast_subtract_rotations(moby->rot.z, handle_yaw)) {
                moby->rot.z = fast_subtract_rotations(moby->rot.z, limit);
            } else if (fast_subtract_rotations(moby->rot.z, handle_yaw) < -1.0471976f) {
                moby->rot.z = fast_add_rotations(moby->rot.z, limit);
            }
        }
        yaw_diff = fast_difference_between_rotations(angle, old_angle);
        crank->turned = crank->turned + yaw_diff;
        crank->progress = crank->turned / (crank->turns * 6.2831855f);
        if (1.0f < crank->progress) crank->progress = 1.0f;
        if (crank->progress < 0.0f) crank->progress = 0.0f;
        fast_mem_zero16(&hero.motion.velocity, 0x90);
        goto done;
    case BOLT_CRANK_LOWERING:
        moby->pos.z = moby->pos.z - frame_time * 0.5f;
        {
            float lowest = crank->rest_height - 0.5f;
            if (moby->pos.z <= lowest) {
                moby->pos.z = lowest;
                moby->state = BOLT_CRANK_DONE;
                crank->progress = 1.0f;
            }
        }
        goto done;
    case BOLT_CRANK_DONE:
        crank->progress = 1.0f;
        goto done;
    }
done:
    set_novalis_bolt_crank_completion_flags((char *)moby);
}
#include "sda.h"
#include "rnc/gameplay/entities/moby.h"

/* Bought-item price entry (0x18 bytes). */
typedef struct {
    u8 pad_00[0x14];
    u16 bolts; /* 0x14 */
    u8 pad_16[2];
} ItemPrice_2e1ac0;

/* pvars of the moby run by gold_weapon_case_update: a scene trigger
   (update_npc_dialogue) followed by the item it sells. */
typedef struct {
    u8 pad_00[4];
    s16 result;   /* 0x04: 2 once the scene finished with a sale */
    u8 pad_06[6];
    f32 radius;   /* 0x0C */
    u8 pad_10[0x30];
    s32 slot;     /* 0x40: -1 none, else index into D_L01_001DEB10 */
} SaleVars_2e1ac0;

extern s32 cutscene_mode __asm__("D_L01_0015F5C4");
extern u8 D_0013E520_2e1ac0[] __asm__("D_0013E520");
extern void FUN_0020c828_2e1ac0(struct Moby *) __asm__("FUN_0020c828");
extern s32 FUN_L00_002668a0_2e1ac0(struct Moby *, SaleVars_2e1ac0 *) __asm__("FUN_L00_002668a0");
extern s32 FUN_L00_00266448_2e1ac0(struct Moby *, SaleVars_2e1ac0 *) __asm__("FUN_L00_00266448");
extern void initialize_vendor_screen(struct Moby *, s32, ItemPrice_2e1ac0 *) __asm__("FUN_L01_002aea70");
extern s32 FUN_0020b178_2e1ac0(s32, s32) __asm__("FUN_0020b178");

extern s32 D_L01_001DEB10[];
extern ItemPrice_2e1ac0 D_L01_001C4530[];
extern s32 alternate_animation_sequence_gp __asm__("D_0015EE20") __attribute__((sda));
void gold_weapon_case_update(struct Moby *m) __asm__("FUN_L01_002e1ac0");
void gold_weapon_case_update(struct Moby *moby) {
    enum {
        GOLD_WEAPON_CASE_INITIALIZE = 0,
        GOLD_WEAPON_CASE_PROCESS_SALE = 1,
        GOLD_WEAPON_CASE_REMOVE_WHEN_PURCHASED = 2,
        GOLD_WEAPON_CASE_IDLE = 3,
        GOLD_WEAPON_CASE_WAIT_FOR_CUTSCENE_END = 4
    };
    SaleVars_2e1ac0 *sale_vars = (SaleVars_2e1ac0 *)moby->pvars;
    s32 item;
    ItemPrice_2e1ac0 *price;

    if (sale_vars != 0 && sale_vars->slot != -1 && ((sale_vars->slot ^ 1) & 1) && alternate_animation_sequence_gp == 0) {
        FUN_0020c828_2e1ac0(moby);
        return;
    }
    switch (moby->state) {
    case GOLD_WEAPON_CASE_INITIALIZE:
        if (moby->oclass != 0x130) {
            moby->state = 2;
        } else if (D_0013E520_2e1ac0[D_L01_001DEB10[sale_vars->slot]] != 0 ||
                   FUN_L00_002668a0_2e1ac0(moby, sale_vars) == -1) {
            moby->state = 3;
        } else {
            moby->state = 1;
        }
        break;
    case GOLD_WEAPON_CASE_PROCESS_SALE:
        sale_vars->radius = 1.9f;
        FUN_L00_00266448_2e1ac0(moby, sale_vars);
        if (sale_vars->result == 2) {
            D_0013E520_2e1ac0[D_L01_001DEB10[sale_vars->slot]] = 1;
            moby->state = 4;
            item = D_L01_001DEB10[sale_vars->slot];
            price = &D_L01_001C4530[item];
            current_bolt_count -= price->bolts;
            initialize_vendor_screen(moby, item, price);
        }
        break;
    case GOLD_WEAPON_CASE_IDLE: /* sold out or done: idle */
        break;
    case GOLD_WEAPON_CASE_WAIT_FOR_CUTSCENE_END:
        if (cutscene_mode == 0) {
            FUN_0020b178_2e1ac0(0, -1);
            moby->state = 3;
        }
        break;
    case GOLD_WEAPON_CASE_REMOVE_WHEN_PURCHASED:
        if (D_0013E520_2e1ac0[D_L01_001DEB10[sale_vars->slot]] != 0) {
            FUN_0020c828_2e1ac0(moby);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E4430), where it is exact; names translated to the US level program. */

extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void compute_sand_mouse_approach_target(int unused, void *out) __asm__("FUN_L01_002e30b8");

void compute_sand_mouse_approach_target(int unused, void *out) {
    char scratch[16];
    char *base = D_0013E533 + 0xE1D;
    fast_vec_normalize(scratch, *(char **)(base + 0x2080) + 0xD0, 1.0f);
    fast_vec_add(out, scratch, base + 0x80);
}
#include "sda.h"

/* Builds a camera-relative transform for a moby and steps its height. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E4488), where it is exact; names translated to the US level program. */

typedef struct {
    float a[4];
} Vs __attribute__((aligned(16)));

extern int probe_vertical_ground_collision(Vs *p, int x, int y, float f, float g) __asm__("FUN_L00_002583f0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

extern float D_L01_001DEB40[];
void sand_mouse_compute_target(char *a, char *m) __asm__("FUN_L01_002e3110");
void sand_mouse_compute_target(char *a, char *m) {
    Vs t0;
    Vs t1;
    struct Hero *g = &hero;
    normalize_vector_xyz(&t0, ((char *)g->moby) + 0xD0, 1.1f);
    normalize_vector_xyz(&t1, ((char *)g->moby) + 0xE0, 1.0f);
    add_vector_xyz(m, &t0, &t1);
    add_vector_xyz(m, m, &g->motion.pos);
    if (*(int *)(m + 0x68) == 0) {
        if (probe_vertical_ground_collision((Vs *)m, (int)a, 0, D_L01_001DEB40[0], 2.0f)) {
            *(float *)(m + 8) = D_L01_001742E8;
        }
    } else {
        *(float *)(m + 8) = *(float *)(m + 8) + 1.0f;
    }
}
struct DriftEmitter {
    s32 count;          /* 0x00 */
    u8 pad4[0x2C];
    f32 spread;         /* 0x30 */
};
extern struct DriftEmitter D_L01_001DEB38;
extern s32 move_moby_with_collision_integer_radius(void *, void *, s32, s32, f32, f32) __asm__("FUN_L00_00258ad0");

/* shrinks and sinks the moby, then hands it to the drift emitter */
s32 update_sand_mouse_fall_motion(void *ctx, struct Moby *movement_state) __asm__("FUN_L01_002e3208");

s32 update_sand_mouse_fall_motion(void *ctx, struct Moby *movement_state) {
    f32 shrink;

    movement_state->pos.z -= frame_time_sq * 29.7f;
    shrink = frame_scale * -0.050000012f + 1.0f;
    movement_state->pos.x *= shrink;
    movement_state->pos.y *= shrink;
    return move_moby_with_collision_integer_radius(ctx, &movement_state->pos, D_L01_001DEB38.count, 0, 0.3f, D_L01_001DEB38.spread);
}
/* Picks the best lock-on target for moby from the visible-moby list: a living, drawn class-5 enemy within range whose direction from `from`
 * is inside the yaw/pitch cone (or the wider near cone when close), scored by angle error and distance,
 * with a clear line of sight (or the blocker is the same kind of enemy). */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E4620), where it is exact; names translated to the US level program. */

extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern float square_root(float) __asm__("FUN_001f9988");
extern float fast_difference_between_rotations_c(float, float) __asm__("func_001FA688");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern char D_L01_00174340_h[] __asm__("D_L01_001742C0");
extern float *func_L00_0025D390_t(void *) __asm__("FUN_002141f8");
extern int func_L00_001EFFF0_t(void *, void *, int, void *, void *) __asm__("FUN_001efa68");

void *select_sand_mouse_spawn_target(void *moby, void *from, void *reference_rotation, float yawMax, float pitchMax, float range, float near,
                        float nearYaw, float nearPitch) __asm__("FUN_L01_002e32a8");

void *select_sand_mouse_spawn_target(void *moby, void *from, void *reference_rotation, float yawMax, float pitchMax, float range, float near,
                        float nearYaw, float nearPitch) {
    float tp[4];
    char *best = 0;
    float bestScore = 1.0e9f;
    char *candidate_moby;
    for (candidate_moby = D_L01_0015FFE4; candidate_moby != 0; candidate_moby = *(char **)(candidate_moby + 0x28)) {
        char *hs = D_L01_00174340_h;
        struct Hero *pl = &hero;
        float *hp;
        int candidate_class;
        float candidate_distance, score, pitch_error_squared;
        if (*(short *)(candidate_moby + 0x32) == 0) continue;
        hp = func_L00_0025D390_t(candidate_moby);
        if (!(*(unsigned short *)(candidate_moby + 0x34) & 0x1000) || candidate_moby == 0 || *(char **)(candidate_moby + 0x24) == 0) continue;
        candidate_class = *(short *)(*(char **)(candidate_moby + 0x24) + 0x46);
        if (candidate_class != 5 || hp == 0 || !(0.0f < *hp)) continue;
        candidate_distance = distance_xyz(from, candidate_moby + 0x10);
        if (!(candidate_distance < range)) continue;
        qcopy(tp, candidate_moby + 0x10);
        tp[2] += 0.4f;
        score = fast_difference_between_rotations_c(((float *)reference_rotation)[2], fast_arc_tan(tp[0] - ((float *)from)[0], tp[1] - ((float *)from)[1]));
        score = score * score;
        if (!(score < yawMax * yawMax)) {
            if (!(candidate_distance < near) || !(score < nearYaw * nearYaw)) continue;
        }
        pitch_error_squared = fast_difference_between_rotations_c(((float *)reference_rotation)[1], fast_arc_tan(candidate_distance, tp[2] - ((float *)from)[2]));
        pitch_error_squared = pitch_error_squared * pitch_error_squared;
        if (!(pitch_error_squared < pitchMax * pitchMax)) {
            if (!(candidate_distance < near) || !(pitch_error_squared < nearPitch * nearPitch)) continue;
        }
        score *= pitch_error_squared;
        score *= square_root(distance_xyz(from, tp));
        if (!(score < bestScore)) continue;
        if (func_L00_001EFFF0_t(from, tp, 0, moby, 0)) {
            char *hit = *(char **)(hs + 0x18);
            if (hit == 0 || hit == ((char *)pl->moby) || hit == 0 || *(char **)(hit + 0x24) == 0
                || *(short *)(*(char **)(hit + 0x24) + 0x46) != candidate_class) {
                continue;
            }
        }
        bestScore = score;
        best = candidate_moby;
    }
    return best;
}
#include "qcopy.h"
#include "qzero.h"
#include "rnc/overlay/quad.h"

extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern void *select_sand_mouse_spawn_target(void *, void *, void *, float, float, float, float, float, float) __asm__("FUN_L01_002e32a8");
extern void spawn_sandmouse_projectile(void *, void *, float, float, float, float) __asm__("FUN_L01_0030c898");

extern float sand_mouse_spawn_target_range_gp __asm__("D_L01_00161904") __attribute__((sda));
extern float sand_mouse_spawn_target_yaw_limit_degrees_gp __asm__("D_L01_00161908") __attribute__((sda));
extern float sand_mouse_spawn_target_pitch_limit_degrees_gp __asm__("D_L01_0016190C") __attribute__((sda));
extern float sand_mouse_spawn_offset_toward_target_distance_gp __asm__("D_L01_00161910") __attribute__((sda));
extern float sand_mouse_spawn_and_fallback_aim_height_offset_gp __asm__("D_L01_00161914") __attribute__((sda));
extern int sand_mouse_projectile_spawn_interval_frames_gp __asm__("D_L01_00161900") __attribute__((sda));
extern int cutscene_mode __asm__("D_L01_0015F5C4");

typedef union {
    OvlQuad q;
    float f[4];
} V4_w22c;

void update_sand_mouse_targeted_spawning(char *moby, char *state) __asm__("FUN_L01_002e35a8");

void update_sand_mouse_targeted_spawning(char *moby, char *state) {
    V4_w22c spawn_position;
    V4_w22c target_aim_position;
    V4_w22c spawn_offset;
    char *target_moby;
    float aim_yaw, aim_pitch, initial_yaw, initial_pitch, target_yaw, spawn_to_target_distance;

    if (fast_dec_timer_s16(state + 0x4E) == 0)
        return;
    qzero(&spawn_position);
    fast_vec_add(&spawn_position, &spawn_position, moby + 0x10);
    spawn_position.f[2] = spawn_position.f[2] + sand_mouse_spawn_and_fallback_aim_height_offset_gp;
    target_moby = select_sand_mouse_spawn_target(moby, &spawn_position, moby + 0x40, sand_mouse_spawn_target_yaw_limit_degrees_gp * DEG_TO_RAD,
                         sand_mouse_spawn_target_pitch_limit_degrees_gp * DEG_TO_RAD, sand_mouse_spawn_target_range_gp, 2.0f, 3.1415927f,
                         3.1415927f);
    if (target_moby != 0) {
        *(char **)(state + 0x68) = target_moby;
        if (hero.motion.pos.f[2] + 1.1f < *(float *)(moby + 0x18)) {
            target_yaw = fast_arc_tan(*(float *)(target_moby + 0x10) - *(float *)(moby + 0x10),
                             *(float *)(target_moby + 0x14) - *(float *)(moby + 0x14));
            if (fast_difference_between_rotations(*(float *)(moby + 0x48), target_yaw) < 0.5235988f) {
                if (cutscene_mode != 2) {
                    char *target_aim_data;
                    qcopy(&target_aim_position, target_moby + 0x10);
                    target_aim_data = suck_get_aim_height(target_moby);
                    if (target_aim_data != 0)
                        target_aim_position.f[2] = target_aim_position.f[2] + *(float *)(target_aim_data + 0x10);
                    else
                        target_aim_position.f[2] = target_aim_position.f[2] + sand_mouse_spawn_and_fallback_aim_height_offset_gp;
                    fast_vec_sub(&spawn_offset, &target_aim_position, &spawn_position);
                    fast_vec_normalize(&spawn_offset, &spawn_offset, sand_mouse_spawn_offset_toward_target_distance_gp);
                    fast_vec_add(&spawn_position, &spawn_position, &spawn_offset);
                    aim_yaw = fast_arc_tan(target_aim_position.f[0] - spawn_position.f[0], target_aim_position.f[1] - spawn_position.f[1]);
                    spawn_to_target_distance = vector_distance(&spawn_position, &target_aim_position);
                    aim_pitch = -fast_arc_tan(spawn_to_target_distance, target_aim_position.f[2] - spawn_position.f[2]);
                    initial_yaw = fast_arc_tan(target_aim_position.f[0] - spawn_position.f[0], target_aim_position.f[1] - spawn_position.f[1]);
                    spawn_to_target_distance = vector_distance(&spawn_position, &target_aim_position);
                    initial_pitch = -fast_arc_tan(spawn_to_target_distance, target_aim_position.f[2] - spawn_position.f[2]);
                    spawn_sandmouse_projectile(moby, &spawn_position, aim_yaw, aim_pitch, initial_yaw, initial_pitch);
                    *(short *)(state + 0x4E) = scale_game_frames(sand_mouse_projectile_spawn_interval_frames_gp);
                    *(int *)(state + 0x64) = *(int *)(state + 0x64) + 1;
                }
            }
        }
    } else {
        *(int *)(state + 0x68) = 0;
    }
}
extern f32 D_L01_00161944 __attribute__((sda));
extern f32 D_L01_00161948 __attribute__((sda));
extern f32 D_L01_0016194C __attribute__((sda));
extern f32 D_L01_00161920 __attribute__((sda));
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern void clamp_vector_length(void *, void *, f32) __asm__("FUN_001f9c90");
extern f32 probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern void blend_moby_animation_3830(void *, int, int, int) __asm__("FUN_00212f90");
extern int random_integer_3830(void) __asm__("FUN_001160d8");

void update_hover_moby_steering(struct Moby *moby, char *movement_state) __asm__("FUN_L01_002e3830");

void update_hover_moby_steering(struct Moby *moby, char *movement_state) {
    enum {
        HOVER_FACE_GOAL_AND_WAIT = 0,
        HOVER_MOVE_TOWARD_GOAL = 2
    };
    Vec4 velocity_adjustment;
    f32 reference_distance, direction_yaw, movement_magnitude, ground, near;
    s32 hit;
    s16 blocked_frames;
    struct Moby *target;

    subtract_vector_xyz(&velocity_adjustment, movement_state, &moby->pos);
    if (moby->state == 6)
        reference_distance = vector_distance(&moby->pos, movement_state + 0x30);
    else
        reference_distance = vector_distance(&moby->pos, &hero.motion.pos);
    vector_distance(&moby->pos, movement_state);

    switch (moby->command) {
    case HOVER_FACE_GOAL_AND_WAIT:
        direction_yaw = fast_arc_tan(*(f32 *)(movement_state + 0) - moby->pos.x, *(f32 *)(movement_state + 4) - moby->pos.y);
        FUN_L00_00258278(moby, direction_yaw, &D_L01_001DEB4C, frame_time_sq * 11.519173f,
                         frame_time_sq * 3.1415927f, frame_time * 11.519173f);
        if (moby->anim_flags & 2) {
            moby->anim_speed = 1.0f;
            if (moby->prev_seq != random_integer_3830() % 3) {
                int seq = random_integer_3830() % 3;
                blend_moby_animation_3830(moby, seq, 0, scale_game_frames(20));
            }
        }
        if (1.3f < reference_distance) {
            if (moby->prev_seq != 5)
                blend_moby_animation_3830(moby, 5, 0, scale_game_frames(10));
            moby->command = 2;
        }
        return;
    case HOVER_MOVE_TOWARD_GOAL:
    case 3:
        break;
    default:
        goto inactive;
    }

    target = *(struct Moby **)(movement_state + 0x68);
    if (target == 0) {
        direction_yaw = fast_arc_tan(*(f32 *)(movement_state + 0) - moby->pos.x, *(f32 *)(movement_state + 4) - moby->pos.y);
        FUN_L00_00258278(moby, direction_yaw, &D_L01_001DEB4C, frame_time_sq * 11.519173f,
                         frame_time_sq * 3.1415927f, frame_time * 11.519173f);
    } else {
        direction_yaw = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
        FUN_L00_00258278(moby, direction_yaw, &D_L01_001DEB4C,
                         D_L01_00161944 * DEG_TO_RAD * frame_time_sq,
                         D_L01_00161948 * DEG_TO_RAD * frame_time_sq,
                         D_L01_0016194C * DEG_TO_RAD * frame_time);
    }
    scale_vector_xyz(&velocity_adjustment, &velocity_adjustment, frame_scale * 0.04f);
    subtract_vector_xyz(&velocity_adjustment, &velocity_adjustment, ((f32 *)(movement_state + 0x10)));
    if (1.3f < reference_distance) {
        scale_vector_xyz(&velocity_adjustment, &velocity_adjustment, frame_scale_sq * 0.13f);
    } else {
        fast_vec_scale(&velocity_adjustment, &velocity_adjustment, frame_scale_sq * 0.25f);
    }
    velocity_adjustment.f[2] *= frame_scale * -0.100000024f + 1.0f;
    add_vector_xyz(((f32 *)(movement_state + 0x10)), ((f32 *)(movement_state + 0x10)), &velocity_adjustment);
    if (moby->state == 6)
        clamp_vector_length(((f32 *)(movement_state + 0x10)), ((f32 *)(movement_state + 0x10)), frame_time * 2.7f);
    hit = move_moby_with_collision_integer_radius(moby, ((f32 *)(movement_state + 0x10)), D_L01_001DEB38.count, 0,
                            1.0f, D_L01_001DEB38.spread);
    movement_magnitude = fast_vec_length(((f32 *)(movement_state + 0x10)));
    if (hit != 0) {
        if (movement_magnitude < 0.005f) {
            blocked_frames = ++*(s16 *)(movement_state + 0x4a);
            if (scale_game_frames(20) < blocked_frames && moby->visible == 0) {
                qcopy(&moby->pos, movement_state);
                *(s16 *)(movement_state + 0x4a) = 0;
            }
        }
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        if ((hit & 2) || moby->pos.z - ground < D_L01_00161920) {
            if (movement_magnitude < 0.005f) {
                if (moby->prev_seq >= 3 && moby->prev_seq != random_integer_3830() % 3) {
                    int seq = random_integer_3830() % 3;
                    blend_moby_animation_3830(moby, seq, 0, scale_game_frames(10));
                }
            } else {
                goto animation3;
            }
        } else {
            if (moby->prev_seq != 5)
                blend_moby_animation_3830(moby, 5, 0, scale_game_frames(7));
        }
    } else {
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        if (moby->pos.z - ground < D_L01_00161920) {
 animation3:
            if (moby->prev_seq != 3)
                blend_moby_animation_3830(moby, 3, 0, scale_game_frames(11));
        } else {
            if (moby->prev_seq != 5)
                blend_moby_animation_3830(moby, 5, 0, scale_game_frames(7));
            *(s16 *)(movement_state + 0x4a) = 0;
        }
    }
collision_done:
    if (moby->state == 6 || !(reference_distance < 1.1f)) return;
    near = 1.1f - reference_distance;
    direction_yaw = fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y);
    ((f32 *)(movement_state + 0x10))[0] -= near * fast_cos(direction_yaw) * (frame_scale_sq * 0.1f);
    ((f32 *)(movement_state + 0x10))[1] -= near * fast_sin(direction_yaw) * (frame_scale_sq * 0.1f);
    return;
inactive:
    moby->command = 0;
    if (1.3f < reference_distance) moby->command = 2;
}
/* State for the parented moby.  The parent index is -1 when unattached. */
typedef int u128_2e3f20 __attribute__((mode(TI)));
typedef struct {
    float target[4], vel[4], start[4], home[4];
    int vol;
    char pad44[4];
    unsigned char b48;
    char pad49;
    short s4A, timer, s4E;
    int parent;
    char pad54[0xC];
    int t60, count;
    void *tgt;
} Data_2e3f20;
typedef struct {
    int unk0;
    float unk4, unk8, unkC, unk10, yaw_speed, unk18, unk1C, unk20, unk24, unk28, unk2C, unk30;
    int unk34, unk38;
    float unk3C, unk40, unk44;
    int unk48, unk4C;
    void *owner;
} Mover_2e3f20;
typedef struct {
    char pad0[0x80];
    float pos[4];
    char pad90[0x10A0];
    struct Moby *helper;
    char pad1134[0x24];
    int i1158;
    char pad115C[0xF24];
    void *p2080;
    char pad2084[8];
    int i208C;
    char pad2090[0x14];
    unsigned char b20A4;
} Hero_2e3f20;
#define hero_2e3f20 ((Hero_2e3f20 *)&hero)
#define mover_2e3f20 (*(Mover_2e3f20 *)&D_L01_001DEB38)
extern struct Moby *ents_2e3f20 __asm__("D_L01_0015FFD8");
extern int D_0015ED84;
extern int D_L01_00161918 __attribute__((sda));
extern int D_L01_0016191C __attribute__((sda));
extern int D_L01_00161924 __attribute__((sda));
extern int D_L01_00161928 __attribute__((sda));
extern int D_L01_0016192C __attribute__((sda));
extern float D_L01_00161930 __attribute__((sda));
extern float D_L01_00161934 __attribute__((sda));
extern float D_L01_00161938 __attribute__((sda));
extern int D_L01_0016193C __attribute__((sda));
extern float D_L01_00161940 __attribute__((sda));
extern char D_L01_0020AE80[];
extern char D_L01_0020AEA0[];
extern void debug_print_2e3f20(char *) __asm__("FUN_001e93b0");
extern void initialize_mover_2e3f20(void *) __asm__("FUN_L01_0026d930");
extern void rig_probe_setup(void *, int, int *, float *, float *, int *) __asm__("FUN_L01_00263a28");
extern void face_target_2e3f20(struct Moby *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern int FUN_L01_0023cf98(int, int);
extern void play_sound_2e3f20(int, int, float) __asm__("FUN_L00_002323b8");
extern int step_mover_2e3f20(void *, void *, float *, float *) __asm__("FUN_L00_00259028");
extern void add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");
extern void spawn_particle_2e3f20(float *, int, int, int, int, float *, float, float, float) __asm__("FUN_L00_00272060");
extern void finish_moby_2e3f20(void *, float) __asm__("FUN_L00_00259fe8");
extern int random_integer_3830(void) __asm__("FUN_001160d8");
extern void blend_moby_animation_3830(void *, int, int, int) __asm__("FUN_00212f90");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int FUN_L00_00257b90(int, int);
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void compute_sand_mouse_approach_target(int, void *) __asm__("FUN_L01_002e30b8");
extern void sand_mouse_compute_target(char *, char *) __asm__("FUN_L01_002e3110");
extern s32 update_sand_mouse_fall_motion(void *, struct Moby *) __asm__("FUN_L01_002e3208");
extern void update_sand_mouse_targeted_spawning(char *, char *) __asm__("FUN_L01_002e35a8");
extern void update_hover_moby_steering(struct Moby *, char *) __asm__("FUN_L01_002e3830");
extern unsigned char D_0013D4C5 __attribute__((section(".data")));
void sand_mouse_update(struct Moby *moby) __asm__("FUN_L01_002e3f20");
void sand_mouse_update(struct Moby *moby) {
    enum {
        SAND_MOUSE_INITIALIZE = 0,
        SAND_MOUSE_WAIT_FOR_HERO_IN_VOLUME = 1,
        SAND_MOUSE_WAIT_FOR_PARENT_READY = 2,
        SAND_MOUSE_MOVE_TOWARD_HERO = 3,
        SAND_MOUSE_BEGIN_FOLLOW_HERO = 4,
        SAND_MOUSE_FOLLOW_HERO = 5,
        SAND_MOUSE_FADE_OUT_AND_RESET_HOME = 6,
        SAND_MOUSE_DAMP_VELOCITY_BEFORE_RETURN = 19,
        SAND_MOUSE_RETURN_TO_PARENT = 20
    };
    Data_2e3f20 *sand_mouse_vars;
    float va[4];
    float vb[4];
    float desired_facing_yaw;

    if (moby == 0) {
        return;
    }
    sand_mouse_vars = *(Data_2e3f20 **)&moby->pvars;
    if (sand_mouse_vars == 0) {
        return;
    }
    if (sand_mouse_vars->parent != -1) {
        if (moby->state >= 5) {
            ((Data_2e3f20 *)ents_2e3f20[sand_mouse_vars->parent].pvars)->target[0] =
                fast_arc_tan(moby->pos.x - ents_2e3f20[sand_mouse_vars->parent].pos.x,
                                  moby->pos.y - ents_2e3f20[sand_mouse_vars->parent].pos.y);
        } else {
            ((Data_2e3f20 *)ents_2e3f20[sand_mouse_vars->parent].pvars)->target[0] =
                fast_arc_tan(hero_2e3f20->pos[0] - ents_2e3f20[sand_mouse_vars->parent].pos.x,
                                  hero_2e3f20->pos[1] - ents_2e3f20[sand_mouse_vars->parent].pos.y);
        }
        if (sand_mouse_vars->parent != -1) {
            *(int *)&ents_2e3f20[sand_mouse_vars->parent].collision = 0;
        }
    }
    switch (moby->state) {
    case SAND_MOUSE_INITIALIZE:
        qzero(sand_mouse_vars->target);
        qzero(sand_mouse_vars->vel);
        sand_mouse_vars->s4A = 0;
        sand_mouse_vars->timer = 0;
        if (moby->prev_seq != 0) {
            blend_moby_animation_3830(moby, 0, 0, scale_game_frames(10));
        }
        moby->update_dist = 0xFF;
        moby->draw_dist = 0xFF;
        if (sand_mouse_vars->parent != -1) {
            unsigned short id;
            qcopy(((float *)&moby->pos), ((float *)&ents_2e3f20[sand_mouse_vars->parent].pos));
            moby->pos.z = moby->pos.z + 0.02f;
            if (D_L01_001BB6B0.collected[(short)(id = moby->save_id)] != 0
                || (*(int *)((char *)D_0014C190_257218 + (((short)id >> 5) * 4 + (D_0015ED84 << 8))) >> (id & 0x1F)) & 1) {
                debug_print_2e3f20(D_L01_0020AE80);
                mark_moby_for_removal(moby);
                return;
            }
        } else {
            debug_print_2e3f20(D_L01_0020AEA0);
            mark_moby_for_removal(moby);
            return;
        }
        qcopy(sand_mouse_vars->start, ((float *)&moby->pos));
        qcopy(sand_mouse_vars->home, ((float *)&moby->pos));
        initialize_mover_2e3f20((char *)&mover_2e3f20);
        {
            int kind;
            float collision_probe_extent;
            int flag;
            rig_probe_setup(moby, 0, &kind, va, &collision_probe_extent, &flag);
            mover_2e3f20.yaw_speed = frame_scale * 0.4f;
            mover_2e3f20.unk8 = 0.15f;
            mover_2e3f20.unkC = 0.15f;
            mover_2e3f20.unk10 = frame_scale * 0.01f;
            mover_2e3f20.unk24 = frame_time * 2.7f;
            mover_2e3f20.unk1C = frame_scale * 0.01f;
            mover_2e3f20.unk0 = truncate_float_to_s32(collision_probe_extent * 1024.0f) + 1;
            mover_2e3f20.unk38 |= 8;
            mover_2e3f20.unk4 = 0.3f;
            mover_2e3f20.unk30 = 0.005f;
            mover_2e3f20.unk3C = frame_time_sq * 11.519173f;
            mover_2e3f20.unk40 = frame_time_sq * 3.1415927f;
            mover_2e3f20.unk44 = frame_time * 11.519173f;
            mover_2e3f20.unk48 = scale_game_frames(4);
            mover_2e3f20.unk28 = 3.1415927f;
        }
        moby->state = 1;
        moby->draw_dist = 0;
        break;
    case SAND_MOUSE_WAIT_FOR_HERO_IN_VOLUME:
        if (D_0013D4C5 != 0 && hero_2e3f20->i1158 == 5) {
            sand_mouse_vars->count = 0;
            sand_mouse_vars->t60 = scale_game_frames(D_L01_00161918);
            if (moby->visible != 0 && (moby->anim_flags & 2)) {
                if (moby->prev_seq != random_integer_3830() % 3) {
                    blend_moby_animation_3830(moby, random_integer_3830() % 3, 0, scale_game_frames(10));
                }
            }
            if (sand_mouse_vars->parent != -1) {
                if (is_point_inside_clip_volume(hero_2e3f20->pos, sand_mouse_vars->vol)) {
                    ents_2e3f20[sand_mouse_vars->parent].command = 3;
                } else {
                    ents_2e3f20[sand_mouse_vars->parent].command = 1;
                }
            }
            if (hero_2e3f20->helper != 0 && hero_2e3f20->i1158 == 5) {
                desired_facing_yaw = fast_arc_tan(hero_2e3f20->pos[0] - moby->pos.x,
                                        hero_2e3f20->pos[1] - moby->pos.y);
                face_target_2e3f20(moby, desired_facing_yaw, &mover_2e3f20.yaw_speed,
                                        frame_time_sq * 11.519173f, frame_time_sq * 3.1415927f, frame_time * 11.519173f);
                qcopy(va, ((float *)&ents_2e3f20[sand_mouse_vars->parent].pos));
                va[2] = va[2] + 0.5f;
                qcopy(vb, hero_2e3f20->pos);
                vb[2] = vb[2] + 0.2f;
                if (is_point_inside_clip_volume(hero_2e3f20->pos, sand_mouse_vars->vol)) {
                    if (!collision_line(va, vb, 0, moby, 0)
                        || D_L01_001742D8 == hero_2e3f20->p2080) {
                        compute_sand_mouse_approach_target((int)moby, sand_mouse_vars);
                        if (hero_2e3f20->helper != 0
                            && hero_2e3f20->helper->oclass == 0x1B1
                            && hero_2e3f20->helper->state != 0xFE
                            && hero_2e3f20->helper->state != 0xFD) {
                            hero_2e3f20->helper->state = 1;
                        }
                        *(void **)((char *)&mover_2e3f20 + 0x50) = moby;
                        FUN_L01_0023cf98(0x1F, 0);
                        play_sound_2e3f20(0, 0, (float)scale_game_frames(6));
                        if (sand_mouse_vars->parent != -1) {
                            ents_2e3f20[sand_mouse_vars->parent].command = 5;
                            moby->state = 2;
                        } else {
                            moby->draw_dist = 0xFF;
                            moby->state = 3;
                        }
                    }
                }
            }
        }
        break;
    case SAND_MOUSE_WAIT_FOR_PARENT_READY:
        if (sand_mouse_vars->parent != -1) {
            if (ents_2e3f20[sand_mouse_vars->parent].state == 5) {
                moby->state = 3;
                sand_mouse_vars->timer = 0;
                moby->draw_dist = 0xFF;
            }
        } else {
            sand_mouse_vars->timer = 0;
            moby->draw_dist = 0xFF;
            moby->state = 3;
        }
        break;
    case SAND_MOUSE_MOVE_TOWARD_HERO:
        compute_sand_mouse_approach_target((int)moby, sand_mouse_vars);
        desired_facing_yaw = fast_arc_tan(sand_mouse_vars->target[0] - moby->pos.x, sand_mouse_vars->target[1] - moby->pos.y);
        face_target_2e3f20(moby, desired_facing_yaw, &mover_2e3f20.yaw_speed,
                                frame_time_sq * 11.519173f, frame_time_sq * 3.1415927f, frame_time * 11.519173f);
        if ((step_mover_2e3f20(moby, &mover_2e3f20, sand_mouse_vars->target, sand_mouse_vars->vel) & 6)
            || vector_distance(((float *)&moby->pos), hero_2e3f20->pos) < 1.0f) {
            qzero(sand_mouse_vars->vel);
            moby->state = 4;
            sand_mouse_vars->timer = scale_game_frames(0x78);
        }
        if (vector_distance(((float *)&moby->pos), sand_mouse_vars->home) > 0.4f && sand_mouse_vars->timer == 0) {
            *(float *)((char *)&mover_2e3f20 + 4) = 0.7f;
            if (moby->prev_seq != 3) {
                blend_moby_animation_3830(moby, 3, 0, scale_game_frames(3));
            }
        }
        break;
    case SAND_MOUSE_BEGIN_FOLLOW_HERO:
        FUN_L01_0023cf98(0, 1);
        moby->flags &= 0xFFBE;
        if (sand_mouse_vars->parent != -1) {
            ents_2e3f20[sand_mouse_vars->parent].flags &= 0xFFBE;
        }
        moby->state = 5;
        if (hero_2e3f20->helper != 0
            && hero_2e3f20->helper->oclass == 0x1B1
            && hero_2e3f20->helper->state != 0xFE
            && hero_2e3f20->helper->state != 0xFD) {
            sand_mouse_vars->b48 = 0xFF;
            hero_2e3f20->helper->state = 2;
        }
        break;
    case SAND_MOUSE_FOLLOW_HERO:
        if (hero_2e3f20->helper != 0
            && hero_2e3f20->i1158 == 5
            && hero_2e3f20->i208C != 0x16
            && hero_2e3f20->i208C != 0x14
            && hero_2e3f20->b20A4 == 0
            && !fast_dec_timer(&sand_mouse_vars->t60)
            && sand_mouse_vars->count < D_L01_0016191C) {
            sand_mouse_compute_target((char *)moby, (char *)sand_mouse_vars);
            update_hover_moby_steering(moby, sand_mouse_vars);
        } else {
            if (moby->prev_seq != 5) {
                blend_moby_animation_3830(moby, 5, 0, scale_game_frames(10));
            }
            qzero(sand_mouse_vars->vel);
            sand_mouse_vars->timer = D_L01_00161924;
            moby->state = 6;
            if (sand_mouse_vars->parent != -1) {
                ents_2e3f20[sand_mouse_vars->parent].command = 5;
            }
        }
        update_sand_mouse_targeted_spawning((char *)moby, (char *)sand_mouse_vars);
        break;
    case SAND_MOUSE_FADE_OUT_AND_RESET_HOME:
        if (fast_dec_timer_s16(&sand_mouse_vars->timer)) {
            moby->alpha = 0x80;
            qcopy(((float *)&moby->pos), sand_mouse_vars->home);
            qcopy(sand_mouse_vars->target, sand_mouse_vars->home);
            qzero(sand_mouse_vars->vel);
            moby->state = 0x13;
            moby->flags &= 0xFDFF;
        } else {
            int cr;
            int cg;
            int cb;
            int col;
            int particle_random_remainder;
            int particle_random_integer;
            float particle_random_float;
            *(u128_2e3f20 *)vb = 0;
            qcopy(va, ((float *)&moby->pos));
            add_random_offset_to_vector_xyz(va, D_L01_00161930);
            cr = (random_integer_3830() + 0x30) & 0x3F;
            cg = (random_integer_3830() + 0x20) & 0x3F;
            cb = random_integer_3830() & 0x2F;
            col = cr | (cg << 8) | (cb << 16) | (D_L01_0016193C << 24);
            particle_random_remainder = random_integer_3830() % 2;
            if (sand_mouse_vars->timer & 1) {
                particle_random_remainder = -particle_random_remainder;
            }
            particle_random_integer = FUN_L00_00257b90(D_L01_00161928, D_L01_0016192C);
            particle_random_float = random_float_between_alt(D_L01_00161934, D_L01_00161938);
            spawn_particle_2e3f20(va, particle_random_integer, col, 2, particle_random_remainder, vb, particle_random_float, 0.001f, D_L01_00161940 * frame_time_sq);
            moby->flags |= 0x200;
            moby->alpha -= 0x80 / D_L01_00161924;
            moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 25.132742f);
        }
        break;
    case SAND_MOUSE_DAMP_VELOCITY_BEFORE_RETURN:
        {
            float velocity_damping_factor = frame_scale * -0.050000012f + 1.0f;
            sand_mouse_vars->vel[0] = sand_mouse_vars->vel[0] * velocity_damping_factor;
            sand_mouse_vars->vel[1] = sand_mouse_vars->vel[1] * velocity_damping_factor;
        }
        if (update_sand_mouse_fall_motion(moby, sand_mouse_vars) & 2) {
            qzero(sand_mouse_vars->vel);
            qcopy(sand_mouse_vars->target, ((float *)&ents_2e3f20[sand_mouse_vars->parent].pos));
            moby->state = 0x14;
        }
        break;
    case SAND_MOUSE_RETURN_TO_PARENT:
        {
            float z;
            int mover_result_flags;
            mover_2e3f20.owner = 0;
            desired_facing_yaw = fast_arc_tan(sand_mouse_vars->target[0] - moby->pos.x, sand_mouse_vars->target[1] - moby->pos.y);
            face_target_2e3f20(moby, desired_facing_yaw, &mover_2e3f20.yaw_speed,
                                    frame_time_sq * 11.519173f, frame_time_sq * 3.1415927f, frame_time * 11.519173f);
            z = moby->pos.z;
            mover_result_flags = step_mover_2e3f20(moby, &mover_2e3f20, sand_mouse_vars->target, sand_mouse_vars->vel);
            moby->pos.z = z;
            if (mover_result_flags & 4) {
                if (moby->prev_seq != 0) {
                    blend_moby_animation_3830(moby, 0, 0, scale_game_frames(6));
                }
                ents_2e3f20[sand_mouse_vars->parent].command = 1;
                moby->state = 1;
                moby->draw_dist = 0;
            }
        }
        break;
    case 21:
        break;
    }
    finish_moby_2e3f20(moby, 0.3f);
    if (sand_mouse_vars->parent != -1) {
        *(int *)&ents_2e3f20[sand_mouse_vars->parent].collision = ents_2e3f20[sand_mouse_vars->parent].pclass->collision;
    }
}
/* Burning debris: flies along its velocity leaving two smoke trails, sticks to the ground, and hurts what it hits. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002EEDD8), where it is exact; names translated to the US level program. */

typedef struct {
    float v[2];
    float f8;
    float fC;
    char pad10[8];
    char b18;
    char b19;
    short s1A;
    char pad1C[0x14];
} Hit_2eedd8;

extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void clear_vector(void *) __asm__("func_001F99F8");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern char D_L01_00174340_c[] __asm__("D_L01_001742C0");

void update_moby_motion_collision_and_burst(char *moby) __asm__("FUN_L01_002eda00");

void update_moby_motion_collision_and_burst(char *moby) {
    enum {
        MOVE_UNTIL_COLLISION_OR_TIMEOUT = 1,
        BURST_AND_REMOVE = 2
    };
    char *moby_vars = *(char **)(moby + 0x78);
    float previous_position[4];
    float z[4];
    Hit_2eedd8 hit_descriptor;
    float gz;
    switch (((unsigned char *)moby)[0x20]) {
    case MOVE_UNTIL_COLLISION_OR_TIMEOUT:
        qcopy(previous_position, moby + 0x10);
        add_vector_xyz(moby + 0x10, moby + 0x10, moby_vars);
        *(float *)(moby + 0x48) = fast_arc_tan(*(float *)(moby_vars + 0), *(float *)(moby_vars + 4));
        clear_vector(z);
        FUN_L00_00269958(moby + 0x10, z, 0x6F00AFFF, 0xFF,
                         scale_game_frames(FUN_L00_00257b90(0xF, 0x16)), 0x28,
                         FUN_L00_00257b90(0x14, 0x23), 1);
        FUN_L00_00269958(moby + 0x10, z, 0x1FFFFFFF, 0x4F4F4F,
                         scale_game_frames(FUN_L00_00257b90(0x1E, 0x3C)), 0x28,
                         FUN_L00_00257b90(0x32, 0x4B), 0);
        gz = *(float *)(moby + 0x18) - probe_ground_height(moby + 0x10, 0, 0.5f);
        if (gz < *(float *)(moby_vars + 0x18)) {
            *(float *)(moby + 0x18) = *(float *)(moby + 0x18) + (*(float *)(moby_vars + 0x18) - gz);
            *(int *)(moby_vars + 8) = 0;
        } else if (*(float *)(moby + 0x18) < hero.ground_point.f[2] + *(float *)(moby_vars + 0x18)) {
            *(int *)(moby_vars + 8) = 0;
        } else {
            *(float *)(moby_vars + 8) -= frame_time_sq * 10.8f;
        }
        if (collision_line(moby + 0x10, previous_position, 0, *(int *)(moby_vars + 0x10), 0) != 0) {
            char *collision_result = D_L01_00174340_c;
            if (*(int *)(collision_result + 0x18) != 0) {
                make_hit_descriptor(&hit_descriptor, moby, 0x10001, 1.0f, moby_vars);
                set_vector_length_xy(&hit_descriptor, &hit_descriptor, 1.0f);
                hit_descriptor.f8 = 1.0f;
                hit_descriptor.fC = 5627.9248f;
                hit_descriptor.b19 = 1;
                hit_descriptor.s1A = *(unsigned short *)(moby + 0xA6);
                hit_descriptor.b18 = 0;
                record_moby_hit(*(void **)(collision_result + 0x18), &hit_descriptor);
            }
            moby[0x20] = 2;
        }
        if (tick_countdown_32_alt(moby_vars + 0x14) != 0)
            moby[0x20] = 2;
        break;
    case BURST_AND_REMOVE:
        cart_burst(moby, moby + 0x10, -1, 0.25f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}
extern void FUN_L00_00263ac8(float, void *, int, void *);
extern void FUN_L00_0025a120(void *);
extern void process_amoeboid_hits_and_update_targeting(char *) __asm__("FUN_L01_002eec68");
void spawn_amoeboid_movement_particles(unsigned char *moby) __asm__("FUN_L01_002ef560");

extern void spawn_amoeboid_movement_particles(unsigned char *) __asm__("FUN_L01_002ef560");
void spawn_amoeboid_burst_sparks(unsigned char *moby, f32 *mode) __asm__("FUN_L01_002ef770");

extern void spawn_amoeboid_burst_sparks(unsigned char *, float *) __asm__("FUN_L01_002ef770");
extern int follow_path(int *, int, int, float, void *, void *, void *) __asm__("FUN_L01_00276fe8");
extern void FUN_L01_0026d930(void *);
extern void FUN_L00_00258b50(void *, void *, void *, void *, float);
extern float approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L00_00261b48(int, void *, void *, void *);
extern void spawn_nearby_effect(void *, void *, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern int FUN_L00_002dc9f0(void *, void *);
extern int FUN_L00_00257470(void *, int, int);
extern int FUN_00214cc8(float, void *);
extern int FUN_00214720(void *, int);
extern void FUN_0022da68(int, int, char *);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int FUN_00213260(int);
extern float FUN_00213308(void);
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float FUN_001f9b48(void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float FUN_001f99c0(float);
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int FUN_001fa6d0(float);
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern void FUN_0020c828(void *);
extern float frame_time_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float D_0015ED70 __attribute__((section(".sdata")));
extern float D_0013F420[4];
extern float D_L01_0015F580[] __attribute__((section(".sdata")));

extern char D_L01_00161A40 __attribute__((sda));
extern char D_L01_00161A50 __attribute__((sda));
extern float D_L01_001619AC __attribute__((sda));

void large_amoeboid_update(unsigned char *moby) __asm__("FUN_L01_002edca0");
void large_amoeboid_update(unsigned char *moby) {
    enum {
        LARGE_AMOEBOID_INITIALIZE = 0,
        LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH = 1,
        LARGE_AMOEBOID_MOVE_TOWARD_STORED_POSITION = 2,
        LARGE_AMOEBOID_WAIT_FOR_PATH_ACQUISITION_ANIMATION_END = 3,
        LARGE_AMOEBOID_FOLLOW_PATH_TO_TARGET = 4,
        LARGE_AMOEBOID_RETURN_TO_STORED_POSITION = 5,
        LARGE_AMOEBOID_LUNGE_AT_TARGET = 6,
        LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESUMING_PATH = 7,
        LARGE_AMOEBOID_PROCESS_KNOCKBACK = 8,
        LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESTARTING_PATH = 9,
        LARGE_AMOEBOID_WAIT_FOR_ACTIVATION_VOLUME = 13,
        LARGE_AMOEBOID_PROCESS_SUCK_CANNON_INTERACTION = 14,
        LARGE_AMOEBOID_MOVE_DIRECTLY_TOWARD_TARGET = 15,
        LARGE_AMOEBOID_WAIT_WHILE_BLOCKED = 16
    };
    enum {
        LARGE_AMOEBOID_LARGE_VARIANT = 0x23c,
        LARGE_AMOEBOID_MEDIUM_VARIANT = 0x361,
        LARGE_AMOEBOID_SMALL_VARIANT = 0x362
    };
    char *data = *(char **)(moby + 0x78);
    char *target;
    char *target_position;
    float move[4];
    float hit[4];
    float angle;
    float distance;
    int path_hit;
    int blocked;
    int result;

    if (moby[0x20] == 12) {
        *(u16 *)(moby + 0x34) |= 1;
        return;
    }
    FUN_L00_00263ac8(2.9f, moby, 0, data + 0x270);
    if (moby[0x31] != 0 && FUN_001f9b48(moby + 0x10, D_L01_00167240) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby[0x7f] = 0x16;
    }
    process_amoeboid_hits_and_update_targeting((char *)moby);
    target = *(char **)(data + 0x200);
    switch (moby[0x20]) {
    case LARGE_AMOEBOID_INITIALIZE:
        qcopy(data + 0x210, moby + 0x10);
        switch (*(short *)(moby + 0xa6)) {
        case LARGE_AMOEBOID_LARGE_VARIANT:
            data[0x58] = FUN_001fa6d0(14.0f);
            data[0x5a] = FUN_001fa6d0(10.4f);
            *(float *)(data + 0x250) = 1.0f;
            break;
        case LARGE_AMOEBOID_MEDIUM_VARIANT:
            data[0x58] = FUN_001fa6d0(14.0f);
            data[0x5a] = FUN_001fa6d0(5.6f);
            *(float *)(data + 0x250) = 0.7f;
            break;
        case LARGE_AMOEBOID_SMALL_VARIANT:
            data[0x58] = FUN_001fa6d0(14.0f);
            data[0x5a] = FUN_001fa6d0(4.0f);
            *(float *)(data + 0x250) = 0.49f;
            break;
        }
        *(float *)(data + 0x30) = *(float *)(data + 0x250);
        *(float *)(data + 0x254) = FUN_00213308();
        if (*(short *)(moby + 0xa6) == 0x362)
            *(void **)(data + 0x130) = &D_L01_00161A50;
        else
            *(void **)(data + 0x130) = &D_L01_00161A40;
        data[0x29] = 1;
        if (*(int *)(data + 0x258) != 0) {
            *(float *)(data + 0x20) = 1.0f;
            *(int *)(data + 0x230) = 1;
        } else {
            *(float *)(data + 0x20) = 3.0f;
        }
        if (*(float *)(data + 0x224) == 0.0f)
            *(float *)(data + 0x224) = 15.0f;
        FUN_L01_0026d930(data + 0x170);
        *(float *)(data + 0x17c) = 2.0f;
        *(float *)(data + 0x178) = 2.0f;
        result = FUN_001fa6d0(*(float *)(data + 0x250) * 0.8f * 1024.0f);
        angle = *(float *)(data + 0x23c) * frame_time_gp;
        *(int *)(data + 0x170) = result;
        *(float *)(data + 0x194) = angle;
        if (*(int *)(data + 0x230) != 0) {
            moby[0x20] = 1;
            *(u16 *)(moby + 0x34) |= 0x1000;
        } else {
            if (*(int *)(data + 0x25c) == -1) moby[0x20] = 12;
            else moby[0x20] = 13;
            *(int *)(moby + 0x94) = 0;
            *(int *)(moby + 0x58) = 0;
            moby[0x31] = 0;
            distance = *(float *)(moby + 0x18) - 20.0f;
            *(u16 *)(moby + 0x34) = (*(u16 *)(moby + 0x34) & 0xefff) | 1;
            *(float *)(moby + 0x18) = distance;
        }
        break;
    case LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH:
        if (*(int *)(data + 0x204) != 2) {
            if (FUN_00213260(4) == 0) {
                path_hit = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                            *(float *)(data + 0x250) * 0.8f,
                                            moby + 0x10, target + 0x10, move);
                if (path_hit != 0) {
                    moby[0x20] = 4;
                    if (moby[0x53] != 1)
                        blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                    clear_u64_value(data + 0x40);
                    *(float *)(data + 0x234) = random_float_between(-0.34906584f, 0.34906584f);
                } else {
                    *(int *)(data + 0x204) = 2;
                }
            }
        }
        if (*(int *)(data + 0x22c) != 0 && *(int *)(data + 0x204) == 2) {
            moby[0x20] = 15;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, 5);
            return;
        }
        *(float *)(data + 0x194) = *(float *)(data + 0x23c) * 0.5f * frame_time_gp;
        if (moby[0x53] != 1)
            blend_moby_animation(moby, 1, 0, 5);
        moby[0x20] = 2;
        break;
    case LARGE_AMOEBOID_MOVE_TOWARD_STORED_POSITION:
        angle = fast_arc_tan(*(float *)(data + 0x210) - *(float *)(moby + 0x10),
                              *(float *)(data + 0x214) - *(float *)(moby + 0x14));
        approach_angle((float *)(moby + 0x48), (float *)(data + 0x220), angle,
                          D_0015ED70 * 1.0471976f, D_0015ED70 * 1.0471976f,
                          frame_time_gp * 3.1415927f);
        move[0] = fast_cos(*(float *)(moby + 0x48));
        move[1] = fast_sin(*(float *)(moby + 0x48));
        move[2] = 0.0f;
        FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
        result = *(int *)(data + 0x204);
        if (result != 2 && FUN_00213260(4) == 0) {
            path_hit = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                        *(float *)(data + 0x250) * 0.8f,
                                        moby + 0x10, target + 0x10, hit);
            if (path_hit != 0) {
                moby[0x20] = 3;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                clear_u64_value(data + 0x40);
                angle = random_float_between(-0.34906584f, 0.34906584f);
                distance = *(float *)(data + 0x23c) * frame_time_gp;
                *(float *)(data + 0x234) = angle;
                *(float *)(data + 0x194) = distance;
            } else {
                *(int *)(data + 0x204) = 2;
            }
        }
        if (*(int *)(data + 0x22c) != 0 && *(int *)(data + 0x204) == 2) {
            moby[0x20] = 15;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, 5);
        }
        spawn_amoeboid_movement_particles(moby);
        return;
    case LARGE_AMOEBOID_MOVE_DIRECTLY_TOWARD_TARGET:
        blocked = 0;
        angle = fast_arc_tan(*(float *)(target + 0x10) - *(float *)(moby + 0x10),
                              *(float *)(target + 0x14) - *(float *)(moby + 0x14));
        approach_angle((float *)(moby + 0x48), (float *)(data + 0x220), angle,
                          D_0015ED70 * 1.0471976f, D_0015ED70 * 1.0471976f,
                          frame_time_gp * 3.1415927f);
        move[0] = fast_cos(*(float *)(moby + 0x48));
        move[1] = fast_sin(*(float *)(moby + 0x48));
        move[2] = 0.0f;
        FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
        if (FUN_L00_00261b48(*(int *)(data + 0x240), moby + 0x10, target + 0x10, hit) != 0 &&
            FUN_001f9b48(moby + 0x10, hit) < 1.5f) blocked = 1;
        if (*(int *)(data + 0x22c) == 0) {
            moby[0x20] = 5;
        } else if (blocked) {
            moby[0x20] = 16;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        } else if (vector_distance(moby + 0x10, target + 0x10) < 2.0f) {
            moby[0x20] = 6;
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 5, scale_game_frames(10));
            return;
        }
        break;
    case LARGE_AMOEBOID_WAIT_WHILE_BLOCKED:
        if (!(moby[0x70] & 2) || *(int *)(data + 0x22c) != 0) return;
        moby[0x20] = 5;
        if (moby[0x53] != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(20));
        return;
    case LARGE_AMOEBOID_FOLLOW_PATH_TO_TARGET: {
        char *target_position;
        unsigned char path_hit;
        target_position = target + 0x10;
        distance = vector_distance(moby + 0x10, target_position);
        if (distance < D_L01_001619AC) {
            moby[0x20] = 6;
            *(float *)(data + 0x194) = (*(float *)(data + 0x23c) + *(float *)(data + 0x23c)) * frame_time_gp;
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            return;
        } else {
            if (follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                        *(float *)(data + 0x250) * 0.8f, moby + 0x10, target_position, hit) != 0)
                path_hit = 1;
            else
                path_hit = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                            0.0f, moby + 0x10, target_position, hit) != 0;
            if (path_hit != 0) {
                angle = fast_arc_tan(hit[0] - *(float *)(moby + 0x10), hit[1] - *(float *)(moby + 0x14));
                angle = fast_add_rotations(angle, *(float *)(data + 0x234));
                approach_angle((float *)(moby + 0x48), (float *)(data + 0x220), angle,
                                  D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f,
                                  frame_time_gp * 25.132742f);
                move[0] = fast_cos(*(float *)(moby + 0x48));
                move[0] += move[0];
                move[1] = fast_sin(*(float *)(moby + 0x48));
                move[1] += move[1];
                move[2] = 0.0f;
                FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
            } else {
                *(int *)(data + 0x204) = 2;
            }
            if (*(int *)(data + 0x204) == 2) moby[0x20] = 5;
            spawn_amoeboid_movement_particles(moby);
            return;
        }
    }
    case LARGE_AMOEBOID_LUNGE_AT_TARGET: {
        char *target_position;
        float *rotation = (float *)(moby + 0x48);
        angle = fast_arc_tan(*(float *)(target + 0x10) - *(float *)(moby + 0x10),
                              *(float *)(target + 0x14) - *(float *)(moby + 0x14));
        approach_angle(rotation, (float *)(data + 0x220), angle,
                          D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f,
                          frame_time_gp * 25.132742f);
        distance = compute_interpolated_record_value(moby);
        if (moby[0x52] == moby[0x53] && 10.0f <= distance && distance <= 16.0f) {
            move[0] = fast_cos(*rotation);
            move[0] += move[0];
            move[1] = fast_sin(*rotation);
            move[1] += move[1];
            move[2] = 0.0f;
            FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
        }
        if (FUN_00214cc8(15.5f, moby) != 0 && vector_distance(moby + 0x10, (target_position = target + 0x10)) < 2.0f &&
            FUN_001f99c0(*(float *)(target + 0x18) - *(float *)(moby + 0x18)) <
                *(float *)(data + 0x250) + *(float *)(data + 0x250)) {
            move[0] = fast_cos(*(float *)(moby + 0x48));
            move[1] = fast_sin(*(float *)(moby + 0x48));
            move[2] = 0.0f;
            move[3] = 5627.9248f;
            qcopy(hit, target_position);
            hit[2] += 0.75f;
            spawn_nearby_effect(target, moby, 0x10001, hit, move, 1.0f);
            return;
        }
        if (moby[0x70] & 2) {
            moby[0x20] = 7;
            if (moby[0x53] != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            *(float *)(data + 0x194) = *(float *)(data + 0x23c) * frame_time_gp;
        }
        break;
    }
    case LARGE_AMOEBOID_WAIT_FOR_PATH_ACQUISITION_ANIMATION_END:
    case LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESUMING_PATH:
        if (!(moby[0x70] & 2)) return;
        moby[0x20] = 4;
        if (moby[0x53] != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        return;
    case LARGE_AMOEBOID_RETURN_TO_STORED_POSITION: {
        int blocked = 0;
        char *target_position;
        distance = vector_distance(moby + 0x10, (target_position = data + 0x210));
        if (distance < 1.0f) {
            moby[0x20] = 1;
        } else {
            if (follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                        *(float *)(data + 0x250) * 0.8f, moby + 0x10, target_position, hit) != 0)
                blocked = 1;
            else
                blocked = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                            0.0f, target_position, target_position, hit) != 0;
        }
        if (blocked != 0) {
            angle = fast_arc_tan(hit[0] - *(float *)(moby + 0x10), hit[1] - *(float *)(moby + 0x14));
            angle = fast_add_rotations(angle, *(float *)(data + 0x234));
            approach_angle((float *)(moby + 0x48), (float *)(data + 0x220), angle,
                              D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f,
                              frame_time_gp * 25.132742f);
            move[0] = fast_cos(*(float *)(moby + 0x48));
            move[0] += move[0];
            move[1] = fast_sin(*(float *)(moby + 0x48));
            move[1] += move[1];
            move[2] = 0.0f;
            FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
        }
        if (*(int *)(data + 0x204) != 2 && (moby[0x20] = 4, moby[0x53] != 1))
            blend_moby_animation(moby, 1, 0, scale_game_frames(3));
        spawn_amoeboid_movement_particles(moby);
        return;
    }
    case LARGE_AMOEBOID_PROCESS_KNOCKBACK:
        result = mine_fall(moby, data + 0x60);
        if (result & 1) {
            path_hit = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                        *(float *)(data + 0x250) * 0.8f,
                                        moby + 0x10, data + 0x210, move);
            if (path_hit != 0) {
                FUN_0022da68(4, 0, (char *)moby);
                *(float *)(data + 0x194) = *(float *)(data + 0x23c) * frame_time_gp;
                moby[0x20] = 9;
                if (moby[0x53] != 5)
                    blend_moby_animation(moby, 5, 4, scale_game_frames(10));
                qcopy(data + 0x210, moby + 0x10);
                return;
            }
            spawn_amoeboid_burst_sparks(moby, D_L01_0015F580);
            FUN_L00_00257470(moby, 0, -1);
        } else if (!(result & 0x120)) {
            return;
        }
        FUN_0020c828(moby);
        return;
    case LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESTARTING_PATH:
        if (moby[0x70] & 2) {
            moby[0x20] = 4;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            clear_u64_value(data + 0x40);
            angle = random_float_between(-0.34906584f, 0.34906584f);
            distance = *(float *)(data + 0x23c) * frame_time_gp;
            *(float *)(data + 0x234) = angle;
            *(float *)(data + 0x194) = distance;
        }
        break;
    case LARGE_AMOEBOID_WAIT_FOR_ACTIVATION_VOLUME:
        *(u16 *)(moby + 0x34) |= 1;
        if (FUN_00214720(D_0013F420, *(int *)(data + 0x25c)) != 0) {
            u16 flags;
            float distance;
            int result;
            distance = *(float *)(moby + 0x18) + 20.0f;
            flags = *(u16 *)(moby + 0x34) | 0x1000;
            moby[0x20] = 1;
            *(u16 *)(moby + 0x34) = flags;
            *(float *)(moby + 0x58) = 1.0f;
            *(float *)(moby + 0x18) = distance;
            result = *(int *)(*(char **)(moby + 0x24) + 0x10);
            *(int *)(moby + 0x94) = result;
            *(u16 *)(moby + 0x34) = flags & 0xfffe;
            moby[0x31] = 1;
        }
        return;
    case LARGE_AMOEBOID_PROCESS_SUCK_CANNON_INTERACTION:
        if (FUN_L00_002dc9f0(moby, data + 0x60)) {
            moby[0x20] = 1;
            *(u16 *)(data + 0x128) = 0;
        }
        break;
    }
}
/* Moby update: hit test against a target, hurt/debris on a hit, else path search and bob. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002F0040), where it is exact; names translated to the US level program. */

typedef int u128_2F0040 __attribute__((mode(TI)));

typedef union { u128_2F0040 q; float f[4]; } V_2F0040;

extern char *FUN_L00_0025a420(void *, int, int);
extern char *activate_smaller_amoeboid(void *) __asm__("FUN_L01_002ef350");
extern char *find_nearest_class_26f_moby_in_list(char *, int) __asm__("FUN_L01_002ef478");
extern char D_0013E533[];
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");

extern float fast_add_rotations_c(float, float) __asm__("func_001FA580");
extern float fast_vec_dist(void *, void *);
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern char *D_L01_0015FFD8;
extern int D_L01_001612D0;
extern float D_L01_001619A0;
extern float D_L01_001619A4;
extern int FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
extern int fast_tween_color(float, int, int);
extern int fun_00213928(void *, void *, void *, int, int *, float *, int, int);
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern short D_L01_00161998 __attribute__((sda));
extern short D_L01_0016199C __attribute__((sda));
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("func_0022DA68");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
float absolute_float(float input) __asm__("func_001F99C0");
extern float D_0013F420[];
extern int D_001413D0[];
extern char *D_L01_0015FFD8;
extern const float D_L01_001619A0_gp __asm__("D_L01_001619A0") __attribute__((sda));
extern const float D_L01_001619A4_gp __asm__("D_L01_001619A4") __attribute__((sda));
extern const float frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;

void process_amoeboid_hits_and_update_targeting(char *moby) {
    V_2F0040 scratch_vector;
    V_2F0040 line_test_endpoint;
    int flag;
    float amount;
    float first_spawn_angle;
    float second_spawn_angle;
    char *moby_vars;
    char *hit_record;
    char *hit_record_moby;
    char *x;
    char *search_path;
    char *spawned;
    char *q;
    char *cand;
    u128_2F0040 *wp;
    int candidate_list_index;
    char *hit_record_moby_for_class_check;
    char *x2;
    char *path_search_position;
    int removal_height_entry_index;
    int path_search_gate_moby_index;
    float size_multiplier;
    float rate;
    float fixed_scale;
    float hit_decrement;
    float candidate_distance;
    float color_cycle_phase;
    float k;
    float seven;

    amount = 0.0f;
    moby_vars = *(char **)(moby + 0x78);
    hit_record = FUN_L00_0025a420(moby, 0x330000, 0);
    if (hit_record != 0) {
        qcopy(&scratch_vector, moby + 0x10);
        scratch_vector.f[2] = scratch_vector.f[2] + 0.5f;
        hit_record_moby = *(char **)(hit_record + 0x20);
        if (hit_record_moby != 0) {
            short cls = *(short *)(hit_record_moby + 0xA6);
            if (cls == 0x47) {
                qcopy(&line_test_endpoint, ((char *)D_0013F420));
                wp = &line_test_endpoint.q;
            } else {
                x = *(char **)(hit_record_moby + 0x24);
                if ((x != 0 && *(short *)(x + 0x46) == 5) || cls == 0xBA) {
                    qcopy(&line_test_endpoint, &scratch_vector);
                    wp = &line_test_endpoint.q;
                } else {
                    qcopy(&line_test_endpoint, hit_record_moby + 0x10);
                    wp = &line_test_endpoint.q;
                }
            }
        } else {
            qcopy(&line_test_endpoint, hit_record);
            wp = &line_test_endpoint.q;
        }
        if (fast_vec_dist(&scratch_vector, wp) < 24.0f) {
            if (func_L00_001EFFF0_t(&scratch_vector, wp, 2, moby, 0) != 0) {
                hit_record = 0;
            }
        }
    }
    fun_00213928(moby, hit_record, moby_vars + 0x20, 0, &flag, &amount, 0, 4);
    if (flag != 1 && amount != 0.0f && *(unsigned char *)(moby + 0x20) != 0xB && *(unsigned char *)(moby + 0x20) != 0xA && *(unsigned char *)(moby + 0x20) != 8) {
        size_multiplier = 0.8f;
        allocate_voice_for_target_entry(0, 0, moby);
        hit_decrement = 1.0f;
        fixed_scale = 1024.0f;
        rate = 0.008f;
        *(float *)(moby_vars + 0x20) = *(float *)(moby_vars + 0x20) - hit_decrement;
        moby_vars[0x9D] = 0;
        *(float *)(moby_vars + 0x70) = rate;
        *(int *)(moby_vars + 0x84) = 8;
        *(int *)(moby_vars + 0x80) = truncate_float_to_s32_c(*(float *)(moby_vars + 0x250) * size_multiplier * fixed_scale);
        *(float *)(moby_vars + 0x88) = *(float *)(moby_vars + 0x250) * size_multiplier;
        spawn_amoeboid_burst_sparks((unsigned char *)moby, (float *)(hit_record + 0x10));
        if (0.0f < *(float *)(moby_vars + 0x20)) {
            spawned = activate_smaller_amoeboid(moby);
            if (spawned != 0) {
                q = *(char **)(spawned + 0x78);
                q[0x9D] = 3;
                *(float *)(q + 0x70) = rate;
                *(int *)(q + 0x84) = 8;
                *(int *)(q + 0x80) = truncate_float_to_s32_c(*(float *)(moby_vars + 0x250) * size_multiplier * fixed_scale);
                k = frame_time_macro;
                seven = 7.0f;
                *(float *)(q + 0x88) = *(float *)(moby_vars + 0x250) * size_multiplier;
                *(float *)(q + 0x78) = D_L01_001619A0_gp * k;
                *(float *)(q + 0x7C) = D_L01_001619A4_gp * k;
                *(int *)(q + 0x84) = *(int *)(q + 0x84) | 4;
                *(float *)(q + 0xB0) = seven;
                *(float *)(q + 0xB4) = 13.0f;
                scratch_vector.q = *(u128_2F0040 *)(hit_record + 0x10);
                trooper_knockback_arc(&scratch_vector, &first_spawn_angle, q + 0x78, q + 0x7C);
                first_spawn_angle = fast_subtract_rotations(first_spawn_angle, 0.7853982f);
                trooper_start_knockback(spawned, q + 0x60, first_spawn_angle, 4, 8, 0);
                spawned[0x20] = 8;
                *(float *)(spawned + 0x58) = hit_decrement;
            }
            spawned = activate_smaller_amoeboid(moby);
            if (spawned != 0) {
                q = *(char **)(spawned + 0x78);
                *(float *)(q + 0x70) = rate;
                q[0x9D] = 3;
                *(int *)(q + 0x84) = 8;
                *(int *)(q + 0x80) = truncate_float_to_s32_c(*(float *)(moby_vars + 0x250) * size_multiplier * fixed_scale);
                k = frame_time_macro;
                seven = 7.0f;
                *(float *)(q + 0x88) = *(float *)(moby_vars + 0x250) * size_multiplier;
                *(float *)(q + 0x78) = D_L01_001619A0_gp * k;
                *(float *)(q + 0x7C) = D_L01_001619A4_gp * k;
                *(int *)(q + 0x84) = *(int *)(q + 0x84) | 4;
                *(float *)(q + 0xB0) = seven;
                *(float *)(q + 0xB4) = 13.0f;
                scratch_vector.q = *(u128_2F0040 *)(hit_record + 0x10);
                trooper_knockback_arc(&scratch_vector, &second_spawn_angle, q + 0x78, q + 0x7C);
                second_spawn_angle = fast_add_rotations_c(second_spawn_angle, 0.7853982f);
                trooper_start_knockback(spawned, q + 0x60, second_spawn_angle, 4, 8, 0);
                spawned[0x20] = 8;
                *(float *)(spawned + 0x58) = hit_decrement;
            }
            hit_record_moby_for_class_check = *(char **)(hit_record + 0x20);
            if (hit_record_moby_for_class_check != 0 && (x2 = *(char **)(hit_record_moby_for_class_check + 0x24)) != 0 && *(short *)(x2 + 0x46) == 5) {
                FUN_L00_00257470(moby, 0x800, -1);
            } else {
                FUN_L00_00257470(moby, 0, -1);
            }
            mark_moby_for_removal_c(moby);
            return;
        }
        *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) & 0xEFFF;
        mark_moby_for_removal_c(moby);
        return;
    }
    *(unsigned char *)(moby + 0xA4) = 0xFF;
    removal_height_entry_index = *(int *)(moby_vars + 0x258);
    if (removal_height_entry_index != 0 && *(unsigned char *)(moby + 0x20) != 0
        && *(float *)(moby + 0x18) < *(float *)(removal_height_entry_index * 0x1190 + D_L01_001612D0 + 8)) {
        clear_u64_value_c(&scratch_vector);
        spawn_amoeboid_burst_sparks((unsigned char *)moby, (float *)&scratch_vector);
        mark_moby_for_removal_c(moby);
        return;
    }
    if (*(int *)(moby_vars + 0x38) != 0) {
        *(int *)(moby_vars + 0x22C) = scale_game_frames(0xF0);
        *(int *)(moby_vars + 0x38) = 0;
    }
    if (tick_countdown_32_alt(moby_vars + 0x22C) != 0) {
        *(float *)(moby_vars + 0x228) = *(float *)(moby_vars + 0x224);
    } else {
        *(float *)(moby_vars + 0x228) = *(float *)(moby_vars + 0x224) + 20.0f;
    }
    path_search_gate_moby_index = *(int *)(moby_vars + 0x260);
    if (path_search_gate_moby_index <= 0 || *(unsigned char *)((path_search_gate_moby_index << 8) + (*(int *)&D_L01_0015FFD8) + 0xBC) != 0) {
        search_path = D_L01_001B0930[*(int *)(moby_vars + 0x24C)];
        path_search_position = moby_vars + 0x1C0;
        if (FUN_L00_0025ff38(*(float *)(moby_vars + 0x228), moby, path_search_position, 0, 0, search_path + 0x10, *(int *)search_path) != 2) {
            if (*(float *)(moby_vars + 0x228) < vector_distance_xy_alt(moby_vars + 0x210, path_search_position)
                || 3.0f < absolute_float(*(float *)(moby + 0x18) - *(float *)(moby_vars + 0x1C8))) {
                *(int *)(moby_vars + 0x204) = 2;
            }
        }
    }
    candidate_list_index = *(int *)(moby_vars + 0x248);
    if (candidate_list_index >= 0) {
        cand = find_nearest_class_26f_moby_in_list(moby, candidate_list_index);
        if (cand != 0) {
            candidate_distance = vector_distance_xy_alt(moby + 0x10, cand + 0x10);
            if (candidate_distance < *(float *)(moby_vars + 0x228)) {
                if (*(int *)(moby_vars + 0x204) == 2
                    || candidate_distance < vector_distance_xy_alt(moby + 0x10, *(char **)(moby_vars + 0x200) + 0x10)) {
                    *(char **)(moby_vars + 0x200) = cand;
                    *(int *)(moby_vars + 0x204) = 1;
                }
            }
        }
    }
    if (*(int *)(moby_vars + 0x200) == 0) {
        *(int *)(moby_vars + 0x200) = *(int *)(((char *)((char (*))D_001413D0)));
    }
    color_cycle_phase = fast_add_rotations_c(*(float *)(moby_vars + 0x254), frame_time * 9.424778f);
    *(float *)(moby_vars + 0x254) = color_cycle_phase;
    *(int *)(moby + 0x90) = fast_tween_color((fast_sin_c(color_cycle_phase) + 1.0f) * 0.5f,
                                           *(int *)&D_L01_00161998, *(int *)&D_L01_0016199C);
}
extern short *D_L01_001ABCC0[];
extern void FUN_L00_00250df8(void *);

char *activate_smaller_amoeboid(void *source_moby) __asm__("FUN_L01_002ef350");

char *activate_smaller_amoeboid(void *source_moby) {
    struct Moby *self = source_moby;
    short *list;
    char *entities;
    int entry;
    int next_class = -1;
    if (self->oclass == 0x23C) {
        next_class = 0x361;
    } else if (self->oclass == 0x361) {
        next_class = 0x362;
    }
    entry = *(volatile u8 *)&self->group;
    entities = (char *)(D_L01_001ABCC0 + entry);
    /* Fakematch: the dead touch of the entity base gives retail's register for the list slot. */
    D_L01_0015FFD8++;
    D_L01_0015FFD8--;
    list = *(short **)entities;
    entities = D_L01_0015FFD8;
    if (list == 0) return 0;
    goto scan;
    do {
        struct Moby *other;
        int index;
advance:
        if ((short)entry < 0) return 0;
        list++;
scan:
        index = *(unsigned short *)list;
        entry = index;
        index &= 0x7FFF;
        other = (struct Moby *)(entities + (index << 8));
        if (other->state != 12 || other->oclass != next_class) goto advance;
        {
            /* Retail reads both pvars pointers and update_dist before writing the entity. */
            u8 *other_vars = *(u8 * volatile *)&other->pvars;
            u8 update_dist = *(volatile u8 *)&self->update_dist;
            u8 *self_vars = self->pvars;
            other->update_dist = update_dist;
            other->draw_dist = self->draw_dist;
            {
                u64 spawn_frame = self->spawn_frame;
                other->visible = 1;
                other->spawn_frame = spawn_frame;
            }
            other->collision = other->pclass->collision;
            other->flags = self->flags;
            qcopy(&other->pos, &self->pos);
            qcopy(&other->rot, &self->rot);
            *(float *)(other_vars + 0x20) = *(float *)(self_vars + 0x20);
            *(float *)(other_vars + 0x224) = *(float *)(self_vars + 0x224);
            FUN_L00_00250df8(other);
            return (char *)other;
        }
    } while (1);
    return 0;
}

typedef struct {
    char p0[0x10];
    char pos[0x10];
    unsigned char state;
    char p21[0x13];
    unsigned short flags;
    char p36[0x70];
    short type;
    char pA8[0x58];
} Ent_2ef478;

extern float dist_2ef478(void *, void *) __asm__("FUN_001f9b48");
extern char *D_L01_0015FFD8;
extern char *D_L01_0015FFD8;

/* Nearest live entity of type 0x26F in list idx within 120 units of self, or 0. */
char *find_nearest_class_26f_moby_in_list(char *self, int candidate_list_index) {
    short *list = D_L01_001ABCC0[candidate_list_index];
    float best = 120.0f;
    char *nearest_entity = 0;
    if (list == 0) {
        return 0;
    }
    while (1) {
        int i = *(unsigned short *)list & 0x7FFF;
        if ((*(Ent_2ef478 * *)&D_L01_0015FFD8)[i].flags & 0x1000) {
            if ((*(Ent_2ef478 * *)&D_L01_0015FFD8)[i].type == 0x26F) {
                if ((*(Ent_2ef478 * *)&D_L01_0015FFD8)[i].state < 0x7F) {
                    float candidate_distance = dist_2ef478((*(Ent_2ef478 * *)&D_L01_0015FFD8)[i].pos, self + 0x10);
                    if (candidate_distance < best) {
                        best = candidate_distance;
                        nearest_entity = D_L01_0015FFD8 + (i << 8);
                    }
                }
            }
        }
        if (*list++ < 0) {
            return nearest_entity;
        }
    }
}
#include "rnc/gameplay/entities/moby.h"
#define MOBY(p) ((struct Moby *)(p))
extern int amoeboid_first_movement_particle_spawn_gate_modulus_plus_one_gp __asm__("D_L01_001619E0") __attribute__((sda));
extern int amoeboid_second_movement_particle_spawn_gate_modulus_plus_one_gp __asm__("D_L01_001619E4") __attribute__((sda));
extern float amoeboid_movement_particle_max_spawn_radius_gp __asm__("D_L01_001619E8") __attribute__((sda));
extern float amoeboid_first_movement_particle_base_scale_max_gp __asm__("D_L01_001619B8") __attribute__((sda));
extern float D_L01_001619BC __attribute__((sda));
extern int amoeboid_first_movement_particle_lifetime_min_gp __asm__("D_L01_001619C0") __attribute__((sda));
extern int amoeboid_first_movement_particle_lifetime_max_gp __asm__("D_L01_001619C4") __attribute__((sda));
extern int D_L01_001619B0 __attribute__((sda));
extern int D_L01_001619B4 __attribute__((sda));
extern int D_L01_001619C8 __attribute__((sda));
extern int D_L01_001619CC __attribute__((sda));
extern float amoeboid_second_movement_particle_base_scale_max_gp __asm__("D_L01_001619D0") __attribute__((sda));
extern float D_L01_001619D4 __attribute__((sda));
extern int amoeboid_second_movement_particle_lifetime_min_gp __asm__("D_L01_001619D8") __attribute__((sda));
extern int amoeboid_second_movement_particle_lifetime_max_gp __asm__("D_L01_001619DC") __attribute__((sda));
extern int random_integer_below(int) __asm__("FUN_00213260");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void spawn_type_52_particle(float *, float, float, int, int, int) __asm__("FUN_L01_00287158");

void spawn_amoeboid_movement_particles(unsigned char *moby) {
    char *data = (char *)MOBY(moby)->pvars;
    float pos[3];
    float angle, radius, lower, upper;
    int time;
    if (MOBY(moby)->visible == 0)
        return;
    if (random_integer_below(amoeboid_first_movement_particle_spawn_gate_modulus_plus_one_gp - 1) == 0) {
        angle = random_angle_radians();
        radius = random_float_between_alt(0.0f, amoeboid_movement_particle_max_spawn_radius_gp);
        lower = random_float_between_alt(0.0f, amoeboid_first_movement_particle_base_scale_max_gp) * *(float *)(data + 0x250);
        upper = random_float_between_alt(amoeboid_first_movement_particle_base_scale_max_gp, D_L01_001619BC) * *(float *)(data + 0x250);
        time = truncate_float_to_s32(random_float_between_alt((float)amoeboid_first_movement_particle_lifetime_min_gp, (float)amoeboid_first_movement_particle_lifetime_max_gp));
        pos[0] = fast_cos(angle) * radius;
        pos[1] = fast_sin(angle) * radius;
        pos[2] = 0.0f;
        add_vector_xyz(pos, pos, &MOBY(moby)->pos);
        spawn_type_52_particle(pos, lower, upper, D_L01_001619B0, D_L01_001619B4,
                         scale_game_frames(time));
    }
    if (random_integer_below(amoeboid_second_movement_particle_spawn_gate_modulus_plus_one_gp - 1) == 0) {
        angle = random_angle_radians();
        radius = random_float_between_alt(0.0f, amoeboid_movement_particle_max_spawn_radius_gp);
        lower = random_float_between_alt(0.0f, amoeboid_second_movement_particle_base_scale_max_gp) * *(float *)(data + 0x250);
        upper = random_float_between_alt(amoeboid_second_movement_particle_base_scale_max_gp, D_L01_001619D4) * *(float *)(data + 0x250);
        time = truncate_float_to_s32(random_float_between_alt((float)amoeboid_second_movement_particle_lifetime_min_gp, (float)amoeboid_second_movement_particle_lifetime_max_gp));
        pos[0] = fast_cos(angle) * radius;
        pos[1] = fast_sin(angle) * radius;
        pos[2] = 0.0f;
        add_vector_xyz(pos, pos, &MOBY(moby)->pos);
        spawn_type_52_particle(pos, lower, upper, D_L01_001619C8, D_L01_001619CC,
                         scale_game_frames(time));
    }
}
extern s32 amoeboid_burst_group_count_gp __asm__("D_L01_001619EC") __attribute__((sda));
extern s32 amoeboid_burst_sparks_per_group_gp __asm__("D_L01_001619F0") __attribute__((sda));
extern f32 amoeboid_burst_group_position_jitter_bound_gp __asm__("D_L01_001619F4") __attribute__((sda));
extern f32 amoeboid_burst_spark_position_jitter_bound_gp __asm__("D_L01_001619F8") __attribute__((sda));
extern f32 amoeboid_burst_spark_velocity_jitter_bound_gp __asm__("D_L01_001619FC") __attribute__((sda));
extern f32 amoeboid_burst_radial_speed_min_gp __asm__("D_L01_00161A00") __attribute__((sda));
extern f32 amoeboid_burst_radial_speed_max_gp __asm__("D_L01_00161A04") __attribute__((sda));
extern f32 amoeboid_burst_vertical_speed_min_gp __asm__("D_L01_00161A08") __attribute__((sda));
extern f32 amoeboid_burst_vertical_speed_max_gp __asm__("D_L01_00161A0C") __attribute__((sda));
extern f32 amoeboid_burst_downward_velocity_adjustment_factor_gp __asm__("D_L01_00161A10") __attribute__((sda));
extern f32 D_L01_00161A14 __attribute__((sda));
extern f32 D_L01_00161A18 __attribute__((sda));
extern s32 D_L01_00161A1C __attribute__((sda));
extern s32 D_L01_00161A20 __attribute__((sda));
extern s32 D_L01_00161A24 __attribute__((sda));
extern s32 D_L01_00161A28 __attribute__((sda));
extern s32 D_L01_00161A2C __attribute__((sda));
extern s32 D_L01_00161A30 __attribute__((sda));
extern s32 amoeboid_burst_spark_duration_base_gp __asm__("D_L01_00161A34") __attribute__((sda));
extern f32 amoeboid_burst_input_motion_scale_max_gp __asm__("D_L01_00161A38") __attribute__((sda));

extern f32 D_0015ED70;
extern void add_random_offset_to_vector_xyz(void *, f32) __asm__("FUN_L00_0025f8e0");
extern int fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern f32 FUN_00220e20(f32) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

void spawn_amoeboid_burst_sparks(unsigned char *moby, f32 *mode) {
    char *data = (char *)MOBY(moby)->pvars;
    f32 spawn_position[4], origin[4], velocity[4];
    u128 *dest;
    f32 *origin_copy;
    u128 *source;
    f32 angle, radius, vertical, lateral, scale, scaled_radius, scaled_vertical;
    s32 i, next, j, lower, upper, first, second, duration;

    i = 0;
    if (amoeboid_burst_group_count_gp > 0) {
      source = (u128 *)(moby + 0x10);
      dest = (u128 *)spawn_position;
      origin_copy = origin;
      do {
        qcopy_nc(dest, source);
        add_random_offset_to_vector_xyz(dest, amoeboid_burst_group_position_jitter_bound_gp * *(f32 *)(data + 0x250));
        spawn_position[2] += 1.0f;
        angle = random_angle_radians();
        radius = random_float_between_alt(amoeboid_burst_radial_speed_min_gp, amoeboid_burst_radial_speed_max_gp);
        lateral = random_float_between_alt(amoeboid_burst_vertical_speed_min_gp, amoeboid_burst_vertical_speed_max_gp);
        vertical = random_float_between_alt(0.0f, amoeboid_burst_input_motion_scale_max_gp);
        scale_vector_xyz(origin_copy, mode, vertical * frame_time_gp);
        scale = fast_cos(angle);
        origin[0] += scale * (radius * frame_time_gp);
        scale = fast_sin(angle);
        scaled_radius = radius * frame_time_gp;
        origin[1] += scale * scaled_radius;
        scaled_vertical = lateral * frame_time_gp;
        origin[2] += scaled_vertical;
        qcopy(velocity, origin_copy);
        next = i + 1;
        velocity[2] -= amoeboid_burst_downward_velocity_adjustment_factor_gp * D_0015ED70 * (f32)scale_game_frames(D_L01_00161A30);
        for (j = 0; j < amoeboid_burst_sparks_per_group_gp; j++) {
            scale = random_float_between_alt(0.5f, 1.5f);
            origin[3] = scale * D_L01_00161A14;
            velocity[3] = scale * D_L01_00161A18;
            add_random_offset_to_vector_xyz(spawn_position, amoeboid_burst_spark_position_jitter_bound_gp);
            add_random_offset_to_vector_xyz(velocity, amoeboid_burst_spark_velocity_jitter_bound_gp * frame_time_gp);
            lower = fast_tween_color(random_float_between_alt(0.0f, 1.0f), D_L01_00161A1C, D_L01_00161A24);
            upper = fast_tween_color(random_float_between_alt(0.0f, 1.0f), D_L01_00161A20, D_L01_00161A28);
            first = scale_game_frames(D_L01_00161A2C);
            second = scale_game_frames(D_L01_00161A30);
            duration = truncate_float_to_s32(FUN_00220e20(random_float_between_alt((f32)amoeboid_burst_spark_duration_base_gp * 0.5f, (f32)amoeboid_burst_spark_duration_base_gp * 2.5f)));
            spawn_spark(spawn_position, origin_copy, velocity, lower, upper, first, second, duration, -1);
        }
        i = next;
      } while (i < amoeboid_burst_group_count_gp);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002F0E60), where it is exact; names translated to the US level program. */

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

int process_amoeboid_suck_cannon_suction(unsigned char *moby) __asm__("FUN_L01_002efa88");

int process_amoeboid_suck_cannon_suction(unsigned char *moby) {
    int value = process_moby_suck_cannon_suction(moby);
    if (value != 0) {
        if (*(short *)(moby + 0xA6) != 0x362) {
            return 0;
        } else if (moby[0x20] == 8) {
            return 0;
        } else {
            moby[0x20] = 0xE;
        }
    } else if (moby[0x20] == 0xE) {
        moby[0x20] = 1;
    }
    return value;
}
extern s32 level_frame_count __asm__("D_L01_0015F5CC");

/* Search backward for the first entry below the loaded threshold. */
s32 reserve_expired_frame_slot(s32 *values, s32 count, s32 offset) __asm__("FUN_L01_002efbf8");

s32 reserve_expired_frame_slot(s32 *values, s32 count, s32 offset) {
    s32 index = (s32)((u32)count - 1);
    if (index >= 0) {
        for (; index >= 0; index--) {
            if (values[index] < level_frame_count) {
                values[index] = (s32)((u32)level_frame_count + (u32)offset);
                break;
            }
        }
    }
    return index;
}

#define NOT_SDA

#define MACRO_ADDR

/* runs the moby update and sets its state byte */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002F30F0), where it is exact; names translated to the US level program. */

extern char *get_moby_suck_cannon_record(char *) __asm__("FUN_L00_002db890");
extern int launch_suck_cannon_projectile(char *, int, int, float) __asm__("FUN_L00_002dbc30");
extern void FUN_0022da68(int, int, char *);

int launch_suck_cannon_projectile_and_sync_moby_state(char *moby, int a, int position_source_moby, float position_z_offset) __asm__("FUN_L01_002f1d18");

int launch_suck_cannon_projectile_and_sync_moby_state(char *moby, int a, int position_source_moby, float position_z_offset) {
    char *callback_record = get_moby_suck_cannon_record(moby);
    int projectile_launch_result = launch_suck_cannon_projectile(moby, a, position_source_moby, position_z_offset);
    if (projectile_launch_result != 0) {
        moby[0x20] = 7;
    } else if ((unsigned char)moby[0x20] == 7) {
        moby[0x20] = 1;
    }
    if (callback_record != 0) {
        if (*(short *)(callback_record + 0x68) == 6) {
            FUN_0022da68(2, 0, moby);
        }
    }
    return projectile_launch_result;
}
extern float FUN_L00_00258110_w7(float *vel, float cur, float target, float k, float d,
                                 float max) __asm__("FUN_L00_00258110");
extern int FUN_001f96f8_w7(int) __asm__("FUN_001f96f8");
extern void blend_moby_animation_w7(void *m, int a, int b, int c) __asm__("FUN_00212f90");
extern float D_0015ED70_lui __asm__("D_0015ED70");

void sandmouse_house_update(unsigned char *m) __asm__("FUN_L01_002f2b68");
void sandmouse_house_update(unsigned char *moby) {
    enum {
        SANDMOUSE_HOUSE_INITIALIZE = 0,
        SANDMOUSE_HOUSE_WAIT_FOR_INITIAL_COMMAND = 1,
        SANDMOUSE_HOUSE_WAIT_FOR_INITIAL_COMMAND_ANIMATION_END = 2,
        SANDMOUSE_HOUSE_WAIT_FOR_FOLLOWUP_COMMAND = 3,
        SANDMOUSE_HOUSE_WAIT_FOR_FOLLOWUP_COMMAND_ANIMATION_END = 4,
        SANDMOUSE_HOUSE_WAIT_FOR_POST_FOLLOWUP_COMMAND = 5,
        SANDMOUSE_HOUSE_WAIT_FOR_POST_FOLLOWUP_COMMAND_ANIMATION_END = 6,
        SANDMOUSE_HOUSE_RESTORE_INITIAL_ANIMATION = 7
    };
    char *house_vars;
    if (moby == 0)
        return;
    house_vars = *(char **)(moby + 0x78);
    if (house_vars != 0) {
        if (moby[0x20] != 0) {
            if (D_0013D4C5 != 0) {
                if (hero.items[2].item_id == 5) {
                    float yaw_response_scale = D_0015ED70_lui;
                    *(float *)(moby + 0x48) =
                        FUN_L00_00258110_w7((float *)(house_vars + 4), *(float *)(moby + 0x48), *(float *)house_vars,
                                            yaw_response_scale * 6.2831855f, yaw_response_scale * 5.7595863f, 0.0f);
                }
            }
        }
    }
    switch (moby[0x20]) {
    case SANDMOUSE_HOUSE_INITIALIZE:
        if (moby[0x53] != 0) {
            blend_moby_animation_w7(moby, 0, 0, FUN_001f96f8_w7(0));
        }
        moby[0x20] = 1;
        *(float *)house_vars = *(float *)(moby + 0x48);
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_INITIAL_COMMAND:
        if ((unsigned char)(moby[0xBC] - 3) < 3) {
            if (moby[0x53] != 1) {
                blend_moby_animation_w7(moby, 1, 0, FUN_001f96f8_w7(10));
            }
            moby[0x20] = 2;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_INITIAL_COMMAND_ANIMATION_END:
        if (moby[0x70] & 2) {
            if (moby[0x53] != 2) {
                blend_moby_animation_w7(moby, 2, 0, FUN_001f96f8_w7(3));
            }
            moby[0x20] = 3;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_FOLLOWUP_COMMAND:
        if ((unsigned char)(moby[0xBC] - 4) < 2) {
            if (moby[0x53] != 3) {
                blend_moby_animation_w7(moby, 3, 0, FUN_001f96f8_w7(3));
            }
            moby[0x20] = 4;
        } else if (moby[0xBC] == 7 || moby[0xBC] == 1) {
            if (moby[0x53] != 6) {
                blend_moby_animation_w7(moby, 6, 0, FUN_001f96f8_w7(3));
            }
            moby[0x20] = 7;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_FOLLOWUP_COMMAND_ANIMATION_END:
        if (moby[0x70] & 2) {
            if (moby[0x53] != 4) {
                blend_moby_animation_w7(moby, 4, 0, FUN_001f96f8_w7(3));
            }
            moby[0x20] = 5;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_POST_FOLLOWUP_COMMAND:
        if (moby[0xBC] == 3 || moby[0xBC] == 6 || moby[0xBC] == 7 || moby[0xBC] == 1) {
            if (moby[0x53] != 5) {
                blend_moby_animation_w7(moby, 5, 0, FUN_001f96f8_w7(3));
            }
            moby[0x20] = 6;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_POST_FOLLOWUP_COMMAND_ANIMATION_END:
        if (moby[0x70] & 2) {
            if (moby[0xBC] == 7 || moby[0xBC] == 1) {
                if (moby[0x53] != 6) {
                    blend_moby_animation_w7(moby, 6, 0, FUN_001f96f8_w7(3));
                }
                moby[0x20] = 7;
            } else {
                if (moby[0x53] != 2) {
                    blend_moby_animation_w7(moby, 2, 0, FUN_001f96f8_w7(3));
                }
                moby[0x20] = 3;
            }
        }
        break;
    case SANDMOUSE_HOUSE_RESTORE_INITIAL_ANIMATION:
        if (moby[0x70] & 2) {
            if (moby[0x53] != 0) {
                blend_moby_animation_w7(moby, 0, 0, FUN_001f96f8_w7(3));
            }
            moby[0x20] = 1;
        }
        break;
    case 8:
        break;
    }
}
#include "sda.h"
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"

/* pvars of the moby run by FUN_L01_002f2eb8 */
typedef struct {
    Vec4 home;         /* position at spawn */
    union {
        struct {
            s32 path;   /* -1, or an index into D_L01_001B0930 */
            s32 volume; /* -1, or a clip volume is_point_inside_clip_volume tests */
        } s;
        s64 both;       /* -1 when neither is set */
    } area;
    s32 count;         /* spawn budget; the moby is deleted when it runs out */
} AreaVars_2f2eb8;

extern int D_0015ED84;
extern s32 *D_L01_001B0930_2f2eb8[] __asm__("D_L01_001B0930");
extern void FUN_0020c828_2f2eb8(struct Moby *) __asm__("FUN_0020c828");
extern s32 FUN_L00_00259740_2f2eb8(void *, void *, s32) __asm__("FUN_L00_00259740");
extern s32 FUN_00214720_2f2eb8(void *, s32) __asm__("FUN_00214720");
extern f32 FUN_001f9b80_2f2eb8(void *, void *) __asm__("FUN_001f9b80");

extern s32 D_L01_00161AF0 __attribute__((sda));
void update_nearest_area_moby_selection(struct Moby *moby) __asm__("FUN_L01_002f2eb8");

void update_nearest_area_moby_selection(struct Moby *moby) {
    AreaVars_2f2eb8 *area_vars = (AreaVars_2f2eb8 *)moby->pvars;
    Vec4 target;
    s32 k;
    s32 *path;
    f32 target_distance;

    if (moby->state == 0) {
        if (area_vars == 0) {
            FUN_0020c828_2f2eb8(moby);
            return;
        }
        moby->update_dist = 0xFF;
        qcopy(&area_vars->home, &moby->pos);
        moby->flags |= 0x41;
        moby->state = 1;
        if (moby->command == 0 && D_0015ED84 < 20) {
            k = D_0014BF10[D_L01_00161AF0 / 2 + D_0015ED84 * 16];
            if (D_L01_00161AF0 & 1) {
                k &= 0xF;
            } else {
                k = (k >> 4) & 0xF;
            }
            D_L01_00161AF0++;
            moby->command = D_L01_00161AF0;
            k *= 5;
            area_vars->count -= k;
            if (area_vars->count <= 0) {
                FUN_0020c828_2f2eb8(moby);
                return;
            }
        }
    } else {
        D_L01_00161AF0 = 0;
    }
    if (hero.items[0].moby != 0 && hero.items[0].item_id == 0x1B) {
        qcopy(&target, hero.items[0].moby->pvars + 0x10);
    } else {
        qcopy(&target, &hero.motion.pos);
    }
    if (area_vars->area.s.path != -1) {
        path = D_L01_001B0930_2f2eb8[area_vars->area.s.path];
        k = FUN_L00_00259740_2f2eb8(&target, path + 4, path[0]);
        if (k) {
            goto in_range;
        }
    }
    if (area_vars->area.s.volume != -1 && FUN_00214720_2f2eb8(&target, area_vars->area.s.volume)) {
        goto in_range;
    }
    if (area_vars->area.both != -1) {
        return;
    }
in_range:
    target_distance = FUN_001f9b80_2f2eb8(&moby->pos, &target);
    if (target_distance < hero.detector_target_dist) {
        hero.metal_detector_target = moby;
        hero.detector_target_dist = target_distance;
        if (target_distance < 20.0f) {
            hero.nearby_moby_close = 1;
        }
    }
}
/* Builds two scaled offset vectors from the moby's matrix and adds them to out. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_0029DA60), where it is exact; names translated to the US level program. */

extern void FUN_001f9d20_q(void *, void *, void *) __asm__("FUN_001f9d20");
extern void FUN_001fa050_q(float *, float *) __asm__("FUN_001fa050");
extern void add_vector_xyz_q(void *, void *, void *) __asm__("FUN_001f9a10");
extern void cross_vectors_xyz_q(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void scale_vector_xyz_q(void *out, void *a, f32 s) __asm__("FUN_001f9a68");

void apply_gunship_path_position_offsets(char *moby, float *out) __asm__("FUN_L01_002f5040");

void apply_gunship_path_position_offsets(char *moby, float *out) {
    float unit_x_axis[4];
    float rotated_x_axis[4];
    float horizontal_offset[4];
    float vertical_plane_offset[4];
    float rotated_x_axis_minus_unit_z[4];
    float rotation_matrix[16];
    char *data;
    *(OvlQuad *)unit_x_axis = 0;
    unit_x_axis[0] = 1.0f;
    data = *(char **)(moby + 0x78);
    if (*(float *)(data + 0x130) != 0.0f || *(float *)(data + 0x134) != 0.0f) {
        FUN_001fa050_q(rotation_matrix, (float *)(moby + 0x40));
        FUN_001f9d20_q(rotated_x_axis, unit_x_axis, rotation_matrix);
        *(OvlQuad *)rotated_x_axis_minus_unit_z = *(OvlQuad *)rotated_x_axis;
        rotated_x_axis_minus_unit_z[2] = rotated_x_axis_minus_unit_z[2] - 1.0f;
        cross_vectors_xyz_q(horizontal_offset, rotated_x_axis, rotated_x_axis_minus_unit_z);
        cross_vectors_xyz_q(vertical_plane_offset, rotated_x_axis, horizontal_offset);
        scale_vector_xyz_q(horizontal_offset, horizontal_offset, *(float *)(data + 0x130));
        scale_vector_xyz_q(vertical_plane_offset, vertical_plane_offset, *(float *)(data + 0x134));
        add_vector_xyz_q(out, out, horizontal_offset);
        add_vector_xyz_q(out, out, vertical_plane_offset);
    }
}
#include "rnc/gameplay/entities/moby.h"

typedef union { u128 q; f32 f[4]; struct { f32 x, y, z, w; } v; } QVec;

typedef struct {
    s32 count;
    u8 pad4[0xC];
    QVec node[1];
} PathNodes;

typedef struct {
    s32 idx;
    s8 dir;
    u8 state;
    u8 pad6[2];
    s32 unk8;
    f32 dist;
    PathNodes *nodes;
    s32 path_id;
    f32 heading;
    s32 unk1C;
} PathCursor;

typedef struct {
    u8 pad0[0x60];
    PathCursor path;          /* 0x60 */
    QVec unk80;               /* 0x80 */
    QVec unk90;               /* 0x90 */
    QVec unkA0;               /* 0xA0 */
    QVec unkB0;               /* 0xB0 */
    QVec unkC0;               /* 0xC0 */
    QVec unkD0;               /* 0xD0 */
    QVec unkE0;               /* 0xE0 */
    f32 unkF0;
    f32 unkF4;
    f32 unkF8;
    f32 unkFC;
    f32 unk100;
    f32 unk104;
    f32 unk108;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    f32 unk118;
    s16 unk11C;
    s16 unk11E;
    u8 unk120;
    u8 unk121;
    s16 unk122;
    f32 unk124;
    f32 unk128;
    f32 unk12C;
    f32 unk130;
    f32 unk134;
    s16 unk138;
    s16 unk13A;
    s16 unk13C;
    s16 unk13E;
    s32 unk140[4];
} Vars;

typedef struct {
    u8 pad0[0x70];
    s32 unk70;
    u8 pad74[0xC];
    QVec unk80;
} ChildVars;

typedef struct {
    u8 pad0[0x28];
    u16 kind;
} HitInfo;

extern s32 D_L01_00161B00;
extern char D_L01_0020B3E0[];
extern s32 D_0015ED84;
extern f32 D_0015ED60;
extern f32 D_0015ED6C;
extern f32 D_0015ED70;
extern void *D_L01_00160770;

extern f32 FUN_001fa6c0(s32);
extern s32 check_scaled_vector_bsphere(struct Moby *, f32) __asm__("FUN_L01_00275690");
extern s32 get_first_moby_in_group(struct Moby **, s32, s32, s32) __asm__("FUN_L00_002591d0");
extern s32 get_next_moby_in_group(struct Moby **, struct Moby *, s32, s32) __asm__("FUN_L00_002592b8");
extern s32 FUN_001f96f8(s32);
extern s32 FUN_L00_0028d8c0(struct Moby *, s32);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern s32 FUN_001f9770(s16 *);
extern f32 FUN_001f9b48(void *, void *);
extern s32 FUN_00213260(s32);
extern s32 FUN_L00_00257b90(s32, s32);
extern s32 FUN_001fa6d0(f32);
extern s32 FUN_001e93b0(const char *, ...);
extern f32 FUN_002132a8(f32, f32);
extern void initialize_path_descriptor(PathCursor *) __asm__("FUN_L01_0028b410");
extern s32 select_point_index_by_distance(void *, PathNodes *, f32) __asm__("FUN_L01_0028b510");
extern f32 FUN_001f9e90(f32, f32);
extern void compute_path_node_tangent(QVec *, PathCursor *, s32, f32, f32) __asm__("FUN_L01_0028b8c8");
extern f32 FUN_001f9b20(void *);
extern f32 FUN_00213308(void);
extern f32 compute_sampled_spline_path_lengths(f32, f32, PathCursor *, s32, struct Moby *) __asm__("FUN_L01_0028bb90");
extern void FUN_L00_0024f7c8(struct Moby *, s32, QVec *);
extern void FUN_001f9a28(void *, void *, void *);
extern void FUN_001f9bf8(void *, void *, f32);
extern void FUN_0022d798(s32);
extern s32 advance_path_node_index(PathCursor *) __asm__("FUN_L01_0028b878");
extern s32 resolve_path_node_index(PathCursor *, s32) __asm__("FUN_L01_0028b828");
extern f32 FUN_001f9de0(f32);
extern f32 FUN_001f9dc8(f32);
extern f32 FUN_001fa580(f32, f32);
extern f32 FUN_001fa5c8(f32, f32);
extern void evaluate_cubic_hermite_segment(f32, QVec *, QVec *, QVec *, QVec *, QVec *) __asm__("FUN_L01_0028ba80");
extern f32 FUN_L00_00258110(f32 *, f32, f32, f32, f32, f32);
extern f32 FUN_L00_0025e310(f32);
extern void FUN_001f99f8(void *);
extern void spawn_type_10_particle(QVec *, QVec *) __asm__("FUN_L01_0027f660");
extern void FUN_001f9a68(void *, void *, f32);
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_L00_0025e450(struct Moby *, QVec *, QVec *, f32, f32, s32, s32, s32, f32, f32, f32, f32,
                             s32, f32, s32, s32, s32, s32);

void gunship_follow_path(struct Moby *m) __asm__("FUN_L01_002f5168");

void gunship_follow_path(struct Moby *moby) {
    enum {
        GUNSHIP_PATH_STATE_INITIALIZE = 0,
        GUNSHIP_PATH_STATE_FOLLOW_PATH = 1,
        GUNSHIP_PATH_STATE_WAIT_FOR_PATH = 2,
        GUNSHIP_PATH_STATE_WAIT_FOR_RESUME_TIMER = 3
    };
    QVec path_displacement;
    QVec zero;
    QVec previous_rotation;
    QVec previous_position;
    QVec off[4];
    QVec dA0;
    struct Moby *it;
    Vars *v;
    PathCursor *path_cursor;
    s32 frozen;
    s32 i;
    s32 never = 0;
    f32 path_speed;
    f32 segment_progress_step;

    zero.q = 0;
    frozen = 0;
    v = (Vars *)moby->pvars;
    if (moby->oclass != 0x336) {
        frozen = check_scaled_vector_bsphere(moby, FUN_001fa6c0(moby->draw_dist)) == -1;
    }
    if (v == 0 || D_L01_00161B00 != 0) {
        return;
    }
    path_cursor = &v->path;
    if (moby->group != 0xFF && moby->state != 0 && moby->state != 3) {
        s32 bad = 0;
        get_first_moby_in_group(&it, moby->group, 0, 0);
        if (it == moby) {
            goto check;
            do {
                get_next_moby_in_group(&it, it, 0, 0);
            check:
                if (it == 0)
                    goto done;
            } while (check_scaled_vector_bsphere(it, FUN_001fa6c0(it->draw_dist)) == -1);
            bad = 1;
        done:
            if (bad == 0) {
                get_first_moby_in_group(&it, moby->group, 0, 0);
                while (it != 0) {
                    Vars *iv = (Vars *)it->pvars;
                    bad = 3;
                    it->state = bad;
                    iv->unk11C = FUN_001f96f8(5);
                    get_next_moby_in_group(&it, it, 0, 0);
                }
            }
        }
    }
    if ((moby->oclass == 0x33 || moby->oclass == 0x46A || moby->oclass == 0x473 || moby->oclass == 0x474)
        && (level_frame_count & 7) == (((s32)moby >> 8) & 7) && v->unk13A <= 0 && moby->state != 0
        && FUN_L00_0028d8c0(moby, v->unk11E) == 0) {
        v->unk11E = allocate_voice_for_target_entry(0, 4, moby);
    }
    switch ((s16)(moby->oclass - 0x33)) {
    case 0:
    case 9:
    case 23:
        if (v->unk138 != -1) {
            if (!FUN_001f9770(&v->unk138))
                break;
            if (FUN_001f9b48(&moby->pos, D_L01_00167240) < 30.0f)
                allocate_voice_for_target_entry(1, 0, moby);
        }
        v->unk138 = FUN_001f96f8(FUN_L00_00257b90(v->unk13C, v->unk13C + v->unk13E));
        break;
    case 13:
    case 17:
        /* Fakematch: duplicating the reset preserves retail's voice-call argument scheduling. */
        if (v->unk138 != -1) {
            if (!FUN_001f9770(&v->unk138))
                break;
            FUN_0022da68((FUN_00213260(100) & 1) ? 1 : 2, 0, (char *)moby);
            v->unk138 = FUN_001f96f8(FUN_L00_00257b90(v->unk13C, v->unk13C + v->unk13E));
        } else {
            v->unk138 = FUN_001f96f8(FUN_L00_00257b90(v->unk13C, v->unk13C + v->unk13E));
        }
        break;
    }

    switch (moby->state) {
    case GUNSHIP_PATH_STATE_INITIALIZE:
        if (path_cursor->path_id != -1) {
            PathNodes *pn = (PathNodes *)D_L01_001B0930[path_cursor->path_id];
            if (FUN_001f9b48(&pn->node[0], &pn->node[pn->count - 1]) < 0.5f)
                pn->count--;
        } else {
            FUN_001e93b0(D_L01_0020B3E0, moby->oclass, FUN_001fa6d0((f32)(moby - (struct Moby *)D_L01_0015FFD8)));
        }
        moby->update_dist = 0xFF;
        v->unk11E = -1;
        if (500.0f < v->unk130)
            v->unk130 = FUN_002132a8(-1.0f, 1.0f) * v->unk130 / 1000.0f;
        if (500.0f < v->unk134)
            v->unk134 = FUN_002132a8(-1.0f, 1.0f) * v->unk134 / 1000.0f;
        if (path_cursor->path_id == -1) {
            moby->state = 2;
            return;
        }
        v->unk121 = (moby - (struct Moby *)D_L01_0015FFD8) / 2 % 7;
        initialize_path_descriptor(path_cursor);
        if (path_cursor->state == 0) {
            if (path_cursor->idx <= 0) {
                s32 selected_node_index = select_point_index_by_distance(&moby->pos, path_cursor->nodes, 0.0f);
                path_cursor->idx = selected_node_index;
                if ((path_cursor->dir >= 0 && selected_node_index == path_cursor->nodes->count - 1) || (path_cursor->dir < 0 && selected_node_index == 0))
                    path_cursor->idx = 0;
            }
            path_cursor->dist = FUN_001f9b48(&moby->pos, &path_cursor->nodes->node[path_cursor->idx]);
            path_cursor->heading = FUN_001f9e90(path_cursor->nodes->node[path_cursor->idx].v.x - moby->pos.x,
                                      path_cursor->nodes->node[path_cursor->idx].v.y - moby->pos.y);
            path_cursor->state = 2;
            path_cursor->unk8 = 0;
            path_cursor->unk1C = 0;
        }
        v->unkE0.q = (path_cursor->nodes->node + path_cursor->idx)->q;
        v->unkF0 = 1.0f;
        compute_path_node_tangent(&v->unkA0, path_cursor, 0, v->unkF4, v->unkF8);
        v->unkB0.q = v->unkC0.q = (path_cursor->nodes->node + path_cursor->idx)->q;
        *(s32 *)&moby->rot.x = 0;
        moby->rot.y = -FUN_001f9e90(FUN_001f9b20(&v->unkA0), v->unkA0.v.z);
        moby->rot.z = FUN_001f9e90(v->unkA0.v.x - zero.v.x, v->unkA0.v.y - zero.v.y);
        v->unk104 = 0;
        v->unk10C = 0;
        *(u128 *)&moby->pos = v->unkE0.q;
        apply_gunship_path_position_offsets((char *)moby, (float *)&moby->pos);
        v->unk121 = 7;
        if (6.2831855f < v->unk12C) {
            v->unk12C = v->unk12C * 0.017453292f;
        } else if (v->unk124 != 0.0f && v->unk12C == -1.0f) {
            v->unk12C = FUN_00213308();
        }
        v->unkD0.q = v->unkC0.q;
        compute_sampled_spline_path_lengths(v->unkF4, v->unkF8, path_cursor, v->unk120 & 0x20, moby);
        if ((v->unk120 & 0x1E) && !(v->unk120 & 1)) {
            s32 i;
            for (i = 0; i < 4; i++) {
                struct Moby *c;
                ChildVars *cv;
                if (!(v->unk120 & (2 << i)) || v->unk140[i] == -1)
                    continue;
                c = (struct Moby *)D_L01_0015FFD8 + v->unk140[i];
                if (c == 0 || (cv = (ChildVars *)c->pvars) == 0)
                    continue;
                cv->unk70 |= 0x1000;
                FUN_L00_0024f7c8(moby, i, &off[i]);
                FUN_001f9a28(&dA0, &off[i], &moby->pos);
                FUN_001f9bf8(&dA0, &dA0, 1.0f);
                *(u128 *)&c->pos = off[i].q;
                cv->unk80.q = dA0.q;
            }
        }
        if (v->unk11C > 0) {
            moby->state = 3;
            return;
        }
        if (D_0015ED84 == 3 || D_0015ED84 == 9)
            v->unk13A = 0;
        moby->state = 1;
        return;

    case GUNSHIP_PATH_STATE_WAIT_FOR_PATH:
        if (path_cursor->path_id == -1)
            return;
        moby->state = 0;
        if (v->unk11E != -1) {
            u8 *e = (u8 *)D_0013E550 + v->unk11E * 0x70;
            if (*(struct Moby **)(e + 0x88) == moby && e[0x74] != 0)
                FUN_0022d798(v->unk11E);
        }
        v->unk11E = -1;
        return;

    case GUNSHIP_PATH_STATE_WAIT_FOR_RESUME_TIMER:
        if (FUN_001f9770(&v->unk11C)) {
            v->unk11C = 0;
            moby->state = 1;
        }
        return;

    case GUNSHIP_PATH_STATE_FOLLOW_PATH:
        break;

    default:
        return;
    }

    previous_rotation.q = *(u128 *)&moby->rot;
    previous_position.q = *(u128 *)&moby->pos;
    if (1.0f <= v->unkF0) {
        s32 advanced_node_index;
        v->unkF0 -= 1.0f;
        advanced_node_index = advance_path_node_index(path_cursor);
        v->unk90.q = v->unkA0.q;
        compute_path_node_tangent(&v->unkA0, path_cursor, 1, v->unkF4, v->unkF8);
        v->unkB0.q = v->unkC0.q;
        v->unkC0.q = (path_cursor->nodes->node + resolve_path_node_index(path_cursor, advanced_node_index + 1))->q;
    }
    {
        f32 speed_modulation_sine = FUN_001f9de0(v->unk12C);
        path_speed = v->unkFC + v->unk124 * D_0015ED60 * speed_modulation_sine * D_0015ED60;
        segment_progress_step = path_speed / v->unkB0.v.w;
        v->unk12C = FUN_001fa580(v->unk12C, v->unk128 * D_0015ED60 * 0.017453292f);
    }
    v->unkE0.q = v->unkD0.q;
    evaluate_cubic_hermite_segment(v->unkF0 + segment_progress_step, &v->unkD0, &v->unkB0, &v->unkC0, &v->unk90, &v->unkA0);
    if (!(v->unk120 & 0x40)) {
        f32 dd = FUN_001f9b48(&v->unkE0, &v->unkD0);
        if (dd != 0.0f) {
            segment_progress_step = segment_progress_step * (path_speed / dd);
            evaluate_cubic_hermite_segment(v->unkF0 + segment_progress_step, &v->unkD0, &v->unkB0, &v->unkC0, &v->unk90, &v->unkA0);
        }
    }
    if (!frozen) {
        f32 k = 0.017453292f;
        f32 steering_angle;
        f32 turn;
        f32 yaw_offset_radians = (f32)v->unk122 * k;
        FUN_001fa5c8(moby->rot.z, yaw_offset_radians);
        steering_angle = FUN_001f9e90(v->unkD0.v.x - v->unkE0.v.x, v->unkD0.v.y - v->unkE0.v.y);
        steering_angle = FUN_001fa580(steering_angle, yaw_offset_radians);
        steering_angle = FUN_001fa5c8(steering_angle, moby->rot.z);
        turn = steering_angle * (1.0f / v->unk110 * D_0015ED60);
        moby->rot.z = FUN_001fa580(turn, moby->rot.z);
        FUN_001f9a28(&path_displacement, &v->unkD0, &v->unkE0);
        steering_angle = -FUN_001f9e90(FUN_001f9b20(&path_displacement), path_displacement.v.z);
        moby->rot.y = FUN_L00_00258110(&v->unk10C, moby->rot.y, steering_angle,
                                            v->unk108 * 10.0f * k * D_0015ED70,
                                            v->unk108 * 5.0f * k * D_0015ED70,
                                            v->unk108 * k * D_0015ED6C);
        steering_angle = FUN_L00_0025e310(turn * v->unk114);
        steering_angle = FUN_001fa5c8(moby->rot.x, steering_angle);
        steering_angle = FUN_L00_0025e310(steering_angle * v->unk118);
        moby->rot.x = FUN_L00_00258110(&v->unk104, moby->rot.x, steering_angle,
                                            v->unk100 * 10.0f * k * D_0015ED70,
                                            v->unk100 * 5.0f * k * D_0015ED70,
                                            v->unk100 * k * D_0015ED6C);
    } else {
        FUN_001f99f8(&moby->rot);
        moby->rot.z = FUN_001f9e90(v->unkD0.v.x - v->unkE0.v.x, v->unkD0.v.y - v->unkE0.v.y);
    }
    v->unkF0 += segment_progress_step;
    *(u128 *)&moby->pos = v->unkE0.q;
    if (!frozen)
        apply_gunship_path_position_offsets((char *)moby, (float *)&moby->pos);
    if (v->unk120 & 0x1F) {
        s32 attachment_point_index = 0;
        s32 i;
        for (i = 0; i < 5; i++) {
            if (!((v->unk120 >> i) & 1))
                continue;
            FUN_L00_0024f7c8(moby, attachment_point_index, &off[0]);
            attachment_point_index++;
            FUN_001f9a28(&off[1], &off[0], &moby->pos);
            if (!(v->unk120 & 1)) {
                struct Moby *c;
                ChildVars *cv;
                if (v->unk140[i - 1] == -1)
                    continue;
                c = (struct Moby *)D_L01_0015FFD8 + v->unk140[i - 1];
                if (c == 0 || (cv = (ChildVars *)c->pvars) == 0)
                    continue;
                FUN_001f9bf8(&off[1], &off[1], 1.0f);
                cv->unk70 |= 0x1000;
                *(u128 *)&c->pos = off[0].q;
                cv->unk80.q = off[1].q;
            } else if (v->unk121 == 0) {
                FUN_001f9bf8(&off[1], &off[1], FUN_002132a8(0.016f, 0.031f) * D_0015ED60);
                spawn_type_10_particle(&off[0], &off[1]);
                FUN_001f9bf8(&off[1], &off[1], FUN_002132a8(0.016f, 0.031f) * D_0015ED60);
                spawn_type_10_particle(&off[0], &off[1]);
            }
        }
        if (v->unk121 == 0)
            v->unk121 = FUN_001f96f8(7);
        v->unk121--;
    }
    if (D_0015ED84 != 3 && D_0015ED84 != 9)
        return;
    {
        HitInfo *hit = (HitInfo *)FUN_L00_0025a420(moby, 0x210000, 0);
        if (hit != 0 && hit->kind != 0x102) {
            QVec *current_position;
            s32 k;
            QVec *dir;
            FUN_001f99f8(&off[0]);
            if (moby->oclass == 0x33) {
                off[0].v.x = FUN_001f9dc8(moby->rot.z) * -2.7470f;
                off[0].v.y = FUN_001f9de0(moby->rot.z) * -2.7470f;
                *(s32 *)&off[0].v.z = 0;
            }
            current_position = (QVec *)&moby->pos;
            FUN_001f9a28(&off[2], current_position, &previous_position);
            off[1].q = off[2].q;
            FUN_001f9a68(&off[1], &off[1], 0.5f);
            FUN_001f9a10(&off[0], &off[0], current_position);
            v->unk13A = FUN_001f96f8(600);
            dir = &off[1];
            /* Fakematch: the never-taken call keeps gcse from propagating &off[1] into the call, so dir
               gets a1 before the class tests and &off[1] gets s0, as in retail. */
            if (never)
                dir = (QVec *)FUN_001f96f8(0);
            k = -1;
            if (moby->oclass == 0x532 || moby->oclass == 0x53B || moby->oclass == 0x53C)
                k = 1;
            FUN_L00_0025e450(moby, dir, &off[0], 0.0f, 0.0f, 0x14, 3, 4, 4.0f, 2.0f, 100000.0f, 3.0f, k, 15.0f, 1,
                             1, -1, 0);
        }
    }
    D_L01_00160770 = (void *)gunship_follow_path;
    moby->hit_slot = 0xFF;
    if (v->unk13A == 0)
        return;
    if (FUN_001f9770(&v->unk13A) && frozen) {
        moby->collision = ((u32 *)moby->pclass)[4];
        moby->flags = (moby->flags & 0xFFBE) | 0x1000;
        return;
    }
    moby->collision = 0;
    moby->flags |= 0x41;
    if (v->unk11E != -1) {
        u8 *e = (u8 *)D_0013E550 + v->unk11E * 0x70;
        if (*(struct Moby **)(e + 0x88) == moby && e[0x74] != 0)
            FUN_0022d798(v->unk11E);
    }
    v->unk11E = -1;
    moby->flags &= 0xEFFF;
}

#include "rnc/rendering/dma_tag.h"
extern void FUN_L01_00261c10(void *, void *) __asm__("FUN_L01_00261c10");
extern void FUN_L01_00261df8(f32 *) __asm__("FUN_L01_00261df8");
extern void update_animated_offset_pairs(f32 *) __asm__("FUN_L01_002b80d0");
extern u64 get_effect_texture(s32) __asm__("FUN_001f44b8");

void render_water_grids(f32 *water_record, s32 count) __asm__("FUN_L01_002b91c8");

void render_water_grids(f32 *water_record, s32 count) {
    f32 corners[4][4];
    f32 near_clip[4];
    f32 far_clip[4];
    f32 x_offset, y_offset, x_end, y_end, z;
    u32 near_bits, far_bits;
    s32 mask;
    s32 i, j, mode;
    s32 height_index, index;
    f32 *height;
    u32 *packet;
    u64 *data;

    build_water_render_setup_packet();
    i = 0;
    if (count > 0) do {
        f32 *next_pos = water_record + 0x464;
        u16 active = *(u16 *)((u8 *)water_record + 0x1e);
        if (active == 0) goto next_entity;
        x_offset = water_record[0] + pool_light[2];
        x_end = water_record[0] - pool_light[2];
        corners[0][0] = x_offset;
        y_offset = water_record[1] + pool_light[3];
        y_end = water_record[1] - pool_light[3];
        corners[0][1] = y_offset;
        z = water_record[2];
        corners[0][2] = z;
        corners[0][3] = 1.0f;
        corners[1][0] = x_end;
        corners[1][1] = y_offset;
        corners[1][2] = z;
        corners[1][3] = 1.0f;
        corners[2][0] = x_offset;
        corners[2][1] = y_end;
        corners[2][2] = z;
        corners[2][3] = 1.0f;
        corners[3][0] = x_end;
        corners[3][1] = y_end;
        corners[3][2] = z;
        corners[3][3] = 1.0f;
        near_bits = 0;
        far_bits = 0;
        for (j = 0; j < 4; j++) {
            subtract_vector_xyz(near_clip, corners[j], D_L01_00167240);
            scale_vector_xyz(far_clip, near_clip, 1024.0f);
            scale_vector_xyz(near_clip, near_clip, 1024.0f);
            transform_vector(far_clip, far_clip, D_L01_00167240 - 0x80);
            transform_vector(near_clip, near_clip, D_L01_00167240 - 0x40);
            far_bits = (far_bits << 8) | is_vector_outside_clip(far_clip);
            near_bits = (near_bits << 8) | is_vector_outside_clip(near_clip);
        }
        if ((near_bits & 0x01010101) == 0x01010101) goto next_entity;
        if ((near_bits & 0x02020202) == 0x02020202) goto next_entity;
        if ((near_bits & 0x04040404) == 0x04040404) goto next_entity;
        if ((near_bits & 0x08080808) == 0x08080808) goto next_entity;
        if ((near_bits & 0x20202020) == 0x20202020) goto next_entity;

        FUN_L01_00261c10((void *)0x70000000, (u8 *)water_record + (D_L01_00161220 * 0x5c0 + 0x50));
        FUN_L01_00261df8(water_record);
        build_height_grid_positions(water_record);
        update_animated_offset_pairs(water_record);
        if (*(u32 *)(water_record + 0x11) & 1) {
            height = (f32 *)0x70002008;
            for (height_index = 0x10; height_index >= 0; height_index--, height += 3) *height = water_record[2];
        }
        if (*(u32 *)(water_record + 0x11) & 2) {
            height = (f32 *)0x700020c8;
            for (height_index = 0x10; height_index >= 0; height_index--, height += 0x33) *height = water_record[2];
        }
        if (*(u32 *)(water_record + 0x11) & 4) {
            height = (f32 *)0x70002cc8;
            for (height_index = 0x10; height_index >= 0; height_index--, height += 3) *height = water_record[2];
        }
        if (*(u32 *)(water_record + 0x11) & 8) {
            height = (f32 *)0x70002008;
            for (height_index = 0x10; height_index >= 0; height_index--, height += 0x33) *height = water_record[2];
        }
        mode = (far_bits & 0x2f2f2f2f) != 0;
        if (!mode) {
            mode = 2;
            if (!(near_bits & 0x2f2f2f2f)) mode = 0;
        }
        mask = *(u16 *)((u8 *)water_record + 0x1e);
        if (*(u32 *)(water_record + 5) != cached_water_texture_id_gp || *(u32 *)(water_record + 6) != cached_water_second_texture_id_gp ||
            *(u8 *)(water_record + 7) != D_L01_00161230 || *(u8 *)((u8 *)water_record + 0x1d) != cached_water_alpha_1_fix_gp) {
            l01_packet.words[0] = 0x10000005;
            l01_packet.words[1] = 0;
            l01_packet.words[2] = 0x11000000;
            l01_packet.words[3] = 0x50000005;
            packet = l01_packet.words;
            l01_packet.words += 4;
            data = (u64 *)(packet + 4);
            data[0] = 0x4000000000008001ULL;
            data[1] = 0xeeee;
            data[2] = ((u64)*(u8 *)((u8 *)water_record + 0x1d) << 32) | 100;
            data[3] = 0x42;
            data[4] = ((u64)*(u8 *)(water_record + 7) << 32) | 100;
            data[5] = 0x43;
            data[6] = get_effect_texture(*(s32 *)(water_record + 6));
            data[7] = 6;
            data[8] = get_effect_texture(*(s32 *)(water_record + 5));
            data[9] = 7;
            l01_packet.words = packet + 0x18;
            {
                u32 texture_id = *(u32 *)(water_record + 5);
                u32 second_texture_id = *(u32 *)(water_record + 6);
                cached_water_texture_id_gp = texture_id;
                cached_water_second_texture_id_gp = second_texture_id;
            }
            D_L01_00161230 = *(u8 *)(water_record + 7);
            cached_water_alpha_1_fix_gp = *(u8 *)((u8 *)water_record + 0x1d);
        }
        for (index = 0; index < 16; index++, mask = (s32)mask >> 1) {
            if (mask & 1) cull_and_submit_water_grid_tile(water_record, index, mode);
        }
next_entity:
        i++;
        water_record = next_pos;
    } while (i < count);
    l01_packet.words[0] = 0x10000000;
    l01_packet.words[1] = 0;
    l01_packet.words[2] = 0x13000000;
    l01_packet.words[3] = 0;
    l01_packet.words += 4;
}
