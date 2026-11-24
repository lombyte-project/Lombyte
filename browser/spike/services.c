/* Browser equivalents of PS2 kernel services reached by the spike build. Each entry states why
   its body is a complete equivalent here, not a placeholder. Anything without a real equivalent
   stays a named trap in the runner instead of being added to this file. */
#include "types.h"

/* FlushCache (0x118A80, EE kernel syscall): invalidates the CPU's instruction and data caches
   after code or data is written to RAM. WebAssembly linear memory has no cache to go stale and
   the module does not execute code loaded into memory, so there is nothing to invalidate. */
void FlushCache(s32 operation);
/* FlushCache(2) invalidates the instruction cache: what run_game_main_loop does right after
   parse_bin copies a new program into RAM. The host is told (spike_program_loaded), and when the
   boot program has just loaded a level program it switches to that program's compiled module. */
extern void spike_program_loaded(void) __attribute__((import_module("env"), import_name("spike_program_loaded")));
void FlushCache(s32 operation) { if (operation == 2) spike_program_loaded(); }
void FlushCache_by_address(s32 operation) __asm__("FUN_00118a80");
void FlushCache_by_address(s32 operation) { (void)operation; }

/* vu0_load_micro_program (0x2334D8, src/rendering/vu1/vu0_load_micro_program.c): writes the VIF0
   DMA channel registers at 0x10008000 to upload a microprogram into VU0 and spins until the
   transfer ends. There is no VU0 here: the routines that microprogram provides are reached with
   `vcallms` from assembly-only helpers, and each of those helpers is supplied as portable C
   (browser/level-math.c, browser/spike/ee-helpers.c). So the upload has no consumer; this records
   the request instead of touching hardware registers. Linked ahead of the recovered object, so
   this definition is the one used. */
static s32 vu0_micro_program_requests;
static s32 vu0_micro_program_last_tag;
void vu0_load_micro_program(s32 dma_tag_address) __asm__("FUN_002334d8");
void vu0_load_micro_program(s32 dma_tag_address) {
    vu0_micro_program_requests += 1;
    vu0_micro_program_last_tag = dma_tag_address;
}
s32 spike_vu0_micro_program_requests(void) { return vu0_micro_program_requests; }
s32 spike_vu0_micro_program_last_tag(void) { return vu0_micro_program_last_tag; }

/* Work that is NOT implemented. Each of these replaces a recovered function whose only effect is
   on a subsystem the browser build does not have yet. They do nothing except report themselves:
   the runner counts every call and lists it under NOT DONE, so none of them is silent. Remove
   an entry when its subsystem exists; do not add game logic here. */
__attribute__((import_module("env"), import_name("spike_deferred")))
extern void spike_deferred(const char *name, const char *reason);

/* src/audio: loads the HUD sound banks through the IOP sound driver. No browser audio yet. */
void load_hud_banks(void) __asm__("FUN_00202a98");
/* load_hud_banks (0x202a98) is not replaced: it loads the HUD and menu graphics banks (icons,
   frames, palettes), and its recovered C runs. */

/* ---- Graphics Synthesizer image upload -> browser texture store -------------------------------
   On the console, sceGsSetDefLoadImage builds a GIF packet (BITBLTBUF/TRXPOS/TRXREG/TRXDIR) and
   sceGsExecLoadImage sends it with the pixels over DMA into GS video memory at block `dbp`;
   sceGsSyncPath waits for the transfer. The browser has no GS video memory. The equivalent here
   is a table of uploads keyed by that same VRAM block address, which a renderer resolves when a
   draw references the address. The upload is complete when it is recorded, so SyncPath has
   nothing to wait for. Pixel data is referenced, not copied: it stays where the loader put it. */
typedef struct {
    s32 dbp;    /* destination block pointer: VRAM address / 256 */
    s32 dbw;    /* destination width in 64-pixel blocks */
    s32 psm;    /* 0x13 PSMT8, 2 PSMCT16, 0 PSMCT32 */
    s32 x, y, w, h;
    u32 source; /* address of the pixels in linear memory */
} GsUpload;

/* The browser GS backend (gs-renderer.mjs, on the host) receives each upload and each frame's
   VIF1 / GIF DMA chain as the console's GS would: spike_gs_load_image (destination, format,
   rectangle, pixels) and spike_dma_chain (channel, chain address). */
extern void spike_gs_load_image(s32 dbp, s32 dbw, s32 psm, s32 x, s32 y, s32 w, s32 h, u32 source)
    __attribute__((import_module("env"), import_name("spike_gs_load_image")));
extern void spike_dma_chain(u32 channel, u32 chain) __attribute__((import_module("env"), import_name("spike_dma_chain")));
enum { GS_UPLOAD_CAPACITY = 4096 };
static GsUpload gs_uploads[GS_UPLOAD_CAPACITY];
static s32 gs_upload_count;
static s32 gs_upload_overflow;

s32 sceGsSetDefLoadImage(void *image, s32 dbp, s32 dbw, s32 psm, s32 x, s32 y, s32 w, s32 h);
s32 sceGsSetDefLoadImage(void *image, s32 dbp, s32 dbw, s32 psm, s32 x, s32 y, s32 w, s32 h) {
    /* The game treats sceGsLoadImage (96 bytes) as opaque between these two calls. */
    GsUpload pending = { dbp & 0xFFFF, dbw & 0xFFFF, psm & 0xFFFF, x & 0xFFFF, y & 0xFFFF, w & 0xFFFF, h & 0xFFFF, 0 };
    __builtin_memcpy(image, &pending, sizeof(pending));
    return 0;
}

s32 sceGsExecLoadImage(void *image, u32 source);
s32 sceGsExecLoadImage(void *image, u32 source) {
    GsUpload pending;
    __builtin_memcpy(&pending, image, sizeof(pending));
    pending.source = source;
    if (gs_upload_count >= GS_UPLOAD_CAPACITY) {
        gs_upload_overflow += 1; /* reported by the runner; never dropped silently */
        return -1;
    }
    gs_uploads[gs_upload_count++] = pending;
    spike_gs_load_image(pending.dbp, pending.dbw, pending.psm, pending.x, pending.y, pending.w, pending.h, source);
    return 0;
}

s32 sceGsSyncPath(s32 mode, s32 timeout) __asm__("FUN_00120558");
s32 sceGsSyncPath(s32 mode, s32 timeout) {
    (void)mode;
    (void)timeout;
    return 0;
}

s32 spike_gs_upload_count(void) { return gs_upload_count; }
s32 spike_gs_upload_overflow(void) { return gs_upload_overflow; }
u32 spike_gs_upload_address(s32 index) {
    return index >= 0 && index < gs_upload_count ? (u32)&gs_uploads[index] : 0;
}

/* The same two GS calls under their address labels (config/us/symbol_addrs.txt), which some
   overlay files use instead of the SDK names. */
s32 sceGsSetDefLoadImage_by_address(void *image, s32 dbp, s32 dbw, s32 psm, s32 x, s32 y, s32 w, s32 h) __asm__("FUN_00122330");
s32 sceGsSetDefLoadImage_by_address(void *image, s32 dbp, s32 dbw, s32 psm, s32 x, s32 y, s32 w, s32 h) {
    return sceGsSetDefLoadImage(image, dbp, dbw, psm, x, y, w, h);
}
s32 sceGsExecLoadImage_by_address(void *image, u32 source) __asm__("FUN_00122658");
s32 sceGsExecLoadImage_by_address(void *image, u32 source) { return sceGsExecLoadImage(image, source); }

/* sceCdSync (0x120C30): waits for the pending disc command. Disc reads here are served
   synchronously by the host (the scenario's read services), so no command is ever pending;
   0 is the SDK's "completed" result. */
