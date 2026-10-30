/* Shared moby class mine: the mine the mine glove throws, its spawner and motion start, and the landing prediction and impact marker drawn while aiming. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/overlay/shared/rendering/effects/explosion_light.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/collision.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/shared/weapons/mine.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/world/collision/collision_hit_material.h"

typedef float V910[4] __attribute__((aligned(16)));

typedef struct {
    char  p0[0x160];
    float f160;
    char  p164[4];
    int   i168;
} Cam910;

extern char *mine_debris(char *, char *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float step_rotation_toward_target(float, float, float) __asm__("FUN_L00_00200510");
extern float FUN_L00_00259710(void *);
extern float fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern int   collision_hit_material(void) __asm__("FUN_001f0b58");
extern int   FUN_L00_001f0d60(float, void *, int, void *);
extern int collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern int mine_blend_color(int, int, float) __asm__("FUN_L00_002371e0");
extern int FUN_L00_00257b90(int, int);
extern int FUN_L00_00258490(void *, void *, void *, float, float, float, float, int);
extern int FUN_L00_00260400(void *, void *, void *, void *, void *, void *);
extern int random_integer_below_cf(void *, void *, float, float) __asm__("FUN_L00_0026f080");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern s32 is_moby_in_class_range_500_to_540(s32) __asm__("FUN_L00_0025e3b8");
extern unsigned       swap_packed_color_channels(unsigned c, int mask) __asm__("FUN_L00_0025c0e8");
extern unsigned char *FUN_002141f8_cf(unsigned char *) __asm__("FUN_L00_002603d0");
extern void          *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void          *spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
             void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");
extern void jet_animate(unsigned char *, float) __asm__("FUN_L00_00259fe8");
extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern void spawn_debris_chunk(void *, void *, int, int) __asm__("FUN_L00_0026f548");
extern void spawn_gravity_floor_burst_particle(
    void *, void *, int, float, float, float, int, int, int) __asm__("FUN_L00_002738e8");
extern void pool_splash(int, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void copy_matrix3x4(void *, void *);
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_sub(void *dst, void *a, void *b) __asm__("FUN_001f9a28");
f32   resolve_camera_surface_height(void *position, s32 optional_output) __asm__("FUN_002135f0");
float absolute_float(float input) __asm__("func_001F99C0");

extern B910           l00_mine_camera_blast_parameters __asm__("D_L00_001E9830");
extern B910           l00_mine_alternate_camera_blast_parameters __asm__("D_L00_001E9880");
extern char          *active_mobies[] __asm__("D_L00_001ABA00");
extern s16            D_L00_001B07B0[];
extern int           *D_L00_00177F00[];
extern s32            D_L00_00166DC0[];
extern s32            D_L00_001612D0_gp __asm__("D_L00_001612D0") __attribute__((sda));
extern s32            D_L00_001612D8_gp __asm__("D_L00_001612D8") __attribute__((sda));
extern short          mine_force_burst __asm__("D_L00_001613D0") __attribute__((sda));
extern short          mine_bounce_ramp_duration __asm__("D_L00_001613D4") __attribute__((sda));
extern unsigned char *D_L00_001B2080[];
extern int            D_L00_0015F5CC;
extern E910           D_E910 __asm__("D_0013E520");
extern H910           coll_hit __asm__("D_L00_00173E40");
extern char          *active_mobies[] __asm__("D_L00_001ABA00") NOT_SDA;
extern s32            D_L00_00166DC0[] NOT_SDA;

short                         D_L01_001613D0 = 0;
short                         D_L01_001613D4 = 0;
s32                           D_L02_001612D0 = 0;
s32                           D_L02_001612D8 = 0;
short                         D_L02_001613A0 = 0;
short                         D_L02_001613A4 = 0;
short                         D_L03_001613B0 = 0;
short                         D_L03_001613B4 = 0;
unsigned char *D_L03_001B2180 NOT_SDA = 0;
s32                           D_L04_001612D0 = 0;
s32                           D_L04_001612D8 = 0;
short                         D_L04_001613A0 = 0;
short                         D_L04_001613A4 = 0;
unsigned char *D_L04_001B2200 NOT_SDA = 0;
short                         D_L05_001613B0 = 0;
short                         D_L05_001613B4 = 0;
unsigned char *D_L05_001B2500 NOT_SDA = 0;
s32                           D_L06_001612D0 = 0;
s32                           D_L06_001612D8 = 0;
short                         D_L06_001613A0 = 0;
short                         D_L06_001613A4 = 0;
unsigned char *D_L06_001B2800 NOT_SDA = 0;
short                         D_L07_001613A0 = 0;
short                         D_L07_001613A4 = 0;
unsigned char *D_L07_001B2100 NOT_SDA = 0;
short                         D_L08_001613A0 = 0;
short                         D_L08_001613A4 = 0;
s32                           D_L09_001612D0 = 0;
s32                           D_L09_001612D8 = 0;
short                         D_L09_00161430 = 0;
short                         D_L09_00161434 = 0;
unsigned char *D_L09_001B2200 NOT_SDA = 0;
s32                           D_L10_001612D0 = 0;
s32                           D_L10_001612D8 = 0;
short                         D_L10_001613B0 = 0;
short                         D_L10_001613B4 = 0;
unsigned char *D_L10_001B2500 NOT_SDA = 0;
short                         D_L11_001613A0 = 0;
short                         D_L11_001613A4 = 0;
short                         D_L12_001613B0 = 0;
short                         D_L12_001613B4 = 0;
unsigned char *D_L12_001B2500 NOT_SDA = 0;
short                         D_L13_00161470 = 0;
short                         D_L13_00161474 = 0;
s32                           D_L14_001612D0 = 0;
s32                           D_L14_001612D8 = 0;
short                         D_L14_001615B0 = 0;
short                         D_L14_001615B4 = 0;
unsigned char *D_L14_001B2780 NOT_SDA = 0;
short                         D_L15_00161490 = 0;
short                         D_L15_00161494 = 0;
unsigned char *D_L15_001B2680 NOT_SDA = 0;
short                         D_L16_00161460 = 0;
short                         D_L16_00161464 = 0;
unsigned char *D_L16_001B2500 NOT_SDA = 0;
short                         D_L17_00161430 = 0;
short                         D_L17_00161434 = 0;
short                         D_L18_00161440 = 0;
short                         D_L18_00161444 = 0;
unsigned char *D_L18_001B2A80 NOT_SDA = 0;
s16 D_L02_001B0DB0            NOT_SDA = 0;
s16 D_L05_001B0C30            NOT_SDA = 0;
s16 D_L06_001B0F30            NOT_SDA = 0;
s16 D_L07_001B0830            NOT_SDA = 0;
s16 D_L11_001B11B0            NOT_SDA = 0;
s16 D_L13_001B0AB0            NOT_SDA = 0;
s16 D_L14_001B0EB0            NOT_SDA = 0;
s16 D_L15_001B0DB0            NOT_SDA = 0;
s16 D_L17_001B10B0            NOT_SDA = 0;
s16 D_L18_001B11B0            NOT_SDA = 0;

typedef float V[4] __attribute__((aligned(16)));

typedef union {
    u128 q;
    f32  f[4];
} V_2a9c50;

typedef struct {
    V_2a9c50 corner[4];
    u32      color[4];
    f32      uv[8];
    s64      unk70;
    s64      tex;
    s64      unk80;
    s64      unk88;
} Q_2a9c50;

typedef struct {
    u8       pad0[0x10];
    V_2a9c50 pos;
    f32      f20, f24, f28;
} PV;

typedef struct {
    u8  pad0[0x78];
    PV *pv;
} M_2a9c50;

extern float scale_time(float) __asm__("FUN_001f96b0");
extern s64   get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

extern s32 D_L00_0015F5C4;

extern f32 scale_time(f32) __asm__("FUN_001f96b0");
extern s32 FUN_001efa68(void *, void *, s32, s32, s32);
extern int D_L00_00173E40_u[] __asm__("D_L00_00173E40");
extern int FUN_001efa68(void *, void *, int, int, int);

extern char D_0013F350[];

extern s32 D_L00_0015F5CC;

extern f32  spin_angles_x __asm__("D_L00_001613E0") __attribute__((sda));
extern f32  spin_angles[] __asm__("D_L00_001613E0") MACRO_ADDR;
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");

extern char *FUN_L00_002603d0_u(void *) __asm__("FUN_L00_002603d0");
extern int   D_L00_0015F5C4_c __asm__("D_L00_0015F5C4");
extern void  FUN_L00_002a9c50_c(void) __asm__("FUN_L00_002a9c50");

/* Spin angles of the second quad: x is read through $gp, the rest as an
   array through the unsplit address macro (from rac1-decomp's form). */

