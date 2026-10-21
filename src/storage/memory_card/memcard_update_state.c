#include "types.h"
#include "rnc/globals.h"
#include "rnc/storage/disc_table.h"

typedef struct {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
    s32 x10;
} McChunk;

#include "rnc/storage/memory_card/memory_card_state.h"
extern char D_0013D1D0[];
extern char D_0013D1E8[];
extern char D_0013D200[];
extern char D_0013D220[];
extern char D_0013D240[];
extern char D_0013D270[];
extern u8 D_0014EED0[];
extern u8 D_001506D0[];
extern s16 cd_read_active[] __asm__("D_001516D8");
extern s32 pause_action_list_mode __asm__("D_0015EE90");
extern char D_0015FE98[];
extern char D_0015FEA0[];
extern u32 mc_gamedata[] __asm__("D_001A04C0");
extern u32 mc_leveldata[] __asm__("D_001A07C0");
extern u8 D_001A0880[];
extern char D_001A08A0[];
extern char D_001E81E8[];

extern void DebugPrint(char *fmt, ...);
extern s32 get_dma_packet_span_bytes(const u32 *packet) __asm__("GetDmaPacketSpanBytes");
extern void RaiseKernelTrap();
extern s32 SceMcFormat(s32 port, s32 slot);
extern s32 SceMcUnformat(s32 port, s32 slot);
extern void calculate_ring_buffer_bounds(s32, McChunk **, s32 *) __asm__("func_001FD6E0");
extern void memcard_restore_info(u8 *, s32, s32) __asm__("func_0020AE60");
extern s32 memcard_restore_data(u8 *, s32, u32 *) __asm__("func_0020AF20");
extern s32 start_audio_stream_read(McChunk *, s32, s32) __asm__("FUN_00216788");
extern s32 sceMcChdir(s32, s32, char *, s32);
extern s32 sceMcClose(s32 fd);
extern s32 sceMcDelete(s32 port, s32 slot, char *name);
extern s32 sceMcGetDir(s32, s32, char *, s32, s32, void *);
extern s32 sceMcGetInfo(s32, s32, s32 *, s32 *, s32 *);
extern s32 sceMcMkdir(s32 port, s32 slot, char *name);
extern s32 sceMcOpen(s32 port, s32 slot, char *name, s32 mode);
extern s32 sceMcRead(s32 fd, void *buf, s32 size);
extern s32 sceMcSeek(s32 fd, s32 offset, s32 origin);
extern s32 sceMcSync(s32 mode, s32 *cmd, s32 *result);
extern s32 sceMcWrite(s32 fd, void *buf, s32 size);
extern s32 sprintf(char *str, const char *format, ...);
extern char *strcpy(char *, const char *);

#define MC   memory_card_state
#define CARD MC.card[MC.cur]

void memcard_update_state(void) __asm__("FUN_002093d8");

