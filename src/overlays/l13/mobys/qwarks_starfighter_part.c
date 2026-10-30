/* Level 13 moby classes: qwarks_starfighter_part, forcefield, purple_ship. */
#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/rendering/level_render_state.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/rendering/geometry_quad.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/commands/set_moby_color.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/audio/streaming/continue_audio_stream_if_ready.h"

extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

void draw_starfighter_tractor_beam(struct Moby *moby) __asm__("FUN_L13_002ea8c8");

void draw_starfighter_tractor_beam(struct Moby *moby) {
    char               *vars = (char *)moby->pvars;
    struct GeometryQuad quad;
    float               start[4];
    float               direction[4];
    float               position[4];
    float               offset[4];
    float               side[4];
    float               up[4];
    float               edge[4];
    float               length;
    float               step;
    float               distance;
    float               width;
    float               scale;
    float               zero = 0.0f;
    float               one = 1.0f;
    u64                 texture;

    texture = get_effect_texture(0x34);
    quad.colors[3] = 0x40808080;
    quad.colors[2] = 0x40808080;
    quad.colors[1] = 0x40808080;
    quad.colors[0] = 0x40808080;
    quad.texture = texture;
    quad.texture_state = 0xFF9000000260LL;
    quad.reserved.value = 4;
    quad.primitive = 0x8000000048LL;
    quad.texture_coordinates[0][0] = zero;
    quad.texture_coordinates[2][0] = zero;
    quad.texture_coordinates[1][0] = one;
    quad.texture_coordinates[3][0] = one;
    quad.texture_coordinates[0][1] = zero;
    quad.texture_coordinates[1][1] = zero;
    quad.texture_coordinates[2][1] = one;
    quad.texture_coordinates[3][1] = one;
    moby_point_position(moby, 0x13, start);
    fast_vec_sub(direction, vars + 0x80, start);
    clamp_vector_length(direction, direction, 100.0f);
    length = fast_vec_length(direction);
    scale = length / 100.0f;
    if (scale < 1.0f)
        step = 1.0f;
    else
        step = scale;
    fast_vec_normalize(direction, direction, step);
    qcopy(position, start);
    distance = zero;
    fast_vec_normalize(offset, direction, convert_integer_to_float(frame_count % 20) / 20.0f);
    fast_vec_sub(position, position, offset);
    while (distance < length) {
        width = (distance * 0.0f) / (length + length) + 0.5f;
        fast_vec_cross(side, direction, D_L13_00160680);
        fast_vec_normalize(side, side, width);
        fast_vec_cross(up, direction, side);
        fast_vec_normalize(up, up, width);
        fast_vec_sub(edge, position, up);
        fast_vec_sub(&quad.positions[0], edge, side);
        fast_vec_add(&quad.positions[1], edge, side);
        fast_vec_add(edge, position, up);
        fast_vec_sub(&quad.positions[2], edge, side);
        fast_vec_add(&quad.positions[3], edge, side);
        if (10.0f < length && length - 10.0f < distance) {
            int color = truncate_float_to_s32((length - distance) * 6.4f) << 24 | 0x808080;
            quad.colors[3] = color;
            quad.colors[2] = color;
            quad.colors[1] = color;
            quad.colors[0] = color;
        }
        draw_geometry_quad(&quad, 0, 1);
        distance += step;
        fast_vec_add(position, position, direction);
    }
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_002EC038), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int  FUN_L00_001fefc8(void *);
extern int  queue_starfighter_voice_line(void *, void *, int, int) __asm__("FUN_L13_002e86f8");
extern unsigned char *fire_class_53_projectile(char *, char *) __asm__("FUN_L13_002ea6b8");
extern void           fire_starfighter_homing_rocket(void *, void *) __asm__("FUN_L13_002ea540");
extern char           D_0013F3D0[];
extern char           D_00140940[];

void update_qwarks_starfighter_attack_commands(void *mobyp, void *pp) __asm__("FUN_L13_002eac00");

void update_qwarks_starfighter_attack_commands(void *mobyp, void *pp) {
    enum {
        STARFIGHTER_COMMAND_TIMED_VOLLEY = 1,
        STARFIGHTER_COMMAND_ALTERNATING_ATTACKS = 2,
        STARFIGHTER_COMMAND_REPEATED_TARGETED_ATTACK = 3,
        STARFIGHTER_COMMAND_WAIT_FOR_ALTERNATING_ATTACKS_WITH_TRACTOR_BEAM = 4,
        STARFIGHTER_COMMAND_ALTERNATING_ATTACKS_WITH_TRACTOR_BEAM = 5,
        STARFIGHTER_COMMAND_ALTERNATING_ATTACKS_WITH_CONDITIONAL_MUZZLE_SWAP_AND_TRACTOR_BEAM = 6,
        STARFIGHTER_COMMAND_REPEATED_VECTOR_ATTACK = 7
    };
    char         *moby = mobyp;
    char         *starfighter_data = pp;
    float         sv[4];
    float         joint_to_hero_distance;
    int           voice_slot_index;
    char         *voice_slot_entry;
    unsigned char c, attack_mode;

    switch (((unsigned char *)moby)[0xBC]) {
    case STARFIGHTER_COMMAND_TIMED_VOLLEY:
        goto s1;
    case STARFIGHTER_COMMAND_ALTERNATING_ATTACKS:
        goto s2;
    case STARFIGHTER_COMMAND_REPEATED_TARGETED_ATTACK:
        goto s3;
    case STARFIGHTER_COMMAND_WAIT_FOR_ALTERNATING_ATTACKS_WITH_TRACTOR_BEAM:
        goto s4;
    case STARFIGHTER_COMMAND_ALTERNATING_ATTACKS_WITH_TRACTOR_BEAM:
        goto s3;
    case STARFIGHTER_COMMAND_ALTERNATING_ATTACKS_WITH_CONDITIONAL_MUZZLE_SWAP_AND_TRACTOR_BEAM:
        goto s3;
    case STARFIGHTER_COMMAND_REPEATED_VECTOR_ATTACK:
        goto s2;
    default:
        return;
    }

s1:
    if (((unsigned char *)moby)[0x20] != 4) {
        goto s1b;
    }
    if (!fast_dec_timer(starfighter_data + 0xF8)) {
        return;
    }
    fire_starfighter_homing_rocket(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x3C);
    ((unsigned char *)starfighter_data)[0xFD]++;
    if (((unsigned char *)starfighter_data)[0xFD] < 11) {
        return;
    }
    ((unsigned char *)starfighter_data)[0xFD] = 0;
    moby[0x20] = 3;
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x12C);
    return;

s1b:
    if (!fast_dec_timer(starfighter_data + 0xF8)) {
        return;
    }
    if (!(*(unsigned short *)(starfighter_data + 0x138) & 1)) {
        if (queue_starfighter_voice_line(moby, starfighter_data, 1, 2)) {
            *(unsigned short *)(starfighter_data + 0x138) |= 1;
        }
    }
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x1E);
    moby[0x20] = 4;
    return;

s3:
    fast_dec_timer(starfighter_data + 0x124);
    fast_dec_timer(starfighter_data + 0x100);
    fast_dec_timer(starfighter_data + 0xF8);
    if (((unsigned char *)moby)[0x20] == 5) {
        goto s308;
    }
    voice_slot_index = *(int *)(starfighter_data + 0x11C);
    if (voice_slot_index != -1) {
        voice_slot_entry = ((char *)&voice_pool) + voice_slot_index * 0x70;
        if (*(int *)(voice_slot_entry + 0x88) == (int)moby &&
            ((unsigned char *)voice_slot_entry)[0x74]) {
            release_voice_slot(voice_slot_index);
        }
        *(int *)(starfighter_data + 0x11C) = -1;
    }
    moby_point_position(moby, 0x13, sv);
    joint_to_hero_distance = fast_vec_dist(sv, ((char *)&D_0013F3D0));
    if (*(int *)(starfighter_data + 0x100) == 0) {
        if (joint_to_hero_distance < 100.0f) {
            if (60.0f < joint_to_hero_distance) {
                if (*(int *)(starfighter_data + 0xF8) != 0) {
                    return;
                }
                if (*(int *)(starfighter_data + 0x124) != 0) {
                    return;
                }
                *(int *)(starfighter_data + 0x100) = scale_game_frames(0x258);
                qcopy(starfighter_data + 0x80, sv);
                ((unsigned char *)starfighter_data)[0xFF] = 0;
                moby[0x20] = 5;
                return;
            }
        }
    }
    if (((unsigned char *)moby)[0xBC] == 3) {
        moby[0x20] = 4;
        if (*(int *)(starfighter_data + 0xF8) != 0) {
            return;
        }
        goto s384;
    }
    if (*(int *)(starfighter_data + 0xF8) != 0) {
        return;
    }
    if (FUN_L00_001fefc8(starfighter_data + 0xFD) == 0) {
        goto s2e;
    }
    if (*(int *)(starfighter_data + 0x124) != 0) {
        goto s2e;
    }
    attack_mode = ((unsigned char *)starfighter_data)[0xFE] ^ 1;
    ((unsigned char *)starfighter_data)[0xFE] = attack_mode;
    if (!attack_mode) {
        ((unsigned char *)starfighter_data)[0xFD] = random_int_between(4, 6);
        moby[0x20] = 4;
    } else {
        ((unsigned char *)starfighter_data)[0xFD] = random_int_between(0x14, 0x32);
        moby[0x20] = 3;
    }
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x78);
    return;

