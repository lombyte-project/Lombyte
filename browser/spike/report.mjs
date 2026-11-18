// Whole-tree spike report. Reads the objects left by compile-all.sh and answers, for one link set
// (boot code + shared overlay code + one level overlay): what is defined, what is undefined, and
// who would have to supply each undefined symbol. Analysis only; nothing here is linked or run.
//
// Usage: node browser/spike/report.mjs [level=l00] [--json build/spike/report.json]
import { execFileSync } from 'node:child_process';
import { readFileSync, readdirSync, writeFileSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = join(dirname(fileURLToPath(import.meta.url)), '..', '..');
const out = join(root, 'build', 'spike');
const level = process.argv[2] && !process.argv[2].startsWith('--') ? process.argv[2] : 'l00';
const jsonIndex = process.argv.indexOf('--json');
const llvmRoot = execFileSync('em-config', ['LLVM_ROOT'], { encoding: 'utf8' }).trim();

const inLinkSet = (name) => !name.startsWith('overlays__')
  || name.startsWith('overlays__shared__')
  || name.startsWith(`overlays__${level}__`);

const objects = readdirSync(join(out, 'obj')).filter((file) => file.endsWith('.o') && inLinkSet(file));
const failed = readdirSync(join(out, 'err')).filter((file) => file.endsWith('.txt') && inLinkSet(file));

// llvm-nm in chunks to stay under the argument limit.
const defined = new Map(); // symbol -> [object]
const definedData = new Map(); // data symbol -> [object]
const undefinedBy = new Map(); // symbol -> Set(object)
for (let start = 0; start < objects.length; start += 200) {
  const chunk = objects.slice(start, start + 200).map((file) => join(out, 'obj', file));
  const text = execFileSync(join(llvmRoot, 'llvm-nm'), ['-A', '--no-sort', ...chunk],
    { encoding: 'utf8', maxBuffer: 1 << 28 });
  for (const line of text.split('\n')) {
    const match = line.match(/\/([^/]+)\.o:\s*([0-9a-f]*)\s+([A-Za-z])\s+(\S+)$/);
    if (!match) continue;
    const [, object, , kind, symbol] = match;
    if (kind === 'U') {
      if (!undefinedBy.has(symbol)) undefinedBy.set(symbol, new Set());
      undefinedBy.get(symbol).add(object);
    } else if (kind === kind.toUpperCase()) { // global definitions only
      if (!defined.has(symbol)) defined.set(symbol, []);
      defined.get(symbol).push(object);
      if ('DBR'.includes(kind)) {
        if (!definedData.has(symbol)) definedData.set(symbol, []);
        definedData.get(symbol).push(object);
      }
    }
  }
}

// Undefined symbols referenced only as functions (`F *UND*` in the symbol table). An
// address-named function with no C body (`D_0012A5D8`, called as csc_dma_chunk_handler) must stay
// an import that traps by name, not be pinned as data in the RAM image.
const undefinedAsData = new Set();
const undefinedAsFunction = new Set();
for (let start = 0; start < objects.length; start += 200) {
  const chunk = objects.slice(start, start + 200).map((file) => join(out, 'obj', file));
  const text = execFileSync(join(llvmRoot, 'llvm-objdump'), ['-t', ...chunk], { encoding: 'utf8', maxBuffer: 1 << 28 });
  for (const line of text.split('\n')) {
    const match = line.match(/\s([FO])\s+\*UND\*\s+[0-9a-f]+\s+(\S+)$/);
    if (match) (match[1] === 'F' ? undefinedAsFunction : undefinedAsData).add(match[2]);
  }
}

// Objects whose data holds function pointers (class tables, callback tables). Their data must
// stay a C definition: the linker fills in WebAssembly table indices, where the retail bytes hold
// PS2 code addresses. Every other address-named global can be pinned in the RAM image and take
// its initial bytes from the retail ELF and level program.
const functionDataObjects = new Set();
for (let start = 0; start < objects.length; start += 200) {
  const chunk = objects.slice(start, start + 200).map((file) => join(out, 'obj', file));
  const text = execFileSync(join(llvmRoot, 'llvm-objdump'), ['-r', ...chunk], { encoding: 'utf8', maxBuffer: 1 << 28 });
  let object = null;
  let inData = false;
  for (const line of text.split('\n')) {
    const file = line.match(/\/([^/]+)\.o:\s+file format/);
    if (file) { object = file[1]; inData = false; continue; }
    const section = line.match(/^RELOCATION RECORDS FOR \[(\w+)\]/);
    if (section) { inData = section[1] === 'DATA'; continue; }
    if (inData && line.includes('R_WASM_TABLE_INDEX')) functionDataObjects.add(object);
  }
}
const ADDRESS_NAMED = /^(?:D|jtbl|jpt|lbl|RO|B)_(?:L\d\d_)?[0-9A-Fa-f]{6,8}/;
const pinnedData = [...definedData.entries()]
  .filter(([symbol, where]) => ADDRESS_NAMED.test(symbol) && where.every((object) => !functionDataObjects.has(object)))
  .map(([symbol]) => symbol);
const keptInC = [...definedData.keys()].filter((symbol) => ADDRESS_NAMED.test(symbol)).length - pinnedData.length;

// What the files that did not compile would have defined. INCLUDE_ASM names are exact; column-0
// function definitions in inline-asm files are a regex approximation.
const fromFailed = new Map(); // symbol -> { file, how }
for (const file of failed) {
  const source = join(root, 'src', `${file.replace(/\.txt$/, '').replaceAll('__', '/')}.c`);
  let text;
  try { text = readFileSync(source, 'utf8'); } catch { continue; }
  const short = source.slice(root.length + 1);
  for (const match of text.matchAll(/INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\)/g)) {
    fromFailed.set(match[1], { file: short, how: 'assembly-only (INCLUDE_ASM)' });
  }
  for (const match of text.matchAll(/^[A-Za-z_][\w \t*]*?\b(\w+)\s*\([^;{]*\)\s*\{/gm)) {
    if (!fromFailed.has(match[1])) fromFailed.set(match[1], { file: short, how: 'C with inline MIPS asm' });
  }
}

const classify = (symbol) => {
  if (fromFailed.has(symbol)) return fromFailed.get(symbol).how;
  if (/^(sce|Sif|sif|_?sce|pad[A-Z]|mc[A-Z]|mpeg|ipu|Vif|Gif|Vu0|Vu1|DMA|Dma)/.test(symbol)) return 'PS2 SDK / hardware service';
  if (undefinedAsFunction.has(symbol) && !undefinedAsData.has(symbol)) return 'address-named function without C';
  if (/^(D|jtbl|jpt|lbl|RO|B)_(L\d\d_)?[0-9A-Fa-f]{6,8}/.test(symbol)) return 'fixed-address data not defined in C';
  if (/^(FUN|func)_(L\d\d_)?[0-9A-Fa-f]{6,8}/.test(symbol)) return 'unnamed function with no compiled definition';
  if (/^(mem(cpy|set|move|cmp)|str\w+|s?printf|v?sn?printf|malloc|free|rand|srand|abs|sinf?|cosf?|sqrtf?|atan2?f?|floorf?|ceilf?|fabsf?|fmodf?|powf?|exit|__\w+)$/.test(symbol)) return 'C library / compiler runtime';
  return 'named symbol with no compiled definition';
};

const undefinedSymbols = [...undefinedBy.keys()].filter((symbol) => !defined.has(symbol));
const groups = new Map();
for (const symbol of undefinedSymbols) {
  const group = classify(symbol);
  if (!groups.has(group)) groups.set(group, []);
  groups.get(group).push({ symbol, users: undefinedBy.get(symbol).size, file: fromFailed.get(symbol)?.file });
}
for (const list of groups.values()) list.sort((a, b) => b.users - a.users);

const duplicates = [...defined.entries()].filter(([, where]) => where.length > 1);

console.log(`link set: boot + shared overlay + ${level}`);
console.log(`objects compiled: ${objects.length}; files that failed to compile: ${failed.length}`);
console.log(`globally defined symbols: ${defined.size}`);
console.log(`symbols defined in more than one object: ${duplicates.length}`);
console.log(`undefined symbols: ${undefinedSymbols.length}`);
console.log(`C-defined address-named globals: ${pinnedData.length} can be pinned at their address; ${keptInC} stay C definitions (${functionDataObjects.size} objects hold function pointers in data)`);
for (const [group, list] of [...groups.entries()].sort((a, b) => b[1].length - a[1].length)) {
  console.log(`\n  ${list.length}  ${group}`);
  for (const entry of list.slice(0, 12)) {
    console.log(`      ${String(entry.users).padStart(4)} users  ${entry.symbol}${entry.file ? `  <- ${entry.file}` : ''}`);
  }
}

if (jsonIndex > 0) {
  writeFileSync(process.argv[jsonIndex + 1], JSON.stringify({
    level, objects: objects.length, failed, defined: defined.size, pinnedData,
    duplicates: duplicates.map(([symbol, where]) => ({ symbol, where })),
    undefined: Object.fromEntries(groups),
  }, null, 1));
}
