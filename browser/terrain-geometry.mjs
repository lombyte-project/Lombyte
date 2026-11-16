export function packedArrays(bytes, start, end, rows) {
  const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
  const arrays = [];
  let cursor = start;
  while (cursor + 4 <= end) {
    const word = view.getUint32(cursor, true);
    const command = (word >>> 24) & 0x7F;
    cursor += 4;
    if (command === 0x30 || command === 0x31) {
      if (cursor + 16 > end) throw new Error('Truncated terrain coordinate record.');
      if (command === 0x30) rows.value = Array.from({ length: 4 }, (_, index) => view.getInt32(cursor + index * 4, true));
      cursor += 16;
    } else if (command === 0x20) {
      cursor += 4;
    } else if ((command & 0x60) === 0x60) {
      const components = ((command >>> 2) & 3) + 1;
      const width = [4, 2, 1, 0][command & 3];
      const count = (word >>> 16) & 255 || 256;
      const length = count * components * width;
      if (!width || cursor + length > end) throw new Error('Invalid terrain packed array.');
      arrays.push({ format: command & 15, address: word & 0x3FF, unsigned: (word & 0x4000) !== 0,
        count, offset: cursor, length, origin: [...rows.value] });
      cursor += (length + 3) & ~3;
    } else if (![0, 1, 2, 3, 4, 5, 6, 7, 0x10, 0x11, 0x13, 0x14, 0x15, 0x17].includes(command)) {
      throw new Error(`Unsupported terrain data record 0x${command.toString(16)}.`);
    }
    if (cursor > end) throw new Error('Terrain data record crosses its fragment boundary.');
  }
  return arrays;
}

export function readTerrainFragments(planet) {
  const bytes = planet.core;
  const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
  const header = new DataView(planet.header.buffer, planet.header.byteOffset, planet.header.byteLength);
  const terrain = header.getUint32(8, true);
  if (terrain + 16 > bytes.length) throw new Error('Terrain header is outside the planet core.');
  const records = terrain + view.getUint32(terrain, true);
  const count = view.getUint32(terrain + 4, true);
  if (count > 10000 || records + count * 64 > bytes.length) throw new Error('Invalid terrain fragment table.');
  return Array.from({ length: count }, (_, index) => {
    const record = records + index * 64;
    const payload = records + view.getUint32(record + 16, true);
    const shared = payload + view.getUint16(record + 0x16, true);
    const medium = payload + view.getUint16(record + 0x18, true);
    const full = payload + view.getUint16(record + 0x1A, true);
    const colours = payload + view.getUint16(record + 0x1E, true);
    if (payload > shared || shared > medium || medium > full || full > colours || colours > bytes.length) {
      throw new Error('Invalid terrain fragment data ranges.');
    }
    const rows = { value: [0, 0, 0, 0] };
    const common = packedArrays(bytes, shared, medium, rows);
    const extra = packedArrays(bytes, medium, full, rows);
    const detail = packedArrays(bytes, full, colours, rows);
    return { index, record, payload, colours, common, extra, detail };
  });
}

export function readTerrainGeometry(planet) {
  const view = new DataView(planet.core.buffer, planet.core.byteOffset, planet.core.byteLength);
  const batches = new Map();
  const fragments = readTerrainFragments(planet);
  let triangles = 0;
  for (const fragment of fragments) {
    const header = fragment.common.find(array => array.format === 13 && array.unsigned);
    if (!header || header.length < 40) throw new Error('Missing terrain vertex layout.');
    const positionsAddress = view.getUint16(header.offset + 12, true);
    const infoAddress = view.getUint16(header.offset + 14, true);
    const indicesAddress = view.getUint16(header.offset + 26, true);
    const stripsAddress = view.getUint16(header.offset + 36, true);
    const positions = [];
    const infos = [];
    for (const array of [...fragment.common, ...fragment.extra, ...fragment.detail]) {
      if (array.format === 9 && !array.unsigned) {
        const first = (array.address - positionsAddress) / 2;
        if (!Number.isInteger(first) || first < 0 || first > 1024) throw new Error('Invalid terrain position address.');
        for (let index = 0; index < array.count; index++) {
          const offset = array.offset + index * 6;
          positions[first + index] = [0, 1, 2].map(axis =>
            (view.getInt16(offset + axis * 2, true) + array.origin[axis]) / 1024);
        }
      } else if (array.format === 13 && !array.unsigned) {
        const first = array.address - infoAddress;
        if (first < 0 || first > 1024) throw new Error('Invalid terrain vertex address.');
        for (let index = 0; index < array.count; index++) {
          const offset = array.offset + index * 8;
          const uv = [0, 1].map(axis => {
            const value = view.getInt16(offset + axis * 2, true);
            return value / (value < 0 ? 8192 : 4096);
          });
          infos[first + index] = { uv, position: (view.getInt16(offset + 6, true) + array.origin[3] - positionsAddress) / 2 };
        }
      }
    }
    const stripArray = fragment.detail.find(array => array.format === 14 && !array.unsigned && array.address === stripsAddress);
    const indexArray = fragment.detail.find(array => array.format === 14 && array.unsigned && array.address === indicesAddress);
    if (!stripArray || !indexArray) throw new Error('Missing full-detail terrain strips.');
    const texturePacket = fragment.payload + view.getUint16(fragment.record + 0x1C, true);
    const textureCount = planet.core[fragment.record + 0x28];
    let activeMaterial = -1;
    let cursor = 0;
    function emit(indices) {
      if (++triangles > 2000000) throw new Error('Terrain exceeds the triangle budget.');
      let batch = batches.get(activeMaterial);
      if (!batch) {
        batch = { material: activeMaterial, positions: [], colours: [], uvs: [] };
        batches.set(activeMaterial, batch);
      }
      for (const vertex of indices) {
        const info = infos[vertex];
        const position = info && positions[info.position];
        if (!position || !Number.isInteger(info.position)) throw new Error('Terrain triangle references an absent position.');
        const colour = fragment.colours + info.position * 4;
        if (colour + 4 > planet.core.length) throw new Error('Terrain vertex colour is out of range.');
        batch.positions.push(...position);
        batch.uvs.push(...info.uv);
        batch.colours.push(...[0, 1, 2].map(channel => Math.min(planet.core[colour + channel] / 128, 1)));
      }
    }
    for (let offset = stripArray.offset; offset < stripArray.offset + stripArray.length; offset += 4) {
      const signedCount = view.getInt8(offset);
      if (signedCount === 0) break;
      if (signedCount < 0 && view.getInt8(offset + 2) >= 0) {
        const slot = view.getInt8(offset + 2) / 5;
        if (!Number.isInteger(slot) || slot >= textureCount || texturePacket + slot * 80 + 4 > planet.core.length) {
          throw new Error('Invalid terrain material slot.');
        }
        activeMaterial = view.getInt32(texturePacket + slot * 80, true);
      }
      const count = signedCount < 0 ? signedCount + 128 : signedCount;
      if (activeMaterial < 0 || cursor + count > indexArray.length) throw new Error('Invalid terrain triangle strip.');
      for (let vertex = 2; vertex < count; vertex++) {
        const start = indexArray.offset + cursor + vertex;
        const triangle = [planet.core[start - 2], planet.core[start - 1], planet.core[start]];
        if (vertex % 2) [triangle[0], triangle[1]] = [triangle[1], triangle[0]];
        if (new Set(triangle).size === 3) emit(triangle);
      }
      cursor += count;
    }
  }
  return { fragments: fragments.length, triangles, batches: [...batches.values()].map(batch => ({
    material: batch.material, positions: new Float32Array(batch.positions),
    colours: new Float32Array(batch.colours), uvs: new Float32Array(batch.uvs),
  })) };
}

