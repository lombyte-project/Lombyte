/* Level 01 moby classes: tall_shroom. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"

typedef struct {
    u8 pad0[0x5C];
    f32 f5c;
} Ent;

extern Ent D_L01_00202D80[];
extern s32 level_frame_count __asm__("D_L01_0015F5CC");
void mine_blend_color(void) __asm__("FUN_L00_002371e0");
void FUN_L01_002b96e0(s32, void *);
extern float convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern float fast_sin(float) __asm__("FUN_001f9de0");

void FUN_L01_00309bf8(void) {
    f32 v;
    s32 i;

    mine_blend_color();
    FUN_L01_002b96e0(3, D_L01_00202D80);
    v = fast_sin((convert_integer_to_float(level_frame_count & 0x3F) - 32.0f) * 0.09817477f);
    v = v * 0.5f + 0.5f;
    for (i = 2; i >= 0; i--) {
        D_L01_00202D80[i].f5c = v;
    }
}
typedef struct {
    u8 pad0[0x30];
    u8 b30;
    u8 b31;
    s16 h32;
} FlagMoby;
extern void FUN_L01_00309bf8(void);
void enqueue_callback_list_1(void *fn, void *arg) __asm__("FUN_001f4600");

/* Sets both flag fields to 0xFF and queues FUN_L01_00309bf8 for the moby. */
void FUN_L01_00309c98(FlagMoby *m) {
    int v = 0xFF;
    m->h32 = v;
    m->b30 = v;
    enqueue_callback_list_1(FUN_L01_00309bf8, m);
}
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/state/usage_stats.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/gameplay/state/level_state.h"
#include "sda.h"

/* Help message playback state (same layout as S00203908 in shared/ui/help/help_request.c). */
struct L01HelpState {
    s32 state;
    u8 pad4[0x20];
    s32 cur;
};

/* Per-message help record at D_00141968: id 0xFFFF = disabled. */
struct L01HelpRecord {
    u16 id;
    u16 pad;
    s32 v;
};

extern struct L01HelpState help_state __asm__("D_L01_00179890");
extern struct L01HelpRecord help_records[] __asm__("D_00141968");
extern s32 cutscene_mode_macro __asm__("D_L01_0015F5C4") MACRO_ADDR;
extern s32 D_L01_0015F688 MACRO_ADDR;
extern s32 D_L01_0015F68C MACRO_ADDR;
extern struct Moby *moby_list __asm__("D_L01_0015FFE4") MACRO_ADDR;
extern s32 tick_countdown_32(s32 *) __asm__("FUN_001f9740");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern f32 distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern s32 help_request(s32, s32) __asm__("FUN_L00_00203908");
extern s32 FUN_L00_002d8128(void *);
extern s32 FUN_L01_00275690(f32, void *);

