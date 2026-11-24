// The level's static world (terrain, ties, shrubs) drawn with WebGL for the game's world pass.
// On the console these are VU1 microprograms fed by tfrag_proc / tie_proc / shrub_proc; here
// services.c hands the pass to the host (spike_world_pass) and this module draws the same
// geometry, decoded from the level files (terrain-geometry.mjs, tie-geometry.mjs,
// shrub-geometry.mjs), with the game's own camera read from memory each frame. The picture goes
// into the GS frame buffer at that point of the frame (GsRenderer.blitWorld), behind what the game
// draws afterwards.
//
// The projection is the one the game's own quad routine uses (fast_draw_quad_real, ee-helpers.c),
// for a world point p:
//   p' = (p - camera) * G.z            camera = view block + 0x140, G = D_00160B20
//   v  = x*V0 + y*V1 + z*V2 + V3       V = view block + 0x40 .. 0x70 (rows)
//   pixel = v.xy * (B.x / v.w) + offset.xy - XYOFFSET / 16
//                                      B = D_00160B30, offset = screen block + 0x1A0
// v.w is negative in front of the camera. As a clip-space matrix (clip.w = -v.w):
//   clip.x = -(B/256) v.x - ((off.x - xo - 256)/256) v.w
//   clip.y =  (B/208) v.y + ((off.y - yo - 208)/208) v.w
//   clip.z =  standard perspective depth in d = -v.w (near / far below)
// Checked against the game: Veldin's terrain projected this way covers the ground the frame's
// fog and sky leave for it, around the GS-drawn dome (docs/build/native-browser-pipeline.md).
import * as THREE from 'three';
import { loadPlanetCore, VELDIN_ARCHIVE } from './level-loader.mjs';
import { readTerrainGeometry, readTerrainTextures, readShrubTextures, readTieTextures } from './terrain-geometry.mjs';
import { readShrubClasses, readShrubInstances } from './shrub-geometry.mjs';
import { readTieClasses, readTieInstances } from './tie-geometry.mjs';
import { loadLevelGameplay, VELDIN_LEVEL } from './gameplay-loader.mjs';
import { initializeDiscTable } from './disc-loader.mjs';
import { readMobyGeometry, UNTEXTURED_MATERIAL } from './moby-geometry.mjs';
import { readMobyAnimationRig } from './moby-animation.mjs';
import { readMobyTextures } from './terrain-geometry.mjs';

// Level 0 addresses of the executable's camera blocks (build/spike/exe-data-l00.json).
export const VELDIN_CAMERA = Object.freeze({ viewBlock: 0x166C80, screenBlock: 0x16CA40, g: 0x160BE0, b: 0x160BF0 });
// The level's moby table: [D_L00_0015FFD8, D_L00_0015FFDC), 0x100-byte records.
export const VELDIN_MOBYS = Object.freeze({ begin: 0x15FFD8, end: 0x15FFDC, stride: 0x100 });
const NEAR = 0.01, FAR = 400;

function texture(image) {
  const t = new THREE.DataTexture(image.rgba, image.width, image.height, THREE.RGBAFormat);
  t.wrapS = t.wrapT = THREE.RepeatWrapping;
  t.magFilter = THREE.LinearFilter;
  t.minFilter = THREE.LinearMipmapLinearFilter;
  t.generateMipmaps = true;
  t.needsUpdate = true;
  return t;
}

export class WorldRenderer {
  // Loads Veldin's world from the disc. modules: { runtime, wad, planetWad } (instantiated exports).
  static async load(reader, modules, canvas = null) {
    const world = new WorldRenderer(512, 416, canvas);
    const planet = await loadPlanetCore(reader, modules.planetWad, VELDIN_ARCHIVE);
    world.addTerrain(planet);
    world.planet = planet;
    world.mobyImages = readMobyTextures(planet);
    if (modules.runtime.runtime_disc_ready?.() !== 1) await initializeDiscTable(modules.runtime, reader, undefined, modules.wad);
    const gameplay = await loadLevelGameplay(modules.runtime, reader, modules.wad, VELDIN_LEVEL);
    world.addScenery(readTieClasses(planet, []), readTieInstances(gameplay.bytes), readTieTextures(planet));
    world.addScenery(readShrubClasses(planet, []), readShrubInstances(gameplay.bytes), readShrubTextures(planet));
    return world;
  }

