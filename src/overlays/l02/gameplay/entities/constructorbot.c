/* Level 02 moby classes: constructorbot. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "rnc/gameplay/entities/moby.h"
#include "qcopy.h"
#include "rnc/overlay/level_labels.h"
typedef struct {
    Vec4f vel;
    s32 timer;
} ShardVars;
typedef struct {
    u8 pad0[0x5C];
    u8 *tint;
} LevelFx;
#include "rnc/overlay/l02/gameplay/entities/constructorbot_globals.h"
extern f32 shard_vertical_velocity_approach_scale_gp __asm__("D_L02_00161F58") __attribute__((sda));
s32 D_L02_00161F5C = 0x30704020;
s32 D_L02_00161F60 = 0x50e0e0e0;
extern LevelFx D_L02_001B2680;
extern void mark_moby_for_removal(struct Moby *) __asm__("FUN_0020c828");
extern void vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern f32 vec_len_xy(void *) __asm__("FUN_001f9b20");
extern f32 angle_atan2(f32, f32) __asm__("FUN_001f9e90");
extern f32 rand_range(f32, f32) __asm__("FUN_002132a8");
extern s32 float_to_int(f32) __asm__("FUN_001fa6d0");
extern s32 scale_frames(s32) __asm__("FUN_001f96f8");
extern void count_down(s32 *) __asm__("FUN_001f9740");
extern u8 *spawn_particle(void *, void *, s32, s32, s32, s32, f32, f32, f32, f32, f32) __asm__("FUN_L00_002712b8");
extern void rand_vec(void *, f32, f32) __asm__("FUN_L00_00257d78");

/* Shard debris update: drifts along its velocity, sheds particle trails and expires when its timer runs out. */
void update_shard_debris(struct Moby *moby) __asm__("FUN_L02_002ebf20");

void update_shard_debris(struct Moby *m) __asm__("FUN_L02_002ebf20");
void update_shard_debris(struct Moby *moby) {
    enum {
        SHARD_STATE_REMOVE = 0,
        SHARD_STATE_ACTIVE = 1
    };
    ShardVars *shard_vars = (ShardVars *)moby->pvars;
    Vec4f trail_position, back, half, jit;
    u8 *particle;
    s32 i;

    switch (moby->state) {
    case SHARD_STATE_REMOVE:
        mark_moby_for_removal(moby);
        return;
    case SHARD_STATE_ACTIVE:
        break;
    default:
        return;
    }
    vec_add(&moby->pos, &moby->pos, &shard_vars->vel);
    approach_value(&shard_vars->vel.z, 0.0f, shard_vertical_velocity_approach_scale_gp * frame_time_sq);
    moby->rot.y = 1.5707964f - angle_atan2(vec_len_xy(&shard_vars->vel), shard_vars->vel.z);
    qcopy(&trail_position, &moby->pos);
    vec_scale(&half, &shard_vars->vel, 0.5f);
    vec_scale(&back, &shard_vars->vel, -0.1f);
    for (i = 1; i >= 0; i--) {
        s32 particle_frame_count = scale_frames(float_to_int(rand_range(20.0f, 60.0f)));
        particle = spawn_particle(&trail_position, &back, particle_frame_count, D_L02_00161F5C >> 24, D_L02_00161F5C & 0xFFFFFF, 3,
                                 rand_range(300000.0f, 500000.0f), 10000.0f, 1.0f, -0.0002f, 0.0f);
        if (particle) {
            particle[2] = *D_L02_001B2680.tint;
            particle[9] = float_to_int(4.0f) - 0x80;
        }
        vec_add(&trail_position, &trail_position, &half);
        rand_vec(&jit, 0.5f, 1.5f);
        vec_add(&trail_position, &trail_position, &jit);
    }
    particle = spawn_particle(&moby->pos, &back, scale_frames(6), D_L02_00161F60 >> 24, D_L02_00161F5C & 0xFFFFFF, 3,
                             250000.0f, 5000.0f, 1.0f, -0.0004f, 0.0f);
    if (particle)
        particle[9] = float_to_int(4.0f) - 0x80;
    if (moby->visible == 0)
        count_down(&shard_vars->timer);
    else
        shard_vars->timer = scale_frames(60);
    if (shard_vars->timer == 0)
        mark_moby_for_removal(moby);
}
#include "qcopy.h"
extern char *create_moby_c(int) __asm__("FUN_0020c4f8");
extern void FUN_L00_002502f0_c(char *, int, int, int) __asm__("FUN_L00_002502f0");
extern float atan2_c(float, float) __asm__("FUN_001f9e90");
extern float vlen_xy_c(void *) __asm__("FUN_001f9b20");
extern int scale_frames_c(int) __asm__("FUN_001f96f8");
int D_L02_00161F4C = 0xd0;
int D_L02_00161F50 = 0x60;
int D_L02_00161F54 = 0x10;

