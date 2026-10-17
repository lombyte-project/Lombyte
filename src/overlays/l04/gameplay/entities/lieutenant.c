/* Level 04 moby classes: lieutenant. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math/vector.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/gameplay/state/usage_stats.h"

/* Vendor update: opens when the hero is close, sets up the shop camera in front of it once
 * its phase reaches 2, and closes when the slot is already used. */
struct VendorMobyView {
    u8 pad0[0x7F];
    u8 unk7F;
    u8 pad80[0x30];
    u8 slot;          /* 0xB0 */
    u8 padB1[0xF];
    Vec4 facing;      /* 0xC0 */
};

struct VendorVars {
    u8 pad0[4];
    s16 phase;        /* 0x04 */
    u8 pad6[6];
    f32 alpha;        /* 0x0C */
    u8 pad10[0x130];
    s32 item;         /* 0x140 */
};

struct ShopCamera {
    Vec4 pos;
    Vec4 rot;
    u8 pad20[0x16];
    s16 active;       /* 0x36 */
};

extern char D_L04_00166F40[];
extern u8 D_0014C050[];
extern struct ShopCamera D_L04_0016C9F0;
extern s32 D_L04_0015F5C4;
extern float FUN_001f9b48(void *, void *);
extern void FUN_L00_0025a120(struct Moby *);
extern void FUN_L02_002e0cd8(struct Moby *);
extern void initialize_npc_dialogue(struct Moby *, struct VendorVars *) __asm__("FUN_L00_002668a0");
extern int FUN_00214720(char *, s32);
extern int update_npc_dialogue(struct Moby *, struct VendorVars *) __asm__("FUN_L00_00266448");
extern void unlock_planet(int) __asm__("FUN_L00_002607d0");
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void fast_vec_normalize(Vec4 *, Vec4 *, float) __asm__("FUN_001f9bf8");
extern void add_vector_xyz(Vec4 *, Vec4 *, void *) __asm__("FUN_001f9a10");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");

void lieutenant_update(struct Moby *moby) __asm__("FUN_L04_002e3078");
void lieutenant_update(struct Moby *moby) {
    Vec4 pos;
    Vec4 rot;
    Vec4 *r;
    struct VendorVars *vars = (struct VendorVars *)moby->pvars;

    if (moby->visible != 0 && FUN_001f9b48(&moby->pos, D_L04_00166F40) < 30.0f) {
        FUN_L00_0025a120(moby);
        ((struct VendorMobyView *)moby)->unk7F = 0x18;
    }
    FUN_L02_002e0cd8(moby);
    switch (moby->state) {
    case 0:
        if (D_0014C050[((struct VendorMobyView *)moby)->slot + current_level_index * 16] == 0xFF) {
            moby->state = 3;
            moby->flags |= 0x41;
            moby->collision = 0;
            break;
        }
        moby->state = 1;
        initialize_npc_dialogue(moby, vars);
        break;
    case 1:
        if (FUN_00214720(&hero.motion.pos, vars->item))
            vars->alpha = 255.0f;
        else
            vars->alpha = 1.0f;
        if (update_npc_dialogue(moby, vars))
            moby->state = 2;
        break;
    case 2:
        if (vars->phase == 2) {
            unlock_planet(6);
            jet_clear_persistent(((struct VendorMobyView *)moby)->slot);
            fast_vec_normalize(&pos, &((struct VendorMobyView *)moby)->facing, 2.0f);
            add_vector_xyz(&pos, &pos, &moby->pos);
            r = &rot;
            qcopy(r, &moby->rot);
            rot.f[2] = fast_add_rotations(rot.f[2], 3.14159f);
            qcopy(&D_L04_0016C9F0.pos, &pos);
            qcopy(&D_L04_0016C9F0.rot, r);
            D_L04_0016C9F0.active = 1;
            memcard_save_data(0, -1);
            moby->state = 3;
            moby->flags |= 0x41;
            moby->collision = 0;
            break;
        }
        if (D_L04_0015F5C4 != 2)
            update_npc_dialogue(moby, vars);
        break;
    case 3:
        moby->flags |= 0x41;
        moby->collision = 0;
        break;
    }
}
#include "sda.h"

