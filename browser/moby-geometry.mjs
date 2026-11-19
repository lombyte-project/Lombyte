import { packedArrays } from './terrain-geometry.mjs';

// The material of a batch with texture reference 0xFFFFFFFF (untextured; see the note where it
// is read).
export const UNTEXTURED_MATERIAL = -1;

export function readMobyClass(planet, classId = 0) {
  const header = new DataView(planet.header.buffer, planet.header.byteOffset, planet.header.byteLength);
  const view = new DataView(planet.core.buffer, planet.core.byteOffset, planet.core.byteLength);
  function range(offset, length) {
    if (!Number.isSafeInteger(offset) || offset < 0 || !Number.isSafeInteger(length) ||
        length < 0 || offset > view.byteLength || length > view.byteLength - offset) {
      throw new Error('Moby data is outside the planet core.');
    }
    return offset;
  }
  if (header.byteLength < 32) throw new Error('Missing planet moby class table.');
  const count = header.getUint32(24, true), table = header.getUint32(28, true);
  if (count > 2048 || table > header.byteLength || count * 32 > header.byteLength - table) {
    throw new Error('Invalid planet moby class table.');
  }
  for (let index = 0; index < count; index++) {
    const entry = table + index * 32;
    if (header.getUint32(entry + 4, true) !== classId) continue;
    const base = header.getUint32(entry, true);
    if (!base) throw new Error('Moby class has no planet model data.');
    range(base, 0x48);
    const packets = range(base + view.getUint32(base, true), 1);
    const scale = view.getFloat32(base + 0x24, true);
    if (!Number.isFinite(scale) || scale <= 0 || scale > 100) throw new Error('Invalid moby model scale.');
    return { base, view, range, packets, scale, highLodCount: view.getUint8(base + 4),
      jointCount: view.getUint8(base + 8),
      skeleton: range(base + view.getUint32(base + 0x14, true), 1),
      textures: Array.from(planet.header.subarray(entry + 16, entry + 32)) };
  }
  throw new Error(`Moby class ${classId} is absent from this planet.`);
}

