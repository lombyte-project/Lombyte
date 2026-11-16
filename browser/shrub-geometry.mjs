// Small scenery ("shrubs") from the level core: plants, rocks and props that the PS2 draws with
// its shrub renderer. Like tie-geometry.mjs this is a format decoder for the browser renderer,
// not recovered code.
//
// Layout, from the recovered loader and class struct plus the disc data:
//   core header word 0xA = class count, word 0xB = table of 0x30-byte entries
//     { class data offset, class id, ..., +0x10: sixteen texture indices by material index }
//   class data (ShrubRenderClass, include/rnc/rendering/shrub_render_class.h):
//     +0x20 the scale the loader copies into each instance, +0x28 packet count,
//     +0x40 packet table of { offset, size }
//   packet = a VIF unpack list (the same commands terrain fragments use) with three arrays:
//     0: { texture count, tag count, vertex count, _ }, then per tag { 12-byte GIF tag whose low
//        15 bits are the vertex count, output offset }, then per texture four register words
//        { +0x0C output offset, +0x20 material index }
//     1: per vertex { x, y, z, output offset }
//     2: per vertex { s, t, q, n }; a negative n marks a vertex that starts a new strip
//   A vertex belongs to the tag at (output offset - tag offset - 1) / 3, in that order. Every tag
//   is a triangle strip. A packet is accepted only if every tag slot is filled exactly once.
//
// Not decoded: normals and lighting (n's low bits), the billboard used at a distance, and the
// per-instance colour. Shrubs are drawn with their textures at uniform brightness.
import { packedArrays } from './terrain-geometry.mjs';

const CLASS_ENTRY_BYTES = 0x30;
const UNIT = 4096;

function decodePacket(core, view, start, size, textureMap, classId) {
  const fail = (why) => { throw new Error(`Shrub class ${classId}, packet 0x${start.toString(16)}: ${why}`); };
  let arrays;
  try {
    arrays = packedArrays(core, start, start + size, { value: [0, 0, 0, 0] });
  } catch (error) {
    fail(error.message);
  }
  if (arrays.length !== 3) fail(`expected three arrays, found ${arrays.length}`);
  const [head, first, second] = arrays;
  const textureCount = view.getInt32(head.offset, true);
  const tagCount = view.getInt32(head.offset + 4, true);
  const vertexCount = view.getInt32(head.offset + 8, true);
  if (textureCount < 1 || textureCount > 8 || tagCount < 1 || tagCount > 64 ||
      head.length !== 16 + tagCount * 16 + textureCount * 64) fail('header counts do not match its size');
  if (first.count !== vertexCount || second.count !== vertexCount ||
      first.length !== vertexCount * 8 || second.length !== vertexCount * 8) fail('vertex arrays do not match the vertex count');

  const textures = Array.from({ length: textureCount }, (_, index) => {
    const at = head.offset + 16 + tagCount * 16 + index * 64;
    const material = view.getInt32(at + 0x20, true);
    if (material < 0 || material >= textureMap.length || textureMap[material] === 0xFF) fail('texture primitive names no texture');
    return { output: view.getInt32(at + 0x0C, true), texture: textureMap[material] };
  });
  const tags = Array.from({ length: tagCount }, (_, index) => {
    const at = head.offset + 16 + index * 16;
    const count = view.getUint32(at, true) & 0x7FFF;
    const output = view.getInt32(at + 12, true);
    if (((view.getUint32(at + 4, true) >>> 15) & 7) !== 4) fail('a tag is not a triangle strip');
    const owner = textures.filter(texture => texture.output <= output).sort((a, b) => b.output - a.output)[0];
    if (!owner || count < 3) fail('a tag has no texture or fewer than three vertices');
    return { count, output, texture: owner.texture, slots: new Array(count).fill(null) };
  });
  for (let index = 0; index < vertexCount; index += 1) {
    const a = first.offset + index * 8;
    const b = second.offset + index * 8;
    const output = view.getInt16(a + 6, true);
    if (view.getInt16(b + 4, true) !== UNIT) fail('a vertex has an unexpected q');
    const vertex = {
      position: [view.getInt16(a, true) / 1024, view.getInt16(a + 2, true) / 1024, view.getInt16(a + 4, true) / 1024],
      uv: [view.getInt16(b, true) / UNIT, view.getInt16(b + 2, true) / UNIT],
      restart: view.getInt16(b + 6, true) < 0,
    };
    const tag = tags.find(candidate => {
      const position = (output - candidate.output - 1) / 3;
      return Number.isInteger(position) && position >= 0 && position < candidate.count;
    });
    if (!tag) fail('a vertex is outside every tag');
    const position = (output - tag.output - 1) / 3;
    if (tag.slots[position]) fail('a tag slot is filled twice');
    tag.slots[position] = vertex;
  }
  if (!tags.every(tag => tag.slots.every(Boolean))) fail('a tag slot is empty');
  return tags.map(tag => ({ texture: tag.texture, vertices: tag.slots }));
}