s2e:
    if (((unsigned char *)starfighter_data)[0xFE] == 0) {
        goto s380;
    }
    fire_class_53_projectile(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x14);
    *(int *)(starfighter_data + 0x124) = scale_game_frames(0xF0);
    return;

s308:
    if (*(int *)(starfighter_data + 0x100) != 0) {
        goto s358;
    }
    if (*(int *)(starfighter_data + 0x124) != 0) {
        return;
    }
    *(int *)(starfighter_data + 0x100) = scale_game_frames(0x258);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x78);
    moby[0x20] = 4;
    allocate_voice_for_target_entry(5, 0, (int)moby);
    return;

s358:
    if ((*(long *)(starfighter_data + 0xF8) & ((((0xFF00L << 32) | 0xFFFFL) << 16) | 0xFFFFL)) !=
        (0x8000L << 41)) {
        return;
    }
s380:
s384:
    fire_starfighter_homing_rocket(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x3C);
    *(int *)(starfighter_data + 0x124) = scale_game_frames(0x78);
    return;

s2:
    if (!fast_dec_timer(starfighter_data + 0xF8)) {
        return;
    }
    if (((unsigned char *)moby)[0xBC] != 2) {
        goto s2b;
    }
    if (FUN_L00_001fefc8(starfighter_data + 0xFD) == 0) {
        goto s2z;
    }
    attack_mode = ((unsigned char *)starfighter_data)[0xFE] ^ 1;
    ((unsigned char *)starfighter_data)[0xFE] = attack_mode;
    if (!attack_mode) {
        ((unsigned char *)starfighter_data)[0xFD] = random_int_between(4, 6);
        moby[0x20] = 4;
    } else {
        ((unsigned char *)starfighter_data)[0xFD] = random_int_between(0x14, 0x32);
        moby[0x20] = 3;
    }
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x3C);
    return;

s2z:
    if (((unsigned char *)starfighter_data)[0xFE] != 0) {
        goto s2y;
    }
    fire_starfighter_homing_rocket(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x3C);
    return;

s2y:
    fire_class_53_projectile(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x14);
    return;

s2b:
    moby[0x20] = 3;
    fire_class_53_projectile(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x14);
    return;

s4:
    moby[0x20] = 3;
    if (((unsigned char *)starfighter_data)[0x10E] != 0) {
        return;
    }
    moby[0xBC] = 5;
    queue_starfighter_voice_line(moby, starfighter_data, 0x1B, 2);
    {
        char *z = *(char **)(*(int *)(((char *)&D_00140940)) + 0x78);
        *(int *)(z + 0x88) = 0;
        *(int *)(z + 0xEC) = 0;
        *(int *)(z + 0x8C) = 0;
        *(int *)(z + 0x118) = 0;
    }
    return;
}

#include "qzero.h"

