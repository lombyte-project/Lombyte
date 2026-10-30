/* Level 14 flying car moby class: path setup, motion and linked record seeding. */
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/camera/update_cam.h"

extern u8 *D_L14_00167500 NOT_SDA;

void seed_linked_record_for_flying_car(u8 *flying_car_moby) __asm__("FUN_L14_003135b8");

#include "rnc/overlay/l14/mobys/flying_car_globals.h"

int voice_is_playing(int a, int i) __asm__("FUN_L00_0028d8c0");

typedef struct {
    char          pad0[2];
    unsigned char b2;
    char          pad3[0x1D];
    float         f20;
    float         f24;
    float         f28;
    char          pad2C[0x14];
} Fx_6ee0;

typedef struct {
    char          pad0[0x60];
    unsigned char effect[0x10]; /* 0x60 */
    int           f70;
    int           f74;
    int           path;  /* 0x78 */
    int           sound; /* 0x7C */
    float         f80;
    float         f84;
    float         f88;
    short         timer; /* 0x8C */
    char          pad8E[2];
    float         f90;
    float         f94;
    float         f98;
    char          pad9C[4];
    int           part; /* 0xA0 */
    float         fA4;
    float         fA8;
    float         fAC;
    int           fB0;
    int           fB4;
    int           fB8;
    int           fBC;
    int           fC0;
    int           trigger; /* 0xC4 */
    int           fC8;
    int           fCC;
    Fx_6ee0       fx; /* 0xD0 */
} Data_6ee0;

typedef struct {
    char  pad0[0x20];
    float f20;
    float f24;
    float f28;
    float f2C;
} Part_6ee0;

typedef struct {
    char          pad0[0x74];
    unsigned char b74;
    char          pad75[0x13];
    struct Moby  *f88;
} Snd_6ee0;

typedef struct {
    char  pad0[0x10];
    Vec4f a;
    Vec4f b;
} Path_6ee0;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

extern void cart_burst(void *, void *, float, float, int) __asm__("FUN_L00_0025f090");

extern int FUN_001e93b0_6ee0() __asm__("FUN_001e93b0");

extern void FUN_L14_00307510_6ee0(void *) __asm__("FUN_L14_00307510");

extern void fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");

extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");

extern void update_moby_rotation_oscillation(void *, void *, void *, float, float, float) __asm__("FUN_L00_00262b80");

extern int FUN_L14_00307620_6ee0(void *) __asm__("FUN_L14_00307620");

char D_L14_001FD118[] = "Flying Car (moby1417) with no flyPath. UID: %d - RAR\n";

char D_L14_001FD150[] = "Flying Car (moby1417) with a flyPath that has no points. UID: %d - RAR\n";

extern int flying_car_path_reset_delay_frames_gp __asm__("D_L14_0016227C") __attribute__((sda));

extern char *D_L14_0015FFD8_macro __asm__("D_L14_0015FFD8") MACRO_ADDR;

extern f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;

void update_flying_car(struct Moby *m) __asm__("FUN_L14_00306ee0");

