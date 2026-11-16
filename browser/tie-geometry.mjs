// Static scenery ("ties") from the level core: the rock walls, cliffs and structures that make up
// most of what a level looks like. This is a format decoder for the browser renderer; the PS2
// renderer feeds the same packets to VU1 microcode, which is not ported.
//
// Layout, from the recovered loader and class struct plus the disc data:
//   core header word 8 = class count, word 9 = table of 0x20-byte entries
//     { class data offset, class id, ..., +0x10: sixteen texture indices, one per material record }
//     (src/overlays/shared/world/load_level_gameplay.c registers them from exactly these fields)
//   class data (ObjectRenderClass, include/rnc/rendering/object_render_class.h):
//     +0x00 three level-of-detail streams (offsets), +0x20 their packet counts,
//     +0x23 material record count, +0x40 the scale the loader copies into each instance
//   stream = 16-byte packet headers: { +0 packet offset, +4 material count, +8 vertex offset and
//     +9 vertex size, both in 16-byte units }
//   packet: +0x00 output offset of each material, +0x10 which material record (offset / 0x50),
//     +0x20 a 12-byte header whose byte 3 is the strip count, then from +0x2C
//     4 bytes per strip { vertex count, _, output offset of the strip's tag, _ }
//   vertices: 16-byte records { x, y, z, slot, s, t, q, slot2 } followed by 24-byte records
//     { _, _, _, slot, x, y, z, 0, s, t, q, slot2 }. A slot is where the vertex goes in the output:
//     strip tag offset + 1 + 3 * position in the strip. A vertex can fill two slots; a second slot
//     equal to the first is a repeat, not another vertex.
// The count of 16-byte records is not read from a header field (its meaning is not established);
// it is the one split for which every record is well formed and every strip slot is filled once.
//
// Not decoded: the two lower levels of detail, per-vertex normals, and the baked lighting
// (light_ties computes vertex colours from the level's directional lights). Ties are therefore
// drawn with their textures at uniform brightness.

const CLASS_ENTRY_BYTES = 0x20;
const PACKET_HEADER_BYTES = 16;
const MATERIAL_RECORD_BYTES = 0x50;
const SMALL_VERTEX_BYTES = 16;
const LARGE_VERTEX_BYTES = 24;
const UNIT_Q = 4096;

function decodePacket(core, view, streamBase, headerAt, textureMap, classId) {
  const fail = (why) => { throw new Error(`Tie class ${classId}, packet header 0x${headerAt.toString(16)}: ${why}`); };
  const packet = streamBase + view.getUint32(headerAt, true);
  const materialCount = view.getUint8(headerAt + 4);
  const vertexStart = packet + view.getUint8(headerAt + 8) * 16;
  const vertexBytes = view.getUint8(headerAt + 9) * 16;
  if (materialCount === 0 || materialCount > 4) fail(`unexpected material count ${materialCount}`);
  if (vertexStart + vertexBytes > core.length) fail('vertex data is outside the level core');

  const materials = Array.from({ length: materialCount }, (_, index) => {
    const record = view.getUint32(packet + 0x10 + index * 4, true);
    if (record % MATERIAL_RECORD_BYTES !== 0 || record / MATERIAL_RECORD_BYTES >= textureMap.length) fail('bad material record');
    const texture = textureMap[record / MATERIAL_RECORD_BYTES];
    if (texture === 0xFF) fail('material record has no texture');
    return { output: view.getUint32(packet + index * 4, true), texture };
  });
  const stripCount = view.getUint8(packet + 0x20 + 3);
  const stripsAt = packet + 0x2C;
  if (stripCount === 0 || stripsAt + stripCount * 4 > vertexStart) fail('strip table overruns the vertex data');
  const strips = Array.from({ length: stripCount }, (_, index) => {
    const count = view.getUint8(stripsAt + index * 4);
    const tag = view.getUint8(stripsAt + index * 4 + 2);
    if (count < 3) fail('strip with fewer than three vertices');
    const owner = materials.filter(material => material.output <= tag).sort((a, b) => b.output - a.output)[0];
    if (!owner) fail('strip precedes every material');
    return { count, tag, texture: owner.texture, slots: new Array(count).fill(null) };
  });

  const place = (slot, vertex) => {
    for (const strip of strips) {
      const position = (slot - strip.tag - 1) / 3;
      if (Number.isInteger(position) && position >= 0 && position < strip.count) {
        if (strip.slots[position]) return false;
        strip.slots[position] = vertex;
        return true;
      }
    }
    return false;
  };
  const read = (at, positionAt, textureAt, slotAt, slot2At) => ({
    position: [0, 2, 4].map(offset => view.getInt16(at + positionAt + offset, true)),
    uv: [view.getInt16(at + textureAt, true) / UNIT_Q, view.getInt16(at + textureAt + 2, true) / UNIT_Q],
    q: view.getInt16(at + textureAt + 4, true),
    slots: [view.getUint16(at + slotAt, true), view.getUint16(at + slot2At, true)],
  });

  // Find the split between 16-byte and 24-byte records.
  let solved = null;
  for (let small = Math.floor(vertexBytes / SMALL_VERTEX_BYTES); small >= 0; small -= 1) {
    // The region is padded to 16 bytes, so up to 16 bytes may follow the last 24-byte record.
    const large = Math.floor((vertexBytes - small * SMALL_VERTEX_BYTES) / LARGE_VERTEX_BYTES);
    for (const strip of strips) strip.slots.fill(null);
    let ok = true;
    const vertices = [];
    for (let index = 0; ok && index < small; index += 1) vertices.push(read(vertexStart + index * 16, 0, 8, 6, 14));
    for (let index = 0; ok && index < large; index += 1) {
      const at = vertexStart + small * SMALL_VERTEX_BYTES + index * LARGE_VERTEX_BYTES;
      if (view.getUint16(at + 14, true) !== 0) ok = false;
      else vertices.push(read(at, 8, 16, 6, 22));
    }
    for (const vertex of vertices) {
      if (!ok) break;
      if (vertex.q !== UNIT_Q || !place(vertex.slots[0], vertex)) ok = false;
      else if (vertex.slots[1] !== 0 && vertex.slots[1] !== vertex.slots[0] && !place(vertex.slots[1], vertex)) ok = false;
    }
    if (ok && strips.every(strip => strip.slots.every(Boolean))) {
      if (solved) fail('vertex layout is ambiguous');
      solved = strips.map(strip => ({ texture: strip.texture, vertices: strip.slots.slice() }));
    }
  }
  if (!solved) fail('no vertex layout fills every strip slot exactly once');
  return solved;
}