/* Update of Qwark's starfighter part: a state machine that spawns its parts, waits for the hero,
 * flies its path, docks with the hero, dies or resets, with sound handles and a camera light. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_002EC4D0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct {
    char           pad0[0x20];
    float          f20;
    short          s24;
    char           pad26[0x4A];
    float          f70;
    float          f74;
    int            i78;
    int            pad7C;
    float          v80[4];
    char           pad90[0xC];
    short          s9C;
    short          s9E;
    int            iA0;
    float          fA4;
    float          fA8;
    float          fAC;
    int            paths[5];
    char          *parts[10];
    char          *owner;
    char          *pF0;
    int            iF4;
    int            iF8;
    unsigned char  bFC;
    unsigned char  bFD;
    unsigned char  bFE;
    unsigned char  bFF;
    int            i100;
    int            hud104;
    int            i108;
    unsigned char  b10C;
    unsigned char  b10D;
    unsigned char  b10E;
    unsigned char  b10F;
    char          *child110;
    int            snd114;
    int            snd118;
    int            snd11C;
    int            light120;
    int            i124;
    int            pad128;
    int            i12C;
    int            pad130;
    int            i134;
    unsigned short h138;
    unsigned char  b13A;
    unsigned char  b13B;
    int            i13C;
    int            pad140;
    short          h144;
    short          h146;
    unsigned char  b148;
} Ship388;

typedef struct {
    char           pad0[0x10];
    float          pos[4];
    unsigned char  state;
    char           pad21[3];
    char          *cls;
    int            pad28;
    float          scale;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short flags;
    char           pad36[0xA];
    float          rot[4];
    char           pad50[0x28];
    Ship388       *d;
    char           pad7C[0x18];
    int            i94;
    char           pad98[0xE];
    short          cls_id;
    char           padA8[8];
    unsigned char  bB0;
    char           padB1[0xB];
    unsigned char  bBC;
    char           padBD[3];
    float          mat[3][4];
} Moby388;

typedef float PVec388[4] __attribute__((aligned(16)));

typedef struct {
    int     n;
    int     pad4[3];
    PVec388 pts[1];
} Path388;

extern Moby388 *D_L13_00174158[];
extern char    *spawn_owner_linked_class_160_moby(void *) __asm__("FUN_L13_002e8438");
extern char    *spawn_moby_joint_attachment(char *, int, int) __asm__("FUN_L13_002ecd10");
extern char     D_L13_001F4F38[];
extern char     qwarks_starfighter_high_elapsed_time_adjustment_message[] __asm__("D_L13_00161958");
extern char     qwarks_starfighter_mid_elapsed_time_adjustment_message[] __asm__("D_L13_00161968");
extern char     jet_camera47[] __asm__("D_L13_00166F80");
extern char     qwarks_starfighter_boss_start_time_format[] __asm__("D_L13_001F4EF0");
extern char     level13_point_light_slots[] __asm__("D_L13_001805C0");
extern char     qwarks_starfighter_tractor_offscreen_abort_message[] __asm__("D_L13_001F4F18");
extern f32      random_float_between(f32, f32) __asm__("func_002132A8");
extern float    D_L13_00160660[] MACRO_ADDR;
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int   D_0015ED84;
extern int   D_0015EEA4;
extern int   D_L13_0015FFD8; /* no foreign declaration */
int          D_L13_00161950 = 0;
int          D_L13_00161954 = -1;
extern int   qwarks_starfighter_command_path_slots[] __asm__("D_L13_001D3448");
extern int   qwarks_starfighter_attachment_class_ids[] __asm__("D_L13_001D34D8");
extern int   DebugPrint_alt() __asm__("FUN_001e93b0");
extern int   allocate_colored_effect_slot(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern int advance_moby_along_selected_path(void *, void *, int, int) __asm__("FUN_L13_002e9160");
extern int
update_moby_path_following_away_from_hero(void *, void *, int, int) __asm__("FUN_L13_002e9408");
extern int   queue_animation_update(int, int, int, int, int, int, int) __asm__("FUN_001ff308");
extern s32   random_integer_below(s32) __asm__("func_00213260");
extern short dialogue_playback_phase[] __asm__("D_0015172A");
extern void  reset_numeric_hud_element() __asm__("FUN_L00_00235d80");
extern void  hud_update_counter() __asm__("FUN_L00_00235ea0");
extern void  FUN_L00_0024f7c8(void *, int, void *);
extern void  FUN_L00_00258278(void *moby, float *vel, float target, float k, float d, float max);
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
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
                             int,
                             float,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern void  emit_gated_moby_joint_effects(void *, void *) __asm__("FUN_L13_002e8920");
extern void  update_moby_joint_attachments(void *, void *) __asm__("FUN_L13_002e8d28");
extern void
update_moby_spring_steering_and_banking(void *, void *, float, float) __asm__("FUN_L13_002e9018");
extern void detach_moby_joint_attachment(void *, void *, int) __asm__("FUN_L13_002e9680");
extern void process_moby_attachment_damage(void *, void *) __asm__("FUN_L13_002e9a48");
extern void draw_starfighter_tractor_beam();
extern void update_qwarks_starfighter_attack_commands(void *, void *) __asm__("FUN_L13_002eac00");
extern void flag_vendor_item(int) __asm__("FUN_L13_00316f48");
extern void FUN_L06_00249738();
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
void        emit_randomized_moby_joint_effects(struct Moby *moby) __asm__("FUN_L13_002e8b58");

#include "rnc/game/game_globals.h"

extern char  D_0013F350[];
extern void  func_L13_002E9F90_2(void *, void *) __asm__("FUN_L13_002e8b58");
extern char *D_L13_00160058_m __asm__("D_L13_0015FFD8");
extern char  D_0013E633_snd[] __asm__("D_0013E533");

void update_qwarks_starfighter(Moby388 *moby) __asm__("FUN_L13_002eb098");

void update_qwarks_starfighter(Moby388 *moby) {
    enum {
        STARFIGHTER_STATE_INITIALIZE = 0,
        STARFIGHTER_STATE_WAIT_FOR_HERO_SHIP = 1,
        STARFIGHTER_STATE_FOLLOW_INITIAL_PATH = 2,
        STARFIGHTER_STATE_FOLLOW_COMMAND_PATH = 3,
        STARFIGHTER_STATE_FOLLOW_COMMAND_PATH_WITH_TIMED_VOLLEY = 4,
        STARFIGHTER_STATE_TRACTOR_BEAM = 5,
        STARFIGHTER_STATE_OWNER_UNAVAILABLE = 6,
        STARFIGHTER_STATE_DEATH_FLIGHT = 7,
        STARFIGHTER_STATE_EXPLODE_AND_REMOVE = 8,
        STARFIGHTER_STATE_RESET = 9
    };
    Ship388 *data = moby->d;
    float    offset[4];
    float    joint[4];
    float    ray_start[4];
    float    probe[4];
    int      i;

    data->i108 = truncate_float_to_s32(data->f20 * 16.0f) & 0xFFF0;
    if (data->i108 >= truncate_float_to_s32(((float)data->s24 - 0.5f) * 16.0f)) {
        data->i108 |= 0xF;
    }
    if (0.5f < data->f20) {
        int segment = ((truncate_float_to_s32(data->f20) % (data->s24 / 6)) * 16) / (data->s24 / 6);
        if (segment < 0)
            segment = 0;
        data->i108 += segment & 0xF;
    } else {
        data->i108 = 0;
    }

    switch (moby->state) {
    case STARFIGHTER_STATE_INITIALIZE: {
        int j;
        if (moby->bB0 != 0xFF && visited52[moby->bB0 + (D_0015ED84 << 4)] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->scale = *(float *)(moby->cls + 0x24) * 4.0f;
        for (i = 0; i < 10; i++) {
            data->parts[i] = spawn_moby_joint_attachment(
                (char *)moby, i, qwarks_starfighter_attachment_class_ids[i]);
        }
        data->owner = (char *)moby;
        for (j = 0; j < 5; j++) {
            char *path = walker_paths[data->paths[j]];
            if (fast_vec_dist(path + 0x10, path + *(int *)path * 16) < 0.5f) {
                *(int *)path -= 1;
            }
        }
        data->fAC = frame_time * 42.0f * 0.1f;
        data->f70 = fast_cos(moby->rot[2]) * data->fAC;
        data->f74 = fast_sin(moby->rot[2]) * data->fAC;
        data->i78 = 0;
        qcopy(moby->pos, walker_paths[data->paths[0]] + 0x10);
        data->s9E = 0;
        data->s9C = 1;
        moby->b30 = 0xFF;
        moby->s32 = 0xFF;
        data->b10C = 1;
        data->bFE = 0;
        data->bFC = 0;
        moby->state = 1;
        data->b10E = 8;
        data->child110 = 0;
        data->snd114 = -1;
        data->snd118 = -1;
        data->snd11C = -1;
        data->f20 = data->s24;
        data->light120 = -1;
        data->h138 = 0;
        data->b148 = 0;
        data->i13C = 0;
        if (data->hud104 != -1) {
            set_animation_parameter(data->hud104, 0);
            data->hud104 = -1;
        }
        moby->i94 = 0;
        moby->flags |= 0x41;
        break;
    }
    case STARFIGHTER_STATE_WAIT_FOR_HERO_SHIP: {
        char *hero = ((char *)&D_0013F350);
        if (*(int *)(hero + 0x2084) == 0x32 && *(Moby388 **)(hero + 0x15F0) != 0) {
            moby->flags &= 0xFFBE;
            moby->i94 = *(int *)(moby->cls + 0x10);
            moby->s32 = 0x3FF;
            if (*(Moby388 **)(hero + 0x15F0) != 0) {
                char *ship = (char *)(*(Moby388 **)(hero + 0x15F0))->d;
                int   elapsed;
                if (D_L13_00161954 == -1) {
                    D_L13_00161954 = D_0015EEA4;
                    DebugPrint_alt(qwarks_starfighter_boss_start_time_format,
                                   D_0015EEA4,
                                   D_0015EEA4 / 60,
                                   D_0015EEA4 / 360);
                }
                elapsed = (unsigned int)(D_0015EEA4 - D_L13_00161954) / 360;
                if (elapsed >= 0x29) {
                    DebugPrint_alt(qwarks_starfighter_high_elapsed_time_adjustment_message);
                    data->s24 = 0x66;
                    data->f20 = 102.0f;
                    *(int *)(ship + 0x138) = 0x221;
                } else if (elapsed >= 0x1A) {
                    DebugPrint_alt(qwarks_starfighter_mid_elapsed_time_adjustment_message);
                    data->s24 = 0x78;
                    data->f20 = 120.0f;
                    *(int *)(ship + 0x138) = 0x234;
                }
            }
            moby->state = 2;
        }
        break;
    }
    case STARFIGHTER_STATE_FOLLOW_INITIAL_PATH: {
        {
            char *hero = ((char *)&D_0013F350);
            if (*(Moby388 **)(hero + 0x15F0) != 0 &&
                (*(Moby388 **)(hero + 0x15F0))->cls_id == 0x45) {
                (*(Moby388 **)(hero + 0x15F0))->bBC = 0;
            }
        }
        data->hud104 = queue_animation_update(0x11,
                                              0xFFFF,
                                              (int)reset_numeric_hud_element,
                                              (int)hud_update_counter,
                                              (int)FUN_L06_00249738,
                                              (int)&data->i108,
                                              (data->s24 << 4) | 0xF);
        data->fAC += (frame_time * 42.0f - data->fAC) * 0.1f;
        if (advance_moby_along_selected_path(moby, data, 0, 1) != 0) {
            char *hero = ((char *)&D_0013F350);
            char *ship;
            moby->bBC = 1;
            ship = (char *)(*(Moby388 **)(hero + 0x15F0))->d;
            *(int *)(ship + 0x88) = 0;
            *(int *)(ship + 0xEC) = 0;
            *(int *)(ship + 0x8C) = 0;
            *(int *)(ship + 0x118) = 0;
            data->b13B = qwarks_starfighter_command_path_slots[moby->bBC];
            data->iF8 = scale_game_frames(0x14);
            moby->state = 3;
            data->s9C = select_point_index_by_distance(
                moby->pos, walker_paths[data->paths[data->b13B]], 0.0f);
            data->s9C = *(unsigned short *)((char *)data + 0x9C) + random_int_between(3, 7);
            data->s9C = data->s9C % *(int *)walker_paths[data->paths[data->b13B]];
            data->s9E = data->s9C - 1;
            if (data->s9E < 0) {
                data->s9E = *(unsigned short *)walker_paths[data->paths[data->b13B]] - 1;
            }
        }
        break;
    }
    case STARFIGHTER_STATE_FOLLOW_COMMAND_PATH:
    case STARFIGHTER_STATE_FOLLOW_COMMAND_PATH_WITH_TIMED_VOLLEY:
        data->hud104 = queue_animation_update(0x11,
                                              0xFFFF,
                                              (int)reset_numeric_hud_element,
                                              (int)hud_update_counter,
                                              (int)FUN_L06_00249738,
                                              (int)&data->i108,
                                              (data->s24 << 4) | 0xF);
        if (*(Moby388 **)(((char *)&D_00140940)) != 0 &&
            (*(Moby388 **)(((char *)&D_00140940)))->cls_id == 0x45) {
            (*(Moby388 **)(((char *)&D_00140940)))->bBC = 0;
        }
        update_qwarks_starfighter_attack_commands(moby, data);
        if (update_moby_path_following_away_from_hero(moby, data, data->b13B, 1) & 2) {
            if (data->b13B != qwarks_starfighter_command_path_slots[moby->bBC]) {
                data->b13B = qwarks_starfighter_command_path_slots[moby->bBC];
                data->s9C = select_point_index_by_distance(
                    moby->pos,
                    walker_paths[data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]],
                    0.0f);
                data->s9C = *(unsigned short *)((char *)data + 0x9C) + random_int_between(3, 7);
                data->s9C =
                    data->s9C %
                    *(int *)
                        walker_paths[data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]];
                data->s9E = data->s9C - 1;
                if (data->s9E < 0) {
                    data->s9E =
                        *(unsigned short *)walker_paths
                            [data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]] -
                        1;
                }
            }
        }
        if (moby->bBC != 4) {
            process_moby_attachment_damage(moby, data);
        } else {
            trooper_anim_finish(moby, (char *)data + 0x60);
        }
        break;
    case STARFIGHTER_STATE_TRACTOR_BEAM: {
        float hero_distance;
        data->hud104 = queue_animation_update(0x11,
                                              0xFFFF,
                                              (int)reset_numeric_hud_element,
                                              (int)hud_update_counter,
                                              (int)FUN_L06_00249738,
                                              (int)&data->i108,
                                              (data->s24 << 4) | 0xF);
        update_qwarks_starfighter_attack_commands(moby, data);
        FUN_L00_0024f7c8(moby, 0x13, joint);
        hero_distance = fast_vec_dist(data->v80, ((char *)&D_0013F3D0));
        if (data->snd11C != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd11C * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0)
                goto have11C;
        }
        data->snd11C = -1;
        data->snd11C = allocate_voice_for_target_entry(4, 4, (int)moby);
    have11C:
        if (data->bFF == 0) {
            {
                char *hero = ((char *)&D_0013F350);
                if (*(Moby388 **)(hero + 0x15F0) != 0 &&
                    (*(Moby388 **)(hero + 0x15F0))->cls_id == 0x45) {
                    (*(Moby388 **)(hero + 0x15F0))->bBC = 0;
                }
            }
            data->fAC += (frame_time * 30.0f - data->fAC) * 0.1f;
            if (frame_time * 40.0f < hero_distance) {
                fast_vec_sub(offset, ((char *)&D_0013F3D0), data->v80);
                fast_vec_normalize(offset, offset, frame_time * 40.0f);
                fast_vec_add(data->v80, data->v80, offset);
            } else {
                qcopy(offset, ((char *)&D_0013F3D0));
                data->iF8 = scale_game_frames(0x78);
                if (!(data->h138 & 4)) {
                    if (queue_starfighter_voice_line(moby, data, 0x17, 4) != 0) {
                        data->h138 |= 4;
                    }
                }
                data->bFF = 1;
            }
            if (advance_moby_along_selected_path(moby, data, data->b13B, 1) & 2) {
                if (data->b13B != qwarks_starfighter_command_path_slots[moby->bBC]) {
                    data->b13B = qwarks_starfighter_command_path_slots[moby->bBC];
                    data->s9C = select_point_index_by_distance(
                        moby->pos,
                        walker_paths[data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]],
                        0.0f);
                    data->s9C = *(unsigned short *)((char *)data + 0x9C) + random_int_between(3, 7);
                    data->s9C = data->s9C %
                                *(int *)walker_paths
                                    [data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]];
                    data->s9E = data->s9C - 1;
                    if (data->s9E < 0) {
                        data->s9E =
                            *(unsigned short *)walker_paths
                                [data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]] -
                            1;
                    }
                }
            }
            fast_vec_sub(ray_start, data->v80, joint);
            fast_vec_normalize(ray_start, ray_start, 8.0f);
            fast_vec_add(ray_start, ray_start, joint);
            if (collision_line(ray_start, data->v80, 0, moby, 0) != 0) {
                if (D_L13_00174158[0] == 0 || D_L13_00174158[0]->cls_id == 0xD4) {
                    data->i100 = 0;
                }
            }
            qcopy(probe, data->v80);
            probe[3] = 1.0f;
            if (fast_bsphere_check(probe, 100.0f) <= 0 && moby->b31 == 0) {
                DebugPrint_alt(qwarks_starfighter_tractor_offscreen_abort_message);
                data->i100 = 0;
            }
            if (fast_vec_dist(ray_start, ((char *)&D_0013F3D0)) > 100.0f) {
                DebugPrint_alt(D_L13_001F4F38);
                data->i100 = 0;
            }
        } else {
            char *hero = ((char *)&D_0013F350);
            float yaw, pitch, distance;
            yaw = fast_arc_tan(*(float *)(hero + 0x80) - moby->pos[0],
                               *(float *)(hero + 0x84) - moby->pos[1]);
            pitch = -fast_arc_tan(vector_distance_xy(moby->pos, hero + 0x80),
                                  *(float *)(hero + 0x88) - moby->pos[2]);
            update_moby_spring_steering_and_banking(moby, data, yaw, pitch);
            if (*(Moby388 **)(hero + 0x15F0) != 0 &&
                (*(Moby388 **)(hero + 0x15F0))->cls_id == 0x45) {
                (*(Moby388 **)(hero + 0x15F0))->bBC = 1;
            }
            data->fAC *= 0.8f;
            fast_vec_dist(moby->pos, ((char *)&D_0013F3D0));
            FUN_L00_0024f7c8(moby, 0x13, probe);
            fast_vec_sub(ray_start, data->v80, probe);
            fast_vec_normalize(ray_start, ray_start, 30.0f);
            fast_vec_add(ray_start, ray_start, probe);
            fast_vec_sub(offset, ray_start, data->v80);
            distance = fast_vec_dist(ray_start, ((char *)&D_0013F3D0));
            if (distance != 30.0f) {
                fast_vec_normalize(offset,
                                   offset,
                                   (frame_time * 15.0f < distance) ? frame_time * 15.0f : distance);
                fast_vec_add(data->v80, data->v80, offset);
            }
            {
                char *hero = ((char *)&D_0013F350);
                if (*(Moby388 **)(hero + 0x15F0) != 0) {
                    Moby388 *ship_moby = *(Moby388 **)(hero + 0x15F0);
                    char    *ship;
                    float    yaw_error, abs_yaw_error, pitch_error, abs_pitch_error;
                    qcopy(ship_moby->pos, data->v80);
                    ship = (char *)ship_moby->d;
                    yaw_error =
                        fast_subtract_rotations(fast_arc_tan(moby->pos[0] - ship_moby->pos[0],
                                                             moby->pos[1] - ship_moby->pos[1]),
                                                *(float *)(ship + 0x38));
                    abs_yaw_error = absolute_float(yaw_error);
                    pitch_error = fast_subtract_rotations(
                        -fast_arc_tan(
                            vector_distance_xy((*(Moby388 **)(hero + 0x15F0))->pos, moby->pos),
                            moby->pos[2] - (*(Moby388 **)(hero + 0x15F0))->pos[2]),
                        *(float *)(ship + 0x34));
                    abs_pitch_error = absolute_float(pitch_error);
                    if (abs_yaw_error > 0.38397244f) {
                        *(float *)(ship + 0x38) =
                            fast_add_rotations(*(float *)(ship + 0x38),
                                               normalize_angle((abs_yaw_error - 0.38397244f) /
                                                               0.17453292f * yaw_error));
                    }
                    if (abs_pitch_error > 0.38397244f) {
                        *(float *)(ship + 0x34) =
                            fast_add_rotations(*(float *)(ship + 0x34),
                                               normalize_angle((abs_pitch_error - 0.38397244f) /
                                                               0.17453292f * pitch_error));
                    }
                }
            }
        }
        if (moby->bBC != 4) {
            process_moby_attachment_damage(moby, data);
        } else {
            trooper_anim_finish(moby, (char *)data + 0x60);
        }
        {
            char *hero = ((char *)&D_0013F350);
            if (fast_difference_between_rotations(
                    moby->rot[2],
                    fast_arc_tan(*(float *)(hero + 0x80) - moby->pos[0],
                                 *(float *)(hero + 0x84) - moby->pos[1])) < 1.0471976f) {
                add_draw_callback(draw_starfighter_tractor_beam, moby);
            }
        }
        break;
    }
    case STARFIGHTER_STATE_OWNER_UNAVAILABLE: {
        char *dialog;
        if (*(Moby388 **)(((char *)&D_00140940)) != 0 &&
            (*(Moby388 **)(((char *)&D_00140940)))->cls_id == 0x45) {
            (*(Moby388 **)(((char *)&D_00140940)))->bBC = 0;
        }
        advance_moby_along_selected_path(moby, data, data->b13B, 1);
        dialog = (char *)&music_stream_state;
        if (*(int *)(dialog + 0x50) == 0) {
            int line = *(int *)(dialog + 0x1C);
            if (line == -1 && (short)data->h138 <= 0) {
                if (queue_starfighter_voice_line(moby, data, D_L13_00161950 + 0x1D, 1) != 0) {
                    D_L13_00161950 = (D_L13_00161950 + 1) % 4;
                    data->h138 += 1;
                }
            }
        }
        break;
    }
    case STARFIGHTER_STATE_DEATH_FLIGHT: {
        float target_yaw, target_pitch;
        if (*(Moby388 **)(((char *)&D_00140940)) != 0 &&
            (*(Moby388 **)(((char *)&D_00140940)))->cls_id == 0x45) {
            (*(Moby388 **)(((char *)&D_00140940)))->bBC = 0;
        }
        if (data->iF8 == 1) {
            detach_moby_joint_attachment(moby, data, 8);
            allocate_voice_for_target_entry(7, 0, (int)moby);
        }
        fast_dec_timer(&data->iF8);
        if (advance_moby_along_selected_path(moby, data, data->b13B, 0) != 0 ||
            data->s9C >= *(int *)walker_paths[data->paths[data->b13B]] - 1) {
            data->iF8 = scale_game_frames(0xB4);
            moby->state = 8;
        }
        moby->rot[0] = fast_add_rotations(moby->rot[0], frame_time * 4.1887903f);
        {
            Path388 *path = (Path388 *)walker_paths[data->paths[data->b13B]];
            int      i = data->s9C;
            int      n = path->n;
            target_yaw =
                fast_arc_tan(path->pts[(i + 3 >= n - 1) ? n - 1 : i + 3][0] - path->pts[i][0],
                             path->pts[(i + 3 < n - 1) ? i + 3 : n - 1][1] - path->pts[i][1]);
        }
        {
            char *path = walker_paths[data->paths[data->b13B]];
            int   i = data->s9C;
            int   n = *(int *)path;
            float flat_distance = vector_distance_xy(
                path + (i * 16 + 0x10), path + (((i + 3 >= n - 1) ? n - 1 : i + 3) * 16 + 0x10));
            Path388 *path2 = (Path388 *)walker_paths[data->paths[data->b13B]];
            int      i2 = data->s9C;
            int      path_point_count = path2->n;
            target_pitch = -fast_arc_tan(
                flat_distance,
                path2->pts[(i2 + 3 >= path_point_count - 1) ? path_point_count - 1 : i2 + 3][2] -
                    path2->pts[i2][2]);
        }
        FUN_L00_00258278(moby,
                         &data->fA8,
                         target_yaw,
                         frame_time_sq * 3.1415927f,
                         frame_time_sq * 6.2831855f,
                         frame_time * 6.2831855f);
        moby->rot[1] = spring_turn(&data->fA4,
                                   moby->rot[1],
                                   target_pitch,
                                   frame_time_sq * 3.1415927f,
                                   frame_time_sq * 6.2831855f,
                                   frame_time * 6.2831855f);
        moby->scale *= 0.99925f;
        data->fAC *= 0.9992f;
        if (random_integer_below(0x11) == 0) {
            float size_ratio = moby->scale / (*(float *)(moby->cls + 0x24) * 3.0f);
            fast_vec_normalize(joint, moby->mat[0], random_float_between(-5.0f, 5.0f) * size_ratio);
            fast_vec_add(offset, moby->pos, joint);
            fast_vec_normalize(joint, moby->mat[1], random_float_between(-4.0f, 4.0f) * size_ratio);
            fast_vec_add(offset, offset, joint);
            fast_vec_normalize(joint, moby->mat[2], random_float_between(-4.0f, 4.0f) * size_ratio);
            fast_vec_add(offset, offset, joint);
            cart_burst(moby, offset, -1, size_ratio * 1.6f, 13.0f);
        }
        func_L13_002E9F90_2(moby, data);
        {
            char *dialog = (char *)&music_stream_state;
            if (*(int *)(dialog + 0x50) == 0 && *(int *)(dialog + 0x1C) == -1) {
                if (data->iF8 < scale_game_frames(0x1A4) && data->i13C < 0) {
                    if (queue_starfighter_voice_line(moby, data, 0x23, 1) != 0) {
                        data->i13C &= 0x7FFFFFFF;
                    }
                } else if (data->iF8 < scale_game_frames(0xB4) && (data->i13C & 0x40000000) &&
                           queue_starfighter_voice_line(moby, data, 0x24, 1) != 0) {
                    data->i13C &= 0xBFFFFFFF;
                }
            }
        }
        break;
    }
    case STARFIGHTER_STATE_EXPLODE_AND_REMOVE: {
        char *view;
        if (*(Moby388 **)(((char *)&D_00140940)) != 0 &&
            (*(Moby388 **)(((char *)&D_00140940)))->cls_id == 0x45) {
            (*(Moby388 **)(((char *)&D_00140940)))->bBC = 0;
        }
        moby->rot[0] = fast_add_rotations(moby->rot[0], frame_time * 4.1887903f);
        view = jet_camera47;
        moby->rot[2] = *(float *)(view + 0x158);
        moby->rot[1] = *(float *)(view + 0x154);
        moby->scale *= 0.98f;
        if (data->iF8 == scale_game_frames(0x1E)) {
            spawn_explosion(moby,
                            &data->f70,
                            0,
                            0.0f,
                            0.0f,
                            0x1E,
                            8,
                            0x20,
                            5.0f,
                            3.0f,
                            0.0f,
                            7,
                            1.0f,
                            0.0f,
                            1,
                            5,
                            -1,
                            0);
        }
        func_L13_002E9F90_2(moby, data);
        if (fast_dec_timer(&data->iF8) != 0) {
            Moby388 *owner = (Moby388 *)(D_L13_00160058_m + (data->i134 << 8));
            owner->state = 8;
            owner->flags |= 2;
            if (data->light120 != -1) {
                release_point_light_slot(data->light120);
            }
            flag_vendor_item(data->i12C);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case STARFIGHTER_STATE_RESET: {
        char  *path;
        char **parts;
        if (data->pF0 != 0) {
            mark_moby_for_removal(data->pF0);
            data->pF0 = 0;
        }
        if (data->child110 != 0) {
            mark_moby_for_removal(data->child110);
            data->child110 = 0;
        }
        parts = data->parts;
        for (i = 0; i < 10; i++) {
            if (parts[i] != 0) {
                mark_moby_for_removal(parts[i]);
                parts[i] = 0;
            }
        }
        if (data->snd118 != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd118 * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0) {
                release_voice_slot(data->snd118);
            }
            data->snd118 = -1;
        }
        if (data->snd114 != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd114 * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0) {
                release_voice_slot(data->snd114);
            }
            data->snd114 = -1;
        }
        if (data->snd11C != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd11C * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0) {
                release_voice_slot(data->snd11C);
            }
            data->snd11C = -1;
        }
        if (data->light120 != -1) {
            release_point_light_slot(data->light120);
        }
        if (data->hud104 != -1) {
            set_animation_parameter(data->hud104, 0);
            data->hud104 = -1;
        }
        path = walker_paths[data->paths[0]];
        qcopy(moby->pos, path + 0x10);
        moby->rot[2] = fast_arc_tan(*(float *)(path + 0x20) - moby->pos[0],
                                    *(float *)(path + 0x24) - moby->pos[1]);
        data->b10C = 1;
        data->s9C = 0;
        data->s9E = 0;
        data->iA0 = 0;
        data->fA4 = 0;
        data->fA8 = 0;
        data->fAC = 0;
        data->iF4 = 0;
        data->iF8 = 0;
        data->bFC = 0;
        data->bFD = 0;
        data->bFE = 0;
        data->bFF = 0;
        data->i100 = 0;
        data->b10D = 0;
        data->b10E = 0;
        data->b10F = 0;
        data->i124 = 0;
        data->h138 = 0;
        data->b13A = 0;
        data->b13B = 0;
        data->i13C = 0;
        data->h144 = 0;
        data->h146 = 0;
        data->b148 = 0;
        moby->bBC = 0;
        qzero(&data->f70);
        qzero(data->v80);
        moby->state = 0;
        return;
    }
    }

    if (moby->pos[0] < 22.0f)
        moby->pos[0] = 22.0f;
    if (moby->pos[0] > 1001.0f)
        moby->pos[0] = 1001.0f;
    if (moby->pos[1] < 22.0f)
        moby->pos[1] = 22.0f;
    if (moby->pos[1] > 1001.0f)
        moby->pos[1] = 1001.0f;
    if (moby->pos[2] < 22.0f)
        moby->pos[2] = 22.0f;
    if (moby->pos[2] > 1001.0f)
        moby->pos[2] = 1001.0f;

    if (moby->bBC == 4) {
        if (data->child110 == 0) {
            data->child110 = spawn_owner_linked_class_160_moby(moby);
        } else if (((Moby388 *)data->child110)->state >= 3) {
            ((Moby388 *)data->child110)->state = 1;
        }
        if (data->snd118 != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd118 * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0)
                goto have118;
        }
        data->snd118 = -1;
        data->snd118 = allocate_voice_for_target_entry(6, 4, (int)moby);
    have118:;
    } else {
        if (data->child110 != 0) {
            Moby388 *child;
            if (data->snd118 != -1) {
                char *voice = D_0013E633_snd + 0x1D + data->snd118 * 0x70;
                if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0) {
                    release_voice_slot(data->snd118);
                }
                data->snd118 = -1;
            }
            child = (Moby388 *)data->child110;
            if (child == 0) {
                data->child110 = 0;
            } else if (child->cls_id != 0x160) {
                data->child110 = 0;
            } else if (child->state == 0xFE) {
                data->child110 = 0;
            } else if (child->state == 0xFD) {
                data->child110 = 0;
            } else if (child->state < 3) {
                child->state = 3;
            }
        }
    }

    if (moby->state >= 2) {
        update_moby_joint_attachments(moby, data);
        if (moby->state != 8 || scale_game_frames(0x78) < data->iF8) {
            emit_gated_moby_joint_effects(moby, data);
        }
        if (data->snd114 != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd114 * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0)
                goto have114;
        }
        data->snd114 = -1;
        data->snd114 = allocate_voice_for_target_entry(0, 4, (int)moby);
    have114:
        transform_vector_by_basis(offset, D_L13_00160660, moby->mat[0]);
        fast_vec_normalize(offset, offset, 7.0f);
        fast_vec_sub(offset, moby->pos, offset);
        offset[3] = 50.0f;
        if (data->light120 == -1) {
            data->light120 = allocate_colored_effect_slot(offset, 50.0f, 0.0f, 3.0f, 0.5f, 0.01f);
        } else {
            char *light = level13_point_light_slots + (data->light120 << 5);
            qcopy(light + 0x10, offset);
        }
    }
    data->b10F = 0;

    {
        Moby388 *owner = (Moby388 *)(D_L13_00160058_m + (data->i134 << 8));
        if (moby->state != 6) {
            if (owner == 0 || owner->cls_id != 0x45 || owner->state == 0xFE ||
                owner->state == 0xFD || owner->state == 7) {
                moby->state = 6;
                if (queue_starfighter_voice_line(moby, data, D_L13_00161950 + 0x1D, 1) != 0) {
                    D_L13_00161950 = (D_L13_00161950 + 1) % 4;
                    data->h138 = 1;
                } else {
                    data->h138 = 0;
                }
            }
        }
    }

    if (data->b13A != 0) {
        if (dialogue_playback_phase[0] == 3) {
            continue_audio_stream_if_ready();
            data->h144 = scale_game_frames(0x168);
            data->h146 = scale_game_frames(0x3C);
            data->b13A = 0;
        }
    } else {
        if (moby->state != 6) {
            char *hero = ((char *)&D_0013F350);
            if (*(int *)(hero + 0x2084) == 0x32) {
                if (*(Moby388 **)(hero + 0x15F0) != 0 &&
                    (*(Moby388 **)(hero + 0x15F0))->cls_id == 0x45 &&
                    (unsigned int)(*(int *)((char *)(*(Moby388 **)(hero + 0x15F0))->d + 0x120) -
                                   1) < 0x270F &&
                    random_integer_below(2) == 0) {
                    queue_starfighter_voice_line(moby, data, 3, 9);
                }
            }
        }
    }
    fast_dec_timer_s16(&data->h144);
    fast_dec_timer_s16(&data->h146);
    if (moby->state >= 2 && moby->state <= 5 && (((unsigned char *)D_0013E533)[0x242C] & 1)) {
        moby->state = 9;
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_002EE148), where it is exact; names translated to the US level program. */