s32 sceCdSync(s32 mode);
s32 sceCdSync(s32 mode) { (void)mode; return 0; }
s32 sceCdSync_by_address(s32 mode) __asm__("FUN_00120c30");
s32 sceCdSync_by_address(s32 mode) { (void)mode; return 0; }

/* sceCdRead (libcdvd): asks the IOP's CD/DVD driver to read `sectors` sectors starting at `lsn`
   into `buffer`, and returns 1 when the command was accepted. The browser's disc is the
   user-selected image; the host reads those bytes (host_disc_read, supplied by the page or the
   Node runner). The read is finished when this returns, which is why sceCdSync above has nothing
   to wait for and sceCdGetError reports no error. */
__attribute__((import_module("env"), import_name("host_disc_read")))
extern s32 host_disc_read(u32 lsn, u32 sectors, void *buffer);
s32 sceCdRead(u32 lsn, u32 sectors, void *buffer, void *mode);
static s32 cd_read_completed; /* a read finished whose callback has not run yet */
s32 sceCdRead(u32 lsn, u32 sectors, void *buffer, void *mode) {
    s32 accepted;
    (void)mode;
    accepted = host_disc_read(lsn, sectors, buffer);
    if (accepted) cd_read_completed = 1;
    return accepted;
}
s32 sceCdGetError(void);
s32 sceCdGetError(void) { return 0; }

/* src/rendering/effects/fade_to_black.c: draws a full-screen fade over `frames` vertical syncs.
   No GS draw path and no frame pacing exist yet. */
void fade_to_black(s32 frames) __asm__("FUN_001f4a58");
void fade_to_black(s32 frames) {
    (void)frames;
    spike_deferred("fade_to_black", "renderer: full-screen fade over N vsyncs (no GS draw path or frame pacing)");
}

/* sceGsSyncV (0x122298): blocks until the next vertical sync. This is the frame boundary: it
   calls the host's spike_vsync_yield, which (in a frames step of run-closure.mjs, after
   link-closure.sh's Asyncify pass) unwinds to the host and resumes here on the next frame, so
   run_level_main_loop runs unchanged one frame per host call. Outside a frames step the host
   returns at once. Counted so a run shows how many frame boundaries the code asked for. */
static s32 vsync_waits;
extern void spike_vsync_yield(void);
s32 sceGsSyncV(s32 mode);
static void cd_run_completion(void);
/* Interrupt handlers the game installs (AddIntcHandler, below). Only cause 2 (INTC_VBLANK_S,
   the vertical blank start) has a source here: it runs once per host frame, from sceGsSyncV,
   before the game's code continues. The handler the game installs there is count_vsync
   (init_once, through sceGsSyncVCallback), defined as int (void). */
static int (*vblank_handler)(void);
static s32 vblank_runs;
s32 spike_vblank_runs(void) { return vblank_runs; }
/* After the boot program hands the machine to a level program (boot-runtime.mjs handOver,
   run-closure.mjs --handover), this module's own state starts empty, while on the console what
   the boot program registered keeps working across the jump. Two such registrations matter:
   - the vsync interrupt handler, count_vsync (init_once, through sceGsSyncVCallback);
   - the disc completion callback, finish_audio_stream_read (register_audio_stream_callback,
     through snd_stream_safe_cd_callback -> sceCdCallback): without it a stream read the level
     waits on (initialize_level_map_archive) never completes.
   The host reads from the boot module whether each was installed (spike_vblank_runs,
   spike_cd_callback_installed) and calls this in the level module with those flags.
   ASSUMED: these are the functions the boot program had installed (the only ones this game
   installs there outside the movie player). */
extern int count_vsync_handler(void) __asm__("FUN_0012f1c8");
extern void finish_audio_stream_read_callback(s32) __asm__("FUN_00216950");
static void (*cd_callback)(s32);
s32 spike_cd_callback_installed(void) { return cd_callback != 0; }
s32 spike_resume_after_handover(s32 vsync, s32 cd) {
    if (vsync) vblank_handler = count_vsync_handler;
    if (cd) cd_callback = finish_audio_stream_read_callback;
    return 1;
}
s32 sceGsSyncV(s32 mode) {
    (void)mode;
    vsync_waits += 1;
    spike_vsync_yield();
    if (vblank_handler) { vblank_handler(); vblank_runs += 1; }
    cd_run_completion();
    return 0;
}
s32 sceGsSyncV_by_address(s32 mode) __asm__("FUN_00122298");
s32 sceGsSyncV_by_address(s32 mode) { return sceGsSyncV(mode); }
s32 spike_vsync_waits(void) { return vsync_waits; }
static s32 vsync_count(void) { return vsync_waits; }

/* ---- Audio: NOT implemented -------------------------------------------------------------------
   src/audio talks to the 989snd driver on the IOP over SIF RPC. None of that exists here. These
   three are the calls level start-up makes. */

/* Asks the IOP to load a sound bank from EE memory and later calls `callback` with the bank
   handle, which start-up stores through `user_data` and spins on until it is not 0xFFFFFFFF.
   With no sound driver there is no handle: 0 is stored so the spin ends. Any code that later
   uses the handle reaches other unimplemented audio calls and traps there by name. */
void snd_bank_load_from_ee_cb(s32 bank, s32 callback, s64 user_data) __asm__("FUN_0012e088");
void snd_bank_load_from_ee_cb(s32 bank, s32 callback, s64 user_data) {
    (void)bank;
    (void)callback;
    *(u32 *)(u32)user_data = 0;
    spike_deferred("snd_bank_load_from_ee_cb", "audio: load a sound bank on the IOP (no handle; 0 stored)");
}
/* snd_flush_sound_commands (0x12dc80) is the recovered C (src/audio/rpc/snd_flush_sound_commands.c);
   the IOP side it talks to is the sound server below. */
void snd_resolve_bank_xrefs(void) __asm__("FUN_0012e1a8");
void snd_resolve_bank_xrefs(void) {
    spike_deferred("snd_resolve_bank_xrefs", "audio: resolve references between loaded sound banks");
}

/* ---- SIF RPC to the IOP -> browser services ----------------------------------------------------
   The EE reaches IOP modules through sceSifBindRpc / sceSifCallRpc, and moves bulk data with
   sceSifSetDma. There is no IOP here. Each IOP server the game uses needs a real replacement;
   the one implemented is the "stash" (server 0x11), which the game uses as spare RAM on the IOP:
   src/streaming/stash sends blocks there by DMA and fetches them back by RPC. Its replacement is
   a block of linear memory addressed with the same IOP addresses.

   Assumption, not read from the IOP module: the stash base (0x00100000) and capacity (1 MiB).
   The game only compares offsets against the capacity the INFO call reports, so the base value
   is arbitrary; a too-small capacity would show up as stash_send_data returning -1.

   Calls to any other server trap by name. */
#include "rnc/sdk/sifrpc.h"

__attribute__((import_module("env"), import_name("spike_unimplemented_rpc")))
extern void spike_unimplemented_rpc(s32 server, s32 function);

enum {
    STASH_SERVER = 0x11,
    SOUND_SERVER = 0x123456,        /* 989snd commands (snd_start_sound_system) */
    SOUND_STREAM_SERVER = 0x123457, /* 989snd streaming */
    STASH_FETCH = 1,
    STASH_INFO = 2,
    STASH_IOP_BASE = 0x00100000,
    STASH_CAPACITY = 0x00100000,
    RPC_CLIENT_CAPACITY = 32
};
static u8 stash_memory[STASH_CAPACITY];
static struct { void *client; s32 server; } rpc_clients[RPC_CLIENT_CAPACITY];
static s32 rpc_client_count;
static s32 sound_batches, sound_commands, stream_starts, sound_banks_by_location;
s32 spike_sound_banks_by_location(void);
static s32 spr_transfers;
s32 spike_spr_transfers(void) { return spr_transfers; }

