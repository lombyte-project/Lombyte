// Scenario for run-closure.mjs: Veldin's original level start-up, as four calls in the order the
// game makes them:
//   initialize_level_runtime()             boot: resident archive, resident classes, moby pool
//   init_stash(); init_level_runtime_state()   first steps of run_level_main_loop
//   load_level_assets_and_gameplay(1, 0)   level archive, classes, textures, then gameplay
// Needs RNC_ISO_PATH, --elf and --level 0. Link with all four roots (see ROOTS).
//
// Real: the boot ELF image, the disc table and level header (copied from the disc the way the
// original readers leave them), the three level archives, and every loader line that runs.
// The archives are placed where src/game/service_level_archive_load.c places them: stacked down
// from 0x1FF8000, each rounded up to 4 KiB (gameplay archive, then the ship-variant level archive,
// then the shared level file), with the same three pointers set.
// Staged by hand, standing in for startup code not run here: the work-buffer pointers in
// D_L00_00173E00. Those buffers live above the 32 MiB RAM image, so they are NOT the console's
// addresses.
import { closeSync, openAsBlob, openSync, readSync } from 'node:fs';
import { readFile } from 'node:fs/promises';
import { IsoReader, SECTOR_BYTES } from '../iso-reader.mjs';
import { initializeDiscTable } from '../disc-loader.mjs';

const DISC_TABLE = 0x00137B80;
const LEVEL_HEADER = 0x0013A4E0; // disc_table + 0x2960: the 0x2434 bytes read_file_entry_with_retry copies
const LEVEL_HEADER_BYTES = 0x2434;
const LEVEL_ROOT = 0x0015EE4C; // [0] shared level file, [1] gameplay archive, [2] level archive (D_0015EE54)
const ARCHIVE_TOP = 0x01FF8000; // D_1FF8000
const SHARED_ARCHIVE = 0x2968; // disc_table offsets of {sector, size} pairs
const GAMEPLAY_ARCHIVE = 0x2970;
const LEVEL_ARCHIVES = 0x12C8;
const LEVEL_LOAD = 0x00173E00; // D_L00_00173E00 buffer descriptor
export const BUFFERS = Object.freeze({
  header: 0x02100000, // +0x04: core header, later the decoded gameplay header
  scratch0: 0x07000000, // +0x00
  scratchC: 0x07400000, // +0x0C
  scratch10: 0x07800000, // +0x10
  assets: 0x04000000, // +0x14: decompressed level assets
  gameplay: 0x06000000, // +0x18: gameplay arena (moby table and what follows)
  scratch1C: 0x07C00000, // +0x1C
});
const MEMORY_BYTES = 0x0A000000;
const MAIN_HEAP = 0x001940C0; // D_001940C0: +0x14 is the buffer initialize_level_runtime unpacks into
const RESIDENT_BUFFER = 0x08000000;
export const ROOTS = ['FUN_00230f60', 'FUN_00232d00', 'FUN_L00_0023d028', 'FUN_L00_00244110'];
const archives = {};

const stats = { decompress: [], sectorReads: 0, sectorsRead: 0 };
let decoder = null;
let disc = null;

const load = (name) => readFile(new URL(`../../build/browser/${name}.wasm`, import.meta.url))
  .then((bytes) => WebAssembly.instantiate(bytes, {}))
  .then(({ instance }) => { instance.exports._initialize?.(); return instance.exports; });