extern char *create_moby(int);
extern char *create_moby(int) __asm__("FUN_0020c4f8");

char *spawn_moby_joint_attachment(char *parent_moby, int attachment_joint_index, int id) {
    char *moby = create_moby(id);
    if (moby != 0) {
        char *data;
        float f = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * MOBY(parent_moby)->scale /
                  *(float *)(((char *)MOBY(parent_moby)->pclass) + 0x24);
        data = ((char *)MOBY(moby)->pvars);
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        MOBY(moby)->scale = f;
        *(int *)(data + 0x44) = attachment_joint_index;
        moby_point_position(parent_moby, attachment_joint_index, moby + 0x10);
        *(char **)(data + 0x3C) = parent_moby;
        update_entity(moby);
        qcopy(moby + 0x40, parent_moby + 0x40);
        *(OvlQuad *)&MOBY(moby)->rot_row0 = *(OvlQuad *)&MOBY(parent_moby)->rot_row0;
        *(OvlQuad *)&MOBY(moby)->rot_row1 = *(OvlQuad *)&MOBY(parent_moby)->rot_row1;
        *(OvlQuad *)&MOBY(moby)->rot_row2 = *(OvlQuad *)&MOBY(parent_moby)->rot_row2;
        *(int *)(data + 0x40) = scale_game_frames(0x258);
        update_entity(moby);
    }
    return moby;
}