void update_flying_car(struct Moby *flying_car_moby) {
    enum {
        FLYING_CAR_INITIALIZE = 0,
        FLYING_CAR_WAIT_FOR_TRIGGER = 1,
        FLYING_CAR_FOLLOW_PATH = 2,
        FLYING_CAR_WAIT_FOR_PATH_RESET = 3
    };
    Data_6ee0 *flying_car_data = (Data_6ee0 *)flying_car_moby->pvars;
    Vec4f      attached_part_offset_component __attribute__((aligned(16)));
    char      *hit;

    hit = find_moby_hit(flying_car_moby, 0x800000, 0);
    flying_car_moby->hit_slot = 0xFF;
    if (hit && flying_car_moby->state != 3) {
        flying_car_moby->state = 3;
        cart_burst(flying_car_moby, &flying_car_moby->pos, 5.0f, 13.0f, -1);
        flying_car_moby->flags |= 0x41;
        flying_car_moby->collision = 0;
        if (flying_car_data->part >= 0) {
            struct Moby *o = (struct Moby *)(D_L14_0015FFD8_macro + (flying_car_data->part << 8));
            o->pos.x = o->pos.y = o->pos.z = 5.0f;
            o->flags |= 0x41;
        }
        if (flying_car_data->sound != -1) {
            Snd_6ee0 *voice_entry = (Snd_6ee0 *)((u8 *)&voice_pool + flying_car_data->sound * 0x70);
            if (voice_entry->f88 == flying_car_moby && voice_entry->b74 != 0) {
                release_voice_slot(flying_car_data->sound);
            }
        }
        flying_car_data->sound = -1;
    }
    switch (flying_car_moby->state) {
    case FLYING_CAR_INITIALIZE:
        if (flying_car_data->path == -1) {
            FUN_001e93b0_6ee0(D_L14_001FD118, (s16)flying_car_moby->save_id);
            mark_moby_for_removal(flying_car_moby);
            return;
        }
        if (*(int *)l14_paths[flying_car_data->path] == 0) {
            FUN_001e93b0_6ee0(D_L14_001FD150, (s16)flying_car_moby->save_id);
            mark_moby_for_removal(flying_car_moby);
            return;
        }
        FUN_L14_00307510_6ee0(flying_car_moby);
        flying_car_moby->state = 2;
        flying_car_moby->update_dist = 0xFF;
        flying_car_moby->draw_dist = 0xFF;
        flying_car_data->sound = -1;
        flying_car_data->timer = scale_game_frames(flying_car_path_reset_delay_frames_gp);
        flying_car_data->fB0 = 0;
        flying_car_data->fB4 = 0;
        flying_car_data->fBC = 0;
        flying_car_data->fC0 = 0;
        flying_car_data->fCC = 0;
        if (flying_car_data->part >= 0) {
            struct Moby *o = (struct Moby *)(D_L14_0015FFD8_macro + (flying_car_data->part << 8));
            o->draw_dist = (u16)flying_car_moby->draw_dist;
            o->update_dist = flying_car_moby->update_dist;
        }
        fill_transfer_words(&flying_car_data->fx, 0, 0x40);
        attach_manipulator(flying_car_moby, 0, &flying_car_data->fx);
        trooper_anim_update(flying_car_moby, flying_car_data->effect);
        if (flying_car_data->trigger >= 0) {
            flying_car_moby->flags |= 0x41;
            flying_car_moby->collision = 0;
            if (flying_car_data->part >= 0) {
                struct Moby *o =
                    (struct Moby *)(D_L14_0015FFD8_macro + (flying_car_data->part << 8));
                o->flags |= 0x41;
            }
            flying_car_moby->state = 1;
        }
        break;
    case FLYING_CAR_WAIT_FOR_TRIGGER:
        if (is_point_inside_clip_volume(&hero.motion.pos, flying_car_data->trigger)) {
            flying_car_moby->flags &= 0xFFBE;
            flying_car_moby->collision = flying_car_moby->pclass->collision;
            if (flying_car_data->part >= 0) {
                struct Moby *o =
                    (struct Moby *)(D_L14_0015FFD8_macro + (flying_car_data->part << 8));
                o->flags &= 0xFFBE;
            }
            flying_car_moby->state = 2;
        }
        break;
    case FLYING_CAR_FOLLOW_PATH:
        flying_car_data->fx.b2 = 1;
        flying_car_data->fx.f20 = flying_car_data->fx.f24 = flying_car_data->fx.f28 = 0.0001f;
        update_moby_rotation_oscillation(flying_car_moby,
                                         &flying_car_data->fBC,
                                         &flying_car_data->fC0,
                                         flying_car_rotation_oscillation_amplitude_gp,
                                         flying_car_rotation_oscillation_common_phase_update_gp,
                                         flying_car_rotation_oscillation_axis_phase_update_gp);
        if (FUN_L14_00307620_6ee0(flying_car_moby)) {
            if (flying_car_data->part >= 0) {
                struct Moby *o =
                    (struct Moby *)(D_L14_0015FFD8_macro + (flying_car_data->part << 8));
                fast_vec_scale(&attached_part_offset_component,
                               &flying_car_moby->rot_row0,
                               flying_car_data->fA4);
                fast_vec_add(&o->pos, &flying_car_moby->pos, &attached_part_offset_component);
                fast_vec_scale(&attached_part_offset_component,
                               &flying_car_moby->rot_row1,
                               flying_car_data->fA8);
                fast_vec_add(&o->pos, &o->pos, &attached_part_offset_component);
                fast_vec_scale(&attached_part_offset_component,
                               &flying_car_moby->rot_row2,
                               flying_car_data->fAC);
                fast_vec_add(&o->pos, &o->pos, &attached_part_offset_component);
                if ((hero.state.control_mode == 13 && hero.grapple_target == o) ||
                    (hero.state.control_mode == 14 && hero.swing_target_moby == o)) {
                    seed_linked_record_for_flying_car(flying_car_moby);
                    flying_car_data->fCC = scale_game_frames(0x1E);
                } else {
                    fast_dec_timer(&flying_car_data->fCC);
                    if (flying_car_data->fCC != 0) {
                        ((Part_6ee0 *)o->pvars)->f20 = 2.0f;
                    } else {
                        Part_6ee0 *pd = (Part_6ee0 *)o->pvars;
                        pd->f20 = 0.0f;
                        if (o->oclass == 0x323) {
                            pd->f20 = 40.0f;
                            pd->f24 = 0.01f;
                            pd->f28 = 0.2f;
                            pd->f2C = frame_time_macro * 24.0f;
                        }
                    }
                }
            }
        }
        break;
    case FLYING_CAR_WAIT_FOR_PATH_RESET:
        if (fast_dec_timer_s16(&flying_car_data->timer)) {
            Path_6ee0 *path = ((Path_6ee0 *)l14_paths[flying_car_data->path]);
            set_moby_color(flying_car_moby,
                           flying_car_data->effect[4],
                           flying_car_data->effect[5],
                           flying_car_data->effect[6]);
            flying_car_moby->flags &= 0xFFBE;
            flying_car_moby->collision = flying_car_moby->pclass->collision;
            flying_car_data->timer = scale_game_frames(flying_car_path_reset_delay_frames_gp);
            flying_car_moby->state = 2;
            flying_car_data->f70 = 0;
            flying_car_data->f74 = 0;
            qcopy(&flying_car_moby->pos, &path->a);
            flying_car_moby->rot.z = fast_arc_tan(path->b.x - path->a.x, path->b.y - path->a.y);
            flying_car_data->f80 = flying_car_data->f84 = flying_car_data->f88 = 0.0f;
            flying_car_data->f90 = flying_car_data->f94 = flying_car_data->f98 = 0.0f;
            if (flying_car_data->part >= 0) {
                struct Moby *o =
                    (struct Moby *)(D_L14_0015FFD8_macro + (flying_car_data->part << 8));
                o->flags &= 0xFFBE;
                fast_vec_scale(&attached_part_offset_component,
                               &flying_car_moby->rot_row0,
                               flying_car_data->fA4);
                fast_vec_add(&o->pos, &flying_car_moby->pos, &attached_part_offset_component);
                fast_vec_scale(&attached_part_offset_component,
                               &flying_car_moby->rot_row1,
                               flying_car_data->fA8);
                fast_vec_add(&o->pos, &o->pos, &attached_part_offset_component);
                fast_vec_scale(&attached_part_offset_component,
                               &flying_car_moby->rot_row2,
                               flying_car_data->fAC);
                fast_vec_add(&o->pos, &o->pos, &attached_part_offset_component);
            }
            if (flying_car_data->trigger >= 0) {
                flying_car_moby->flags |= 0x41;
                flying_car_moby->state = 1;
                flying_car_moby->collision = 0;
            }
        }
        break;
    }
    if (flying_car_moby->state == 0 || flying_car_moby->state == 3 || flying_car_moby->state == 1) {
        return;
    }
    if (voice_is_playing(flying_car_moby, flying_car_data->sound) == 0) {
        flying_car_data->sound = allocate_voice_for_target_entry(0, 4, flying_car_moby);
    }
}