extern int D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
extern s32 current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
/* Novalis tutorial hint triggers: plays help messages and counts usage stats. */
void FUN_L01_0030acb8(struct Moby *moby) {
    s32 *data;
    struct Moby *m;
    struct Moby *m2;
    s32 t;

    data = (s32 *)moby->pvars;
    tick_countdown_32(&data[6]);
    switch (moby->state) {
    case 0:
        data[5] = 0;
        moby->update_dist = 0xFF;
        moby->state = 1;
        data[10] = 0;
        break;
    case 1:
        if (level_frame_count < scale_game_frames(400) && usage_stats.stat[12].count != 0 && usage_stats.stat[11].count == 0) {
            if (scale_game_frames(D_0015EEA4_macro) - usage_stats.stat[12].unk2 * 600 < scale_game_frames(1100) && help_records[0x51].v >= 0) {
                help_request(0x4E27, 0x51);
            }
        }
        if (level_frame_count < scale_game_frames(400) && usage_stats.stat[29].count != 0) {
            if (scale_game_frames(D_0015EEA4_macro) - usage_stats.stat[29].unk2 * 600 < scale_game_frames(1100) && help_records[0x72].v >= 0) {
                help_request(0x4E28, 0x72);
            }
        }
        if (cutscene_mode_macro == 0) {
            if (level_available[3] != 0 && level_available[2] == 0 && help_state.state == 0 &&
                help_state.cur == -1 && help_records[4].id == 0) {
                help_request(0x3E8, 4);
            } else if (level_available[3] == 0 && level_available[2] != 0 && help_state.state == 0 &&
                       help_state.cur == -1 && help_records[5].id == 0) {
                help_request(0x3E9, 5);
            } else if (level_available[3] != 0 && level_available[2] != 0 && help_state.state == 0 &&
                       help_state.cur == -1 && help_records[6].id == 0 && help_records[4].id == 0) {
                help_request(0x3EA, 6);
            }
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, data[15]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) && help_state.state == 0 &&
            help_state.cur == -1) {
            t = scale_game_frames(D_0015EEA4_macro) - usage_stats.stat[9].unk2 * 600;
            /* "< 1" rather than "!": cc1 would otherwise test the bit as (v >> level) & 1 */
            if (scale_game_frames(216000) < t && (u32)(help_records[0x43].v & (1 << current_level_index_macro)) < 1) {
                help_request(0x3EF, 0x43);
            }
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, data[14]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) && D_L01_0015F68C < 3 &&
            help_state.state == 0 && help_state.cur == -1 && help_records[0x40].id == 0) {
            help_request(0x3EC, 0x40);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, data[14]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) && hero.state.current == 1 &&
            D_L01_0015F688 == -1 && help_state.state == 0 && help_state.cur == -1 &&
            help_records[0x40].id != 0 && help_records[0x41].id == 0) {
            help_request(0x3ED, 0x41);
        }
        if (help_records[0x42].id < 2 && is_point_inside_clip_volume(&hero.motion.pos, data[12]) && hero.state.prev == 0x3B &&
            hero.state_timer == 0) {
            for (m = moby_list; m != 0; m = m->next) {
                if (m->oclass == 0x118 && distance_xyz(&m->pos, &hero.motion.pos) < 3.0f &&
                    *(f32 *)m->pvars != 1.0f) {
                    data[13]++;
                    if (data[13] == 3 || data[13] == 6) {
                        help_request(0x3EE, 0x42);
                    }
                }
            }
        }
        if (hero.state.control_mode == 0x12 && data[10] == 0 && help_state.state == 0 &&
            help_state.cur == -1 &&
            (is_point_inside_clip_volume(&hero.motion.pos, data[8]) || is_point_inside_clip_volume(&hero.motion.pos, data[9]))) {
            data[11]++;
            if (is_point_inside_clip_volume(&hero.motion.pos, data[8]) && scale_game_frames(240) < data[11] &&
                usage_stats.stat[22].count == 0) {
                help_request(0x3F0, 0x44);
                if (usage_stats.stat[22].count < 0xFFFF) {
                    usage_stats.stat[22].count++;
                }
                t = scale_game_frames(D_0015EEA4_macro) / 600;
                if (usage_stats.stat[22].unk2 < t) {
                    usage_stats.stat[22].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                }
                usage_stats.stat[22].level_mask = usage_stats.stat[22].level_mask | (1 << current_level_index_macro) | 0x80000000;
            } else if (is_point_inside_clip_volume(&hero.motion.pos, data[9]) && scale_game_frames(360) < data[11] &&
                       usage_stats.stat[23].count == 0) {
                help_request(0x3F0, 0x44);
                if (usage_stats.stat[23].count < 0xFFFF) {
                    usage_stats.stat[23].count++;
                }
                t = scale_game_frames(D_0015EEA4_macro) / 600;
                if (usage_stats.stat[23].unk2 < t) {
                    usage_stats.stat[23].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                }
                usage_stats.stat[23].level_mask = usage_stats.stat[23].level_mask | (1 << current_level_index_macro) | 0x80000000;
            }
        } else if (hero.state.control_mode == 0x11) {
            help_records[0x44].id = 0xFFFF;
        } else {
            data[11] = 0;
        }
        for (m2 = moby_list; m2 != 0; m2 = m2->next) {
            if (m2->oclass == 0x326 && distance_xyz(&m2->pos, &hero.motion.pos) < 10.0f && help_state.state == 0 &&
                help_state.cur == -1 && help_records[1].id == 0 && help_records[2].id == 0 &&
                usage_stats.stat[13].count == 0 && FUN_L00_002d8128(m2) == 0) {
                if (hero.health.hp < hero_max_health) {
                    if (usage_stats.stat[13].count < 0xFFFF) {
                        usage_stats.stat[13].count++;
                    }
                    t = scale_game_frames(D_0015EEA4_macro) / 600;
                    if (usage_stats.stat[13].unk2 < t) {
                        usage_stats.stat[13].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                    }
                    usage_stats.stat[13].level_mask = usage_stats.stat[13].level_mask | (1 << current_level_index_macro) | 0x80000000;
                    help_request(1, 1);
                } else if (FUN_L01_00275690(255.0f, m2) != 0) {
                    help_request(2, 2);
                }
            }
        }
        break;
    }
}
#include "rnc/gameplay/entities/moby.h"