export function services(context) {
  // fast_decompress(source, destination): the console routine is assembly-only. Decode the WAD
  // at `source` with the existing native decoder (browser/wad.c) and return the decoded size.
  const decompress = (source, destination) => {
      const bytes = context.bytes();
      if (bytes[source] !== 0x57 || bytes[source + 1] !== 0x41 || bytes[source + 2] !== 0x44) {
        throw new Error(`fast_decompress: no WAD stream at 0x${source.toString(16)}`);
      }
      const length = context.view().getUint32(source + 3, true);
      if (length <= 0 || length > decoder.wad_input_capacity()) throw new Error(`fast_decompress: bad WAD size ${length}`);
      decoder.wad_reset();
      new Uint8Array(decoder.memory.buffer, decoder.wad_input_address(), length).set(bytes.subarray(source, source + length));
      const size = decoder.wad_decode(length, decoder.wad_output_capacity());
      if (size <= 0 || decoder.wad_ready() !== 1 || decoder.wad_guards_valid() !== 1) {
        throw new Error(`fast_decompress: decode failed (${size})`);
      }
      context.bytes().set(new Uint8Array(decoder.memory.buffer, decoder.wad_output_address(), size), destination);
      decoder.wad_reset();
      stats.decompress.push({ source, destination, length, size });
      stageCollision(destination);
      return size;
  };
  // The decoded level assets hold the static collision mesh. Copy it into the collision module's
  // own checked storage (browser/collision.c) as soon as the assets are decoded, so the original
  // code's first ground probe (initialize_hero) has a world to query. The range is the one the
  // game page uses: asset header word 0x14 up to the texture payload at word 0x60.
  const stageCollision = (destination) => {
    const view = context.view();
    // Boot code uses the same descriptor for the resident archive before the level program loads.
    if (!stats.levelProgram || destination !== view.getUint32(LEVEL_LOAD + 0x14, true)) return; // not the level assets
    const header = view.getUint32(LEVEL_LOAD + 0x04, true);
    const start = view.getUint32(header + 0x14, true);
    const end = view.getUint32(header + 0x60, true);
    const { runtime_collision_reset: reset, runtime_collision_stage_address: stage,
      runtime_collision_capacity: capacity, runtime_collision_load: loadMesh } = context.exports;
    if (!loadMesh) throw new Error('the collision module is not linked');
    if (!(start > 0 && end > start && end - start <= capacity())) throw new Error(`collision range ${start}-${end} is not usable`);
    reset();
    context.bytes().copyWithin(stage(), destination + start, destination + end);
    const result = loadMesh(end - start);
    if (result < 0) throw new Error(`collision mesh rejected (${result})`);
    stats.collision = { bytes: end - start, cells: context.exports.runtime_collision_cells(), faces: context.exports.runtime_collision_faces() };
  };
  return {
    FUN_L00_0024d8d0: decompress, // the level program's copy
    FUN_0020b618: decompress, // decompress_wad, the boot program's copy
    // The disc: sceCdRead in services.c lands here. Returns 1, the SDK's "command accepted".
    host_disc_read(sector, count, destination) {
      if (sector < 0 || count <= 0 || destination + count * SECTOR_BYTES > context.memory.buffer.byteLength) {
        throw new Error(`disc read of ${count} sectors at ${sector} into 0x${destination.toString(16)} is out of range`);
      }
      const got = readSync(disc, context.bytes(), destination, count * SECTOR_BYTES, sector * SECTOR_BYTES);
      if (got !== count * SECTOR_BYTES) throw new Error('short disc read');
      stats.sectorReads += 1;
      stats.sectorsRead += count;
      return 1;
    },
  };
}

// Output-only work with no consumer in this build yet. Reported as NOT DONE on every run.
export const deferred = {
  FUN_00237370: 'light_ties: bakes directional-light vertex colours for the PS2 tie renderer (assembly-only)',
  FUN_0022a5e0: 'the same bake for shrub instances (assembly-only, unnamed)',
  FUN_L00_00251ce8: 'the level copy of executable 0x20ede8 (3,236 bytes of EE/VU0 assembly, unnamed): called as (moby, slot | 0x300) when an animation is set; it fills the moby\'s cached pose in that render slot. Not decoded, so no joint matrices exist for any animated model.',
  FUN_00234f98: 'light_tfrags: the same bake for terrain fragments (assembly-only)',
  FUN_001f4a58: 'fade_to_black: screen fade (renderer)',
  FUN_L00_00250df8: 'update_entity: builds each moby\'s rotation matrix (+0xC0), bounding sphere (+0x00) and collision-grid entry; calls a VU0 microprogram that is not decoded. Positions, rotations and scales are unaffected; matrices and bounds stay zero.',
};

