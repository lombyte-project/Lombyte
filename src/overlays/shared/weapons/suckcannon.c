/* Shared moby class suckcannon: the suck cannon, its suction of mobys, the shots it fires and its beam. */

#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/input/pad_state.h"
#include "rnc/math/rotations/matrix_to_quaternion.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "qzero.h"
#include "rnc/overlay/shared/weapons/suckcannon_data/suck_beam_draw_mesh.h"

#define G1 (&D_0013E520)

typedef struct {
    int a[16];
} V __attribute__((aligned(16)));

typedef struct {
    u8  pad0[0x2078];
    s32 i2078;
    s32 i207C;
} G_2db508;

typedef struct {
    char p0[0x154];
    f32  f154;
    f32  f158;
    char p1[0x350 - 0x15C];
    char m350[0x40];
} C_002da250;

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V_002da250;

typedef struct {
    f32 x, y, z, w;
} V_da360;

typedef struct M_da360 {
    u8              p0[0x10];
    V_da360         pos;
    u8              p20[4];
    u8             *p24;
    struct M_da360 *next;
    u8              p2c[4];
    u8              p30[4];
    u16             f34;
    u8              p36[0xa6 - 0x36];
    s16             a6;
} M_da360;

typedef struct {
    u8  p0[0x68];
    s16 h68;
    s16 h6a;
} R;

typedef struct {
    u8 p[9];
    u8 b9;
} G1T;

typedef struct {
    u8  p[0x2078];
    s32 i2078;
} G2T;

typedef struct {
    u8  pad0[0x2050];
    u8 *slot[10];
    s32 i2078;
    s32 i207C;
} G_002da660;

typedef struct {
    u8 p[9];
    u8 b9;
} H_002da660;

/* Update the Suck Cannon: suction, aiming and firing, the stored mobys,
   target selection, usage statistics and cancelling the release input. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DBC40), where it is exact; names translated to the US level program. */
typedef unsigned int SuckQuad __attribute__((mode(TI)));

typedef union {
    struct {
        float x, y, z, w;
    } f;
    SuckQuad q;
} SuckVec __attribute__((aligned(16)));

typedef struct {
    SuckVec row[3];
} SuckMatrix;

typedef struct {
    SuckVec row[4];
} SuckMatrix4;

typedef union {
    struct {
        SuckVec     velocity, end;
        SuckMatrix  rotation;
        SuckMatrix4 inverse;
        SuckVec     point, offset;
    } aim;
    struct {
        SuckVec     velocity;
        SuckMatrix  rotation;
        SuckMatrix4 inverse;
        SuckVec     launch, point, offset;
    } fire;
} SuckWork;

typedef struct SuckMoby SuckMoby;

typedef struct {
    u8 pad[8];
    void (*fire)(SuckMoby *, SuckVec *, SuckMoby *, float);
} SuckHandlers;

typedef struct SuckClass {
    u8            pad0[0x2C];
    SuckHandlers *handlers;
    u8            pad30[0x16];
    s16           category;
} SuckClass;

typedef struct SuckCannon {
    s32       unused;
    s32       sound;
    s32       cooldown;
    s32       blocked;
    SuckMoby *target;
    s32       target_timer;
    float     target_height;
} SuckCannon;

struct SuckMoby {
    u8          pad0[0xC];
    float       radius;
    SuckVec     pos;
    s8          state;
    u8          pad21[3];
    SuckClass  *cls;
    SuckMoby   *next;
    u8          pad2C[8];
    u16         flags;
    u8          pad36[0x1D];
    u8          anim_state;
    u8          pad54[0x1C];
    u8          anim_flags;
    u8          pad71[7];
    SuckCannon *data;
    u8          pad7C[0x18];
    s32         collision;
};

typedef struct SuckHero {
    u8         pad0[0x80];
    SuckVec    pos;
    u8         pad90[0x70];
    SuckVec    displacement; /* 0x100: added to the launch velocity */
    u8         pad110[0x530];
    SuckMatrix basis; /* 0x640: aims the shot outside first person */
    u8         pad670[0x14];
    float      pitch;
    float      yaw;
    u8         pad68C[0xA14];
    s32        fire_buttons; /* 0x10A0: item slot 0 button mask */
    u8         pad10A4[0x8];
    u8         firing; /* 0x10AC: 2 while a shot is fired */
    u8         pad10AD[0x3];
    s32        ready_frames; /* 0x10B0: must exceed 15 (22) frames to suck (fire) */
    s32        slot_state;   /* 0x10B4: 3 = released */
    u8         pad10B8[0x1F8];
    s32        aim_locked;
    u8         pad12B4[0x4];
    SuckMoby  *aim_target;
    float      aim_yaw;
    u8         pad12C0[0xD90];
    SuckMoby  *stored[10]; /* 0x2050: sucked-in mobys, 5 slots (10 with the gold cannon) */
    s32        count;      /* 0x2078: stored mobys ready to fire */
    s32        sucking;    /* 0x207C: mobys still being pulled in */
    SuckMoby  *moby;
    s32        state; /* 0x2084: 30 = first-person aim */
    u8         pad2088[0x4];
    s32        control_mode;
    u8         pad2090[0x17];
    u8         unk20A7;
    u8         pad20A8[0x4];
    u8         unk20AC;
} SuckHero;

typedef struct SuckInput {
    u8  pad0[0x1A0];
    s32 held;
    u8  pad1A4[0x4];
    s32 used;
} SuckInput;

typedef struct SuckVoice {
    u8        pad0[0x4];
    u8        active;
    u8        pad5[0x13];
    SuckMoby *owner;
    u8        pad1C[0x54];
} SuckVoice;

typedef struct SuckStats {
    u8  pad0[0x48];
    u16 count;
    u16 longest;
    s32 level_mask;
} SuckStats;

typedef struct SuckHit {
    u8        pad0[0x18];
    SuckMoby *moby;
    u8        pad1C[0x4];
    SuckVec   pos;
} SuckHit;

typedef struct {
    u8 pad[9];
    u8 gold;
} SuckItemVariant; /* item 9 (Suck Cannon) is gold */

typedef struct {
    u8    pad[0x10];
    float height;
} SuckAimHeight;

typedef s32 (*Fn_db890)(void *);

typedef struct Cb_db890 {
    char     pad[0x10];
    Fn_db890 f10;
} Cb_db890;

typedef struct Sub {
    char      pad[0x2C];
    Cb_db890 *f2C;
} Sub;

typedef struct Obj {
    char pad0[0x20];
    u8   f20;
    char pad1[3];
    Sub *f24;
    char pad2[0x50];
    s32  f78;
} Obj;

typedef struct {
    u8  pad0[0x2050];
    s32 slot[11];
    s32 count;
} G;

typedef int u128_DD3D8 __attribute__((mode(TI)));

typedef struct {
    u128_DD3D8 v[5];
} B50;

typedef struct {
    int v[6];
} I6;

typedef struct {
    char  p0[0x160];
    float f160;
    int   p164;
    int   i168;
} Cam;

typedef struct {
    int v[16];
} __attribute__((aligned(16))) Mtx_002dc938;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2DDEA0;

typedef struct {
    char pad0[0xC];
    void (*fC)(void *);
} Cb;

typedef struct {
    char  pad0[0x10];
    int   i10;
    char  pad14[0x10];
    float f24;
    char  pad28[4];
    Cb   *f2C;
    char  pad30[0x16];
    short h46;
} Cls;

typedef struct {
    float          x, y, f8, timer;
    Vec_2DDEA0     vel;
    Vec_2DDEA0     q20;
    Vec_2DDEA0     v30;
    Vec_2DDEA0     v40;
    Vec_2DDEA0     v50;
    struct Moby   *owner;
    float          f64;
    short          state;
    short          idx;
    float          f6C;
    unsigned char *anims;
    struct Moby   *target;
    float          f78, f7C, f80;
    int            i84, i88;
    float          f8C, f90;
    int            i94;
    float          f98;
    short          h9C, h9E;
} R_2DDEA0;

typedef struct {
    char         pad0[0x18];
    struct Moby *hit;
    char         pad1C[4];
    Vec_2DDEA0   v20;
    Vec_2DDEA0   v30;
    Vec_2DDEA0   v40;
} Coll;

typedef struct {
    Vec_2DDEA0    v;
    char          pad10[8];
    unsigned char b18;
    unsigned char b19;
    short         h1A;
    int           pad1C;
} Hit_2DDEA0;

typedef union {
    Vec_2DDEA0 v[15];
    u128       q;
    struct {
        Vec_2DDEA0 pad;
        Hit_2DDEA0 hit;
    } h;
} Frame_2DDEA0;

typedef struct {
    float        q[4][4];
    unsigned int col[4];
    float        x0, y0, x1, y1, x2, y2, x3, y3;
    long         word0, word1, word2, word3;
} Pkt;

typedef struct {
    char pad[0x2084];
    int  f;
    int  pad2;
    int  mode;
} G_c;

typedef int u128_DF168 __attribute__((mode(TI)));

typedef struct {
    int pad[5];
    int f14;
} E04_DF168;

typedef struct {
    short idx;
    short angle;
    float f;
    float phase;
    float phase_step;
    float speed;
    char *p;
} E04;

/* Form the Suck Cannon's bending suction tube and update its particles. */
/* Ported from rac1-decomp (nonmatching/shared/func_L00_002DF410.c: func_L00_002DF410). */
typedef unsigned int Q41 __attribute__((mode(TI)));

typedef union {
    struct {
        float x, y, z, w;
    } f;
    Q41 q;
} V41 __attribute__((aligned(16)));

typedef struct {
    u8  pad0[0x34];
    u16 flags;
    u8  pad36[0x8A];
    V41 basis[3];
} M41;

typedef struct {
    u8   pad0[0x640];
    V41  matrix[3];
    u8   pad670[0x1A10];
    M41 *moby;
    int  camera_mode;
    u8   pad2088[0x240];
    s16  special;
} Hero41;

typedef struct {
    float unknown0;
    int   mode;
    u8    pad8[2];
    u8    r, g;
    u8    padC[4];
    V41   velocity;
    float acceleration;
} Tail41;

typedef struct Particle41 {
    u8     pad0[0xA];
    s16    life;
    float  size;
    V41    pos;
    Tail41 tail;
} Particle41;

typedef float VQ[4] __attribute__((aligned(16)));

typedef struct {
    u8  pad[0x46];
    s16 h46;
} T_2df1e0;

typedef struct {
    u8        pad[0x10];
    u8        v10[0x10];
    u8        b20;
    u8        pad2[3];
    T_2df1e0 *p24;
} O_2df1e0;

