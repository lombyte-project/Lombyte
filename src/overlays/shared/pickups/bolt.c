/* Shared moby class bolt (and bolt_hexnut, bolt_starnut, bolt_circlenut): bolts dropped and collected. */
#include "types.h"
#include "rnc/game/bolt_sources.h"
#include "rnc/mobys/moby_class_id.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/ui/hud/draw_hud_item_selector_panel.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"

typedef struct {
    u8  pad0[0x24];
    f32 scale; /* 0x24 */
} BoltClass;

typedef struct {
    Vec4         pos;  /* 0x00 */
    Vec4         rot;  /* 0x10 */
    Vec4         vel;  /* 0x20 */
    Vec4         spin; /* 0x30 */
    u8           pad40[8];
    f32          unk48;       /* 0x48 */
    f32          unk4C;       /* 0x4C */
    f32          unk50;       /* 0x50 */
    u8           landed;      /* 0x54 */
    u8           unk55;       /* 0x55 */
    u8           hop_frames;  /* 0x56 */
    u8           hop_left;    /* 0x57 */
    f32          unk58;       /* 0x58 */
    void        *ground_moby; /* 0x5C */
    struct Bolt *unk60;       /* 0x60 */
    f32          speed;       /* 0x64 */
    f32          ground_z;    /* 0x68 */
    s16          slot;        /* 0x6C */
    s16          slot_timer;  /* 0x6E */
} BoltVars;

typedef struct Bolt {
    u8         pad0[0x10];
    Vec4       pos;   /* 0x10 */
    u8         state; /* 0x20 */
    u8         pad21[3];
    BoltClass *pclass; /* 0x24 */
    u8         pad28[4];
    f32        scale; /* 0x2C */
    u8         unk30; /* 0x30 */
    u8         unk31; /* 0x31 */
    u16        unk32; /* 0x32 */
    u16        flags; /* 0x34 */
    u8         pad36[0x3D];
    u8         unk73; /* 0x73 */
    u8         pad74[4];
    BoltVars  *vars; /* 0x78 */
    u8         pad7C[0x2A];
    s16        oclass; /* 0xA6 */
    u8         padA8[8];
    u8         unkB0; /* 0xB0 */
    u8         padB1;
    s16        unkB2; /* 0xB2 */
    u8         padB4[0xC];
    Vec4       rot; /* 0xC0 */
    u8         padD0[0x10];
    Vec4       unkE0; /* 0xE0 */
} Bolt;

typedef struct {
    u8 pad0[0x454];
    u8 killed[1]; /* 0x454: indexed by the moby save id */
} MobyKillTable;

typedef struct {
    Vec4 pos;
    s16  pad10[2];
    s16  moby; /* 0x14 */
    u8   pad16[0xA];
} BoltTrailSlot;

/* What a bolt moby's unkB8 points at: byte 0xB1 is its source's per-level id. */
typedef struct {
    u8 pad_00[0xB1];
    u8 id; /* 0x80 set: no id */
} BoltSource;

/* Vars of a bolt moby (classes 0xD..0x10). */
typedef struct {
    u8  pad_00[0x20];
    f32 vel[4] __attribute__((aligned(16)));  /* 0x20 */
    f32 spin[4] __attribute__((aligned(16))); /* 0x30: per-frame rotation (quaternion) */
    u8  counted; /* 0x40: CollectBolt also adds the bolt to D_0015EE2C */
    u8  pad_41[0xF];
    f32 unk50; /* random angle */
    u8  pad_54;
    u8  unk55; /* bit 0 set at random */
    u8  pad_56[2];
    f32 unk58; /* random angle */
    u8  pad_5C[0xC];
    f32 start_z; /* 0x68 */
    s16 unk6C;
    s16 unk6E; /* random 0 .. 600 frames */
} BoltVars_u;

extern float D_0015ED6C;
extern float scale_time(float) __asm__("FUN_001f96b0");

extern s32                         D_L00_0015F5C4;
extern CollisionHit D_L00_00173E40 NOT_SDA;
extern Vec4          l00_bolt_glint_local_offsets_by_class[] __asm__("D_L00_001D73D0");
extern BoltTrailSlot glints[] __asm__("D_L00_0016EA40");
extern u8           *D_L00_0015FFDC;
extern s32           D_L00_0015FFBC;
extern u8           *moby_table_macro __asm__("D_L00_0015FFD8") MACRO_ADDR;
extern u8            mission_state_macro[] __asm__("D_L00_0015FC88") MACRO_ADDR;
extern s32           D_0015ED84;
extern MobyKillTable D_L00_001BA5D0 NOT_SDA;
extern MobyKillTable D_L00_001BB230 NOT_SDA;
extern u8                           D_0014C050[][16];
extern f32                          D_0015ED6C;

extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern s32  FUN_001efa68(void *, void *, s32, s32, s32);
extern void approach_value(void *, f32, f32) __asm__("FUN_00213ed8");
extern s32  FUN_L00_001f0d60(void *, s32, void *, f32);
extern s32  allocate_glint_slot(void *, s32, f32) __asm__("FUN_L00_001fe990");
extern s32  FUN_L00_002603d0(void *);
extern void convert_pose_to_reference_local(void *, void *, void *, void *, void *, void *) __asm__("FUN_L00_00260668");
extern s32  FUN_L00_00257b90(s32, s32);
extern void build_quaternion_from_axis_rotations(void *, void *) __asm__("FUN_L00_00259f50");
extern void FUN_L00_0025f730(void *, f32);
extern void FUN_L00_002a6de8_c(Bolt *, void *, s32) __asm__("FUN_L00_002a6de8");
extern void FUN_L00_002a6b70_c(Bolt *) __asm__("FUN_L00_002a6b70");
extern void FUN_L00_002a7210_c(Bolt *, void *, f32, f32) __asm__("FUN_L00_002a7210");

extern s32 D_0015EE2C; /* bolts counted when the bolt's pvars[0x40] is set or it is an early moby */

extern s32   level_bolts[] __asm__("D_0013DF38"); /* bolts collected per level */
extern s32   D_L00_0015F598;
extern void *D_L00_0015FFDC_u __asm__("D_L00_0015FFDC");
extern void
queue_animation_update(s32, s32, void *, void *, void *, void *, s32) __asm__("FUN_001ff308");

extern u8 *FUN_L00_00272060(
    void *pos, s32 life, f32 size0, f32 size1, s32 color, s32 mode, s32 spin, void *vel, f32 f2);

extern char *FUN_L00_002603d0_c(void *) __asm__("FUN_L00_002603d0");
extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int   transform_attachment_pose(
      int x, char *o, float *p, float *q, float *r, float *s) __asm__("FUN_L00_002604f0");

extern int  D_15F5CC_2a7210 __asm__("D_L00_0015F5CC");
extern void build_quaternion_from_axis_angle(void *, void *, float) __asm__("FUN_00214530");

extern int   last_bolt_attraction_sound_frame_gp __asm__("D_L00_001613AC") __attribute__((sda));
extern float bolt_spin_angle_limit_degrees_gp __asm__("D_L00_001613A8") __attribute__((sda));

extern void *D_001413D0 NOT_SDA;
extern void             FUN_L00_001ff500(void *, void *, float);

struct Bolt;

void bolt_update(Bolt *obj) __asm__("FUN_L00_002a5dd8");

