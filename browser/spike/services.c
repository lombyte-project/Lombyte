/* Browser equivalents of PS2 kernel services reached by the spike build. Each entry states why
   its body is a complete equivalent here, not a placeholder. Anything without a real equivalent
   stays a named trap in the runner instead of being added to this file. */
#include "types.h"

/* FlushCache (0x118A80, EE kernel syscall): invalidates the CPU's instruction and data caches
   after code or data is written to RAM. WebAssembly linear memory has no cache to go stale and
   the module does not execute code loaded into memory, so there is nothing to invalidate. */
void FlushCache(s32 operation);
void FlushCache(s32 operation) { (void)operation; }
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
void load_hud_banks(void) { spike_deferred("load_hud_banks", "audio: HUD sound banks (no browser audio backend)"); }

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
s32 sceCdRead(u32 lsn, u32 sectors, void *buffer, void *mode) {
    (void)mode;
    return host_disc_read(lsn, sectors, buffer);
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

/* sceGsSyncV (0x122298): blocks until the next vertical sync. Frame pacing belongs to the host
   (requestAnimationFrame in a page); this build has no frame loop yet, so the wait returns at
   once. Counted so a run shows how many frame boundaries the code asked for. */
static s32 vsync_waits;
s32 sceGsSyncV(s32 mode);
s32 sceGsSyncV(s32 mode) { (void)mode; vsync_waits += 1; return 0; }
s32 sceGsSyncV_by_address(s32 mode) __asm__("FUN_00122298");
s32 sceGsSyncV_by_address(s32 mode) { (void)mode; vsync_waits += 1; return 0; }
s32 spike_vsync_waits(void) { return vsync_waits; }

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
s32 snd_flush_sound_commands(void) __asm__("FUN_0012dc80");
s32 snd_flush_sound_commands(void) {
    spike_deferred("snd_flush_sound_commands", "audio: send queued commands to the IOP sound driver");
    return 0;
}
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
    STASH_FETCH = 1,
    STASH_INFO = 2,
    STASH_IOP_BASE = 0x00100000,
    STASH_CAPACITY = 0x00100000,
    RPC_CLIENT_CAPACITY = 32
};
static u8 stash_memory[STASH_CAPACITY];
static struct { void *client; s32 server; } rpc_clients[RPC_CLIENT_CAPACITY];
static s32 rpc_client_count;

static s32 rpc_server_of(void *client) {
    s32 i;
    for (i = 0; i < rpc_client_count; i++) {
        if (rpc_clients[i].client == client) return rpc_clients[i].server;
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

s32 sceSifBindRpc(struct sceSifClientData *client, s32 server, s32 mode);
s32 sceSifBindRpc(struct sceSifClientData *client, s32 server, s32 mode) {
    (void)mode;
    if (server != STASH_SERVER) {
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
    spike_unimplemented_rpc(server, function);
    return -1;
}

/* sceSifSetDma: queue `count` EE-to-IOP transfers {source, IOP address, bytes, mode}. Only the
   stash's address range exists on this side. Returns a nonzero transfer id. */
struct SifDmaTransfer { u32 data, addr, size, mode; };
u32 sceSifSetDma(struct SifDmaTransfer *transfers, s32 count);
u32 sceSifSetDma(struct SifDmaTransfer *transfers, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        __builtin_memcpy(stash_at(transfers[i].addr, transfers[i].size), (void *)transfers[i].data, transfers[i].size);
    }
    return 1;
}

/* sceCdCallback: registers a function the SDK calls from the disc interrupt when a command
   completes, and returns the previous one. The game registers its audio-stream callback here.
   The callback is recorded and returned correctly, but it is NOT invoked: reads complete inside
   sceCdRead, and the only registered user is audio streaming, which has no backend. */
static void *cd_callback;
void *sceCdCallback(void *callback);
void *sceCdCallback(void *callback) {
    void *previous = cd_callback;
    cd_callback = callback;
    spike_deferred("sceCdCallback", "audio: disc-completion callback recorded but never invoked (only audio streaming uses it)");
    return previous;
}

/* src/rendering/vu1/dmac_vif1_enable.c: installs the VIF1 DMA interrupt handler and unmasks the
   channel in the DMA controller at 0x1000E010. The VIF1 chain is how display lists reach the
   vector unit and GS; no renderer consumes it here. */
void dmac_vif1_enable(void) __asm__("FUN_00233d00");
void dmac_vif1_enable(void) {
    spike_deferred("dmac_vif1_enable", "renderer: VIF1 DMA channel and interrupt handler (display lists have no consumer)");
}
