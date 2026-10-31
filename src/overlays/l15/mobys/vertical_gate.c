/* Level 15 vertical gate moby class. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/l15/mobys/vertical_gate_globals.h"

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2A48B0;

typedef struct {
    Vec_2A48B0 home;        /* 0x00: x y z w at open */
    int        vol_near;    /* 0x10 */
    int        vol_trigger; /* 0x14 */
    float      travel;      /* 0x18 */
    int        sound;       /* 0x1C */
    int        key;         /* 0x20 */
    int        trigger;     /* 0x24 */
    int        locked;      /* 0x28 */
} GateVars;

typedef struct {
    char          pad00[0x10];
    Vec_2A48B0    pos;   /* 0x10 */
    unsigned char state; /* 0x20 */
    char          pad21[0xF];
    unsigned char f30; /* 0x30 */
    char          pad31[0x47];
    GateVars     *vars; /* 0x78 */
} GateMoby;

typedef struct {
    char          pad00[0x74];
    unsigned char active; /* 0x74 */
    char          pad75[0x13];
    GateMoby     *owner; /* 0x88 */
} SoundSlot;

extern float D_0015ED6C;

float vertical_gate_travel_distance_gp __asm__("D_L15_001614F8") = 4.25f;

extern int D_L15_00161AC0;

extern unsigned char D_001413F4[];

extern unsigned char D_0013DD50 NOT_SDA;

extern unsigned char D_0013F3D0_2A48B0[] __asm__("D_0013F3D0");

extern char D_L15_00167440_2A48B0[] __asm__("D_L15_001673C0");

extern int l15_key_gate_open_lui __asm__("D_L15_00184AA8") NOT_SDA;

void vertical_gate_update(GateMoby *gate_moby) __asm__("FUN_L15_002a36d0");

/* Moby class 92 */
void vertical_gate_update(GateMoby *gate_moby) {
    enum {
        GATE_INITIALIZE = 0,
        GATE_WAIT_TO_OPEN = 1,
        GATE_OPENING = 2,
        GATE_OPEN = 3,
        GATE_CLOSING = 4,
        GATE_POSITION_OVERRIDE = 5
    };
    GateVars *gate_vars = gate_moby->vars;
    float     step;

    switch (gate_moby->state) {
    case GATE_INITIALIZE:
        *(u128 *)&gate_vars->home = *(u128 *)&gate_moby->pos;
        gate_moby->state = 1;
        gate_vars->locked = 0;
        if (gate_vars->key != -1) {
            gate_moby->f30 = 0xFF;
        }
        if (D_0013DD50 != 0 && gate_vars->key != -1) {
            gate_moby->pos.z = gate_moby->pos.z - vertical_gate_travel_distance_gp;
            gate_moby->state = 5;
        }
        break;
    case GATE_WAIT_TO_OPEN:
        gate_vars->travel = 0;
        if (gate_vars->key != -1) {
            l15_key_gate_open_lui = 0;
        }
        if (gate_vars->trigger != -1) {
            struct Moby *trigger_moby =
                (struct Moby *)((char *)l15_moby_array + (gate_vars->trigger << 8));
            if (trigger_moby == 0 || trigger_moby->state == 0xFE || trigger_moby->state == 0xFD) {
                if (D_L15_00161AC0 == 0) {
                    gate_moby->state = 2;
                    gate_vars->locked = 1;
                }
            }
        } else {
            if (is_point_inside_clip_volume(D_0013F3D0_2A48B0, gate_vars->vol_near)) {
                if (gate_vars->key == -1 || D_0013F3D0_2A48B0[0x2024] == 2) {
                    gate_moby->state = 2;
                }
            }
        }
        if (D_0013DD50 != 0 && gate_vars->key != -1) {
            gate_moby->pos.z = gate_vars->home.z - vertical_gate_travel_distance_gp;
            gate_moby->state = 5;
        }
        break;
    case GATE_OPENING:
        step = (vertical_gate_travel_distance_gp / 0.6f) * D_0015ED6C;
        if (gate_vars->key != -1) {
            step = step * 5.0f;
        }
        if (((SoundSlot *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->owner != gate_moby ||
            ((SoundSlot *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->active == 0) {
            gate_vars->sound = allocate_voice_for_target_entry(0, 4, gate_moby);
        }
        if (vertical_gate_travel_distance_gp < (gate_vars->travel = gate_vars->travel + step)) {
            if (gate_vars->sound != -1 &&
                ((SoundSlot *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->owner ==
                    gate_moby &&
                ((SoundSlot *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->active != 0) {
                release_voice_slot(gate_vars->sound);
            }
            gate_vars->sound = -1;
            gate_vars->travel = vertical_gate_travel_distance_gp;
            gate_moby->state = 3;
        }
        {
            float t = gate_vars->travel;
            float h = gate_vars->home.z;
            float z;
            if (gate_vars->key != -1) {
                t = -t;
                z = h + t;
            } else {
                z = h + t;
            }
            gate_moby->pos.z = z;
        }
        break;
    case GATE_OPEN:
        if (gate_vars->key != -1) {
            l15_key_gate_open_lui = 1;
        }
        if (is_point_inside_clip_volume(D_0013F3D0_2A48B0, gate_vars->vol_trigger) != 0 ||
            is_point_inside_clip_volume(D_L15_00167440_2A48B0, gate_vars->vol_trigger) != 0) {
            if (is_point_inside_clip_volume(D_0013F3D0_2A48B0, gate_vars->key) == 0) {
                break;
            }
        }
        if (gate_vars->locked == 0) {
            gate_moby->state = 4;
        }
        break;
    case GATE_CLOSING:
        step = (vertical_gate_travel_distance_gp / 0.6f) * D_0015ED6C;
        if (gate_vars->key != -1) {
            step = step * 5.0f;
        }
        if (((SoundSlot *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->owner != gate_moby ||
            ((SoundSlot *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->active == 0) {
            gate_vars->sound = allocate_voice_for_target_entry(0, 4, gate_moby);
        }
        if ((gate_vars->travel = gate_vars->travel - step) < 0.0f) {
            if (gate_vars->sound != -1 &&
                ((SoundSlot *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->owner ==
                    gate_moby &&
                ((SoundSlot *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->active != 0) {
                release_voice_slot(gate_vars->sound);
            }
            gate_vars->travel = 0;
            gate_vars->sound = -1;
            gate_moby->state = 1;
            if (gate_vars->key != -1) {
                gate_moby->state = 5;
                l15_key_gate_open_lui = 0;
            }
        }
        {
            float t = gate_vars->travel;
            float h = gate_vars->home.z;
            float z;
            if (gate_vars->key != -1) {
                t = -t;
                z = h + t;
            } else {
                z = h + t;
            }
            gate_moby->pos.z = z;
        }
        if (is_point_inside_clip_volume(D_0013F3D0_2A48B0, gate_vars->vol_near) != 0 ||
            is_point_inside_clip_volume(D_L15_00167440_2A48B0, gate_vars->vol_near) != 0) {
            gate_moby->state = 2;
        }
        break;
    case GATE_POSITION_OVERRIDE:
        if (D_0013DD50 != 0) {
            gate_moby->pos.z = gate_vars->home.z - vertical_gate_travel_distance_gp;
            l15_key_gate_open_lui = 1;
        } else {
            if (D_001413F4[0] == 2) {
                break;
            }
            gate_moby->pos.z = gate_vars->home.z;
            gate_moby->state = 1;
            l15_key_gate_open_lui = 0;
        }
        break;
    }
}

extern u8 D_001413F4[];

extern u8 D_0013DD50;
