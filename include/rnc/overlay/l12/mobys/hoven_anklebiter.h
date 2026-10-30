#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_HOVEN_ANKLEBITER_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_HOVEN_ANKLEBITER_H

#include "types.h"
#include "rnc/mobys/moby.h"

/* Level 12 hoven_anklebiter: its moby and pvars layouts and the target report it shares with the blarg trooper. */

typedef struct AnklebiterData AnklebiterData;

typedef struct AnklebiterMoby {
    char            p0[0x10];
    float           pos[4];
    unsigned char   state, group;
    char            p22[2];
    char           *model;
    char            p28[9];
    unsigned char   enabled;
    unsigned short  joints, flags;
    char            p36[0xA];
    float           rot[4];
    char            p50[2];
    unsigned char   anim_end, anim;
    float           anim_time, anim_rate;
    char            p5C[0x14];
    unsigned char   anim_flags;
    char            p71[7];
    AnklebiterData *data;
    char            p7C[3];
    unsigned char   active;
    char            p80[0x14];
    int             collision;
    char            p98[0x24];
    unsigned char   proximity;
} AnklebiterMoby;

struct AnklebiterData {
    char          p0[0x20];
    float         health;
    char          p24[5];
    unsigned char mode;
    char          p2A[0x2E];
    unsigned char blend;
    char          p59;
    unsigned char count;
    char          p5B[0x6D];
    short         stuck;
    char          pCA[6];
    void         *tuning;
    char          pD4[0x4C];
    float         fall[4], gravity, zero, launch_speed, launch_time;
    int           fall_flags, fall_mode;
    float         damping;
    char          p14C[0x11];
    unsigned char falling;
    char          p15E[0x12];
    float         fall_a, fall_b;
    char          p178[8];
    int           effect_class;
    char          p184[4];
    float         effect_size0, effect_size1;
    char          p190[0x14];
    float         effect_speed;
    char          p1A8[0x10];
    int           effect_flags;
    char          p1BC[0x14];
    float         idle_pos[4], spare, idle_speed;
    char          p1E8[0x18];
    char          collision_data[0x68];
    float         look_angle;
    char          p26C[0x14];
    float         home[4];
    char          p290[0x10];
    float         turn_velocity, circle_angle, circle_rate, range;
    char          p2B0[8];
    int           init0, sleep, circle_timer, alert_timer;
    short         group_timer, pad2CA, idle_timer, pad2CE;
    int           path, blocked, return_timer, group_enabled;
    int          *group_count;
    int           group_check;
};

/* What the target picker FUN_L01_00274b78 fills in: the anklebiter and the blarg trooper read the chosen moby. */
typedef struct TargetReport {
    float           v0[4], v10[4], v20[4], v30[4];
    AnklebiterMoby *moby;
    int             mode;
    char            pad48[8];
} TargetReport;

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_HOVEN_ANKLEBITER_H */