  // width x height: the GS frame the projection maps to (512 x 416). With a display canvas the world
  // is drawn there at that canvas's own size (the page's resolution, antialiased); without one it
  // is drawn off screen at the GS size for render() to return.
  constructor(width, height, displayCanvas = null) {
    this.width = width; this.height = height;
    this.display = displayCanvas;
    const canvas = displayCanvas ?? (typeof OffscreenCanvas !== 'undefined' ? new OffscreenCanvas(width, height) : document.createElement('canvas'));
    this.renderer = new THREE.WebGLRenderer({ canvas, antialias: !!displayCanvas, alpha: true, premultipliedAlpha: false });
    this.renderer.outputColorSpace = THREE.LinearSRGBColorSpace; // GS values out as they are
    this.renderer.setClearColor(0x000000, 0);
    this.target = new THREE.WebGLRenderTarget(width, height, { depthBuffer: true });
    this.scene = new THREE.Scene();
    this.camera = new THREE.Camera();
    this.camera.matrixAutoUpdate = false;
    this.pixels = new Uint8Array(width * height * 4);
    this.triangles = 0;
  }

  material(image, vertexColors = false) {
    return new THREE.MeshBasicMaterial({ map: texture(image), vertexColors, side: THREE.DoubleSide, alphaTest: 0.1 });
  }

  addTerrain(planet) {
    const terrain = readTerrainGeometry(planet);
    const images = readTerrainTextures(planet);
    for (const batch of terrain.batches) {
      const image = images[batch.material];
      if (!image) continue;
      const geometry = new THREE.BufferGeometry();
      geometry.setAttribute('position', new THREE.BufferAttribute(batch.positions, 3));
      geometry.setAttribute('color', new THREE.BufferAttribute(batch.colours, 3));
      geometry.setAttribute('uv', new THREE.BufferAttribute(batch.uvs, 2));
      const mesh = new THREE.Mesh(geometry, this.material(image, true));
      mesh.frustumCulled = false;
      this.scene.add(mesh);
      this.triangles += batch.positions.length / 9;
    }
  }

  // Ties and shrubs: per class, instanced over the gameplay file's placements (as the old viewer
  // did: the instance rows with the class scale).
  addScenery(classes, instances, images) {
    const byClass = new Map();
    for (const instance of instances) {
      if (!byClass.has(instance.classId)) byClass.set(instance.classId, []);
      byClass.get(instance.classId).push(instance);
    }
    const materials = new Map();
    const matrix = new THREE.Matrix4(), scale = new THREE.Matrix4();
    for (const [classId, placed] of byClass) {
      const sceneryClass = classes.get(classId);
      if (!sceneryClass) continue;
      for (const batch of sceneryClass.batches) {
        const image = images[batch.texture];
        if (!image) continue;
        if (!materials.has(batch.texture)) materials.set(batch.texture, this.material(image));
        const geometry = new THREE.BufferGeometry();
        geometry.setAttribute('position', new THREE.BufferAttribute(batch.positions, 3));
        geometry.setAttribute('uv', new THREE.BufferAttribute(batch.uvs, 2));
        const mesh = new THREE.InstancedMesh(geometry, materials.get(batch.texture), placed.length);
        placed.forEach((instance, index) => {
          matrix.fromArray(instance.matrix);
          matrix.elements[3] = matrix.elements[7] = matrix.elements[11] = 0;
          matrix.elements[15] = 1;
          scale.makeScale(sceneryClass.scale, sceneryClass.scale, sceneryClass.scale);
          mesh.setMatrixAt(index, matrix.multiply(scale));
        });
        mesh.instanceMatrix.needsUpdate = true;
        mesh.frustumCulled = false;
        this.scene.add(mesh);
        this.triangles += batch.positions.length / 9 * placed.length;
      }
    }
  }

