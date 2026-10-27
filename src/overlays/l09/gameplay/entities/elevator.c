/* Level 09 moby classes: elevator, refueling_station, airship_part, turret, barrier, shuttle. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_globals.h"
#define F(p, o) (*(float *)((p) + (o)))

/* Moves a moby along a path between its ends once its collectable flag is set, easing between nodes. */
extern char *D_L09_001B0630[];
typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L09LevelState;
extern L09LevelState D_L09_001BB3B0;
extern int D_L09_001BA650[];
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9b48(void *, void *);
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");
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
void elevator_update(unsigned char *moby) __asm__("FUN_L09_003033a0");
void elevator_update(unsigned char *moby) {
    enum {
        ELEVATOR_STATE_AT_PATH_END = 0,
        ELEVATOR_STATE_AT_PATH_START = 1,
        ELEVATOR_STATE_MOVING = 2
    };
    char *data = *(char **)(moby + 0x78);
    char *path;
    float orig[4], delta[4], segment_offset[4];
    int on = 0;
    if (*(int *)(data + 0xB4) == -1) {
        return;
    }
    path = D_L09_001B0630[*(int *)(data + 0xB4)];
    if (*(int *)(data + 0xC0) != -1) {
        unsigned short id;
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xC0)) ||
            D_L09_001BB3B0.collected[(short)(id = *(unsigned short *)(moby + 0xB2))] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            if (*(int *)(data + 0xC0) != -1) {
                D_0014C190_257218[current_level_index][(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
                    1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
                D_L09_001BA650[(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
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
    case ELEVATOR_STATE_AT_PATH_END:
        if (on) {
            float far;
            if ((*(short *)(data + 0xB8) != 0 && is_hero_on_moby(moby)) ||
                (far = FUN_001f9b48(&hero.motion.pos, path + *(int *)path * 16),
                 FUN_001f9b48(&hero.motion.pos, path + 0x10) < far)) {
                if (*(int *)(data + 0xC8) == 0 && is_hero_on_moby(moby)) {
                    hero_set_state(0x72, 1);
                }
                moby[0xBC] = 2;
                F(data, 0xAC) = -1.0f / multiply_global_factor_ed64(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case ELEVATOR_STATE_AT_PATH_START:
        if (on) {
            float near;
            if ((*(short *)(data + 0xB8) != 0 && is_hero_on_moby(moby)) ||
                (near = FUN_001f9b48(&hero.motion.pos, path + 0x10),
                 FUN_001f9b48(&hero.motion.pos, path + *(int *)path * 16) < near)) {
                if (*(int *)(data + 0xC8) == 0 && is_hero_on_moby(moby)) {
                    hero_set_state(0x72, 1);
                }
                allocate_voice_for_target_entry(1, 0, moby);
                moby[0xBC] = 2;
                F(data, 0xAC) = 1.0f / multiply_global_factor_ed64(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case ELEVATOR_STATE_MOVING:
        if (*(short *)(data + 0xBA) != 0 ||
            (F(data, 0xBC) < 0.0f &&
             vector_distance((moby + 0x10), &hero.motion.pos) < 2.0f &&
             absolute_float(hero.motion.pos.f[2] - F(moby, 0x18)) < 4.0f &&
             F(moby, 0x18) - hero.motion.pos.f[2] > 1.0f)) {
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
            float segment_fraction, node_index_as_float;
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
                if (*(int *)(data + 0xC8) == 0 && is_hero_on_moby(moby)) {
                    hero_set_state(HERO_STATE_IDLE, 1);
                }
                if (0.0f < F(data, 0xAC)) {
                    moby[0xBC] = 0;
                    F(data, 0xA4) = 1.0f;
                } else {
                    moby[0xBC] = 1;
                    F(data, 0xA4) = 0.0f;
                }
                allocate_voice_for_target_entry(2, 0, moby);
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
            segment_fraction = convert_integer_to_float(*(int *)path - 1);
            i = func_001FA898_r(segment_fraction * F(data, 0xA4));
            segment_fraction = convert_integer_to_float(*(int *)path - 1);
            node_index_as_float = convert_integer_to_float(i);
            segment_fraction *= F(data, 0xA4);
            segment_fraction -= node_index_as_float;
            if (i == *(int *)path - 1) {
                qcopy((moby + 0x10), path + 0x10 + i * 16);
            } else {
                segment_start = path + 0x10 + i * 16;
                subtract_vector_xyz(segment_offset, path + 0x20 + i * 16, segment_start);
                scale_vector_xyz(segment_offset, segment_offset, segment_fraction);
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
#include "rnc/gameplay/entities/moby.h"

/* Damage source returned by find_moby_hit. */
typedef struct {
    u8 pad0[0x2C];
    f32 damage;
} VendorHitSource;

typedef struct {
    u8 pad0[0x26];
    s16 hurt_timer;
    u8 pad28[0x38];
    u8 anim[8];
    u8 pad68[8];
    s32 link;
} VendorVars;

extern struct Moby *moby_list __asm__("D_L09_0015FFD8");
char D_L09_00208F90[] = "Moby class %d Damage Received: %f\n";
char D_L09_00208FF0[] = "Moby class %d Damage Received: %f\n";
extern VendorHitSource *find_moby_hit(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern s32 fptodp(f32) __asm__("FUN_00120478");
extern void debug_print(char *, s32, s32) __asm__("FUN_001e93b0");
extern s32 FUN_00213928(struct Moby *, VendorHitSource *, f32 *, s32, s32 *, s32, s32, s32);
extern void FUN_L00_00257470(struct Moby *, s32, s32);
extern void trooper_anim_update(struct Moby *, u8 *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(struct Moby *, u8 *) __asm__("FUN_L00_0025d538");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");

void handle_refueling_station_damage(struct Moby *moby, VendorVars *vars, f32 *health) __asm__("FUN_L09_00303b30");

void handle_refueling_station_damage(struct Moby *moby, VendorVars *vars, f32 *health) {
    enum {
        DAMAGE_RESULT_UNAVAILABLE = 0
    };
    VendorHitSource *damage_hit_record;
    struct Moby *link;
    s32 hit;

    if (moby->state != 3) {
        damage_hit_record = find_moby_hit(moby, 0x330000, 0);
        if (damage_hit_record != 0) {
            debug_print(D_L09_00208F90, moby->oclass, fptodp(damage_hit_record->damage));
        }
        switch (FUN_00213928(moby, damage_hit_record, health, 0, &hit, 0, 0, 4)) {
        case 1:
        case 2:
            *health = 0.0f;
            break;
        case DAMAGE_RESULT_UNAVAILABLE:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            break;
        }
        if (hit >= 2) {
            if (*health <= damage_hit_record->damage) {
                *health = 0.0f;
                moby->flags &= ~0x1000;
                if (vars->link != -1) {
                    link = &moby_list[vars->link];
                    if (link != 0 && link->state != 0xFE && link->state != 0xFD
                        && (link->oclass == 0x494 || link->oclass == 0x49D || link->oclass == 0x4A0)) {
                        link->command = 1;
                    }
                }
                FUN_L00_00257470(moby, 0, -1);
                vars->anim[7] = 0x78;
                trooper_anim_update(moby, vars->anim);
                moby->state = 3;
            } else {
                *health -= damage_hit_record->damage;
                vars->anim[7] = 0xFA;
                vars->hurt_timer = scale_game_frames(60);
                trooper_anim_update(moby, vars->anim);
            }
        }
        moby->hit_slot = 0xFF;
    }
    trooper_anim_finish(moby, vars->anim);
}
/* Pressure plate: waits for its trigger slot, hums while armed, wakes its linked moby when stepped on, then shatters. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_003050C0), where it is exact; names translated to the US level program. */

extern char D_0013D388[];
extern float D_L09_00166F40[];
extern int D_L09_0015F5CC;
extern int D_L09_0015FFD8_u __asm__("D_L09_0015FFD8");
extern int FUN_L00_0025e450_u() __asm__("FUN_L00_0025e450");
extern unsigned char voice_pool[] __asm__("D_0013E550");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
s32 allocate_voice_for_target_entry_c(s32 entry_index, s32 flags,
                                      void *target) __asm__("FUN_0022da68");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern int func_0022ED80_r(int, int, char *) __asm__("FUN_0022da68");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");

void refueling_station_update(struct Moby *m) __asm__("FUN_L09_00303d10");
void refueling_station_update(struct Moby *moby) {
    enum {
        REFUELING_STATION_STATE_INITIALIZE = 0,
        REFUELING_STATION_STATE_WAIT_COMMAND = 1,
        REFUELING_STATION_STATE_EXPLOSION_DELAY = 2,
        REFUELING_STATION_STATE_EXPLODE = 3
    };
    char *station_vars = (char *)moby->pvars;
    float explosion_offset[4];
    unsigned char *removal_check_base;
    handle_refueling_station_damage(moby, station_vars, station_vars + 0x20);
    switch (moby->state) {
    case REFUELING_STATION_STATE_INITIALIZE:
        if (*(short *)(station_vars + 0x76) != -1 &&
            (removal_check_base = (unsigned char *)D_0013D388 + *(short *)(station_vars + 0x76))[0x39] != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        *(int *)(station_vars + 0x7C) = -1;
        moby->state = 1;
        break;
    case REFUELING_STATION_STATE_WAIT_COMMAND:
        if ((D_L09_0015F5CC & 7) == (((int)moby >> 8) & 7)) {
            if (*(int *)(station_vars + 0x7C) == -1 ||
                (voice_is_playing(moby, *(int *)(station_vars + 0x7C)) == 0 && *(int *)(station_vars + 0x7C) == -1)) {
                *(int *)(station_vars + 0x7C) = func_0022ED80_r(1, 4, moby);
            }
        }
        if (moby->command == 1) {
            if (*(int *)(station_vars + 0x78) != -1) {
                unsigned char *o = (unsigned char *)(D_L09_0015FFD8_u + (*(int *)(station_vars + 0x78) << 8));
                if (o[0x20] == 1)
                    o[0xBC] = o[0x20];
            }
            *(short *)(station_vars + 0x74) = scale_game_frames(0xA);
            if (*(int *)(station_vars + 0x7C) != -1) {
                char *voice_entry = voice_pool + *(int *)(station_vars + 0x7C) * 0x70;
                if (*(char **)(voice_entry + 0x88) == moby && ((unsigned char *)voice_entry)[0x74] != 0) {
                    release_voice_slot(*(int *)(station_vars + 0x7C));
                }
            }
            *(int *)(station_vars + 0x7C) = -1;
            moby->update_dist = 0xFF;
            moby->state = 2;
        }
        break;
    case REFUELING_STATION_STATE_EXPLOSION_DELAY:
        if (fast_dec_timer_s16(station_vars + 0x74)) {
            if (*(int *)(station_vars + 0x70) != -1) {
                unsigned char *o = (unsigned char *)(D_L09_0015FFD8_u + (*(int *)(station_vars + 0x70) << 8));
                if (o != 0 && o[0x20] != 0xFE && o[0x20] != 0xFD &&
                    (*(short *)(o + 0xA6) == 0x494 || *(short *)(o + 0xA6) == 0x49D ||
                     *(short *)(o + 0xA6) == 0x4A0)) {
                    o[0x30] = 0xFF;
                    o[0xBC] = 1;
                }
            }
            moby->state = 3;
        }
        break;
    case REFUELING_STATION_STATE_EXPLODE:
        subtract_vector_xyz_c(explosion_offset, D_L09_00166F40, &moby->pos);
        spawn_explosion(moby, explosion_offset, 0, 0.0f, 0.0f, 0x14, 9, 0x20, 10.0f, 7.0f, 20.0f, 2.0f, 0,
                              40.0f, 0, 1, -1, 0);
        mark_moby_for_removal(moby);
        break;
    }
}
typedef struct {
    u8 pad0[0x26];
    s16 hurt_timer;
    u8 pad28[0x38];
    s32 link;
    s32 link2;
} VendorVars2;

void handle_linked_moby_damage(struct Moby *moby, VendorVars2 *vars, f32 *health) __asm__("FUN_L09_00303fc8");

void handle_linked_moby_damage(struct Moby *moby, VendorVars2 *vars, f32 *health) {
    enum {
        DAMAGE_RESULT_UNAVAILABLE = 0
    };
    VendorHitSource *damage_hit_record;
    struct Moby *link;
    s32 hit;

    if (moby->state != 3) {
        damage_hit_record = find_moby_hit(moby, 0x330000, 0);
        if (damage_hit_record != 0) {
            debug_print(D_L09_00208FF0, moby->oclass, fptodp(damage_hit_record->damage));
        }
        switch (FUN_00213928(moby, damage_hit_record, health, 0, &hit, 0, 0, 4)) {
        case 1:
        case 2:
            *health = 0.0f;
            break;
        case DAMAGE_RESULT_UNAVAILABLE:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            break;
        }
        if (hit >= 2) {
            if (*health <= damage_hit_record->damage) {
                *health = 0.0f;
                moby->flags &= ~0x1000;
                if (vars->link != -1) {
                    link = &moby_list[vars->link];
                    if (link != 0 && link->state != 0xFE && link->state != 0xFD
                        && (link->oclass == 0x494 || link->oclass == 0x49D || link->oclass == 0x4A0)) {
                        link->command = 1;
                    }
                }
                /* Retail checks the 0x64 link but indexes with the 0x60 one. */
                if (vars->link2 != -1) {
                    link = &moby_list[vars->link];
                    if (link != 0 && link->state != 0xFE && link->state != 0xFD
                        && (link->oclass == 0x494 || link->oclass == 0x49D || link->oclass == 0x4A0)) {
                        link->command = 1;
                    }
                }
                FUN_L00_00257470(moby, 0, -1);
                moby->state = 3;
            } else {
                *health -= damage_hit_record->damage;
                vars->hurt_timer = scale_game_frames(60);
            }
        }
        moby->hit_slot = 0xFF;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00305580), where it is exact; names translated to the US level program. */


extern char *D_L09_0015FFE4;
extern float FUN_001f9b48(void *, void *);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

char *find_nearby_chain_attachment(char *self, char *search_position, int bone) __asm__("FUN_L09_003041d0");

char *find_nearby_chain_attachment(char *self, char *search_position, int bone) {
    char *best = 0;
    float bestd = 1000000000.0f;
    char query_position[16];
    char candidate_joint_position[16];
    char *candidate_moby;
    char *tp = query_position;
    *(OvlQuad *)query_position = *(OvlQuad *)search_position;
    for (candidate_moby = D_L09_0015FFE4; candidate_moby != 0; candidate_moby = *(char **)(candidate_moby + 0x28)) {
        if (candidate_moby == self)
            continue;
        if (candidate_moby != 0 && *(unsigned char *)(candidate_moby + 0x20) != 0xFE &&
            *(unsigned char *)(candidate_moby + 0x20) != 0xFD) {
            if (*(short *)(candidate_moby + 0xA6) == 0x49D || *(short *)(candidate_moby + 0xA6) == 0x494 ||
                *(short *)(candidate_moby + 0xA6) == 0x4A0) {
                float candidate_distance;
                if (bone != -1) {
                    moby_point_position(candidate_moby, bone, candidate_joint_position);
                    candidate_distance = FUN_001f9b48(tp, candidate_joint_position);
                } else {
                    candidate_distance = FUN_001f9b48(tp, candidate_moby + 0x10);
                }
                if (candidate_distance < bestd) {
                    bestd = candidate_distance;
                    best = candidate_moby;
                }
            }
        }
    }
    if (best == 0)
        return 0;
    if (bestd < 0.5f)
        return best;
    if (*(short *)(best + 0xA6) == 0x4A0 && bestd < 10.0f)
        return best;
    return 0;
}
#ifndef NON_MATCHING
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Airship chain link (class 1181) update: finds the parts it hangs between, swings between them, and on its trigger drops, wakes its neighbours and bursts. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00305710), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef int u128_305710 __attribute__((mode(TI)));

typedef struct {
    char pad0[0x60];
    int hull;                  /* 0x60: moby index of the part it hangs from, -1 if none */
    int next;                  /* 0x64: moby index of the next part, -1 if none */
    unsigned char loose;       /* 0x68 */
    signed char flag;          /* 0x69 */
    short timer;               /* 0x6A */
    int trigger;               /* 0x6C */
} Data_305710;

typedef struct {
    char pad0[0x39];
    unsigned char b39;
} Flags_305710;

char *find_nearby_chain_attachment(char *self, char *pos, int bone) __asm__("FUN_L09_003041d0");
extern Moby *D_L09_0015FFD8;
extern char D_0013D388[];
extern char D_L09_00209050[];
extern char D_L09_00209080[];
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern f32 distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern float D_L09_00166F40[];
extern float vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern float subtract_vector_xyz_alt(float, float) __asm__("FUN_001fa580");
extern float vector_length_xyz_alt(float, float) __asm__("FUN_001fa5c8");
extern int D_L09_0015F5CC;
extern int stub_printf() __asm__("FUN_001e93b0");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
void FUN_L00_0025e450(f32 burst, f32 unused_float, f32 owner_effect, f32 owner_effect2, f32 cutoff, f32 scale, f32 flash, void *owner, s32 unused2, void *pos, s32 puff_count, s32 spark_count, s32 shard_count, s32 sound, s32 shake, s32 spray_count, s32 tint, u8 alternate);
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern Moby *func_L09_00305580_305710(void *, void *, int) __asm__("FUN_L09_003041d0");

void airship_chain_link_update(Moby *m) __asm__("FUN_L09_00304360");
void airship_chain_link_update(Moby *m) {
    Data_305710 *d = (*(Data_305710 **)&m->pvars);
    float v20[4];
    float v30[4];
    float v40[4];
    Moby *hull;
    Moby *next;
    Moby *r;
    float dist;
    int n;
    int t;

    switch (m->state) {
    case 0:
        if (d->flag != -1 && ((Flags_305710 *)(D_0013D388 + d->flag))->b39 != 0) {
            mark_moby_for_removal(m);
            return;
        }
        if (d->hull == -1) {
            FUN_L00_0024f7c8(m, 0, v20);
            r = func_L09_00305580_305710(m, v20, -1);
            if (r != 0) {
                d->hull = r - D_L09_0015FFD8;
            } else {
                stub_printf(D_L09_00209050, (*(short *)&m->save_id));
            }
        }
        if (d->next == -1) {
            *(u128_305710 *)v20 = *(u128_305710 *)((f32 *)&m->pos);
            r = func_L09_00305580_305710(m, v20, 0);
            if (r != 0) {
                d->next = r - D_L09_0015FFD8;
            } else {
                stub_printf(D_L09_00209080, (*(short *)&m->save_id));
            }
        }
        m->state = 1;
        break;
    case 1:
        if (m->visible == 0 || distance_xyz(((f32 *)&m->pos), D_L09_00166F40) > 32.0f) {
            if (D_L09_0015F5CC % 8 != (short)((*(short *)&m->save_id) % 8)) {
                return;
            }
        }
        hull = 0;
        next = 0;
        if (d->hull != -1) {
            hull = &D_L09_0015FFD8[d->hull];
            if (hull == 0 || hull->state == 0xFE || hull->state == 0xFD) {
                hull = 0;
            }
        }
        if (d->next != -1) {
            next = &D_L09_0015FFD8[d->next];
            if (next == 0 || next->state == 0xFE || next->state == 0xFD) {
                d->loose = 0;
                next = 0;
            } else if ((*(Data_305710 **)&next->pvars)->loose == 0) {
                d->loose = 0;
            }
        }
        if (d->loose == 0 && hull != 0 && next != 0 && next->oclass == 0x49D) {
            if ((*(Data_305710 **)&next->pvars)->loose != 0) {
                dist = vector_distance(((f32 *)&m->pos), ((f32 *)&hull->pos));
                m->rot.y = subtract_vector_xyz_alt(m->rot.y, vector_length_xyz_alt(-angle_atan2(dist, hull->pos.z - m->pos.z), m->rot.y) * 0.1f);
                m->rot.z = subtract_vector_xyz_alt(m->rot.z, vector_length_xyz_alt(angle_atan2(hull->pos.x - m->pos.x, hull->pos.y - m->pos.y), m->rot.z) * 0.1f);
            } else {
                subtract_vector_xyz(v20, ((f32 *)&hull->pos), ((f32 *)&m->pos));
                fast_vec_normalize(v20, v20, (vector_length_xyz(v20) - 1.3f) * 0.7f);
                if (next->oclass == 0x49D && next->state != 0xFE && next->state != 0xFD) {
                    subtract_vector_xyz(v30, ((f32 *)&next->pos), ((f32 *)&m->pos));
                    fast_vec_normalize(v30, v30, (vector_length_xyz(v30) - 1.3f) * 0.7f);
                    add_vector_xyz(v40, v20, v30);
                } else {
                    qcopy(v40, v20);
                }
                v40[2] -= frame_time_sq * 9.8f;
                add_vector_xyz(((f32 *)&m->pos), ((f32 *)&m->pos), v40);
                dist = vector_distance(((f32 *)&m->pos), ((f32 *)&hull->pos));
                m->rot.y = subtract_vector_xyz_alt(m->rot.y, vector_length_xyz_alt(-angle_atan2(dist, hull->pos.z - m->pos.z), m->rot.y) * 0.1f);
                m->rot.z = subtract_vector_xyz_alt(m->rot.z, vector_length_xyz_alt(angle_atan2(hull->pos.x - m->pos.x, hull->pos.y - m->pos.y), m->rot.z) * 0.1f);
            }
        } else {
            d->loose = 1;
        }
        if (m->command == 1) {
            if (d->trigger != -1) {
                Moby *o = &D_L09_0015FFD8[d->trigger];
                if (o->state == 1) {
                    o->command = o->state;
                }
            }
            d->timer = scale_game_frames_alt(10);
            m->update_dist = 0xFF;
            m->state = 2;
        }
        break;
    case 2:
        m->update_dist = 0xFF;
        hull = 0;
        if (d->hull != -1) {
            hull = &D_L09_0015FFD8[d->hull];
            if (hull == 0 || hull->state == 0xFE || hull->state == 0xFD) {
                hull = 0;
            }
        }
        if (hull != 0) {
            subtract_vector_xyz(v20, ((f32 *)&hull->pos), ((f32 *)&m->pos));
            fast_vec_normalize(v20, v20, vector_length_xyz(v20) - 1.3f);
            qcopy(v30, v20);
            n = scale_game_frames_alt(10);
            t = d->timer - 3;
            v30[2] -= frame_time_sq * 9.8f * 2.0f * convert_integer_to_float(n - t);
            add_vector_xyz(((f32 *)&m->pos), ((f32 *)&m->pos), v30);
            dist = vector_distance(((f32 *)&m->pos), ((f32 *)&hull->pos));
            m->rot.y = subtract_vector_xyz_alt(m->rot.y, vector_length_xyz_alt(-angle_atan2(dist, hull->pos.z - m->pos.z), m->rot.y) * 0.5f);
            m->rot.z = subtract_vector_xyz_alt(m->rot.z, vector_length_xyz_alt(angle_atan2(hull->pos.x - m->pos.x, hull->pos.y - m->pos.y), m->rot.z) * 0.5f);
        }
        if (fast_dec_timer_s16(&d->timer)) {
            if (d->hull != -1) {
                Moby *o = &D_L09_0015FFD8[d->hull];
                if (o != 0 && o->state != 0xFE && o->state != 0xFD
                    && (o->oclass == 0x494 || o->oclass == 0x49D || o->oclass == 0x4A0)) {
                    o->update_dist = 0xFF;
                    o->command = 1;
                }
            }
            if (d->next != -1) {
                Moby *o = &D_L09_0015FFD8[d->next];
                if (o != 0 && o->state != 0xFE && o->state != 0xFD
                    && (o->oclass == 0x494 || o->oclass == 0x49D || o->oclass == 0x4A0)) {
                    o->update_dist = 0xFF;
                    o->command = 1;
                }
            }
            m->state = 3;
        }
        break;
    case 3:
        subtract_vector_xyz(v20, D_L09_00166F40, ((f32 *)&m->pos));
        spawn_explosion(m, v20, 0, 0.0f, 0.0f, 10, 5, 8, 2.0f, 1.0f, 10.0f, 1.0f, 0, 10.0f, 0, 1, -1, 0);
        mark_moby_for_removal(m);
        break;
    }
}
#else
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float atan2_f(float, float) __asm__("FUN_001f9e90");
extern void scale_vector_00304360(float, void *, void *) __asm__("FUN_001f9bf8");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern char D_L09_00209050[];
extern char D_L09_00209080[];
extern void print_missing_link(char *, short) __asm__("FUN_001e93b0");
extern void effect_00304360(f32, f32, f32, f32, f32, f32, f32,
                             void *, void *, int, int, int, int, int, int, int, int, int) __asm__("FUN_L00_0025e450");

void airship_chain_link_update(unsigned char *moby) __asm__("FUN_L09_00304360");
void airship_chain_link_update(unsigned char *moby) {
    unsigned char *data = *(unsigned char **)(moby + 0x78);
    unsigned char *link;
    unsigned char *link2;
    float work[4], work2[4], work3[4];
    float angle;
    int index;
    int tick;
    short oclass;

    switch ((int)moby[0x20]) {
case 0:
    {
        unsigned char *link;
        if ((signed char)data[0x69] != -1 && ((unsigned char *)D_0013D388 + (signed char)data[0x69])[0x39] != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        if (*(int *)(data + 0x60) == -1) {
            FUN_L00_0024f7c8(moby, 0, work);
            link = (unsigned char *)find_nearby_chain_attachment((char *)moby, (char *)work, -1);
            if (link != 0) {
                *(int *)(data + 0x60) = ((char *)link - (char *)moby_list) >> 8;
            } else {
                print_missing_link(D_L09_00209050, *(short *)(moby + 0xb2));
            }
        }
        if (*(int *)(data + 0x64) == -1) {
            *(OvlQuad *)work = *(OvlQuad *)(moby + 0x10);
            link = (unsigned char *)find_nearby_chain_attachment((char *)moby, (char *)work, 0);
            if (link != 0) {
                *(int *)(data + 0x64) = ((char *)link - (char *)moby_list) >> 8;
            } else {
                print_missing_link(D_L09_00209080, *(short *)(moby + 0xb2));
            }
        }
        moby[0x20] = 1;
        return;
    }
case 1:
    {
        float rotation_scale;
        float height;
        if (moby[0x31] == 0 || FUN_001f9b48(moby + 0x10, D_L09_00166F40) > 32.0f) {
            int a = *(short *)(moby + 0xb2);
            int b = D_L09_0015F5CC;
            if (b % 8 != (short)(a % 8)) return;
        }
        link = 0;
        link2 = 0;
        index = *(int *)(data + 0x60);
        if (index != -1) {
            link = (unsigned char *)((char *)D_L09_0015FFD8_u + (index << 8));
            if (link == 0 || link[0x20] == 0xfe || link[0x20] == 0xfd) link = 0;
        }
        index = *(int *)(data + 0x64);
        if (index != -1) {
            link2 = (unsigned char *)((char *)moby_list + (index << 8));
            if (link2 == 0 || link2[0x20] == 0xfe || link2[0x20] == 0xfd) {
                data[0x68] = 0;
                link2 = 0;
            } else if (*(unsigned char *)(*(char **)(link2 + 0x78) + 0x68) == 0) {
                data[0x68] = 0;
            }
        }
        if (*(unsigned char *)(data + 0x68) == 0) {
            if (link == 0) {
                data[0x68] = 1;
            } else if (link2 != 0 && *(short *)(link2 + 0xa6) == 0x49d) {
                if (*(unsigned char *)(*(char **)(link2 + 0x78) + 0x68) != 0) {
                    angle = vector_distance(moby + 0x10, link + 0x10);
                    height = F(link, 0x18);
                    rotation_scale = 0.1f;
                } else {
                    subtract_vector_xyz(work, link + 0x10, moby + 0x10);
                    scale_vector_00304360((fast_vec_length(work) - 1.3f) * 0.7f, work, work);
                    if (*(short *)(link2 + 0xa6) == 0x49d && link2[0x20] != 0xfe && link2[0x20] != 0xfd) {
                        subtract_vector_xyz(work2, link2 + 0x10, moby + 0x10);
                        scale_vector_00304360((fast_vec_length(work2) - 1.3f) * 0.7f, work2, work2);
                        add_vector_xyz(work3, work, work2);
                    } else {
                        qcopy(work3, work);
                    }
                    work3[2] = work3[2] - frame_time_sq * 9.8f;
                    rotation_scale = 0.1f;
                    add_vector_xyz(moby + 0x10, moby + 0x10, work3);
                    angle = vector_distance(moby + 0x10, link + 0x10);
                    height = F(link, 0x18);
                }
                angle = atan2_f(angle, height - F(moby, 0x18));
                F(moby, 0x44) = fast_add_rotations(F(moby, 0x44), fast_subtract_rotations(-angle, F(moby, 0x44)) * rotation_scale);
                angle = atan2_f(F(link, 0x10) - F(moby, 0x10), F(link, 0x14) - F(moby, 0x14));
                F(moby, 0x48) = fast_add_rotations(F(moby, 0x48), fast_subtract_rotations(angle, F(moby, 0x48)) * rotation_scale);
            } else {
                data[0x68] = 1;
            }
        } else {
            data[0x68] = 1;
        }
        if (moby[0xbc] != 1) return;
        index = *(int *)(data + 0x6c);
        if (index != -1) {
            unsigned char *link2 = (unsigned char *)((char *)moby_list + (index << 8));
            if (link2[0x20] == 1) link2[0xbc] = 1;
        }
        *(short *)(data + 0x6a) = scale_game_frames(10);
        moby[0x30] = 0xff;
        moby[0x20] = 2;
        return;
    }
case 2:
    {
        unsigned char *link;
        link = 0;
        moby[0x30] = 0xff;
        index = *(int *)(data + 0x60);
        if (index != -1) {
            link = (unsigned char *)((char *)D_L09_0015FFD8_u + (index << 8));
            if (link == 0 || link[0x20] == 0xfe || link[0x20] == 0xfd) link = 0;
        }
        if (link != 0) {
            subtract_vector_xyz(work, link + 0x10, moby + 0x10);
            scale_vector_00304360(fast_vec_length(work) - 1.3f, work, work);
            qcopy(work2, work);
            tick = scale_game_frames(10);
            work2[2] -= (frame_time_sq * 9.8f + frame_time_sq * 9.8f) *
                        convert_integer_to_float(tick - (*(short *)(data + 0x6a) - 3));
            add_vector_xyz(moby + 0x10, moby + 0x10, work2);
            angle = vector_distance(moby + 0x10, link + 0x10);
            angle = atan2_f(angle, F(link, 0x18) - F(moby, 0x18));
            F(moby, 0x44) = fast_add_rotations(F(moby, 0x44), fast_subtract_rotations(-angle, F(moby, 0x44)) * 0.5f);
            angle = atan2_f(F(link, 0x10) - F(moby, 0x10), F(link, 0x14) - F(moby, 0x14));
            F(moby, 0x48) = fast_add_rotations(F(moby, 0x48), fast_subtract_rotations(angle, F(moby, 0x48)) * 0.5f);
        }
        if (fast_dec_timer_s16(data + 0x6a) == 0) return;
        index = *(int *)(data + 0x60);
        if (index != -1) {
            unsigned char *link;
            link = (unsigned char *)((char *)moby_list + (index << 8));
            if (link != 0 && link[0x20] != 0xfe && link[0x20] != 0xfd) {
                oclass = *(short *)(link + 0xa6);
                if (oclass == 0x494 || oclass == 0x49d || oclass == 0x4a0) {
                    link[0x30] = 0xff;
                    link[0xbc] = 1;
                }
            }
        }
        index = *(int *)(data + 0x64);
        if (index != -1) {
            unsigned char *link2;
            link2 = (unsigned char *)((char *)moby_list + (index << 8));
            if (link2 != 0 && link2[0x20] != 0xfe && link2[0x20] != 0xfd) {
                oclass = *(short *)(link2 + 0xa6);
                if (oclass == 0x494 || oclass == 0x49d || oclass == 0x4a0) {
                    link2[0x30] = 0xff;
                    link2[0xbc] = 1;
                }
            }
        }
        moby[0x20] = 3;
        return;
    }
case 3:
    {
        subtract_vector_xyz(work, D_L09_00166F40, moby + 0x10);
        effect_00304360(0.0f, 0.0f, 2.0f, 1.0f, 10.0f, 1.0f, 10.0f,
                          moby, work, 0, 10, 5, 8, 0, 0, 1, -1, 0);
        mark_moby_for_removal(moby);
    }
    }
}
#endif
#include "rnc/input/pad_state.h"
/* Gaspar turret the hero mounts: aims with the stick, fires its two barrels in turn,
   and while idle swings toward the hero with its beam light on. */
typedef struct {
    u8 pad0[0x10];
    Vec4 aim;                         /* 0x10: FUN_L00_001fff28 target */
    u8 pad20[0x20];
    s16 unk40;
    s16 mount_delay;                  /* 0x42: frames before the hero can mount again */
    f32 pitch;                        /* 0x44 */
    s32 unk48;
    s32 unk4C;
    u8 pad50[2];
    s16 dismount_delay;               /* 0x52 */
    s16 barrel;                       /* 0x54: 0 fires barrel 1 next, 1 barrel 2 */
    u8 pad56[6];
    f32 rest_z;                       /* 0x5C */
    f32 yaw_speed;                    /* 0x60 */
    f32 pitch_speed;                  /* 0x64 */
    s32 beam;                         /* 0x68: D_L09_00180440 slot, -1: none */
    s32 beam_frames;                  /* 0x6C */
} TurretVars;

typedef struct {
    f32 size;
    u8 pad4[0xC];
    f32 pos[3];
    f32 alpha;
} TurretBeam;

typedef struct {
    u8 pad0[0x160];
    f32 unk160;
    u8 pad164[4];
    s32 unk168;
    u8 pad16C[4];
    f32 unk170;
    u8 pad174[4];
    s32 unk178;
} TurretShake;

/* moby the sound collision probe ignores (clamp_voice_position_to_collision) */
extern struct Moby *sound_probe_ignore[] __asm__("D_0013E5BC");
extern s32 D_0015EDDC;
#include "rnc/overlay/l09/gameplay/entities/elevator_globals.h"
extern f32 turret_camera_offset_length_gp __asm__("D_L09_00161CB0") __attribute__((sda));
extern f32 turret_camera_z_offset_gp __asm__("D_L09_00161CB4") __attribute__((sda));
extern f32 D_L09_00161CC0[2] __attribute__((sda)); /* a vector; declared small so Ps2EeAs reaches it through $gp */
extern TurretShake D_L09_00166E00;
extern TurretBeam D_L09_00180440[];
extern void spawn_class_144_moby(struct Moby *, Vec4 *) __asm__("FUN_L09_002efda8");
extern struct Moby *spawn_class_4ea_projectile(void *, void *, void *, int) __asm__("FUN_L09_00307ba8");
extern void FUN_L08_00313ba8(void);
extern int allocate_colored_effect_slot(void *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");
extern void FUN_L00_0026dd20(void *, void *, int, int, float);
extern void divide_vector_xyz(void *, void *, float) __asm__("FUN_L00_001ff378");
extern void suck_draw_reticle(void *, int, void *, int, int, int, float, float, float) __asm__("FUN_L00_001edf60");
extern void FUN_L00_002eac18(int);
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void fast_mem_zero16(void *, int) __asm__("FUN_001f9810");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern void attach_manipulator(struct Moby *, s32, void *) __asm__("FUN_0020cb10");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00250df8(void *);
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_L00_001fff28(void *, int, float);

void turret_update(struct Moby *moby) __asm__("FUN_L09_00304c80");
void turret_update(struct Moby *moby) {
    enum {
        TURRET_STATE_INITIALIZE = 0,
        TURRET_STATE_IDLE = 4,
        TURRET_STATE_MOUNTED = 5
    };
    Vec4 seat;
    Vec4 offset;
    Vec4 work_vector;
    Vec4 shot;
    Vec4 muzzle;
    Vec4 firing_vector_or_camera_rotation;
    Vec4 rnd;
    TurretVars *vars;
    TurretBeam *beam;
    float stick_magnitude;
    float stick_y;
    int beam_frame_count;
    int i;

    vars = (TurretVars *)moby->pvars;
    moby->flags &= ~4;
    moby_point_position(moby, 3, &seat);
    seat.f[2] += 0.4f;
    if (moby->state >= 4) {
        if (fast_dec_timer_s16(&vars->mount_delay) != 0 && moby->command == 1 &&
            hero.state.control_mode != 0x14 && hero.state.control_mode != 7 && hero.state.current != 0x3D) {
            qcopy(&hero.motion.pos, &seat);
            qcopy(&hero.motion.rot, &moby->rot);
            hero.motion.rot.f[2] = fast_add_rotations(1.5707964f, hero.motion.rot.f[2]);
            offset.f[0] = fast_cos(hero.motion.rot.f[2]) * -0.7f;
            offset.f[1] = fast_sin(hero.motion.rot.f[2]) * -0.7f;
            offset.f[2] = 0.0f;
            add_vector_xyz(&hero.motion.pos, &hero.motion.pos, &offset);
            fast_mem_zero16(&hero.motion.velocity, 0x90);
            if (D_0013CAE4 & 0x10) {
                FUN_L00_002eac18(0);
                hero_set_state(0, 1);
                moby->command = 2;
                moby->state = 4;
                blend_moby_animation(moby, 0, 0, 1);
            } else {
                if (moby->state != 5) {
                    vars->dismount_delay = 2;
                    jet_mount(&moby->pos, &moby->rot, 0, 0, 0);
                    if (moby->prev_seq != 5) {
                        blend_moby_animation(moby, 5, 0, scale_game_frames(5));
                    }
                }
                moby->state = 5;
            }
        } else if (moby->command == 2) {
            if (FUN_001f9b48(&seat, &hero.moby->pos) > 1.2f || hero.ground_moby != moby) {
                moby->command = 0;
                vars->mount_delay = scale_game_frames(60);
            }
        } else if (hero.state.control_mode != 0x14 && hero.state.control_mode != 7 &&
                   hero.state.current != 0x3D && hero.ground_moby == moby &&
                   FUN_001f9b48(&seat, &hero.moby->pos) < 1.2f && vars->mount_delay == 0) {
            moby->command = 1;
            hero_set_state(0x32, 1);
        }
    }
    switch (moby->state) {
    case TURRET_STATE_INITIALIZE:
        attach_manipulator(moby, 0, vars);
        work_vector.q = *(u128 *)&moby->pos;
        spawn_class_144_moby(moby, &work_vector);
        moby->state = 4;
        vars->rest_z = moby->pos.z;
        vars->pitch = 0.0f;
        moby->rot.z = 0.0f;
        vars->beam = -1;
        vars->mount_delay = 0;
        vars->unk48 = 0;
        vars->unk4C = 0;
        vars->unk40 = 0;
        sound_probe_ignore[0] = moby;
        return;
    case TURRET_STATE_IDLE:
        if (vector_distance(&moby->pos, &hero.motion.pos) < 4.0f || hero.ground_moby == moby) {
            moby->pos.z += ((vars->rest_z - 1.0f) - moby->pos.z) * 0.11f;
            vars->yaw_speed = fast_add_rotations(vars->yaw_speed,
                fast_add_rotations(fast_subtract_rotations(
                    fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y),
                    moby->rot.z), 1.5707964f) * 0.01f) * 0.9f;
            moby->rot.z = fast_add_rotations(moby->rot.z, vars->yaw_speed);
            transform_vector_by_basis(&work_vector, D_L09_00161CC0, &moby->rot_row0);
            add_vector_xyz(&work_vector, &work_vector, &seat);
            if (vars->beam == -1) {
                vars->beam_frames = 0;
                vars->beam = allocate_colored_effect_slot(&work_vector, 0.4f, 0.0f, 4.0f, 4.0f, 4.0f);
                allocate_voice_for_target_entry(1, 0, moby);
            } else {
                beam = &D_L09_00180440[vars->beam];
                beam->pos[0] = work_vector.f[0];
                beam->pos[1] = work_vector.f[1];
                beam->pos[2] = work_vector.f[2];
                beam_frame_count = ++vars->beam_frames;
                if (beam_frame_count > 40 || !(beam_frame_count & 4)) {
                    beam->size = 4.0f;
                    beam->alpha = 0.4f;
                } else if (beam_frame_count >= 25 && (beam_frame_count & 0xA) == 0xA) {
                    beam->size = 4.0f;
                    beam->alpha = 0.4f;
                } else {
                    beam->alpha = 0.0f;
                }
            }
        } else {
            if (moby->pos.z < vars->rest_z) {
                moby->pos.z += frame_time;
            } else {
                moby->pos.z = vars->rest_z;
            }
            if (hero.ground_moby != moby && vars->beam != -1) {
                allocate_voice_for_target_entry(2, 0, moby);
                release_point_light_slot(vars->beam);
                vars->beam = -1;
            }
        }
        break;
    case TURRET_STATE_MOUNTED:
        sound_probe_ignore[0] = moby;
        if (moby->pos.z < vars->rest_z) {
            moby->pos.z += frame_time;
        } else {
            moby->pos.z = vars->rest_z;
        }
        work_vector.f[0] = hero.stick_x;
        stick_y = -hero.stick_y;
        if (!D_0015EDDC) {
            stick_y = hero.stick_y;
        }
        work_vector.f[1] = stick_y;
        stick_magnitude = vector_length_xy(&work_vector);
        if (stick_magnitude > 1.0f) {
            divide_vector_xyz(&work_vector, &work_vector, stick_magnitude);
        }
        vars->yaw_speed += frame_time_sq * 3.1415927f * -work_vector.f[0];
        vars->pitch_speed += frame_time_sq * 0.7853982f * -work_vector.f[1];
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->yaw_speed);
        vars->pitch = fast_add_rotations(vars->pitch, vars->pitch_speed);
        if (vars->pitch < -0.5235988f) {
            vars->pitch = -0.5235988f;
            vars->pitch_speed = 0.0f;
        } else if (vars->pitch > 0.5235988f) {
            vars->pitch = 0.5235988f;
            vars->pitch_speed = 0.0f;
        }
        vars->yaw_speed *= 0.97f;
        vars->pitch_speed *= 0.97f;
        FUN_L00_001fff28(&vars->aim, 1, -vars->pitch);
        if (controller_state.held & 0x20) {
            if (controller_state.pressed & 0x20) {
                blend_moby_animation(moby, vars->barrel == 0 ? 4 : 3, 0, 1);
            } else if (moby->seq == 3) {
                if (absolute_float(compute_interpolated_record_value(moby) - 1.0f) <= 0.25f) {
                    build_spherical_offset(&shot, frame_time * 60.0f, fast_add_rotations(1.5707964f, moby->rot.z), vars->pitch);
                    moby_point_position(moby, 1, &muzzle);
                    spawn_class_4ea_projectile(moby, &muzzle, &shot, scale_game_frames(90));
                    allocate_voice_for_target_entry(0, 0, moby);
                    D_L09_00166E00.unk160 = 0.03f;
                    D_L09_00166E00.unk168 = scale_game_frames(10);
                    D_L09_00166E00.unk170 = 0.03f;
                    D_L09_00166E00.unk178 = scale_game_frames(7);
                    vars->barrel = 0;
                    for (i = 0; i < 5; i++) {
                        rnd.q = 0;
                        rnd.f[0] = random_float_between(-1.0f, 1.0f);
                        rnd.f[1] = random_float_between(-1.0f, 1.0f);
                        rnd.f[2] = random_float_between(-1.0f, 1.0f);
                        firing_vector_or_camera_rotation.q = rnd.q;
                        normalize_vector_xyz(&firing_vector_or_camera_rotation, &firing_vector_or_camera_rotation, fast_vec_length(&shot) * (frame_scale * (0.65f - 1.0f) + 1.0f));
                        add_vector_xyz(&firing_vector_or_camera_rotation, &shot, &firing_vector_or_camera_rotation);
                        normalize_vector_xyz(&firing_vector_or_camera_rotation, &firing_vector_or_camera_rotation, random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                        FUN_L00_0026dd20(&muzzle, &firing_vector_or_camera_rotation, 0x5F2F4F6F, random_int_between(scale_game_frames(5), scale_game_frames(10)), 50000.0f);
                    }
                    normalize_vector_xyz(&shot, &shot, frame_time * 0.5f);
                    FUN_L00_0026dd20(&muzzle, &shot, 0x2F4F7F7F, random_int_between(scale_game_frames(4), scale_game_frames(7)), 500000.0f);
                } else if (moby->anim_flags & 2) {
                    blend_moby_animation(moby, 4, 0, 1);
                }
            } else if (moby->seq == 4) {
                if (absolute_float(compute_interpolated_record_value(moby) - 1.0f) <= 0.25f) {
                    build_spherical_offset(&shot, frame_time * 60.0f, fast_add_rotations(1.5707964f, moby->rot.z), vars->pitch);
                    moby_point_position(moby, 2, &muzzle);
                    spawn_class_4ea_projectile(moby, &muzzle, &shot, scale_game_frames(90));
                    allocate_voice_for_target_entry(0, 0, moby);
                    D_L09_00166E00.unk160 = 0.03f;
                    D_L09_00166E00.unk168 = scale_game_frames(10);
                    D_L09_00166E00.unk170 = 0.03f;
                    D_L09_00166E00.unk178 = scale_game_frames(7);
                    vars->barrel = 1;
                    for (i = 0; i < 5; i++) {
                        rnd.q = 0;
                        rnd.f[0] = random_float_between(-1.0f, 1.0f);
                        rnd.f[1] = random_float_between(-1.0f, 1.0f);
                        rnd.f[2] = random_float_between(-1.0f, 1.0f);
                        firing_vector_or_camera_rotation.q = rnd.q;
                        normalize_vector_xyz(&firing_vector_or_camera_rotation, &firing_vector_or_camera_rotation, fast_vec_length(&shot) * (frame_scale * (0.65f - 1.0f) + 1.0f));
                        add_vector_xyz(&firing_vector_or_camera_rotation, &shot, &firing_vector_or_camera_rotation);
                        normalize_vector_xyz(&firing_vector_or_camera_rotation, &firing_vector_or_camera_rotation, random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                        FUN_L00_0026dd20(&muzzle, &firing_vector_or_camera_rotation, 0x5F2F4F6F, random_int_between(scale_game_frames(5), scale_game_frames(10)), 50000.0f);
                    }
                    normalize_vector_xyz(&shot, &shot, frame_time * 0.5f);
                    FUN_L00_0026dd20(&muzzle, &shot, 0x2F4F7F7F, random_int_between(scale_game_frames(4), scale_game_frames(7)), 500000.0f);
                } else if (moby->anim_flags & 2) {
                    blend_moby_animation(moby, 3, 0, 1);
                }
            }
        } else if (moby->prev_seq != 5 && (moby->anim_flags & 2)) {
            blend_moby_animation(moby, 5, 0, 1);
        }
        if (fast_dec_timer_s16(&vars->dismount_delay) != 0) {
            FUN_L00_00250df8(moby);
            moby->flags |= 4;
            FUN_L08_00313ba8();
            moby_point_position(moby, 0, &work_vector);
            build_spherical_offset(&shot, turret_camera_offset_length_gp, fast_add_rotations(1.5707964f, moby->rot.z), vars->pitch);
            shot.f[2] += turret_camera_z_offset_gp;
            add_vector_xyz(&muzzle, &work_vector, &shot);
            FUN_L00_002ea9d8(&muzzle);
            firing_vector_or_camera_rotation.f[0] = 0.0f;
            firing_vector_or_camera_rotation.f[1] = -vars->pitch;
            firing_vector_or_camera_rotation.f[2] = fast_add_rotations(1.5707964f, moby->rot.z);
            FUN_L00_002eaa30(&firing_vector_or_camera_rotation);
        }
        suck_draw_reticle(moby, 0xFF0F0FFF, 0, 0xD, -1, 4, 1.0f, 0.0f, 90.0f);
        break;
    }
}
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_001f9bf8(f32, void *, void *);
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void initialize_hit_descriptor(void *, void *, s32, f32) __asm__("FUN_L00_00259858");
extern s32 collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern char D_L09_002090B0[];

typedef struct { char pad[0x18]; char *hit; } CollisionState;
extern CollisionState D_L09_00173FC0;

void update_damage_causer(struct Moby *moby) __asm__("FUN_L09_00305a28");

void update_damage_causer(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float work[24];
    int i;
    float count_as_float;
    float start;
    float end;

    if (moby->state == 0) {
        moby->state = 1;
        *(s32 *)(data + 0x14) = -1;
    }
    if ((D_L09_0015F5CC & 7) == (((int)moby >> 8) & 7)) {
        if (*(s32 *)(data + 0x14) == -1 ||
            (voice_is_playing(moby, *(s32 *)(data + 0x14)) == 0 &&
             *(s32 *)(data + 0x14) == -1)) {
            *(s32 *)(data + 0x14) = allocate_voice_for_target_entry(0, 4, moby);
        }
    }
    count_as_float = convert_integer_to_float(*(s32 *)(data + 8));
    start = F(data, 0);
    end = F(data, 4);
    *(OvlQuad *)work = 0;
    work[2] = (end - (start + start)) / count_as_float;
    qcopy(work + 4, (char *)moby + 0x10);
    transform_vector_by_basis(work, work, (char *)moby + 0xc0);
    FUN_001f9bf8(F(data, 0), work + 8, work);
    fast_vec_add(work + 4, work + 4, work + 8);
    initialize_hit_descriptor(work + 12, moby, *(s32 *)(data + 0x10), *(float *)(data + 0xc));
    i = 0;
    if (*(s32 *)(data + 8) > 0) do {
        if (collect_mobies_in_sphere(F(data, 0), work + 4, 1, moby, work + 12) != 0) {
            char *hit = D_L09_00173FC0.hit;
            stub_printf(D_L09_002090B0, *(s16 *)((char *)moby + 0xb2), *(s16 *)(hit + 0xa6),
                        *(s16 *)(hit + 0xb2), F(data, 0xc));
        }
        i++;
        fast_vec_add(work + 4, work + 4, work);
    } while (i < *(s32 *)(data + 8));
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* spawn a moby copying position, rotation and data word from owner */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_003095D0), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00250df8(void *);
extern void copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");

char *spawn_barrier_replacement(struct Moby *owner, int spawned_moby_class) __asm__("FUN_L09_00308220");

char *spawn_barrier_replacement(struct Moby *owner, int spawned_moby_class) {
    char *moby = func_0020D348_m(spawned_moby_class);
    if (moby != 0) {
        unsigned char state;
        ((unsigned char *)moby)[0x30] = 0xFF;
        state = 0xFF;
        *(short *)(moby + 0x32) = state;
        moby[0x31] = 1;
        moby[0x20] = 0;
        qcopy(moby + 0x10, &owner->pos);
        qcopy(moby + 0x40, &owner->rot);
        moby[0xBC] = scale_game_frames(10);
        FUN_L00_00250df8(moby);
        copy_hero_moby_field_38(moby);
        *(float *)(moby + 0x2C) = owner->scale;
        *(unsigned short *)(moby + 0x34) = owner->flags;
        **(int **)(moby + 0x78) = *(int *)owner->pvars;
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00309688), where it is exact; names translated to the US level program. */

typedef struct BarrierData_309688 {
    int idx;
} BarrierData_309688;

typedef struct Hit_309688 {
    char pad0[0x10];
    float pos[4];
} Hit_309688;

typedef struct Flags_309688 {
    char pad0[0x43];
    unsigned char f43;
} Flags_309688;

char *FUN_L09_00308220_c(struct Moby *owner, int cls) __asm__("FUN_L09_00308220");
extern char *FUN_L00_0025a420(void *, int, int);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz_c(void *) __asm__("FUN_001f9af0");
#include "rnc/overlay/l09/gameplay/entities/elevator_data/barrier_update.h"
extern int FUN_L00_001fefc8();
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern unsigned char D_0014C050_c[] __asm__("D_0014C050");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
float absolute_float(float input) __asm__("func_001F99C0");
struct Moby *spawn_tumbling_debris(struct Moby *src, Vec4 *pos, Vec4 *vel, s32 kind, f32 scale) __asm__("FUN_L09_002ef868");
void FUN_L00_0025e450(f32 burst, f32 unused_float, f32 owner_effect, f32 owner_effect2, f32 cutoff, f32 scale, f32 flash, void *owner, s32 unused2, void *pos, s32 puff_count, s32 spark_count, s32 shard_count, s32 sound, s32 shake, s32 spray_count, s32 tint, u8 alternate);
extern int func_L00_001FEF78_309688(void *) __asm__("FUN_L00_001fefc8");
extern void func_L00_00260108_309688(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern Hit_309688 *func_L00_0025B478_309688(void *, int, int) __asm__("FUN_L00_0025a420");
extern int func_L09_003095D0_309688(void *, int) __asm__("FUN_L09_00308220");
extern int func_001F9850_309688(int) __asm__("FUN_001f96f8");
extern int func_0022ED80_309688(int, int, void *) __asm__("FUN_0022da68");
extern float func_002140F8_309688(float, float) __asm__("FUN_002132a8");
extern float func_001F9F90_309688(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_309688(float) __asm__("FUN_001f9de0");
extern void func_001F9EC0_309688(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float func_001F9B88_309688(float) __asm__("FUN_001f99c0");
extern void func_L00_001FF4B0_309688(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_L09_002F0BB8_309688(void *, void *, void *, int, float) __asm__("FUN_L09_002ef868");
extern float func_001FA888_309688(int) __asm__("FUN_001fa6c0");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_L00_0025F4A8_309688(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void func_0020D678_309688(void *) __asm__("FUN_0020c828");
extern unsigned char D_0014171B_309688[] __asm__("D_0014C050");
extern char alternate_item_available[] __asm__("D_0013D388");

void barrier_update(Moby *moby) __asm__("FUN_L09_003082d8");
void barrier_update(Moby *moby)
{
    BarrierData_309688 *data = (*(BarrierData_309688 **)&moby->pvars);
    float origin[4], center[4], piece_pos[4], piece_velocity[4], push[4];
    Hit_309688 *hit;
    int has_next;
    int count;
    int i;
    float angle;
    float scale;

    if (moby->state == 0) {
        if (data->idx != -1) ((Flags_309688 *)(alternate_item_available + data->idx))->f43 = 0;
        moby->state = 1;
    }
    if (moby->mission != 0xFF) {
        int row = current_level_index * 16;
        if (D_0014171B_309688[moby->mission + row] == 0xFF) {
            if (data->idx != -1) ((Flags_309688 *)(alternate_item_available + data->idx))->f43 = 1;
            func_0020D678_309688(moby);
            return;
        }
    }
    has_next = 0;
    hit = func_L00_0025B478_309688(moby, 0x40000, 0);
    if (func_L00_001FEF78_309688(&moby->command) == 0) return;
    if (hit == 0) return;
    if (moby->oclass < 0x508) has_next = func_L09_003095D0_309688(moby, moby->oclass + 1) != 0;
    moby->command = func_001F9850_309688(10);
    qcopy(origin, ((f32 *)&moby->pos));
    qcopy(center, ((f32 *)&moby->pos));
    if (moby->oclass < 0x508) {
        func_0022ED80_309688(0, 0, moby);
    } else {
        func_0022ED80_309688(0, 0, moby);
    }
    moby->hit_slot = 0xFF;
    count = 40;
    if (0.85f < radial_effect_count_reduction_control || 0.85f < barrier_radial_effect_count_reduction_control) count = 30;
    angle = -3.1415927f;
    for (i = 0; i < count; i++) {
        *(u128 *)piece_velocity = 0;
        piece_velocity[0] = func_002140F8_309688(-0.4f, 0.4f);
        piece_velocity[1] = func_002140F8_309688(-0.6f, 0.6f);
        piece_velocity[2] = func_002140F8_309688(-0.4f, 0.4f);
        *(u128 *)piece_pos = *(u128 *)piece_velocity;
        scale = func_002140F8_309688(0.3f, 2.6f);
        piece_pos[0] = piece_pos[0] + scale * func_001F9F90_309688(angle);
        scale = scale * func_001F9FA8_309688(angle);
        piece_pos[2] = piece_pos[2] + scale;
        func_001F9EC0_309688(piece_pos, piece_pos, ((f32 *)&moby->rot_row0));
        fast_vec_add(piece_pos, piece_pos, center);
        fast_vec_sub(piece_velocity, piece_pos, origin);
        scale = func_001F9B88_309688(6.0f - fast_vec_length(piece_velocity)) / 6.0f;
        func_L00_001FF4B0_309688(push, hit->pos, scale * scale * 8.0f);
        fast_vec_add(piece_velocity, piece_velocity, push);
        func_L00_001FF4B0_309688(piece_velocity, piece_velocity, scale * (func_002140F8_309688(5.0f, 8.0f) * frame_time));
        if (moby->oclass == 0x508) {
            int odd = i & 1;
            func_L09_002F0BB8_309688(moby, piece_pos, piece_velocity, odd, func_002140F8_309688(1.0f, 1.3f));
        }
        if (i % 3 == 0) {
            func_L00_00260108_309688(moby, piece_pos, -1, func_002140F8_309688(0.3f, 0.6f), 0.0f);
        }
        angle = angle + 6.2831855f / func_001FA888_309688(count);
    }
    fast_vec_scale(piece_pos, hit->pos, 0.5f);
    func_L00_0025F4A8_309688(moby, hit->pos, origin, 0.0f, 0.0f, 10, 3, 16, 3.0f, 2.0f, 1.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
    if (moby->oclass == 0x508) {
        if (data->idx != -1) ((Flags_309688 *)(alternate_item_available + data->idx))->f43 = 1;
    }
    if (moby->oclass == 0x508 || has_next) func_0020D678_309688(moby);
}
#include "qzero.h"

/* Level 9 moby update: 0 sets up and either rises from below or reports a missing target, 1 waits for the target
 * moby to die, 2 rides with the hero (wobbles data+0x60/0x64 towards the hero's offset) or settles back.
 * The settling branch stores the height through a struct member (Mpos): a member store does not make the compiler
 * reload D_0015EE6C, a cast store does. The hero block is offsets of D_0013E633 cast to a struct, as retail
 * derives 0xE1D from the register holding 0xE9D. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_0030A560), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef int u128_30A560 __attribute__((mode(TI)));

typedef struct {
    char pad0[0x18];
    float z;
} Mpos_30A560;

extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern int D_L09_0015FFD8_c __asm__("D_L09_0015FFD8");
extern float floating_ship_piece_hero_offset_tilt_gain_gp __asm__("D_L09_00161DC4") __attribute__((sda));
extern float floating_ship_piece_sink_speed_gp __asm__("D_L09_00161DC8") __attribute__((sda));
extern float floating_ship_piece_height_return_rate_gp __asm__("D_L09_00161DCC") __attribute__((sda));
#include "rnc/overlay/l09/gameplay/entities/elevator_data/update_floating_ship_piece.h"
extern int DebugPrint_alt() __asm__("FUN_001e93b0");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern unsigned char D_0013F350[];
extern void FUN_L00_00260738_c(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern void scale_vector_xyz_c2(void *, void *, float) __asm__("FUN_001f9a68");
float FUN_L00_0025bc98(float *p, float *q, int n, float x, float a, float b, float c);
void reorient_position_about_pivot(void *a0, void *a1, void *a2, void *a3, void *a4) __asm__("FUN_L07_00288968");
extern void func_001F9C30_30A560(void *, void *, float) __asm__("FUN_001f9a68");
extern float func_002140F8_30A560(float, float) __asm__("FUN_002132a8");
extern void func_001E9730_30A560() __asm__("FUN_001e93b0");
extern int func_001F9908_30A560(int *) __asm__("FUN_001f9740");
extern int func_0022ED80_30A560(int, int, char *) __asm__("FUN_0022da68");
extern int func_001F9850_30A560(int) __asm__("FUN_001f96f8");
extern void func_001F9BF0_30A560(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
extern float func_L00_0025CCF0_30A560(void *, float, void *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern void reorient_position_about_pivot(void *a0, void *a1, void *a2, void *a3, void *a4) __asm__("FUN_L07_00288968");
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern float func_001FA748_30A560(float, float) __asm__("FUN_001fa580");
extern void func_001F9BD8_30A560(void *, void *, void *) __asm__("FUN_001f9a10");
extern void func_L00_002617B0_30A560(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern char D_L09_00209518_30A560[] __asm__("D_L09_00209228");
extern unsigned char D_0013E633_30A560[] __asm__("D_0013F350");
extern unsigned char D_0013F3D0[];

void update_floating_ship_piece(char *moby) __asm__("FUN_L09_003091b0");

void update_floating_ship_piece(char *moby) {
    enum {
        PLATFORM_STATE_INITIALIZE = 0,
        PLATFORM_STATE_WAIT_FOR_LINK_REMOVAL = 1,
        PLATFORM_STATE_ACTIVE = 2
    };
    char *data = *(char **)(moby + 0x78);
    float movement[4];
    float original_rotation[4];
    float hero_offset[4];
    float old_angles[4];
    float old_pos[4];
    float carried_pos[4];
    float start_z, new_z, sink_level, old_z;
    int state;
    char *linked;
    float side_dot, front_dot;
    unsigned char linked_state;

    func_001F9C30_30A560(movement, moby + 0x10, -1.0f);
    qcopy(original_rotation, moby + 0x40);
    old_z = *(float *)(moby + 0x18);
    sink_level = *(float *)(data + 0x84) - 0.300000012f;
    state = ((unsigned char *)moby)[0x20];
    switch (state) {
    case PLATFORM_STATE_INITIALIZE:
        *(int *)(data + 0x80) = 1;
        qcopy(data + 0x60, moby + 0x40);
        qzero(data + 0x70);
        start_z = *(float *)(moby + 0x18);
        *(float *)(data + 0x84) = start_z;
        if (*(int *)(data + 0x8C) != -1) {
            *(float *)(moby + 0x18) = start_z - 5.0f;
            *(float *)(moby + 0x40) = func_002140F8_30A560(-1.57079637f, 1.57079637f);
            *(float *)(moby + 0x44) = func_002140F8_30A560(-1.57079637f, 1.57079637f);
            moby[0x20] = 1;
        } else {
            func_001E9730_30A560(D_L09_00209518_30A560, *(short *)(moby + 0xB2), *(short *)(moby + 0xA6));
            moby[0x20] = 2;
        }
        break;
    case PLATFORM_STATE_WAIT_FOR_LINK_REMOVAL:
        linked = (char *)(D_L09_0015FFD8_c + (*(int *)(data + 0x8C) << 8));
        if (linked == 0) {
            goto set2;
        }
        linked_state = ((unsigned char *)linked)[0x20];
        if (linked_state == 0xFE) {
            goto set2;
        }
        if (linked_state != 0xFD) {
            break;
        }
    set2:
        moby[0x20] = 2;
        break;
    case PLATFORM_STATE_ACTIVE:
        if (((Hero *)D_0013E633_30A560)->air_frames.s == 0 && ((char *)((Hero *)D_0013E633_30A560)->ground_moby) == moby) {
            if (*(int *)(data + 0x88) == 0) {
                func_0022ED80_30A560(0, 0, moby);
            }
            if (*(int *)(data + 0x88) < func_001F9850_30A560(30)) {
                *(int *)(data + 0x88) = func_001F9850_30A560(30);
            }
        }
        if (*(int *)(data + 0x88) != 0) {
            float deg_to_rad;
            float tilt;
            float pitch_target;
            func_001F9908_30A560((int *)(data + 0x88));
            func_001F9BF0_30A560(hero_offset, (D_0013E633_30A560 + 0x80), moby + 0x10);
            deg_to_rad = 0.0174532924f;
            side_dot = fast_vec_dot(hero_offset, moby + 0xC0);
            front_dot = fast_vec_dot(hero_offset, moby + 0xD0);
            tilt = floating_ship_piece_hero_offset_tilt_gain_gp;
            pitch_target = tilt * side_dot;
            *(u128_30A560 *)old_angles = *(u128_30A560 *)(data + 0x60);
            *(u128_30A560 *)old_pos = *(u128_30A560 *)(moby + 0x10);
            func_L00_0025CCF0_30A560(data + 0x60, -tilt * front_dot, data + 0x70, 0,
                              floating_ship_piece_loaded_tilt_time_squared_gain_gp * deg_to_rad * frame_time_sq,
                              floating_ship_piece_loaded_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                              floating_ship_piece_loaded_tilt_snap_tolerance_scale_gp * deg_to_rad * frame_time);
            func_L00_0025CCF0_30A560(data + 0x64, pitch_target, data + 0x74, 0,
                              floating_ship_piece_loaded_tilt_time_squared_gain_gp * deg_to_rad * frame_time_sq,
                              floating_ship_piece_loaded_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                              floating_ship_piece_loaded_tilt_snap_tolerance_scale_gp * deg_to_rad * frame_time);
            new_z = *(float *)(moby + 0x18) - floating_ship_piece_sink_speed_gp * frame_time;
            *(float *)(moby + 0x18) = new_z;
            if (sink_level < old_z && new_z <= sink_level) {
                func_0022ED80_30A560(1, 0, moby);
            }
            reorient_position_about_pivot((D_0013E633_30A560 + 0x80), carried_pos, moby + 0x10, old_angles, data + 0x60);
            func_001F9BF0_30A560(D_0013E633_30A560 + 0xF0, carried_pos, (D_0013E633_30A560 + 0x80));
            ((Hero *)D_0013E633_30A560)->motion.platform_delta.f[2] = ((Hero *)D_0013E633_30A560)->motion.platform_delta.f[2] + (*(float *)(moby + 0x18) - old_pos[2]);
        } else {
            float rise;
            ((Mpos_30A560 *)moby)->z = ((Mpos_30A560 *)moby)->z + (rise = floating_ship_piece_height_return_rate_gp * frame_time) * (*(float *)(data + 0x84) - ((Mpos_30A560 *)moby)->z);
            if (((Mpos_30A560 *)moby)->z < *(float *)(data + 0x84) - 2.0f) {
                float deg_to_rad;
                ((Mpos_30A560 *)moby)->z = ((Mpos_30A560 *)moby)->z + rise;
                deg_to_rad = 0.0174532924f;
                func_L00_0025CCF0_30A560(data + 0x60, 0.0f, data + 0x70, 0,
                                  floating_ship_piece_return_tilt_time_squared_gain_gp * deg_to_rad * frame_time_sq * 3.0f,
                                  floating_ship_piece_return_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                                  frame_time * 1.57079637f);
                func_L00_0025CCF0_30A560(data + 0x64, 0.0f, data + 0x74, 0,
                                  floating_ship_piece_return_tilt_time_squared_gain_gp * deg_to_rad * frame_time_sq * 3.0f,
                                  floating_ship_piece_return_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                                  frame_time * 1.57079637f);
            } else {
                float deg_to_rad;
                deg_to_rad = 0.0174532924f;
                func_L00_0025CCF0_30A560(data + 0x60, 0.0f, data + 0x70, 0,
                                  floating_ship_piece_return_tilt_time_squared_gain_gp * deg_to_rad * frame_time_sq,
                                  floating_ship_piece_return_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                                  floating_ship_piece_return_tilt_snap_tolerance_scale_gp * deg_to_rad * frame_time);
                func_L00_0025CCF0_30A560(data + 0x64, 0.0f, data + 0x74, 0,
                                  floating_ship_piece_return_tilt_time_squared_gain_gp * deg_to_rad * frame_time_sq,
                                  floating_ship_piece_return_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                                  floating_ship_piece_return_tilt_snap_tolerance_scale_gp * deg_to_rad * frame_time);
            }
            if (((Mpos_30A560 *)moby)->z > *(float *)(data + 0x84)) {
                ((Mpos_30A560 *)moby)->z = *(float *)(data + 0x84);
            }
            if (old_z < sink_level) {
                if (sink_level <= ((Mpos_30A560 *)moby)->z) {
                    func_0022ED80_30A560(2, 0, moby);
                }
            }
        }
        update_moby_vertical_oscillation(0.140000001f, frame_time * 0.52359879f, moby, (float *)(data + 0x98), (float *)(data + 0x9C));
        update_moby_rotation_oscillation(moby, data + 0x90, data + 0x94, 0.0872664601f,
                          frame_time * 0.436332315f, frame_time * 0.541052043f);
        *(float *)(moby + 0x40) = func_001FA748_30A560(*(float *)(data + 0x60), *(float *)(moby + 0x40));
        *(float *)(moby + 0x44) = func_001FA748_30A560(*(float *)(data + 0x64), *(float *)(moby + 0x44));
        func_001F9BD8_30A560(movement, movement, moby + 0x10);
        func_L00_002617B0_30A560(data + 0x20, movement, original_rotation, moby + 0x40);
        break;
    }
}
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 node[1];
} PathNodes_00309c08;
typedef struct {
    u8 pad0[0x70];
    u8 unk70[0x38];
    f32 speed;
    s32 path;
    u8 padB0[8];
    s16 dir;
    s16 node;
    f32 yaw_offset;
    f32 spin;
    u8 padC4[8];
    f32 lag;
} PathFlyerVars_00309c08;
extern void attach_manipulator(struct Moby *, s32, void *) __asm__("FUN_0020cb10");
extern void scale_vector_00309c08(f32, void *, void *) __asm__("FUN_001f9a68");
extern s32 check_voice_target_00309c08(void *, s32) __asm__("FUN_L00_0028d8c0");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern u8 D_0013F3D0[];
extern u8 D_0014C050[];
extern s32 D_L09_0015F5CC __attribute__((section(".data")));
extern f32 atan2_f(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int advance_shuttle_along_path(struct Moby *) __asm__("FUN_L09_0030a298");

void shuttle_update(struct Moby *moby) __asm__("FUN_L09_00309c08");
void shuttle_update(struct Moby *moby) {
    enum {
        SHUTTLE_STATE_INITIALIZE = 0,
        SHUTTLE_STATE_WAIT_FOR_DEPARTURE = 1,
        SHUTTLE_STATE_WAIT_FOR_DISMOUNT = 2,
        SHUTTLE_STATE_FOLLOW_PATH = 3
    };
    PathFlyerVars_00309c08 *vars = (PathFlyerVars_00309c08 *)moby->pvars;
    PathNodes_00309c08 *path = (PathNodes_00309c08 *)D_L09_001B0630[vars->path];
    Vec4 *position = &moby->pos;
    Vec4 original;
    Vec4 rotation;
    s16 dir;
    s32 voice;

    scale_vector_00309c08(-1.0f, &original, position);
    qcopy(&rotation, &moby->rot);
    if (vars == 0) return;

    switch (moby->state) {
    case SHUTTLE_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        vars->yaw_offset = 3.1415927f;
        *(s32 *)((u8 *)vars + 0xC4) = -1;
        *(s32 *)((u8 *)vars + 0xC8) = 0;
        vars->lag = 0.0f;
        if (*(s32 *)((u8 *)vars + 0xA0) != 0) {
            PathNodes_00309c08 *path;
            Vec4 *endpoint;
            attach_manipulator(moby, 0, (u8 *)vars + 0x60);
            path = (PathNodes_00309c08 *)D_L09_001B0630[vars->path];
            moby->state = 1;
            if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
                vars->dir = -1;
                vars->node = path->count - 2;
                endpoint = (Vec4 *)((path->count << 4) + (s32)path);
            } else {
                vars->dir = 1;
                endpoint = &path->node[0];
                vars->node = 1;
            }
            *position = *endpoint;
            moby->rot.z = atan2_f(path->node[vars->node].f[0] - moby->pos.x,
                                   path->node[vars->node].f[1] - moby->pos.y);
        }
        moby->rot.z = fast_add_rotations(moby->rot.z, 3.1415927f);
        break;
    case SHUTTLE_STATE_WAIT_FOR_DISMOUNT:
        vars->lag += (1.0f - vars->lag) * 0.01f;
        update_moby_vertical_oscillation(vars->lag * 0.14f, frame_time * 0.5235988f,
                          (char *)moby, (f32 *)((u8 *)vars + 0xD8), (f32 *)((u8 *)vars + 0xDC));
        update_moby_rotation_oscillation(moby, (char *)vars + 0xD0, (char *)vars + 0xD4,
                          vars->lag * 0.08726646f, frame_time * 0.31415927f,
                          frame_time * 0.4886922f);
        if (hero.ground_moby != moby) moby->state = 1;
        break;
    case SHUTTLE_STATE_WAIT_FOR_DEPARTURE:
        vars->lag += (1.0f - vars->lag) * 0.01f;
        update_moby_vertical_oscillation(vars->lag * 0.14f, frame_time * 0.5235988f,
                          (char *)moby, (f32 *)((u8 *)vars + 0xD8), (f32 *)((u8 *)vars + 0xDC));
        update_moby_rotation_oscillation(moby, (char *)vars + 0xD0, (char *)vars + 0xD4,
                          vars->lag * 0.08726646f, frame_time * 0.31415927f,
                          frame_time * 0.4886922f);
        if (*(s32 *)((u8 *)vars + 0xC8) != 0) {
            if ((vars->dir > 0 && *(s32 *)((u8 *)vars + 0xB4) != -1 && is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)vars + 0xB4))) ||
                (vars->dir < 0 && *(s32 *)((u8 *)vars + 0xB0) != -1 && is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)vars + 0xB0)))) {
                moby->state = 3;
            }
        }
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            *(s32 *)((u8 *)vars + 0xC8) = 1;
            moby->state = 3;
        }
        break;
    case SHUTTLE_STATE_FOLLOW_PATH:
        if ((D_L09_0015F5CC & 7) == (((s32)moby >> 8) & 7)) {
            if (*(s32 *)((u8 *)vars + 0xC4) == -1 ||
                (!check_voice_target_00309c08(moby, *(s32 *)((u8 *)vars + 0xC4)) &&
                 *(s32 *)((u8 *)vars + 0xC4) == -1)) {
                *(s32 *)((u8 *)vars + 0xC4) = allocate_voice_for_target_entry(0, 4, moby);
            }
        }
        if (vars->yaw_offset == 0.0f) {
            if (vars->dir < 0) {
                if (*(s32 *)((u8 *)vars + 0xB4) == -1 ||
                    !is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)vars + 0xB4)))
                    goto test_forward;
                if (!(vector_distance(&moby->pos, &path->node[path->count - 1]) > 8.0f) ||
                    !(vector_distance(&moby->pos, &path->node[0]) > 8.0f))
                    goto advance_path;
                vars->dir = 1;
                vars->yaw_offset = 3.1415927f;
            } else {
            test_forward:
                if (vars->dir > 0 && *(s32 *)((u8 *)vars + 0xB0) != -1 &&
                    is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)vars + 0xB0)) &&
                    vector_distance(&moby->pos, &path->node[path->count - 1]) > 8.0f &&
                    vector_distance(&moby->pos, &path->node[0]) > 8.0f) {
                    vars->dir = -1;
                    vars->yaw_offset = 3.1415927f;
                }
            }
        }
    advance_path:
        if (advance_shuttle_along_path(moby)) {
            voice = *(s32 *)((u8 *)vars + 0xC4);
            if (voice != -1) {
                u8 *entry = (u8 *)D_0013E550 + voice * 0x70;
                if (*(struct Moby **)(entry + 0x88) == moby && entry[0x74] != 0)
                    release_voice_slot(voice);
            }
            *(s32 *)((u8 *)vars + 0xC4) = -1;
            vars->lag = 0.0f;
            *(s32 *)((u8 *)vars + 0xDC) = 0;
            moby->state = 2;
            vars->yaw_offset = 3.1415927f;
            dir = vars->dir;
            vars->dir = -dir;
            if ((s16)-dir > 0) vars->node = 0;
            else vars->node = path->count - 1;
        }
        break;
    }
    add_vector_xyz(&original, &original, &moby->pos);
    update_entity_matrix((u8 *)vars + 0x20, &original, &rotation, &moby->rot);
}
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_0030B5E8.c: func_L09_0030B5E8), where it is exact; names translated to the US level program. */

void interpolate_rotation_angle(float *output_angle, float current_angle, float target_angle, float scale) __asm__("FUN_L09_0030a238");

void interpolate_rotation_angle(float *output_angle, float current_angle, float target_angle, float scale) {
    float scaled_angle_delta = fast_subtract_rotations(target_angle, current_angle) * scale;
    *output_angle = scaled_angle_delta;
    *output_angle = fast_add_rotations(scaled_angle_delta, current_angle);
}
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 node[1];
} PathNodes;

typedef struct {
    u8 pad0[0x70];
    u8 unk70[0x38];
    f32 speed;          /* 0xA8 */
    s32 path;           /* 0xAC: index into D_L09_001B0630 */
    u8 padB0[8];
    s16 dir;            /* 0xB8: > 0 runs the path forward, else backward */
    s16 node;           /* 0xBA: node being flown to */
    f32 yaw_offset;     /* 0xBC */
    f32 spin;           /* 0xC0 */
    u8 padC4[8];
    f32 lag;            /* 0xCC: eases to 0, slowing the turn while high */
} PathFlyerVars;

extern f32 D_L09_00161DF8 __attribute__((sda));
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern float atan2_f(float, float) __asm__("FUN_001f9e90");
extern float advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern void FUN_L00_001fff28(void *, int, float);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern void add_random_offset_to_vector_xyz(void *, f32) __asm__("FUN_L00_0025f8e0");
extern void spawn_moving_resizing_height_snapped_alpha_fade_particle(void *, int, int, float, float) __asm__("FUN_L00_002715e8");

/* Flies the moby along its path toward the current node, turning and banking
 * toward it, steps the node at each arrival and now and then drops a puff.
 * Returns nonzero once it has run off the end of the path. */
int advance_shuttle_along_path(struct Moby *moby) {
    PathFlyerVars *vars = (PathFlyerVars *)moby->pvars;
    PathNodes *path;
    f32 endpoint_distance;
    Vec4 target;
    Vec4 step;
    Vec4 puff;
    int done = 0;

    path = (PathNodes *)D_L09_001B0630[vars->path];
    vars->lag += (0.0f - vars->lag) * 0.005f;
    qcopy(&target, &path->node[vars->node]);
    target.f[2] += vars->speed * D_L09_00161DF8;
    subtract_vector_xyz(&step, &target, &moby->pos);
    normalize_vector_xyz(&step, &step, vars->speed);
    add_vector_xyz(&moby->pos, &moby->pos, &step);
    interpolate_rotation_angle(&moby->rot.z, moby->rot.z,
                     fast_add_rotations(atan2_f(path->node[vars->node].f[0] - moby->pos.x,
                                         path->node[vars->node].f[1] - moby->pos.y),
                                 vars->yaw_offset),
                     (1.0f - vars->lag) * 0.02f);
    interpolate_rotation_angle(&moby->rot.x, moby->rot.x, 0.0f, 0.02f);
    interpolate_rotation_angle(&moby->rot.y, moby->rot.y, 0.0f, 0.02f);
    if (vars->dir > 0) {
        f32 v;
        if (FUN_001f9b48(&target, &moby->pos) < vars->speed + vars->speed) {
            vars->node++;
            if (vars->node == path->count)
                done = 1;
        }
        endpoint_distance = FUN_001f9b48(&moby->pos, &path->node[path->count - 1]);
        v = 0.0f;
        advance_accelerated_scalar(endpoint_distance, frame_time_sq * 3.0f, frame_time_sq * 3.0f, frame_time * 10.0f, &v, &vars->speed);
    } else {
        f32 v;
        if (FUN_001f9b48(&target, &moby->pos) < vars->speed + vars->speed) {
            vars->node--;
            if (vars->node == -1)
                done = 1;
        }
        endpoint_distance = FUN_001f9b48(&moby->pos, &path->node[0]);
        v = 0.0f;
        advance_accelerated_scalar(endpoint_distance, frame_time_sq * 3.0f, frame_time_sq * 3.0f, frame_time * 10.0f, &v, &vars->speed);
    }
    if (path->node[vars->node].f[3] == 42.0f)
        vars->yaw_offset = 0.0f;
    vars->spin = fast_add_rotations(vars->spin, vars->speed);
    FUN_L00_001fff28(vars->unk70, 0, vars->spin);
    if (vector_distance(&moby->pos, &path->node[path->count - 1]) > 8.0f &&
        vector_distance(&moby->pos, &path->node[0]) > 8.0f && !random_integer_below(7)) {
        qcopy(&puff, &moby->pos);
        add_random_offset_to_vector_xyz(&puff, 0.5f);
        puff.f[2] = 25.01f;
        spawn_moving_resizing_height_snapped_alpha_fade_particle(&puff, 0, 0x40103080, vars->speed * 3.0f / (frame_time * 10.0f), 12600.0f);
    }
    return done;
}
