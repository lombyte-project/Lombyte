/* Level 11 moby classes: oil_tanker_front, energy_sphere, blarg_ship. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"
#include "rnc/gameplay/camera/camera_globals.h"
extern float D_0015ED6C_n[] __asm__("D_0015ED68") __attribute__((section(".sdata")));

extern char *D_L11_0015FFD8;
extern char *path_table[] __asm__("D_L11_001B0EB0");
extern float D_L11_001622CC __attribute__((sda));
extern float D_L11_001622D0 __attribute__((sda));
extern float D_L11_001622D4 __attribute__((sda));
extern float D_L11_001622D8 __attribute__((sda));
extern float D_L11_001622DC __attribute__((sda));
extern float D_L11_001622E0 __attribute__((sda));
extern float D_L11_001622FC_gp __asm__("D_L11_001622FC") __attribute__((sda));

/* Private variables of the moby FUN_L11_00315968 updates (only the fields it uses). */
struct L11PatrolVars {
    u8 pad0[0x20];
    f32 hp;                 /* 0x20 */
    u8 pad24[0x14];
    s32 retimer;            /* 0x38: nonzero restarts the timer at 0x244 */
    u8 pad3C[0xD4];
    u8 move[7];             /* 0x110: movement state (trooper_anim_update / trooper_anim_finish) */
    u8 unk117;              /* 0x117 */
    u8 pad118[8];
    u8 unk120[0x10];        /* 0x120 */
    f32 unk130;             /* 0x130 */
    f32 unk134;             /* 0x134 */
    f32 unk138;             /* 0x138 */
    f32 unk13C;             /* 0x13C */
    s32 unk140;             /* 0x140 */
    s32 unk144;             /* 0x144 */
    f32 unk148;             /* 0x148 */
    u8 pad14C[0x11];
    u8 unk15D;              /* 0x15D */
    u8 pad15E[0x12];
    f32 unk170;             /* 0x170 */
    f32 unk174;             /* 0x174 */
    u8 pad178[0x2C];
    f32 unk1A4;             /* 0x1A4 */
    u8 pad1A8[0x28];
    Vec4f goal;             /* 0x1D0 */
    u8 pad1E0[0x30];
    struct Moby *target;    /* 0x210 */
    s32 unk214;             /* 0x214 */
    u8 pad218[0x18];
    s32 path_index;         /* 0x230 */
    s32 path2_index;        /* 0x234 */
    u8 pad238[4];
    f32 range;              /* 0x23C */
    u8 pad240[4];
    s32 timer;              /* 0x244 */
    f32 reach;              /* 0x248 */
    u8 pad24C[8];
    s32 carrier;            /* 0x254: listed moby index, -1: none */
};

/* What find_moby_hit reports about the moby's last hit. */
struct L11HitInfo {
    u8 pad0[0x10];
    Vec4 pos;           /* 0x10 */
    struct Moby *moby;  /* 0x20: the moby that hit */
};

