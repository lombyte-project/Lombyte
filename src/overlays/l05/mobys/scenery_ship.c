/* Level 05 moby class scenery_ship: ships flying a path, with glow quads and trails. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/overlay/l05/mobys/scenery_ship_globals.h"
#include "rnc/math/random.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/moby_spawn.h"

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D8190), where it is exact; names translated to the US level program. */

extern int  get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int  get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

void draw_group_moby_glow_quads(unsigned char *moby) __asm__("FUN_L05_002d6f48");

void draw_group_moby_glow_quads(unsigned char *moby) {
    unsigned char *it;
    int            i;
    char          *group_member_vars;
    char          *glow_position;
    get_first_moby_in_group((int *)&it, moby[0x21], 0, 0);
    while (it != 0) {
        if (it[0x31] != 0) {
            group_member_vars = *(char **)(it + 0x78);
            if (*(short *)(it + 0xA6) == MOBY(moby)->oclass) {
                glow_position = group_member_vars + 0x330;
                for (i = 2; i >= 0; i--) {
                    draw_offset_textured_effect_quad(
                        glow_position, *(int *)(group_member_vars + 0x360), 0.45f, 0.4f);
                    glow_position += 0x10;
                }
            }
        }
        get_next_moby_in_group((int *)&it, (int)it, 0, 0);
    }
}

extern float fast_sin(float);

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int  lerp_2d7020(float, int, int) __asm__("FUN_001fa6e0");
extern int  frame_number __asm__("D_L05_0015F5CC");
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Per-frame glow: updates three parts, pulses the colour, and queues a redraw
 * once per frame. */
void update_moby_glow_and_queue_draw(char *moby) __asm__("FUN_L05_002d7020");

void update_moby_glow_and_queue_draw(char *moby) {
    char *moby_data;
    int   i;
    int   interpolated_glow_color;
    int   current_frame;
    int   k;
    int  *last_queued_frame_ptr;

    if (MOBY(moby)->group != 0xFF && MOBY(moby)->visible != 0) {
        moby_data = ((char *)MOBY(moby)->pvars);
        for (i = 0; i < 3; i++) {
            moby_point_position(moby, i, moby_data + 0x330 + i * 16);
        }
        *(float *)(moby_data + 0x368) =
            fast_add_rotations(*(float *)(moby_data + 0x368), frame_time * 1.9198622f);
        interpolated_glow_color =
            lerp_2d7020(fast_sin(*(float *)(moby_data + 0x368)) * 0.5f + 0.5f, 0x1E1EB4, 0x1EB41E);
        current_frame = frame_number;
        MOBY(moby)->glow_color = interpolated_glow_color;
        k = (interpolated_glow_color & 0xFF0000) | 0x30000000;
        *(int *)(moby_data + 0x360) =
            k | (interpolated_glow_color & 0xFF00) | (interpolated_glow_color & 0xFF);
        if (current_frame != 0) {
            last_queued_frame_ptr = *(int **)(moby_data + 0x364);
            if (current_frame != *last_queued_frame_ptr) {
                *last_queued_frame_ptr = current_frame;
                add_draw_callback(draw_group_moby_glow_quads, moby);
            }
        }
    }
}

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern char  D_L05_0015F580_macro[] __asm__("D_L05_0015F580") MACRO_ADDR;

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D8388), where it is exact; names translated to the US level program. */

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4f_2d7140;

typedef int u128_2D8388 __attribute__((mode(TI)));

typedef union {
    u128_2D8388 q;
    float       f[4];
} Buf;

typedef struct {
    /* 0x000 */ int            path;
    /* 0x004 */ float          f4;
    /* 0x008 */ float          f8;
    /* 0x00C */ float          fC;
    /* 0x010 */ int            pad10[3];
    /* 0x01C */ float          f1C;
    /* 0x020 */ Vec4f_2d7140   trail[3][16];
    /* 0x320 */ short          f320;
    /* 0x322 */ unsigned short f322;
    /* 0x324 */ int           *f324;
    /* 0x328 */ int            f328;
    /* 0x32C */ int            pad32C[15];
    /* 0x368 */ float          f368;
} Data_2D8388;

extern float D_0015ED6C;
extern float D_0015ED70;

#include "rnc/overlay/l05/mobys/scenery_ship_data/scenery_ship_update.h"