/* The 989snd commands the game waits on, answered from a model of VAG streams; nothing is played.
     0x2C play a VAG stream (snd_play_vag_stream_by_loc_ex_cb: { location, size, ... }): a nonzero
          handle (0 makes music_secondary_start_callback start the request again).
     0x4F is it buffered: 1 for a live handle (music_channel_ready_callback then marks the channel
          ready, which start_scene waits for).
     0x19 is it still playing / 0x32 time remaining: from the stream's length, read from a VAGp
          header at its disc location (big-endian data size at +0x0C, sample rate at +0x10;
          28 samples per 16-byte block) and the vsyncs since it started (60 a second).
   ASSUMED, not read from the IOP module: that the location is a disc sector, and that time
   remaining is in 48 kHz samples (the game's own placeholder is 48000). A stream without a VAGp
   header (help-desk voice lines, for instance) takes its length from the request itself:
   { location, size in bytes, pitch << 16 | volume, ... } (snd_play_vag_stream_by_loc_ex_cb), the
   SPU pitch giving the rate (0x1000 = 48 kHz). Before that fallback, such a stream reported
   "still playing" forever and the help message waiting on it never finished. Other commands
   answer 0. */
static struct { s32 live; s32 start; s32 samples; s32 rate; } vag_streams[64];
static s32 streams_without_header;
s32 spike_streams_without_header(void) { return streams_without_header; }
static s32 vsync_count(void);
static s32 sound_command(s32 command, const void *data, s32 size) {
    if (command == 0x2C && size >= 4) {
        /* A stream frees its slot when its time is up, whether or not the game asks about it
           (on the IOP a finished stream releases its voice); with every slot live, the oldest
           is replaced. Before this, slots leaked: after 64 streams every request was answered
           0, which music_secondary_start_callback retries at once, filling the command buffer
           until snd_send_iop_command_no_wait spun forever in snd_flush_sound_commands. */
        s32 oldest = 0;
        for (s32 i = 0; i < 64; i++) {
            if (!vag_streams[i].live) continue;
            if (vag_streams[i].samples >= 0 && (vsync_count() - vag_streams[i].start) * vag_streams[i].rate / 60 >= vag_streams[i].samples)
                vag_streams[i].live = 0;
            else if (vag_streams[i].start < vag_streams[oldest].start || !vag_streams[oldest].live)
                oldest = i;
        }
        s32 any_free = 0;
        for (s32 i = 0; i < 64; i++) if (!vag_streams[i].live) any_free = 1;
        if (!any_free) vag_streams[oldest].live = 0;
        for (s32 i = 0; i < 64; i++) {
            if (vag_streams[i].live) continue;
            static u8 sector[2048];
            const s32 location = *(const s32 *)data;
            vag_streams[i].live = 1;
            vag_streams[i].start = vsync_count();
            vag_streams[i].samples = -1;
            vag_streams[i].rate = 0;
            if (location > 0 && host_disc_read((u32)location, 1, sector) && sector[0] == 'V' &&
                sector[1] == 'A' && sector[2] == 'G' && sector[3] == 'p') {
                const u32 bytes = ((u32)sector[12] << 24) | ((u32)sector[13] << 16) | ((u32)sector[14] << 8) | sector[15];
                const u32 rate = ((u32)sector[16] << 24) | ((u32)sector[17] << 16) | ((u32)sector[18] << 8) | sector[19];
                if (rate >= 8000 && rate <= 96000) {
                    vag_streams[i].samples = (s32)(bytes / 16 * 28);
                    vag_streams[i].rate = (s32)rate;
                }
            }
            if (vag_streams[i].samples < 0 && size >= 12) {
                const s32 bytes = ((const s32 *)data)[1];
                const u32 pitch = (u32)((const s32 *)data)[2] >> 16;
                streams_without_header += 1;
                if (bytes > 0 && pitch > 0) {
                    vag_streams[i].samples = bytes / 16 * 28;
                    vag_streams[i].rate = (s32)((pitch * 48000u) / 0x1000u);
                }
            }
            stream_starts += 1;
            return 0x100 + i;
        }
        return 0;
    }
    if (size >= 4 && (command == 0x4F || command == 0x19 || command == 0x32)) {
        const s32 handle = *(const s32 *)data - 0x100;
        s32 elapsed, total;
        if (handle < 0 || handle >= 64 || !vag_streams[handle].live) return 0;
        if (command == 0x4F) return 1;
        if (vag_streams[handle].samples < 0) return command == 0x19 ? 1 : 48000;
        elapsed = (vsync_count() - vag_streams[handle].start) * vag_streams[handle].rate / 60;
        total = vag_streams[handle].samples;
        if (elapsed >= total) { if (command == 0x19) vag_streams[handle].live = 0; return 0; }
        return command == 0x19 ? 1 : (s32)((s64)(total - elapsed) * 48000 / vag_streams[handle].rate);
    }
    return 0;
}
s32 spike_sound_stream_starts(void) { return stream_starts; }

static s32 rpc_server_of(void *client) {
    s32 i;
    for (i = 0; i < rpc_client_count; i++) {
        if (rpc_clients[i].client == client) return rpc_clients[i].server;
    }
    /* A client bound before this module took over (the boot program binds, the level program it
       loads calls): the binding is in the client data in EE RAM, as on the console. */
    {
        const u32 bound = (u32)((struct sceSifClientData *)client)->serve;
        if (client && (bound == STASH_SERVER || bound == SOUND_SERVER || bound == SOUND_STREAM_SERVER)) return (s32)bound;
    }
    return -1;
}

static u8 *stash_at(u32 iop_address, u32 bytes) {
    if (iop_address < STASH_IOP_BASE || bytes > STASH_CAPACITY ||
        iop_address - STASH_IOP_BASE > STASH_CAPACITY - bytes) {
        __builtin_trap(); /* outside the stash: a bug here or in the caller, never ignored */
    }
    return stash_memory + (iop_address - STASH_IOP_BASE);
}

/* sceSifInitRpc: sets up the SIF RPC layer (kernel interrupt handlers, DMA buffers, the IOP's
   server table). The calls above complete inside the call, so there is nothing to set up; the
   recovered SDK version (src/sdk/rpc/rpc/sce_sif_init_rpc.c) would need the EE kernel. */
void sceSifInitRpc(s32 mode);
void sceSifInitRpc(s32 mode) { (void)mode; }

s32 sceSifBindRpc(struct sceSifClientData *client, s32 server, s32 mode);
s32 sceSifBindRpc(struct sceSifClientData *client, s32 server, s32 mode) {
    (void)mode;
    if (server != STASH_SERVER && server != SOUND_SERVER && server != SOUND_STREAM_SERVER) {
        spike_unimplemented_rpc(server, -1);
        return -1;
    }
    if (rpc_server_of(client) < 0) {
        if (rpc_client_count == RPC_CLIENT_CAPACITY) __builtin_trap();
        rpc_clients[rpc_client_count].client = client;
        rpc_clients[rpc_client_count].server = server;
        rpc_client_count += 1;
    }
    client->serve = (void *)(u32)server; /* nonzero: bound */
    return 0;
}

/* The bind and every call complete before returning, so no call is ever in flight. */
s32 SceSifCheckStatRpc(void *client);
s32 SceSifCheckStatRpc(void *client) { (void)client; return 0; }

s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send, s32 send_bytes,
                  void *receive, s32 receive_bytes, void *end_function, void *end_parameter);