/* UpdateMoby_1343: tutorial hint and statistic triggers around the player on Eudora. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002E57F8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
    int f30;
    int f34;
    int f38;
    int f3C;
    int f40;
    int f44;
    int f48;
    int f4C;
    int f50;
} Data_2E57F8;

typedef struct {
    unsigned short count;
    unsigned short time;
    unsigned int mask;
} Help_2E57F8;

extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern int D_L04_00179590[] __attribute__((section(".data")));
extern int help_request(int a, int b) __asm__("FUN_L00_00203908");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern unsigned char D_0013D4D4 __attribute__((section(".data")));
extern unsigned char D_0013D4E3[];
extern unsigned char D_0014C050_c[] __asm__("D_0014C050");
extern unsigned short D_001417C2 __attribute__((section(".data")));
extern unsigned short D_00141892 __attribute__((section(".data")));
extern unsigned short D_001418D0 __attribute__((section(".data")));
extern unsigned short D_00141908 __attribute__((section(".data")));
extern unsigned short D_00141910 __attribute__((section(".data")));
extern unsigned short D_00141940 __attribute__((section(".data")));
extern char D_00141968[];
extern u8 items_acquired[] __asm__("D_0013D4E8");
extern Moby *D_L04_00160064_2E57F8 __asm__("D_L04_0015FFE4");
extern Moby *D_L04_00160058_2E57F8 __asm__("D_L04_0015FFD8");

extern s32 current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern int D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
extern s32 D_0015EEAC_macro __asm__("D_0015EEAC") MACRO_ADDR;
void FUN_L04_002e4418(Moby *moby) {
    Data_2E57F8 *data = *(void **)&moby->pvars;

    switch (moby->state) {
    case 0:
        moby->update_dist = 0xFF;
        moby->state = 1;
        data->f14 = -1;
        break;
    case 1:
        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f0) != 0
            && D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1
            && D_0014C050_c[moby->mission + (current_level_index_macro << 4)] != 0xFF) {
            if (data->f14 == -1) {
                struct UsageStat *usage = usage_stats.stat;
                data->f14 = D_0015EEAC_macro;
                if (usage[0x18].count < 0xFFFF) {
                    usage[0x18].count++;
                }
                if (scale_game_frames_alt(D_0015EEA4_macro) / 600 > usage[0x18].unk2) {
                    usage[0x18].unk2 = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                }
                usage[0x18].level_mask = usage[0x18].level_mask | (1 << current_level_index_macro) | 0x80000000;
            } else if (D_00141908 >= 3) {
                if (D_0013D4D4 == 0) {
                    Help_2E57F8 *help = (Help_2E57F8 *)(((char *)&D_00141968));
                    if (help[0x1D].count == 0) {
                        help_request(0xFA1, 0x1D);
                    }
                } else {
                    int elapsed = scale_game_frames_alt(D_0015EEA4_macro) - D_001417C2 * 600;
                    if (scale_game_frames_alt(0x4650) < elapsed) {
                        Help_2E57F8 *help = (Help_2E57F8 *)(((char *)&D_00141968));
                        if (help[0x1C].count == 0) {
                            help_request(0xFA0, 0x1C);
                        }
                    }
                }
            }
        }

        {
            Help_2E57F8 *help = (Help_2E57F8 *)(((char *)&D_00141968));
            if (help[0x42].count < 4) {
                if (is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f34) != 0
                    || is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f38) != 0) {
                    Hero *hero = (Hero *)(((char *)&D_0013F3D0 - 0x80));
                    if (hero->state.prev == 0x3B && hero->state_timer == 0) {
                        Moby *other;
                        for (other = D_L04_00160064_2E57F8; other != 0; other = other->next) {
                            if (other->oclass == 0x118
                                && distance_xyz(((float *)&other->pos), ((char *)&D_0013F3D0)) < 3.0f
                                && *(float *)*(void **)&other->pvars != 1.0f) {
                                data->f20++;
                                if (data->f20 == 3 || data->f20 == 6) {
                                    help_request(0x3EE, 0x42);
                                }
                            }
                        }
                    }
                }
            }
        }

        {
            unsigned char *item_flags = ((char *)&items_acquired);
            if (item_flags[9] != 0 && D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
                Help_2E57F8 *help = (Help_2E57F8 *)(((char *)&D_00141968));
                if (help[0x1E].count == 0) {
                    help_request(0xFA2, 0x1E);
                }
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f3C) != 0 && D_00141940 == 0
            && D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
            Help_2E57F8 *help = (Help_2E57F8 *)(((char *)&D_00141968));
            if (help[0x77].count == 0) {
                help_request(0x4E2D, 0x77);
            }
        }

        if (data->f48 != -1) {
            if (D_L04_00160058_2E57F8[data->f48].oclass == 0x267 && D_L04_00160058_2E57F8[data->f48].state == 4) {
                menu_item_enabled_16 = 1;
            }
        }
        if (data->f4C != -1) {
            if (D_L04_00160058_2E57F8[data->f4C].oclass == 0x267 && D_L04_00160058_2E57F8[data->f4C].state == 4) {
                menu_item_enabled_17 = 1;
            }
        }
        if (data->f40 != -1) {
            if (D_L04_00160058_2E57F8[data->f40].oclass == 0x118 && D_L04_00160058_2E57F8[data->f40].state == 5) {
                menu_item_enabled_18 = 1;
            }
        }
        if (data->f44 != -1) {
            if (D_L04_00160058_2E57F8[data->f44].oclass == 0x118 && D_L04_00160058_2E57F8[data->f44].state == 5) {
                menu_item_enabled_19 = 1;
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f4) != 0
            && (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 || ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) == 9)
            && D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
            if (D_0013D4DA == 0) {
                Help_2E57F8 *help = (Help_2E57F8 *)(((char *)&D_00141968));
                int elapsed = scale_game_frames_alt(D_0015EEA4_macro) - help[0x1F].time * 600;
                if ((int)(scale_game_frames_alt(0x12) * 60.0f) < elapsed || help[0x1F].time * 600 == 0) {
                    help_request(0xFA3, 0x1F);
                } else {
                    int now = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                    if (help[0x1F].time < now) {
                        help[0x1F].time = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                    }
                }
            } else {
                if (data->f48 != -1 && D_L04_00160058_2E57F8[data->f48].oclass == 0x267 && D_L04_00160058_2E57F8[data->f48].state == 4) {
                    struct UsageStat *usage = usage_stats.stat;
                    unsigned int count = usage[0x11].count;
                    if (count <= 0xFFFE) {
                        usage[0x11].count = count + 1;
                    }
                    if (scale_game_frames_alt(D_0015EEA4_macro) / 600 > usage[0x11].unk2) {
                        usage[0x11].unk2 = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                    }
                    usage[0x11].level_mask = usage[0x11].level_mask | (1 << current_level_index_macro) | 0x80000000;
                } else if (data->fC == 0) {
                    if (D_001418D0 == 0) {
                        Help_2E57F8 *help;
                        data->f8++;
                        help = (Help_2E57F8 *)(((char *)&D_00141968));
                        if (help[0x20].count != 0) {
                            int elapsed = scale_game_frames_alt(D_0015EEA4_macro) - help[0x20].time * 600;
                            if ((int)(scale_game_frames_alt(0x12) * 60.0f) < elapsed || help[0x20].time * 600 == 0) {
                                help_request(0xFA4, 0x20);
                            } else {
                                int now = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                                if (help[0x20].time < now) {
                                    help[0x20].time = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                                }
                            }
                        } else {
                            int now;
                            help[0x20].count++;
                            now = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                            if (help[0x20].time < now) {
                                help[0x20].time = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                            }
                            help[0x20].mask = help[0x20].mask | (1 << current_level_index_macro) | 0x80000000;
                        }
                    }
                }
            }
        } else if (data->fC != 0) {
            if (is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f4) == 0) {
                data->fC = 0;
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f50) != 0
            && (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 || ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) == 9)
            && D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
            int elapsed = scale_game_frames_alt(D_0015EEA4_macro) - D_00141892 * 600;
            if (scale_game_frames_alt(0x34BC0) < elapsed) {
                Help_2E57F8 *help = (Help_2E57F8 *)(((char *)&D_00141968));
                if (!(help[0x43].mask & (1 << current_level_index_macro))) {
                    help_request(0x3EF, 0x43);
                }
            }
        }

        {
            Help_2E57F8 *help = (Help_2E57F8 *)(((char *)&D_00141968));
            if (help[0x46].count == 0 || help[0x47].count == 0) {
                Hero *hero = (Hero *)(((char *)&D_0013F3D0 - 0x80));
                Moby *ground;
                if (hero->state.current == 0x72 && (ground = hero->ground_moby) != 0 && ground->oclass == 0x267
                    && is_point_inside_clip_volume((char *)hero + 0x80, data->f4) != 0) {
                    if (help[0x46].count == 0) {
                        help_request(0x7DB, 0x46);
                    } else if (help[0x47].count == 0) {
                        data->f1C++;
                        if (scale_game_frames_alt(0x708) < data->f1C) {
                            help_request(0x7DC, 0x47);
                        }
                    }
                } else {
                    data->f1C = 0;
                }
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f30) != 0) {
            struct UsageStat *usage = usage_stats.stat;
            if (usage[0x19].count == 0
                && (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 || ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) == 9)) {
                int now;
                usage[0x19].count++;
                now = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                if (usage[0x19].unk2 < now) {
                    usage[0x19].unk2 = scale_game_frames_alt(D_0015EEA4_macro) / 600;
                }
                usage[0x19].level_mask = usage[0x19].level_mask | (1 << current_level_index_macro) | 0x80000000;
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f2C) != 0 && D_00141910 == 0
            && ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) >= 2 && ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) != 9
            && data->f28 == 0) {
            data->f24++;
            data->f28 = scale_game_frames_alt(0xB4);
        } else if (data->f28 != 0) {
            if (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 || ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) == 9) {
                data->f28 = 0;
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), data->f2C) != 0 && D_00141910 == 0
            && (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 || ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) == 9)
            && data->f24 >= 3
            && D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
            Help_2E57F8 *help = (Help_2E57F8 *)(((char *)&D_00141968));
            if (help[0x49].count == 0) {
                help_request(0xFA5, 0x49);
            }
        }
        break;
    }
}
#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    u8 pad0[4];
    f32 f4;
    f32 f8;
    s16 hC;
    s16 hE;
    f32 vel[4] __attribute__((aligned(16)));
    f32 f20, f24, f28;
} PV2e5178;
typedef struct {
    u8 pad0[0x10];
    f32 pos[4] __attribute__((aligned(16)));
    u8 b20, b21, b22, b23;
    u8 pad24[8];
    f32 f2c;
    u8 pad30[0x10];
    f32 f40, f44, f48;
    u8 pad4c[0x2C];
    PV2e5178 *pv;
} M2e5178;
extern f32 g48_2e5178 __asm__("D_L04_00161D78") __attribute__((sda));
extern f32 g4c_2e5178 __asm__("D_L04_00161D7C") __attribute__((sda));
extern u128 tgt_2e5178[] __asm__("D_L04_00174000") __attribute__((section(".data")));
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 appr_2e5178(f32, f32) __asm__("FUN_001fa580");
extern s32 tick_2e5178(void *) __asm__("FUN_001f9770");
extern s32 coll_2e5178(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
extern f32 fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern void refl_2e5178(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern f32 rnd_2e5178(f32, f32) __asm__("FUN_L00_00257c48");
extern void kill_2e5178(void *) __asm__("FUN_0020c828");
void FUN_L04_002e5178(M2e5178 *m) {
    PV2e5178 *pv = m->pv;
    u128 old;
    u128 *t;
    qcopy(&old, m->pos);
    pv->vel[2] -= frame_time_sq * 20.0f;
    fast_vec_add(m->pos, m->pos, pv->vel);
    m->f40 = appr_2e5178(m->f40, pv->f20);
    m->f44 = appr_2e5178(m->f44, pv->f24);
    m->f48 = appr_2e5178(m->f48, pv->f28);
    switch (m->b20) {
    case 0:
        if (m->pos[2] < pv->f4 - 3.0f) {
            m->b20 = 1;
        } else if (tick_2e5178(&pv->hE) && pv->hC != 0 && coll_2e5178(m->pos, 2, 0, pv->f8)) {
            t = tgt_2e5178;
            if (fast_vec_dot(pv->vel, t) < 0.0f) {
                qcopy(m->pos, t - 1);
                pv->hC--;
                refl_2e5178(pv->vel, pv->vel, t);
                fast_vec_scale(pv->vel, pv->vel, 0.75f);
                pv->f20 = 0.0f;
                pv->f24 = rnd_2e5178(g48_2e5178, g4c_2e5178) * DEG_TO_RAD * frame_time;
                pv->f28 = rnd_2e5178(g48_2e5178, g4c_2e5178) * DEG_TO_RAD * frame_time;
            }
        }
        break;
    case 1:
        m->f2c *= 0.9f;
        if (m->b23 < 4) {
            kill_2e5178(m);
        } else {
            m->b23 -= 4;
        }
        break;
    }
}
/* Waits for state 1, then passes the slot for the level's current mode to cutscene_set_camera_target. */
struct L04MissionState {
    char pad0[0x30];
    int mode;
    char pad1[0x178 - 0x34];
    int slot[4];
};

extern int D_L04_0015F5C4;
extern struct L04MissionState D_L04_0016C9E0;
extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void FUN_L04_002e53a0(struct Moby *m) {
    switch (m->state) {
    case 0:
        m->update_dist = 0xFF;
        m->state = 1;
        break;
    case 1:
        if (D_L04_0015F5C4 == 2) {
            if (D_L04_0016C9E0.mode == 0 || D_L04_0016C9E0.mode == 1) {
                int v = D_L04_0016C9E0.mode;
                int i;
                i = 0;
                if (v == 0)
                    i = 3;
                else if (v == 1)
                    i = 2;
                cutscene_set_camera_target(D_L04_0016C9E0.slot[i]);
            }
        }
        break;
    }
}