void initialize_flying_car_path(struct Moby *moby) __asm__("FUN_L14_00307510");

void initialize_flying_car_path(struct Moby *moby) {
    char *flying_car_data = (char *)moby->pvars;
    char *path = ((char *)l14_paths[*(int *)(flying_car_data + 0x78)]);
    int   i;
    for (i = 0; i < *(int *)path; i++) {
        int   point_byte_offset = i * 16;
        float segment_length = fast_vec_dist(path + (point_byte_offset + 0x10),
                                             path + (((i + 1) % *(int *)path) * 16 + 0x10));
        *(float *)(path + point_byte_offset + 0x1C) = segment_length;
    }
    *(int *)(flying_car_data + 0x70) = 0;
    *(int *)(flying_car_data + 0x74) = 0;
    qcopy(&moby->pos, path + 0x10);
    moby->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                               *(float *)(path + 0x24) - *(float *)(path + 0x14));
    *(float *)(flying_car_data + 0x90) = *(float *)(flying_car_data + 0x94) =
        *(float *)(flying_car_data + 0x98) = *(float *)(flying_car_data + 0x80) =
            *(float *)(flying_car_data + 0x84) = *(float *)(flying_car_data + 0x88) = 0.0f;
}

extern char *D_L14_0015FFD8_7620 __asm__("D_L14_0015FFD8");

