/* Level 09 moby class 0x49d: airship chain link. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/quad.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"

#define F(p, o) (*(float *)((p) + (o)))

#include "rnc/mobys/moby.h"

extern struct Moby *moby_list __asm__("D_L09_0015FFD8");

/* Pressure plate: waits for its trigger slot, hums while armed, wakes its linked moby when stepped on, then shatters. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_003050C0), where it is exact; names translated to the US level program. */
extern char  D_0013D388[];
extern float D_L09_00166F40[];
extern int   D_L09_0015F5CC;
extern void  spawn_explosion(void *,
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

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00305580), where it is exact; names translated to the US level program. */
extern char *D_L09_0015FFE4;
char        *find_nearby_chain_attachment(char *self,
                                          char *search_position,
                                          int   bone) __asm__("FUN_L09_003041d0");
void         FUN_L00_0025e450(f32   burst,
                              f32   unused_float,
                              f32   owner_effect,
                              f32   owner_effect2,
                              f32   cutoff,
                              f32   scale,
                              f32   flash,
                              void *owner,
                              s32   unused2,
                              void *pos,
                              s32   puff_count,
                              s32   spark_count,
                              s32   shard_count,
                              s32   sound,
                              s32   shake,
                              s32   spray_count,
                              s32   tint,
                              u8    alternate);

extern s32 D_L09_0015F5CC NOT_SDA;