void memcard_update_state(void) {
    char name[0x40];

    if (MC.busy) {
        MC.busy = sceMcSync(1, &MC.cmd, &MC.result) == 0;
        return;
    }
    MC.busy = 1;

    switch (MC.state) {
    case MEMCARD_QUERY_INFO:
        if (MC.cur < 0) {
            MC.cur = 0;
        }
        if (sceMcGetInfo(CARD.port, CARD.slot, &CARD.type, &CARD.free, &CARD.format) == 0) {
            MC.state = 1;
        }
        break;

    case MEMCARD_PROCESS_INFO_RESULT:
        if (MC.result != 0) {
            CARD.save_index = -3;
            CARD.sync_result = MC.result;
            CARD.errors = -1;
        }
        if (++MC.cur < 1) {
            MC.state = 0;
        } else {
            MC.state = 2;
        }
        MC.busy = 0;
        break;

    case MEMCARD_DISPATCH_PENDING_OPERATION:
        if (MC.pending_state >= 0) {
            if (MC.card[0].sync_result == 0) {
                if (MC.pending_card >= 0) {
                    MC.cur = MC.pending_card;
                    MC.state = MC.pending_state;
                    MC.err = 0;
                } else {
                    MC.err = 0x271A;
                }
            }
            MC.pending_state = -1;
            MC.pending_card = -1;
        } else {
            if (++MC.cur >= 31) {
                MC.cur = 0;
                MC.state = 0;
            }
        }
        MC.busy = 0;
        break;

    case MEMCARD_REQUEST_FORMAT:
        if (CARD.format == 0) {
            if (SceMcFormat(CARD.port, CARD.slot) == 0) {
                MC.state = 4;
            }
            break;
        }
        MC.busy = 0;
        MC.state = 2;
        break;

    case MEMCARD_PROCESS_FORMAT_RESULT:
        if (MC.result != 0) {
            MC.err = 1;
            MC.err_card = MC.cur;
        }
        MC.state = 0;
        MC.cur = 0;
        MC.busy = 0;
        break;

    case MEMCARD_REQUEST_UNFORMAT:
        if (CARD.format == 0) {
            if (SceMcUnformat(CARD.port, CARD.slot) == 0) {
                MC.state = 6;
            }
            break;
        }
        MC.busy = 0;
        MC.state = 2;
        break;

    case MEMCARD_PROCESS_UNFORMAT_RESULT:
        if (MC.result != 0) {
            MC.err = 2;
            MC.err_card = MC.cur;
        }
        MC.state = 0;
        MC.cur = 0;
        MC.busy = 0;
        break;

    case MEMCARD_REQUEST_CHDIR:
        if (sceMcChdir(CARD.port, CARD.slot, D_0013D1D0, 0) == 0) {
            MC.sub = 0;
            MC.state = 8;
        }
        break;

    case MEMCARD_PROBE_SAVE_HEADER:
        switch (MC.sub) {
        case MEMCARD_PROBE_PROCESS_CHDIR_RESULT:
            if (MC.result == 0) {
                s32 *save_index_ptr = &CARD.save_index;
                if (*save_index_ptr < 0) {
                    *save_index_ptr = -1;
                }
                MC.busy = 0;
                MC.sub = 1;
            } else {
                CARD.save_index = -2;
                if (MC.result == -2) {
                    MC.err = 3;
                    MC.err_card = MC.cur;
                } else if (MC.result != -4) {
                    MC.err = 4;
                    MC.err_card = MC.cur;
                }
                MC.state = 0;
                MC.cur = 0;
                MC.busy = 0;
            }
            break;
        case MEMCARD_PROBE_REQUEST_OPEN:
            sprintf(name, D_0013D270, 0);
            if (sceMcOpen(CARD.port, CARD.slot, name, 1) == 0) {
                MC.sub++;
            }
            break;
        case MEMCARD_PROBE_PROCESS_OPEN_RESULT:
            if (MC.result >= 0) {
                MC.fd = MC.result;
                MC.sub = 3;
            } else {
                CARD.save_index = -2;
                MC.err_card = MC.cur;
                if (MC.result == -7) {
                    MC.err = 0x2710;
                } else if (MC.result == -5) {
                    MC.err = 0x2711;
                } else if (MC.result == -4) {
                    MC.err = 0x2712;
                } else if (MC.result == -3) {
                    MC.err = 0x2713;
                } else if (MC.result == -2) {
                    MC.err = 3;
                } else {
                    MC.err = 4;
                }
                MC.state = 0;
                MC.cur = 0;
            }
            MC.busy = 0;
            break;
        case MEMCARD_PROBE_REQUEST_HEADER_READ:
            MC.size = 8;
            if (sceMcRead(MC.fd, &CARD.main_size, 8) == 0) {
                MC.sub = 4;
            }
            break;
        case MEMCARD_PROBE_PROCESS_HEADER_READ_RESULT:
            if (MC.result == MC.size) {
                CARD.errors = 0;
                if (CARD.main_size != get_dma_packet_span_bytes(mc_gamedata)) {
                    CARD.errors++;
                }
                if (CARD.record_size != get_dma_packet_span_bytes(mc_leveldata)) {
                    CARD.errors++;
                }
                MC.busy = 0;
                MC.sub = 5;
            } else {
                CARD.save_index = -2;
                MC.err_card = MC.cur;
                if (MC.result >= 0) {
                    MC.err = 0x2716;
                    if (sceMcClose(MC.fd) == 0) {
                        MC.state = 0;
                        MC.cur = 0;
                        MC.busy = 0;
                    }
                    break;
                }
                if (MC.result == -5) {
                    MC.err = 0x2711;
                } else if (MC.result == -4) {
                    MC.err = 0x2714;
                } else if (MC.result == -3) {
                    MC.err = 0x2715;
                } else if (MC.result == -2) {
                    MC.err = 3;
                } else {
                    MC.err = 4;
                }
                MC.state = 0;
                MC.cur = 0;
                MC.busy = 0;
            }
            break;
        case MEMCARD_PROBE_REQUEST_CLOSE:
            if (sceMcClose(MC.fd) == 0) {
                MC.busy = 0;
                MC.state = 21;
            }
            break;
        }
        break;

    case MEMCARD_BEGIN_CREATE_SAVE_DIRECTORY:
        MC.state = 10;
        MC.sub = 0;
        CARD.save_index = 0;
    case MEMCARD_CREATE_SAVE_DIRECTORY:
        switch (MC.sub) {
        case MEMCARD_CREATE_CHECK_FREE_SPACE:
            if (CARD.free >= 350) {
                MC.sub = 1;
            } else {
                MC.err = 7;
                MC.err_card = MC.cur;
                MC.state = 0;
                MC.cur = 0;
            }
            MC.busy = 0;
            break;
        case MEMCARD_CREATE_REQUEST_MKDIR:
            if (sceMcMkdir(CARD.port, CARD.slot, D_0013D1D0) == 0) {
                MC.sub = 2;
            }
            break;
        case MEMCARD_CREATE_PROCESS_MKDIR_RESULT:
            if (MC.result == 0 || MC.result == -4) {
                MC.sub = 3;
                break;
            }
            MC.err_card = MC.cur;
            if (MC.result == -3) {
                MC.err = 7;
            } else if (MC.result == -2) {
                MC.err = 6;
            } else {
                MC.err = 0xD;
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        case MEMCARD_CREATE_REQUEST_RESOURCE_READ: {
            McChunk *chunk;
            s32 size;

            calculate_ring_buffer_bounds(disc_table.memcard_data.size << 11, &chunk, &size);
            start_audio_stream_read(chunk, disc_table.memcard_data.sector, disc_table.memcard_data.size);
            MC.sub = 4;
            MC.busy = 0;
            break;
        }
        case MEMCARD_CREATE_WAIT_RESOURCE_READ:
            if (cd_read_active[0] == 0) {
                MC.sub = 5;
            }
            MC.busy = 0;
            break;
        case MEMCARD_CREATE_REQUEST_ICON_SYS_OPEN:
        case MEMCARD_CREATE_REQUEST_STATIC_ICON_OPEN:
        case MEMCARD_CREATE_REQUEST_SAVE_OPEN:
        case MEMCARD_CREATE_REQUEST_AUXILIARY_FILE_OPEN:
            switch (MC.sub) {
            case MEMCARD_CREATE_REQUEST_ICON_SYS_OPEN:
                strcpy(name, D_0013D200);
                break;
            case MEMCARD_CREATE_REQUEST_STATIC_ICON_OPEN:
                strcpy(name, D_0013D220);
                break;
            case MEMCARD_CREATE_REQUEST_AUXILIARY_FILE_OPEN:
                strcpy(name, D_0013D240);
                break;
            case MEMCARD_CREATE_REQUEST_SAVE_OPEN:
                sprintf(name, D_0013D270, CARD.save_index);
                break;
            }
            if (sceMcOpen(CARD.port, CARD.slot, name, 0x203) == 0) {
                MC.fd = -1;
                MC.sub++;
            }
            break;
        case MEMCARD_CREATE_PROCESS_ICON_SYS_OPEN_RESULT_AND_REQUEST_WRITE:
        case MEMCARD_CREATE_PROCESS_STATIC_ICON_OPEN_RESULT_AND_REQUEST_WRITE:
        case MEMCARD_CREATE_PROCESS_SAVE_OPEN_RESULT_AND_REQUEST_WRITE:
        case MEMCARD_CREATE_PROCESS_AUXILIARY_FILE_OPEN_RESULT_AND_REQUEST_WRITE: {
            McChunk *chunk;
            s32 size;

            if (MC.result >= 0) {
                if (MC.fd < 0) {
                    MC.fd = MC.result;
                }
                calculate_ring_buffer_bounds(disc_table.memcard_data.size << 11, &chunk, &size);
                switch (MC.sub) {
                case MEMCARD_CREATE_PROCESS_ICON_SYS_OPEN_RESULT_AND_REQUEST_WRITE:
                    MC.size = 0x3C4;
                    MC.buf = (u8 *)chunk + chunk->x0;
                    break;
                case MEMCARD_CREATE_PROCESS_STATIC_ICON_OPEN_RESULT_AND_REQUEST_WRITE:
                    MC.size = chunk->xC;
                    MC.buf = (u8 *)chunk + chunk->x8;
                    break;
                case MEMCARD_CREATE_PROCESS_AUXILIARY_FILE_OPEN_RESULT_AND_REQUEST_WRITE:
                    if (pause_action_list_mode != 0) {
                        MC.size = 0x3C00;
                    } else {
                        MC.size = 0x3C04;
                    }
                    MC.buf = &current_level_index;
                    break;
                case MEMCARD_CREATE_PROCESS_SAVE_OPEN_RESULT_AND_REQUEST_WRITE: {
                    s32 n = get_dma_packet_span_bytes(mc_gamedata);
                    MC.size = n + get_dma_packet_span_bytes(mc_leveldata) * 20 + 8;
                    MC.buf = (u8 *)chunk + chunk->x10;
                    break;
                }
                }
                if (sceMcWrite(MC.fd, MC.buf, MC.size) == 0) {
                    MC.sub++;
                }
            } else {
                MC.err = 0xA;
                MC.err_card = MC.cur;
                MC.state = 0;
                MC.cur = 0;
                MC.busy = 0;
            }
            break;
        }
        case MEMCARD_CREATE_PROCESS_ICON_SYS_WRITE_RESULT_AND_REQUEST_CLOSE:
        case MEMCARD_CREATE_PROCESS_STATIC_ICON_WRITE_RESULT_AND_REQUEST_CLOSE:
        case MEMCARD_CREATE_PROCESS_SAVE_WRITE_RESULT_AND_REQUEST_CLOSE:
        case MEMCARD_CREATE_PROCESS_AUXILIARY_FILE_WRITE_RESULT_AND_REQUEST_CLOSE:
            if (MC.result == MC.size) {
                if (sceMcClose(MC.fd) == 0) {
                    MC.sub++;
                }
                break;
            }
            MC.err_card = MC.cur;
            if (MC.result >= 0) {
                MC.err = 0xB;
                if (sceMcClose(MC.fd) == 0) {
                    MC.state = 0;
                    MC.cur = 0;
                    MC.busy = 0;
                }
                break;
            }
            if (MC.result == -4) {
                MC.err = 8;
            } else if (MC.result == -3) {
                MC.err = 7;
            } else if (MC.result == -2) {
                MC.err = 6;
            } else {
                MC.err = 0xD;
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        case MEMCARD_CREATE_PROCESS_ICON_SYS_CLOSE_RESULT:
        case MEMCARD_CREATE_PROCESS_STATIC_ICON_CLOSE_RESULT:
        case MEMCARD_CREATE_PROCESS_SAVE_CLOSE_RESULT:
            if (MC.result == 0) {
                MC.sub++;
            } else {
                MC.err = 0xC;
                MC.err_card = MC.cur;
                MC.state = 0;
                MC.cur = 0;
            }
            MC.busy = 0;
            break;
        case MEMCARD_CREATE_ADVANCE_SAVE_INDEX:
            if (++CARD.save_index < 5) {
                MC.sub = 13;
            } else {
                MC.sub++;
            }
            MC.busy = 0;
            break;
        case MEMCARD_CREATE_RESET_CARD_STATUS:
            MC.card[0].save_index = -1;
            MC.state = 0;
            MC.cur = 0;
            MC.card[0].errors = 0;
            break;
        }
        break;

    case MEMCARD_BEGIN_DELETE_SAVE_DIRECTORY:
        MC.state = 12;
        MC.sub = 0;
        CARD.save_index = -2;
    case MEMCARD_DELETE_SAVE_DIRECTORY:
        switch (MC.sub) {
        case MEMCARD_DELETE_REQUEST_FIRST_DIRECTORY_ENTRY:
            if (sceMcGetDir(CARD.port, CARD.slot, D_0013D1E8, 0, 1, D_001A0880) == 0) {
                MC.sub = 1;
            }
            break;
        case MEMCARD_DELETE_PROCESS_ENTRY_AND_REQUEST_DELETE:
            if (MC.result == 1) {
                sprintf(name, D_0015FE98, D_0013D1D0, D_001A08A0);
                DebugPrint(D_0015FEA0, name);
                if (sceMcDelete(CARD.port, CARD.slot, name) == 0) {
                    MC.sub = 2;
                }
            } else {
                if (MC.result == 0) {
                    MC.sub = 3;
                } else {
                    MC.err_card = MC.cur;
                    if (MC.result == -5) {
                        MC.err = 0x10;
                    } else if (MC.result == -4) {
                        MC.err = 0xF;
                    } else if (MC.result == -2) {
                        MC.err = 0xE;
                    } else {
                        MC.err = 0x12;
                    }
                    MC.state = 0;
                    MC.cur = 0;
                }
                MC.busy = 0;
            }
            break;
        case MEMCARD_DELETE_REQUEST_NEXT_DIRECTORY_ENTRY:
            if (sceMcGetDir(CARD.port, CARD.slot, D_0013D1E8, 1, 1, D_001A0880) == 0) {
                MC.sub = 1;
            }
            break;
        case MEMCARD_DELETE_REQUEST_DIRECTORY_DELETE:
            if (sceMcDelete(CARD.port, CARD.slot, D_0013D1D0) == 0) {
                DebugPrint(D_001E81E8, D_0013D1D0);
                MC.sub = 4;
            }
            break;
        case MEMCARD_DELETE_PROCESS_DIRECTORY_DELETE_RESULT:
            if (MC.result != 0) {
                MC.err_card = MC.cur;
                if (MC.result == -6) {
                    MC.err = 0x11;
                } else if (MC.result == -5) {
                    MC.err = 0x10;
                } else if (MC.result == -4) {
                    MC.err = 0xF;
                } else if (MC.result == -2) {
                    MC.err = 0xE;
                } else {
                    MC.err = 0x12;
                }
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        }
        break;

    case MEMCARD_BEGIN_SCAN_SAVE_INFO:
        MC.state = 22;
        CARD.scan_save_index = -1;
        MC.record_index = 0;
        break;

    case MEMCARD_ADVANCE_SAVE_INFO_SCAN:
        CARD.scan_save_index++;
        if (CARD.scan_save_index < 5) {
            MC.sub = 0;
            MC.state = 23;
        } else {
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
        }
        break;
    case MEMCARD_BEGIN_READ_SAVE:
        if (CARD.save_index < 0) {
            MC.busy = 0;
            MC.state = 0;
            MC.cur = 0;
            MC.err = 0x13;
            break;
        }
        MC.record_index = 0;
        MC.sub = 0;
        MC.state = 14;
    case MEMCARD_READ_SAVE:
    case MEMCARD_READ_SCANNED_SAVE_INFO:
        switch (MC.sub) {
        case MEMCARD_READ_SAVE_REQUEST_OPEN:
            if (MC.state == 23) {
                sprintf(name, D_0013D270, CARD.scan_save_index);
            } else {
                sprintf(name, D_0013D270, CARD.save_index);
            }
            if (sceMcOpen(CARD.port, CARD.slot, name, 1) == 0) {
                MC.sub++;
            }
            break;
        case MEMCARD_READ_SAVE_PROCESS_OPEN_RESULT:
            if (MC.result >= 0) {
                MC.fd = MC.result;
                MC.sub = 2;
            } else {
                MC.err_card = MC.cur;
                if (MC.result == -7) {
                    MC.err = 0x14;
                } else if (MC.result == -5) {
                    MC.err = 0x15;
                } else if (MC.result == -4) {
                    MC.err = 0x16;
                } else if (MC.result == -3) {
                    MC.err = 0x17;
                } else if (MC.result == -2) {
                    MC.err = 0x18;
                } else {
                    MC.err = 0x1C;
                }
                MC.state = 0;
                MC.cur = 0;
            }
            MC.busy = 0;
            break;
        case MEMCARD_READ_SAVE_REQUEST_HEADER_READ:
            MC.size = 8;
            if (sceMcRead(MC.fd, &CARD.main_size, 8) == 0) {
                MC.sub = 3;
            }
            break;
        case MEMCARD_READ_SAVE_PROCESS_HEADER_READ_RESULT:
            if (MC.result == MC.size) {
                MC.busy = 0;
                MC.sub = 4;
                break;
            }
            MC.err_card = MC.cur;
            if (MC.result >= 0) {
                MC.err = 0x1B;
                if (sceMcClose(MC.fd) == 0) {
                    MC.state = 0;
                    MC.cur = 0;
                    MC.busy = 0;
                }
                break;
            }
            if (MC.result == -5) {
                MC.err = 0x15;
            } else if (MC.result == -4) {
                MC.err = 0x19;
            } else if (MC.result == -3) {
                MC.err = 0x1A;
            } else if (MC.result == -2) {
                MC.err = 0x18;
            } else {
                MC.err = 0x1C;
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        case MEMCARD_READ_SAVE_REQUEST_MAIN_DATA_READ:
            MC.size = CARD.main_size;
            if (MC.size > 0x1800) {
                RaiseKernelTrap();
            }
            if (sceMcRead(MC.fd, D_0014EED0, MC.size) == 0) {
                MC.sub = 5;
            }
            break;
        case MEMCARD_READ_SAVE_PROCESS_MAIN_DATA_READ_RESULT:
            if (MC.result == MC.size) {
                if (MC.state == 23) {
                    memcard_restore_info(D_0014EED0, MC.cur, CARD.scan_save_index);
                    MC.sub = 8;
                } else {
                    CARD.errors = memcard_restore_data(D_0014EED0, 0, mc_gamedata);
                    MC.sub = 6;
                }
                MC.busy = 0;
                break;
            }
            MC.err_card = MC.cur;
            if (MC.result >= 0) {
                MC.err = 0x1B;
                if (sceMcClose(MC.fd) == 0) {
                    MC.state = 0;
                    MC.cur = 0;
                    MC.busy = 0;
                }
                break;
            }
            if (MC.result == -5) {
                MC.err = 0x15;
            } else if (MC.result == -4) {
                MC.err = 0x19;
            } else if (MC.result == -3) {
                MC.err = 0x1A;
            } else if (MC.result == -2) {
                MC.err = 0x18;
            } else {
                MC.err = 0x1C;
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        case MEMCARD_READ_SAVE_REQUEST_RECORD_READ:
            MC.size = CARD.record_size;
            if (MC.size > 0x1000) {
                RaiseKernelTrap();
            }
            if (sceMcRead(MC.fd, D_001506D0, MC.size) == 0) {
                MC.sub = 7;
            }
            break;
        case MEMCARD_READ_SAVE_PROCESS_RECORD_READ_RESULT:
            if (MC.result == MC.size) {
                CARD.errors += memcard_restore_data(D_001506D0, MC.record_index, mc_leveldata);
                if (++MC.record_index < 20) {
                    MC.sub = 6;
                } else {
                    MC.sub = 8;
                }
                MC.busy = 0;
                break;
            }
            MC.err_card = MC.cur;
            if (MC.result >= 0) {
                MC.err = 0x1B;
                if (sceMcClose(MC.fd) == 0) {
                    MC.state = 0;
                    MC.cur = 0;
                    MC.busy = 0;
                }
                break;
            }
            if (MC.result == -5) {
                MC.err = 0x15;
            } else if (MC.result == -4) {
                MC.err = 0x19;
            } else if (MC.result == -3) {
                MC.err = 0x1A;
            } else if (MC.result == -2) {
                MC.err = 0x18;
            } else {
                MC.err = 0x1C;
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        case MEMCARD_READ_SAVE_REQUEST_CLOSE:
            if (sceMcClose(MC.fd) == 0) {
                if (MC.state == 23) {
                    MC.busy = 0;
                    MC.state = 22;
                } else {
                    MC.state = 0;
                    MC.cur = 0;
                }
            }
            break;
        }
        break;

    case MEMCARD_BEGIN_UPDATE_SAVE:
        if (CARD.errors != 0) {
            MC.busy = 0;
            MC.state = 0;
            MC.cur = 0;
            MC.err = 0x2717;
            break;
        }
        if (CARD.save_index < 0) {
            MC.busy = 0;
            MC.state = 0;
            MC.cur = 0;
            MC.err = 0x1D;
            break;
        }
        MC.sub = 0;
        MC.state = 16;
    case MEMCARD_UPDATE_SAVE:
        switch (MC.sub) {
        case MEMCARD_UPDATE_SAVE_REQUEST_OPEN:
            sprintf(name, D_0013D270, CARD.save_index);
            if (sceMcOpen(CARD.port, CARD.slot, name, 2) == 0) {
                MC.sub = 1;
            }
            break;
        case MEMCARD_UPDATE_SAVE_PROCESS_OPEN_RESULT:
            if (MC.result >= 0) {
                MC.fd = MC.result;
                MC.sub = 2;
            } else {
                MC.err_card = MC.cur;
                if (MC.result == -7) {
                    MC.err = 0x1E;
                } else if (MC.result == -5) {
                    MC.err = 0x1F;
                } else if (MC.result == -4) {
                    MC.err = 0x20;
                } else if (MC.result == -3) {
                    MC.err = 0x21;
                } else if (MC.result == -2) {
                    MC.err = 0x22;
                } else {
                    MC.err = 0x26;
                }
                MC.state = 0;
                MC.cur = 0;
            }
            MC.busy = 0;
            break;
        case MEMCARD_UPDATE_SAVE_REQUEST_HEADER_SKIP:
            if (sceMcSeek(MC.fd, 8, 0) == 0) {
                MC.sub = 3;
            }
            break;
        case MEMCARD_UPDATE_SAVE_PROCESS_HEADER_SKIP_RESULT:
            if (MC.result >= 0) {
                MC.sub = 4;
            } else {
                MC.state = 0;
                MC.cur = 0;
                MC.busy = 0;
            }
            break;
        case MEMCARD_UPDATE_SAVE_REQUEST_MAIN_DATA_WRITE:
            if (sceMcWrite(MC.fd, D_0014EED0, get_dma_packet_span_bytes(mc_gamedata)) == 0) {
                MC.sub = 5;
            }
            break;
        case MEMCARD_UPDATE_SAVE_PROCESS_MAIN_DATA_WRITE_RESULT:
            if (MC.result == get_dma_packet_span_bytes(mc_gamedata)) {
                MC.sub = 6;
                break;
            }
            MC.err_card = MC.cur;
            if (MC.result >= 0) {
                MC.err = 0x25;
                if (sceMcClose(MC.fd) == 0) {
                    MC.state = 0;
                    MC.cur = 0;
                    MC.busy = 0;
                }
                break;
            }
            if (MC.result == -5) {
                MC.err = 0x1F;
            } else if (MC.result == -4) {
                MC.err = 0x23;
            } else if (MC.result == -3) {
                MC.err = 0x24;
            } else if (MC.result == -2) {
                MC.err = 0x22;
            } else {
                MC.err = 0x26;
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        case MEMCARD_UPDATE_SAVE_REQUEST_RECORD_SEEK:
            if (MC.record_index == 0) {
                MC.sub = 8;
                break;
            }
            if (sceMcSeek(MC.fd, MC.record_index * get_dma_packet_span_bytes(mc_leveldata), 1) == 0) {
                MC.sub = 7;
            }
            break;
        case MEMCARD_UPDATE_SAVE_PROCESS_RECORD_SEEK_RESULT:
            if (MC.result >= 0) {
                MC.sub = 8;
            } else {
                MC.state = 0;
                MC.cur = 0;
                MC.busy = 0;
            }
            break;
        case MEMCARD_UPDATE_SAVE_REQUEST_RECORD_WRITE:
            if (sceMcWrite(MC.fd, D_001506D0, get_dma_packet_span_bytes(mc_leveldata)) == 0) {
                MC.sub = 9;
            }
            break;
        case MEMCARD_UPDATE_SAVE_PROCESS_RECORD_WRITE_RESULT:
            if (MC.result == get_dma_packet_span_bytes(mc_leveldata)) {
                MC.sub = 10;
                break;
            }
            MC.err_card = MC.cur;
            if (MC.result >= 0) {
                MC.err = 0x25;
                if (sceMcClose(MC.fd) == 0) {
                    MC.state = 0;
                    MC.cur = 0;
                    MC.busy = 0;
                }
                break;
            }
            if (MC.result == -5) {
                MC.err = 0x1F;
            } else if (MC.result == -4) {
                MC.err = 0x23;
            } else if (MC.result == -3) {
                MC.err = 0x24;
            } else if (MC.result == -2) {
                MC.err = 0x18;
            } else {
                MC.err = 0x1C;
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        case MEMCARD_UPDATE_SAVE_REQUEST_CLOSE:
            if (sceMcClose(MC.fd) == 0) {
                MC.state = 0;
                MC.cur = 0;
            }
            break;
        }
        break;

    case MEMCARD_REQUEST_SAVE_RESOURCE_READ: {
        McChunk *chunk;
        s32 size;

        calculate_ring_buffer_bounds(disc_table.memcard_data.size << 11, &chunk, &size);
        start_audio_stream_read(chunk, disc_table.memcard_data.sector, disc_table.memcard_data.size);
        MC.sub = 18;
        MC.busy = 0;
        break;
    }

    case MEMCARD_WAIT_SAVE_RESOURCE_READ_AND_SELECT_BUFFER:
        if (cd_read_active[0] == 0) {
            McChunk *chunk;
            s32 size;

            calculate_ring_buffer_bounds(disc_table.memcard_data.size << 11, &chunk, &size);
            MC.sub = 19;
            MC.buf = (u8 *)chunk + chunk->x10;
        }
        MC.busy = 0;
        break;

    case MEMCARD_BEGIN_WRITE_FULL_SAVE:
        if (CARD.errors != 0) {
            MC.busy = 0;
            MC.state = 0;
            MC.cur = 0;
            MC.err = 0x2718;
            break;
        }
        if (CARD.save_index < 0) {
            MC.busy = 0;
            MC.state = 0;
            MC.cur = 0;
            MC.err = 0x27;
            break;
        }
        MC.record_index = 0;
        MC.sub = 0;
        MC.state = 20;
    case MEMCARD_WRITE_FULL_SAVE:
        switch (MC.sub) {
        case MEMCARD_WRITE_FULL_SAVE_REQUEST_OPEN:
            sprintf(name, D_0013D270, CARD.save_index);
            if (sceMcOpen(CARD.port, CARD.slot, name, 2) == 0) {
                MC.sub = 1;
            }
            break;
        case MEMCARD_WRITE_FULL_SAVE_PROCESS_OPEN_RESULT_AND_REQUEST_WRITE:
            if (MC.result >= 0) {
                s32 n;

                MC.fd = MC.result;
                n = get_dma_packet_span_bytes(mc_gamedata);
                MC.size = n + get_dma_packet_span_bytes(mc_leveldata) * 20 + 8;
                if (sceMcWrite(MC.fd, MC.buf, MC.size) == 0) {
                    MC.sub = 2;
                }
                break;
            }
            MC.err = 0x2B;
            MC.err_card = MC.cur;
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        case MEMCARD_WRITE_FULL_SAVE_PROCESS_WRITE_RESULT_AND_REQUEST_CLOSE:
            if (MC.result == MC.size) {
                if (sceMcClose(MC.fd) == 0) {
                    MC.sub = 3;
                }
                break;
            }
            MC.err_card = MC.cur;
            if (MC.result >= 0) {
                MC.err = 0xB;
                if (sceMcClose(MC.fd) == 0) {
                    MC.state = 0;
                    MC.cur = 0;
                    MC.busy = 0;
                }
                break;
            }
            if (MC.result == -4) {
                MC.err = 0x28;
            } else if (MC.result == -3) {
                MC.err = 0x29;
            } else if (MC.result == -2) {
                MC.err = 0x2A;
            } else {
                MC.err = 0x2D;
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        case MEMCARD_WRITE_FULL_SAVE_PROCESS_CLOSE_RESULT:
            if (MC.result != 0) {
                MC.err = 0x2C;
                MC.err_card = MC.cur;
            }
            MC.state = 0;
            MC.cur = 0;
            MC.busy = 0;
            break;
        }
        break;
    }
}

extern __typeof__(memcard_update_state) func_002093D8 __attribute__((alias("FUN_002093d8")));
