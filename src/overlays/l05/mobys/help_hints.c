/* Level 05 moby class help_hints (0x543): plays the Rilgar help messages and records the usage stats they depend on. */
#include "types.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/hero/hero.h"
#include "rnc/game/usage_stats.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/l05/mobys/help_hints_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern char D_0013F3D0[];
extern int  camera_secondary_mode_lui __asm__("D_001413D4") NOT_SDA, D_001413DC NOT_SDA,
    D_0013D464 NOT_SDA;
extern unsigned char menu_item_enabled_1 __asm__("D_0013D395"), D_0013D4D6, D_0013D388, D_0013D4CF;
extern int           frame_number __asm__("D_L05_0015F5CC"), D_L05_0015FFD8;
extern int           D_001417E0[];
extern int           D_0014EE90[];
extern unsigned char D_0014C050[];
extern f32           D_0013F3D8, D_0013F3DC;
extern void          help_request(int, int) __asm__("FUN_L00_00203908");
extern void          collect_moby_and_clear_path_keys(void *) __asm__("FUN_L00_002d5830");
typedef unsigned int L05Vector __attribute__((mode(TI), aligned(16)));

typedef struct {
    unsigned short count, best;
    int            flags;
} VendorEventRecord;

extern VendorEventRecord D_00141968[];
#define D_00141B8C (D_00141968[68].flags)
#define D_00141BAC (D_00141968[72].flags)
#define D_00141BF4 (D_00141968[81].flags)
#define D_00141CFC (D_00141968[114].flags)
#define D_00141A82 (D_00141968[35].best)
#define D_00141A88 (D_00141968[36].count)
#define D_00141A8A (D_00141968[36].best)
#define D_00141A8C (D_00141968[36].flags)
#define D_00141A90 (D_00141968[37].count)
#define D_00141A92 (D_00141968[37].best)
#define D_00141A94 (D_00141968[37].flags)
#define D_00141A98 (D_00141968[38].count)
#define D_00141A9C (D_00141968[38].flags)
#define D_00141A78 (D_00141968[34].count)
#define D_00141C58 (D_00141968[94].count)
#define D_00141C5A (D_00141968[94].best)
#define D_00141C5C (D_00141968[94].flags)
#define D_00141B88 (D_00141968[68].count)
#define D_00141BB8 (D_00141968[74].count)
typedef struct {
    unsigned char  pad_0[82];
    unsigned short D_00141772;
    unsigned char  pad_54[38];
    unsigned short D_0014179A;
    unsigned char  pad_7c[6];
    unsigned short D_001417A2;
    unsigned char  pad_84[6];
    unsigned short D_001417AA;
    unsigned char  pad_8c[22];
    unsigned short D_001417C2;
} L05Fields_00141720;

extern L05Fields_00141720 l05_00141720 __asm__("D_00141720");
#define D_001417A2 (l05_00141720.D_001417A2)
#define D_0014179A (l05_00141720.D_0014179A)
#define D_001417C2 (l05_00141720.D_001417C2)
#define D_00141772 (l05_00141720.D_00141772)
#define D_001417AA (l05_00141720.D_001417AA)
typedef struct {
    unsigned char D_0013D388;
    unsigned char pad_1[12];
    unsigned char menu_item_enabled_1;
} L05Fields_0013D388;

extern L05Fields_0013D388 alternate_item_available __asm__("D_0013D388");
#define D_0013D388          (alternate_item_available.D_0013D388)
#define menu_item_enabled_1 (alternate_item_available.menu_item_enabled_1)
typedef struct {
    unsigned char pad_0[15];
    unsigned char D_0013D4CF;
    unsigned char pad_10[6];
    unsigned char D_0013D4D6;
} L05Fields_0013D4C0;

extern L05Fields_0013D4C0 item_available __asm__("D_0013D4C0");
#define D_0013D4D6 (item_available.D_0013D4D6)
#define D_0013D4CF (item_available.D_0013D4CF)
#define D_0013F64C (*(int *)(D_0013F3D0 + 0x27c))
#define D_0013F65E (*(short *)(D_0013F3D0 + 0x28e))

void update_help_hints(struct Moby *moby) __asm__("FUN_L05_0031bf40");