export function readMobyGeometry(planet, classId = 0) {
  const model = readMobyClass(planet, classId);
  const { base, view, range } = model;
  range(model.packets, model.highLodCount * 16);
  const cache = new Map(), skinCache = new Map(), matrixPalette = new Map(), batches = new Map();
  function jointSkin(joint) {
    // Rigid classes (crates, orbs) have no skeleton; every vertex is model-space.
    if (model.jointCount === 0) return { joints: [0, 0, 0, 0], weights: [1, 0, 0, 0] };
    if (joint >= model.jointCount) throw new Error('Moby skin joint is outside its skeleton.');
    return { joints: [joint, 0, 0, 0], weights: [1, 0, 0, 0] };
  }
  function skinAt(address) {
    if (address % 4 || !matrixPalette.has(address)) throw new Error('Moby references an absent skin matrix.');
    return matrixPalette.get(address);
  }
  let triangles = 0, activeMaterial = 0;
  for (let packet = 0; packet < model.highLodCount; packet++) {
    const record = model.packets + packet * 16;
    const data = range(base + view.getUint32(record, true), view.getUint16(record + 4, true) * 16);
    const end = data + view.getUint16(record + 4, true) * 16;
    const arrays = packedArrays(planet.core, data, end, { value: [0, 0, 0, 0] });
    const uvArray = arrays.find(array => array.format === 5);
    const indexArray = arrays.find(array => array.format === 14);
    const textureArray = arrays.find(array => array.format === 12);
    if (!uvArray || !indexArray || indexArray.length < 4) throw new Error('Missing moby draw arrays.');
    const vertexHeader = range(base + view.getUint32(record + 8, true), 32);
    const transfers = view.getUint32(vertexHeader, true);
    const count = view.getUint32(vertexHeader + 4, true) + view.getUint32(vertexHeader + 8, true) +
      view.getUint32(vertexHeader + 12, true);
    const duplicates = view.getUint32(vertexHeader + 16, true);
    const total = view.getUint32(vertexHeader + 20, true);
    const vertexOffset = view.getUint32(vertexHeader + 24, true);
    const vertexEnd = view.getUint32(vertexHeader + 28, true);
    const epilogues = (vertexEnd - vertexOffset) / 16 - count;
    if (count > 128 || transfers > 128 || total !== count + duplicates || total !== uvArray.count ||
        total !== view.getUint8(record + 15) || epilogues < 1 || epilogues > 6 || !Number.isInteger(epilogues)) {
      throw new Error('Invalid moby vertex layout.');
    }
    const vertexStart = range(vertexHeader + vertexOffset, (count + epilogues) * 16);
    range(vertexHeader + 32, transfers * 2);
    for (let transfer = 0; transfer < transfers; transfer++) {
      const offset = vertexHeader + 32 + transfer * 2;
      const address = view.getUint8(offset + 1);
      if (address % 4) throw new Error('Unaligned moby skin matrix.');
      matrixPalette.set(address, jointSkin(view.getUint8(offset)));
    }
    const positions = [], skins = [];
    const twoWay = view.getUint32(vertexHeader + 4, true);
    const threeWay = view.getUint32(vertexHeader + 8, true);
    for (let index = 0; index < count; index++) {
      const vertex = vertexStart + index * 16;
      const joint = view.getUint16(vertex, true) >>> 9;
      let skin;
      if (index < twoWay + threeWay) {
        const three = index >= twoWay;
        if (!three) matrixPalette.set(view.getUint8(vertex + 6), jointSkin(joint));
        const addresses = [view.getUint8(vertex + 2), view.getUint8(vertex + 3)];
        if (three) addresses.push(joint * 2);
        const sources = addresses.map(skinAt);
        if (sources.some(source => source.weights[1] !== 0)) throw new Error('Nested moby skin blends are unsupported.');
        skin = { joints: [0, 0, 0, 0], weights: [0, 0, 0, 0] };
        let sum = 0;
        sources.forEach((source, part) => {
          skin.joints[part] = source.joints[0];
          skin.weights[part] = view.getUint8(vertex + 4 + part);
          sum += skin.weights[part];
        });
        if (!sum) throw new Error('Moby vertex has no skin weight.');
        skin.weights = skin.weights.map(weight => weight / sum);
        matrixPalette.set(view.getUint8(vertex + 7), skin);
      } else {
        matrixPalette.set(view.getUint8(vertex + 3), jointSkin(joint));
        skin = skinAt(view.getUint8(vertex + 2));
      }
      skins.push(skin);
      const position = [0, 1, 2].map(axis => view.getInt16(vertexStart + index * 16 + 10 + axis * 2, true) * model.scale / 1024);
      positions.push(position);
      const delayed = index + 7, last = count + epilogues - 1;
      const idAddress = delayed <= last ? vertexStart + delayed * 16
        : vertexStart + last * 16 + 4 + (delayed - last - 1) * 2;
      const id = view.getUint16(range(idAddress, 2), true) & 511;
      if (id !== 511) {
        cache.set(id, position);
        skinCache.set(id, skin);
      }
    }
    const duplicateStart = range((vertexHeader + 32 + transfers * 2 + 7) & ~7, duplicates * 2);
    for (let index = 0; index < duplicates; index++) {
      const id = view.getUint16(duplicateStart + index * 2, true) >>> 7;
      const position = cache.get(id);
      if (!position) throw new Error(`Absent moby cached vertex ${id} in packet ${packet}.`);
      positions.push(position);
      skins.push(skinCache.get(id));
    }
    let primitive = 0, secret = 0;
    const strip = [];
    const secrets = [view.getUint8(indexArray.offset + 2)];
    if (textureArray) {
      if (textureArray.length % 64) throw new Error('Invalid moby texture primitive array.');
      for (let offset = textureArray.offset; offset < textureArray.offset + textureArray.length; offset += 64) {
        for (const field of [12, 28, 44, 60]) secrets.push(view.getUint32(offset + field, true) & 255);
      }
    }
    let drawEnd = indexArray.offset + indexArray.length;
    while (drawEnd > indexArray.offset + 4 && view.getUint8(drawEnd - 1) === 0) drawEnd--;
    if (drawEnd >= indexArray.offset + 7 && [1, 2, 3].every(tail => view.getUint8(drawEnd - tail) === 1)) drawEnd -= 3;
    for (let offset = indexArray.offset + 4; offset < drawEnd; offset++) {
      let raw = view.getUint8(offset), restart = false;
      if (!raw) {
        if (planet.core.subarray(offset, indexArray.offset + indexArray.length).every(value => value === 0)) break;
        if (!textureArray || primitive * 64 >= textureArray.length || secret >= secrets.length) {
          throw new Error('Moby draw index references an absent texture primitive.');
        }
        const material = view.getUint32(textureArray.offset + primitive * 64 + 32, true);
        // 0xFFFFFFFF is the untextured material: prepare_resident_class_render_data passes a
        // negative index through, and set_up_vis_gif_viewer / build_indexed_resident_render_packet
        // then bind the fixed 8x8 texture at GS block 0x3FFB, which init_once fills with
        // 0x80808080 (neutral grey, opaque: x1.0 under modulate). The vertex colour and blending that
        // shade such parts are not decoded; game.mjs leaves those batches out.
        if (material !== 0xFFFFFFFF && material >= 16) throw new Error(`Unsupported moby texture reference ${material}.`);
        activeMaterial = material === 0xFFFFFFFF ? UNTEXTURED_MATERIAL : material;
        primitive++;
        raw = secrets[secret++];
        restart = true;
      }
      const index = (raw & 127) - 1;
      if (index < 0 || index >= positions.length) throw new Error('Moby draw index is outside its vertex table.');
      strip.push(index);
      if (strip.length < 3 || raw & 128 || restart) continue;
      const tail = strip.length - 1;
      const indices = tail % 2 ? [strip[tail - 1], strip[tail - 2], strip[tail]]
        : [strip[tail - 2], strip[tail - 1], strip[tail]];
      if (new Set(indices).size < 3) continue;
      if (++triangles > 200000) throw new Error('Moby exceeds the triangle budget.');
      let batch = batches.get(activeMaterial);
      if (!batch) {
        batch = { material: activeMaterial, positions: [], uvs: [], joints: [], weights: [] };
        batches.set(activeMaterial, batch);
      }
      for (const vertex of indices) {
        batch.positions.push(...positions[vertex]);
        batch.joints.push(...skins[vertex].joints);
        batch.weights.push(...skins[vertex].weights);
        batch.uvs.push(view.getInt16(uvArray.offset + vertex * 4, true) / 4096,
          view.getInt16(uvArray.offset + vertex * 4 + 2, true) / 4096);
      }
    }
  }
  // scale: the class scale (class + 0x24) already applied to the positions.
  return { triangles, packets: model.highLodCount, textures: model.textures, scale: model.scale,
    batches: [...batches.values()].map(batch => ({ ...batch,
      positions: new Float32Array(batch.positions), uvs: new Float32Array(batch.uvs),
      joints: new Uint16Array(batch.joints), weights: new Float32Array(batch.weights) })) };
}