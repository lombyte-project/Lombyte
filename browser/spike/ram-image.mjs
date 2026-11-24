// Generates build/spike/ram.s: one data segment standing in for EE RAM from 0x00100000, with a
// label at the original address of every fixed-address global the recovered C refers to but does
// not define. Linked first with --global-base=0x100000, each `D_XXXXXXXX` then sits at its PS2
// address in wasm linear memory, so original data can be copied in unchanged. wasm32 has no
// absolute symbols (the assembler rejects them), which is why this is a segment with labels.
//
// Usage: node browser/spike/report.mjs l00 --json build/spike/report-l00.json
//        node browser/spike/ram-image.mjs build/spike/report-l00.json
import { readFileSync, writeFileSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';

export const RAM_BASE = 0x00100000;
const RAM_LIMIT = 0x02000000; // 32 MiB of EE main memory

const root = join(dirname(fileURLToPath(import.meta.url)), '..', '..');
const report = JSON.parse(readFileSync(process.argv[2] ?? join(root, 'build/spike/report-l00.json'), 'utf8'));
// Globals no C file defines, plus C-defined ones that hold no function pointers. Pinning the
// second group too matters: boot code and level code name the same address differently
// (D_0015FF1C / D_L00_0015FF1C), and a C definition would be placed apart from the image, giving
// the two names separate storage. ram.o is linked first with --allow-multiple-definition, so its
// label wins and the C initializer is dropped; the retail ELF and level program data, copied in
// at run time, supply the initial bytes instead.
// Also the fixed-address labels that only the browser's own ports and services name
// (browser/spike/ee-helpers.c, services.c): `__asm__("D_...")` declarations there.
const portLabels = ['ee-helpers.c', 'services.c'].flatMap((file) =>
  [...readFileSync(join(root, 'browser/spike', file), 'utf8').matchAll(/__asm__\("(D_(?:L\d\d_)?[0-9A-F]{8})"\)/g)].map((m) => m[1]));
const symbols = [...new Set([
  ...(report.undefined['fixed-address data not defined in C'] ?? []).map((entry) => entry.symbol),
  ...(report.pinnedData ?? []),
  ...portLabels,
])];

// The level program keeps its own copy of the executable's data at other addresses
// (exe-data-map.py). A symbol named by its executable address (no level prefix) is pinned where
// the level keeps that object, so it is the same storage the level's `D_Lnn_` name refers to.
// An address between two mapped addresses that move by the same amount moves with them.
let moves = [];
// SPIKE_NO_EXE_MAP=1: the boot program's image (boot.wasm), which runs before a level program
// exists, so executable names keep their own addresses.
try {
  if (process.env.SPIKE_NO_EXE_MAP) throw new Error('boot program: no map');
  const mapping = JSON.parse(readFileSync(join(root, `build/spike/exe-data-${report.level}.json`), 'utf8'));
  moves = Object.entries(mapping).map(([from, to]) => [Number(from), Number(to)]).sort((a, b) => a[0] - b[0]);
} catch { /* no mapping generated: executable names keep their own addresses */ }
let moved = 0;
const levelAddress = (address) => {
  let low = 0;
  let high = moves.length - 1;
  while (low <= high) {
    const mid = (low + high) >> 1;
    if (moves[mid][0] === address) { moved += 1; return moves[mid][1]; }
    if (moves[mid][0] < address) low = mid + 1; else high = mid - 1;
  }
  const below = moves[high];
  const above = moves[low];
  if (below && above && below[1] - below[0] === above[1] - above[0]) { moved += 1; return address + (below[1] - below[0]); }
  return address;
};

const placed = [];
const outside = [];
for (const symbol of symbols) {
  const match = symbol.match(/^(?:D|jtbl|jpt|lbl|RO|B)_(L\d\d_)?([0-9A-Fa-f]{6,8})/);
  const named = match ? Number.parseInt(match[2], 16) : NaN;
  const address = match && !match[1] ? levelAddress(named) : named;
  if (address >= RAM_BASE && address < RAM_LIMIT) placed.push({ symbol, address });
  else outside.push(symbol);
}
placed.sort((a, b) => a.address - b.address);
const end = (placed.at(-1).address + 0x10000) & ~0xFFFF; // slack past the last label; sizes are unknown

// Named .rodata so the linker lays it out before every other segment (it orders .rodata, .data,
// .bss); wasm memory has no read-only pages, so the image is still writable.
const lines = ['\t.section\t.rodata.ee_ram,"",@', '\t.globl\tee_ram', '\t.type\tee_ram,@object', '\t.p2align\t4', 'ee_ram:'];
let cursor = RAM_BASE;
const sizes = [];
for (let index = 0; index < placed.length; index += 1) {
  const { symbol, address } = placed[index];
  if (address > cursor) lines.push(`\t.skip\t${address - cursor}`);
  cursor = address;
  // The assembler requires a size. The real one is unknown here, so each label runs to the next
  // distinct address; labels sharing an address (`_gp`, `_store` views) share that extent.
  let next = index + 1;
  while (next < placed.length && placed[next].address === address) next += 1;
  const size = (next < placed.length ? placed[next].address : end) - address;
  lines.push(`\t.globl\t${symbol}`, `\t.type\t${symbol},@object`, `${symbol}:`);
  sizes.push(`\t.size\t${symbol}, ${size}`);
}
lines.push(`\t.skip\t${end - cursor}`, `\t.size\tee_ram, ${end - RAM_BASE}`, ...sizes, '');
writeFileSync(join(root, process.env.SPIKE_RAM_OUT || 'build/spike/ram.s'), lines.join('\n'));
writeFileSync(join(root, 'build/spike/ram-outside.txt'), `${outside.join('\n')}\n`);
console.log(`ram.s: ${placed.length} labels, 0x${RAM_BASE.toString(16)}..0x${end.toString(16)} (${((end - RAM_BASE) / 1048576).toFixed(1)} MiB)`);
console.log(`executable-named globals pinned at their level address: ${moved}`);
console.log(`not placed (outside EE main memory): ${outside.length} -> build/spike/ram-outside.txt`);