void bolt_update(Bolt *bolt_moby) {
    enum {
        BOLT_INITIALIZE = 0x0,
        BOLT_FALLING = 0x1,
        BOLT_SETTLING = 0x2,
        BOLT_IDLE = 0x3,
        BOLT_MOVE_TO_HERO = 0x4
    };
    BoltVars *v = bolt_moby->vars;
    Vec4      geometry_scratch;
    Vec4      vector_scratch;
    Vec4      rotation_scratch;
    Vec4      collection_offset;
    Vec4      attraction_rotation_angles;
    Vec4      attraction_rotation_basis[3];
    s32       hit;
    f32       sc;
    f32       hr;
    f32       settling_inclination_limit;
    f32       k;
    f32       velocity_decay_step;
    f32       u;
    f32       settling_hop_phase;
    f32       hero_distance;
    f32       x;
    f32       rotation_blend_step;
    s32       flag;
    u8        i;

    if (D_L00_0015F5C4 == 2) {
        bolt_moby->unk31 = 0;
        bolt_moby->flags |= 1;
    } else {
        bolt_moby->unk31 = 1;
        bolt_moby->flags &= 0xFFFE;
    }
    switch (bolt_moby->state) {
    case BOLT_INITIALIZE:
        bolt_moby->state = 3;
        bolt_moby->unk73 = 0x14;
        bolt_moby->flags |= 0x100;
        bolt_moby->unk30 = 0x20;
        bolt_moby->unk32 = 0x20;
        if (random_integer_below(2) == 0) {
            v->unk55 |= 1;
        }
        fast_vec_normalize(&geometry_scratch, &bolt_moby->unkE0, 0.2f);
        fast_vec_add(&geometry_scratch, &geometry_scratch, &bolt_moby->pos);
        fast_vec_normalize(&vector_scratch, &bolt_moby->unkE0, -3.0f);
        fast_vec_add(&vector_scratch, &vector_scratch, &bolt_moby->pos);
        hit = FUN_001efa68(&geometry_scratch, &vector_scratch, 0x22, 0, 0);
        if (v->landed == 0 && hit != 0) {
            qcopy(&v->pos, &D_L00_00173E40.point);
            clear_u64_value(&v->rot);
            v->rot.f[2] = fast_arc_tan(D_L00_00173E40.normal_x, D_L00_00173E40.normal_y);
            v->rot.f[1] =
                fast_arc_tan(D_L00_00173E40.normal_z, vector_length_xy(&D_L00_00173E40.normal_x));
            if (D_L00_00173E40.moby != 0 && FUN_L00_002603d0((void *)D_L00_00173E40.moby) != 0) {
                void *gm = (void *)D_L00_00173E40.moby;
                v->ground_moby = gm;
                convert_pose_to_reference_local(bolt_moby, gm, v, &v->rot, v, &v->rot);
            }
        } else {
            qcopy(&v->pos, &bolt_moby->pos);
            clear_u64_value(&v->rot);
        }
        v->unk50 = random_angle_radians();
        v->unk58 = random_angle_radians();
        v->ground_z = bolt_moby->pos.f[2];
        v->slot_timer = FUN_L00_00257b90(0, scale_game_frames(600));
        v->slot = -1;
        switch (bolt_moby->oclass) {
        case BOLT_CLASS_ONE:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.65f;
            break;
        case BOLT_CLASS_FIVE:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.7f;
            break;
        case BOLT_CLASS_TWENTY:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.75f;
            break;
        case BOLT_CLASS_FIFTY:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.6f;
            break;
        }
        break;
    case BOLT_FALLING:
        v->vel.f[2] -= frame_time_sq * 10.8f;
        fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &v->vel);
        quaternion_to_rotation_basis(&v->spin, &rotation_scratch);
        multiply_matrix3x3(&bolt_moby->rot, &rotation_scratch, &bolt_moby->rot);
        switch (bolt_moby->oclass) {
        case BOLT_CLASS_ONE:
            hr = 0.15f;
            break;
        case BOLT_CLASS_FIVE:
            hr = 0.23f;
            break;
        case BOLT_CLASS_TWENTY:
            hr = 0.23f;
            break;
        default:
            hr = 0.25f;
            break;
        }
        if (bolt_moby->pos.f[2] < v->ground_z - 2.0f &&
            bolt_moby->pos.f[2] < hero.motion.pos.f[2] - 2.0f) {
            FUN_L00_002a7210_c(
                bolt_moby, hero.moby, random_float_between(-15.0f, 15.0f) * 0.017453292f, 0.0f);
            return;
        }
        if (FUN_L00_001f0d60(&bolt_moby->pos, 0x22, bolt_moby, hr) == 0) {
            return;
        }
        qcopy(&v->pos, &D_L00_00173E40.point);
        clear_u64_value(&v->rot);
        v->rot.f[2] = fast_arc_tan(D_L00_00173E40.normal_x, D_L00_00173E40.normal_y);
        v->rot.f[1] =
            fast_arc_tan(D_L00_00173E40.normal_z, vector_length_xy(&D_L00_00173E40.normal_x));
        fast_vec_sub(&geometry_scratch, &bolt_moby->pos, &D_L00_00173E40.point);
        reflect_vector(&v->vel, &v->vel, &geometry_scratch);
        if (vector_distance_xy(&bolt_moby->pos, &hero.motion.pos) < hero.reach_distance &&
            absolute_float(bolt_moby->pos.f[2] - hero.motion.pos.f[2]) < hero.reach_height) {
            if (hero.health.hp == 0) {
                return;
            }
            fast_vec_normalize(&v->vel, &v->vel, random_float_between(3.0f, 7.0f) * D_0015ED6C);
            FUN_L00_002a7210_c(bolt_moby, hero.moby, 0.0f, 0.0f);
            return;
        }
        settling_inclination_limit = 0.7853982f;
        if (!(fast_arc_tan(geometry_scratch.f[2], vector_length_xy(&geometry_scratch)) <
              settling_inclination_limit)) {
            return;
        }
        if (!(fast_arc_tan(D_L00_00173E40.normal_z, vector_length_xy(&D_L00_00173E40.normal_x)) <
              settling_inclination_limit)) {
            return;
        }
        bolt_moby->state = 2;
        v->hop_frames = v->hop_left =
            truncate_float_to_s32(scale_time(random_float_between(30.0f, 40.0f)));
        if (D_L00_00173E40.moby != 0 && FUN_L00_002603d0((void *)D_L00_00173E40.moby) != 0) {
            void *gm = (void *)D_L00_00173E40.moby;
            v->ground_moby = gm;
            convert_pose_to_reference_local(bolt_moby, gm, v, &v->rot, v, &v->rot);
        }
        matrix_to_euler(&bolt_moby->rot, &vector_scratch);
        build_quaternion_from_axis_rotations(&v->spin, &vector_scratch);
        return;
    case BOLT_SETTLING:
        FUN_L00_002a6de8_c(bolt_moby, &vector_scratch, 0);
        k = 0.25f;
        vector_scratch.f[2] += k;
        matrix_to_euler(&bolt_moby->rot, &geometry_scratch);
        build_quaternion_from_axis_rotations(&rotation_scratch, &geometry_scratch);
        rotation_blend_step = 1.0f / convert_integer_to_float(v->hop_left);
        settling_hop_phase =
            convert_integer_to_float(v->hop_left) / convert_integer_to_float(v->hop_frames);
        quaternion_nlerp(&v->spin, &v->spin, &rotation_scratch, rotation_blend_step);
        lerp_vector(&bolt_moby->pos, &bolt_moby->pos, &vector_scratch, settling_hop_phase);
        settling_hop_phase -= 0.5f;
        bolt_moby->pos.f[2] +=
            -(settling_hop_phase * settling_hop_phase - k) * ((f32)v->hop_frames * 0.025f);
        quaternion_to_rotation_basis(&v->spin, &bolt_moby->rot);
        if (--v->hop_left != 0) {
            return;
        }
        bolt_moby->state = 3;
        v->hop_frames = v->hop_left =
            truncate_float_to_s32(scale_time(random_float_between(10.0f, 20.0f)));
        return;
    case BOLT_IDLE:
        hero_distance = vector_distance_xy(&bolt_moby->pos, &hero.motion.pos);
        if (hero_distance < hero.reach_distance &&
            absolute_float(bolt_moby->pos.f[2] - hero.motion.pos.f[2]) < hero.reach_height) {
            FUN_L00_002a7210_c(bolt_moby, hero.moby, 0.0f, 0.0f);
            return;
        }
        if ((u8 *)bolt_moby > D_L00_0015FFDC && D_L00_0015FFBC < 200 && 48.0f < hero_distance &&
            bolt_moby->unk31 == 0) {
            mark_moby_for_removal(bolt_moby);
            return;
        }
        if (bolt_moby->unk31 != 0 || v->ground_moby != 0) {
            FUN_L00_002a6de8_c(bolt_moby, 0, 0);
            if (v->hop_left != 0) {
                u = convert_integer_to_float(v->hop_left) / convert_integer_to_float(v->hop_frames);
                u -= 0.5f;
                bolt_moby->pos.f[2] += -(u * u - 0.25f) * ((f32)v->hop_frames * 0.0125f);
                v->hop_left--;
            }
            if (v->slot >= 0) {
                transform_vector_by_basis(&geometry_scratch,
                                          &l00_bolt_glint_local_offsets_by_class[bolt_moby->oclass],
                                          &bolt_moby->rot);
                fast_vec_add(&glints[v->slot], &geometry_scratch, &bolt_moby->pos);
                if (glints[v->slot].moby != (((u8 *)bolt_moby - moby_table_macro) >> 8)) {
                    v->slot = -1;
                }
                return;
            }
            if (--v->slot_timer > 0) {
                return;
            }
            transform_vector_by_basis(&geometry_scratch,
                                      &l00_bolt_glint_local_offsets_by_class[bolt_moby->oclass],
                                      &bolt_moby->rot);
            fast_vec_add(&geometry_scratch, &geometry_scratch, &bolt_moby->pos);
            v->slot = allocate_glint_slot(
                &geometry_scratch, ((u8 *)bolt_moby - moby_table_macro) >> 8, 1.4f);
            v->slot_timer = FUN_L00_00257b90(scale_game_frames(300), scale_game_frames(600));
        } else if (v->hop_left != 0) {
            v->hop_left--;
        }
        return;
    case BOLT_MOVE_TO_HERO:
        flag = 0;
        if (v->unk60->oclass == 0) {
            fast_vec_sub(&geometry_scratch, &bolt_moby->pos, &hero.ground_point);
            transform_vector(&geometry_scratch, &geometry_scratch, hero.inv_rot_matrix);
            if (geometry_scratch.f[2] < 0.0f) {
                flag = 1;
            }
        }
        if (flag) {
            fast_vec_normalize(&geometry_scratch, &hero.rot_matrix[8], D_0015ED6C * 7.0f);
            fast_vec_sub(&vector_scratch, &geometry_scratch, &v->vel);
            FUN_L00_0025f730(&vector_scratch, frame_time_sq * 50.0f);
            fast_vec_add(&v->vel, &v->vel, &vector_scratch);
        } else {
            velocity_decay_step = frame_time_sq * 10.0f;
            if (velocity_decay_step < fast_vec_length(&v->vel)) {
                fast_vec_normalize(&geometry_scratch, &v->vel, velocity_decay_step);
                fast_vec_sub(&v->vel, &v->vel, &geometry_scratch);
            }
            approach_value(&v->speed, D_0015ED6C * 48.0f, frame_time_sq * 16.0f);
            x = D_0015ED6C * 8.0f;
            if (x < hero.motion.speed_xy) {
                fast_vec_normalize(
                    &vector_scratch, &hero.motion.displacement, (hero.motion.speed_xy - x) * 0.5f);
                fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &vector_scratch);
            }
        }
        quaternion_to_rotation_basis(&v->spin, &geometry_scratch);
        multiply_matrix3x3(&bolt_moby->rot, &geometry_scratch, &bolt_moby->rot);
        fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &v->vel);
        fast_vec_sub(&collection_offset, &hero.motion.body_center, &bolt_moby->pos);
        if (v->speed < fast_vec_length(&collection_offset)) {
            if (!(v->unk4C == 0.0f && v->unk48 == 0.0f)) {
                transform_vector_by_basis(
                    &collection_offset, &collection_offset, hero.inv_rot_matrix);
                clear_u64_value(&attraction_rotation_angles);
                attraction_rotation_angles.f[1] = v->unk48;
                attraction_rotation_angles.f[2] = v->unk4C;
                euler_rotation_basis(attraction_rotation_basis, &attraction_rotation_angles);
                transform_vector_by_basis(
                    &collection_offset, &collection_offset, attraction_rotation_basis);
                transform_vector_by_basis(&collection_offset, &collection_offset, hero.rot_matrix);
            }
            fast_vec_normalize(&collection_offset, &collection_offset, v->speed);
            fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &collection_offset);
            return;
        }
        FUN_L00_002a6b70_c(bolt_moby);
        if ((u8 *)bolt_moby < D_L00_0015FFDC) {
            D_L00_001BA5D0.killed[bolt_moby->unkB2] = bolt_moby->unkB0 + 2;
            if (bolt_moby->unkB0 == 0xFF || (mission_state_macro[bolt_moby->unkB0] != 0xFF &&
                                             D_0014C050[D_0015ED84][bolt_moby->unkB0] == 0xFF)) {
                D_L00_001BB230.killed[bolt_moby->unkB2] = bolt_moby->unkB0 + 2;
            }
        }
        mark_moby_for_removal(bolt_moby);
        return;
    }
}