// Returns Map(classId -> { scale, batches: [{ texture, positions, uvs }], triangles, packets }).
// Positions are in class units; multiply by `scale` and the instance matrix. A packet that does
// not verify is left out and described in `failures` if given, else it throws.
export function readShrubClasses(planet, failures, restartRule = 'new-strip') {
  const header = new DataView(planet.header.buffer, planet.header.byteOffset, planet.header.byteLength);
  const core = planet.core;
  const view = new DataView(core.buffer, core.byteOffset, core.byteLength);
  const count = header.getUint32(0xA * 4, true);
  const table = header.getUint32(0xB * 4, true);
  if (count > 512 || table + count * CLASS_ENTRY_BYTES > header.byteLength) throw new Error('Invalid shrub class table.');
  const classes = new Map();
  for (let index = 0; index < count; index += 1) {
    const entry = table + index * CLASS_ENTRY_BYTES;
    const data = header.getUint32(entry, true);
    const classId = header.getInt32(entry + 4, true);
    if (data + 0x40 > core.length) throw new Error(`Shrub class ${classId} is outside the level core.`);
    const textureMap = [...planet.header.subarray(entry + 0x10, entry + 0x20)];
    const scale = view.getFloat32(data + 0x20, true);
    const packetCount = view.getInt16(data + 0x28, true);
    if (!(scale > 0 && scale < 1000) || packetCount < 0 || packetCount > 1024 ||
        data + 0x40 + packetCount * 8 > core.length) throw new Error(`Shrub class ${classId} has an invalid header.`);
    const byTexture = new Map();
    let triangles = 0;
    let decoded = 0;
    for (let packet = 0; packet < packetCount; packet += 1) {
      const start = data + view.getUint32(data + 0x40 + packet * 8, true);
      const size = view.getInt32(data + 0x44 + packet * 8, true);
      let strips;
      try {
        if (size <= 0 || start + size > core.length) throw new Error(`Shrub class ${classId}: packet outside the level core`);
        strips = decodePacket(core, view, start, size, textureMap, classId);
      } catch (error) {
        if (!failures) throw error;
        failures.push(error.message);
        continue;
      }
      decoded += 1;
      for (const strip of strips) {
        if (!byTexture.has(strip.texture)) byTexture.set(strip.texture, { positions: [], uvs: [] });
        const batch = byTexture.get(strip.texture);
        let run = 0; // vertices since the last restart
        for (let corner = 0; corner < strip.vertices.length; corner += 1) {
          if (restartRule === 'new-strip') run = strip.vertices[corner].restart ? 1 : run + 1;
          else run += 1;
          if (run < 3 || (restartRule === 'skip-one' && strip.vertices[corner].restart)) continue;
          const order = run % 2 === 1 ? [corner - 2, corner - 1, corner] : [corner - 1, corner - 2, corner];
          for (const at of order) {
            const vertex = strip.vertices[at];
            batch.positions.push(...vertex.position);
            batch.uvs.push(...vertex.uv);
          }
          triangles += 1;
        }
      }
    }
    classes.set(classId, {
      scale,
      triangles,
      packets: { decoded, total: packetCount },
      batches: [...byTexture.entries()].map(([texture, batch]) => ({
        texture, positions: new Float32Array(batch.positions), uvs: new Float32Array(batch.uvs),
      })),
    });
  }
  return classes;
}

// Placed shrubs from the gameplay file: header word 0x3C -> { count, 12 bytes, then 0x70-byte
// records { class id, ..., +0x10: four rows of a 4x4 matrix, ... } } as load_level_gameplay.c
// reads them. Returns [{ classId, matrix: Float32Array(16) }].
export function readShrubInstances(gameplay) {
  const view = new DataView(gameplay.buffer, gameplay.byteOffset, gameplay.byteLength);
  const table = view.getUint32(0x3C, true);
  if (table + 16 > gameplay.length) throw new Error('Shrub instance table is outside the gameplay file.');
  const count = view.getInt32(table, true);
  if (count < 0 || count > 50000 || table + 16 + count * 0x70 > gameplay.length) throw new Error('Invalid shrub instance table.');
  return Array.from({ length: count }, (_, index) => {
    const record = table + 16 + index * 0x70;
    const matrix = new Float32Array(16);
    for (let element = 0; element < 16; element += 1) matrix[element] = view.getFloat32(record + 0x10 + element * 4, true);
    return { classId: view.getInt32(record, true), matrix };
  });
}
