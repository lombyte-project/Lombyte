/* Level 06 moby class 0x463: switchable beam barrier; turns on after its linked barrier and bursts when its switch moby fires. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"

extern float D_L06_00167540[];

extern char *D_L06_0015FFD8 __asm__("D_L06_0015FFD8");

extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00305BF8), where it is exact; names translated to the US level program. */
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

/* Level 06 update for moby class 1123: waits for its partner, animates four ribbons, then bursts into particles. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00305E38), where it is exact; names translated to the US level program. */
typedef struct {
    float a[4][30][4];
    float b[4][30][4];
    int   timer[4];
    int   f10;
    int   f14;
    int   f18;
} Data_305E38;

extern float D_0015ED6C;

#include "rnc/overlay/l06/mobys/switchable_beam_barrier_data/update_switchable_beam_barrier_3.h"

extern float D_L06_001FE890[];
extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern int   D_L06_00162234;
extern int   D_L06_00162238;
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern float D_L06_001621F8 __attribute__((sda));

#include "rnc/overlay/l06/mobys/switchable_beam_barrier_data/update_switchable_beam_barrier_2.h"

extern float D_L06_00162208 __attribute__((sda));

#include "rnc/overlay/l06/mobys/switchable_beam_barrier_data/switchable_beam_barrier_update.h"

extern void FUN_L00_00257d78(void *, float, float);
extern void FUN_L06_00217300(void *);
extern void add_draw_callback(int, char *) __asm__("FUN_001f4600");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
void        test_moby_vector_stream_collision_lines(char *m) __asm__("FUN_L06_00305000");
void        draw_moby_vector_streams(char *m) __asm__("FUN_L06_00305170");
extern void
     func_L00_00273F80_r(void *, void *, int, float, int, int, int) __asm__("FUN_L00_002730e0");
void switchable_beam_barrier_update(char *moby) __asm__("FUN_L06_003049f8");

