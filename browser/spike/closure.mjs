// Per-mission work list. From one root function, walk every call and data reference through the
// objects left by compile-all.sh and report what the root reaches: recovered C that already
// compiles, and each missing symbol with the reason it is missing. Analysis only.
//
// Usage: node browser/spike/closure.mjs <root symbol>[,<root symbol>...] [level=l00] [--list]
//   e.g. node browser/spike/closure.mjs FUN_L00_002d9dc8        (clank_cutscene_trigger_update)
// Function references are exact (per-function relocations). Data is coarser: reaching any data
// symbol of an object counts all of that object's data relocations, so tables over-report.
import { execFileSync } from 'node:child_process';
import { readFileSync, readdirSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = join(dirname(fileURLToPath(import.meta.url)), '..', '..');
const out = join(root, 'build', 'spike');
const args = process.argv.slice(2).filter((arg) => !arg.startsWith('--'));
const rootSymbol = args[0];
const level = args[1] ?? 'l00';
const listAll = process.argv.includes('--list');
if (!rootSymbol) {
  console.error('usage: node browser/spike/closure.mjs <root symbol> [level] [--list]');
  process.exit(2);
}
const llvmRoot = execFileSync('em-config', ['LLVM_ROOT'], { encoding: 'utf8' }).trim();
const inLinkSet = (name) => !name.startsWith('overlays__')
  || name.startsWith('overlays__shared__') || name.startsWith(`overlays__${level}__`);
const objects = readdirSync(join(out, 'obj')).filter((file) => file.endsWith('.o') && inLinkSet(file));
const failed = readdirSync(join(out, 'err')).filter((file) => file.endsWith('.txt') && inLinkSet(file));
const sourceOf = (object) => `src/${object.replace(/\.(o|txt)$/, '').replaceAll('__', '/')}.c`;

// node key: `${object}\0${symbol}` for functions, `${object}\0<data>` for an object's data.
const definedIn = new Map(); // symbol -> [{ object, data }]
const edges = new Map(); // node -> Set(symbol)
const functionNodes = new Set(); // every function body seen, file-local (static) ones included
const addEdge = (node, symbol) => {
  if (!edges.has(node)) edges.set(node, new Set());
  edges.get(node).add(symbol);
};
const define = (symbol, object, data) => {
  if (!definedIn.has(symbol)) definedIn.set(symbol, []);
  definedIn.get(symbol).push({ object, data });
};

// Functions at one address in one object (a name and its alias("...") names): objdump labels the
// body with one of them, so their edges are shared below.
const aliasGroups = new Map();
for (let start = 0; start < objects.length; start += 100) {
  const chunk = objects.slice(start, start + 100).map((file) => join(out, 'obj', file));
  const text = execFileSync(join(llvmRoot, 'llvm-objdump'), ['-d', '-r', '-t', '--no-show-raw-insn', ...chunk],
    { encoding: 'utf8', maxBuffer: 1 << 30 });
  let object = null;
  let node = null;
  for (const line of text.split('\n')) {
    let match;
    if ((match = line.match(/\/([^/]+\.o):\s+file format/))) {
      object = match[1]; node = null;
    } else if ((match = line.match(/^([0-9a-f]+) (g|w|l)\s+(F|O) (CODE|DATA)\s+[0-9a-f]+ (?:\.hidden )?(\S+)$/))) {
      if (match[2] !== 'l') define(match[5], object, match[3] === 'O');
      if (match[3] === 'F') {
        const key = `${object}\0${match[1]}`;
        if (!aliasGroups.has(key)) aliasGroups.set(key, []);
        aliasGroups.get(key).push(`${object}\0${match[5]}`);
      }
    } else if ((match = line.match(/^RELOCATION RECORDS FOR \[(\w+)\]/))) {
      node = match[1] === 'DATA' ? `${object}\0<data>` : null;
    } else if (/^Disassembly of section/.test(line)) {
      node = null;
    } else if ((match = line.match(/^[0-9a-f]+ <(\S+)>:$/))) {
      node = `${object}\0${match[1]}`;
      functionNodes.add(node);
    } else if (node && (match = line.match(/R_WASM_(\w+)\s+(\S+?)(?:[+-]\d+)?$/))) {
      if (!/^(TYPE_INDEX|GLOBAL_INDEX|SECTION_OFFSET|FUNCTION_OFFSET|TAG_INDEX)/.test(match[1])) addEdge(node, match[2]);
    }
  }
}

// With -d, llvm-objdump prints code relocations inline and omits the data section's records, so
// function pointers held in data (the level's handler tables) need a second, relocation-only pass.
for (let start = 0; start < objects.length; start += 100) {
  const chunk = objects.slice(start, start + 100).map((file) => join(out, 'obj', file));
  const text = execFileSync(join(llvmRoot, 'llvm-objdump'), ['-r', ...chunk], { encoding: 'utf8', maxBuffer: 1 << 30 });
  let object = null;
  let node = null;
  for (const line of text.split('\n')) {
    let match;
    if ((match = line.match(/\/([^/]+\.o):\s+file format/))) {
      object = match[1]; node = null;
    } else if ((match = line.match(/^RELOCATION RECORDS FOR \[(.+)\]/))) {
      node = match[1] === 'DATA' ? `${object}\0<data>` : null;
    } else if (node && (match = line.match(/R_WASM_(\w+)\s+(\S+?)(?:[+-]\d+)?$/))) {
      if (!/^(TYPE_INDEX|GLOBAL_INDEX|SECTION_OFFSET|FUNCTION_OFFSET|TAG_INDEX)/.test(match[1])) addEdge(node, match[2]);
    }
  }
}

for (const names of aliasGroups.values()) {
  if (names.length < 2) continue;
  const union = new Set(names.flatMap((name) => [...(edges.get(name) ?? [])]));
  for (const name of names) { edges.set(name, union); functionNodes.add(name); }
}

const fromFailed = new Map();
for (const file of failed) {
  let text;
  try { text = readFileSync(join(root, sourceOf(file)), 'utf8'); } catch { continue; }
  for (const match of text.matchAll(/INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\)/g)) {
    fromFailed.set(match[1], { file: sourceOf(file), how: 'assembly-only (INCLUDE_ASM)' });
  }
  for (const match of text.matchAll(/__asm__\("(\w+)"\)/g)) {
    if (!fromFailed.has(match[1])) fromFailed.set(match[1], { file: sourceOf(file), how: 'in a file with inline MIPS asm' });
  }
}
const classify = (symbol) => {
  if (fromFailed.has(symbol) && !/^D_/.test(symbol)) return fromFailed.get(symbol).how;
  if (/^(D|jtbl|jpt|lbl|RO|B)_(L\d\d_)?[0-9A-Fa-f]{6,8}/.test(symbol)) return 'fixed-address data not defined in C';
  if (/^(FUN|func)_(L\d\d_)?[0-9A-Fa-f]{6,8}/.test(symbol)) return 'function with no compiled definition';
  if (/^__/.test(symbol)) return null; // wasm linker-provided
  return 'named symbol with no compiled definition';
};

