#include "types.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"

typedef struct {
    s16 f00;
    u8  f02;
    u8  f03;
    s16 f04;
    u16 f06;
    s16 f08;
    s16 f0a;
    s16 f0c;
    u8  f0e;
    u8  f0f;
} L00Entry;

extern L00Entry actuator_wave[8] __asm__("D_00165480");
extern f32      fast_cos(f32) __asm__("func_001F9DC8");

s32 actuator_calc_power(s32 *output_power) __asm__("FUN_001e8d08");

s32 actuator_calc_power(s32 *output_power) {
    enum {
        ACTUATOR_WAVE_INACTIVE = 0,
        ACTUATOR_WAVE_RECTANGULAR = 1,
        ACTUATOR_WAVE_TRIANGULAR = 2,
        ACTUATOR_WAVE_RISING_RAMP = 3,
        ACTUATOR_WAVE_FALLING_RAMP = 4,
        ACTUATOR_WAVE_COSINE = 5
    };
    s32  aa[3][4];
    s32 *power_wave_counts;
    s32 *scale_wave_counts;
    s32  i;
    s32  count;

    count = 0;
    scale_wave_counts = aa[1];
    power_wave_counts = aa[2];
    for (i = 0; i < 2; i++) {
        output_power[i] = 0;
        aa[0][i] = 0;
        aa[1][i] = 0;
        aa[2][i] = 0;
    }
    for (i = 0; i < 8; i++) {
        L00Entry *wave_entry;
        s32       rem;
        s32       waveform_value;
        u16       wave_countdown;

        if (actuator_wave[i].f00 == 0) {
            continue;
        }
        wave_entry = &actuator_wave[i];
        count++;
        wave_countdown = actuator_wave[i].f06;
        actuator_wave[i].f06 = wave_countdown - 1;
        if ((s16)wave_countdown <= 0) {
            wave_entry->f00 = 0;
        }
        if (actuator_wave[i].f04 != 0) {
            actuator_wave[i].f04 = actuator_wave[i].f04 - 1;
            continue;
        }
        waveform_value = 0;
        actuator_wave[i].f08++;
        rem = wave_entry->f08 % (wave_entry->f0a + wave_entry->f0c);
        switch (wave_entry->f00) {
        case ACTUATOR_WAVE_INACTIVE:
            break;
        case ACTUATOR_WAVE_RECTANGULAR:
            if (rem < wave_entry->f0a) {
                waveform_value = wave_entry->f0e + wave_entry->f0f;
            } else {
                waveform_value = wave_entry->f0f;
            }
            break;
        case ACTUATOR_WAVE_TRIANGULAR:
            if (rem < wave_entry->f0a) {
                waveform_value = (wave_entry->f0e * rem) / wave_entry->f0a;
            } else {
                waveform_value =
                    (wave_entry->f0e * (wave_entry->f0c + wave_entry->f0a - rem)) / wave_entry->f0c;
            }
            waveform_value = waveform_value + wave_entry->f0f;
            break;
        case ACTUATOR_WAVE_RISING_RAMP:
            if (rem < wave_entry->f0a) {
                waveform_value = (wave_entry->f0e * rem) / wave_entry->f0a + wave_entry->f0f;
            }
            break;
        case ACTUATOR_WAVE_FALLING_RAMP:
            if (rem >= wave_entry->f0a) {
                waveform_value = (wave_entry->f0e * (wave_entry->f0c + wave_entry->f0a - rem)) /
                                     wave_entry->f0c +
                                 wave_entry->f0f;
            }
            break;
        case ACTUATOR_WAVE_COSINE: {
            f32 cosine_wave_intermediate;
            f32 wave_amplitude;
            if (rem < wave_entry->f0c) {
                cosine_wave_intermediate = convert_integer_to_float(rem) * 3.1415927f /
                                           convert_integer_to_float(wave_entry->f0c);
            } else {
                cosine_wave_intermediate =
                    3.1415927f - convert_integer_to_float(rem - wave_entry->f0c) * 3.1415927f /
                                     convert_integer_to_float(wave_entry->f0a);
            }
            cosine_wave_intermediate = fast_cos(cosine_wave_intermediate);
            wave_amplitude = convert_integer_to_float(wave_entry->f0e);
            waveform_value =
                truncate_float_to_s32((cosine_wave_intermediate * wave_amplitude + wave_amplitude) *
                                      0.5f) +
                wave_entry->f0f;
            if (waveform_value >= 256) {
                waveform_value = 255;
            }
            break;
        }
        }
        if (wave_entry->f03 != 0) {
            aa[0][wave_entry->f02] = aa[0][wave_entry->f02] + waveform_value;
            {
                s32 *t = scale_wave_counts + wave_entry->f02;
                *t = *t + 1;
            }
        } else {
            output_power[wave_entry->f02] = output_power[wave_entry->f02] + waveform_value;
            {
                s32 *t = power_wave_counts + wave_entry->f02;
                *t = *t + 1;
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (power_wave_counts[i] != 0) {
            output_power[i] = output_power[i] / power_wave_counts[i];
        }
        if (scale_wave_counts[i] != 0) {
            aa[0][i] = aa[0][i] / scale_wave_counts[i];
            output_power[i] = (output_power[i] * aa[0][i]) / 255;
        }
    }
    return count;
}

extern __typeof__(actuator_calc_power) func_001E8D08 __attribute__((alias("FUN_001e8d08")));
