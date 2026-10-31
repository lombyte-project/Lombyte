/* Level 08 moby class 0xf8: bomb. */
#include "types.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/math/random.h"

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */
#include "qcopy.h"

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern int  D_L08_00174658_lui __asm__("D_L08_00174658") NOT_SDA;

/* Bomb (class 248) update: fuse, bounce and blast state machine on the moby's state byte */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D36F8), where it is exact; names translated to the US level program. */
typedef int v128_36F8 __attribute__((mode(TI)));

typedef struct {
    char b[16];
} blk16;

struct Probe_36F8 {
    float v[4];
    void *moby;
    int   flags;
    int   pad;
    float f;
    int   st;
};

struct Ray_36F8 {
    float             start[4];
    float             end[4];
    struct Probe_36F8 x;
};

union Coll_36F8 {
    struct Ray_36F8   r;
    struct Probe_36F8 p;
};

#include "rnc/overlay/collision.h"

extern CollisionHit     D_L08_00174640;
extern blk16            l08_bomb_moving_particle_initial_colors __asm__("D_L08_00161868");
extern float D_0015ED6C MACRO_ADDR;
extern float            D_0015ED70_gp __asm__("D_0015ED70") MACRO_ADDR;
extern float            D_L08_00174660[];
extern int              collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int              coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
float      bomb_downward_acceleration_gp __asm__("D_L08_00161860") = 15.0f;
extern void FUN_L00_001ff660_c(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void jet_animate(void *, float) __asm__("FUN_L00_00259fe8");
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
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
extern void
FUN_L00_00269958_c(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern float D_L08_0015F580_macro[] __asm__("D_L08_0015F580") MACRO_ADDR;
void         moby_248_update(char *moby) __asm__("FUN_L08_002d23f0");
extern float D_0015ED6C;

blk16 l08_bomb_moving_particle_initial_colors __asm__("D_L08_00161868") = {
    {-1, -1, -1, 0x2f, -1, -1, 0, 0x2f, -1, 0x7f, 0, 0x2f, -1, 0x4f, 0, 0x2f}
};

/* Moby class 248 */
void moby_248_update(char *moby) {
    enum {
        BOMB_STATE_MOVING = 1,
        BOMB_STATE_STATIONARY_FUSE = 2,
        BOMB_STATE_EXPLODE_AND_REMOVE = 0x62,
        BOMB_STATE_BURST_AND_REMOVE = 0x63
    };
    union Coll_36F8 c;
    blk16           table_or_bounce_scratch;
    float           normalized_direction[4];
    float           random_direction[4];
    char           *bomb_vars;
    int             st;
    int             two;
    float           collision_z_offset;

    bomb_vars = ((char *)MOBY(moby)->pvars);
    if (MOBY(moby)->state != 0x37) {
        if (coll_sphere(0.5f, moby + 0x10, 0x11, moby)) {
            int sphere_hit_moby = D_L08_00174658_lui;
            if (sphere_hit_moby != 0 && (sphere_hit_moby != *(int *)(bomb_vars + 0x30) ||
                                         *(int *)(bomb_vars + 0x24) != 0)) {
                if (!(MOBY(moby)->command & 2))
                    MOBY(moby)->state = 0x62;
                else
                    MOBY(moby)->state = 0x63;
                qcopy((moby + 0x10), ((char *)D_L08_00174660));
                FUN_L00_001ff660_c(
                    bomb_vars + 0x10, bomb_vars + 0x10, ((char *)D_L08_00174660) + 0x20);
                fast_vec_normalize(bomb_vars + 0x10, bomb_vars + 0x10, D_0015ED6C + D_0015ED6C);
            }
        }
    }

    st = MOBY(moby)->state;
    two = 2;
    switch (st) {
    case BOMB_STATE_MOVING: {
        float velocity_magnitude;
        velocity_magnitude = fast_vec_length((bomb_vars + 0x10));
        MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, velocity_magnitude * 0.5f);
        if (*(int *)(bomb_vars + 0x24) == 0) {
            float lo = -1.0f;
            float hi = 1.0f;
            int  *selected_table_entry;
            table_or_bounce_scratch = l08_bomb_moving_particle_initial_colors;
            *(v128_36F8 *)random_direction = 0;
            random_direction[0] = random_float_between(lo, hi);
            random_direction[1] = random_float_between(lo, hi);
            random_direction[2] = random_float_between(lo, hi);
            *(v128_36F8 *)normalized_direction = *(v128_36F8 *)random_direction;
            fast_vec_normalize(normalized_direction, normalized_direction, D_0015ED6C);
            selected_table_entry = (int *)table_or_bounce_scratch.b + random_integer_below(4);
            FUN_L00_00269958_c((moby + 0x10),
                               normalized_direction,
                               *selected_table_entry,
                               0x4FFF,
                               scale_game_frames(0x2D),
                               0x32,
                               0x78,
                               1);
        }
        qcopy(c.r.start, moby + 0x10);
        collision_z_offset = 0.16f;
        c.r.start[2] -= collision_z_offset;
        fast_vec_add((moby + 0x10), (moby + 0x10), (bomb_vars + 0x10));
        *(float *)(bomb_vars + 0x18) -= bomb_downward_acceleration_gp * D_0015ED70_gp;
        qcopy(c.r.end, moby + 0x10);
        c.r.end[2] -= collision_z_offset;
        c.r.x.moby = moby;
        c.r.x.flags = 0x810000;
        c.r.x.st = st;
        c.r.x.f = 1.0f;
        qcopy(c.r.x.v, bomb_vars + 0x10);
        if (collision_line(c.r.start, c.r.end, 0, moby, 0)) {
            char *collision_result = (char *)&D_L08_00174640;
            int   v = *(int *)(collision_result + 0x18);
            if (v != 0) {
                if (v != *(int *)(bomb_vars + 0x30) || *(int *)(bomb_vars + 0x24) != 0) {
                    if (!(MOBY(moby)->command & 2))
                        MOBY(moby)->state = 0x62;
                    else
                        MOBY(moby)->state = 0x63;
                    qcopy((moby + 0x10), ((char *)D_L08_00174660));
                    {
                        char *v = bomb_vars + 0x10;
                        FUN_L00_001ff660_c(v, v, ((char *)D_L08_00174660) + 0x20);
                        fast_vec_normalize(v, v, D_0015ED6C + D_0015ED6C);
                    }
                }
            } else if (*(int *)(collision_result + 0x1C) > 0) {
                if (MOBY(moby)->command & 1) {
                    float k;
                    allocate_voice_for_target_entry(1, 0, moby);
                    qcopy((moby + 0x10), collision_result + 0x20);
                    MOBY(moby)->pos.z += collision_z_offset;
                    FUN_L00_001ff660_c(
                        (bomb_vars + 0x10), (bomb_vars + 0x10), collision_result + 0x40);
                    fast_vec_cross(
                        &table_or_bounce_scratch, (bomb_vars + 0x10), collision_result + 0x40);
                    k = D_0015ED6C * 1.5f;
                    fast_vec_normalize(&table_or_bounce_scratch,
                                       &table_or_bounce_scratch,
                                       random_float_between(-k, k));
                    fast_vec_add((bomb_vars + 0x10), (bomb_vars + 0x10), &table_or_bounce_scratch);
                    fast_vec_scale((bomb_vars + 0x10), (bomb_vars + 0x10), 0.4f);
                    *(int *)(bomb_vars + 0x24) += 1;
                    *(int *)(bomb_vars + 0x20) = scale_game_frames(0xB4);
                    if (fast_vec_length((bomb_vars + 0x10)) < D_0015ED6C) {
                        int r;
                        MOBY(moby)->state = two;
                        r = scale_game_frames(0x78);
                        *(int *)(bomb_vars + 0x20) = r;
                        *(float *)(bomb_vars + 0x28) = 254.0f / (float)r;
                        clear_u64_value((bomb_vars + 0x10));
                    }
                } else {
                    MOBY(moby)->state = 0x62;
                    qcopy((moby + 0x10), collision_result + 0x20);
                    FUN_L00_001ff660_c(
                        (bomb_vars + 0x10), (bomb_vars + 0x10), collision_result + 0x40);
                    fast_vec_normalize(
                        (bomb_vars + 0x10), (bomb_vars + 0x10), D_0015ED6C + D_0015ED6C);
                }
            }
        } else if (fast_dec_timer(bomb_vars + 0x20)) {
            if (MOBY(moby)->command & 2) {
                MOBY(moby)->state = 0x63;
            } else {
                MOBY(moby)->state = 0x62;
            }
        }
        break;
    }
    case BOMB_STATE_STATIONARY_FUSE:
        if (fast_dec_timer(bomb_vars + 0x20)) {
            MOBY(moby)->state = 0x63;
        } else if (*(int *)(bomb_vars + 0x20) == scale_game_frames(0x3C) ||
                   *(int *)(bomb_vars + 0x20) == scale_game_frames(0x28) ||
                   *(int *)(bomb_vars + 0x20) == scale_game_frames(0x14)) {
            *(unsigned char *)(bomb_vars + 7) = 0xFA;
            trooper_anim_update(moby, (short *)bomb_vars);
        }
        trooper_anim_finish(moby, bomb_vars);
        break;
    case BOMB_STATE_EXPLODE_AND_REMOVE:
        allocate_voice_for_target_entry(0, 0, moby);
        c.p.moby = moby;
        c.p.flags = 0x810001;
        c.p.f = 1.0f;
        clear_u64_value(&c.p);
        c.p.st = 0;
        collect_mobies_in_sphere(moby + 0x10, 1.0f, 0x10, moby, &c.p);
        spawn_explosion(moby,
                        D_L08_0015F580_macro,
                        0,
                        0.0f,
                        0.0f,
                        7,
                        10,
                        20,
                        3.0f,
                        1.7f,
                        4.0f,
                        1.0f,
                        -1,
                        7.0f,
                        0,
                        7,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        return;
    case BOMB_STATE_BURST_AND_REMOVE: {
        char *p;
        allocate_voice_for_target_entry(0, 0, moby);
        p = moby + 0x10;
        c.p.moby = moby;
        c.p.flags = 0x810001;
        c.p.f = 1.0f;
        clear_u64_value(&c.p);
        c.p.st = 0;
        collect_mobies_in_sphere(p, 1.0f, 0x10, moby, &c.p);
        cart_burst(moby, p, -1, 0.5f, 13.0f);
        mark_moby_for_removal(moby);
        return;
    }
    }
    jet_animate(moby, 0.2f);
}