/* Spawns a projectile moby from src at pos, heading along dir, with its lifetime in the moby data. */
char *spawn_projectile_moby(char *source_moby, void *pos, float *dir) __asm__("FUN_L02_002ec228");

char *spawn_projectile_moby(char *source_moby, void *spawn_position, float *projectile_direction) {
    char *moby;
    char *data;
    moby = create_moby_c(0x4B8);
    if (moby != 0) {
        moby[0x31] = 1;
        *(unsigned char *)(moby + 0x30) = *(short *)(moby + 0x32) = 0xFF;
        FUN_L00_002502f0_c(moby, D_L02_00161F4C, D_L02_00161F50, D_L02_00161F54);
        *(short *)(moby + 0x34) = *(unsigned short *)(source_moby + 0x34);
        qcopy(moby + 0x10, spawn_position);
        *(float *)(moby + 0x48) = atan2_c(projectile_direction[0], projectile_direction[1]);
        *(float *)(moby + 0x44) = atan2_c(vlen_xy_c(projectile_direction), projectile_direction[2]);
        moby[0x20] = 1;
        data = *(char **)(moby + 0x78);
        qcopy(data, projectile_direction);
        *(int *)(data + 0x10) = scale_frames_c(0x3C);
    }
    return moby;
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* UpdateMoby_1324: tutorial hint triggers around the player */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002E21F8.c: func_L02_002EFCC8), where it is exact; names translated to the US level program. */

typedef struct {
    int pad0;
    int pad4;
    int active;
    char rest[0x40];
} Entry_L02_0017A140;

typedef struct {
    unsigned char b[0x20];
} Flags_0013D5C8;

extern Entry_L02_0017A140 D_L02_0017A0C0[];

extern int help_request(int a, int b) __asm__("FUN_L00_00203908");
extern int allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern unsigned char D_0013D4C0[];
extern unsigned char D_0014C050[][16];
extern unsigned char D_0015EDDC[4];
extern unsigned char opt_cam_invert_x[4] __asm__("D_0015EDE0");
extern int D_0015EEA4;
extern unsigned char D_0013D388[];
extern unsigned char D_0013D408[];
extern unsigned short D_001417A2 __attribute__((section(".data")));
extern unsigned short D_00141888 __attribute__((section(".data")));
extern unsigned short D_001418E8 __attribute__((section(".data")));
extern void queue_dialogue_message(int arg0, int arg1) __asm__("FUN_L00_00263d40");
extern char D_00141968[];
extern s32 D_0014EE90[];

void update_tutorial_hint_triggers(struct Moby *moby) __asm__("FUN_L02_002ee890");