extern int  coll_sphere(void *, float, int, struct Moby *) __asm__("FUN_L00_001f0d60");
extern char coll_hit_1638[] __asm__("D_L13_00174140");

struct PieceVars {
    u8           pad0[0x10];
    Vec4f        vel;
    u128         unk20;
    f32          spin[3];
    struct Moby *parent;
    int          timer;
    int          point;
};

void update_qwarks_starfighter_part(struct Moby *moby) __asm__("FUN_L13_002ece00");

void update_qwarks_starfighter_part(struct Moby *moby) {
    enum {
        PART_STATE_INITIALIZE = 0,
        PART_STATE_SYNC_PARENT_SCALE = 1,
        PART_STATE_IDLE = 2,
        PART_STATE_MOVE_AND_SPIN = 3,
        PART_STATE_BURST_AND_REMOVE = 4
    };
    struct PieceVars *vars = (struct PieceVars *)moby->pvars;
    struct Moby      *parent = vars->parent;
    struct Moby      *hit;
    char             *coll;
    if (parent == 0 || parent->oclass != 0x184 || parent->state == 0xFE || parent->state == 0xFD) {
        goto remove;
    }
    moby->flags = (moby->flags & 0xFFFC);
    moby->flags = moby->flags | (vars->parent->flags & 3);
    switch (moby->state) {
    case PART_STATE_INITIALIZE:
        moby->state = 1;
        qzero(&vars->unk20);
        break;
    case PART_STATE_SYNC_PARENT_SCALE:
        moby->scale = moby->pclass->scale * vars->parent->scale / vars->parent->pclass->scale;
        moby->spawn_frame = vars->parent->spawn_frame;
        break;
    case PART_STATE_IDLE:
        break;
    case PART_STATE_MOVE_AND_SPIN:
        fast_vec_add(&moby->pos, &moby->pos, &vars->vel);
        fast_vec_scale(&vars->vel, &vars->vel, 0.995f);
        if (coll_sphere(&moby->pos, 2.0f, 0, moby) != 0) {
            coll = coll_hit_1638;
            hit = *(struct Moby **)(coll + 0x18);
            if (hit == 0 ||
                (hit->oclass != 0x191 && hit->oclass != 0x184 && hit->oclass != 0x185 &&
                 hit->oclass != 0x186 && hit->oclass != 0x187 && hit->oclass != 0x188 &&
                 hit->oclass != 0x189 && hit->oclass != 0x18A && hit->oclass != 0x18B &&
                 hit->oclass != 0x18C && hit->oclass != 0x18F && hit->oclass != 0x190)) {
                reflect_vector(&vars->vel, &vars->vel, coll + 0x40);
                moby->state = 4;
            }
        }
        moby->rot.x = fast_add_rotations(moby->rot.x, vars->spin[0]);
        moby->rot.y = fast_add_rotations(moby->rot.y, vars->spin[1]);
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->spin[2]);
        vars->spin[0] *= 0.999f;
        vars->spin[1] *= 0.999f;
        vars->spin[2] *= 0.999f;
        if (moby->pos.x < 10.0f || 1013.0f < moby->pos.x || moby->pos.y < 10.0f ||
            1013.0f < moby->pos.y || moby->pos.z < 10.0f || 1013.0f < moby->pos.z) {
            goto remove;
        }
        if (fast_dec_timer(&vars->timer) == 0 || moby->visible != 0) {
            break;
        }
    remove:
        mark_moby_for_removal(moby);
        break;
    case PART_STATE_BURST_AND_REMOVE:
        spawn_explosion(moby,
                        &vars->vel,
                        0,
                        2.0f,
                        4.0f,
                        10,
                        3,
                        16,
                        4.0f,
                        2.0f,
                        9.0f,
                        -1,
                        1.0f,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        break;
    }
}