/* second name for D_0015ED84: retail loads the level index again here */

/* spawns a sparkle particle */

/*
 * CollectBolt: adds the bolt moby's value (1, or 5 / 20 / 50 for classes
 * 0xE / 0xF / 0x10) to the bolt count, the level's total and its source's
 * stats, shows the bolt counter and spawns two sparkles at the bolt.
 */

void collect_bolt(struct Moby *bolt) __asm__("FUN_L00_002a6b70");

void collect_bolt(struct Moby *bolt) {
    Vec4        sparkle_initial_velocity;
    Vec4        pos;
    s32         value;
    s32         spin;
    f32         k;
    f32         size;
    BoltSource *bolt_source;

    switch (bolt->oclass) {
    default:
    case 0:
        value = 1;
        break;
    case BOLT_CLASS_FIVE:
        value = 5;
        break;
    case BOLT_CLASS_TWENTY:
        value = 20;
        break;
    case BOLT_CLASS_FIFTY:
        value = 50;
        break;
    }

    bolt_source = bolt->parent;
    if (bolt_source && !(bolt_source->id & 0x80))
        bolt_source_stats[current_level_index][bolt_source->id].bolts += value;
    current_bolt_count += value;
    if (bolt->pvars[0x40] || (D_L00_0015F598 && (void *)bolt < D_L00_0015FFDC_u))
        D_0015EE2C += value;
    level_bolts[current_level_index] += value;

    k = 0.7f;
    queue_animation_update(2,
                           0x754E,
                           initialize_bolt_counter_hud,
                           update_bolt_counter_hud_animation_and_visibility,
                           draw_bolt_counter_hud_panel,
                           &current_bolt_count,
                           9999999);

    /* two sparkles 10 units out along a random direction, spinning opposite ways */
    random_vector(&sparkle_initial_velocity, D_0015ED6C * k, D_0015ED6C);
    fast_vec_scale(&pos, &sparkle_initial_velocity, 10.0f);
    fast_vec_add(&pos, &pos, &bolt->pos);
    spin = random_integer_below(2);
    if (spin == 0)
        spin--;
    size = random_float_between(0.4f, 0.5f);
    FUN_L00_00272060(&pos,
                     scale_game_frames(0x19),
                     size * 0.2f,
                     size,
                     0x7F207F7F,
                     0,
                     spin,
                     &sparkle_initial_velocity,
                     0.0f);
    spin = -spin;
    FUN_L00_00272060(&pos,
                     scale_game_frames(0x19),
                     size * 0.14f,
                     size * k,
                     0x7F7F7F7F,
                     1,
                     spin,
                     &sparkle_initial_velocity,
                     0.0f);
}