int          D_L05_001613D8 = 0x80000080;
int          D_L05_001613DC = 0x80202020;
float        D_L05_001613E0 = 5.0f;
int          D_L05_001613E4 = 1;
extern float scenery_ship_removal_effect_basis_scale_gp __asm__("D_L05_001613E8")
    __attribute__((sda));
extern float scenery_ship_removal_effect_z_addend_gp __asm__("D_L05_001613EC") __attribute__((sda));
void         draw_group_scenery_ship_trails(struct Moby *moby) __asm__("FUN_L05_002d7920");
extern float func_001F9B88_2D8388(float input) __asm__("FUN_001f99c0");
extern float func_001F9FA8_2D8388(float) __asm__("FUN_001f9de0");
extern int   func_001FA8A8_2D8388(int, int, float) __asm__("FUN_001fa6e0");
extern void  advance_scenery_ship_along_path(char *moby, int *path) __asm__("FUN_L05_002d7890");
extern int   func_L00_0028EB98_2D8388(void *, int) __asm__("FUN_L00_0028d8c0");
extern int   allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void  func_001F9BD8_2D8388(float *, float *, float *) __asm__("FUN_001f9a10");
extern int   func_001F9850_2D8388(int) __asm__("FUN_001f96f8");
extern void *func_L00_00265050_2D8388(char  *src,
                                      int    cls,
                                      float *pos,
                                      void  *mat,
                                      int    a8,
                                      int    a9,
                                      float *v10,
                                      float *v11,
                                      float  scale,
                                      float *v12) __asm__("FUN_L00_00263fd8");
extern void  update_moby_glow_and_queue_draw(char *moby) __asm__("FUN_L05_002d7020");
extern void  func_L05_002D8B68_2D8388(void) __asm__("FUN_L05_002d7920");
extern float D_L05_001672C8_2D8388[] __asm__("D_L05_001671C8");

void         scenery_ship_update(char *moby) __asm__("FUN_L05_002d7140");
extern float fast_vec_dist(void *, void *);