#include "types.h"

extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void spawn_nearby_effect_without_direction(float, void *, void *, int, void *) __asm__("FUN_L10_0024c7f0");
extern void *memcpy(void *, const void *, unsigned int);

extern void *sphere_hits_ed158[] __asm__("D_L13_00178200");

void spawn_forcefield_contact_effects(void *moby, void *data) __asm__("FUN_L13_002ed158");

void spawn_forcefield_contact_effects(void *moby, void *data) {
    float  matrix[12];
    float  query[4];
    float  candidate_pos[4];
    float  local_pos[4];
    float  rotation_matrix[16];
    float  probe[4];
    void **candidates;
    int    count, hit_count, i, j;
    char  *candidate;
    short  half_width, half_height, max_size;
    float  radius;
    float  limit, tolerance, height_limit, height_tolerance;

    euler_rotation_basis(matrix, moby + 0x40);
    max_size =
        *(short *)(data + 10) < *(short *)(data + 8) ? *(short *)(data + 8) : *(short *)(data + 10);
    radius = (float)max_size * 0.5f;
    qzero(query);
    query[2] = radius;
    transform_vector_by_basis(query, query, matrix);
    fast_vec_add(query, query, moby + 0x10);
    count = collect_mobies_in_sphere(query, radius, 0x10, moby, 0);
    if (count == 0)
        return;
    candidates = __builtin_alloca(((unsigned)count << 5) >> 3);
    memcpy(candidates, sphere_hits_ed158, count * 4);
    for (i = 0; i < count; i++) {
        candidate = *(void **)((u32)candidates + (i << 2));
        if (candidate == 0 || (unsigned char)candidate[0x20] == 0xfe ||
            (unsigned char)candidate[0x20] == 0xfd)
            continue;
        fast_vec_sub(candidate_pos, candidate + 0x10, moby + 0x10);
        transpose_rotation_matrix(rotation_matrix, matrix);
        transform_vector(local_pos, candidate_pos, rotation_matrix);
        qcopy(probe, local_pos);
        half_width = *(short *)(data + 8);
        limit = (float)half_width * 0.5f;
        tolerance = limit + 0.7f;
        if (probe[0] < -tolerance)
            probe[0] = (float)-half_width * 0.5f;
        else if (tolerance < probe[0])
            probe[0] = limit;
        half_height = *(short *)(data + 10);
        height_limit = (float)half_height * 0.5f;
        height_tolerance = height_limit + 1.5f;
        if (probe[2] < -height_tolerance)
            probe[2] = (float)-half_height * 0.5f;
        else if (height_tolerance < probe[2])
            probe[2] = height_limit;
        probe[1] = 0.0f;
        transform_vector_by_basis(probe, probe, matrix);
        fast_vec_add(probe, probe, moby + 0x10);
        hit_count = collect_mobies_in_sphere(probe, 0.7f, 0x10, moby, 0);
        for (j = 0; j < hit_count; j++) {
            if (sphere_hits_ed158[j] == candidate)
                spawn_nearby_effect_without_direction(1.0f, candidate, moby, 0x10001, probe);
        }
    }
}