/* pvars of the lava blob FUN_L01_0030ba18 moves */
typedef struct {
    Vec4f vel;         /* 0x00 */
    Vec4f spin;        /* 0x10 */
    s32 timer;         /* 0x20 */
    s32 owner;         /* 0x24 */
} BlobVars;

typedef struct {
    u8 pad0[0x1C];
    s32 have_normal;   /* 0x1C */
    u8 pad20[0x20];
    Vec4f normal;      /* 0x40 */
} BlobHitInfo;

extern f32 D_L01_00162058 __attribute__((sda));
extern f32 D_L01_0016205C __attribute__((sda));
extern f32 D_L01_00162060 __attribute__((sda));
extern BlobHitInfo D_L01_001742C0;
extern s32 D_L01_0015F580;
extern void FUN_L01_0030bfa8(void);
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 FUN_001f99c0(f32);
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern s32 tick_countdown_32(s32 *) __asm__("FUN_001f9740");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void clear_vector(void *) __asm__("FUN_001f99f8");
extern void FUN_L00_0025e450(void *, void *, void *, f32, f32, s32, s32, s32, f32, f32, f32, s32, f32, f32,
                             s32, s32, s32, s32);
extern void spark_cbb0(void *, void *, s32, s32, f32, s32, s32) __asm__("FUN_L00_0026cbb0");
extern int FUN_L00_00257b90(int lo, int hi);
extern u8 *spawn_puff(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

void FUN_L01_0030ba18(struct Moby *m) {
    BlobVars *v;
    s32 hit;
    s32 i;
    s32 n;
    s32 c;
    s32 alpha;
    s32 grey;
    f32 s;
    u8 *p;
    u8 *q;

    v = (BlobVars *)m->pvars;
    if (random_integer_below(2)) {
        enqueue_callback_list_1(FUN_L01_0030bfa8, m);
    }
    add_vector_xyz(&m->pos, &m->pos, v);
    if (FUN_001f99c0(v->vel.x) > D_L01_00162060 * frame_scale) {
        v->vel.x *= (D_L01_0016205C - 1.0f) * frame_scale + 1.0f;
    }
    if (FUN_001f99c0(v->vel.y) > D_L01_00162060 * frame_scale) {
        v->vel.y *= (D_L01_0016205C - 1.0f) * frame_scale + 1.0f;
    }
    if (v->vel.z > 0.0f) {
        v->vel.z *= (D_L01_0016205C - 1.0f) * frame_scale + 1.0f;
    }
    v->vel.z -= D_L01_00162058 * frame_time_sq;
    m->rot.x = fast_add_rotations(m->rot.x, v->spin.x);
    m->rot.y = fast_add_rotations(m->rot.y, v->spin.y);
    m->rot.z = fast_add_rotations(m->rot.z, v->spin.z);
    if (m->pos.x < 2.0f || m->pos.x > 1021.0f || m->pos.y < 2.0f || m->pos.y > 1021.0f ||
        m->pos.z < 2.0f || m->pos.z > 1021.0f) {
        mark_moby_for_removal(m);
        return;
    }
    hit = coll_sphere(2.0f, &m->pos, 0, (void *)v->owner);
    if (tick_countdown_32(&v->timer) || hit) {
        if (hit) {
            if (D_L01_001742C0.have_normal) {
                FUN_L00_001ff660(v, v, &D_L01_001742C0.normal);
            } else {
                clear_vector(v);
            }
        }
        FUN_L00_0025e450(m, v, &m->pos, 0.0f, 0.0f, 0x14, 3, 4, 4.0f, 2.0f, 100000.0f, 0, 3.0f, 15.0f, 1, 1,
                         -1, 0);
        mark_moby_for_removal(m);
        return;
    }
    s = random_float_between(0.5f, 1.5f) * 210000.0f;
    spark_cbb0(&m->pos, &D_L01_0015F580, 0x4F007FFF, 0x1FFFFFFF, s,
               FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)), 1);
    alpha = 0x60000000;
    for (i = 0; i < 2; i++) {
        n = random_integer_below(6);
        if (random_integer_below(2)) {
            n = -n;
        }
        s = random_float_between(40000.0f, 100000.0f);
        c = FUN_L00_00257b90(0x30, 0xFF);
        grey = (c << 8) | alpha;
        p = spawn_puff(&m->pos, 0.025f, 1.0f, 1.0f, n, &D_L01_0015F580, s, c | ((c << 16) | grey));
        if (p != NULL) {
            p[3] = 0x44;
            q = p + 0x20;
            *(s16 *)(p + 0xA) = scale_game_frames(60);
            *(s32 *)(q + 4) = 2;
            q[0xA] = 0x60;
            q[0xB] = p[0xA];
        }
    }
}
#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    u8 pad0[0x10];
    f32 spin_x;
    f32 spin_y;
    f32 spin_z;
    u8 pad1c[4];
    s32 timer;
    void *parent;
} DebrisVars;