/* Moby class 1123 */
void switchable_beam_barrier_update(char *moby) {
    enum {
        INITIALIZE_VECTOR_STREAMS = 0,
        WAIT_FOR_LINKED_MOBY = 1,
        UPDATE_VECTOR_STREAMS = 2,
        EMIT_PARTICLE_BURST = 3
    };
    Data_305E38 *vector_stream_vars = ((Data_305E38 *)MOBY(moby)->pvars);
    union {
        float tmp[30][4];
        struct {
            float a[16];
            float b[16];
            float pos[4];
            float vel[4];
        } m;
    } u;

    switch ((unsigned char)moby[0x20]) {
    case INITIALIZE_VECTOR_STREAMS:
        MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
                            vector_stream_moby_scale_multiplier_gp;
        D_L06_00162238 = 0x16;
        D_L06_00162234 = 0x2D;
        vector_stream_vars->timer[1] = 0xB;
        vector_stream_vars->timer[2] = 0x17;
        vector_stream_vars->timer[3] = 0x22;
        vector_stream_vars->timer[0] = 0;
        vector_stream_vars->f18 = -1;
        if (vector_stream_vars->f10 != -1) {
            moby[0x20] = 1;
            MOBY(moby)->collision = 0;
        } else {
            moby[0x20] = 2;
        }
        break;
    case WAIT_FOR_LINKED_MOBY: {
        char *e = D_L06_0015FFD8 + (vector_stream_vars->f10 << 8);
        if (*(short *)(e + 0xA6) == 0x463 && (unsigned char)e[0x20] < 3) {
            return;
        }
        moby[0x20] = 2;
        MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        break;
    }
    case UPDATE_VECTOR_STREAMS: {
        float stream_point_x;
        float stream_point_x_step;
        float unwrapped_stream_phase;
        int   i;
        int   j;
        int   k;
        int   stream_point_index;

        if (voice_is_playing(moby, vector_stream_vars->f18) == 0) {
            vector_stream_vars->f18 = allocate_voice_for_target_entry(0, 4, moby);
        }
        test_moby_vector_stream_collision_lines(moby);
        unwrapped_stream_phase = D_L06_001621F8 + vector_stream_phase_rate_gp * D_0015ED6C;
        D_L06_001621F8 = unwrapped_stream_phase;
        if (1.0f < unwrapped_stream_phase) {
            D_L06_001621F8 = unwrapped_stream_phase - 1.0f;
        }
        stream_point_x_step = vector_stream_x_extent_gp / 30.0f;
        stream_point_x = -(vector_stream_x_extent_gp * 0.5f);
        for (i = 0; i < 4; i++) {
            if (vector_stream_vars->timer[i] == 0) {
                for (j = 0; j < 30; j++) {
                    FUN_L06_00217300(vector_stream_vars->a[i][j]);
                    vector_stream_vars->a[i][j][0] = stream_point_x;
                    FUN_L00_00257d78(
                        u.tmp[j], 0.0f, vector_stream_increment_random_scale_bound_gp * D_0015ED6C);
                    stream_point_x += stream_point_x_step;
                }
                for (k = 1; k < 29; k++) {
                    fast_vec_add(vector_stream_vars->b[i][k], u.tmp[k - 1], u.tmp[k]);
                    fast_vec_add(
                        vector_stream_vars->b[i][k], vector_stream_vars->b[i][k], u.tmp[k + 1]);
                    fast_vec_scale(
                        vector_stream_vars->b[i][k], vector_stream_vars->b[i][k], 0.333f);
                }
                vector_stream_vars->timer[i] = D_L06_00162234;
            } else {
                for (stream_point_index = 1; stream_point_index < 29; stream_point_index++) {
                    fast_vec_add(vector_stream_vars->a[i][stream_point_index],
                                 vector_stream_vars->a[i][stream_point_index],
                                 vector_stream_vars->b[i][stream_point_index]);
                }
                vector_stream_vars->timer[i]--;
            }
        }
        if (fast_vec_dist(moby + 0x10, ((char *)D_L06_00167540)) < 48.0f) {
            add_draw_callback((int)draw_moby_vector_streams, moby);
        }
        if (vector_stream_vars->f14 >= 0 &&
            *(unsigned char *)(D_L06_0015FFD8 + 0xBC + (vector_stream_vars->f14 << 8)) != 0) {
            int voice_slot_index;
            MOBY(moby)->collision = 0;
            moby[0x20] = 3;
            voice_slot_index = vector_stream_vars->f18;
            if (voice_slot_index != -1) {
                char *e = ((char *)&voice_pool) + voice_slot_index * 0x70;
                if (*(char **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(voice_slot_index);
                }
            }
            vector_stream_vars->f18 = -1;
            allocate_voice_for_target_entry(1, 0, moby);
        }
        break;
    }
    case EMIT_PARTICLE_BURST: {
        int i;
        int j;

        euler_rotation_matrix(u.m.a, (float *)&MOBY(moby)->rot);
        set_scale_matrix(u.m.b, D_L06_00162208);
        multiply_matrix(u.m.a, u.m.a, u.m.b);
        for (i = 0; i < 4; i++) {
            euler_rotation_matrix(u.m.b, &D_L06_001FE890[i * 4]);
            multiply_matrix(u.m.b, u.m.a, u.m.b);
            fast_vec_add(u.m.b + 12, u.m.b + 12, moby + 0x10);
            u.m.b[14] += (float)i * 1.7f + 0.6f;
            u.m.b[15] = 1.0f;
            for (j = 0; j < 50; j++) {
                int           color;
                float         scale;
                int           particle_lifetime_frames;
                unsigned char life;
                fast_vec_normalize(
                    u.m.pos, u.m.b, randf_sym(0.0f, vector_stream_x_extent_gp * 0.75f));
                fast_vec_add(u.m.pos, u.m.pos, u.m.b + 12);
                color = fast_tween_color(vector_stream_burst_color_blend_a_gp,
                                         vector_stream_burst_color_blend_b_gp,
                                         random_float_between(0.0f, 1.0f));
                scale = random_float_between(vector_stream_burst_min_particle_scale_gp,
                                             vector_stream_burst_max_particle_scale_gp);
                particle_lifetime_frames = truncate_float_to_s32(
                    random_float_between((float)vector_stream_burst_min_lifetime_frames_gp,
                                         (float)vector_stream_burst_max_lifetime_frames_gp));
                FUN_L00_00257d78(u.m.vel,
                                 vector_stream_burst_velocity_random_scale_bound_a_gp * D_0015ED6C,
                                 vector_stream_burst_velocity_random_scale_bound_b_gp * D_0015ED6C);
                life = scale_game_frames(particle_lifetime_frames);
                func_L00_00273F80_r(u.m.pos,
                                    u.m.vel,
                                    color,
                                    scale,
                                    life,
                                    (unsigned char)random_integer_below(0xFF),
                                    0);
            }
        }
        moby[0x20] = 4;
        break;
    }
    }
}

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00306440), where it is exact; names translated to the US level program. */
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void  initialize_hit_descriptor(char *a, int b, int c, float d) __asm__("FUN_L00_00259858");
extern float vector_stream_collision_local_x_offset_gp __asm__("D_L06_001621E8")
    __attribute__((sda));
extern float vector_stream_collision_local_y_offset_gp __asm__("D_L06_001621EC")
    __attribute__((sda));
extern float vector_stream_collision_local_z_offset_gp __asm__("D_L06_001621F0")
    __attribute__((sda));