/* Steers a thrown moby along its path: builds its orientation from the data's angles and, when done, releases the held target. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A8088), where it is exact; names translated to the US level program. */

void update_bolt_grounded_pose(char *moby, void *out1, void *out2) __asm__("FUN_L00_002a6de8");

void update_bolt_grounded_pose(char *moby, void *out1, void *out2) {
    char *data = ((char *)MOBY(moby)->pvars);
    float local_rotation_basis[12];
    float base_rotation_basis[12];
    float local_rotation_angles[4];
    float rotated_position_offset[4];
    float A[4];
    float B[4];
    float D[4];
    float C[4];
    float rotation_angle_step, class_local_rotation_angle, base_axis_offset_distance,
        rotated_axis_offset_distance, ground_collision_radius;

    if (*(char **)(data + 0x5C) != 0) {
        transform_attachment_pose(
            (int)moby, *(char **)(data + 0x5C), (float *)data, (float *)(data + 0x10), A, B);
    } else {
        qcopy(A, data);
        qcopy(B, data + 0x10);
    }
    switch (MOBY(moby)->oclass) {
    case BOLT_CLASS_ONE:
        rotation_angle_step = 0.04f;
        class_local_rotation_angle = -0.25f;
        base_axis_offset_distance = 0.04875f;
        rotated_axis_offset_distance = 0.325f;
        ground_collision_radius = 0.15f;
        break;
    case BOLT_CLASS_FIVE:
        rotation_angle_step = 0.03f;
        class_local_rotation_angle = -0.5f;
        base_axis_offset_distance = 0.0245f;
        rotated_axis_offset_distance = 0.35f;
        ground_collision_radius = 0.23f;
        break;
    case BOLT_CLASS_TWENTY:
        rotation_angle_step = 0.025f;
        class_local_rotation_angle = -0.4f;
        base_axis_offset_distance = 0.05625f;
        rotated_axis_offset_distance = 0.375f;
        ground_collision_radius = 0.23f;
        break;
    default:
        rotation_angle_step = 0.0175f;
        class_local_rotation_angle = -0.35f;
        base_axis_offset_distance = 0.135f;
        rotated_axis_offset_distance = 0.3f;
        ground_collision_radius = 0.25f;
        break;
    }
    if (*(unsigned char *)(data + 0x55) & 1) {
        *(float *)(data + 0x50) = fast_add_rotations(*(float *)(data + 0x50), rotation_angle_step);
        *(float *)(data + 0x58) = fast_add_rotations(*(float *)(data + 0x58), -0.0125f);
    } else {
        *(float *)(data + 0x50) =
            fast_subtract_rotations(*(float *)(data + 0x50), rotation_angle_step);
        *(float *)(data + 0x58) = fast_subtract_rotations(*(float *)(data + 0x58), -0.0125f);
    }
    qcopy(local_rotation_angles, data + 0x50);
    local_rotation_angles[1] = class_local_rotation_angle;
    euler_rotation_basis(local_rotation_basis, local_rotation_angles);
    euler_rotation_basis(base_rotation_basis, B);
    multiply_matrix3x3(moby + 0xC0, base_rotation_basis, local_rotation_basis);
    fast_vec_scale(moby + 0x10, &base_rotation_basis[8], base_axis_offset_distance);
    fast_vec_scale(rotated_position_offset, moby + 0xC0, rotated_axis_offset_distance);
    fast_vec_add(moby + 0x10, moby + 0x10, rotated_position_offset);
    fast_vec_add(moby + 0x10, moby + 0x10, A);
    if (out1 != 0)
        qcopy(out1, A);
    if (out2 != 0)
        qcopy(out2, B);
    if (MOBY(moby)->state == 3) {
        char *ground_moby = *(char **)(data + 0x5C);
        if (ground_moby != 0 && *(unsigned char *)(data + 0x57) == 0) {
            int   should_detach_from_ground;
            char *x;
            if ((unsigned char)ground_moby[0x20] == 0xFE ||
                (*(unsigned char **)(data + 0x5C))[0x20] == 0xFD) {
                should_detach_from_ground = 1;
            } else {
                should_detach_from_ground =
                    coll_sphere(ground_collision_radius, moby + 0x10, 0x22, moby) == 0;
            }
            if (should_detach_from_ground) {
                moby[0x20] = 1;
                clear_u64_value(data + 0x30);
                *(float *)(data + 0x3C) = 1.0f;
                x = FUN_L00_002603d0_c(*(char **)(data + 0x5C));
                if (x != 0) {
                    qcopy(data + 0x20, x + 0x10);
                } else {
                    clear_u64_value(data + 0x20);
                }
                fast_vec_normalize(C, &base_rotation_basis[8], D_0015ED6C);
                fast_vec_normalize(D, (float *)&MOBY(moby)->rot_row0, D_0015ED6C);
                fast_vec_add(C, C, D);
                fast_vec_add(data + 0x20, data + 0x20, C);
                fast_vec_add(moby + 0x10, moby + 0x10, data + 0x20);
                *(char **)(data + 0x5C) = 0;
            }
        }
    }
}

