#ifndef LOMBYTE_RNC_RENDERING_FS_AA_PACKETS_H
#define LOMBYTE_RNC_RENDERING_FS_AA_PACKETS_H

#include "types.h"
#include "sda.h"

/*
 * GIF packets the full-screen anti-aliasing passes send. setup_fs_aa_buffer
 * fills them as 64-bit words; the word counts are how far it writes:
 * a header of GIF tag and A+D registers, then 16 strips of vertex words.
 * Each one ends where the next object starts.
 */
extern u64 first_clear_packet[40] __asm__("D_0013CC90") NOT_SDA;   /* 8 header words + 32 */
extern u64 second_clear_packet[40] __asm__("D_0013CDD0") NOT_SDA;  /* 8 header words + 32 */
extern u64 fs_aa_draw_packet[76] __asm__("D_00151900") NOT_SDA;    /* 12 header words + 64 */
extern u64 fs_aa_transfer_packet[82] __asm__("D_00151B60") NOT_SDA; /* 12 + 64 strip words + 6 tail */
extern u64 fs_aa_resample_packet[74] __asm__("D_00151DF0") NOT_SDA; /* 10 header words + 64 */
extern u64 fs_aa_clear_packet[42] __asm__("D_00152040") NOT_SDA;   /* 10 header words + 32 */

#endif /* LOMBYTE_RNC_RENDERING_FS_AA_PACKETS_H */