s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send, s32 send_bytes,
                  void *receive, s32 receive_bytes, void *end_function, void *end_parameter) {
    const s32 server = rpc_server_of(client);
    (void)mode;
    (void)send_bytes;
    (void)end_function;
    (void)end_parameter;
    if (server == SOUND_STREAM_SERVER && function == 3 && receive_bytes >= 4) {
        /* snd_BankLoadByLoc: { disc location, byte offset } -> a bank handle in the receive word.
           The 989snd driver would read the bank into sound RAM; there is no audio engine yet, so
           the handle names the request (0x8000 + n, nonzero) and nothing is loaded. Counted. */
        sound_banks_by_location += 1;
        *(s32 *)receive = 0x8000 + sound_banks_by_location;
        return 0;
    }
    if (server == STASH_SERVER && function == STASH_INFO && receive_bytes >= 8) {
        s32 reply[2] = { STASH_IOP_BASE, STASH_CAPACITY };
        __builtin_memcpy(receive, reply, sizeof(reply));
        return 0;
    }
    if (server == STASH_SERVER && function == STASH_FETCH && receive_bytes >= 0) {
        u32 iop_address;
        __builtin_memcpy(&iop_address, send, 4);
        __builtin_memcpy(receive, stash_at(iop_address, (u32)receive_bytes), (u32)receive_bytes);
        return 0;
    }
    if (server == SOUND_SERVER && receive_bytes >= 8) {
        /* The 989snd driver on the IOP runs each queued command and answers in the reply buffer:
           a 0xFFFFFFFF marker, one return word per command, then another 0xFFFFFFFF
           (snd_got_returns checks both markers). Function 0x4D is a batch (snd_send_current_batch:
           a count word, then records { s16 command, s16 size, size bytes padded to 4 }); any other
           function is one command with the payload as its data (snd_send_iop_command_and_wait). */
        const s32 words = receive_bytes / 4;
        s32 *reply = receive;
        for (s32 i = 1; i < words - 1; i++) reply[i] = 0;
        if (function == 0x4D) {
            const u8 *record = (const u8 *)send + 4;
            const s32 count = *(const s32 *)send;
            for (s32 i = 0; i < count && i < words - 2; i++) {
                const s16 command = *(const s16 *)record, size = *(const s16 *)(record + 2);
                reply[1 + i] = sound_command(command, record + 4, size);
                record += 4 + ((size + 3) & ~3);
            }
        } else if (words >= 3) {
            reply[1] = sound_command(function, send, send_bytes);
        }
        reply[0] = -1;
        reply[words - 1] = -1;
        sound_batches += 1;
        sound_commands += words - 2;
        return 0;
    }
    spike_unimplemented_rpc(server, function);
    return -1;
}
s32 spike_sound_batches(void) { return sound_batches; }
s32 spike_sound_commands(void) { return sound_commands; }

/* sceSifSetDma: queue `count` EE-to-IOP transfers {source, IOP address, bytes, mode}. Only the
   stash's address range exists on this side. Returns a nonzero transfer id. */
struct SifDmaTransfer { u32 data, addr, size, mode; };
static u8 *iop_heap_bytes(u32 address);
u32 sceSifSetDma(struct SifDmaTransfer *transfers, s32 count);
u32 sceSifSetDma(struct SifDmaTransfer *transfers, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        const u32 addr = transfers[i].addr, size = transfers[i].size;
        if (addr >= 0x00010000 && addr < 0x00100000 && size <= 0x00100000 - addr) {
            /* the IOP heap (sceSifAllocIopHeap below): module images */
            __builtin_memcpy(iop_heap_bytes(addr), (void *)transfers[i].data, size);
            continue;
        }
        __builtin_memcpy(stash_at(addr, size), (void *)transfers[i].data, size);
    }
    return 1;
}

/* sceCdCallback: registers a function the SDK calls from the disc interrupt with the finished
   command's code (1 = read), and returns the previous one. The game registers
   finish_audio_stream_read (0x216950), which marks music_stream_state's read idle; the map and
   HUD loads wait on that (read_disc_sectors_and_wait). Reads finish inside sceCdRead here, so the
   callback runs at the next vertical sync (cd_run_completion, from sceGsSyncV), a point the
   console's interrupt has always passed by. */
void *sceCdCallback(void *callback);
void *sceCdCallback(void *callback) {
    void *previous = (void *)cd_callback;
    cd_callback = (void (*)(s32))callback;
    return previous;
}
static void cd_run_completion(void) {
    if (cd_read_completed && cd_callback) {
        cd_read_completed = 0;
        cd_callback(1);
    }
}

/* src/rendering/vu1/dmac_vif1_enable.c: installs the VIF1 DMA interrupt handler and unmasks the
   channel in the DMA controller at 0x1000E010. The VIF1 chain is how display lists reach the
   vector unit and GS; no renderer consumes it here. */
void dmac_vif1_enable(void) __asm__("FUN_00233d00");
void dmac_vif1_enable(void) {
    spike_deferred("dmac_vif1_enable", "renderer: VIF1 DMA channel and interrupt handler (display lists have no consumer)");
}

/* sceDmaSend (0x1232c8): starts a DMA channel on a chain. VIF1 (registers at 0x10009000) carries
   the frame's VU1 display list from vu1_send_chain_render_buffer; its address is recorded for a
   renderer (spike_vif1_chain_address). The transfer completes at once here, so the VIF1 DMA
   interrupt handler's end state follows: the END tag clears every busy bit of the level's chain
   flags (D_L00_001611A0, which vu1_send_chain_render_buffer sets to 0x1F and vu1_sync_chain waits
   on; see vif1_dma_interrupt_handler.c). No VU1 program runs and nothing is drawn. Other channels
   are reported as not done. */
/* The executable's name: in level.wasm it is pinned at the level copy (D_L00_001611A0, exe-data-l00.json),
   in boot.wasm at its own address. */
extern volatile s32 level_vif_chain_flags __asm__("D_00160EE0");
static u32 vif1_chain_address, vif1_chain_sends;
void sceDmaSend(void *channel, u32 chain) __asm__("FUN_001232c8");
void sceDmaSend(void *channel, u32 chain) {
    if ((u32)channel == 0x10009000u) {
        vif1_chain_address = chain;
        vif1_chain_sends += 1;
        spike_dma_chain(0x10009000u, chain);
        level_vif_chain_flags &= ~0x1f;
        return;
    }
    if ((u32)channel == 0x1000A000u) { /* GIF (PATH3): a chain of GIF packets straight to the GS */
        spike_dma_chain(0x1000A000u, chain);
        return;
    }
    spike_deferred("sceDmaSend", "DMA: a channel other than VIF1 (no consumer)");
}
/* Boot code (vu1_chain.c, through include/rnc/sdk/dma.h) calls it by its name. */
void sceDmaSend_by_name(void *channel, u32 chain) __asm__("sceDmaSend");
void sceDmaSend_by_name(void *channel, u32 chain) { sceDmaSend(channel, chain); }
u32 spike_vif1_chain_address(void) { return vif1_chain_address; }
u32 spike_vif1_chain_sends(void) { return vif1_chain_sends; }

/* DMA channel 9 (main memory to scratchpad), 0x1000D400. WriteDmaChannelRegisters (0x20b3c0)
   stores SADR (+0x80) = destination, QWC (+0x20), MADR (+0x10), then CHCR = 0x100 (start, normal
   mode); dma_to_spr_sync (0x20b3e0) spins until CHCR & 0x100 is clear. Here the transfer runs at
   once: qwc quadwords from MADR (bits 4-24, main memory) to the scratchpad mapped at 0xF0000
   (normalize-sources.py) at SADR bits 4-13, and CHCR's start bit is cleared on return. */
void WriteDmaChannelRegisters(s32 address, s32 qwc, s32 destination) {
    volatile u32 *channel = (volatile u32 *)0x1000D400;
    channel[0x80 / 4] = (u32)destination;
    channel[0x20 / 4] = (u32)qwc;
    channel[0x10 / 4] = (u32)address;
    const u32 bytes = ((u32)qwc & 0xFFFF) * 16;
    u8 *to = (u8 *)0xF0000 + ((u32)destination & 0x3FF0);
    const u8 *from = (const u8 *)((u32)address & 0x01FFFFF0);
    if (((u32)destination & 0x3FF0) + bytes <= 0x4000) __builtin_memcpy(to, from, bytes);
    channel[0] = 0;
    spr_transfers += 1;
}
void dma_to_spr_sync(void) __asm__("FUN_0020b3e0");
void dma_to_spr_sync(void) {}