  // Mobys, from the game's own moby table each frame (see the header): class (+0xAA), state
  // (+0x20, < 0 = inactive), position (+0x10), scale (+0x2C), basis rows (+0xC0..+0xE0), and the
  // joint matrices the game's pose code computes (matrices(moby) -> joint count, matrix(j) ->
  // 16 floats). A skinned class's bone j gets
  //   position * basis * (scale / class scale) * [joint rotation rows | translation * class scale / 1024]
  // (decoded vertices carry the class scale / 1024; transform_scaled_vertex_batch applies
  // scale / 1024, the basis and the position to the game's joint-space points). Classes the
  // decoders cannot read, and untextured parts, are left out.
  syncMobys(view, matrices) {
    this.mobyClasses ??= new Map();
    this.mobyInstances ??= new Map();
    this.mobyGroup ??= (() => { const g = new THREE.Group(); this.scene.add(g); return g; })();
    const begin = view.getUint32(VELDIN_MOBYS.begin, true), end = view.getUint32(VELDIN_MOBYS.end, true);
    if (!begin || end <= begin || end - begin > 0x100000) return;
    const seen = new Set();
    for (let m = begin; m < end; m += VELDIN_MOBYS.stride) {
      if (view.getInt8(m + 0x20) < 0) continue;
      const classId = view.getUint16(m + 0xAA, true);
      const cls = this.mobyClass(classId);
      if (!cls) continue;
      const key = `${m}:${classId}`;
      let inst = this.mobyInstances.get(key);
      if (!inst) { inst = this.makeMobyInstance(cls); this.mobyInstances.set(key, inst); this.mobyGroup.add(inst.group); }
      seen.add(key);
      inst.group.visible = true;
      const f = (o) => view.getFloat32(m + o, true);
      const scale = f(0x2C);
      const world = new THREE.Matrix4().set(
        f(0xC0), f(0xD0), f(0xE0), f(0x10),
        f(0xC4), f(0xD4), f(0xE4), f(0x14),
        f(0xC8), f(0xD8), f(0xE8), f(0x18),
        0, 0, 0, 1);
      const ratio = scale / cls.scale;
      world.multiply(new THREE.Matrix4().makeScale(ratio, ratio, ratio));
      if (!inst.bones) { inst.group.matrix.copy(world); continue; }
      const joints = matrices.count(m);
      if (joints !== inst.bones.length) { inst.group.visible = false; continue; }
      const k = cls.scale / 1024;
      for (let j = 0; j < joints; j++) {
        const r = matrices.matrix(j);
        const local = new THREE.Matrix4().set(
          r[0], r[4], r[8], r[12] * k,
          r[1], r[5], r[9], r[13] * k,
          r[2], r[6], r[10], r[14] * k,
          0, 0, 0, 1);
        inst.bones[j].matrixWorld.multiplyMatrices(world, local);
      }
    }
    for (const [key, inst] of this.mobyInstances) if (!seen.has(key)) inst.group.visible = false;
  }

  mobyClass(classId) {
    if (this.mobyClasses.has(classId)) return this.mobyClasses.get(classId);
    let cls = null;
    try {
      const model = readMobyGeometry(this.planet, classId);
      let rig = null;
      try { rig = readMobyAnimationRig(this.planet, classId); } catch { rig = null; }
      const batches = model.batches.filter((b) => b.material !== UNTEXTURED_MATERIAL && this.mobyImages[model.textures[b.material]]);
      cls = { model, rig, scale: model.scale, batches, materials: new Map() };
    } catch { cls = null; }
    this.mobyClasses.set(classId, cls);
    return cls;
  }

  makeMobyInstance(cls) {
    const group = new THREE.Group();
    group.matrixAutoUpdate = false;
    let bones = null, skeleton = null;
    if (cls.rig) {
      bones = Array.from({ length: cls.rig.jointCount }, (_, j) => { const b = new THREE.Bone(); b.position.fromArray(cls.rig.bindPositions, j * 3); return b; });
      for (let j = 1; j < bones.length; j++) bones[cls.rig.parents[j]].add(bones[j]);
      bones[0].updateMatrixWorld(true);
      skeleton = new THREE.Skeleton(bones);
      skeleton.calculateInverses();
      for (const b of bones) { b.matrixAutoUpdate = false; b.matrixWorldAutoUpdate = false; }
    }
    for (const batch of cls.batches) {
      const textureIndex = cls.model.textures[batch.material];
      if (!cls.materials.has(textureIndex)) cls.materials.set(textureIndex, this.material(this.mobyImages[textureIndex]));
      const geometry = new THREE.BufferGeometry();
      geometry.setAttribute('position', new THREE.BufferAttribute(batch.positions, 3));
      geometry.setAttribute('uv', new THREE.BufferAttribute(batch.uvs, 2));
      let mesh;
      if (skeleton) {
        geometry.setAttribute('skinIndex', new THREE.BufferAttribute(batch.joints, 4));
        geometry.setAttribute('skinWeight', new THREE.BufferAttribute(batch.weights, 4));
        mesh = new THREE.SkinnedMesh(geometry, cls.materials.get(textureIndex));
        mesh.bind(skeleton, new THREE.Matrix4());
        mesh.matrixAutoUpdate = false;
      } else {
        mesh = new THREE.Mesh(geometry, cls.materials.get(textureIndex));
      }
      mesh.frustumCulled = false;
      group.add(mesh);
    }
    return { group, bones };
  }