extern int  free_effect_particle() __asm__("FUN_L00_00267a08");
s32         scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void build_look_at_matrix(void *, void *, float, void *) __asm__("FUN_00214890");
extern s64  get_effect_texture(s32) __asm__("func_001F44B8");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void FUN_L00_002db848(void);
extern void FUN_L00_002da250_c(void *, f32 *, f32 *, void *) __asm__("FUN_L00_002da250");
extern void suck_beam_update(void *, void *) __asm__("FUN_L00_002ddf60");
extern R   *FUN_L00_002db890_c(M_da360 *) __asm__("FUN_L00_002db890");
extern s32  FUN_001efa68_c(void *, V_da360 *, s32, M_da360 *, s32) __asm__("FUN_001efa68");
extern void FUN_L00_002da1e0_c(M_da360 *) __asm__("FUN_L00_002da1e0");
extern void FUN_L00_002df1e0_c(M_da360 *) __asm__("FUN_L00_002df1e0");
u8         *FUN_L00_002db890_002da660(u8 *) __asm__("FUN_L00_002db890");
void        FUN_001e93b0_002da660(void *, s32, s32) __asm__("FUN_001e93b0");
extern int  collision_segment_hits(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int  queue_animation_update(int, int, int, int, int, int, int) __asm__("FUN_001ff308");
extern void start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
extern void hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void update_hud_counter_cap_and_visibility(void) __asm__("FUN_L00_002377b8");
extern void hud_draw_gauge_frame(void) __asm__("FUN_L00_00237840");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void suck_beam_queue_draw(int) __asm__("FUN_L00_002defe0");
void        stop_suck_cannon_suction_effect(int callback_context) __asm__("FUN_L00_002df048");
extern void suck_delete_callback(void) __asm__("FUN_L00_00237798");
extern void suck_reset(SuckMoby *) __asm__("FUN_L00_002ddca0");
extern void
suck_draw_reticle(int, int, float, float, int, int, int, float, int) __asm__("FUN_L00_001edf60");
void            debug_print(void *) __asm__("FUN_001e93b0");
u8             *FUN_L00_002db890_002db8f8(u8 *) __asm__("FUN_L00_002db890");
u8             *FUN_L00_002db890_002dbb20(u8 *) __asm__("FUN_L00_002db890");
void            FUN_L00_002d9fd0_002dbb20(void *, void *, void *) __asm__("FUN_L00_002d9fd0");
void            mark_moby_for_removal_cf(void *, s32, s32) __asm__("FUN_L00_00257470");
s32             FUN_001f96f8_002dbb20(s32) __asm__("FUN_001f96f8");
u8             *FUN_L00_002db890_002dbc30(u8 *) __asm__("FUN_L00_002db890");
s32             FUN_001f96f8_002dbc30(s32) __asm__("FUN_001f96f8");
void            FUN_001f9a10_002dbc30(void *, void *, void *) __asm__("FUN_001f9a10");
u8             *FUN_L00_002db890_u(u8 *) __asm__("FUN_L00_002db890");
extern char    *spawn_debris(char *, char *, int, int, int) __asm__("FUN_L00_002af450");
extern char    *mine_camera_blast(char *, char *, int, char *) __asm__("FUN_L00_002d3838");
extern s32      scale_game_frames_c(s32) __asm__("func_001F96F8");
extern unsigned swap_packed_color_channels(unsigned, int) __asm__("FUN_L00_0025c0e8");
extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void swap_color_channel_words(int *, int *, int *, int) __asm__("FUN_L00_0025c088");
extern void spawn_particle(void *, void *, int, int, float, int, int, int, int, float) __asm__("FUN_L00_0026a9f0");
extern char *func_L00_002E0CB8_DD3D8(float,
                                     void *,
                                     void *,
                                     void *,
                                     int,
                                     unsigned char,
                                     unsigned char,
                                     unsigned char,
                                     unsigned char) __asm__("FUN_L00_002df808");
extern int  func_L00_001F2BE8_DD3D8(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
void        FUN_0020e098_002dc938(char *) __asm__("FUN_0020e098");
extern void build_quaternion_from_axis_angle(void *, void *, f32) __asm__("FUN_00214530");
f32         normalize_angle(f32 a) __asm__("FUN_L00_0025e310");
void        spawn_effect_particle_with_transformed_random_offset(void *pos,
                                                                 void *mat) __asm__("FUN_L00_0026c3d0");
extern R_2DDEA0 *func_L00_002DCD40_2DDEA0(struct Moby *) __asm__("FUN_L00_002db890");
extern void      approach_w(void *, void *, float, float, float, float) __asm__("FUN_L00_00258278");
extern int       func_001F9850_2DDEA0(int) __asm__("FUN_001f96f8");
extern void      extract_matrix_rotation_quaternion(void *, void *) __asm__("FUN_002144d8");
extern void      func_001FA6C0_2DDEA0(void *, void *) __asm__("FUN_001fa4f8");
extern int       func_L00_002DB6C8_2DDEA0(void *, void *) __asm__("FUN_L00_002da218");
extern void      func_001F9BF0_2DDEA0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void      func_001F9BD8_2DDEA0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void      func_001E9768_2DDEA0(void *, void *) __asm__("FUN_001e93e8");
extern void      func_L00_001FF4B0_2DDEA0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void      func_0020EEE8_2DDEA0(void *) __asm__("FUN_0020e098");
extern void      func_L00_002DDDE8_2DDEA0(void *, void *, int) __asm__("FUN_L00_002dc938");
extern float     func_001F9CB8_2DDEA0(void *) __asm__("FUN_001f9af0");
extern float     func_L00_001FF3E0_2DDEA0(void *) __asm__("FUN_L00_001ff430");
extern void      func_001F9C30_2DDEA0(void *, void *, float) __asm__("FUN_001f9a68");
extern int func_L00_001F2BE8_2DDEA0(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int func_L00_001F10E0_2DDEA0(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void func_L00_001FF610_2DDEA0(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern int
func_L00_00259B08_2DDEA0(void *, void *, float, int, float, int) __asm__("FUN_L00_00258ad0");
extern void func_001E9730_2DDEA0(void *) __asm__("FUN_001e93b0");
extern void build_look_at_matrix_c2(void *, void *, float, void *) __asm__("build_look_at_matrix");
extern float
            rope_spring_turn(void *, float, float, float, float, float) __asm__("FUN_L00_001eb328");
extern int  collision_line_ddf60(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void interpolate_vector(void *, void *, void *, float) __asm__("FUN_L00_0025b6b8");
extern Particle41 *
spawn_puff(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");
extern float vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern void  scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
s32          is_moby_class_e2_cc_de_3ee_d6_e1_d5_or_df(O_2df1e0 *) __asm__("FUN_L00_0025e3f8");
void         begin_bolt_attraction(O_2df1e0 *, void *, f32, f32) __asm__("FUN_L00_002a7210");
void         try_collect_ammo_pickup(O_2df1e0 *) __asm__("FUN_L00_002c5be8");

extern int                      D_L00_00173E40_lui[] __asm__("D_L00_00173E40") NOT_SDA;
extern char                     D_L00_00166DC0[];
extern unsigned char D_0013E529 NOT_SDA;
extern char                    *suck_cannon_moby_slots[] __asm__("D_001413A0");
extern LevelCamera              l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;
extern M_da360                 *D_L00_0015FFE4;
extern G1T                      D_0013E520;
extern char                     D_L00_001E9FE8[] NOT_SDA;
extern int                      D_L00_00173E40_lui[];
extern s32                      D_0015EEA4;
extern SuckMoby                *suck_moby_list __asm__("D_L00_0015FFE4");
extern char                     suck_fire_tuning[1] __asm__("D_L00_00161B30") __attribute__((sda));
extern u8                      *D_001403E0_lui __asm__("D_001403E0") NOT_SDA;
extern u32   suck_cannon_storage_euler_offset[1] __asm__("D_L00_00161B20") __attribute__((sda));
extern u8    D_0013F3D0_lui[] __asm__("D_0013F3D0") NOT_SDA;
extern B50   l00_suck_cannon_explosion_blast_parameters __asm__("D_L00_001EA0B0");
extern I6    l00_suck_cannon_explosion_first_color_choices __asm__("D_L00_001EA100");
extern I6    l00_suck_cannon_explosion_second_color_choices __asm__("D_L00_001EA118");
extern float D_0015ED6C;
extern int  *D_L00_00177F00[];
extern float frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern unsigned char        D_0013E629_2DDEA0[] __asm__("D_0013E529");
extern unsigned char        D_001404FC_2DDEA0[] __asm__("D_001403FC");
extern char                 D_0013F4D0_2DDEA0[] __asm__("D_0013F3D0");
extern char                 D_0013FA90_2DDEA0[] __asm__("D_0013F990");
extern char                 D_L00_001EA4B0_2DDEA0[] __asm__("D_L00_001EA130");
extern char                 D_L00_001E5EE0[] NOT_SDA;
extern char                 D_L00_001E6020[] NOT_SDA;
extern float                D_L00_00161B50 __attribute__((sda));
extern short                D_L00_001E6008[] NOT_SDA;
extern int                  D_L00_00161B44_gp __asm__("D_L00_00161B44") __attribute__((sda));
extern int                  D_L00_00161B40_gp __asm__("D_L00_00161B40") __attribute__((sda));
extern E04                  D_L00_001E4C80[];
extern char                 D_L00_00161B60[] MACRO_ADDR;
extern char                 D_L00_00161B70[] MACRO_ADDR;
extern char                 D_L00_00166FD0[];
extern char                 D_L00_001E5F30[];
extern char                 D_L00_001E5F40[];
extern float D_L00_00161B80 MACRO_ADDR;
extern float D_L00_00161B84 MACRO_ADDR;
extern int                  D_L00_00161B40 __attribute__((sda));
extern int                  D_L00_00161B44 __attribute__((sda));
extern int                  D_L00_00161B54 __attribute__((sda));
extern int                  D_L00_00161B58 __attribute__((sda));
extern short                D_L00_001E6008[];
extern V41                  l00_suck_beam_ring_source_offsets[] __asm__("D_L00_001E4B40");
extern E04                  D_L00_001E4C80_lui[] __asm__("D_L00_001E4C80");
extern char                 D_L00_00161B60[];
extern int                  D_L00_00161B58_gp __asm__("D_L00_00161B58") __attribute__((sda));
extern char                 D_L00_00161B70[];
extern char                 collision_hit_point[] __asm__("D_L00_00173E60");
extern char                 D_L00_001E5EE0[];
extern char                 D_L00_001E6020[];
extern float                D_0015ED60;
extern float                D_L00_00161B84;
extern float                D_L00_00161B80;
extern float                suck_beam_segment_turn_spring_states[] __asm__("D_L00_001E5FE0");
extern s32                  D_L00_00161B40_gp __asm__("D_L00_00161B40") __attribute__((sda));
extern s32                  D_L00_00161B44_gp __asm__("D_L00_00161B44") __attribute__((sda));
extern f32                  D_L00_00161B50_gp __asm__("D_L00_00161B50") __attribute__((sda));
extern u8                   D_L00_001E5F40_lui[][16] __asm__("D_L00_001E5F40") NOT_SDA;

E04 D_L00_001E4C80[200] = {0};

E04 D_L01_001FF5E0[200] = {0};

E04 D_L02_001D7750[200] = {0};

E04 D_L03_001D6550[200] = {0};

E04 D_L04_001D7390[200] = {0};

E04 D_L05_0020BEC0[200] = {0};

E04 D_L06_001D8E40[200] = {0};

E04 D_L07_001DAE00[200] = {0};

E04 D_L08_001ED720[200] = {0};

E04 D_L09_00204100[200] = {0};

E04 D_L10_001D7F00[200] = {0};

E04 D_L11_001D7AD0[200] = {0};

E04 D_L12_001F94A0[200] = {0};

E04 D_L13_001D7B00[200] = {0};

E04 D_L14_001E3FB0[200] = {0};

E04 D_L15_001D74D0[200] = {0};

E04 D_L16_001D69F0[200] = {0};

E04 D_L17_001D77F0[200] = {0};

E04 D_L18_001D7B30[200] = {0};

s16 D_L01_00200968[1] __attribute__((section(".data"))) = {0};

s16 D_L02_001D8AD8[1] __attribute__((section(".data"))) = {0};

s16 D_L03_001D78D8[1] __attribute__((section(".data"))) = {0};

s16 D_L04_001D8718[1] __attribute__((section(".data"))) = {0};

s16 D_L05_0020D248[1] __attribute__((section(".data"))) = {0};

s16 D_L06_001DA1C8[1] __attribute__((section(".data"))) = {0};

s16 D_L07_001DC188[1] __attribute__((section(".data"))) = {0};

s16 D_L08_001EEAA8[1] __attribute__((section(".data"))) = {0};

s16 D_L09_00205488[1] __attribute__((section(".data"))) = {0};

s16 D_L10_001D9288[1] __attribute__((section(".data"))) = {0};

s16 D_L11_001D8E58[1] __attribute__((section(".data"))) = {0};

s16 D_L12_001FA828[1] __attribute__((section(".data"))) = {0};

s16 D_L13_001D8E88[1] __attribute__((section(".data"))) = {0};

s16 D_L14_001E5338[1] __attribute__((section(".data"))) = {0};

s16 D_L15_001D8858[1] __attribute__((section(".data"))) = {0};

s16 D_L16_001D7D78[1] __attribute__((section(".data"))) = {0};

s16 D_L17_001D8B78[1] __attribute__((section(".data"))) = {0};

s16 D_L18_001D8EB8[1] __attribute__((section(".data"))) = {0};

char D_L01_0020B708 NOT_SDA = 0x63;
char                D_L01_00161EB0[1] = {-37};

u32 D_L01_00161EA0[1] = {0xbfc90fdb};

I6 D_L01_0020B820 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L01_0020B838 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L01_00200840 NOT_SDA = 0;
char D_L01_00200980 NOT_SDA = 0;
float               D_L01_00161ED0 = 0.006f;
int                 D_L01_00161EC4 = 1;
int                 D_L01_00161EC0 = 1;
char                D_L01_00161EE0 = 0;
char                D_L01_00161EF0 = 0;
char D_L01_00200890 NOT_SDA = 0;
char D_L01_002008A0 NOT_SDA = 0;
float               D_L01_00161F00 = 0;
float               D_L01_00161F04 = 0;
int                 D_L01_00161ED4 = 0;
int                 D_L01_00161ED8 = 0;
char D_L02_001FAB98 NOT_SDA = 0x63;
char                D_L02_00161E50[1] = {-37};

u32 D_L02_00161E40[1] = {0xbfc90fdb};

I6 D_L02_001FACB0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L02_001FACC8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L02_001D89B0 NOT_SDA = 0;
char D_L02_001D8AF0 NOT_SDA = 0;
float               D_L02_00161E70 = 0.006f;
int                 D_L02_00161E64 = 1;
int                 D_L02_00161E60 = 1;
char                D_L02_00161E80 = 0;
char                D_L02_00161E90 = 0;
char D_L02_001D8A00 NOT_SDA = 0;
char D_L02_001D8A10 NOT_SDA = 0;
float               D_L02_00161EA0 = 0;
float               D_L02_00161EA4 = 0;
int                 D_L02_00161E74 = 0;
int                 D_L02_00161E78 = 0;
char D_L03_001E3448 NOT_SDA = 0x63;
char                D_L03_00161B50[1] = {-37};

u32 D_L03_00161B40[1] = {0xbfc90fdb};

I6 D_L03_001E3560 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L03_001E3578 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L03_001D77B0 NOT_SDA = 0;
char D_L03_001D78F0 NOT_SDA = 0;
float               D_L03_00161B70 = 0.006f;
int                 D_L03_00161B64 = 1;
int                 D_L03_00161B60 = 1;
char                D_L03_00161B80 = 0;
char                D_L03_00161B90 = 0;
char D_L03_001D7800 NOT_SDA = 0;
char D_L03_001D7810 NOT_SDA = 0;
float               D_L03_00161BA0 = 0;
float               D_L03_00161BA4 = 0;
int                 D_L03_00161B74 = 0;
int                 D_L03_00161B78 = 0;
char D_L04_001DC5B8 NOT_SDA = 0x63;
char                D_L04_00161C80[1] = {-37};

u32 D_L04_00161C70[1] = {0xbfc90fdb};

I6 D_L04_001DC6D0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L04_001DC6E8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L04_001D85F0 NOT_SDA = 0;
char D_L04_001D8730 NOT_SDA = 0;
float               D_L04_00161CA0 = 0.006f;
int                 D_L04_00161C94 = 1;
int                 D_L04_00161C90 = 1;
char                D_L04_00161CB0 = 0;
char                D_L04_00161CC0 = 0;
char D_L04_001D8640 NOT_SDA = 0;
char D_L04_001D8650 NOT_SDA = 0;
float               D_L04_00161CD0 = 0;
float               D_L04_00161CD4 = 0;
int                 D_L04_00161CA4 = 0;
int                 D_L04_00161CA8 = 0;
char D_L05_002187F8 NOT_SDA = 0x63;
char                D_L05_00161E20[1] = {-37};

u32 D_L05_00161E10[1] = {0xbfc90fdb};

I6 D_L05_00218910 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L05_00218928 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L05_0020D120 NOT_SDA = 0;
char D_L05_0020D260 NOT_SDA = 0;
float               D_L05_00161E40 = 0.006f;
int                 D_L05_00161E34 = 1;
int                 D_L05_00161E30 = 1;
char                D_L05_00161E50 = 0;
char                D_L05_00161E60 = 0;
char D_L05_0020D170 NOT_SDA = 0;
char D_L05_0020D180 NOT_SDA = 0;
float               D_L05_00161E70 = 0;
float               D_L05_00161E74 = 0;
int                 D_L05_00161E44 = 0;
int                 D_L05_00161E48 = 0;
char D_L06_00201458 NOT_SDA = 0x63;
char                D_L06_00161C80[1] = {-37};

u32 D_L06_00161C70[1] = {0xbfc90fdb};

I6 D_L06_00201570 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L06_00201588 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L06_001DA0A0 NOT_SDA = 0;
char D_L06_001DA1E0 NOT_SDA = 0;
float               D_L06_00161CA0 = 0.006f;
int                 D_L06_00161C94 = 1;
int                 D_L06_00161C90 = 1;
char                D_L06_00161CB0 = 0;
char                D_L06_00161CC0 = 0;
char D_L06_001DA0F0 NOT_SDA = 0;
char D_L06_001DA100 NOT_SDA = 0;
float               D_L06_00161CD0 = 0;
float               D_L06_00161CD4 = 0;
int                 D_L06_00161CA4 = 0;
int                 D_L06_00161CA8 = 0;
char D_L07_002113A8 NOT_SDA = 0x63;
char                D_L07_00161AD0[1] = {-37};

u32 D_L07_00161AC0[1] = {0xbfc90fdb};

I6 D_L07_002114C0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L07_002114D8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L07_001DC060 NOT_SDA = 0;
char D_L07_001DC1A0 NOT_SDA = 0;
float               D_L07_00161AF0 = 0.006f;
char                D_L07_00161B00 = 0;
char                D_L07_00161B10 = 0;
char D_L07_001DC0B0 NOT_SDA = 0;
char D_L07_001DC0C0 NOT_SDA = 0;
float               D_L07_00161B20 = 0;
float               D_L07_00161B24 = 0;
int                 D_L07_00161AF4 = 0;
int                 D_L07_00161AF8 = 0;
char D_L08_001FC458 NOT_SDA = 0x63;
char                D_L08_00162120[1] = {-37};

u32 D_L08_00162110[1] = {0xbfc90fdb};

I6 D_L08_001FC570 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L08_001FC588 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L08_001EE980 NOT_SDA = 0;
char D_L08_001EEAC0 NOT_SDA = 0;
float               D_L08_00162140 = 0.006f;
int                 D_L08_00162134 = 1;
int                 D_L08_00162130 = 1;
char                D_L08_00162150 = 0;
char                D_L08_00162160 = 0;
char D_L08_001EE9D0 NOT_SDA = 0;
char D_L08_001EE9E0 NOT_SDA = 0;
float               D_L08_00162170 = 0;
float               D_L08_00162174 = 0;
int                 D_L08_00162144 = 0;
int                 D_L08_00162148 = 0;
char D_L09_00208D28 NOT_SDA = 0x63;
char                D_L09_00161B70[1] = {-37};

u32 D_L09_00161B60[1] = {0xbfc90fdb};

I6 D_L09_00208E40 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L09_00208E58 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L09_00205360 NOT_SDA = 0;
char D_L09_002054A0 NOT_SDA = 0;
float               D_L09_00161B90 = 0.006f;
int                 D_L09_00161B84 = 1;
int                 D_L09_00161B80 = 1;
char                D_L09_00161BA0 = 0;
char                D_L09_00161BB0 = 0;
char D_L09_002053B0 NOT_SDA = 0;
char D_L09_002053C0 NOT_SDA = 0;
float               D_L09_00161BC0 = 0;
float               D_L09_00161BC4 = 0;
int                 D_L09_00161B94 = 0;
int                 D_L09_00161B98 = 0;
char D_L10_001DCF28 NOT_SDA = 0x63;
char                D_L10_00161B70[1] = {-37};

u32 D_L10_00161B60[1] = {0xbfc90fdb};

I6 D_L10_001DD040 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L10_001DD058 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L10_001D9160 NOT_SDA = 0;
char D_L10_001D92A0 NOT_SDA = 0;
float               D_L10_00161B90 = 0.006f;
int                 D_L10_00161B84 = 1;
int                 D_L10_00161B80 = 1;
char                D_L10_00161BA0 = 0;
char                D_L10_00161BB0 = 0;
char D_L10_001D91B0 NOT_SDA = 0;
char D_L10_001D91C0 NOT_SDA = 0;
float               D_L10_00161BC0 = 0;
float               D_L10_00161BC4 = 0;
int                 D_L10_00161B94 = 0;
int                 D_L10_00161B98 = 0;
char D_L11_0021B2B8 NOT_SDA = 0x63;
char                D_L11_00161D60[1] = {-37};

u32 D_L11_00161D50[1] = {0xbfc90fdb};

I6 D_L11_0021B3D0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L11_0021B3E8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L11_001D8D30 NOT_SDA = 0;
char D_L11_001D8E70 NOT_SDA = 0;
float               D_L11_00161D80 = 0.006f;
int                 D_L11_00161D74 = 1;
int                 D_L11_00161D70 = 1;
char                D_L11_00161D90 = 0;
char                D_L11_00161DA0 = 0;
char D_L11_001D8D80 NOT_SDA = 0;
char D_L11_001D8D90 NOT_SDA = 0;
float               D_L11_00161DB0 = 0;
float               D_L11_00161DB4 = 0;
int                 D_L11_00161D84 = 0;
int                 D_L11_00161D88 = 0;
char D_L12_0020C758 NOT_SDA = 0x63;
char                D_L12_00161C80[1] = {-37};

u32 D_L12_00161C70[1] = {0xbfc90fdb};

I6 D_L12_0020C870 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L12_0020C888 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L12_001FA700 NOT_SDA = 0;
char D_L12_001FA840 NOT_SDA = 0;
float               D_L12_00161CA0 = 0.006f;
int                 D_L12_00161C94 = 1;
int                 D_L12_00161C90 = 1;
char                D_L12_00161CB0 = 0;
char                D_L12_00161CC0 = 0;
char D_L12_001FA750 NOT_SDA = 0;
char D_L12_001FA760 NOT_SDA = 0;
float               D_L12_00161CD0 = 0;
float               D_L12_00161CD4 = 0;
int                 D_L12_00161CA4 = 0;
int                 D_L12_00161CA8 = 0;
char D_L13_001F5318 NOT_SDA = 0x63;
char                D_L13_00161CC0[1] = {-37};

u32 D_L13_00161CB0[1] = {0xbfc90fdb};

I6 D_L13_001F5430 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L13_001F5448 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L13_001D8D60 NOT_SDA = 0;
char D_L13_001D8EA0 NOT_SDA = 0;
float               D_L13_00161CE0 = 0.006f;
int                 D_L13_00161CD4 = 1;
int                 D_L13_00161CD0 = 1;
char                D_L13_00161CF0 = 0;
char                D_L13_00161D00 = 0;
char D_L13_001D8DB0 NOT_SDA = 0;
char D_L13_001D8DC0 NOT_SDA = 0;
float               D_L13_00161D10 = 0;
float               D_L13_00161D14 = 0;
int                 D_L13_00161CE4 = 0;
int                 D_L13_00161CE8 = 0;
char D_L14_001FCC68 NOT_SDA = 0x63;
char                D_L14_00161EC0[1] = {-37};

u32 D_L14_00161EB0[1] = {0xbfc90fdb};

I6 D_L14_001FCD80 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L14_001FCD98 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L14_001E5210 NOT_SDA = 0;
char D_L14_001E5350 NOT_SDA = 0;
float               D_L14_00161EE0 = 0.006f;
int                 D_L14_00161ED4 = 1;
int                 D_L14_00161ED0 = 1;
char                D_L14_00161EF0 = 0;
char                D_L14_00161F00 = 0;
char D_L14_001E5260 NOT_SDA = 0;
char D_L14_001E5270 NOT_SDA = 0;
float               D_L14_00161F10 = 0;
float               D_L14_00161F14 = 0;
int                 D_L14_00161EE4 = 0;
int                 D_L14_00161EE8 = 0;
char D_L15_001E3318 NOT_SDA = 0x63;
char                D_L15_00161E60[1] = {-37};

u32 D_L15_00161E50[1] = {0xbfc90fdb};

I6 D_L15_001E3430 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L15_001E3448 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L15_001D8730 NOT_SDA = 0;
char D_L15_001D8870 NOT_SDA = 0;
float               D_L15_00161E80 = 0.006f;
int                 D_L15_00161E74 = 1;
int                 D_L15_00161E70 = 1;
char                D_L15_00161E90 = 0;
char                D_L15_00161EA0 = 0;
char D_L15_001D8780 NOT_SDA = 0;
char D_L15_001D8790 NOT_SDA = 0;
float               D_L15_00161EB0 = 0;
float               D_L15_00161EB4 = 0;
int                 D_L15_00161E84 = 0;
int                 D_L15_00161E88 = 0;
char D_L16_001E83A8 NOT_SDA = 0x63;
char                D_L16_00161BE0[1] = {-37};

u32 D_L16_00161BD0[1] = {0xbfc90fdb};

I6 D_L16_001E84C0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L16_001E84D8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L16_001D7C50 NOT_SDA = 0;
char D_L16_001D7D90 NOT_SDA = 0;
float               D_L16_00161C00 = 0.006f;
int                 D_L16_00161BF4 = 1;
int                 D_L16_00161BF0 = 1;
char                D_L16_00161C10 = 0;
char                D_L16_00161C20 = 0;
char D_L16_001D7CA0 NOT_SDA = 0;
char D_L16_001D7CB0 NOT_SDA = 0;
float               D_L16_00161C30 = 0;
float               D_L16_00161C34 = 0;
int                 D_L16_00161C04 = 0;
int                 D_L16_00161C08 = 0;
char D_L17_001E6AC8 NOT_SDA = 0x63;
char                D_L17_00161D50[1] = {-37};

u32 D_L17_00161D40[1] = {0xbfc90fdb};

I6 D_L17_001E6BE0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L17_001E6BF8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L17_001D8A50 NOT_SDA = 0;
char D_L17_001D8B90 NOT_SDA = 0;
float               D_L17_00161D70 = 0.006f;
int                 D_L17_00161D64 = 1;
int                 D_L17_00161D60 = 1;
char                D_L17_00161D80 = 0;
char                D_L17_00161D90 = 0;
char D_L17_001D8AA0 NOT_SDA = 0;
char D_L17_001D8AB0 NOT_SDA = 0;
float               D_L17_00161DA0 = 0;
float               D_L17_00161DA4 = 0;
int                 D_L17_00161D74 = 0;
int                 D_L17_00161D78 = 0;
char D_L18_001F27F8 NOT_SDA = 0x63;
char                D_L18_00161E70[1] = {-37};

u32 D_L18_00161E60[1] = {0xbfc90fdb};

I6 D_L18_001F2910 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

I6 D_L18_001F2928 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

char D_L18_001D8D90 NOT_SDA = 0;
char D_L18_001D8ED0 NOT_SDA = 0;
float               D_L18_00161E90 = 0.006f;
int                 D_L18_00161E84 = 1;
int                 D_L18_00161E80 = 1;
char                D_L18_00161EA0 = 0;
char                D_L18_00161EB0 = 0;
char D_L18_001D8DE0 NOT_SDA = 0;
char D_L18_001D8DF0 NOT_SDA = 0;
float               D_L18_00161EC0 = 0;
float               D_L18_00161EC4 = 0;
int                 D_L18_00161E94 = 0;
int                 D_L18_00161E98 = 0;

/* False in modes 0, 1, 5, 12, and in modes 2 and 4 unless the state is 0x3C. */
int is_suck_cannon_suction_blocked_by_hero_state(void) __asm__("FUN_L00_002d9f78");

int is_suck_cannon_suction_blocked_by_hero_state(void) {
    int suction_blocked = 1;
    switch (hero.state.control_mode) {
    case 0:
    case 1:
    case 5:
    case 12:
        suction_blocked = 0;
        break;
    case 2:
    case 4:
        if (hero.state.current != 0x3C)
            suction_blocked = 0;
        break;
    }
    return suction_blocked;
}

/* Composes the 0xC0 matrix of a with the one built from c into b+0xC0 and flags b for update. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DB480), where it is exact; names translated to the US level program. */
void compose_moby_rotation_with_euler_offset(char *source_moby,
                                             char *destination_moby,
                                             void *euler_angles) __asm__("FUN_L00_002d9fd0");

void compose_moby_rotation_with_euler_offset(char *source_moby,
                                             char *destination_moby,

                                             void *euler_angles) {
    V euler_rotation, source_rotation, composed_rotation;
    euler_rotation_matrix(&euler_rotation, euler_angles);
    copy_matrix_rotation(&source_rotation, source_moby + 0xC0);
    multiply_matrix(&composed_rotation, &source_rotation, &euler_rotation);
    copy_matrix3x4(destination_moby + 0xC0, &composed_rotation);
    MOBY(destination_moby)->flags |= 4;
    refresh_moby_spatial_bounds_from_basis(destination_moby);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DB508), where it is exact; names translated to the US level program. */
int process_suck_cannon_suction_candidate(u8   *cannon_moby,
                                          void *cannon_data,
                                          void *m_,
                                          void *r_,
                                          void *suction_origin) __asm__("FUN_L00_002da058");

int process_suck_cannon_suction_candidate(

    u8 *cannon_moby, void *cannon_data, void *m_, void *r_, void *suction_origin) {
    char *candidate_moby = (char *)m_;
    char *suction_record = (char *)r_;
    int   suction_callback_result = 0;
    int (*fn)(void *, void *, void *) =
        **(int (***)(void *, void *, void *))(((char *)MOBY(candidate_moby)->pclass) + 0x2C);
    char *x;
    int   c;
    int   occupied_slot_count;
    int   i;
    if (fn != 0) {
        suction_callback_result = fn(candidate_moby, suction_origin, cannon_moby);
        if (suction_callback_result == 2 && *(short *)(suction_record + 0x68) != 3) {
            occupied_slot_count = hero.sucked_moby_count + hero.suck_cannon_incoming;
            if (D_0013E529 == 0 ? occupied_slot_count < 5 : occupied_slot_count < 10) {
                x = *(char **)(suction_record + 0x70);
                if (((unsigned char *)candidate_moby)[0x53] != (unsigned char)x[3]) {
                    blend_moby_animation(
                        candidate_moby, (unsigned char)x[3], 0, scale_game_frames(10));
                }
                *(short *)(suction_record + 0x68) = 3;
                *(int *)(suction_record + 0x6C) = 0;
                MOBY(candidate_moby)->anim_speed = 1.0f;
                *(float *)(suction_record + 0x64) =
                    vector_distance_xy(candidate_moby + 0x10, suction_origin);
                for (i = 0; i < (D_0013E529 == 0 ? 5 : 10); i++) {
                    G_2db508 *gg = (G_2db508 *)&hero;
                    if (suck_cannon_moby_slots[i] == 0) {
                        suck_cannon_moby_slots[i] = candidate_moby;
                        *(short *)(suction_record + 0x6A) = i;
                        gg->i207C++;
                        break;
                    }
                }
            }
        }
    }
    return suction_callback_result;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DB690), where it is exact; names translated to the US level program. */

/* Calls the optional function reached through the moby pointer at +0x24 (slot 0xC); 1 if null. */
int dispatch_suck_cannon_suction_cancel(struct Moby *moby) __asm__("FUN_L00_002da1e0");

int dispatch_suck_cannon_suction_cancel(struct Moby *moby) {
    int (*fn)(char *) = *(int (**)(char *))(*(char **)(((char *)moby->pclass) + 0x2C) + 0xC);
    if (fn != 0) {
        return fn(moby);
    }
    return 1;
}

void compute_suck_cannon_suction_aim_and_origin(
    struct Moby *cannon_moby,
    f32         *out_suction_yaw,
    f32         *out_suction_pitch,
    f32         *out_suction_origin) __asm__("FUN_L00_002da250");

void compute_suck_cannon_suction_aim_and_origin(struct Moby *cannon_moby,
                                                f32         *out_suction_yaw,
                                                f32         *out_suction_pitch,

                                                f32 *out_suction_origin) {
    moby_point_position(cannon_moby, 0, out_suction_origin);
    if (hero.state.current == 1 || hero.state.current == 0x1E) {
        V_002da250   suction_origin_offset;
        LevelCamera *suction_aim_camera = &l00_camera_lui;
        suction_origin_offset.y = 0;
        suction_origin_offset.x = 1.2f;
        *out_suction_yaw = suction_aim_camera->yaw;
        suction_origin_offset.z = 0;
        *out_suction_pitch = -suction_aim_camera->pitch;
        transform_vector_by_basis(
            &suction_origin_offset, &suction_origin_offset, suction_aim_camera->unk350);
        out_suction_origin[2] = suction_origin_offset.z + cannon_moby->pos.z;
        out_suction_origin[0] = suction_origin_offset.x + hero.motion.pos.f[0];
        out_suction_origin[1] = suction_origin_offset.y + hero.motion.pos.f[1];
    } else if (hero.unk22C8 == 1) {
        *out_suction_yaw = hero.held_pose_yaw;
        *out_suction_pitch = cannon_moby->rot.y;
    } else {
        *out_suction_yaw = hero.held_pose_yaw;
        *out_suction_pitch = hero.basis_pitch;
    }
}

void update_suck_cannon_suction(u8 *cannon_moby, void *cannon_data) __asm__("FUN_L00_002da360");

void update_suck_cannon_suction(u8 *cannon_moby, void *cannon_data) {
    u8       cannon_suction_origin[0x10] __attribute__((aligned(16)));
    V_da360  raised_candidate_position;
    f32      suction_yaw, suction_pitch;
    M_da360 *candidate_moby;
    R       *suction_record;
    f32      distance_xy, thr, target_pitch_or_angle_difference;
    s32      stored_moby_count;

    FUN_L00_002da250_c(cannon_moby, &suction_yaw, &suction_pitch, cannon_suction_origin);
    suck_beam_update(cannon_moby, cannon_suction_origin);
    for (candidate_moby = D_L00_0015FFE4; candidate_moby != 0;
         candidate_moby = candidate_moby->next) {
        if (candidate_moby->a6 != 0 && (candidate_moby->f34 & 0x1000) &&
            *(void **)*(u8 **)(candidate_moby->p24 + 0x2c) != (void *)FUN_L00_002db848) {
            suction_record = FUN_L00_002db890_c(candidate_moby);
            if (suction_record != 0 && suction_record->h68 < 6) {
                distance_xy = vector_distance_xy(cannon_moby + 0x10, &candidate_moby->pos);
                if (distance_xy < 15.0f && !is_suck_cannon_suction_blocked_by_hero_state() &&
                    ((stored_moby_count = hero.sucked_moby_count),
                     G1->b9 == 0 ? stored_moby_count < 5 : stored_moby_count < 10)) {
                    if (suction_record->h68 < 3) {
                        thr = 0.043864909559488297f;
                        if (distance_xy < 3.0f)
                            thr = 3.68587064743042f;
                        suction_record->h6a = -1;
                        qcopy(&raised_candidate_position, &candidate_moby->pos);
                        raised_candidate_position.z += 0.4f;
                        target_pitch_or_angle_difference = fast_difference_between_rotations(
                            suction_yaw,
                            fast_arc_tan(raised_candidate_position.x - MOBY(cannon_moby)->pos.x,
                                         raised_candidate_position.y - MOBY(cannon_moby)->pos.y));
                        if (target_pitch_or_angle_difference * target_pitch_or_angle_difference <
                            thr) {
                            target_pitch_or_angle_difference = fast_arc_tan(
                                distance_xy,
                                raised_candidate_position.z - MOBY(cannon_moby)->pos.z);
                            target_pitch_or_angle_difference = fast_difference_between_rotations(
                                suction_pitch, target_pitch_or_angle_difference);
                            if (target_pitch_or_angle_difference *
                                    target_pitch_or_angle_difference <
                                thr) {
                                if (!FUN_001efa68_c(cannon_suction_origin,
                                                    &raised_candidate_position,
                                                    2,
                                                    candidate_moby,
                                                    0))
                                    goto hit;
                                if (suction_record->h68 > 0)
                                    FUN_L00_002da1e0_c(candidate_moby);
                            } else if (suction_record->h68 > 0)
                                FUN_L00_002da1e0_c(candidate_moby);
                        } else if (suction_record->h68 > 0)
                            FUN_L00_002da1e0_c(candidate_moby);
                    } else if (suction_record->h68 < 4) {
                    hit:
                        process_suck_cannon_suction_candidate(cannon_moby,
                                                              cannon_data,
                                                              candidate_moby,
                                                              suction_record,
                                                              cannon_suction_origin);
                    }
                } else if (suction_record->h68 != 0)
                    FUN_L00_002da1e0_c(candidate_moby);
            }
        } else {
            FUN_L00_002df1e0_c(candidate_moby);
        }
    }
}

void refresh_suck_cannon_slots(void) __asm__("FUN_L00_002da660");

void refresh_suck_cannon_slots(void) {
    s32 i;
    ((G_002da660 *)&hero)->i207C = 0;
    ((G_002da660 *)&hero)->i2078 = 0;
    for (i = 0; i < (item_text_variant[9] == 0 ? 5 : 10); i++) {
        u8 *suction_record;
        s32 k;
        if (((G_002da660 *)&hero)->slot[i] == 0)
            continue;
        suction_record = FUN_L00_002db890_002da660(((G_002da660 *)&hero)->slot[i]);
        if (suction_record == 0 || (k = *(s16 *)(suction_record + 0x68)) < 3 || k >= 5 ||
            *(s16 *)(suction_record + 0x6A) == -1) {
            u8 *slot_moby = ((G_002da660 *)&hero)->slot[i];
            FUN_001e93b0_002da660(
                D_L00_001E9FE8, ((s16)MOBY(slot_moby)->save_id), MOBY(slot_moby)->oclass);
            ((G_002da660 *)&hero)->slot[i] = 0;
        } else if (k == 3) {
            ((G_002da660 *)&hero)->i207C++;
        } else if (k == 4) {
            ((G_002da660 *)&hero)->i2078++;
        }
    }
}

void suckcannon_update(SuckMoby *self) __asm__("FUN_L00_002da790");

void suckcannon_update(SuckMoby *self) {
    enum {
        SUCKCANNON_STATE_INITIALIZE = 0,
        SUCKCANNON_STATE_EMPTY_IDLE = 1,
        SUCKCANNON_STATE_SUCTION_STARTUP = 2,
        SUCKCANNON_STATE_SUCTION_ACTIVE = 3,
        SUCKCANNON_STATE_LOADED_AIM = 4,
        SUCKCANNON_STATE_FIRE_STORED_MOBY = 5
    };
    SuckWork    work;
    SuckCannon *cannon;
    SuckStats  *stats;
    int         slot_released, i, sound, next_state;
    SuckMoby   *stored;

    queue_animation_update(4,
                           0x753F,
                           (int)suck_delete_callback,
                           (int)update_hud_counter_cap_and_visibility,
                           (int)hud_draw_gauge_frame,
                           (int)&((SuckHero *)&hero)->count,
                           5);
    cannon = self->data;
    slot_released = ((SuckHero *)&hero)->slot_state == 3;
    if ((u8)self->state != 3 && (u8)self->state != 0)
        suck_beam_queue_draw((int)self);
    refresh_suck_cannon_slots();
    switch ((u8)self->state) {
    case SUCKCANNON_STATE_INITIALIZE:
        for (i = 0; i < (item_text_variant[9] == 0 ? 5 : 10); ++i) {
            if (((SuckHero *)&hero)->stored[i]) {
                ((SuckHero *)&hero)->stored[i]->collision = 0;
                ((SuckHero *)&hero)->stored[i]->flags &= ~0x1000;
            }
        }
        ((SuckHero *)&hero)->firing = 0;
        if (((SuckHero *)&hero)->count > 0) {
            cannon->blocked = 1;
            blend_moby_animation(self, 6, 0, scale_game_frames(4));
            self->state = 4;
        } else {
            cannon->cooldown = 0;
            self->state = 1;
        }
        suck_reset(self);
        break;
    case SUCKCANNON_STATE_EMPTY_IDLE:
        if (self->anim_flags & 2)
            blend_moby_animation(self, 1, 0, scale_game_frames(0));
        fast_dec_timer(&cannon->cooldown);
        if (cannon->cooldown == 0)
            cancel_hero_item_animation();
        if (((SuckHero *)&hero)->count > 0) {
            blend_moby_animation(self, 6, 0, scale_game_frames(4));
            self->state = 4;
            break;
        }
        if ((controller_state.held & ((SuckHero *)&hero)->fire_buttons) && cannon->cooldown == 0 &&
            ((SuckHero *)&hero)->unk20A7 == 0 && ((SuckHero *)&hero)->unk20AC == 0 &&
            scale_game_frames(15) < ((SuckHero *)&hero)->ready_frames &&
            is_suck_cannon_suction_blocked_by_hero_state() == 0) {
            start_equipped_item_animation();
            cannon->sound = allocate_voice_for_target_entry(2, 4, (int)self);
            cannon->cooldown = scale_game_frames(10);
            stats = (SuckStats *)(((char *)&item_usage));
            if (stats->count < 65535)
                stats->count++;
            if (scale_game_frames(D_0015EEA4) / 600 > stats->longest)
                stats->longest = scale_game_frames(D_0015EEA4) / 600;
            stats->level_mask = (stats->level_mask | (1 << current_level_index)) | 0x80000000;
            self->state = 2;
        }
        break;
    case SUCKCANNON_STATE_SUCTION_STARTUP:
        if (self->anim_flags & 2)
            blend_moby_animation(self, 3, 0, scale_game_frames(5));
        if (is_suck_cannon_suction_blocked_by_hero_state() != 0 || ((SuckHero *)&hero)->unk20AC ||
            ((SuckHero *)&hero)->unk20A7) {
            sound = cannon->sound;
            if (sound != -1) {
                SuckVoice *s = (SuckVoice *)(((char *)&voice_pool)) + sound;
                if (s[1].owner == self && s[1].active)
                    release_voice_slot(sound);
            }
            cannon->sound = -1;
            cancel_hero_item_animation();
            ((SuckHero *)&hero)->firing = 0;
            if (((SuckHero *)&hero)->count > 0) {
                blend_moby_animation(self, 6, 0, scale_game_frames(4));
                next_state = 4;
                goto changed;
            } else {
                cannon->cooldown = 0;
                blend_moby_animation(self, 1, 0, scale_game_frames(4));
                next_state = 1;
                goto changed;
            }
        } else if (fast_dec_timer(&cannon->cooldown)) {
            cannon->cooldown = scale_game_frames(20);
            next_state = 3;
            goto changed;
        }
        break;
    case SUCKCANNON_STATE_SUCTION_ACTIVE:
        if (self->anim_state != 3 && (self->anim_state != 0 || (self->anim_flags & 2)))
            blend_moby_animation(self, 3, 0, scale_game_frames(5));
        fast_dec_timer(&cannon->cooldown);
        if (!(((SuckHero *)&hero)->sucking ||
              (((SuckHero *)&hero)->count < (item_text_variant[9] == 0 ? 5 : 10) &&
               is_suck_cannon_suction_blocked_by_hero_state() == 0 &&
               ((controller_state.held & ((SuckHero *)&hero)->fire_buttons) || cannon->cooldown) &&
               !((SuckHero *)&hero)->unk20AC && !((SuckHero *)&hero)->unk20A7))) {
            cancel_hero_item_animation();
            sound = cannon->sound;
            if (sound != -1) {
                SuckVoice *s = (SuckVoice *)(((char *)&voice_pool)) + sound;
                if (s[1].owner == self && s[1].active)
                    release_voice_slot(sound);
            }
            cannon->sound = -1;
            stop_suck_cannon_suction_effect((int)self);
            ((SuckHero *)&hero)->firing = 0;
            if (((SuckHero *)&hero)->count > 0) {
                blend_moby_animation(self, 6, 0, scale_game_frames(4));
                cannon->cooldown = 0;
                cannon->blocked = 1;
                self->state = 4;
                goto done;
            } else {
                cannon->cooldown = 0;
                blend_moby_animation(self, 1, 0, scale_game_frames(4));
                self->state = 1;
                goto done;
            }
        } else {
            if (self->anim_flags & 2)
                blend_moby_animation(self, 3, 0, scale_game_frames(2));
            update_suck_cannon_suction((u8 *)self, cannon);
        }
        break;
    case SUCKCANNON_STATE_LOADED_AIM:
        fast_dec_timer(&cannon->cooldown);
        if (!cannon->cooldown)
            cancel_hero_item_animation();
        if (self->anim_flags & 2) {
            blend_moby_animation(self, 6, 0, scale_game_frames(0));
            cancel_hero_item_animation();
        }
        if (!(controller_state.held & ((SuckHero *)&hero)->fire_buttons))
            cannon->blocked = 0;
        if (fast_dec_timer(&cannon->target_timer) ||
            (cannon->target && ((u8)cannon->target->state & 0x80)))
            cannon->target = 0;
        if (((SuckHero *)&hero)->state == 1)
            hero_set_state(30, 1);
        if (((SuckHero *)&hero)->state == 30 && (((SuckHero *)&hero)->moby->flags & 1) &&
            !((SuckHero *)&hero)->unk20AC) {
            LevelCamera *camera = &l00_camera;
            SuckVec     *eye;
            SuckHit     *hit;
            work.aim.velocity.q = 0;
            work.aim.velocity.f.z = 50.0f;
            copy_matrix3x4(&work.aim.rotation, camera->unk0);
            eye = (SuckVec *)&camera->pos;
            transpose_rotation_matrix(&work.aim.inverse, &work.aim.rotation);
            transform_vector(&work.aim.velocity, &work.aim.velocity, &work.aim.inverse);
            fast_vec_add(&work.aim.end, eye, &work.aim.velocity);
            if (collision_segment_hits(eye, &work.aim.end, 0, (int)self, 0)) {
                hit = (SuckHit *)D_L00_00173E40_lui;
                if (hit->moby && (hit->moby->flags & 0x1000)) {
                    cannon->target = hit->moby;
                    cannon->target_timer = scale_game_frames(20);
                    cannon->target_height = hit->pos.f.z - cannon->target->pos.f.z;
                }
            }
            if (cannon->target && !cannon->cooldown) {
                qcopy(&work.aim.point, &cannon->target->pos);
                work.aim.point.f.z += cannon->target_height;
                suck_draw_reticle((int)self, 0xFF0FFF0F, 1.0f, 0.0f, 0, 0x21, -1, 90.0f, 4);
            } else {
                suck_draw_reticle((int)self, 0xFF0F0FFF, 1.0f, 0.0f, 0, 0x21, -1, 90.0f, 4);
            }
        }
        if (((SuckHero *)&hero)->aim_locked) {
            cannon->target = ((SuckHero *)&hero)->aim_target;
            cannon->target_timer = scale_game_frames(20);
            cannon->target_height = collision_hit_z - cannon->target->pos.f.z;
        }
        if (!cannon->blocked && ((SuckHero *)&hero)->count > 0 && !cannon->cooldown &&
            (controller_state.held & ((SuckHero *)&hero)->fire_buttons) &&
            ((SuckHero *)&hero)->control_mode != 7 && ((SuckHero *)&hero)->control_mode != 3 &&
            ((SuckHero *)&hero)->control_mode != 18 && ((SuckHero *)&hero)->control_mode != 17 &&
            !((SuckHero *)&hero)->unk20AC && !((SuckHero *)&hero)->unk20A7 &&
            scale_game_frames(22) < ((SuckHero *)&hero)->ready_frames) {
            start_equipped_item_animation();
            blend_moby_animation(self, 5, 0, scale_game_frames(2));
            ((SuckHero *)&hero)->firing = 2;
            cannon->cooldown = scale_game_frames(25);
            stats = (SuckStats *)(((char *)&item_usage));
            if (stats->count < 65535)
                stats->count++;
            if (scale_game_frames(D_0015EEA4) / 600 > stats->longest)
                stats->longest = scale_game_frames(D_0015EEA4) / 600;
            stats->level_mask = (stats->level_mask | (1 << current_level_index)) | 0x80000000;
            self->state = 5;
        }
        break;
    case 6:
        break;
    case SUCKCANNON_STATE_FIRE_STORED_MOBY:
        stored = 0;
        if (!(0.0f <= compute_interpolated_record_value(self)))
            break;
        for (i = 0; i < (item_text_variant[9] == 0 ? 5 : 10); i++) {
            if (((SuckHero *)&hero)->stored[i]) {
                stored = ((SuckHero *)&hero)->stored[i];
                ((SuckHero *)&hero)->stored[i] = 0;
                break;
            }
        }
        if (stored) {
            SuckMoby *best, *candidate_target;
            float     yaw, pitch, limit;
            work.fire.velocity.q = 0;
            copy_matrix3x4(&work.fire.rotation, l00_camera.unk0);
            transpose_rotation_matrix(&work.fire.inverse, &work.fire.rotation);
            if (((SuckHero *)&hero)->state == 30 && (((SuckHero *)&hero)->moby->flags & 1)) {
                work.fire.offset.q = 0;
                work.fire.offset.f.z = frame_scale * 0.4462f;
                work.fire.point.q = work.fire.offset.q;
                transform_vector(&work.fire.velocity, &work.fire.point, &work.fire.inverse);
                work.fire.offset.q = 0;
                work.fire.offset.f.x = 0.15f;
                work.fire.offset.f.y = 0.3f;
                work.fire.offset.f.z = 1.0f;
                transform_vector(&work.fire.launch, &work.fire.offset, &work.fire.inverse);
                fast_vec_add(&work.fire.launch, &work.fire.launch, &l00_camera.pos);
            } else {
                work.fire.offset.q = 0;
                work.fire.offset.f.x = frame_scale * 0.4462f;
                work.fire.point.q = work.fire.offset.q;
                transform_vector_by_basis(
                    &work.fire.velocity, &work.fire.point, &((SuckHero *)&hero)->basis);
                FUN_L00_0024f7c8(self, 0, &work.fire.launch);
            }
            qcopy(&stored->pos, &work.fire.launch);
            refresh_moby_spatial_bounds_from_basis(stored);
            fast_vec_add(
                &work.fire.velocity, &work.fire.velocity, &((SuckHero *)&hero)->displacement);
            if (((SuckHero *)&hero)->state != 30 || !(((SuckHero *)&hero)->moby->flags & 1)) {
                limit = 10000.0f;
                best = 0;
                if (((SuckHero *)&hero)->aim_locked)
                    yaw = ((SuckHero *)&hero)->aim_yaw;
                else
                    yaw = fast_arc_tan(work.fire.velocity.f.x, work.fire.velocity.f.y);
                pitch =
                    -fast_arc_tan(vector_length_xy(&work.fire.velocity), work.fire.velocity.f.z);
                candidate_target = suck_moby_list;
                while (candidate_target) {
                    if (candidate_target->state >= 0 && candidate_target != stored) {
                        float          tyaw, tpitch, dyaw, distance, offangle;
                        SuckAimHeight *candidate_aim_height;
                        qcopy(&work.fire.point, &candidate_target->pos);
                        candidate_aim_height = suck_get_aim_height(candidate_target);
                        if (candidate_aim_height)
                            work.fire.point.f.z += candidate_aim_height->height;
                        else
                            work.fire.point.f.z += 0.5f;
                        if ((candidate_target->flags & 0x1000) && candidate_target &&
                            candidate_target->cls && candidate_target->cls->category == 5) {
                            tyaw = fast_arc_tan(work.fire.point.f.x - work.fire.launch.f.x,
                                                work.fire.point.f.y - work.fire.launch.f.y);
                            tpitch = fast_arc_tan(
                                vector_distance_xy(&work.fire.launch, &work.fire.point),
                                work.fire.point.f.z - work.fire.launch.f.z);
                            dyaw = fast_difference_between_rotations(yaw, tyaw);
                            distance = fast_vec_dist(&work.fire.launch, &work.fire.point);
                            if (distance < 2.5f &&
                                fast_difference_between_rotations(
                                    ((SuckHero *)&hero)->yaw,
                                    fast_arc_tan(
                                        candidate_target->pos.f.x - ((SuckHero *)&hero)->pos.f.x,
                                        candidate_target->pos.f.y - ((SuckHero *)&hero)->pos.f.y)) <
                                    1.0471976f &&
                                absolute_float(tpitch) < 0.7853982f) {
                                best = candidate_target;
                                break;
                            }
                            if (!(limit < distance)) {
                                float delta;
                                build_spherical_offset(&work.fire.offset, distance, yaw, pitch);
                                fast_vec_add(
                                    &work.fire.offset, &work.fire.offset, &work.fire.launch);
                                delta = fast_vec_dist(&work.fire.offset, &work.fire.point);
                                offangle =
                                    1.5707964f -
                                    fast_arc_sin(1.0f - delta * delta /
                                                            ((distance + distance) * distance));
                                if (0.4537856f < offangle) {
                                    float radius = candidate_target->radius * 0.0009765625f;
                                    float adjustment =
                                        1.5707964f -
                                        fast_arc_sin(distance / square_root(distance * distance +
                                                                            radius * radius));
                                    if (radius < distance)
                                        offangle -= adjustment;
                                }
                                if (offangle < 0.4537856f) {
                                    if (collision_segment_hits(
                                            D_L00_00166DC0, &work.fire.point, 6, (int)self, 0))
                                        break;
                                    limit = distance;
                                    pitch = -tpitch;
                                    if (dyaw < 0.17453292f) {
                                        yaw = tyaw;
                                        best = candidate_target;
                                    }
                                }
                            }
                        }
                    }
                    candidate_target = candidate_target->next;
                }
                if (best)
                    cannon->target = best;
            }
            compose_moby_rotation_with_euler_offset((char *)self, (char *)stored, suck_fire_tuning);
            if (stored->cls->handlers->fire)
                stored->cls->handlers->fire(
                    stored, &work.fire.velocity, cannon->target, cannon->target_height);
            ((SuckHero *)&hero)->count--;
            if (((SuckHero *)&hero)->count > 0) {
                ((SuckHero *)&hero)->firing = 0;
                self->state = 4;
                goto done;
            } else {
                cannon->cooldown = scale_game_frames(20);
                next_state = 1;
                ((SuckHero *)&hero)->firing = 0;
                self->state = next_state;
                goto done;
            }
        }
        break;
    }
    goto done;
changed:
    self->state = next_state;
done:
    if (slot_released) {
        sound = cannon->sound;
        if (sound != -1) {
            SuckVoice *s = (SuckVoice *)(((char *)&voice_pool)) + sound;
            if (s[1].owner == self && s[1].active)
                release_voice_slot(sound);
        }
        cannon->sound = -1;
        if (controller_state.held & 5)
            controller_state.released |= 5;
    }
}

/* Calls the class function at +0x10 for a moby not in state 0xFE/0xFD with data; else 0. */
s32 get_moby_suck_cannon_record(Obj *moby) __asm__("FUN_L00_002db890");

s32 get_moby_suck_cannon_record(Obj *moby) {
    Fn_db890 class_callback;
    if (moby == 0 || moby->f20 == 0xFE)
        return 0;
    if (moby->f20 == 0xFD || moby->f78 == 0 || moby->f24 == 0)
        return 0;
    class_callback = moby->f24->f2C->f10;
    if (class_callback)
        return class_callback(moby);
    return 0;
}

s32 process_moby_suck_cannon_suction(u8 *moby,
                                     u8 *position,
                                     u8 *target) __asm__("FUN_L00_002db8f8");

s32 process_moby_suck_cannon_suction(u8 *moby, u8 *position, u8 *target) {
    enum {
        SUCK_DISPATCH_ALIGNING_FOR_SUCTION = 0,
        SUCK_DISPATCH_RISING_FOR_SUCTION = 1,
        SUCK_DISPATCH_BEING_PULLED_IN = 2,
        SUCK_DISPATCH_STORED = 3,
        SUCK_DISPATCH_FLYING_PROJECTILE = 4,
        SUCK_DISPATCH_ROLLING_PROJECTILE = 5,
        SUCK_DISPATCH_FALLING_AFTER_CANCEL = 6
    };
    u8 *record = FUN_L00_002db890_002db8f8(moby);
    s16 state;
    if (record == 0)
        goto missing;
    if (moby == 0 || moby[0x20] == 0xFE || moby[0x20] == 0xFD || *(s16 *)(record + 0x68) == 8)
        goto fail;
    qcopy_nc(record, position);
    *(u8 **)(record + 0x60) = target;
    *(f32 *)(record + 0xC) = 1.0f;
    state = (s16)(*(u16 *)(record + 0x68) - 1);
    switch (state) {
    case SUCK_DISPATCH_ALIGNING_FOR_SUCTION:
        if (1.0f <= *(f32 *)(record + 0x6C)) {
            {
                u8 *seq = *(u8 **)(record + 0x70);
                if (moby[0x53] != seq[2]) {
                    blend_moby_animation(moby, seq[2], 0, scale_game_frames(10));
                }
            }
            *(s32 *)(record + 0x6C) = 0;
            *(s16 *)(record + 0x68) = 2;
            return 1;
        }
        {
            u8 *seq = *(u8 **)(record + 0x70);
            if (moby[0x53] != seq[1]) {
                blend_moby_animation(moby, seq[1], 0, scale_game_frames(10));
            }
        }
        return 1;
    case SUCK_DISPATCH_RISING_FOR_SUCTION:
        if (!(1.0f <= *(f32 *)(record + 0x6C)))
            return 1;
        return 2;
    case SUCK_DISPATCH_BEING_PULLED_IN:
        break;
    case SUCK_DISPATCH_STORED:
        return 3;
    case SUCK_DISPATCH_FLYING_PROJECTILE:
    case SUCK_DISPATCH_ROLLING_PROJECTILE:
    case SUCK_DISPATCH_FALLING_AFTER_CANCEL: {
        u8 *seq = *(u8 **)(record + 0x70);
        if (moby[0x53] != seq[3]) {
            blend_moby_animation(moby, seq[3], 0, scale_game_frames(10));
        }
    }
        *(s16 *)(record + 0x68) = 3;
        *(s32 *)(record + 0x6C) = 0;
        MOBY(moby)->anim_speed = 1.0f;
        *(f32 *)(record + 0x64) = vector_distance_xy(moby + 0x10, target + 0x10);
        break;
    default: {
        u8 *seq = *(u8 **)(record + 0x70);
        if (moby[0x53] != seq[1]) {
            blend_moby_animation(moby, seq[1], 0, scale_game_frames(10));
        }
    }
        *(s32 *)(record + 0x6C) = 0;
        *(s16 *)(record + 0x68) = 1;
        return 1;
    }
    return 2;
missing:
    debug_print(D_L00_001EA030_002db8f8);
fail:
    return 0;
}

s32 store_moby_in_suck_cannon(u8 *moby, OvlQuad *position) __asm__("FUN_L00_002dbb20");

s32 store_moby_in_suck_cannon(u8 *moby, OvlQuad *position) {
    u8          *suction_record = FUN_L00_002db890_002dbb20(moby);
    struct Moby *sound_source_moby;
    if (suction_record == 0)
        return 0;
    if (moby != 0 && moby[0x20] != 0xFE && moby[0x20] != 0xFD &&
        *(s16 *)(suction_record + 0x68) != 8) {
        qcopy(moby + 0x10, position);
        FUN_L00_002d9fd0_002dbb20(
            *(void **)(suction_record + 0x60), moby, suck_cannon_storage_euler_offset);
        refresh_moby_spatial_bounds_from_basis(moby);
        sound_source_moby = D_001403E0_lui;
        if (sound_source_moby != 0 && sound_source_moby->oclass == 0x351) {
            allocate_voice_for_target_entry(5, 0, sound_source_moby);
        }
        mark_moby_for_removal_cf(moby, 0, -1);
        *(s16 *)(suction_record + 0x68) = 4;
        if (moby[0x53] != *(u8 *)(*(u8 **)(suction_record + 0x70) + 4)) {
            u8 stored_animation = *(u8 *)(*(u8 **)(suction_record + 0x70) + 4);
            blend_moby_animation(moby, stored_animation, 0, FUN_001f96f8_002dbb20(10));
        }
        *(s32 *)(moby + 0x2C) = 0;
        return 1;
    }
    return 0;
}

s32 launch_suck_cannon_projectile(u8 *moby,
                                  u8 *launch_velocity,
                                  u8 *target,
                                  f32 target_height_offset) __asm__("FUN_L00_002dbc30");

s32 launch_suck_cannon_projectile(u8 *moby,
                                  u8 *launch_velocity,
                                  u8 *target,

                                  f32 target_height_offset) {
    u8 *projectile_record = FUN_L00_002db890_002dbc30(moby);
    u8 *x;
    u8 *d;
    if (projectile_record == 0)
        return 0;
    if (moby == 0 || moby[0x20] == 0xFE || moby[0x20] == 0xFD ||
        *(s16 *)(projectile_record + 0x68) == 8)
        goto fail;
    moby[0x31] = 1;
    MOBY(moby)->flags &= 0xFFFC;
    if (*(f32 *)(launch_velocity + 0xC) == 1.23456f) {
        if (moby[0x53] != *(u8 *)(*(u8 **)(projectile_record + 0x70) + 6)) {
            u8 c = *(u8 *)(*(u8 **)(projectile_record + 0x70) + 6);
            blend_moby_animation(moby, c, 0, FUN_001f96f8_002dbc30(0x14));
        }
        MOBY(moby)->collision_disable = *(s32 *)(projectile_record + 0x88);
        *(s16 *)(projectile_record + 0x68) = 6;
    } else {
        if (moby[0x53] != *(u8 *)(*(u8 **)(projectile_record + 0x70) + 5)) {
            u8 c = *(u8 *)(*(u8 **)(projectile_record + 0x70) + 5);
            blend_moby_animation(moby, c, 0, FUN_001f96f8_002dbc30(0));
        }
        *(s16 *)(projectile_record + 0x68) = 5;
    }
    qcopy(projectile_record + 0x10, launch_velocity);
    *(s32 *)(projectile_record + 0x94) = FUN_001f96f8_002dbc30(300);
    *(u8 **)(projectile_record + 0x74) = target;
    if (target != 0) {
        x = suck_get_aim_height(target);
        if (x != 0)
            *(u16 *)(x + 0x1E) |= 0x80;
        qcopy(projectile_record + 0x40, target + 0x10);
        *(f32 *)(projectile_record + 0x90) = target_height_offset;
        *(f32 *)(projectile_record + 0x48) += target_height_offset;
    }
    if (vector_distance_xy(moby + 0x10, D_0013F3D0_lui) < 1.0f) {
        FUN_001f9a10_002dbc30(moby + 0x10, moby + 0x10, projectile_record + 0x10);
    }
    if (*(s16 *)(projectile_record + 0x9C) >= 0) {
        allocate_voice_for_target_entry(*(s16 *)(projectile_record + 0x9C), 0, moby);
    }
    return 2;
fail:
    return 0;
}

s32 cancel_suck_cannon_suction(struct Moby *moby) __asm__("FUN_L00_002dbe20");

s32 cancel_suck_cannon_suction(struct Moby *moby) {
    enum {
        SUCK_CANNON_INACTIVE = 0,
        SUCK_CANNON_ALIGNING_FOR_SUCTION = 1,
        SUCK_CANNON_RISING_FOR_SUCTION = 2,
        SUCK_CANNON_BEING_PULLED_IN = 3,
        SUCK_CANNON_STORED = 4,
        SUCK_CANNON_FLYING_PROJECTILE = 5,
        SUCK_CANNON_ROLLING_PROJECTILE = 6,
        SUCK_CANNON_FALLING_AFTER_CANCEL = 7
    };
    u8 *suction_record = FUN_L00_002db890_u(moby);
    s32 k;
    if (suction_record == 0)
        return 0;
    if (moby == 0 || moby->state == 0xFE || moby->state == 0xFD)
        return 0;
    k = *(s16 *)(suction_record + 0x68);
    if (k == 8)
        return 0;
    switch (k) {
    case SUCK_CANNON_BEING_PULLED_IN:
        if (*(s16 *)(suction_record + 0x6A) != -1) {
            hero.unk2050[*(s16 *)(suction_record + 0x6A)] = 0;
            if (hero.suck_cannon_incoming > 0)
                hero.suck_cannon_incoming--;
        }
        /* fall through */
    case SUCK_CANNON_ALIGNING_FOR_SUCTION:
    case SUCK_CANNON_RISING_FOR_SUCTION: {
        if (moby->prev_seq != *(u8 *)(*(u8 **)(suction_record + 0x70) + 7)) {
            u8 cancel_animation = *(u8 *)(*(u8 **)(suction_record + 0x70) + 7);
            blend_moby_animation(moby, cancel_animation, 0, scale_game_frames(10));
        }
        *(s16 *)(suction_record + 0x68) = 7;
        return 0;
    }
    case SUCK_CANNON_STORED:
        return 1;
    case SUCK_CANNON_INACTIVE:
    case SUCK_CANNON_FLYING_PROJECTILE:
    case SUCK_CANNON_ROLLING_PROJECTILE:
    case SUCK_CANNON_FALLING_AFTER_CANCEL:
    case 9:
        return 0;
    }
    return 0;
}

/* Explosion burst for a moby: debris sprays, sparks, coloured flashes, camera shake and an optional damage area. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DD3D8), where it is exact; names translated to the US level program. */
void spawn_suck_cannon_projectile_explosion(char *self, char *q) __asm__("FUN_L00_002dbf28");

void spawn_suck_cannon_projectile_explosion(char *self, char *projectile_suction_record) {
    B50   blk;
    float to_camera[4];
    float spray_bias_vector[4];
    float spray_axis[4];
    float spray_axis_component[4];
    float spray_vector_or_effect_position[4];
    float random_spray_seed[4];
    I6    first_color_choices;
    I6    second_color_choices;
    int   r0, g0, b0;
    int   r1, g1, b1;
    float variant_effect_scale, camera_distance, near_camera_distance_deficit, sc, dt;
    int   step, i, j, k;

    blk = l00_suck_cannon_explosion_blast_parameters;
    *(u128_DD3D8 *)spray_axis = 0;
    spray_axis[2] = D_0015ED6C + D_0015ED6C;
    i = 0;
    *(u128_DD3D8 *)spray_bias_vector = *(u128_DD3D8 *)spray_axis;
    *(u128_DD3D8 *)spray_axis = 0;
    spray_axis[2] = 1.0f;
    variant_effect_scale = convert_integer_to_float((&D_0013E520)->b9) + 1.0f;
    step = 1;
    if ((&D_0013E520)->b9 == 0)
        step = 2;
    fast_vec_scale(spray_bias_vector, spray_bias_vector, variant_effect_scale);
    fast_vec_sub(to_camera, D_L00_00166DC0, self + 0x10);
    camera_distance = fast_vec_length(to_camera);
    for (; i < 10 / step; i++) {
        *(u128_DD3D8 *)random_spray_seed = 0;
        random_spray_seed[0] = random_float_between(-1.0f, 1.0f);
        random_spray_seed[1] = random_float_between(-1.0f, 1.0f);
        *(u128_DD3D8 *)spray_vector_or_effect_position = *(u128_DD3D8 *)random_spray_seed;
        dt = fast_vec_dot(spray_vector_or_effect_position, spray_axis);
        fast_vec_normalize(spray_axis_component, spray_axis, dt);
        fast_vec_sub(
            spray_vector_or_effect_position, spray_vector_or_effect_position, spray_axis_component);
        fast_vec_normalize(spray_axis_component, spray_axis, random_float_between(0.0f, 1.0f));
        fast_vec_add(
            spray_vector_or_effect_position, spray_vector_or_effect_position, spray_axis_component);
        fast_vec_normalize(spray_vector_or_effect_position,
                           spray_vector_or_effect_position,
                           variant_effect_scale * random_float_between(3.5f, 6.5f) * D_0015ED6C);
        fast_vec_add(
            spray_vector_or_effect_position, spray_vector_or_effect_position, spray_bias_vector);
        {
            int h = scale_game_frames_c(60);
            spawn_debris(self + 0x10,
                         (char *)spray_vector_or_effect_position,
                         random_int_between(h, scale_game_frames_c(120)),
                         0,
                         (&D_0013E520)->b9);
        }
    }
    for (j = 0; j < 4 / step; j++) {
        *(u128_DD3D8 *)random_spray_seed = 0;
        random_spray_seed[0] = random_float_between(-1.0f, 1.0f);
        random_spray_seed[1] = random_float_between(-1.0f, 1.0f);
        *(u128_DD3D8 *)spray_vector_or_effect_position = *(u128_DD3D8 *)random_spray_seed;
        dt = fast_vec_dot(spray_vector_or_effect_position, spray_axis);
        fast_vec_normalize(spray_axis_component, spray_axis, dt);
        fast_vec_sub(
            spray_vector_or_effect_position, spray_vector_or_effect_position, spray_axis_component);
        fast_vec_normalize(spray_axis_component, spray_axis, random_float_between(0.0f, 1.0f));
        fast_vec_add(
            spray_vector_or_effect_position, spray_vector_or_effect_position, spray_axis_component);
        fast_vec_normalize(spray_vector_or_effect_position,
                           spray_vector_or_effect_position,
                           variant_effect_scale * random_float_between(6.5f, 10.0f) * D_0015ED6C);
        fast_vec_add(
            spray_vector_or_effect_position, spray_vector_or_effect_position, spray_bias_vector);
        {
            int h = scale_game_frames_c(60);
            spawn_debris(self + 0x10,
                         (char *)spray_vector_or_effect_position,
                         random_int_between(h, scale_game_frames_c(90)),
                         1,
                         (&D_0013E520)->b9);
        }
    }
    *(u128_DD3D8 *)random_spray_seed = 0;
    random_spray_seed[0] = random_float_between(-1.0f, 1.0f);
    random_spray_seed[1] = random_float_between(-1.0f, 1.0f);
    random_spray_seed[2] = random_float_between(-1.0f, 1.0f);
    *(u128_DD3D8 *)spray_vector_or_effect_position = *(u128_DD3D8 *)random_spray_seed;
    to_camera[2] += camera_distance * 0.5f;
    fast_vec_normalize(spray_vector_or_effect_position,
                       spray_vector_or_effect_position,
                       camera_distance / 5.0f * D_0015ED6C);
    fast_vec_normalize(to_camera, to_camera, (camera_distance + camera_distance) * D_0015ED6C);
    fast_vec_add(spray_vector_or_effect_position, spray_vector_or_effect_position, to_camera);
    clamp_vector_length(
        spray_vector_or_effect_position, spray_vector_or_effect_position, D_0015ED6C * 10.0f);
    {
        int h = scale_game_frames_c(60);
        spawn_debris(self + 0x10,
                     (char *)spray_vector_or_effect_position,
                     random_int_between(h, scale_game_frames_c(90)),
                     1,
                     (&D_0013E520)->b9);
    }
    if (camera_distance < 6.0f) {
        k = truncate_float_to_s32(camera_distance) + 1;
    } else {
        k = 4;
    }
    near_camera_distance_deficit = 0.0f;
    if (camera_distance < 7.0f) {
        near_camera_distance_deficit = 7.0f - camera_distance;
    }
    if (k > 0) {
        int color_effects_remaining = k;
        do {
            float sh = variant_effect_scale * random_float_between(8.0f, 10.0f) * D_0015ED6C -
                       near_camera_distance_deficit * D_0015ED6C;
            int   A, B, r1, color_effect_random_frames;
            float big;
            first_color_choices = l00_suck_cannon_explosion_first_color_choices;
            second_color_choices = l00_suck_cannon_explosion_second_color_choices;
            A = swap_packed_color_channels(first_color_choices.v[random_integer_below(6)],
                                           (&D_0013E520)->b9);
            B = swap_packed_color_channels(second_color_choices.v[random_integer_below(6)],
                                           (&D_0013E520)->b9);
            big = variant_effect_scale * 400000.0f;
            r1 = random_int_between(scale_game_frames_c(15), scale_game_frames_c(20));
            color_effect_random_frames =
                random_int_between(scale_game_frames_c(25), scale_game_frames_c(30));
            spawn_particle(self + 0x10,
                           spray_bias_vector,
                           A,
                           B,
                           big,
                           r1,
                           color_effect_random_frames,
                           0,
                           0,
                           sh);
        } while (--color_effects_remaining);
    }
    if (camera_distance > 9.0f) {
        float size = variant_effect_scale * 4.0f;
        func_L00_002E0CB8_DD3D8(size,
                                self,
                                self + 0x10,
                                spray_bias_vector,
                                scale_game_frames_c(15),
                                0x7F,
                                0x7F,
                                0x7F,
                                0x20);
        r0 = 0x7F;
        g0 = 0x20;
        b0 = 0;
        swap_color_channel_words(&r0, &g0, &b0, (&D_0013E520)->b9);
        func_L00_002E0CB8_DD3D8(
            size, self, self + 0x10, spray_bias_vector, scale_game_frames_c(24), r0, g0, b0, 0x20);
    }
    r1 = 0x7F;
    g1 = 0x3F;
    b1 = 0;
    swap_color_channel_words(&r1, &g1, &b1, (&D_0013E520)->b9);
    {
        float s = variant_effect_scale * 4.0f;
        func_L00_002E0CB8_DD3D8(
            s, self, self + 0x10, spray_bias_vector, scale_game_frames_c(20), r1, g1, b1, 0x30);
    }
    r1 = 0x60;
    g1 = 0x10;
    b1 = 0;
    swap_color_channel_words(&r1, &g1, &b1, (&D_0013E520)->b9);
    {
        float s = variant_effect_scale * 3.5f;
        func_L00_002E0CB8_DD3D8(
            s, self, self + 0x10, spray_bias_vector, scale_game_frames_c(27), r1, g1, b1, 0x40);
    }
    r1 = 0x20;
    g1 = 0;
    b1 = 0;
    swap_color_channel_words(&r1, &g1, &b1, (&D_0013E520)->b9);
    {
        float s = variant_effect_scale * 3.0f;
        func_L00_002E0CB8_DD3D8(
            s, self, self + 0x10, spray_bias_vector, scale_game_frames_c(29), r1, g1, b1, 0x20);
    }
    l00_camera.shake_amount =
        (camera_distance < 20.0f) ? 0.4f - camera_distance * 0.0175f : 0.050000012f;
    l00_camera.shake_frames = scale_game_frames_c(25);
    mine_camera_blast((char *)&blk, self + 0x10, 0, 0);
    if ((&D_0013E520)->b9) {
        k = func_L00_001F2BE8_DD3D8(3.0f, self + 0x10, 0x10, self, 0);
        *(u128_DD3D8 *)spray_vector_or_effect_position = *(u128_DD3D8 *)(self + 0x10);
        send_blast_hits(self,
                        spray_vector_or_effect_position,
                        D_L00_00177F00,
                        k,
                        0,
                        0x830000,
                        2,
                        1,
                        2.0f,
                        1.0f,
                        1.0f);
    }
}

void finalize_suck_cannon_moby(char *moby,
                               char *suction_record,
                               int   flag) __asm__("FUN_L00_002dc938");

void finalize_suck_cannon_moby(char *moby, char *suction_record, int flag) {
    Mtx_002dc938 rotation_matrix;
    void (*fn)(char *);
    if (flag)
        spawn_suck_cannon_projectile_explosion(moby, suction_record);
    MOBY(moby)->collision_disable = *(int *)(suction_record + 0x84);
    qzero(moby + 0x40);
    euler_rotation_matrix(&rotation_matrix, moby + 0x40);
    copy_matrix3x4(moby + 0xC0, &rotation_matrix);
    FUN_0020e098_002dc938(moby);
    MOBY(moby)->flags &= ~4;
    MOBY(moby)->collision_disable = *(int *)(suction_record + 0x84);
    MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24);
    *(short *)(suction_record + 0x68) = 0;
    fn = *(void (**)(char *))(*(char **)(((char *)MOBY(moby)->pclass) + 0x2C) + 0x14);
    if (fn)
        fn(moby);
    else
        mark_moby_for_removal(moby);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DDEA0), where it is exact; names translated to the US level program. */
int FUN_L00_002dc9f0(struct Moby *moby, char *vars);

int FUN_L00_002dc9f0(struct Moby *moby, char *vars) {
    enum {
        SUCK_UPDATE_ALIGNING_FOR_SUCTION = 0,
        SUCK_UPDATE_RISING_FOR_SUCTION = 1,
        SUCK_UPDATE_BEING_PULLED_IN = 2,
        SUCK_UPDATE_STORED = 3,
        SUCK_UPDATE_FLYING_PROJECTILE = 4,
        SUCK_UPDATE_ROLLING_PROJECTILE = 5,
        SUCK_UPDATE_FALLING_AFTER_CANCEL = 6,
        SUCK_UPDATE_DEATH = 7
    };
    Frame_2DDEA0 work;
    R_2DDEA0    *record;

    if (moby == 0 || moby->state == 0xFE || moby->state == 0xFD)
        return 0;
    record = func_L00_002DCD40_2DDEA0(moby);
    if (record != 0) {
        *(u128 *)&record->v30 = *(u128 *)&moby->pos;
        switch ((short)(record->state - 1)) {
        case SUCK_UPDATE_ALIGNING_FOR_SUCTION: {
            float yaw, tolerance;
            tolerance = 0.01f;
            yaw = fast_add_rotations(fast_arc_tan(record->x - moby->pos.x, record->y - moby->pos.y),
                                     3.1415927f);
            approach_w(moby,
                       vars + 0x44,
                       yaw,
                       frame_scale_sq_macro * 0.19634955f,
                       frame_scale_sq_macro * 0.19634955f,
                       frame_scale * 0.3927f);
            if (fast_subtract_rotations(yaw, tolerance) < moby->rot.z &&
                moby->rot.z < fast_add_rotations(yaw, tolerance)) {
                record->state = 2;
                record->f6C = 0.0f;
            }
            clear_u64_value(&record->vel);
            break;
        }
        case SUCK_UPDATE_RISING_FOR_SUCTION:
            moby->anim_speed = record->f6C + 1.0f;
            record->f6C = record->f6C + 1.0f / convert_integer_to_float(func_001F9850_2DDEA0(1));
            if (1.0f < record->f6C)
                record->f6C = 1.0f;
            moby->pos.z += record->f6C / 100.0f * frame_scale;
            extract_matrix_rotation_quaternion(&record->q20, &moby->rot_row0);
            clear_u64_value(&record->vel);
            break;
        case SUCK_UPDATE_BEING_PULLED_IN: {
            float one, distance, half;
            moby->visible = 1;
            moby->flags &= 0xFFFE;
            one = 1.0f;
            if (record->f6C < one) {
                record->f6C =
                    record->f6C + one / convert_integer_to_float(func_001F9850_2DDEA0(30));
                work.q = 0;
                work.v[0].x = -1.5707964f;
                work.v[0].z = -1.5707964f;
                euler_rotation_matrix(&work.v[11], &work.v[0]);
                copy_matrix_rotation(&work.v[3], &record->owner->rot_row0);
                multiply_matrix(&work.v[7], &work.v[3], &work.v[11]);
                matrix_to_quaternion(&work.v[1], &work.v[7]);
                quaternion_nlerp(&work.v[2], &record->q20, &work.v[1], record->f6C);
                func_001FA6C0_2DDEA0(&work.v[2], &work.v[3]);
                copy_matrix3x4(&moby->rot_row0, &work.v[3]);
                moby->flags = (moby->flags | 4) & 0x7FFF;
            } else {
                record->f6C = one;
            }
            moby_point_position(record->owner, 0, &work.v[0]);
            half = 0.5f;
            distance = vector_distance_xy(&moby->pos, &work.v[0]);
            moby->scale =
                ((Cls *)moby->pclass)->f24 * (distance / (record->f64 + record->f64) + half);
            record->f98 = record->f98 + frame_scale_sq_macro * 0.11f;
            if (frame_scale * 0.75f < record->f98)
                record->f98 = frame_scale * 0.75f;
            if (distance < record->f98 * 70.0f) {
                blend_moby_animation(record->owner, 4, 0, func_001F9850_2DDEA0(2));
            }
            if (distance < 3.0f) {
                D_001404FC_2DDEA0[0] = 2;
                moby->collision = 0;
            }
            if (distance < record->f98) {
                hero.suck_cannon_incoming--;
                hero.sucked_moby_count++;
                func_L00_002DB6C8_2DDEA0(moby, &work.v[0]);
                hero.unk2050[record->idx]->flags &= 0xEFFF;
            } else {
                transpose_rotation_matrix(&work.v[5], D_0013FA90_2DDEA0);
                func_001F9BF0_2DDEA0(&work.v[2], &moby->pos, &work.v[0]);
                transform_vector(&work.v[2], &work.v[2], &work.v[5]);
                if (work.v[2].x < 0.0f) {
                    if (0.0f < work.v[2].y) {
                        {
                            float t = -work.v[2].x;
                            float r;
                            if (t < 2.0f)
                                r = t;
                            else
                                r = 2.0f;
                            work.v[3].y = r;
                        }
                    } else {
                        work.v[3].y = (-2.0f < work.v[2].x) ? work.v[2].x : -2.0f;
                    }
                    work.v[3].x = -work.v[2].x * 0.5f;
                } else {
                    work.v[3].y = work.v[2].y * half;
                    work.v[3].x =
                        (absolute_float(work.v[2].y) < 2.0f) ? absolute_float(work.v[2].y) : 2.0f;
                }
                work.v[3].z = 0.0f;
                transform_vector_by_basis(&work.v[3], &work.v[3], D_0013FA90_2DDEA0);
                work.v[3].z = 0.0f;
                qcopy(&work.v[4], &work.v[0]);
                func_001F9BD8_2DDEA0(&work.v[0], &work.v[0], &work.v[3]);
                func_001E9768_2DDEA0(&work.v[4], &work.v[0]);
                func_001F9BF0_2DDEA0(&work.v[1], &work.v[0], &moby->pos);
                func_L00_001FF4B0_2DDEA0(&work.v[1], &work.v[1], record->f98);
                func_001F9BD8_2DDEA0(&moby->pos, &moby->pos, &work.v[1]);
                func_0020EEE8_2DDEA0(moby);
            }
            break;
        }
        case SUCK_UPDATE_STORED:
            moby->flags = (moby->flags | 3) & 0xFFFB;
            moby->visible = 0;
            moby->collision = 0;
            if (moby->anim_flags & 2) {
                if (moby->prev_seq != record->anims[4]) {
                    unsigned char anim = record->anims[4];
                    blend_moby_animation(moby, anim, 0, func_001F9850_2DDEA0(3));
                }
                moby->scale = ((Cls *)moby->pclass)->f24 * 0.3f;
                qzero(&moby->rot);
            }
            break;
        case SUCK_UPDATE_FLYING_PROJECTILE:
        case SUCK_UPDATE_ROLLING_PROJECTILE: {
            float spin, one;
            int   hit_wall, surface;
            moby->visible = 1;
            moby->flags &= 0xFFFE;
            if (moby->pos.z < 2.0f) {
                func_L00_002DDDE8_2DDEA0(moby, record, 0);
                return 0;
            }
            work.q = *(u128 *)&record->vel;
            moby->scale = moby->scale + ((Cls *)moby->pclass)->f24 * (record->f7C * frame_scale);
            if (((Cls *)moby->pclass)->f24 < moby->scale)
                moby->scale = ((Cls *)moby->pclass)->f24;
            if (record->target != 0 && record->target->state != 0xFE &&
                record->target->state != 0xFD) {
                int max_life = func_001F9850_2DDEA0(0x12C);
                if (record->i94 < max_life - func_001F9850_2DDEA0(5)) {
                    float speed, quad_a, quad_b, root, root1, root2, time;
                    qcopy(&work.v[3], &record->target->pos);
                    work.v[3].z = work.v[3].z + record->f90;
                    func_001F9BF0_2DDEA0(&work.v[2], &work.v[3], &record->v40);
                    func_001F9BF0_2DDEA0(&work.v[1], &work.v[3], &moby->pos);
                    qcopy(&record->v40, &work.v[3]);
                    speed = func_001F9CB8_2DDEA0(&record->vel);
                    quad_a = speed * speed - func_L00_001FF3E0_2DDEA0(&work.v[2]);
                    quad_b = (work.v[2].x * work.v[1].x + work.v[2].y * work.v[1].y +
                              work.v[2].z * work.v[1].z) *
                             -2.0f;
                    root = square_root(quad_b * quad_b -
                                       quad_a * 4.0f * -func_L00_001FF3E0_2DDEA0(&work.v[1]));
                    root1 = (-quad_b + root) / (2.0f * quad_a);
                    root2 = (-quad_b - root) / (2.0f * quad_a);
                    if (0.0f < root1 && 0.0f < root2) {
                        time = (root2 < root1) ? root1 : root2;
                    } else if (0.0f < root1) {
                        time = root1;
                    } else if (0.0f < root2) {
                        time = root2;
                    } else {
                        time = -1.0f;
                    }
                    if (0.0f < time) {
                        func_001F9C30_2DDEA0(&work.v[4], &work.v[2], time);
                        func_001F9BD8_2DDEA0(&work.v[4], &work.v[4], &work.v[1]);
                        func_L00_001FF4B0_2DDEA0(
                            &work.v[4], &work.v[4], frame_time_sq_macro * 90.0f);
                        func_001F9BD8_2DDEA0(&record->vel, &record->vel, &work.v[4]);
                        func_L00_001FF4B0_2DDEA0(&record->vel, &record->vel, speed);
                    } else {
                        func_L00_001FF4B0_2DDEA0(
                            &work.v[1], &work.v[1], frame_time_sq_macro * 90.0f);
                        func_001F9BD8_2DDEA0(&record->vel, &record->vel, &work.v[1]);
                        func_L00_001FF4B0_2DDEA0(&record->vel, &record->vel, speed);
                    }
                }
            }
            one = 1.0f;
            func_001F9BD8_2DDEA0(&moby->pos, &moby->pos, &record->vel);
            make_hit_descriptor(&work.v[1], moby, 0x430000, 2.0f, &record->vel);
            set_vector_length_xy(&work.v[1], &work.v[1], one);
            work.h.hit.v.w = 5627.925f;
            work.h.hit.b18 = 1;
            work.h.hit.b19 = 3;
            work.h.hit.h1A = 0x351;
            work.h.hit.v.z = one;
            hit_wall = func_L00_001F2BE8_2DDEA0(record->f80, &moby->pos, 1, moby, &work.v[1]);
            hit_wall |= func_L00_001F10E0_2DDEA0(record->f80, &moby->pos, 0, moby);
            if (hit_wall != 0) {
                qcopy_nc(&moby->pos, &((Coll *)D_L00_00173E40_lui)->v30);
                func_001F9BF0_2DDEA0(&record->v50, &((Coll *)D_L00_00173E40_lui)->v20, &moby->pos);
            }
            if (((Coll *)D_L00_00173E40_lui)->hit != 0 &&
                ((Coll *)D_L00_00173E40_lui)->hit->oclass == 0)
                hit_wall = 0;
            if (record->state == 6) {
                moby->flags &= 0x7FFF;
                moby->collision = ((Cls *)moby->pclass)->i10;
                moby->flags |= 4;
                record->v50.z = one;
                record->v50.x = 0.0f;
                record->v50.y = 0.0f;
                record->v50.w = 0.01f;
                spin = func_001F9CB8_2DDEA0(&work.v[0]) / record->f80;
                extract_matrix_rotation_quaternion(&work.v[1], &moby->rot_row0);
                func_L00_001FF4B0_2DDEA0(&record->v50, &record->v50, one);
                normalize_angle(spin);
                func_L00_001FF4B0_2DDEA0(&work.v[4], &record->vel, -1.0f);
                fast_vec_cross(&work.v[3], &work.v[4], &record->v50);
                func_L00_001FF4B0_2DDEA0(&work.v[3], &work.v[3], one);
                build_quaternion_from_axis_angle(&work.v[2], &work.v[3], spin);
                multiply_quaternions(&work.v[5], &work.v[1], &work.v[2]);
                func_001FA6C0_2DDEA0(&work.v[5], &work.v[6]);
                func_L00_001FF4B0_2DDEA0(&moby->rot_row0, &work.v[6], one);
                func_L00_001FF4B0_2DDEA0(&moby->rot_row1, &work.v[7], one);
                func_L00_001FF4B0_2DDEA0(&moby->rot_row2, &work.v[8], one);
            }
            if (hit_wall != 0) {
                surface = 0;
                if (((Coll *)D_L00_00173E40_lui)->hit != 0 &&
                    ((Cls *)((Coll *)D_L00_00173E40_lui)->hit->pclass) != 0) {
                    surface = ((Cls *)((Coll *)D_L00_00173E40_lui)->hit->pclass)->h46;
                }
                func_L00_001FF4B0_2DDEA0(
                    &((Coll *)D_L00_00173E40_lui)->v40, &((Coll *)D_L00_00173E40_lui)->v40, 1.0f);
                if (moby->prev_seq != record->anims[6]) {
                    unsigned char anim = record->anims[6];
                    blend_moby_animation(moby, anim, 0, func_001F9850_2DDEA0(0x14));
                }
                moby->collision_disable = record->i88;
                record->state = 6;
                if (0.87266463f <
                    fast_arc_tan(((Coll *)D_L00_00173E40_lui)->v40.z,
                                 vector_length_xy(&((Coll *)D_L00_00173E40_lui)->v40))) {
                    if (record->h9E >= 0)
                        allocate_voice_for_target_entry(record->h9E, 0, moby);
                    func_L00_002DDDE8_2DDEA0(moby, record, 1);
                    return 0;
                }
                if (D_0013E629_2DDEA0[0] != 0 && surface >= 5) {
                    if (record->h9E >= 0)
                        allocate_voice_for_target_entry(record->h9E, 0, moby);
                    func_L00_002DDDE8_2DDEA0(moby, record, 1);
                }
                qzero(&work.v[1]);
                work.v[1].z = ((Coll *)D_L00_00173E40_lui)->v40.z;
                func_L00_001FF610_2DDEA0(&record->vel, &record->vel, &work.v[1]);
            }
            if (fast_dec_timer(&record->i94) != 0) {
                func_L00_002DDDE8_2DDEA0(moby, record, 1);
                return 0;
            }
            if (60.0f < vector_distance_xy(&moby->pos, D_0013F4D0_2DDEA0)) {
                func_L00_002DDDE8_2DDEA0(moby, record, 1);
                return 0;
            }
            if (func_001F9CB8_2DDEA0(&record->vel) < frame_scale * 0.01f) {
                func_L00_002DDDE8_2DDEA0(moby, record, 1);
                return 0;
            }
            record->vel.z = record->vel.z - frame_scale_sq_macro * 0.00925f;
            spawn_effect_particle_with_transformed_random_offset(&moby->pos, &moby->rot_row0);
            break;
        }
        case SUCK_UPDATE_FALLING_AFTER_CANCEL: {
            int landed;
            moby->visible = 1;
            moby->flags &= 0xFFFE;
            record->vel.z = record->vel.z - frame_scale_sq_macro * 0.013f;
            if (moby->pos.z < 2.0f) {
                func_L00_002DDDE8_2DDEA0(moby, record, 1);
                return 0;
            }
            landed = func_L00_00259B08_2DDEA0(
                moby, &record->vel, 0.8f, truncate_float_to_s32(record->f8C * 1024.0f), 0.0f, 0);
            moby->rot.x = moby->rot.x / 1.5f;
            moby->rot.y = moby->rot.y / 1.5f;
            if (landed != 0) {
                moby->rot.x = 0.0f;
                moby->rot.y = 0.0f;
                moby->anim_speed = 1.0f;
                return 1;
            }
            if (moby->anim_flags & 2) {
                moby->anim_speed = record->f78;
            }
            break;
        }
        case SUCK_UPDATE_DEATH:
            func_001E9730_2DDEA0(D_L00_001EA4B0_2DDEA0);
            break;
        }
    }
    if (moby->flags & 4)
        func_0020EEE8_2DDEA0(moby);
    if (record->state < 4) {
        if (record->timer <= 0.0f) {
            if (((Cls *)moby->pclass)->f2C->fC != 0)
                ((Cls *)moby->pclass)->f2C->fC(moby);
        }
        record->timer = record->timer - 1.0f;
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DED98), where it is exact; names translated to the US level program. */
void suck_beam_draw_mesh(void) __asm__("FUN_L00_002dd8e8");

void suck_beam_draw_mesh(void) {
    Pkt    quad_packet;
    float  vc[4];
    float  vd[4];
    float  ve[4];
    float  vf[4];
    float  row_texture_u, next_row_texture_u, am, segment_texture_v, next_segment_texture_v, dot;
    float  step;
    short *row_alpha_counter;
    int    i, j, k, row_alpha_or_packed_color;
    unsigned short y;
    quad_packet.word1 = get_effect_texture(0x15);
    quad_packet.word2 = 0x0000FF9000000260L;
    quad_packet.word3 = 0x8000000048L;
    quad_packet.word0 = 0;
    row_texture_u = suction_effect_texture_u_offset;
    suction_effect_texture_u_offset = row_texture_u + 0.001f;
    if (suction_effect_texture_u_offset > 7.0f)
        suction_effect_texture_u_offset -= 7.0f;
    step = 0.3f;
    row_texture_u += step;
    for (i = 1; i < 10; i++) {
        segment_texture_v = suction_effect_texture_v_offset;
        suction_effect_texture_v_offset = segment_texture_v + D_L00_00161B50;
        if (suction_effect_texture_v_offset > 7.0f)
            suction_effect_texture_v_offset -= 7.0f;
        next_row_texture_u = row_texture_u + step;
        for (j = 0; j < 20; j++) {
            qcopy(quad_packet.q[0], D_L00_001E6020 + (j * 16 + i * 0x140));
            qcopy(quad_packet.q[1], D_L00_001E6020 + ((j + 1) % 20 * 16 + i * 0x140));
            qcopy(quad_packet.q[2], D_L00_001E5EE0 + (j * 16 + i * 0x140));
            qcopy(quad_packet.q[3], D_L00_001E5EE0 + ((j + 1) % 20 * 16 + i * 0x140));
            fast_vec_sub(vc, quad_packet.q[0], quad_packet.q[1]);
            fast_vec_sub(vd, quad_packet.q[2], quad_packet.q[1]);
            fast_vec_cross(ve, vd, vc);
            fast_vec_sub(vf, quad_packet.q[1], &g_debug_camera);
            dot = fast_vec_dot(ve, vf);
            if (0.0f <= dot) {
                segment_texture_v += 0.05f;
            } else {
                next_segment_texture_v = segment_texture_v + 0.05f;
                am = row_texture_u - step;
                y = D_L00_001E6008[i - 1];
                row_alpha_counter = &D_L00_001E6008[i];
                row_alpha_or_packed_color = *row_alpha_counter;
                for (k = 0; k < 2; k++) {
                    if (row_alpha_or_packed_color < 0)
                        row_alpha_or_packed_color = 0;
                    if (row_alpha_or_packed_color > 0x40)
                        row_alpha_or_packed_color = 0x40;
                    row_alpha_or_packed_color = (row_alpha_or_packed_color << 24) | 0x404040;
                    quad_packet.col[k * 2] = row_alpha_or_packed_color;
                    quad_packet.col[k * 2 + 1] = row_alpha_or_packed_color;
                    row_alpha_or_packed_color = (short)y;
                }
                if (i == 1) {
                    quad_packet.col[3] = 0x20404040;
                    quad_packet.col[2] = 0x20404040;
                } else if (i == 9) {
                    quad_packet.col[1] = 0x404040;
                    quad_packet.col[0] = 0x404040;
                }
                quad_packet.x0 = row_texture_u;
                quad_packet.y0 = segment_texture_v;
                quad_packet.x1 = row_texture_u;
                quad_packet.y1 = next_segment_texture_v;
                quad_packet.x2 = am;
                quad_packet.y2 = segment_texture_v;
                quad_packet.x3 = am;
                quad_packet.y3 = next_segment_texture_v;
                segment_texture_v = next_segment_texture_v;
                draw_geometry_quad(&quad_packet, 0, 0);
            }
        }
        row_texture_u = next_row_texture_u;
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}

/* Sets D_L00_00161B40_gp and D_L00_00161B44_gp to 1 and clears D_L00_001E6008. */
void FUN_L00_002ddca0(void) {
    D_L00_00161B40_gp = 1;
    D_L00_00161B44_gp = 1;
    (*(short *)D_L00_001E6008) = 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DF168), where it is exact; names translated to the US level program. */
void suck_beam_build(char *a, char *b) __asm__("FUN_L00_002ddcb8");

void suck_beam_build(char *cannon_moby, char *suction_origin) {
    u128_DF168 beam_initialization_vectors[7];
    char      *orientation_up_vector;
    short     *w;
    int        i, hero_state, j;
    if (D_L00_00161B40 == 0)
        return;
    D_L00_00161B84 = 1.7f;
    D_L00_00161B40 = 0;
    hero_state = hero.state.current;
    if ((hero_state == 0x1E || hero_state == 1) && (hero.moby->flags & 1)) {
        qcopy((char *)beam_initialization_vectors, D_L00_00166FD0);
        qcopy((char *)beam_initialization_vectors + 0x10, D_L00_00166FD0 + 0x10);
        qcopy((char *)beam_initialization_vectors + 0x20, D_L00_00166FD0 + 0x20);
        orientation_up_vector = (char *)beam_initialization_vectors + 0x20;
    } else {
        if (hero.unk22C8 == 1) {
            fast_vec_scale(beam_initialization_vectors, cannon_moby + 0xD0, -1.0f);
            qcopy((char *)beam_initialization_vectors + 0x10, cannon_moby + 0xC0);
            qcopy((char *)beam_initialization_vectors + 0x20, cannon_moby + 0xE0);
            orientation_up_vector = (char *)beam_initialization_vectors + 0x20;
        } else {
            qcopy((char *)beam_initialization_vectors, &hero.basis[0]);
            qcopy((char *)beam_initialization_vectors + 0x10, &hero.basis[1]);
            qcopy((char *)beam_initialization_vectors + 0x20, &hero.basis[2]);
            orientation_up_vector = (char *)beam_initialization_vectors + 0x20;
        }
    }
    fast_vec_normalize(
        (char *)beam_initialization_vectors + 0x30, beam_initialization_vectors, D_L00_00161B84);
    fast_vec_normalize((char *)beam_initialization_vectors + 0x40, orientation_up_vector, 1.0f);
    qcopy(D_L00_001E5F40, suction_origin);
    D_L00_001E6008[0] = 0;
    for (i = 1; i < 10; i++) {
        fast_vec_add(D_L00_001E5F40 + i * 0x10,
                     D_L00_001E5F30 + i * 0x10,
                     (char *)beam_initialization_vectors + 0x30);
        D_L00_001E6008[i] = 0;
    }
    clear_u64_value(D_L00_00161B60);
    beam_initialization_vectors[5] = 0;
    beam_initialization_vectors[6] = 0;
    *(float *)((char *)beam_initialization_vectors + 0x58) = 0.1f;
    *(float *)((char *)beam_initialization_vectors + 0x5C) = 1.0f;
    *(float *)((char *)beam_initialization_vectors + 0x60) = 1.0f;
    *(float *)((char *)beam_initialization_vectors + 0x6C) = 1.0f;
    {
        float initial_random_angle = random_angle_radians();
        build_look_at_matrix_c2(D_L00_00161B70,
                                (char *)beam_initialization_vectors + 0x50,
                                initial_random_angle,
                                (char *)beam_initialization_vectors + 0x60);
        D_L00_00161B80 = initial_random_angle;
    }
    D_L00_00161B50 = 0.006f;
    D_L00_00161B54 = 0;
    D_L00_00161B58 = 0;
    if (D_L00_00161B44 != 0) {
        D_L00_00161B44 = 0;
        for (j = 199; j >= 0; j--)
            ((E04_DF168 *)D_L00_001E4C80)[j].f14 = 0;
    }
}

void suck_beam_update(void *moby, void *position) {
    V41 step, up, orient[3], ray_end, start, end, axis, tmp_a, tmp_b, frame[3], tmp_c, motion,
        tmp_d, tmp_e, interp, cross_c, basis_c, tmp_f, unused_a, unused_b;
    V41        *orientation;
    int         i, j, n, segment, next_ring;
    float       beam_step_blend_fraction, angle, turn, delta, dot, length, amount, speed;
    Particle41 *p;
    V41        *anchor, *previous, *source;

    suck_beam_build(moby, position);
    if ((((Hero41 *)&hero)->camera_mode == 30 || ((Hero41 *)&hero)->camera_mode == 1) &&
        (((Hero41 *)&hero)->moby->flags & 1)) {
        qcopy(&orient[0], D_L00_00166FD0);
        qcopy(&orient[1], D_L00_00166FD0 + 16);
        qcopy(&orient[2], D_L00_00166FD0 + 32);
        orientation = &orient[0];
    } else if (((Hero41 *)&hero)->special == 1) {
        fast_vec_scale(&orient[0], &((M41 *)moby)->basis[1], -1.0f);
        {
            M41 *m = moby;
            qcopy_nc(&orient[1], &m->basis[0]);
            qcopy_nc(&orient[2], &m->basis[2]);
        }
        orientation = &orient[0];
    } else {
        qcopy(&orient[0], &((Hero41 *)&hero)->matrix[0]);
        qcopy(&orient[1], &((Hero41 *)&hero)->matrix[1]);
        qcopy(&orient[2], &((Hero41 *)&hero)->matrix[2]);
        orientation = &orient[0];
    }
    fast_vec_normalize(&step, orientation, D_L00_00161B84);
    fast_vec_normalize(&up, &orient[2], 1.0f);
    qcopy(D_L00_001E5F40, position);
    fast_vec_normalize(&ray_end, orientation, D_L00_00161B84 * 10.0f);
    fast_vec_add(&ray_end, &ray_end, position);
    if (collision_line_ddf60(position, &ray_end, 20, (int)((Hero41 *)&hero)->moby, 0)) {
        int n, i, k;
        n = truncate_float_to_s32(vector_distance_xy(collision_hit_point, position) /
                                  D_L00_00161B84);
        for (i = n; i < 10; i++) {
            if (D_L00_001E6008[i] > 64)
                D_L00_001E6008[i] = 64;
            D_L00_001E6008[i] = (u16)D_L00_001E6008[i] - 4;
            if (D_L00_001E6008[i] < 0)
                D_L00_001E6008[i] = 0;
        }
        for (k = n - 1; k >= 0; k--)
            D_L00_001E6008[k] = D_L00_001E6008[k + 1] + 32;
    } else {
        int i;
        D_L00_001E6008[0] = (u16)D_L00_001E6008[0] + 4;
        for (i = 1; i < 10; i++)
            D_L00_001E6008[i] = D_L00_001E6008[i - 1] - 32;
    }
    transform_vector_by_basis(&start, &(*(V41 *)D_L00_00161B60), orientation);
    fast_vec_add(&start, &start, &step);
    transform_vector_by_basis(&end, &(*(V41 *)D_L00_00161B70), orientation);
    fast_vec_add(&end, &end, &step);
    beam_step_blend_fraction =
        (float)D_L00_00161B54 / convert_integer_to_float(scale_game_frames(60));
    D_L00_00161B54++;
    step.f.x = start.f.x + (end.f.x - start.f.x) * beam_step_blend_fraction;
    step.f.y = start.f.y + (end.f.y - start.f.y) * beam_step_blend_fraction;
    step.f.z = start.f.z + (end.f.z - start.f.z) * beam_step_blend_fraction;
    if (D_L00_00161B54 >= scale_game_frames(60)) {
        qcopy(&(*(V41 *)D_L00_00161B60), &(*(V41 *)D_L00_00161B70));
        fast_vec_normalize(&axis, orientation, D_L00_00161B84);
        tmp_a.q = 0;
        tmp_b.q = 0;
        tmp_a.f.z = 0.1f;
        tmp_a.f.w = 1.0f;
        tmp_b.f.x = 1.0f;
        tmp_b.f.w = 1.0f;
        angle =
            fast_add_rotations(D_L00_00161B80, random_float_between(45.0f, 315.0f) * 0.0174532924f);
        build_look_at_matrix(&(*(V41 *)D_L00_00161B70), &tmp_a, angle, &tmp_b);
        D_L00_00161B80 = angle;
        D_L00_00161B54 = 0;
    }
    (*(float *)&D_L00_00161B58_gp) = fast_add_rotations((*(float *)&D_L00_00161B58_gp), 0.1f);
    qcopy(&axis, &orient[2]);
    qcopy(&frame[2], &axis);
    fast_vec_cross(&frame[1], &step, &frame[2]);
    fast_vec_normalize(&frame[1], &frame[1], 1.0f);
    fast_vec_cross(&frame[0], &frame[2], &frame[1]);
    {
        int k;
        for (k = 0; k < 20; k++) {
            fast_vec_scale(&tmp_a, &l00_suck_beam_ring_source_offsets[k], 0.1f);
            transform_vector_by_basis(&tmp_a, &tmp_a, frame);
            fast_vec_add(&((V41 *)D_L00_001E6020)[k], D_L00_001E5F40, &tmp_a);
        }
    }
    {
        int i;
        for (i = 1; i < 10; i++) {
            anchor = (V41 *)(D_L00_001E5F40 + i * 16);
            previous = (V41 *)(D_L00_001E5F30 + i * 16);
            fast_vec_sub(&tmp_a, anchor, previous);
            qcopy(&tmp_b, &step);
            fast_vec_cross(&tmp_c, &tmp_b, &tmp_a);
            dot = fast_vec_dot(&tmp_a, &tmp_b);
            {
                float target =
                    1.5707964f - fast_arc_sin(dot / (fast_vec_length(&tmp_a) * D_L00_00161B84));
                turn = rope_spring_turn(
                    &suck_beam_segment_turn_spring_states[i], 0.0f, target, 0.06f, 0.2f, 0.0f);
                delta = fast_subtract_rotations(target, turn);
            }
            if (absolute_float(delta) > 0.31415927f)
                turn = fast_add_rotations(turn, fast_subtract_rotations(delta, 0.31415927f));
            build_look_at_matrix(&motion, &tmp_a, turn, &tmp_c);
            fast_vec_normalize(&tmp_d, &motion, D_L00_00161B84);
            lerp_vector(&motion, &motion, &tmp_d, 1.0f);
            {
                float len = fast_vec_length(&motion);
                if (len > 2.0f)
                    fast_vec_scale(&motion, &motion, 2.0f / len);
            }
            if (i >= 6)
                lerp_vector(
                    &motion, &motion, &tmp_a, (float)i / convert_integer_to_float(10) + 0.0f);
            fast_vec_add(anchor, previous, &motion);
            qcopy_nc(&tmp_e, &step);
            qcopy_nc(&basis_c, &axis);
            fast_vec_normalize(&interp, &tmp_e, 1.0f);
            fast_vec_cross(&cross_c, &interp, &basis_c);
            fast_vec_normalize(&cross_c, &cross_c, 1.0f);
            fast_vec_cross(&basis_c, &cross_c, &interp);
            qcopy(&axis, &basis_c);
            {
                int j;
                for (j = 0; j < 20; j++) {
                    angle = (float)i * 0.3f + 0.1f;
                    angle += fast_sin(fast_add_rotations(
                                 (*(float *)&D_L00_00161B58_gp) + (float)i * 0.7f, 0.0f)) *
                             0.1f;
                    fast_vec_scale(&tmp_f, &l00_suck_beam_ring_source_offsets[j], angle);
                    transform_vector_by_basis(&tmp_f, &tmp_f, &interp);
                    fast_vec_add(
                        &((V41(*)[20])D_L00_001E6020)[i][j], D_L00_001E5F40 + i * 16, &tmp_f);
                }
            }
            fast_vec_normalize(&step, &motion, D_L00_00161B84);
        }
    }
    add_draw_callback(suck_beam_draw_mesh, (int)moby);
    {
        int count, sparks;
        count = 0;
        sparks = 0;
        for (i = 0; i < 200; i++) {
            if (!D_L00_001E4C80_lui[i].p && count < 5) {
                for (segment = 9; segment >= 0 && D_L00_001E6008[segment] < 64; segment--)
                    ;
                if (segment > 0) {
                    if (segment >= 2 && sparks < 2) {
                        {
                            int  k = random_integer_below(20);
                            V41 *ring;
                            amount = random_float_between(0.2f, 1.0f);
                            ring = (V41 *)D_L00_001E6020 + k;
                            lerp_vector(&tmp_a, ring, ring + segment * 20, amount);
                        }
                        lerp_vector(&tmp_c, D_L00_001E5F40, D_L00_001E5F40 + segment * 16, amount);
                        speed = random_float_between(60000.0f, 180000.0f);
                        p = spawn_puff(&tmp_a,
                                       0.0f,
                                       1.0f,
                                       1.03f,
                                       random_integer_below(5) + 1,
                                       &tmp_b,
                                       speed,
                                       0x404040);
                        if (p) {
                            Tail41 *tail = &p->tail;
                            fast_vec_sub(&motion, &tmp_a, &tmp_c);
                            sparks++;
                            fast_vec_normalize(&tail->velocity, &motion, 0.01f);
                            tail->velocity.f.z += 0.02f;
                        }
                    }
                    qcopy(&tmp_a, D_L00_001E5F40 + segment * 16);
                    clear_u64_value(&tmp_b);
                    speed = random_float_between(60000.0f, 180000.0f);
                    {
                        Particle41 *main =
                            spawn_puff(&tmp_a, 0.0f, 1.0f, 1.0f, 6, &tmp_b, speed, 0x7F404040);
                        D_L00_001E4C80_lui[i].p = (char *)main;
                        if (main) {
                            Tail41 *tail = &main->tail;
                            main->life = 200;
                            tail->mode = 2;
                            count++;
                            tail->r = 127;
                            tail->g = 200;
                        }
                    }
                    D_L00_001E4C80_lui[i].idx = segment;
                    D_L00_001E4C80_lui[i].f = 0;
                    D_L00_001E4C80_lui[i].phase = 0;
                    D_L00_001E4C80_lui[i].angle = random_integer_below(20);
                    D_L00_001E4C80_lui[i].phase_step = random_float_between(0.1f, 0.8f);
                    D_L00_001E4C80_lui[i].speed = random_float_between(0.1f, 0.4f);
                }
            }
            if (D_L00_001E4C80_lui[i].p) {
                fast_vec_sub(&tmp_a,
                             D_L00_001E5F30 + D_L00_001E4C80_lui[i].idx * 16,
                             D_L00_001E5F40 + D_L00_001E4C80_lui[i].idx * 16);
                length = fast_vec_length(&tmp_a);
                D_L00_001E4C80_lui[i].f += D_L00_001E4C80_lui[i].speed;
                ((Particle41 *)D_L00_001E4C80_lui[i].p)->life = 200;
                if (length < D_L00_001E4C80_lui[i].f) {
                    D_L00_001E4C80_lui[i].idx = (u16)D_L00_001E4C80_lui[i].idx - 1;
                    if (D_L00_001E4C80_lui[i].idx <= 0) {
                        free_effect_particle(D_L00_001E4C80_lui[i].p);
                        D_L00_001E4C80_lui[i].p = 0;
                    } else {
                        D_L00_001E4C80_lui[i].f -= length;
                        fast_vec_sub(&tmp_a,
                                     D_L00_001E5F30 + D_L00_001E4C80_lui[i].idx * 16,
                                     D_L00_001E5F40 + D_L00_001E4C80_lui[i].idx * 16);
                        length = fast_vec_length(&tmp_a);
                    }
                }
                if (D_L00_001E4C80_lui[i].p) {
                    amount = D_L00_001E4C80_lui[i].f / length;
                    if (D_L00_001E4C80_lui[i].idx == 2)
                        ((Particle41 *)D_L00_001E4C80_lui[i].p)->life = 100;
                    else if (D_L00_001E4C80_lui[i].idx < 2) {
                        ((Particle41 *)D_L00_001E4C80_lui[i].p)->life =
                            truncate_float_to_s32((1.0f - amount) * 100.0f);
                        if (((Particle41 *)D_L00_001E4C80_lui[i].p)->life < 61)
                            ((Particle41 *)D_L00_001E4C80_lui[i].p)->life = 60;
                    }

                    if (D_L00_001E4C80_lui[i].idx == 1) {
                        fast_vec_sub(
                            &tmp_b, position, &((Particle41 *)D_L00_001E4C80_lui[i].p)->pos);
                        fast_vec_normalize(&tmp_b, &tmp_b, D_L00_001E4C80_lui[i].speed);
                        fast_vec_add(&((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,
                                     &((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,
                                     &tmp_b);
                        ((Particle41 *)D_L00_001E4C80_lui[i].p)->size *=
                            1.0f + D_0015ED60 * -0.0500000119f;
                    } else {
                        fast_vec_scale(&tmp_a, &tmp_a, amount);
                        fast_vec_add(&((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,
                                     D_L00_001E5F40 + D_L00_001E4C80_lui[i].idx * 16,
                                     &tmp_a);
                        j = D_L00_001E4C80_lui[i].angle;
                        fast_vec_sub(&tmp_b,
                                     (char *)((V41 *)D_L00_001E6020) +
                                         D_L00_001E4C80_lui[i].idx * 320 + j * 16,
                                     D_L00_001E5F40 + D_L00_001E4C80_lui[i].idx * 16);
                        fast_vec_sub(&tmp_c,
                                     (char *)((V41 *)D_L00_001E5EE0) +
                                         D_L00_001E4C80_lui[i].idx * 320 + j * 16,
                                     D_L00_001E5F30 + D_L00_001E4C80_lui[i].idx * 16);
                        interpolate_vector(&motion, &tmp_b, &tmp_c, amount);
                        j = (j + 19) % 20;
                        fast_vec_sub(&tmp_d,
                                     (char *)((V41 *)D_L00_001E6020) +
                                         D_L00_001E4C80_lui[i].idx * 320 + j * 16,
                                     D_L00_001E5F40 + D_L00_001E4C80_lui[i].idx * 16);
                        fast_vec_sub(&tmp_e,
                                     (char *)((V41 *)D_L00_001E5EE0) +
                                         D_L00_001E4C80_lui[i].idx * 320 + j * 16,
                                     D_L00_001E5F30 + D_L00_001E4C80_lui[i].idx * 16);
                        interpolate_vector(&interp, &tmp_d, &tmp_e, amount);
                        interpolate_vector(&motion, &motion, &interp, D_L00_001E4C80_lui[i].phase);
                        D_L00_001E4C80_lui[i].phase += D_L00_001E4C80_lui[i].phase_step;
                        if (D_L00_001E4C80_lui[i].phase >= 1.0f) {
                            D_L00_001E4C80_lui[i].phase -= 1.0f;
                            D_L00_001E4C80_lui[i].angle = j;
                        }
                        fast_vec_add(&((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,
                                     &((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,
                                     &motion);
                    }
                }
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002E0490), where it is exact; names translated to the US level program. */

/* When D_L00_001E6008[0] > 0: caps the ten counters at 0x40, lowers each by 4, queues 002dd8e8. */
void suck_beam_queue_draw(int draw_callback_context) {
    int i;
    if (D_L00_001E6008[0] > 0) {
        for (i = 0; i < 10; i++) {
            if (D_L00_001E6008[i] > 0x40)
                D_L00_001E6008[i] = 0x40;
            D_L00_001E6008[i] = (unsigned short)D_L00_001E6008[i] - 4;
        }
        add_draw_callback(suck_beam_draw_mesh, draw_callback_context);
    }
}

/* pull each active entry's marker toward its target, then update the rest */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002E04F8), where it is exact; names translated to the US level program. */
void stop_suck_cannon_suction_effect(int callback_context) {
    VQ    segment_offset, segment_position, marker_offset;
    int   i;
    float segment_length;
    char *p;

    *(int *)&D_L00_00161B40_gp = 1;
    *(float *)&D_L00_00161B50_gp = *(float *)&D_L00_00161B50_gp * 0.5f;
    *(int *)&D_L00_00161B44_gp = 1;
    for (i = 0; i < 200; i++) {
        p = D_L00_001E4C80[i].p;
        if (p != 0) {
            D_L00_001E4C80[i].p = 0;
            fast_vec_sub(segment_offset,
                         D_L00_001E5F30 + D_L00_001E4C80[i].idx * 16,
                         D_L00_001E5F40 + D_L00_001E4C80[i].idx * 16);
            segment_length = vector_length_xyz(segment_offset);
            scale_vector_xyz(segment_offset, segment_offset, D_L00_001E4C80[i].f / segment_length);
            fast_vec_add(
                segment_position, D_L00_001E5F40 + D_L00_001E4C80[i].idx * 16, segment_offset);
            fast_vec_sub(marker_offset, p + 0x10, segment_position);
            fast_vec_normalize(marker_offset, marker_offset, 0.03f);
            scale_vector_xyz(segment_offset, segment_offset, 0.1f / segment_length);
            fast_vec_add(p + 0x30, marker_offset, segment_offset);
            *(short *)(p + 0xA) = 0x46;
        }
    }
    suck_beam_queue_draw(callback_context);
}

void scatter_moby_from_hazards(O_2df1e0 *p) __asm__("FUN_L00_002df1e0");

void scatter_moby_from_hazards(O_2df1e0 *pickup_moby) {
    s32 i;
    f32 beam_segment_collection_radius;
    if (pickup_moby == 0)
        return;
    if (pickup_moby->b20 == 0xFE)
        return;
    if (pickup_moby->b20 == 0xFD)
        return;
    if (random_integer_below(10))
        return;
    if (!(pickup_moby->p24 && pickup_moby->p24->h46 == 0x13) &&
        !is_moby_class_e2_cc_de_3ee_d6_e1_d5_or_df(pickup_moby))
        return;
    if (!(fast_vec_dist(pickup_moby->v10, &hero.motion.pos) < 30.0f))
        return;
    for (i = 0; i < 10 && D_L00_001E6008[i] >= 8; i++) {
        beam_segment_collection_radius = i * 0.3f + 0.1f;
        if (beam_segment_collection_radius < 2.0f)
            beam_segment_collection_radius = 2.0f;
        if (fast_vec_dist(pickup_moby->v10, D_L00_001E5F40_lui[i]) <
            beam_segment_collection_radius) {
            if (pickup_moby && pickup_moby->p24 && pickup_moby->p24->h46 == 0x13) {
                begin_bolt_attraction(pickup_moby, hero.moby, 0.0f, 0.0f);
            } else {
                try_collect_ammo_pickup(pickup_moby);
            }
        }
    }
}