void draw_mine_predicted_impact_marker(M_2a9c50 *moby) __asm__("FUN_L00_002a9c50");

void draw_mine_predicted_impact_marker(M_2a9c50 *moby) {
    Q_2a9c50 quad;
    V_2a9c50 quad_transform[4];
    V_2a9c50 mz[4];
    V_2a9c50 direction_rotation_angles;
    PV      *pv = moby->pv;
    s32      i;
    s32      odd;
    f32      direction_inclination, spin_angle_numerator;
    if (D_L00_0015F5C4 == 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        odd = i & 1;
        direction_inclination =
            fast_arc_tan(square_root(pv->f20 * pv->f20 + pv->f28 * pv->f28), pv->f24);
        direction_rotation_angles.f[0] = fast_add_rotations(-direction_inclination, 1.57079637f);
        direction_rotation_angles.f[1] = fast_arc_tan(pv->f28, pv->f20);
        direction_rotation_angles.f[2] = 0.0f;
        direction_rotation_angles.f[3] = 0.0f;
        spin_angles_x = 0.0f;
        spin_angle_numerator = convert_integer_to_float(D_L00_0015F5CC % scale_game_frames(180));
        {
            f32 spin_period = scale_time(180.0f);
            f32 quad_spin_angle;
            spin_angle_numerator *= 6.28318548f;
            quad_spin_angle = fast_normalize_angle(spin_angle_numerator / spin_period);
            if (odd) {
                quad_spin_angle = -quad_spin_angle;
            }
            spin_angles[1] = quad_spin_angle;
        }
        spin_angles[2] = 0.0f;
        spin_angles[3] = 0.0f;
        euler_rotation_matrix(mz, &spin_angles_x);
        euler_rotation_matrix(quad_transform, &direction_rotation_angles);
        multiply_matrix(quad_transform, quad_transform, mz);
        qcopy(&quad_transform[3], &pv->pos);
        quad.color[0] = 0x80808080;
        quad.color[1] = 0x80808080;
        quad.color[2] = 0x80808080;
        quad.color[3] = 0x80808080;
        quad.tex = get_effect_texture(odd ? 0x12 : 0x11);
        quad.unk80 = 0xFF9000000260L;
        quad.unk88 = 0x8000000048L;
        quad.unk70 = 0;
        quad.corner[0].f[0] = -1.0f;
        quad.corner[0].f[1] = 0.0f;
        quad.corner[0].f[2] = 1.0f;
        quad.corner[0].f[3] = 1.0f;
        quad.corner[1].f[0] = 1.0f;
        quad.corner[1].f[1] = 0.0f;
        quad.corner[1].f[2] = 1.0f;
        quad.corner[1].f[3] = 1.0f;
        quad.corner[2].f[0] = -1.0f;
        quad.corner[2].f[1] = 0.0f;
        quad.corner[2].f[2] = -1.0f;
        quad.corner[2].f[3] = 1.0f;
        quad.corner[3].f[0] = 1.0f;
        quad.corner[3].f[1] = 0.0f;
        quad.corner[3].f[2] = -1.0f;
        quad.corner[3].f[3] = 1.0f;
        quad.uv[0] = 0.0f;
        quad.uv[1] = 1.0f;
        quad.uv[2] = 1.0f;
        quad.uv[3] = 1.0f;
        quad.uv[4] = 0.0f;
        quad.uv[5] = 0.0f;
        quad.uv[6] = 1.0f;
        quad.uv[7] = 0.0f;
        draw_geometry_quad(&quad, quad_transform, 1);
    }
}

struct Amb {
    float f0;
    float f4;
    float f8;
    short hC;
    short hE;
    float f10;
    float f14;
    float f18;
    short h1C;
    short h1E;
};

typedef struct {
    u8 pad0[0x454];
    u8 killed[1]; /* 0x454: indexed by the moby save id */
} MobyKillTable;

typedef struct {
    char p0[0x50];
    u8   b50;
    u8   b51;
    u8   b52;
    u8   b53;
    f32  f54;
} O_002a7c70;

typedef float WrenchVec[4] __attribute__((aligned(16)));

typedef union {
    u128 q;
    f32  f[4];
} V_2aa008;

typedef struct {
    u8  pad[0x5C];
    s32 i5C;
} D_2aa008;

typedef struct {
    u8  pad[0x34];
    s16 h34;
} M_2aa008;

typedef struct N_2aa008 {
    u8               pad[0x10];
    V_2aa008         v10;
    u8               b20;
    u8               pad2[0x28 - 0x21];
    struct N_2aa008 *p28;
    f32              f2C;
    u8               pad3[0x78 - 0x30];
    void            *p78;
    u8               pad4[0xA6 - 0x7C];
    s16              hA6;
} N_2aa008;

typedef struct {
    u8       pad[0x10];
    V_2aa008 v10;
} Q_2aa008;

extern float D_0015ED6C_2a9ed0 __asm__("D_0015ED6C");
extern int   D_L00_0015F5CC_2a9ed0 __asm__("D_L00_0015F5CC");

extern f32       D_0015ED6C_2aa008 __asm__("D_0015ED6C");
extern N_2aa008 *D_L00_0015FFE4_2aa008 __asm__("D_L00_0015FFE4");
extern V_2aa008  D_L00_00173E60_2aa008[] __asm__("D_L00_00173E60") NOT_SDA;
s32              FUN_L00_002603d0_2aa008(void *) __asm__("FUN_L00_002603d0");

unsigned char *spawn_mine(int owner, OvlQuad *pos) __asm__("FUN_L00_002a9ed0");