/* World-geometry rendering passes (stage 4 of native-browser-pipeline.md: "replace the relevant
   PS2 rendering passes with a browser graphics backend"). tfrag_proc (0x233fb0, assembly) streams
   the terrain fragments through the scratchpad and builds VU1 packets for the tfrag microprogram;
   in the browser the WebGL terrain renderer (terrain-geometry.mjs) draws the same fragments, so the
   pass is handed to it: the call is counted and the display list is left unchanged. Game state is
   not read back from this pass. */
static s32 world_passes[5];
s32 spike_world_passes(s32 which) { return which >= 0 && which < 5 ? world_passes[which] : 0; }
extern void spike_world_pass(s32 pass) __attribute__((import_module("env"), import_name("spike_world_pass")));
void tfrag_proc(void) __asm__("FUN_00233fb0");
void tfrag_proc(void) {
    world_passes[0] += 1;
    spike_world_pass(0); /* the host draws the world here, into the frame being built */
    *(u32 *)(0xF0000 + 0x3400) = 0; /* the visible-fragment list { fragments, count } ends at once */
}
/* compute_tfrag_texture_usage (0x234bd8, assembly) is the end of the tfrag pass: it reads the
   fragment list above with the VU0 registers tfrag_proc leaves loaded, after clearing the texture
   usage table (scratchpad 0x3C00, 0xC0 bytes). With the list empty its whole effect is the
   clear, done here; build_tfrag_texture_dma then queues no terrain texture uploads. */
void compute_tfrag_texture_usage(void) __asm__("FUN_00234bd8");
void compute_tfrag_texture_usage(void) { __builtin_memset((void *)(0xF0000 + 0x3C00), 0, 0xC0); }

/* WriteDmaChannel (0x1f9928): DMA channel 8 (scratchpad to main memory), 0x1000D000: SADR (+0x80)
   = scratchpad, MADR (+0x10) = destination, QWC (+0x20), CHCR = 0x100; then an uncached read of
   0x20100000 (write-buffer flush, nothing to do here). The copy runs at once and CHCR's start bit
   is clear on return. */
s32 WriteDmaChannel(u32 destination, u32 scratchpad, u32 qwc) {
    volatile u32 *channel = (volatile u32 *)0x1000D000;
    channel[0x80 / 4] = scratchpad;
    channel[0x10 / 4] = destination;
    channel[0x20 / 4] = qwc;
    const u32 bytes = (qwc & 0xFFFF) * 16;
    if ((scratchpad & 0x3FF0) + bytes <= 0x4000)
        __builtin_memcpy((u8 *)(destination & 0x01FFFFF0), (const u8 *)0xF0000 + (scratchpad & 0x3FF0), bytes);
    channel[0] = 0;
    spr_transfers += 1;
    return 0x100; /* v0, the CHCR value written */
}

/* tie_proc (0x235be8, assembly): the tie (static scenery) pass, culling each placed tie and
   building VU1 packets for the tie microprogram. The browser's tie renderer (tie-geometry.mjs)
   draws them, so the pass is handed to it. For build_tie_texture_dma, which reads one word per
   tie class from scratchpad 0x3200 (the nearest visible distance tie_proc found; 0x7fffffff: not
   visible), every class (count: level 0x160FBC, D_00160F4C) is reported not visible, so the
   PS2 path queues no tie textures. */
extern s32 tie_class_count __asm__("D_00160F4C");   /* level copy 0x160FBC */
extern s32 shrub_class_count __asm__("D_001603CC"); /* level copy 0x16048C */
extern s32 movie_after_words[2] __asm__("D_00161208");
void tie_proc(void) __asm__("FUN_00235be8");
void tie_proc(void) {
    const s32 classes = tie_class_count;
    u32 *distances = (u32 *)(0xF0000 + 0x3200);
    world_passes[1] += 1;
    for (s32 i = 0; i < classes && i < 0x100; i++) distances[i] = 0x7fffffff;
}

/* shrub_proc (0x228be8, assembly): the shrub pass, handed to the browser's shrub renderer
   (shrub-geometry.mjs) like tie_proc. build_shrub_texture_dma reads one nearest distance per
   shrub class from scratchpad 0x3100 (0x7fffffff: not visible); every class (count: level
   0x16048C) is reported not visible, so the PS2 path queues no shrub textures. */
void shrub_proc(void) __asm__("FUN_00228be8");
void shrub_proc(void) {
    const s32 classes = shrub_class_count;
    u32 *distances = (u32 *)(0xF0000 + 0x3100);
    world_passes[2] += 1;
    for (s32 i = 0; i < classes && i < 0x140; i++) distances[i] = 0x7fffffff;
}

/* 0x1f89a4 (assembly, level copy 0x1fe08c): the sprite pass after prepare_glow_sprites. It fades
   each queued item by how much of it the scratchpad samples at 0x2400 show (copied there by
   prepare_glow_sprites), then writes UNPACK headers and per-vertex data from the item's texture
   mesh for the VU1 program font_queue_vu_state queued. Like the other VU1 passes it is handed to
   the browser renderer: counted, display list unchanged. */
void draw_glow_sprite_pass(void) __asm__("FUN_001f89a4");
void draw_glow_sprite_pass(void) { world_passes[3] += 1; }

/* ---- Boot system services (init_once, run from main in --boot mode) ---- */

/* reset_vif1_gif (0x1204b8, sceGsResetPath): VIF1_FBRST = 1 (0x10003C10), VIF1_ERR = 2
   (0x10003C20), VU1 reset through vi28 |= 0x200 then the wait on vi29 bit 0x100 and vi28 = 0x404
   (no VU1 here: nothing to wait for), the two quadwords at 0x1312A0 / 0x1312B0 into the VIF1
   FIFO (0x10005000; kept as the last FIFO write), and GIF_CTRL = 1 (0x10003000). */
static u8 vif1_fifo_init[32];
s32 reset_vif1_gif(void) __asm__("FUN_001204b8");
s32 reset_vif1_gif(void) {
    *(volatile u32 *)0x10003C10 = 1;
    *(volatile u32 *)0x10003C20 = 2;
    __builtin_memcpy(vif1_fifo_init, (const void *)0x1312A0, 32);
    __builtin_memcpy((void *)0x10005000, (const void *)0x1312B0, 16);
    *(volatile u32 *)0x10003000 = 1;
    return 0x10003000;
}

/* The CD library (libcdvd) at its API. On the console these drive the IOP's cdvdman through
   kernel threads, semaphores and SIF RPC; the browser has no IOP, and disc reads are host reads
   (sceCdRead above). sceCdInit and sceCdMmode answer 1 (done), sceCdDiskReady 2 (SCECdComplete:
   disc ready), sceCdBreak 1. Counted. */
static s32 cd_api_calls;
s32 spike_cd_api_calls(void) { return cd_api_calls; }
int sceCdInit(int init_mode) { (void)init_mode; cd_api_calls += 1; return 1; }
s32 sceCdDiskReady(s32 mode) __asm__("FUN_00121190");
s32 sceCdDiskReady(s32 mode) { (void)mode; cd_api_calls += 1; return 2; }
s32 sceCdMmode(s32 media_mode) { (void)media_mode; cd_api_calls += 1; return 1; }
s32 sceCdBreak(void) { cd_api_calls += 1; return 1; }