void update_tutorial_hint_triggers(struct Moby *moby) {
    enum {
        AIRBORNE_DIALOGUE_UNARMED = 0,
        AIRBORNE_DIALOGUE_ARMED = 1
    };
    int *data = (int *)moby->pvars;
    moby->update_dist = 0xFF;

    if (is_point_inside_clip_volume(&hero.motion.pos, data[0x17]) != 0 &&
        ((unsigned int)hero.state.control_mode) < 2) {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x278) == 0 && D_001418E8 == 0) {
            int count = 0;
            int i;
            for (i = 0; i < 37; i++) {
                if (D_L02_0017A0C0[i].active == 0 && i != 8 && i != 24) {
                    if ((D_0013D4C0)[i] != 0) {
                        count++;
                    }
                }
            }
            if (count >= 2) {
                help_request(0x4E25, 0x4F);
            }
        }
    }

    switch (data[0x15]) {
    case AIRBORNE_DIALOGUE_UNARMED:
        if (is_point_inside_clip_volume(&hero.motion.pos, data[0x12]) != 0) {
            if (hero.state.current == 0x2C) {
                data[0x15] = 1;
            }
        }
        break;
    case AIRBORNE_DIALOGUE_ARMED: {
        struct Hero *h = &hero;
        if (h->grounded_frames != 0) {
            data[0x15] = 0;
        } else if (is_point_inside_clip_volume(&h->motion.pos, data[0x13]) != 0) {
            unsigned char *dialogue_latch_bytes = D_0013D408;
            if (dialogue_latch_bytes[1] == 0) {
                dialogue_latch_bytes[1] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        break;
    }
    }

    {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x60) == 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, data[4]) != 0) {
                int lvl = data[0];
                if (D_0014C050[current_level_index][lvl] != 0xFF) {
                    unsigned char *f = D_0013D4C0;
                    if (f[0x10] != 0 && ((int *)(((char *)&D_0014EE90)))[lvl] >= 6) {
                        int t = scale_game_frames(D_0015EEA4) - D_001417A2 * 600;
                        if (scale_game_frames(0x4650) < t) {
                            help_request(0x7D2, 0xC);
                        }
                    }
                }
            }
        }
    }

    if (is_point_inside_clip_volume(&hero.motion.pos, data[0x10]) != 0) {
        data[0x16] = 1;
    }
    if (data[0x16] != 0) {
        if (D_00141888 < 3) {
            char *b = ((char *)&D_00141968);
            if (!(*(unsigned int *)(b + 0x264) & (1 << current_level_index))) {
                help_request(0x4E22, 0x4C);
            }
        }
        if (D_0015EDDC[0] != 0 && opt_cam_invert_x[0] != 0) {
            char *b = ((char *)&D_00141968);
            if (!(*(unsigned int *)(b + 0x3B4) & (1 << current_level_index))) {
                help_request(0x4E2C, 0x76);
            }
        } else {
            char *b = ((char *)&D_00141968);
            *(unsigned short *)(b + 0x3B0) = 0xFFFF;
        }
    }

    {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x68) == 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, data[4]) != 0) {
                int lvl = data[0];
                if (D_0014C050[current_level_index][lvl] != 0xFF) {
                    unsigned char *f = D_0013D4C0;
                    if (f[0x10] == 0 && (float)((int *)(((char *)&D_0014EE90)))[lvl] >= 3.0f) {
                        help_request(0x7D3, 0xD);
                    }
                }
            }
        }
    }

    {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x80) == 0) {
            char *g = (char *)&hero.motion.pos;
            char *h;
            if (is_point_inside_clip_volume(g, data[6]) != 0) {
                *(unsigned short *)(b + 0x80) = 0xFFFF;
            }
            h = g - 0x80;
            if (*(int *)(h + 0x2084) != 0x2C && *(int *)(h + 0x2090) == 0x2C &&
                *(int *)(h + 0x198) == 1) {
                data[0xC]++;
            }
            if (data[0xC] >= 2) {
                if (is_point_inside_clip_volume(&hero.motion.pos, data[7]) != 0) {
                    help_request(0x7D6, 0x10);
                }
            }
        }
    }

    if (((Flags_0013D5C8 *)(D_0013D4C0))->b[0xC] == 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data[5]) != 0) {
            char *b = ((char *)&D_00141968);
            int t = scale_game_frames(D_0015EEA4) - *(unsigned short *)(b + 0x8A) * 600;
            if ((int)(scale_game_frames(0x12) * 60.0f) < t ||
                *(unsigned short *)(b + 0x8A) * 600 == 0) {
                help_request(0x7D7, 0x11);
            } else {
                int n = scale_game_frames(D_0015EEA4) / 600;
                if (*(unsigned short *)(b + 0x8A) < n) {
                    *(unsigned short *)(b + 0x8A) = scale_game_frames(D_0015EEA4) / 600;
                }
            }
        }
    }
    if (((Flags_0013D5C8 *)(D_0013D4C0))->b[0xC] != 0 &&
        is_point_inside_clip_volume(&hero.motion.pos, data[5]) != 0) {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x90) != 0) {
            int t = scale_game_frames(D_0015EEA4) - *(unsigned short *)(b + 0x92) * 600;
            if ((int)(scale_game_frames(0x12) * 60.0f) < t ||
                *(unsigned short *)(b + 0x92) * 600 == 0) {
                help_request(0x7D8, 0x12);
            } else {
                int n = scale_game_frames(D_0015EEA4) / 600;
                if (*(unsigned short *)(b + 0x92) < n) {
                    *(unsigned short *)(b + 0x92) = scale_game_frames(D_0015EEA4) / 600;
                }
            }
        } else {
            int n;
            (*(unsigned short *)(b + 0x90))++;
            n = scale_game_frames(D_0015EEA4) / 600;
            if (*(unsigned short *)(b + 0x92) < n) {
                *(unsigned short *)(b + 0x92) = scale_game_frames(D_0015EEA4) / 600;
            }
            *(int *)(b + 0x94) = *(int *)(b + 0x94) | (1 << current_level_index) | 0x80000000;
        }
    } else if (is_point_inside_clip_volume(&hero.motion.pos, data[7]) != 0) {
        char *b = ((char *)&D_00141968);
        *(unsigned short *)(b + 0x90) = 0xFFFF;
    }

    {
        char *b = ((char *)&D_00141968);
        unsigned short v = *(unsigned short *)(b + 0x230);
        if (v == 0 || *(unsigned short *)(b + 0x238) == 0) {
            struct Hero *h = &hero;
            if (h->state.current == 0x72) {
                char *ground_moby = ((char *)h->ground_moby);
                if (ground_moby != 0 && *(short *)(ground_moby + 0xA6) == 0x267) {
                    if (v == 0) {
                        help_request(0x7DB, 0x46);
                    } else if (*(unsigned short *)(b + 0x238) == 0) {
                        data[0x14]++;
                        if (scale_game_frames(600) < data[0x14]) {
                            help_request(0x7DC, 0x47);
                        }
                    }
                }
            }
        }
    }

    {
        unsigned char *g = D_0013D388;
        if (g[0x14] == 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, data[0x11]) != 0) {
                g[0x14] = 1;
            }
        }
    }
    qcopy(data + 8, &hero.motion.pos);
}
extern void build_rotation_matrix(void *out, void *rot) __asm__("FUN_001fa030");
extern s32 sphere_in_view(void *sphere, f32 dist) __asm__("FUN_001fa728");
extern s32 tick_timer32(s32 *) __asm__("FUN_001f9740");
extern s32 random_remainder(s32) __asm__("FUN_00213260");
extern f32 random_float(f32, f32) __asm__("FUN_002132a8");
extern f32 scale_time(f32) __asm__("FUN_001f96b0");
extern s32 truncate_time(f32) __asm__("FUN_001fa6d0");
extern s32 lerp_color(s32, s32, f32) __asm__("FUN_001fa6e0");
/* fast_vec_normalize (out = in scaled to length s), with the float declared first (same registers): the scale is then
 * evaluated before the vector addresses, as in retail. */