int begin_bolt_attraction(void *ov,
                          void *tv,
                          float attraction_yaw_angle,
                          float attraction_pitch_angle) __asm__("FUN_L00_002a7210");

int begin_bolt_attraction(void *ov,
                          void *tv,
                          float attraction_yaw_angle,

                          float attraction_pitch_angle) {
    char *bolt_moby = ov;
    char *target_moby = tv;
    char *bolt_vars = ((char *)MOBY(bolt_moby)->pvars);
    int   sound_cooldown_frames, last, now;
    float k;
    float attraction_geometry_scratch[3][4] __attribute__((aligned(16)));

    if (MOBY(bolt_moby)->state == 4) {
        return 0;
    }
    sound_cooldown_frames = scale_game_frames(3);
    last = last_bolt_attraction_sound_frame_gp;
    now = D_15F5CC_2a7210;
    if (last + sound_cooldown_frames < now || now < last) {
        last_bolt_attraction_sound_frame_gp = now;
        allocate_voice_for_target_entry(0, 0x20, bolt_moby);
    } else {
        if (attraction_yaw_angle == 0.0f)
            attraction_yaw_angle = random_float_between(-30.0f, 30.0f) * DEG_TO_RAD;
        if (attraction_pitch_angle == 0.0f)
            attraction_pitch_angle = random_float_between(0.0f, 30.0f) * DEG_TO_RAD;
    }
    if (MOBY(bolt_moby)->state == 3) {
        euler_rotation_basis(attraction_geometry_scratch, bolt_vars + 0x10);
        fast_vec_normalize(bolt_vars + 0x20,
                           attraction_geometry_scratch[2],
                           random_float_between(3.0f, 7.0f) * frame_time);
    }
    {
        *(float *)(bolt_vars + 0x4C) = attraction_yaw_angle;
        *(float *)(bolt_vars + 0x48) = attraction_pitch_angle;
        *(int *)(bolt_vars + 0x64) = 0;
        *(char **)(bolt_vars + 0x60) = target_moby;
        fast_vec_sub(attraction_geometry_scratch[1], target_moby + 0x10, bolt_moby + 0x10);
        fast_vec_cross(
            attraction_geometry_scratch[0], bolt_vars + 0x20, attraction_geometry_scratch[1]);
        fast_vec_normalize(bolt_vars + 0x30, attraction_geometry_scratch[0], 1.0f);
        k = random_float_between(bolt_spin_angle_limit_degrees_gp * 0.333000004f,
                                 bolt_spin_angle_limit_degrees_gp) *
            DEG_TO_RAD;
        build_quaternion_from_axis_angle(bolt_vars + 0x30, bolt_vars + 0x30, k * frame_time);
    }
    MOBY(bolt_moby)->state = 4;
    return 1;
}

