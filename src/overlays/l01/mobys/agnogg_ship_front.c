/* Level 01 moby class agnogg_ship_front. */
#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/save/memcard_save_data.h"

#include "rnc/overlay/level_save_state.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/game/game_globals.h"

typedef struct {
    u8  pad0[0xA6];
    s16 type;
} Moby;

typedef struct {
    u8    pad0[0x44];
    s16   count;
    u8    pad46[0x178 - 0x46];
    Moby *list[1];
} MobyList;

typedef struct {
    u8  b0;
    u8  active;
    u8  pad2[0x1E];
    f32 x;
    f32 y;
    f32 z;
} Effect;

extern Effect D_L01_0017C8C0;
extern void   attach_manipulator(void *, int, void *) __asm__("FUN_0020cb10");

extern MobyList fx_cutscene __asm__("D_L01_0016CCE0");
void            apply_cutscene_big_head_cheat(void) __asm__("FUN_L01_002fac80");

void apply_cutscene_big_head_cheat(void) {
    s32 i;

    if (cutscene_mode == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < fx_cutscene.count; i++) {
            if (fx_cutscene.list[i]->type == 0x32B && D_L01_0017C8C0.active == 0) {
                attach_manipulator(fx_cutscene.list[i], 0, &D_L01_0017C8C0);
                D_L01_0017C8C0.z = D_L01_0017C8C0.y = D_L01_0017C8C0.x = 2.75f;
            }
        }
    }
}

/* Agnogg ship update (moby classes 730, 731, 790 on level 01): state machine on moby->state. */
/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002FABE8.c: func_L01_002FC140), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} V4;

typedef struct {
    int pad0[4];
    int f10;
} Class_2FC140;

typedef struct {
    /* 0x00 */ char pad0[0x10];
    /* 0x10 */ V4   v;
    /* 0x20 */ int  pad20;
    /* 0x24 */ int  rec;
    /* 0x28 */ int  pad28;
    /* 0x2C */ int  timer;
} Link;

typedef struct {
    /* 0x00 */ char           pad0[0x10];
    /* 0x10 */ V4             pos;
    /* 0x20 */ unsigned char  state;
    /* 0x21 */ char           pad21[3];
    /* 0x24 */ Class_2FC140  *pclass;
    /* 0x28 */ char           pad28[8];
    /* 0x30 */ unsigned char  f30;
    /* 0x31 */ char           pad31;
    /* 0x32 */ unsigned short f32;
    /* 0x34 */ unsigned short flags;
    /* 0x36 */ char           pad36[0x1C];
    /* 0x52 */ unsigned char  f52;
    /* 0x53 */ unsigned char  f53;
    /* 0x54 */ char           pad54[0x1C];
    /* 0x70 */ unsigned char  f70;
    /* 0x71 */ char           pad71[7];
    /* 0x78 */ void          *data;
    /* 0x7C */ char           pad7C[0x18];
    /* 0x94 */ int            f94;
    /* 0x98 */ char           pad98[0xE];
    /* 0xA6 */ short          oclass;
    /* 0xA8 */ char           padA8[8];
    /* 0xB0 */ unsigned char  fB0;
    /* 0xB1 */ char           padB1;
    /* 0xB2 */ short          fB2;
    /* 0xB4 */ char           padB4[8];
    /* 0xBC */ unsigned char  fBC;
    /* 0xBD */ char           padBD[0x43];
} Moby_2FC140;

typedef struct {
    /* 0x00 */ float f0;
    /* 0x04 */ int   link;
    /* 0x08 */ short timer;
    /* 0x0A */ short fA;
    /* 0x0C */ float fC;
    /* 0x10 */ int   f10;
    /* 0x14 */ int   list[7];
    /* 0x30 */ int   rec;
    /* 0x34 */ int   f34;
    /* 0x38 */ int   f38;
    /* 0x3C */ int   f3C;
} Data;

#include "rnc/overlay/cuboid.h"

typedef struct {
    unsigned char pad0[0x454];
    unsigned char collected[1];
} Lv;

extern Moby_2FC140 *ents_macro __asm__("D_L01_0015FFD8") MACRO_ADDR;
extern LevelCuboid *l01_cuboids_macro __asm__("D_L01_001600EC") MACRO_ADDR;
extern int          cutscene_mode_macro __asm__("D_L01_0015F5C4") MACRO_ADDR;
extern char         D_L01_001BAD60_2FC140[] __asm__("D_L01_001BAA50");
extern char         D_L01_001BAAA0[] __asm__("D_L01_001BAAA0");
extern void         apply_cutscene_big_head_cheat(void) __asm__("FUN_L01_002fac80");
extern void         blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void         start_scene(int) __asm__("FUN_L00_00298840");
extern void         deactivate_recorded_moby(void) __asm__("FUN_L00_0028e990");
extern int          is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void         set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");
extern void         queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void         unlock_planet(int) __asm__("FUN_L00_002607d0");
extern void         queue_planet_unlock_message(int) __asm__("FUN_L00_00262d38");
extern void record_moby_context_bytes(void *, int, void *, int, void *) __asm__("FUN_L01_0029b0a0");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void show_tracked_moby(void) __asm__("FUN_L01_002a2480");