  // The clip matrix for the game's current camera (see the header). view: DataView of EE memory.
  updateCamera(view, xyoffset, at = VELDIN_CAMERA) {
    const f = (address) => view.getFloat32(address, true);
    const V = Array.from({ length: 16 }, (_, i) => f(at.viewBlock + 0x40 + 4 * i));
    const c = [0, 1, 2].map((i) => f(at.viewBlock + 0x140 + 4 * i));
    const g = f(at.g + 8), B = f(at.b);
    const ox = f(at.screenBlock + 0x1A0), oy = f(at.screenBlock + 0x1A4);
    const xo = Number(xyoffset & 0xFFFFn) / 16, yo = Number((xyoffset >> 32n) & 0xFFFFn) / 16;
    // World-to-v rows: v_k = sum_j V[4j + k] * g * (p_j - c_j) + V[12 + k].
    const row = (k) => [g * V[k], g * V[4 + k], g * V[8 + k], V[12 + k] - g * (V[k] * c[0] + V[4 + k] * c[1] + V[8 + k] * c[2])];
    const vx = row(0), vy = row(1), vw = row(3);
    const kx = (ox - xo - this.width / 2) / (this.width / 2), ky = (oy - yo - this.height / 2) / (this.height / 2);
    const cx = vx.map((a, i) => -(B / (this.width / 2)) * a - kx * vw[i]);
    const cy = vy.map((a, i) => (B / (this.height / 2)) * a + ky * vw[i]);
    const zA = (FAR + NEAR) / (FAR - NEAR), zB = (2 * FAR * NEAR) / (FAR - NEAR);
    const cz = vw.map((a, i) => -zA * a - (i === 3 ? zB : 0));
    const cw = vw.map((a) => -a);
    const m = new THREE.Matrix4().set(...cx, ...cy, ...cz, ...cw);
    this.camera.projectionMatrix.copy(m);
    this.camera.projectionMatrixInverse.copy(m).invert();
  }

