// Walks one frame's VU1 display list: the DMA chain the game hands to VIF1 (sceDmaSend from
// vu1_send_chain_render_buffer, address recorded by services.c) and the VIF code stream it
// carries. A reader, not a renderer: it returns the stream's parts in order so a renderer can
// take them (DIRECT GIF packets for the GS, UNPACK data and MSCAL starts for VU1 programs).
//
// DMA source chain (EE manual, ch. 8): a 128-bit tag { qwc 0-15, id 28-30, addr 32-62 }.
//   0 refe: qwc at addr, then end   1 cnt: qwc after the tag, next tag after them
//   2 next: qwc after the tag, next tag at addr   3 ref: qwc at addr, next tag after the tag
//   4 refs: as ref   5 call: qwc after the tag, push, next tag at addr
//   6 ret: qwc after the tag, pop (end when the stack is empty)   7 end: qwc after the tag, end
// vu1_send_chain_render_buffer ORs 0xC0 into CHCR (TTE and TIE), so each tag's upper 64 bits
// (two VIF codes) go into the stream ahead of its data.
// VIF codes (VIF manual): cmd 24-30, num 16-23, imm 0-15; DIRECT/DIRECTHL carry imm quadwords
// (0 = 65536) of GIF data; UNPACK carries packed vectors (size from vn, vl and num); MPG carries
// num * 8 bytes of microcode; STMASK one word, STROW/STCOL four.

const VIF = {
  0x00: 'NOP', 0x01: 'STCYCL', 0x02: 'OFFSET', 0x03: 'BASE', 0x04: 'ITOP', 0x05: 'STMOD',
  0x06: 'MSKPATH3', 0x07: 'MARK', 0x10: 'FLUSHE', 0x11: 'FLUSH', 0x13: 'FLUSHA', 0x14: 'MSCAL',
  0x15: 'MSCALF', 0x17: 'MSCNT', 0x20: 'STMASK', 0x30: 'STROW', 0x31: 'STCOL', 0x4A: 'MPG',
  0x50: 'DIRECT', 0x51: 'DIRECTHL',
};

// The VIF stream bytes of the chain starting at `start`, in transfer order.
export function readChain(bytes, start, { maxTags = 200000, tte = true } = {}) {
  const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
  const parts = [];
  const stack = [];
  let at = start >>> 0;
  for (let tags = 0; tags < maxTags; tags += 1) {
    if (at + 16 > bytes.length) throw new Error(`DMA tag at 0x${at.toString(16)} is outside memory`);
    const lo = view.getUint32(at, true), hi = view.getUint32(at + 4, true);
    const qwc = lo & 0xFFFF, id = (lo >>> 28) & 7;
    // ADDR bit 31 (SPR) selects the scratchpad, mapped at 0xF0000 here (normalize-sources.py).
    const addr = (hi & 0x80000000) ? 0xF0000 + (hi & 0x3FF0) : hi & 0x7FFFFFF0;
    // TTE: the tag's upper 64 bits go to VIF1 as two VIF codes. They are kept in place of a
    // whole quadword (the lower half as zero words, which are NOPs) so the data that follows
    // stays quadword-aligned, as VIF1 sees it (DIRECT and MPG data start on such boundaries).
    if (tte) parts.push(new Uint8Array(8), bytes.subarray(at + 8, at + 16)); // GIF (PATH3) chains carry no tag
    const inline = at + 16;
    const take = (from) => { if (qwc) parts.push(bytes.subarray(from, from + qwc * 16)); };
    switch (id) {
      case 0: take(addr); return concat(parts);
      case 1: take(inline); at = inline + qwc * 16; break;
      case 2: take(inline); at = addr; break;
      case 3: case 4: take(addr); at = inline; break;
      case 5: take(inline); stack.push(inline + qwc * 16); at = addr; break;
      case 6: take(inline); if (!stack.length) return concat(parts); at = stack.pop(); break;
      case 7: take(inline); return concat(parts);
      default: throw new Error(`DMA tag id ${id}`);
    }
  }
  throw new Error('DMA chain did not end');
}

function concat(parts) {
  const total = parts.reduce((sum, part) => sum + part.length, 0);
  const out = new Uint8Array(total);
  let at = 0;
  for (const part of parts) { out.set(part, at); at += part.length; }
  return out;
}

function unpackBytes(cmd, num) {
  const vn = (cmd >> 2) & 3, vl = cmd & 3;
  const elementBits = [32, 16, 8, 5][vl];
  const perVector = vl === 3 ? 16 : elementBits * (vn + 1);
  return Math.ceil((perVector * (num || 256)) / 32) * 4;
}

// Parses a VIF stream into { code, name, num, imm, data } records (data: the payload bytes).
export function parseVif(stream) {
  const view = new DataView(stream.buffer, stream.byteOffset, stream.byteLength);
  const out = [];
  let at = 0;
  while (at + 4 <= stream.length) {
    const word = view.getUint32(at, true);
    const cmd = (word >>> 24) & 0x7F, num = (word >>> 16) & 0xFF, imm = word & 0xFFFF;
    at += 4;
    let size = 0;
    if (cmd === 0x20) size = 4;
    else if (cmd === 0x30 || cmd === 0x31) size = 16;
    else if (cmd === 0x4A) size = (num || 256) * 8;
    else if (cmd === 0x50 || cmd === 0x51) { at = (at + 15) & ~15; size = (imm || 65536) * 16; }
    else if (cmd >= 0x60) size = unpackBytes(cmd, num);
    const name = cmd >= 0x60 ? 'UNPACK' : (VIF[cmd] ?? `?${cmd.toString(16)}`);
    out.push({ code: word, name, num, imm, data: stream.subarray(at, at + size) });
    at += size;
  }
  return out;
}

export function summarize(records) {
  const counts = {};
  let directBytes = 0;
  for (const record of records) {
    counts[record.name] = (counts[record.name] ?? 0) + 1;
    if (record.name === 'DIRECT' || record.name === 'DIRECTHL') directBytes += record.data.length;
  }
  return { counts, directBytes };
}