/*
 * Spawns a bolt worth `value` (1, 5, 20 or 50) at `pos` with velocity `vel`,
 * coloured like `src`, spinning about vel x (src - pos). flags & 2 sends it
 * toward D_001413D0 (& 0x10 with a random angle). When no moby is free the
 * value goes straight into the bolt count.
 */

struct Moby *spawn_bolt_by_value(struct Moby *bolt_source,
                                 void        *pos,
                                 void        *vel,
                                 int          flags,
                                 int          value,
                                 int          counted) __asm__("FUN_L00_002a7438");

struct Moby *spawn_bolt_by_value(

    struct Moby *bolt_source, void *pos, void *vel, int flags, int value, int counted) {
    enum {
        BOLT_VALUE_ONE = 1,
        BOLT_VALUE_FIVE = 5,
        BOLT_VALUE_TWENTY = 20,
        BOLT_VALUE_FIFTY = 50
    };
    f32          spin_axis[4] __attribute__((aligned(16)));
    f32          bolt_to_source_offset[4] __attribute__((aligned(16)));
    struct Moby *spawned_bolt;
    BoltVars_u  *bolt_vars;
    struct Moby *root;
    u8          *color;
    float        scale;
    float        spin;
    float        angle;

    switch (value) {
    default:
    case BOLT_VALUE_ONE:
        spawned_bolt = (struct Moby *)create_moby(0xD);
        scale = 0.65f;
        break;
    case BOLT_VALUE_FIVE:
        spawned_bolt = (struct Moby *)create_moby(0xE);
        scale = 0.7f;
        break;
    case BOLT_VALUE_TWENTY:
        spawned_bolt = (struct Moby *)create_moby(0xF);
        scale = 0.75f;
        break;
    case BOLT_VALUE_FIFTY:
        spawned_bolt = (struct Moby *)create_moby(0x10);
        scale = 0.6f;
        break;
    }
    if (0 != spawned_bolt) {
        bolt_vars = (BoltVars_u *)spawned_bolt->pvars;
        bolt_vars->counted = counted;
        /* the bolt source is the root of src's unkB8 chain */
        root = bolt_source;
        if (bolt_source != 0) {
            while (root->parent != 0)
                root = root->parent;
        }
        spawned_bolt->parent = root;
        spawned_bolt->unk73 = 20;
        spawned_bolt->flags |= 0x100;
        spawned_bolt->update_dist = 0x40;
        spawned_bolt->draw_dist = 0x40;
        spawned_bolt->visible = 1;
        spawned_bolt->state = 1;
        spawned_bolt->scale = spawned_bolt->pclass->scale * scale;
        euler_rotation_basis(&spawned_bolt->rot_row0, &spawned_bolt->rot);
        if ((bolt_source->flags & 0x20) && (color = *(u8 **)(bolt_source->pvars + 0xC)) != 0)
            set_moby_color(spawned_bolt, color[4], color[5], color[6]);
        else
            spawned_bolt->spawn_frame = bolt_source->spawn_frame;
        bolt_vars->unk6E = random_int_between(0, scale_game_frames(600));
        bolt_vars->unk6C = -1;
        qcopy(&spawned_bolt->pos, pos);
        qcopy(bolt_vars->vel, vel);
        if (random_integer_below(2))
            bolt_vars->unk55 |= 1;
        bolt_vars->unk50 = random_angle_radians();
        bolt_vars->unk58 = random_angle_radians();
        bolt_vars->start_z = spawned_bolt->pos.z;
        spin = random_float_between(0.0f, bolt_spin_angle_limit_degrees_gp) * 0.017453292f *
               D_0015ED6C;
        fast_vec_sub(bolt_to_source_offset, &bolt_source->pos, &spawned_bolt->pos);
        fast_vec_cross(spin_axis, bolt_vars->vel, bolt_to_source_offset);
        FUN_L00_001ff500(bolt_vars->spin, spin_axis, 1.0f);
        build_quaternion_from_axis_angle(bolt_vars->spin, bolt_vars->spin, spin);
        if (flags & 2) {
            angle = 0.0f;
            if (flags & 0x10)
                angle = random_float_between(-30.0f, 30.0f) * 0.017453292f;
            begin_bolt_attraction(spawned_bolt, D_001413D0, angle, 0.0f);
        }
        update_entity((u8 *)spawned_bolt);
    } else {
        current_bolt_count += value;
    }
    return spawned_bolt;
}