/* Moby class 79 */
void scenery_ship_update(char *moby) {
    enum {
        SCENERY_SHIP_INITIALIZE = 0,
        SCENERY_SHIP_FOLLOW_PATH = 1,
        SCENERY_SHIP_SPAWN_EFFECTS_AND_REMOVE = 2
    };
    Data_2D8388 *scenery_ship_instance_vars = ((Data_2D8388 *)MOBY(moby)->pvars);
    char        *hit;
    char        *path;
    char        *current;
    char        *previous_path_node_base;
    char        *next_path_node_base;
    char        *point;
    int          i;
    int          k;
    int          lifetime;
    int          damaged;
    Buf          left_offset;
    Buf          tail_offset;
    Buf          right_offset;
    float        in_angle;
    float        out_angle;
    float        turn;
    float        max_turn;
    float        turn_scale;
    float       *effect_origin_position, *effect_origin_rotation, *tab;

    if (D_L05_001672C8_2D8388[0] < 55.0f)
        return;
    hit = find_moby_hit(moby, 0x10000, 0);
    damaged = 0;
    switch (MOBY(moby)->state) {
    case SCENERY_SHIP_INITIALIZE: {
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
                            scenery_ship_initialization_scale_multiplier_gp;
        scenery_ship_instance_vars->f1C = random_angle_radians();
        scenery_ship_instance_vars->f328 = -1;
        scenery_ship_instance_vars->f368 = random_angle_radians();
        path = ((char *)l05_paths[scenery_ship_instance_vars->path]);
        if (*(float *)(path + 0x1C) == -1.0f) {
            max_turn = 0.0f;
            point = path + ((*(int *)path - 1) << 4);
            in_angle = fast_arc_tan(*(float *)(path + 0x10) - *(float *)(point + 0x10),
                                    *(float *)(path + 0x14) - *(float *)(point + 0x14));
            out_angle = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                     *(float *)(path + 0x24) - *(float *)(path + 0x14));
            *(float *)(path + 0x1C) = fast_subtract_rotations(out_angle, in_angle);
            for (i = 1; i < *(int *)path - 1; i++) {
                current = path + i * 16;
                previous_path_node_base = path + (i - 1) * 16;
                in_angle = fast_arc_tan(
                    *(float *)(current + 0x10) - *(float *)(previous_path_node_base + 0x10),
                    *(float *)(current + 0x14) - *(float *)(previous_path_node_base + 0x14));
                next_path_node_base = path + (i + 1) * 16;
                out_angle = fast_arc_tan(
                    *(float *)(next_path_node_base + 0x10) - *(float *)(current + 0x10),
                    *(float *)(next_path_node_base + 0x14) - *(float *)(current + 0x14));
                turn = fast_subtract_rotations(out_angle, in_angle);
                *(float *)(current + 0x1C) = turn;
                if (max_turn < func_001F9B88_2D8388(turn)) {
                    max_turn = func_001F9B88_2D8388(*(float *)(current + 0x1C));
                }
            }
            in_angle = fast_arc_tan(*(float *)(path + ((*(int *)path - 1) << 4) + 0x10) -
                                        *(float *)(path + ((*(int *)path - 2) << 4) + 0x10),
                                    *(float *)(path + ((*(int *)path - 1) << 4) + 0x14) -
                                        *(float *)(path + ((*(int *)path - 2) << 4) + 0x14));
            out_angle = fast_arc_tan(
                *(float *)(path + 0x10) - *(float *)(path + ((*(int *)path - 1) << 4) + 0x10),
                *(float *)(path + 0x14) - *(float *)(path + ((*(int *)path - 1) << 4) + 0x14));
            *(float *)(path + ((*(int *)path - 1) << 4) + 0x1C) =
                fast_subtract_rotations(out_angle, in_angle);
            turn_scale = 1.04719755f / max_turn;
            for (k = 0; k < *(int *)path; k++) {
                point = path + (k << 4);
                *(float *)(point + 0x1C) = *(float *)(point + 0x1C) * turn_scale;
            }
        }
        scenery_ship_instance_vars->fC =
            fast_vec_dist((float *)(path + 0x10), (float *)(path + 0x20));
        MOBY(moby)->state = 1;
        scenery_ship_instance_vars->f8 = (float)*(int *)path * scenery_ship_instance_vars->f4;
        break;
    }
    case SCENERY_SHIP_FOLLOW_PATH: {
        float phase =
            fast_add_rotations(scenery_ship_instance_vars->f1C, D_0015ED6C * 12.566370614f);
        float wave;
        scenery_ship_instance_vars->f1C = phase;
        wave = func_001F9FA8_2D8388(phase);
        MOBY(moby)->glow_color =
            func_001FA8A8_2D8388(D_L05_001613D8, D_L05_001613DC, (wave + 1.0f) * 0.5f);
        advance_scenery_ship_along_path(moby, (int *)l05_paths[scenery_ship_instance_vars->path]);
        if (func_L00_0028EB98_2D8388(moby, scenery_ship_instance_vars->f328) == 0) {
            scenery_ship_instance_vars->f328 = allocate_voice_for_target_entry(1, 4, (int)moby);
        }
        if ((frame_number & 3) == 0) {
            scenery_ship_instance_vars->f322 = (scenery_ship_instance_vars->f322 + 1) & 0xF;
            if ((float)scenery_ship_instance_vars->f320 < D_L05_001613E0) {
                scenery_ship_instance_vars->f320++;
            }
        }
        left_offset.q = 0;
        left_offset.f[0] = -1.2f;
        left_offset.f[1] = 0.4f;
        left_offset.f[2] = 0.45f;
        transform_vector_by_basis(&left_offset, &left_offset, moby + 0xC0);
        func_001F9BD8_2D8388(
            (float *)&scenery_ship_instance_vars->trail[0][(short)scenery_ship_instance_vars->f322],
            left_offset.f,
            (float *)&MOBY(moby)->pos);
        scenery_ship_instance_vars->trail[0][(short)scenery_ship_instance_vars->f322].w = 1.0f;
        tail_offset.q = 0;
        tail_offset.f[0] = -1.5f;
        tail_offset.f[2] = -0.3f;
        transform_vector_by_basis(&tail_offset, &tail_offset, moby + 0xC0);
        func_001F9BD8_2D8388(
            (float *)&scenery_ship_instance_vars->trail[1][(short)scenery_ship_instance_vars->f322],
            tail_offset.f,
            (float *)&MOBY(moby)->pos);
        scenery_ship_instance_vars->trail[1][(short)scenery_ship_instance_vars->f322].w = 1.0f;
        right_offset.q = 0;
        right_offset.f[0] = -1.2f;
        right_offset.f[1] = -0.4f;
        right_offset.f[2] = 0.45f;
        transform_vector_by_basis(&right_offset, &right_offset, moby + 0xC0);
        func_001F9BD8_2D8388(
            (float *)&scenery_ship_instance_vars->trail[2][(short)scenery_ship_instance_vars->f322],
            right_offset.f,
            (float *)&MOBY(moby)->pos);
        scenery_ship_instance_vars->trail[2][(short)scenery_ship_instance_vars->f322].w = 1.0f;
        if (frame_number != *scenery_ship_instance_vars->f324) {
            *scenery_ship_instance_vars->f324 = frame_number;
            add_draw_callback(func_L05_002D8B68_2D8388, moby);
        }
        if (MOBY(moby)->pos.x < 8.0f) {
            MOBY(moby)->collision = 0;
        } else if (MOBY(moby)->pos.y < 8.0f) {
            MOBY(moby)->collision = 0;
        } else {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        }
        if (hit != 0 && 0.0f < *(float *)(hit + 0x2C))
            damaged = 1;
        if (damaged)
            MOBY(moby)->state = 2;
        break;
    }
    case SCENERY_SHIP_SPAWN_EFFECTS_AND_REMOVE: {
        allocate_voice_for_target_entry(0, 0, (int)moby);
        tab = (float *)D_L05_0015F580_macro;
        effect_origin_position = (float *)&MOBY(moby)->pos;
        effect_origin_rotation = (float *)&MOBY(moby)->rot;
        fast_vec_scale(
            &left_offset, moby + 0xC0, scenery_ship_removal_effect_basis_scale_gp * frame_scale);
        left_offset.f[2] = left_offset.f[2] + scenery_ship_removal_effect_z_addend_gp * frame_scale;
        lifetime = func_001F9850_2D8388(0x5A);
        func_L00_00265050_2D8388(moby,
                                 0x72E,
                                 effect_origin_position,
                                 effect_origin_rotation,
                                 lifetime,
                                 0,
                                 left_offset.f,
                                 tab,
                                 D_0015ED70 * 12.0f,
                                 tab);
        lifetime = func_001F9850_2D8388(0x5A);
        func_L00_00265050_2D8388(moby,
                                 0x72E,
                                 effect_origin_position,
                                 effect_origin_rotation,
                                 lifetime,
                                 0,
                                 left_offset.f,
                                 tab,
                                 D_0015ED70 * 12.0f,
                                 tab);
        lifetime = func_001F9850_2D8388(0x5A);
        func_L00_00265050_2D8388(moby,
                                 0x72E,
                                 effect_origin_position,
                                 effect_origin_rotation,
                                 lifetime,
                                 0,
                                 left_offset.f,
                                 tab,
                                 D_0015ED70 * 12.0f,
                                 tab);
        mark_moby_for_removal(moby);
        return;
    }
    }
    if (D_L05_001613E4)
        update_moby_glow_and_queue_draw(moby);
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D8AD8), where it is exact; names translated to the US level program. */