export function readTerrainTextures(planet) {
  return readIndexedTextures(planet, 48);
}

// Tie (scenery) textures: core header words 0x10 (count) and 0x11 (definitions), the table
// load_level_assets_and_gameplay packs into tie_packed_texture_refs.
export function readTieTextures(planet) {
  return readIndexedTextures(planet, 64);
}

// Shrub textures: core header words 0x12 (count) and 0x13 (definitions).
export function readShrubTextures(planet) {
  return readIndexedTextures(planet, 72);
}

export function readMobyTextures(planet) {
  return readIndexedTextures(planet, 56);
}

function readIndexedTextures(planet, tableOffset) {
  const header = new DataView(planet.header.buffer, planet.header.byteOffset, planet.header.byteLength);
  const count = header.getUint32(tableOffset, true);
  const definitions = header.getUint32(tableOffset + 4, true);
  const pixelsBase = header.getUint32(96, true);
  const uploadCount = header.getUint32(0, true);
  const uploads = header.getUint32(4, true);
  if (count > 1024 || definitions + count * 16 > header.byteLength ||
      uploadCount > 10000 || uploads + uploadCount * 16 > header.byteLength) throw new Error('Invalid terrain texture tables.');
  const palettes = new Map();
  let destination = 0;
  for (let index = 0; index < uploadCount; index++) {
    const record = uploads + index * 16;
    const format = header.getUint32(record, true);
    const size = header.getUint32(record + 4, true);
    const source = header.getUint32(record + 12, true);
    let length;
    if (format === 0 || format === 2) {
      length = format === 0 ? 1024 : 512;
      palettes.set(destination / 256, { source, format });
    } else if (format === 19) {
      length = Math.max((size & 65535) * (size >>> 16), 256);
    } else throw new Error('Unsupported terrain upload pixel format.');
    if (source + length > planet.textureImages.length) throw new Error('Terrain upload exceeds its source data.');
    destination += length;
  }
  return Array.from({ length: count }, (_, index) => {
    const record = definitions + index * 16;
    const pixels = pixelsBase + header.getUint32(record, true);
    const width = header.getUint16(record + 4, true);
    const height = header.getUint16(record + 6, true);
    const palette = palettes.get(header.getUint16(record + 10, true));
    if (!width || !height || width > 1024 || height > 1024 || pixels + width * height > planet.core.length || !palette) {
      throw new Error('Invalid terrain indexed texture.');
    }
    const rgba = new Uint8Array(width * height * 4);
    const clut = new DataView(planet.textureImages.buffer, planet.textureImages.byteOffset, planet.textureImages.byteLength);
    for (let pixel = 0; pixel < width * height; pixel++) {
      const colour = planet.core[pixels + pixel];
      const slot = (colour & 0xE7) | ((colour & 8) << 1) | ((colour & 16) >> 1);
      if (palette.format === 0) {
        const source = palette.source + slot * 4;
        rgba.set([clut.getUint8(source), clut.getUint8(source + 1), clut.getUint8(source + 2),
          Math.min(clut.getUint8(source + 3) * 2, 255)], pixel * 4);
      } else {
        const packed = clut.getUint16(palette.source + slot * 2, true);
        rgba.set([(packed & 31) * 255 / 31, ((packed >>> 5) & 31) * 255 / 31,
          ((packed >>> 10) & 31) * 255 / 31, packed & 0x8000 ? 255 : 0], pixel * 4);
      }
    }
    return { width, height, rgba };
  });
}