unsigned char *spawn_mine(int owner, OvlQuad *spawn_position) {
    unsigned char *mine_moby = create_moby(0x4A);
    if (mine_moby) {
        unsigned char *mine_vars = ((unsigned char *)MOBY(mine_moby)->pvars);
        mine_moby[0x30] = 0xFF;
        MOBY(mine_moby)->draw_dist = 0x7F;
        mine_moby[0x31] = 1;
        MOBY(mine_moby)->scale = MOBY(mine_moby)->scale * 0.5f;
        mine_moby[0x20] = 0;
        *(int *)(mine_vars + 0x30) = owner;
        *(short *)(mine_vars + 0x36) = 0;
        *(int *)(mine_vars + 0x78) = 0;
        qcopy(mine_moby + 0x10, spawn_position);
        clear_u64_value(mine_vars);
        {
            float a = random_float_between(D_0015ED6C_2a9ed0 * 1.0471976f,
                                           D_0015ED6C_2a9ed0 * 3.1415927f);
            float frame_rate_scale = D_0015ED6C_2a9ed0;
            float frame_scaled_random_sample;
            int   x;
            float z;
            *(float *)(mine_vars + 0x38) = a;
            frame_scaled_random_sample =
                random_float_between(frame_rate_scale * 1.0471976f, frame_rate_scale * 3.1415927f);
            x = D_L00_0015F5CC_2a9ed0;
            z = 0.0f;
            *(float *)(mine_vars + 0x3C) = frame_scaled_random_sample;
            *(float *)(mine_vars + 0x64) = z;
            *(float *)(mine_vars + 0x60) = z;
            *(float *)(mine_vars + 0x70) = z;
            *(float *)(mine_vars + 0x6C) = z;
            *(int *)(mine_vars + 0x5C) = x;
            *(int *)(mine_vars + 0x74) = 0;
            *(int *)(mine_vars + 0x68) = -1;
            MOBY(mine_moby)->collision = 0;
        }
        update_entity(mine_moby);
    }
    return mine_moby;
}

void initialize_moby_motion_at_position(V_2aa008 *requested_position,
                                        N_2aa008 *self,
                                        void     *base_motion_vector) __asm__("FUN_L00_002aa008");