/* Fades the moby in, waits for its linked moby to be collected or gone, fades out and kills it; spins it all the while. */
extern int  D_0015ED84_n __asm__("D_0015ED84");
extern void stub_printf(char *, int, int) __asm__("FUN_001e93b0");
extern void spawn_forcefield_contact_effects(void *, void *) __asm__("FUN_L13_002ed158");
extern void offset_moby_class_point_pairs(void *, int, int) __asm__("FUN_L00_0024efb0");

extern char forcefield_missing_trigger_format[] __asm__("D_L13_001F4FB0");
void        forcefield_update(struct Moby *m) __asm__("FUN_L13_002ed4a8");

void forcefield_update(struct Moby *moby) {
    enum {
        FORCEFIELD_STATE_INITIALIZE = 0,
        FORCEFIELD_STATE_MONITOR_LINKED_MOBY = 1,
        FORCEFIELD_STATE_FADE_OUT = 2,
        FORCEFIELD_STATE_REMOVE = 3
    };
    char *forcefield_data = ((char *)moby->pvars);
    int   wrap_adjusted_step;
    switch (moby->state) {
    case FORCEFIELD_STATE_INITIALIZE:
        moby->state = 1;
        moby->alpha = 0x14;
        moby->flags |= 0xA08;
        *(short *)(forcefield_data + 0xE) = -1;
        *(float *)(forcefield_data + 4) = 1.0f;
        set_moby_color(moby, 0x80, 0x80, 0x80);
        break;
    case FORCEFIELD_STATE_MONITOR_LINKED_MOBY:
        if (*(int *)forcefield_data != -1) {
            unsigned char *linked_moby =
                (unsigned char *)(moby_base + (*(int *)forcefield_data << 8));
            if (linked_moby == 0 || linked_moby[0x20] == 0xFE || linked_moby[0x20] == 0xFD ||
                (D_0014C190_257218[D_0015ED84_n][((short)MOBY(linked_moby)->save_id) >> 5] >>
                 (MOBY(linked_moby)->save_id & 0x1F)) &
                    1) {
                *(float *)(forcefield_data + 4) = 1.0f;
                moby->state = 2;
            }
        } else {
            *(float *)(forcefield_data + 4) = 1.0f;
            moby->state = 2;
            stub_printf(forcefield_missing_trigger_format, moby->oclass, ((short)moby->save_id));
        }
        spawn_forcefield_contact_effects(moby, forcefield_data);
        break;
    case FORCEFIELD_STATE_FADE_OUT:
        *(float *)(forcefield_data + 4) -= frame_scale * 0.05f;
        if (*(float *)(forcefield_data + 4) < 0.0f)
            moby->state = 3;
        else
            moby->alpha = truncate_float_to_s32(*(float *)(forcefield_data + 4) * 20.0f);
        break;
    case FORCEFIELD_STATE_REMOVE:
        if (*(short *)(forcefield_data + 0xE) != -1) {
            release_point_light_slot(*(short *)(forcefield_data + 0xE));
            *(short *)(forcefield_data + 0xE) = -1;
        }
        mark_moby_for_removal(moby);
        return;
    }
    wrap_adjusted_step = 0xC0;
    *(short *)(forcefield_data + 0xC) += 0xC0;
    if (*(short *)(forcefield_data + 0xC) > 0x1000) {
        *(short *)(forcefield_data + 0xC) -= 0x1000;
        wrap_adjusted_step = -0xF40;
    } else if (*(short *)(forcefield_data + 0xC) < 0) {
        *(short *)(forcefield_data + 0xC) += 0x1000;
        wrap_adjusted_step = 0x10C0;
    }
    offset_moby_class_point_pairs(((void *)moby->pclass), wrap_adjusted_step, 0);
}

/* Clear a vendor moby's field and set its flags unless it is the current target. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_002F4BE8), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x34];
    unsigned short flags;
    char           pad36[0x5E];
    int            field94;
} Level13VendorMoby;

void purple_ship_update(Level13VendorMoby *moby) __asm__("FUN_L13_002f3750");

void purple_ship_update(Level13VendorMoby *moby) {
    if (*(void **)(((char *)&level_render_state)) != moby) {
        moby->field94 = 0;
        moby->flags |= 3;
    }
}

/* Per-frame update: awards a stat/flag when its linked moby is gone, then sets a flag byte from a test. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_002F4C10), where it is exact; names translated to the US level program. */

extern int           help_request_state[] __asm__("D_L13_00179710");
extern int           scale_game_frames(int);
extern unsigned char menu_item_enabled_115[] __asm__("D_0013D407");
extern char          D_00141968[];
extern char          statistics_level_flags[] __asm__("D_00141CE4");

void update_linked_moby_statistics_and_hero_volume_flag(struct Moby *moby) __asm__("FUN_L13_002f3778");