/* The IOP side, at its API. The browser has no IOP: the drivers the game loads there (989snd,
   cdvdman, padman, mcman, ...) are browser services in this file. sceSifRebootIop and
   sceSifSyncIop report done, sceSifInitIopHeap succeeds, sceSifAllocIopHeap hands out IOP
   addresses from the RPC stash's range, and sceSifLoadModuleBuffer records each module (its
   name from the IRX's .iopmod section is not read here; the module's IOP address is kept) and
   answers a module id. */
enum { IOP_HEAP_BASE = 0x00010000, IOP_HEAP_END = 0x00100000 }; /* below the RPC stash */
static u8 iop_heap_memory[IOP_HEAP_END - IOP_HEAP_BASE];
static s32 iop_modules, iop_reboots;
static u32 iop_heap_cursor = IOP_HEAP_BASE, iop_heap_last = 0;
s32 spike_iop_modules(void) { return iop_modules; }
s32 spike_iop_reboots(void) { return iop_reboots; }
s32 sceSifRebootIop(const char *img) { (void)img; iop_reboots += 1; return 1; }
/* Bound to the address label the normalized tree's callers use (FUN_0011cf10). */
int sceSifSyncIop(void) __asm__("FUN_0011cf10");
int sceSifSyncIop(void) { return 1; }
s32 sceSifInitIopHeap(void) { return 0; }
void *sceSifAllocIopHeap(s32 size) {
    const u32 at = iop_heap_cursor, bytes = ((u32)size + 0xFF) & ~0xFFu;
    if (size <= 0 || bytes > IOP_HEAP_END - at) return 0; /* the SDK's "no memory" */
    iop_heap_cursor += bytes;
    iop_heap_last = at;
    return (void *)at;
}
/* Only the most recent allocation is given back (load_irx_module frees each module's buffer
   before the next); others stay allocated. */
s32 sceSifFreeIopHeap(void *address) {
    if ((u32)address == iop_heap_last && iop_heap_last != 0) { iop_heap_cursor = iop_heap_last; iop_heap_last = 0; }
    return 0;
}
s32 sceSifLoadModuleBuffer(s32 iop_address, s32 arguments_length, const char *arguments) __asm__("FUN_0011cd78");
s32 sceSifLoadModuleBuffer(s32 iop_address, s32 arguments_length, const char *arguments) {
    (void)iop_address; (void)arguments_length; (void)arguments;
    iop_modules += 1;
    return iop_modules;
}
int SceFsReset(void) __asm__("FUN_0011bc48");
int SceFsReset(void) { return 0; }
/* The console's system language setting: 1 (English), which init_once maps to game_language 0. */
int sceScfGetLanguage(void) { return 1; }
/* EE kernel switches with nothing behind them here: the cache and interrupt state. */
s32 EnableCache(s32 which) { (void)which; return 0; }
s32 DIntr(void) { return 0; }
s32 EnableInterrupts(void) { return 0; } /* s32: the normalized callers read a result */
static u8 *iop_heap_bytes(u32 address) { return iop_heap_memory + (address - IOP_HEAP_BASE); }

/* EE kernel GS calls. GsPutIMR writes the GS interrupt mask (IMR, 0x12001010). SetGsCrt selects
   the video mode: kept for the browser display (interlace, mode 2 NTSC / 3 PAL, field/frame).
   Interrupt handlers: RemoveIntcHandler and disabling an interrupt cause have nothing to undo
   here (vertical sync is the host frame, sceGsSyncV). */
static s32 gs_crt_interlace = -1, gs_crt_mode = -1, gs_crt_field = -1;
s32 spike_gs_crt_mode(void) { return gs_crt_mode; }
s32 spike_gs_crt_interlace(void) { return gs_crt_interlace; }
s32 spike_gs_crt_field(void) { return gs_crt_field; }
s32 GsPutIMR(s32 imr) { *(volatile u64 *)0x12001010 = (u64)(u32)imr; return 0; }
void SetGsCrt(s32 interlace, s32 mode, s32 field) { gs_crt_interlace = interlace; gs_crt_mode = mode; gs_crt_field = field; }
s32 RemoveIntcHandler(s32 cause, s32 id) { (void)id; if (cause == 2) vblank_handler = 0; return 0; }
/* RemoveDmacHandler / DisableDmac (0x1190f8): DMA-end interrupts. No DMAC handler is installed
   in this build (dmac_vif1_enable is deferred; DMA channels complete inside their services), so
   there is nothing to remove or mask. */
s32 RemoveDmacHandler(s32 channel, s32 id) { (void)channel; (void)id; return 0; }
s32 disable_dmac(s32 channel) __asm__("FUN_001190f8");
s32 disable_dmac(s32 channel) { (void)channel; return 0; }

/* init_dma (0x20b418): D_PCR (0x1000E020) |= 0x200, D_CTRL (0x1000E000) = 1, and QWC = 0 for
   channels VIF0, VIF1, GIF, fromIPU, toIPU, fromSPR and toSPR (0x10008000 .. 0x1000D400 + 0x20).
   Register writes only; the DMA channels here are services. */
s32 init_dma(void) __asm__("FUN_0020b418");
s32 init_dma(void) {
    static const u32 channels[] = { 0x8000, 0x9000, 0xA000, 0xB000, 0xB400, 0xD000, 0xD400 };
    *(volatile u32 *)0x1000E020 |= 0x200;
    *(volatile u32 *)0x1000E000 = 1;
    for (u32 i = 0; i < sizeof(channels) / sizeof(channels[0]); i++) *(volatile u32 *)(0x10000000 + channels[i] + 0x20) = 0;
    return 0;
}
/* sceSifDmaStat: < 0 when the transfer has finished; sceSifSetDma's copies finish inside it. */
s32 sceSifDmaStat(s32 id) { (void)id; return -1; }
/* 0x11c9b0 is sceSifFreeIopHeap (load_irx_module frees each module's buffer with it); the
   recovered src/sdk/rpc/invoke_state_initializer.c names it otherwise and declares no
   parameter, so callers here pass no address. Bound with that type, it gives back the most
   recent allocation, which is the buffer load_irx_module just used. */
int iop_heap_free_recent(void) __asm__("FUN_0011c9b0");
int iop_heap_free_recent(void) { return sceSifFreeIopHeap((void *)iop_heap_last); }

/* The controller: libdbc / libpad2 (scePad2*) at their API. On the console they reach the IOP's
   dbcman and padman over SIF RPC; here one DualShock 2 is always connected in port 2 slot 0 (the
   socket init_pads opens) and its state comes from the host: spike_pad_buttons() returns the held
   buttons in the report's bit order (byte 0 << 8 | byte 1, 1 = pressed; the page maps the
   keyboard or a gamepad, run-closure.mjs a --pad script). scePad2Read answers the 0x12-byte
   DualShock 2 report: active-low buttons, the analog sticks from the host (spike_pad_sticks:
   bytes right x, right y, left x, left y, 0x80 = centred, 0x00 = up / left), no pressure. */
extern u32 spike_pad_buttons(void) __attribute__((import_module("env"), import_name("spike_pad_buttons")));
extern u32 spike_pad_sticks(void) __attribute__((import_module("env"), import_name("spike_pad_sticks")));
static s32 pad_reads;
s32 spike_pad_reads(void) { return pad_reads; }
s32 sceDbcInit(void) { return 1; }
s32 scePad2Init(void) { return 1; }
s32 scePad2CreateSocket(void *param, void *addr) { (void)param; (void)addr; return 0; }
s32 scePad2GetState(s32 socket) { (void)socket; return 1; /* ready */ }
s32 scePad2GetButtonProfile(s32 socket, u8 *profile) {
    (void)socket;
    profile[0] = 0xFF; profile[1] = 0xFF; profile[2] = 0xFF; profile[3] = 0xFF; /* every button */
    return 4;
}
s32 sceVibGetProfile(s32 port, u8 *profile) { (void)port; profile[0] = 0x03; profile[1] = 0; return 2; }
s32 scePad2Read(s32 port, u8 *report) {
    (void)port;
    const u32 held = spike_pad_buttons() & 0xFFFF, low = ~held & 0xFFFF;
    report[0] = (u8)(low >> 8);
    report[1] = (u8)low;
    const u32 sticks = spike_pad_sticks();
    for (s32 i = 2; i < 6; i++) report[i] = (u8)(sticks >> (8 * (i - 2)));
    for (s32 i = 6; i < 0x12; i++) report[i] = 0;
    pad_reads += 1;
    return 0x12;
}