const reached = new Set(); // node keys
const missing = new Map(); // symbol -> Set(referencing source)
const queue = [];
const visitSymbol = (symbol, from) => {
  const where = definedIn.get(symbol);
  // A file-local function (static, such as the argument-reordering adapters normalize-sources.py
  // writes) has no global definition: follow it inside the object that refers to it.
  const local = from ? `${from.split('\0')[0]}\0${symbol}` : null;
  if (!where && local && functionNodes.has(local)) {
    if (!reached.has(local)) { reached.add(local); queue.push(local); }
    return;
  }
  if (!where) {
    if (classify(symbol) === null) return;
    if (!missing.has(symbol)) missing.set(symbol, new Set());
    if (from) missing.get(symbol).add(sourceOf(from.split('\0')[0]));
    return;
  }
  const { object, data } = where[0];
  const node = data ? `${object}\0<data>` : `${object}\0${symbol}`;
  if (reached.has(node)) return;
  reached.add(node);
  queue.push(node);
};
for (const one of rootSymbol.split(',')) {
  const before = reached.size;
  visitSymbol(one, null);
  if (reached.size === before && !definedIn.has(one)) {
    const why = fromFailed.get(one);
    console.error(`${one} has no compiled definition${why ? `: ${why.how} in ${why.file}` : ''}`);
    process.exit(1);
  }
}
while (queue.length) {
  const node = queue.pop();
  for (const symbol of edges.get(node) ?? []) visitSymbol(symbol, node);
}

const functions = [...reached].filter((node) => !node.endsWith('\0<data>'));
const files = new Set([...reached].map((node) => sourceOf(node.split('\0')[0])));
console.log(`root ${rootSymbol} (${sourceOf([...reached][0].split('\0')[0])}), link set boot + shared + ${level}`);
console.log(`reaches ${functions.length} compiled functions in ${files.size} recovered files`);
const groups = new Map();
for (const [symbol, users] of missing) {
  const group = classify(symbol);
  if (!groups.has(group)) groups.set(group, []);
  groups.get(group).push({ symbol, users: [...users], file: fromFailed.get(symbol)?.file });
}
console.log(`missing ${missing.size} symbols:`);
for (const [group, list] of [...groups.entries()].sort((a, b) => b[1].length - a[1].length)) {
  console.log(`\n  ${list.length}  ${group}`);
  list.sort((a, b) => b.users.length - a.users.length);
  for (const entry of listAll ? list : list.slice(0, 15)) {
    console.log(`      ${entry.symbol}${entry.file ? `  <- ${entry.file}` : ''}  (used by ${entry.users.length} file${entry.users.length === 1 ? '' : 's'})`);
  }
}
if (listAll) {
  console.log('\nreached files:');
  for (const file of [...files].sort()) console.log(`  ${file}`);
}