typedef struct {
    u8 pad0[0x10];
    u128 pos;
    u8 state;
    u8 pad21[0xB];
    f32 scale;
    u8 b30;
    u8 b31;
    s16 h32;
    u8 pad34[0xC];
    u128 rot;
    u8 pad50[0x28];
    DebrisVars *vars;
} DebrisMoby;

extern DebrisMoby *create_moby(s32 oclass) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8(DebrisMoby *);
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 randf_sym(f32 lo, f32 hi) __asm__("FUN_L00_00257c48");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");

DebrisMoby *FUN_L01_0030be70(DebrisMoby *src, void *dir, f32 scale) {
    DebrisMoby *m;
    DebrisVars *v;

    m = create_moby(0x5E6);
    if (m != NULL) {
        v = m->vars;
        v->parent = src;
        qcopy(&m->pos, &src->pos);
        m->rot = src->rot;
        m->state = 0;
        m->b30 = 0xFF;
        m->h32 = 0xFF;
        m->b31 = 1;
        m->scale = m->scale * scale;
        fast_vec_normalize(v, dir, random_float_between(0.8f, 1.2f));
        v->spin_x = randf_sym(0.0f, 0.008726646f);
        v->spin_y = randf_sym(0.05235988f, 0.13962634f);
        v->spin_z = randf_sym(0.0f, DEG_TO_RAD);
        v->timer = scale_game_frames(200);
        FUN_L00_00250df8(m);
    }
    return m;
}
/* Cutscene state at D_L01_0016CCE0 (as struct L10Data in l10/gameplay/vendor/002ea1f0.c). */
typedef struct {
    u8 pad0[0x30];
    s32 scene;
    s32 frame;
    u8 pad38[0x178 - 0x38];
    struct Moby *slot[5];
} CutsceneFx;

typedef union {
    u128 q;
    f32 f[4];
} FxVec;

/* FlockTail/FlockPart in l00/gameplay/entities/veldin_robot_enemy.c */
typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[2];
    u8 unkA;
    u8 unkB;
} FxSparkTail;

typedef struct {
    u8 pad0[9];
    u8 unk9;
    s16 unkA;
    u8 padC[0x20 - 0xC];
    FxSparkTail tail;
} FxSpark;

