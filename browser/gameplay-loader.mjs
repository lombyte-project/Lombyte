import { SECTOR_BYTES } from './iso-reader.mjs';

// Level 0 is Veldin in the original 19-entry extent table at D_0013A448.
export const VELDIN_LEVEL = 0;
// Header slot read by load_level_gameplay as disc_table.sound_archive (NTSC);
// slot 3 (sound_archive_alt) is the PAL variant. This port supports the USA disc.
export const NTSC_GAMEPLAY_SLOT = 2;
const LEVEL_HEADER_SECTORS = 5;
const GAMEPLAY_HEADER_BYTES = 0x60;
export const PLACED_MOBY_RECORD_BYTES = 0x78;

// Reads a level's gameplay archive the way the original startup does:
// read_file_entry_with_retry(level) fills D_0013A4E0 from the level extent,
// then the gameplay slot of that header is read and fast-decompressed.
export async function loadLevelGameplay(api, reader, decoder, level, signal) {
  if (api.runtime_disc_ready() !== 1) throw new Error('Initialize the disc table before loading gameplay.');
  const headerSector = api.runtime_disc_level_header_sector(level);
  if (headerSector < 0) throw new Error(`Level ${level} has no valid header extent.`);
  const header = await reader.readSectors(headerSector, LEVEL_HEADER_SECTORS, signal);
  signal?.throwIfAborted();
  if (header.length !== LEVEL_HEADER_SECTORS * SECTOR_BYTES) throw new Error('Incomplete level header read.');
  new Uint8Array(api.memory.buffer, api.runtime_disc_stage_address(), header.length).set(header);
  if (api.runtime_disc_level_header_load(level, header.length) !== 1) {
    throw new Error('Original level header reader failed.');
  }
  const sector = api.runtime_disc_level_slot_sector(NTSC_GAMEPLAY_SLOT);
  const count = api.runtime_disc_level_slot_count(NTSC_GAMEPLAY_SLOT);
  if (sector < 0 || count <= 0) throw new Error('Level header has no gameplay archive.');
  if (count * SECTOR_BYTES > decoder.wad_input_capacity()) throw new Error('Gameplay archive exceeds decoder capacity.');
  const compressed = await reader.readSectors(sector, count, signal);
  signal?.throwIfAborted();
  if (compressed[0] !== 0x57 || compressed[1] !== 0x41 || compressed[2] !== 0x44) {
    throw new Error('Gameplay archive is not a WAD stream.');
  }
  decoder.wad_reset();
  new Uint8Array(decoder.memory.buffer, decoder.wad_input_address(), compressed.length).set(compressed);
  const size = decoder.wad_decode(compressed.length, decoder.wad_output_capacity());
  if (size <= 0 || decoder.wad_ready() !== 1 || decoder.wad_guards_valid() !== 1) {
    decoder.wad_reset();
    throw new Error(`Gameplay archive decoding failed (${size}).`);
  }
  const bytes = new Uint8Array(decoder.memory.buffer, decoder.wad_output_address(), size).slice();
  decoder.wad_reset();
  return { level, sector, count, bytes, mobys: readPlacedMobys(bytes) };
}

// Field order follows the hdr->f44 loop in
// src/overlays/shared/world/load_level_gameplay.c. This only reads records;
// mission/save filtering and instance initialization stay in original C.
export function readPlacedMobys(bytes) {
  const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
  if (view.byteLength < GAMEPLAY_HEADER_BYTES) throw new Error('Gameplay header is truncated.');
  const table = view.getUint32(0x44, true);
  if (table > view.byteLength - 16) throw new Error('Placed moby table is outside the gameplay data.');
  const count = view.getInt32(table, true);
  const extraSlots = view.getInt32(table + 4, true);
  if (count < 0 || count > 4096 || extraSlots < 0 || extraSlots > 4096) {
    throw new Error('Invalid placed moby counts.');
  }
  const mobys = [];
  let offset = table + 16;
  for (let index = 0; index < count; index++) {
    if (offset > view.byteLength - PLACED_MOBY_RECORD_BYTES) throw new Error('Placed moby record is truncated.');
    const size = view.getInt32(offset, true);
    if (size < PLACED_MOBY_RECORD_BYTES || size % 4 || size > view.byteLength - offset) {
      throw new Error(`Invalid placed moby record size at index ${index}.`);
    }
    const f32 = at => view.getFloat32(offset + at, true);
    const i32 = at => view.getInt32(offset + at, true);
    const position = [f32(0x30), f32(0x34), f32(0x38)];
    const rotation = [f32(0x3C), f32(0x40), f32(0x44)];
    if (![...position, ...rotation, f32(0x1C)].every(Number.isFinite)) {
      throw new Error(`Placed moby ${index} has a non-finite transform.`);
    }
    mobys.push({
      index, mission: i32(0x04), flags: i32(0x08), uid: i32(0x0C),
      bolts: i32(0x10), bolts2: i32(0x14), classId: i32(0x18), scale: f32(0x1C),
      drawDistance: view.getUint16(offset + 0x20, true), updateDistance: view.getUint8(offset + 0x24),
      position, rotation, group: view.getUint8(offset + 0x48),
      snapToGround: i32(0x4C) !== 0, groundOffset: f32(0x50), pvarIndex: i32(0x58),
      color: [i32(0x64), i32(0x68), i32(0x6C)], light: i32(0x70), tableValue: i32(0x74),
    });
    offset += size;
  }
  return { count, extraSlots, end: offset, mobys };
}

// load_level_gameplay.c: "The moby of class 0 marks where the hero starts."
export function findHeroStart(placed) {
  const hero = placed.mobys.find(moby => moby.classId === 0);
  if (!hero) throw new Error('Gameplay data has no class-0 hero start.');
  return { position: hero.position, rotation: hero.rotation };
}

// Background colour and fog from the gameplay file's first block (header word 0), in the order
// load_level_gameplay.c reads it: background r, g, b; fog r, g, b; then fog near distance, far
// distance, near intensity and far intensity as floats. Distances are in 1/1024 world units.
// Intensity is how much of the scene still shows through: 255 is no fog.
export function readLevelFog(bytes) {
  const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
  const block = view.getUint32(0, true);
  if (block + 44 > bytes.length) throw new Error('Level settings block is outside the gameplay file.');
  const colour = at => [0, 1, 2].map(index => view.getInt32(block + (at + index) * 4, true) & 0xFF);
  const number = at => view.getFloat32(block + at * 4, true);
  return { background: colour(0), fog: colour(3), nearDistance: number(6) / 1024, farDistance: number(7) / 1024,
    nearIntensity: number(8), farIntensity: number(9) };
}