char *find_nearby_chain_attachment(char *excluded_moby, char *search_position, int bone) {
    char *best = 0;
    float bestd = 1000000000.0f;
    char  query_position[16];
    char  candidate_joint_position[16];
    char *candidate_moby;
    char *tp = query_position;
    *(OvlQuad *)query_position = *(OvlQuad *)search_position;
    for (candidate_moby = D_L09_0015FFE4; candidate_moby != 0;
         candidate_moby = *(char **)(candidate_moby + 0x28)) {
        if (candidate_moby == excluded_moby)
            continue;
        if (candidate_moby != 0 && *(unsigned char *)(candidate_moby + 0x20) != 0xFE &&
            *(unsigned char *)(candidate_moby + 0x20) != 0xFD) {
            if (*(short *)(candidate_moby + 0xA6) == 0x49D ||
                *(short *)(candidate_moby + 0xA6) == 0x494 ||
                *(short *)(candidate_moby + 0xA6) == 0x4A0) {
                float candidate_distance;
                if (bone != -1) {
                    moby_point_position(candidate_moby, bone, candidate_joint_position);
                    candidate_distance = fast_vec_dist(tp, candidate_joint_position);
                } else {
                    candidate_distance = fast_vec_dist(tp, candidate_moby + 0x10);
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

#include "qcopy.h"

/* Airship chain link (class 1181) update: finds the parts it hangs between, swings between them, and on its trigger drops, wakes its neighbours and bursts. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00305710), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef int u128_305710 __attribute__((mode(TI)));

typedef struct {
    char          pad0[0x60];
    int           hull;    /* 0x60: moby index of the part it hangs from, -1 if none */
    int           next;    /* 0x64: moby index of the next part, -1 if none */
    unsigned char loose;   /* 0x68 */
    signed char   flag;    /* 0x69 */
    short         timer;   /* 0x6A */
    int           trigger; /* 0x6C */
} Data;

typedef struct {
    char          pad0[0x39];
    unsigned char b39;
} Flags;

extern Moby *moby_list __asm__("D_L09_0015FFD8");
extern char  D_L09_00209050[];
extern char  D_L09_00209080[];
extern int   stub_printf() __asm__("FUN_001e93b0");
extern void  FUN_L00_0024f7c8(void *, int, void *);
void         FUN_L00_0025e450(f32   burst,
                              f32   unused_float,
                              f32   owner_effect,
                              f32   owner_effect2,
                              f32   cutoff,
                              f32   scale,
                              f32   flash,
                              void *owner,
                              s32   unused2,
                              void *pos,
                              s32   puff_count,
                              s32   spark_count,
                              s32   shard_count,
                              s32   sound,
                              s32   shake,
                              s32   spray_count,
                              s32   tint,
                              u8    alternate);
extern void  spawn_explosion(void *,
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

void airship_chain_link_update(Moby *m) __asm__("FUN_L09_00304360");

/* Moby class 1181 */
void airship_chain_link_update(Moby *m) {
    Data *d = (*(Data **)&m->pvars);
    float v20[4];
    float v30[4];
    float v40[4];
    Moby *hull;
    Moby *next;
    Moby *r;
    float dist;
    int   n;
    int   t;

    switch (m->state) {
    case 0:
        if (d->flag != -1 && ((Flags *)(D_0013D388 + d->flag))->b39 != 0) {
            mark_moby_for_removal(m);
            return;
        }
        if (d->hull == -1) {
            FUN_L00_0024f7c8(m, 0, v20);
            r = find_nearby_chain_attachment(m, v20, -1);
            if (r != 0) {
                d->hull = r - moby_list;
            } else {
                stub_printf(D_L09_00209050, (*(short *)&m->save_id));
            }
        }
        if (d->next == -1) {
            *(u128_305710 *)v20 = *(u128_305710 *)((f32 *)&m->pos);
            r = find_nearby_chain_attachment(m, v20, 0);
            if (r != 0) {
                d->next = r - moby_list;
            } else {
                stub_printf(D_L09_00209080, (*(short *)&m->save_id));
            }
        }
        m->state = 1;
        break;
    case 1:
        if (m->visible == 0 || fast_vec_dist(((f32 *)&m->pos), D_L09_00166F40) > 32.0f) {
            if (D_L09_0015F5CC % 8 != (short)((*(short *)&m->save_id) % 8)) {
                return;
            }
        }
        hull = 0;
        next = 0;
        if (d->hull != -1) {
            hull = &moby_list[d->hull];
            if (hull == 0 || hull->state == 0xFE || hull->state == 0xFD) {
                hull = 0;
            }
        }
        if (d->next != -1) {
            next = &moby_list[d->next];
            if (next == 0 || next->state == 0xFE || next->state == 0xFD) {
                d->loose = 0;
                next = 0;
            } else if ((*(Data **)&next->pvars)->loose == 0) {
                d->loose = 0;
            }
        }
        if (d->loose == 0 && hull != 0 && next != 0 && next->oclass == 0x49D) {
            if ((*(Data **)&next->pvars)->loose != 0) {
                dist = vector_distance_xy(((f32 *)&m->pos), ((f32 *)&hull->pos));
                m->rot.y = fast_add_rotations(
                    m->rot.y,
                    fast_subtract_rotations(-fast_arc_tan(dist, hull->pos.z - m->pos.z), m->rot.y) *
                        0.1f);
                m->rot.z = fast_add_rotations(
                    m->rot.z,
                    fast_subtract_rotations(
                        fast_arc_tan(hull->pos.x - m->pos.x, hull->pos.y - m->pos.y), m->rot.z) *
                        0.1f);
            } else {
                fast_vec_sub(v20, ((f32 *)&hull->pos), ((f32 *)&m->pos));
                fast_vec_normalize(v20, v20, (fast_vec_length(v20) - 1.3f) * 0.7f);
                if (next->oclass == 0x49D && next->state != 0xFE && next->state != 0xFD) {
                    fast_vec_sub(v30, ((f32 *)&next->pos), ((f32 *)&m->pos));
                    fast_vec_normalize(v30, v30, (fast_vec_length(v30) - 1.3f) * 0.7f);
                    fast_vec_add(v40, v20, v30);
                } else {
                    qcopy(v40, v20);
                }
                v40[2] -= frame_time_sq * 9.8f;
                fast_vec_add(((f32 *)&m->pos), ((f32 *)&m->pos), v40);
                dist = vector_distance_xy(((f32 *)&m->pos), ((f32 *)&hull->pos));
                m->rot.y = fast_add_rotations(
                    m->rot.y,
                    fast_subtract_rotations(-fast_arc_tan(dist, hull->pos.z - m->pos.z), m->rot.y) *
                        0.1f);
                m->rot.z = fast_add_rotations(
                    m->rot.z,
                    fast_subtract_rotations(
                        fast_arc_tan(hull->pos.x - m->pos.x, hull->pos.y - m->pos.y), m->rot.z) *
                        0.1f);
            }
        } else {
            d->loose = 1;
        }
        if (m->command == 1) {
            if (d->trigger != -1) {
                Moby *o = &moby_list[d->trigger];
                if (o->state == 1) {
                    o->command = o->state;
                }
            }
            d->timer = scale_game_frames(10);
            m->update_dist = 0xFF;
            m->state = 2;
        }
        break;
    case 2:
        m->update_dist = 0xFF;
        hull = 0;
        if (d->hull != -1) {
            hull = &moby_list[d->hull];
            if (hull == 0 || hull->state == 0xFE || hull->state == 0xFD) {
                hull = 0;
            }
        }
        if (hull != 0) {
            fast_vec_sub(v20, ((f32 *)&hull->pos), ((f32 *)&m->pos));
            fast_vec_normalize(v20, v20, fast_vec_length(v20) - 1.3f);
            qcopy(v30, v20);
            n = scale_game_frames(10);
            t = d->timer - 3;
            v30[2] -= frame_time_sq * 9.8f * 2.0f * convert_integer_to_float(n - t);
            fast_vec_add(((f32 *)&m->pos), ((f32 *)&m->pos), v30);
            dist = vector_distance_xy(((f32 *)&m->pos), ((f32 *)&hull->pos));
            m->rot.y = fast_add_rotations(
                m->rot.y,
                fast_subtract_rotations(-fast_arc_tan(dist, hull->pos.z - m->pos.z), m->rot.y) *
                    0.5f);
            m->rot.z = fast_add_rotations(
                m->rot.z,
                fast_subtract_rotations(
                    fast_arc_tan(hull->pos.x - m->pos.x, hull->pos.y - m->pos.y), m->rot.z) *
                    0.5f);
        }
        if (fast_dec_timer_s16(&d->timer)) {
            if (d->hull != -1) {
                Moby *o = &moby_list[d->hull];
                if (o != 0 && o->state != 0xFE && o->state != 0xFD &&
                    (o->oclass == 0x494 || o->oclass == 0x49D || o->oclass == 0x4A0)) {
                    o->update_dist = 0xFF;
                    o->command = 1;
                }
            }
            if (d->next != -1) {
                Moby *o = &moby_list[d->next];
                if (o != 0 && o->state != 0xFE && o->state != 0xFD &&
                    (o->oclass == 0x494 || o->oclass == 0x49D || o->oclass == 0x4A0)) {
                    o->update_dist = 0xFF;
                    o->command = 1;
                }
            }
            m->state = 3;
        }
        break;
    case 3:
        fast_vec_sub(v20, D_L09_00166F40, ((f32 *)&m->pos));
        spawn_explosion(
            m, v20, 0, 0.0f, 0.0f, 10, 5, 8, 2.0f, 1.0f, 10.0f, 1.0f, 0, 10.0f, 0, 1, -1, 0);
        mark_moby_for_removal(m);
        break;
    }
}
#else
extern void print_missing_link(char *, short) __asm__("FUN_001e93b0");
extern void effect_00304360(f32,
                            f32,
                            f32,
                            f32,
                            f32,
                            f32,
                            f32,
                            void *,
                            void *,
                            int,
                            int,
                            int,
                            int,
                            int,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

void airship_chain_link_update(unsigned char *moby) __asm__("FUN_L09_00304360");

/* Moby class 1181 */
void airship_chain_link_update(unsigned char *moby) {
    unsigned char *data = *(unsigned char **)(moby + 0x78);
    unsigned char *link;
    unsigned char *link2;
    float          work[4], work2[4], work3[4];
    float          angle;
    int            index;
    int            tick;
    short          oclass;

    switch ((int)moby[0x20]) {
    case 0: {
        unsigned char *link;
        if ((signed char)data[0x69] != -1 &&
            ((unsigned char *)D_0013D388 + (signed char)data[0x69])[0x39] != 0) {
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
    case 1: {
        float rotation_scale;
        float height;
        if (moby[0x31] == 0 || fast_vec_dist(moby + 0x10, D_L09_00166F40) > 32.0f) {
            int a = *(short *)(moby + 0xb2);
            int b = D_L09_0015F5CC;
            if (b % 8 != (short)(a % 8))
                return;
        }
        link = 0;
        link2 = 0;
        index = *(int *)(data + 0x60);
        if (index != -1) {
            link = (unsigned char *)((char *)moby_list + (index << 8));
            if (link == 0 || link[0x20] == 0xfe || link[0x20] == 0xfd)
                link = 0;
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
                    angle = vector_distance_xy(moby + 0x10, link + 0x10);
                    height = F(link, 0x18);
                    rotation_scale = 0.1f;
                } else {
                    fast_vec_sub(work, link + 0x10, moby + 0x10);
                    fast_vec_normalize((fast_vec_length(work) - 1.3f) * 0.7f, work, work);
                    if (*(short *)(link2 + 0xa6) == 0x49d && link2[0x20] != 0xfe &&
                        link2[0x20] != 0xfd) {
                        fast_vec_sub(work2, link2 + 0x10, moby + 0x10);
                        fast_vec_normalize((fast_vec_length(work2) - 1.3f) * 0.7f, work2, work2);
                        fast_vec_add(work3, work, work2);
                    } else {
                        qcopy(work3, work);
                    }
                    work3[2] = work3[2] - frame_time_sq * 9.8f;
                    rotation_scale = 0.1f;
                    fast_vec_add(moby + 0x10, moby + 0x10, work3);
                    angle = vector_distance_xy(moby + 0x10, link + 0x10);
                    height = F(link, 0x18);
                }
                angle = fast_arc_tan(angle, height - F(moby, 0x18));
                F(moby, 0x44) = fast_add_rotations(
                    F(moby, 0x44), fast_subtract_rotations(-angle, F(moby, 0x44)) * rotation_scale);
                angle = fast_arc_tan(F(link, 0x10) - F(moby, 0x10), F(link, 0x14) - F(moby, 0x14));
                F(moby, 0x48) = fast_add_rotations(
                    F(moby, 0x48), fast_subtract_rotations(angle, F(moby, 0x48)) * rotation_scale);
            } else {
                data[0x68] = 1;
            }
        } else {
            data[0x68] = 1;
        }
        if (moby[0xbc] != 1)
            return;
        index = *(int *)(data + 0x6c);
        if (index != -1) {
            unsigned char *link2 = (unsigned char *)((char *)moby_list + (index << 8));
            if (link2[0x20] == 1)
                link2[0xbc] = 1;
        }
        *(short *)(data + 0x6a) = scale_game_frames(10);
        moby[0x30] = 0xff;
        moby[0x20] = 2;
        return;
    }
    case 2: {
        unsigned char *link;
        link = 0;
        moby[0x30] = 0xff;
        index = *(int *)(data + 0x60);
        if (index != -1) {
            link = (unsigned char *)((char *)moby_list + (index << 8));
            if (link == 0 || link[0x20] == 0xfe || link[0x20] == 0xfd)
                link = 0;
        }
        if (link != 0) {
            fast_vec_sub(work, link + 0x10, moby + 0x10);
            fast_vec_normalize(fast_vec_length(work) - 1.3f, work, work);
            qcopy(work2, work);
            tick = scale_game_frames(10);
            work2[2] -= (frame_time_sq * 9.8f + frame_time_sq * 9.8f) *
                        convert_integer_to_float(tick - (*(short *)(data + 0x6a) - 3));
            fast_vec_add(moby + 0x10, moby + 0x10, work2);
            angle = vector_distance_xy(moby + 0x10, link + 0x10);
            angle = fast_arc_tan(angle, F(link, 0x18) - F(moby, 0x18));
            F(moby, 0x44) = fast_add_rotations(
                F(moby, 0x44), fast_subtract_rotations(-angle, F(moby, 0x44)) * 0.5f);
            angle = fast_arc_tan(F(link, 0x10) - F(moby, 0x10), F(link, 0x14) - F(moby, 0x14));
            F(moby, 0x48) = fast_add_rotations(
                F(moby, 0x48), fast_subtract_rotations(angle, F(moby, 0x48)) * 0.5f);
        }
        if (fast_dec_timer_s16(data + 0x6a) == 0)
            return;
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
    case 3: {
        fast_vec_sub(work, D_L09_00166F40, moby + 0x10);
        effect_00304360(
            0.0f, 0.0f, 2.0f, 1.0f, 10.0f, 1.0f, 10.0f, moby, work, 0, 10, 5, 8, 0, 0, 1, -1, 0);
        mark_moby_for_removal(moby);
    }
    }
}
#endif