extern CutsceneFx fx_cutscene __asm__("D_L01_0016CCE0");
/* MACRO_ADDR: retail rebuilds this address with lui/addiu at each use */
extern u128 fx_prev_pos __asm__("D_L01_00162070") MACRO_ADDR;
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void cutscene_set_camera_target(struct Moby *) __asm__("FUN_L00_002637f8");
extern void moby_point_position(struct Moby *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern s32 random_int_between(s32, s32) __asm__("FUN_L00_00257b90");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern char *spawn_splash(f32, f32, void *, void *, void *) __asm__("FUN_L00_002718d0");
extern char *spawn_burst(void *, f32) __asm__("FUN_L00_002d7e90");
extern void spawn_debris_chunk(void *, void *, s32, s32) __asm__("FUN_L00_0026f548");
extern FxSpark *spawn_spark(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");
extern void spawn_explosion(void *, void *, void *, f32, f32, s32, s32, s32, f32, f32, f32, f32, s32, f32, s32, s32, s32,
                       s32) __asm__("FUN_L00_0025e450");
extern void draw_trail(void *, void *, void *, f32, f32) __asm__("FUN_L00_00263b70");
extern void clear_vector(void *) __asm__("FUN_001f99f8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void render_environment_mapped_object(void) __asm__("FUN_002327a0");

/* Cutscene effects: camera targets, splashes, sparks and trails keyed on the cutscene scene and frame. */
void FUN_L01_0030c190(struct Moby *moby) {
    FxVec a;
    FxVec b;
    FxVec c;
    FxVec d;
    struct Moby *m;
    char *e;
    FxSpark *s;
    f32 ang;
    f32 r;
    s32 i;
    s32 j;
    s32 n;
    s32 color;
    s32 spread;
    s32 step;
    s32 node;

    switch (moby->state) {
    case 0:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case 1:
        if (cutscene_mode_macro == 2) {
            if (fx_cutscene.scene == 1 || (fx_cutscene.scene == 2 && fx_cutscene.frame <= scale_game_frames(148)) ||
                fx_cutscene.scene == 3 || fx_cutscene.scene == 4) {
                s32 target = 3;

                if (fx_cutscene.scene == 1 || fx_cutscene.scene == 3 || fx_cutscene.scene == 4) {
                    target = 3;
                } else if (fx_cutscene.scene == 2) {
                    target = 2;
                }
                cutscene_set_camera_target(fx_cutscene.slot[target]);
            }
            if (fx_cutscene.scene == 1) {
                if (fx_cutscene.frame >= scale_game_frames(472) && fx_cutscene.frame <= scale_game_frames(537) &&
                    fx_cutscene.slot[0] != 0) {
                    moby_point_position(fx_cutscene.slot[0], 1, &a);
                    e = spawn_splash(random_float_between(0.5f, 2.0f), random_integer_below(2) ? 2.0f : -2.0f, &a, &D_L01_0015F580,
                                        &a.f[2]);
                    if (e != 0) {
                        *(s16 *)(e + 0xA) = scale_game_frames(60);
                    }
                }
                if (fx_cutscene.scene == 1 && fx_cutscene.frame == scale_game_frames(472) && fx_cutscene.slot[0] != 0) {
                    moby_point_position(fx_cutscene.slot[0], 1, &a);
                    {
                        char *burst = spawn_burst(&a, 3.0f);

                        if (burst != 0) {
                            burst[0x23] = 0x70;
                        }
                    }
                    for (i = 0; i < 16; i++) {
                        ang = random_angle_radians();
                        r = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
                        b.f[0] = fast_cos(ang) * r;
                        b.f[1] = fast_sin(ang) * r;
                        b.f[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
                        n = random_int_between(90, 120);
                        spawn_debris_chunk(&a, &b, random_integer_below(2), n);
                    }
                }
            }
            if (fx_cutscene.scene == 4) {
                m = fx_cutscene.slot[4];
                if (m != 0 && m->oclass == 0x213) {
                    enqueue_callback_list_1(render_environment_mapped_object, m);
                    if (fx_cutscene.frame >= scale_game_frames(850) && fx_cutscene.frame <= scale_game_frames(1160)) {
                        color = 0x7F204080;
                        for (node = 1; node < 7; node++) {
                            moby_point_position(m, node, &a);
                            for (j = 0; j < 2; j++) {
                                spread = random_integer_below(16);
                                s = spawn_spark(&a, 0.1f, 1.0f, 0.9f, random_integer_below(2) == 0 ? spread : -spread,
                                                   &D_L01_0015F580, 200000.0f, color);
                                if (s != 0) {
                                    FxSparkTail *t = &s->tail;

                                    if (fx_cutscene.frame >= scale_game_frames(1100)) {
                                        s->unkA = scale_game_frames(30);
                                    } else {
                                        s->unkA = scale_game_frames(6);
                                    }
                                    s->unk9 = truncate_float_to_s32(4.0f) + 0x40;
                                    t->unk4 = 2;
                                    t->unkA = color >> 24;
                                    t->unkB = s->unkA;
                                }
                            }
                        }
                    }
                }
            }
            if (fx_cutscene.scene == 5 && fx_cutscene.frame < scale_game_frames(120)) {
                m = fx_cutscene.slot[2];
                if (m != 0) {
                    a.q = 0;
                    moby_point_position(m, 5, &b);
                    if (fx_cutscene.frame >= 2) {
                        subtract_vector_xyz(&a, &b, &fx_prev_pos);
                        qcopy(&fx_prev_pos, &b);
                    } else {
                        qcopy(&fx_prev_pos, &b);
                    }
                    if (fx_cutscene.frame == scale_game_frames(118)) {
                        clear_vector(&c);
                        spawn_explosion(m, &c, &b, 0.0f, 0.0f, 3, 200, 200, 5.0f, 2.5f, 9.0f, 1.0f, -1, 60.0f, 0, 60, -1, 0);
                        qcopy(&d, &b);
                        d.f[2] = d.f[2] - 1.0f;
                        spawn_explosion(m, &c, &d, 0.0f, 0.0f, 300, 2, 2, 1.0f, 0.5f, 9.0f, 1.0f, -1, 60.0f, 0, 0, -1, 0);
                    }
                    enqueue_callback_list_1(render_environment_mapped_object, m);
                    moby_point_position(m, 1, &c);
                    moby_point_position(m, 2, &d);
                    subtract_vector_xyz(&c, &c, &a);
                    subtract_vector_xyz(&d, &d, &a);
                    scale_vector_xyz(&a, &a, 0.25f);
                    for (step = 0; step < 4; step++) {
                        draw_trail(&c, &c, &D_L01_0015F580, 150000.0f, 60000.0f);
                        draw_trail(&d, &d, &D_L01_0015F580, 150000.0f, 60000.0f);
                        add_vector_xyz(&c, &c, &a);
                        add_vector_xyz(&d, &d, &a);
                    }
                }
            }
        }
        break;
    }
}
typedef struct {
    u8 pad0[0x2C];
    f32 f2c;
} TriggerHit;

typedef struct {
    u8 pad0[0x10];
    u8 pos[0x10];
    u8 state;
    u8 pad21[0x1F];
    u8 rot[0x10];
} TriggerMoby;

extern s32 D_L01_0015F580;
void *find_moby_hit(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("FUN_0022da68");
extern void FUN_L01_002787a0();
extern void *FUN_L00_00263fd8(void *, s32, void *, void *, s32, s32, void *, void *, f32, void *);
extern void FUN_L01_00278e20();
void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

void tall_shroom_update(TriggerMoby *obj) __asm__("FUN_L01_0030d0f0");
void tall_shroom_update(TriggerMoby *obj) {
    TriggerHit *hit;
    s32 flag;

    flag = 0;
    hit = find_moby_hit(obj, 0x10000, 0);
    switch (obj->state) {
    case 0:
        obj->state = 1;
        break;
    case 1:
        if (hit != 0 && hit->f2c > 0.0f) {
            flag = 1;
        }
        if (flag) {
            obj->state = 2;
        }
        break;
    case 2:
        allocate_voice_for_target_entry(0, 0, obj);
        FUN_L01_002787a0(obj);
        FUN_L00_00263fd8(obj, 0x717, obj->pos, obj->rot, 0, 0, &D_L01_0015F580, &D_L01_0015F580,
                         0.0f, &D_L01_0015F580);
        FUN_L01_00278e20(obj, 0x718);
        mark_moby_for_removal(obj);
        break;
    }
}
typedef struct {
    f32 x, y, z, w;
} EnvVec;

extern EnvVec D_L01_00167240;
extern f32 D_L01_00162110[2];
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");

void FUN_L01_0030ef18(s32 n, f32 (*normals)[3], f32 (*verts)[3], s32 unused, f32 (*uv)[2]) {
    EnvVec eye;
    EnvVec pos;
    EnvVec refl;
    EnvVec nrm;
    s32 i;
    f32 len;

    for (i = 0; i < n; i++) {
        pos.w = 1.0f;
        pos.x = verts[i][0];
        pos.y = verts[i][1];
        pos.z = verts[i][2];
        fast_vec_sub(&eye, &pos, &D_L01_00167240);
        fast_vec_normalize(&eye, &eye, 1.0f);
        nrm.w = 1.0f;
        nrm.x = -normals[i][0];
        nrm.y = -normals[i][1];
        nrm.z = -normals[i][2];
        fast_vec_normalize(&nrm, &nrm, 1.0f);
        fast_vec_scale(&refl, &nrm, fast_vec_dot(&nrm, &eye) * 2.0f);
        fast_vec_sub(&refl, &eye, &refl);
        fast_vec_normalize(&refl, &refl, 1.0f);
        refl.z += 1.0f;
        len = fast_vec_length(&refl) * 2.0f;
        uv[i][0] = (refl.x / len + 0.5f) * 2.0f + D_L01_00162110[0];
        uv[i][1] = (refl.y / len + 0.5f) * 2.0f + D_L01_00162110[1];
    }
}
extern s32 D_L01_00202EA0[];
extern s32 D_L01_00208108[];
extern s32 D_L01_00208120[];
extern s32 D_L01_00208138[];
extern s32 D_L01_00208150[];
extern u8 D_L01_00208170[];
extern s32 get_effect_texture(s32) __asm__("FUN_001f44b8");
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");
void FUN_L01_0030ef18();
void FUN_L00_001fde98(s32, s32, s32, void *, s32);

void FUN_L01_0030f0e0(void) {
    s32 i;

    vu1_add_g_sregister(6, get_effect_texture(0x28));
    vu1_add_g_sregister(0x42, 0x2000000064);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 5; i++) {
        FUN_L01_0030ef18(D_L01_00202EA0[i], D_L01_00208138[i], D_L01_00208108[i], D_L01_00208120[i],
                         D_L01_00208170);
        FUN_L00_001fde98(D_L01_00202EA0[i], D_L01_00208108[i], D_L01_00208150[i], D_L01_00208170,
                         1);
    }
}
typedef struct {
    u8 pad0[0x20];
    u8 state;
} ScrollMoby;

extern f32 frame_delta_time __asm__("D_0015ED7C");
void FUN_L01_0030f0e0(void);
void enqueue_callback_list_1(void *fn, void *arg) __asm__("FUN_001f4600");

void FUN_L01_0030f208(ScrollMoby *m) {
    f32 d;

    switch (m->state) {
    case 0:
        D_L01_00162110[0] = 0.0f;
        D_L01_00162110[1] = 0.0f;
        m->state = 1;
        break;
    case 1:
        d = frame_delta_time * 0.025f;
        D_L01_00162110[0] += d;
        D_L01_00162110[1] += d;
        if (D_L01_00162110[0] > 1.0f) {
            D_L01_00162110[0] -= 1.0f;
        }
        if (D_L01_00162110[0] < -1.0f) {
            D_L01_00162110[0] += 1.0f;
        }
        if (D_L01_00162110[1] > 1.0f) {
            D_L01_00162110[1] -= 1.0f;
        }
        if (D_L01_00162110[1] < -1.0f) {
            D_L01_00162110[1] += 1.0f;
        }
        enqueue_callback_list_1(FUN_L01_0030f0e0, m);
        break;
    }
}