extern void scale_vector_by(f32 s, void *out, void *in) __asm__("FUN_001f9bf8");
extern void add_vector(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_vector(void *) __asm__("FUN_001f99f8");
extern void random_vector_jitter(void *out, f32 a, f32 b) __asm__("FUN_L00_00257d78");
extern char *spawn_smoke_particle(void *pos, void *vel, void *acc, s32, s32, s32, s32, s32, s32) __asm__("FUN_00218888");
#include "rnc/overlay/l02/gameplay/entities/constructorbot_data/update_smoke_vent_8.h"
extern s32 D_L02_0016205C __attribute__((sda));
extern s32 D_L02_00162060 __attribute__((sda));
#include "rnc/overlay/l02/gameplay/entities/constructorbot_data/update_smoke_vent_7.h"
extern f32 D_L02_00162068 __attribute__((sda));
extern f32 D_L02_0016206C __attribute__((sda));
extern f32 D_L02_00162070 __attribute__((sda));
extern f32 D_L02_00162074 __attribute__((sda));
s32 D_L02_00162078 = 0x404080ff;
s32 D_L02_0016207C = 0x404080c0;
s32 D_L02_00162080 = 0x20804050;
s32 D_L02_00162084 = 0x20603038;
#include "rnc/overlay/l02/gameplay/entities/constructorbot_data/update_smoke_vent_6.h"
extern s32 D_L02_0016209C __attribute__((sda));
extern s32 D_L02_001620A0 __attribute__((sda));
#include "rnc/overlay/l02/gameplay/entities/constructorbot_data/update_smoke_vent_5.h"
extern f32 D_L02_001620A8 __attribute__((sda));
extern f32 D_L02_001620AC __attribute__((sda));
extern f32 D_L02_001620B0 __attribute__((sda));
extern f32 D_L02_001620B4 __attribute__((sda));
s32 D_L02_001620B8 = 0x6000ff80;
s32 D_L02_001620BC = 0x6020ff40;
s32 D_L02_001620C0 = 0x20408040;
s32 D_L02_001620C4 = 0x20206020;
#include "rnc/overlay/l02/gameplay/entities/constructorbot_data/update_smoke_vent_4.h"
extern s32 D_L02_001620E0 __attribute__((sda));
extern s32 D_L02_001620E4 __attribute__((sda));
#include "rnc/overlay/l02/gameplay/entities/constructorbot_data/update_smoke_vent_3.h"
extern f32 D_L02_001620EC __attribute__((sda));
extern f32 D_L02_001620F0 __attribute__((sda));
extern f32 D_L02_001620F4 __attribute__((sda));
extern f32 D_L02_001620F8 __attribute__((sda));
s32 D_L02_001620FC = 0x804080ff;
s32 D_L02_00162100 = 0x8040ffff;
s32 D_L02_00162104 = 0x80204040;
s32 D_L02_00162108 = 0x80204080;
extern s32 view_gated_smoke_burst_interval_min_gp __asm__("D_L02_0016210C") __attribute__((sda));
extern s32 view_gated_smoke_burst_interval_max_gp __asm__("D_L02_00162110") __attribute__((sda));
s32 D_L02_00162114 = 0x10019;
#include "rnc/overlay/l02/gameplay/entities/constructorbot_data/update_smoke_vent_2.h"
extern s32 D_L02_00162130 __attribute__((sda));
extern s32 D_L02_00162134 __attribute__((sda));
#include "rnc/overlay/l02/gameplay/entities/constructorbot_data/update_smoke_vent.h"
extern f32 D_L02_0016213C __attribute__((sda));
extern f32 D_L02_00162140 __attribute__((sda));
extern f32 D_L02_00162144 __attribute__((sda));
extern f32 D_L02_00162148 __attribute__((sda));
s32 D_L02_0016214C = 0x804080ff;
s32 D_L02_00162150 = 0x8040ffff;
s32 D_L02_00162154 = 0x80204040;
s32 D_L02_00162158 = 0x80204080;
extern s32 alternating_smoke_burst_interval_min_gp __asm__("D_L02_0016215C") __attribute__((sda));
extern s32 alternating_smoke_burst_interval_max_gp __asm__("D_L02_00162160") __attribute__((sda));
s32 D_L02_00162164 = 0x10019;

/* Smoke vent: once visible within 120 units it puffs bursts (D_L02_001620C8 / D_L02_00162118 parameter blocks)
 * on a timer plus occasional single puffs (D_L02_00162088 / D_L02_00162048); state 2 alternates its puff life.
 * Each burst and puff block has its own locals: shared ones become long-lived pseudos and shift the allocation. */
void update_smoke_vent(struct Moby *moby) __asm__("FUN_L02_002ef020");

void update_smoke_vent(struct Moby *moby) {
    enum {
        SMOKE_VENT_STATE_INITIALIZE = 0,
        SMOKE_VENT_STATE_VIEW_GATED_EMISSION = 1,
        SMOKE_VENT_STATE_ALTERNATING_PUFF_TIMING = 2,
        SMOKE_VENT_STATE_REMOVE = 3
    };
    char *data = (char *)moby->pvars;
    Vec4 view_sphere;
    Vec4 smoke_particle_velocity;
    Vec4 acc;
    Vec4 vector_perturbation;
    s32 count1;
    s32 k1;
    s32 time_a1_burst;
    s32 time_b1_burst;
    s32 color_a1_burst;
    s32 color_b1_burst;
    s32 count2;
    s32 k2;
    s32 time_a2_burst;
    s32 time_b2_burst;
    s32 color_a2_burst;
    s32 color_b2_burst;
    s32 time_a1_puff;
    s32 time_b1_puff;
    s32 color_a1_puff;
    s32 color_b1_puff;
    f32 rise1_puff;
    s32 time_a2_puff;
    s32 time_b2_puff;
    s32 color_a2_puff;
    s32 color_b2_puff;
    f32 rise2_puff;

    switch (moby->state) {
    case SMOKE_VENT_STATE_INITIALIZE:
        moby->state = data[0] + 1;
        build_rotation_matrix(&moby->rot_row0, &moby->rot);
        break;
    case SMOKE_VENT_STATE_VIEW_GATED_EMISSION:
        qcopy(&view_sphere, &moby->pos);
        view_sphere.f[3] = 6.0f;
        if (sphere_in_view(&view_sphere, 120.0f) == -1) {
            break;
        }
        if (tick_timer32((s32 *)(data + 0xC0)) != 0) {
            *(s32 *)(data + 0xC0) = truncate_time(scale_time(random_float((f32)view_gated_smoke_burst_interval_min_gp, (f32)view_gated_smoke_burst_interval_max_gp)));
            count1 = random_remainder(7) != 0 ? 1 : random_remainder(5) + 5;
            for (k1 = 0; k1 < count1; k1++) {
                random_vector_jitter(&vector_perturbation, 0.0f, view_gated_smoke_burst_jitter_magnitude_max_gp * frame_time);
                scale_vector_by(view_gated_smoke_burst_velocity_magnitude_scale_gp * (random_float(-view_gated_smoke_burst_motion_variation_fraction_gp, view_gated_smoke_burst_motion_variation_fraction_gp) + 1.0f) * frame_time, &smoke_particle_velocity, &moby->rot_row0);
                add_vector(&smoke_particle_velocity, &smoke_particle_velocity, &vector_perturbation);
                scale_vector_by(view_gated_smoke_burst_acceleration_magnitude_scale_gp * (random_float(-view_gated_smoke_burst_motion_variation_fraction_gp, view_gated_smoke_burst_motion_variation_fraction_gp) + 1.0f) * frame_time, &acc, &smoke_particle_velocity);
                acc.f[2] -= view_gated_smoke_burst_vertical_acceleration_decrement_scale_gp * frame_time;
                smoke_particle_velocity.f[3] = random_float(D_L02_001620EC, D_L02_001620F0);
                acc.f[3] = random_float(D_L02_001620F4, D_L02_001620F8);
                color_a1_burst = lerp_color(D_L02_001620FC, D_L02_00162100, random_float(0.0f, 1.0f));
                color_b1_burst = lerp_color(D_L02_00162104, D_L02_00162108, random_float(0.0f, 1.0f));
                time_a1_burst = truncate_time(scale_time((f32)view_gated_smoke_burst_timing_random_span_gp * random_float(0.0f, 1.0f) + 1.0f));
                time_b1_burst = truncate_time(scale_time((f32)D_L02_001620E0 * (random_float(-view_gated_smoke_burst_timing_variation_fraction_gp, view_gated_smoke_burst_timing_variation_fraction_gp) + 1.0f)));
                spawn_smoke_particle(&moby->pos, &smoke_particle_velocity, &acc, color_a1_burst, color_b1_burst, time_a1_burst, time_b1_burst,
                               truncate_time(scale_time((f32)D_L02_001620E4 * (random_float(-view_gated_smoke_burst_timing_variation_fraction_gp, view_gated_smoke_burst_timing_variation_fraction_gp) + 1.0f))),
                               D_L02_00162114);
            }
        }
        if (random_remainder(4) != 0) {
            break;
        }
        scale_vector_by(view_gated_smoke_puff_velocity_magnitude_scale_gp * (random_float(-view_gated_smoke_puff_motion_variation_fraction_gp, view_gated_smoke_puff_motion_variation_fraction_gp) + 1.0f) * frame_time, &smoke_particle_velocity, &moby->rot_row0);
        rise1_puff = view_gated_smoke_puff_vertical_acceleration_scale_gp * (random_float(-view_gated_smoke_puff_motion_variation_fraction_gp, view_gated_smoke_puff_motion_variation_fraction_gp) + 1.0f) * frame_time;
        clear_vector(&acc);
        acc.f[2] = rise1_puff;
        scale_vector_by(random_float(-view_gated_smoke_puff_lateral_acceleration_magnitude_bound_gp, view_gated_smoke_puff_lateral_acceleration_magnitude_bound_gp) * frame_time, &vector_perturbation, &moby->rot_row1);
        add_vector(&acc, &acc, &vector_perturbation);
        smoke_particle_velocity.f[3] = random_float(D_L02_001620A8, D_L02_001620AC);
        acc.f[3] = random_float(D_L02_001620B0, D_L02_001620B4);
        color_a1_puff = lerp_color(D_L02_001620B8, D_L02_001620BC, random_float(0.0f, 1.0f));
        color_b1_puff = lerp_color(D_L02_001620C0, D_L02_001620C4, random_float(0.0f, 1.0f));
        time_a1_puff = truncate_time(scale_time((f32)view_gated_smoke_puff_timing_random_span_gp * random_float(0.0f, 1.0f) + 1.0f));
        time_b1_puff = truncate_time(scale_time((f32)D_L02_0016209C * (random_float(-view_gated_smoke_puff_timing_variation_fraction_gp, view_gated_smoke_puff_timing_variation_fraction_gp) + 1.0f)));
        spawn_smoke_particle(&moby->pos, &smoke_particle_velocity, &acc, color_a1_puff, color_b1_puff, time_a1_puff, time_b1_puff,
                       truncate_time(scale_time((f32)D_L02_001620A0 * (random_float(-view_gated_smoke_puff_timing_variation_fraction_gp, view_gated_smoke_puff_timing_variation_fraction_gp) + 1.0f))), -1);
        break;
    case SMOKE_VENT_STATE_ALTERNATING_PUFF_TIMING:
        if (tick_timer32((s32 *)(data + 0xC4)) != 0) {
            if (*(s32 *)(data + 0xC8) < 21) {
                *(s32 *)(data + 0xC8) = 30;
            } else {
                *(s32 *)(data + 0xC8) = 10;
            }
            *(s32 *)(data + 0xC4) = truncate_time(scale_time(random_float(120.0f, 180.0f)));
        }
        qcopy(&view_sphere, &moby->pos);
        view_sphere.f[3] = 6.0f;
        if (sphere_in_view(&view_sphere, 120.0f) == -1 && random_remainder(4) != 0) {
            break;
        }
        if (tick_timer32((s32 *)(data + 0xC0)) != 0) {
            *(s32 *)(data + 0xC0) = truncate_time(scale_time(random_float((f32)alternating_smoke_burst_interval_min_gp, (f32)alternating_smoke_burst_interval_max_gp)));
            count2 = random_remainder(7) != 0 ? 1 : random_remainder(5) + 5;
            for (k2 = 0; k2 < count2; k2++) {
                random_vector_jitter(&vector_perturbation, 0.0f, alternating_smoke_burst_jitter_magnitude_max_gp * frame_time);
                scale_vector_by(alternating_smoke_burst_velocity_magnitude_scale_gp * (random_float(-alternating_smoke_burst_motion_variation_fraction_gp, alternating_smoke_burst_motion_variation_fraction_gp) + 1.0f) * frame_time, &smoke_particle_velocity, &moby->rot_row0);
                add_vector(&smoke_particle_velocity, &smoke_particle_velocity, &vector_perturbation);
                scale_vector_by(alternating_smoke_burst_acceleration_magnitude_scale_gp * (random_float(-alternating_smoke_burst_motion_variation_fraction_gp, alternating_smoke_burst_motion_variation_fraction_gp) + 1.0f) * frame_time, &acc, &smoke_particle_velocity);
                acc.f[2] -= alternating_smoke_burst_vertical_acceleration_decrement_scale_gp * frame_time;
                smoke_particle_velocity.f[3] = random_float(D_L02_0016213C, D_L02_00162140);
                acc.f[3] = random_float(D_L02_00162144, D_L02_00162148);
                color_a2_burst = lerp_color(D_L02_0016214C, D_L02_00162150, random_float(0.0f, 1.0f));
                color_b2_burst = lerp_color(D_L02_00162154, D_L02_00162158, random_float(0.0f, 1.0f));
                time_a2_burst = truncate_time(scale_time((f32)alternating_smoke_burst_timing_random_span_gp * random_float(0.0f, 1.0f) + 1.0f));
                time_b2_burst = truncate_time(scale_time((f32)D_L02_00162130 * (random_float(-alternating_smoke_burst_timing_variation_fraction_gp, alternating_smoke_burst_timing_variation_fraction_gp) + 1.0f)));
                spawn_smoke_particle(&moby->pos, &smoke_particle_velocity, &acc, color_a2_burst, color_b2_burst, time_a2_burst, time_b2_burst,
                               truncate_time(scale_time((f32)D_L02_00162134 * (random_float(-alternating_smoke_burst_timing_variation_fraction_gp, alternating_smoke_burst_timing_variation_fraction_gp) + 1.0f))),
                               D_L02_00162164);
            }
        }
        scale_vector_by(alternating_smoke_puff_velocity_magnitude_scale_gp * (random_float(-alternating_smoke_puff_motion_variation_fraction_gp, alternating_smoke_puff_motion_variation_fraction_gp) + 1.0f) * frame_time, &smoke_particle_velocity, &moby->rot_row0);
        rise2_puff = alternating_smoke_puff_vertical_acceleration_scale_gp * (random_float(-alternating_smoke_puff_motion_variation_fraction_gp, alternating_smoke_puff_motion_variation_fraction_gp) + 1.0f) * frame_time;
        clear_vector(&acc);
        acc.f[2] = rise2_puff;
        scale_vector_by(random_float(-alternating_smoke_puff_lateral_acceleration_magnitude_bound_gp, alternating_smoke_puff_lateral_acceleration_magnitude_bound_gp) * frame_time, &vector_perturbation, &moby->rot_row1);
        add_vector(&acc, &acc, &vector_perturbation);
        smoke_particle_velocity.f[3] = random_float(D_L02_00162068, D_L02_0016206C);
        acc.f[3] = random_float(D_L02_00162070, D_L02_00162074);
        color_a2_puff = lerp_color(D_L02_00162078, D_L02_0016207C, random_float(0.0f, 1.0f));
        color_b2_puff = lerp_color(D_L02_00162080, D_L02_00162084, random_float(0.0f, 1.0f));
        time_a2_puff = truncate_time(scale_time((f32)*(s32 *)(data + 0xC8) * random_float(0.0f, 1.0f) + 1.0f));
        time_b2_puff = truncate_time(scale_time((f32)D_L02_0016205C * (random_float(-alternating_smoke_puff_timing_variation_fraction_gp, alternating_smoke_puff_timing_variation_fraction_gp) + 1.0f)));
        spawn_smoke_particle(&moby->pos, &smoke_particle_velocity, &acc, color_a2_puff, color_b2_puff, time_a2_puff, time_b2_puff,
                       truncate_time(scale_time((f32)D_L02_00162060 * (random_float(-alternating_smoke_puff_timing_variation_fraction_gp, alternating_smoke_puff_timing_variation_fraction_gp) + 1.0f))), -1);
        break;
    case SMOKE_VENT_STATE_REMOVE:
        mark_moby_for_removal(moby);
        break;
    }
}
extern void compute_spherical_offset_from_positions(void *, void *, void *) __asm__("FUN_L00_001ed630");

/* Sets the vendor camera mode; mode 3 also aims it from the hero's position. */
void set_vendor_camera_mode(int mode, int camera_argument_value) __asm__("FUN_L02_002f8a18");

void set_vendor_camera_mode(int mode, int camera_argument_value) {
    enum {
        VENDOR_CAMERA_MODE_SET_ARGUMENT_PAIR = 2,
        VENDOR_CAMERA_MODE_CAPTURE_HERO_POSITION = 3
    };
    char *vendor_camera_context = D_L02_00167280;
    char *vendor_camera = *(char **)(vendor_camera_context + 0x180);
    char *camera_data = *(char **)(vendor_camera + 0x70);
    char *camera_argument_pair = camera_data + 0xE0;

    vendor_camera[0x88] = mode;
    switch (mode) {
    case VENDOR_CAMERA_MODE_CAPTURE_HERO_POSITION:
        qcopy(camera_data + 0x110, &hero.motion.pos);
        compute_spherical_offset_from_positions(camera_data + 0xFC, vendor_camera_context + 0x140, &hero.motion.pos);
    case VENDOR_CAMERA_MODE_SET_ARGUMENT_PAIR:
        *(int *)(camera_argument_pair + 4) = camera_argument_value;
        *(int *)(camera_data + 0xE0) = camera_argument_value;
        break;
    }
}