extern void sample_camera_path(int *, int, void *, float *, int, float) __asm__("FUN_00214e58");

extern float scenery_ship_path_advance_scale_gp __asm__("D_L05_001613D0") __attribute__((sda));
void         advance_scenery_ship_along_path(char *moby, int *path) __asm__("FUN_L05_002d7890");

void advance_scenery_ship_along_path(char *moby, int *path) {
    float *moby_data = ((float *)MOBY(moby)->pvars);
    moby_data[2] = moby_data[2] + scenery_ship_path_advance_scale_gp * frame_time / moby_data[3];
    if ((float)path[0] < moby_data[2]) {
        moby_data[2] = moby_data[2] - (float)path[0];
    }
    sample_camera_path(path, 1, moby + 0x10, (float *)&MOBY(moby)->rot, 0, moby_data[2]);
    MOBY(moby)->rot.x = MOBY(moby)->rot.w;
}

extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");

/* Retail quad packet: vertices +0x00, colors +0x40, UVs +0x50, commands +0x70. */
typedef struct {
    char  vertices[0x40];
    int   color[4];
    float uv[4][2];
    u64   zero;
    u64   effect_texture;
    u64   texture;
    u64   flags;
} L05GlowPacket;

int                  D_L05_001613F0 = 0;
int                  D_L05_001613F4 = 0;
int                  D_L05_001613F8 = 0;
int                  D_L05_001613FC = 2;
int                  D_L05_00161400 = 0;
int                  D_L05_00161404 = 1;
int                  D_L05_00161408 = 0x40806060;
int                  D_L05_0016140C = 0x802020;
extern unsigned char l05_scenery_ship_extended_trail_selector __asm__("D_L05_0015EDB2");
extern struct Moby  *D_L05_0015FFD8 __asm__("D_L05_0015FFD8");
extern short        *D_L05_001ABCC0_7920[] __asm__("D_L05_001ABCC0");

