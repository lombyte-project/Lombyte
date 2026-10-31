/* Glow particle moby classes 0x70 and 0x4A8: tinted, spinning particles that shrink and fade, and the functions that spawn them. */

#include "types.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"

typedef unsigned int u128_2ac910 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2ac910 q;
    float       f[4];
} Q_2ac910;

typedef struct {
    u128_2ac910 p;
    void       *o;
    int         n;
    float       f18;
    short       h1c;
    short       h1e;
} V_2ac910;

typedef struct {
    char  pad[0x14];
    int   n;
    float f18;
    s16   h1c;
    s16   h1e;
} V_2acad8;

typedef struct {
    char      pad[0x23];
    u8        b23;
    char      pad2[0x2C - 0x24];
    float     f2c;
    char      pad3[0x40 - 0x30];
    float     r[3];
    char      pad4[0x78 - 0x4C];
    V_2acad8 *v;
} M_2acad8;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2df808;

typedef unsigned int U128_2df808 __attribute__((mode(TI), aligned(16)));

typedef union {
    U128_2df808 q;
    float       f[4];
} R_2df808;

void                  set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
int                   FUN_001f96f8_2ac910(int) __asm__("FUN_001f96f8");
void                  update_entity(void *) __asm__("FUN_L00_00250df8");
extern unsigned char *FUN_L00_0024e838_2df808(int) __asm__("FUN_L00_0024e838");
extern float          FUN_L00_00257be8_2df808(float, float) __asm__("FUN_L00_00257be8");

/* Spawns a particle moby with a random rotation, owned by `owner`, at `pos` with velocity `vel`, tinted red/green/blue
   and living `lifetime` frames; returns the new moby or NULL. */
unsigned char *spawn_glow_particle(void *owner,
                                   void *pos,
                                   void *vel,
                                   int   lifetime,
                                   int   red,
                                   int   green,
                                   int   blue,
                                   int   alpha,
                                   float scale) __asm__("FUN_L00_002ac910");

unsigned char *spawn_glow_particle(void *glow_source_moby,
                                   void *spawn_position,
                                   void *initial_velocity,
                                   int   lifetime,
                                   int   red,
                                   int   green,
                                   int   blue,
                                   int   alpha,

                                   float scale) {
    unsigned char *moby;
    red &= 0xFF;
    green &= 0xFF;
    blue &= 0xFF;
    alpha &= 0xFF;
    moby = create_moby(0x70);
    if (moby) {
        Q_2ac910  rot_copy, random_initial_rotation;
        V_2ac910 *glow_particle_data = ((V_2ac910 *)MOBY(moby)->pvars);
        float     elapsed;
        random_initial_rotation.q = 0;
        random_initial_rotation.f[0] = random_float_between(-3.1415927f, 3.1415927f);
        random_initial_rotation.f[1] = random_float_between(-3.1415927f, 3.1415927f);
        random_initial_rotation.f[2] = random_float_between(-3.1415927f, 3.1415927f);
        rot_copy.q = random_initial_rotation.q;
        moby[0x20] = 0;
        moby[0x23] = alpha;
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        set_moby_color(moby, red, green, blue);
        glow_particle_data->f18 = MOBY(moby)->scale * scale;
        *(int *)(moby + 0x2C) = 0;
        glow_particle_data->o = glow_source_moby;
        qcopy(moby + 0x10, spawn_position);
        qcopy(glow_particle_data, initial_velocity);
        qcopy(moby + 0x40, &rot_copy);
        glow_particle_data->n = lifetime;
        glow_particle_data->h1c =
            *(unsigned short *)&glow_particle_data->n - FUN_001f96f8_2ac910(4);
        elapsed = convert_integer_to_float(glow_particle_data->n - glow_particle_data->h1c);
        MOBY(moby)->scale =
            elapsed * glow_particle_data->f18 / convert_integer_to_float(glow_particle_data->n);
        glow_particle_data->h1e = alpha;
        update_entity(moby);
    }
    return moby;
}

void glow_particle_update(M_2acad8 *particle_moby) __asm__("FUN_L00_002acad8");

/* Moby classes 112, 1192 */
void glow_particle_update(M_2acad8 *particle_moby) {
    V_2acad8 *v = particle_moby->v;
    float     elapsed_lifetime_frames, total_lifetime_frames;
    int       n;
    elapsed_lifetime_frames = convert_integer_to_float(v->n - v->h1c);
    total_lifetime_frames = convert_integer_to_float(v->n);
    particle_moby->f2c = elapsed_lifetime_frames * v->f18 / total_lifetime_frames;
    particle_moby->r[0] = fast_add_rotations(particle_moby->r[0], frame_time * 3.1415927f);
    particle_moby->r[1] = fast_add_rotations(particle_moby->r[1], frame_time * 3.1415927f);
    particle_moby->r[2] = fast_add_rotations(particle_moby->r[2], frame_time * 3.1415927f);
    if (fast_dec_timer_s16(&v->h1c)) {
        mark_moby_for_removal(particle_moby);
        return;
    }
    n = v->n / 2;
    if (v->h1c < n) {
        particle_moby->b23 = v->h1c * v->h1e / n;
    }
}

unsigned char *spawn_randomly_rotated_colored_moby(int           owner,
                                                   V_2df808     *position,
                                                   V_2df808     *vector,
                                                   int           flags,
                                                   unsigned char red,
                                                   unsigned char green,
                                                   unsigned char blue,
                                                   unsigned char alpha,
                                                   float         scale) __asm__("FUN_L00_002df808");

unsigned char *spawn_randomly_rotated_colored_moby(int           source_moby_address,
                                                   V_2df808     *position,
                                                   V_2df808     *vector,
                                                   int           flags,
                                                   unsigned char red,
                                                   unsigned char green,
                                                   unsigned char blue,
                                                   unsigned char alpha,

                                                   float scale) {
    unsigned char *moby = FUN_L00_0024e838_2df808(0x4a8);
    unsigned char *spawned_moby_private_data;
    R_2df808       rotation_copy;
    R_2df808       rotation;
    if (moby != 0) {
        spawned_moby_private_data = ((unsigned char *)MOBY(moby)->pvars);
        rotation.q = 0;
        rotation.f[0] = FUN_L00_00257be8_2df808(-3.1415927f, 3.1415927f);
        rotation.f[1] = FUN_L00_00257be8_2df808(-3.1415927f, 3.1415927f);
        rotation.f[2] = FUN_L00_00257be8_2df808(-3.1415927f, 3.1415927f);
        rotation_copy.q = rotation.q;
        moby[0x20] = 0;
        moby[0x23] = alpha;
        moby[0x30] = 0xff;
        MOBY(moby)->draw_dist = 0xff;
        moby[0x31] = 1;
        set_moby_color(moby, red, green, blue);
        *(float *)(spawned_moby_private_data + 0x18) = MOBY(moby)->scale * scale;
        *(int *)(moby + 0x2c) = 0;
        *(int *)(spawned_moby_private_data + 0x10) = source_moby_address;
        qcopy(moby + 0x10, position);
        qcopy(spawned_moby_private_data, vector);
        qcopy(moby + 0x40, &rotation_copy);
        *(unsigned short *)(spawned_moby_private_data + 0x1e) = alpha;
        *(int *)(spawned_moby_private_data + 0x14) = flags;
        *(unsigned short *)(spawned_moby_private_data + 0x1c) =
            *(unsigned short *)(spawned_moby_private_data + 0x14);
        update_entity(moby);
    }
    return moby;
}