extern float D_L14_00162274 __attribute__((sda));

extern int sample_path(char *path, float *out, void *pos, void *t, int dir, float step) __asm__("FUN_L00_0025d808");

extern void cart_burst(void *, void *, f32, f32, s32) __asm__("FUN_L00_0025f090");

extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);

extern f32 vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");

int update_flying_car_path_motion(struct Moby *moby) __asm__("FUN_L14_00307620");

int update_flying_car_path_motion(struct Moby *moby) {
    float pos[4];
    float delta[4];
    char *flying_car_data = (char *)moby->pvars;
    float yaw;
    float bank;
    float pitch;
    int   end;
    int   voice_slot_index;

    end = sample_path(((char *)l14_paths[*(int *)(flying_car_data + 0x78)]),
                      pos,
                      flying_car_data + 0x70,
                      flying_car_data + 0x74,
                      *(short *)(flying_car_data + 0x8E),
                      *(float *)(flying_car_data + 0xC8) * frame_time);
    if ((*(short *)(flying_car_data + 0x8E) == 0 && end != 0) ||
        moby->pos.z <= flying_car_burst_height_threshold_gp) {
        cart_burst(moby, &moby->pos, 5.0f, 13.0f, -1);
        moby->state = 3;
        moby->flags |= 0x41;
        moby->collision = 0;
        if (*(int *)(flying_car_data + 0xA0) >= 0) {
            struct Moby *attached_part_moby =
                (struct Moby *)(D_L14_0015FFD8_7620 + *(int *)(flying_car_data + 0xA0) * 256);
            attached_part_moby->flags |= 0x41;
            attached_part_moby->pos.z = 5.0f;
            attached_part_moby->pos.y = 5.0f;
            attached_part_moby->pos.x = 5.0f;
        }
        voice_slot_index = *(int *)(flying_car_data + 0x7C);
        if (voice_slot_index != -1) {
            VoicePoolWindow *voice_entry =
                (VoicePoolWindow *)((u8 *)&voice_pool + voice_slot_index * 0x70);
            if (voice_entry->voice.owner == (VoiceMoby *)moby && voice_entry->voice.state != 0)
                release_voice_slot(voice_slot_index);
        }
        *(int *)(flying_car_data + 0x7C) = -1;
        return 0;
    }
    if (((unsigned)(hero.state.current - 0x25) < 2 && *(int *)(flying_car_data + 0xA0) >= 0 &&
         hero.grapple_target ==
             (struct Moby *)(D_L14_0015FFD8_7620 + *(int *)(flying_car_data + 0xA0) * 256)) ||
        (hero.state.control_mode == 0xE && *(int *)(flying_car_data + 0xA0) >= 0 &&
         hero.swing_target_moby ==
             (struct Moby *)(D_L14_0015FFD8_7620 + *(int *)(flying_car_data + 0xA0) * 256))) {
        float lim;
        float s;
        s = *(float *)(flying_car_data + 0xB4) +
            *(float *)(flying_car_data + 0xB8) * frame_scale_sq;
        lim = flying_car_max_sink_offset_rate_gp * frame_time;
        *(float *)(flying_car_data + 0xB4) = s;
        if (lim < s)
            *(float *)(flying_car_data + 0xB4) = lim;
        *(float *)(flying_car_data + 0xB0) += *(float *)(flying_car_data + 0xB4);
    } else {
        *(float *)(flying_car_data + 0xB0) -= flying_car_sink_offset_recovery_rate_gp * frame_time;
        *(int *)(flying_car_data + 0xB4) = 0;
        if (*(float *)(flying_car_data + 0xB0) < *(float *)(flying_car_data + 0xB4))
            *(float *)(flying_car_data + 0xB0) = *(float *)(flying_car_data + 0xB4);
    }
    pos[2] -= *(float *)(flying_car_data + 0xB0);
    if (pos[2] < 10.0f)
        pos[2] = 10.0f;
    vendor_spring(&moby->pos.x,
                  (f32 *)(flying_car_data + 0x80),
                  pos[0],
                  flying_car_tracking_spring_gain_gp,
                  flying_car_motion_spring_damping_gp,
                  0.0f);
    vendor_spring(&moby->pos.y,
                  (f32 *)(flying_car_data + 0x84),
                  pos[1],
                  flying_car_tracking_spring_gain_gp,
                  flying_car_motion_spring_damping_gp,
                  0.0f);
    vendor_spring(&moby->pos.z,
                  (f32 *)(flying_car_data + 0x88),
                  pos[2],
                  flying_car_tracking_spring_gain_gp,
                  flying_car_motion_spring_damping_gp,
                  0.0f);
    fast_vec_sub(delta, pos, &moby->pos);
    if (0.01f < absolute_float(delta[0]) && 0.01f < absolute_float(delta[1])) {
        yaw = fast_arc_tan(delta[0], delta[1]);
        bank = fast_subtract_rotations(moby->rot.z, yaw);
        FUN_L00_00258278(moby,
                         yaw,
                         (f32 *)(flying_car_data + 0x98),
                         flying_car_tracking_spring_gain_gp,
                         flying_car_motion_spring_damping_gp,
                         0.0f);
        pitch = -fast_arc_tan(vector_length_xy(delta), delta[2]);
        vendor_spring(&moby->rot.y,
                      (f32 *)(flying_car_data + 0x94),
                      pitch,
                      flying_car_tracking_spring_gain_gp,
                      flying_car_motion_spring_damping_gp,
                      0.0f);
        bank /= flying_car_full_bank_yaw_error_gp;
        if (1.0f < bank)
            bank = 1.0f;
        if (bank < -1.0f)
            bank = -1.0f;
        vendor_spring(&moby->rot.x,
                      (f32 *)(flying_car_data + 0x90),
                      bank * D_L14_00162274,
                      0.003f,
                      flying_car_motion_spring_damping_gp,
                      0.0f);
    }
    return 1;
}

void seed_linked_record_for_flying_car(u8 *flying_car_moby) {
    u8 *link_source_record = D_L14_00167500;
    u8 *linked_subrecord;
    u8 *linked_record;

    if (*(s16 *)(link_source_record + 0x86) != 7) {
        return;
    }
    linked_subrecord = *(u8 **)(link_source_record + 0x70) + 0x40;
    *(s32 *)(linked_subrecord + 0x40) = 1;
    *(s32 *)(linked_subrecord + 0x44) = (s32)flying_car_moby;
    *(f32 *)(linked_subrecord + 0x4C) = 2.0f;
    linked_record = *(u8 **)(link_source_record + 0x70);
    *(f32 *)(linked_record + 0xA0) = 7.0f;
    *(f32 *)(linked_record + 0xB0) = 2.0f;
}

extern int sample_path(char *path, float *out, void *seg, void *t, int dir, float step) __asm__("FUN_L00_0025d808");

int flying_car_path_reset_delay_frames_gp __asm__("D_L14_0016227C") = 0x1f4;