float D_L05_001CDD70[4][2] = {
    {0.0f, 0.5f},
    {1.0f, 0.5f},
    {0.0f, 0.5f},
    {1.0f, 0.5f},
};

extern u64          get_effect_texture(int) __asm__("FUN_001f44b8");
extern int          interpolate_color_7920(float, int, int) __asm__("FUN_001fa6e0");
extern void         draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
extern void         memset(void *, int, int) __asm__("FUN_001153fc");
typedef struct Moby Moby;

/* Builds 16 fading glow quads, then draws three ribbons of quads for each linked moby of class 0x4F in view. */
void draw_group_scenery_ship_trails(struct Moby *moby) {
    L05GlowPacket  packet[16];
    float          matrix[8];
    float          center[4];
    int            i;
    int            j;
    short         *list;
    struct Moby   *member;
    char          *scenery_ship_vars;
    float          phase;
    float          ratio;
    u64            texture;
    L05GlowPacket *glow_packet;

    if (D_L05_001613F0 != 0)
        return;
    if (l05_scenery_ship_extended_trail_selector != 0)
        D_L05_001613E0 = 16.0f;
    else
        D_L05_001613E0 = 5.0f;
    for (i = 0; i < 16; i++) {
        texture = get_effect_texture(0x13);
        glow_packet = &packet[i];
        glow_packet->effect_texture = texture;
        glow_packet->texture = 0xff9000000260ULL;
        glow_packet->zero = 0;
        glow_packet->flags = (u64)D_L05_001613F8 | ((u64)D_L05_001613FC << 2) |
                             ((u64)D_L05_00161400 << 4) | ((u64)D_L05_00161404 << 6) |
                             0x8000000000ULL;
        phase = convert_integer_to_float((frame_number + 3) & 3) / (D_L05_001613E0 * 4.0f);
        for (j = 0; j < 4; j++) {
            packet[i].uv[j][0] = D_L05_001CDD70[j][0];
            packet[i].uv[j][1] = D_L05_001CDD70[j][1];
            ratio = convert_integer_to_float(i + 1 - (j >> 1)) / (D_L05_001613E0 + phase);
            if (ratio > 1.0f)
                ratio = 1.0f;
            else if (ratio < 0.0f)
                ratio = 0.0f;
            packet[i].color[j] = interpolate_color_7920(ratio, D_L05_00161408, D_L05_0016140C);
        }
    }
    memset(matrix, 0, 0x20);
    matrix[2] = 0.15f;
    matrix[6] = -0.15f;
    list = D_L05_001ABCC0_7920[(unsigned char)((char)moby->group)];
    do {
        member = (struct Moby *)(((*list & 0x7fff) << 8) + (int)D_L05_0015FFD8);
        if (member->oclass == 0x4f && member->state == 1) {
            scenery_ship_vars = (char *)member->pvars;
            qcopy_nc(center, &member->pos);
            center[3] = 20.0f;
            if (fast_bsphere_check(center, 512.0f) != -1) {
                int   trail_segment_start_index;
                int   ribbon;
                int   k;
                char *points;
                int   index;
                int   frame;
                for (frame = 0; frame < *(short *)(scenery_ship_vars + 0x320); frame++) {
                    trail_segment_start_index =
                        (*(short *)(scenery_ship_vars + 0x322) - frame + 15) & 15;
                    for (ribbon = 0; ribbon < 3; ribbon++) {
                        if (D_L05_001613F4 == 0) {
                            points = scenery_ship_vars + (ribbon * 0x100 + 0x20);
                            for (k = 0; k < 4; k++) {
                                index = (trail_segment_start_index + (k >> 1)) & 15;
                                fast_vec_add(packet[frame].vertices + k * 0x10,
                                             points + (index << 4),
                                             (char *)matrix + (k & 1) * 0x10);
                            }
                            draw_geometry_quad(&packet[frame], 0, 0);
                        }
                    }
                }
            }
        }
    } while (*list++ >= 0);
}