extern struct L11HitInfo *FUN_L00_0025a420_h(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern s32 fun_00213928(void *, void *, void *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_00213928");
extern void FUN_L00_0025f3e8(void *, void *, s32, f32, f32);
extern s32 is_moby_in_state_five(void *) __asm__("FUN_L11_0030a830");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern s32 FUN_001fa6d0(float);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float vector_distance(float *, float *) __asm__("FUN_001f9b80");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(void *, void *, s32, s32, s32, float) __asm__("FUN_L00_0025c558");
extern void FUN_L00_00257470(void *, s32, s32);
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_00212f90(void *, s32, s32, s32);
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern float random_float_between(float a, float b) __asm__("FUN_002132a8");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern s32 fast_dec_timer(void *) __asm__("FUN_001f9740");
extern s32 FUN_L00_0025ff38(void *, void *, s32, s32, void *, s32, float);

/* Path patroller update: rides its carrier moby until dropped, takes hits (hit and death states), and follows its D_L11_001B0EB0 path toward the hero. */
void update_puffoid_damage_and_path_goal(struct Moby *moby) __asm__("FUN_L11_00315968");

void update_puffoid_damage_and_path_goal(struct Moby *moby) {
    enum {
        HIT_RESPONSE_NO_ADDITIONAL_REACTION = 0,
        HIT_RESPONSE_DEATH = 1,
        HIT_RESPONSE_DEATH_KNOCKBACK = 2,
        HIT_RESPONSE_KNOCKBACK = 3,
        HIT_RESPONSE_KNOCKBACK_ALTERNATE = 4,
        HIT_RESPONSE_KNOCKBACK_VARIANT = 5,
        HIT_RESPONSE_KNOCKBACK_REACTION = 6,
        HIT_RESPONSE_KNOCKBACK_ARC = 7,
        HIT_RESPONSE_KNOCKBACK_MOTION = 8,
        HIT_RESPONSE_NON_KNOCKBACK_REACTION = 9,
        HIT_RESPONSE_NON_KNOCKBACK_REACTION_ALTERNATE = 10,
        HIT_RESPONSE_COOLDOWN_NO_KNOCKBACK = 11
    };
    struct L11PatrolVars *patrol_vars = (struct L11PatrolVars *)moby->pvars;
    Vec4 position_scratch;
    s32 out;
    float dmg;
    struct L11HitInfo *hit;
    char *path;

    moby->scale = moby->pclass->scale * D_L11_001622CC;
    patrol_vars->unk1A4 = D_L11_001622FC_gp * frame_time;
    if (patrol_vars->carrier != -1 && moby->state != 0xB && moby->state != 9 && moby->state != 0x12 &&
        moby->state != 0x11 && moby->state != 0 && moby->state != 0xC && moby->state != 0xD) {
        if (moby->pos.z < ((struct Moby *)D_L11_0015FFD8)[patrol_vars->carrier].pos.z + 0.5f) {
            FUN_L00_0025f3e8(moby, &moby->pos, -1, 0.5f, 10.0f);
            mark_moby_for_removal(moby);
        }
    }
    if (moby->state == 0xC && patrol_vars->carrier != -1) {
        if (is_moby_in_state_five(&((struct Moby *)D_L11_0015FFD8)[patrol_vars->carrier]) != 0 && moby->visible == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        if (((struct Moby *)D_L11_0015FFD8)[patrol_vars->carrier].state >= 2) {
            moby->visible = 1;
            moby->flags &= ~1;
            moby->collision = moby->pclass->collision;
        }
    }
    dmg = 0.0f;
    hit = FUN_L00_0025a420_h(moby, 0x330000, 0);
    {
        s32 kind = fun_00213928(moby, hit, &patrol_vars->hp, 0, &out, &dmg, 0, 4);
        if (out != 1 && moby->state != 0xB) {
            patrol_vars->hp -= dmg;
            if (hit->moby->oclass == 0x47) {
                kind = 3;
            }
            if (patrol_vars->hp <= 0.0f) {
                kind = 1;
            }
            patrol_vars->unk140 = FUN_001fa6d0(512.0f);
            patrol_vars->unk148 = 0.2f;
            patrol_vars->unk134 = 0.0f;
            patrol_vars->unk130 = D_L11_001622D0 * frame_time_sq;
            patrol_vars->unk144 = 9;
            patrol_vars->unk15D = 0;
            switch (kind) {
            case HIT_RESPONSE_NO_ADDITIONAL_REACTION:
                break;
            case HIT_RESPONSE_NON_KNOCKBACK_REACTION:
            case HIT_RESPONSE_NON_KNOCKBACK_REACTION_ALTERNATE:
                patrol_vars->unk117 = 0xFA;
                break;
            case HIT_RESPONSE_KNOCKBACK:
            case HIT_RESPONSE_KNOCKBACK_ALTERNATE:
            case HIT_RESPONSE_KNOCKBACK_VARIANT:
            case HIT_RESPONSE_KNOCKBACK_REACTION:
            case HIT_RESPONSE_KNOCKBACK_ARC:
            case HIT_RESPONSE_KNOCKBACK_MOTION: {
                float yaw;
                patrol_vars->unk138 = D_L11_001622D8 * frame_time;
                patrol_vars->unk13C = D_L11_001622D4 * frame_time;
                patrol_vars->unk170 = 7.5f;
                patrol_vars->unk174 = 11.0f;
                yaw = fast_arc_tan(moby->pos.x - hit->moby->pos.x, moby->pos.y - hit->moby->pos.y);
                position_scratch.q = hit->pos.q;
                trooper_knockback_arc(&position_scratch, &yaw, &patrol_vars->unk138, &patrol_vars->unk13C);
                trooper_start_knockback(moby, patrol_vars->unk120, 3, 1, 0, yaw);
                if (moby->state < 0xC) {
                    moby->state = 9;
                } else {
                    moby->state = 0x11;
                }
                patrol_vars->unk117 = 0x78;
                break;
            }
            case HIT_RESPONSE_DEATH:
            case HIT_RESPONSE_DEATH_KNOCKBACK: {
                float yaw;
                moby->flags &= ~0x1000;
                patrol_vars->unk130 = D_L11_001622D0 * frame_time_sq;
                patrol_vars->unk138 = D_L11_001622E0 * frame_time;
                patrol_vars->unk13C = D_L11_001622DC * frame_time;
                patrol_vars->unk170 = 7.5f;
                patrol_vars->unk174 = 11.0f;
                yaw = fast_arc_tan(moby->pos.x - hit->moby->pos.x, moby->pos.y - hit->moby->pos.y);
                position_scratch.q = hit->pos.q;
                trooper_knockback_arc(&position_scratch, &yaw, &patrol_vars->unk138, &patrol_vars->unk13C);
                trooper_start_knockback(moby, patrol_vars->unk120, 3, 1, 0, yaw);
                if (moby->state < 0xC) {
                    moby->state = 0xB;
                } else {
                    moby->state = 0x12;
                }
                patrol_vars->unk117 = 0xF0;
                FUN_L00_00257470(moby, 0, -1);
                break;
            }
            case HIT_RESPONSE_COOLDOWN_NO_KNOCKBACK:
                break;
            }
            trooper_anim_update(moby, patrol_vars->move);
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, patrol_vars->move);
    if (patrol_vars->timer != 0) {
        if (moby->state == 2 || (moby->state == 3 && (moby->anim_flags & 2))) {
            moby->state = 0xA;
            if (moby->prev_seq != 2) {
                FUN_00212f90(moby, 2, 0, scale_game_frames(10));
            }
        }
    }
    if (patrol_vars->retimer != 0) {
        patrol_vars->timer = FUN_001fa6d0(multiply_global_factor_ed64(random_float_between(180.0f, 240.0f)));
        patrol_vars->retimer = 0;
    }
    fast_dec_timer(&patrol_vars->timer);
    if (patrol_vars->timer != 0) {
        patrol_vars->reach = patrol_vars->range + 64.0f;
    } else {
        patrol_vars->reach = patrol_vars->range;
    }
    path = path_table[patrol_vars->path_index];
    if (FUN_L00_0025ff38(moby, &patrol_vars->goal, 0, 0, path + 0x10, *(s32 *)path, patrol_vars->reach) != 2) {
        Vec4f *goal_check_position;
        if (moby->state == 1) {
            goal_check_position = (Vec4f *)(path_table[patrol_vars->path2_index] + 0x20);
        } else {
            goal_check_position = &moby->pos;
        }
        qcopy(&position_scratch, goal_check_position);
        if (vector_distance(position_scratch.f, &patrol_vars->goal.x) > patrol_vars->reach || position_scratch.f[2] - patrol_vars->goal.z > 3.0f) {
            patrol_vars->unk214 = 2;
        }
    }
    if (patrol_vars->target == 0) {
        patrol_vars->target = hero.moby;
        qcopy(&patrol_vars->goal, &hero.motion.pos);
    }
}
extern float D_6C_e[] __asm__("D_0015ED68") __attribute__((section(".sdata")));
extern float D_L11_001622FC_gp __asm__("D_L11_001622FC") __attribute__((sda));
extern float FUN_001f9e90_c(float, float) __asm__("FUN_001f9e90");
extern float FUN_001fa580_c(float, float) __asm__("FUN_001fa580");
extern float FUN_001f9dc8_c(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_c(float) __asm__("FUN_001f9de0");
extern void FUN_L00_0025be00_c(char *, char *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern float FUN_0020c9e0_c(char *) __asm__("FUN_0020c9e0");
extern void FUN_L00_00258b50_c(char *, char *, float *, float *, float) __asm__("FUN_L00_00258b50");

void update_moby_target_heading_and_conditional_movement(char *moby, float *target_position) __asm__("FUN_L11_00315f58");

void update_moby_target_heading_and_conditional_movement(char *moby, float *target_position)
{
    char *moby_vars = *(char **)(moby + 0x78);
    float planar_heading_vector[4];
    float helper_scratch_buffer[4];
    float f;
    f = FUN_001fa580_c(FUN_001f9e90_c(target_position[0] - *(float *)(moby + 0x10), target_position[1] - *(float *)(moby + 0x14)), *(float *)(moby_vars + 0x24C));
    FUN_L00_0025be00_c(moby + 0x48, moby_vars + 0x250, f, frame_time_sq * 8.726646f, frame_time_sq * 8.726646f, D_6C_e[1] * 12.566371f);
    f = FUN_0020c9e0_c(moby);
    if (*(u8 *)(moby + 0x52) == *(u8 *)(moby + 0x53) && 8.0f < f && f < 22.0f) {
        *(float *)(moby_vars + 0x1A4) = D_L11_001622FC_gp * frame_time;
        planar_heading_vector[0] = FUN_001f9dc8_c(*(float *)(moby + 0x48)) * 2.0f;
        planar_heading_vector[1] = FUN_001f9de0_c(*(float *)(moby + 0x48)) * 2.0f;
        *(int *)&planar_heading_vector[2] = 0;
        FUN_L00_00258b50_c(moby, moby_vars + 0x180, planar_heading_vector, helper_scratch_buffer, 1.0f);
    }
}
extern short *D_L11_001AC240[];
extern char *D_L11_0015FFD8;

/* Counts the listed mobys of class 0x4DE in states 13 to 17. */
short count_group_class_4de_mobys_in_states_13_to_17(struct Moby *self) __asm__("FUN_L11_00316090");

short count_group_class_4de_mobys_in_states_13_to_17(struct Moby *self) {
    unsigned short *moby_list_entry;
    short matching_moby_count;
    char *base;

    if (self->group == 0xFF)
        return 0;
    moby_list_entry = (unsigned short *)D_L11_001AC240[self->group];
    matching_moby_count = 0;
    if (moby_list_entry == 0)
        return 0;
    base = D_L11_0015FFD8;
    for (;;) {
        struct Moby *moby = (struct Moby *)(base + ((*moby_list_entry & 0x7FFF) << 8));
        if (moby->oclass == 0x4DE) {
            if (moby->state >= 13) {
                if (moby->state < 18)
                    matching_moby_count++;
            }
        }
        if ((short)*moby_list_entry++ < 0)
            return matching_moby_count;
    }
}
/* True when the moby is in state 0x14, 0xB, 0x12 or 0xC. */
s32 is_moby_in_bob_target_exclusion_state(const u8 *moby) __asm__("FUN_L11_00316128");

s32 is_moby_in_bob_target_exclusion_state(const u8 *moby) {
    u32 moby_state = moby[0x20];
    if (moby_state == 0x14 || moby_state == 0xB || moby_state == 0x12 || moby_state == 0xC) {
        return 1;
    }
    return 0;
}

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

/* Grabbed check: while held, park the moby in state 0x14 (remembering its state) unless it cannot be grabbed; restore the saved state once released. */
int update_moby_suck_cannon_suction_state(struct Moby *moby) __asm__("FUN_L11_00316160");

int update_moby_suck_cannon_suction_state(struct Moby *moby) {
    int grab_check_result = process_moby_suck_cannon_suction(moby);
    if (grab_check_result != 0) {
        if (moby->state == 2 || moby->state == 4 || moby->state == 5 || moby->state == 6 ||
            moby->state == 0xE || moby->state == 0xF || moby->state == 0x10 || moby->state == 0x14) {
            if (moby->state != 0x14) {
                moby->command = moby->state;
                moby->state = 0x14;
            }
        } else {
            grab_check_result = 0;
        }
    } else if (moby->state == 0x14) {
        moby->state = moby->command;
    }
    return grab_check_result;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00317680), where it is exact; names translated to the US level program. */

extern void *store_moby_in_suck_cannon(void *) __asm__("FUN_L00_002dbb20");

void *FUN_L11_00316210(struct Moby *arg) {
    void *found = store_moby_in_suck_cannon(arg);
    if (found != 0) {
        unsigned char state = arg->state;
        if (state != 0x14) {
            arg->command = state;
            arg->state = 0x14;
        }
    } else if (arg->state == 0x14) {
        arg->state = arg->command;
    }
    return found;
}

#include "qcopy.h"

extern float D_L11_00162308 __attribute__((sda));
extern float D_L11_0016230C __attribute__((sda));
extern float D_L11_00162310 __attribute__((sda));
extern struct Moby *spawn_moby_class(s32) __asm__("FUN_0020c4f8");
extern float vector_distance(float *, float *) __asm__("FUN_001f9b80");
extern s32 FUN_00214720(void *, s32);
extern s32 FUN_0022da68(s32, s32, void *);
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_00250df8(struct Moby *);

/* Private variables of the two-door moby FUN_L11_00316320 runs. */
struct L11GateVars {
    struct Moby *left;   /* 0x0 */
    struct Moby *right;  /* 0x4 */
    f32 t;               /* 0x8 */
    f32 speed;           /* 0xC */
    s32 unk10;           /* 0x10 */
};

/* Two-door gate: spawns both 0x4DF doors, then once the hero is close lifts, swings and tilts them open. */
void update_two_door_gate(struct Moby *moby) __asm__("FUN_L11_00316320");

void update_two_door_gate(struct Moby *moby) {
    enum {
        GATE_INITIALIZE_DOORS = 0,
        GATE_WAIT_FOR_HERO_TRIGGER = 1,
        GATE_RAISE_DOORS = 2,
        GATE_ZERO_DOOR_Y_ROTATION = 3,
        GATE_TILT_DOORS_APART = 4,
        GATE_OPEN_IDLE = 5,
        GATE_APPLY_CONFIGURED_DOOR_POSE = 6
    };
    struct L11GateVars *gate_vars = (struct L11GateVars *)moby->pvars;
    float door_position_offset[4];

    switch (moby->state) {
    case GATE_INITIALIZE_DOORS: {
        struct Moby *door_moby;
        struct Moby *door_rotation_target;
        u8 *door_vars;
        moby->scale *= 2.0f;
        gate_vars->left = spawn_moby_class(0x4DF);
        gate_vars->left->draw_dist = 0x40;
        gate_vars->left->visible = 1;
        gate_vars->left->spawn_frame = hero.moby->spawn_frame;
        gate_vars->left->flags = moby->flags | 0x20;
        door_moby = gate_vars->left;
        qcopy(&door_moby->pos, &moby->pos);
        door_moby->pos.z -= 4.0f;
        door_rotation_target = gate_vars->left;
        qcopy(&door_rotation_target->rot, &moby->rot);
        door_rotation_target->rot.x = -(D_L11_0016230C * 0.017453292f);
        gate_vars->left->rot.y = D_L11_00162310 * 0.017453292f;
        door_vars = gate_vars->left->pvars;
        *(u8 **)(door_vars + 8) = door_vars + 0x20;
        *(u32 *)(door_vars + 0x5C) |= 1;
        gate_vars->right = spawn_moby_class(0x4DF);
        gate_vars->right->draw_dist = 0x40;
        gate_vars->right->visible = 1;
        gate_vars->right->spawn_frame = hero.moby->spawn_frame;
        gate_vars->right->flags = moby->flags | 0x8020;
        door_moby = gate_vars->right;
        qcopy(&door_moby->pos, &moby->pos);
        door_moby->pos.z -= 4.0f;
        door_rotation_target = gate_vars->right;
        qcopy(&door_rotation_target->rot, &moby->rot);
        door_rotation_target->rot.x = D_L11_0016230C * 0.017453292f;
        gate_vars->right->rot.y = D_L11_00162310 * 0.017453292f;
        door_vars = gate_vars->right->pvars;
        *(u8 **)(door_vars + 8) = door_vars + 0x20;
        *(u32 *)(door_vars + 0x5C) |= 1;
        moby->state = 1;
        break;
    }
    case GATE_WAIT_FOR_HERO_TRIGGER:
        if (vector_distance(&moby->pos.x, &hero.motion.pos.f[0]) < 12.0f &&
            FUN_00214720(&hero.motion.pos, gate_vars->unk10) != 0) {
            gate_vars->t = 0.0f;
            moby->state = 2;
            FUN_0022da68(0, 4, gate_vars->left);
        }
        break;
    case GATE_RAISE_DOORS:
        advance_accelerated_scalar(&gate_vars->t, &gate_vars->speed, 1.0f, frame_time_sq * 8.0f, frame_time_sq * 16.0f,
                                   frame_time * 8.0f);
        fast_vec_normalize(door_position_offset, &moby->rot_row0, 0.5f);
        fast_vec_add(&gate_vars->left->pos, &moby->pos, door_position_offset);
        gate_vars->left->pos.z = moby->pos.z + gate_vars->t * 4.0f - 4.0f;
        fast_vec_add(&gate_vars->right->pos, &moby->pos, door_position_offset);
        gate_vars->right->pos.z = moby->pos.z + gate_vars->t * 4.0f - 4.0f;
        FUN_L00_00250df8(gate_vars->left);
        FUN_L00_00250df8(gate_vars->right);
        if (gate_vars->t >= 1.0f) {
            gate_vars->t = 0.0f;
            moby->state = 3;
        }
        break;
    case GATE_ZERO_DOOR_Y_ROTATION: {
        float a;
        advance_accelerated_scalar(&gate_vars->t, &gate_vars->speed, 1.0f, frame_time_sq * 10.0f, frame_time_sq * 20.0f,
                                   frame_time * 10.0f);
        a = (1.0f - gate_vars->t) * (D_L11_00162310 * 0.017453292f);
        gate_vars->left->rot.y = a;
        gate_vars->right->rot.y = a;
        FUN_L00_00250df8(gate_vars->left);
        FUN_L00_00250df8(gate_vars->right);
        if (gate_vars->t >= 1.0f) {
            gate_vars->t = 0.0f;
            moby->state = 4;
        }
        break;
    }
    case GATE_TILT_DOORS_APART: {
        float a;
        advance_accelerated_scalar(&gate_vars->t, &gate_vars->speed, 1.0f, frame_time_sq * 12.0f, frame_time_sq * 24.0f,
                                   frame_time * 12.0f);
        gate_vars->left->pos.z = moby->pos.z + gate_vars->t * 0.25f;
        gate_vars->right->pos.z = moby->pos.z + gate_vars->t * 0.25f;
        a = gate_vars->t * 1.5707964f;
        gate_vars->left->rot.x = -a;
        gate_vars->right->rot.x = a;
        FUN_L00_00250df8(gate_vars->left);
        FUN_L00_00250df8(gate_vars->right);
        if (gate_vars->t >= 1.0f) {
            gate_vars->t = 0.0f;
            moby->state = 5;
        }
        break;
    }
    case GATE_OPEN_IDLE:
        break;
    case GATE_APPLY_CONFIGURED_DOOR_POSE:
        fast_vec_normalize(door_position_offset, &moby->rot_row0, 0.5f);
        fast_vec_add(&gate_vars->left->pos, &moby->pos, door_position_offset);
        gate_vars->left->pos.z = moby->pos.z + D_L11_00162308;
        gate_vars->left->rot.x = -(D_L11_0016230C * 0.017453292f);
        gate_vars->left->rot.y = D_L11_00162310 * 0.017453292f;
        FUN_L00_00250df8(gate_vars->left);
        fast_vec_add(&gate_vars->right->pos, &moby->pos, door_position_offset);
        gate_vars->right->pos.z = moby->pos.z + D_L11_00162308;
        gate_vars->right->rot.x = D_L11_0016230C * 0.017453292f;
        gate_vars->right->rot.y = D_L11_00162310 * 0.017453292f;
        FUN_L00_00250df8(gate_vars->right);
        break;
    }
}

/* Private variables of the chained moby class 0x527 (oil_tanker_front_update and its helpers). */
struct L11ChainVars {
    u8 pad0[0x60];
    s32 path_index;          /* 0x60: index into D_L11_001B0EB0 */
    s32 count;               /* 0x64: live links */
    s32 point;               /* 0x68: current path point */
    f32 t;                   /* 0x6C: progress between points */
    struct Moby *links[8];   /* 0x70 */
    s16 offsets[8];          /* 0x90 */
    f32 frac[8];             /* 0xA0: position between path points */
    f32 catchup[8];          /* 0xC0: path distance a link still trails its slot by after a link dies; eased to 0 */
    f32 health;              /* 0xE0 */
    s16 group;               /* 0xE4 */
    u8 padE6[6];
    s32 path_sel;            /* 0xEC: index into D_L11_001B0EB0 on spawn */
    char *path;              /* 0xF0 */
    s32 sound;               /* 0xF4 */
};

extern float D_L11_00162320 __attribute__((sda));
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern s32 FUN_0022da68(s32, s32, void *);
extern s32 voice_is_playing(void *, s32) __asm__("FUN_L00_0028d8c0");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
void update_oil_tanker_front_path_pose(struct Moby *moby) __asm__("FUN_L11_00317678");

void update_oil_tanker_front_path_pose(struct Moby *) __asm__("FUN_L11_00317678");
void update_oil_tanker_link_path_poses(struct Moby *) __asm__("FUN_L11_00317820");
void process_oil_tanker_damage_and_destruction(struct Moby *moby) __asm__("FUN_L11_00317c98");

void process_oil_tanker_damage_and_destruction(struct Moby *) __asm__("FUN_L11_00317c98");
void distribute_group_class_527_mobys_along_path(struct Moby *moby, int table_index) __asm__("FUN_L11_003180a0");

void distribute_group_class_527_mobys_along_path(struct Moby *, int) __asm__("FUN_L11_003180a0");

/* Chain head update: spawns the eight 0x4F1 links, waits for D_001413D4 to reach 0x32, then moves along its path and runs the link helpers. */
void oil_tanker_front_update(struct Moby *moby) __asm__("FUN_L11_003172c0");
void oil_tanker_front_update(struct Moby *moby) {
    enum {
        OIL_TANKER_INITIALIZE = 0,
        OIL_TANKER_WAIT_FOR_CAMERA_MODE = 1,
        OIL_TANKER_FOLLOW_PATH_WITH_DAMAGE = 2
    };
    struct L11ChainVars *chain_vars = (struct L11ChainVars *)moby->pvars;

    switch (moby->state) {
    case OIL_TANKER_INITIALIZE: {
        int i;

        distribute_group_class_527_mobys_along_path(moby, chain_vars->path_index);
        if (visited52[moby->mission + current_level_index * 0x10] == 0xFF) {
            mark_moby_for_removal(moby);
            break;
        }
        moby->update_dist = 0xFF;
        moby->draw_dist = 0x200;
        chain_vars->path = path_table[chain_vars->path_sel];
        qcopy(&moby->pos, chain_vars->path + 0x10);
        chain_vars->health = 80.0f;
        chain_vars->count = 8;
        chain_vars->sound = -1;
        chain_vars->point = 0;
        chain_vars->t = 0.0f;
        moby->state = 1;
        for (i = 0; i < 8; i++) {
            struct Moby *link;
            chain_vars->links[i] = spawn_moby_class(0x4F1);
            chain_vars->links[i]->draw_dist = 0x200;
            chain_vars->links[i]->update_dist = 0xFF;
            chain_vars->links[i]->visible = 1;
            chain_vars->links[i]->spawn_frame = moby->spawn_frame;
            chain_vars->links[i]->flags = moby->flags & 0xFFDF;
            chain_vars->links[i]->scale = chain_vars->links[i]->pclass->scale * 2.0f;
            link = chain_vars->links[i];
            qcopy(&link->pos, &moby->pos);
            qcopy(&link->rot, &moby->rot);
            chain_vars->offsets[i] = *(u16 *)chain_vars->path - 100;
            chain_vars->catchup[i] = 0.0f;
        }
        break;
    }
    case OIL_TANKER_WAIT_FOR_CAMERA_MODE:
        if (camera_secondary_mode[0] == 0x32) {
            int i;

            moby->state = 2;
            chain_vars->path = path_table[chain_vars->path_index];
            chain_vars->point = 0;
            for (i = 0; i < 8; i++) {
                chain_vars->offsets[i] = *(u16 *)chain_vars->path - 100;
                chain_vars->catchup[i] = 0.0f;
            }
        }
        /* fallthrough */
    case OIL_TANKER_FOLLOW_PATH_WITH_DAMAGE: {
        int i;

        chain_vars->t += D_L11_00162320;
        if (chain_vars->t > 1.0f) {
            chain_vars->t -= 1.0f;
            chain_vars->point = (chain_vars->point + 1) % *(s32 *)chain_vars->path;
        }
        for (i = 0; i < 8; i++) {
            float before = chain_vars->catchup[i];
            approach_value(&chain_vars->catchup[i], 0.0f, frame_time * 5.0f);
            if (before != 0.0f && chain_vars->catchup[i] == 0.0f && chain_vars->links[i] != 0) {
                FUN_0022da68(0, 0, chain_vars->links[i]);
            }
        }
        update_oil_tanker_front_path_pose(moby);
        update_oil_tanker_link_path_poses(moby);
        if (moby->state == 2) {
            process_oil_tanker_damage_and_destruction(moby);
        }
        if (voice_is_playing(moby, chain_vars->sound) == 0) {
            chain_vars->sound = FUN_0022da68(0, 4, moby);
        }
        break;
    }
    }
}

/* Orients a moby along a path of three points read from its data table. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00318AE8), where it is exact; names translated to the US level program. */

extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float vector_distance(float *, float *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void lerp_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");

void update_oil_tanker_front_path_pose(struct Moby *moby) {
    char *chain_vars = (char *)moby->pvars;
    int i = *(int *)(chain_vars + 0x68);
    char *path = *(char **)(chain_vars + 0xF0);
    int path_point_count;
    float current_path_point[4];
    float next_path_point[4];
    float lookahead_path_point[4];
    float *pb;
    float *pc;
    float x, y, z;
    qcopy(current_path_point, path + i * 16 + 0x10);
    path_point_count = *(int *)path;
    qcopy(pb = next_path_point, path + (i + 1) % path_point_count * 16 + 0x10);
    qcopy(pc = lookahead_path_point, path + (i + 2) % path_point_count * 16 + 0x10);
    x = fast_arc_tan(current_path_point[0] - next_path_point[0], current_path_point[1] - next_path_point[1]);
    y = fast_arc_tan(next_path_point[0] - lookahead_path_point[0], next_path_point[1] - lookahead_path_point[1]);
    z = fast_arc_tan(vector_distance(current_path_point, pb), next_path_point[2] - current_path_point[2]);
    moby->rot.y = fast_add_rotations(
        fast_subtract_rotations(fast_arc_tan(vector_distance(pb, pc), lookahead_path_point[2] - next_path_point[2]), z) *
            *(float *)(chain_vars + 0x6C),
        z);
    moby->rot.z =
        fast_add_rotations(fast_subtract_rotations(y, x) * *(float *)(chain_vars + 0x6C), x);
    lerp_vector(&moby->pos, current_path_point, pb, *(float *)(chain_vars + 0x6C));
}
extern float D_L11_00162324 __attribute__((sda));
extern float D_L11_00162328 __attribute__((sda));
extern s32 D_L11_0016232C __attribute__((sda));
extern s32 D_L11_0015F5C4;
extern float FUN_001f9b48(void *, void *);
extern s32 FUN_001fa6d0(float);
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");

/* A chain path in D_L11_001B0EB0: a point count, then the points (w = 42 marks a sound point). */
struct L11ChainPath {
    s32 count;
    u8 pad4[0xC];
    Vec4f pts[1];
};

/* Moves each chain link along the path to a fixed distance behind the one before it and turns it to follow the curve. */
void update_oil_tanker_link_path_poses(struct Moby *moby) {
    struct L11ChainVars *chain_vars = (struct L11ChainVars *)moby->pvars;
    char *path = chain_vars->path;
    struct L11ChainPath *chain_path = (struct L11ChainPath *)chain_vars->path;
    float spacing_reference_position[4];
    int i;

    fast_vec_normalize(spacing_reference_position, &moby->rot_row0, D_L11_00162328);
    fast_vec_add(spacing_reference_position, spacing_reference_position, &moby->pos);
    for (i = 0; i < chain_vars->count; i++) {
        int segment_start_index = chain_vars->offsets[i];
        int segment_end_index = (segment_start_index + 1) % chain_path->count;
        int lookahead_point_index = (segment_start_index + 2) % chain_path->count;
        float segment_start_distance = FUN_001f9b48(&chain_path->pts[segment_start_index], spacing_reference_position);
        float segment_end_distance = FUN_001f9b48(&chain_path->pts[segment_end_index], spacing_reference_position);
        float segment_fraction;
        struct Moby *link;
        float yaw0, yaw1, pitch0;
        float *qa, *qb, *qc; /* the points as raw rows: [4], [5], [6] = x, y, z */

        while (segment_end_distance > D_L11_00162324) {
            segment_start_index = segment_end_index;
            segment_end_index = (segment_start_index + 1) % chain_path->count;
            segment_start_distance = segment_end_distance;
            lookahead_point_index = (segment_end_index + 1) % chain_path->count;
            segment_end_distance = FUN_001f9b48(&chain_path->pts[segment_end_index], spacing_reference_position);
        }
        {
            float *q = (float *)(path + segment_start_index * 16);

            if (q[7] == 42.0f) {
                u8 *lv = chain_vars->links[i]->pvars;
                if (*(s32 *)(lv + 0x28) == 0) {
                    if (D_L11_0015F5C4 == 0) {
                        FUN_0022da68(2, 0, chain_vars->links[i]);
                    }
                    *(s32 *)(lv + 0x28) = scale_game_frames(D_L11_0016232C);
                }
            }
        }
        if (segment_start_distance > D_L11_00162324) {
            segment_fraction = (segment_start_distance - D_L11_00162324) / (segment_start_distance - segment_end_distance);
        } else {
            segment_fraction = 0.0f;
        }
        chain_vars->offsets[i] = segment_start_index;
        chain_vars->frac[i] = segment_fraction;
        if (chain_vars->catchup[i] != 0.0f) {
            float catchup_adjusted_progress = segment_fraction - chain_vars->catchup[i];
            int k = FUN_001fa6d0(catchup_adjusted_progress);
            catchup_adjusted_progress -= (float)FUN_001fa6d0(catchup_adjusted_progress);
            if (catchup_adjusted_progress < 0.0f) {
                k--;
                catchup_adjusted_progress += 1.0f;
            }
            segment_start_index = (segment_start_index + k + chain_path->count) % chain_path->count;
            segment_end_index = (segment_end_index + k + chain_path->count) % chain_path->count;
            lookahead_point_index = (lookahead_point_index + k + chain_path->count) % chain_path->count;
            segment_fraction = catchup_adjusted_progress;
        }
        link = chain_vars->links[i];
        lerp_vector(&link->pos, &chain_path->pts[segment_start_index], &chain_path->pts[segment_end_index], segment_fraction);
        qcopy(spacing_reference_position, &chain_vars->links[i]->pos);
        qa = (float *)(path + segment_start_index * 16);
        qb = (float *)(path + segment_end_index * 16);
        qc = (float *)(path + lookahead_point_index * 16);
        yaw0 = fast_arc_tan(qa[4] - qb[4], qa[5] - qb[5]);
        yaw1 = fast_arc_tan(qb[4] - qc[4], qb[5] - qc[5]);
        pitch0 = -fast_arc_tan(vector_distance(&chain_path->pts[segment_end_index].x, &chain_path->pts[segment_start_index].x),
                               qa[6] - qb[6]);
        chain_vars->links[i]->rot.y = fast_add_rotations(
            fast_subtract_rotations(-fast_arc_tan(vector_distance(&chain_path->pts[lookahead_point_index].x, &chain_path->pts[segment_end_index].x),
                                                  qb[6] - qc[6]),
                                    pitch0) * segment_fraction,
            pitch0);
        chain_vars->links[i]->rot.z = fast_add_rotations(fast_subtract_rotations(yaw1, yaw0) * segment_fraction, yaw0);
        FUN_L00_00250df8(chain_vars->links[i]);
        fast_vec_normalize(spacing_reference_position, &chain_vars->links[i]->rot_row0, D_L11_00162328);
        fast_vec_add(spacing_reference_position, spacing_reference_position, &chain_vars->links[i]->pos);
    }
}
extern char D_00141C18[];
extern float D_L11_0015F580[] __attribute__((section(".sdata")));
extern float D_L11_00162330 __attribute__((sda));
extern struct Moby *find_moby_hit(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern s32 FUN_0022da68(s32, s32, void *);
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
void *FUN_L00_00263fd8(char *src, int cls, float *pos, void *mat, int a8, int a9, float *v10,
                       float *v11, float scale, float *v12);
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
void start_oil_tanker_link_destruction(struct Moby *link_moby) __asm__("FUN_L11_00318488");

void start_oil_tanker_link_destruction(struct Moby *) __asm__("FUN_L11_00318488");

/* Drops the chain's damaged links: takes the hits, shrinks the chain as its health falls and blows the head up when no link is left. */
void process_oil_tanker_damage_and_destruction(struct Moby *moby) {
    struct L11ChainVars *chain_vars = (struct L11ChainVars *)moby->pvars;
    int i;
    int last_hit_link_index = -1;
    float total = 0.0f;
    float fragment_initial_velocity[4];

    for (i = 0; i < chain_vars->count; i++) {
        struct Moby *hit = find_moby_hit(chain_vars->links[i], -1, 0);
        if (hit != 0) {
            last_hit_link_index = i;
            total += hit->scale;
            chain_vars->links[i]->hit_slot = 0xFF;
        }
    }
    approach_value(&chain_vars->health, 0.0f, total);
    if (total != 0.0f && chain_vars->health / 80.0f * 8.0f <= (float)(chain_vars->count - 1)) {
        struct Moby *last_hit_link;
        struct Moby *detached_tail_link;
        *(u16 *)D_00141C18 = 0xFFFF;
        chain_vars->count--;
        start_oil_tanker_link_destruction(chain_vars->links[chain_vars->count]);
        last_hit_link = chain_vars->links[last_hit_link_index];
        detached_tail_link = chain_vars->links[chain_vars->count];
        qcopy(&detached_tail_link->pos, &last_hit_link->pos);
        qcopy(&detached_tail_link->rot, &last_hit_link->rot);
        FUN_0022da68(1, 0, detached_tail_link);
        if (last_hit_link_index < chain_vars->count) {
            float path_point_offset_delta = chain_vars->offsets[last_hit_link_index] - chain_vars->offsets[last_hit_link_index + 1];
            int j;
            for (j = last_hit_link_index; j < chain_vars->count; j++) {
                chain_vars->catchup[j] = chain_vars->catchup[j + 1];
            }
            chain_vars->catchup[last_hit_link_index] += path_point_offset_delta;
            if (chain_vars->catchup[last_hit_link_index] < 0.0f) {
                chain_vars->catchup[last_hit_link_index] += (float)*(s32 *)chain_vars->path;
            }
        }
        {
            int k;
            for (k = last_hit_link_index; k < chain_vars->count; k++) {
                chain_vars->offsets[k] = chain_vars->offsets[k + 1];
            }
        }
        if (chain_vars->count == 0) {
            char *head_position = (char *)&moby->pos;
            float *dv;
            char *head_rotation;
            fast_vec_scale(fragment_initial_velocity, &moby->rot_row0, (dv = D_L11_0015F580, head_rotation = (char *)&moby->rot, D_L11_00162330 * frame_scale));
            fragment_initial_velocity[2] += frame_scale * 0.08f;
            FUN_0022da68(2, 0, moby);
            cart_burst(moby, head_position, -1, 3.0f, 13.0f);
            FUN_L00_00263fd8((char *)moby, 0x602, (float *)head_position, head_rotation, scale_game_frames(0x5A), 0, fragment_initial_velocity, dv, frame_time_sq * 12.0f, dv);
            FUN_L00_00263fd8((char *)moby, 0x603, (float *)head_position, head_rotation, scale_game_frames(0x5A), 0, fragment_initial_velocity, dv, frame_time_sq * 12.0f, dv);
            FUN_L00_00263fd8((char *)moby, 0x784, (float *)head_position, head_rotation, scale_game_frames(0x5A), 0, fragment_initial_velocity, dv, frame_time_sq * 12.0f, dv);
            mark_moby_for_removal(moby);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_003194C0), where it is exact; names translated to the US level program. */

int collect_oil_tanker_chain_mobys(struct Moby *moby, void **out) __asm__("FUN_L11_00318050");

int collect_oil_tanker_chain_mobys(struct Moby *moby, void **out) {
    char *data = (char *)moby->pvars;
    int i;
    int count = *(int *)(data + 0x64) + 1;
    out[0] = moby;
    for (i = 0; i < *(int *)(data + 0x64); i++) {
        out[i + 1] = ((void **)(data + 0x70))[i];
    }
    return count;
}
extern char D_L11_001B0EB0[];

/* Spreads the 0x527 chain mobys of a D_L11_001AC240 group evenly along path table_index: each gets a start point and its path index. */
/* idx is shared by both loops (one multi-block pseudo), which gives the second loop retail's register choice. */
void distribute_group_class_527_mobys_along_path(struct Moby *moby, int table_index) {
    short group = ((struct L11ChainVars *)moby->pvars)->group;
    u16 *list = (u16 *)D_L11_001AC240[group];
    short count = 0;
    short offset = 0;
    short stride;
    char *table;
    u16 entry;
    int moby_array_index;

    if (list == 0)
        return;
    do {
        entry = *list;
        moby_array_index = entry & 0x7FFF;
        if (((struct Moby *)D_L11_0015FFD8)[moby_array_index].oclass == 0x527)
            count++;
        list++;
    } while ((short)entry >= 0);

    table = *(char **)(D_L11_001B0EB0 + table_index * 4);
    stride = *(int *)table / count;
    list = (u16 *)D_L11_001AC240[group];
    do {
        struct Moby *group_moby;
        moby_array_index = *list & 0x7FFF;
        group_moby = (struct Moby *)((moby_array_index << 8) + (u32)D_L11_0015FFD8);
        if (group_moby->oclass == 0x527) {
            struct L11ChainVars *data = (struct L11ChainVars *)group_moby->pvars;
            /* no memory clobber: lets the D_L11_0015FFD8 load be hoisted out of the loop as in retail */
            qcopy_nc(&group_moby->pos, table + offset * 16 + 0x10);
            data->count = offset;
            data->path_index = table_index;
            data->point = 0;
            offset += stride;
        }
    } while (*(short *)list++ >= 0);
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00319660), where it is exact; names translated to the US level program. */

char *FUN_L11_0031ada0(void *position, void *vector, void *owner, float scale);
extern char D_L11_00161E18[] __attribute__((section(".sdata")));
extern float D_L11_0015F580[] __attribute__((section(".sdata")));
extern float random_float_between(float a, float b) __asm__("FUN_002132a8");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern char D_L11_0015FFBC[4] __attribute__((sda));
extern char D_L11_00162340[4] __attribute__((sda));
extern char D_L11_00162350[4] __attribute__((sda));
extern char D_L11_00162360[4] __attribute__((sda));
extern char D_L11_00162364[4] __attribute__((sda));
extern char D_L11_00162368[4] __attribute__((sda));
extern char D_L11_0016236C[4] __attribute__((sda));
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void add_vector_xyz(void *, void *, void *);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern void tick_countdown_32(int *arg0);
extern void transform_vector_by_basis(void *, void *, void *);
void *FUN_L00_00263fd8(char *src, int cls, float *pos, void *mat, int a8, int a9, float *v10,
                       float *v11, float scale, float *v12);
extern int func_001F9908_i(void *) __asm__("FUN_001f9740");
extern char *func_L11_0031C210_5(void *, void *, void *, int, float) __asm__("FUN_L11_0031ada0");
extern void *func_L00_00265050_s(char *, int, void *, void *, int, int, float *, float *, float,
                                 float *) __asm__("FUN_L00_00263fd8");

void update_oil_tanker_link_effects_and_destruction(struct Moby *moby) __asm__("FUN_L11_003181f0");

void update_oil_tanker_link_effects_and_destruction(struct Moby *moby) {
    enum {
        TANKER_LINK_EMIT_EFFECT_AND_SAVE_POSITION = 0,
        TANKER_LINK_BURST_AND_HIDE = 1
    };
    char *link_vars = (char *)moby->pvars;
    float effect_scratch_vector[4];
    float effect_motion_vector[4];
    float random_motion_offset[4];
    switch (moby->state) {
    case TANKER_LINK_EMIT_EFFECT_AND_SAVE_POSITION:
        if (func_001F9908_i(link_vars + 0x28) == 0 && random_integer_below(*(int *)&D_L11_00162340) == 0 &&
            moby->visible != 0 && *(int *)&D_L11_0015FFBC >= 0x47) {
            transform_vector_by_basis(effect_scratch_vector, &D_L11_00162350, &moby->rot_row0);
            add_vector_xyz(effect_scratch_vector, effect_scratch_vector, &moby->pos);
            random_vector(random_motion_offset, 0.0f, *(float *)&D_L11_00162368 * D_0015ED6C_n[1]);
            subtract_vector_xyz(effect_motion_vector, &moby->pos, link_vars);
            effect_motion_vector[2] -= D_0015ED6C_n[1] + D_0015ED6C_n[1];
            add_vector_xyz(effect_motion_vector, effect_motion_vector, random_motion_offset);
            func_L11_0031C210_5(
                effect_scratch_vector, effect_motion_vector, D_L11_00161E18, 0,
                random_float_between(*(float *)&D_L11_00162360, *(float *)&D_L11_00162364));
        }
        qcopy(link_vars, &moby->pos);
        break;
    case TANKER_LINK_BURST_AND_HIDE: {
        char *link_position = &moby->pos;
        float *dv;
        char *link_rotation;
        cart_burst(moby, link_position, -1, 5.0f, 13.0f);
        scale_vector_xyz(
            effect_scratch_vector, &moby->rot_row0,
            (dv = D_L11_0015F580, link_rotation = &moby->rot, *(float *)&D_L11_0016236C * frame_scale));
        effect_scratch_vector[2] += frame_scale * 0.08f;
        func_L00_00265050_s(moby, 0x785, link_position, link_rotation, scale_game_frames(0x5A), 0, effect_scratch_vector, dv, frame_time_sq * 12.0f,
                            dv);
        func_L00_00265050_s(moby, 0x786, link_position, link_rotation, scale_game_frames(0x5A), 0, effect_scratch_vector, dv, frame_time_sq * 12.0f,
                            dv);
        func_L00_00265050_s(moby, 0x787, link_position, link_rotation, scale_game_frames(0x5A), 0, effect_scratch_vector, dv, frame_time_sq * 12.0f,
                            dv);
        moby->state = 2;
        moby->visible = 0;
        moby->flags |= 1;
        break;
    }
    }
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_003198F8), where it is exact; names translated to the US level program. */

extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");

void start_oil_tanker_link_destruction(struct Moby *link_moby) {
    char *link_vars = (char *)link_moby->pvars;
    link_moby->state = 1;
    link_moby->collision = 0;
    fast_vec_sub(link_vars + 0x10, &link_moby->pos, link_vars);
    *(float *)(link_vars + 0x20) = random_float_between_alt(20.0f, 60.0f) * DEG_TO_RAD * D_0015ED6C_n[1];
    *(float *)(link_vars + 0x24) = random_float_between_alt(20.0f, 60.0f) * DEG_TO_RAD * D_0015ED6C_n[1];
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00319FA0), where it is exact; names translated to the US level program. */

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L11_00319028_c(char *moby) __asm__("FUN_L11_00319028");
extern void FUN_L11_003191c0_c(char *moby) __asm__("FUN_L11_003191c0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void tick_countdown_32_u(void *) __asm__("func_001F9740");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern int func_001F9908_i(void *) __asm__("FUN_001f9740");

extern short D_L11_00162388 __attribute__((sda));
void energy_sphere_update(unsigned char *moby) __asm__("FUN_L11_00318b30");
void energy_sphere_update(unsigned char *moby) {
    enum {
        ENERGY_SPHERE_APPROACH_SCALE_AND_REMOVE = 1,
        ENERGY_SPHERE_MOVE_AND_DETONATE = 2
    };
    char *data = *(char **)(moby + 0x78);
    float previous_position[4];
    float *vp;
    FUN_L11_00319028_c((char *)moby);
    switch (moby[0x20]) {
    case ENERGY_SPHERE_APPROACH_SCALE_AND_REMOVE:
        if (func_001F9908_i(data + 0x20)) {
            float *scale_approach_pointer = *(float **)(moby + 0x24);
            float scale_approach_frames = (float)scale_game_frames(0x14);
            float target_scale = scale_approach_pointer[9];
            float scale_approach_step = (*(float **)(moby + 0x24))[9] / scale_approach_frames;
            scale_approach_pointer = (float *)(data + 0x1C);
            approach_value((float *)(moby + 0x2C), target_scale, scale_approach_step);
            approach_value(scale_approach_pointer, *(float *)&D_L11_00162388,
                           *(float *)&D_L11_00162388 / (float)scale_game_frames(0x14));
        }
        if (func_001F9908_i(data + 0x28)) {
            mark_moby_for_removal(moby);
        }
        break;
    case ENERGY_SPHERE_MOVE_AND_DETONATE: {
        char *sphere_position = (char *)moby + 0x10;
        FUN_L11_003191c0_c((char *)moby);
        vp = previous_position;
        qcopy(vp, sphere_position);
        add_vector_xyz(sphere_position, sphere_position, data);
        if (func_001F9908_i(data + 0x20) || collision_line(vp, (char *)moby + 0x10, 0, moby, 0) ||
            collect_mobies_in_sphere(0.333f, (char *)moby + 0x10, 0, *(void **)(data + 0x24), 0)) {
            spawn_explosion(moby, data, (char *)moby + 0x10, 1.5f, 2.0f, 10, 3, 16, 4.0f, 2.0f,
                             9.0f, -1, 1.0f, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal(moby);
        }
        break;
    }
    }
}
#include "qcopy.h"

/* Spawns a spark effect moby from a source moby, initialises its particle bytes and warms it up. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A180), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int FUN_L11_00318ed8_u() __asm__("FUN_L11_00318ed8");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

char *spawn_class_511_moby_with_sparks(char *source_moby, void *pos, int ticks) __asm__("FUN_L11_00318d10");

char *spawn_class_511_moby_with_sparks(char *source_moby, void *pos, int ticks) {
    char *mob = func_0020D348_m(0x511);
    if (mob != 0) {
        char *moby_vars;
        unsigned char *initialization_byte_cursor;
        int i, k, initialization_byte_value;
        *(short *)(mob + 0x32) = 0x40;
        *(unsigned char *)(mob + 0x30) = 0xFF;
        mob[0x31] = 1;
        *(int *)(mob + 0x2C) = 0;
        moby_vars = *(char **)(mob + 0x78);
        *(long *)(mob + 0x38) = *(long *)(source_moby + 0x38);
        mob[0x20] = 1;
        qcopy(mob + 0x10, pos);
        qcopy(mob + 0x40, source_moby + 0x40);
        clear_u64_value(moby_vars);
        *(char **)(moby_vars + 0x24) = source_moby;
        initialization_byte_cursor = (unsigned char *)(moby_vars + 0x14);
        *(int *)(moby_vars + 0x20) = scale_game_frames(ticks);
        *(int *)(moby_vars + 0x28) = 5;
        euler_rotation_basis(mob + 0xC0, mob + 0x40);
        for (i = 0, initialization_byte_value = 10; i < 4; i++, initialization_byte_value += 10, initialization_byte_cursor++) {
            initialization_byte_cursor[-4] = random_integer_below(0xFF);
            if (i * (i & 1))
                initialization_byte_cursor[0] = 0xFF;
            else
                initialization_byte_cursor[0] = 1;
            initialization_byte_cursor[4] = initialization_byte_value;
        }
        for (k = 0x31; k >= 0; k--)
            FUN_L11_00318ed8_u(mob);
    }
    return mob;
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A2E8), where it is exact; names translated to the US level program. */

void set_moby_position_and_refresh_removal_timer(struct Moby *moby, void *new_position) __asm__("FUN_L11_00318e78");

void set_moby_position_and_refresh_removal_timer(struct Moby *moby, void *new_position) {
    char *moby_vars = (char *)moby->pvars;
    qcopy(&moby->pos, new_position);
    *(int *)(moby_vars + 0x28) = 5;
}
#include "qcopy.h"
extern int scale_ticks(int) __asm__("FUN_001f96f8");

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A308), where it is exact; names translated to the US level program. */

void FUN_L11_00318e98(struct Moby *moby, void *src) {
    char *moby_vars;
    moby->state = 2;
    moby_vars = (char *)moby->pvars;
    qcopy(moby_vars, src);
    *(int *)(moby_vars + 0x20) = scale_ticks(0x3C);
}
/* Builds a few direction vectors from a moby's axes and calls the 0x219780 spawner. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A348), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

extern short D_L11_0016238C __attribute__((sda));
extern short D_L11_00162390 __attribute__((sda));
extern short D_L11_00162394 __attribute__((sda));
extern short D_L11_00162398 __attribute__((sda));
extern short D_L11_0016239C __attribute__((sda));
extern short D_L11_001623A0 __attribute__((sda));
extern short D_L11_001623A4 __attribute__((sda));
extern short D_L11_001623A8 __attribute__((sda));
void FUN_L11_00318ed8(struct Moby *source_moby) {
    float spark_position[4];
    float radial_offset[4];
    float row1_offset[4];
    float first_spark_vector[4];
    float second_spark_vector[4];
    float radial_distance = random_float_between_alt(2.5f, 3.0f);
    float radial_angle = random_angle_radians();
    fast_vec_normalize(row1_offset, &source_moby->rot_row1, radial_distance * fast_sin(radial_angle));
    fast_vec_normalize(radial_offset, &source_moby->rot_row2, radial_distance * fast_cos(radial_angle));
    fast_vec_add(radial_offset, radial_offset, row1_offset);
    fast_vec_sub(spark_position, &source_moby->pos, radial_offset);
    fast_vec_scale(first_spark_vector, radial_offset,
                 1.0f / (float)scale_game_frames(*(int *)&D_L11_00162394 + *(int *)&D_L11_00162398 +
                                            *(int *)&D_L11_0016239C));
    qcopy(second_spark_vector, first_spark_vector);
    first_spark_vector[3] = *(float *)&D_L11_001623A0;
    second_spark_vector[3] = *(float *)&D_L11_001623A4;
    spawn_spark(spark_position, first_spark_vector, second_spark_vector, *(int *)&D_L11_0016238C, *(int *)&D_L11_00162390,
                 *(int *)&D_L11_00162394, *(int *)&D_L11_00162398, *(int *)&D_L11_0016239C,
                 *(int *)&D_L11_001623A8);
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A498), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float_q(s32) __asm__("FUN_001fa6c0");
extern float D_L11_001677C0_q[] __asm__("D_L11_001677C0") __attribute__((section(".data")));
extern int FUN_001fa6e0_q(int, int, float) __asm__("FUN_001fa6e0");
extern s32 truncate_float_to_s32_q(f32) __asm__("FUN_001fa6d0");
extern unsigned char *FUN_L00_00272f68_q(float f, void *pos, int a1, unsigned char a2, int idx,
                                         int flag, int s, int mode) __asm__("FUN_L00_00272f68");
extern void add_vector_xyz_q(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz_q(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz_q(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern int func_001FA898_r_q(float) __asm__("FUN_001fa6d0");

extern short D_L11_00162384 __attribute__((sda));
extern short D_L11_00162380 __attribute__((sda));
void FUN_L11_00319028(struct Moby *moby) {
    float effect_position[4];
    float position_step[4];
    char *data = (char *)moby->pvars;
    unsigned char *effect_byte_cursor;
    float *moby_position = &moby->pos.x;
    int i;
    subtract_vector_xyz_q(effect_position, D_L11_001677C0_q, moby_position);
    effect_byte_cursor = (unsigned char *)(data + 0x10);
    normalize_vector_xyz_q(effect_position, effect_position, -0.3f);
    normalize_vector_xyz_q(position_step, effect_position, 0.1f);
    add_vector_xyz_q(effect_position, effect_position, moby_position);
    for (i = 0; i < 4; i++) {
        int a =
            FUN_001fa6e0_q(*(int *)&D_L11_00162380, *(int *)&D_L11_00162384, (float)i * 0.333f);
        unsigned char updated_effect_byte;
        effect_byte_cursor[0] += effect_byte_cursor[4];
        updated_effect_byte = func_001FA898_r_q((float)effect_byte_cursor[0]);
        FUN_L00_00272f68_q(*(float *)(data + 0x1C) * convert_integer_to_float_q(effect_byte_cursor[8]) * 0.025f, effect_position, a,
                           updated_effect_byte, 0x35, 1, 2, 0);
        effect_byte_cursor++;
        add_vector_xyz_q(effect_position, effect_position, position_step);
    }
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A630), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");

extern short D_L11_001623AC __attribute__((sda));
extern short D_L11_001623B0 __attribute__((sda));
extern short D_L11_001623B4 __attribute__((sda));
extern short D_L11_001623B8 __attribute__((sda));
extern short D_L11_001623BC __attribute__((sda));
extern short D_L11_001623C0 __attribute__((sda));
extern short D_L11_001623C4 __attribute__((sda));
extern short D_L11_001623C8 __attribute__((sda));
extern short D_L11_001623CC __attribute__((sda));
void FUN_L11_003191c0(struct Moby *moby) {
    float first_spark_vector[4];
    float second_spark_vector[4];
    int first_spark_color;
    int second_spark_color;
    random_vector(first_spark_vector, *(float *)&D_L11_001623AC * D_0015ED6C_n[1],
                     *(float *)&D_L11_001623B0 * D_0015ED6C_n[1]);
    random_vector(second_spark_vector, *(float *)&D_L11_001623AC * D_0015ED6C_n[1],
                     *(float *)&D_L11_001623B0 * D_0015ED6C_n[1]);
    first_spark_vector[3] = *(float *)&D_L11_001623B4;
    second_spark_vector[3] = *(float *)&D_L11_001623B8;
    first_spark_color = fast_tween_color(*(int *)&D_L11_001623BC, *(int *)&D_L11_001623C0,
                      random_float_between_alt(0.0f, 1.0f));
    second_spark_color = fast_tween_color(*(int *)&D_L11_001623BC, *(int *)&D_L11_001623C0,
                      random_float_between_alt(0.0f, 1.0f));
    spawn_spark(&moby->pos, first_spark_vector, second_spark_vector, first_spark_color, second_spark_color, *(int *)&D_L11_001623C4, *(int *)&D_L11_001623C8,
                 *(int *)&D_L11_001623CC, *(int *)&D_L11_001623A8);
}
#include "sda.h"

/* UpdateMoby 1319: a flying craft that circles, follows a path, strafes the player and fires. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031ACA8), where it is exact; names translated to the US level program. */

extern char *spawn_class_3f9_moby(int owner, float *dir, float *pos, float size, float len, float z) __asm__("FUN_L11_00308f48");
extern char *spawn_moby_with_random_yaw(void *position, void *vector, int id) __asm__("FUN_L11_0030f5a8");
extern char D_L11_001B0EB0[];
extern char D_L11_001F14B0[];
extern char D_L11_001F14C0[];
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float D_L11_0015F580[] MACRO_ADDR;
extern float D_L11_001677C0_c[] __asm__("D_L11_001677C0");
extern float D_L11_001677D0[];
extern float FUN_001f9b48(void *, void *);
extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern float approach_angle(float *p, float *v, float a, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern int level_frame __asm__("D_L11_0015F5CC");
extern int FUN_001fa728_u(char *, float) __asm__("FUN_001fa728");
extern int random_integer_below_c2(int) __asm__("FUN_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_0025f730(void *a, float x);
extern void update_moby_trail_points(void *) __asm__("FUN_L11_00319670");
extern void update_moby_hit_stats_and_state(void *) __asm__("FUN_L11_0031a1c0");
extern void update_moby_path_transform(void *) __asm__("FUN_L11_0031a438");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern void vu_euler_rotation_basis(void *, void *) __asm__("func_001FA030");
void blend_moby_animation_c(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

extern short D_L11_001623D0 __attribute__((sda));
void blarg_ship_update(struct Moby *moby) __asm__("FUN_L11_00319838");
void blarg_ship_update(struct Moby *moby) {
    enum {
        BLARG_SHIP_INITIALIZE = 0,
        BLARG_SHIP_FOLLOW_PATH = 1,
        BLARG_SHIP_BURST_AND_WAIT_TO_REACTIVATE = 2,
        BLARG_SHIP_WAIT_TO_REACTIVATE = 3,
        BLARG_SHIP_INACTIVE = 4,
        BLARG_SHIP_MANEUVER_AND_FIRE = 5,
        BLARG_SHIP_BURST_AND_DEACTIVATE = 6
    };
    float scratch_vector[4];
    float target_position[4];
    float hero_position_delta[4];
    float basis_rotation_angles[4];
    float basis_row0[4];
    float basis_row1[4];
    float basis_row2[4];
    float direction_to_hero[4];
    float shot_displacement[4];
    float shot_velocity[4];
    float sA0[4];
    float sB0[4];
    float movement_distance_scratch;
    char *moby_vars = (char *)moby->pvars;
    float placement_angle;
    float distance_to_target;
    char *spawned_shot;

    update_moby_hit_stats_and_state(moby);
    switch (moby->state) {
    case BLARG_SHIP_INITIALIZE:
        moby->draw_dist = 0xFF;
        moby->update_dist = 0xFF;
        if (*(int *)(moby_vars + 0x88) == 0) {
            moby->state = 1;
            *(float *)(moby_vars + 0x6C) = randf_sym(0.3f, 0.5f);
            *(float *)(moby_vars + 0x70) = random_angle_radians();
            *(float *)(moby_vars + 0x74) = randf_sym(60.0f, 120.0f) * DEG_TO_RAD * D_0015ED6C_n[1];
            *(float *)(moby_vars + 0x78) = random_float_between_alt(8.0f, 12.0f);
        } else {
            moby->state = 4;
            moby->collision = 0;
            moby->flags |= 1;
        }
        break;
    case BLARG_SHIP_FOLLOW_PATH:
    case BLARG_SHIP_BURST_AND_WAIT_TO_REACTIVATE:
        update_moby_trail_points(moby);
        *(float *)(moby_vars + 0x68) += *(float *)(moby_vars + 0x6C);
        if (*(float *)(moby_vars + 0x68) > 1.0f) {
            *(float *)(moby_vars + 0x68) -= 1.0f;
            *(int *)(moby_vars + 0x64) =
                (*(int *)(moby_vars + 0x64) + 1) % **(int **)(D_L11_001B0EB0 + *(int *)(moby_vars + 0x60) * 4);
        } else if (*(float *)(moby_vars + 0x68) < 0.0f) {
            *(float *)(moby_vars + 0x68) += 1.0f;
            *(int *)(moby_vars + 0x64) =
                (*(int *)(moby_vars + 0x64) + **(int **)(D_L11_001B0EB0 + *(int *)(moby_vars + 0x60) * 4) - 1) %
                **(int **)(D_L11_001B0EB0 + *(int *)(moby_vars + 0x60) * 4);
        }
        fast_add_rotations(*(float *)(moby_vars + 0x70), *(float *)(moby_vars + 0x74));
        update_moby_path_transform(moby);
        if (moby->state == 2) {
            cart_burst(moby, &moby->pos, -1, 2.0f, 13.0f);
            if (random_integer_below_c2(2)) {
                spawn_moby_with_random_yaw(&moby->pos, D_L11_0015F580, 0x4C4);
            } else {
                spawn_moby_with_random_yaw(&moby->pos, D_L11_0015F580, 0x4C2);
            }
            allocate_voice_for_target_entry(1, 0, (int)moby);
            *(unsigned short *)(D_00141C18) = 0xFFFF;
            moby->state = 3;
            moby->collision = 0;
            moby->flags |= 1;
        } else if (moby->visible == 0) {
            if (FUN_001f9b48(&moby->pos, &hero.motion.pos) < 32.0f) {
                moby->state = 3;
                moby->collision = 0;
                moby->flags |= 1;
            }
        }
        break;
    case BLARG_SHIP_WAIT_TO_REACTIVATE:
        qcopy(scratch_vector, &moby->pos);
        scratch_vector[3] = 3.0f;
        if (FUN_001fa728_u((char *)&moby->pos, 512.0f) == -1) {
            if (FUN_001f9b48(&moby->pos, &hero.motion.pos) > 32.0f) {
                *(int *)(moby_vars + 0x84) += 1;
                if (scale_game_frames(60) < *(int *)(moby_vars + 0x84)) {
                    moby->state = 1;
                    if (moby->prev_seq) {
                        blend_moby_animation_c(moby, 0, 0, scale_game_frames(10));
                    }
                    moby->flags &= 0xFFFE;
                    moby->collision = moby->pclass->collision;
                }
            } else {
                *(int *)(moby_vars + 0x84) = 0;
            }
        } else {
            *(int *)(moby_vars + 0x84) = 0;
        }
        break;
    case BLARG_SHIP_INACTIVE:
        break;
    case BLARG_SHIP_MANEUVER_AND_FIRE:
        movement_distance_scratch = 0.0f;
        qcopy(basis_rotation_angles, D_L11_001677D0);
        basis_rotation_angles[0] = 0.0f;
        if (basis_rotation_angles[1] > 0.2617994f) {
            basis_rotation_angles[1] = 0.2617994f;
        } else if (basis_rotation_angles[1] < -0.2617994f) {
            basis_rotation_angles[1] = -0.2617994f;
        }
        vu_euler_rotation_basis(basis_row0, basis_rotation_angles);
        subtract_vector_xyz(hero_position_delta, &hero.motion.pos, moby_vars + 0xB0);
        qcopy(moby_vars + 0xB0, &hero.motion.pos);
        add_vector_xyz(&moby->pos, &moby->pos, hero_position_delta);
        placement_angle = fast_add_rotations(0.7853982f, (float)*(int *)(moby_vars + 0x88) * 1.5707964f);
        normalize_vector_xyz(scratch_vector, basis_row1, fast_sin(placement_angle) * 8.0f);
        add_vector_xyz(target_position, D_L11_001677C0_c, scratch_vector);
        normalize_vector_xyz(scratch_vector, basis_row2, fast_cos(placement_angle) * 8.0f);
        add_vector_xyz(target_position, target_position, scratch_vector);
        normalize_vector_xyz(scratch_vector, basis_row0, 20.0f);
        add_vector_xyz(target_position, target_position, scratch_vector);
        subtract_vector_xyz(scratch_vector, target_position, &moby->pos);
        distance_to_target = vector_length_xyz(scratch_vector);
        if (*(char *)&hero.ship_flags & 2) {
            advance_accelerated_scalar(&movement_distance_scratch, (float *)(moby_vars + 0x6C), distance_to_target, frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f, D_0015ED6C_n[1]);
        } else if (moby->command) {
            advance_accelerated_scalar(&movement_distance_scratch, (float *)(moby_vars + 0x6C), distance_to_target, frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f, D_0015ED6C_n[1] * 15.0f);
        } else {
            advance_accelerated_scalar(&movement_distance_scratch, (float *)(moby_vars + 0x6C), distance_to_target, frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f, D_0015ED6C_n[1] * 5.0f);
        }
        FUN_L00_0025f730(scratch_vector, *(float *)(moby_vars + 0x6C));
        add_vector_xyz(&moby->pos.x, &moby->pos.x, scratch_vector);
        if (moby->command) {
            if (moby->visible) {
                *(int *)(moby_vars + 0x98) += 1;
            }
            if (FUN_001f9b48(target_position, &moby->pos.x) < 2.0f) {
                moby->command = 0;
            } else if (scale_game_frames(120) < *(int *)(moby_vars + 0x98)) {
                moby->command = 0;
            }
        } else {
            float *hero_position = (float *)(&hero.motion.pos);
            float *yaw = &moby->rot.z;
            float *pitch = &moby->rot.y;
            float steering_target_angle;
            subtract_vector_xyz(direction_to_hero, hero_position, &moby->pos.x);
            steering_target_angle = fast_arc_tan(direction_to_hero[0], direction_to_hero[1]);
            approach_angle(yaw, (float *)(moby_vars + 0x8C), steering_target_angle, frame_time_sq * 12.566371f,
                             frame_time_sq * 12.566371f, D_0015ED6C_n[1] * 12.566371f);
            steering_target_angle = -fast_arc_tan(vector_length_xy(direction_to_hero), direction_to_hero[2]);
            approach_angle(pitch, (float *)(moby_vars + 0x90), steering_target_angle, frame_time_sq * 12.566371f,
                             frame_time_sq * 12.566371f, D_0015ED6C_n[1] * 12.566371f);
            if (level_frame % 10 == 0) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                approach_value((float *)(moby_vars + 0x94), 0.0f, 0.25f);
                normalize_vector_xyz(sB0, hero_position_delta, *(float *)(moby_vars + 0x94));
                add_vector_xyz(sB0, sB0, hero_position);
                if (level_frame % 20 == 0) {
                    transform_vector_by_basis(sA0, D_L11_001F14B0, &moby->rot_row0);
                } else {
                    transform_vector_by_basis(sA0, D_L11_001F14C0, &moby->rot_row0);
                }
                add_vector_xyz(sA0, sA0, &moby->pos.x);
                subtract_vector_xyz(shot_displacement, sB0, sA0);
                normalize_vector_xyz(shot_velocity, shot_displacement, *(float *)&D_L11_001623D0 * D_0015ED6C_n[1]);
                add_vector_xyz(shot_velocity, shot_velocity, hero_position_delta);
                spawned_shot = spawn_class_3f9_moby((int)moby, shot_velocity, sA0, 128.0f, -1.0f, 10.0f);
                if (spawned_shot) {
                    subtract_vector_xyz(shot_velocity, shot_velocity, hero_position_delta);
                    *(float *)(spawned_shot + 0x44) = -fast_arc_tan(vector_length_xy(shot_velocity), shot_velocity[2]);
                    *(float *)(spawned_shot + 0x48) = fast_arc_tan(shot_velocity[0], shot_velocity[1]);
                }
            }
            if (moby->visible == 0) {
                moby->state = 4;
                moby->collision = 0;
                moby->flags |= 1;
            }
        }
        break;
    case BLARG_SHIP_BURST_AND_DEACTIVATE:
        *(unsigned short *)(D_00141C18) = 0xFFFF;
        subtract_vector_xyz(scratch_vector, &hero.motion.pos, moby_vars + 0xB0);
        qcopy(moby_vars + 0xB0, &hero.motion.pos);
        add_vector_xyz(&moby->pos, &moby->pos, scratch_vector);
        cart_burst(moby, &moby->pos, -1, 2.0f, 13.0f);
        if (random_integer_below_c2(2)) {
            spawn_moby_with_random_yaw(&moby->pos, D_L11_0015F580, 0x4C4);
        } else {
            spawn_moby_with_random_yaw(&moby->pos, D_L11_0015F580, 0x4C2);
        }
        allocate_voice_for_target_entry(1, 0, (int)moby);
        moby->state = 4;
        moby->collision = 0;
        moby->flags |= 1;
        break;
    }
}
/* Counts the listed mobys of class 0x527 in state 5. */

extern short *D_L11_001AC240[];
extern char *D_L11_0015FFD8;

short count_listed_blarg_ships_in_state_5(int moby_list_index) __asm__("FUN_L11_0031a758");

short count_listed_blarg_ships_in_state_5(int moby_list_index) {
    short *moby_list_entry = D_L11_001AC240[moby_list_index];
    short found = 0;
    short n = 0;
    char *base = D_L11_0015FFD8;
    do {
        unsigned char *moby = (unsigned char *)(((moby_list_entry[0] & 0x7FFF) << 8) + (int)base);
        if (*(short *)(moby + 0xA6) == 0x527 && moby[0x20] == 5) {
            found = ++n;
        }
    } while (*moby_list_entry++ >= 0);
    return found;
}
extern char *D_L11_0015FFD8;
extern char D_L11_001677C0[];
extern f32 D_L11_001677D0[];
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern short *D_L11_001AC240[];
extern s32 random_integer_below_c(s32 arg0) __asm__("FUN_00213260");
extern void blend_moby_animation(void *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

char *activate_listed_blarg_ship(int list) __asm__("FUN_L11_0031a7d8");

char *activate_listed_blarg_ship(int list) {
    short *moby_list_entry = D_L11_001AC240[list];
    float A[4], B[4], C[4], D[4], E[4], F[4];
    char *selected_moby;
    char *moby_vars;
    int moby_byte_offset;

    do {
        moby_byte_offset = (*moby_list_entry & 0x7FFF) << 8;
        if (*(short *)(0xA6 + moby_byte_offset + D_L11_0015FFD8) != 0x527)
            continue;
        if (*(unsigned char *)(0x20 + moby_byte_offset + D_L11_0015FFD8) != 4)
            continue;
        if (random_integer_below_c(10))
            continue;
        selected_moby = D_L11_0015FFD8 + moby_byte_offset;
        moby_vars = *(char **)(selected_moby + 0x78);
        qcopy(C, D_L11_001677D0);
        C[0] = 0;
        if (C[1] > 0.17453292f)
            C[1] = 0.17453292f;
        else if (C[1] < -0.17453292f)
            C[1] = -0.17453292f;
        euler_rotation_basis(D, C);
        {
            float placement_angle = fast_add_rotations(0.7853982f, (float)*(int *)(moby_vars + 0x88) * 1.5707964f);
            fast_vec_normalize(A, E, fast_sin(placement_angle) * 12.0f);
            fast_vec_add(B, D_L11_001677C0, A);
            fast_vec_normalize(A, F, fast_cos(placement_angle) * 12.0f);
        }
        fast_vec_add(B, B, A);
        fast_vec_normalize(A, D, 6.0f);
        fast_vec_add(B, B, A);
        qcopy(moby_vars + 0xB0, D_0013E533 + 0xE9D);
        qcopy(selected_moby + 0x10, B);
        selected_moby[0x20] = 5;
        selected_moby[0xBC] = 1;
        *(float *)(moby_vars + 0x6C) = D_0015ED6C_n[1] * 20.0f;
        *(float *)(moby_vars + 0x94) = 5.0f;
        *(int *)(moby_vars + 0x98) = 0;
        if (*(unsigned char *)(selected_moby + 0x53))
            blend_moby_animation(selected_moby, 0, 0, scale_game_frames(10));
        *(unsigned short *)(selected_moby + 0x34) &= 0xFFFE;
        *(int *)(selected_moby + 0x94) = *(int *)(*(char **)(selected_moby + 0x24) + 0x10);
        return selected_moby;
    } while (*moby_list_entry++ >= 0);
    return 0;
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031BEF0), where it is exact; names translated to the US level program. */

extern int count_group_e81e0(int, int) __asm__("FUN_L01_0026e008");
extern unsigned char D_0013D408[];
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void allocate_voice_for_bank_entry_alt(int, int, int) __asm__("FUN_0022db10");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

void update_moby_list_empty_dialogue_trigger(struct Moby *moby) __asm__("FUN_L11_0031aa80");

void update_moby_list_empty_dialogue_trigger(struct Moby *moby) {
    int *data = (int *)moby->pvars;
    if (data[0] == -1) {
        mark_moby_for_removal(moby);
    } else if (data[1] != -1 && count_group_e81e0(data[1], -1) == 0) {
        unsigned char *dialogue_latch_base = D_0013D408;
        if (dialogue_latch_base[0x13] == 0) {
            dialogue_latch_base[0x13] = 1;
            allocate_voice_for_bank_entry_alt(1, 0, 0);
            queue_dialogue_message(0x53d6, -1);
        }
    }
}
/* Bubble update: rises to the surface, then pops and fades, drifting with its velocity. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031BF78), where it is exact; names translated to the US level program. */

extern float approach_value(float *p, float target, float maxstep);
extern float random_float_between(float a, float b) __asm__("FUN_002132a8");
extern float vector_length_xy(void *);
extern int level_frame __asm__("D_L11_0015F5CC");
extern int random_integer_below_c3(int) __asm__("FUN_00213260");
extern int scale_game_frames(int);
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
void spawn_bubble_surface_effects(struct Moby *source_moby, float *ground, int spark_count, int ground_effect_count) __asm__("FUN_L11_0031ae68");

void spawn_bubble_surface_effects(struct Moby *m, float *ground, int n1, int n2) __asm__("FUN_L11_0031ae68");
extern void FUN_L11_0031b098_c(char *) __asm__("FUN_L11_0031b098");
extern void add_vector_xyz(void *, void *, void *);
extern void scale_vector_xyz(void *, void *, float);
float absolute_float(float input) __asm__("func_001F99C0");
unsigned char *FUN_L00_0026f080(void *pos, float *v, float x, float y);
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern char *func_L00_0026FF20_p(void *, float *, float, float) __asm__("FUN_L00_0026f080");

extern short D_L11_00162400 __attribute__((sda));
extern short D_L11_00162404 __attribute__((sda));
extern short D_L11_00162408 __attribute__((sda));
extern short D_L11_0016240C __attribute__((sda));
extern short D_L11_00162410 __attribute__((sda));
extern short D_L11_00162414 __attribute__((sda));
extern short D_L11_00162418 __attribute__((sda));
extern short D_L11_0016241C __attribute__((sda));
void update_bubble_motion_and_scale(struct Moby *bubble_moby) __asm__("FUN_L11_0031ab08");

void update_bubble_motion_and_scale(struct Moby *bubble_moby) {
    enum {
        BUBBLE_MOVE_UNTIL_SURFACE_CROSSING = 1,
        BUBBLE_SHRINK_TO_REMOVAL = 2
    };
    char *bubble_vars = (char *)bubble_moby->pvars;
    float randomized_effect_vector[4];
    float horizontal_speed;
    float rate;
    char *spawned_effect;
    if (bubble_moby->visible == 0) {
        if (++*(short *)(bubble_vars + 0x22) > scale_game_frames(0xF)) {
            mark_moby_for_removal(bubble_moby);
            return;
        }
    } else {
        *(short *)(bubble_vars + 0x22) = 0;
    }
    switch (bubble_moby->state) {
    case BUBBLE_MOVE_UNTIL_SURFACE_CROSSING:
        *(float *)(bubble_vars + 8) -= *(float *)&D_L11_00162400 * frame_time_sq;
        if (**(float **)(bubble_vars + 0x24) + 3.0f < bubble_moby->pos.z) {
            if (random_integer_below_c3(*(int *)&D_L11_00162410) == 0) {
                FUN_L11_0031b098_c(bubble_moby);
            }
        }
        if (bubble_moby->pos.z < **(float **)(bubble_vars + 0x24)) {
            scale_vector_xyz(bubble_vars, bubble_vars, *(float *)&D_L11_0016240C);
            bubble_moby->state = 2;
            spawn_bubble_surface_effects((struct Moby *)bubble_moby, *(float **)(bubble_vars + 0x24), 8, 4);
        }
        break;
    case BUBBLE_SHRINK_TO_REMOVAL:
        *(float *)(bubble_vars + 8) -= *(float *)&D_L11_00162404 * frame_time_sq;
        rate = bubble_moby->pclass->scale / (float)scale_game_frames(0x3C);
        if ((level_frame & 3) == 0) {
            spawn_bubble_surface_effects((struct Moby *)bubble_moby, *(float **)(bubble_vars + 0x24), 0, 1);
            random_vector(randomized_effect_vector, 0.0f, D_0015ED6C_n[1]);
            randomized_effect_vector[2] = absolute_float(randomized_effect_vector[2]);
            spawned_effect = func_L00_0026FF20_p(&bubble_moby->pos, randomized_effect_vector, *(float *)&D_L11_00162414,
                                    **(float **)(bubble_vars + 0x24));
            if (spawned_effect != 0) {
                *(int *)(spawned_effect + 4) = fast_tween_color(*(int *)&D_L11_00162418, *(int *)&D_L11_0016241C,
                                               random_float_between(0.0f, 1.0f));
            }
        }
        approach_value(&bubble_moby->scale, 0.0f, rate);
        if (bubble_moby->scale == 0.0f) {
            mark_moby_for_removal(bubble_moby);
            return;
        }
        break;
    }
    add_vector_xyz(&bubble_moby->pos, &bubble_moby->pos, bubble_vars);
    horizontal_speed = vector_length_xy(bubble_vars);
    approach_value(&horizontal_speed, 0.0f, *(float *)&D_L11_00162408 * frame_time_sq);
    set_vector_length_xy(bubble_vars, bubble_vars, horizontal_speed);
}
extern void *create_moby() __asm__("FUN_0020c4f8");
extern float random_angle_radians(void) __asm__("FUN_00213308");

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031C210), where it is exact; names translated to the US level program. */

char *FUN_L11_0031ada0(void *position, void *vector, void *owner, float scale) {
    char *moby = create_moby(0x5F4);
    if (moby != 0) {
        char *data = *(char **)(moby + 0x78);
        moby[0x20] = 1;
        qcopy(moby + 0x10, position);
        *(float *)(moby + 0x40) = random_angle_radians();
        *(float *)(moby + 0x48) = random_angle_radians();
        *(short *)(moby + 0x32) = 0xFF;
        ((unsigned char *)moby)[0x30] = 0xFF;
        moby[0x31] = 1;
        *(float *)(moby + 0x2C) = *(float *)(*(char **)(moby + 0x24) + 0x24) * scale;
        *(void **)(data + 0x24) = owner;
        qcopy(data, vector);
    }
    return moby;
}

#define NOT_SDA

#define MACRO_ADDR

/* Bursts n1 sparks and n2 smoke puffs at the moby, the puffs sitting on the ground height. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031C2D8), where it is exact; names translated to the US level program. */

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_L11_0015F580[];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between(float a, float b) __asm__("FUN_002132a8");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern char D_L11_00162420[4] __attribute__((sda));
extern char D_L11_00162424[4] __attribute__((sda));
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void FUN_L00_0026f548(void *, void *, int, int);
extern void add_vector_xyz(void *, void *, void *);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern char *func_L00_00272770_p(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");

void spawn_bubble_surface_effects(struct Moby *source_moby, float *ground, int spark_count, int ground_effect_count) {
    float effect_spawn_scratch[4];
    int i;
    for (i = 0; i < spark_count; i++) {
        float emission_angle = random_angle_radians();
        float horizontal_emission_scale = random_float_between(D_0015ED6C_n[1] * 0.0f, D_0015ED6C_n[1] * 6.0f);
        int k;
        effect_spawn_scratch[0] = fast_cos(emission_angle) * horizontal_emission_scale;
        effect_spawn_scratch[1] = fast_sin(emission_angle) * horizontal_emission_scale;
        effect_spawn_scratch[2] = random_float_between(D_0015ED6C_n[1] * 3.0f, D_0015ED6C_n[1] * 6.5f);
        k = random_int_between(0x5A, 0x78);
        FUN_L00_0026f548(&source_moby->pos, effect_spawn_scratch, random_integer_below(2), k);
    }
    for (i = 0; i < ground_effect_count; i++) {
        char *spawned_ground_effect;
        random_vector(effect_spawn_scratch, 1.0f, 1.0f);
        add_vector_xyz(effect_spawn_scratch, effect_spawn_scratch, &source_moby->pos);
        effect_spawn_scratch[2] = *ground + 0.05f;
        spawned_ground_effect = func_L00_00272770_p(effect_spawn_scratch, D_L11_0015F580, ground, random_float_between(3.0f, 6.0f),
                                i == 0 ? 1.0f : -1.0f);
        if (spawned_ground_effect != 0) {
            spawned_ground_effect[9] = func_001FA898_r(4.0f) + 0x40;
            *(short *)(spawned_ground_effect + 0xA) = func_001FA898_r(multiply_global_factor_ed64(random_float_between(30.0f, 90.0f)));
            *(int *)(spawned_ground_effect + 4) = fast_tween_color(*(int *)&D_L11_00162420, *(int *)&D_L11_00162424,
                                           random_float_between(0.0f, 1.0f));
        }
    }
}
#define NOT_SDA __attribute__((section(".data")))

#define MACRO_ADDR __attribute__((section(".sdata")))

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031C508), where it is exact; names translated to the US level program. */

extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void clear_vector(void *) __asm__("func_001F99F8");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

extern short D_L11_00162428 __attribute__((sda));
extern short D_L11_0016242C __attribute__((sda));
extern short D_L11_00162430 __attribute__((sda));
extern short D_L11_00162434 __attribute__((sda));
extern short D_L11_00162438 __attribute__((sda));
extern short D_L11_0016243C __attribute__((sda));
extern short D_L11_00162440 __attribute__((sda));
extern short D_L11_00162444 __attribute__((sda));
extern short D_L11_00162448 __attribute__((sda));
extern short D_L11_0016244C __attribute__((sda));
extern short D_L11_00162450 __attribute__((sda));
extern short D_L11_00162454 __attribute__((sda));
void FUN_L11_0031b098(struct Moby *source_moby) {
    float randomized_velocity[4];
    float vertical_spark_vector[4];
    char *moby_vars;
    int initial_spark_color, second_spark_color, spark_frame_count, second_spark_frame_count, third_spark_frame_count;
    moby_vars = (char *)source_moby->pvars;
    random_vector(randomized_velocity, 0.0f, *(float *)&D_L11_00162428 * D_0015ED6C_n[1]);
    add_vector_xyz(randomized_velocity, randomized_velocity, moby_vars);
    clear_vector(vertical_spark_vector);
    vertical_spark_vector[2] = random_float_between_alt(*(float *)&D_L11_0016242C, *(float *)&D_L11_00162430) *
            D_0015ED6C_n[1];
    randomized_velocity[3] = *(float *)&D_L11_00162434;
    vertical_spark_vector[3] = *(float *)&D_L11_00162438;
    initial_spark_color = fast_tween_color(*(int *)&D_L11_0016243C, *(int *)&D_L11_00162440,
                      random_float_between_alt(0.0f, 1.0f));
    second_spark_color = fast_tween_color(*(int *)&D_L11_00162444, *(int *)&D_L11_00162448,
                      random_float_between_alt(0.0f, 1.0f));
    spark_frame_count = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(
        (float)*(int *)&D_L11_0016244C * 0.8f, (float)*(int *)&D_L11_0016244C * 1.2f)));
    second_spark_frame_count = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(
        (float)*(int *)&D_L11_00162450 * 0.8f, (float)*(int *)&D_L11_00162450 * 1.2f)));
    third_spark_frame_count = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(
        (float)*(int *)&D_L11_00162454 * 0.8f, (float)*(int *)&D_L11_00162454 * 1.2f)));
    spawn_spark(&source_moby->pos, randomized_velocity, vertical_spark_vector, initial_spark_color, second_spark_color, spark_frame_count, second_spark_frame_count, third_spark_frame_count, -1);
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031ED48), where it is exact; names translated to the US level program. */

extern int D_L11_001FF750_u[] __asm__("D_L11_001FF750");
extern int D_L11_001FF770[];
extern short D_L11_00162530_d __asm__("D_L11_00162530") __attribute__((sda));
extern short D_L11_00162570_d __asm__("D_L11_00162570") __attribute__((sda));
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void FUN_L08_00258090(void *, int, int, void *);

extern short D_L11_00162540 __attribute__((sda));
extern short D_L11_00162550 __attribute__((sda));
void FUN_L11_0031d8d8(int pair_offset_index) {
    int *element_counts = (int *)&D_L11_00162530_d;
    int *triple_word_source_tables = (int *)&D_L11_00162540;
    int *pair_source_tables = (int *)&D_L11_00162550;
    int *word_source_tables = (int *)&D_L11_00162570_d;
    int i;
    for (i = 0; i < 4; i++) {
        FUN_L08_00258090(D_L11_001FF770, pair_source_tables[i], element_counts[i], (char *)D_L11_001FF750_u + pair_offset_index * 8);
        FUN_L00_001fde98(element_counts[i], triple_word_source_tables[i], word_source_tables[i], D_L11_001FF770, 1);
    }
}
extern int D_L11_001624F8 __attribute__((sda));
extern int D_L11_001624FC;
extern int D_L11_00162518 __attribute__((sda));
extern int D_L11_0016251C;
extern int D_L11_00162528 __attribute__((sda));
extern int get_effect_texture_alt_31db50(int) __asm__("FUN_001f44b8");
extern void FUN_001f76a0_31db50(void) __asm__("FUN_001f76a0");
extern void FUN_L11_0031d8d8_c(int) __asm__("FUN_L11_0031d8d8");
extern void FUN_L08_002f0c18(int);
extern void emit_pair_offset_render_batch(int) __asm__("FUN_L08_002f0cf0");
extern void queue_oil_tanker_front_texture_2b_batches(int) __asm__("FUN_L11_0031e858");
extern void emit_oil_tanker_render_batches_with_pair_offset(int) __asm__("FUN_L11_0031e780");
void vu1_add_g_sregister_31db50(s32 a0, s64 a1) __asm__("FUN_00233980");

extern unsigned char D_L11_0016258C[] __attribute__((section(".sdata")));
void draw_level11_layered_effect_batches(void) __asm__("FUN_L11_0031db50");

void draw_level11_layered_effect_batches(void) {
    vu1_add_g_sregister_31db50(0x42, 0x8000000044L);
    vu1_add_g_sregister_31db50(8, 0);
    vu1_add_g_sregister_31db50(0x14, 0xFF9000000260L);
    vu1_add_g_sregister_31db50(0x47, 0x513F1);
    FUN_001f76a0_31db50();
    vu1_add_g_sregister_31db50(6, get_effect_texture_alt_31db50(0x2C));
    vu1_add_g_sregister_31db50(0x42, (long)D_L11_001624F8 << 32 | 0x44);
    FUN_L11_0031d8d8_c(0);
    if (D_L11_0016258C[4] != 0) {
        vu1_add_g_sregister_31db50(6, get_effect_texture_alt_31db50(0x29));
        vu1_add_g_sregister_31db50(0x42, (long)((D_L11_001624FC * D_L11_0016258C[4]) >> 8) << 32 |
                                             0x68);
        FUN_L11_0031d8d8_c(1);
        vu1_add_g_sregister_31db50(0x42, (long)((D_L11_001624FC * D_L11_0016258C[4]) >> 8) << 32 |
                                             0x62);
        FUN_L11_0031d8d8_c(2);
    }
    vu1_add_g_sregister_31db50(6, get_effect_texture_alt_31db50(0x2A));
    vu1_add_g_sregister_31db50(0x42, (long)D_L11_00162518 << 32 | 0x48);
    emit_pair_offset_render_batch(0);
    vu1_add_g_sregister_31db50(0x42, (long)D_L11_0016251C << 32 | 0x48);
    emit_pair_offset_render_batch(1);
    vu1_add_g_sregister_31db50(6, get_effect_texture_alt_31db50(0x2B));
    vu1_add_g_sregister_31db50(0x42, (long)D_L11_00162528 << 32 | 0x48);
    FUN_L08_002f0c18(0);
    vu1_add_g_sregister_31db50(0x47, 0x5360B);
}
/* Level overlay code after gameplay/vendor; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

/* triggers objects matching the moby's class */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_003065F8), where it is exact; names translated to the US level program. */

extern int D_L11_001625B0[2] __attribute__((section(".sdata")));
typedef struct {
    float f;
    int pad;
} Fl;
extern Fl D_L11_0016259C[] __attribute__((section(".sdata")));
extern Fl D_L11_001625AC[] __attribute__((section(".sdata")));
extern Pair D_L11_00162598[] __attribute__((section(".sdata")));
extern Pair D_L11_001625A8[] __attribute__((section(".sdata")));
extern Pair D_L11_001FF750[];
extern char D_L11_001F9A78[];
extern char D_L11_001FF730[];
extern char D_L11_00162530[4] __attribute__((sda));
extern char D_L11_00162570[4] __attribute__((sda));
extern char D_L11_001FF758[];
extern char D_L11_001F9A48[];
extern char D_L11_001F4978[];
extern float D_L11_001624E8 __attribute__((sda));
extern float D_L11_001624EC;
extern float D_L11_001624F0;
extern float D_L11_001624F4;
extern char D_L11_00167680[];
extern int D_L11_0016252C __attribute__((sda));
extern float vector_length_xyz(void *);
extern int fast_bsphere_check(void *, float) __asm__("FUN_001fa728");
extern void draw_level11_layered_effect_batches(void) __asm__("FUN_L11_0031db50");
extern void scale_rgba_tables(void *, int, void *, float, float, float, float, int) __asm__("FUN_L08_002f0f68");
extern void advance_wrapped_sine_pair(void *, float, float, float, void *) __asm__("FUN_L08_002f1158");
extern void FUN_L08_002f12a0(int);
extern void FUN_L08_002f1378(int);
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
extern void subtract_vector_xyz(void *, void *, void *);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

extern unsigned char D_L11_00162590;
void update_level11_layered_effect_and_queue_draw(struct Moby *moby) __asm__("FUN_L11_0031e2f0");

void update_level11_layered_effect_and_queue_draw(struct Moby *moby) {
    enum {
        EFFECT_INITIALIZE = 0,
        EFFECT_UPDATE_AND_QUEUE_DRAW = 1
    };
    int i;
    float effect_bounds[4];
    float effect_to_reference_delta[4];
    float effect_distance;
    char *reference_position_base;

    switch (moby->state) {
    case EFFECT_INITIALIZE:
        if (D_L11_0016252C == 0) {
            scale_rgba_tables(D_L11_00162570, 4, D_L11_00162530, 0.7f, 1.0f, 0.9f, 0.5f, 1);
            scale_rgba_tables(D_L11_001F9A48, 0xB, D_L11_001F4978, 0.5f, 0.5f, 0.5f, 1.0f, 1);
            scale_rgba_tables(D_L11_001FF730, 8, D_L11_001F9A78, 0.25f, 0.25f, 0.25f, 0.7f, 1);
            D_L11_0016252C = 1;
        }
        moby->state = 1;
        for (i = 0; i < 3; i++) {
            D_L11_001FF750[i].a = 0;
            D_L11_001FF750[i].b = 0;
            D_L11_001625B0[0] = 0;
            D_L11_001625B0[1] = 0;
        }
        for (i = 0; i < 2; i++) {
            D_L11_00162598[i].a = 0;
            D_L11_0016259C[i].f = i * 0.4f;
        }
        for (i = 0; i < 1; i++) {
            D_L11_001625A8[i].a = 0;
            D_L11_001625AC[i].f = i * 0.4f;
        }
        break;
    case EFFECT_UPDATE_AND_QUEUE_DRAW:
        reference_position_base = D_L11_00167680;
        if (*(float *)(reference_position_base + 0x148) < 170.0f)
            return;
        effect_bounds[0] = D_L11_001624E8;
        effect_bounds[1] = D_L11_001624EC;
        effect_bounds[2] = D_L11_001624F0;
        effect_bounds[3] = D_L11_001624F4;
        if (fast_bsphere_check(effect_bounds, 1000.0f) < 0)
            return;
        if (!(180.0f < *(float *)(reference_position_base + 0x148)))
            return;
        effect_to_reference_delta[0] = D_L11_001624E8;
        effect_to_reference_delta[1] = D_L11_001624EC;
        effect_to_reference_delta[2] = D_L11_001624F0;
        effect_to_reference_delta[3] = 1.0f;
        subtract_vector_xyz(effect_to_reference_delta, reference_position_base + 0x140, effect_to_reference_delta);
        effect_distance = vector_length_xyz(effect_to_reference_delta);
        if (effect_distance <= 35.0f) {
            FUN_L08_002f12a0(0);
            advance_wrapped_sine_pair(D_L11_001FF758, 0.52f, 0.62f, 0.06f, (void *)D_L11_001625B0);
            FUN_L08_002f12a0(2);
            if (effect_distance < 15.0f) {
                D_L11_00162590 = 0xFF;
            } else if (effect_distance < 35.0f) {
                D_L11_0016258C[4] = func_001FA898_r((1.0f - (effect_distance - 15.0f) / 20.0f) * 255.0f);
            } else {
                D_L11_00162590 = 0;
            }
        }
        FUN_L08_002f1378(0);
        FUN_L08_002f1378(0);
        FUN_L08_002f1378(1);
        enqueue_callback_list_1(draw_level11_layered_effect_batches, moby);
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_0031EFC0.c: func_L11_0031FDA0), where it is exact; names translated to the US level program. */

extern char D_L11_00167680[];
extern int D_L11_001625CC;
extern int D_L11_001625EC;
extern int D_L11_00162610; /* no foreign declaration */
extern int get_effect_texture(int) __asm__("FUN_001f44b8");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern char D_L11_001625C8[4] __attribute__((sda));
extern char D_L11_001625E8[4] __attribute__((sda));
extern char D_L11_001625F8[4] __attribute__((sda));
extern void draw_scrolled_mesh_batch(int arg) __asm__("FUN_L08_002f0b40");
extern void FUN_L08_002f0c18(int);
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void vu1_add_g_sregister(int, long) __asm__("FUN_00233980");

void FUN_L11_0031e930(struct Moby *moby) {
    int *moby_vars = (int *)moby->pvars;
    int flag = 0;
    char *reference_position_base;
    vu1_add_g_sregister(0x42, 0x8000000044L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1L);
    font_queue_vu_state();
    reference_position_base = D_L11_00167680;
    if (*(float *)(reference_position_base + 0x148) > 255.0f || is_point_inside_clip_volume(reference_position_base + 0x140, moby_vars[0]) ||
        is_point_inside_clip_volume(reference_position_base + 0x140, moby_vars[1]) ||
        is_point_inside_clip_volume(reference_position_base + 0x140, moby_vars[2])) {
        flag = 1;
    }
    if (flag) {
        vu1_add_g_sregister(6, get_effect_texture(0x2C));
        vu1_add_g_sregister(0x42, ((long)*(int *)&D_L11_001625C8 << 32) | 0x44);
        draw_scrolled_mesh_batch(0);
    }
    if (*(unsigned char *)&D_L11_00162610 != 0 && flag) {
        vu1_add_g_sregister(6, get_effect_texture(0x29));
        vu1_add_g_sregister(
            0x42, ((long)((D_L11_001625CC * *(unsigned char *)&D_L11_00162610) >> 8) << 32) | 0x68);
        draw_scrolled_mesh_batch(1);
        vu1_add_g_sregister(
            0x42, ((long)((D_L11_001625CC * *(unsigned char *)&D_L11_00162610) >> 8) << 32) | 0x62);
        draw_scrolled_mesh_batch(2);
    }
    vu1_add_g_sregister(6, get_effect_texture(0x2A));
    vu1_add_g_sregister(0x42, ((long)*(int *)&D_L11_001625E8 << 32) | 0x48);
    emit_oil_tanker_render_batches_with_pair_offset(0);
    vu1_add_g_sregister(0x42, ((long)D_L11_001625EC << 32) | 0x48);
    emit_oil_tanker_render_batches_with_pair_offset(1);
    if (flag) {
        vu1_add_g_sregister(6, get_effect_texture(0x2B));
        vu1_add_g_sregister(0x42, ((long)*(int *)&D_L11_001625F8 << 32) | 0x48);
        queue_oil_tanker_front_texture_2b_batches(0);
    }
}
/* Level overlay code after gameplay/vendor; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

/* triggers objects matching the moby's class */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_003065F8), where it is exact; names translated to the US level program. */

typedef struct {
    int a, b;
} PairB;

extern int D_L11_00162630[2] __attribute__((section(".sdata")));
typedef struct {
    float f;
    int pad;
} FlB;
extern FlB D_L11_0016261C[] __attribute__((section(".sdata")));
extern FlB D_L11_0016262C[] __attribute__((section(".sdata")));
extern PairB D_L11_00162618[] __attribute__((section(".sdata")));
extern PairB D_L11_00162628[] __attribute__((section(".sdata")));
extern PairB D_L11_00217758[];
extern s32 D_L11_00200728[];
extern s32 D_L11_002078D8[];
extern s32 D_L11_00207900[];
extern s32 D_L11_00215220[];
extern char D_L11_00217760[];
extern s32 D_L11_00217740[];
extern s32 D_L11_00215268[];
extern float D_L11_001625B8 __attribute__((sda));
extern float D_L11_001625BC;
extern float D_L11_001625C0;
extern float D_L11_001625C4;
extern char D_L11_00167680[];
extern int D_L11_001625FC __attribute__((sda));
extern unsigned char D_L11_00162610_c __asm__("D_L11_00162610");
extern float vector_length_xyz(void *);
extern int fast_bsphere_check(void *, float) __asm__("FUN_001fa728");
extern void FUN_L11_0031e930_c(void) __asm__("FUN_L11_0031e930");
extern void scale_rgba_tables(void *, int, void *, float, float, float, float, int) __asm__("FUN_L08_002f0f68");
extern void advance_wrapped_sine_pair(void *, float, float, float, void *) __asm__("FUN_L08_002f1158");
extern void FUN_L08_002f12a0(int);
extern void FUN_L08_002f1378(int);
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
extern void subtract_vector_xyz(void *, void *, void *);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void update_level11_distance_faded_effect_and_queue_draw(struct Moby *moby) __asm__("FUN_L11_0031f150");

void update_level11_distance_faded_effect_and_queue_draw(struct Moby *moby) {
    enum {
        EFFECT_INITIALIZE = 0,
        EFFECT_UPDATE_AND_QUEUE_DRAW = 1
    };
    int i;
    float effect_bounds[4];
    float effect_to_reference_delta[4];
    float effect_distance;
    char *reference_position_base;

    switch (moby->state) {
    case EFFECT_INITIALIZE:
        if (D_L11_001625FC == 0) {
            scale_rgba_tables(D_L11_00215220, 17, D_L11_00207900, 0.7f, 1.0f, 0.9f, 0.5f, 1);
            scale_rgba_tables(D_L11_00217740, 5, D_L11_00215268, 0.5f, 0.5f, 0.5f, 1.0f, 1);
            scale_rgba_tables(D_L11_002078D8, 9, D_L11_00200728, 0.25f, 0.25f, 0.25f, 0.7f, 1);
            D_L11_001625FC = 1;
        }
        moby->state = 1;
        for (i = 0; i < 3; i++) {
            D_L11_00217758[i].a = 0;
            D_L11_00217758[i].b = 0;
            D_L11_00162630[0] = 0;
            D_L11_00162630[1] = 0;
        }
        for (i = 0; i < 2; i++) {
            D_L11_00162618[i].a = 0;
            D_L11_0016261C[i].f = i * 0.4f;
        }
        for (i = 0; i < 1; i++) {
            D_L11_00162628[i].a = 0;
            D_L11_0016262C[i].f = i * 0.4f;
        }
        break;
    case EFFECT_UPDATE_AND_QUEUE_DRAW:
        reference_position_base = D_L11_00167680;
        if (*(float *)(reference_position_base + 0x148) < 170.0f)
            return;
        effect_bounds[0] = D_L11_001625B8;
        effect_bounds[1] = D_L11_001625BC;
        effect_bounds[2] = D_L11_001625C0;
        effect_bounds[3] = D_L11_001625C4;
        if (fast_bsphere_check(effect_bounds, 1000.0f) < 0)
            return;
        if (!(180.0f < *(float *)(reference_position_base + 0x148)))
            return;
        effect_to_reference_delta[0] = D_L11_001625B8;
        effect_to_reference_delta[1] = D_L11_001625BC;
        effect_to_reference_delta[2] = D_L11_001625C0;
        effect_to_reference_delta[3] = 1.0f;
        subtract_vector_xyz(effect_to_reference_delta, reference_position_base + 0x140, effect_to_reference_delta);
        effect_distance = vector_length_xyz(effect_to_reference_delta);
        if (effect_distance <= 100.0f) {
            FUN_L08_002f12a0(0);
            advance_wrapped_sine_pair(D_L11_00217760, 0.52f, 0.62f, 0.06f, (void *)D_L11_00162630);
            FUN_L08_002f12a0(2);
            if (effect_distance < 80.0f) {
                D_L11_00162610_c = 0xFF;
            } else if (effect_distance < 100.0f) {
                D_L11_00162610_c = func_001FA898_r((1.0f - (effect_distance - 80.0f) / 20.0f) * 255.0f);
            } else {
                D_L11_00162610_c = 0;
            }
        }
        FUN_L08_002f1378(0);
        FUN_L08_002f1378(0);
        FUN_L08_002f1378(1);
        enqueue_callback_list_1(FUN_L11_0031e930_c, moby);
        break;
    }
}