// Returns Map(classId -> { scale, batches: [{ texture, positions, uvs }], triangles, packets }).
// Positions are in class units (stored value / 1024); multiply by `scale` and the instance matrix.
// A packet this decoder cannot verify is left out and described in `failures` (an array the
// caller passes in); without one, the first such packet throws.
export function readTieClasses(planet, failures) {
  const header = new DataView(planet.header.buffer, planet.header.byteOffset, planet.header.byteLength);
  const core = planet.core;
  const view = new DataView(core.buffer, core.byteOffset, core.byteLength);
  const count = header.getUint32(8 * 4, true);
  const table = header.getUint32(9 * 4, true);
  if (count > 512 || table + count * CLASS_ENTRY_BYTES > header.byteLength) throw new Error('Invalid tie class table.');
  const classes = new Map();
  for (let index = 0; index < count; index += 1) {
    const entry = table + index * CLASS_ENTRY_BYTES;
    const data = header.getUint32(entry, true);
    const classId = header.getInt32(entry + 4, true);
    if (data + 0x50 > core.length) throw new Error(`Tie class ${classId} is outside the level core.`);
    const textureMap = [...planet.header.subarray(entry + 0x10, entry + 0x20)];
    const streamBase = data + view.getUint32(data, true);
    const packetCount = view.getUint8(data + 0x20);
    const scale = view.getFloat32(data + 0x40, true);
    if (!(scale > 0 && scale < 1000)) throw new Error(`Tie class ${classId} has an implausible scale.`);
    if (streamBase + packetCount * PACKET_HEADER_BYTES > core.length) throw new Error(`Tie class ${classId} stream is outside the level core.`);
    const byTexture = new Map();
    let triangles = 0;
    let decoded = 0;
    for (let packet = 0; packet < packetCount; packet += 1) {
      let strips;
      try {
        strips = decodePacket(core, view, streamBase, streamBase + packet * PACKET_HEADER_BYTES, textureMap, classId);
      } catch (error) {
        if (!failures) throw error;
        failures.push(error.message);
        continue;
      }
      decoded += 1;
      for (const strip of strips) {
        if (!byTexture.has(strip.texture)) byTexture.set(strip.texture, { positions: [], uvs: [] });
        const batch = byTexture.get(strip.texture);
        for (let corner = 2; corner < strip.vertices.length; corner += 1) {
          const order = corner % 2 === 0 ? [corner - 2, corner - 1, corner] : [corner - 1, corner - 2, corner];
          for (const at of order) {
            const vertex = strip.vertices[at];
            batch.positions.push(vertex.position[0] / 1024, vertex.position[1] / 1024, vertex.position[2] / 1024);
            batch.uvs.push(vertex.uv[0], vertex.uv[1]);
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

// Placed ties from the gameplay file: header word 0x34 -> { count, 12 bytes, then 0xE0-byte
// records }. A record is { class id, ..., +0x10: four rows of a 4x4 matrix, ... } as
// load_level_gameplay.c reads it. Returns [{ classId, matrix: Float32Array(16) }].
export function readTieInstances(gameplay) {
  const view = new DataView(gameplay.buffer, gameplay.byteOffset, gameplay.byteLength);
  const table = view.getUint32(0x34, true);
  if (table + 16 > gameplay.length) throw new Error('Tie instance table is outside the gameplay file.');
  const count = view.getInt32(table, true);
  if (count < 0 || count > 20000 || table + 16 + count * 0xE0 > gameplay.length) throw new Error('Invalid tie instance table.');
  return Array.from({ length: count }, (_, index) => {
    const record = table + 16 + index * 0xE0;
    const matrix = new Float32Array(16);
    for (let element = 0; element < 16; element += 1) matrix[element] = view.getFloat32(record + 0x10 + element * 4, true);
    return { classId: view.getInt32(record, true), matrix };
  });
}