extern unsigned char D_L01_0015FC88[] MACRO_ADDR;
extern f32           frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern s32           current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
void                 agnogg_ship_front_update(Moby_2FC140 *moby) __asm__("FUN_L01_002fad68");

/* Moby classes 730, 731, 790 */
void agnogg_ship_front_update(Moby_2FC140 *moby) {
    enum {
        AGNOGG_SHIP_INITIALIZE = 0,
        AGNOGG_SHIP_WAIT_FOR_DESCENT_TRIGGER = 1,
        AGNOGG_SHIP_DESCEND_TO_TARGET_HEIGHT = 2,
        AGNOGG_SHIP_WAIT_FOR_ANIMATION_1_GATE = 3,
        AGNOGG_SHIP_WAIT_FOR_LINKED_MOBIES_INACTIVE = 4,
        AGNOGG_SHIP_WAIT_BEFORE_SCENE = 5,
        AGNOGG_SHIP_WAIT_FOR_SCENE_3_END = 6,
        AGNOGG_SHIP_WAIT_TO_START_SCENE_4 = 7,
        AGNOGG_SHIP_WAIT_FOR_SCENE_4_END = 8,
        AGNOGG_SHIP_HOLD_TARGET_HEIGHT = 10
    };
    Data *data;
    Link *linked_moby_data;
    int   level_table_byte;
    int   inactive_or_absent_link_count;
    int   flag;
    int   i;
    int   scene_start_gate_value;
    float fz;
    float fd;

    data = moby->data;
    apply_cutscene_big_head_cheat();
    fast_dec_timer_s16(&data->timer);
    switch (moby->state) {
    case AGNOGG_SHIP_INITIALIZE:
        fz = moby->pos.z - 0.75f;
        moby->f30 = 0xFF;
        moby->state = 1;
        data->timer = 0;
        fd = data->f0;
        data->fC = fz;
        moby->f94 = 0;
        moby->flags = moby->flags | 0x41;
        moby->pos.z = moby->pos.z + fd;
        break;
    case AGNOGG_SHIP_WAIT_FOR_DESCENT_TRIGGER:
        level_table_byte =
            *(unsigned char *)(moby->fB0 + (current_level_index_macro << 4) + visited52);
        if (level_table_byte == 0xFF) {
            if (moby->f53 != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(0x14));
            }
            moby->state = 10;
            moby->flags = moby->flags & 0xFFBE;
            moby->f32 = level_table_byte;
            moby->f94 = moby->pclass->f10;
        } else {
            if (cutscene_mode_macro == 0) {
                unsigned char *pp = alternate_item_available;
                if (pp[0xF] == 0) {
                    start_scene(5);
                    pp[0xF] = 1;
                }
            }
            deactivate_recorded_moby();
            if (is_point_inside_clip_volume(D_0013F3D0, data->f10) != 0) {
                moby->flags = moby->flags & 0xFFBE;
                moby->f94 = moby->pclass->f10;
                if (data->link != -1) {
                    linked_moby_data = ents_macro[data->link].data;
                    linked_moby_data->timer = scale_game_frames(0x1A4);
                    ents_macro[data->link].fBC = 1;
                    clear_u64_value(&l01_cuboids_macro[linked_moby_data->rec].rot);
                    clear_u64_value(&linked_moby_data->v);
                    l01_cuboids_macro[linked_moby_data->rec].rot.y = -fast_arc_tan(
                        vector_distance_xy(&l01_cuboids_macro[linked_moby_data->rec].pos,
                                           &moby->pos),
                        moby->pos.z - l01_cuboids_macro[linked_moby_data->rec].pos.z);
                    l01_cuboids_macro[linked_moby_data->rec].rot.z =
                        fast_arc_tan(moby->pos.x - l01_cuboids_macro[linked_moby_data->rec].pos.x,
                                     moby->pos.y - l01_cuboids_macro[linked_moby_data->rec].pos.y);
                }
                moby->state = 2;
            }
        }
        break;
    case AGNOGG_SHIP_DESCEND_TO_TARGET_HEIGHT:
        moby->pos.z = moby->pos.z - frame_time_macro * 5.0f;
        if (data->link != -1) {
            linked_moby_data = ents_macro[data->link].data;
            linked_moby_data->v.y = -fast_arc_tan(
                vector_distance_xy(&l01_cuboids_macro[linked_moby_data->rec].pos, &moby->pos),
                moby->pos.z - l01_cuboids_macro[linked_moby_data->rec].pos.z);
            linked_moby_data->v.z =
                fast_arc_tan(moby->pos.x - l01_cuboids_macro[linked_moby_data->rec].pos.x,
                             moby->pos.y - l01_cuboids_macro[linked_moby_data->rec].pos.y);
        }
        if (moby->f53 == 0) {
            if (moby->pos.z - data->fC <= 0.0f) {
                set_moby_animation(moby, 1, 0);
            }
        }
        if (moby->pos.z <= data->fC) {
            moby->pos.z = data->fC;
            moby->state = 3;
        }
        break;
    case AGNOGG_SHIP_WAIT_FOR_ANIMATION_1_GATE:
        if (moby->f52 != 1) {
            break;
        }
        if ((moby->f70 & 2) == 0) {
            break;
        }
        if (moby->f53 != 2) {
            blend_moby_animation(moby, 2, 0, scale_game_frames(0x14));
        }
        if (moby->oclass == 0x2DA) {
            moby->state = 10;
        } else {
            moby->state = 4;
        }
        break;
    case AGNOGG_SHIP_WAIT_FOR_LINKED_MOBIES_INACTIVE:
        flag = 1;
        inactive_or_absent_link_count = 0;
        i = 0;
        do {
            if (data->list[i] != -1 && (char *)ents_macro + (data->list[i] << 8) != 0 &&
                ((Moby_2FC140 *)((char *)ents_macro + (data->list[i] << 8)))->state != 0xFE &&
                ((Moby_2FC140 *)((char *)ents_macro + (data->list[i] << 8)))->state != 0xFD) {
                flag = 0;
                break;
            }
            inactive_or_absent_link_count++;
            i++;
        } while (inactive_or_absent_link_count < 3);
        if (flag == 0) {
            break;
        }
        scene_start_gate_value = D_001413DC[0];
        if (scene_start_gate_value == 7) {
            break;
        }
        if (scene_start_gate_value == 0x14) {
            break;
        }
        data->timer = scale_game_frames(0x2D);
        moby->state = 5;
        break;
    case AGNOGG_SHIP_WAIT_BEFORE_SCENE:
        if (data->timer != 0) {
            break;
        }
        data->fA = moby->f32;
        moby->flags = moby->flags | 0x41;
        start_scene(3);
        moby->state = 6;
        break;
    case AGNOGG_SHIP_WAIT_FOR_SCENE_3_END:
        if (cutscene_mode_macro == 2) {
            break;
        }
        queue_scene_movie_with_game_language(3);
        moby->state = 7;
        break;
    case AGNOGG_SHIP_WAIT_TO_START_SCENE_4:
        if (cutscene_mode_macro == 2) {
            break;
        }
        start_scene(4);
        moby->state = 8;
        break;
    case AGNOGG_SHIP_WAIT_FOR_SCENE_4_END:
        if (cutscene_mode_macro == 2) {
            break;
        }
        unlock_planet(3);
        queue_planet_unlock_message(3);
        moby->flags = moby->flags & 0xFFBE;
        if (data->link != -1) {
            ((Lv *)D_L01_001BAD60_2FC140)->collected[ents_macro[data->link].fB2] =
                ents_macro[data->link].fB0 + 2;
            if (ents_macro[data->link].fB0 == 0xFF ||
                (D_L01_0015FC88[ents_macro[data->link].fB0] != 0xFF &&
                 visited52[ents_macro[data->link].fB0 + (current_level_index_macro << 4)] ==
                     0xFF)) {
                l01_save_state.cleared[ents_macro[data->link].fB2] = ents_macro[data->link].fB0 + 2;
            }
        }
        if (data->f3C != -1) {
            linked_moby_data = ents_macro[data->f3C].data;
            linked_moby_data->timer = scale_game_frames(0xB4);
            ents_macro[data->f3C].fBC = 1;
            if (data->f34 != -1 && data->f38 != -1) {
                ents_macro[data->f34].fBC = 2;
                ents_macro[data->f38].fBC = 2;
                record_moby_context_bytes(
                    &ents_macro[data->f34].fBC, 1, &ents_macro[data->f34], 1, D_L01_001BAAA0);
                record_moby_context_bytes(
                    &ents_macro[data->f38].fBC, 1, &ents_macro[data->f38], 1, D_L01_001BAAA0);
            }
        }
        jet_clear_persistent(moby->fB0);
        if (data->rec != -1) {
            jet_reset_camera(&l01_cuboids_macro[data->rec].pos, &l01_cuboids_macro[data->rec].rot);
        }
        memcard_save_data(0, -1);
        show_tracked_moby();
        moby->state = 10;
        break;
    case AGNOGG_SHIP_HOLD_TARGET_HEIGHT:
        moby->pos.z = data->fC;
        break;
    }
}

extern int scale_game_frames(int);

#include "qcopy.h"