export async function setup(context) {
  const isoPath = process.env.RNC_ISO_PATH;
  if (!isoPath) throw new Error('Set RNC_ISO_PATH to the local USA disc image.');
  const pages = (MEMORY_BYTES - context.memory.buffer.byteLength) / 65536;
  if (pages > 0) context.memory.grow(Math.ceil(pages));
  disc = openSync(isoPath, 'r');
  const runtime = await load('runtime');
  decoder = await load('planet-wad');
  runtime.runtime_init();
  const reader = new IsoReader(await openAsBlob(isoPath));
  await reader.open();
  await initializeDiscTable(runtime, reader);

  // The disc table exactly as the original loader left it in the runtime module.
  const table = new Uint8Array(runtime.memory.buffer, runtime.runtime_disc_table_address(), runtime.runtime_disc_table_size());
  context.bytes().set(table, DISC_TABLE);
  // The level header: what read_file_entry_with_retry(0) copies from the level's extent.
  const headerSector = runtime.runtime_disc_level_header_sector(0);
  readSync(disc, context.bytes(), LEVEL_HEADER, LEVEL_HEADER_BYTES, headerSector * SECTOR_BYTES);
  // service_level_archive_load's layout, for ship variant 0 (level_archives[0 + 1]).
  const view = context.view();
  const extent = (offset) => ({ sector: view.getInt32(DISC_TABLE + offset, true), sectors: view.getInt32(DISC_TABLE + offset + 4, true) });
  const align = (sectors) => ((sectors << 11) + 0xFFF) & 0xFFFFF000;
  let cursor = ARCHIVE_TOP;
  for (const [name, offset] of [['gameplay', GAMEPLAY_ARCHIVE], ['level', LEVEL_ARCHIVES + 8], ['shared', SHARED_ARCHIVE]]) {
    const { sector, sectors } = extent(offset);
    if (sector <= 0 || sectors <= 0 || sectors > 16384) throw new Error(`disc table has no ${name} archive`);
    cursor -= align(sectors);
    if (readSync(disc, context.bytes(), cursor, sectors * SECTOR_BYTES, sector * SECTOR_BYTES) !== sectors * SECTOR_BYTES) {
      throw new Error(`short read of the ${name} archive`);
    }
    archives[name] = { address: cursor, sector, sectors };
  }
  view.setUint32(LEVEL_ROOT, archives.shared.address, true);
  view.setUint32(LEVEL_ROOT + 4, archives.gameplay.address, true);
  view.setUint32(LEVEL_ROOT + 8, archives.level.address, true);
  view.setUint32(LEVEL_LOAD + 0x00, BUFFERS.scratch0, true);
  view.setUint32(LEVEL_LOAD + 0x04, BUFFERS.header, true);
  view.setUint32(LEVEL_LOAD + 0x0C, BUFFERS.scratchC, true);
  view.setUint32(LEVEL_LOAD + 0x10, BUFFERS.scratch10, true);
  view.setUint32(LEVEL_LOAD + 0x14, BUFFERS.assets, true);
  view.setUint32(LEVEL_LOAD + 0x18, BUFFERS.gameplay, true);
  view.setUint32(LEVEL_LOAD + 0x1C, BUFFERS.scratch1C, true);
  view.setUint32(MAIN_HEAP + 0x14, RESIDENT_BUFFER, true); // set by boot code not run here
  for (const [name, { address, sector, sectors }] of Object.entries(archives)) {
    console.log(`  ${name} archive: ${sectors} sectors from ${sector} at 0x${address.toString(16)}`);
  }
  console.log(`staged Veldin: disc table ${table.length} bytes, level header from sector ${headerSector}`);
  return { sequence: [
    { symbol: ROOTS[0], args: [] },
    // The console now loads the level program over 0x15EF00 and up, replacing the boot program's
    // game code and data there (class tables included). Without this, the three resident classes
    // registered by call 1 stay in the level's class table, pointing at overwritten memory.
    { host: (run) => {
      run.loadLevelProgram();
      stats.levelProgram = true;
      // The loaded handler tables hold console code addresses; put the compiled C tables there.
      if (!run.exports.spike_install_level_tables) throw new Error('level handler tables are not linked');
      run.exports.spike_install_level_tables();
    }, label: 'level program data reloaded (the console loads the level program here)' },
    { symbol: ROOTS[1], args: [] },
    { symbol: ROOTS[2], args: [] },
    { symbol: ROOTS[3], args: [1, 0] }, // full load, nothing kept from a previous level
  ] };
}

export function report(context) {
  const uploads = context.exports.spike_gs_upload_count?.() ?? 0;
  if (uploads) {
    const view = context.view();
    const formats = {};
    let pixels = 0;
    for (let index = 0; index < uploads; index += 1) {
      const at = context.exports.spike_gs_upload_address(index);
      const psm = view.getInt32(at + 8, true);
      formats[psm] = (formats[psm] ?? 0) + 1;
      if (psm === 0x13) pixels += view.getInt32(at + 20, true) * view.getInt32(at + 24, true);
    }
    console.log(`texture store: ${uploads} uploads recorded (${JSON.stringify(formats)} by GS format, ${pixels} indexed pixels), overflow ${context.exports.spike_gs_upload_overflow()}`);
  }
  for (const call of stats.decompress) {
    console.log(`fast_decompress: ${call.length} -> ${call.size} bytes at 0x${call.destination.toString(16)}`);
  }
  if (stats.collision) console.log(`collision mesh: ${stats.collision.bytes} bytes, ${stats.collision.cells} cells, ${stats.collision.faces} faces`);
  const unverified = context.exports.spike_collision_unverified?.() ?? 0;
  if (unverified) console.log(`NOT VERIFIED - ${unverified} collision queries passed a mask, ignored moby or option whose retail meaning is unknown; answered from the static world only`);
  console.log(`disc reads: ${stats.sectorReads} (${stats.sectorsRead} sectors)`);
  if (disc !== null) closeSync(disc);
}
