// Scenario for run-closure.mjs: the original load_level_gameplay(1) over Veldin's real gameplay
// archive from the local disc (RNC_ISO_PATH).
//
// What is real: the boot ELF image (pass --elf), the gameplay archive bytes read from the disc,
// and every line of the loader that executes. What is staged by hand, mirroring what earlier
// startup code would have done: the loader's two buffer pointers and level_root[1].
// The archive is decoded once with the existing native decoder (browser/wad.c); the loader's own
// fast_decompress call (assembly-only on the console) is served from that result.
import { openAsBlob } from 'node:fs';
import { readFile } from 'node:fs/promises';
import { IsoReader } from '../iso-reader.mjs';
import { initializeDiscTable } from '../disc-loader.mjs';
import { loadLevelGameplay, VELDIN_LEVEL } from '../gameplay-loader.mjs';

const LEVEL_LOAD = 0x00173E00; // D_L00_00173E00: +4 header buffer, +0x18 archive buffer
const LEVEL_ROOT = 0x0015EE4C; // level_root[]; [1] is the compressed gameplay archive
const HEADER_BUFFER = 0x01000000; // decoded gameplay lands here (free RAM above the image in use)
const ARCHIVE_BUFFER = 0x01400000;

let gameplay = null;
let decompressCalls = 0;

const load = (name) => readFile(new URL(`../../build/browser/${name}.wasm`, import.meta.url))
  .then((bytes) => WebAssembly.instantiate(bytes, {}))
  .then(({ instance }) => { instance.exports._initialize?.(); return instance.exports; });

export function services(context) {
  return {
    // fast_decompress(source, destination): serve the decode done in setup().
    FUN_L00_0024d8d0(source, destination) {
      if (source !== ARCHIVE_BUFFER || destination !== HEADER_BUFFER) {
        throw new Error(`fast_decompress(0x${source.toString(16)}, 0x${destination.toString(16)}) is not the staged gameplay archive`);
      }
      context.bytes().set(gameplay.bytes, destination);
      decompressCalls += 1;
      return gameplay.bytes.length;
    },
  };
}

// Renderer-only precomputation. The browser renderer does not read these results yet, so the
// loader is allowed past them; each is reported as NOT DONE on every run.
export const deferred = {
  FUN_00237370: 'light_ties: bakes directional-light vertex colours for the PS2 tie renderer (assembly-only)',
  FUN_0022a5e0: 'the same bake for shrub instances (assembly-only, unnamed)',
};

export async function setup(context) {
  const isoPath = process.env.RNC_ISO_PATH;
  if (!isoPath) throw new Error('Set RNC_ISO_PATH to the local USA disc image.');
  const [runtime, decoder] = await Promise.all([load('runtime'), load('wad')]);
  runtime.runtime_init();
  const reader = new IsoReader(await openAsBlob(isoPath));
  await reader.open();
  await initializeDiscTable(runtime, reader);
  gameplay = await loadLevelGameplay(runtime, reader, decoder, VELDIN_LEVEL);
  const view = context.view();
  view.setUint32(LEVEL_LOAD + 4, HEADER_BUFFER, true);
  view.setUint32(LEVEL_LOAD + 0x18, ARCHIVE_BUFFER, true);
  view.setUint32(LEVEL_ROOT + 4, ARCHIVE_BUFFER, true);
  console.log(`staged Veldin gameplay: ${gameplay.bytes.length} decoded bytes from sector ${gameplay.sector}, ${gameplay.mobys.records?.length ?? gameplay.mobys.length} placed records`);
  return { args: [1] }; // skip_gameplay_read: the archive is already in memory
}

export function report() {
  console.log(`fast_decompress served ${decompressCalls} time(s)`);
}