  // The world's depth in the GS's own units, at the GS size, for the game's Z buffer (gs-renderer
  // blitZ): RGBA rows bottom-up, the 24-bit GS depth in r | g << 8 | b << 16, a = 255 where world
  // geometry covers the pixel. The game's 3D effects compute XYZF z = B * v.z / v.w + offset.z
  // (fast_draw_quad_real: Q = D_00160B30.x / v.w, screen offset at screen block + 0x1A8), kept
  // modulo 2^24 by the Z24 buffer. The view's z column is a multiple Kz of its w column, so
  // v.z = Kz (v.w - V15) + V14 and z = B Kz + offset.z + B (V14 - Kz V15) / v.w: affine in 1 / v.w,
  // as WebGL's depth d = (zA + 1) / 2 + zB / (2 v.w) is. Each pixel's GS depth follows from d.
  gsDepth(view, at = VELDIN_CAMERA) {
    if (!this.depthTarget) {
      this.depthTarget = new THREE.WebGLRenderTarget(this.width, this.height, { depthBuffer: true });
      this.depthTarget.depthTexture = new THREE.DepthTexture(this.width, this.height, THREE.UnsignedIntType);
      this.packTarget = new THREE.WebGLRenderTarget(this.width, this.height, { depthBuffer: false });
      this.packMaterial = new THREE.ShaderMaterial({
        uniforms: { depth: { value: this.depthTarget.depthTexture } },
        vertexShader: 'varying vec2 at; void main() { at = uv; gl_Position = vec4(position.xy, 0.0, 1.0); }',
        fragmentShader: `precision highp float; uniform sampler2D depth; varying vec2 at;
          void main() {
            float d = texture2D(depth, at).x;
            if (d >= 1.0) { gl_FragColor = vec4(0.0); return; }
            float n = floor(d * 16777215.0 + 0.5);
            float hi = floor(n / 65536.0), mid = floor((n - hi * 65536.0) / 256.0), lo = n - hi * 65536.0 - mid * 256.0;
            gl_FragColor = vec4(lo / 255.0, mid / 255.0, hi / 255.0, 1.0);
          }`,
      });
      this.packScene = new THREE.Scene();
      const quad = new THREE.Mesh(new THREE.PlaneGeometry(2, 2), this.packMaterial);
      quad.frustumCulled = false; // a full-screen pass; the game's camera would cull it
      this.packScene.add(quad);
      this.depthPixels = new Uint8Array(this.width * this.height * 4);
    }
    // Rendered now, read back asynchronously (a synchronous readPixels waits for the whole GPU
    // frame); the depth returned is the last one that has arrived, at most a frame or two old,
    // converted with the camera it was rendered with.
    const f = (address) => view.getFloat32(address, true);
    const V = Array.from({ length: 16 }, (_, i) => f(at.viewBlock + 0x40 + 4 * i));
    const camera = { V, B: f(at.b), oz: f(at.screenBlock + 0x1A8) };
    if (!this.depthPending) {
      this.renderer.setRenderTarget(this.depthTarget);
      this.renderer.clear();
      this.renderer.render(this.scene, this.camera);
      this.renderer.setRenderTarget(this.packTarget);
      this.renderer.render(this.packScene, this.camera);
      this.renderer.setRenderTarget(null);
      const buffer = new Uint8Array(this.width * this.height * 4);
      this.depthPending = this.renderer.readRenderTargetPixelsAsync(this.packTarget, 0, 0, this.width, this.height, buffer)
        .then(() => { this.depthReady = { buffer, camera }; this.depthPending = null; }, () => { this.depthPending = null; });
    }
    if (!this.depthReady) return null;
    const ready = this.depthReady;
    this.depthReady = null;
    this.depthPixels = ready.buffer;
    return this.convertDepth(ready.camera);
  }

  convertDepth({ V, B, oz }) {
    const px = this.depthPixels;
    const zc = [V[2], V[6], V[10]], wc = [V[3], V[7], V[11]];
    const Kz = (zc[0] * wc[0] + zc[1] * wc[1] + zc[2] * wc[2]) / (wc[0] * wc[0] + wc[1] * wc[1] + wc[2] * wc[2]);
    const C0 = B * Kz + oz, C1 = B * (V[14] - Kz * V[15]);
    const zA = (FAR + NEAR) / (FAR - NEAR), zB = (2 * FAR * NEAR) / (FAR - NEAR);
    for (let i = 0; i < px.length; i += 4) {
      if (!px[i + 3]) continue;
      const d = (px[i] | px[i + 1] << 8 | px[i + 2] << 16) / 16777215;
      const inverseW = (2 * d - 1 - zA) / zB;
      let z = Math.round(C0 + C1 * inverseW) % 16777216;
      if (z < 0) z += 16777216;
      px[i] = z & 255; px[i + 1] = (z >> 8) & 255; px[i + 2] = (z >> 16) & 255;
    }
    return px;
  }

  // Draws the world for the game's current camera; returns RGBA rows bottom-up.
  // Draws the world into the display canvas at its current size (drawDisplay) ...
  drawDisplay(view, xyoffset, matrices = null) {
    const c = this.display;
    const w = Math.max(1, Math.round(c.clientWidth * (globalThis.devicePixelRatio || 1)));
    const h = Math.max(1, Math.round(c.clientHeight * (globalThis.devicePixelRatio || 1)));
    if (c.width !== w || c.height !== h) this.renderer.setSize(w, h, false);
    this.updateCamera(view, xyoffset);
    if (matrices && this.planet) this.syncMobys(view, matrices);
    this.renderer.setRenderTarget(null);
    this.renderer.clear();
    this.renderer.render(this.scene, this.camera);
  }

  // ... or off screen at the GS size, returning RGBA rows bottom-up.
  render(view, xyoffset, matrices = null) {
    this.updateCamera(view, xyoffset);
    if (matrices && this.planet) this.syncMobys(view, matrices);
    this.renderer.setRenderTarget(this.target);
    this.renderer.clear();
    this.renderer.render(this.scene, this.camera);
    this.renderer.readRenderTargetPixels(this.target, 0, 0, this.width, this.height, this.pixels);
    this.renderer.setRenderTarget(null);
    return this.pixels;
  }
}