s32           D_L01_0015FFBC = 0;
int           D_L01_001613AC = 0;
float         D_L01_001613A8 = 7.2e+02f;
s32           D_L02_0015FFBC = 0;
MobyKillTable D_L02_001BABD0 = {0};

int                  D_L02_00161380 = 0;
float                D_L02_0016137C = 7.2e+02f;
void *D_L02_00174458 NOT_SDA = 0;
s32                  D_L03_0015FFBC = 0;
int                  D_L03_00161388 = 0;
float                D_L03_00161384 = 7.2e+02f;
s32                  D_L04_0015FFBC = 0;
MobyKillTable        D_L04_001BA750 = {0};

int           D_L04_00161380 = 0;
float         D_L04_0016137C = 7.2e+02f;
s32           D_L05_0015FFBC = 0;
int           D_L05_00161380 = 0;
float         D_L05_0016137C = 7.2e+02f;
s32           D_L06_0015FFBC = 0;
MobyKillTable D_L06_001BAD50 = {0};

MobyKillTable D_L06_001BB9B0 = {0};

int           D_L06_00161380 = 0;
float         D_L06_0016137C = 7.2e+02f;
s32           D_L07_0015FFBC = 0;
MobyKillTable D_L07_001BA650 = {0};

MobyKillTable D_L07_001BB2B0 = {0};

int           D_L07_00161380 = 0;
float         D_L07_0016137C = 7.2e+02f;
int           D_L08_00161380 = 0;
float         D_L08_0016137C = 7.2e+02f;
s32           D_L09_0015FFBC = 0;
MobyKillTable D_L09_001BA750 = {0};

int                  D_L09_001613F0 = 0;
float                D_L09_001613EC = 7.2e+02f;
void *D_L09_00173FD8 NOT_SDA = 0;
s32                  D_L10_0015FFBC = 0;
int                  D_L10_00161380 = 0;
float                D_L10_0016137C = 7.2e+02f;
int                  D_L11_00161380 = 0;
float                D_L11_0016137C = 7.2e+02f;
s32                  D_L12_0015FFBC = 0;
MobyKillTable        D_L12_001BAA50 = {0};

MobyKillTable D_L12_001BB6B0 = {0};

int           D_L12_00161380 = 0;
float         D_L12_0016137C = 7.2e+02f;
s32           D_L13_0015FFBC = 0;
MobyKillTable D_L13_001BA8D0 = {0};

MobyKillTable D_L13_001BB530 = {0};

int           D_L13_00161380 = 0;
float         D_L13_0016137C = 7.2e+02f;
s32           D_L14_0015FFBC = 0;
MobyKillTable D_L14_001BACD0 = {0};

int           D_L14_001614A0 = 0;
float         D_L14_0016149C = 7.2e+02f;
s32           D_L15_0015FFBC = 0;
int           D_L15_00161380 = 0;
float         D_L15_0016137C = 7.2e+02f;
s32           D_L16_0015FFBC = 0;
MobyKillTable D_L16_001BAA50 = {0};

MobyKillTable D_L16_001BB6B0 = {0};

int           D_L16_00161380 = 0;
float         D_L16_0016137C = 7.2e+02f;
s32           D_L17_0015FFBC = 0;
MobyKillTable D_L17_001BAED0 = {0};

int           D_L17_00161380 = 0;
float         D_L17_0016137C = 7.2e+02f;
s32           D_L18_0015FFBC = 0;
MobyKillTable D_L18_001BAFD0 = {0};

MobyKillTable D_L18_001BBC30 = {0};

int   D_L18_00161380 = 0;
float D_L18_0016137C = 7.2e+02f;