/* EE kernel interrupt controller calls. AddIntcHandler(cause, handler, next) installs a handler
   (id 1 + cause); cause 2 runs at each vertical sync (sceGsSyncV). Other causes are recorded only:
   no source raises them here. enable_intc / disable_intc (0x119090 / 0x119028) mask a cause;
   the vertical blank handler runs whenever one is installed. */
static s32 intc_handlers_other;
s32 AddIntcHandler(s32 cause, s32 handler, s32 next) {
    (void)next;
    if (cause == 2) vblank_handler = (int (*)(void))handler;
    else intc_handlers_other += 1;
    return 1 + cause;
}
s32 spike_intc_handlers_other(void) { return intc_handlers_other; }
s32 enable_intc(s32 cause) __asm__("FUN_00119090");
s32 enable_intc(s32 cause) { (void)cause; return 1; }
s32 disable_intc(s32 cause) __asm__("FUN_00119028");
s32 disable_intc(s32 cause) { (void)cause; return 1; }

/* The memory card: libmc at its API (on the console, mcman / mcserv on the IOP over SIF RPC),
   as a filesystem in browser memory: one formatted 8 MB PS2 card (type 2) in port 0 slot 0, empty
   at start (the page can later persist it). Directories and files with names up to 31 bytes,
   absolute ("/BASCUS-97199RATCHET/...") or relative to the directory sceMcChdir set. Every call
   is accepted at once (returns 0) and its result is what sceMcSync reports next, with the
   command's libmc function code, as the IOP side would. Result codes follow libmc: Open returns a
   descriptor or -4 (not found); Chdir 0 or -4; Mkdir 0 or -4 (exists); Read/Write the byte
   count; Delete 0 or -4; GetDir the number of entries written as 64-byte sceMcTblGetDir records.
   Port 1 has no card (GetInfo -10). Flush and file info are not used by the game. */
enum { MC_FUNC_GET_INFO = 1, MC_FUNC_OPEN = 2, MC_FUNC_CLOSE = 3, MC_FUNC_SEEK = 4, MC_FUNC_READ = 5,
       MC_FUNC_WRITE = 6, MC_FUNC_MKDIR = 0x0B, MC_FUNC_CHDIR = 0x0C, MC_FUNC_GET_DIR = 0x0D,
       MC_FUNC_DELETE = 0x0F };