void update_help_hints(struct Moby *moby) {
    int *data = ((int *)moby->pvars);
    int  value, elapsed, limit;
    moby->update_dist = 0xff;

    if (is_point_inside_clip_volume(D_0013F3D0, data[20])) {
        if (*(int *)(D_0013F3D0 + 0x200c) == 0x12 && data[26]++ > scale_game_frames(0x168)) {
            if ((unsigned int)(D_00141B8C & (1 << gameplay_event_level_index)) < 1)
                help_request(0x3f0, 0x44);
        } else if (D_001413DC == 0x11 || menu_item_enabled_1) {
            D_00141B88 = 0xffff;
        }
    }
    if (is_point_inside_clip_volume(D_0013F3D0, data[16])) {
        elapsed = scale_game_frames(gameplay_event_frame_count) - usage_stats.stat[9].unk2 * 600;
        limit = (int)((float)scale_game_frames(0xe10) * 60.0f);
        if (limit < elapsed && (unsigned int)(D_00141BAC & (1 << gameplay_event_level_index)) < 1)
            help_request(0x7dd, 0x48);
    }
    if (frame_number < scale_game_frames(60) && usage_stats.stat[12].count &&
        !usage_stats.stat[11].count && D_00141BF4 >= 0)
        help_request(0x4e27, 0x51);
    if (frame_number < scale_game_frames(60) && usage_stats.stat[29].count) {
        if (D_00141CFC >= 0)
            help_request(0x4e28, 0x72);
        usage_stats.stat[29].count = 0;
    }
    if (!D_0013D4D6) {
        if (is_point_inside_clip_volume(D_0013F3D0, data[9]) && !D_0013F64C) {
            elapsed = scale_game_frames(gameplay_event_frame_count) - D_00141A82 * 600;
            limit = (int)((float)scale_game_frames(18) * 60.0f);
            if (limit < elapsed || D_00141A82 * 600 == 0)
                help_request(0x138a, 0x23);
            else {
                value = scale_game_frames(gameplay_event_frame_count) / 600;
                if (D_00141A82 < value)
                    D_00141A82 = scale_game_frames(gameplay_event_frame_count) / 600;
            }
        }
    }
    if (D_0013D4D6) {
        if (is_point_inside_clip_volume(D_0013F3D0, data[9]) && !D_0013F64C) {
            if (D_00141A88 != 0) {
                elapsed = scale_game_frames(gameplay_event_frame_count) - D_00141A8A * 600;
                limit = (int)((float)scale_game_frames(18) * 60.0f);
                if (limit < elapsed || D_00141A8A * 600 == 0)
                    help_request(0x138b, 0x24);
                else {
                    value = scale_game_frames(gameplay_event_frame_count) / 600;
                    if (D_00141A8A < value)
                        D_00141A8A = scale_game_frames(gameplay_event_frame_count) / 600;
                }
            } else {
                D_00141A88++;
                value = scale_game_frames(gameplay_event_frame_count) / 600;
                if (D_00141A8A < value)
                    D_00141A8A = scale_game_frames(gameplay_event_frame_count) / 600;
                D_00141A8C = D_00141A8C | (1 << gameplay_event_level_index) | 0x80000000;
            }
        }
        if (D_001413DC == 0x13)
            D_00141A88 = -1;
    }
    if (D_00141A98 < 2) {
        if (D_0014C050[data[8] + gameplay_event_level_index * 16] == 0xff) {
            if (D_00141A90 < 0xffff)
                D_00141A90 = D_00141A90 + 1;
            value = scale_game_frames(gameplay_event_frame_count) / 600;
            if (D_00141A92 < value)
                D_00141A92 = scale_game_frames(gameplay_event_frame_count) / 600;
            D_00141A94 = D_00141A94 | (1 << gameplay_event_level_index) | 0x80000000;
        } else if (D_0014EE90[data[8]] > 2 && is_point_inside_clip_volume(D_0013F3D0, data[11])) {
            elapsed = scale_game_frames(gameplay_event_frame_count) - D_001417A2 * 600;
            if (scale_game_frames(18000) < elapsed) {
                elapsed = scale_game_frames(gameplay_event_frame_count) - D_0014179A * 600;
                if (scale_game_frames(18000) < elapsed) {
                    elapsed = scale_game_frames(gameplay_event_frame_count) - D_001417C2 * 600;
                    if (scale_game_frames(18000) < elapsed) {
                        elapsed = scale_game_frames(gameplay_event_frame_count) - D_00141772 * 600;
                        if (scale_game_frames(18000) < elapsed) {
                            elapsed =
                                scale_game_frames(gameplay_event_frame_count) - D_001417AA * 600;
                            if (scale_game_frames(18000) < elapsed && D_00141A9C >= 0)
                                help_request(0x138d, 0x26);
                        }
                    }
                }
            }
        }
    }
    if (D_0013D388 && D_00141A78 == 0 && D_001413DC == 0x16)
        help_request(0x1389, 0x22);
    if (D_00141C58 == 0) {
        if (hero.state.current == 10 && (is_point_inside_clip_volume(&hero.motion.pos, data[13]) ||
                                         is_point_inside_clip_volume(&hero.motion.pos, data[14]))) {
            if (D_00141C58 < 0xffff)
                D_00141C58 = D_00141C58 + 1;
            value = scale_game_frames(gameplay_event_frame_count) / 600;
            if (D_00141C5A < value)
                D_00141C5A = scale_game_frames(gameplay_event_frame_count) / 600;
            D_00141C5C = D_00141C5C | (1 << gameplay_event_level_index) | 0x80000000;
        }
        if (hero.state.control_mode == 0x12) {
            if (is_point_inside_clip_volume(data + 4, data[13]) ||
                is_point_inside_clip_volume(data + 4, data[14])) {
                clear_u64_value(data + 4);
                data[24]++;
            }
        } else if (data[24] > 1 && (is_point_inside_clip_volume(&hero.motion.pos, data[13]) ||
                                    is_point_inside_clip_volume(&hero.motion.pos, data[14])))
            help_request(0x138e, 0x5e);
    }
    if (!usage_stats.stat[18].count && !D_00141BB8 && D_0013D4CF &&
        is_point_inside_clip_volume(D_0013F3D0, data[18]) && D_0013D464 >= 20)
        help_request(20000, 0x4a);
    value = data[21];
    if (value != -1 && D_0014C050[data[22] + gameplay_event_level_index * 16] == 0xff &&
        D_0014C050[data[8] + gameplay_event_level_index * 16] == 0xff &&
        (frame_number < 5 || (D_0013F64C && (*(short *)(D_0013F64C + 0xa6) == 0x3e6 ||
                                             *(short *)(D_0013F64C + 0xa6) == 0x33e)))) {
        collect_moby_and_clear_path_keys((void *)(D_L05_0015FFD8 + value * 256));
        data[21] = -1;
    }
    qcopy(data, D_0013F3D0);
    if (!D_0013F65E)
        qcopy(data + 4, D_0013F3D0);
}
