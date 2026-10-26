# VU1 packets and VIF1 DMA lifecycle

`src/rendering/vu1/` groups the packet emitters, chain submission and completion
handling that share the VIF1 transfer path. It includes the existing VIF-code,
alternate GS-register and font GS-register helpers. The directory boundary does
not merge translation units or change their compiler routes.

## Flow and ownership

- `vu1_init_chain` selects the chain start/end addresses, initializes the packet
  cursor and derives the capacity limits from the current chain size.
- `vu1_add_data_ref`, `vu1_add_g_sregister`, `vu1_set_scissor`,
  `vu1_gs_regs_normal` and `vu1_tex_flush` append DMA/GIF packet words or
  references through the existing cursor views. Scissor emits its own register
  packet; it does not call `vu1_add_g_sregister`.
- `vu1_chain.c` owns both `vu1_send_chain` and `vu1_sync_chain`. Submission sets
  the busy mask, checks the chain size, appends the terminating tag, flushes the
  cache and sends through DMA channel 1. Synchronization polls the selected busy
  bits and retains the existing timeout/reset path.
- `dmac_vif1_enable` installs the VIF1 and diagnostic handlers and enables the
  channel interrupt; `dmac_vif1_disable` removes them and clears their IDs.
- `vif1_dma_interrupt_handler` clears busy bits according to the DMA tag, or the
  entire chain mask at termination, and retains the channel restart and CPU
  synchronization operations.

The diagnostic callback and graphics reset implementation remain in their
existing modules. This grouping documents the recovered code's boundaries;
it is not new PS2 runtime or native gameplay validation.

## Preservation constraints

`vif_chain_flags` is defined before the functions in `vu1_chain.c`, under the
canonical label `D_00160EE0`. Its `SDATA_OVERLAYS` owner in `configure.py` points
to `rendering/vu1/vu1_chain`; its address, file offset and four-byte extent stay
unchanged. Moving that key with the source is necessary to preserve data
placement.

Keep the cursor's `MACRO_ADDR` and store aliases, volatile accesses, signedness,
old-style SDK declarations, function aliases and local type views unchanged.
In particular, the scissor expression retains its final unsigned term and the
GS-register emitter retains its repeated cursor loads. The interrupt handler's
kernel macros remain in their original order. These files are organized by
ownership without introducing a common prototype header or rewriting packets.

The boot YAML controls link order. Its owner paths and the matching executable
owners in the overlay catalogue move together; addresses, ordering, fingerprints
and catalogue placements do not change. The standard overlay gate does not
independently qualify those executable-owner copies.