void test_moby_vector_stream_collision_lines(char *moby) {
    float P[4];
    float Q[4];
    float A[12];
    initialize_hit_descriptor((char *)A, (int)moby, 0x10000, 6.0f);
    P[0] = vector_stream_collision_local_x_offset_gp;
    P[1] = vector_stream_collision_local_y_offset_gp;
    P[2] = vector_stream_collision_local_z_offset_gp;
    Q[0] = -vector_stream_collision_local_x_offset_gp;
    Q[1] = vector_stream_collision_local_y_offset_gp;
    Q[2] = vector_stream_collision_local_z_offset_gp;
    transform_vector_by_basis(P, P, moby + 0xC0);
    fast_vec_add(P, P, moby + 0x10);
    transform_vector_by_basis(Q, Q, moby + 0xC0);
    fast_vec_add(Q, Q, moby + 0x10);
    collision_line(P, Q, 0, (int)moby, (int)A);
    P[0] = vector_stream_collision_local_x_offset_gp;
    P[1] = -vector_stream_collision_local_y_offset_gp;
    P[2] = vector_stream_collision_local_z_offset_gp;
    Q[0] = -vector_stream_collision_local_x_offset_gp;
    Q[1] = -vector_stream_collision_local_y_offset_gp;
    Q[2] = vector_stream_collision_local_z_offset_gp;
    transform_vector_by_basis(P, P, moby + 0xC0);
    fast_vec_add(P, P, moby + 0x10);
    transform_vector_by_basis(Q, Q, moby + 0xC0);
    fast_vec_add(Q, Q, moby + 0x10);
    collision_line(P, Q, 0, (int)moby, (int)A);
}

/* draws four copies of a model transformed by a table, then spawns a part for each */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_003065B0), where it is exact; names translated to the US level program. */
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern int   vector_stream_blend_color_b_selector_gp __asm__("D_L06_001621C4") __attribute__((sda));
extern int   vector_stream_blend_alpha_c_selector_gp __asm__("D_L06_001621C8") __attribute__((sda));
extern int   vector_stream_blend_color_a_selector_gp __asm__("D_L06_001621C0") __attribute__((sda));
extern int   vector_stream_blend_color_d_selector_gp __asm__("D_L06_001621CC") __attribute__((sda));
extern int   vector_stream_timer_threshold_color_gp __asm__("D_L06_001621E0") __attribute__((sda));
extern int   vector_stream_cycle_boundary_color_gp __asm__("D_L06_001621E4") __attribute__((sda));
extern float vector_stream_texture_u_step_gp __asm__("D_L06_001621F4") __attribute__((sda));
extern f32   D_L06_001621F8_gp __asm__("D_L06_001621F8") __attribute__((sda));
extern f32   D_L06_00162208_gp __asm__("D_L06_00162208") __attribute__((sda));
extern void  FUN_L06_00216b88(int *, float *);
extern void  font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void  vu1_add_g_sregister(int, long) __asm__("FUN_00233980");

void draw_moby_vector_streams(char *moby) {
    float  rotation_scale_matrix[16];
    float  scale_matrix[16];
    float  copy_transform_matrices[5][16];
    int    shared_helper_arguments[10];
    char  *vector_stream_storage;
    float *transform_matrix_base;
    int   *sp;
    float *pp;
    float *tp;
    float *mp;
    int    i;
    int    j;
    vector_stream_storage = ((char *)MOBY(moby)->pvars);
    euler_rotation_matrix(rotation_scale_matrix, (float *)&MOBY(moby)->rot);
    set_scale_matrix(scale_matrix, *(float *)&D_L06_00162208_gp);
    multiply_matrix(rotation_scale_matrix, rotation_scale_matrix, scale_matrix);
    vu1_add_g_sregister(6, get_effect_texture(0xE));
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x42,
                        (long)vector_stream_blend_color_a_selector_gp |
                            ((long)vector_stream_blend_color_b_selector_gp << 2) |
                            ((long)vector_stream_blend_alpha_c_selector_gp << 4) |
                            ((long)vector_stream_blend_color_d_selector_gp << 6) | 0x8000000000L);
    font_queue_vu_state();
    transform_matrix_base = copy_transform_matrices[0];
    sp = shared_helper_arguments;
    pp = copy_transform_matrices[0] + 12;
    tp = D_L06_001FE890;
    mp = transform_matrix_base;
    for (i = 0; i < 4; i++) {
        euler_rotation_matrix(mp, tp);
        tp += 4;
        multiply_matrix(mp, rotation_scale_matrix, mp);
        mp += 16;
        fast_vec_add(pp, pp, moby + 0x10);
        pp[2] += (float)i * 1.7f + 0.6f;
        pp[3] = 1.0f;
        pp += 16;
    }
    shared_helper_arguments[0] = D_L06_00162234;
    shared_helper_arguments[2] = (int)(vector_stream_storage + 0xF00);
    shared_helper_arguments[1] = D_L06_00162238;
    shared_helper_arguments[3] = vector_stream_timer_threshold_color_gp;
    shared_helper_arguments[4] = vector_stream_cycle_boundary_color_gp;
    *(float *)&shared_helper_arguments[7] = vector_stream_texture_u_step_gp;
    *(float *)&shared_helper_arguments[8] = *(float *)&D_L06_001621F8_gp;
    shared_helper_arguments[5] = 0x1E;
    shared_helper_arguments[6] = (int)vector_stream_storage;
    pp = transform_matrix_base;
    for (j = 3; j >= 0; j--) {
        FUN_L06_00216b88(sp, pp);
        pp += 16;
    }
}

int D_L06_00162234 = 0;

int D_L06_00162238 = 0;

f32 D_L06_001621F8_gp __asm__("D_L06_001621F8") = 0;