void update_linked_moby_statistics_and_hero_volume_flag(struct Moby *moby) {
    char          *data;
    unsigned char *menu_item_enable_base;
    moby->update_dist = 0xFF;
    data = (char *)moby->pvars;
    if (help_request_state[0] == 0 && help_request_state[9] == -1) {
        unsigned int hero_control_mode = *(unsigned int *)(&hero.state.control_mode);
        if ((hero_control_mode < 2 || hero_control_mode == 9) ? D_L13_0015F5C4 == 0 : 0) {
            int   linked_moby_index = *(int *)(data + 4);
            char *linked_moby;
            if (linked_moby_index != -1) {
                linked_moby = moby_base + (linked_moby_index << 8);
                if (linked_moby != 0 && MOBY(linked_moby)->oclass == 0xAA) {
                    int linked_moby_state = (unsigned char)linked_moby[0x20];
                    if (linked_moby_state == 0xFE)
                        goto join;
                    if (linked_moby_state != 0xFD)
                        goto direct;
                }
            }
        join:
            menu_item_enable_base = menu_item_enabled_115 + 0xB9;
            if (menu_item_enable_base[0xB] != 0 || menu_item_enable_base[0xD] != 0) {
                unsigned char *statistics_base = ((char *)&D_00141968);
                unsigned int   saturating_count = *(unsigned short *)(statistics_base + 0x378);
                if (saturating_count <= 0xFFFE) {
                    *(unsigned short *)(statistics_base + 0x378) = saturating_count + 1;
                }
                if (scale_game_frames(D_0015EEA4) / 600 >
                    *(unsigned short *)(statistics_base + 0x37A)) {
                    *(unsigned short *)(statistics_base + 0x37A) =
                        scale_game_frames(D_0015EEA4) / 600;
                }
                *(unsigned int *)(statistics_base + 0x37C) =
                    *(unsigned int *)(statistics_base + 0x37C) | (1 << current_level_index) |
                    0x80000000;
            } else {
            direct:
                if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)data) != 0) {
                    if (*(int *)(((char *)&statistics_level_flags)) >= 0) {
                        help_request(0x32C8, 0x6F);
                    }
                }
            }
        }
    }
    if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 8)) != 0) {
        hero.vent_updraft = 0;
    } else {
        hero.vent_updraft = 1;
    }
}
/* State machine: waits for a linked moby to reach one of a set of states, then pushes states onto linked mobys. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_002F9D18), where it is exact; names translated to the US level program. */

typedef struct {
    char  v[16];
    char  padv[16];
    float f20;
    char  pad24[4];
    float f28;
    char  pad2C[4];
    int   f30;
    int   f34;
} Child_q;

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child_q       *child;
    char           pad7C[0x14];
    int            f90;
} Obj_q;

extern int debug_print_alt_q() __asm__("FUN_001e93b0");

extern char D_L13_001F51B0[] NOT_SDA;
void        update_moby_state_relay(struct Moby *moby) __asm__("FUN_L13_002f8880");

void update_moby_state_relay(struct Moby *moby) {
    enum {
        STATE_RELAY_INITIALIZE = 0,
        STATE_RELAY_WAIT_FOR_SECOND_STATE_SET = 1,
        STATE_RELAY_WAIT_FOR_FIRST_STATE_SET = 2
    };
    char        *data = ((char *)moby->pvars);
    struct Moby *monitored_moby = 0;
    int          monitored_moby_index = *(int *)data;
    int          found;
    int          i;
    if (monitored_moby_index != -1) {
        monitored_moby = moby_base + (monitored_moby_index << 8);
    }
    switch (data[0x1C]) {
    case STATE_RELAY_INITIALIZE:
        if (monitored_moby == 0) {
            debug_print_alt_q(D_L13_001F51B0, ((short)moby->save_id), moby->oclass);
            mark_moby_for_removal(moby);
        } else if (data[0x1D] != 0) {
            data[0x1C] = 1;
        } else {
            data[0x1C] = 2;
        }
        break;
    case STATE_RELAY_WAIT_FOR_SECOND_STATE_SET:
        found = 0;
        for (i = 0; i < 6; i++) {
            if ((unsigned char)((char)monitored_moby->state) == (data + i)[0xA]) {
                found = 1;
                break;
            }
        }
        if (found) {
            int idx2 = *(int *)(data + 0x10);
            if (idx2 != -1) {
                char *o = *(char **)&moby_base + (idx2 << 8);
                if (data[0x15] != -1)
                    o[0x20] = data[0x15];
                if (data[0x17] != -1)
                    o[0xBC] = data[0x17];
            }
            if (data[0x19] != -1)
                moby->state = data[0x19];
            if (data[0x1B] != -1)
                moby->command = data[0x1B];
            data[0x1C] = 2;
        }
        break;
    case STATE_RELAY_WAIT_FOR_FIRST_STATE_SET:
        found = 0;
        for (i = 0; i < 6; i++) {
            if ((unsigned char)((char)monitored_moby->state) == (data + i)[4]) {
                found = 1;
                break;
            }
        }
        if (found) {
            int idx2 = *(int *)(data + 0x10);
            if (idx2 != -1) {
                char *o = *(char **)&moby_base + (idx2 << 8);
                if (data[0x14] != -1)
                    o[0x20] = data[0x14];
                if (data[0x16] != -1)
                    o[0xBC] = data[0x16];
            }
            if (data[0x18] != -1)
                moby->state = data[0x18];
            if (data[0x1A] != -1)
                moby->command = data[0x1A];
            data[0x1C] = 1;
        }
        break;
    }
}

/* Gate that follows its controller moby: opens when the controller is in one of its six "open" states,
 * closes on one of its six "close" states, fading its light with the opening and spinning its wheel. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_002F9F10), where it is exact; names translated to the US level program. */

extern char gate_missing_controller_format[] __asm__("D_L13_001F51E0");
extern int  D_L13_0015FFD8; /* no foreign declaration */
extern int  debug_print_alt() __asm__("FUN_001e93b0");

void update_controller_linked_gate(struct Moby *moby) __asm__("FUN_L13_002f8a78");

void update_controller_linked_gate(struct Moby *moby) {
    enum {
        GATE_STATE_INITIALIZE = 0,
        GATE_STATE_OPEN = 1,
        GATE_STATE_CLOSING = 2,
        GATE_STATE_CLOSED = 3,
        GATE_STATE_OPENING = 4
    };
    char *gate_data = ((char *)moby->pvars);
    char *controller_moby = 0;
    int   open;
    int   close;
    int   step;
    if (*(int *)gate_data != -1) {
        controller_moby = D_L13_0015FFD8 + (*(int *)gate_data << 8);
    }
    open = 0;
    close = 0;
    if (((unsigned char *)moby)[0x20] != 0) {
        int i;
        for (i = 0; i < 6; i++) {
            if (((unsigned char *)controller_moby)[0x20] == (gate_data + i)[4]) {
                open = 1;
                break;
            }
        }
        for (i = 0; i < 6; i++) {
            if (((unsigned char *)controller_moby)[0x20] == (gate_data + i)[0xA]) {
                close = 1;
                break;
            }
        }
    }
    switch (((unsigned char *)moby)[0x20]) {
    case GATE_STATE_INITIALIZE:
        if (controller_moby == 0) {
            debug_print_alt(gate_missing_controller_format, ((short)moby->save_id), moby->oclass);
            mark_moby_for_removal(moby);
            return;
        }
        set_moby_color(moby, 0x80, 0x80, 0x80);
        moby->flags |= 0xA08;
        if (*(int *)(gate_data + 0x18) == 0) {
            *(float *)(gate_data + 0x10) = 1.0f;
            moby->state = 1;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
            moby->alpha = 0x14;
        } else {
            *(float *)(gate_data + 0x10) = 0.0f;
            moby->state = 3;
            moby->collision = 0;
            moby->alpha = 0;
        }
        break;
    case GATE_STATE_OPEN:
        if (close) {
            moby->collision = 0;
            moby->state = 2;
        }
        break;
    case GATE_STATE_CLOSING:
        if (open) {
            moby->state = 4;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
        }
        *(float *)(gate_data + 0x10) -= frame_scale * 0.05f;
        if (*(float *)(gate_data + 0x10) < 0.0f) {
            *(float *)(gate_data + 0x10) = 0.0f;
            moby->state = 3;
            moby->collision = 0;
            moby->visible = 0;
            moby->flags |= 1;
        }
        break;
    case GATE_STATE_CLOSED:
        if (open) {
            moby->state = 4;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
            moby->visible = 1;
            moby->flags &= 0xFFFE;
        }
        break;
    case GATE_STATE_OPENING:
        if (close) {
            moby->collision = 0;
            moby->state = 2;
        }
        *(float *)(gate_data + 0x10) += frame_scale * 0.05f;
        if (1.0f < *(float *)(gate_data + 0x10)) {
            *(float *)(gate_data + 0x10) = 1.0f;
            moby->state = 1;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
        }
        break;
    }
    moby->alpha = truncate_float_to_s32(*(float *)(gate_data + 0x10) * 20.0f);
    step = 0xC0;
    *(int *)(gate_data + 0x14) += step;
    if (*(int *)(gate_data + 0x14) > 0x1000) {
        *(int *)(gate_data + 0x14) -= 0x1000;
        step -= 0x1000;
    } else if (*(int *)(gate_data + 0x14) < 0) {
        *(int *)(gate_data + 0x14) += 0x1000;
        step += 0x1000;
    }
    offset_moby_class_point_pairs(((unsigned char *)moby->pclass), step, 0);
}

char D_L13_001F4F38[] = "tractor too long, abort\n";