enum { MC_ENTRIES = 64, MC_NAME = 64, MC_FILE_BYTES = 0x40000, MC_DESCRIPTORS = 8 };
static struct { s32 used, dir; char path[MC_NAME]; u8 *data; s32 size; } mc_entries[MC_ENTRIES];
static u8 mc_storage[MC_ENTRIES][MC_FILE_BYTES];
static struct { s32 open, entry, position, mode; } mc_fds[MC_DESCRIPTORS];
static char mc_cwd[MC_NAME] = "/";
static s32 mc_last_code = 0, mc_last_result = 0, mc_calls;
s32 spike_mc_calls(void) { return mc_calls; }
static s32 mc_done(s32 code, s32 result) { mc_calls += 1; mc_last_code = code; mc_last_result = result; return 0; }
static void mc_resolve(const char *name, char *out) {
    s32 n = 0;
    if (name[0] != '/') {
        for (const char *c = mc_cwd; *c && n < MC_NAME - 1; c++) out[n++] = *c;
        if (n > 0 && out[n - 1] != '/' && n < MC_NAME - 1) out[n++] = '/';
    }
    for (const char *c = name; *c && n < MC_NAME - 1; c++) out[n++] = *c;
    while (n > 1 && out[n - 1] == '/') n--;
    out[n] = 0;
}
static s32 mc_same(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }
static s32 mc_find(const char *path) {
    for (s32 i = 0; i < MC_ENTRIES; i++) if (mc_entries[i].used && mc_same(mc_entries[i].path, path)) return i;
    return -1;
}
static s32 mc_parent_exists(const char *path) {
    char parent[MC_NAME];
    s32 last = 0;
    for (s32 i = 0; path[i]; i++) if (path[i] == '/') last = i;
    if (last == 0) return 1; /* the root */
    for (s32 i = 0; i < last; i++) parent[i] = path[i];
    parent[last] = 0;
    const s32 e = mc_find(parent);
    return e >= 0 && mc_entries[e].dir;
}
static s32 mc_create(const char *path, s32 dir) {
    for (s32 i = 0; i < MC_ENTRIES; i++) {
        if (mc_entries[i].used) continue;
        mc_entries[i].used = 1; mc_entries[i].dir = dir; mc_entries[i].size = 0; mc_entries[i].data = mc_storage[i];
        for (s32 k = 0; k < MC_NAME; k++) { mc_entries[i].path[k] = path[k]; if (!path[k]) break; }
        return i;
    }
    return -1;
}
s32 sceMcInit(void) { mc_calls += 1; return 0; }
s32 sceMcGetInfo(s32 port, s32 slot, s32 type, s32 free_blocks, s32 format) {
    (void)slot;
    if (port != 0) return mc_done(MC_FUNC_GET_INFO, -10); /* no card */
    if (type) *(s32 *)type = 2;
    if (free_blocks) *(s32 *)free_blocks = 8000;
    if (format) *(s32 *)format = 1;
    return mc_done(MC_FUNC_GET_INFO, 0); /* the same card as before */
}
s32 sceMcChdir(s32 port, s32 slot, s8 *path, s32 pwd) {
    char full[MC_NAME];
    (void)port; (void)slot; (void)pwd;
    mc_resolve((const char *)path, full);
    if (!(full[0] == '/' && full[1] == 0)) {
        const s32 e = mc_find(full);
        if (e < 0 || !mc_entries[e].dir) return mc_done(MC_FUNC_CHDIR, -4);
    }
    for (s32 k = 0; k < MC_NAME; k++) { mc_cwd[k] = full[k]; if (!full[k]) break; }
    return mc_done(MC_FUNC_CHDIR, 0);
}
s32 sceMcMkdir(s32 port, s32 slot, const char *name) {
    char full[MC_NAME];
    (void)port; (void)slot;
    mc_resolve(name, full);
    if (mc_find(full) >= 0) return mc_done(MC_FUNC_MKDIR, -4);
    if (!mc_parent_exists(full)) return mc_done(MC_FUNC_MKDIR, -4);
    return mc_done(MC_FUNC_MKDIR, mc_create(full, 1) >= 0 ? 0 : -3);
}
s32 sceMcOpen(s32 port, s32 slot, char *name, s32 mode) {
    char full[MC_NAME];
    (void)port; (void)slot;
    mc_resolve(name, full);
    s32 e = mc_find(full);
    if (e < 0) {
        if (!(mode & 0x200) || !mc_parent_exists(full)) return mc_done(MC_FUNC_OPEN, -4);
        e = mc_create(full, 0);
        if (e < 0) return mc_done(MC_FUNC_OPEN, -3);
    }
    if (mc_entries[e].dir) return mc_done(MC_FUNC_OPEN, -4);
    if (mode & 0x400) mc_entries[e].size = 0; /* truncate */
    for (s32 fd = 0; fd < MC_DESCRIPTORS; fd++) {
        if (mc_fds[fd].open) continue;
        mc_fds[fd].open = 1; mc_fds[fd].entry = e; mc_fds[fd].position = 0; mc_fds[fd].mode = mode;
        return mc_done(MC_FUNC_OPEN, fd);
    }
    return mc_done(MC_FUNC_OPEN, -7);
}
s32 sceMcClose(s32 fd) {
    if (fd < 0 || fd >= MC_DESCRIPTORS || !mc_fds[fd].open) return mc_done(MC_FUNC_CLOSE, -4);
    mc_fds[fd].open = 0;
    return mc_done(MC_FUNC_CLOSE, 0);
}
s32 sceMcSeek(s32 fd, s32 offset, s32 origin) {
    if (fd < 0 || fd >= MC_DESCRIPTORS || !mc_fds[fd].open) return mc_done(MC_FUNC_SEEK, -4);
    const s32 base = origin == 0 ? 0 : origin == 1 ? mc_fds[fd].position : mc_entries[mc_fds[fd].entry].size;
    mc_fds[fd].position = base + offset < 0 ? 0 : base + offset;
    return mc_done(MC_FUNC_SEEK, mc_fds[fd].position);
}
s32 sceMcRead(s32 fd, s32 *destination, s32 bytes) {
    if (fd < 0 || fd >= MC_DESCRIPTORS || !mc_fds[fd].open) return mc_done(MC_FUNC_READ, -4);
    const s32 e = mc_fds[fd].entry, at = mc_fds[fd].position;
    s32 n = mc_entries[e].size - at;
    if (n > bytes) n = bytes;
    if (n < 0) n = 0;
    __builtin_memcpy(destination, mc_entries[e].data + at, (u32)n);
    mc_fds[fd].position += n;
    return mc_done(MC_FUNC_READ, n);
}
s32 sceMcWrite(s32 fd, u8 *buffer, s32 bytes) {
    if (fd < 0 || fd >= MC_DESCRIPTORS || !mc_fds[fd].open) return mc_done(MC_FUNC_WRITE, -4);
    const s32 e = mc_fds[fd].entry, at = mc_fds[fd].position;
    if (at + bytes > MC_FILE_BYTES) return mc_done(MC_FUNC_WRITE, -8);
    __builtin_memcpy(mc_entries[e].data + at, buffer, (u32)bytes);
    mc_fds[fd].position += bytes;
    if (mc_fds[fd].position > mc_entries[e].size) mc_entries[e].size = mc_fds[fd].position;
    return mc_done(MC_FUNC_WRITE, bytes);
}
s32 sceMcDelete(s32 port, s32 slot, char *name) {
    char full[MC_NAME];
    (void)port; (void)slot;
    mc_resolve(name, full);
    const s32 e = mc_find(full);
    if (e < 0) return mc_done(MC_FUNC_DELETE, -4);
    mc_entries[e].used = 0;
    return mc_done(MC_FUNC_DELETE, 0);
}
s32 sceMcGetDir(s32 port, s32 slot, s8 *name, s32 mode, s32 maxent, s32 table) {
    char pattern[MC_NAME], dir[MC_NAME];
    (void)port; (void)slot; (void)mode;
    mc_resolve((const char *)name, pattern);
    /* "<dir>/<star>" lists that directory; "<path>" names one entry. */
    s32 n = 0, star = 0, last = 0;
    for (s32 i = 0; pattern[i]; i++) { if (pattern[i] == '*') star = 1; if (pattern[i] == '/') last = i; }
    for (s32 i = 0; i < last; i++) dir[i] = pattern[i];
    dir[last] = 0;
    u8 *out = (u8 *)table;
    for (s32 i = 0; i < MC_ENTRIES && n < maxent; i++) {
        if (!mc_entries[i].used) continue;
        const char *p = mc_entries[i].path;
        if (star) {
            s32 k = 0;
            while (dir[k] && p[k] == dir[k]) k++;
            if (dir[k] || p[k] != '/') continue;
            s32 nested = 0;
            for (const char *c = p + k + 1; *c; c++) if (*c == '/') nested = 1;
            if (nested) continue;
        } else if (!mc_same(p, pattern)) continue;
        if (out) {
            u8 *rec = out + 64 * n;
            __builtin_memset(rec, 0, 64);
            *(u32 *)(rec + 16) = (u32)mc_entries[i].size;
            *(u16 *)(rec + 20) = mc_entries[i].dir ? 0x8427 : 0x8497;
            const char *leaf = p;
            for (const char *c = p; *c; c++) if (*c == '/') leaf = c + 1;
            for (s32 k = 0; k < 31 && leaf[k]; k++) rec[32 + k] = (u8)leaf[k];
        }
        n++;
    }
    return mc_done(MC_FUNC_GET_DIR, n);
}
s32 sceMcSync(s32 poll_mode, s32 *active_code_out, s32 *completion_status_out) {
    (void)poll_mode;
    if (active_code_out) *active_code_out = mc_last_code;
    if (completion_status_out) *completion_status_out = mc_last_result;
    return 1; /* finished */
}

/* Movies. play_mpeg_movie (0x23a3b8) runs the game's MPEG player: libmpeg on the EE feeding the
   IPU (the PS2's MPEG-2 decoder) from a CD stream, in its own thread. The browser has no IPU, so
   the movie is handed to the host at this call: spike_play_movie(sector, bytes, channel) plays
   the PSS stream at that disc extent with the audio channel the caller picked (the fifth
   argument, init_all's third: 0 for the boot intro, the language track for cutscenes) and
   returns when it ends or is skipped (the page decodes and shows
   it; run-closure.mjs records it and returns). Then, as the original does after term_all, the
   words at 0x161208 / 0x16120C are cleared. Returns 0, as the original does. */
extern void spike_play_movie(s32 sector, s32 bytes, s32 channel) __attribute__((import_module("env"), import_name("spike_play_movie")));
static s32 movies_played;
s32 spike_movies_played(void) { return movies_played; }
s32 play_mpeg_movie(s32 movie_source, s32 movie_size, s32 decoder_buffer_base, void *player, s32 audio_channel)
    __asm__("FUN_0023a3b8");
s32 play_mpeg_movie(s32 movie_source, s32 movie_size, s32 decoder_buffer_base, void *player, s32 audio_channel) {
    (void)decoder_buffer_base; (void)player;
    movies_played += 1;
    spike_play_movie(movie_source, movie_size, audio_channel);
    movie_after_words[0] = 0; /* the original's literal 0x161208 / 0x16120C (executable addresses) */
    movie_after_words[1] = 0;
    return 0;
}
s32 spike_sound_banks_by_location(void) { return sound_banks_by_location; }

/* part_proc (0x217c18, 765 instructions of assembly): the particle pass, building sprite batches
   for the VU1 particle microprogram from the particle table update_parts runs. Handed to the
   browser renderer like the other VU1 passes (counted, display list unchanged); particles do not
   feed game state. */
void part_proc(void) __asm__("FUN_00217c18");
void part_proc(void) { world_passes[4] += 1; }

/* sceCdReadClock: the console's real-time clock (read through cdvdman on the IOP) as sceCdCLOCK
   { stat, second, minute, hour, pad, day, month, year }, BCD. The host fills it from its own
   clock (spike_read_clock). Returns 1, success. */
extern void spike_read_clock(u8 *clock) __attribute__((import_module("env"), import_name("spike_read_clock")));
s32 sceCdReadClock(u8 *clock) { spike_read_clock(clock); return 1; }
/* sceScfGetLocalTimefromRTC: on the console the RTC runs in Japan time and this shifts it by the
   system configuration's time zone and summer time (read from the ROM). The host clock behind
   sceCdReadClock is already local time, so there is nothing to shift. */
void sceScfGetLocalTimefromRTC(s32 rtc_time) { (void)rtc_time; }