void initialize_moby_motion_at_position(V_2aa008 *requested_position,
                                        N_2aa008 *self,

                                        void *base_motion_vector) {
    V_2aa008     random_offset;
    V_2aa008     vector_scratch;
    V_2aa008     collision_position_offset;
    M_2aa008    *moby_vars;
    struct Hero *P;
    N_2aa008    *scanned_moby;
    s32          count;
    N_2aa008    *best;
    s32          bestv;
    V_2aa008    *k;
    vector_scratch.q = 0;
    moby_vars = self->p78;
    vector_scratch.f[0] = random_float_between(-1.0f, 1.0f);
    vector_scratch.f[1] = random_float_between(-1.0f, 1.0f);
    vector_scratch.f[2] = random_float_between(-1.0f, 1.0f);
    self->b20 = 1;
    self->f2C = self->f2C / 0.65f;
    random_offset.q = vector_scratch.q;
    fast_vec_normalize(&random_offset,
                       &random_offset,
                       random_float_between(D_0015ED6C_2aa008 * 0.01f, D_0015ED6C_2aa008 * 0.1f));
    fast_vec_add(moby_vars, &random_offset, base_motion_vector);
    P = &hero;
    if (P->ground_moby && FUN_L00_002603d0_2aa008(P->ground_moby)) {
        fast_vec_add(moby_vars, moby_vars, &P->motion.displacement);
    }
    moby_vars->h34 = scale_game_frames(0x78);
    count = 0;
    best = 0;
    bestv = 0;
    for (scanned_moby = D_L00_0015FFE4_2aa008; scanned_moby; scanned_moby = scanned_moby->p28) {
        if (scanned_moby->b20 != 0xFE)
            if (scanned_moby->b20 != 0xFD)
                if (scanned_moby->hA6 == self->hA6)
                    if (scanned_moby != self) {
                        if (best == 0 || ((D_2aa008 *)scanned_moby->p78)->i5C < bestv) {
                            bestv = ((D_2aa008 *)scanned_moby->p78)->i5C;
                            best = scanned_moby;
                        }
                        count++;
                    }
    }
    if (count >= 8)
        ((D_2aa008 *)best->p78)->i5C = -1;
    qcopy(&self->v10, requested_position);
    vector_scratch.q = ((Q_2aa008 *)hero.moby)->v10.q;
    vector_scratch.f[2] = requested_position->f[2];
    if (collision_line(&vector_scratch, requested_position, 0, hero.moby, 0)) {
        k = D_L00_00173E60_2aa008;
        qcopy(&self->v10, &k[0]);
        fast_vec_normalize(&collision_position_offset, &k[2], frame_time_sq * 9.7f);
        fast_vec_add(&self->v10, &self->v10, &collision_position_offset);
        reflect_vector(moby_vars, moby_vars, &k[2]);
        fast_vec_scale(moby_vars, moby_vars, 0.6f);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AB548), where it is exact; names translated to the US level program. */

int predict_mine_impact(char *trajectory_state, char *moby, int mode) __asm__("FUN_L00_002aa2a8");

int predict_mine_impact(char *trajectory_state, char *moby, int mode) {
    float trajectory_position[4];
    float B[4];
    float C[4];
    float box[4];
    int   x;
    char *base = D_0013F350;
    int   i;
    char *owner_moby_vars;
    if (*(int *)(base + 0x2084) == 0x72)
        return 0;
    if (D_L00_0015F5C4_c != 0)
        return 0;
    qcopy(C, moby + 0x10);
    if (MOBY(moby)->state == 0) {
        trajectory_position[0] =
            fast_cos(fast_add_rotations(
                -0.36196801f, fast_arc_tan(*(float *)(base + 0x670), *(float *)(base + 0x674)))) *
            0.859039f;
        trajectory_position[1] =
            fast_sin(fast_add_rotations(
                -0.36196801f, fast_arc_tan(*(float *)(base + 0x670), *(float *)(base + 0x674)))) *
            0.859039f;
        *(int *)&trajectory_position[2] = 0;
        fast_vec_add(trajectory_position, trajectory_position, base + 0x80);
        trajectory_position[2] = trajectory_position[2] + 0.49082f;
        qcopy(B, trajectory_state);
        if (*(int *)(base + 0x2FC) != 0 &&
            FUN_L00_002603d0_u((void *)*(int *)(base + 0x2FC)) != 0 && mode != 0) {
            fast_vec_add(B, B, base + 0x100);
        }
        x = scale_game_frames(0x78);
    } else {
        owner_moby_vars = *(char **)(*(char **)(trajectory_state + 0x30) + 0x78);
        qcopy(trajectory_position, moby + 0x10);
        qcopy(B, trajectory_state);
        {
            short xs = *(short *)(trajectory_state + 0x34);
            int   owner_prediction_stop_signal = *(int *)(owner_moby_vars + 0x50);
            x = xs;
            if (owner_prediction_stop_signal != 0)
                *(short *)(trajectory_state + 0x36) = 1;
        }
    }
    {
        char *base2 = D_0013F350;
        char *hero_moby = *(char **)(base2 + 0x2080);
        *(u128 *)box = *(u128 *)(hero_moby + 0x10);
        box[2] = trajectory_position[2];
        if (FUN_001efa68(box, trajectory_position, 0x10, (int)hero_moby, 0) != 0) {
            if (collision_hit_material() != 0 && D_L00_00173E40_u[7] > 0) {
                qcopy(trajectory_state + 0x10, (char *)D_L00_00173E40_u + 0x20);
            }
        }
    }
    while (fast_dec_timer(&x) == 0) {
        qcopy(C, trajectory_position);
        i = 9;
        do {
            fast_vec_add(trajectory_position, trajectory_position, B);
            B[2] = B[2] - frame_time_sq * 9.8f;
        } while (--i >= 0);
        if (FUN_001efa68(C, trajectory_position, 0x12, (int)moby, 0) == 0)
            continue;
        if (collision_hit_material() == 0 && 0.0f < B[2])
            continue;
        if (D_L00_00173E40_u[7] > 0) {
            qcopy(trajectory_state + 0x10, (char *)D_L00_00173E40_u + 0x20);
            break;
        }
    }
    if (x != 0) {
        if (mode == 0) {
            if (coll_hit_moby == 0)
                return 0;
            if (FUN_L00_002603d0_u(coll_hit_moby) == 0)
                return 0;
        }
        if (mode != 2 || coll_hit_moby == 0 || FUN_L00_002603d0_u(coll_hit_moby) == 0) {
            add_draw_callback(FUN_L00_002a9c50_c, moby);
        }
        {
            char *camera_position = (char *)&D_L00_00166DC0;
            char *hit_position = trajectory_state + 0x10;
            *(u128 *)(trajectory_state + 0x20) = *(u128 *)&coll_hit_normal;
            fast_vec_sub(C, hit_position, camera_position);
            fast_vec_scale(C, C, 0.95f);
            fast_vec_add(hit_position, C, camera_position);
        }
        return 1;
    }
    return 0;
}

/* Mine (moby class 74) update: falls and bounces, sticks to what it hits, homes on a nearby target and explodes. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002AB910), where it is exact; names translated to the US level program. */
void mine_update(M910 *m) __asm__("FUN_L00_002aa670");

void mine_update(M910 *mine_moby) {
    enum {
        MINE_DEBRIS_FIXED_SECONDARY_COLOR = 0,
        MINE_DEBRIS_FIXED_LIFETIME = 1,
        MINE_DEBRIS_SHORT_LIVED = 2,
        MINE_DEBRIS_SPHERICAL_VELOCITY = 3
    };
    B910  camera_blast_parameters = l00_mine_camera_blast_parameters;
    B910  alternate_camera_blast_parameters = l00_mine_alternate_camera_blast_parameters;
    U910  collision_response_velocity;
    U910  old;
    U910  up;
    W910 *v;
    M910 *hd;
    int   mine_state_snapshot;

    hd = 0;
    old.q = 0;
    up.q = 0;
    old.f[2] = frame_time + frame_time;
    up.f[2] = 1.0f;
    collision_response_velocity.q = old.q;
    v = mine_moby->v;
    if ((hero.unk20A5 != 0 || hero.unk20AF != 0) && mine_moby->state == 0) {
        mine_moby->flags |= 0x41;
    } else {
        mine_moby->flags &= 0xFFBE;
    }
    if ((hero.items[0].moby == 0 || ((M910 *)hero.items[0].moby)->cls != 0xBE) &&
        mine_moby->state == 0) {
        mark_moby_for_removal(mine_moby);
        return;
    }
    if (hero.vehicle_mode != 0 && mine_moby->state == 0) {
        mark_moby_for_removal(mine_moby);
        return;
    }
    if (v->owner != 0 && v->owner->state != 0xFE && v->owner->state != 0xFD &&
        v->owner->cls == 0xBE && v->h36 == 0 && 0.0f < fast_vec_length(v)) {
        if (hero.ground_moby == 0 || FUN_002141f8_cf(hero.ground_moby) == 0) {
            predict_mine_impact(v, mine_moby, 1);
        } else if (predict_mine_impact(v, mine_moby, 0) == 0) {
            predict_mine_impact(v, mine_moby, 2);
        }
    }
    mine_state_snapshot = mine_moby->state;
    if (mine_state_snapshot == 0) {
        if (hero.items[0].moby != 0 && ((M910 *)hero.items[0].moby)->cls == 0xBE) {
            float mine_attachment_transform[4][4];
            float owner_bone_transform[4][4];
            U910  attachment_rotation_angles;
            {
                attachment_rotation_angles.q = 0;
                attachment_rotation_angles.f[0] = 3.1415927f;
                euler_rotation_matrix(mine_attachment_transform, &attachment_rotation_angles);
                build_moby_bone_transform((char *)v->owner, 0, (char *)owner_bone_transform);
                multiply_matrix(
                    mine_attachment_transform, owner_bone_transform, mine_attachment_transform);
                copy_matrix3x4(mine_moby->mtx, mine_attachment_transform);
                mine_moby->i94 = 0;
                mine_moby->flags |= 0x100;
            }
            return;
        }
        mark_moby_for_removal(mine_moby);
        return;
    }
    {
        Qry910 q;
        {
            float k;
            float explosion_camera_distance;
            float gz;

            mine_moby->flags &= 0xFEFF;
            qcopy(&old, &mine_moby->pos);
            if (!(mine_state_snapshot & 4)) {
                fast_vec_add(&mine_moby->pos, &mine_moby->pos, v);
                if (v->h50 == 0) {
                    v->vel.f[2] -= frame_time_sq * 9.8f;
                } else {
                    v->vel.f[2] -= frame_time_sq * 9.8f / 10.0f;
                    if (random_integer_below(scale_game_frames(6) - 1) == 0) {
                        float wz = resolve_camera_surface_height((float *)&mine_moby->pos, &q);

                        scale_vector_xyz(&q, v, 0.25f);
                        random_integer_below_cf(
                            &mine_moby->pos, &q, random_float_between(0.05f, 0.1f) * 210000.0f, wz);
                    }
                }
            }
            q.flags = 0x830000;
            q.fC = 3.0f;
            q.moby = mine_moby;
            q.i10 = 1;
            qcopy(&q, v);
            set_vector_length_xy(&q, &q, 1.0f);
            q.b8 = 2;
            q.dir[3] = 5627.925f;
            q.b9 = 1;
            q.cls = mine_moby->cls;
            mine_moby->pos.f[2] -= 0.4f;
            q.dir[2] = 1.0f;
            k = convert_integer_to_float(D_L00_0015F5CC - v->i74);
            k = k / scale_time(*(float *)&mine_bounce_ramp_duration);
            if (1.0f < k) {
                k = 1.0f;
            } else if (k < 0.0f) {
                k = 0.0f;
            }
            mine_moby->pos.f[2] -= v->f6C * fast_sin(v->f70) * k;
            if (!(mine_moby->state & 4) &&
                collision_line(&old, &mine_moby->pos, 0x10, (int)mine_moby, (int)&q) != 0) {
                if (v->h50 == 0 && collision_hit_material() == 0 && v->vel.f[2] < 0.0f) {
                    V910 t;
                    V910 n;
                    {
                        int   i;
                        char *p;

                        pool_splash(*(int *)&D_L00_001612D0_gp,
                                    *(int *)&D_L00_001612D8_gp,
                                    1,
                                    mine_moby->pos.f[0],
                                    mine_moby->pos.f[1],
                                    0.5f,
                                    -0.35f);
                        v->h50 = 1;
                        clear_u64_value((float *)v);
                        v->vel.f[2] = frame_time * -1.5f;
                        t[0] = mine_moby->pos.f[0];
                        t[1] = mine_moby->pos.f[1];
                        t[2] = coll_hit.pos.f[2];
                        p = spawn_burst_moby(t, 2.0f);
                        if (p != 0) {
                            p[0x23] = 0x70;
                        }
                        for (i = 0; i < 16; i++) {
                            float ang = random_angle_radians();
                            float rr = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
                            int   life;

                            n[0] = fast_cos(ang) * rr;
                            n[1] = fast_sin(ang) * rr;
                            n[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
                            life = FUN_L00_00257b90(0x5A, 0x78);
                            spawn_debris_chunk(t, n, random_integer_below(2), life);
                        }
                    }
                } else if (v->h50 != 0 || collision_hit_material() != 0) {
                    V910 t;
                    V910 n;
                    V910 r;
                    V910 ov;
                    {
                        if (coll_hit.moby != 0 &&
                            (is_moby_in_class_range_500_to_540((int)coll_hit.moby) != 0 ||
                             (coll_hit.moby->flags & 0x5000)) &&
                            coll_hit.moby->cls != mine_moby->cls && coll_hit.moby != hero.moby &&
                            coll_hit.moby != v->owner) {
                            mine_moby->bBC = 1;
                            if (mine_moby->state != 1) {
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                mine_moby->pos.f[2] = probe_ground_height(&mine_moby->pos, 0, 0.5f);
                            }
                        } else if (coll_hit.moby != 0 && coll_hit.moby->cls == mine_moby->cls) {
                            fast_vec_sub(&coll_hit.nrm, &mine_moby->pos, &coll_hit.moby->pos);
                            if (v->vel.f[2] < frame_time * -9.8f) {
                                mine_moby->bBC = 1;
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                reflect_vector(&collision_response_velocity, v, &coll_hit.nrm);
                                fast_vec_normalize(&collision_response_velocity,
                                                   &collision_response_velocity,
                                                   frame_time + frame_time);
                                fast_vec_normalize(&up, &coll_hit.nrm, 1.0f);
                            } else {
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                fast_vec_normalize(t, &coll_hit.nrm, frame_time_sq * 9.7f);
                                fast_vec_add(&mine_moby->pos, &mine_moby->pos, t);
                                reflect_vector(v, v, &coll_hit.nrm);
                                scale_vector_xyz(v, v, 0.4f);
                                fast_vec_normalize(t, &coll_hit.nrm, 1.0f);
                                fast_vec_dot(t, &up);
                                qcopy(n, &coll_hit.nrm);
                                goto orient;
                            }
                        } else if ((coll_hit.moby != 0 && coll_hit.moby != hero.moby &&
                                    coll_hit.moby != v->owner) ||
                                   coll_hit.i1C > 0) {
                            if (v->h50 != 0 || collision_hit_material() == 0 ||
                                v->vel.f[2] < frame_time * -9.8f) {
                                mine_moby->bBC = 1;
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                reflect_vector(&collision_response_velocity, v, &coll_hit.nrm);
                                fast_vec_normalize(&collision_response_velocity,
                                                   &collision_response_velocity,
                                                   frame_time + frame_time);
                                fast_vec_normalize(&up, &coll_hit.nrm, 1.0f);
                            } else {
                                if (coll_hit.moby != 0) {
                                    hd = (M910 *)FUN_002141f8_cf((unsigned char *)coll_hit.moby);
                                }
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                fast_vec_normalize(t, &coll_hit.nrm, frame_time_sq * 9.7f);
                                fast_vec_add(&mine_moby->pos, &mine_moby->pos, t);
                                *(OvlQuad *)ov = v->vel.q;
                                reflect_vector(v, v, &coll_hit.nrm);
                                scale_vector_xyz(v, v, 0.6f);
                                v->vel.f[2] *= 0.5f;
                                if (v->f6C == 0.0f) {
                                    float ph;

                                    v->f6C = absolute_float(ov[2]) * 2.0f;
                                    ph = -2.3561945f;
                                    if (0.0f < ov[2]) {
                                        ph = 0.7853982f;
                                    }
                                    v->f70 = ph;
                                    v->i74 = D_L00_0015F5CC;
                                }
                                fast_vec_normalize(t, &coll_hit.nrm, 1.0f);
                                if (0.707f < fast_vec_dot(t, &up) && ov[2] < 0.0f) {
                                    float fz = probe_ground_height(&mine_moby->pos, 0, 0.5f);

                                    if (mine_moby->pos.f[2] < fz) {
                                        mine_moby->pos.f[2] = fz;
                                    }
                                    v->vel.f[2] = -v->vel.f[2];
                                    if (fast_vec_length(v) < frame_time * 0.5f || hd != 0) {
                                        clear_u64_value((float *)v);
                                        mine_moby->state |= 4;
                                        mine_moby->i94 = *(int *)(mine_moby->p24 + 0x10);
                                        if (coll_hit.moby != 0 && hd != 0) {
                                            v->p4C = coll_hit.moby;
                                        }
                                    }
                                }
                                qcopy(n, &coll_hit.nrm);
                            orient: {
                                r[0] =
                                    n[0] * fast_cos(mine_moby->rz) + n[1] * fast_sin(mine_moby->rz);
                                r[1] =
                                    n[1] * fast_cos(mine_moby->rz) - n[0] * fast_sin(mine_moby->rz);
                                r[2] = n[2];
                                mine_moby->rx = step_rotation_toward_target(
                                    mine_moby->rx,
                                    -fast_arc_tan(square_root(r[0] * r[0] + r[2] * r[2]), r[1]),
                                    0.05235988f);
                                mine_moby->ry = step_rotation_toward_target(
                                    mine_moby->ry, fast_arc_tan(r[2], r[0]), 0.05235988f);
                            }
                            }
                        }
                    }
                }
            } else if (mine_moby->state & 4) {
                float fz = probe_ground_height(&mine_moby->pos, 0, 0.5f);

                if (v->p4C != 0) {
                    if (!(v->p4C != 0 && v->p4C->state != 0xFE && v->p4C->state != 0xFD)) {
                        mine_moby->bBC = 1;
                        v->p4C = 0;
                    } else if (coll_hit_moby != 0) {
                        hd = (M910 *)FUN_002141f8_cf((unsigned char *)coll_hit_moby);
                        if (hd != 0) {
                            FUN_L00_00260400(mine_moby,
                                             v->p4C,
                                             &mine_moby->pos,
                                             &mine_moby->rx,
                                             &mine_moby->pos,
                                             &mine_moby->rx);
                        }
                    }
                    if (hd == 0) {
                        int s;

                        mine_moby->state = 1;
                        s = v->i68;
                        if (s != -1) {
                            char *tab = ((char *)&voice_pool);
                            char *e = tab + s * 0x70;

                            if (*(M910 **)(e + 0x88) == mine_moby &&
                                *(unsigned char *)(e + 0x74) != 0) {
                                release_voice_slot(s);
                            }
                        }
                        v->vel.f[2] = 0.0f;
                        v->i68 = -1;
                        v->h34 = scale_game_frames(0x78);
                        v->p4C = 0;
                    }
                } else if (!(mine_moby->state & 0x10)) {
                    if (mine_moby->pos.f[2] - fz < frame_time_sq * 40.0f) {
                        mine_moby->pos.f[2] = fz;
                        v->h34 = 0;
                        if (coll_hit.moby != 0 &&
                            FUN_002141f8_cf((unsigned char *)coll_hit.moby) != 0) {
                            v->p4C = coll_hit.moby;
                        }
                    } else {
                        int s;

                        mine_moby->state = 1;
                        s = v->i68;
                        if (s != -1) {
                            char *tab = ((char *)&voice_pool);
                            char *e = tab + s * 0x70;

                            if (*(M910 **)(e + 0x88) == mine_moby &&
                                *(unsigned char *)(e + 0x74) != 0) {
                                release_voice_slot(s);
                            }
                        }
                        v->vel.f[2] = 0.0f;
                        v->i68 = -1;
                        v->h34 = scale_game_frames(0x78);
                    }
                }
                if ((v->p54 == 0 || v->p54->state == 0xFE || v->p54->state == 0xFD ||
                     !(v->p54->flags & 0x1000)) &&
                    (mine_moby->state & 0x10)) {
                    int s;

                    mine_moby->state = 1;
                    s = v->i68;
                    if (s != -1) {
                        char *tab = ((char *)&voice_pool);
                        char *e = tab + s * 0x70;

                        if (*(M910 **)(e + 0x88) == mine_moby &&
                            *(unsigned char *)(e + 0x74) != 0) {
                            release_voice_slot(s);
                        }
                    }
                    v->vel.f[2] = 0.0f;
                    v->p54 = 0;
                    v->i68 = -1;
                    v->h34 = scale_game_frames(0x78);
                    if (mine_moby->anim != 0) {
                        blend_moby_animation(mine_moby, 0, 0, 10);
                    }
                }
                if (v->p54 != 0 && (mine_moby->state & 0x10)) {
                    V910   t;
                    V910   n;
                    Qry910 homing_hit_descriptor;
                    U910   probe;
                    U910   collision_correction;
                    {
                        float sp;
                        float max_homing_step;
                        int   mode;

                        *(OvlQuad *)n = mine_moby->pos.q;
                        if ((mine_moby->b70 & 2) && mine_moby->b52 == 1 && mine_moby->anim != 2) {
                            blend_moby_animation(mine_moby, 2, 0, 10);
                        }
                        if (voice_is_playing(mine_moby, v->i68) == 0) {
                            v->i68 = allocate_voice_for_target_entry(1, 4, (int)mine_moby);
                        }
                        sp = v->f40 + frame_time_sq * 15.0f;
                        max_homing_step = frame_time * 4.0f;
                        v->f40 = sp;
                        if (max_homing_step < sp) {
                            v->f40 = max_homing_step;
                        }
                        mode = v->h52;
                        if (mode == 1) {
                            mine_moby->pos.f[2] += frame_scale * 0.05f;
                            if (fast_dec_timer(&v->i48) != 0) {
                                v->i48 = scale_game_frames(7);
                                v->h52 = 2;
                            }
                        } else if (mode == 2) {
                            mine_moby->pos.f[2] -= frame_scale * 0.05f;
                            if (fast_dec_timer(&v->i48) != 0) {
                                v->h52 = 0;
                            }
                        }
                        fast_vec_sub(t, &v->p54->pos, &mine_moby->pos);
                        fast_vec_normalize(t, t, v->f40);
                        fast_vec_add(&mine_moby->pos, &mine_moby->pos, t);
                        make_hit_descriptor(&homing_hit_descriptor, mine_moby, 0x10000, t, 1.0f);
                        qcopy(&probe, &mine_moby->pos);
                        probe.f[2] += 0.35f;
                        if (collect_mobies_in_sphere(
                                &probe, 0x10, mine_moby, &homing_hit_descriptor, 0.35f) != 0 &&
                            coll_hit_moby != 0 && ((M910 *)coll_hit_moby)->cls != 0x363 &&
                            ((M910 *)coll_hit_moby)->cls != 0x4A &&
                            ((M910 *)coll_hit_moby)->cls != 0x458 &&
                            ((M910 *)coll_hit_moby)->cls != 0x365 &&
                            ((M910 *)coll_hit_moby)->cls != 0x367) {
                            mine_moby->bBC = 1;
                        }
                        qcopy(&probe, &mine_moby->pos);
                        probe.f[2] += 0.25f;
                        if (FUN_L00_001f0d60(0.25f, &probe, 0, mine_moby) != 0) {
                            fast_vec_sub(&collision_correction, &coll_hit_position, &probe);
                            fast_vec_add(&mine_moby->pos, &mine_moby->pos, &collision_correction);
                        }
                        if (fast_dec_timer(&v->i58) != 0) {
                            mine_moby->bBC = 1;
                        }
                    }
                }
                update_moby_rotation_oscillation(mine_moby,
                                                 (char *)&v->f60,
                                                 (char *)&v->f64,
                                                 0.5235988f,
                                                 frame_time * 0.5235988f,
                                                 frame_time * 0.29670596f);
                if (mine_moby->state & 4) {
                    V910 t;
                    {
                        fast_vec_sub(t, &mine_moby->pos, &old);
                        v->vel.q = *(OvlQuad *)t;
                    }
                }
            }
            if (!(mine_moby->state & 2) && fast_dec_timer_s16(&v->h34) != 0) {
                mine_moby->state |= 2;
                v->p54 = 0;
            }
            {
                char *moby_hit_record = find_moby_hit(mine_moby, 0x830000, 0);

                if (moby_hit_record != 0 && *(M910 **)(moby_hit_record + 0x20) != 0 &&
                    (*(M910 **)(moby_hit_record + 0x20))->cls != mine_moby->cls) {
                    mine_moby->bBC = 1;
                }
            }
            mine_moby->bA4 = 0xFF;
            if (mine_moby->state & 2) {
                if (!(mine_moby->state & 4) ||
                    (vector_distance_xy(&((M910 *)hero.moby)->pos, &mine_moby->pos) < 0.75f &&
                     absolute_float(mine_moby->pos.f[2] - hero.motion.pos.f[2]) < 0.25f &&
                     (mine_moby->state & 0x10))) {
                    mine_moby->bBC = 1;
                } else if (fast_vec_dist(&((M910 *)hero.moby)->pos, &mine_moby->pos) < 0.75f) {
                    V910 t;
                    V910 n;
                    V910 r;
                    {
                        fast_vec_sub(r, &mine_moby->pos, &((M910 *)hero.moby)->pos);
                        *(OvlQuad *)t = *(OvlQuad *)r;
                        fast_vec_normalize(t, t, 0.75f);
                        fast_vec_add(r, &((M910 *)hero.moby)->pos, t);
                        *(OvlQuad *)n = *(OvlQuad *)r;
                        FUN_L00_00258490(
                            mine_moby, &mine_moby->pos, n, 0.4f, 0.25f, 0.25f, 0.5235988f, 0);
                    }
                } else {
                    M910 *best = 0;
                    float range =
                        (convert_integer_to_float(*((unsigned char *)&D_E910 + 0x11)) + 1.0f) *
                        4.0f;
                    int   i;
                    M910 *candidate_moby;

                    if (v->i78 != 0) {
                        range *= 3.0f;
                    }
                    i = 0;
                    candidate_moby = (M910 *)active_mobies[i];
                    while (candidate_moby != 0) {
                        float          rg;
                        unsigned char *p;

                        if (30.0f < fast_vec_dist(&mine_moby->pos, &candidate_moby->pos)) {
                            goto next;
                        }
                        if (fast_vec_dist(&mine_moby->pos, &candidate_moby->pos) < 1.5f) {
                            mine_moby->bBC = 1;
                            break;
                        }
                        if (is_moby_in_class_range_500_to_540((int)candidate_moby) != 0) {
                            goto next;
                        }
                        rg = range;
                        p = suck_get_aim_height((unsigned char *)candidate_moby);
                        if (p != 0) {
                            rg = range + convert_integer_to_float(p[0xA]) * 0.125f;
                        }
                        if (!(vector_distance_xy(&mine_moby->pos, &candidate_moby->pos) < rg)) {
                            goto next;
                        }
                        if (!(absolute_float(candidate_moby->pos.f[2] - mine_moby->pos.f[2]) <
                              2.0f)) {
                            goto next;
                        }
                        if (v->p54 != 0 && v->p54->state != 0xFE && v->p54->state != 0xFD) {
                            goto next;
                        }
                        if (best != 0) {
                            float candidate_distance =
                                vector_distance_xy(&mine_moby->pos, &candidate_moby->pos);

                            if (!(candidate_distance <
                                  vector_distance_xy(&mine_moby->pos, &best->pos))) {
                                goto next;
                            }
                        }
                        mine_moby->state |= 0x10;
                        best = candidate_moby;
                        blend_moby_animation(mine_moby, 1, 0, scale_game_frames(3));
                        allocate_voice_for_target_entry(0, 0, (int)mine_moby);
                        v->f40 = 0.0f;
                        v->i58 = scale_game_frames(600);
                        v->h52 = 1;
                        v->i48 = scale_game_frames(7);
                    next:
                        i++;
                        candidate_moby = (M910 *)active_mobies[i];
                    }
                    if (best != 0) {
                        v->p54 = best;
                    }
                }
            }
            v->i78 = 0;
            if (v->i5C < 0 || *(int *)&mine_force_burst != 0) {
                cart_burst(mine_moby, &mine_moby->pos, -1, 0.33f, 13.0f);
                mine_spawn_drops(mine_moby, D_L00_001B07B0);
                mark_moby_for_removal(mine_moby);
            }
            {
                float amp = v->f6C;

                if (0.05f < amp) {
                    float damped = frame_scale * -0.012000024f * amp + amp;

                    v->f6C = damped;
                    if (damped < 0.05f) {
                        v->f6C = 0.05f;
                    }
                }
            }
            k = convert_integer_to_float(D_L00_0015F5CC - v->i74);
            k = k / scale_time(*(float *)&mine_bounce_ramp_duration);
            if (1.0f < k) {
                k = 1.0f;
            } else if (k < 0.0f) {
                k = 0.0f;
            }
            v->f70 = fast_add_rotations(v->f70, frame_time * 6.2831855f);
            mine_moby->pos.f[2] = mine_moby->pos.f[2] + v->f6C * fast_sin(v->f70) * k + 0.4f;
            if (mine_moby->bBC != 0) {
                explosion_camera_distance = vector_distance_xy(&mine_moby->pos, D_L00_00166DC0);
                if (v->h50 == 0) {
                    V910 t;
                    {
                        float s = convert_integer_to_float(D_E910.b11) * 0.5f + 1.0f;
                        int   hit =
                            collect_mobies_in_sphere(&mine_moby->pos, 0x10, mine_moby, 0, s + s);

                        *(OvlQuad *)t = mine_moby->pos.q;
                        send_blast_hits(mine_moby,
                                        t,
                                        D_L00_00177F00,
                                        hit,
                                        0,
                                        0x810000,
                                        4,
                                        1,
                                        3.0f,
                                        0.25f,
                                        1.5f);
                    }
                }
                gz = FUN_L00_00259710(&mine_moby->pos);
                if (v->h50 != 0) {
                    V910 t;
                    V910 n;
                    V910 r;
                    {
                        float wz;
                        int   i;
                        int   j;
                        int   l;
                        int   cb;
                        int   cg;
                        int   cr;

                        *(OvlQuad *)t = mine_moby->pos.q;
                        t[2] += 3.5f;
                        wz = probe_ground_height(t, 0, 0.5f);
                        for (i = 0; i < 150; i++) {
                            *(OvlQuad *)r = 0;
                            r[0] = random_float_between(-1.0f, 1.0f);
                            r[1] = random_float_between(-1.0f, 1.0f);
                            r[2] = random_float_between(-0.5f, -2.0f);
                            *(OvlQuad *)n = *(OvlQuad *)r;
                            fast_vec_normalize(
                                n, n, random_float_between(frame_time * 4.0f, frame_time * 8.0f));
                            random_integer_below_cf(&mine_moby->pos,
                                                    n,
                                                    random_float_between(0.05f, 0.1f) * 210000.0f,
                                                    wz);
                        }
                        cr = 0x60;
                        cg = 0x60;
                        cb = 0x7F;
                        for (j = 0; j < 150; j++) {
                            float rr;
                            float ang;

                            *(OvlQuad *)r = 0;
                            r[0] = random_float_between(frame_time * -0.75f, frame_time * 0.75f);
                            r[1] = random_float_between(frame_time * -0.75f, frame_time * 0.75f);
                            r[2] = random_float_between(frame_time * 4.0f, frame_time * 16.0f);
                            *(OvlQuad *)n = *(OvlQuad *)r;
                            rr = random_float_between(0.0f, 0.35f);
                            ang = random_angle_radians();
                            r[0] = fast_cos(ang) * rr;
                            r[1] = fast_sin(ang) * rr;
                            r[2] = 0.0f;
                            fast_vec_add(r, r, &mine_moby->pos);
                            r[2] = wz + 0.5f;
                            n[2] -= rr * frame_time * 8.0f;
                            spawn_gravity_floor_burst_particle(r,
                                                               n,
                                                               0,
                                                               random_float_between(0.35f, 1.25f),
                                                               wz,
                                                               frame_time_sq * 20.0f,
                                                               (0x16 << 24) | (cb << 16) |
                                                                   (cg << 8) | cr,
                                                               (cb << 16) | (cg << 8) | cr,
                                                               1);
                        }
                        for (l = 0; l < 20; l++) {
                            int   c1 = mine_blend_color(0x7F000000,
                                                      (0x7F << 24) | (cb << 16) | (cg << 8) | cr,
                                                      random_float_between(0.25f, 1.0f));
                            int   c2 = mine_blend_color(0,
                                                      ((cb / 2) << 16) | ((cg / 2) << 8) | (cr / 2),
                                                      random_float_between(0.5f, 1.0f));
                            float sz;
                            int   life;

                            r[0] = random_float_between(-3.0f, 3.0f);
                            r[1] = random_float_between(-3.0f, 3.0f);
                            r[2] = random_float_between(frame_time * 4.0f, frame_time * 14.0f);
                            fast_vec_normalize(r, r, random_float_between(1.5f, 3.0f) * frame_time);
                            r[2] = random_float_between(3.0f, 6.0f) * frame_time;
                            *(OvlQuad *)n = mine_moby->pos.q;
                            n[2] = wz;
                            c1 = swap_packed_color_channels(c1, D_E910.b11);
                            c2 = swap_packed_color_channels(c2, D_E910.b11);
                            sz = random_float_between(31500.002f, 52500.0f);
                            life =
                                FUN_L00_00257b90(scale_game_frames(0x5A), scale_game_frames(0x96));
                            spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
                                n, r, c1, c2, sz, life, 1, D_L00_001B2080[11][0], -1);
                        }
                    }
                } else if (gz + 2.0f < mine_moby->pos.f[2]) {
                    V910 t;
                    {
                        clear_u64_value(t);
                        spawn_explosion(mine_moby,
                                        t,
                                        0,
                                        0.0f,
                                        0.0f,
                                        3,
                                        3,
                                        5,
                                        2.0f,
                                        1.0f,
                                        4.0f,
                                        1.0f,
                                        2,
                                        7.0f,
                                        0,
                                        1,
                                        -1,
                                        D_E910.b11);
                    }
                } else {
                    float sc = convert_integer_to_float(D_E910.b11) * 0.5f + 1.0f;
                    int   explosion_debris_count =
                        truncate_float_to_s32(explosion_camera_distance * 15.0f);
                    int i;

                    for (i = 0; i < explosion_debris_count; i++) {
                        int   kind = random_integer_below(4);
                        float tbl[4] = {
                            frame_time * 14.0f, frame_time * 22.0f, frame_time * 12.0f, 0.0f};
                        V910 n;
                        V910 r;
                        {
                            float rr;
                            float ang;

                            *(OvlQuad *)r = 0;
                            r[0] = random_float_between(frame_time * -0.5f, frame_time * 0.5f) * sc;
                            r[1] = random_float_between(frame_time * -0.5f, frame_time * 0.5f) * sc;
                            r[2] = random_float_between(frame_time * 4.0f, tbl[kind]) * sc;
                            *(OvlQuad *)n = *(OvlQuad *)r;
                            rr = random_float_between(0.0f, kind == 1 ? 0.5f : 0.25f) * sc;
                            ang = random_angle_radians();
                            r[0] = fast_cos(ang) * rr;
                            r[1] = fast_sin(ang) * rr;
                            r[2] = 0.0f;
                            fast_vec_add(r, r, &mine_moby->pos);
                            r[2] -= rr * fast_cos(0.785398f);
                            n[2] -= rr * frame_time * 8.0f;
                            switch (kind) {
                            case MINE_DEBRIS_FIXED_SECONDARY_COLOR: {
                                int   c1 = swap_packed_color_channels(0x1F101820, D_E910.b11);
                                float sz = random_float_between(200000.0f, 300000.0f) * sc;
                                int   life = FUN_L00_00257b90(scale_game_frames(0xB4),
                                                            scale_game_frames(0xF0));

                                mine_debris((char *)r, (char *)n, c1, 0x101010, sz, life, 0);
                                break;
                            }
                            case MINE_DEBRIS_FIXED_LIFETIME: {
                                int   c1 = swap_packed_color_channels(0x3F081020, D_E910.b11);
                                int   c2 = swap_packed_color_channels(0x0F081020, D_E910.b11);
                                float sz = random_float_between(50000.0f, 100000.0f) * sc;

                                mine_debris(
                                    (char *)r, (char *)n, c1, c2, sz, scale_game_frames(0xB4), 1);
                                break;
                            }
                            case MINE_DEBRIS_SHORT_LIVED: {
                                int c1 = swap_packed_color_channels(
                                    random_integer_below(100) < 0x28 ? 0x5FF8F8F8 : 0x2F486078,
                                    D_E910.b11);
                                int   c2;
                                float sz;
                                int   life;

                                c2 = swap_packed_color_channels(0x0F000020, D_E910.b11);
                                sz = sc * 150000.0f;
                                life = FUN_L00_00257b90(scale_game_frames(0x1E),
                                                        scale_game_frames(0x2D));
                                mine_debris((char *)r, (char *)n, c1, c2, sz, life, 2);
                                break;
                            }
                            case MINE_DEBRIS_SPHERICAL_VELOCITY: {
                                int c1 = mine_blend_color(
                                    0x7F000000, 0x7F182030, random_float_between(0.25f, 1.0f));
                                int c2 =
                                    mine_blend_color(0, 0x5F5F5F, random_float_between(0.5f, 1.0f));
                                float sz;
                                int   life;

                                c1 = swap_packed_color_channels(c1, D_E910.b11);
                                c2 = swap_packed_color_channels(c2, D_E910.b11);
                                build_spherical_offset(n,
                                                       random_float_between(0.0f, 1.0f) *
                                                           frame_time,
                                                       ang,
                                                       random_angle_radians());
                                sz = random_float_between(200000.0f, 300000.0f) * sc;
                                life = FUN_L00_00257b90(scale_game_frames(0xF0),
                                                        scale_game_frames(0x12C));
                                mine_debris((char *)r, (char *)n, c1, c2, sz, life, 3);
                                break;
                            }
                            }
                        }
                    }
                }
                mine_moby->bBC = 0;
                if (D_E910.b11 != 0) {
                    allocate_voice_for_target_entry(3, 0, (int)mine_moby);
                } else {
                    allocate_voice_for_target_entry(2, 0, (int)mine_moby);
                }
                l00_camera.shake_amount = explosion_camera_distance < 20.0f
                                              ? 0.4f - explosion_camera_distance * 0.0175f
                                              : 0.050000012f;
                l00_camera.shake_frames = scale_game_frames(0x19);
                fast_vec_dist(&mine_moby->pos, hero.motion.pos.f);
                if (D_E910.b11 != 0) {
                    mine_camera_blast(
                        (char *)&alternate_camera_blast_parameters, (char *)&mine_moby->pos, 0, 0);
                } else {
                    mine_camera_blast(
                        (char *)&camera_blast_parameters, (char *)&mine_moby->pos, 0, 0);
                }
                mine_spawn_drops(mine_moby, D_L00_001B07B0);
                mark_moby_for_removal(mine_moby);
            } else {
                jet_animate((unsigned char *)mine_moby, 0.25f);
            }
        }
    }
}

f32 D_L01_001613E0 = 0;
f32 D_L02_001613B0 = 0;
f32 D_L03_001613C0 = 0;

f32 D_L04_001613B0 = 0;
f32 D_L05_001613C0 = 0;
f32 D_L06_001613B0 = 0;

f32 D_L07_001613B0 = 0;
f32 D_L08_001613B0 = 0;

f32 D_L09_00161440 = 0;

f32 D_L10_001613C0 = 0;
f32 D_L11_001613B0 = 0;
f32 D_L12_001613C0 = 0;

f32 D_L13_00161480 = 0;
f32 D_L14_001615C0 = 0;

f32 D_L15_001614A0 = 0;
f32 D_L16_00161470 = 0;
f32 D_L17_00161440 = 0;

f32 D_L18_00161450 = 0;
